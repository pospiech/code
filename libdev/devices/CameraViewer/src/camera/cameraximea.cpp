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

bool CameraXimea::isOpen()
{
    Q_D(CameraXimea);
    return d->isOpen;
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

float CameraXimea::exposureMax() const
{
    Q_D(const CameraXimea);
    return d->exposureMax();
}

float CameraXimea::exposureMin() const
{
    Q_D(const CameraXimea);
    return d->exposureMin();
}


float CameraXimea::gain() const
{
    Q_D(const CameraXimea);
    return d->gain();
}

float CameraXimea::gainMax() const
{
    Q_D(const CameraXimea);
    return d->gainMax();
}

float CameraXimea::gainMin() const
{
    Q_D(const CameraXimea);
    return d->gainMin();
}

void CameraXimea::setGain(float gain)
{
    Q_D(CameraXimea);
    d->setGain(gain);
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

QSize CameraXimea::sensorSize()
{
    Q_D(CameraXimea);
    return d->sensorSize();
}

int CameraXimea::binningIncrementX() const
{
    Q_D(const CameraXimea);
    return d->binningIncrementX();
}

int CameraXimea::binningIncrementY() const
{
    Q_D(const CameraXimea);
    return d->binningIncrementY();
}



void CameraXimea::capture()
{
    Q_D(CameraXimea);
    d->startAcquisition();
    d->getImage();
    d->stopAcquisition();

    d->updateImageData(d->toQImage(d->imageSize()));
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

QMap<QString, QString> CameraXimea::getParamters()
{
    Q_D(CameraXimea);
    return d->getParamters();
}

QString CameraXimea::deviceName()
{
    Q_D(CameraXimea);
    return d->deviceInfo(d->cameraNumber, XI_PRM_DEVICE_NAME);
}

QString CameraXimea::deviceSerial()
{
    Q_D(CameraXimea);
    return d->deviceInfo(d->cameraNumber, XI_PRM_DEVICE_SN);
}

