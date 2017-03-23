#include "plotcomplexdata.h"
#include "ui_plotcomplexdata.h"

PlotComplexData::PlotComplexData(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlotComplexData)
{
    ui->setupUi(this);
}

PlotComplexData::~PlotComplexData()
{
    delete ui;
}
