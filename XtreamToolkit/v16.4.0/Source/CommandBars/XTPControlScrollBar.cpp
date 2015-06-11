// XTPControlScrollBar.cpp : implementation of the CXTPControlScrollBar class.
//
// This file is a part of the XTREME COMMANDBARS MFC class library.
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

#include "StdAfx.h"

#include "Common/XTPPropExchange.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPHookManager.h"
#include <Common/ScrollBar/XTPScrollBase.h>
#include <Common/ScrollBar/XTPScrollBarPaintManager.h>

#include <CommandBars/XTPCommandBarsDefines.h>
#include <CommandBars/XTPControl.h>
#include <CommandBars/XTPControlScrollBar.h>
#include <CommandBars/XTPPaintManager.h>
#include <CommandBars/XTPCommandBar.h>


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_XTP_CONTROL(CXTPControlScrollBar, CXTPControl)

CXTPControlScrollBar::CXTPControlScrollBar()
{
	m_nWidth = 150;

	m_spi.fVert = FALSE;
	m_spi.fSizebox = FALSE;

	m_nMax = 100;
	m_nMin = 0;
	m_nPos = 0;
	m_nPage = 0;

	m_bInScroll = FALSE;

	m_scrollBarStyle = xtpScrollStyleDefault;
}

CXTPControlScrollBar::~CXTPControlScrollBar()
{
}

void CXTPControlScrollBar::Copy(CXTPControl* pControl, BOOL bRecursive)
{
	CXTPControl::Copy(pControl, bRecursive);

	CXTPControlScrollBar* pControlScrollBar = DYNAMIC_DOWNCAST(CXTPControlScrollBar, pControl);
	if (!pControlScrollBar)
		return;

	SetRange(pControlScrollBar->m_nMin, pControlScrollBar->m_nMax);
	SetPos(pControlScrollBar->GetPos());
	m_nPage = pControlScrollBar->m_nPage;
	m_scrollBarStyle = pControlScrollBar->m_scrollBarStyle;

}

void CXTPControlScrollBar::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPControl::DoPropExchange(pPX);

	PX_Int(pPX, _T("Min"), m_nMin, 0);
	PX_Int(pPX, _T("Max"), m_nMax, 100);
	PX_Int(pPX, _T("Pos"), m_nPos, 0);
	PX_Enum(pPX, _T("ScrollBarStyle"), m_scrollBarStyle, xtpScrollStyleDefault);
}

CSize CXTPControlScrollBar::GetSize(CDC* /*pDC*/)
{
	CSize size;
	if(!m_spi.fSizebox)
	{
		if (m_spi.fVert)
		{
			size.cx = GetScrollBarPaintManager()->m_cxVScroll + 4;
			size.cy = GetWidth();
		}
		else
		{
			size.cx = GetWidth();
			size.cy = GetScrollBarPaintManager()->m_cyHScroll + 4;
		}
	}
	else
	{
		size = GetRect().Size();
	}

	return size;
}

void CXTPControlScrollBar::Draw(CDC* pDC)
{
	if (!m_pSBTrack || !m_pSBTrack->bTrackThumb)
		SetupScrollInfo();

	GetScrollBarPaintManager()->DrawScrollBar(pDC, this);
}

void CXTPControlScrollBar::SetPos(int nPos)
{
	if (m_pSBTrack != NULL && !m_bInScroll)
		return;

	m_nPos = nPos;

	RedrawParent(FALSE);
}

int CXTPControlScrollBar::GetPos() const
{
	return m_nPos;
}

void CXTPControlScrollBar::SetRange( int nMin, int nMax)
{
	if ((m_nMin != nMin) || (m_nMax != nMax))
	{
		m_nMin = nMin;
		m_nMax = nMax;
		RedrawParent(FALSE);
	}
}

BOOL CXTPControlScrollBar::HasFocus() const
{
	return m_pSBTrack != NULL;
}

void CXTPControlScrollBar::OnCalcDynamicSize(DWORD dwMode)
{
	m_spi.fVert = (dwMode & LM_VERTDOCK);
	m_spi.fSizebox = FALSE;
}

void CXTPControlScrollBar::OnClick(BOOL bKeyboard, CPoint point)
{
	if (IsCustomizeMode() && !bKeyboard)
	{
		CustomizeStartDrag(point);
	}
	else if (!bKeyboard)
	{
		m_pParent->SetPopuped(-1);
		if (m_pParent->IsTrackingMode() == TRUE_POPUP) m_pParent->SetTrackingMode(TRUE, FALSE, FALSE);
		m_pParent->SetSelected(GetIndex());

		PerformTrackInit(m_pParent->m_hWnd, point, &m_spi, (GetKeyState(VK_SHIFT) < 0) ? TRUE : FALSE);
	}
}

void CXTPControlScrollBar::OnScroll(UINT nSBCode, UINT nPos)
{
	int nCurPos = GetPos();

	switch (nSBCode)
	{
		case SB_TOP: nCurPos = m_nMin; break;
		case SB_BOTTOM: nCurPos = m_nMax; break;

		case SB_LINEUP: nCurPos = max(nCurPos - 1, m_nMin); break;
		case SB_LINEDOWN: nCurPos = min(nCurPos + 1, m_nMax); break;

		case SB_THUMBTRACK: nCurPos = nPos; break;
		case SB_PAGEUP: nCurPos = max(nCurPos - m_nMax / 10, m_nMin); break;
		case SB_PAGEDOWN: nCurPos = min(nCurPos + m_nMax / 10, m_nMax); break;
	}

	if (nCurPos == m_nPos)
		return;

	m_nPos = nCurPos;
	RedrawParent(FALSE);

	NotifySite(CBN_XTP_EXECUTE);
}

//////////////////////////////////////////////////////////////////////////
// CXTPControlScrollBar::Base

CRect CXTPControlScrollBar::GetScrollBarRect()
{
	CRect rcItem(GetRect());
	rcItem.DeflateRect(2, 2, 2, 2);

	if (m_pParent->GetType() == xtpBarTypePopup && m_pParent->GetPosition() == xtpBarPopup)
	{
		CXTPPaintManager* pPaintManager = GetPaintManager();
		rcItem.left += pPaintManager->GetPopupBarGripperWidth(GetParent())
			+ pPaintManager->m_nPopupBarTextPadding - 2;
	}

	return rcItem;
}

void CXTPControlScrollBar::GetScrollInfo(SCROLLINFO* psi)
{
	psi->cbSize = sizeof(SCROLLINFO);

	psi->fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
	psi->nMax = m_nMax;
	psi->nMin = m_nMin;
	psi->nPage = m_nPage;
	psi->nPos = m_nPos;
}

BOOL CXTPControlScrollBar::IsScrollBarEnabled() const
{
	return GetEnabled();
}

CWnd* CXTPControlScrollBar::GetParentWindow() const
{
	return m_pParent;
}

CXTPScrollBarPaintManager* CXTPControlScrollBar::GetScrollBarPaintManager() const
{
	return m_scrollBarStyle == xtpScrollStyleSlider ?
		(CXTPScrollBarPaintManager*)GetPaintManager()->GetSliderPaintManager() :
		(CXTPScrollBarPaintManager*)GetPaintManager()->GetScrollBarPaintManager();
}

void CXTPControlScrollBar::DoScroll(int cmd, int pos)
{

#ifndef _XTP_ACTIVEX
	m_bInScroll = TRUE;

	CWnd* pParent = GetParent()->GetOwnerSite();

	NMXTPSCROLL nmScroll;
	nmScroll.hdr.code = XTP_SBN_SCROLL;
	nmScroll.hdr.idFrom = GetID();
	nmScroll.hdr.hwndFrom = GetParent()->GetSafeHwnd();
	nmScroll.pSender = this;
	nmScroll.nPos = pos;
	nmScroll.nSBCode = cmd;

	LRESULT lResult = 0;
	AFX_NOTIFY notify;
	notify.pResult = &lResult;
	notify.pNMHDR = (NMHDR*)&nmScroll.hdr;

	pParent->OnCmdMsg(GetID(), MAKELONG(XTP_SBN_SCROLL, WM_NOTIFY), &notify, NULL);

	m_bInScroll = FALSE;

	if (*notify.pResult)
		return;

#endif

	OnScroll(cmd, pos);
}

void CXTPControlScrollBar::RedrawScrollBar()
{
	RedrawParent(FALSE);
}

void CXTPControlScrollBar::OnMouseMove(CPoint point)
{
	int ht = HitTestScrollBar(point);

	if (ht != m_spi.ht)
	{
		m_spi.ht = ht;
		RedrawParent(FALSE);
	}
}


#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPControlScrollBar, CXTPControl)
	DISP_PROPERTY_EX_ID(CXTPControlScrollBar, "Pos", 200, GetPos, SetPos, VT_I4)
	DISP_PROPERTY_NOTIFY_ID(CXTPControlScrollBar, "Min", 201, m_nMin, OleRangeChanged, VT_I4)
	DISP_PROPERTY_NOTIFY_ID(CXTPControlScrollBar, "Max", 202, m_nMax, OleRangeChanged, VT_I4)
	DISP_PROPERTY_ID(CXTPControlScrollBar, "ScrollBarStyle", 204, m_scrollBarStyle, VT_I4)
END_DISPATCH_MAP()

// {72568A35-789A-40d9-B564-7577DA549319}
static const GUID IID_ICommandBarScrollBar =
{ 0x72568a35, 0x789a, 0x40d9, { 0xb5, 0x64, 0x75, 0x77, 0xda, 0x54, 0x93, 0x19 } };

BEGIN_INTERFACE_MAP(CXTPControlScrollBar, CXTPControl)
	INTERFACE_PART(CXTPControlScrollBar, IID_ICommandBarScrollBar, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPControlScrollBar, IID_ICommandBarScrollBar)

void CXTPControlScrollBar::OleRangeChanged()
{
	SetRange(m_nMin, m_nMax);
}

#endif
