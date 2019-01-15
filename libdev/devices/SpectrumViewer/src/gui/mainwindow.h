#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "camerainterface.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void findCameras();

private slots:
    void on_pushButtonTakeImage_clicked();

    void on_actionClose_triggered();

    void on_pushButtonCameraOpen_clicked();

    void on_pushButtonCameraClose_clicked();

private:
    Ui::MainWindow *ui;
    QScopedPointer<CameraInterface> camera;
    QList<CameraInterface*> cameraList;
};

#endif // MAINWINDOW_H
