include( ../config.pri )
PROJECT_ROOT = ..
TEMPLATE = lib

# #####################################################################
# set Target Name
# #####################################################################
TARGET = $${LIBRARY_NAME}

# #####################################################################
# Directories
# #####################################################################
BIN_DIR = $${PROJECT_ROOT}/lib
CONFIG(debug, debug|release):DESTDIR = $${BIN_DIR}/$${COMPILER}/debug/
CONFIG(release, debug|release)::DESTDIR = $${BIN_DIR}/$${COMPILER}/release/

BUILD_DIR = build
CONFIG(debug, debug|release):OBJECTS_DIR = $${BUILD_DIR}/debug/
CONFIG(release, debug|release):OBJECTS_DIR = $${BUILD_DIR}/release/

EXAMPLE_DIR = ../example/bin
CONFIG(debug, debug|release):DLLDESTDIR = $${EXAMPLE_DIR}/$${COMPILER}/debug/
CONFIG(release, debug|release):DLLDESTDIR = $${EXAMPLE_DIR}/$${COMPILER}/release/

MOC_DIR = $${BUILD_DIR}/moc
UI_DIR = $${BUILD_DIR}/ui

# #####################################################################
# CONFIG
# #####################################################################
contains(CONFIG, CameraSMXDll):CONFIG += dll
else:CONFIG += staticlib
# CONFIG += dll

# #####################################################################
# DEFINES
# #####################################################################
win32:CameraSMXDll:DEFINES += SMXCAMERA_MAKEDLL
win32:CameraSMXDll:DEFINES += CAMERAIMAGE_MAKEDLL

# #####################################################################
# Libaries
# #####################################################################
LIBS += -L../lib/Sumix/SMX150 -lSMX150

# #####################################################################
# Include directories
# #####################################################################
INCLUDEPATH += ../include/ \
    $${UI_DIR}

# #####################################################################
# Sources
# #####################################################################
SOURCEPATH = .
include( sourcelib.pri )
