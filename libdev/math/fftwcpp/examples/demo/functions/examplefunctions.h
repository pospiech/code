#ifndef EXAMPLEFUNCTIONS_H
#define EXAMPLEFUNCTIONS_H


#include "functionfactory.h"

#include <cmath>
static const double pi = 3.14159265358979323846;

using namespace std;
#include <vector>



class RectFunction : public FunctionFactory
{
public:
    RectFunction() : FunctionFactory()
    {
        setParameter("waist", 10);
        setMin("waist", 1);
    }

    double max(const QString name, size_t N)
    {
        Q_UNUSED(name);
        return N;
    }

    void parameterChanged(const QString name)
    {
        Q_UNUSED(name);
        waist = parameter("waist");
        halfwaist = waist/2;
    }

    double function(size_t xi, size_t N)
    {
        double x = double(xi) - N/2.0;
        if (abs(x) < halfwaist) {
            return 1.0;
        } else {
            return 0.0;
        }
    }

    double function(size_t xi,size_t yi, size_t Nx, size_t Ny)
    {
        const double x = double(xi) - Nx/2.0;
        const double y = double(yi) - Ny/2.0;
        if ( (abs(x) < halfwaist) && (abs(y) < halfwaist) ){
            return 1.0;
        } else {
            return 0.0;
        }
    }


    QString description() {
        return QString("Rect Function (0..1)");
    }
private:
    double waist;
    double halfwaist;
};

class  GaussFunction: public FunctionFactory
{
public:
    GaussFunction() : FunctionFactory()
    {
        setParameter("waist", 10);
        setMin("waist", 1);
    }

    double max(const QString name, size_t N)
    {
        Q_UNUSED(name);
        return N;
    }

    void parameterChanged(const QString name)
    {
        Q_UNUSED(name);
        waist = parameter("waist");
        halfwaist = waist/2;
    }

    double function(size_t xi, size_t N)
    {
        const double x = double(xi) - N/2.0;
        double value = exp(-( (x*x)/(waist*waist) ) );
        return value;
    }

    double function(size_t xi,size_t yi, size_t Nx, size_t Ny)
    {
        const double x = double(xi) - Nx/2.0;
        const double y = double(yi) - Ny/2.0;
        const double r2 = pow(x,2) + pow(y,2);
        double value = exp(-( (r2)/(waist*waist) ) );
        return value;
    }

    QString description() {
        return QString("Gauss Function (0..1, width)");
    }
private:
    double waist;
    double halfwaist;

};

/*!
 * Sinus Function
 */
class SinusFunction : public FunctionFactory
{
public:
    SinusFunction() : FunctionFactory()
    {
        setParameter("width", 10);
        setMin("width", 1);
    }

    double max(const QString name, size_t N)
    {
        Q_UNUSED(name);
        return N;
    }

    void parameterChanged(const QString name)
    {
        Q_UNUSED(name);
        width = parameter("width");
    }

    double function(size_t xi, size_t N)
    {
        const double x = double(xi) - N/2.0;
        // values from -pi .. 0
        double value = (pi * (sin(pi * x/width)+1)/2) - pi;
        return value;
    }

    double function(size_t xi, size_t yi, size_t Nx, size_t Ny)
    {
        const double valueX = function(xi, Nx);
        const double valueY = function(yi, Ny);
        // values from -pi .. 0
        double value = sqrt(valueX * valueY);
        return value;
    }


    QString description() {
        return QString("Sinus Function (-pi..0, width)");
    }
private:
    double width;
};

/*!
 * Rect Grating Function
 */
class RectGratingFunction : public FunctionFactory
{
public:
    RectGratingFunction() : FunctionFactory()
    {
        setParameter("width", 10);
        setMin("waist", 1);
    }

    double max(const QString name, size_t N)
    {
        Q_UNUSED(name);
        return N;
    }

    void parameterChanged(const QString name)
    {
        Q_UNUSED(name);
        width = parameter("width");
    }

    double function(size_t xi, size_t N)
    {
        double x = double(xi) - N/2.0;
        // sin with period = 2*width from -1 .. +1
        double value = (2*(sin(pi * x/width)+1)/2) - 1;
        if (value > 0) {
            value = 0;
        }
        else if (value < 0) {
            value = -pi;
        }

        return value;
    }

    double function(size_t xi, size_t yi, size_t Nx, size_t Ny)
    {
        const double valueX = function(xi, Nx);
        const double valueY = function(yi, Ny);
        // -pi, -pi/2, 0 as values
        double value = (valueX + valueY)/2.0;
        return value;
    }


    QString description() {
        return QString("Rect Period Function (-pi..0, width)");
    }

private:
    double width;

};

/*!
 * Tilt Function
 */
class TiltFunction : public FunctionFactory
{
public:
    TiltFunction() : FunctionFactory()
    {
        setParameter("tilt", 10);
        setParameter("angle", 45);
        setMin("angle", 0);
        setMax("angle", 360);
    }

    double min(const QString name, size_t N)
    {
        if (name=="tilt")
            return -N;
        else
            return FunctionFactory::min(name, N);
    }

    double max(const QString name, size_t N)
    {
        if (name=="tilt")
            return N;
        else
            return FunctionFactory::min(name, N);
    }

    void parameterChanged(const QString name)
    {
        if (name == "tilt")
            tilt = parameter("tilt");
        else if (name == "angle")
            angle = parameter("angle")*pi/180.0;
    }

    double function(size_t xi, size_t N)
    {
        double value = double(xi)/N * tilt;
        return value;
    }

    double function(size_t xi, size_t yi, size_t Nx, size_t Ny)
    {
        const double yt = yi * sin(angle);
        const double xt = xi * cos(angle);
        double phase = xt/Nx * tilt + yt/Ny * tilt;
        phase = fmod(phase,2*pi) - pi;
        return phase;
    }

    QString description() {
        return QString("Tilt (Slope) (tilt)");
    }
private:
    double tilt;
    double angle;
};


/*!
 * Fresnel Lens Function
 */
class FresnelLensFunction : public FunctionFactory
{
public:
    FresnelLensFunction() : FunctionFactory()
    {
        setParameter("f", 10);
        setMin("f", 0);
        setMax("f", 10000);

        k = 2 * pi / lambda;
    }

    void parameterChanged(const QString name)
    {
        Q_UNUSED(name);
        f = parameter("f");
    }

    double function(size_t xi, size_t N)
    {
        const double x = (double(xi)-N/2.0) * dx;
        const double rsqr = pow(x,2);
        double value = -k * rsqr / (2*f);
        return value;
    }

    double function(size_t xi, size_t yi, size_t Nx, size_t Ny)
    {
        const double x = (double(xi)-Nx/2.0) * dx;
        const double y = (double(yi)-Ny/2.0) * dx;
        const double rsqr = pow(x,2)+pow(y,2);
        double value = -k * rsqr / (2*f);
        return value;
    }


    QString description() {
        return QString("Fresnel Lens Function (f)");
    }
private:
    // real world parameters, here fixed
    const double dx = 0.001; // 1mm
    const double lambda = 1e-6; // 1000 nm
    double k;
    double f;

};

#endif // EXAMPLEFUNCTIONS_H
