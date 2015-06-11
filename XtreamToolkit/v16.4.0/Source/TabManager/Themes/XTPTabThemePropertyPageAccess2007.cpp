#include "StdAfx.h"

#include <Common/XTPColorManager.h>
#include <Common/XTPDrawHelpers.h>
#include <Common/XTPResourceImage.h>
#include <Common/XTPImageManager.h>

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
#include <TabManager/Themes/XTPTabThemePropertyPage2007.h>
#include <TabManager/Themes/XTPTabThemePropertyPageAccess2007.h>
#include <TabManager/ColorSets/XTPTabColorSetDefault.h>
#include <TabManager/ColorSets/XTPTabColorSetWinNative.h>


//////////////////////////////////////////////////////////////////////////
// CAppearanceSetPropertyPageAccess2007

void CAppearanceSetPropertyPageAccess2007::RefreshMetrics()
{
	CAppearanceSetPropertyPage2007::RefreshMetrics();

	m_nButtonHeight = max(m_nButtonHeight, 24);
}

CRect CAppearanceSetPropertyPageAccess2007::GetButtonDrawRect(const CXTPTabManagerItem* pItem)
{
	CRect rc = pItem->GetRect();

	return pItem->GetTabManager()->IsHorizontalPosition() ?
		CRect(rc.left, rc.top, rc.right + m_nButtonHeight / 2, rc.bottom) :
		CRect(rc.left, rc.top, rc.right, rc.bottom + m_nButtonHeight / 2);
}


CRect CAppearanceSetPropertyPageAccess2007::FillTabControl(CXTPTabManager* pTabManager, CDC* pDC, CRect rcControl)
{
	CRect rcHeader = GetHeaderRect(rcControl, pTabManager);
	pDC->FillSolidRect(rcHeader, GetColorSet()->m_clrHeaderFace.clrLight);

	CRect rcClient = GetClientRect(rcControl, pTabManager);

	if (m_pPaintManager->m_clientFrame == xtpTabFrameBorder)
	{
		GetColorSet()->FillClient(pDC, rcClient, pTabManager);
		pDC->Draw3dRect(rcClient, GetColorSet()->m_clrFrameBorder, GetColorSet()->m_clrFrameBorder);
	}
	if (m_pPaintManager->m_clientFrame == xtpTabFrameSingleLine)
	{
		GetColorSet()->FillClient(pDC, rcClient, pTabManager);
		DrawSingleLineBorder(pDC, rcClient, pTabManager->GetPosition(), GetColorSet()->m_clrFrameBorder, GetColorSet()->m_clrFrameBorder);
	}

	return rcHeader;
}

void CAppearanceSetPropertyPageAccess2007::DrawButtonBackground(CDC* pDC, CXTPTabManagerItem* pItem, CRect rc)
{
	CXTPResourceImages* pImages = XTPResourceImages();

	CXTPResourceImage* pImage = pImages->LoadFile(_T("ACCESSTAB"));

	int nState = 0;
	if (pItem->IsSelected() && pItem->IsHighlighted())
	{
		nState = 3;
	}
	else if (pItem->IsSelected())
	{
		nState = 2;
	}
	else if (pItem->IsHighlighted())
	{
		nState = 1;
	}

	CRect rcSrc(pImage->GetSource(nState, 4));

	if (pImage) pImage->DrawImage(pDC, rc, rcSrc, CRect(5, 10, 22, 10), 0xFF00FF);
}

void CAppearanceSetPropertyPageAccess2007::DrawSingleButton(CDC* pDC, CXTPTabManagerItem* pItem)
{
	CXTPResourceImages* pImages = XTPResourceImages();

	if (!pImages->IsValid())
	{
		CXTPTabPaintManagerTheme::DrawSingleButton(pDC, pItem);
		return;
	}

	CRect rc = pItem->GetRect();
	if (IsHorizontalTabPosition(pItem->GetTabManager()->GetPosition())) rc.right += m_nButtonHeight / 2 - 1; else rc.bottom += m_nButtonHeight / 2 - 1;

	switch (pItem->GetTabManager()->GetPosition())
	{
	case xtpTabPositionTop:
		DrawButtonBackground(pDC, pItem, rc);
		break;

	case xtpTabPositionBottom:
		DrawRotatedButton(pDC, rc, pItem, FALSE, &CColorSetWinNative::DrawRotatedBitsBottom, &CColorSetWinNative::DrawRotatedBitsBottom);
		break;

	case xtpTabPositionLeft:
		DrawRotatedButton(pDC, rc, pItem, TRUE, &CColorSetWinNative::DrawRotatedBitsLeft, &CColorSetWinNative::DrawRotatedBitsLeft);
		break;

	case xtpTabPositionRight:
		DrawRotatedButton(pDC, rc, pItem, TRUE, &CColorSetWinNative::DrawRotatedBitsInvertRight, &CColorSetWinNative::DrawRotatedBitsRight);
		break;
	}

	m_pPaintManager->DrawSingleButtonIconAndText(pDC, pItem, pItem->GetRect(), TRUE);
}
