//===========================================================================
// Summary:
//     CXTPDockingPaneOffice2003OutlookTheme is CXTPDockingPaneOffice2003Theme derived class, represents
//     Office 2003 theme for docking panes.
// Remarks:
//     Call CXTPDockingPaneManager::SetTheme(xtpPaneThemeOffice2003Outlook); to set this theme.
//===========================================================================
class _XTP_EXT_CLASS CXTPDockingPaneOffice2003OutlookTheme : public CXTPDockingPaneOffice2003Theme
{
public:

	//-------------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPDockingPaneOffice2003OutlookTheme object.
	//-------------------------------------------------------------------------
	CXTPDockingPaneOffice2003OutlookTheme();

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPDockingPaneOffice2003OutlookTheme object, handles cleanup and deallocation.
	//-------------------------------------------------------------------------
	virtual ~CXTPDockingPaneOffice2003OutlookTheme();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to refresh the visual metrics of the pane.
	//-----------------------------------------------------------------------
	virtual void RefreshMetrics();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get caption gripper for tabbed container
	// Parameters:
	//     pPane - Tabbed container to test
	// Returns: Bounding rectangle of gripper to set size cursor
	//-----------------------------------------------------------------------
	virtual CRect GetCaptionGripperRect(const CXTPDockingPaneTabbedContainer* pPane);
};
