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
    examples/plottoolstest.cpp \
    examples/functions/qplotfunction.cpp


HEADERS += \
    src/qplotcurve.h \
    src/qlineplot.h \
    src/qplotzoomer.h \
    src/qcolorpalette.h \
    src/qbaseplotwidget.h \
    src/qcolormap.h \
    src/qmatrixplot.h \
    examples/functions/qplotfunction.h \
    examples/functions/constants.h \
    examples/functions/functions.h

INCLUDEPATH += $${PROJECT_ROOT}/examples/functions
INCLUDEPATH += $${PROJECT_ROOT}/src

DISTFILES += \
    docs/plottools.dox
