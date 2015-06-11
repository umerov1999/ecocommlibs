//===========================================================================
// Summary:
//     CXTPDockingPaneOffice2002VisioTheme is CXTPDockingPaneVisualStudio2003Theme derived class, represents
//     Visio theme for docking panes.
// Remarks:
//     Call CXTPDockingPaneManager::SetTheme(xtpPaneThemeOffice2002Visio); to set this theme.
//===========================================================================
class _XTP_EXT_CLASS CXTPDockingPaneOffice2002VisioTheme : public CXTPDockingPaneVisualStudio2003Theme
{
public:

	//-------------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPDockingPaneOffice2002VisioTheme object.
	//-------------------------------------------------------------------------
	CXTPDockingPaneOffice2002VisioTheme();

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
	void DrawCaption(CDC* pDC, CXTPDockingPaneTabbedContainer* pPane, CRect rc);

	//-----------------------------------------------------------------------
	// Summary:
	//     Override this member function to draw a tabbed container.
	// Parameters:
	//     pDC    - Reference to the device context in which to draw.
	//     pPane - Pointer to the tabbed container.
	//     rc    - Client rectangle of the tabbed container.
	//-----------------------------------------------------------------------
	void DrawPane(CDC* pDC, CXTPDockingPaneTabbedContainer* pPane, CRect rc);

	//-----------------------------------------------------------------------
	// Summary:
	//     Override this member to change the caption rectangle of the child
	//     docking pane.
	// Parameters:
	//     pPane - Pointer to a CXTPDockingPaneTabbedContainer object.
	//     rc    - Caption rectangle to be changed.
	//-----------------------------------------------------------------------
	void AdjustCaptionRect(const CXTPDockingPaneTabbedContainer* pPane, CRect& rc);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to draw frame of floating window
	// Parameters:
	//     pDC    - Pointer to a valid device context
	//     pPane - Floating window pointer
	//     rc    - Client rectangle of floating frame
	//-----------------------------------------------------------------------
	void DrawFloatingFrame(CDC* pDC, CXTPDockingPaneMiniWnd* pPane, CRect rc);
};
