class _XTP_EXT_CLASS CXTPListBoxThemeOffice2013 : public CXTPListBoxTheme
{
	struct CXTPListBoxThemeOffice2013Colors
	{
		CXTPPaintManagerColor m_clrBackground;
		CXTPPaintManagerColor m_clrText;
	};

public:
	CXTPListBoxThemeOffice2013();

	virtual void DrawItem(CDC *pDC, CXTPListBox *pListBox, LPDRAWITEMSTRUCT lpDIS);

	virtual void RefreshMetrics();

public:

	CXTPListBoxThemeOffice2013Colors m_colorNormal;
	CXTPListBoxThemeOffice2013Colors m_colorHot;
	CXTPListBoxThemeOffice2013Colors m_colorSelected;
};
