// XTPMarkupGdiPlusDeviceContext.cpp: implementation of the CXTPMarkupGdiPlusDeviceContext class.
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

#include <Common/XTPDrawHelpers.h>
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

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////////
// CXTPMarkupGdiPlusDeviceContext



CXTPGdiPlus* CXTPMarkupGdiPlusDeviceContext::GetGdiPlus()
{
	static CXTPGdiPlus GdiPlus;
	return &GdiPlus;
}

void CXTPGdiPlus::Register(BOOL bInit)
{
	static GdiplusStartupOutput gdiplusStartupOutput;
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
	m_smoothingMode = SmoothingModeHighQuality;
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
	GetGdiPlus()->GdipSetSmoothingMode(pGpGraphics, m_smoothingMode);
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

void CXTPMarkupGdiPlusDeviceContext::SetSmoothingMode(XTPMarkupSmoothingMode smoothingMode)
{
	SmoothingMode gdipSmoothingMode = SmoothingModeInvalid;

	switch(smoothingMode)
	{
	case xtpMarkupSmoothingDefault:     gdipSmoothingMode = SmoothingModeDefault;     break;
	case xtpMarkupSmoothingNone:        gdipSmoothingMode = SmoothingModeNone;        break;
	case xtpMarkupSmoothingHighSpeed:   gdipSmoothingMode = SmoothingModeHighSpeed;   break;
	case xtpMarkupSmoothingHighQuality: gdipSmoothingMode = SmoothingModeHighQuality; break;
	case xtpMarkupSmoothingAntiAlias:   gdipSmoothingMode = SmoothingModeAntiAlias;   break;
	}

	if (SmoothingModeInvalid != gdipSmoothingMode)
	{
		m_smoothingMode = gdipSmoothingMode;
	}
}

void CXTPMarkupGdiPlusDeviceContext::Ellipse(CRect rc, CXTPMarkupStrokeStyle* pStrokeStyle, CXTPMarkupBrush* pFillBrush)
{
	GpGraphics *pGpGraphics = GetGraphics();
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

	if (pData->GetCount() > 0)
	{
		GpPath* pGpPath = NULL;
		GetGdiPlus()->GdipCreatePath2((GpPointF*)pData->GetPoints(), pData->GetTypes(), pData->GetCount(), FillModeAlternate, &pGpPath);

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

//////////////////////////////////////////////////////////////////////////
// The following function (GdipCreateBitmapFromHICON_Fixed_Native)
// is a fixed version of Microsoft one
// The problem of the original code not saving of transparency
// of HICON (alpha channel). The bug dates to 2001 (!) when GDI+
// was originally introduced. It is widely mentioned on Microsoft websites
// with some workarounds (not working for our case).
// The bug is not fixed and doesn't seem to be planned to be fixed.
// The code is based on wine (www.winehq.org) code
//////////////////////////////////////////////////////////////////////////

Gdiplus::Status CXTPMarkupGdiPlusDeviceContext::GdipCreateBitmapFromHICON_Fixed(HICON hicon, Gdiplus::GpBitmap** bitmap) const
{
	Gdiplus::GpStatus returnStatus = Gdiplus::Ok;

	Gdiplus::GpStatus stat;

	BITMAP bm;
	int width = 0, height = 0;
	Gdiplus::Rect rect;
	Gdiplus::BitmapData lockeddata;
	BOOL has_alpha;
	int x, y;

	BITMAPINFOHEADER bih;
	DWORD *src = NULL;
	BYTE *dst_row = NULL;
	DWORD *dst = NULL;

	ICONINFO iinfo;
	ZeroMemory(&iinfo, sizeof(ICONINFO));

	BYTE *pbBitmapBits = NULL;

	HDC hScreenDC = NULL;

	try
	{
		if(!bitmap || !::GetIconInfo(hicon, &iinfo))
			throw Gdiplus::InvalidParameter;

		// get the size of the icon
		if (!GetObject(iinfo.hbmColor ? iinfo.hbmColor : iinfo.hbmMask, sizeof(bm), &bm))
			throw Gdiplus::GenericError;

		width = bm.bmWidth;

		if (iinfo.hbmColor)
			height = abs(bm.bmHeight);
		else // combined bitmap + mask
			height = abs(bm.bmHeight) / 2;

		pbBitmapBits = (BYTE *)::HeapAlloc(::GetProcessHeap(), 0, 4*width*height);

		if (!pbBitmapBits)
			throw Gdiplus::OutOfMemory;

		stat = GetGdiPlus()->GdipCreateBitmapFromScan0(width, height, 0, PixelFormat32bppARGB, NULL, bitmap);

		rect.X = 0;
		rect.Y = 0;
		rect.Width = width;
		rect.Height = height;

		stat = GetGdiPlus()->GdipBitmapLockBits(*bitmap, &rect, Gdiplus::ImageLockModeWrite, PixelFormat32bppARGB, &lockeddata);

		bih.biSize = sizeof(bih);
		bih.biWidth = width;
		bih.biHeight = -height;
		bih.biPlanes = 1;
		bih.biBitCount = 32;
		bih.biCompression = BI_RGB;
		bih.biSizeImage = 0;
		bih.biXPelsPerMeter = 0;
		bih.biYPelsPerMeter = 0;
		bih.biClrUsed = 0;
		bih.biClrImportant = 0;

		hScreenDC = ::GetDC(NULL);
		if (iinfo.hbmColor)
		{
			::GetDIBits(hScreenDC, iinfo.hbmColor, 0, height, pbBitmapBits, (BITMAPINFO*)&bih, DIB_RGB_COLORS);

			if (bm.bmBitsPixel == 32)
			{
				has_alpha = FALSE;

				// If any pixel has a non-zero alpha, ignore hbmMask
				src = (DWORD*)pbBitmapBits;
				for (x=0; x<width && !has_alpha; x++)
					for (y=0; y<height && !has_alpha; y++)
						if ((*src++ & 0xff000000) != 0)
							has_alpha = TRUE;
			}
			else
				has_alpha = FALSE;
		}
		else
		{
			::GetDIBits(hScreenDC, iinfo.hbmMask, 0, height, pbBitmapBits, (BITMAPINFO*)&bih, DIB_RGB_COLORS);

			has_alpha = FALSE;
		}

		// copy the image data to the Bitmap
		src = (DWORD*)pbBitmapBits;
		dst_row = (BYTE *)lockeddata.Scan0;
		for (y=0; y<height; y++)
		{
			memcpy(dst_row, src, width*4);
			src += width;
			dst_row += lockeddata.Stride;
		}

		if (!has_alpha)
		{
			if (iinfo.hbmMask)
			{
				// read alpha data from the mask
				if (iinfo.hbmColor)
					::GetDIBits(hScreenDC, iinfo.hbmMask, 0, height, pbBitmapBits, (BITMAPINFO*)&bih, DIB_RGB_COLORS);
				else
					::GetDIBits(hScreenDC, iinfo.hbmMask, height, height, pbBitmapBits, (BITMAPINFO*)&bih, DIB_RGB_COLORS);

				src = (DWORD*)pbBitmapBits;
				dst_row = (BYTE *)lockeddata.Scan0;
				for (y=0; y<height; y++)
				{
					dst = (DWORD*)dst_row;
					for (x=0; x<width; x++)
					{
						DWORD src_value = *src++;
						if (src_value)
							*dst++ = 0;
						else
							*dst++ |= 0xff000000;
					}
					dst_row += lockeddata.Stride;
				}
			}
			else
			{
				// set constant alpha of 255
				dst_row = pbBitmapBits;
				for (y=0; y<height; y++)
				{
					dst = (DWORD*)dst_row;
					for (x=0; x<height; x++)
						*dst++ |= 0xff000000;
					dst_row += lockeddata.Stride;
				}
			}
		}
	}
	catch (Gdiplus::Status err_status)
	{
		returnStatus = err_status;
	}

	if (hScreenDC)
		::ReleaseDC(0, hScreenDC);

	if (iinfo.hbmColor)
		::DeleteObject(iinfo.hbmColor);

	if (iinfo.hbmColor)
		::DeleteObject(iinfo.hbmMask);

	if (bitmap && *bitmap)
		stat = GetGdiPlus()->GdipBitmapUnlockBits(*bitmap,&lockeddata);

	if (pbBitmapBits)
		::HeapFree(GetProcessHeap(), 0, pbBitmapBits);

	return returnStatus;
}

GpBitmap *CXTPMarkupGdiPlusDeviceContext::IconToGdiplusBitmap(CXTPImageManagerIcon* pIcon) const
{
	GpBitmap *pGpBitmap = NULL;

	// Copying icon handle is required, because initial CXTPImageManagerIconHandle
	// ICON/Bitmap members are always NULL, after copying it's OK
	CXTPImageManagerIconHandle iconHandle;
	iconHandle.CopyHandle(pIcon->GetIcon());

	if (iconHandle.IsAlpha())
	{
		BITMAP bmpInfo;
		::GetObject(iconHandle.GetBitmap(), sizeof(BITMAP), &bmpInfo);
		int cxBitmap = bmpInfo.bmWidth;
		int cyBitmap = bmpInfo.bmHeight;

		if (GetGdiPlus()->GdipCreateBitmapFromScan0(cxBitmap, cyBitmap, cxBitmap*4, PixelFormat32bppARGB, (BYTE*)bmpInfo.bmBits, &pGpBitmap) == Ok)
			GetGdiPlus()->GdipImageRotateFlip(pGpBitmap, Rotate180FlipX);

	}
	else
	{
		if (iconHandle.GetIcon())
		{
			GdipCreateBitmapFromHICON_Fixed(iconHandle.GetIcon(), &pGpBitmap);
		}
		else
		{
			if (iconHandle.GetBitmap())
				GetGdiPlus()->GdipCreateBitmapFromHBITMAP(iconHandle.GetBitmap(), 0, &pGpBitmap);
		}
	}

	return pGpBitmap;
}

void CXTPMarkupGdiPlusDeviceContext::DrawImage(CXTPImageManagerIcon* pIcon, CRect rc)
{
	// GDI-plus drawing must draw in its own context instead of calling CXTPMarkupGdiPlusDeviceContext::GetDC
	// because the DC returned by GetGdiPlus()->GdipGetDC() will not respect the transformation applied to the
	// GDI-plus context
	// The solution is too convert CXTPImageManagerIcon to GpBitmap
	// FYI: CXTPMarkupDeviceContext::DrawImage is using a simple drawing

	GpGraphics* pGpGraphics = GetGraphics();

	if (pGpGraphics)
	{
		GpBitmap *pGpBitmap = IconToGdiplusBitmap(pIcon);

		ASSERT(pGpBitmap);

		if (pGpBitmap)
		{
			HDC hDC = GetDC();

			DWORD dwLayout = XTPDrawHelpers()->GetLayout(hDC);
			if (LAYOUT_RTL & dwLayout)
			{
				XTPDrawHelpers()->SetContextRTL(hDC, LAYOUT_BITMAPORIENTATIONPRESERVED);

				ReleaseGraphics(pGpGraphics);

				pGpGraphics = GetGraphics();
			}

			ReleaseDC(hDC);

			if (LAYOUT_RTL & dwLayout)
			{
				XTPDrawHelpers()->SetContextRTL(hDC, dwLayout);
			}

			GetGdiPlus()->GdipDrawImageRect(pGpGraphics, pGpBitmap, (REAL)rc.left, (REAL)rc.top, (REAL)rc.Width(), (REAL)rc.Height());
			GetGdiPlus()->GdipDisposeImage(pGpBitmap);
		}
		else
		{
			// if the conversion failed, we need to provide the old drawing code

			HDC hDc = GetDC();

			pIcon->Draw(CDC::FromHandle(hDc), rc.TopLeft(), rc.Size());

			ReleaseDC(hDc);
		}

		ReleaseGraphics(pGpGraphics);
	}
	else
	{
		CXTPMarkupDeviceContext::DrawImage(pIcon, rc);
	}
}

void CXTPMarkupGdiPlusDeviceContext::DrawLine(int x1, int y1, int x2, int y2, CXTPMarkupStrokeStyle* pStrokeStyle)
{
	HDC hDC = GetDC();

	DWORD dwLayout = XTPDrawHelpers()->GetLayout(hDC);
	if (LAYOUT_RTL & dwLayout)
	{
		XTPDrawHelpers()->SetContextRTL(hDC, LAYOUT_BITMAPORIENTATIONPRESERVED);
	}

	GpGraphics *pGpGraphics = GetGraphics();

	if (LAYOUT_RTL & dwLayout)
	{
		XTPDrawHelpers()->SetContextRTL(hDC, dwLayout);
	}

	ReleaseDC(hDC);

	//Fix disappears on lines in RTL mode. 
	//When mode is RTL draw lines using GDI instead using of GDI+
	//Set of smoothing mode will not work when we draw line with GDI in RTL mode.
	if ((LAYOUT_RTL & dwLayout) || !pGpGraphics)
	{
		CXTPMarkupDeviceContext::SetSmoothingMode( (XTPMarkupSmoothingMode) m_smoothingMode);
		//DrawLine function draws a line from the current position up to, but not including, the last point
		CXTPMarkupDeviceContext::DrawLine(x1, y1, x2, y2, pStrokeStyle);
		//Call to draw only last point (x2, y2) 
		CXTPMarkupDeviceContext::DrawLine(x2, y2, x1, y1, pStrokeStyle);
	}
	else
	{
		GetGdiPlus()->GdipSetSmoothingMode(pGpGraphics, m_smoothingMode);
		GetGdiPlus()->GdipSetPixelOffsetMode(pGpGraphics, (pStrokeStyle->nStrokeThickness % 2) ? PixelOffsetModeDefault : PixelOffsetModeHalf);
		
		GpPen* pGpPen = CreateGpPen(pStrokeStyle, CRect(x1, y1, x2, y2));

		GetGdiPlus()->GdipDrawLineI(pGpGraphics, pGpPen, x1, y1, x2, y2);

		GetGdiPlus()->GdipDeletePen(pGpPen);

		ReleaseGraphics(pGpGraphics);
	}
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
