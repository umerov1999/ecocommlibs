class _XTP_EXT_CLASS CXTPSliderThemeOffice2013 : public CXTPSliderPaintManager
{
public:
	CXTPSliderThemeOffice2013(CXTPPaintManager *pPaintManager);

public:
	virtual void RefreshMetrics();
	virtual void DrawScrollBar(CDC *pDC, CXTPScrollBase *pScrollBar);
};
