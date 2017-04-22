#ifndef CALCULATIONMANAGER_H
#define CALCULATIONMANAGER_H

#include <QObject>
#include <QMutex>
#include <QScopedPointer>
#include <complex>
using std::complex;
#include <vector>
using std::vector;

#include "fftallocator.h"
using namespace fftwcpp;

class CalculationManagerPrivate;

enum class fftAction {shift=true, noshift=false};

class CalculationManager : public QObject
{
    Q_OBJECT
public:
    CalculationManager();
    virtual ~CalculationManager();

    void setShiftBeforeFFT(bool doShift);
    bool shiftBeforeFFT() const;
    void setData(const vector<complex<double>,fftalloc<complex<double> > > & data);
    void setIterations(size_t N);
    void setSpeed(double timeSeconds);
    double speed() const;

    vector<complex<double>,fftalloc<complex<double> > > data(fftAction action = fftAction::noshift);
    vector<complex<double>,fftalloc<complex<double> > > dataFwdFourier(fftAction doShift = fftAction::noshift);
    vector<complex<double>,fftalloc<complex<double> > > dataInvFourier(fftAction doShift = fftAction::noshift);
    vector<complex<double>,fftalloc<complex<double> > > dataCompare(fftAction doShift = fftAction::noshift);

public slots:
    // thread run function
    void process();    

signals:
    void finished();
    void error(QString err);
    void iteration(int);

protected:
    const QScopedPointer<CalculationManagerPrivate> d_ptr;

private:
    // requires d_ptr
    Q_DECLARE_PRIVATE(CalculationManager)
    // mutex for locking shared data
    QMutex mutex;
};

#endif // CALCULATIONMANAGER_H
