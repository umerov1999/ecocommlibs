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

#include <TabManager/Themes/XTPTabThemeOffice2000.h>



//////////////////////////////////////////////////////////////////////////
// CXTPTabThemeOffice2000

void CXTPTabThemeOffice2000::RefreshMetrics()
{
	CXTPTabPaintManagerTheme::RefreshMetrics();
}

CRect CXTPTabThemeOffice2000::GetButtonDrawRect(const CXTPTabManagerItem* pItem)
{
	CRect rc = pItem->GetRect();
	rc.InflateRect(2, 2);

	return rc;
}

CRect CXTPTabThemeOffice2000::GetHeaderClipBox(CDC* pDC, CXTPTabManager* pTabManager)
{
	CRect rcClipBox;
	pDC->GetClipBox(rcClipBox);

	CRect rcHeader = pTabManager->GetHeaderRect();

	if (pTabManager->GetPosition() == xtpTabPositionBottom || pTabManager->GetPosition() == xtpTabPositionRight)
		InflateRectEx(rcHeader, CRect(0, 0, 0, 1), pTabManager->GetPosition());

	rcClipBox.IntersectRect(rcClipBox, rcHeader);

	return rcClipBox;
}

CRect CXTPTabThemeOffice2000::FillTabControl(CXTPTabManager* pTabManager, CDC* pDC, CRect rcControl)
{
	CRect rcHeader = GetHeaderRect(rcControl, pTabManager);
	InflateRectEx(rcHeader, CRect(0, 0, 0, 1), pTabManager->GetPosition());

	GetColorSet()->FillHeader(pDC, rcHeader, pTabManager);


	CRect rcClient = GetClientRect(rcControl, pTabManager);
	if (m_pPaintManager->m_clientFrame == xtpTabFrameBorder)
	{
		GetColorSet()->FillClient(pDC, rcClient, pTabManager);
		Draw3dRect(pDC, rcClient, GetColorSet()->m_csPropertyPage.clrHighlight, GetColorSet()->m_csPropertyPage.clrDarkShadow);

		HorizontalLine(pDC, rcClient.left + 1, rcClient.bottom - 2, rcClient.Width() - 2, GetColorSet()->m_csPropertyPage.clrShadow);
		VerticalLine(pDC, rcClient.right - 2, rcClient.top + 1, rcClient.Height() - 2, GetColorSet()->m_csPropertyPage.clrShadow);
	}
	if (m_pPaintManager->m_clientFrame == xtpTabFrameSingleLine)
	{
		switch (pTabManager->GetPosition())
		{
			case xtpTabPositionTop: HorizontalLine(pDC, rcClient.left, rcClient.top, rcClient.Width(), GetColorSet()->m_csPropertyPage.clrHighlight); break;
			case xtpTabPositionLeft: VerticalLine(pDC, rcClient.left, rcClient.top, rcClient.Height(), GetColorSet()->m_csPropertyPage.clrHighlight); break;
			case xtpTabPositionBottom:
				HorizontalLine(pDC, rcClient.left, rcClient.bottom - 1, rcClient.Width(), GetColorSet()->m_csPropertyPage.clrDarkShadow);
				HorizontalLine(pDC, rcClient.left, rcClient.bottom - 2, rcClient.Width(), GetColorSet()->m_csPropertyPage.clrShadow);
				break;
			case xtpTabPositionRight:
				VerticalLine(pDC, rcClient.right - 1, rcClient.top, rcClient.Height(), GetColorSet()->m_csPropertyPage.clrDarkShadow);
				VerticalLine(pDC, rcClient.right - 2, rcClient.top, rcClient.Height(), GetColorSet()->m_csPropertyPage.clrShadow);
				break;
		}

	}


	return rcHeader;
}

CRect CXTPTabThemeOffice2000::GetClientMargin()
{
	CRect rc = m_pPaintManager->m_rcClientMargin;

	if (m_pPaintManager->GetColorSet()->IsAppThemed() && m_pPaintManager->m_clientFrame == xtpTabFrameBorder)
	{
		if (m_pPaintManager->GetPosition() == xtpTabPositionTop ||
			m_pPaintManager->GetPosition() == xtpTabPositionLeft)
		{
			rc.bottom += 2;
		}
		rc.right += 2;
	}

	return rc;
}



void CXTPTabThemeOffice2000::DrawSingleButton(CDC* pDC, CXTPTabManagerItem* pItem)
{
	CRect rc = pItem->GetRect();

	COLORREF clrShadow = GetColorSet()->m_csPropertyPage.clrShadow, clrHighlight = GetColorSet()->m_csPropertyPage.clrHighlight, clrDarkShadow = GetColorSet()->m_csPropertyPage.clrDarkShadow;

	switch (pItem->GetTabManager()->GetPosition())
	{
	case xtpTabPositionTop:
		{
			if (pItem->IsSelected())
				rc.InflateRect(2, 2, 2, 0);

			COLORREF clr = GetColorSet()->FillPropertyButton(pDC, CRect(rc.left + 1, rc.top + 1, rc.right - 1, rc.bottom - 1), pItem);

			VerticalLine(pDC, rc.left, rc.top + 2, rc.Height() - 2, clrHighlight);
			Pixel(pDC, rc.left + 1, rc.top + 1, clrHighlight);
			HorizontalLine(pDC, rc.left + 2, rc.top, rc.Width() - 4, clrHighlight);

			Pixel(pDC, rc.right - 2 , rc.top + 1, clrDarkShadow);

			VerticalLine(pDC, rc.right - 1, rc.top + 2, rc.Height() - 3, clrDarkShadow);
			VerticalLine(pDC, rc.right - 2, rc.top + 2, rc.Height() - 3, clrShadow);

			if (pItem->IsSelected() && pItem->GetItemRow() == 0)
				HorizontalLine(pDC, rc.left + 1, rc.bottom - 1, rc.Width() - 3, clr);
		}
		break;

	case xtpTabPositionLeft:
		{
			if (pItem->IsSelected())
				rc.InflateRect(2, 2, 0, 2);

			COLORREF clr = GetColorSet()->FillPropertyButton(pDC, CRect(rc.left + 1, rc.top + 1, rc.right - 1, rc.bottom - 1), pItem);

			HorizontalLine(pDC, rc.left + 2, rc.top, rc.Width() - 2, clrHighlight);
			Pixel(pDC, rc.left + 1, rc.top + 1, clrHighlight);

			VerticalLine(pDC, rc.left, rc.top + 2, rc.Height() - 4, clrHighlight);

			HorizontalLine(pDC, rc.left + 1, rc.bottom - 2, rc.Width() - 2, clrShadow);
			HorizontalLine(pDC, rc.left + 2, rc.bottom - 1, rc.Width() - 3, clrDarkShadow);

			Pixel(pDC, rc.left + 2, rc.bottom - 2, clrDarkShadow);
			Pixel(pDC, rc.left + 2, rc.bottom - 1, clrShadow);

			if (pItem->IsSelected() && pItem->GetItemRow() == 0)
				VerticalLine(pDC, rc.right - 1, rc.top + 1, rc.Height() - 3, clr);
		}
		break;

	case xtpTabPositionBottom:
		{
			if (pItem->IsSelected())
				rc.InflateRect(2, 2);

			COLORREF clr = GetColorSet()->FillPropertyButton(pDC, CRect(rc.left + 1, rc.top + 1, rc.right - 1, rc.bottom - 1), pItem);

			VerticalLine(pDC, rc.left, rc.top + 1, rc.Height() - 2, clrHighlight);
			Pixel(pDC, rc.left + 1, rc.bottom - 1, clrHighlight);

			HorizontalLine(pDC, rc.left + 2, rc.bottom -1, rc.Width() - 4, clrShadow);
			HorizontalLine(pDC, rc.left + 2, rc.bottom, rc.Width() - 4, clrDarkShadow);

			Pixel(pDC, rc.right - 2, rc.bottom - 1, clrDarkShadow);

			VerticalLine(pDC, rc.right - 1, rc.top + 1, rc.Height() - 2, clrDarkShadow);
			VerticalLine(pDC, rc.right - 2, rc.top + 1, rc.Height() - 2, clrShadow);

			if (pItem->IsSelected() && pItem->GetItemRow() == 0)
			{
				HorizontalLine(pDC, rc.left + 1, rc.top + 1, rc.Width() - 1, clr);
				rc.bottom += 2;
			}
		}
		break;

	case xtpTabPositionRight:
		{
			if (pItem->IsSelected())
				rc.InflateRect(2, 2);

			COLORREF clr = GetColorSet()->FillPropertyButton(pDC, CRect(rc.left + 1, rc.top + 1, rc.right - 1, rc.bottom - 1), pItem);

			HorizontalLine(pDC, rc.left + 1, rc.top, rc.Width() - 2, clrHighlight);
			Pixel(pDC, rc.right - 1, rc.top + 1, clrHighlight);

			VerticalLine(pDC, rc.right - 1, rc.top + 2, rc.Height() - 4, clrShadow);
			VerticalLine(pDC, rc.right - 0, rc.top + 2, rc.Height() - 4, clrDarkShadow);

			Pixel(pDC, rc.right - 1, rc.bottom - 2, clrDarkShadow);

			HorizontalLine(pDC, rc.left + 1, rc.bottom - 2, rc.Width() - 2, clrShadow);
			HorizontalLine(pDC, rc.left + 1, rc.bottom - 1, rc.Width() - 2, clrDarkShadow);

			if (pItem->IsSelected() && pItem->GetItemRow() == 0)
			{
				VerticalLine(pDC, rc.left + 1, rc.top + 1, rc.Height() - 1, clr);
				rc.right += 2;
			}
		}
		break;


	}


	m_pPaintManager->DrawSingleButtonIconAndText(pDC, pItem, rc, TRUE);
}
