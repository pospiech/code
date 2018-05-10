// DlgSMXCameraSettings.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "CameraSMX.h"
#include "DlgSMXCameraSettings.h"


// CDlgSMXCameraSettings-Dialogfeld

IMPLEMENT_DYNAMIC(CDlgSMXCameraSettings, CDialog)

CDlgSMXCameraSettings::CDlgSMXCameraSettings(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSMXCameraSettings::IDD, pParent)
{

}

CDlgSMXCameraSettings::~CDlgSMXCameraSettings()
{
}

void CDlgSMXCameraSettings::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_DEVICENAME, m_Wnd_EditDevicename);
	DDX_Control(pDX, IDC_COMBO_FREQ, m_WndComboFreq);
	DDX_Control(pDX, IDC_COMBO_GAIN, m_WndComboGain);
	DDX_Control(pDX, IDC_COMBO_DECIMATION, m_WndComboDecimation);
	DDX_Control(pDX, IDC_EDIT_BRIGHT, m_WndEditBright);
	DDX_Control(pDX, IDC_EDIT_CONTRAST, m_WndEditContrast);
	DDX_Control(pDX, IDC_EDIT_GAMMA, m_WndEditGamma);
	DDX_Control(pDX, IDC_COMBO_COLORDEPTH, m_WndComboColordepth);
	DDX_Control(pDX, IDC_COMBO_BITDEPTH, m_WndComboBitdepth);
	DDX_Control(pDX, IDC_COMBO_BAYER_ALG, m_WndComboBayerAlg);
	DDX_Control(pDX, IDC_EDIT_MAXFRAMEWIDTH, m_WndEditMaxFramewidth);
	DDX_Control(pDX, IDC_EDIT_MAXFRAMEHEIGTH, m_WndEditMaxFrameheight);
	DDX_Control(pDX, IDC_EDIT_START_X, m_WndEditStartX);
	DDX_Control(pDX, IDC_EDIT_WIDTH_X, m_WndEditWidthX);
	DDX_Control(pDX, IDC_EDIT_START_Y, m_WndEditStartY);
	DDX_Control(pDX, IDC_EDIT_HEIGHT_Y, m_WndEditHeightY);
	DDX_Control(pDX, IDC_COMBO_MIRROR_X, m_WndComboMirrorX);
	DDX_Control(pDX, IDC_COMBO_MIRROR_Y, m_WndComboMirrorY);
	DDX_Control(pDX, IDC_EDIT_EXP_SNAP, m_WndEditExpSnap);
	DDX_Control(pDX, IDC_EDIT_EXP_SNAP_MS, m_WndEditExpSnapMs);
	DDX_Control(pDX, IDC_EDIT_EXP_VIDEO, m_WndEditExpVideo);
	DDX_Control(pDX, IDC_EDIT_EXP_VIDEO_MS, m_WndEditExpVideoMs);
	DDX_Control(pDX, IDC_COMBO_EXP_MULTI, m_WndComboExpMulti);
}


BEGIN_MESSAGE_MAP(CDlgSMXCameraSettings, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgSMXCameraSettings::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_SETTINGS_APPLY, &CDlgSMXCameraSettings::OnBnClickedButtonSettingsApply)
END_MESSAGE_MAP()


// CDlgSMXCameraSettings-Meldungshandler

BOOL CDlgSMXCameraSettings::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_WndComboFreq.AddString("3");
	m_WndComboFreq.AddString("6");
	m_WndComboFreq.AddString("8");
	m_WndComboFreq.AddString("10");
	m_WndComboFreq.AddString("12");
	m_WndComboFreq.AddString("13");
	m_WndComboFreq.AddString("16");
	m_WndComboFreq.AddString("20");
	m_WndComboFreq.AddString("24");
	m_WndComboFreq.AddString("40");
	m_WndComboFreq.AddString("48");
	
	for (int i=0; i<=16; i++)
	{
		CString buffer;
		buffer.Format("%d",i);
		m_WndComboGain.AddString(buffer);
	}

	m_WndComboDecimation.AddString("1 (volle Größe)");
	m_WndComboDecimation.AddString("2 (Viertel)");
		
	m_WndComboColordepth.AddString("8");
	m_WndComboColordepth.AddString("24");

	m_WndComboBitdepth.AddString("8");
	m_WndComboBitdepth.AddString("10");

	m_WndComboBayerAlg.AddString("0 : Monochrome");
	m_WndComboBayerAlg.AddString("1 : Nearest Neighbor");
	m_WndComboBayerAlg.AddString("2 : Bilinear");
	m_WndComboBayerAlg.AddString("3 : Laplacian");
	m_WndComboBayerAlg.AddString("4 : Real Monochrome");
	m_WndComboBayerAlg.AddString("5 : Bayer Average");
		
	m_WndComboMirrorX.AddString("X Achse nicht spiegeln");
	m_WndComboMirrorX.AddString("X Achse spiegeln");	

	m_WndComboMirrorY.AddString("Y Achse nicht spiegeln");
	m_WndComboMirrorY.AddString("Y Achse spiegeln");

	m_WndComboExpMulti.AddString("1x ");
	m_WndComboExpMulti.AddString("2x ");
	m_WndComboExpMulti.AddString("4x ");
	m_WndComboExpMulti.AddString("8x ");
	CameraSettingsToDialog();
	return TRUE;  
}

void CDlgSMXCameraSettings::CameraSettingsToDialog()
{
	if (Camera.Initialized())
	{
		char* charbuffer = Camera.Settings.getDeviceName();
		CString cbuffer = charbuffer;
		m_Wnd_EditDevicename.SetWindowText(cbuffer);	
		
		int value = Camera.Settings.getFrequency();
		switch (value)
		{
			case 0: value = 9; break;
			case 1: value = 8; break;
			case 2: value = 7; break;
			case 3: value = 6; break;
			case 4: value = 5; break;
			case 5: value = 4; break;
			case 6: value = 3; break;
			case 7: value = 2; break;
			case 8: value = 1; break;
			case 9: value = 0; break;
			case 10: value = 10; break;
		}
		
		//if	(value == 3){ m_Frequency = 9; }
		//if 	(value == 6) { m_Frequency = 8; }
		//if 	(value == 8) { m_Frequency = 7; }
		//if 	(value == 10) { m_Frequency = 6; }
		//if 	(value == 12) { m_Frequency = 5; }
		//if 	(value == 13) { m_Frequency = 4; }
		//if 	(value == 16) { m_Frequency = 3; }
		//if 	(value == 20) { m_Frequency = 2; }
		//if 	(value == 24) { m_Frequency = 1; }
		//if 	(value == 40) { m_Frequency = 0; }
		//if 	(value == 48) { m_Frequency = 10; }

		m_WndComboFreq.SetCurSel(value);

		value = Camera.Settings.getGain();
		m_WndComboGain.SetCurSel(value);

		m_WndComboDecimation.SetCurSel(Camera.Settings.getDecimation() - 1);
		
		CString buffer;
		buffer.Format("%d", Camera.Settings.getBrightness());
		m_WndEditBright.SetWindowText(buffer);

		buffer.Format("%d", Camera.Settings.getContrast());
		m_WndEditContrast.SetWindowText(buffer);

		buffer.Format("%d", Camera.Settings.getGamma());
		m_WndEditGamma.SetWindowText(buffer);
	
		if (Camera.Settings.getColorDepth() == 8)
		{
			m_WndComboColordepth.SetCurSel(0);
		} else {
			m_WndComboColordepth.SetCurSel(1);
		}

		if (Camera.Settings.getUse10Bits() == false)
		{
			m_WndComboBitdepth.SetCurSel(0);
		} else {
			m_WndComboBitdepth.SetCurSel(1);
		}

		m_WndComboBayerAlg.SetCurSel(Camera.Settings.getBayerAlg());
	
		buffer.Format("%d",Camera.Settings.getCameraFrameSize().Width);
		m_WndEditMaxFramewidth.SetWindowText(buffer);
		buffer.Format("%d",Camera.Settings.getCameraFrameSize().Height);
		m_WndEditMaxFrameheight.SetWindowText(buffer);
	
		buffer.Format("%d",Camera.Settings.getViewPort().left);
		m_WndEditStartX.SetWindowText(buffer);
		buffer.Format("%d",Camera.Settings.getViewPort().top);
		m_WndEditStartY.SetWindowText(buffer);
		buffer.Format("%d",Camera.Settings.getViewPort().Width());
		m_WndEditWidthX.SetWindowText(buffer);
		buffer.Format("%d",Camera.Settings.getViewPort().Height());
		m_WndEditHeightY.SetWindowText(buffer);
	
		if (Camera.Settings.getPictureMirroring().Horizontal)
		{
			m_WndComboMirrorX.SetCurSel(1);
		} else {
			m_WndComboMirrorX.SetCurSel(0);
		}
		
		if (Camera.Settings.getPictureMirroring().Vertical)
		{
			m_WndComboMirrorY.SetCurSel(1);
		} else {
			m_WndComboMirrorY.SetCurSel(0);
		}

		buffer.Format("%d",	Camera.Settings.getSnapshotExposure());
		m_WndEditExpSnap.SetWindowText(buffer);
		buffer.Format("%f",	Camera.Settings.getSnapshotExposureMs());
		m_WndEditExpSnapMs.SetWindowText(buffer);
		buffer.Format("%d",	Camera.Settings.getExposure());
		m_WndEditExpVideo.SetWindowText(buffer);
		buffer.Format("%f",	Camera.Settings.getExposureMs());
		m_WndEditExpVideoMs.SetWindowText(buffer);

		m_WndComboExpMulti.SetCurSel(Camera.Settings.getSnapshotExposureMultiplier());
	}
}

void CDlgSMXCameraSettings::DialogToCameraSettings()
{
	if (Camera.Initialized())
	{		
		
		int value;
		switch (m_WndComboFreq.GetCurSel())
		{
			case 9: value = 0; break;
			case 8: value = 1; break;
			case 7: value = 2; break;
			case 6: value = 3; break;
			case 5: value = 4; break;
			case 4: value = 5; break;
			case 3: value = 6; break;
			case 2: value = 7; break;
			case 1: value = 8; break;
			case 0: value = 9; break;
			case 10: value = 10; break;
		}
		Camera.Settings.setFrequency(value);
		Camera.Settings.setGain(m_WndComboGain.GetCurSel());
		Camera.Settings.setDecimation(m_WndComboDecimation.GetCurSel() + 1);
		
		CString buffer;
		m_WndEditBright.GetWindowText(buffer);
		int intValue;
		intValue = CStringToInt(buffer);
		intValue = CorrectRangeMinMax(intValue, -127, 127);
		Camera.Settings.setBrightness(intValue);


		m_WndEditContrast.GetWindowText(buffer);
		intValue = CStringToInt(buffer);
		intValue = CorrectRangeMinMax(intValue, -127, 127);
		Camera.Settings.setContrast(intValue);

		m_WndEditGamma.GetWindowText(buffer);
		intValue = CStringToInt(buffer);
		intValue = CorrectRangeMinMax(intValue, -127, 127);
		Camera.Settings.setGamma(intValue);		
	
		if (m_WndComboColordepth.GetCurSel() == 0)
		{
			Camera.Settings.setColorDepth(8); 
		} else {
			Camera.Settings.setColorDepth(24);
		}

		if (m_WndComboBitdepth.GetCurSel() == 0)
		{
			Camera.Settings.setUse10Bits(false);
		} else {
			Camera.Settings.setUse10Bits(true);
		}

		Camera.Settings.setBayerAlg(m_WndComboBayerAlg.GetCurSel());
		
		CRect rc;
		m_WndEditStartX.GetWindowText(buffer);
		intValue = CStringToInt(buffer);		
		intValue = CorrectRangeMinMax(intValue, 0, Camera.Settings.getCameraFrameSize().Width);
		rc.left = intValue;


		m_WndEditStartY.GetWindowText(buffer);
		intValue = CStringToInt(buffer);		
		intValue = CorrectRangeMinMax(intValue, 0, Camera.Settings.getCameraFrameSize().Height);
		rc.top = intValue;


		m_WndEditWidthX.GetWindowText(buffer);
		intValue = CStringToInt(buffer);		
		intValue = CorrectRangeMinMax(intValue, 1, Camera.Settings.getCameraFrameSize().Width - rc.left);
		rc.right = rc.left + intValue;


		m_WndEditHeightY.GetWindowText(buffer);
		intValue = CStringToInt(buffer);		
		intValue = CorrectRangeMinMax(intValue, 1, Camera.Settings.getCameraFrameSize().Height - rc.top);
		rc.bottom = rc.top + intValue;

		Camera.Settings.setViewPort(rc);

		bool MirrorHorz;
		bool MirrorVert;
		if (m_WndComboMirrorX.GetCurSel() == 1)
		{			
			MirrorHorz = true;			
		} else {
			MirrorHorz = false;
		}		
		if (m_WndComboMirrorY.GetCurSel() == 1)
		{
			MirrorVert = true;
		} else {
			MirrorVert = false;
		}
		Camera.Settings.setPictureMirroring(MirrorVert, MirrorHorz);
		
		m_WndEditExpSnap.GetWindowText(buffer);
		intValue = CStringToInt(buffer);		
		intValue = CorrectRangeMinMax(intValue, Camera.Settings.getSnapshotExposureLimits().Min, Camera.Settings.getSnapshotExposureLimits().Max);
		Camera.Settings.setSnapshotExposure(intValue);

		float fltValue;		
		m_WndEditExpSnapMs.GetWindowText(buffer);
		fltValue = CStringToFloat(buffer);		
		fltValue = CorrectRangeMinMax(fltValue, Camera.Settings.getSnapshotExposureLimitsMs().Min, Camera.Settings.getSnapshotExposureLimitsMs().Max);
		Camera.Settings.setSnapshotExposureMs(fltValue);

		m_WndEditExpVideo.GetWindowText(buffer);
		intValue = CStringToInt(buffer);		
		intValue = CorrectRangeMinMax(intValue, Camera.Settings.getExposureLimits().Min, Camera.Settings.getExposureLimits().Max);		
		Camera.Settings.setExposure(intValue);

		m_WndEditExpVideoMs.GetWindowText(buffer);
		fltValue = CStringToFloat(buffer);		
		fltValue = CorrectRangeMinMax(fltValue, Camera.Settings.getExposureLimitsMs().Min, Camera.Settings.getExposureLimitsMs().Max);
		Camera.Settings.setExposureMs(fltValue);
		
		Camera.Settings.setSnapshotExposureMultiplier(m_WndComboExpMulti.GetCurSel());
	}
}

void CDlgSMXCameraSettings::CameraSettingsToCamera()
{
	if (Camera.Initialized())
	{		
		DialogToCameraSettings();
		Camera.TransmitSettingsToCamera();
	}
}

void CDlgSMXCameraSettings::OnBnClickedOk()
{
	OnOK();
}

void CDlgSMXCameraSettings::OnBnClickedButtonSettingsApply()
{
	CameraSettingsToCamera();
}
