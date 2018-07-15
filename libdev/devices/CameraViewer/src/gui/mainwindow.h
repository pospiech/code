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

    void testImage();

private slots:
    void on_pushButtonTakeImage_clicked();

    void on_actionClose_triggered();

private:
    Ui::MainWindow *ui;
    QScopedPointer<CameraInterface> camera;
};

#endif // MAINWINDOW_H
