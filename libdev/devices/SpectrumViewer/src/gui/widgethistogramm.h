#ifndef WIDGETHISTOGRAMM_H
#define WIDGETHISTOGRAMM_H

#include <QWidget>
#include <QScopedPointer>

//class QLineSeries;
//class QChartView;
//class QChart;

#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QtCharts/QLogValueAxis>

QT_CHARTS_USE_NAMESPACE

#include <vector>

class WidgetHistogramm : public QWidget
{
    Q_OBJECT
public:
    explicit WidgetHistogramm(QWidget *parent = nullptr);
    ~WidgetHistogramm();

signals:

public slots:
    void setData(const std::vector<int> data);


private:
    QLineSeries * series;
    QLineSeries * serieslog;
    QChart * chart;
    QChartView * chartView;
    QValueAxis * axisX;
    QValueAxis * axisY;
//    QLogValueAxis * axisY3;
    QValueAxis * axisY3;


};

#endif // WIDGETHISTOGRAMM_H
