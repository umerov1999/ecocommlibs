#include "StdAfx.h"

#include <Common/XTPSystemHelpers.h>
#include <Common/XTPColorManager.h>
#include <Common/XTPHookManager.h>
#include <Common/XTPToolTipContext.h>

#include <TabManager/Includes.h>

#include <CommandBars/XTPCommandBarsDefines.h>
#include <CommandBars/XTPCommandBars.h>
#include <CommandBars/XTPCommandBar.h>
#include <CommandBars/XTPControls.h>
#include <CommandBars/XTPControl.h>
#include <CommandBars/XTPControlButton.h>

#include <CommandBars/XTPTabClientWnd.h>
#include <CommandBars/TabClient/Workspace.h>
#include <CommandBars/TabClient/XTPControlTabWorkspace.h>

//////////////////////////////////////////////////////////////////////////
// CXTPControlTabWorkspace

IMPLEMENT_XTP_CONTROL(CXTPControlTabWorkspace, CXTPControlButton)

CXTPControlTabWorkspace::CXTPControlTabWorkspace()
{
	m_bForceRecalc = FALSE;
	m_nWidth = 220;

	SetFlags(xtpFlagSkipFocus | xtpFlagNoMovable);
}

CXTPControlTabWorkspace::~CXTPControlTabWorkspace()
{

}

void CXTPControlTabWorkspace::RedrawControl(LPCRECT lpRect, BOOL bAnimate)
{
	m_pParent->Redraw(lpRect, bAnimate);
}

CXTPTabPaintManager* CXTPControlTabWorkspace::GetPaintManager() const
{
	return CWorkspace::GetPaintManager();
}

CWnd* CXTPControlTabWorkspace::GetWindow() const
{
	return (CWnd*)GetParent();
}

void CXTPControlTabWorkspace::OnMouseMove(CPoint point)
{
	CXTPControl::OnMouseMove(point);

	if (!m_pTabClientWnd)
		return;

	PerformMouseMove(m_pParent->m_hWnd, point);
}

void CXTPControlTabWorkspace::OnRecalcLayout()
{
	m_bForceRecalc = TRUE;
	m_pParent->OnRecalcLayout();
	RedrawControl(GetRect(), FALSE);
}

XTPTabPosition CXTPControlTabWorkspace::GetPosition() const
{
	switch (m_pParent->GetPosition())
	{
		case xtpBarLeft: return xtpTabPositionLeft;
		case xtpBarRight: return xtpTabPositionRight;
		case xtpBarBottom: return xtpTabPositionBottom;
	}
	return xtpTabPositionTop;
}

void CXTPControlTabWorkspace::OnRemoved()
{
	if (m_pTabClientWnd && m_pTabClientWnd->m_pTabWorkspace == this)
	{
		m_pTabClientWnd->m_pTabWorkspace = NULL;
		m_pTabClientWnd->m_arrWorkspace.RemoveAll();
	}
}

void CXTPControlTabWorkspace::SetRect(CRect rcControl)
{
	if (rcControl == CXTPControl::m_rcControl && !m_bForceRecalc)
		return;

	m_bForceRecalc = FALSE;
	CXTPControl::SetRect(rcControl);

	CXTPTabPaintManager* pPaintManager = GetPaintManager();
	if (!pPaintManager)
		return;

	CClientDC dc(m_pParent);
	pPaintManager->RepositionTabControl(this, &dc, rcControl);
}

void CXTPControlTabWorkspace::Draw(CDC* pDC)
{
	CXTPTabPaintManager* pPaintManager = GetPaintManager();
	if (!pPaintManager)
	{
		CXTPControlButton::Draw(pDC);
		return;
	}
	pPaintManager->DrawTabControl(this, pDC, GetRect());
}

CSize CXTPControlTabWorkspace::GetSize(CDC* pDC)
{
	CXTPTabPaintManager* pPaintManager = GetPaintManager();
	if (!pPaintManager)
		return CXTPControl::GetSize(pDC);

	int nHeight = pPaintManager->GetAppearanceSet()->GetHeaderHeight(this);
	if (pPaintManager->m_bStaticFrame) nHeight += 2;

	return IsVerticalPosition(m_pParent->GetPosition()) ? CSize(nHeight, m_nWidth) : CSize(m_nWidth, nHeight);
}

INT_PTR CXTPControlTabWorkspace::OnToolHitTest(CPoint point, TOOLINFO* pTI) const
{
	if (!m_pTabClientWnd)
		return -1;

	CXTPTabManagerItem* pItem = HitTest(point);

	if (pItem)
	{
		if (GetPaintManager()->m_toolBehaviour == xtpTabToolTipNever)
			return 0;

		if (GetPaintManager()->m_toolBehaviour == xtpTabToolTipShrinkedOnly && !pItem->IsItemShrinked())
			return 0;

		CString strTip = GetItemTooltip(pItem);
		if (strTip.IsEmpty())
			return 0;

		INT_PTR nHit = pItem->GetIndex();

		CXTPToolTipContext::FillInToolInfo(pTI, GetParent()->GetSafeHwnd(), pItem->GetRect(), nHit, strTip);

		return nHit;
	}

	return 0;
}

void CXTPControlTabWorkspace::OnClick(BOOL bKeyboard, CPoint point)
{
	if (!m_pTabClientWnd)
	{
		CXTPControlButton::OnClick(bKeyboard, point);
		return;
	}

	if (!bKeyboard)
	{
		m_pTabClientWnd->m_bLockUpdate = TRUE;
#ifndef _XTP_ACTIVEX
		m_pTabClientWnd->m_bDelayLock = TRUE;
#endif

		PerformClick(m_pParent->m_hWnd, point);

#ifdef _XTP_ACTIVEX
		m_pTabClientWnd->m_bLockUpdate = FALSE;
#endif
	}
}
