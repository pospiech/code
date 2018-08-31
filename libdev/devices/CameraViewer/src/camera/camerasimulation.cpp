#include "camerasimulation.h"
#include "camerasimulation_p.h"

using std::vector;

CameraSimulation::CameraSimulation()
    : d_ptr(new CameraSimulationPrivate)
{

}

CameraSimulation::~CameraSimulation()
{
    // for QScopedPointer required.
}

/** Initialize - find number of devices and return first camera */
bool CameraSimulation::initialize()
{
    return true;
}

void CameraSimulation::openCamera()
{
    Q_D(CameraSimulation);
    d->openCamera();
}

void CameraSimulation::closeCamera()
{
    Q_D(CameraSimulation);
    d->closeCamera();
}

bool CameraSimulation::isOpen()
{
    Q_D(CameraSimulation);
    return d->isOpen;
}


float CameraSimulation::exposure() const
{
    Q_D(const CameraSimulation);
    return d->exposure();
}

void CameraSimulation::setExposure(float exposure_ms)
{
    Q_D(CameraSimulation);
    d->setExposure(exposure_ms);
}

QRect CameraSimulation::roi() const
{
    Q_D(const CameraSimulation);
    return d->roi();
}

void CameraSimulation::setROI(QRect roi)
{
    Q_D(CameraSimulation);
    d->setROI(roi);
}

void CameraSimulation::capture()
{
    Q_D(CameraSimulation);
    d->createData();
}


QImage CameraSimulation::image()
{
    Q_D(CameraSimulation);

    QMutexLocker locker(&d->mutex);
    return d->lastImage;
}

vector<int> CameraSimulation::data()
{
    Q_D(CameraSimulation);

    QMutexLocker locker(&d->mutex);
    return d->grayVector;
}

vector<int> CameraSimulation::histogram()
{
    Q_D(CameraSimulation);

    QMutexLocker locker(&d->mutex);
    return d->histVector;
}
