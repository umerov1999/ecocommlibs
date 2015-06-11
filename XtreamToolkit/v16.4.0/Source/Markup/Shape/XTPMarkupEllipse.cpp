// XTPMarkupEllipse.cpp: implementation of the CXTPMarkupEllipse class.
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
// CXTPMarkupEllipse


IMPLEMENT_MARKUPCLASS(L"Ellipse", CXTPMarkupEllipse, CXTPMarkupShape)

void CXTPMarkupEllipse::RegisterMarkupClass()
{

}

CXTPMarkupEllipse::CXTPMarkupEllipse()
{
}

CXTPMarkupEllipse::~CXTPMarkupEllipse()
{

}

CSize CXTPMarkupEllipse::MeasureOverride(CXTPMarkupDrawingContext* /*pDC*/, CSize szAvailableSize)
{
	CSize szBound(0, 0);

	if (GetStretch() != xtpMarkupStretchNone)
	{
		if (szAvailableSize.cx < INT_MAX)
			szBound.cx = szAvailableSize.cx;

		if (szAvailableSize.cy < INT_MAX)
			szBound.cy = szAvailableSize.cy;
	}

	return szBound;
}

void CXTPMarkupEllipse::OnRender(CXTPMarkupDrawingContext* pDC)
{

	CXTPMarkupStrokeStyle strokeStyle;
	GetStrokeStyle(&strokeStyle);

	CXTPMarkupBrush* pFillBrush = GetFill();

	if (!strokeStyle.pStrokeBrush && !pFillBrush)
		return;

	pDC->Ellipse(CRect(0, 0, GetRenderSize().cx, GetRenderSize().cy), &strokeStyle, pFillBrush);
}

CXTPMarkupInputElement* CXTPMarkupEllipse::InputHitTestOverride(CPoint point) const
{
	if (m_szRenderSize.cx < 1 || m_szRenderSize.cy < 1)
		return (CXTPMarkupInputElement*)this;

   // Determine radii
	double a = m_szRenderSize.cx / 2;
	double b = m_szRenderSize.cy / 2;

	double x = point.x - a;
	double y = point.y - b;

	return ((x * x) / (a * a) + (y * y) / (b * b) <= 1) ? (CXTPMarkupInputElement*)this : NULL;
}


#ifdef _XTP_ACTIVEX

//////////////////////////////////////////////////////////////////////////
// Ellipse


BEGIN_DISPATCH_MAP(CXTPMarkupEllipse, CXTPMarkupShape)
END_DISPATCH_MAP()

// {10229842-AE6D-46a8-BC77-CEE7D5CE9ED7}
static const GUID IID_IMarkupEllipse =
{ 0x10229842, 0xae6d, 0x46a8, { 0xbc, 0x77, 0xce, 0xe7, 0xd5, 0xce, 0x9e, 0xd7 } };


BEGIN_INTERFACE_MAP(CXTPMarkupEllipse, CXTPMarkupFrameworkElement)
	INTERFACE_PART(CXTPMarkupEllipse, IID_IMarkupEllipse, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPMarkupEllipse, IID_IMarkupEllipse)

#endif // _XTP_ACTIVEX
