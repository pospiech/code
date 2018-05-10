#pragma once
#include "afxwin.h"
#include "memdc.h"

class CGraphCtrl : 	public CStatic
{
// Construction
public:
	CGraphCtrl(void);
	CGraphCtrl(const int x, const int y);
// Destruction
public:
	~CGraphCtrl(void);

// Attributes
public:

// Operations
public:

// Overrides
// ClassWizard generated virtual function overrides
//{{AFX_VIRTUAL(CGraphCtrl)
public:

protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	CPoint PixelNumber;
	int m_DibSize;
	int m_DibColorDepth;
	bool m_AnalyseData;
	// Size and Resize 
	void Resize(const long width, const long height);
	void FullResize(const CRect newRect);
	void SetSize(const long width, const long height);
	CRect GetSize();	
	CRect GetPlotDataSize();
	bool DIBDataEnabled;	
	void SetDIB(char* DIBArray, const int sizevalue, const int buffersize);
	void SetDIBSize(const int value);
	void SetDIBColorDepth(const int value);
	// Set Data
	char* pDIBData;
	void SetPlotDataSize(const int x, const int y);
	//
	// Plot
	void UpdatePlot();
	//void PlotToDC(CMemDC& memDC);
	//void PlotToDC(CPaintDC* pDC);
 //	void SetPixel(CDC* pDC, const int ix, const int iy, const int div, const COLORREF c);
	//int rescale(const double value, const double datalength, const double reallength);

	// Bitmap Operations
	//BOOL SaveBitmap(CString csFileName);
	//BOOL LoadBitmap(CString csFileName);
	//BOOL LoadBitmapData(CString csFileName);
	//CBitmapOp BitmapOperations;
	
protected:
	//{{AFX_MSG(CGraphCtrl)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();	
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	//int ArrPos(const int x, const int y);
};

