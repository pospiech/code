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

#EXAMPLE_DIR = $${PROJECT_ROOT}/bin
#debug:DLLDESTDIR = $${EXAMPLE_DIR}/debug/
#release:DLLDESTDIR = $${EXAMPLE_DIR}/release/

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
win32-g++:LIBS += $${PROJECT_ROOT}/lib/cutelogger/libCuteLogger.a
win32-g++:LIBS += $${PROJECT_ROOT}/lib/ueye/uEye_api.dll
# -------------------------------------------------
# Include directories
# -------------------------------------------------
INCLUDEPATH += ../include/
INCLUDEPATH += gui/
INCLUDEPATH += log/
INCLUDEPATH += camera/
INCLUDEPATH += camera/QCameraImage/
INCLUDEPATH += $${UI_DIR}

INCLUDEPATH += ../include/camera/uEye/
INCLUDEPATH += ../include/camera/ximea/
INCLUDEPATH += ../include/cutelogger/


HEADERS += \
    #camera/QCameraImage/QRgbMatrix.h \
    # camera/QCameraImage/QCameraImageBase.h \
    # camera/QCameraImage/QCameraImage.h \
    gui/mainwindow.h \
    gui/imageviewer.h \
    camera/camerainterface.h \
    camera/cameraximea.h \
    gui/widgethistogramm.h \
    camera/cameraximea_p.h \
    camera/cameraueye.h \
    camera/cameraueye_p.h \
    camera/cameratools.h \
    gui/widgetmessagelist.h \
    log/signalappender.h \
    camera/camerasimulation.h \
    camera/camerasimulation_p.h \
    camera/cameraimplementation.h

SOURCES += \
    #camera/QCameraImage/QRgbMatrix.cpp \
    # camera/QCameraImage/QCameraImageBase.cpp \
    # camera/QCameraImage/QCameraImage.cpp \
    main.cpp \
    gui/mainwindow.cpp \
    gui/imageviewer.cpp \
    camera/cameraximea.cpp \
    gui/widgethistogramm.cpp \
    camera/cameraueye.cpp \
    gui/widgetmessagelist.cpp \
    log/signalappender.cpp \
    camera/camerasimulation.cpp \
    camera/cameraimplementation.cpp

FORMS += \
    gui/mainwindow.ui
