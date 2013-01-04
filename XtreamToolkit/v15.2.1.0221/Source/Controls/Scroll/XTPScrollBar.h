// XTPScrollBar.h interface for the CXTPScrollBar class.
//
// This file is a part of the XTREME CONTROLS MFC class library.
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
#if !defined(__XTPSCROLLBAR_H__)
#define __XTPSCROLLBAR_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CXTPWinThemeWrapper;

//===========================================================================
// Summary:
//     Scrollbar control.
// Remarks:
//     Scroll bars provide easy navigation through a long list of items or a
//     large amount of information.  They can also provide an analog representation
//     of current position.  You can use a scroll bar as an input device or
//     indicator of speed or quantity.for example, to control the volume of a
//     computer game or to view the time elapsed in a timed process.
//===========================================================================
class _XTP_EXT_CLASS CXTPScrollBar : public CScrollBar
{
	DECLARE_DYNAMIC(CXTPScrollBar)

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

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPScrollBar object
	//-----------------------------------------------------------------------
	CXTPScrollBar();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPScrollBar object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	~CXTPScrollBar();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to switch the visual theme of the control.
	// Parameters:
	//     nTheme - New visual theme. Can be any of the values listed in the Remarks section.
	// Remarks:
	//     nStyle can be one of the following:
	//     * <b>xtpControlThemeDefault</b> Standard appearance style.
	//     * <b>xtpControlThemeFlat</b> Flat appearance style.
	//     * <b>xtpControlThemeUltraFlat</b> Ultra flat appearance style.
	//     * <b>xtpControlThemeOffice2000</b> Office 2000 appearance style.
	//     * <b>xtpControlThemeOfficeXP</b> Office XP appearance style.
	//     * <b>xtpControlThemeOffice2003</b> Office 2003 appearance style.
	//     * <b>xtpControlThemeResource</b> Office 2007 appearance style.
	//-----------------------------------------------------------------------
	void SetTheme(XTPControlTheme nTheme);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the control is drawn using visual styles\SkinFramework, if supported.
	// Remarks:
	//     This method enables Windows XP visual styles for the control. This
	//     Control will draw with visual styles if the control and the operating
	//     system supports it.
	//     Visual styles and SkinFramwork skins are specifications for the
	//     appearance of controls.  GetUseVisualStyle determines whether to
	//     use the currently set Windows XP visual style.  If using SkinFramwork,
	//     it determines whether to use the currently set style of the SkinFramework.
	// Returns:
	//     TRUE if the currently set Windows XP visual style will be used to
	//     theme the control.  If using SkinFramwork, the currently set style
	//     of the SkinFramework will be used to skin the control.
	//
	//     FALSE if the currently set appearance\style in the OS will be used.
	//-----------------------------------------------------------------------
	BOOL GetUseVisualStyle() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Enables Windows XP visual styles for the control.
	// Remarks:
	//     This method enables Windows XP visual styles for the control. This
	//     Control will draw with visual styles if the control and the operating
	//     system supports it.
	//     Visual styles and SkinFramwork skins are specifications for the
	//     appearance of controls.  GetUseVisualStyle determines whether to
	//     use the currently set Windows XP visual style.  If using SkinFramwork,
	//     it determines whether to use the currently set style of the SkinFramework.
	// Parameters:
	//     bUseVisualStyle - TRUE if the currently set Windows XP visual style will be used to
	//                       theme the control.  If using SkinFramwork, the currently set style
	//                       of the SkinFramework will be used to skin the control.
	//
	//                       FALSE if the currently set appearance\style in the OS will be used.
	//-----------------------------------------------------------------------
	void SetUseVisualStyle(BOOL bUseVisualStyle = TRUE);


protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called from OnInitDialog or OnInitialUpdate
	//     to initialize the control.
	//-----------------------------------------------------------------------
	void Init();

	// -----------------------------------------------------------------
	// Summary:
	//     This member is called to update color, text and other visual elements
	//     of the control.
	// -----------------------------------------------------------------
	void RefreshMetrics();

protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get SCROLLBARTRACKINFO information
	// See Also: GetScrollBarPosInfo
	//-----------------------------------------------------------------------
	SCROLLBARTRACKINFO* GetScrollBarTrackInfo();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get SCROLLBARPOSINFO information
	// See Also: GetScrollBarTrackInfo
	//-----------------------------------------------------------------------
	SCROLLBARPOSINFO* GetScrollBarPosInfo();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to check ScrollBar part under pointer.
	// Parameters:
	//     pt - Mouse pointer to test
	//-----------------------------------------------------------------------
	int HitTestScrollBar(POINT pt);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to start scrollbar tracking
	// Parameters:
	//     hWnd - Parent Window handle
	//     point - Starting cursor position
	//     pSBInfo - Scrollbar information
	//     bDirect - TRUE to scroll to specified position
	//-----------------------------------------------------------------------
	void PerformTrackInit(HWND hWnd, CPoint point, SCROLLBARPOSINFO* pSBInfo, BOOL bDirect);

public:
	//{{AFX_CODEJOCK_PRIVATE
	void SetCommandBars(void*);
	void SetScrollBarStyle(int nTheme);
	//}}AFX_CODEJOCK_PRIVATE
protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw scrollbar.
	// Parameters:
	//     pDC - Pointer to device context
	//-----------------------------------------------------------------------
	void DrawScrollBar(CDC* pDC);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw an Office 2007 style scrollbar.
	// Parameters:
	//     pDC - Pointer to device context
	//-----------------------------------------------------------------------
	void DrawScrollBar2007(CDC* pDC);


protected:
//{{AFX_CODEJOCK_PRIVATE
	void EndScroll(BOOL fCancel);
	void MoveThumb(int px);
	void TrackThumb(UINT message, CPoint pt);
	void TrackBox(UINT message, CPoint point);
	void ContScroll();
	void SetupScrollInfo();
	virtual void CalcScrollBarInfo(LPRECT lprc, SCROLLBARPOSINFO* pSBInfo, SCROLLINFO* pSI);
	void CalcTrackDragRect(SCROLLBARTRACKINFO* pSBTrack) const;

	CRect GetScrollBarRect();
	void GetScrollInfo(SCROLLINFO* psi);
	BOOL IsScrollBarEnabled() const;
	CWnd* GetParentWindow() const;
	void DoScroll(int cmd, int pos);
	void RedrawScrollBar();
	int SBPosFromPx(SCROLLBARPOSINFO*  pSBInfo, int px);
//}}AFX_CODEJOCK_PRIVATE

protected:

//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	BOOL PreCreateWindow(CREATESTRUCT& cs);
	void PreSubclassWindow();


	//{{AFX_MSG(CXTPSpinButtonCtrl)
	afx_msg void OnMouseLeave();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnEnable(BOOL bEnable);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg LRESULT OnSetScrollInfo(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGetScrollInfo(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint pos);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSysColorChange();
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE

protected:
	int m_cxHScroll;            // Width, in pixels, of the arrow bitmap on a horizontal scroll bar
	int m_cyHScroll;            // Height, in pixels, of a horizontal scroll bar.

	int m_cyVScroll;            // Height, in pixels, of the arrow bitmap on a vertical scroll bar.
	int m_cxVScroll;            // Width, in pixels, of a vertical scroll bar;

protected:
	SCROLLBARPOSINFO m_spi;                 // ScrollBar position.
	SCROLLBARTRACKINFO* m_pSBTrack;         // ScrollBar tracking

protected:
	CXTPWinThemeWrapper* m_themeScrollBar; // Scrollbar theme.

protected:
	COLORREF m_crBackPushed;  // Color when pushed.
	COLORREF m_crBackHilite;  // Color when border.
	COLORREF m_crBorderHilite;  // Color of border when highlighted.
	COLORREF m_crBorder;  // Color of border.
	COLORREF m_crBack;  // Color of backgrount.


	BOOL m_bUseVisualStyle;  // True to use Visual Styles.
	XTPControlTheme m_nTheme; // Scrollbar theme.
	BOOL m_bPreSubclassInit;  // TRUE is subclassed.
};

AFX_INLINE BOOL CXTPScrollBar::GetUseVisualStyle() const {
	return m_bUseVisualStyle;
}
AFX_INLINE CXTPScrollBar::SCROLLBARTRACKINFO* CXTPScrollBar::GetScrollBarTrackInfo() {
	return m_pSBTrack;
}
AFX_INLINE CXTPScrollBar::SCROLLBARPOSINFO* CXTPScrollBar::GetScrollBarPosInfo() {
	return &m_spi;
}


#endif // #if !defined(__XTPSCROLLBAR_H__)
