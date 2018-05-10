// CameraSMX.h : Hauptheaderdatei für die PROJECT_NAME-Anwendung
//

#pragma once

#ifndef __AFXWIN_H__
	#error "\"stdafx.h\" vor dieser Datei für PCH einschließen"
#endif

#include "resource.h"		// Hauptsymbole
#include "CameraSMX150.h"
#include "DlgSMXCameraSettings.h"
#include "CameraSMXDlg.h"

// CCameraSMXApp:
// Siehe CameraSMX.cpp für die Implementierung dieser Klasse
//

class CCameraSMXApp : public CWinApp
{
public:
	CCameraSMXApp();

// Überschreibungen
	public:
	virtual BOOL InitInstance();

// Implementierung
	CCameraSMXDlg dlg;
	CDlgSMXCameraSettings m_DlgSMXCameraSettings;

	DECLARE_MESSAGE_MAP()
};

extern CCameraSMXApp theApp;

extern CameraSMX Camera;