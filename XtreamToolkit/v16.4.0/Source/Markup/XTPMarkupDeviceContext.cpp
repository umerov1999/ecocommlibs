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
#include <math.h>

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
	int nCount = pData->GetCount();
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

void CXTPMarkupDeviceContext::SetSmoothingMode(XTPMarkupSmoothingMode)
{
	// GDI does not support smoothing
}

void CXTPMarkupDeviceContext::DrawImage(CXTPImageManagerIcon* pIcon, CRect rc)
{
	CDC *pDC = CDC::FromHandle(GetSafeHdc());

	if (pDC)
		pIcon->Draw(pDC, rc.TopLeft(), rc.Size());
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

void CXTPMarkupDeviceContext::DrawLine(int x1, int y1, int x2, int y2, CXTPMarkupStrokeStyle *pStrokeStyle)
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
