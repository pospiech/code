#include "QuEyeCamera.h"
#include "QCameraImage.h"
#include <QDebug>
#include <QTime>
#include <QMutexLocker>
#include <QMutex>

#include <vector>
using std::vector;

class QuEyeCameraPrivate
{
public:
    QuEyeCameraPrivate()
        : handleWindow(0)
        , widgetVideoWindow(0)
        , SizeX(0)
        , SizeY(0)
        , imageReady(false)
        , decimation(1)
        , isInitialized(false)        
        {}

    QRgbMatrix rgbMatrix;
    QImage image;
    HWND handleWindow;
    QCameraImage * widgetVideoWindow;
    QMutex  snapshotMutex;

    // uEye variables
    HIDS    handleCamera;   //!< handle to camera
    HWND    handleDisplay;  //!< handle to diplay window
    HWND    handleClassWindows;

    int     colorMode;      //!< Y8/RGB16/RGB24/REG32
    int     BitsPerPixel;   //!< number of bits needed store one pixel
    int     BytesPerPixel;  //!< number of bytes needed store one pixel
    int     BytesPerLine;
    int     ImageSize;      //!< size of image in bytes
    int     SizeX;          //!< width of video
    int     SizeY;          //!< height of video
    int     MemoryId;       //!< grabber memory - buffer ID
    char*   ImageMemory;    //!< grabber memory - pointer to buffer
    int     RenderMode;     //!< render  mode
    int     Pitch;          //!< pitch of image lines
    int     decimation;

    bool    imageReady;
    bool    isInitialized;
};

QuEyeCamera::QuEyeCamera(QWidget* parent /*= 0*/)
    : QWidget(parent)
    , d(new QuEyeCameraPrivate)
{
    d->handleCamera = 0;
    d->RenderMode = IS_RENDER_FIT_TO_WINDOW;
    d->handleClassWindows = (HWND)QWidget::winId();

    openCamera();		// open a camera handle
}

QuEyeCamera::~QuEyeCamera()
{
    if( d->handleCamera != 0 )
    {
        //free old image mem.
        is_FreeImageMem( d->handleCamera, d->ImageMemory, d->MemoryId );

        is_ExitCamera( d->handleCamera );
        d->isInitialized = false;
        emit connectionStatusChanged();
    }

    delete d;
}

void QuEyeCamera::setWindowHandle(HWND handle)
{
    d->handleWindow = handle;
}

void QuEyeCamera::setWidgetVideoWindow(QCameraImage *widget)
{
    d->widgetVideoWindow = widget;
}


int QuEyeCamera::initCamera(HIDS *hCam, HWND hWnd)
{
    int result = is_InitCamera(hCam, hWnd);
    switch (result)
    {
    case IS_SUCCESS:
        d->isInitialized = true;
        emit connectionStatusChanged();
        break;
    case IS_NO_SUCCESS:
        d->isInitialized = false;
        Error("Error: InitCamera failed.");
        break;
    case IS_STARTER_FW_UPLOAD_NEEDED:
        d->isInitialized = false;
        Error("Error: InitCamera failed: Firmware must be updated.");
        break;
    }
    return result;
}

bool QuEyeCamera::isInitialized()
{
    return d->isInitialized;
}

bool QuEyeCamera::isConnected()
{
    return d->isInitialized;
}


bool QuEyeCamera::openCamera()
{
    if ( d->handleCamera != 0 )
    {
        //free old image mem.
        is_FreeImageMem( d->handleCamera, d->ImageMemory, d->MemoryId );
        is_ExitCamera( d->handleCamera );
    }

    // init camera
    d->handleCamera = (HIDS) 0;				// open next camera
    int result = initCamera(&d->handleCamera, NULL);	// init camera - no window handle required
    qDebug() << "InitCamera Return Value" << result;
    if(result == IS_SUCCESS )
    {

        // init image size
        SENSORINFO SensorInfo;
        is_GetSensorInfo(d->handleCamera, &SensorInfo);

        d->SizeX = sensorSize().width();
        d->SizeY = sensorSize().height();

        // display initialization
        is_SetImageSize (d->handleCamera, d->SizeX, d->SizeY);
        is_SetDisplayMode (d->handleCamera, IS_SET_DM_DIB);


        // setup the color depth to the current windows setting
        is_GetColorDepth (d->handleCamera, &d->BitsPerPixel, &d->colorMode);
        is_SetColorMode (d->handleCamera, d->colorMode);


        // free previous allocatd memory and allocate image memory buffer
        // according to image size and color depth
        allocateMemmory();

        // use color depth according to monochrome or color camera
        if( SensorInfo.nColorMode == IS_COLORMODE_MONOCHROME )
        {
            // monochrome camera
            d->colorMode = IS_CM_MONO8;
            setColorMode(d->colorMode);
        }
        else
        {
            // color camera
            //d->colorMode = IS_CM_RGBA8_PACKED; // compatible with Qt
            d->colorMode = IS_CM_RGB8_PACKED; // compatible with Qt
            setColorMode(d->colorMode);

            bool bAutoSoftwareWhitebalance;
            bool bAutoSensorWhitebalance;

            double dblWB;
            is_SetAutoParameter(d->handleCamera, IS_GET_ENABLE_AUTO_WHITEBALANCE, &dblWB, NULL);
            bAutoSoftwareWhitebalance = (dblWB == 1.0);

            is_SetAutoParameter(d->handleCamera, IS_GET_ENABLE_AUTO_SENSOR_WHITEBALANCE, &dblWB, NULL);
            bAutoSensorWhitebalance = (dblWB == 1.0);

            dblWB = 0.0;

            // Disable software white balance
            if (bAutoSoftwareWhitebalance)
            {
                is_SetAutoParameter( d->handleCamera, IS_SET_ENABLE_AUTO_WHITEBALANCE, &dblWB, &dblWB);
            }

            // Disable sensor white balance
            if (bAutoSensorWhitebalance)
            {
                is_SetAutoParameter( d->handleCamera, IS_SET_ENABLE_AUTO_SENSOR_WHITEBALANCE, &dblWB, &dblWB);
            }

            // enable messages
            is_EnableMessage(d->handleCamera, IS_FRAME, d->handleClassWindows);
        }
        // enable the dialog based error report
        int result =  is_SetErrorReport (d->handleCamera, IS_ENABLE_ERR_REP); //IS_DISABLE_ERR_REP);
        if (result != IS_SUCCESS)
        {
            Error("ERROR: Can not enable the automatic uEye error report!");
            return false;
        }

    }
    else
    {
        Error("ERROR: Can not open uEye camera!");
        return false;
    }

    return true;
}

QuEyeCamera::SensorType QuEyeCamera::sensorType()
{
    SENSORINFO SensorInfo;
    is_GetSensorInfo(d->handleCamera, &SensorInfo);

    return static_cast<QuEyeCamera::SensorType>(SensorInfo.nColorMode);
}

QString QuEyeCamera::deviceName()
{
    SENSORINFO SensorInfo;
    is_GetSensorInfo(d->handleCamera, &SensorInfo);
    QString name = QString(SensorInfo.strSensorName);
    return name;
}

QString QuEyeCamera::serial()
{
    CAMINFO Info;
    is_GetCameraInfo (d->handleCamera, &Info);
    return QString(Info.SerNo);
}

/*!
 * free previous allocatd memory and allocate image memory buffer
 * according to image size and color depth
 */
bool QuEyeCamera::allocateMemmory()
{
    bool bReturn = true;

    // free memory
    if( d->ImageMemory != NULL )
    {
        is_FreeImageMem( d->handleCamera, d->ImageMemory, d->MemoryId);
        d->ImageMemory = NULL;
        d->MemoryId = 0;
    }

    // main memory allocate and activate
    int result =  is_AllocImageMem(	d->handleCamera,
                                d->SizeX,
                                d->SizeY,
                                d->BitsPerPixel,
                                &d->ImageMemory,
                                &d->MemoryId);
    if( result == IS_SUCCESS )
    {
        is_SetImageMem (d->handleCamera, d->ImageMemory, d->MemoryId);	// set memory active
        is_InquireImageMem (
                d->handleCamera,
                d->ImageMemory,
                d->MemoryId,
                &d->SizeX,
                &d->SizeY,
                &d->BitsPerPixel,
                &d->Pitch );	// read back for calculation

        d->BytesPerPixel = (d->BitsPerPixel+1)/8; // calculate bytes per pixel
        d->ImageSize = d->SizeX * d->SizeY * d->BytesPerPixel;	// image size [bytes]
        d->imageReady = false;
    }
    else
    {
        bReturn = false;
    }

    return bReturn;
}

///////////////////////////////////////////////////////////////////////////////
//
// METHOD QuEyeCamera::OnButtonStart()
//
// DESCRIPTION: start live display and return immediately
//
///////////////////////////////////////////////////////////////////////////////
void QuEyeCamera::startVideo()
{
    if( d->handleCamera == 0 )
        openCamera();

    if( d->handleCamera != 0 )
    {
        // Capture live video
        d->widgetVideoWindow->setVideoActive(true);
        is_CaptureVideo( d->handleCamera, IS_WAIT );

    }
}


///////////////////////////////////////////////////////////////////////////////
//
// METHOD QuEyeCamera::OnButtonStop()
//
// DESCRIPTION: stop live display and return immediately
//
///////////////////////////////////////////////////////////////////////////////
void QuEyeCamera::stopVideo()
{
    // stop live immediately - force stop
    if( d->handleCamera != 0 ) {
        is_StopLiveVideo( d->handleCamera, IS_FORCE_VIDEO_STOP );
        d->widgetVideoWindow->setVideoActive(false);
    }

}

void QuEyeCamera::takeSnapshot()
{
    QMutexLocker locker(&d->snapshotMutex);

    QTime time; time.start();

    if ( d->handleCamera == 0 )
        openCamera();

    if ( d->handleCamera != 0 )
    {
        d->rgbMatrix.setDataValid(false);
        d->imageReady = false;
        // FreezeVideo takes picture and save it to
        // assigned memory
        if( is_FreezeVideo( d->handleCamera, IS_WAIT ) == IS_SUCCESS ) {

            QImage::Format imageFormat = imageFormatQt();

            // if QImage Format is recognised save directly to this format
            if (imageFormat != QImage::Format_Invalid) {
                d->image = QImage (
                        (uchar*)d->ImageMemory,
                        frameSize().width(),
                        frameSize().height(),
                        imageFormat);

                d->image.setColorCount(8);
                d->imageReady = true;
                d->widgetVideoWindow->setVideoActive(false);


            } else {
                // otherwise print to Window if handle is valid
//                if (d->handleWindow) {
//                    d->widgetVideoWindow->setVideoActive(true); // directly rendered!
//                    is_RenderBitmap( d->handleCamera, d->MemoryId, d->handleWindow, d->RenderMode );
//                } else {
                    // or convert manually to Image (most time consuming)
                    d->widgetVideoWindow->setVideoActive(false);
                    convertMemoryToRGBMatrix();
//                }
            }
            locker.unlock();
            emit snapShotFinished();
        } else {
            Error("Error: Get Single Image Failed");
        }
    }
    qDebug() << "takeSnapshot(Time)" << time.elapsed();
}


QImage::Format QuEyeCamera::imageFormatQt()
{
    QImage::Format imageFormat = QImage::Format_Invalid;
    int validMode = IS_CM_RGB8_PACKED;
    if (d->colorMode == validMode) {
        imageFormat = QImage::Format_RGB888;
    }
//    switch( d->colorMode )
//    {
//    case IS_CM_RGBA8_PACKED:
//        // <r(8) g(8) b(8) x(8)>  | <r(8) g(8) b(8) x(8)> | <r(8) ...
//        imageFormat = QImage::Format_Invalid;
//                //QImage::Format_RGB32;
//        break;
//
//    case IS_CM_RGB8_PACKED:
//        // <r(8) g(8) b(8)> <r(8) | g(8) b(8)> <r(8) g(8) ... >
//
//        imageFormat = QImage::Format_RGB888;
//        break;
//
//    case IS_CM_BGR565_PACKED:
//        {
//            // b(5) g(6) r(5) | b(5) g(6) r(5)
//            // imageFormat = QImage::Format_RGB16;
//            break;
//        }
//    case IS_CM_BGR555_PACKED:
//        {
//            // b(5) g(5) r(5) x(1) | b(5) g(5) r(5) x(1)
//            // imageFormat = QImage::Format_RGB555;
//            break;
//        }
//    case IS_CM_MONO8:
//        {
//            // 8 | 8 | 8 | 8
//            break;
//        }
//    }
    return imageFormat;
}

void QuEyeCamera::convertMemoryToRGBMatrix()
{
    // continue when we have a valid image buffer
    if( d->ImageMemory != NULL )
    {
        if (d->rgbMatrix.isDataValid())
            return;

        QTime time; time.start();

        // get pointer to active image buffer
        void *pMemVoid;
        is_GetImageMem( d->handleCamera, &pMemVoid );

        // pitch gibt das Zeileninkrement in Bytes zurück.
        // Das Zeileninkrement ist die Anzahl Bytes vom Beginn einer Zeile
        // bis zum Beginn der nächsten Zeile.
        // Das Zeileninkrement kann größer sein als aus den beim Aufruf
        // von is_AllocImageMem() übergebenen Parametern ersichtlich ist.
        // Das Zeileninkrement ist immer eine durch 4 teilbare Anzahl Bytes.
        int nPitch;
        is_GetImageMemPitch( d->handleCamera, &nPitch );

        d->rgbMatrix.setSize(frameSize().width(), frameSize().height());
        // select convertion method:
        d->colorMode = colorMode();
        switch( d->colorMode )
        {
        case IS_CM_RGBA8_PACKED:
            {
                RGB32Pixel* imageIterator = reinterpret_cast<RGB32Pixel*> (pMemVoid);
                for ( int y = 0; y < frameSize().height(); y++ ) {
                    for ( int x = 0; x < frameSize().width(); x++ ) {
                        d->rgbMatrix.setPixel(x,y,imageIterator->r, imageIterator->g, imageIterator->b);
                        imageIterator++;
                    }
                }
            }
            break;
        case IS_CM_RGB8_PACKED:
            {
                RGB24Pixel* imageIterator = reinterpret_cast<RGB24Pixel*> (pMemVoid);
                for ( int y = 0; y < frameSize().height(); y++ ) {
                    for ( int x = 0; x < frameSize().width(); x++ ) {
                        d->rgbMatrix.setPixel(x,y,imageIterator->r, imageIterator->g, imageIterator->b);
                        imageIterator++;
                    }
                }
            }
            break;
        case IS_CM_BGRA8_PACKED:
            {
                BGR32Pixel* imageIterator = reinterpret_cast<BGR32Pixel*> (pMemVoid);
                for ( int y = 0; y < frameSize().height(); y++ ) {
                    for ( int x = 0; x < frameSize().width(); x++ ) {
                        d->rgbMatrix.setPixel(x,y,imageIterator->r, imageIterator->g, imageIterator->b);
                        imageIterator++;
                    }
                }
            }
            break;
        case IS_CM_BGR8_PACKED:
            {
                BGR24Pixel* imageIterator = reinterpret_cast<BGR24Pixel*> (pMemVoid);
                for ( int y = 0; y < frameSize().height(); y++ ) {
                    for ( int x = 0; x < frameSize().width(); x++ ) {
                        d->rgbMatrix.setPixel(x,y,imageIterator->r, imageIterator->g, imageIterator->b);
                        imageIterator++;
                    }
                }
            }
            break;
        default:
            {
                // calculate pixel offset (pixel locatation in memory)
                char* pPixelPointer;
                int r,g,b; r = g = b = 0;
                for ( int y = 0; y < frameSize().height(); y++ ) {
                    long yoffset = ((long)y * nPitch );
                    for ( int x = 0; x < frameSize().width(); x++ ) {
                        // (x*bytes) + (y * sizex)
                        long lMemoryPixelOffset = ((long)x * d->BytesPerPixel) + yoffset;
                        pPixelPointer = (reinterpret_cast<char*> (pMemVoid) + (lMemoryPixelOffset)) ;

                        pixelToRGB(pPixelPointer, r, g, b);
                        d->rgbMatrix.setPixel(x,y,r,g,b);
                    }
                }

            }
        }

        qDebug() << "Time (convertMemoryToRGBMatrix): " << time.elapsed();        
        d->rgbMatrix.setDataValid(true);
    }
}


void QuEyeCamera::pixelToRGB(const char* pPixelPointer, int & r, int & g, int & b)
{
    // some variables for pixel peek
    // 32 bit
    unsigned int  nPixVal = 0;

    // pixel peek according color format
    switch( d->colorMode )
    {
    case IS_CM_RGBA8_PACKED:
        // <r(8) g(8) b(8) x(8)> <r(8) g(8) b(8) x(8)>

    case IS_CM_RGB8_PACKED:
        // =
        // <r(8) g(8) b(8)> <r(8) | g(8) b(8)> <r(8) g(8) ... >

        r = static_cast<int>(*(reinterpret_cast<const char*>(pPixelPointer+2)));
        g = static_cast<int>(*(reinterpret_cast<const char*>(pPixelPointer+1)));
        b = static_cast<int>(*(reinterpret_cast<const char*>(pPixelPointer+0)));
        break;

    case IS_CM_BGRA8_PACKED:
        // <b(8) g(8) r(8) x(8)> <b(8) g(8) r(8) x(8)>

    case IS_CM_BGR8_PACKED:
        // =
        // <b(8) g(8) r(8)> <b(8) | g(8) r(8)> <b(8) g(8) ... >

        b = static_cast<int>(*(reinterpret_cast<const char*>(pPixelPointer+2)));
        g = static_cast<int>(*(reinterpret_cast<const char*>(pPixelPointer+1)));
        r = static_cast<int>(*(reinterpret_cast<const char*>(pPixelPointer+0)));
        break;

    case IS_CM_BGR565_PACKED:
        {
        // b(5) g(6) r(5) | b(5) g(6) r(5)
        nPixVal = *(reinterpret_cast<unsigned int*>(const_cast<char*>(pPixelPointer)));
        unsigned char cPixVal1 = (nPixVal & 0xf800) >> 11;
        unsigned char cPixVal2 = (nPixVal & 0x07e0) >> 5;
        unsigned char cPixVal3 = (nPixVal & 0x001f) >> 0;
        b = static_cast<int>(cPixVal1);
        g = static_cast<int>(cPixVal2);
        r = static_cast<int>(cPixVal3);
        break;
        }
    case IS_CM_BGR555_PACKED:
        {
        // b(5) g(5) r(5) x(1) | b(5) g(5) r(5) x(1)
        nPixVal = *(reinterpret_cast<unsigned int*>(const_cast<char*>(pPixelPointer)));
        unsigned char cPixVal1 = (nPixVal & 0x7c00) >> 10;
        unsigned char cPixVal2 = (nPixVal & 0x03e0) >> 5;
        unsigned char cPixVal3 = (nPixVal & 0x001f) >> 0;
        b = static_cast<int>(cPixVal1);
        g = static_cast<int>(cPixVal2);
        r = static_cast<int>(cPixVal3);

        break;
        }
    case IS_CM_MONO8:
        {
        // 8 | 8 | 8 | 8
        unsigned char cPixVal1 = *(reinterpret_cast<const char*>(pPixelPointer));
        r = g = b = static_cast<int>(cPixVal1);
        break;
        }
    case IS_SET_CM_BAYER:
        {

        break;
        }
    default:
        // we never should pass this point
        return;
    }
}



QImage QuEyeCamera::image()
{
    QMutexLocker locker(&d->snapshotMutex);

    if (d->imageReady) {
        return d->image;
    } else {
        if (!d->rgbMatrix.isDataValid()) {
            convertMemoryToRGBMatrix();
        }
        return d->rgbMatrix.toImage();
//        } else {
//            d->image = QImage(frameSize(), QImage::Format_RGB666);
//            d->image.fill(0);
//            return d->image;
//        }
    }

}

QRgbMatrix & QuEyeCamera::rgbMatrix()
{
    if (!d->rgbMatrix.isDataValid()) {
        convertMemoryToRGBMatrix();
    }

    return d->rgbMatrix;
}

///////////////////////////////////////////////////////////////////////////////
//
// METHOD:		CamSetColorMode()
//
// DESCRIPTION: Set the color mode according to:
//				- BitsPerPixel
//				- colorMode
//				- reallocate memory
//
///////////////////////////////////////////////////////////////////////////////
bool QuEyeCamera::setColorMode(int colorMode)
{
    bool bReturn = true;
    if ( d->handleCamera == 0 ) {
        return false;
    }

    // set new color mode, read back color mode
    d->colorMode = colorMode;
    int result =  is_SetColorMode(d->handleCamera, colorMode );
    if( result == IS_SUCCESS )
    {
        // change bits per pixel according color mode
        d->BitsPerPixel = bitsPerPixel();

        // reallocte image buffers
        bReturn = allocateMemmory();
    }
    else
    {
        bReturn = false;
    }


    return bReturn;
}



int QuEyeCamera::colorMode()
{
    int colorMode  = -1;
    if ( d->handleCamera != 0 ) {
        colorMode = is_SetColorMode( d->handleCamera, IS_GET_COLOR_MODE );
    }
    return colorMode;
}


int QuEyeCamera::bitsPerPixel()
{
    int bits = 0;
    if ( d->handleCamera != 0 )
    {

        // change bits per pixel according color mode
        switch(colorMode())
        {
        case IS_CM_RGBA8_PACKED:
        case IS_CM_BGRA8_PACKED:
        case IS_CM_RGBY8_PACKED:
        case IS_CM_BGRY8_PACKED:
        case IS_CM_RGB10V2_PACKED:
        case IS_CM_BGR10V2_PACKED:
            bits = 32;
            break;
        case IS_CM_RGB8_PACKED:
        case IS_CM_BGR8_PACKED:
        case IS_SET_CM_UYVY:
            bits = 24;
            break;
        case IS_CM_MONO12:
        case IS_CM_MONO16:
        case IS_CM_BAYER_RG12:
        case IS_CM_BAYER_RG16:
        case IS_CM_BGR565_PACKED:
        case IS_CM_BGR555_PACKED:
            bits = 16;
            break;
        case IS_CM_MONO8:
        case IS_CM_BAYER_RG8:
            bits = 8;
            break;
        default:
            // we never should pass this point
            bits = 32;
            qDebug() << "ERROR in CamSetColorMode().\nInvalid color mode.";
            return false;
        }
    }
    return bits;
}

void QuEyeCamera::loadParameter()
{
    if ( d->handleCamera == 0 )
        openCamera();

    if ( d->handleCamera != 0 )
    {
        if( is_LoadParameters( d->handleCamera, NULL ) == IS_SUCCESS )
        {
            // realloc image mem with actual sizes and depth.
            is_FreeImageMem( d->handleCamera, d->ImageMemory, d->MemoryId );
            d->SizeX = is_SetImageSize( d->handleCamera, IS_GET_IMAGE_SIZE_X, 0 );
            d->SizeY = is_SetImageSize( d->handleCamera, IS_GET_IMAGE_SIZE_Y, 0 );

            d->BitsPerPixel = bitsPerPixel();

            // memory initialization
            allocateMemmory();
        }
    }
}


QSize QuEyeCamera::sensorSize()
{
    int sizeX, sizeY;
    sizeX = sizeY = 0;
    // Check if the camera supports an arbitrary AOI    
    int nAOISupported = 0;
    bool bAOISupported = true;
    if (is_ImageFormat(d->handleCamera,
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
            is_GetSensorInfo (d->handleCamera, &sInfo);
            sizeX = sInfo.nMaxWidth;
            sizeY = sInfo.nMaxHeight;
    }
    else
    {
        // Get image size of the current format
        sizeX = is_SetImageSize(d->handleCamera, IS_GET_IMAGE_SIZE_X, 0);
        sizeY = is_SetImageSize(d->handleCamera, IS_GET_IMAGE_SIZE_Y, 0);
    }
    return QSize(sizeX, sizeY);
}

QSize QuEyeCamera::frameSize()
{
    //QSize size = QSize(d->SizeX, d->SizeY / d->decimation);
    QSize size = QSize(d->SizeX, d->SizeY);
    return size;
}

///////////////////////////////////////////////////////////////////////////////
//
// METHOD CIdsSimpleLiveDlg::OnUEyeMessage()
//
// DESCRIPTION: - handles the messages from the uEye camera
//				- messages must be enabled using is_EnableMessage()
//
///////////////////////////////////////////////////////////////////////////////

bool QuEyeCamera::winEventFilter ( MSG * msg, long * result )
{
    bool processNotByQt = false;
    switch ( msg->wParam )
    {
    case IS_DEVICE_REMOVED:
        processNotByQt = true;
        break;
    case IS_DEVICE_RECONNECTED:
        processNotByQt = true;
        break;
    case IS_FRAME:
        if( d->ImageMemory != NULL ) {
            if (d->handleWindow) {
                is_RenderBitmap( d->handleCamera, d->MemoryId, d->handleWindow, d->RenderMode );
            }
        }
        processNotByQt = true;
        break;
    }
    return processNotByQt;
}


/*! is_GetFramesPerSecond() gibt im Livebetrieb mit is_CaptureVideo()
 *  die Anzahl der tatsächlich eingezogenen Bilder pro Sekunde zurück.
 */
double QuEyeCamera::framesPerSecond()
{
    double dblFPS;
    is_GetFramesPerSecond (d->handleCamera, &dblFPS);
    return dblFPS;
}

/*! Einstellen der Bildwiederholrate (Framerate)
 * des Sensors im freilaufenden Modus (Livemodus)
 */
double QuEyeCamera::setFramesPerSecond(double fps)
{
    double dblFPS;
    is_SetFrameRate(d->handleCamera, fps, &dblFPS);
    return dblFPS;
}

/*! Gibt den einstellbaren Bildratenbereich zurück */
QuEyeCamera::Limits QuEyeCamera::framesPerSecondRange()
{
    Limits limits;
    is_GetFrameTimeRange(d->handleCamera,
                         &limits.min,
                         &limits.max,
                         &limits.intervall);
    Limits limitsFPS;
    limitsFPS.min = 1.0/limits.max;
    limitsFPS.max = 1.0/limits.min;
    limitsFPS.intervall = 1.0/(limits.min + limits.intervall);

    return limitsFPS;
}

/*! Gibt den einstellbaren Pixeltaktbereich zurück */
QuEyeCamera::Limits QuEyeCamera::pixelClockRange()
{
    int min;
    int max;
    is_GetPixelClockRange(d->handleCamera, &min, &max);

    Limits limits;
    limits.min = double(min);
    limits.max = double(max);
    limits.intervall = 1;
    return limits;
}

/*! setzt den Pixeltakt in MHz */
void QuEyeCamera::setPixelClock(int clock)
{
    is_SetPixelClock (d->handleCamera, clock);


}
/*! liefert den Pixeltakt in MHz */
int QuEyeCamera::pixelClock()
{
    int clock = is_SetPixelClock (d->handleCamera, IS_GET_PIXEL_CLOCK);
    return clock;
}


/* Ein-/Ausschalten der Schwarzwert-Korrektur */
void QuEyeCamera::setBlackCompensation(bool enabled, int offset)
{
    INT nEnable;
    switch (enabled)
    {
    case true:  nEnable = IS_BL_COMPENSATION_ENABLE; break;
    case false: nEnable = IS_BL_COMPENSATION_DISABLE; break;
    }
    offset = correctMinMax(offset, 0, 255);
    is_SetBlCompensation(d->handleCamera, nEnable, offset, 0);
}

void QuEyeCamera::getBlackCompensation(bool & enabled, int & offset)
{

    INT nEnable = is_SetBlCompensation(d->handleCamera, IS_GET_BL_COMPENSATION, 0, 0);
    switch (nEnable)
    {
    case IS_BL_COMPENSATION_ENABLE:  enabled = true; break;
    case IS_BL_COMPENSATION_DISABLE: enabled = false; break;
    }

    offset = is_SetBlCompensation(d->handleCamera, IS_GET_BL_OFFSET, 0, 0);
}

/*! Einstellen der Belichtungszeit (setzt die Belichtungsdauer in ms)*/
double QuEyeCamera::setExposure(double exposure)
{
    double exposureResult;
    is_SetExposureTime (d->handleCamera, exposure, &exposureResult);
    return exposureResult;
}

/*! Abfrage der Belichtungszeit (in ms)*/
double QuEyeCamera::exposure()
{
    double exposureResult;
    is_SetExposureTime (d->handleCamera, IS_GET_EXPOSURE_TIME, &exposureResult);
    return double(exposureResult);
}

/*! Gibt den einstellbaren Belichtungsbereich zurück
 *
 * Die Schrittweite für die Einstellung der Belichtungszeit (intervall) ist vom aktuellen
 * Timing des Sensors (Pixeltakt, Bildrate) abhängig. Die kleinste Schrittweite entspricht
 * immer einer Zeilendauer, also der Zeit, die der Sensor zum Auslesen einer Bildzeile benötigt.
 */
QuEyeCamera::Limits QuEyeCamera::exposureRange()
{
    Limits limits;
    is_GetExposureRange(d->handleCamera,
                        &limits.min,
                        &limits.max,
                        &limits.intervall);
    return limits;
}

/*! Zusätzliche Hardwareverstärkung des Sensors  einstellen */
void QuEyeCamera::setGainBoostEnabled(bool enable)
{
    INT mode;
    if (enable)
        mode = IS_SET_GAINBOOST_ON;
    else
        mode = IS_SET_GAINBOOST_OFF;

    is_SetGainBoost(d->handleCamera, mode);
}

/*! Zusätzliche Hardwareverstärkung des Sensors abfragen */
bool QuEyeCamera::isGainBoostEnabled()
{
    bool enabled;
    INT mode = is_SetGainBoost(d->handleCamera, IS_GET_GAINBOOST);
    if (mode == IS_SET_GAINBOOST_ON)
        enabled = true;
    else
        enabled = false;
    return enabled;
}

/*! Einstellen des Gammawertes  (digitale Nachbearbeitung) */
void QuEyeCamera::setGamma(int value)
{
    value = correctMinMax(value, 1, 1000);
    is_SetGamma (d->handleCamera, value);
}

/*! Auslesen des Gammawertes  (digitale Nachbearbeitung) */
int QuEyeCamera::gamma()
{
    INT value = is_SetGamma (d->handleCamera, IS_GET_GAMMA);
    return static_cast<int>(value);
}

/*! Aktiviert/deaktiviert den Global-Start-Shutter */
void QuEyeCamera::setShutterMode(QuEyeCamera::ShutterMode mode)
{
    is_SetGlobalShutter(d->handleCamera, static_cast<INT>(mode));
}

/*! Liefert aktuellen den Global-Start-Shutter Mode zurück */
QuEyeCamera::ShutterMode  QuEyeCamera::shutterMode()
{
    QuEyeCamera::ShutterMode shuttermode;
    INT mode = is_SetGlobalShutter(d->handleCamera, IS_GET_GLOBAL_SHUTTER);
    switch (mode)
    {
    case IS_NOT_SUPPORTED:
    case IS_SET_GLOBAL_SHUTTER_OFF:
        shuttermode = RollingShutter;
        break;
    default:
        shuttermode = GlobalShutter;
    }

    return shuttermode;
}

bool QuEyeCamera::isGlobalShutterSupported()
{
    INT mode = is_SetGlobalShutter(d->handleCamera, IS_GET_SUPPORTED_GLOBAL_SHUTTER);
    if (mode == IS_SET_GLOBAL_SHUTTER_OFF) {
        return false;
    } else {
        return true;
    }
}

/*! Gammaregelung des Sensors einstellen */
void QuEyeCamera::setGain(int master)
{
    master = correctMinMax(master, 0, 100);
    is_SetHardwareGain(d->handleCamera, master,
                       IS_IGNORE_PARAMETER, IS_IGNORE_PARAMETER, IS_IGNORE_PARAMETER);

}

void QuEyeCamera::setGainRGB(int red,int green,int blue)
{
    red = correctMinMax(red, 0, 100);
    green = correctMinMax(green, 0, 100);
    blue = correctMinMax(blue, 0, 100);
    is_SetHardwareGain(d->handleCamera, IS_IGNORE_PARAMETER, red,green,blue);
}

int QuEyeCamera::gain()
{
    INT master = is_SetHardwareGain(d->handleCamera, IS_GET_MASTER_GAIN,
                       IS_IGNORE_PARAMETER, IS_IGNORE_PARAMETER, IS_IGNORE_PARAMETER);
    return static_cast<int>(master);
}

QuEyeCamera::RGBPixel QuEyeCamera::gainRGB()
{
    INT red = is_SetHardwareGain(d->handleCamera, IS_GET_RED_GAIN, IS_IGNORE_PARAMETER, IS_IGNORE_PARAMETER, IS_IGNORE_PARAMETER);
    INT green = is_SetHardwareGain(d->handleCamera, IS_GET_GREEN_GAIN, IS_IGNORE_PARAMETER, IS_IGNORE_PARAMETER, IS_IGNORE_PARAMETER);
    INT blue = is_SetHardwareGain(d->handleCamera, IS_GET_BLUE_GAIN, IS_IGNORE_PARAMETER, IS_IGNORE_PARAMETER, IS_IGNORE_PARAMETER);
    RGBPixel rgb;
    rgb.red = int(red);
    rgb.green = int(green);
    rgb.blue = int(blue);
    return rgb;
}

int QuEyeCamera::colorDepth()
{
    int depth;
    int mode;
    is_GetColorDepth(d->handleCamera, &depth, &mode);
    return depth;
}

void QuEyeCamera::setAOI(QRect AOI)
{
    INT XPos, YPos, Width, Height;
    XPos = AOI.left();
    YPos = AOI.top();
    Width = AOI.width();
    Height = AOI.height();

    is_SetAOI (d->handleCamera, IS_SET_IMAGE_AOI, &XPos, &YPos, &Width, &Height);
}

QRect QuEyeCamera::AOI()
{
    INT XPos, YPos, Width, Height;
    is_SetAOI (d->handleCamera, IS_GET_IMAGE_AOI, &XPos, &YPos, &Width, &Height);
    return QRect(XPos, YPos, Width, Height);
}

void QuEyeCamera::setViewPort(QRect AOI)
{
    setAOI(AOI);
}

QRect QuEyeCamera::viewPort()
{
    return AOI();
}

void QuEyeCamera::setBinning(int factor)
{
    d->decimation = factor;
    switch (factor)
    {
    case 1:
        is_SetBinning (d->handleCamera, IS_BINNING_DISABLE);
        break;
    case 2:
        is_SetBinning (d->handleCamera, IS_BINNING_2X_VERTICAL || IS_BINNING_2X_HORIZONTAL);
        break;
    case 4:
        is_SetBinning (d->handleCamera, IS_BINNING_4X_VERTICAL || IS_BINNING_4X_HORIZONTAL);
        break;
    case 6:
        is_SetBinning (d->handleCamera, IS_BINNING_6X_VERTICAL || IS_BINNING_6X_HORIZONTAL);
        break;
    default:
        Error("Error: Invalid decimation value");
        d->decimation = 1;
        return;
    }
    //is_SetImageSize (d->handleCamera, frameSize().width(), frameSize().height());
    qDebug() << "Binning: " << factor;
}

int QuEyeCamera::binning()
{
    int factor = 1;
    INT mode = is_SetBinning (d->handleCamera, IS_GET_BINNING_FACTOR_VERTICAL);
    switch (mode)
    {
    case IS_BINNING_DISABLE:
        factor = 1;
        break;
    case IS_BINNING_2X_VERTICAL:
        factor = 2;
        break;
    case IS_BINNING_4X_VERTICAL:
        factor = 4;
        break;
    case IS_BINNING_6X_VERTICAL:
        factor = 6;
        break;
    default:
        Error("Error: Invalid decimation value");
        return 0;
    }
    d->decimation = factor;
    return factor;
}


void QuEyeCamera::setWhiteBalanceEnabled(bool enable)
{
    double pval1;
    if (enable) { pval1 = 1; } else { pval1= 0; }
    is_SetAutoParameter (d->handleCamera, IS_SET_ENABLE_AUTO_WHITEBALANCE, &pval1, 0);
}

bool QuEyeCamera::whiteBalanceEnabled()
{
    double pval1;
    is_SetAutoParameter (d->handleCamera, IS_GET_ENABLE_AUTO_WHITEBALANCE, &pval1, 0);
    if (pval1 == 1) {
        return true;
    } else {
        return false;
    }
}


void QuEyeCamera::setAutoBrightness(int value)
{
    double dvalue = double(correctMinMax(value, 0, 255));
    is_SetAutoParameter (d->handleCamera, IS_SET_AUTO_REFERENCE, &dvalue, 0);
}

int QuEyeCamera::autoBrightness()
{
    double dvalue;
    is_SetAutoParameter (d->handleCamera, IS_GET_AUTO_REFERENCE, &dvalue, 0);
    return int(dvalue);
}

void QuEyeCamera::saveParameters(QString filename)
{
    QByteArray ba = filename.toLatin1();
    is_SaveParameters (d->handleCamera, ba);
}

void QuEyeCamera::loadParameters(QString filename)
{
    QByteArray ba = filename.toLatin1();
    is_LoadParameters(d->handleCamera, ba);
}

void QuEyeCamera::Error(const QString & message)
{
    emit errorOccured(message);
}

void QuEyeCamera::GetHistogramm(vector<int> & red, vector<int> & green, vector<int> & blue)
{
    red.clear(); red.resize(256);
    green.clear(); green.resize(256);
    blue.clear(); blue.resize(256);

    // Speicher für RGB-Histogramm anlegen
    DWORD bgrBuffer [256*3];

    //Pointer für jede Histogramm-Farbe anlegen
    DWORD *pBlueHisto  = bgrBuffer;
    DWORD *pGreenHisto = bgrBuffer + 256;
    DWORD *pRedHisto = bgrBuffer + 512;

    is_GetImageHistogram (d->handleCamera, d->MemoryId, d->colorMode, bgrBuffer);

    for (int i=0; i < 256*3; ++i) {
        red[i] = pRedHisto[i];
        green[i] = pGreenHisto[i];
        blue[i] = pBlueHisto[i];
    }
}
