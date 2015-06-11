// XTPSpinButtonCtrl.h interface for the CXTPSpinButtonCtrl class.
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
#if !defined(__XTPSPINBUTTONCTRL_H__)
#define __XTPSPINBUTTONCTRL_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CXTPEdit;
class CXTPWinThemeWrapper;

class _XTP_EXT_CLASS CXTPSpinButtonCtrl : public CSpinButtonCtrl
{
	DECLARE_DYNAMIC(CXTPSpinButtonCtrl)

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPSpinButtonCtrl object
	//-----------------------------------------------------------------------
	CXTPSpinButtonCtrl();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPSpinButtonCtrl object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	~CXTPSpinButtonCtrl();

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
	UINT HitTest(CPoint pt);
	void OnDraw(CDC* pDC);

	// -----------------------------------------------------------------
	// Summary:
	//     This member is called to update color, text and other visual elements
	//     of the control.
	// -----------------------------------------------------------------
	void RefreshMetrics();
	BOOL HasSharedBorder() const;

	CXTPEdit* GetEditBuddy() const;

	void DrawVisualStyle(CDC* pDC);
	void DrawOffice2007(CDC* pDC);
	void DrawFlat(CDC* pDC);
	void DrawFlatButton(CDC* pDC, UINT nButton);
	void DrawArrowGlyph2(CDC* pDC, CRect rcArrow, BOOL bHorz, BOOL bUpArrow, BOOL bEnabled);
	void DrawStandard(CDC* pDC);
	BOOL IsVertical() const;
	CRect GetButtonRect(int nButton);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called from OnInitDialog or OnInitialUpdate
	//     to initialize the control.
	//-----------------------------------------------------------------------
	void Init();

protected:

	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()
	BOOL PreCreateWindow(CREATESTRUCT& cs);
	void PreSubclassWindow();


	//{{AFX_MSG(CXTPSpinButtonCtrl)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnMouseLeave();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE


protected:
	CXTPWinThemeWrapper* m_themeSpin;
	BOOL m_bUseVisualStyle;
	XTPControlTheme m_nTheme;
	BOOL m_bPreSubclassInit;

	COLORREF m_crBackPushed;
	COLORREF m_crBackHilite;
	COLORREF m_crBorderHilite;
	COLORREF m_crBorder;
	COLORREF m_crBack;
	COLORREF m_crArrowEnabled;
	COLORREF m_crArrowDisabled;
	UINT m_nHotButton;
	UINT m_nPressedButton;
};


AFX_INLINE BOOL CXTPSpinButtonCtrl::GetUseVisualStyle() const {
	return m_bUseVisualStyle;
}

#endif // #if !defined(__XTPSPINBUTTONCTRL_H__)
