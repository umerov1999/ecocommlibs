// XTPComboBox.h interface for the CXTPComboBox class.
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
#if !defined(__XTPCOMBOBOX_H__)
#define __XTPCOMBOBOX_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CXTPComboBoxTheme;
class CXTPComboBoxEditCtrl;

class _XTP_EXT_CLASS CXTPComboBox : public CComboBox
{
	DECLARE_DYNAMIC(CXTPComboBox)

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPComboBox object
	//-----------------------------------------------------------------------
	CXTPComboBox();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPComboBox object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	~CXTPComboBox();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to switch the visual theme of the control.
	// Parameters:
	//     nTheme - New visual theme. Can be any of the values listed in the Remarks section.
	// Remarks:
	//     nStyle can be one of the following:
	//     * <b>xtpControlThemeStandard</b> Standard appearance style.
	//     * <b>xtpControlThemeFlat</b> Flat appearance style.
	//     * <b>xtpControlThemeUltraFlat</b> Ultra flat appearance style.
	//     * <b>xtpControlThemeOffice2000</b> Office 2000 appearance style.
	//     * <b>xtpControlThemeOfficeXP</b> Office XP appearance style.
	//     * <b>xtpControlThemeOffice2003</b> Office 2003 appearance style.
	//     * <b>xtpControlThemeOffice2007</b> Office 2007 appearance style.
	//     * <b>xtpControlThemeVisualStudio2012Light</b> VS 2012 Light style theme.
	//     * <b>xtpControlThemeVisualStudio2012Dark</b> VS 2012 Dark style theme.
	//-----------------------------------------------------------------------
	BOOL SetTheme(XTPControlTheme nTheme);

	virtual void SetTheme(CXTPComboBoxTheme *pTheme);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns current combo box theme or NULL if no theme is set.
	//-----------------------------------------------------------------------
	CXTPComboBoxTheme* GetTheme();
	const CXTPComboBoxTheme* GetTheme() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Redraws the button.
	// Parameters:
	//     bUpdateWindow - TRUE to notify the parent window.
	//-----------------------------------------------------------------------
	void RedrawComboBox(BOOL bUpdateWindow = FALSE);

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

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function enables or disables auto completion.
	// Parameters:
	//     bEnable - TRUE to enable auto completion, otherwise FALSE.
	//-----------------------------------------------------------------------
	void EnableAutoCompletion(BOOL bEnable = TRUE);

	// -----------------------------------------------------------------
	// Summary:
	//     This member is called to update color, text and other visual elements
	//     of the control.
	// -----------------------------------------------------------------
	void RefreshMetrics();

public:

	BOOL IsFocused() const;
	BOOL IsHighlighted() const;

	HBRUSH GetClientBrush(CDC* pDC);

	CRect GetComboBoxRect() const;

	void SetHighlighted(BOOL bHot);

protected:

	void RedrawFocusedFrame();
	BOOL NeedRedraw();

//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	BOOL PreCreateWindow(CREATESTRUCT& cs);
	void PreSubclassWindow();
	void Init();
	BOOL PreTranslateMessage(MSG* pMsg);

	//{{AFX_MSG(CXTPComboBox)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);//reflect message

	void OnMouseLeave();
	void OnMouseMove(UINT nFlags, CPoint point);
	void OnSysColorChange();

	BOOL OnEndSel();
	BOOL OnEditUpdate();
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE

protected:
	BOOL m_bPreSubclassInit;

	BOOL m_bUseVisualStyle;
	BOOL m_bFlatStyle;

	CXTPComboBoxTheme* m_pTheme;  // Pointer to the current theme object.

	BOOL m_bHighlighted;
	BOOL m_bFocused;

	BOOL m_bAutoComp;
	BOOL m_bDisableAC;

	CXTPComboBoxEditCtrl *m_pWndEdit;
	friend class CXTPComboBoxEditCtrl;
};

AFX_INLINE BOOL CXTPComboBox::GetUseVisualStyle() const {
	return m_bUseVisualStyle;
}

AFX_INLINE BOOL CXTPComboBox::IsFocused() const {
	return m_bFocused;
}

AFX_INLINE BOOL CXTPComboBox::IsHighlighted() const {
	return m_bHighlighted;
}


AFX_INLINE BOOL CXTPComboBox::GetFlatStyle() const{
	return m_bFlatStyle;
}
AFX_INLINE void CXTPComboBox::SetFlatStyle(BOOL bFlatStyle/* = TRUE*/) {
	m_bFlatStyle = bFlatStyle;
	if (m_hWnd) Invalidate(FALSE);
}
AFX_INLINE void CXTPComboBox::EnableAutoCompletion(BOOL bEnable/*=TRUE*/) {
	m_bAutoComp = bEnable;
}

#endif // #if !defined(__XTPCOMBOBOX_H__)
