/* -*- mode: C++ ; c-file-style: "stroustrup" -*- *****************************
 * Plot Tools Library (based on Qwt)
 * Copyright (C) 2016   Matthias Pospiech
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the Qwt License, Version 1.0
 *****************************************************************************/

#ifndef QPLOTZOOMER_H
#define QPLOTZOOMER_H

#include <qwt_plot.h>
#include <qwt_plot_zoomer.h>

#include <QMap>
#include <array>

//#define DEBUG_QPLOTEXT 1

struct PlotScale{
public:
    PlotScale() {}
    PlotScale(double new_min, double new_max): min(new_min), max(new_max) {}
    double min;
    double max;
};


/*!
    \class QPlotZoomer
    \brief Derived from QwtPlotZoomer, implements a fix for correct inital zoom base.

    Typically when using the classes QBasePlotWidget or the derived QLinePlot or QMatrixPlot
    it is not necessary to create an instance of this class, since a zoomer is implemented
    within QBasePlotWidget.

    This is the code used within QBasePlotWidget
    @code
        // create zoomer - must be after canvas ist defined, otherwise
        // zoom is create with wrong canvas
        m_zoomerY1 = new QPlotZoomer(QwtPlot::xBottom, QwtPlot::yLeft, this);
        m_zoomerY2 = new QPlotZoomer(QwtPlot::xTop, QwtPlot::yRight, this);

        // enable first axis
        m_zoomerY1->setEnabled(true);
        // disable second axis
        m_zoomerY2->setEnabled(false);
    @endcode

    From outside the zoomer can be accessed using the QBasePlotWidget interface.
    @code
    // Zoom
    plot2->zoomerY2()->setEnabled(true);

    @endcode
 */


class QPlotZoomer: public QwtPlotZoomer
{
    Q_OBJECT
public:
    QPlotZoomer( QwtPlot * qwtplot )
        : QwtPlotZoomer( qwtplot->canvas() )
        , m_plot(qwtplot)
    {
        initZoomer();
        // connect zoomed signal for handling wrong zooms states
        connect(this, SIGNAL(zoomed (const QRectF &)), this, SLOT(OnZoomed()));

    }

    QPlotZoomer( int xAxis, int yAxis, QwtPlot * qwtplot )
        : QwtPlotZoomer( xAxis, yAxis, qwtplot->canvas() )
        , m_plot(qwtplot)
    {
        initZoomer();
        // connect zoomed signal for handling wrong zooms states
        connect(this, SIGNAL(zoomed (const QRectF &)), this, SLOT(OnZoomed()));
    }


    virtual ~QPlotZoomer( )
    {

    }

    //! Set manual scale that must be used by zoomer
    void setAxisScale (int axisId, double min, double max)
    {
        // insert or change value in map
        mapScale.insert(axisId, PlotScale (min, max));
    }

    //! Set scale to auto, so that zoomer does not fix a manual scale
    //! Only necessary if a manual scale was applied.
    void setAxisAutoScale (int axisId)
    {
        // remove axisID from map
        mapScale.remove(axisId);
    }


private slots:
    /*!
     *  handle zoom events, especially reset zoom-base
     *  if back to initial zoom state
     */
    void OnZoomed()
    {
        if ( zoomRectIndex() == 0 && zoomStack().size() == 0 )
        {
            // replot, since this is called when new data is applied
            // and last zoom state my not be base state.
            setZoomBase(true); // replot
        }

        // only reset zoombase and replot if coming from
        // zoomed state and state is at the initual size
        if ( zoomRectIndex() == 0 && zoomStack().size() > 1 )
        {
            std::array<int,4> axisIDs = {QwtPlot::yLeft,
                                         QwtPlot::yRight,
                                         QwtPlot::xBottom,
                                         QwtPlot::xTop};
            // iterate over all axis Ids
            for (auto &axisID : axisIDs)
            {
                if ( mapScale.contains(axisID) ) {
                    PlotScale scale = mapScale[axisID];
                    m_plot->setAxisScale(axisID, scale.min, scale.max);
                } else {
                    m_plot->setAxisAutoScale(axisID);
                }
            }
            setZoomBase(true);
        }
        debugZoomerStack();
    }


private:
    void initZoomer()
    {
        setTrackerMode( QwtPicker::AlwaysOff );
        setRubberBand( QwtPicker::RectRubberBand );
        setRubberBandPen( QPen( Qt::black, 1,Qt::SolidLine ) );

        // RightButton: zoom out by 1
        // Ctrl+RightButton: zoom out to full size

        setMousePattern( QwtEventPattern::MouseSelect2,
                         Qt::RightButton, Qt::ControlModifier );
        setMousePattern( QwtEventPattern::MouseSelect3,
                         Qt::RightButton );
    }

    void debugZoomerStack()
    {
#ifdef DEBUG_QPLOTEXT
        int index = this->zoomRectIndex();
        QStack<QRectF> stack = this->zoomStack();
        qDebug() << "--- Zoom Stack -----------------";
        for(int i = 0; i < stack.size(); ++i) {
            if (i == index) {
                qDebug() << i << "*, " << stack.at(i);
            } else {
                qDebug() << i << ", " << stack.at(i);
            }
        }
#endif
    }

private:
    QwtPlot * m_plot;
    QMap<int, PlotScale> mapScale;
};


#endif // QPLOTZOOMER_H
