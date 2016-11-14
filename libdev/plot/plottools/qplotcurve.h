/* -*- mode: C++ ; c-file-style: "stroustrup" -*- *****************************
 * Plot Tools Library (based on Qwt)
 * Copyright (C) 2016   Matthias Pospiech
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the Qwt License, Version 1.0
 *****************************************************************************/

#ifndef QPLOTCURVE_H
#define QPLOTCURVE_H

#include <qwt_plot_curve.h>

/*!
    \class QPlotCurve
    \brief Class for replacing the QwtPlotCurve class for line plot data

    This class basically provides additionally to the QwtPlotCurve base class
    a convenience `setData` function and defines a changeable margin around the curves.

    Example
    @code
    QLinePlot * plot = new QLinePlot();

    QPlotCurve * curve = new QPlotCurve();
    curve->setTitle( "sin(x)" );
    curve->setPen( QColorPalette::color(2), 4 ),
    curve->setYAxis(QwtPlot::yLeft);
    curve->setData(x, y);

    // attach Curve to qwtPlot
    curve->attach( plot );

    @endcode
 */


class QPlotCurve : public QwtPlotCurve
{
public:
    //! Constructor
    //! Sets basic QwtPlotCurve properties as default
    //!
    //! The margin is set to 5%

    QPlotCurve() : QwtPlotCurve()
    {
        QwtPlotCurve::setPaintAttribute(QwtPlotCurve::ClipPolygons, true);
        QwtPlotCurve::setRenderHint( QwtPlotItem::RenderAntialiased, true );
        QwtPlotCurve::setPaintAttribute(QwtPlotCurve::ClipPolygons, true);
        QwtPlotCurve::setStyle(QwtPlotCurve::Lines);
        QwtPlotCurve::setLegendAttribute( QwtPlotCurve::LegendShowLine );

        setBoundingMarginPercent(5);

    }

    virtual ~QPlotCurve()
    {
    }

    //! Set data to the curve (as a copy)
    //! Internally `QwtPlotCurve::setRawSamples` is used.
    //!
    //! \param x x-axis
    //! \param y y-axis
    void setData(const std::vector<double> & x, const std::vector<double> & y)
    {
        // vector sizes must be equal
        Q_ASSERT(x.size() == y.size());
        xaxis.clear();
        ydata.clear();
        // copy to internal vector
        try {
            xaxis = std::vector<double>(x);
            ydata = std::vector<double>(y);
        } catch (std::bad_alloc const&) {
            qCritical() << "Memory allocation fail!" << endl;
            xaxis.clear();
            ydata.clear();
            return;
        }
        QwtPlotCurve::setRawSamples(&xaxis[0], &ydata[0], xaxis.size());
    }

    //! set the bounding margin as percentage of the width and height
    //!
    //! \param percentage value in percentage (0..100)
    void setBoundingMarginPercent(double percentage)
    {
        if (percentage >= 0){
            m_marginPercentage = percentage/100.0;
        }
    }

    //! overloaded `boundingRect()`
    //!
    //! uses the bounding margin value ot correct the margin
    QRectF boundingRect() const
    {
        QRectF currentRect = QwtPlotCurve::boundingRect();
        if ( currentRect.isValid() )
        {
            double marginX = currentRect.width() * m_marginPercentage;
            double marginY = currentRect.height() * m_marginPercentage;
            currentRect.adjust(-marginX, -marginY, marginX, marginY);
        }
        return currentRect;
    }

private:
    double m_marginPercentage;
    std::vector<double> xaxis;
    std::vector<double> ydata;

};


#endif // QPLOTCURVE_H
