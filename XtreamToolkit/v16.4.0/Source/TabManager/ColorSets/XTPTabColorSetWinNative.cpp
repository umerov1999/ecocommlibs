#include "StdAfx.h"

#include <Common/XTPColorManager.h>
#include <Common/XTPImageManager.h>
#include <Common/XTPWinThemeWrapper.h>

#include <TabManager/XTPTabManagerDefines.h>
#include <TabManager/XTPTabManagerAtom.h>
#include <TabManager/XTPTabManagerNavigateButton.h>
#include <TabManager/XTPTabManagerNavigateButtons.h>
#include <TabManager/XTPTabManagerItem.h>
#include <TabManager/XTPTabManager.h>
#include <TabManager/XTPTabPaintManager.h>
#include <TabManager/XTPTabPaintManagerTheme.h>
#include <TabManager/XTPTabPaintManagerColorSet.h>
#include <TabManager/ColorSets/XTPTabColorSetDefault.h>
#include <TabManager/ColorSets/XTPTabColorSetWinNative.h>


//////////////////////////////////////////////////////////////////////////
// CColorSetWinNative

CColorSetWinNative::CColorSetWinNative()
{
	m_themeTabButton = new CXTPWinThemeWrapper();
	m_themeToolbar = new CXTPWinThemeWrapper();
}

CColorSetWinNative::~CColorSetWinNative()
{
	SAFE_DELETE(m_themeTabButton);
	SAFE_DELETE(m_themeToolbar);
}


void CColorSetWinNative::RefreshMetrics()
{
	CXTPTabColorSetDefault::RefreshMetrics();

	m_themeTabButton->OpenThemeData(0, L"TAB");

	if (m_themeTabButton->IsAppThemed())
	{
		m_csPropertyPage.clrHighlight.SetStandardValue(XTP_TABMANAGER_COLOR_NONE);
		m_csPropertyPage.clrShadow.SetStandardValue(XTP_TABMANAGER_COLOR_NONE);
		m_csPropertyPage.clrDarkShadow.SetStandardValue(XTP_TABMANAGER_COLOR_NONE);


		COLORREF clrClientFace = m_clrPaneFace;
		if (SUCCEEDED(m_themeTabButton->GetThemeColor(TABP_PANE, 0, TMT_FILLCOLORHINT, &clrClientFace)))
		{
			m_clrPaneFace.SetStandardValue(clrClientFace);
		}

		COLORREF clrText;
		if (SUCCEEDED(m_themeTabButton->GetThemeColor(TABP_TOPTABITEM, TIS_NORMAL, TMT_TEXTCOLOR, &clrText)))
		{
			m_clrTextNormal.SetStandardValue(clrText);
		}

		if (SUCCEEDED(m_themeTabButton->GetThemeColor(TABP_TOPTABITEM, TIS_SELECTED, TMT_TEXTCOLOR, &clrText)))
		{
			m_clrTextSelected.SetStandardValue(clrText);
		}

		m_clrTextHighlight = m_clrTextNormal;

		if (SUCCEEDED(m_themeTabButton->GetThemeColor(TABP_TOPTABITEM, TIS_HOT, TMT_TEXTCOLOR, &clrText)))
		{
			m_clrTextHighlight.SetStandardValue(clrText);
		}

		m_clrTextInactive = m_clrTextNormal;
	}

	m_themeToolbar->OpenThemeData(0, L"TOOLBAR");
}

BOOL CColorSetWinNative::IsAppThemed() const
{
	return m_themeTabButton->IsAppThemed();
}

void CColorSetWinNative::DrawRotatedBitsBottom(int cx, int cy, UINT* pSrcBits, UINT* pDestBits)
{
	UINT* pSrc = &pSrcBits[cx * (cy - 1)];
	for (int i = 0; i < cy; i++)
	{
		pSrcBits = pSrc;
		for (int j = 0; j < cx; j++)
		{
			*pDestBits++ = *pSrcBits++;
		}
		pSrc -= cx;
	}
}

void CColorSetWinNative::DrawRotatedBitsInvertRight(int cx, int cy, UINT* pSrcBits, UINT* pDestBits)
{
	UINT* pSrc = pSrcBits;
	UINT* pDest = pDestBits + cx - 1;

	for (int i = 0; i < cx; i++)
	{
		pSrcBits = pSrc;
		pDestBits = pDest;
		for (int j = 0; j < cy; j++)
		{
			*pDestBits = *pSrcBits;
			pSrcBits++;
			pDestBits += cx;
		}
		pSrc += cy;
		pDest -= 1;
	}
}

void CColorSetWinNative::DrawRotatedBitsRight(int cx, int cy, UINT* pSrcBits, UINT* pDestBits)
{
	UINT* pSrc = pSrcBits;
	UINT* pDest = pDestBits + cx * cy - cx;
	for (int i = 0; i < cy; i++)
	{
		pSrcBits = pSrc;
		pDestBits = pDest;
		for (int j = 0; j < cx; j++)
		{
			*pDestBits++ = *pSrcBits;
			pSrcBits += cy;
		}
		pSrc += 1;
		pDest -= cx;
	}
}

void CColorSetWinNative::DrawRotatedBitsLeft(int cx, int cy, UINT* pSrcBits, UINT* pDestBits)
{
	UINT* pSrc = pSrcBits;
	pDestBits += cx * cy - 1;
	for (int i = 0; i < cy; i++)
	{
		pSrcBits = pSrc;
		for (int j = 0; j < cx; j++)
		{
			*pDestBits-- = *pSrcBits;
			pSrcBits += cy;
		}
		pSrc += 1;
	}
}


void CColorSetWinNative::DrawRotatedButton(CDC* pDC, CRect rcItem, CXTPTabManagerItem* pItem, BOOL bSwap, LPFNDRAWROTATEDBITS pfnRotatedProc)
{
	const int cx = rcItem.Width();
	const int cy = rcItem.Height();

	UINT* pSrcBits = NULL;
	HBITMAP hbmSrc = CXTPImageManager::Create32BPPDIBSection(NULL, bSwap ? cy : cx, bSwap ? cx : cy, (LPBYTE*)&pSrcBits);
	if (!pSrcBits)
		return;

	UINT* pDestBits = NULL;
	HBITMAP hbmDest = CXTPImageManager::Create32BPPDIBSection(NULL, cx, cy, (LPBYTE*)&pDestBits);
	if (!pDestBits)
		return;

	CDC dc;
	dc.CreateCompatibleDC(NULL);
	HGDIOBJ hbmpOld = ::SelectObject(dc, hbmSrc);

	CRect rcDraw(0, 0, bSwap ? cy: cx, bSwap ? cx : cy);

	dc.FillSolidRect(rcDraw, GetXtremeColor(COLOR_3DFACE));

	m_themeTabButton->DrawThemeBackground(dc.GetSafeHdc(), pItem->GetIndex() == 0 ? TABP_TOPTABITEMLEFTEDGE :
				pItem->GetIndex() == pItem->GetTabManager()->GetItemCount() - 1 && !pItem->IsSelected() ? TABP_TOPTABITEMRIGHTEDGE : TABP_TOPTABITEM,
		pItem->IsSelected() ? TIS_SELECTED: pItem->IsHighlighted() ? TIS_HOT : TIS_NORMAL, &rcDraw, 0);

	(*pfnRotatedProc)(cx, cy, pSrcBits, pDestBits);

	pDC->DrawState(rcItem.TopLeft(), rcItem.Size(), hbmDest, DST_BITMAP);

	::SelectObject(dc, hbmpOld);

	DeleteObject(hbmSrc);
	DeleteObject(hbmDest);
}


COLORREF CColorSetWinNative::FillPropertyButton(CDC* pDC, CRect rcItem, CXTPTabManagerItem* pItem)
{
	if (!m_themeTabButton->IsAppThemed())
		return CXTPTabColorSetDefault::FillPropertyButton(pDC, rcItem, pItem);

	if (pItem->IsSelected())
	{
		switch (pItem->GetTabManager()->GetPosition())
		{
			case xtpTabPositionTop:
				rcItem.InflateRect(0, 0, 0, 2);
				break;

			case xtpTabPositionLeft:
				rcItem.InflateRect(0, 0, 1, 0);
				break;
		}
	}

	switch (pItem->GetTabManager()->GetPosition())
	{
		case xtpTabPositionTop:
			rcItem.InflateRect(1, 1, 1, 0);
			m_themeTabButton->DrawThemeBackground(pDC->GetSafeHdc(),
				pItem->GetIndex() == 0 ? TABP_TOPTABITEMLEFTEDGE :
				pItem->GetIndex() == pItem->GetTabManager()->GetItemCount() - 1 && !pItem->IsSelected() ? TABP_TOPTABITEMRIGHTEDGE : TABP_TOPTABITEM,
				pItem->IsSelected() ? TIS_SELECTED : pItem->IsHighlighted() ? TIS_HOT : TIS_NORMAL, &rcItem, 0);
			break;

		case xtpTabPositionBottom:
			rcItem.InflateRect(1, 0, 1, 2);
			DrawRotatedButton(pDC, rcItem, pItem, FALSE, DrawRotatedBitsBottom);
			break;

		case xtpTabPositionLeft:
			rcItem.InflateRect(1, 1, 0, 1);
			DrawRotatedButton(pDC, rcItem, pItem, TRUE, DrawRotatedBitsLeft);
			break;

		case xtpTabPositionRight:
			rcItem.InflateRect(0, 1, 2, 1);
			DrawRotatedButton(pDC, rcItem, pItem, TRUE, DrawRotatedBitsRight);
			break;
	}

	return XTP_TABMANAGER_COLOR_NONE;
}

COLORREF CColorSetWinNative::FillClient(CDC* pDC, CRect rc, CXTPTabManager* pTabManager)
{
	if (!m_themeTabButton->IsAppThemed())
		return CXTPTabColorSetDefault::FillClient(pDC, rc, pTabManager);

	switch (pTabManager->GetPosition())
	{
		case xtpTabPositionBottom:
			rc.InflateRect(0, 0, 0, 1);
			break;

		case xtpTabPositionRight:
			rc.InflateRect(0, 0, 1, 0);
			break;
	}

	m_themeTabButton->DrawThemeBackground(pDC->GetSafeHdc(), TABP_PANE, 0, &rc, 0);

	return 0;
}

void CColorSetWinNative::SelectClipRgn(CDC* pDC, CRect rcClient, CXTPTabManager* pTabManager)
{
	if (!m_themeTabButton->IsAppThemed())
	{
		CXTPTabColorSetDefault::SelectClipRgn(pDC, rcClient, pTabManager);
		return;
	}

	CRect rc = m_pPaintManager->GetAppearanceSet()->GetClientRect(rcClient, pTabManager);

	switch (pTabManager->GetPosition())
	{
	case xtpTabPositionBottom:
		pDC->ExcludeClipRect(rc.left, rc.bottom - 2, rc.left + 1, rc.bottom - 1);
		break;

	case xtpTabPositionTop:
	case xtpTabPositionLeft:
		pDC->ExcludeClipRect(rc.left, rc.top, rc.left + 1, rc.top + 1);
		break;

	case xtpTabPositionRight:
		pDC->ExcludeClipRect(rc.right - 2, rc.top, rc.right - 1, rc.top + 1);
		break;
	}
}

void CColorSetWinNative::FillNavigateButton(CDC* pDC, CXTPTabManagerNavigateButton* pButton, CRect& rc)
{
	if (!m_themeToolbar->IsAppThemed())
	{
		CXTPTabColorSetDefault::FillNavigateButton(pDC, pButton, rc);
		return;
	}

	if (pButton->IsEnabled())
	{
		m_themeToolbar->DrawThemeBackground(pDC->GetSafeHdc(), TP_BUTTON,
			pButton->IsPressed() ? TS_PRESSED :
			pButton->IsHighlighted() ? TS_HOT : TS_NORMAL, &rc, 0);
	}

	pDC->SetTextColor(GetXtremeColor(COLOR_BTNTEXT));

}

void CColorSetWinNative::FillStateButton(CDC* pDC, CRect rcItem, CXTPTabManagerItem* pItem)
{
	if (!m_themeToolbar->IsAppThemed() || !pItem->IsSelected())
	{
		CXTPTabColorSetDefault::FillStateButton(pDC, rcItem, pItem);
		return;
	}

	m_themeToolbar->DrawThemeBackground(pDC->GetSafeHdc(), TP_BUTTON,
		pItem->IsHighlighted() ? TS_HOTCHECKED : TS_CHECKED, &rcItem, 0);
}
