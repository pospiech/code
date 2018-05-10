#include "QCameraImage.h"

#include <QPainter>

class QCameraImagePrivate
{
public:
    QCameraImagePrivate()
        : crossX(0), crossY(0)
    {}

    int crossX;
    int crossY;

};

QCameraImage::QCameraImage(QWidget *parent)
    : QCameraImageBase(parent)
    , d(new QCameraImagePrivate)
{
}

QCameraImage::~QCameraImage()
{
    delete d;
}

void QCameraImage::paintEventExtras(QPainter & painter)
{
    painter.drawRect(QRect(d->crossX,d->crossY,20,20));
    painter.drawRect(QRect(d->crossX-10,d->crossY-10,40,40));
}

void QCameraImage::setCross(int x, int y)
{
    d->crossX = x;
    d->crossY = y;
}
