#include "WidgetHistogramm.h"
#include <QGridLayout>

#include <QDebug>
#include "Logger.h"

WidgetHistogramm::WidgetHistogramm(QWidget *parent) : QWidget(parent)
{
    // create line Series for the data
    series = new QLineSeries();
    // create Chart Object (QWidget)
    chart = new QChart();
    chart->legend()->hide();
    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->setTitle("Histogramm");

    // create a view inside the Chart
    chartView = new QChartView(chart, this);

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

    auto max = *max_element(std::begin(data), std::end(data));

    QList<QPointF> points;
    points.reserve(data.size());

    for(std::vector<int>::size_type i = 0; i != data.size(); i++) {
        QPointF point(i, data[i]*100/max);
        points.append(point);
    }
    series->clear();
    series->append(points);

    chart->axisX()->setRange(0, data.size());
//    chart->axisX()->setRange(0, 100);
    chart->axisY()->setRange(0, 100);

//    series->clear();
//    for(std::vector<int>::size_type i = 0; i != data.size(); i++) {
//        series->append(i, data[i]);
//        // qDebug() << i << data[i] << endl;
//    }
}

