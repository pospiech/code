#ifndef CAMERASIMULATION_P_H
#define CAMERASIMULATION_P_H

#include "cameraimplementation.h"

#include <stdexcept>
#include <cmath>

class CameraSimulationPrivate: public CameraImplementation
{
public:
    CameraSimulationPrivate()
    {
        _exposure = 10;
        _gain = 0;
        sizeSensor = QSize(640,480);
        _roi = QRect(0,0,sizeSensor.width(),sizeSensor.height());
        _colorDepth = 8;

        createTestData();
    }

    float _exposure;
    float _gain;
    QRect _roi;
    int _colorDepth;

    std::vector<quint16> imageVector;
    std::vector<quint16> imageVectorROI;

    /** create test data of full sensor */
    void createTestData()
    {
        int pixels = sensorSize().width() * sensorSize().height();
        imageVector.resize(pixels);
        // make sure image contains all values up to max
        float correctionValue = 65535 / pixels;
        if (correctionValue  < 1)
            correctionValue  = 1.0;
        for (int i=0; i < pixels; ++i){
            imageVector[i] = std::fmod(i*correctionValue,65535);
        }
    }


    /** handle camera error messages */
    void errorHandler(QString message) const
    {
        LOG_WARNING() << "CameraViewer: Simlation: "<< message << endl;
    }

    size_t numberOfDevices()
    {
        return 1;
    }

    /** returns size of sensor in Pixel  */
    QSize sensorSize()
    {
        return sizeSensor;
    }

    /** returns current image size in Pixel  */
    QSize imageSize()
    {
        return QSize(_roi.width(), _roi.height());
    }

    /** calculates Bits to Bytes per Pixel */
    int bytesPerPixel()
    {
        return 4;
    }

    /** calculates total bytes of image */
    int imageSizeBytes()
    {
        return sizeSensor.width() * sizeSensor.height()*bytesPerPixel();
    }


    /** determine bits of a single pixel, depending on the format */
    int bitsPerPixel()
    {
        // Mono16
        return 16;
    }


    /** convert raw memory data to QImage format */
    void createData()
    {
        int pixels = imageSize().width() * imageSize().height();

        // fill roi vector with full size sensor data
        grayVector.resize(pixels);
        int bitDepth = bitsPerPixel();
        size_t histSize = 1 << bitDepth;
        histVector.clear();
        histVector.resize(histSize);

        size_t index = 0;
        for ( int y = _roi.y(); y < _roi.height(); ++y )
        {
            int yoffset = y * _roi.width();
            for ( int x = _roi.x(); x < _roi.width()  ; ++x )
            {
                int pos = x + yoffset;
                quint16 value = imageVector[pos];
                grayVector[index] = value;
                histVector[static_cast<size_t>(value)] += 1;
                index++;
            }
        }

        // todo: enable single line...
        QImage image = toQImage(imageSize());
        updateImageData(image);
    }


//    /** convert raw memory data to QImage format */
//    QImage toQImage()
//    {
//        // get current image size
//        const size_t sizeX = static_cast<size_t>(imageSize().width());
//        const size_t sizeY = static_cast<size_t>(imageSize().height());

//        auto maxValue = *max_element(std::begin(grayVector), std::end(grayVector));
//        auto minValue = *min_element(std::begin(grayVector), std::end(grayVector));
//        float maxDivider = (maxValue-minValue);

//        // convert gray values into QImage data
//        QImage image = QImage(static_cast<int>(sizeX), static_cast<int>(sizeY), QImage::Format_ARGB32_Premultiplied);
//        for ( size_t y = 0; y < sizeY; ++y )
//        {
//            size_t yoffset = y * sizeX;
//            QRgb *line = reinterpret_cast<QRgb *>(image.scanLine(int(y))) ;
//            for ( size_t x = 0; x < sizeX  ; ++x )
//            {
//                size_t pos = x + yoffset;
//                int color = static_cast<int>((grayVector[static_cast<size_t>(pos)] - minValue) / maxDivider * 255);
//                *line++ = qRgb(color, color, color);
//            }
//        }

//        LOG_INFO() << "to QImage from data (finished)";
//        return image;
//    }



    /** This function initializes the device and returns a device handle.
     */
    void openCamera()
    {
        LOG_INFO() << "open Camera (finished)";
    }



    /** This function will un-initialize the specified device,
     * closes its handle and releases allocated resources.
     */
    void closeCamera()
    {
        LOG_INFO() << "close Camera (finished)";
    }


    void getImage()
    {
                createData();
        LOG_INFO() << "Get Image (finished)";
    }

    /** Sets exposure time in microseconds. */
    void setExposure(double exposure_ms)
    {
        _exposure = exposure_ms;
    }

    /** Get exposure time in microseconds. */
    double exposure() const
    {
        return _exposure;
    }

    /** Sets gain factor */
    void setGain(float gain)
    {
        _gain = gain;
    }

    /** gain factor */
    float gain()
    {
        return _gain;
    }

    /** get parameters of camera, results are in QMap */
    QMap<QString, QString> getParamters()
    {
        QMap<QString, QString> mapParam;
        try{
            mapParam.insert("Sensor Name", "Simulated Sensor");
            mapParam.insert("Pixel size / µm", "5.5");
            mapParam.insert("Sensor Type", QString("Mono"));
            mapParam.insert("Serial", "0123456");
        }
        catch(const std::exception& e)
        {
            qDebug() << "Error:" << e.what();
        }
        return mapParam;
    }

    void configureBinning(bool enableBinning, Qt::Orientation orientation, int binningPixels = 2)
    {
        errorHandler("binning is not supported");
    }


    /** set ROI using offets and width and height */
    void setROI(QRect roi)
    {
        _roi = roi;
    }

    /** get ROI using offets and width and height */
    QRect roi() const
    {
        return _roi;
    }

    /** get color depth in bit */
    int colorDepth()
    {
        return _colorDepth;
    }
};





#endif // CAMERASIMULATION_P_H
