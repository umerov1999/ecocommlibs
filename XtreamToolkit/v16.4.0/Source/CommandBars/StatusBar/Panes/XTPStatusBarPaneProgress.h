//===========================================================================
// Summary:
//     CXTPStatusBarProgressPane is a CXTPStatusBarPane derived class. CXTPStatusBarProgressPane works with
//     CXTStatusBar and allows you to create progress bar in your status bar area.
//===========================================================================
class _XTP_EXT_CLASS CXTPStatusBarProgressPane : public CXTPStatusBarPane, public CXTPProgressBase
{
	DECLARE_DYNAMIC(CXTPStatusBarProgressPane)
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPStatusBarProgressPane object
	//-----------------------------------------------------------------------
	CXTPStatusBarProgressPane();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPStatusBarProgressPane object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	~CXTPStatusBarProgressPane();

protected:
	//----------------------------------------------------------------------
	// Summary:
	//     This method is called to redraw progress bar.
	//----------------------------------------------------------------------
	virtual void RedrawProgress();

	//----------------------------------------------------------------------
	// Summary:
	//     This method is called to retrieve pointer to CXTPProgressPaintManager.
	//----------------------------------------------------------------------
	virtual CXTPProgressPaintManager* GetProgressPaintManager() const;

	//----------------------------------------------------------------------
	// Summary:
	//     This method is called to get bounding rectangle of progressbar
	//----------------------------------------------------------------------
	virtual CRect GetProgressRect();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw pane item. Override it to draw custom pane.
	// Parameters:
	//     pDC    - Pointer to a valid device context
	//     rcItem - Item rectangle
	//-----------------------------------------------------------------------
	virtual void OnDraw(CDC* pDC, CRect rcItem);

public:
#ifdef _XTP_ACTIVEX
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPStatusBarProgressPane);

	int OleGetPos();
	void OleSetPos(int nPos);
	void OleRedrawProgress();
//}}AFX_CODEJOCK_PRIVATE
#endif
};
