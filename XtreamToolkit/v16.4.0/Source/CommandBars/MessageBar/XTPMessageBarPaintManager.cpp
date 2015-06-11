#include <StdAfx.h>

#include <Common/XTPDrawHelpers.h>
#include <Common/XTPColorManager.h>

#include <CommandBars/XTPCommandBarsDefines.h>
#include <CommandBars/XTPPaintManager.h>

#include <CommandBars/MessageBar/XTPMessageBar.h>
#include <CommandBars/MessageBar/XTPMessageBarPaintManager.h>

CXTPMessageBarPaintManager::CXTPMessageBarPaintManager(CXTPPaintManager *pPaintManager)
	: m_pPaintManager(pPaintManager)
{
}

void CXTPMessageBarPaintManager::FillMessageBar(CDC* pDC, CXTPMessageBar* pBar)
{
	CXTPClientRect rcClient(pBar);
	pDC->FillSolidRect(rcClient, GetXtremeColor(COLOR_3DFACE));

	CRect rcMessage = pBar->GetMessageRect();
	pDC->Draw3dRect(rcMessage, GetXtremeColor(COLOR_3DSHADOW), GetXtremeColor(COLOR_3DSHADOW));

	rcMessage.DeflateRect(1, 1);
	pDC->FillSolidRect(rcMessage, GetXtremeColor(COLOR_INFOBK));
}

void CXTPMessageBarPaintManager::DrawMessageBarButton(CDC* pDC, CXTPMessageBarButton* pButton)
{
	BOOL bCloseButton = (pButton->m_nID == SC_CLOSE);
	CRect rc(pButton->m_rcButton);

	if (pButton->m_bPressed)
	{
		pDC->FillSolidRect(rc, GetXtremeColor(COLOR_3DFACE));
		m_pPaintManager->DrawRectangle(pDC, rc, pButton->m_bHot, pButton->m_bPressed, TRUE, FALSE, FALSE, xtpBarTypeNormal, xtpBarTop);
	}
	else if (pButton->m_bHot)
	{
		pDC->FillSolidRect(rc, GetXtremeColor(COLOR_3DFACE));
		m_pPaintManager->DrawRectangle(pDC, rc, pButton->m_bHot, FALSE, TRUE, FALSE, FALSE, xtpBarTypeNormal, xtpBarTop);
	}
	else if (!bCloseButton)
	{
		pDC->FillSolidRect(rc, GetXtremeColor(COLOR_3DFACE));
		pDC->Draw3dRect(rc, GetXtremeColor(COLOR_3DSHADOW), GetXtremeColor(COLOR_3DSHADOW));
	}

	if (bCloseButton)
	{
		CPoint pt = rc.CenterPoint();
		CXTPPenDC pen(pDC, 0);

		pDC->MoveTo(pt.x - 4, pt.y - 3);
		pDC->LineTo(pt.x + 3, pt.y + 4);
		pDC->MoveTo(pt.x - 3, pt.y - 3);
		pDC->LineTo(pt.x + 4, pt.y + 4);

		pDC->MoveTo(pt.x - 4, pt.y + 3);
		pDC->LineTo(pt.x + 3, pt.y - 4);
		pDC->MoveTo(pt.x - 3, pt.y + 3);
		pDC->LineTo(pt.x + 4, pt.y - 4);
	}
}

CRect CXTPMessageBarPaintManager::GetCloseButtonRect(int x, int y, int /*cy*/)
{
	CSize sz(18,18);
	return CRect(CPoint(x - sz.cx - 1, y), sz);
}
