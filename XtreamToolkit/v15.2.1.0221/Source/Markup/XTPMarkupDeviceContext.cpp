// XTPMarkupDrawingContext.cpp: implementation of the CXTPMarkupDrawingContext class.
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

#include "Common/XTPImageManager.h"

#include "XTPMarkupObject.h"
#include "XTPMarkupInputElement.h"
#include "XTPMarkupUIElement.h"
#include "XTPMarkupFrameworkElement.h"
#include "XTPMarkupDrawingContext.h"
#include "XTPMarkupDeviceContext.h"
#include "XTPMarkupShape.h"
#include "XTPMarkupThickness.h"
#include <Markup/Path/XTPMarkupPathData.h>

#include "GraphicLibrary/GdiPlus/GdiPlus.h"
using namespace Gdiplus;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CXTPMarkupDeviceContext::CXTPMarkupDeviceContext(HDC hDC, HDC hAttribDC)
{
	m_hDC = hDC;
	m_hAttribDC = hAttribDC;

	if (m_hDC)
	{
		::GetWindowExtEx(m_hDC, &m_sizeWinExt);
		::GetViewportExtEx(m_hDC, &m_sizeVpExt);
	}
	else
	{
		m_sizeWinExt = m_sizeVpExt = CSize(1, 1);
	}

	if ((m_sizeWinExt.cx == 0) || (m_sizeWinExt.cy == 0))
	{
		m_sizeVpExt = m_sizeWinExt = CSize(1, 1);
	}
	m_sizeWinExt.cx = abs(m_sizeWinExt.cx);
	m_sizeWinExt.cy = abs(m_sizeWinExt.cy);
	m_sizeVpExt.cx = abs(m_sizeVpExt.cx);
	m_sizeVpExt.cy = abs(m_sizeVpExt.cy);

	m_hOldFont = NULL;
}

CXTPMarkupDeviceContext::~CXTPMarkupDeviceContext()
{

}

void CXTPMarkupDeviceContext::CreatePen(CPen& penStroke, CXTPMarkupStrokeStyle* pStrokeStyle)
{
	if (pStrokeStyle && pStrokeStyle->nStrokeThickness > 0 && pStrokeStyle->pStrokeBrush)
		penStroke.CreatePen(PS_SOLID, pStrokeStyle->nStrokeThickness, pStrokeStyle->pStrokeBrush->GetHintColor());
}

void CXTPMarkupDeviceContext::Ellipse(CRect rc, CXTPMarkupStrokeStyle* pStrokeStyle, CXTPMarkupBrush* pFillBrush)
{
	CPen penStroke;
	CreatePen(penStroke, pStrokeStyle);

	CBrush brushFill;
	if (pFillBrush) brushFill.CreateSolidBrush(pFillBrush->GetHintColor());

	HGDIOBJ hOldPen = penStroke.GetSafeHandle() ? ::SelectObject(GetSafeHdc(), penStroke) : ::SelectObject(GetSafeHdc(), ::GetStockObject(NULL_PEN));
	HGDIOBJ hOldBrush = brushFill.GetSafeHandle() ? ::SelectObject(GetSafeHdc(), brushFill) : ::SelectObject(GetSafeHdc(), ::GetStockObject(NULL_BRUSH));

	::Ellipse(GetSafeHdc(), rc.left, rc.top, rc.right, rc.bottom);

	if (hOldPen) ::SelectObject(GetSafeHdc(), hOldPen);
	if (hOldBrush) ::SelectObject(GetSafeHdc(), hOldBrush);
}

void CXTPMarkupDeviceContext::Polyline(const POINT* points, int nCount, CXTPMarkupStrokeStyle* pStrokeStyle)
{
	CPen penStroke;
	CreatePen(penStroke, pStrokeStyle);

	HGDIOBJ hOldPen = penStroke.GetSafeHandle() ? ::SelectObject(GetSafeHdc(), penStroke) : ::SelectObject(GetSafeHdc(), ::GetStockObject(NULL_PEN));

	::Polyline(GetSafeHdc(), points, nCount);

	if (hOldPen) ::SelectObject(GetSafeHdc(), hOldPen);

}

void CXTPMarkupDeviceContext::Polygon(const POINT* points, int nCount, CXTPMarkupStrokeStyle* pStrokeStyle, CXTPMarkupBrush* pFillBrush)
{
	CPen penStroke;
	CreatePen(penStroke, pStrokeStyle);

	CBrush brushFill;
	if (pFillBrush) brushFill.CreateSolidBrush(pFillBrush->GetHintColor());

	HGDIOBJ hOldPen = penStroke.GetSafeHandle() ? ::SelectObject(GetSafeHdc(), penStroke) : ::SelectObject(GetSafeHdc(), ::GetStockObject(NULL_PEN));
	HGDIOBJ hOldBrush = brushFill.GetSafeHandle() ? ::SelectObject(GetSafeHdc(), brushFill) : ::SelectObject(GetSafeHdc(), ::GetStockObject(NULL_BRUSH));

	::Polygon(GetSafeHdc(), points, (int)nCount);

	if (hOldPen) ::SelectObject(GetSafeHdc(), hOldPen);
	if (hOldBrush) ::SelectObject(GetSafeHdc(), hOldBrush);
}

void CXTPMarkupDeviceContext::DrawPath(CXTPMarkupPathData *pData, CXTPMarkupStrokeStyle* pStrokeStyle, CXTPMarkupBrush* pFillBrush)
{
	int nCount = pData->GetCoint();
	if (nCount == 0)
		return;

	::BeginPath(m_hDC);

	const MARKUP_POINTF* pPoints = pData->GetPoints();
	const BYTE* pTypes = pData->GetTypes();

	for (int i = 0; i < nCount; i++)
	{
		switch (pTypes[i] & 7)
		{
		case xtpMarkupPathPointTypeStart:
			::MoveToEx(m_hDC, (int)pPoints[i].x, (int)pPoints[i].y, NULL);
			break;

		case xtpMarkupPathPointTypeLine:
			::LineTo(m_hDC, (int)pPoints[i].x, (int)pPoints[i].y);
			break;

		case xtpMarkupPathPointTypeBezier:
			POINT pt[] = {(int)pPoints[i].x, (int)pPoints[i].y, (int)pPoints[i + 1].x, (int)pPoints[i + 1].y, (int)pPoints[i + 2].x, (int)pPoints[i + 2].y};
			::PolyBezierTo(m_hDC, pt, 3);
			i += 2;
			break;
		}
		if (pTypes[i] & xtpMarkupPathPointTypeCloseSubpath)
			::CloseFigure(m_hDC);
	}

	::EndPath(m_hDC);

	CBrush brushFill;
	CPen penStroke;
	HGDIOBJ hOldBrush = NULL;
	HGDIOBJ hOldPen = NULL;

	if (pFillBrush)
	{
		brushFill.CreateSolidBrush(pFillBrush->GetHintColor());
		hOldBrush = brushFill.GetSafeHandle() ? ::SelectObject(GetSafeHdc(), brushFill) : ::SelectObject(GetSafeHdc(), ::GetStockObject(NULL_BRUSH));
	}

	CreatePen(penStroke, pStrokeStyle);
	if (penStroke.GetSafeHandle())
	{
		hOldPen = penStroke.GetSafeHandle() ? ::SelectObject(GetSafeHdc(), penStroke) : ::SelectObject(GetSafeHdc(), ::GetStockObject(NULL_PEN));

	}
	if (brushFill.GetSafeHandle() && penStroke.GetSafeHandle())
	{
		::StrokeAndFillPath(m_hDC);
	}
	else if (brushFill.GetSafeHandle())
	{
		::FillPath(m_hDC);

	}
	else if (penStroke.GetSafeHandle())
	{
		::StrokePath(m_hDC);
	}

	if (hOldBrush) ::SelectObject(GetSafeHdc(), hOldBrush);
	if (hOldPen) ::SelectObject(GetSafeHdc(), hOldPen);

}

POINT CXTPMarkupDeviceContext::TranslatePoint(const POINT& ptVisualOffset) const
{
	POINT pt;
	pt.x = ptVisualOffset.x * m_sizeVpExt.cx / m_sizeWinExt.cx;
	pt.y = ptVisualOffset.y * m_sizeVpExt.cy / m_sizeWinExt.cy;
	return pt;
}

void CXTPMarkupDeviceContext::OffsetViewport(const POINT& ptVisualOffset)
{
	POINT pt = TranslatePoint(ptVisualOffset);

	OffsetViewportOrgEx(m_hDC, pt.x, pt.y, NULL);
}


void CXTPMarkupDeviceContext::DrawRectangle(CRect rc, CXTPMarkupBrush* pBrush, CXTPMarkupThickness* pThickness)
{
	if (!pThickness)
		return;

	if (pThickness->GetLeft() > 0)
		FillRectangle(CRect(rc.left, rc.top, rc.left + pThickness->GetLeft(), rc.bottom), pBrush);

	if (pThickness->GetRight() > 0)
		FillRectangle(CRect(rc.right - pThickness->GetRight(), rc.top, rc.right, rc.bottom), pBrush);

	if (pThickness->GetTop() > 0)
		FillRectangle(CRect(rc.left, rc.top, rc.right, rc.top + pThickness->GetTop()), pBrush);

	if (pThickness->GetBottom() > 0)
		FillRectangle(CRect(rc.left, rc.bottom - pThickness->GetBottom(), rc.right, rc.bottom), pBrush);
}

int _cdecl CXTPMarkupDeviceContext::_GradientStopCompare(const void *arg1, const void *arg2)
{
	double& dOffset1 = ((GRADIENTSTOP*)arg1)->dOffset;
	double& dOffset2 = ((GRADIENTSTOP*)arg2)->dOffset;

	return dOffset1 > dOffset2 ? 1 : dOffset1 < dOffset2 ? -1 : ((GRADIENTSTOP*)arg1)->nIndex - ((GRADIENTSTOP*)arg2)->nIndex;
};


void CXTPMarkupDeviceContext::FillRoundRectangle(CRect rc, CXTPMarkupBrush* pBrush, double* pCornerRadius)
{
	if (!pCornerRadius)
	{
		FillRectangle(rc, pBrush);
		return;
	}

	float l = (float)rc.left, r = (float)rc.right, t = (float)rc.top, b = (float)rc.bottom;

	CXTPMarkupPathGeometryBuilder builder;

	builder.BeginFigure(l, t + (float)pCornerRadius[0]);
	builder.BezierTo(l, t, l + (float)pCornerRadius[1], t);

	builder.LineTo(r - (float)pCornerRadius[2], t);
	builder.BezierTo(r, t, r, t + (float)pCornerRadius[3]);

	builder.LineTo(r, b - (float)pCornerRadius[4]);
	builder.BezierTo(r, b, r - (float)pCornerRadius[5], b);

	builder.LineTo(l + (float)pCornerRadius[6], b);
	builder.BezierTo(l, b, l, b - (float)pCornerRadius[7]);

	builder.CloseFigure();

	CXTPMarkupPathData* pData = builder.CreateData();
	pData->m_nPixelOffsetMode = 4;

	CXTPMarkupStrokeStyle style;
	memset(&style, 0, sizeof(style));

	DrawPath(pData, &style, pBrush);

	delete pData;
}

void CXTPMarkupDeviceContext::DrawRoundRectangle(CRect rc, CXTPMarkupBrush* pBrush, CXTPMarkupThickness* pThickness, double* pCornerRadius)
{
	if (!pCornerRadius)
	{
		DrawRectangle(rc, pBrush, pThickness);
		return;
	}
	int nStroke = pThickness->GetLeft();


	float f = float(nStroke) / 2;

	float l = (float)rc.left + f, r = (float)rc.right - f, t = (float)rc.top + f, b = (float)rc.bottom - f;

	CXTPMarkupPathGeometryBuilder builder;

	builder.BeginFigure(l, t + (float)pCornerRadius[0]);
	builder.BezierTo(l, t, l + (float)pCornerRadius[1], t);

	builder.LineTo(r - (float)pCornerRadius[2], t);
	builder.BezierTo(r, t, r, t + (float)pCornerRadius[3]);

	builder.LineTo(r, b - (float)pCornerRadius[4]);
	builder.BezierTo(r, b, r - (float)pCornerRadius[5], b);

	builder.LineTo(l + (float)pCornerRadius[6], b);
	builder.BezierTo(l, b, l, b - (float)pCornerRadius[7]);

	builder.CloseFigure();

	CXTPMarkupPathData* pData = builder.CreateData();
	pData->m_nPixelOffsetMode = 4;

	CXTPMarkupStrokeStyle style;
	memset(&style, 0, sizeof(style));
	style.nStrokeThickness = nStroke;
	style.pStrokeBrush = pBrush;

	DrawPath(pData, &style, NULL);

	delete pData;
}

void CXTPMarkupDeviceContext::FillRectangle(CRect rc, CXTPMarkupBrush* pBrush)
{
	if (IsSolidBrush(pBrush))
	{
		CXTPMarkupSolidColorBrush* pSolidColorBrush = (CXTPMarkupSolidColorBrush*)pBrush;

		CXTPMarkupColor* pColor = pSolidColorBrush->GetColor();

		if (pColor)
		{
			COLORREF clrOld = ::SetBkColor(GetSafeHdc(), (COLORREF)(*pColor) & 0xFFFFFF);
			::ExtTextOut(GetSafeHdc(), 0, 0, ETO_OPAQUE, rc, NULL, 0, NULL);
			::SetBkColor(GetSafeHdc(), clrOld);
		}
		return;
	}

	if (IsLinearGradientBrush(pBrush))
	{
		CXTPMarkupLinearGradientBrush* pLinearGradientBrush = (CXTPMarkupLinearGradientBrush*)pBrush;

		HDC hDC = GetSafeHdc();
		int cx = rc.Width();
		int cy = rc.Height();

		CXTPMarkupGradientStops* pGradientStops = pLinearGradientBrush->GetGradientStops();
		if (pGradientStops->GetCount() == 0)
			return;

		if (cx <= 0 || cy <= 0)
			return;

		if (pGradientStops->GetCount() == 1)
		{
			COLORREF clr = pGradientStops->GetItem(0)->GetColor() & 0xFFFFFF;
			COLORREF clrOld = ::SetBkColor(hDC, clr);
			::ExtTextOut(hDC, 0, 0, ETO_OPAQUE, rc, NULL, 0, NULL);
			::SetBkColor(hDC, clrOld);
			return;
		}

		CXTPMarkupPoint ptStartPoint(0, 0);
		CXTPMarkupPoint* pt = MARKUP_STATICCAST(CXTPMarkupPoint, pBrush->GetValue(CXTPMarkupLinearGradientBrush::m_pStartPointProperty));
		if (pt) ptStartPoint = *pt;

		CXTPMarkupPoint ptEndPoint(1, 1);
		pt = MARKUP_STATICCAST(CXTPMarkupPoint, pBrush->GetValue(CXTPMarkupLinearGradientBrush::m_pEndPointProperty));
		if (pt) ptEndPoint = *pt;

		ptStartPoint.x *= cx;
		ptStartPoint.y *= cy;

		ptEndPoint.x *= cx;
		ptEndPoint.y *= cy;

		BOOL bHorizontal = ptStartPoint.y == ptEndPoint.y;
		BOOL bVertical = ptStartPoint.x == ptEndPoint.x;

		if (bHorizontal && bVertical)
			return;

		CRect rcClipBox;
		::GetClipBox(m_hDC, rcClipBox);

		//if (pDC->IsPrinting())
		//  rcClipBox.InflateRect(1, 1);

		rcClipBox.OffsetRect(-rc.TopLeft());

		if (!rcClipBox.IntersectRect(&rcClipBox, CRect(0, 0, cx, cy)))
			return;

		LPDWORD lpBits;
		HBITMAP hBitmap = CXTPImageManager::Create32BPPDIBSection(0, cx, cy, (LPBYTE*)&lpBits);


		int nCount = pGradientStops->GetCount();
		GRADIENTSTOP* pStops = new GRADIENTSTOP[nCount];

		for (int i = 0; i < nCount; i++)
		{
			CXTPMarkupGradientStop* pItem = pGradientStops->GetItem(i);

			COLORREF clr = pItem->GetColor();
			pStops[i].clr.rgbRed = GetRValue(clr);
			pStops[i].clr.rgbGreen = GetGValue(clr);
			pStops[i].clr.rgbBlue = GetBValue(clr);
			pStops[i].clr.rgbReserved = 0;
			pStops[i].dOffset = pItem->GetOffset();
			pStops[i].nIndex = i;
		}

		GRADIENTSTOP *pStopFirst = pStops, *pStopLast = pStops + nCount - 1, *pStop;

		qsort(pStopFirst, nCount, sizeof(GRADIENTSTOP), _GradientStopCompare);

		for (pStop = pStopFirst + 1; pStop <= pStopLast; pStop++)
		{
			pStop->dDiff = pStop->dOffset - (pStop - 1)->dOffset;
		}

		CXTPMarkupPoint ptCenter((double)cx / 2, (double)cy /2);

		double len = sqrt(pow(ptStartPoint.x - ptEndPoint.x, 2) + pow(ptStartPoint.y - ptEndPoint.y, 2));
		double cosA = (ptEndPoint.x - ptStartPoint.x) / len;
		double sinA = (ptEndPoint.y - ptStartPoint.y) / len;

		double dDiff = - ptCenter.x * cosA  - ptCenter.y * sinA + ptCenter.x;
		double dStart = ptStartPoint.x * cosA + ptStartPoint.y * sinA + dDiff;
		double dEnd = ptEndPoint.x * cosA + ptEndPoint.y * sinA + dDiff;

		LPDWORD lpPixel = lpBits;

		double fDist = 1.0 / (dEnd - dStart);
		cosA = cosA * fDist;
		sinA = sinA * fDist;
		dDiff = (dDiff - dStart) * fDist;

		if (bVertical)
		{
			lpPixel += (cy - rcClipBox.bottom) * cx;

			for (int y = (cy - rcClipBox.bottom); y < (cy - rcClipBox.top); y++)
			{
				double fAmount = double(cy - y) * sinA + dDiff;

				DWORD dwPixel = 0;

				if (fAmount <= pStopFirst->dOffset)
				{
					dwPixel =  *((LPDWORD)&pStopFirst->clr);
				}
				else if (fAmount >= pStopLast->dOffset)
				{
					dwPixel =  *((LPDWORD)&pStopLast->clr);
				}
				else for (pStop = pStopFirst + 1; pStop <= pStopLast; pStop++)
				{
					if (fAmount < pStop->dOffset && pStop->dDiff != 0)
					{
						const RGBQUAD& clrFrom = (pStop - 1)->clr;
						const RGBQUAD& clrTo = pStop->clr;

						double fAmountA = (pStop->dOffset - fAmount) / pStop->dDiff;
						double fAmountB = (1.0 - fAmountA);

						((LPBYTE)&dwPixel)[2] = (BYTE)(clrFrom.rgbRed * fAmountA + clrTo.rgbRed * fAmountB);
						((LPBYTE)&dwPixel)[1] = (BYTE)(clrFrom.rgbGreen * fAmountA + clrTo.rgbGreen * fAmountB);
						((LPBYTE)&dwPixel)[0] = (BYTE)(clrFrom.rgbBlue * fAmountA + clrTo.rgbBlue * fAmountB);
						break;
					}
				}

				for (int x = 0; x < cx; x++)
				{
					*lpPixel = dwPixel;
					lpPixel++;
				}
			}

		}
		else if (bHorizontal)
		{
			dDiff += double(cy) * sinA;

			lpPixel += rcClipBox.left;

			for (int x = rcClipBox.left; x < rcClipBox.right; x++)
			{
				double fAmount = (double)x * cosA + dDiff;

				if (fAmount <= pStopFirst->dOffset)
				{
					*lpPixel =  *((LPDWORD)&pStopFirst->clr);
				}
				else if (fAmount >= pStopLast->dOffset)
				{
					*lpPixel =  *((LPDWORD)&pStopLast->clr);
				}
				else for (pStop = pStopFirst + 1; pStop <= pStopLast; pStop++)
				{
					if (fAmount < pStop->dOffset && pStop->dDiff != 0)
					{
						const RGBQUAD& clrFrom = (pStop - 1)->clr;
						const RGBQUAD& clrTo = pStop->clr;

						double fAmountA = (pStop->dOffset - fAmount) / pStop->dDiff;
						double fAmountB = (1.0 - fAmountA);

						((LPBYTE)lpPixel)[2] = (BYTE)(clrFrom.rgbRed * fAmountA + clrTo.rgbRed * fAmountB);
						((LPBYTE)lpPixel)[1] = (BYTE)(clrFrom.rgbGreen * fAmountA + clrTo.rgbGreen * fAmountB);
						((LPBYTE)lpPixel)[0] = (BYTE)(clrFrom.rgbBlue * fAmountA + clrTo.rgbBlue * fAmountB);
						break;
					}
				}

				lpPixel++;
			}

			lpPixel += (cx - rcClipBox.right);

			int y = max(1, cy - rcClipBox.bottom);

			if (y > 1)
			{
				lpPixel += cx * (cy - rcClipBox.bottom - 1);
			}

			for (; y < (cy - rcClipBox.top); y++)
			{
				memcpy(lpPixel, lpBits, sizeof(DWORD) * cx);
				lpPixel += cx;
			}
		}
		else
		{
			lpPixel += (cy - rcClipBox.bottom) * cx;

			for (int y = (cy - rcClipBox.bottom); y < (cy - rcClipBox.top); y++)
			{
				lpPixel += rcClipBox.left;

				for (int x = rcClipBox.left; x < rcClipBox.right; x++)
				{
					double fAmount = (double)x * cosA + double(cy - y) * sinA + dDiff;

					if (fAmount <= pStopFirst->dOffset)
					{
						*lpPixel =  *((LPDWORD)&pStopFirst->clr);
					}
					else if (fAmount >= pStopLast->dOffset)
					{
						*lpPixel =  *((LPDWORD)&pStopLast->clr);
					}
					else for (pStop = pStopFirst + 1; pStop <= pStopLast; pStop++)
					{
						if (fAmount < pStop->dOffset && pStop->dDiff != 0)
						{
							const RGBQUAD& clrFrom = (pStop - 1)->clr;
							const RGBQUAD& clrTo = pStop->clr;

							double fAmountA = (pStop->dOffset - fAmount) / pStop->dDiff;
							double fAmountB = (1.0 - fAmountA);

							((LPBYTE)lpPixel)[2] = (BYTE)(clrFrom.rgbRed * fAmountA + clrTo.rgbRed * fAmountB);
							((LPBYTE)lpPixel)[1] = (BYTE)(clrFrom.rgbGreen * fAmountA + clrTo.rgbGreen * fAmountB);
							((LPBYTE)lpPixel)[0] = (BYTE)(clrFrom.rgbBlue * fAmountA + clrTo.rgbBlue * fAmountB);
							break;
						}
					}

					lpPixel++;
				}

				lpPixel += (cx - rcClipBox.right);
			}
		}

		HDC hBitmapDC = ::CreateCompatibleDC(hDC);

		HBITMAP hOldBitmap = (HBITMAP)SelectObject(hBitmapDC, hBitmap);

		BitBlt(hDC, rc.left + rcClipBox.left, rc.top + rcClipBox.top, rcClipBox.Width(), rcClipBox.Height(), hBitmapDC, rcClipBox.left, rcClipBox.top, SRCCOPY);

		SelectObject(hBitmapDC, hOldBitmap);
		DeleteDC(hBitmapDC);

		DeleteObject(hBitmap);
		delete[] pStops;
	}
}

void CXTPMarkupDeviceContext::DrawLine(int x1, int y1, int x2, int y2, CXTPMarkupStrokeStyle* pStrokeStyle)
{
	CPen penStroke;
	CreatePen(penStroke, pStrokeStyle);

	HGDIOBJ hOldPen = penStroke.GetSafeHandle() ? ::SelectObject(m_hDC, penStroke) : ::SelectObject(m_hDC, ::GetStockObject(NULL_PEN));

	::MoveToEx(m_hDC, x1, y1, NULL);
	::LineTo(m_hDC, x2, y2);

	if (hOldPen) ::SelectObject(m_hDC, hOldPen);
}

SIZE CXTPMarkupDeviceContext::MeasureString(LPCWSTR lpszText, int nCount) const
{
	SIZE sz;
	VERIFY(::GetTextExtentPoint32W(m_hAttribDC, lpszText, nCount, &sz));
	return sz;
}

void CXTPMarkupDeviceContext::SetTextColor(CXTPMarkupBrush* pBrush)
{
	::SetTextColor(m_hDC, pBrush ? pBrush->GetHintColor() : 0);
}

void CXTPMarkupDeviceContext::SetTextFont(CXTPMarkupFont* pFont)
{
	if (!m_hDC)
		return;

	if (pFont == NULL && m_hOldFont)
	{
		SelectFontObject(m_hOldFont);
		m_hOldFont = NULL;
	}
	else if (pFont != NULL && m_hOldFont == NULL)
	{
		m_hOldFont = SelectFontObject(pFont->m_hFont);
	}
	else if (pFont != NULL)
	{
		SelectFontObject(pFont->m_hFont);
	}
}

HFONT CXTPMarkupDeviceContext::SelectFontObject(HFONT hFont)
{
	HFONT hOldObj = 0;

	if (m_hDC != m_hAttribDC)
		hOldObj = (HFONT)::SelectObject(m_hDC, hFont);

	if (m_hAttribDC != NULL)
		hOldObj = (HFONT)::SelectObject(m_hAttribDC, hFont);

	return hOldObj;
}

void CXTPMarkupDeviceContext::DrawString(LPCWSTR lpszString, UINT nCount, LPCRECT lpRect)
{
	if (m_hAttribDC == m_hDC)
	{
		::ExtTextOutW(m_hDC, lpRect->left, lpRect->top, 0, lpRect, lpszString, nCount, 0);
		return;
	}

	LPINT lpDxWidths = NULL;

	ASSERT(m_hDC != NULL);
	ASSERT(m_hAttribDC != NULL);
	ASSERT(lpszString != NULL);
	ASSERT(lpDxWidths == NULL ||
			AfxIsValidAddress(lpDxWidths, sizeof(int) * nCount, FALSE));
	ASSERT(AfxIsValidAddress(lpszString, nCount, FALSE));

	int* pDeltas = NULL;
	LPWSTR pOutputString = NULL;
	int nRightFixup = 0;

	if (nCount == 0)    // Do nothing
		return;

	pDeltas = new int[nCount];
	pOutputString = new WCHAR[nCount];

	int x = lpRect->left;

	ComputeDeltas(x, (LPWSTR)lpszString, nCount, FALSE, 0, NULL, 0,
		pOutputString, pDeltas, nRightFixup);

	lpDxWidths = pDeltas;
	lpszString = pOutputString;

	::ExtTextOutW(m_hDC, x, lpRect->top, 0, lpRect, lpszString, nCount, lpDxWidths);

	delete[] pDeltas;
	delete[] pOutputString;
}

int CXTPMarkupDeviceContext::ComputeNextTab(int x, UINT nTabStops, LPINT lpnTabStops,
										int nTabOrigin, int nTabWidth) const
{
	x -= nTabOrigin;        // normalize position to tab origin
	for (UINT i = 0; i < nTabStops; i++, lpnTabStops++)
	{
		if (*lpnTabStops > x)
			return *lpnTabStops + nTabOrigin;
	}
	return (x / nTabWidth + 1) * nTabWidth + nTabOrigin;
}

CSize CXTPMarkupDeviceContext::ComputeDeltas(int& x, LPCWSTR lpszString, UINT &nCount,
	BOOL bTabbed, UINT nTabStops, LPINT lpnTabStops, int nTabOrigin,
	LPWSTR lpszOutputString, int* pnDxWidths, int& nRightFixup) const
{
	TEXTMETRIC tmAttrib;
	TEXTMETRIC tmScreen;
	::GetTextMetrics(m_hAttribDC, &tmAttrib);
	::GetTextMetrics(m_hDC, &tmScreen);

	CSize sizeExtent;
	::GetTextExtentPoint32A(m_hAttribDC, "A", 1, &sizeExtent);

	CPoint ptCurrent;
	UINT nAlignment = ::GetTextAlign(m_hAttribDC);
	BOOL bUpdateCP = (nAlignment & TA_UPDATECP) != 0;
	if (bUpdateCP)
	{
		::GetCurrentPositionEx(m_hDC, &ptCurrent);
		x = ptCurrent.x;
	}

	LPCWSTR lpszCurChar = lpszString;
	LPCWSTR lpszStartRun = lpszString;
	int* pnCurDelta = pnDxWidths;
	int nStartRunPos = x;
	int nCurrentPos = x;
	int nStartOffset = 0;

	int nTabWidth = 0;
	if (bTabbed)
	{
		if (nTabStops == 1)
		{
			nTabWidth = lpnTabStops[0];
		}
		else
		{
			// Get default size of a tab
			nTabWidth = LOWORD(::GetTabbedTextExtentA(m_hAttribDC,
				"\t", 1, 0, NULL));
		}
	}

	for (UINT i = 0; i < nCount; i++)
	{
		BOOL bSpace = ((_TUCHAR)*lpszCurChar == (_TUCHAR)tmAttrib.tmBreakChar);
		if (bSpace || (bTabbed && *lpszCurChar == '\t'))
		{
			// bSpace will be either TRUE (==1) or FALSE (==0).  For spaces
			// we want the space included in the GetTextExtent, for tabs we
			// do not want the tab included
			int nRunLength = (int)(lpszCurChar - lpszStartRun) + (bSpace ? 1 : 0);

			CSize sizeExtent2;
			::GetTextExtentPoint32W(m_hAttribDC, lpszStartRun, nRunLength,
				&sizeExtent2);
			int nNewPos = nStartRunPos + sizeExtent2.cx
				- tmAttrib.tmOverhang;

			// now, if this is a Tab (!bSpace), compute the next tab stop
			if (!bSpace)
			{
				nNewPos = ComputeNextTab(nNewPos, nTabStops, lpnTabStops,
								nTabOrigin, nTabWidth);
			}

			// Add this width to previous width
			if (pnCurDelta == pnDxWidths)
				nStartOffset += nNewPos - nCurrentPos;
			else
				*(pnCurDelta-1) += nNewPos - nCurrentPos;

			nCurrentPos = nNewPos;

			nStartRunPos = nCurrentPos;     // set start of run
			// *lpszCurChar must be SBC: tmBreakChar or '\t'
			lpszStartRun = lpszCurChar + 1;
		}
		else
		{
			// For the non-tabbed or non-tab-character case
			int cxScreen;
			if (_istlead(*lpszCurChar))
			{
				cxScreen = tmScreen.tmAveCharWidth;
				*pnCurDelta = tmAttrib.tmAveCharWidth;
			}
			else
			{
				::GetCharWidth(m_hDC, (_TUCHAR)*lpszCurChar,
					(_TUCHAR)*lpszCurChar, &cxScreen);
				if (!::GetCharWidth(m_hAttribDC, (_TUCHAR)*lpszCurChar,
					(_TUCHAR)*lpszCurChar, pnCurDelta))
				{
					// If printer driver fails the above call, use the average width
					*pnCurDelta = tmAttrib.tmAveCharWidth;
				}
			}
			*pnCurDelta -= tmAttrib.tmOverhang;
			cxScreen -= tmScreen.tmOverhang;
			nCurrentPos += *pnCurDelta;     // update current position

			// Center character in allotted space
			if (pnCurDelta != pnDxWidths)
			{
				int diff = (*pnCurDelta - cxScreen) / 2;
				*pnCurDelta -= diff;
				*(pnCurDelta - 1) += diff;
			}
			*lpszOutputString++ = *lpszCurChar;
			if (_istlead(*lpszCurChar))
			{
				*lpszOutputString++ = *(lpszCurChar+1); // copy trailing byte
				*(pnCurDelta + 1) = *pnCurDelta;        // double wide
				nCurrentPos += *pnCurDelta;
				pnCurDelta++;
				i++;
			}
			pnCurDelta++;
		}
		lpszCurChar++;
	}

	nAlignment &= TA_CENTER|TA_RIGHT;
	sizeExtent.cx = nCurrentPos - x;
	nRightFixup = 0;

	if (nAlignment == TA_LEFT)
		x += nStartOffset;      // Full left side adjustment
	else if (nAlignment == TA_CENTER)
		x += nStartOffset/2;    // Adjust Center by 1/2 left side adjustment
	else if (nAlignment == TA_RIGHT)
		nRightFixup = nStartOffset; // Right adjust needed later if TA_UPDATECP

	if (bUpdateCP)
		::MoveToEx(m_hDC, x, ptCurrent.y, NULL);

	nCount = (UINT)(pnCurDelta - pnDxWidths);   // number of characters output
	return sizeExtent;
}


BOOL CXTPMarkupDeviceContext::IsSolidBrush(CXTPMarkupBrush* pBrush) const
{
	return pBrush && (pBrush->GetType() == MARKUP_TYPE(CXTPMarkupSolidColorBrush));
}

BOOL CXTPMarkupDeviceContext::IsLinearGradientBrush(CXTPMarkupBrush* pBrush) const
{
	return pBrush && (pBrush->GetType() == MARKUP_TYPE(CXTPMarkupLinearGradientBrush));
}

void CXTPMarkupDeviceContext::IntersectClipRect(const RECT& rcLayoutClip)
{
	::IntersectClipRect(m_hDC, rcLayoutClip.left, rcLayoutClip.top, rcLayoutClip.right, rcLayoutClip.bottom);
}

void CXTPMarkupDeviceContext::GetClipBox(LPRECT lpRect)
{
	::GetClipBox(m_hDC, lpRect);
}

HRGN CXTPMarkupDeviceContext::SaveClipRegion()
{
	HRGN hrgnClip = ::CreateRectRgn(0, 0, 0, 0);

	if (hrgnClip != NULL)
	{
		if (GetClipRgn(m_hDC, hrgnClip) != 1)
		{
			DeleteObject(hrgnClip);
			hrgnClip = (HRGN)-1;
		}
	}
	return hrgnClip;
}

void CXTPMarkupDeviceContext::RestoreClipRegion(HRGN hrgnClip)
{
	if (hrgnClip != NULL)
	{
		if (hrgnClip == (HRGN)-1)
		{
			ExtSelectClipRgn(m_hDC, NULL, RGN_COPY);
		}
		else
		{
			ExtSelectClipRgn(m_hDC, hrgnClip, RGN_COPY);
			DeleteObject(hrgnClip);
		}
	}
}

HDC CXTPMarkupDeviceContext::GetDC()
{
	return m_hDC;
}

void CXTPMarkupDeviceContext::ReleaseDC(HDC /*hDC*/)
{
}


//////////////////////////////////////////////////////////////////////////
// CXTPMarkupGdiPlusDeviceContext

#define DECLARE_GDIPLUS_METHOD(val, calltype, proc, proc_param, call_param)\
	inline GpStatus calltype proc##proc_param\
	{\
		typedef Status (calltype* PFN##proc)##proc_param;\
		if (!m_ptrMethods[val])\
		{\
			m_ptrMethods[val] = GetProcAddress(m_hModule, #proc);\
		}\
		return ((PFN##proc)m_ptrMethods[val])##call_param;\
	}

#define DECLARE_GDIPLUS_METHOD_VOID(val, calltype, proc, proc_param, call_param)\
	inline void calltype proc##proc_param\
	{\
		typedef void (calltype* PFN##proc)##proc_param;\
		if (!m_ptrMethods[val])\
		{\
			m_ptrMethods[val] = GetProcAddress(m_hModule, #proc);\
		}\
		((PFN##proc)m_ptrMethods[val])##call_param;\
	}


class CXTPMarkupGdiPlusDeviceContext::CGdiPlus
{
public:
	CGdiPlus()
	{
		m_hModule = NULL;
		m_nGdiplusToken = NULL;
		m_nCount = 0;
		ZeroMemory(&m_ptrMethods, sizeof(m_ptrMethods));
	}

public:
	void Register(BOOL bInit);

	DECLARE_GDIPLUS_METHOD(0, WINAPI, GdiplusStartup, (ULONG_PTR *token, const GdiplusStartupInput *input, GdiplusStartupOutput *output), (token, input, output))
	DECLARE_GDIPLUS_METHOD_VOID(1, WINAPI, GdiplusShutdown, (ULONG_PTR token), (token))

	DECLARE_GDIPLUS_METHOD(2, WINGDIPAPI, GdipSetPixelOffsetMode, (GpGraphics* graphics, PixelOffsetMode pixelOffsetMode), (graphics, pixelOffsetMode))
	DECLARE_GDIPLUS_METHOD(3, WINGDIPAPI, GdipSetPageUnit, (GpGraphics *graphics, GpUnit unit), (graphics, unit))
	DECLARE_GDIPLUS_METHOD(4, WINGDIPAPI, GdipSetSmoothingMode, (GpGraphics *graphics, SmoothingMode smoothingMode), (graphics, smoothingMode))
	DECLARE_GDIPLUS_METHOD(5, WINGDIPAPI, GdipSetLinePresetBlend, (GpLineGradient *brush, GDIPCONST ARGB *blend, GDIPCONST REAL* positions, INT count), (brush, blend, positions, count))

	DECLARE_GDIPLUS_METHOD(6, WINGDIPAPI, GdipCreateFromHDC2, (HDC hdc, HANDLE hDevice, GpGraphics **graphics), (hdc, hDevice, graphics))
	DECLARE_GDIPLUS_METHOD(7, WINGDIPAPI, GdipCreateFromHDC, (HDC hdc, GpGraphics **graphics), (hdc, graphics))
	DECLARE_GDIPLUS_METHOD(8, WINGDIPAPI, GdipCreatePath2, (GDIPCONST GpPointF* points, GDIPCONST BYTE* data, INT dataCount, GpFillMode fillMode, GpPath **path), (points, data, dataCount, fillMode, path))
	DECLARE_GDIPLUS_METHOD(9, WINGDIPAPI, GdipCreateSolidFill, (ARGB color, GpSolidFill **brush), (color, brush))

	DECLARE_GDIPLUS_METHOD(10, WINGDIPAPI, GdipCreateLineBrush, (GDIPCONST GpPointF* point1, GDIPCONST GpPointF* point2, ARGB color1, ARGB color2, GpWrapMode wrapMode, GpLineGradient **lineGradient), (point1, point2, color1, color2, wrapMode, lineGradient))
	DECLARE_GDIPLUS_METHOD(11, WINGDIPAPI, GdipCreatePen1, (ARGB color, REAL width, GpUnit unit, GpPen **pen), (color, width, unit, pen))
	DECLARE_GDIPLUS_METHOD(12, WINGDIPAPI, GdipCreatePen2, (GpBrush *brush, REAL width, GpUnit unit, GpPen **pen), (brush, width, unit, pen))

	DECLARE_GDIPLUS_METHOD(13, WINGDIPAPI, GdipDeleteGraphics, (GpGraphics *graphics), (graphics))
	DECLARE_GDIPLUS_METHOD(14, WINGDIPAPI, GdipDeleteBrush, (GpBrush *brush), (brush))
	DECLARE_GDIPLUS_METHOD(15, WINGDIPAPI, GdipDeletePen, (GpPen *pen), (pen))
	DECLARE_GDIPLUS_METHOD(16, WINGDIPAPI, GdipDeletePath, (GpPath* path), (path))

	DECLARE_GDIPLUS_METHOD(17, WINGDIPAPI, GdipDrawEllipse, (GpGraphics *graphics, GpPen *pen, REAL x, REAL y, REAL width, REAL height), (graphics, pen, x, y, width, height))
	DECLARE_GDIPLUS_METHOD(18, WINGDIPAPI, GdipDrawLinesI, (GpGraphics *graphics, GpPen *pen, GDIPCONST GpPoint *points, INT count), (graphics, pen, points, count))
	DECLARE_GDIPLUS_METHOD(19, WINGDIPAPI, GdipDrawPolygonI, (GpGraphics *graphics, GpPen *pen, GDIPCONST GpPoint *points, INT count), (graphics, pen, points, count))
	DECLARE_GDIPLUS_METHOD(20, WINGDIPAPI, GdipDrawPath, (GpGraphics *graphics, GpPen *pen, GpPath *path), (graphics, pen, path))
	DECLARE_GDIPLUS_METHOD(21, WINGDIPAPI, GdipDrawLineI, (GpGraphics *graphics, GpPen *pen, INT x1, INT y1, INT x2, INT y2), (graphics, pen, x1, y1, x2, y2))

	DECLARE_GDIPLUS_METHOD(22, WINGDIPAPI, GdipFillEllipse, (GpGraphics *graphics, GpBrush *brush, REAL x, REAL y, REAL width, REAL height), (graphics, brush, x, y, width, height))
	DECLARE_GDIPLUS_METHOD(23, WINGDIPAPI, GdipFillPolygonI, (GpGraphics *graphics, GpBrush *brush, GDIPCONST GpPoint *points, INT count, GpFillMode fillMode), (graphics, brush, points, count, fillMode))
	DECLARE_GDIPLUS_METHOD(24, WINGDIPAPI, GdipFillRectangleI, (GpGraphics *graphics, GpBrush *brush, INT x, INT y, INT width, INT height), (graphics, brush, x, y, width, height))
	DECLARE_GDIPLUS_METHOD(25, WINGDIPAPI, GdipFillPath, (GpGraphics *graphics, GpBrush *brush, GpPath *path), (graphics, brush, path))
	DECLARE_GDIPLUS_METHOD(26, WINGDIPAPI, GdipGetClipBoundsI, (GpGraphics *graphics, GpRect *rect), (graphics, rect))

	DECLARE_GDIPLUS_METHOD(27, WINGDIPAPI, GdipCreateFontFromLogfontA, (HDC hdc, LOGFONTA *logfont, GpFont **font), (hdc, logfont, font))
	DECLARE_GDIPLUS_METHOD(28, WINGDIPAPI, GdipCreateFontFromLogfontW, (HDC hdc, LOGFONTW *logfont, GpFont **font), (hdc, logfont, font))
	DECLARE_GDIPLUS_METHOD(29, WINGDIPAPI, GdipDeleteFont, (GpFont* font), (font))

	DECLARE_GDIPLUS_METHOD(30, WINGDIPAPI, GdipMeasureString, ( GpGraphics *graphics, GDIPCONST WCHAR *string, INT length, GDIPCONST GpFont *font, GDIPCONST RectF *layoutRect, GDIPCONST GpStringFormat *stringFormat, RectF *boundingBox, INT *codepointsFitted, INT *linesFilled ),
		(graphics, string, length, font, layoutRect, stringFormat, boundingBox, codepointsFitted, linesFilled))

	DECLARE_GDIPLUS_METHOD(31, WINGDIPAPI, GdipTranslateWorldTransform, (GpGraphics *graphics, REAL dx, REAL dy, GpMatrixOrder order), (graphics, dx, dy, order))

	DECLARE_GDIPLUS_METHOD(32, WINGDIPAPI, GdipDeleteStringFormat, (GpStringFormat *format), (format))
	DECLARE_GDIPLUS_METHOD(33, WINGDIPAPI, GdipCreateStringFormat, ( INT formatAttributes, LANGID language, GpStringFormat **format ), (formatAttributes, language, format))

	DECLARE_GDIPLUS_METHOD(34, WINGDIPAPI, GdipGetDC, (GpGraphics* graphics, HDC * hdc), (graphics, hdc))
	DECLARE_GDIPLUS_METHOD(35, WINGDIPAPI, GdipReleaseDC, (GpGraphics* graphics, HDC hdc), (graphics, hdc))

	DECLARE_GDIPLUS_METHOD(36, WINGDIPAPI, GdipDrawString, ( GpGraphics *graphics, GDIPCONST WCHAR *string, INT length, GDIPCONST GpFont *font, GDIPCONST RectF *layoutRect, GDIPCONST GpStringFormat *stringFormat, GDIPCONST GpBrush *brush ),
		( graphics, string, length, font, layoutRect, stringFormat, brush ))

	DECLARE_GDIPLUS_METHOD(37, WINGDIPAPI, GdipStringFormatGetGenericTypographic, (GpStringFormat **format), ( format ))
	DECLARE_GDIPLUS_METHOD(38, WINGDIPAPI, GdipCloneStringFormat, (GDIPCONST GpStringFormat *format, GpStringFormat **newFormat), (format, newFormat))
	DECLARE_GDIPLUS_METHOD(39, WINGDIPAPI, GdipSetStringFormatFlags, (GpStringFormat *format, INT flags), (format, flags))

	DECLARE_GDIPLUS_METHOD(40, WINGDIPAPI, GdipSetPenDashArray, (GpPen *pen, GDIPCONST REAL *dash, INT count), (pen, dash, count))
	DECLARE_GDIPLUS_METHOD(41, WINGDIPAPI, GdipSetPenLineJoin, (GpPen *pen, GpLineJoin lineJoin), (pen, lineJoin))
	DECLARE_GDIPLUS_METHOD(42, WINGDIPAPI, GdipSetPenStartCap, (GpPen *pen, GpLineCap startCap), (pen, startCap))
	DECLARE_GDIPLUS_METHOD(43, WINGDIPAPI, GdipSetPenEndCap, (GpPen *pen, GpLineCap endCap), (pen, endCap))

	DECLARE_GDIPLUS_METHOD(44, WINGDIPAPI, GdipCreateRegion, (GpRegion **region), (region))
	DECLARE_GDIPLUS_METHOD(45, WINGDIPAPI, GdipGetClip, (GpGraphics *graphics, GpRegion *region), (graphics, region))
	DECLARE_GDIPLUS_METHOD(46, WINGDIPAPI, GdipSetClipRegion, (GpGraphics *graphics, GpRegion *region, CombineMode combineMode), (graphics, region, combineMode))
	DECLARE_GDIPLUS_METHOD(47, WINGDIPAPI, GdipDeleteRegion, (GpRegion *region), (region))
	DECLARE_GDIPLUS_METHOD(48, WINGDIPAPI, GdipSetClipRectI, (GpGraphics *graphics, INT x, INT y, INT width, INT height, CombineMode combineMode),  (graphics, x, y, width, height, combineMode))

public:
	HMODULE m_hModule;
	ULONG_PTR m_nGdiplusToken;
	int m_nCount;
	LPVOID m_ptrMethods[50];
};

CXTPMarkupGdiPlusDeviceContext::CGdiPlus* CXTPMarkupGdiPlusDeviceContext::GetGdiPlus()
{
	static CGdiPlus GdiPlus;
	return &GdiPlus;
}

void CXTPMarkupGdiPlusDeviceContext::CGdiPlus::Register(BOOL bInit)
{
	if (bInit)
	{
		m_nCount++;

		if (m_nCount > 1)
			return;

		ASSERT(m_nGdiplusToken == 0 && m_hModule == 0);

		ZeroMemory(&m_ptrMethods, sizeof(m_ptrMethods));

		m_hModule = LoadLibrary(_T("GdiPlus.dll"));

		if (m_hModule)
		{
			GdiplusStartupInput gdiplusStartupInput;
			GdiplusStartup(&m_nGdiplusToken, &gdiplusStartupInput, NULL);
		}
	}
	else
	{
		m_nCount--;

		if (m_nCount != 0)
			return;

		if (m_hModule)
		{
			GdiplusShutdown(m_nGdiplusToken);
			FreeLibrary(m_hModule);
		}

		ZeroMemory(&m_ptrMethods, sizeof(m_ptrMethods));
		m_hModule = NULL;
		m_nGdiplusToken = 0;
	}
}

void CXTPMarkupGdiPlusDeviceContext::Register(BOOL bInit)
{
	GetGdiPlus()->Register(bInit);
}

CXTPMarkupGdiPlusDeviceContext::CXTPMarkupGdiPlusDeviceContext(HDC hDC, HDC hAttribDC)
	: CXTPMarkupDeviceContext(hDC, hAttribDC)
{
}

CXTPMarkupGdiPlusDeviceContext::~CXTPMarkupGdiPlusDeviceContext()
{
}

GpGraphics* CXTPMarkupGdiPlusDeviceContext::GetGraphics()
{
	GpGraphics* pGpGraphics = NULL;

	if (GetGdiPlus()->m_nGdiplusToken)
	{
		if (m_hDC != m_hAttribDC)
			GetGdiPlus()->GdipCreateFromHDC2(m_hDC, m_hAttribDC, &pGpGraphics);
		else
			GetGdiPlus()->GdipCreateFromHDC(m_hDC, &pGpGraphics);
	}

	if (!pGpGraphics)
		return NULL;

	GetGdiPlus()->GdipSetPageUnit(pGpGraphics, UnitPixel);
	GetGdiPlus()->GdipSetSmoothingMode(pGpGraphics, SmoothingModeHighQuality);
	GetGdiPlus()->GdipSetPixelOffsetMode(pGpGraphics, PixelOffsetModeHalf);

	return pGpGraphics;
}

void CXTPMarkupGdiPlusDeviceContext::ReleaseGraphics(GpGraphics* pGpGraphics)
{
	GetGdiPlus()->GdipDeleteGraphics(pGpGraphics);
}


void CXTPMarkupGdiPlusDeviceContext::OffsetViewport(const POINT& ptViewortOrg)
{
	CXTPMarkupDeviceContext::OffsetViewport(ptViewortOrg);
}


void CXTPMarkupGdiPlusDeviceContext::Ellipse(CRect rc, CXTPMarkupStrokeStyle* pStrokeStyle, CXTPMarkupBrush* pFillBrush)
{
	GpGraphics* pGpGraphics = GetGraphics();
	if (!pGpGraphics)
	{
		CXTPMarkupDeviceContext::Ellipse(rc, pStrokeStyle, pFillBrush);
		return;
	}

	int nStrokeThickness = pStrokeStyle->nStrokeThickness > 0 ? pStrokeStyle->nStrokeThickness : 0;

	GetGdiPlus()->GdipSetSmoothingMode(pGpGraphics, SmoothingModeHighQuality);
	GetGdiPlus()->GdipSetPixelOffsetMode(pGpGraphics, PixelOffsetModeHalf);

	RectF rcBound((REAL)rc.left, (REAL)rc.top, (REAL)rc.Width(), (REAL)rc.Height());

	rcBound.Inflate(-(REAL)((REAL)nStrokeThickness / 2.0), -(REAL)((REAL)nStrokeThickness / 2.0));

	if (pFillBrush)
	{
		GpBrush* pGpBrush = CreateGpBrush(pFillBrush, &rc);

		GetGdiPlus()->GdipFillEllipse(pGpGraphics, pGpBrush, rcBound.X, rcBound.Y, rcBound.Width, rcBound.Height);

		GetGdiPlus()->GdipDeleteBrush(pGpBrush);
	}

	if (nStrokeThickness > 0)
	{
		GpPen* pGpPen = CreateGpPen(pStrokeStyle, rc);

		GetGdiPlus()->GdipDrawEllipse(pGpGraphics, pGpPen, rcBound.X, rcBound.Y, rcBound.Width, rcBound.Height);

		GetGdiPlus()->GdipDeletePen(pGpPen);
	}

	ReleaseGraphics(pGpGraphics);
}

void CXTPMarkupGdiPlusDeviceContext::Polyline(const POINT* points, int nCount, CXTPMarkupStrokeStyle* pStrokeStyle)
{
	GpGraphics* pGpGraphics = GetGraphics();
	if (!pGpGraphics)
	{
		CXTPMarkupDeviceContext::Polyline(points, nCount, pStrokeStyle);
		return;
	}

	GetGdiPlus()->GdipSetSmoothingMode(pGpGraphics, SmoothingModeHighQuality);
	GetGdiPlus()->GdipSetPixelOffsetMode(pGpGraphics, (pStrokeStyle->nStrokeThickness % 2) ? PixelOffsetModeDefault : PixelOffsetModeHalf);

	GpPen* pGpPen = CreateGpPen(pStrokeStyle, GetPointsBoundRect(points, nCount));

	if (pGpPen)
	{
		GetGdiPlus()->GdipDrawLinesI(pGpGraphics, pGpPen, (GpPoint*)points, nCount);

		GetGdiPlus()->GdipDeletePen(pGpPen);
	}

	ReleaseGraphics(pGpGraphics);

}

CRect CXTPMarkupGdiPlusDeviceContext::GetPointsBoundRect(const POINT* points, int nCount) const
{
	CRect rc(INT_MAX, INT_MAX, -INT_MAX, -INT_MAX);
	for (int i = 0; i < nCount; i++)
	{
		const POINT& pt = points[i];
		rc.left = min(rc.left, pt.x);
		rc.top = min(rc.top, pt.y);
		rc.right = max(rc.right, pt.x);
		rc.bottom = max(rc.bottom, pt.y);
	}

	return rc;
}

void CXTPMarkupGdiPlusDeviceContext::Polygon(const POINT* points, int nCount, CXTPMarkupStrokeStyle* pStrokeStyle, CXTPMarkupBrush* pFillBrush)
{
	GpGraphics* pGpGraphics = GetGraphics();
	if (!pGpGraphics)
	{
		CXTPMarkupDeviceContext::Polygon(points, nCount, pStrokeStyle, pFillBrush);
		return;
	}
	int nStrokeThickness = pStrokeStyle->nStrokeThickness > 0 ? pStrokeStyle->nStrokeThickness : 0;

	GetGdiPlus()->GdipSetSmoothingMode(pGpGraphics, SmoothingModeHighQuality);

	CRect rcBound = GetPointsBoundRect(points, nCount);

	if (pFillBrush)
	{
		GpBrush* pGpBrush = CreateGpBrush(pFillBrush, rcBound);

		if (pGpBrush)
		{
			GetGdiPlus()->GdipSetPixelOffsetMode(pGpGraphics, PixelOffsetModeHalf);

			GetGdiPlus()->GdipFillPolygonI(pGpGraphics, pGpBrush, (GpPoint*)points, nCount, FillModeAlternate);

			GetGdiPlus()->GdipDeleteBrush(pGpBrush);
		}
	}

	if (nStrokeThickness > 0)
	{
		GpPen* pGpPen = CreateGpPen(pStrokeStyle, rcBound);

		if (pGpPen)
		{
			GetGdiPlus()->GdipSetPixelOffsetMode(pGpGraphics, (nStrokeThickness % 2) ? PixelOffsetModeDefault : PixelOffsetModeHalf);

			GetGdiPlus()->GdipDrawPolygonI(pGpGraphics, pGpPen, (GpPoint*)points, nCount);

			GetGdiPlus()->GdipDeletePen(pGpPen);
		}
	}

	ReleaseGraphics(pGpGraphics);
}

void CXTPMarkupGdiPlusDeviceContext::DrawPath(CXTPMarkupPathData *pData, CXTPMarkupStrokeStyle* pStrokeStyle, CXTPMarkupBrush* pFillBrush)
{
	GpGraphics* pGpGraphics = GetGraphics();
	if (!pGpGraphics)
	{
		CXTPMarkupDeviceContext::DrawPath(pData, pStrokeStyle, pFillBrush);
		return;
	}

	GetGdiPlus()->GdipSetSmoothingMode(pGpGraphics, SmoothingModeHighQuality);

	if (pData->GetCoint() > 0)
	{
		GpPath* pGpPath = NULL;
		GetGdiPlus()->GdipCreatePath2((GpPointF*)pData->GetPoints(), pData->GetTypes(), pData->GetCoint(), FillModeAlternate, &pGpPath);

		if (pGpPath)
		{
			if (pFillBrush)
			{
				GetGdiPlus()->GdipSetPixelOffsetMode(pGpGraphics, PixelOffsetModeHalf);

				GpBrush* pGpBrush = CreateGpBrush(pFillBrush, pData->GetBounds());
				GetGdiPlus()->GdipFillPath(pGpGraphics, pGpBrush, pGpPath);
				GetGdiPlus()->GdipDeleteBrush(pGpBrush);
			}

			if (pStrokeStyle && pStrokeStyle->nStrokeThickness > 0)
			{
				if (pData->m_nPixelOffsetMode == 0)
					GetGdiPlus()->GdipSetPixelOffsetMode(pGpGraphics, PixelOffsetModeHalf);
				else
					GetGdiPlus()->GdipSetPixelOffsetMode(pGpGraphics, (PixelOffsetMode)pData->m_nPixelOffsetMode);

				GpPen* pGpPen = CreateGpPen(pStrokeStyle, pData->GetBounds());
				GetGdiPlus()->GdipDrawPath(pGpGraphics, pGpPen, pGpPath);

				GetGdiPlus()->GdipDeletePen(pGpPen);
			}

			GetGdiPlus()->GdipDeletePath(pGpPath);
		}
	}

	ReleaseGraphics(pGpGraphics);
}

void CXTPMarkupGdiPlusDeviceContext::DrawLine(int x1, int y1, int x2, int y2, CXTPMarkupStrokeStyle* pStrokeStyle)
{
	GpGraphics* pGpGraphics = GetGraphics();
	if (!pGpGraphics)
	{
		CXTPMarkupDeviceContext::DrawLine(x1, y1, x2, y2, pStrokeStyle);
		return;
	}

	GetGdiPlus()->GdipSetSmoothingMode(pGpGraphics, x1 == x2 || y1 == y2 ? SmoothingModeHighSpeed : SmoothingModeHighQuality);
	GetGdiPlus()->GdipSetPixelOffsetMode(pGpGraphics, (pStrokeStyle->nStrokeThickness % 2) ? PixelOffsetModeDefault : PixelOffsetModeHalf);

	GpPen* pGpPen = CreateGpPen(pStrokeStyle, CRect(x1, y1, x2, y2));

	GetGdiPlus()->GdipDrawLineI(pGpGraphics, pGpPen, x1, y1, x2, y2);

	GetGdiPlus()->GdipDeletePen(pGpPen);

	ReleaseGraphics(pGpGraphics);
}

GpBrush* CXTPMarkupGdiPlusDeviceContext::CreateGpBrush(CXTPMarkupBrush* pBrush, LPCRECT lpRect) const
{
	if (IsSolidBrush(pBrush))
	{
		CXTPMarkupSolidColorBrush* pSolidColorBrush = (CXTPMarkupSolidColorBrush*)pBrush;

		CXTPMarkupColor* pColor = pSolidColorBrush->GetColor();

		if (!pColor)
			return NULL;

		GpSolidFill* pGpBrush = NULL;
		COLORREF clr = *pColor;

		GetGdiPlus()->GdipCreateSolidFill(RGB(GetBValue(clr), GetGValue(clr), GetRValue(clr)) | (clr & 0xFF000000) , &pGpBrush);

		return pGpBrush;
	}

	if (IsLinearGradientBrush(pBrush))
	{
		CXTPMarkupLinearGradientBrush* pLinearGradientBrush = (CXTPMarkupLinearGradientBrush*)pBrush;

		PointF ptStartPoint(0, 0);
		CXTPMarkupPoint* pt = MARKUP_STATICCAST(CXTPMarkupPoint, pBrush->GetValue(CXTPMarkupLinearGradientBrush::m_pStartPointProperty));
		if (pt) ptStartPoint = PointF((REAL)pt->x, (REAL)pt->y);

		PointF ptEndPoint(1, 1);
		pt = MARKUP_STATICCAST(CXTPMarkupPoint, pBrush->GetValue(CXTPMarkupLinearGradientBrush::m_pEndPointProperty));
		if (pt) ptEndPoint = PointF((REAL)pt->x, (REAL)pt->y);

		CXTPMarkupGradientStops* pGradientStops = pLinearGradientBrush->GetGradientStops();
		int nCount = pGradientStops->GetCount();
		if (nCount == 0)
			return NULL;

		GRADIENTSTOP* pStops = new GRADIENTSTOP[nCount];
		int i;

		for (i = 0; i < nCount; i++)
		{
			CXTPMarkupGradientStop* pItem = pGradientStops->GetItem(i);

			COLORREF clr = pItem->GetColor();
			pStops[i].clr.rgbRed = GetRValue(clr);
			pStops[i].clr.rgbGreen = GetGValue(clr);
			pStops[i].clr.rgbBlue = GetBValue(clr);
			pStops[i].clr.rgbReserved =  ((BYTE)((clr)>>24));
			pStops[i].dOffset = pItem->GetOffset();
			pStops[i].nIndex = i;
		}

		qsort(pStops, nCount, sizeof(GRADIENTSTOP), _GradientStopCompare);

		GpLineGradient* pGpBrush = NULL;

		PointF ptStart(lpRect->left + ptStartPoint.X * (lpRect->right - lpRect->left), lpRect->top + ptStartPoint.Y * (lpRect->bottom - lpRect->top));
		PointF ptEnd(lpRect->left + ptEndPoint.X * (lpRect->right - lpRect->left), lpRect->top + ptEndPoint.Y * (lpRect->bottom - lpRect->top));

		GetGdiPlus()->GdipCreateLineBrush(&ptStart, &ptEnd,
			*(ARGB*)&pStops[0].clr, *(ARGB*)&pStops[nCount - 1].clr, WrapModeTileFlipXY, &pGpBrush);

		ARGB* pBlend = new ARGB[nCount + 2];
		REAL* pPositions = new REAL[nCount + 2];
		for (i = 0; i < nCount; i++)
		{
			pBlend[i + 1] = *(ARGB*)&pStops[i].clr;
			pPositions[i + 1] = (REAL)pStops[i].dOffset;
		}
		pBlend[0] = *(ARGB*)&pStops[0].clr;
		pPositions[0] = 0;

		pBlend[nCount + 1] = *(ARGB*)&pStops[nCount - 1].clr;
		pPositions[nCount + 1] = 1;

		GetGdiPlus()->GdipSetLinePresetBlend(pGpBrush, pBlend, pPositions, nCount + 2);

		delete[] pPositions;
		delete[] pBlend;

		delete[] pStops;

		return pGpBrush;
	}


	return NULL;
}

GpPen* CXTPMarkupGdiPlusDeviceContext::CreateGpPen(CXTPMarkupStrokeStyle* pStrokeStyle, LPCRECT lpRect) const
{
	if (!pStrokeStyle || !pStrokeStyle->pStrokeBrush)
		return NULL;

	CXTPMarkupBrush* pBrush = pStrokeStyle->pStrokeBrush;

	GpPen* pGpPen = 0;

	if (IsSolidBrush(pBrush))
	{
		CXTPMarkupSolidColorBrush* pSolidColorBrush = (CXTPMarkupSolidColorBrush*)pBrush;

		CXTPMarkupColor* pColor = pSolidColorBrush->GetColor();

		if (!pColor)
			return NULL;

		COLORREF clr = *pColor;

		GetGdiPlus()->GdipCreatePen1(RGB(GetBValue(clr), GetGValue(clr), GetRValue(clr)) | (clr & 0xFF000000) , (REAL)pStrokeStyle->nStrokeThickness, UnitWorld, &pGpPen);
	}
	else
	{
		GpBrush* pGpBrush = CreateGpBrush(pBrush, lpRect);

		if (pGpBrush)
		{
			GetGdiPlus()->GdipCreatePen2(pGpBrush, (REAL)pStrokeStyle->nStrokeThickness, UnitWorld, &pGpPen);

			GetGdiPlus()->GdipDeleteBrush(pGpBrush);
		}
	}

	if (!pGpPen)
		return NULL;

	if (pStrokeStyle->pStrokeDashArray)
	{
		if (pStrokeStyle->pStrokeDashArray->GetCount() == 1)
		{
			REAL dist = pStrokeStyle->pStrokeDashArray->GetAt(0);
			REAL rDashStyle[] = {dist, dist};
			GetGdiPlus()->GdipSetPenDashArray(pGpPen, rDashStyle, 2);
		}
		else
		{
			GetGdiPlus()->GdipSetPenDashArray(pGpPen, pStrokeStyle->pStrokeDashArray->GetData(), pStrokeStyle->pStrokeDashArray->GetCount());
		}
	}

	if (pStrokeStyle->nStrokeLineJoin != xtpMarkupLineJoinMiter)
	{
		GetGdiPlus()->GdipSetPenLineJoin(pGpPen, (LineJoin)pStrokeStyle->nStrokeLineJoin);
	}

	if (pStrokeStyle->nStrokeStartLineCap != xtpMarkupLineCapFlat)
	{
		GetGdiPlus()->GdipSetPenStartCap(pGpPen, (LineCap)pStrokeStyle->nStrokeStartLineCap);
	}

	if (pStrokeStyle->nStrokeEndLineCap != xtpMarkupLineCapFlat)
	{
		GetGdiPlus()->GdipSetPenEndCap(pGpPen, (LineCap)pStrokeStyle->nStrokeEndLineCap);
	}


	return pGpPen;
}

void CXTPMarkupGdiPlusDeviceContext::FillRectangle(CRect rc, CXTPMarkupBrush* pBrush)
{
	if (m_hDC && IsGDIBrush(pBrush))
	{
		CXTPMarkupDeviceContext::FillRectangle(rc, pBrush);
		return;
	}

	GpGraphics* pGpGraphics = GetGraphics();
	if (!pGpGraphics)
	{
		CXTPMarkupDeviceContext::FillRectangle(rc, pBrush);
		return;
	}

	GpBrush* pGpBrush = CreateGpBrush(pBrush, rc);

	if (pGpBrush)
	{
		GetGdiPlus()->GdipSetSmoothingMode(pGpGraphics, SmoothingModeHighSpeed);
		GetGdiPlus()->GdipSetPixelOffsetMode(pGpGraphics, PixelOffsetModeHalf);
		GetGdiPlus()->GdipFillRectangleI(pGpGraphics, pGpBrush, rc.left, rc.top, rc.Width(), rc.Height());
		GetGdiPlus()->GdipDeleteBrush(pGpBrush);
	}

	ReleaseGraphics(pGpGraphics);
}

BOOL CXTPMarkupGdiPlusDeviceContext::IsGDIBrush(CXTPMarkupBrush* pBrush) const
{
	if (!pBrush)
		return TRUE;

	CXTPMarkupSolidColorBrush* pSolidColorBrush = MARKUP_DYNAMICCAST(CXTPMarkupSolidColorBrush, pBrush);
	if (!pSolidColorBrush)
		return FALSE;

	CXTPMarkupColor* pColor = pSolidColorBrush->GetColor();
	if (!pColor)
		return TRUE;

	return ((*pColor) & 0xFF000000) == 0xFF000000;
}

void CXTPMarkupGdiPlusDeviceContext::DrawRectangle(CRect rc, CXTPMarkupBrush* pBrush, CXTPMarkupThickness* pThickness)
{
	if (!pThickness)
		return;

	if (m_hDC && IsGDIBrush(pBrush))
	{
		CXTPMarkupDeviceContext::DrawRectangle(rc, pBrush, pThickness);
		return;
	}

	GpGraphics* pGpGraphics = GetGraphics();
	if (!pGpGraphics)
	{
		CXTPMarkupDeviceContext::DrawRectangle(rc, pBrush, pThickness);
		return;
	}

	GetGdiPlus()->GdipSetSmoothingMode(pGpGraphics, SmoothingModeHighSpeed);

	GpBrush* pGpBrush = CreateGpBrush(pBrush, rc);

	if (pThickness->GetLeft() > 0)
		GetGdiPlus()->GdipFillRectangleI(pGpGraphics, pGpBrush, rc.left, rc.top,  pThickness->GetLeft(), rc.Height());

	if (pThickness->GetRight() > 0)
		GetGdiPlus()->GdipFillRectangleI(pGpGraphics, pGpBrush, rc.right - pThickness->GetRight(), rc.top, pThickness->GetRight(), rc.Height());

	if (pThickness->GetTop() > 0)
		GetGdiPlus()->GdipFillRectangleI(pGpGraphics, pGpBrush, rc.left + pThickness->GetLeft(), rc.top, rc.Width() - pThickness->GetLeft() - pThickness->GetRight(), pThickness->GetTop());

	if (pThickness->GetBottom() > 0)
		GetGdiPlus()->GdipFillRectangleI(pGpGraphics, pGpBrush, rc.left + pThickness->GetLeft(), rc.bottom - pThickness->GetBottom(), rc.Width() - pThickness->GetLeft() - pThickness->GetRight(), pThickness->GetBottom());

	GetGdiPlus()->GdipDeleteBrush(pGpBrush);

	ReleaseGraphics(pGpGraphics);

}

void CXTPMarkupGdiPlusDeviceContext::FillRoundRectangle(CRect rc, CXTPMarkupBrush* pBrush, double* pCornerRadius)
{
	CXTPMarkupDeviceContext::FillRoundRectangle(rc, pBrush, pCornerRadius);
}

void CXTPMarkupGdiPlusDeviceContext::DrawRoundRectangle(CRect rc, CXTPMarkupBrush* pBrush, CXTPMarkupThickness* pThickness, double* pCornerRadius)
{
	CXTPMarkupDeviceContext::DrawRoundRectangle(rc, pBrush, pThickness, pCornerRadius);
}

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
