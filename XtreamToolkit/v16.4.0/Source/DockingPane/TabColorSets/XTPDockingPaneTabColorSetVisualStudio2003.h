class CXTPDockingPaneTabColorSetVisualStudio2003 : public CXTPTabColorSetOffice2003
{
public:
	CXTPDockingPaneTabColorSetVisualStudio2003(BOOL bHighlightSelected);

	void RefreshMetrics();

public:
	void XTPFillFramePartRect(CDC* pDC, CRect rc, CWnd* pWnd, CWnd* pSite);

	void FillHeader(CDC* pDC, CRect rc, CXTPTabManager* pTabManager);

	COLORREF FillPropertyButton(CDC* pDC, CRect rcItem, CXTPTabManagerItem* pItem);

protected:
	BOOL m_bGradientButton;
	BOOL m_bTransparentButton;
	BOOL m_bHighlightSelected;
};
