#include "qplotfunction.h"

#include <QDebug>

using std::vector;

class DataMinMax {
public:
    DataMinMax() : min(0), max(0) {}
    DataMinMax(double minimum, double maximum) : min(minimum), max(maximum) {}
    double min;
    double max;
};


class QPlotFunctionPrivate
{
public:

    QPlotFunctionPrivate()
        : dataPointsX(100)
        , dataPointsY(100)
        , rangeX(-10,10)
        , rangeY(-10,10)
    {
        resolutionX = (rangeX.max - rangeX.min) / dataPointsX;
        resolutionY = (rangeY.max - rangeY.min) / dataPointsY;
    }
    ~QPlotFunctionPrivate()
    {
    }

    size_t dataPointsX;
    size_t dataPointsY;
    DataMinMax rangeX;
    DataMinMax rangeY;
    double resolutionX;
    double resolutionY;
    QString name;
};

QPlotFunction::QPlotFunction(QString name)
    : d(new QPlotFunctionPrivate())
{
    setName(name);
}

QPlotFunction::~QPlotFunction()
{
    delete d; d = NULL;
}

void QPlotFunction::calculate(vector<double> * xAxis, vector<double> * yData)
{
    try {
        xAxis->resize(d->dataPointsX);
        yData->resize(d->dataPointsX);
    } catch (std::bad_alloc const&) {
        qCritical() << "Memory allocation fail!" << endl;
        xAxis->clear();
        yData->clear();
        return;
    }

    for (size_t i=0; i < d->dataPointsX; ++i)
    {
        double x = d->rangeX.min + double(i)*d->resolutionX;
        double y = operator()(x);
        (*xAxis)[i] = x;
        (*yData)[i] = y;
    }
}

void QPlotFunction::calculate2D(vector<double> * xAxis, vector<double> * yAxis, vector<double> * data)
{
    try {
        xAxis->resize(d->dataPointsX);
        yAxis->resize(d->dataPointsY);
        data->resize(d->dataPointsX * d->dataPointsY);
    } catch (std::bad_alloc const&) {
        qCritical() << "Memory allocation fail!" << endl;
        xAxis->clear();
        yAxis->clear();
        data->clear();
        return;
    }

    // calculate x-Axis
    for (size_t ix=0; ix < d->dataPointsX; ++ix)
    {
        double x = d->rangeX.min + double(ix)*d->resolutionX;
        (*xAxis)[ix] = x;
    }

    // calculate y-Axis
    for (size_t iy=0; iy < d->dataPointsY; ++iy)
    {
        double y = d->rangeY.min + double(iy)*d->resolutionY;
        (*yAxis)[iy] = y;
    }


    // calculate data for every (x,y) point
    for (size_t iy=0; iy < d->dataPointsY; ++iy)
    {
        double y = (*yAxis)[iy];
        for (size_t ix=0; ix < d->dataPointsX; ++ix)
        {
            double x = (*xAxis)[ix];
            double value = operator()(x,y);
            //(*data)[iy + d->dataPointsY * ix] = value;
            //row-major format
            (*data)[ix + d->dataPointsX * iy] = value;
        }
    }
}

void QPlotFunction::setName(QString name)
{
    d->name = name;
}

QString QPlotFunction::name()
{
    return d->name;
}


void QPlotFunction::setRange(double minimum, double maximum)
{
    setRangeX(minimum, maximum);
}

void QPlotFunction::setRangeX(double minimum, double maximum)
{
    setMinimumX(minimum);
    setMaximumX(maximum);
}

void QPlotFunction::setRangeY(double minimum, double maximum)
{
    setMinimumY(minimum);
    setMaximumY(maximum);
}

void QPlotFunction::setMinimum(double minimum)
{
    setMinimumX(minimum);
}

void QPlotFunction::setMinimumX(double minimum)
{
    d->rangeX.min = minimum;
    if (d->rangeX.min > d->rangeX.max) {
        d->rangeX.max = d->rangeX.min;
    }
    calculateResolution();
}

void QPlotFunction::setMinimumY(double minimum)
{
    d->rangeY.min = minimum;
    if (d->rangeY.min > d->rangeY.max) {
        d->rangeY.max = d->rangeY.min;
    }
    calculateResolution();
}

double QPlotFunction::minimum()
{
    return minimumX();
}

double QPlotFunction::minimumX()
{
    return d->rangeX.min;
}

double QPlotFunction::minimumY()
{
    return d->rangeY.min;
}

void QPlotFunction::setMaximum(double maximum)
{
    setMaximumX(maximum);
}

void QPlotFunction::setMaximumX(double maximum)
{
    d->rangeX.max = maximum;
    if (d->rangeX.min > d->rangeX.max) {
        d->rangeX.min = d->rangeX.max;
    }
    calculateResolution();
}

void QPlotFunction::setMaximumY(double maximum)
{
    d->rangeY.max = maximum;
    if (d->rangeY.min > d->rangeY.max) {
        d->rangeY.min = d->rangeY.max;
    }
    calculateResolution();
}

double QPlotFunction::maximum()
{
    return maximumX();
}

double QPlotFunction::maximumX()
{
    return d->rangeX.max;
}

double QPlotFunction::maximumY()
{
    return d->rangeY.max;
}

void QPlotFunction::setDataPoints(size_t points)
{
    setDataPointsX(points);
}

void QPlotFunction::setDataPointsX(size_t points)
{
    d->dataPointsX = points;
    calculateResolution();
}

void QPlotFunction::setDataPointsY(size_t points)
{
    d->dataPointsY = points;
    calculateResolution();
}

void QPlotFunction::calculateResolution()
{
    d->resolutionX = (d->rangeX.max - d->rangeX.min) / d->dataPointsX;
    d->resolutionY = (d->rangeY.max - d->rangeY.min) / d->dataPointsY;
}

int QPlotFunction::dataPoints()
{
    return dataPointsX();
}

int QPlotFunction::dataPointsX()
{
    return int(d->dataPointsX);
}

int QPlotFunction::dataPointsY()
{
    return int(d->dataPointsY);
}

void QPlotFunction::setResolution(double resolution)
{
    setResolutionX(resolution);
}

void QPlotFunction::setResolutionX(double resolution)
{
    if (resolution > 0) {
        d->resolutionX = resolution;
        d->dataPointsX = int((d->rangeX.max - d->rangeX.min) / d->resolutionX);
    } else {
        qWarning() << "Can not use dx < 0";
    }
}

void QPlotFunction::setResolutionY(double resolution)
{
    if (resolution > 0) {
        d->resolutionY = resolution;
        d->dataPointsY = int((d->rangeY.max - d->rangeY.min) / d->resolutionY);
    } else {
        qWarning() << "Can not use dy < 0";
    }
}

double QPlotFunction::resolution()
{
    return resolutionX();
}

double QPlotFunction::resolutionX()
{
    return d->resolutionX;
}

double QPlotFunction::resolutionY()
{
    return d->resolutionY;
}

