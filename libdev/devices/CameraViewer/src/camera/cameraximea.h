#ifndef CAMERAXIMEA_H
#define CAMERAXIMEA_H

#include <QObject>
#include "camerainterface.h"

class CameraXimea : public CameraInterface, QObject
{
     Q_OBJECT
public:
    CameraXimea();



};

#endif // CAMERAXIMEA_H
