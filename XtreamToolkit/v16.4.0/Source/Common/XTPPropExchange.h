//////////////////////////////////////////////////////////////////////
// XTPPropExchange.h: interface for the CXTPPropExchange class.
//
// This file is a part of the XTREME TOOLKIT PRO MFC class library.
// (c)1998-2013 Codejock Software, All Rights Reserved.
//
// THIS SOURCE FILE IS THE PROPERTY OF CODEJOCK SOFTWARE AND IS NOT TO BE
// RE-DISTRIBUTED BY ANY MEANS WHATSOEVER WITHOUT THE EXPRESSED WRITTEN
// CONSENT OF CODEJOCK SOFTWARE.
//
// THIS SOURCE CODE CAN ONLY BE USED UNDER THE TERMS AND CONDITIONS OUTLINED
// IN THE XTREME TOOLKIT PRO LICENSE AGREEMENT. CODEJOCK SOFTWARE GRANTS TO
// YOU (ONE SOFTWARE DEVELOPER) THE LIMITED RIGHT TO USE THIS SOFTWARE ON A
// SINGLE COMPUTER.
//
// CONTACT INFORMATION:
// support@codejock.com
// http://www.codejock.com
//
/////////////////////////////////////////////////////////////////////////////

//{{AFX_CODEJOCK_PRIVATE
#if !defined(__XTPPROPEXCHANGE_H__)
#define __XTPPROPEXCHANGE_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XTPXMLHelpers.h"
#include "XTPPropExchange.inc"

#pragma warning(disable:4510)
#pragma warning(disable:4610)

#define XTP_LOCALE_ISO8601 ((LCID)-1)

class CXTPPropExchange;
class CXTPPropExchangeEnumerator;

enum VARTYPE_EX
{
	VT_EX_RECT = 100,
	VT_EX_SIZE = 101
};

//===========================================================================
// Summary:
//     CXTPPropExchange is standalone pure class.
//     The CXTPPropExchange class allows you to save a complex network of objects in a permanent binary or text form (usually disk storage) that persists after those objects are deleted. Later you can load the objects from persistent storage, reconstituting them in memory. This process of making data persistent is called 'serialization.'
//
//     You can use one of derived classes:
//       * CXTPPropExchangeArchive to serialize to archive,
//       * CXTPPropExchangeRegistry to serialize to registry key,
//       * CXTPPropExchangeXMLNode to serialize to XML node.
// Example:
// <code>
// CXTPPropExchangeXMLNode px(TRUE, 0, _T("Settings"));
// if (px.LoadFromFile(m_strIniFileName))
// {
//     m_wndTaskPanel.GetGroups()->DoPropExchange(&px);
// }
// </code>
//===========================================================================
class _XTP_EXT_CLASS CXTPPropExchange : public CXTPCmdTarget
{
	DECLARE_DYNAMIC(CXTPPropExchange)
public:

	//-------------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPPropExchange object.
	//-------------------------------------------------------------------------
	CXTPPropExchange();

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPPropExchange object, handles cleanup and de-
	//     allocation.
	//-------------------------------------------------------------------------
	virtual ~CXTPPropExchange();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     This method determines whether properties are being loaded to
	//     the class or stored from it.
	// Returns:
	//     Nonzero if properties are being loaded; otherwise, it is zero.
	//-----------------------------------------------------------------------
	BOOL IsLoading() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method determines whether properties are being loaded to
	//     the class or stored from it.
	// Returns:
	//     Nonzero if the properties are currently being used for storing; otherwise 0.
	//-----------------------------------------------------------------------
	BOOL IsStoring() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to use default values specified in PX_* functions while load process.
	// Parameters:
	//     bUseDefaultOnLoad - TRUE to use default value; FALSE to ignore it.
	// See Also: UseDefaultOnSave, PX_String, PX_Int, PX_Bool
	//-----------------------------------------------------------------------
	void UseDefaultOnLoad(BOOL bUseDefaultOnLoad = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to use default values specified in PX_* functions while save process.
	// Parameters:
	//     bUseDefaultOnSave - TRUE to use default value; FALSE to ignore it.
	// See Also: UseDefaultOnLoad, PX_String, PX_Int, PX_Bool
	//-----------------------------------------------------------------------
	void UseDefaultOnSave(BOOL bUseDefaultOnSave = TRUE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines if CXTPPropExchange is used as root section
	// Returns:
	//     TRUE if CXTPPropExchange object it root section.
	// Remarks:
	//     Used only in CXTPPropExchangeXMLNode and CXTPPropExchangeRegistry classes
	// See Also: GetSection
	//-----------------------------------------------------------------------
	BOOL IsRootSection() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function from the DoPropExchange function to determine the version of the object that is currently being de-serialized
	// Returns:
	//     During de-serialization, the version of the object being read.
	//-----------------------------------------------------------------------
	UINT GetSchema() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Exchanges a property between a storage and the class.
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
								void* pvProp, const void* pvDefault = NULL) = 0;

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

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to serialize or initialize the state of a class's version information
	//-------------------------------------------------------------------------
	void ExchangeSchema();

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to serialize or initialize the state of a user locale
	//-------------------------------------------------------------------------
	void ExchangeLocale();

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
	virtual CXTPPropExchange* GetSection(LPCTSTR lpszSection) = 0;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this method to free section information before storing
	//-------------------------------------------------------------------------
	virtual void EmptySection();

	//-----------------------------------------------------------------------
	// Summary:
	//     Use this member function to Write DWORD number to storage
	// Parameters:
	//     dwCount - DWORD number
	// See Also: ReadCount
	//-----------------------------------------------------------------------
	virtual void WriteCount (DWORD dwCount);

	//-----------------------------------------------------------------------
	// Summary:
	//     Use this member function to read DWORD number from storage
	// Returns:
	//     DWORD number
	// See Also: WriteCount
	//-----------------------------------------------------------------------
	virtual DWORD ReadCount ();

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
	//     This method is called before exchange properties.
	// Returns:
	//     TRUE if storage ready to exchange properties.
	//-----------------------------------------------------------------------
	virtual BOOL OnBeforeExchange();

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
	static void AFX_CDECL BinToString(BYTE* pData, DWORD nBytes, CString& str);
	static BOOL AFX_CDECL StringToBin(const CString& str, BYTE*& pData, DWORD& nBytes);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     This methods is called to copy internal data from root section.
	// Parameters:
	//     pRootSection - Parent section
	//-----------------------------------------------------------------------
	void InitSection(CXTPPropExchange* pRootSection);

//{{AFX_CODEJOCK_PRIVATE
public:
	static void AFX_CDECL PreformatString(CString& strValue, BOOL bLoading);

	virtual void ExchangeSchemaSafe();


protected:
	DWORD GetSizeOfVarType(VARTYPE vt);
	BOOL IsSamePropValue(VARTYPE vtProp, const void* pv1, const void* pv2);
	BOOL CopyPropValue(VARTYPE vtProp, void* pvDest, const void* pvSrc);

	BOOL ExchangePropStringHelper(CString& strValue, VARTYPE vtProp, void* pvProp, const void* pvDefault);
	BOOL ExchangePropDateHelper(CString& strValue, COleDateTime& dt);
	void PreformatString(CString& strValue);
	void PreformatString(BSTR bstrValue);

public:
	static CRuntimeClass* FindRuntimeClass(LPCSTR szClassName);
//}}AFX_CODEJOCK_PRIVATE

public:
	DWORD_PTR m_dwData;         // User defined 32-bit value.
	CObject* m_pOwner;          // User defined Owner value.
	static LCID m_lcidDateTime; // User regional language

protected:
	BOOL m_bLoading;            // TRUE if properties are being loaded to the class.
	UINT m_nSchema;             // Schema of object.
	BOOL m_bChildSection;       // TRUE if section is child
	BOOL m_bEmptySection;       // TRUE if EmptySection method was called.
	BOOL m_bInitialized;        // TRUE if section was initialized.
	BOOL m_bUseDefaultOnLoad;   // TRUE if default values must be used in load process.
	BOOL m_bUseDefaultOnSave;   // TRUE if default values must be used in save process.

};



//---------------------------------------------------------------------------
// Summary:
//     Call this function within your class's DoPropExchange member function to serialize or initialize a property of type BYTE.
//     The property's value is read from or written to the variable referenced by nValue, as appropriate. If nDefault is specified, it will be used as the property's default value. This value is used if, for any reason, the serialization process fails.
// Parameters:
//     pPX - Pointer to the CXTPPropExchange object (typically passed as a parameter to DoPropExchange).
//     pszPropName - The name of the property being exchanged.
//     nValue - Reference to the variable where the property is stored (typically a member variable of your class).
//     nDefault - Default value for the property
// Returns:
//     Nonzero if the exchange was successful; 0 if unsuccessful.
//---------------------------------------------------------------------------
_XTP_EXT_CLASS BOOL AFX_CDECL PX_Byte(CXTPPropExchange* pPX, LPCTSTR pszPropName, BYTE& nValue, BYTE nDefault);

//---------------------------------------------------------------------------
// Summary:
//     Call this function within your class's DoPropExchange member function to serialize or initialize a property of type short.
//     The property's value is read from or written to the variable referenced by sValue, as appropriate. If sDefault is specified, it will be used as the property's default value. This value is used if, for any reason, the serialization process fails.
// Parameters:
//     pPX - Pointer to the CXTPPropExchange object (typically passed as a parameter to DoPropExchange).
//     pszPropName - The name of the property being exchanged.
//     sValue - Reference to the variable where the property is stored (typically a member variable of your class).
//     sDefault - Default value for the property
// Returns:
//     Nonzero if the exchange was successful; 0 if unsuccessful.
//---------------------------------------------------------------------------
_XTP_EXT_CLASS BOOL AFX_CDECL PX_Short(CXTPPropExchange* pPX, LPCTSTR pszPropName, short& sValue);
_XTP_EXT_CLASS BOOL AFX_CDECL PX_Short(CXTPPropExchange* pPX, LPCTSTR pszPropName, short& sValue, short sDefault);// <COMBINE PX_Short>

//---------------------------------------------------------------------------
// Summary:
//     Call this function within your class's DoPropExchange member function to serialize or initialize a property of type USHORT.
//     The property's value is read from or written to the variable referenced by usValue, as appropriate. If usDefault is specified, it will be used as the property's default value. This value is used if, for any reason, the serialization process fails.
// Parameters:
//     pPX - Pointer to the CXTPPropExchange object (typically passed as a parameter to DoPropExchange).
//     pszPropName - The name of the property being exchanged.
//     usValue - Reference to the variable where the property is stored (typically a member variable of your class).
//     usDefault - Default value for the property
// Returns:
//     Nonzero if the exchange was successful; 0 if unsuccessful.
//---------------------------------------------------------------------------
_XTP_EXT_CLASS BOOL AFX_CDECL PX_UShort(CXTPPropExchange* pPX, LPCTSTR pszPropName, USHORT& usValue);
_XTP_EXT_CLASS BOOL AFX_CDECL PX_UShort(CXTPPropExchange* pPX, LPCTSTR pszPropName, USHORT& usValue, USHORT usDefault); // <COMBINE PX_UShort>


//---------------------------------------------------------------------------
// Summary:
//     Call this function within your class's DoPropExchange member function to serialize or initialize a property of type long.
//     The property's value is read from or written to the variable referenced by lValue, as appropriate. If lDefault is specified, it will be used as the property's default value. This value is used if, for any reason, the serialization process fails.
// Parameters:
//     pPX - Pointer to the CXTPPropExchange object (typically passed as a parameter to DoPropExchange).
//     pszPropName - The name of the property being exchanged.
//     lValue - Reference to the variable where the property is stored (typically a member variable of your class).
//     lDefault - Default value for the property
// Returns:
//     Nonzero if the exchange was successful; 0 if unsuccessful.
//---------------------------------------------------------------------------
_XTP_EXT_CLASS BOOL AFX_CDECL PX_Long(CXTPPropExchange* pPX, LPCTSTR pszPropName, long& lValue);
_XTP_EXT_CLASS BOOL AFX_CDECL PX_Long(CXTPPropExchange* pPX, LPCTSTR pszPropName, long& lValue, long lDefault); // <COMBINE PX_Long>



//---------------------------------------------------------------------------
// Summary:
//     Call this function within your class's DoPropExchange member function to serialize or initialize a property of type DWORD.
//     The property's value is read from or written to the variable referenced by dwValue, as appropriate. If dwDefault is specified, it will be used as the property's default value. This value is used if, for any reason, the serialization process fails.
// Parameters:
//     pPX - Pointer to the CXTPPropExchange object (typically passed as a parameter to DoPropExchange).
//     pszPropName - The name of the property being exchanged.
//     dwValue - Reference to the variable where the property is stored (typically a member variable of your class).
//     dwDefault - Default value for the property
// Returns:
//     Nonzero if the exchange was successful; 0 if unsuccessful.
//---------------------------------------------------------------------------
_XTP_EXT_CLASS BOOL AFX_CDECL PX_DWord(CXTPPropExchange* pPX, LPCTSTR pszPropName, DWORD& dwValue);
_XTP_EXT_CLASS BOOL AFX_CDECL PX_DWord(CXTPPropExchange* pPX, LPCTSTR pszPropName, DWORD& dwValue, DWORD dwDefault); // <COMBINE PX_DWord>

//---------------------------------------------------------------------------
// Summary:
//     Call this function within your class's DoPropExchange member function
//     to serialize or initialize a property of type ULONGLONG.
//     The property's value is read from or written to the variable referenced
//     by ullValue, as appropriate. If ullDefault is specified, it will be used
//     as the property's default value. This value is used if, for any reason,
//     the serialization process fails.
// Parameters:
//     pPX - Pointer to the CXTPPropExchange object (typically passed as a parameter to DoPropExchange).
//     pszPropName  - The name of the property being exchanged.
//     ullValue     - Reference to the variable where the property is stored
//                    (typically a member variable of your class).
//     ullDefault   - Default value for the property.
// Returns:
//     Nonzero if the exchange was successful; 0 if unsuccessful.
//---------------------------------------------------------------------------
_XTP_EXT_CLASS BOOL AFX_CDECL PX_UI8(CXTPPropExchange* pPX, LPCTSTR pszPropName,
									 ULONGLONG& ullValue, ULONGLONG ullDefault);

//---------------------------------------------------------------------------
// Summary:
//     Call this function within your class's DoPropExchange member function to serialize or initialize a property of type int.
//     The property's value is read from or written to the variable referenced by nValue, as appropriate. If nDefault is specified, it will be used as the property's default value. This value is used if, for any reason, the serialization process fails.
// Parameters:
//     pPX - Pointer to the CXTPPropExchange object (typically passed as a parameter to DoPropExchange).
//     pszPropName - The name of the property being exchanged.
//     nValue - Reference to the variable where the property is stored (typically a member variable of your class).
//     nDefault - Default value for the property
// Returns:
//     Nonzero if the exchange was successful; 0 if unsuccessful.
//---------------------------------------------------------------------------
_XTP_EXT_CLASS BOOL AFX_CDECL PX_Int(CXTPPropExchange* pPX, LPCTSTR pszPropName, int& nValue);
_XTP_EXT_CLASS BOOL AFX_CDECL PX_Int(CXTPPropExchange* pPX, LPCTSTR pszPropName, int& nValue, int nDefault); // <COMBINE PX_Int>

//---------------------------------------------------------------------------
// Summary:
//     Call this function within your class's DoPropExchange member function to serialize or initialize a property of type ULONG.
//     The property's value is read from or written to the variable referenced by ulValue, as appropriate. If ulDefault is specified, it will be used as the property's default value. This value is used if, for any reason, the serialization process fails.
// Parameters:
//     pPX - Pointer to the CXTPPropExchange object (typically passed as a parameter to DoPropExchange).
//     pszPropName - The name of the property being exchanged.
//     ulValue - Reference to the variable where the property is stored (typically a member variable of your class).
//     ulDefault - Default value for the property
// Returns:
//     Nonzero if the exchange was successful; 0 if unsuccessful.
//---------------------------------------------------------------------------
_XTP_EXT_CLASS BOOL AFX_CDECL PX_ULong(CXTPPropExchange* pPX, LPCTSTR pszPropName, ULONG& ulValue);
_XTP_EXT_CLASS BOOL AFX_CDECL PX_ULong(CXTPPropExchange* pPX, LPCTSTR pszPropName, ULONG& ulValue, ULONG ulDefault); // <COMBINE PX_ULong>

//---------------------------------------------------------------------------
// Summary:
//     Call this function within your class's DoPropExchange member function to serialize or initialize a property of type BOOL.
//     The property's value is read from or written to the variable referenced by bValue, as appropriate. If bDefault is specified, it will be used as the property's default value. This value is used if, for any reason, the serialization process fails.
// Parameters:
//     pPX - Pointer to the CXTPPropExchange object (typically passed as a parameter to DoPropExchange).
//     pszPropName - The name of the property being exchanged.
//     bValue - Reference to the variable where the property is stored (typically a member variable of your class).
//     bDefault - Default value for the property
// Returns:
//     Nonzero if the exchange was successful; 0 if unsuccessful.
//---------------------------------------------------------------------------
_XTP_EXT_CLASS BOOL AFX_CDECL PX_Bool(CXTPPropExchange* pPX, LPCTSTR pszPropName, BOOL& bValue);
_XTP_EXT_CLASS BOOL AFX_CDECL PX_Bool(CXTPPropExchange* pPX, LPCTSTR pszPropName, BOOL& bValue, BOOL bDefault); // <COMBINE PX_Bool>

//---------------------------------------------------------------------------
// Summary:
//     Call this function within your class's DoPropExchange member function to serialize or initialize a character string property.
//     The property's value is read from or written to the variable referenced by strValue, as appropriate. If lpszDefault is specified, it will be used as the property's default value. This value is used if, for any reason, the serialization process fails.
// Parameters:
//     pPX - Pointer to the CXTPPropExchange object (typically passed as a parameter to DoPropExchange).
//     pszPropName - The name of the property being exchanged.
//     strValue - Reference to the variable where the property is stored (typically a member variable of your class).
//     lpszDefault - Default value for the property
// Returns:
//     Nonzero if the exchange was successful; 0 if unsuccessful.
//---------------------------------------------------------------------------
_XTP_EXT_CLASS BOOL AFX_CDECL PX_String(CXTPPropExchange* pPX, LPCTSTR pszPropName, CString& strValue);
_XTP_EXT_CLASS BOOL AFX_CDECL PX_String(CXTPPropExchange* pPX, LPCTSTR pszPropName, CString& strValue, LPCTSTR lpszDefault); // <COMBINE PX_String>

//---------------------------------------------------------------------------
// Summary:
//     Call this function within your class's DoPropExchange member function to serialize or initialize a character string property.
//     The property's value is read from or written to the variable referenced by strValue, as appropriate. If lpszDefault is specified, it will be used as the property's default value. This value is used if, for any reason, the serialization process fails.
// Parameters:
//     pPX - Pointer to the CXTPPropExchange object (typically passed as a parameter to DoPropExchange).
//     pszPropName - The name of the property being exchanged.
//     bstrValue   - Reference to the variable where the property is stored (typically a member variable of your class).
//     bstrDefault - Default value for the property
// Returns:
//     Nonzero if the exchange was successful; 0 if unsuccessful.
//---------------------------------------------------------------------------
_XTP_EXT_CLASS BOOL AFX_CDECL PX_Bstr(CXTPPropExchange* pPX, LPCTSTR pszPropName, BSTR& bstrValue, BSTR bstrDefault);

//---------------------------------------------------------------------------
// Summary:
//     Call this function within your class's DoPropExchange member function to serialize or initialize a property of type float.
//     The property's value is read from or written to the variable referenced by floatValue, as appropriate. If floatDefault is specified, it will be used as the property's default value. This value is used if, for any reason, the serialization process fails.
// Parameters:
//     pPX - Pointer to the CXTPPropExchange object (typically passed as a parameter to DoPropExchange).
//     pszPropName - The name of the property being exchanged.
//     floatValue - Reference to the variable where the property is stored (typically a member variable of your class).
//     floatDefault - Default value for the property
// Returns:
//     Nonzero if the exchange was successful; 0 if unsuccessful.
//---------------------------------------------------------------------------
_XTP_EXT_CLASS BOOL AFX_CDECL PX_Float(CXTPPropExchange* pPX, LPCTSTR pszPropName, float& floatValue);
_XTP_EXT_CLASS BOOL AFX_CDECL PX_Float(CXTPPropExchange* pPX, LPCTSTR pszPropName, float& floatValue, float floatDefault); // <COMBINE PX_Float>

//---------------------------------------------------------------------------
// Summary:
//     Call this function within your class's DoPropExchange member function to serialize or initialize a property of type double.
//     The property's value is read from or written to the variable referenced by doubleValue, as appropriate. If doubleDefault is specified, it will be used as the property's default value. This value is used if, for any reason, the serialization process fails.
// Parameters:
//     pPX - Pointer to the CXTPPropExchange object (typically passed as a parameter to DoPropExchange).
//     pszPropName - The name of the property being exchanged.
//     doubleValue - Reference to the variable where the property is stored (typically a member variable of your class).
//     doubleDefault - Default value for the property
// Returns:
//     Nonzero if the exchange was successful; 0 if unsuccessful.
//---------------------------------------------------------------------------
_XTP_EXT_CLASS BOOL AFX_CDECL PX_Double(CXTPPropExchange* pPX, LPCTSTR pszPropName, double& doubleValue);
_XTP_EXT_CLASS BOOL AFX_CDECL PX_Double(CXTPPropExchange* pPX, LPCTSTR pszPropName, double& doubleValue, double doubleDefault); // <COMBINE PX_Double>

//---------------------------------------------------------------------------
// Summary:
//     Call this function within your class's DoPropExchange member function to serialize or initialize a property of type SIZE.
//     The property's value is read from or written to the variable referenced by szValue, as appropriate. If szDefault is specified, it will be used as the property's default value. This value is used if, for any reason, the serialization process fails.
// Parameters:
//     pPX - Pointer to the CXTPPropExchange object (typically passed as a parameter to DoPropExchange).
//     pszPropName - The name of the property being exchanged.
//     szValue - Reference to the variable where the property is stored (typically a member variable of your class).
//     szDefault - Default value for the property
// Returns:
//     Nonzero if the exchange was successful; 0 if unsuccessful.
//---------------------------------------------------------------------------
_XTP_EXT_CLASS BOOL AFX_CDECL PX_Size(CXTPPropExchange* pPX, LPCTSTR pszPropName, SIZE& szValue, SIZE szDefault);

//---------------------------------------------------------------------------
// Summary:
//     Call this function within your class's DoPropExchange member function to serialize or initialize a property of type RECT.
//     The property's value is read from or written to the variable referenced by rcValue, as appropriate. If rcDefault is specified, it will be used as the property's default value. This value is used if, for any reason, the serialization process fails.
// Parameters:
//     pPX - Pointer to the CXTPPropExchange object (typically passed as a parameter to DoPropExchange).
//     pszPropName - The name of the property being exchanged.
//     rcValue - Reference to the variable where the property is stored (typically a member variable of your class).
//     rcDefault - Default value for the property
// Returns:
//     Nonzero if the exchange was successful; 0 if unsuccessful.
//---------------------------------------------------------------------------
_XTP_EXT_CLASS BOOL AFX_CDECL PX_Rect(CXTPPropExchange* pPX, LPCTSTR pszPropName, RECT& rcValue, RECT rcDefault);

//---------------------------------------------------------------------------
// Summary:
//     Call this function within your class's DoPropExchange member function to serialize or initialize a property of type POINT.
//     The property's value is read from or written to the variable referenced by ptValue, as appropriate. If ptDefault is specified, it will be used as the property's default value. This value is used if, for any reason, the serialization process fails.
// Parameters:
//     pPX - Pointer to the CXTPPropExchange object (typically passed as a parameter to DoPropExchange).
//     pszPropName - The name of the property being exchanged.
//     ptValue - Reference to the variable where the property is stored (typically a member variable of your class).
//     ptDefault - Default value for the property
// Returns:
//     Nonzero if the exchange was successful; 0 if unsuccessful.
//---------------------------------------------------------------------------
_XTP_EXT_CLASS BOOL AFX_CDECL PX_Point(CXTPPropExchange* pPX, LPCTSTR pszPropName, POINT& ptValue, POINT ptDefault);

//---------------------------------------------------------------------------
// Summary:
//     Call this function within your class's DoPropExchange member function to serialize or initialize a property of type COleDateTime.
//     The property's value is read from or written to the variable referenced by dtValue, as appropriate.
// Parameters:
//     pPX - Pointer to the CXTPPropExchange object (typically passed as a parameter to DoPropExchange).
//     pszPropName - The name of the property being exchanged.
//     dtValue - Reference to the variable where the property is stored (typically a member variable of your class).
// Returns:
//     Nonzero if the exchange was successful; 0 if unsuccessful.
//---------------------------------------------------------------------------
_XTP_EXT_CLASS BOOL AFX_CDECL PX_DateTime(CXTPPropExchange* pPX, LPCTSTR pszPropName, COleDateTime& dtValue);


//---------------------------------------------------------------------------
// Summary:
//     Call this function within your class's DoPropExchange member function to serialize or initialize a property of type CRuntimeClass.
//     The property's value is read from or written to the variable referenced by pClass, as appropriate. If pDefaultClass is specified, it will be used as the property's default value. This value is used if, for any reason, the serialization process fails.
// Parameters:
//     pPX - Pointer to the CXTPPropExchange object (typically passed as a parameter to DoPropExchange).
//     pszPropName - The name of the property being exchanged.
//     pClass - Reference to the variable where the property is stored (typically a member variable of your class).
//     pDefaultClass - Default value for the property
// Returns:
//     Nonzero if the exchange was successful; 0 if unsuccessful.
//---------------------------------------------------------------------------
_XTP_EXT_CLASS BOOL AFX_CDECL PX_RuntimeClass(CXTPPropExchange* pPX, LPCTSTR pszPropName, CRuntimeClass*& pClass, CRuntimeClass* pDefaultClass);

//---------------------------------------------------------------------------
// Summary:
//     Call this function within your class's DoPropExchange member function to serialize object.
// Parameters:
//     pPX - Pointer to the CXTPPropExchange object (typically passed as a parameter to DoPropExchange).
//     pszPropName - The name of the property being exchanged.
//     pObject - Object you need to serialize
// Returns:
//     Nonzero if the exchange was successful; 0 if unsuccessful.
//---------------------------------------------------------------------------
_XTP_EXT_CLASS BOOL AFX_CDECL PX_Serialize(CXTPPropExchange* pPX, LPCTSTR pszPropName, CObject* pObject);


//---------------------------------------------------------------------------
// Summary:
//     Call this function within your class's DoPropExchange member function to serialize BYTE array.
// Parameters:
//     pPX - Pointer to the CXTPPropExchange object (typically passed as a parameter to DoPropExchange).
//     pszPropName - The name of the property being exchanged.
//     pData - Byte array
//     nBytes - Size of Byte array.
// Returns:
//     Nonzero if the exchange was successful; 0 if unsuccessful.
//---------------------------------------------------------------------------
_XTP_EXT_CLASS BOOL AFX_CDECL PX_Blob(CXTPPropExchange* pPX, LPCTSTR pszPropName, BYTE*& pData, DWORD& nBytes);


//---------------------------------------------------------------------------
// Summary:
//     Call this function within your class's DoPropExchange member function to serialize or initialize a property of type COleVariant.
//     The property's value is read from or written to the variable referenced by variant, as appropriate. If variantDefault is specified, it will be used as the property's default value. This value is used if, for any reason, the serialization process fails.
// Parameters:
//     pPX - Pointer to the CXTPPropExchange object (typically passed as a parameter to DoPropExchange).
//     pszPropName - The name of the property being exchanged.
//     variant - Reference to the variable where the property is stored (typically a member variable of your class).
//     variantDefault - Default value for the property
// Returns:
//     Nonzero if the exchange was successful; 0 if unsuccessful.
//---------------------------------------------------------------------------
_XTP_EXT_CLASS BOOL AFX_CDECL PX_Variant(CXTPPropExchange* pPX, LPCTSTR pszPropName, COleVariant& variant, COleVariant variantDefault);


//---------------------------------------------------------------------------
// Summary:
//     Call this function within your class's DoPropExchange member function to serialize or initialize a enum property.
//     The property's value is read from or written to the variable referenced by nValue, as appropriate. If nDefault is specified, it will be used as the property's default value. This value is used if, for any reason, the serialization process fails.
// Parameters:
//     pPX - Pointer to the CXTPPropExchange object (typically passed as a parameter to DoPropExchange).
//     pszPropName - The name of the property being exchanged.
//     nValue - Reference to the variable where the property is stored (typically a member variable of your class).
//     nDefault - Default value for the property
// Returns:
//     Nonzero if the exchange was successful; 0 if unsuccessful.
//---------------------------------------------------------------------------
template <class TEnum>
AFX_INLINE BOOL AFX_CDECL PX_Enum(CXTPPropExchange* pPX, LPCTSTR pszPropName, TEnum& nValue, TEnum nDefault) {
	ASSERT(sizeof(TEnum) == sizeof(int));
	return PX_Int(pPX, pszPropName, (int&)nValue, (int)nDefault);
}


//---------------------------------------------------------------------------
// Summary:
//     Call this function within your class's DoPropExchange member function to serialize any class having DoPropExchange method
// Parameters:
//     pPX - Pointer to the CXTPPropExchange object (typically passed as a parameter to DoPropExchange).
//     pszPropName - The name of the property being exchanged.
//     pObject - Reference to the object.
//     pObjectRuntime - Default runtime class  of object
// Returns:
//     Nonzero if the exchange was successful; 0 if unsuccessful.
//---------------------------------------------------------------------------
template <class TObject>
AFX_INLINE BOOL AFX_CDECL PX_Object(CXTPPropExchange* pPX, LPCTSTR pszPropName, TObject*& pObject, CRuntimeClass* pObjectRuntime)
{
	CXTPPropExchangeSection secObject(pPX->GetSection(pszPropName));
	if (!secObject->ExchangeObjectInstance((CObject*&)pObject, pObjectRuntime))
		return FALSE;
	pObject->DoPropExchange(&secObject);
	return TRUE;
}
template <class TObject>
AFX_INLINE BOOL AFX_CDECL PX_Object(CXTPPropExchange* pPX, TObject*& pObject, CRuntimeClass* pObjectRuntime)
{
	if (!pPX->ExchangeObjectInstance((CObject*&)pObject, pObjectRuntime))
		return FALSE;
	pObject->DoPropExchange(pPX);
	return TRUE;
}


//---------------------------------------------------------------------------


AFX_INLINE BOOL CXTPPropExchange::OnBeforeExchange() {
	m_bInitialized = TRUE;
	return TRUE;
}
AFX_INLINE void CXTPPropExchange::UseDefaultOnLoad(BOOL bUseDefaultOnLoad) {
	m_bUseDefaultOnLoad = bUseDefaultOnLoad;
}

AFX_INLINE void CXTPPropExchange::UseDefaultOnSave(BOOL bUseDefaultOnSave) {
	m_bUseDefaultOnSave = bUseDefaultOnSave;
}


AFX_INLINE BOOL CXTPPropExchange::IsLoading() const {
	return m_bLoading;
}
AFX_INLINE BOOL CXTPPropExchange::IsStoring() const {
	return !m_bLoading;
}
AFX_INLINE UINT CXTPPropExchange::GetSchema() const {
	return m_nSchema;
}
AFX_INLINE BOOL CXTPPropExchange::IsRootSection() const {
	return !m_bChildSection;
}
AFX_INLINE BOOL CXTPPropExchange::IsAllowDefaultValues() const {
	return TRUE;
}
AFX_INLINE BOOL CXTPPropExchange::IsAllowBlobValues() const {
	return FALSE;
}


#include <Common/PropExchange/XTPPropExchangeEnumerator.h>
#include <Common/PropExchange/XTPPropExchangeEnumeratorPtr.h>
#include <Common/PropExchange/XTPPropExchangeSection.h>
#include <Common/PropExchange/XTPPropExchangeArchive.h>
#include <Common/PropExchange/XTPPropExchangeXMLNode.h>
#include <Common/PropExchange/XTPPropExchangeRegistry.h>
#include <Common/PropExchange/XTPPropExchangeIniFile.h>

#ifdef _XTP_ACTIVEX
//{{AFX_CODEJOCK_PRIVATE
extern CXTPPropExchange* PropExchangeFromControl(LPDISPATCH lpDispatch);
//}}AFX_CODEJOCK_PRIVATE
#endif

#endif // #if !defined(__XTPPROPEXCHANGE_H__)
