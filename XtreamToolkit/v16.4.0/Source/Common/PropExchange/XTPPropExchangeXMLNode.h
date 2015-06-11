#ifndef _XTP_EXCLUDE_XML


//===========================================================================
// Summary:
//     CXTPPropExchangeXMLNode is CXTPPropExchange derived class.
//     The CXTPPropExchangeXMLNode class allows you to save a complex network of objects in a XML file that persists after those objects are deleted. Later you can load the objects from persistent storage, reconstituting them in memory. This process of making data persistent is called 'serialization.'
// Example:
// <code>
// CXTPPropExchangeXMLNode px(TRUE, 0, _T("Settings"));
// if (px.LoadFromFile(m_strIniFileName))
// {
//     m_wndTaskPanel.GetGroups()->DoPropExchange(&px);
// }
// </code>
//===========================================================================
class _XTP_EXT_CLASS CXTPPropExchangeXMLNode : public CXTPPropExchange
{
	class CXMLEnumerator;
	DECLARE_DYNAMIC(CXTPPropExchangeXMLNode)
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs CXTPPropExchangeXMLNode a object.
	// Parameters:
	//     bLoading      - A flag that specifies whether objects will be loaded from or stored.
	//     xmlParentNode - Parent node element. Can be NULL.
	//     lpszSection   - Points to a null-terminated string that specifies the section containing the entry.
	//-----------------------------------------------------------------------
	CXTPPropExchangeXMLNode(BOOL bLoading, CXTPDOMNodePtr xmlParentNode, LPCTSTR lpszSection);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if XML installed in System (IE 4.0)
	//-----------------------------------------------------------------------
	static BOOL AFX_CDECL IsXMLSupported();

public:

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

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this method to free section information before storing
	//-------------------------------------------------------------------------
	virtual void EmptySection();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to enumerate child sections.
	// Parameters:
	//     lpszSection - child section name.
	// Returns:
	//     Returns an enumerator for child sections.
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
	// See Also: CXTPPropExchangeEnumerator
	//-----------------------------------------------------------------------
	virtual CXTPPropExchangeEnumerator* GetEnumerator(LPCTSTR lpszSection);

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to check if property entry exists.
	// Parameters:
	//     pszPropName - The name of the property being checked.
	//-----------------------------------------------------------------------
	BOOL IsPropertyExists(LPCTSTR pszPropName);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to check if section exists.
	// Parameters:
	//     lpszSection - The name of the section being checked.
	//-----------------------------------------------------------------------
	BOOL IsSectionExists(LPCTSTR lpszSection);

	// Summary:
	//     Retrieves section name of the XML node.
	// Returns:
	//     Name of the XML node.
	//-----------------------------------------------------------------------
	CString GetSectionName();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to store data in compact mode (To use node attributes instead nodes)
	// Parameters:
	//     bCompactMode - TRUE to use compact mode (default)
	//-----------------------------------------------------------------------
	void SetCompactMode(BOOL bCompactMode);

	//-----------------------------------------------------------------------
	// Summary:
	//     Loads the contents of an existing file into a XML.
	// Parameters:
	//     lpszFileName -   A CString value that contains the name of a file to be loaded into the XML
	// Returns:
	//     TRUE if successful; otherwise 0.
	//-----------------------------------------------------------------------
	BOOL LoadFromFile(LPCTSTR lpszFileName);

	//-----------------------------------------------------------------------
	// Summary:
	//     Loads the contents of string to XML.
	// Parameters:
	//     lpszState - A pointer to a value that contains XML nodes
	//     bstrState - BSTR value that contains XML nodes
	// Returns:
	//     TRUE if successful; otherwise 0.
	//-----------------------------------------------------------------------
	BOOL LoadFromString(LPCSTR lpszState);
	BOOL LoadFromString(LPCWSTR lpszState);
	BOOL LoadFromString(BSTR bstrState); // <combine CXTPPropExchangeXMLNode::LoadFromString@LPCSTR>

	BOOL LoadFromEncodedString(LPCSTR lpszState);
	BOOL LoadFromEncodedString(LPCWSTR lpszState);

	BOOL LoadFromResource(HINSTANCE hInstance, LPCTSTR lpszResourceName, LPCTSTR lpszResourceType);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the XML representation of the node and all its descendants.
	// Parameters:
	//     bFormatted - TRUE to format XML output
	// Returns:
	//     XML representation of this node and all its descendants.
	//-----------------------------------------------------------------------
	CString GetXML(BOOL bFormatted = FALSE);
	CString GetEncodedXML(BOOL bFormatted = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Saves an XML document to the specified location.
	// Parameters:
	//     lpszFileName - Destination file name.
	//     bFormatted  - TRUE to format XML output
	//-----------------------------------------------------------------------
	HRESULT SaveToFile(LPCTSTR lpszFileName, BOOL bFormatted = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called before exchange properties.
	// Returns:
	//     TRUE if storage ready to exchange properties.
	//-----------------------------------------------------------------------
	virtual BOOL OnBeforeExchange();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to change loading flag.
	// Parameters
	//     bLoading - A flag that specifies whether objects will be loaded from or stored.
	//-----------------------------------------------------------------------
	void SetLoading(BOOL bLoading);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to append one section to another
	// Parameters
	//     pNode - Section to be append.
	//-----------------------------------------------------------------------
	BOOL PutSection(CXTPPropExchangeXMLNode* pNode);

	//-----------------------------------------------------------------------
	// Summary:
	//     Creates cloned version of the section
	// Parameters:
	//     bRecursive - TRUE to clone recursively
	//-----------------------------------------------------------------------
	CXTPPropExchangeXMLNode* Clone(BOOL bRecursive);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set encoding for XML output
	// Parameters:
	//     lpszEncoding - encoding that will be used in XML file
	// Remarks:
	//     Default encoding is UTF-8
	// Example:
	//     <code>px.SetEncoding(_T("windows-1251"));</code>
	//-----------------------------------------------------------------------
	void SetEncoding(LPCTSTR lpszEncoding);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to get access to dom xml node for current section
	//-----------------------------------------------------------------------
	CXTPDOMNodePtr GetSectionNode() const {
		return m_xmlSectionNode;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Creates new XML node and put it as Child
	// Parameters
	//     pszSection - New section name
	//-----------------------------------------------------------------------
	CXTPPropExchangeXMLNode* CreateSection(LPCTSTR pszSection);

public:
	CXTPDOMDocumentPtr GetDocumentPtr() const;
	CString GetEncoding() const;

protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves profile node.
	// Parameters:
	//     lpszSection - Section name of profile node
	// Returns:
	//     Profile node.to be used
	//-----------------------------------------------------------------------
	CXTPDOMNodePtr GetProfileNode(LPCTSTR lpszSection);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves entry node which the value is to be written.
	// Parameters:
	//     lpszEntry - Points to a null-terminated string that contains the entry into which the value is to be written.
	//     bCreate - TRUE to create if doesn't exist;
	// Returns:
	//     Entry node to be used
	//-----------------------------------------------------------------------
	CXTPDOMNodePtr GetEntryNode(LPCTSTR lpszEntry, BOOL bCreate);

private:
	void UpdateDocumentEncoding();
	BOOL CreateDocumentInstance();
	void FormatFile(LPCTSTR lpszFileName);
	void TraceParseError();
	CString BstrToString(BSTR bstrVal);
	void FormatString(CString& str);

protected:
	CXTPDOMDocumentPtr m_xmlDocPtr;         // XML Document pointer
	CXTPDOMNodePtr m_xmlParentNode;         // XML Parent node pointer
	CXTPDOMNodePtr m_xmlSectionNode;        // XML Section node pointer
	CString m_strSectionName;               // Section name
	CString m_strEncoding;                  // XML encoding.
	BOOL m_bCompactMode;                    // TRUE to use compact mode.
	friend class CXMLEnumerator;
	friend class CPropExchangeCtrl;
};


AFX_INLINE CXTPDOMDocumentPtr CXTPPropExchangeXMLNode::GetDocumentPtr() const
{
	return m_xmlDocPtr;
}

AFX_INLINE CString CXTPPropExchangeXMLNode::GetEncoding() const
{
	return m_strEncoding;
}

#endif
