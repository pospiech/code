################################################################
# QScienceSpinBox Library
# Copyright (C) 2015   Matthias Pospiech
################################################################

######################################################################
# Target Names
######################################################################

LIB_NAME          = QSciSpinBox
build_pass:CONFIG(debug, debug|release) {
    unix: LIB_NAME = $$join(LIB_NAME,,,_debug)
    else: LIB_NAME = $$join(LIB_NAME,,,d)
}

######################################################################
# QT
######################################################################

QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


######################################################################
# Install paths
######################################################################

PROJECT_INSTALL_PREFIX = $$[QT_INSTALL_PREFIX]

unix {
    PROJECT_INSTALL_PREFIX    = /usr/local/qscispinbox
}

win32 {
    PROJECT_INSTALL_PREFIX    = ./install
}

PROJECT_INSTALL_DOCS      = $${PROJECT_INSTALL_PREFIX}/doc
PROJECT_INSTALL_HEADERS   = $${PROJECT_INSTALL_PREFIX}/include
PROJECT_INSTALL_LIBS      = $${PROJECT_INSTALL_PREFIX}/lib

PROJECT_OUT_ROOT = $${OUT_PWD}/..

######################################################################
# Build the static/shared libraries.
# If QScienceSpinBoxDll is enabled, a shared library is built, otherwise
# it will be a static library.
######################################################################

BUILD_CONFIG           += QScienceSpinBoxDll
