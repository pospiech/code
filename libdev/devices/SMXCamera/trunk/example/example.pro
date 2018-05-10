include( ../config.pri )
BINARY_NAME = QSMXCameraExample
TEMPLATE = app
debug:CONFIG += console

# #####################################################################
# set Target Name
# #####################################################################
CONFIG(debug, debug|release):win32:BINARY_NAME = $$join(BINARY_NAME,,,d)
TARGET = $${BINARY_NAME}

# #####################################################################
# Directories
# #####################################################################
BIN_DIR = bin
CONFIG(debug, debug|release):DESTDIR = $${BIN_DIR}/$${COMPILER}/debug/
CONFIG(release, debug|release):DESTDIR = $${BIN_DIR}/$${COMPILER}/release/

BUILD_DIR = build
CONFIG(debug, debug|release):OBJECTS_DIR = $${BUILD_DIR}/debug/
CONFIG(release, debug|release):OBJECTS_DIR = $${BUILD_DIR}/release/

MOC_DIR = $${BUILD_DIR}/moc
UI_DIR = $${BUILD_DIR}/ui

# #####################################################################
# DEFINES
# #####################################################################
win32:CameraSMXDll:DEFINES += SMXCAMERA_DLL
win32:CameraSMXDll:DEFINES += CAMERAIMAGE_DLL

# #####################################################################
# Include directories
# #####################################################################
INCLUDEPATH += $${BUILD_DIR} \
    $${MOC_DIR} \
    $${UI_DIR} \
    src \
    ../include \
    ../src

# #####################################################################
# Libaries
# #####################################################################
CONFIG(debug, debug|release):LIBS += -L../lib/$${COMPILER}/debug/ \
    -l$${LIBRARY_NAME}
CONFIG(release, debug|release):LIBS += -L../lib/$${COMPILER}/release/ \
    -l$${LIBRARY_NAME}

# #####################################################################
# Sources
# #####################################################################
include( sourceExample.pri )
SOURCEPATH = ../src

