################################################################
# fftwcpp Library
# Copyright (C) 2016   Matthias Pospiech
################################################################

PROJECT_ROOT = $${PWD}/..
EXAMPLES_ROOT = $${PWD}/

PROJECT_OUT_ROOT = $${OUT_PWD}/../..

INCLUDEPATH += $${PROJECT_ROOT}/src
DEPENDPATH  += $${PROJECT_ROOT}/src


######################################################################
# release/debug mode
######################################################################

#win32 {
#    # On Windows you can't mix release and debug libraries.
#    # For your own application development you might need a debug version.
#    # Enable debug_and_release + build_all if you want to build both.

#    CONFIG           += debug_and_release
#    CONFIG           += build_all
#}
#else {

#    CONFIG           += release
#}
#CONFIG           += release
#CONFIG           -= debug

# #####################################################################
# Config
# #####################################################################

TEMPLATE = app
CONFIG(debug, debug|release):CONFIG += console

######################################################################
# QT
######################################################################

QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport


######################################################################
# paths for building the project
######################################################################

BUILD_DIR = $${PWD}/build

MOC_DIR             = $${BUILD_DIR}/moc
UI_DIR              = $${BUILD_DIR}/ui
RCC_DIR             = $${BUILD_DIR}/resources

!debug_and_release {
    # in case of debug_and_release object files
    # are built in the release and debug subdirectories
    OBJECTS_DIR       = $${BUILD_DIR}
}

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
#win32 {
#	DEFINES    += QT_DLL FFTW_DLL
#}

######################################################################
# lib Loading
######################################################################

LIBRARYPATH     = $${PROJECT_OUT_ROOT}/lib

win32{
    # LIBS += -L$${LIBRARYPATH} -l$${LIB_NAME}
}


######################################################################
# Include PATH
######################################################################

INCLUDEPATH += $${BUILD_DIR} \
    $${MOC_DIR} \
    $${UI_DIR} \
    functions gui

INCLUDEPATH += $${PROJECT_ROOT}/include/fftw
