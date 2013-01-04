// XTPMarkupDefinitionBase.cpp: implementation of the CXTPMarkupDefinitionBase class.
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
// CXTPMarkupDefinitionBase

IMPLEMENT_MARKUPCLASS(L"DefinitionBase", CXTPMarkupDefinitionBase, CXTPMarkupObject);

void CXTPMarkupDefinitionBase::RegisterMarkupClass()
{
}

CXTPMarkupDefinitionBase::CXTPMarkupDefinitionBase(BOOL bIsColumnDefinition)
{
	m_bIsColumnDefinition = bIsColumnDefinition;
	m_nMinSize = 0;
	m_nSizeType = 0;
	m_nMeasureSize = 0;
}

void CXTPMarkupDefinitionBase::OnBeforeLayout()
{
	m_nSizeType = 0;
	m_nMinSize = 0;
	m_nMeasureSize = 0;
}

void CXTPMarkupDefinitionBase::UpdateMinSize(int nMinSize)
{
	m_nMinSize = max(m_nMinSize, nMinSize);
}

int CXTPMarkupDefinitionBase::GetUserMinSize() const
{
	CXTPMarkupInt* pMinValue = MARKUP_STATICCAST(CXTPMarkupInt, GetValue(m_bIsColumnDefinition ? CXTPMarkupColumnDefinition::m_pMinWidthProperty : CXTPMarkupRowDefinition::m_pMinHeightProperty));
	return pMinValue != NULL ? (int)*pMinValue : 0;
}

double CXTPMarkupDefinitionBase::GetPreferredSize() const
{
	double nMinSize = m_nMinSize;
	if ((m_nSizeType != CXTPMarkupGrid::sizeTypeAuto) && (nMinSize < m_nMeasureSize))
	{
		nMinSize = m_nMeasureSize;
	}
	return nMinSize;
}

int CXTPMarkupDefinitionBase::GetUserMaxSize() const
{
	CXTPMarkupInt* pMaxValue = MARKUP_STATICCAST(CXTPMarkupInt, GetValue(m_bIsColumnDefinition ? CXTPMarkupColumnDefinition::m_pMaxWidthProperty : CXTPMarkupRowDefinition::m_pMaxHeightProperty));
	return pMaxValue != NULL ? (int)*pMaxValue : INT_MAX;
}

CXTPMarkupGridLength* CXTPMarkupDefinitionBase::GetUserSize() const
{
	CXTPMarkupGridLength* pValue = MARKUP_STATICCAST(CXTPMarkupGridLength, GetValue(m_bIsColumnDefinition ? CXTPMarkupColumnDefinition::m_pWidthProperty : CXTPMarkupRowDefinition::m_pHeightProperty));

	ASSERT(pValue);
	return pValue;
}

void CXTPMarkupDefinitionBase::OnDefinitionPropertyChanged(CXTPMarkupObject* d, CXTPMarkupPropertyChangedEventArgs* /*e*/)
{
	CXTPMarkupDefinitionBase* pDefinition = MARKUP_DYNAMICCAST(CXTPMarkupDefinitionBase, d);
	if (!pDefinition)
		return;

	CXTPMarkupGrid* pGrid = MARKUP_DYNAMICCAST(CXTPMarkupGrid, pDefinition->GetLogicalParent() ? pDefinition->GetLogicalParent()->GetLogicalParent() : NULL);
	if (!pGrid)
		return;

	pGrid->InvalidateMeasure();
}


#ifdef _XTP_ACTIVEX

//////////////////////////////////////////////////////////////////////////
// CXTPMarkupDefinitionBase

BEGIN_DISPATCH_MAP(CXTPMarkupDefinitionBase, CXTPMarkupObject)
END_DISPATCH_MAP()

// {10389842-AE6D-46a8-BC77-CEE7D5CE9ED7}
static const GUID IID_IMarkupDefinitionBase =
{ 0x10389842, 0xae6d, 0x46a8, { 0xbc, 0x77, 0xce, 0xe7, 0xd5, 0xce, 0x9e, 0xd7 } };


BEGIN_INTERFACE_MAP(CXTPMarkupDefinitionBase, CXTPMarkupObject)
	INTERFACE_PART(CXTPMarkupDefinitionBase, IID_IMarkupDefinitionBase, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPMarkupDefinitionBase, IID_IMarkupDefinitionBase)

#endif
