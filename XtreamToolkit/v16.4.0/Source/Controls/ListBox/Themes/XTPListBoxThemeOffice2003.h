class _XTP_EXT_CLASS CXTPListBoxThemeOffice2003 : public CXTPListBoxThemeOfficeXP
{
public:
	virtual void DrawItem(CDC *pDC, CXTPListBox *pListBox, LPDRAWITEMSTRUCT lpDIS);

	virtual void RefreshMetrics();
};
