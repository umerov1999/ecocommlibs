//===========================================================================
// Summary:
//     CXTPPropExchangeIniFile is CXTPPropExchange derived class.
//     The CXTPPropExchangeIniFile class allows you to save a complex network of objects in a ini file that persists after those objects are deleted. Later you can load the objects from persistent storage, reconstituting them in memory. This process of making data persistent is called 'serialization.'
// Example:
// <code>
// CXTPPropExchangeIniFile px(TRUE, 0, _T("Settings"));
// if (px.LoadFromFile(m_strIniFileName))
// {
//     m_wndTaskPanel.GetGroups()->DoPropExchange(&px);
// }
// </code>
//===========================================================================
class _XTP_EXT_CLASS CXTPPropExchangeIniFile : public CXTPPropExchange
{
	DECLARE_DYNAMIC(CXTPPropExchangeIniFile)
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs CXTPPropExchangeIniFile a object.
	// Parameters:
	//     bLoading           - A flag that specifies whether objects will be loaded from or stored.
	//     lpszParentSection  - Root section. Can be NULL.
	//     lpszSection        - Points to a null-terminated string that specifies the section containing the entry.
	//-----------------------------------------------------------------------
	CXTPPropExchangeIniFile(BOOL bLoading, LPCTSTR lpszParentSection, LPCTSTR lpszSection);

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPPropExchangeIniFile object, handles cleanup and de-
	//     allocation.
	//-------------------------------------------------------------------------
	~CXTPPropExchangeIniFile();

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

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called before exchange properties.
	// Returns:
	//     TRUE if storage ready to exchange properties.
	//-----------------------------------------------------------------------
	virtual BOOL OnBeforeExchange();

	//-----------------------------------------------------------------------
	// Summary:
	//     Saves an ini file to the specified location.
	// Parameters:
	//     strFileName - Destination file name.
	//-----------------------------------------------------------------------
	void SaveToFile(LPCTSTR strFileName);

	//-----------------------------------------------------------------------
	// Summary:
	//     Loads the contents of an existing ini file.
	// Parameters:
	//     lpszFileName -   A CString value that contains the name of a file to be loaded
	// Returns:
	//     TRUE if successful; otherwise 0.
	//-----------------------------------------------------------------------
	BOOL LoadFromFile(LPCTSTR lpszFileName);

	//-----------------------------------------------------------------------
	// Summary:
	//     Assign ini file to CXTPPropExchangeIniFile class.
	// Parameters:
	//     lpszFileName -   A CString value that contains the name of a file to be loaded/saved
	//-----------------------------------------------------------------------
	void AssignFile(LPCTSTR lpszFileName);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this method to free section information before storing
	//-------------------------------------------------------------------------
	virtual void EmptySection();


protected:
	CString m_strSectionName;   // Section name.
	CString m_strFileName;      // Temporary file name.
	BOOL m_bTempFile;           // TRUE if temporary file was created.
};
