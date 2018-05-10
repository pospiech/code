# -------------------------------------------------
# Define Variables
# -------------------------------------------------
BUILD_DIR = build
BIN_DIR = bin
# -------------------------------------------------
win32 {
    DEBUG_SUFFIX      = d
}
CONFIG(debug, debug|release) {
    SUFFIX_STR = $${DEBUG_SUFFIX}
}
else {
    SUFFIX_STR = $${RELEASE_SUFFIX}
}
# -------------------------------------------------


# -------------------------------------------------
# Target Directory
# -------------------------------------------------
debug {
    DESTDIR = $${BIN_DIR}/debug/
}
release{
    DESTDIR = $${BIN_DIR}/release/
}

# -------------------------------------------------
# Executable Filename
# -------------------------------------------------

TARGET = $${BINARY_NAME}$${SUFFIX_STR}


#CONFIG += qt thread debug

# -------------------------------------------------
# Prepocessor Definitions
# -------------------------------------------------
win32{
    #DEFINES +=  QWT_DLL
    DEFINES +=  NOMINMAX
}

# -------------------------------------------------
# Objects Directory
# -------------------------------------------------
debug {
    OBJECTS_DIR = $${BUILD_DIR}/debug/
}
release{
    OBJECTS_DIR = $${BUILD_DIR}/release/
}

# -------------------------------------------------
# Moc Directory
# -------------------------------------------------
MOC_DIR = moc

# -------------------------------------------------
# Moc Directory
# -------------------------------------------------
UI_DIR  = ui

# -------------------------------------------------
# Include directories
# -------------------------------------------------
INCLUDEPATH = src;ui;include;include/boost;include/fftw;include/qwt;include/qextserialport;include/matthias/qtwidgets;include/matthias/SMX;include/matthias/GPIB;include/matthias/PI;include/matthias/PiezoJena;include/matthias/Math;include/matthias/qwtplot;include/matthias/Common;

# -------------------------------------------------
# Libaries
# -------------------------------------------------

#unix{
#    LIBS += -L/usr/local/lib -lmath
#}
win32{
#    debug{
#        LIBS += lib/GPIB/gpib-32.obj
#        LIBS += lib/MMC/MMC413.lib
#        LIBS += lib/qextserialport/debug/qextserialport.lib
#        LIBS += lib/qwt/msvc/debug/qwtd5.lib
#        LIBS += lib/fftw/double/libfftw3-3.lib
#    }
#    release{
#        LIBS += lib/GPIB/gpib-32.obj
#        LIBS += lib/MMC/MMC413.lib
#        LIBS += lib/qextserialport/release/qextserialport.lib
#        LIBS += lib/qwt/msvc/release/qwt5.lib
#        LIBS += lib/fftw/double/libfftw3-3.lib
#    }
    debug{
        LIBS += lib/qwt/mingw/debug/libqwtd5.a
    }
    release{
        LIBS += lib/qwt/mingw/release/libqwt5.a
    }
}
