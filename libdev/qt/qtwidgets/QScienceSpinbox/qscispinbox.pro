################################################################
# QScienceSpinBox Library
# Copyright (C) 2015   Matthias Pospiech
################################################################

TEMPLATE = subdirs
CONFIG   += ordered

SUBDIRS = src examples




## -------------------------------------------------
## Define Variables
## -------------------------------------------------
#BUILD_DIR = build
#BIN_DIR = bin
#BINARY_NAME = QSciSpinBoxExample

## #####################################################################
## release/debug mode
## If you want to build both DEBUG_SUFFIX and RELEASE_SUFFIX
## have to differ to avoid, that they overwrite each other.
## #####################################################################
## Enable debug_and_release + build_all if you want to build both.
#CONFIG += debug # release/debug/debug_and_release
#CONFIG -= release

## #####################################################################
## Auto select compiler
## #####################################################################
#COMPILER = other
#win32-g++:COMPILER = mingw
#win32-msvc*:COMPILER = msvc
#linux-g++:COMPILER = gcc


## #####################################################################
## Directories
## #####################################################################
#BIN_DIR = bin
#debug:DESTDIR = $${BIN_DIR}/$${COMPILER}/debug/
#release:DESTDIR = $${BIN_DIR}/$${COMPILER}/release/

#BUILD_DIR = build
#debug:OBJECTS_DIR = $${BUILD_DIR}/debug/
#release:OBJECTS_DIR = $${BUILD_DIR}/release/

#MOC_DIR = $${BUILD_DIR}/moc
#UI_DIR = $${BUILD_DIR}/ui


