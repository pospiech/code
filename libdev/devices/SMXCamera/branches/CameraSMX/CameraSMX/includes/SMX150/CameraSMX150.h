#pragma once

#include "TypeExt.h"
#include "SMX150.h"
#include "DlgSMXCameraSettings.h"

////////////////////////////////////////////////////////////////////////
class CameraSMXSettings
{
public:
	CameraSMXSettings();   
	~CameraSMXSettings();

private:
	bool checkMinMax(int value, int min, int max)
	{
		if ((value >= min) && (value <= max))
		{
			return true;
		} else {
			return false;
		}
	}
	bool checkMinMax(float value, float min, float max)
	{
		if ((value >= min) && (value <= max))
		{
			return true;
		} else {
			return false;
		}
	}

public:

	void setFrequency(const int value){ 
		m_Frequency = value;
	}
	void setGain(const int value){ m_Gain = value; }
	void setSnapshotExposureLimits(const int minvalue, const int maxvalue)
	{ 
		m_SnapshotExposureLimits.Min = minvalue;
		m_SnapshotExposureLimits.Max = maxvalue;
	}
	void setSnapshotExposureLimitsMs(const float minvalue, const float maxvalue)
	{ 
		m_SnapshotExposureLimitsMs.Min = minvalue;
		m_SnapshotExposureLimitsMs.Max = maxvalue;
	}

	void setExposureLimits(const int minvalue, const int maxvalue)
	{ 
		m_ExposureLimits.Min = minvalue;
		m_ExposureLimits.Max = maxvalue;
	}
	void setExposureLimitsMs(const float minvalue, const float maxvalue)
	{ 
		m_ExposureLimitsMs.Min = minvalue;
		m_ExposureLimitsMs.Max = maxvalue;
	}
	void setSnapshotExposureMs(const float value)
	{ 
		if (checkMinMax(value, m_SnapshotExposureLimitsMs.Min, m_SnapshotExposureLimitsMs.Max))
		{
			m_SnapshotExposureMs  = value; 
		}
	}
	void setSnapshotExposure(const int value)
	{
		if (checkMinMax(value, m_SnapshotExposureLimits.Min, m_SnapshotExposureLimits.Max))
		{
			m_SnapshotExposure = value; 
		}		
	}
	void setExposure(const int value)
	{ 
		if (checkMinMax(value, m_ExposureLimits.Min, m_ExposureLimits.Max))
		{
			m_Exposure = value; 
		}				
	}
	void setExposureMs(const float value)
	{
		if (checkMinMax(value, m_ExposureLimitsMs.Min, m_ExposureLimitsMs.Max))
		{
			m_ExposureMs = value; 
		}				
	}	
	void setSnapshotExposureMultiplier(const int value)
	{ 
		if ((value >= 0) && (value < 4))
		{
			m_SnapshotExposureMultiplier = value; 
		}
	}
	void setPictureMirroring(const bool vertvalue, const bool horzvalue)
	{
		m_PictureMirroring.Vertical = vertvalue; 
		m_PictureMirroring.Horizontal = horzvalue; 
	}
	void setDecimation(const int value)
	{ 
		if ((value == 1) || (value == 2))
		{
			m_Decimation = value;
		}
	}
	void setBrightness(const int value)
	{ 
		if (checkMinMax(value,-127,127)) 
		{
			m_Brightness = value; 
		}
	}
	void setGamma(const int value)
	{ 
		if (checkMinMax(value,-127,127)) { m_Gamma = value; }
	}
	void setContrast(const int value)
	{ 
		if (checkMinMax(value,-127,127)) { m_Contrast = value; }
	}
	void setDeviceName(char* value)
	{
		strncpy(m_DeviceName, value, 64); 
	}
	void setCameraFrameSize(const int widthvalue, const int heightvalue)
	{
		m_CameraFrameSize.Width = widthvalue; 
		m_CameraFrameSize.Height = heightvalue; 
	}
	void setViewPort(const CRect value){ m_ViewPort = value; }
	void setSensortype(const int value){ m_Sensortype = value; }
	void setColorDepth(const int value)
	{
		if ((value == 8) || (value == 24))
		{
			m_ColorDepth = value; 
		}
	}
	void setUse10Bits(const bool value){ m_Use10Bits = value; }
	void setBayerAlg(const int value){ m_BayerAlg = value;}

	/* ////////////// Get ////////////// */
	int getFrequency(){ return m_Frequency; }	
	int getGain(){ return m_Gain; }
	CStructLimits<int>	 getSnapshotExposureLimits()   { return m_SnapshotExposureLimits; 	}
	CStructLimits<float> getSnapshotExposureLimitsMs() { return m_SnapshotExposureLimitsMs; 	}
	CStructLimits<int>	 getExposureLimits() 		   { return m_ExposureLimits; 	}
	CStructLimits<float> getExposureLimitsMs()		   { return m_ExposureLimitsMs; }
	float getSnapshotExposureMs() { return m_SnapshotExposureMs; }
	int   getSnapshotExposure()   { return m_SnapshotExposure; }
	int   getExposure()	  { return m_Exposure; }
	float getExposureMs() { return m_ExposureMs; }
	int getSnapshotExposureMultiplier(){ return m_SnapshotExposureMultiplier; }
	CStructVertHorz<BOOLEAN> getPictureMirroring() { return m_PictureMirroring; }
	int getDecimation(){ return m_Decimation; }
	int getBrightness(){ return m_Brightness; }
	int getGamma(){  return m_Gamma; }
	int getContrast(){  return m_Contrast; }
	char* getDeviceName(){ return m_DeviceName; }
	CStructSize<int> getCameraFrameSize(){ return m_CameraFrameSize; }
	CRect getViewPort(){ return m_ViewPort; }
	int getSensortype(){ return m_Sensortype; }
	int getColorDepth(){ return m_ColorDepth; }
	bool getUse10Bits(){ return m_Use10Bits; }
	int getBayerAlg(){ return m_BayerAlg;}
private:
	int m_Frequency;
	int m_Gain;
	float m_SnapshotExposureMs;
	int  m_SnapshotExposure;	
	float m_ExposureMs;
	int m_Exposure;
	CStructLimits<int> m_SnapshotExposureLimits;
	CStructLimits<float> m_SnapshotExposureLimitsMs;
	CStructLimits<int> m_ExposureLimits;
	CStructLimits<float> m_ExposureLimitsMs;
	int m_SnapshotExposureMultiplier;
	CStructVertHorz<BOOLEAN> m_PictureMirroring;
	int m_Decimation; // 1 or 2
	int m_Brightness;
	int m_Gamma;
	int m_Contrast;
	char m_DeviceName[64];	
	CStructSize<int> m_CameraFrameSize;
	CRect m_ViewPort;
	int m_Sensortype; // 0 black, 1 color
	int m_ColorDepth;
	bool m_Use10Bits;
	int m_BayerAlg;
};

////////////////////////////////////////////////////////////////////////
class CameraSMX
{
public:
	CameraSMX(void);   
	~CameraSMX(void);

private:
	bool CameraInitialized;
	CStructSize<int> m_CameraFrame;
	CStructSize<int> m_CameraMaxFrame;
	CSumixCam m_CameraSMX;
	HANDLE m_HandleCamera; 
	TFrameParams m_CameraParameters;
	TCameraInfo m_CameraInfo;
	TCameraInfoEx m_CameraInfoExtended;
	
	char* m_CameraFrameArrayRgb;	
	char* m_CameraFrameArray;
	int m_BufSize;
	int m_DibSize;

public:
	
	CameraSMXSettings Settings;
	bool Initialized();
	bool Initialize();
	void UpdateSettingsWithCameraInfo();
	void TransmitSettingsToCamera();
	char* GetFrame();
	int GetDibSize();
	int GetBufferSize();
	int GetRgbBufferSize();
	void GrabVideoFrame();
	void GetSnapshot();
	void UpdateScreenParameters();
	void SaveFrameToFile(CString szPathName);
	CStructSize<int> getFrameSize();
	void BayerToRgb();
};



