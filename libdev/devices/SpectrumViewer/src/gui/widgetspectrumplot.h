#ifndef WIDGETSPECTRUMPLOT_H
#define WIDGETSPECTRUMPLOT_H

#include <QWidget>

#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QtCharts/QLogValueAxis>
QT_CHARTS_USE_NAMESPACE

#include <vector>

class WidgetSpectrumPlot : public QWidget
{
    Q_OBJECT
public:
    explicit WidgetSpectrumPlot(QWidget *parent = nullptr);

signals:

public slots:
    void setData(const std::vector<int> data);

private:
    void createGUI();
    QLineSeries * series;
    QChart * chart;
    QChartView * chartView;
    QValueAxis * axisX;
    QValueAxis * axisY;
};

#endif // WIDGETSPECTRUMPLOT_H





