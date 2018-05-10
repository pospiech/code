#include "dialogcamerasettings.h"
#include "ui_dialogcamerasettings.h"

DialogCameraSettings::DialogCameraSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogCameraSettings)
{
    ui->setupUi(this);
}

DialogCameraSettings::~DialogCameraSettings()
{
    delete ui;
}

void DialogCameraSettings::setCamera(QuEyeCamera *camera)
{
    ui->widgetSettings->setCamera(camera);
}

void DialogCameraSettings::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
