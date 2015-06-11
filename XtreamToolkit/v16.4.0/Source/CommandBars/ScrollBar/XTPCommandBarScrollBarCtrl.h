//===========================================================================
// Summary:
//     CXTPCommandBarScrollBarCtrl is a CScrollBar derived class.
//===========================================================================
class _XTP_EXT_CLASS CXTPCommandBarScrollBarCtrl : public CXTPScrollBarCtrl
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPCommandBarScrollBarCtrl object
	//-----------------------------------------------------------------------
	CXTPCommandBarScrollBarCtrl();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPCommandBarScrollBarCtrl object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTPCommandBarScrollBarCtrl();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to set the command bar's object.
	// Parameters:
	//     pCommandBars - CXTPCommandBars to be set.
	//-----------------------------------------------------------------------
	void SetCommandBars(CXTPCommandBars* pCommandBars);

protected:

	virtual CXTPScrollBarPaintManager* GetScrollBarPaintManager() const;

protected:

	DECLARE_MESSAGE_MAP()
	//{{AFX_MSG(CXTPCommandBarScrollBarCtrl)
	//}}AFX_MSG

protected:
	CXTPCommandBars *m_pCommandBars;            // Parent CommandBars object
};


AFX_INLINE void CXTPCommandBarScrollBarCtrl::SetCommandBars(CXTPCommandBars *pCommandBars)
{
	m_pCommandBars = pCommandBars;
}
