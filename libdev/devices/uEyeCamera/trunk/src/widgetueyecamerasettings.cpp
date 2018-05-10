#include "widgetueyecamerasettings.h"
#include "ui_widgetueyecamerasettings.h"
#include "QuEyeCamera.h"

WidgetUEyeCameraSettings::WidgetUEyeCameraSettings(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::WidgetUEyeCameraSettings)
    , camera(0)
    , blockAllSignals(true)
{
        ui->setupUi(this);
        initWidgets();
}

WidgetUEyeCameraSettings::~WidgetUEyeCameraSettings()
{
    delete ui;
}

void WidgetUEyeCameraSettings::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void WidgetUEyeCameraSettings::initWidgets()
{

    ui->comboBoxShutterMode->addItem("Global Shutter", QuEyeCamera::GlobalShutter);
    ui->comboBoxShutterMode->addItem("Rolling Shutter", QuEyeCamera::RollingShutter);


    ui->comboBoxColorMode->addItem("RGB32 [r(8) g(8) b(8) x(8)] (Qt)", QuEyeCamera::RGB32);
    ui->comboBoxColorMode->addItem("RGB24 [r(8) g(8) b(8) | r(8)] (Qt)", QuEyeCamera::RGB24);
    ui->comboBoxColorMode->addItem("BGR32 [b(8) g(8) r(8) x(8)] ", QuEyeCamera::BGR32);
    ui->comboBoxColorMode->addItem("BGR24 [b(8) g(8) r(8) | r(8)] ", QuEyeCamera::BGR24);
    ui->comboBoxColorMode->addItem("BGR16 [b(5) g(6) r(5) | b(5) g(6) r(5)]", QuEyeCamera::BGR16);
    ui->comboBoxColorMode->addItem("BGR15 [b(5) g(5) r(5) x(1) | b(5) g(5) r(5) x(1)]", QuEyeCamera::BGR15);
    ui->comboBoxColorMode->addItem("MONO8 [8 | 8 | 8 | 8]", QuEyeCamera::MONO8);

    ui->comboBoxDecimation->addItem("1x", 1);
    ui->comboBoxDecimation->addItem("2x", 2);
    ui->comboBoxDecimation->addItem("4x", 4);
    ui->comboBoxDecimation->addItem("6x", 6);

    ui->checkBoxFixedRGBGainValues->setChecked(true);

    ui->doubleSpinBoxGamma->setRange(0.01, 10);

    ui->spinBoxSensorWidth->setRange(0, 1e6);
    ui->spinBoxSensorHeight->setRange(0, 1e6);

    ui->doubleSpinBoxAutoBrightness->setRange(0,255);

    if (!camera) {
        ui->tabWidget->setEnabled(false);
    } else {
        ui->tabWidget->setEnabled(camera->isConnected());
    }
}

void WidgetUEyeCameraSettings::updateWidgetsWithCameraInfo()
{
    if (!camera->isConnected())
        return;


    ui->comboBoxColorMode->setCurrentIndex(
            ui->comboBoxColorMode->findData(camera->colorMode()));

    if (camera->isGlobalShutterSupported()) {
        ui->comboBoxShutterMode->setCurrentIndex(
                ui->comboBoxShutterMode->findData(camera->shutterMode()));
    } else {
        ui->comboBoxShutterMode->setCurrentIndex(
                ui->comboBoxShutterMode->findData(QuEyeCamera::RollingShutter));
        ui->comboBoxShutterMode->setEnabled(false);
    }

    QuEyeCamera::Limits limits = camera->exposureRange();
    ui->doubleSpinBoxExposure->setRange(limits.min, limits.max);
    ui->doubleSpinBoxExposure->setSingleStep(limits.intervall);
    ui->doubleSpinBoxExposure->setValue(camera->exposure());

    ui->doubleSpinBoxFrequency->setRange(camera->pixelClockRange().min, camera->pixelClockRange().max);
    ui->doubleSpinBoxFrequency->setSingleStep(camera->pixelClockRange().intervall);
    ui->doubleSpinBoxFrequency->setValue(camera->pixelClock());

    ui->doubleSpinBoxGainMono->setRange(camera->gainLimits().min, camera->gainLimits().max);
    ui->doubleSpinBoxGainRed->setRange(camera->gainLimits().min, camera->gainLimits().max);
    ui->doubleSpinBoxGainGreen->setRange(camera->gainLimits().min, camera->gainLimits().max);
    ui->doubleSpinBoxGainBlue->setRange(camera->gainLimits().min, camera->gainLimits().max);

    ui->checkBoxGainBoost->setChecked(camera->isGainBoostEnabled());

    ui->comboBoxDecimation->setCurrentIndex(
            ui->comboBoxDecimation->findData(camera->binning()));

    ui->doubleSpinBoxGamma->setValue(camera->gamma()/100);

    ui->spinBoxSensorWidth->setValue(camera->sensorSize().width());
    ui->spinBoxSensorHeight->setValue(camera->sensorSize().height());

    ui->spinBoxViewPortLeft->setRange(0, camera->sensorSize().width());
    ui->spinBoxViewPortWidth->setRange(1, camera->sensorSize().width());
    ui->spinBoxViewPortTop->setRange(0, camera->sensorSize().height());
    ui->spinBoxViewPortHeight->setRange(1, camera->sensorSize().height());

    ui->spinBoxViewPortLeft->setValue(camera->viewPort().left());
    ui->spinBoxViewPortWidth->setValue(camera->viewPort().width());
    ui->spinBoxViewPortTop->setValue(camera->viewPort().top());
    ui->spinBoxViewPortHeight->setValue(camera->viewPort().height());

    ui->checkBoxWhiteBalance->setChecked(camera->whiteBalanceEnabled());

    ui->doubleSpinBoxAutoBrightness->setValue(camera->autoBrightness());

    ui->labelDevicename->setText(camera->deviceName());

    ui->doubleSpinBoxAutoBrightness->setValue(camera->autoBrightness());

    switch (camera->sensorType())
    {
    case QuEyeCamera::Monochrome:
        ui->labelSensorType->setText("Monochrome");
        break;
    case QuEyeCamera::Color:
    default:
        ui->labelSensorType->setText("Color");
    }

    ui->labelSerial->setText(QString("%1").arg(camera->serial()));

    updatePixelNumber();
}

void WidgetUEyeCameraSettings::updatePixelNumber()
{
    int pixels = camera->viewPort().width() * camera->viewPort().height();
    if (pixels > 1e6) {
        double megapixel = double(pixels) / 1.0e6;
        ui->labelMegapixel->setText(QString("%1 Mp").arg(megapixel, 0,'f', 2));
    } else {
        ui->labelMegapixel->setText(QString("%1 Pixel").arg(pixels));
    }
}

void WidgetUEyeCameraSettings::OnCameraConnectionChanged()
{
    if (!camera) return;
    if (camera->isConnected()) {
        updateWidgetsStatusIfConnected();
        updateWidgetsWithCameraInfo();
        blockAllSignals = false;
    }
}

void WidgetUEyeCameraSettings::updateWidgetsStatusIfConnected()
{
    if (!camera) return;
    ui->tabWidget->setEnabled(camera->isConnected());
}

void WidgetUEyeCameraSettings::setCamera(QuEyeCamera *parentCamera)
{
    camera = parentCamera;
    connect(camera, SIGNAL(connectionStatusChanged()), this, SLOT(OnCameraConnectionChanged()));
    OnCameraConnectionChanged();
}

void WidgetUEyeCameraSettings::on_comboBoxDecimation_currentIndexChanged(int index)
{
    if (!camera) return;
    if (blockAllSignals ) return;

    camera->setBinning(ui->comboBoxDecimation->itemData(index).toInt());

}

void WidgetUEyeCameraSettings::OnViewPortChanged()
{
    if (!camera) return;
    if (blockAllSignals ) return;

    QRect rect;
    rect.setLeft(ui->spinBoxViewPortLeft->value());
    rect.setTop(ui->spinBoxViewPortTop->value());
    rect.setWidth(ui->spinBoxViewPortWidth->value());
    rect.setHeight(ui->spinBoxViewPortHeight->value());
    camera->setViewPort(rect);
    updatePixelNumber();
}

void WidgetUEyeCameraSettings::on_spinBoxViewPortLeft_valueChanged(int )
{
    OnViewPortChanged();
}

void WidgetUEyeCameraSettings::on_spinBoxViewPortTop_valueChanged(int )
{
    OnViewPortChanged();
}

void WidgetUEyeCameraSettings::on_spinBoxViewPortHeight_valueChanged(int )
{
    OnViewPortChanged();
}

void WidgetUEyeCameraSettings::on_spinBoxViewPortWidth_valueChanged(int )
{
    OnViewPortChanged();
}

void WidgetUEyeCameraSettings::on_comboBoxShutterMode_currentIndexChanged(int index)
{
    if (!camera) return;
    if (blockAllSignals ) return;

    camera->setShutterMode(
            static_cast<QuEyeCamera::ShutterMode>(
                    ui->comboBoxShutterMode->itemData(index).toInt()));

}

void WidgetUEyeCameraSettings::on_doubleSpinBoxExposure_valueChanged(double value)
{
    if (!camera) return;
    if (blockAllSignals ) return;

    double exposure = camera->setExposure(value);

    ui->doubleSpinBoxExposure->blockSignals(true);
    ui->doubleSpinBoxExposure->setValue(exposure);
    ui->doubleSpinBoxExposure->blockSignals(false);
}

void WidgetUEyeCameraSettings::on_doubleSpinBoxAutoBrightness_valueChanged(double )
{
    if (!camera) return;
    if (blockAllSignals ) return;

    camera->setAutoBrightness(ui->doubleSpinBoxAutoBrightness->value());

}

void WidgetUEyeCameraSettings::on_doubleSpinBoxFrequency_valueChanged(double frequency)
{
    if (!camera) return;
    if (blockAllSignals ) return;

    camera->setPixelClock(int(frequency));
    QuEyeCamera::Limits limits;
    limits = camera->exposureRange();
    ui->doubleSpinBoxExposure->setRange(limits.min, limits.max);
}

void WidgetUEyeCameraSettings::on_comboBoxColorMode_currentIndexChanged(int index)
{
    if (!camera) return;
    if (blockAllSignals ) return;

    camera->setColorMode(
            static_cast<QuEyeCamera::ColorMode>(
                    ui->comboBoxColorMode->itemData(index).toInt()));

}

void WidgetUEyeCameraSettings::on_doubleSpinBoxGainMono_valueChanged(double value)
{
    if (!camera) return;
    if (blockAllSignals ) return;

    camera->setGain(value);

}

void WidgetUEyeCameraSettings::on_checkBoxFixedRGBGainValues_stateChanged(int value)
{
    if (!camera) return;
    if (blockAllSignals ) return;

    Qt::CheckState state = static_cast<Qt::CheckState>(value);
    if (state == Qt::Checked) {
        int r = ui->doubleSpinBoxGainRed->value();
        int g = ui->doubleSpinBoxGainGreen->value();
        int b = ui->doubleSpinBoxGainBlue->value();
        int m = std::max(std::max(r,g), b);
        ui->doubleSpinBoxGainRed->setValue(m);
        ui->doubleSpinBoxGainGreen->setValue(m);
        ui->doubleSpinBoxGainBlue->setValue(m);
    }
    else if (state == Qt::Unchecked){

    }

}

void WidgetUEyeCameraSettings::OnGainRGBChanged()
{
    if (!camera) return;
    if (blockAllSignals ) return;

    int r = ui->doubleSpinBoxGainRed->value();
    int g = ui->doubleSpinBoxGainGreen->value();
    int b = ui->doubleSpinBoxGainBlue->value();
    int m = std::max(std::max(r,g), b);
    if (ui->checkBoxFixedRGBGainValues->isChecked()) {
        camera->setGainRGB(m,m,m);
    } else {
        camera->setGainRGB(r,g,b);
    }
}

void WidgetUEyeCameraSettings::on_doubleSpinBoxGainRed_valueChanged(double )
{
    OnGainRGBChanged();
}

void WidgetUEyeCameraSettings::on_doubleSpinBoxGainGreen_valueChanged(double )
{
    OnGainRGBChanged();
}

void WidgetUEyeCameraSettings::on_doubleSpinBoxGainBlue_valueChanged(double )
{
    OnGainRGBChanged();
}

void WidgetUEyeCameraSettings::on_checkBoxGainBoost_stateChanged(int value)
{
    if (!camera) return;
    if (blockAllSignals ) return;

    Qt::CheckState state = static_cast<Qt::CheckState>(value);
    if (state == Qt::Checked) {
        camera->setGainBoostEnabled(true);
    } else {
        camera->setGainBoostEnabled(false);
    }
}

void WidgetUEyeCameraSettings::on_doubleSpinBoxGamma_valueChanged(double value)
{
    if (!camera) return;
    if (blockAllSignals ) return;

    camera->setGamma(value*100);
}

void WidgetUEyeCameraSettings::on_checkBoxWhiteBalance_stateChanged(int value)
{
    if (!camera) return;
    if (blockAllSignals ) return;

    Qt::CheckState state = static_cast<Qt::CheckState>(value);
    if (state == Qt::Checked) {
        camera->setWhiteBalanceEnabled(true);
    } else {
        camera->setWhiteBalanceEnabled(false);
    }

}
