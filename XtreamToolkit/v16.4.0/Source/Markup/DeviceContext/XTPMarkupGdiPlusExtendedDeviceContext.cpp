// XTPMarkupGdiPlusExtendedDeviceContext.cpp: implementation of the CXTPMarkupGdiPlusExtendedDeviceContext class.
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

#include "GraphicLibrary/GdiPlus/GdiPlus.h"
using namespace Gdiplus;

#include <Common/XTPImageManager.h>

#include <Markup/XTPMarkupObject.h>
#include <Markup/XTPMarkupInputElement.h>
#include <Markup/XTPMarkupUIElement.h>
#include <Markup/XTPMarkupFrameworkElement.h>
#include <Markup/XTPMarkupDrawingContext.h>
#include <Markup/XTPMarkupDeviceContext.h>
#include <Markup/XTPMarkupShape.h>
#include <Markup/XTPMarkupThickness.h>
#include <Markup/Path/XTPMarkupPathData.h>
#include <Markup/DeviceContext/XTPGdiPlus.h>
#include <Markup/DeviceContext/XTPMarkupGdiPlusDeviceContext.h>
#include <Markup/DeviceContext/XTPMarkupGdiPlusExtendedDeviceContext.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////////
//  CXTPMarkupGdiPlusExtendedDeviceContext

CXTPMarkupGdiPlusExtendedDeviceContext::CXTPMarkupGdiPlusExtendedDeviceContext(GpGraphics* pGpGraphics)
	: CXTPMarkupGdiPlusDeviceContext(0, 0)
{
	m_pGraphics = pGpGraphics;

	m_pGpFont = NULL;
	m_pGpTextBrush = NULL;

	GpStringFormat* pGpGenericTypographicStringFormat = NULL;
	GetGdiPlus()->GdipStringFormatGetGenericTypographic(&pGpGenericTypographicStringFormat);

	m_pGpStringFormat = NULL;
	GetGdiPlus()->GdipCloneStringFormat(pGpGenericTypographicStringFormat, &m_pGpStringFormat);

	GetGdiPlus()->GdipSetStringFormatFlags(m_pGpStringFormat, StringFormatFlagsNoWrap + StringFormatFlagsMeasureTrailingSpaces + StringFormatFlagsNoClip);

}

CXTPMarkupGdiPlusExtendedDeviceContext::~CXTPMarkupGdiPlusExtendedDeviceContext()
{
	if (m_pGpFont)
	{
		GetGdiPlus()->GdipDeleteFont(m_pGpFont);
		m_pGpFont = NULL;
	}

	if (m_pGpTextBrush)
	{
		GetGdiPlus()->GdipDeleteBrush(m_pGpTextBrush);
		m_pGpTextBrush = NULL;
	}

	GetGdiPlus()->GdipDeleteStringFormat(m_pGpStringFormat);
}


GpGraphics* CXTPMarkupGdiPlusExtendedDeviceContext::GetGraphics()
{
	return m_pGraphics;
}

void CXTPMarkupGdiPlusExtendedDeviceContext::ReleaseGraphics(GpGraphics* /*pGpGraphics*/)
{

}

void CXTPMarkupGdiPlusExtendedDeviceContext::GetClipBox(LPRECT lpRect)
{
	GpRect rect;
	GetGdiPlus()->GdipGetClipBoundsI(m_pGraphics, &rect);

	lpRect->left = rect.GetLeft();
	lpRect->top = rect.GetTop();
	lpRect->right = rect.GetRight();
	lpRect->bottom = rect.GetBottom();
}


HRGN CXTPMarkupGdiPlusExtendedDeviceContext::SaveClipRegion()
{
	GpRegion* pGpRegion = NULL;

	GetGdiPlus()->GdipCreateRegion(&pGpRegion);
	GetGdiPlus()->GdipGetClip(m_pGraphics, pGpRegion);

	return (HRGN)pGpRegion;

}

void CXTPMarkupGdiPlusExtendedDeviceContext::RestoreClipRegion(HRGN hrgnClip)
{
	GpRegion* pGpRegion = (GpRegion*)hrgnClip;
	if (pGpRegion != NULL)
	{
		GetGdiPlus()->GdipSetClipRegion(m_pGraphics, pGpRegion, CombineModeReplace);
		GetGdiPlus()->GdipDeleteRegion(pGpRegion);
	}
}


void CXTPMarkupGdiPlusExtendedDeviceContext::IntersectClipRect(const RECT& rcLayoutClip)
{
	GetGdiPlus()->GdipSetClipRectI(m_pGraphics, rcLayoutClip.left, rcLayoutClip.top,
		rcLayoutClip.right - rcLayoutClip.left, rcLayoutClip.bottom - rcLayoutClip.top, CombineModeIntersect);
}

void CXTPMarkupGdiPlusExtendedDeviceContext::OffsetViewport(const POINT& ptViewortOrg)
{
	GetGdiPlus()->GdipTranslateWorldTransform(m_pGraphics, (REAL)ptViewortOrg.x, (REAL)ptViewortOrg.y, MatrixOrderPrepend);

	HDC hDC = GetDC();
	POINT pt = TranslatePoint(ptViewortOrg);
	OffsetViewportOrgEx(hDC, pt.x, pt.y, NULL);
	ReleaseDC(hDC);
}

void CXTPMarkupGdiPlusExtendedDeviceContext::SetTextFont(CXTPMarkupFont* pFont)
{
	if (m_pGpFont)
	{
		GetGdiPlus()->GdipDeleteFont(m_pGpFont);
		m_pGpFont = NULL;
	}

	if (pFont)
	{
		HDC hDC = GetDC();

#ifdef _UNICODE
		GetGdiPlus()->GdipCreateFontFromLogfontW(hDC, &pFont->m_lf, &m_pGpFont);
#else
		GetGdiPlus()->GdipCreateFontFromLogfontA(hDC, &pFont->m_lf, &m_pGpFont);
#endif


		ReleaseDC(hDC);
	}
}

void CXTPMarkupGdiPlusExtendedDeviceContext::SetTextColor(CXTPMarkupBrush* pBrush)
{
	if (m_pGpTextBrush)
	{
		GetGdiPlus()->GdipDeleteBrush(m_pGpTextBrush);
		m_pGpTextBrush = NULL;
	}

	m_pGpTextBrush = CreateGpBrush(pBrush, CRect(0, 0, 1, 1));
}

SIZE CXTPMarkupGdiPlusExtendedDeviceContext::MeasureString(LPCWSTR lpszText, int nCount) const
{
	RectF rc;
	RectF layoutRect(0, 0, 0.0f, 0.0f);

	GetGdiPlus()->GdipMeasureString(m_pGraphics, lpszText, nCount, m_pGpFont, &layoutRect, m_pGpStringFormat, &rc, 0, 0);

	SIZE sz;
	sz.cx = (int)ceil(rc.Width);
	sz.cy = (int)ceil(rc.Height);

	return sz;
}

void CXTPMarkupGdiPlusExtendedDeviceContext::DrawString(LPCWSTR lpszString, UINT nCount, LPCRECT lpRect)
{
	RectF rc((REAL)lpRect->left, (REAL)lpRect->top, (REAL)(lpRect->right - lpRect->left), (REAL)(lpRect->bottom - lpRect->top));
	GetGdiPlus()->GdipDrawString(m_pGraphics, lpszString, nCount, m_pGpFont, &rc, m_pGpStringFormat, m_pGpTextBrush);
}

HDC CXTPMarkupGdiPlusExtendedDeviceContext::GetDC()
{
	HDC hDC;
	GetGdiPlus()->GdipGetDC(m_pGraphics, &hDC);
	return hDC;
}

void CXTPMarkupGdiPlusExtendedDeviceContext::ReleaseDC(HDC hDC)
{
	GetGdiPlus()->GdipReleaseDC(m_pGraphics, hDC);
}
