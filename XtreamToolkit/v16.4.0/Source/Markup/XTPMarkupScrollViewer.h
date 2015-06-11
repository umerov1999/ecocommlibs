// XTPMarkupScrollViewer.h: interface for the CXTPMarkupScrollViewer class.
//
// This file is a part of the XTREME TOOLKIT PRO MFC class library.
// (c)1998-2013 Codejock Software, All Rights Reserved.
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


//{{AFX_CODEJOCK_PRIVATE
class CXTPMarkupBrush;
class CXTPMarkupBuilder;
class CXTPMarkupScrollBarGripper;
struct XTP_SCROLLBAR_POSINFO;
struct XTP_SCROLLBAR_TRACKINFO;
//}}AFX_CODEJOCK_PRIVATE


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
	CXTPMarkupScrollBar();

public:
	void SetOrientation(XTPMarkupOrientation orientation);
	void SetScrollInfo(SCROLLINFO* pScrollInfo);
	void GetScrollInfo(SCROLLINFO* pScrollInfo);

protected:
	CSize MeasureOverride(CXTPMarkupDrawingContext* pDC, CSize szAvailableSize);
	CSize ArrangeOverride(CSize szFinalSize);
	void OnRender(CXTPMarkupDrawingContext* pDC);

	void CalcScrollBarInfo(LPRECT lprc, XTP_SCROLLBAR_POSINFO *pSBInfo, SCROLLINFO* pSI);

	void OnMouseLeave(CXTPMarkupMouseEventArgs* e);
	void OnMouseEnter(CXTPMarkupMouseEventArgs* e);
	void OnMouseMove(CXTPMarkupMouseEventArgs* e);

	void OnMouseLeftButtonUp(CXTPMarkupMouseButtonEventArgs* e);
	void OnMouseLeftButtonDown(CXTPMarkupMouseButtonEventArgs* e);

	BOOL IsEnabled() const;

private:
	void PerformTrackInit(HWND hWnd, CPoint point, XTP_SCROLLBAR_POSINFO *pSBInfo, BOOL bDirect);
	void CalcTrackDragRect(XTP_SCROLLBAR_TRACKINFO *pSBTrack) const;

	void DoScroll(int cmd, int pos);
	int HitTestScrollBar(POINT pt) const;
	void EndScroll(BOOL fCancel);
	void RedrawScrollBar();
	void MoveThumb(int px);
	void TrackThumb(UINT message, CPoint pt);
	void TrackBox(UINT message, CPoint point);
	void ContScroll();
	void ScreenToClient(HWND hWnd,  LPPOINT lpPoint);

protected:
	XTPMarkupOrientation m_orientation;

	XTP_SCROLLBAR_POSINFO    m_spi;                 // ScrollBar position.
	XTP_SCROLLBAR_TRACKINFO *m_pSBTrack;         // ScrollBar tracking
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

#ifdef _XTP_ACTIVEX
//{{AFX_CODEJOCK_PRIVATE
public:
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPMarkupScrollViewer);

	afx_msg long OleGetVerticalOffset();
	afx_msg long OleGetHorizontalOffset();
	afx_msg void OleSetVerticalOffset(int nOffset);
	afx_msg void OleSetHorizontalOffset(int nOffset);

//}}AFX_CODEJOCK_PRIVATE
#endif

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
