//===========================================================================
// Summary:
//     CXTPPropExchangeRegistry is CXTPPropExchange derived class.
//     The CXTPPropExchangeRegistry class allows you to save a complex network of objects in a system registry that persists after those objects are deleted. Later you can load the objects from persistent storage, reconstituting them in memory. This process of making data persistent is called 'serialization.'
// Example:
// <code>
// HKEY hKey = AfxGetApp()->GetAppRegistryKey();
// CXTPPropExchangeRegistry px(FALSE, hKey, szSection);
// DoPropExchange(&px);
// RegCloseKey(hKey);
// </code>
//===========================================================================
class _XTP_EXT_CLASS CXTPPropExchangeRegistry : public CXTPPropExchange
{
	DECLARE_DYNAMIC(CXTPPropExchangeRegistry)
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs CXTPPropExchangeRegistry a object.
	// Parameters:
	//     bLoading    - A flag that specifies whether objects will be loaded from or stored.
	//     hParentKey  - Root HKEY
	//     lpszSection - Points to a null-terminated string that specifies the section containing the entry.
	//-----------------------------------------------------------------------
	CXTPPropExchangeRegistry(BOOL bLoading, HKEY hParentKey, LPCTSTR lpszSection);

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPPropExchangeRegistry object, handles cleanup and de-
	//     allocation.
	//-------------------------------------------------------------------------
	~CXTPPropExchangeRegistry();

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
	//     Call this method to check if section exists.
	// Parameters:
	//     lpszSection - The name of the section being checked.
	//-----------------------------------------------------------------------
	BOOL IsSectionExists(LPCTSTR lpszSection);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to check if property entry exists.
	// Parameters:
	//     pszPropName - The name of the property being checked.
	//-----------------------------------------------------------------------
	BOOL IsPropertyExists(LPCTSTR pszPropName);

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
	virtual CXTPPropExchange* GetSection(LPCTSTR lpszSection);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this method to free section information before storing
	//-------------------------------------------------------------------------
	virtual void EmptySection();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called before exchange properties.
	// Returns:
	//     TRUE if storage ready to exchange properties.
	//-----------------------------------------------------------------------
	virtual BOOL OnBeforeExchange();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to determine if storage allows exchange plain blob values.
	// Returns:
	//     TRUE if storage allows exchange plain blob values.
	//-----------------------------------------------------------------------
	virtual BOOL IsAllowBlobValues() const;


private:
	LONG DelRegTreeHelper(HKEY hParentKey, LPCTSTR lpszKeyName);

protected:
	HKEY m_hSectionKey;         // Current registry key
	HKEY m_hParentKey;          // Parent registry key
	CString m_strSectionName;   // Section name
	BOOL m_bCloseParentKey;     // TRUE to automatically close parent key
};


AFX_INLINE BOOL CXTPPropExchangeRegistry::IsAllowBlobValues() const
{
	return TRUE;
}
