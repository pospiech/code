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

    void capture();

    QRect roi() const;
    void setROI(QRect roi);
    QSize sensorSize();

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
