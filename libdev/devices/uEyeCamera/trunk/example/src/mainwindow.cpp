#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtCore/QDebug>
#include <QtCore/QTime>
#include <QFileDialog>
#include <QMessageBox>

#include "QuEyeCamera.h"
#include "dialogcamerasettings.h"

//#include "..\..\src\build\moc\moc_QCameraImage.cpp"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindowClass)
    , camera(new QuEyeCamera)
    , timer(0)
{
    ui->setupUi(this);
    connectSignalSlots();
    initValues();    
    camera->setWindowHandle(ui->widgetCameraImage->windowHandle());
    camera->setWidgetVideoWindow(ui->widgetCameraImage);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete camera;
    delete timer;
}

void MainWindow::connectSignalSlots()
{
    connect(ui->actionQuit, SIGNAL(triggered()), this, SLOT(OnActionQuit()));
    connect(ui->actionCameraSettings, SIGNAL(triggered()), this, SLOT(OnActionCameraSettings()));
    connect(camera, SIGNAL(errorOccured(QString)), ui->labelMessage, SLOT(setText(const QString & )));
}

void MainWindow::OnActionQuit()
{
    close();
}

void MainWindow::initValues()
{
    ui->spinBoxX->setRange(0,10000);
    ui->spinBoxY->setRange(0,10000);
}

void MainWindow::OnActionCameraSettings()
{    
    if (!dlgCameraSettings) {
        dlgCameraSettings = new DialogCameraSettings(this);
        dlgCameraSettings->setCamera(camera);
    }

    dlgCameraSettings->show();
    dlgCameraSettings->activateWindow();
}
3

void MainWindow::on_pushButtonGetImage_clicked()
{
    QSize size = ui->widgetCameraImage->size();
    ui->widgetCameraImage->setViewSize(size.width(), size.height());
    //camera->setWindowHandle(ui->widgetCameraImage->windowHandle());
    camera->takeSnapshot();
    ui->widgetCameraImage->setCross(ui->spinBoxX->value(), ui->spinBoxY->value());
    ui->widgetCameraImage->setImage(camera->image());

}

void MainWindow::on_pushButtonSaveImage_clicked()
{
    if (!camera->isConnected()) {
        QMessageBox::warning (this, "Camera Error", "Camera not connected");
        return;
    }

    static QString startDirectory = "";

    QFileDialog::Options options;

    QString filters;
    filters =  "Image File (*.png);;" ;
    filters += "Any files (*.*)";

    QString selectedFilter;    

    QString fileName = QFileDialog::getSaveFileName(
            this,			// parent widget
            tr("Select file to save"),	//Caption
            startDirectory,             // Dir
            filters,                    // Filter
            &selectedFilter,            // returns selected filter
            options);			// possible options

    if (!fileName.isEmpty())
    {
        startDirectory = fileName;
        camera->image().save(fileName, "png");
    }

}

void MainWindow::on_pushButtonStartVideo_clicked()
{
//    connect(camera, SIGNAL(snapShotFinished()), this, SLOT(OnNewSnapShotImage()));
//    camera->takeSnapshot();
    //camera->startVideo();
    delete timer;
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(on_pushButtonGetImage_clicked()));
    timer->start(350);
}

void MainWindow::on_pushButtonStopVideo_clicked()
{
    //camera->disconnect(SIGNAL(snapShotFinished()));
    //camera->stopVideo();
    if (timer) {
        timer->stop();
    }
}

void MainWindow::OnNewSnapShotImage()
{
    ui->widgetCameraImage->setImage(camera->image());
    camera->takeSnapshot();
}
