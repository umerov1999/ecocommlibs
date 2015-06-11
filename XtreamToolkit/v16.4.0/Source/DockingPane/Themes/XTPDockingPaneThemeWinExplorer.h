//===========================================================================
// Summary:
//     CXTPDockingPaneWinExplorerTheme is CXTPDockingPaneDefaultTheme derived class, represents
//     explorer theme for docking panes.
// Remarks:
//     Call CXTPDockingPaneManager::SetTheme(xtpPaneThemeWinExplorer); to set this theme.
//===========================================================================
class _XTP_EXT_CLASS CXTPDockingPaneWinExplorerTheme : public CXTPDockingPaneGripperedTheme
{
public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPDockingPaneWinExplorerTheme object.
	//-------------------------------------------------------------------------
	CXTPDockingPaneWinExplorerTheme();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to refresh the visual metrics of the pane.
	//-----------------------------------------------------------------------
	void RefreshMetrics();

protected:

	// ------------------------------------------------------------------
	// Summary:
	//     Draw common part of tabbed and floating caption.
	// Parameters:
	//     pDC :        Reference to the device context in which to draw.
	//     pPane :      Container which caption need to draw.
	//     rcCaption :  Caption bounding rectangle
	//     strTitle :   Caption text.
	//     bActive :    TRUE if caption is active.
	//     bVertical :  TRUE to draw caption vertically
	//
	// ------------------------------------------------------------------
	virtual void DrawCaptionPart(CDC* pDC, CXTPDockingPaneBase* pPane, CRect rcCaption, const CString& strTitle, BOOL bActive, BOOL bVertical);
};
