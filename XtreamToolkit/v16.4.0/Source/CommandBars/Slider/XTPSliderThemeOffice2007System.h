class _XTP_EXT_CLASS CXTPSliderThemeOffice2007System : public CXTPSliderPaintManager
{
public:
	CXTPSliderThemeOffice2007System(CXTPPaintManager *pPaintManager);

public:
	virtual void RefreshMetrics();
	virtual void DrawScrollBar(CDC *pDC, CXTPScrollBase *pScrollBar);
};
