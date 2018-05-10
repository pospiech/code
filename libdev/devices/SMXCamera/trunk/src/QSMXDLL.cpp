#include "QSMXDLL.h"


#include <QDebug>

QSMXDLL::QSMXDLL(QObject *parent, bool loadDll)
    : QLibrary(parent)
{
    if (loadDll) {
        QSMXDLL::loadDLL();
    }
}

QSMXDLL::~QSMXDLL()
{
    QLibrary::unload();
}

#define RESOLVEDLLNAME(name) \
    name = (name##_t)(QLibrary::resolve(#name)); \
    if (name == NULL) { goto FunctionNotFoundError; }

bool QSMXDLL::loadDLL(QString filename)
{
    QLibrary::setFileName(filename);
    QLibrary::load();
    if (!QLibrary::isLoaded()) {
        qFatal(QString("dll \"" + filename + "\" could not be loaded!").toLocal8Bit());
        return false;
    }

    RESOLVEDLLNAME(CxOpenDevice);
    RESOLVEDLLNAME(CxCloseDevice);

    RESOLVEDLLNAME(CxGetCameraInfo);
    RESOLVEDLLNAME(CxGetCameraInfoEx);

    RESOLVEDLLNAME(CxGetStreamMode);
    RESOLVEDLLNAME(CxSetStreamMode);

    RESOLVEDLLNAME(CxGetScreenParams);
    RESOLVEDLLNAME(CxSetScreenParams);
    RESOLVEDLLNAME(CxActivateScreenParams);

    RESOLVEDLLNAME(CxGetFrequency);
    RESOLVEDLLNAME(CxSetFrequency);

    RESOLVEDLLNAME(CxGetExposureMinMax);
    RESOLVEDLLNAME(CxGetExposure);
    RESOLVEDLLNAME(CxSetExposure);

    RESOLVEDLLNAME(CxGetExposureMinMaxMs);
    RESOLVEDLLNAME(CxGetExposureMs);
    RESOLVEDLLNAME(CxSetExposureMs);

    RESOLVEDLLNAME(CxSetGain);
    RESOLVEDLLNAME(CxGetGain);

    RESOLVEDLLNAME(CxGrabVideoFrame);
    RESOLVEDLLNAME(CxGetFramePtr);

    RESOLVEDLLNAME(CxSetBrightnessContrastGamma);
    RESOLVEDLLNAME(CxSetConvertionTab);
    RESOLVEDLLNAME(CxGetConvertionTab);
    RESOLVEDLLNAME(CxSetDefaultConvertionTab);

    RESOLVEDLLNAME(CxStartVideo);
    RESOLVEDLLNAME(CxStopVideo);
    RESOLVEDLLNAME(CxSetBayerAlg);
    RESOLVEDLLNAME(CxBayerToRgb);

    RESOLVEDLLNAME(CxGetFrameCounter);

    RESOLVEDLLNAME(CxGetSnapshotExposureMinMax);
    RESOLVEDLLNAME(CxGetSnapshotExposure);
    RESOLVEDLLNAME(CxSetSnapshotExposure);

    RESOLVEDLLNAME(CxGetSnapshotExposureMinMaxMs);
    RESOLVEDLLNAME(CxGetSnapshotExposureMs);
    RESOLVEDLLNAME(CxSetSnapshotExposureMs);

    RESOLVEDLLNAME(CxSetSnapshotExposureMultiplier);
    RESOLVEDLLNAME(CxGetSnapshotExposureMultiplier);

    RESOLVEDLLNAME(CxSetMultipleSnapshotExposure);
    RESOLVEDLLNAME(CxGetMultipleSnapshotExposure);
    RESOLVEDLLNAME(CxSetMultipleSnapshotExposureMs);
    RESOLVEDLLNAME(CxGetMultipleSnapshotExposureMs);

    RESOLVEDLLNAME(CxGetSnapshot);
    RESOLVEDLLNAME(CxCancelSnapshot);

    RESOLVEDLLNAME(CxGetDACRawOffset);
    RESOLVEDLLNAME(CxSetDACRawOffset);
    RESOLVEDLLNAME(CxGetDACFineOffset);
    RESOLVEDLLNAME(CxSetDACFineOffset);

    RESOLVEDLLNAME(CxSetContinuousPrecharge);
    RESOLVEDLLNAME(CxGetContinuousPrecharge);

    RESOLVEDLLNAME(CxSetTristateOut);
    RESOLVEDLLNAME(CxGetTristateOut);

    RESOLVEDLLNAME(CxElectricalBlack);

    RESOLVEDLLNAME(CxWriteSensorReg);
    RESOLVEDLLNAME(CxReadSensorReg);

    RESOLVEDLLNAME(CxSetControlReg);
    RESOLVEDLLNAME(CxGetControlReg);

    RESOLVEDLLNAME(CxSet10BitsOutput);
    RESOLVEDLLNAME(CxGet10BitsOutput);

    RESOLVEDLLNAME(CxSetLineBlank);
    RESOLVEDLLNAME(CxGetLineBlank);

    RESOLVEDLLNAME(CxSetHBlank);
    RESOLVEDLLNAME(CxGetHBlank);

    RESOLVEDLLNAME(CxSetFrameRate);
    RESOLVEDLLNAME(CxGetFrameRate);

    RESOLVEDLLNAME(CxSetMaxFrameRate);
    RESOLVEDLLNAME(CxSetDefaultFrameRate);

    RESOLVEDLLNAME(CxSetVDDC);
    RESOLVEDLLNAME(CxGetVDDC);

    RESOLVEDLLNAME(CxSetVDDH);
    RESOLVEDLLNAME(CxGetVDDH);

    RESOLVEDLLNAME(CxSetADCSampleDelay);
    RESOLVEDLLNAME(CxGetADCSampleDelay);

    RESOLVEDLLNAME(CxGetStatistics);

    return true;

FunctionNotFoundError:
    unload();
    qFatal(QString("invalid dll \"" + filename + "\"").toLocal8Bit());
    //throw std::runtime_error(QString("invalid dll \"" + filename + "\"").toStdString());
    return false;

}
