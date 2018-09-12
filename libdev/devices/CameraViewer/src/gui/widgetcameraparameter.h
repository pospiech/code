#ifndef WIDGETCAMERAPARAMETER_H
#define WIDGETCAMERAPARAMETER_H

#include <QWidget>

namespace Ui {
class WidgetCameraParameter;
}

class CameraInterface;

class WidgetCameraParameter : public QWidget
{
    Q_OBJECT
public:
    explicit WidgetCameraParameter(QWidget *parent = nullptr);

signals:

public slots:
    void setCameraHandle(CameraInterface* handle);

private slots:
//    void on_horizontalSliderExposure_valueChanged(int value);
//    void on_horizontalSliderGain_valueChanged(int value);
    void OndoubleSpinBoxExposure_valueChanged(double value);
//    void on_doubleSpinBoxGain_valueChanged(double value);
//    void on_spinBoxX_valueChanged(int value);
//    void on_spinBoxdX_valueChanged(int value);
//    void on_spinBoxY_valueChanged(int value);
//    void on_spinBoxdY_valueChanged(int value);

private:
    Ui::WidgetCameraParameter *ui;
    CameraInterface *camera = nullptr;
    bool m_SpinboxValueChanged;

};

#endif // WIDGETCAMERAPARAMETER_H
