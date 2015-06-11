class CXTPCommandBarsOffice2003Theme;

class _XTP_EXT_CLASS CXTPStatusBarThemeOffice2003 : public CXTPStatusBarPaintManager
{
public:
	CXTPStatusBarThemeOffice2003(CXTPCommandBarsOffice2003Theme *pTheme);

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
	virtual void DrawStatusBarPaneBorder(CDC* pDC, CRect rc, CXTPStatusBarPane* pPane, BOOL bGripperPane);

protected:

	CXTPCommandBarsOffice2003Theme *m_pTheme;
};
