#ifndef CAMERAINTERFACE_H
#define CAMERAINTERFACE_H

#include<QtCore/QRect>
#include<QImage>

class CameraInterface
{
public:
    virtual ~CameraInterface()
    {

    }

    virtual QString description() const = 0;
    virtual QString	errorString() const = 0;

    virtual void capture() = 0;

    virtual void openCamera() = 0;
    virtual void closeCamera() = 0;

    virtual float exposure() const = 0;
    virtual void setExposure(float exposure) = 0;

    virtual QRect roi() const = 0;
    virtual void setROI(QRect roi) = 0;

};

#endif // CAMERAINTERFACE_H
