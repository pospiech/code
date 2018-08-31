#ifndef CAMERAUEYE_P_H
#define CAMERAUEYE_P_H

#include "cameratools.h"
#include "cameraimplementation.h"

#include <uEye.h>

#include <QtDebug>
#include <QtCore/QRect>
#include <QString>


#include <stdexcept>
#include <math.h>


class CameraUEyePrivate: public CameraImplementation
{
public:
    CameraUEyePrivate()
    {
        m_hCam = 0;
        m_pcImageMemory = nullptr;
    }

    // uEye varibles
    HIDS	m_hCam;			// handle to camera
    INT		m_nColorMode;	// Y8/RGB16/RGB24/REG32
    INT		m_nBitsPerPixel;// number of bits needed store one pixel
    INT		m_lMemoryId;	// grabber memory - buffer ID
    char*	m_pcImageMemory;// grabber memory - pointer to buffer

//    QSize sizeSensor;
//    QImage lastImage;
//    QMutex mutex;

//    bool isOpen;

//    std::vector<int> grayVector;
//    std::vector<int> histVector;

    /** handle camera error messages */
    void errorHandler(int retCode) const
    {
        if (retCode != IS_SUCCESS) {
            INT Err;
            IS_CHAR* msg;
            is_GetError (m_hCam, &Err, &msg);
            LOG_WARNING() << "CameraViewer: uEye Error Code: "<< retCode << QString::fromLatin1(msg) << endl;
        }
    }

    size_t numberOfDevices()
    {
        INT nNumCam = 0;
        errorHandler(is_GetNumberOfCameras( &nNumCam ));
        return static_cast<size_t>(nNumCam);
    }




    /** convert raw memory data to QImage format */
    void createData()
    {
        // continue when we have a valid image buffer
        if( this->m_pcImageMemory != nullptr )
        {
            QTime time; time.start();

            // get pointer to active image buffer
            void *pMemVoid;
            is_GetImageMem( this->m_hCam, &pMemVoid );

            // get current image size
            const size_t sizeX = static_cast<size_t>(imageSize().width());
            const size_t sizeY = static_cast<size_t>(imageSize().height());
            const size_t size = sizeX * sizeY;

            // make sure all relevant parameters are correct
            if (!(sizeX > 0 && sizeY > 0)) {
                QString errorStr = "Parameters <width> or <height> are 0";
                LOG_FATAL() << errorStr;
                throw std::runtime_error(errorStr.toStdString());
            }


            grayVector.resize(size);
            // 1 << 8 = 256, 1 << 10 = 1024
            int bitDepth = bitsPerPixel();
            size_t histSize = 1 << bitDepth;
            histVector.clear();
            histVector.resize(histSize);


            // select convertion method:
            switch( this->colorMode() )
            {
            case IS_CM_MONO8: // 8 bit
                {
                quint8* imageIterator = reinterpret_cast<quint8*> (pMemVoid);
                for (size_t count = 0; count < size; ++count)
                {
                    const int value = static_cast<int>(*imageIterator);
                    grayVector[count] = value;
                    histVector[static_cast<size_t>(value)] += 1;
                    imageIterator++;
                }
                break;
                }
            case IS_CM_MONO10:
            case IS_CM_MONO12:
            case IS_CM_MONO16:
                {
                quint16* imageIterator = reinterpret_cast<quint16*> (pMemVoid);
                for (size_t count = 0; count < size; ++count)
                {
                    const int value = static_cast<int>(*imageIterator);
                    grayVector[count] = value;
                    histVector[static_cast<size_t>(value)] += 1;
                    imageIterator++;
                }
                break;
                }
            default:
                LOG_WARNING() << "Unhandled image format";
                return;
            } // end of switch

            LOG_INFO() << "Time: " << time.elapsed();
            LOG_INFO() << "create Data from Raw Image (finished)";
        }
    }


//    /** convert raw memory data to QImage format */
//    QImage toQImage()
//    {
//        // get current image size
//        const size_t sizeX = static_cast<size_t>(imageSize().width());
//        const size_t sizeY = static_cast<size_t>(imageSize().height());

//        auto maxValue = *max_element(std::begin(grayVector), std::end(grayVector));
//        auto minValue = *min_element(std::begin(grayVector), std::end(grayVector));
//        float maxDivider = (maxValue-minValue);

//        LOG_INFO() << "max" << maxValue;
//        LOG_INFO() << "min" << maxValue;
//        LOG_INFO() << "d" << maxValue;

//        // convert gray values into QImage data
//        QImage image = QImage(static_cast<int>(sizeX), static_cast<int>(sizeY), QImage::Format_ARGB32_Premultiplied);
//        // data[ix + iy*dataPointsY]
//        // data[iy + dataPointsY * ix]
//        for ( size_t y = 0; y < sizeY; ++y )
//        {
//            size_t yoffset = y * sizeX;
//            QRgb *line = reinterpret_cast<QRgb *>(image.scanLine(int(y))) ;
//            for ( size_t x = 0; x < sizeX  ; ++x )
//            {
//                size_t pos = x + yoffset;
//                int color = static_cast<int>((grayVector[static_cast<size_t>(pos)] - minValue) / maxDivider * 255);
//                *line++ = qRgb(color, color, color);
//            }
//        }

//        // convert to destiny format
//        // image = std::move(image).convertToFormat(dstFormat);

//        LOG_INFO() << "to QImage from data (finished)";
//        return image;
//    }

//    /** update image data with next image */
//    void updateImageData(QImage& nextImage)
//    {
//        QMutexLocker locker(&mutex);
//        this->lastImage = nextImage;
//        LOG_INFO() << "update Image (finished)";
//    }


    /** This function initializes the device and returns a device handle.
     */
    void openCamera()
    {
        if ( m_hCam != 0 )
        {
            //free old image mem.
            errorHandler(is_FreeImageMem( m_hCam, m_pcImageMemory, m_lMemoryId ));
            errorHandler(is_ExitCamera( m_hCam ));
        }

        // init camera
        m_hCam = static_cast<HIDS>(0); // open next camera
        int retcode;
        retcode = is_InitCamera(&m_hCam, nullptr); // init camera - no window handle required
        if( retcode == IS_SUCCESS )
        {
            sizeSensor = sensorSize();

            SENSORINFO sInfo;
            is_GetSensorInfo(m_hCam, &sInfo);
            LOG_DEBUG() << "ColorMode" << static_cast<int>(sInfo.nColorMode);

//#define IS_COLORMODE_INVALID                0
//#define IS_COLORMODE_MONOCHROME             1
//#define IS_COLORMODE_BAYER                  2
//#define IS_COLORMODE_CBYCRY                 4
//#define IS_COLORMODE_JPEG                   8

            if (static_cast<int>(sInfo.nColorMode) == IS_COLORMODE_INVALID)
            {
                LOG_ERROR() << "invalid Color Mode from Camera!";
                return;
            }


            if (static_cast<int>(sInfo.nColorMode) == IS_COLORMODE_MONOCHROME)
            {
                setColorMode(IS_CM_MONO8);
                this->m_nBitsPerPixel = bitsPerPixel();
            }


            if (!allocateMemmory()) {
                LOG_ERROR() << "Memory Initialsation failed";
                closeCamera();
                return;
            }

            // display initialization
            IS_SIZE_2D imageSize;
            imageSize.s32Width = sizeSensor.width();
            imageSize.s32Height = sizeSensor.height();

            errorHandler(is_AOI(m_hCam, IS_AOI_IMAGE_SET_SIZE, static_cast<void*>(&imageSize), sizeof(imageSize)));

            // allows direct access to image data
            errorHandler(is_SetDisplayMode (m_hCam, IS_SET_DM_DIB));
        }
        else
        {
            errorHandler(retcode);
            LOG_ERROR() << "open Camera Failed";
            return;
        }
        LOG_INFO() << "open Camera (finished)";
    }

    /*!
     * free previous allocatd memory and allocate image memory buffer
     * according to image size and color depth
     */
    bool allocateMemmory()
    {
        bool bReturn = true;

        // free memory
        if( m_pcImageMemory != nullptr )
        {
            errorHandler(is_FreeImageMem( m_hCam, m_pcImageMemory, m_lMemoryId ));
            m_pcImageMemory = nullptr;
            m_lMemoryId = 0;
        }

        // main memory allocate and activate
        int result =  is_AllocImageMem(	m_hCam,
                                    sizeSensor.width(),
                                    sizeSensor.height(),
                                    m_nBitsPerPixel,
                                    &m_pcImageMemory,
                                    &m_lMemoryId);
        if( result == IS_SUCCESS )
        {
            // set memory active
            errorHandler(is_SetImageMem (m_hCam, m_pcImageMemory, m_lMemoryId));
        }
        else
        {
            bReturn = false;
        }

        return bReturn;
    }


    /** calculates Bits to Bytes per Pixel */
    int bytesPerPixel()
    {
        return (m_nBitsPerPixel+1)/8;
    }

    /** calculates total bytes of image */
    int imageSizeBytes()
    {
        return sizeSensor.width() * sizeSensor.height()*bytesPerPixel();
    }

    /** This function will un-initialize the specified device,
     * closes its handle and releases allocated resources.
     */
    void closeCamera()
    {
        if( m_hCam != 0 )
        {
            // Disable messages
            errorHandler(is_EnableMessage(m_hCam,    IS_DEVICE_REMOVED,      nullptr));
            errorHandler(is_EnableMessage(m_hCam,    IS_DEVICE_RECONNECTED,  nullptr));
            errorHandler(is_EnableMessage(m_hCam,    IS_FRAME,               nullptr));
            errorHandler(is_EnableMessage(m_hCam,    IS_TRIGGER,             nullptr));

            // Stop live video
            errorHandler(is_StopLiveVideo( m_hCam, IS_WAIT ));

            // Free the allocated buffer
            if( m_pcImageMemory != nullptr)
            {
                errorHandler(is_FreeImageMem( m_hCam, m_pcImageMemory, m_lMemoryId ));
                m_pcImageMemory = nullptr;
            }

            // Close camera
            errorHandler(is_ExitCamera( m_hCam ));
            m_hCam = 0;
        }

        LOG_INFO() << "close Camera (finished)";
    }


    /** This function waits for next image is available at transport buffer.
     * If available - it does all required image processing
     * (unpack, sensor-defect-correction, demosaic) and fills image information
     * to structure at img parameter.
     */
    void getImage()
    {
        if ( m_hCam == 0 )
            openCamera();

        if ( m_hCam != 0 )
        {
            int result = is_FreezeVideo( m_hCam, IS_WAIT );
            if (result == IS_SUCCESS)
                createData();
        }
        LOG_INFO() << "Get Image (finished)";
    }

//    Limits exposureRange()
//    {
//        Limits limits;
//        is_GetExposureRange(m_hCam,
//                            &limits.min,
//                            &limits.max,
//                            &limits.intervall);
//        return limits;
//    }


    /** Sets exposure time in microseconds. */
    void setExposure(double exposure_ms)
    {                      
        //set the exposure time
        this->setAutoExposure(false);
        errorHandler(is_Exposure(m_hCam, IS_EXPOSURE_CMD_SET_EXPOSURE, (void*)&exposure_ms, sizeof(exposure_ms)));
    }

    /** Get exposure time in microseconds. */
    double exposure() const
    {
        double exposure_ms;
        errorHandler(is_Exposure(m_hCam, IS_EXPOSURE_CMD_GET_EXPOSURE, (void*)&exposure_ms, sizeof(exposure_ms)));
        return exposure_ms;
    }

    /** Sets gain factor */
    void setGain(float gain)
    {
        gain *= 100;
        int maxGain = is_SetHWGainFactor (m_hCam, IS_INQUIRE_MASTER_GAIN_FACTOR, 100);

        if (gain < 0)
            gain = 0;
        if (gain > maxGain)
            gain = maxGain;

        errorHandler(is_SetHWGainFactor(m_hCam, IS_SET_MASTER_GAIN_FACTOR, static_cast<INT>(gain)));

    }

    /** gain factor */
    float gain()
    {
        INT result = is_SetHWGainFactor(m_hCam, IS_GET_MASTER_GAIN_FACTOR, 0);
        return static_cast<float>(result)/100;
    }

    /** get parameters of camera, results are in QMap */
    QMap<QString, QString> getParamters()
    {
        QMap<QString, QString> mapParam;
        try{
            SENSORINFO SensorInfo;
            errorHandler(is_GetSensorInfo(m_hCam, &SensorInfo));
            mapParam.insert("Sensor Name", QString(SensorInfo.strSensorName));
            mapParam.insert("Pixel size / µm", QString("%1").arg(SensorInfo.wPixelSize/100));
            if (SensorInfo.nColorMode == IS_COLORMODE_MONOCHROME)
                mapParam.insert("Sensor Type", QString("Mono"));
            else
                mapParam.insert("Sensor Type", QString("Color"));

            CAMINFO Info;
            errorHandler(is_GetCameraInfo (m_hCam, &Info));
            mapParam.insert("Serial", QString(Info.SerNo));
        }
        catch(const std::exception& e)
        {
            qDebug() << "Error:" << e.what();
        }
        return mapParam;
    }

    void configureBinning(bool enableBinning, Qt::Orientation orientation, int binningPixels = 2)
    {
        if (enableBinning) {

            int binningID;
            if (orientation == Qt::Vertical) {
                switch (binningPixels)
                {
                case 1:
                    errorHandler(is_SetBinning (m_hCam, IS_BINNING_DISABLE));
                    break;
                case 2: binningID = IS_BINNING_2X_VERTICAL; break;
                case 3: binningID = IS_BINNING_3X_VERTICAL; break;
                case 4: binningID = IS_BINNING_4X_VERTICAL; break;
                case 5: binningID = IS_BINNING_5X_VERTICAL; break;
                case 6: binningID = IS_BINNING_6X_VERTICAL; break;
                case 8: binningID = IS_BINNING_8X_VERTICAL; break;
                case 16: binningID = IS_BINNING_16X_VERTICAL; break;
                default:
                    LOG_ERROR() << "Error: Invalid decimation value";
                    return;
                }
            }
            else {
                switch (binningPixels)
                {
                case 1:
                    errorHandler(is_SetBinning (m_hCam, IS_BINNING_DISABLE));
                    break;
                case 2: binningID = IS_BINNING_2X_HORIZONTAL; break;
                case 3: binningID = IS_BINNING_3X_HORIZONTAL; break;
                case 4: binningID = IS_BINNING_4X_HORIZONTAL; break;
                case 5: binningID = IS_BINNING_5X_HORIZONTAL; break;
                case 6: binningID = IS_BINNING_6X_HORIZONTAL; break;
                case 8: binningID = IS_BINNING_8X_HORIZONTAL; break;
                case 16: binningID = IS_BINNING_16X_HORIZONTAL; break;
                default:
                    LOG_ERROR() << "Error: Invalid decimation value";
                    return;
                }

            }
            errorHandler(is_SetBinning (m_hCam, binningID));
        }
        else {
            errorHandler(is_SetBinning (m_hCam, IS_BINNING_DISABLE));
        }
    }


    /** set ROI using offets and width and height */
    void setROI(QRect roi)
    {
        IS_RECT aoiRect;
        aoiRect.s32X = roi.x() | IS_AOI_IMAGE_POS_ABSOLUTE;
        aoiRect.s32Y = roi.y() | IS_AOI_IMAGE_POS_ABSOLUTE;
        aoiRect.s32Width = roi.width();
        aoiRect.s32Height = roi.height();

        errorHandler(is_AOI(m_hCam, IS_AOI_IMAGE_SET_AOI, &aoiRect, sizeof(aoiRect)));
    }

    /** get ROI using offets and width and height */
    QRect roi() const
    {        
        IS_RECT aoiRect;
        errorHandler(is_AOI(m_hCam, IS_AOI_IMAGE_SET_AOI, &aoiRect, sizeof(aoiRect)));

        return QRect(aoiRect.s32X, aoiRect.s32Y, aoiRect.s32Width, aoiRect.s32Width);
    }

    /** enable auto exposure */
    void setAutoExposure(bool enable)
    {
        double param1 = enable ? 1.0 : 0.0;
        double param2 = 0;
        errorHandler(is_SetAutoParameter(m_hCam, IS_SET_ENABLE_AUTO_SHUTTER, &param1, &param2));
        //is_SetAutoParameter (m_hCam, IS_SET_AUTO_REFERENCE, &dvalue, 0);
    }

    /** set new Color Mode (Pixel Data Format) */
    void setColorMode(int colorMode)
    {
        if ( m_hCam == 0 ) {
            return;
        }

        // set new color mode, read back color mode
        errorHandler(is_SetColorMode(m_hCam, colorMode ));
    }

    /** get Color Mode (Pixel Data Format) */
    int colorMode()
    {
        int colorMode  = -1;
        if ( m_hCam != 0 ) {
            colorMode = is_SetColorMode( m_hCam, IS_GET_COLOR_MODE );
        }
        return colorMode;
    }

    /** get color depth in bit */
    int colorDepth()
    {
        int colorMode  = -1;
        int colorDepth  = -1;
        if ( m_hCam != 0 ) {
            errorHandler(is_GetColorDepth( m_hCam, &colorDepth, &colorMode ));
        }
        LOG_DEBUG() << "color Mode" << colorMode;
        LOG_DEBUG() << "color Depth" << colorDepth;
        return colorDepth;
    }

    /** determine bits of a single pixel, depending on the format */
    int bitsPerPixel()
    {
        int m_nBitsPerPixel ;
        switch( is_SetColorMode( m_hCam, IS_GET_COLOR_MODE ) )
        {
        case IS_CM_RGBA12_UNPACKED:
        case IS_CM_BGRA12_UNPACKED:
            m_nBitsPerPixel = 64;
            break;

        case IS_CM_RGB12_UNPACKED:
        case IS_CM_BGR12_UNPACKED:
        case IS_CM_RGB10_UNPACKED:
        case IS_CM_BGR10_UNPACKED:
            m_nBitsPerPixel = 48;
            break;

        case IS_CM_RGBA8_PACKED:
        case IS_CM_BGRA8_PACKED:
        case IS_CM_RGB10_PACKED:
        case IS_CM_BGR10_PACKED:
        case IS_CM_RGBY8_PACKED:
        case IS_CM_BGRY8_PACKED:
            m_nBitsPerPixel = 32;
            break;

        case IS_CM_RGB8_PACKED:
        case IS_CM_BGR8_PACKED:
            m_nBitsPerPixel = 24;
            break;

        case IS_CM_BGR565_PACKED:
        case IS_CM_UYVY_PACKED:
        case IS_CM_CBYCRY_PACKED:
            m_nBitsPerPixel = 16;
            break;

        case IS_CM_BGR5_PACKED:
            m_nBitsPerPixel = 15;
            break;

        case IS_CM_MONO16:
        case IS_CM_SENSOR_RAW16:
        case IS_CM_MONO12:
        case IS_CM_SENSOR_RAW12:
        case IS_CM_MONO10:
        case IS_CM_SENSOR_RAW10:
            m_nBitsPerPixel = 16;
            break;

        case IS_CM_RGB8_PLANAR:
            m_nBitsPerPixel = 24;
            break;

        case IS_CM_MONO8:
        case IS_CM_SENSOR_RAW8:
        default:
            m_nBitsPerPixel = 8;
            break;
        }
        return m_nBitsPerPixel;
    }

    /** returns size of sensor in Pixel  */
    QSize sensorSize()
    {
        int sizeX, sizeY;
        sizeX = sizeY = 0;
        // Check if the camera supports an arbitrary AOI
        int nAOISupported = 0;
        bool bAOISupported = true;
        if (is_ImageFormat(m_hCam,
                           IMGFRMT_CMD_GET_ARBITRARY_AOI_SUPPORTED,
                           (void*)&nAOISupported,
                           sizeof(nAOISupported)) == IS_SUCCESS)
        {
            bAOISupported = (nAOISupported != 0);
        }

        if (bAOISupported)
        {
            // Get maximum image size
            SENSORINFO sInfo;
            errorHandler(is_GetSensorInfo (m_hCam, &sInfo));
            sizeX = static_cast<int>(sInfo.nMaxWidth);
            sizeY = static_cast<int>(sInfo.nMaxHeight);
        }
        else
        {
            // Only ueye xs
            // Get image size of the current format
            IS_SIZE_2D imageSize;
            errorHandler(is_AOI(m_hCam, IS_AOI_IMAGE_GET_SIZE, (void*)&imageSize, sizeof(imageSize)));

            sizeX = imageSize.s32Width;
            sizeY = imageSize.s32Height;
        }
        return QSize(sizeX, sizeY);
    }

    /** returns current image size in Pixel  */
    QSize imageSize()
    {
        char* pcMem;
        INT nImageBufferID;
        INT result = is_GetActiveImageMem(m_hCam, &pcMem, &nImageBufferID);
        if (result != IS_SUCCESS)
        {
            errorHandler(result);
            return QSize(0,0);
        }

        UEYEIMAGEINFO ImageInfo;
        errorHandler(is_GetImageInfo (m_hCam, nImageBufferID, &ImageInfo, sizeof(ImageInfo)));
        return QSize(static_cast<int>(ImageInfo.dwImageWidth), static_cast<int>(ImageInfo.dwImageHeight));
    }

};




#endif // CAMERAUEYE_P_H
