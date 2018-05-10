include( ../config.pri )
PROJECT_ROOT = ..

# #####################################################################
# Directories
# #####################################################################
BIN_DIR = $${PROJECT_ROOT}/lib
debug:DESTDIR = $${BIN_DIR}/debug/
release:DESTDIR = $${BIN_DIR}/release/

BUILD_DIR = build
debug:OBJECTS_DIR = $${BUILD_DIR}/debug/
release:OBJECTS_DIR = $${BUILD_DIR}/release/

EXAMPLE_DIR = ../example/bin
debug:DLLDESTDIR = $${EXAMPLE_DIR}/debug/
release:DLLDESTDIR = $${EXAMPLE_DIR}/release/

MOC_DIR = $${BUILD_DIR}/moc
UI_DIR = $${BUILD_DIR}/ui


# #####################################################################
# FILES
# #####################################################################
# -------------------------------------------------
# Libaries
# -------------------------------------------------
# LIBS += -L$${PROJECT_ROOT}/lib/uEye -luEye_api
LIBS += -L$${PROJECT_ROOT}/lib/ximea/x86 -lxiapi32

# -------------------------------------------------
# Include directories
# -------------------------------------------------
INCLUDEPATH += ../include/
INCLUDEPATH += camera/
INCLUDEPATH += $${UI_DIR}

INCLUDEPATH += ../include/QCameraImage/
INCLUDEPATH += ../include/camera/uEye/
INCLUDEPATH += ../include/camera/ximea/

HEADERS += \
    ../include/camera/QRgbMatrix.h \
    ../include/camera/QCameraImageBase.h \
    ../include/camera/QCameraImage.h \
    gui/mainwindow.h \
    camera/camerainterface.h \
    camera/cameraximea.h

SOURCES += \
    ../include/camera/QRgbMatrix.cpp \
    ../include/camera/QCameraImageBase.cpp \
    ../include/camera/QCameraImage.cpp \
    main.cpp \
    gui/mainwindow.cpp \
    camera/cameraximea.cpp

FORMS += \
    gui/mainwindow.ui
