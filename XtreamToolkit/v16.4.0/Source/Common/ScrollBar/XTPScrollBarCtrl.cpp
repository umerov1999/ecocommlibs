// XTPScrollBar.cpp : implementation of the CXTPScrollBarCtrl class.
//
// This file is a part of the XTREME CONTROLS MFC class library.
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

#include <Common/XTPDrawHelpers.h>
#include <Common/XTPColorManager.h>
#include <Common/XTPResourceImage.h>
#include <Common/XTPWinThemeWrapper.h>

// Scroll bar
#include <Common/ScrollBar/XTPScrollBase.h>
#include <Common/ScrollBar/XTPScrollBarCtrl.h>
#include <Common/ScrollBar/XTPScrollBarPaintManager.h>

// Themes
#include <Common/ScrollBar/Themes/XTPScrollBarThemeWindowsClassic.h>
#include <Common/ScrollBar/Themes/XTPScrollBarThemeWindowsUx.h>
#include <Common/ScrollBar/Themes/XTPScrollBarThemeFlat.h>
#include <Common/ScrollBar/Themes/XTPScrollBarThemeUltraFlat.h>
#include <Common/ScrollBar/Themes/XTPScrollBarThemeOffice2000.h>
#include <Common/ScrollBar/Themes/XTPScrollBarThemeOfficeXP.h>
#include <Common/ScrollBar/Themes/XTPScrollBarThemeOffice2003.h>
#include <Common/ScrollBar/Themes/XTPScrollBarThemeOffice2013.h>
#include <Common/ScrollBar/Themes/XTPScrollBarThemeResource.h>
#include <Common/ScrollBar/Themes/XTPScrollBarThemeVisualStudio2012.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CXTPScrollBarCtrl, CScrollBar)

/////////////////////////////////////////////////////////////////////////////
// CXTPScrollBarCtrl
/////////////////////////////////////////////////////////////////////////////

CXTPScrollBarCtrl::CXTPScrollBarCtrl()
	: m_pPaintManager(NULL)
{
	m_bPreSubclassInit = TRUE;

	ZeroMemory(&m_spi, sizeof(m_spi));

	m_pSBTrack = NULL;

	SetTheme(xtpScrollBarThemeWindowsDefault);
}

CXTPScrollBarCtrl::~CXTPScrollBarCtrl()
{
	SAFE_DELETE(m_pPaintManager);
}

void CXTPScrollBarCtrl::SetTheme(XTPScrollBarTheme theme)
{
	SAFE_DELETE(m_pPaintManager);

	m_scrollBarStyle = xtpScrollStyleDefault;

	switch(theme)
	{
	case xtpScrollBarThemeWindowsClassic:
		m_pPaintManager = new CXTPScrollBarThemeWindowsClassic();
		break;

	case xtpScrollBarThemeOffice2000:
		m_pPaintManager = new CXTPScrollBarThemeOffice2000();
		break;

	case xtpScrollBarThemeOffice2003:
		m_pPaintManager = new CXTPScrollBarThemeOffice2003();
		break;

	case xtpScrollBarThemeResource:
		m_pPaintManager  = new CXTPScrollBarThemeResource();
		break;

	case xtpScrollBarThemeOffice2007Light:
		m_pPaintManager  = new CXTPScrollBarThemeResource();
		m_scrollBarStyle = xtpScrollStyleOffice2007Light;
		break;

	case xtpScrollBarThemeOffice2007Dark:
		m_pPaintManager  = new CXTPScrollBarThemeResource();
		m_scrollBarStyle = xtpScrollStyleOffice2007Dark;
		break;

	case xtpScrollBarThemeOfficeXP:
		m_pPaintManager = new CXTPScrollBarThemeOfficeXP();
		break;

	case xtpScrollBarThemeOffice2013:
		m_pPaintManager = new CXTPScrollBarThemeOffice2013();
		break;

	case xtpScrollBarThemeVisualStudio2012Light:
		m_pPaintManager  = new CXTPScrollBarThemeVisualStudio2012();
		m_scrollBarStyle = xtpScrollStyleVisualStudio2012Light;
		break;

	case xtpScrollBarThemeVisualStudio2012Dark:
		m_pPaintManager  = new CXTPScrollBarThemeVisualStudio2012();
		m_scrollBarStyle = xtpScrollStyleVisualStudio2012Dark;
		break;

	case xtpScrollBarThemeFlat:
		m_pPaintManager  = new CXTPScrollBarThemeFlat();
		m_scrollBarStyle = xtpScrollStyleFlat;
		break;

	case xtpScrollBarThemeUltraFlat:
		m_pPaintManager  = new CXTPScrollBarThemeUltraFlat();
		m_scrollBarStyle = xtpScrollStyleFlat;
		break;

	default:
		m_pPaintManager = new CXTPScrollBarThemeWindowsUx();
		break;
	}

	RefreshMetrics();
}




BEGIN_MESSAGE_MAP(CXTPScrollBarCtrl, CScrollBar)
//{{AFX_MSG_MAP(CXTPScrollBarCtrl)
	ON_WM_CREATE()
	ON_MESSAGE(WM_PRINTCLIENT, OnPrintClient)
	ON_WM_PAINT()
	ON_WM_ENABLE()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE_VOID(WM_MOUSELEAVE, OnMouseLeave)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_MESSAGE(SBM_SETSCROLLINFO, OnSetScrollInfo)
	ON_MESSAGE(SBM_GETSCROLLINFO, OnGetScrollInfo)
	ON_WM_SETFOCUS()
	ON_WM_CONTEXTMENU()
	ON_WM_SYSCOLORCHANGE()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CXTPScrollBarCtrl::Init()
{
	m_spi.fVert = GetStyle() & SBS_VERT;
	m_spi.fSizebox = GetStyle() & (SBS_SIZEBOX | SBS_SIZEGRIP);

	RefreshMetrics();
	Invalidate(FALSE);
}

void CXTPScrollBarCtrl::PreSubclassWindow()
{
	CScrollBar::PreSubclassWindow();

	if (m_bPreSubclassInit)
	{
		// Initialize the control.
		Init();
	}
}

int CXTPScrollBarCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CScrollBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Initialize the control.
	Init();

	return 0;
}

BOOL CXTPScrollBarCtrl::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CScrollBar::PreCreateWindow(cs))
		return FALSE;

	// When creating controls dynamically Init() must
	// be called from OnCreate() and not from
	// PreSubclassWindow().

	m_bPreSubclassInit = FALSE;

	return TRUE;
}

void CXTPScrollBarCtrl::OnSysColorChange()
{
	CScrollBar::OnSysColorChange();
	XTPColorManager()->RefreshColors();

	RefreshMetrics();

	Invalidate(FALSE);
}


//////////////////////////////////////////////////////////////////////////
// Scrolling


CRect CXTPScrollBarCtrl::GetScrollBarRect()
{
	return CXTPClientRect(this);
}

void CXTPScrollBarCtrl::GetScrollInfo(SCROLLINFO *psi)
{
	psi->cbSize = sizeof(SCROLLINFO);

	psi->fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
	::GetScrollInfo(m_hWnd, SB_CTL, psi);
}

BOOL CXTPScrollBarCtrl::IsScrollBarEnabled() const
{
	return IsWindowEnabled();
}

CWnd* CXTPScrollBarCtrl::GetParentWindow() const
{
	return GetParent();
}

void CXTPScrollBarCtrl::DoScroll(int cmd, int pos)
{
	if(!m_spi.fSizebox)
	{
		GetParent()->SendMessage((UINT)(m_spi.fVert ? WM_VSCROLL : WM_HSCROLL),
			MAKELONG(cmd, pos), (LPARAM)m_hWnd);
	}
}

void CXTPScrollBarCtrl::RedrawScrollBar()
{
	if (m_hWnd) Invalidate(FALSE);
}


void CXTPScrollBarCtrl::OnEnable(BOOL /*bEnable*/)
{
	Invalidate(FALSE);
}

void CXTPScrollBarCtrl::OnMouseLeave()
{
	OnMouseMove(0, CPoint(-1, -1));
}

void CXTPScrollBarCtrl::OnMouseMove(UINT /*nFlags*/, CPoint point)
{
	int ht = HitTestScrollBar(point);

	if (ht != m_spi.ht)
	{
		m_spi.ht = ht;
		Invalidate(FALSE);

		if (m_spi.ht != HTNOWHERE)
		{
			TRACKMOUSEEVENT tme = {sizeof(TRACKMOUSEEVENT), TME_LEAVE, m_hWnd, HOVER_DEFAULT};
			_TrackMouseEvent(&tme);
		}
	}
}

void CXTPScrollBarCtrl::OnLButtonDown(UINT /*nFlags*/, CPoint point)
{
	if ((GetStyle() & WS_TABSTOP))
		SetFocus();

	PerformTrackInit(m_hWnd, point, &m_spi, (GetKeyState(VK_SHIFT) < 0) ? TRUE : FALSE);
}

void CXTPScrollBarCtrl::OnContextMenu(CWnd* pWnd, CPoint pos)
{
	CScrollBar::OnContextMenu(pWnd, pos);
	Invalidate(FALSE);
}

void CXTPScrollBarCtrl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	OnLButtonDown(nFlags, point);
}

LRESULT CXTPScrollBarCtrl::OnSetScrollInfo(WPARAM wParam, LPARAM lParam)
{
	LRESULT bResult = DefWindowProc(SBM_SETSCROLLINFO, FALSE, lParam);

	if (wParam)
	{
		Invalidate(FALSE);
		UpdateWindow();
	}

	return bResult;
}

LRESULT CXTPScrollBarCtrl::OnGetScrollInfo(WPARAM wParam, LPARAM lParam)
{
	LRESULT bResult = DefWindowProc(SBM_GETSCROLLINFO, wParam, lParam);

	LPSCROLLINFO lpsi = (LPSCROLLINFO)lParam;

	if (lpsi && (lpsi->fMask & SIF_TRACKPOS) && m_pSBTrack)
	{
		lpsi->nTrackPos = m_pSBTrack->posNew;
	}

	return bResult;
}


void CXTPScrollBarCtrl::OnSetFocus(CWnd *pOldWnd)
{
	UNREFERENCED_PARAMETER(pOldWnd);
}


void CXTPScrollBarCtrl::RefreshMetrics()
{
	if (NULL != m_pPaintManager)
	{
		m_pPaintManager->RefreshMetrics();
	}
}



//////////////////////////////////////////////////////////////////////////
// Drawing.

void CXTPScrollBarCtrl::OnPaint()
{
	CPaintDC dc(this);
	CXTPBufferDC dcBuffer(dc);
	OnDraw(&dcBuffer);
}

void CXTPScrollBarCtrl::OnDraw(CDC *pDC)
{
	m_spi.fVert = GetStyle() & SBS_VERT;
	m_spi.fSizebox = GetStyle() & (SBS_SIZEBOX | SBS_SIZEGRIP);

	if (!m_pSBTrack || !m_pSBTrack->bTrackThumb)
		SetupScrollInfo();

	GetScrollBarPaintManager()->DrawScrollBar(pDC, this);
}

LRESULT CXTPScrollBarCtrl::OnPrintClient(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	CDC *pDC = CDC::FromHandle(HDC(wParam));

	if (NULL != pDC)
	{
		OnDraw(pDC);
	}

	return 1;
}
