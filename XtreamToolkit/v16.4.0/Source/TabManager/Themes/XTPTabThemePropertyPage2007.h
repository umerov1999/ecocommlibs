//-------------------------------------------------------------------------
// Summary:
//     CAppearanceSetPropertyPage2007 is a CXTPTabPaintManagerTheme derived
//     class that represents the Office 2007 tab appearance.
// Remarks:
//     To use the Office 2007 appearance, SetAppearance is used to apply
//     the xtpTabAppearancePropertyPage2007 XTPTabAppearanceStyle.
//
// See Also: XTPTabColorStyle, XTPTabAppearanceStyle, SetAppearance, GetAppearance, GetAppearanceSet,
//           SetColor, GetColor, GetColorSet, SetColorSet, SetAppearanceSet
//-------------------------------------------------------------------------
class _XTP_EXT_CLASS CAppearanceSetPropertyPage2007 : public CXTPTabPaintManagerTheme
{
public:

	//-------------------------------------------------------------------------
	// Summary:
	//     Constructs a CAppearanceSetPropertyPage2003 object.
	//-------------------------------------------------------------------------
	CAppearanceSetPropertyPage2007()
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
	virtual XTPTabColorStyle GetDefaultColorSet() { return xtpTabColorResource; }

//{{AFX_CODEJOCK_PRIVATE
protected:
	typedef void (AFX_CDECL* LPFNDRAWROTATEDBITS)(int cx, int cy, UINT* pSrcBits, UINT* pDestBits);
	void DrawRotatedButton(CDC* pDC, CRect rcItem, CXTPTabManagerItem* pItem, BOOL bSwap, LPFNDRAWROTATEDBITS pfnRotatedProcBack, LPFNDRAWROTATEDBITS pfnRotatedProc);
	virtual void DrawButtonBackground(CDC* pDC, CXTPTabManagerItem* pItem, CRect rc);
//}}AFX_CODEJOCK_PRIVATE
};
