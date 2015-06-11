// XTPProgressCtrl.h interface for the CXTPProgressCtrl class.
//
// This file is a part of the XTREME CONTROLS MFC class library.
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
#if !defined(__XTPPROGRESSCTRL_H__)
#define __XTPPROGRESSCTRL_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef PBS_MARQUEE
#define PBS_MARQUEE             0x08
#endif

#ifndef PBM_SETMARQUEE
#define PBM_SETMARQUEE          (WM_USER+10)
#endif

class CXTPWinThemeWrapper;

//===========================================================================
// Summary:
//     The ProgressBar control shows the progress of a lengthy operation by
//     filling a rectangle with chunks from left to right.
// Remarks:
//      A ProgressBar control has a range and a current position.  The range
//      represents the entire duration of the operation.  The current position
//      represents the progress the application has made toward completing the
//      operation.  The Max and Min properties set the limits of the range.
//      The Value property specifies the current position within that range.
//      Because chunks are used to fill in the control, the amount filled in
//      only approximates the Value property's current setting.  Based on the
//      control's size, the Value property determines when to display the next chunk.
//===========================================================================
class _XTP_EXT_CLASS CXTPProgressCtrl : public CProgressCtrl
{
	DECLARE_DYNAMIC(CXTPProgressCtrl)

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPProgressCtrl object
	//-----------------------------------------------------------------------
	CXTPProgressCtrl();
	~CXTPProgressCtrl();


public:

	// -----------------------------------------------------------------
	// Summary:
	//    This member function sets the bar color for the progress bar.
	// Parameters:
	//    clrBarColor  - COLORREF value that specifies the new bar color.
	// Returns:
	//     COLORREF value used for the bar.
	// -----------------------------------------------------------------
	void SetBarColor(COLORREF clrBarColor);

	// -----------------------------------------------------------------
	// Summary:
	//     Turns marquee mode on or off for the current progress bar
	//     control.
	// Parameters:
	//     fMarqueeMode - TRUE to turn marquee mode on, or FALSE to turn
	//                    marquee mode off.
	//     nInterval    - Time in milliseconds between updates of the
	//                    marquee animation.
	// Example:
	//    The following code example starts and stops the marquee
	//    scrolling animation.
	// <code>
	// int piAlpha[5] = { 25, 50, 75, 100, 100 };
	// m_wndProgress.SetMarquee(TRUE, 50);
	// </code>
	// -----------------------------------------------------------------
	BOOL SetMarquee(BOOL fMarqueeMode, int nInterval);

	// -----------------------------------------------------------------
	// Summary:
	//    This member function sets the background color for the progress bar.
	// Parameters:
	//    clrNew  - COLORREF value that specifies the new background color.
	// Returns:
	//     COLORREF value used for the background.
	// -----------------------------------------------------------------
	COLORREF SetBkColor(COLORREF clrNew);

	// -----------------------------------------------------------------
	// Summary:
	//    This member function sets the text color for the progress bar.
	// Parameters:
	//    clrNew  - COLORREF value that specifies the new text color.
	// Returns:
	//     COLORREF value used for text.
	// -----------------------------------------------------------------
	COLORREF SetTextColor(COLORREF clrNew);


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
	//     Gets the flat style appearance of the button control.
	// Remarks:
	//     The control will appear flat until the mouse pointer moves over it,
	//     at which point it appears three-dimensional.
	// Returns:
	//     TRUE if the flat style is used, FALSE otherwise.
	//-----------------------------------------------------------------------
	BOOL GetFlatStyle() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the flat style appearance of the button control.
	// Parameters:
	//     bFlatStyle - TRUE to use the flat style, FALSE otherwise.
	// Remarks:
	//     The control will appear flat until the mouse pointer moves over it,
	//     at which point it appears three-dimensional.
	//-----------------------------------------------------------------------
	void SetFlatStyle(BOOL bFlatStyle = TRUE);

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

	// -----------------------------------------------------------------
	// Summary:
	//     This member is called to update color, text and other visual elements
	//     of the control.
	// -----------------------------------------------------------------
	void RefreshMetrics();
	void DrawNcBorders(CDC* pDC, CRect rc);

	//-----------------------------------------------------------------------
	// Summary:
	//     A helper for rendering the control appearance.
	// Parameters:
	//     pDC - Pointer to a valid device context
	//-----------------------------------------------------------------------
	void DoPaint(CDC* pDC);
	void PaintOffice2007(CDC* pDC);

	// -----------------------------------------------------------------
	// Summary:
	//    This member function gets the background color for the progress bar.
	// Returns:
	//     COLORREF value used for the background.
	// -----------------------------------------------------------------
	COLORREF GetBackColor();
	void DoStep(int nStep);

//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	BOOL PreCreateWindow(CREATESTRUCT& cs);
	void PreSubclassWindow();
	void Init();

	//{{AFX_MSG(CXTPEdit)

	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSysColorChange();

	LRESULT OnSetPos(WPARAM /*wParam*/, LPARAM /*lParam*/);
	LRESULT OnStepIt(WPARAM /*wParam*/, LPARAM /*lParam*/);
	void OnTimer(UINT_PTR nIDEvent);
	LRESULT OnStartMarquee(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE


protected:
	BOOL m_bPreSubclassInit;
	int m_nMarqueePos;

	BOOL m_bUseVisualStyle;
	int m_nMarqueeDelay;
	CXTPWinThemeWrapper* m_themeProgress;
	XTPControlTheme m_nTheme;

	CXTPPaintManagerColor m_clrBorderNormal;
	CXTPPaintManagerColor m_clrBarColor;
	CXTPPaintManagerColor m_clrTextColor;
	CXTPPaintManagerColor m_clrBackColor;
	BOOL m_bFlatStyle;
};
AFX_INLINE BOOL CXTPProgressCtrl::SetMarquee(BOOL fMarqueeMode, int nInterval) {
	ASSERT(::IsWindow(m_hWnd)); return (BOOL)::SendMessage(m_hWnd, PBM_SETMARQUEE, (WPARAM)fMarqueeMode, (LPARAM)nInterval);
}


AFX_INLINE BOOL CXTPProgressCtrl::GetUseVisualStyle() const {
	return m_bUseVisualStyle;
}
AFX_INLINE BOOL CXTPProgressCtrl::GetFlatStyle() const{
	return m_bFlatStyle;
}
AFX_INLINE void CXTPProgressCtrl::SetFlatStyle(BOOL bFlatStyle/* = TRUE*/) {
	m_bFlatStyle = bFlatStyle;
	if (m_hWnd) Invalidate(FALSE);
}

#endif // #if !defined(__XTPPROGRESSCTRL_H__)
