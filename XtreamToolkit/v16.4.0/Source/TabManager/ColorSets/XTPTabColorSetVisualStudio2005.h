//-------------------------------------------------------------------------
// Summary:
//     CColorSetVisualStudio2005 is a CXTPTabColorSetOffice2003 derived class that represents the
//     Visual Studio 2005 "Whidbey" tab color set.
// Remarks:
//     To use the Visual Studio 2005 "Whidbey" color set, SetColor is used to apply
//     the xtpTabColorVisualStudio2005 XTPTabColorStyle.
//
//     The following appearances (XTPTabAppearanceStyle) support CColorSetVisualStudio2005:
//     * <b>xtpTabAppearancePropertyPage2003</b> Office 2003 appearance.
//     * <b>xtpTabAppearanceStateButtons</b>     State Button appearance.
//
// See Also: XTPTabColorStyle, XTPTabAppearanceStyle, SetAppearance, GetAppearance, GetAppearanceSet,
//           SetColor, GetColor, GetColorSet, SetColorSet, SetAppearanceSet
//-------------------------------------------------------------------------
class _XTP_EXT_CLASS CColorSetVisualStudio2005 : public CXTPTabColorSetOffice2003
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CColorSetVisualStudio2005 object.
	//-----------------------------------------------------------------------
	CColorSetVisualStudio2005();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called to refresh the visual metrics of the tabs.
	// Remarks:
	//     All of the color members are refreshed when this is called.
	//     This member can be override this member to change the colors of
	//     the color members.
	//-----------------------------------------------------------------------
	void RefreshMetrics();

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
	//
	//     If m_bGradientButton is TRUE, then CXTPTabColorSetOffice2003::FillPropertyButton
	//     is used to fill the tab button with a gradient fill. See
	//     CXTPTabColorSetOffice2003::FillPropertyButton for details.
	//
	//     If m_bGradientButton is FALSE then CXTPTabPaintManagerColorSet::FillPropertyButton is used.
	//
	// See Also: m_bGradientButton
	//-----------------------------------------------------------------------
	COLORREF FillPropertyButton(CDC* pDC, CRect rcItem, CXTPTabManagerItem* pItem);

protected:
	BOOL m_bGradientButton;         // TRUE if luna colors are enabled.  This will be TRUE when using Windows XP and the current system theme is xtpSystemThemeSilver, xtpSystemThemeOlive, or xtpSystemThemeOlive, FALSE otherwise.
};

//{{AFX_CODEJOCK_PRIVATE
// obsolete, for backward compatibility only.
class _XTP_EXT_CLASS CColorSetWhidbey : public CColorSetVisualStudio2005 {};
//}}AFX_CODEJOCK_PRIVATE
