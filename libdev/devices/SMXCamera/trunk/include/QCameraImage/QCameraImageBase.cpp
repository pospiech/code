#include "QCameraImageBase.h"

#include <QtCore/QSize>
#include <QtCore/QString>
#include <QtCore/QTime>
#include <QtGui/QPainter>
#include <QtCore/QDebug>


class QCameraImagePrivate{
public:
    QCameraImagePrivate()
        : imageChanged(true)
        , doAutoResize(true)
        , isVideoActive(false)
        , aspectRatio(Qt::IgnoreAspectRatio)
    {}

    QPixmap pixmap;
    QSize viewSize;
    QTime time;
    bool imageChanged;
    bool doAutoResize;
    bool isVideoActive;
    Qt::AspectRatioMode aspectRatio;
};

QCameraImageBase::QCameraImageBase(QWidget* parent, Qt::WFlags flags)
    : QWidget(parent, flags)
    , d(new QCameraImagePrivate)
{
    setAttribute(Qt::WA_NoSystemBackground);
    setAttribute(Qt::WA_NativeWindow);
    setAttribute(Qt::WA_DontCreateNativeAncestors);
}

QCameraImageBase::~QCameraImageBase()
{
    delete d;
}

void QCameraImageBase::resizeEvent ( QResizeEvent * event  )
{
    update();
}

void QCameraImageBase::paintEvent(QPaintEvent * event)
{
    if (d->isVideoActive)
        return;

    if (d->pixmap.width() * d->pixmap.height() == 0)
        return;

    QPainter painter(this);
    if (!d->viewSize.isValid() || d->doAutoResize) {
        d->viewSize = this->size();
    }
    int width = d->pixmap.width();
    int height = d->pixmap.height();
    double scaleFracX = double(d->viewSize.width())/double(width);
    double scaleFracY = double(d->viewSize.height())/double(height);
    painter.scale(scaleFracX, scaleFracY);
    //d->pixmap = d->pixmap.scaled(scaleFracX, scaleFracY, d->aspectRatio);
    painter.drawPixmap(0, 0, d->pixmap);

    paintEventExtras(painter);

    if (d->imageChanged) {
        qDebug() << "Time (paintEvent): " << d->time.elapsed();
    }
    d->imageChanged = false;
}

void QCameraImageBase::paintEventExtras(QPainter & painter)
{

}

HWND QCameraImageBase::windowHandle()
{
    return static_cast<HWND>(this->winId());
}

void QCameraImageBase::setImage(const QImage & image)
{
    d->time.start();
    d->imageChanged = true;
    d->pixmap = QPixmap::fromImage(image);
    update(); // calls paintEvent
}

void QCameraImageBase::setPixmap(const QPixmap & pixmap)
{
    d->time.start();
    d->imageChanged = true;
    d->pixmap = pixmap;
    repaint();
}

void QCameraImageBase::setViewSize(const int & width, const int & height)
{
    d->viewSize = QSize(width, height);
}

void QCameraImageBase::setAutoResize(bool resize)
{
    d->doAutoResize = resize;
}

void QCameraImageBase::saveImage(QString fileName, QString format)
{
    d->pixmap.save(fileName, format.toLatin1());
}

void QCameraImageBase::setVideoActive(bool enabled)
{
    d->isVideoActive = enabled;
}

void QCameraImageBase::setAspectRatio(Qt::AspectRatioMode ratio)
{
    d->aspectRatio = ratio;
}
