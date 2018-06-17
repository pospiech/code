#include "cameraximea.h"

#include "xiApi.h"
#include <memory.h>
#include <QtDebug>
#include <QtCore/QRect>
#include <QString>
#include <QImage>
#include <QMutex>
#include <QMutexLocker>


static QVector<QRgb> grayScaleColorTable(int colorcount = 256) {
  static QVector<QRgb> table;
  if (table.isEmpty()) {
    Q_ASSERT(colorcount >0 && colorcount < 65537);
    table.resize(colorcount);
    auto *data = table.data();
    for (int i = 0; i < table.size(); ++i)
      data[i] = qRgb(i, i, i);
  }
  return table;
}

class CameraXimeaPrivate
{
public:
    CameraXimeaPrivate()
    {
        memset(&image,0,sizeof(image));
        image.size = sizeof(XI_IMG);
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

    /** convert raw memory data to QImage format
     * by Kuba Ober
     * https://stackoverflow.com/questions/50877195/how-to-convert-ximea-xiapi-camera-data-into-qimage
     */
    QImage fromXiImg(const XI_IMG *src, QImage::Format dstFormat = QImage::Format_ARGB32_Premultiplied) {
      // make sure all relevant parameters are correct
      Q_ASSERT(src->width > 0 && src->height > 0 && src->padding_x >= 0 && src->bp_size > 0);
      // destiny format must be valid
      Q_ASSERT(dstFormat != QImage::Format_Invalid);

      // settings
      bool swap = false; // apply rgb swap (if source is swapped)

      QVector<QRgb> colorTable;

      // determine number of Bytes in source image
      // set equilvalent QImage format.
      int srcPixelBytes = 0;
      QImage::Format srcFormat = QImage::Format_Invalid;
      switch (src->frm) {
      case XI_MONO8: // 8 bits per pixel
        srcPixelBytes = 1;
        if (QT_VERSION >= QT_VERSION_CHECK(5,5,0))
          srcFormat = QImage::Format_Grayscale8;
        else {
          colorTable = grayScaleColorTable();
          srcFormat = QImage::Format_Indexed8;
        }
        break;
      case XI_MONO16: // 16 bits per pixel
          srcPixelBytes = 2;
          colorTable = grayScaleColorTable(65536);
          srcFormat = QImage::Format_RGB32;
          break;
      case XI_RGB24:
        srcPixelBytes = 3;
        srcFormat = QImage::Format_RGB888;
        break;
      case XI_RGB32:
        srcPixelBytes = 4;
        srcFormat = QImage::Format_RGB32;
        break;
      }

      // handle error -> no format selected
      if (srcFormat == QImage::Format_Invalid) {
        qWarning("Unhandled XI_IMG image format");
        return {};
      }


      // determine bytesPerLine = width * bytes + padding_x
      Q_ASSERT(srcPixelBytes > 0 && srcPixelBytes <= 4);
      int bytesPerLine = src->width * srcPixelBytes + src->padding_x;
      if ((bytesPerLine * src->height - src->padding_x) > src->bp_size) {
        qWarning("Inconsistent XI_IMG data");
        return {};
      }

      // Constructs an image with the given width, height and format, that uses an existing memory buffer
      // in this case the XI src->bp, which is cast to a usable QImage format.
      QImage ret{static_cast<const uchar*>(src->bp), src->width, src->height,
                 bytesPerLine, srcFormat};

      // The QImage is only filled with an indexed format,
      // no information how to color these, therefore a color table needs to be set
      if (!colorTable.isEmpty())
        ret.setColorTable(colorTable);

      // convert to destiny format if necessary
      if (srcFormat != dstFormat)
        ret = std::move(ret).convertToFormat(dstFormat);

      // swap RGB if set
      if (swap)
        ret = std::move(ret).rgbSwapped();

      if (!ret.isDetached()) // ensure that we don't share XI_IMG's data buffer
        ret.detach();

      return ret;
    }

    /** convert raw memory data to QImage format */
    QImage toQImage(void * pMemVoid, size_t size, int format, int sizeX, int sizeY)
    {
        // test
        size = sizeX*sizeY;

        std::vector<int> grayVector(size);
        // convert / copy pointer data into vector: 8 bit
        if (static_cast<XI_IMG_FORMAT>(format) == XI_MONO8)
        {
            quint8* imageIterator = reinterpret_cast<quint8*> (pMemVoid);
            for (size_t count = 0; count < size; ++count)
            {
                grayVector[count] = static_cast<int>(*imageIterator);
                imageIterator++;
            }
        }
        // convert / copy pointer data into vector: 16 bit
        if (static_cast<XI_IMG_FORMAT>(format) == XI_MONO16)
        {
            quint16* imageIterator = reinterpret_cast<quint16*> (pMemVoid);
            for (size_t count = 0; count < size; ++count)
            {
                grayVector[count] = static_cast<int>(*imageIterator);
                imageIterator++;
            }
        }


        // Determine corresponding Qt Image Format
        QImage::Format qFormat = QImage::Format_Invalid;
        switch (static_cast<XI_IMG_FORMAT>(format))
        {
        case XI_MONO8: // 8 bits per pixel
        case XI_MONO16: // 16 bits per pixel
              qFormat = QImage::Format_Mono;
        }
        if (qFormat == QImage::Format_Invalid)
            qFatal("invalid image Format");

        // convert gray values into QImage data
        QImage image = QImage(static_cast<int>(sizeX), static_cast<int>(sizeY), QImage::Format_RGB32);
        for ( int y = 0; y < sizeY; ++y )
        {
            int yoffset = sizeY*y;
            QRgb *line = reinterpret_cast<QRgb *>(image.scanLine(y)) ;
            for ( int x = 0; x < sizeX  ; ++x )
            {
                int pos = x + yoffset;
                int color = grayVector[static_cast<size_t>(pos)] * 20;
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
            QString errString;
            switch (retCode)
            {
            //case XI_OK                : errString = "Function call succeeded"; break;
            case XI_INVALID_HANDLE    : errString = "Invalid handle"; break;
            case XI_READREG           : errString = "Register read error"; break;
            case XI_WRITEREG          : errString = "Register write error"; break;
            case XI_FREE_RESOURCES    : errString = "Freeing resources error"; break;
            case XI_FREE_CHANNEL      : errString = "Freeing channel error"; break;
            case XI_FREE_BANDWIDTH    : errString = "Freeing bandwith error"; break;
            case XI_READBLK           : errString = "Read block error"; break;
            case XI_WRITEBLK          : errString = "Write block error"; break;
            case XI_NO_IMAGE          : errString = "No image"; break;
            case XI_TIMEOUT           : errString = "Timeout"; break;
            case XI_INVALID_ARG            : errString = "Invalid arguments supplied"; break;
            case XI_NOT_SUPPORTED          : errString = "Not supported"; break;
            case XI_ISOCH_ATTACH_BUFFERS   : errString = "Attach buffers error"; break;
            case XI_GET_OVERLAPPED_RESULT  : errString = "Overlapped result"; break;
            case XI_MEMORY_ALLOCATION      : errString = "Memory allocation error"; break;
            case XI_DLLCONTEXTISNULL       : errString = "DLL context is NULL"; break;
            case XI_DLLCONTEXTISNONZERO    : errString = "DLL context is non zero"; break;
            case XI_DLLCONTEXTEXIST        : errString = "DLL context exists"; break;
            case XI_TOOMANYDEVICES         : errString = "Too many devices connected"; break;
            case XI_ERRORCAMCONTEXT        : errString = "Camera context error"; break;
            case XI_UNKNOWN_HARDWARE       : errString = "Unknown hardware"; break;
            case XI_INVALID_TM_FILE        : errString = "Invalid TM file"; break;
            case XI_INVALID_TM_TAG         : errString = "Invalid TM tag"; break;
            case XI_INCOMPLETE_TM          : errString = "Incomplete TM"; break;
            case XI_BUS_RESET_FAILED       : errString = "Bus reset error"; break;
            case XI_NOT_IMPLEMENTED        : errString = "Not implemented"; break;
            case XI_SHADING_TOOBRIGHT      : errString = "Shading is too bright"; break;
            case XI_SHADING_TOODARK        : errString = "Shading is too dark"; break;
            case XI_TOO_LOW_GAIN           : errString = "Gain is too low"; break;
            case XI_INVALID_BPL            : errString = "Invalid bad pixel list"; break;
            case XI_BPL_REALLOC            : errString = "Bad pixel list realloc error"; break;
            case XI_INVALID_PIXEL_LIST     : errString = "Invalid pixel list"; break;
            case XI_INVALID_FFS            : errString = "Invalid Flash File System"; break;
            case XI_INVALID_PROFILE        : errString = "Invalid profile"; break;
            case XI_INVALID_CALIBRATION    : errString = "Invalid calibration"; break;
            case XI_INVALID_BUFFER         : errString = "Invalid buffer"; break;
            case XI_INVALID_DATA           : errString = "Invalid data"; break;
            case XI_TGBUSY                 : errString = "Timing generator is busy"; break;
            case XI_IO_WRONG               : errString = "Wrong operation open/write/read/close"; break;
            case XI_ACQUISITION_ALREADY_UP : errString = "Acquisition already started"; break;
            case XI_OLD_DRIVER_VERSION     : errString = "Old version of device driver installed to the system."; break;
            case XI_GET_LAST_ERROR         : errString = "To get error code please call GetLastError function."; break;
            case XI_CANT_PROCESS           : errString = "Data cannot be processed"; break;
            case XI_ACQUISITION_STOPED     : errString = "Acquisition is stopped. It needs to be started to perform operation."; break;
            case XI_ACQUISITION_STOPED_WERR: errString = "Acquisition has been stopped with an error."; break;
            case XI_INVALID_INPUT_ICC_PROFILE      : errString = "Input ICC profile missing or corrupted"; break;
            case XI_INVALID_OUTPUT_ICC_PROFILE     : errString = "Output ICC profile missing or corrupted"; break;
            case XI_DEVICE_NOT_READY       : errString = "Device not ready to operate"; break;
            case XI_SHADING_TOOCONTRAST    : errString = "Shading is too contrast"; break;
            case XI_ALREADY_INITIALIZED    : errString = "Module already initialized"; break;
            case XI_NOT_ENOUGH_PRIVILEGES  : errString = "Application does not have enough privileges (one or more app)"; break;
            case XI_NOT_COMPATIBLE_DRIVER  : errString = "Installed driver is not compatible with current software"; break;
            case XI_TM_INVALID_RESOURCE    : errString = "TM file was not loaded successfully from resources"; break;
            case XI_DEVICE_HAS_BEEN_RESETED: errString = "Device has been reset, abnormal initial state"; break;
            case XI_NO_DEVICES_FOUND       : errString = "No Devices Found"; break;
            case XI_RESOURCE_OR_FUNCTION_LOCKED    : errString = "Resource (device) or function locked by mutex"; break;
            case XI_BUFFER_SIZE_TOO_SMALL  : errString = "Buffer provided by user is too small"; break;
            case XI_COULDNT_INIT_PROCESSOR : errString = "Couldnt initialize processor."; break;
            case XI_NOT_INITIALIZED        : errString = "The object/module/procedure/process being referred to has not been started."; break;
            case XI_RESOURCE_NOT_FOUND     : errString = "Resource not found(could be processor, file, item...)."; break;
            case XI_UNKNOWN_PARAM          : errString = "Unknown parameter"; break;
            case XI_WRONG_PARAM_VALUE      : errString = "Wrong parameter value"; break;
            case XI_WRONG_PARAM_TYPE       : errString = "Wrong parameter type"; break;
            case XI_WRONG_PARAM_SIZE       : errString = "Wrong parameter size"; break;
            case XI_BUFFER_TOO_SMALL       : errString = "Input buffer is too small"; break;
            case XI_NOT_SUPPORTED_PARAM    : errString = "Parameter is not supported"; break;
            case XI_NOT_SUPPORTED_PARAM_INFO       : errString = "Parameter info not supported"; break;
            case XI_NOT_SUPPORTED_DATA_FORMAT      : errString = "Data format is not supported"; break;
            case XI_READ_ONLY_PARAM        : errString = "Read only parameter"; break;
            case XI_BANDWIDTH_NOT_SUPPORTED: errString = "This camera does not support currently available bandwidth"; break;
            case XI_INVALID_FFS_FILE_NAME  : errString = "FFS file selector is invalid or NULL"; break;
            case XI_FFS_FILE_NOT_FOUND     : errString = "FFS file not found"; break;
            case XI_PARAM_NOT_SETTABLE     : errString = "Parameter value cannot be set (might be out of range or invalid)."; break;
            case XI_SAFE_POLICY_NOT_SUPPORTED      : errString = "Safe buffer policy is not supported. E.g. when transport target is set to GPU (GPUDirect)."; break;
            case XI_GPUDIRECT_NOT_AVAILABLE: errString = "GPUDirect is not available. E.g. platform isn't supported or CUDA toolkit isn't installed."; break;
            case XI_PROC_OTHER_ERROR       : errString = "Processing error - other"; break;
            case XI_PROC_PROCESSING_ERROR  : errString = "Error while image processing."; break;
            case XI_PROC_INPUT_FORMAT_UNSUPPORTED  : errString = "Input format is not supported for processing."; break;
            case XI_PROC_OUTPUT_FORMAT_UNSUPPORTED : errString = "Output format is not supported for processing."; break;
            case XI_OUT_OF_RANGE           : errString = "Parameter value is out of range"; break;
            }

            qDebug() << "CameraViewer: Ximea Error Code: "<< retCode << errString << endl;
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
        DWORD NumberDevices;
        XI_RETURN stat = xiGetNumberDevices(&NumberDevices);
        errorHandler(stat);

        size_t count = static_cast<size_t>(NumberDevices);

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
bool CameraXimea::initialize()
{
    Q_D(CameraXimea);
    size_t count = d->numberOfDevices();

    if (count > 0){
        d->setCameraNumber(0); // numbers start at 0
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
    // todo: add get roi
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


