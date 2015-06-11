//-------------------------------------------------------------------------
// Summary:
//     CAppearanceSetPropertyPageAccess2007 is a CXTPTabPaintManagerTheme derived
//     class that represents the Office 2007 tab appearance.
// Remarks:
//     To use the Office 2007 appearance, SetAppearance is used to apply
//     the xtpTabAppearancePropertyPage2007 XTPTabAppearanceStyle.
//
// See Also: XTPTabColorStyle, XTPTabAppearanceStyle, SetAppearance, GetAppearance, GetAppearanceSet,
//           SetColor, GetColor, GetColorSet, SetColorSet, SetAppearanceSet
//-------------------------------------------------------------------------
class _XTP_EXT_CLASS CAppearanceSetPropertyPageAccess2007 : public CAppearanceSetPropertyPage2007
{
public:

	//-------------------------------------------------------------------------
	// Summary:
	//     Constructs a CAppearanceSetPropertyPageAccess2007 object.
	//-------------------------------------------------------------------------
	CAppearanceSetPropertyPageAccess2007()
	{
		m_rcHeaderMargin.SetRect(4, 1, 0, 0);
	}

protected:
	//-------------------------------------------------------------------------
	// Summary:
	//     This member is called to refresh the appearance metrics.
	// Remarks:
	//     This member is used to refresh the appearance metrics such
	//     as font, button height, margin size, etc.  Override this
	//     member to change the appearance metrics.
	// See Also: CXTPTabPaintManager::SetFontIndirect, GetHeaderMargin,
	//           CXTPTabPaintManagerTheme::GetHeaderMargin::GetClientMargin,
	//           CXTPTabPaintManagerTheme::GetHeaderMargin::GetButtonLength,
	//           CXTPTabPaintManagerTheme::GetHeaderMargin::GetButtonHeight
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
	//     DrawSingleButton calls CXTPTabPaintManager::DrawSingleButtonIconAndText
	//     after the button is drawn.  This is called to draw the button's
	//     icon and text.
	//
	//     If m_bDoubleHighlightedBorder = TRUE, then a double border
	//     is drawn on mouse over when hot tracking is enabled.
	//
	// See Also:: CXTPTabPaintManager::DrawTabControlEx, CXTPTabPaintManager::DrawSingleButtonIconAndText, m_bDoubleHighlightedBorder
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
	//     xtpTabColorOffice2003
	// See Also: GetSupportedColorSets
	//-----------------------------------------------------------------------
	virtual XTPTabColorStyle GetDefaultColorSet() { return xtpTabColorOffice2007Access; }

	//-----------------------------------------------------------------------
	// Summary:
	//     This method called to get bounding rectangle of item that was drawn
	// Parameters:
	//     pItem - Item to test
	// Returns: Bounding rectangle of drawing item
	// See Also: CXTPTabManagerItem::GetRect
	//-----------------------------------------------------------------------
	CRect GetButtonDrawRect(const CXTPTabManagerItem* pItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to get the top, left, right, and bottom
	//     tab header margins.
	// Returns:
	//     CRect struct containing the left, top, right, and bottom margins
	//     of the tab header.  This is the margin (space) placed around the
	//     tab buttons inside the tab header.
	// Remarks:
	//     The CRect structure will contain the margins of the tab header.
	//     CXTPTabPaintManagerTheme::m_rcHeaderMargin contains
	//     these margins.
	//
	// See Also: CXTPTabPaintManagerTheme::m_rcHeaderMargin, CXTPTabPaintManager::GetHeaderRect
	//-----------------------------------------------------------------------
	virtual CRect GetHeaderMargin()
	{
		return CRect(m_rcHeaderMargin.left, m_rcHeaderMargin.top, m_rcHeaderMargin.right + m_nButtonHeight / 2, m_rcHeaderMargin.bottom);
	}

private:
	void DrawButtonBackground(CDC* pDC, CXTPTabManagerItem* pItem, CRect rc);
};
