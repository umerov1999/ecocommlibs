// XTPMarkupInputElement.cpp: implementation of the CXTPMarkupInputElement class.
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

#include "stdafx.h"

#include "XTPMarkupObject.h"
#include "XTPMarkupInputElement.h"
#include "XTPMarkupContext.h"
#include "XTPMarkupBuilder.h"
#include "XTPMarkupUIElement.h"
#include "XTPMarkupResourceDictionary.h"
#include "XTPMarkupKeyboardNavigation.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPMarkupRoutedEvent

CXTPMarkupRoutedEvent::CXTPMarkupRoutedEvent()
{

}

CXTPMarkupRoutedEvent* CXTPMarkupRoutedEvent::RegisterRoutedEvent(LPCWSTR lpszName, RoutingStrategy routingStrategy, CXTPMarkupType* pOwnerType)
{
	CXTPMarkupRoutedEvent* pEvent = (CXTPMarkupRoutedEvent*)RegisterCommon(new CXTPMarkupRoutedEvent(), lpszName, MARKUP_TYPE(CXTPMarkupString), pOwnerType, FALSE);

	if (routingStrategy != routingDirect)
	{
		pEvent->m_pMetadata = new CXTPMarkupPropertyMetadata(NULL, routingStrategy);
	}

	return pEvent;
}

//////////////////////////////////////////////////////////////////////////
// CXTPMarkupRoutedEventArgs

CXTPMarkupRoutedEventArgs::CXTPMarkupRoutedEventArgs(CXTPMarkupRoutedEvent* pEvent, CXTPMarkupObject* pSource)
{
	m_pSource = pSource;
	m_pEvent = pEvent;
	m_bHandled = FALSE;

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPMarkupRoutedEventArgs::CXTPMarkupRoutedEventArgs()
{
	m_pSource = NULL;
	m_pEvent = NULL;
	m_bHandled = FALSE;

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPMarkupMouseEventArgs::CXTPMarkupMouseEventArgs(CXTPMarkupRoutedEvent* pEvent)
	: CXTPMarkupRoutedEventArgs(pEvent, NULL)
{
	m_hWnd = 0;
	m_point = 0;
}

CPoint CXTPMarkupMouseEventArgs::GetPosition(CXTPMarkupInputElement* relativeTo)
{
	CPoint pt = m_point;

	CXTPMarkupObject* pVisual = relativeTo;

	while (pVisual != 0)
	{
		if (pVisual->IsKindOf(MARKUP_TYPE(CXTPMarkupVisual)))
		{
			pt -= ((CXTPMarkupVisual*)pVisual)->GetVisualOffset();
		}
		pVisual = pVisual->GetLogicalParent();
	}
	return pt;
}


CXTPMarkupMouseButtonEventArgs::CXTPMarkupMouseButtonEventArgs(CXTPMarkupRoutedEvent* pEvent)
	: CXTPMarkupMouseEventArgs(pEvent)
{

}

CXTPMarkupQueryCursorEventArgs::CXTPMarkupQueryCursorEventArgs()
	: CXTPMarkupMouseEventArgs(NULL)
{

}

CXTPMarkupMouseWheelEventArgs::CXTPMarkupMouseWheelEventArgs()
	: CXTPMarkupMouseEventArgs(NULL)
{

}




//////////////////////////////////////////////////////////////////////////
// CXTPMarkupEventHandlerMap

CXTPMarkupEventHandlerMap::CXTPMarkupEventHandlerMap()
{

}

CXTPMarkupEventHandlerMap::~CXTPMarkupEventHandlerMap()
{
	RemoveAll();
}

void CXTPMarkupEventHandlerMap::RemoveAll()
{
	POSITION pos = m_mapHandlers.GetStartPosition();
	while (pos)
	{
		CPtrList* pHandlers;
		CXTPMarkupRoutedEvent* pEvent;
		m_mapHandlers.GetNextAssoc(pos, pEvent, pHandlers);

		while (!pHandlers->IsEmpty())
		{
			CXTPMarkupDelegate* pDelegate = (CXTPMarkupDelegate*)pHandlers->RemoveTail();
			MARKUP_RELEASE(pDelegate);
		}
		delete pHandlers;
	}
	m_mapHandlers.RemoveAll();
}

void CXTPMarkupEventHandlerMap::Add(CXTPMarkupRoutedEvent* pEvent, CXTPMarkupDelegate* pDelegate)
{
	CPtrList* pHandlers;
	if (!m_mapHandlers.Lookup(pEvent, pHandlers))
	{
		pHandlers  = new CPtrList();
		m_mapHandlers.SetAt(pEvent, pHandlers);
	}

	pHandlers->AddTail(pDelegate);
}

void CXTPMarkupEventHandlerMap::Raise(CXTPMarkupObject* pSender, CXTPMarkupRoutedEventArgs* pEventArgs)
{
	CPtrList* pHandlers;
	if (!m_mapHandlers.Lookup(pEventArgs->GetEvent(), pHandlers))
		return;

	POSITION pos = pHandlers->GetHeadPosition();

	while (pos)
	{
		CXTPMarkupDelegate* pDelegate = (CXTPMarkupDelegate*)pHandlers->GetNext(pos);

		pDelegate->Execute(pSender, pEventArgs);
	}
}


//////////////////////////////////////////////////////////////////////////
// CXTPMarkupInputElement

CXTPMarkupDependencyProperty* CXTPMarkupInputElement::m_pCursorProperty = NULL;
CXTPMarkupDependencyProperty* CXTPMarkupInputElement::m_pIsMouseOverProperty = NULL;
CXTPMarkupDependencyProperty* CXTPMarkupInputElement::m_pIsKeyboardFocusedProperty = NULL;
CXTPMarkupDependencyProperty* CXTPMarkupInputElement::m_pResourcesProperty = NULL;
CXTPMarkupDependencyProperty* CXTPMarkupInputElement::m_pStyleProperty = NULL;
CXTPMarkupDependencyProperty* CXTPMarkupInputElement::m_pIsFocusableProperty = NULL;
CXTPMarkupDependencyProperty* CXTPMarkupInputElement::m_pToolTipProperty = NULL;

CXTPMarkupRoutedEvent* CXTPMarkupInputElement::m_pMouseLeaveEvent = NULL;
CXTPMarkupRoutedEvent* CXTPMarkupInputElement::m_pMouseEnterEvent = NULL;
CXTPMarkupRoutedEvent* CXTPMarkupInputElement::m_pMouseLeftButtonUpEvent = NULL;
CXTPMarkupRoutedEvent* CXTPMarkupInputElement::m_pMouseLeftButtonDownEvent = NULL;
CXTPMarkupRoutedEvent* CXTPMarkupInputElement::m_pMouseRightButtonUpEvent = NULL;
CXTPMarkupRoutedEvent* CXTPMarkupInputElement::m_pMouseRightButtonDownEvent = NULL;
CXTPMarkupRoutedEvent* CXTPMarkupInputElement::m_pMouseMoveEvent = NULL;
CXTPMarkupRoutedEvent* CXTPMarkupInputElement::m_pLostMouseCaptureEvent = NULL;


IMPLEMENT_MARKUPCLASS(NULL, CXTPMarkupInputElement, CXTPMarkupObject);

void CXTPMarkupInputElement::RegisterMarkupClass()
{
	CXTPMarkupStyle::RegisterType();

	m_pCursorProperty = CXTPMarkupDependencyProperty::Register(L"Cursor", MARKUP_TYPE(CXTPMarkupInt), MARKUP_TYPE(CXTPMarkupInputElement),
		new CXTPMarkupPropertyMetadata(NULL, &CXTPMarkupBuilder::ConvertCursor, &CXTPMarkupInputElement::OnCursorPropertyChanged));

	m_pIsMouseOverProperty = CXTPMarkupDependencyProperty::Register(L"IsMouseOver", MARKUP_TYPE(CXTPMarkupBool), MARKUP_TYPE(CXTPMarkupInputElement),
		new CXTPMarkupPropertyMetadata(CXTPMarkupBool::CreateFalseValue()));

	m_pResourcesProperty = CXTPMarkupStyle::m_pResourcesProperty->AddOwner(MARKUP_TYPE(CXTPMarkupInputElement));

	m_pStyleProperty = CXTPMarkupDependencyProperty::Register(L"Style", MARKUP_TYPE(CXTPMarkupStyle), MARKUP_TYPE(CXTPMarkupInputElement),
		new CXTPMarkupPropertyMetadata(NULL, &CXTPMarkupInputElement::OnStylePropertyChanged));

	m_pIsKeyboardFocusedProperty = CXTPMarkupDependencyProperty::Register(L"IsKeyboardFocused", MARKUP_TYPE(CXTPMarkupBool), MARKUP_TYPE(CXTPMarkupInputElement),
		new CXTPMarkupPropertyMetadata(CXTPMarkupBool::CreateFalseValue(), CXTPMarkupPropertyMetadata::flagAffectsRender));

	m_pIsFocusableProperty = CXTPMarkupDependencyProperty::Register(L"IsFocusable", MARKUP_TYPE(CXTPMarkupBool), MARKUP_TYPE(CXTPMarkupInputElement),
		new CXTPMarkupPropertyMetadata(CXTPMarkupBool::CreateFalseValue()));

	m_pToolTipProperty = CXTPMarkupDependencyProperty::Register(L"ToolTip", MARKUP_TYPE(CXTPMarkupObject), MARKUP_TYPE(CXTPMarkupInputElement));

	m_pMouseLeaveEvent = CXTPMarkupRoutedEvent::RegisterRoutedEvent(L"MouseLeave",
		CXTPMarkupRoutedEvent::routingDirect, MARKUP_TYPE(CXTPMarkupInputElement));
	m_pMouseEnterEvent = CXTPMarkupRoutedEvent::RegisterRoutedEvent(L"MouseEnter",
		CXTPMarkupRoutedEvent::routingDirect, MARKUP_TYPE(CXTPMarkupInputElement));
	m_pMouseLeftButtonUpEvent = CXTPMarkupRoutedEvent::RegisterRoutedEvent(L"MouseLeftButtonUp",
		CXTPMarkupRoutedEvent::routingDirect, MARKUP_TYPE(CXTPMarkupInputElement));
	m_pMouseLeftButtonDownEvent = CXTPMarkupRoutedEvent::RegisterRoutedEvent(L"MouseLeftButtonDown",
		CXTPMarkupRoutedEvent::routingDirect, MARKUP_TYPE(CXTPMarkupInputElement));
	m_pMouseRightButtonUpEvent = CXTPMarkupRoutedEvent::RegisterRoutedEvent(L"MouseRightButtonUp",
		CXTPMarkupRoutedEvent::routingDirect, MARKUP_TYPE(CXTPMarkupInputElement));
	m_pMouseRightButtonDownEvent = CXTPMarkupRoutedEvent::RegisterRoutedEvent(L"MouseRightButtonDown",
		CXTPMarkupRoutedEvent::routingDirect, MARKUP_TYPE(CXTPMarkupInputElement));
	m_pMouseMoveEvent = CXTPMarkupRoutedEvent::RegisterRoutedEvent(L"MouseMove",
		CXTPMarkupRoutedEvent::routingDirect, MARKUP_TYPE(CXTPMarkupInputElement));
	m_pLostMouseCaptureEvent= CXTPMarkupRoutedEvent::RegisterRoutedEvent(L"LostMouseCapture",
		CXTPMarkupRoutedEvent::routingDirect, MARKUP_TYPE(CXTPMarkupInputElement));
}

CXTPMarkupInputElement::CXTPMarkupInputElement()
{
	m_pMarkupContext = NULL;
	m_pHandlers = NULL;

	m_pActiveTriggers = NULL;
	m_pTriggerProperties = NULL;
	m_pStyleCache = NULL;

}

CXTPMarkupInputElement::~CXTPMarkupInputElement()
{
	SAFE_DELETE(m_pHandlers);

	MARKUP_RELEASE(m_pActiveTriggers);
	MARKUP_RELEASE(m_pTriggerProperties);
}

void CXTPMarkupInputElement::OnMouseLeave(CXTPMarkupMouseEventArgs* /*e*/)
{
	SetValue(m_pIsMouseOverProperty, NULL);
}

void CXTPMarkupInputElement::OnMouseEnter(CXTPMarkupMouseEventArgs* /*e*/)
{
	SetValue(m_pIsMouseOverProperty, CXTPMarkupBool::CreateTrueValue());
}

void CXTPMarkupInputElement::OnMouseMove(CXTPMarkupMouseEventArgs* /*e*/)
{
}

void CXTPMarkupInputElement::OnQueryCursor(CXTPMarkupQueryCursorEventArgs* e)
{
	CXTPMarkupInt* pCursor = MARKUP_STATICCAST(CXTPMarkupInt, GetValue(m_pCursorProperty));
	if (pCursor)
	{
		e->SetHandled();
		e->m_hCursor = AfxGetApp()->LoadStandardCursor(MAKEINTRESOURCE((int)*pCursor));
	}
}

void CXTPMarkupInputElement::OnMouseWheel(CXTPMarkupMouseWheelEventArgs* /*e*/)
{

}

void CXTPMarkupInputElement::OnMouseLeftButtonUp(CXTPMarkupMouseButtonEventArgs* /*e*/)
{
}

void CXTPMarkupInputElement::OnMouseRightButtonUp(CXTPMarkupMouseButtonEventArgs* /*e*/)
{
}

void CXTPMarkupInputElement::OnMouseLeftButtonDown(CXTPMarkupMouseButtonEventArgs* /*e*/)
{
}

void CXTPMarkupInputElement::OnMouseRightButtonDown(CXTPMarkupMouseButtonEventArgs* /*e*/)
{
}

void CXTPMarkupInputElement::OnLostMouseCapture(CXTPMarkupMouseEventArgs* /*e*/)
{

}

void CXTPMarkupInputElement::SetToolTip(LPCTSTR lpszTooltip)
{
	SetValue(m_pToolTipProperty, new CXTPMarkupString(lpszTooltip));
}

void CXTPMarkupInputElement::RaiseEvent(CXTPMarkupRoutedEventArgs* pEventArgs)
{
	if (pEventArgs->GetEvent() == NULL)
	{
		ASSERT(FALSE);
		return;
	}

	if (pEventArgs->GetEvent()->GetFlags() & CXTPMarkupRoutedEvent::routingBubble)
	{
		CXTPMarkupObject* pParent = this;
		while (pParent)
		{
			if (pParent->IsKindOf(MARKUP_TYPE(CXTPMarkupInputElement)))
			{
				CXTPMarkupEventHandlerMap* pHandlers = ((CXTPMarkupInputElement*)pParent)->m_pHandlers;
				if (pHandlers) pHandlers->Raise(pParent, pEventArgs);

				if (pEventArgs->IsHandled())
					return;
			}

			pParent = pParent->GetLogicalParent();
		}

		CXTPMarkupContext* pMarkupContext = GetMarkupContext();
		if (pMarkupContext)
		{
			pMarkupContext->RaiseEvent(this, pEventArgs);
		}
	}
	else
	{
		if (m_pHandlers) m_pHandlers->Raise(this, pEventArgs);
	}
}

void CXTPMarkupInputElement::AddHandler(CXTPMarkupRoutedEvent* pEvent, CXTPMarkupDelegate* pDelegate)
{
	if (m_pHandlers == NULL)
		m_pHandlers = new CXTPMarkupEventHandlerMap();

	m_pHandlers->Add(pEvent, pDelegate);
}


CXTPMarkupInputElement* CXTPMarkupInputElement::InputHitTest(CPoint /*point*/) const
{
	return NULL;
}

void CXTPMarkupInputElement::SetPropertyObject(CXTPMarkupBuilder* pBuilder, CXTPMarkupDependencyProperty* pProperty, CXTPMarkupObject* pValue)
{
	if (pProperty->IsEvent())
	{
		CXTPMarkupContext* pMarkupContext = GetMarkupContext();
		if (!pMarkupContext)
		{
			pBuilder->ThrowBuilderException(_T("Markup Context is NULL"));
		}

		LPCWSTR lpszDelegate = (LPCWSTR)*((CXTPMarkupString*)pValue);

		CXTPMarkupDelegate* pDelegate = pMarkupContext->LookupDelegate(lpszDelegate);
		if (!pDelegate)
		{
			pBuilder->ThrowBuilderException(CXTPMarkupBuilder::FormatString(_T("'%ls' delegate not found"), (LPCTSTR)lpszDelegate));
		}

		AddHandler((CXTPMarkupRoutedEvent*)pProperty,  pDelegate);
		MARKUP_RELEASE(pValue);
		MARKUP_ADDREF(pDelegate);

		return;
	}

	CXTPMarkupObject::SetPropertyObject(pBuilder, pProperty, pValue);

}
void CXTPMarkupInputElement::OnFinalRelease()
{
	if (NULL != m_pMarkupContext)
	{
		m_pMarkupContext->FinalizeMarkupObject(this);
	}

	CXTPMarkupObject::OnFinalRelease();
}

void CXTPMarkupInputElement::CaptureMouse()
{
	m_pMarkupContext->CaptureMouse(this);
}

void CXTPMarkupInputElement::ReleaseMouseCapture()
{
	m_pMarkupContext->ReleaseMouseCapture(this);
}

void CXTPMarkupInputElement::ApplyTree(CXTPMarkupObject* pChild)
{
	int nCount = pChild->GetLogicalChildrenCount();

	for (int i = 0; i < nCount; i++)
	{
		CXTPMarkupObject* p = pChild->GetLogicalChild(i);

		ApplyTree(p);
	}

	if (pChild->IsKindOf(MARKUP_TYPE(CXTPMarkupInputElement)))
	{
		((CXTPMarkupInputElement*)pChild)->OnApply();
	}
}

void CXTPMarkupInputElement::Apply()
{
	ApplyTree(this);
}



CXTPMarkupObject* CXTPMarkupInputElement::FindResource(const CXTPMarkupObject* pKey) const
{
	return CXTPMarkupResourceDictionary::FindResource(this, pKey);
}

void CXTPMarkupInputElement::OnApply()
{
	if (m_pStyleCache == NULL)
	{
		CXTPMarkupType* pType = GetType();
		if (!pType)
			return;

		CXTPMarkupStyle* pStyle = MARKUP_DYNAMICCAST(CXTPMarkupStyle, FindResource(pType));
		if (pStyle)
		{
			pStyle->Seal();
			m_pStyleCache = pStyle;
		}
	}

	if (m_pStyleCache != NULL)
	{
		FireTriggers(NULL, NULL);
	}
}


class CXTPMarkupActiveTriggerCollection : public CXTPMarkupTriggerCollection
{
public:
	CXTPMarkupActiveTriggerCollection()
	{
		m_bLogicalParent = FALSE;
	}

};

BOOL CXTPMarkupInputElement::AddStyleTriggers(CXTPMarkupStyle* pStyle, CXTPMarkupDependencyProperty* pProperty, CXTPMarkupObject* pNewValue)
{
	BOOL bChanged = FALSE;

	CXTPMarkupStyle* pBasedStyle = pStyle->GetBasedStyle();
	if (pBasedStyle)
	{
		bChanged = AddStyleTriggers(pBasedStyle, pProperty, pNewValue);
	}

	CXTPMarkupTriggerCollection* pTriggers = pStyle->GetTriggers();
	if (!pTriggers)
		return bChanged;

	BOOL bAllProperties = (pProperty == NULL);

	for (int i = pTriggers->GetCount() - 1; i >= 0; i--)
	{
		CXTPMarkupTrigger* pTrigger = pTriggers->GetItem(i);

		if (bAllProperties)
		{
			pProperty = pTrigger->GetTriggerProperty();
			pNewValue = GetValue(pProperty);
		}

		if (pTrigger->GetTriggerProperty() == pProperty && pTrigger->GetTriggerValue()->IsEqual(pNewValue))
		{
			if (!m_pActiveTriggers)
			{
				m_pActiveTriggers = new CXTPMarkupActiveTriggerCollection();
			}

			pTrigger->AddRef();
			m_pActiveTriggers->Add(pTrigger);
			bChanged = TRUE;
		}
	}

	return bChanged;
}

void CXTPMarkupInputElement::FireTriggers(CXTPMarkupDependencyProperty* pProperty, CXTPMarkupObject* pNewValue)
{
	BOOL bChanged = FALSE;
	int i;

	CMap<CXTPMarkupDependencyProperty*, CXTPMarkupDependencyProperty*, CXTPMarkupObject*, CXTPMarkupObject*> mapOldValues;

	if (m_pActiveTriggers)
	{
		for (i = m_pActiveTriggers->GetCount() - 1; i >= 0; i--)
		{
			CXTPMarkupTrigger* pTrigger = m_pActiveTriggers->GetItem(i);

			if (pTrigger->GetTriggerProperty() == pProperty)
			{
				if (!pTrigger->GetTriggerValue()->IsEqual(pNewValue))
				{
					CXTPMarkupSetterColection* pSetters = pTrigger->GetSetters();
					if(NULL == pSetters)
					{
						continue;
					}

					for (int j = 0; j < pSetters->GetCount(); j++)
					{
						CXTPMarkupSetter* pSetter = pSetters->GetItem(j);
						CXTPMarkupDependencyProperty* pSetterProperty = pSetter->GetSetterProperty();

						CXTPMarkupObject* pOldValue = m_pTriggerProperties->Lookup(pSetterProperty);

						CXTPMarkupObject* pTempValue;
						if (!mapOldValues.Lookup(pSetterProperty, pTempValue))
						{
							MARKUP_ADDREF(pOldValue);
							mapOldValues.SetAt(pSetterProperty, pOldValue);
						}

						m_pTriggerProperties->Set(pSetterProperty, NULL);
					}

					m_pActiveTriggers->Remove(i);
					bChanged = TRUE;
				}
			}
		}
	}

	if (m_pStyleCache)
	{
		bChanged = AddStyleTriggers(m_pStyleCache, pProperty, pNewValue) || bChanged;
	}
	else if (GetType()->GetTypeStyle())
	{
		bChanged = AddStyleTriggers(GetType()->GetTypeStyle(), pProperty, pNewValue) || bChanged;
	}


	if (bChanged)
	{
		if (!m_pTriggerProperties)
			m_pTriggerProperties = new CXTPMarkupProperties(NULL);

		for (i = 0; i < m_pActiveTriggers->GetCount(); i++)
		{
			CXTPMarkupTrigger* pTrigger = m_pActiveTriggers->GetItem(i);

			CXTPMarkupSetterColection* pSetters = pTrigger->GetSetters();
			if(NULL == pSetters)
			{
				continue;
			}

			for (int j = 0; j < pSetters->GetCount(); j++)
			{
				CXTPMarkupSetter* pSetter = pSetters->GetItem(j);

				CXTPMarkupObject* pValue = pSetter->GetSetterValue();

				CXTPMarkupDependencyProperty* pSetterProperty = pSetter->GetSetterProperty();

				CXTPMarkupObject* pOldValue = m_pTriggerProperties->Lookup(pSetterProperty);

				if (!pValue->IsEqual(pOldValue))
				{
					CXTPMarkupObject* pTempValue;
					if (!mapOldValues.Lookup(pSetterProperty, pTempValue))
					{
						MARKUP_ADDREF(pOldValue);
						mapOldValues.SetAt(pSetterProperty, pOldValue);
					}

					pValue->AddRef();
					m_pTriggerProperties->Set(pSetterProperty, pValue);
				}
			}
		}
	}

	POSITION pos = mapOldValues.GetStartPosition();
	while (pos)
	{
		CXTPMarkupDependencyProperty* pProperty;
		CXTPMarkupObject* pOldValue;
		mapOldValues.GetNextAssoc(pos, pProperty, pOldValue);

		CXTPMarkupObject* pNewValue = m_pTriggerProperties->Lookup(pProperty);

		if (!::IsEqual(pNewValue, pOldValue))
		{
			OnPropertyChanged(pProperty, pOldValue, pNewValue);

			if (pProperty->GetFlags() & CXTPMarkupPropertyMetadata::flagInherited)
			{
				RecursePropertyChanged(pProperty, pOldValue, pNewValue);
			}
		}

		MARKUP_RELEASE(pOldValue);
	}
}

void CXTPMarkupInputElement::OnCursorPropertyChanged(CXTPMarkupObject* d, CXTPMarkupPropertyChangedEventArgs* /*e*/)
{
	CXTPMarkupInputElement* pElement = MARKUP_DYNAMICCAST(CXTPMarkupInputElement, d);
	if (!pElement)
	{
		ASSERT(FALSE);
		return;
	}

	pElement->GetMarkupContext()->HandleSetCursor();

}

void CXTPMarkupInputElement::OnStylePropertyChanged(CXTPMarkupObject* d, CXTPMarkupPropertyChangedEventArgs* e)
{
	CXTPMarkupInputElement* pElement = MARKUP_DYNAMICCAST(CXTPMarkupInputElement, d);
	if (!pElement)
	{
		ASSERT(FALSE);
		return;
	}

	CXTPMarkupStyle* pStyle = MARKUP_STATICCAST(CXTPMarkupStyle, e->m_pNewValue);

	if (pStyle)
	{
		pStyle->Seal();
	}

	pElement->m_pStyleCache = pStyle;
}

CXTPMarkupObject* CXTPMarkupInputElement::GetValueCore(CXTPMarkupDependencyProperty* pProperty) const
{
	CXTPMarkupObject* pValue = CXTPMarkupObject::GetValueCore(pProperty);
	if (pValue)
		return pValue;

	if (m_pTriggerProperties)
	{
		pValue = m_pTriggerProperties->Lookup(pProperty);
		if (pValue)
			return pValue;
	}

	if (m_pStyleCache)
	{
		pValue = m_pStyleCache->GetStyleValue(pProperty);
		if (pValue)
			return pValue;
	}

	CXTPMarkupStyle* pTypeStyle = GetType()->GetTypeStyle();
	if (pTypeStyle)
		return pTypeStyle->GetStyleValue(pProperty);

	return NULL;
}

void CXTPMarkupInputElement::Focus()
{
	if (!m_pMarkupContext)
		return;

	m_pMarkupContext->GetKeyboardNavigation()->Focus(this);
}

CXTPMarkupString* CXTPMarkupInputElement::GetName() const
{
	return MARKUP_STATICCAST(CXTPMarkupString, GetValue(m_pNameProperty));
}

void CXTPMarkupInputElement::SetName(LPCTSTR lpszName)
{
	SetValue(m_pNameProperty, new CXTPMarkupString(lpszName));
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPMarkupInputElement, CXTPMarkupObject)
	DISP_PROPERTY_EX_ID(CXTPMarkupInputElement, "Cursor", 100, OleGetCursor, OleSetCursor, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPMarkupInputElement, "ToolTip", 101, OleGetToolTip, OleSetToolTip, VT_VARIANT)
	DISP_PROPERTY_EX_ID(CXTPMarkupInputElement, "IsMouseOver", 102, OleGetIsMouseOver, SetNotSupported, VT_BOOL)
	DISP_FUNCTION_ID(CXTPMarkupInputElement, "AddHandler", 103, OleAddHandler, VT_EMPTY, VTS_DISPATCH VTS_WBSTR)

	DISP_PROPERTY_EX_ID(CXTPMarkupInputElement, "MouseLeaveEvent", 104, OleGetMouseLeaveEvent, SetNotSupported, VT_DISPATCH)
	DISP_PROPERTY_EX_ID(CXTPMarkupInputElement, "MouseEnterEvent", 105, OleGetMouseEnterEvent, SetNotSupported, VT_DISPATCH)
	DISP_PROPERTY_EX_ID(CXTPMarkupInputElement, "MouseLeftButtonUpEvent", 106, OleGetMouseLeftButtonUpEvent, SetNotSupported, VT_DISPATCH)
	DISP_PROPERTY_EX_ID(CXTPMarkupInputElement, "MouseLeftButtonDownEvent", 107, OleGetMouseLeftButtonDownEvent, SetNotSupported, VT_DISPATCH)
	DISP_PROPERTY_EX_ID(CXTPMarkupInputElement, "MouseRightButtonUpEvent", 108, OleGetMouseRightButtonUpEvent, SetNotSupported, VT_DISPATCH)
	DISP_PROPERTY_EX_ID(CXTPMarkupInputElement, "MouseRightButtonDownEvent", 109, OleGetMouseRightButtonDownEvent, SetNotSupported, VT_DISPATCH)
	DISP_PROPERTY_EX_ID(CXTPMarkupInputElement, "MouseMoveEvent", 110, OleGetMouseMoveEvent, SetNotSupported, VT_DISPATCH)
	DISP_PROPERTY_EX_ID(CXTPMarkupInputElement, "LostMouseCaptureEvent", 111, OleGetLostMouseCaptureEvent, SetNotSupported, VT_DISPATCH)

	DISP_FUNCTION_ID(CXTPMarkupInputElement, "FindName", 112, OleFindName, VT_DISPATCH, VTS_WBSTR)
	DISP_PROPERTY_EX_ID(CXTPMarkupInputElement, "Name", 113, OleGetName, OleSetName, VT_BSTR)

	DISP_FUNCTION_ID(CXTPMarkupInputElement, "CaptureMouse", 114, CaptureMouse, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CXTPMarkupInputElement, "ReleaseMouseCapture", 115, ReleaseMouseCapture, VT_EMPTY, VTS_NONE)

END_DISPATCH_MAP()

// {10189842-AE6D-46a8-BC77-CEE7D5CE9ED7}
static const GUID IID_IMarkupInputElement =
{ 0x10189842, 0xae6d, 0x46a8, { 0xbc, 0x77, 0xce, 0xe7, 0xd5, 0xce, 0x9e, 0xd7 } };


BEGIN_INTERFACE_MAP(CXTPMarkupInputElement, CXTPMarkupObject)
	INTERFACE_PART(CXTPMarkupInputElement, IID_IMarkupInputElement, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPMarkupInputElement, IID_IMarkupInputElement)

LPDISPATCH CXTPMarkupInputElement::OleFindName(LPCOLESTR lpszName)
{
	return XTPGetDispatch(FindName(lpszName));
}

BSTR CXTPMarkupInputElement::OleGetName()
{
	CXTPMarkupString* pName = GetName();
	if (pName)
		return SysAllocString(*pName);

	return NULL;
}

void CXTPMarkupInputElement::OleSetName(LPCTSTR lpszName)
{
	SetName(lpszName);
}

long CXTPMarkupInputElement::OleGetCursor()
{
	CXTPMarkupInt* pCursor = MARKUP_STATICCAST(CXTPMarkupInt, GetValue(m_pCursorProperty));
	return pCursor ? (int)*pCursor : 0;
}

void CXTPMarkupInputElement::OleSetCursor(long nValue)
{
	SetValue(m_pCursorProperty, new CXTPMarkupInt(nValue));
}

VARIANT CXTPMarkupInputElement::OleGetToolTip()
{
	VARIANT va;
	VariantInit(&va);
	va.vt = VT_NULL;

	CXTPMarkupObject* pTag = GetToolTip();
	if (!pTag)
		return va;

	if (IsStringObject(pTag))
	{
		va.vt = VT_BSTR;
		va.bstrVal = SysAllocString(*(CXTPMarkupString*)pTag);
	}
	else
	{
		va.vt = VT_DISPATCH;
		va.pdispVal = XTPGetDispatch(pTag);
	}
	return va;
}

void CXTPMarkupInputElement::OleSetToolTip(const VARIANT& va)
{
	if (va.vt == VT_BSTR)
	{
		SetValue(m_pToolTipProperty, new CXTPMarkupString(va.bstrVal));
	}
}

BOOL CXTPMarkupInputElement::OleGetIsMouseOver()
{
	return IsMouseOver();
}

LPDISPATCH CXTPMarkupInputElement::OleGetMouseLeaveEvent()
{
	return XTPGetDispatch(m_pMouseLeaveEvent);
}

LPDISPATCH CXTPMarkupInputElement::OleGetMouseEnterEvent()
{
	return XTPGetDispatch(m_pMouseEnterEvent);
}

LPDISPATCH CXTPMarkupInputElement::OleGetMouseLeftButtonUpEvent()
{
	return XTPGetDispatch(m_pMouseLeftButtonUpEvent);
}

LPDISPATCH CXTPMarkupInputElement::OleGetMouseLeftButtonDownEvent()
{
	return XTPGetDispatch(m_pMouseLeftButtonDownEvent);
}

LPDISPATCH CXTPMarkupInputElement::OleGetMouseRightButtonUpEvent()
{
	return XTPGetDispatch(m_pMouseRightButtonUpEvent);
}

LPDISPATCH CXTPMarkupInputElement::OleGetMouseRightButtonDownEvent()
{
	return XTPGetDispatch(m_pMouseRightButtonDownEvent);
}

LPDISPATCH CXTPMarkupInputElement::OleGetMouseMoveEvent()
{
	return XTPGetDispatch(m_pMouseMoveEvent);
}

LPDISPATCH CXTPMarkupInputElement::OleGetLostMouseCaptureEvent()
{
	return XTPGetDispatch(m_pLostMouseCaptureEvent);
}

//////////////////////////////////////////////////////////////////////////
// CXTPMarkupRoutedEvent


BEGIN_DISPATCH_MAP(CXTPMarkupRoutedEvent, CXTPMarkupDependencyProperty)
	DISP_PROPERTY_EX_ID(CXTPMarkupRoutedEvent, "Name", 100, OleGetName, SetNotSupported, VT_BSTR)
END_DISPATCH_MAP()

// {10419842-AE6D-46a8-BC77-CEE7D5CE9ED7}
static const GUID IID_IMarkupRoutedEvent =
{ 0x10419842, 0xae6d, 0x46a8, { 0xbc, 0x77, 0xce, 0xe7, 0xd5, 0xce, 0x9e, 0xd7 } };


BEGIN_INTERFACE_MAP(CXTPMarkupRoutedEvent, CXTPMarkupDependencyProperty)
	INTERFACE_PART(CXTPMarkupRoutedEvent, IID_IMarkupRoutedEvent, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPMarkupRoutedEvent, IID_IMarkupRoutedEvent)

BSTR CXTPMarkupRoutedEvent::OleGetName()
{
	return SysAllocString(GetName());
}

#endif
