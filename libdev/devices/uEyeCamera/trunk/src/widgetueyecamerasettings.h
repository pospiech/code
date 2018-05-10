#ifndef WIDGETUEYECAMERASETTINGS_H
#define WIDGETUEYECAMERASETTINGS_H

#include "QCameraGlobal.h"
#include "uEye.h"

class QuEyeCamera;

#include <QtWidgets/QWidget>

namespace Ui {
    class WidgetUEyeCameraSettings;
}

class QCAMERA_EXPORT WidgetUEyeCameraSettings : public QWidget {
    Q_OBJECT
public:
    WidgetUEyeCameraSettings(QWidget *parent = 0);
    ~WidgetUEyeCameraSettings();
    void setCamera(QuEyeCamera * parentCamera);


private:
    void initWidgets();

private slots:
    void on_checkBoxWhiteBalance_stateChanged(int );
    void on_doubleSpinBoxGamma_valueChanged(double value);
    void on_checkBoxGainBoost_stateChanged(int );
    void on_doubleSpinBoxGainBlue_valueChanged(double );
    void on_doubleSpinBoxGainGreen_valueChanged(double );
    void on_doubleSpinBoxGainRed_valueChanged(double );
    void on_checkBoxFixedRGBGainValues_stateChanged(int );
    void on_doubleSpinBoxGainMono_valueChanged(double );
    void on_comboBoxColorMode_currentIndexChanged(int index);
    void on_doubleSpinBoxFrequency_valueChanged(double );
    void on_doubleSpinBoxAutoBrightness_valueChanged(double );
    void on_doubleSpinBoxExposure_valueChanged(double );
    void on_comboBoxShutterMode_currentIndexChanged(int index);
    void on_spinBoxViewPortWidth_valueChanged(int );
    void on_spinBoxViewPortHeight_valueChanged(int );
    void on_spinBoxViewPortTop_valueChanged(int );
    void on_spinBoxViewPortLeft_valueChanged(int );
    void on_comboBoxDecimation_currentIndexChanged(int index);
    void updateWidgetsWithCameraInfo();
    void updatePixelNumber();
    void OnCameraConnectionChanged();
    void updateWidgetsStatusIfConnected();
    void OnViewPortChanged();
    void OnGainRGBChanged();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::WidgetUEyeCameraSettings *ui;

    QuEyeCamera * camera;
    bool blockAllSignals;

};

#endif // WIDGETUEYECAMERASETTINGS_H
