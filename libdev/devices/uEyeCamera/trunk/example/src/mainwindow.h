#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QtCore/QPointer>
//#include <QtCore/QScopedPointer>
#include <QTimer>

class QuEyeCamera;
class DialogCameraSettings;

namespace Ui
{
    class MainWindowClass;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void initValues();
    void connectSignalSlots();


private slots:
    void on_pushButtonStopVideo_clicked();
    void on_pushButtonStartVideo_clicked();
    void on_pushButtonSaveImage_clicked();
    void on_pushButtonGetImage_clicked();
    void OnActionQuit();
    void OnActionCameraSettings();
    void OnNewSnapShotImage();

private:
    //const QScopedPointer<Ui::MainWindowClass> ui;
    Ui::MainWindowClass * ui;
    QPointer<DialogCameraSettings> dlgCameraSettings;

    QuEyeCamera  * camera;

    QTimer *timer;

};



#endif // MAINWINDOW_H
