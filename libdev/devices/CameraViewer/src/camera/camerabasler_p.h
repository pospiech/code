#ifndef CAMERABASLER_P_H
#define CAMERABASLER_P_H

#include "cameraimplementation.h"

#define APIIMPORT __declspec(dllimport)
#define PYLON_WIN_BUILD

#include <pylon/PylonIncludes.h>
#include <pylon/usb/BaslerUsbInstantCamera.h>
#include <pylon/usb/_BaslerUsbCameraParams.h>
using namespace Basler_UsbCameraParams;

#ifdef PYLON_WIN_BUILD
#    include <pylon/PylonGUI.h>
#endif

// Allow all the names in the namespace GenApi to be used without qualification.
using namespace GenApi;


// Namespace for using pylon objects.
using namespace Pylon;
using namespace std;


#include <QtDebug>
#include <QtCore/QRect>
#include <QString>

#include <stdexcept>
#include <math.h>

/// NodeMap for CInstantCamera
//AnalogControl
//	GainSelector
//	Gain
//	GainAuto
//	AutoGainLowerLimit
//	AutoGainUpperLimit
//	BlackLevelSelector
//	BlackLevel
//	Gamma
//	BslColorSpaceMode
//ImageFormatControl
//	SensorWidth
//	SensorHeight
//	WidthMax
//	HeightMax
//	Width
//	Height
//	OffsetX
//	OffsetY
//	BinningHorizontalMode
//	BinningHorizontal
//	BinningVerticalMode
//	BinningVertical
//	ReverseX
//	ReverseY
//	PixelFormat
//	PixelSize
//	PixelDynamicRangeMin
//	PixelDynamicRangeMax
//	TestPattern
//AcquisitionControl
//	AcquisitionMode
//	AcquisitionStart
//	AcquisitionStop
//	SensorShutterMode
//	OverlapMode
//	BslImmediateTriggerMode
//	ExposureTime
//	ExposureAuto
//	AutoExposureTimeLowerLimit
//	AutoExposureTimeUpperLimit
//	TriggerSelector
//	TriggerMode
//	TriggerSoftware
//	TriggerSource
//	TriggerActivation
//	ExposureMode
//	AcquisitionFrameRate
//	ResultingFrameRate
//ImageQualityControl
//	BslContrastMode
//	BslBrightness
//	BslContrast
//	DefectPixelCorrectionMode
//DigitalIOControl
//	LineSelector
//	LineMode
//	LineInverter
//	LineSource
//	LineFormat
//	LineDebouncerTime
//	LineStatus
//	LineStatusAll
//	UserOutputSelector
//	UserOutputValue
//AutoFunctionControl
//	AutoTargetBrightness
//	AutoTargetBrightnessDamping
//	AutoBacklightCompensation
//	AutoFunctionProfile
//	AutoFunctionROIControl
//		AutoFunctionROISelector
//		AutoFunctionROIWidth
//		AutoFunctionROIHeight
//		AutoFunctionROIOffsetX
//		AutoFunctionROIOffsetY
//		AutoFunctionROIUseBrightness
//UserSetControl
//	UserSetSelector
//	UserSetLoad
//	UserSetSave
//	UserSetDefault
//TestControl
//	TestPendingAck
//TransportLayerControl
//	PayloadSize
//	BslUSBSpeedMode
//	PayloadTransferSize
//	PayloadTransferCount
//	PayloadFinalTransfer1Size
//	PayloadFinalTransfer2Size
//DeviceControl
//	DeviceVendorName
//	DeviceModelName
//	DeviceManufacturerInfo
//	DeviceVersion
//	DeviceFirmwareVersion
//	DeviceSerialNumber
//	DeviceUserID
//	DeviceScanType
//	DeviceSFNCVersionMajor
//	DeviceSFNCVersionMinor
//	DeviceSFNCVersionSubMinor
//	DeviceReset
//	DeviceLinkSelector
//	DeviceLinkSpeed
//	DeviceLinkThroughputLimitMode
//	DeviceLinkThroughputLimit
//	DeviceIndicatorMode
//	DeviceRegistersStreamingStart
//	DeviceRegistersStreamingEnd


//class SafePylon
//{
//public:
//       SafePylon()
//       {
//           LOG_INFO() << "Constructor: SafePylon";
//           PylonInitialize();
//       }
//       ~SafePylon()
//       {
//           LOG_INFO() << "Destructor: SafePylon";
//           PylonTerminate();
//       }
//};

//class SafeInstantCamera
//        : public Pylon::CBaslerUsbInstantCamera
//        , public SafePylon
//{
//public:
//       SafeInstantCamera() : CBaslerUsbInstantCamera(), SafePylon()
//       {
//       }
//};

class CameraBaslerPrivate: public CameraImplementation
{
public:
    CameraBaslerPrivate()
    {
        LOG_INFO() << "Constructor: CameraBaslerPrivate";
        PylonInitialize();
        isOpen = false;
    }
    ~CameraBaslerPrivate()
    {
        LOG_INFO() << "Destructor: CameraBaslerPrivate";
        closeCamera();
        PylonTerminate();
    }

    // Provides convenient access to a camera device.
    // CInstantCamera (no support for usb)
    CBaslerUsbInstantCamera camera;
    // This smart pointer will receive the grab result data.
    CGrabResultPtr ptrGrabResult;


    bool initialize()
    {
        try {            
            // enumerate all connected devices
            DeviceInfoList_t lstDevices;
            CTlFactory::GetInstance().EnumerateDevices( lstDevices );
            if (! lstDevices.empty()) {
                return true;
            } else {
                return false;
            }
        }
        catch (const Pylon::GenericException& e) {
            LOG_ERROR() << "Error in PylonInitialize: " << e.what();
            return false;
        }
    }
    /** This function initializes the device
     */
    void openCamera()
    {
        try {
            // Attaches a Pylon device to the Instant Camera.
            camera.Attach(CTlFactory::GetInstance().CreateFirstDevice());
            LOG_INFO() << "using camera: " << camera.GetDeviceInfo().GetModelName().c_str() << endl;
            camera.Open();
            isOpen = camera.IsOpen();
            if (!isOpen) {
                LOG_ERROR() << "camera not opened.";
                return;
            }

            configureMaxBitDepth();
            // disable auto funtions
            camera.GainAuto.SetValue(GainAuto_Off);
            camera.Gain.SetValue(0.0);
            camera.ExposureAuto.SetValue(ExposureAuto_Off);

            LOG_INFO() << "open Camera";
        }
        catch (const Pylon::GenericException& e) {
            LOG_ERROR() << "Error in PylonInitialize: " << e.what();
            return;
        }

    }

    /** This function will un-initialize the specified device,
     * closes its handle and releases allocated resources.
     */
    void closeCamera()
    {
        try {
            // closes the attached camera device
            camera.Close();
            camera.DetachDevice();
            camera.DestroyDevice();

            PylonTerminate();
            PylonInitialize();

            isOpen = false;
            LOG_INFO() << "close Camera (finished)";
        }
        catch (const Pylon::GenericException& e) {
            LOG_ERROR() << "Error in PylonInitialize: " << e.what();
            return;
        }

    }

    /** convert raw memory data to QImage format */
    void createData()
    {
        if (!ptrGrabResult) {
            throw;
        }

        using namespace std;
        // Access the image data.
        const size_t sizeX = static_cast<size_t>(ptrGrabResult->GetWidth());
        const size_t sizeY = static_cast<size_t>(ptrGrabResult->GetHeight());
        const size_t size = sizeX * sizeY;
        // make sure all relevant parameters are correct
        if (!(sizeX > 0 && sizeY > 0)) {
            QString errorStr = "Parameters <width> or <height> are 0";
            LOG_FATAL() << errorStr;
            throw std::runtime_error(errorStr.toStdString());
        }

        void *pMemVoid = ptrGrabResult->GetBuffer();
        // cout << "Gray value of first pixel: " << (uint32_t) pImageBuffer[0] << endl << endl;

        EPixelType pixelFormat = ptrGrabResult->GetPixelType();
        LOG_INFO() << "pixelFormat" << pixelFormat;

        grayVector.resize(size);
        // 1 << 8 = 256, 1 << 10 = 1024
        size_t histSize = 1 << bitsPerPixel(pixelFormat);
        histVector.clear();
        histVector.resize(histSize);

        // convert / copy pointer data into vector: 8 bit
        switch (pixelFormat)
        {
        case EPixelType::PixelType_Mono8:
        {
            quint8* imageIterator = reinterpret_cast<quint8*> (pMemVoid);
            for (size_t count = 0; count < size; ++count)
            {
                const int value = static_cast<int>(*imageIterator);
                grayVector[count] = value;
                histVector[static_cast<size_t>(value)] += 1;
                imageIterator++;
            }
            break;
        }
        case EPixelType::PixelType_Mono10:
        case EPixelType::PixelType_Mono12:
        case EPixelType::PixelType_Mono16:
        {
            quint16* imageIterator = reinterpret_cast<quint16*> (pMemVoid);
            for (size_t count = 0; count < size; ++count)
            {
                const int value = static_cast<int>(*imageIterator);
                grayVector[count] = value;
                histVector[static_cast<size_t>(value)] += 1;
                imageIterator++;
            }
            break;
        }
        default:
            LOG_ERROR() << "Unhandled EPixelType image format";
        }

        LOG_INFO() << "create Data from Raw Image (finished)";
    }

    /** returns bytes per pixel */
    size_t bytesPerPixel(EPixelType format)
    {
        size_t  srcPixelBytes = 0;
        switch (format) {
        case EPixelType::PixelType_Mono8: // 8 bits per pixel
            srcPixelBytes = 1;
            break;
        case EPixelType::PixelType_Mono10:// 16 bits per pixel
        case EPixelType::PixelType_Mono12:
        case EPixelType::PixelType_Mono16:
            srcPixelBytes = 2;
            break;
        }
        Q_ASSERT(srcPixelBytes > 0 && srcPixelBytes <= 4);
        return srcPixelBytes;
    }

    /** returns bytes per pixel */
    size_t bitsPerPixel(EPixelType format)
    {
        size_t srcPixelBits = 0;
        switch (format) {
        case EPixelType::PixelType_Mono8: srcPixelBits = 8; break;
        case EPixelType::PixelType_Mono10: srcPixelBits = 10; break;
        case EPixelType::PixelType_Mono12: srcPixelBits = 12; break;
        case EPixelType::PixelType_Mono16: srcPixelBits = 16; break;
        }
        Q_ASSERT(srcPixelBits > 0 && srcPixelBits <= 16);
        return srcPixelBits;
    }

    /** get ROI using offets and width and height */
    QRect roi() const
    {
        QRect roi;
        if (!camera.IsOpen()) {
            LOG_ERROR() << "camera not opened.";
            return QRect(0,0,0,0);;
        }

        try {
            // Get the camera control object.
            INodeMap &control = const_cast<CBaslerUsbInstantCamera *>(&camera)->GetNodeMap();

            const CIntegerPtr width = control.GetNode("Width");
            const CIntegerPtr height = control.GetNode("Height");
            const CIntegerPtr offsetX = control.GetNode("OffsetX");
            const CIntegerPtr offsetY = control.GetNode("OffsetY");

            roi.setX(static_cast<int>(offsetX->GetValue()));
            roi.setY(static_cast<int>(offsetY->GetValue()));
            roi.setWidth(static_cast<int>(width->GetValue()));
            roi.setHeight(static_cast<int>(height->GetValue()));

        }
        catch (const Pylon::GenericException& e) {
            throw RUNTIME_EXCEPTION( "Could not get ROI configuration. const GenericException caught in imageSize method msg=%hs", e.what());
        }
        return roi;
    }


    /** set ROI using offets and width and height */
    void setROI(QRect roi)
    {
        if (!camera.IsOpen()) {
            LOG_ERROR() << "camera not opened.";
            return;
        }
        try {
            QRect currROI = this->roi();
            // Get the camera control object.
            INodeMap &control = camera.GetNodeMap();

            const CIntegerPtr width = control.GetNode("Width");
            const CIntegerPtr height = control.GetNode("Height");
            const CIntegerPtr offsetX = control.GetNode("OffsetX");
            const CIntegerPtr offsetY = control.GetNode("OffsetY");

            if (roi.top() > currROI.top()) {
                // width and heigt must be reduced before offset
                // otherwise offset max and min values are limited.
                if (IsWritable(height))  height->SetValue(roi.height());
                if (IsWritable(offsetY)) offsetY->SetValue(roi.y());
            }
            else {
                // offset must be reset first to ensure higher height is possible
                if (IsWritable(offsetY)) offsetY->SetValue(roi.y());
                if (IsWritable(height))  height->SetValue(roi.height());
            }
            // similar for width ...
            if (roi.left() > currROI.left()) {
                // width and heigt must be reduced before offset
                // otherwise offset max and min values are limited.
                if (IsWritable(width))   width->SetValue(roi.width());
                if (IsWritable(offsetX)) offsetX->SetValue(roi.x());
            }
            else {
                // offset must be reset first to ensure higher width is possible
                if (IsWritable(offsetX)) offsetX->SetValue(roi.x());
                if (IsWritable(width))   width->SetValue(roi.width());
            }
        }
        catch (const Pylon::GenericException& e)
        {
            throw RUNTIME_EXCEPTION( "Could not apply configuration. const GenericException caught in setROI method msg=%hs", e.what());
        }
    }

    /** returns current image size in Pixel  */
    QSize imageSize()
    {
        if (!camera.IsOpen()) {
            LOG_ERROR()<< "camera not opened";
            return QSize(0,0);
        }

        QRect curr_roi = this->roi();
        return QSize(curr_roi.width(), curr_roi.height());
    }


    /** This function starts the data acquisition on the devices specified by the handle. */
    void startAcquisition()
    {
        if (!camera.IsOpen()) {
            LOG_ERROR()<< "camera not opened";
            return;
        }

        try {
            const uint32_t c_countOfImagesToGrab = 1;
            camera.StartGrabbing( c_countOfImagesToGrab );
            LOG_INFO() << "start Aquisition (finished)";
        }
        catch (const Pylon::GenericException& e) {
            throw RUNTIME_EXCEPTION( "startAcquisition failed msg=%hs", e.what());
        }

    }

    /** Ends the work cycle of the camera, stops data acquisition
     * and deallocates internal image buffers.
     */
    void stopAcquisition()
    {
        try {
            if (camera.IsOpen()) {
                camera.StopGrabbing();
                LOG_INFO() << "stop Aquisition (finished)";
            }
        }
        catch (const Pylon::GenericException& e) {
            throw RUNTIME_EXCEPTION( "stopAcquisition failed msg=%hs", e.what());
        }

    }

    /** This function waits for next image is available at transport buffer.
     */
    void getImage()
    {
        try {
            if (!camera.IsOpen()) {
                LOG_ERROR()<< "camera not opened";
                return;
            }

            if ( camera.IsGrabbing())
            {
                // Wait for an image and then retrieve it. A timeout of 5000 ms is used.
                camera.RetrieveResult( 5000, ptrGrabResult, TimeoutHandling_ThrowException);
                // Image grabbed successfully?
                if (ptrGrabResult->GrabSucceeded())
                {
                    createData();
                }
                else {
                    LOG_ERROR() << "Error: Grab did not succeed";
                }
                // create data and histogram
                LOG_INFO() << "Get Image (finished)";
            }
            else {
                LOG_ERROR() << "basler camera is not grabbing";
            }
        }
        catch (const Pylon::GenericException& e) {
            throw RUNTIME_EXCEPTION( "getImage failed msg=%hs", e.what());
        }

    }

    /** Sets exposure time in microseconds. */
    void setExposure(float exposure_ms)
    {
        if (!camera.IsOpen()) {
            LOG_ERROR()<< "camera not opened";
            return;
        }

        // exposure time is set in microseconds ms->µs
        camera.ExposureTime.SetValue(static_cast<double>(exposure_ms*1000.0f));
//        INodeMap &control = camera.GetNodeMap();
//        const CIntegerPtr exposureNode = control.GetNode("Exposure");
//        // Setting "exposure" parameter (10ms=10000us)
//        exposureNode->SetValue(static_cast<int64_t>(exposure_ms*1000));
    }

    /** Get exposure time in microseconds. */
    float exposure() const
    {
        if (!camera.IsOpen()) {
            LOG_ERROR()<< "camera not opened";
            return 0;
        }

        // exposure µs -> ms
        float value = static_cast<float>(camera.ExposureTime.GetValue());
        return value/1000.0f; //
    }

    float exposureMin() const
    {
        if (!camera.IsOpen()) {
            LOG_ERROR()<< "camera not opened";
            return 0;
        }

        float value = static_cast<float>(camera.ExposureTime.GetMin());
        return value/1000.0f; // us -> ms
    }

    float exposureMax() const
    {
        if (!camera.IsOpen()) {
            LOG_ERROR()<< "camera not opened";
            return 0;
        }

        float value = static_cast<float>(camera.ExposureTime.GetMax());
        return value/1000.0f; // us -> ms
    }

    /** Sets gain in dB. */
    void setGain(float gain_in_db)
    {
        if (!camera.IsOpen()) {
            LOG_ERROR()<< "camera not opened";
            return;
        }

        LOG_INFO() << "setting gain to:" << gain_in_db;
        camera.Gain.SetValue(static_cast<double>(gain_in_db));

    }

    /** gain in dB. */
    float gain() const
    {
        if (!camera.IsOpen()) {
            LOG_ERROR()<< "camera not opened";
            return 0;
        }

        return static_cast<float>(camera.Gain.GetValue());
    }

    float gainMax() const
    {
        if (!camera.IsOpen()) {
            LOG_ERROR()<< "camera not opened";
            return 0;
        }

        return static_cast<float>(camera.Gain.GetMax());
    }

    float gainMin() const
    {
        if (!camera.IsOpen()) {
            LOG_ERROR()<< "camera not opened";
            return 0;
        }

        return static_cast<float>(camera.Gain.GetMin());
    }

    /** configure Binning */
    void configureBinning(bool enableBinning, Qt::Orientation orientation, int binningPixels = 2)
    {                
        if (!camera.IsOpen()) {
            LOG_ERROR()<< "camera not opened";
            return;
        }

        if (enableBinning) {
            if (orientation == Qt::Horizontal){
                camera.BinningHorizontal.SetValue(binningPixels);
                camera.BinningHorizontalMode.SetValue(BinningHorizontalModeEnums::BinningHorizontalMode_Sum);
            } else {
                camera.BinningVertical.SetValue(binningPixels);
                camera.BinningVerticalMode.SetValue(BinningVerticalModeEnums::BinningVerticalMode_Sum);
            }
        }
        else {
            if (orientation == Qt::Horizontal){
                camera.BinningHorizontal.SetValue(1);
            } else {
                camera.BinningVertical.SetValue(1);
            }
        }
    }

    void configureMaxBitDepth()
    {
        if (!camera.IsOpen()) {
            LOG_ERROR()<< "camera not opened";
            return;
        }
        try {
            if ( GenApi::IsAvailable( camera.PixelFormat.GetEntry(PixelFormat_Mono12)))
            {
                camera.PixelFormat.SetValue(PixelFormat_Mono12);
                LOG_INFO() << "New PixelFormat  : " << camera.PixelFormat.ToString() << " (" << camera.PixelFormat.GetValue() << ")" << endl;
            }
            else if ( GenApi::IsAvailable( camera.PixelFormat.GetEntry(PixelFormat_Mono10)))
            {
                camera.PixelFormat.SetValue(PixelFormat_Mono10);
                LOG_INFO() << "New PixelFormat  : " << camera.PixelFormat.ToString() << " (" << camera.PixelFormat.GetValue() << ")" << endl;
            }
            else if ( GenApi::IsAvailable( camera.PixelFormat.GetEntry(PixelFormat_Mono8)))
            {
                camera.PixelFormat.SetValue(PixelFormat_Mono8);
                LOG_INFO() << "New PixelFormat  : " << camera.PixelFormat.ToString() << " (" << camera.PixelFormat.GetValue() << ")" << endl;
            }
            else {
                // nothing worked, reset
                LOG_INFO() << "Keeping PixelFormat  : " << camera.PixelFormat.ToString() << " (" << camera.PixelFormat.GetValue() << ")" << endl;
            }
        }
        catch (const Pylon::GenericException& e)
        {
            throw RUNTIME_EXCEPTION( "Could not apply Pixel format. msg=%hs", e.what());
        }
    }

    /** Format of image data */
    void setImageDataFormat(int formatCode = PixelFormatEnums::PixelFormat_Mono8)
    {
        if (!camera.IsOpen()) {
            LOG_ERROR()<< "camera not opened";
            return;
        }

        // PixelFormat_Mono8
        // PixelFormat_Mono10
        // PixelFormat_Mono12
        // PixelFormat_Mono16
        bool verify;
        camera.PixelFormat.SetValue(static_cast<PixelFormatEnums>(formatCode), verify);
        LOG_INFO() << verify;

    }

    /** get binning increment in X direction */
    int binningIncrementX() const
    {
        if (!camera.IsOpen()) {
            LOG_ERROR()<< "camera not opened";
            return 0;
        }

        return static_cast<int>(camera.BinningHorizontal.GetInc());
    }

    /** get binning increment in Y direction */
    int binningIncrementY() const
    {
        if (!camera.IsOpen()) {
            LOG_ERROR()<< "camera not opened";
            return 0;
        }

        return static_cast<int>(camera.BinningVertical.GetInc());
    }

    /** get ROI using offets and width and height */
    QSize sensorSize()
    {
        if (!camera.IsOpen()) {
            LOG_ERROR()<< "camera not opened";
            return QSize(0,0);
        }

        this->sizeSensor.setWidth(static_cast<int>(camera.SensorWidth.GetValue()));
        this->sizeSensor.setHeight(static_cast<int>(camera.SensorHeight.GetValue()));

        return this->sizeSensor;
    }

    /** get parameters of camera, results are in QMap */
    QMap<QString, QString> getParamters()
    {
        QMap<QString, QString> mapParam;
        if (camera.IsOpen()) {
            try{
                INodeMap& nodemap = camera.GetNodeMap();
                QString strSerial = QString::fromLatin1(CStringPtr(nodemap.GetNode( "DeviceSerialNumber"))->GetValue().c_str());
                mapParam.insert("Serial", strSerial);
                QString strVendorName = QString::fromLatin1(CStringPtr( nodemap.GetNode( "DeviceVendorName") )->GetValue().c_str());
                mapParam.insert("Vendor Name", strVendorName);
                QString strDeviceName = QString::fromLatin1(CStringPtr( nodemap.GetNode( "DeviceModelName") )->GetValue().c_str());
                mapParam.insert("Device Name", strDeviceName);
                QString strDeviceFirmwareVersion = QString::fromLatin1(CStringPtr( nodemap.GetNode( "DeviceFirmwareVersion") )->GetValue().c_str());
                mapParam.insert("Device Name", strDeviceFirmwareVersion);
                QString strDeviceVersion = QString::fromLatin1(CStringPtr( nodemap.GetNode( "DeviceVersion") )->GetValue().c_str());
                mapParam.insert("Device Version", strDeviceVersion);
                QString strDeviceUserID = QString::fromLatin1(CStringPtr( nodemap.GetNode( "DeviceUserID") )->GetValue().c_str());
                mapParam.insert("Device User ID", strDeviceUserID);
            }
            catch(const std::exception& e)
            {
                qDebug() << "Error:" << e.what();
            }
        }
        else {
             LOG_ERROR()<< "camera not opened";
        }
        return mapParam;
    }

    /** handle camera error messages */
//    void errorHandler(int retCode, QString strInformation = "") const
//    {
//            LOG_WARNING() << "CameraViewer: Ximea Error Code: "<< retCode << errString << strInformation << endl;
//            if (retCode == XI_OUT_OF_RANGE)
//                throw std::out_of_range(errString.toStdString());
//            else
//                throw std::runtime_error(errString.toStdString());
//        }
//    }
};




#endif // CAMERABASLER_P_H
