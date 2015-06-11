//===========================================================================
// Summary:
//     CXTPPropExchangeSection is standalone class. It is used to delete memory allocated by CXTPPropExchange::GetSection.
// Example:
// <code>
// CXTPPropExchangeSection pxTaskPanel(px.GetSection(_T("TaskPanel")));
// </code>
//===========================================================================
class _XTP_EXT_CLASS CXTPPropExchangeSection
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs CXTPPropExchangeSection a object.
	// Parameters:
	//     pPX -  pointer to the CXTPPropExchange object that is the ultimate source or destination of the persistent data.
	//-----------------------------------------------------------------------
	explicit CXTPPropExchangeSection(CXTPPropExchange *pPX);

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs CXTPPropExchangeSection object and automatically assign m_pPX .
	// Parameters:
	//     bLoading - TRUE to load
	//     lpszSection - Section of registry
	//-----------------------------------------------------------------------
	CXTPPropExchangeSection(BOOL bLoading, LPCTSTR lpszSection);

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPPropExchangeSection object, handles cleanup and de-
	//     allocation.
	//-------------------------------------------------------------------------
	virtual ~CXTPPropExchangeSection();

	CXTPPropExchange& operator* () const;
	CXTPPropExchange* operator->() const;
	CXTPPropExchange* operator& () const;

private:
	CXTPPropExchange *m_pPX;
};
