#include "cameraximea.h"

#include "xiApi.h"
#include <memory.h>
#include <QtDebug>
#include <QtCore/QRect>

class CameraXimeaPrivate
{
    CameraXimeaPrivate()
    {
        memset(&image,0,sizeof(image));
        image.size = sizeof(XI_IMG);
    }

    // structure containing information about incoming image.
    // -> does not contain image, only pointer to image
    XI_IMG image;

    // windows handle
    HANDLE xiH = NULL;

    /** handle camera error messages */
    void errorHandler(int retCode)
    {
        if (retCode != XI_OK)
        {
            qDebug() << "Ximea Error Code: " << retCode << endl;
        }
    }


    /** This function initializes the device and returns a device handle.
     */
    void openCamera()
    {
        XI_RETURN stat = XI_OK;
        stat = xiOpenDevice(0, &xiH);
        errorHandler(stat);
    }

    /** This function will un-initialize the specified device,
     * closes its handle and releases allocated resources.
     */
    void closeCamera()
    {
        XI_RETURN stat = XI_OK;
        stat = xiCloseDevice(xiH);
        errorHandler(stat);
    }

    /** This function enumerates all devices connected and returns the number of discovered devices.
     * It is needed to be called before any other function of API is called by application.
     */
    int numberOfDevices()
    {
        PDWORD pNumberDevices;
        XI_RETURN stat = xiGetNumberDevices(pNumberDevices);
        errorHandler(stat);
        int count = static_cast<int>(*pNumberDevices);
        return count;
    }

    /** This function starts the data acquisition on the devices specified by the handle. */
    void startAcquisition()
    {
       XI_RETURN stat = xiStartAcquisition(xiH);
       errorHandler(stat);
    }

    /** Ends the work cycle of the camera, stops data acquisition
     * and deallocates internal image buffers.
     */
    void stopAcquisition()
    {
       XI_RETURN stat = xiStopAcquisition(xiH);
       errorHandler(stat);
    }

    /** This function waits for next image is available at transport buffer.
     * If available - it does all required image processing
     * (unpack, sensor-defect-correction, demosaic) and fills image information
     * to structure at img parameter.
     */
    void getImage()
    {
        const DWORD TimeOut = 5000;
        XI_RETURN stat = xiGetImage(xiH, TimeOut, &image);
        errorHandler(stat);
    }

    /** This function returns selected parameter of camera without opening it.
     * It allows to quickly get information from each camera in multiple camera setups.
     */
    void deviceInfo(size_t DevId=0)
    {
        const char* prm;
        void* val;
        DWORD * size;
        XI_PRM_TYPE * type;

        XI_RETURN stat =
                xiGetDeviceInfoString(
                    static_cast<DWORD>(DevId), // index of the device
                    prm,                       // parameter name string
                    value,                     // pointer to result string
                    size);                     // size of string

        errorHandler(stat);

//        Note: This function is capable to return only limited set of parameters:
//        XI_PRM_DEVICE_SN
//        XI_PRM_DEVICE_NAME
//        XI_PRM_DEVICE_INSTANCE_PATH
//        XI_PRM_DEVICE_LOCATION_PATH
//        XI_PRM_DEVICE_TYPE
    }

    /** Sets exposure time in microseconds. */
    void setExposure(sizt_t exposure_ms)
    {
        // Setting "exposure" parameter (10ms=10000us)
        stat = xiSetParamInt(xiH, XI_PRM_EXPOSURE, 1000*static_cast<int>(exposure_ms));
        errorHandler(stat);
    }

    /** Sets gain in dB. */
    void setGain(float gain_in_db)
    {
        errorHandler(xiSetParamFloat(xiH, XI_PRM_GAIN, gain_in_db ));
    }

    void configureBinning(bool enableBinning, Qt::Orientation orientation, int binningPixels)
    {
        if (enableBinning) {
            char * binnMode;
            char * binnOrient;
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
            errorHandler(xiSetParamInt(xiH, XI_PRM_BINNING_VERTICAL_MODE, XI_BIN_MODE_AVERAGE));

            // Defines number of vertical photo-sensitive cells to combine.
            int binningPixels = 2;
            errorHandler(xiSetParamInt(xiH, XI_PRM_BINNING_VERTICAL, binningPixels));
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
        errorHandler(xiSetParamInt(xiH, XI_PRM_IMAGE_DATA_FORMAT,
                                   static_cast<XI_IMG_FORMAT>(formatCode)));
    }

    /** set ROI using offets and width and height */
    void setROI(QRect roi)
    {
        errorHandler(xiSetParamInt(xiH, XI_PRM_OFFSET_X, roi.x()));
        errorHandler(xiSetParamInt(xiH, XI_PRM_OFFSET_Y, roi.y()));
        errorHandler(xiSetParamInt(xiH, XI_PRM_WIDTH, roi.width()));
        errorHandler(xiSetParamInt(xiH, XI_PRM_HEIGHT, roi.height()));
    }

    void getParamters()
    {
        // device sensor id as number
        int sensor_id;
        //!  Returns the device model id.
        errorHandler(xiGetParamInt(handle, XI_PRM_DEVICE_MODEL_ID, &model_id));

        //! Returns the device sensor model id.
        errorHandler(xiGetParamInt(handle, XI_PRM_SENSOR_MODEL_ID, &sensor_id));

        //! Returns device serial number. Only string form is possible.
        //! It might contain also alphabet characters.
        char sn[20]=0;
        errorHandler(xiGetParamString(xiH, XI_PRM_DEVICE_SN, sn, sizeof(sn)));

    }

    //! Setting the API debug level allows to select amount of messages stored to debug output.
    void setDebugLevel(size_t level)
    {
//        type	representing value	result
//        XI_DL_DETAIL	0	Prints same as XI_DL_TRACE plus locking of resources
//        XI_DL_TRACE     1	Prints errors, warnings and important informations
//        XI_DL_WARNING	2	Prints all errors and warnings
//        XI_DL_ERROR     3	Prints all errors
//        XI_DL_FATAL     4	Prints only important errors
//        XI_DL_DISABLED	100	Prints no messages
        xiSetParamInt(xiH, XI_PRM_DEBUG_LEVEL, XI_DL_TRACE);
    }
};

CameraXimea::CameraXimea()
{

}
