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
    QString	errorString() {return ""; }

    void openCamera();
    void closeCamera();

    float exposure() const;
    void setExposure(float exposure_ms);

    void capture();

    QRect roi() const;
    void setROI(QRect roi);

    bool initialize();

    QImage image();

    std::vector<int> data();
    std::vector<int> histogram();

protected:
    const QScopedPointer<CameraUEyePrivate> d_ptr;

private:
    // requires d_ptr
    Q_DECLARE_PRIVATE(CameraUEye)

};


#endif // CAMERAUEYE_H
