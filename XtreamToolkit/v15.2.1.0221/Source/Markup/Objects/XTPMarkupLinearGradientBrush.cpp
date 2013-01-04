// XTPMarkupLinearGradientBrush.cpp: implementation of the CXTPMarkupLinearGradientBrush class.
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
#include "../XTPMarkupDrawingContext.h"


//////////////////////////////////////////////////////////////////////////
// CXTPMarkupLinearGradientBrush

CXTPMarkupDependencyProperty* CXTPMarkupLinearGradientBrush::m_pStartPointProperty = NULL;
CXTPMarkupDependencyProperty* CXTPMarkupLinearGradientBrush::m_pEndPointProperty = NULL;
CXTPMarkupDependencyProperty* CXTPMarkupLinearGradientBrush::m_pGradientStopsProperty = NULL;

IMPLEMENT_MARKUPCLASS(L"LinearGradientBrush", CXTPMarkupLinearGradientBrush, CXTPMarkupBrush);

void CXTPMarkupLinearGradientBrush::RegisterMarkupClass()
{
	m_pStartPointProperty = CXTPMarkupDependencyProperty::Register(L"StartPoint", MARKUP_TYPE(CXTPMarkupPoint), MARKUP_TYPE(CXTPMarkupLinearGradientBrush));
	m_pEndPointProperty = CXTPMarkupDependencyProperty::Register(L"EndPoint", MARKUP_TYPE(CXTPMarkupPoint), MARKUP_TYPE(CXTPMarkupLinearGradientBrush));

	m_pGradientStopsProperty = CXTPMarkupDependencyProperty::Register(L"GradientStops", MARKUP_TYPE(CXTPMarkupGradientStops), MARKUP_TYPE(CXTPMarkupLinearGradientBrush));
}


CXTPMarkupLinearGradientBrush::CXTPMarkupLinearGradientBrush()
{
	SetValue(m_pGradientStopsProperty, new CXTPMarkupGradientStops());
}

CXTPMarkupLinearGradientBrush::CXTPMarkupLinearGradientBrush(CXTPMarkupGradientStops* pGradientStops)
{
	SetValue(m_pGradientStopsProperty, pGradientStops);
}

COLORREF CXTPMarkupLinearGradientBrush::GetHintColor() const
{
	CXTPMarkupGradientStops* pGradientStops = GetGradientStops();

	if (!pGradientStops || pGradientStops->GetCount() == 0)
		return 0;

	return pGradientStops->GetItem(0)->GetColor() & 0xFFFFFF;
}

CXTPMarkupLinearGradientBrush::~CXTPMarkupLinearGradientBrush()
{
}

void CXTPMarkupLinearGradientBrush::SetStartPoint(double x, double y)
{
	SetValue(m_pStartPointProperty, new CXTPMarkupPoint(x, y));
}

void CXTPMarkupLinearGradientBrush::SetEndPoint(double x, double y)
{
	SetValue(m_pEndPointProperty, new CXTPMarkupPoint(x, y));
}

void CXTPMarkupLinearGradientBrush::SetContentObject(CXTPMarkupBuilder* pBuilder, CXTPMarkupObject* pContent)
{
	CXTPMarkupGradientStops* pGradientStops = GetGradientStops();
	if (pGradientStops) pGradientStops->SetContentObject(pBuilder, pContent);
}
