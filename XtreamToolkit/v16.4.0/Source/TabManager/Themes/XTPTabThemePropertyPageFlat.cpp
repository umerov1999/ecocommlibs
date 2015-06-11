#include "StdAfx.h"

#include <Common/XTPColorManager.h>
#include <Common/XTPDrawHelpers.h>

#include <TabManager/XTPTabManagerDefines.h>
#include <TabManager/XTPTabManagerAtom.h>
#include <TabManager/XTPTabManagerNavigateButton.h>
#include <TabManager/XTPTabManagerNavigateButtons.h>
#include <TabManager/XTPTabManagerItem.h>
#include <TabManager/XTPTabManager.h>
#include <TabManager/XTPTabPaintManagerColorSet.h>
#include <TabManager/XTPTabPaintManager.h>
#include <TabManager/XTPTabPaintManager.inl>
#include <TabManager/XTPTabPaintManagerTheme.h>

#include <TabManager/Themes/XTPTabThemeFlat.h>
#include <TabManager/Themes/XTPTabThemePropertyPageFlat.h>


//////////////////////////////////////////////////////////////////////////
// CAppearanceSetPropertyPageFlat

void CAppearanceSetPropertyPageFlat::BlurPoint(CDC* pDC, int x, int y, COLORREF clr, int nAlpha)
{
	COLORREF clrOld = pDC->GetPixel(x, y);
	pDC->SetPixel(x, y, CXTPDrawHelpers::BlendColors(clr, clrOld, (double)nAlpha / 255));
}

void CAppearanceSetPropertyPageFlat::BlurTab(CDC* pDC, CRect rcTab, COLORREF clrBackground, COLORREF clrBorder, XTPTabPosition tabPosition)
{
	switch (tabPosition)
	{
	case xtpTabPositionTop:
		{
			BlurPoint(pDC, rcTab.left, rcTab.top + 1, clrBackground, 120);
			BlurPoint(pDC, rcTab.left + 1, rcTab.top, clrBackground, 120);

			BlurPoint(pDC, rcTab.right - 2, rcTab.top, clrBackground, 120);
			BlurPoint(pDC, rcTab.right - 1, rcTab.top + 1, clrBackground, 120);

			BlurPoint(pDC, rcTab.left + 1, rcTab.top + 1, clrBorder, 180);
			BlurPoint(pDC, rcTab.right - 2, rcTab.top + 1, clrBorder, 180);

			BlurPoint(pDC, rcTab.left + 2, rcTab.top + 1, clrBorder, 60);
			BlurPoint(pDC, rcTab.left + 1, rcTab.top +2, clrBorder, 60);
			BlurPoint(pDC, rcTab.right - 3, rcTab.top + 1, clrBorder, 60);
			BlurPoint(pDC, rcTab.right - 2, rcTab.top + 2, clrBorder, 60);
		}
		break;

	case xtpTabPositionLeft:
		{

			BlurPoint(pDC, rcTab.left, rcTab.top + 1, clrBackground, 120);
			BlurPoint(pDC, rcTab.left + 1, rcTab.top, clrBackground, 120);

			BlurPoint(pDC, rcTab.left, rcTab.bottom - 2, clrBackground, 120);
			BlurPoint(pDC, rcTab.left + 1, rcTab.bottom - 1, clrBackground, 120);

			BlurPoint(pDC, rcTab.left + 1, rcTab.top + 1, clrBorder, 180);
			BlurPoint(pDC, rcTab.left + 1, rcTab.bottom - 2, clrBorder, 180);

			BlurPoint(pDC, rcTab.left + 2, rcTab.top + 1, clrBorder, 60);
			BlurPoint(pDC, rcTab.left + 1, rcTab.top +2, clrBorder, 60);
			BlurPoint(pDC, rcTab.left + 2, rcTab.bottom - 2, clrBorder, 60);
			BlurPoint(pDC, rcTab.left + 1, rcTab.bottom - 3, clrBorder, 60);

		}
		break;

	case xtpTabPositionBottom:
		{
			BlurPoint(pDC, rcTab.left, rcTab.bottom - 2, clrBackground, 120);
			BlurPoint(pDC, rcTab.left + 1, rcTab.bottom -1, clrBackground, 120);

			BlurPoint(pDC, rcTab.right - 1, rcTab.bottom - 2, clrBackground, 120);
			BlurPoint(pDC, rcTab.right - 2, rcTab.bottom - 1, clrBackground, 120);

			BlurPoint(pDC, rcTab.left + 1, rcTab.bottom - 2, clrBorder, 180);
			BlurPoint(pDC, rcTab.right - 2, rcTab.bottom - 2, clrBorder, 180);

			BlurPoint(pDC, rcTab.left + 2, rcTab.bottom - 2, clrBorder, 60);
			BlurPoint(pDC, rcTab.left + 1, rcTab.bottom - 3, clrBorder, 60);
			BlurPoint(pDC, rcTab.right - 3, rcTab.bottom - 2, clrBorder, 60);
			BlurPoint(pDC, rcTab.right - 2, rcTab.bottom - 3, clrBorder, 60);
		}
		break;

	case xtpTabPositionRight:
		{
			BlurPoint(pDC, rcTab.right - 1, rcTab.top + 1, clrBackground, 120);
			BlurPoint(pDC, rcTab.right - 2, rcTab.top, clrBackground, 120);

			BlurPoint(pDC, rcTab.right - 1, rcTab.bottom - 2, clrBackground, 120);
			BlurPoint(pDC, rcTab.right - 2, rcTab.bottom - 1, clrBackground, 120);

			BlurPoint(pDC, rcTab.right - 2, rcTab.top + 1, clrBorder, 180);
			BlurPoint(pDC, rcTab.right - 2, rcTab.bottom - 2, clrBorder, 180);

			BlurPoint(pDC, rcTab.right - 3, rcTab.top + 1, clrBorder, 60);
			BlurPoint(pDC, rcTab.right - 2, rcTab.top +2, clrBorder, 60);
			BlurPoint(pDC, rcTab.right - 3, rcTab.bottom - 2, clrBorder, 60);
			BlurPoint(pDC, rcTab.right - 2, rcTab.bottom - 3, clrBorder, 60);

		}
		break;
	}
}

void CAppearanceSetPropertyPageFlat::DrawSingleButton(CDC* pDC, CXTPTabManagerItem* pItem)
{
	CRect rcItem(pItem->GetRect());
	XTPTabPosition tabPosition = pItem->GetTabManager()->GetPosition();

	InflateRectEx(rcItem, CRect(0, 0, 1, 0), tabPosition);

	CRect rcTab(rcItem);

	CRect rcClip;
	pDC->GetClipBox(rcClip);
	CRgn rgn;
	rgn.CreateRectRgnIndirect(rcClip);

	InflateRectEx(rcTab, CRect(0, 0, 0, 1), tabPosition);

	XTPDrawHelpers()->ExcludeCorners(pDC, rcTab);

	CRect rcEntry(rcItem);

	InflateRectEx(rcEntry, CRect(-1, -1, -1, 0), tabPosition);


	if (!m_bDrawNormalTab  && !pItem->IsSelected() && !pItem->IsHighlighted())
	{
		m_pPaintManager->DrawSingleButtonIconAndText(pDC, pItem, pItem->GetRect(), TRUE);
		return;
	}

	CXTPTabPaintManagerColorSet* pColorSet = GetColorSet();
	COLORREF clrFrameBorder = pItem->IsHighlighted() && !pItem->IsSelected() ? pColorSet->m_clrFrameHighlightBorder : COLORREF_NULL;
	if (clrFrameBorder == COLORREF_NULL) clrFrameBorder = pColorSet->m_clrFrameBorder;

	if (m_bVisualStudio2005Style)
	{
		if (pItem->IsSelected())
		{
			pColorSet->FillPropertyButton(pDC, rcEntry, pItem);
			if (!pItem->IsSelected())
				DrawSingleLineBorder(pDC, rcItem, XTPTabPosition((tabPosition + 2) % 4), clrFrameBorder, clrFrameBorder);
			InflateRectEx(rcItem, CRect(0, 0, 0, 2), tabPosition);

			CXTPPenDC pen(*pDC, clrFrameBorder);
			CGdiObject* pOldBrush = pDC->SelectStockObject(NULL_BRUSH);
			pDC->RoundRect(rcItem, CPoint(6, 6));
			pDC->SelectObject(pOldBrush);
		}
		else
		{
			if (pItem->GetIndex() < pItem->GetTabManager()->GetItemCount() - 1)
			{
				if (IsHorizontalTabPosition(tabPosition))
					pDC->FillSolidRect(rcItem.right - 1, rcItem.top + 3, 1, rcItem.Height() - 6, clrFrameBorder);
				else
					pDC->FillSolidRect(rcItem.left + 3, rcItem.bottom - 1, rcItem.Width() - 6, 1, clrFrameBorder);
			}
		}

	}
	else
	{
		pColorSet->FillPropertyButton(pDC, rcEntry, pItem);

		if (m_bBlurPoints)
		{
			if (pItem->IsSelected())
				InflateRectEx(rcItem, CRect(0, 0, 0, 1), tabPosition);

			pDC->Draw3dRect(rcItem, clrFrameBorder, clrFrameBorder);
		}
		else
		{
			if (m_pPaintManager->m_clientFrame != xtpTabFrameNone)
			{
				if (!pItem->IsSelected())
					DrawSingleLineBorder(pDC, rcItem, XTPTabPosition((tabPosition + 2) % 4), clrFrameBorder, clrFrameBorder);
			}

			InflateRectEx(rcItem, CRect(0, 0, 0, 2), tabPosition);

			CXTPPenDC pen(*pDC, clrFrameBorder);
			CGdiObject* pOldBrush = pDC->SelectStockObject(NULL_BRUSH);
			pDC->RoundRect(rcItem, CPoint(6, 6));
			pDC->SelectObject(pOldBrush);
		}
	}

	pDC->SelectClipRgn(&rgn);

	if (m_bBlurPoints)
	{
		BlurTab(pDC, rcTab, pColorSet->m_clrHeaderFace, clrFrameBorder, tabPosition);
	}

	m_pPaintManager->DrawSingleButtonIconAndText(pDC, pItem, pItem->GetRect(), TRUE);
}
