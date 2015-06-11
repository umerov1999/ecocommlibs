class _XTP_EXT_CLASS CXTPStatusBarThemeOffice2013 : public CXTPStatusBarPaintManager
{
public:
	CXTPStatusBarThemeOffice2013(CXTPPaintManager *pPaintManager);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to fill a status bar.
	// Parameters:
	//     pDC  - Pointer to a valid device context
	//     pBar - Points to a CXTPStatusBar object
	//-----------------------------------------------------------------------
	virtual void FillStatusBar(CDC *pDC, CXTPStatusBar *pBar);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the status bar's gripper.
	// Parameters:
	//     pDC      - Pointer to a valid device context
	//     rcClient - Client rectangle of the status bar.
	//-----------------------------------------------------------------------
	virtual void DrawStatusBarGripper(CDC *pDC, CRect rcClient);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw frame of single cell of status bar.
	// Parameters:
	//     pDC - Points to a valid device context.
	//     rc - CRect object specifying size of area.
	//     pPane - The status bar pane need to draw
	//     bGripperPane - TRUE if pane is last cell of status bar
	//-----------------------------------------------------------------------
	virtual void DrawStatusBarPaneBorder(CDC *pDC, CRect rc, CXTPStatusBarPane *pPane, BOOL bGripperPane);

	//-----------------------------------------------------------------------
	// Input:   pDC - Pointer to a valid device context.
	//          rc - CRect object specifying size of area.
	//          pPane - The status bar pane need to draw.
	// Summary: This method is called to draw a status bar switch pane separator.
	//-----------------------------------------------------------------------
	virtual void DrawStatusBarPaneSeparator(CDC *pDC, CRect rc, CXTPStatusBarPane *pPane);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw a single pane text of the status bar.
	// Parameters:
	//     pDC     - Pointer to a valid device context.
	//     pPane - The status bar pane need to draw.
	//     rcItem - Item rectangle.
	//-----------------------------------------------------------------------
	virtual void DrawStatusBarPaneEntry(CDC *pDC, CRect rcItem, CXTPStatusBarPane *pPane);

	//-----------------------------------------------------------------------
	// Input:   pDC - Pointer to a valid device context.
	//          rcItem - Item rectangle.
	//          pPane - The status bar pane need to draw.
	// Summary: This method is called to draw a status bar button.
	//-----------------------------------------------------------------------
	virtual void DrawStatusBarButtonFace(CDC *pDC, CRect rcItem, CXTPStatusBarPane *pPane);

	//-----------------------------------------------------------------------
	// Input:   pDC - Pointer to a valid device context.
	//          rcItem - Item rectangle.
	//          pPane - The status bar pane need to draw.
	// Summary: This method is called to draw a status bar switch pane.
	// Returns: Returns width of switch pane
	//-----------------------------------------------------------------------
	virtual int DrawStatusBarSwitchPane(CDC *pDC, CRect rcItem, CXTPStatusBarSwitchPane *pPane);

	virtual COLORREF GetTextColor(CXTPStatusBarPane *pPane);

protected:

	COLORREF m_clrButtonNormal;
	COLORREF m_clrButtonChecked;
	COLORREF m_clrButtonHighlighted;
	COLORREF m_clrButtonPressed;
};
