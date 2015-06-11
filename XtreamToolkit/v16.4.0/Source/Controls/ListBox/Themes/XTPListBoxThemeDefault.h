class _XTP_EXT_CLASS CXTPListBoxThemeDefault : public CXTPListBoxTheme
{
public:
	virtual void DrawItem(CDC *pDC, CXTPListBox *pListBox, LPDRAWITEMSTRUCT lpDIS);

	virtual void RefreshMetrics();
};
