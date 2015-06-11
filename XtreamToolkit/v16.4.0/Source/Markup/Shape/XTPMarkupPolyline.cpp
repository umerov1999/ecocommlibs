// XTPMarkupPolyline.cpp: implementation of the CXTPMarkupPolyline class.
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

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////////
// CXTPMarkupPolyline


CXTPMarkupDependencyProperty* CXTPMarkupPolyline::m_pPointsProperty = NULL;

IMPLEMENT_MARKUPCLASS(L"Polyline", CXTPMarkupPolyline, CXTPMarkupShape)

void CXTPMarkupPolyline::RegisterMarkupClass()
{
	m_pPointsProperty = CXTPMarkupDependencyProperty::Register(L"Points", MARKUP_TYPE(CXTPMarkupPointCollection), MARKUP_TYPE(CXTPMarkupPolyline),
		new CXTPMarkupPropertyMetadata(NULL, CXTPMarkupPropertyMetadata::flagAffectsMeasure));
}

CXTPMarkupPolyline::CXTPMarkupPolyline()
{
}

CXTPMarkupPolyline::~CXTPMarkupPolyline()
{

}

void CXTPMarkupPolyline::SetPoints(CXTPMarkupPointCollection* pPoints)
{
	SetValue(m_pPointsProperty, pPoints);
}

CXTPMarkupPointCollection* CXTPMarkupPolyline::GetPoints() const
{
	return MARKUP_STATICCAST(CXTPMarkupPointCollection, GetValue(m_pPointsProperty));
}

CSize CXTPMarkupPolyline::MeasureOverride(CXTPMarkupDrawingContext* /*pDC*/, CSize szAvailableSize)
{
	CXTPMarkupPointCollection* pPoints = GetPoints();
	if (!pPoints)
		return CSize(0, 0);
	CRect rcBounds = pPoints->GetBounds();

	CSize szBound(max(rcBounds.Width(), rcBounds.right), max(rcBounds.Height(),rcBounds.bottom));

	if ((szBound.cx == 0 || szBound.cy == 0) && GetStroke())
	{
		int nStroke = (GetStrokeThickness() + 1) / 2;

		if (szBound.cy == 0) szBound.cy += nStroke;
		if (szBound.cx == 0) szBound.cx += nStroke;
	}

	if (GetStretch() != xtpMarkupStretchNone)
	{
		if (szAvailableSize.cx < INT_MAX)
			szBound.cx = szAvailableSize.cx;

		if (szAvailableSize.cy < INT_MAX)
			szBound.cy = szAvailableSize.cy;
	}
	return szBound;
}

void CXTPMarkupPolyline::OnRender(CXTPMarkupDrawingContext* pDC)
{
	CXTPMarkupStrokeStyle strokeStyle;
	GetStrokeStyle(&strokeStyle);

	if (strokeStyle.nStrokeThickness <= 0)
		return;

	CXTPMarkupPointCollection* pPoints = GetPoints();
	if (!pPoints)
		return;

	const CXTPMarkupPointCollection::CPointArray& arr = pPoints->GetPoints();

	if (GetStretch() != xtpMarkupStretchNone)
	{
		CXTPMarkupPointCollection::CPointArray arrStretch;
		pPoints->Stretch(arrStretch, GetRenderSize());

		pDC->Polyline(arrStretch.GetData(), (int)arrStretch.GetSize(), &strokeStyle);
	}
	else
	{
		pDC->Polyline(arr.GetData(), (int)arr.GetSize(), &strokeStyle);
	}
}


#ifdef _XTP_ACTIVEX

/////////////////////////////////////////////////////////////////////////
// Polyline


BEGIN_DISPATCH_MAP(CXTPMarkupPolyline, CXTPMarkupShape)
END_DISPATCH_MAP()

// {10249842-AE6D-46a8-BC77-CEE7D5CE9ED7}
static const GUID IID_IMarkupPolyline =
{ 0x10249842, 0xae6d, 0x46a8, { 0xbc, 0x77, 0xce, 0xe7, 0xd5, 0xce, 0x9e, 0xd7 } };


BEGIN_INTERFACE_MAP(CXTPMarkupPolyline, CXTPMarkupFrameworkElement)
	INTERFACE_PART(CXTPMarkupPolyline, IID_IMarkupPolyline, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPMarkupPolyline, IID_IMarkupPolyline)

#endif // _XTP_ACTIVEX
