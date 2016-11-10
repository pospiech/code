/* -*- mode: C++ ; c-file-style: "stroustrup" -*- *****************************
 * Plot Tools Library (based on Qwt)
 * Copyright (C) 2016   Matthias Pospiech
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the Qwt License, Version 1.0
 *****************************************************************************/

#ifndef QMATRIXPLOT_H
#define QMATRIXPLOT_H

#include <algorithm> //
#include <iterator> // std::begin

#include <qwt_plot_spectrogram.h>
#include <qwt_matrix_raster_data.h>
#include <qwt_interval.h>
#include <qwt_plot_layout.h>
#include <qwt_scale_widget.h>

#include "qcolormap.h"
#include "qbaseplotwidget.h"


/*!
    \class QMatrixPlot
    \brief Create false color plots of 2d data (pcolor plots in matlab)

    Example:
    @code
    QMatrixPlot * plotMatrix = new QMatrixPlot();

    plotMatrix->setTitle( "2D Plot Demo" );
    plotMatrix->setAxisTitle (QwtPlot::xBottom, "x-axis");
    plotMatrix->setAxisTitle (QwtPlot::yLeft, "y-axis");
    plotMatrix->setAxisTitle (QwtPlot::yRight, "signal amplitude");

    plotMatrix->setMatrixData(data,
                        xaxis.size(),
                        QwtInterval(xaxis.front(), xaxis.back()),
                        QwtInterval(yaxis.front(), yaxis.back()));

    // enable interpolation, otherwise use QwtMatrixRasterData::NearestNeighbour
    plotMatrix->setResampleMode(QwtMatrixRasterData::BilinearInterpolation);

    // enable contour lines
    plotMatrix->setContourSteps(11);
    plotMatrix->showContour( true );

    // set color map
    plotMatrix->setColorMap( QColorMap::Jet );

    plotMatrix->replot();
    @endcode
 */

class QMatrixPlot : public QBasePlotWidget
{
    Q_OBJECT
public:
    QMatrixPlot(QWidget *parent = NULL):
        QBasePlotWidget( parent ),
        d_alpha(255)
    {

        // create QwtPlotSpectrogram
        d_spectrogram = new QwtPlotSpectrogram();
        d_spectrogram->setRenderThreadCount( 0 ); // use system specific thread count
        d_spectrogram->setCachePolicy( QwtPlotRasterItem::PaintCache );

        // create QwtMatrixRasterData
        m_MatrixRasterData = new QwtMatrixRasterData();
        // set empty data, otherwise other functions fail
        d_spectrogram->setData(m_MatrixRasterData);
        d_spectrogram->attach( this );

        enableAxis( QwtPlot::yRight );
        // A color bar on the right axis
        QwtScaleWidget *rightAxis = axisWidget( QwtPlot::yRight );
        rightAxis->setColorBarEnabled( true );        

        // requires a valid dataset
        setColorMap( QColorMap::Jet );
    }
    //! Destructor: delete QwtPlotSpectrogram and QwtMatrixRasterData instances
    virtual ~QMatrixPlot()
    {
        if (d_spectrogram)
            delete d_spectrogram;
        // is already deleted by Destructor of QwtPlotSpectrogram
        //if (m_MatrixRasterData)
        //    delete m_MatrixRasterData;
    }

    /*!
       Set data with number of columns (rows are calculated)
       and create plot with x and y axis filled with matrix indices.

       \param values vector with size = numColumns x numRows
       \param numColumns number of columns

       \sa setMatrixData(const QVector< double > &values, int numColumns, QwtInterval xAxisRange, QwtInterval yAxisRange)
    */
    void setMatrixData(const QVector< double > &values, int numColumns)
    {
        size_t numRows = values.size()/numColumns;
        QwtInterval xAxisRange = QwtInterval( 0, numColumns );
        QwtInterval yAxisRange = QwtInterval( 0, numRows );
        setMatrixData(values, numColumns, xAxisRange, yAxisRange);
    }
    /*!
       Set data with number of columns (rows are calculated)
       and create plot with x and y axis according to the given
       x and y ranges.

       \param values vector with size = numColumns x numRows
       \param numColumns number of columns
       \param xAxisRange range (min, max) for creation of x-axis
       \param yAxisRange range (min, max) for creation of y-axis
    */
    void setMatrixData(const QVector< double > &values, int numColumns, QwtInterval xAxisRange, QwtInterval yAxisRange)
    {
        m_MatrixRasterData->setInterval( Qt::XAxis, xAxisRange );
        m_MatrixRasterData->setInterval( Qt::YAxis, yAxisRange );

        double minValue = *std::min_element( std::begin(values), std::end(values) );
        double maxValue = *std::max_element( std::begin(values), std::end(values) );
        m_MatrixRasterData->setInterval( Qt::ZAxis, QwtInterval(minValue, maxValue) );

        m_MatrixRasterData->setValueMatrix (values, numColumns);        
        d_spectrogram->setData( m_MatrixRasterData );

        const QwtInterval zInterval = d_spectrogram->data()->interval( Qt::ZAxis );
        setAxisScale( QwtPlot::yRight, zInterval.minValue(), zInterval.maxValue() );

        QwtScaleWidget *axis = axisWidget( QwtPlot::yRight );
        axis->setColorMap( zInterval, QColorMap::map(d_mapType) );

        setContourSteps(11);
    }


public slots:

    /*! enable contour lines

       \param on switch on/off
     */
    void showContour( bool on )
    {
        d_spectrogram->setDisplayMode( QwtPlotSpectrogram::ContourMode, on );
        replot();
    }

    /*! enable spectrogram plot

       \param on switch on/off
     */
    void showSpectrogram( bool on )
    {
        d_spectrogram->setDisplayMode( QwtPlotSpectrogram::ImageMode, on );
        d_spectrogram->setDefaultContourPen(
            on ? QPen( Qt::black, 0 ) : QPen( Qt::NoPen ) );

        replot();
    }
    //! set alpha (transparency) value
    //!
    //! \param alpha value for transparency
    void setAlpha( int alpha )
    {
        // setting an alpha value doesn't make sense in combination
        // with a color map interpolating the alpha value

        d_alpha = alpha;
        if ( d_mapType != QColorMap::Alpha )
        {
            d_spectrogram->setAlpha( alpha );
            replot();
        }
    }

    //! set contour levels
    //!
    //! \param contourLevels
    void setContourLevels(QList<double> contourLevels)
    {
        d_spectrogram->setContourLevels( contourLevels );
    }

    //! create contour levels based on number of steps and data min - max value
    //!
    //! \param steps number of steps
    void setContourSteps(size_t steps)
    {
        QList<double> contourLevels;
        const QwtInterval zInterval = d_spectrogram->data()->interval( Qt::ZAxis );
        const double stepSize = (zInterval.maxValue()-zInterval.minValue())/(steps-1);

        double level = zInterval.minValue();
        for ( size_t i = 0; i < steps; ++i )
        {
            contourLevels += level;
            level = level + stepSize;
        }
        this->setContourLevels( contourLevels );

        d_spectrogram->setContourLevels( contourLevels );
    }


    //! set color map for data display and colorbar
    //!
    //! \param type ColorMap::ColorMapType
    void setColorMap( int type )
    {
        d_mapType = type;

        d_spectrogram->setColorMap( QColorMap::map(type) );

        QwtScaleWidget *axis = axisWidget( QwtPlot::yRight );
        const QwtInterval zInterval = d_spectrogram->data()->interval( Qt::ZAxis );
        axis->setColorMap( zInterval, QColorMap::map(type) );

        int alpha = d_alpha;
        if (type == QColorMap::Alpha)
        {
             alpha = 255;
        }
        d_spectrogram->setAlpha( alpha );

        replot();
    }

    //! set resample mode (interpolate or not)
    //!
    //! \param mode QwtMatrixRasterData::ResampleMode
    void setResampleMode(QwtMatrixRasterData::ResampleMode mode)
    {
        m_MatrixRasterData->setResampleMode(mode);
    }


private:
    QwtPlotSpectrogram  * d_spectrogram;
    QwtMatrixRasterData * m_MatrixRasterData;

    int d_alpha;
    int d_mapType;
};


#endif // QMATRIXPLOT_H
