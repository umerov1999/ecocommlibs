//===========================================================================
// Summary:
//     CXTPDockingPaneVisualStudio2003Theme is CXTPDockingPaneGripperedTheme derived class, represents
//     Office XP theme for docking panes.
// Remarks:
//     Call CXTPDockingPaneManager::SetTheme(xtpPaneThemeVisualStudio2003); to set this theme.
//===========================================================================
class _XTP_EXT_CLASS CXTPDockingPaneVisualStudio2003Theme : public CXTPDockingPaneGripperedTheme
{
public:

	//-------------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPDockingPaneVisualStudio2003Theme object.
	//-------------------------------------------------------------------------
	CXTPDockingPaneVisualStudio2003Theme();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to refresh the visual metrics of the pane.
	//-----------------------------------------------------------------------
	virtual void RefreshMetrics();

protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     Draw common part of tabbed and floating caption.
	// Parameters:
	//     pDC        - Reference to the device context in which to draw.
	//     pPane     - Container which caption need to draw.
	//     rcCaption - Caption bounding rectangle
	//     strTitle  - Caption text.
	//     bActive   - TRUE if caption is active.
	//     bVertical - TRUE to draw caption vertically
	//-----------------------------------------------------------------------
	virtual void DrawCaptionPart(CDC* pDC, CXTPDockingPaneBase* pPane, CRect rcCaption, const CString& strTitle, BOOL bActive, BOOL bVertical);

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
	BOOL m_bGradientCaption;                            // TRUE to draw gradient caption.
	int m_nCaptionButtonSize;
};
