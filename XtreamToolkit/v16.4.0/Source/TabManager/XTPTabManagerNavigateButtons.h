//===========================================================================
// Summary:
//     Navigate buttons array definition
//===========================================================================
class _XTP_EXT_CLASS CXTPTabManagerNavigateButtons
{

public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPTabManagerNavigateButtons object.
	//-------------------------------------------------------------------------
	CXTPTabManagerNavigateButtons();

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPTabManagerNavigateButtons object, handles cleanup and deallocation.
	//-------------------------------------------------------------------------
	virtual ~CXTPTabManagerNavigateButtons();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves CXTPTabManagerNavigateButton by its index.
	// Parameters:
	//     nIndex - Zero-based index of the button.
	// Returns:
	//     Pointer to CXTPTabManagerNavigateButton if successful; NULL otherwise.
	//-----------------------------------------------------------------------
	CXTPTabManagerNavigateButton* GetAt(int nIndex) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves CXTPTabManagerNavigateButton by its index.
	// Parameters:
	//     nIndex - Zero-based index of the button.
	// Returns:
	//     Pointer to CXTPTabManagerNavigateButton if successful; NULL otherwise.
	//-----------------------------------------------------------------------
	CXTPTabManagerNavigateButton* operator[] (int nIndex) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Adds CXTPTabManagerNavigateButton to collection
	// Parameters:
	//     pButton - New button to be added
	// Returns:
	//     Index of button in collection
	//-----------------------------------------------------------------------
	int Add(CXTPTabManagerNavigateButton* pButton);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to insert CXTPTabManagerNavigateButton to collection
	// Parameters:
	//     nIndex - Index to insert button to
	//     pButton - New button to be added
	//-----------------------------------------------------------------------
	void InsertAt(int nIndex, CXTPTabManagerNavigateButton* pButton);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to remove CXTPTabManagerNavigateButton from collection
	// Parameters:
	//     nIndex - Index of button to remove
	//-----------------------------------------------------------------------
	void RemoveAt(int nIndex);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to clean all buttons
	//-----------------------------------------------------------------------
	void RemoveAll();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to get size of collection
	// Returns:
	//     Number of elements in collection
	//-----------------------------------------------------------------------
	int GetSize() const;


protected:
	CArray<CXTPTabManagerNavigateButton*, CXTPTabManagerNavigateButton*> m_arrButtons;  /// Collection of buttons
};

AFX_INLINE CXTPTabManagerNavigateButton* CXTPTabManagerNavigateButtons::GetAt(int nIndex) const
{
	return m_arrButtons.GetAt(nIndex);
}

AFX_INLINE int CXTPTabManagerNavigateButtons::Add(CXTPTabManagerNavigateButton *pButton)
{
	return (int)m_arrButtons.Add(pButton);
}

AFX_INLINE int CXTPTabManagerNavigateButtons::GetSize() const
{
	return (int)m_arrButtons.GetSize();
}

AFX_INLINE void CXTPTabManagerNavigateButtons::InsertAt(int nIndex, CXTPTabManagerNavigateButton *pButton)
{
	m_arrButtons.InsertAt(nIndex, pButton);
}

AFX_INLINE CXTPTabManagerNavigateButton* CXTPTabManagerNavigateButtons::operator[] (int nIndex) const
{
	return m_arrButtons.GetAt(nIndex);
}
