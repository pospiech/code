HEADERS += src/mainwindow.h \
    src/DialogCameraSMX.h \
    src/globaldevices.h

SOURCES += src/main.cpp \
    src/mainwindow.cpp \
    src/DialogCameraSMX.cpp \
    src/globaldevices.cpp

FORMS += src/mainwindow.ui
FORMS += src/DialogCameraSMX.ui

INCLUDEPATH += ../include/QCameraImage/ \
    ../include/Sumix/SMX150/
