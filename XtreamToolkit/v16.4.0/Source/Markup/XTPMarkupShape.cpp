// XTPMarkupShape.cpp: implementation of the CXTPMarkupShape class.
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
// CXTPMarkupShape


CXTPMarkupDependencyProperty* CXTPMarkupShape::m_pFillProperty               = NULL;
CXTPMarkupDependencyProperty* CXTPMarkupShape::m_pStrokeProperty             = NULL;
CXTPMarkupDependencyProperty* CXTPMarkupShape::m_pStrokeThicknessProperty    = NULL;
CXTPMarkupDependencyProperty* CXTPMarkupShape::m_pStretchProperty            = NULL;
CXTPMarkupDependencyProperty* CXTPMarkupShape::m_pStrokeDashArrayProperty    = NULL;
CXTPMarkupDependencyProperty* CXTPMarkupShape::m_pStrokeStartLineCapProperty = NULL;
CXTPMarkupDependencyProperty* CXTPMarkupShape::m_pStrokeEndLineCapProperty   = NULL;
CXTPMarkupDependencyProperty* CXTPMarkupShape::m_pStrokeLineJoinProperty     = NULL;
CXTPMarkupDependencyProperty* CXTPMarkupShape::m_pSmoothingMode              = NULL;

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

	m_pSmoothingMode = CXTPMarkupDependencyProperty::Register(L"SmoothingMode", MARKUP_TYPE(CXTPMarkupEnum), MARKUP_TYPE(CXTPMarkupShape),
		new CXTPMarkupPropertyMetadata(CXTPMarkupEnum::CreateValue(xtpMarkupSmoothingHighQuality), &CXTPMarkupShape::ConvertSmoothing, CXTPMarkupPropertyMetadata::flagAffectsRender));
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

CXTPMarkupObject* CXTPMarkupShape::ConvertSmoothing(CXTPMarkupBuilder*, CXTPMarkupObject *pObject)
{
	CXTPMarkupObject *pSmoothing = NULL; // Return value

	if (IsStringObject(pObject))
	{
		CXTPMarkupString *pString = MARKUP_DYNAMICCAST(CXTPMarkupString, pObject);

		if (0 == _wcsicmp(*pString, L"Default"))     pSmoothing = CXTPMarkupEnum::CreateValue(xtpMarkupSmoothingDefault);
		if (0 == _wcsicmp(*pString, L"None"))        pSmoothing = CXTPMarkupEnum::CreateValue(xtpMarkupSmoothingNone);
		if (0 == _wcsicmp(*pString, L"HighSpeed"))   pSmoothing = CXTPMarkupEnum::CreateValue(xtpMarkupSmoothingHighSpeed);
		if (0 == _wcsicmp(*pString, L"HighQuality")) pSmoothing = CXTPMarkupEnum::CreateValue(xtpMarkupSmoothingHighQuality);
		if (0 == _wcsicmp(*pString, L"AntiAlias"))   pSmoothing = CXTPMarkupEnum::CreateValue(xtpMarkupSmoothingAntiAlias);
	}

	return pSmoothing;
}


#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPMarkupShape, CXTPMarkupFrameworkElement)
	DISP_PROPERTY_EX_ID(CXTPMarkupShape, "Fill", 500, OleGetFill, OleSetFill, VT_DISPATCH)
	DISP_PROPERTY_EX_ID(CXTPMarkupShape, "Stroke", 501, OleGetStroke, OleSetStroke, VT_DISPATCH)
	DISP_PROPERTY_EX_ID(CXTPMarkupShape, "StrokeThickness", 502, OleGetStrokeThickness, OleSetStrokeThickness, VT_I4)

	DISP_PROPERTY_EX_ID(CXTPMarkupShape, "StrokeStartLineCap", 503, OleGetStrokeStartLineCap, OleSetStrokeStartLineCap, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPMarkupShape, "StrokeEndLineCap", 504, OleGetStrokeEndLineCap, OleSetStrokeEndLineCap, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPMarkupShape, "StrokeLineJoin", 505, OleGetStrokeLineJoin, OleSetStrokeLineJoin, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPMarkupShape, "StrokeDashArray", 506, OleGetStrokeDashArray, OleSetStrokeDashArray, VT_DISPATCH)

END_DISPATCH_MAP()

// {10119842-AE6D-46a8-BC77-CEE7D5CE9ED7}
static const GUID IID_IMarkupShape =
{ 0x10119842, 0xae6d, 0x46a8, { 0xbc, 0x77, 0xce, 0xe7, 0xd5, 0xce, 0x9e, 0xd7 } };


BEGIN_INTERFACE_MAP(CXTPMarkupShape, CXTPMarkupFrameworkElement)
	INTERFACE_PART(CXTPMarkupShape, IID_IMarkupShape, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPMarkupShape, IID_IMarkupShape)

LPDISPATCH CXTPMarkupShape::OleGetFill()
{
	return XTPGetDispatch(GetFill());
}

void CXTPMarkupShape::OleSetFill(LPDISPATCH lpBackground)
{
	SetFill(MARKUP_STATICCAST(CXTPMarkupBrush, FromDispatch(lpBackground)));
}

LPDISPATCH CXTPMarkupShape::OleGetStroke()
{
	return XTPGetDispatch(GetStroke());
}

void CXTPMarkupShape::OleSetStroke(LPDISPATCH lpBackground)
{
	SetStroke(MARKUP_STATICCAST(CXTPMarkupBrush, FromDispatch(lpBackground)));
}

int CXTPMarkupShape::OleGetStrokeThickness()
{
	return GetStrokeThickness();
}

void CXTPMarkupShape::OleSetStrokeThickness(int nThickness)
{
	SetStrokeThickness(nThickness);
}


int CXTPMarkupShape::OleGetStrokeStartLineCap()
{
	return GetStrokeStartLineCap();
}

void CXTPMarkupShape::OleSetStrokeStartLineCap(int nValue)
{
	SetStrokeStartLineCap((XTPMarkupLineCap)nValue);
}

int CXTPMarkupShape::OleGetStrokeEndLineCap()
{
	return GetStrokeEndLineCap();
}

void CXTPMarkupShape::OleSetStrokeEndLineCap(int nValue)
{
	SetStrokeEndLineCap((XTPMarkupLineCap)nValue);
}

int CXTPMarkupShape::OleGetStrokeLineJoin()
{
	return GetStrokeLineJoin();
}

void CXTPMarkupShape::OleSetStrokeLineJoin(int nValue)
{
	SetStrokeLineJoin((XTPMarkupLineJoin)nValue);
}

LPDISPATCH CXTPMarkupShape::OleGetStrokeDashArray()
{
	return XTPGetDispatch(GetStrokeDashArray());
}

void CXTPMarkupShape::OleSetStrokeDashArray(LPDISPATCH lpValue)
{
	SetStrokeDashArray(MARKUP_STATICCAST(CXTPMarkupDoubleCollection, FromDispatch(lpValue)));
}

#endif
