// XTPMarkupRectangle.cpp: implementation of the CXTPMarkupRectangle class.
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
// CXTPMarkupRectangle

IMPLEMENT_MARKUPCLASS(L"Rectangle", CXTPMarkupRectangle, CXTPMarkupShape)

CXTPMarkupDependencyProperty* CXTPMarkupRectangle::m_pRadiusXProperty = NULL;
CXTPMarkupDependencyProperty* CXTPMarkupRectangle::m_pRadiusYProperty = NULL;

void CXTPMarkupRectangle::RegisterMarkupClass()
{
	m_pRadiusXProperty = CXTPMarkupDependencyProperty::Register(L"RadiusX", MARKUP_TYPE(CXTPMarkupInt), MARKUP_TYPE(CXTPMarkupRectangle),
		new CXTPMarkupPropertyMetadata(NULL, CXTPMarkupPropertyMetadata::flagAffectsRender));

	m_pRadiusYProperty = CXTPMarkupDependencyProperty::Register(L"RadiusY", MARKUP_TYPE(CXTPMarkupInt), MARKUP_TYPE(CXTPMarkupRectangle),
		new CXTPMarkupPropertyMetadata(NULL, CXTPMarkupPropertyMetadata::flagAffectsRender));

}

CXTPMarkupRectangle::CXTPMarkupRectangle()
{
}

CXTPMarkupRectangle::~CXTPMarkupRectangle()
{

}

CSize CXTPMarkupRectangle::MeasureOverride(CXTPMarkupDrawingContext* /*pDC*/, CSize szAvailableSize)
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

void CXTPMarkupRectangle::OnRender(CXTPMarkupDrawingContext* pDC)
{
	CXTPMarkupBrush* pStrokeBrush = GetStroke();
	CXTPMarkupBrush* pFillBrush = GetFill();

	if (!pStrokeBrush && !pFillBrush)
		return;

	int nStroke = pStrokeBrush ? GetStrokeThickness() : 0;

	CRect rc(0, 0, GetRenderSize().cx, GetRenderSize().cy);
	if (rc.Width() == 0 || rc.Height() == 0)
		return;

	double nRadiusX = GetRadiusX(), nRadiusY = GetRadiusY();
	nRadiusX = min((double)(rc.Width() - nStroke) / 2, nRadiusX);
	nRadiusY = min((double)(rc.Height() - nStroke) / 2, nRadiusY);

	double* pnCorners = NULL;
	if (nRadiusX > 0 && nRadiusY > 0)
	{
		pnCorners = new double[8];
		pnCorners[0] = pnCorners[3] = pnCorners[4] = pnCorners[7] = (double)nRadiusY;
		pnCorners[1] = pnCorners[2] = pnCorners[5] = pnCorners[6] = (double)nRadiusX;
	}

	if (pFillBrush)
	{
		CRect rcFill(rc);
		rcFill.DeflateRect(nStroke / 2, nStroke / 2);

		pDC->FillRectangle(rcFill, pFillBrush, pnCorners);
	}

	if (nStroke > 0)
	{
		CXTPMarkupThickness strokeThickness(nStroke);
		pDC->DrawRectangle(rc, pStrokeBrush, &strokeThickness, pnCorners);
	}

	if (pnCorners) delete[] pnCorners;
}


#ifdef _XTP_ACTIVEX

//////////////////////////////////////////////////////////////////////////
// Rectangle


BEGIN_DISPATCH_MAP(CXTPMarkupRectangle, CXTPMarkupShape)
	DISP_PROPERTY_EX_ID(CXTPMarkupRectangle, "RadiusX", 600, OleGetRadiusX, OleSetRadiusX, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPMarkupRectangle, "RadiusY", 601, OleGetRadiusY, OleSetRadiusY, VT_I4)
END_DISPATCH_MAP()

// {10219842-AE6D-46a8-BC77-CEE7D5CE9ED7}
static const GUID IID_IMarkupRectangle =
{ 0x10219842, 0xae6d, 0x46a8, { 0xbc, 0x77, 0xce, 0xe7, 0xd5, 0xce, 0x9e, 0xd7 } };


BEGIN_INTERFACE_MAP(CXTPMarkupRectangle, CXTPMarkupFrameworkElement)
	INTERFACE_PART(CXTPMarkupRectangle, IID_IMarkupRectangle, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPMarkupRectangle, IID_IMarkupRectangle)

int CXTPMarkupRectangle::OleGetRadiusX()
{
	return GetRadiusX();
}

void CXTPMarkupRectangle::OleSetRadiusX(int nValue)
{
	SetRadiusX(nValue);
}

int CXTPMarkupRectangle::OleGetRadiusY()
{
	return GetRadiusY();
}

void CXTPMarkupRectangle::OleSetRadiusY(int nValue)
{
	SetRadiusY(nValue);
}

#endif // _XTP_ACTIVEX
