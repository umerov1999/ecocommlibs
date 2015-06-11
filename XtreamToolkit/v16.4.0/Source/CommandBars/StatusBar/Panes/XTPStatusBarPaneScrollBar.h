//===========================================================================
// Summary:
//     CXTPStatusBarScrollBarPane is a CXTPStatusBarPane derived class. CXTPStatusBarScrollBarPane works with
//     CXTStatusBar and allows you to create themed ScrollBar in your status
//     bar area.
//===========================================================================
class _XTP_EXT_CLASS CXTPStatusBarScrollBarPane : public CXTPStatusBarPane, public CXTPScrollBase
{
	DECLARE_DYNAMIC(CXTPStatusBarScrollBarPane)

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPStatusBarScrollBarPane object.
	//-----------------------------------------------------------------------
	CXTPStatusBarScrollBarPane();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPStatusBarScrollBarPane object, handles cleanup and
	//     deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTPStatusBarScrollBarPane();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set position of scrollbar.
	// Parameters:
	//     nValue    - Scrollbar position to be set.
	//-----------------------------------------------------------------------
	void SetPos(int nValue);

	//-----------------------------------------------------------------------
	// Input:   nMin - Lower limit of the scrollbar control's range.
	//          nMax - Upper limit of the scrollbar control's range.
	// Summary: Sets the rangle of the scroll bar.
	//-----------------------------------------------------------------------
	void SetRange(int nMin, int nMax);

	//-----------------------------------------------------------------------
	// Summary: Gets the current position of the scroll bar, whose return value
	//          is always between the values for the Max and Min properties, inclusive.
	// Returns: Current position of the scroll bar.
	//-----------------------------------------------------------------------
	int GetPos() const;

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set tooltip for scroll part of scrollbar.
	// Parameters:
	//     nPart    - Part of scrollbar to set tooltip. Can be XTP_HTSCROLLUP, XTP_HTSCROLLDOWN, XTP_HTSCROLLUPPAGE, XTP_HTSCROLLDOWNPAGE, XTP_HTSCROLLTHUMB
	//     lpszTooltip - Text of tooltip to set
	//-----------------------------------------------------------------------
	void SetTooltipPart(int nPart, LPCTSTR lpszTooltip);

protected:
//{{AFX_CODEJOCK_PRIVATE
	virtual CRect GetScrollBarRect();
	virtual void GetScrollInfo(SCROLLINFO* psi);
	virtual void DoScroll(int cmd, int pos);
	virtual void RedrawScrollBar();
	virtual CXTPScrollBarPaintManager* GetScrollBarPaintManager() const;
	virtual BOOL IsScrollBarEnabled() const;
	virtual CWnd* GetParentWindow() const;
	virtual INT_PTR CXTPStatusBarScrollBarPane::OnToolHitTest(CPoint point, TOOLINFO* pTI) const;

	virtual void OnDraw(CDC* pDC, CRect rcItem);
	virtual void OnMouseMove(CPoint point);
	virtual void OnLButtonDown(CPoint point);
//}}AFX_CODEJOCK_PRIVATE


protected:
	int m_nMin;             // Lower limit of the scrollbar control's range.
	int m_nMax;             // Upper limit of the scrollbar control's range.
	int m_nValue;           // Scrollbar position.
	int m_nPage;            // Scrollbar Page.
	CString m_strToolTipPart[5]; // Text of tooltip.
};
