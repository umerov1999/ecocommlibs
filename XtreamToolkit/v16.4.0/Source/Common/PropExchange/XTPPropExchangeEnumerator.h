class CXTPPropExchange;

//===========================================================================
// Summary:
//     CXTPPropExchangeEnumerator is standalone class. It used by CXTPPropExchange to enumerate child sections.
//
// Example:
// <code>
// extern CXTPPropExchange* pPX;
//
// CXTPPropExchangeEnumeratorPtr pEnumerator(pPX->GetEnumerator(_T("Icon")));
// POSITION posEnum = pEnumerator->GetPosition();
//
// while (posEnum)
// {
//     CXTPPropExchangeSection secIcon(pEnumerator->GetNext(posEnum));
// }
// </code>
// See Also: CXTPPropExchange, CXTPPropExchange::GetEnumerator
//===========================================================================
class _XTP_EXT_CLASS CXTPPropExchangeEnumerator
{
public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPropExchangeEnumerator object.
	// Parameters:
	//     pContainer - Parent section of enumerator
	//     lpszSection - Enumerator section name
	//-------------------------------------------------------------------------
	CXTPPropExchangeEnumerator(CXTPPropExchange* pContainer, LPCTSTR lpszSection);

	//-------------------------------------------------------------------------
	//     Destroys a CXTPPropExchangeEnumerator object, handles cleanup and de-
	//     allocation.
	//-------------------------------------------------------------------------
	virtual ~CXTPPropExchangeEnumerator();

public:

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this method to init enumerable section
	// Parameters:
	//     dwCount - Count of items to be stored.
	//     bCompactMode - TRUE to use "compact mode" of XML nodes.
	// Returns:
	//     A POSITION value that can be used for iteration; NULL if the section is empty.
	//-------------------------------------------------------------------------
	virtual POSITION GetPosition(DWORD dwCount = 0, BOOL bCompactMode = TRUE);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this method to get next enumerable section to load.
	// Parameters:
	//     pos - A reference to a POSITION value returned by a previous GetNext, GetPosition, or other member function call.
	//-------------------------------------------------------------------------
	virtual CXTPPropExchange* GetNext(POSITION& pos);

	CXTPPropExchange* GetContainer() const;
	virtual int GetCount() const;

protected:
	CXTPPropExchange*  m_pContainer;    // Parent section of enumerator
	CString m_strSectionName;           // Enumerator section name
	DWORD m_nCount;                     // Count of items to load.
};


AFX_INLINE CXTPPropExchange* CXTPPropExchangeEnumerator::GetContainer() const
{
	return m_pContainer;
}

AFX_INLINE int CXTPPropExchangeEnumerator::GetCount() const
{
	return m_nCount;
}
