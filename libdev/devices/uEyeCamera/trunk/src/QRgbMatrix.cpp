#include "QRgbMatrix.h"

#include <QTime>
#include <QDebug>

#include <vector>
using std::vector;

#include <cmath>


class QRgbMatrixPrivate
{
public:
    QRgbMatrixPrivate()        
        : isMonochrom(false)
        , bitsPerPixel(8)
        , bitsPerPixelCorrection(1.0)
        , isValidData(false)
        {}

    bool isMonochrom;

    int bitsPerPixel;
    double bitsPerPixelCorrection;

    vector<int> red;
    vector<int> green;
    vector<int> blue;

    bool isValidData;
};


QRgbMatrix::QRgbMatrix()
    : d(new QRgbMatrixPrivate)
{
    m_size_x = m_size_y = 0;
}

QRgbMatrix::QRgbMatrix(int x, int y)
    : d(new QRgbMatrixPrivate)
{
    resize(x, y);
}

QRgbMatrix::~QRgbMatrix()
{
    delete d;
}

void QRgbMatrix::resize(size_t x, size_t y)
{
    d->red.resize(x*y);
    d->green.resize(x*y);
    d->blue.resize(x*y);

    m_size_x = x;
    m_size_y = y;

}

void QRgbMatrix::setDataValid(bool valid)
{
    d->isValidData = valid;
}

bool QRgbMatrix::isDataValid()
{
    return d->isValidData;
}


bool QRgbMatrix::isMonochrome()
{
    return d->isMonochrom;
}

void QRgbMatrix::setMonochrome(bool enable)
{
    d->isMonochrom = enable;
}

bool QRgbMatrix::isColored()
{
    return !d->isMonochrom;
}

void QRgbMatrix::setColored(bool enable)
{
    d->isMonochrom = !enable;
}

QSize QRgbMatrix::size()
{
    return QSize(m_size_x, m_size_y);
}

void QRgbMatrix::setSize(QSize size)
{
    resize(size.width(), size.height());
}

void QRgbMatrix::setSize(int xsize, int ysize)
{
    resize(xsize, ysize);
}

int QRgbMatrix::bitsPerPixel()
{
    return d->bitsPerPixel;
}

void QRgbMatrix::setBitsPerPixel(int bits)
{
    if ((bits >=8) && (bits <= 32)) {
        d->bitsPerPixel = bits;
        double maxColorValue = pow(2.0,d->bitsPerPixel) - 1;
        d->bitsPerPixelCorrection = 255.0 / maxColorValue;
    }
}

QColor QRgbMatrix::operator()(int x, int y) const
{    
    int pos = ArrPos(x,y);
    int r = int(d->red[pos]   * d->bitsPerPixelCorrection);
    int g = int(d->green[pos] * d->bitsPerPixelCorrection);
    int b = int(d->blue[pos]  * d->bitsPerPixelCorrection);

    if (d->isMonochrom) {
        return QColor(g,g,g);
    } else {
        return QColor(r,g,b);
    }
}

int QRgbMatrix::red(int x, int y) const
{
    int pos = ArrPos(x,y);
    return int(d->red[pos]   * d->bitsPerPixelCorrection);
}

int QRgbMatrix::blue(int x, int y) const
{
    int pos = ArrPos(x,y);
    return int(d->blue[pos]  * d->bitsPerPixelCorrection);
}

int QRgbMatrix::green(int x, int y) const
{
    int pos = ArrPos(x,y);
    return int(d->green[pos] * d->bitsPerPixelCorrection);
}

int QRgbMatrix::gray(int x, int y) const
{
    int pos = ArrPos(x,y);

    if (d->isMonochrom) {
        return int(d->green[pos] * d->bitsPerPixelCorrection);

    } else {
        int r = int(d->red[pos]   * d->bitsPerPixelCorrection);
        int g = int(d->green[pos] * d->bitsPerPixelCorrection);
        int b = int(d->blue[pos]  * d->bitsPerPixelCorrection);

        return qGray(r,g,b);
    }
}

void QRgbMatrix::setPixel(int x, int y, int gray)
{
    int pos = ArrPos(x,y);
    d->green[pos] = gray;
}

void QRgbMatrix::setPixel(int x, int y, int r, int g, int b)
{
    int pos = ArrPos(x,y);
    d->red[pos] = r;
    d->green[pos] = g;
    d->blue[pos] = b;
}

void QRgbMatrix::setPixel(int x, int y, QColor color)
{
    int pos = ArrPos(x,y);
    d->red[pos] = color.red();
    d->green[pos] = color.green();
    d->blue[pos] = color.blue();
}

QImage QRgbMatrix::toImage()
{
//    QTime time;
//    time.start();
    QImage image = QImage(m_size_x, m_size_y, QImage::Format_RGB32);

    int r, g, b;
    r = g = b = 0;
    double correction = d->bitsPerPixelCorrection;
    for ( int y = 0; y < m_size_y; y++ )
    {
        QRgb *line = (QRgb *)image.scanLine(y) ;
        for ( int x = 0; x < m_size_x; x++ )
        {
            int pos = ArrPos(x,y);

            r = d->red[pos];
            g = d->green[pos];
            b = d->blue[pos];

            r = int(r * correction);
            g = int(g * correction);
            b = int(b * correction);

            if (d->isMonochrom) {
                *line++ = qRgb(g, g, g);
            } else {
                *line++ = qRgb(r, g, b);
            }
        }
    }
//    qDebug() << "Time (toImage): " << time.elapsed();
    return image;
}

QPixmap QRgbMatrix::toPixmap()
{
    return QPixmap::fromImage(toImage());
}
