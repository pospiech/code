#-------------------------------------------------
#
# Project created by QtCreator 2018-12-14T09:41:09
#
#-------------------------------------------------

QT       += core gui
QT       += charts
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SpectrumViewer
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

# -------------------------------------------------
# libs
# -------------------------------------------------

win32-g++:LIBS += $${PWD}/lib/avantes/libcolcalc.a
win32-g++:LIBS += $${PWD}/lib/avantes/libAVSIrrad.a
win32-g++:LIBS += $${PWD}/lib/avantes/libavaspec.a
win32-g++:LIBS += $${PWD}/lib/cutelogger/mingw/libCuteLogger.a

win32-msvc*:LIBS += $${PWD}/lib/avantes/colcalc.lib
win32-msvc*:LIBS += $${PWD}/lib/avantes/AVSIrrad.lib
win32-msvc*:LIBS += $${PWD}/lib/avantes/avaspec.lib
win32-msvc*:LIBS += $${PWD}/lib/cutelogger/release/CuteLogger.lib
# -------------------------------------------------
# Include directories
# -------------------------------------------------
INCLUDEPATH += include/
INCLUDEPATH += include/cutelogger
INCLUDEPATH += include/avantes/
INCLUDEPATH += src/
INCLUDEPATH += src/devices/
INCLUDEPATH += src/gui/
INCLUDEPATH += src/log/

SOURCES += \
        src\main.cpp \
        src\mainwindow.cpp \
        src/devices/avantesspectrometer.cpp \
        src/applicationcallbacks.cpp \
        src/log/signalappender.cpp \
    src/gui/widgetspectrumplot.cpp

HEADERS += \
        src\mainwindow.h \
        src/devices/avantesspectrometer.h \
        src/applicationcallbacks.h \
        src/log/signalappender.h \
    src/gui/widgetspectrumplot.h

FORMS += \
        src\mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# #####################################################################
# DEFINES
# #####################################################################
win32 {
    DEFINES += NOMINMAX
    DEFINES += _CRT_SECURE_NO_WARNINGS
}


# #####################################################################
# Directories
# #####################################################################
PROJECT_ROOT = $${PWD}

BIN_DIR = $${PROJECT_ROOT}/bin/msvc
BUILD_DIR = $${PROJECT_ROOT}/build/

#debug:DESTDIR = $${BIN_DIR}/debug/
#release:DESTDIR = $${BIN_DIR}/release/

#debug:OBJECTS_DIR = $${BUILD_DIR}/debug/
#release:OBJECTS_DIR = $${BUILD_DIR}/release/

MOC_DIR = $${BUILD_DIR}/moc
UI_DIR = $${BUILD_DIR}/ui
