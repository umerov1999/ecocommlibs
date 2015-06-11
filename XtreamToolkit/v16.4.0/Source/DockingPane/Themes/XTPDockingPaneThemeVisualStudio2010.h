//===========================================================================
// Summary:
//     CXTPDockingPaneVisualStudio2010Theme is CXTPDockingPaneVisualStudio2008Theme
//     derived class, represents Visual Studio 2010 Beta 1 theme for docking
//     panes.
// Remarks:
//     Call CXTPDockingPaneManager::SetTheme(xtpPaneThemeVisualStudio2010); to
//     set this theme.
//===========================================================================
class _XTP_EXT_CLASS CXTPDockingPaneVisualStudio2010Theme : public CXTPDockingPaneVisualStudio2008Theme
{
public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPDockingPaneVisualStudio2010Theme object.
	//-------------------------------------------------------------------------
	CXTPDockingPaneVisualStudio2010Theme();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to refresh the visual metrics of manager.
	//-----------------------------------------------------------------------
	virtual void RefreshMetrics();

protected:
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

	void DrawFloatingFrame(CDC* pDC, CXTPDockingPaneMiniWnd* pPane, CRect rc);

	virtual void DrawCaptionButtonEntry(CDC* pDC, CXTPDockingPaneCaptionButton* pButton, CPoint pt, COLORREF clrButton);
};
