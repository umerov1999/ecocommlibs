class _XTP_EXT_CLASS CXTPDockingPaneTabThemeVisualStudio2012 : public CXTPTabThemeFlat
{
public:

	CXTPDockingPaneTabThemeVisualStudio2012();

	virtual void RefreshMetrics();

	virtual void DrawText(CDC *pDC, CXTPTabManager *pManager, const CString& str, LPRECT lpRect, UINT nFormat);

	virtual void DrawSingleButton(CDC *pDC, CXTPTabManagerItem *pItem);
};
