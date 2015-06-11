// XTPMarkupRowDefinition.cpp: implementation of the CXTPMarkupRowDefinition class.
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
// CXTPMarkupRowDefinition

CXTPMarkupDependencyProperty* CXTPMarkupRowDefinition::m_pHeightProperty = NULL;
CXTPMarkupDependencyProperty* CXTPMarkupRowDefinition::m_pMinHeightProperty = NULL;
CXTPMarkupDependencyProperty* CXTPMarkupRowDefinition::m_pMaxHeightProperty = NULL;

IMPLEMENT_MARKUPCLASS(L"RowDefinition", CXTPMarkupRowDefinition, CXTPMarkupDefinitionBase);

void CXTPMarkupRowDefinition::RegisterMarkupClass()
{
	m_pHeightProperty = CXTPMarkupDependencyProperty::Register(L"Height", MARKUP_TYPE(CXTPMarkupGridLength), MARKUP_TYPE(CXTPMarkupRowDefinition),
		new CXTPMarkupPropertyMetadata(new CXTPMarkupGridLength(1, CXTPMarkupGridLength::unitTypeStar), &CXTPMarkupDefinitionBase::OnDefinitionPropertyChanged));

	m_pMinHeightProperty = CXTPMarkupDependencyProperty::Register(L"MinHeight", MARKUP_TYPE(CXTPMarkupInt), MARKUP_TYPE(CXTPMarkupRowDefinition),
		new CXTPMarkupPropertyMetadata(NULL, &CXTPMarkupDefinitionBase::OnDefinitionPropertyChanged));

	m_pMaxHeightProperty = CXTPMarkupDependencyProperty::Register(L"MaxHeight", MARKUP_TYPE(CXTPMarkupInt), MARKUP_TYPE(CXTPMarkupRowDefinition),
		new CXTPMarkupPropertyMetadata(NULL, &CXTPMarkupDefinitionBase::OnDefinitionPropertyChanged));
}

CXTPMarkupRowDefinition::CXTPMarkupRowDefinition()
	: CXTPMarkupDefinitionBase(FALSE)
{

}


#ifdef _XTP_ACTIVEX

//////////////////////////////////////////////////////////////////////////
// CXTPMarkupRowDefinition

BEGIN_DISPATCH_MAP(CXTPMarkupRowDefinition, CXTPMarkupDefinitionBase)
	DISP_PROPERTY_EX_ID(CXTPMarkupRowDefinition, "Height", 200, OleGetHeight, OleSetHeight, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPMarkupRowDefinition, "MinHeight", 201, OleGetMinHeight, OleSetMinHeight, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPMarkupRowDefinition, "MaxHeight", 202, OleGetMaxHeight, OleSetMaxHeight, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPMarkupRowDefinition, "UnitType", 203, OleGetUnitType, OleSetUnitType, VT_I4)
END_DISPATCH_MAP()

// {10409842-AE6D-46a8-BC77-CEE7D5CE9ED7}
static const GUID IID_IMarkupRowDefinition =
{ 0x10409842, 0xae6d, 0x46a8, { 0xbc, 0x77, 0xce, 0xe7, 0xd5, 0xce, 0x9e, 0xd7 } };


BEGIN_INTERFACE_MAP(CXTPMarkupRowDefinition, CXTPMarkupDefinitionBase)
	INTERFACE_PART(CXTPMarkupRowDefinition, IID_IMarkupRowDefinition, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPMarkupRowDefinition, IID_IMarkupRowDefinition)

void CXTPMarkupRowDefinition::OleSetMinHeight(int nHeight)
{
	SetMinHeight(nHeight);
}

void CXTPMarkupRowDefinition::OleSetMaxHeight(int nHeight)
{
	SetMinHeight(nHeight);
}

void CXTPMarkupRowDefinition::OleSetHeight(int nHeight)
{
	SetHeight(nHeight);
}

int CXTPMarkupRowDefinition::OleGetMinHeight()
{
	return GetUserMinSize();
}

int CXTPMarkupRowDefinition::OleGetMaxHeight()
{
	return GetUserMaxSize();
}

int CXTPMarkupRowDefinition::OleGetHeight()
{
	CXTPMarkupGridLength* pValue = GetUserSize();
	return (int)pValue->GetValue();
}


int CXTPMarkupRowDefinition::OleGetUnitType()
{
	CXTPMarkupGridLength* pValue = GetUserSize();
	return pValue->GetUnitType();
}
void CXTPMarkupRowDefinition::OleSetUnitType(int nType)
{
	SetHeight(new CXTPMarkupGridLength(OleGetHeight(), (CXTPMarkupGridLength::GridUnitType)nType));
}

#endif
