#ifndef DIALOGCAMERASETTINGS_H
#define DIALOGCAMERASETTINGS_H

#include <QDialog>

class QuEyeCamera;

namespace Ui {
    class DialogCameraSettings;
}

class DialogCameraSettings : public QDialog {
    Q_OBJECT
public:
    DialogCameraSettings(QWidget *parent = 0);
    ~DialogCameraSettings();

    void setCamera(QuEyeCamera *camera);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::DialogCameraSettings *ui;
};

#endif // DIALOGCAMERASETTINGS_H
