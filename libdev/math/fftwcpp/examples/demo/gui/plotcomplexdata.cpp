#include "plotcomplexdata.h"
//#include "ui_plotcomplexdata.h"

#include <QGridLayout>
#include <QThread>
static const double pi = 3.14159265358979323846;

#include "plottools.h"
#include "phaseunwrap.h"
#include "layouts.h"

class PlotComplexDataPrivate
{
public:
    friend class PlotComplexData;
    Q_DISABLE_COPY(PlotComplexDataPrivate)
    Q_DECLARE_PUBLIC(PlotComplexData)
    PlotComplexData * const q_ptr;

    PlotComplexDataPrivate(PlotComplexData * baseClass)
        : q_ptr(baseClass)        
    {
        setSize(QSize(0,0));
        dimension = PlotComplexData::Dimension::undefined;
        dimensionChanged = false;
    }

    vector<double> dataAmplitude;
    vector<double> dataPhase;
    PlotComplexData::Dimension dimension;

    QwtPlot * plot1D;
    QwtPlot * plot2DAmplitude;
    QwtPlot * plot2DPhase;

    QString plotTitle;

    size_t sizeX;
    size_t sizeY;

    bool isRemovePhaseFlipping;
    bool dimensionChanged;

    void setSize(QSize size)
    {
        PlotComplexData::Dimension previousDimension = dimension;
        if (size.height() == 0){
            dimension = PlotComplexData::Dimension::oneDim;
            sizeX = size.width();
            sizeY = 0;
        }
        else {
            dimension = PlotComplexData::Dimension::twoDim;
            sizeX = size.width();
            sizeY = size.height();
        }
        if (previousDimension != dimension)
            dimensionChanged = true;
        else
            dimensionChanged = false;
    }
};

PlotComplexData::PlotComplexData(QWidget *parent) :
    QWidget(parent),
    d_ptr(new PlotComplexDataPrivate(this))
{
    gridLayoutPlots = new QGridLayout(this);
    gridLayoutPlots->setObjectName(QStringLiteral("gridLayoutPlots"));

    createPlotWidgets(Dimension::oneDim);
}

PlotComplexData::~PlotComplexData()
{
}

void PlotComplexData::setTitle(QString title)
{
    Q_D(PlotComplexData);

    if (title.isEmpty())
        return;

    switch (d->dimension)
    {
    case Dimension::oneDim:
        d->plot1D->setTitle( title );
        break;
    case Dimension::twoDim:
//        gridLayoutPlots->
        break;
    case Dimension::undefined:
        break;
    }
    // save in case Dimension was undefined
    d->plotTitle = title;
}
void PlotComplexData::setupLinePlot(QwtPlot * qwtplot)
{

    QLinePlot * plot = dynamic_cast<QLinePlot*>(qwtplot);
    if (!plot)
        qFatal("error casting to QLinePlot");

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

    // remove all previous plotitems
    plot->clear();

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

    // curves must be attached to plot, otherwise
    // nothing is applied.
    plot->setColorPalette(QColorPalette::MSOffice2007);

    plot->replot();
}

void PlotComplexData::setupMatrixPlot(QwtPlot * plot, const QString title)
{    
    QMatrixPlot * plotMatrix = dynamic_cast<QMatrixPlot*>(plot);
    if (!plotMatrix)
        qFatal("error casting to QMatrixPlot");


    plotMatrix->setTitle(title);
    plotMatrix->setAxisTitle (QwtPlot::xBottom, "");
    plotMatrix->setAxisTitle (QwtPlot::yLeft, "");
    plotMatrix->setAxisTitle (QwtPlot::yRight, "");


    // enable interpolation, otherwise use QwtMatrixRasterData::NearestNeighbour
    plotMatrix->setResampleMode(QwtMatrixRasterData::NearestNeighbour);

    // enable contour lines
//    plotMatrix->setContourSteps(11);
//    plotMatrix->showContour( true );

    // set color map
    plotMatrix->setColorMap( QColorMap::Jet );
    plotMatrix->setAutoReplot( true );

    plotMatrix->replot();

}

void PlotComplexData::createPlotWidgets(PlotComplexData::Dimension dimension)
{
    Q_D(PlotComplexData);

    if (d->dimensionChanged)
    {
        clearLayout(gridLayoutPlots);

        // create widgets only on Dimension changes and initially
        switch (dimension)
        {
        case Dimension::oneDim:
            d->plot1D = new QLinePlot(this);
            setupLinePlot(d->plot1D);
            gridLayoutPlots->addWidget(d->plot1D);
            break;
        case Dimension::twoDim:
            d->plot2DAmplitude = new QMatrixPlot(this);
            d->plot2DPhase = new QMatrixPlot(this);
            gridLayoutPlots->addWidget(d->plot2DAmplitude, 1, 1);
            gridLayoutPlots->addWidget(d->plot2DPhase, 1, 2);
            setupMatrixPlot(d->plot2DAmplitude, "Amplitude");
            setupMatrixPlot(d->plot2DPhase, "Phase");
            break;
        case Dimension::undefined:
            qFatal("createPlotWidgets: Dimension must not be undefined");
            break;
        }
        // reset dimensionChanged
        d->dimensionChanged = false;
    }
    // update title if set previously
    setTitle(d->plotTitle);
}

vector<double> PlotComplexData::createAxis(size_t length)
{
    // create x-axis
    vector<double> axis;
    try {
        axis.resize(length);
    } catch (std::bad_alloc const&) {
        qCritical() << "Memory allocation fail!" << endl;
    }

    const int halfsize = int(length/2);
    for (size_t i=0; i < length; ++i)
    {
        axis[i] = int(i) - halfsize;
    }
    return axis;
}

void PlotComplexData::updatePlotData(vector<double> & dataAmplitude, vector<double> & dataPhase, QSize size)
{
    Q_D(PlotComplexData);

    // set sizeX and sizeY and determine Dimension
    d->setSize(size);
    // create plot widgets for Dimension
    createPlotWidgets(d->dimension);

    // create x-axis
    vector<double> xaxis = createAxis(d->sizeX);
    // also for y-axis
    vector<double> yaxis;
    if (d->sizeY > 0)
        yaxis = createAxis(d->sizeY);

    // wrap phase values by 2 pi
    std::transform(dataPhase.begin(), dataPhase.end(), dataPhase.begin(),
                   [](double v){return fmod(v, pi*1.00001); }
    );

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

        plot2DAmplitude->setMatrixData(QVector<double>::fromStdVector(dataAmplitude),
                            xaxis.size(),
                            QwtInterval(xaxis.front(), xaxis.back()),
                            QwtInterval(yaxis.front(), yaxis.back()));

//        plot2DAmplitude->canvas()->setPaintAttribute(QwtPlotCanvas::ImmediatePaint);
//        plot2DAmplitude->zoomerY1()->setZoomBase(true);
        QThread::sleep(0.2);
        plot2DAmplitude->replot();

        QMatrixPlot * plot2DPhase = dynamic_cast<QMatrixPlot*>(d->plot2DPhase);
        if (!plot2DPhase)
            qFatal("error casting to QMatrixPlot");

        plot2DPhase->setMatrixData(QVector<double>::fromStdVector(dataPhase),
                            xaxis.size(),
                            QwtInterval(xaxis.front(), xaxis.back()),
                            QwtInterval(yaxis.front(), yaxis.back()));

//        plot2DPhase->canvas()->setPaintAttribute(QwtPlotCanvas::ImmediatePaint);
        QThread::sleep(0.2);
        plot2DPhase->replot();


        break;
    }
    case Dimension::undefined:
        qFatal("updatePlotData: Dimension must not be undefined");
        break;
    } // end switch

}


void PlotComplexData::updatePlotData(const std::vector<complex<double>,fftalloc<complex<double> > > & data, QSize size)
{
    size_t N = data.size();
    vector<double> dataAmplitude(N);
    vector<double> dataPhase(N);

    for(std::vector<complex<double> >::size_type i = 0; i != data.size(); i++) {
        dataAmplitude[i] = abs(data[i]);
        dataPhase[i]     = arg(data[i]);
    }

    updatePlotData(dataAmplitude, dataPhase, size);
}
