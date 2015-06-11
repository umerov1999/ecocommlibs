// XTPFlowGraphDrawContextGdiPlus.cpp : implementation of the CXTPFlowGraphDrawContextGdiPlus class.
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

#include "stdafx.h"
#include <math.h>

#include <Common/ScrollBar/XTPScrollBase.h>
#include <Common/ScrollBar/XTPScrollBarCtrl.h>
#include <Common/ScrollBar/XTPScrollBarContainer.h>

#include "Common/XTPDrawHelpers.h"
#include "Common/XTPResourceManager.h"
#include "Common/XTPVC80Helpers.h"
#include "GraphicLibrary/GdiPlus/GdiPlus.h"

#include "Markup/XTPMarkupObject.h"
#include "Markup/XTPMarkupInputElement.h"
#include "Markup/XTPMarkupUIElement.h"
#include "Markup/XTPMarkupContext.h"
#include "Markup/XTPMarkupDrawingContext.h"

#include "XTPFlowGraphTools.h"
#include "XTPFlowGraphDrawContext.h"
#include "XTPFlowGraphDrawContextGdiPlus.h"
#include "XTPFlowGraphControl.h"
#include "XTPFlowGraphImage.h"

#pragma comment(lib, "GdiPlus.lib")

using namespace Gdiplus;
using namespace Gdiplus::DllExports;

//////////////////////////////////////////////////////////////////////////
// CXTPFlowGraphDrawContextGdiPlus

class CXTPFlowGraphDrawContextGdiPlus::CGdiPlus
{
public:
	CGdiPlus()
	{
		m_hModule = NULL;
		m_nGdiplusToken = NULL;
		m_nCount = 0;
	}

public:
	void Register(BOOL bInit);

public:
	HMODULE m_hModule;
	ULONG_PTR m_nGdiplusToken;
	int m_nCount;
};


void CXTPFlowGraphDrawContextGdiPlus::CGdiPlus::Register(BOOL bInit)
{
	static GdiplusStartupOutput gdiplusStartupOutput;
	if (bInit)
	{
		m_nCount++;

		if (m_nCount > 1)
			return;

		ASSERT(m_nGdiplusToken == 0 && m_hModule == 0);


		m_hModule = LoadLibrary(_T("GdiPlus.dll"));

		if (m_hModule)
		{
			GdiplusStartupInput gdiplusStartupInput;
			gdiplusStartupInput.SuppressBackgroundThread = TRUE;
			GdiplusStartup(&m_nGdiplusToken, &gdiplusStartupInput, &gdiplusStartupOutput);
			gdiplusStartupOutput.NotificationHook(&m_nGdiplusToken);
		}
	}
	else
	{
		m_nCount--;

		if (m_nCount != 0)
			return;

		if (m_hModule)
		{
			// Termination of background thread, which is causing the shutdown problem.
			gdiplusStartupOutput.NotificationUnhook(m_nGdiplusToken);
			GdiplusShutdown(m_nGdiplusToken);
			FreeLibrary(m_hModule);
		}

		m_hModule = NULL;
		m_nGdiplusToken = 0;
	}
}

void CXTPFlowGraphDrawContextGdiPlus::Register(BOOL bInit)
{
	static CGdiPlus GdiPlus;

	GdiPlus.Register(bInit);
}

CXTPFlowGraphDrawContextGdiPlus::CXTPFlowGraphDrawContextGdiPlus(HDC hDC, HDC hAttribDC, BOOL bPrinting)
	: CXTPFlowGraphDrawContext(hDC, hAttribDC, bPrinting)
{
	m_hDC = hDC;
	m_bPrinting = FALSE;

	if (hAttribDC && hAttribDC != hDC)
	{
		m_pGraphics = new Graphics(hDC, hAttribDC);
	}
	else
	{
		m_pGraphics = new Graphics(hDC);
	}
	m_bPrinting = bPrinting;


	m_pGraphics->SetPageUnit(UnitPixel);
	m_pGraphics->SetSmoothingMode(SmoothingModeHighQuality);
	m_pGraphics->SetPixelOffsetMode(PixelOffsetModeHalf);

#if 0
	SetSmoothingMode(0);
#endif

	m_pActiveBrush = NULL;
	m_pActivePen = NULL;
	m_pActiveTextBrush = NULL;
	m_pActiveFont = NULL;

	Rect rcScreenBounds;
	m_pGraphics->GetClipBounds(&rcScreenBounds);
	m_rcClipRect = CRect(rcScreenBounds.GetLeft(), rcScreenBounds.GetTop(), rcScreenBounds.GetRight(), rcScreenBounds.GetBottom());

	m_nBrushCount = 0;
}

void CXTPFlowGraphDrawContextGdiPlus::SetSmoothingMode(int nMode)
{
	if (nMode)
	{
		m_pGraphics->SetTextRenderingHint(TextRenderingHintSystemDefault);
		m_pGraphics->SetSmoothingMode(SmoothingModeHighQuality);
		m_pGraphics->SetPixelOffsetMode(PixelOffsetModeHalf);
	}
	else
	{
		m_pGraphics->SetTextRenderingHint(TextRenderingHintSingleBitPerPixelGridFit);
		m_pGraphics->SetSmoothingMode(SmoothingModeHighSpeed);
		m_pGraphics->SetPixelOffsetMode(PixelOffsetModeHighSpeed);
	}
}

CXTPFlowGraphDrawContextGdiPlus::~CXTPFlowGraphDrawContextGdiPlus()
{
	SAFE_DELETE(m_pActivePen);
	SAFE_DELETE(m_pActiveFont);

	for (int i = 0; i < m_nBrushCount; i++)
	{
		delete m_arrBrushes[i].pBrush;
	}
}

void CXTPFlowGraphDrawContextGdiPlus::SetWorldTransform(CPoint ptOffset, double dZoomLevel)
{
	m_pGraphics->ResetTransform();

	m_pGraphics->TranslateTransform((REAL)-ptOffset.x, (REAL)-ptOffset.y, MatrixOrderAppend);
	m_pGraphics->ScaleTransform((REAL)dZoomLevel, (REAL)dZoomLevel, MatrixOrderAppend);

	Rect rcScreenBounds;
	m_pGraphics->GetClipBounds(&rcScreenBounds);
	m_rcClipRect = CRect(rcScreenBounds.GetLeft(), rcScreenBounds.GetTop(), rcScreenBounds.GetRight(), rcScreenBounds.GetBottom());
}

void CXTPFlowGraphDrawContextGdiPlus::SetClipRect(CRect rc)
{
	Rect rcClip(rc.left, rc.top, rc.Width(), rc.Height());
	m_pGraphics->SetClip(rcClip);

	Rect rcScreenBounds;
	m_pGraphics->GetClipBounds(&rcScreenBounds);
	m_rcClipRect = CRect(rcScreenBounds.GetLeft(), rcScreenBounds.GetTop(), rcScreenBounds.GetRight(), rcScreenBounds.GetBottom());
}

CRect CXTPFlowGraphDrawContextGdiPlus::GetClipRect() const
{
	return m_rcClipRect;
}

DWORD CXTPFlowGraphDrawContextGdiPlus::ColorRefToARGB(COLORREF clr)
{
	DWORD dwArgb = RGB(GetBValue(clr), GetGValue(clr), GetRValue(clr));
	if (clr & 0xFF000000)
		dwArgb |= (clr & 0xFF000000);
	else
		dwArgb |= 0xFF000000;

	return dwArgb;
}

void CXTPFlowGraphDrawContextGdiPlus::Clear(COLORREF clr)
{
	m_pGraphics->Clear(ColorRefToARGB(clr));
}

void CXTPFlowGraphDrawContextGdiPlus::FillSolidRect(LPCRECT lpRect, COLORREF clr)
{
	FillSolidRect(lpRect->left, lpRect->top, lpRect->right - lpRect->left,
		lpRect->bottom - lpRect->top, clr);

}

Gdiplus::Brush* CXTPFlowGraphDrawContextGdiPlus::CreateBrush(COLORREF clr)
{
	int i;

	for (i = 0; i < m_nBrushCount; i++)
	{
		if (m_arrBrushes[i].clr == clr)
			return m_arrBrushes[i].pBrush;
	}

	if (m_nBrushCount == MAX_BRUSH)
	{
		delete m_arrBrushes[m_nBrushCount - 1].pBrush;
	}
	else if (m_nBrushCount)
	{
		m_arrBrushes[m_nBrushCount] = m_arrBrushes[m_nBrushCount - 1];
	}

	for (i = m_nBrushCount - 1; i >= 1; i--)
	{
		m_arrBrushes[i] = m_arrBrushes[i - 1];
	}

	if (m_nBrushCount != MAX_BRUSH)
		m_nBrushCount++;

	m_arrBrushes[0].pBrush = new SolidBrush(ColorRefToARGB(clr));
	m_arrBrushes[0].clr = clr;

	return m_arrBrushes[0].pBrush;
}

void CXTPFlowGraphDrawContextGdiPlus::FillSolidRect(int x, int y, int cx, int cy, COLORREF clr)
{
	if (cx < 0)
	{
		x += cx;
		cx = -cx;
	}

	if (cy < 0)
	{
		y += cy;
		cy = -cy;
	}

	Brush* pBrush = CreateBrush(clr);
	m_pGraphics->FillRectangle(pBrush, x, y, cx, cy);
}

void CXTPFlowGraphDrawContextGdiPlus::Draw3dRect(LPCRECT lpRect, COLORREF clrTopLeft, COLORREF clrBottomRight)
{
	Draw3dRect(lpRect->left, lpRect->top, lpRect->right - lpRect->left,
		lpRect->bottom - lpRect->top, clrTopLeft, clrBottomRight);

}

void CXTPFlowGraphDrawContextGdiPlus::Draw3dRect(int x, int y, int cx, int cy, COLORREF clrTopLeft, COLORREF clrBottomRight)
{
	FillSolidRect(x, y, cx - 1, 1, clrTopLeft);
	FillSolidRect(x, y, 1, cy - 1, clrTopLeft);
	FillSolidRect(x + cx, y, -1, cy, clrBottomRight);
	FillSolidRect(x, y + cy, cx, -1, clrBottomRight);
}


void CXTPFlowGraphDrawContextGdiPlus::DrawFrame(int x, int y, int cx, int cy, int sz, COLORREF clr)
{
	FillSolidRect(x, y, cx - sz, sz, clr);
	FillSolidRect(x, y, sz, cy - sz, clr);
	FillSolidRect(x + cx, y, -sz, cy, clr);
	FillSolidRect(x, y + cy, cx, -sz, clr);
}

CSize CXTPFlowGraphDrawContextGdiPlus::MeasureMarkupElement(CXTPMarkupUIElement* pMarkupUIElement)
{
	pMarkupUIElement->GetMarkupContext()->SetDefaultFont(&m_lfActiveFont);

	CXTPMarkupDrawingContext dc(pMarkupUIElement->GetMarkupContext(), m_pGraphics);
	pMarkupUIElement->Measure(&dc, CSize(INT_MAX, INT_MAX));

	CSize sz = pMarkupUIElement->GetDesiredSize();

	return sz;
}

void CXTPFlowGraphDrawContextGdiPlus::DrawMarkupElement(CXTPMarkupUIElement* pMarkupUIElement, LPCRECT lpRect, UINT /*uFormat*/)
{
//  pMarkupUIElement->GetMarkupContext()->SetDefaultFont(&m_lfActiveFont);
//  pMarkupUIElement->GetMarkupContext()->SetDefaultTextColor(m_clrTextColor);

	CXTPMarkupDrawingContext dc(pMarkupUIElement->GetMarkupContext(), m_pGraphics);
	pMarkupUIElement->Arrange(lpRect);
	pMarkupUIElement->Render(&dc);
}


CSize CXTPFlowGraphDrawContextGdiPlus::MeasureString(const CString& str)
{

	if (!m_pActiveFont)
		return CSize(0, 0);


	RectF boundingBox;
	RectF layoutRect(0, 0, 0.0f, 0.0f);

	if (str.GetLength() != 0)
	{
		m_pGraphics->MeasureString(XTP_CT2CW(str), -1, m_pActiveFont, layoutRect, StringFormat::GenericTypographic(), &boundingBox);
	}
	else
	{
		m_pGraphics->MeasureString(L" ", 1, m_pActiveFont, layoutRect, StringFormat::GenericTypographic(), &boundingBox);
	}

	return CSize((int)ceil(boundingBox.Width), (int)(boundingBox.Height));
}

void CXTPFlowGraphDrawContextGdiPlus::DrawText(LPCTSTR lpszText, LPCRECT lpRect, UINT uFormat)
{
	if (!m_pActiveFont)
		return;


	StringFormat stringFormat(StringFormat::GenericTypographic());
	stringFormat.SetLineAlignment(uFormat & DT_VCENTER ? StringAlignmentCenter: StringAlignmentNear);
	stringFormat.SetAlignment(uFormat & DT_CENTER ? StringAlignmentCenter : uFormat & DT_RIGHT ? StringAlignmentFar : StringAlignmentNear);

	RectF rect((REAL)lpRect->left, (REAL)lpRect->top, (REAL)lpRect->right - lpRect->left, (REAL)lpRect->bottom - lpRect->top);

	m_pGraphics->DrawString(XTP_CT2CW(lpszText), -1, m_pActiveFont, rect, &stringFormat, m_pActiveTextBrush);
}

void CXTPFlowGraphDrawContextGdiPlus::DrawCurve(Gdiplus::GraphicsPath* pPath)
{
	if (m_pActivePen)
	{
		m_pGraphics->DrawPath(m_pActivePen, pPath);
	}
}

void CXTPFlowGraphDrawContextGdiPlus::Ellipse(LPCRECT lpRect)
{
	if (m_pActiveBrush)
	{
		m_pGraphics->FillEllipse(m_pActiveBrush, lpRect->left, lpRect->top, lpRect->right - lpRect->left, lpRect->bottom - lpRect->top);
	}

	if (m_pActivePen)
	{
		m_pGraphics->DrawEllipse(m_pActivePen, lpRect->left, lpRect->top, lpRect->right - lpRect->left, lpRect->bottom - lpRect->top);
	}
}

void CXTPFlowGraphDrawContextGdiPlus::Line(float x1, float y1, float x2, float y2)
{
	if (m_pActivePen)
	{
		m_pGraphics->DrawLine(m_pActivePen, x1, y1, x2, y2);
	}
}

void CXTPFlowGraphDrawContextGdiPlus::SetBrush(COLORREF clr)
{
	m_pActiveBrush = CreateBrush(clr);
}

void CXTPFlowGraphDrawContextGdiPlus::SetPen(COLORREF clrPen, int nWidth, BOOL bDashed /*= FALSE*/)
{
	SAFE_DELETE(m_pActivePen);

	if (nWidth)
	{
		m_pActivePen = new Pen(ColorRefToARGB(clrPen), (REAL)nWidth);

		m_pActivePen->SetDashStyle(bDashed ? DashStyleDash : DashStyleSolid);
	}
}

void CXTPFlowGraphDrawContextGdiPlus::SetTextColor(COLORREF clr)
{
	m_pActiveTextBrush = CreateBrush(clr);
	m_clrTextColor = clr;
}

void CXTPFlowGraphDrawContextGdiPlus::SetFont(LOGFONT* lf)
{
	SAFE_DELETE(m_pActiveFont);
	if (!lf)
		return;

	m_pActiveFont = new Gdiplus::Font(m_hDC, lf);
	m_lfActiveFont = *lf;
}

void CXTPFlowGraphDrawContextGdiPlus::DrawGrid(CRect rc, CPoint ptScrollOffset, double dZoomLevel, COLORREF clrGridColor)
{
	double gap = 15.0 * dZoomLevel;

	double x =  -ptScrollOffset.x * dZoomLevel;
	int cnt = int(x / gap);
	x -= cnt * gap;

	while (x < 0) x += gap;
	while (x > gap) x -= gap;


	SmoothingMode nSmoothingMode= m_pGraphics->GetSmoothingMode();

	m_pGraphics->SetSmoothingMode(SmoothingModeHighSpeed);


	Brush* pBrush = CreateBrush(clrGridColor);

	for (x; x < rc.right; x += gap)
	{
		m_pGraphics->FillRectangle(pBrush, (int)x, rc.top, 1, rc.Height());
	}

	double y =  -ptScrollOffset.y * dZoomLevel;
	cnt = int(y / gap);
	y -= cnt * gap;

	while (y < 0) y += gap;
	while (y > gap) y -= gap;

	for (y; y < rc.bottom; y += gap)
	{
		m_pGraphics->FillRectangle(pBrush, rc.left, (int)y, rc.Width(), 1);
	}

	m_pGraphics->SetSmoothingMode(nSmoothingMode);
}

void CXTPFlowGraphDrawContextGdiPlus::GradientFill(CRect rc, COLORREF clrFrom, COLORREF clrTo, BOOL bHoriz)
{
	LinearGradientBrush brush(Rect(rc.left, rc.top, rc.Width(), rc.Height()),
		ColorRefToARGB(clrFrom), ColorRefToARGB(clrTo), bHoriz ? LinearGradientModeHorizontal : LinearGradientModeVertical);

	m_pGraphics->FillRectangle(&brush, rc.left, rc.top, rc.Width(), rc.Height());
}

void CXTPFlowGraphDrawContextGdiPlus::DrawImage(CXTPFlowGraphImage* pImage, CRect rc)
{
	m_pGraphics->DrawImage(pImage->GetImage(), rc.left, rc.top, rc.Width(), rc.Height());
}

void CXTPFlowGraphDrawContextGdiPlus::FillPolygon(const POINT* pts, int nCount)
{
	m_pGraphics->FillPolygon(m_pActiveBrush, (Point*)pts, nCount);

}


#ifdef _XTP_DEMOMODE

void CXTPFlowGraphDrawContextGdiPlus::DrawWatermarkBackground(CRect rc)
{
	m_pGraphics->SetTextRenderingHint(TextRenderingHintAntiAlias);

	SolidBrush brush(Color(100, 102, 102, 102));


	LOGFONT lf = {0};
	lf.lfHeight = LONG(rc.Width() * 80 / 1000);

	lf.lfWeight = FW_BOLD;
	STRCPY_S(lf.lfFaceName, LF_FACESIZE, _T("Myraid Pro"));

	Font font(m_hDC, &lf);


	StringFormat stringFormat;

	stringFormat.SetLineAlignment(StringAlignmentCenter);
	stringFormat.SetAlignment(StringAlignmentCenter);

	RectF rcRect((float)rc.left, (float)rc.top, (float)rc.Width(), (float)rc.Height());
	m_pGraphics->DrawString(L"Codejock Chart Pro Trial", -1, &font, rcRect, &stringFormat, &brush);

}
#endif
