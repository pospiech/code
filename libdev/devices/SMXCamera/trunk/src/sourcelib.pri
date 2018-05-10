HEADERS += $${SOURCEPATH}/QCameraSMXGlobal.h \    
    $${SOURCEPATH}/QCameraSMX150.h \
    $${SOURCEPATH}/WidgetCameraSMXSettings.h \
    $${SOURCEPATH}/../include/QCameraImage/QCameraImageBase.h \
    $${SOURCEPATH}/QSMXDLL.h \
    $${SOURCEPATH}/QCameraImage.h


SOURCES += $${SOURCEPATH}/QCameraSMX150.cpp \
    $${SOURCEPATH}/WidgetCameraSMXSettings.cpp \    
    $${SOURCEPATH}/../include/QCameraImage/QCameraImageBase.cpp \
    $${SOURCEPATH}/QSMXDLL.cpp \
    $${SOURCEPATH}/QCameraImage.cpp

FORMS += $${SOURCEPATH}/WidgetCameraSMXSettings.ui

INCLUDEPATH += $${SOURCEPATH}/../include/QCameraImage/ \
    $${SOURCEPATH}/../include/Sumix/SMX150/

