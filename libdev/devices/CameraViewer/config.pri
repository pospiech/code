# #####################################################################
# Version
# #####################################################################
# VER_MAI      = 0
# VER_MIN      = 1
# VER_PAT      = 0
# VERSION      = $${VER_MAI}.$${VER_MIN}.$${VER_PAT}

TARGET = CameraViewer
TEMPLATE = app

# #####################################################################
# qmake internal options
# #####################################################################
# CONFIG += qt
# CONFIG += warn_on
# CONFIG += thread

QT += core
QT += gui
QT += gui-private
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# Necessary for uEye Cameras
QMAKE_CXXFLAGS_WARN_OFF = $$QMAKE_CXXFLAGS_WARN_OFF -w34100

# #####################################################################
# Library Name
# #####################################################################
# LIBRARY_NAME = uEyeCamera
# CONFIG(debug, debug|release):win32:LIBRARY_NAME = $$join(LIBRARY_NAME,,,d)

# #####################################################################
# Build the static/shared libraries.
# #####################################################################
# CONFIG += uEyeCameraDll
# win32:uEyeCameraDll:DEFINES += QT_DLL UEYECAMERA_DLL CAMERAIMAGE_DLL


# #####################################################################
# release/debug mode
# If you want to build both DEBUG_SUFFIX and RELEASE_SUFFIX
# have to differ to avoid, that they overwrite each other.
# #####################################################################
# Enable debug_and_release + build_all if you want to build both.
# CONFIG += debug # release/debug/debug_and_release
# CONFIG -= release
# On Windows you can't mix release and debug libraries.
# CONFIG           += debug_and_release
# CONFIG           += build_all


# #####################################################################
# DEFINES
# #####################################################################
win32 { 
    DEFINES += NOMINMAX
    DEFINES += _CRT_SECURE_NO_WARNINGS
}

# #####################################################################
# uEye Library
# #####################################################################

