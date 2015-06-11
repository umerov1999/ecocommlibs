//-------------------------------------------------------------------------
// Summary:
//     CXTPTabThemeOffice2000 is a CXTPTabPaintManagerTheme derived class that represents the
//     Office 2000 tab appearance.
// Remarks:
//     To use the Office 2000 appearance, SetAppearance is used to apply
//     the xtpTabAppearancePropertyPage XTPTabAppearanceStyle.
//
//     CXTPTabThemeOffice2000 supports the following XTPTabColorStyle:
//     * <b>xtpTabColorDefault</b>    Default color style.
//     * <b>xtpTabColorOffice2003</b> Office 2003 color style.
//     * <b>xtpTabColorWinNative</b>      Windows XP color style.
//
// See Also: XTPTabColorStyle, XTPTabAppearanceStyle, SetAppearance, GetAppearance, GetAppearanceSet,
//           SetColor, GetColor, GetColorSet, SetColorSet, SetAppearanceSet
//-------------------------------------------------------------------------
class _XTP_EXT_CLASS CXTPTabThemeOffice2000 : public CXTPTabPaintManagerTheme
{
public:

	//-------------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPTabThemeOffice2000 object.
	//-------------------------------------------------------------------------
	CXTPTabThemeOffice2000()
	{
		m_rcHeaderMargin.SetRect(2, 3, 6, 0);
	}

	//-------------------------------------------------------------------------
	// Summary:
	//     This member is called to refresh the appearance metrics.
	// Remarks:
	//     This member is used to refresh the appearance metrics such
	//     as font, button height, margin size, etc.  Override this
	//     member to change the appearance metrics.
	// See Also:
	//     CXTPTabPaintManager::SetFontIndirect, CXTPTabPaintManagerTheme::GetHeaderMargin,
	//     CXTPTabPaintManagerTheme::GetHeaderMargin::GetClientMargin,
	//     CXTPTabPaintManagerTheme::GetHeaderMargin::GetButtonLength,
	//     CXTPTabPaintManagerTheme::GetHeaderMargin::GetButtonHeight
	//-------------------------------------------------------------------------
	virtual void RefreshMetrics();

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
	//          DrawSingleButton calls CXTPTabPaintManager::DrawSingleButtonIconAndText
	//          after the button is drawn.  This is called to draw the button's
	//          icon and text.
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
	//     xtpTabColorDefault
	// See Also: GetSupportedColorSets
	//-----------------------------------------------------------------------
	virtual XTPTabColorStyle GetDefaultColorSet() { return xtpTabColorDefault; }

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to get the top, left, right, and bottom
	//     tab client area margins.
	// Returns:
	//     CRect struct containing the left, top, right, and bottom margins
	//     of the tab client area.  This is the margin (space) placed around the
	//     tab client area inside the tab control.
	//
	// Remarks:
	//     The CRect structure will contain the margins of the tab client area.
	//     CXTPTabPaintManager::m_rcClientMargin contains these margins.
	//
	//     This is the margin placed around the tab client area in the tab control.
	//
	// Example:
	// <code>
	// // Set the top, left, right, and bottom tab header margins to 5 pixels.  This will
	// // place a margin of 5 pixels around the tab buttons in the tab header.
	// m_wndTabControl.GetPaintManager()->GetAppearanceSet()->m_rcHeaderMargin.SetRect(5, 5, 5, 5);
	//
	// // Set the top, left, right, and bottom tab control margins to 4 pixels.  This will
	// // place a margin of 4 pixels around the entire tab control (Includes tab header and tab client area).
	// m_wndTabControl.GetPaintManager()->m_rcControlMargin.SetRect(4, 4, 4, 4);
	//
	// // Set the top, left, right, and bottom tab button margins to 2 pixels.  This will
	// // place a margin of 2 pixels around the text and icon in the tab buttons.
	// m_wndTabControl.GetPaintManager()->m_rcButtonMargin.SetRect(2, 2, 2, 2);
	//
	// // Set the top, left, right, and bottom tab client margins to 10 pixels.  This will
	// // place a margin of 10 pixels around the tab client area.
	// m_wndTabControl.GetPaintManager()->m_rcClientMargin.SetRect(10, 10, 10, 10);
	// </code>
	// See Also: CXTPTabPaintManager::m_rcClientMargin, CXTPTabPaintManagerTheme::GetHeaderMargin,
	//-----------------------------------------------------------------------
	virtual CRect GetClientMargin();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method called to get bounding rectangle of item that was drawn
	// Parameters:
	//     pItem - Item to test
	// Returns: Bounding rectangle of drawing item
	// See Also: CXTPTabManagerItem::GetRect
	//-----------------------------------------------------------------------
	virtual CRect GetButtonDrawRect(const CXTPTabManagerItem* pItem);

	virtual CRect GetHeaderClipBox(CDC* pDC, CXTPTabManager* pTabManager);
};

class CXTPTabPaintManager::CAppearanceSetPropertyPage : public CXTPTabThemeOffice2000
{

};
