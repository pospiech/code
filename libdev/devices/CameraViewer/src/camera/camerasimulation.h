#ifndef CAMERASIMULATION_H
#define CAMERASIMULATION_H

#include <QObject>
#include "camerainterface.h"

class CameraSimulationPrivate;

class CameraSimulation: public QObject, public CameraInterface
{
    Q_OBJECT
public:

    CameraSimulation();
    virtual ~CameraSimulation();

    QString description() const { return "Camera Simulation"; }
    QString deviceName() { return "Simulation"; }
    QString deviceSerial() {return "0123"; }


    void openCamera();
    void closeCamera();
    bool isOpen();

    float exposure() const;
    void setExposure(float exposure_ms);
    float exposureMax() const {return 1000.0f;}
    float exposureMin() const {return 0.001f;}

    float gain() const {return 1.0f;}
    float gainMax() const{return 10.0f;}
    float gainMin() const {return 0.0f;}
    void setGain(float gain) {}


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
    const QScopedPointer<CameraSimulationPrivate> d_ptr;

private:
    // requires d_ptr
    Q_DECLARE_PRIVATE(CameraSimulation)



};



#endif // CAMERASIMULATION_H
