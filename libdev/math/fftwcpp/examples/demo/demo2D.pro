################################################################
# fftwcpp Library
# Copyright (C) 2016   Matthias Pospiech
################################################################

include( ../examples.pri )

# #####################################################################
# set Target Name
# #####################################################################

TARGET  = QFFTDemo2D
CONFIG(debug, debug|release):win32:TARGET = $$join(TARGET,,,d)

# #####################################################################
# qwt
# #####################################################################

DEFINES += QWT_DLL

# C:\Users\matthias\Documents\svn\CPP\libdev\math\eigen_fft\fftwcpp\examples\demo
# C:\Users\matthias\Documents\svn\CPP\libs\qwt-6.1.3

message($${PWD})
# D:/Git/code/libdev/math/fftwcpp/examples/demo
# D:/Git/code/../../../../..
# D:\Git\code\libs\qwt-6.1.3\src

# QWT_PATH = C:/Users/matthias/Documents/svn/CPP/libs/qwt-6.1.3
QWT_PATH = $${PWD}/../../../../../libs/qwt-6.1.3


win32 {
CONFIG(debug, debug|release):LIBS += -L$${QWT_PATH}/lib/ -lqwtd
CONFIG(release, debug|release):LIBS += -L$${QWT_PATH}/lib/ -lqwt
}

INCLUDEPATH += $${QWT_PATH}/src

# #####################################################################
# qwt plot tools
# #####################################################################

#C:\Git\code\libdev\plot\plottools\trunk\src
#C:\Git\code\libdev\math\fftwcpp\examples\demo\plottools
# INCLUDEPATH += $${EXAMPLES_ROOT}../../../plot/plottools/trunk/src

# D:/Git/code/libdev/math/fftwcpp/examples/demo
# D:/Git/code/libdev/../../../..
# D:\Git\code\libdev\plot\plottools\trunk\src

INCLUDEPATH += $${PWD}/../../../../plot/plottools/trunk/src

# INCLUDEPATH += C:/Git/code/libdev/plot/plottools/trunk/src

# #####################################################################
# Includes
# #####################################################################

INCLUDEPATH += $${EXAMPLES_ROOT}/demo/gui
INCLUDEPATH += $${EXAMPLES_ROOT}/demo/functions
INCLUDEPATH += $${EXAMPLES_ROOT}/demo/math

######################################################################
# Libs
######################################################################

LIBS += -L$${PROJECT_ROOT}/lib/fftw/double/ -llibfftw3-3


# #####################################################################
# Sources
# #####################################################################


SOURCES += main.cpp \		
    gui/mainwindow2D.cpp \
    calculationmanager.cpp \
    gui/plotcomplexdata.cpp

HEADERS += gui/mainwindow2D.h \
    calculationmanager.h \
    functions/examplefunctions.h \
    functions/complexfunctionfactory.h \
    functions/functionfactory.h \
    gui/layouts.h \
    math/sgn.h \
    math/phaseunwrap.h \
    gui/plotcomplexdata.h

# add header files for moc
HEADERS += ../../../../plot/plottools/trunk/src/qbaseplotwidget.h \
    ../../../../plot/plottools/trunk/src/qlineplot.h \
    ../../../../plot/plottools/trunk/src/qplotzoomer.h \
    ../../../../plot/plottools/trunk/src/qmatrixplot.h


#    plottools/qbaseplotwidget.h \
#    plottools/qcolormap.h \
#    plottools/qcolorpalette.h \
#    plottools/qlineplot.h \
#    plottools/qmatrixplot.h \
#    plottools/qplotcurve.h \
#    plottools/qplotzoomer.h \

FORMS 	+= gui/mainwindow.ui
    #gui/plotcomplexdata.ui

