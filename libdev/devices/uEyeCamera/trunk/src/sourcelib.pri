HEADERS += $${SOURCEPATH}/QCameraGlobal.h \
    $${SOURCEPATH}/QuEyeCamera.h \
#    $${SOURCEPATH}/QCameraWidget.h \
    $${SOURCEPATH}/QRgbMatrix.h \
    $${SOURCEPATH}/../include/QCameraImage/QCameraImageBase.h \
    $${SOURCEPATH}/QCameraImage.h \
    $${SOURCEPATH}/widgetueyecamerasettings.h

SOURCES += $${SOURCEPATH}/QuEyeCamera.cpp \
 #   $${SOURCEPATH}/QCameraWidget.cpp \
    $${SOURCEPATH}/QRgbMatrix.cpp \
    $${SOURCEPATH}/widgetueyecamerasettings.cpp \
    $${SOURCEPATH}/../include/QCameraImage/QCameraImageBase.cpp \
    $${SOURCEPATH}/QCameraImage.cpp

FORMS += widgetueyecamerasettings.ui

INCLUDEPATH += $${SOURCEPATH}/../include/QCameraImage/
INCLUDEPATH += $${SOURCEPATH}/../include/uEye/
