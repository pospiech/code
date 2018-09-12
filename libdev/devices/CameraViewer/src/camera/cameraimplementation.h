#ifndef CAMERAIMPLEMENTATION_H
#define CAMERAIMPLEMENTATION_H

#include <vector>
#include <QImage>
#include <QMutex>
#include <QMutexLocker>
#include <QSize>
#include <exception>


#include "Logger.h"

class CameraImplementation
{
public:
    CameraImplementation();

    bool isOpen;

    std::vector<int> grayVector;
    std::vector<int> histVector;
    QSize sizeSensor;

    void updateImageData(const QImage &nextImage);
    QImage toQImage(const QSize imageSize);

    QImage lastImage;
    QMutex mutex;

};

#endif // CAMERAIMPLEMENTATION_H
