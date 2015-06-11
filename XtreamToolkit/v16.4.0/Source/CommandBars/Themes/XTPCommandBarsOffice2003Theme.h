// XTPCommandBarsOffice2003Theme.h : interface for the CXTPCommandBarsOffice2003Theme class.
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
#if !defined(__XTPCOMMANDBARSOFFICE2003THEME_H__)
#define __XTPCOMMANDBARSOFFICE2003THEME_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

//===========================================================================
// Summary:
//     The CXTPCommandBarsOffice2003Theme class is used to enable an Office 2003 style theme for Command Bars
// See Also: CXTPPaintManager::SetTheme
//===========================================================================
class _XTP_EXT_CLASS CXTPCommandBarsOffice2003Theme : public CXTPCommandBarsOfficeXPTheme
{
	DECLARE_DYNAMIC(CXTPCommandBarsOffice2003Theme)
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPCommandBarsOffice2003Theme object.
	//-----------------------------------------------------------------------
	CXTPCommandBarsOffice2003Theme();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPCommandBarsOffice2003Theme object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTPCommandBarsOffice2003Theme();

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
	static BOOL AFX_CDECL LoadModernToolbarIcons(UINT nIDResource, UINT* pCommands, int nCount, CSize szIcon = CSize(16, 15)); // <combine CXTPCommandBarsOffice2003Theme::LoadModernToolbarIcons@UINT>

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
	//     nID         - Bar's control ID.
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
	virtual XTPPaintTheme BaseTheme() { return xtpThemeOffice2003; }

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

#define CXTPOffice2003Theme CXTPCommandBarsOffice2003Theme // Old name

#endif // #if !defined(__XTPCOMMANDBARSOFFICE2003THEME_H__)
