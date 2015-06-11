// XTPCaption.cpp : implementation of the CXTPCaption class.
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

#include <Controls/Resource.h>
#include <Controls/Defines.h>
#include <Controls/Util/XTPControlTheme.h>
#include <Controls/Util/XTPUtil.h>
#include <Controls/Util/XTPGlobal.h>
#include <Controls/Button/XTPButton.h>
#include <Controls/Button/XTPButtonTheme.h>
#include <Controls/Button/Themes/XTPButtonThemeUltraFlat.h>
#include <Controls/Button/Themes/XTPButtonThemeOfficeXP.h>
#include <Controls/Button/Themes/XTPButtonThemeOffice2003.h>

#include <Controls/Static/XTPCaptionTheme.h>
#include <Controls/Static/Themes/XTPCaptionButtonThemeOfficeXP.h>
#include <Controls/Static/Themes/XTPCaptionButtonThemeOffice2003.h>

#include "XTPCaption.h"
#include "XTPCaptionPopupWnd.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CXTPCaptionButton

CXTPCaptionButton::CXTPCaptionButton()
: m_pCaption(NULL)
{

}

/////////////////////////////////////////////////////////////////////////////
// CXTPCaption

CXTPCaption::CXTPCaption()
: m_pTheme(NULL)
{
	m_bAppCaption = false;
	m_bUserColors = false;
	m_strCaption = _T("");
	m_hIcon = NULL;
	m_pChildWnd = NULL;
	m_pParentView = NULL;
	m_pSplitterWnd = NULL;
	m_pPopupWnd = NULL;
	m_nOffset = 0;
	m_nBorder = 0;
	m_sizeIcon.cx = __min(XTPAuxData().cxSmIcon, 16);
	m_sizeIcon.cy = __min(XTPAuxData().cySmIcon, 16);
	m_dwExStyle = 0L;
	m_clrText = ::GetSysColor(COLOR_BTNTEXT);
	m_clrBorder = ::GetSysColor(COLOR_3DFACE);
	m_clrFace = ::GetSysColor(COLOR_3DFACE);

	VERIFY(SetTheme(xtpControlThemeDefault));
}

CXTPCaption::~CXTPCaption()
{
	CMDTARGET_RELEASE(m_pTheme);
}

IMPLEMENT_DYNAMIC(CXTPCaption, CStatic)

BEGIN_MESSAGE_MAP(CXTPCaption, CStatic)
	//{{AFX_MSG_MAP(CXTPCaption)
	ON_WM_PAINT()
	ON_MESSAGE(WM_PRINTCLIENT, OnPrintClient)
	ON_WM_ERASEBKGND()
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_SYSCOLORCHANGE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP

	ON_BN_CLICKED(XTP_IDC_BTN_CLOSE, OnCaptButton)
	ON_MESSAGE_VOID(CPN_XTP_PUSHPINBUTTON, OnPushPinButton)
	ON_MESSAGE_VOID(CPN_XTP_PUSHPINCANCEL, OnPushPinCancel)
	ON_MESSAGE(WM_XTP_SETCONTROLTHEME, OnSetTheme)

END_MESSAGE_MAP()

void CXTPCaption::RefreshMetrics()
{
	if (m_pTheme)
		m_pTheme->RefreshMetrics(this);

	if (::IsWindow(m_hWnd))
		RedrawWindow();
}

BOOL CXTPCaptionButton::SetTheme(XTPControlTheme eTheme)
{
	CMDTARGET_RELEASE(m_pTheme);

	switch (eTheme)
	{
	case xtpControlThemeOfficeXP:
		m_pTheme = new CXTPCaptionButtonThemeOfficeXP();
		break;
	case xtpControlThemeOffice2003:
		m_pTheme = new CXTPCaptionButtonThemeOffice2003();
		break;
	default:
		m_pTheme = new CXTPCaptionButtonTheme();
	}


	RefreshMetrics();

	return (m_pTheme != NULL);
}

BOOL CXTPCaption::SetTheme(CXTPCaptionTheme* pTheme)
{
	CMDTARGET_RELEASE(m_pTheme);

	m_pTheme = pTheme;

	m_btnCaption.SetTheme(pTheme->GetThemeStyle());

	RefreshMetrics();

	return (m_pTheme != NULL);
}

BOOL CXTPCaption::SetTheme(XTPControlTheme eTheme)
{
	switch (eTheme)
	{
		case xtpControlThemeOfficeXP:
			return SetTheme(new CXTPCaptionThemeOfficeXP());

		case xtpControlThemeOffice2003:
			return SetTheme(new CXTPCaptionThemeOffice2003());
	}

	return SetTheme(new CXTPCaptionTheme());
}

LRESULT CXTPCaption::OnSetTheme(WPARAM wParam, LPARAM /*lParam*/)
{
	XTPControlTheme eTheme = (XTPControlTheme)wParam;

	SetTheme(eTheme);

	return 0;
}

BOOL CXTPCaption::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	if (message == WM_SETTINGCHANGE || message == WM_SYSCOLORCHANGE)
	{
		RefreshMetrics();
	}

	return CStatic::OnWndMsg(message, wParam, lParam, pResult);
}

void CXTPCaption::OnSize(UINT nType, int cx, int cy)
{
	CStatic::OnSize(nType, cx, cy);

	if (m_hIcon != NULL)
		RedrawWindow();
}

void CXTPCaption::OnPaint()
{
	// background is already filled in gray
	CPaintDC dc(this);

	// Get the client rect.
	CXTPClientRect rectClient(this);

	// Paint to a memory device context to help
	// eliminate screen flicker.
	CXTPBufferDC memDC(dc);

	// If the caption button is a valid window and visible exclude from painting...
	if (::IsWindow(m_btnCaption.m_hWnd) && m_btnCaption.IsWindowVisible())
	{
		CXTPWindowRect rcButton(&m_btnCaption);
		ScreenToClient(&rcButton);
		memDC.ExcludeClipRect(&rcButton);
	}

	// draw the background, text and icon.
	DrawCaptionBack(&memDC, rectClient);
	DrawCaptionText(&memDC);
	DrawCaptionIcon(&memDC, rectClient);
}

LRESULT CXTPCaption::OnPrintClient(WPARAM wParam, LPARAM /*lParam*/)
{
	CDC* pDC = CDC::FromHandle((HDC)wParam);
	if (pDC)
	{
		CXTPClientRect rectClient(this);

		DrawCaptionBack(pDC, rectClient);
		DrawCaptionText(pDC);
		DrawCaptionIcon(pDC, rectClient);
	}
	return 1;
}

BOOL CXTPCaption::OnEraseBkgnd(CDC* /*pDC*/)
{
	return TRUE;
}

void CXTPCaption::DrawCaptionBack(CDC* pDC, CRect& rcItem)
{
	GetTheme()->DrawCaptionBack(pDC, this, rcItem);
}

void CXTPCaption::DrawCaptionText(CDC* pDC)
{
	GetTheme()->DrawCaptionText(pDC, this);
}

void CXTPCaption::DrawCaptionIcon(CDC* pDC, CRect& rcItem)
{
	GetTheme()->DrawCaptionIcon(pDC, this, rcItem);
}

BOOL CXTPCaption::Create(CWnd* pParentWnd, LPCTSTR lpszWindowName, DWORD dwExStyle, DWORD dwStyle, const CRect& rect, UINT nID)
{
	// Let the base class create the control.
	if (!CStatic::Create(NULL, dwStyle | WS_CLIPCHILDREN, rect, pParentWnd, nID))
	{
		TRACE(_T("Unable to create caption.\n"));
		return FALSE;
	}

	SetFont(&XTPAuxData().font);

	// save the style.
	m_dwExStyle = (dwExStyle & (CPWS_EX_GROOVE_EDGE | CPWS_EX_RAISED_EDGE | CPWS_EX_CLOSEBUTTON));
	ModifyStyleEx(0, (dwExStyle & ~m_dwExStyle));

	// Save the window text.
	m_strCaption = lpszWindowName;

	if (HasCloseButton())
	{
		// Create the caption's close button.
		if (!m_btnCaption.Create(NULL, WS_VISIBLE | WS_CHILD | BS_ICON | BS_CENTER | BS_VCENTER,
			CRect(0, 0, 0, 0), this, XTP_IDC_BTN_CLOSE))
		{
			TRACE0("Unable to create caption button.\n");
			return -1;
		}

		// Create the image list used by frame buttons.
		m_ilButton.Create (XTP_IDB_BTN_OUT, 16, 1, RGB(255, 0, 255));

		// and set the icon for the caption's close button
		m_btnCaption.SetIcon(CSize(16, 15), m_ilButton.ExtractIcon(
			(XTPColorManager()->GetCurrentSystemTheme() == xtpSystemThemeAero)? 3: 2));
	}
	else
	{
		// Create the button to be used with child window.
		if (!m_btnCaption.Create(m_strCaption, BS_ICON | BS_VCENTER,
			CRect(0, 0, 0, 0), this, XTP_IDC_BTN_CLOSE))
		{
			TRACE0("Unable to create caption button.\n");
			return FALSE;
		}

		m_bAppCaption = true;
	}


	m_btnCaption.SetCaption(this);

	SetWindowPos(NULL, 0, 0, 0, 0,
		SWP_NOSIZE | SWP_NOMOVE | SWP_FRAMECHANGED);

	return TRUE;
}

void CXTPCaption::ModifyCaptionStyle(int nBorderSize, CFont* pFont, LPCTSTR lpszWindText, HICON hIcon)
{
	ASSERT(::IsWindow(m_hWnd));

	if (nBorderSize != 0)
	{
		m_nBorder = nBorderSize;
	}

	if (pFont != NULL)
	{
		SetFont(pFont);

		// button font should match caption
		if (::IsWindow(m_btnCaption.m_hWnd))
			m_btnCaption.SetFont(GetFont());
	}

	if (lpszWindText != NULL)
	{
		m_strCaption = lpszWindText;
	}

	if (hIcon != NULL)
	{
		m_hIcon = hIcon;
	}

	Invalidate();
}

void CXTPCaption::SetChildWindow(CWnd* pChild, CWnd* pNotifyWnd)
{
	ASSERT_VALID (pChild);      // must be valid.
	ASSERT_VALID (pNotifyWnd);  // must be valid.

	// save window pointers, and show caption button.
	m_pChildWnd = pChild;
	SetOwner(pNotifyWnd);

	m_pParentView = pChild->GetParent();
	ASSERT_KINDOF(CView, m_pParentView);

	m_pSplitterWnd = m_pParentView->GetParent();
	ASSERT_KINDOF(CSplitterWnd, m_pSplitterWnd);

	// get the size of the child and parent windows.
	m_pChildWnd->GetClientRect (&m_rcChild);
	m_pParentView->GetClientRect (&m_rcParent);
	m_pSplitterWnd->GetClientRect (&m_rcSplitter);

	// save the size of the childs caption area.
	m_nOffset = (m_rcParent.Height() - m_rcChild.Height());

	// make the caption button visible.
	m_btnCaption.ShowWindow(SW_SHOW);
	m_btnCaption.UpdateWindow();

	// inflate the size of the parent to add a border, this will
	// also be the size of the popup window.
	int cx = ::GetSystemMetrics (SM_CXEDGE)*2;
	int cy = ::GetSystemMetrics (SM_CYEDGE)*2;
	m_rcParent.InflateRect(cx, cy);
}

void CXTPCaption::KillChildWindow()
{
	// Hide the caption button.
	if (::IsWindow(m_btnCaption.m_hWnd))
	{
		m_btnCaption.ShowWindow(SW_HIDE);
		m_btnCaption.SetState(FALSE);
	}

	// Destroy the pop-up window.
	if (::IsWindow(m_pPopupWnd->GetSafeHwnd()))
	{
		m_pPopupWnd->DestroyWindow();
	}

	SAFE_DELETE(m_pPopupWnd);
}

BOOL CXTPCaption::ShowPopupWindow()
{
	// already visible return TRUE.
	if (::IsWindowVisible(m_pPopupWnd->GetSafeHwnd()))
		return TRUE;

	// if not a window yet create it.
	if (!::IsWindow(m_pPopupWnd->GetSafeHwnd()))
	{
		m_pPopupWnd = new CXTPCaptionPopupWnd;

		if (!m_pPopupWnd->Create(CRect(0, 0, 0, 0), this, m_pChildWnd))
		{
			SAFE_DELETE(m_pPopupWnd);
			return FALSE;
		}
	}

	// view pointer is not valid.
	if (!::IsWindow(m_pParentView->GetSafeHwnd()))
		return FALSE;

	// splitter pointer is not valid.
	if (!::IsWindow(m_pSplitterWnd->GetSafeHwnd()))
		return FALSE;

	CRect rcPopup(m_rcParent);
	m_pParentView->ClientToScreen(&rcPopup);

	// get the current size of the splitter window.
	CRect rcSplitter;
	m_pSplitterWnd->GetClientRect(&rcSplitter);

	// check to see if the size has changed.
	if (rcSplitter.Height() > m_rcSplitter.Height())
	{
		rcPopup.bottom += (rcSplitter.Height() - m_rcSplitter.Height());
	}
	else if (m_rcSplitter.Height() > rcSplitter.Height())
	{
		rcPopup.bottom -= (m_rcSplitter.Height() - rcSplitter.Height());
	}

	// display the popup window.
	m_pPopupWnd->MoveWindow(&rcPopup);
	m_pPopupWnd->ShowWindow(SW_SHOW);
	m_pPopupWnd->RecalcLayout();

	m_pPopupWnd->SetTheme(m_pTheme->GetThemeStyle());

	// press the caption button.
	m_btnCaption.SetState(TRUE);

	return TRUE;
}

void CXTPCaption::OnCaptButton()
{
	if (HasCloseButton())
	{
		CWnd* pOwner = GetOwner();
		ASSERT_VALID(pOwner);

		if (pOwner)
		{
			// Notify of selection change.
			pOwner->SendMessage(WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(),
				BN_CLICKED), (LPARAM)m_hWnd);
		}
	}
	else
	{
		ShowPopupWindow();
	}
}

void CXTPCaption::OnPushPinButton()
{
	KillChildWindow();

	// get the current size of the child's parent window and
	// adjust the size of the child accordingly.
	m_pParentView->GetClientRect (&m_rcParent);
	m_rcChild = m_rcParent;
	m_rcChild.top += m_nOffset;

	// move the child window to its correct location.
	m_pChildWnd->MoveWindow (m_rcChild);

	CWnd* pNotifyWnd = GetOwner();
	ASSERT_VALID(pNotifyWnd);

	if (::IsWindow(pNotifyWnd->GetSafeHwnd()))
	{
		// Notify that push pin button was pressed.
		pNotifyWnd->SendMessage(CPN_XTP_PUSHPINBUTTON);
		Invalidate();
	}
}

void CXTPCaption::OnPushPinCancel()
{
	m_btnCaption.SetState(FALSE);
	m_pPopupWnd->DestroyWindow();

	SAFE_DELETE (m_pPopupWnd);

	CWnd* pNotifyWnd = GetOwner();
	ASSERT_VALID(pNotifyWnd);

	if (::IsWindow(pNotifyWnd->GetSafeHwnd()))
	{
		// Notify that popup window has lost input focus.
		pNotifyWnd->SendMessage(CPN_XTP_PUSHPINCANCEL);
		Invalidate();
	}
}

void CXTPCaption::UpdateCaption(LPCTSTR lpszWindowText, HICON hIcon)
{
	ASSERT(::IsWindow(m_hWnd));

	CRect rcClient;
	GetClientRect(&rcClient);
	CDC* pDC = GetDC();

	DrawCaptionBack(pDC, rcClient);

	if (lpszWindowText != NULL)
	{
		m_strCaption = lpszWindowText;
		UpdateCaptionText(pDC);
	}

	if (hIcon != NULL)
	{
		m_hIcon = hIcon;
		DrawCaptionIcon(pDC, rcClient);
	}

	ReleaseDC(pDC);
}

void CXTPCaption::UpdateCaptionText(CDC* pDC)
{
	if (::IsWindow(m_btnCaption.m_hWnd))
	{
		m_btnCaption.SetWindowText(m_strCaption);
		m_btnCaption.MoveWindow(GetButtonRect());
	}

	DrawCaptionText(pDC);
}

CRect CXTPCaption::GetTextRect() const
{
	if (m_strCaption.IsEmpty())
		return CXTPEmptyRect();

	CXTPClientRect rcClient(this);

	// Use a NULL window dc to get the size the caption button
	// should be using CDC::GetTextExtent(...).
	CWindowDC dc(NULL);
	CXTPFontDC fontDC(&dc, GetFont());

	TEXTMETRIC tm;
	dc.GetTextMetrics(&tm);

	// Calculate the button area size.
	CSize size = dc.GetTextExtent(m_strCaption, m_strCaption.GetLength());
	size.cx += tm.tmAveCharWidth + 12;
	size.cy = rcClient.Height();

	int iWidth = rcClient.Width() - (m_hIcon ? 18 : 0);
	if (size.cx > iWidth)
		size.cx = iWidth;

	return CRect(m_nBorder, m_nBorder, m_nBorder + size.cx, size.cy - m_nBorder);
}

CRect CXTPCaption::GetButtonRect() const
{
	if (!::IsWindow(m_btnCaption.m_hWnd))
		return CXTPEmptyRect();

	if (!HasCloseButton())
		return GetTextRect();

	CSize szIcon(16, 15);
	CXTPClientRect rcClient(this);

	CRect rcButton;
	rcButton.top = (rcClient.Height()-szIcon.cy)/2;
	rcButton.bottom = rcButton.top+szIcon.cy;

	// ensure offset is the same for top and right edge.
	int x = rcButton.top-rcClient.top;

	rcButton.left = (rcClient.Width()-szIcon.cx)-x;
	rcButton.right = rcButton.left+szIcon.cx;

	return rcButton;
}

void CXTPCaption::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos)
{
	CStatic::OnWindowPosChanged(lpwndpos);

	if (::IsWindow(m_btnCaption.m_hWnd))
		m_btnCaption.MoveWindow(GetButtonRect());
}

void CXTPCaption::SetCaptionColors(COLORREF clrBorder, COLORREF clrFace, COLORREF clrText)
{
	ASSERT(::IsWindow(m_hWnd));

	m_bUserColors = true;

	m_clrBorder = clrBorder;
	m_clrFace = clrFace;
	m_clrText = clrText;

	if (::IsWindow(m_btnCaption.m_hWnd))
	{
//      m_btnCaption.SetColorFace(m_clrFace);
//      m_btnCaption.SetColorText(m_clrText);
//      m_btnCaption.SetColorShadow(::GetSysColor(COLOR_3DDKSHADOW));
	}
}

void CXTPCaption::SetOffice2003Colors(bool b2003Colors/*= true*/)
{
	m_bUserColors = b2003Colors;
	SetTheme(b2003Colors ? xtpControlThemeOffice2003 : xtpControlThemeDefault);
}

void CXTPCaption::OnSysColorChange()
{
	CStatic::OnSysColorChange();

	// update caption colors if not user defined.
	if (m_bUserColors == false)
	{
		m_clrText = ::GetSysColor(COLOR_BTNTEXT);
		m_clrBorder = ::GetSysColor(COLOR_3DFACE);
		m_clrFace = ::GetSysColor(COLOR_3DFACE);
	}
}
