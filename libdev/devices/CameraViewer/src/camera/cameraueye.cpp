#include "cameraueye.h"

#include "cameraueye_p.h"

#include <QMutexLocker>

using std::vector;

CameraUEye::CameraUEye()
    : d_ptr(new CameraUEyePrivate)
{

}

CameraUEye::~CameraUEye()
{
    // for QScopedPointer required.
}

/** Initialize - find number of devices and return first camera */
bool CameraUEye::initialize()
{
    Q_D(CameraUEye);
    if (d->numberOfDevices() != 0)
        return true;
    else
        return false;
}

void CameraUEye::openCamera()
{
    Q_D(CameraUEye);
    d->openCamera();
}

void CameraUEye::closeCamera()
{
    Q_D(CameraUEye);
    d->closeCamera();
}

float CameraUEye::exposure() const
{
    Q_D(const CameraUEye);
    return d->exposure();
}

void CameraUEye::setExposure(float exposure_ms)
{
    Q_D(CameraUEye);
    d->setExposure(exposure_ms);
}

QRect CameraUEye::roi() const
{
    Q_D(const CameraUEye);
    return d->roi();
}

void CameraUEye::setROI(QRect roi)
{
    Q_D(CameraUEye);
    d->setROI(roi);
}

void CameraUEye::capture()
{
    Q_D(CameraUEye);
    d->getImage();

    QImage image = d->toQImage();
    d->updateImageData(image);
}


QImage CameraUEye::image()
{
    Q_D(CameraUEye);

    QMutexLocker locker(&d->mutex);
    return d->lastImage;
}

vector<int> CameraUEye::data()
{
    Q_D(CameraUEye);

    QMutexLocker locker(&d->mutex);
    return d->grayVector;
}

vector<int> CameraUEye::histogram()
{
    Q_D(CameraUEye);

    QMutexLocker locker(&d->mutex);
    return d->histVector;
}
