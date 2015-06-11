class CXTPPaintManager;

//-------------------------------------------------------------------------
// Summary: Paint manager for the slider control.
//-------------------------------------------------------------------------
class _XTP_EXT_CLASS CXTPSliderPaintManager : public CXTPScrollBarPaintManager
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPSliderPaintManager object
	// Parameters:
	//     pPaintManager - PaintManager of CommandBars
	//-----------------------------------------------------------------------
	CXTPSliderPaintManager(CXTPPaintManager* pPaintManager);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw scrollbar of the slider
	// Parameters:
	//     pDC - Pointer to device context
	//     pScrollBar - ScrollBar to draw
	//-----------------------------------------------------------------------
	virtual void DrawScrollBar(CDC *pDC, CXTPScrollBase *pScrollBar);

	//-------------------------------------------------------------------------
	// Summary:
	//     Recalculates gallery metrics
	//-------------------------------------------------------------------------
	virtual void RefreshMetrics();

public:

	CXTPPaintManager *m_pPaintManager;
};
