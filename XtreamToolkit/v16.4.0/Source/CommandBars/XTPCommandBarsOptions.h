class CXTPCommandBars;
class CXTPPropExchange;

//===========================================================================
// Summary:
//     Options of the command bars.
// See Also:
//     CXTPCommandBars::GetCommandBarsOptions
// Example:
//     <code>pCommandBars->GetCommandBarsOptions()->animationType = xtpAnimateFade;</code>
//===========================================================================
class _XTP_EXT_CLASS CXTPCommandBarsOptions : public CXTPCmdTarget
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs an CXTPCommandBarsOptions object
	//-----------------------------------------------------------------------
	CXTPCommandBarsOptions();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Reads or writes options from or to an archive.
	// Parameters:
	//     ar - A CArchive object to serialize to or from.
	//-----------------------------------------------------------------------
	void Serialize(CArchive& ar);

	//-----------------------------------------------------------------------
	// Summary:
	//     Reads or writes options from or to an archive.
	// Parameters:
	//     pPX - A CXTPPropExchange object to serialize to or from.
	//     bUserOptionsOnly - TRUE to serialize user's options only.
	//-----------------------------------------------------------------------
	void DoPropExchange(CXTPPropExchange* pPX, BOOL bUserOptionsOnly = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to enable/disble menu underlines.
	// Parameters:
	//     keyboardCues - determines if menu underlines is enabled.
	// Remarks:
	//     keyboardCues parameter can be one of the following:
	//        * xtpKeyboardCuesShowAlways - Show keyboard cues always
	//        * xtpKeyboardCuesShowNever - Show keyboard cues always
	//        * xtpKeyboardCuesShowWindowsDefault - System defined
	//-----------------------------------------------------------------------
	void ShowKeyboardCues(XTPKeyboardCuesShow keyboardCues);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get menu underlines state
	// Returns:
	//     Returns one of the following:
	//        * xtpKeyboardCuesShowAlways - Show keyboard cues always
	//        * xtpKeyboardCuesShowNever - Show keyboard cues always
	//        * xtpKeyboardCuesShowWindowsDefault - System defined
	//-----------------------------------------------------------------------
	XTPKeyboardCuesShow GetShowKeyboardCues() const;

public:
	BOOL bAlwaysShowFullMenus;                          // TRUE to show full menus always.
	BOOL bShowFullAfterDelay;                           // TRUE to show full menus after short delay.
	BOOL bToolBarScreenTips;                            // TRUE to show toolbar's tooltips
	BOOL bToolBarAccelTips;                             // TRUE to add accelerators to tooltips.
	BOOL bLargeIcons;                                   // TRUE to use large toolbars icons.
	BOOL bSyncFloatingBars;                             // TRUE to hide floating bars when site frame deactivated.
	BOOL bShowExpandButtonAlways;                       // TRUE to show expand buttons always.
	BOOL bDblClickFloat;                                // TRUE to make toolbar floating be double-clicking.
	BOOL bShowTextBelowIcons;                           // TRUE to show captions below icons.
	BOOL bAltDragCustomization;                         // Allows users to change a toolbar button's position by dragging it while holding down ALT.
	BOOL bAutoHideUnusedPopups;                         // TRUE to hide unused popup controls.  This is similar to how the Windows Start menu hides infrequently used popups.
	BOOL bDirtyState;                                   // TRUE if the user has done any type of customization to menus or toolbars. (I.e. Moving a toolbar or adding a new button).  After a load or save of the BarStae this is set to FALSE.
	BOOL bShowPopupBarToolTips;                         // TRUE to display ToolTips for controls in popup menus.

	BOOL bShowContextMenuAccelerators;                  // TRUE to show accelerators in contect menus.

	XTPAnimationType animationType;                     // Animation type.
	BOOL bAutoUpdateShortcuts;                          // TRUE to update shortcuts from accelerator table
	DWORD keyboardCuesUse;                              // Keyboard cues using
	BOOL bFlyByStatus;                                  // Status bar displays information about the button

	CSize szIcons;                                      // Size of the normal icons.
	CSize szLargeIcons;                                 // Size of the large icons.
	CSize szPopupIcons;                                 // Size of icons displayed in popup menus.

	BOOL bUseAltNumPadKeys;                             // TRUE to allow activate menu with Alt + num pad key.
	BOOL bUseSystemSaveBitsStyle;                       // TRUE to use CS_SAVEBITS class style for popups and shadows
	BOOL bDisableCommandIfNoHandler;                    // TRUE to disable commands if no handler presented
	BOOL bWrapLargePopups;                              // Wrap controls for large popups instead scroll them.

	CMap<UINT, UINT, BOOL, BOOL> m_mapHiddenCommands;   // Hidden command array.
	CMap<UINT, UINT, BOOL, BOOL> m_mapUsedCommands;     // Used commands array.
	BOOL bShowKeyboardTips;                             // TRUE to use keyboard tips.

	BOOL bShowMiniToolbarTips;                          // TRUE to show tips for controls in a mini toolbar, default is FALSE.

protected:

	// DPI-scalling parameters
	BOOL m_bAutoScaleDpiGalleryItems;                       // TRUE by default. Enables scalling of ribbon gallery items (both inline and displayed in popup)
	BOOL m_bAutoScaleDpiToolbarIcons;                       // FALSE by default. Enables scalling of toolbar (on ribbon as well) buttons icons

public:

	void SetDPIScallingOptions(BOOL bScaleGalleryItems, BOOL bScaleToolbarIcons = FALSE);

	BOOL GetGalleryItemsScalling() const;
	BOOL GetToolbarsIconsScalling() const;

private:
	CXTPCommandBars* m_pCommandBars;
	XTPKeyboardCuesShow keyboardCuesShow;

#ifdef _XTP_ACTIVEX
//{{AFX_CODEJOCK_PRIVATE

	long nUpdatePeriod;
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPCommandBarsOptions);
public:
	afx_msg BOOL OleGetOfficeStyleDisabledIcons();
	afx_msg void OleSetOfficeStyleDisabledIcons(BOOL bOfficeStyleDisabledIcons);
	afx_msg BOOL OleGetFadedIcons();
	afx_msg void OleSetFadedIcons(BOOL);
	afx_msg BOOL OleGetDisabledIcons();
	afx_msg void OleSetDisabledIcons(BOOL);
	afx_msg BOOL OleGetIconsWithShadow();
	afx_msg void OleSetIconsWithShadow(BOOL);
	afx_msg void OleSetIconSize(BOOL bBigIcons, long cx, long cy);
	afx_msg void OleSetPopupIconSize(long cx, long cy);
	afx_msg void OleAddHiddenCommand(long nIDCommand);
	afx_msg BOOL OleGetLunaColors();
	afx_msg void OleSetLunaColors(BOOL bLunaColors);
	afx_msg BOOL OleGetUseSharedImageList();
	afx_msg void OleSetUseSharedImageList(BOOL bUseSharedImageList);
	afx_msg void OleSetFont(LPFONTDISP pFontDisp);
	afx_msg double OleGetMaxPopupWidth();
	afx_msg void OleSetMaxPopupWidth(double dMaxWidthDivisor);
	LPFONTDISP OleGetFont();
	afx_msg BOOL OleIsBalloonStyleSupported();
	afx_msg int OleGetShowKeyboardCues();
	afx_msg LPFONTDISP OleGetSmallCaptionFont();
	afx_msg LPFONTDISP OleGetComboBoxFont();
	afx_msg void OleSetSmallCaptionFont(LPFONTDISP pFontDisp);
	afx_msg void OleSetComboBoxFont(LPFONTDISP pFontDisp);
	afx_msg void OleSetDPIScallingOptions(BOOL bScaleGalleryItems, BOOL bScaleToolbarIcons);

	afx_msg void OleUpdatePeriodChanged();
	afx_msg void OleRemoveHiddenCommands();
	DECLARE_PROPERTY(ExpandHoverDelay, long);
	DECLARE_PROPERTY(ExpandDelay, long);

	BOOL bSyncActiveFloatingFrames;

//}}AFX_CODEJOCK_PRIVATE
#endif

	friend class CXTPCommandBars;
};

static const TCHAR _xtpCommandBarOptionsSection[] = _T("%s-Options");
