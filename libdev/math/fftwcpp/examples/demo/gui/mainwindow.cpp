#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDoubleSpinBox>

#include <algorithm> // std::min_element
#include <iterator>  // std::begin, std::end

#include <QList>
#include <QSharedPointer>
#include <QThread>
#include <QButtonGroup>

#include "functions/examplefunctions.h"
#include "math/sgn.h"

#include <complex>
using std::complex;

#include "layouts.h"

#include "fftwcpp.h"
#include "fftshift.h"
#include "fftallocator.h"
using namespace fftwcpp;

#include "phaseunwrap.h"
#include "plotcomplexdata.h"

#include "plottools.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    complexFF(new ComplexFunctionFactory()),
    calculationManager(new CalculationManager()),
    thread(new QThread(this))
{
    ui->setupUi(this);
    setupWidgets();

    // setup thread connections
    connect(thread, SIGNAL (started()), calculationManager.data(), SLOT (process()));
    connect(calculationManager.data(), SIGNAL (finished()), thread, SLOT (quit()));
    connect(calculationManager.data(), SIGNAL (finished()), this, SLOT (getResultsAndPlot()));
    connect(calculationManager.data(), SIGNAL (iteration(int)), this, SLOT (updateProgressBar(int)));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupWidgets()
{
    ui->spinBoxIterations->setMaximum(10000);

    functionListAmplitude.append(QSharedPointer<FunctionFactory>(new NullFunction));
    functionListAmplitude.append(QSharedPointer<FunctionFactory>(new OneFunction));
    functionListAmplitude.append(QSharedPointer<FunctionFactory>(new RectFunction));
    functionListAmplitude.append(QSharedPointer<FunctionFactory>(new GaussFunction));
    functionListAmplitude.append(QSharedPointer<FunctionFactory>(new SinusFunction));
    functionListAmplitude.append(QSharedPointer<FunctionFactory>(new RectGratingFunction));
    functionListAmplitude.append(QSharedPointer<FunctionFactory>(new TiltFunction));
    functionListAmplitude.append(QSharedPointer<FunctionFactory>(new FresnelLensFunction));

    functionListPhase = functionListAmplitude;

    for (int i = 0; i < functionListAmplitude.size(); ++i) {
        QString functionName;
        functionName = functionListAmplitude.at(i)->description();
        ui->comboBoxFunctionsAmplitude->addItem(functionName, i);
        ui->comboBoxFunctionsPhase->addItem(functionName, i);
    }

    ui->comboBoxDataPoints->addItem("64", 64);
    ui->comboBoxDataPoints->addItem("128", 128);
    ui->comboBoxDataPoints->addItem("256", 256);
    ui->comboBoxDataPoints->addItem("512", 512);
    ui->comboBoxDataPoints->addItem("1024", 1024);
    ui->comboBoxDataPoints->addItem("4096", 4096);
    ui->comboBoxDataPoints->addItem("16384", 16384);

    for (size_t i = 0; i < 4; ++i) {
        this->plotList.append(new QLinePlot(this));
    }
    ui->gridLayoutPlots->addWidget(plotList[0], 1, 1);
    ui->gridLayoutPlots->addWidget(plotList[1], 1, 2);
    ui->gridLayoutPlots->addWidget(plotList[2], 2, 1);
    ui->gridLayoutPlots->addWidget(plotList[3], 2, 2);

    // make plot classed identifiable
    plotList[0]->setProperty("type", "data");
    plotList[1]->setProperty("type", "fft");
    plotList[2]->setProperty("type", "ifft");
    plotList[3]->setProperty("type", "compare");

    // configure their apperience
    setupPlot(plotList[0], "Original Data");
    setupPlot(plotList[1], "Fourier Transform");
    setupPlot(plotList[2], "Invers Fourier Transform of Fourier Data");
    setupPlot(plotList[3], "Difference to Original Data");

    // init data
    ui->comboBoxFunctionsAmplitude->setCurrentIndex(3);
    ui->comboBoxFunctionsPhase->setCurrentIndex(5);

    buttonGroupFFTDimension = new QButtonGroup(this);
    buttonGroupFFTDimension->addButton(ui->radioButton1D, 1);
    buttonGroupFFTDimension->addButton(ui->radioButton2D, 2);
    buttonGroupFFTDimension->setExclusive(true);
    ui->radioButton1D->setChecked(true);

    connect(buttonGroupFFTDimension, static_cast<void(QButtonGroup::*)(int)>(&QButtonGroup::buttonClicked),
            this, &MainWindow::on_buttonGroupFFTDimension_buttonClicked);

//    QButtonGroup::buttonClicked(int id)
//    connect(buttonGroup, static_cast<void(QButtonGroup::*)(int)>(&QButtonGroup::buttonClicked),
//            [=](int id){
//        qDebug() << "button ID: " << id ;
//    });

    on_pushButtonStartFFT_clicked();

}

void MainWindow::on_buttonGroupFFTDimension_buttonClicked(int id)
{

}

void MainWindow::setupPlot(QLinePlot *plot, QString title)
{
    // remove all previous plotitems
    plot->clear();

    plot->setTitle( title );
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
    plot->replot();

    // curves must be attached to plot, otherwise
    // nothing is applied.
    plot->setColorPalette(QColorPalette::MSOffice2007);

}


void MainWindow::updatePlotData(QwtPlot *plot, const std::vector<complex<double>,fftalloc<complex<double> > > & data)
{
    size_t N = data.size();
    vector<double> dataAmplitude(N);
    vector<double> dataPhase(N);

    for(std::vector<complex<double> >::size_type i = 0; i != data.size(); i++) {
        dataAmplitude[i] = abs(data[i]);
        dataPhase[i]     = arg(data[i]);
    }

    updatePlotData(plot, dataAmplitude, dataPhase);

}



void MainWindow::updatePlotData(QwtPlot *plot, vector<double> & dataAmplitude, vector<double> & dataPhase )
{
    vector<double> x(dataAmplitude.size());

    int halfsize = int(dataAmplitude.size()/2);
    // if phase values should be unwrapped wrap them
    // by 2 pi to fit into complex values.
    for (size_t i=0; i < dataPhase.size(); ++i)
    {
        x[i] = int(i) - halfsize;
        dataPhase[i] = fmod(dataPhase[i], pi*1.00001);
    }
    // remove phase flipping in ifft data if selected
    if (ui->checkBoxPhaseUnwrap->checkState() == Qt::Checked) {
        if (plot->property("type").toString() == "ifft")
        {
            removePhaseFlipping(dataPhase);
        }
    }
    // currently only 1D plots are supported
    QLinePlot * plot1D = dynamic_cast<QLinePlot*>(plot);
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

void MainWindow::on_actionQuit_triggered()
{
    QApplication::quit();
}

void MainWindow::on_comboBoxFunctionsAmplitude_currentIndexChanged(int index)
{
    createFunctionParameterList(ComplexType::Amplitude, index);
}

void MainWindow::on_comboBoxFunctionsPhase_currentIndexChanged(int index)
{
    createFunctionParameterList(ComplexType::Phase, index);
}

//! create unique function parameter list for Phase or Amplitude type
void MainWindow::createFunctionParameterList(ComplexType type, int index)
{
    int iList = 0;
    QVBoxLayout *verticalLayout;
    QSharedPointer<FunctionFactory> function;
    // determine combobox list index, layout and parameterlist
    switch (type){
    case Amplitude:
        iList = ui->comboBoxFunctionsAmplitude->itemData(index).toInt();
        verticalLayout = ui->verticalLayoutAmplitude;
        function = functionListAmplitude.at(iList);
        mapAmplitudeParameterWidgets.clear();
        break;
    case Phase:
        iList = ui->comboBoxFunctionsPhase->itemData(index).toInt();
        verticalLayout = ui->verticalLayoutPhase;
        function = functionListPhase.at(iList);
        mapPhaseParameterWidgets.clear();
        break;
    }
    // clear layout with parameter list
    clearLayout(verticalLayout);
    // create new widgets
    const int N = ui->comboBoxDataPoints->currentData().toInt();

    for (int i = 0; i < function->parameterList().size(); ++i) {

        QString valueName = function->parameterList().at(i);
        double value = function->parameter(valueName);
        double minLimit = function->min(valueName, N);
        double maxLimit = function->max(valueName, N);

        addFunctionParameter(type, verticalLayout, valueName, value, minLimit, maxLimit);
    }
}

void MainWindow::updateFunctionParameterSettings()
{
    applyFunctionParameterSetting(this->functionListAmplitude.at(ui->comboBoxFunctionsAmplitude->currentIndex()),
                                  mapAmplitudeParameterWidgets);

    applyFunctionParameterSetting(this->functionListPhase.at(ui->comboBoxFunctionsPhase->currentIndex()),
                                  mapPhaseParameterWidgets);
}

void MainWindow::applyFunctionParameterSetting(QSharedPointer<FunctionFactory> function,
                                               QMap<QString, QWidget*> map)
{
    const int N = ui->comboBoxDataPoints->currentData().toInt();

    // iterate over all parameters of current selected function
    for (int i = 0; i < function->parameterList().size(); ++i) {

        QString valueName = function->parameterList().at(i);
        double value = function->parameter(valueName);
        const double min = function->min(valueName, N);
        const double max = function->max(valueName, N);

        // select corresponding spinbox

        QDoubleSpinBox * spinbox = dynamic_cast<QDoubleSpinBox*>(map[valueName]);
        if (spinbox) {
            spinbox->blockSignals(true);
            spinbox->setMinimum(min);
            spinbox->setMaximum(max);
            spinbox->blockSignals(false);
            spinbox->setValue(value);
        }
        else
        {
            qDebug() << "Spinbox pointer wrong";
        }
    }
}

void MainWindow::addFunctionParameter(ComplexType type, QBoxLayout * layout, QString valueName, double value, double min, double max)
{
    QHBoxLayout * hLayout = new QHBoxLayout();
    QDoubleSpinBox * spinbox = new QDoubleSpinBox();
    QLabel * label = new QLabel();
    label->setText(valueName);
    spinbox->setProperty("previous", spinbox->value());
    spinbox->setValue(value);
    spinbox->setMinimum(min);
    spinbox->setMaximum(max);
    spinbox->setProperty("valueName", valueName);
    spinbox->setProperty("type", static_cast<int>(type));

    switch (type){
    case Amplitude:
        mapAmplitudeParameterWidgets[valueName] = spinbox;
        break;
    case Phase:
        mapPhaseParameterWidgets[valueName] = spinbox;
        break;
    }


    connect(spinbox, SIGNAL(valueChanged(double)), this, SLOT(OnFunctionParameterChanged(double)), Qt::UniqueConnection);

    hLayout->addWidget(label, 1);
    hLayout->addWidget(spinbox, 3);

    layout->addLayout(hLayout);

}

void MainWindow::OnFunctionParameterChanged(double value)
{
    // get spinbox
    QDoubleSpinBox *spinbox = qobject_cast<QDoubleSpinBox *>(QObject::sender());
    // if data is valid
    if (spinbox) {
        // get parameter name: valueName
        QString valueName = spinbox->property("valueName").toString();

        // determine type: amplitude or phase
        ComplexType type = static_cast<ComplexType>(spinbox->property("type").toInt());

        // get corresponding combobox index = function index
        // get list of parameters at iList
        int iList;
        QList<QString> parameterList;
        switch (type){
        case Amplitude:
            iList = ui->comboBoxFunctionsAmplitude->currentData().toInt();
            parameterList = functionListAmplitude.at(iList)->parameterList();
            break;
        case Phase:
            iList = ui->comboBoxFunctionsPhase->currentData().toInt();
            parameterList = functionListPhase.at(iList)->parameterList();
            break;
        }

        // apply parameter to function
        if ( parameterList.contains(valueName) ) {
            switch (type){
            case Amplitude:
                functionListAmplitude.at(iList)->setParameter(valueName, value); break;
            case Phase:
                functionListPhase.at(iList)->setParameter(valueName, value); break;
            }
        }
    }
}

void MainWindow::getResultsAndPlot()
{
    qDebug() << "time elapsed for fft operations: " << calculationManager->speed() << endl;
    ui->labelTimeElapsed->setText(QString("%1 ms").arg(calculationManager->speed()*1000));

    // if nothing is selected do not correct
    fftAction doShift = fftAction::noshift;
    // otherwise according to selected shift
    if (static_cast<bool>(ui->checkBoxCorrectShiftDisplay->isChecked()))
    {
         doShift = static_cast<fftAction>(calculationManager->shiftBeforeFFT());
    }

    updatePlotData(plotList[0], calculationManager->data(doShift) );
    updatePlotData(plotList[1], calculationManager->dataFwdFourier(doShift) );
    updatePlotData(plotList[2], calculationManager->dataInvFourier(doShift) );



    const std::vector<complex<double>,fftalloc<complex<double> > > inv = calculationManager->dataInvFourier(doShift);
    const std::vector<complex<double>,fftalloc<complex<double> > > orig = calculationManager->data(doShift);

    size_t N = orig.size();
    vector<double> dataAmplitude(N);
    vector<double> dataPhase(N);

    for(std::vector<double>::size_type i = 0; i != N; i++) {
        dataAmplitude[i] = abs(inv[i]) - abs(orig[i]);
        dataPhase[i]     = arg(inv[i]) - arg(orig[i]);
    }

    updatePlotData(plotList[3], dataAmplitude, dataPhase);

//    updatePlotData(plotList[3], calculationManager->dataCompare(doShift) );
}

void MainWindow::on_checkBoxPhaseShift_stateChanged(int state)
{
    const bool checkState = static_cast<bool>(state);
    calculationManager->setShiftBeforeFFT(checkState);
    startFFT();
}

void MainWindow::on_checkBoxCorrectShiftDisplay_stateChanged(int state)
{
    Q_UNUSED(state);
    // update all plots
    getResultsAndPlot();
}


void MainWindow::on_checkBoxPhaseUnwrap_stateChanged(int state)
{
    Q_UNUSED(state);
    // update plot only
    fftAction doShift = static_cast<fftAction>(calculationManager->shiftBeforeFFT());
    updatePlotData(plotList[2], calculationManager->dataInvFourier(doShift) );
}

void MainWindow::on_pushButtonStartFFT_clicked()
{
    startFFT();
}

void MainWindow::startFFT()
{
    // TODO: Add mutex

    const int N = ui->comboBoxDataPoints->currentData().toInt();

    const int iterations = ui->spinBoxIterations->value();

    QSharedPointer<FunctionFactory> functionAmplitude =
            QSharedPointer<FunctionFactory>(this->functionListAmplitude.at(ui->comboBoxFunctionsAmplitude->currentIndex()));

    QSharedPointer<FunctionFactory> functionPhase =
            QSharedPointer<FunctionFactory>(this->functionListPhase.at(ui->comboBoxFunctionsPhase->currentIndex()));

    complexFF->setAmplitudeFunction(functionAmplitude);
    complexFF->setPhaseFunction(functionPhase);

    calculationManager->setIterations(iterations);
    calculationManager->setData(complexFF->complexData(N);
    calculationManager->moveToThread(thread);

    //connect(calculationManager, SIGNAL (error(QString)), this, SLOT (errorString(QString)));
    //connect(calculationManager, SIGNAL (finished()), calculationManager, SLOT (deleteLater()));
    //connect(thread, SIGNAL (finished()), thread, SLOT (deleteLater()));

    cout << "FFT thread started." << endl;
    thread->start();

}



void MainWindow::on_comboBoxDataPoints_currentIndexChanged(int index)
{
    updateFunctionParameterSettings();
}

void MainWindow::updateProgressBar(int percent)
{
    ui->progressBar->setValue(percent);
}
