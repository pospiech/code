#ifndef _MEMDC_H_
#define _MEMDC_H_

//////////////////////////////////////////////////
// CMemDC - memory DC
//
// Author: Keith Rule
// Email:  keithr@europa.com
// Copyright 1996-2002, Keith Rule
//
// You may freely use or modify this code provided this
// Copyright is included in all derived versions.
//
// History - 10/3/97 Fixed scrolling bug.
//                   Added print support. - KR
//
//           11/3/99 Fixed most common complaint. Added
//                   background color fill. - KR
//
//           11/3/99 Added support for mapping modes other than
//                   MM_TEXT as suggested by Lee Sang Hun. - KR
//
//           02/11/02 Added support for CScrollView as supplied
//                    by Gary Kirkham. - KR
//
// This class implements a memory Device Context which allows
// flicker free drawing.

// Changed by Matthias Pospiech (2007) with additional support of 
// streching a bitmap onto a dc.

class CMemDC : public CDC {
private:	
	CBitmap		m_bitmap;		// Offscreen bitmap
	CBitmap*	m_oldBitmap;	// bitmap originally found in CMemDC
	CDC*		m_pDC;			// Saves CDC passed in constructor
	CRect		m_rect_dest;	// Rectangle of drawing area.
	CRect		m_rect_source;  // Rectangle of source area.
	BOOL		m_bMemDC;		// TRUE if CDC really is a Memory DC.
public:
	
	CMemDC(CDC* pDC, const CRect* pSourceRect = NULL, const CRect* pDestRect = NULL) : CDC()
	{
		ASSERT(pDC != NULL); 

		// Some initialization
		m_pDC = pDC;
		m_oldBitmap = NULL;
		m_bMemDC = !pDC->IsPrinting();

		// Get the rectangle to draw
		if (pDestRect == NULL) {
			pDC->GetClipBox(&m_rect_dest);
		} else {
			m_rect_dest = *pDestRect;
		}
		// Get the rectangle to draw
		if (pSourceRect == NULL) {
			pDC->GetClipBox(&m_rect_source);
		} else {
			m_rect_source = *pSourceRect;
		}
		
		if (m_bMemDC) {
			// Create a Memory DC
			CreateCompatibleDC(pDC);
			pDC->LPtoDP(&m_rect_dest);

			m_bitmap.CreateCompatibleBitmap(pDC, m_rect_dest.Width(), m_rect_dest.Height());
			m_oldBitmap = SelectObject(&m_bitmap);

			//SetMapMode(pDC->GetMapMode());
			pDC->SetMapMode(MM_ANISOTROPIC);
			//SetWindowExt(pDC->GetWindowExt());
			pDC->SetWindowExt(m_rect_source.Width(), m_rect_source.Height());
			//SetViewportExt(pDC->GetViewportExt());
			pDC->SetViewportExt(m_rect_dest.Width(), m_rect_dest.Height()); 
			

			pDC->DPtoLP(&m_rect_dest);
			SetWindowOrg(m_rect_dest.left, m_rect_dest.top);
		} else {
			// Make a copy of the relevent parts of the current DC for printing
			m_bPrinting = pDC->m_bPrinting;
			m_hDC       = pDC->m_hDC;
			m_hAttribDC = pDC->m_hAttribDC;
		}

		// Fill background 
		FillSolidRect(m_rect_dest, pDC->GetBkColor());
	}
	
	~CMemDC()	
	{		
		if (m_bMemDC) {

			//m_pDC->SetMapMode(MM_TEXT);
			//ASSERT(m_pDC->GetMapMode() == MM_TEXT);

			m_pDC->BitBlt(
				m_rect_dest.left, // x-coord of destination upper-left corner
				m_rect_dest.top,  // y-coord of destination upper-left corner
				m_rect_dest.Width(), // width of destination rectangle
				m_rect_dest.Height(), // height of destination rectangle
				this,
				m_rect_dest.left, // x-coord of destination upper-left corner
				m_rect_dest.top,  // y-coord of destination upper-left corner
				SRCCOPY);

			//m_pDC->StretchBlt(
			//	m_rect_dest.left, // x-coord of destination upper-left corner
			//	m_rect_dest.top,  // y-coord of destination upper-left corner
			//	m_rect_dest.Width(), // width of destination rectangle
			//	m_rect_dest.Height(), // height of destination rectangle
			//	this, // handle to source DC
			//	0, // x-coord of source upper-left corner
			//	0, // y-coord of source upper-left corner
			//	m_rect_source.Width(), // width of source rectangle
			//	m_rect_source.Height(),  // height of source rectangle
			//	SRCCOPY);			


			//Swap back the original bitmap.
			SelectObject(m_oldBitmap);		
		} else {
			// All we need to do is replace the DC with an illegal value,
			// this keeps us from accidently deleting the handles associated with
			// the CDC that was passed to the constructor.			
			m_hDC = m_hAttribDC = NULL;
		}	
	}
	
	// Allow usage as a pointer	
	CMemDC* operator->() 
	{
		return this;
	}	

	// Allow usage as a pointer	
	operator CMemDC*() 
	{
		return this;
	}
};

#endif