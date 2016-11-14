################################################################
# Plot Tools Library bases on Qwt Widget Library
# Copyright (C) 2016   Matthias Pospiech
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the LGPL License, Version 1.0
################################################################

include( examples.pri )

TARGET = PlotToolsExamples

SOURCES = \
    examples\plottoolstest.cpp \
    examples/functions/qplotfunction.cpp


HEADERS += \
    qplotcurve.h \
    qlineplot.h \
    qplotzoomer.h \
    qcolorpalette.h \
    qbaseplotwidget.h \
    qcolormap.h \
    qmatrixplot.h \
    examples/functions/qplotfunction.h \
    examples/functions/constants.h \
    examples/functions/functions.h

INCLUDEPATH += $${PROJECT_ROOT}/examples/functions

DISTFILES += \
    docs/plottools.dox
