class CXTPPaintManager;
class CXTPStatusBarPane;

class _XTP_EXT_CLASS CXTPStatusBarThemeOfficeXP : public CXTPStatusBarPaintManager
{
public:
	CXTPStatusBarThemeOfficeXP(CXTPPaintManager *pPaintManager);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw frame of single cell of status bar.
	// Parameters:
	//     pDC - Points to a valid device context.
	//     rc - CRect object specifying size of area.
	//     pPane - The status bar pane need to draw
	//     bGripperPane - TRUE if pane is last cell of status bar
	//-----------------------------------------------------------------------
	virtual void DrawStatusBarPaneBorder(CDC* pDC, CRect rc, CXTPStatusBarPane* pPane, BOOL bGripperPane);

	//-----------------------------------------------------------------------
	// Input:   pDC - Pointer to a valid device context.
	//          rc - CRect object specifying size of area.
	//          pPane - The status bar pane need to draw.
	// Summary: This method is called to draw a status bar switch pane separator.
	//-----------------------------------------------------------------------
	virtual void DrawStatusBarPaneSeparator(CDC* pDC, CRect rc, CXTPStatusBarPane* pPane);
};
