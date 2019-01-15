#include "camerabasler.h"
#include "camerabasler_p.h"

CameraBasler::CameraBasler(QObject *parent)
    : QObject(parent)
    , d_ptr(new CameraBaslerPrivate)
{

}


/** Initialize - find number of devices and return first camera */
bool CameraBasler::initialize()
{
    Q_D(CameraBasler);    
    return d->initialize();
}

void CameraBasler::openCamera()
{
    Q_D(CameraBasler);
    d->openCamera();
}

void CameraBasler::closeCamera()
{
    Q_D(CameraBasler);
    d->closeCamera();
}

bool CameraBasler::isOpen()
{
    Q_D(CameraBasler);
    return d->isOpen;
}

float CameraBasler::exposure() const
{
    Q_D(const CameraBasler);
    return d->exposure();
}

void CameraBasler::setExposure(float exposure_ms)
{
    Q_D(CameraBasler);
    d->setExposure(exposure_ms);
}

float CameraBasler::exposureMax() const
{
    Q_D(const CameraBasler);
    return d->exposureMax();
}

float CameraBasler::exposureMin() const
{
    Q_D(const CameraBasler);
    return d->exposureMin();
}


float CameraBasler::gain() const
{
    Q_D(const CameraBasler);
    return d->gain();
}

float CameraBasler::gainMax() const
{
    Q_D(const CameraBasler);
    return d->gainMax();
}

float CameraBasler::gainMin() const
{
    Q_D(const CameraBasler);
    return d->gainMin();
}

void CameraBasler::setGain(float gain)
{
    Q_D(CameraBasler);
    d->setGain(gain);
}

QRect CameraBasler::roi() const
{
    Q_D(const CameraBasler);
    return d->roi();
}

void CameraBasler::setROI(QRect roi)
{
    Q_D(CameraBasler);
    d->setROI(roi);
}

QSize CameraBasler::sensorSize()
{
    Q_D(CameraBasler);
    return d->sensorSize();
}

int CameraBasler::binningIncrementX() const
{
    Q_D(const CameraBasler);
    return d->binningIncrementX();
}

int CameraBasler::binningIncrementY() const
{
    Q_D(const CameraBasler);
    return d->binningIncrementY();
}



void CameraBasler::capture()
{
    Q_D(CameraBasler);
    d->startAcquisition();
    d->getImage();
    d->stopAcquisition();

    d->updateImageData(d->toQImage(d->imageSize()));
}


QImage CameraBasler::image()
{
    Q_D(CameraBasler);

    QMutexLocker locker(&d->mutex);
    return d->lastImage;
}

vector<int> CameraBasler::data()
{
    Q_D(CameraBasler);

    QMutexLocker locker(&d->mutex);
    return d->grayVector;
}

vector<int> CameraBasler::histogram()
{
    Q_D(CameraBasler);

    QMutexLocker locker(&d->mutex);
    return d->histVector;
}

QMap<QString, QString> CameraBasler::getParamters()
{
    Q_D(CameraBasler);
    return d->getParamters();
}

QString CameraBasler::deviceName()
{
    Q_D(CameraBasler);
    const QString deviceName = d->getParamters().key("Device Name");
    qDebug() << deviceName ;
    return deviceName;
}

QString CameraBasler::deviceSerial()
{
    Q_D(CameraBasler);
    const QString serial = d->getParamters().key("Serial");
    qDebug() << serial ;
    return serial;

}

