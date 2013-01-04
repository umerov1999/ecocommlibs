// XTPMarkupPoint.cpp: implementation of the CXTPMarkupPoint class.
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
#include "../XTPMarkupBuilder.h"


//////////////////////////////////////////////////////////////////////////
// CXTPMarkupPoint

IMPLEMENT_MARKUPCLASS(NULL, CXTPMarkupPoint, CXTPMarkupObject);

void CXTPMarkupPoint::RegisterMarkupClass()
{

}

CXTPMarkupPoint::CXTPMarkupPoint()
{
	x = 0;
	y = 0;
}

CXTPMarkupPoint::CXTPMarkupPoint(double x, double y)
{
	this->x = x;
	this->y = y;
}

CXTPMarkupPoint& CXTPMarkupPoint::operator=(const CXTPMarkupPoint& srcPoint)
{
	x = srcPoint.x;
	y = srcPoint.y;
	return *this;
}

CXTPMarkupObject* CXTPMarkupPoint::ConvertFrom(CXTPMarkupBuilder* /*pBuilder*/, CXTPMarkupObject* pObject) const
{
	if (IsStringObject(pObject))
	{
		LPCWSTR lpszValue = *(CXTPMarkupString*)pObject;

		double x = 0, y = 0;
		if (!CXTPMarkupBuilder::ConvertDouble(lpszValue, x, _T(','), FALSE))
			return NULL;

		if (!CXTPMarkupBuilder::ConvertDouble(lpszValue, y))
			return NULL;

		return new CXTPMarkupPoint(x, y);
	}
	return NULL;
}
