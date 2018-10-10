#include "widgethistogramm.h"
#include <QGridLayout>

#include <QDebug>
#include <QTime>
#include "Logger.h"

#include <QtCharts/QSplineSeries>
#include <QtCharts/QValueAxis>
#include <QtCharts/QCategoryAxis>

WidgetHistogramm::WidgetHistogramm(QWidget *parent) : QWidget(parent)
{

    chart = new QChart();
    chart->legend()->hide();
    chart->setTitle("Histogramm");

    axisX = new QValueAxis;
    axisX->setLabelFormat("%g");
    chart->addAxis(axisX, Qt::AlignBottom);

    series = new QLineSeries;
    chart->addSeries(series);

    axisY = new QValueAxis;
    axisY->setTitleText("linear scale");
    axisY->setLinePenColor(series->pen().color());
    //        axisY->setGridLinePen((series->pen()));

    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisX);
    series->attachAxis(axisY);

    serieslog = new QLineSeries;
    chart->addSeries(serieslog);


//    axisY3 = new QLogValueAxis();
    axisY3 = new QValueAxis();

    axisY3->setTitleText("logarithmic scale");
//    axisY3->setLabelFormat("%g");
//    axisY3->setBase(10.0);
    axisY3->setLinePenColor(serieslog->pen().color());

    chart->addAxis(axisY3, Qt::AlignRight);
    serieslog->attachAxis(axisX);
    serieslog->attachAxis(axisY3);

    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    // Create a layout and add Chart
    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(chartView);
}

WidgetHistogramm::~WidgetHistogramm()
{

}


void WidgetHistogramm::setData(const std::vector<int> data)
{
    if (data.size() <= 0)
    {
        LOG_DEBUG() << "Histogram Data empty";
        return;
    }

    auto max = *max_element(std::begin(data), std::end(data)) / 100;

    QVector<QPointF> points(data.size());
    QVector<QPointF> pointsLog(data.size());

    // calculate log values
    std::vector<float> logvalues(data.size());
    float logValue = 0;
    for(std::vector<int>::size_type i = 0; i != data.size(); ++i) {
        if (data[i] > 0) {
            logValue = log10(data[i]);
        }
        else {
            logValue = 0;
        }
        logvalues[i] = logValue;
    }
    auto maxlog = *max_element(std::begin(logvalues), std::end(logvalues)) / 100;

    for(std::vector<int>::size_type i = 0; i != data.size(); ++i) {
      points[i] = QPointF(i, data[i]/max);
      pointsLog[i] = QPointF(i, logvalues[i]/maxlog);
    }

    series->replace(points);
    serieslog->replace(pointsLog);

    chart->axisX()->setRange(0, data.size());
    chart->axisY()->setRange(0, 100);
    chart->axisX(serieslog)->setRange(0, data.size());
    chart->axisY(serieslog)->setRange(0, 100);
}

