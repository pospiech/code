#ifndef COMPLEXFUNCTIONFACTORY_H
#define COMPLEXFUNCTIONFACTORY_H

#include <vector>
#include <complex>
#include <algorithm>
#include <iterator> // std::begin



using std::vector;
using std::complex;

#include "functionfactory.h"
#include "fftallocator.h"
using namespace fftwcpp;

#include <QSharedPointer>

class ComplexFunctionFactory
{
public:
    ComplexFunctionFactory()
    {
        // default implementation
        phaseFunction.reset(new NullFunction());
        amplitudeFunction.reset(new OneFunction());
    }

    virtual ~ComplexFunctionFactory()
    {
        // necessary for QSharedPointer
    }


    void setAmplitudeFunction(QSharedPointer<FunctionFactory> function)
    {
        amplitudeFunction.swap(function);
    }

    void setAmplitudeFunction(FunctionFactory * function)
    {
        amplitudeFunction.reset(function);
    }

    void setPhaseFunction(QSharedPointer<FunctionFactory> function)
    {
        phaseFunction.swap(function);
    }

    void setPhaseFunction(FunctionFactory * function)
    {
        phaseFunction.reset(function);
    }

    virtual vector<double> amplitudeData(size_t N)
    {
        return amplitudeFunction->data(N);
    }

    virtual vector<double> phaseData(size_t N)
    {
        return phaseFunction->data(N);
    }

    virtual vector<double> amplitudeData(size_t Nx, size_t Ny)
    {
        return amplitudeFunction->data(Nx, Ny);
    }

    virtual vector<double> phaseData(size_t Nx, size_t Ny)
    {
        return phaseFunction->data(Nx, Ny);
    }

    vector<complex<double>,fftalloc<complex<double> > > complexData(size_t N)
    {
        vector<double> vAmplitude = this->amplitudeData(N);
        vector<double> vPhase = this->phaseData(N);
        return createComplexFromAmplitudePhase(vAmplitude, vPhase, N );
    }

    vector<complex<double>,fftalloc<complex<double> > > complexData(size_t Nx, size_t Ny)
    {

        vector<double> vAmplitude = this->amplitudeData(Nx, Ny);
        vector<double> vPhase = this->phaseData(Nx, Ny);
        return createComplexFromAmplitudePhase(vAmplitude, vPhase, Nx*Ny );
    }


private:

    vector<complex<double>,fftalloc<complex<double> > > createComplexFromAmplitudePhase(vector<double> & vAmplitude,  vector<double> & vPhase, size_t N )
    {
        vector<complex<double>,fftalloc<complex<double> > > vComplex(N);
        // amplitude must not be negative (polar expects >=0)
        double minValue = *std::min_element( std::begin(vAmplitude), std::end(vAmplitude) );

        if (minValue <  0){
            // add minValue to vAmplitude
            for(double& d : vAmplitude)
                d += -minValue;
        }

        // combine two vectors using polar
        std::transform(begin(vAmplitude), end(vAmplitude),
                       begin(vPhase),   begin(vComplex),
                       [](double a, double p) { return std::polar(a, p); });
        return vComplex;

    }

    QSharedPointer<FunctionFactory> phaseFunction;
    QSharedPointer<FunctionFactory> amplitudeFunction;
};

#endif // COMPLEXFUNCTIONFACTORY_H
