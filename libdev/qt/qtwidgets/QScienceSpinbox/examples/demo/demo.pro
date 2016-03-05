################################################################
# QScienceSpinBox Library
# Copyright (C) 2015   Matthias Pospiech
################################################################

include( ../examples.pri )

# #####################################################################
# set Target Name
# #####################################################################

TARGET  = QScienceSpinBoxExample
CONFIG(debug, debug|release):win32:TARGET = $$join(TARGET,,,d)

# #####################################################################
# Sources
# #####################################################################

SOURCES += main.cpp \
		mainwindow.cpp

HEADERS += mainwindow.h
FORMS 	+= mainwindow.ui
