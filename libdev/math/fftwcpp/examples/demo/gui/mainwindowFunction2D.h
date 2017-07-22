#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QScopedPointer>
#include <QSharedPointer>
#include <QVector>

class QwtPlot;
class QLinePlot;

#include "functions/functionfactory.h"
#include "functions/complexfunctionfactory.h"

namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

private:
    void setupWidgets(const int functionIndex);

private:
    Ui::MainWindow *ui;
    QList<QSharedPointer<FunctionFactory>> functionListAmplitude;
    QList<QSharedPointer<FunctionFactory>> functionListPhase;

public:


};

#endif // MAINWINDOW_H
