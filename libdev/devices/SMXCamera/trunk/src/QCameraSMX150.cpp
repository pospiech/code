#include "QCameraSMX150.h"

#include <QSize>
#include <QTime>
#include "QCameraImage.h"
#include "SMX150.h"
//#include "QSMXDLL.h"

#include <iostream>
#include <fstream>
#include <iomanip>  //setiosflags
#include <cstdlib>
#include <sstream>
#include <cmath>
using namespace std;

class QCameraSMXPrivate
{
public:
    QCameraSMXPrivate()
        : isInitialized(false)
        , handleCamera(0)
        , widgetVideoWindow(0)
        , windowDC(0)
        , frameArrayRgb(0)
        , frameArray(0)
        , imageReady(false)
    {
        // fill data structure with 0 (code from camera vendor)
        memset( &cameraParameters, 0, sizeof(cameraParameters) );
    }

    ~QCameraSMXPrivate()
    {
        delete [] frameArrayRgb;
        delete [] frameArray;
        // delete cameraSMX;
    }

    // QSMXDLL * cameraSMX;
    bool isInitialized;
    HANDLE handleCamera;
    QCameraImage * widgetVideoWindow;
    HDC windowDC;
    QSize frameSize;
    QSize sensorSize;

    TFrameParams cameraParameters;
    TCameraInfo cameraInfo;
    TCameraInfoEx cameraInfoExtended;

    TRgbPix * frameArrayRgb;
    char *frameArray;

    bool imageReady;
    QImage image;
};


class QCameraSettingsPrivate
{
public:
    QCameraSettingsPrivate()
        : brightness(0)
        , constrast(0)
        , gamma(0)
    {}
    int brightness;
    int constrast;
    int gamma;

    QCameraSMX::BayerAlgorithm bayerAlgorithm;
};

QCameraSMX::QCameraSMX(QObject *parent)
    //: QObject()
    : QSMXDLL(parent)
    , d( new QCameraSMXPrivate)
    , settings(new QCameraSettingsPrivate)
{

}

QCameraSMX::~QCameraSMX(void)
{
    if (d->isInitialized)
    {
        CxCloseDevice(d->handleCamera);
        unload(); //unload DLL
        d->isInitialized = false;
        emit connectionStatusChanged(false);
    }

    delete d;
    delete settings;
}

void QCameraSMX::Error(const QString & message)
{
    emit errorOccured(message);
}

bool QCameraSMX::isInitialized()
{
    return d->isInitialized;
}

bool QCameraSMX::isConnected()
{
    return d->isInitialized;
}

bool QCameraSMX::initialize()
{
    if (!d->isInitialized)
    {
        bool isLoaded = loadDLL(); //loads Camera dll and does initialisation
        if (!isLoaded) {
            Error("Error: can not load DLL");
            return false;
        }

        d->handleCamera = CxOpenDevice(0); // open camera and save device
        if (d->handleCamera == INVALID_HANDLE_VALUE)
        {
            // unload(); //unload DLL
            Error("Error: can not open Camera");
            return false;
        }
    }
    d->isInitialized = true;

    // Get Information about Camera
    d->sensorSize = sensorSize();
    //d->bufferSize = d->sensorSize.width() * d->sensorSize.height();
    setViewPort(QRect(0,0,d->sensorSize.width(), d->sensorSize.height()));

    setDACOffsetRaw(55);
    setDACOffsetFine(65);

    emit connectionStatusChanged(true);
    return true;
}

void QCameraSMX::setWidgetVideoWindow(QCameraImage *widget)
{
    d->widgetVideoWindow = widget;
}

/* --- TCameraInfo ---------------*/
QString QCameraSMX::deviceName()
{
    CxGetCameraInfo (d->handleCamera,&d->cameraInfo);
    return QString::fromLocal8Bit(d->cameraInfo.DeviceName,64);
}

QCameraSMX::SensorType QCameraSMX::sensorType()
{
    CxGetCameraInfo (d->handleCamera,&d->cameraInfo);
    switch (d->cameraInfo.SensorType)
    {
    case 0 : return QCameraSMX::Monochrome;
    case 1 :
    default:
        return QCameraSMX::Color;
    }
}

QSize QCameraSMX::sensorSize()
{
    CxGetCameraInfo (d->handleCamera,&d->cameraInfo);
    return QSize(d->cameraInfo.MaxWidth, d->cameraInfo.MaxHeight);
}

int QCameraSMX::maxFrameSize()
{
    return sensorSize().width()  * sensorSize().height();
}

/* --- TCameraInfoEx ---------------*/
int QCameraSMX::modelID()
{
    CxGetCameraInfoEx(d->handleCamera, &d->cameraInfoExtended);
    return static_cast<int>(d->cameraInfoExtended.HWModelID);
}

int QCameraSMX::version()
{
    CxGetCameraInfoEx(d->handleCamera, &d->cameraInfoExtended);
    return static_cast<int>(d->cameraInfoExtended.HWVersion);
}

int QCameraSMX::serial()
{
    CxGetCameraInfoEx(d->handleCamera, &d->cameraInfoExtended);
    return static_cast<int>(d->cameraInfoExtended.HWSerial);
}

/* --- TFrameParams ---------------*/

int QCameraSMX::colorDepth()
{
    CxGetScreenParams(d->handleCamera, &d->cameraParameters);
    return d->cameraParameters.ColorDeep;
}

void QCameraSMX::setColorDepth(int depth)
{
    CxGetScreenParams(d->handleCamera, &d->cameraParameters);
    switch (depth)
    {
    case 8:
    case 24:
        d->cameraParameters.ColorDeep = depth;
        break;
    default:
        Error("Error: Invalid Color Depth value");
        return;
    }
    CxSetScreenParams(d->handleCamera, &d->cameraParameters);
    CxActivateScreenParams(d->handleCamera);
}

int QCameraSMX::decimation()
{
    CxGetScreenParams(d->handleCamera, &d->cameraParameters);
    return d->cameraParameters.Decimation;
}

void QCameraSMX::setDecimation(int decimation)
{
    CxGetScreenParams(d->handleCamera, &d->cameraParameters);
    switch (decimation)
    {
    case 1:
    case 2:
    case 4:
        d->cameraParameters.Decimation = decimation;
        break;
    default:
        Error("Error: Invalid decimation value");
        return;
    }
    CxSetScreenParams(d->handleCamera, &d->cameraParameters);
    CxActivateScreenParams(d->handleCamera);
}


QRect QCameraSMX::viewPort()
{
    CxGetScreenParams(d->handleCamera, &d->cameraParameters);
    return QRect(d->cameraParameters.StartX,
                 d->cameraParameters.StartY,
                 d->cameraParameters.Width,
                 d->cameraParameters.Height);
}

int QCameraSMX::viewSize()
{
    CxGetScreenParams(d->handleCamera, &d->cameraParameters);
    return d->cameraParameters.Width * d->cameraParameters.Height / (decimation()*decimation());
}


void QCameraSMX::setViewPort(QRect rect)
{
    CxGetScreenParams(d->handleCamera, &d->cameraParameters);

    d->cameraParameters.StartX = rect.left();
    d->cameraParameters.StartY = rect.top();
    d->cameraParameters.Width = rect.width();
    d->cameraParameters.Height = rect.height();
    CxSetScreenParams(d->handleCamera, &d->cameraParameters);
    CxActivateScreenParams(d->handleCamera);
}

bool QCameraSMX::mirrorVertical()
{
    CxGetScreenParams(d->handleCamera, &d->cameraParameters);
    return static_cast<bool>(d->cameraParameters.MirrorV);
}

void QCameraSMX::setMirrorVertical(bool mirror)
{
    CxGetScreenParams(d->handleCamera, &d->cameraParameters);
    d->cameraParameters.MirrorV = static_cast<BOOLEAN>(mirror);
    CxSetScreenParams(d->handleCamera, &d->cameraParameters);
    CxActivateScreenParams(d->handleCamera);
}

bool QCameraSMX::mirrorHorizontal()
{
    CxGetScreenParams(d->handleCamera, &d->cameraParameters);
    return static_cast<bool>(d->cameraParameters.MirrorH);
}

void QCameraSMX::setMirrorHorizontal(bool mirror)
{
    CxGetScreenParams(d->handleCamera, &d->cameraParameters);
    d->cameraParameters.MirrorH = static_cast<BOOLEAN>(mirror);
    CxSetScreenParams(d->handleCamera, &d->cameraParameters);
    CxActivateScreenParams(d->handleCamera);
}

/* --- else ---------------*/

double QCameraSMX::frequency()
{
    BYTE frequencyID;
    double frequency = 0;
    CxGetFrequency(d->handleCamera, &frequencyID);

    switch (frequencyID)
    {
    case 0: frequency = 40; break;
    case 1: frequency = 24; break;
    case 2: frequency = 20; break;
    case 3: frequency = 16; break;
    case 4: frequency = 13.33; break;
    case 5: frequency = 12; break;
    case 6: frequency = 10; break;
    case 7: frequency = 8; break;
    case 8: frequency = 6.66; break;
    case 9: frequency = 3.33; break;
    case 10: frequency = 48; break;
    }

    return frequency;
}

void QCameraSMX::setFrequency(double frequency)
{
    BYTE ID = 0;
    if (frequency <= 3.33) ID = 9;
    else if (frequency <= 6.66) ID = 8;
    else if (frequency <= 8.0) ID = 7;
    else if (frequency <= 10) ID = 6;
    else if (frequency <= 12) ID = 5;
    else if (frequency <= 13.33) ID = 4;
    else if (frequency <= 16) ID = 3;
    else if (frequency <= 20) ID = 2;
    else if (frequency <= 24) ID = 1;
    else if (frequency <= 40) ID = 0;
    else if (frequency <= 48) ID = 10;

    CxSetFrequency(d->handleCamera, ID);
}

QCameraSMX::Limits QCameraSMX::gainLimits()
{    
    QCameraSMX::Limits limits;
    limits.min = 0;
    limits.max = 100;
    return limits;
}


int QCameraSMX::gain()
{
    int gainMono, gainRed, gainGreen, gainBlue;
    CxGetGain(d->handleCamera, & gainMono, & gainRed,& gainGreen,& gainBlue);
    return gainMono;
}

QCameraSMX::RGBPixel QCameraSMX::gainRGB()
{
    QCameraSMX::RGBPixel rgb;
    int gainMono;
    CxGetGain(d->handleCamera, & gainMono, & rgb.red,& rgb.green,& rgb.blue);
    return rgb;
}

void QCameraSMX::setGain(int mono)
{
    int gainMono, gainRed, gainGreen, gainBlue;

    CxGetGain(d->handleCamera, & gainMono, & gainRed, & gainGreen,& gainBlue);
    CxSetGain(d->handleCamera, mono, gainRed, gainGreen, gainBlue);
}

void QCameraSMX::setGainRGB(int red,int green,int blue)
{
    int gainMono, gainRed, gainGreen, gainBlue;

    CxGetGain(d->handleCamera, & gainMono, & gainRed,& gainGreen,& gainBlue);
    CxSetGain(d->handleCamera, gainMono, red, green, blue);

}

int QCameraSMX::brightness()
{
    return settings->brightness;
}

void QCameraSMX::setBrightness(int brightness)
{
    settings->brightness = correctMinMax(brightness,-127,127);
    CxSetBrightnessContrastGamma(d->handleCamera,
                                              settings->brightness,
                                              settings->constrast,
                                              settings->gamma);
}

int QCameraSMX::constrast()
{
    return settings->constrast;
}

void QCameraSMX::setConstrast(int constrast)
{
    settings->constrast = correctMinMax(constrast,-127,127);
    CxSetBrightnessContrastGamma(d->handleCamera,
                                              settings->brightness,
                                              settings->constrast,
                                              settings->gamma);
}

int QCameraSMX::gamma()
{
    return settings->gamma;
}

void QCameraSMX::setGamma(int gamma)
{
    settings->gamma = correctMinMax(gamma,-127,127);
    CxSetBrightnessContrastGamma(d->handleCamera,
                                 settings->brightness,
                                 settings->constrast,
                                 settings->gamma);
}

int QCameraSMX::bitDepth()
{
    BOOL use10Bits;
    int bitDepth;
    CxGet10BitsOutput(d->handleCamera, & use10Bits);
    if (use10Bits == TRUE)    {
        bitDepth = 10;
    } else {
        bitDepth = 8;
    }
    return bitDepth;
}

void QCameraSMX::setBitDepth(int bits)
{
    switch(bits)
    {
    case 10:
        CxSet10BitsOutput(d->handleCamera, TRUE);
        break;
    default:
        CxSet10BitsOutput(d->handleCamera, FALSE);
    }
}

QCameraSMX::BayerAlgorithm QCameraSMX::bayerAlgorithm()
{
    return settings->bayerAlgorithm;

}

/*!
 * sets Bayer convertion algorithm for current
 * CxSetStartVideo. Note: only applicable to SMC 150 C
 */
void QCameraSMX::setBayerAlgorithm(QCameraSMX::BayerAlgorithm algorithm)
{
    settings->bayerAlgorithm = algorithm;
    CxSetBayerAlg(static_cast<int>(algorithm));
}

double QCameraSMX::frameRate()
{
    float frameRate;
    CxGetFrameRate(d->handleCamera, &frameRate);
    return static_cast<double>(frameRate);
}

int QCameraSMX::snapshotExposureMultiplier()
{
    DWORD value;
    CxGetSnapshotExposureMultiplier(d->handleCamera, & value);
    return static_cast<int>(value);

}

void QCameraSMX::setSnapshotExposureMultiplier(int mulitplier)
{
    switch (mulitplier)
    {
    case 0:
    case 1:
    case 2:
    case 3: break;
    default:
        Error("Error: invalid Snapshot Exposure Multiplier");
        return;
    }

    CxSetSnapshotExposureMultiplier(d->handleCamera, static_cast<DWORD>(mulitplier));
}

QCameraSMX::Limits QCameraSMX::snapshotExposureLimits()
{
    float minExposureMs, maxExposureMs;
    CxGetSnapshotExposureMinMaxMs(d->handleCamera, & minExposureMs, & maxExposureMs);
    QCameraSMX::Limits limits;
    limits.min = minExposureMs;
    limits.max = maxExposureMs;
    return limits;
}

QCameraSMX::Limits QCameraSMX::exposureLimits()
{
    float minExposureMs, maxExposureMs;
    CxGetExposureMinMaxMs(d->handleCamera, & minExposureMs, & maxExposureMs);
    QCameraSMX::Limits limits;
    limits.min = minExposureMs;
    limits.max = maxExposureMs;
    return limits;
}

double QCameraSMX::snapshotExposure()
{
    float exposureMs;
    CxGetSnapshotExposureMs(d->handleCamera, & exposureMs);
    return static_cast<double>(exposureMs);
}

double QCameraSMX::exposure()
{
    float exposureMs;
    CxGetExposureMs(d->handleCamera, & exposureMs);
    return static_cast<double>(exposureMs);
}

double QCameraSMX::setSnapshotExposure(double exposureMs)
{
    float exposureMsResult;
    CxSetSnapshotExposureMs(d->handleCamera, static_cast<float>(exposureMs), & exposureMsResult);
    return static_cast<double>(exposureMsResult);
}

double QCameraSMX::setExposure(double exposureMs)
{
    float exposureMsResult;
    CxSetExposureMs(d->handleCamera, static_cast<float>(exposureMs), & exposureMsResult);
    return static_cast<double>(exposureMsResult);
}

void QCameraSMX::grabVideoFrame()
{
    if (!d->isInitialized)
        return;

    CxGrabVideoFrame(d->handleCamera, d->frameArray, sizeof(d->frameArray));
}


void QCameraSMX::takeSnapshot()
{
    if (!d->isInitialized)
        return;

    d->imageReady = false;

    QTime time; time.start();
    QTime time2;
    Error("SMX Camera: processing Snapshot...");

    // only for debug
//    setShutterMode(QCameraSMX::GlobalShutter);
//    this->setBitDepth(8);
//    this->setFrequency(16);
//    this->setGain(10);
//    this->setSnapshotExposure(10);

    QString buffer;
    if (this->shutterMode() == QCameraSMX::GlobalShutter) {
        buffer.append("GlobalShutter, ");
    } else {
        buffer.append("RollingShutter, ");
    }
    buffer.append(QString("Freq: %1 MHz, ").arg(this->frequency()));
    buffer.append(QString("Gain %1 , ").arg(this->gain()));
    buffer.append(QString("Exposure %1 ms, ").arg(this->snapshotExposure()));
    buffer.append(QString("Bits %1, ").arg(this->bitDepth()));

    qDebug() << buffer;

    delete [] d->frameArray;
    int buffersize = maxFrameSize();
    if (bitDepth() > 8) {
        buffersize = buffersize * 2; // single pixel saved in 2 bytes
    }
    // qDebug() << "BufferSize: " << buffersize << ", x" << sensorSize().width() << ", y" << sensorSize().height();
    d->frameArray = new char[buffersize];

    if (this->shutterMode() == QCameraSMX::GlobalShutter) {
        // get Frame in Global Shutter Mode
        int iTimeOut=2;
        BOOL boolTriggerEnabled = FALSE;
        BOOL boolTriggerPosEnabled = FALSE;
        DWORD retlength;

        time2.start();
        CxGetSnapshot(d->handleCamera,
                      iTimeOut,
                      boolTriggerEnabled,
                      boolTriggerPosEnabled,
                      TRUE,
                      d->frameArray, // pointer to frame buffer
                      buffersize, // buffer size in bytes (8 bit)
                      &retlength);   //

        qDebug() << QString("CxGetSnapshot: %1 ms ").arg(time2.elapsed());

        unsigned int checksize = this->viewPort().width() * this->viewPort().height();
        // 1310720
        // 1232276
        if (retlength != checksize)
        {
            Error(QString("Error while getting Snapshot: wrong buffer length(%1)").arg(retlength));
            return;
        }
        if (retlength == 0) {
            Error("SMX Camera Error: Snapshot with empty frame");
            return;
        }

    } else { // Rolling Shutter
        time2.start();
        CxGrabVideoFrame(d->handleCamera,
                         d->frameArray, // pointer to frame buffer
                         buffersize);   // buffer size in bytes (8 bit)

        qDebug() << QString("CxGrabVideoFrame: %1 ms ").arg(time2.elapsed());
    }


    if (sensorType() == Color) {
        BayerToRgb();
    } else {
        convertMonoToRGBArray();
    }
    time2.start();
    convertRGBtoQImage();
    qDebug() << QString("convertRGBtoQImage: %1 ms ").arg(time2.elapsed());

    Error("SMX Camera: Snapshot finished" + QString(" in %1 ms").arg(time.elapsed()));
}

void QCameraSMX::BayerToRgb()
{		   
    delete [] d->frameArrayRgb;
    d->frameArrayRgb = new TRgbPix[viewSize()];

    d->frameArrayRgb = CxBayerToRgb(
            d->frameArray,
            viewPort().width(),
            viewPort().height(),
            static_cast<int>(bayerAlgorithm()),
            d->frameArrayRgb);
}

void QCameraSMX::convertMonoToRGBArray()
{
    QTime time; time.start();

    delete [] d->frameArrayRgb;
    d->frameArrayRgb = new TRgbPix[viewSize()];
    TRgbPix pixel;

    if (bitDepth() == 8) {
        // convert 8 bit into 24 Bit Array

        int size = viewSize();
        for (int i = 0; i < size; ++i)
        {
            BYTE gray = d->frameArray[i];
            pixel.r = gray;
            pixel.g = gray;
            pixel.b = gray;
            d->frameArrayRgb[i] = pixel;
        }
    } else {
        int *pixelArray = new int[viewSize()];
        char* pPixelPointer;
        int bytesPerPixel = 2;
        void *pMemVoid = d->frameArray;
        unsigned int nPixVal;
        for (int i = 0; i < viewSize(); ++i)
        {
            int iMemoryPixelOffset = (i * bytesPerPixel);
            pPixelPointer = (reinterpret_cast<char*> (pMemVoid) + (iMemoryPixelOffset));
            nPixVal = *(reinterpret_cast<unsigned int*>(const_cast<char*>(pPixelPointer)));
            unsigned char cPixVal = (nPixVal & 0xffc0) >> 2;
            pixelArray[i] = static_cast<int>(cPixVal);

            pixel.r = pixel.g = pixel.b = pixelArray[i]/1024;
            d->frameArrayRgb[i] = pixel;
        }
        delete [] pixelArray;
    }
    qDebug() << "convertMonoToRGBArray()" << time.elapsed();
}

void QCameraSMX::convertRGBtoQImage()
{
    CxGetScreenParams(d->handleCamera, &d->cameraParameters);
    int size_x = d->cameraParameters.Width / decimation();
    int size_y = d->cameraParameters.Height / decimation();

    if (!d->imageReady) {
        d->image = QImage(
                (uchar*)d->frameArrayRgb,
                viewPort().width(),
                viewPort().height(),
                QImage::Format_RGB888);
        d->image.setColorCount(8);

        d->imageReady = true;
    }
    if ((d->frameArrayRgb != 0) && !d->imageReady) {
        d->image = QImage(size_x, size_y, QImage::Format_RGB32);
        d->image.fill(255);
        d->imageReady = true;
    }
}

vector<int> QCameraSMX::histogramm()
{
    int histChannels = pow(2.0,double(bitDepth()));
    vector<int> histogramm(histChannels);

    if (sensorType() == Color) {

    } else {
        int size = viewSize();
        for (int i = 0; i < size; ++i)
        {
            int gray = d->frameArrayRgb[i].g;
            if (gray < histChannels) {
                histogramm[gray] += 1;
            }
        }
    }
    return histogramm;
}

int QCameraSMX::peakValue()
{
    int max = 0;
    if (sensorType() == Color) {

    } else {
        int size = viewSize();
        for (int i = 0; i < size; ++i)
        {
            int gray = d->frameArrayRgb[i].g;
            if (gray > max) {
                max = gray;
            }
        }
    }
    return max;
}


QImage QCameraSMX::image()
{
    convertRGBtoQImage();
    return d->image;
    //return QImage((uchar*)d->frameArrayRgb, viewPort().width(), viewPort().height(), QImage::Format_RGB888);
}

QCameraSMX::ShutterMode QCameraSMX::shutterMode()
{
    BYTE mode;
    CxGetStreamMode(d->handleCamera, & mode);
    return static_cast<QCameraSMX::ShutterMode>(mode);
}

void QCameraSMX::setShutterMode(QCameraSMX::ShutterMode shutterMode)
{
    BYTE mode = static_cast<BYTE>(shutterMode);
    CxSetStreamMode(d->handleCamera, mode);
}

void QCameraSMX::setDACOffsetRaw(int offset)
{
    offset = this->correctMinMax(offset, 0, 127);
    CxSetDACRawOffset(d->handleCamera, static_cast<BYTE>(offset));
}

void QCameraSMX::setDACOffsetFine(int offset)
{
    offset = this->correctMinMax(offset, 0, 127);
    CxSetDACFineOffset(d->handleCamera, static_cast<BYTE>(offset));
}

int QCameraSMX::DACOffsetRaw()
{
    BYTE offset;
    CxGetDACRawOffset(d->handleCamera, &offset);
    return offset;
}

int QCameraSMX::DACOffsetFine()
{
    BYTE offset;
    CxGetDACFineOffset(d->handleCamera, &offset);
    return offset;
}

void QCameraSMX::startVideo()
{
    if( d->handleCamera != 0 )
    {
        if (d->widgetVideoWindow != 0 ) {
            // Capture live video
            d->widgetVideoWindow->setVideoActive(true);
            d->windowDC = d->widgetVideoWindow->getDC();
            int DeviceID = 0;
            CxStartVideo(d->windowDC, DeviceID);
        }
    }
}

void QCameraSMX::stopVideo()
{
    // stop live immediately - force stop
    if(( d->handleCamera != 0) && (d->windowDC != 0)) {
        int DeviceID = 0;
        CxStopVideo(DeviceID);
        d->widgetVideoWindow->setVideoActive(false);
        d->widgetVideoWindow->releaseDC(d->windowDC);
        d->windowDC = 0;
    }
}

//
//BOOL  SMX150_API CxGrabVideoFrame(d->handleCamera,void *Buffer,int BufSize);
//PVOID SMX150_API CxGetFramePtr(d->handleCamera);

//CxCancelSnapshot(d->handleCamera);
//CxElectricalBlack(d->handleCamera, BOOL ElectricalBlack);
//CxWriteSensorReg(d->handleCamera, DWORD RegNo, DWORD  Value);
//CxReadSensorReg (d->handleCamera, DWORD RegNo, DWORD *Value);
//

void QCameraSMX::saveFrameToTextFile(QString filename)
{
    //    int xdim = viewSize().width();
    //    int ydim = viewSize().height();
    //    int textwidth = 10;
    //
    //    ofstream FileOut;
    //    FileOut.open(szPathName.c_str());
    //    FileOut.flags(ios::right);
    //    if (FileOut)
    //    {
    //        FileOut << setw(textwidth)
    //        {
    //            for (int ix=0; ix < xdim; ix++)
    //            {
    //                FileOut << d->frameArray[iy + ydim * ix];
    //            }
    //            FileOut << endl;
    //        }
    //    }
    //    else
    //    {
    //        qDebug() << "Datei konnte nicht geöffnet werden";
    //    }
    //    FileOut.close();
}



