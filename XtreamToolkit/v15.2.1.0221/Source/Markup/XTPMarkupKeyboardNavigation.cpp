// XTPMarkupKeyboardNavigation.cpp: implementation of the CXTPMarkupKeyboardNavigation class.
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
#include "Text/XTPMarkupInline.h"
#include "XTPMarkupKeyboardNavigation.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


CXTPMarkupDependencyProperty* CXTPMarkupKeyboardNavigation::m_pIsTabStopProperty = NULL;
CXTPMarkupDependencyProperty* CXTPMarkupKeyboardNavigation::m_pTabNavigationProperty = NULL;
CXTPMarkupDependencyProperty* CXTPMarkupKeyboardNavigation::m_pTabIndexProperty = NULL;

IMPLEMENT_MARKUPCLASS(L"KeyboardNavigation", CXTPMarkupKeyboardNavigation, CXTPMarkupObject)

void CXTPMarkupKeyboardNavigation::RegisterMarkupClass()
{
	m_pIsTabStopProperty = CXTPMarkupDependencyProperty::RegisterAttached(L"IsTabStop", MARKUP_TYPE(CXTPMarkupBool), MARKUP_TYPE(CXTPMarkupKeyboardNavigation),
		new CXTPMarkupPropertyMetadata(CXTPMarkupBool::CreateTrueValue()));

	m_pTabNavigationProperty = CXTPMarkupDependencyProperty::RegisterAttached(L"TabNavigation", MARKUP_TYPE(CXTPMarkupEnum), MARKUP_TYPE(CXTPMarkupKeyboardNavigation),
		new CXTPMarkupPropertyMetadata(NULL, &CXTPMarkupKeyboardNavigation::ConvertKeyboardNavigationMode));

	m_pTabIndexProperty = CXTPMarkupDependencyProperty::RegisterAttached(L"TabIndex", MARKUP_TYPE(CXTPMarkupInt), MARKUP_TYPE(CXTPMarkupKeyboardNavigation),
		new CXTPMarkupPropertyMetadata(new CXTPMarkupInt(INT_MAX)));
}

CXTPMarkupKeyboardNavigation::CXTPMarkupKeyboardNavigation(CXTPMarkupContext* pContext)
{
	m_pFocused = NULL;
	m_pLastFocused = NULL;

	m_pMarkupContext = pContext;
}

CXTPMarkupKeyboardNavigation::~CXTPMarkupKeyboardNavigation()
{
	MARKUP_RELEASE(m_pFocused);
	MARKUP_RELEASE(m_pLastFocused);
}

XTPMarkupKeyboardNavigationMode CXTPMarkupKeyboardNavigation::GetTabNavigation(CXTPMarkupObject* pElement)
{
	CXTPMarkupEnum* pValue = MARKUP_STATICCAST(CXTPMarkupEnum, pElement->GetValue(m_pTabNavigationProperty));
	return pValue ? (XTPMarkupKeyboardNavigationMode)(int)*pValue : xtpMarkupKeyboardNavigationContinue;
}

CXTPMarkupObject* CXTPMarkupKeyboardNavigation::ConvertKeyboardNavigationMode(CXTPMarkupBuilder* /*pBuilder*/, CXTPMarkupObject* pObject)
{
	if (IsStringObject(pObject))
	{
		LPCWSTR lpszValue = *((CXTPMarkupString*)pObject);
		int nLength = ((CXTPMarkupString*)pObject)->GetLength();

		if (nLength == 8 && _wcsicmp(lpszValue, L"Continue") == 0) return CXTPMarkupEnum::CreateValue(xtpMarkupKeyboardNavigationContinue);
		if (nLength == 4 && _wcsicmp(lpszValue, L"Once") == 0) return CXTPMarkupEnum::CreateValue(xtpMarkupKeyboardNavigationOnce);
		if (nLength == 5 && _wcsicmp(lpszValue, L"Cycle") == 0) return CXTPMarkupEnum::CreateValue(xtpMarkupKeyboardNavigationCycle);
		if (nLength == 4 && _wcsicmp(lpszValue, L"None") == 0) return CXTPMarkupEnum::CreateValue(xtpMarkupKeyboardNavigationNone);
		if (nLength == 9 && _wcsicmp(lpszValue, L"Contained") == 0) return CXTPMarkupEnum::CreateValue(xtpMarkupKeyboardNavigationContained);
		if (nLength == 5 && _wcsicmp(lpszValue, L"Local") == 0) return CXTPMarkupEnum::CreateValue(xtpMarkupKeyboardNavigationLocal);
	}

	return NULL;
}


BOOL CXTPMarkupKeyboardNavigation::OnWndMsg(UINT /*message*/, WPARAM /*wParam*/, LPARAM /*lParam*/, LRESULT* /*pResult*/)
{
	// NotImplmeneted
	return FALSE;

#if 0
	if (message == WM_SETFOCUS)
	{
		if (m_pLastFocused && !m_pFocused)
		{
			Focus(m_pLastFocused);
			MARKUP_RELEASE(m_pLastFocused);
		}
	}

	if (message == WM_KILLFOCUS)
	{
		if (m_pFocused)
		{
			m_pLastFocused = m_pFocused;
			MARKUP_ADDREF(m_pLastFocused);

			Focus(NULL);
		}
	}

	if (message == WM_KEYDOWN)
	{
		ProcessInput((UINT)wParam);
	}

	return FALSE;
#endif
}

BOOL CXTPMarkupKeyboardNavigation::IsInNavigationTree(CXTPMarkupObject* pVisual)
{
	CXTPMarkupUIElement* pElement = MARKUP_DYNAMICCAST(CXTPMarkupUIElement, pVisual);
	if ((pElement != NULL) && (pElement->IsVisible()))
	{
		return TRUE;
	}

	return FALSE;
}


CXTPMarkupObject* CXTPMarkupKeyboardNavigation::GetParent(CXTPMarkupObject* e)
{
	if (!e)
		return NULL;

	if (e->IsKindOf(MARKUP_TYPE(CXTPMarkupVisual)))
	{
		CXTPMarkupVisual* reference = ((CXTPMarkupVisual*)e)->GetVisualParent();
		while (reference != NULL)
		{
			if (IsInNavigationTree(reference))
			{
				return reference;
			}

			reference = reference->GetVisualParent();
		}
	}
	else if (e->IsKindOf(MARKUP_TYPE(CXTPMarkupFrameworkContentElement)))
	{
		return (CXTPMarkupObject*)(((CXTPMarkupFrameworkContentElement*)e)->GetParent());
	}

	return NULL;
}

XTPMarkupKeyboardNavigationMode CXTPMarkupKeyboardNavigation::GetKeyNavigationMode(CXTPMarkupObject* pElement)
{
	CXTPMarkupEnum* pValue = MARKUP_STATICCAST(CXTPMarkupEnum, pElement->GetValue(m_pNavigationProperty));
	return pValue ? (XTPMarkupKeyboardNavigationMode)(int)*pValue : xtpMarkupKeyboardNavigationContinue;
}

BOOL CXTPMarkupKeyboardNavigation::IsGroup(CXTPMarkupObject* e)
{
	return (GetKeyNavigationMode(e) != xtpMarkupKeyboardNavigationContinue);
}

CXTPMarkupObject* CXTPMarkupKeyboardNavigation::GetGroupParent(CXTPMarkupObject* e, BOOL bIncludeCurrent)
{
	CXTPMarkupObject* obj2 = e;
	if (!bIncludeCurrent)
	{
		obj2 = e;
		e = GetParent(e);
		if (e == NULL)
		{
			return obj2;
		}
	}
	while (e != NULL)
	{
		if (IsGroup(e))
		{
			return e;
		}
		obj2 = e;
		e = GetParent(e);
	}
	return obj2;
}

BOOL CXTPMarkupKeyboardNavigation::IsTabStopOrGroup(CXTPMarkupObject* e)
{
	return IsTabStop(e) || IsGroup(e);
}

CXTPMarkupObject* CXTPMarkupKeyboardNavigation::FocusedElement(CXTPMarkupObject* /*e*/)
{
	return NULL;
}

CXTPMarkupObject* CXTPMarkupKeyboardNavigation::GetTreeFirstChild(CXTPMarkupObject* e)
{
	CXTPMarkupObject* obj2 = FocusedElement(e);
	if (obj2 != NULL)
	{
		return obj2;
	}

	CXTPMarkupUIElement* o = MARKUP_DYNAMICCAST(CXTPMarkupUIElement, e);
	if (o != NULL && o->IsVisible())
	{
		CXTPMarkupVisual* reference = o;

		if (reference != NULL)
		{
			int childrenCount = reference->GetVisualChildrenCount();
			for (int i = 0; i < childrenCount; i++)
			{
				CXTPMarkupVisual* child = reference->GetVisualChild(i);
				if (IsInNavigationTree(child))
				{
					return child;
				}

				CXTPMarkupObject* firstChild = GetTreeFirstChild(child);
				if (firstChild != NULL)
				{
					return firstChild;
				}
			}
		}
	}
	return NULL;
}

CXTPMarkupObject* CXTPMarkupKeyboardNavigation::GetTreeNextSibling(CXTPMarkupObject* e)
{
	CXTPMarkupObject* parent = GetParent(e);

	{
		CXTPMarkupVisual* reference = MARKUP_DYNAMICCAST(CXTPMarkupVisual, parent);
		CXTPMarkupObject* obj4 = MARKUP_DYNAMICCAST(CXTPMarkupVisual, e);

		if ((reference != NULL) && obj4)
		{
			int childrenCount = reference->GetVisualChildrenCount();
			int childIndex = 0;
			while (childIndex < childrenCount)
			{
				if (reference->GetVisualChild(childIndex) == obj4)
				{
					break;
				}
				childIndex++;
			}
			childIndex++;
			while (childIndex < childrenCount)
			{
				CXTPMarkupObject* child = reference->GetVisualChild(childIndex);

				if (IsInNavigationTree(child))
				{
					return child;
				}
				childIndex++;
			}
		}
	}
	return NULL;
}

CXTPMarkupObject* CXTPMarkupKeyboardNavigation::GetNextInTree(CXTPMarkupObject* e, CXTPMarkupObject* container)
{
	CXTPMarkupObject* firstChild = NULL;
	if ((e == container) || !IsGroup(e))
	{
		firstChild = GetTreeFirstChild(e);
	}
	if ((firstChild != NULL) || (e == container))
	{
		return firstChild;
	}
	CXTPMarkupObject* parent = e;
	do
	{
		CXTPMarkupObject* nextSibling = GetTreeNextSibling(parent);
		if (nextSibling != NULL)
		{
			return nextSibling;
		}
		parent = GetParent(parent);
	}
	while ((parent != NULL) && (parent != container));
	return NULL;
}

int CXTPMarkupKeyboardNavigation::GetTabIndexHelper(CXTPMarkupObject* d)
{
	CXTPMarkupInt* pValue = MARKUP_STATICCAST(CXTPMarkupInt, d->GetValue(m_pTabIndexProperty));
	return pValue ? (int)*pValue : INT_MAX;
}

CXTPMarkupObject* CXTPMarkupKeyboardNavigation::GetFirstTabInGroup(CXTPMarkupObject* container)
{
	CXTPMarkupObject* obj2 = NULL;
	int num = INT_MIN;
	CXTPMarkupObject* e = container;
	while ((e = GetNextInTree(e, container)) != NULL)
	{
		if (IsTabStopOrGroup(e))
		{
			int tabIndexHelper = GetTabIndexHelper(e);
			if ((tabIndexHelper < num) || (obj2 == NULL))
			{
				num = tabIndexHelper;
				obj2 = e;
			}
		}
	}
	return obj2;
}

CXTPMarkupObject* CXTPMarkupKeyboardNavigation::GetNextTabWithSameIndex(CXTPMarkupObject* e, CXTPMarkupObject* container)
{
	int tabIndexHelper = GetTabIndexHelper(e);
	CXTPMarkupObject* obj2 = e;
	while ((obj2 = GetNextInTree(obj2, container)) != NULL)
	{
		if (IsTabStopOrGroup(obj2) && (GetTabIndexHelper(obj2) == tabIndexHelper))
		{
			return obj2;
		}
	}
	return NULL;
}

CXTPMarkupObject* CXTPMarkupKeyboardNavigation::GetNextTabWithNextIndex(CXTPMarkupObject* e, CXTPMarkupObject* container, XTPMarkupKeyboardNavigationMode tabbingType)
{
	CXTPMarkupObject* obj2 = NULL;
	CXTPMarkupObject* obj3 = NULL;
	int num = INT_MIN;
	int num2 = INT_MIN;
	int tabIndexHelper = GetTabIndexHelper(e);
	CXTPMarkupObject* obj4 = container;
	while ((obj4 = GetNextInTree(obj4, container)) != NULL)
	{
		if (IsTabStopOrGroup(obj4))
		{
			int num4 = GetTabIndexHelper(obj4);
			if ((num4 > tabIndexHelper) && ((num4 < num2) || (obj2 == NULL)))
			{
				num2 = num4;
				obj2 = obj4;
			}
			if ((num4 < num) || (obj3 == NULL))
			{
				num = num4;
				obj3 = obj4;
			}
		}
	}
	if ((tabbingType == xtpMarkupKeyboardNavigationCycle) && (obj2 == NULL))
	{
		obj2 = obj3;
	}
	return obj2;
}


CXTPMarkupObject* CXTPMarkupKeyboardNavigation::GetNextTabInGroup(CXTPMarkupObject* e, CXTPMarkupObject* container, XTPMarkupKeyboardNavigationMode tabbingType)
{
	if (tabbingType == xtpMarkupKeyboardNavigationNone)
	{
		return NULL;
	}
	if ((e == NULL) || (e == container))
	{
		return GetFirstTabInGroup(container);
	}
	if (tabbingType == xtpMarkupKeyboardNavigationOnce)
	{
		return NULL;
	}
	CXTPMarkupObject* nextTabWithSameIndex = GetNextTabWithSameIndex(e, container);
	if (nextTabWithSameIndex != NULL)
	{
		return nextTabWithSameIndex;
	}
	return GetNextTabWithNextIndex(e, container, tabbingType);
}

AFX_INLINE BOOL GetBoolValue(CXTPMarkupObject* e, CXTPMarkupDependencyProperty* p)
{
	CXTPMarkupBool* pValue = MARKUP_STATICCAST(CXTPMarkupBool, e->GetValue(p));
	return pValue ? (BOOL)*pValue : FALSE;
}

BOOL CXTPMarkupKeyboardNavigation::IsTabStop(CXTPMarkupObject* e)
{
	if (e->IsKindOf(MARKUP_TYPE(CXTPMarkupFrameworkElement)))
	{
		CXTPMarkupFrameworkElement* pElement = (CXTPMarkupFrameworkElement*)e;
		return pElement->IsFocusable() && GetBoolValue(pElement, m_pIsTabStopProperty) && pElement->IsEnabled() && pElement->IsVisible();
	}
	else if (e->IsKindOf(MARKUP_TYPE(CXTPMarkupFrameworkContentElement)))
	{
		return FALSE;
	}

	return FALSE;
}


CXTPMarkupObject* CXTPMarkupKeyboardNavigation::GetTabOnceActiveElement(CXTPMarkupObject* /*d*/)
{
	return NULL;
}

CXTPMarkupObject* CXTPMarkupKeyboardNavigation::GetActiveElement(CXTPMarkupObject* d)
{
	return GetTabOnceActiveElement(d);
}

CXTPMarkupObject* CXTPMarkupKeyboardNavigation::GetNextTab(CXTPMarkupObject* e, CXTPMarkupObject* pContainer, BOOL goDownOnly)
{
	XTPMarkupKeyboardNavigationMode keyNavigationMode = GetKeyNavigationMode(pContainer);
	if (e == NULL)
	{
		if (IsTabStop(pContainer))
		{
			return pContainer;
		}
		CXTPMarkupObject* pActiveElement = GetActiveElement(pContainer);
		if (pActiveElement)
		{
			return GetNextTab(NULL, pActiveElement, TRUE);
		}
	}
	else if (((keyNavigationMode == xtpMarkupKeyboardNavigationOnce) || (keyNavigationMode == xtpMarkupKeyboardNavigationNone)) && (pContainer != e))
	{
		if (goDownOnly)
		{
			return NULL;
		}
		CXTPMarkupObject* groupParent = GetGroupParent(pContainer);
		return GetNextTab(pContainer, groupParent, goDownOnly);
	}
	CXTPMarkupObject* obj4 = NULL;
	CXTPMarkupObject* obj5 = e;
	XTPMarkupKeyboardNavigationMode tabbingType = keyNavigationMode;
	while ((obj5 = GetNextTabInGroup(obj5, pContainer, tabbingType)) != NULL)
	{
		if (obj4 == obj5)
		{
			break;
		}
		if (obj4 == NULL)
		{
			obj4 = obj5;
		}
		CXTPMarkupObject* obj6 = GetNextTab(NULL, obj5, TRUE);
		if (obj6 != NULL)
		{
			return obj6;
		}
		if (tabbingType == xtpMarkupKeyboardNavigationOnce)
		{
			tabbingType = xtpMarkupKeyboardNavigationContained;
		}
	}
	if ((!goDownOnly && (tabbingType != xtpMarkupKeyboardNavigationContained)) && (GetParent(pContainer) != NULL))
	{
		return GetNextTab(pContainer, GetGroupParent(pContainer), FALSE);
	}
	return NULL;
}

void CXTPMarkupKeyboardNavigation::Navigate(CXTPMarkupInputElement* pCurrentElement, XTPMarkupFocusNavigationDirection direction)
{
	CXTPMarkupObject* pNextInDirection = NULL;
	switch (direction)
	{
	case xtpMarkupFocusNavigationDirectionNext:
		m_pNavigationProperty = m_pTabNavigationProperty;
		pNextInDirection = GetNextTab(pCurrentElement, GetGroupParent(pCurrentElement, TRUE), FALSE);
		break;

   /*case xtpMarkupFocusNavigationDirectionPrevious:
		m_pNavigationProperty = m_pTabNavigationProperty;
		pNextInDirection = GetPrevTab(pCurrentElement, NULL, FALSE);
		break;*/
	}

	if (pNextInDirection != NULL)
	{
		if (pNextInDirection->IsKindOf(MARKUP_TYPE(CXTPMarkupInputElement)))
		{
			((CXTPMarkupInputElement*)pNextInDirection)->Focus();
		}
	}
}

void CXTPMarkupKeyboardNavigation::Navigate(CXTPMarkupInputElement* pSourceElement, UINT nChar)
{
	if (nChar == VK_TAB)
	{
		Navigate(pSourceElement, (::GetKeyState(VK_SHIFT) & 0x8000) == 0 ? xtpMarkupFocusNavigationDirectionNext : xtpMarkupFocusNavigationDirectionPrevious);
	}
}

void CXTPMarkupKeyboardNavigation::ProcessInput(UINT nChar)
{
	if (!m_pFocused)
		return;

	if (nChar == VK_TAB)
	{
		Navigate(m_pFocused, nChar);
	}
}

void CXTPMarkupKeyboardNavigation::Focus(CXTPMarkupInputElement* /*pInputElement*/)
{
	// NotImplmeneted
	return;

#if 0
	if (!m_pMarkupContext->m_hContextWnd)
		return;

	if (m_pFocused == pInputElement)
		return;

	if (m_pFocused)
	{
		m_pFocused->SetValue(CXTPMarkupInputElement::m_pIsKeyboardFocusedProperty, NULL);

		MARKUP_RELEASE(m_pFocused);
	}

	m_pFocused = pInputElement;

	if (m_pFocused)
	{
		m_pFocused->SetValue(CXTPMarkupInputElement::m_pIsKeyboardFocusedProperty, CXTPMarkupBool::CreateTrueValue());

		MARKUP_ADDREF(m_pFocused);

		::SetFocus(m_pMarkupContext->m_hContextWnd);
	}
#endif
}
