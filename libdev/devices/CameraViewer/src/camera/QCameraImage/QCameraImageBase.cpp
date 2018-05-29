#include "QCameraImageBase.h"

#include <QtCore/QSize>
#include <QtCore/QString>
#include <QtCore/QTime>
#include <QtGui/QPainter>
#include <QtCore/QDebug>

/** from
 * https://stackoverflow.com/questions/14048565/get-hwnd-on-windows-with-qt5-from-wid
 * http://lists.qt-project.org/pipermail/interest/2013-June/007650.html
 **/

#include <QWindow>
#include <QGuiApplication>
#include <qpa/qplatformnativeinterface.h>

static QWindow* windowForWidget(const QWidget* widget)
{
    QWindow* window = widget->windowHandle();
    if (window)
        return window;
    const QWidget* nativeParent = widget->nativeParentWidget();
    if (nativeParent)
        return nativeParent->windowHandle();
    return 0;
}

HWND getHWNDForWidget(const QWidget* widget)
{
    QWindow* window = ::windowForWidget(widget);
    if (window && window->handle())
    {
        HWND hwnd  = static_cast<HWND>(QGuiApplication::platformNativeInterface()->nativeResourceForWindow(QByteArrayLiteral("handle"),window));
        return hwnd;
    }
    return 0;
}


class QCameraImageBasePrivate{
public:
    QCameraImageBasePrivate()
        : imageChanged(true)
        , doAutoResize(true)
        , isVideoActive(false)
        , aspectRatio(Qt::IgnoreAspectRatio)
    {}

    bool imageChanged;
    bool doAutoResize;
    bool isVideoActive;

    QPixmap pixmap;
    QSize viewSize;
    QTime time;

    Qt::AspectRatioMode aspectRatio;
};

QCameraImageBase::QCameraImageBase(QWidget* parent, Qt::WindowFlags flags)
    : QWidget(parent, flags)
    , d_ptr(new QCameraImageBasePrivate)
{
    setAttribute(Qt::WA_NoSystemBackground);
    setAttribute(Qt::WA_NativeWindow);
    setAttribute(Qt::WA_DontCreateNativeAncestors);
}

QCameraImageBase::~QCameraImageBase()
{
}

void QCameraImageBase::resizeEvent ( QResizeEvent * event  )
{
    Q_UNUSED(event)

    update();
}

void QCameraImageBase::paintEvent(QPaintEvent * event)
{
    Q_UNUSED(event)

    Q_D(QCameraImageBase);
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
    Q_UNUSED(painter)
}

HWND QCameraImageBase::windowHandle()
{
    return getHWNDForWidget(this);
    // works only in Qt 4
    // return static_cast<HWND>(this->winId());
}

void QCameraImageBase::setImage(const QImage & image)
{
    Q_D(QCameraImageBase);

    d->time.start();
    d->imageChanged = true;
    d->pixmap = QPixmap::fromImage(image);
    update(); // calls paintEvent
}

void QCameraImageBase::setPixmap(const QPixmap & pixmap)
{
    Q_D(QCameraImageBase);

    d->time.start();
    d->imageChanged = true;
    d->pixmap = pixmap;
    repaint();
}

void QCameraImageBase::setViewSize(const int & width, const int & height)
{
    Q_D(QCameraImageBase);

    d->viewSize = QSize(width, height);
}

void QCameraImageBase::setAutoResize(bool resize)
{
    Q_D(QCameraImageBase);

    d->doAutoResize = resize;
}

void QCameraImageBase::saveImage(QString fileName, QString format)
{
    Q_D(QCameraImageBase);

    d->pixmap.save(fileName, format.toLatin1());
}

void QCameraImageBase::setVideoActive(bool enabled)
{
    Q_D(QCameraImageBase);

    d->isVideoActive = enabled;
}

void QCameraImageBase::setAspectRatio(Qt::AspectRatioMode ratio)
{
    Q_D(QCameraImageBase);

    d->aspectRatio = ratio;
}
