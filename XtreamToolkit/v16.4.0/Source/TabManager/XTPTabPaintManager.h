// XTPTabPaintManager.h: interface for the CXTPTabPaintManager class.
//
// This file is a part of the XTREME TOOLKIT PRO MFC class library.
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
#if !defined(__XTPTABPAINTMANAGER_H__)
#define __XTPTABPAINTMANAGER_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXTPTabManager;
class CXTPTabManagerAtom;
class CXTPTabManagerItem;
class CXTPTabManagerNavigateButton;
class CXTPWinThemeWrapper;
class CXTPTabPaintManager;
class CXTPTabPaintManagerColorSet;
class CXTPTabPaintManagerTheme;


//-------------------------------------------------------------------------
// Summary:
//     CXTPTabPaintManager is a CCmdTarget derived class that is used to
//     store the color and appearance information of the tabs in the
//     tab manager.
//-------------------------------------------------------------------------
class _XTP_EXT_CLASS CXTPTabPaintManager : public CXTPCmdTarget
{
public:

	class CAppearanceSetPropertyPage;
	class CAppearanceSetFlat;

public:

	class CColorSetDefault;

public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPTabPaintManager object.
	//-------------------------------------------------------------------------
	CXTPTabPaintManager();

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPTabPaintManager object, handles cleanup and deallocation.
	//-------------------------------------------------------------------------
	virtual ~CXTPTabPaintManager();


public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to apply one of the "built-in" color sets to
	//     the tabs.
	// Parameters:
	//     tabColor - XTPTabColorStyle to apply to the tabs.
	// Returns:
	//     Pointer to the CXTPTabPaintManagerColorSet object applied.
	// See Also: XTPTabColorStyle, SetColorSet, GetColor, GetColorSet, XTPTabAppearanceStyle,
	//           SetAppearanceSet, SetAppearance, GetAppearance, GetAppearanceSet
	//-----------------------------------------------------------------------
	CXTPTabPaintManagerColorSet* SetColor(XTPTabColorStyle tabColor);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to apply a custom color set to the tabs.
	// Parameters:
	//     pColorSet - Pointer to custom CXTPTabPaintManagerColorSet
	//                 appearance set.
	// Returns:
	//     Pointer to the newly set custom color set.
	// See Also:
	//     XTPTabColorStyle, SetColor, GetColor, GetColorSet, XTPTabAppearanceStyle,
	//     SetAppearanceSet, SetAppearance, GetAppearance, GetAppearanceSet
	//-----------------------------------------------------------------------
	CXTPTabPaintManagerColorSet* SetColorSet(CXTPTabPaintManagerColorSet* pColorSet);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to apply one of the "built-in" appearances to
	//     the tabs.
	// Parameters:
	//     tabAppearance - XTPTabAppearanceStyle to apply to the tabs.
	// Returns:
	//     Pointer to the CXTPTabPaintManagerTheme object applied.
	// Remarks:
	//     Use SetAppearance to apply a "Built-In" theme.
	//     To apply a custom them, use the SetAppearanceSet member.
	// See Also: XTPTabAppearanceStyle, SetAppearanceSet, GetAppearance, GetAppearanceSet,
	//           SetColor, GetColor, GetColorSet, SetColorSet
	//-----------------------------------------------------------------------
	CXTPTabPaintManagerTheme* SetAppearance(XTPTabAppearanceStyle tabAppearance);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to apply a custom appearance set.
	// Parameters:
	//     pAppearanceSet - Pointer to custom CXTPTabPaintManagerTheme
	//                      appearance set.
	// Remarks:
	//     An appearance set specifies how the tabs will look.  This
	//     is the actual physical structure of the tab buttons.  For example,
	//     tab buttons with an Excel appearance style will have a
	//     half hexagon shape.  SetColorSet is used to set custom
	//     colors.
	//
	//     See the GUI_Eclipse sample for an example of how to create
	//     a custom appearance set and apply it to the tabs.
	// Returns:
	//     Pointer to newly set custom appearance set.
	// See Also: XTPTabAppearanceStyle, SetAppearance, GetAppearance, GetAppearanceSet,
	//           SetColor, GetColor, GetColorSet, SetColorSet
	//-----------------------------------------------------------------------
	CXTPTabPaintManagerTheme* SetAppearanceSet(CXTPTabPaintManagerTheme* pAppearanceSet);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the currently set custom appearance set.
	// Returns:
	//     Pointer to the currently used custom appearance set.
	// Remarks:
	//     An appearance set specifies how the tabs will look.  This
	//     is the actual physical structure of the tab buttons.  For example,
	//     tab buttons with an Excel appearance style will have a
	//     half hexagon shape.  SetColorSet is used to set
	//     custom colors.
	//
	//     See the GUI_Eclipse sample for an example of how to create
	//     a custom appearance set and apply it to the tabs.
	// See Also: GetAppearanceSet, SetAppearanceSet, SetAppearance
	//-----------------------------------------------------------------------
	CXTPTabPaintManagerTheme* GetAppearanceSet() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the currently used custom color set.
	// Returns:
	//     Currently used custom color set.
	// See Also: XTPTabColorStyle, GetColor, SetColorSet, SetColor
	//-----------------------------------------------------------------------
	CXTPTabPaintManagerColorSet* GetColorSet() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine the currently set appearance style.
	// Returns:
	//     The currently set XTPTabAppearanceStyle.
	// Remarks:
	//     The appearance is the physical structure of the tab buttons.
	// See Also: XTPTabAppearanceStyle, GetAppearanceSet, SetAppearanceSet, SetAppearance
	//-----------------------------------------------------------------------
	XTPTabAppearanceStyle GetAppearance() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the currently used tab color style.
	// Returns:
	//     Currently used color style.
	// Remarks:
	//     Color styles are used to colorize the tabs.  For example,
	//     applying the xtpTabColorWinNative color style will give the
	//     tabs Windows XP colorization.
	// See Also: XTPTabColorStyle, SetColor, GetColorSet, SetColorSet
	//-----------------------------------------------------------------------
	XTPTabColorStyle GetColor() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to determine is luna colors are disabled.
	// Returns:
	//     TRUE if luna colors are disabled, FALSE is luna colors are
	//     enabled.
	// See Also: DisableLunaColors
	//-----------------------------------------------------------------------
	BOOL IsLunaColorsDisabled() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to enable/disable luna colors.
	// Parameters:
	//     bDisableLunaColors - TRUE to disable luna colors, FALSE to
	//                          enable luna colors.
	// Remarks:
	//     Luna colors are the Windows XP colors (Silver, Blue, Olive).
	//     CXTPDrawHelpers::GetCurrentSystemTheme can be called to get
	//     the currently used system theme.
	// See Also: IsLunaColorsDisabled, XTPCurrentSystemTheme,
	//           CXTPDrawHelpers::GetCurrentSystemTheme
	//-----------------------------------------------------------------------
	void DisableLunaColors(BOOL bDisableLunaColors);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the COLORREF value for the specified OneNote color.
	// Parameters:
	//     tabColor - XTPTabOneNoteColor flag indicating which OneNote color
	//                to retrieve. See remarks section for available flags.
	// Remarks:
	//     The tab manager calls this member function to retrieve the RGB
	//     color for the value specified by <i>tabColor</i>. <i>tabColor</i>
	//     can be one of the following values:
	//     * <b>xtpTabColorBlue</b>    Blue tab color used when OneNote
	//                                 colors enabled.
	//     * <b>xtpTabColorYellow</b>  Yellow tab color used when OneNote
	//                                 colors enabled.
	//     * <b>xtpTabColorGreen</b>   Green tab color used when OneNote
	//                                 colors enabled.
	//     * <b>xtpTabColorRed</b>     Red tab color used when OneNote
	//                                 colors enabled.
	//     * <b>xtpTabColorPurple</b>  Purple tab color used when OneNote
	//                                 colors enabled.
	//     * <b>xtpTabColorCyan</b>    Cyan tab color used when OneNote
	//                                 colors enabled.
	//     * <b>xtpTabColorOrange</b>  Orange tab color used when OneNote
	//                                 colors enabled.
	//     * <b>xtpTabColorMagenta</b> Magenta tab color used when OneNote
	//                                 colors enabled.
	// Example:
	//     See WM_XTP_GETTABCOLOR for an example.
	// See Also:
	//     XTPTabOneNoteColor
	//-----------------------------------------------------------------------
	static COLORREF AFX_CDECL GetOneNoteColor(XTPTabOneNoteColor tabColor);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to enable\disable OneNote colorization for
	//     the tabs.
	// Parameters:
	//     bOneNoteColors - TRUE to enable OneNote colors, FALSE to
	//                      disable OneNote colors.
	// Remarks:
	//     OneNote colors are similar to the tab colorization of tabs in
	//     Microsoft OneNote.
	//-----------------------------------------------------------------------
	void SetOneNoteColors(BOOL bOneNoteColors);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the tab position.
	// Parameters:
	//     tabPosition - New XTPTabPosition to apply to tab manager.
	// Remarks:
	//     The position refers to where the tab buttons are located. The
	//     tab buttons can be positioned on the top, bottom, left, or right
	//     side of the tab client area.
	// See Also: GetPosition
	//-----------------------------------------------------------------------
	void SetPosition(XTPTabPosition tabPosition);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the tab layout style.
	// Parameters:
	//     tabLayout - New XTPTabLayoutStyle to apply to the tabs.
	// Remarks:
	//     The layout refers to how the tab buttons are sized within the
	//     tab client header.  The tab buttons can be in a compressed,
	//     fixed, auto size, or size to fit layout.
	// See Also: GetLayout
	//-----------------------------------------------------------------------
	void SetLayout(XTPTabLayoutStyle tabLayout);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the position of the tab buttons.
	// Returns:
	//     XTPTabPosition of tab buttons.
	// Remarks:
	//     The position refers to where the tab buttons are located. The
	//     tab buttons can be positioned on the top, bottom, left, or right
	//     side of the tab client area.
	// See Also: SetPosition
	//-----------------------------------------------------------------------
	XTPTabPosition GetPosition() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the currently set tab layout.
	// Returns:
	//     Currently used XTPTabLayoutStyle.
	// Remarks:
	//     The layout refers to how the tab buttons are sized within the
	//     tab client header.  The tab buttons can be in a compressed,
	//     fixed, auto size, or size to fit layout.
	// See Also: SetLayout
	//-----------------------------------------------------------------------
	XTPTabLayoutStyle GetLayout() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the font for the tabs.
	// Parameters:
	//     pLogFont - New LOGFONT to use for text in the tab buttons.
	//     bUseStandardFont - When calling SetFontIndirect, always
	//                        use FALSE, this will indicate that pLogFont
	//                        should be used as the new tab font.
	//
	//                        TRUE is used when SetFontIndirect
	//                        is called internally when the system font
	//                        is changed.
	//
	// Remarks:
	//     When bUseStandardFont is TRUE, the standard system "icon" font
	//     will be used.  The standard "icon" font is Tahoma 8pt.
	//-----------------------------------------------------------------------
	void SetFontIndirect(LOGFONT* pLogFont, BOOL bUseStandardFont = FALSE);

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called to draw the tab control.
	// Parameters:
	//     pTabManager - Pointer to the tab manager to draw.
	//     pDC         - Pointer to a valid device context.
	//     rcControl   - Bounding rectangle of the client area.
	// Remarks:
	//     This member is called in various Draw and Paint functions.
	//
	//     This member will call CXTPTabPaintManagerTheme::DrawTabControl
	//     for the currently set appearance. CXTPTabPaintManagerTheme::DrawTabControl
	//     will then call CXTPTabPaintManager::DrawTabControlEx.
	//-----------------------------------------------------------------------
	virtual void DrawTabControl(CXTPTabManager* pTabManager, CDC* pDC, CRect rcControl);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called to adjust the client rectangle.  It will
	//     return the new client rectangle with margins included.
	// Parameters:
	//     pTabManager - Pointer to the tab manager to adjust the client rectangle.
	//     rcClient    - [out] CRect to store client rectangle.
	// Remarks:
	//     AdjustClientRect and RepositionTabControl are called when the window
	//     is resized.  When DrawTabControl is called the changes will be
	//     used when drawing the tab control.
	//
	//     This member will adjust the client rectangle of the entire tab
	//     manager if IsDrawStaticFrame is TRUE to make room to draw the static
	//     frame.
	//
	//     Then CXTPTabPaintManagerTheme::AdjustClientRect is called
	//     if there is one or more tabs in the tab manager.
	// See Also:
	//     CXTPTabPaintManagerTheme::AdjustClientRect
	//-----------------------------------------------------------------------
	virtual void AdjustClientRect(CXTPTabManager* pTabManager, CRect& rcClient);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to move the tab buttons of the tab control to their
	//     correct positions.
	// Parameters:
	//     pTabManager - Pointer to the tab manager the tab button are on.
	//     pDC         - Pointer to a valid device context.
	//     rcControl   - Client rectangle of control.
	// Remarks:
	//     RepositionTabControl and AdjustClientRect are called when the window
	//     is resized.  When DrawTabControl is called the changes will be
	//     used when drawing the tab control.
	//
	//     CXTPTabPaintManagerTheme::RepositionTabControl is then
	//     called for the currently set appearance.
	//-----------------------------------------------------------------------
	virtual void RepositionTabControl(CXTPTabManager* pTabManager, CDC* pDC, CRect rcControl);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called to reposition the tab navigation buttons.
	// Parameters:
	//     pTabManager - Pointer to the tab manager the navigation button are on.
	//     rcClient    - Client rectangle of control.
	// Returns:
	//     Tab header rectangle including header margins.
	// Remarks:
	//     This member is called in CXTPTabPaintManager::RepositionTabControlEx
	//     to reposition the tab navigation buttons.
	//-----------------------------------------------------------------------
	CRect RepositionNavigateButtons(CXTPTabManager* pTabManager, CRect rcClient);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called to fill the tab navigation buttons.
	// Parameters:
	//     pDC     - Pointer to a valid device context.
	//     pButton - Tab navigation button to fill.
	//     rc      - Bounding rectangle of the tab navigation button.
	// Remarks:
	//     This member calls m_pColorSet->FillNavigateButton.
	//
	//          The XTPTabColorStyle CXTPTabPaintManagerColorSet classes override this to perform
	//          actions such as painting the highlighting, pressed, and normal
	//          versions of the tab navigation buttons.
	//
	// See Also: CXTPTabPaintManagerColorSet::FillNavigateButton, CXTPTabManager::GetNavigateButton, CXTPTabManagerNavigateButton
	//-----------------------------------------------------------------------
	void FillNavigateButton(CDC* pDC, CXTPTabManagerNavigateButton* pButton, CRect& rc);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called to draw tab navigation buttons.
	// Parameters:
	//     pDC     - Pointer to a valid device context.
	//     pButton - Tab navigation button to fill.
	//     rc      - Bounding rectangle of the tab navigation button.
	// See Also: CXTPTabPaintManagerColorSet::FillNavigateButton, CXTPTabManager::GetNavigateButton, CXTPTabManagerNavigateButton
	//-----------------------------------------------------------------------
	virtual void DrawNavigateButton(CDC* pDC, CXTPTabManagerNavigateButton* pButton, CRect& rc);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the tab button's text and icon.
	// Parameters:
	//     pDC    - Pointer to a valid device context.
	//     pItem  - Tab button to draw text and icon on.
	//     rcItem - Bounding rectangle of the tab.
	//     bDraw  - TRUE to draw the text and icon, FALSE to calculate
	//              the button width (no drawing is done).
	// Returns:
	//     Returns 0 if bDraw is TRUE and drawing is successful.
	//
	//          Returns the button width including the left and right button margin
	//          if bDraw is FALSE.
	//
	// Remarks:
	//     This method only draws the text and icon on a tab button.
	//
	//          CXTPTabPaintManagerTheme::DrawSingleButton calls
	//          CXTPTabPaintManager::DrawSingleButtonIconAndText
	//          after the button is drawn.  This is called to draw the button's
	//          icon and text.
	//
	// See Also: CXTPTabPaintManager::DrawTabControlEx, CXTPTabPaintManagerTheme::DrawSingleButton
	//-----------------------------------------------------------------------
	virtual int DrawSingleButtonIconAndText(CDC* pDC, CXTPTabManagerItem* pItem, CRect rcItem, BOOL bDraw);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to ensure that a tab is visible.
	// Parameters:
	//     pTabManager - Pointer to the tab manager the tab in on.
	//     pItem       - CXTPTabManagerItem tab to make visible.
	// See Also: CXTPTabManager::SetHeaderOffset, CXTPTabManager::GetHeaderOffset
	//-----------------------------------------------------------------------
	void EnsureVisible(CXTPTabManager* pTabManager, CXTPTabManagerItem* pItem);

	//-------------------------------------------------------------------------
	// Summary:
	//     This member is called to refresh the visual metrics of the
	//     tab main manager.
	// Remarks:
	//     This member checks if m_bUseStandardFont is TRUE, if it is,
	//     then it updates the font with the standard system "icon" font
	//     which is Tahoma 8pt.
	//     This member also calls the RefreshMetrics member for the
	//     currently used CXTPTabPaintManagerColorSet and CXTPTabPaintManagerTheme.
	//-------------------------------------------------------------------------
	virtual void RefreshMetrics();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member adds a control to the list of controls that receive
	//     notification when a property of paint manager has changed.
	// Parameters:
	//     pObserver - List of controls that receive
	//     notification when a property of paint manager has changed.
	// Remarks:
	//     This member is not used in the MFC version.  This is only used
	//     in the ActiveX version and does nothing in MFC.
	//-----------------------------------------------------------------------
	void AddObserver(CXTPTabManagerAtom* pObserver);


	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to enable or disable tooltips show
	// Parameters:
	//     behaviour - Tooltips behaviour will be set. See remarks section for available flags.
	// Remarks:
	//     <i>behaviour</i> parameter can be one of the following:
	//     * <b>xtpTabToolTipNever</b> Show tooltips for tabs always
	//     * <b>xtpTabToolTipAlways</b> Doesn't show tooltips for tabs
	//     * <b>xtpTabToolTipShrinkedOnly</b> Show tooltips only if tab was shrinked (see xtpTabLayoutSizeToFit layout)
	//-----------------------------------------------------------------------
	void EnableToolTips(XTPTabToolTipBehaviour behaviour = xtpTabToolTipAlways);

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the CFont used for displaying tab
	//     caption text.
	// Parameters:
	//     bVertical - TRUE if the XTPTabPosition is vertical, FALSE if
	//                 the XTPTabPosition is horizontal.
	// Returns:
	//     CFont used to display caption text in the tab buttons.
	// See Also: GetBoldFont
	//-----------------------------------------------------------------------
	CFont* GetFont(BOOL bVertical = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get the CFont used for displaying tab
	//     caption text when bold.
	// Parameters:
	//     bVertical - TRUE if the XTPTabPosition is vertical, FALSE if
	//                 the XTPTabPosition is horizontal.
	// Returns:
	//     CFont used to display bold caption text in the tab buttons.
	// Remarks:
	//     Bold font is used to display caption text when a tab is
	//     selected and m_bBoldSelected = TRUE.
	//
	//          If m_bBoldNormal = TRUE, then the tabs will use this font
	//          even when not selected.
	// See Also: m_bBoldNormal, m_bBoldSelected, GetFont
	//-----------------------------------------------------------------------
	CFont* GetBoldFont(BOOL bVertical = FALSE);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called to change the tab button size when the
	//     XTPTabLayoutStyle is xtpTabLayoutSizeToFit.
	// Parameters:
	//     pTabManager - Pointer to tab manager.
	//     nWidth      - Total width that all tab buttons must fit in.
	// Remarks:
	//     When calculating nWidth, the left and right rcHeaderMargin and
	//     tab navigation button widths should be subtracted from the
	//     TabClient width.
	// See Also: XTPTabLayoutStyle, SetLayout, GetLayout
	//-----------------------------------------------------------------------
	void SizeToFit(CXTPTabManager* pTabManager, int nWidth);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called to draw the tab control.
	// Parameters:
	//     pTabManager - Pointer to the tab manager to draw.
	//     pDC         - Pointer to a valid device context.
	//     rcControl   - Bounding rectangle of the client area.
	// Remarks:
	//     DrawTabControl calls CXTPTabPaintManagerTheme::DrawTabControl
	//     which will then call DrawTabControlEx if it has not been overridden.
	// See Also:
	//     CXTPTabPaintManagerTheme::DrawTabControl
	//-----------------------------------------------------------------------
	void DrawTabControlEx(CXTPTabManager* pTabManager, CDC* pDC, CRect rcControl);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to move the tab buttons of the tab control to their
	//     correct positions.
	// Parameters:
	//     pTabManager - Pointer to the tab manager the tab button are on.
	//     pDC         - Pointer to a valid device context.
	//     rcClient    - Client rectangle of control
	// Remarks:
	//     RepositionTabControl calls CXTPTabPaintManagerTheme::RepositionTabControl
	//     which will then call RepositionTabControlEx if it has not been overridden.
	//
	//     RepositionTabControl and AdjustClientRect are called when the window
	//     is resized.  When DrawTabControl is called the changes will be
	//     used when drawing the tab control.
	// See Also:
	//     CXTPTabPaintManagerTheme::RepositionTabControl
	//-----------------------------------------------------------------------
	void RepositionTabControlEx(CXTPTabManager* pTabManager, CDC* pDC, CRect rcClient);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to move the tab buttons of the multi row tab control to their
	//     correct positions.
	// Parameters:
	//     pTabManager - Pointer to the tab manager the tab button are on.
	//     pDC         - Pointer to a valid device context.
	//     rcClient    - Client rectangle of control
	//-----------------------------------------------------------------------
	void RepositionTabControlMultiRow(CXTPTabManager* pTabManager, CDC* pDC, CRect rcClient);

	//-------------------------------------------------------------------------
	// Summary:
	//     This member is called when a property of the tab paint manager
	//     has changed.
	// Remarks:
	//     This member is called when the value of a property is changed.
	//     For example, when hot tracking in enabled/disabled.
	//-------------------------------------------------------------------------
	virtual void OnPropertyChanged();

protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called to get the total button length including
	//     the left and right button margin.
	// Parameters:
	//     nValue - Width of tab button.
	//     nMin   - Minimum width of tab button.
	//     nMax   - Maximum width of tab button.
	// Returns:
	//     Tab button length including the left and right button margin.
	// See Also: CXTPTabManager::GetItemMetrics
	//-----------------------------------------------------------------------
	int _GetButtonLength(int nValue, int nMin, int nMax);


	// -----------------------------------------------------------------------
	// Summary:
	//     This member is called to place an ellipse "..." in the middle of a
	//     string.
	// Parameters:
	//     pDC :       Pointer to a valid device context.
	//     pManager :  Pointer to the tab manager to draw.
	//     strItem :   String item to place an ellipse in.
	//     rcItem :    Bounding rectangle of tab button. This is the rectangle
	//                 (in logical coordinates) in which the text is to be
	//                 formatted.
	//     uFormat :   See nFormat parameter of CDC\:\:DrawText.
	//
	// -----------------------------------------------------------------------
	void DrawTextPathEllipsis(CDC* pDC, CXTPTabManager* pManager, const CString& strItem, CRect rcItem, UINT uFormat);

protected:

//{{AFX_CODEJOCK_PRIVATE
	int GetPrefixTextExtent(CDC* pDC, const CString& str, UINT uFormat);
	void StripMnemonics(CString& strClear);
	void DrawRowItems(CXTPTabManager* pTabManager, CDC* pDC, const CRect& rcClient, int nItemRow);
	void CreateMultiRowIndexer(CXTPTabManager* pTabManager, CDC* pDC, int nWidth);
	virtual void DrawFocusRect(CDC* pDC, CXTPTabManagerItem* pItem, CRect rcItem);
	BOOL _CreateMultiRowIndexerBestFit(CXTPTabManager* pTabManager, int nWidth, int nRow, int nTotalLength);
	BOOL _CreateMultiRowIndexerPlain(CXTPTabManager* pTabManager, int nWidth, int nRow, int nTotalLength);

	int _ReduceNumberOfRowsForPlainIndexer(CXTPTabManager* pTabManager, int nRowCount, int nWidth, int nTotalLength); // returns nRowCount or a lesser value when plain indexer can produce extra rows

	static int _cdecl _SizeToFitCompare(const void *arg1, const void *arg2);

//}}AFX_CODEJOCK_PRIVATE

public:
	BOOL              m_bHotTracking;               // If TRUE, tab hot tracking will be enabled.
	BOOL              m_bShowIcons;                 // If TRUE, tab icons will be drawn is icons were added
	BOOL              m_bBoldSelected;              // If TRUE, the text of selected tabs will be displayed in bold font
	BOOL              m_bBoldNormal;                // If TRUE, bold font will be used for all tab both selected and non-selected.
	BOOL              m_bDisableLunaColors;         // If FALSE, Tabs will not use LunaColors, is the current theme uses LunaColors
	BOOL              m_bOneNoteColors;             // If TRUE, tabs will use OneNote colorization
	XTPTabClientFrame m_clientFrame;                // Frame style of tab client area.
	BOOL              m_bStaticFrame;               // If TRUE, a static frame will be drawn around the entire tab area
	CRect             m_rcClientMargin;             // Margin around tab client area.
	CRect             m_rcControlMargin;            // Margin around entire tab control (tab client header and tab client area).
	CRect             m_rcButtonMargin;             // Margin around the text in tab button.
	CRect             m_rcButtonTextPadding;             // Margin around the text in tab button.
	BOOL              m_bInvertGradient;            // If TRUE, the colors passed into CXTPTabPaintManagerColorSet::GradientFill will be swapped before the rectangle is filled with the gradient.
	BOOL              m_bFillBackground;            // If TRUE, then the tab client space is filled using CXTPTabPaintManager.CXTPTabPaintManagerColorSet.m_clrControlFace color.  This is only FALSE when drawing DockingPanePanel AutoHide tabs because there is no client area to draw.
	CSize             m_szIcon;                     // Size of icon displayed in tab button.
	BOOL              m_bDrawTextPathEllipsis;      // If TRUE, when the size of a tab button is too small to display the caption the middle of the caption will be replaced with an ellipse "...", If FALSE, the the end of the caption will be replaced with the ellipse.
	BOOL              m_bDrawTextEndEllipsis;       // If TRUE, when the size of a tab button is too small to display the caption the end of the caption will be replaced with an ellipse "...", If FALSE, the the end of the caption will be replaced with the ellipse.
	BOOL              m_bDrawTextNoPrefix;          // If FALSE, then an ampersand '&' in the tab button caption will be ignored and a double ampersand '&&' will display a single ampersand '&'.  If TRUE, the the caption will be displayed unmodified.
	UINT              m_nDrawTextFormat;            // Text format for tabs (default - DT_LEFT | DT_VCENTER)
	BOOL              m_bDrawTextHidePrefix;        // If FALSE, then an ampersand '&' in the tab button caption will be ignored and a double ampersand '&&' will display a single ampersand '&'.  If TRUE, the the caption will be hidden.
	BOOL              m_bMultiRowFixedSelection;    // FALSE to move row with selected item close to client pane.
	BOOL              m_bMultiRowJustified;         // Stretch each row of tabs to fill the entire width of the control.
	BOOL              m_bDrawFocusRect;             // TRUE to draw focus rect for focused item.
	int               m_bSelectOnDragOver;          // 1 to activate tab when user drag files/text under it. 2 - to activate with small delay.
	int               m_nButtonExtraLength;         // Additional padding of tabs
	BOOL              m_bSelectOnButtonDown;        // if TRUE, when tab selected on mouse button down.
	BOOL              m_bClearTypeTextQuality;      // TRUE to enable ClearType text for the font.
	BOOL              m_bRotateImageOnVerticalDraw; // TRUE to rotate image when tab located left or right
	BOOL              m_bClipHeader;                // TRUE to select clip rectangle before draw tabs
	CSize             m_szNavigateButton;           // Navigate buttons size
	BOOL              m_bShowTabs;                  // TRUE to show tabs
	BOOL              m_bMultiRowTabsOptimization;  // TRUE if the stab manager will arrange tabs best according to their widths. True by default. Otherwise - plain ordering

	int               m_nMinTabWidth;               // Minimum tabs width
	int               m_nMaxTabWidth;               // Maximum tabs width
	int               m_nFixedTabWidth;             // Tabs width for fixed layout
	XTPTabToolTipBehaviour m_toolBehaviour;         // Tootips behaviour
	BOOL              m_bVerticalTextTopToBottom;   // Draw vertical Text Top-to-Bottom or Bottom-to-Top

	static DWORD      m_nSelectOnDragOverDelay;     // Delay before Select on Drag Over (300 by default)

protected:
	CXTPTabPaintManagerTheme*   m_pAppearanceSet;             // Currently set CXTPTabPaintManagerTheme.
	CXTPTabPaintManagerColorSet*        m_pColorSet;                  // Currently used CXTPTabPaintManagerColorSet.
	CArray<CXTPTabManagerAtom*, CXTPTabManagerAtom*> m_arrObservers; // List of controls that receive notification when a property of paint manager has changed.
	XTPTabAppearanceStyle m_tabAppearance;          // Currently set appearance style.
	XTPTabColorStyle  m_tabColor;                   // Currently used color style.
	XTPTabPosition    m_tabPosition;                // Currently set tab button position.  The position refers to where the tab buttons are located. The tab buttons can be positioned on the top, bottom, left, or right side of the tab client area.
	XTPTabLayoutStyle m_tabLayout;                  // Currently set tab button layout.  The layout refers to how the tab buttons are sized within the tab client header.
	CFont             m_fntNormal;                  // Font used to display text in tab buttons.
	CFont             m_fntBold;                    // Font used to display bold text in the tab buttons.  I.e. When a tab is selected and m_bBoldSelected = TRUE.
	CFont             m_fntVerticalNormal;          // Font used to display tab button caption when the tab position is vertical.
	CFont             m_fntVerticalBold;            // Font used to display bold text in the tab buttons when the tab position is vertical.  I.e. When a tab is selected and m_bBoldSelected = TRUE.
	BOOL              m_bUseStandardFont;           // TRUE to use standard system "icon" font which is Tahoma 8pt.

private:
	int m_nItemColor;

	friend class CXTPTabPaintManagerTheme;
	friend class CXTPTabPaintManagerColorSet;
	friend class CXTPTabManager;

#ifdef _XTP_ACTIVEX
//{{AFX_CODEJOCK_PRIVATE
public:
	afx_msg BOOL OleIsColorSupported(long nColor);
	afx_msg LPDISPATCH OleGetClientMargin();
	afx_msg LPDISPATCH OleGetControlMargin();
	afx_msg LPDISPATCH OleGetHeaderMargin();
	afx_msg LPDISPATCH OleGetButtonMargin();
	afx_msg void OleSetFont(LPFONTDISP pFontDisp);
	afx_msg LPFONTDISP OleGetFont();
	afx_msg LPDISPATCH OleGetColorSet();
	afx_msg void OleSetIconSize(long cx, long cy);
	afx_msg void OleRefreshMetrics();
	afx_msg void OnClearTypeTextQualityChanged();
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPTabPaintManager);

	friend class CXTPTabPaintManagerMargin;
//}}AFX_CODEJOCK_PRIVATE
#endif

};



//////////////////////////////////////////////////////////////////////////

AFX_INLINE CFont* CXTPTabPaintManager::GetFont(BOOL bVertical)
{
	return bVertical ? &m_fntVerticalNormal : &m_fntNormal;
}

AFX_INLINE CFont* CXTPTabPaintManager::GetBoldFont(BOOL bVertical)
{
	return bVertical ? &m_fntVerticalBold : &m_fntBold;
}

AFX_INLINE CXTPTabPaintManagerTheme* CXTPTabPaintManager::GetAppearanceSet() const
{
	return m_pAppearanceSet;
}

AFX_INLINE CXTPTabPaintManagerColorSet* CXTPTabPaintManager::GetColorSet() const
{
	return m_pColorSet;
}

AFX_INLINE void CXTPTabPaintManager::EnableToolTips(XTPTabToolTipBehaviour behaviour)
{
	m_toolBehaviour = behaviour;
}


#endif // !defined(__XTPTABPAINTMANAGER_H__)
