// XTPMarkupShape.cpp: implementation of the CXTPMarkupShape class.
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
#include <math.h>

#include <Common/XTPVC80Helpers.h>

#include "XTPMarkupObject.h"
#include "XTPMarkupInputElement.h"
#include "XTPMarkupUIElement.h"
#include "XTPMarkupFrameworkElement.h"
#include "XTPMarkupShape.h"
#include "XTPMarkupDrawingContext.h"
#include "XTPMarkupBuilder.h"
#include "XTPMarkupThickness.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPMarkupShape


CXTPMarkupDependencyProperty* CXTPMarkupShape::m_pFillProperty = NULL;
CXTPMarkupDependencyProperty* CXTPMarkupShape::m_pStrokeProperty = NULL;
CXTPMarkupDependencyProperty* CXTPMarkupShape::m_pStrokeThicknessProperty = NULL;
CXTPMarkupDependencyProperty* CXTPMarkupShape::m_pStretchProperty = NULL;
CXTPMarkupDependencyProperty* CXTPMarkupShape::m_pStrokeDashArrayProperty = NULL;
CXTPMarkupDependencyProperty* CXTPMarkupShape::m_pStrokeStartLineCapProperty = NULL;
CXTPMarkupDependencyProperty* CXTPMarkupShape::m_pStrokeEndLineCapProperty = NULL;
CXTPMarkupDependencyProperty* CXTPMarkupShape::m_pStrokeLineJoinProperty = NULL;

IMPLEMENT_MARKUPCLASS(L"Shape", CXTPMarkupShape, CXTPMarkupFrameworkElement)

void CXTPMarkupShape::RegisterMarkupClass()
{
	m_pFillProperty = CXTPMarkupDependencyProperty::Register(L"Fill", MARKUP_TYPE(CXTPMarkupBrush), MARKUP_TYPE(CXTPMarkupShape),
		new CXTPMarkupPropertyMetadata(NULL, CXTPMarkupPropertyMetadata::flagAffectsRender));

	m_pStrokeProperty = CXTPMarkupDependencyProperty::Register(L"Stroke", MARKUP_TYPE(CXTPMarkupBrush), MARKUP_TYPE(CXTPMarkupShape),
		new CXTPMarkupPropertyMetadata(NULL, CXTPMarkupPropertyMetadata::flagAffectsRender));

	m_pStrokeThicknessProperty = CXTPMarkupDependencyProperty::Register(L"StrokeThickness", MARKUP_TYPE(CXTPMarkupInt), MARKUP_TYPE(CXTPMarkupShape),
		new CXTPMarkupPropertyMetadata(NULL, CXTPMarkupPropertyMetadata::flagAffectsRender));

	m_pStretchProperty = CXTPMarkupDependencyProperty::Register(L"Stretch", MARKUP_TYPE(CXTPMarkupEnum), MARKUP_TYPE(CXTPMarkupShape),
		new CXTPMarkupPropertyMetadata(CXTPMarkupEnum::CreateValue(xtpMarkupStretchNone), &CXTPMarkupBuilder::ConvertStretch, CXTPMarkupPropertyMetadata::flagAffectsRender));

	m_pStrokeDashArrayProperty = CXTPMarkupDependencyProperty::Register(L"StrokeDashArray", MARKUP_TYPE(CXTPMarkupDoubleCollection), MARKUP_TYPE(CXTPMarkupShape),
		new CXTPMarkupPropertyMetadata(NULL, CXTPMarkupPropertyMetadata::flagAffectsMeasure));

	m_pStrokeStartLineCapProperty = CXTPMarkupDependencyProperty::Register(L"StrokeStartLineCap", MARKUP_TYPE(CXTPMarkupEnum), MARKUP_TYPE(CXTPMarkupShape),
		new CXTPMarkupPropertyMetadata(CXTPMarkupEnum::CreateValue(xtpMarkupLineCapFlat), &CXTPMarkupShape::ConvertLineCap, CXTPMarkupPropertyMetadata::flagAffectsRender));

	m_pStrokeEndLineCapProperty = CXTPMarkupDependencyProperty::Register(L"StrokeEndLineCap", MARKUP_TYPE(CXTPMarkupEnum), MARKUP_TYPE(CXTPMarkupShape),
		new CXTPMarkupPropertyMetadata(CXTPMarkupEnum::CreateValue(xtpMarkupLineCapFlat), &CXTPMarkupShape::ConvertLineCap, CXTPMarkupPropertyMetadata::flagAffectsRender));

	m_pStrokeLineJoinProperty = CXTPMarkupDependencyProperty::Register(L"StrokeLineJoin", MARKUP_TYPE(CXTPMarkupEnum), MARKUP_TYPE(CXTPMarkupShape),
		new CXTPMarkupPropertyMetadata(CXTPMarkupEnum::CreateValue(xtpMarkupLineJoinMiter), &CXTPMarkupShape::ConvertLineJoin, CXTPMarkupPropertyMetadata::flagAffectsRender));
}

CXTPMarkupShape::CXTPMarkupShape()
{
}

CXTPMarkupShape::~CXTPMarkupShape()
{

}

CXTPMarkupBrush* CXTPMarkupShape::GetFill() const
{
	return MARKUP_STATICCAST(CXTPMarkupBrush, GetValue(m_pFillProperty));
}

void CXTPMarkupShape::SetFill(CXTPMarkupBrush* brush)
{
	SetValue(m_pFillProperty, brush);
}

CXTPMarkupBrush* CXTPMarkupShape::GetStroke() const
{
	return MARKUP_STATICCAST(CXTPMarkupBrush, GetValue(m_pStrokeProperty));
}

void CXTPMarkupShape::SetStroke(CXTPMarkupBrush* brush)
{
	SetValue(m_pStrokeProperty, brush);
}

void CXTPMarkupShape::GetStrokeStyle(CXTPMarkupStrokeStyle* pStrokeStyle) const
{
	pStrokeStyle->pStrokeBrush = GetStroke();
	pStrokeStyle->nStrokeThickness = pStrokeStyle->pStrokeBrush ? GetStrokeThickness() : 0;
	pStrokeStyle->pStrokeDashArray = GetStrokeDashArray();
	pStrokeStyle->nStrokeStartLineCap = GetStrokeStartLineCap();
	pStrokeStyle->nStrokeEndLineCap = GetStrokeEndLineCap();
	pStrokeStyle->nStrokeLineJoin = GetStrokeLineJoin();
}

CXTPMarkupObject* CXTPMarkupShape::ConvertLineCap(CXTPMarkupBuilder* /*pBuilder*/, CXTPMarkupObject* pObject)
{
	if (IsStringObject(pObject))
	{
		LPCWSTR lpszValue = *((CXTPMarkupString*)pObject);
		int nLength = ((CXTPMarkupString*)pObject)->GetLength();

		if (nLength == 4 && _wcsicmp(lpszValue, L"Flat") == 0) return CXTPMarkupEnum::CreateValue(xtpMarkupLineCapFlat);
		if (nLength == 6 && _wcsicmp(lpszValue, L"Square") == 0) return CXTPMarkupEnum::CreateValue(xtpMarkupLineCapSquare);
		if (nLength == 5 && _wcsicmp(lpszValue, L"Round") == 0) return CXTPMarkupEnum::CreateValue(xtpMarkupLineCapRound);
		if (nLength == 8 && _wcsicmp(lpszValue, L"Triangle") == 0) return CXTPMarkupEnum::CreateValue(xtpMarkupLineCapTriangle);

	}
	return NULL;
}

CXTPMarkupObject* CXTPMarkupShape::ConvertLineJoin(CXTPMarkupBuilder* /*pBuilder*/, CXTPMarkupObject* pObject)
{
	if (IsStringObject(pObject))
	{
		LPCWSTR lpszValue = *((CXTPMarkupString*)pObject);
		int nLength = ((CXTPMarkupString*)pObject)->GetLength();

		if (nLength == 5 && _wcsicmp(lpszValue, L"Miter") == 0) return CXTPMarkupEnum::CreateValue(xtpMarkupLineJoinMiter);
		if (nLength == 5 && _wcsicmp(lpszValue, L"Bevel") == 0) return CXTPMarkupEnum::CreateValue(xtpMarkupLineJoinBevel);
		if (nLength == 5 && _wcsicmp(lpszValue, L"Round") == 0) return CXTPMarkupEnum::CreateValue(xtpMarkupLineJoinRound);

	}
	return NULL;
}


//////////////////////////////////////////////////////////////////////////
// CXTPMarkupPointCollection

IMPLEMENT_MARKUPCLASS(NULL, CXTPMarkupPointCollection, CXTPMarkupObject)

void CXTPMarkupPointCollection::RegisterMarkupClass()
{

}

CXTPMarkupPointCollection::CXTPMarkupPointCollection()
{
	m_rcBounds.SetRectEmpty();
}

CXTPMarkupPointCollection::CXTPMarkupPointCollection(CPointArray& arr)
{
	m_rcBounds.SetRectEmpty();

	m_arrPoints.Copy(arr);

	int nCount = (int)m_arrPoints.GetSize();

	if (nCount > 0)
	{
		m_rcBounds.SetRect(INT_MAX, INT_MAX, -INT_MAX, -INT_MAX);

		for (int i = 0; i < nCount; i++)
		{
			POINT& pt = m_arrPoints[i];
			if (pt.x > m_rcBounds.right) m_rcBounds.right = pt.x;
			if (pt.y > m_rcBounds.bottom) m_rcBounds.bottom = pt.y;
			if (pt.x < m_rcBounds.left) m_rcBounds.left = pt.x;
			if (pt.y < m_rcBounds.top) m_rcBounds.top = pt.y;
		}
	}
}


BOOL CXTPMarkupPointCollection::GetNextValue(LPCWSTR& lpszValue, int& nValue)
{
	while (*lpszValue == ' ') lpszValue++;
	LPCWSTR lpszNext = lpszValue;

	nValue = 0;
	int nSign = 1;
	if (*lpszNext == '-')
	{
		nSign = -1;
		lpszNext++;
	}

	if (*lpszNext == 0)
		return FALSE;

	while (*lpszNext != 0)
	{
		WCHAR c = *lpszNext;

		if (c == ' ' || c == ',')
		{
			if (lpszNext == lpszValue)
				return FALSE;
			lpszNext++;
			break;
		}

		if (c <'0' || c > '9')
			return FALSE;

		nValue = 10 * nValue + (c - '0');

		lpszNext++;
	}

	lpszValue = lpszNext;
	nValue *= nSign;

	return TRUE;
}

BOOL CXTPMarkupPointCollection::ConvertFromString(LPCWSTR lpszValue, CPointArray& arr)
{
	if (!lpszValue)
		return FALSE;

	while (*lpszValue != 0)
	{
		int x, y;
		if (!GetNextValue(lpszValue, x))
			return FALSE;

		if (!GetNextValue(lpszValue, y))
			return FALSE;

		while (*lpszValue == ' ') lpszValue++;

		POINT pt = {x, y};
		arr.Add(pt);
	}

	return TRUE;
}

CXTPMarkupObject* CXTPMarkupPointCollection::ConvertFrom(CXTPMarkupBuilder* /*pBuilder*/, CXTPMarkupObject* pObject) const
{
	if (IsStringObject(pObject))
	{
		CPointArray arr;
		if (ConvertFromString(*((CXTPMarkupString*)pObject), arr))
		{
			return new CXTPMarkupPointCollection(arr);
		}
	}

	return NULL;
}

//////////////////////////////////////////////////////////////////////////
// CXTPMarkupPolygon


CXTPMarkupDependencyProperty* CXTPMarkupPolygon::m_pPointsProperty = NULL;

IMPLEMENT_MARKUPCLASS(L"Polygon", CXTPMarkupPolygon, CXTPMarkupShape)

void CXTPMarkupPolygon::RegisterMarkupClass()
{
	m_pPointsProperty = CXTPMarkupDependencyProperty::Register(L"Points", MARKUP_TYPE(CXTPMarkupPointCollection), MARKUP_TYPE(CXTPMarkupPolygon),
		new CXTPMarkupPropertyMetadata(NULL, CXTPMarkupPropertyMetadata::flagAffectsMeasure));

}

CXTPMarkupPolygon::CXTPMarkupPolygon()
{
}

CXTPMarkupPolygon::~CXTPMarkupPolygon()
{

}

void CXTPMarkupPolygon::SetPoints(CXTPMarkupPointCollection* pPoints)
{
	SetValue(m_pPointsProperty, pPoints);
}

CXTPMarkupPointCollection* CXTPMarkupPolygon::GetPoints() const
{
	return MARKUP_STATICCAST(CXTPMarkupPointCollection, GetValue(m_pPointsProperty));
}

CSize CXTPMarkupPolygon::MeasureOverride(CXTPMarkupDrawingContext* /*pDC*/, CSize szAvailableSize)
{
	CXTPMarkupPointCollection* pPoints = GetPoints();
	if (!pPoints)
		return CSize(0, 0);
	CRect rcBounds = pPoints->GetBounds();

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

void CXTPMarkupPolygon::OnRender(CXTPMarkupDrawingContext* pDC)
{
	CXTPMarkupStrokeStyle strokeStyle;
	GetStrokeStyle(&strokeStyle);

	CXTPMarkupBrush* pFillBrush = GetFill();

	if (!strokeStyle.pStrokeBrush && !pFillBrush)
		return;

	CXTPMarkupPointCollection* pPoints = GetPoints();
	if (!pPoints)
		return;

	const CXTPMarkupPointCollection::CPointArray& arr = pPoints->GetPoints();

	if (GetStretch() != xtpMarkupStretchNone)
	{
		CXTPMarkupPointCollection::CPointArray arrStretch;
		pPoints->Stretch(arrStretch, GetRenderSize());

		pDC->Polygon(arrStretch.GetData(), (int)arrStretch.GetSize(), &strokeStyle, pFillBrush);
	}
	else
	{
		pDC->Polygon(arr.GetData(), (int)arr.GetSize(), &strokeStyle, pFillBrush);
	}
}

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

void CXTPMarkupPointCollection::Stretch(CPointArray& arrStretch, CSize sz)
{
	const CPointArray& arr = GetPoints();
	arrStretch.Copy(arr);
	int nCount = (int)arrStretch.GetSize();

	CRect rc = GetBounds();
	if (rc.Width() == 0 && rc.Height() == 0)
		return;

	for (int i = 0; i < nCount; i++)
	{
		POINT& pt = arrStretch[i];
		if (rc.Width() != 0) pt.x = (pt.x - rc.left) * sz.cx / rc.Width();
		if (rc.Height() != 0) pt.y = (pt.y - rc.top) * sz.cy / rc.Height();
	}
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


//////////////////////////////////////////////////////////////////////////
// CXTPMarkupLine


CXTPMarkupDependencyProperty* CXTPMarkupLine::m_pX1Property = NULL;
CXTPMarkupDependencyProperty* CXTPMarkupLine::m_pY1Property = NULL;
CXTPMarkupDependencyProperty* CXTPMarkupLine::m_pX2Property = NULL;
CXTPMarkupDependencyProperty* CXTPMarkupLine::m_pY2Property = NULL;


IMPLEMENT_MARKUPCLASS(L"Line", CXTPMarkupLine, CXTPMarkupShape)

void CXTPMarkupLine::RegisterMarkupClass()
{
	m_pX1Property = CXTPMarkupDependencyProperty::Register(L"X1", MARKUP_TYPE(CXTPMarkupInt), MARKUP_TYPE(CXTPMarkupLine),
		new CXTPMarkupPropertyMetadata(NULL, CXTPMarkupPropertyMetadata::flagAffectsMeasure));

	m_pY1Property = CXTPMarkupDependencyProperty::Register(L"Y1", MARKUP_TYPE(CXTPMarkupInt), MARKUP_TYPE(CXTPMarkupLine),
		new CXTPMarkupPropertyMetadata(NULL, CXTPMarkupPropertyMetadata::flagAffectsMeasure));

	m_pX2Property = CXTPMarkupDependencyProperty::Register(L"X2", MARKUP_TYPE(CXTPMarkupInt), MARKUP_TYPE(CXTPMarkupLine),
		new CXTPMarkupPropertyMetadata(NULL, CXTPMarkupPropertyMetadata::flagAffectsMeasure));

	m_pY2Property = CXTPMarkupDependencyProperty::Register(L"Y2", MARKUP_TYPE(CXTPMarkupInt), MARKUP_TYPE(CXTPMarkupLine),
		new CXTPMarkupPropertyMetadata(NULL, CXTPMarkupPropertyMetadata::flagAffectsMeasure));
}

CXTPMarkupLine::CXTPMarkupLine()
{
}

CXTPMarkupLine::~CXTPMarkupLine()
{

}

int CXTPMarkupLine::GetX1() const
{
	CXTPMarkupInt* pValue = MARKUP_STATICCAST(CXTPMarkupInt, GetValue(m_pX1Property));
	return pValue != NULL ? (int)*pValue : 0;
}

int CXTPMarkupLine::GetX2() const
{
	CXTPMarkupInt* pValue = MARKUP_STATICCAST(CXTPMarkupInt, GetValue(m_pX2Property));
	return pValue != NULL ? (int)*pValue : 0;
}

int CXTPMarkupLine::GetY1() const
{
	CXTPMarkupInt* pValue = MARKUP_STATICCAST(CXTPMarkupInt, GetValue(m_pY1Property));
	return pValue != NULL ? (int)*pValue : 0;
}

int CXTPMarkupLine::GetY2() const
{
	CXTPMarkupInt* pValue = MARKUP_STATICCAST(CXTPMarkupInt, GetValue(m_pY2Property));
	return pValue != NULL ? (int)*pValue : 0;
}

CSize CXTPMarkupLine::MeasureOverride(CXTPMarkupDrawingContext* /*pDC*/, CSize szAvailableSize)
{
	int x1 = GetX1();
	int x2 = GetX2();
	int y1 = GetY1();
	int y2 = GetY2();

	int x = max(x1, x2);
	int y = max(y1, y2);

	if ((x1 == x2 || y1 == y2) && GetStroke())
	{
		int nStroke = (GetStrokeThickness() + 1) / 2;

		if (x1 == x2) x += nStroke;
		if (y1 == y2) y += nStroke;
	}

	CSize szBound(x, y);

	if (GetStretch() != xtpMarkupStretchNone)
	{
		if (szAvailableSize.cx < INT_MAX)
			szBound.cx = szAvailableSize.cx;

		if (szAvailableSize.cy < INT_MAX)
			szBound.cy = szAvailableSize.cy;
	}
	return szBound;

}

void CXTPMarkupLine::GetPoints(int& x1, int& y1, int& x2, int& y2) const
{
	x1 = GetX1();
	x2 = GetX2();
	y1 = GetY1();
	y2 = GetY2();

	if (GetStretch() != xtpMarkupStretchNone)
	{
		int x = max(x1, x2);
		int y = max(y1, y2);

		CSize sz = GetRenderSize();
		if (x1 == x2)
			x1 = x2 = sz.cx / 2;
		else
		{
			x1 = (x1 == x ? sz.cx : 0);
			x2 = (x2 == x ? sz.cx : 0);
		}

		if (y1 == y2)
			y1 = y2 = sz.cy / 2;
		else
		{
			y1 = (y1 == y ? sz.cy : 0);
			y2 = (y2 == y ? sz.cy : 0);
		}
	}
}

void CXTPMarkupLine::OnRender(CXTPMarkupDrawingContext* pDC)
{
	CXTPMarkupStrokeStyle strokeStyle;
	GetStrokeStyle(&strokeStyle);

	if (strokeStyle.nStrokeThickness <= 0)
		return;

	int x1, x2, y1, y2;
	GetPoints(x1, y1, x2, y2);

	if (x1 == 0 && x2 == 0 && y1 == 0 && y2 == 0)
		return;

	pDC->DrawLine(x1, y1, x2, y2, &strokeStyle);
}

CXTPMarkupInputElement* CXTPMarkupLine::InputHitTestOverride(CPoint point) const
{
	if (!GetStroke())
		return NULL;

	int nStroke = GetStrokeThickness();
	if (nStroke == 0)
		return NULL;

	int x1, x2, y1, y2;
	GetPoints(x1, y1, x2, y2);

	double x = x2 - x1;
	double y = y2 - y1;

	double xp = point.x - x1;
	double yp = point.y - y1;

	double s = (double)nStroke / 2.0;

	if (xp + s < min(x, 0) || xp - s > max(x, 0) || yp + s < min(y, 0) || yp - s > max(y, 0))
		return NULL;

	if (x == 0)
		return fabs(xp) <= s ? (CXTPMarkupInputElement*)this : NULL;

	double yStroke = fabs(s / cos(atan(y / x)));

	double y0 = y * xp / (double)x;

	return fabs(y0 - yp) <= yStroke ? (CXTPMarkupInputElement*)this : NULL;
}








