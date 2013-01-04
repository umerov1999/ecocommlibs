// XTPMarqueeCtrl.cpp : implementation file
//
// This file is a part of the XTREME CONTROLS MFC class library.
// (c)1998-2012 Codejock Software, All Rights Reserved.
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

#include "Common/XTPWinThemeWrapper.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPColorManager.h"

#include "XTPMarqueeCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifndef PBS_MARQUEE
#define PBS_MARQUEE 0x08
#endif

#define PERCENT2BYTE(i) (BYTE)((i*255)/100)

CXTPMarqueeCtrl::CXTPMarqueeCtrl()
: m_nPos(0)
, m_nGap(0)
, m_cxChunk(12)
, m_bWinThemed(FALSE)
{
	m_chAlpha[0] = PERCENT2BYTE(25);
	m_chAlpha[1] = PERCENT2BYTE(50);
	m_chAlpha[2] = PERCENT2BYTE(75);
	m_chAlpha[3] = PERCENT2BYTE(100);
	m_chAlpha[4] = PERCENT2BYTE(100);

	m_themeProgress = new CXTPWinThemeWrapper();
}

CXTPMarqueeCtrl::~CXTPMarqueeCtrl()
{
	SAFE_DELETE(m_themeProgress);
}

IMPLEMENT_DYNAMIC(CXTPMarqueeCtrl, CProgressCtrl)

BEGIN_MESSAGE_MAP(CXTPMarqueeCtrl, CProgressCtrl)
	//{{AFX_MSG_MAP(CXTPMarqueeCtrl)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// CXTPMarqueeCtrl auto-initialization
//---------------------------------------------------------------------------

BOOL CXTPMarqueeCtrl::OnInitControl()
{
	if (::IsWindow(m_hWnd))
	{
		// initialize the progress bar.
		SetMarquee();
		return TRUE;
	}

	return FALSE;
}

void CXTPMarqueeCtrl::PreSubclassWindow()
{
	CProgressCtrl::PreSubclassWindow();

	if (m_bIsSubclassed)
	{
		// Initialize the control.
		OnInitControl();
	}
}

int CXTPMarqueeCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CProgressCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Initialize the control.
	OnInitControl();

	return 0;
}

BOOL CXTPMarqueeCtrl::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CProgressCtrl::PreCreateWindow(cs))
		return FALSE;

	// When creating controls dynamically InitControl() must
	// be called from OnCreate() and not from
	// PreSubclassWindow().

	m_bIsSubclassed = FALSE;

	return TRUE;
}

void CXTPMarqueeCtrl::RefreshMetrics()
{
	m_themeProgress->OpenThemeData(0, L"Progress");

	m_bWinThemed = UseWinThemes();

	// set classic windows colors.
	m_crBack.SetStandardValue(::GetSysColor(m_bWinThemed? COLOR_WINDOW: COLOR_3DFACE));
	m_crChunk.SetStandardValue(::GetSysColor(COLOR_HIGHLIGHT));

	m_nPos = 0;
}

// CXTPMarqueeCtrl drawing.
//---------------------------------------------------------------------------

void CXTPMarqueeCtrl::OnDrawBackground(CDC* pDC, CRect rcClient)
{
	pDC->FillSolidRect(&rcClient, m_crBack);

	if (m_bWinThemed)
	{
		m_themeProgress->DrawThemeBackground(pDC->GetSafeHdc(),
			IsVertical()? PP_BARVERT: PP_BAR, PBS_NORMAL, &rcClient, NULL);
	}


}

void CXTPMarqueeCtrl::OnDrawMarquee(CDC* pDC, CRect rcClient)
{
	int nEdge = (m_bWinThemed? 3: 1);

	rcClient.DeflateRect(nEdge, nEdge);
	pDC->IntersectClipRect(rcClient);

	// calculate marquee size.
	int x  = IsVertical()? rcClient.left: m_nPos;
	int y  = IsVertical()? m_nPos : rcClient.top;

	// determine the dimensions for the bitmap.
	int cx = IsVertical()? rcClient.Width(): m_cxChunk;
	int cy = IsVertical()? m_cxChunk: rcClient.Height();

	// create a memory device context, and select the bitmap into it.

	CBitmap bmpChunk;
	bmpChunk.CreateCompatibleBitmap(pDC, cx, cy);

	CXTPCompatibleDC dcChunk(pDC, &bmpChunk);

	for (int i = 0; i < _countof(m_chAlpha); ++ i)
	{
		dcChunk.BitBlt(0, 0, cx, cy, pDC, x, y, SRCCOPY);

		// draw the chunk using the memory dc.
		CXTPEmptyRect rcChunk;

		if (IsVertical())
		{
			rcChunk.right  = cx;
			rcChunk.bottom = m_cxChunk - (IsSmooth() ? 0 : m_nGap);
		}
		else
		{
			rcChunk.right  = m_cxChunk - (IsSmooth() ? 0 : m_nGap);
			rcChunk.bottom = cy;
		}

		if (m_bWinThemed)
		{
			m_themeProgress->DrawThemeBackground(dcChunk,
				IsVertical()? PP_CHUNKVERT: PP_CHUNK, PBS_NORMAL, &rcChunk, NULL);
		}
		else
		{
			dcChunk.FillSolidRect(rcChunk, m_crChunk);
		}

		if (XTPDrawHelpers()->m_pfnAlphaBlend)
		{
			BLENDFUNCTION bf;
			bf.BlendOp = AC_SRC_OVER;
			bf.BlendFlags = 0;
			bf.SourceConstantAlpha = m_chAlpha[i];
			bf.AlphaFormat = 0;

			XTPDrawHelpers()->m_pfnAlphaBlend(pDC->GetSafeHdc(), x, y, cx, cy, dcChunk, 0, 0, cx, cy, bf);
		}
		else
		{
			::BitBlt(pDC->GetSafeHdc(), x, y, cx, cy, dcChunk, 0, 0, SRCCOPY);
		}

		if (IsVertical())
		{
			y += cy;
		}
		else
		{
			x += cx;
		}
	}
}

void CXTPMarqueeCtrl::OnPaint()
{
	if (IsMarquee())
	{
		CXTPClientRect rcClient(this);

		CPaintDC dc(this);
		CXTPBufferDC dcMem(dc);

		OnDrawBackground(&dcMem, rcClient);
		OnDrawMarquee(&dcMem, rcClient);
	}
	else
	{
		CProgressCtrl::OnPaint();
	}
}

BOOL CXTPMarqueeCtrl::OnEraseBkgnd(CDC* pDC)
{
	if (IsMarquee())
	{
		return TRUE;
	}
	else
	{
		return CProgressCtrl::OnEraseBkgnd(pDC);
	}
}


// CXTPMarqueeCtrl message handling.
//---------------------------------------------------------------------------

BOOL CXTPMarqueeCtrl::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	switch (message)
	{
	case WM_SETTINGCHANGE:
	case WM_SYSCOLORCHANGE:
		{
			RefreshMetrics();
			RedrawWindow();
		}
		break;
	}

	return CProgressCtrl::OnWndMsg(message, wParam, lParam, pResult);
}

void CXTPMarqueeCtrl::OnTimer(UINT_PTR nIDEvent)
{
	if (IsMarquee())
	{
		if (nIDEvent == 1)
		{
			CXTPClientRect rcClient(this);

			// calculate current position.
			if (IsVertical())
			{
				int cy = m_cxChunk;
				m_nPos += cy;

				if (m_nPos >= (rcClient.bottom - cy))
					m_nPos = -(((cy*5)-2)+cy);
			}
			else
			{
				int cx = m_cxChunk;
				m_nPos += cx;

				if (m_nPos >= (rcClient.right - cx))
					m_nPos = -(((cx*5)-2)+cx);
			}

			RedrawWindow();
		}
	}
	else
	{
		CProgressCtrl::OnTimer(nIDEvent);
	}
}

BOOL CXTPMarqueeCtrl::UseWinThemes()
{
	if (::IsWindow(m_hWnd))
	{
		if (!m_themeProgress->IsAppThemeReady())
			return FALSE;

		return m_themeProgress->ThemeDataOpen();
	}

	return FALSE;
}

BOOL CXTPMarqueeCtrl::IsSmooth()
{
	return (GetStyle() & PBS_SMOOTH) != 0;
}

BOOL CXTPMarqueeCtrl::IsVertical()
{
	return (GetStyle() & PBS_VERTICAL) != 0;
}

BOOL CXTPMarqueeCtrl::IsMarquee()
{
	return (GetStyle() & PBS_MARQUEE) != 0;
}

void CXTPMarqueeCtrl::SetColors(COLORREF crBack, COLORREF crChunk)
{
	m_crBack = crBack;
	::SendMessage(m_hWnd, PBM_SETBKCOLOR, 0, (LPARAM)m_crBack);

	m_crChunk = crChunk;
	::SendMessage(m_hWnd, PBM_SETBARCOLOR, 0, (LPARAM)m_crChunk);
}

void CXTPMarqueeCtrl::SetMarquee(BOOL bMarqueeMode /*=TRUE*/, int nInterval /*=50*/, int nGap /*=2*/, int cx /*=12*/, int* piTrans /*=NULL*/)
{
	if (bMarqueeMode)
	{
		// set transparency.
		if (piTrans)
		{
			for (int i = 0; i < _countof(m_chAlpha); ++i)
			{
				m_chAlpha[i] = PERCENT2BYTE(piTrans[i]);
			}
		}

		// set the progress chunk size.
		m_cxChunk = cx;

		// set chunk gap and create bitmaps.
		m_nGap = nGap;
		RefreshMetrics();

		// set the marquee style and start timer.
		ModifyStyle(0, PBS_MARQUEE);
		SetTimer(1, nInterval, NULL);
	}
	else
	{
		// stop the timer and remove marquee style.
		KillTimer(1);
		ModifyStyle(PBS_MARQUEE, 0);
	}

	RedrawWindow();
}
