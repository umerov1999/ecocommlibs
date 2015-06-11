// XTPMarkupPointCollection.cpp: implementation of the CXTPMarkupPointCollection class.
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
