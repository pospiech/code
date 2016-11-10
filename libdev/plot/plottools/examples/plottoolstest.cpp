#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>

#include <QDebug>
#include <QHBoxLayout>

#include <qwt_legend.h>
#include <qwt_plot_legenditem.h>

#include "qlineplot.h"
#include "qplotcurve.h"
#include "qcolorpalette.h"
#include "qmatrixplot.h"


#include <vector>
using std::vector;

#include "functions.h"

QWidget * createLinePlot(vector<double> & x, vector<double> & y1)
{
    /// first plot (single line)
    QLinePlot * plot1 = new QLinePlot();
    plot1->setTitle( "Plot Demo - single Y axis" );

    // legend item
    QwtPlotLegendItem * legendItem = new QwtPlotLegendItem();
    legendItem->attach( plot1 );

    legendItem->setRenderHint( QwtPlotItem::RenderAntialiased );
    legendItem->setMaxColumns( 1 );
    legendItem->setAlignment( Qt::AlignRight );

    QColor color( Qt::white );
    legendItem->setTextPen( color );
    legendItem->setBorderPen( color );

    QColor c( Qt::gray );
    c.setAlpha( 200 );
    legendItem->setBackgroundBrush( c );


    plot1->setAxisTitle (QwtPlot::xBottom, "x-axis");
    plot1->setAxisTitle (QwtPlot::yLeft, "y-axis 1");

    // Grid
    plot1->grid()->enableX(true);
    plot1->grid()->enableY(true);

    // first curve
    QPlotCurve *curve = new QPlotCurve();
    curve->setTitle( "sin(x)" );
    curve->setPen( QColorPalette::color(1), 4 ),
    curve->setYAxis(QwtPlot::yLeft);
    curve->setRawSamples(&x[0], &y1[0], x.size());

    // attach Curve to qwtPlot
    curve->attach( plot1 );
    plot1->replot();

    // curves must be attached to plot, otherwise
    // nothing is applied.
    plot1->setColorPalette(QColorPalette::MSOffice2007);

    return plot1;
}


QWidget * createLinePlot2Y(vector<double> & x, vector<double> & y21, vector<double> & y22)
{
    /// second plot (two lines, on two y axis)
    QLinePlot * plot2 = new QLinePlot();
    plot2->setTitle( "Plot Demo - two Y axis" );

    // legend
    QwtLegend *legend = new QwtLegend;
    plot2->insertLegend( legend, QwtPlot::BottomLegend );

    plot2->enableAxis(QwtPlot::yRight);

    plot2->setAxisTitle (QwtPlot::xBottom, "x-axis");
    plot2->setAxisTitle (QwtPlot::yLeft, "y-axis 1");
    plot2->setAxisTitle (QwtPlot::yRight, "y-axis 2");

    // Zoom
    plot2->zoomerY2()->setEnabled(true);

    // Grid
    plot2->grid()->enableX(true);
    plot2->grid()->enableY(true);

    QPlotCurve *curve21 = new QPlotCurve();
    curve21->setTitle( "x^2" );
    curve21->setPen( QColorPalette::color(1), 4 ),
    curve21->setYAxis(QwtPlot::yLeft);
    curve21->setData(x, y21);

    QPlotCurve *curve22 = new QPlotCurve();
    curve22->setTitle( "sin(x)" );
    curve22->setPen( QColorPalette::color(2), 4 ),
    curve22->setYAxis(QwtPlot::yRight);
    curve22->setData(x, y22);

    // attach Curve to qwtPlot
    curve21->attach( plot2 );
    curve22->attach( plot2 );

    plot2->replot();

    // curves must be attached to plot, otherwise
    // nothing is applied.
    plot2->setColorPalette(QColorPalette::MSOffice2007);

    return plot2;
}

QWidget * createMatrixPlot(vector<double> & xaxis, vector<double> & yaxis, vector<double> & data)
{
    QMatrixPlot * plotMatrix = new QMatrixPlot();
    plotMatrix->setTitle( "2D Plot Demo" );

    plotMatrix->setAxisTitle (QwtPlot::xBottom, "x-axis");
    plotMatrix->setAxisTitle (QwtPlot::yLeft, "y-axis");
    plotMatrix->setAxisTitle (QwtPlot::yRight, "signal amplitude");

    plotMatrix->setMatrixData(QVector<double>::fromStdVector(data),
                        xaxis.size(),
                        QwtInterval(xaxis.front(), xaxis.back()),
                        QwtInterval(yaxis.front(), yaxis.back()));
    plotMatrix->setResampleMode(QwtMatrixRasterData::BilinearInterpolation);
    plotMatrix->setContourSteps(11);
    plotMatrix->showContour( true );



    plotMatrix->replot();

    return plotMatrix;
}

int main( int argc, char **argv )
{
    QApplication a( argc, argv );

    // **** plot 1 **************************
    vector<double> x(100);
    vector<double> y1(x.size());
    for (size_t i = 0; i< x.size(); ++i) { x[i] = int(i)-50; }
    for (size_t i = 0; i< y1.size(); ++i) { y1[i] = sin(double(x[i])/10.0); }

    QWidget * plot1 = createLinePlot(x, y1);

    // **** plot 2 **************************
    vector<double> y21(x.size());
    vector<double> y22(x.size());

    for (size_t i = 0; i< x.size(); ++i) { x[i] = int(i)-50; }
    for (size_t i = 0; i< y21.size(); ++i) { y21[i] = pow(double(x[i])/10,2); }
    for (size_t i = 0; i< y22.size(); ++i) { y22[i] = sin(double(x[i])/10.0); }

    QWidget * plot2 = createLinePlot2Y(x, y21, y22);

    // **** plot 3 **************************
    RippleFunction function;
    function.setDataPointsX(1000);
    function.setDataPointsY(1000);
    function.setRangeX(-10, 10);
    function.setRangeY(-10, 10);

    vector<double> xaxis;
    vector<double> yaxis;
    vector<double> data;

    function.calculate2D(&xaxis, &yaxis, &data);

    QWidget * plotMatrix = createMatrixPlot(xaxis, yaxis, data);


    // **** add all plots to the window **************************
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(plot1);
    layout->addWidget(plot2);
    layout->addWidget(plotMatrix);

    // Set layout in QWidget
    QWidget *centralWidget = new QWidget();
    centralWidget->setLayout(layout);

    QMainWindow window;
    window.setCentralWidget(centralWidget);
    window.resize(1200, 400);
    window.show();

    return a.exec();
}
