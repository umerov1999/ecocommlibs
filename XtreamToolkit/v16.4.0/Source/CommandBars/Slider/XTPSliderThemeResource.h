class _XTP_EXT_CLASS CXTPSliderResourceTheme : public CXTPSliderPaintManager
{
public:
	CXTPSliderResourceTheme(CXTPPaintManager* pPaintManager)
		: CXTPSliderPaintManager(pPaintManager)
	{

	}

public:
	virtual void DrawScrollBar(CDC* pDC, CXTPScrollBase* pGallery);
	virtual void RefreshMetrics();
};
