/* -*- mode: C++ ; c-file-style: "stroustrup" -*- *****************************
 * Plot Tools Library (based on Qwt)
 * Copyright (C) 2016   Matthias Pospiech
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the Qwt License, Version 1.0
 *****************************************************************************/

#ifndef QBASEPLOTWIDGET_H
#define QBASEPLOTWIDGET_H

#include <qwt_plot.h>
#include <qwt_plot_layout.h>
#include <qwt_plot_canvas.h>
#include <qwt_scale_engine.h>
#include <qwt_scale_widget.h>
#include <qwt_scale_draw.h>
#include <qwt_plot_grid.h>
#include <qwt_text_label.h>

#include <QPrinter>
#include <QPrintDialog>
#include <qwt_plot_renderer.h>

#include "qplotzoomer.h"


/*!
    \class QBasePlotWidget
    \brief Base class for all QwtPlot derived classes in this library

    Use QBasePlotWidget as you would use QwtPlot,
    so for any further reading refer to the Qwt documention.

    Additionally to QwtPlot this class offers:
    - default QPlotZoomer zoomers, first for left y-axis, second (disabled by default) for the right y-axis
    - grid, disabled by default
    - the canvas (not accessible with QwtPlot)
    - a renderer function (not tested though!)

    Thus all classes derived from QBasePlotWidget provide a default zoom function.

    @code
    // exchange
    QwtPlot * plot = new QwtPlot();
    // with
    QBasePlotWidget * plot = new QBasePlotWidget();
    // or derive from QBasePlotWidget
    @endcode
 */

class QBasePlotWidget : public QwtPlot
{
    Q_OBJECT
public:
    QBasePlotWidget(QWidget* parent = 0)
        : QwtPlot(parent)
    {
        // set expanding size policy
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(20);
        sizePolicy.setVerticalStretch(20);
        QWidget::setSizePolicy(sizePolicy);

        // set dault margins for Widget
        const int margin = 5;
        QWidget::setContentsMargins( margin, margin, margin, 0 );

        // set canvas with white background.
        m_canvas = new QwtPlotCanvas();
        m_canvas->setBorderRadius( 1 );
        m_canvas->setLineWidth(1);
        QwtPlot::setCanvas( m_canvas );
        QwtPlot::setCanvasBackground( Qt::white );

        QwtPlot::setFrameStyle(QFrame::NoFrame);
        QwtPlot::plotLayout()->setAlignCanvasToScales(true);

        // reduce giant font size to standard font size on title and labels
        QFont font = QFont(fontInfo().family(), fontInfo().pointSize(), QFont::Normal);
        this->titleLabel()->setFont(font);
        this->axisWidget(QwtPlot::xTop)->setFont(font);
        this->axisWidget(QwtPlot::xBottom)->setFont(font);
        this->axisWidget(QwtPlot::yLeft)->setFont(font);
        this->axisWidget(QwtPlot::yRight)->setFont(font);

        QwtText text;
        text.setFont(font);
        this->setAxisTitle(QwtPlot::xBottom, text);
        this->setAxisTitle(QwtPlot::xBottom,text);
        this->setAxisTitle(QwtPlot::yLeft,text);
        this->setAxisTitle(QwtPlot::yRight,text);

        QwtPlot::setAxisAutoScale(QwtPlot::xBottom);
        QwtPlot::setAxisAutoScale(QwtPlot::yLeft);
        QwtPlot::setAxisAutoScale(QwtPlot::yRight);

        QwtPlot::axisScaleEngine(QwtPlot::xBottom)->setAttributes(QwtScaleEngine::Floating);
        QwtPlot::axisScaleEngine(QwtPlot::yLeft)->setAttributes(QwtScaleEngine::Floating);
        QwtPlot::axisScaleEngine(QwtPlot::yRight)->setAttributes(QwtScaleEngine::Floating);

        // create zoomer - must be after canvas ist defined, otherwise
        // zoom is create with wrong canvas
        m_zoomerY1 = new QPlotZoomer(QwtPlot::xBottom, QwtPlot::yLeft, this);
        m_zoomerY2 = new QPlotZoomer(QwtPlot::xTop, QwtPlot::yRight, this);

        // set zoomer properties
        m_zoomerY1->setEnabled(true);
        // disable second axis
        m_zoomerY2->setEnabled(false);

        // Avoid jumping when labels with more/less digits
        // appear/disappear when scrolling vertically
        const QFontMetrics fm( axisWidget( QwtPlot::yLeft )->font() );
        QwtScaleDraw *sd = axisScaleDraw( QwtPlot::yLeft );
        sd->setMinimumExtent( fm.width( "100.00" ) );

        // create grid
        m_grid = new QwtPlotGrid();
        m_grid->attach( this );
        // set color and style
        m_grid->setMajorPen(QPen(Qt::gray, 0, Qt::DotLine));
        m_grid->setMinorPen(QPen(Qt::lightGray, 0, Qt::DotLine));
        m_grid->enableX(false);
        m_grid->enableY(false);
    }

    virtual ~QBasePlotWidget()
    {
        if (m_zoomerY1)
            delete m_zoomerY1;
        if (m_zoomerY2)
            delete m_zoomerY2;
        if (m_grid)
            delete m_grid;
    }

    //! overwrites `QwtPlot::setAxisScale`
    //! applies scale to zoomer
    void setAxisScale (int axisId, double min, double max, double step = 0)
    {
        QwtPlot::setAxisScale(axisId, min, max, step);
        // autoscale must be set on both zoomer,
        // because they do not know each other.
        switch (static_cast<QwtPlot::Axis>(axisId))
        {
        case xBottom:
            this->zoomerY1()->setAxisScale(QwtPlot::xBottom, min, max);
            this->zoomerY2()->setAxisScale(QwtPlot::xBottom, min, max);
            break;
        case yLeft:
            this->zoomerY1()->setAxisScale(QwtPlot::yLeft, min, max);
            this->zoomerY2()->setAxisScale(QwtPlot::yLeft, min, max);
            break;
        case yRight:
            this->zoomerY1()->setAxisScale(QwtPlot::yRight, min, max);
            this->zoomerY2()->setAxisScale(QwtPlot::yRight, min, max);
            break;
        case xTop:
            this->zoomerY1()->setAxisScale(QwtPlot::xTop, min, max);
            this->zoomerY2()->setAxisScale(QwtPlot::xTop, min, max);
            break;
        case axisCnt: break;
        }
    }

    //! overwrites `QwtPlot::setAxisAutoScale`
    //! deletes scale to zoomer
    void setAxisAutoScale( int axisId, bool on = true )
    {
        QwtPlot::setAxisAutoScale(axisId, on);
        if (on) {
            // autoscale must be reset on both zoomer,
            // because they do not know each other.
            switch (static_cast<QwtPlot::Axis>(axisId))
            {
            case xBottom:
                this->zoomerY1()->setAxisAutoScale(QwtPlot::xBottom);
                this->zoomerY2()->setAxisAutoScale(QwtPlot::xBottom);
                break;
            case yLeft:
                this->zoomerY1()->setAxisAutoScale(QwtPlot::yLeft);
                this->zoomerY2()->setAxisAutoScale(QwtPlot::yLeft);
                break;
            case yRight:
                this->zoomerY1()->setAxisAutoScale(QwtPlot::yRight);
                this->zoomerY2()->setAxisAutoScale(QwtPlot::yRight);
                break;
            case xTop:
                this->zoomerY1()->setAxisAutoScale(QwtPlot::xTop);
                this->zoomerY2()->setAxisAutoScale(QwtPlot::xTop);
                break;
            case axisCnt: break;
            }
        }
    }

    //! Zoomer for Y1 axis
    //!
    //! \return pointer to QwtPlotZoomer
    QPlotZoomer * zoomerY1() const
    {
        return m_zoomerY1;
    }

    //! Zoomer for Y2 axis
    //!
    //! \return pointer to QwtPlotZoomer
    QPlotZoomer * zoomerY2() const
    {
        return m_zoomerY2;
    }

    //! Grid attached to QwtPlot
    //!
    //! \return pointer to QwtPlotZoomer
    QwtPlotGrid * grid() const
    {
        return m_grid;
    }

    //! Canvas used by QwtPlot
    //!
    //! The canvas pointer must not be changed in QwtPlot
    //! since it makes this pointer invalid and will
    //! leads to a crash of all zoomers.
    //!
    //! \return pointer to QwtPlotCanvas
    QwtPlotCanvas * canvas() const
    {
        return m_canvas;
    }

#ifndef QT_NO_PRINTER

    //! Print the plot
    //! \note not tested!
    void printPlot(QString filename = "plot")
    {
        QPrinter printer( QPrinter::HighResolution );
        printer.setOrientation( QPrinter::Landscape );
        printer.setOutputFileName( filename );

        QPrintDialog dialog( &printer );
        if ( dialog.exec() )
        {
            QwtPlotRenderer renderer;

            if ( printer.colorMode() == QPrinter::GrayScale )
            {
                renderer.setDiscardFlag( QwtPlotRenderer::DiscardBackground );
                renderer.setDiscardFlag( QwtPlotRenderer::DiscardCanvasBackground );
                renderer.setDiscardFlag( QwtPlotRenderer::DiscardCanvasFrame );
                renderer.setLayoutFlag( QwtPlotRenderer::FrameWithScales );
            }

            renderer.renderTo( this, printer );
        }
    }

#endif


private:
    QPlotZoomer * m_zoomerY1;
    QPlotZoomer * m_zoomerY2;
    QwtPlotGrid * m_grid;
    QwtPlotCanvas * m_canvas;
};

#endif // QBASEPLOTWIDGET_H
