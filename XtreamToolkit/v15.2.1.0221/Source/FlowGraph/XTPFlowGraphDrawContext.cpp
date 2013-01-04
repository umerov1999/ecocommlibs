// XTPFlowGraphDrawContext.cpp : implementation of the CXTPFlowGraphDrawContext class.
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

#include "Common/XTPDrawHelpers.h"
#include "Common/XTPResourceManager.h"
#include "Common/XTPImageManager.h"
#include "Common/XTPVC80Helpers.h"

#include "GraphicLibrary/GdiPlus/GdiPlus.h"

#include "Markup/XTPMarkupObject.h"
#include "Markup/XTPMarkupInputElement.h"
#include "Markup/XTPMarkupUIElement.h"
#include "Markup/XTPMarkupContext.h"
#include "Markup/XTPMarkupDrawingContext.h"

#include "XTPFlowGraphTools.h"
#include "XTPFlowGraphDrawContext.h"
#include "XTPFlowGraphControl.h"
#include "XTPFlowGraphImage.h"

using namespace Gdiplus;
using namespace Gdiplus::DllExports;

//////////////////////////////////////////////////////////////////////////
// CXTPFlowGraphDrawContext


CXTPFlowGraphDrawContext::CXTPFlowGraphDrawContext(HDC hDC, HDC /*hAttribDC*/, BOOL bPrinting)
{
	m_hDC = hDC;
	m_bPrinting = bPrinting;

#if 0
	SetSmoothingMode(0);
#endif

	m_hActiveBrush = NULL;
	m_hActivePen = NULL;
	m_hActiveFont = NULL;

	::GetClipBox(m_hDC, m_rcClipRect);
}

void CXTPFlowGraphDrawContext::SetSmoothingMode(int /*nMode*/)
{
}

CXTPFlowGraphDrawContext::~CXTPFlowGraphDrawContext()
{
	if (m_hActivePen) DeleteObject(m_hActivePen);
}

void CXTPFlowGraphDrawContext::SetWorldTransform(CPoint ptOffset, double dZoomLevel)
{
	::SetMapMode(m_hDC, MM_ANISOTROPIC);

	::SetViewportExtEx(m_hDC, int(dZoomLevel * 96), int(dZoomLevel * 96), 0);

	::SetWindowExtEx(m_hDC, int(96), int(96 ), 0);

	CSize m_sizeWinExt, m_sizeVpExt;

	::GetWindowExtEx(m_hDC, &m_sizeWinExt);
	::GetViewportExtEx(m_hDC, &m_sizeVpExt);

	SetViewportOrgEx(m_hDC, -ptOffset.x * m_sizeVpExt.cx / m_sizeWinExt.cx, -ptOffset.y * m_sizeVpExt.cy / m_sizeWinExt.cy, NULL);

	::GetClipBox(m_hDC, m_rcClipRect);
}

void CXTPFlowGraphDrawContext::SetClipRect(CRect /*rc*/)
{

}

CRect CXTPFlowGraphDrawContext::GetClipRect() const
{
	return m_rcClipRect;
}


void CXTPFlowGraphDrawContext::Clear(COLORREF clr)
{
	FillSolidRect(m_rcClipRect, clr);
}

void CXTPFlowGraphDrawContext::FillSolidRect(LPCRECT lpRect, COLORREF clr)
{
	if ((clr & 0xFF000000) == 0)
	{
		::SetBkColor(m_hDC, clr);
		::ExtTextOut(m_hDC, 0, 0, ETO_OPAQUE, lpRect, NULL, 0, NULL);
	}
	else
	{

		LPDWORD lpBits;
		HBITMAP hBitmap = CXTPImageManager::Create32BPPDIBSection(m_hDC, 1, 1, (LPBYTE*)&lpBits);
		if (hBitmap)
		{
			lpBits[0] = RGB(GetBValue(clr), GetGValue(clr), GetRValue(clr));

			HDC hDC = CreateCompatibleDC(m_hDC);
			HBITMAP hOldBitmap = (HBITMAP)SelectObject(hDC, hBitmap);

			XTPImageManager()->AlphaBlend2(m_hDC, lpRect, hDC, CRect(0, 0, 1, 1), 100);

			SelectObject(hDC, hOldBitmap);
			DeleteObject(hBitmap);
			DeleteDC(hDC);
		}
	}
}

void CXTPFlowGraphDrawContext::FillSolidRect(int x, int y, int cx, int cy, COLORREF clr)
{
	CRect rect(x, y, x + cx, y + cy);
	FillSolidRect(rect, clr);
}

void CXTPFlowGraphDrawContext::Draw3dRect(LPCRECT lpRect, COLORREF clrTopLeft, COLORREF clrBottomRight)
{
	Draw3dRect(lpRect->left, lpRect->top, lpRect->right - lpRect->left,
		lpRect->bottom - lpRect->top, clrTopLeft, clrBottomRight);

}

void CXTPFlowGraphDrawContext::Draw3dRect(int x, int y, int cx, int cy, COLORREF clrTopLeft, COLORREF clrBottomRight)
{
	FillSolidRect(x, y, cx - 1, 1, clrTopLeft);
	FillSolidRect(x, y, 1, cy - 1, clrTopLeft);
	FillSolidRect(x + cx, y, -1, cy, clrBottomRight);
	FillSolidRect(x, y + cy, cx, -1, clrBottomRight);
}


void CXTPFlowGraphDrawContext::DrawFrame(int x, int y, int cx, int cy, int sz, COLORREF clr)
{
	FillSolidRect(x, y, cx - sz, sz, clr);
	FillSolidRect(x, y, sz, cy - sz, clr);
	FillSolidRect(x + cx, y, -sz, cy, clr);
	FillSolidRect(x, y + cy, cx, -sz, clr);
}



CSize CXTPFlowGraphDrawContext::MeasureMarkupElement(CXTPMarkupUIElement* pMarkupUIElement)
{
	pMarkupUIElement->GetMarkupContext()->SetDefaultFont(m_hActiveFont);


	CXTPMarkupDrawingContext dc(pMarkupUIElement->GetMarkupContext(), m_hDC);
	pMarkupUIElement->Measure(&dc, CSize(INT_MAX, INT_MAX));

	CSize sz = pMarkupUIElement->GetDesiredSize();

	return sz;
}

void CXTPFlowGraphDrawContext::DrawMarkupElement(CXTPMarkupUIElement* pMarkupUIElement, LPCRECT lpRect, UINT /*uFormat*/)
{

	pMarkupUIElement->GetMarkupContext()->SetDefaultFont(m_hActiveFont);
	pMarkupUIElement->GetMarkupContext()->SetDefaultTextColor(::GetTextColor(m_hDC));

	CXTPMarkupDrawingContext dc(pMarkupUIElement->GetMarkupContext(), m_hDC);
	pMarkupUIElement->Arrange(lpRect);
	pMarkupUIElement->Render(&dc);
}


CSize CXTPFlowGraphDrawContext::MeasureString(const CString& str)
{
	if (!m_hActiveFont)
		return CSize(0, 0);

	HFONT hOldFont = (HFONT)SelectObject(m_hDC, m_hActiveFont);

	CSize sz;
	if (str.GetLength() != 0)
	{
		::GetTextExtentPoint(m_hDC, str, str.GetLength(), &sz);
	}
	else
	{
		::GetTextExtentPoint(m_hDC, _T(" "), 1, &sz);
	}

	::SelectObject(m_hDC, hOldFont);

	return sz;
}

void CXTPFlowGraphDrawContext::DrawText(LPCTSTR lpszText, LPCRECT lpRect, UINT uFormat)
{
	if (!m_hActiveFont)
		return;

	HFONT hOldFont = (HFONT)SelectObject(m_hDC, m_hActiveFont);

	::SetBkMode(m_hDC, TRANSPARENT);
	::DrawText(m_hDC, lpszText, (int)_tcslen(lpszText), (LPRECT)lpRect, uFormat | DT_SINGLELINE);

	::SelectObject(m_hDC, hOldFont);
}

void CXTPFlowGraphDrawContext::DrawCurve(Gdiplus::GraphicsPath* pPath)
{
	int nCount = pPath->GetPointCount();

	Point* lppt = new Point[nCount];
	pPath->GetPathPoints(lppt, nCount);

	BYTE* pTypes = new BYTE[nCount];
	pPath->GetPathTypes(pTypes, nCount);

	CDC* pDC = CDC::FromHandle(m_hDC);

	int nIndex;
	Point* pptLastMoveTo = NULL;

	HPEN hOldPen = (HPEN)SelectObject(m_hDC, m_hActivePen);

	// for each of the points we have...
	for (nIndex = 0; nIndex <nCount; nIndex++)
	{
		switch(pTypes[nIndex])
		{
			// React to information from the path by drawing the data
			// we received. For each of the points, record our own
			// "last active point" so we can close figures, lines, and
			// Beziers.

		case PathPointTypeStart:
			if (pptLastMoveTo != NULL && nIndex > 0)
				pDC->LineTo(pptLastMoveTo->X, pptLastMoveTo->Y);
			pDC->MoveTo(lppt[nIndex].X, lppt[nIndex].Y);
			pptLastMoveTo = &lppt[nIndex];
			break;

		case PathPointTypeLine | PathPointTypeCloseSubpath:
			pDC->LineTo(lppt[nIndex].X, lppt[nIndex].Y);
			if (pptLastMoveTo != NULL)
				pDC->LineTo(pptLastMoveTo->X, pptLastMoveTo->Y);
			pptLastMoveTo = NULL;
			break;

		case PathPointTypeLine:
			pDC->LineTo(lppt[nIndex].X, lppt[nIndex].Y);
			break;

		case PathPointTypeBezier | PathPointTypeCloseSubpath:
			ASSERT(nIndex + 2 <= nCount);
			pDC->PolyBezierTo((POINT*)&lppt[nIndex], 3);
			nIndex += 2;
			if (pptLastMoveTo != NULL)
				pDC->LineTo(pptLastMoveTo->X, pptLastMoveTo->Y);
			pptLastMoveTo = NULL;
			break;

		case PathPointTypeBezier:
			ASSERT(nIndex + 2 <= nCount);
			pDC->PolyBezierTo((POINT*)&lppt[nIndex], 3);
			nIndex += 2;
			break;
		}
	}

	delete[] lppt;
	delete[] pTypes;

	SelectObject(m_hDC, hOldPen);
}

void CXTPFlowGraphDrawContext::Ellipse(LPCRECT lpRect)
{
	HBRUSH hOldBrush = 0;
	HPEN hOldPen = 0;


	if (m_hActiveBrush)
	{
		hOldBrush = (HBRUSH)SelectObject(m_hDC, m_hActiveBrush);
	}

	if (m_hActivePen)
	{
		hOldPen = (HPEN)SelectObject(m_hDC, m_hActivePen);
	}
	else
	{
		hOldPen = (HPEN)SelectObject(m_hDC, GetStockObject(NULL_PEN));
	}

	::Ellipse(m_hDC, lpRect->left, lpRect->top, lpRect->right, lpRect->bottom);

	if (hOldBrush)
	{
		SelectObject(m_hDC, hOldBrush);
	}

	if (hOldPen)
	{
		SelectObject(m_hDC, hOldPen);
	}
}

void CXTPFlowGraphDrawContext::Line(float x1, float y1, float x2, float y2)
{
	if (!m_hActivePen)
		return;

	HPEN hOldPen = 0;
	if (m_hActivePen)
	{
		hOldPen = (HPEN)SelectObject(m_hDC, m_hActivePen);
	}

	MoveToEx(m_hDC, (int)x1, (int)y1, 0);
	LineTo(m_hDC, (int)x2, (int)y2);

	if (hOldPen)
	{
		SelectObject(m_hDC, hOldPen);
	}
}

void CXTPFlowGraphDrawContext::SetBrush(COLORREF clr)
{
	if (clr == (COLORREF)-1)
		m_hActiveBrush = 0;
	else
		m_hActiveBrush = m_arrBrushes.GetHandle(clr);
}

void CXTPFlowGraphDrawContext::SetPen(COLORREF clrPen, int nWidth)
{
	if (m_hActivePen)
	{
		DeleteObject(m_hActivePen);
		m_hActivePen = 0;
	}

	if (nWidth)
	{
		m_hActivePen = ::CreatePen(PS_SOLID, nWidth, clrPen);
	}
}

void CXTPFlowGraphDrawContext::SetTextColor(COLORREF clr)
{
	::SetTextColor(m_hDC, clr);
}

void CXTPFlowGraphDrawContext::SetFont(LOGFONT* lf)
{
	if (!lf)
	{
		m_hActiveFont = 0;
	}
	else
	{
		m_hActiveFont = m_arrFonts.GetHandle(*lf);
	}
}

void CXTPFlowGraphDrawContext::DrawGrid(CRect rc, CPoint ptScrollOffset, double dZoomLevel, COLORREF clrGridColor)
{
	double gap = 15.0 * dZoomLevel;

	double x =  -ptScrollOffset.x * dZoomLevel;
	int cnt = int(x / gap);
	x -= cnt * gap;

	while (x < 0) x += gap;
	while (x > gap) x -= gap;


	for (x; x < rc.right; x += gap)
	{
		FillSolidRect((int)x, rc.top, 1, rc.Width(), clrGridColor);
	}

	double y =  -ptScrollOffset.y * dZoomLevel;
	cnt = int(y / gap);
	y -= cnt * gap;

	while (y < 0) y += gap;
	while (y > gap) y -= gap;

	for (y; y < rc.bottom; y += gap)
	{
		FillSolidRect(rc.left, (int)y, rc.Width(), 1,clrGridColor);
	}
}

void CXTPFlowGraphDrawContext::GradientFill(CRect rc, COLORREF clrFrom, COLORREF clrTo, BOOL bHoriz)
{
	XTPDrawHelpers()->GradientFill(CDC::FromHandle(m_hDC), rc, clrFrom, clrTo, bHoriz);
}

void CXTPFlowGraphDrawContext::DrawImage(CXTPFlowGraphImage* pImage, CRect rc)
{
	CXTPImageManagerIcon* pIcon = pImage->GetIcon();

	if (pIcon)
	{
		pIcon->Draw(CDC::FromHandle(m_hDC), rc.TopLeft(), rc.Size());
	}
}

void CXTPFlowGraphDrawContext::DrawSelectionRect(CRect rcSelectedArea)
{
	COLORREF clrSelectionFrame = GetSysColor(COLOR_HIGHLIGHT);
	COLORREF clrSelection = clrSelectionFrame | 0x88000000;

	FillSolidRect(rcSelectedArea, clrSelection);

	Draw3dRect(rcSelectedArea, clrSelectionFrame, clrSelectionFrame);
}

void CXTPFlowGraphDrawContext::FillPolygon(const POINT* pts, int nCount)
{
	HBRUSH hOldBrush = 0;
	HPEN hOldPen = 0;

	if (m_hActiveBrush)
	{
		hOldBrush = (HBRUSH)SelectObject(m_hDC, m_hActiveBrush);
	}

	if (m_hActivePen)
	{
		hOldPen = (HPEN)SelectObject(m_hDC, m_hActivePen);
	}
	else
	{
		hOldPen = (HPEN)SelectObject(m_hDC, GetStockObject(NULL_PEN));
	}

	::Polygon(m_hDC, pts, nCount);

	if (hOldBrush)
	{
		SelectObject(m_hDC, hOldBrush);
	}
	if (hOldPen)
	{
		SelectObject(m_hDC, hOldPen);
	}
}

#ifdef _XTP_DEMOMODE
void CXTPFlowGraphDrawContext::DrawWatermarkBackground(CRect rc)
{

	LOGFONT lf = {0};
	lf.lfHeight = LONG(rc.Width() * 80 / 1000);

	lf.lfWeight = FW_BOLD;
	STRCPY_S(lf.lfFaceName, LF_FACESIZE, _T("Myraid Pro"));

	CFont font;
	font.CreateFontIndirect(&lf);

	HFONT hOldFont = (HFONT)SelectObject(m_hDC, font.GetSafeHandle());


	SetTextColor(RGB(102, 102, 102));

	::SetBkMode(m_hDC, TRANSPARENT);
	::DrawText(m_hDC, _T("Codejock Chart Pro Trial"), -1, rc, DT_VCENTER | DT_CENTER | DT_SINGLELINE);

	SelectObject(m_hDC, hOldFont);

}
#endif
