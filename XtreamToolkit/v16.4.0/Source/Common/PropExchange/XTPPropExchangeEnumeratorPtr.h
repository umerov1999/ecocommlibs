class _XTP_EXT_CLASS CXTPPropExchangeEnumeratorPtr
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs CXTPPropExchangeEnumeratorPtr a object.
	// Parameters:
	//     pPX -  pointer to the CXTPPropExchangeEnumerator object that is the ultimate source or destination of the persistent data.
	//-----------------------------------------------------------------------
	explicit CXTPPropExchangeEnumeratorPtr(CXTPPropExchangeEnumerator *pPX);

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPPropExchangeEnumeratorPtr object, handles cleanup and de-
	//     allocation.
	//-------------------------------------------------------------------------
	virtual ~CXTPPropExchangeEnumeratorPtr();

	CXTPPropExchangeEnumerator& operator* () const;
	CXTPPropExchangeEnumerator* operator->() const;
	CXTPPropExchangeEnumerator* operator& () const;

private:
	CXTPPropExchangeEnumerator *m_pPX;
};
