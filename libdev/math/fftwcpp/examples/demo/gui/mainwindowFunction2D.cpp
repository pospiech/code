#include "mainwindowFunction2D.h"
#include "ui_mainwindowFunction2D.h"

#include <algorithm> // std::min_element
#include <iterator>  // std::begin, std::end

#include "functions/examplefunctions.h"
#include "math/sgn.h"

#include <complex>
using std::complex;

#include "layouts.h"

#include "plottools.h"

// all plot details are within this class
// the mainwindow only transfers data
#include "plotcomplexdata.h"

class DataMinMax {
public:
    DataMinMax() : min(0), max(0) {}
    DataMinMax(double minimum, double maximum) : min(minimum), max(maximum) {}
    double min;
    double max;
};


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setupWidgets(3);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupWidgets(const int functionIndex)
{

    functionListAmplitude.append(QSharedPointer<FunctionFactory>(new NullFunction));
    functionListAmplitude.append(QSharedPointer<FunctionFactory>(new OneFunction));
    functionListAmplitude.append(QSharedPointer<FunctionFactory>(new RectFunction));
    functionListAmplitude.append(QSharedPointer<FunctionFactory>(new GaussFunction));
    functionListAmplitude.append(QSharedPointer<FunctionFactory>(new SinusFunction));
    functionListAmplitude.append(QSharedPointer<FunctionFactory>(new RectGratingFunction));
    functionListAmplitude.append(QSharedPointer<FunctionFactory>(new TiltFunction));
    functionListAmplitude.append(QSharedPointer<FunctionFactory>(new FresnelLensFunction));

    functionListPhase = functionListAmplitude;

    QMatrixPlot * plotMatrix = new QMatrixPlot(this);
    ui->gridLayoutPlots->addWidget(plotMatrix, 1, 1);

    const int N = 256;

    QSharedPointer<FunctionFactory> functionAmplitude =
            QSharedPointer<FunctionFactory>(this->functionListAmplitude.at(functionIndex));

    QSharedPointer<ComplexFunctionFactory>  complexFunctionFactory =
            QSharedPointer<ComplexFunctionFactory>(new ComplexFunctionFactory);

    complexFunctionFactory->setAmplitudeFunction(functionAmplitude);
    vector<double> data = complexFunctionFactory->amplitudeData(N,N);


    vector<double> xAxis(N);
    vector<double> yAxis(N);

    DataMinMax rangeX;
    DataMinMax rangeY;
    rangeX.min = -N/2;
    rangeX.max = N/2;
    rangeY = rangeX;

    // calculate x-Axis
    for (size_t ix=0; ix < xAxis.size(); ++ix)
    {
        double x = rangeX.min + double(ix);
        xAxis[ix] = x;
    }

    // calculate y-Axis
    for (size_t iy=0; iy < yAxis.size(); ++iy)
    {
        double y = rangeY.min + double(iy);
        yAxis[iy] = y;
    }


    plotMatrix->setTitle( this->functionListAmplitude.at(functionIndex)->description() );
    plotMatrix->setAxisTitle (QwtPlot::xBottom, "x-axis");
    plotMatrix->setAxisTitle (QwtPlot::yLeft, "y-axis");
    plotMatrix->setAxisTitle (QwtPlot::yRight, "signal amplitude");

    plotMatrix->setMatrixData(QVector<double>::fromStdVector(data),
                        xAxis.size(),
                        QwtInterval(xAxis.front(), xAxis.back()),
                        QwtInterval(yAxis.front(), yAxis.back()));

    // enable interpolation, otherwise use QwtMatrixRasterData::NearestNeighbour
    plotMatrix->setResampleMode(QwtMatrixRasterData::NearestNeighbour);

    // enable contour lines
//    plotMatrix->setContourSteps(11);
//    plotMatrix->showContour( true );

    // set color map
    plotMatrix->setColorMap( QColorMap::Jet );

    plotMatrix->replot();


}
