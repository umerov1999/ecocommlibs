//===========================================================================
// Summary:
//     CXTPDockingPaneVisualStudio2008Theme is CXTPDockingPaneVisualStudio2005Theme
//     derived class, represents Visual Studio 2008 theme for docking panes.
// Remarks:
//     Call CXTPDockingPaneManager::SetTheme(xtpPaneThemeVisualStudio2008); to set
//     this theme.
//===========================================================================
class _XTP_EXT_CLASS CXTPDockingPaneVisualStudio2008Theme : public CXTPDockingPaneVisualStudio2005Theme
{
public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPDockingPaneVisualStudio2008Theme object.
	//-------------------------------------------------------------------------
	CXTPDockingPaneVisualStudio2008Theme();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to refresh the visual metrics of manager.
	//-----------------------------------------------------------------------
	virtual void RefreshMetrics();

protected:
};
