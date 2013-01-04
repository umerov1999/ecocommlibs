// XTPResourceImage.cpp: implementation of the CXTPResourceImage class.
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

#include "XTPImageManager.h"
#include "XTPColorManager.h"
#include "XTPResourceManager.h"
#include "XTPDrawHelpers.h"
#include "XTPSystemHelpers.h"

#include "XTPVC80Helpers.h"

#include "XTPResourceImage.h"
#include "XTPNotifyConnection.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define TILE_VERTICAL     1
#define TILE_HORIZONTAL   2
#define TILE_ALL          3

//---------------------------------------------------------------------------
//    Should be a prime number:
// 37, 53, 79 , 101, 127, 199, 503, 1021, 1511, 2003, 3001, 4001, 5003, 6007, 8009, 12007, 16001, 32003, 48017, 64007
#define XTP_OFFICE2007_IMAGES_HASH_TABLE_SIZE 199

IMPLEMENT_DYNCREATE(CXTPResourceImageList, CCmdTarget)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXTPResourceImage::CXTPResourceImage(CXTPResourceImages* pImages)
{
	m_bAlpha = FALSE;
	m_bInvert = FALSE;
	m_bMultiply = FALSE;

	m_szExtent = CSize(0, 0);

	m_bImageOrientaionPreserved = FALSE;
	m_pImages = pImages;

	m_pBits = NULL;
	m_bFlipped = FALSE;
}

CXTPResourceImage::~CXTPResourceImage()
{
	if (m_pBits != NULL)
	{
		free(m_pBits);
	}
}

BOOL CXTPResourceImage::ConvertToBits(HBITMAP hBitmap)
{
	CDC dcSrc;
	dcSrc.CreateCompatibleDC(NULL);

	LPVOID pBits = NULL;

	if (m_bAlpha)
	{
		BITMAPINFO  bmi;
		memset(&bmi, 0, sizeof(bmi));
		bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);

		if (GetDIBits(dcSrc, hBitmap, 0, 0, NULL, (BITMAPINFO*)&bmi, DIB_RGB_COLORS) == 0)
			return FALSE;

		if (bmi.bmiHeader.biBitCount != 32)
			return FALSE;

		int nSize = bmi.bmiHeader.biHeight * bmi.bmiHeader.biWidth * 4;
		pBits = malloc(nSize);
		if (pBits == NULL)
			return FALSE;

		PBITMAPINFO pBitmapInfo = (PBITMAPINFO)malloc(sizeof(BITMAPINFOHEADER) + 3 * sizeof(COLORREF));
		if (pBitmapInfo == NULL)
		{
			free(pBits);
			return FALSE;
		}

		MEMCPY_S(pBitmapInfo, &bmi, sizeof(BITMAPINFOHEADER));

		if (GetDIBits(dcSrc, hBitmap, 0, bmi.bmiHeader.biHeight, pBits, pBitmapInfo, DIB_RGB_COLORS) == NULL)
		{
			free(pBits);
			free(pBitmapInfo);
			return FALSE;
		}

		free(pBitmapInfo);
	}
	else
	{
		LPDWORD lpBits = NULL;
		HBITMAP hDCBitmap = CXTPImageManager::Create32BPPDIBSection(NULL, m_szExtent.cx, m_szExtent.cy, (LPBYTE*)&lpBits);
		if (!lpBits)
			return FALSE;

		HBITMAP hOldBitmap = (HBITMAP)::SelectObject(dcSrc, hDCBitmap);

		dcSrc.DrawState(CPoint(0, 0), m_szExtent, hBitmap, DSS_NORMAL, 0);

		::SelectObject(dcSrc, hOldBitmap);

		int nSize = m_szExtent.cx * m_szExtent.cy * 4;

		pBits = malloc(nSize);
		if (!pBits)
			return FALSE;

		MEMCPY_S(pBits, lpBits, nSize);

		DeleteObject(hDCBitmap);
	}

	ASSERT(pBits);

	m_pBits = pBits;

	return TRUE;
}

void CXTPResourceImage::SetBitmap(HBITMAP hBitmap, BOOL bAlpha /*= FALSE*/)
{
	ASSERT(m_pBits == NULL);

	m_bAlpha = bAlpha;
	m_bInvert = FALSE;
	m_bMultiply = FALSE;

	BITMAP bmpInfo;
	if (!GetObject(hBitmap, sizeof(BITMAP), &bmpInfo))
		return;

	m_szExtent = CSize(bmpInfo.bmWidth, bmpInfo.bmHeight);

	VERIFY(ConvertToBits(hBitmap));

	DeleteObject(hBitmap);
}

BOOL CXTPResourceImage::LoadFile(HMODULE hModule, LPCTSTR lpszBitmapFileName)
{
	m_bMultiply = FALSE;
	m_bInvert = FALSE;
	m_bAlpha = FALSE;

	if (m_pBits)
	{
		free(m_pBits);
		m_pBits = NULL;
	}

	if (!hModule)
		return FALSE;

	BOOL bAlpha = FALSE;

	HBITMAP hBmp = CXTPImageManagerIcon::LoadBitmapFromResource(hModule, lpszBitmapFileName, &bAlpha);

	if (!hBmp)
		return FALSE;

	SetBitmap(hBmp, bAlpha);

	return TRUE;
}

void CXTPResourceImage::Premultiply()
{
	if (!m_bAlpha || !m_pBits || m_bMultiply)
		return;

	RGBQUAD* lprgbBitmap = (RGBQUAD*)m_pBits;
	int nCount = m_szExtent.cx * m_szExtent.cy;

	for (int i = 0; i < nCount; i++)
	{
		int nAlpha = lprgbBitmap->rgbReserved;
		lprgbBitmap->rgbBlue = BYTE(lprgbBitmap->rgbBlue * nAlpha / 255);
		lprgbBitmap->rgbGreen = BYTE(lprgbBitmap->rgbGreen * nAlpha / 255);
		lprgbBitmap->rgbRed = BYTE(lprgbBitmap->rgbRed * nAlpha / 255);
		lprgbBitmap++;
	}

	m_bMultiply = TRUE;
}

void CXTPResourceImage::DrawImageFlipped(BOOL bFlip)
{
	if (m_bFlipped == bFlip)
		return;

	m_bFlipped = bFlip;

	InvertBitmap();
	m_bInvert = !m_bInvert; // restore.
}

void CXTPResourceImage::InvertBitmap()
{
	if (!m_pBits)
		return;

	DWORD* pDest = (DWORD*)malloc(m_szExtent.cx * m_szExtent.cy * 4);
	DWORD* pDestOrig = pDest;
	DWORD* pBits = (DWORD*)m_pBits;

	for (int y = 0; y < m_szExtent.cy; y++)
	{
		DWORD* pBitsRow = &pBits[y * m_szExtent.cx + m_szExtent.cx - 1];

		for (int x = 0; x < m_szExtent.cx; x++)
		{
			*pDest++ = *pBitsRow--;
		}
	}

	free(m_pBits);
	m_pBits = pDestOrig;

	m_bInvert = !m_bInvert;
}

BOOL CXTPResourceImage::DrawImagePart(CDC* pDCDest, CRect rcDest, CDC* pDCSrc, CRect rcSrc) const
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

void CXTPResourceImage::DrawImage(CDC* pDC, const CRect& rcDest, CRect rcSrc, const CRect& rcSizingMargins, COLORREF clrTransparent, DWORD dwFlags)
{
	if (this == NULL)
		return;

	if (m_pBits == NULL)
		return;

	if (rcSizingMargins.top + rcSizingMargins.bottom > rcSrc.Height())
		return;

	if ((dwFlags & XTP_DI_COMPOSITE) == 0)
	{
		DrawImage(pDC, rcDest, rcSrc, rcSizingMargins, clrTransparent);
		return;
	}

	if (m_bAlpha)
	{
		DrawImage(pDC, rcDest, rcSrc, rcSizingMargins);
		return;
	}

	CSize sz(rcDest.Width(), rcDest.Height());
	CBitmap bmp;
	LPDWORD lpBits;
	bmp.Attach(CXTPImageManager::Create32BPPDIBSection(pDC->GetSafeHdc(), sz.cx, sz.cy, (LPBYTE*)&lpBits));

	CXTPCompatibleDC dc(pDC, &bmp);

	CRect rcDestOrig(0, 0, sz.cx, sz.cy);

	if (clrTransparent != COLORREF_NULL) dc.FillSolidRect(rcDestOrig, clrTransparent);
	DrawImage(&dc, rcDestOrig, rcSrc, rcSizingMargins);

	for (int i = 0; i < sz.cx * sz.cy; i++)
	{
		if (clrTransparent != COLORREF_NULL && lpBits[0] == clrTransparent)
		{
			lpBits[0] = 0;
		}
		else
		{
			lpBits[0] |= 0xFF000000;
		}
		lpBits++;
	}

	XTPImageManager()->AlphaBlend(*pDC, rcDest, dc, rcDestOrig);
}

void CXTPResourceImage::DrawImage(CDC* pDC, const CRect& rcDest, CRect rcSrc, const CRect& rcSizingMargins, COLORREF clrTransparent)
{
	if (this == NULL)
		return;

	if (m_pBits == NULL)
		return;

	if (rcSizingMargins.top + rcSizingMargins.bottom > rcSrc.Height())
		return;

	if (clrTransparent == COLORREF_NULL || m_bAlpha)
	{
		DrawImage(pDC, rcDest, rcSrc, rcSizingMargins);
		return;
	}

	CBitmap bmp;
	bmp.CreateCompatibleBitmap(pDC, rcDest.Width(), rcDest.Height());

	CXTPCompatibleDC dc(pDC, &bmp);
	//dc.SetLayout(0);

	CRect rcDestOrig(0, 0, rcDest.Width(), rcDest.Height());

	dc.FillSolidRect(rcDestOrig, clrTransparent);
	DrawImage(&dc, rcDestOrig, rcSrc, rcSizingMargins);

	XTPImageManager()->TransparentBlt(*pDC, rcDest, dc, rcDestOrig, clrTransparent);

}

void CXTPResourceImage::DrawImage(CDC* pDC, const CRect& rcDest, CRect rcSrc)
{
	DrawImage(pDC, rcDest, rcSrc, CRect(0, 0, 0, 0));
}

BOOL CXTPResourceImage::GetBitmapPixel(CPoint pt, COLORREF& clrPixel)
{
	if (pt.x < 0 || pt.x >= m_szExtent.cx ||
		pt.y < 0 || pt.y >= m_szExtent.cy)
		return FALSE;

	// Initialize the surface pointers.
	LPDWORD lprgbBitmap = (LPDWORD)m_pBits + m_szExtent.cx * (m_szExtent.cy - pt.y - 1);
	clrPixel = lprgbBitmap[pt.x];

	return TRUE;
}

void CXTPResourceImage::DrawImage(CDC* pDC, const CRect& rcDest, CRect rcSrc, CRect rcSizingMargins)
{
	if (this == NULL)
		return;

	if (m_pBits == NULL)
		return;

	if (rcSizingMargins.top + rcSizingMargins.bottom > rcSrc.Height())
		return;

	ASSERT(m_pImages);
	if (!m_pImages)
		return;

	CXTPLockGuard lock(m_pImages->m_cs);

	if (rcDest.Width() == rcSrc.Width())
	{
		rcSizingMargins.left = 0;
		rcSizingMargins.right = 0;
	}

	if (rcDest.Height() == rcSrc.Height())
	{
		rcSizingMargins.top = 0;
		rcSizingMargins.bottom = 0;
	}

	pDC->SetStretchBltMode(COLORONCOLOR);

	CRect rcDestSizingMargins = rcSizingMargins;

	if (rcDest.left >= rcDest.right || rcDest.top >= rcDest.bottom)
		return;

	BOOL bRtl = CXTPDrawHelpers::IsContextRTL(pDC);

	if (m_bAlpha && m_bImageOrientaionPreserved)
	{
		if ((bRtl && !m_bInvert) || (!bRtl && m_bInvert))
		{
			InvertBitmap();
		}
	}

	if (m_szExtent.cx > 512 || m_szExtent.cy > 512)
	{
		ASSERT(FALSE);
		return;
	}

	CXTPResourceImages::CBitmapDC& dcSrc = m_pImages->m_dc;
	if (!dcSrc.GetSafeHdc() || ! m_pImages->m_pLayerBits)
		return;

	LPDWORD lpBitsSrc = ((LPDWORD)m_pBits) + (m_szExtent.cy - rcSrc.bottom) * m_szExtent.cx;
	LPDWORD lpBitsDest = m_pImages->m_pLayerBits + 512 * (512 - rcSrc.Height());
	rcSrc.OffsetRect(0, -rcSrc.top);

	for (int y = 0; y < rcSrc.bottom; y++)
	{
		MEMCPY_S(lpBitsDest, lpBitsSrc, m_szExtent.cx * 4);
		lpBitsSrc += m_szExtent.cx;
		lpBitsDest += 512;
	}

	dcSrc.SetBitmap(m_pImages->m_hbmLayer);

	if ((rcDest.Height() <= rcSizingMargins.top + rcSizingMargins.bottom) || (rcSrc.Height() == rcSizingMargins.top + rcSizingMargins.bottom))
	{
		rcDestSizingMargins.top = MulDiv(rcDest.Height(), rcSizingMargins.top, (rcSizingMargins.top + rcSizingMargins.bottom));
		rcDestSizingMargins.bottom = rcDest.Height() - rcDestSizingMargins.top;

		if (rcDestSizingMargins.bottom > 0 && (rcDestSizingMargins.bottom % 2 != rcSizingMargins.bottom % 2))
			rcDestSizingMargins.bottom++;
	}

	if ((rcDest.Width() <= rcSizingMargins.left + rcSizingMargins.right) || (rcSrc.Width() == rcSizingMargins.left + rcSizingMargins.right))
	{
		rcDestSizingMargins.left= rcDest.Width() * rcSizingMargins.left / (rcSizingMargins.left + rcSizingMargins.right);
		rcDestSizingMargins.right = rcDest.Width() - rcDestSizingMargins.left;
	}

	if (rcSizingMargins.IsRectNull())
	{
		DrawImagePart(pDC, rcDest, &dcSrc, rcSrc);
	}
	else if (bRtl && m_bAlpha)
	{
		DrawImagePart(pDC, CRect(rcDest.right - rcDestSizingMargins.left, rcDest.top, rcDest.right, rcDest.top + rcDestSizingMargins.top),
			&dcSrc, CRect(rcSrc.left, rcSrc.top, rcSrc.left + rcSizingMargins.left, rcSrc.top + rcSizingMargins.top ));
		DrawImagePart(pDC, CRect(rcDest.left + rcDestSizingMargins.right, rcDest.top, rcDest.right - rcDestSizingMargins.left, rcDest.top + rcDestSizingMargins.top),
			&dcSrc, CRect(rcSrc.left + rcSizingMargins.left, rcSrc.top, rcSrc.right - rcSizingMargins.right, rcSrc.top + rcSizingMargins.top ));
		DrawImagePart(pDC, CRect(rcDest.left, rcDest.top, rcDest.left + rcDestSizingMargins.right, rcDest.top + rcDestSizingMargins.top),
			&dcSrc, CRect(rcSrc.right - rcSizingMargins.right, rcSrc.top , rcSrc.right, rcSrc.top + rcSizingMargins.top ));

		DrawImagePart(pDC, CRect(rcDest.right - rcDestSizingMargins.left, rcDest.top + rcDestSizingMargins.top, rcDest.right, rcDest.bottom - rcDestSizingMargins.bottom),
			&dcSrc, CRect(rcSrc.left, rcSrc.top + rcSizingMargins.top , rcSrc.left + rcSizingMargins.left, rcSrc.bottom - rcSizingMargins.bottom ));

		DrawImagePart(pDC, CRect(rcDest.left + rcDestSizingMargins.right, rcDest.top + rcDestSizingMargins.top, rcDest.right - rcDestSizingMargins.left, rcDest.bottom - rcDestSizingMargins.bottom),
			&dcSrc, CRect(rcSrc.left + rcSizingMargins.left, rcSrc.top + rcSizingMargins.top , rcSrc.right - rcSizingMargins.right, rcSrc.bottom - rcSizingMargins.bottom ));

		DrawImagePart(pDC, CRect(rcDest.left, rcDest.top + rcDestSizingMargins.top, rcDest.left + rcDestSizingMargins.right , rcDest.bottom - rcDestSizingMargins.bottom),
			&dcSrc, CRect(rcSrc.right - rcSizingMargins.right, rcSrc.top + rcSizingMargins.top , rcSrc.right, rcSrc.bottom - rcSizingMargins.bottom));

		DrawImagePart(pDC, CRect(rcDest.right - rcDestSizingMargins.left, rcDest.bottom - rcDestSizingMargins.bottom, rcDest.right, rcDest.bottom),
			&dcSrc, CRect(rcSrc.left, rcSrc.bottom - rcSizingMargins.bottom , rcSrc.left + rcSizingMargins.left, rcSrc.bottom ));
		DrawImagePart(pDC, CRect(rcDest.left + rcDestSizingMargins.right, rcDest.bottom - rcDestSizingMargins.bottom, rcDest.right - rcDestSizingMargins.left, rcDest.bottom),
			&dcSrc, CRect(rcSrc.left + rcSizingMargins.left, rcSrc.bottom - rcSizingMargins.bottom  , rcSrc.right - rcSizingMargins.right, rcSrc.bottom ));
		DrawImagePart(pDC, CRect(rcDest.left, rcDest.bottom - rcDestSizingMargins.bottom, rcDest.left + rcDestSizingMargins.right, rcDest.bottom),
			&dcSrc, CRect(rcSrc.right - rcSizingMargins.right, rcSrc.bottom - rcSizingMargins.bottom , rcSrc.right, rcSrc.bottom));
	}
	else
	{
		DrawImagePart(pDC, CRect(rcDest.left, rcDest.top, rcDest.left + rcDestSizingMargins.left, rcDest.top + rcDestSizingMargins.top),
			&dcSrc, CRect(rcSrc.left, rcSrc.top, rcSrc.left + rcSizingMargins.left, rcSrc.top + rcSizingMargins.top ));
		DrawImagePart(pDC, CRect(rcDest.left + rcDestSizingMargins.left, rcDest.top, rcDest.right - rcDestSizingMargins.right, rcDest.top + rcDestSizingMargins.top),
			&dcSrc, CRect(rcSrc.left + rcSizingMargins.left, rcSrc.top, rcSrc.right - rcSizingMargins.right, rcSrc.top + rcSizingMargins.top ));
		DrawImagePart(pDC, CRect(rcDest.right - rcDestSizingMargins.right, rcDest.top, rcDest.right, rcDest.top + rcDestSizingMargins.top),
			&dcSrc, CRect(rcSrc.right - rcSizingMargins.right, rcSrc.top , rcSrc.right, rcSrc.top + rcSizingMargins.top ));

		DrawImagePart(pDC, CRect(rcDest.left, rcDest.top + rcDestSizingMargins.top, rcDest.left + rcDestSizingMargins.left, rcDest.bottom - rcDestSizingMargins.bottom),
			&dcSrc, CRect(rcSrc.left, rcSrc.top + rcSizingMargins.top , rcSrc.left + rcSizingMargins.left, rcSrc.bottom - rcSizingMargins.bottom ));

		DrawImagePart(pDC, CRect(rcDest.left + rcDestSizingMargins.left, rcDest.top + rcDestSizingMargins.top, rcDest.right - rcDestSizingMargins.right, rcDest.bottom - rcDestSizingMargins.bottom),
			&dcSrc, CRect(rcSrc.left + rcSizingMargins.left, rcSrc.top + rcSizingMargins.top , rcSrc.right - rcSizingMargins.right, rcSrc.bottom - rcSizingMargins.bottom ));

		DrawImagePart(pDC, CRect(rcDest.right - rcDestSizingMargins.right , rcDest.top + rcDestSizingMargins.top, rcDest.right, rcDest.bottom - rcDestSizingMargins.bottom),
			&dcSrc, CRect(rcSrc.right - rcSizingMargins.right, rcSrc.top + rcSizingMargins.top , rcSrc.right, rcSrc.bottom - rcSizingMargins.bottom));

		DrawImagePart(pDC, CRect(rcDest.left, rcDest.bottom - rcDestSizingMargins.bottom, rcDest.left + rcDestSizingMargins.left, rcDest.bottom),
			&dcSrc, CRect(rcSrc.left, rcSrc.bottom - rcSizingMargins.bottom , rcSrc.left + rcSizingMargins.left, rcSrc.bottom ));
		DrawImagePart(pDC, CRect(rcDest.left + rcDestSizingMargins.left, rcDest.bottom - rcDestSizingMargins.bottom, rcDest.right - rcDestSizingMargins.right, rcDest.bottom),
			&dcSrc, CRect(rcSrc.left + rcSizingMargins.left, rcSrc.bottom - rcSizingMargins.bottom  , rcSrc.right - rcSizingMargins.right, rcSrc.bottom ));
		DrawImagePart(pDC, CRect(rcDest.right - rcDestSizingMargins.right, rcDest.bottom - rcDestSizingMargins.bottom, rcDest.right, rcDest.bottom),
			&dcSrc, CRect(rcSrc.right - rcSizingMargins.right, rcSrc.bottom - rcSizingMargins.bottom , rcSrc.right, rcSrc.bottom));
	}

	dcSrc.SetBitmap(NULL);
}

CSize CXTPResourceImage::GetExtent() const
{
	return m_szExtent;
}

CRect CXTPResourceImage::GetSource(int nState, int nCount) const
{
	if (this == NULL)
		return CRect(0, 0, 0, 0);

	CRect rcImage(0, 0, m_szExtent.cx, m_szExtent.cy / nCount);
	rcImage.OffsetRect(0, nState * rcImage.Height());

	return rcImage;
}

int CXTPResourceImage::GetHeight() const
{
	return m_szExtent.cy;
}

int CXTPResourceImage::GetWidth() const
{
	return m_szExtent.cx;
}

BOOL CXTPResourceImage::IsAlphaImage() const
{
	return m_bAlpha;
}

//////////////////////////////////////////////////////////////////////////
CXTPResourceImages::CXTPResourceImages()
{
	m_mapImages.InitHashTable(XTP_OFFICE2007_IMAGES_HASH_TABLE_SIZE, FALSE);
	m_mapPropeties.InitHashTable(101, FALSE);

	m_bFreeOnRelease = FALSE;
	m_hResource = 0;
	m_lpTextFile = 0;
	m_lpTextFileEnd = 0;
	m_pConnection = NULL;
	m_bPremultiplyImages = FALSE;

	SetHandle(AfxGetInstanceHandle());

	m_pLayerBits = NULL;
	m_hbmLayer = CXTPImageManager::Create32BPPDIBSection(NULL, 512, 512, (LPBYTE*)&m_pLayerBits);

	HDC hdcScreen = ::GetDC(HWND_DESKTOP);
	m_dc.Attach(::CreateCompatibleDC(hdcScreen));
	ReleaseDC(HWND_DESKTOP, hdcScreen);

	::InitializeCriticalSection(&m_cs);


}

CXTPResourceImages* AFX_CDECL XTPResourceImages()
{
	static CXTPResourceImages images;
	return &images;
}

CXTPResourceImages::~CXTPResourceImages()
{
	CMDTARGET_RELEASE(m_pConnection);

	RemoveAll();

	if (m_hResource && m_bFreeOnRelease)
	{
		FreeLibrary(m_hResource);
	}

	if (m_hbmLayer)
	{
		DeleteObject(m_hbmLayer);
	}

	::DeleteCriticalSection(&m_cs);
}

CXTPNotifyConnection* CXTPResourceImages::GetConnection()
{
	if (!m_pConnection)
		m_pConnection = new CXTPNotifyConnection();

	return m_pConnection;
}

void CXTPResourceImages::RemoveAll()
{
	CString strImageFile;
	CXTPResourceImage* pImage;

	POSITION pos = m_mapImages.GetStartPosition();
	while (pos != NULL)
	{
		m_mapImages.GetNextAssoc( pos, strImageFile, (void*&)pImage);
		CMDTARGET_RELEASE(pImage);
	}
	m_mapImages.RemoveAll();
}

CString CXTPResourceImages::_ImageNameToResourceName(LPCTSTR lpszImageFile)
{
	CString strImage = GetImageString(_T("Images"), lpszImageFile);

	if (strImage.IsEmpty())
	{
		strImage = lpszImageFile;
	}

	strImage.Replace(_T('.'), _T('_'));
	strImage.Replace(_T('\\'), _T('_'));

	return strImage;
}

CXTPResourceImage* CXTPResourceImages::LoadImage(LPCTSTR lpszImageFile)
{
	CString strImageFile = _ImageNameToResourceName(lpszImageFile);

	CXTPResourceImage* pImage = new CXTPResourceImage(this);

	if (!pImage->LoadFile(m_hResource, strImageFile))
	{
		delete pImage;
		return NULL;
	}

	return pImage;
}

CXTPResourceImage* CXTPResourceImages::LoadFile(LPCTSTR lpszImageFile)
{
	CXTPLockGuard lock(m_cs);

	if (!m_hResource)
		return NULL;

	if (lpszImageFile == NULL)
		return NULL;

	CXTPResourceImage* pImage = 0;
	if (m_mapImages.Lookup(lpszImageFile, (void*&)pImage))
		return pImage;

	pImage = LoadImage(lpszImageFile);
	if (!pImage)
	{
		m_mapImages.SetAt(lpszImageFile, NULL);
		return NULL;
	}

	if (m_bPremultiplyImages)
	{
		pImage->Premultiply();
	}

	m_mapImages.SetAt(lpszImageFile, pImage);

	return pImage;
}

BOOL CXTPResourceImages::LoadResources()
{
	RemoveAll();

	HRSRC hRsrc = 0;

	if (!m_strIniFileName.IsEmpty())
	{
		hRsrc = FindResource(m_hResource, m_strIniFileName, _T("TEXTFILE"));
	}
	else
	{
		hRsrc = FindResource(m_hResource, _T("OFFICE2007BLUE_INI"), _T("TEXTFILE"));

		if (hRsrc)
		{
			m_strIniFileName =  _T("OFFICE2007BLUE_INI");
		}
	}

	if (!hRsrc && m_strIniFileName.IsEmpty())
	{
		hRsrc = FindResourceIniFile(m_hResource);
	}

	BOOL bResult = FALSE;
	if (hRsrc)
	{
		HGLOBAL hResData = LoadResource(m_hResource, hRsrc);
		if (hResData)
		{
			m_lpTextFile = (LPSTR)LockResource(hResData);
			m_lpTextFileEnd = m_lpTextFile + SizeofResource(m_hResource, hRsrc);

			bResult = TRUE;
		}
	}

	LoadPropeties();

	if (m_pConnection)
		m_pConnection->SendEvent(XTP_NC_COMMON_RESOURCEIMAGES_CHANGED, 0, 0);

	m_bPremultiplyImages = GetImageInt(_T("Global"), _T("PremultiplyImages"), 0);

	return bResult;
}

BOOL CXTPResourceImages::SetHandle(LPCTSTR lpszDllFileName, LPCTSTR lpszIniFileName)
{
	CString strDllFileName(lpszDllFileName);
	CString strIniFileName(lpszIniFileName);

	if (!lpszIniFileName)
	{
		int nIndex = strDllFileName.Find(_T(','));
		if (nIndex != -1)
		{
			strIniFileName = strDllFileName.Mid(nIndex + 1);
			strDllFileName = strDllFileName.Left(nIndex);
		}
	}

	HMODULE hModule = LoadLibrary(strDllFileName);

	if (hModule)
	{
		m_strDllFileName = strDllFileName;
		return SetHandle(hModule, strIniFileName, TRUE);
	}

	return SetHandle(AfxGetInstanceHandle(), strIniFileName);
}

void CXTPResourceImages::AssertValid()
{
	if (IsValid())
		return;

	XTP_ERROR_MSG(
		"Error Locating Office2007 Resources.\n\n"
		"Make sure you have included the resources to your project's .rc2 file. For example to provide\n"
		"support for Office 2007 Black Visual Style by adding the following line to your project .rc2 file:\n\n"
		"#include <Styles/Office2007Black/Office2007Black.rc> to your project .rc2 file.\n\n"
		"Giving you access to the Office 2007 resources.");
}

BOOL CXTPResourceImages::IsValid() const
{
	return m_lpTextFile != NULL;
}

BOOL CALLBACK CXTPResourceImages::EnumResNameProc(HMODULE /*hModule*/,
	LPCTSTR /*lpszType*/, LPTSTR lpszName, LONG_PTR lParam)
{
	CString* pIniFileName = (CString*)lParam;
	*pIniFileName = lpszName;

	return FALSE;
}

HRSRC CXTPResourceImages::FindResourceIniFile(HMODULE hResource)
{
	CString strIniFileName;
	EnumResourceNames(hResource, _T("TEXTFILE"), &CXTPResourceImages::EnumResNameProc, (LONG_PTR)&strIniFileName);

	if (strIniFileName.IsEmpty())
		return 0;

	return FindResource(m_hResource, strIniFileName, _T("TEXTFILE"));
}

BOOL CXTPResourceImages::SetHandle(HMODULE hResource, LPCTSTR lpszIniFileName, BOOL bFreeOnRelease)
{
	if (m_hResource && m_bFreeOnRelease)
	{
		FreeLibrary(m_hResource);
	}

	m_strIniFileName = lpszIniFileName;

	m_strIniFileName.Replace(_T('.'), _T('_'));

	m_hResource = hResource;
	m_bFreeOnRelease = bFreeOnRelease;

	m_lpTextFile = m_lpTextFileEnd = NULL;

	return LoadResources();
}

BOOL CXTPResourceImages::InitResourceHandle(LPCTSTR lpszTestImageFile, LPCTSTR lpResType)
{
	CString strResourceName = _ImageNameToResourceName(lpszTestImageFile);

	HMODULE hResource = XTPResourceManager()->GetResourceHandle();
	if (hResource == 0)
		hResource = AfxGetInstanceHandle();

	if (!::FindResource(hResource, strResourceName, lpResType))
	{
		hResource = AfxFindResourceHandle(strResourceName, lpResType);
		if (!hResource)
			return FALSE;
	}

	SetHandle(hResource);
	return TRUE;
}

BOOL CXTPResourceImages::ReadString(CString& str, LPSTR& lpTextFile)
{
	ASSERT(lpTextFile);
	if (!lpTextFile)
		return FALSE;

	LPSTR pNextLine = strchr(lpTextFile, '\r');

	if (pNextLine == NULL || pNextLine > m_lpTextFileEnd)
		return FALSE;

	str = CString(lpTextFile, (int)(pNextLine - lpTextFile));
	lpTextFile = pNextLine + 1;
	if (lpTextFile[0] == '\n') lpTextFile = lpTextFile + 1;

	return TRUE;
}

COLORREF CXTPResourceImages::GetImageColor(LPCTSTR lpszSectionName, LPCTSTR lpszKeyName, COLORREF clrDefault) const
{
	CString strValue = GetImageValue(lpszSectionName, lpszKeyName);
	if (strValue.IsEmpty())
		return clrDefault;

	int r = 0, g = 0, b = 0;
	SCANF_S(strValue, _T("%i %i %i"), &r, &g, &b);
	COLORREF clrVal = RGB(r, g, b);
	return clrVal;
}

CRect CXTPResourceImages::GetImageRect(LPCTSTR lpszSectionName, LPCTSTR lpszKeyName, CRect rcDefault) const
{
	CString strValue = GetImageValue(lpszSectionName, lpszKeyName);
	if (strValue.IsEmpty())
		return rcDefault;

	int l = 0, r = 0, t = 0, b = 0;

	SCANF_S(strValue, _T("%i %i %i %i"), &l, &t, &r, &b);

	return CRect(l, t, r, b);
}

int CXTPResourceImages::GetImageInt(LPCTSTR lpszSectionName, LPCTSTR lpszKeyName, int nDefault) const
{
	CString strValue = GetImageValue(lpszSectionName, lpszKeyName);
	if (strValue.IsEmpty())
		return nDefault;

	return _ttoi(strValue);
}

CString CXTPResourceImages::GetImageString(LPCTSTR lpszSectionName, LPCTSTR lpszKeyName) const
{
	return GetImageValue(lpszSectionName, lpszKeyName);
}

UINT CXTPResourceImages::GetKeyHash(LPCTSTR lpszSection, LPCTSTR lpszKeyName) const
{
	if (!lpszSection || !lpszKeyName)
		return 0;

	UINT nHash = 0;

	int i;

	for (i = 0; lpszSection[i] != 0; i++)
	{
		int c = (int)_totupper(lpszSection[i]);
		nHash = (nHash << 5) + nHash + c;
	}

	for (i = 0; lpszKeyName[i] != 0; i++)
	{
		int c = (int)_totupper(lpszKeyName[i]);
		nHash = (nHash << 5) + nHash + c;
	}

	return nHash;
}

CString CXTPResourceImages::GetImageValue(LPCTSTR lpszSectionName, LPCTSTR lpszKeyName) const
{
	if (m_lpTextFile == NULL)
		return _T("");

	UINT nHash = GetKeyHash(lpszSectionName, lpszKeyName);

	CString strValue;

	if (m_mapPropeties.Lookup(nHash, strValue))
		return strValue;

	return _T("");
}

void CXTPResourceImages::LoadPropeties()
{
	m_mapPropeties.RemoveAll();

	if (!m_lpTextFile)
		return;

	CString str;

	LPSTR lpTextFile = m_lpTextFile;

	CString strSection;

	while (ReadString(str, lpTextFile))
	{
		str.TrimLeft();

		int nCommentIndex = str.Find(_T(';'));

		if (nCommentIndex == 0)
			continue;

		if (nCommentIndex > 0)
		{
			str = str.Left(nCommentIndex);
		}

		if (str.IsEmpty())
			continue;

		if (str[0] == _T('['))
		{
			str.TrimRight();

			strSection = str.Mid(1, str.GetLength() - 2);
		}
		else if (!strSection.IsEmpty())
		{
			int nPos = str.Find(_T('='));
			if (nPos <= 0)
				continue;

			CString strProperty = str.Left(nPos);
			strProperty.TrimRight();


			CString strValue = str.Mid(nPos + 1);
			strValue.TrimLeft();
			strValue.TrimRight();

			UINT nHash = GetKeyHash(strSection, strProperty);

#ifdef _DEBUG
			CString strOldValue;
			ASSERT(!m_mapPropeties.Lookup(nHash, strOldValue));
#endif
			m_mapPropeties[nHash] = strValue;
		}
	}
}

//////////////////////////////////////////////////////////////////////////
CXTPResourceImageList::CXTPResourceImageList()
{
}

CXTPResourceImageList::~CXTPResourceImageList()
{
	RemoveAll();
}

CXTPResourceImage* CXTPResourceImageList::SetBitmap(HBITMAP hBitmap, UINT nID, BOOL bAlptha, BOOL bCopyBitmap)
{
	ASSERT(hBitmap);
	if (!hBitmap)
		return NULL;

	HBITMAP hBmp2 = hBitmap;
	if (bCopyBitmap)
	{
		// HBITMAP CXTPImageManagerIcon::CopyAlphaBitmap(HBITMAP hBitmap)
		hBmp2 = (HBITMAP)::CopyImage(hBitmap, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);
		ASSERT(hBmp2);
		if (!hBmp2)
			return NULL;
	}

	BITMAP bmpinfo;
	if (!::GetObject(hBmp2, sizeof(bmpinfo), &bmpinfo))
	{
		ASSERT(FALSE);
		::DeleteObject(hBmp2);
		return NULL;
	}

	CXTPResourceImage* pImage = new CXTPResourceImage(this);
	if (!pImage)
		return NULL;

	pImage->SetBitmap(hBmp2, bAlptha);

	SetBitmap(pImage, nID, FALSE);

	return pImage;
}

BOOL CXTPResourceImageList::SetBitmap(CXTPResourceImage* pImage, UINT nID, BOOL bCallAddRef)
{
	ASSERT(pImage);
	if (!pImage)
		return FALSE;

	if (bCallAddRef)
		CMDTARGET_ADDREF(pImage);

	// free prev. image if such exists
	CXTPResourceImage* pImage_old = GetBitmap(nID);
	CMDTARGET_RELEASE(pImage_old);

	m_mapID2Image.SetAt(nID, pImage);

	return TRUE;
}

CXTPResourceImage* CXTPResourceImageList::GetBitmap(UINT nID)
{
	CXTPResourceImage* pImage = NULL;
	if (!m_mapID2Image.Lookup(nID, pImage))
		return NULL;

	return pImage;
}

BOOL CXTPResourceImageList::Remove(UINT nID)
{
	CString strImageFile;
	CXTPResourceImage* pImage = NULL;

	//-------------------------------------------------
	if (!m_mapID2Image.Lookup(nID, pImage))
		return FALSE;

	m_mapID2Image.RemoveKey(nID);
	CMDTARGET_RELEASE(pImage);

	return TRUE;
}

void CXTPResourceImageList::RemoveAll()
{
	UINT nID;
	CXTPResourceImage* pImage;

	POSITION pos = m_mapID2Image.GetStartPosition();
	while (pos != NULL)
	{
		m_mapID2Image.GetNextAssoc( pos, nID, pImage);
		CMDTARGET_RELEASE(pImage);
	}

	m_mapID2Image.RemoveAll();

	m_mapPropeties.RemoveAll();
}

BOOL CXTPResourceImageList::LoadBitmap(LPCTSTR lpcszPath, UINT nID)
{
	BOOL bAlphaBitmap = FALSE;
	HBITMAP hBmp = CXTPImageManagerIcon::LoadBitmapFromFile(lpcszPath, &bAlphaBitmap);

	CXTPResourceImage* pImage = NULL;
	if (hBmp)
	{
		pImage = SetBitmap(hBmp, nID, bAlphaBitmap, FALSE);
	}

	return !!pImage;
}

/////////////////////////////////////////////////////////////////////////////
