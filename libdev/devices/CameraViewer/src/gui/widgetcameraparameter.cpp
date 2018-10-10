#include "widgetcameraparameter.h"
#include "widgetcameraparameter_gui.h"

#include <QGridLayout>

#include <QDebug>
#include "Logger.h"
#include <exception>
#include <cmath>


#include <QProxyStyle>

// see https://stackoverflow.com/questions/52296702/qt-slot-called-twice
class BugFixStyle : public QProxyStyle
{
public:
    int styleHint(StyleHint stylehint, const QStyleOption *opt, const QWidget *widget, QStyleHintReturn *returnData) const
    {
        if(stylehint == QStyle::SH_SpinBox_ClickAutoRepeatThreshold)
        {
            return 8000; //8 seconds threshold
        }
        return QProxyStyle::styleHint(stylehint, opt, widget, returnData);
    }
};

#include "camerainterface.h"

int exposureToSlider(double value)
{
    return static_cast<int>(log10(value)*100.0);
}

double sliderToExposure(int value)
{
    return pow(10,static_cast<double>(value)/100.0);
}


WidgetCameraParameter::WidgetCameraParameter(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::WidgetCameraParameter)    
{
    ui->setupUi(this);

    // value range
    m_SpinboxValueChanged = false;


    setExposureMinMax(1, 1000);
    setGainMinMax(1, 10);

    ui->doubleSpinBoxExposure->setStyle(new BugFixStyle());

    // connect
    connect(ui->doubleSpinBoxExposure, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
           this, &WidgetCameraParameter::on_doubleSpinBoxExposure_valueChanged);

    connect(ui->doubleSpinBoxGain, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
           this, &WidgetCameraParameter::on_doubleSpinBoxGain_valueChanged);

    connect(ui->horizontalSliderExposure, QOverload<int>::of(&QSlider::valueChanged),
           this, &WidgetCameraParameter::on_horizontalSliderExposure_valueChanged);

    connect(ui->horizontalSliderGain, QOverload<int>::of(&QSlider::valueChanged),
           this, &WidgetCameraParameter::on_horizontalSliderGain_valueChanged);

    connect(ui->spinBoxX, QOverload<int>::of(&QSpinBox::valueChanged),
           this, &WidgetCameraParameter::on_spinBoxX_valueChanged);

    connect(ui->spinBoxY, QOverload<int>::of(&QSpinBox::valueChanged),
           this, &WidgetCameraParameter::on_spinBoxY_valueChanged);

    connect(ui->spinBoxdX, QOverload<int>::of(&QSpinBox::valueChanged),
           this, &WidgetCameraParameter::on_spinBoxdX_valueChanged);

    connect(ui->spinBoxdY, QOverload<int>::of(&QSpinBox::valueChanged),
           this, &WidgetCameraParameter::on_spinBoxdY_valueChanged);


    connect(ui->spinBoxX, &QSpinBox::editingFinished,
           this, &WidgetCameraParameter::on_spinBoxX_editingFinished);

    connect(ui->spinBoxY, &QSpinBox::editingFinished,
           this, &WidgetCameraParameter::on_spinBoxY_editingFinished);

    connect(ui->spinBoxdX, &QSpinBox::editingFinished,
           this, &WidgetCameraParameter::on_spinBoxdX_editingFinished);

    connect(ui->spinBoxdY, &QSpinBox::editingFinished,
           this, &WidgetCameraParameter::on_spinBoxdY_editingFinished);

}


void WidgetCameraParameter::setExposureMinMax(double min, double max)
{
    ui->doubleSpinBoxExposure->setMinimum(min);
    ui->doubleSpinBoxExposure->setMaximum(max);
    ui->doubleSpinBoxExposure->setDecimals(4);
    ui->doubleSpinBoxExposure->setSuffix(" ms");
    ui->doubleSpinBoxExposure->setSingleStep(1.0);

    // 0.001 -> -300, 1000 -> 300
    const int minValue = exposureToSlider(ui->doubleSpinBoxExposure->minimum());
    const int maxValue = exposureToSlider(ui->doubleSpinBoxExposure->maximum());
    ui->horizontalSliderExposure->setMinimum(minValue);
    ui->horizontalSliderExposure->setMaximum(maxValue);
    ui->horizontalSliderExposure->setTickInterval(1);
}


void WidgetCameraParameter::setGainMinMax(double min, double max)
{
    ui->doubleSpinBoxGain->setMinimum(min);
    ui->doubleSpinBoxGain->setMaximum(max);
    ui->doubleSpinBoxGain->setSingleStep(0.1);
    ui->horizontalSliderGain->setMinimum(static_cast<int>(ui->doubleSpinBoxGain->minimum()*10));
    ui->horizontalSliderGain->setMaximum(static_cast<int>(ui->doubleSpinBoxGain->maximum()*10));
    ui->horizontalSliderGain->setTickInterval(1);

}


void WidgetCameraParameter::setCameraHandle(CameraInterface* handle)
{
    // fill site 1 with parameters
    camera = handle;
    ui->labelCameraName->setText(camera->deviceName());
    ui->labelCameraSerial->setText(camera->deviceSerial());

    // fill site 2 with parameters
    QMap<QString, QString> map = camera->getParamters();

    ui->listWidgetCameraInfo->clear();
    for( auto key: map.keys() )
    {
        QString strItem = key + ": " + map.value( key );
        ui->listWidgetCameraInfo->addItem(strItem);
    }

    // get current camera values
    ui->doubleSpinBoxExposure->setValue(static_cast<double>(camera->exposure()));
    QSize sensorSize = camera->sensorSize();
    ui->labelSensorSizeX->setText(QString("x = %1 px (width)").arg(sensorSize.width()));
    ui->labelSensorSizeY->setText(QString("y = %1 px (height)").arg(sensorSize.height()));


    setExposureMinMax(camera->exposureMin(), camera->exposureMax());
    setGainMinMax(camera->gainMin(), camera->gainMax());
    resetROI();
}

void WidgetCameraParameter::resetROI()
{
    QSize sensorSize = camera->sensorSize();
    int incrX = camera->binningIncrementX();
    int incrY = camera->binningIncrementY();
    ui->spinBoxX->setMinimum(0);
    ui->spinBoxX->setMaximum(sensorSize.width());
    ui->spinBoxX->setSingleStep(incrX);
    ui->spinBoxY->setMinimum(0);
    ui->spinBoxY->setMaximum(sensorSize.height());
    ui->spinBoxY->setSingleStep(incrY);
    ui->spinBoxdX->setMinimum(0);
    ui->spinBoxdX->setMaximum(sensorSize.width());
    ui->spinBoxX->setSingleStep(incrX);
    ui->spinBoxdY->setMinimum(0);
    ui->spinBoxdY->setMaximum(sensorSize.height());
    ui->spinBoxdY->setSingleStep(incrY);
    // set sensorsize
    QRect roi(QPoint(0,0), sensorSize);
    camera->setROI(roi);
    // get values back
    roi = camera->roi();
    ui->spinBoxX->setValue(roi.x());
    ui->spinBoxY->setValue(roi.y());
    ui->spinBoxdX->setValue(roi.width());
    ui->spinBoxdY->setValue(roi.height());
}
void WidgetCameraParameter::on_horizontalSliderExposure_valueChanged(int value)
{
    // only if the value was changed within the slider
    if (!m_SpinboxValueChanged)
        ui->doubleSpinBoxExposure->setValue(sliderToExposure(value));
}

void WidgetCameraParameter::on_horizontalSliderGain_valueChanged(int value)
{
    // only if the value was changed within the slider
    if (!m_SpinboxValueChanged)
        ui->doubleSpinBoxGain->setValue(value/10);
}

void WidgetCameraParameter::on_doubleSpinBoxExposure_valueChanged(double value)
{    
    static double lastValue;
    if (fabs(lastValue - 0) < 1e-9)
    {
        // step down has set the value to 0 (value - step = 0)
        if (lastValue > value) {
            if (value <= ui->doubleSpinBoxExposure->minimum())
            {
                ui->doubleSpinBoxExposure->setSingleStep(ui->doubleSpinBoxExposure->singleStep()/10);
                value = lastValue - ui->doubleSpinBoxExposure->singleStep();
                lastValue = value;
                ui->doubleSpinBoxExposure->setValue(value);
                return;
            }
        }
        // step up has reached next 1e^x value (10 times single step)
        // step is increased
        if (lastValue < value) {
            double tenStep = 10*ui->doubleSpinBoxExposure->singleStep();
            if ((lastValue <= tenStep) && (value > tenStep))
            {
                ui->doubleSpinBoxExposure->setSingleStep(ui->doubleSpinBoxExposure->singleStep()*10);
                value = tenStep + ui->doubleSpinBoxExposure->singleStep();
                lastValue = value;
                ui->doubleSpinBoxExposure->setValue(value);
                return;
            }
        }
    }
    if (!camera){
        LOG_ERROR() << "no camera loaded";
        return;
    }
    if (!(value>0)){
        LOG_ERROR() << "Exposure value must be > 0";
        return;
    }
    if (camera->isOpen())
    {        
        m_SpinboxValueChanged = true;
        ui->horizontalSliderExposure->blockSignals(true);

        float exposure = static_cast<float>(value);
        float cameraExposure = 0;
        try {
            camera->setExposure(exposure);
            cameraExposure = camera->exposure();
            if (fabs(cameraExposure-exposure) > 1e-4)
            {
                LOG_WARNING() << "resulting exposure differs" << fabs(cameraExposure-exposure);
            }

        } catch (const std::runtime_error& error) {
            LOG_ERROR() << error.what() << endl;
            LOG_INFO() <<" resulting exposure is " << cameraExposure << " ms";
        }
        catch (const std::out_of_range & error) {
            LOG_ERROR() << error.what() << endl;
        }


        ui->horizontalSliderExposure->setValue(exposureToSlider(cameraExposure));

        ui->horizontalSliderExposure->blockSignals(false);

    }
    m_SpinboxValueChanged = false;
    lastValue = value;
}

void WidgetCameraParameter::on_doubleSpinBoxGain_valueChanged(double value)
{
    ui->horizontalSliderGain->blockSignals(true);
    ui->horizontalSliderGain->setValue(value*10);
    if (!camera){
        LOG_ERROR() << "no camera loaded";
        return;
    }
    if (!(value>0)){
        LOG_ERROR() << "Exposure value must be > 0";
        return;
    }
    if (camera->isOpen())
    {
        m_SpinboxValueChanged = true;
        try {
            camera->setGain(static_cast<float>(value));
        } catch (const std::runtime_error& error) {
            LOG_ERROR() << error.what() << endl;
        }
        catch (const std::out_of_range & error) {
            LOG_ERROR() << error.what() << endl;
        }

        m_SpinboxValueChanged = false;
    }
    ui->horizontalSliderGain->blockSignals(false);
}

void WidgetCameraParameter::setNewROI(QRect roi)
{
    try {
        camera->setROI(roi);
    }
    catch (const std::runtime_error& error) {
        LOG_ERROR() << error.what() << endl;
        LOG_INFO() << "camera:" << camera->roi() << ", new:" << roi;
    }
}

bool WidgetCameraParameter::checkParameterROI(QSpinBox * spinBox, int incr)
{
    int value = spinBox->value();
    if (value % incr != 0)
    {
        spinBox->setStyleSheet("color: rgb(161,0,0); ");
        return false;
    }
    else {
        spinBox->setStyleSheet("color: rgb(0,0,0); ");
        return true;
    }
}

void WidgetCameraParameter::on_spinBoxX_valueChanged(int value)
{
    if (!checkParameterROI(ui->spinBoxX, camera->binningIncrementX()))
        return;

    QSize sensorSize = camera->sensorSize();
    if (value < sensorSize.width())
    {
        QRect roiOrig = camera->roi();
        QRect roiNew(roiOrig);

        roiNew.setLeft (value);
        // restore height
        if (value < roiOrig.left())
            roiNew.setWidth(roiOrig.width());

        setNewROI(roiNew);
        ui->spinBoxdX->setMaximum(sensorSize.width()-value);
    }

}

void WidgetCameraParameter::on_spinBoxdX_valueChanged(int value)
{
    if (!checkParameterROI(ui->spinBoxdX, camera->binningIncrementX()))
        return;

    QSize sensorSize = camera->sensorSize();
    QRect roi = camera->roi();
    if (value < (sensorSize.width()-roi.left()))
    {
        roi.setWidth(value);
        setNewROI(roi);
    }
}

void WidgetCameraParameter::on_spinBoxY_valueChanged(int value)
{
    if (!checkParameterROI(ui->spinBoxY, camera->binningIncrementY()))
        return;

    QSize sensorSize = camera->sensorSize();
    if (value < sensorSize.height())
    {
        QRect roiOrig = camera->roi();
        QRect roiNew(roiOrig);

        roiNew.setTop(value);
        // restore height
        if (value < roiOrig.top())
            roiNew.setHeight(roiOrig.height());

        setNewROI(roiNew);
        ui->spinBoxdY->setMaximum(sensorSize.height()-value);
    }
}

void WidgetCameraParameter::on_spinBoxdY_valueChanged(int value)
{
    if (!checkParameterROI(ui->spinBoxdY, camera->binningIncrementY()))
        return;

    QSize sensorSize = camera->sensorSize();
    QRect roi = camera->roi();
    if (value < (sensorSize.height()-roi.top()))
    {
        roi.setHeight(value);
        setNewROI(roi);
    }
}

void WidgetCameraParameter::correctParameterROI(QSpinBox * spinBox, int incr)
{
    int value = spinBox->value();
    if (value % incr != 0)
    {
        float division = static_cast<float>(value) / static_cast<float>(incr);
        value = static_cast<int>(round(division) * incr);
        spinBox->setValue(value);
    }
}

void WidgetCameraParameter::on_spinBoxX_editingFinished()
{
    correctParameterROI(ui->spinBoxX, camera->binningIncrementX());
}

void WidgetCameraParameter::on_spinBoxdX_editingFinished()
{
    correctParameterROI(ui->spinBoxdX, camera->binningIncrementX());
}

void WidgetCameraParameter::on_spinBoxY_editingFinished()
{
    correctParameterROI(ui->spinBoxY, camera->binningIncrementY());
}

void WidgetCameraParameter::on_spinBoxdY_editingFinished()
{
    correctParameterROI(ui->spinBoxdY, camera->binningIncrementY());
}

