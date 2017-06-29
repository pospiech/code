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

    // data vectors
    vector<complex<double>,fftalloc<complex<double> > > dataOriginal; //! Orignal Data
    vector<complex<double>,fftalloc<complex<double> > > dataFwdFourier;  //! Fourier Transformed Data
    vector<complex<double>,fftalloc<complex<double> > > dataInvFourier; //! Invers Fourier Calculated Data

    void setSize(size_t N)
    {
        this->N = N;
        dataFwdFourier.resize(N);
        dataInvFourier.resize(N);
    }
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
    d_ptr->doShiftBeforeFFT = doShift;
}


//! value of shift variable for fftshift operation before the forward fft
bool CalculationManager::shiftBeforeFFT() const
{
    return d_ptr->doShiftBeforeFFT;
}


//! set number of iterations for fft loop
void CalculationManager::setIterations(size_t N)
{
    d_ptr->iterations = N;
}

//! set original data
//! \param data complex double vector
void CalculationManager::setData(const vector<complex<double>,fftalloc<complex<double> > > & data, QSize size = QSize(0,0))
{
    QMutexLocker locker(&mutex);
    d_ptr->dataOriginal = data;
    d_ptr->setSize(data.size());
    if (size.isEmpty())
        size.setWidth(data.size());
    d_ptr->setMatrixSize(size);
}

//! get original data
//! \param action fftshift action
//! \return complex double vector
//!
//! If fftshift is applied, the a shifted copy of the original data is returned.
vector<complex<double>,fftalloc<complex<double> > > CalculationManager::data(fftAction action)
{
    QMutexLocker locker(&mutex);

    if (action==fftAction::shift) {
        // create copy
        vector<complex<double>,fftalloc<complex<double> > > copy (d_ptr->dataOriginal);
        // shift copy
        if (d_ptr->is2D)
            fftshift(copy, d_ptr->size.width(), d_ptr->size.height());
        else
            fftshift(copy);
        return copy;
    }
    else {
        return d_ptr->dataOriginal;
    }
}

//! get forward fourier data
vector<complex<double>,fftalloc<complex<double> > > CalculationManager::dataFwdFourier(fftAction action)
{
    QMutexLocker locker(&mutex);

    if (action==fftAction::shift) {
        vector<complex<double>,fftalloc<complex<double> > > copy = d_ptr->dataFwdFourier;
        // shift copy
        if (d_ptr->is2D)
            fftshift(copy, d_ptr->size.width(), d_ptr->size.height());
        else
            fftshift(copy);
        return copy;
    }
    else {
        return d_ptr->dataFwdFourier;
    }
}

//! get invers fourier data
vector<complex<double>,fftalloc<complex<double> > > CalculationManager::dataInvFourier(fftAction action)
{
    QMutexLocker locker(&mutex);

    if (action==fftAction::shift) {
        vector<complex<double>,fftalloc<complex<double> > > copy = d_ptr->dataInvFourier;
        // shift copy
        if (d_ptr->is2D)
            fftshift(copy, d_ptr->size.width(), d_ptr->size.height());
        else
            fftshift(copy);
        return copy;
    }
    else {
        return d_ptr->dataInvFourier;
    }
}

//! get (complex) substrattion of original and invers fourier data
//! \note mathematically both must be the same.
vector<complex<double>,fftalloc<complex<double> > > CalculationManager::dataCompare(fftAction action)
{
    QMutexLocker locker(&mutex);

    vector<complex<double>,fftalloc<complex<double> > > dataCompare(d_ptr->N);
    // substract vectors
    std::transform(begin(d_ptr->dataOriginal),
                   end(d_ptr->dataOriginal),
                   begin(d_ptr->dataInvFourier),
                   begin(dataCompare),std::minus< complex<double> >());

//    for(std::vector<complex<double> >::size_type i = 0; i != dataCompare.size(); i++) {
//        if (abs(dataCompare[i]) < 1e-15) {
//            dataCompare[i] = 0;
//        }
//    }

    if (action==fftAction::shift)
        // shift copy
        if (d_ptr->is2D)
            fftshift(dataCompare, d_ptr->size.width(), d_ptr->size.height());
        else
            fftshift(dataCompare);

    return dataCompare;
}

//! execute series of forward and invers fft operations
//! \note This is a slot for the QThread signal started()
void CalculationManager::process()
{
    QMutexLocker locker(&mutex);

    cout << "calc FFT: process(), iterations: " << d_ptr->iterations<< endl;

    const clock_t clockStart=clock();
    double percent = 0;

    if (!d_ptr->is2D)
    {
        if (d_ptr->doShiftBeforeFFT)
            ifftshift(d_ptr->dataOriginal);

        // forward FFT;
        d_ptr->fft.fwd(d_ptr->dataFwdFourier, d_ptr->dataOriginal);

        for (size_t i = 0; i < d_ptr->iterations; ++i)
        {
            if (i>0) {
                // forward FFT; input is last invers FFT
                d_ptr->fft.fwd(d_ptr->dataFwdFourier, d_ptr->dataInvFourier);
            }

            // invers FFT;
            d_ptr->fft.inv(d_ptr->dataInvFourier, d_ptr->dataFwdFourier);

            percent = double(i+1)/(d_ptr->iterations)*100;
            emit iteration(int(percent));
        }
    }
    else { // 2D
        if (d_ptr->doShiftBeforeFFT)
            ifftshift(d_ptr->dataOriginal, d_ptr->size.width(), d_ptr->size.height());

        // forward FFT;
//        d_ptr->fft.fwd2D(d_ptr->dataFwdFourier, d_ptr->dataOriginal);

        for (size_t i = 0; i < d_ptr->iterations; ++i)
        {
            if (i>0) {
                // forward FFT; input is last invers FFT
//                d_ptr->fft.fwd2D(d_ptr->dataFwdFourier, d_ptr->dataInvFourier);
            }

            // invers FFT;
//            d_ptr->fft.inv2D(d_ptr->dataInvFourier, d_ptr->dataFwdFourier);

            percent = double(i+1)/(d_ptr->iterations)*100;
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
    d_ptr->speed = timeSeconds;
}

/*! time elapsed for FFT Calculation in seconds */
double CalculationManager::speed() const
{
    return d_ptr->speed;
}

/*! provide size of 1D vector or 2D matrix */
QSize CalculationManager::size() const
{
    return d_ptr->size;
}
