#ifndef CAMERA_SMX_H_
#define CAMERA_SMX_H_

#include "QCameraSMXGlobal.h"

#include "QSMXDLL.h"

#include <QObject>
#include <QtCore/QRect>
#include <QtCore/QDebug>
#include <QtCore/QSize>

#include <QImage>

class QCameraImage;
class QCameraSMXPrivate;
class QCameraSettingsPrivate;

////////////////////////////////////////////////////////////////////////
class QSMXCAMERA_EXPORT QCameraSMX : public QSMXDLL // public QObject,
{
    Q_OBJECT;
public:
    QCameraSMX(QObject *parent = 0);
    ~QCameraSMX(void);

    enum ShutterMode { GlobalShutter = 0, RollingShutter = 1};

    struct Limits {
        Limits(): min(0), max(0) {}
        double min;
        double max;
    };

    enum SensorType {
        Monochrome = 0,
        Color = 1
    };

    class RGBPixel {
    public:
        RGBPixel()
            : red(0)
            , blue(0)
            , green(0)
        {}
        int red;
        int blue;
        int green;
    };


    enum BayerAlgorithm {
        SimpleMonochrome = 0,
        NearestNeighbor = 1,
        Bilinear = 2,
        Laplacian = 3,
        RealMonochrome = 4,
        BayerAverage = 5
    };


private:
    QCameraSMXPrivate * const  d;
    QCameraSettingsPrivate * const settings;


private:
    void Error(const QString & message);

    template <class T>
            T correctMinMax(T value, T min, T max)
    {
        if (value > max) return max;
        if (value < min) return min;
        return value;
    }

    int maxFrameSize();
    void BayerToRgb();
    void convertMonoToRGBArray();
    void convertRGBtoQImage();


public:
    bool isConnected();
    bool isInitialized();

    bool initialize();

    QString deviceName();
    SensorType sensorType();
    QSize sensorSize();

    int modelID();
    int version();
    int serial();

    int colorDepth();
    void setColorDepth(int depth);

    int decimation();
    void setDecimation(int decimation);

    QRect viewPort();
    int viewSize();
    void setViewPort(QRect rect);

    bool mirrorVertical();
    void setMirrorVertical(bool mirror);
    bool mirrorHorizontal();
    void setMirrorHorizontal(bool mirror);


    double frequency();
    void setFrequency(double frequency);

    Limits gainLimits();
    int gain();
    RGBPixel gainRGB();
    void setGain(int mono);
    void setGainRGB(int red,int green,int blue);

    int brightness();
    void setBrightness(int brightness);

    int constrast();
    void setConstrast(int constrast);

    int gamma();
    void setGamma(int gamma);

    int bitDepth();
    void setBitDepth(int bits);

    BayerAlgorithm bayerAlgorithm();
    void setBayerAlgorithm(BayerAlgorithm algorithm);

    double frameRate();

    int snapshotExposureMultiplier();
    void setSnapshotExposureMultiplier(int mulitplier);
    Limits snapshotExposureLimits();
    Limits exposureLimits();
    double snapshotExposure();
    double exposure();
    double setSnapshotExposure(double exposureMs);
    double setExposure(double exposureMs);

    ShutterMode shutterMode();
    void setShutterMode(ShutterMode shutterMode);

    void setDACOffsetRaw(int offset);
    void setDACOffsetFine(int offset);
    int DACOffsetRaw();
    int DACOffsetFine();

    void grabVideoFrame();
    void takeSnapshot();

    void setWidgetVideoWindow(QCameraImage * widget);

    void startVideo();
    void stopVideo();

    QImage image();
    std::vector<int> histogramm();
    int peakValue();

    void saveFrameToTextFile(QString filename);    

signals:
    void errorOccured(QString error);
    void connectionStatusChanged(bool);

};

#endif
