#ifndef PLOTCOMPLEXDATA_H
#define PLOTCOMPLEXDATA_H

#include <QWidget>
#include <QScopedPointer>

namespace Ui {
class PlotComplexData;
}

class PlotComplexDataPrivate;

class PlotComplexData : public QWidget
{
    Q_OBJECT

public:
    explicit PlotComplexData(QWidget *parent = 0);
    virutal ~PlotComplexData();

protected:
    const QScopedPointer<PlotComplexDataPrivate> d_ptr;

private:
    Q_DECLARE_PRIVATE(PlotComplexDataPrivate)

private:
    Ui::PlotComplexData *ui;
};

#endif // PLOTCOMPLEXDATA_H
