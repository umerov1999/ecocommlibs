class CXTPTabManagerItem;
class CXTPTabManager;
class CXTPTabManagerNavigateButton;
class CXTPTabPaintManager;

//-------------------------------------------------------------------------
// Summary:
//     CXTPTabPaintManagerColorSet is a class that represents the colors of all the
//     components (tab button, tab button border, etc) of the tab manager.
// Remarks:
//     CXTPTabPaintManagerColorSet colorizes the different parts that make up a tab, tab header,
//     tab client, and tab manager control.
//
//     Colors should be updated in the RefreshMetrics member's for each
//     CXTPTabPaintManagerColorSet object.
//-------------------------------------------------------------------------
class _XTP_EXT_CLASS CXTPTabPaintManagerColorSet
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Colors used for Excel tab button colorization.
	// Remarks:
	//     Members of COLORSET_TAB_EXCEL are only used when the
	//     XTPTabAppearanceStyle is set to xtpTabThemeExcel2003.
	//
	//     Both the m_csExcelSelected and m_csExcelNormal color sets are
	//     used when the xtpTabThemeExcel2003 appearance is applied.
	//
	//     By default, the "selected" excel style tab will display the
	//     clrLeftInnerBorder and clrTopInnerBorder in addition to the
	//     outer borders to indicate that the tab is selected.
	//
	//     The following color sets customize members of COLORSET_TAB_EXCEL
	//     in addition to the color members of CXTPTabPaintManagerColorSet:
	//     * <b>CXTPTabColorSetOffice2003</b>  Office 2003 Color Set.
	//     * <b>CXTPTabColorSetDefault</b>     Default Color Set.
	//
	// See Also: m_csExcelSelected, m_csExcelNormal
	//-----------------------------------------------------------------------
	struct COLORSET_TAB_EXCEL
	{
		CXTPPaintManagerColor clrLeftInnerBorder;       // Color of the left inner border of the tab button.
		CXTPPaintManagerColor clrTopInnerBorder;        // Color of the top inner border of the tab button.
		CXTPPaintManagerColor clrRightInnerBorder;      // Color of the right inner border of the tab button.
		CXTPPaintManagerColor clrLeftOuterBorder;       // Color of the left outer border of the tab button.
		CXTPPaintManagerColor clrRightOuterBorder;      // Color of the right outer border of the tab button.
		CXTPPaintManagerColor clrTopOuterBorder;        // Color of the top outer border of the tab button.
	};

	//-----------------------------------------------------------------------
	// Summary:
	//     Colors used for Office 2000 tab button colorization.
	// Remarks:
	//     The tab buttons and the tab client area cast a shadow. The shadow consists
	//     of a normal and dark color.  One or both of these colors can be used
	//     depending on the selected XTPTabAppearanceStyle.
	//
	//     Members of COLORSET_TAB_PROPERTYPAGE are only used when the
	//     XTPTabAppearanceStyle is set to xtpTabAppearancePropertyPage,
	//     xtpTabAppearancePropertyPageFlat, xtpTabAppearancePropertyPageSelected,
	//     and xtpTabAppearancePropertyPage2003.
	//
	//     The following color sets customize members of COLORSET_TAB_PROPERTYPAGE
	//     in addition to the color members of CXTPTabPaintManagerColorSet:
	//     * <b>CXTPTabColorSetOffice2003</b> Office 2003 Color Set.
	//     * <b>CColorSetWinNative</b>      Windows XP Color Set.
	//     * <b>CXTPTabColorSetDefault</b>    Default Color Set.
	//
	// See Also: m_csPropertyPage
	//-----------------------------------------------------------------------
	struct COLORSET_TAB_PROPERTYPAGE
	{
		CXTPPaintManagerColor clrHighlight;      // Color of tab button on mouse over when hot tracking is enabled.
		CXTPPaintManagerColor clrShadow;         // Color of the tab button's shadow.
		CXTPPaintManagerColor clrDarkShadow;     // Color of the tab button's dark shadow.
	};

	//-----------------------------------------------------------------------
	// Summary:
	//     Colors used for Office 2003 tab button colorization.
	// Remarks:
	//     Members of COLORSET_TAB_PROPERTYPAGE2003 are only used when the
	//     XTPTabAppearanceStyle is set to xtpTabAppearancePropertyPage2003.
	//
	//     The following color sets customize members of COLORSET_TAB_PROPERTYPAGE2003
	//     in addition to the color members of CXTPTabPaintManagerColorSet:
	//     * <b>CXTPTabColorSetOffice2003</b> Office 2003 Color Set.
	//     * <b>CColorSetVisualStudio2005</b>    Visual Studio 2005 "Whidbey" Color Set.
	//     * <b>CXTPTabColorSetDefault</b>    Default Color Set.
	//
	// See Also: m_csPropertyPage2003
	//-----------------------------------------------------------------------
	struct COLORSET_TAB_PROPERTYPAGE2003
	{
		CXTPPaintManagerColor clrDarkShadow;            // Color of the tab buttons shadow.
		CXTPPaintManagerColor clrFrameBorder;           // Color of the XTPTabClientFrame.
		CXTPPaintManagerColor clrBorderLeftHighlight;   // Color of the left border of a tab button.
		CXTPPaintManagerColor clrBorderTopHighlight;    // Color of the top border of a tab button.
	};

	//-----------------------------------------------------------------------
	// Summary:
	//     Colors used for "State Button" tab button colorization.
	// Remarks:
	//
	//     Members of COLORSET_TAB_STATEBUTTON are only used when the
	//     XTPTabAppearanceStyle is set to xtpTabAppearanceStateButtons.
	//
	//     The following color sets customize members of COLORSET_TAB_STATEBUTTON
	//     in addition to the color members of CXTPTabPaintManagerColorSet:
	//     * <b>CXTPTabColorSetOffice2003</b> Office 2003 Color Set.
	//     * <b>CXTPTabColorSetDefault</b>    Default Color Set.
	//
	// See Also: m_csStateButton
	//-----------------------------------------------------------------------
	struct COLORSET_TAB_STATEBUTTON
	{
		CXTPPaintManagerColorGradient clrFace;    // Color of the currently selected tab button.
		CXTPPaintManagerColorGradient clrBorder;  // Border color of the currently selected tab button.
	};

public:

	//-------------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPTabPaintManagerColorSet object.
	//-------------------------------------------------------------------------
	CXTPTabPaintManagerColorSet();

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPTabPaintManagerColorSet object, handles cleanup and deallocation.
	//-------------------------------------------------------------------------
	virtual ~CXTPTabPaintManagerColorSet();

public:

	//-------------------------------------------------------------------------
	// Summary:
	//     This member is called to refresh the visual metrics of the tabs.
	// Remarks:
	//     All of the color members are refreshed when this is called.
	//     This member can be override this member to change the colors of
	//     the color members.
	//-------------------------------------------------------------------------
	virtual void RefreshMetrics();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to change the color of text in the tab.
	// Parameters:
	//     pDC   - Pointer to a valid device context.
	//     pItem - Change text color of this tab.
	//-----------------------------------------------------------------------
	virtual void SetTextColor(CDC* pDC, CXTPTabManagerItem* pItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called to fill the header of the TabClient of the tab manager.
	// Parameters:
	//     pDC         - Pointer to a valid device context.
	//     rc          - Bounding rectangle of the  tab header.
	//     pTabManager - Fill the header of this CXTPTabManager object.
	// Remarks:
	//     The TabClient header is the bounding rectangle with tab tab buttons.
	//     The bounding rectangle can be retrieved with the CXTPTabPaintManagerTheme::GetHeaderRect,
	//     and CXTPTabManager::GetHeaderRect members.
	// See Also: CXTPTabPaintManagerTheme::GetHeaderRect, CXTPTabManager::GetHeaderRect
	//-----------------------------------------------------------------------
	virtual void FillHeader(CDC* pDC, CRect rc, CXTPTabManager* pTabManager);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called to fill the TabClient area of the tab manager.
	// Parameters:
	//     pDC         - Pointer to a valid device context.
	//     rc          - Bounding rectangle of the tab client area.
	//     pTabManager - Fill the tab client area of this CXTPTabManager object.
	// Remarks:
	//     The TabClient bounding rectangle can be retrieved with the
	//     CXTPTabPaintManagerTheme::GetClientRect,
	//     and CXTPTabManager::GetClientRect members.
	// Returns:
	//     If a tab is selected then the color from CXTPTabManager::GetItemColor
	//     for the currently selected tab is returned,
	//     otherwise m_clrButtonSelected is returned.
	//-----------------------------------------------------------------------
	virtual COLORREF FillClient(CDC* pDC, CRect rc, CXTPTabManager* pTabManager);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called to fill a tab button when the XTPTabAppearanceStyle is
	//     xtpTabAppearanceStateButtons.
	// Parameters:
	//     pDC    - Pointer to a valid device context.
	//     rcItem - Bounding rectangle of the tab button to fill.
	//     pItem  - Tab button to draw.
	// Remarks:
	//     This member is only used to fill buttons when the XTPTabAppearanceStyle is
	//     xtpTabAppearanceStateButtons, for all other buttons, FillPropertyButton is
	//     used.
	// See Also: FillPropertyButton
	//-----------------------------------------------------------------------
	virtual void FillStateButton(CDC* pDC, CRect rcItem, CXTPTabManagerItem* pItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called to fill a tab button.
	// Parameters:
	//     pDC    - Pointer to a valid device context.
	//     rcItem - Bounding rectangle of the tab button to fill.
	//     pItem  - Tab button to draw.
	// Returns:
	//     COLORREF returned from CXTPTabManager::GetItemColor(pItem).
	// Remarks:
	//     This member is used to fill all tab buttons except when the XTPTabAppearanceStyle is
	//     xtpTabAppearanceStateButtons.  xtpTabAppearanceStateButtons require
	//     the FillStateButton member.
	// See Also: FillStateButton
	//-----------------------------------------------------------------------
	virtual COLORREF FillPropertyButton(CDC *pDC, CRect rcItem, CXTPTabManagerItem *pItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to protect client area to be drawn.
	// Parameters:
	//     pDC         - Pointer to a valid device context.
	//     rcClient    - Bounding rectangle of the TabClient area.  This will
	//                   be the clipping region for the device context.
	//     pTabManager - Pointer to the CXTPTabManager.
	//-----------------------------------------------------------------------
	virtual void SelectClipRgn(CDC* pDC, CRect rcClient, CXTPTabManager* pTabManager);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called to fill the tab navigation buttons.
	// Parameters:
	//     pDC     - Pointer to a valid device context.
	//     pButton - Tab navigation button to fill.
	//     rc      - Bounding rectangle of the tab navigation button.
	// Remarks:
	//     This member must be overridden and takes care of filling the
	//     tab navigation buttons that are in the header of the TabClient.
	//     The XTPTabColorStyle CXTPTabPaintManagerColorSet classes override this to perform
	//     actions such as painting the highlighting, pressed, and normal
	//     versions of the tab navigation buttons.
	//
	// See Also: CXTPTabManager::GetNavigateButton, CXTPTabManagerNavigateButton
	//-----------------------------------------------------------------------
	virtual void FillNavigateButton(CDC* pDC, CXTPTabManagerNavigateButton* pButton, CRect& rc) = 0;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the color of a tab.
	// Parameters:
	//     pItem - Get color of this Tab.
	// Returns:
	//     COLORREF of pItem.
	// Remarks:
	//     This is used in many of the CXTPTabPaintManagerColorSet "fill" functions.
	// See Also: CXTPTabManager::GetItemColor, XTPTabColorStyle, SetColor, GetColorSet, SetColorSet
	//-----------------------------------------------------------------------
	virtual COLORREF GetItemColor(CXTPTabManagerItem* pItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to do a gradient fill of a rectangle
	//     with color that smoothly fades from top to bottom.
	//
	// Parameters:
	//     pDC      - Pointer to a valid device context.
	//     rc       - Rectangle to fill with gradient.
	//     clrLight - COLORREF to start gradient from.
	//     clrDark  - COLORREF to fill to.
	//     position - XTPTabPosition of the tab buttons.
	//
	//Remarks:
	//     The color will smoothly fade from clrLight to clrDark, from
	//     top to bottom.
	//
	//     If clrLight is XTP_TABMANAGER_COLOR_SHADED, then the
	//     rectangle will be filled with a special "shaded" fill
	//     type and clrDark will be ignored.  See description of
	//     XTP_TABMANAGER_COLOR_SHADED for more information on this
	//     fill type.
	//
	// See Also: XTP_TABMANAGER_COLOR_SHADED
	//-----------------------------------------------------------------------
	void GradientFill(CDC* pDC, CRect rc, COLORREF clrLight, COLORREF clrDark, XTPTabPosition position);

	//-----------------------------------------------------------------------
	// Summary:
	//     The member is called to determine if this ColorSet supports
	//     Windows XP Themes and if a Windows XP Theme is enabled.
	// Returns:
	//     Always returns FALSE in the base class implementation.
	// Remarks:
	//     This member is overridden in CXTPTabPaintManager::CColorSetWinNative.
	// See Also: CXTPTabPaintManager::CColorSetWinNative::IsAppThemed
	//-----------------------------------------------------------------------
	virtual BOOL IsAppThemed() const;

public:

	CXTPPaintManagerColorGradient m_clrHeaderFace;      // Color set of the tab header background.
	CXTPPaintManagerColorGradient m_clrAutoHideFace;    // Color set of the auto-hide panel background.  This is the tab header for docking pane tabs when the pane is hidden.
	CXTPPaintManagerColor         m_clrFrameBorder;     // Color of border placed around the tab buttons and tab client.
	CXTPPaintManagerColor         m_clrControlFace;     // Color of the control foreground.  This is the area that the tab header and tab client are drawn on.
	CXTPPaintManagerColor         m_clrPaneFace;        // Color of the control pane foreground.
	CXTPPaintManagerColor         m_clrBorderHighlight; // Color of highlight border

	CXTPPaintManagerColor         m_clrButtonNormal;      // Color of normal tabs.  Normal tabs are tabs that are not selected or disabled.
	CXTPPaintManagerColor         m_clrButtonSelected;    // Color of the currently selected tab.
	CXTPPaintManagerColor         m_clrButtonHighlighted; // Tab button color on mouse over when hot tracking is enabled.
	CXTPPaintManagerColor         m_clrButtonPressed;     // Tab pressed button color
	CXTPPaintManagerColor         m_clrButtonBorder;      // Tab border (edge) color.

	CXTPPaintManagerColor         m_clrTextNormal;      // Color of text in a normal tab.  Normal tabs are tabs that are not selected or disabled.
	CXTPPaintManagerColor         m_clrTextInactive;    // Color of text for tabs in all non-active groups of tabs in MDITabClient.
	CXTPPaintManagerColor         m_clrTextSelected;    // Color of text in a "selected" tab button.  The "selected" tab button is the tab with focus.  I.e. The tab becomes active when it is clicked.
	CXTPPaintManagerColor         m_clrTextHighlight;   // Color of text on mouse over when hot tracking is enabled.
	CXTPPaintManagerColor         m_clrTextDisabled;    // Color of text when a tab is disabled.  A disabled tab cannot receive focus.

	CXTPPaintManagerColor         m_clrActiveFilesButton;                      // Color of navigation button.
	CXTPPaintManagerColor         m_clrActiveFilesButtonHighlighted;           // Color of highlighted navigation button.
	CXTPPaintManagerColor         m_clrActiveFilesButtonPressed;               // Color of selected navigation button.
	CXTPPaintManagerColor         m_clrActiveFilesButtonBackground;            // Color of highlighted navigation button background.
	CXTPPaintManagerColor         m_clrActiveFilesButtonBackgroundHighlighted; // Color of navigation button background.
	CXTPPaintManagerColor         m_clrActiveFilesButtonBackgroundPressed;     // Color of selected navigation button background.

	COLORSET_TAB_STATEBUTTON      m_csStateButton;      // Color set of state button style tabs.
	COLORSET_TAB_PROPERTYPAGE     m_csPropertyPage;     // Color set of visual studio style tabs.
	COLORSET_TAB_PROPERTYPAGE2003 m_csPropertyPage2003; // Color set of Office 2003 style tabs.
	COLORSET_TAB_EXCEL            m_csExcelSelected;    // Color set of Selected (focused) Excel style tabs.
	COLORSET_TAB_EXCEL            m_csExcelNormal;      // Color set of Normal Excel style tabs.
	CXTPPaintManagerColor         m_clrFrameHighlightBorder;

protected:
	CXTPTabPaintManager*          m_pPaintManager;      // Self Paint Manager pointer.

	friend class CXTPTabPaintManager;
};

AFX_INLINE void CXTPTabPaintManagerColorSet::SelectClipRgn(CDC* /*pDC*/, CRect /*rcClient*/, CXTPTabManager* /*pTabManager*/)
{
}
