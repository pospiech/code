#include "plotcomplexdata.h"
#include "ui_plotcomplexdata.h"

#include <QGridLayout>
static const double pi = 3.14159265358979323846;

#include "plottools.h"
#include "phaseunwrap.h"

class PlotComplexDataPrivate
{
public:
    Q_DISABLE_COPY(PlotComplexDataPrivate)
    Q_DECLARE_PUBLIC(PlotComplexData)
    PlotComplexData * const q_ptr;

    PlotComplexDataPrivate(PlotComplexData * baseClass)
        : q_ptr(baseClass)
    {
        dimension = PlotComplexData::oneDim;
    }
    vector<double> dataAmplitude;
    vector<double> dataPhase;
    PlotComplexData::Dimension dimension;

    QwtPlot * plot1D;
    QwtPlot * plot2DAmplitude;
    QwtPlot * plot2DPhase;

    bool isRemovePhaseFlipping;

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

void PlotComplexData::setTitle(QString title)
{
//    plot->setTitle( title );
}

void PlotComplexData::setupPlot1D()
{
    Q_D(PlotComplexData);
    QLinePlot * plot = dynamic_cast<QLinePlot*>(d->plot1D);
    if (!plot)
        qFatal("error casting to QLinePlot");
    // remove all previous plotitems
    plot->clear();

    plot->setBoundingMarginPercent(10);

    // legend
//    QwtLegend *legend = new QwtLegend;
//    plot->insertLegend( legend, QwtPlot::BottomLegend );

    plot->enableAxis(QwtPlot::yRight);

    plot->setAxisTitle (QwtPlot::xBottom, "x / Data Points");
    plot->setAxisTitle (QwtPlot::yLeft, "amplitude");
    plot->setAxisTitle (QwtPlot::yRight, "phase (-pi..pi)");

    // Zoom
    plot->zoomerY2()->setEnabled(true);

    // Grid
    plot->grid()->enableX(true);
    plot->grid()->enableY(true);

    QPlotCurve *curve1 = new QPlotCurve();
    curve1->setTitle( "phase" );
    curve1->setPen( QColorPalette::color(1), 1 ),
    curve1->setYAxis(QwtPlot::yRight);
    // fix scale for Y2 axis
    plot->setAxisScale(QwtPlot::yRight, -pi, pi);

    QPlotCurve *curve2 = new QPlotCurve();
    curve2->setTitle( "amplitude" );
    curve2->setPen( QColorPalette::color(2), 1 ),
    curve2->setYAxis(QwtPlot::yLeft);

    // attach Curve to qwtPlot
    curve1->attach( plot );
    curve2->attach( plot );
    plot->replot();

    // curves must be attached to plot, otherwise
    // nothing is applied.
    plot->setColorPalette(QColorPalette::MSOffice2007);
}


//void PlotComplexData::setData(const std::vector<complex<double>,fftalloc<complex<double> > > & data, size_t sizeY = 0)
//{


//    // copy to internal vector
//    try {
//        d->dataAmplitude.resize(N);
//        d->dataPhase.resize(N);
//    } catch (std::bad_alloc const&) {
//        qCritical() << "Memory allocation fail!" << endl;
//        d->dataAmplitude.clear();
//        d->dataPhase.clear();
//        return;
//    }

//    for(std::vector<complex<double> >::size_type i = 0; i != data.size(); i++) {
//        d->dataAmplitude[i] = abs(data[i]);
//        d->dataPhase[i]     = arg(data[i]);
//    }
//}


void PlotComplexData::createPlotWidgets(PlotComplexData::Dimension dimension)
{
    Q_D(PlotComplexData);

    if (d->dimension != dimension)
    {
        QLayoutItem *child;
        while ((child = gridLayoutPlots->takeAt(0)) != 0)
            delete child;
    }
    switch (dimension)
    {
    case Dimension::oneDim:
        d->plot1D = new QLinePlot(this);
        gridLayoutPlots->addWidget(d->plot1D, 1, 1);
        setupPlot1D();
        break;
    case Dimension::twoDim:
        d->plot2DAmplitude = new QMatrixPlot(this);
        d->plot2DPhase = new QMatrixPlot(this);
        gridLayoutPlots->addWidget(d->plot2DAmplitude, 1, 1);
        gridLayoutPlots->addWidget(d->plot2DPhase, 1, 2);
        break;
    }

}

void PlotComplexData::updatePlotData(vector<double> & dataAmplitude, vector<double> & dataPhase, size_t sizeY = 0 )
{
    Q_D(PlotComplexData);

    // determine size of vector or matrix
    size_t sizeX;
    const size_t N = dataAmplitude.size();
    if (sizeY == 0)
    {
        sizeX = N;
        // create plot widgets (and destroy wrong ones)
        createPlotWidgets(Dimension::oneDim);
        d->dimension = Dimension::oneDim;
    }
    else
    {
        sizeX = N/sizeY;
        // check sizeX is a full number (integer)
        // create plot widgets (and destroy wrong ones)
        createPlotWidgets(Dimension::twoDim);
        d->dimension = Dimension::twoDim;
    }

    // create x-axis
    vector<double> xaxis(sizeX);
    const int halfsizeX = int(sizeX/2);
    for (size_t i=0; i < sizeX; ++i)
    {
        xaxis[i] = int(i) - halfsizeX;
    }
    // also for y-axis
    vector<double> yaxis(sizeY);
    if (sizeY > 0) {
        const int halfsizeY = int(sizeY/2);
        for (size_t i=0; i < sizeY; ++i)
        {
            yaxis[i] = int(i) - halfsizeY;
        }
    }
    // wrap phase values by 2 pi
    for (size_t i=0; i < dataPhase.size(); ++i)
    {
        dataPhase[i] = fmod(dataPhase[i], pi*1.00001);
    }
    // remove phase flipping in ifft data if selected
    if (d->isRemovePhaseFlipping) {
        removePhaseFlipping(dataPhase);
    }
    // manual scale: for amplitude (Phase is fixed anyway)
    double minValue = *std::min_element( std::begin(dataAmplitude), std::end(dataAmplitude) );
    double maxValue = *std::max_element( std::begin(dataAmplitude), std::end(dataAmplitude) );
    minValue = std::min(0.0, minValue);
    if (minValue == maxValue)
        maxValue = minValue + 1;

    switch (d->dimension)
    {
    case Dimension::oneDim:
    {
        // create line plot
        QLinePlot * plot1D = dynamic_cast<QLinePlot*>(d->plot1D);
        if (!plot1D)
            qFatal("error casting to QLinePlot");

        // pass data points to graphs:
        plot1D->curve(0)->setData(xaxis, dataPhase);
        plot1D->curve(1)->setData(xaxis, dataAmplitude);

        plot1D->setAxisScale(QwtPlot::yLeft,minValue,maxValue * 1.2);
        plot1D->replot();

        break;
    }
    case Dimension::twoDim:
    {
        QMatrixPlot * plot2DAmplitude = dynamic_cast<QMatrixPlot*>(d->plot2DAmplitude);
        if (!plot2DAmplitude)
            qFatal("error casting to QMatrixPlot");

        plot2DAmplitude->setAxisTitle (QwtPlot::xBottom, "x-axis");
        plot2DAmplitude->setAxisTitle (QwtPlot::yLeft, "y-axis");
//        plotMatrix->setAxisTitle (QwtPlot::yRight, "signal amplitude");

        plot2DAmplitude->setMatrixData(QVector<double>::fromStdVector(dataAmplitude),
                            xaxis.size(),
                            QwtInterval(xaxis.front(), xaxis.back()),
                            QwtInterval(yaxis.front(), yaxis.back()));
        plot2DAmplitude->setResampleMode(QwtMatrixRasterData::NearestNeighbour);
//        plotMatrix->setContourSteps(11);
//        plotMatrix->showContour( true );

        plot2DAmplitude->replot();

        break;
    }
    } // end switch

}


void PlotComplexData::updatePlotData(const std::vector<complex<double>,fftalloc<complex<double> > > & data, size_t sizeY = 0)
{
    size_t N = data.size();
    vector<double> dataAmplitude(N);
    vector<double> dataPhase(N);

    for(std::vector<complex<double> >::size_type i = 0; i != data.size(); i++) {
        dataAmplitude[i] = abs(data[i]);
        dataPhase[i]     = arg(data[i]);
    }

    updatePlotData(dataAmplitude, dataPhase, sizeY);
}



//void MainWindow::updatePlotData(QwtPlot *plot, vector<double> & dataAmplitude, vector<double> & dataPhase )
//{
//    vector<double> x(dataAmplitude.size());

//    int halfsize = int(dataAmplitude.size()/2);
//    // if phase values should be unwrapped wrap them
//    // by 2 pi to fit into complex values.
//    for (size_t i=0; i < dataPhase.size(); ++i)
//    {
//        x[i] = int(i) - halfsize;
//        dataPhase[i] = fmod(dataPhase[i], pi*1.00001);
//    }
//    // remove phase flipping in ifft data if selected
//    if (ui->checkBoxPhaseUnwrap->checkState() == Qt::Checked) {
//        if (plot->property("type").toString() == "ifft")
//        {
//            removePhaseFlipping(dataPhase);
//        }
//    }
//    // currently only 1D plots are supported
//    QLinePlot * plot1D = dynamic_cast<QLinePlot*>(plot);
//    if (!plot1D)
//        return;

//    // pass data points to graphs:
//    plot1D->curve(0)->setData(x, dataPhase);
//    plot1D->curve(1)->setData(x, dataAmplitude);

//    // manual scale: for amplitude (Phase is fixed anyway)
//    double minValue = *std::min_element( std::begin(dataAmplitude), std::end(dataAmplitude) );
//    double maxValue = *std::max_element( std::begin(dataAmplitude), std::end(dataAmplitude) );
//    minValue = std::min(0.0, minValue);

//    if (minValue == maxValue)
//    {
//        maxValue = minValue + 1;
//    }

//    plot1D->setAxisScale(QwtPlot::yLeft,minValue,maxValue * 1.2);
//    plot1D->replot();
//}
