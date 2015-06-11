//-------------------------------------------------------------------------
// Summary:
//     CColorSetVisualStudio2003 is a CXTPTabColorSetDefault derived class that represents the
//     Visual Studio tab color set.
// Remarks:
//     To use the Visual Studio color set, SetColor is used to apply
//     the xtpTabColorVisualStudio2003 XTPTabColorStyle.
//
// See Also: XTPTabColorStyle, XTPTabAppearanceStyle, SetAppearance, GetAppearance, GetAppearanceSet,
//           SetColor, GetColor, GetColorSet, SetColorSet, SetAppearanceSet
//-------------------------------------------------------------------------
class _XTP_EXT_CLASS CColorSetVisualStudio2003 : public CXTPTabColorSetDefault
{
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

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called to fill the tab navigation buttons.
	// Parameters:
	//     pDC     - Pointer to a valid device context.
	//     pButton - Tab navigation button to fill.
	//     rc      - Bounding rectangle of the tab navigation button.
	// Remarks:
	//     This member takes care of filling the tab navigation buttons
	//     that are in the header of the TabClient.
	//     The XTPTabColorStyle CXTPTabPaintManagerColorSet classes override this to perform
	//     actions such as painting the highlighting, pressed, and normal
	//     versions of the tab navigation buttons.
	//
	// See Also: CXTPTabManager::GetNavigateButton, CXTPTabManagerNavigateButton
	//-----------------------------------------------------------------------
	void FillNavigateButton(CDC* pDC, CXTPTabManagerNavigateButton* pButton, CRect& rc);

};

//{{AFX_CODEJOCK_PRIVATE
// obsolete, for backward compatibility only.
class _XTP_EXT_CLASS CColorSetVisualStudio : public CColorSetVisualStudio2003 {};
//}}AFX_CODEJOCK_PRIVATE
