#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "camera/cameraximea.h"
#include "camera/cameraueye.h"
//#include "camera/QCameraImage/QCameraImageBase.h"

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
    // camera.reset(new CameraXimea());
    camera.reset(new CameraUEye());

    SignalAppender* signalAppender = new SignalAppender;
    signalAppender->setFormat("[%{type:-7}] %{time}{dd-MM-yyyy, HH:mm}; Thread: %{threadid}; <%{Function}>; %{message}\n");
    signalAppender->setDetailsLevel(Logger::Trace);
    Logger::globalInstance()->registerAppender(signalAppender);

    QObject::connect(signalAppender, &SignalAppender::logMessage,
                      ui->widgetMessageList, &WidgetMessageList::addString);

   resize(QGuiApplication::primaryScreen()->availableSize() * 3 / 5);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::testImage()
{

}

void MainWindow::on_pushButtonTakeImage_clicked()
{    
    bool isReady = camera->initialize();
    if (isReady){
        ui->widgetMessageList->addString("Camera opened");
        camera->openCamera();
        camera->setExposure(1);
        camera->capture();
        LOG_INFO() << " set image to widget";
        ui->widgetCameraImage->setImage(camera->image());
        LOG_INFO() << " set data to histogram";
        ui->widgetHistogram->setData(camera->histogram());
        camera->closeCamera();
        LOG_INFO() << "finished take Image";
    }

}

void MainWindow::on_actionClose_triggered()
{
    QCoreApplication::quit();
}
