#include "plotcomplexdata.h"
#include "ui_plotcomplexdata.h"

#include <QGridLayout>

#include "plottools.h"



class PlotComplexDataPrivate
{
public:
    Q_DISABLE_COPY(PlotComplexDataPrivate)
    Q_DECLARE_PUBLIC(PlotComplexData)
    PlotComplexData * const q_ptr;

    PlotComplexDataPrivate(PlotComplexData * baseClass)
        : q_ptr(baseClass)
    {

    }
    vector<double> dataAmplitude;
    vector<double> dataPhase;
    PlotComplexData::Dimension dimension;

    QwtPlot * plot1D;
    QwtPlot * plot2D;

    bool isPhaseUnwrap;

};

PlotComplexData::PlotComplexData(QWidget *parent) :
    QWidget(parent),
    d_ptr(new PlotComplexDataPrivate(this))
{
//    ui(new Ui::PlotComplexData),

//    ui->setupUi(this);

    gridLayoutPlots = new QGridLayout(this);
    gridLayoutPlots->setObjectName(QStringLiteral("gridLayoutPlots"));

    createPlotWidgets(Dimension::oneDim);
}

PlotComplexData::~PlotComplexData()
{
//    delete ui;
}

void PlotComplexData::setData(const std::vector<complex<double>,fftalloc<complex<double> > > & data, size_t sizeY = 0)
{
    Q_D(PlotComplexData);

    // determine size of vector or matrix
    size_t sizeX;
    size_t N = data.size();
    if (sizeY == 0)
    {
        sizeX = N;
        d->dimension = Dimension::oneDim;
    }
    else
    {
        sizeX = N/sizeY;
        // check sizeX is a full number (integer)

        d->dimension = Dimension::twoDim;
    }

    // copy to internal vector
    try {
        d->dataAmplitude.resize(N);
        d->dataPhase.resize(N);
    } catch (std::bad_alloc const&) {
        qCritical() << "Memory allocation fail!" << endl;
        d->dataAmplitude.clear();
        d->dataPhase.clear();
        return;
    }

    for(std::vector<complex<double> >::size_type i = 0; i != data.size(); i++) {
        d->dataAmplitude[i] = abs(data[i]);
        d->dataPhase[i]     = arg(data[i]);
    }
}


void PlotComplexData::createPlotWidgets(PlotComplexData::Dimension dimension)
{
    Q_D(PlotComplexData);

    switch (dimension)
    {
    case Dimension::oneDim:
        d->plot1D = new QLinePlot(this);
        gridLayoutPlots->addWidget(d->plot1D);
        break;
    case Dimension::twoDim:
        break;
    }

}

void PlotComplexData::updatePlotData(vector<double> & dataAmplitude, vector<double> & dataPhase )
{
    Q_D(PlotComplexData);

    vector<double> x(dataAmplitude.size());

    int halfsize = int(dataAmplitude.size()/2);
    // if phase values should be unwrapped wrap them
    // by 2 pi to fit into complex values.
//    for (size_t i=0; i < dataPhase.size(); ++i)
//    {
//        x[i] = int(i) - halfsize;
//        dataPhase[i] = fmod(dataPhase[i], pi*1.00001);
//    }
//    // remove phase flipping in ifft data if selected
//    if (d->isPhaseUnwrap) {
//        if (plot->property("type").toString() == "ifft")
//        {
//            removePhaseFlipping(dataPhase);
//        }
//    }
//    // currently only 1D plots are supported
    QLinePlot * plot1D = dynamic_cast<QLinePlot*>(d->plot1D);
    if (!plot1D)
        return;

    // pass data points to graphs:
    plot1D->curve(0)->setData(x, dataPhase);
    plot1D->curve(1)->setData(x, dataAmplitude);

    // manual scale: for amplitude (Phase is fixed anyway)
    double minValue = *std::min_element( std::begin(dataAmplitude), std::end(dataAmplitude) );
    double maxValue = *std::max_element( std::begin(dataAmplitude), std::end(dataAmplitude) );
    minValue = std::min(0.0, minValue);

    if (minValue == maxValue)
    {
        maxValue = minValue + 1;
    }

    plot1D->setAxisScale(QwtPlot::yLeft,minValue,maxValue * 1.2);
    plot1D->replot();
}
