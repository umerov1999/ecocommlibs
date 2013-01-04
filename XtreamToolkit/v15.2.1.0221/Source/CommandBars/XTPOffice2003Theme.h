// XTPOffice2003Theme.h : interface for the CXTPOffice2003Theme class.
//
// This file is a part of the XTREME COMMANDBARS MFC class library.
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
#if !defined(__XTPOFFICE2003THEME_H__)
#define __XTPOFFICE2003THEME_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

//===========================================================================
// Summary:
//     The CXTPOffice2003Theme class is used to enable an Office 2003 style theme for Command Bars
// See Also: CXTPPaintManager::SetTheme
//===========================================================================
class _XTP_EXT_CLASS CXTPOffice2003Theme : public CXTPOfficeTheme
{
	DECLARE_DYNAMIC(CXTPOffice2003Theme)
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPOffice2003Theme object.
	//-----------------------------------------------------------------------
	CXTPOffice2003Theme();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPOffice2003Theme object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTPOffice2003Theme();

	//-----------------------------------------------------------------------
	// Summary:
	//     Loads bitmap specified by nIDResource and converts its colors.
	// Parameters:
	//     nIDResource - Specifies bitmap resource ID.
	// Returns:
	//     Handle to bitmap with loaded bitmap.
	//-----------------------------------------------------------------------
	static HBITMAP AFX_CDECL GetModernBitmap(UINT nIDResource);

	//-----------------------------------------------------------------------
	// Summary:
	//     Loads bitmap specified by nIDResource and adds it to ImageManager.
	// Parameters:
	//     nIDResource - Specifies bitmap resource ID.
	//     pCommands - Pointer to an array of IDs.
	//     nCount - Number of elements in the array pointed to by lpIDArray.
	//     szIcon - Size of the icons.
	// Returns:
	//     Returns TRUE if successful, FALSE otherwise
	// See also: GetModernBitmap, CXTPImageManager
	//-----------------------------------------------------------------------
	static BOOL AFX_CDECL LoadModernToolbarIcons(UINT nIDResource);
	static BOOL AFX_CDECL LoadModernToolbarIcons(UINT nIDResource, UINT* pCommands, int nCount, CSize szIcon = CSize(16, 15)); // <combine CXTPOffice2003Theme::LoadModernToolbarIcons@UINT>

	//-----------------------------------------------------------------------
	// Summary:
	//     This method creates a new clipping region that consists of the existing clipping region minus the corners rectangles.
	// Parameters:
	//     pDC - Pointer to a valid device context.
	//     rc - CRect object specifying size of area.
	//-----------------------------------------------------------------------
	void ExcludeCorners(CDC* pDC, CRect rc);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to refresh the visual metrics of the manager.
	//-----------------------------------------------------------------------
	virtual void RefreshMetrics();

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
	//     This method is called to fill a dockbar.
	// Parameters:
	//     pDC  - Pointer to a valid device context
	//     pBar - Points to a CXTPDockBar object
	//-----------------------------------------------------------------------
	virtual void FillDockBar(CDC* pDC, CXTPDockBar* pBar);

	//-----------------------------------------------------------------------
	// Parameters:   pDC - Pointer to a valid device context.
	//          pBar - Pointer to the message bar to draw.
	// Summary: Draws the message bar.
	//-----------------------------------------------------------------------
	virtual void FillMessageBar(CDC* pDC, CXTPMessageBar* pBar);

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
	//-----------------------------------------------------------------------
	virtual void DrawRectangle(CDC* pDC, CRect rc, BOOL bSelected, BOOL bPressed, BOOL bEnabled, BOOL bChecked, BOOL bPopuped, XTPBarType barType, XTPBarPosition barPosition);

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
	// See Also: XTPSpecialControl
	//-----------------------------------------------------------------------
	virtual CSize DrawSpecialControl(CDC* pDC, XTPSpecialControl controlType, CXTPControl* pButton, CXTPCommandBar* pBar, BOOL bDraw, LPVOID lpParam);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to retrieve the command bar's borders.
	// Parameters:
	//     pBar - Points to a CXTPCommandBar object that the borders need to get.
	// Returns:
	//     Borders of the command bar.
	//-----------------------------------------------------------------------
	virtual CRect GetCommandBarBorders(CXTPCommandBar* pBar);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw a tear-off gripper of Popup Bar.
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
	// Summary:
	//     This method is called to draw the status bar's gripper.
	// Parameters:
	//     pDC      - Pointer to a valid device context
	//     rcClient - Client rectangle of the status bar.
	//-----------------------------------------------------------------------
	virtual void DrawStatusBarGripper(CDC* pDC, CRect rcClient);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw frame of single cell of status bar.
	// Parameters:
	//     pDC - Points to a valid device context.
	//     rc - CRect object specifying size of area.
	//     pPane - The status bar pane need to draw
	//     bGripperPane - TRUE if pane is last cell of status bar
	//-----------------------------------------------------------------------
	virtual void DrawStatusBarPaneBorder(CDC* pDC, CRect rc, CXTPStatusBarPane* pPane, BOOL bGripperPane);

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
	//     Retrieves the base theme of the manager.
	// Returns:
	//     Returns the base theme.
	//-----------------------------------------------------------------------
	XTPPaintTheme BaseTheme() { return xtpThemeOffice2003; }

	//-----------------------------------------------------------------------
	// Summary:
	//     Generates expand bitmap used for CXTPPopupBar.
	//-----------------------------------------------------------------------
	void CreateGradientCircle();

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns TRUE if Floating Toolbar drawn with gradient background
	// Parameters:
	//     pBar - Toolbar to check
	// Returns:
	//     TRUE if Toolbar has gradient background
	//-----------------------------------------------------------------------
	virtual BOOL HasFloatingBarGradientEntry(CXTPCommandBar* pBar);

protected:
	//{{AFX_CODEJOCK_PRIVATE
	void DrawExpandSymbols(CDC* pDC, BOOL bVertical, CRect rc, BOOL bHiddenExists, COLORREF clr);
	void RectangleEx(CDC* pDC, CRect rc, int nColorBorder, BOOL bHoriz, CXTPPaintManagerColorGradient& color);
	//}}AFX_CODEJOCK_PRIVATE

public:
	CXTPPaintManagerColorGradient m_clrDockBar;             // Color of dockbar face
	CXTPPaintManagerColorGradient m_clrCommandBar;          // Color of commandbar face
	CXTPPaintManagerColorGradient m_clrToolbarExpand;       // Toolbar Expand button color.
	CXTPPaintManagerColorGradient m_clrMenuExpand;          // MenuBar Expand button color.
	CXTPPaintManagerColorGradient m_clrMenuExpandedGripper; // Color of popup bar gripper.
	CXTPPaintManagerColorGradient m_clrMenuGripper;         // Color of the popup menu gripper.
	CXTPPaintManagerColorGradient m_clrPopupControl;        // color of popup control

	CXTPPaintManagerColor m_clrToolbarShadow;    // Toolbar bottom shadow color
	CXTPPaintManagerColor m_clrStatusPane;       // Color of status bar cell..
	CXTPPaintManagerColor m_clrTearOffGripper;  // TearOff gripper color

	BOOL     m_bLunaTheme;                       // TRUE if luna theme is active.
	BOOL     m_bPlainSeparators;                 // TRUE to draw plain separators

	BOOL m_bFlatGripper;

	int m_nToolbarSeparatorMargin;

	CImageList m_ilGradientCircle;               // Gradient circle holder.
	CXTPPaintManagerColorGradient m_grcLunaChecked;         // Background color of checked button
	CXTPPaintManagerColorGradient m_grcLunaPushed;          // Background color of pushed button
	CXTPPaintManagerColorGradient m_grcLunaSelected;        // Background color of selected button

};

//===========================================================================
// Summary:
//     The CXTPWhidbeyTheme class is used to enable a Visual Studio 2005 style theme for Command Bars
// See Also: CXTPPaintManager::SetTheme
//===========================================================================
class _XTP_EXT_CLASS CXTPWhidbeyTheme : public CXTPOffice2003Theme
{
	DECLARE_DYNAMIC(CXTPWhidbeyTheme)
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPWhidbeyTheme object.
	//-----------------------------------------------------------------------
	CXTPWhidbeyTheme();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPWhidbeyTheme object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTPWhidbeyTheme();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to refresh the visual metrics of the manager.
	//-----------------------------------------------------------------------
	void RefreshMetrics();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the base theme of the manager.
	// Returns:
	//     Returns the base theme.
	//-----------------------------------------------------------------------
	XTPPaintTheme BaseTheme() { return xtpThemeWhidbey; }

};

//===========================================================================
// Summary:
//     The CXTPVisualStudio2008Theme class is used to enable a Visual Studio 2008 style theme for Command Bars
// See Also: CXTPPaintManager::SetTheme
//===========================================================================
class _XTP_EXT_CLASS CXTPVisualStudio2008Theme : public CXTPWhidbeyTheme
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPVisualStudio2008Theme object.
	//-----------------------------------------------------------------------
	CXTPVisualStudio2008Theme();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPVisualStudio2008Theme object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTPVisualStudio2008Theme();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to refresh the visual metrics of the manager.
	//-----------------------------------------------------------------------
	void RefreshMetrics();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to retrieve the command bar's borders.
	// Parameters:
	//     pBar - Points to a CXTPCommandBar object that the borders need to get.
	// Returns:
	//     Borders of the command bar.
	//-----------------------------------------------------------------------
	virtual CRect GetCommandBarBorders(CXTPCommandBar* pBar);

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
	virtual void DrawPopupBarGripper(CDC* pDC, int xPos, int yPos, int cx, int cy, BOOL bExpanded);

	//-----------------------------------------------------------------------
	// Summary: This method is called to fill the control's face.
	// Parameters:
	//          pDC         - Pointer to a valid device context
	//          rc          - Rectangle to draw.
	//          bSelected   - TRUE if the control is selected.
	//          bPressed    - TRUE if the control is pushed.
	//          bEnabled    - TRUE if the control is enabled.
	//          bChecked    - TRUE if the control is checked.
	//          bPopuped    - TRUE if the control is popuped.
	//          barType     - Parent's bar type.
	//          barPosition - Parent's bar position.
	//-----------------------------------------------------------------------
	virtual void DrawRectangle(CDC* pDC, CRect rc, BOOL bSelected, BOOL bPressed, BOOL bEnabled, BOOL bChecked, BOOL bPopuped, XTPBarType barType, XTPBarPosition barPosition);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method retrieves buttons text color.
	// Parameters:
	//     bSelected   - TRUE if the control is selected.
	//     bPressed    - TRUE if the control is pushed.
	//     bEnabled    - TRUE if the control is enabled.
	//     bChecked    - TRUE if the control is checked.
	//     bPopuped    - TRUE if the control is popuped.
	//     barType     - Parent's bar type
	//     barPosition - Parent's bar position.
	// Returns:
	//     Buttons text color.
	//-----------------------------------------------------------------------
	virtual COLORREF GetRectangleTextColor(BOOL bSelected, BOOL bPressed, BOOL bEnabled, BOOL bChecked, BOOL bPopuped, XTPBarType barType, XTPBarPosition barPosition);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to determine offset of popuped bar.
	// Parameters:
	//     rc        - Control's bounding rectangle.
	//     pControl  - Points to a CXTPControl object
	//     bVertical - TRUE if control docked vertically.
	//-----------------------------------------------------------------------
	virtual void AdjustExcludeRect(CRect& rc, CXTPControl* pControl, BOOL bVertical);

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
	virtual CSize DrawCommandBarSeparator(CDC* pDC, CXTPCommandBar* pBar, CXTPControl* pControl, BOOL bDraw);

public:

	//-----------------------------------------------------------------------
	// Summary: Draws pixel in specific position with specify Alpha.
	// Parameters:
	//          pDC - Pointer to device context.
	//          xPos - Position to draw.
	//          yPos - Position to draw.
	//          clr - Color of pixel to draw.
	//          nAlpha - Opacity of pixel.
	//-----------------------------------------------------------------------
	static void AFX_CDECL AlphaPixel(CDC* pDC, int xPos, int yPos, COLORREF clr, int nAlpha);
	static void AFX_CDECL AlphaBorder(CDC* pDC, CRect rc, COLORREF clrBorder);

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the base theme of the manager.
	// Returns:
	//     Returns the base theme.
	//-----------------------------------------------------------------------
	XTPPaintTheme BaseTheme() { return xtpThemeVisualStudio2008; }


public:
	CXTPPaintManagerColorGradient m_clrMenuSelection;           // Gradient used to draw the current menu seleciton.
	CXTPPaintManagerColor m_clrMenuSelectionBorder;         // Color used to draw the border of the current menu selection.
	CXTPPaintManagerColor m_clrMenuSelectionText;           // Color used to draw the text of the current menu selection.

};

//===========================================================================
// Summary:
//     The CXTPVisualStudio2008Theme class is used to enable a Visual Studio 2008 style theme for Command Bars
// See Also: CXTPPaintManager::SetTheme
//===========================================================================
class _XTP_EXT_CLASS CXTPVisualStudio2010Theme : public CXTPOffice2003Theme
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPVisualStudio2008Theme object.
	//-----------------------------------------------------------------------
	CXTPVisualStudio2010Theme();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPVisualStudio2008Theme object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTPVisualStudio2010Theme();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to refresh the visual metrics of the manager.
	//-----------------------------------------------------------------------
	void RefreshMetrics();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to retrieve the command bar's borders.
	// Parameters:
	//     pBar - Points to a CXTPCommandBar object that the borders need to get.
	// Returns:
	//     Borders of the command bar.
	//-----------------------------------------------------------------------
	virtual CRect GetCommandBarBorders(CXTPCommandBar* pBar);

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
	virtual void DrawPopupBarGripper(CDC* pDC, int xPos, int yPos, int cx, int cy, BOOL bExpanded);

	//-----------------------------------------------------------------------
	// Summary: This method is called to fill the control's face.
	// Parameters:
	//          pDC         - Pointer to a valid device context
	//          rc          - Rectangle to draw.
	//          bSelected   - TRUE if the control is selected.
	//          bPressed    - TRUE if the control is pushed.
	//          bEnabled    - TRUE if the control is enabled.
	//          bChecked    - TRUE if the control is checked.
	//          bPopuped    - TRUE if the control is popuped.
	//          barType     - Parent's bar type.
	//          barPosition - Parent's bar position.
	//-----------------------------------------------------------------------
	virtual void DrawRectangle(CDC* pDC, CRect rc, BOOL bSelected, BOOL bPressed, BOOL bEnabled, BOOL bChecked, BOOL bPopuped, XTPBarType barType, XTPBarPosition barPosition);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method retrieves buttons text color.
	// Parameters:
	//     bSelected   - TRUE if the control is selected.
	//     bPressed    - TRUE if the control is pushed.
	//     bEnabled    - TRUE if the control is enabled.
	//     bChecked    - TRUE if the control is checked.
	//     bPopuped    - TRUE if the control is popuped.
	//     barType     - Parent's bar type
	//     barPosition - Parent's bar position.
	// Returns:
	//     Buttons text color.
	//-----------------------------------------------------------------------
	virtual COLORREF GetRectangleTextColor(BOOL bSelected, BOOL bPressed, BOOL bEnabled, BOOL bChecked, BOOL bPopuped, XTPBarType barType, XTPBarPosition barPosition);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to determine offset of popuped bar.
	// Parameters:
	//     rc        - Control's bounding rectangle.
	//     pControl  - Points to a CXTPControl object
	//     bVertical - TRUE if control docked vertically.
	//-----------------------------------------------------------------------
	virtual void AdjustExcludeRect(CRect& rc, CXTPControl* pControl, BOOL bVertical);

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
	virtual CSize DrawCommandBarSeparator(CDC* pDC, CXTPCommandBar* pBar, CXTPControl* pControl, BOOL bDraw);

	virtual void FillCommandBarEntry(CDC* pDC, CXTPCommandBar* pBar);

	virtual void FillStatusBar(CDC* pDC, CXTPStatusBar* pBar);

	void DrawStatusBarGripper(CDC* pDC, CRect rcClient);
protected:

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the base theme of the manager.
	// Returns:
	//     Returns the base theme.
	//-----------------------------------------------------------------------
	XTPPaintTheme BaseTheme() { return xtpThemeVisualStudio2010; }


public:
	CXTPPaintManagerColor m_clrMenuSeparator;
};

#endif // #if !defined(__XTPOFFICE2003THEME_H__)
