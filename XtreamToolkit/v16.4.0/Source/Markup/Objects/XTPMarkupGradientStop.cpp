// XTPMarkupGradientStop.cpp: implementation of the CXTPMarkupGradientStop class.
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
#include "../XTPMarkupDrawingContext.h"


//////////////////////////////////////////////////////////////////////////
// CXTPMarkupGradientStop
CXTPMarkupDependencyProperty* CXTPMarkupGradientStop::m_pColorProperty = NULL;
CXTPMarkupDependencyProperty* CXTPMarkupGradientStop::m_pOffsetProperty = NULL;

IMPLEMENT_MARKUPCLASS(L"GradientStop", CXTPMarkupGradientStop, CXTPMarkupObject);

void CXTPMarkupGradientStop::RegisterMarkupClass()
{
	m_pColorProperty = CXTPMarkupDependencyProperty::Register(L"Color", MARKUP_TYPE(CXTPMarkupColor), MARKUP_TYPE(CXTPMarkupGradientStop));
	m_pOffsetProperty = CXTPMarkupDependencyProperty::Register(L"Offset", MARKUP_TYPE(CXTPMarkupDouble), MARKUP_TYPE(CXTPMarkupGradientStop));
}

CXTPMarkupGradientStop::CXTPMarkupGradientStop()
{

}

CXTPMarkupGradientStop::CXTPMarkupGradientStop(COLORREF clr, double dOffset)
{
	SetValue(m_pColorProperty, new CXTPMarkupColor(clr));
	SetValue(m_pOffsetProperty, new CXTPMarkupDouble(dOffset));
}

COLORREF CXTPMarkupGradientStop::GetColor() const
{
	CXTPMarkupColor* pColor = MARKUP_STATICCAST(CXTPMarkupColor, GetValue(m_pColorProperty));
	return pColor != NULL ? (COLORREF)*pColor : 0;
}

double CXTPMarkupGradientStop::GetOffset() const
{
	CXTPMarkupDouble* pOffset = MARKUP_STATICCAST(CXTPMarkupDouble, GetValue(m_pOffsetProperty));
	return pOffset != NULL ? (double)*pOffset : 0.0;

}


IMPLEMENT_MARKUPCLASS(L"", CXTPMarkupGradientStops, CXTPMarkupCollection);

void CXTPMarkupGradientStops::RegisterMarkupClass()
{
}

CXTPMarkupGradientStops::CXTPMarkupGradientStops()
{
	m_pElementType = MARKUP_TYPE(CXTPMarkupGradientStop);
}
