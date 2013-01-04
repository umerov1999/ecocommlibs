// XTPToolTipContext.cpp: implementation of the CXTPToolTipContext class.
//
// This file is a part of the XTREME TOOLKIT PRO MFC class library.
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

#if _MSC_VER < 1200
#define _XTP_EXCLUDE_HTML
#else
#include <mshtml.h>
#include <mshtmhst.h>
#endif // !_MSC_VER < 1200

#include "XTPToolTipContext.h"
#include "XTPVC80Helpers.h"
#include "XTPDrawHelpers.h"
#include "XTPColorManager.h"
#include "XTPImageManager.h"
#include "XTPResourceImage.h"
#include "XTPSystemHelpers.h"
#include "XTPRichRender.h"
#include "XTPResourceManager.h"
#include "XTPMarkupRender.h"

#ifdef _INC_COMDEF
// Warning "ComDef.h" was included
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

#pragma warning (disable : 4097) // typedef-name 'CXTPToolTipContextTipOfficeCtrl' used as synonym for class-name 'CRichEditToolTip'

#define WM_SYSKEYFIRST WM_SYSKEYDOWN
#define WM_SYSKEYLAST WM_SYSDEADCHAR

#define VERSION_IE5             MAKELONG(80, 5)

#define TTS_NOANIMATE           0x10
#define TTS_NOFADE              0x20
#define TTS_BALLOON             0x40


#define CS_DROPSHADOW       0x00020000


#ifndef TTM_SETTITLE

#define TTM_SETTITLEA           (WM_USER + 32)  // wParam = TTI_*, lParam = char* szTitle
#define TTM_SETTITLEW           (WM_USER + 33)  // wParam = TTI_*, lParam = wchar* szTitle

#ifdef UNICODE
#define TTM_SETTITLE            TTM_SETTITLEW
#else
#define TTM_SETTITLE            TTM_SETTITLEA
#endif

#endif

#ifndef WF_TRACKINGTOOLTIPS
#define WF_TRACKINGTOOLTIPS 0x0400
#endif

#define SAFE_DELETEWINDOW(ptr) \
	if (ptr) { ptr->DestroyWindow(); delete ptr; ptr = NULL; }

CXTPToolTipContext* CXTPToolTipContext::m_pModuleContext = 0;


IMPLEMENT_DYNAMIC(CXTPToolTipContextToolTip, CWnd)


void XTPStripEllipsis(LPTSTR lpszText)
{
	if (lpszText == NULL || lpszText == LPSTR_TEXTCALLBACK)
		return;

	int len = (int)_tcslen(lpszText);

	if (len > 3)
	{
		for (int i = len - 3; i < len; i++)
		{
			if (lpszText[i] != _T('.'))
				return;
		}

		lpszText[len - 3] = 0;
	}
};

//////////////////////////////////////////////////////////////////////////
// CXTPToolTipContextToolTip

CXTPToolTipContextToolTip::CXTPToolTipContextToolTip(CXTPToolTipContext* pContext)
	: m_pContext(pContext)
{
	m_bActive = FALSE;
	m_toolVisible.Reset();
	m_toolDisabled.Reset();
	m_toolDelay.Reset();

	m_pIcon = NULL;

	m_bWindowRegion = FALSE;

	m_nDelayTimer = 0;
	m_nAutoPopTimer = 0;

	m_dwLastTip = 0;
	m_nDelayInitial = 500;
	m_nDelayReshow = 200;
	m_nDelayAutoPop = 0;
}

void CXTPToolTipContext::SetFont(CFont* pFont)
{
	LOGFONT lf;
	pFont->GetLogFont(&lf);

	SetFontIndirect(&lf);
}

void CXTPToolTipContext::SetFontIndirect(LOGFONT* lpLogFont)
{
	if (!lpLogFont)
		return;

	lpLogFont->lfWeight = FW_NORMAL;
	m_fnt.DeleteObject();
	m_fnt.CreateFontIndirect(lpLogFont);

	lpLogFont->lfWeight = FW_BOLD;
	m_fntTitle.DeleteObject();
	m_fntTitle.CreateFontIndirect(lpLogFont);

	if (m_pToolTip) m_pToolTip->DestroyWindow();
}


CXTPToolTipContextToolTip::~CXTPToolTipContextToolTip()
{
	for (int i = 0; i < (int)m_arrTools.GetSize(); i++)
	{
		delete m_arrTools[i];
	}
	m_arrTools.RemoveAll();
	HookMouseMove(FALSE);

}


BEGIN_MESSAGE_MAP(CXTPToolTipContextToolTip, CWnd)
	//{{AFX_MSG_MAP(CXTPToolTipContextToolTip)
	ON_MESSAGE(TTM_ADDTOOL, OnAddTool)
	ON_MESSAGE(TTM_ACTIVATE, OnActivate)
	ON_MESSAGE(TTM_DELTOOL, OnDelTool)
	ON_MESSAGE(TTM_RELAYEVENT, OnRelayEvent)
	ON_MESSAGE(TTM_SETTITLE, OnSetTitle)
	ON_MESSAGE(TTM_UPDATETIPTEXT, OnUpdateTipText)
	ON_MESSAGE(TTM_SETDELAYTIME, OnSetDelayTime)
	ON_MESSAGE(XTP_TTM_SETIMAGE, OnSetImage)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_NCHITTEST_EX()
	ON_WM_MOUSEACTIVATE()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_MESSAGE(TTM_WINDOWFROMPOINT, OnWindowFromPoint)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTPToolTipContextToolTip message handlers
BOOL CXTPToolTipContextToolTip::Create(CWnd* pParentWnd, DWORD dwStyle)
{
	UINT nClassStyle = (dwStyle & XTP_TTS_NOSHADOW ? 0 : CS_DROPSHADOW) | CS_SAVEBITS | CS_OWNDC;

	if ((nClassStyle & CS_DROPSHADOW) && !XTPSystemVersion()->IsWinXPOrGreater()) // Windows XP only
	{
		nClassStyle &= ~CS_DROPSHADOW;
	}


	BOOL bResult = CWnd::CreateEx(WS_EX_TOOLWINDOW, AfxRegisterWndClass(nClassStyle, AfxGetApp()->LoadStandardCursor(IDC_ARROW)), NULL,
		WS_POPUP | dwStyle, // force WS_POPUP
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		pParentWnd->GetSafeHwnd(), NULL, NULL);

	if (bResult)
		SetOwner(pParentWnd);
	return bResult;
}

void CXTPToolTipContextToolTip::OnDestroy()
{
	m_toolVisible.Reset();
	m_toolDisabled.Reset();
	m_toolDelay.Reset();

	for (int i = 0; i < (int)m_arrTools.GetSize(); i++)
	{
		delete m_arrTools[i];
	}
	m_arrTools.RemoveAll();

	m_strTitle.Empty();
	m_pIcon = NULL;

	Activate(FALSE);
	HookMouseMove(FALSE);

	CWnd::OnDestroy();
}

LRESULT CXTPToolTipContextToolTip::OnSetImage(WPARAM, LPARAM lParam)
{
	m_pIcon = (CXTPImageManagerIcon*)lParam;
	return 0;
}

LRESULT CXTPToolTipContextToolTip::OnSetTitle(WPARAM, LPARAM lParam)
{
	m_strTitle = (LPCTSTR)lParam;
	return 0;
}

LRESULT CXTPToolTipContextToolTip::OnSetDelayTime(WPARAM dwDuration, LPARAM lParam)
{
	UINT iTime = LOWORD(lParam);

	switch (dwDuration)
	{
	case TTDT_RESHOW:
		m_nDelayReshow = iTime;
		break;

	case TTDT_INITIAL:
		m_nDelayInitial = iTime;
		break;

	case TTDT_AUTOPOP:
		m_nDelayAutoPop = iTime;
		break;
	}
	return 0;
}

LRESULT CXTPToolTipContextToolTip::OnUpdateTipText(WPARAM, LPARAM lParam)
{
	LPTOOLINFO lpToolInfo = LPTOOLINFO(lParam);

	for (int i = 0; i < (int)m_arrTools.GetSize(); i++)
	{
		TOOLITEM* pItem = m_arrTools[i];

		if ((pItem->uId == lpToolInfo->uId) && (pItem->hwnd == lpToolInfo->hwnd))
		{
			pItem->bAutoCaption = (lpToolInfo->lpszText == LPSTR_TEXTCALLBACK);
			pItem->strCaption = pItem->bAutoCaption ? _T("") : lpToolInfo->lpszText;
			break;
		}
	}

	return 0;
}


LRESULT CXTPToolTipContextToolTip::OnAddTool(WPARAM, LPARAM lParam)
{
	return AddTool((LPTOOLINFO)lParam);
}

LRESULT CXTPToolTipContextToolTip::OnActivate(WPARAM wParam, LPARAM)
{
	Activate((BOOL)wParam);
	return 0;
}

LRESULT CXTPToolTipContextToolTip::OnDelTool(WPARAM, LPARAM lParam)
{
	DelTool((LPTOOLINFO)lParam);
	return 0;
}

LRESULT CXTPToolTipContextToolTip::OnRelayEvent(WPARAM, LPARAM lParam)
{
	RelayEvent((LPMSG)lParam);
	return 0;
}

LRESULT CXTPToolTipContextToolTip::OnWindowFromPoint(WPARAM, LPARAM lParam)
{
	LPPOINT lpPoint = (LPPOINT)lParam;
	HWND hWnd = lpPoint ? ::WindowFromPoint(*lpPoint) : 0;

	if ((hWnd != NULL) && (GetWindowLong(hWnd, GWL_STYLE) & WS_CHILD) && (::SendMessage(hWnd, XTP_TTM_WINDOWFROMPOINT, 0, 0) == 1))
	{
		return (LRESULT)::GetParent(hWnd);
	}

	return (LRESULT)hWnd;
}

BOOL CXTPToolTipContextToolTip::AddTool(LPTOOLINFO lpToolInfo)
{
	ASSERT(lpToolInfo);

	if (!lpToolInfo)
		return FALSE;

	TOOLITEM* pti = new TOOLITEM;
	pti->hwnd = lpToolInfo->hwnd;
	pti->uFlags = lpToolInfo->uFlags;
	pti->uId = lpToolInfo->uId;
	pti->rect = lpToolInfo->rect;
	pti->bAutoCaption = (lpToolInfo->lpszText == LPSTR_TEXTCALLBACK);
	pti->strCaption = pti->bAutoCaption ? _T("") : lpToolInfo->lpszText;
	pti->hinst = lpToolInfo->hinst;

	pti->rectExclude.SetRectEmpty();

	if (lpToolInfo->cbSize == sizeof(XTP_TOOLTIP_TOOLINFO_EX))
	{
		pti->rectExclude = ((XTP_TOOLTIP_TOOLINFO_EX*)lpToolInfo)->pToolInfo->rcExclude;
	}


	m_arrTools.Add(pti);

	return TRUE;
}

void CXTPToolTipContextToolTip::Activate(BOOL bActivate)
{
	m_bActive = bActivate;

	if (!bActivate && GetSafeHwnd() && m_nDelayTimer)
	{
		KillTimer(m_nDelayTimer);
		m_nDelayTimer = 0;
		m_toolDelay.Reset();
	}
	if (!bActivate && GetSafeHwnd() && IsWindowVisible())
	{
		SetVisibleTool(NULL);
	}
}

void CXTPToolTipContextToolTip::DelTool(LPTOOLINFO lpToolInfo)
{
	ASSERT(lpToolInfo);
	if (!lpToolInfo)
		return;

	for (int i = 0; i < (int)m_arrTools.GetSize(); i++)
	{
		TOOLITEM* pItem = m_arrTools[i];

		if ((pItem->uId == lpToolInfo->uId) && (pItem->hwnd == lpToolInfo->hwnd))
		{

			if (m_toolVisible.IsEqual(pItem))
				SetVisibleTool(0);

			if (m_toolDisabled.IsEqual(pItem)) m_toolDisabled.Reset();

			if (m_toolDelay.IsEqual(pItem)) m_toolDelay.Reset();

			m_arrTools.RemoveAt(i);
			delete pItem;

			break;
		}
	}

}

CXTPToolTipContextToolTip::TOOLITEM* CXTPToolTipContextToolTip::FindTool()
{
	if (!(GetKeyState(VK_LBUTTON) >= 0 && GetKeyState(VK_RBUTTON) >= 0 &&
		GetKeyState(VK_MBUTTON) >= 0))
	{
		return NULL;
	}

	CPoint pt;
	GetCursorPos(&pt);

	for (int i = 0; i < m_arrTools.GetSize(); i++)
	{
		TOOLITEM* pItem = m_arrTools[i];

		if (!::IsWindow(pItem->hwnd))
			continue;

		CWnd* pWnd = CWnd::FromHandle(pItem->hwnd);

		CRect rcTool = pItem->rect;

		if (pItem->uFlags & TTF_IDISHWND)
		{
			pWnd->GetWindowRect(rcTool);
		}
		else
		{
			pWnd->ClientToScreen(rcTool);
		}

		if (rcTool.PtInRect(pt) && ((pItem->uFlags & TTF_IDISHWND) || ((HWND)OnWindowFromPoint(0, (LRESULT)(LPPOINT)&pt) == pItem->hwnd)))
			return pItem;
	}
	return NULL;
}

#ifndef IS_INTRESOURCE
#define IS_INTRESOURCE(x) (((size_t)(x) >> 16) == 0)
#endif

CString CXTPToolTipContextToolTip::GetToolText(TOOLITEM* lpToolInfo)
{
	if (lpToolInfo->bAutoCaption)
	{
		UINT_PTR id = lpToolInfo->uId;
		TOOLTIPTEXT tt = {0};
		tt.hdr.hwndFrom = m_hWnd;
		tt.hdr.idFrom = lpToolInfo->uId;
		tt.hdr.code = TTN_NEEDTEXT;

		tt.uFlags = lpToolInfo->uFlags;
		tt.hinst = NULL;

		tt.szText[0] = _T('\0');
		tt.lpszText = tt.szText;

		::SendMessage(lpToolInfo->hwnd, WM_NOTIFY, id, (LPARAM)&tt);

		if (IS_INTRESOURCE(tt.lpszText))
		{
			XTPResourceManager()->LoadLocaleString(tt.hinst, (UINT)(UINT_PTR)tt.lpszText, lpToolInfo->strCaption);
		}
		else
		{
			lpToolInfo->strCaption = tt.lpszText;
		}

		if ((m_pContext->GetControlStyle() & TTS_NOPREFIX) == 0)
		{
			CXTPDrawHelpers::StripMnemonics(lpToolInfo->strCaption);
		}

		lpToolInfo->bAutoCaption = FALSE;
	}


	return lpToolInfo->strCaption;
}

int CXTPToolTipContextToolTip::GetMaxTipWidth() const
{
	const int nWidth = m_pContext->GetMaxTipWidth();

	if (nWidth == -1)
	{
		return ::GetSystemMetrics(SM_CXSCREEN);
	}
	return nWidth;
}

CSize CXTPToolTipContextToolTip::GetToolSize(TOOLITEM* lpToolInfo)
{
	CClientDC dc(this);

	CFont* pOldFont = dc.SelectObject(&m_pContext->m_fnt);

	CString str = GetToolText(lpToolInfo);

	if (str.IsEmpty())
	{
		dc.SelectObject(pOldFont);
		return CSize(0);
	}

	int nMaxTipWidth = GetMaxTipWidth();

	CRect rcMargin = m_pContext->GetMargin();
	CSize szMargin(3 + rcMargin.left + rcMargin.right + 3, 3 + rcMargin.top + rcMargin.bottom + 3);

	DWORD dwFlags = DT_NOPREFIX | DT_EXPANDTABS;

	BOOL bDrawImage = m_pIcon != NULL;
	BOOL bDrawTitle = !m_strTitle.IsEmpty();
	BOOL bDrawImageTop = TRUE;
	CSize szImage(0, 0);
	CSize szTitle(0, 0);

	if (bDrawTitle)
	{
		CXTPFontDC fntTitle(&dc, &m_pContext->m_fntTitle);

		CRect rcTitle(0, 0, 0, 0);
		dc.DrawText(m_strTitle, rcTitle, dwFlags | DT_CALCRECT | DT_SINGLELINE);

		szTitle = CSize(rcTitle.Width() + 4, rcTitle.Height() + 10 + 10);
	}

	if (bDrawImage)
	{
		CSize szIcon(m_pIcon->GetWidth(), m_pIcon->GetHeight());
		bDrawImageTop = (szIcon.cy <= 16);

		if (bDrawImageTop)
		{
			if (!bDrawTitle)
			{
				szImage.cx = szIcon.cx + 3;
			}
			else
			{
				szTitle.cx += szIcon.cx + 1;
			}
		}
		else
		{
			szImage.cx = szIcon.cx + 5;
		}
		szImage.cy = szIcon.cy;
	}

	CRect rcText(0, 0, nMaxTipWidth - szMargin.cx, 0);

	if (bDrawTitle)
		rcText.right = max(szTitle.cx, nMaxTipWidth - szMargin.cx) - (10 + 15);

	dc.DrawText(str, rcText, dwFlags | DT_CALCRECT | DT_WORDBREAK);
	dc.SelectObject(pOldFont);

	CSize szText = rcText.Size();

	CSize sz(0, 0);
	sz.cy = max(szImage.cy, szText.cy);
	sz.cx = szImage.cx + szText.cx;

	if (bDrawTitle)
	{
		sz.cx = max(sz.cx + 10 + 15, szTitle.cx);
		sz.cy += szTitle.cy;
	}

	sz += szMargin;

	return sz;
}

HHOOK CXTPToolTipContextToolTip::m_hHookMouse = 0;
CXTPToolTipContextToolTip* CXTPToolTipContextToolTip::m_pWndMonitor = 0;

LRESULT CALLBACK CXTPToolTipContextToolTip::MouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode != HC_ACTION || !m_pWndMonitor || m_pWndMonitor->m_toolVisible.IsEmpty())
		return CallNextHookEx(m_hHookMouse, nCode, wParam, lParam);

	POINT pt = ((PMOUSEHOOKSTRUCT)lParam)->pt;
	HWND hWnd = (HWND)m_pWndMonitor->OnWindowFromPoint(0, (LPARAM)&pt);

	if (hWnd != m_pWndMonitor->m_toolVisible.hwnd)
	{
		MSG msg;
		msg.pt = pt;
		msg.message = WM_MOUSEMOVE;
		m_pWndMonitor->RelayEvent(&msg);
	}

	return CallNextHookEx(m_hHookMouse, nCode, wParam, lParam);
}

void CXTPToolTipContextToolTip::HookMouseMove(BOOL bSetupHook)
{
	if (bSetupHook && m_hHookMouse == 0 && (m_pWndMonitor == NULL))
	{
		m_hHookMouse = SetWindowsHookEx(WH_MOUSE, MouseProc, 0, GetCurrentThreadId ());
		m_pWndMonitor = this;
	}
	else if (!bSetupHook && m_hHookMouse && (m_pWndMonitor == this))
	{
		UnhookWindowsHookEx(m_hHookMouse);
		m_hHookMouse = 0;
		m_pWndMonitor = NULL;
	}
}

void CXTPToolTipContextToolTip::EnsureVisible(CRect& rcToolTip)
{
	CRect rcWork = XTPMultiMonitor()->GetWorkArea();
	int nGap = 4;

	if (rcWork.right - nGap < rcToolTip.right)
	{
		rcToolTip.OffsetRect(- rcToolTip.right + rcWork.right - nGap, 0);
	}

	if (rcWork.left > rcToolTip.left)
	{
		rcToolTip.OffsetRect(rcWork.left - rcToolTip.left, 0);
	}

	if (rcWork.bottom - nGap < rcToolTip.bottom)
	{
		CPoint pt;
		GetCursorPos(&pt);
		rcToolTip.OffsetRect(0, pt.y - rcToolTip.bottom - 3);
	}
}

int CXTPToolTipContext::GetCursorHeight()
{
	HCURSOR hCursor = GetCursor();

	ICONINFO iconInfo;
	if (!GetIconInfo(hCursor, &iconInfo))
		return 21;

	BITMAP bm;
	if (!GetObject(iconInfo.hbmMask, sizeof(BITMAP), &bm))
		return 21;

	WORD curBits[16 * 8];
	memset(&curBits, -1, sizeof(curBits));

	if (!GetBitmapBits(iconInfo.hbmMask, sizeof(curBits), curBits))
		return 21;

	int nBitSize = sizeof(WORD) * 8;
	int nCount = MulDiv(bm.bmWidth, bm.bmHeight, nBitSize);
	int nXor = 0;

	if (!iconInfo.hbmColor)
	{
		nXor = nCount - 1;
		nCount = nCount / 2;
	}

	if (nCount >= sizeof(curBits)/sizeof(WORD)) nCount = sizeof(curBits)/sizeof(WORD) - 1;
	if (nXor >= sizeof(curBits)/sizeof(WORD)) nXor = 0;

	int i = nCount - 1;

	for (; i >= 0; i--)
	{
		if (curBits[i] != 0xFFFF || (nXor && (curBits[nXor--] != 0)))
			break;
	}

	if (iconInfo.hbmColor) DeleteObject(iconInfo.hbmColor);
	if (iconInfo.hbmMask) DeleteObject(iconInfo.hbmMask);

	return MulDiv(i + 1, nBitSize, (int)bm.bmWidth) - (int)iconInfo.yHotspot;
}

void CXTPToolTipContextToolTip::SetRoundRectRegion(CWnd* pWnd)
{
	CXTPWindowRect rc(pWnd);
	rc.OffsetRect(-rc.TopLeft());

	int cx = rc.Width(), cy = rc.Height();

	RECT rgn[] =
	{
		{1, 0, cx - 1, 1}, {0, 1, cx, cy - 1}, {1, cy - 1, cx - 1, cy}
	};

	int nSizeData = sizeof(RGNDATAHEADER) + sizeof(rgn);

	RGNDATA* pRgnData = (RGNDATA*)malloc(nSizeData);
	if (!pRgnData)
		return;
	MEMCPY_S(&pRgnData->Buffer, (void*)&rgn, sizeof(rgn));

	pRgnData->rdh.dwSize = sizeof(RGNDATAHEADER);
	pRgnData->rdh.iType = RDH_RECTANGLES;
	pRgnData->rdh.nCount = sizeof(rgn) / sizeof(RECT);
	pRgnData->rdh.nRgnSize = 0;
	pRgnData->rdh.rcBound = CRect(0, 0, cx, cy);

	CRgn rgnResult;
	VERIFY(rgnResult.CreateFromData(NULL, nSizeData, pRgnData));

	free(pRgnData);

	pWnd->SetWindowRgn((HRGN)rgnResult.Detach(), FALSE);
	m_bWindowRegion = TRUE;
}

void CXTPToolTipContextToolTip::SetVisibleTool(TOOLITEM* pVisibleTool)
{
	if (!m_toolVisible.IsEqual(pVisibleTool))
	{
		CSize sz(0);

		if (pVisibleTool)
		{
			sz = GetToolSize(pVisibleTool);
			if (sz == CSize(0)) pVisibleTool = 0;
		}

		if (m_hWnd)
		{
			SetWindowPos(0, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_HIDEWINDOW | SWP_NOACTIVATE | SWP_NOOWNERZORDER);
			if (m_bWindowRegion)
			{
				SetWindowRgn(NULL, FALSE);
				m_bWindowRegion = FALSE;
			}
		}

		if (pVisibleTool)
		{
			CRect rcToolTip(0, 0, 0, 0);

			if (!pVisibleTool->rectExclude.IsRectEmpty())
			{
				CWnd* pWnd = CWnd::FromHandle(pVisibleTool->hwnd);
				//CRect rcExclude = pVisibleTool->rectExclude;
				//rcToolTip = pVisibleTool->rect;
				rcToolTip.SetRect(pVisibleTool->rect.left, pVisibleTool->rectExclude.bottom,
					pVisibleTool->rect.left + sz.cx, pVisibleTool->rectExclude.bottom + sz.cy);

				pWnd->ClientToScreen(rcToolTip);

			}
			else
			{
				CPoint pt;
				GetCursorPos(&pt);
				int nCursorHeight = CXTPToolTipContext::GetCursorHeight();

				rcToolTip.SetRect(pt.x, pt.y + nCursorHeight, pt.x + sz.cx, pt.y + nCursorHeight + sz.cy);

				if (GetWindowLong(pVisibleTool->hwnd, GWL_EXSTYLE) & WS_EX_LAYOUTRTL)
				{
					rcToolTip.OffsetRect(-sz.cx, 0);
				}
			}


			EnsureVisible(rcToolTip);

			SetWindowPos(&CWnd::wndTop, rcToolTip.left, rcToolTip.top,
				rcToolTip.Width(), rcToolTip.Height(), SWP_NOACTIVATE | SWP_NOOWNERZORDER);

			UINT_PTR id = pVisibleTool->uId;
			NMHDR hdr = {0};
			hdr.hwndFrom = m_hWnd;
			hdr.idFrom = pVisibleTool->uId;
			hdr.code = TTN_SHOW;
			::SendMessage(pVisibleTool->hwnd, WM_NOTIFY, id, (LPARAM)&hdr);

			if (GetStyle() & XTP_TTS_OFFICE2007FRAME)
			{
				SetRoundRectRegion(this);
			}

			SetWindowPos(0, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER);

			Invalidate(FALSE);

			HookMouseMove(TRUE);

			m_toolDisabled.Reset();

			if (m_nDelayAutoPop > 0)
			{
				if (m_nAutoPopTimer > 0)
				{
					KillTimer(m_nAutoPopTimer);
					m_nAutoPopTimer = 0;
				}

				m_nAutoPopTimer = SetTimer(3, m_nDelayAutoPop, NULL);
			}
		}
		else
		{
			HookMouseMove(FALSE);
			m_toolDelay.Reset();

			if (m_nAutoPopTimer > 0)
			{
				if (m_hWnd) KillTimer(m_nAutoPopTimer);
				m_nAutoPopTimer = 0;
			}
		}

		m_toolVisible.Assign(pVisibleTool);

		m_dwLastTip = GetTickCount();

		OnVisibleChanged(pVisibleTool != NULL);
	}
}

void CXTPToolTipContextToolTip::RelayEvent(LPMSG lpMsg)
{
	if (!m_bActive)
		return;

	if (!((lpMsg->message >= WM_MOUSEFIRST && lpMsg->message <= WM_MOUSELAST) ||
		(lpMsg->message >= WM_NCMOUSEMOVE && lpMsg->message <= WM_NCMBUTTONDBLCLK)))
		return;

	TOOLITEM* pVisibleTool = FindTool();

	if (lpMsg->message == WM_LBUTTONDOWN)
	{
		if (pVisibleTool)
		{
			m_toolDisabled = *pVisibleTool;
			pVisibleTool = 0;
		}
	}
	if (pVisibleTool && (m_toolDisabled.IsEqual(pVisibleTool)))
	{
		pVisibleTool = 0;
	}

	if (m_nDelayTimer && !m_toolDelay.IsEqual(pVisibleTool))
	{
		KillTimer(m_nDelayTimer);
		m_nDelayTimer = 0;
		m_toolDelay.Reset();
	}

	if (pVisibleTool)
	{
		if (!m_toolVisible.IsEqual(pVisibleTool))
		{
			if (GetTickCount() - m_dwLastTip < m_nDelayReshow)
			{
				SetVisibleTool(pVisibleTool);
			}
			else if (!m_toolDelay.IsEqual(pVisibleTool))
			{
				m_toolDelay = *pVisibleTool;
				m_nDelayTimer = SetTimer(1, m_nDelayInitial, NULL);
			}
		}
	}
	else
	{
		SetVisibleTool(0);
	}


}

void CXTPToolTipContextToolTip::DrawEntry(CDC* pDC, TOOLITEM* lpToolInfo, CRect rc)
{
	CString str = lpToolInfo->strCaption;

	rc.DeflateRect(m_pContext->GetMargin());
	rc.DeflateRect(3, 3, 3, 3);

	DWORD dwFlags = DT_NOPREFIX | DT_EXPANDTABS;
	BOOL bLayoutRTL = lpToolInfo->hwnd && GetWindowLong(lpToolInfo->hwnd, GWL_EXSTYLE) & WS_EX_LAYOUTRTL;

	if (bLayoutRTL)
	{
		dwFlags |= DT_RTLREADING | DT_RIGHT;
	}

	CRect rcTitle(rc.left + 2, rc.top + 2, rc.right - 2, rc.bottom);

	BOOL bDrawImage = m_pIcon != NULL;
	BOOL bDrawTitle = !m_strTitle.IsEmpty();
	BOOL bDrawImageTop = TRUE;
	CSize szImage(0, 0);

	if (bDrawImage)
	{
		szImage = CSize(m_pIcon->GetWidth(), m_pIcon->GetHeight());
		bDrawImageTop = (szImage.cy <= 16);

		if (bDrawImageTop)
		{
			CPoint ptIcon = bLayoutRTL ? CPoint(rc.right - szImage.cx, rc.top) : rc.TopLeft();
			m_pIcon->Draw(pDC, ptIcon);

			if (bLayoutRTL)
			{
				if (bDrawTitle) rcTitle.right -= szImage.cx + 1; else rc.right -= szImage.cx + 3;
			}
			else
			{
				if (bDrawTitle) rcTitle.left += szImage.cx + 1; else rc.left += szImage.cx + 3;
			}
		}
	}

	if (bDrawTitle)
	{
		CXTPFontDC fnt(pDC, &m_pContext->m_fntTitle);
		pDC->DrawText(m_strTitle, rcTitle, dwFlags | DT_SINGLELINE);

		rc.top += pDC->GetTextExtent(m_strTitle).cy;

		if (bLayoutRTL)
		{
			rc.DeflateRect(15, 12, 10, 0);
		}
		else
		{
			rc.DeflateRect(10, 12, 15, 0);
		}
	}

	if (bDrawImage && !bDrawImageTop)
	{
		CPoint ptIcon = bLayoutRTL ? CPoint(rc.right - szImage.cx, rc.top) : rc.TopLeft();
		m_pIcon->Draw(pDC, ptIcon);
		if (bLayoutRTL) rc.right -= szImage.cx + 5; else rc.left += szImage.cx + 5;
	}

	pDC->DrawText(str, rc, dwFlags | DT_WORDBREAK);
}

void CXTPToolTipContextToolTip::DrawBackground(CDC* pDC, TOOLITEM* /*lpToolInfo*/, CRect rc)
{
	m_pContext->DrawBackground(pDC, rc);
}

void CXTPToolTipContextToolTip::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CXTPClientRect rc(this);

	DrawBackground(&dc, &m_toolVisible, rc);

	COLORREF clrTextColor = m_pContext->GetTipTextColor();
	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(clrTextColor);
	CXTPFontDC font(&dc, &m_pContext->m_fnt);

	DrawEntry(&dc, &m_toolVisible, rc);
}

void CXTPToolTipContext::DrawBackground(CDC* pDC, CRect rc)
{
	if (m_dwStyle & XTP_TTS_OFFICE2007FRAME)
	{
		CXTPResourceImages* pImages = XTPResourceImages();

		COLORREF clrLight = pImages->GetImageColor(_T("Window"), _T("TooltipLight"));
		COLORREF clrDark = pImages->GetImageColor(_T("Window"), _T("TooltipDark"));

		if (clrLight == COLORREF_NULL) clrLight = RGB(255, 255, 255);
		if (clrDark == COLORREF_NULL) clrDark = RGB(201, 217, 239);

		XTPDrawHelpers()->GradientFill(pDC, rc, clrLight, clrDark, FALSE);

		CXTPResourceImage* pImage = pImages->LoadFile(_T("TOOLTIPFRAME"));
		if (pImage)
		{
			pImage->DrawImage(pDC, rc, pImage->GetSource(0, 1), CRect(3, 3, 3, 3), 0xFF00FF);
		}
		else
		{
			COLORREF clrBorder = pImages->GetImageColor(_T("Window"), _T("TooltipBorder"));
			if (clrBorder == COLORREF_NULL) clrBorder = RGB(118, 118, 118);
			pDC->Draw3dRect(rc, clrBorder, clrBorder);
		}
	}
	else
	{
		pDC->FillSolidRect(rc, GetTipBkColor());

		if (m_dwStyle & XTP_TTS_OFFICEFRAME)
			pDC->Draw3dRect(rc, GetSysColor(COLOR_3DFACE), 0);
		else
			pDC->Draw3dRect(rc, 0, 0);
	}

	pDC->SetTextColor(GetTipTextColor());
}

BOOL CXTPToolTipContextToolTip::OnEraseBkgnd(CDC* /*pDC*/)
{
	return TRUE;
}

LRESULT CXTPToolTipContextToolTip::OnNcHitTest(CPoint /*point*/)
{
	return (LRESULT)HTTRANSPARENT;
}

int CXTPToolTipContextToolTip::OnMouseActivate(CWnd* /*pDesktopWnd*/, UINT /*nHitTest*/, UINT /*message*/)
{
	return MA_NOACTIVATE;
}

void CXTPToolTipContextToolTip::OnMouseMove(UINT /*nFlags*/, CPoint /*point*/)
{
	SetVisibleTool(NULL);
}

void CXTPToolTipContextToolTip::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == m_nDelayTimer)
	{
		if (!m_toolDelay.IsEmpty())
		{
			TOOLITEM* pVisibleTool = FindTool();

			if (m_toolDelay.IsEqual(pVisibleTool))
			{
				SetVisibleTool(pVisibleTool);
			}
		}
		KillTimer(m_nDelayTimer);
		m_nDelayTimer = 0;
		m_toolDelay.Reset();
	}

	if (nIDEvent == m_nAutoPopTimer)
	{
		KillTimer(m_nAutoPopTimer);
		m_nAutoPopTimer = 0;

		Activate(FALSE);
	}

	CWnd::OnTimer(nIDEvent);
}

//////////////////////////////////////////////////////////////////////////
// CXTPToolTipContext::CStandardToolTip

class CXTPToolTipContext::CStandardToolTip : public CToolTipCtrl
{
	DECLARE_MESSAGE_MAP()
	LRESULT OnWindowFromPoint(WPARAM, LPARAM);
};

typedef CXTPToolTipContext::CStandardToolTip CXTPToolTipContextStandardTip;

BEGIN_MESSAGE_MAP(CXTPToolTipContextStandardTip, CToolTipCtrl)
	ON_MESSAGE(TTM_WINDOWFROMPOINT, OnWindowFromPoint)
END_MESSAGE_MAP()

LRESULT CXTPToolTipContext::CStandardToolTip::OnWindowFromPoint(WPARAM, LPARAM)
{
	HWND hWnd = (HWND)Default();


	if ((hWnd != NULL) && (GetWindowLong(hWnd, GWL_STYLE) & WS_CHILD) && (::SendMessage(hWnd, XTP_TTM_WINDOWFROMPOINT, 0, 0) == 1))
	{
		return (LRESULT)::GetParent(hWnd);
	}

	return (LRESULT)hWnd;
}

//////////////////////////////////////////////////////////////////////////
// CXTPToolTipContext::CLunaToolTip

class CXTPToolTipContext::CLunaToolTip : public CXTPToolTipContextToolTip
{
public:
	CLunaToolTip(CXTPToolTipContext* pContext)
		: CXTPToolTipContextToolTip(pContext)
	{

	}

	void DrawBackground(CDC* pDC, TOOLITEM* /*lpToolInfo*/, CRect rc)
	{
		switch (XTPColorManager()->GetCurrentSystemTheme())
		{
			case xtpSystemThemeBlue:
			case xtpSystemThemeRoyale:
			case xtpSystemThemeAero:
				XTPDrawHelpers()->GradientFill(pDC, rc, RGB(255, 212, 151), RGB(255, 242, 200), FALSE);
				pDC->Draw3dRect(rc, RGB(0, 0, 128), RGB(0, 0, 128));
				break;
			case xtpSystemThemeOlive:
				XTPDrawHelpers()->GradientFill(pDC, rc, RGB(255, 212, 151), RGB(255, 242, 200), FALSE);
				pDC->Draw3dRect(rc, RGB(63, 93, 56), RGB(63, 93, 56));
				break;
			case xtpSystemThemeSilver:
				XTPDrawHelpers()->GradientFill(pDC, rc, RGB(255, 212, 151), RGB(255, 242, 200), FALSE);
				pDC->Draw3dRect(rc, RGB(75, 75, 111), RGB(75, 75, 111));
				break;
			default:
				XTPDrawHelpers()->GradientFill(pDC, rc, m_pContext->GetTipBkColor(), RGB(255, 255, 255), FALSE);
				pDC->Draw3dRect(rc, 0, 0);
		}
	}
};

//////////////////////////////////////////////////////////////////////////
// CXTPToolTipContext::CRichEditToolTip

class CXTPToolTipContext::CRichEditToolTip : public CXTPToolTipContextToolTip
{
public:
	CRichEditToolTip(CXTPToolTipContext* pContext)
		: CXTPToolTipContextToolTip(pContext)
	{

	}

protected:
	virtual void DrawEntry(CDC* pDC, TOOLITEM* lpToolInfo, CRect rc);
	virtual CSize GetToolSize(TOOLITEM* lpToolInfo);

protected:
	CXTPRichRender m_render;
};

CSize CXTPToolTipContext::CRichEditToolTip::GetToolSize(TOOLITEM* lpToolInfo)
{
	if (!m_render.GetTextService())
		return CSize(0);

	CString sText = GetToolText(lpToolInfo);
	if (sText.IsEmpty())
		return CSize(0);

	CClientDC dc(this);
	CXTPFontDC font(&dc, &m_pContext->m_fnt);

	CHARFORMATW cf;
	ZeroMemory(&cf, sizeof(CHARFORMATW));

	cf.cbSize = sizeof(CHARFORMATW);
	cf.dwMask = CFM_BOLD | CFM_COLOR | CFM_ITALIC | CFM_PROTECTED | CFM_STRIKEOUT | CFM_UNDERLINE;
	cf.crTextColor = m_pContext->GetTipTextColor();

	LOGFONT lf;
	if (m_pContext->GetFont()->GetLogFont(&lf))
	{
		cf.yHeight = -MulDiv(lf.lfHeight, 1440, ::GetDeviceCaps(dc, LOGPIXELSY));
		WCSNCPY_S(cf.szFaceName, LF_FACESIZE, XTP_CT2CW(lf.lfFaceName), LF_FACESIZE);
		cf.dwMask |= CFM_FACE | CFM_SIZE;
	}

	m_render.SetText(NULL);
	m_render.SetDefaultCharFormat(&cf);
	m_render.SetText(sText);

	CRect rcMargin = m_pContext->GetMargin();
	CSize szMargin(3 + rcMargin.left + rcMargin.right + 3, 3 + rcMargin.top + rcMargin.bottom + 3);

	CSize size = m_render.GetTextExtent(&dc, GetMaxTipWidth() - szMargin.cx);

	size += szMargin;

	return size;
}

void CXTPToolTipContext::CRichEditToolTip::DrawEntry(CDC* pDC, TOOLITEM* /*lpToolInfo*/, CRect rc)
{
	rc.DeflateRect(m_pContext->GetMargin());
	rc.DeflateRect(3, 3, 3, 3);

	m_render.DrawText(pDC, rc);
}

class CXTPToolTipContext::CMarkupToolTip : public CXTPToolTipContextToolTip
{
public:
	CMarkupToolTip(CXTPToolTipContext* pContext)
		: CXTPToolTipContextToolTip(pContext)
	{
		m_pUIElement = NULL;
		m_pMarkupContext = XTPMarkupCreateContext();
	}

	~CMarkupToolTip()
	{
		XTPMarkupReleaseElement(m_pUIElement);
		XTPMarkupReleaseContext(m_pMarkupContext);
	}

protected:
	virtual void DrawEntry(CDC* pDC, TOOLITEM* lpToolInfo, CRect rc);
	virtual CSize GetToolSize(TOOLITEM* lpToolInfo);

protected:
	CXTPMarkupUIElement* m_pUIElement;
	CXTPMarkupContext* m_pMarkupContext;
};

CSize CXTPToolTipContext::CMarkupToolTip::GetToolSize(TOOLITEM* lpToolInfo)
{
	XTPMarkupReleaseElement(m_pUIElement);

	CString sText = GetToolText(lpToolInfo);
	if (sText.IsEmpty())
		return CSize(0);

	XTPMarkupSetDefaultFont(m_pMarkupContext, (HFONT)m_pContext->GetFont()->GetSafeHandle(), m_pContext->GetTipTextColor());

	m_pUIElement = XTPMarkupParseText(m_pMarkupContext, sText);

	if (!m_pUIElement)
	{
		return CXTPToolTipContextToolTip::GetToolSize(lpToolInfo);
	}

	CRect rcMargin = m_pContext->GetMargin();
	CSize szMargin(3 + rcMargin.left + rcMargin.right + 3, 3 + rcMargin.top + rcMargin.bottom + 3);

	CSize size = XTPMarkupMeasureElement(m_pUIElement, GetMaxTipWidth() - szMargin.cx);

	size += szMargin;

	return size;
}

void CXTPToolTipContext::CMarkupToolTip::DrawEntry(CDC* pDC, TOOLITEM* lpToolInfo, CRect rc)
{
	if (m_pUIElement)
	{
		CRect rcMargin = m_pContext->GetMargin();

		CRect rcText(rc);
		rcText.DeflateRect(3 + rcMargin.left, 3 + rcMargin.top, rcMargin.right + 3, rcMargin.bottom + 3);

		XTPMarkupRenderElement(m_pUIElement, pDC->GetSafeHdc(), rcText);
	}
	else
	{
		CXTPToolTipContextToolTip::DrawEntry(pDC, lpToolInfo, rc);
	}
}

#ifndef _XTP_EXCLUDE_HTML

//////////////////////////////////////////////////////////////////////////
// CXTPToolTipContext::CHTMLToolTip

class CXTPToolTipContext::CHTMLToolTip : public CXTPToolTipContextToolTip
{
public:
	CHTMLToolTip(CXTPToolTipContext* pContext);
	~CHTMLToolTip();

protected:
	virtual void DrawEntry(CDC* pDC, TOOLITEM* lpToolInfo, CRect rc);
	virtual CSize GetToolSize(TOOLITEM* lpToolInfo);
	virtual void OnVisibleChanged(BOOL bVisble);

protected:
	DECLARE_INTERFACE_MAP()

BEGIN_INTERFACE_PART(OleClientSite, IOleClientSite)
	STDMETHOD(SaveObject)( void) { return E_NOTIMPL;}
	STDMETHOD (GetMoniker)(
		/* [in] */ DWORD /*dwAssign*/,
		/* [in] */ DWORD /*dwWhichMoniker*/,
		/* [out] */ IMoniker ** /*ppmk*/) { return E_NOTIMPL;}
	STDMETHOD(GetContainer)(
		/* [out] */ IOleContainer ** /*ppContainer*/) { return E_NOTIMPL;}
	STDMETHOD(ShowObject)( void) { return E_NOTIMPL;}
	STDMETHOD(OnShowWindow)(
		/* [in] */ BOOL /*fShow*/) { return E_NOTIMPL;}
	STDMETHOD(RequestNewObjectLayout)( void) { return E_NOTIMPL;}
END_INTERFACE_PART(OleClientSite)

BEGIN_INTERFACE_PART(DocHostUIHandler, IDocHostUIHandler)
	STDMETHOD(ShowContextMenu)(/* [in] */ DWORD dwID,
		/* [in] */ POINT __RPC_FAR *ppt,
		/* [in] */ IUnknown __RPC_FAR *pcmdtReserved,
		/* [in] */ IDispatch __RPC_FAR *pdispReserved);
	STDMETHOD(GetHostInfo)(
		/* [out][in] */ DOCHOSTUIINFO __RPC_FAR *pInfo);
	STDMETHOD(ShowUI)(
		/* [in] */ DWORD dwID,
		/* [in] */ IOleInPlaceActiveObject __RPC_FAR *pActiveObject,
		/* [in] */ IOleCommandTarget __RPC_FAR *pCommandTarget,
		/* [in] */ IOleInPlaceFrame __RPC_FAR *pFrame,
		/* [in] */ IOleInPlaceUIWindow __RPC_FAR *pDoc);
	STDMETHOD(HideUI)(void);
	STDMETHOD(UpdateUI)(void);
	STDMETHOD(EnableModeless)(/* [in] */ BOOL fEnable);
	STDMETHOD(OnDocWindowActivate)(/* [in] */ BOOL fEnable);
	STDMETHOD(OnFrameWindowActivate)(/* [in] */ BOOL fEnable);
	STDMETHOD(ResizeBorder)(
		/* [in] */ LPCRECT prcBorder,
		/* [in] */ IOleInPlaceUIWindow __RPC_FAR *pUIWindow,
		/* [in] */ BOOL fRameWindow);
	STDMETHOD(TranslateAccelerator)(
		/* [in] */ LPMSG lpMsg,
		/* [in] */ const GUID __RPC_FAR *pguidCmdGroup,
		/* [in] */ DWORD nCmdID);
	STDMETHOD(GetOptionKeyPath)(
		/* [out] */ LPOLESTR __RPC_FAR *pchKey,
		/* [in] */ DWORD dw);
	STDMETHOD(GetDropTarget)(
		/* [in] */ IDropTarget __RPC_FAR *pDropTarget,
		/* [out] */ IDropTarget __RPC_FAR *__RPC_FAR *ppDropTarget);
	STDMETHOD(GetExternal)(
		/* [out] */ IDispatch __RPC_FAR *__RPC_FAR *ppDispatch);
	STDMETHOD(TranslateUrl)(
		/* [in] */ DWORD dwTranslate,
		/* [in] */ OLECHAR __RPC_FAR *pchURLIn,
		/* [out] */ OLECHAR __RPC_FAR *__RPC_FAR *ppchURLOut);
	STDMETHOD(FilterDataObject)(
		/* [in] */ IDataObject __RPC_FAR *pDO,
		/* [out] */ IDataObject __RPC_FAR *__RPC_FAR *ppDORet);
END_INTERFACE_PART(DocHostUIHandler)


protected:
	CBitmap m_bmpScreen;
	CWnd m_wndBrowser;
	IWebBrowser2* m_pBrowserApp;
};


//////////////////////////////////////////////////////////////////////////
// CWebBrowserSite

BEGIN_INTERFACE_MAP(CXTPToolTipContext::CHTMLToolTip, CWnd)
	INTERFACE_PART(CXTPToolTipContext::CHTMLToolTip, IID_IOleClientSite, OleClientSite)
	INTERFACE_PART(CXTPToolTipContext::CHTMLToolTip, IID_IDocHostUIHandler, DocHostUIHandler)
END_INTERFACE_MAP()

#define IMPLEMENT_INTERFACE_PART(theClass, localClass)\
STDMETHODIMP_(ULONG) theClass::X##localClass::AddRef()\
{\
	METHOD_PROLOGUE(theClass, localClass)\
	return pThis->ExternalAddRef();\
}\
STDMETHODIMP_(ULONG) theClass::X##localClass::Release()\
{\
	METHOD_PROLOGUE(theClass, localClass)\
	return pThis->ExternalRelease();\
}\
STDMETHODIMP theClass::X##localClass::QueryInterface(REFIID riid, void **ppvObj)\
{\
	METHOD_PROLOGUE(theClass, localClass)\
	HRESULT hr = (HRESULT)pThis->ExternalQueryInterface(&riid, ppvObj);\
	return hr;\
}

IMPLEMENT_INTERFACE_PART(CXTPToolTipContext::CHTMLToolTip, OleClientSite)

IMPLEMENT_INTERFACE_PART(CXTPToolTipContext::CHTMLToolTip, DocHostUIHandler)

STDMETHODIMP CXTPToolTipContext::CHTMLToolTip::XDocHostUIHandler::GetHostInfo( DOCHOSTUIINFO* pInfo )
{
	pInfo->dwFlags = 0x40000 | DOCHOSTUIFLAG_NO3DBORDER;
	pInfo->dwFlags |= DOCHOSTUIFLAG_DIALOG;
	pInfo->dwFlags |= DOCHOSTUIFLAG_SCROLL_NO;
	pInfo->dwDoubleClick = DOCHOSTUIDBLCLK_DEFAULT;
	return S_OK;
}

STDMETHODIMP  CXTPToolTipContext::CHTMLToolTip::XDocHostUIHandler::ShowUI(
				DWORD /*dwID*/,
				IOleInPlaceActiveObject * /*pActiveObject*/,
				IOleCommandTarget * /*pCommandTarget*/,
				IOleInPlaceFrame * /*pFrame*/,
				IOleInPlaceUIWindow * /*pDoc*/)
{
	return S_OK;
}

STDMETHODIMP  CXTPToolTipContext::CHTMLToolTip::XDocHostUIHandler::HideUI(void)
{
	return S_OK;
}

STDMETHODIMP  CXTPToolTipContext::CHTMLToolTip::XDocHostUIHandler::UpdateUI(void)
{
	return S_OK;
}

STDMETHODIMP  CXTPToolTipContext::CHTMLToolTip::XDocHostUIHandler::EnableModeless(BOOL /*fEnable*/)
{
	return E_NOTIMPL;
}

STDMETHODIMP  CXTPToolTipContext::CHTMLToolTip::XDocHostUIHandler::OnDocWindowActivate(BOOL /*fActivate*/)
{
	return E_NOTIMPL;
}

STDMETHODIMP  CXTPToolTipContext::CHTMLToolTip::XDocHostUIHandler::OnFrameWindowActivate(BOOL /*fActivate*/)
{
	return E_NOTIMPL;
}

STDMETHODIMP CXTPToolTipContext::CHTMLToolTip::XDocHostUIHandler::ResizeBorder(
				LPCRECT /*prcBorder*/,
				IOleInPlaceUIWindow* /*pUIWindow*/,
				BOOL /*fRameWindow*/)
{
	return E_NOTIMPL;
}

STDMETHODIMP  CXTPToolTipContext::CHTMLToolTip::XDocHostUIHandler::ShowContextMenu(
				DWORD /*dwID*/,
				POINT* /*pptPosition*/,
				IUnknown* /*pCommandTarget*/,
				IDispatch* /*pDispatchObjectHit*/)
{
	return S_OK;
}

STDMETHODIMP  CXTPToolTipContext::CHTMLToolTip::XDocHostUIHandler::TranslateAccelerator(LPMSG /*lpMsg*/,
			/* [in] */ const GUID __RPC_FAR* /*pguidCmdGroup*/,
			/* [in] */ DWORD /*nCmdID*/)
{
	return S_FALSE;
}

STDMETHODIMP  CXTPToolTipContext::CHTMLToolTip::XDocHostUIHandler::GetOptionKeyPath(BSTR* /*pbstrKey*/, DWORD)
{
	return E_NOTIMPL;
}

STDMETHODIMP CXTPToolTipContext::CHTMLToolTip::XDocHostUIHandler::GetDropTarget(
			/* [in] */ IDropTarget __RPC_FAR* /*pDropTarget*/,
			/* [out] */ IDropTarget __RPC_FAR *__RPC_FAR* /*ppDropTarget*/)
{
	return E_NOTIMPL;
}

STDMETHODIMP CXTPToolTipContext::CHTMLToolTip::XDocHostUIHandler::GetExternal(
			/* [out] */ IDispatch __RPC_FAR *__RPC_FAR* /*ppDispatch*/)
{
	return E_NOTIMPL;
}

STDMETHODIMP CXTPToolTipContext::CHTMLToolTip::XDocHostUIHandler::TranslateUrl(
			/* [in] */ DWORD /*dwTranslate*/,
			/* [in] */ OLECHAR __RPC_FAR* /*pchURLIn*/,
			/* [out] */ OLECHAR __RPC_FAR *__RPC_FAR* /*ppchURLOut*/)
{
	return E_NOTIMPL;
}

STDMETHODIMP CXTPToolTipContext::CHTMLToolTip::XDocHostUIHandler::FilterDataObject(
			/* [in] */ IDataObject __RPC_FAR* /*pDO*/,
			/* [out] */ IDataObject __RPC_FAR *__RPC_FAR* /*ppDORet*/)
{
	return E_NOTIMPL;
}

CXTPToolTipContext::CHTMLToolTip::CHTMLToolTip(CXTPToolTipContext* pContext)
	: CXTPToolTipContextToolTip(pContext)
{
	AfxEnableControlContainer();
	m_pBrowserApp = 0;
}

CXTPToolTipContext::CHTMLToolTip::~CHTMLToolTip()
{
	SAFE_RELEASE(m_pBrowserApp);
}

CSize CXTPToolTipContext::CHTMLToolTip::GetToolSize(TOOLITEM* lpToolInfo)
{
	m_bmpScreen.DeleteObject();

	if (m_wndBrowser.GetSafeHwnd() == 0)
	{
		// create the control window
		// AFX_IDW_PANE_FIRST is a safe but arbitrary ID
		if (!m_wndBrowser.CreateControl(CLSID_WebBrowser, 0,
			WS_VISIBLE | WS_CHILD, CRect(0, 0, 200, 200), this, AFX_IDW_PANE_FIRST))
		{
			return 0;
		}

		LPUNKNOWN lpUnk = m_wndBrowser.GetControlUnknown();
		HRESULT hr = lpUnk->QueryInterface(IID_IWebBrowser2, (void**) &m_pBrowserApp);
		if (!SUCCEEDED(hr) || !m_pBrowserApp)
		{
			m_pBrowserApp = NULL;
			m_wndBrowser.DestroyWindow();
			return 0;
		}

		IOleObject* spOleObj = NULL;

		m_pBrowserApp->QueryInterface(IID_IOleObject, (void**)&spOleObj);
		if (spOleObj)
		{
			spOleObj->SetClientSite((IOleClientSite*)GetInterface(&IID_IOleClientSite));
			spOleObj->Release();
		}

		CString strFaceName = _T("Arial");
		int nFontSize = 11;

		LOGFONT lf;
		if (m_pContext->GetFont()->GetLogFont(&lf))
		{
			strFaceName = lf.lfFaceName;
			nFontSize = lf.lfHeight < 0 ? -lf.lfHeight : lf.lfHeight;
		}

		CString strBackColor; COLORREF clrBack = m_pContext->GetTipBkColor();
		strBackColor.Format(_T("#%0.2x%0.2x%0.2x"), GetRValue(clrBack), GetGValue(clrBack), GetBValue(clrBack));

		CString strBody;
		strBody.Format(
			_T("about:<head><style> body {font-family:%s; font-size:%i;}")
			_T("table {font-family:%s; font-size:%i;}</style></head>")
			_T("<body topmargin=0 leftmargin=0 bottommargin=0 rightmargin=0 bgColor=%s></body>"),
			(LPCTSTR)strFaceName, nFontSize, (LPCTSTR)strFaceName, nFontSize, (LPCTSTR)strBackColor);

		COleVariant varEmpty;
		BSTR bstrBody = strBody.AllocSysString();
		m_pBrowserApp->Navigate(bstrBody,
			(VARIANT*)&varEmpty,
			(VARIANT*)&varEmpty,
			(VARIANT*)&varEmpty,
			(VARIANT*)&varEmpty);
		SysFreeString(bstrBody);

		READYSTATE rs;
		do
		{
			AfxGetApp()->PumpMessage();
			m_pBrowserApp->get_ReadyState (&rs);
		}
		while (rs != READYSTATE_COMPLETE);
	}

	CString strText = GetToolText(lpToolInfo);

	if (strText.IsEmpty())
		return CSize(0);

	IDispatch* lpDocument = NULL;
	m_pBrowserApp->get_Document(&lpDocument);

	if (lpDocument == 0)
		return 0;

	// Verify that what we get is a pointer to a IHTMLDocument2
	// interface. To be sure, let's query for
	// the IHTMLDocument2 interface (through smart pointers)
	IHTMLDocument2* lpHTML = NULL;
	lpDocument->QueryInterface(IID_IHTMLDocument2, (void**)&lpHTML);
	if (lpHTML == 0)
	{
		lpDocument->Release();
		return 0;
	}

	// The previous step is sufficient to keep Explorer aside
	// without an explicit check against the loader module

	// Extract the source code of the document
	IHTMLElement* lpBody = NULL;

		// Get the BODY object
	HRESULT hr = lpHTML->get_body(&lpBody);
	if (FAILED(hr) || (lpBody == 0))
	{
		lpHTML->Release();
		lpDocument->Release();
		return 0;
	}

	BSTR bstrText = strText.AllocSysString();
	lpBody->put_innerHTML(bstrText);
	SysFreeString(bstrText);

	m_wndBrowser.MoveWindow(0, 0, 650, 200);


	IHTMLTextContainer* lpHTMLContainer = NULL;
	lpBody->QueryInterface(IID_IHTMLTextContainer, (void**)&lpHTMLContainer);

	if (lpHTMLContainer == 0)
	{
		lpBody->Release();
		lpHTML->Release();
		lpDocument->Release();
		return 0;
	}

	long nScrollHeight, nScrollWidth;
	lpHTMLContainer->get_scrollHeight(&nScrollHeight);

	int nBestWidth = m_pContext->GetMaxTipWidth();
	int nBestHeight = nScrollHeight;

	int cxFirst = 0;
	int cxLast = nBestWidth * 2;

	do
	{
		// Taking a guess
		int cx = (cxFirst + cxLast) / 2;

		m_wndBrowser.MoveWindow(0, 0, cx, nScrollHeight);

		lpHTMLContainer->get_scrollHeight(&nScrollHeight);
		lpHTMLContainer->get_scrollWidth(&nScrollWidth);

		if (nScrollWidth > cx && nScrollHeight <= nBestHeight)
		{
			nBestWidth = nScrollWidth;
			break;
		}

		if (nScrollHeight > nBestHeight)
		{
			// If the control required a larger height, then
			// it's too narrow.
			cxFirst = cx + 1;
		}
		else
		{
			// If the control didn't required a larger height,
			// then it's too wide.
			cxLast = cx - 1;
			nBestWidth = cx;
		}
	}
	while (cxFirst <= cxLast);

	m_wndBrowser.MoveWindow(0, 0, nBestWidth, nBestHeight);

	CSize sz(nBestWidth, nBestHeight);
	CRect rcMargin = m_pContext->GetMargin();

#if 0
	if (sz.cx > 0 && sz.cy > 0)
	{
		CWindowDC dcPaint(this);

		CDC dc;
		dc.CreateCompatibleDC(&dcPaint);
		m_bmpScreen.CreateCompatibleBitmap(&dcPaint, sz.cx, sz.cy);

		CBitmap* pOldBitmap = dc.SelectObject(&m_bmpScreen);

		dc.FillSolidRect(0, 0, sz.cx, sz.cy, m_pContext->GetTipBkColor());
		m_wndBrowser.SendMessage(WM_PRINT, (WPARAM)dc.GetSafeHdc(), PRF_CLIENT | PRF_CHILDREN);

		dc.SelectObject(pOldBitmap);
	}
	m_wndBrowser.MoveWindow(0, 0, 0, 0);
#else
	m_wndBrowser.MoveWindow(3 + rcMargin.left, 3 + rcMargin.top, nBestWidth, nBestHeight);
#endif

	CSize szMargin(3 + rcMargin.left + rcMargin.right + 3, 3 + rcMargin.top + rcMargin.bottom + 3);
	sz += szMargin;


	lpHTMLContainer->Release();
	lpBody->Release();
	lpHTML->Release();
	lpDocument->Release();

	return sz;
}

void CXTPToolTipContext::CHTMLToolTip::DrawEntry(CDC* pDC, TOOLITEM* /*lpToolInfo*/, CRect rc)
{
	rc.DeflateRect(m_pContext->GetMargin());
	rc.DeflateRect(3, 3, 3, 3);

	if (m_bmpScreen.GetSafeHandle())
	{
		pDC->DrawState(rc.TopLeft(), CSize(0, 0), &m_bmpScreen, DSS_NORMAL, 0);
	}
}

void CXTPToolTipContext::CHTMLToolTip::OnVisibleChanged(BOOL bVisble)
{
	if (!bVisble)
	{
		m_bmpScreen.DeleteObject();
	}
}

#endif // _XTP_EXCLUDE_HTML


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXTPToolTipContext::CXTPToolTipContext()
{
	m_pToolTip = 0;
	m_bShowTitleAndDescription = FALSE;
	m_bShowImage = FALSE;
	m_nImageSize = ICON_BIG;
	m_nImageBase = 0;
	m_toolStyle = xtpToolTipStandard;
	m_nMaxTipWidth = ::GetSystemMetrics(SM_CXSCREEN) / 2;
	ZeroMemory(&m_lastInfo, sizeof(m_lastInfo));

	m_dwComCtlVersion = XTPSystemVersion()->GetComCtlVersion();

	m_clrTipTextColor = m_clrTipBkColor = COLORREF_NULL;
	m_nIconTitle = TTI_INFO;
	m_dwStyle = TTS_NOPREFIX;
	m_rcMargin.SetRect(0, 0, 0, 0);

	m_nDelayAutoPop = m_nDelayInitial = m_nDelayReshow = -1;


	NONCLIENTMETRICS ncm;
	::ZeroMemory(&ncm, sizeof(NONCLIENTMETRICS));

	ncm.cbSize = sizeof(NONCLIENTMETRICS);
	VERIFY(::SystemParametersInfo(SPI_GETNONCLIENTMETRICS,
		sizeof(NONCLIENTMETRICS), &ncm, 0));

	ncm.lfStatusFont.lfWeight = FW_NORMAL;
	ncm.lfStatusFont.lfCharSet = XTPResourceManager()->GetFontCharset();
	m_fnt.CreateFontIndirect(&ncm.lfStatusFont);

	ncm.lfStatusFont.lfWeight = FW_BOLD;
	m_fntTitle.CreateFontIndirect(&ncm.lfStatusFont);

	m_bStripEllipsisFromToolTip = TRUE;


}

CXTPToolTipContext::~CXTPToolTipContext()
{
	SAFE_DELETEWINDOW(m_pToolTip);

	if (m_pModuleContext == this)
	{
		m_pModuleContext = 0;
	}

}

BOOL CXTPToolTipContext::IsBalloonStyleSupported() const
{
	return m_dwComCtlVersion >= VERSION_IE5;
}

void CXTPToolTipContext::ShowTitleAndDescription(BOOL bShowTitleAndDescription, int nIconTitle)
{
	m_bShowTitleAndDescription = bShowTitleAndDescription;
	m_nIconTitle = nIconTitle;
}

void CXTPToolTipContext::ShowImage(BOOL bShowImage, int nImageBase, int nImageSize)
{
	m_bShowImage = bShowImage;
	m_nImageBase = nImageBase;
	m_nImageSize = nImageSize;

	SAFE_DELETEWINDOW(m_pToolTip);
}

void CXTPToolTipContext::FillInToolInfo(TOOLINFO* pTI, HWND hWnd, CRect rect, INT_PTR nHit, const CString& strToolTip)
{
#if _MSC_VER < 1200 // MFC 5.0
	if (pTI != NULL && pTI->cbSize >= sizeof(TOOLINFO))
#else
	if (pTI != NULL && pTI->cbSize >= sizeof(XTP_TOOLTIP_TOOLINFO))
#endif
	{
		pTI->hwnd = hWnd;
		pTI->rect = rect;
		pTI->uId = nHit;
		pTI->lpszText = (LPTSTR) ::calloc(strToolTip.GetLength() + 1, sizeof(TCHAR));
		if (pTI->lpszText != NULL) STRCPY_S(pTI->lpszText, strToolTip.GetLength() + 1, (LPCTSTR)strToolTip);
	}
}

void CXTPToolTipContext::FillInToolInfo(TOOLINFO* pTI, HWND hWnd, CRect rect, INT_PTR nHit,
	const CString& strToolTip, const CString& strTitle, const CString& strDescription, CXTPImageManager* pImageManager)
{
	FillInToolInfo(pTI, hWnd, rect, nHit, strToolTip);

	if (pTI != NULL && pTI->cbSize == sizeof(XTP_TOOLTIP_TOOLINFO_EX))
	{
		XTP_TOOLTIP_CONTEXT* ptc = ((XTP_TOOLTIP_TOOLINFO_EX*)pTI)->pToolInfo;
		if (!ptc)
			return;

		if (!strDescription.IsEmpty() && !strTitle.IsEmpty())
		{
			ptc->lpszDescription = (LPTSTR)::calloc(strDescription.GetLength() + 1, sizeof(TCHAR));
			if (ptc->lpszDescription != NULL) STRCPY_S(ptc->lpszDescription, strDescription.GetLength() + 1, (LPCTSTR)strDescription);

			ptc->lpszTitle = (LPTSTR)::calloc(strTitle.GetLength() + 1, sizeof(TCHAR));
			if (ptc->lpszTitle != NULL) STRCPY_S(ptc->lpszTitle, strTitle.GetLength() + 1, (LPCTSTR)strTitle);
		}

		ptc->pImageManager = pImageManager;
	}
}


void CXTPToolTipContext::SetMargin(LPCRECT lprc)
{
	if (lprc)
	{
		m_rcMargin = *lprc;
		if (m_pToolTip) m_pToolTip->DestroyWindow();
	}
}

void CXTPToolTipContext::SetStyle(XTPToolTipStyle toolStyle)
{
	if (m_toolStyle != toolStyle && (toolStyle != xtpToolTipBalloon || IsBalloonStyleSupported()))
	{
		m_toolStyle = toolStyle;
		SAFE_DELETEWINDOW(m_pToolTip);

		ModifyToolTipStyle(XTP_TTS_OFFICEFRAME | XTP_TTS_NOSHADOW, m_toolStyle == xtpToolTipOffice ? XTP_TTS_OFFICEFRAME | XTP_TTS_NOSHADOW : 0);
		ModifyToolTipStyle(XTP_TTS_OFFICE2007FRAME, m_toolStyle == xtpToolTipResource ? XTP_TTS_OFFICE2007FRAME : 0);
	}
}

XTPToolTipStyle CXTPToolTipContext::GetStyle() const
{
	return m_toolStyle;
}


void CXTPToolTipContext::SetMaxTipWidth(int iWidth)
{
	m_nMaxTipWidth = iWidth;
	if (m_pToolTip) m_pToolTip->DestroyWindow();
}

int CXTPToolTipContext::GetMaxTipWidth() const
{
	return m_nMaxTipWidth;
}

COLORREF CXTPToolTipContext::GetTipBkColor() const
{
	return m_clrTipBkColor != COLORREF_NULL ? m_clrTipBkColor :GetXtremeColor(COLOR_INFOBK);
}

void CXTPToolTipContext::SetTipBkColor(COLORREF clr)
{
	m_clrTipBkColor = clr;
	if (m_pToolTip) m_pToolTip->DestroyWindow();
}


void CXTPToolTipContext::ModifyToolTipStyle(DWORD dwRemove, DWORD dwAdd)
{
	DWORD dwStyle = (m_dwStyle & ~dwRemove) | dwAdd;

	if (m_dwStyle != dwStyle)
	{
		m_dwStyle = dwStyle;
		SAFE_DELETEWINDOW(m_pToolTip);
	}
}

COLORREF CXTPToolTipContext::GetTipTextColor() const
{
	return m_clrTipTextColor != COLORREF_NULL ? m_clrTipTextColor :
		m_dwStyle & XTP_TTS_OFFICE2007FRAME ?  RGB(76, 76, 76) : GetXtremeColor(COLOR_INFOTEXT);
}

void CXTPToolTipContext::SetTipTextColor(COLORREF clr)
{
	m_clrTipTextColor = clr;
	if (m_pToolTip) m_pToolTip->DestroyWindow();
}

void CXTPToolTipContext::SetDelayTime(DWORD dwDuration, int iTime)
{
	switch (dwDuration)
	{
	case TTDT_RESHOW:
		m_nDelayReshow = iTime;
		break;

	case TTDT_INITIAL:
		m_nDelayInitial = iTime;
		break;

	case TTDT_AUTOPOP:
		m_nDelayAutoPop = iTime;
		break;

	default:
		ASSERT(FALSE);

	}
	if (m_pToolTip) m_pToolTip->DestroyWindow();
}


void CXTPToolTipContext::RelayToolTipMessage(CWnd* pToolTip, MSG* pMsg)
{
	// transate the message based on TTM_WINDOWFROMPOINT
	MSG msg = *pMsg;
	msg.hwnd = (HWND)pToolTip->SendMessage(TTM_WINDOWFROMPOINT, 0, (LPARAM)&msg.pt);
	CPoint pt = pMsg->pt;
	if (msg.message >= WM_MOUSEFIRST && msg.message <= WM_MOUSELAST)
		::ScreenToClient(msg.hwnd, &pt);
	msg.lParam = MAKELONG(pt.x, pt.y);

	// relay mouse event before deleting old tool
	pToolTip->SendMessage(TTM_RELAYEVENT, 0, (LPARAM)&msg);
}


void CXTPToolTipContext::FilterToolTipMessage(CWnd* pWndHost, UINT message, WPARAM wParam, LPARAM lParam)
{
	MSG msg;
	msg.wParam = wParam;
	msg.lParam = lParam;
	msg.message = message;
	msg.hwnd = pWndHost->m_hWnd;
	GetCursorPos(&msg.pt);

	FilterToolTipMessage(pWndHost, &msg);
}

void CXTPToolTipContext::FilterToolTipMessage(CWnd* pWndHost, MSG* pMsg)
{
	FilterToolTipMessageHelper(pWndHost, pMsg, TRUE);
}


void PASCAL CXTPToolTipContext::FilterToolTipMessageStatic(MSG* pMsg, CWnd* pWnd)
{
	if (m_pModuleContext)
	{
		m_pModuleContext->FilterToolTipMessageHelper(pWnd, pMsg, FALSE);
	}
	else
	{
		pWnd->FilterToolTipMessage(pMsg);
	}
}

void CXTPToolTipContext::SetModuleToolTipContext()
{
	AFX_MODULE_STATE* pModuleState = _AFX_CMDTARGET_GETSTATE();
	pModuleState->m_pfnFilterToolTipMessage = &CXTPToolTipContext::FilterToolTipMessageStatic;
	m_pModuleContext = this;
}

CWnd* CXTPToolTipContext::CreateToolTip(CWnd* /*pOwner*/)
{
	switch (m_toolStyle)
	{
	case xtpToolTipOffice:
		return new CXTPToolTipContextToolTip(this);

	case xtpToolTipRTF:
		return new CRichEditToolTip(this);

	case xtpToolTipLuna:
		return new CLunaToolTip(this);

	case xtpToolTipResource:
		return new CXTPToolTipContextToolTip(this);

#ifndef _XTP_EXCLUDE_HTML
	case xtpToolTipHTML:
		return new CHTMLToolTip(this);
#endif

	case xtpToolTipMarkup:
		return new CMarkupToolTip(this);
	}

	return new CStandardToolTip();
}

void CXTPToolTipContext::SetToolTipCtrl(CXTPToolTipContextToolTip* pToolTipCtrl)
{
	SAFE_DELETEWINDOW(m_pToolTip);
	m_pToolTip = pToolTipCtrl;
}

void CXTPToolTipContext::CancelToolTips()
{
	if (m_pToolTip->GetSafeHwnd() != NULL)
		m_pToolTip->SendMessage(TTM_ACTIVATE, FALSE);
}

INT_PTR CXTPToolTipContext::OnToolHitTest(CWnd* pWnd, CPoint point, TOOLINFO* pToolInfo)
{
	return pWnd->OnToolHitTest(point, pToolInfo);
}

void CXTPToolTipContext::FilterToolTipMessageHelper(CWnd* pWndHost, MSG* pMsg, BOOL bIgnoreFlags)
{
	// this CWnd has tooltips enabled
	UINT message = pMsg->message;
	if ((message == WM_MOUSEMOVE || message == WM_NCMOUSEMOVE || message == WM_MOUSELEAVE ||
		message == WM_LBUTTONUP || message == WM_RBUTTONUP ||
		message == WM_MBUTTONUP) &&
		(GetKeyState(VK_LBUTTON) >= 0 && GetKeyState(VK_RBUTTON) >= 0 &&
		GetKeyState(VK_MBUTTON) >= 0))
	{
		if (!bIgnoreFlags)
		{
			// make sure that tooltips are not already being handled
			CWnd* pWnd = CWnd::FromHandle(pMsg->hwnd);
			while (pWnd != NULL && !(pWnd->m_nFlags & (WF_TOOLTIPS | WF_TRACKINGTOOLTIPS)))
			{
				pWnd = pWnd->GetParent();
			}
			if (pWnd != pWndHost)
			{
				return;
			}
		}

		BOOL bTopParentActive = CXTPDrawHelpers::IsTopParentActive(pWndHost->GetSafeHwnd());

		if (m_pToolTip->GetSafeHwnd() && m_pToolTip->IsWindowVisible() && !bTopParentActive)
		{
			m_pToolTip->SendMessage(TTM_ACTIVATE, FALSE);
			return;
		}


		CWnd* pOwner = pWndHost->GetParentOwner();

		if (m_pToolTip != NULL && m_pToolTip->GetOwner() != pOwner)
		{
			m_pToolTip->DestroyWindow();
		}

		if (m_pToolTip == NULL || m_pToolTip->GetSafeHwnd() == NULL)
		{
			if (m_pToolTip == NULL)
			{
				m_pToolTip = CreateToolTip(pOwner);
			}

			if (!m_pToolTip)
				return;

			if (!m_pToolTip->GetSafeHwnd())
			{
				if (DYNAMIC_DOWNCAST(CXTPToolTipContextToolTip, m_pToolTip))
				{
					((CXTPToolTipContextToolTip*)m_pToolTip)->Create(pOwner, m_dwStyle | TTS_ALWAYSTIP | TTS_NOPREFIX);
				}
				else if (DYNAMIC_DOWNCAST(CToolTipCtrl, m_pToolTip))
				{
					((CToolTipCtrl*)m_pToolTip)->Create(pOwner, m_dwStyle | TTS_ALWAYSTIP | TTS_NOPREFIX | (m_toolStyle == xtpToolTipBalloon ? TTS_BALLOON : 0));
				}
				else
				{
					ASSERT(FALSE);
					return;
				}
			}

			if (!m_pToolTip->GetSafeHwnd())
				return;

			m_pToolTip->SetFont(&m_fnt);

			m_pToolTip->SendMessage(TTM_SETMAXTIPWIDTH, 0, m_nMaxTipWidth);
			m_pToolTip->SendMessage(TTM_SETMARGIN, 0, (LPARAM)(LPRECT)&m_rcMargin);

			if (m_clrTipBkColor != COLORREF_NULL)
				m_pToolTip->SendMessage(TTM_SETTIPBKCOLOR, m_clrTipBkColor);

			if (m_clrTipTextColor != COLORREF_NULL)
				m_pToolTip->SendMessage(TTM_SETTIPTEXTCOLOR, m_clrTipTextColor);

			if (m_nDelayReshow != -1)
				m_pToolTip->SendMessage(TTM_SETDELAYTIME, TTDT_RESHOW, MAKELPARAM(m_nDelayReshow, 0));

			if (m_nDelayInitial != -1)
				m_pToolTip->SendMessage(TTM_SETDELAYTIME, TTDT_INITIAL, MAKELPARAM(m_nDelayInitial, 0));

			if (m_nDelayAutoPop != -1)
				m_pToolTip->SendMessage(TTM_SETDELAYTIME, TTDT_AUTOPOP, MAKELPARAM(m_nDelayAutoPop, 0));

			m_pToolTip->SendMessage(TTM_ACTIVATE, FALSE);
		}

		CWnd* pToolTip = m_pToolTip;

		ASSERT_VALID(pToolTip);
		ASSERT(::IsWindow(pToolTip->m_hWnd));

		XTP_TOOLTIP_CONTEXT tc;
		ZeroMemory(&tc, sizeof(tc));

		// determine which tool was hit
		CPoint point = pMsg->pt;
		::ScreenToClient(pWndHost->m_hWnd, &point);

		XTP_TOOLTIP_TOOLINFO_EX tiHit;
		memset(&tiHit, 0, sizeof(XTP_TOOLTIP_TOOLINFO_EX));

		tiHit.cbSize = m_bShowTitleAndDescription || m_bShowImage ? sizeof(XTP_TOOLTIP_TOOLINFO_EX) : sizeof(XTP_TOOLTIP_TOOLINFO);
		tiHit.pToolInfo = &tc;
		INT_PTR nHit = OnToolHitTest(pWndHost, point, (TOOLINFO*)&tiHit);

		if (m_bShowTitleAndDescription && tc.lpszDescription &&
			tc.lpszTitle && (_tcscmp(tc.lpszDescription, tc.lpszTitle) == 0))
		{
			free(tc.lpszDescription);
			tc.lpszDescription = 0;
		}

		if ((m_dwStyle & TTS_NOPREFIX) == 0)
		{
			CXTPDrawHelpers::StripMnemonics(tiHit.lpszText);
			CXTPDrawHelpers::StripMnemonics(tiHit.pToolInfo->lpszTitle);
		}

		if (m_bStripEllipsisFromToolTip)
		{
			XTPStripEllipsis(tiHit.lpszText);
			XTPStripEllipsis(tiHit.pToolInfo->lpszTitle);
		}


		// build new toolinfo and if different than current, register it
		BOOL bTipInfoChanged = m_lastInfo.uId != tiHit.uId || m_lastInfo.hwnd != tiHit.hwnd;

		if (bTipInfoChanged || CRect(m_lastInfo.rect) != CRect(tiHit.rect))
		{
			if (m_lastInfo.cbSize >= sizeof(XTP_TOOLTIP_TOOLINFO) && !bTipInfoChanged)
				pToolTip->SendMessage(TTM_DELTOOL, 0, (LPARAM)&m_lastInfo);

			if (nHit != -1)
			{
				// add new tool and activate the tip
				XTP_TOOLTIP_TOOLINFO_EX ti = tiHit;
				ti.uFlags &= ~(TTF_NOTBUTTON | TTF_ALWAYSTIP);

				VERIFY(pToolTip->SendMessage(TTM_ADDTOOL, 0, (LPARAM)&ti));

				if ((m_toolStyle != xtpToolTipRTF && m_toolStyle != xtpToolTipHTML && m_toolStyle != xtpToolTipMarkup) && ((m_dwComCtlVersion >= VERSION_IE5) || (m_toolStyle != xtpToolTipStandard)))
				{
					if (m_bShowTitleAndDescription && tc.lpszDescription && tc.lpszTitle)
					{
						pToolTip->SendMessage(TTM_SETTITLE, m_nIconTitle, (LPARAM)(LPCTSTR)tc.lpszTitle);

						ti.lpszText = tc.lpszDescription;
						pToolTip->SendMessage(TTM_UPDATETIPTEXT, 0, (LPARAM)&ti);
					}
					else
					{
						pToolTip->SendMessage(TTM_SETTITLE, 0, 0);
					}
				}

				if (m_bShowImage)
				{
					CXTPImageManagerIcon* pIcon = NULL;
					pIcon =  tc.pImageManager ? tc.pImageManager->GetImage(UINT(m_nImageBase + nHit), m_nImageSize) : 0;
					if (pIcon)
					{
						pToolTip->SendMessage(XTP_TTM_SETIMAGE, 0, (LPARAM)pIcon);
					}
					else
					{
						pToolTip->SendMessage(XTP_TTM_SETIMAGE, 0, NULL);
					}
				}

				if ((tiHit.uFlags & TTF_ALWAYSTIP) || bTopParentActive)
				{
					// allow the tooltip to popup when it should
					pToolTip->SendMessage(TTM_ACTIVATE, TRUE);

					// bring the tooltip window above other popup windows
					::SetWindowPos(pToolTip->m_hWnd, HWND_TOP, 0, 0, 0, 0,
						SWP_NOACTIVATE | SWP_NOSIZE | SWP_NOMOVE | SWP_NOOWNERZORDER);
				}
			}
			else
			{
				pToolTip->SendMessage(TTM_ACTIVATE, FALSE);
			}

			// relay mouse event before deleting old tool
			RelayToolTipMessage(pToolTip, pMsg);

			//// now safe to delete the old tool
			if (m_lastInfo.cbSize >= sizeof(XTP_TOOLTIP_TOOLINFO) && bTipInfoChanged)
				pToolTip->SendMessage(TTM_DELTOOL, 0, (LPARAM)&m_lastInfo);

			m_lastInfo = tiHit;
		}
		else
		{
			// relay mouse events through the tooltip
			if (nHit != -1)
				RelayToolTipMessage(pToolTip, pMsg);
		}

		if ((tiHit.lpszText != LPSTR_TEXTCALLBACK) && (tiHit.hinst == 0))
			free(tiHit.lpszText);

		if (tc.lpszDescription  != 0)
			free(tc.lpszDescription);

		if (tc.lpszTitle != 0)
			free(tc.lpszTitle);
	}
	else
	{
		if (!bIgnoreFlags)
		{
			// make sure that tooltips are not already being handled
			CWnd* pWnd = CWnd::FromHandle(pMsg->hwnd);
			while (pWnd != NULL && pWnd != pWndHost && !(pWnd->m_nFlags & (WF_TOOLTIPS | WF_TRACKINGTOOLTIPS)))
			{
				pWnd = pWnd->GetParent();
			}
			if (pWnd != pWndHost)
				return;
		}

		BOOL bKeys = (message >= WM_KEYFIRST && message <= WM_KEYLAST) ||
			(message >= WM_SYSKEYFIRST && message <= WM_SYSKEYLAST);
		if ((bKeys ||
			(message == WM_LBUTTONDOWN || message == WM_LBUTTONDBLCLK) ||
			(message == WM_RBUTTONDOWN || message == WM_RBUTTONDBLCLK) ||
			(message == WM_MBUTTONDOWN || message == WM_MBUTTONDBLCLK) ||
			(message == WM_NCLBUTTONDOWN || message == WM_NCLBUTTONDBLCLK) ||
			(message == WM_NCRBUTTONDOWN || message == WM_NCRBUTTONDBLCLK) ||
			(message == WM_NCMBUTTONDOWN || message == WM_NCMBUTTONDBLCLK)))
		{
			CancelToolTips();
		}
	}
}

