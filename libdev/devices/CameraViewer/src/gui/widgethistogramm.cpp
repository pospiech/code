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

    QTime myTimer;
    myTimer.start();

    QVector<QPointF> points(static_cast<int>(data.size()));
    QVector<QPointF> pointsLog(static_cast<int>(data.size()));
    std::vector<float> logvalues(data.size());

    // maximum value
    auto max = *max_element(std::begin(data), std::end(data)) / 100.0f;

    // log only valid if any value > 0
    if ( max > 0) {
        // calculate log values
        float logValue = 0;
        for(std::vector<int>::size_type i = 0; i != data.size(); ++i) {
            if (data[i] > 0) {
                logValue = log10(static_cast<float>(data[i]));
            }
            else {
                logValue = 0;
            }
            logvalues[i] = logValue;
        }
    }
    else {
        std::fill (logvalues.begin(),logvalues.end(),0);
    }
    auto maxlog = *max_element(std::begin(logvalues), std::end(logvalues)) / 100.0f;

    if (maxlog <= 0) {
        LOG_ERROR() << " all points are value 0";
        return;
    }

    for(std::vector<int>::size_type i = 0; i != data.size(); ++i) {
        const int index = static_cast<int>(i);
        points[index] = QPointF(index, static_cast<double>(data[i]/max));
        pointsLog[index] = QPointF(index, static_cast<double>(logvalues[i]/maxlog));
    }

    series->replace(points);
    serieslog->replace(pointsLog);

    chart->axisX()->setRange(0, data.size());
    chart->axisY()->setRange(0, 100);
    chart->axisX(serieslog)->setRange(0, data.size());
    chart->axisY(serieslog)->setRange(0, 100);

    LOG_INFO() << "time for histogram plot" << myTimer.elapsed()/1000.0 << " / seconds";
}

