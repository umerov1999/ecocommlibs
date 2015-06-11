// XTPCommandBarsNativeXPTheme.h : interface for the CXTPCommandBarsNativeXPTheme class.
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
#if !defined(__XTPCOMMANDBARSNATIVEXPTHEME_H__)
#define __XTPCOMMANDBARSNATIVEXPTHEME_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//===========================================================================
// Summary:
//     The CXTPCommandBarsNativeXPTheme class is used to enable a Windows XP style theme for Command Bars
// See Also: CXTPPaintManager::SetTheme
//===========================================================================
class _XTP_EXT_CLASS CXTPCommandBarsNativeXPTheme : public CXTPCommandBarsOffice2000Theme
{
	DECLARE_DYNAMIC(CXTPCommandBarsNativeXPTheme)
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPCommandBarsNativeXPTheme object.
	//-----------------------------------------------------------------------
	CXTPCommandBarsNativeXPTheme();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPCommandBarsNativeXPTheme object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	~CXTPCommandBarsNativeXPTheme();

protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to refresh the visual metrics of the manager.
	//-----------------------------------------------------------------------
	virtual void RefreshMetrics();

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
	//     This method is called to fill the control's face
	// Parameters:
	//     pDC         - Pointer to a valid device context
	//     pButton - Points to a CXTPControl object to draw.
	//-----------------------------------------------------------------------
	void DrawControlEntry(CDC* pDC, CXTPControl* pButton);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method returns control text color to draw
	// Parameters:
	//     pButton - Points to a CXTPControl object to draw.
	//-----------------------------------------------------------------------
	COLORREF GetControlTextColor(CXTPControl* pButton);

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
	//     This method is called to fill a dockbar.
	// Parameters:
	//     pDC  - Pointer to a valid device context
	//     pBar - Points to a CXTPDockBar object
	//-----------------------------------------------------------------------
	virtual void FillDockBar(CDC* pDC, CXTPDockBar* pBar);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to fill a dockbar.
	// Parameters:
	//     pDC - Points to a valid device context.
	//     pWnd   - Client area to fill.
	//     pFrame - Parent frame window.
	//-----------------------------------------------------------------------
	void FillDockBarRect(CDC* pDC, CWnd* pWnd, CWnd* pFrame);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to determine offset of popuped bar.
	// Parameters:
	//     rc       - Control's bounding rectangle.
	//     pControl - Points to a CXTPControl object
	//     bVertical - TRUE if control docked vertically.
	//-----------------------------------------------------------------------
	void AdjustExcludeRect(CRect& rc, CXTPControl* pControl, BOOL bVertical);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws split button frame
	// Parameters:
	//     pDC      - Points to a valid device context.
	//     pButton  - Pointer to split button to draw.
	//     rcButton - Bounding rectangle to draw
	//-----------------------------------------------------------------------
	void DrawSplitButtonFrame(CDC* pDC, CXTPControl* pButton, CRect rcButton);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw frame of edit control.
	// Parameters:
	//     pDC - Points to a valid device context.
	//     rc - CRect object specifying size of area.
	//     bEnabled  - TRUE if control is enabled.
	//     bSelected - TRUE if control is selected.
	//-----------------------------------------------------------------------
	virtual void DrawControlEditFrame(CDC* pDC, CRect rc, BOOL bEnabled, BOOL bSelected);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw button od combo box control.
	// Parameters:
	//     pDC - Points to a valid device context.
	//     rcBtn     - Button bounding rectangle.
	//     bEnabled  - TRUE if combo box is enabled
	//     bSelected - TRUE if combo box is selected
	//     bDropped  - TRUE TRUE if combo box is dropped.
	//-----------------------------------------------------------------------
	virtual void DrawControlComboBoxButton(CDC* pDC, CRect rcBtn, BOOL bEnabled, BOOL bSelected, BOOL bDropped);

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
	//     Retrieves the base theme of the manager.
	// Returns:
	//     Returns the base theme.
	//-----------------------------------------------------------------------
	virtual XTPPaintTheme BaseTheme() { return xtpThemeNativeWinXP; }

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if theme is enabled
	// Returns:
	//     TRUE if WinXP theme is enabled
	//-----------------------------------------------------------------------
	BOOL IsThemeEnabled() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method draws edit control spin buttons
	// Parameters:
	//     pDC - Pointer to device context
	//     pControlEdit - Edit control
	//-----------------------------------------------------------------------
	virtual void DrawControlEditSpin(CDC* pDC, CXTPControlEdit* pControlEdit);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the specified controls.
	// Parameters:
	//     pDC         - Pointer to a valid device context
	//     controlType - Special control enumerator.
	//     pButton     - Points to a CXTPControl object to draw.
	//     pBar        - Parent CXTPCommandBar object.
	//     bDraw       - TRUE to draw; FALSE to retrieve the size of the
	//                   control.
	//     lpParam     - Specified parameter.
	// Returns:
	//     Size of the control.
	//-----------------------------------------------------------------------
	virtual CSize DrawSpecialControl(CDC* pDC, XTPSpecialControl controlType, CXTPControl* pButton, CXTPCommandBar* pBar, BOOL bDraw, LPVOID lpParam);

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
	//     Call this member function to draw a check mark.
	// Parameters:
	//     pDC        - Pointer to a valid device context
	//     rcCheck    - Specifies the rectangle in logical units.
	//     clr        - Color to fill.
	//     bEnabled   - TRUE to enable, FALSE to disable the button.
	//-----------------------------------------------------------------------
	virtual void DrawPopupBarCheckMark(CDC* pDC, CRect rcCheck, BOOL bEnabled, COLORREF clr);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to draw a radio button.
	// Parameters:
	//     pDC        - Pointer to a valid device context
	//     rcCheck    - Specifies the rectangle in logical units.
	//     clr        - Color to fill.
	//     bEnabled   - TRUE to enable, FALSE to disable the button.
	//-----------------------------------------------------------------------
	virtual void DrawPopupBarRadioMark(CDC* pDC, CRect rcCheck, BOOL bEnabled, COLORREF clr);

	//-----------------------------------------------------------------------
	// Input:   pBar - Popup bar.
	// Summary: Calculates gripper of popup bar.
	// Returns: Width of gripper to draw.
	//-----------------------------------------------------------------------
	virtual int GetPopupBarGripperWidth(CXTPCommandBar* pBar);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to draw Menu MDI buttons
	// Parameters:
	//     pDC        - Pointer to a valid device context
	//     pButton    - Button to draw
	//-----------------------------------------------------------------------
	virtual void DrawControlMDIButton(CDC* pDC, CXTPControlButton* pButton);


protected:
	CXTPWinThemeWrapper* m_themeRebar;           // Rebar theme helper
	CXTPWinThemeWrapper* m_themeToolbar;         // Toolbar theme helper
	CXTPWinThemeWrapper* m_themeCombo;           // ComboBox theme helper
	CXTPWinThemeWrapper* m_themeWindow;          // Window theme helper
	CXTPWinThemeWrapper* m_themeSpin;            // Window theme helper
	CXTPWinThemeWrapper* m_themeMenu;            // Menu theme helper
	COLORREF            m_clrEdgeShadowColor;   // Color of toolbar bottom line
	COLORREF            m_clrEdgeHighLightColor;// Color of toolbar top line
	BOOL                m_bFlatMenus;           // TRUE if OS flat menu option enabled

	COLORREF            m_clrSelectedText;      // Selectext text color
	COLORREF            m_clrPressedText;       // Pressed text color.
};

#define CXTPNativeXPTheme CXTPCommandBarsNativeXPTheme // Old name

#endif // !defined(__XTPCOMMANDBARSNATIVEXPTHEME_H__)
