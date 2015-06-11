class CXTPScrollBarCtrl;

class _XTP_EXT_CLASS CXTPWnd : public CWnd
{
	DECLARE_DYNAMIC(CXTPWnd)

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPWnd object
	//-----------------------------------------------------------------------
	CXTPWnd();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPWnd object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPWnd();


public:
	//{{AFX_VIRTUAL(CXTPWnd)
	virtual CScrollBar* GetScrollBarCtrl(int nBar) const;
	virtual BOOL PreCreateWindow(CREATESTRUCT &cs);
	virtual void PreSubclassWindow();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

	DECLARE_MESSAGE_MAP()
	//{{AFX_MSG(CXTPWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS *lpncsp);
	//}}AFX_MSG

protected:

	void SetScrollBarTheme(XTPScrollBarTheme theme);

	BOOL m_bEnableThemedScrollBar;

	CXTPScrollBarCtrl *m_pwndScrollBar[2];
};
