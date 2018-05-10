#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "DialogCameraSMX.h"
#include "globaldevices.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindowClass)
    , devices(new GlobalDevices)
{
    ui->setupUi(this);    
    connectSignalSlots();
}

MainWindow::~MainWindow()
{
    delete ui;
    //delete devices;
    // delete cameraSMX;
    // cameraSMX = 0;
}

void MainWindow::connectSignalSlots()
{
    connect(ui->actionQuit, SIGNAL(triggered()), this, SLOT(OnActionQuit()));
    connect(ui->actionCameraSMX, SIGNAL(triggered()), this, SLOT(OnCameraSMX()));
}

void MainWindow::OnActionQuit()
{
    close();
}

void MainWindow::OnCameraSMX()
{
//    if (!camera)
//    {
//        camera = new CameraSMX(this);
//        Camera->Initialize();
//    }
    //if (Camera->isConnected())
    //{
    if (!dlgCameraSMX)
        dlgCameraSMX = new DialogCameraSMX(this);

    dlgCameraSMX->show();
    dlgCameraSMX->activateWindow();
    //}
}

