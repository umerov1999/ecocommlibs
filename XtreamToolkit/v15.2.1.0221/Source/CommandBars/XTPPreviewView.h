// XTPPreviewView.h : header file
//
// This file is a part of the XTREME TOOLKIT PRO MFC class library.
// (c)1998-2012 Codejock Software, All Rights Reserved.
//
// THIS SOURCE FILE IS THE PROPERTY OF CODEJOCK SOFTWARE AND IS NOT TO BE
// RE-DISTRIBUTED BY ANY MEANS WHATSOEVER WITHOUT THE EXPRESSED WRITTEN
// CONSENT OF CODEJOCK SOFTWARE.
//
// THIS SOURCE CODE CAN ONLY BE USED UNDER THE TERMS AND CONDITIONS OUTLINED
// IN THE XTREME TOOLKIT PRO LICENSE AGREEMENT. CODEJOCK SOFTWARE GRANTS TO
// YOU (ONE SOFTWARE DEVELOPER) THE LIMITED RIGHT TO USE THIS SOFTWARE ON A
// SINGLE COMPUTER.
//
// CONTACT INFORMATION:
// support@codejock.com
// http://www.codejock.com
//
/////////////////////////////////////////////////////////////////////////////

//{{AFX_CODEJOCK_PRIVATE
#if !defined(__XTPPREVIEWVIEW_H__)
#define __XTPPREVIEWVIEW_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CXTPPreviewView view

#define XTP_ZOOM_WHOLEPAGE 0 // Fit the entire preview page on the screen.
#define XTP_ZOOM_PAGEWIDTH 1 // Fit the preview page to the width of the screen.


//===========================================================================
// Summary: CXTPPreviewView is CPreviewView derived class that represents Print Preview window with themed toolBar.
//===========================================================================
class _XTP_EXT_CLASS CXTPPreviewView : public CPreviewView
{
	DECLARE_DYNCREATE(CXTPPreviewView)

protected:

	//-------------------------------------------------------------------------
	// Summary:
	//      Constructs a CXTPPreviewView object.
	// Parameters:
	//      nPageCount - Number of pages to be displayed in print preview mode.
	//                   You must increase the page count to make sure there is
	//                   an entry available for each page that can be displayed.
	//-------------------------------------------------------------------------
	CXTPPreviewView(int nPageCount = 9);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPPreviewView object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTPPreviewView();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the zoom state of the preview window.
	// Parameters:
	//     nNewState - Zoom state to use.  Possible values (XPT_ZOOM_WHOLEPAGE, XPT_ZOOM_PAGEWIDTH)
	//                 Or you can specify an in integer that represents a percentage to zoom the page.
	//     nPage - Page number to zoom.
	//     point - Point on page to zoom.
	//-----------------------------------------------------------------------
	void SetZoomState(UINT nNewState, UINT nPage, CPoint point);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns the current zoom state of the preview window.
	// Returns:
	//     Current zoom state of the preview window.
	//-----------------------------------------------------------------------
	int GetZoomState() const;

//{{AFX_CODEJOCK_PRIVATE
private:
	CXTPControl* FindToolbarButton(UINT nCommandID);
	virtual void PositionPage(UINT nPage);
	void SetScaledSize(UINT nPage);
	void DoZoom(UINT nPage, CPoint point);
	CSize CalcPageDisplaySize() ;
	void SetupScrollbar() ;
	void ZoomIn(UINT nPage, CPoint point);

protected:

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

//}}AFX_CODEJOCK_PRIVATE

protected:

//{{AFX_CODEJOCK_PRIVATE

	//{{AFX_VIRTUAL(CXTPPreviewView)
	public:
	void RecalLayout();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTPPreviewView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC*);
	afx_msg void OnNumPageChange();
	afx_msg void OnZoomIn();
	afx_msg void OnZoomOut();
	afx_msg void OnUpdateNumPageChange(CCmdUI* pCmdUI);
	afx_msg void OnUpdateZoomIn(CCmdUI* pCmdUI);
	afx_msg void OnUpdateZoomOut(CCmdUI* pCmdUI);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnPreviewPages();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) ;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGotoPageSetFocus(NMHDR* pNMHDR, LRESULT* pRes);
	afx_msg void OnGotoPageKillFocus(NMHDR* pNMHDR, LRESULT* pRes);
	afx_msg void OnUpdateGotoPage(CCmdUI* pCmdUI);
	afx_msg void OnZoomCombo(NMHDR* pNMHDR, LRESULT* pRes);
	afx_msg void OnOnePage();
	afx_msg void OnTwoPage(NMHDR* pNMHDR, LRESULT* pRes);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

//}}AFX_CODEJOCK_PRIVATE

protected:
	PAGE_INFO* m_pPageInfo2;    // Embedded array for the default implementation - replaces MFC one of size 2!
	CXTPToolBar* m_pCommandBar; // Pointer to the "built-in" print preview toolbar.
	int m_cxPixelsPerInch;      // Width of pixels in preview mode.
	int m_cyPixelsPerInch;      // Height of pixels in preview mode.

	// these variables are used in the page layout algorithm

	int m_Across ;          // number across the page, >= 1
	int m_Down ;            // number down the page, >= 1
	CPoint m_PageOffset;    // amount to move the page from one position to the next

	COLORREF m_clrBackColor;
};

AFX_INLINE int CXTPPreviewView::GetZoomState() const {
	return m_nZoomState;
}

#endif // !defined(__XTPPREVIEWVIEW_H__)
