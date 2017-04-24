CONFIG += qt
CONFIG += warn_on
CONFIG += thread

QT += core gui
QT += opengl

greaterThan(QT_MAJOR_VERSION, 4) {
    QT += widgets
    QT += printsupport
}

# #####################################################################
# DLL Defines
# #####################################################################


win32 {
    DEFINES += QWT_DLL QT_DLL
}


# #####################################################################
# Variables
# #####################################################################

PROJECT_ROOT = $${PWD}


# #####################################################################
# release/debug mode
# If you want to build both DEBUG_SUFFIX and RELEASE_SUFFIX
# have to differ to avoid, that they overwrite each other.
# #####################################################################
# On Windows you can't mix release and debug libraries.
# Enable debug_and_release + build_all if you want to build both.
CONFIG += debug # release/debug/debug_and_release
CONFIG -= release

#CONFIG           += debug_and_release
#CONFIG           += build_all


# #####################################################################
# Directories
# #####################################################################
BIN_DIR = $${PROJECT_ROOT}/bin/
CONFIG(debug, debug|release):DESTDIR = $${PROJECT_ROOT}/bin_debug/
CONFIG(release, debug|release):DESTDIR = $${PROJECT_ROOT}/bin/

BUILD_DIR = build
CONFIG(debug, debug|release):OBJECTS_DIR = $${BUILD_DIR}/debug/
CONFIG(release, debug|release):OBJECTS_DIR = $${BUILD_DIR}/release/

MOC_DIR = $${BUILD_DIR}/moc
UI_DIR = $${BUILD_DIR}/ui

# #####################################################################
# FILES
# #####################################################################

# D:\Git\code\libdev\plot\plottools\trunk
# D:\Git\code\..\..\..\..
# D:\Git\code\libs\qwt-6.1.3\

QWT_PATH = $${PWD}/../../../../libs/qwt-6.1.3

win32 {
CONFIG(debug, debug|release):LIBS += -L$${QWT_PATH}/lib/ -lqwtd
CONFIG(release, debug|release):LIBS += -L$${QWT_PATH}/lib/ -lqwt
}

# #####################################################################
# INCLUDEPATH
# #####################################################################

INCLUDEPATH += $${QWT_PATH}/src

