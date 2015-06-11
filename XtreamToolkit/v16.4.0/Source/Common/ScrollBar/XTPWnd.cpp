#include "StdAfx.h"

#include <Common/XTPDrawHelpers.h>
#include <Common/ScrollBar/XTPScrollBase.h>
#include <Common/ScrollBar/XTPScrollBarCtrl.h>
#include <Common/ScrollBar/XTPWnd.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CXTPWnd, CWnd)


CXTPWnd::CXTPWnd()
{
	m_bEnableThemedScrollBar = TRUE;
	m_pwndScrollBar[SB_HORZ] = new CXTPScrollBarCtrl();
	m_pwndScrollBar[SB_VERT] = new CXTPScrollBarCtrl();
}

CXTPWnd::~CXTPWnd()
{
	SAFE_DELETE(m_pwndScrollBar[SB_HORZ]);
	SAFE_DELETE(m_pwndScrollBar[SB_VERT]);
}

BEGIN_MESSAGE_MAP(CXTPWnd, CWnd)
	//{{AFX_MSG_MAP(CXTPWnd)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CXTPWnd::PreCreateWindow(CREATESTRUCT &cs)
{
	return CWnd::PreCreateWindow(cs);
}

void CXTPWnd::PreSubclassWindow()
{
}

LRESULT CXTPWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (WM_CREATE == message)
	{
		//LPCREATESTRUCT lpCreateStruct = LPCREATESTRUCT(lParam);
		//OnCreate(lpCreateStruct);
		//return 0;
	}

	if (WM_NCCALCSIZE == message)
	{
		//LPNCCALCSIZE_PARAMS lpncsp = LPNCCALCSIZE_PARAMS(lParam);
		//OnNcCalcSize(FALSE, lpncsp);
		//return 0;
	}

	return CWnd::WindowProc(message, wParam, lParam);
}


int CXTPWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (-1 == CWnd::OnCreate(lpCreateStruct))
	{
		return -1;
	}

	m_pwndScrollBar[SB_HORZ]->Create(WS_CHILD | WS_VISIBLE | SBS_HORZ, CRect(0, 0, 0, 0), this, 100);
	m_pwndScrollBar[SB_VERT]->Create(WS_VISIBLE | SBS_VERT, CRect(0, 0, 0, 0), this, 101);

	return 0;
}

void CXTPWnd::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS *lpncsp)
{
	CWnd::OnNcCalcSize(bCalcValidRects, lpncsp);

	DWORD dwStyle = GetStyle();

	int cxHScroll = ::GetSystemMetrics(SM_CXVSCROLL);


	if ((WS_VSCROLL & dwStyle) && m_bEnableThemedScrollBar)
	{
		lpncsp->rgrc->right += cxHScroll;
	}
}

void CXTPWnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	DWORD dwStyle = GetStyle();

//  int cxHScroll = ::GetSystemMetrics(SM_CXHSCROLL);
	int cyHScroll = ::GetSystemMetrics(SM_CYHSCROLL);
	int cxVScroll = ::GetSystemMetrics(SM_CXVSCROLL);
//  int cyVScroll = ::GetSystemMetrics(SM_CYVSCROLL);

	BOOL bHScroll = (WS_HSCROLL & dwStyle);
	BOOL bVScroll = (WS_VSCROLL & dwStyle);

	if (::IsWindow(m_pwndScrollBar[SB_HORZ]->GetSafeHwnd()))
	{
		if (bHScroll)
		{
			m_pwndScrollBar[SB_HORZ]->MoveWindow(0, cy-cyHScroll, cx, cyHScroll);
			m_pwndScrollBar[SB_HORZ]->ShowWindow(SW_SHOW);
		}
		else
		{
			m_pwndScrollBar[SB_HORZ]->ShowWindow(SW_HIDE);
		}
	}

	if (::IsWindow(m_pwndScrollBar[SB_VERT]->GetSafeHwnd()))
	{
		if (bVScroll)
		{
			m_pwndScrollBar[SB_VERT]->MoveWindow(cx - cxVScroll, 0, cxVScroll, cy - (bHScroll ? cyHScroll : 0));
			m_pwndScrollBar[SB_VERT]->ShowWindow(SW_SHOW);
		}
		else
		{
			m_pwndScrollBar[SB_VERT]->ShowWindow(SW_HIDE);
		}
	}
}

CScrollBar* CXTPWnd::GetScrollBarCtrl(int nBar) const
{
	CScrollBar *pwndScrollBar = NULL;

	if (NULL != m_pwndScrollBar[nBar])
	{
		pwndScrollBar = m_pwndScrollBar[nBar];
	}
	else
	{
		pwndScrollBar = CWnd::GetScrollBarCtrl(nBar);
	}

	return pwndScrollBar;
}

void CXTPWnd::SetScrollBarTheme(XTPScrollBarTheme theme)
{
	m_pwndScrollBar[SB_HORZ]->SetTheme(theme);
	m_pwndScrollBar[SB_VERT]->SetTheme(theme);
}
