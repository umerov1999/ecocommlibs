// XTPMarkupDefinitionCollection.cpp: implementation of the CXTPMarkupDefinitionCollection class.
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

#include "StdAfx.h"

#include "../XTPMarkupObject.h"
#include "../XTPMarkupInputElement.h"
#include "../XTPMarkupUIElement.h"
#include "../XTPMarkupFrameworkElement.h"
#include "../XTPMarkupPanel.h"
#include "../XTPMarkupGrid.h"
#include "../XTPMarkupBuilder.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////////
// CXTPMarkupDefinitionCollection

IMPLEMENT_MARKUPCLASS(NULL, CXTPMarkupDefinitionCollection, CXTPMarkupCollection);

void CXTPMarkupDefinitionCollection::RegisterMarkupClass()
{

}

CXTPMarkupDefinitionCollection::CXTPMarkupDefinitionCollection()
{
}

IMPLEMENT_MARKUPCLASS(NULL, CXTPMarkupRowDefinitionCollection, CXTPMarkupDefinitionCollection);

void CXTPMarkupRowDefinitionCollection::RegisterMarkupClass()
{

}

CXTPMarkupRowDefinitionCollection::CXTPMarkupRowDefinitionCollection()
{
	m_pElementType = MARKUP_TYPE(CXTPMarkupRowDefinition);
}

IMPLEMENT_MARKUPCLASS(NULL, CXTPMarkupColumnDefinitionCollection, CXTPMarkupDefinitionCollection);

void CXTPMarkupColumnDefinitionCollection::RegisterMarkupClass()
{

}

CXTPMarkupColumnDefinitionCollection::CXTPMarkupColumnDefinitionCollection()
{
	m_pElementType = MARKUP_TYPE(CXTPMarkupColumnDefinition);
}


#ifdef _XTP_ACTIVEX

//////////////////////////////////////////////////////////////////////////
// CXTPMarkupDefinitionCollection

BEGIN_DISPATCH_MAP(CXTPMarkupDefinitionCollection, CXTPMarkupObject)
	DISP_FUNCTION_ID(CXTPMarkupDefinitionCollection, "Count", 1, OleGetItemCount, VT_I4, VTS_NONE)
	DISP_FUNCTION_ID(CXTPMarkupDefinitionCollection, "Item", DISPID_VALUE, OleGetItem, VT_DISPATCH, VTS_I4)
	DISP_FUNCTION_ID(CXTPMarkupDefinitionCollection, "_NewEnum", DISPID_NEWENUM, OleNewEnum, VT_UNKNOWN, VTS_NONE)
	DISP_FUNCTION_ID(CXTPMarkupDefinitionCollection, "Add", 2, OleAdd, VT_EMPTY, VTS_DISPATCH)
	DISP_FUNCTION_ID(CXTPMarkupDefinitionCollection, "Insert", 3, OleInsert, VT_EMPTY, VTS_I4 VTS_DISPATCH)
	DISP_FUNCTION_ID(CXTPMarkupDefinitionCollection, "Clear", 4, RemoveAll, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CXTPMarkupDefinitionCollection, "RemoveAt", 5, Remove, VT_EMPTY, VTS_I4)
END_DISPATCH_MAP()

// {10379842-AE6D-46a8-BC77-CEE7D5CE9ED7}
static const GUID IID_IMarkupDefinitionCollection =
{ 0x10379842, 0xae6d, 0x46a8, { 0xbc, 0x77, 0xce, 0xe7, 0xd5, 0xce, 0x9e, 0xd7 } };


BEGIN_INTERFACE_MAP(CXTPMarkupDefinitionCollection, CXTPMarkupObject)
	INTERFACE_PART(CXTPMarkupDefinitionCollection, IID_IMarkupDefinitionCollection, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPMarkupDefinitionCollection, IID_IMarkupDefinitionCollection)

IMPLEMENT_ENUM_VARIANT(CXTPMarkupDefinitionCollection)

long CXTPMarkupDefinitionCollection::OleGetItemCount()
{
	return (long)GetCount();
}

LPDISPATCH CXTPMarkupDefinitionCollection::OleGetItem(long nIndex)
{
	return XTPGetDispatch(GetItem(nIndex));
}

void CXTPMarkupDefinitionCollection::OleAdd(LPDISPATCH lpElementDisp)
{
	if (!lpElementDisp)
		AfxThrowOleException(E_INVALIDARG);

	Add(MARKUP_STATICCAST(CXTPMarkupDefinitionBase, FromDispatch(lpElementDisp)));
}

void CXTPMarkupDefinitionCollection::OleInsert(long Index, LPDISPATCH lpElementDisp)
{
	if (!lpElementDisp)
		AfxThrowOleException(E_INVALIDARG);

	Insert(Index, MARKUP_STATICCAST(CXTPMarkupDefinitionBase, FromDispatch(lpElementDisp)));
}

#endif
