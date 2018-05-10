#ifndef CAMERAIMAGEBASE_H_
#define CAMERAIMAGEBASE_H_

#include <QtGlobal>

#if defined(Q_WS_WIN)

#ifdef CAMERAIMAGE_DLL

#if defined(CAMERAIMAGE_NODLL)     // Do not create a DLL library
    #define CAMERAIMAGE_EXPORT
#else
    #if defined(CAMERAIMAGE_MAKEDLL) // create a DLL library
        #define CAMERAIMAGE_EXPORT  __declspec(dllexport)
    #else                           // use a DLL library
        #define CAMERAIMAGE_EXPORT  __declspec(dllimport)
    #endif
#endif

#endif // CAMERAIMAGE_DLL

#endif // Q_WS_WIN

#ifndef CAMERAIMAGE_EXPORT
#define CAMERAIMAGE_EXPORT
#endif

#include <windows.h>

class QCameraImagePrivate;
class QPaintEvent;
class QResizeEvent;

#include <QtGui/QWidget>
#include <QtGui/QImage>
#include <QtGui/QPixmap>

#if defined(_MSC_VER) /* MSVC Compiler */
#pragma warning( disable : 4100 )
#endif

class CAMERAIMAGE_EXPORT QCameraImageBase : public QWidget
{
    Q_OBJECT
public:
    QCameraImageBase(QWidget* parent = 0, Qt::WFlags flags = 0);
    virtual ~QCameraImageBase();

private:
    QCameraImagePrivate * const d;

protected:
    void paintEvent(QPaintEvent * event);
    void resizeEvent(QResizeEvent * event);

    virtual void paintEventExtras(QPainter & painter);

public:
    HWND windowHandle();

    void setImage(const QImage & image);
    void setPixmap(const QPixmap & pixmap);
    void setImageSize(const int & width, const int & height);
    void setViewSize(const int & width, const int & height);
    void setAutoResize(bool resize);
    void saveImage(QString fileName, QString format = "PNG");
    void setVideoActive(bool enabled);

    void setAspectRatio(Qt::AspectRatioMode ratio);

};

#endif
