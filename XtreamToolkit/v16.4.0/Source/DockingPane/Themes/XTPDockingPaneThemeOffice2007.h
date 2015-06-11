//===========================================================================
// Summary:
//     CXTPDockingPaneOffice2007Theme is CXTPDockingPaneOffice2003Theme derived class, represents
//     Office 2007 theme for docking panes.
// Remarks:
//     Call CXTPDockingPaneManager::SetTheme(xtpPaneThemeResource); to set this theme.
//===========================================================================
class _XTP_EXT_CLASS CXTPDockingPaneOffice2007Theme : public CXTPDockingPaneOffice2003Theme
{
public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPDockingPaneOffice2007Theme object.
	//-------------------------------------------------------------------------
	CXTPDockingPaneOffice2007Theme();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to refresh the visual metrics of the pane.
	//-----------------------------------------------------------------------
	void RefreshMetrics();

	//-----------------------------------------------------------------------
	// Summary:
	//     Override this member function to draw a caption.
	// Parameters:
	//     pDC - Reference to the device context in which to draw.
	//     pPane - Pointer to the tabbed container.
	//     rc - Client rectangle of the tabbed container.
	//-----------------------------------------------------------------------
	void DrawCaption(CDC *pDC, CXTPDockingPaneTabbedContainer *pPane, CRect rc);
};
