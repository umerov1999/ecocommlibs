class _XTP_EXT_CLASS CXTPListBoxThemeResource : public CXTPListBoxTheme
{
public:
	virtual void DrawItem(CDC *pDC, CXTPListBox *pListBox, LPDRAWITEMSTRUCT lpDIS);

	virtual void RefreshMetrics();
};
