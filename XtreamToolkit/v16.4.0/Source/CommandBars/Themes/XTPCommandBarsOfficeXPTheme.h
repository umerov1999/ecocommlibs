// XTPCommandBarsOfficeXPTheme.h : interface for the CXTPCommandBarsOfficeXPTheme class.
//
// This file is a part of the XTREME COMMANDBARS MFC class library.
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
#if !defined(__XTPCOMMANDBARSOFFICEXPTHEME_H__)
#define __XTPCOMMANDBARSOFFICEXPTHEME_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

//===========================================================================
// Summary:
//     The CXTPCommandBarsOfficeXPTheme class is used to enable an Office XP style theme for Command Bars
// See Also: CXTPPaintManager::SetTheme
//===========================================================================
class _XTP_EXT_CLASS CXTPCommandBarsOfficeXPTheme : public CXTPPaintManager
{
	DECLARE_DYNAMIC(CXTPCommandBarsOfficeXPTheme)
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPCommandBarsOfficeXPTheme object.
	//-----------------------------------------------------------------------
	CXTPCommandBarsOfficeXPTheme();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to fill the command bar's face.
	// Parameters:
	//     pDC  - Pointer to a valid device context
	//     pBar - Points to a CXTPCommandBar object to draw.
	//-----------------------------------------------------------------------
	virtual void FillCommandBarEntry(CDC* pDC, CXTPCommandBar* pBar);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method retrieves buttons text color
	// Parameters:
	//     bSelected   - TRUE if the control is selected.
	//     bPressed    - TRUE if the control is pushed.
	//     bEnabled    - TRUE if the control is enabled.
	//     bChecked    - TRUE if the control is checked.
	//     bPopuped    - TRUE if the control is popuped.
	//     barType     - Parent's bar type
	//     barPosition - Parent's bar position.
	//-----------------------------------------------------------------------
	virtual COLORREF GetRectangleTextColor(BOOL bSelected, BOOL bPressed, BOOL bEnabled, BOOL bChecked, BOOL bPopuped, XTPBarType barType, XTPBarPosition barPosition);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the command bar's gripper.
	// Parameters:
	//     pDC   - Pointer to a valid device context
	//     pBar  - Points to a CXTPCommandBar object
	//     bDraw - TRUE to draw; FALSE to retrieve the size of the gripper.
	// Returns:
	//     Size of the gripper.
	//-----------------------------------------------------------------------
	virtual CSize DrawCommandBarGripper(CDC* pDC, CXTPCommandBar* pBar, BOOL bDraw = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw a command bar's separator.
	// Parameters:
	//     pDC      - Pointer to a valid device context
	//     pBar     - Points to a CXTPCommandBar object
	//     pControl - Points to a CXTPControl object
	//     bDraw    - TRUE to draw; FALSE to retrieve the size of the separator.
	// Returns:
	//     This method is called to draw a command bar's separator.
	//-----------------------------------------------------------------------
	virtual CSize DrawCommandBarSeparator(CDC* pDC, CXTPCommandBar* pBar, CXTPControl* pControl, BOOL bDraw = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the control placed on the ListBox.
	// Parameters:
	//     pDC       - Pointer to a valid device context
	//     pButton   - Points to a CXTPControl object
	//     rc        - Bounding rectangle to draw.
	//     bSelected - TRUE if the control is selected.
	//     bDraw     - TRUE to draw; FALSE to retrieve the size of the control.
	//     pCommandBars - CommandBars object which metrics need to use.
	// Returns:
	//     Size of the control.
	//-----------------------------------------------------------------------
	virtual CSize DrawListBoxControl(CDC* pDC, CXTPControl* pButton, CRect rc, BOOL bSelected, BOOL bDraw, CXTPCommandBars* pCommandBars = 0);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to determine offset of popuped bar.
	// Parameters:
	//     rc       - Control's bounding rectangle.
	//     pControl - Points to a CXTPControl object
	//     bVertical - TRUE if control docked vertically.
	//-----------------------------------------------------------------------
	virtual void AdjustExcludeRect(CRect& rc, CXTPControl* pControl, BOOL bVertical);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw a tear-off gripper of a Popup Bar.
	// Parameters:
	//     pDC       - Pointer to a valid device context
	//     rcGripper - Bounding rectangle of the gripper.
	//     bSelected - True if the gripper is selected.
	//     bDraw     - TRUE to draw; FALSE to retrieve the size of the gripper.
	// Returns:
	//     Size of the gripper.
	//-----------------------------------------------------------------------
	virtual CSize DrawTearOffGripper(CDC* pDC, CRect rcGripper, BOOL bSelected, BOOL bDraw);

	//-----------------------------------------------------------------------
	// Summary: This method is called to draw a resize gripper of a Popup Bar.
	// Input:   pDC       - Pointer to a valid device context
	//          rcGripper - Bounding rectangle of the gripper.
	//          nFlags    - Position of resize gripper
	//-----------------------------------------------------------------------
	virtual void DrawPopupResizeGripper(CDC* pDC, CRect rcGripper, int nFlags);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to draw gripper of dialog bar.
	// Parameters:
	//     pDC   - Points to a valid device context.
	//     pBar  - Dialog Bar pointer
	//     bDraw - TRUE to draw gripper, FALSE to calculate size.
	// Returns:
	//     Size of gripper to be drawn.
	//-----------------------------------------------------------------------
	virtual CSize DrawDialogBarGripper(CDC* pDC, CXTPDialogBar* pBar, BOOL bDraw);

protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw popup bar right gripper.
	// Parameters:
	//     pDC       - Points to a valid device context.
	//     xPos         - Specifies the logical x coordinate of the upper-left corner of the rectangle.
	//     yPos         - Specifies the logical y coordinate of the upper-left corner of the destination rectangle.
	//     cx        - Specifies the width of the rectangle.
	//     cy        - Specifies the height of the rectangle.
	//     bExpanded - TRUE if expanded.gripper.
	//-----------------------------------------------------------------------
	virtual void DrawPopupBarGripper(CDC* pDC, int xPos, int yPos, int cx, int cy, BOOL bExpanded = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the control placed on the PopupBar.
	// Parameters:
	//     pDC     - Pointer to a valid device context
	//     pButton - Points to a CXTPControl object
	//     bDraw   - TRUE to draw; FALSE to retrieve the size of the control.
	// Returns:
	//     Size of the control.
	//-----------------------------------------------------------------------
	virtual CSize DrawControlPopupParent(CDC* pDC, CXTPControl* pButton, BOOL bDraw);


	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to fill the control's face
	// Parameters:
	//     pDC         - Pointer to a valid device context
	//     rc          - Rectangle to draw.
	//     bSelected   - TRUE if the control is selected.
	//     bPressed    - TRUE if the control is pushed.
	//     bEnabled    - TRUE if the control is enabled.
	//     bChecked    - TRUE if the control is checked.
	//     bPopuped    - TRUE if the control is popuped.
	//     barType     - Parent's bar type
	//     barPosition - Parent's bar position.
	//     nID         - Bar's control ID.
	//-----------------------------------------------------------------------
	virtual void DrawRectangle(CDC* pDC, CRect rc, BOOL bSelected, BOOL bPressed, BOOL bEnabled, BOOL bChecked, BOOL bPopuped, XTPBarType barType, XTPBarPosition barPosition);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the image of the control.
	// Parameters:
	//     pDC           - Pointer to a valid device context
	//     pt            - Position to draw.
	//     sz            - Size of the image.
	//     pImage        - Points to a CXTPImageManagerIcon object
	//     bSelected     - TRUE if the control is selected.
	//     bPressed      - TRUE if the control is pushed.
	//     bEnabled      - TRUE if the control is enabled.
	//     bChecked      - TRUE if the control is checked.
	//     bPopuped      - TRUE if the control is popuped.
	//     bToolBarImage - TRUE if it is a toolbar image.
	//-----------------------------------------------------------------------
	virtual void DrawImage(CDC* pDC, CPoint pt, CSize sz, CXTPImageManagerIcon* pImage, BOOL bSelected, BOOL bPressed, BOOL bEnabled = TRUE, BOOL bChecked = FALSE, BOOL bPopuped = FALSE, BOOL bToolBarImage = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the control.
	// Parameters:
	//     pDC    - Pointer to a valid device context
	//     bDraw  - TRUE to draw; FALSE to retrieve the size of the control.
	//     pComboBox - CXTPControlComboBox pointer need to draw.
	// Returns:
	//     Size of the control.
	//-----------------------------------------------------------------------
	virtual CSize DrawControlComboBox(CDC* pDC, CXTPControlComboBox* pComboBox, BOOL bDraw);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the control.
	// Parameters:
	//     pDC   - Pointer to a valid device context
	//     bDraw - TRUE to draw; FALSE to retrieve the size of the control.
	//     pEdit - Edit control to draw.
	// Returns:
	//     Size of the control.
	//-----------------------------------------------------------------------
	virtual CSize DrawControlEdit(CDC* pDC, CXTPControlEdit* pEdit, BOOL bDraw);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws intersect rectangle of popup bar and its control.
	// Parameters:
	//     pDC       - Points to a valid device context.
	//     pPopupBar - Popup bar pointer.
	//     clr       - COLORREF specifies RGB color value.
	//-----------------------------------------------------------------------
	void FillIntersectRect(CDC* pDC, CXTPPopupBar* pPopupBar, COLORREF clr);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the base theme of the manager.
	// Returns:
	//     Returns the base theme.
	//-----------------------------------------------------------------------
	virtual XTPPaintTheme BaseTheme() { return xtpThemeOfficeXP; }

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the frame for the SplitButton
	//     control
	// Parameters:
	//     pDC     - Pointer to a valid device context
	//     pButton - Points to a CXTPControl object
	//     rc      - Bounding rectangle to draw
	//-----------------------------------------------------------------------
	virtual void DrawSplitButtonFrame(CDC* pDC, CXTPControl* pButton, CRect rc);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw check box mark area
	// Parameters:
	//     pDC - Pointer to a valid device context
	//     rc - Bounding rectangle
	//     bDraw - TRUE to draw; FALSE to find size
	//     bSelected - TRUE if control is selected
	//     bPressed - TRUE if control is pressed
	//     bChecked - TRUE if control is checked
	//     bEnabled - TRUE if control is enabled
	// Returns: Size of check box mark
	//-----------------------------------------------------------------------
	virtual CSize DrawControlCheckBoxMark(CDC* pDC, CRect rc, BOOL bDraw, BOOL bSelected, BOOL bPressed, BOOL bChecked, BOOL bEnabled);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw radio button mark area
	// Parameters:
	//     pDC - Pointer to a valid device context
	//     rc - Bounding rectangle
	//     bDraw - TRUE to draw; FALSE to find size
	//     bSelected - TRUE if control is selected
	//     bPressed - TRUE if control is pressed
	//     bChecked - TRUE if control is checked
	//     bEnabled - TRUE if control is enabled
	// Returns: Size of radio button mark
	//-----------------------------------------------------------------------
	virtual CSize DrawControlRadioButtonMark(CDC* pDC, CRect rc, BOOL bDraw, BOOL bSelected, BOOL bPressed, BOOL bChecked, BOOL bEnabled);

	virtual void DrawCheckMark(CDC* pDC, CRect rcCheck, BOOL bIsRadio, BOOL bEnabled, BOOL bSelected);

protected:
	//{{AFX_CODEJOCK_PRIVATE
	virtual void DrawSplitButtonPopup(CDC* pDC, CXTPControl* pButton);
	virtual void DrawControlPopupGlyph(CDC* pDC, CXTPControl* pButton);
	virtual void FillPopupLabelEntry(CDC* pDC, CRect rc);
	//}}AFX_CODEJOCK_PRIVATE
};

#define CXTPOfficeTheme CXTPCommandBarsOfficeXPTheme

#endif // !defined(__XTPCOMMANDBARSOFFICEXPTHEME_H__)
