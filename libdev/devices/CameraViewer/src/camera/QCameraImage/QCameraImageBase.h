#ifndef CAMERAIMAGEBASE_H_
#define CAMERAIMAGEBASE_H_

#include <windows.h>

class QCameraImageBasePrivate;
class QPaintEvent;
class QResizeEvent;

#include <QtWidgets/QWidget>
#include <QImage>
#include <QPixmap>
#include <QtCore/QScopedPointer>


#if defined(_MSC_VER) /* MSVC Compiler */
#pragma warning( disable : 4100 )
#endif



class QCameraImageBase : public QWidget
{
    Q_OBJECT
public:
    QCameraImageBase(QWidget* parent = 0, Qt::WindowFlags flags = 0);
    virtual ~QCameraImageBase();

protected:
    const QScopedPointer<QCameraImageBasePrivate> d_ptr;

private:
    // requires d_ptr
    Q_DECLARE_PRIVATE(QCameraImageBase)

protected:
    void paintEvent(QPaintEvent * event);
    void resizeEvent(QResizeEvent * event);

    virtual void paintEventExtras(QPainter & painter);

public:
    HWND windowHandle();

    void setImage(const QImage image);
    void setPixmap(const QPixmap pixmap);
    void setImageSize(const int & width, const int & height);
    void setViewSize(const int & width, const int & height);
    void setAutoResize(bool resize);
    void saveImage(QString fileName, QString format = "PNG");
    void setVideoActive(bool enabled);

    void setAspectRatio(Qt::AspectRatioMode ratio);

};

#endif
