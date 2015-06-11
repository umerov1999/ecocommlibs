//===========================================================================
// Summary:
//     CXTPPropExchangeArchive is CXTPPropExchange derived class.
//     The CXTPPropExchangeArchive class allows you to save a complex network of objects in a permanent binary form (usually disk storage) that persists after those objects are deleted. Later you can load the objects from persistent storage, reconstituting them in memory. This process of making data persistent is called 'serialization.'
// Example:
// <code>
// CFile myFile("myfile", CFile::modeCreate | CFile::modeWrite);
//
// // Create a storing archive.
// CArchive arStore(&myFile, CArchive::store);
// CXTPPropExchangeArchive px(arStore);
// PX_String(&px, _T("Caption"), m_strCaption);
// </code>
//===========================================================================
class _XTP_EXT_CLASS CXTPPropExchangeArchive : public CXTPPropExchange
{
	DECLARE_DYNAMIC(CXTPPropExchangeArchive)
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPropExchangeArchive object.
	// Parameters:
	//     ar - A reference to the CArchive object that is the ultimate source or destination of the persistent data.
	//-----------------------------------------------------------------------
	CXTPPropExchangeArchive(CArchive& ar);

	//-----------------------------------------------------------------------
	// Summary:
	//     Exchanges a property between a storage medium and the control.
	// Parameters:
	//     pszPropName - The name of the property being exchanged.
	//     vtProp      - A symbol specifying the type of the property being
	//                   exchanged.
	//     pvProp      - A pointer to the property's value.
	//     pvDefault   - Pointer to a default value for the property.
	// Remarks:
	//     Possible values of properties are:
	//     <TABLE>
	//         Symbol         Property Type
	//         -------------  -------------
	//         VT_I2          short
	//         VT_I4          long
	//         VT_BOOL        BOOL
	//         VT_BSTR        CString
	//         VT_CY          CY
	//         VT_R4          float
	//         VT_R8          double
	//     </TABLE>
	// Returns:
	//     Nonzero if the exchange was successful; 0 if unsuccessful.
	//-----------------------------------------------------------------------
	virtual BOOL ExchangeProp(LPCTSTR pszPropName, VARTYPE vtProp,
				void* pvProp, const void* pvDefault = NULL);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method serializes a property that stores binary large object (BLOB) data.
	// Parameters:
	//     pszPropName - The name of the property being exchanged.
	//     pData       - A pointer to a user-supplied buffer that contains the data to be serialized
	//     nBytes      - An integer that specifies the number of bytes to be read or written.
	// Returns:
	//     Nonzero if the exchange was successful; 0 if unsuccessful.
	//-----------------------------------------------------------------------
	virtual BOOL ExchangeBlobProp(LPCTSTR pszPropName, BYTE*& pData, DWORD& nBytes);

	//-----------------------------------------------------------------------
	// Summary:
	//     Exchanges a CRuntimeClass between a storage and the class.
	// Parameters:
	//     pszPropName   - The name of the property being exchanged.
	//     pClass        - CRuntimeClass class pointer.
	//     pDefaultClass - Pointer to a default runtime class for the property.
	// Returns:
	//     Nonzero if the exchange was successful; 0 if unsuccessful.
	//-----------------------------------------------------------------------
	virtual BOOL ExchangeRuntimeClass(LPCTSTR pszPropName, CRuntimeClass*& pClass, CRuntimeClass* pDefaultClass);

	//-----------------------------------------------------------------------
	// Summary:
	//     Exchanges a class instance.
	// Parameters:
	//     pObject        - Object being exchanged.
	//     pObjectRuntime - Default RUntime class of object
	// Returns:
	//     Nonzero if the exchange was successful; 0 if unsuccessful.
	//-----------------------------------------------------------------------
	virtual BOOL ExchangeObjectInstance(CObject*& pObject, CRuntimeClass* pObjectRuntime);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method create new Archive to serialize.
	// Parameters:
	//     pszPropName - The name of the property being exchanged.
	// Returns:
	//     CArchive pointer. You must call ReleaseArchive to free memory.
	// See Also: ReleaseArchive
	//-----------------------------------------------------------------------
	virtual CArchive* GetArchive(LPCTSTR pszPropName);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method releases CArchive pointer.
	// Parameters:
	//     pszPropName - The name of the property being exchanged.
	//     pArchive    - Archive pointer was previously returned by GetArchive member.
	// See Also: GetArchive
	//-----------------------------------------------------------------------
	virtual void ReleaseArchive(LPCTSTR pszPropName, CArchive* pArchive);

	//-----------------------------------------------------------------------
	// Summary:
	//     Writes a specified number of bytes.
	// Parameters:
	//     pszPropName - The name of the property being exchanged.
	//     lpBuf       - A pointer to a user-supplied buffer that contains the data to be written.
	//     nCount      - An integer that specifies the number of bytes to be written.
	//-----------------------------------------------------------------------
	virtual void Write (LPCTSTR pszPropName, const void* lpBuf, UINT nCount);

	//-----------------------------------------------------------------------
	// Summary:
	//     Reads a specified number of bytes.
	// Parameters:
	//     pszPropName - The name of the property being exchanged.
	//     lpBuf       - A pointer to a user-supplied buffer that is to receive the data
	//     nCount      - An unsigned integer specifying the number of bytes to be read from the archive.
	// Returns:
	//     An unsigned integer containing the number of bytes actually read.
	//-----------------------------------------------------------------------
	virtual UINT Read (LPCTSTR pszPropName, void* lpBuf, UINT nCount);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to determine if storage supported default values.
	// Returns:
	//     TRUE if storage allows default values
	//-----------------------------------------------------------------------
	virtual BOOL IsAllowDefaultValues() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to determine if storage allows exchange plain blob values.
	// Returns:
	//     TRUE if storage allows exchange plain blob values.
	//-----------------------------------------------------------------------
	virtual BOOL IsAllowBlobValues() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method creates new section for text base storages (CXTPPropExchangeXMLNode and CXTPPropExchangeRegistry)
	// Parameters:
	//     lpszSection - The name of the section.
	// Returns:
	//     CXTPPropExchange pointer to new section. You must delete this pointer or use CXTPPropExchangeSection to manage it.
	// Example:
	// <code>
	// CXTPPropExchangeSection pxTaskPanel(px.GetSection(_T("TaskPanel")));
	// m_wndTaskPanel.GetGroups()->DoPropExchange(&pxTaskPanel);
	// </code>
	//-----------------------------------------------------------------------
	CXTPPropExchange* GetSection(LPCTSTR lpszSection);

	//-----------------------------------------------------------------------
	// Summary:
	//     Use this member function to write DWORD number to storage
	// Parameters:
	//     dwCount - DWORD number
	// See Also: ReadCount
	//-----------------------------------------------------------------------
	virtual void WriteCount(DWORD dwCount);

	//-----------------------------------------------------------------------
	// Summary:
	//     Use this member function to read DWORD number from storage
	// Returns:
	//     DWORD number
	// See Also: WriteCount
	//-----------------------------------------------------------------------
	virtual DWORD ReadCount();

	CArchive& GetSelfArchive();

private:
	CXTPPropExchangeArchive& operator=(const CXTPPropExchangeArchive &) { ASSERT(FALSE); return *this; }

protected:

//{{AFX_CODEJOCK_PRIVATE
	virtual void ExchangeSchemaSafe();

	class CSelfArchive : public CArchive
	{
		friend class CXTPPropExchangeArchive;
	};
//}}AFX_CODEJOCK_PRIVATE


	CSelfArchive& m_ar;         // Contains the underlying CArchive for this CXTPPropExchangeArchive object.
};


AFX_INLINE BOOL CXTPPropExchangeArchive::IsAllowDefaultValues() const
{
	return FALSE;
}

AFX_INLINE BOOL CXTPPropExchangeArchive::IsAllowBlobValues() const
{
	return TRUE;
}

AFX_INLINE CArchive& CXTPPropExchangeArchive::GetSelfArchive()
{
	return m_ar;
}
