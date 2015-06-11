// XTPColorDialog.h : header file
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
#if !defined(__XTPCOLORDIALOG_H__)
#define __XTPCOLORDIALOG_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//===========================================================================
// Summary:
//     Creates an eye dropper control that can be used to select any color
//     that is displayed anywhere on the screen. To use the eye dropper control,
//     click and hold the left mouse button and move it over the screen. The
//     color that is currently under the cursor will be selected as the new
//     color.
//===========================================================================
class _XTP_EXT_CLASS CXTPEyeDropper : public CWnd
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPEyeDropper object
	//-----------------------------------------------------------------------
	CXTPEyeDropper();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPEyeDropper object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTPEyeDropper();

	//{{AFX_VIRTUAL(CXTPEyeDropper)
protected:
	virtual void PreSubclassWindow();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to creates an eye dropper control.
	// Parameters:
	//     dwStyle    - Specifies the window style attributes.
	//     rect       - The size and position of the window, in client coordinates of pParentWnd.
	//     pParentWnd - The parent window.
	//     nID        - The ID of the child window.
	// Returns:
	//     TRUE if successful, otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to enable or disable setting the RGB color
	//     value as text on the Windows clipboard.
	// Parameters:
	//     bSetClipboard - TRUE to set the RGB color value on the clipboard.
	//-----------------------------------------------------------------------
	void SetClipboard(BOOL bSetClipboard);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to enable or disable the display for the
	//     color preview window. The color preview window is a rectangle that
	//     is displayed to the right of the eye dropper that displays the
	//     currently selected color.
	// Parameters:
	//     bShowPreview - TRUE to a preview window for selected color.
	//-----------------------------------------------------------------------
	void ShowPreview(BOOL bShowPreview);

	//{{AFX_CODEJOCK_PRIVATE
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	//}}AFX_CODEJOCK_PRIVATE

protected:
	int      m_nPadding;       // Size of the gap between eyedropper and preview display.
	CSize    m_icoSize;        // Size of the preview icon.
	BOOL     m_bPreInit;       // TRUE when initializing from PreSubclassWindow.
	BOOL     m_bSetClipboard;  // TRUE to set the selected RGB color on the clipboard.
	BOOL     m_bShowPreview;   // TRUE to show color preview.
	HICON    m_hIcon;          // Handle to the eye dropper icon.
	HCURSOR  m_hCurEyeDropper; // Handle to the eye dropper cursor.
	HCURSOR  m_hCurPrevious;   // Handle to the previous cursor.
	COLORREF m_color;          // Selected RGB color value.

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called by the CXTPEyeDropper class to
	//     perform initialization when the window is created or sub-classed.
	// Returns:
	//     TRUE if the window was successfully initialized, otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL Init();

	//-----------------------------------------------------------------------
	// Summary:
	//     Used by the eye dropper control to draw the eye dropper box.
	// Parameters:
	//     pDC - Points to a valid device context for the eye dropper window.
	//-----------------------------------------------------------------------
	virtual void DrawDropper(CDC* pDC);

	//-----------------------------------------------------------------------
	// Summary:
	//     Used by the eye dropper control to draw the color preview box.
	// Parameters:
	//     pDC - Points to a valid device context for the eye dropper window.
	//-----------------------------------------------------------------------
	virtual void DrawColor(CDC* pDC);

	//-----------------------------------------------------------------------
	// Summary:
	//     Used by the eye dropper control to place RGB color values as text
	//     on the Windows clipboard.
	// Parameters:
	//     lpszClipText - NULL terminated string representing the current
	//                    RGB color value.
	//-----------------------------------------------------------------------
	BOOL SetClipboardText(LPCTSTR lpszClipText);

	//-----------------------------------------------------------------------
	// Summary:
	//     Used by the eye dropper control to determine the rectangle for
	//     drawing the color preview box.
	// Returns:
	//     A CRect object indicating the size in client coordinates.
	//-----------------------------------------------------------------------
	CRect GetColorRect();

	//-----------------------------------------------------------------------
	// Summary:
	//     Used by the eye dropper control to determine the rectangle for
	//     drawing the eye dropper box.
	// Returns:
	//     A CRect object indicating the size in client coordinates.
	//-----------------------------------------------------------------------
	CRect GetDropperRect();

	//{{AFX_MSG(CXTPEyeDropper)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

//===========================================================================
// Summary:
//     CXTPColorDialog is a CPropertySheet derived class.
//     It is an Office(tm) style color dialog and can be used
//     as a drop in replacement for the MFC CColorDialog API.
//===========================================================================
class _XTP_EXT_CLASS CXTPColorDialog : public CPropertySheet
{
	DECLARE_DYNAMIC(CXTPColorDialog)

public:

	// ---------------------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPColorDialog object
	// Parameters:
	//     clrNew -      An RGB value that represents the new color selection.
	//     clrCurrent -  An RGB value that represents the default color
	//                   selection.
	//     dwFlags -     Style for color dialog. See the Remarks section for available styles.
	//     pWndParent -  A pointer to the dialog box's parent or owner window.
	// Remarks:
	//     Styles to be added or removed can be combined by using the bitwise
	//     OR (|) operator. It can be one or more of the following:
	//     * <b>CPS_XTP_SHOW3DSELECTION</b> Displays the color selection
	//       box with a 3D raised border.
	//     * <b>CPS_XTP_SHOWHEXVALUE</b> Displays the hex equivalent of the
	//       selected color.
	// See Also:
	//     CXTPColorPopup::Create, CXTPColorSelectorCtrl::Create,
	//     CXTPColorPicker::ModifyCPStyle
	// ---------------------------------------------------------------------------------
	CXTPColorDialog(COLORREF clrNew, COLORREF clrCurrent, DWORD dwFlags = 0L, CWnd* pWndParent = NULL);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPColorDialog object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTPColorDialog();

public:

	// -------------------------------------------------------------------
	// Summary:
	//     Sets the active color for the dialog.
	// Parameters:
	//     clr -      An RGB value that represents the color.
	//     bNotify -  TRUE to notify tab pages of a color change.
	//
	// Remarks:
	//     Call this member function to set the current color selection to
	//     the color value specified in <i>'clr'</i>. The dialog box will
	//     automatically update the user's selection based on the value of
	//     the <i>'clr'</i> parameter.
	// -------------------------------------------------------------------
	void SetNewColor(COLORREF clr, BOOL bNotify = TRUE);

	// ------------------------------------------------------------------
	// Summary:
	//     Call this function to set the new color selection to the color
	//     value specified in 'clr'.
	// Parameters:
	//     clr -  An RGB value that represents the color.
	// ------------------------------------------------------------------
	void SetCurrentColor(COLORREF clr);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to retrieve the information about
	//     the current color.
	// Returns:
	//     A COLORREF value that contains the RGB information for
	//     the current color specified when the dialog was instantiated.
	//-----------------------------------------------------------------------
	COLORREF GetCurrentColor();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to retrieve the information about
	//     the color that the user selected.
	// Returns:
	//     A COLORREF value that contains the RGB information
	//     for the new color selected in the color dialog box.
	//-----------------------------------------------------------------------
	COLORREF GetColor();

	// -------------------------------------------------------------
	// Summary:
	//     Call this member function to copy the string specified by
	//     'strText' to the Windows clipboard.
	// Parameters:
	//     strText -  A NULL terminated string.
	// Returns:
	//     TRUE if successful, otherwise returns FALSE.
	// -------------------------------------------------------------
	BOOL CopyToClipboard(const CString& strText);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function returns a CString object that represents the
	//     HEX conversion for the specified RGB color.
	// Parameters:
	//     clr - An RGB value that represents the color.
	// Returns:
	//     A CString object.
	//-----------------------------------------------------------------------
	static CString RGBtoHex(COLORREF clr);

	// ------------------------------------------------------------------------
	// Summary:
	//     Calculates the size of the dialog's tab control.
	// Remarks:
	//     This function is used to calculate the size of the dialog's tab
	//     control, the first tab item, the size of the OK button, and the size
	//     \of the cancel button. Then, resize the tab control to fit on the
	//     dialog. Also, reposition the OK and Cancel buttons. Next, reposition
	//     the Hex-Display. Finally, resize the property sheet.
	// ------------------------------------------------------------------------
	virtual void CalculateRects();

protected:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(CXTPColorDialog)
	virtual BOOL OnInitDialog();
	void AddPage(CPropertyPage* pPage);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTPColorDialog)
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnEyeDropperMoving(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEyeDropperSelect(WPARAM wParam, LPARAM lParam);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE

protected:

	DWORD         m_dwStyle;       // Styles for the dialog.
	CEdit         m_wndHexEdit;    // Edit window to display the color hex value.
	CXTPEyeDropper m_wndEyeDropper; // TODO
	CRect         m_rcNew;         // Size of the rectangle that represents the new color.
	CRect         m_rcCurrent;     // Size of the rectangle that represents the current color.
	COLORREF      m_clrNew;        // A COLORREF value that contains the RGB information for the new color.
	COLORREF      m_clrCurrent;    // A COLORREF value that contains the RGB information for the current color.

private:
	int m_nTextHeight;
	int m_nGapSize;
};

//////////////////////////////////////////////////////////////////////

AFX_INLINE COLORREF CXTPColorDialog::GetCurrentColor() {
	return m_clrCurrent;
}
AFX_INLINE COLORREF CXTPColorDialog::GetColor() {
	return m_clrNew;
}

AFX_INLINE void CXTPEyeDropper::SetClipboard(BOOL bSetClipboard) {
	m_bSetClipboard = bSetClipboard;
}
AFX_INLINE void CXTPEyeDropper::ShowPreview(BOOL bShowPreview) {
	m_bShowPreview = bShowPreview;
}

/////////////////////////////////////////////////////////////////////////////
// CXTPEyeDropper window

const DWORD CPS_XTP_SHOWHEXVALUE    = 0x0020;  //<ALIAS CXTPColorDialog::CXTPColorDialog@COLORREF@COLORREF@DWORD@CWnd*>
const DWORD CPS_XTP_SHOW3DSELECTION = 0x0040;  //<ALIAS CXTPColorDialog::CXTPColorDialog@COLORREF@COLORREF@DWORD@CWnd*>
const DWORD CPS_XTP_SHOWEYEDROPPER  = 0x0080;  //<ALIAS CXTPColorDialog::CXTPColorDialog@COLORREF@COLORREF@DWORD@CWnd*>

#endif // !defined(__XTPCOLORDIALOG_H__)
