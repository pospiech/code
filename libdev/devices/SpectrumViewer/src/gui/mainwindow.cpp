#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "camera/camerabasler.h"
#include "camera/cameraximea.h"
#include "camera/cameraueye.h"
#include "camera/camerasimulation.h"

#include <QScopedPointer>

#include <QtCharts>
using namespace QtCharts;

#include "Logger.h"
#include "signalappender.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    SignalAppender* signalAppender = new SignalAppender;
    signalAppender->setFormat("[%{type:-7}] %{time}{dd-MM-yyyy, HH:mm}; Thread: %{threadid}; <%{Function}>; %{message}");
    signalAppender->setDetailsLevel(Logger::Trace);
    Logger::globalInstance()->registerAppender(signalAppender);

    QObject::connect(signalAppender, &SignalAppender::logMessage,
                      ui->widgetMessageList, &WidgetMessageList::addString);

    resize(QGuiApplication::primaryScreen()->availableSize() * 4 / 5);

    // set widgets as if no camera is open
    on_pushButtonCameraClose_clicked();

    // find cameras and add to list
    findCameras();

    // set ratio for splitter
    QList<int> sizes({400, 200});
    ui->splitter->setSizes(sizes);
    ui->tabWidget->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;

    qDeleteAll(cameraList);
}



void MainWindow::findCameras()
{
    // test ueye Cameras
    CameraBasler * cameraBasler = new CameraBasler;
    bool isReady = cameraBasler->initialize();
    if (isReady)
        cameraList.append(cameraBasler);

    // test ueye Cameras
    CameraUEye * cameraUEye = new CameraUEye;
    isReady = cameraUEye->initialize();
    if (isReady)
        cameraList.append(cameraUEye);

    // test Ximea Cameras
    CameraXimea * cameraXimea = new CameraXimea;
    isReady = cameraXimea->initialize();
    if (isReady)
        cameraList.append(cameraXimea);

    // test Simulation Camera
    CameraSimulation * cameraSimulation = new CameraSimulation;
    isReady = cameraSimulation->initialize();
    if (isReady)
        cameraList.append(cameraSimulation);

    for (int i = 0; i < cameraList.size(); ++i)
    {
        ui->comboBoxCameraSelect->addItem(cameraList.at(i)->description(), i);
    }
    if (cameraList.size() > 0) {
        ui->comboBoxCameraSelect->setCurrentIndex(0);
    }

}

void MainWindow::on_pushButtonTakeImage_clicked()
{    
    int index = ui->comboBoxCameraSelect->currentData().toInt();
    CameraInterface * camera = cameraList.at(index);
    if (camera->isOpen()) {        
        camera->capture();
        LOG_INFO() << " set image to widget";
        ui->widgetCameraImage->setImage(camera->image());
        LOG_INFO() << " set data to histogram";
        ui->widgetHistogram->setData(camera->histogram());
        LOG_INFO() << "finished take Image";
    }
}

void MainWindow::on_actionClose_triggered()
{
    on_pushButtonCameraClose_clicked();
    QCoreApplication::quit();
}

void MainWindow::on_pushButtonCameraOpen_clicked()
{
    int index = ui->comboBoxCameraSelect->currentIndex();
    if (index >= 0) {
        CameraInterface * camera = cameraList.at(index);
        camera->openCamera();
//        camera->setExposure(2.0);
        ui->pushButtonCameraClose->setEnabled(true);
        ui->pushButtonCameraOpen->setEnabled(false);
        ui->pushButtonTakeImage->setEnabled(true);
        ui->tabWidget->setEnabled(true);
        ui->widgetCameraParameter->setCameraHandle(camera);
    }
}

void MainWindow::on_pushButtonCameraClose_clicked()
{
    int index = ui->comboBoxCameraSelect->currentIndex();
    if (index >= 0) {
        CameraInterface * camera = cameraList.at(index);
        camera->closeCamera();
    }
    ui->pushButtonCameraClose->setEnabled(false);
    ui->pushButtonCameraOpen->setEnabled(true);
    ui->pushButtonTakeImage->setEnabled(false);
    ui->tabWidget->setEnabled(false);
}
