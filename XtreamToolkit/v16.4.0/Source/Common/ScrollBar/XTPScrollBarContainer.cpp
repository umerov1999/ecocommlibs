// XTPScrollBarContainer.cpp: CXTPScrollBarContainer class.
//
// This file is a part of the XTREME TOOLKIT PRO MFC class library.
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
#include <Common/ScrollBar/XTPScrollBase.h>
#include <Common/ScrollBar/XTPScrollBarCtrl.h>
#include <Common/ScrollBar/XTPScrollBarContainer.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define ASSERT_SCROLLBAR_ID(nID) \
	ASSERT((AFX_IDW_HSCROLL_FIRST <= (nID) && (nID) < (AFX_IDW_HSCROLL_FIRST + 0x10)) \
	|| (AFX_IDW_VSCROLL_FIRST <= (nID) && (nID) < (AFX_IDW_VSCROLL_FIRST + 0x10)) \
	|| AFX_IDW_SIZE_BOX == (nID))

// CXTPScrollBarContainerImpl::STDSCROLLBAR
//
CXTPScrollBarContainerImpl::STDSCROLLBAR::STDSCROLLBAR()
	: pCtrl(NULL)
	, bVisible(FALSE)
{
}

CXTPScrollBarContainerImpl::STDSCROLLBAR::~STDSCROLLBAR()
{
	ASSERT(NULL == pCtrl);
}

CXTPScrollBarCtrl* CXTPScrollBarContainerImpl::STDSCROLLBAR::operator -> () throw()
{
	ASSERT(NULL != pCtrl);
	return pCtrl;
}

const CXTPScrollBarCtrl* CXTPScrollBarContainerImpl::STDSCROLLBAR::operator -> () const throw()
{
	ASSERT(NULL != pCtrl);
	return pCtrl;
}

///////////////////////////////////////////////////////////////////////////////
// CXTPScrollBarContainerImpl
CXTPScrollBarContainerImpl::CXTPScrollBarContainerImpl()
	: m_pContainer(NULL)
	, m_nTheme(xtpScrollBarThemeWindowsDefault)
	, m_bCreated(FALSE)
{
}

CXTPScrollBarContainerImpl::~CXTPScrollBarContainerImpl()
{
	while(!m_ScrollBars.IsEmpty())
	{
		CXTPScrollBarCtrl* pScrollBar = m_ScrollBars.RemoveHead();
		if(::IsWindow(pScrollBar->GetSafeHwnd()))
		{
			pScrollBar->DestroyWindow();
		}
		delete pScrollBar;
	}

	DestroyStdScrollBar(SbGripper);
	DestroyStdScrollBar(SbHorz);
	DestroyStdScrollBar(SbVert);
}

inline CXTPScrollBarContainerImpl::StdScrollBarId
	CXTPScrollBarContainerImpl::SBtoSBId(int nBar)
{
	StdScrollBarId sbId = SbInvalid;
	switch(nBar)
	{
	case SB_VERT:
		sbId = SbVert;
		break;
	case SB_HORZ:
		sbId = SbHorz;
		break;
	}
	return sbId;
}

inline UINT CXTPScrollBarContainerImpl::SBIdToSBWs(StdScrollBarId nBar)
{
	ASSERT(SbInvalid != nBar);
	static const UINT sbWStyles[] = { WS_HSCROLL, WS_VSCROLL, 0 };
	return sbWStyles[nBar];
}

inline int CXTPScrollBarContainerImpl::SBIdToSB(StdScrollBarId nBar)
{
	ASSERT(SbInvalid != nBar && SbGripper != nBar);
	static const int sbBars[] = { SB_HORZ, SB_VERT };
	return sbBars[nBar];
}

void CXTPScrollBarContainerImpl::SetContainer(CWnd* pContainer)
{
	m_pContainer = pContainer;
}

CScrollBar* CXTPScrollBarContainerImpl::GetScrollBarCtrl(int nBar)
{
	ASSERT(SB_VERT == nBar || SB_HORZ == nBar);

	CScrollBar* pScrollBar = NULL;

	StdScrollBarId nBarId = SBtoSBId(nBar);
	if(HasStdBar(nBarId))
	{
		pScrollBar = m_StdScrollBar[nBar].pCtrl;
	}

	//// Make sure the scroll bar requested is created
	//// because sometimes this method gets invoked before OnCreate
	//// but when parent is already created.
	//CXTPScrollBarContainerImpl* pThis = const_cast<CXTPScrollBarContainerImpl*>(this);
	//CScrollBar* pScrollBar = pThis->CreateStdScrollBar(SBtoSBId(nBar));

	return pScrollBar;
}

void CXTPScrollBarContainerImpl::SetScrollBarTheme(XTPScrollBarTheme theme)
{
	m_nTheme = theme;

	// Apply theme to standard scroll bars.
	static const StdScrollBarId nBarIds[] = { SbVert, SbHorz, SbGripper };
	for(int i = 0; i < _countof(nBarIds); ++i)
	{
		if(HasStdBar(nBarIds[i]))
		{
			m_StdScrollBar[nBarIds[i]]->SetTheme(m_nTheme);
		}
	}

	// Apply theme to user scroll bars.
	POSITION nScrollBarPosition = m_ScrollBars.GetHeadPosition();
	while(NULL != nScrollBarPosition)
	{
		CXTPScrollBarCtrl* pScrollBar = m_ScrollBars.GetNext(nScrollBarPosition);
		if(::IsWindow(pScrollBar->GetSafeHwnd()))
		{
			pScrollBar->SetTheme(m_nTheme);
		}
	}
}

void CXTPScrollBarContainerImpl::OnWindowProc(
	UINT message,
	WPARAM wParam,
	LPARAM lParam,
	LRESULT& lResult)
{
	switch(message)
	{
	case WM_CREATE:
		if(-1 != lResult)
		{
			LPCREATESTRUCT lpCreateStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
			OnCreate(lpCreateStruct);
		}
		break;
	case WM_NCCALCSIZE:
		{
			LPNCCALCSIZE_PARAMS lpncsp = reinterpret_cast<LPNCCALCSIZE_PARAMS>(lParam);
			OnNcCalcSize((BOOL)wParam, lpncsp);
		}
		break;
	case WM_WINDOWPOSCHANGED:
	case WM_WINDOWPOSCHANGING:
		{
			WINDOWPOS* lpWinPos = reinterpret_cast<WINDOWPOS*>(lParam);
			OnWindowPosChange(lpWinPos);
		}
		break;
	case WM_STYLECHANGED:
		{
			LPSTYLESTRUCT pStyleStruct = reinterpret_cast<LPSTYLESTRUCT>(lParam);
			OnStyleChanged((long)wParam, pStyleStruct);
		}
		break;
	}
}

void CXTPScrollBarContainerImpl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	m_bCreated = TRUE;
	SyncStdScrollBarStates();

	UNREFERENCED_PARAMETER(lpCreateStruct);
}

void CXTPScrollBarContainerImpl::OnWindowPosChange(WINDOWPOS* lpWndPos)
{
	RepositionStdScrollBars();

	UNREFERENCED_PARAMETER(lpWndPos);
}

void CXTPScrollBarContainerImpl::OnNcCalcSize(
	BOOL bCalcValidRects,
	NCCALCSIZE_PARAMS *lpncsp)
{
	if(HasStdBar(SbVert))
	{
		int cxVScroll = ::GetSystemMetrics(SM_CXVSCROLL);
		lpncsp->rgrc->right += cxVScroll;
	}

	if(HasStdBar(SbHorz))
	{
		int cyHScroll = ::GetSystemMetrics(SM_CXHSCROLL);
		lpncsp->rgrc->bottom += cyHScroll;
	}

	UNREFERENCED_PARAMETER(bCalcValidRects);
}

void CXTPScrollBarContainerImpl::OnStyleChanged(
	LONG nType,
	LPSTYLESTRUCT pStyleStruct)
{
	ASSERT(NULL != pStyleStruct);

	if(GWL_STYLE == nType
		&& m_bCreated
		&& 0 != ((pStyleStruct->styleNew ^ pStyleStruct->styleOld)
			& (WS_HSCROLL | WS_VSCROLL)))
	{
		SyncStdScrollBarStates();
	}
}

BOOL CXTPScrollBarContainerImpl::HasStdBar(StdScrollBarId nBar) const
{
	ASSERT(SbInvalid != nBar);

	BOOL bHas = (NULL != m_StdScrollBar[nBar].pCtrl
		&& ::IsWindow(m_StdScrollBar[nBar]->m_hWnd));
	return bHas;
}

CScrollBar* CXTPScrollBarContainerImpl::CreateStdScrollBar(StdScrollBarId nBar)
{
	ASSERT(NULL != m_pContainer);
	ASSERT(m_bCreated);

	CScrollBar* pScrollBar = NULL;

	if(SbInvalid != nBar && NULL == m_StdScrollBar[nBar].pCtrl)
	{
		m_StdScrollBar[nBar].pCtrl = new CXTPScrollBarCtrl();
		if(NULL != m_StdScrollBar[nBar].pCtrl)
		{
			STDSCROLLBAR& stdBarInfo = m_StdScrollBar[nBar];

			static const UINT sbStyles[] = { SBS_HORZ, SBS_VERT, SBS_SIZEGRIP };
			BOOL bCreated = stdBarInfo->Create(
				WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | sbStyles[nBar],
				CRect(0, 0, 0, 0), m_pContainer, 0);
			if(bCreated)
			{
				stdBarInfo->SetTheme(m_nTheme);
				stdBarInfo.bVisible = TRUE;
			}

			pScrollBar = stdBarInfo.pCtrl;
		}
	}

	return pScrollBar;
}

void CXTPScrollBarContainerImpl::DestroyStdScrollBar(StdScrollBarId nBar)
{
	if(SbInvalid != nBar)
	{
		if(NULL != m_StdScrollBar[nBar].pCtrl)
		{
			if(HasStdBar(nBar))
			{
				m_StdScrollBar[nBar]->DestroyWindow();
			}

			delete m_StdScrollBar[nBar].pCtrl;
			m_StdScrollBar[nBar].pCtrl = NULL;
		}

		m_StdScrollBar[nBar].bVisible = FALSE;
	}
}

void CXTPScrollBarContainerImpl::RepositionStdScrollBars(const RECT* lpRect /*= NULL*/)
{
	ASSERT(NULL != m_pContainer);

	CRect rc;
	if(NULL != lpRect)
	{
		rc = *lpRect;
	}
	else
	{
		m_pContainer->GetClientRect(&rc);
	}

	int cyHScroll = ::GetSystemMetrics(SM_CYHSCROLL);
	int cxVScroll = ::GetSystemMetrics(SM_CXVSCROLL);

	int cx = rc.Width();
	int cy = rc.Height();

	BOOL bHScroll = HasStdBar(SbHorz);
	BOOL bVScroll = HasStdBar(SbVert);

	if(bHScroll)
	{
		m_StdScrollBar[SbHorz]->SetWindowPos(
			&CWnd::wndTop,
			rc.left, cy - cyHScroll,
			(bVScroll && m_StdScrollBar[SbVert].bVisible ? cx - cxVScroll : cx), cyHScroll,
			m_StdScrollBar[SbHorz].bVisible ? SWP_SHOWWINDOW : SWP_HIDEWINDOW);
	}

	if(bVScroll)
	{
		m_StdScrollBar[SbVert]->SetWindowPos(
			&CWnd::wndTop,
			cx - cxVScroll, 0,
			cxVScroll, cy - (bHScroll && m_StdScrollBar[SbHorz].bVisible ? cyHScroll : 0),
			m_StdScrollBar[SbVert].bVisible ? SWP_SHOWWINDOW : SWP_HIDEWINDOW);
	}

	if(HasStdBar(SbGripper))
	{
		m_StdScrollBar[SbGripper].bVisible = bVScroll && bHScroll
			&& m_StdScrollBar[SbHorz].bVisible
			&& m_StdScrollBar[SbVert].bVisible;

		m_StdScrollBar[SbGripper]->SetWindowPos(
			&CWnd::wndTop,
			cx - cxVScroll, cy - cyHScroll,
			cxVScroll, cyHScroll,
			m_StdScrollBar[SbGripper].bVisible ? SWP_SHOWWINDOW : SWP_HIDEWINDOW);
	}
}

BOOL CXTPScrollBarContainerImpl::CreateScrollBarCtrl(DWORD dwStyle, UINT nID)
{
	ASSERT_SCROLLBAR_ID(nID);
	ASSERT(NULL != m_pContainer);
	ASSERT(m_bCreated);

	BOOL result = FALSE;

	DestroyScrollBarCtrl(nID);

	if(::IsWindow(m_pContainer->GetSafeHwnd()))
	{
		CXTPScrollBarCtrl* pScrollBar = new CXTPScrollBarCtrl();
		if(NULL != pScrollBar)
		{
			if(pScrollBar->Create(
				WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | dwStyle,
				CRect(0, 0, 0, 0), m_pContainer, nID))
			{
				m_ScrollBars.AddTail(pScrollBar);
				pScrollBar->SetTheme(m_nTheme);
				result = TRUE;
			}
			else
			{
				delete pScrollBar;
			}
		}
	}

	return result;
}

void CXTPScrollBarContainerImpl::DestroyScrollBarCtrl(UINT nID)
{
	ASSERT_SCROLLBAR_ID(nID);
	ASSERT(NULL != m_pContainer);

	CWnd* pItem = m_pContainer->GetDlgItem(nID);
	if(NULL != pItem)
	{
		ASSERT_KINDOF(CXTPScrollBarCtrl, pItem);
		CXTPScrollBarCtrl* pScrollBar = static_cast<CXTPScrollBarCtrl*>(pItem);
		POSITION pos = m_ScrollBars.Find(pScrollBar);
		if(NULL != pos)
		{
			m_ScrollBars.RemoveAt(pos);
		}

		if(::IsWindow(pItem->GetSafeHwnd()))
		{
			pItem->DestroyWindow();
		}

		delete pScrollBar;
	}
}

void CXTPScrollBarContainerImpl::ShowScrollBar(UINT nBar, BOOL bShow)
{
	ASSERT(SB_HORZ == nBar || SB_VERT == nBar);
	ASSERT(NULL != m_pContainer);

	if(m_bCreated)
	{
		if(SB_HORZ == nBar || SB_BOTH == nBar)
		{
			ShowScrollBarInternal(SbHorz, bShow);
		}

		if(SB_VERT == nBar || SB_BOTH == nBar)
		{
			ShowScrollBarInternal(SbVert, bShow);
		}

		BOOL bShowGripper = HasStdBar(SbVert) && HasStdBar(SbHorz);
		ShowScrollBarInternal(SbGripper, bShowGripper);

		m_pContainer->SetWindowPos(&CWnd::wndTop, 0, 0, 0, 0,
			SWP_NOMOVE | SWP_NOZORDER | SWP_NOSIZE | SWP_FRAMECHANGED);

		RepositionStdScrollBars();
	}
}

void CXTPScrollBarContainerImpl::ShowScrollBarInternal(StdScrollBarId nBar, BOOL bShow)
{
	ASSERT(NULL != m_pContainer);

	if(SbInvalid != nBar)
	{
		if(SbGripper != nBar)
		{
			m_pContainer->ShowScrollBar(SBIdToSB(nBar), bShow);
		}

		if(bShow)
		{
			CreateStdScrollBar(nBar);
		}
		else
		{
			DestroyStdScrollBar(nBar);
		}
	}
}

BOOL CXTPScrollBarContainerImpl::EnableScrollBar(
	int nSBFlags,
	UINT nArrowFlags /*= ESB_ENABLE_BOTH*/)
{
	BOOL bEnable = (ESB_ENABLE_BOTH == nArrowFlags);

	if((SB_VERT == nSBFlags || SB_BOTH == nSBFlags) && HasStdBar(SbVert))
	{
		m_StdScrollBar[SbVert]->EnableWindow(bEnable);
	}

	if((SB_HORZ == nSBFlags || SB_BOTH == nSBFlags) && HasStdBar(SbHorz))
	{
		m_StdScrollBar[SbHorz]->EnableWindow(bEnable);
	}

	return TRUE;
}

void CXTPScrollBarContainerImpl::SyncStdScrollBarStates()
{
	ASSERT(NULL != m_pContainer);

	UINT sbStyle = m_pContainer->GetStyle() & (WS_HSCROLL | WS_VSCROLL);
	ShowScrollBar(SB_HORZ, 0 != (sbStyle & WS_HSCROLL));
	ShowScrollBar(SB_VERT, 0 != (sbStyle & WS_VSCROLL));
}

void CXTPScrollBarContainerImpl::RefreshMetrics()
{
	static const StdScrollBarId nBarIds[] = { SbHorz, SbVert, SbGripper };
	for(int i = 0; i < _countof(nBarIds); ++i)
	{
		if(HasStdBar(nBarIds[i]))
		{
			m_StdScrollBar[nBarIds[i]]->RefreshMetrics();
		}
	}

	POSITION nScrollBarPosition = m_ScrollBars.GetHeadPosition();
	while(NULL != nScrollBarPosition)
	{
		CXTPScrollBarCtrl* pScrollBar = m_ScrollBars.GetNext(nScrollBarPosition);
		if(::IsWindow(pScrollBar->GetSafeHwnd()))
		{
			pScrollBar->RefreshMetrics();
		}
	}
}
