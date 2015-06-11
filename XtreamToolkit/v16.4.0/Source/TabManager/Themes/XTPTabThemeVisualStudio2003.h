//-------------------------------------------------------------------------
// Summary:
//     CXTPTabThemeVisualStudio2003 is a CXTPTabPaintManagerTheme derived
//     class that represents the Visual Studio tab appearance.
// Remarks:
//     To use the Visual Studio appearance, SetAppearance is used to apply
//     the xtpTabAppearanceVisualStudio XTPTabAppearanceStyle.
//
//     CXTPTabThemeVisualStudio2003 supports the following XTPTabColorStyle:
//     * <b>xtpTabColorDefault</b>      Default color style.
//     * <b>xtpTabColorVisualStudio2003</b> Visual Studio color style.
//     * <b>xtpTabColorOffice2003</b>   Office 2003 color style.
//     * <b>xtpTabColorWinNative</b>        Windows XP color style.
//     * <b>xtpTabColorVisualStudio2005</b>      Visual Studio 2005 "Whidbey" color style.
//
// See Also: XTPTabColorStyle, XTPTabAppearanceStyle, SetAppearance, GetAppearance, GetAppearanceSet,
//           SetColor, GetColor, GetColorSet, SetColorSet, SetAppearanceSet
//-------------------------------------------------------------------------
class _XTP_EXT_CLASS CXTPTabThemeVisualStudio2003 : public CXTPTabPaintManagerTheme
{
public:

	//-------------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPTabThemeVisualStudio2003 object.
	//-------------------------------------------------------------------------
	CXTPTabThemeVisualStudio2003()
	{
		m_rcHeaderMargin.SetRect(4, 2, 4, 0);
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw a single tab button in the tab
	//     client header area.
	// Parameters:
	//     pDC   - Pointer to a valid device context.
	//     pItem - Tab button to draw.
	// Remarks:
	//     This method will draw a single tab button.  This method only
	//     draws the button, no text or icon is added.
	//
	//     DrawSingleButton calls CXTPTabPaintManager::DrawSingleButtonIconAndText
	//     after the button is drawn.  This is called to draw the button's
	//     icon and text.
	// See Also:: CXTPTabPaintManager::DrawTabControlEx, CXTPTabPaintManager::DrawSingleButtonIconAndText
	//-----------------------------------------------------------------------
	virtual void DrawSingleButton(CDC* pDC, CXTPTabManagerItem* pItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member fills in the tab header and tab client area.  It
	//     will then draw tab client frame.
	// Parameters:
	//     pTabManager - Pointer to tab manager to fill.
	//     pDC         - Pointer to a valid device context.
	//     rcControl   - Bounding rectangle of the tab manager control.
	//                   See CXTPTabManager::GetControlRect.
	// Returns:
	//     Tab header bounding rectangle.  This value is returned from GetHeaderRect.
	// Remarks:
	//     If CXTPTabPaintManager::m_bFillBackground is TRUE, then the tab client
	//     space is filled using CXTPTabPaintManagerColorSet::m_clrControlFace color.
	//     CXTPTabPaintManager::m_bFillBackground is only FALSE when drawing
	//     DockingPanePanel AutoHide tabs because there is no client area to draw.
	// See Also: CXTPTabPaintManagerTheme::GetHeaderRect, CXTPTabPaintManager::m_bFillBackground, XTPTabClientFrame, CXTPTabPaintManager::m_clientFrame
	//-----------------------------------------------------------------------
	virtual CRect FillTabControl(CXTPTabManager* pTabManager, CDC* pDC, CRect rcControl);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the default XTPTabColorStyle for this appearance.
	// Returns:
	//     xtpTabColorVisualStudio2003
	// See Also: GetSupportedColorSets
	//-----------------------------------------------------------------------
	virtual XTPTabColorStyle GetDefaultColorSet() { return xtpTabColorVisualStudio2003; }
};

#define CAppearanceSetVisualStudio CXTPTabThemeVisualStudio2003
