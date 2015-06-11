//===========================================================================
// Summary:
//     CXTPDockingPaneVisualStudio2005Beta1Theme is CXTPDockingPaneVisualStudio2003Theme derived class, represents
//     Visual Studio 2005 theme for docking panes.
// Remarks:
//     Call CXTPDockingPaneManager::SetTheme(xtpPaneThemeVisualStudio2005Beta1); to set this theme.
//===========================================================================
class _XTP_EXT_CLASS CXTPDockingPaneVisualStudio2005Beta1Theme : public CXTPDockingPaneVisualStudio2003Theme
{
public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPDockingPaneVisualStudio2005Beta1Theme object.
	//-------------------------------------------------------------------------
	CXTPDockingPaneVisualStudio2005Beta1Theme();

public:

	//-------------------------------------------------------------------------
	// Summary:
	//     This method is called to refresh the visual metrics of the pane.
	//-------------------------------------------------------------------------
	void RefreshMetrics();
};
