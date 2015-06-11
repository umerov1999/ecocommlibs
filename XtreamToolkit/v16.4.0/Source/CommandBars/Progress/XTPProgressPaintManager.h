class CXTPProgressBase;
class CXTPPaintManager;

//===========================================================================
// Summary: CXTPProgressPaintManager object
//===========================================================================
class _XTP_EXT_CLASS CXTPProgressPaintManager
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPProgressPaintManager object
	// Parameters:
	//     pPaintManager - PaintManager of commandbars
	//-----------------------------------------------------------------------
	CXTPProgressPaintManager(CXTPPaintManager* pPaintManager);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPProgressPaintManager object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTPProgressPaintManager();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw progress
	// Parameters:
	//     pDC - Pointer to device context
	//     pProgressBar - Progress to draw
	//-----------------------------------------------------------------------
	virtual void DrawProgress(CDC* pDC, CXTPProgressBase* pProgressBar);

	//-------------------------------------------------------------------------
	// Summary:
	//     Recalculates metrics
	//-------------------------------------------------------------------------
	virtual void RefreshMetrics();

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns parent paintmanager object
	//-----------------------------------------------------------------------
	CXTPPaintManager* GetPaintManager() const;

protected:
	CXTPPaintManager* m_pPaintManager;          // Pointer to parent PaintManager

public:
	int m_cyProgress;           // Progress height
};

AFX_INLINE CXTPPaintManager* CXTPProgressPaintManager::GetPaintManager() const
{
	return m_pPaintManager;
}
