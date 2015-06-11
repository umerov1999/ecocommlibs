// XTPRibbonBackstageView.cpp : implementation file
//
// This file is a part of the XTREME RIBBON MFC class library.
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
#include <Common/XTPImageManager.h>
#include <Common/XTPResourceImage.h>
#include <Common/XTPColorManager.h>
#include <Common/XTPSystemHelpers.h>
#include <Common/XTPHookManager.h>
#include <Common/ScrollBar/XTPScrollInfo.h>

#include <CommandBars/XTPCommandBarsDefines.h>
#include <CommandBars/XTPCommandBars.h>
#include <CommandBars/XTPControls.h>
#include <CommandBars/XTPControl.h>
#include <CommandBars/XTPControlButton.h>
#include <CommandBars/XTPControlPopup.h>
#include <CommandBars/XTPKeyboardManager.h>
#include <CommandBars/XTPCommandBar.h>
#include <CommandBars/XTPToolBar.h>
#include <CommandBars/XTPMenuBar.h>
#include <CommandBars/XTPPopupBar.h>

#include <Ribbon/XTPRibbonPopups.h>
#include <Ribbon/XTPRibbonBar.h>
#include <Ribbon/XTPRibbonSystemButton.h>
#include <Ribbon/Backstage/XTPRibbonBackstageView.h>
#include <Ribbon/Backstage/XTPRibbonBackstageCommand.h>
#include <Ribbon/Backstage/XTPRibbonBackstageTab.h>

#include <Ribbon/Backstage/XTPRibbonBackstagePaintManager.h>
#include <Ribbon/Backstage/XTPRibbonBackstageViewThemeResource.h>
#include <Ribbon/Backstage/XTPRibbonBackstageViewThemeOffice2013.h>


IMPLEMENT_XTP_COMMANDBAR(CXTPRibbonBackstageView, CXTPCommandBar);

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



CXTPRibbonBackstageView::CXTPRibbonBackstageView()
	: m_pPaintManager(NULL)
{
	m_nMenuWidth = 0;

#define CLEARTYPE_QUALITY   5

	LOGFONT lf;
	XTPDrawHelpers()->GetIconLogFont(&lf);

	lf.lfQuality = CLEARTYPE_QUALITY;

	m_fntCommand.CreateFontIndirect(&lf);

	HDC hDC = ::GetDC(NULL);
	UINT uPointSize = MulDiv(-lf.lfHeight, 72, ::GetDeviceCaps(hDC, LOGPIXELSY));
	lf.lfHeight = -MulDiv(++uPointSize, ::GetDeviceCaps(hDC, LOGPIXELSY), 72);
	::ReleaseDC(NULL, hDC);

	m_fntTab.CreateFontIndirect(&lf);

	m_pHighlightedControl = NULL;
	m_pFocusedControl = NULL;
	m_pActiveTab = NULL;

	m_pSystemButton = NULL;

	SetTheme(xtpThemeResource);
}

CXTPRibbonBackstageView::~CXTPRibbonBackstageView()
{
	SAFE_DELETE(m_pPaintManager);
}

BEGIN_MESSAGE_MAP(CXTPRibbonBackstageView, CWnd)
	ON_WM_PAINT()
	ON_WM_KEYDOWN()
	ON_COMMAND(IDCANCEL, OnCancel)
	ON_WM_MOUSEMOVE()
	ON_MESSAGE_VOID(WM_MOUSELEAVE, OnMouseLeave)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_GETDLGCODE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


CXTPRibbonBackstageView* AFX_CDECL CXTPRibbonBackstageView::CreateBackstageView(CXTPCommandBars *pCommandBars)
{
	CXTPRibbonBackstageView *pView = new CXTPRibbonBackstageView();
	pView->SetCommandBars(pCommandBars);

	return pView;
}

CXTPRibbonBackstageCommand* CXTPRibbonBackstageView::AddCommand(UINT nID)
{
	CXTPRibbonBackstageCommand *pControl = new CXTPRibbonBackstageCommand();
	GetControls()->Add(pControl, nID);
	return pControl;
}

CXTPRibbonBackstageTab* CXTPRibbonBackstageView::AddTab(CWnd *pWnd, UINT nID)
{
	CXTPRibbonBackstageTab *pTab = new CXTPRibbonBackstageTab(pWnd->GetSafeHwnd());
	GetControls()->Add(pTab, nID);

	return pTab;
}

int CXTPRibbonBackstageView::IndexOf(CXTPControl *pControl) const
{
	if (NULL == pControl)
		return -1;

	for (int i = 0; i<GetControlCount(); i++)
	{
		if (GetControl(i) == pControl)
		{
			return i;
		}
	}

	return -1;
}

CWnd* CXTPRibbonBackstageView::GetActiveControlPane() const
{
	CXTPRibbonBackstageTab *pControlCustom = DYNAMIC_DOWNCAST(CXTPRibbonBackstageTab, m_pActiveTab);

	if (pControlCustom)
	{
		CWnd *pWnd =  pControlCustom->GetControlPane();
		if (pWnd && !IsWindow(pWnd->m_hWnd))
			return NULL;

		return pWnd;
	}
	return NULL;
}

CXTPRibbonBackstageTab* CXTPRibbonBackstageView::GetActiveTab() const
{
	return m_pActiveTab;
}

void CXTPRibbonBackstageView::SetActiveTab(CXTPRibbonBackstageTab* pTab)
{
	CWnd* pOldContent = GetActiveControlPane();

	m_pActiveTab = pTab;

	CWnd* pNewContent = GetActiveControlPane();

	if (pNewContent)
	{
		pNewContent->ModifyStyle(WS_POPUP, WS_CHILD);
		pNewContent->ModifyStyleEx(0, WS_EX_CONTROLPARENT);
		pNewContent->SetParent(this);
	}

	if (pOldContent && pOldContent->IsKindOf(RUNTIME_CLASS(CPropertyPage)))
	{
		((CPropertyPage*)pOldContent)->OnKillActive();
	}

	if (pOldContent && pOldContent != pNewContent)
	{
		pOldContent->ShowWindow(SW_HIDE);
		pOldContent->SetParent(GetSite());
	}

	if (m_pActiveTab)
	{
		m_pActiveTab->NotifySite(XTP_BS_TABCHANGED);
	}

	if (pNewContent && pNewContent->IsKindOf(RUNTIME_CLASS(CPropertyPage)))
	{
		((CPropertyPage*)pNewContent)->OnSetActive();
	}

	RecalcLayout();
}

CXTPControl* CXTPRibbonBackstageView::GetFocusedControl() const
{
	return m_pFocusedControl;
}

void CXTPRibbonBackstageView::SetFocusedControl(CXTPControl* pFocusedControl)
{
	if (m_pFocusedControl != pFocusedControl)
	{
		m_pFocusedControl = pFocusedControl;

		if (pFocusedControl && !IsCommand(pFocusedControl) && pFocusedControl->GetEnabled())
		{
			SetActiveTab((CXTPRibbonBackstageTab*)pFocusedControl);
		}

		Invalidate(FALSE);
	}
}

CXTPControl* CXTPRibbonBackstageView::GetHighlightedControl() const
{
	return m_pHighlightedControl;
}

BOOL CXTPRibbonBackstageView::IsActiveTab(CXTPControl *pControl) const
{
	return m_pActiveTab == pControl;
}

BOOL CXTPRibbonBackstageView::IsFocusedControl(CXTPControl *pControl) const
{
	return m_pFocusedControl == pControl;
}

BOOL CXTPRibbonBackstageView::IsHighlightedControl(CXTPControl *pControl) const
{
	return m_pHighlightedControl == pControl;
}

void CXTPRibbonBackstageView::SetTheme(XTPPaintTheme theme)
{
	SAFE_DELETE(m_pPaintManager);

	switch(theme)
	{
	case xtpThemeOffice2013:
		m_pPaintManager = new CXTPRibbonBackstageViewThemeOffice2013();
		break;
	default:
		m_pPaintManager = new CXTPRibbonBackstageViewThemeResource();
		break;
	}
}

void CXTPRibbonBackstageView::OnExecute(CXTPControl* pControl)
{
	SetTrackingMode(FALSE);

	if (pControl)
	{
		pControl->OnExecute();
	}
}


void CXTPRibbonBackstageView::OnCancel()
{
	SetTrackingMode(FALSE);
}

CXTPControl* CXTPRibbonBackstageView::HitTest(CPoint point) const
{
	return GetControls()->HitTest(point);
}

BOOL CXTPRibbonBackstageView::IsCommand(CXTPControl *pControl) const
{
	return pControl->IsKindOf(RUNTIME_CLASS(CXTPRibbonBackstageCommand));
}




int CXTPRibbonBackstageView::OnHookMessage(HWND /*hWnd*/, UINT nMessage, WPARAM& wParam, LPARAM& /*lParam*/, LRESULT& /*lResult*/)
{
	if (nMessage == WM_WINDOWPOSCHANGED)
	{
		RecalcLayout();
	}

	if (nMessage == WM_CLOSE)
	{
		OnCancel();
		return FALSE;
	}

	if (nMessage == WM_ACTIVATEAPP)
	{
		SetFocus();
	}

	if (nMessage == WM_KEYDOWN && wParam == VK_ESCAPE)
	{
		if (GetSite()->IsWindowEnabled())
		{
			OnCancel();
			return TRUE;
		}
	}

	if (nMessage == WM_KEYDOWN || nMessage == WM_KEYUP)
	{
		return FALSE_EXIT;
	}

	if (nMessage == WM_MOUSEWHEEL)
	{
		return FALSE_EXIT;
	}

	return FALSE;
}

void CXTPRibbonBackstageView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);

	if (nChar == VK_SPACE || nChar == VK_RETURN)
	{
		if (m_pFocusedControl && IsCommand(m_pFocusedControl) && m_pFocusedControl->GetEnabled())
		{
			OnExecute(m_pFocusedControl);
		}
	}
	else if (nChar == VK_ESCAPE)
	{
		OnCancel();
	}
	else if (nChar == VK_DOWN)
	{
		if (GetControlCount() == 0)
			return;


		int nIndex = IndexOf(m_pFocusedControl);
		if (nIndex == -1) nIndex = IndexOf(m_pActiveTab);


		nIndex = GetControls()->GetNext(nIndex, +1, TRUE);

		SetFocusedControl(GetControl(nIndex));
	}
	else if (nChar == VK_UP)
	{
		if (GetControlCount() == 0)
			return;

		int nIndex = IndexOf(m_pFocusedControl);
		if (nIndex == -1) nIndex = IndexOf(m_pActiveTab);

		nIndex = GetControls()->GetNext(nIndex, -1, TRUE);

		SetFocusedControl(GetControl(nIndex));
	}
	else if (nChar == VK_TAB)
	{
		CWnd* pWnd = GetActiveControlPane();
		if (pWnd)
		{
			pWnd->SetFocus();

			HWND hWnd = ::GetNextDlgTabItem(pWnd->GetSafeHwnd(), NULL, FALSE);

			if (hWnd)
			{
				pWnd->SendMessage(WM_NEXTDLGCTL, (WPARAM)hWnd, 1);
			}
		}
	}
}

UINT CXTPRibbonBackstageView::OnGetDlgCode()
{
	return DLGC_WANTALLKEYS;
}

void CXTPRibbonBackstageView::OnMouseLeave()
{
	OnMouseMove(0, CPoint(-1, -1));
}

void CXTPRibbonBackstageView::OnMouseMove(UINT nFlags, CPoint point)
{
	CWnd::OnMouseMove(nFlags, point);


	CXTPControl* pHighlightedControl = HitTest(point);
	if (pHighlightedControl && !pHighlightedControl->GetEnabled())
		pHighlightedControl = NULL;

	if (pHighlightedControl != m_pHighlightedControl)
	{
		m_pHighlightedControl = pHighlightedControl;

		if (m_pHighlightedControl)
		{
			TRACKMOUSEEVENT tme =
			{
				sizeof(TRACKMOUSEEVENT), TME_LEAVE, m_hWnd
			};
			_TrackMouseEvent(&tme);
		}
		Invalidate(FALSE);
	}
}

void CXTPRibbonBackstageView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CWnd::OnLButtonDown(nFlags, point);

	SetFocus();

	CXTPControl* pFocusedControl = HitTest(point);
	if (pFocusedControl && !pFocusedControl->GetEnabled())
		pFocusedControl = NULL;

	SetFocusedControl(pFocusedControl);
}

void CXTPRibbonBackstageView::OnLButtonUp(UINT nFlags, CPoint point)
{
	CWnd::OnLButtonUp(nFlags, point);

	CXTPControl* pFocusedControl = HitTest(point);

	if (pFocusedControl && IsCommand(pFocusedControl) && pFocusedControl == m_pFocusedControl && pFocusedControl->GetEnabled())
	{
		OnExecute(pFocusedControl);
	}

}


void CXTPRibbonBackstageView::OnPaint()
{
	CPaintDC dcPaint(this);
	CXTPBufferDC dc(dcPaint);

	m_pPaintManager->DrawBackground(&dc, this);

	for (int i = 0; i < GetControlCount(); i++)
	{
		dc.SetTextColor(RGB(0,0,0));
		m_pPaintManager->DrawControl(&dc, this, GetControl(i));
	}
}


void CXTPRibbonBackstageView::RecalcLayout()
{
	CXTPRibbonBar* pRibbonBar = DYNAMIC_DOWNCAST(CXTPRibbonBar, GetCommandBars()->GetMenuBar());
	ASSERT (pRibbonBar);

	int nCaptionHeight = 0;

	if (pRibbonBar->IsWindowVisible())
	{
		nCaptionHeight = 1 + pRibbonBar->GetCaptionHeight() + pRibbonBar->GetRibbonTopBorder() + pRibbonBar->GetTabsHeight();
	}

	CWnd* pParentWnd = GetCommandBars()->GetSite();

	CRect rc;
	pParentWnd->GetClientRect(rc);

	rc.top += nCaptionHeight;

	SetWindowPos(&CWnd::wndTop, rc.left, rc.top, rc.Width(), rc.Height(), SWP_SHOWWINDOW);



	CClientDC dc(this);
	CXTPFontDC font(&dc, &m_fntCommand);

	m_nMenuWidth = XTPDpiHelper()->ScaleX(132);
	int nCommandHeight = XTPDpiHelper()->ScaleY(16) + 10;     // max(26, dc.GetTextExtent(_T("X"), 1).cy + 6 );
	int nPageHeight = XTPDpiHelper()->ScaleY(30) + 7;       // max(37, dc.GetTextExtent(_T("X"), 1).cy + 10 );

	int i;

	for (i = 0; i < GetControlCount(); i++)
	{
		CXTPControl* pControl = GetControl(i);
		if (!pControl->IsVisible())
			continue;

		font.SetFont(IsCommand(pControl) ? &m_fntCommand : &m_fntTab);

		int nWidth = dc.GetTextExtent(pControl->GetCaption()).cx + m_pPaintManager->m_nCommandGap + m_pPaintManager->m_nTextGap + m_pPaintManager->m_nCommandGap + 6;

		CXTPImageManagerIcon* pIcon = GetImageManager()->GetImage(pControl->GetIconId());
		if (pIcon)
		{
			nWidth += pIcon->GetWidth() + m_pPaintManager->m_nImageMargin;
		}

		m_nMenuWidth = max(m_nMenuWidth, nWidth);
	}


	int top = 7;

	for (i = 0; i < GetControlCount(); i++)
	{
		CXTPControl* pControl = GetControl(i);
		if (!pControl->IsVisible())
			continue;

		if (IsCommand(pControl))
		{
			CRect rc(m_pPaintManager->m_nCommandGap, top, m_nMenuWidth - m_pPaintManager->m_nCommandGap, top + nCommandHeight);

			pControl->SetRect(rc);
			top += nCommandHeight + 2;
		}
		else
		{
			CRect rc(0, top, m_nMenuWidth, top + nPageHeight);

			pControl->SetRect(rc);
			top += nPageHeight + 1;
		}
	}

	CWnd* pWnd = GetActiveControlPane();

	if (pWnd)
	{
		CSize szControlPaneMinSize = m_pActiveTab->m_szControlPaneMinSize;


		CRect rcContent(m_nMenuWidth, 2, rc.right, rc.bottom - nCaptionHeight);
		rcContent.right = max(rcContent.right, rcContent.left + szControlPaneMinSize.cx);
		rcContent.bottom = max(rcContent.bottom, rcContent.top + szControlPaneMinSize.cy);


		pWnd->SetWindowPos(0, rcContent.left, rcContent.top, rcContent.Width(), rcContent.Height(), SWP_SHOWWINDOW | SWP_NOZORDER | SWP_NOACTIVATE);
	}

	Invalidate(FALSE);
}


BOOL CXTPRibbonBackstageView::Popup(CXTPControlPopup* pControlPopup, BOOL /*bSelectFirst = FALSE*/)
{
	CXTPRibbonControlSystemButton* pSystemButton = DYNAMIC_DOWNCAST(CXTPRibbonControlSystemButton, pControlPopup);
	ASSERT (pSystemButton);
	if (!pSystemButton)
		return FALSE;

	m_pSystemButton = pSystemButton;

	GetCommandBars()->ClosePopups();

	m_bTracking = TRUE;
	pSystemButton->OnSetPopup(TRUE);

	CXTPRibbonBar* pRibbonBar = DYNAMIC_DOWNCAST(CXTPRibbonBar, pSystemButton->GetParent());
	ASSERT (pRibbonBar);

	m_pFocusedControl = NULL;
	m_pHighlightedControl = NULL;

	// comments by IP
	// (LPARAM)this is anyway not a CXTPPopupBar
	// pSystemButton->GetCommandBar() should be CXTPRibbonSystemPopupBar but only when constructing CXTPRibbonControlSystemButton
	// after that MainFrm.cpp (in all MFC samples) resets it to CXTPRibbonBackstageView via SetCommandBar
	// see mantis #197

	//CWnd* pWndOwner = GetOwnerSite();
	//if (pWndOwner) pWndOwner->SendMessage(WM_XTP_INITCOMMANDSPOPUP, 0, (LPARAM)this);

	ShowBackstageView();

	OnIdleUpdateCmdUI(TRUE, 0L);

	m_bTracking = FALSE;
	SetTrackingMode(TRUE);


	return TRUE;
}

void CXTPRibbonBackstageView::OnDestroy()
{
	m_bTracking = FALSE;

	CWnd* pParentWnd = GetCommandBars()->GetSite();

	XTPHookManager()->RemoveHook(pParentWnd->GetSafeHwnd(), this);

	for (int i = 0; i < m_arrChildWindows.GetSize(); i++)
	{
		HWND hWndChild = m_arrChildWindows[i];
		::ShowWindow(hWndChild, SW_SHOWNOACTIVATE);
	}

	SetActiveTab(NULL);

	if (m_pSystemButton)
	{
		m_pSystemButton->OnSetPopup(FALSE);

		m_pSystemButton->GetParent()->Redraw(0, FALSE);

		GetCommandBars()->RecalcFrameLayout();
	}

	GetCommandBars()->GetKeyboardManager()->UnhookKeyboard(this);

	m_pSystemButton = NULL;

	CWnd::OnDestroy();
}

BOOL CXTPRibbonBackstageView::SetTrackingMode(int bMode, BOOL /*bSelectFirst = TRUE*/, BOOL /*bKeyboard = FALSE*/)
{
	if (m_bTracking == bMode)
		return FALSE;

	m_bTracking = bMode;

	CXTPRibbonBar* pRibbonBar = DYNAMIC_DOWNCAST(CXTPRibbonBar, GetCommandBars()->GetMenuBar());
	ASSERT (pRibbonBar);

	for (int i = 0; i < pRibbonBar->GetControlCount(); i++)
	{
		pRibbonBar->GetControl(i)->OnEnabledChanged();
	}

	if (!m_hWnd)
		return FALSE;

	CWnd* pWnd = GetSite();
	CXTPCommandBars* pCommandBars = GetCommandBars();

	if (!bMode)
	{
		DestroyWindow();

		if (pWnd && ::IsWindow(pWnd->m_hWnd)) pWnd->SendMessage(WM_XTP_UNINITCOMMANDSPOPUP, 0, (LPARAM)this);
	}
	else
	{

	}

	if (pCommandBars)
	{
		pCommandBars->OnTrackingModeChanged(this, bMode);
	}
	return TRUE;
}

void CXTPRibbonBackstageView::ShowBackstageView()
{
	CWnd* pParentWnd = GetCommandBars()->GetSite();

	CWnd::Create(AfxRegisterWndClass(0, AfxGetApp()->LoadStandardCursor(IDC_ARROW)), 0, WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, CRect(0, 0, 0, 0), pParentWnd, 0);


	XTPHookManager()->SetHook(pParentWnd->GetSafeHwnd(), this);

	GetCommandBars()->GetKeyboardManager()->HookKeyboard(this);


	RecalcLayout();


	m_arrChildWindows.RemoveAll();

	for (HWND hWndChild = ::GetWindow(pParentWnd->m_hWnd, GW_CHILD); hWndChild != NULL; hWndChild = ::GetWindow(hWndChild, GW_HWNDNEXT))
	{
		if (::GetDlgCtrlID(hWndChild) == AFX_IDW_DOCKBAR_TOP || hWndChild == m_hWnd)
			continue;

		DWORD dwStyle = ::GetWindowLong(hWndChild, GWL_STYLE);
		if ((dwStyle & (WS_VISIBLE|WS_DISABLED)) == WS_VISIBLE)
		{
			::ShowWindow(hWndChild, SW_HIDE);
			m_arrChildWindows.Add(hWndChild);
		}
	}

	::SetFocus(m_hWnd);

	for (int i = 0; i < GetControlCount(); i++)
	{
		if (GetControl(i)->IsItemDefault())
		{
			SetFocusedControl(GetControl(i));
			break;
		}
	}
}

BOOL AFXAPI _XTPCompareClassName(HWND hWnd, LPCTSTR lpszClassName)
{
	ASSERT(::IsWindow(hWnd));
	TCHAR szTemp[32];
	::GetClassName(hWnd, szTemp, _countof(szTemp));
	return lstrcmpi(szTemp, lpszClassName) == 0;
}

BOOL CXTPRibbonBackstageView::PreTranslateMessage(MSG* pMsg)
{
	// for modeless processing (or modal)
	ASSERT(m_hWnd != NULL);

	// allow tooltip messages to be filtered
	if (CWnd::PreTranslateMessage(pMsg))
		return TRUE;

	// don't translate dialog messages when in Shift+F1 help mode
	CFrameWnd* pFrameWnd = GetTopLevelFrame();
	if (pFrameWnd != NULL && pFrameWnd->m_bHelpMode)
		return FALSE;

	// fix around for VK_ESCAPE in a multiline Edit that is on a Dialog
	// that doesn't have a cancel or the cancel is disabled.
	if (pMsg->message == WM_KEYDOWN &&
		(pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_CANCEL) &&
		(::GetWindowLong(pMsg->hwnd, GWL_STYLE) & ES_MULTILINE) &&
		_XTPCompareClassName(pMsg->hwnd, _T("Edit")))
	{
		HWND hItem = ::GetDlgItem(m_hWnd, IDCANCEL);
		if (hItem == NULL || ::IsWindowEnabled(hItem))
		{
			SendMessage(WM_COMMAND, IDCANCEL, 0);
			return TRUE;
		}
	}
	// filter both messages to dialog and from children
	return PreTranslateInput(pMsg);
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPRibbonBackstageView, CXTPCommandBar)
	DISP_FUNCTION_ID(CXTPRibbonBackstageView, "AddCommand", 1000, OleAddCommand, VT_DISPATCH, VTS_I4 VTS_BSTR)
	DISP_FUNCTION_ID(CXTPRibbonBackstageView, "AddTab", 1001, OleAddTab, VT_DISPATCH, VTS_I4 VTS_BSTR VTS_HANDLE)
	DISP_FUNCTION_ID(CXTPRibbonBackstageView, "AddTab", 1002, OleClose, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CXTPRibbonBackstageView, "SetTheme", 1003, OleSetTheme, VT_EMPTY, VTS_I4)
END_DISPATCH_MAP()

// {66CE0F75-CB1F-40fd-B073-4BC151A5205F}
static const GUID IID_IRibbonBackstageView =
{ 0x66ce0f75, 0xcb1f, 0x40fd, { 0xb0, 0x73, 0x4b, 0xc1, 0x51, 0xa5, 0x20, 0x5f } };


BEGIN_INTERFACE_MAP(CXTPRibbonBackstageView, CXTPCommandBar)
	INTERFACE_PART(CXTPRibbonBackstageView, IID_IRibbonBackstageView, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPRibbonBackstageView, IID_IRibbonBackstageView)


LPDISPATCH CXTPRibbonBackstageView::OleAddCommand(long nId, LPCTSTR lpszCaption)
{
	CXTPControl* pControl = AddCommand(nId);
	pControl->SetCaption(lpszCaption);

	return XTPGetDispatch(pControl);
}

void CXTPRibbonBackstageView::OleSetTheme(long nTheme)
{
	SetTheme((XTPPaintTheme)nTheme);
}

LPDISPATCH CXTPRibbonBackstageView::OleAddTab(long nId, LPCTSTR lpszCaption, OLE_HANDLE hWnd)
{
	CXTPRibbonBackstageTab* pControl = (CXTPRibbonBackstageTab*)GetControls()->Add(new CXTPRibbonBackstageTab((HWND)hWnd), nId);
	pControl->SetCaption(lpszCaption);

	return XTPGetDispatch(pControl);

}

void CXTPRibbonBackstageView::OleClose()
{
	OnCancel();
}

#endif // _XTP_ACTIVEX
