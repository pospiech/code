#ifndef CAMERAXIMEA_H
#define CAMERAXIMEA_H

#include <QObject>
#include "camerainterface.h"

#include<QtCore/QScopedPointer>

class CameraXimeaPrivate;

class CameraXimea : public QObject, public CameraInterface
{
     Q_OBJECT
public:
    CameraXimea();
    virtual ~CameraXimea();

    QString description() const { return "XIMEA"; }
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
    int binningIncrementX() const;
    int binningIncrementY() const;

    bool initialize();

    QImage image();

    std::vector<int> data();
    std::vector<int> histogram();

    QMap<QString, QString> getParamters();

protected:
    const QScopedPointer<CameraXimeaPrivate> d_ptr;

private:
    // requires d_ptr
    Q_DECLARE_PRIVATE(CameraXimea)

};


#endif // CAMERAXIMEA_H
