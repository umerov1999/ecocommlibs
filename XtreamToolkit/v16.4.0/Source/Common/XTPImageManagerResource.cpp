#include <StdAfx.h>
#include <Common/XTPDrawHelpers.h>
#include <Common/XTPSystemHelpers.h>
#include <Common/XTPImageManager.h>
#include <Common/XTPImageManagerResource.h>

//////////////////////////////////////////////////////////////////////////
// CXTPImageManagerResource

CXTPImageManagerResource::CXTPImageManagerResource(CXTPImageManager* pImageManager, CSize szIcon)
{
	m_pImageManager = pImageManager;
	m_szIcon = szIcon;
	m_pBits = NULL;
	m_nAllocWidth = 0;

	m_hbmpImage = 0;
	m_hbmpMask = 0;

	HDC hdcScreen = ::GetDC(HWND_DESKTOP);
	m_dc.Attach(::CreateCompatibleDC(hdcScreen));
	ReleaseDC(HWND_DESKTOP, hdcScreen);

	::InitializeCriticalSection(&m_cs);
}

CXTPImageManagerResource::~CXTPImageManagerResource()
{
	RemoveAll();

	::DeleteCriticalSection(&m_cs);
}

void CXTPImageManagerResource::RemoveAll()
{
	int i;
	for (i = 0; i < (int)m_arrIcons.GetSize(); i++)
	{
		delete m_arrIcons[i];
	}
	m_arrIcons.RemoveAll();

	for (i = 0; i < (int)m_arrAlloc.GetSize(); i++)
	{
		delete m_arrAlloc[i];
	}
	m_arrAlloc.RemoveAll();


	if (m_hbmpImage)
	{
		DeleteObject(m_hbmpImage);
		m_hbmpImage = NULL;
	}
	if (m_hbmpMask)
	{
		DeleteObject(m_hbmpMask);
		m_hbmpMask = NULL;
	}

	m_nAllocWidth = 0;
	m_pBits = NULL;
}

XTP_IMAGERESOURCE_INFO* CXTPImageManagerResource::AllocBitmap()
{
	if (m_arrAlloc.GetSize() > 0)
	{
		XTP_IMAGERESOURCE_INFO* pRInfo = m_arrAlloc[m_arrAlloc.GetSize() - 1];
		m_arrAlloc.RemoveAt(m_arrAlloc.GetSize() - 1);
		m_arrIcons.Add(pRInfo);
		return pRInfo;
	}

	if (!m_hbmpImage && !m_hbmpMask)
	{
		m_hbmpImage = CXTPImageManager::Create32BPPDIBSection(NULL, m_szIcon.cx, m_szIcon.cy, &m_pBits);
		m_hbmpMask = CreateBitmap(m_szIcon.cx, m_szIcon.cy, 1, 1, NULL);

		m_nAllocWidth = m_szIcon.cx;

		if (!m_hbmpImage || !m_hbmpMask)
			return NULL;

		XTP_IMAGERESOURCE_INFO* pRInfo = new XTP_IMAGERESOURCE_INFO();
		pRInfo->pResource = this;
		pRInfo->bAlpha = FALSE;
		pRInfo->rc = CRect(0, 0, m_szIcon.cx, m_szIcon.cy);

		m_arrIcons.Add(pRInfo);
		return pRInfo;
	}

	LPBYTE pBits = NULL;
	HBITMAP hbmpImage = CXTPImageManager::Create32BPPDIBSection(NULL, m_szIcon.cx + m_nAllocWidth, m_szIcon.cy, &pBits);
	HBITMAP hbmpMask = CreateBitmap(m_szIcon.cx + m_nAllocWidth, m_szIcon.cy, 1, 1, NULL);

	if (hbmpImage && pBits)
	{
		LPBYTE pBitsI = pBits;

		for (int y = 0; y < m_szIcon.cy; y++)
		{
			memcpy(pBitsI, m_pBits, m_nAllocWidth * 4);
			m_pBits += m_nAllocWidth * 4;
			pBitsI += (m_szIcon.cx + m_nAllocWidth) * 4;
		}
	}
	else
	{
		return NULL;
	}

	if (hbmpMask)
	{
		m_dc.SetBitmap(hbmpMask);
		m_dc.DrawState(CPoint(0, 0), CSize(0, 0), m_hbmpMask, 0, 0);
		m_dc.SetBitmap(NULL);
	}
	else
	{
		return NULL;
	}

	DeleteObject(m_hbmpImage);
	DeleteObject(m_hbmpMask);

	m_hbmpImage = hbmpImage;
	m_hbmpMask = hbmpMask;
	m_pBits = pBits;

	XTP_IMAGERESOURCE_INFO* pRInfo = new XTP_IMAGERESOURCE_INFO();
	pRInfo->pResource = this;
	pRInfo->bAlpha = FALSE;
	pRInfo->rc = CRect(m_nAllocWidth, 0, m_nAllocWidth + m_szIcon.cx, m_szIcon.cy);
	m_arrIcons.Add(pRInfo);

	m_nAllocWidth += m_szIcon.cx;

	return pRInfo;
}

void CXTPImageManagerResource::Clear(XTP_IMAGERESOURCE_INFO* pRInfo)
{
	CXTPLockGuard lock(m_cs);

	for (int i = 0; i < (int)m_arrIcons.GetSize(); i++)
	{
		if (m_arrIcons[i] == pRInfo)
		{
			m_arrIcons.RemoveAt(i);
			m_arrAlloc.Add(pRInfo);

			if (m_arrAlloc.GetSize() * m_szIcon.cx == m_nAllocWidth)
			{
				ASSERT(m_arrIcons.GetSize() == 0);
				RemoveAll();
			}
			return;
		}
	}
}

XTP_IMAGERESOURCE_INFO* CXTPImageManagerResource::Add(const CXTPImageManagerIconHandle& hHandle)
{
	CXTPLockGuard lock(m_cs);

	ASSERT(hHandle.m_pRInfo == NULL);
	ASSERT(!hHandle.IsEmpty());
	if (hHandle.IsEmpty())
		return NULL;

	if (hHandle.GetExtent() != GetExtent())
		return NULL;

	if (!hHandle.m_hIcon && !hHandle.m_hBitmap)
		return NULL;

	XTP_IMAGERESOURCE_INFO* pRInfo = AllocBitmap();
	if (pRInfo == NULL)
		return NULL;

	if (hHandle.m_hIcon)
	{
		ICONINFO iconInfo;
		if (CXTPImageManager::IsAlphaIconsImageListSupported() && GetIconInfo(hHandle.m_hIcon, &iconInfo))
		{
			LPBYTE pBitsI = NULL;
			BOOL bAlpha = -1;
			HBITMAP hBitmap = CXTPImageManagerIcon::PreMultiplyAlphaBitmap(iconInfo.hbmColor, &bAlpha, &pBitsI);

			DeleteObject(iconInfo.hbmColor);
			DeleteObject(iconInfo.hbmMask);

			if (bAlpha == TRUE && pBitsI)
			{
				for (int y = 0; y < m_szIcon.cy; y++)
				{
					LPBYTE pBitsRow = m_pBits + ((y * m_nAllocWidth + pRInfo->rc.left) * 4);
					memcpy(pBitsRow, pBitsI, m_szIcon.cx * 4);
					pBitsI += m_szIcon.cx * 4;
				}

				pRInfo->bAlpha = TRUE;

				DeleteObject(hBitmap);
				return pRInfo;
			}

			if (hBitmap) DeleteObject(hBitmap);
		}

		if (m_hbmpImage)
		{
			m_dc.SetBitmap(m_hbmpImage);
			m_dc.FillSolidRect(pRInfo->rc, 0);
			DrawIconEx(m_dc, pRInfo->rc.left, pRInfo->rc.top, hHandle.m_hIcon, 0, 0, 0, NULL, DI_NORMAL);
			m_dc.SetBitmap(NULL);
		}

		if (m_hbmpMask)
		{
			m_dc.SetBitmap(m_hbmpMask);
			m_dc.FillSolidRect(pRInfo->rc, 0);
			DrawIconEx(m_dc, pRInfo->rc.left, pRInfo->rc.top, hHandle.m_hIcon, 0, 0, 0, NULL, DI_MASK);
			m_dc.SetBitmap(NULL);
		}

		pRInfo->bAlpha = FALSE;
	}
	else
	{
		LPBYTE pBitsI = ((CXTPImageManagerIconHandle&)hHandle).PreMultiply();
		if (!pBitsI)
			return NULL;

		for (int y = 0; y < m_szIcon.cy; y++)
		{
			LPBYTE pBitsRow = m_pBits + ((y * m_nAllocWidth + pRInfo->rc.left) * 4);
			memcpy(pBitsRow, pBitsI, m_szIcon.cx * 4);
			pBitsI += m_szIcon.cx * 4;
		}

		pRInfo->bAlpha = TRUE;
	}

	return pRInfo;
}


void CXTPImageManagerResource::CreateIcon(CXTPImageManagerIconHandle& hHandle, XTP_IMAGERESOURCE_INFO* pRInfo)
{
	CXTPLockGuard lock(m_cs);

	hHandle.Clear();

	HBITMAP hBitmap = 0;
	LPBYTE pBits = 0;


	{
		hBitmap = CXTPImageManager::Create32BPPDIBSection(NULL, m_szIcon.cx, m_szIcon.cy, &pBits);
		if (!hBitmap || !pBits)
			return;

		LPBYTE pBitsI = pBits;

		for (int y = 0; y < m_szIcon.cy; y++)
		{
			LPBYTE pBitsRow = m_pBits + ((y * m_nAllocWidth + pRInfo->rc.left) * 4);
			memcpy(pBitsI, pBitsRow, m_szIcon.cx * 4);
			pBitsI += m_szIcon.cx * 4;
		}
	}

	if (pRInfo->bAlpha)
	{
		hHandle.m_hBitmap = hBitmap;
		hHandle.m_pBits = pBits;
	}
	else
	{
		HBITMAP hbmpMask = CreateBitmap(m_szIcon.cx, m_szIcon.cy, 1, 1, NULL);
		{
			CXTPCompatibleDC dc(0, hbmpMask);
			m_dc.SetBitmap(m_hbmpMask);
			dc.BitBlt(0, 0, m_szIcon.cx, m_szIcon.cy, &m_dc, pRInfo->rc.left, 0, SRCCOPY);
			m_dc.SetBitmap(NULL);
		}

		ICONINFO ii;
		ii.fIcon    = TRUE;
		ii.xHotspot = 0;
		ii.yHotspot = 0;
		ii.hbmColor = hBitmap;
		ii.hbmMask  = hbmpMask;
		hHandle.m_hIcon = CreateIconIndirect(&ii);

		DeleteObject(hBitmap);
		DeleteObject(hbmpMask);
	}

	hHandle.m_bClearHandles = TRUE;
}


void CXTPImageManagerResource::DrawIconComposited(CDC* pDC, XTP_IMAGERESOURCE_INFO* pRInfo, CPoint pt, CSize szIcon)
{
	CBitmap bmp;
	LPDWORD lpBits = 0;
	bmp.Attach(CXTPImageManager::Create32BPPDIBSection(pDC->GetSafeHdc(), szIcon.cx, szIcon.cy, (LPBYTE*)&lpBits));
	if (!lpBits)
		return;

	CXTPCompatibleDC dc(&m_dc, &bmp);

	dc.SetBkColor(0xFFFFFF);
	dc.SetTextColor(0);

	if (m_hbmpMask)
	{
		m_dc.SetBitmap(m_hbmpMask);
		StretchBlt(dc, 0, 0, szIcon.cx, szIcon.cy, m_dc, pRInfo->rc.left, 0, m_szIcon.cx, m_szIcon.cy, SRCCOPY);
		m_dc.SetBitmap(NULL);
	}

	for (int i = 0; i < szIcon.cx * szIcon.cy; i++)
	{
		*lpBits =  (*lpBits == 0) ? 0xFF000000 : 0;
		lpBits++;
	}

	if (m_hbmpImage)
	{
		m_dc.SetBitmap(m_hbmpImage);
		StretchBlt(dc, 0, 0, szIcon.cx, szIcon.cy, m_dc, pRInfo->rc.left, 0, m_szIcon.cx, m_szIcon.cy, SRCINVERT);
		m_dc.SetBitmap(NULL);
	}

	XTPImageManager()->AlphaBlend(*pDC, CRect(pt, szIcon), dc, CRect(0, 0, szIcon.cx, szIcon.cy));
}


void CXTPImageManagerResource::Draw(CDC* pDC, XTP_IMAGERESOURCE_INFO* pRInfo, CPoint pt, CSize szIcon, BOOL bDrawComposited)
{
	CXTPLockGuard lock(m_cs);

	if (!pRInfo->bAlpha)
	{
		pDC->SetStretchBltMode(COLORONCOLOR);

		if (bDrawComposited)
		{
			DrawIconComposited(pDC, pRInfo, pt, szIcon);
			return;
		}

		if (m_pImageManager->IsDrawReverted(pDC))
		{
			pt.x += szIcon.cx;
			szIcon.cx = -szIcon.cx;
		}

		if (m_hbmpMask)
		{
			m_dc.SetBitmap(m_hbmpMask);
			m_dc.SetTextColor(0);
			m_dc.SetBkColor(0xFFFFFF);

			COLORREF clrOldText = pDC->SetTextColor(0);
			COLORREF clrOldBk = pDC->SetBkColor(0xFFFFFF);

			StretchBlt(pDC->GetSafeHdc(), pt.x, pt.y, szIcon.cx, szIcon.cy, m_dc, pRInfo->rc.left,  0,  m_szIcon.cx, m_szIcon.cy, SRCAND);

			pDC->SetBkColor(clrOldBk);
			pDC->SetTextColor(clrOldText);
			m_dc.SetBitmap(NULL);
		}

		if (m_hbmpImage)
		{
			m_dc.SetBitmap(m_hbmpImage);
			StretchBlt(pDC->GetSafeHdc(), pt.x, pt.y, szIcon.cx, szIcon.cy, m_dc, pRInfo->rc.left,  0,  m_szIcon.cx, m_szIcon.cy, SRCINVERT);
			m_dc.SetBitmap(NULL);
		}
	}
	else
	{
		m_dc.SetBitmap(m_hbmpImage);
		XTPImageManager()->AlphaBlend(pDC->GetSafeHdc(), CRect(pt, szIcon),
			m_dc, CRect(pRInfo->rc.TopLeft(), m_szIcon));
		m_dc.SetBitmap(NULL);
	}
}

CSize CXTPImageManagerResource::GetExtent() const
{
	return m_szIcon;
}
