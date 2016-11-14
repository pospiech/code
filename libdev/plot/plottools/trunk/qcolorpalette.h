/* -*- mode: C++ ; c-file-style: "stroustrup" -*- *****************************
 * Plot Tools Library (based on Qwt)
 * Copyright (C) 2016   Matthias Pospiech
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the Qwt License, Version 1.0
 *****************************************************************************/

#ifndef QCOLORPALETTE_H
#define QCOLORPALETTE_H

#include <vector>
#include <QColor>

/*!
    \class QColorPalette
    \brief Select colors according to a well definte color palette

    Implemented Palettes are
    - Standard,
    - StandardDark,
    - MSOffice2007,
    - Gnuplot,
    - Oxygen,
    - Pale

    Example:
    @code
    QLinePlot * plot = new QLinePlot();
    ...
    QPlotCurve curve = new QPlotCurve();
    ...
    curve->attach( plot );
    // curves must be attached to plot, otherwise
    // palette can not be applied (works in list of curves)
    plot->setColorPalette(QColorPalette::MSOffice2007);
    @endcode
 */

class QColorPalette
{
public:
    enum paletteIdentifier {
        Standard,
        StandardDark,
        MSOffice2007,
        Gnuplot,
        Oxygen,
        Pale
    };

    //! Create vector of QColors with selected palette
    //!
    //! \param id QColorPalette::paletteIdentifier
    //! \param totalnumber maximum number of used colors, palette is different for number of colors.
    static std::vector<QColor> colors(paletteIdentifier id = QColorPalette::Standard, size_t totalnumber = 0)
    {
        if (totalnumber > 6) { totalnumber = 6; }

        std::vector<QColor> colors;
        switch (id)
        {
        case Standard:
        {
            switch(totalnumber)
            {
            case 1:
                colors.push_back(QColor(Qt::blue));
                break;
            case 2:
                colors.push_back(QColor(Qt::blue));
                colors.push_back(QColor(Qt::red));
                break;
            case 3:
                colors.push_back(QColor(Qt::blue));
                colors.push_back(QColor(Qt::red));
                colors.push_back(QColor(Qt::green));
                break;
            default:
                colors.push_back(QColor(Qt::blue));
                colors.push_back(QColor(Qt::red));
                colors.push_back(QColor(Qt::green));
                colors.push_back(QColor(Qt::cyan));
                colors.push_back(QColor(Qt::magenta));
                colors.push_back(QColor(Qt::yellow));
                break;
            }
            break;
        }
        case StandardDark:
        {
            switch(totalnumber)
            {
            case 1:
                colors.push_back(QColor(Qt::darkBlue));
                break;
            case 2:
                colors.push_back(QColor(Qt::darkBlue));
                colors.push_back(QColor(Qt::darkRed));
                break;
            case 3:
                colors.push_back(QColor(Qt::darkBlue));
                colors.push_back(QColor(Qt::darkRed));
                colors.push_back(QColor(Qt::darkGreen));
                break;
            default:
                colors.push_back(QColor(Qt::darkBlue));
                colors.push_back(QColor(Qt::darkRed));
                colors.push_back(QColor(Qt::darkGreen));
                colors.push_back(QColor(Qt::darkCyan));
                colors.push_back(QColor(Qt::darkMagenta));
                colors.push_back(QColor(Qt::darkYellow));
                break;
            }
            break;
        }
        case Gnuplot:
        {
            colors.push_back(QColor(139,  0,  0));  //dark-red
            colors.push_back(QColor(0  ,100,  0));  //dark-green
            colors.push_back(QColor(0  ,0  ,139));  // dark-blue
            colors.push_back(QColor(0  ,139,139));  //dark-cyan
            colors.push_back(QColor(139,0  ,139));  //dark-magenta
            colors.push_back(QColor(0  ,206,209));  //dark-turquoise
            colors.push_back(QColor(255, 20,147));  //dark-pink
            colors.push_back(QColor(233,150,122));  //dark-salmon
            colors.push_back(QColor(189,183,107));  //dark-khaki
            colors.push_back(QColor(255,140,  0));  //dark-orange
            colors.push_back(QColor(148,  0,211));  //dark-violet
            colors.push_back(QColor(184,134, 11));  //dark-goldenrod
            break;
        }
        case MSOffice2007:
            colors.push_back(QColor(56, 93, 138));  // blue
            colors.push_back(QColor(140, 56, 54));  // red
            colors.push_back(QColor(113, 137, 63)); // green
            colors.push_back(QColor(92, 71, 118));  // lila
            colors.push_back(QColor(53, 125, 145)); // violett
            colors.push_back(QColor(182, 109, 49)); // orange
            colors.push_back(QColor(66, 109, 161));
            break;
        case Oxygen:
        {
            switch(totalnumber)
            {
            case 1:
                colors.push_back(QColor(0, 0, 191));     // blue
                break;
            case 2:
            case 3:
                colors.push_back(QColor(191, 0 , 0));    // red
                colors.push_back(QColor(0, 0, 191));     // blue
                colors.push_back(QColor(0, 140, 0));     // green
                colors.push_back(QColor(255, 191, 0));   // orange
                break;
            default:
                colors.push_back(QColor(191, 94, 0));    // brown
                colors.push_back(QColor(191, 0 , 0));    // red
                colors.push_back(QColor(204, 0, 154));   // rosa
                colors.push_back(QColor(90, 0, 179));    // lila
                colors.push_back(QColor(0, 0, 191));     // blue
                colors.push_back(QColor(0, 140, 0));     // green
                colors.push_back(QColor(139, 179, 0));   // green-orange
                colors.push_back(QColor(255, 191, 0));   // orange
                colors.push_back(QColor(136, 136, 136)); // gray
                break;
            }
            break;
        }
        case Pale:
        {
            colors.push_back(QColor(204, 68, 0));
            colors.push_back(QColor(64, 51, 204));
            colors.push_back(QColor(51, 204, 51));
            colors.push_back(QColor(204, 190, 0));
            colors.push_back(QColor(0, 119, 179));
            break;
        }
        }
        return colors;
    }

    //! Get n-th color of selected palette
    //!
    //! \param number selected index
    //! \param id QColorPalette::paletteIdentifier
    //! \param totalnumber maximum number of used colors, palette is different for number of colors.
    //!
    //! \sa colors
    static QColor color(size_t number, paletteIdentifier id = QColorPalette::Standard, size_t totalnumber = 0)
    {
        std::vector<QColor> colorPalette = QColorPalette::colors(id, totalnumber);
        size_t size = colorPalette.size();
        return colorPalette[number % size];
    }

};

#endif // QCOLORPALETTE_H
