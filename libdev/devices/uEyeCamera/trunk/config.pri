# #####################################################################
# Version
# #####################################################################
# VER_MAI      = 0
# VER_MIN      = 1
# VER_PAT      = 0
# VERSION      = $${VER_MAI}.$${VER_MIN}.$${VER_PAT}
# #####################################################################
# qmake internal options
# #####################################################################
CONFIG += qt
CONFIG += warn_on
CONFIG += thread

QMAKE_CXXFLAGS_WARN_OFF = $$QMAKE_CXXFLAGS_WARN_OFF -w34100

# #####################################################################
# Library Name
# #####################################################################
LIBRARY_NAME = uEyeCamera
CONFIG(debug, debug|release):win32:LIBRARY_NAME = $$join(LIBRARY_NAME,,,d)

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
# Build the static/shared libraries.
# #####################################################################
CONFIG += uEyeCameraDll

# #####################################################################
# Auto select compiler
# #####################################################################
#COMPILER = other
#win32-g++:COMPILER = mingw
#win32-msvc*:COMPILER = msvc
#linux-g++:COMPILER = gcc

# #####################################################################
# DEFINES
# #####################################################################
win32:uEyeCameraDll:DEFINES += QT_DLL UEYECAMERA_DLL CAMERAIMAGE_DLL
win32 { 
    DEFINES += NOMINMAX
    DEFINES += _CRT_SECURE_NO_WARNINGS
}

# #####################################################################
# uEye Library
# #####################################################################

