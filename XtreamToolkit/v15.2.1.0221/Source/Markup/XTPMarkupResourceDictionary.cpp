// XTPMarkupResourceDictionary.cpp: implementation of the CXTPMarkupResourceDictionary class.
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

#include "XTPMarkupObject.h"
#include "XTPMarkupInputElement.h"
#include "XTPMarkupUIElement.h"
#include "XTPMarkupFrameworkElement.h"

#include "XTPMarkupResourceDictionary.h"
#include "XTPMarkupBuilder.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////////
// CXTPMarkupResourceDictionary

IMPLEMENT_MARKUPCLASS(NULL, CXTPMarkupResourceDictionary, CXTPMarkupObject)

void CXTPMarkupResourceDictionary::RegisterMarkupClass()
{

}

CXTPMarkupResourceDictionary::CXTPMarkupResourceDictionary()
{

}

CXTPMarkupResourceDictionary::~CXTPMarkupResourceDictionary()
{
	CXTPMarkupObject* pKey;
	CXTPMarkupObject* pValue;

	POSITION pos = m_mapDictionary.GetStartPosition();
	while (pos)
	{
		m_mapDictionary.GetNextAssoc(pos, pKey, pValue);

		MARKUP_RELEASE(pKey);
		MARKUP_RELEASE(pValue);
	}
}

CXTPMarkupObject* CXTPMarkupResourceDictionary::Lookup(const CXTPMarkupObject* pKey) const
{
	CXTPMarkupObject* pValue;

	if (m_mapDictionary.Lookup((CXTPMarkupObject*)pKey, pValue))
		return pValue;

	return NULL;
}

void CXTPMarkupResourceDictionary::Add(CXTPMarkupObject* pKey, CXTPMarkupObject* pValue)
{
	m_mapDictionary.SetAt(pKey, pValue);
}

CXTPMarkupObject* CXTPMarkupResourceDictionary::GetObjectKey(CXTPMarkupObject* pContent) const
{
	if (!pContent)
		return NULL;

	CXTPMarkupObject* pKey = pContent->GetValue(m_pKeyProperty);
	if (pKey)
		return pKey;

	if (MARKUP_DYNAMICCAST(CXTPMarkupStyle, pContent))
	{
		return pContent->GetValue(CXTPMarkupStyle::m_pTargetTypeProperty);
	}

	return NULL;

}

void CXTPMarkupResourceDictionary::SetContentObject(CXTPMarkupBuilder* pBuilder, CXTPMarkupObject* pContent)
{
	CXTPMarkupObject* pKey = GetObjectKey(pContent);

	if (pKey == NULL)
	{
		pBuilder->ThrowBuilderException(_T("Objects added to Dictionary must have Key attribute or some other type of associated Key"));
	}

	if (Lookup(pKey))
	{
		pBuilder->ThrowBuilderException(_T("Dictionary key is already used. Key attributes must be unique."));
	}

	MARKUP_ADDREF(pKey);
	Add(pKey, pContent);
}

CXTPMarkupObject* CXTPMarkupResourceDictionary::FindResource(const CXTPMarkupObject* pElement, const CXTPMarkupObject* pKey)
{
	if (pElement->IsKindOf(MARKUP_TYPE(CXTPMarkupResourceDictionary)))
	{
		return ((CXTPMarkupResourceDictionary*)pElement)->Lookup(pKey);
	}

	while (pElement)
	{
		CXTPMarkupResourceDictionary* pResources = MARKUP_STATICCAST(CXTPMarkupResourceDictionary, pElement->GetValue(CXTPMarkupStyle::m_pResourcesProperty));

		if (pResources)
		{
			CXTPMarkupObject* pValue = pResources->Lookup(pKey);
			if (pValue)
				return pValue;
		}

		CXTPMarkupStyle* pStyle = MARKUP_STATICCAST(CXTPMarkupStyle, pElement->GetValue(CXTPMarkupFrameworkElement::m_pStyleProperty));
		if (pStyle)
		{
			CXTPMarkupObject* pValue = pStyle->FindResource(pKey);
			if (pValue)
				return pValue;
		}

		if (pElement->GetType() == MARKUP_TYPE(CXTPMarkupStyle))
			return NULL;

		pElement = pElement->GetLogicalParent();
	}

	return NULL;
}

//////////////////////////////////////////////////////////////////////////
// CXTPMarkupSetter
CXTPMarkupDependencyProperty* CXTPMarkupSetter::m_pPropertyProperty = NULL;
CXTPMarkupDependencyProperty* CXTPMarkupSetter::m_pValueProperty = NULL;

IMPLEMENT_MARKUPCLASS(L"Setter", CXTPMarkupSetter, CXTPMarkupObject);

void CXTPMarkupSetter::RegisterMarkupClass()
{
	m_pPropertyProperty = CXTPMarkupDependencyProperty::Register(L"Property", MARKUP_TYPE(CXTPMarkupObject), MARKUP_TYPE(CXTPMarkupSetter));
	m_pValueProperty = CXTPMarkupDependencyProperty::Register(L"Value", MARKUP_TYPE(CXTPMarkupObject), MARKUP_TYPE(CXTPMarkupSetter));
}

CXTPMarkupSetter::CXTPMarkupSetter()
{

}

CXTPMarkupSetter::CXTPMarkupSetter(CXTPMarkupDependencyProperty* pProperty, CXTPMarkupObject* pValue)
{
	ASSERT(pProperty && pValue);

	pProperty->AddRef();
	SetValue(m_pPropertyProperty, pProperty);
	SetValue(m_pValueProperty, pValue);
}


//////////////////////////////////////////////////////////////////////////
// CXTPMarkupSetterColection

IMPLEMENT_MARKUPCLASS(NULL, CXTPMarkupSetterColection, CXTPMarkupCollection);

void CXTPMarkupSetterColection::RegisterMarkupClass()
{

}

CXTPMarkupSetterColection::CXTPMarkupSetterColection()
{
	m_pElementType = MARKUP_TYPE(CXTPMarkupSetter);
}

//////////////////////////////////////////////////////////////////////////
// CXTPMarkupStyle

CXTPMarkupDependencyProperty* CXTPMarkupStyle::m_pTargetTypeProperty = NULL;
CXTPMarkupDependencyProperty* CXTPMarkupStyle::m_pBasedOnProperty = NULL;
CXTPMarkupDependencyProperty* CXTPMarkupStyle::m_pResourcesProperty = NULL;
CXTPMarkupDependencyProperty* CXTPMarkupStyle::m_pTriggersProperty = NULL;

IMPLEMENT_MARKUPCLASS(L"Style", CXTPMarkupStyle, CXTPMarkupObject)

void CXTPMarkupStyle::RegisterMarkupClass()
{
	CXTPMarkupSetter::RegisterType();
	CXTPMarkupTrigger::RegisterType();

	m_pResourcesProperty = CXTPMarkupDependencyProperty::Register(L"Resources", MARKUP_TYPE(CXTPMarkupResourceDictionary), MARKUP_TYPE(CXTPMarkupStyle));

	m_pTargetTypeProperty = CXTPMarkupDependencyProperty::Register(L"TargetType", MARKUP_TYPE(CXTPMarkupType), MARKUP_TYPE(CXTPMarkupStyle));
	m_pBasedOnProperty = CXTPMarkupDependencyProperty::Register(L"BasedOn", MARKUP_TYPE(CXTPMarkupStyle), MARKUP_TYPE(CXTPMarkupStyle));

	m_pTriggersProperty = CXTPMarkupDependencyProperty::Register(L"Triggers", MARKUP_TYPE(CXTPMarkupTriggerCollection), MARKUP_TYPE(CXTPMarkupStyle));
}


CXTPMarkupStyle::CXTPMarkupStyle()
{
	m_pSetters = new CXTPMarkupSetterColection();
	m_pSetters->SetLogicalParent(this);

	m_bSealed = FALSE;

	m_pProperties = NULL;
}

CXTPMarkupStyle::~CXTPMarkupStyle()
{
	if (m_pSetters)
	{
		m_pSetters->SetLogicalParent(NULL);
		MARKUP_RELEASE(m_pSetters);
	}

	MARKUP_RELEASE(m_pProperties);
}


void CXTPMarkupStyle::ResolveTriggerProperty(CXTPMarkupBuilder* pBuilder, CXTPMarkupTrigger* pTrigger)
{
	ResolveSetterProperty(pBuilder, pTrigger);

	CXTPMarkupSetterColection* pSetters = pTrigger->GetSetters();
	int nCount = pSetters ? pSetters->GetCount() : 0;

	for (int i = 0; i < nCount; i++)
	{
		ResolveSetterProperty(pBuilder, pSetters->GetItem(i));
	}
}

void CXTPMarkupStyle::ResolveSetterProperty(CXTPMarkupBuilder* pBuilder, CXTPMarkupObject* pSetter)
{
	CXTPMarkupType* pTargetType = MARKUP_STATICCAST(CXTPMarkupType, GetValue(m_pTargetTypeProperty));

	CXTPMarkupDependencyProperty* pProperty = (CXTPMarkupDependencyProperty*)pSetter->GetValue(CXTPMarkupSetter::m_pPropertyProperty);
	if (!pProperty)
	{
		pBuilder->ThrowBuilderException(_T("Must specify both Property and Value for Setter."));
	}

	if (pProperty->GetType() == MARKUP_TYPE(CXTPMarkupDependencyProperty))
	{

	}
	else if (IsStringObject(pProperty))
	{
		LPCWSTR lpszTagName = *((CXTPMarkupString*)pProperty);

		pProperty = pBuilder->FindProperty(pTargetType, lpszTagName);

		if (pProperty == NULL)
		{
			pBuilder->ThrowBuilderException(pBuilder->FormatString(_T("Cannot find the Style Property '%ls'"), (LPCTSTR)lpszTagName));
		}

		pProperty->AddRef();
		pSetter->SetValue(CXTPMarkupSetter::m_pPropertyProperty, pProperty);
	}
	else
	{
		pBuilder->ThrowBuilderException(_T("Must specify both Property and Value for Setter."));
	}

	CXTPMarkupObject* pValue = pSetter->GetValue(CXTPMarkupSetter::m_pValueProperty);
	if (!pValue)
	{
		pBuilder->ThrowBuilderException(_T("Must specify both Property and Value for Setter."));
	}

	if (!pValue->IsKindOf(pProperty->GetPropetyType()))
	{
		CXTPMarkupObject* pNewValue = pBuilder->ConvertValue(pProperty, pValue);
		pSetter->SetValue(CXTPMarkupSetter::m_pValueProperty, pNewValue);
	}

}

void CXTPMarkupStyle::SetContentObject(CXTPMarkupBuilder* pBuilder, CXTPMarkupObject* pContent)
{
	if (pContent->IsKindOf(MARKUP_TYPE(CXTPMarkupSetter)))
	{
		ResolveSetterProperty(pBuilder, (CXTPMarkupSetter*)pContent);
		m_pSetters->SetContentObject(pBuilder, pContent);
	}
	else
	{
		CXTPMarkupObject::SetContentObject(pBuilder, pContent);
	}
}

void CXTPMarkupStyle::SetPropertyObject(CXTPMarkupBuilder* pBuilder, CXTPMarkupDependencyProperty* pProperty, CXTPMarkupObject* pValue)
{
	if (pProperty == m_pTriggersProperty)
	{
		if (pValue && pValue->IsKindOf(MARKUP_TYPE(CXTPMarkupTriggerCollection)))
		{
			CXTPMarkupTriggerCollection* pTriggers = (CXTPMarkupTriggerCollection*)pValue;
			for (int i = 0; i < pTriggers->GetCount(); i++)
			{
				ResolveTriggerProperty(pBuilder, pTriggers->GetItem(i));
			}
		}
	}

	CXTPMarkupObject::SetPropertyObject(pBuilder, pProperty, pValue);
}

void CXTPMarkupStyle::Seal()
{
	if (m_bSealed)
		return;

	ASSERT(!m_pProperties);

	if (!m_pProperties)
		m_pProperties = new CXTPMarkupProperties(NULL);

	SetLogicalParent(NULL);

	CXTPMarkupStyle* pStyle = GetBasedStyle();
	if (pStyle)
	{
		pStyle->Seal();

		m_pProperties->Copy(pStyle->m_pProperties);
	}


	int nCount = m_pSetters->GetCount();
	for (int i = 0; i < nCount; i++)
	{
		CXTPMarkupSetter* pSetter = m_pSetters->GetItem(i);

		CXTPMarkupObject* pValue = pSetter->GetSetterValue();
		MARKUP_ADDREF(pValue);

		m_pProperties->Set(pSetter->GetSetterProperty(), pValue);
	}


	m_bSealed = TRUE;
}

CXTPMarkupObject* CXTPMarkupStyle::GetStyleValue(CXTPMarkupDependencyProperty* pProperty) const
{
	return m_pProperties ? m_pProperties->Lookup(pProperty) : NULL;
}

void CXTPMarkupType::SetTypeStyle(CXTPMarkupStyle* pStyle)
{
	if (m_pTypeStyle)
	{
		m_pTypeStyle->Release();
	}

	m_pTypeStyle = pStyle;

	if (pStyle)
	{
		pStyle->Seal();
	}
}

CXTPMarkupObject* CXTPMarkupStyle::FindResource(const CXTPMarkupObject* pKey) const
{
	return CXTPMarkupResourceDictionary::FindResource(this, pKey);
}

//////////////////////////////////////////////////////////////////////////
// CXTPMarkupTrigger

CXTPMarkupDependencyProperty* CXTPMarkupTrigger::m_pPropertyProperty = NULL;
CXTPMarkupDependencyProperty* CXTPMarkupTrigger::m_pSettersProperty = NULL;
CXTPMarkupDependencyProperty* CXTPMarkupTrigger::m_pValueProperty = NULL;


IMPLEMENT_MARKUPCLASS(L"Trigger", CXTPMarkupTrigger, CXTPMarkupObject)

void CXTPMarkupTrigger::RegisterMarkupClass()
{
	CXTPMarkupSetter::RegisterType();

	m_pPropertyProperty = CXTPMarkupSetter::m_pPropertyProperty->AddOwner(MARKUP_TYPE(CXTPMarkupTrigger));
	m_pSettersProperty = CXTPMarkupDependencyProperty::Register(L"Setters", MARKUP_TYPE(CXTPMarkupSetterColection), MARKUP_TYPE(CXTPMarkupTrigger));
	m_pValueProperty = CXTPMarkupSetter::m_pValueProperty->AddOwner(MARKUP_TYPE(CXTPMarkupTrigger));
}

CXTPMarkupTrigger::CXTPMarkupTrigger()
{

}

CXTPMarkupTrigger::CXTPMarkupTrigger(CXTPMarkupDependencyProperty* pProperty, CXTPMarkupObject* pValue)
{
	ASSERT(pProperty && pValue);

	pProperty->AddRef();
	SetValue(m_pPropertyProperty, pProperty);
	SetValue(m_pValueProperty, pValue);
}


//////////////////////////////////////////////////////////////////////////
// CXTPMarkupTriggerCollection
IMPLEMENT_MARKUPCLASS(NULL, CXTPMarkupTriggerCollection, CXTPMarkupCollection)

void CXTPMarkupTriggerCollection::RegisterMarkupClass()
{

}

CXTPMarkupTriggerCollection::CXTPMarkupTriggerCollection()
{
	m_pElementType = MARKUP_TYPE(CXTPMarkupTrigger);
}
