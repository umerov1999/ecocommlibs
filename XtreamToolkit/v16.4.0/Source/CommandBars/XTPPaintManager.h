// XTPPaintManager.h : interface for the CXTPPaintManager class.
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
#if !defined(__XTPPAINTMANAGER_H__)
#define __XTPPAINTMANAGER_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CXTPWinThemeWrapper;
class CXTPControlButton;
class CXTPControlPopup;
class CXTPControl;
class CXTPToolBar;
class CXTPCommandBar;
class CXTPCommandBars;
class CXTPPopupBar;
class CXTPControlComboBox;
class CXTPTabControl;
class CXTPDockBar;
class CXTPControlEdit;
class CXTPImageManagerIcon;
class CXTPDialogBar;

class CXTPStatusBar;
class CXTPStatusBarPane;
class CXTPStatusBarSwitchPane;
class CXTPStatusBarPaintManager;

class CXTPMessageBar;
class CXTPMessageBarButton;
class CXTPMessageBarPaintManager;

class CXTPCommandBarKeyboardTip;
class CXTPShadowManager;
class CXTPResourceImages;
class CXTPResourceImage;

// Paint manager
class CXTPRibbonPaintManager;
class CXTPControlGalleryPaintManager;
class CXTPSliderPaintManager;
class CXTPProgressPaintManager;
class CXTPFramePaintManager;
class CXTPScrollBarPaintManager;

//-----------------------------------------------------------------------
// Summary:
//     Special control with extended drawing.
// See Also: CXTPPaintManager::DrawSpecialControl
//
// <KEYWORDS xtpButtonExpandToolbar, xtpButtonExpandMenu, xtpButtonHideFloating, xtpButtonExpandFloating, xtpButtonCustomize>
//-----------------------------------------------------------------------
enum XTPSpecialControl
{
	xtpButtonExpandToolbar, // Control is option button of docking toolbar.
	xtpButtonExpandMenu,    // Control is expand button of popup bar.
	xtpButtonHideFloating,  // Control is hide button of floating toolbar.
	xtpButtonExpandFloating // Control is option button of floating toolbar.
};


//------------------------------------------------------------------------
// Summary:
//     This structure collect icons options for command bars.
// Example:
//     <code>XTPPaintManager()->GetIconsInfo()->bIconsWithShadow = FALSE;</code>
// See Also: CXTPPaintManager::GetIconsInfo
//------------------------------------------------------------------------
struct XTP_COMMANDBARS_ICONSINFO
{
	BOOL bUseFadedIcons;    // If TRUE, Icons will appear faded.
	BOOL bUseDisabledIcons; // If TRUE, disabled icons will be used when the control is disabled.
	BOOL bIconsWithShadow;  // If TRUE, when the mouse pointer is moved over a command bar control, the icons will appear to "jump" away from the screen casting a shadow.
	BOOL bOfficeStyleDisabledIcons;  // If TRUE, Office 2003 style disabled icons will be drawn when using disabled icons.
};

//===========================================================================
// Summary:
//     PaintManagers used CFont wrapper.
//===========================================================================
class _XTP_EXT_CLASS CXTPPaintManagerFont : public CFont
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPaintManagerFont object.
	//-----------------------------------------------------------------------
	CXTPPaintManagerFont()
	{
		m_bStandardFont = TRUE;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets application defined font
	// Parameters:
	//     lpLogFont - Application defined font
	//-----------------------------------------------------------------------
	BOOL SetCustomFont(const LOGFONT* lpLogFont)
	{
		m_bStandardFont = (lpLogFont == NULL);
		if (lpLogFont)
		{
			DeleteObject();
			return CreateFontIndirect(lpLogFont);
		}
		return TRUE;
	}


	//-----------------------------------------------------------------------
	// Summary:
	//      Call this method to set custom font
	// Parameters:
	//      bCustomFont - TRUE if custom font used
	// See Also: SetStandardFont
	//-----------------------------------------------------------------------
	void SetCustomFont(BOOL bCustomFont)
	{
		m_bStandardFont = !bCustomFont;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets paint manager's font
	// Parameters:
	//     lpLogFont - PaintManager's defined font
	//-----------------------------------------------------------------------
	BOOL SetStandardFont(const LOGFONT* lpLogFont)
	{
		if (lpLogFont && (m_bStandardFont || !GetSafeHandle()))
		{
			DeleteObject();
			return CreateFontIndirect(lpLogFont);
		}
		return TRUE;
	}

protected:
	BOOL m_bStandardFont; // TRUE to use standard font;
};


//{{AFX_CODEJOCK_PRIVATE
#define XTP_COMMANDBAR_MIN_FONT_HEIGHT    10
#define XTP_COMMANDBAR_MIN_CONTROL_HEIGHT (XTP_COMMANDBAR_MIN_FONT_HEIGHT + (XTP_COMMANDBAR_MIN_FONT_HEIGHT * 80) / 100)
#define XTP_COMMANDBAR_SPLITBUTTON_DROPDOWN_WIDTH (XTP_COMMANDBAR_MIN_FONT_HEIGHT + 1)
#define XTP_COMMANDBAR_TRIANGLE_SIZE      8

#define XTP_COMMANDBAR_TEXT_HEIGHT_FACTOR    110
#define XTP_COMMANDBAR_CONTROL_HEIGHT_FACTOR 190
//}}AFX_CODEJOCK_PRIVATE

//===========================================================================
// Summary:
//     Paint manager of the command bars.
//===========================================================================
class _XTP_EXT_CLASS CXTPPaintManager : public CXTPCmdTarget
{
	DECLARE_DYNAMIC(CXTPPaintManager)
private:
	friend class CXTPPaintManager* XTPPaintManager();
	class CPaintManagerDestructor;

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Specifies the resource that contains the bitmaps to be loaded for the
	//     theme.  This allows images to be loaded from something other than bitmap files.
	// Parameters:
	//     hResource - Handle to a visual style dll file handle. Or
	//                 some other resource that contains the images
	//                 to load for the theme.
	//     lpszIniFileName - String identifier of ini file with color specifications
	// Remarks:
	//     The images are the bitmaps that represent all the visual components
	//     of the Ribbon Bar.  For example tab buttons, group buttons, menu buttons,
	//     toolbar buttons, option button, toolbar dropdown, etc.
	//     The images are loaded using LoadImage and are stored in the m_pImages
	//     image collection.
	//     Images for the Office 2007 theme can be found in the \Source\Ribbon\Res
	//     folder.
	// Example:
	//     <code>XTPPaintManager()->SetImageHandle(XTPSkinManager()->GetResourceFile()->GetModuleHandle());</code>
	// See Also: LoadImage, m_pImages
	//-----------------------------------------------------------------------
	virtual void SetImageHandle(HMODULE hResource, LPCTSTR lpszIniFileName);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the control.
	// Parameters:
	//     pDC     - Pointer to a valid device context
	//     pButton - Points to a CXTPControl object
	//     bDraw   - TRUE to draw; FALSE to retrieve the size of the control.
	// Returns:
	//     Size of the control.
	//-----------------------------------------------------------------------
	CSize DrawControl(CDC* pDC, CXTPControl* pButton, BOOL bDraw = TRUE);

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
	//     This method is called to draw the control placed on the ToolBar.
	// Parameters:
	//     pDC     - Pointer to a valid device context
	//     pButton - Points to a CXTPControl object
	//     bDraw   - TRUE to draw; FALSE to retrieve the size of the control.
	// Returns:
	//     Size of the control.
	//-----------------------------------------------------------------------
	virtual CSize DrawControlToolBarParent(CDC* pDC, CXTPControl* pButton, BOOL bDraw);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the frame for the SplitButton
	//     control
	// Parameters:
	//     pDC     - Pointer to a valid device context
	//     pButton - Points to a CXTPControl object
	//     rc      - Bounding rectangle to draw
	//-----------------------------------------------------------------------
	virtual void DrawSplitButtonFrame(CDC* pDC, CXTPControl* pButton, CRect rc) = 0;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the control.
	// Parameters:
	//     pDC    - Pointer to a valid device context
	//     pCombo - ComboBox to draw.
	//     bDraw  - TRUE to draw; FALSE to retrieve the size of the control.
	// Returns:
	//     Size of the control.
	//-----------------------------------------------------------------------
	virtual CSize DrawControlComboBox(CDC* pDC, CXTPControlComboBox* pCombo, BOOL bDraw);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method returns the edit control back color to draw.
	// Parameters:
	//     pControl - Points to a CXTPControl object to get the back color from.
	// Returns: Edit control back color.
	//-----------------------------------------------------------------------
	virtual COLORREF GetControlEditBackColor(CXTPControl* pControl);

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
	virtual CSize DrawListBoxControl(CDC* pDC, CXTPControl* pButton, CRect rc, BOOL bSelected, BOOL bDraw, CXTPCommandBars* pCommandBars = 0) = 0;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to fill the command bar's face.
	// Parameters:
	//     pDC  - Pointer to a valid device context
	//     pBar - Points to a CXTPCommandBar object to draw.
	//-----------------------------------------------------------------------
	virtual void FillCommandBarEntry(CDC* pDC, CXTPCommandBar* pBar) = 0;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to fill the control's face
	// Parameters:
	//     pDC         - Pointer to a valid device context
	//     pButton - Points to a CXTPControl object to draw.
	//-----------------------------------------------------------------------
	virtual void DrawControlEntry(CDC* pDC, CXTPControl* pButton);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method returns control text color to draw
	// Parameters:
	//     pButton - Points to a CXTPControl object to draw.
	//-----------------------------------------------------------------------
	virtual COLORREF GetControlTextColor(CXTPControl* pButton);

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
	virtual COLORREF GetRectangleTextColor(BOOL bSelected, BOOL bPressed, BOOL bEnabled, BOOL bChecked, BOOL bPopuped, XTPBarType barType, XTPBarPosition barPosition) = 0;

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
	virtual void DrawRectangle(CDC* pDC, CRect rc, BOOL bSelected, BOOL bPressed, BOOL bEnabled, BOOL bChecked, BOOL bPopuped, XTPBarType barType, XTPBarPosition barPosition) = 0;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to fill the control's face
	// Parameters:
	//     pDC  - Pointer to a valid device context
	//     pBar - Points to a CXTPControl object to draw.
	// Returns:
	//     TRUE if the rectange was drawn, otherwise returns FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL DrawRectangle(CDC* pDC, CXTPControl* pButton);

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
	virtual CSize DrawCommandBarSeparator(CDC* pDC, CXTPCommandBar* pBar, CXTPControl* pControl, BOOL bDraw = TRUE) = 0;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw a command bar's group separator.
	// Parameters:
	//     pDC      - Pointer to a valid device context
	//     pBar     - Points to a CXTPCommandBar object
	//     pLeftControl - Points to a left CXTPControl object
	//     pRightControl - Points to a left CXTPControl object
	//     bDraw    - TRUE to draw; FALSE to retrieve the size of the separator.
	// Returns:
	//     This method is called to draw a command bar's separator.
	//-----------------------------------------------------------------------
	virtual void DrawCommandBarGroupSeparator(CDC* pDC, CXTPCommandBar* pBar, CXTPControl* pLeftControl, CXTPControl* pRightControl, BOOL bDraw = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to determine offset of popuped bar.
	// Parameters:
	//     rc        - Control's bounding rectangle.
	//     pControl  - Points to a CXTPControl object
	//     bVertical - TRUE if control docked vertically.
	//-----------------------------------------------------------------------
	virtual void AdjustExcludeRect(CRect& rc, CXTPControl* pControl, BOOL bVertical) = 0;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to fill a dockbar.
	// Parameters:
	//     pDC  - Pointer to a valid device context
	//     pBar - Points to a CXTPDockBar object.
	//-----------------------------------------------------------------------
	virtual void FillDockBar(CDC* pDC, CXTPDockBar* pBar);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws workspace space
	// Parameters:
	//     pDC - Pointer to device context
	//     rc - Bounding rectangle
	//     rcExclude - Excluded rectangle
	//-----------------------------------------------------------------------
	virtual void FillWorkspace(CDC *pDC, CRect rc, CRect rcExclude);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to refresh the visual metrics of the manager.
	//-----------------------------------------------------------------------
	virtual void RefreshMetrics();

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines the kind of shadow to use.
	// Returns:
	//     TRUE to use alpha and solid shadow; otherwise returns FALSE
	//-----------------------------------------------------------------------
	int GetShadowOptions() const;

	virtual void DrawExpandGlyph(CDC* pDC, CXTPControl* pButton, CPoint pt0, CPoint pt1, CPoint pt2, COLORREF clr);

	virtual void DrawHiddenGlyph(CDC* pDC, CXTPControl* pButton, COLORREF clr, BOOL bVertical);

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
	virtual CSize DrawTearOffGripper(CDC* pDC, CRect rcGripper, BOOL bSelected, BOOL bDraw) = 0;

	//-----------------------------------------------------------------------
	// Summary: This method is called to draw popup bar resize gripper.
	// Input:   pDC       - Pointer to a valid device context.
	//          rcGripper - Bounding rectangle of the gripper.
	//          nFlags    - Position of resize gripper
	//-----------------------------------------------------------------------
	virtual void DrawPopupResizeGripper(CDC* pDC, CRect rcGripper, int nFlags) = 0;

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
	virtual void DrawImage(CDC* pDC, CPoint pt, CSize sz, CXTPImageManagerIcon* pImage, BOOL bSelected, BOOL bPressed, BOOL bEnabled = TRUE, BOOL bChecked = FALSE, BOOL bPopuped = FALSE, BOOL bToolBarImage = TRUE) = 0;

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
	virtual CSize DrawDialogBarGripper(CDC* pDC, CXTPDialogBar* pBar, BOOL bDraw) = 0;

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines split button command rectangle
	// Parameters:
	//     pButton - Button to test
	// Returns:
	//     Bounding rectangle of command part of split button
	//-----------------------------------------------------------------------
	virtual CRect GetSplitButtonCommandRect(CXTPControl* pButton);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to set bounding region for popup bars
	// Parameters:
	//     pCommandBar - Pointer to popup bar which region need to set
	//-----------------------------------------------------------------------
	virtual void SetCommandBarRegion(CXTPCommandBar* pCommandBar);

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

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Creates new PaintManager pointer.
	// Parameters:
	//     paintTheme - Theme to be created.
	// Returns:
	//     Pointer to new CXTPPaintManager object.
	// Example:
	//     <code>pCommandBars->SetPaintManager(CXTPPaintManager::CreateTheme(xtpThemeOfficeXP);</code>
	//-----------------------------------------------------------------------
	static CXTPPaintManager* AFX_CDECL CreateTheme(XTPPaintTheme paintTheme);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to switch the visual theme of the Command Bars.
	// Parameters:
	//     paintTheme - Theme to be set. Can be any of the values listed in
	//                  the Remarks section.
	// Remarks:
	//     paintTheme can be one of following:
	//     * <b>xtpThemeOfficeXP</b> Enables Office XP theme.
	//     * <b>xtpThemeOffice2000</b> Enables Office 2000 theme.
	//     * <b>xtpThemeOffice2003</b> Enables Office 2003 theme.
	//     * <b>xtpThemeNativeWinXP</b> Enables Windows XP themes support.
	//     * <b>xtpThemeWhidbey</b> Enables Visual Studio 2005 theme.
	//-----------------------------------------------------------------------
	static void AFX_CDECL SetTheme(XTPPaintTheme paintTheme);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set a custom theme.
	// Parameters:
	//     pTheme - theme to be set.
	//-----------------------------------------------------------------------
	static void AFX_CDECL SetCustomTheme(CXTPPaintManager* pTheme);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the current visual theme.
	// Returns:
	//     The current theme.
	//-----------------------------------------------------------------------
	XTPPaintTheme GetCurrentTheme() { return m_themeCurrent;}

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to destroy a paint manager object.
	//-----------------------------------------------------------------------
	static void AFX_CDECL Done();

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the base theme of the manager.
	// Returns:
	//     Returns the base theme.
	//-----------------------------------------------------------------------
	virtual XTPPaintTheme BaseTheme() = 0;

	virtual BOOL RequiresResourceImages();

public:

	// ---------------------------------------------------------------------
	// Summary:
	//     Call this member to retrieve the font of the command bar control.
	// Parameters:
	//     pControl :  Points to a CXTPCommandBar object.
	// Returns:
	//     A pointer to a CFont object.
	// ---------------------------------------------------------------------
	virtual CFont* GetControlFont(CXTPControl* pControl);

	//-----------------------------------------------------------------------
	// Input:   pCommandBar - Points to a CXTPCommandBar object to determine
	//                        the font used to draw text.
	// Summary: Call this member to retrieve the font of the command bar.
	// Returns: A pointer to a CFont object.
	//-----------------------------------------------------------------------
	virtual CFont* GetCommandBarFont(CXTPCommandBar* pCommandBar);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine how to draw text
	// Parameters:
	//     pControl - Points to a CXTPControl object.
	//-----------------------------------------------------------------------
	virtual XTPBarTextOrientation GetControlTextOrientation(CXTPControl* pControl);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws a triangle.
	// Parameters:
	//     pDC - Pointer to a valid device context
	//     pt0 - Specifies the logical coordinates of the first point.
	//     pt1 - Specifies the logical coordinates of the second point.
	//     pt2 - Specifies the logical coordinates of the third point.
	//     clr - Color to fill.
	//-----------------------------------------------------------------------
	static void AFX_CDECL Triangle(CDC* pDC, CPoint pt0, CPoint pt1, CPoint pt2, COLORREF clr);

	//-----------------------------------------------------------------------
	// Summary:
	//     Refreshes the fonts to use.
	//-----------------------------------------------------------------------
	virtual void UpdateFonts();

	//-----------------------------------------------------------------------
	// Summary:
	//     Refreshes the standard colors to use.
	//-----------------------------------------------------------------------
	virtual void UpdateColors();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set command bars font.
	// Parameters:
	//     pLogFont         - New LOGFONT to use for text.
	//     bUseStandardFont - When calling SetCommandBarsFontIndirect, always
	//                        use FALSE, this will indicate that pLogFont
	//                        should be used as the new font.
	//-----------------------------------------------------------------------
	void SetCommandBarsFontIndirect(LOGFONT* pLogFont, BOOL bUseStandardFont = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns the general font.
	// Returns:
	//     A Pointer to a CFont object
	//-----------------------------------------------------------------------
	CFont* GetRegularFont();

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns the general bold font.
	// Returns:
	//     A Pointer to a CFont object
	//-----------------------------------------------------------------------
	CFont* GetRegularBoldFont();

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns the icon font.
	// Returns:
	//     A pointer to a CFont object
	//-----------------------------------------------------------------------
	CFont* GetIconFont();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to retrieve caption of floating mini frame.
	// Returns:
	//     A pointer to a CFont object
	//-----------------------------------------------------------------------
	CFont* GetSmCaptionFont();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to use Tahoma font despite system metrics.
	// Parameters:
	//     bUseOfficeFont - TRUE to use office font.
	//-----------------------------------------------------------------------
	void UseOfficeFont(bool bUseOfficeFont);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method returns the common controls height.
	// Returns:
	//     Controls height.
	//-----------------------------------------------------------------------
	int GetControlHeight() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws a rectangle.
	// Parameters:
	//     pDC    - Pointer to a valid device context
	//     rc     - Specifies the rectangle in logical units.
	//     nPen   - Specifies the color used to paint the rectangle.
	//     nBrush - Specifies the color used to fill the rectangle.
	//-----------------------------------------------------------------------
	void Rectangle(CDC* pDC, LPCRECT rc, int nPen, int nBrush);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to draw a three-dimensional rectangle.
	// Parameters:
	//     pDC          - Pointer to a valid device context
	//     rc           - Specifies the rectangle in logical units.
	//     nTopLeft     - Specifies the color of the top and left sides
	//                    of the three-dimensional rectangle.
	//     nBottomRight - Specifies the color of the bottom and right sides
	//                    of the three-dimensional rectangle.
	//-----------------------------------------------------------------------
	void Draw3dRect(CDC* pDC, LPCRECT rc, int nTopLeft, int nBottomRight);

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
	// Input:   pDC - Pointer to a valid device context.
	//          rcCheck - Specifies the rectangle in logical units.
	//          clr - Color to fill.
	// Summary: Call this member function to draw a check mark.
	//-----------------------------------------------------------------------
	void DrawCheckMark(CDC* pDC, CRect rcCheck, COLORREF clr = 0) {
		DrawPopupBarCheckMark(pDC, rcCheck, TRUE, clr);
	}

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
	// Summary:
	//     Call this member function to draw a combo box expand symbol
	// Parameters:
	//     pDC        - Pointer to a valid device context
	//     rc    - Specifies the rectangle in logical units.
	//     clr        - Color to fill.
	//-----------------------------------------------------------------------
	virtual void DrawComboExpandMark(CDC* pDC, CRect rc, COLORREF clr);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to draw Menu MDI buttons
	// Parameters:
	//     pDC        - Pointer to a valid device context
	//     pButton    - Button to draw
	//-----------------------------------------------------------------------
	virtual void DrawControlMDIButton(CDC* pDC, CXTPControlButton* pButton);

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws a line.
	// Parameters:
	//     pDC  - Pointer to a valid device context
	//     p0  - Specifies the logical coordinates of the start position.
	//     p1  - Specifies the logical coordinates of the endpoint for the line.
	//     x0   - Specifies the logical x coordinate of the start position.
	//     y0   - Specifies the logical y coordinate of the start position.
	//     x1   - Specifies the logical x coordinate of the endpoint for the line.
	//     y1   - Specifies the logical y coordinate of the endpoint for the line.
	//     nPen - Specifies the color used to paint the line.
	//     clrPen - Specifies the color used to paint the line.
	//-----------------------------------------------------------------------
	void Line(CDC* pDC, CPoint p0, CPoint p1);
	void Line(CDC* pDC, int x0, int y0, int x1, int y1, int nPen); //<combine CXTPPaintManager::Line@CDC*@CPoint@CPoint>
	void HorizontalLine(CDC* pDC, int x0, int y0, int x1, COLORREF clrPen); //<combine CXTPPaintManager::Line@CDC*@CPoint@CPoint>
	void VerticalLine(CDC* pDC, int x0, int y0, int y1, COLORREF clrPen); //<combine CXTPPaintManager::Line@CDC*@CPoint@CPoint>

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to draw a split button expand symbol
	// Parameters:
	//     pDC         - Pointer to a valid device context
	//     pControl    - Points to a CXTPControl object
	//     pt          - Position of expand symbol
	//     bSelected   - TRUE if the control is selected.
	//     bPopuped    - TRUE if the control is popuped.
	//     bEnabled    - TRUE if the control is enabled.
	//     bVert       - TRUE if control is vertical
	//-----------------------------------------------------------------------
	virtual void DrawDropDownGlyph(CDC* pDC, CXTPControl* pControl, CPoint pt, BOOL bSelected, BOOL bPopuped, BOOL bEnabled, BOOL bVert);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the pixel at the point.
	// Parameters:
	//     pDC  - Pointer to a valid device context
	//     xPos    - Specifies the logical x coordinate of the point to be set.
	//     yPos    - Specifies the logical y coordinate of the point to be set.
	//     nPen -  Specifies the color used to paint the point
	//-----------------------------------------------------------------------
	void Pixel(CDC* pDC, int xPos, int yPos, int nPen);

	// -------------------------------------------------------------------------
	// Summary:
	//     Gradient fills the rectangle.
	// Parameters:
	//     pDC :         Pointer to a valid device context
	//     lpRect :      Specifies the rectangle in logical units.
	//     crFrom :      Start color.
	//     crTo :        Endpoint color.
	//     bHorz :       TRUE to fill horizontally.
	//     lpRectClip :  Pointer to a RECT structure that contains a clipping
	//                   rectangle. This parameter is optional and may be set to
	//                   NULL.
	// -------------------------------------------------------------------------
	void GradientFill(CDC* pDC, LPRECT lpRect, COLORREF crFrom, COLORREF crTo, BOOL bHorz, LPCRECT lpRectClip = NULL);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves specific icon's options.
	// See Also: XTP_COMMANDBARS_ICONSINFO
	//-----------------------------------------------------------------------
	XTP_COMMANDBARS_ICONSINFO* GetIconsInfo();

	//////////////////////////////////////////////////////////////////////////
	/// Animation Routines

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to animate the PopupBar.
	// Parameters:
	//     pDestDC       - Pointer to device context you must draw to.
	//     pSrcDC        - Device context that contains the bitmap you must
	//                     take.
	//     rc            - Bounding rectangle.
	//     animationType - Type of animation to perform.
	// See Also: SetAnimationDelay
	//-----------------------------------------------------------------------
	virtual void Animate(CDC* pDestDC, CDC* pSrcDC, CRect rc, XTPAnimationType animationType);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to animate the expanding process.
	// Parameters:
	//     pCommandBar - Command bar to animate.
	//     pDestDC     - Pointer to device context you must draw to.
	//     pSrcDC      - Device context that contains the bitmap you must
	//                   take.
	//     bExpandDown - TRUE to animate expanding top-to-bottom; FALSE to animate  bottom-to-top.
	//-----------------------------------------------------------------------
	virtual void AnimateExpanding(CXTPCommandBar* pCommandBar, CDC* pDestDC, CDC* pSrcDC, BOOL bExpandDown);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set animation delay.
	// Parameters:
	//     nAnimationSteps - Number of steps to take during animation.
	//     nAnimationTime  - Amount of time to rest, in milliseconds, between
	//                       each step.
	//-----------------------------------------------------------------------
	void SetAnimationDelay(int nAnimationSteps, int nAnimationTime);

	//-----------------------------------------------------------------------
	// Summary:
	//     This function retrieves the current color of the specified XP
	//     display element. Display elements are the parts of a window
	//     and the display that appear on the system display screen.
	//     The function returns the red, green, blue (RGB) color value
	//     of the given element.  To provide a consistent look to all
	//     Windows applications, the operating system provides and
	//     maintains global settings that define the colors of various
	//     aspects of the display. These settings as a group are sometimes
	//     referred to as a "color scheme." To view the current color
	//     settings, click Control Panel, double-click the Display icon,
	//     and then click the "Appearance" tab.
	// Parameters:
	//     nIndex - Specifies the display element whose color is to be
	//              retrieved.  This parameter can be one of the
	//              following values displayed in the table below:
	// Remarks:
	//     <TABLE>
	//     <b>Constant</b>                   <b>Value</b>  <b>Description</b>
	//     --------------------------------  ============  ------------------------------------------------------------
	//     COLOR_SCROLLBAR                   0             Scroll bar color
	//     COLOR_BACKGROUND                  1             Desktop color
	//     COLOR_ACTIVECAPTION               2             Color of the title bar for the active window, Specifies the left side color in the color gradient of an active window's title bar if the gradient effect is enabled.
	//     COLOR_INACTIVECAPTION             3             Color of the title bar for the inactive window, Specifies the left side color in the color gradient of an inactive window's title bar if the gradient effect is enabled.
	//     COLOR_MENU                        4             Menu background color
	//     COLOR_WINDOW                      5             Window background color
	//     COLOR_WINDOWFRAME                 6             Window frame color
	//     COLOR_MENUTEXT                    7             Color of text on menus
	//     COLOR_WINDOWTEXT                  8             Color of text in windows
	//     COLOR_CAPTIONTEXT                 9             Color of text in caption, size box, and scroll arrow
	//     COLOR_ACTIVEBORDER                10            Border color of active window
	//     COLOR_INACTIVEBORDER              11            Border color of inactive window
	//     COLOR_APPWORKSPACE                12            Background color of multiple-document interface (MDI) applications
	//     COLOR_HIGHLIGHT                   13            Background color of items selected in a control
	//     COLOR_HIGHLIGHTTEXT               14            Text color of items selected in a control
	//     COLOR_BTNFACE                     15            Face color for three-dimensional display elements and for dialog box backgrounds.
	//     COLOR_BTNSHADOW                   16            Color of shading on the edge of command buttons
	//     COLOR_GRAYTEXT                    17            Grayed (disabled) text
	//     COLOR_BTNTEXT                     18            Text color on push buttons
	//     COLOR_INACTIVECAPTIONTEXT         19            Color of text in an inactive caption
	//     COLOR_BTNHIGHLIGHT                20            Highlight color for 3-D display elements
	//     COLOR_3DDKSHADOW                  21            Darkest shadow color for 3-D display elements
	//     COLOR_3DLIGHT                     22            Second lightest 3-D color after 3DHighlight, Light color for three-dimensional display elements (for edges facing the light source.)
	//     COLOR_INFOTEXT                    23            Color of text in ToolTips
	//     COLOR_INFOBK                      24            Background color of ToolTips
	//     COLOR_HOTLIGHT                    26            Color for a hot-tracked item. Single clicking a hot-tracked item executes the item.
	//     COLOR_GRADIENTACTIVECAPTION       27            Right side color in the color gradient of an active window's title bar. COLOR_ACTIVECAPTION specifies the left side color.
	//     COLOR_GRADIENTINACTIVECAPTION     28            Right side color in the color gradient of an inactive window's title bar. COLOR_INACTIVECAPTION specifies the left side color.
	//     XPCOLOR_TOOLBAR_FACE              30            XP toolbar background color.
	//     XPCOLOR_HIGHLIGHT                 31            XP menu item selected color.
	//     XPCOLOR_HIGHLIGHT_BORDER          32            XP menu item selected border color.
	//     XPCOLOR_HIGHLIGHT_PUSHED          33            XP menu item pushed color.
	//     XPCOLOR_HIGHLIGHT_CHECKED         36            XP menu item checked color.
	//     XPCOLOR_HIGHLIGHT_CHECKED_BORDER  37            An RGB value that represents the XP menu item checked border color.
	//     XPCOLOR_ICONSHADDOW               34            XP menu item icon shadow.
	//     XPCOLOR_GRAYTEXT                  35            XP menu item disabled text color.
	//     XPCOLOR_TOOLBAR_GRIPPER           38            XP toolbar gripper color.
	//     XPCOLOR_SEPARATOR                 39            XP toolbar separator color.
	//     XPCOLOR_DISABLED                  40            XP menu icon disabled color.
	//     XPCOLOR_MENUBAR_FACE              41            XP menu item text background color.
	//     XPCOLOR_MENUBAR_EXPANDED          42            XP hidden menu commands background color.
	//     XPCOLOR_MENUBAR_BORDER            43            XP menu border color.
	//     XPCOLOR_MENUBAR_TEXT              44            XP menu item text color.
	//     XPCOLOR_HIGHLIGHT_TEXT            45            XP menu item selected text color.
	//     XPCOLOR_TOOLBAR_TEXT              46            XP toolbar text color.
	//     XPCOLOR_PUSHED_TEXT               47            XP toolbar pushed text color.
	//     XPCOLOR_TAB_INACTIVE_BACK         48            XP inactive tab background color.
	//     XPCOLOR_TAB_INACTIVE_TEXT         49            XP inactive tab text color.
	//     XPCOLOR_HIGHLIGHT_PUSHED_BORDER   50            An RGB value that represents the XP border color for pushed in 3D elements.
	//     XPCOLOR_CHECKED_TEXT              45            XP color for text displayed in a checked button.
	//     XPCOLOR_3DFACE                    51            XP face color for three- dimensional display elements and for dialog box backgrounds.
	//     XPCOLOR_3DSHADOW                  52            XP shadow color for three-dimensional display elements (for edges facing away from the light source).
	//     XPCOLOR_EDITCTRLBORDER            53            XP color for the border color of edit controls.
	//     XPCOLOR_FRAME                     54            Office 2003 frame color.
	//     XPCOLOR_SPLITTER_FACE             55            XP splitter face color.
	//     XPCOLOR_LABEL                     56            Color for label control (xtpControlLabel)
	//     XPCOLOR_STATICFRAME               57            WinXP Static frame color
	//     </TABLE>
	// Returns:
	//     The red, green, blue (RGB) color value of the given element.
	//-----------------------------------------------------------------------
	COLORREF GetXtremeColor(UINT nIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the colors for each element to the
	//     color in the specified color array.
	// Parameters:
	//     cElements    - Number of elements in array.
	//     lpaElements  - Array of elements.
	//     lpaRgbValues - Array of RGB values.
	//-----------------------------------------------------------------------
	void SetColors(int cElements, CONST INT* lpaElements, CONST COLORREF* lpaRgbValues);

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw control text
	// Parameters:
	//      pDC - Pointer to valid device context
	//      pControl - Pointer to control to draw
	//      rcText - Bounding rectangle of the text
	//      bDraw - TRUE to draw text; FALSE to calculate size
	//      bVert - TRUE if text is vertical
	//      bCentered - TRUE if text is centered
	//      bTriangled - TRUE if triangle drawn
	// Returns: Size of the text
	//-----------------------------------------------------------------------
	CSize DrawControlText(CDC* pDC, CXTPControl* pControl, CRect rcText, BOOL bDraw, BOOL bVert, BOOL bCentered, BOOL bTriangled);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if the font exists in the system.
	// Parameters:
	//     lpszFaceName - Font needed to test.
	// Returns:
	//     TRUE if font exists; otherwise returns FALSE
	//-----------------------------------------------------------------------
	static BOOL AFX_CDECL FontExists(LPCTSTR lpszFaceName);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will search a string, strip off the mnemonic
	//     '&', and reformat the string.
	// Parameters:
	//     strClear - Text needed to strip.
	//-----------------------------------------------------------------------
	static void AFX_CDECL StripMnemonics(CString& strClear);


public:

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to prepare caption fro Right-To-Left draw
	// Parameters:
	//     strCaption - Caption to modify
	//-----------------------------------------------------------------------
	virtual void UpdateRTLCaption(CString& strCaption);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to determine the current Windows XP
	//     theme in use.
	// Returns:
	//     A XTPCurrentSystemTheme enumeration that represents the
	//     current Windows theme in use, can be one of the following
	//     values:
	//     * <b>xtpSystemThemeUnknown</b> Indicates no known theme in use
	//     * <b>xtpSystemThemeBlue</b> Indicates blue theme in use
	//     * <b>xtpSystemThemeOlive</b> Indicates olive theme in use
	//     * <b>xtpSystemThemeSilver</b> Indicates silver theme in use
	// See Also: SetLunaTheme, RefreshLunaColors
	//-----------------------------------------------------------------------
	XTPCurrentSystemTheme GetCurrentSystemTheme();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to force system theme.
	// Parameters:
	//     systemTheme - theme to use. Can be any of the values listed in the Remarks section.
	// Remarks:
	//     theme can be one of the following:
	//     * <b>xtpSystemThemeUnknown</b> Disables <i>luna</i> theme support.
	//     * <b>xtpSystemThemeBlue</b> Enables blue theme
	//     * <b>xtpSystemThemeOlive</b> Enables olive theme
	//     * <b>xtpSystemThemeSilver</b> Enables silver theme
	//-----------------------------------------------------------------------
	void SetLunaTheme(XTPCurrentSystemTheme systemTheme);

	//-----------------------------------------------------------------------
	// Summary:
	//     Calculates gripper of popup bar.
	// Parameters:
	//     pBar - Popup bar.
	// Returns:
	//     Width of gripper to draw.
	//-----------------------------------------------------------------------
	virtual int GetPopupBarGripperWidth(CXTPCommandBar* pBar);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns paint manager used to draw galleries.
	// Returns:
	//     Returns paint manager used to draw galleries.
	//-----------------------------------------------------------------------
	virtual CXTPControlGalleryPaintManager* GetGalleryPaintManager() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns paint manager used to draw scrollbars.
	// Returns:
	//     Returns paint manager used to draw scrollbars.
	//-----------------------------------------------------------------------
	virtual CXTPScrollBarPaintManager* GetScrollBarPaintManager() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns paint manager used to draw slider panes.
	// Returns:
	//     Returns paint manager used to draw slider panes.
	//-----------------------------------------------------------------------
	virtual CXTPSliderPaintManager* GetSliderPaintManager() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns paint manager used to draw progress bars.
	// Returns:
	//     Returns paint manager used to draw progress bars.
	//-----------------------------------------------------------------------
	virtual CXTPProgressPaintManager* GetProgressPaintManager() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns paint manager used to draw frames.
	// Returns:
	//     Returns paint manager used to draw frames.
	//-----------------------------------------------------------------------
	virtual CXTPFramePaintManager* GetFramePaintManager();

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns paint manager used to draw the status bar.
	// Returns:
	//     Returns paint manager used to draw the status bar.
	//-----------------------------------------------------------------------
	virtual CXTPStatusBarPaintManager* GetStatusBarPaintManager();

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves ribbon paint manager.
	// Returns:
	//     Pointer to CXTPRibbonPaintManager object.
	//-----------------------------------------------------------------------
	virtual CXTPRibbonPaintManager* GetRibbonPaintManager();

	virtual CXTPMessageBarPaintManager* GetMessageBarPaintManager();

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns shadow manager used to draw shadows.
	// Returns:
	//     Returns shadow manager used to draw shadows.
	//-----------------------------------------------------------------------
	CXTPShadowManager* GetShadowManager() const;


//{{AFX_CODEJOCK_PRIVATE
public:
	// deprecated.
	void ShowKeyboardCues(BOOL bShow);

	CFont* GetIconBoldFont()
	{
		return &m_fontIconBold;
	}

	virtual void DrawKeyboardTip(CDC* pDC, CXTPCommandBarKeyboardTip* pWnd, BOOL bSetRegion);

	CSize GetAutoIconSize(BOOL bLarge) const;

	void FillCompositeRect(CDC* pDC, LPCRECT lpRect, COLORREF clr);

	static void AFX_CDECL FillCompositeAlpha(CDC* pDC, CRect rc);
	_XTP_DEPRECATE("This function or variable is no longer available. Please use 'DrawControlText' instead")
	CSize DrawTextEx(CDC*, CString, CRect, BOOL, BOOL, BOOL, BOOL, BOOL = FALSE, BOOL = FALSE) {
		return CSize(0, 0);
	}
	int GetSplitDropDownHeight() const;
	void AlphaEllipse(CDC* pDC, CRect rc, COLORREF clrBorder, COLORREF clrFace);

	static void AFX_CDECL DrawTextApi(CDC* pDC, const CString& str, LPRECT lpRect, UINT format, BOOL bComposited);
	CSize DrawControlText2(CDC* pDC, CXTPControl* pControl, CRect rcText, BOOL bDraw, BOOL bVert, BOOL bTrangled);

	int GetEditHeight() const;
	int GetTextHeight() const;
	int GetFontHeight() const;
	void SetFontHeight(int nFontHeight);

protected:
	virtual BOOL IsFlatToolBar(CXTPCommandBar* pCommandBar);

	void AlphaBlendU(PBYTE pDest, PBYTE pSrcBack, int cx, int cy, PBYTE pSrc, BYTE byAlpha);
	void _DrawCheckMark(CDC* pDC, CRect rcCheck, COLORREF clr = 0, BOOL bLayoutRTL = FALSE);

	void DrawControlText3(CDC* pDC, CXTPControl* pControl, const CString& str, const CString& strClear, CRect rcText, BOOL bVert, BOOL bCentered);
	void SplitString(const CString& str, CString& strFirstRow, CString& strSecondRow);

	CSize DrawControlText(CDC* pDC, CXTPControl* pControl, CRect rcText, BOOL bDraw, BOOL bTriangled, CSize szButton, BOOL bDrawImage);
	CSize GetControlSize(CXTPControl* pControl, CSize sz, BOOL bVert);
	virtual CSize GetPopupBarImageSize(CXTPCommandBar* pBar);
	virtual void DrawPopupBarText(CDC* pDC, const CString& strText, CRect& rcText, UINT nFlags, BOOL bDraw, BOOL bSelected, BOOL bEnabled);

	CSize GetIconSize(CXTPControl* pButton);

	CSize GetStretchIconSize(CXTPImageManagerIcon* pImage, CSize szIcon);

	static int CalcEntityHeight(
		int nFontHeight,
		int nFactor,
		int nMinHeight = XTP_COMMANDBAR_MIN_CONTROL_HEIGHT,
		int nCustomDPI = 0);
	static int CalcTextHeight(
		int nFontHeight,
		int nFactor = XTP_COMMANDBAR_TEXT_HEIGHT_FACTOR,
		int nCustomDPI = 0);
	static int CalcCtrlHeight(
		int nFontHeight,
		int nFactor = XTP_COMMANDBAR_CONTROL_HEIGHT_FACTOR,
		int nCustomDPI = 0);
	static int CalcSplitButtonDropDownWidth(int nTextHeight);
	static int CalcSplitButtonDropDownHeight(int nFontHeight);

//}}AFX_CODEJOCK_PRIVATE


// Combo-box / Edit-box
public:
	CXTPPaintManagerColor m_clrEditTextNormal;
	CXTPPaintManagerColor m_clrEditTextDisabled;
	CXTPPaintManagerColor m_clrEditTextHighlighted;

// Message bar
public:
	COLORREF m_clrMessageBarText;           // Message bar text color
	CRect    m_rcMessageBarMargin;          // Margin of the message bar.

// Status bar
public:
	COLORREF m_clrStatusBarBackground;
	COLORREF m_clrStatusBarText;
	COLORREF m_clrStatusTextColor;          // Status bar text color
	COLORREF m_clrStatusTextGrayColor;      // Status bar gray text color.
	COLORREF m_clrStatusGripperLite;        // Status bar gripper light color.
	COLORREF m_clrStatusGripperDark;        // Status bar gripper dark color.

// Paint manager
public:
	CXTPFramePaintManager          *m_pFramePaintManager;      // Frame paint manager
	CXTPStatusBarPaintManager      *m_pStatusBarPaintManager;  // Paint manager of the status bar
	CXTPRibbonPaintManager         *m_pRibbonPaintManager;     // Ribbon Paint Manager
	CXTPControlGalleryPaintManager *m_pGalleryPaintManager;    // Gallery paint manager
	CXTPProgressPaintManager       *m_pProgressPaintManager;   // Progress bar paint manager
	CXTPSliderPaintManager         *m_pSliderPaintManager;     // Slider pane paint manager
	CXTPScrollBarPaintManager      *m_pScrollBarPaintManager;  // Scrollbar paint manager
	CXTPMessageBarPaintManager     *m_pMessageBarPaintManager;

public:
	CXTPPaintManagerColorGradient m_clrDisabledIcon;    // Disabled icons color
	CXTPPaintManagerColor m_clrFloatingGripper;     // Gripper color of floating toolbar.
	CXTPPaintManagerColor m_clrFloatingGripperText; // Gripper text color of floating toolbar.

	BOOL m_bThickCheckMark;             // TRUE to draw thick check mark
	BOOL m_bShowShadow;                 // TRUE to show shadow
	BOOL m_bThemedStatusBar;            // TRUE to draw status bar using WinXP theme.
	BOOL m_bThemedCheckBox;             // TRUE to draw check box using WinXP theme.
	BOOL m_bClearTypeTextQuality;       // TRUE to enable ClearType text for the font.

	BOOL m_bFlatToolBar;                // TRUE to draw toolbars flat
	BOOL m_bFlatMenuBar;                // TRUE to draw menubar flat
	int m_nPopupBarTextPadding;         // Distance between gripper and control text.
	BOOL m_bSelectImageInPopupBar;      // TRUE to select image as in explorer theme.
	BOOL m_bEnableAnimation;            // TRUE to enable animation for all toolbars

	BOOL m_bOffice2007Padding;          // TRUE if Office2007 metrics used
	BOOL m_bOffsetPopupLabelText;       // TRUE if labels in popups drawn with offset
	BOOL m_bWrapCaptionBelowText;       // Wrap text for xtpButtonIconAndCaptionBelow buttons

	CXTPPaintManagerFont m_fontSmCaption;   // Small caption font.
	CXTPPaintManagerFont m_fontIcon;        // Icon font.
	CXTPPaintManagerFont m_fontIconBold;    // Icon bold font.
	CXTPPaintManagerFont m_fontToolTip;     // Tooltip font

	int   m_nSplitButtonDropDownWidth;      // Split button drop down width in toolbars
	int   m_nSplitButtonDropDownHeight;     // Split button drop down height in toolbars
	int   m_nSplitButtonPopupWidth;         // Split button drop down width in popups
	BOOL  m_bAutoResizeIcons;               // TRUE to automatically resize icons using current DPI
	int m_nStatusBarMinHeight;          // Minimum height for the status bar

	PVOID m_pfnSetLayeredWindowAttributes;  // point to Transparency proc in USER32.dll module

	CRect m_rcStatusBarBorder;          // Bounding rectangle of the status bar border

	BOOL  m_bUseOfficeFont;             // TRUE to use Tahoma font.
	CString m_strOfficeFont;            // Office font
	int   m_nTextHeight;                // Regular text field height (not font height).

protected:
	CFont m_fontRegular;                // Regular font.
	CFont m_fontRegularBold;            // Regular bold font.
	CFont m_fontVert;                   // Vertical font.
	CFont m_fontVertBold;               // Vertical font.

	int   m_nEditHeight;                // Edit text height.
	int   m_nFontHeight;                // Custom font height (optional).
	BOOL  m_bUseStandardFont;           // TRUE to use system font.
	BOOL  m_bEmbossedDisabledText;      // TRUE to draw disabled text embossed

	CXTPWinThemeWrapper* m_themeStatusBar;   // StatusBar theme helper.
	CXTPWinThemeWrapper* m_themeButton;      // Buttons theme helper.

	XTPPaintTheme m_themeCurrent;           // Current theme.
	static CXTPPaintManager* s_pInstance;   // Instance of the manager.

	XTP_COMMANDBARS_ICONSINFO m_iconsInfo;  // Specific icons options.


	int m_nAnimationSteps;                  // Steps of the animation.
	int m_nAnimationTime;                   // Time of the animation.

	BOOL m_bShowKeyboardCues;               // TRUE to show keyboard cues - deprecated.

	CXTPPaintManagerColor m_arrColor[XPCOLOR_LAST + 1];  // Self colors array.
	XTPCurrentSystemTheme m_systemTheme;   // Current system theme.

	CXTPShadowManager* m_pShadowManager;            // Shadow manager

protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPaintManager object
	//-----------------------------------------------------------------------
	CXTPPaintManager();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPPaintManager object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPPaintManager();

public:
	//---------------------------------------------------------------------------
	// Summary:
	//     CLogFont is a self initializing LOGFONT derived class. It
	//     will allow you to create or copy a LOGFONT object, and defines the
	//     attributes of a font.
	//---------------------------------------------------------------------------
	struct _XTP_EXT_CLASS CLogFont : public LOGFONT
	{
		//-----------------------------------------------------------------------
		// Summary:
		//     Constructs an CLogFont object.
		//-----------------------------------------------------------------------
		CLogFont();
	};

	//---------------------------------------------------------------------------
	// Summary:
	//     CNonClientMetrics is a self initializing NONCLIENTMETRICS derived
	//     class. It contains the scalable metrics associated with the
	//     non-client area of a non-minimized window.  This class is used by
	//     the SPI_GETNONCLIENTMETRICS and SPI_SETNONCLIENTMETRICS actions of
	//     SystemParametersInfo.
	//---------------------------------------------------------------------------
	struct _XTP_EXT_CLASS CNonClientMetrics : public NONCLIENTMETRICS
	{
		//-----------------------------------------------------------------------
		// Summary:
		//     Constructs a CNonClientMetrics object
		//-----------------------------------------------------------------------
		CNonClientMetrics();
	};

#ifdef _XTP_ACTIVEX
//{{AFX_CODEJOCK_PRIVATE

		/*  x64 #ifdef below is a fix for OLE_HANDLE. While HBITMAP/HICON are still 32-bit on 64-bit Windows, OLE_HANDLE (long) can still be used.
		But OLE_HANDLE cannot be used as HMODULE which is always 64-bit
	*/

#ifndef _WIN64
#define MODULE_HANDLE           OLE_HANDLE
#define MODULE_HANDLE_TYPE      VTS_I4
#else
#define MODULE_HANDLE           LONG64
#define MODULE_HANDLE_TYPE      VTS_I8
#endif

	DECLARE_DISPATCH_MAP()
	DECLARE_OLETYPELIB_EX(CXTPPaintManager);
	DECLARE_INTERFACE_MAP()

	void OleLoadFrameIcon(MODULE_HANDLE Module, LPCTSTR pszFileName, long Width, long Height);
	void OleRefreshMetrics();
	void OleFillWorkspace(OLE_HANDLE hDC, int x, int y, int cx, int cy);
	void OleDrawRectangle(OLE_HANDLE hDC, int x, int y, int cx, int cy, BOOL bSelected, BOOL bPressed, BOOL bEnabled, BOOL bChecked, BOOL bPopuped, XTPBarType barType, XTPBarPosition barPosition);
	COLORREF OleGetRectangleTextColor(BOOL bSelected, BOOL bPressed, BOOL bEnabled, BOOL bChecked, BOOL bPopuped, XTPBarType barType, XTPBarPosition barPosition);
//}}AFX_CODEJOCK_PRIVATE
#endif

	friend class CCommandBarsCtrl;
	friend class CXTPCommandBarsOptions;
	friend class CXTPCommandBars;
	friend class CXTPRibbonPaintManager;
	friend class CXTPStatusBarPaintManager;
	friend class CXTPStatusBarThemeVisualStudio2010;
	friend class CXTPStatusBarThemeOffice2003;
};


//---------------------------------------------------------------------------
// Summary:
//     Call this function to access CXTPPaintManager members.
//     Since this class is designed as a single instance object you can
//     only access version info through this method. You <b>cannot</b>
//     directly instantiate an object of type CXTPPaintManager.
// Example:
//     <code>XTPPaintManager()->GetIconsInfo()->bIconsWithShadow = FALSE;</code>
//---------------------------------------------------------------------------
CXTPPaintManager* XTPPaintManager();

//-----------------------------------------------------------------------
// Summary: Call this function to access CXTPPaintManager members. Since
//          this class is designed as a single instance object you can
//          only access version info through this method. You cannot
//          directly instantiate an object of type CXTPPaintManager.

// Returns: A CXTPPaintManager object.
//-----------------------------------------------------------------------
AFX_INLINE CXTPPaintManager* XTPPaintManager()
{
	if (CXTPPaintManager::s_pInstance == NULL)
	{
		CXTPPaintManager::SetTheme(xtpThemeOffice2000);
	}
	return CXTPPaintManager::s_pInstance;
}






//===========================================================================
// Summary:
//     The CXTPReBarPaintManager template class is used to enable a ReBar theme for Command Bars
// See Also: CXTPPaintManager::SetTheme
//===========================================================================
template <class TBase>
class CXTPReBarPaintManager : public TBase
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPReBarPaintManager object.
	//-----------------------------------------------------------------------
	CXTPReBarPaintManager()
	{
		RefreshMetrics();
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to refresh the visual metrics of the manager.
	//-----------------------------------------------------------------------
	void RefreshMetrics()
	{
		TBase::RefreshMetrics();

		m_themeRebar.OpenThemeData(0, L"REBAR");
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to fill the command bar's face.
	// Parameters:
	//     pDC  - Pointer to a valid device context
	//     pBar - Points to a CXTPCommandBar object to draw.
	//-----------------------------------------------------------------------
	void FillCommandBarEntry (CDC* pDC, CXTPCommandBar* pBar)
	{
		if (pBar->GetPosition() != xtpBarFloating && pBar->GetPosition() != xtpBarPopup)
		{
			if (m_themeRebar.IsAppThemed())
			{
				CXTPWindowRect rcClient(pBar->GetParent());
				pBar->ScreenToClient(&rcClient);

				m_themeRebar.DrawThemeBackground(pDC->GetSafeHdc(), 0, 0, &rcClient, 0);
			}
			else
				pDC->FillSolidRect(CXTPClientRect(pBar), GetSysColor(COLOR_3DFACE));
		}
		else
		{
			TBase::FillCommandBarEntry(pDC, pBar);
		}
	}

protected:
	CXTPWinThemeWrapper m_themeRebar;  // ReBar theme helper.
};






//{{AFX_CODEJOCK_PRIVATE
namespace XTPPaintThemes
{

}
//}}AFX_CODEJOCK_PRIVATE

//////////////////////////////////////////////////////////////////////////

AFX_INLINE void CXTPPaintManager::SetAnimationDelay(int nAnimationSteps, int nAnimationTime)
{
	m_nAnimationSteps = nAnimationSteps;
	m_nAnimationTime = nAnimationTime;
}

AFX_INLINE CFont* CXTPPaintManager::GetRegularFont()
{
	return this == NULL ? CFont ::FromHandle((HFONT)::GetStockObject(DEFAULT_GUI_FONT)): &m_fontRegular;
}

AFX_INLINE CFont* CXTPPaintManager::GetRegularBoldFont()
{
	return &m_fontRegularBold;
}

AFX_INLINE CFont* CXTPPaintManager::GetIconFont()
{
	return &m_fontIcon;
}

AFX_INLINE int CXTPPaintManager::GetFontHeight() const
{
	return m_nFontHeight;
}

AFX_INLINE CFont* CXTPPaintManager::GetSmCaptionFont()
{
	return &m_fontSmCaption;
}

AFX_INLINE void CXTPPaintManager::ShowKeyboardCues(BOOL bShow)
{
	m_bShowKeyboardCues = bShow;
}

AFX_INLINE CXTPScrollBarPaintManager* CXTPPaintManager::GetScrollBarPaintManager() const
{
	return m_pScrollBarPaintManager;
}

AFX_INLINE CXTPControlGalleryPaintManager* CXTPPaintManager::GetGalleryPaintManager() const
{
	return m_pGalleryPaintManager;
}

AFX_INLINE CXTPSliderPaintManager* CXTPPaintManager::GetSliderPaintManager() const
{
	return m_pSliderPaintManager;
}

AFX_INLINE CXTPProgressPaintManager* CXTPPaintManager::GetProgressPaintManager() const
{
	return m_pProgressPaintManager;
}

AFX_INLINE CXTPShadowManager* CXTPPaintManager::GetShadowManager() const
{
	return m_pShadowManager;
}

AFX_INLINE int CXTPPaintManager::GetEditHeight() const
{
	return m_nEditHeight;
}

AFX_INLINE int CXTPPaintManager::GetTextHeight() const
{
	return m_nTextHeight;
}

AFX_INLINE int CXTPPaintManager::GetSplitDropDownHeight() const
{
	return m_nSplitButtonDropDownHeight;
}

AFX_INLINE BOOL CXTPPaintManager::RequiresResourceImages()
{
	return FALSE;
}

#endif // #if !defined(__XTPPAINTMANAGER_H__)
