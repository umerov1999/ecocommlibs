class _XTP_EXT_CLASS CXTPTabColorSetOffice2013 : public CXTPTabPaintManagerColorSet
{
public:
	CXTPTabColorSetOffice2013();

	//-------------------------------------------------------------------------
	// Summary:
	//     This member is called to refresh the visual metrics of the tabs.
	// Remarks:
	//     All of the color members are refreshed when this is called.
	//     This member can be override this member to change the colors of
	//     the color members.
	//-------------------------------------------------------------------------
	virtual void RefreshMetrics();

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
	virtual COLORREF GetItemColor(CXTPTabManagerItem *pItem);

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
	virtual void FillNavigateButton(CDC *pDC, CXTPTabManagerNavigateButton *pButton, CRect &rc);
};
