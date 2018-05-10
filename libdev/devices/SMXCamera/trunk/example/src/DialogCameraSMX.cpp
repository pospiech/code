#include "DialogCameraSMX.h"
#include <QFileDialog>
#include "globaldevices.h"
#include "QCameraSMX150.h"

DialogCameraSMX::DialogCameraSMX(QWidget* parent /*= 0*/, Qt::WFlags flags /*= 0*/) //: QWidget(parent, flags)
    : QDialog(parent, flags)
    , ui(new Ui::DialogCameraSMX)
{	
    ui->setupUi(this);
    connectSignalSlots();
    initWidgets();

    if (!cameraSMX) {
        cameraSMX = new QCameraSMX();
        connect(cameraSMX, SIGNAL(errorOccured(QString)), this, SLOT(OnCameraError(QString)));
    }
    ui->widgetCameraSettings->setCamera(cameraSMX);
    cameraSMX->setWidgetVideoWindow(ui->widgetCameraImage);
}


DialogCameraSMX::~DialogCameraSMX()
{
    delete cameraSMX;
    delete ui;    
}

void DialogCameraSMX::connectSignalSlots()
{
    connect(ui->pushButtonClose, SIGNAL(clicked()), this, SLOT(OnButtonClose()));
    connect(ui->pushButtonCapture, SIGNAL(clicked()), this, SLOT(OnButtonCapture()));
    connect(ui->pushButtonSaveData, SIGNAL(clicked()), this, SLOT(OnButtonSaveData()));
    connect(ui->pushButtonSavePicture, SIGNAL(clicked()), this, SLOT(OnButtonSavePicture()));
    connect(ui->pushButtonConnectCamera, SIGNAL(clicked()), this, SLOT(OnButtonConnect()));
}

void DialogCameraSMX::initWidgets()
{
    ui->widgetCameraImage->setAutoResize(true);
}

void DialogCameraSMX::closeEvent ( QCloseEvent * event )
{

}

void DialogCameraSMX::OnCameraError(QString error)
{
    ui->labelCameraMessage->setText(error);
}

void DialogCameraSMX::OnButtonClose()
{
    close();
}

void DialogCameraSMX::OnButtonConnect()
{
    cameraSMX->initialize();
}

void DialogCameraSMX::OnButtonCapture()
{
    if (cameraSMX->isInitialized())
    {
        cameraSMX->stopVideo();
        cameraSMX->takeSnapshot();
        ui->widgetCameraImage->setImage(cameraSMX->image());
    }
}

void DialogCameraSMX::OnButtonSaveData()
{
    QFileDialog::Options options;

    QString filters;
    filters =  "Image Data File (*.dat);;" ;
    filters += "Any files (*.*)";

    QString selectedFilter;

    QString fileName = QFileDialog::getSaveFileName(
            this,						// parent widget
            tr("Select file to save"),	//Cpation
            "", // Dir
            filters,					// Filter
            &selectedFilter,			// returns selected filter
            options);					// possible options
    
    if (!fileName.isEmpty())
    {
        // cameraSMX->SaveFrameToFile(fileName.toStdString());
    }
}

void DialogCameraSMX::OnButtonSavePicture()
{
    QFileDialog::Options options;

    QString filters;
    filters =  "Image File (*.png);;" ;
    filters += "Any files (*.*)";

    QString selectedFilter;

    QString fileName = QFileDialog::getSaveFileName(
            this,						// parent widget
            tr("Select file to save"),	//Cpation
            "", // Dir
            filters,					// Filter
            &selectedFilter,			// returns selected filter
            options);					// possible options
    
    if (!fileName.isEmpty())
    {
        ui->widgetCameraImage->saveImage(fileName);
    }

}

void DialogCameraSMX::on_pushButtonStartVideo_clicked()
{
    cameraSMX->startVideo();
}

void DialogCameraSMX::on_pushButtonStopVideo_clicked()
{
    cameraSMX->stopVideo();
}
