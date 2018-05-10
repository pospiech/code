// CameraSMXDlg.cpp : Implementierungsdatei
//

#include "stdafx.h"
#include "CameraSMX.h"
#include "CameraSMXDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCameraSMXDlg-Dialogfeld

CCameraSMXDlg::CCameraSMXDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCameraSMXDlg::IDD, pParent)
{
	//m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCameraSMXDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCameraSMXDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CCameraSMXDlg::OnBnClickedButtonClose)
	ON_BN_CLICKED(IDC_BUTTON_GETFRAME, &CCameraSMXDlg::OnBnClickedButtonGetframe)
	ON_BN_CLICKED(IDC_BUTTON_SETUP_CAMERA, &CCameraSMXDlg::OnBnClickedButtonSetupCamera)
END_MESSAGE_MAP()


// CCameraSMXDlg-Meldungshandler

BOOL CCameraSMXDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	bool result = Camera.Initialize();
	if (result)
	{		
		m_WndGraphCamera.SetSize(Camera.Settings.getViewPort().Width(), Camera.Settings.getViewPort().Height());
		m_WndGraphCamera.SetPlotDataSize(Camera.Settings.getViewPort().Width(), Camera.Settings.getViewPort().Height());
		//m_WndGraphCamera.SetAnalyseData(true);
		//m_WndGraphCamera.SetPlotDataType(0); 
	}
	return TRUE;  
}

void CCameraSMXDlg::OnPaint()
{
	CDialog::OnPaint();
}

// Die System ruft diese Funktion auf, um den Cursor abzufragen, der angezeigt wird, während der Benutzer
//  das minimierte Fenster mit der Maus zieht.
HCURSOR CCameraSMXDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CCameraSMXDlg::OnBnClickedButtonGetframe()
{
	if (Camera.Initialized())
	{
		Camera.GetSnapshot();
		m_WndGraphCamera.SetDIB(Camera.GetFrame(),Camera.GetDibSize(), Camera.GetRgbBufferSize());
		m_WndGraphCamera.SetDIBColorDepth(Camera.Settings.getColorDepth());
		m_WndGraphCamera.UpdatePlot();
	}
}

void CCameraSMXDlg::OnBnClickedButtonSetupCamera()
{
	INT_PTR result = theApp.m_DlgSMXCameraSettings.DoModal();
	if (Camera.Initialized())
	{
		CRect rc;
		//GetClientRect(rc);
		GetWindowRect(rc);
		rc.right = rc.left + Camera.Settings.getViewPort().Width() + 50;
		rc.bottom = rc.top + Camera.Settings.getViewPort().Height() + 50;
		ResizeDialog(rc);

		m_WndGraphCamera.SetSize(Camera.Settings.getViewPort().Width(), Camera.Settings.getViewPort().Height());
		m_WndGraphCamera.SetPlotDataSize(Camera.Settings.getViewPort().Width(), Camera.Settings.getViewPort().Height());
		
	}
}


void CCameraSMXDlg::ResizeDialog(const CRect newRect)
{	
	// Resize Dialog 			
	SetWindowPos(&CWnd::wndTop, newRect.left, newRect.top, newRect.right, newRect.bottom,		
		SWP_SHOWWINDOW | // Displays the window.		
		SWP_NOCOPYBITS // Do NOT Save content
		);
}

void CCameraSMXDlg::OnBnClickedButtonClose()
{
	PostQuitMessage(0);
}
