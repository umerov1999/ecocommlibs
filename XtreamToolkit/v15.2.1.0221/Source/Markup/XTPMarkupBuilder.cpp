// XTPMarkupBuilder.cpp: implementation of the CXTPMarkupBuilder class.
//
// This file is a part of the XTREME TOOLKIT PRO MFC class library.
// (c)1998-2012 Codejock Software, All Rights Reserved.
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

#include "stdafx.h"

#include "Common/XTPVc80Helpers.h"

#include "XTPMarkupBuilder.h"
#include "XTPMarkupParser.h"
#include "XTPMarkupObject.h"
#include "XTPMarkupInputElement.h"
#include "XTPMarkupUIElement.h"
#include "XTPMarkupFrameworkElement.h"
#include "XTPMarkupControl.h"

#include "XTPMarkupDrawingContext.h"
#include "XTPMarkupContext.h"
#include "XTPMarkupTextBlock.h"
#include "Text/XTPMarkupInline.h"
#include "Text/XTPMarkupRun.h"
#include "XTPMarkupScrollViewer.h"
#include "XTPMarkupResourceDictionary.h"
#include "XTPMarkupShape.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

class CXTPMarkupObjectProperty : public CXTPMarkupObject
{
	DECLARE_MARKUPCLASS(CXTPMarkupObjectProperty)
public:
	CXTPMarkupObjectProperty(CXTPMarkupDependencyProperty* pProperty = 0)
	{
		m_pProperty = pProperty;
		m_pContent = NULL;
		m_pCollection = NULL;
	}

	~CXTPMarkupObjectProperty()
	{
		MARKUP_RELEASE(m_pContent);
		MARKUP_RELEASE(m_pCollection);
	}

	virtual void SetContentObject(CXTPMarkupBuilder* pBuilder, CXTPMarkupObject* pContent)
	{
		CXTPMarkupType* pContentType = m_pProperty->GetPropetyType();

		if (pContent->IsKindOf(pContentType))
		{
			m_pContent = pContent;
		}
		else if (IsStringObject(pContent))
		{
			m_pContent = pContent;
		}
		else
		{
			if (m_pContent == NULL)
			{
				m_pCollection = pBuilder->GetMarkupContext()->CreateMarkupObject(m_pProperty->GetPropetyType());
				if (m_pCollection) m_pCollection->SetContentObject(pBuilder, pContent);

				m_pContent = m_pCollection;
				m_pCollection = NULL;
			}
			else
			{
				m_pContent->SetContentObject(pBuilder, pContent);
			}
		}
	}

	CXTPMarkupObject* FindResource(const CXTPMarkupObject* pKey) const
	{
		if (m_pContent)
			return CXTPMarkupResourceDictionary::FindResource(m_pContent, pKey);

		return NULL;
	}

	CXTPMarkupObject* m_pContent;
	CXTPMarkupObject* m_pCollection;
	CXTPMarkupDependencyProperty* m_pProperty;
};

IMPLEMENT_MARKUPCLASS(NULL, CXTPMarkupObjectProperty, CXTPMarkupObject)

void CXTPMarkupObjectProperty::RegisterMarkupClass()
{

}

class CXTPMarkupColorKey : public CXTPMarkupObject
{
	DECLARE_MARKUPCLASS(CXTPMarkupColorKey);

public:
	CXTPMarkupColorKey(int nIndex = 0)
	{
		m_nIndex = nIndex;
	}

public:
	int m_nIndex;
};

IMPLEMENT_MARKUPCLASS(NULL, CXTPMarkupColorKey, CXTPMarkupObject)

void CXTPMarkupColorKey::RegisterMarkupClass()
{

}

class CXTPMarkupBrushKey : public CXTPMarkupObject
{
	DECLARE_MARKUPCLASS(CXTPMarkupBrushKey);

public:
	CXTPMarkupBrushKey(int nIndex = 0)
	{
		m_nIndex = nIndex;
	}

public:
	int m_nIndex;
};

IMPLEMENT_MARKUPCLASS(NULL, CXTPMarkupBrushKey, CXTPMarkupObject)

void CXTPMarkupBrushKey::RegisterMarkupClass()
{

}

//////////////////////////////////////////////////////////////////////////
// CXTPMarkupBuilder::CStaticExtension

class CXTPMarkupBuilder::CStaticExtension
{
public:
	CStaticExtension();
	~CStaticExtension();
public:
	CXTPMarkupObject* ProvideValue(CXTPMarkupBuilder* pBuilder, LPCWSTR lpszValue);

public:
	CXTPMarkupObject* GetSystemColor(LPCWSTR lpszValue);
	CXTPMarkupObject* GetSystemBrush(LPCWSTR lpszValue);
	CXTPMarkupObject* GetSystemColor(int nIndex);
	CXTPMarkupObject* GetSystemBrush(int nIndex);
	CXTPMarkupObject* GetSystemColorKey(LPCWSTR lpszValue);
	CXTPMarkupObject* GetSystemBrushKey(LPCWSTR lpszValue);
	int GetSystemIndex(LPCWSTR lpszValue, int nLength);

private:
	CXTPMarkupBrush* m_pBrushes[31];
	CXTPMarkupColor* m_pColors[31];

	CXTPMarkupObject* m_pBrushKeys[31];
	CXTPMarkupObject* m_pColorKeys[31];

};

CXTPMarkupBuilder::CStaticExtension::CStaticExtension()
{
	memset(m_pBrushes, 0, sizeof(m_pBrushes));
	memset(m_pColors, 0, sizeof(m_pColors));
	memset(m_pBrushKeys, 0, sizeof(m_pBrushKeys));
	memset(m_pColorKeys, 0, sizeof(m_pColorKeys));
}

CXTPMarkupBuilder::CStaticExtension::~CStaticExtension()
{
	for (int i = 0; i < 31; i++)
	{
		MARKUP_RELEASE(m_pBrushes[i]);
		MARKUP_RELEASE(m_pColors[i]);
		MARKUP_RELEASE(m_pBrushKeys[i]);
		MARKUP_RELEASE(m_pColorKeys[i]);
	}
}

int CXTPMarkupBuilder::CStaticExtension::GetSystemIndex(LPCWSTR lpszValue, int nLength)
{
	if (wcsncmp(L"ActiveBorder", lpszValue, nLength) == 0) return 10;
	if (wcsncmp(L"ActiveCaption", lpszValue, nLength) == 0) return 2;
	if (wcsncmp(L"ActiveCaptionText", lpszValue, nLength) == 0) return 9;
	if (wcsncmp(L"AppWorkspace", lpszValue, nLength) == 0) return 12;
	if (wcsncmp(L"Control", lpszValue, nLength) == 0) return 15;
	if (wcsncmp(L"ControlDark", lpszValue, nLength) == 0) return 0x10;
	if (wcsncmp(L"ControlDarkDark", lpszValue, nLength) == 0) return 0x15;
	if (wcsncmp(L"ControlLight", lpszValue, nLength) == 0) return 0x16;
	if (wcsncmp(L"ControlLightLight", lpszValue, nLength) == 0) return 20;
	if (wcsncmp(L"ControlText", lpszValue, nLength) == 0) return 0x12;
	if (wcsncmp(L"Desktop", lpszValue, nLength) == 0) return 1;
	if (wcsncmp(L"GradientActiveCaption", lpszValue, nLength) == 0) return 0x1b;
	if (wcsncmp(L"GradientInactiveCaption", lpszValue, nLength) == 0) return 0x1c;
	if (wcsncmp(L"GrayText", lpszValue, nLength) == 0) return 0x11;
	if (wcsncmp(L"Highlight", lpszValue, nLength) == 0) return 13;
	if (wcsncmp(L"HighlightText", lpszValue, nLength) == 0) return 14;
	if (wcsncmp(L"HotTrack", lpszValue, nLength) == 0) return 0x1a;
	if (wcsncmp(L"InactiveBorder", lpszValue, nLength) == 0) return 11;
	if (wcsncmp(L"InactiveCaption", lpszValue, nLength) == 0) return 3;
	if (wcsncmp(L"InactiveCaptionText", lpszValue, nLength) == 0) return 0x13;
	if (wcsncmp(L"Info", lpszValue, nLength) == 0) return 0x18;
	if (wcsncmp(L"InfoText", lpszValue, nLength) == 0) return 0x17;
	if (wcsncmp(L"Menu", lpszValue, nLength) == 0) return 4;
	if (wcsncmp(L"MenuBar", lpszValue, nLength) == 0) return 30;
	if (wcsncmp(L"MenuHighlight", lpszValue, nLength) == 0) return 0x1d;
	if (wcsncmp(L"MenuText", lpszValue, nLength) == 0) return 7;
	if (wcsncmp(L"ScrollBar", lpszValue, nLength) == 0) return 0;
	if (wcsncmp(L"Window", lpszValue, nLength) == 0) return 5;
	if (wcsncmp(L"WindowFrame", lpszValue, nLength) == 0) return 6;
	if (wcsncmp(L"WindowText", lpszValue, nLength) == 0) return 8;

	return 0;
}

CXTPMarkupObject* CXTPMarkupBuilder::CStaticExtension::GetSystemColor(LPCWSTR lpszValue)
{
	int nLength = (int)wcslen(lpszValue);
	if (nLength < 20)
		return NULL;

	if (wcsncmp(lpszValue, L"SystemColors.", 13) != 0)
		return NULL;

	if (wcscmp(lpszValue + nLength - 5, L"Color") != 0)
		return NULL;

	int nIndex = GetSystemIndex(lpszValue + 13, nLength - 13 - 5);
	if (nIndex == 0)
		return NULL;

	return GetSystemColor(nIndex);
}

CXTPMarkupObject* CXTPMarkupBuilder::CStaticExtension::GetSystemColor(int nIndex)
{
	if (m_pColors[nIndex] == NULL)
	{
		m_pColors[nIndex] = new CXTPMarkupColor(GetSysColor(nIndex) | 0xFF000000);
	}

	MARKUP_ADDREF(m_pColors[nIndex]);
	return m_pColors[nIndex];
}

CXTPMarkupObject* CXTPMarkupBuilder::CStaticExtension::GetSystemColorKey(LPCWSTR lpszValue)
{
	int nLength = (int)wcslen(lpszValue);
	if (nLength < 20)
		return NULL;

	if (wcsncmp(lpszValue, L"SystemColors.", 13) != 0)
		return NULL;

	if (wcscmp(lpszValue + nLength - 8, L"ColorKey") != 0)
		return NULL;

	int nIndex = GetSystemIndex(lpszValue + 13, nLength - 13 - 8);
	if (nIndex == 0)
		return NULL;

	if (m_pColorKeys[nIndex] == NULL)
	{
		m_pColorKeys[nIndex] = new CXTPMarkupColorKey(nIndex);
	}

	MARKUP_ADDREF(m_pColorKeys[nIndex]);
	return m_pColorKeys[nIndex];
}

CXTPMarkupObject* CXTPMarkupBuilder::CStaticExtension::GetSystemBrushKey(LPCWSTR lpszValue)
{
	int nLength = (int)wcslen(lpszValue);
	if (nLength < 20)
		return NULL;

	if (wcsncmp(lpszValue, L"SystemColors.", 13) != 0)
		return NULL;

	if (wcscmp(lpszValue + nLength - 8, L"BrushKey") != 0)
		return NULL;

	int nIndex = GetSystemIndex(lpszValue + 13, nLength - 13 - 8);
	if (nIndex == 0)
		return NULL;

	if (m_pBrushKeys[nIndex] == NULL)
	{
		m_pBrushKeys[nIndex] = new CXTPMarkupBrushKey(nIndex);
	}

	MARKUP_ADDREF(m_pBrushKeys[nIndex]);
	return m_pBrushKeys[nIndex];
}

CXTPMarkupObject* CXTPMarkupBuilder::CStaticExtension::GetSystemBrush(LPCWSTR lpszValue)
{
	int nLength = (int)wcslen(lpszValue);
	if (nLength < 20)
		return NULL;

	if (wcsncmp(lpszValue, L"SystemColors.", 13) != 0)
		return NULL;

	if (wcscmp(lpszValue + nLength - 5, L"Brush") != 0)
		return NULL;

	int nIndex = GetSystemIndex(lpszValue + 13, nLength - 13 - 5);
	if (nIndex == 0)
		return NULL;

	return GetSystemBrush(nIndex);
}

CXTPMarkupObject* CXTPMarkupBuilder::CStaticExtension::GetSystemBrush(int nIndex)
{
	if (m_pBrushes[nIndex] == NULL)
	{
		m_pBrushes[nIndex] = new CXTPMarkupSolidColorBrush(GetSysColor(nIndex) | 0xFF000000);
	}

	MARKUP_ADDREF(m_pBrushes[nIndex]);
	return m_pBrushes[nIndex];
}

CXTPMarkupObject* CXTPMarkupBuilder::CStaticExtension::ProvideValue(CXTPMarkupBuilder* pBuilder, LPCWSTR lpszValue)
{
	CXTPMarkupObject* pObject = GetSystemColor(lpszValue);
	if (pObject)
		return pObject;

	pObject = GetSystemBrush(lpszValue);
	if (pObject)
		return pObject;

	pObject = GetSystemColorKey(lpszValue);
	if (pObject)
		return pObject;

	pObject = GetSystemBrushKey(lpszValue);
	if (pObject)
		return pObject;

	pBuilder->ThrowBuilderException(pBuilder->FormatString(_T("Cannot find the static member  '%ls'"), (LPCTSTR)lpszValue));
	return NULL;
}



//////////////////////////////////////////////////////////////////////////
// CXTPMarkupBuilder::CStaticResourceExtension

class CXTPMarkupBuilder::CStaticResourceExtension
{
public:
	CStaticResourceExtension();
	~CStaticResourceExtension();
public:
	CXTPMarkupObject* ProvideValue(CXTPMarkupBuilder* pBuilder, LPCWSTR lpszValue);
	CXTPMarkupObject* FindResources(CXTPMarkupObject* pElement, CXTPMarkupObject* pValue);
	CXTPMarkupObject* ResolveKey(CXTPMarkupBuilder* pBuilder, LPCWSTR lpszValue);

};

CXTPMarkupBuilder::CStaticResourceExtension::CStaticResourceExtension()
{
}

CXTPMarkupBuilder::CStaticResourceExtension::~CStaticResourceExtension()
{

}

CXTPMarkupObject* CXTPMarkupBuilder::CStaticResourceExtension::FindResources(CXTPMarkupObject* pElement, CXTPMarkupObject* pKey)
{
	if (pElement->IsKindOf(MARKUP_TYPE(CXTPMarkupFrameworkElement)))
	{
		CXTPMarkupObject* pValue = ((CXTPMarkupFrameworkElement*)pElement)->FindResource(pKey);
		if (pValue)
			return pValue;
	}

	if (pElement->IsKindOf(MARKUP_TYPE(CXTPMarkupFrameworkContentElement)))
	{
		CXTPMarkupObject* pValue = ((CXTPMarkupFrameworkContentElement*)pElement)->FindResource(pKey);
		if (pValue)
			return pValue;
	}

	if (pElement->IsKindOf(MARKUP_TYPE(CXTPMarkupStyle)))
	{
		CXTPMarkupObject* pValue = ((CXTPMarkupStyle*)pElement)->FindResource(pKey);
		if (pValue)
			return pValue;
	}

	if (pElement->IsKindOf(MARKUP_TYPE(CXTPMarkupObjectProperty)))
	{
		CXTPMarkupObject* pValue = ((CXTPMarkupObjectProperty*)pElement)->FindResource(pKey);
		if (pValue)
			return pValue;
	}

	return NULL;
}

CXTPMarkupObject* CXTPMarkupBuilder::CStaticResourceExtension::ResolveKey(CXTPMarkupBuilder* pBuilder, LPCWSTR lpszValue)
{
	while (*lpszValue == ' ' || *lpszValue == '\n' || *lpszValue == '\r')
		lpszValue++;

	if (lpszValue[0] == '{')
	{
		if (lpszValue[1] == '}')
		{
			lpszValue = lpszValue + 2;
		}
		else
		{
			return pBuilder->ResolveMarkupExtension(lpszValue + 1);
		}
	}

	return CXTPMarkupString::CreateValue(lpszValue);
}

CXTPMarkupObject* CXTPMarkupBuilder::CStaticResourceExtension::ProvideValue(CXTPMarkupBuilder* pBuilder, LPCWSTR lpszValue)
{
	CXTPMarkupObject* pKey = ResolveKey(pBuilder, lpszValue);
	if (!pKey)
		return NULL;

	POSITION pos = pBuilder->m_arrObjects.GetTailPosition();
	while (pos)
	{
		CXTPMarkupObject* pElement = pBuilder->m_arrObjects.GetPrev(pos);

		CXTPMarkupObject* pValue = FindResources(pElement, pKey);

		if (pValue)
		{
			MARKUP_RELEASE(pKey);
			MARKUP_ADDREF(pValue);
			return pValue;
		}
	}

	if (pKey->IsKindOf(MARKUP_TYPE(CXTPMarkupColorKey)))
	{
		CXTPMarkupObject* pValue = pBuilder->GetStaticExtension()->GetSystemColor(((CXTPMarkupColorKey*)pKey)->m_nIndex);
		MARKUP_RELEASE(pKey);
		return pValue;
	}

	if (pKey->IsKindOf(MARKUP_TYPE(CXTPMarkupBrushKey)))
	{
		CXTPMarkupObject* pValue = pBuilder->GetStaticExtension()->GetSystemBrush(((CXTPMarkupBrushKey*)pKey)->m_nIndex);
		MARKUP_RELEASE(pKey);
		return pValue;
	}

	MARKUP_RELEASE(pKey);

	pBuilder->ThrowBuilderException(pBuilder->FormatString(_T("Cannot find resource named '%ls'. Resource names are case sensitive"), (LPCTSTR)lpszValue));

	return 0;
}

//////////////////////////////////////////////////////////////////////////
// CXTPMarkupBuilder

CXTPMarkupBuilder::CStaticExtension* CXTPMarkupBuilder::GetStaticExtension()
{
	if (m_pStaticExtension == NULL)
	{
		m_pStaticExtension = new CStaticExtension();
	}

	return m_pStaticExtension ;
}

CXTPMarkupBuilder::CStaticResourceExtension* CXTPMarkupBuilder::GetStaticResourceExtension()
{
	if (m_pStaticResourceExtension == NULL)
	{
		m_pStaticResourceExtension = new CStaticResourceExtension();
	}

	return m_pStaticResourceExtension ;
}


//////////////////////////////////////////////////////////////////////////
// CXTPMarkupBuilderException

IMPLEMENT_DYNAMIC(CXTPMarkupBuilderException, CException)

CXTPMarkupBuilderException::CXTPMarkupBuilderException(LPCTSTR lpszError)
{
	m_bInitialized = TRUE;
	m_bLoaded = TRUE;

	m_szMessage[255] = 0;
	STRNCPY_S(m_szMessage, 256, lpszError, 255);
}

BOOL CXTPMarkupBuilderException::GetErrorMessage(LPTSTR lpszError, UINT nMaxError, PUINT pnHelpContext)
{
	ASSERT(lpszError != NULL && AfxIsValidString(lpszError, nMaxError));
	if (!lpszError)
		return FALSE;

	if (pnHelpContext != NULL)
		*pnHelpContext = 0;

	// if we didn't load our string (eg, we're a console app)
	lstrcpyn(lpszError, m_szMessage, nMaxError);

	return TRUE;
}


//////////////////////////////////////////////////////////////////////////
// CStringBuilder

class CXTPMarkupBuilder::CStringBuilder
{
public:
	CStringBuilder();
	~CStringBuilder();

public:
	void Add(LPCWSTR lpszWord);
	void Empty();

	int GetLength() const
	{
		return m_nLength;
	}

	LPCWSTR GetBuffer()
	{
		ASSERT(m_lpszData);
		ASSERT(m_nLength < m_nAlloc);
		m_lpszData[m_nLength] = 0;

		return m_lpszData;
	}

protected:
	int m_nAlloc;
	int m_nLength;
	LPWSTR m_lpszData;
};

CXTPMarkupBuilder::CStringBuilder::CStringBuilder()
{
	m_nAlloc = 0;
	m_nLength = 0;
	m_lpszData = NULL;
}

CXTPMarkupBuilder::CStringBuilder::~CStringBuilder()
{
	if (m_lpszData)
	{
		delete[] m_lpszData;
	}
}

void CXTPMarkupBuilder::CStringBuilder::Add(LPCWSTR lpszWord)
{
	int nWordLength = (int)wcslen(lpszWord);

	if (m_nAlloc < m_nLength + nWordLength + 1)
	{
		int nAlloc = m_nLength + max(256, nWordLength + 1);
		LPWSTR lpszData = new WCHAR[nAlloc];

		if (m_nLength > 0) memcpy(lpszData, m_lpszData, m_nLength * sizeof(WCHAR));
		if (m_lpszData) delete[] m_lpszData;

		m_lpszData = lpszData;
		m_nAlloc = nAlloc;
	}

	memcpy(m_lpszData + m_nLength, lpszWord, nWordLength * sizeof(WCHAR));
	m_nLength += nWordLength;
}

void CXTPMarkupBuilder::CStringBuilder::Empty()
{
	m_nLength = 0;
}
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXTPMarkupBuilder::CXTPMarkupBuilder(CXTPMarkupContext* pContext)
{
	m_pMarkupContext = pContext;
	ASSERT(m_pMarkupContext);

	m_pStaticExtension = NULL;
	m_pStaticResourceExtension = NULL;
}

CXTPMarkupBuilder::~CXTPMarkupBuilder()
{
	SAFE_DELETE(m_pStaticExtension);
	SAFE_DELETE(m_pStaticResourceExtension);
}


inline LPWSTR DupString(LPCWSTR lpszString)
{
	int nLen = (int)wcslen(lpszString);
	LPWSTR lpcw = new WCHAR[nLen + 1];
	MEMCPY_S(lpcw, lpszString, (nLen + 1) * sizeof(WCHAR));
	return lpcw;
}

CXTPMarkupObject* CXTPMarkupBuilder::CreateObject(LPCWSTR lpszTag)
{
	CXTPMarkupType* pType = CXTPMarkupType::LookupTag(lpszTag);
	if (!pType)
		return NULL;

	CXTPMarkupObject* pMarkupObject = m_pMarkupContext->CreateMarkupObject(pType);

	if (!pMarkupObject)
	{
		return NULL;
	}

	return pMarkupObject;
}

CString CXTPMarkupBuilder::FormatString(LPCTSTR lpszFormat, LPCTSTR lpszParameter)
{
	CString str;
	str.Format(lpszFormat, lpszParameter);
	return str;
}

CString CXTPMarkupBuilder::FormatString(LPCTSTR lpszFormat, LPCTSTR lpszParameter1, LPCTSTR lpszParameter2)
{
	CString str;
	str.Format(lpszFormat, lpszParameter1, lpszParameter2);
	return str;
}

CString CXTPMarkupBuilder::FormatString(LPCTSTR lpszFormat, LPCTSTR lpszParameter1, LPCTSTR lpszParameter2, LPCTSTR lpszParameter3)
{
	CString str;
	str.Format(lpszFormat, lpszParameter1, lpszParameter2, lpszParameter3);
	return str;
}

CXTPMarkupDependencyProperty* CXTPMarkupBuilder::FindProperty(LPCWSTR lpszTag, LPCWSTR lpszAttribute)
{
	CXTPMarkupType* pType = CXTPMarkupType::LookupTag(lpszTag);
	if (!pType)
		return NULL;

	return CXTPMarkupDependencyProperty::FindProperty(pType, lpszAttribute);
}

CXTPMarkupDependencyProperty* CXTPMarkupBuilder::FindProperty(CXTPMarkupType* pRuntimeClass, LPCWSTR lpszAttribute)
{
	LPCWSTR lpszAttributeDot = wcschr(lpszAttribute, L'.');

	if (lpszAttributeDot != NULL)
	{
		*(LPWSTR)lpszAttributeDot = 0;
		CXTPMarkupDependencyProperty* pProperty = FindProperty(lpszAttribute, lpszAttributeDot + 1);
		*(LPWSTR)lpszAttributeDot = '.';

		return pProperty;
	}

	if (!pRuntimeClass)
		return NULL;

	return CXTPMarkupDependencyProperty::FindProperty(pRuntimeClass, lpszAttribute);
}

CXTPMarkupObject* CXTPMarkupBuilder::ConvertValue(CXTPMarkupDependencyProperty* pProperty, CXTPMarkupObject* pValue)
{
	CXTPMarkupObject* pNewValue = NULL;

	if (pProperty->GetMetadata() && pProperty->GetMetadata()->m_pConverter)
	{
		pNewValue = (*pProperty->GetMetadata()->m_pConverter)(this, pValue);
	}

	if (pNewValue == NULL)
	{
		CXTPMarkupAutoPtr pConverter(GetMarkupContext()->CreateMarkupObject(pProperty->GetPropetyType()));
		pNewValue = pConverter->ConvertFrom(this, pValue);
	}

	if (!pNewValue)
	{
		ThrowBuilderException(CXTPMarkupBuilder::FormatString(_T("Cannot convert string in attribute '%ls' to object of type '%ls'"),
			(LPCTSTR)pProperty->GetName(), (LPCTSTR)pProperty->GetPropetyType()->m_lpszClassName));
	}
	return pNewValue;
}

CXTPMarkupObject* CXTPMarkupBuilder::CreateTypeKey(LPCWSTR lpszType)
{
	CXTPMarkupType* pType = CXTPMarkupType::LookupTag(lpszType);
	if (!pType)
	{
		ThrowBuilderException(FormatString(_T("Cannot find type '%ls'"), (LPCTSTR)lpszType));
		return NULL;
	}

	pType->AddRef();
	return pType;
}

CXTPMarkupObject* CXTPMarkupBuilder::ResolveMarkupExtension(LPCWSTR lpszValue)
{
	int nLength = (int)wcslen(lpszValue);

	if (lpszValue[nLength - 1] != '}')
	{
		ThrowBuilderException(_T("Markup extension expressions must end with '}'"));
	}

	*((LPWSTR)lpszValue + nLength - 1) = '\0';

	if (wcsncmp(lpszValue, L"x:Static ", 9) == 0)
	{
		return GetStaticExtension()->ProvideValue(this, lpszValue + 9);
	}

	if (wcsncmp(lpszValue, L"StaticResource ", 15) == 0)
	{
		return GetStaticResourceExtension()->ProvideValue(this, lpszValue + 15);
	}

	if (wcsncmp(lpszValue, L"DynamicResource ", 16) == 0)
	{
		return GetStaticResourceExtension()->ProvideValue(this, lpszValue + 16);
	}

	if (wcsncmp(lpszValue, L"x:Type ", 7) == 0)
	{
		return CreateTypeKey(lpszValue + 7);
	}

	ThrowBuilderException(FormatString(_T("The tag '%ls' does not exist in XML namespace"), (LPCTSTR)lpszValue));

	return NULL;
}

CXTPMarkupUIElement* CXTPMarkupBuilder::Parse(CXTPMarkupParser* pParser)
{
	m_strLastError.Empty();

	if (!pParser->FindFirstTag())
	{
		m_strLastError = _T("Data at the root level is invalid");
		return NULL;
	}

	ASSERT(m_arrObjects.IsEmpty());

	CXTPMarkupObject* pRootElement = NULL;
	CXTPMarkupObject* pContentObject = NULL;

	CStringBuilder strBody;

	BOOL bBodySpace = FALSE;

	TRY
	{
		for (;;)
		{
			CXTPMarkupParser::TokenType t = pParser->GetNextToken();

			switch (t)
			{
			case CXTPMarkupParser::tokenError:
				{
					ThrowBuilderException(FormatString(_T("%ls"), (LPCTSTR)pParser->GetValue()));
				}
				break;

			case CXTPMarkupParser::tokenEof:
				{
					if (pRootElement == NULL)
						ThrowBuilderException(_T("Root element is missing"));

					if (!m_arrObjects.IsEmpty())
					{
						ThrowBuilderException(FormatString(_T("Unexpected end of file has occurred. The following elements are not closed: '%ls'"), (LPCTSTR)m_arrObjects.GetTail()->m_lpMarkupTag));
					}

				}
				goto FINISH;

			case CXTPMarkupParser::tokenTagStart:
				{
					if (m_arrObjects.IsEmpty() && pRootElement)
					{
						ThrowBuilderException(_T("There are multiple root elements"));
					}

					if (!m_arrObjects.IsEmpty())
					{
						CXTPMarkupObject* pLastObject = m_arrObjects.GetTail();
						if (strBody.GetLength() > 0)
						{
							if (bBodySpace) strBody.Add(L" ");
							pContentObject = CXTPMarkupString::CreateValue(strBody.GetBuffer(), strBody.GetLength());
							pLastObject->SetContentObject(this, pContentObject);
							pContentObject = NULL;
						}
						else if (bBodySpace && pLastObject->HasContentObject() && pLastObject->AllowWhiteSpaceContent())
						{
							pContentObject = CXTPMarkupString::CreateValue(L" ", 1);
							pLastObject->SetContentObject(this, pContentObject);
							pContentObject = NULL;
						}
					}

					strBody.Empty();
					bBodySpace = FALSE;

					LPCWSTR lpszTagName = pParser->GetTagName();
					LPCWSTR lpszAttributeDot = wcschr(lpszTagName, L'.');

					CXTPMarkupObject* pObject = NULL;

					if (lpszAttributeDot != NULL)
					{
						*(LPWSTR)lpszAttributeDot = 0;
						CXTPMarkupDependencyProperty* pProperty = FindProperty(lpszTagName, lpszAttributeDot + 1);
						*(LPWSTR)lpszAttributeDot = '.';

						if (!pProperty)
						{
							ThrowBuilderException(FormatString(_T("The tag '%ls' does not exist in XML namespace"), (LPCTSTR)lpszTagName));
						}

						if (m_arrObjects.IsEmpty())
						{
							ThrowBuilderException(FormatString(_T("The tag '%ls' does not exist in XML namespace"), (LPCTSTR)lpszTagName));
						}

						/*if (!m_arrObjects.GetTail()->IsPropertySupported(pProperty))
						{
							ThrowBuilderException(FormatString(_T("The tag '%ls' does not exist in XML namespace"), (LPCTSTR)lpszTagName));
						}*/

						pObject = new CXTPMarkupObjectProperty(pProperty);

						m_arrObjects.AddTail(pObject);
					}
					else
					{
						pObject = CreateObject(lpszTagName);
						if (!pObject)
						{
							ThrowBuilderException(FormatString(_T("The tag '%ls' does not exist in XML namespace"), (LPCTSTR)lpszTagName));
						}

						m_arrObjects.AddTail(pObject);
					}
					pObject->m_lpMarkupTag = DupString(lpszTagName);
				}
				break;

			case CXTPMarkupParser::tokenTagEnd:
				{
					LPCWSTR lpszTagName = pParser->GetTagName();

					if (m_arrObjects.IsEmpty())
					{
						ThrowBuilderException(_T("Unexpected end tag"));
					}

					pRootElement = m_arrObjects.RemoveTail();

					if (wcscmp(pRootElement->m_lpMarkupTag, lpszTagName) != 0)
					{
						ThrowBuilderException(FormatString(_T("The '%ls' start tag does not match the end tag of '%ls'"), (LPCTSTR)pRootElement->m_lpMarkupTag, (LPCTSTR)lpszTagName));
					}

					if (strBody.GetLength() > 0)
					{
						pContentObject = CXTPMarkupString::CreateValue(strBody.GetBuffer(), strBody.GetLength());
						pRootElement->SetContentObject(this, pContentObject);
						pContentObject = NULL;
					}
					strBody.Empty();
					bBodySpace = FALSE;

					if (pRootElement->IsKindOf(MARKUP_TYPE(CXTPMarkupObjectProperty)))
					{
						CXTPMarkupObjectProperty* pObjectProperty = (CXTPMarkupObjectProperty*)pRootElement;

						if (pObjectProperty->m_pContent == NULL)
							ThrowBuilderException(FormatString(_T("'%ls' property does not have a value"), (LPCTSTR)pObjectProperty->m_pProperty->GetName()));

						ASSERT(!m_arrObjects.IsEmpty());

						pRootElement = pObjectProperty->m_pContent;
						CXTPMarkupDependencyProperty* pProperty = pObjectProperty->m_pProperty;

						MARKUP_ADDREF(pObjectProperty->m_pContent);
						MARKUP_RELEASE(pObjectProperty);

						m_arrObjects.GetTail()->SetPropertyObject(this, pProperty, pRootElement);
						pRootElement = NULL;
					}
					else if (!m_arrObjects.IsEmpty())
					{
						m_arrObjects.GetTail()->SetContentObject(this, pRootElement);
						pRootElement = NULL;
					}

				}
				break;

			case CXTPMarkupParser::tokenAttribute:
				{
					LPCWSTR lpszAtributeName = pParser->GetAttributeName();

					if (m_arrObjects.IsEmpty())
					{
						ThrowBuilderException(_T("Unexpected error"));
					}

					if (wcsncmp(lpszAtributeName, L"xmlns", 5) == 0)
						break;

					CXTPMarkupObject* pObject = m_arrObjects.GetTail();

					CXTPMarkupDependencyProperty* pProperty = FindProperty(pObject->GetType(), lpszAtributeName);

					if (!pProperty)
					{
						ThrowBuilderException(FormatString(_T("The property '%ls' does not exist in XML namespace"), (LPCTSTR)lpszAtributeName));
					}

					/*if (!pObject->IsPropertySupported(pProperty))
					{
						ThrowBuilderException(FormatString(_T("The property '%ls' does not exist in XML namespace"), (LPCTSTR)lpszAtributeName));
					}*/

					pContentObject = NULL;

					LPCWSTR lpszValue = pParser->GetValue();
					if (lpszValue[0] == '{')
					{
						if (lpszValue[1] == '}')
						{
							lpszValue = lpszValue + 2;
						}
						else
						{
							pContentObject = ResolveMarkupExtension(lpszValue + 1);
						}
					}

					if (!pContentObject) pContentObject = CXTPMarkupString::CreateValue(lpszValue);
					pObject->SetPropertyObject(this, pProperty, pContentObject);
					pContentObject = NULL;
				}
				break;

			case CXTPMarkupParser::tokenSpace:
				bBodySpace = TRUE;
				break;

			case CXTPMarkupParser::tokenWord:
				{
					if (m_arrObjects.IsEmpty())
					{
						ThrowBuilderException(_T("Data at the root level is invalid"));
					}

					if (bBodySpace)
					{
						if (strBody.GetLength() > 0 || m_arrObjects.GetTail()->HasContentObject())
							strBody.Add(L" ");
					}
					bBodySpace = FALSE;

					strBody.Add(pParser->GetValue());
				}
				break;

			case CXTPMarkupParser::tokenPIStart:
				break;

			case CXTPMarkupParser::tokenPIEnd:
				{
					LPCWSTR lpszValue = pParser->GetValue();
					LPCWSTR lpszEncoding = wcsstr(lpszValue, L"encoding");
					if (lpszEncoding)
					{
						BOOL bUTF8 = wcsncmp(lpszEncoding + 10, L"utf-8", 5) == 0;
						if (bUTF8)
						{
							pParser->SetEncoding(CP_UTF8);
						}
					}
				}
				break;

			case CXTPMarkupParser::tokenCommentStart:
				break;

			case CXTPMarkupParser::tokenCommentEnd:
				break;

			case CXTPMarkupParser::tokenData:
				break;
			}
		}
	}
	CATCH (CXTPMarkupBuilderException, e)
	{
		//e->ReportError(MB_ICONEXCLAMATION | MB_OK);
		TCHAR szErrorMessage[512];
		UINT nHelpContext;

		if (e->GetErrorMessage(szErrorMessage, 512, &nHelpContext))
		{
			m_strLastError.Format(_T("%s. Line %i, position %i"), szErrorMessage, pParser->GetLine() + 1, pParser->GetPosition() + 1);
			TRACE(m_strLastError);
		}

		MARKUP_RELEASE(pRootElement);
	}
	END_CATCH

FINISH:

	MARKUP_RELEASE(pContentObject);

	while (!m_arrObjects.IsEmpty())
	{
		m_arrObjects.RemoveTail()->Release();
	}

	if (pRootElement && !pRootElement->IsKindOf(MARKUP_TYPE(CXTPMarkupUIElement)))
	{
		m_strLastError = _T("Root element have to be 'CXTPMarkupUIElement' type");
		TRACE(m_strLastError);

		MARKUP_RELEASE(pRootElement);
		return NULL;
	}

	if (pRootElement)
	{
		((CXTPMarkupUIElement*)pRootElement)->Apply();
	}

	return (CXTPMarkupUIElement*)pRootElement;
}


BOOL CXTPMarkupBuilder::ColorStringToKnownColor(LPCWSTR lpszValue, COLORREF& clr)
{
	int nLength = (int)wcslen(lpszValue);

	WCHAR ch = lpszValue[0];
	if (ch >= 'a' && ch <= 'z')
		ch = (TCHAR)(ch + 'A' - 'a');
	else if (ch < 'A' || ch > 'Z')
		return FALSE;

	switch (nLength)
	{
	case 3:
		if (ch == _T('R') && _wcsicmp(lpszValue, L"RED") == 0) return clr = 0xff0000ff;
		if (ch == _T('T') && _wcsicmp(lpszValue, L"TAN") == 0) return clr = 0xff8cb4d2;
		return FALSE;
	case 4:
		switch (ch)
		{
		case _T('A'): if (_wcsicmp(lpszValue, L"AQUA") == 0) return clr = 0xffffff00; return FALSE;
		case _T('B'): if (_wcsicmp(lpszValue, L"BLUE") == 0) return clr = 0xffff0000; return FALSE;
		case _T('C'): if (_wcsicmp(lpszValue, L"CYAN") == 0) return clr = 0xffffff00; return FALSE;
		case _T('G'):
			if (_wcsicmp(lpszValue, L"GOLD") == 0) return clr = 0xff00d7ff;
			if (_wcsicmp(lpszValue, L"GRAY") == 0) return clr = 0xff808080;
			return FALSE;
		case _T('L'): if (_wcsicmp(lpszValue, L"LIME") == 0) return clr = 0xff00ff00; return FALSE;
		case _T('N'): if (_wcsicmp(lpszValue, L"NAVY") == 0) return clr = 0xff800000; return FALSE;
		case _T('P'):
			if (_wcsicmp(lpszValue, L"PERU") == 0) return clr = 0xff3f85cd;
			if (_wcsicmp(lpszValue, L"PINK") == 0) return clr = 0xffcbc0ff;
			if (_wcsicmp(lpszValue, L"PLUM") == 0) return clr = 0xffdda0dd;
			return FALSE;
		case _T('S'): if (_wcsicmp(lpszValue, L"SNOW") == 0) return clr = 0xfffafaff; return FALSE;
		case _T('T'): if (_wcsicmp(lpszValue, L"TEAL") == 0) return clr = 0xff808000; return FALSE;
		}
		return FALSE;
	case 5:
		switch (ch)
		{
		case _T('A'): if (_wcsicmp(lpszValue, L"AZURE") == 0) return clr = 0xfffffff0; return FALSE;
		case _T('B'):
			if (_wcsicmp(lpszValue, L"BEIGE") == 0) return clr = 0xffdcf5f5;
			if (_wcsicmp(lpszValue, L"BLACK") == 0) return clr = 0xff000000;
			if (_wcsicmp(lpszValue, L"BROWN") == 0) return clr = 0xff2a2aa5;
			return FALSE;
		case _T('C'): if (_wcsicmp(lpszValue, L"CORAL") == 0) return clr = 0xff507fff; return FALSE;
		case _T('G'): if (_wcsicmp(lpszValue, L"GREEN") == 0) return clr = 0xff008000; return FALSE;
		case _T('I'): if (_wcsicmp(lpszValue, L"IVORY") == 0) return clr = 0xfff0ffff; return FALSE;
		case _T('K'): if (_wcsicmp(lpszValue, L"KHAKI") == 0) return clr = 0xff8ce6f0; return FALSE;
		case _T('L'): if (_wcsicmp(lpszValue, L"LINEN") == 0) return clr = 0xffe6f0fa; return FALSE;
		case _T('O'): if (_wcsicmp(lpszValue, L"OLIVE") == 0) return clr = 0xff008080; return FALSE;
		case _T('W'):
			if (_wcsicmp(lpszValue, L"WHEAT") == 0) return clr = 0xffb3def5;
			if (_wcsicmp(lpszValue, L"WHITE") == 0) return clr = 0xffffffff;
			return FALSE;
		}
		return FALSE;
	case 6:
		switch (ch)
		{
		case _T('M'): if (_wcsicmp(lpszValue, L"MAROON") == 0) return clr = 0xff000080; return FALSE;
		case _T('O'):
			if (_wcsicmp(lpszValue, L"ORANGE") == 0) return clr = 0xff00a5ff;
			if (_wcsicmp(lpszValue, L"ORCHID") == 0) return clr = 0xffd670da;
			return FALSE;
		case _T('P'): if (_wcsicmp(lpszValue, L"PURPLE") == 0) return clr = 0xff800080; return FALSE;
		case _T('S'):
			if (_wcsicmp(lpszValue, L"SALMON") == 0) return clr = 0xff7280fa;
			if (_wcsicmp(lpszValue, L"SIENNA") == 0) return clr = 0xff2d52a0;
			if (_wcsicmp(lpszValue, L"SILVER") == 0) return clr = 0xffc0c0c0;
			return FALSE;
		case _T('T'): if (_wcsicmp(lpszValue, L"TOMATO") == 0) return clr = 0xff4763ff; return FALSE;
		case _T('V'): if (_wcsicmp(lpszValue, L"VIOLET") == 0) return clr = 0xffee82ee; return FALSE;
		case _T('Y'): if (_wcsicmp(lpszValue, L"YELLOW") == 0) return clr = 0xff00ffff; return FALSE;
		case _T('I'): if (_wcsicmp(lpszValue, L"INDIGO") == 0) return clr = 0xff82004b; return FALSE;
		case _T('B'): if (_wcsicmp(lpszValue, L"BISQUE") == 0) return clr = 0xffc4e4ff; return FALSE;
		}
		return FALSE;
	case 7:
		switch (ch)
		{
		case _T('C'): if (_wcsicmp(lpszValue, L"CRIMSON") == 0) return clr = 0xff3c14dc; return FALSE;
		case _T('D'):
			if (_wcsicmp(lpszValue, L"DARKRED") == 0) return clr = 0xff00008b;
			if (_wcsicmp(lpszValue, L"DIMGRAY") == 0) return clr = 0xff696969;
			return FALSE;
		case _T('F'): if (_wcsicmp(lpszValue, L"FUCHSIA") == 0) return clr = 0xffff00ff; return FALSE;
		case _T('H'): if (_wcsicmp(lpszValue, L"HOTPINK") == 0) return clr = 0xffb469ff; return FALSE;
		case _T('M'): if (_wcsicmp(lpszValue, L"MAGENTA") == 0) return clr = 0xffff00ff; return FALSE;
		case _T('O'): if (_wcsicmp(lpszValue, L"OLDLACE") == 0)return clr = 0xffe6f5fd; return FALSE;
		case _T('S'): if (_wcsicmp(lpszValue, L"SKYBLUE") == 0) return clr = 0xffebce87; return FALSE;
		case _T('T'): if (_wcsicmp(lpszValue, L"THISTLE") == 0) return clr = 0xffd8bfd8; return FALSE;
		}
		return FALSE;
	case 8:
		switch (ch)
		{
		case _T('C'): if (_wcsicmp(lpszValue, L"CORNSILK") == 0) return clr = 0xffdcf8ff; return FALSE;
		case _T('D'):
			if (_wcsicmp(lpszValue, L"DARKBLUE") == 0) return clr = 0xff8b0000;
			if (_wcsicmp(lpszValue, L"DARKCYAN") == 0) return clr = 0xff8b8b00;
			if (_wcsicmp(lpszValue, L"DARKGRAY") == 0) return clr = 0xffa9a9a9;
			if (_wcsicmp(lpszValue, L"DEEPPINK") == 0) return clr = 0xff9314ff;
			return FALSE;
		case _T('H'): if (_wcsicmp(lpszValue, L"HONEYDEW") == 0) return clr = 0xfff0fff0; return FALSE;
		case _T('L'): if (_wcsicmp(lpszValue, L"LAVENDER") == 0) return clr = 0xfffae6e6; return FALSE;
		case _T('M'): if (_wcsicmp(lpszValue, L"MOCCASIN") == 0) return clr = 0xffb5e4ff; return FALSE;
		case _T('S'):
			if (_wcsicmp(lpszValue, L"SEAGREEN") == 0) return clr = 0xff578b2e;
			if (_wcsicmp(lpszValue, L"SEASHELL") == 0) return clr = 0xffeef5ff;
			return FALSE;
		}
		return FALSE;
	case 9:
		switch (ch)
		{
		case _T('A'): if (_wcsicmp(lpszValue, L"ALICEBLUE") == 0) return clr = 0xfffff8f0; return FALSE;
		case _T('B'): if (_wcsicmp(lpszValue, L"BURLYWOOD") == 0) return clr = 0xff87b8de; return FALSE;
		case _T('C'):
			if (_wcsicmp(lpszValue, L"CADETBLUE") == 0) return clr = 0xffa09e5f;
			if (_wcsicmp(lpszValue, L"CHOCOLATE") == 0) return clr = 0xff1e69d2;
			return FALSE;
		case _T('D'):
			if (_wcsicmp(lpszValue, L"DARKGREEN") == 0) return clr = 0xff006400;
			if (_wcsicmp(lpszValue, L"DARKKHAKI") == 0) return clr = 0xff6bb7bd;
			return FALSE;
		case _T('F'): if (_wcsicmp(lpszValue, L"FIREBRICK") == 0) return clr = 0xff2222b2; return FALSE;
		case _T('G'):
			if (_wcsicmp(lpszValue, L"GAINSBORO") == 0) return clr = 0xffdcdcdc;
			if (_wcsicmp(lpszValue, L"GOLDENROD") == 0) return clr = 0xff20a5da;
			return FALSE;
		case _T('I'): if (_wcsicmp(lpszValue, L"INDIANRED") == 0)return clr = 0xff5c5ccd; return FALSE;
		case _T('L'):
			if (_wcsicmp(lpszValue, L"LAWNGREEN") == 0)return clr = 0xff00fc7c;
			if (_wcsicmp(lpszValue, L"LIGHTBLUE") == 0) return clr = 0xffe6d8ad;
			if (_wcsicmp(lpszValue, L"LIGHTCYAN") == 0) return clr = 0xffffffe0;
			if (_wcsicmp(lpszValue, L"LIGHTGRAY") == 0) return clr = 0xffd3d3d3;
			if (_wcsicmp(lpszValue, L"LIGHTPINK") == 0) return clr = 0xffc1b6ff;
			if (_wcsicmp(lpszValue, L"LIMEGREEN") == 0) return clr = 0xff32cd32;
			return FALSE;
		case _T('M'):
			if (_wcsicmp(lpszValue, L"MINTCREAM") == 0) return clr = 0xfffafff5;
			if (_wcsicmp(lpszValue, L"MISTYROSE") == 0) return clr = 0xffe1e4ff;
			return FALSE;
		case _T('O'):
			if (_wcsicmp(lpszValue, L"OLIVEDRAB") == 0) return clr = 0xff238e6b;
			if (_wcsicmp(lpszValue, L"ORANGERED") == 0) return clr = 0xff0045ff;
			return FALSE;
		case _T('P'):
			if (_wcsicmp(lpszValue, L"PALEGREEN") == 0) return clr = 0xff98fb98;
			if (_wcsicmp(lpszValue, L"PEACHPUFF") == 0) return clr = 0xffb9daff;
			return FALSE;
		case _T('R'):
			if (_wcsicmp(lpszValue, L"ROSYBROWN") == 0) return clr = 0xff8f8fbc;
			if (_wcsicmp(lpszValue, L"ROYALBLUE") == 0) return clr = 0xffe16941;
			return FALSE;
		case _T('S'):
			if (_wcsicmp(lpszValue, L"SLATEBLUE") == 0) return clr = 0xffcd5a6a;
			if (_wcsicmp(lpszValue, L"SLATEGRAY") == 0)  return clr = 0xff908070;
			if (_wcsicmp(lpszValue, L"STEELBLUE") == 0) return clr = 0xffb48246;
			return FALSE;
		case _T('T'): if (_wcsicmp(lpszValue, L"TURQUOISE") == 0) return clr = 0xffd0e040; return FALSE;
		}
		return FALSE;
	case 10:
		switch (ch)
		{
		case _T('A'): if (_wcsicmp(lpszValue, L"AQUAMARINE") == 0) return clr = 0xffd4ff7f; return FALSE;
		case _T('B'): if (_wcsicmp(lpszValue, L"BLUEVIOLET") == 0) return clr = 0xffe22b8a; return FALSE;
		case _T('C'): if (_wcsicmp(lpszValue, L"CHARTREUSE") == 0) return clr = 0xff00ff7f; return FALSE;
		case _T('D'):
			if (_wcsicmp(lpszValue, L"DARKORANGE") == 0) return clr = 0xff008cff;
			if (_wcsicmp(lpszValue, L"DARKORCHID") == 0) return clr = 0xffcc3299;
			if (_wcsicmp(lpszValue, L"DARKSALMON") == 0) return clr = 0xff7a96e9;
			if (_wcsicmp(lpszValue, L"DARKVIOLET") == 0) return clr = 0xffd30094;
			if (_wcsicmp(lpszValue, L"DODGERBLUE") == 0) return clr = 0xffff901e;
			return FALSE;
		case _T('G'): if (_wcsicmp(lpszValue, L"GHOSTWHITE") == 0) return clr = 0xfffff8f8; return FALSE;
		case _T('L'):
			if (_wcsicmp(lpszValue, L"LIGHTCORAL") == 0)  return clr = 0xff8080f0;
			if (_wcsicmp(lpszValue, L"LIGHTGREEN") == 0) return clr = 0xff90ee90;
			return FALSE;
		case _T('M'):
			if (_wcsicmp(lpszValue, L"MEDIUMBLUE") == 0) return clr = 0xffcd0000; return FALSE;
		case _T('P'):
			if (_wcsicmp(lpszValue, L"PAPAYAWHIP") == 0) return clr = 0xffd5efff;
			if (_wcsicmp(lpszValue, L"POWDERBLUE") == 0) return clr = 0xffe6e0b0;
			return FALSE;
		case _T('S'): if (_wcsicmp(lpszValue, L"SANDYBROWN") == 0) return clr = 0xff60a4f4; return FALSE;
		case _T('W'): if (_wcsicmp(lpszValue, L"WHITESMOKE") == 0) return clr = 0xfff5f5f5; return FALSE;
		}
		return FALSE;
	case 11:
		switch (ch)
		{
		case _T('D'):
			if (_wcsicmp(lpszValue, L"DARKMAGENTA") == 0) return clr = 0xff8b008b;
			if (_wcsicmp(lpszValue, L"DEEPSKYBLUE") == 0) return clr = 0xffffbf00;
			return FALSE;
		case _T('F'):
			if (_wcsicmp(lpszValue, L"FLORALWHITE") == 0)  return clr = 0xfff0faff;
			if (_wcsicmp(lpszValue, L"FORESTGREEN") == 0) return clr = 0xff228b22;
			return FALSE;
		case _T('G'): if (_wcsicmp(lpszValue, L"GREENYELLOW") == 0) return clr = 0xff2fffad; return FALSE;
		case _T('L'):
			if (_wcsicmp(lpszValue, L"LIGHTSALMON") == 0)  return clr = 0xff7aa0ff;
			if (_wcsicmp(lpszValue, L"LIGHTYELLOW") == 0) return clr = 0xffe0ffff;
			return FALSE;
		case _T('N'):
			if (_wcsicmp(lpszValue, L"NAVAJOWHITE") == 0) return clr = 0xffaddeff;
			return FALSE;
		case _T('S'):
			if (_wcsicmp(lpszValue, L"SADDLEBROWN") == 0) return clr = 0xff13458b;
			if (_wcsicmp(lpszValue, L"SPRINGGREEN") == 0) return clr = 0xff7fff00;
			return FALSE;
		case _T('T'): if (_wcsicmp(lpszValue, L"TRANSPARENT") == 0) {clr = 0x00ffffff; return TRUE;} return FALSE;
		case _T('Y'): if (_wcsicmp(lpszValue, L"YELLOWGREEN") == 0) return clr = 0xff32cd9a; return FALSE;
		}
		return FALSE;
	case 12:
		switch (ch)
		{
		case _T('L'):
			if (_wcsicmp(lpszValue, L"LIGHTSKYBLUE") == 0) return clr = 0xffface87;
			if (_wcsicmp(lpszValue, L"LEMONCHIFFON") == 0)   return clr = 0xffcdfaff;
			return FALSE;
		case _T('M'):
			if (_wcsicmp(lpszValue, L"MEDIUMORCHID") == 0) return clr = 0xffd355ba;
			if (_wcsicmp(lpszValue, L"MEDIUMPURPLE") == 0) return clr = 0xffdb7093;
			if (_wcsicmp(lpszValue, L"MIDNIGHTBLUE") == 0) return clr = 0xff701919;
			return FALSE;
		case _T('D'): if (_wcsicmp(lpszValue, L"DARKSEAGREEN") == 0) return clr = 0xff8fbc8f; return FALSE;
		case _T('A'): if (_wcsicmp(lpszValue, L"ANTIQUEWHITE") == 0) return clr = 0xffd7ebfa; return FALSE;
		}
		return FALSE;
	case 13:
		switch (ch)
		{
		case _T('D'):
			if (_wcsicmp(lpszValue, L"DARKSLATEBLUE") == 0) return clr = 0xff8b3d48;
			if (_wcsicmp(lpszValue, L"DARKSLATEGRAY") == 0) return clr = 0xff4f4f2f;
			if (_wcsicmp(lpszValue, L"DARKGOLDENROD") == 0) return clr = 0xff0b86b8;
			if (_wcsicmp(lpszValue, L"DARKTURQUOISE") == 0) return clr = 0xffd1ce00;
			return FALSE;
		case _T('L'):
			if (_wcsicmp(lpszValue, L"LIGHTSEAGREEN") == 0) return clr = 0xffaab220;
			if (_wcsicmp(lpszValue, L"LAVENDERBLUSH") == 0) return clr = 0xfff5f0ff;
			return FALSE;
		case _T('P'):
			if (_wcsicmp(lpszValue, L"PALEGOLDENROD") == 0) return clr = 0xffaae8ee;
			if (_wcsicmp(lpszValue, L"PALETURQUOISE") == 0) return clr = 0xffeeeeaf;
			if (_wcsicmp(lpszValue, L"PALEVIOLETRED") == 0) return clr = 0xff9370db;
			return FALSE;
		}
		return FALSE;
	case 14:
		switch (ch)
		{
		case _T('B'): if (_wcsicmp(lpszValue, L"BLANCHEDALMOND") == 0) return clr = 0xffcdebff; return FALSE;
		case _T('C'): if (_wcsicmp(lpszValue, L"CORNFLOWERBLUE") == 0) return clr = 0xffed9564; return FALSE;
		case _T('D'): if (_wcsicmp(lpszValue, L"DARKOLIVEGREEN") == 0) return clr = 0xff2f6b55; return FALSE;
		case _T('L'):
			if (_wcsicmp(lpszValue, L"LIGHTSLATEGRAY") == 0) return clr = 0xff998877;
			if (_wcsicmp(lpszValue, L"LIGHTSTEELBLUE") == 0) return clr = 0xffdec4b0;
			return FALSE;
		case _T('M'): if (_wcsicmp(lpszValue, L"MEDIUMSEAGREEN") == 0) return clr = 0xff71b33c; return FALSE;
		}
		return FALSE;
	case 15:
		if (_wcsicmp(lpszValue, L"MEDIUMSLATEBLUE") == 0) return clr = 0xffee687b;
		if (_wcsicmp(lpszValue, L"MEDIUMTURQUOISE") == 0) return clr = 0xffccd148;
		if (_wcsicmp(lpszValue, L"MEDIUMVIOLETRED") == 0) return clr = 0xff8515c7;
		return FALSE;
	case 16:
		if (_wcsicmp(lpszValue, L"MEDIUMAQUAMARINE") == 0) return clr = 0xffaacd66;
		return FALSE;
	case 17:
		if (_wcsicmp(lpszValue, L"MEDIUMSPRINGGREEN") == 0) return clr = 0xff9afa00;
		return FALSE;
	case 20:
		if (_wcsicmp(lpszValue, L"LIGHTGOLDENRODYELLOW") == 0) return clr = 0xffd2fafa;
		return FALSE;
	}

	return FALSE;
}


CXTPMarkupObject* CXTPMarkupBuilder::ConvertTextDecorations(CXTPMarkupBuilder* /*pBuilder*/, CXTPMarkupObject* pObject)
{
	if (IsStringObject(pObject))
	{
		LPCWSTR lpszValue = *((CXTPMarkupString*)pObject);

		if (_wcsicmp(lpszValue, L"Underline") == 0)
			return CXTPMarkupEnum::CreateValue(1);

		if (_wcsicmp(lpszValue, L"Strikethrough") == 0)
			return CXTPMarkupEnum::CreateValue(2);

		if (_wcsicmp(lpszValue, L"") == 0)
			return CXTPMarkupEnum::CreateValue(0);
	}

	return NULL;
}

CXTPMarkupObject* CXTPMarkupBuilder::ConvertFontWeight(CXTPMarkupBuilder* /*pBuilder*/, CXTPMarkupObject* pObject)
{
	if (IsStringObject(pObject))
	{
		LPCWSTR lpszValue = *((CXTPMarkupString*)pObject);
		int nLength = ((CXTPMarkupString*)pObject)->GetLength();

		switch (nLength)
		{
		case 4:
			if (_wcsicmp(lpszValue, L"Bold") == 0) return CXTPMarkupEnum::CreateValue(FW_BOLD);
			if (_wcsicmp(lpszValue, L"Thin") == 0) return CXTPMarkupEnum::CreateValue(FW_THIN);
			return NULL;

		case 5:
			if (_wcsicmp(lpszValue, L"Light") == 0) return CXTPMarkupEnum::CreateValue(FW_LIGHT);
			if (_wcsicmp(lpszValue, L"Black") == 0) return CXTPMarkupEnum::CreateValue(FW_BLACK);
			if (_wcsicmp(lpszValue, L"Heavy") == 0) return CXTPMarkupEnum::CreateValue(FW_HEAVY);
			return NULL;

		case 6:
			if (_wcsicmp(lpszValue, L"Normal") == 0) return CXTPMarkupEnum::CreateValue(FW_NORMAL);
			if (_wcsicmp(lpszValue, L"Medium") == 0) return CXTPMarkupEnum::CreateValue(FW_MEDIUM);
			return NULL;

		case 7:
			if (_wcsicmp(lpszValue, L"Regular") == 0) return CXTPMarkupEnum::CreateValue(FW_REGULAR);
			return NULL;

		case 8:
			if (_wcsicmp(lpszValue, L"SemiBold") == 0) return CXTPMarkupEnum::CreateValue(FW_SEMIBOLD);
			if (_wcsicmp(lpszValue, L"DemiBold") == 0) return CXTPMarkupEnum::CreateValue(FW_DEMIBOLD);
			return NULL;

		case 9:
			if (_wcsicmp(lpszValue, L"ExtraBold") == 0) return CXTPMarkupEnum::CreateValue(FW_EXTRABOLD);
			if (_wcsicmp(lpszValue, L"UltraBold") == 0) return CXTPMarkupEnum::CreateValue(FW_ULTRABOLD);
			return NULL;

		case 10:
			if (_wcsicmp(lpszValue, L"ExtraLight") == 0) return CXTPMarkupEnum::CreateValue(FW_EXTRALIGHT);
			if (_wcsicmp(lpszValue, L"ExtraBlack") == 0) return CXTPMarkupEnum::CreateValue(950);
			return NULL;
		}

	}

	return NULL;
}

CXTPMarkupObject* CXTPMarkupBuilder::ConvertFontQuality(CXTPMarkupBuilder* /*pBuilder*/, CXTPMarkupObject* pObject)
{
	if (IsStringObject(pObject))
	{
		LPCWSTR lpszValue = *((CXTPMarkupString*)pObject);
		int nLength = ((CXTPMarkupString*)pObject)->GetLength();

		if (nLength == 9 && _wcsicmp(lpszValue, L"ClearType") == 0)
			return CXTPMarkupEnum::CreateValue(5);
	}
	return NULL;
}

CXTPMarkupObject* CXTPMarkupBuilder::ConvertLength(CXTPMarkupBuilder* /*pBuilder*/, CXTPMarkupObject* pObject)
{
	if (IsStringObject(pObject))
	{
		LPCWSTR lpszValue = *((CXTPMarkupString*)pObject);
		int nLength = ((CXTPMarkupString*)pObject)->GetLength();

		if (nLength == 4 && _wcsicmp(lpszValue, L"Auto") == 0)
			return new CXTPMarkupInt(INT_MAX);

		int nValue = _wtoi(lpszValue);
		if (nValue == 0)
			return new CXTPMarkupInt(nValue);

		if ((nLength > 2) && _wcsicmp(lpszValue + nLength - 2, L"pt") == 0)
		{
			return new CXTPMarkupInt(MulDiv(nValue, 96, 72));
		}

		if ((nLength > 2) && _wcsicmp(lpszValue + nLength - 2, L"in") == 0)
		{
			return new CXTPMarkupInt(nValue * 96);
		}

		if ((nLength > 2) && _wcsicmp(lpszValue + nLength - 2, L"cm") == 0)
		{
			return new CXTPMarkupInt(int((double)nValue * 37.79528));
		}

		if ((nLength > 2) && _wcsicmp(lpszValue + nLength - 2, L"mm") == 0)
		{
			return new CXTPMarkupInt(int((double)nValue * 3.779528));
		}

		return new CXTPMarkupInt(nValue);
	}

	return NULL;
}

CXTPMarkupObject* CXTPMarkupBuilder::ConvertFontStyle(CXTPMarkupBuilder* /*pBuilder*/, CXTPMarkupObject* pObject)
{
	if (IsStringObject(pObject))
	{
		LPCWSTR lpszValue = *((CXTPMarkupString*)pObject);
		int nLength = ((CXTPMarkupString*)pObject)->GetLength();

		if (nLength == 6 && _wcsicmp(lpszValue, L"Normal") == 0)
			return CXTPMarkupEnum::CreateValue(0);

		if (nLength == 6 && _wcsicmp(lpszValue, L"Italic") == 0)
			return CXTPMarkupEnum::CreateValue(1);
	}

	return NULL;
}

//////////////////////////////////////////////////////////////////////////

CXTPMarkupObject* CXTPMarkupBuilder::ConvertVerticalAlignment(CXTPMarkupBuilder* /*pBuilder*/, CXTPMarkupObject* pObject)
{
	if (IsStringObject(pObject))
	{
		LPCWSTR lpszValue = *((CXTPMarkupString*)pObject);
		int nLength = ((CXTPMarkupString*)pObject)->GetLength();

		if (nLength == 3 && _wcsicmp(lpszValue, L"Top") == 0)
			return CXTPMarkupEnum::CreateValue(xtpMarkupVerticalAlignmentTop);

		if (nLength == 6 && _wcsicmp(lpszValue, L"Center") == 0)
			return CXTPMarkupEnum::CreateValue(xtpMarkupVerticalAlignmentCenter);

		if (nLength == 6 && _wcsicmp(lpszValue, L"Bottom") == 0)
			return CXTPMarkupEnum::CreateValue(xtpMarkupVerticalAlignmentBottom);

		if (nLength == 7 && _wcsicmp(lpszValue, L"Stretch") == 0)
			return CXTPMarkupEnum::CreateValue(xtpMarkupVerticalAlignmentStretch);
	}

	return NULL;
}

CXTPMarkupObject* CXTPMarkupBuilder::ConvertHorizontalAlignment(CXTPMarkupBuilder* /*pBuilder*/, CXTPMarkupObject* pObject)
{
	if (IsStringObject(pObject))
	{
		LPCWSTR lpszValue = *((CXTPMarkupString*)pObject);
		int nLength = ((CXTPMarkupString*)pObject)->GetLength();

		if (nLength == 4 && _wcsicmp(lpszValue, L"Left") == 0)
			return CXTPMarkupEnum::CreateValue(xtpMarkupHorizontalAlignmentLeft);

		if (nLength == 6 && _wcsicmp(lpszValue, L"Center") == 0)
			return CXTPMarkupEnum::CreateValue(xtpMarkupHorizontalAlignmentCenter);

		if (nLength == 5 && _wcsicmp(lpszValue, L"Right") == 0)
			return CXTPMarkupEnum::CreateValue(xtpMarkupHorizontalAlignmentRight);

		if (nLength == 7 && _wcsicmp(lpszValue, L"Stretch") == 0)
			return CXTPMarkupEnum::CreateValue(xtpMarkupHorizontalAlignmentStretch);
	}

	return NULL;
}

CXTPMarkupObject* CXTPMarkupBuilder::ConvertTextWrapping(CXTPMarkupBuilder* /*pBuilder*/, CXTPMarkupObject* pObject)
{
	if (IsStringObject(pObject))
	{
		LPCWSTR lpszValue = *((CXTPMarkupString*)pObject);
		int nLength = ((CXTPMarkupString*)pObject)->GetLength();

		if (nLength == 4 && _wcsicmp(lpszValue, L"Wrap") == 0)
			return CXTPMarkupEnum::CreateValue(xtpMarkupTextWrap);

		if (nLength == 16 && _wcsicmp(lpszValue, L"WrapWithOverflow") == 0)
			return CXTPMarkupEnum::CreateValue(xtpMarkupTextWrapWithOverflow);

		if (nLength == 6 && _wcsicmp(lpszValue, L"NoWrap") == 0)
			return CXTPMarkupEnum::CreateValue(xtpMarkupTextNoWrap);
	}

	return NULL;
}

CXTPMarkupObject* CXTPMarkupBuilder::ConvertTextTrimming(CXTPMarkupBuilder* /*pBuilder*/, CXTPMarkupObject* pObject)
{
	if (IsStringObject(pObject))
	{
		LPCWSTR lpszValue = *((CXTPMarkupString*)pObject);
		int nLength = ((CXTPMarkupString*)pObject)->GetLength();

		if (nLength == 4 && _wcsicmp(lpszValue, L"None") == 0)
			return CXTPMarkupEnum::CreateValue(xtpMarkupTextTrimmingNone);

		if (nLength == 17 && _wcsicmp(lpszValue, L"CharacterEllipsis") == 0)
			return CXTPMarkupEnum::CreateValue(xtpMarkupTextTrimmingCharacterEllipsis);

		if (nLength == 12 && _wcsicmp(lpszValue, L"WordEllipsis") == 0)
			return CXTPMarkupEnum::CreateValue(xtpMarkupTextTrimmingWordEllipsis);
	}

	return NULL;
}

CXTPMarkupObject* CXTPMarkupBuilder::ConvertTextAlignment(CXTPMarkupBuilder* /*pBuilder*/, CXTPMarkupObject* pObject)
{
	if (IsStringObject(pObject))
	{
		LPCWSTR lpszValue = *((CXTPMarkupString*)pObject);
		int nLength = ((CXTPMarkupString*)pObject)->GetLength();

		if (nLength == 6 && _wcsicmp(lpszValue, L"Center") == 0)
			return CXTPMarkupEnum::CreateValue(xtpMarkupTextAlignmentCenter);

		if (nLength == 4 && _wcsicmp(lpszValue, L"Left") == 0)
			return CXTPMarkupEnum::CreateValue(xtpMarkupTextAlignmentLeft);

		if (nLength == 5 &&_wcsicmp(lpszValue, L"Right") == 0)
			return CXTPMarkupEnum::CreateValue(xtpMarkupTextAlignmentRight);

		if (nLength == 7 &&_wcsicmp(lpszValue, L"Justify") == 0)
			return CXTPMarkupEnum::CreateValue(xtpMarkupTextAlignmentJustify);
	}

	return NULL;
}

CXTPMarkupObject* CXTPMarkupBuilder::ConvertOrientation(CXTPMarkupBuilder* /*pBuilder*/, CXTPMarkupObject* pObject)
{
	if (IsStringObject(pObject))
	{
		LPCWSTR lpszValue = *((CXTPMarkupString*)pObject);
		int nLength = ((CXTPMarkupString*)pObject)->GetLength();

		if (nLength == 10 && _wcsicmp(lpszValue, L"Horizontal") == 0)
			return CXTPMarkupEnum::CreateValue(xtpMarkupOrientationHorizontal);

		if (nLength == 8 && _wcsicmp(lpszValue, L"Vertical") == 0)
			return CXTPMarkupEnum::CreateValue(xtpMarkupOrientationVertical);
	}

	return NULL;
}

CXTPMarkupObject* CXTPMarkupBuilder::ConvertBaselineAlignment(CXTPMarkupBuilder* /*pBuilder*/, CXTPMarkupObject* pObject)
{
	if (IsStringObject(pObject))
	{
		LPCWSTR lpszValue = *((CXTPMarkupString*)pObject);
		int nLength = ((CXTPMarkupString*)pObject)->GetLength();

		if (nLength == 3 && _wcsicmp(lpszValue, L"Top") == 0) return CXTPMarkupEnum::CreateValue(xtpMarkupBaselineTop);
		if (nLength == 6 && _wcsicmp(lpszValue, L"Center") == 0) return CXTPMarkupEnum::CreateValue(xtpMarkupBaselineCenter);
		if (nLength == 6 && _wcsicmp(lpszValue, L"Bottom") == 0) return CXTPMarkupEnum::CreateValue(xtpMarkupBaselineBottom);
		if (nLength == 8 && _wcsicmp(lpszValue, L"Baseline") == 0) return CXTPMarkupEnum::CreateValue(xtpMarkupBaseline);
		if (nLength == 7 && _wcsicmp(lpszValue, L"TextTop") == 0) return CXTPMarkupEnum::CreateValue(xtpMarkupBaselineTextTop);
		if (nLength == 10 && _wcsicmp(lpszValue, L"TextBottom") == 0) return CXTPMarkupEnum::CreateValue(xtpMarkupBaselineTextBottom);
		if (nLength == 9 && _wcsicmp(lpszValue, L"Subscript") == 0) return CXTPMarkupEnum::CreateValue(xtpMarkupBaselineSubscript);
		if (nLength == 11 && _wcsicmp(lpszValue, L"Superscript") == 0) return CXTPMarkupEnum::CreateValue(xtpMarkupBaselineSuperscript);
	}

	return NULL;
}

CXTPMarkupObject* CXTPMarkupBuilder::ConvertCursor(CXTPMarkupBuilder* /*pBuilder*/, CXTPMarkupObject* pObject)
{
	if (IsStringObject(pObject))
	{
		LPCWSTR lpszValue = *((CXTPMarkupString*)pObject);
		int nLength = ((CXTPMarkupString*)pObject)->GetLength();

		if (nLength == 4 && _wcsicmp(lpszValue, L"None") == 0) return new CXTPMarkupInt(0);
		if (nLength == 2 && _wcsicmp(lpszValue, L"No") == 0) return new CXTPMarkupInt((DWORD)(DWORD_PTR)IDC_NO);
		if (nLength == 5 && _wcsicmp(lpszValue, L"Arrow") == 0) return new CXTPMarkupInt((DWORD)(DWORD_PTR)IDC_ARROW);
		if (nLength == 11 && _wcsicmp(lpszValue, L"AppStarting") == 0) return new CXTPMarkupInt((DWORD)(DWORD_PTR)IDC_APPSTARTING);
		if (nLength == 5 && _wcsicmp(lpszValue, L"Cross") == 0) return new CXTPMarkupInt((DWORD)(DWORD_PTR)IDC_CROSS);
		if (nLength == 5 && _wcsicmp(lpszValue, L"IBeam") == 0) return new CXTPMarkupInt((DWORD)(DWORD_PTR)IDC_IBEAM);
		if (nLength == 6 && _wcsicmp(lpszValue, L"SizeAll") == 0) return new CXTPMarkupInt((DWORD)(DWORD_PTR)IDC_SIZEALL);
		if (nLength == 8 && _wcsicmp(lpszValue, L"SizeNESW") == 0) return new CXTPMarkupInt((DWORD)(DWORD_PTR)IDC_SIZENESW);
		if (nLength == 6 && _wcsicmp(lpszValue, L"SizeNS") == 0) return new CXTPMarkupInt((DWORD)(DWORD_PTR)IDC_SIZENS);
		if (nLength == 8 && _wcsicmp(lpszValue, L"SizeNWSE") == 0) return new CXTPMarkupInt((DWORD)(DWORD_PTR)IDC_SIZENWSE);
		if (nLength == 6 && _wcsicmp(lpszValue, L"SizeWE") == 0) return new CXTPMarkupInt((DWORD)(DWORD_PTR)IDC_SIZEWE);
		if (nLength == 7 && _wcsicmp(lpszValue, L"UpArrow") == 0) return new CXTPMarkupInt((DWORD)(DWORD_PTR)IDC_UPARROW);
		if (nLength == 4 && _wcsicmp(lpszValue, L"Wait") == 0) return new CXTPMarkupInt((DWORD)(DWORD_PTR)IDC_WAIT);
		if (nLength == 4 && _wcsicmp(lpszValue, L"Hand") == 0) return new CXTPMarkupInt((DWORD)32649);
	}

	return NULL;
}
CXTPMarkupObject* CXTPMarkupBuilder::ConvertVisibility(CXTPMarkupBuilder* /*pBuilder*/, CXTPMarkupObject* pObject)
{
	if (IsStringObject(pObject))
	{
		LPCWSTR lpszValue = *((CXTPMarkupString*)pObject);
		int nLength = ((CXTPMarkupString*)pObject)->GetLength();

		if (nLength == 7 && _wcsicmp(lpszValue, L"Visible") == 0) return CXTPMarkupEnum::CreateValue(xtpMarkupVisibilityVisible);
		if (nLength == 6 && _wcsicmp(lpszValue, L"Hidden") == 0) return CXTPMarkupEnum::CreateValue(xtpMarkupVisibilityHidden);
		if (nLength == 9 && _wcsicmp(lpszValue, L"Collapsed") == 0) return CXTPMarkupEnum::CreateValue(xtpMarkupVisibilityCollapsed);
	}
	return NULL;
}

CXTPMarkupObject* CXTPMarkupBuilder::ConvertContent(CXTPMarkupBuilder* pBuilder, CXTPMarkupObject* pObject)
{
	if (IsStringObject(pObject))
	{
		CXTPMarkupRun* pRun = MARKUP_CREATE(CXTPMarkupRun, pBuilder->GetMarkupContext());
		pRun->SetText((CXTPMarkupString*)pObject);
		MARKUP_ADDREF(pObject);

		CXTPMarkupTextBlock* pTextBlock = MARKUP_CREATE(CXTPMarkupTextBlock, pBuilder->GetMarkupContext());
		pTextBlock->GetInlines()->Add(pRun);

		return pTextBlock;
	}
	return pObject;

}

CXTPMarkupObject* CXTPMarkupBuilder::ConvertScrollBarVisibility(CXTPMarkupBuilder* /*pBuilder*/, CXTPMarkupObject* pObject)
{
	if (IsStringObject(pObject))
	{
		LPCWSTR lpszValue = *((CXTPMarkupString*)pObject);
		int nLength = ((CXTPMarkupString*)pObject)->GetLength();

		if (nLength == 4 && _wcsicmp(lpszValue, L"Auto") == 0) return CXTPMarkupEnum::CreateValue(xtpMarkupScrollBarAuto);
		if (nLength == 7 && _wcsicmp(lpszValue, L"Visible") == 0) return CXTPMarkupEnum::CreateValue(xtpMarkupScrollBarVisible);
		if (nLength == 8 && _wcsicmp(lpszValue, L"Disabled") == 0) return CXTPMarkupEnum::CreateValue(xtpMarkupScrollBarDisabled);
	}
	return NULL;
}

CXTPMarkupObject* CXTPMarkupBuilder::ConvertStretch(CXTPMarkupBuilder* /*pBuilder*/, CXTPMarkupObject* pObject)
{
	if (IsStringObject(pObject))
	{
		LPCWSTR lpszValue = *((CXTPMarkupString*)pObject);
		int nLength = ((CXTPMarkupString*)pObject)->GetLength();

		if (nLength == 4 && _wcsicmp(lpszValue, L"None") == 0) return CXTPMarkupEnum::CreateValue(xtpMarkupStretchNone);
		if (nLength == 4 && _wcsicmp(lpszValue, L"Fill") == 0) return CXTPMarkupEnum::CreateValue(xtpMarkupStretchFill);
		if (nLength == 7 && _wcsicmp(lpszValue, L"Uniform") == 0) return CXTPMarkupEnum::CreateValue(xtpMarkupStretchUniform);
		if (nLength == 13 && _wcsicmp(lpszValue, L"UniformToFill") == 0) return CXTPMarkupEnum::CreateValue(xtpMarkupStretchUniformToFill);
	}
	return NULL;
}

BOOL CXTPMarkupBuilder::ConvertDouble(LPCWSTR& lpszValue, double& dValue, TCHAR cStop, BOOL bCheckTail)
{
	if (!lpszValue)
		return FALSE;

	while (*lpszValue == ' ') lpszValue++;

	dValue = 0;
	BOOL bSign = FALSE;
	if (*lpszValue == '-')
	{
		bSign = TRUE;
		lpszValue++;
	}
	else if (*lpszValue == '+')
	{
		lpszValue++;
	}

	LPCWSTR lpszNext = lpszValue;

	if (*lpszNext == 0)
		return FALSE;

	BOOL bDecimal = 0;
	double dDecimal = 0;

	while (*lpszNext != 0)
	{
		WCHAR c = *lpszNext;

		if (c == ' ' || c == cStop)
		{
			if (lpszNext == lpszValue)
				return FALSE;

			if (c == ' ')
			{
				while (*lpszNext == ' ') lpszNext++;
			}

			if (cStop != 0 && *lpszNext == cStop)
				lpszNext++;
			break;
		}

		if (c == '.')
		{
			if (bDecimal)
				return FALSE;
			bDecimal = TRUE;
			dDecimal = 1;

			if (lpszValue == lpszNext)
				lpszValue = ++lpszNext;
			else lpszNext++;

			continue;
		}

		if (c <'0' || c > '9')
			return FALSE;

		if (!bDecimal)
		{
			dValue = 10 * dValue + (c - '0');
		}
		else
		{
			dDecimal /= 10;
			dValue += dDecimal * double(c - '0');
		}

		lpszNext++;
	}

	if (lpszNext == lpszValue)
		return FALSE;

	lpszValue = lpszNext;
	while (*lpszValue == ' ') lpszValue++;

	if (bCheckTail && lpszValue[0] != 0)
		return FALSE;

	if (bSign) dValue = -dValue;

	return TRUE;
}
