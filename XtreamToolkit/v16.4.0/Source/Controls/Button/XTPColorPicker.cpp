// XTPColorPicker.cpp : implementation of the CXTPColorPicker class.
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

#include "stdafx.h"

#include "Common/XTPColorManager.h"
#include "Common/XTPDrawHelpers.h"

#include "../Util/XTPControlTheme.h"

#include "../Defines.h"
#include "XTPButton.h"
#include "XTPButtonTheme.h"
#include "../Popup/XTPColorSelectorCtrl.h"
#include "../Popup/XTPColorPopup.h"
#include "XTPColorPicker.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const UINT WM_XTP_SHOWPOPUP = (WM_XTP_CONTROLS_BASE + 29);


/////////////////////////////////////////////////////////////////////////////
// DDX Routines

_XTP_EXT_CLASS void AFXAPI DDX_XTPColorPicker(CDataExchange *pDX, int nIDC, COLORREF& value)
{
	HWND hWndCtrl = pDX->PrepareCtrl(nIDC);
	ASSERT (hWndCtrl != NULL);

	CXTPColorPicker* pColorPicker = (CXTPColorPicker*) CWnd::FromHandle(hWndCtrl);
	if (pDX->m_bSaveAndValidate)
	{
		value = pColorPicker->GetColor();
	}
	else
	{
		pColorPicker->SetColor(value);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CXTPColorPicker

CXTPColorPicker::CXTPColorPicker()
: m_dwPopup(CPS_XTP_EXTENDED | CPS_XTP_MORECOLORS)
, m_clrSelected(CLR_DEFAULT)
, m_clrDefault(CLR_DEFAULT)
, m_bShowText(FALSE)
, m_bColorText(FALSE)
, m_bPopupActive(FALSE)
{
	m_nCols = 8;
	m_pExtendedColors = 0;
	m_nExtendedColors = 0;
}

CXTPColorPicker::~CXTPColorPicker()
{

}

IMPLEMENT_DYNAMIC(CXTPColorPicker, CXTPButton)

BEGIN_MESSAGE_MAP(CXTPColorPicker, CXTPButton)
	//{{AFX_MSG_MAP(CXTPColorPicker)
	//}}AFX_MSG_MAP
	ON_MESSAGE(CPN_XTP_SELCHANGE, OnSelChange)
	ON_MESSAGE(CPN_XTP_DROPDOWN, OnDropDown)
	ON_MESSAGE(CPN_XTP_CLOSEUP, OnCloseUp)
	ON_MESSAGE(CPN_XTP_SELENDOK, OnSelEndOK)
	ON_MESSAGE(CPN_XTP_SELENDCANCEL, OnSelEndCancel)
	ON_MESSAGE(CPN_XTP_SELNOFILL, OnSelNoFill)
	ON_MESSAGE_VOID(WM_XTP_SHOWPOPUP, OnShowPopup)
END_MESSAGE_MAP()



void CXTPColorPicker::DrawColorPicker(CDC* pDC)
{
	// draw the color box.
	CRect rcItem;
	GetClientRect(rcItem);
	rcItem.DeflateRect(5, 5, 20, 5);

	BOOL  bSelected = GetChecked() || IsPushed();
	if (bSelected && (m_pTheme->m_bOffsetHiliteText || m_pTheme->IsVisualThemeUsed(this)))
		rcItem.OffsetRect(1, 1);

	BOOL bEnabled = IsWindowEnabled();

	COLORREF clrButton = GetColor();

	// create and select the brush for the rectangle background.
	COLORREF clrRect = !bEnabled ? GetXtremeColor(COLOR_3DFACE) : clrButton;
	//if (pColorPicker->IsTextShown() && pColorPicker->IsTextInColor())
	//  clrRect = pColorPicker->GetContrastColor();
	CXTPBrushDC dcBrush(pDC->m_hDC, clrRect);

	// create and select the pen for the rectangle border.
	COLORREF crPen = !bEnabled ? GetXtremeColor(COLOR_3DSHADOW) : RGB(0, 0, 0);
	CXTPPenDC dcPen(pDC->m_hDC, crPen);


	// draw the color rectangle.
	if ( clrButton != (COLORREF)-1) pDC->Rectangle(rcItem);

	CString strText = GetButtonText();
	if (!strText.IsEmpty())
	{
		COLORREF clrText = GetContrastColor();
		pDC->SetTextColor(clrText);
		pDC->SetBkMode(TRANSPARENT);

		pDC->DrawText(strText, rcItem, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}


	// Draw arrow
	GetClientRect(rcItem);
	rcItem.DeflateRect(GetBorderGap(), GetBorderGap());

	rcItem.left = rcItem.right - 16;
	if (bSelected && (m_pTheme->m_bOffsetHiliteText || m_pTheme->IsVisualThemeUsed(this)))
		rcItem.OffsetRect(1, 1);

	CPoint ptCenter = rcItem.CenterPoint();

	XTPDrawHelpers()->Triangle(pDC, CPoint(ptCenter.x - 4, ptCenter.y - 2),
		CPoint(ptCenter.x + 4, ptCenter.y - 2), CPoint(ptCenter.x, ptCenter.y + 2), 0);
}

void CXTPColorPicker::DoDraw(CDC* pDC)
{
	m_pTheme->DrawButtonBackground(pDC, this);

	DrawColorPicker(pDC);

	if (::GetFocus() == m_hWnd)
	{
		m_pTheme->DrawFocusRect(pDC, this);
	}
}

BOOL CXTPColorPicker::IsPushed()
{
	return m_bPopupActive;
}


/////////////////////////////////////////////////////////////////////////////
// CXTPColorPicker message handlers

LRESULT CXTPColorPicker::OnSelChange(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	CWnd* pOwner = GetOwner();
	ASSERT_VALID(pOwner);

	if (pOwner != NULL)
	{
		// Notify of selection change.
		pOwner->SendMessage(WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(),
			CPN_XTP_SELCHANGE), (LPARAM)m_hWnd);
	}

	return 0;
}

LRESULT CXTPColorPicker::OnDropDown(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	CWnd* pOwner = GetOwner();
	ASSERT_VALID(pOwner);

	if (pOwner != NULL)
	{
		// Notify of popup activation.
		pOwner->SendMessage(WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(),
			CPN_XTP_DROPDOWN), (LPARAM)m_hWnd);
	}

	return 0;
}

LRESULT CXTPColorPicker::OnCloseUp(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	m_bPopupActive = FALSE;

	CWnd* pOwner = GetOwner();
	ASSERT_VALID(pOwner);

	if (pOwner != NULL)
	{
		// Notify of popup close up.
		pOwner->SendMessage(WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(),
			CPN_XTP_CLOSEUP), (LPARAM)m_hWnd);
	}
	ReleaseCapture();
	SetFocus();
	Invalidate();
	return 0;
}

LRESULT CXTPColorPicker::OnSelEndOK(WPARAM wParam, LPARAM /*lParam*/)
{
	CWnd* pOwner = GetOwner();
	ASSERT_VALID(pOwner);

	if (pOwner != NULL)
	{
		SetColor((COLORREF)wParam);

		// Notify of popup close up.
		// NB: doing so may destroy the picker so lets copy off
		// the window handle/control ID for safety
		HWND hWndSender = m_hWnd;
		int nControlID = GetDlgCtrlID();
		pOwner->SendMessage(WM_COMMAND, MAKEWPARAM(nControlID,
			CPN_XTP_CLOSEUP), (LPARAM)hWndSender);

		// Notify of selection made.
		pOwner->SendMessage(WM_COMMAND, MAKEWPARAM(nControlID,
			CPN_XTP_SELENDOK), (LPARAM)hWndSender);
	}

	return 0;
}

LRESULT CXTPColorPicker::OnSelEndCancel(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	CWnd* pOwner = GetOwner();
	ASSERT_VALID(pOwner);

	if (pOwner != NULL)
	{
		// Notify of popup close up.
		pOwner->SendMessage(WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(),
			CPN_XTP_CLOSEUP), (LPARAM)m_hWnd);

		// Notify of cancel.
		pOwner->SendMessage(WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(),
			CPN_XTP_SELENDCANCEL), (LPARAM)m_hWnd);
	}

	return 0;
}

LRESULT CXTPColorPicker::OnSelNoFill(WPARAM wParam, LPARAM /*lParam*/)
{
	CWnd* pOwner = GetOwner();
	ASSERT_VALID(pOwner);

	if (pOwner != NULL)
	{
		SetColor((COLORREF)wParam);

		// Notify of no fill press.
		pOwner->SendMessage(WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(),
			CPN_XTP_SELNOFILL), (LPARAM)m_hWnd);
	}

	return 0;
}

void CXTPColorPicker::SetColor(COLORREF clr)
{
	if (clr != GetColor())
	{
		m_clrSelected = clr;
		if (m_hWnd) RedrawWindow();
	}
}

COLORREF CXTPColorPicker::GetContrastColor() const
{
	COLORREF cr = GetColor();
	double Intensity = ((299 * GetRValue(cr)) + (587 * GetGValue(cr)) + (114 * GetBValue(cr))) / 1000.0;
	return Intensity > 128 ? RGB(0x00,0x00,0x00) : RGB(0xff,0xff,0xff);
}

void CXTPColorPicker::ShowPopupWindow()
{
	m_bPopupActive = TRUE;

	// Make sure that we have input focus.
	if (GetFocus() != this)
	{
		SetFocus();
	}

	// Post a message instead of displaying right away - this will take care
	// of asynchronous focusing issues
	PostMessage(WM_XTP_SHOWPOPUP);
}

void CXTPColorPicker::OnShowPopup()
{
	// Get the size of the picker button.
	CRect rcWindow;
	GetWindowRect(&rcWindow);

	// Create the color popup window.
	CXTPColorPopup *pColorPopup = new CXTPColorPopup(TRUE);

	if (m_pExtendedColors)
	{
		pColorPopup->SetColors(m_pExtendedColors, m_nExtendedColors, m_nCols);
	}

	pColorPopup->SetTheme(m_pTheme->IsVisualThemeUsed(this) ? new CXTPColorSelectorCtrlThemeOfficeXP() :
		m_pTheme->GetThemeStyle() == xtpControlThemeDefault || m_pTheme->GetThemeStyle() == xtpControlThemeOffice2000 ? new CXTPColorSelectorCtrlTheme():
		m_pTheme->GetThemeStyle() == xtpControlThemeResource ? new CXTPColorSelectorCtrlThemeResource() :
		m_pTheme->GetThemeStyle() == xtpControlThemeOffice2003 ? new CXTPColorSelectorCtrlThemeOffice2003() :
		new CXTPColorSelectorCtrlThemeOfficeXP());

	if (!pColorPopup->Create(rcWindow, this, m_dwPopup, GetColor(), GetDefaultColor()))
		return;
	pColorPopup->SetFocus();
}

BOOL CXTPColorPicker::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	if (!m_bPopupActive)
	{
		if (message == WM_LBUTTONDOWN)
		{
			ShowPopupWindow();
			// in this case the message is not "swallowed" so the button will
			// get it and display itself in a recessed state
		}
		else if (message == WM_KEYDOWN &&
			(wParam == VK_RETURN ||
			wParam == VK_SPACE ||
			wParam == VK_DOWN))
		{
			ShowPopupWindow();
			return TRUE;  // swallow message
		}
	}

	return CXTPButton::OnWndMsg(message, wParam, lParam, pResult);
}
