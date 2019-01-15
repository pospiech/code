#include "widgetspectrumplot.h"

#include "Logger.h"
#include <QGridLayout>
#include <QTime>


WidgetSpectrumPlot::WidgetSpectrumPlot(QWidget *parent) : QWidget(parent)
{
    createGUI();
}


void WidgetSpectrumPlot::createGUI()
{
    chart = new QChart();
    chart->legend()->hide();
    chart->setTitle("Spectrum");

    axisX = new QValueAxis;
    axisX->setLabelFormat("%g");
    chart->addAxis(axisX, Qt::AlignBottom);

    series = new QLineSeries;
    chart->addSeries(series);

    axisY = new QValueAxis;
    axisY->setTitleText("counts");
    axisY->setLinePenColor(series->pen().color());

    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisX);
    series->attachAxis(axisY);

    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    // Create a layout and add Chart
    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(chartView);
}

void WidgetSpectrumPlot::setData(const std::vector<int> data)
{
    if (data.size() <= 0)
    {
        LOG_DEBUG() << "Spectrum Data empty";
        return;
    }

    QTime myTimer;
    myTimer.start();

    QVector<QPointF> points(static_cast<int>(data.size()));

    // maximum value
    auto max = *max_element(std::begin(data), std::end(data)) / 100.0f;


    for(std::vector<int>::size_type i = 0; i != data.size(); ++i) {
        const int index = static_cast<int>(i);
        points[index] = QPointF(index, static_cast<double>(data[i]));

    }

    series->replace(points);

    chart->axes(Qt::Horizontal).back()->setRange(0, data.size());
    chart->axes(Qt::Vertical).back()->setRange(0, max);

//    xAxis.setRange(0, data.size());
//    yAxis.setRange(0, max);

    LOG_INFO() << "time for spectrum plot" << myTimer.elapsed()/1000.0 << " / seconds";
}

