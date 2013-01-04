// XTPRibbonBackstageView.cpp : implementation file
//
// This file is a part of the XTREME RIBBON MFC class library.
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

#include "Common/XTPDrawHelpers.h"
#include "Common/XTPImageManager.h"
#include "Common/XTPResourceImage.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPHookManager.h"

#include "CommandBars/XTPCommandBarsDefines.h"
#include "CommandBars/XTPCommandBars.h"
#include "CommandBars/XTPControls.h"
#include "CommandBars/XTPControl.h"
#include "CommandBars/XTPControlButton.h"
#include "CommandBars/XTPControlPopup.h"
#include "CommandBars/XTPKeyboardManager.h"
#include "CommandBars/XTPCommandBar.h"
#include "CommandBars/XTPToolBar.h"
#include "CommandBars/XTPMenuBar.h"
#include "CommandBars/XTPPopupBar.h"

#include "XTPRibbonPopups.h"
#include "XTPRibbonBar.h"
#include "XTPRibbonSystemButton.h"
#include "XTPRibbonBackstageView.h"


IMPLEMENT_XTP_COMMANDBAR(CXTPRibbonBackstageView, CXTPCommandBar);

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int COMMAND_GAP = 6;
const int TEXT_GAP = 15;
const int IMAGE_MARGIN = 7;


CXTPRibbonBackstageView::CXTPRibbonBackstageView()
{
	m_nMenuWidth = 0;

	LOGFONT lf;
	XTPDrawHelpers()->GetIconLogFont(&lf);

	lf.lfHeight = -11;
	m_fntCommand.CreateFontIndirect(&lf);

	lf.lfHeight = -12;
	m_fntTab.CreateFontIndirect(&lf);

	m_pHighlightedControl = NULL;
	m_pFocusedControl = NULL;
	m_pActiveTab = NULL;

	m_pSystemButton = NULL;

}

CXTPRibbonBackstageView* AFX_CDECL CXTPRibbonBackstageView::CreateBackstageView(CXTPCommandBars* pCommandBars)
{
	CXTPRibbonBackstageView* pView = new CXTPRibbonBackstageView();
	pView->SetCommandBars(pCommandBars);

	return pView;
}

CXTPRibbonBackstageView::~CXTPRibbonBackstageView()
{

}

int CXTPRibbonBackstageView::IndexOf(CXTPControl* pControl) const
{
	if (!pControl)
		return -1;

	for (int i = 0; i < GetControlCount(); i++)
		if (GetControl(i) == pControl)
			return i;

	return -1;
}

CWnd* CXTPRibbonBackstageView::GetActiveControlPane() const
{
	CXTPRibbonBackstageTab* pControlCustom = DYNAMIC_DOWNCAST(CXTPRibbonBackstageTab, m_pActiveTab);

	if (pControlCustom)
	{
		CWnd* pWnd =  pControlCustom->GetControlPane();
		if (pWnd && !IsWindow(pWnd->m_hWnd))
			return NULL;

		return pWnd;
	}
	return NULL;
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

BOOL CXTPRibbonBackstageView::IsCommand(CXTPControl* pControl) const
{
	return DYNAMIC_DOWNCAST(CXTPRibbonBackstageCommand, pControl) != NULL;
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
	CXTPClientRect rc(this);

	CXTPBufferDC dc(dcPaint);

	dc.FillSolidRect(rc, RGB(255, 255, 255));

	dc.SetBkMode(TRANSPARENT);

	COLORREF clrText = XTPResourceImages()->GetImageColor(_T("Ribbon"), _T("BackstageMenuTextColor"), 0);
	dc.SetTextColor(clrText);


	CXTPResourceImage* pImage = XTPResourceImages()->LoadFile(_T("BackstageTopBorder"));
	if (pImage)
	{
		pImage->DrawImage(&dc, CRect(0, 0, rc.Width(), 2), pImage->GetSource());
	}

	pImage = XTPResourceImages()->LoadFile(_T("BackstageMenuBackground"));

	if (pImage)
	{
		pImage->DrawImage(&dc, CRect(0, 2, m_nMenuWidth, rc.bottom), pImage->GetSource());
	}


	for (int i = 0; i < GetControlCount(); i++)
	{
		dc.SetTextColor(clrText);
		DrawControl(&dc, GetControl(i));
	}
}

#define DT_HIDEPREFIX  0x00100000

void CXTPRibbonBackstageView::DrawControl(CDC* pDC, CXTPControl* pControl)
{
	if (!pControl->IsVisible())
		return;

	BOOL bEnabled = pControl->GetEnabled();

	COLORREF clrText = pDC->GetTextColor();
	if (!bEnabled)
		clrText = GetXtremeColor(XPCOLOR_GRAYTEXT);

	pDC->SetTextColor(m_pActiveTab == pControl ? RGB(255, 255, 255) : clrText);



	if (IsCommand(pControl))
	{
		if (m_pHighlightedControl == pControl || m_pFocusedControl == pControl)
		{
			CXTPResourceImage* pImage = XTPResourceImages()->LoadFile(_T("BackstageMenuCommand"));
			if (pImage)
			{
				pImage->DrawImage(pDC, pControl->GetRect(), pImage->GetSource(!pControl->GetEnabled() ? 2 : 0, 3), CRect(2, 2, 2, 2));
			}
		}

		CXTPFontDC font(pDC, &m_fntCommand);

		CRect rcText(pControl->GetRect());
		rcText.DeflateRect(TEXT_GAP, 2, 2, 2);

		CXTPImageManagerIcon* pIcon = GetImageManager()->GetImage(pControl->GetIconId());
		if (pIcon)
		{
			pIcon->Draw(pDC, CPoint(rcText.left, (rcText.top + rcText.bottom - pIcon->GetHeight()) / 2), pIcon->GetIcon(bEnabled ? xtpImageNormal : xtpImageDisabled));

			rcText.left += pIcon->GetWidth() + IMAGE_MARGIN;
		}

		pDC->DrawText(pControl->GetCaption(), rcText, DT_HIDEPREFIX | DT_VCENTER | DT_SINGLELINE);
	}
	else
	{
		if (m_pHighlightedControl == pControl || m_pActiveTab == pControl || m_pFocusedControl == pControl)
		{
			CXTPResourceImage* pImage = XTPResourceImages()->LoadFile(_T("BackstageMenuPage"));
			if (pImage)
			{
				pImage->DrawImage(pDC, pControl->GetRect(), pImage->GetSource(!pControl->GetEnabled() ? 3 : m_pActiveTab == pControl ? 1 : 0, 4), CRect(2, 2, 2, 2));
			}

			if (m_pActiveTab == pControl)
			{
				CXTPResourceImage* pImage = XTPResourceImages()->LoadFile(_T("BackstageMenuPageGlyph"));
				if (pImage)
				{
					CRect rcSrc(pImage->GetSource());
					CRect rc(pControl->GetRect());
					CRect rcGlyph(CPoint(rc.right - rcSrc.Width(), (rc.top + rc.bottom - rcSrc.Height()) / 2), rcSrc.Size());

					pImage->DrawImage(pDC, rcGlyph, rcSrc);
				}
			}

		}

		CXTPFontDC font(pDC, &m_fntTab);

		CRect rcText(pControl->GetRect());
		rcText.DeflateRect(TEXT_GAP + COMMAND_GAP, 2, 2, 2);

		CXTPImageManagerIcon* pIcon = GetImageManager()->GetImage(pControl->GetIconId());
		if (pIcon)
		{
			pIcon->Draw(pDC, CPoint(rcText.left, (rcText.top + rcText.bottom - pIcon->GetHeight()) / 2), pIcon->GetIcon(bEnabled ? xtpImageNormal : xtpImageDisabled));

			rcText.left += pIcon->GetWidth() + IMAGE_MARGIN;
		}


		pDC->DrawText(pControl->GetCaption(), rcText, DT_HIDEPREFIX | DT_VCENTER | DT_SINGLELINE);
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

	m_nMenuWidth = 132;
	int nCommandHeight = max(26, dc.GetTextExtent(_T("X"), 1).cy + 6);
	int nPageHeight = max(37, dc.GetTextExtent(_T("X"), 1).cy + 10);


	int i;

	for (i = 0; i < GetControlCount(); i++)
	{
		CXTPControl* pControl = GetControl(i);
		if (!pControl->IsVisible())
			continue;

		font.SetFont(IsCommand(pControl) ? &m_fntCommand : &m_fntTab);

		int nWidth = dc.GetTextExtent(pControl->GetCaption()).cx + COMMAND_GAP + TEXT_GAP + COMMAND_GAP + 6;

		CXTPImageManagerIcon* pIcon = GetImageManager()->GetImage(pControl->GetIconId());
		if (pIcon)
		{
			nWidth += pIcon->GetWidth() + IMAGE_MARGIN;
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
			CRect rc(COMMAND_GAP, top, m_nMenuWidth - COMMAND_GAP, top + nCommandHeight);

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

	CWnd* pWndOwner = GetOwnerSite();
	if (pWndOwner) pWndOwner->SendMessage(WM_XTP_INITCOMMANDSPOPUP, 0, (LPARAM)this);


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

//////////////////////////////////////////////////////////////////////////
//

IMPLEMENT_XTP_CONTROL(CXTPRibbonBackstageCommand, CXTPControl)

CXTPRibbonBackstageCommand::CXTPRibbonBackstageCommand()
{

}

IMPLEMENT_XTP_CONTROL(CXTPRibbonBackstageTab, CXTPControl)

CXTPRibbonBackstageTab::CXTPRibbonBackstageTab(HWND hwndControl)
{
	m_szControlPaneMinSize = CSize(0, 0);

	m_hwndControl = hwndControl;
}

CWnd* CXTPRibbonBackstageTab::GetControlPane() const
{
	return CWnd::FromHandle(m_hwndControl);
}

CXTPRibbonBackstageCommand* CXTPRibbonBackstageView::AddCommand(UINT nID)
{
	CXTPRibbonBackstageCommand* pControl = (CXTPRibbonBackstageCommand*)GetControls()->Add(new CXTPRibbonBackstageCommand(), nID);
	return pControl;
}

CXTPRibbonBackstageTab* CXTPRibbonBackstageView::AddTab(CWnd* pWnd, UINT nID)
{
	CXTPRibbonBackstageTab* pTab = (CXTPRibbonBackstageTab*)GetControls()->Add(new CXTPRibbonBackstageTab(pWnd->GetSafeHwnd()), nID);

	return pTab;
}




