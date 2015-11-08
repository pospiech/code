#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#include "ui/ui_MainWindow.h"

class MainWindow : public QMainWindow, protected Ui_MainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget* parent = 0, Qt::WindowFlags flags = 0);
    virtual ~MainWindow();

private slots:
    void on_pushButton_clicked(bool checked = false); 
    void on_pushButtonSet_clicked(bool checked = false); 


};
#endif
