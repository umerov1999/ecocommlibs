//===========================================================================
// Summary:
//     CXTPStatusBarSliderPane is a CXTPStatusBarPane derived class. CXTPStatusBarSliderPane works with
//     CXTStatusBar and allows you to create themed Slider in your status
//     bar area.
//===========================================================================
class _XTP_EXT_CLASS CXTPStatusBarSliderPane : public CXTPStatusBarScrollBarPane
{
	DECLARE_DYNAMIC(CXTPStatusBarSliderPane)

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPStatusBarSliderPane object.
	//-----------------------------------------------------------------------
	CXTPStatusBarSliderPane();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPStatusBarSliderPane object, handles cleanup and
	//     deallocation
	//-----------------------------------------------------------------------
	~CXTPStatusBarSliderPane();

protected:

	//----------------------------------------------------------------------
	// Summary:
	//     This method is called to retrieve pointer to CXTPScrollBarPaintManager.
	//----------------------------------------------------------------------
	virtual CXTPScrollBarPaintManager* GetScrollBarPaintManager() const;

public:

	//-----------------------------------------------------------------------
	// Summary: Call this method to set ticks for slider
	// Input:   pTicks - Pointer to double array of ticks
	//          nCount - Count of elements in array.
	//-----------------------------------------------------------------------
	void SetTicks(double* pTicks, int nCount);

#ifdef _XTP_ACTIVEX
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPStatusBarSliderPane);
	void OleSetTooltipPart(int nPart, LPCTSTR lpszTooltip);
	void OleSetTicks(const VARIANT& Ticks);
//}}AFX_CODEJOCK_PRIVATE
#endif
};
