// XTPColorPicker.h : interface for the CXTPColorPicker class.
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
#if !defined(__XTPCOLORPICKER_H__)
#define __XTPCOLORPICKER_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

// class forwards
class CXTPColorPopup;
struct XTP_PICK_BUTTON;

//===========================================================================
// Summary:
//     CXTPColorPicker is a CXTPButton derived class. It displays a color picker
//     popup window (CXTPColorPopup) for Office(tm) style color selection.
//===========================================================================
class _XTP_EXT_CLASS CXTPColorPicker : public CXTPButton
{
	DECLARE_DYNAMIC(CXTPColorPicker)

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPColorPicker object
	//-----------------------------------------------------------------------
	CXTPColorPicker();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPColorPicker object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTPColorPicker();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will set the color value for the color
	//     picker.
	// Parameters:
	//     clr - An RGB value that represents the new RGB color for the
	//           control.
	//-----------------------------------------------------------------------
	virtual void SetColor(COLORREF clr);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will return the currently selected color.
	// Returns:
	//     An RGB color value that represents the currently selected color.
	//-----------------------------------------------------------------------
	virtual COLORREF GetColor() const;

	// -------------------------------------------------------------------
	// Summary:
	//     This member function will display any text associated with the
	//     color picker button control. If the button has text, it will be
	//     displayed.
	// Parameters:
	//     bShowText -  TRUE to display button text.
	// -------------------------------------------------------------------
	virtual void ShowText(BOOL bShowText);

	// --------------------------------------------------------------------
	// Summary:
	//     This member function will determine if text is displayed for the
	//     color picker button control.
	// Returns:
	//     TRUE if the color picker displays text, otherwise returns FALSE.
	// --------------------------------------------------------------------
	virtual BOOL IsTextShown() const;

	// ------------------------------------------------------------------
	// Summary:
	//     This member function will display any text associated with the
	//     color picker button control in color.
	// Parameters:
	//     bColorText -  TRUE to display the button text in color.
	// ------------------------------------------------------------------
	virtual void ShowTextInColor(BOOL bColorText);

	// -----------------------------------------------------------------
	// Summary:
	//     This member function will determine if the text for the color
	//     picker button control is displayed in color.
	// Returns:
	//     TRUE if the button text is in color, otherwise returns FALSE.
	// -----------------------------------------------------------------
	virtual BOOL IsTextInColor() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will set the default RGB color value for
	//     the color picker.
	// Parameters:
	//     clrDefault - An RGB value that represents the default color
	//     for the color picker.
	//-----------------------------------------------------------------------
	virtual void SetDefaultColor(COLORREF clrDefault);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will return the default RGB color value for
	//     the color picker.
	// Returns:
	//     An RGB value that represents the default color value for the
	//     color picker.
	//-----------------------------------------------------------------------
	virtual COLORREF GetDefaultColor() const;

	// ------------------------------------------------------------------------------
	// Summary:
	//     Call this member function to modify the color picker display
	//     style.
	// Parameters:
	//     dwRemove -  Specifies the window styles to be removed during style
	//                 modification.
	//     dwAdd -     Specifies the window styles to be added during style modification.
	// Remarks:
	//     Styles to be added or removed can be combined by using the bitwise
	//     OR (|) operator. It can be one or more of the following:
	//     * <b>CPS_XTP_NOFILL</b> The color picker will display a No Fill
	//           button rather than the default Automatic Color button.
	//     * <b>CPS_XTP_EXTENDED</b> The color picker will display 40
	//           extended colors rather than the default 16 colors.
	//     * <b>CPS_XTP_MORECOLORS</b> The color picker will display a
	//           More Colors button which will display a CXTPColorDialog.
	//     * <b>CPS_XTP_SHOW3DSELECTION</b> Displays the color selection
	//           box with a 3D raised border in CXTPColorDialog.
	//     * <b>CPS_XTP_SHOWHEXVALUE</b> Displays the hex equivalent of
	//           the selected color.
	// See Also:
	//     CXTPColorPopup::Create, CXTPColorSelectorCtrl::Create
	// ------------------------------------------------------------------------------
	virtual void ModifyCPStyle(DWORD dwRemove, DWORD dwAdd);

	// -------------------------------------------------------------------
	// Summary:
	//     This member function will activate the color picker and display
	//     the color popup window.
	// -------------------------------------------------------------------
	virtual void ShowPopupWindow();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set custom colors for color picker
	// Parameters:
	//     pColors - Static array of colors and tooltips
	//     nCount  - Number of colors
	//     nCols   - Number of columns to show
	//-----------------------------------------------------------------------
	void SetColors(XTP_PICK_BUTTON* pColors, int nCount, int nCols = 8);

protected:

	// ------------------------------------------------------------------------
	// Summary:
	//     Retrieves the background contrast color for the CXTPColorPicker.
	// Returns:
	//     An RGB value either BLACK or WHITE.
	// Remarks:
	//     This member function is called to return the contrast color for the
	//     color picker button. The contrast color is used when text is
	//     displayed to contrast with the currently selected color. For
	//     \example, if the currently selected color is WHITE, then BLACK would
	//     be returned so the text is legible.
	// ------------------------------------------------------------------------
	virtual COLORREF GetContrastColor() const;

protected:
	virtual void DoDraw(CDC* pDC);
	virtual void DrawColorPicker(CDC* pDC);

public:
	virtual BOOL IsPushed();

protected:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(CXTPColorPicker)
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTPColorPicker)
	afx_msg LRESULT OnSelChange(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDropDown(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCloseUp(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSelEndOK(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSelEndCancel(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSelNoFill(WPARAM wParam, LPARAM lParam);
	afx_msg void OnShowPopup();
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE

protected:
	BOOL     m_bShowText;    // TRUE if the color picker displays text.
	BOOL     m_bColorText;   // TRUE if the color picker text is colorized; only valid if m_bShowText is TRUE.
	BOOL     m_bPopupActive; // TRUE if the popup window is currently active.
	DWORD    m_dwPopup;      // Represents the style for the color picker.
	COLORREF m_clrSelected;  // An RGB value that represents the color for the display color.
	COLORREF m_clrDefault;   // An RGB value that represents the color for the default color.

	XTP_PICK_BUTTON*  m_pExtendedColors;         // Custom colors
	int               m_nExtendedColors;         // Custom colors count
	int               m_nCols;           // Number of columns in the color popup window.
	friend class CXTPColorPickerTheme;
};

//////////////////////////////////////////////////////////////////////

AFX_INLINE void CXTPColorPicker::SetColors(XTP_PICK_BUTTON* pColors, int nCount, int nCols) {
	m_pExtendedColors = pColors;
	m_nExtendedColors = nCount;
	m_nCols = nCols;
}
AFX_INLINE COLORREF CXTPColorPicker::GetColor() const {
	return m_clrSelected;
}
AFX_INLINE void CXTPColorPicker::ShowText(BOOL bShowText) {
	m_bShowText = bShowText; RedrawWindow();
}
AFX_INLINE BOOL CXTPColorPicker::IsTextShown() const {
	return m_bShowText;
}
AFX_INLINE void CXTPColorPicker::ShowTextInColor(BOOL bColorText) {
	m_bColorText = bColorText; RedrawWindow();
}
AFX_INLINE BOOL CXTPColorPicker::IsTextInColor() const {
	return m_bColorText;
}
AFX_INLINE void CXTPColorPicker::SetDefaultColor(COLORREF clrDefault) {
	m_clrDefault = clrDefault; RedrawWindow();
}
AFX_INLINE COLORREF CXTPColorPicker::GetDefaultColor() const {
	return m_clrDefault;
}
AFX_INLINE void CXTPColorPicker::ModifyCPStyle(DWORD dwRemove, DWORD dwAdd) {
	m_dwPopup &= ~dwRemove; m_dwPopup |= dwAdd; RedrawWindow();
}

// ------------------------------------------------------------------------
// Summary:
//     The DDX_XTPColorPicker function manages the transfer of
//     integer data between a color picker control in a dialog box, form
//     view, or control view object and a COLORREF data member of the
//     dialog box, form view, or control view object.
//
//     When DDX_XTPColorPicker is called, <i>'value'</i> is set to
//     the current state of the color picker control.
// Parameters:
//     pDX -    A pointer to a CDataExchange object. The framework supplies
//              this object to establish the context of the data exchange,
//              including its direction.
//     nIDC -   The resource ID of the color picker control associated with
//              the control property.
//     value -  A reference to a member variable of the dialog box, form view,
//              or control view object with which data is exchanged.
// See Also:
//     CXTPColorPicker
// ------------------------------------------------------------------------
_XTP_EXT_CLASS void AFXAPI DDX_XTPColorPicker(CDataExchange* pDX, int nIDC, COLORREF& value);

#endif // #if !defined(__XTPCOLORPICKER_H__)
