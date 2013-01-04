// XTPMarkupObject.cpp: implementation of the CXTPMarkupObject class.
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
#include "Common/XTPResourceManager.h"

#include "XTPMarkupObject.h"
#include "XTPMarkupBuilder.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


CXTPMarkupPropertyChangedEventArgs::CXTPMarkupPropertyChangedEventArgs(CXTPMarkupDependencyProperty* pProperty, CXTPMarkupObject* pOldValue, CXTPMarkupObject* pNewValue)
{
	m_pProperty = pProperty;
	m_pOldValue = pOldValue;
	m_pNewValue = pNewValue;
}

struct CXTPMarkupFromNameKey
{
public:
	CXTPMarkupFromNameKey(LPCWSTR lpszName = 0, CXTPMarkupType* pOwnerType = 0)
	{
		m_lpszName = lpszName;
		m_pOwnerType = pOwnerType;
	}

	BOOL operator==(const CXTPMarkupFromNameKey& key) const
	{
		return m_pOwnerType == key.m_pOwnerType && wcscmp(m_lpszName, key.m_lpszName) == 0;
	}

	LPCWSTR m_lpszName;
	CXTPMarkupType* m_pOwnerType;
};


class CXTPMarkupPropertyFromNameMap : public CMap<CXTPMarkupFromNameKey, CXTPMarkupFromNameKey&, CXTPMarkupDependencyProperty*, CXTPMarkupDependencyProperty*>
{
public:
	CXTPMarkupPropertyFromNameMap();
	~CXTPMarkupPropertyFromNameMap();
};


CXTPMarkupPropertyFromNameMap::CXTPMarkupPropertyFromNameMap()
{

}

CXTPMarkupPropertyFromNameMap::~CXTPMarkupPropertyFromNameMap()
{
	POSITION pos = GetStartPosition();
	while (pos)
	{
		CXTPMarkupFromNameKey key;
		CXTPMarkupDependencyProperty* dp;
		GetNextAssoc(pos, key, dp);
		dp->Release();
	}
	RemoveAll();
}

template<>
AFX_INLINE UINT AFXAPI HashKey<CXTPMarkupFromNameKey&>(CXTPMarkupFromNameKey& key)
{
	// default identity hash - works for most primitive values
	return HashKey<LPCWSTR>(key.m_lpszName) & ((UINT)(DWORD)(DWORD_PTR)key.m_pOwnerType) >> 16;
}


CXTPMarkupPropertyFromNameMap* CXTPMarkupDependencyProperty::GetPropertyMap()
{
	static CXTPMarkupPropertyFromNameMap m_PropertyFromName;
	return &m_PropertyFromName;
}

//////////////////////////////////////////////////////////////////////////
// CXTPMarkupPropertyMetadata
CXTPMarkupPropertyMetadata::CXTPMarkupPropertyMetadata(CXTPMarkupObject* pDefaultValue, DWORD dwFlags)
{
	m_pDefaultValue = pDefaultValue;
	m_pConverter = NULL;
	m_dwFlags = dwFlags;
	m_pPropertyChanged = NULL;
}

CXTPMarkupPropertyMetadata::CXTPMarkupPropertyMetadata(CXTPMarkupObject* pDefaultValue, PFNCONVERTFROM pConverter, DWORD dwFlags)
{
	m_pDefaultValue = pDefaultValue;
	m_pConverter = pConverter;
	m_dwFlags = dwFlags;
	m_pPropertyChanged = NULL;
}

CXTPMarkupPropertyMetadata::CXTPMarkupPropertyMetadata(CXTPMarkupObject* pDefaultValue, PFNPROPERTYCHANGED pPropertyChanged, DWORD dwFlags)
{
	m_pDefaultValue = pDefaultValue;
	m_pConverter = NULL;
	m_dwFlags = dwFlags;
	m_pPropertyChanged = pPropertyChanged;
}

CXTPMarkupPropertyMetadata::CXTPMarkupPropertyMetadata(CXTPMarkupObject* pDefaultValue, PFNCONVERTFROM pConverter, PFNPROPERTYCHANGED pPropertyChanged, DWORD dwFlags)
{
	m_pDefaultValue = pDefaultValue;
	m_pConverter = pConverter;
	m_dwFlags = dwFlags;
	m_pPropertyChanged = pPropertyChanged;

}



CXTPMarkupPropertyMetadata::~CXTPMarkupPropertyMetadata()
{
	if (m_pDefaultValue)
	{
		MARKUP_RELEASE(m_pDefaultValue);
	}
}

//////////////////////////////////////////////////////////////////////////
// CXTPMarkupDependencyProperty

int CXTPMarkupDependencyProperty::s_nCount = 0;

IMPLEMENT_MARKUPCLASS(NULL, CXTPMarkupDependencyProperty, CXTPMarkupObject);

void CXTPMarkupDependencyProperty::RegisterMarkupClass()
{

}

CXTPMarkupDependencyProperty::CXTPMarkupDependencyProperty()
{
	m_lpszName = 0;
	m_pPropetyType = 0;
	m_pOwnerType = 0;
	m_bAttached = FALSE;
	m_pMetadata = NULL;
	m_nIndex = -1;
}

CXTPMarkupDependencyProperty::~CXTPMarkupDependencyProperty()
{
	if (m_pMetadata)
	{
		delete m_pMetadata;
	}
}


CXTPMarkupDependencyProperty* CXTPMarkupDependencyProperty::RegisterCommon(CXTPMarkupDependencyProperty* dp, LPCWSTR lpszName, CXTPMarkupType* pPropetyType, CXTPMarkupType* pOwnerType, BOOL bAttached)
{
	CXTPMarkupFromNameKey key(lpszName, pOwnerType);

	dp->m_lpszName = lpszName;
	dp->m_pPropetyType = pPropetyType;
	dp->m_pOwnerType = pOwnerType;
	dp->m_bAttached = bAttached;

	CXTPMarkupPropertyFromNameMap* pPropertyFromNameMap = GetPropertyMap();

	if (!dp->IsEvent())
	{
		dp->m_nIndex = s_nCount++;
	}

	pPropertyFromNameMap->SetAt(key, dp);

	return dp;
}

CXTPMarkupDependencyProperty* CXTPMarkupDependencyProperty::RegisterAttached(LPCWSTR lpszName, CXTPMarkupType* pPropetyType, CXTPMarkupType* pOwnerType, CXTPMarkupPropertyMetadata* pMetadata)
{
	CXTPMarkupDependencyProperty* dp =  RegisterCommon(new CXTPMarkupDependencyProperty(), lpszName, pPropetyType, pOwnerType, TRUE);
	dp->m_pMetadata = pMetadata;
	return dp;
}

CXTPMarkupDependencyProperty* CXTPMarkupDependencyProperty::Register(LPCWSTR lpszName, CXTPMarkupType* pPropetyType, CXTPMarkupType* pOwnerType, CXTPMarkupPropertyMetadata* pMetadata)
{
	CXTPMarkupDependencyProperty* dp = RegisterCommon(new CXTPMarkupDependencyProperty(), lpszName, pPropetyType, pOwnerType, FALSE);
	dp->m_pMetadata = pMetadata;
	return dp;
}

CXTPMarkupDependencyProperty* CXTPMarkupDependencyProperty::AddOwner(CXTPMarkupType* pOwnerType)
{
	CXTPMarkupFromNameKey key(m_lpszName, pOwnerType);

	CXTPMarkupPropertyFromNameMap* pPropertyFromNameMap = GetPropertyMap();
	pPropertyFromNameMap->SetAt(key, this);

	AddRef();

	return this;
}

CXTPMarkupDependencyProperty* CXTPMarkupDependencyProperty::FindProperty(CXTPMarkupType* pRuntimeClass, LPCWSTR lpszAttribute)
{
	if (!lpszAttribute)
		return NULL;

	if (lpszAttribute[0] == 'x' && lpszAttribute[1] == ':')
		lpszAttribute = lpszAttribute + 2;

	CXTPMarkupPropertyFromNameMap* pMap = CXTPMarkupDependencyProperty::GetPropertyMap();

	while (pRuntimeClass != NULL)
	{
		CXTPMarkupFromNameKey nameKey(lpszAttribute, pRuntimeClass);
		CXTPMarkupDependencyProperty* pProperty = NULL;

		if (pMap->Lookup(nameKey, pProperty))
			return pProperty;

		pRuntimeClass = pRuntimeClass->GetBaseType();

	}

	return NULL;
}

//////////////////////////////////////////////////////////////////////////
// CXTPMarkupType

class CXTPMarkupType::CClassList
{
public:
	CClassList();
	~CClassList();

public:
	void Add(LPCWSTR lpszTag, CXTPMarkupType* pClass);
	CXTPMarkupType* Lookup(LPCWSTR lpszTag);

public:
	CMap<LPCWSTR, LPCWSTR, CXTPMarkupType*, CXTPMarkupType*> m_mapTypes;
	CTypedSimpleList<CXTPMarkupType*> m_classList;

};

template<> inline BOOL AFXAPI CompareElements(const LPCWSTR* pElement1, const LPCWSTR* pElement2)
{
	return wcscmp(*pElement1, *pElement2) == 0;
}


CXTPMarkupType::CClassList::CClassList()
{
	m_classList.Construct(offsetof(CXTPMarkupType, m_pNextType));
}

CXTPMarkupType::CClassList::~CClassList()
{
	CXTPMarkupType* pType = m_classList.GetHead();
	while (pType)
	{
		CXTPMarkupType* pTypeDelete = pType;

		pType = pType->m_pNextType;

		MARKUP_RELEASE(pTypeDelete);
	}

}


void CXTPMarkupType::CClassList::Add(LPCWSTR lpszTag, CXTPMarkupType* pClass)
{
	if (lpszTag && lpszTag[0] != '\0')
	{
		ASSERT(Lookup(lpszTag) == NULL);
		m_mapTypes.SetAt(lpszTag, pClass);
	}
	m_classList.AddHead(pClass);
}

CXTPMarkupType* CXTPMarkupType::CClassList::Lookup(LPCWSTR lpszTag)
{
	CXTPMarkupType* pType = NULL;

	if (m_mapTypes.Lookup(lpszTag, pType))
	{
		return pType;
	}

	return NULL;
}

CXTPMarkupType::CClassList* CXTPMarkupType::GetClassList()
{
	static CClassList list;
	return &list;
}


CXTPMarkupType* CXTPMarkupType::LookupTag(LPCWSTR lpszTag)
{
	return GetClassList()->Lookup(lpszTag);
}

IMPLEMENT_MARKUPCLASS(NULL, CXTPMarkupType, CXTPMarkupObject)

void CXTPMarkupType::RegisterMarkupClass()
{

}

CXTPMarkupType::CXTPMarkupType()
{
	m_lpszTag = NULL;
	m_lpszClassName = NULL;
	m_pfnCreateObject = NULL;
	m_pfnGetBaseType = NULL;
	m_pfnRegisterMarkupClass = NULL;
	m_pNextType = NULL;
	m_bRegister = FALSE;
	m_pTypeStyle = NULL;
}

CXTPMarkupType::CXTPMarkupType(LPCWSTR lpszTag, LPCWSTR lpszClassName, PFNCREATEOBJECT pfnCreateObject, PFNGETBASETYPE pfnGetBaseType, PFNREGISTERMARKUPCLASS pfnRegisterMarkupClass)
{
	m_lpszTag = lpszTag;
	m_lpszClassName = lpszClassName;
	m_pfnCreateObject = pfnCreateObject;
	m_pfnGetBaseType = pfnGetBaseType;
	m_pfnRegisterMarkupClass = pfnRegisterMarkupClass;
	m_pNextType = NULL;
	m_bRegister = FALSE;
	m_pTypeStyle = NULL;

	GetClassList()->Add(lpszTag, this);
}

CXTPMarkupObject* CXTPMarkupType::ConvertFrom(CXTPMarkupBuilder* /*pBuilder*/, CXTPMarkupObject* pObject) const
{
	if (IsStringObject(pObject))
	{
		LPCWSTR lpszValue = *((CXTPMarkupString*)pObject);

		CXTPMarkupType* pType = GetClassList()->Lookup(lpszValue);
		if (pType)
		{
			pType->AddRef();
			return pType;
		}
	}

	return NULL;
}

CXTPMarkupType::~CXTPMarkupType()
{
	if (m_pTypeStyle)
	{
		((CXTPMarkupObject*)m_pTypeStyle)->Release();
	}
}

CXTPMarkupObject* CXTPMarkupType::CreateObject() const
{
	return (*m_pfnCreateObject)();
}

CXTPMarkupType* CXTPMarkupType::GetBaseType() const
{
	return (*m_pfnGetBaseType)();
}

void CXTPMarkupType::Register()
{
	if (m_bRegister)
		return;

	CXTPMarkupType* pType = GetBaseType();
	if (pType && !pType->m_bRegister)
		pType->Register();

	m_bRegister = TRUE;
	(*m_pfnRegisterMarkupClass)();
}

void AFX_CDECL CXTPMarkupType::RegisterAll()
{
	CXTPMarkupType* pType = GetClassList()->m_classList.GetHead();
	while (pType)
	{
		pType->Register();
		pType = pType->m_pNextType;
	}
}

BOOL CXTPMarkupType::IsDerivedFrom(const CXTPMarkupType* pBaseClass) const
{
	const CXTPMarkupType* pClassThis = this;
	while (pClassThis != NULL)
	{
		if (pClassThis == pBaseClass)
			return TRUE;

		pClassThis = pClassThis->GetBaseType();
	}
	return FALSE;
}

CXTPMarkupObject* CXTPMarkupType::DynamicDownCast(CXTPMarkupType* pType, CXTPMarkupObject* pObject)
{
	if (pObject != NULL && pObject->IsKindOf(pType))
		return pObject;
	else
		return NULL;
}

CXTPMarkupObject* CXTPMarkupType::StaticDownCast(CXTPMarkupType* pType, CXTPMarkupObject* pObject)
{
	pType;
	ASSERT(pObject == NULL || pObject->IsKindOf(pType));

	return pObject;
}

//////////////////////////////////////////////////////////////////////////
// CXTPMarkupProperties

CXTPMarkupProperties::CXTPMarkupProperties(CXTPMarkupObject* pOwner)
{
	m_pOwner = pOwner;
	m_nTableSize = CXTPMarkupDependencyProperty::s_nCount;
	m_pTable = new CXTPMarkupObject*[m_nTableSize];
	ZeroMemory(m_pTable, m_nTableSize * sizeof(CXTPMarkupObject*));
}

CXTPMarkupProperties::~CXTPMarkupProperties()
{
	for (int i = 0; i < m_nTableSize; i++)
	{
		if (m_pTable[i]) m_pTable[i]->Release();
	}
	delete[] m_pTable;
}

BOOL CXTPMarkupProperties::IsPropertyValid(CXTPMarkupDependencyProperty* pProperty) const
{
	if (pProperty->m_nIndex == -1)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	if (pProperty->m_nIndex >= m_nTableSize)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	return TRUE;
}

CXTPMarkupObject* CXTPMarkupProperties::Lookup(CXTPMarkupDependencyProperty* pProperty) const
{
	if (!IsPropertyValid(pProperty))
		return NULL;

	return m_pTable[pProperty->m_nIndex];
}

void CXTPMarkupProperties::Copy(CXTPMarkupProperties* pOwner)
{
	ASSERT(pOwner->m_nTableSize == m_nTableSize);

	for (int i = 0; i < pOwner->m_nTableSize; i++)
	{
		if (pOwner->m_pTable[i])
		{
			m_pTable[i] = pOwner->m_pTable[i];
			m_pTable[i]->AddRef();
		}
	}
}

void CXTPMarkupProperties::Set(CXTPMarkupDependencyProperty* pProperty, CXTPMarkupObject* pValue)
{
	if (!IsPropertyValid(pProperty))
		return;

	int nIndex = pProperty->m_nIndex;

	if (m_pTable[nIndex])
	{
		if (m_pOwner) m_pTable[nIndex]->SetLogicalParent(NULL);
		MARKUP_RELEASE(m_pTable[nIndex]);

		m_pTable[nIndex] = NULL;
	}

	if (pValue)
	{
		m_pTable[nIndex] = pValue;
		if (m_pOwner) pValue->SetLogicalParent(m_pOwner);
	}
}


//////////////////////////////////////////////////////////////////////////
// CXTPMarkupObject

CXTPMarkupDependencyProperty* CXTPMarkupObject::m_pNameProperty = NULL;
CXTPMarkupDependencyProperty* CXTPMarkupObject::m_pKeyProperty = NULL;

CXTPMarkupObject* AFX_CDECL CXTPMarkupObject::CreateMarkupObject()
{
	return new CXTPMarkupObject();
}

CXTPMarkupType* CXTPMarkupObject::GetMarkupBaseType()
{
	return NULL;
}

const CXTPMarkupType* CXTPMarkupObject::typeCXTPMarkupObject = new CXTPMarkupType(
	NULL, L"CXTPMarkupObject", &CXTPMarkupObject::CreateMarkupObject, &CXTPMarkupObject::GetMarkupBaseType, &CXTPMarkupObject::RegisterMarkupClass);

CXTPMarkupType* CXTPMarkupObject::GetType() const
{
	return MARKUP_TYPE(CXTPMarkupObject);
}

void CXTPMarkupObject::RegisterMarkupClass()
{
	m_pNameProperty = CXTPMarkupDependencyProperty::Register(L"Name", MARKUP_TYPE(CXTPMarkupString), MARKUP_TYPE(CXTPMarkupObject));
	m_pKeyProperty = CXTPMarkupDependencyProperty::Register(L"Key", MARKUP_TYPE(CXTPMarkupObject), MARKUP_TYPE(CXTPMarkupObject));
}


CXTPMarkupObject::CXTPMarkupObject()
{
	m_dwRef = 1;
	m_pLogicalParent = NULL;
	m_lpMarkupTag = NULL;

	m_pProperties = NULL;

}

CXTPMarkupObject::~CXTPMarkupObject()
{
	ASSERT(m_dwRef <= 1);

	SAFE_DELETE(m_pProperties);

	if (m_lpMarkupTag) delete[] m_lpMarkupTag;
}

DWORD CXTPMarkupObject::AddRef()
{
	return InterlockedIncrement(&m_dwRef);
}

void CXTPMarkupObject::OnFinalRelease()
{
	delete this;
}

DWORD CXTPMarkupObject::Release()
{
	if (m_dwRef == 0)
		return 0;

	LONG lResult = InterlockedDecrement(&m_dwRef);
	if (lResult == 0)
	{
		OnFinalRelease();
	}
	return lResult;
}

CXTPMarkupObject* CXTPMarkupObject::ConvertFrom(CXTPMarkupBuilder* /*pBuilder*/, CXTPMarkupObject* /*pObject*/) const
{
	return NULL;
}

void CXTPMarkupObject::SetValue(CXTPMarkupDependencyProperty* pProperty, CXTPMarkupObject* pValue)
{
	if (!pProperty)
		return;

	if (pValue != NULL && !pValue->GetType()->IsDerivedFrom(pProperty->GetPropetyType()))
	{
		ASSERT(FALSE);
		return;
	}

	if (pValue && !m_pProperties)
	{
		m_pProperties = new CXTPMarkupProperties(this);
	}

	CXTPMarkupObject* pOldValue = GetValue(pProperty);

	if (m_pProperties)
	{
		m_pProperties->Set(pProperty, pValue);
	}

	CXTPMarkupObject* pNewValue = GetValue(pProperty);

	if (::IsEqual(pNewValue, pOldValue))
		return;

	OnPropertyChanged(pProperty, pOldValue, pNewValue);

	if (pProperty->GetFlags() & CXTPMarkupPropertyMetadata::flagInherited)
	{
		RecursePropertyChanged(pProperty, pOldValue, pNewValue);
	}

}

void CXTPMarkupObject::RecursePropertyChanged(CXTPMarkupDependencyProperty* pProperty, CXTPMarkupObject* pOldValue, CXTPMarkupObject* pNewValue)
{
	int nCount = GetLogicalChildrenCount();

	for (int i = 0; i < nCount; i++)
	{
		CXTPMarkupObject* pObject = GetLogicalChild(i);
		if (!pObject)
			continue;

		if (pObject->IsKindOf(pProperty->GetOwnerType()))
		{
			pObject->OnPropertyChanged(pProperty, pOldValue, pNewValue);
		}

		pObject->RecursePropertyChanged(pProperty, pOldValue, pNewValue);
	}

}

void CXTPMarkupObject::OnPropertyChanged(CXTPMarkupDependencyProperty* pProperty, CXTPMarkupObject* pOldValue, CXTPMarkupObject* pNewValue)
{
	if (pProperty->GetMetadata() && pProperty->GetMetadata()->m_pPropertyChanged != NULL)
	{
		CXTPMarkupPropertyChangedEventArgs e(pProperty, pOldValue, pNewValue);
		(*pProperty->GetMetadata()->m_pPropertyChanged)(this, &e);
	}
}


CXTPMarkupObject* CXTPMarkupObject::GetValueSource(CXTPMarkupDependencyProperty* pProperty) const
{
	CXTPMarkupObject* pValue = GetValueCore(pProperty);
	if (pValue)
	{
		return (CXTPMarkupObject*)this;
	}

	if (pProperty->GetFlags() & CXTPMarkupPropertyMetadata::flagInherited)
	{
		CXTPMarkupObject* pLogicalParent = GetLogicalParent();
		while (pLogicalParent)
		{
			pValue = pLogicalParent->GetValueCore(pProperty);

			if (pValue)
				return pLogicalParent;

			pLogicalParent = pLogicalParent->GetLogicalParent();
		}
	}

	return NULL;
}

CXTPMarkupObject* CXTPMarkupObject::GetValueCore(CXTPMarkupDependencyProperty* pProperty) const
{
	CXTPMarkupObject* pValue = m_pProperties ? m_pProperties->Lookup(pProperty) : NULL;

	return pValue;
}

CXTPMarkupObject* CXTPMarkupObject::GetValue(CXTPMarkupDependencyProperty* pProperty) const
{
	CXTPMarkupObject* pValue = GetValueCore(pProperty);
	if (pValue)
		return pValue;

	if (pProperty->GetFlags() & CXTPMarkupPropertyMetadata::flagInherited)
	{
		CXTPMarkupObject* pLogicalParent = GetLogicalParent();
		while (pLogicalParent)
		{
			pValue = pLogicalParent->GetValueCore(pProperty);
			if (pValue)
				return pValue;

			pLogicalParent = pLogicalParent->GetLogicalParent();
		}
	}

	if (pProperty->GetMetadata() && pProperty->GetMetadata()->m_pDefaultValue)
		return pProperty->GetMetadata()->m_pDefaultValue;

	return NULL;
}

void CXTPMarkupObject::SetLogicalParent(CXTPMarkupObject* pObject)
{
	m_pLogicalParent = pObject;
}

CXTPMarkupObject* CXTPMarkupObject::GetLogicalParent() const
{
	return m_pLogicalParent;
}

void CXTPMarkupObject::SetContentObject(CXTPMarkupBuilder* pBuilder, CXTPMarkupObject* pContent)
{
	if (IsStringObject(pContent))
	{
		pBuilder->ThrowBuilderException(CXTPMarkupBuilder::FormatString(
			_T("Cannot convert the string '%ls' into a '%ls' object"),
			(LPCTSTR)(LPCWSTR)*((CXTPMarkupString*)pContent), (LPCTSTR)GetType()->m_lpszClassName));
	}
	else
	{
		pBuilder->ThrowBuilderException(CXTPMarkupBuilder::FormatString(_T("Cannot add content to an object of type '%ls'"),
			(LPCTSTR)GetType()->m_lpszClassName));
	}
}

BOOL CXTPMarkupObject::HasContentObject() const
{
	return FALSE;
}

BOOL CXTPMarkupObject::AllowWhiteSpaceContent() const
{
	return FALSE;
}

void CXTPMarkupObject::SetPropertyObject(CXTPMarkupBuilder* pBuilder, CXTPMarkupDependencyProperty* pProperty, CXTPMarkupObject* pValue)
{
	ASSERT(pValue);

	if (pValue != NULL && !pValue->IsKindOf(pProperty->GetPropetyType()))
	{
		CXTPMarkupObject* pNewValue = pBuilder->ConvertValue(pProperty, pValue);

		MARKUP_RELEASE(pValue);
		pValue = pNewValue;
	}

	SetValue(pProperty, pValue);
}

CXTPMarkupObject* CXTPMarkupObject::FindName(LPCWSTR lpszName)
{
	CXTPMarkupString* pName = MARKUP_STATICCAST(CXTPMarkupString, GetValue(m_pNameProperty));
	if (pName && (wcscmp(*pName, lpszName) == 0))
		return this;

	int nCount = GetLogicalChildrenCount();
	for (int i = 0; i < nCount; i++)
	{
		CXTPMarkupObject* pResult = GetLogicalChild(i)->FindName(lpszName);
		if (pResult)
		{
			return pResult;
		}
	}

	return NULL;
}

int CXTPMarkupObject::GetLogicalChildrenCount() const
{
	return 0;
}

CXTPMarkupObject* CXTPMarkupObject::GetLogicalChild(int /*nIndex*/) const
{
	ASSERT(FALSE);
	return NULL;
}



BOOL CXTPMarkupObject::IsKindOf(const CXTPMarkupType* pClass) const
{
	CXTPMarkupType* pTypeThis = GetType();
	return pTypeThis->IsDerivedFrom(pClass);
}

//////////////////////////////////////////////////////////////////////////
// CXTPMarkupAutoPtr


CXTPMarkupAutoPtr::CXTPMarkupAutoPtr(CXTPMarkupObject* pObject /*= NULL*/)
{
	m_pObject = pObject;
}

CXTPMarkupAutoPtr::~CXTPMarkupAutoPtr()
{
	MARKUP_RELEASE(m_pObject);
}

CXTPMarkupObject* CXTPMarkupAutoPtr::AddRef() const
{
	m_pObject->AddRef();
	return m_pObject;
}

CXTPMarkupObject* CXTPMarkupAutoPtr::operator->() const
{
	return m_pObject;
}


//////////////////////////////////////////////////////////////////////////
// CXTPMarkupInt

IMPLEMENT_MARKUPCLASS(NULL, CXTPMarkupInt, CXTPMarkupObject);

void CXTPMarkupInt::RegisterMarkupClass()
{

}

CXTPMarkupInt::CXTPMarkupInt(int nValue)
{
	m_nValue = nValue;
}

CXTPMarkupObject* CXTPMarkupInt::ConvertFrom(CXTPMarkupBuilder* /*pBuilder*/, CXTPMarkupObject* pObject) const
{
	if (IsStringObject(pObject))
	{
		return new CXTPMarkupInt(_wtoi(*((CXTPMarkupString*)pObject)));
	}

	return NULL;
}

BOOL CXTPMarkupInt::IsEqual(const CXTPMarkupObject* pObject) const
{
	if (!pObject)
		return FALSE;

	if (pObject->GetType() != MARKUP_TYPE(CXTPMarkupInt))
		return FALSE;

	return m_nValue == ((CXTPMarkupInt*)pObject)->m_nValue;
}

//////////////////////////////////////////////////////////////////////////
// CXTPMarkupDouble

IMPLEMENT_MARKUPCLASS(NULL, CXTPMarkupDouble, CXTPMarkupObject);

void CXTPMarkupDouble::RegisterMarkupClass()
{

}

CXTPMarkupDouble::CXTPMarkupDouble(double dValue)
{
	m_dValue = dValue;
}

CXTPMarkupObject* CXTPMarkupDouble::ConvertFrom(CXTPMarkupBuilder* /*pBuilder*/, CXTPMarkupObject* pObject) const
{
	if (IsStringObject(pObject))
	{
		LPCWSTR lpszValue = *(CXTPMarkupString*)pObject;

		double x = 0;
		if (!CXTPMarkupBuilder::ConvertDouble(lpszValue, x))
			return NULL;

		return new CXTPMarkupDouble(x);
	}

	return NULL;
}

BOOL CXTPMarkupDouble::IsEqual(const CXTPMarkupObject* pObject) const
{
	if (!pObject)
		return FALSE;

	if (pObject->GetType() != MARKUP_TYPE(CXTPMarkupDouble))
		return FALSE;

	return m_dValue == ((CXTPMarkupDouble*)pObject)->m_dValue;
}

//////////////////////////////////////////////////////////////////////////
// CXTPMarkupEnum

IMPLEMENT_MARKUPCLASS(NULL, CXTPMarkupEnum, CXTPMarkupObject);

void CXTPMarkupEnum::RegisterMarkupClass()
{

}

CXTPMarkupEnum::CXTPMarkupEnum(int nValue)
{
	m_nValue = nValue;
}

CXTPMarkupObject* CXTPMarkupEnum::ConvertFrom(CXTPMarkupBuilder* /*pBuilder*/, CXTPMarkupObject* /*pObject*/) const
{
	return NULL;
}

BOOL CXTPMarkupEnum::IsEqual(const CXTPMarkupObject* pObject) const
{
	if (!pObject)
		return FALSE;

	if (pObject->GetType() != MARKUP_TYPE(CXTPMarkupEnum))
		return FALSE;

	return m_nValue == ((CXTPMarkupEnum*)pObject)->m_nValue;
}

CXTPMarkupEnum* CXTPMarkupEnum::CreateValue(int nValue)
{
	static CXTPMarkupAutoPtr defaultValues[8];

	if (nValue > 7 || nValue < 0)
		return new CXTPMarkupEnum(nValue);

	CXTPMarkupObject*& pObject = defaultValues[nValue].m_pObject;

	if (pObject == NULL)
		pObject = new CXTPMarkupEnum(nValue);

	pObject->AddRef();
	return (CXTPMarkupEnum*)pObject;
}


//////////////////////////////////////////////////////////////////////////
// CXTPMarkupBool

IMPLEMENT_MARKUPCLASS(NULL, CXTPMarkupBool, CXTPMarkupObject);

void CXTPMarkupBool::RegisterMarkupClass()
{

}

CXTPMarkupBool::CXTPMarkupBool(BOOL bValue)
{
	m_bValue = bValue;
}

CXTPMarkupBool* CXTPMarkupBool::CreateTrueValue()
{
	static CXTPMarkupAutoPtr defaultValue(new CXTPMarkupBool(TRUE));
	return (CXTPMarkupBool*)defaultValue.AddRef();
}

CXTPMarkupBool* CXTPMarkupBool::CreateValue(BOOL bValue)
{
	return bValue ? CreateTrueValue() : CreateFalseValue();
}

CXTPMarkupBool* CXTPMarkupBool::CreateFalseValue()
{
	static CXTPMarkupAutoPtr defaultValue(new CXTPMarkupBool(FALSE));
	return (CXTPMarkupBool*)defaultValue.AddRef();
}

CXTPMarkupObject* CXTPMarkupBool::ConvertFrom(CXTPMarkupBuilder* /*pBuilder*/, CXTPMarkupObject* pObject) const
{
	if (IsStringObject(pObject))
	{
		if (_wcsicmp(*((CXTPMarkupString*)pObject), L"True") == 0)
		{
			return CreateTrueValue();
		}

		if (_wcsicmp(*((CXTPMarkupString*)pObject), L"False") == 0)
		{
			return CreateFalseValue();
		}
	}
	return FALSE;
}

UINT CXTPMarkupBool::GetHashKey() const
{
	return m_bValue;
}

BOOL CXTPMarkupBool::IsEqual(const CXTPMarkupObject* pObject) const
{
	if (!pObject)
		return FALSE;

	if (pObject->GetType() != MARKUP_TYPE(CXTPMarkupBool))
		return FALSE;

	if (m_bValue != ((CXTPMarkupBool*)pObject)->m_bValue)
		return FALSE;

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// CXTPMarkupColor

IMPLEMENT_MARKUPCLASS(L"Color", CXTPMarkupColor, CXTPMarkupObject);

void CXTPMarkupColor::RegisterMarkupClass()
{

}

CXTPMarkupColor::CXTPMarkupColor(COLORREF nValue)
{
	m_nValue = nValue | 0xFF000000;
}

CXTPMarkupColor::CXTPMarkupColor(BYTE bAlpha, COLORREF nValue)
{
	m_nValue = (nValue & 0xFFFFFF) | (bAlpha  << 24);
}


BOOL CXTPMarkupColor::ConvertFromString(LPCWSTR lpszValue, COLORREF& clr)
{
	if (lpszValue[0] == '#' && wcslen(lpszValue) == 7)
	{
		COLORREF clrRed = 0, clrGreen = 0, clrBlue = 0;
		if (WSCANF_S(lpszValue, L"#%2x%2x%2x", &clrRed, &clrGreen, &clrBlue) != 3)
			return FALSE;

		clr = RGB(clrRed, clrGreen, clrBlue) | 0xFF000000;
		return TRUE;
	}

	if (lpszValue[0] == '#' && wcslen(lpszValue) == 9)
	{
		COLORREF clrRed = 0, clrGreen = 0, clrBlue = 0, clrAlpha = 0;
		if (WSCANF_S(lpszValue, L"#%2x%2x%2x%2x", &clrAlpha, &clrRed, &clrGreen, &clrBlue) != 4)
			return FALSE;

		clr = RGB(clrRed, clrGreen, clrBlue) | (clrAlpha << 24);
		return TRUE;
	}

	if (lpszValue[0] != '#')
	{
		if (CXTPMarkupBuilder::ColorStringToKnownColor(lpszValue, clr))
		{
			return TRUE;
		}
	}
	return FALSE;
}

CXTPMarkupObject* CXTPMarkupColor::ConvertFrom(CXTPMarkupBuilder* /*pBuilder*/, CXTPMarkupObject* pObject) const
{
	if (IsStringObject(pObject))
	{
		COLORREF clr;
		if (ConvertFromString(*((CXTPMarkupString*)pObject), clr))
		{
			return new CXTPMarkupColor((BYTE)(clr >> 24), clr & 0xFFFFFF);
		}
	}

	return NULL;
}

BOOL CXTPMarkupColor::IsEqual(const CXTPMarkupObject* pObject) const
{
	if (!pObject)
		return FALSE;

	if (pObject->GetType() != MARKUP_TYPE(CXTPMarkupColor))
		return FALSE;

	return m_nValue == ((CXTPMarkupColor*)pObject)->m_nValue;
}

void CXTPMarkupColor::SetContentObject(CXTPMarkupBuilder* pBuilder, CXTPMarkupObject* pContent)
{
	if (IsStringObject(pContent))
	{
		COLORREF clr;
		if (ConvertFromString(*((CXTPMarkupString*)pContent), clr))
		{
			m_nValue = clr;
			MARKUP_RELEASE(pContent);
			return;
		}
	}

	CXTPMarkupObject::SetContentObject(pBuilder, pContent);
}


//////////////////////////////////////////////////////////////////////////
// CXTPMarkupString

IMPLEMENT_MARKUPCLASS(NULL, CXTPMarkupString, CXTPMarkupObject);

void CXTPMarkupString::RegisterMarkupClass()
{

}

CXTPMarkupString::CXTPMarkupString(LPCSTR lpszValue)
{
	Init(XTP_CT2CW(lpszValue), -1);
}

CXTPMarkupString::CXTPMarkupString(LPCWSTR lpszValue, int nLength)
{
	Init(lpszValue, nLength);
}

void CXTPMarkupString::Init(LPCWSTR lpszValue, int nLength)
{
	m_nLength = !lpszValue ? 0 : nLength == -1 ? (int)wcslen(lpszValue) : nLength;

	m_lpszValue = new WCHAR[m_nLength + 1];

	if (!lpszValue)
		m_lpszValue[0] = 0;
	else if (nLength == -1)
		memcpy(m_lpszValue, lpszValue, (m_nLength + 1) * sizeof(WCHAR));
	else
	{
		memcpy(m_lpszValue, lpszValue, nLength * sizeof(WCHAR));
		m_lpszValue[nLength] = 0;
	}
}

CXTPMarkupString* CXTPMarkupString::CreateValue(LPCWSTR lpszString, int nLength)
{
	if (lpszString == 0 || nLength == 0 || lpszString[0] == 0)
	{
		static CXTPMarkupAutoPtr stringEmpty(new CXTPMarkupString(NULL, 0));
		return (CXTPMarkupString*)stringEmpty.AddRef();
	}

	if (lpszString[1] != 0)
		return new CXTPMarkupString(lpszString, nLength);

	if (lpszString[0] == L' ')
	{
		static CXTPMarkupAutoPtr stringSpace(new CXTPMarkupString(L" ", 1));
		return (CXTPMarkupString*)stringSpace.AddRef();
	}

	if (lpszString[0] == L'\n')
	{
		static CXTPMarkupAutoPtr stringLineBreak(new CXTPMarkupString(L"\n", 1));
		return (CXTPMarkupString*)stringLineBreak.AddRef();
	}

	return new CXTPMarkupString(lpszString, nLength);
}

UINT CXTPMarkupString::GetHashKey() const
{
	LPCWSTR key = m_lpszValue;

	UINT nHash = 0;
	while (*key)
		nHash = (nHash<<5) + nHash + *key++;
	return nHash;
}

BOOL CXTPMarkupString::IsEqual(const CXTPMarkupObject* pObject) const
{
	if (!pObject)
		return FALSE;

	if (pObject->GetType() != MARKUP_TYPE(CXTPMarkupString))
		return FALSE;

	if (m_nLength != ((CXTPMarkupString*)pObject)->m_nLength)
		return FALSE;

	return wcscmp(m_lpszValue, ((CXTPMarkupString*)pObject)->m_lpszValue) == 0;
}


CXTPMarkupString::~CXTPMarkupString()
{
	delete[] m_lpszValue;
}

//////////////////////////////////////////////////////////////////////////
// CXTPMarkupCollection

IMPLEMENT_MARKUPCLASS(NULL, CXTPMarkupCollection, CXTPMarkupObject);

void CXTPMarkupCollection::RegisterMarkupClass()
{

}

CXTPMarkupCollection::CXTPMarkupCollection()
{
	m_pElementType = NULL;
	m_bLogicalParent = TRUE;
}

CXTPMarkupCollection::~CXTPMarkupCollection()
{
	RemoveAll();
}

void CXTPMarkupCollection::RemoveAll()
{
	for (int i = 0; i < m_arrItems.GetSize(); i++)
	{
		if (m_bLogicalParent) m_arrItems[i]->SetLogicalParent(NULL);
		m_arrItems[i]->Release();
	}
	m_arrItems.RemoveAll();

	OnChanged();
}

void CXTPMarkupCollection::Remove(int nIndex)
{
	if (nIndex < m_arrItems.GetSize())
	{
		CXTPMarkupObject* pItem = m_arrItems[nIndex];
		m_arrItems.RemoveAt(nIndex);

		if (m_bLogicalParent) pItem->SetLogicalParent(NULL);
		pItem->Release();

		OnChanged();
	}
}

void CXTPMarkupCollection::Insert(int nIndex, CXTPMarkupObject* pElement)
{
	ASSERT(pElement);
	ASSERT(m_pElementType);
	ASSERT(pElement->IsKindOf(m_pElementType));

	if (!pElement || !pElement->IsKindOf(m_pElementType))
		return;

	if (m_bLogicalParent)
	{
		ASSERT(pElement->GetLogicalParent() == NULL);
		pElement->SetLogicalParent(this);
	}

	if (nIndex < 0 || nIndex > (int)m_arrItems.GetSize())
		nIndex = (int)m_arrItems.GetSize();

	m_arrItems.InsertAt(nIndex, pElement);

	OnItemAdded(pElement, nIndex);
	OnChanged();
}

void CXTPMarkupCollection::Add(CXTPMarkupObject* pElement)
{
	Insert((int)m_arrItems.GetSize(), pElement);
}


int CXTPMarkupCollection::IndexOf(CXTPMarkupObject* pElement) const
{
	for (int i = 0; i < m_arrItems.GetSize(); i++)
	{
		if (pElement == m_arrItems[i])
			return i;
	}
	return -1;
}

void CXTPMarkupCollection::OnItemAdded(CXTPMarkupObject* /*pItem*/, int /*nIndex*/)
{

}

void CXTPMarkupCollection::OnChanged()
{

}

void CXTPMarkupCollection::SetContentObject(CXTPMarkupBuilder* pBuilder, CXTPMarkupObject* pContent)
{
	ASSERT(m_pElementType);

	if (pContent->IsKindOf(m_pElementType))
	{
		Add(pContent);
	}
	else
	{
		pBuilder->ThrowBuilderException(CXTPMarkupBuilder::FormatString(_T("'%ls' object cannot be added to '%ls'. Object cannot be converted to type '%ls'"),
			(LPCTSTR)pContent->GetType()->m_lpszClassName, (LPCTSTR)GetType()->m_lpszClassName, (LPCTSTR)m_pElementType->m_lpszClassName));
	}
}

BOOL CXTPMarkupCollection::HasContentObject() const
{
	return GetCount() > 0;
}

int CXTPMarkupCollection::GetLogicalChildrenCount() const
{
	return (int)m_arrItems.GetSize();
}

CXTPMarkupObject* CXTPMarkupCollection::GetLogicalChild(int nIndex) const
{
	return m_arrItems[nIndex];
}


//////////////////////////////////////////////////////////////////////////
// CXTPMarkupDoubleCollection

IMPLEMENT_MARKUPCLASS(L"DoubleCollection", CXTPMarkupDoubleCollection, CXTPMarkupObject)

void CXTPMarkupDoubleCollection::RegisterMarkupClass()
{

}

CXTPMarkupDoubleCollection::CXTPMarkupDoubleCollection()
{
}

CXTPMarkupDoubleCollection::CXTPMarkupDoubleCollection(CDoubleArray& arr)
{
	m_arr.Copy(arr);
}


int CXTPMarkupDoubleCollection::GetCount() const
{
	return (int)m_arr.GetSize();
}

const float* CXTPMarkupDoubleCollection::GetData() const
{
	return m_arr.GetData();
}

float CXTPMarkupDoubleCollection::GetAt(int nIndex) const
{
	return m_arr.GetAt(nIndex);
}

void CXTPMarkupDoubleCollection::RemoveAll()
{
	m_arr.RemoveAll();
}

void CXTPMarkupDoubleCollection::Remove(int nIndex)
{
	m_arr.RemoveAt(nIndex);
}

void CXTPMarkupDoubleCollection::Add(float fValue)
{
	m_arr.Add(fValue);
}


BOOL CXTPMarkupDoubleCollection::GetNextValue(LPCWSTR& lpszValue, float& fValue)
{
	while (*lpszValue == ' ') lpszValue++;
	LPCWSTR lpszNext = lpszValue;

	fValue = 0;
	int nSign = 1;
	if (*lpszNext == '-')
	{
		nSign = -1;
		lpszNext++;
	}

	if (*lpszNext == 0)
		return FALSE;

	int factor = 1;

	while (*lpszNext != 0)
	{
		WCHAR c = *lpszNext;

		if (c == ' ' || c == ',')
		{
			if (lpszNext == lpszValue)
				return FALSE;
			lpszNext++;
			break;
		}

		if (c == '.')
		{
			if (factor != 1)
				return FALSE;
			factor = 10;
			lpszNext++;
			continue;
		}

		if (c <'0' || c > '9')
			return FALSE;

		if (factor != 1)
		{
			fValue += float(c - '0') / factor;
			factor *= 10;
		}
		else
		{
			fValue = 10 * fValue + (c - '0');
		}

		lpszNext++;
	}

	lpszValue = lpszNext;
	fValue *= nSign;

	return TRUE;
}

BOOL CXTPMarkupDoubleCollection::ConvertFromString(LPCWSTR lpszValue, CDoubleArray& arr)
{
	if (!lpszValue)
		return FALSE;

	while (*lpszValue != 0)
	{
		float x;
		if (!GetNextValue(lpszValue, x))
			return FALSE;

		while (*lpszValue == ' ') lpszValue++;

		arr.Add(x);
	}

	return TRUE;
}

CXTPMarkupObject* CXTPMarkupDoubleCollection::ConvertFrom(CXTPMarkupBuilder* /*pBuilder*/, CXTPMarkupObject* pObject) const
{
	if (IsStringObject(pObject))
	{
		CDoubleArray arr;
		if (ConvertFromString(*((CXTPMarkupString*)pObject), arr))
		{
			return new CXTPMarkupDoubleCollection(arr);
		}
	}

	return NULL;
}




