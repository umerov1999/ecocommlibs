class _XTP_EXT_CLASS CXTPMessageBarThemeOffice2003 : public CXTPMessageBarPaintManager
{
public:
	CXTPMessageBarThemeOffice2003(CXTPCommandBarsOffice2003Theme *pTheme);

	//-----------------------------------------------------------------------
	// Parameters:   pDC - Pointer to a valid device context.
	//          pBar - Pointer to the message bar to draw.
	// Summary: Draws the message bar.
	//-----------------------------------------------------------------------
	virtual void FillMessageBar(CDC* pDC, CXTPMessageBar* pBar);

protected:

	CXTPCommandBarsOffice2003Theme *m_pTheme;
};
