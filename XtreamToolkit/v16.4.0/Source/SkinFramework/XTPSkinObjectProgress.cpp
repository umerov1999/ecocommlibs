// XTPSkinObjectProgress.cpp: implementation of the CXTPSkinObjectProgress class.
//
// This file is a part of the XTREME SKINFRAMEWORK MFC class library.
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

#include "stdafx.h"

#include <Common/XTPWinThemeWrapper.h>
#include <Common/XTPDrawHelpers.h>
#include <Common/XTPSystemHelpers.h>
#include <Common/ScrollBar/XTPScrollBase.h>
#include <Common/XTPSynchro.h>

#include "XTPSkinObject.h"
#include "XTPSkinObjectFrame.h"
#include "XTPSkinObjectProgress.h"
#include "XTPSkinManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#ifndef PBS_MARQUEE
#define PBS_MARQUEE             0x08
#endif

#ifndef PBM_SETMARQUEE
#define PBM_SETMARQUEE          (WM_USER+10)
#endif

#define TID_TIMER  102

//////////////////////////////////////////////////////////////////////////
// CXTPSkinObjectProgress
IMPLEMENT_DYNCREATE(CXTPSkinObjectProgress, CXTPSkinObjectFrame)

CXTPSkinObjectProgress::CXTPSkinObjectProgress()
{
	m_strClassName = _T("PROGRESS");

	m_nMarqueePos = 0;
	m_nMarqueeDelay = 0;
}

CXTPSkinObjectProgress::~CXTPSkinObjectProgress()
{

}


BEGIN_MESSAGE_MAP(CXTPSkinObjectProgress, CXTPSkinObjectFrame)
	//{{AFX_MSG_MAP(CXTPSkinObjectProgress)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_MESSAGE(WM_PRINTCLIENT, OnPrintClient)
	ON_MESSAGE(PBM_SETMARQUEE, OnStartMarquee)
	ON_MESSAGE(PBM_STEPIT, OnStepIt)
	ON_MESSAGE(PBM_SETPOS, OnSetPos)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTPSkinObjectProgress message handlers


int CXTPSkinObjectProgress::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CXTPSkinObjectFrame::OnCreate(lpCreateStruct) == -1)
		return -1;

	if ((GetExStyle() & WS_EX_STATICEDGE) && m_pManager->IsEnabled())
	{
		ModifyStyleEx(WS_EX_STATICEDGE, 0);
		SetWindowPos(NULL, 0,0,0,0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);
	}

	return 0;
}

void CXTPSkinObjectProgress::OnSkinChanged(BOOL bPrevState, BOOL bNewState)
{
	CXTPSkinObjectFrame::OnSkinChanged(bPrevState, bNewState);

	BOOL bRemovedStaticEdge = m_pManager->IsEnabled() || CXTPWinThemeWrapper().IsThemeActive();
	BOOL bStaticEdgeRemoved = (GetExStyle() & WS_EX_STATICEDGE) == 0;

	if (bStaticEdgeRemoved != bRemovedStaticEdge)
	{
		if (bRemovedStaticEdge)
			ModifyStyleEx(WS_EX_STATICEDGE, 0);
		else
			ModifyStyleEx(0, WS_EX_STATICEDGE);

		SetWindowPos(NULL, 0,0,0,0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);
	}
}

BOOL CXTPSkinObjectProgress::OnEraseBkgnd(CDC* pDC)
{
	BOOL bResult = TRUE;
	if (!IsDrawingEnabled())
	{
		bResult = CXTPSkinObjectFrame::OnEraseBkgnd(pDC);
	}
	return bResult;
}

LRESULT CXTPSkinObjectProgress::OnPrintClient(WPARAM wParam, LPARAM lParam)
{
	if ((lParam & PRF_CLIENT) == 0 || !IsDrawingEnabled())
		return Default();

	CDC* pDC = CDC::FromHandle((HDC)wParam);
	if (pDC) OnDraw(pDC);
	return 1;
}

void CXTPSkinObjectProgress::OnPaint()
{
	if (IsDrawingEnabled())
	{
		CXTPSkinObjectPaintDC dc(this); // device context for painting
		OnDraw(&dc);
	}
	else
	{
		CXTPSkinObjectFrame::OnPaint();
	}
}

void CXTPSkinObjectProgress::OnDraw(CDC* pDC)
{
	CRect rcClient;
	GetClientRect(&rcClient);

	CXTPBufferDC dcMem(*pDC, rcClient);

	dcMem.FillSolidRect(rcClient, GetColor(COLOR_3DFACE));

	DWORD dwStyle = GetStyle();
	BOOL bVert = dwStyle & PBS_VERTICAL;
	int nBarPart = bVert ? PP_BARVERT : PP_BAR;
	int nChunkPart = bVert ? PP_CHUNKVERT : PP_CHUNK;

	CXTPSkinManagerClass* pClassProgress = GetSkinClass();
	pClassProgress->DrawThemeBackground(&dcMem, nBarPart, 0,  rcClient);

	CRect rc = rcClient;

	CRect rcMargins = pClassProgress->GetThemeRect(nBarPart, 0,  TMT_SIZINGMARGINS);

	rc.DeflateRect(rcMargins);

	int iWidth, dxBlock;

	if (GetStyle() & PBS_MARQUEE)
	{
		rcClient = rc;

		dxBlock = pClassProgress->GetThemeInt(0, 0, TMT_PROGRESSCHUNKSIZE, rc.Height() * 2 / 3);
		int dxSpace = pClassProgress->GetThemeInt(0, 0, TMT_PROGRESSSPACESIZE, 2);

		rc.left = m_nMarqueePos + rcClient.left;

		if (dxBlock == 1 && dxSpace == 0)
		{
			dxBlock = 8;

			rc.right = rc.left + 5 * dxBlock;

			if (rc.right > rcClient.right)
			{
				CRect rcPart(rcClient.left, rc.top, rcClient.left + rc.right - rcClient.right, rc.bottom);
				pClassProgress->DrawThemeBackground(&dcMem, nChunkPart, 0,  rcPart);

				rc.right = rcClient.right;
			}

			pClassProgress->DrawThemeBackground(&dcMem, nChunkPart, 0,  rc);
			return;
		}

		for (int i = 0; i < 5; i++)
		{
			if (rc.left >= rcClient.right)
				rc.left = rcClient.left;

			rc.right = rc.left + dxBlock;

			if (rc.right > rcClient.right)
				rc.right = rcClient.right;

			pClassProgress->DrawThemeBackground(&dcMem, nChunkPart, 0,  rc);
			rc.left = rc.right + dxSpace;
		}
		return;
	}

	PBRANGE range;
	SendMessage(PBM_GETRANGE, (WPARAM) FALSE, (LPARAM) &range);

	int iLow = range.iLow;
	int iHigh = range.iHigh;
	int iPos =  (int)SendMessage(PBM_GETPOS);


	if (bVert)
	{
		iWidth = rc.Height();
		dxBlock = (rcClient.right - rcClient.left) * 2 / 3;
	}
	else
	{
		iWidth = rc.Width();
		dxBlock = (rcClient.bottom - rcClient.top) * 2 / 3;
	}

	int x = MulDiv(iWidth, iPos - iLow, iHigh - iLow);

	dxBlock = pClassProgress->GetThemeInt(0, 0, TMT_PROGRESSCHUNKSIZE, dxBlock);
	int dxSpace = pClassProgress->GetThemeInt(0, 0, TMT_PROGRESSSPACESIZE, 2);


	if (dxBlock == 0)
		dxBlock = 1;

	if (dxBlock == 1 && dxSpace == 0)
	{
		if (bVert)
		{
			rc.top = rc.bottom - x;
		}
		else
		{
			rc.right = rc.left + x;
		}
		pClassProgress->DrawThemeBackground(&dcMem, nChunkPart, 0,  rc);
		return;
	}

	int nBlocks = (x + (dxBlock + dxSpace) - 1) / (dxBlock + dxSpace);

	for (int i = 0; i < nBlocks; i++)
	{
		if (bVert)
		{
			rc.top = rc.bottom - dxBlock;

			if (rc.bottom <= rcClient.top)
				break;

			if (rc.top <= rcClient.top)
				rc.top = rcClient.top + 1;

		}
		else
		{
			rc.right = rc.left + dxBlock;

			if (rc.left >= rcClient.right)
				break;

			if (rc.right >= rcClient.right)
				rc.right = rcClient.right - 1;
		}

		pClassProgress->DrawThemeBackground(&dcMem, nChunkPart, 0,  rc);


		if (bVert)
			rc.bottom = rc.top - dxSpace;
		else
			rc.left = rc.right + dxSpace;
	}
}

LRESULT CXTPSkinObjectProgress::OnStepIt(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	if (((GetStyle() & PBS_MARQUEE) == 0))
		return Default();

	DoStep(1);
	return 1;
}

LRESULT CXTPSkinObjectProgress::OnSetPos(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	LRESULT lResult = Default();

	Invalidate(FALSE);
	return lResult;
}


LRESULT CXTPSkinObjectProgress::OnStartMarquee(WPARAM wParam, LPARAM lParam)
{
	m_nMarqueePos = 0;
	m_nMarqueeDelay = 0;

	if (((GetStyle() & PBS_MARQUEE) == 0))
		return Default();

	BOOL bMarquee = wParam && (lParam > 0);
	m_nMarqueePos = 0;

	KillTimer(TID_TIMER);

	if (bMarquee)
	{
		m_nMarqueeDelay = (UINT)lParam;
		SetTimer(TID_TIMER, m_nMarqueeDelay, NULL);
	}

	return 1;
}

void CXTPSkinObjectProgress::DoStep(int nStep)
{
	CXTPClientRect rcClient(this);

	CXTPSkinManagerClass* pClassProgress = GetSkinClass();

	DWORD dwStyle = GetStyle();
	BOOL bVert = dwStyle & PBS_VERTICAL;
	CRect rcMargins = pClassProgress->GetThemeRect(bVert ? PP_BARVERT : PP_BAR, 0,  TMT_SIZINGMARGINS);

	rcClient.DeflateRect(rcMargins);

	int dxBlock = pClassProgress->GetThemeInt(0, 0, TMT_PROGRESSCHUNKSIZE, rcClient.Height() * 2 / 3);
	int dxSpace = pClassProgress->GetThemeInt(0, 0, TMT_PROGRESSSPACESIZE, 2);

	if (dxBlock == 1 && dxSpace == 0)
		dxBlock = 8;

	m_nMarqueePos += (dxBlock + dxSpace) * nStep;

	if (m_nMarqueePos > rcClient.right)
		m_nMarqueePos = rcClient.left + m_nMarqueePos - rcClient.right;

	Invalidate(FALSE);
}

void CXTPSkinObjectProgress::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == TID_TIMER && (GetStyle() & PBS_MARQUEE))
	{
		DoStep(m_nMarqueeDelay > 50 ? 1 : 2);
		return;
	}

	CWnd::OnTimer(nIDEvent);
}
