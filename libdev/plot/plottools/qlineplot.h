/* -*- mode: C++ ; c-file-style: "stroustrup" -*- *****************************
 * Plot Tools Library (based on Qwt)
 * Copyright (C) 2016   Matthias Pospiech
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the Qwt License, Version 1.0
 *****************************************************************************/

#ifndef QLinePlot_H
#define QLinePlot_H

#include <QPen>

#include "qplotcurve.h"
#include "qbaseplotwidget.h"
#include "qcolorpalette.h"

#include <vector>

/*!
    \class QLinePlot
    \brief Class for the creation of line plots with multiple curves on a single or both Y-Axis

    Example with
    - two y-axis,
    - a legend below,
    - zoomer set up for both y-axis
    - grid enabled in x and y
    - a custom color palette applied

    @code
    QLinePlot * plot2 = new QLinePlot();
    plot2->setTitle( "Plot Demo - two Y axis" );

    // legend
    QwtLegend *legend = new QwtLegend;
    plot2->insertLegend( legend, QwtPlot::BottomLegend );

    plot2->enableAxis(QwtPlot::yRight);

    plot2->setAxisTitle (QwtPlot::xBottom, "x-axis");
    plot2->setAxisTitle (QwtPlot::yLeft, "y-axis 1");
    plot2->setAxisTitle (QwtPlot::yRight, "y-axis 2");

    // Zoom
    plot2->zoomerY2()->setEnabled(true);

    // Grid
    plot2->grid()->enableX(true);
    plot2->grid()->enableY(true);

    QPlotCurve *curve21 = new QPlotCurve();
    curve21->setTitle( "x^2" );
    curve21->setPen( QColorPalette::color(1), 4 ),
    curve21->setYAxis(QwtPlot::yLeft);
    curve21->setData(x, y21);

    QPlotCurve *curve22 = new QPlotCurve();
    curve22->setTitle( "sin(x)" );
    curve22->setPen( QColorPalette::color(2), 4 ),
    curve22->setYAxis(QwtPlot::yRight);
    curve22->setData(x, y22);
    // alternatively use direct pointers:
    // curve22->setRawSamples(&x[0], &y22[0], x.size());

    // attach Curve to qwtPlot
    curve21->attach( plot2 );
    curve22->attach( plot2 );

    plot2->replot();

    // curves must be attached to plot, otherwise
    // nothing is applied.
    plot2->setColorPalette(QColorPalette::MSOffice2007);

    @endcode
 */


class QLinePlot : public QBasePlotWidget
{
    Q_OBJECT
public:
    QLinePlot(QWidget* parent = 0): QBasePlotWidget(parent)
    {
        m_marginPercentage = 0;
    }

    virtual ~QLinePlot()
    {

    }

    //! create vector of all attached QPlotCurve items
    //!
    //! \return vector of QPlotCurve pointers
    std::vector<QPlotCurve*> curves()
    {
        QList<QwtPlotItem *> listItems = this->itemList();
        std::vector<QPlotCurve*> curveList;
        // create list of all curves
        for( int i=0; i<listItems.count(); ++i )
        {
            QPlotCurve* derived = dynamic_cast<QPlotCurve*>(listItems[i]);
            if (derived) {
                curveList.push_back(derived);
            }
        }
        return curveList;
    }


    //! set margin for all curves, used in QPlotCurve to define a margin
    //! to the axis boundaries
    //!
    //! \param value bounding margin in percent (0..100)
     void setBoundingMarginPercent(double value)
    {
        if (value >= 0){
            // if any
            std::vector<QPlotCurve*> curveList = curves();
            if (curveList.size() > 0) {
                m_marginPercentage = value;
                // apply margin value to all curves
                for (size_t i = 0; i < curveList.size(); ++i) {
                    curveList[i]->setBoundingMarginPercent(m_marginPercentage);
                }
            }
        }
    }

    //! Set color palette and apply it to all attached curves
    void setColorPalette(QColorPalette::paletteIdentifier palette)
    {
        std::vector<QPlotCurve*> curveList = curves();
        const size_t count = curveList.size();
        if (count > 0) {
            for (size_t i = 0; i < count; ++i) {
                // read current pen
                QPen pen = curveList[i]->pen();
                // and exchange only color
                pen.setColor(QColorPalette::color(i, palette, count));
                curveList[i]->setPen(pen);
            }
        }

    }

    //! delete all PlotItems (and thus also curves) from the plot
    void clear()
    {
        QList<QwtPlotItem *> listItems = this->itemList();
        for( int i=0; i<listItems.count(); ++i )
        {
            listItems[i]->detach();
            delete listItems[i];
        }
    }


    //! overwrites `QwtPlot::setAxisScale`
    //! uses the internal margin value to correct the scale
    void setAxisScale (int axisId, double min, double max, double step)
    {
        double margin = (max-min)* m_marginPercentage;
        min = min - margin;
        max = max + margin;
        QwtPlot::setAxisScale(axisId, min, max, step);
    }

private:
    double m_marginPercentage;

};



#endif // QLinePlot_H
