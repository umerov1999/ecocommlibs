// XTPMarkupPanel.cpp: implementation of the CXTPMarkupPanel class.
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
#include "XTPMarkupUIElement.h"
#include "XTPMarkupFrameworkElement.h"
#include "XTPMarkupPanel.h"
#include "XTPMarkupDrawingContext.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////////
// CXTPMarkupUIElementCollection

CXTPMarkupUIElementCollection::CXTPMarkupUIElementCollection()
{
	m_pElementType = MARKUP_TYPE(CXTPMarkupUIElement);
}

CXTPMarkupUIElementCollection::~CXTPMarkupUIElementCollection()
{
}

void CXTPMarkupUIElementCollection::OnChanged()
{
	CXTPMarkupUIElement* pPanel = MARKUP_DYNAMICCAST(CXTPMarkupUIElement, GetLogicalParent());
	if (pPanel)
	{
		pPanel->InvalidateMeasure();
	}
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CXTPMarkupDependencyProperty* CXTPMarkupPanel::m_pBackgroundProperty = NULL;

IMPLEMENT_MARKUPCLASS(L"Panel", CXTPMarkupPanel, CXTPMarkupFrameworkElement)

void CXTPMarkupPanel::RegisterMarkupClass()
{
	m_pBackgroundProperty = CXTPMarkupDependencyProperty::Register(L"Background", MARKUP_TYPE(CXTPMarkupBrush), MARKUP_TYPE(CXTPMarkupPanel),
		new CXTPMarkupPropertyMetadata(NULL, CXTPMarkupPropertyMetadata::flagAffectsRender));
}

CXTPMarkupPanel::CXTPMarkupPanel()
{
	m_pChildren = new CXTPMarkupUIElementCollection();
	m_pChildren->SetLogicalParent(this);
}

CXTPMarkupPanel::~CXTPMarkupPanel()
{
	if (m_pChildren)
	{
		m_pChildren->SetLogicalParent(NULL);
		m_pChildren->Release();
	}
}

void CXTPMarkupPanel::SetContentObject(CXTPMarkupBuilder* pBuilder, CXTPMarkupObject* pContent)
{
	m_pChildren->SetContentObject(pBuilder, pContent);
}

BOOL CXTPMarkupPanel::HasContentObject() const
{
	return m_pChildren->HasContentObject();
}

CXTPMarkupBrush* CXTPMarkupPanel::GetBackground() const
{
	return MARKUP_STATICCAST(CXTPMarkupBrush, GetValue(m_pBackgroundProperty));
}

void CXTPMarkupPanel::SetBackground(CXTPMarkupBrush* brush)
{
	SetValue(m_pBackgroundProperty, brush);
}

void CXTPMarkupPanel::OnRender(CXTPMarkupDrawingContext* pDC)
{
	CSize szRender = GetRenderSize();
	if (szRender.cx <= 0 || szRender.cy <= 0)
		return;

	CXTPMarkupBrush* pBackground = GetBackground();

	if (pBackground)
	{
		pDC->FillRectangle(CRect(0, 0, szRender.cx, szRender.cy), pBackground);
	}
}

CXTPMarkupInputElement* CXTPMarkupPanel::InputHitTestOverride(CPoint /*point*/) const
{
	return GetBackground() != NULL ? (CXTPMarkupInputElement*)this : NULL;
}



#ifdef _XTP_ACTIVEX


BEGIN_DISPATCH_MAP(CXTPMarkupUIElementCollection, CXTPMarkupObject)
	DISP_FUNCTION_ID(CXTPMarkupUIElementCollection, "Count", 1, OleGetItemCount, VT_I4, VTS_NONE)
	DISP_FUNCTION_ID(CXTPMarkupUIElementCollection, "Item", DISPID_VALUE, OleGetItem, VT_DISPATCH, VTS_I4)
	DISP_FUNCTION_ID(CXTPMarkupUIElementCollection, "_NewEnum", DISPID_NEWENUM, OleNewEnum, VT_UNKNOWN, VTS_NONE)
	DISP_FUNCTION_ID(CXTPMarkupUIElementCollection, "Add", 2, OleAdd, VT_EMPTY, VTS_DISPATCH)
	DISP_FUNCTION_ID(CXTPMarkupUIElementCollection, "Insert", 3, OleInsert, VT_EMPTY, VTS_I4 VTS_DISPATCH)
	DISP_FUNCTION_ID(CXTPMarkupUIElementCollection, "Clear", 4, RemoveAll, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CXTPMarkupUIElementCollection, "RemoveAt", 5, Remove, VT_EMPTY, VTS_I4)
END_DISPATCH_MAP()

// {10149842-AE6D-46a8-BC77-CEE7D5CE9ED7}
static const GUID IID_IMarkupUIElementCollection =
{ 0x10149842, 0xae6d, 0x46a8, { 0xbc, 0x77, 0xce, 0xe7, 0xd5, 0xce, 0x9e, 0xd7 } };


BEGIN_INTERFACE_MAP(CXTPMarkupUIElementCollection, CXTPMarkupObject)
	INTERFACE_PART(CXTPMarkupUIElementCollection, IID_IMarkupUIElementCollection, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPMarkupUIElementCollection, IID_IMarkupUIElementCollection)

IMPLEMENT_ENUM_VARIANT(CXTPMarkupUIElementCollection)

long CXTPMarkupUIElementCollection::OleGetItemCount()
{
	return (long)GetCount();
}

LPDISPATCH CXTPMarkupUIElementCollection::OleGetItem(long nIndex)
{
	return XTPGetDispatch(GetItem(nIndex));
}

void CXTPMarkupUIElementCollection::OleAdd(LPDISPATCH lpElementDisp)
{
	if (!lpElementDisp)
		AfxThrowOleException(E_INVALIDARG);

	Add(MARKUP_STATICCAST(CXTPMarkupUIElement, FromDispatch(lpElementDisp)));
}

void CXTPMarkupUIElementCollection::OleInsert(long Index, LPDISPATCH lpElementDisp)
{
	if (!lpElementDisp)
		AfxThrowOleException(E_INVALIDARG);

	Insert(Index, MARKUP_STATICCAST(CXTPMarkupUIElement, FromDispatch(lpElementDisp)));
}





BEGIN_DISPATCH_MAP(CXTPMarkupPanel, CXTPMarkupFrameworkElement)
	DISP_PROPERTY_EX_ID(CXTPMarkupPanel, "Background", 500, OleGetBackground, OleSetBackground, VT_DISPATCH)
	DISP_PROPERTY_EX_ID(CXTPMarkupPanel, "Children", 501, OleGetChildren, SetNotSupported, VT_DISPATCH)
END_DISPATCH_MAP()

// {10079842-AE6D-46a8-BC77-CEE7D5CE9ED7}
static const GUID IID_IMarkupPanel =
{ 0x10079842, 0xae6d, 0x46a8, { 0xbc, 0x77, 0xce, 0xe7, 0xd5, 0xce, 0x9e, 0xd7 } };


BEGIN_INTERFACE_MAP(CXTPMarkupPanel, CXTPMarkupFrameworkElement)
	INTERFACE_PART(CXTPMarkupPanel, IID_IMarkupPanel, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPMarkupPanel, IID_IMarkupPanel)

LPDISPATCH CXTPMarkupPanel::OleGetChildren()
{
	return XTPGetDispatch(GetChildren());
}

LPDISPATCH CXTPMarkupPanel::OleGetBackground()
{
	return XTPGetDispatch(GetBackground());
}

void CXTPMarkupPanel::OleSetBackground(LPDISPATCH lpBackground)
{
	SetBackground(MARKUP_STATICCAST(CXTPMarkupBrush, FromDispatch(lpBackground)));
}

#endif
