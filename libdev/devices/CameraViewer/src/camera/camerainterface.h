#ifndef CAMERAINTERFACE_H
#define CAMERAINTERFACE_H

#include<QtCore/QRect>
#include<QImage>
#include <vector>

class CameraInterface
{
public:
    virtual ~CameraInterface()
    {

    }

    virtual QString description() const = 0;
    virtual QString	errorString() = 0;

    virtual void capture() = 0;

    virtual bool initialize() = 0;
    virtual void openCamera() = 0;
    virtual void closeCamera() = 0;

    virtual float exposure() const = 0;
    virtual void setExposure(float exposure) = 0;

    virtual QRect roi() const = 0;
    virtual void setROI(QRect roi) = 0;

    virtual std::vector<int> data() = 0;
    virtual std::vector<int> histogram() = 0;
    virtual QImage image() = 0;
};

#endif // CAMERAINTERFACE_H
