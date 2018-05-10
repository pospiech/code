// CameraSMXDlg.h : Headerdatei
//

#pragma once
#include "graphctrl.h"

// CCameraSMXDlg-Dialogfeld
class CCameraSMXDlg : public CDialog
{
// Konstruktion
public:
	CCameraSMXDlg(CWnd* pParent = NULL);	// Standardkonstruktor

// Dialogfelddaten
	enum { IDD = IDD_CAMERASMX_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV-Unterstützung


// Implementierung
protected:
	HICON m_hIcon;

	// Generierte Funktionen für die Meldungstabellen
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonClose();
	afx_msg void OnBnClickedButtonGetframe();
	afx_msg void OnBnClickedButtonSetupCamera();

public:
	void ResizeDialog(const CRect newRect);

public:
	//CameraSMX Camera;	
	CGraphCtrl m_WndGraphCamera;	
};
