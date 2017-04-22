#ifndef PLOTCOMPLEXDATA_H
#define PLOTCOMPLEXDATA_H

#include <QWidget>
#include <QScopedPointer>

#include <vector>
using std::vector;

#include <complex>
using std::complex;

#include "fftallocator.h"
using namespace fftwcpp;



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
    void setData(const std::vector<complex<double>,fftalloc<complex<double> > > & data, size_t sizeY /* = 0 */);
    void updatePlotData(const std::vector<complex<double>,fftalloc<complex<double> > > & data, size_t sizeY);
    void updatePlotData(vector<double> & dataAmplitude, vector<double> & dataPhase , size_t sizeY);

private:
    void createPlotWidgets(PlotComplexData::Dimension dimension);
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
