class _XTP_EXT_CLASS CXTPProgressThemeWindowsUx : public CXTPProgressPaintManager
{
public:
	CXTPProgressThemeWindowsUx(CXTPPaintManager *pPaintManager);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPProgressThemeWindowsUx object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTPProgressThemeWindowsUx();

public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Recalculates metrics
	//-------------------------------------------------------------------------
	virtual void RefreshMetrics();

	virtual void DrawProgress(CDC *pDC, CXTPProgressBase *pGallery);

protected:
	CXTPWinThemeWrapper *m_themeProgress; // Theme wrapper
};
