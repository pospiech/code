#ifndef CAMERAUEYE_H
#define CAMERAUEYE_H

#include <QObject>
#include "camerainterface.h"
#include <uEye.h>

#include<QtCore/QScopedPointer>

class CameraUEyePrivate;


class CameraUEye: public QObject, public CameraInterface
{
    Q_OBJECT
public:

    CameraUEye();
    virtual ~CameraUEye();

    QString description() const { return "uEye"; }
    QString deviceName();
    QString deviceSerial();


    void openCamera();
    void closeCamera();
    bool isOpen();

    float exposure() const;
    void setExposure(float exposure_ms);
    float exposureMax() const;
    float exposureMin() const;

    float gain() const;
    float gainMax() const;
    float gainMin() const;
    void setGain(float gain);

    void capture();

    QRect roi() const;
    void setROI(QRect roi);
    QSize sensorSize();
    int binningIncrementX() const { return 1;}
    int binningIncrementY() const { return 1;}

    bool initialize();

    QImage image();

    std::vector<int> data();
    std::vector<int> histogram();

    QMap<QString, QString> getParamters();

protected:
    const QScopedPointer<CameraUEyePrivate> d_ptr;

private:
    // requires d_ptr
    Q_DECLARE_PRIVATE(CameraUEye)

};


#endif // CAMERAUEYE_H
