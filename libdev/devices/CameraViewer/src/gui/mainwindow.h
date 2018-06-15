#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class CameraXimea;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void testImage();

private slots:
    void on_pushButtonTakeImage_clicked();

private:
    Ui::MainWindow *ui;
    QScopedPointer<CameraXimea> camera;
};

#endif // MAINWINDOW_H
