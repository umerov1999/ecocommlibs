//===========================================================================
// Summary:
//     CXTPDockingPaneVisualStudio6Theme is CXTPDockingPaneGripperedTheme derived class, represents
//     Visual Studio 6 theme for docking panes.
// Remarks:
//     Call CXTPDockingPaneManager::SetTheme(xtpPaneThemeVisualStudio6); to set this theme.
//===========================================================================
class _XTP_EXT_CLASS CXTPDockingPaneVisualStudio6Theme : public CXTPDockingPaneGripperedTheme
{
public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPDockingPaneVisualStudio6Theme object.
	//-------------------------------------------------------------------------
	CXTPDockingPaneVisualStudio6Theme();

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
