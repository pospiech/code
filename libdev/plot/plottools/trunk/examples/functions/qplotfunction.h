#ifndef QPLOTFUNCTION_H
#define QPLOTFUNCTION_H


#include <QString>

#include <vector>

class QPlotFunctionPrivate;

//! Abstract base class for mathematical functions

class QPlotFunction
{
public:
    QPlotFunction(QString name = QString(""));
    ~QPlotFunction();

    virtual double operator()(double x) = 0;
    virtual double operator()(double x, double y) = 0;

    void calculate(std::vector<double> * xAxis, std::vector<double> * yData);
    void calculate2D(std::vector<double> * xAxis, std::vector<double> * yAxis, std::vector<double> * data);

    void setName(QString name);
    QString name();

    void setRange(double minimum, double maximum);
    void setRangeX(double minimum, double maximum);
    void setRangeY(double minimum, double maximum);

    void setMinimum(double minimum);
    double minimum();
    void setMinimumX(double minimum);
    double minimumX();
    void setMinimumY(double minimum);
    double minimumY();

    void setMaximum(double maximum);
    double maximum();
    void setMaximumX(double maximum);
    double maximumX();
    void setMaximumY(double maximum);
    double maximumY();

    void setDataPoints(size_t points);
    int dataPoints();
    void setDataPointsX(size_t points);
    int dataPointsX();
    void setDataPointsY(size_t points);
    int dataPointsY();

    void setResolution(double resolution);
    double resolution();
    void setResolutionX(double resolution);
    double resolutionX();
    void setResolutionY(double resolution);
    double resolutionY();

private:
    void calculateResolution();

private:
    QPlotFunctionPrivate* d;

};

#endif // QPLOTFUNCTION_H
