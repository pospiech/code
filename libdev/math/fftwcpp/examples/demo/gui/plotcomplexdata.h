#ifndef PLOTCOMPLEXDATA_H
#define PLOTCOMPLEXDATA_H

#include <QWidget>
#include <QScopedPointer>
#include <QSize>

#include <vector>
using std::vector;

#include <complex>
using std::complex;

#include "fftallocator.h"
using namespace fftwcpp;

#include "plottools.h"


namespace Ui {
class PlotComplexData;
}

class PlotComplexDataPrivate;
class QGridLayout;

class PlotComplexData : public QWidget
{
    Q_OBJECT

public:
    explicit PlotComplexData(QWidget *parent = 0);
    virtual ~PlotComplexData();

    enum Dimension {
        oneDim = 1, // 1D
        twoDim = 2  // 2D
    };

    void setTitle(QString title);
    void updatePlotData(const std::vector<complex<double>,fftalloc<complex<double> > > & data, QSize size);
    void updatePlotData(vector<double> & dataAmplitude, vector<double> & dataPhase, QSize size);

private:
    void createPlotWidgets(PlotComplexData::Dimension dimension);
    vector<double> createAxis(size_t length);
    void setupPlot();


protected:
    const QScopedPointer<PlotComplexDataPrivate> d_ptr;

private:
    Q_DECLARE_PRIVATE(PlotComplexData)

private:
    Ui::PlotComplexData *ui;
    QGridLayout *gridLayoutPlots;
};

#endif // PLOTCOMPLEXDATA_H
