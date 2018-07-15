#include "cameraximea.h"
#include "cameraximea_p.h"

using std::vector;

CameraXimea::CameraXimea()
    : d_ptr(new CameraXimeaPrivate)
{

}

CameraXimea::~CameraXimea()
{
    // for QScopedPointer required.
}

/** Initialize - find number of devices and return first camera */
bool CameraXimea::initialize()
{
    Q_D(CameraXimea);
    size_t count = d->numberOfDevices();

    if (count > 0){
        d->setCameraNumber(0); // numbers start at 0
        return true;
    }
    else{
        return false;
    }

}

void CameraXimea::openCamera()
{
    Q_D(CameraXimea);
    d->openCamera(d->cameraNumber);
}

void CameraXimea::closeCamera()
{
    Q_D(CameraXimea);
    d->closeCamera();
}

float CameraXimea::exposure() const
{
    Q_D(const CameraXimea);
    return d->exposure();
}

void CameraXimea::setExposure(float exposure_ms)
{
    Q_D(CameraXimea);
    d->setExposure(exposure_ms);
}

QRect CameraXimea::roi() const
{
    Q_D(const CameraXimea);    
    return d->roi();
}

void CameraXimea::setROI(QRect roi)
{
    Q_D(CameraXimea);
    d->setROI(roi);
}

void CameraXimea::capture()
{
    Q_D(CameraXimea);
    d->startAcquisition();
    d->getImage();
    d->stopAcquisition();

    QImage image = d->toQImage(&d->image);
    d->updateImageData(image);
}


QImage CameraXimea::image()
{
    Q_D(CameraXimea);

    QMutexLocker locker(&d->mutex);
    return d->lastImage;
}

vector<int> CameraXimea::data()
{
    Q_D(CameraXimea);

    QMutexLocker locker(&d->mutex);
    return d->grayVector;
}

vector<int> CameraXimea::histogram()
{
    Q_D(CameraXimea);

    QMutexLocker locker(&d->mutex);
    return d->histVector;
}
