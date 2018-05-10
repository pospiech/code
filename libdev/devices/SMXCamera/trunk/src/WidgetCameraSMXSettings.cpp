#include "WidgetCameraSMXSettings.h"
#include "ui_WidgetCameraSMXSettings.h"

#include "QCameraSMX150.h"

WidgetCameraSMXSettings::WidgetCameraSMXSettings(QWidget* parent /*= 0*/, Qt::WFlags flags /*= 0*/)
    : QWidget(parent, flags)
    , ui(new Ui::WidgetCameraSMXSettings)    
    , camera(0)
    , blockAllSignals(true)
{
        ui->setupUi(this);
        initWidgets();
}


WidgetCameraSMXSettings::~WidgetCameraSMXSettings()
{
    delete ui;
    // delete camera;
}

void WidgetCameraSMXSettings::initWidgets()
{
    ui->comboBoxShutterMode->addItem("Global Shutter", QCameraSMX::GlobalShutter);
    ui->comboBoxShutterMode->addItem("Rolling Shutter", QCameraSMX::RollingShutter);

    ui->comboBoxSnapshotMultiplier->addItem("1x", 0);
    ui->comboBoxSnapshotMultiplier->addItem("2x", 1);
    ui->comboBoxSnapshotMultiplier->addItem("4x", 2);
    ui->comboBoxSnapshotMultiplier->addItem("8x", 3);

    ui->comboBoxFrequency->addItem("48", 48);
    ui->comboBoxFrequency->addItem("40", 40);
    ui->comboBoxFrequency->addItem("24", 24);
    ui->comboBoxFrequency->addItem("20", 20);
    ui->comboBoxFrequency->addItem("16", 16);
    ui->comboBoxFrequency->addItem("13.33", 13);
    ui->comboBoxFrequency->addItem("12", 12);
    ui->comboBoxFrequency->addItem("10", 10);
    ui->comboBoxFrequency->addItem("8", 8);
    ui->comboBoxFrequency->addItem("6.66", 6);
    ui->comboBoxFrequency->addItem("3.33",3);

    ui->comboBoxDecimation->addItem("1x", 1);
    ui->comboBoxDecimation->addItem("2x", 2);
    ui->comboBoxDecimation->addItem("4x", 4);

    ui->comboBoxColorDepth->addItem("8 Bit (256)", 8);
    ui->comboBoxColorDepth->addItem("24 Bit (16.7 M)", 24);

    ui->comboBoxBitDepth->addItem("8 Bit (256)", 8);
    ui->comboBoxBitDepth->addItem("10 Bit (1024)", 10);

    ui->comboBoxBayerAlgorithm->addItem("SimpleMonochrome", QCameraSMX::SimpleMonochrome);
    ui->comboBoxBayerAlgorithm->addItem("NearestNeighbor", QCameraSMX::NearestNeighbor);
    ui->comboBoxBayerAlgorithm->addItem("Bilinear", QCameraSMX::Bilinear);
    ui->comboBoxBayerAlgorithm->addItem("Laplacian", QCameraSMX::Laplacian);
    ui->comboBoxBayerAlgorithm->addItem("RealMonochrome", QCameraSMX::RealMonochrome);
    ui->comboBoxBayerAlgorithm->addItem("BayerAverage", QCameraSMX::BayerAverage);

    ui->checkBoxFixedRGBGainValues->setChecked(true);

    ui->spinBoxBrightness->setRange(-127, 127);
    ui->spinBoxContrast->setRange(-127, 127);
    ui->spinBoxGamma->setRange(-127, 127);

    ui->spinBoxSensorWidth->setRange(0, 1e6);
    ui->spinBoxSensorHeight->setRange(0, 1e6);

    ui->comboBoxMirrorX->addItem("on", true);
    ui->comboBoxMirrorX->addItem("off", false);

    ui->comboBoxMirrorY->addItem("on", true);
    ui->comboBoxMirrorY->addItem("off", false);

    ui->spinBoxDACOffsetRaw->setRange(0,127);
    ui->spinBoxDACOffsetFine->setRange(0,127);

    if (!camera) {
        ui->tabWidget->setEnabled(false);
    } else {
        ui->tabWidget->setEnabled(camera->isConnected());
    }
}

void WidgetCameraSMXSettings::setCamera(QCameraSMX * parentCamera)
{
    camera = parentCamera;
    connect(camera, SIGNAL(connectionStatusChanged()), this, SLOT(OnCameraConnectionChanged()));
    OnCameraConnectionChanged();
}

void WidgetCameraSMXSettings::updateWidgetsWithCameraInfo()
{
    if (!camera->isConnected())
        return;

    ui->comboBoxShutterMode->setCurrentIndex(static_cast<int>(camera->shutterMode()));

    ui->comboBoxSnapshotMultiplier->setCurrentIndex(
            ui->comboBoxSnapshotMultiplier->findData(camera->snapshotExposureMultiplier()));


    ui->comboBoxFrequency->setCurrentIndex(
            ui->comboBoxFrequency->findData(int(camera->frequency())));

    ui->comboBoxDecimation->setCurrentIndex(
            ui->comboBoxDecimation->findData(camera->decimation()));

    ui->comboBoxColorDepth->setCurrentIndex(
            ui->comboBoxColorDepth->findData(camera->colorDepth()));

    ui->comboBoxBitDepth->setCurrentIndex(
            ui->comboBoxBitDepth->findData(camera->bitDepth()));

    ui->comboBoxBayerAlgorithm->setCurrentIndex(
            ui->comboBoxBayerAlgorithm->findData(int(camera->bayerAlgorithm())));

    ui->spinBoxBrightness->setValue(camera->brightness());
    ui->spinBoxContrast->setValue(camera->constrast());
    ui->spinBoxGamma->setValue(camera->gamma());


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

    ui->comboBoxMirrorX->setCurrentIndex(
            ui->comboBoxMirrorX->findData(int(camera->mirrorVertical())));

    ui->comboBoxMirrorY->setCurrentIndex(
            ui->comboBoxMirrorY->findData(int(camera->mirrorHorizontal())));

    ui->doubleSpinBoxExposure->setRange(camera->exposureLimits().min, camera->exposureLimits().max);

    ui->doubleSpinBoxGainMono->setRange(camera->gainLimits().min, camera->gainLimits().max);
    ui->doubleSpinBoxGainRed->setRange(camera->gainLimits().min, camera->gainLimits().max);
    ui->doubleSpinBoxGainGreen->setRange(camera->gainLimits().min, camera->gainLimits().max);
    ui->doubleSpinBoxGainBlue->setRange(camera->gainLimits().min, camera->gainLimits().max);

    ui->labelDevicename->setText(camera->deviceName());

    switch (camera->sensorType())
    {
    case QCameraSMX::Monochrome:
        ui->labelSensorType->setText("Monochrome");
        break;
    case QCameraSMX::Color:
    default:
        ui->labelSensorType->setText("Color");
    }

    ui->labelHardwareID->setText(QString("%1").arg(camera->modelID()));
    ui->labelVersion->setText(QString("%1").arg(camera->version()));
    ui->labelSerial->setText(QString("%1").arg(camera->serial()));

    ui->spinBoxDACOffsetRaw->setValue(camera->DACOffsetRaw());
    ui->spinBoxDACOffsetFine->setValue(camera->DACOffsetFine());

    updatePixelNumber();
}

void WidgetCameraSMXSettings::updatePixelNumber()
{
    int pixels = camera->viewPort().width() * camera->viewPort().height();
    if (pixels > 1e6) {
        double megapixel = double(pixels) / 1.0e6;
        ui->labelMegapixel->setText(QString("%1 Mp").arg(megapixel, 0,'f', 2));
    } else {
        ui->labelMegapixel->setText(QString("%1 Pixel").arg(pixels));
    }
}


void WidgetCameraSMXSettings::OnCameraConnectionChanged()
{
    if (!camera) return;
    if (camera->isConnected()) {
        // camera->initialize();
        updateWidgetsStatusIfConnected();
        updateWidgetsWithCameraInfo();
        blockAllSignals = false;
    }
}

void WidgetCameraSMXSettings::updateWidgetsStatusIfConnected()
{
    if (!camera) return;
    ui->tabWidget->setEnabled(camera->isConnected());
}

void WidgetCameraSMXSettings::on_comboBoxShutterMode_currentIndexChanged(int index)
{
    if (!camera) return;
    if (blockAllSignals ) return;

    camera->setShutterMode(
            static_cast<QCameraSMX::ShutterMode>(
                    ui->comboBoxShutterMode->itemData(index).toInt()));
}

void WidgetCameraSMXSettings::on_doubleSpinBoxExposure_valueChanged(double value)
{
    if (!camera) return;
    if (blockAllSignals ) return;

    double exposure = 0;
    switch (camera->shutterMode())
    {
    case QCameraSMX::GlobalShutter:
        exposure = camera->setSnapshotExposure(value);
        break;
    case QCameraSMX::RollingShutter:
        exposure = camera->setExposure(value);
        break;
    }
    ui->doubleSpinBoxExposure->blockSignals(true);
    ui->doubleSpinBoxExposure->setValue(exposure);
    ui->doubleSpinBoxExposure->blockSignals(false);
}

void WidgetCameraSMXSettings::on_comboBoxSnapshotMultiplier_currentIndexChanged(int index)
{
    if (!camera) return;
    if (blockAllSignals ) return;

    camera->setSnapshotExposureMultiplier(ui->comboBoxSnapshotMultiplier->itemData(index).toInt());
}

void WidgetCameraSMXSettings::on_comboBoxFrequency_currentIndexChanged(int index)
{
    if (!camera) return;
    if (blockAllSignals ) return;

    double frequency = ui->comboBoxFrequency->itemData(index).toDouble();
    camera->setFrequency(frequency);
    QCameraSMX::Limits limits;
    switch (camera->shutterMode())
    {
    case QCameraSMX::GlobalShutter:
        limits = camera->snapshotExposureLimits();
        break;
    case QCameraSMX::RollingShutter:
        limits = camera->exposureLimits();
        break;
    }
    ui->doubleSpinBoxExposure->setRange(limits.min, limits.max);
}

void WidgetCameraSMXSettings::on_comboBoxDecimation_currentIndexChanged(int index)
{
    if (!camera) return;
    if (blockAllSignals ) return;

    camera->setDecimation(ui->comboBoxDecimation->itemData(index).toInt());
}

void WidgetCameraSMXSettings::on_comboBoxColorDepth_currentIndexChanged(int index)
{
    if (!camera) return;
    if (blockAllSignals ) return;

    camera->setColorDepth(ui->comboBoxColorDepth->itemData(index).toInt());
}

void WidgetCameraSMXSettings::on_comboBoxBitDepth_currentIndexChanged(int index)
{
    if (!camera) return;
    if (blockAllSignals ) return;

    int bits = ui->comboBoxBitDepth->itemData(index).toInt();
    camera->setBitDepth(bits);
}

void WidgetCameraSMXSettings::on_comboBoxBayerAlgorithm_currentIndexChanged(int index)
{
    if (!camera) return;
    if (blockAllSignals ) return;

    camera->setBayerAlgorithm(
            static_cast<QCameraSMX::BayerAlgorithm>(
                ui->comboBoxBayerAlgorithm->itemData(index).toInt()));
}

void WidgetCameraSMXSettings::on_doubleSpinBoxGainMono_valueChanged(double value)
{
    if (!camera) return;
    if (blockAllSignals ) return;

    camera->setGain(value);
}

void WidgetCameraSMXSettings::on_checkBoxFixedRGBGainValues_stateChanged(int value)
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



void WidgetCameraSMXSettings::on_doubleSpinBoxGainRed_valueChanged(double value)
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

void WidgetCameraSMXSettings::on_doubleSpinBoxGainGreen_valueChanged(double value)
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

void WidgetCameraSMXSettings::on_doubleSpinBoxGainBlue_valueChanged(double value)
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

void WidgetCameraSMXSettings::on_spinBoxBrightness_valueChanged(int value)
{
    if (!camera) return;
    if (blockAllSignals ) return;

    camera->setBrightness(value);
}

void WidgetCameraSMXSettings::on_spinBoxContrast_valueChanged(int value)
{
    if (!camera) return;
    if (blockAllSignals ) return;

    camera->setConstrast(value);
}

void WidgetCameraSMXSettings::on_spinBoxGamma_valueChanged(int value)
{
    if (!camera) return;
    if (blockAllSignals ) return;

    camera->setGamma(value);
}

void WidgetCameraSMXSettings::on_spinBoxViewPortLeft_valueChanged(int value)
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

void WidgetCameraSMXSettings::on_spinBoxViewPortTop_valueChanged(int value)
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

void WidgetCameraSMXSettings::on_spinBoxViewPortHeight_valueChanged(int value)
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

void WidgetCameraSMXSettings::on_spinBoxViewPortWidth_valueChanged(int value)
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

void WidgetCameraSMXSettings::on_comboBoxMirrorX_currentIndexChanged(int index)
{
    if (!camera) return;
    if (blockAllSignals ) return;

    camera->setMirrorVertical(ui->comboBoxMirrorX->itemData(index).toBool());
}

void WidgetCameraSMXSettings::on_comboBoxMirrorY_currentIndexChanged(int index)
{
    if (!camera) return;
    if (blockAllSignals ) return;

    camera->setMirrorHorizontal(ui->comboBoxMirrorY->itemData(index).toBool());
}

void WidgetCameraSMXSettings::on_spinBoxDACOffsetRaw_valueChanged(int value)
{
    if (blockAllSignals ) return;

    camera->setDACOffsetRaw(value);
}

void WidgetCameraSMXSettings::on_spinBoxDACOffsetFine_valueChanged(int value)
{
    if (blockAllSignals ) return;

    camera->setDACOffsetFine(value);
}
