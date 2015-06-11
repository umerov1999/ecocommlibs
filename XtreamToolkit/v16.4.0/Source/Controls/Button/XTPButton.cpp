// XTButton.cpp : implementation of the CXTPButton class.
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
#include <Common/XTPImageManager.h>
#include <Common/XTPMarkupRender.h>
#include <Common/XTPToolTipContext.h>
#include <Common/XTPMaskEditT.h>
#include <Common/XTPWinThemeWrapper.h>
#include <Common/XTPColorManager.h>

#include <Controls/Util/XTPControlTheme.h>

#include <Controls/Defines.h>
#include <Controls/Button/XTPButton.h>
#include <Controls/Button/XTPButtonTheme.h>
#include <Controls/Edit/XTPEdit.h>

#include <Controls/Button/Themes/XTPButtonThemeFlat.h>
#include <Controls/Button/Themes/XTPButtonThemeUltraFlat.h>
#include <Controls/Button/Themes/XTPButtonThemeOffice2000.h>
#include <Controls/Button/Themes/XTPButtonThemeOfficeXP.h>
#include <Controls/Button/Themes/XTPButtonThemeOffice2003.h>
#include <Controls/Button/Themes/XTPButtonThemeResource.h>
#include <Controls/Button/Themes/XTPButtonThemeOffice2013.h>
#include <Controls/Button/Themes/XTPButtonThemeVisualStudio2012.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifndef WM_UPDATEUISTATE
#define WM_UPDATEUISTATE    0x0128
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTPButton
/////////////////////////////////////////////////////////////////////////////

#ifndef _XTP_ACTIVEX_BUTTON
IMPLEMENT_DYNAMIC(CXTPButton, CXTPButtonBase)
#endif

CXTPButton::CXTPButton()
: m_pTheme(NULL)
{
	m_bHot = FALSE;
	m_bPushed = FALSE;

	m_bChecked = FALSE;

	m_bFlatStyle = FALSE;
	m_bUseVisualStyle = TRUE;

	m_bEnableMarkup = FALSE;
	m_pMarkupContext = NULL;
	m_pUIElement = NULL;

	m_nPushButtonStyle = 0;

	m_nBorderGap = 4;
	m_nImageGap = 3;

	m_pIcon = 0;

	m_nImageAlignment = BS_VCENTER | BS_CENTER;
	m_nTextImageRelation = xtpButtonImageBeforeText;

	m_nBorderStyle = 0;
	m_bShowFocus = TRUE;

	m_hWndBuddy = 0;

	m_pToolTipContext = new CXTPToolTipContext();

	VERIFY(SetTheme(xtpControlThemeDefault));
}

CXTPButton::~CXTPButton()
{
	CMDTARGET_RELEASE(m_pTheme);

	XTPMarkupReleaseElement(m_pUIElement);
	XTPMarkupReleaseContext(m_pMarkupContext);

	CMDTARGET_RELEASE(m_pIcon);
	CMDTARGET_RELEASE(m_pToolTipContext);
}

BEGIN_MESSAGE_MAP(CXTPButton, CXTPButtonBase)
	//{{AFX_MSG_MAP(CXTPButton)
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_MESSAGE(WM_PRINTCLIENT, OnPrintClient)
	ON_WM_CHAR()
	ON_WM_KEYDOWN()
	ON_MESSAGE(WM_GETDLGCODE, OnGetDlgCode)
	//}}AFX_MSG_MAP
#ifndef _XTP_ACTIVEX_BUTTON
	ON_WM_PAINT()
#endif
	ON_MESSAGE_VOID(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(BM_SETSTYLE, OnDefaultAndInvalidate)
	ON_MESSAGE(BM_SETCHECK, OnDefaultAndInvalidate)
	ON_MESSAGE(WM_CAPTURECHANGED, OnCaptureChanged)
	ON_MESSAGE_VOID(WM_ENABLE, OnInvalidate)
	ON_MESSAGE(WM_UPDATEUISTATE, OnUpdateUIState)
	ON_MESSAGE(BM_SETSTATE, OnSetState)
	ON_MESSAGE(WM_SETTEXT, OnSetText)
	ON_MESSAGE(WM_XTP_SETCONTROLTHEME, OnSetTheme)
END_MESSAGE_MAP()

void CXTPButton::OnMouseMove(UINT nFlags, CPoint point)
{
	if (GetButtonStyle() == BS_GROUPBOX)
		return;

	CXTPButtonBase::OnMouseMove(nFlags, point);

	CRect rc = GetButtonRect();

	BOOL bHot = rc.PtInRect(point);
	if (bHot != m_bHot)
	{
		m_bHot = bHot;
		RedrawButton();

		if (m_bHot)
		{
			TRACKMOUSEEVENT tme = {sizeof(TRACKMOUSEEVENT), TME_LEAVE, m_hWnd, HOVER_DEFAULT};
			_TrackMouseEvent(&tme);
		}
	}
}

void CXTPButton::OnMouseLeave()
{
	OnMouseMove(0, CPoint(-1, -1));
}

void CXTPButton::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_nPushButtonStyle == xtpButtonSplitDropDown)
	{
		CRect rcDropDown(GetButtonRect());
		rcDropDown.left = rcDropDown.right - 16 - GetBorderGap();

		if (rcDropDown.PtInRect(point))
		{
			DoDropDown();
			return;
		}
	}

	CXTPButtonBase::OnLButtonDown(nFlags, point);
}

void CXTPButton::OnLButtonUp(UINT nFlags, CPoint point)
{
	CXTPButtonBase::OnLButtonUp(nFlags, point);
}

LRESULT CXTPButton::OnSetState(WPARAM wParam, LPARAM /*lParam*/)
{
	BOOL bVisible = GetStyle() & WS_VISIBLE;
	if (bVisible) ModifyStyle(WS_VISIBLE, 0);
	Default();
	if (bVisible) ModifyStyle(0, WS_VISIBLE);

	if (m_bPushed == 2)
		return 0;

	m_bPushed = (wParam != 0);
	RedrawButton();

	if (m_bPushed && IsDropDownStyle() && IsPushButton() && (m_nPushButtonStyle != xtpButtonSplitDropDown))
	{
		DoDropDown();
	}

	return 0;
}

CRect CXTPButton::GetButtonRect()
{
	CXTPClientRect rc(this);

	if (m_hWndBuddy)
	{
		CXTPEdit* pEditBuddy = DYNAMIC_DOWNCAST(CXTPEdit, CWnd::FromHandlePermanent(m_hWndBuddy));
		if (pEditBuddy) pEditBuddy->AdjustBuddyRect(this, rc, UDS_ALIGNRIGHT);
	}

	return rc;
}

void CXTPButton::DoDropDown()
{
	m_bPushed = 2;
	RedrawButton();

	OnDropDown();

	m_bPushed = 0;
	RedrawButton();
}

void CXTPButton::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (IsDropDownStyle() && (nChar == VK_SPACE))
	{
		DoDropDown();
		return;
	}

	if ((m_nPushButtonStyle == xtpButtonDropDown || m_nPushButtonStyle == xtpButtonDropDownRight) && (nChar == VK_RETURN))
	{
		DoDropDown();
		return;
	}

	CXTPButtonBase::OnChar(nChar, nRepCnt, nFlags);
}

LRESULT CXTPButton::OnGetDlgCode(WPARAM wParam, LPARAM lParam)
{
	LRESULT lCode = CXTPButtonBase::OnGetDlgCode();

	if (IsDropDownStyle() && lParam && ((LPMSG)lParam)->message == WM_CHAR && wParam == VK_SPACE)
	{
		lCode |= DLGC_WANTCHARS;
	}

	if ((m_nPushButtonStyle == xtpButtonDropDown || m_nPushButtonStyle == xtpButtonDropDownRight)
		&& lParam && (((LPMSG)lParam)->message == WM_KEYDOWN || ((LPMSG)lParam)->message == WM_CHAR) && wParam == VK_RETURN)
	{
		lCode |= DLGC_WANTALLKEYS;
	}

	return lCode;
}

void CXTPButton::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_SPACE && IsDropDownStyle())
		return;

	if (nChar == VK_RETURN && (m_nPushButtonStyle == xtpButtonDropDown || m_nPushButtonStyle == xtpButtonDropDownNoGlyph || m_nPushButtonStyle == xtpButtonDropDownRight))
	{
		return;
	}

	CXTPButtonBase::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CXTPButton::OnDropDown()
{
	GetParent()->SendMessage(WM_COMMAND,
		(WPARAM)MAKELONG(::GetDlgCtrlID(GetSafeHwnd()), CBN_DROPDOWN),
		(LPARAM)GetSafeHwnd());
}

void CXTPButton::OnSetFocus(CWnd* pOldWnd)
{
	CXTPButtonBase::OnSetFocus(pOldWnd);
	RedrawButton(TRUE);
}

void CXTPButton::OnKillFocus(CWnd* pNewWnd)
{
	CXTPButtonBase::OnKillFocus(pNewWnd);
	RedrawButton(TRUE);
}

LRESULT CXTPButton::OnCaptureChanged(WPARAM wParam, LPARAM lParam)
{
	return OnDefaultAndInvalidate(wParam, lParam);
}

LRESULT CXTPButton::OnDefaultAndInvalidate(WPARAM, LPARAM)
{
	LRESULT lResult = Default();
	RedrawButton(TRUE);

	return lResult;
}

void CXTPButton::OnInvalidate()
{
	RedrawButton(TRUE);
}

LRESULT CXTPButton::OnPrintClient(WPARAM wParam, LPARAM lParam)
{
	if ((lParam & PRF_CLIENT) == 0)
		return Default();

	CDC* pDC = CDC::FromHandle((HDC)wParam);
	if (pDC) OnDraw(pDC);
	return 1;
}

void CXTPButton::OnPaint()
{
	CPaintDC dcPaint(this);
	OnDraw(&dcPaint);
}

void CXTPButton::OnDraw(CDC* pDC)
{
	if ((GetButtonStyle() == BS_GROUPBOX))
	{
		CFont* pOldFont = pDC->SelectObject(CWnd::GetFont());
		pDC->SetBkColor(GetSysColor(COLOR_3DFACE));
		pDC->SetBkMode(TRANSPARENT);

		DoDraw(pDC);

		pDC->SelectObject(pOldFont);
	}
	else
	{
		CXTPClientRect rc(this);
		CXTPBufferDC memDC(*pDC, rc);

		HBRUSH hBrush = (HBRUSH)GetParent()->SendMessage(WM_CTLCOLORBTN, (WPARAM)memDC.GetSafeHdc(), (LPARAM)GetSafeHwnd());
		if (hBrush)
		{
			::FillRect(memDC.GetSafeHdc(), rc, hBrush);
		}
		else
		{
			memDC.FillSolidRect(rc, GetSysColor(COLOR_3DFACE));
		}

		CFont* pOldFont = memDC.SelectObject(CWnd::GetFont());
		memDC.SetBkMode(TRANSPARENT);

		DoDraw(&memDC);

		memDC.SelectObject(pOldFont);
	}
}

void CXTPButton::DoDraw(CDC* pDC)
{
	if (m_hWndBuddy)
	{
		CXTPEdit* pEditBuddy = DYNAMIC_DOWNCAST(CXTPEdit, CWnd::FromHandlePermanent(m_hWndBuddy));
		if (pEditBuddy) pEditBuddy->DrawBuddyBorders(this, pDC, CXTPClientRect(this), UDS_ALIGNRIGHT);
	}

	m_pTheme->DrawButton(pDC, this);
}

LRESULT CXTPButton::OnUpdateUIState(WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult = ::DefWindowProc(m_hWnd, WM_UPDATEUISTATE, wParam, lParam);
	RedrawButton();

	return lResult;
}

LRESULT CXTPButton::OnSetText(WPARAM wParam, LPARAM lParam)
{
	BOOL bVisible = GetStyle() & WS_VISIBLE;
	if (bVisible) ModifyStyle(WS_VISIBLE, 0);

	LRESULT lResult = DefWindowProc(WM_SETTEXT, wParam, lParam);

	if (bVisible) ModifyStyle(0, WS_VISIBLE);


	InternalTextChanged();
	RedrawButton();

	return lResult;
}

int CXTPButton::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	return CXTPButtonBase::OnCreate(lpCreateStruct);
}

BOOL CXTPButton::OnEraseBkgnd(CDC* /*pDC*/)
{
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////

COLORREF CXTPButton::GetButtonTextColor()
{
	return GetSysColor(COLOR_BTNTEXT);
}

COLORREF CXTPButton::GetButtonBackColor()
{
	return GetSysColor(COLOR_BTNFACE);
}

BYTE CXTPButton::GetButtonStyle() const
{
	BYTE bStyle = BYTE(GetStyle() & 0xF);
	return bStyle;
}


CSize CXTPButton::GetImageSize()
{
	if (m_pIcon)
		return CSize(m_pIcon->GetWidth(), m_pIcon->GetHeight());

	return CSize(0, 0);
}

void CXTPButton::DrawImage(CDC* pDC, CRect rc)
{
	CXTPImageManagerIcon* pIcon = GetIcon();
	if (pIcon)
	{
		BOOL  bSelected = GetChecked() || IsPushed();

		if (!IsWindowEnabled())
		{
			pIcon->Draw(pDC, rc.TopLeft(), pIcon->GetDisabledIcon(), rc.Size());
		}
		else if (IsHighlighted() || bSelected)
		{
			pIcon->Draw(pDC, rc.TopLeft(), GetChecked() ? pIcon->GetCheckedIcon() :
				IsPushed() ? pIcon->GetPressedIcon() : pIcon->GetHotIcon(), rc.Size());
		}
		else
		{
			pIcon->Draw(pDC, rc.TopLeft(), pIcon->GetIcon(), rc.Size());
		}
	}
}

BOOL CXTPButton::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	if (message == WM_SETTINGCHANGE || message == WM_SYSCOLORCHANGE)
	{
		RefreshMetrics();
	}

	if (m_pUIElement)
	{
		if (XTPMarkupRelayMessage(m_pUIElement, message, wParam, lParam, pResult))
			return TRUE;
	}

	if (m_pToolTipContext)
	{
		m_pToolTipContext->FilterToolTipMessage(this, message, wParam, lParam);
	}

	return CXTPButtonBase::OnWndMsg(message, wParam, lParam, pResult);
}

int CXTPButton::GetTextAlignment() const
{
	DWORD bHorz;
	DWORD bVert;

	DWORD dwStyle = GetStyle();

	bHorz = dwStyle & (BS_LEFT | BS_RIGHT | BS_CENTER);
	bVert = dwStyle & (BS_TOP | BS_BOTTOM | BS_VCENTER);

	if (!bHorz || !bVert)
	{
		if (IsPushButton())
		{
			if (!bHorz)
				bHorz = BS_CENTER;
		}
		else
		{
			if (!bHorz)
				bHorz = BS_LEFT;
		}

		if (!bVert)
			bVert = BS_VCENTER;
	}

	return bHorz | bVert;
}

CSize CXTPButton::GetGlyphSize(BOOL bCheckBox)
{
	CRect rcButton = GetButtonRect();
	CSize sz(13, 13);

	if (m_bUseVisualStyle && m_pTheme->m_themeButton->IsAppThemed())
	{
		m_pTheme->m_themeButton->GetThemePartSize(NULL,
			bCheckBox ? BP_CHECKBOX : BP_RADIOBUTTON,
			1, rcButton, TS_TRUE, &sz);
	}

	return sz;
}

void CXTPButton::InternalTextChanged()
{
	if (m_pMarkupContext)
	{
		XTPMarkupReleaseElement(m_pUIElement);
		m_pUIElement = XTPMarkupParseText(m_pMarkupContext, GetButtonText());
	}
}

CString CXTPButton::GetButtonText()
{
	CString str;
	if (m_hWnd) GetWindowText(str);
	return str;
}

void CXTPButton::EnableMarkup(BOOL bEnableMarkup)
{
	m_bEnableMarkup = bEnableMarkup;

	XTPMarkupReleaseElement(m_pUIElement);
	XTPMarkupReleaseContext(m_pMarkupContext);

	if (m_bEnableMarkup)
	{
		m_pMarkupContext = XTPMarkupCreateContext(m_hWnd);
	}

	InternalTextChanged();
}

BOOL CXTPButton::IsPushButton() const
{
	DWORD dwStyle = GetStyle();

	switch (GetButtonStyle())
	{
		case LOBYTE(BS_PUSHBUTTON):
		case LOBYTE(BS_DEFPUSHBUTTON):
		case LOBYTE(BS_OWNERDRAW):
			return TRUE;

		default:
			if ((dwStyle & BS_PUSHLIKE) == BS_PUSHLIKE)
				return TRUE;
			break;
	}

	return FALSE;
}

void CXTPButton::SetTheme(CXTPButtonTheme *pPaintManager)
{
	CMDTARGET_RELEASE(m_pTheme);
	m_pTheme = pPaintManager;

	m_bUseVisualStyle = pPaintManager->GetThemeStyle() == xtpControlThemeDefault;

	if (m_pTheme)
	{
		m_pTheme->RefreshMetrics(this);
	}

	if (::IsWindow(m_hWnd))
	{
		RedrawButton();
	}
}

LRESULT CXTPButton::OnSetTheme(WPARAM wParam, LPARAM /*lParam*/)
{
	XTPControlTheme eTheme = (XTPControlTheme)wParam;

	SetTheme(eTheme);

	return 0;
}

void CXTPButton::RefreshMetrics()
{
	if (m_pTheme)
		m_pTheme->RefreshMetrics(this);

	if (::IsWindow(m_hWnd))
		RedrawButton();
}

BOOL CXTPButton::SetTheme(XTPControlTheme eTheme)
{
	CMDTARGET_RELEASE(m_pTheme);

	switch (eTheme)
	{
	case xtpControlThemeFlat:
		m_pTheme = new CXTPButtonThemeFlat();
		break;
	case xtpControlThemeUltraFlat:
		m_pTheme = new CXTPButtonThemeUltraFlat();
		break;
	case xtpControlThemeOffice2000:
		m_pTheme = new CXTPButtonThemeOffice2000();
		break;
	case xtpControlThemeOfficeXP:
		m_pTheme = new CXTPButtonThemeOfficeXP();
		break;
	case xtpControlThemeOffice2003:
		m_pTheme = new CXTPButtonThemeOffice2003();
		break;
	case xtpControlThemeResource:
		m_pTheme = new CXTPButtonThemeResource();
		break;
	case xtpControlThemeOffice2013:
		m_pTheme = new CXTPButtonThemeOffice2013();
		break;
	case xtpControlThemeVisualStudio2012Light:
		m_pTheme = new CXTPButtonThemeVisualStudio2012(TRUE);
		break;
	case xtpControlThemeVisualStudio2012Dark:
		m_pTheme = new CXTPButtonThemeVisualStudio2012(FALSE);
		break;
	default:
		m_pTheme = new CXTPButtonTheme();
	}

	m_bUseVisualStyle = (eTheme == xtpControlThemeDefault);

	RefreshMetrics();

	return (m_pTheme != NULL);
}

void CXTPButton::SetUseVisualStyle(BOOL bUseVisualStyle/* = TRUE*/)
{
	m_bUseVisualStyle = bUseVisualStyle;
	m_pTheme->RefreshMetrics(this);
	RedrawButton();
}


BOOL CXTPButton::GetChecked()
{
	if (IsPushButton() || !m_hWnd)
	{
		return m_bChecked;
	}

	return (BOOL)::SendMessage(m_hWnd, BM_GETCHECK, 0, 0);
}

void CXTPButton::SetChecked(BOOL bChecked)
{
	if (IsPushButton() || !m_hWnd)
	{
		m_bChecked = bChecked;
		RedrawButton();
	}
	else
	{
		::SendMessage(m_hWnd, BM_SETCHECK, bChecked, 0);
	}
}

BOOL CXTPButton::SetIcon(CSize size, UINT nID, UINT nHotID/*= 0*/, BOOL bRedraw/*= TRUE*/)
{
	return CXTPButton::SetIcon(
		size, MAKEINTRESOURCE(nID), MAKEINTRESOURCE(nHotID), bRedraw);
}

BOOL CXTPButton::SetIcon(CSize size, LPCTSTR lpszID, LPCTSTR lpszHotID/*= NULL*/, BOOL bRedraw/*= TRUE*/)
{
	CXTPImageManagerIconHandle hIcon;
	CXTPImageManagerIconHandle hIconHot;

	hIcon.CreateIconFromResource(lpszID, size);

	// Return false if the icon handle is NULL.
	if (hIcon.IsEmpty())
	{
		TRACE0("Failed to load Icon resource.\n");
		return FALSE;
	}

	// If we are using a pushed image as well...
	if (lpszHotID)
	{
		hIconHot.CreateIconFromResource(lpszHotID, size);

		// Return false if the icon handle is NULL.
		if (hIconHot.IsEmpty())
		{
			TRACE0("Failed to load Icon resource.\n");
			return FALSE;
		}
	}

	return CXTPButton::SetIcon(size, hIcon, hIconHot, bRedraw);
}

BOOL CXTPButton::SetIcon(CSize /*size*/, CXTPImageManagerIcon* pIcon, BOOL bRedraw/*= TRUE*/)
{
	// Construct the icon manager.
	CMDTARGET_RELEASE(m_pIcon);

	m_pIcon = pIcon;

	// Redraw the button.
	if (bRedraw)
		RedrawButton();

	return TRUE;
}

BOOL CXTPButton::SetIcon(CSize size, const CXTPImageManagerIconHandle& hIcon, const CXTPImageManagerIconHandle& hIconHot, BOOL bRedraw/*= TRUE*/)
{
	ASSERT(!hIcon.IsEmpty());

	// Save the image size and set the icon
	// handles to NULL.
	size = size != CSize(0) ? size : hIcon.GetExtent();

	CXTPImageManagerIcon* pIcon = new CXTPImageManagerIcon(0, size.cx, size.cy);
	pIcon->SetIcon(hIcon);

	if (!hIconHot.IsEmpty())
		pIcon->SetHotIcon(hIconHot);

	return SetIcon(size, pIcon, bRedraw);
}


BOOL CXTPButton::SetIcon(CSize size, HICON hIcon, HICON hIconHot/*= NULL*/, BOOL bRedraw/*= TRUE*/)
{
	return SetIcon(size, CXTPImageManagerIconHandle(hIcon), CXTPImageManagerIconHandle(hIconHot), bRedraw);
}

BOOL CXTPButton::SetBitmap(CSize size, UINT nID, BOOL bRedraw/*= TRUE*/)
{
	// Free previous resources (if any).

	CXTPImageManagerIconHandle hIconHandle;

	BOOL bAlphaBitmap = FALSE;
	HBITMAP hBitmap = CXTPImageManagerIcon::LoadBitmapFromResource(MAKEINTRESOURCE(nID), &bAlphaBitmap);

	if (!hBitmap)
		return FALSE;

	if (bAlphaBitmap)
	{
		hIconHandle.Attach(hBitmap, FALSE);
	}
	else
	{
		CBitmap bmpIcon;
		bmpIcon.Attach(hBitmap);

		// convert the bitmap to a transparent icon.
		HICON hIcon = CXTPTransparentBitmap(bmpIcon).ConvertToIcon();

		hIconHandle.Attach(hIcon, FALSE);
	}

	if (hIconHandle.IsEmpty())
		return FALSE;

	return CXTPButton::SetIcon(size, hIconHandle, CXTPImageManagerIconHandle(), bRedraw);
}

void CXTPButton::SetBuddy(CWnd* pWndBuddy, BOOL bMoveButton)
{
	m_hWndBuddy = pWndBuddy->GetSafeHwnd();
	if (!m_hWndBuddy)
		return;

	CRect rcBuddy;
	::GetWindowRect(m_hWndBuddy, &rcBuddy);

	GetParent()->ScreenToClient(rcBuddy);

	CXTPWindowRect rc(this);
	int nWidth = rc.Width();

	if (bMoveButton)
	{
		::MoveWindow(m_hWndBuddy, rcBuddy.left, rcBuddy.top, rcBuddy.Width() - nWidth + 2, rcBuddy.Height(), TRUE);
	}
	else
	{
		GetParent()->ScreenToClient(rc);
		::MoveWindow(m_hWndBuddy, rcBuddy.left, rcBuddy.top, (rc.left + 2) - rcBuddy.left, rcBuddy.Height(), TRUE);
		rcBuddy.right = rc.left + nWidth;
	}

	CRect rcButton = rcBuddy;
	rcButton.left = rcButton.right - nWidth;

	::MoveWindow(m_hWnd, rcButton.left, rcButton.top, rcButton.Width(), rcButton.Height(), TRUE);

}


INT_PTR CXTPButton::OnToolHitTest(CPoint point, TOOLINFO* pTI) const
{

	ASSERT_VALID(this);
	ASSERT(::IsWindow(m_hWnd));

	CString strTip = GetTooltip();
	if (strTip.GetLength() == 0)
		return -1;

	CXTPButton* pButton = (CXTPButton*)this;

	CXTPClientRect rc(pButton);
	rc.DeflateRect(2, 2);

	if (rc.PtInRect(point))
	{
		CXTPToolTipContext::FillInToolInfo(pTI, m_hWnd, rc, 1, strTip, pButton->GetButtonText(), strTip);
		return 1;
	}

	return -1;
}
