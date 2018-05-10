#ifndef QCameraImage_H
#define QCameraImage_H

#include "QCameraGlobal.h"
#include "QCameraImageBase.h"

class QCameraImagePrivate;

class QCAMERA_EXPORT QCameraImage : public QCameraImageBase
{
public:
    QCameraImage(QWidget* parent = 0);
    virtual ~QCameraImage();

public:
    void paintEventExtras(QPainter & painter);
    void setCross(int x, int y);

private:
    QCameraImagePrivate * const d;

};

#endif // QCameraImage_H
