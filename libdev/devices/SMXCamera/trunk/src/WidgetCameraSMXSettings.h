#ifndef WIDGETCAMERASMXSETTINGS_H_
#define WIDGETCAMERASMXSETTINGS_H_

#include "QCameraSMXGlobal.h"

#include <QWidget>

class QCameraSMX;

namespace Ui
{
    class WidgetCameraSMXSettings;
}

class QSMXCAMERA_EXPORT WidgetCameraSMXSettings : public QWidget
{
    Q_OBJECT
public:
    WidgetCameraSMXSettings(QWidget* parent = 0, Qt::WFlags flags = 0);
    virtual ~WidgetCameraSMXSettings();

    void setCamera(QCameraSMX * parentCamera);

private:
    void initWidgets();

private slots:
    void on_spinBoxDACOffsetFine_valueChanged(int );
    void on_spinBoxDACOffsetRaw_valueChanged(int );
    void on_comboBoxMirrorY_currentIndexChanged(int index);
    void on_comboBoxMirrorX_currentIndexChanged(int index);
    void on_spinBoxViewPortWidth_valueChanged(int );
    void on_spinBoxViewPortHeight_valueChanged(int );
    void on_spinBoxViewPortTop_valueChanged(int );
    void on_spinBoxViewPortLeft_valueChanged(int );
    void on_spinBoxGamma_valueChanged(int );
    void on_spinBoxContrast_valueChanged(int );
    void on_spinBoxBrightness_valueChanged(int );
    void on_doubleSpinBoxGainBlue_valueChanged(double );
    void on_doubleSpinBoxGainGreen_valueChanged(double );
    void on_doubleSpinBoxGainRed_valueChanged(double );
    void on_checkBoxFixedRGBGainValues_stateChanged(int );
    void on_doubleSpinBoxGainMono_valueChanged(double );
    void on_comboBoxBayerAlgorithm_currentIndexChanged(int index);
    void on_comboBoxBitDepth_currentIndexChanged(int index);
    void on_comboBoxColorDepth_currentIndexChanged(int index);
    void on_comboBoxDecimation_currentIndexChanged(int index);
    void on_comboBoxFrequency_currentIndexChanged(int index);
    void on_comboBoxSnapshotMultiplier_currentIndexChanged(int index);
    void on_doubleSpinBoxExposure_valueChanged(double );
    void on_comboBoxShutterMode_currentIndexChanged(int index);
    //void on_pushButtonConnect_clicked();
    void updateWidgetsWithCameraInfo();
    void updateWidgetsStatusIfConnected();
    void OnCameraConnectionChanged();
    void updatePixelNumber();

private:
    Ui::WidgetCameraSMXSettings * ui;

    QCameraSMX * camera;
    bool blockAllSignals;

};
#endif
