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

#include <TabManager/Themes/XTPTabThemeVisio.h>


//////////////////////////////////////////////////////////////////////////
// CAppearanceSetVisio


void CAppearanceSetVisio::AdjustClientRect(CXTPTabManager* pTabManager, CRect& rcClient)
{
	DeflateRectEx(rcClient, m_pPaintManager->m_rcControlMargin, pTabManager->GetPosition());

	rcClient = GetClientRect(rcClient, pTabManager);

	DeflateRectEx(rcClient, GetClientMargin(), pTabManager->GetPosition());
}

void CAppearanceSetVisio::RepositionTabControl(CXTPTabManager* pTabManager, CDC* /*pDC*/, CRect rcClient)
{

	CXTPTabPaintManagerTheme::DeflateRectEx(rcClient, m_pPaintManager->m_rcControlMargin, pTabManager->GetPosition());

	if (pTabManager->GetItemCount() == 0)
		return;

	int nButtonHeight = GetButtonHeight(pTabManager);

	CRect rc(rcClient);

	if (m_pPaintManager->m_clientFrame != xtpTabFrameNone)
	{
		rc.left++;
	}

	rc.top++;

	CRect rcItem(rc.left, rc.top, rc.right, rc.top);

	for (int i = 0; i < pTabManager->GetItemCount(); i++)
	{
		CXTPTabManagerItem* pItem = pTabManager->GetItem(i);

		rcItem.bottom = rcItem.top + nButtonHeight;

		pItem->SetRect(rcItem);

		rcItem.top += nButtonHeight;

		if (pItem->IsSelected())
		{
			int nBottom = max(rcItem.top + nButtonHeight, rc.bottom - (pTabManager->GetItemCount() - i - 1) * nButtonHeight);
			nBottom = max(rcItem.top, min(nBottom, rc.bottom - 1));
			rcItem.top = nBottom;
		}
	}
}

CRect CAppearanceSetVisio::GetHeaderRect(CRect /*rcControl*/, CXTPTabManager* /*pTabManager*/)
{
	return CXTPEmptyRect();
}


CRect CAppearanceSetVisio::GetClientRect(CRect rcControl, CXTPTabManager* pTabManager)
{
	CXTPTabManagerItem* pItem = pTabManager->GetSelectedItem();

	if (pItem)
	{
		int nButtonHeight = GetButtonHeight(pTabManager);

		rcControl.top += (pItem->GetIndex() + 1) * nButtonHeight + 1;

		rcControl.bottom -= (pTabManager->GetItemCount() - pItem->GetIndex() - 1) * nButtonHeight;

		rcControl.bottom = max(rcControl.top + nButtonHeight, rcControl.bottom);

		if (pItem->GetIndex() == pTabManager->GetItemCount() - 1 &&
			m_pPaintManager->m_clientFrame != xtpTabFrameNone) rcControl.bottom--;
	}

	if (m_pPaintManager->m_clientFrame != xtpTabFrameNone)
	{
		rcControl.DeflateRect(1, 0, 1, 0);
	}


	return rcControl;
}


void CAppearanceSetVisio::DrawSingleButton(CDC* pDC, CXTPTabManagerItem* pItem)
{
	CRect rcItem = pItem->GetRect();

	GetColorSet()->FillPropertyButton(pDC, rcItem, pItem);

	pDC->Draw3dRect(rcItem, GetColorSet()->m_clrBorderHighlight, GetColorSet()->m_clrFrameBorder);

	for (int y = rcItem.top + 4; y < rcItem.bottom - 3; y += 2)
	{
		HorizontalLine(pDC, rcItem.right - 7, y, 3, GetXtremeColor(XPCOLOR_TOOLBAR_GRIPPER));
	}
	rcItem.right -= 7;

	m_pPaintManager->DrawSingleButtonIconAndText(pDC, pItem, rcItem, TRUE);
}

void CAppearanceSetVisio::DrawTabControl(CXTPTabManager* pTabManager, CDC* pDC, CRect rcClient)
{
	pDC->FillSolidRect(rcClient, GetColorSet()->m_clrControlFace);

	CXTPTabPaintManagerTheme::DeflateRectEx(rcClient, m_pPaintManager->m_rcControlMargin, pTabManager->GetPosition());

	CRgn rgn;
	rgn.CreateRectRgnIndirect(rcClient);
	pDC->SelectClipRgn(&rgn);

	if (m_pPaintManager->m_clientFrame != xtpTabFrameNone)
	{
		pDC->Draw3dRect(rcClient, GetColorSet()->m_clrFrameBorder, GetColorSet()->m_clrFrameBorder);
	}
	else if (pTabManager->GetItemCount() > 0)
	{
		HorizontalLine(pDC, rcClient.left, rcClient.top, rcClient.Width(), GetColorSet()->m_clrFrameBorder);
	}

	pDC->SetBkMode(TRANSPARENT);

	for (int i = pTabManager->GetItemCount() - 1; i >= 0; i--)
	{
		CXTPTabManagerItem* pItem = pTabManager->GetItem(i);

		CRect rcItem = pItem->GetRect();

		if (CRect().IntersectRect(rcClient, rcItem) && !rcItem.IsRectEmpty())
		{
			DrawSingleButton(pDC, pItem);
		}
	}

	pDC->SelectClipRgn(NULL);
}
