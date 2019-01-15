#ifndef WIDGETCAMERAPARAMETER_H
#define WIDGETCAMERAPARAMETER_H

#include <QWidget>

namespace Ui {
class WidgetCameraParameter;
}

class CameraInterface;
class QSpinBox;

class WidgetCameraParameter : public QWidget
{
    Q_OBJECT
public:
    explicit WidgetCameraParameter(QWidget *parent = nullptr);

signals:

public slots:
    void setCameraHandle(CameraInterface* handle);
    void setExposureMinMax(double min, double max);
    void setGainMinMax(double min, double max);
    void resetROI();
    void setNewROI(QRect roi);

private slots:
    void on_horizontalSliderExposure_valueChanged(int value);
    void on_horizontalSliderGain_valueChanged(int value);
    void on_doubleSpinBoxExposure_valueChanged(double value);
    void on_doubleSpinBoxGain_valueChanged(double value);
    void on_spinBoxX_valueChanged(int value);
    void on_spinBoxdX_valueChanged(int value);
    void on_spinBoxY_valueChanged(int value);
    void on_spinBoxdY_valueChanged(int value);

    bool checkParameterROI(QSpinBox * spinBox, int incr);
    void correctParameterROI(QSpinBox * spinBox, int incr);
    void on_spinBoxX_editingFinished();
    void on_spinBoxdX_editingFinished();
    void on_spinBoxY_editingFinished();
    void on_spinBoxdY_editingFinished();

    void on_pushButtonResetROI_pressed();

private:
    Ui::WidgetCameraParameter *ui;
    CameraInterface *camera = nullptr;
    bool m_SpinboxValueChanged;

};

#endif // WIDGETCAMERAPARAMETER_H
