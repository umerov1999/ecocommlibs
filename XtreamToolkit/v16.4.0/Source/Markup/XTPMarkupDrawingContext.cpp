// XTPMarkupDrawingContext.cpp: implementation of the CXTPMarkupDrawingContext class.
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

#include <Common/XTPIntel80Helpers.h>

#include <Markup/XTPMarkupObject.h>
#include <Markup/XTPMarkupDrawingContext.h>
#include <Markup/XTPMarkupContext.h>
#include <Markup/XTPMarkupBuilder.h>
#include <Markup/XTPMarkupDeviceContext.h>
#include <Markup/XTPMarkupThickness.h>

#include <GraphicLibrary/GdiPlus/GdiPlus.h>
using namespace Gdiplus;

#include <Markup/DeviceContext/XTPGdiPlus.h>
#include <Markup/DeviceContext/XTPMarkupGdiPlusDeviceContext.h>
#include <Markup/DeviceContext/XTPMarkupGdiPlusExtendedDeviceContext.h>


#pragma warning(disable:4510)
#pragma warning(disable:4610)


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


void CXTPMarkupDrawingContext::Register(BOOL bInit)
{
#ifndef _XTP_EXCLUDE_GDIPLUS
	CXTPMarkupGdiPlusDeviceContext::Register(bInit);
#endif
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXTPMarkupDrawingContext::CXTPMarkupDrawingContext(CXTPMarkupContext* pMarkupContext, HDC hDC, HDC hAttribDC /* = 0 */)
{
	m_hDC = hDC;
	m_hAttribDC = hAttribDC ? hAttribDC : hDC;
	m_bDeleteDC = FALSE;

	Init(pMarkupContext);
}

CXTPMarkupDrawingContext::CXTPMarkupDrawingContext(CXTPMarkupContext* pMarkupContext)
{
	m_hDC = CreateCompatibleDC(0);
	m_hAttribDC = m_hDC;
	m_bDeleteDC = TRUE;

	Init(pMarkupContext);
}

CXTPMarkupDrawingContext::CXTPMarkupDrawingContext(HDC hDC)
{
	m_hDC = hDC;
	m_hAttribDC = m_hDC;
	m_bDeleteDC = FALSE;

	Init(NULL);
}

CXTPMarkupDrawingContext::CXTPMarkupDrawingContext()
{
	m_hDC = CreateCompatibleDC(0);
	m_hAttribDC = m_hDC;
	m_bDeleteDC = TRUE;

	Init(NULL);
}

CXTPMarkupDrawingContext::CXTPMarkupDrawingContext(CXTPMarkupContext* /*pMarkupContext*/, GpGraphics* pGpGraphics)
{
	m_hDC = NULL;
	m_hAttribDC = NULL;
	m_bDeleteDC = FALSE;

	m_pDeviceContext = new CXTPMarkupGdiPlusExtendedDeviceContext(pGpGraphics);

	m_pSelectedFont = NULL;
	m_hOldFont = NULL;

	m_hDWP = NULL;

	m_rcClipBox.SetRectEmpty();
	m_pDeviceContext->GetClipBox(m_rcClipBox);
}

CXTPMarkupDrawingContext::CXTPMarkupDrawingContext(CXTPMarkupContext* /*pMarkupContext*/, Graphics* pGraphics)
{
	USES_PROTECTED_ACCESS(CXTPMarkupDrawingContext, Graphics, GpGraphics, nativeGraphics);

	GpGraphics* pGpGraphics = PROTECTED_ACCESS(Graphics, pGraphics, nativeGraphics);

	m_hDWP = NULL;

	m_hDC = NULL;
	m_hAttribDC = NULL;
	m_bDeleteDC = FALSE;

	m_pDeviceContext = new CXTPMarkupGdiPlusExtendedDeviceContext(pGpGraphics);

	m_pSelectedFont = NULL;
	m_hOldFont = NULL;

	m_rcClipBox.SetRectEmpty();
	m_pDeviceContext->GetClipBox(m_rcClipBox);
}

void CXTPMarkupDrawingContext::Init(CXTPMarkupContext* pMarkupContext)
{
	if (!pMarkupContext || pMarkupContext->IsGdiPlusEnabled())
	{
#ifndef _XTP_EXCLUDE_GDIPLUS
		m_pDeviceContext = new CXTPMarkupGdiPlusDeviceContext(m_hDC, m_hAttribDC);
#else
		m_pDeviceContext = new CXTPMarkupDeviceContext(m_hDC, m_hAttribDC);
#endif
	}
	else
	{
		m_pDeviceContext = new CXTPMarkupDeviceContext(m_hDC, m_hAttribDC);
	}

	m_pSelectedFont = NULL;
	m_hOldFont = NULL;

	m_rcClipBox.SetRectEmpty();
	m_pDeviceContext->GetClipBox(m_rcClipBox);

	m_hDWP = NULL;

	::SetBkMode(m_hDC, TRANSPARENT);
}

CXTPMarkupDrawingContext::~CXTPMarkupDrawingContext()
{
	SetFont(NULL);

	delete m_pDeviceContext;

	if (m_bDeleteDC)
	{
		DeleteDC(m_hDC);
	}
}

HDC CXTPMarkupDrawingContext::Detach()
{
	SetFont(NULL);

	HDC hDC = m_hDC;
	m_hDC = NULL;
	return hDC;
}

void CXTPMarkupDrawingContext::OffsetViewport(const POINT& ptVisualOffset)
{
	m_pDeviceContext->OffsetViewport(ptVisualOffset);

	m_pDeviceContext->GetClipBox(m_rcClipBox);
}

void CXTPMarkupDrawingContext::SetSmoothingMode(XTPMarkupSmoothingMode smoothingMode)
{
	m_pDeviceContext->SetSmoothingMode(smoothingMode);
}

void CXTPMarkupDrawingContext::IntersectClipRect(const RECT& rcLayoutClip)
{
	m_pDeviceContext->IntersectClipRect(rcLayoutClip);

	m_pDeviceContext->GetClipBox(m_rcClipBox);
}

HRGN CXTPMarkupDrawingContext::SaveClipRegion()
{
	return m_pDeviceContext->SaveClipRegion();
}

void CXTPMarkupDrawingContext::RestoreClipRegion(HRGN hrgnClip)
{
	if (hrgnClip != NULL)
	{
		m_pDeviceContext->RestoreClipRegion(hrgnClip);

		m_pDeviceContext->GetClipBox(m_rcClipBox);
	}
}

void CXTPMarkupDrawingContext::FillSolidRect(CRect rc, COLORREF clr)
{
	CXTPMarkupSolidColorBrush brush(clr);
	m_pDeviceContext->FillRectangle(rc, &brush);

}
void CXTPMarkupDrawingContext::FillRectangle(CRect rc, CXTPMarkupBrush* pBrush, double* pCornerRadius)
{
	if (pCornerRadius)
		m_pDeviceContext->FillRoundRectangle(rc, pBrush, pCornerRadius);
	else
		m_pDeviceContext->FillRectangle(rc, pBrush);
}

void CXTPMarkupDrawingContext::DrawFrame(CRect rc, CXTPMarkupBrush* pBrush)
{
	CXTPMarkupThickness thickness(1);
	DrawRectangle(rc, pBrush, &thickness);
}

void CXTPMarkupDrawingContext::DrawTextLine(LPCWSTR lpszText, UINT nCount, LPCRECT lpRect)
{
	m_pDeviceContext->DrawString(lpszText, nCount, lpRect);
}

void CXTPMarkupDrawingContext::SetFont(CXTPMarkupFont* pFont)
{
	if (m_pSelectedFont == pFont)
		return;

	m_pDeviceContext->SetTextFont(pFont);

	MARKUP_RELEASE(m_pSelectedFont);
	m_pSelectedFont = pFont;
	MARKUP_ADDREF(m_pSelectedFont);
}

SIZE CXTPMarkupDrawingContext::GetTextExtent(LPCWSTR lpszText, int nCount) const
{
	return m_pDeviceContext->MeasureString(lpszText, nCount);
}


void CXTPMarkupDrawingContext::DrawLine(int x1, int y1, int x2, int y2, CXTPMarkupStrokeStyle* pStrokeStyle)
{
	if (!pStrokeStyle || pStrokeStyle->nStrokeThickness <= 0)
		return;

	m_pDeviceContext->DrawLine(x1, y1, x2, y2, pStrokeStyle);
}

void CXTPMarkupDrawingContext::Polygon(const POINT* points, int nCount, CXTPMarkupStrokeStyle* pStrokeStyle, CXTPMarkupBrush* pFillBrush)
{
	m_pDeviceContext->Polygon(points, nCount, pStrokeStyle, pFillBrush);
}

void CXTPMarkupDrawingContext::Polyline(const POINT* points, int nCount, CXTPMarkupStrokeStyle* pStrokeStyle)
{
	m_pDeviceContext->Polyline(points, nCount, pStrokeStyle);
}

void CXTPMarkupDrawingContext::Ellipse(CRect rc, CXTPMarkupStrokeStyle* pStrokeStyle, CXTPMarkupBrush* pFillBrush)
{
	m_pDeviceContext->Ellipse(rc, pStrokeStyle, pFillBrush);
}

void CXTPMarkupDrawingContext::DrawRectangle(CRect rc, CXTPMarkupBrush* pBrush, CXTPMarkupThickness* pThickness, double* pCornerRadius)
{
	if (!pThickness || !pBrush)
		return;

	if (pCornerRadius)
		m_pDeviceContext->DrawRoundRectangle(rc, pBrush, pThickness, pCornerRadius);
	else
		m_pDeviceContext->DrawRectangle(rc, pBrush, pThickness);
}

void CXTPMarkupDrawingContext::DrawPath(CXTPMarkupPathData *pData, CXTPMarkupStrokeStyle* pStrokeStyle, CXTPMarkupBrush* pFillBrush)
{
	m_pDeviceContext->DrawPath(pData, pStrokeStyle, pFillBrush);
}

void CXTPMarkupDrawingContext::DrawImage(CXTPImageManagerIcon* pIcon, CRect rc)
{
	m_pDeviceContext->DrawImage(pIcon, rc);
}

HDC CXTPMarkupDrawingContext::GetDC()
{
	return m_pDeviceContext->GetDC();
}

void CXTPMarkupDrawingContext::ReleaseDC(HDC hDC)
{
	m_pDeviceContext->ReleaseDC(hDC);
}

void CXTPMarkupDrawingContext::SetTextColor(CXTPMarkupBrush* pBrush)
{
	m_pDeviceContext->SetTextColor(pBrush);
}

//////////////////////////////////////////////////////////////////////////
// CXTPMarkupPrintingContext

CXTPMarkupPrintingContext::CXTPMarkupPrintingContext(CXTPMarkupContext* pMarkupContext, HDC hDC, HDC hAttribDC)
	: CXTPMarkupDrawingContext(pMarkupContext, hDC, hAttribDC)
{
}
CXTPMarkupPrintingContext::CXTPMarkupPrintingContext(HDC hDC, HDC hAttribDC)
	: CXTPMarkupDrawingContext(NULL, hDC, hAttribDC)
{
}
