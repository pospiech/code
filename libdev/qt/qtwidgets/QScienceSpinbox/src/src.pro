################################################################
# QScienceSpinBox Library
# Copyright (C) 2015   Matthias Pospiech
################################################################

# qmake project file for building the libraries

PROJECT_ROOT = $${PWD}/..
include( $${PROJECT_ROOT}/config.pri )
include( $${PROJECT_ROOT}/build.pri )
# include( $${PROJECT_ROOT}/functions.pri )

######################################################################
# TEMPLATE
######################################################################

TEMPLATE          = lib

######################################################################
# TARGET
######################################################################
TARGET            = $${LIB_NAME}

######################################################################
# DESTDIR
######################################################################

DESTDIR           = $${PROJECT_OUT_ROOT}/lib

######################################################################
# Configuration (dll)
######################################################################


contains(BUILD_CONFIG, QScienceSpinBoxDll) {
    CONFIG += dll
    win32: DEFINES += QT_DLL QSCIENCESPINBOX_DLL QSCIENCESPINBOX_MAKEDLL
}
else {
    CONFIG += staticlib
} 

######################################################################
# Source files
######################################################################

include ( $${PWD}/src.pri )

######################################################################
# Install directives
######################################################################

target.path    = $${PROJECT_INSTALL_LIBS}
INSTALLS       = target 

