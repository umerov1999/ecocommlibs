// XTPMarkupPath.cpp: implementation of the CXTPMarkupPath class.
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
#include <math.h>

#include <Common/XTPVC80Helpers.h>

#include <Markup/XTPMarkupObject.h>
#include <Markup/XTPMarkupInputElement.h>
#include <Markup/XTPMarkupUIElement.h>
#include <Markup/XTPMarkupFrameworkElement.h>
#include <Markup/XTPMarkupShape.h>
#include <Markup/XTPMarkupDrawingContext.h>
#include <Markup/XTPMarkupBuilder.h>
#include <Markup/XTPMarkupThickness.h>

#include "XTPMarkupPath.h"
#include "XTPMarkupPathData.h"
#include "XTPMarkupPathGeometry.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////////
// CXTPMarkupPath


CXTPMarkupDependencyProperty* CXTPMarkupPath::m_pPropertyData = NULL;

IMPLEMENT_MARKUPCLASS(L"Path", CXTPMarkupPath, CXTPMarkupShape)

void CXTPMarkupPath::RegisterMarkupClass()
{
	m_pPropertyData = CXTPMarkupDependencyProperty::Register(L"Data", MARKUP_TYPE(CXTPMarkupPathData), MARKUP_TYPE(CXTPMarkupPath),
		new CXTPMarkupPropertyMetadata(NULL, CXTPMarkupPropertyMetadata::flagAffectsMeasure));
}

CXTPMarkupPath::CXTPMarkupPath()
{
}

CXTPMarkupPath::~CXTPMarkupPath()
{

}

void CXTPMarkupPath::SetContentObject(CXTPMarkupBuilder *pBuilder, CXTPMarkupObject *pContent)
{
	UNREFERENCED_PARAMETER(pBuilder);

	if (pContent->IsKindOf(MARKUP_TYPE(CXTPMarkupPathData)))
	{
		SetValue(m_pPropertyData, pContent);
	}
	else
	{
		CXTPMarkupObject::SetContentObject(pBuilder, pContent);
	}
}

CXTPMarkupPathData* CXTPMarkupPath::GetData() const
{
	return MARKUP_STATICCAST(CXTPMarkupPathData, GetValue(m_pPropertyData));
}

CSize CXTPMarkupPath::MeasureOverride(CXTPMarkupDrawingContext *pDC, CSize szAvailableSize)
{
	UNREFERENCED_PARAMETER(pDC);

	CXTPMarkupPathData *pData = GetData();

	if (NULL == pData)
	{
		return CSize(0, 0);
	}

	CRect rcBounds = pData->GetBounds();

	CSize szBound(max(rcBounds.Width(), rcBounds.right), max(rcBounds.Height(),rcBounds.bottom));

	if (GetStretch() != xtpMarkupStretchNone)
	{
		if (szAvailableSize.cx < INT_MAX)
			szBound.cx = szAvailableSize.cx;

		if (szAvailableSize.cy < INT_MAX)
			szBound.cy = szAvailableSize.cy;
	}
	return szBound;
}

void CXTPMarkupPath::OnRender(CXTPMarkupDrawingContext *pDC)
{
	CXTPMarkupStrokeStyle strokeStyle;
	GetStrokeStyle(&strokeStyle);

	CXTPMarkupBrush *pFillBrush = GetFill();

	if (!strokeStyle.pStrokeBrush && !pFillBrush)
		return;

	CXTPMarkupPathData* pData = GetData();
	if (!pData)
		return;

	if (GetStretch() != xtpMarkupStretchNone)
	{
		CXTPMarkupPathData* pGeometryStretch = pData->Stretch(GetRenderSize());
		pDC->DrawPath(pGeometryStretch, &strokeStyle, pFillBrush);
		delete pGeometryStretch;
	}
	else
	{
		pDC->DrawPath(pData, &strokeStyle, pFillBrush);
	}
}


#ifdef _XTP_ACTIVEX

/////////////////////////////////////////////////////////////////////////
// Path


BEGIN_DISPATCH_MAP(CXTPMarkupPath, CXTPMarkupShape)
END_DISPATCH_MAP()

// {10259842-AE6D-46a8-BC77-CEE7D5CE9ED7}
static const GUID IID_IMarkupPath =
{ 0x10259842, 0xae6d, 0x46a8, { 0xbc, 0x77, 0xce, 0xe7, 0xd5, 0xce, 0x9e, 0xd7 } };


BEGIN_INTERFACE_MAP(CXTPMarkupPath, CXTPMarkupFrameworkElement)
	INTERFACE_PART(CXTPMarkupPath, IID_IMarkupPath, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPMarkupPath, IID_IMarkupPath)

#endif
