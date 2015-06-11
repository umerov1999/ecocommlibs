//-------------------------------------------------------------------------
// Summary:
//     CColorSetWinNative is a CXTPTabColorSetDefault derived class that represents the
//     Windows XP tab color set.
// Remarks:
//     To use the Windows XP color set, SetColor is used to apply
//     the xtpTabColorWinNative XTPTabColorStyle.
//
//     The following appearances (XTPTabAppearanceStyle) support CColorSetWinNative:
//     * <b>xtpTabAppearancePropertyPage</b>         Office 2000 appearance.
//     * <b>xtpTabAppearancePropertyPageSelected</b> Office 2000 selected appearance.
//     * <b>xtpTabAppearanceStateButtons</b>         State Button appearance.
//
// See Also: XTPTabColorStyle, XTPTabAppearanceStyle, SetAppearance, GetAppearance, GetAppearanceSet,
//           SetColor, GetColor, GetColorSet, SetColorSet, SetAppearanceSet
//-------------------------------------------------------------------------
class _XTP_EXT_CLASS CColorSetWinNative : public CXTPTabColorSetDefault
{
public:
	CColorSetWinNative();
	virtual ~CColorSetWinNative();

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
	//     This member is called to fill a tab button.
	// Parameters:
	//     pDC    - Pointer to a valid device context.
	//     rcItem - Bounding rectangle of the tab button to fill.
	//     pItem  - Tab button to draw.
	// Returns:
	//     XTP_TABMANAGER_COLOR_NONE
	// Remarks:
	//     This member is used to fill all tab buttons except when the XTPTabAppearanceStyle is
	//     xtpTabAppearanceStateButtons.  xtpTabAppearanceStateButtons require
	//     the FillStateButton member.
	//
	//          If IsAppThemed is FALSE, then CXTPTabColorSetDefault::FillPropertyButton
	//          is used.
	// See Also: FillStateButton
	//-----------------------------------------------------------------------
	virtual COLORREF FillPropertyButton(CDC* pDC, CRect rcItem, CXTPTabManagerItem* pItem);

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
	//     If IsAppThemed is TRUE, then 0 is returned.
	//
	//     If IsAppThemed is FALSE and a tab is selected then the color from CXTPTabManager::GetItemColor
	//     for the currently selected tab is returned, otherwise CXTPTabPaintManagerColorSet::m_clrButtonSelected is returned.
	//-----------------------------------------------------------------------
	virtual COLORREF FillClient(CDC* pDC, CRect rc, CXTPTabManager* pTabManager);

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
	//     This member takes care of filling the tab navigation buttons
	//     that are in the header of the TabClient.
	//     The XTPTabColorStyle CXTPTabPaintManagerColorSet classes override this to perform
	//     actions such as painting the highlighting, pressed, and normal
	//     versions of the tab navigation buttons.
	//
	//     If IsAppThemed is FALSE, then CXTPTabColorSetDefault::FillNavigationButton
	//     is used.
	//
	// See Also: CXTPTabManager::GetNavigateButton, CXTPTabManagerNavigateButton
	//-----------------------------------------------------------------------
	virtual void FillNavigateButton(CDC* pDC, CXTPTabManagerNavigateButton* pButton, CRect& rc);

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
	//
	//     If IsAppThemed is FALSE and/or the tab button is not selected, then
	//     CXTPTabColorSetDefault::FillStateButton is used.
	//
	// See Also: FillPropertyButton
	//-----------------------------------------------------------------------
	virtual void FillStateButton(CDC* pDC, CRect rcItem, CXTPTabManagerItem* pItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     The member is called to determine if this ColorSet supports
	//     Windows XP Themes and if a Windows XP Theme is enabled.
	// Returns:
	//     TRUE if Windows XP themes are enabled and a Windows XP theme
	//     is currently used.
	// Remarks:
	//     IsAppThemed will return FALSE if running an operating system
	//     before Windows XP i.e. (98, 2000, NT) or if the Standard/Clasic
	//     themes are used with Windows XP.
	//
	//     This member is called internally to determine how to colorize
	//     the tab buttons.  If FALSE, CXTPTabPaintManager::CXTPTabColorSetDefault
	//     is used to colorize the tab buttons.  If TRUE, then Windows XP
	//     colorization is used to colorize the tabs.
	// See Also: CXTPTabPaintManager::IsAppThemed
	//-----------------------------------------------------------------------
	virtual BOOL IsAppThemed() const;

private:
	typedef void (AFX_CDECL* LPFNDRAWROTATEDBITS)(int cx, int cy, UINT* pSrcBits, UINT* pDestBits);

//{{AFX_CODEJOCK_PRIVATE
public:
	void DrawRotatedButton(CDC* pDC, CRect rcItem, CXTPTabManagerItem* pItem, BOOL bSwap, LPFNDRAWROTATEDBITS pfnRotatedProc);
	static void AFX_CDECL DrawRotatedBitsBottom(int cx, int cy, UINT* pSrcBits, UINT* pDestBits);
	static void AFX_CDECL DrawRotatedBitsRight(int cx, int cy, UINT* pSrcBits, UINT* pDestBits);
	static void AFX_CDECL DrawRotatedBitsLeft(int cx, int cy, UINT* pSrcBits, UINT* pDestBits);
	static void AFX_CDECL DrawRotatedBitsInvertRight(int cx, int cy, UINT* pSrcBits, UINT* pDestBits);
//}}AFX_CODEJOCK_PRIVATE

protected:
	CXTPWinThemeWrapper* m_themeTabButton;   // Internally used helper to draw PropertyPage Buttons using Windows XP color set.
	CXTPWinThemeWrapper* m_themeToolbar;     // Internally used helper to draw State Button using Windows XP color set.
};

//{{AFX_CODEJOCK_PRIVATE
// obsolete, for backward compatibility only.
class _XTP_EXT_CLASS CColorSetWinXP : public CColorSetWinNative {};
//}}AFX_CODEJOCK_PRIVATE
