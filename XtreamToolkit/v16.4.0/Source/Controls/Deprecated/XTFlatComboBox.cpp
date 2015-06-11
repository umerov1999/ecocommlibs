// XTFlatComboBox.cpp : implementation of the CXTFlatComboBox class.
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

#include "Common/XTPDrawHelpers.h"
#include "Common/XTPMaskEditT.h"
#include "Common/XTPColorManager.h"

#include "../Util/XTPControlTheme.h"
#include "../Edit/XTPEdit.h"

#include "XTThemeManager.h"
#include "XTVC50Helpers.h"
#include "XTFlatComboBox.h"
#include "XTFlatControlsTheme.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define EVENT_TIMER   1000

/////////////////////////////////////////////////////////////////////////////
// CXTFlatComboBox

CXTFlatComboBox::CXTFlatComboBox()
: CXTThemeManagerStyleHost(GetThemeFactoryClass())
, m_bDisableAC(FALSE)
, m_bFlatLook(TRUE)
, m_bPainted(FALSE)
, m_bHasFocus(FALSE)
, m_bAutoComp(FALSE)
, m_nStyle(0)
, m_nStyleEx(0)
, m_crBack(COLORREF_NULL)
, m_crText(COLORREF_NULL)
{
}

CXTFlatComboBox::~CXTFlatComboBox()
{
}

IMPLEMENT_THEME_HOST(CXTFlatComboBox)
IMPLEMENT_THEME_REFRESH(CXTFlatComboBox, CComboBox)

IMPLEMENT_DYNAMIC(CXTFlatComboBox, CComboBox)

BEGIN_MESSAGE_MAP(CXTFlatComboBox, CComboBox)
	//{{AFX_MSG_MAP(CXTFlatComboBox)
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_CONTROL_REFLECT_EX(CBN_SETFOCUS, OnSetFocus)
	ON_CONTROL_REFLECT_EX(CBN_KILLFOCUS, OnKillFocus)
	ON_WM_SETCURSOR()
	ON_CONTROL_REFLECT_EX(CBN_EDITUPDATE, OnEditUpdate)
	ON_CONTROL_REFLECT_EX(CBN_CLOSEUP, OnEndSel)
	ON_WM_ERASEBKGND()
	ON_MESSAGE(WM_PRINTCLIENT, OnPrintClient)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTFlatComboBox message handlers

BOOL CXTFlatComboBox::PointInRect()
{
	ASSERT(::IsWindow(m_hWnd));

	CRect rc;
	GetWindowRect(rc);

	CPoint pt;
	GetCursorPos(&pt);

	return rc.PtInRect(pt);
}

BOOL CXTFlatComboBox::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	if (IsFlat() && (message == WM_MOUSEMOVE) && PointInRect())
	{
		SetTimer(EVENT_TIMER, 10, NULL);
		OnTimer(EVENT_TIMER);
	}

	return CComboBox::OnSetCursor(pWnd, nHitTest, message);
}

void CXTFlatComboBox::DisableFlatLook(BOOL bDisable)
{
	if (IsFlat() == bDisable)
	{
		m_bFlatLook = !bDisable;

		if (::IsWindow(m_hWnd))
		{
			if (IsFlat())
			{
				// save style.
				m_nStyle = GetStyle() & (WS_BORDER);
				m_nStyleEx = GetExStyle() & (WS_EX_CLIENTEDGE | WS_EX_STATICEDGE);

				ModifyStyle(WS_BORDER, 0);
				ModifyStyleEx(WS_EX_CLIENTEDGE | WS_EX_STATICEDGE, 0);
			}
			else
			{
				ModifyStyle(0, m_nStyle);
				ModifyStyleEx(0, m_nStyleEx);
			}

			SetWindowPos(NULL, 0, 0, 0, 0,
				SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED | SWP_NOZORDER);
		}
	}
}

void CXTFlatComboBox::OnTimer(UINT_PTR nIDEvent)
{
	if (EVENT_TIMER == nIDEvent)
	{
		if (!PointInRect())
		{
			KillTimer(EVENT_TIMER);

			if (m_bPainted == TRUE)
			{
				RedrawWindow();
			}

			m_bPainted = FALSE;
		}

		// on mouse over, show raised.
		else if (!m_bPainted)
		{
			RedrawWindow();
			m_bPainted = TRUE;
		}
	}
	else
	{
		CComboBox::OnTimer(nIDEvent);
	}
}

BOOL CXTFlatComboBox::OnEraseBkgnd(CDC* /*pDC*/)
{
	return TRUE;
}

void CXTFlatComboBox::OnPaint()
{
	// do default rendering first.
	CComboBox::OnPaint();

	// get the client device context.
	CClientDC dc(this);
	DoPaint(&dc);
}

void CXTFlatComboBox::CalcClientRect(CRect& rc)
{
	// deflate by 3D border, 3D edge and drop arrow (thumb).
	rc.DeflateRect(GetTheme()->GetBorderSize());
	rc.DeflateRect(GetTheme()->GetEdgeSize());
	rc.right -= GetTheme()->GetThumbSize().cx;
}

// a helper for rendering the control appearance
void CXTFlatComboBox::DoPaint(CDC* pDC)
{
	// draw theme.
	if (IsFlat())
	{
		// Get the client rect.
		CXTPClientRect r(this);

		// exclude client portions from redraw.
		CRect rClip(r);
		CalcClientRect(rClip);
		pDC->ExcludeClipRect(&rClip);

		// Paint to a memory device context to help
		// eliminate screen flicker.
		CXTPBufferDC memDC(*pDC, r);
		memDC.FillSolidRect(r, GetXtremeColor(COLOR_3DFACE));

		if (m_bHasFocus || PointInRect())
		{
			DrawCombo(&memDC, GetDroppedState() ? xtMouseSelect : xtMouseHover);
		}
		else
		{
			DrawCombo(&memDC, xtMouseNormal);
		}
	}
}

void CXTFlatComboBox::DrawCombo(CDC* pDC, XTMouseState  eState)
{
	GetTheme()->DrawFlatComboBox(pDC, this, eState);
}

BOOL CXTFlatComboBox::OnSetFocus()
{
	if (IsFlat())
	{
		m_bHasFocus = TRUE;
		RedrawWindow();
	}

	return FALSE;
}

BOOL CXTFlatComboBox::OnKillFocus()
{
	if (IsFlat())
	{
		m_bHasFocus = FALSE;
		RedrawWindow();
	}

	return FALSE;
}

BOOL CXTFlatComboBox::PreTranslateMessage(MSG* pMsg)
{
	// Make sure that the keystrokes continue to the edit control.
	if (pMsg->message == WM_KEYDOWN || pMsg->message == WM_KEYUP)
	{
		// if tab, return or escape key, just use default.
		switch (pMsg->wParam)
		{
		case VK_DELETE:
		case VK_BACK:
			{
				if (m_bAutoComp)
				{
					m_bDisableAC = (pMsg->message == WM_KEYDOWN);
				}
				break;
			}
		case VK_TAB:
		case VK_RETURN:
		case VK_ESCAPE:
			{
				return CComboBox::PreTranslateMessage(pMsg);
			}
		}

		// If the combo box has an edit control, don't allow
		// the framework to process accelerators, let the edit
		// control handle it instead.  GetEditSel() will return
		// CB_ERR if there is no edit control present...

		if (GetEditSel() != (DWORD)CB_ERR)
		{
			::TranslateMessage(pMsg);
			::DispatchMessage(pMsg);

			return TRUE;
		}
	}

	return CComboBox::PreTranslateMessage(pMsg);
}

BOOL CXTFlatComboBox::OnEditUpdate()
{
	// if we are not to auto update the text, get outta here
	if (m_bAutoComp)
	{
		if (m_bDisableAC)
		{
			Default();
		}
		else
		{
			// Get the text in the edit box
			CString strItemTyped;
			GetWindowText(strItemTyped);
			int nLength = strItemTyped.GetLength();

			if (nLength >= 1)
			{
				// Currently selected range
				DWORD dwCurSel = GetEditSel();
				int nStart = LOWORD(dwCurSel);
				int nEnd = HIWORD(dwCurSel);

				// Search for, and select in, and string in the combo box that is prefixed
				// by the text in the edit box
				if (SelectString(-1, strItemTyped) == CB_ERR)
				{
					SetWindowText(strItemTyped);     // No text selected, so restore what was there before
					if (dwCurSel != (DWORD)CB_ERR)
					{
						SetEditSel(nStart, nEnd);   //restore cursor postion
					}
				}

				// Set the text selection as the additional text that we have added
				if (nEnd < nLength && dwCurSel != (DWORD)CB_ERR)
				{
					SetEditSel(nStart, nEnd);
				}
				else
				{
					SetEditSel(nLength, -1);
				}
			}
		}
	}

	return FALSE;
}

BOOL CXTFlatComboBox::OnEndSel()
{
	if (IsFlat())
	{
		Invalidate();
	}

	return FALSE;
}

LRESULT CXTFlatComboBox::OnPrintClient(WPARAM wp, LPARAM)
{
	LRESULT lResult = Default();

	CDC* pDC = CDC::FromHandle((HDC)wp);
	DoPaint(pDC);

	return lResult;
}


/////////////////////////////////////////////////////////////////////////////
// CXTFlatEdit
/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_THEME_HOST(CXTFlatEdit)
IMPLEMENT_THEME_REFRESH(CXTFlatEdit, CXTEdit)

CXTFlatEdit::CXTFlatEdit()
: CXTThemeManagerStyleHost(GetThemeFactoryClass())
, m_bFlatLook(TRUE)
, m_bHasFocus(FALSE)
, m_bPainted(FALSE)
{
	m_nStyle = m_nStyleEx = 0;
}

CXTFlatEdit::~CXTFlatEdit()
{
}

IMPLEMENT_DYNAMIC(CXTFlatEdit, CXTEdit)

BEGIN_MESSAGE_MAP(CXTFlatEdit, CXTEdit)
	//{{AFX_MSG_MAP(CXTFlatEdit)
	ON_WM_NCPAINT()
	ON_WM_TIMER()
	ON_WM_SETCURSOR()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTFlatEdit message handlers

BOOL CXTFlatEdit::PointInRect()
{
	ASSERT(::IsWindow(m_hWnd));

	CRect rc;
	GetWindowRect(rc);

	CPoint pt;
	GetCursorPos(&pt);

	return rc.PtInRect(pt);
}

BOOL CXTFlatEdit::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	if (IsFlat() && (message == WM_MOUSEMOVE) && PointInRect())
	{
		SetTimer(EVENT_TIMER, 10, NULL);
		OnTimer(EVENT_TIMER);
	}

	return CXTEdit::OnSetCursor(pWnd, nHitTest, message);
}

void CXTFlatEdit::DisableFlatLook(BOOL bDisable)
{
	if (m_bFlatLook == bDisable)
	{
		m_bFlatLook = !bDisable;
		SendMessage(WM_NCPAINT);
	}
}

void CXTFlatEdit::OnTimer(UINT_PTR nIDEvent)
{
	if (EVENT_TIMER == nIDEvent)
	{
		if (!PointInRect())
		{
			KillTimer(EVENT_TIMER);

			if (m_bPainted == TRUE)
			{
				SendMessage(WM_NCPAINT);
			}

			m_bPainted = FALSE;
		}

		// on mouse over, show raised.
		else if (!m_bPainted)
		{
			SendMessage(WM_NCPAINT);
			m_bPainted = TRUE;
		}
	}
	else
	{
		CXTEdit::OnTimer(nIDEvent);
	}
}

void CXTFlatEdit::OnNcPaint()
{
	if (IsFlat())
	{
		CWindowDC dc(this);

		CXTPClientRect rClient(this);
		CXTPWindowRect rWindow(this);
		ScreenToClient(rWindow);
		rClient.OffsetRect(-rWindow.left, -rWindow.top);
		dc.ExcludeClipRect(rClient);
		rWindow.OffsetRect(-rWindow.left, -rWindow.top);

		CXTPBufferDC memDC(dc, rWindow);
		dc.FillSolidRect(rWindow, IsWindowEnabled() ? GetXtremeColor(COLOR_WINDOW) : GetXtremeColor(COLOR_3DFACE));

		DrawBorders(&memDC, rWindow);
	}
	else
	{
		Default();
	}
}

// a helper for rendering the control appearance
void CXTFlatEdit::DrawBorders(CDC* pDC, const CRect& rWindow)
{
	if (m_bHasFocus || PointInRect())
	{
		GetTheme()->DrawBorders(pDC, this, rWindow, xtMouseHover);
	}
	else
	{
		GetTheme()->DrawBorders(pDC, this, rWindow, xtMouseNormal);
	}
}

void CXTFlatEdit::OnSetFocus(CWnd* pOldWnd)
{
	CXTEdit::OnSetFocus(pOldWnd);

	if (IsFlat())
	{
		m_bHasFocus = TRUE;
		Invalidate();
		SendMessage(WM_NCPAINT);
	}
}

void CXTFlatEdit::OnKillFocus(CWnd* pNewWnd)
{
	CXTEdit::OnKillFocus(pNewWnd);

	if (IsFlat())
	{
		m_bHasFocus = FALSE;
		Invalidate();
		SendMessage(WM_NCPAINT);
	}
}
