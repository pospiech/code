#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QtCore/QPointer>
//#include <QtCore/QScopedPointer>

class DialogCameraSMX;
class GlobalDevices;

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
    void OnActionQuit();
    void OnCameraSMX();

private:
    Ui::MainWindowClass * ui;
    QPointer<DialogCameraSMX> dlgCameraSMX;
    GlobalDevices * devices;


};



#endif // MAINWINDOW_H
