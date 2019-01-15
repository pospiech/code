#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "avantes/avaspec.h"
#include "avantes/colcalc.h"
#include "avantes/irrad.h"

#include <QtGui>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    resize(QGuiApplication::primaryScreen()->availableSize()  * 4 / 5);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionQuit_triggered()
{
    QCoreApplication::quit();
}
