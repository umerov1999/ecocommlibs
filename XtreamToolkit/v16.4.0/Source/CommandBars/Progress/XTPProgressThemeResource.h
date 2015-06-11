class _XTP_EXT_CLASS CXTPProgressResourceTheme : public CXTPProgressPaintManager
{
public:
	CXTPProgressResourceTheme(CXTPPaintManager* pPaintManager);

public:
	virtual void DrawProgress(CDC* pDC, CXTPProgressBase* pGallery);
};
