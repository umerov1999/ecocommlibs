class _XTP_EXT_CLASS CXTPDockingPaneTabColorSetVisualStudio2012 : public CXTPTabPaintManagerColorSet
{
public:
	CXTPDockingPaneTabColorSetVisualStudio2012();

	virtual void RefreshMetrics();

	virtual COLORREF GetItemColor(CXTPTabManagerItem *pItem);

	virtual void FillNavigateButton(CDC *pDC, CXTPTabManagerNavigateButton *pButton, CRect &rc);
};



class _XTP_EXT_CLASS CXTPDockingPaneTabColorSetVisualStudio2012Light : public CXTPDockingPaneTabColorSetVisualStudio2012
{
public:
	virtual void RefreshMetrics();
};


class _XTP_EXT_CLASS CXTPDockingPaneTabColorSetVisualStudio2012Dark : public CXTPDockingPaneTabColorSetVisualStudio2012
{
public:
	virtual void RefreshMetrics();
};
