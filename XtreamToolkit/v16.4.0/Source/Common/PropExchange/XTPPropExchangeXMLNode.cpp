#include "StdAfx.h"
#include <afxole.h>

#include <Common/XTPPropExchange.h>
#include <Common/XTPVC80Helpers.h>
#include <Common/XTPResourceManager.h>

#pragma warning(disable : 4310)

#ifdef _UNICODE
#define CT2BSTR(x) (BSTR)(LPCWSTR)x
#else
#define CT2BSTR(x) (BSTR)XTP_CT2CW(x)
#endif


//////////////////////////////////////////////////////////////////////////
// CXTPPropExchangeXMLNode

#ifndef _XTP_EXCLUDE_XML

IMPLEMENT_DYNAMIC(CXTPPropExchangeXMLNode, CXTPPropExchange)

CXTPPropExchangeXMLNode::CXTPPropExchangeXMLNode(BOOL bLoading, CXTPDOMNodePtr xmlParentNode, LPCTSTR lpszSection)
{
	m_bLoading = bLoading;
	m_strSectionName = lpszSection;
	m_xmlParentNode = xmlParentNode;
	m_xmlSectionNode = NULL;
	m_bCompactMode = TRUE;

	if (xmlParentNode)
	{
		xmlParentNode->get_ownerDocument(&m_xmlDocPtr);

		if (m_xmlDocPtr == 0)
		{
			m_xmlDocPtr = xmlParentNode;
		}
	}

	m_strEncoding = _T("UTF-8");

#ifdef _XTP_ACTIVEX
	EnableAutomation();
#endif
}

BOOL CXTPPropExchangeXMLNode::IsXMLSupported()
{
	CXTPDOMDocumentPtr xmlDocPtr;

	if (!SUCCEEDED(xmlDocPtr.CreateInstance(CLSID_XTPDOMDocument)))
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CXTPPropExchangeXMLNode::CreateDocumentInstance()
{
	if (m_xmlDocPtr == NULL)
	{
		if (!SUCCEEDED(m_xmlDocPtr.CreateInstance(CLSID_XTPDOMDocument)))
		{
			TRACE(_T("Warning: CreateDocumentInstance failed in CXTPPropExchangeXMLNode --\n"));
			TRACE(_T("\tperhaps AfxOleInit() has not been called, or msxml3.dll not found."));

			return FALSE;
		}

		m_xmlDocPtr->put_preserveWhiteSpace(VARIANT_TRUE);

	}
	return TRUE;
}

void CXTPPropExchangeXMLNode::TraceParseError()
{
#ifdef _DEBUG
	XTPXML::IXMLDOMParseErrorPtr parseError = 0;
	if (FAILED(m_xmlDocPtr->get_parseError(&parseError)))
		return;

	BSTR bstrReason = 0;
	if (FAILED(parseError->get_reason(&bstrReason)))
		return;

	CString strReason(bstrReason);
	TRACE(_T("Warning: CXTPPropExchangeXMLNode::LoadFromString failed. ParseError.Reason = %s"), (LPCTSTR)strReason);

	SysFreeString(bstrReason);
#endif
}

BOOL CXTPPropExchangeXMLNode::LoadFromEncodedString(LPCWSTR lpszState)
{
	return LoadFromEncodedString(XTP_CW2CA(lpszState));
}

BOOL CXTPPropExchangeXMLNode::LoadFromEncodedString(LPCSTR lpszState)
{
	if (!lpszState)
		return FALSE;

	if (!CreateDocumentInstance())
		return FALSE;


	ASSERT(m_xmlDocPtr != 0);

	VARIANT_BOOL bResult;

	VARIANT va;
	va.vt = VT_ARRAY | VT_UI1;

	ULONG nLen = (ULONG)strlen(lpszState);
	va.parray = SafeArrayCreateVector(VT_UI1, 0, nLen);
	MEMCPY_S(va.parray->pvData, lpszState, nLen);

	HRESULT hr = m_xmlDocPtr->load(va, &bResult);
	VariantClear(&va);

	if (FAILED(hr) || bResult == VARIANT_FALSE)
	{
		TraceParseError();
		return FALSE;
	}

	m_xmlParentNode = m_xmlDocPtr;
	m_xmlSectionNode = NULL;
	m_bInitialized = FALSE;

	return TRUE;
}

BOOL CXTPPropExchangeXMLNode::LoadFromString(LPCSTR lpszState)
{
	return LoadFromString((BSTR)XTP_CA2CW(lpszState));
}

BOOL CXTPPropExchangeXMLNode::LoadFromString(LPCWSTR lpszState)
{
	return LoadFromString((BSTR)(lpszState));
}

BOOL CXTPPropExchangeXMLNode::LoadFromString(BSTR lpszState)
{
	if (!CreateDocumentInstance())
		return FALSE;

	ASSERT(m_xmlDocPtr != 0);

	VARIANT_BOOL bResult;
	HRESULT hr = m_xmlDocPtr->loadXML(lpszState, &bResult);
	if (FAILED(hr) || bResult == VARIANT_FALSE)
	{
		TraceParseError();
		return FALSE;
	}

	m_xmlParentNode = m_xmlDocPtr;
	m_xmlSectionNode = NULL;
	m_bInitialized = FALSE;

	return TRUE;
}

BOOL CXTPPropExchangeXMLNode::LoadFromResource(HINSTANCE hInstance, LPCTSTR lpszResourceName, LPCTSTR lpszResourceType)
{
	HRSRC hRsrc = ::FindResource(hInstance, lpszResourceName, lpszResourceType);
	if (hRsrc == NULL)
		return FALSE;

	HGLOBAL hGlobal = LoadResource(hInstance, hRsrc);
	if (hGlobal == NULL)
		return FALSE;

	LPCSTR pData = (LPCSTR)LockResource(hGlobal);
	if (pData == NULL)
		return FALSE;

	DWORD dwSize = (DWORD)SizeofResource(hInstance, hRsrc);
	if (dwSize == 0)
		return FALSE;

	LPSTR lpstr = (LPSTR)malloc(dwSize + 1);
	if (!lpstr)
		return FALSE;

	MEMCPY_S(lpstr, pData, dwSize);
	lpstr[dwSize] = 0;

	BOOL bResult = LoadFromEncodedString(lpstr);

	UnlockResource(hGlobal);
	FreeResource(hGlobal);

	free(lpstr);

	return bResult;
}


BOOL CXTPPropExchangeXMLNode::LoadFromFile(LPCTSTR strFileName)
{
	if (!FILEEXISTS_S(strFileName))
		return FALSE;

	if (!CreateDocumentInstance())
		return FALSE;

	ASSERT(m_xmlDocPtr != 0);

	VARIANT_BOOL bResult;
	HRESULT hr = m_xmlDocPtr->load(COleVariant(strFileName), &bResult);
	if (FAILED(hr) || bResult == VARIANT_FALSE)
	{
		TraceParseError();
		return FALSE;
	}

	m_xmlParentNode = m_xmlDocPtr;
	m_xmlSectionNode = NULL;
	m_bInitialized = FALSE;

	return TRUE;
}


void CXTPPropExchangeXMLNode::FormatString(CString& str)
{
	const LPCTSTR lpszCrLf = _T("\r\n");

#define ADDINDENT(str, i, strOffset)\
	str.Insert(i, lpszCrLf);\
	str.Insert(i + 2, strOffset);\
	i += strOffset.GetLength() + 2;


	CString strOffset(_T('\t'));

	for (int i = 1; i < str.GetLength(); i++)
	{
		if (((str[i] == _T('>')) && (str[i - 1] == _T('/'))) || ((str[i] == _T('<')) && (str[i + 1] == _T('/'))))
		{
			strOffset.Delete(strOffset.GetLength() - 1);
			if (str[i] == _T('<') && str[i - 1] == _T('>'))
			{
				ADDINDENT(str, i, strOffset);
			}
		}
		else if (str[i] == _T('<'))
		{
			ADDINDENT(str, i, strOffset);
			strOffset += _T('\t');
		}
	}
}


CString CXTPPropExchangeXMLNode::GetEncodedXML(BOOL bFormatted)
{
	CString str;
	if (!OnBeforeExchange())
		return str;

	ASSERT(m_xmlDocPtr != 0);

	if (IsRootSection())
	{
		UpdateDocumentEncoding();

		COleStreamFile stream;
		if(!stream.CreateMemoryStream())
			return str;

		VARIANT var;
		var.vt = VT_UNKNOWN;
		var.punkVal = stream.GetStream();

		if (FAILED(m_xmlDocPtr->save(var)))
			return str;

		stream.SeekToBegin();
		DWORD dwLength = (DWORD)stream.GetLength();

		char* lpszBuffer = new char[dwLength + 1];
		stream.Read(lpszBuffer, dwLength);

		if (dwLength > 0 && lpszBuffer[dwLength - 1] <= 13) dwLength--; // Remove /n
		if (dwLength > 0 && lpszBuffer[dwLength - 1] <= 13) dwLength--; // Remove /r

		lpszBuffer[dwLength] = 0;

		str = lpszBuffer;

		delete[] lpszBuffer;
	}
	else
	{
		ASSERT(FALSE);
	}

	if (bFormatted)
	{
		FormatString(str);
	}

	return str;
}

CString CXTPPropExchangeXMLNode::GetXML(BOOL bFormatted)
{
	CString str;
	if (!OnBeforeExchange())
		return str;

	ASSERT(m_xmlDocPtr != 0);

	BSTR bstrXML = 0;

	if (IsRootSection())
	{
		if (FAILED(m_xmlDocPtr->get_xml(&bstrXML)))
			return str;
	}
	else
	{
		if (FAILED(m_xmlSectionNode->get_xml(&bstrXML)))
			return str;
	}

	str = bstrXML;
	SysFreeString(bstrXML);

	if (bFormatted)
	{
		FormatString(str);
	}

	return str;
}

void CXTPPropExchangeXMLNode::FormatFile(LPCTSTR lpszFileName)
{
	const LPCSTR lpszCrLf = "\r\n";
	const LPCSTR lpszOffset = "\t";

	#define WRITEINDENT\
		fileBuffer.Write(lpszBegin, int(lpszCurrent - lpszBegin) - nWhiteSpace);\
		fileBuffer.Write(lpszCrLf, 2);\
		for (int j = 0; j < nOffset; j++) fileBuffer.Write(lpszOffset, 1);\
		lpszBegin = lpszCurrent


	CFile file;
	if (!file.Open(lpszFileName, CFile::modeRead))
		return;

	DWORD dwLength = (DWORD)file.GetLength();

	if (dwLength < 3)
		return;

	BYTE* lpszBuffer = new BYTE[dwLength + 1];

	file.Read(lpszBuffer, dwLength);
	lpszBuffer[dwLength] = 0;
	file.Close();

	if (!file.Open(lpszFileName, CFile::modeWrite | CFile::modeCreate))
	{
		delete[] lpszBuffer;
		return;
	}

	CMemFile fileBuffer;

	int nOffset = 1, nWhiteSpace = 0;
	if (dwLength > 2 && lpszBuffer[dwLength - 2] == '\r')
		lpszBuffer[dwLength - 2] = 0;

	BYTE *lpszCurrent = lpszBuffer + 1, *lpszBegin = lpszBuffer;

	while (*lpszCurrent++)
	{
		BYTE& chCurrent = *lpszCurrent;

		if (((chCurrent == _T('>')) && (*(lpszCurrent - 1) == '/' || *(lpszCurrent - 1) == '?'))
			|| ((chCurrent == _T('<')) && (*(lpszCurrent + 1) == '/')))
		{
			nOffset--;
			if (chCurrent == '<' && *(lpszCurrent - 1 - nWhiteSpace) == '>')
			{
				WRITEINDENT;
			}
		}
		else if (chCurrent == '<')
		{
			WRITEINDENT;
			nOffset++;
		}

		nWhiteSpace = (chCurrent == '\r' || chCurrent == '\n' || chCurrent == '\t') ? nWhiteSpace + 1 : 0;
	}

	fileBuffer.Write(lpszBegin, int(lpszCurrent - lpszBegin) - 1);
	fileBuffer.Write(lpszCrLf, 2);

	delete[] lpszBuffer;

	dwLength = (DWORD)fileBuffer.GetLength();
	BYTE* lpszFileBuffer = fileBuffer.Detach();

	file.Write(lpszFileBuffer, dwLength);
	file.Close();

	free(lpszFileBuffer);
}

void CXTPPropExchangeXMLNode::UpdateDocumentEncoding()
{
	ASSERT(m_xmlDocPtr != 0);

	CString strInstruction = _T("version=\"1.0\" encoding=\"") + m_strEncoding + _T("\"");

	BOOL bInstructionExists = FALSE;

	CXTPDOMNodePtr ptrBaseInstruction = 0;
	if (SUCCEEDED(m_xmlDocPtr->selectSingleNode(L"/node()", &ptrBaseInstruction)) && ptrBaseInstruction != 0)
	{
		BSTR bstrNodeName = 0;
		ptrBaseInstruction->get_nodeName(&bstrNodeName);

		if (wcscmp(bstrNodeName, L"xml") == 0)
		{
			bInstructionExists = TRUE;
		}

		SysFreeString(bstrNodeName);
	}

	XTPXML::IXMLDOMProcessingInstructionPtr ptrInstruction = 0;
	if (!bInstructionExists && SUCCEEDED(m_xmlDocPtr->createProcessingInstruction(L"xml", CT2BSTR(strInstruction), &ptrInstruction)))
	{
		CXTPDOMNodePtr xmlNodePtr;
		if (SUCCEEDED(m_xmlDocPtr->selectSingleNode(L"/node()", &xmlNodePtr)))
		{
			m_xmlDocPtr->insertBefore(ptrInstruction, _variant_t(xmlNodePtr.GetInterfacePtr()), 0);
		}
	}
}

HRESULT CXTPPropExchangeXMLNode::SaveToFile(LPCTSTR strFileName, BOOL bFormatted)
{
	CString str;
	if (!OnBeforeExchange())
		return E_FAIL;

	UpdateDocumentEncoding();

	HRESULT hr = m_xmlDocPtr->save(COleVariant(strFileName));
	if (FAILED(hr))
		return hr;

	if (bFormatted)
	{
		FormatFile(strFileName);
	}

	return S_OK;
}

CXTPPropExchange* CXTPPropExchangeXMLNode::GetSection(LPCTSTR lpszSection)
{
	OnBeforeExchange();

	CXTPPropExchangeXMLNode* pSection = new CXTPPropExchangeXMLNode(IsLoading(), m_xmlSectionNode, lpszSection);
	pSection->m_bCompactMode = m_bCompactMode;
	pSection->InitSection(this);
	return pSection;
}

BOOL CXTPPropExchangeXMLNode::OnBeforeExchange()
{
	if (!m_bInitialized)
	{
		m_bInitialized = TRUE;

		if (m_xmlDocPtr == NULL && IsRootSection())
		{
			ASSERT(m_xmlParentNode == 0);

			if (!CreateDocumentInstance())
				return FALSE;

			if (m_xmlDocPtr == 0)
				return FALSE;

			m_xmlParentNode = m_xmlDocPtr;
			ASSERT(m_xmlParentNode != 0);
		}

		if ((m_xmlDocPtr == NULL) || (m_xmlParentNode == NULL))
			return FALSE;

		ASSERT(m_xmlSectionNode == 0);

		m_xmlSectionNode = GetProfileNode(m_strSectionName);

		if (m_xmlSectionNode != NULL && IsRootSection())
		{
			PX_Bool(this, _T("CompactMode"), m_bCompactMode, FALSE);
		}
	}

	if ((m_xmlDocPtr == NULL) || (m_xmlParentNode == NULL))
		return FALSE;

	return m_xmlSectionNode != NULL;
}

void CXTPPropExchangeXMLNode::EmptySection()
{
	CXTPPropExchange::EmptySection();

	OnBeforeExchange();

	if (m_xmlSectionNode != 0 && m_xmlParentNode != 0)
	{
		m_xmlParentNode->removeChild(m_xmlSectionNode, NULL);
		m_xmlSectionNode = 0;
		m_bInitialized = FALSE;
	}
}

void CXTPPropExchangeXMLNode::SetCompactMode(BOOL bCompactMode)
{
	m_bCompactMode = bCompactMode;
}


CXTPDOMNodePtr CXTPPropExchangeXMLNode::GetProfileNode(LPCTSTR strSection)
{
	ASSERT(m_xmlParentNode != 0);
	CXTPDOMNodePtr xmlNodePtr;
	CXTPDOMElementPtr xmlElementPtr;

	HRESULT hr = m_xmlParentNode->selectSingleNode(CT2BSTR(strSection), &xmlNodePtr);

	if (FAILED(hr) || xmlNodePtr == NULL)
	{
		m_xmlDocPtr->createElement(CT2BSTR(strSection), &xmlElementPtr);
		ASSERT(xmlElementPtr != 0);
		if (xmlElementPtr)
		{
			m_xmlParentNode->appendChild(xmlElementPtr, &xmlNodePtr);
		}
	}

	return xmlNodePtr;
}

CXTPDOMNodePtr CXTPPropExchangeXMLNode::GetEntryNode(LPCTSTR lpszEntry, BOOL bCreate)
{
	ASSERT(m_xmlSectionNode != 0);

	CXTPDOMNodePtr xmlNodeEntryPtr;

	if (lpszEntry == 0 || _tcslen(lpszEntry) == 0)
		return m_xmlSectionNode;

	if (m_bCompactMode)
	{
		XTPXML::IXMLDOMNamedNodeMapPtr map;
		m_xmlSectionNode->get_attributes(&map);
		HRESULT hr = map->getNamedItem(CT2BSTR(lpszEntry), &xmlNodeEntryPtr);

		if (FAILED(hr) || xmlNodeEntryPtr == NULL)
		{
			if (bCreate)
			{
				XTPXML::IXMLDOMAttributePtr xmlElementPtr;
				m_xmlDocPtr->createAttribute(CT2BSTR(lpszEntry), &xmlElementPtr);
				if (xmlElementPtr)
				{
					map->setNamedItem(xmlElementPtr, &xmlNodeEntryPtr);
				}
			}
			else
			{
				return 0;
			}
		}
	}
	else
	{
		CXTPDOMNodePtr xmlNodeSectionPtr = m_xmlSectionNode;

		HRESULT hr = xmlNodeSectionPtr->selectSingleNode(CT2BSTR(lpszEntry), &xmlNodeEntryPtr);
		if (FAILED(hr) || xmlNodeEntryPtr == NULL)
		{
			if (bCreate)
			{

				CXTPDOMElementPtr xmlElementPtr;
				m_xmlDocPtr->createElement(CT2BSTR(lpszEntry), &xmlElementPtr);
				ASSERT(xmlElementPtr != 0);
				if (xmlElementPtr)
				{
					xmlNodeSectionPtr->appendChild(xmlElementPtr, &xmlNodeEntryPtr);
				}
			}
			else
			{
				return 0;
			}
		}

	}
	ASSERT(xmlNodeEntryPtr != 0);
	return xmlNodeEntryPtr;
}

CString CXTPPropExchangeXMLNode::BstrToString(BSTR bstrVal)
{
	return CString(XTP_CW2CT(bstrVal));
}

BOOL CXTPPropExchangeXMLNode::ExchangeProp(LPCTSTR pszPropName, VARTYPE vtProp,
	void* pvProp, const void* pvDefault)
{
	if (m_bLoading)
	{
		if (!m_bUseDefaultOnLoad)
			pvDefault = NULL;

		if (!OnBeforeExchange())
			return CopyPropValue(vtProp, pvProp, pvDefault);

		CXTPDOMNodePtr xmlEntryPtr = GetEntryNode(pszPropName, FALSE);
		if (xmlEntryPtr == 0)
			return CopyPropValue(vtProp, pvProp, pvDefault);

		BSTR bstrValue = NULL;
		xmlEntryPtr->get_text(&bstrValue);

		if (vtProp == VT_BSTR)
		{
			PreformatString(bstrValue);
			*(BSTR*)pvProp = bstrValue;
			return TRUE;
		}

		CString strValue(BstrToString(bstrValue));
		if (bstrValue) SysFreeString(bstrValue);

		return ExchangePropStringHelper(strValue, vtProp, pvProp, pvDefault);
	}
	else
	{
		if (!m_bUseDefaultOnSave)
			pvDefault = NULL;

		if (IsSamePropValue(vtProp, pvProp, pvDefault))
			return TRUE;

		if (!OnBeforeExchange())
			return FALSE;

		CXTPDOMNodePtr xmlEntryPtr = GetEntryNode(pszPropName, TRUE);
		ASSERT(xmlEntryPtr != 0);

		if (vtProp == VT_BSTR)
		{
			xmlEntryPtr->put_text(*(BSTR*)pvProp);
			return TRUE;
		}

		CString strValue;
		if (!ExchangePropStringHelper(strValue, vtProp, pvProp, pvDefault))
			return FALSE;

		xmlEntryPtr->put_text(CT2BSTR(strValue));
	}
	return TRUE;

}
CString CXTPPropExchangeXMLNode::GetSectionName()
{
	if (!OnBeforeExchange())
		return _T("");

	BSTR bstrValue = NULL;
	m_xmlSectionNode->get_nodeName(&bstrValue);

	CString strValue(bstrValue);
	if (bstrValue) SysFreeString(bstrValue);

	return strValue;
}

BOOL CXTPPropExchangeXMLNode::IsSectionExists(LPCTSTR lpszSection)
{
	if (!OnBeforeExchange())
		return FALSE;

	CXTPDOMNodePtr xmlNodePtr;

	HRESULT hr = m_xmlSectionNode->selectSingleNode(CT2BSTR(lpszSection), &xmlNodePtr);

	if (FAILED(hr) || xmlNodePtr == NULL)
		return FALSE;

	return TRUE;
}

BOOL CXTPPropExchangeXMLNode::PutSection(CXTPPropExchangeXMLNode* pNode)
{
	if (!pNode)
		return FALSE;

	if (!OnBeforeExchange())
		return FALSE;

	if (!pNode || !pNode->OnBeforeExchange())
		return FALSE;

	ASSERT(m_xmlSectionNode != 0);

	if (FAILED(m_xmlSectionNode->appendChild(pNode->m_xmlSectionNode, NULL)))
		return FALSE;

	return TRUE;
}

CXTPPropExchangeXMLNode* CXTPPropExchangeXMLNode::Clone(BOOL bRecursive)
{
	if (!OnBeforeExchange())
		return NULL;

	ASSERT(m_xmlParentNode != 0);
	CXTPDOMNodePtr xmlNodePtr;

	VARIANT_BOOL vbRecursive;
	vbRecursive = bRecursive ? VARIANT_TRUE : VARIANT_FALSE;
	if (FAILED(m_xmlSectionNode->cloneNode(vbRecursive, &xmlNodePtr)))
		return NULL;

	CXTPPropExchangeXMLNode* pSection = (CXTPPropExchangeXMLNode*)GetSection(GetSectionName());

	pSection->m_bInitialized = TRUE;
	pSection->m_xmlSectionNode = xmlNodePtr;

	return pSection;
}

CXTPPropExchangeXMLNode* CXTPPropExchangeXMLNode::CreateSection(LPCTSTR pszSection)
{
	if (!pszSection || !OnBeforeExchange())
		return NULL;

	ASSERT(m_xmlParentNode != 0);
	CXTPDOMNodePtr xmlNodePtr;
	CXTPDOMElementPtr xmlElementPtr;

	if (m_xmlSectionNode == 0 || m_xmlDocPtr == 0)
		return NULL;

	m_xmlDocPtr->createElement(CT2BSTR(pszSection), &xmlElementPtr);
	if (xmlElementPtr == 0)
	{
		ASSERT(0);
		return NULL;
	}

	if (FAILED(m_xmlSectionNode->appendChild(xmlElementPtr, &xmlNodePtr)))
		return NULL;

	CXTPPropExchangeXMLNode* pSection = (CXTPPropExchangeXMLNode*)GetSection(pszSection);

	pSection->m_bInitialized = TRUE;
	pSection->m_xmlSectionNode = xmlNodePtr;

	return pSection;
}


void CXTPPropExchangeXMLNode::SetLoading(BOOL bLoading)
{
	m_bLoading = bLoading;
}

BOOL CXTPPropExchangeXMLNode::IsPropertyExists(LPCTSTR pszPropName)
{
	if (!OnBeforeExchange())
		return FALSE;

	if (GetEntryNode(pszPropName, FALSE) != NULL)
		return TRUE;

	return FALSE;
}


class CXTPPropExchangeXMLNode::CXMLEnumerator : public CXTPPropExchangeEnumerator
{
public:
	CXMLEnumerator::CXMLEnumerator(CXTPPropExchangeXMLNode* pContainer, LPCTSTR lpszSection)
		: CXTPPropExchangeEnumerator(pContainer, lpszSection)
	{
	}

public:
	CXTPDOMNodePtr GetProfileNode(POSITION pos)
	{
		CXTPDOMNodePtr xmlNodePtr;

		if (m_xmlNodes == NULL)
			return NULL;

		if (SUCCEEDED(m_xmlNodes->get_item((int)(INT_PTR)pos, &xmlNodePtr)))
			return xmlNodePtr;

		return 0;
	}

	POSITION GetPosition(DWORD dwCount = 0, BOOL /*bCompactMode*/ = TRUE)
	{
		m_nCount = dwCount;

		if (!m_pContainer)
			return NULL;

		if (m_pContainer->IsLoading())
		{
			if (!m_pContainer->OnBeforeExchange())
				return NULL;

			CString strNode;
			strNode.Format(_T("%s"), m_strSectionName.IsEmpty() ? _T("*") : (LPCTSTR)m_strSectionName);

			if (FAILED(((CXTPPropExchangeXMLNode*)m_pContainer)->m_xmlSectionNode->selectNodes(CT2BSTR(strNode), &m_xmlNodes)))
			{
				return NULL;
			}

			m_xmlNextNode = GetProfileNode(0);
			if (m_xmlNextNode == 0)
				return NULL;
		}

		return (POSITION)1;
	}

	CXTPPropExchange* GetNext(POSITION& pos)
	{
		CXTPPropExchangeXMLNode* pSection = (CXTPPropExchangeXMLNode*)m_pContainer->GetSection(m_strSectionName);

		CXTPDOMNodePtr xmlNodePtr;
		CXTPDOMElementPtr xmlElementPtr;


		if (m_pContainer->IsStoring())
		{
			ASSERT((DWORD)(DWORD_PTR)pos <= m_nCount);

			pSection->m_xmlDocPtr->createElement(CT2BSTR(m_strSectionName), &xmlElementPtr);
			ASSERT(xmlElementPtr != 0);
			if (xmlElementPtr)
			{
				((CXTPPropExchangeXMLNode*)m_pContainer)->m_xmlSectionNode->appendChild(xmlElementPtr, &xmlNodePtr);
			}

			pos++;
			if ((DWORD)(DWORD_PTR)pos > m_nCount) pos = NULL;
		}
		else
		{
			ASSERT(m_xmlNextNode != NULL);
			ASSERT(m_xmlNodes != NULL);

			xmlNodePtr = m_xmlNextNode;

			m_xmlNextNode = GetProfileNode(pos);
			if (m_xmlNextNode == NULL)
				pos = NULL;
			else
				pos++;

		}

		pSection->m_bInitialized = TRUE;
		pSection->m_xmlSectionNode = xmlNodePtr;

		return pSection;
	}

	int GetCount() const
	{
		long nCount = m_nCount;

		if (m_pContainer && m_pContainer->IsLoading() && SUCCEEDED(m_xmlNodes->get_length(&nCount)))
		{
			return nCount;
		}

		return nCount;
	}

protected:
	CXTPDOMNodePtr m_xmlNextNode;
	XTPXML::IXMLDOMNodeListPtr m_xmlNodes;
};



CXTPPropExchangeEnumerator* CXTPPropExchangeXMLNode::GetEnumerator(LPCTSTR lpszSection)
{
	if (IsStoring() || !IsPropertyExists(_T("Count")))
		return new CXMLEnumerator(this, lpszSection);

	return new CXTPPropExchangeEnumerator(this, lpszSection);
}

void CXTPPropExchangeXMLNode::SetEncoding(LPCTSTR lpszEncoding)
{
	m_strEncoding = lpszEncoding;
}

#endif
