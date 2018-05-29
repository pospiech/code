include( ../config.pri )
PROJECT_ROOT = ..

# #####################################################################
# Directories
# #####################################################################
BIN_DIR = $${PROJECT_ROOT}/bin
debug:DESTDIR = $${BIN_DIR}/debug/
release:DESTDIR = $${BIN_DIR}/release/

BUILD_DIR = $${PROJECT_ROOT}/build
debug:OBJECTS_DIR = $${BUILD_DIR}/debug/
release:OBJECTS_DIR = $${BUILD_DIR}/release/

EXAMPLE_DIR = $${PROJECT_ROOT}/bin
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
# LIBS += -L$${PROJECT_ROOT}/lib/ximea/x86 -lxiapi32.dll
win32-g++:LIBS += $${PROJECT_ROOT}/lib/ximea/x86/xiapi32.dll
# -------------------------------------------------
# Include directories
# -------------------------------------------------
INCLUDEPATH += ../include/
INCLUDEPATH += gui/
INCLUDEPATH += camera/
INCLUDEPATH += camera/QCameraImage/
INCLUDEPATH += $${UI_DIR}

INCLUDEPATH += ../include/camera/uEye/
INCLUDEPATH += ../include/camera/ximea/


HEADERS += \
    #camera/QCameraImage/QRgbMatrix.h \
    camera/QCameraImage/QCameraImageBase.h \
    # camera/QCameraImage/QCameraImage.h \
    gui/mainwindow.h \
    camera/camerainterface.h \
    camera/cameraximea.h

SOURCES += \
    #camera/QCameraImage/QRgbMatrix.cpp \
    camera/QCameraImage/QCameraImageBase.cpp \
    # camera/QCameraImage/QCameraImage.cpp \
    main.cpp \
    gui/mainwindow.cpp \
    camera/cameraximea.cpp

FORMS += \
    gui/mainwindow.ui
