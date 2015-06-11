// XTPChartControl.h
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
#if !defined(__XTPCHARTCONTROL_H__)
#define __XTPCHARTCONTROL_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


const UINT WM_XTP_CHART_BASE = (WM_USER + 9700);

static const DWORD XTP_NC_CHARTMOUSEMOVE    = (WM_XTP_CHART_BASE + 1);



class CXTPChartContent;
class CXTPChartDeviceCommand;
class CXTPChartDrawThread;
class CXTPChartDrawThreadDeviceCommand;
class CXTPChartContentView;
class CXTPToolTipContext;


//===========================================================================
// Summary:
//     CXTPChartControl is CWnd derived class, represents main Window that holds Content and draw chart
// Remarks:
//===========================================================================
class _XTP_EXT_CLASS CXTPChartControl : public CWnd, public CXTPChartContainer
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartControl object.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartControl();

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPChartControl object, handles cleanup
	//-------------------------------------------------------------------------
	virtual ~CXTPChartControl();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the pointer to Content object
	// Returns:
	//     A pointer to CXTPChartContent object.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartContent* GetContent() const;

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method creates the Chart Control
	// Parameters:
	//     dwStyle - Style for the window.
	//     rect -  Specifies the size and position of the control.
	//     pParentWnd - Specifies the parent window of the control.
	//     nID - Specifies the window control ID.
	// Returns:
	//     TRUE if successful, otherwise returns FALSE.
	//-----------------------------------------------------------------------
	BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);

	void Draw(CDC *pDC, CRect rc);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function whenever a change in the content of the chart.
	//     This call causes a redraw.
	// Parameters:
	//     updateOptions - Options that indicated what kind of changes done.
	// Remarks:
	//-----------------------------------------------------------------------
	void OnChartChanged(XTPChartUpdateOptions updateOptions = xtpChartUpdateView);

	//-----------------------------------------------------------------------
	// Summary:
	//     Register the window class if it has not already been registered.
	// Parameters:
	//     hInstance - Instance of resource where control is located
	// Returns:
	//     TRUE if the window class was successfully registered.
	//-----------------------------------------------------------------------
	BOOL RegisterWindowClass(HINSTANCE hInstance = NULL);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to hit test element under cursor
	// Parameters:
	//     point - Position to hit test.
	// Returns:
	//     Pointer to CXTPChartElement element under position.
	//-----------------------------------------------------------------------
	CXTPChartElement* HitTest(CPoint point) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to draw chart to device context
	// Parameters:
	//     hDC - Device context to draw to
	//     szBounds - Bounding position for chart
	//-----------------------------------------------------------------------
	void PrintToDC(HDC hDC, CRect rcBounds);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to save chart to file as image
	// Parameters:
	//     lpszFilePath - File path to save
	//     szBounds - Bounding size for chart
	//-----------------------------------------------------------------------
	void SaveAsImage(LPCTSTR lpszFilePath, CSize szBounds);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set content of chart control
	// Parameters:
	//     pContent - Pointer to new content
	//-----------------------------------------------------------------------
	void SetContent(CXTPChartContent* pContent);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set for best reaction to Chart changes. After each chart Changes, ChartControl will call UpdateWindow to redraw it.
	// Parameters:
	//     bUpdateWindow - TRUE to call UpdateWindow after each changes.
	//-----------------------------------------------------------------------
	void SetUpdateWindow(BOOL bUpdateWindow);


	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get tooltip context pointer.
	//-----------------------------------------------------------------------
	CXTPToolTipContext* GetToolTipContext() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to enable Chart tooltips
	//-----------------------------------------------------------------------
	void EnableToolTips(BOOL bEnable /* = TRUE */);


public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set capture to View.
	// Parameters:
	//     pView - View that will receive all mouse messages.
	//-----------------------------------------------------------------------
	void SetCapture(CXTPChartElementView* pView);

protected:
	//{{AFX_VIRTUAL(CXTPReportControl)
	virtual void PreSubclassWindow();
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	virtual INT_PTR OnToolHitTest(CPoint point, TOOLINFO* pTI) const;
	//}}AFX_VIRTUAL

	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP();
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnCaptureChanged(CWnd* pWnd);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
//}}AFX_CODEJOCK_PRIVATE

protected:
	CXTPChartContent* m_pContent;       // Chart Content

	CBitmap m_bmpCache;                 // Bitmap Cache
	CXTPChartDrawThreadDeviceCommand* m_pCommand;  // Root of Chart Command tree
	CXTPChartContentView* m_pContentView;   // Current Content View
	DWORD m_dwUpdateOptions;                // Update options
	CXTPToolTipContext* m_pToolTipContext;          // Tooltip Context.


	CXTPChartDrawThread* m_pDrawThread; // Helper draw thread that render 3D Charts
	CXTPChartElementView* m_pCaptureView;   // Captured View.
	BOOL m_bUpdateWindow;   // UpdateWindow flag.
};

AFX_INLINE CXTPChartContent* CXTPChartControl::GetContent() const {
	return m_pContent;
}
AFX_INLINE void CXTPChartControl::SetUpdateWindow(BOOL bUpdateWindow) {
	m_bUpdateWindow = bUpdateWindow;
}
AFX_INLINE CXTPToolTipContext* CXTPChartControl::GetToolTipContext() const {
	return m_pToolTipContext;
}


#endif //#if !defined(__XTPCHARTCONTROL_H__)
