#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "camera/cameraximea.h"
#include "camera/QCameraImage/QCameraImageBase.h"

#include <QScopedPointer>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    camera.reset(new CameraXimea());
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
        camera->openCamera();
        camera->capture();
        ui->widgetCameraImage->setImage(camera->image());
        //camera->closeCamera();
    }

}
