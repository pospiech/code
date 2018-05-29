#include "cameraximea.h"

#include "xiApi.h"
#include <memory.h>
#include <QtDebug>
#include <QtCore/QRect>
#include <QString>
#include <QImage>
#include <QMutex>
#include <QMutexLocker>

class CameraXimeaPrivate
{
public:
    CameraXimeaPrivate()
    {
        memset(&image,0,sizeof(image));
        image.size = sizeof(XI_IMG);

        numberOfDevices();
    }

    QImage lastImage;
    QMutex mutex;

    // structure containing information about incoming image.
    // -> does not contain image, only pointer to image
    XI_IMG image;

    // windows handle
    HANDLE xiHandle = NULL;

    // camera number and count
    size_t cameraNumber = 0;
    size_t cameraCount = 0;

    /** convert raw memory data to QImage format */
    QImage toQImage(void * pMemVoid, size_t size, int format, int sizeX, int sizeY)
    {

        // Determine Image Format
        QImage::Format qFormat = QImage::Format_Invalid;
        switch (static_cast<XI_IMG_FORMAT>(format))
        {
        case XI_MONO8: // 8 bits per pixel
        case XI_MONO16: // 16 bits per pixel
              qFormat = QImage::Format_Mono;
        }
        if (qFormat == QImage::Format_Invalid)
            qFatal("invalid image Format");

        // convert / copy pointer data into vector
        std::vector<int> grayVector(size);
        if (static_cast<XI_IMG_FORMAT>(format) == XI_MONO16)
        {
            qint16* imageIterator = reinterpret_cast<qint16*> (pMemVoid);
            for (size_t count = 0; count < size; ++count)
            {
                grayVector[count] = static_cast<int>(*imageIterator);
                imageIterator++;
            }
        }

        // convert gray values into QImage data
        QImage image = QImage(static_cast<int>(sizeX), static_cast<int>(sizeY), qFormat);
        for ( int y = 0; y < sizeY; ++y )
        {
            int yoffset = sizeY*y;
            QRgb *line = reinterpret_cast<QRgb *>(image.scanLine(y)) ;
            for ( int x = 0; x < sizeX  ; ++x )
            {
                int pos = x + yoffset;
                int color = grayVector[static_cast<size_t>(pos)];
                *line++ = qRgb(color, color, color);
            }
        }
        return image;
    }

    /** update image data with next image */
    void updateImageData(QImage& nextImage)
    {
        QMutexLocker locker(&mutex);
        this->lastImage = nextImage;
    }


    /** handle camera error messages */
    void errorHandler(int retCode) const
    {
        if (retCode != XI_OK)
        {
            qDebug() << "Ximea Error Code: " << retCode << endl;
        }
    }

    /** stores camera Number for futher usage
     */
    void setCameraNumber(size_t cameraNumber)
    {
        if (cameraNumber <= this->cameraCount) {
            this->cameraNumber = cameraNumber;
        }
        else {
            qDebug() << "camera number requested higher than maximum attached camera count";
        }

    }

    /** This function initializes the device and returns a device handle.
     */
    void openCamera(size_t cameraNumber)
    {
        XI_RETURN stat = XI_OK;
        stat = xiOpenDevice(static_cast<DWORD>(cameraNumber), &xiHandle);
        errorHandler(stat);
    }

    /** This function will un-initialize the specified device,
     * closes its handle and releases allocated resources.
     */
    void closeCamera()
    {
        XI_RETURN stat = XI_OK;
        stat = xiCloseDevice(xiHandle);
        errorHandler(stat);
    }

    /** This function enumerates all devices connected and returns the number of discovered devices.
     * It is needed to be called before any other function of API is called by application.
     */
    size_t numberOfDevices()
    {
        PDWORD pNumberDevices = nullptr;
        XI_RETURN stat = xiGetNumberDevices(pNumberDevices);
        errorHandler(stat);

        // in case of error, pointer is 0
        size_t count = 0;
        if (pNumberDevices)
            count = static_cast<size_t>(*pNumberDevices);

        this->cameraCount = count;

        return count;
    }

    /** This function starts the data acquisition on the devices specified by the handle. */
    void startAcquisition()
    {
       XI_RETURN stat = xiStartAcquisition(xiHandle);
       errorHandler(stat);
    }

    /** Ends the work cycle of the camera, stops data acquisition
     * and deallocates internal image buffers.
     */
    void stopAcquisition()
    {
       XI_RETURN stat = xiStopAcquisition(xiHandle);
       errorHandler(stat);
    }

    /** This function waits for next image is available at transport buffer.
     * If available - it does all required image processing
     * (unpack, sensor-defect-correction, demosaic) and fills image information
     * to structure at img parameter.
     */
    void getImage()
    {
        const DWORD TimeOut = 1000; // time in ms
        XI_RETURN stat = xiGetImage(xiHandle, TimeOut, &image);
        errorHandler(stat);
    }

    /** Sets exposure time in microseconds. */
    void setExposure(float exposure_ms)
    {
        // Setting "exposure" parameter (10ms=10000us)
        XI_RETURN stat = xiSetParamInt(xiHandle, XI_PRM_EXPOSURE, static_cast<int>(1000 * exposure_ms));
        errorHandler(stat);
    }

    /** Get exposure time in microseconds. */
    float exposure() const
    {
        int value;
        XI_RETURN stat = xiGetParamInt(xiHandle, XI_PRM_EXPOSURE, &value);
        errorHandler(stat);
        return value / 1000; // us -> ms
    }

    /** Sets gain in dB. */
    void setGain(float gain_in_db)
    {
        XI_RETURN stat = xiSetParamFloat(xiHandle, XI_PRM_GAIN, gain_in_db );
        errorHandler(stat);
    }

    /** gain in dB. */
    float gain()
    {
        float gain_in_db;
        XI_RETURN stat = xiGetParamFloat(xiHandle, XI_PRM_GAIN, &gain_in_db );
        errorHandler(stat);
        return gain_in_db;
    }


    void configureBinning(bool enableBinning, Qt::Orientation orientation, int binningPixels = 2)
    {
        if (enableBinning) {
            const char * binnMode;
            const char * binnOrient;
            if (orientation == Qt::Horizontal){
                binnMode = XI_PRM_BINNING_HORIZONTAL_MODE;
                binnOrient = XI_PRM_BINNING_HORIZONTAL;
            } else {
                binnMode = XI_PRM_BINNING_VERTICAL_MODE;
                binnOrient = XI_PRM_BINNING_VERTICAL;
            }
            //  Sets the mode to use to combine vertical pixel together.
            // XI_BIN_MODE_SUM     0	increased sensitivity.
            // XI_BIN_MODE_AVERAGE	1	increased signal/noise ratio.
            errorHandler(xiSetParamInt(xiHandle, binnMode, XI_BIN_MODE_AVERAGE));

            // Defines number of vertical photo-sensitive cells to combine.            
            errorHandler(xiSetParamInt(xiHandle, binnOrient, binningPixels));
        }
    }

    /** Format of image data returned by function xiGetImage.
     *  In order to simplify the control of the camera from application
     * - the xiAPI automatically changes selected camera parameters and Image
     */
    void setImageDataFormat(int formatCode = XI_MONO16)
    {
        // XI_MONO8
        // XI_MONO16
        errorHandler(xiSetParamInt(xiHandle, XI_PRM_IMAGE_DATA_FORMAT,
                                   static_cast<XI_IMG_FORMAT>(formatCode)));
    }

    /** set ROI using offets and width and height */
    void setROI(QRect roi)
    {
        errorHandler(xiSetParamInt(xiHandle, XI_PRM_OFFSET_X, roi.x()));
        errorHandler(xiSetParamInt(xiHandle, XI_PRM_OFFSET_Y, roi.y()));
        errorHandler(xiSetParamInt(xiHandle, XI_PRM_WIDTH, roi.width()));
        errorHandler(xiSetParamInt(xiHandle, XI_PRM_HEIGHT, roi.height()));
    }

    /** get ROI using offets and width and height */
    QRect roi() const
    {
        QRect roi;
        int value;
        errorHandler(xiGetParamInt(xiHandle, XI_PRM_OFFSET_X, &value));
        roi.setX(value);
        errorHandler(xiGetParamInt(xiHandle, XI_PRM_OFFSET_Y, &value));
        roi.setY(value);
        errorHandler(xiGetParamInt(xiHandle, XI_PRM_WIDTH, &value));
        roi.setWidth(value);
        errorHandler(xiGetParamInt(xiHandle, XI_PRM_HEIGHT, &value));
        roi.setHeight(value);
        return roi;
    }

    /** This function returns selected parameter of camera without opening it.
     * It allows to quickly get information from each camera in multiple camera setups.
     */
    QString deviceInfo(size_t DevId=0, const char* parameter = XI_PRM_DEVICE_SN)
    {
        //        Note: This function is capable to return only limited set of parameters:        
        // XI_PRM_DEVICE_SN:
        // XI_PRM_DEVICE_NAME:
        // XI_PRM_DEVICE_INSTANCE_PATH:
        // XI_PRM_DEVICE_LOCATION_PATH:
        // XI_PRM_DEVICE_TYPE:

        char value[100];
        DWORD size = 100;

        XI_RETURN stat =
                xiGetDeviceInfoString(
                    static_cast<DWORD>(DevId), // index of the device
                    parameter,                 // parameter name string
                    value,                     // pointer to result string
                    size);                     // size of string

        errorHandler(stat);

        return QString::fromLocal8Bit(value, static_cast<int>(size));
    }

    /** get parameters of camera, results are in QMap */
    QMap<QString, QString> getParamters()
    {
        QMap<QString, QString> mapParam;
        try{
            //!  Returns the device model id.
            // device model id as number
            int model_id;
            errorHandler(xiGetParamInt(xiHandle, XI_PRM_DEVICE_MODEL_ID, &model_id));
            mapParam.insert("ModelID", QString().arg(model_id));

            //! Returns the device sensor model id.
            // device sensor id as number
            int sensor_id;
            errorHandler(xiGetParamInt(xiHandle, XI_PRM_SENSOR_MODEL_ID, &sensor_id));
            mapParam.insert("SensorID", QString().arg(sensor_id));

            //! Returns device serial number. Only string form is possible.
            //! It might contain also alphabet characters.
            char sn[20];
            errorHandler(xiGetParamString(xiHandle, XI_PRM_DEVICE_SN, sn, sizeof(sn)));
            mapParam.insert("DeviceSN", QString(sn));
        }
        catch(const std::exception& e)
        {
            qDebug() << "Error:" << e.what();
        }
        return mapParam;
    }

    //! Setting the API debug level allows to select amount of messages stored to debug output.
    void setDebugLevel(XI_DEBUG_LEVEL level)
    {
//        type	representing value	result
//        XI_DL_DETAIL	0	Prints same as XI_DL_TRACE plus locking of resources
//        XI_DL_TRACE     1	Prints errors, warnings and important informations
//        XI_DL_WARNING	2	Prints all errors and warnings
//        XI_DL_ERROR     3	Prints all errors
//        XI_DL_FATAL     4	Prints only important errors
//        XI_DL_DISABLED	100	Prints no messages
        errorHandler(xiSetParamInt(xiHandle, XI_PRM_DEBUG_LEVEL, level));
    }
};




CameraXimea::CameraXimea()
    : d_ptr(new CameraXimeaPrivate)
{

}

CameraXimea::~CameraXimea()
{
    // for QScopedPointer required.
}

/** Initialize - find number of devices and return first camera */
bool CameraXimea::Initialize()
{
    Q_D(CameraXimea);
    size_t count = d->cameraCount;
    if (count > 0){
        d->setCameraNumber(1);
        return true;
    }
    else{
        return false;
    }

}

void CameraXimea::openCamera()
{
    Q_D(CameraXimea);
    d->openCamera(d->cameraNumber);
}

void CameraXimea::closeCamera()
{
    Q_D(CameraXimea);
    d->closeCamera();
}

float CameraXimea::exposure() const
{
    Q_D(const CameraXimea);
    return d->exposure();
}

void CameraXimea::setExposure(float exposure_ms)
{
    Q_D(CameraXimea);
    d->setExposure(exposure_ms);
}

QRect CameraXimea::roi() const
{
    Q_D(const CameraXimea);

}

void CameraXimea::setROI(QRect roi)
{
    Q_D(CameraXimea);
    d->setROI(roi);
}

void CameraXimea::capture()
{
    Q_D(CameraXimea);
    d->startAcquisition();
    d->getImage();
    d->stopAcquisition();
    int sizeX = static_cast<int>(d->image.width);
    int sizeY = static_cast<int>(d->image.height);

    XI_IMG_FORMAT format = d->image.frm;

    QImage image = d->toQImage(d->image.bp, d->image.bp_size, format, sizeX, sizeY);
    d->updateImageData(image);
}


QImage CameraXimea::image()
{
    Q_D(CameraXimea);

    QMutexLocker locker(&d->mutex);
    return d->lastImage;
}


