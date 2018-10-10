#include "cameraimplementation.h"


CameraImplementation::CameraImplementation()
{

}

/** update image data with next image */
void CameraImplementation::updateImageData(const QImage& nextImage)
{
    QMutexLocker locker(&mutex);
    this->lastImage = nextImage;
    LOG_INFO() << "update Image (finished)";
}

/** convert raw memory data to QImage format */
QImage CameraImplementation::toQImage(const QSize imageSize)
{
    // get current image size
    const size_t sizeX = static_cast<size_t>(imageSize.width());
    const size_t sizeY = static_cast<size_t>(imageSize.height());

    auto maxValue = *max_element(std::begin(grayVector), std::end(grayVector));
    auto minValue = *min_element(std::begin(grayVector), std::end(grayVector));
    float maxDivider = (maxValue-minValue);

//    LOG_INFO() << "max" << maxValue;
//    LOG_INFO() << "min" << minValue;
//    LOG_INFO() << "d" << maxDivider;

    // convert gray values into QImage data
    QImage image = QImage(static_cast<int>(sizeX), static_cast<int>(sizeY), QImage::Format_ARGB32_Premultiplied);
    for ( size_t y = 0; y < sizeY; ++y )
    {
        size_t yoffset = y * sizeX;
        QRgb *line = reinterpret_cast<QRgb *>(image.scanLine(int(y))) ;
        for ( size_t x = 0; x < sizeX  ; ++x )
        {
            size_t pos = x + yoffset;
            int color = (grayVector[static_cast<size_t>(pos)] - minValue) / maxDivider * 255;
            *line++ = qRgb(color, color, color);
        }
    }

    // convert to destiny format
    // image = std::move(image).convertToFormat(dstFormat);

    LOG_INFO() << "to QImage from data (finished)";
    return image;
}
