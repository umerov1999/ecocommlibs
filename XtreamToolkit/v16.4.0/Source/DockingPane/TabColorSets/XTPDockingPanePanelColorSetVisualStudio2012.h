class _XTP_EXT_CLASS CXTPDockingPanePanelColorSetVisualStudio2012 : public CXTPTabPaintManagerColorSet
{
public:
	CXTPDockingPanePanelColorSetVisualStudio2012();

	virtual void RefreshMetrics();

	virtual COLORREF GetItemColor(CXTPTabManagerItem *pItem);

	virtual void FillNavigateButton(CDC *pDC, CXTPTabManagerNavigateButton *pButton, CRect &rc);
};



class _XTP_EXT_CLASS CXTPDockingPanePanelColorSetVisualStudio2012Light : public CXTPDockingPanePanelColorSetVisualStudio2012
{
public:
	virtual void RefreshMetrics();
};


class _XTP_EXT_CLASS CXTPDockingPanePanelColorSetVisualStudio2012Dark : public CXTPDockingPanePanelColorSetVisualStudio2012
{
public:
	virtual void RefreshMetrics();
};
