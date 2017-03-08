#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "QPlotFunction.h"
#include "constants.h"
#include <cmath>
#include <float.h>
//#include <QDebug>

namespace PlotFunction
{
    enum FunctionType { Gauss, Sinus, Sombrero, Rosenbrock, Hat, Ripple, Saddle };
}

class GaussianFunction : public QPlotFunction
{
public:
    GaussianFunction()
        : QPlotFunction("Gauss")
        , wx(1), wy(1), amplitude(1)
    {}

    double operator()(double x)
    {
       return operator()(x, 0);
    }

    double operator()(double x, double y)
    {
       return amplitude * exp(-( (x*x)/(wx*wx) + (y*y)/(wy*wy) ) );
    }

    void setWidth(double width) { wx = width; }
    void setWidthX(double width) { wx = width; }
    void setWidthY(double width) { wy = width; }
    void setAmplitude(double amplitude) { this->amplitude = amplitude; }

private:
    double wx;
    double wy;
    double amplitude;
};

class SinusFunction : public QPlotFunction
{
public:
    SinusFunction()
        : QPlotFunction("Sinus")
        , period(2), amplitude(1)
    { }

    double operator() (double x)
    {
        return amplitude * sin(2*constants::math::pi * x/period);
    }

    double operator() (double x, double y)
    {
        return amplitude * (sin(2*constants::math::pi * x/period) * sin(2*constants::math::pi * y/period));
    }

    void setPeriodLength(double period) { this->period = period; }
    void setAmplitude(double amplitude) { this->amplitude = amplitude; }

private:
    double period;
    double amplitude;
};

class SombreroFunction : public QPlotFunction
{
public:
    SombreroFunction()
        : QPlotFunction("Sombrero")
        , amplitude(1)
    { }

    double operator()(double x)
    {
        return operator()(x, 0);
    }

    double operator()(double x, double y)
    {
        double n = sqrt(x*x+y*y);        

        if (n < DBL_MIN)
            return amplitude;

        return amplitude * sin(n) / n;
    }

    void setAmplitude(double amplitude) { this->amplitude = amplitude; }

private:
    double amplitude;

};

class RosenbrockFunction : public QPlotFunction
{
public:
    RosenbrockFunction(): QPlotFunction("Rosenbrock"){ }

    double operator()(double x)
    {
        return operator()(x, 0);
    }

    double operator()(double x, double y)
    {
        return 0.7 * log10((1-x)*(1-x) + 10 * (y - x*x)*(y - x*x));
    }
};

class HatFunction : public QPlotFunction
{
public:
    HatFunction(): QPlotFunction("Hat"){ }

    double operator()(double x)
    {
        return operator()(x, 0);
    }

    double operator()(double x, double y)
    {
        return 1.0 / (x*x+y*y+0.5);
    }
};


class RippleFunction : public QPlotFunction
{
public:
    RippleFunction(): QPlotFunction("Ripple"){ }

    double operator()(double x)
    {
        return operator()(x, 0);
    }

    double operator()(double x, double y)
    {
        return (cos(sqrt(x*x+y*y) + cos(sqrt(((x+.913*2*constants::math::pi)*(x+.913*2*constants::math::pi))+y*y))
                    + cos(sqrt(((x-.913*2*constants::math::pi)*(x-.913*2*constants::math::pi))+(y*y))))*4);
    }
};

class SaddleFunction : public QPlotFunction
{
public:
    SaddleFunction(): QPlotFunction("Saddle"){ }

    double operator()(double x)
    {
        return operator()(x, 0);
    }

    double operator()(double x, double y)
    {
        return x*x - y*y;
    }
};


#endif // FUNCTIONS_H
