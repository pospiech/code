#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "camera/cameraximea.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::testImage()
{
    CameraXimea * camera = new CameraXimea();
    camera->Initialize();
    camera->openCamera();
    camera->capture();
    camera->closeCamera();

}

