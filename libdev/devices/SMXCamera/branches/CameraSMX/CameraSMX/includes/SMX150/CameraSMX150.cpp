#include "StdAfx.h"
#include "CameraSMX150.h"


CameraSMXSettings::CameraSMXSettings(void)
{
	setBrightness(0);
	setGamma(0);
	setContrast(0);
	setBayerAlg(0);
	setUse10Bits(false);
}
CameraSMXSettings::~CameraSMXSettings(void)
{

}

CameraSMX::CameraSMX(void)
{
	CameraInitialized = false;
	m_CameraFrameArray = NULL;
	m_CameraFrameArrayRgb = NULL;
	m_DibSize = 0;
}

CameraSMX::~CameraSMX(void)
{
	if (CameraInitialized)
	{
		m_CameraSMX.CxCloseDevice(m_HandleCamera);
		m_CameraSMX.Shutdown(); //unload DLL
	}
	if (m_CameraFrameArray != NULL)
	{
		delete [] m_CameraFrameArray;
		delete [] m_CameraFrameArrayRgb;
	}

}


bool CameraSMX::Initialize()
{
	if (!CameraInitialized)
	{
		m_CameraSMX.Initialize(); //loads Camera dll and does initialisation
		m_HandleCamera = m_CameraSMX.CxOpenDevice(0); // open camera and save device
		if (m_HandleCamera == INVALID_HANDLE_VALUE)
		{
			m_CameraSMX.Shutdown(); //unload DLL
			return false;
		}
	}
	CameraInitialized = true;
	// Get Information about Camera
	UpdateSettingsWithCameraInfo();
		
	m_CameraMaxFrame.Width  = Settings.getCameraFrameSize().Width;
	m_CameraMaxFrame.Height = Settings.getCameraFrameSize().Height;
	if (m_CameraFrameArray == NULL)
	{
		m_CameraFrameArray = new char[m_CameraMaxFrame.Width  * m_CameraMaxFrame.Height ];		
		m_BufSize = m_CameraMaxFrame.Width  * m_CameraMaxFrame.Height;
	}
	
	return true;
}

void CameraSMX::UpdateSettingsWithCameraInfo()
{	
	if (!CameraInitialized) 
		return;

	m_CameraSMX.CxGetCameraInfo (m_HandleCamera,&m_CameraInfo);
	Settings.setCameraFrameSize(m_CameraInfo.MaxWidth, m_CameraInfo.MaxHeight);
	Settings.setSensortype(m_CameraInfo.SensorType);
	Settings.setDeviceName(m_CameraInfo.DeviceName);

	m_CameraSMX.CxGetScreenParams(m_HandleCamera, &m_CameraParameters);
	Settings.setColorDepth(m_CameraParameters.ColorDeep);
	Settings.setDecimation(m_CameraParameters.Decimation);
	Settings.setViewPort(CRect(m_CameraParameters.StartX, m_CameraParameters.StartY, m_CameraParameters.Width, m_CameraParameters.Height));
	Settings.setPictureMirroring(m_CameraParameters.MirrorV != 0, m_CameraParameters.MirrorH != 0);

	m_CameraSMX.CxGetCameraInfoEx(m_HandleCamera, &m_CameraInfoExtended);	

	BYTE m_Frequency;
	m_CameraSMX.CxGetFrequency(m_HandleCamera, &m_Frequency);
	Settings.setFrequency((int)m_Frequency);
	
	// ********************** Exposure ***************************** //
	int m_MinExposure = 0;
	int m_MaxExposure = 0;
	m_CameraSMX.CxGetExposureMinMax(m_HandleCamera, &m_MinExposure, &m_MaxExposure);
	Settings.setExposureLimits(m_MinExposure, m_MaxExposure);
	
	float m_MinExposureMs = 0;
	float m_MaxExposureMs = 0;
	m_CameraSMX.CxGetExposureMinMaxMs(m_HandleCamera, &m_MinExposureMs, &m_MaxExposureMs);
	Settings.setExposureLimitsMs(m_MinExposureMs, m_MaxExposureMs);
	
	m_CameraSMX.CxGetSnapshotExposureMinMax(m_HandleCamera, & m_MinExposure, & m_MaxExposure);
	Settings.setSnapshotExposureLimits(m_MinExposure,  m_MaxExposure);
	
	m_CameraSMX.CxGetSnapshotExposureMinMaxMs(m_HandleCamera, & m_MinExposureMs, & m_MaxExposureMs);
	Settings.setSnapshotExposureLimitsMs(m_MinExposureMs,  m_MaxExposureMs);

	int m_Exposure;
	m_CameraSMX.CxGetExposure(m_HandleCamera, & m_Exposure);
	Settings.setExposure(m_Exposure);

	float m_ExposureMs;
	m_CameraSMX.CxGetExposureMs(m_HandleCamera, & m_ExposureMs);
	Settings.setExposureMs(m_ExposureMs);

	int m_SnapshotExp=0;
	m_CameraSMX.CxGetSnapshotExposure(m_HandleCamera, & m_SnapshotExp);
	Settings.setSnapshotExposure(m_SnapshotExp);

	float m_SnapshotExpMs=0;	
	m_CameraSMX.CxGetSnapshotExposureMs(m_HandleCamera, & m_SnapshotExpMs);
	Settings.setSnapshotExposureMs(m_SnapshotExpMs);
	
	DWORD m_Value;
	m_CameraSMX.CxGetSnapshotExposureMultiplier(m_HandleCamera, & m_Value);
	Settings.setSnapshotExposureMultiplier((int)m_Value);

	// *** Gain ****
	int m_GainMono;
	int m_GainR;
	int m_GainG;
	int m_GainB;
	m_CameraSMX.CxGetGain(m_HandleCamera, & m_GainMono, & m_GainR,& m_GainG,& m_GainB);
	Settings.setGain(m_GainMono);

	// *** 10 bits ****
	
	BOOL m_Use10Bits;
	m_CameraSMX.CxGet10BitsOutput(m_HandleCamera, & m_Use10Bits);
	Settings.setUse10Bits(m_Use10Bits != 0);


	//m_CameraSMX.CxGetStreamMode(m_HandleCamera, BYTE *StreamMode);
	//m_CameraSMX.CxGetFramePtr(m_HandleCamera);	
	// Reads 1024 bit table for convertion of 10bit -> 8bit
	//m_CameraSMX.CxGetConvertionTab(m_HandleCamera, BYTE *Buf1024); 	
	// Reads current frame rate for video
	//m_CameraSMX.CxGetFrameCounter(int DeviceId,long *FrameCounter);
	//m_CameraSMX.CxGetDACRawOffset (m_HandleCamera, BYTE *Raw);
	//m_CameraSMX.CxGetDACFineOffset(m_HandleCamera, BYTE *Fine);
	//m_CameraSMX.CxGetControlReg (m_HandleCamera, BYTE *Value,  BYTE ControlRegNo);
	//m_CameraSMX.CxGetLineBlank(m_HandleCamera, WORD *Blank);
	//m_CameraSMX.CxGetHBlank(m_HandleCamera, WORD *Blank);
	//m_CameraSMX.CxGetFrameRate(m_HandleCamera, float *FrameRate);
	//m_CameraSMX.CxGetVDDC(m_HandleCamera, BOOL *VDDC);
	//m_CameraSMX.CxGetVDDH(m_HandleCamera, BOOL *VDDH);
	//m_CameraSMX.CxGetADCSampleDelay(m_HandleCamera, int* Delay );
	//m_CameraSMX.CxGetStatistics(m_HandleCamera, int* Frames, int *BadLines );

}

void CameraSMX::TransmitSettingsToCamera()
{	
	if (!CameraInitialized) 
		return;

	//m_CameraSMX.CxSetStreamMode(m_HandleCamera, BYTE  StreamMode);

	UpdateScreenParameters(); //CxSetScreenParams
	//m_CameraParameters.ColorDeep = 	Settings.getColorDepth();
	//m_CameraParameters.Decimation = Settings.getDecimation();
	//m_CameraParameters.StartX = Settings.getViewPort().left;
	//m_CameraParameters.StartY = Settings.getViewPort().top;
	//m_CameraParameters.Width = Settings.getViewPort().Width();
	//m_CameraParameters.Height = Settings.getViewPort().Height();
	//m_CameraParameters.MirrorV = Settings.getPictureMirroring().Vertical;
	//m_CameraParameters.MirrorH = Settings.getPictureMirroring().Horizontal;	
	//m_CameraSMX.CxSetScreenParams(m_HandleCamera, &m_CameraParameters);
	
	BYTE value = (BYTE)Settings.getFrequency();
	m_CameraSMX.CxSetFrequency(m_HandleCamera, value);
	m_CameraSMX.CxSetExposure(m_HandleCamera, Settings.getExposure());
	float ExposureSet;
	m_CameraSMX.CxSetExposureMs(m_HandleCamera, Settings.getExposureMs(), & ExposureSet);
	m_CameraSMX.CxSetGain(m_HandleCamera,Settings.getGain(), 0, 0, 0);
	m_CameraSMX.CxSetBrightnessContrastGamma(m_HandleCamera,
		Settings.getBrightness(),
		Settings.getContrast(),
		Settings.getGamma());
	m_CameraSMX.CxSetSnapshotExposure(m_HandleCamera,Settings.getSnapshotExposure());
	float ExpSet;
	m_CameraSMX.CxSetSnapshotExposureMs(m_HandleCamera,Settings.getSnapshotExposureMs(),& ExpSet);
	m_CameraSMX.CxSetSnapshotExposureMultiplier(m_HandleCamera, (DWORD) Settings.getSnapshotExposureMultiplier());
	m_CameraSMX.CxSet10BitsOutput(m_HandleCamera, (BOOL)Settings.getUse10Bits());
	m_CameraSMX.CxSetBayerAlg(Settings.getBayerAlg());

	//m_CameraSMX.CxSetDACRawOffset (m_HandleCamera, BYTE  Raw);
	//m_CameraSMX.CxSetDACFineOffset(m_HandleCamera, BYTE  Fine);
	//m_CameraSMX.CxSetConvertionTab(m_HandleCamera, BYTE *Buf1024);
	//m_CameraSMX.CxSetDefaultConvertionTab(m_HandleCamera);	
	//m_CameraSMX.CxSetContinuousPrecharge(m_HandleCamera, BOOL  ContinuousPrecharge);
	//m_CameraSMX.CxSetTristateOut(m_HandleCamera, BOOL  TristateOut);                               
	//m_CameraSMX.CxSetControlReg (m_HandleCamera, BYTE  Value, BYTE Mask, BYTE ControlRegNo);
	//m_CameraSMX.CxSetLineBlank(m_HandleCamera, WORD  Blank);
	//m_CameraSMX.CxSetHBlank(m_HandleCamera, WORD  Blank);
	//m_CameraSMX.CxSetFrameRate(m_HandleCamera, float  FrameRate, int *ResultCode);              
	//m_CameraSMX.CxSetMaxFrameRate(m_HandleCamera);
	//m_CameraSMX.CxSetDefaultFrameRate(m_HandleCamera);                
	//m_CameraSMX.CxSetVDDC(m_HandleCamera, BOOL  VDDC);                         
	//m_CameraSMX.CxSetVDDH(m_HandleCamera, BOOL  VDDH);               
	//m_CameraSMX.CxSetADCSampleDelay(m_HandleCamera, int  Delay );
}

char* CameraSMX::GetFrame()
{
	return m_CameraFrameArrayRgb;
}

void CameraSMX::GrabVideoFrame()
{
	if (!CameraInitialized) 
		return;

	m_CameraSMX.CxGrabVideoFrame(m_HandleCamera, m_CameraFrameArray, m_BufSize);
}


void CameraSMX::GetSnapshot()
{
	if (!CameraInitialized) 
		return;

	//if (m_CameraFrameArray == NULL)
	//{
	//	m_CameraFrameArray = new char[m_CameraMaxFrame.Width  * m_CameraMaxFrame.Height ];		
	//}

	int iTimeOut=1;
	bool boolTriggerEnabled = false;
	bool boolTriggerPosEnabled = false;
	DWORD retlength;
	int result = m_CameraSMX.CxGetSnapshot(m_HandleCamera, 
		iTimeOut, 
		boolTriggerEnabled, 
		boolTriggerPosEnabled, 
		true, 
		m_CameraFrameArray, 
		m_BufSize, 
		&retlength);
	
	//int checksize = getFrameSize().Width * getFrameSize().Height;
	//if (retlength != checksize)
	//{
	//	//AfxMessageBox("Error while getting Snapshot");
	//	//return;
	//}
	if (retlength == 0) {
		AfxMessageBox("Error: Snapshot with empty frame");
		return;
	}	
	BayerToRgb();
}

void CameraSMX::UpdateScreenParameters()
{
	if (!CameraInitialized) 
		return;

	m_CameraParameters.ColorDeep = Settings.getColorDepth();
	m_CameraParameters.Decimation = Settings.getDecimation();
	m_CameraParameters.StartX = Settings.getViewPort().left;
	m_CameraParameters.StartY = Settings.getViewPort().top;
	m_CameraParameters.Width = Settings.getViewPort().Width();
	m_CameraParameters.Height = Settings.getViewPort().Height();
	m_CameraParameters.MirrorV = Settings.getPictureMirroring().Vertical;
	m_CameraParameters.MirrorH = Settings.getPictureMirroring().Horizontal;
	
	m_CameraSMX.CxSetScreenParams(m_HandleCamera, & m_CameraParameters);
	
	m_CameraSMX.CxActivateScreenParams(m_HandleCamera);
}

void CameraSMX::BayerToRgb()
{		
	if (Settings.getColorDepth() == 8)
	{
		m_DibSize = getFrameSize().Width * getFrameSize().Height;
		if (m_CameraFrameArrayRgb!=NULL) { delete [] m_CameraFrameArrayRgb; }
		m_CameraFrameArrayRgb = new char[m_BufSize];
		m_CameraFrameArrayRgb = (char*)m_CameraFrameArray;
		
	} else {
		m_DibSize = getFrameSize().Width * getFrameSize().Height * 3;
		if (m_CameraFrameArrayRgb!=NULL) { delete [] m_CameraFrameArrayRgb; }		
		m_CameraFrameArrayRgb = new char[m_BufSize * 3];
		m_CameraFrameArrayRgb = (char*)m_CameraSMX.CxBayerToRgb(
			m_CameraFrameArray,
			getFrameSize().Width,
			getFrameSize().Height,
			Settings.getBayerAlg(),
			(TRgbPix*)m_CameraFrameArrayRgb);	
	}

	//if (m_CameraFrameArray != NULL)
	//{
	//	delete [] m_CameraFrameArray;
	//	m_CameraFrameArray = NULL;
	//}

}

//
//BOOL  SMX150_API CxGrabVideoFrame(m_HandleCamera,void *Buffer,int BufSize);
//PVOID SMX150_API CxGetFramePtr(m_HandleCamera);
//m_CameraSMX.CxStartVideo(HDC WindowDC,int DeviceId);
//m_CameraSMX.CxStopVideo(int DeviceId);
//m_CameraSMX.CxCancelSnapshot(m_HandleCamera);
//m_CameraSMX.CxElectricalBlack(m_HandleCamera, BOOL ElectricalBlack);
//m_CameraSMX.CxWriteSensorReg(m_HandleCamera, DWORD RegNo, DWORD  Value);
//m_CameraSMX.CxReadSensorReg (m_HandleCamera, DWORD RegNo, DWORD *Value);
//

int CameraSMX::GetDibSize()
{
	return m_DibSize;
}

int CameraSMX::GetBufferSize()
{
	return m_BufSize; 
}

int CameraSMX::GetRgbBufferSize()
{
	if (Settings.getColorDepth() == 8)
	{
		return m_BufSize; 
	} else {
		return m_BufSize * 3; 
	}
}



//void CameraSMX::SaveFrameToFile(CString szPathName)
//{
//	int xdim = getFrameSize().Width;
//	int ydim = getFrameSize().Height;
//	int m_Textwidth = 10;
//	char* Filename  = szPathName.GetBuffer(szPathName.GetLength());	
//	ofstream FileOut;
//	FileOut.open(Filename);
//	FileOut.flags(ios::right);
//	if (FileOut)  
//	{
//		for (int iy=0; iy < ydim; iy++)
//		{			
//			for (int ix=0; ix < xdim; ix++)
//			{						
//				FileOut << setw(m_Textwidth) << m_CameraFrameArray[iy + ydim * ix];
//			}
//			FileOut << endl;
//		}
//	}
//	else 
//	{  
//	    AfxMessageBox("Datei konnte nicht geöffnet werden");		
//	}
//	FileOut.close();
//	szPathName.ReleaseBuffer();
//}

CStructSize<int> CameraSMX::getFrameSize()
{
	CStructSize<int> FrameSize;
	FrameSize.Width = Settings.getViewPort().Width();
	FrameSize.Height = Settings.getViewPort().Height();
	return FrameSize;
}

bool CameraSMX::Initialized()
{
	return CameraInitialized;
}