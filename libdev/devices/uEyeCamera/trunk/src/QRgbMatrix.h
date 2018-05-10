#ifndef QRGBMATRIX_H
#define QRGBMATRIX_H

#include "QCameraGlobal.h"

#include <QtCore/QSize>
#include <QtGui/QColor>
#include <QtGui/QPixmap>
#include <QtGui/QImage>


class QRgbMatrixPrivate;

class QCAMERA_EXPORT QRgbMatrix
{
public:
    QRgbMatrix();
    QRgbMatrix(int x, int y);
    virtual ~QRgbMatrix();

    void setDataValid(bool valid);
    bool isDataValid();

    bool isMonochrome();
    void setMonochrome(bool enable);

    bool isColored();
    void setColored(bool enable);

    QSize size();
    void setSize(QSize size);
    void setSize(int xsize, int ysize);

    int bitsPerPixel();
    void setBitsPerPixel(int bits);

    // operators
    QColor operator()(int x, int y) const;

    int red(int x, int y) const;
    int blue(int x, int y) const;
    int green(int x, int y) const;
    int gray(int x, int y) const;

    void setPixel(int x, int y, int gray);
    void setPixel(int x, int y, int r, int g, int b);
    void setPixel(int x, int y, QColor color);

    // convert
    QImage toImage();
    QPixmap toPixmap();

    // export

    // void saveAsText(QString filename);

private:
    QRgbMatrixPrivate * const d;

    int m_size_x;
    int m_size_y;

    void resize(size_t x, size_t y);

    int ArrPos(int x, int y) const
    {
            // y + height * x
            // return (y + m_size_y * x);
        return (x + m_size_x * y);
    }


};

#endif // QRGBMATRIX_H
