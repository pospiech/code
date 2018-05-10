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
debug:DESTDIR = $${BIN_DIR}/$${COMPILER}/debug/
release:DESTDIR = $${BIN_DIR}/$${COMPILER}/release/

BUILD_DIR = build
debug:OBJECTS_DIR = $${BUILD_DIR}/debug/
release:OBJECTS_DIR = $${BUILD_DIR}/release/

EXAMPLE_DIR = ../example/bin
debug:DLLDESTDIR = $${EXAMPLE_DIR}/$${COMPILER}/debug/
release:DLLDESTDIR = $${EXAMPLE_DIR}/$${COMPILER}/release/

MOC_DIR = $${BUILD_DIR}/moc
UI_DIR = $${BUILD_DIR}/ui

# #####################################################################
# CONFIG
# #####################################################################
contains(CONFIG, uEyeCameraDll):CONFIG += dll
else:CONFIG += staticlib
# CONFIG += dll

# #####################################################################
# DEFINES
# #####################################################################
win32:uEyeCameraDll:DEFINES += UEYECAMERA_MAKEDLL
win32:uEyeCameraDll:DEFINES += CAMERAIMAGE_MAKEDLL

# #####################################################################
# FILES
# #####################################################################
# -------------------------------------------------
# Libaries
# -------------------------------------------------
LIBS += -L$${PROJECT_ROOT}/lib/uEye -luEye_api

# -------------------------------------------------
# Include directories
# -------------------------------------------------
INCLUDEPATH += ../include/ \
    $${UI_DIR}
SOURCEPATH = .
include( sourcelib.pri )
