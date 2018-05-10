#include "globaldevices.h"
#include "QCameraSMX150.h"

QCameraSMX* cameraSMX;

GlobalDevices::GlobalDevices()
{
    cameraSMX = 0;
}

GlobalDevices::~GlobalDevices()
{
    delete cameraSMX; cameraSMX = 0;
}

