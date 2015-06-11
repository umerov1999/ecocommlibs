class _XTP_EXT_CLASS CXTPTabThemeVisualStudio2010 : public CAppearanceSetPropertyPageFlat
{
public:

	//-------------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPTabThemeVisualStudio2010 object.
	// Remarks:
	//     By default, m_bBlurPoints is TRUE.  This will smooth the edges
	//     of the tabs.
	//-------------------------------------------------------------------------
	CXTPTabThemeVisualStudio2010()
	{
		m_bDrawNormalTab = FALSE;
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
	//
	//     If m_bBlurPoints is TRUE, then the corners of the tabs will be
	//     smoothed.
	//
	// See Also:: CXTPTabPaintManager::DrawTabControlEx, CXTPTabPaintManager::DrawSingleButtonIconAndText, m_bBlurPoints
	//-----------------------------------------------------------------------
	virtual void DrawSingleButton(CDC* pDC, CXTPTabManagerItem* pItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get the default XTPTabColorStyle for this appearance.
	// Returns:
	//     xtpTabColorDefault
	// See Also: GetSupportedColorSets
	//-----------------------------------------------------------------------
	virtual XTPTabColorStyle GetDefaultColorSet() { return xtpTabColorVisualStudio2010; }

public:
};

#define CAppearanceSetVisualStudio2010 CXTPTabThemeVisualStudio2010 // Old name
