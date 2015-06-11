// XTPSkinImage.cpp: implementation of the CXTPSkinImage class.
//
// This file is a part of the XTREME SKINFRAMEWORK MFC class library.
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

#include <Common/Tmschema.h>
#include <Common/XTPImageManager.h>
#include <Common/XTPColorManager.h>
#include <Common/XTPDrawHelpers.h>
#include <Common/XTPVC80Helpers.h>
#include <Common/XTPSystemHelpers.h>
#include <Common/XTPSynchro.h>

#include "XTPSkinImage.h"
#include "XTPSkinManagerResource.h"
#include "XTPSkinManager.h"
#include "XTPSkinManagerSchema.h"
#include "XTPSkinDrawTools.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define TILE_VERTICAL     1
#define TILE_HORIZONTAL   2
#define TILE_ALL          3


//////////////////////////////////////////////////////////////////////
// CXTPSkinImage

CXTPSkinImage::CXTPSkinImage()
{
	m_hBitmap = NULL;
	m_bAlpha = FALSE;
	m_bFiltered = FALSE;
	m_bInvert = FALSE;
	m_bMirrorImage = TRUE;
	m_bOptimized = FALSE;
}

CXTPSkinImage::~CXTPSkinImage()
{
	if (m_hBitmap != NULL)
	{
		DeleteObject(m_hBitmap);
	}
}

void CXTPSkinImage::SetBitmap(HBITMAP hBitmap, BOOL bAlpha /*= FALSE*/)
{
	ASSERT(m_hBitmap == NULL);

	m_hBitmap = hBitmap;
	m_bAlpha = bAlpha;

	m_bOptimized = FALSE;
	m_arrSolidRects.RemoveAll();

	m_szBitmap = _GetExtent();
}

#define FREE(hHandle) if (hHandle) { free(hHandle); hHandle = 0; }


BOOL CXTPSkinImage::CheckBitmapRect(LPBYTE pBits, CRect rcCheck, CSize sz)
{
	if (rcCheck.left >= rcCheck.right || rcCheck.top >= rcCheck.bottom ||
		rcCheck.left < 0 || rcCheck.top < 0 || rcCheck.right >= sz.cx || rcCheck.bottom >= sz.cy)
	{
		return FALSE;
	}

	BOOL bFirst = TRUE;
	COLORREF clrTransparent = COLORREF_NULL;

	for (int x = rcCheck.left; x < rcCheck.right; x++)
	for (int y = rcCheck.top; y < rcCheck.bottom; y++)
	{
		COLORREF clr = ((LPDWORD)pBits)[x + (sz.cy - y - 1) * sz.cx];

		if (bFirst)
			clrTransparent = clr;
		else if (clrTransparent != clr)
			return FALSE;

		bFirst = FALSE;
	}

	SOLIDRECT sr;
	sr.rc = rcCheck;
	sr.clr = RGB(GetBValue(clrTransparent), GetGValue(clrTransparent), GetRValue(clrTransparent));

	if (m_bAlpha)
		sr.clr |= (clrTransparent & 0xFF000000);

	m_arrSolidRects.Add(sr);
	return TRUE;
}


void CXTPSkinImage::CreateSolidRectArray(int nBitmaps, BOOL bHorizontalImage, const CRect& rcSizingMargins)
{
	ASSERT(nBitmaps >= 1);
	if (!m_hBitmap || nBitmaps < 1)
		return;

	if (m_bOptimized || m_bFiltered)
		return;

	if (nBitmaps == 1)
		bHorizontalImage = FALSE;

	CSize sz(GetExtent());

	m_bOptimized = TRUE;
	m_arrSolidRects.RemoveAll();

	HBITMAP hbm = NULL;
	LPBYTE pBits = NULL;

	CDC dcSrc;
	dcSrc.CreateCompatibleDC(NULL);

	TRY
	{
		if (!m_bAlpha)
		{
			hbm = CXTPImageManager::Create32BPPDIBSection(dcSrc, sz.cx, sz.cy, &pBits);

			{
				CXTPCompatibleDC dc(NULL, hbm);
				dc.DrawState(CPoint(0, 0), sz, m_hBitmap, 0, NULL);
			}
		}
		else
		{
			PBITMAPINFO pBitmapInfo = 0;

			UINT nSize;
			if (!CXTPImageManagerIcon::GetBitmapBits(dcSrc, m_hBitmap, pBitmapInfo, (LPVOID&)pBits, nSize))
				return;

			FREE(pBitmapInfo);
		}

		if (!pBits)
			return;


		if (!bHorizontalImage)
		{
			int nSpriteHeight = sz.cy / nBitmaps;

			for (int i = 0; i < nBitmaps; i++)
			{
				CRect rcSprite(0, i * nSpriteHeight, sz.cx, (i + 1) * nSpriteHeight);
				if (CheckBitmapRect(pBits, rcSprite, sz))
					continue;

				if (rcSizingMargins.IsRectNull())
					continue;

				CheckBitmapRect(pBits, CRect(rcSprite.left, rcSprite.top + rcSizingMargins.top,
					rcSprite.left + rcSizingMargins.left, rcSprite.bottom - rcSizingMargins.bottom), sz);

				CheckBitmapRect(pBits, CRect(rcSprite.left + rcSizingMargins.left, rcSprite.top,
					rcSprite.right - rcSizingMargins.right, rcSprite.top + rcSizingMargins.top), sz);

				CheckBitmapRect(pBits, CRect(rcSprite.right - rcSizingMargins.right , rcSprite.top + rcSizingMargins.top,
					rcSprite.right, rcSprite.bottom - rcSizingMargins.bottom), sz);

				CheckBitmapRect(pBits, CRect(rcSprite.left + rcSizingMargins.left, rcSprite.bottom - rcSizingMargins.bottom,
					rcSprite.right - rcSizingMargins.right, rcSprite.bottom), sz);

				CheckBitmapRect(pBits, CRect(rcSprite.left + rcSizingMargins.left, rcSprite.top + rcSizingMargins.top,
					rcSprite.right - rcSizingMargins.right, rcSprite.bottom - rcSizingMargins.bottom), sz);
			}
		}
	}
	CATCH (CMemoryException, e)
	{
		TRACE(_T("Failed -- Memory exception thrown."));
	}
	END_CATCH

	if (hbm && !m_bAlpha)
	{
		DeleteObject(hbm);
	}
	if (m_bAlpha)
	{
		FREE(pBits);
	}


	m_bOptimized = TRUE;
}

void CXTPSkinImage::FilterImage(COLORREF clrTransparent)
{
	if (!m_hBitmap || m_bFiltered)
		return;

	m_bFiltered = TRUE;

	CXTPSkinManager* pManager =  XTPSkinManager();

	if (!pManager->IsColorFilterExists())
		return;

	CDC dcSrc;
	dcSrc.CreateCompatibleDC(NULL);

	if (!m_bAlpha)
	{
		BITMAPINFO bmi;
		HBITMAP hbm;
		LPBYTE pBits;
		BITMAP hbmInfo;
		GetObject(m_hBitmap, sizeof(BITMAP), &hbmInfo);

		// Initialize header to 0s.
		ZeroMemory(&bmi, sizeof(bmi));

		// Fill out the fields you care about.
		bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bmi.bmiHeader.biWidth = hbmInfo.bmWidth;
		bmi.bmiHeader.biHeight = hbmInfo.bmHeight;
		bmi.bmiHeader.biPlanes = 1;
		bmi.bmiHeader.biBitCount = 32;
		bmi.bmiHeader.biCompression = BI_RGB;

		// Create the surface.
		hbm = CreateDIBSection(NULL, &bmi, DIB_RGB_COLORS, (LPVOID*)&pBits, NULL, 0);

		{
			CXTPCompatibleDC dc(NULL, hbm);
			dc.DrawState(CPoint(0, 0), CSize(hbmInfo.bmWidth, hbmInfo.bmHeight), m_hBitmap, 0, NULL);
		}

		DeleteObject(m_hBitmap);
		m_hBitmap = hbm;
	}

	PBYTE pBits = 0;
	PBITMAPINFO pBitmapInfo = 0;
	HBITMAP hBitmapAlpha = 0;

	TRY
	{
		UINT nSize;
		if (!CXTPImageManagerIcon::GetBitmapBits(dcSrc, m_hBitmap, pBitmapInfo, (LPVOID&)pBits, nSize))
			return;

		BYTE* pDest = NULL;

		hBitmapAlpha = CreateDIBSection(dcSrc, pBitmapInfo, DIB_RGB_COLORS, (void**)&pDest, NULL, 0);
		if (pDest == NULL || hBitmapAlpha == NULL)
			AfxThrowMemoryException();

		MEMCPY_S(pDest, pBits, nSize);

		for (UINT i = 0; i < nSize; i += 4)
		{
			COLORREF clr = RGB(pDest[i + 2], pDest[i + 1], pDest[i + 0]);

			if (clr != clrTransparent)
			{
				pManager->ApplyColorFilter(clr);

				pDest[i + 0] = GetBValue(clr);
				pDest[i + 1] = GetGValue(clr);
				pDest[i + 2] = GetRValue(clr);
			}
		}
	}
	CATCH (CMemoryException, e)
	{
		TRACE(_T("Failed -- Memory exception thrown."));
	}
	END_CATCH

	FREE(pBits);
	FREE(pBitmapInfo);

	DeleteObject(m_hBitmap);
	m_hBitmap = hBitmapAlpha;
}

BOOL CXTPSkinImage::LoadFile(LPCTSTR lpszFileName)
{
	m_bAlpha = FALSE;

	BOOL bAlpha = FALSE;
	HBITMAP hBmp = CXTPImageManagerIcon::LoadBitmapFromFile(lpszFileName, &bAlpha);

	if (!hBmp)
		return FALSE;

	if (bAlpha)
	{
		if (XTPSkinManager()->GetSchema()->m_bPreMultiplyImages)
		{
			m_hBitmap = CXTPImageManagerIcon::PreMultiplyAlphaBitmap(hBmp, &m_bAlpha);
			DeleteObject(hBmp);
		}
		else
		{
			m_hBitmap = hBmp;
			m_bAlpha = TRUE;
		}
	}
	else
	{
		m_hBitmap = hBmp;
	}

	m_szBitmap = _GetExtent();

	return TRUE;
}

BOOL CXTPSkinImage::LoadFile(HMODULE hModule, LPCTSTR lpszBitmapFileName)
{
	if (!hModule)
		return FALSE;

	m_bAlpha = FALSE;

	ASSERT(m_hBitmap == NULL);

	BOOL bAlpha = FALSE;
	HBITMAP hBmp = XTPSkinFrameworkLoadBitmap(hModule, lpszBitmapFileName, bAlpha);

	if (!hBmp)
		return FALSE;

	if (bAlpha)
	{
		if (XTPSkinManager()->GetSchema()->m_bPreMultiplyImages)
		{
			m_hBitmap = CXTPImageManagerIcon::PreMultiplyAlphaBitmap(hBmp, &m_bAlpha);
			DeleteObject(hBmp);
		}
		else
		{
			m_hBitmap = hBmp;
			m_bAlpha = TRUE;
		}
	}
	else
	{
		m_hBitmap = hBmp;
	}

	m_szBitmap = _GetExtent();

	return m_hBitmap != NULL;
}

BOOL CXTPSkinImage::DrawImagePart(CDC* pDCDest, const CRect& rcDest, CDC* pDCSrc, const CRect& rcSrc) const
{
	if (rcDest.Width() <= 0 || rcDest.Height() <= 0 || rcSrc.Width() <= 0 || rcSrc.Height() <= 0)
		return TRUE;

	BOOL bResult = TRUE;

	if (m_bAlpha)
	{
		bResult = XTPImageManager()->AlphaBlend(pDCDest->GetSafeHdc(), rcDest, pDCSrc->GetSafeHdc(), rcSrc);
	}
	else if ((rcSrc.Width() == rcDest.Width()) && (rcSrc.Height() == rcDest.Height()))
	{
		bResult = pDCDest->BitBlt(rcDest.left, rcDest.top, rcDest.Width(), rcDest.Height(),
			pDCSrc, rcSrc.left, rcSrc.top, SRCCOPY);
	}
	else
	{
		bResult = pDCDest->StretchBlt(rcDest.left, rcDest.top, rcDest.Width(), rcDest.Height(),
			pDCSrc, rcSrc.left, rcSrc.top, rcSrc.Width(), rcSrc.Height(), SRCCOPY);
	}

	return bResult;
}


BOOL CXTPSkinImage::DrawImageTile(
	CDC* pDCDest,
	const CRect& rcDest,
	CDC* pDCSrc,
	const CRect& rcSrc,
	BOOL bTile,
	COLORREF clrTransparent /*= COLORREF_NULL*/) const
{
	if (rcDest.Width() <= 0 || rcDest.Height() <= 0 || rcSrc.Width() <= 0 || rcSrc.Height() <= 0)
		return TRUE;

	SOLIDRECT sr;
	if (FindSolidRect(rcSrc, sr))
	{
		COLORREF clrFilter = sr.clr & 0xFFFFFF;
		if (clrFilter != clrTransparent) XTPSkinManager()->ApplyColorFilter(clrFilter);

		if (!m_bAlpha)
		{
			pDCDest->FillSolidRect(rcDest, clrFilter);
			return TRUE;
		}
		else
		{
			if (sr.clr == 0)
				return TRUE;

			if ((sr.clr >> 24) == 0xFF)
			{
				pDCDest->FillSolidRect(rcDest, clrFilter);
				return  TRUE;
			}
		}
	}

	if (!bTile)
	{
		return DrawImagePart(pDCDest, rcDest, pDCSrc, rcSrc);
	}

	if (rcSrc.Width() == 1 && (rcSrc.Height() == 1 || rcSrc.Height() == rcDest.Height()))
	{
		return DrawImagePart(pDCDest, rcDest, pDCSrc, rcSrc);
	}

	if (rcSrc.Height() == 1 && (rcSrc.Width() == 1 || rcSrc.Width() == rcDest.Width()))
	{
		return DrawImagePart(pDCDest, rcDest, pDCSrc, rcSrc);
	}

	if (rcSrc.right <= rcSrc.left || rcDest.right <= rcDest.left)
		return TRUE;

	if (rcSrc.bottom <= rcSrc.top || rcDest.bottom <= rcDest.top)
		return TRUE;

	int nHeight = bTile & TILE_VERTICAL? min(rcSrc.Height(), rcDest.Height()): rcDest.Height();
	int nWidth = bTile & TILE_HORIZONTAL? min(rcSrc.Width(), rcDest.Width()): rcDest.Width();

	for (int x = rcDest.left; x < rcDest.right; x += nWidth)
	{
		for (int y = rcDest.top; y < rcDest.bottom; y += nHeight)
		{
			CRect rcDestTile(x, y, x + nWidth, y + nHeight);
			CRect rcSrcTile(rcSrc);

			if (bTile & TILE_VERTICAL)
			{
				if (rcSrcTile.Height() > rcDestTile.Height())
					rcSrcTile.bottom = rcSrcTile.top + rcDestTile.Height();

				if (rcDestTile.bottom > rcDest.bottom)
				{
					rcDestTile.bottom = rcDest.bottom;
					rcSrcTile.bottom = rcSrcTile.top + rcDestTile.Height();
				}
			}

			if (bTile & TILE_HORIZONTAL)
			{
				if (rcSrcTile.Width() > rcDestTile.Width())
					rcSrcTile.right = rcSrcTile.left + rcDestTile.Width();

				if (rcDestTile.right > rcDest.right)
				{
					rcDestTile.right = rcDest.right;
					rcSrcTile.right = rcSrcTile.left + rcDestTile.Width();
				}
			}

			DrawImagePart(pDCDest, rcDestTile, pDCSrc, rcSrcTile);
		}
	}
	return TRUE;
}

BOOL CXTPSkinImage::FindSolidRect(const CRect& rcSrc, SOLIDRECT& sr) const
{
	for (int i = 0; i < m_arrSolidRects.GetSize(); i++)
	{
		sr = m_arrSolidRects[i];

		RECT rc;
		if (IntersectRect(&rc, &rcSrc, &sr.rc) && rcSrc == rc)
		{
			return TRUE;
		}
	}
	return FALSE;
}

void CXTPSkinImage::DrawImage(
	CDC* pDC,
	const CRect& rcDest,
	const CRect& rcSrc,
	const CRect& rcSizingMargins,
	int nSizingType,
	BOOL bBorderOnly)
{
	DrawImageInternal(pDC, rcDest, rcSrc, rcSizingMargins, nSizingType, bBorderOnly);
}

void CXTPSkinImage::DrawImage(
	CDC* pDC,
	const CRect& rcDest,
	const CRect& rcSrc,
	const CRect& rcSizingMargins,
	COLORREF clrTransparent,
	int nSizingType,
	BOOL bBorderOnly)
{
	if (rcSizingMargins.top + rcSizingMargins.bottom > rcSrc.Height())
		return;

	SOLIDRECT sr;
	if (FindSolidRect(rcSrc, sr))
	{
		COLORREF clrFilter = sr.clr & 0xFFFFFF;
		if (clrFilter != clrTransparent) XTPSkinManager()->ApplyColorFilter(clrFilter);

		if (!m_bAlpha)
		{
			if (clrFilter != clrTransparent)
				pDC->FillSolidRect(rcDest, clrFilter);
			return;
		}
		else
		{
			if (sr.clr == 0)
				return;

			if ((sr.clr >> 24) == 0xFF)
			{
				pDC->FillSolidRect(rcDest, clrFilter);
				return;
			}
		}
	}

	if (clrTransparent == COLORREF_NULL || m_bAlpha)
	{
		DrawImageInternal(pDC, rcDest, rcSrc, rcSizingMargins, nSizingType, bBorderOnly, clrTransparent);
		return;
	}

	FilterImage(clrTransparent);

	CBitmap bmp;
	bmp.CreateCompatibleBitmap(pDC, rcDest.Width(), rcDest.Height());

	CXTPCompatibleDC dc(pDC, &bmp);

	CRect rcDestOrig(0, 0, rcDest.Width(), rcDest.Height());

	dc.FillSolidRect(rcDestOrig, clrTransparent);
	DrawImageInternal(&dc, rcDestOrig, rcSrc, rcSizingMargins, nSizingType, bBorderOnly, clrTransparent);

	XTPImageManager()->TransparentBlt(*pDC, rcDest, dc, rcDestOrig, clrTransparent);

}

void CXTPSkinImage::InvertBitmap()
{
	if (!m_bAlpha)
		return;

	HBITMAP hBitmap = m_hBitmap;
	m_hBitmap = CXTPImageManagerIcon::InvertAlphaBitmap(m_hBitmap);
	DeleteObject(hBitmap);

	m_bInvert = !m_bInvert;
}

void CXTPSkinImage::DrawImageInternal(
	CDC* pDC,
	const CRect& rcDest,
	const CRect& rcSrc,
	const CRect& rcSizingMargins,
	int nSizingType,
	BOOL bBorderOnly,
	COLORREF clrTransparent /*= COLORREF_NULL*/)
{
	if (m_hBitmap == NULL)
		return;

	if (rcSizingMargins.top + rcSizingMargins.bottom > rcSrc.Height())
		return;

	FilterImage(COLORREF_NULL);


	pDC->SetStretchBltMode(COLORONCOLOR);

	CRect rcDestSizingMargins = rcSizingMargins;
	BOOL bTile = nSizingType == ST_TILE? TILE_ALL: 0;

	if (rcDest.left >= rcDest.right || rcDest.top >= rcDest.bottom)
		return;

	BOOL bRtl = CXTPDrawHelpers::IsContextRTL(pDC);

	if (m_bAlpha && m_bMirrorImage)
	{
		if ((bRtl && !m_bInvert) || (!bRtl && m_bInvert))
		{
			InvertBitmap();
		}
	}

	CXTPCompatibleDC dcSrc(pDC, m_hBitmap);

	if (bRtl && !m_bAlpha && m_bMirrorImage)
	{
		CXTPDrawHelpers::SetContextRTL(&dcSrc, FALSE);
	}

	if (rcSizingMargins.IsRectNull())
	{
		DrawImageTile(pDC, rcDest, &dcSrc, rcSrc, bTile, clrTransparent);
		return;
	}

	if ((rcDest.Height() <= rcSizingMargins.top + rcSizingMargins.bottom) || (rcSrc.Height() == rcSizingMargins.top + rcSizingMargins.bottom))
	{
		rcDestSizingMargins.top = MulDiv(rcDest.Height(), rcSizingMargins.top, (rcSizingMargins.top + rcSizingMargins.bottom));
		rcDestSizingMargins.bottom = rcDest.Height() - rcDestSizingMargins.top;

		if (rcDestSizingMargins.bottom > 0 && (rcDestSizingMargins.bottom % 2 != rcSizingMargins.bottom % 2))
			rcDestSizingMargins.bottom++;


		if (bTile) bTile -= TILE_VERTICAL;
	}

	if ((rcDest.Width() <= rcSizingMargins.left + rcSizingMargins.right) || (rcSrc.Width() == rcSizingMargins.left + rcSizingMargins.right))
	{
		rcDestSizingMargins.left = MulDiv(rcDest.Width(), rcSizingMargins.left, (rcSizingMargins.left + rcSizingMargins.right));
		rcDestSizingMargins.right = rcDest.Width() - rcDestSizingMargins.left;

		if (bTile) bTile -= TILE_HORIZONTAL;
	}

	/*if ((rcDestSizingMargins.top +  rcDestSizingMargins.bottom <
		rcSizingMargins.top +  rcSizingMargins.bottom)
		&& rcDest.Width() < rcSrc.Width() + 3)
	{
		DrawImagePart(pDC, CRect(rcDest.left, rcDest.top, rcDest.right, rcDest.top + rcDestSizingMargins.top),
			&dcSrc, CRect(rcSrc.left, rcSrc.top , rcSrc.right, rcSrc.top + rcSizingMargins.top));

		DrawImagePart(pDC, CRect(rcDest.left, rcDest.bottom - rcDestSizingMargins.bottom, rcDest.right, rcDest.bottom),
			&dcSrc, CRect(rcSrc.left, rcSrc.bottom - rcSizingMargins.bottom , rcSrc.right, rcSrc.bottom ));

		return;
	}*/

	DrawImagePart(pDC, CRect(rcDest.left, rcDest.top, rcDest.left + rcDestSizingMargins.left, rcDest.top + rcDestSizingMargins.top),
		&dcSrc, CRect(rcSrc.left, rcSrc.top, rcSrc.left + rcSizingMargins.left, rcSrc.top + rcSizingMargins.top ));
	DrawImageTile(pDC, CRect(rcDest.left + rcDestSizingMargins.left, rcDest.top, rcDest.right - rcDestSizingMargins.right, rcDest.top + rcDestSizingMargins.top),
		&dcSrc, CRect(rcSrc.left + rcSizingMargins.left, rcSrc.top, rcSrc.right - rcSizingMargins.right, rcSrc.top + rcSizingMargins.top ), bTile, clrTransparent);
	DrawImagePart(pDC, CRect(rcDest.right - rcDestSizingMargins.right, rcDest.top, rcDest.right, rcDest.top + rcDestSizingMargins.top),
		&dcSrc, CRect(rcSrc.right - rcSizingMargins.right, rcSrc.top , rcSrc.right, rcSrc.top + rcSizingMargins.top ));

	DrawImageTile(pDC, CRect(rcDest.left, rcDest.top + rcDestSizingMargins.top, rcDest.left + rcDestSizingMargins.left, rcDest.bottom - rcDestSizingMargins.bottom),
		&dcSrc, CRect(rcSrc.left, rcSrc.top + rcSizingMargins.top , rcSrc.left + rcSizingMargins.left, rcSrc.bottom - rcSizingMargins.bottom ), bTile, clrTransparent);

	if (!bBorderOnly)
	{
		DrawImageTile(pDC, CRect(rcDest.left + rcDestSizingMargins.left, rcDest.top + rcDestSizingMargins.top, rcDest.right - rcDestSizingMargins.right, rcDest.bottom - rcDestSizingMargins.bottom),
			&dcSrc, CRect(rcSrc.left + rcSizingMargins.left, rcSrc.top + rcSizingMargins.top , rcSrc.right - rcSizingMargins.right, rcSrc.bottom - rcSizingMargins.bottom ), bTile, clrTransparent);
	}

	DrawImageTile(pDC, CRect(rcDest.right - rcDestSizingMargins.right , rcDest.top + rcDestSizingMargins.top, rcDest.right, rcDest.bottom - rcDestSizingMargins.bottom),
		&dcSrc, CRect(rcSrc.right - rcSizingMargins.right, rcSrc.top + rcSizingMargins.top , rcSrc.right, rcSrc.bottom - rcSizingMargins.bottom), bTile, clrTransparent);

	DrawImagePart(pDC, CRect(rcDest.left, rcDest.bottom - rcDestSizingMargins.bottom, rcDest.left + rcDestSizingMargins.left, rcDest.bottom),
		&dcSrc, CRect(rcSrc.left, rcSrc.bottom - rcSizingMargins.bottom , rcSrc.left + rcSizingMargins.left, rcSrc.bottom ));
	DrawImageTile(pDC, CRect(rcDest.left + rcDestSizingMargins.left, rcDest.bottom - rcDestSizingMargins.bottom, rcDest.right - rcDestSizingMargins.right, rcDest.bottom),
		&dcSrc, CRect(rcSrc.left + rcSizingMargins.left, rcSrc.bottom - rcSizingMargins.bottom  , rcSrc.right - rcSizingMargins.right, rcSrc.bottom ), bTile, clrTransparent);
	DrawImagePart(pDC, CRect(rcDest.right - rcDestSizingMargins.right, rcDest.bottom - rcDestSizingMargins.bottom, rcDest.right, rcDest.bottom),
		&dcSrc, CRect(rcSrc.right - rcSizingMargins.right, rcSrc.bottom - rcSizingMargins.bottom , rcSrc.right, rcSrc.bottom));

}

CSize CXTPSkinImage::GetExtent() const
{
	return m_szBitmap;
}

CSize CXTPSkinImage::_GetExtent() const
{
	if (!m_hBitmap)
		return CSize(0, 0);

	BITMAP bmpInfo;
	GetObject(m_hBitmap, sizeof(BITMAP), &bmpInfo);

	return CSize(bmpInfo.bmWidth, bmpInfo.bmHeight);
}


DWORD CXTPSkinImage::GetHeight() const
{
	return m_szBitmap.cy;
}

DWORD CXTPSkinImage::GetWidth() const
{
	return m_szBitmap.cx;
}

BOOL CXTPSkinImage::IsAlphaImage() const
{
	return m_bAlpha;
}

CRect CXTPSkinImage::GetSource(int nState, int nCount) const
{
	CSize szExtent = m_szBitmap;
	CRect rcImage(0, 0, szExtent.cx, szExtent.cy / nCount);
	rcImage.OffsetRect(0, nState * rcImage.Height());

	return rcImage;
}

//////////////////////////////////////////////////////////////////////////
//

CXTPSkinImages::CXTPSkinImages()
{

}

CXTPSkinImages::~CXTPSkinImages()
{
	RemoveAll();
}

void CXTPSkinImages::RemoveAll()
{
	POSITION pos = m_mapImages.GetStartPosition();
	CString strImageFile;
	CXTPSkinImage* pImage;
	while (pos != NULL)
	{
		m_mapImages.GetNextAssoc( pos, strImageFile, (void*&)pImage);
		delete pImage;
	}
	m_mapImages.RemoveAll();
}

CXTPSkinImage* CXTPSkinImages::LoadFile(CXTPSkinManagerResourceFile* pResourceFile, LPCTSTR lpszImageFile)
{
	if (!pResourceFile)
		return NULL;

	if (lpszImageFile == NULL)
		return NULL;

	CXTPSkinImage* pImage = 0;
	if (m_mapImages.Lookup(lpszImageFile, (void*&)pImage))
		return pImage;

	pImage = pResourceFile->LoadImage(lpszImageFile);
	if (!pImage)
		return NULL;

	m_mapImages.SetAt(lpszImageFile, pImage);
	return pImage;
}

CSize CXTPSkinImages::GetExtent(CXTPSkinManagerResourceFile* pResourceFile, LPCTSTR lpszImageFile)
{
	CXTPSkinImage* pImage = LoadFile(pResourceFile, lpszImageFile);

	if (!pImage)
		return CSize(0, 0);

	return pImage->GetExtent();
}
