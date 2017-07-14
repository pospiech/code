#include "calculationmanager.h"

#include <algorithm>
#include <functional>
#include <iterator> // std::begin

#include "fftwcpp.h"
#include "fftshift.h"
using namespace fftwcpp;

#include <QMutexLocker>
#include <QDebug>

class CalculationManagerPrivate{
//    Q_DECLARE_Public(CalculationManager)
//    Q_DISABLE_COPY(CalculationManagerPrivate)
public:
    CalculationManagerPrivate(CalculationManager * parent)
        : q_ptr(parent)
        , speed(0)
        , is2D(false)
        , dimensions(0)
    {        
    }
    ~CalculationManagerPrivate()
    {
    } 

public:
    // create q-pointer for parent class, USE Q_Q for access
    CalculationManager * q_ptr;
    Q_DECLARE_PUBLIC(CalculationManager)

    // create FFT class implementation
    // each implementation has its own plan map.
    FFT<double> fft;
    size_t N;
    QSize size;
    bool doShiftBeforeFFT;
    size_t iterations;
    double speed;
    bool is2D;
    size_t dimensions; //! none = 0, 1D = 1, 2D = 2, etc.

    // data vectors
    vector<complex<double>,fftalloc<complex<double> > > dataOriginal; //! Orignal Data
    vector<complex<double>,fftalloc<complex<double> > > dataFwdFourier;  //! Fourier Transformed Data
    vector<complex<double>,fftalloc<complex<double> > > dataInvFourier; //! Invers Fourier Calculated Data

    //! set size of vector elements (total number of elements)
    void setDataSize(size_t N)
    {
        this->N = N;
        dataFwdFourier.resize(N);
        dataInvFourier.resize(N);
    }

    //! set size of 1D vector or 2D Matrix
    void setMatrixSize(QSize size)
    {
        this->size = size;
        if (size.height() != 0)
            is2D = true;
        else
            is2D = false;
    }

};


CalculationManager::CalculationManager()
    : QObject()
    , d_ptr(new CalculationManagerPrivate(this))
{
    setShiftBeforeFFT(true);
    setIterations(1);
}

CalculationManager::~CalculationManager()
{

}


//! set shift variable for fftshift operation before the forward fft
void CalculationManager::setShiftBeforeFFT(bool doShift)
{
    Q_D(CalculationManager);
    d->doShiftBeforeFFT = doShift;
}


//! value of shift variable for fftshift operation before the forward fft
bool CalculationManager::shiftBeforeFFT() const
{
    Q_D(const CalculationManager);
    return d->doShiftBeforeFFT;
}


//! set number of iterations for fft loop
void CalculationManager::setIterations(size_t N)
{
    Q_D(CalculationManager);
    d->iterations = N;
}

void CalculationManager::setDimensions(size_t N)
{
    Q_D(CalculationManager);
    const size_t  oldN = d->dimensions;
    d->dimensions = N;
    if (N != oldN)
        emit dimensionsChanged(N);
}

size_t CalculationManager::dimensions() const
{
    Q_D(const CalculationManager);
    return d->dimensions;
}

/*! provide size of 1D vector or 2D matrix */
QSize CalculationManager::size() const
{
    Q_D(const CalculationManager);
    return d->size;
}


//! set original data
//! \param data complex double vector
void CalculationManager::setData(const vector<complex<double>,fftalloc<complex<double> > > & data, QSize size = QSize(0,0))
{
    Q_D(CalculationManager);
    QMutexLocker locker(&mutex);
    d->dataOriginal = data;
    d->setDataSize(data.size());
    if (size.isEmpty())
        size.setWidth(data.size());
    d->setMatrixSize(size);
}

//! get original data
//! \param action fftshift action
//! \return complex double vector
//!
//! If fftshift is applied, the a shifted copy of the original data is returned.
vector<complex<double>,fftalloc<complex<double> > > CalculationManager::data(fftAction action)
{
    Q_D(CalculationManager);
    QMutexLocker locker(&mutex);

    if (action==fftAction::shift) {
        // create copy
        vector<complex<double>,fftalloc<complex<double> > > copy (d->dataOriginal);
        // shift copy
        if (d->is2D)
            fftshift(copy, d->size.width(), d->size.height());
        else
            fftshift(copy);
        return copy;
    }
    else {
        return d->dataOriginal;
    }
}

//! get forward fourier data
vector<complex<double>,fftalloc<complex<double> > > CalculationManager::dataFwdFourier(fftAction action)
{
    Q_D(CalculationManager);
    QMutexLocker locker(&mutex);

    if (action==fftAction::shift) {
        vector<complex<double>,fftalloc<complex<double> > > copy = d->dataFwdFourier;
        // shift copy
        if (d->is2D)
            fftshift(copy, d->size.width(), d->size.height());
        else
            fftshift(copy);
        return copy;
    }
    else {
        return d->dataFwdFourier;
    }
}

//! get invers fourier data
vector<complex<double>,fftalloc<complex<double> > > CalculationManager::dataInvFourier(fftAction action)
{
    Q_D(CalculationManager);
    QMutexLocker locker(&mutex);

    if (action==fftAction::shift) {
        vector<complex<double>,fftalloc<complex<double> > > copy = d->dataInvFourier;
        // shift copy
        if (d->is2D)
            fftshift(copy, d->size.width(), d->size.height());
        else
            fftshift(copy);
        return copy;
    }
    else {
        return d->dataInvFourier;
    }
}

//! get (complex) substrattion of original and invers fourier data
//! \note mathematically both must be the same.
vector<complex<double>,fftalloc<complex<double> > > CalculationManager::dataCompare(fftAction action)
{
    Q_D(CalculationManager);
    QMutexLocker locker(&mutex);

    vector<complex<double>,fftalloc<complex<double> > > dataCompare(d->N);
    // substract vectors
    std::transform(begin(d->dataOriginal),
                   end(d->dataOriginal),
                   begin(d->dataInvFourier),
                   begin(dataCompare),std::minus< complex<double> >());

//    for(std::vector<complex<double> >::size_type i = 0; i != dataCompare.size(); i++) {
//        if (abs(dataCompare[i]) < 1e-15) {
//            dataCompare[i] = 0;
//        }
//    }

    if (action==fftAction::shift)
    {
        // shift copy
        if (d->is2D)
            fftshift(dataCompare, d->size.width(), d->size.height());
        else
            fftshift(dataCompare);
    }
    return dataCompare;
}

//! execute series of forward and invers fft operations
//! \note This is a slot for the QThread signal started()
void CalculationManager::process()
{
    Q_D(CalculationManager);
    QMutexLocker locker(&mutex);

    cout << "calc FFT: process(), iterations: " << d->iterations<< endl;

    const clock_t clockStart=clock();
    double percent = 0;

    if (!d->is2D)
    {
        if (d->doShiftBeforeFFT)
            ifftshift(d->dataOriginal);

        // forward FFT;
        d->fft.fwd(d->dataFwdFourier, d->dataOriginal);

        for (size_t i = 0; i < d->iterations; ++i)
        {
            if (i>0) {
                // forward FFT; input is last invers FFT
                d->fft.fwd(d->dataFwdFourier, d->dataInvFourier);
            }

            // invers FFT;
            d->fft.inv(d->dataInvFourier, d->dataFwdFourier);

            percent = double(i+1)/(d->iterations)*100;
            emit iteration(int(percent));
        }
    }
    else { // 2D
        if (d->doShiftBeforeFFT)
            ifftshift(d->dataOriginal, d->size.width(), d->size.height());

        // forward FFT;
        d->fft.fwd2(d->dataFwdFourier, d->dataOriginal, d->size.width(), d->size.height());

        for (size_t i = 0; i < d->iterations; ++i)
        {
            if (i>0) {
                // forward FFT; input is last invers FFT
                d->fft.fwd2(d->dataFwdFourier, d->dataInvFourier, d->size.width(), d->size.height());
            }

            // invers FFT;
            d->fft.inv2(d->dataInvFourier, d->dataFwdFourier, d->size.width(), d->size.height());

            percent = double(i+1)/(d->iterations)*100;
            emit iteration(int(percent));
        }

    }
        const clock_t clockEnd=clock();
    // time elapsed for FFT Calculation in seconds
    setSpeed( (double)(clockEnd  - clockStart) / CLOCKS_PER_SEC );

    emit finished();
}

//! set time elapsed for FFT Calculation in seconds
void CalculationManager::setSpeed(double timeSeconds)
{
    Q_D(CalculationManager);
    d->speed = timeSeconds;
}

/*! time elapsed for FFT Calculation in seconds */
double CalculationManager::speed() const
{
    Q_D(const CalculationManager);
    return d->speed;
}

