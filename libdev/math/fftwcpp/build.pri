################################################################
# FFTW CPP Library
# Copyright (C) 2016   Matthias Pospiech
################################################################

######################################################################
# qmake internal options
######################################################################

CONFIG           -= qt
CONFIG           += warn_on

######################################################################
# release/debug mode
######################################################################

#win32 {
#    # On Windows you can't mix release and debug libraries.
#    # For your own application development you might need a debug version.
#    # Enable debug_and_release + build_all if you want to build both.

#    CONFIG           += debug_and_release
#    CONFIG           += build_all
#}
#else {

#    CONFIG           += release
#}


######################################################################
# paths for building the project
######################################################################

BUILD_DIR = $${PWD}/build

MOC_DIR             = $${BUILD_DIR}/moc
UI_DIR              = $${BUILD_DIR}/ui
RCC_DIR             = $${BUILD_DIR}/resources

!debug_and_release {
    # in case of debug_and_release object files
    # are built in the release and debug subdirectories
    OBJECTS_DIR       = $${BUILD_DIR}
}


# #####################################################################
# Defines for Compilation
# #####################################################################


win32 {
    # DEFINES += NOMINMAX
    # DEFINES += _CRT_SECURE_NO_WARNINGS
}
