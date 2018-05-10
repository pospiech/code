#include "stdafx.h"
#include "GraphCtrl.h"


CGraphCtrl::CGraphCtrl(void)
{
	PixelNumber.x=0;
	PixelNumber.y=0;
	pDIBData = NULL;
	DIBDataEnabled = false;
	m_DibSize = 0;
}

CGraphCtrl::CGraphCtrl(const int x, const int y)
{
	SetPlotDataSize(x,y);
}

CGraphCtrl::~CGraphCtrl(void)
{
}

BEGIN_MESSAGE_MAP(CGraphCtrl, CStatic)
	//{{AFX_MSG_MAP(CGraphCtrl)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()	
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGraphCtrl message handlers

BOOL CGraphCtrl::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CStatic::OnNotify(wParam, lParam, pResult);
}


LRESULT CGraphCtrl::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CStatic::WindowProc(message, wParam, lParam);
}

BOOL CGraphCtrl::OnEraseBkgnd(CDC* pDC) 
{	
	return FALSE;	
}


void CGraphCtrl::OnPaint() 
{
	CPaintDC dc(this); // device context for painting	
	
	if (PixelNumber.x == 0)
		return;


	if (DIBDataEnabled)
	{

		//First step: Create BITMAP Header info:
		BITMAPINFOHEADER bitmapInfo;
		::ZeroMemory(&bitmapInfo,sizeof(BITMAPINFOHEADER));
		bitmapInfo.biSize = sizeof(BITMAPINFOHEADER);
		bitmapInfo.biWidth = PixelNumber.x;/*enter Width*/
		bitmapInfo.biHeight = -PixelNumber.y;/*enter Height*/
		bitmapInfo.biPlanes = 1;
		bitmapInfo.biBitCount = m_DibColorDepth;
		bitmapInfo.biCompression = BI_RGB;  //RGB		
		bitmapInfo.biSizeImage = m_DibSize;;

		SetDIBitsToDevice(
			dc, 
			0,	//dx
			0,	//dy
			PixelNumber.x, // width
			PixelNumber.y, // height
			0,
			0,
			0,
			PixelNumber.y,
			pDIBData,
			(LPBITMAPINFO)&bitmapInfo,
			DIB_RGB_COLORS);

		//DIBDataEnabled=false;
		return;
	} 
}


void CGraphCtrl::Resize(const long width, const long height)
{	
	SetWindowPos(NULL, 0, 0, width, height, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
}

void CGraphCtrl::FullResize(const CRect newRect)
{
    SetWindowPos(NULL, newRect.left, newRect.top, newRect.Width(), newRect.Height(), SWP_NOACTIVATE | SWP_NOZORDER);	
}


void CGraphCtrl::SetSize(const long width, const long height)
{
	Resize(width-1, height-1);
}
CRect CGraphCtrl::GetSize()
{
	CRect rc;
	GetClientRect(&rc);		
	return rc;
}

void CGraphCtrl::SetPlotDataSize(const int x, const int y)
{
	PixelNumber.x = x;
	PixelNumber.y = y;
}

void CGraphCtrl::SetDIB(char* DIBArray, const int sizevalue, const int buffersize)
{	
	SetDIBSize(sizevalue);
	if  ((PixelNumber.x>0) && (PixelNumber.y > 0) && (m_DibSize != 0))
	{
		DIBDataEnabled = true;
		if (pDIBData != NULL)
		{
			delete [] pDIBData;
		}
		pDIBData = new char[buffersize];
		memcpy(pDIBData, DIBArray, buffersize*sizeof(char));		
	}
}

void CGraphCtrl::SetDIBSize(const int value)
{
	m_DibSize = value;
}

void CGraphCtrl::SetDIBColorDepth(const int value)
{
	m_DibColorDepth = value;
}


void CGraphCtrl::UpdatePlot()
{		
	Invalidate();
	UpdateWindow();	
}
