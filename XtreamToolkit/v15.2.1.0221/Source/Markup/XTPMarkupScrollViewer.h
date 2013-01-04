// XTPMarkupScrollViewer.h: interface for the CXTPMarkupScrollViewer class.
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
#if !defined(__XTPMARKUPSCROLLVIEWER_H__)
#define __XTPMARKUPSCROLLVIEWER_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CXTPMarkupBrush;
class CXTPMarkupBuilder;
class CXTPMarkupScrollBarGripper;


enum XTPMarkupScrollBarVisibility
{
	xtpMarkupScrollBarDisabled,
	xtpMarkupScrollBarAuto,
	xtpMarkupScrollBarVisible
};


class _XTP_EXT_CLASS CXTPMarkupScrollBar : public CXTPMarkupControl
{
	DECLARE_MARKUPCLASS(CXTPMarkupScrollBar);

protected:
	//===========================================================================
	// Summary:
	//     The SCROLLBARPOSINFO structure contains scroll bar information.
	//===========================================================================
	struct SCROLLBARPOSINFO
	{
		int    posMin;  // Minimum position
		int    posMax;  // Maximum position
		int    page;    // Page size
		int    pos;     // Position of thumb

		int    pxTop;   // Top bounding rectangle
		int    pxBottom;// Bottom bounding rectangle
		int    pxLeft;  // Left bounding rectangle
		int    pxRight; // Right bounding rectangle
		int    cpxThumb;// Size of thumb button
		int    pxUpArrow;   // Position of Up arrow
		int    pxDownArrow; // Position of Down arrow
		int    pxStart;     // Previous position of thumb button
		int    pxThumbBottom;   // Thumb bottom bounding rectangle
		int    pxThumbTop;      // Thumb top bounding rectangle
		int    pxPopup; // In-place scroll position
		int    cpx;     // position in pixels
		int    pxMin;   // Minimum position in pixels

		RECT   rc;      // Bounding rectangle
		int    ht;      // Hit test
		BOOL   fVert;   // TRUE if vertical
	};

	//===========================================================================
	// Summary:
	//     The SCROLLBARTRACKINFO structure contains tracking information.
	//===========================================================================
	struct SCROLLBARTRACKINFO
	{
		BOOL   fHitOld;     // Previous hittest
		RECT   rcTrack;     // Bounding rectangle of available thumb position
		UINT   cmdSB;       // Scroll command
		UINT_PTR  hTimerSB; // Timer identifier
		int    dpxThumb;    // Delta of thumb button
		int    pxOld;       // Previous position in pixels
		int    posOld;      // Previous position
		int    posNew;      // new position
		HWND   hWndTrack;   // Parent tracking window

		BOOL   bTrackThumb; // TRUE if thumb is tracking
		SCROLLBARPOSINFO* pSBInfo;  // SCROLLBARPOSINFO pointer
	};


protected:
	CXTPMarkupScrollBar();

public:
	void SetOrientation(XTPMarkupOrientation orientation);
	void SetScrollInfo(SCROLLINFO* pScrollInfo);
	void GetScrollInfo(SCROLLINFO* pScrollInfo);

protected:
	CSize MeasureOverride(CXTPMarkupDrawingContext* pDC, CSize szAvailableSize);
	CSize ArrangeOverride(CSize szFinalSize);
	void OnRender(CXTPMarkupDrawingContext* pDC);

	void CalcScrollBarInfo(LPRECT lprc, SCROLLBARPOSINFO* pSBInfo, SCROLLINFO* pSI);

	void OnMouseLeave(CXTPMarkupMouseEventArgs* e);
	void OnMouseEnter(CXTPMarkupMouseEventArgs* e);
	void OnMouseMove(CXTPMarkupMouseEventArgs* e);

	void OnMouseLeftButtonUp(CXTPMarkupMouseButtonEventArgs* e);
	void OnMouseLeftButtonDown(CXTPMarkupMouseButtonEventArgs* e);

	BOOL IsEnabled() const;

private:
	void PerformTrackInit(HWND hWnd, CPoint point, SCROLLBARPOSINFO* pSBInfo, BOOL bDirect);
	void CalcTrackDragRect(SCROLLBARTRACKINFO* pSBTrack) const;

	void DoScroll(int cmd, int pos);
	int HitTestScrollBar(POINT pt) const;
	void EndScroll(BOOL fCancel);
	void RedrawScrollBar();
	void MoveThumb(int px);
	void TrackThumb(UINT message, CPoint pt);
	void TrackBox(UINT message, CPoint point);
	void ContScroll();
	int SBPosFromPx(CXTPMarkupScrollBar::SCROLLBARPOSINFO*  pSBInfo, int px);
	void ScreenToClient(HWND hWnd,  LPPOINT lpPoint);

protected:
	XTPMarkupOrientation m_orientation;

	SCROLLBARPOSINFO m_spi;                 // ScrollBar position.
	SCROLLBARTRACKINFO* m_pSBTrack;         // ScrollBar tracking
	SCROLLINFO m_si;
};

class _XTP_EXT_CLASS CXTPMarkupScrollViewer : public CXTPMarkupContentControl
{
	DECLARE_MARKUPCLASS(CXTPMarkupScrollViewer);
protected:
	CXTPMarkupScrollViewer();
	~CXTPMarkupScrollViewer();

public:
	int GetVisualChildrenCount() const;
	CXTPMarkupVisual* GetVisualChild(int nIndex) const;

	void Scroll(int nBar, int cmd, int pos);
	void SetScrollPos(int nBar, int pos);
	int GetScrollPos(int nBar);
	int GetScrollLimit(int nBar);

	XTPMarkupScrollBarVisibility GetVerticalScrollBarVisibility() const;
	XTPMarkupScrollBarVisibility GetHorizontalScrollBarVisibility() const;



protected:
	CSize ArrangeOverride(CSize szFinalSize);
	CSize MeasureOverride(CXTPMarkupDrawingContext* pDC, CSize szAvailableSize);

	void OnMouseWheel(CXTPMarkupMouseWheelEventArgs* e);

protected:
	void CreateScrollBar(int nBar);


protected:
	CXTPMarkupScrollBar* m_pVerticalScrollBar;
	CXTPMarkupScrollBar* m_pHorizontalScrollBar;
	CXTPMarkupScrollBarGripper* m_pScrollBarGripper;

	int m_xOffset;
	int m_yOffset;

public:
	static CXTPMarkupDependencyProperty* m_pVerticalScrollBarVisibilityProperty;
	static CXTPMarkupDependencyProperty* m_pHorizontalScrollBarVisibilityProperty;
};



AFX_INLINE void CXTPMarkupScrollBar::SetOrientation(XTPMarkupOrientation orientation) {
	m_orientation = orientation;
}

AFX_INLINE XTPMarkupScrollBarVisibility CXTPMarkupScrollViewer::GetVerticalScrollBarVisibility() const {
	CXTPMarkupEnum* pValue = MARKUP_STATICCAST(CXTPMarkupEnum, GetValue(m_pVerticalScrollBarVisibilityProperty));
	return pValue ? (XTPMarkupScrollBarVisibility)(int)(*pValue) : xtpMarkupScrollBarVisible;
}

AFX_INLINE XTPMarkupScrollBarVisibility CXTPMarkupScrollViewer::GetHorizontalScrollBarVisibility() const {
	CXTPMarkupEnum* pValue = MARKUP_STATICCAST(CXTPMarkupEnum, GetValue(m_pHorizontalScrollBarVisibilityProperty));
	return pValue ? (XTPMarkupScrollBarVisibility)(int)(*pValue) : xtpMarkupScrollBarDisabled;
}

#endif // !defined(__XTPMARKUPSCROLLVIEWER_H__)
