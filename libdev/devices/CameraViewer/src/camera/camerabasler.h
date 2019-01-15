#ifndef CAMERABASLER_H
#define CAMERABASLER_H

#include <QObject>
#include "camerainterface.h"

class CameraBaslerPrivate;

class CameraBasler : public QObject, public CameraInterface
{
    Q_OBJECT
public:
    explicit CameraBasler(QObject *parent = nullptr);
    virtual ~CameraBasler() = default;

    QString description() const { return "Basler"; }
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
    const QScopedPointer<CameraBaslerPrivate> d_ptr;

private:
    // requires d_ptr
    Q_DECLARE_PRIVATE(CameraBasler)




public slots:
};

#endif // CAMERABASLER_H
