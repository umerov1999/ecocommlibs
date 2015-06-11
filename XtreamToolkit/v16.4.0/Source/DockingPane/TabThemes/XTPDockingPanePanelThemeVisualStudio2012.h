class _XTP_EXT_CLASS CXTPDockingPanePanelThemeVisualStudio2012 : public CXTPTabThemeFlat
{
public:

	CXTPDockingPanePanelThemeVisualStudio2012();

	virtual void RefreshMetrics();

	virtual void DrawText(CDC *pDC, CXTPTabManager *pManager, const CString& str, LPRECT lpRect, UINT nFormat);

	virtual void DrawSingleButton(CDC *pDC, CXTPTabManagerItem *pItem);

protected:

	int m_nGripperWidth;
};
