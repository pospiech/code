include( ../config.pri )
BINARY_NAME = uEyeCameraExample
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
debug:DESTDIR = $${BIN_DIR}/$${COMPILER}/debug/
release:DESTDIR = $${BIN_DIR}/$${COMPILER}/release/

BUILD_DIR = build
debug:OBJECTS_DIR = $${BUILD_DIR}/debug/
release:OBJECTS_DIR = $${BUILD_DIR}/release/

MOC_DIR = $${BUILD_DIR}/moc
UI_DIR = $${BUILD_DIR}/ui

# #####################################################################
# DEFINES
# #####################################################################
win32:uEyeCameraDll:DEFINES += UEYECAMERA_DLL
win32:uEyeCameraDll:DEFINES += CAMERAIMAGE_DLL

# #####################################################################
# FILES
# #####################################################################
# -------------------------------------------------
# Include directories
# -------------------------------------------------
INCLUDEPATH += $${BUILD_DIR} \
    $${MOC_DIR} \
    $${UI_DIR} \
    src \
    ../include \
    ../src

# -------------------------------------------------
# Libaries
# -------------------------------------------------
debug:LIBS += -L../lib/$${COMPILER}/debug/ -l$${LIBRARY_NAME}
release:LIBS += -L../lib/$${COMPILER}/release/ -l$${LIBRARY_NAME}

# -------------------------------------------------
# Sources
# -------------------------------------------------

include( sourceExample.pri )

SOURCEPATH = ../src
# include( ../src/sourcelib.pri )
