#pragma once
#include "afxwin.h"
#include "MFCExt.h"
#include "resource.h"

// CDlgSMXCameraSettings-Dialogfeld

class CDlgSMXCameraSettings : public CDialog
{
	DECLARE_DYNAMIC(CDlgSMXCameraSettings)

public:
	CDlgSMXCameraSettings(CWnd* pParent = NULL);   // Standardkonstruktor
	virtual ~CDlgSMXCameraSettings();

// Dialogfelddaten
	enum { IDD = IDD_DIALOG_SMXCAMERA_SETTINGS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonSettingsApply();	

private:
	void CameraSettingsToDialog();
	void DialogToCameraSettings();
	void CameraSettingsToCamera();

public:
	CEdit m_Wnd_EditDevicename;
	CEdit m_WndEditBright;
	CEdit m_WndEditContrast;
	CEdit m_WndEditGamma;
	CEdit m_WndEditMaxFramewidth;
	CEdit m_WndEditMaxFrameheight;
	CEdit m_WndEditStartX;
	CEdit m_WndEditWidthX;
	CEdit m_WndEditStartY;
	CEdit m_WndEditHeightY;
	CEdit m_WndEditExpSnap;
	CEdit m_WndEditExpSnapMs;
	CEdit m_WndEditExpVideo;
	CEdit m_WndEditExpVideoMs;
	CComboBox m_WndComboFreq;
	CComboBox m_WndComboGain;
	CComboBox m_WndComboDecimation;
	CComboBox m_WndComboColordepth;
	CComboBox m_WndComboBitdepth;
	CComboBox m_WndComboBayerAlg;
	CComboBox m_WndComboMirrorX;
	CComboBox m_WndComboMirrorY;
	CComboBox m_WndComboExpMulti;
};
