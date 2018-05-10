#ifndef QUEYECAMERA_H
#define QUEYECAMERA_H

#include "QCameraGlobal.h"
#include "uEye.h"
#include "QRgbMatrix.h"

#include <QtWidgets/QWidget>
#include <QImage>
#include <QRect>
#include <QSize>

class QuEyeCameraPrivate;
class QCameraImage;

class QCAMERA_EXPORT QuEyeCamera : public QWidget
{
    Q_OBJECT
public:
    QuEyeCamera(QWidget* parent = 0);
    virtual ~QuEyeCamera();

    struct RGBPixel
    {
        RGBPixel(): red(0), blue(0), green(0) {}
        int red;
        int blue;
        int green;
    };

    struct RGB32Pixel
    {
        unsigned char r;
        unsigned char g;
        unsigned char b;
        unsigned char x; // unused, 32bit alignment
    };

    struct RGB24Pixel
    {
        unsigned char r;
        unsigned char g;
        unsigned char b;
    };

    struct BGR32Pixel
    {
        unsigned char b;
        unsigned char g;
        unsigned char r;
        unsigned char x; // unused, 32bit alignment
    };

    struct BGR24Pixel
    {
        unsigned char b;
        unsigned char g;
        unsigned char r;
    };


    struct Limits {
        Limits(): min(0), max(0), intervall(0) {}
        Limits(double nmin, double nmax, double nintervall=0)
            : min(nmin)
            , max(nmax)
            , intervall(nintervall) {}
        double min;
        double max;
        double intervall;
    };

    enum ShutterMode { GlobalShutter = IS_SET_GLOBAL_SHUTTER_ON, RollingShutter = IS_SET_GLOBAL_SHUTTER_OFF};

    enum ColorMode {
        RGB32 = IS_CM_RGBA8_PACKED,
        RGB24 = IS_CM_RGB8_PACKED,
        BGR32 = IS_CM_BGRA8_PACKED,
        BGR24 = IS_CM_BGR8_PACKED,
        BGR16 = IS_CM_BGR565_PACKED,
        BGR15 = IS_CM_BGR555_PACKED,
        MONO8 = IS_CM_MONO8,
        MONO12 = IS_CM_MONO12,
        MONO16 = IS_CM_MONO16,
    };

    enum SensorType {
        Monochrome = IS_COLORMODE_MONOCHROME,
        Color = IS_COLORMODE_BAYER
    };

public:
    int initCamera (HIDS *hCam, HWND hWnd);

    bool isInitialized();
    bool isConnected();

    void setWindowHandle(HWND hWnd);
    void setWidgetVideoWindow(QCameraImage *widget);
    bool openCamera();
    bool allocateMemmory();
    void loadParameter();
    QSize sensorSize();
    QSize frameSize();

    void takeSnapshot();

    void startVideo();
    void stopVideo();

    QImage image();
    QRgbMatrix & rgbMatrix();

    SensorType sensorType();
    QString deviceName();
    QString serial();

    int bitsPerPixel();

    bool setColorMode(int colorMode);
    int colorMode();


    double framesPerSecond();
    double setFramesPerSecond(double fps);
    Limits framesPerSecondRange();

    Limits pixelClockRange();
    void setPixelClock(int clock);
    int pixelClock();


    void setBlackCompensation(bool enabled, int offset);
    void getBlackCompensation(bool & enabled, int & offset);

    Limits exposureRange();
    double setExposure(double exposure);
    double exposure();

    void setGainBoostEnabled(bool enable);
    bool isGainBoostEnabled();

    void setGamma(int value);
    int gamma();

    void setShutterMode(ShutterMode mode);
    ShutterMode  shutterMode();
    bool isGlobalShutterSupported();

    void setGain(int master);
    void setGainRGB(int red,int green,int blue);
    int gain();
    RGBPixel gainRGB();
    QuEyeCamera::Limits gainLimits() { return QuEyeCamera::Limits(0,100); }

    int colorDepth();

    void setAOI(QRect AOI);
    QRect AOI();
    void setViewPort(QRect AOI);
    QRect viewPort();

    void setBinning(int factor);
    int binning();

    void setWhiteBalanceEnabled(bool enable);
    bool whiteBalanceEnabled();

    void setAutoBrightness(int value);
    int autoBrightness();

    void saveParameters(QString filename);
    void loadParameters(QString filename);

    void GetHistogramm(std::vector<int> & red, std::vector<int> & green, std::vector<int> & blue);


private:
    QImage::Format imageFormatQt();
    void convertMemoryToRGBMatrix();
    void pixelToRGB(const char* pPixelPointer, int & r, int & g, int & b);

    bool winEventFilter ( MSG * msg, long * result );
    void Error(const QString & message);

    template <class T>
            T correctMinMax(T value, T min, T max)
    {
        if (value > max) return max;
        if (value < min) return min;
        return value;
    }


private:

    QuEyeCameraPrivate * const d;

signals:
    void errorOccured(QString message);
    void connectionStatusChanged();
    void snapShotFinished();

};

#endif // QUEYECAMERA_H
