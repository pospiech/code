#ifndef DIALOGCAMERASMX_H_
#define DIALOGCAMERASMX_H_


#include "ui_DialogCameraSMX.h"
#include <QtGui/QDialog>

namespace Ui
{
    class DialogCameraSMX;
}


class DialogCameraSMX : public QDialog
{
    Q_OBJECT
public:
    DialogCameraSMX(QWidget* parent = 0, Qt::WFlags flags = 0);
    virtual ~DialogCameraSMX();

private slots:
    void on_pushButtonStopVideo_clicked();
    void on_pushButtonStartVideo_clicked();
    void closeEvent ( QCloseEvent * event );

    void OnButtonClose();
    void OnButtonCapture();
    void OnButtonSaveData();
    void OnButtonSavePicture();
    void OnButtonConnect();
    void OnCameraError(QString error);

private:
    void connectSignalSlots();
    void initWidgets();

private:
    Ui::DialogCameraSMX * ui;


};
#endif
