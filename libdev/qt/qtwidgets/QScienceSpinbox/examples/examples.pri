################################################################
# QScienceSpinBox Library
# Copyright (C) 2015   Matthias Pospiech
################################################################

PROJECT_ROOT = $${PWD}/..
include( $${PROJECT_ROOT}/config.pri )
include( $${PROJECT_ROOT}/build.pri )
# include( $${PROJECT_ROOT}/functions.pri )

PROJECT_OUT_ROOT = $${OUT_PWD}/../..

INCLUDEPATH += $${PROJECT_ROOT}/src
DEPENDPATH  += $${PROJECT_ROOT}/src

# #####################################################################
# Config
# #####################################################################

TEMPLATE = app
CONFIG(debug, debug|release):CONFIG += console


######################################################################
# DESTDIR -> binary
######################################################################

!debug_and_release {

    DESTDIR      = $${PROJECT_OUT_ROOT}/examples/bin
}
else {
    CONFIG(debug, debug|release) {

        DESTDIR      = $${PROJECT_OUT_ROOT}/examples/bin_debug
    }
    else {

        DESTDIR      = $${PROJECT_OUT_ROOT}/examples/bin
    }
}

QMAKE_RPATHDIR *= $${PROJECT_OUT_ROOT}/lib


######################################################################
# DLL Loading
######################################################################
win32 {
    contains(BUILD_CONFIG, QScienceSpinBoxDll) {
        DEFINES    += QT_DLL QSCIENCESPINBOX_DLL
    }
}

######################################################################
# lib Loading
######################################################################

LIBRARYPATH     = $${PROJECT_OUT_ROOT}/lib

win32{
    LIBS += -L$${LIBRARYPATH} -l$${LIB_NAME}
}


######################################################################
# Include PATH
######################################################################

INCLUDEPATH += $${BUILD_DIR} \
    $${MOC_DIR} \
    $${UI_DIR} \

