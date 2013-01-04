// XTPMarkupSolidColorBrush.cpp: implementation of the CXTPMarkupSolidColorBrush class.
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
// CXTPMarkupSolidColorBrush

CXTPMarkupDependencyProperty* CXTPMarkupSolidColorBrush::m_pColorProperty = NULL;

IMPLEMENT_MARKUPCLASS(L"SolidColorBrush", CXTPMarkupSolidColorBrush, CXTPMarkupBrush);

void CXTPMarkupSolidColorBrush::RegisterMarkupClass()
{
	m_pColorProperty = CXTPMarkupDependencyProperty::Register(L"Color", MARKUP_TYPE(CXTPMarkupColor), MARKUP_TYPE(CXTPMarkupSolidColorBrush));
}

CXTPMarkupSolidColorBrush::CXTPMarkupSolidColorBrush()
{
}

CXTPMarkupSolidColorBrush::CXTPMarkupSolidColorBrush(COLORREF clr)
{
	SetValue(m_pColorProperty, new CXTPMarkupColor(clr | 0xFF000000));
}

CXTPMarkupSolidColorBrush::CXTPMarkupSolidColorBrush(BYTE nAlpha, COLORREF clr)
{
	SetValue(m_pColorProperty, new CXTPMarkupColor(nAlpha, clr));
}

BOOL CXTPMarkupSolidColorBrush::IsEqual(const CXTPMarkupObject* pObject) const
{
	if (!pObject)
		return FALSE;

	if (pObject->GetType() != MARKUP_TYPE(CXTPMarkupSolidColorBrush))
		return FALSE;

	return GetHintColor() == ((CXTPMarkupSolidColorBrush*)pObject)->GetHintColor();

}

COLORREF CXTPMarkupSolidColorBrush::GetHintColor() const
{
	CXTPMarkupColor* pColor = MARKUP_STATICCAST(CXTPMarkupColor, GetValue(m_pColorProperty));

	if (pColor)
	{
		return (*pColor) & 0xFFFFFF;
	}

	return 0;
}

CXTPMarkupColor* CXTPMarkupSolidColorBrush::GetColor() const
{
	return MARKUP_STATICCAST(CXTPMarkupColor, GetValue(m_pColorProperty));
}


#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPMarkupSolidColorBrush, CXTPMarkupBrush)
	DISP_PROPERTY_EX_ID(CXTPMarkupSolidColorBrush, "Color", 100, OleGetColor, OleSetColor, VT_COLOR)
END_DISPATCH_MAP()

// {10169842-AE6D-46a8-BC77-CEE7D5CE9ED7}
static const GUID IID_IMarkupSolidColorBrush =
{ 0x10169842, 0xae6d, 0x46a8, { 0xbc, 0x77, 0xce, 0xe7, 0xd5, 0xce, 0x9e, 0xd7 } };


BEGIN_INTERFACE_MAP(CXTPMarkupSolidColorBrush, CXTPMarkupBrush)
	INTERFACE_PART(CXTPMarkupSolidColorBrush, IID_IMarkupSolidColorBrush, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPMarkupSolidColorBrush, IID_IMarkupSolidColorBrush)

COLORREF CXTPMarkupSolidColorBrush::OleGetColor()
{
	CXTPMarkupColor* pColor = MARKUP_STATICCAST(CXTPMarkupColor, GetValue(m_pColorProperty));
	return pColor != NULL ? (COLORREF)*pColor : 0xFF000000;
}

void CXTPMarkupSolidColorBrush::OleSetColor(COLORREF clr)
{
	int nAlpha = clr >> 24;

	if (nAlpha == 0) nAlpha = 0xFF;

	SetValue(m_pColorProperty, new CXTPMarkupColor(nAlpha,  clr));
}

#endif
