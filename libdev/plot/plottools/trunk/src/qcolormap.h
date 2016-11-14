/* -*- mode: C++ ; c-file-style: "stroustrup" -*- *****************************
 * Plot Tools Library (based on Qwt)
 * Copyright (C) 2016   Matthias Pospiech
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the Qwt License, Version 1.0
 *****************************************************************************/

#ifndef QCOLORMAP_H
#define QCOLORMAP_H

#include "qwt_color_map.h"
#include "qwt_interval.h"


/*!
    \brief Colormap: Jet

    Colormap similar to the Jet colormap in Matlab.
    Colors from blue to red.
    Derived from QwtLinearColorMap.
 */
class JetColorMapRGB: public QwtLinearColorMap
{
public:
    JetColorMapRGB():
        QwtLinearColorMap( QColor(0,0,189), QColor(132,0,0), QwtColorMap::RGB )
    {
        double pos;
        pos = 1.0/13.0*1.0; this->addColorStop(pos, QColor(0,0,255));
        pos = 1.0/13.0*2.0; this->addColorStop(pos, QColor(0,66,255));
        pos = 1.0/13.0*3.0; this->addColorStop(pos, QColor(0,132,255));
        pos = 1.0/13.0*4.0; this->addColorStop(pos, QColor(0,189,255));
        pos = 1.0/13.0*5.0; this->addColorStop(pos, QColor(0,255,255));
        pos = 1.0/13.0*6.0; this->addColorStop(pos, QColor(66,255,189));
        pos = 1.0/13.0*7.0; this->addColorStop(pos, QColor(132,255,132));
        pos = 1.0/13.0*8.0; this->addColorStop(pos, QColor(189,255,66));
        pos = 1.0/13.0*9.0; this->addColorStop(pos, QColor(255,255,0));
        pos = 1.0/13.0*10.0; this->addColorStop(pos, QColor(255,189,0));
        pos = 1.0/13.0*12.0; this->addColorStop(pos, QColor(255,66,0));
        pos = 1.0/13.0*13.0; this->addColorStop(pos, QColor(189,0,0));
    }
};


/*!
    \brief Colormap: Gray

    Colormap with colors from black to white
    Derived from QwtLinearColorMap.
 */
class GrayColorMapRGB: public QwtLinearColorMap
{
public:
    GrayColorMapRGB():
        QwtLinearColorMap( Qt::black, Qt::white, QwtColorMap::RGB )
    {
    }
};

/*!
    \brief Colormap: Linear

    Colormap with colors from cyan - green - yellow - red
    Derived from QwtLinearColorMap.
 */
class LinearColorMapRGB: public QwtLinearColorMap
{
public:
    LinearColorMapRGB():
        QwtLinearColorMap( Qt::darkCyan, Qt::red, QwtColorMap::RGB )
    {
        addColorStop( 0.1, Qt::cyan );
        addColorStop( 0.6, Qt::green );
        addColorStop( 0.95, Qt::yellow );
    }
};

/*!
    \brief Colormap: Linear (indexed)

    Colormap with colors from cyan - green - yellow - red
    Derived from QwtLinearColorMap.
 */
class LinearColorMapIndexed: public QwtLinearColorMap
{
public:
    LinearColorMapIndexed():
        QwtLinearColorMap( Qt::darkCyan, Qt::red, QwtColorMap::Indexed )
    {
        addColorStop( 0.1, Qt::cyan );
        addColorStop( 0.6, Qt::green );
        addColorStop( 0.95, Qt::yellow );
    }
};


/*!
    \brief Colormap: Hue

    Derived from QwtLinearColorMap.
 */
class HueColorMap: public QwtColorMap
{
public:
    // class backported from Qwt 6.2

    HueColorMap():
        d_hue1(0),
        d_hue2(359),
        d_saturation(150),
        d_value(200)
    {
        updateTable();
    }

    virtual QRgb rgb( const QwtInterval &interval, double value ) const
    {
        if ( qIsNaN(value) )
            return 0u;

        const double width = interval.width();
        if ( width <= 0 )
            return 0u;

        if ( value <= interval.minValue() )
            return d_rgbMin;

        if ( value >= interval.maxValue() )
            return d_rgbMax;

        const double ratio = ( value - interval.minValue() ) / width;
        int hue = d_hue1 + qRound( ratio * ( d_hue2 - d_hue1 ) );

        if ( hue >= 360 )
        {
            hue -= 360;

            if ( hue >= 360 )
                hue = hue % 360;
        }

        return d_rgbTable[hue];
    }

    virtual unsigned char colorIndex( const QwtInterval &, double ) const
    {
        // we don't support indexed colors
        return 0;
    }


private:
    void updateTable()
    {
        for ( int i = 0; i < 360; i++ )
            d_rgbTable[i] = QColor::fromHsv( i, d_saturation, d_value ).rgb();

        d_rgbMin = d_rgbTable[ d_hue1 % 360 ];
        d_rgbMax = d_rgbTable[ d_hue2 % 360 ];
    }

    int d_hue1, d_hue2, d_saturation, d_value;
    QRgb d_rgbMin, d_rgbMax, d_rgbTable[360];
};

/*!
    \brief Colormap: Alpha

    Derived from QwtAlphaColorMap.
 */
class AlphaColorMap: public QwtAlphaColorMap
{
public:
    AlphaColorMap()
    {
        //setColor( QColor("DarkSalmon") );
        setColor( QColor("SteelBlue") );
    }
};


/*!
    \class QColorMap
    \brief Factory for QwtColorMap colormaps.

    Example:
    @code
    QwtColorMap * map = QColorMap::map( QColorMap::Jet );
    @endcode
 */
class QColorMap
{
public:
    enum ColorMap
    {
        Standard,
        RGB, // equal Standard
        Gray,
        Jet, // blue -> red
        Index,
        Hue,
        Alpha
    };

public:
    /*!
       Static class funcrion to build a color map according to the given map type

       \param maptype Selector for the map to be created
    */
    static QwtColorMap * map(int maptype = ColorMap::Standard)
    {
        switch( maptype )
        {
        case ColorMap::Hue:   { return new HueColorMap(); break; }
        case ColorMap::Alpha: { return new AlphaColorMap(); break; }
        case ColorMap::Index: { return new LinearColorMapIndexed(); break; }
        case ColorMap::Jet:   { return new JetColorMapRGB(); break; }
        case ColorMap::Gray:  { return new GrayColorMapRGB(); break; }
        case ColorMap::RGB:
        case ColorMap::Standard:
        default:              { return new LinearColorMapRGB(); break; }
        }
    }
};

#endif // QCOLORMAP_H
