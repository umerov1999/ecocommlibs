//===========================================================================
// Summary:
//     CXTPDockingPaneVisualStudio2005Beta2Theme is CXTPDockingPaneVisualStudio2005Beta1Theme derived class, represents
//     Visual Studio 2005 beta 2 theme for docking panes.
// Remarks:
//     Call CXTPDockingPaneManager::SetTheme(xtpPaneThemeVisualStudio2005Beta1); to set this theme.
//===========================================================================
class _XTP_EXT_CLASS CXTPDockingPaneVisualStudio2005Beta2Theme : public CXTPDockingPaneVisualStudio2005Beta1Theme
{
public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPDockingPaneVisualStudio2005Beta2Theme object.
	//-------------------------------------------------------------------------
	CXTPDockingPaneVisualStudio2005Beta2Theme();

	//-------------------------------------------------------------------------
	// Summary:
	//     This method is called to refresh the visual metrics of the pane.
	//-------------------------------------------------------------------------
	void RefreshMetrics();

	//-----------------------------------------------------------------------
	// Summary:
	//     Draws the background of the pane caption.
	// Parameters:
	//     pDC        - Reference to the device context in which to draw.
	//     pPane     - Container of the caption needs to draw the background.
	//     rcCaption - Caption bounding rectangle
	//     strTitle  - Caption text.
	//     bActive   - TRUE if caption is active.
	//     bVertical - TRUE to draw caption vertically
	//-----------------------------------------------------------------------
	virtual COLORREF FillCaptionPart(CDC* pDC, CXTPDockingPaneBase* pPane, CRect rcCaption, BOOL bActive, BOOL bVertical);

protected:
	COLORREF m_clrCaptionBorder;    //Color used to draw the border of the pane caption.
};
