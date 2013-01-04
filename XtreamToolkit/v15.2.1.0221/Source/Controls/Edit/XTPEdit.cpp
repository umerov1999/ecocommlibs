// XTPEdit.cpp : implementation of the CXTPEdit class.
//
// This file is a part of the XTREME CONTROLS MFC class library.
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
#include "Common/XTPResourceManager.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPResourceImage.h"
#include "Common/XTPRegExp.h"
#include "Common/XTPMaskEditT.h"
#include "Common/XTPWinThemeWrapper.h"
#include "../Util/XTPControlTheme.h"

#include "../Util/XTPGlobal.h"
#include "../Resource.h"
#include "../Util/XTPFunctions.h"
#include "XTPEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#pragma warning(disable: 4097)


/////////////////////////////////////////////////////////////////////////////
// CXTPMaskEdit
/////////////////////////////////////////////////////////////////////////////

CXTPEdit::CXTPEdit()
{
	m_bPreSubclassInit = TRUE;

	m_bUseVisualStyle = TRUE;
	m_nTheme = xtpControlThemeDefault;

	m_bHighlighted = FALSE;
	m_bFocused = FALSE;
	m_bFlatStyle = FALSE;

	m_bShowBorder = TRUE;

	m_pRegExp = NULL;

	m_themeEdit = new CXTPWinThemeWrapper();
}

CXTPEdit::~CXTPEdit()
{
	SAFE_DELETE(m_pRegExp);
	SAFE_DELETE(m_themeEdit);
}


IMPLEMENT_DYNAMIC(CXTPEdit, CEdit)

BEGIN_MESSAGE_MAP(CXTPEdit, CEdit)
	ON_MASKEDIT_REFLECT()
	ON_WM_CREATE()
	ON_WM_NCPAINT()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_MESSAGE_VOID(WM_MOUSELEAVE, OnMouseLeave)
	ON_WM_NCCALCSIZE()
	ON_WM_MOUSEMOVE()
	ON_WM_KILLFOCUS()
	ON_WM_SETFOCUS()
	ON_WM_SYSCOLORCHANGE()

	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEditCut)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CLEAR, OnUpdateEditClear)
	ON_COMMAND(ID_EDIT_SELECT_ALL, OnEditSelectAll)
	ON_UPDATE_COMMAND_UI(ID_EDIT_SELECT_ALL, OnUpdateEditSelectAll)
	ON_WM_INITMENUPOPUP()

	ON_CONTROL_REFLECT_EX(EN_CHANGE, OnChange)
END_MESSAGE_MAP()



BOOL CXTPEdit::SetEditMask(LPCTSTR lpszMask, LPCTSTR lpszLiteral, LPCTSTR lpszDefault /*= NULL*/)
{
	return TBase::SetEditMask(lpszMask, lpszLiteral, lpszDefault);
}

void CXTPEdit::Init()
{
	RefreshMetrics();

	if (m_bShowBorder)
	{
		m_bShowBorder = GetExStyle() & WS_EX_CLIENTEDGE;
	}

	if (m_bShowBorder)
	{
		ModifyStyleEx(WS_EX_CLIENTEDGE, 0, 0);
		ModifyStyle(WS_BORDER, 0, SWP_FRAMECHANGED);
		PostMessage(WM_NCPAINT);
	}
}

void CXTPEdit::PreSubclassWindow()
{
	TBase::PreSubclassWindow();

	if (m_bPreSubclassInit)
	{
		// Initialize the control.
		Init();
	}
}

int CXTPEdit::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (TBase::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Initialize the control.
	Init();

	return 0;
}

BOOL CXTPEdit::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!TBase::PreCreateWindow(cs))
		return FALSE;

	// When creating controls dynamically Init() must
	// be called from OnCreate() and not from
	// PreSubclassWindow().

	m_bPreSubclassInit = FALSE;

	return TRUE;
}

void CXTPEdit::OnContextMenu(CWnd*, CPoint point)
{
	if (point.x == -1 && point.y == -1)
	{
		//keystroke invocation
		CRect rect;
		GetClientRect(rect);
		ClientToScreen(rect);

		point = rect.TopLeft();
		point.Offset(5, 5);
	}

	SetFocus();

	CMenu menu;
	CXTPResourceManager::AssertValid(XTPResourceManager()->LoadMenu(&menu, XTP_IDM_POPUP));

	CMenu* pPopup = menu.GetSubMenu(1);
	ASSERT(pPopup != NULL);
	CWnd* pWndPopupOwner = this;

	XTPContextMenu(pPopup, TPM_LEFTALIGN | TPM_RIGHTBUTTON,
		point.x, point.y, pWndPopupOwner, XTP_IDR_TBAR_HEXEDIT, FALSE);
}

void CXTPEdit::OnEditUndo()
{
	MaskUndo();
}

void CXTPEdit::OnUpdateEditUndo(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(CanUndo());
}

void CXTPEdit::OnEditCut()
{
	MaskCut();
}

void CXTPEdit::OnUpdateEditCut(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(HasSelection() && ((GetStyle() & ES_READONLY) == 0));
}

void CXTPEdit::OnEditCopy()
{
	MaskCopy();
}

void CXTPEdit::OnUpdateEditCopy(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(HasSelection());
}

void CXTPEdit::OnEditPaste()
{
	MaskPaste();
}

void CXTPEdit::OnUpdateEditPaste(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(CanPaste());
}

void CXTPEdit::OnEditClear()
{
	MaskClear();
}

void CXTPEdit::OnUpdateEditClear(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(HasSelection() && ((GetStyle() & ES_READONLY) == 0));
}

void CXTPEdit::OnEditSelectAll()
{
	SetSel(0, -1);
}

void CXTPEdit::OnUpdateEditSelectAll(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(GetWindowTextLength() > 0);
}

BOOL CXTPEdit::CanPaste()
{
	return IsWindowEnabled() && ((GetStyle() & ES_READONLY) == 0) & ::IsClipboardFormatAvailable(CF_TEXT);
}

BOOL CXTPEdit::HasSelection() const
{
	int nStartChar, nEndChar;
	GetSel(nStartChar, nEndChar);
	return (nStartChar < nEndChar);
}

BOOL CXTPEdit::OnEraseBkgnd(CDC* /*pDC*/)
{
	return TRUE;
}

BOOL CXTPEdit::IsPatternValid()
{
	if (!m_pRegExp)
		return TRUE;

	CString strWindowText;
	GetWindowText(strWindowText);

	int nCount = m_pRegExp->Execute(strWindowText);
	if (nCount != 1)
		return FALSE;

	long nStart, nLength;
	m_pRegExp->GetMatch(0, &nStart, &nLength);

	return nStart == 0 && nLength == strWindowText.GetLength();

}

void CXTPEdit::OnPaint()
{
	CPaintDC dcPaint(this); // device context for painting

	// Get the client rect.
	CXTPClientRect rectClient(this);

	// Paint to a memory device context to help
	// eliminate screen flicker.
	CXTPBufferDC memDC(dcPaint);
	::FillRect(memDC, rectClient, GetClientBrush(&memDC));

	// Now let the window do its default painting...
	CEdit::DefWindowProc(WM_PAINT, (WPARAM)memDC.m_hDC, 0);

	if (!IsPatternValid())
	{
		CString strText;
		GetWindowText(strText);

		CRect rect;
		const int iIncrement = 2;

		// Get the rect for the entire edit control
		((CEdit*)this)->GetRect(&rect);

		// Check that wavy line will not over-write edit control border...
		if (rectClient.bottom - rect.bottom >= iIncrement + 1)
		{
			CSize size;
			int iX = 0;

			rect.bottom -= iIncrement - 2;

			// We must use the correct font when using GetTextExtent..!
			CXTPFontDC font(&memDC, CWnd::GetFont());
			CXTPPenDC pen(memDC, 0xFF);

			BOOL bLayoutRLT = GetExStyle() & WS_EX_RIGHT;

			if (bLayoutRLT)
			{
				size =  memDC.GetTextExtent (strText);

				// Check for case of empty string
				if (size.cx == 0) size.cx = iIncrement * 3;
				// Dont draw off the end of the edit control
				else if (size.cx > rect.Width()) size.cx = rect.Width();

				int iOffset = 0;

				if (GetStyle () & ES_CENTER)
				{
					iOffset = (rect.Width () - size.cx) / 2;
				}

				rect.right -= iOffset;
				// Starting x co-ordinate to start drawing from
				iX = rect.right;
				// Start at the bottom left of the edit control
				memDC.MoveTo(rect.right, rect.bottom);

				// Draw the wavy line like in Microsoft Word
				while (iX > rect.right - size.cx)
				{
					iX -= iIncrement;

					if (iX < rect.right - size.cx)
						break;

					memDC.LineTo (iX, rect.bottom + iIncrement);
					iX -= iIncrement;

					if (iX < rect.right - size.cx)
						break;

					memDC.LineTo(iX, rect.bottom);
				}
			}
			else
			{

				// This part deals with the *displayed* Text Extent
				size = memDC.GetTextExtent (strText.
					Mid(((CEdit*)this)->CharFromPos (CPoint (rect.left + 1, rect.top))));

				// Check for case of empty string
				if (size.cx == 0) size.cx = iIncrement * 3;
				// Dont draw off the end of the edit control
				else if (size.cx > rect.right) size.cx = rect.right;

				int iOffset = 0;

				if (GetStyle () & ES_CENTER)
				{
					iOffset = (rect.Width () - size.cx) / 2;
				}
				else if (GetStyle () & ES_RIGHT)
				{
					iOffset = rect.Width () - size.cx;
				}

				rect.left += iOffset;
				// Starting x co-ordinate to start drawing from
				iX = rect.left;
				// Start at the bottom left of the edit control
				memDC.MoveTo(rect.left, rect.bottom);

				// Draw the wavy line like in Microsoft Word
				while (iX < size.cx + rect.left)
				{
					iX += iIncrement;

					if (iX > size.cx + rect.left)
						break;

					memDC.LineTo (iX, rect.bottom + iIncrement);
					iX += iIncrement;

					if (iX > size.cx + rect.left)
						break;

					memDC.LineTo(iX, rect.bottom);
				}
			}
		}
		else
		{
			TRACE (_T("Edit Control too small to draw wavy line!\r\n"));
		}

	}
}

void CXTPEdit::OnInitMenuPopup(CMenu* pPopupMenu, UINT /*nIndex*/, BOOL bSysMenu)
{
	Default();

	if (!bSysMenu)
	{
		ASSERT(pPopupMenu != NULL);
		if (!pPopupMenu)
			return;

		// check the enabled state of various menu items
		CCmdUI state;
		state.m_pMenu = pPopupMenu;
		ASSERT(state.m_pOther == NULL);

		state.m_nIndexMax = pPopupMenu->GetMenuItemCount();
		for (state.m_nIndex = 0; state.m_nIndex < state.m_nIndexMax;
			state.m_nIndex++)
		{
			state.m_nID = pPopupMenu->GetMenuItemID(state.m_nIndex);
			if (state.m_nID == 0)
				continue; // menu separator or invalid cmd - ignore it

			ASSERT(state.m_pOther == NULL);
			ASSERT(state.m_pMenu != NULL);
			if (state.m_nID == (UINT)-1)
			{
				// possibly a popup menu, route to first item of that popup
				state.m_pSubMenu = pPopupMenu->GetSubMenu(state.m_nIndex);
				if (state.m_pSubMenu == NULL)
					continue;

				state.m_nID = state.m_pSubMenu->GetMenuItemID(0);
				if (state.m_nID == 0 || state.m_nID == (UINT)-1)
					continue; // first item of popup can't be routed to

				state.DoUpdate(this, FALSE);  // popups are never auto disabled
			}
			else
			{
				// normal menu item
				// Auto enable/disable if command is _not_ a system command
				state.m_pSubMenu = NULL;
				state.DoUpdate(this, state.m_nID < 0xF000);
			}
		}
	}
}

bool CXTPEdit::Initialize(CWnd* /*pParentWnd*/)
{
	// edit control must be valid in order to initialize.
	ASSERT_VALID(this);
	if (!::IsWindow(m_hWnd))
		return false;

	// set the font and parent for the browse edit.
	SetFont(&XTPAuxData().font);

	return true;
}

void CXTPEdit::SetTheme(XTPControlTheme nTheme)
{
	m_nTheme = nTheme;

	m_bUseVisualStyle = (m_nTheme == xtpControlThemeDefault);

	RefreshMetrics();

	RedrawEdit();
}

void CXTPEdit::RefreshMetrics()
{
	RefreshXtremeColors();
	m_clrBorderNormal = m_clrBorderHot = GetXtremeColor(COLOR_3DSHADOW);

	if (m_nTheme == xtpControlThemeFlat)
	{
		m_clrBorderNormal = m_clrBorderHot = GetXtremeColor(COLOR_WINDOWFRAME);

	}
	if (m_nTheme == xtpControlThemeOfficeXP || m_nTheme == xtpControlThemeOffice2003)
	{
		m_clrBorderHot = GetXtremeColor(XPCOLOR_HIGHLIGHT_BORDER);
	}

	if (m_nTheme == xtpControlThemeOffice2003)
	{
		XTPCurrentSystemTheme systemTheme = XTPColorManager()->GetCurrentSystemTheme();
		switch (systemTheme)
		{
		case xtpSystemThemeBlue:
		case xtpSystemThemeRoyale:
		case xtpSystemThemeAero:
			m_clrBorderNormal = RGB(127, 157, 185);
			m_clrBorderHot = RGB(0, 0, 128);
			break;

		case xtpSystemThemeOlive:
			m_clrBorderNormal = RGB(164, 185, 127);
			m_clrBorderHot = RGB(63, 93, 56);
			break;

		case xtpSystemThemeSilver:
			m_clrBorderNormal = RGB(165, 172, 178);
			m_clrBorderHot = RGB(75, 75, 11);
			break;
		}
	}

	if (m_nTheme == xtpControlThemeResource)
	{
		CXTPResourceImages* pImages = XTPResourceImages();

		m_clrBorderHot = m_clrBorderNormal = pImages->GetImageColor(_T("Toolbar"), _T("ControlEditBorder"));
	}

	m_themeEdit->OpenTheme(0, L"EDIT");
}

void CXTPEdit::OnNcPaint()
{
	TBase::OnNcPaint();
	if (!m_bShowBorder)
		return;

	CWindowDC dc(this);

	CRect rc;
	GetWindowRect(&rc);
	rc.OffsetRect(-rc.TopLeft());

	DrawNcBorders(&dc, rc);

	if ((GetStyle() & (WS_VSCROLL | WS_HSCROLL)) == (WS_VSCROLL | WS_HSCROLL))
	{
		if (m_bShowBorder) rc.DeflateRect(2, 2);
		rc.SetRect(rc.right - GetSystemMetrics(SM_CXVSCROLL), rc.bottom - GetSystemMetrics(SM_CYHSCROLL), rc.right, rc.bottom);
		dc.FillSolidRect(rc, GetSysColor(COLOR_BTNFACE));
	}
}

void CXTPEdit::FillSolidRect(HDC hdc, int x, int y, int cx, int cy, HBRUSH hBrush)
{
	::FillRect(hdc, CRect(x, y, x + cx, y + cy), hBrush);
}

void  CXTPEdit::DrawFrame(HDC hdc, LPRECT lprc, int nSize, HBRUSH hBrush)
{
	FillSolidRect(hdc, lprc->left, lprc->top, lprc->right - lprc->left - nSize, nSize, hBrush);
	FillSolidRect(hdc, lprc->left, lprc->top, nSize, lprc->bottom - lprc->top - nSize, hBrush);
	FillSolidRect(hdc, lprc->right - nSize, lprc->top, nSize, lprc->bottom - lprc->top, hBrush);
	FillSolidRect(hdc, lprc->left, lprc->bottom - nSize, lprc->right - lprc->left, nSize, hBrush);
}

void CXTPEdit::DrawNcBorders(CDC* pDC, CRect rc)
{
	if (!m_bShowBorder)
		return;

	CRect rcIntersect(0, 0, 0, 0);

	HWND hwndBuddy = ::GetWindow(m_hWnd, GW_HWNDNEXT);
	if (hwndBuddy)
	{
		CXTPWindowRect rcSpin(hwndBuddy);
		CXTPWindowRect rcEdit(m_hWnd);

		if (rcIntersect.IntersectRect(rcSpin, rcEdit))
		{
			rcIntersect.OffsetRect(-rcEdit.TopLeft());
			pDC->ExcludeClipRect(rcIntersect);
		}
		else
		{
			rcIntersect.SetRectEmpty();
		}

	}

	HBRUSH hBrush = GetClientBrush(pDC);

	if (!m_bHighlighted && !m_bFocused && m_bFlatStyle)
	{
		DrawFrame(*pDC, rc, 2, hBrush);
		return;
	}

	if (m_bUseVisualStyle && m_themeEdit->IsAppThemed())
	{
		CRect rcClient(rc.left + 1, rc.top + 1, rc.right - 1, rc.bottom - 1);
		pDC->ExcludeClipRect(rcClient);

		if (SUCCEEDED(m_themeEdit->DrawThemeBackground(*pDC, 0, IsWindowEnabled() ? ETS_NORMAL : ETS_DISABLED, &rc, NULL)))
		{
			pDC->SelectClipRgn(NULL);
			if (!rcIntersect.IsRectEmpty()) pDC->ExcludeClipRect(rcIntersect);
			rc.DeflateRect(1, 1);
			DrawFrame(*pDC, rc, 1, hBrush);
			return;
		}
	}

	if (m_nTheme == xtpControlThemeDefault)
	{
		pDC->DrawEdge(rc, EDGE_SUNKEN, BF_RECT);
		return;
	}
	if (m_nTheme == xtpControlThemeOffice2000)
	{
		pDC->Draw3dRect(rc, m_clrBorderNormal, GetXtremeColor(COLOR_BTNHIGHLIGHT));
	}
	else
	{
		COLORREF clrBorder = m_bHighlighted || m_bFocused ? m_clrBorderHot : m_clrBorderNormal;
		pDC->Draw3dRect(rc, clrBorder, clrBorder);
	}

	rc.DeflateRect(1, 1);
	DrawFrame(*pDC, rc, 1, hBrush);
}

void CXTPEdit::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp)
{
	TBase::OnNcCalcSize(bCalcValidRects, lpncsp);

	if (!m_bShowBorder)
		return;

	lpncsp[0].rgrc->top += 2;
	lpncsp[0].rgrc->left += 2;
	lpncsp[0].rgrc->right -= 2;
	lpncsp[0].rgrc->bottom -= 2;
}

void CXTPEdit::OnMouseLeave()
{
	OnMouseMove(0, CPoint(-1, -1));
}

void CXTPEdit::OnMouseMove(UINT nFlags, CPoint point)
{
	CRect rc;
	GetClientRect(&rc);

	BOOL bHot = rc.PtInRect(point);
	if (m_bFlatStyle && bHot != m_bHighlighted)
	{
		m_bHighlighted = bHot;
		RedrawFocusedFrame();

		if (bHot)
		{
			TRACKMOUSEEVENT tme = {sizeof(TRACKMOUSEEVENT), TME_LEAVE, m_hWnd, HOVER_DEFAULT};
			_TrackMouseEvent(&tme);
		}
	}


	TBase::OnMouseMove(nFlags, point);
}

void CXTPEdit::OnKillFocus(CWnd* pNewWnd)
{
	TBase::OnKillFocus(pNewWnd);

	m_bFocused = FALSE;
	RedrawFocusedFrame();
}

void CXTPEdit::OnSetFocus(CWnd* pOldWnd)
{
	TBase::OnSetFocus(pOldWnd);

	m_bFocused = TRUE;
	RedrawFocusedFrame();
}

void CXTPEdit::RedrawEdit()
{
	if (m_hWnd) SendMessage(WM_NCPAINT);
}
void CXTPEdit::RedrawFocusedFrame()
{
	if (m_clrBorderHot == m_clrBorderNormal && !m_bFlatStyle)
		return;

	if (!m_bShowBorder)
		return;

	RedrawEdit();
}

HBRUSH CXTPEdit::GetClientBrush(CDC* pDC)
{
	BOOL bEnabled = IsWindowEnabled() && ((GetStyle() & ES_READONLY) == 0);

	HBRUSH hBrush = (HBRUSH)::SendMessage(::GetParent(m_hWnd), !bEnabled ? WM_CTLCOLORSTATIC : WM_CTLCOLOREDIT, (WPARAM)pDC->GetSafeHdc(), (LPARAM)m_hWnd);

	if (hBrush)
		return hBrush;

	return GetSysColorBrush(!bEnabled ? COLOR_3DFACE : COLOR_WINDOW);
}

void CXTPEdit::SetUseVisualStyle(BOOL bUseVisualStyle/* = TRUE*/)
{
	m_bUseVisualStyle = bUseVisualStyle;

	RefreshMetrics();
	RedrawEdit();
}

BOOL CXTPEdit::OnChange()
{
	if (m_pRegExp)
	{
		Invalidate(FALSE);
	}
	return FALSE;
}

void CXTPEdit::SetPattern(LPCTSTR lpszPattern)
{
	SAFE_DELETE(m_pRegExp);

	if (lpszPattern && lpszPattern[0] != 0)
	{
		m_pRegExp = new CXTPRegExp();
		m_pRegExp->SetPattern(lpszPattern);
	}

	if (m_hWnd) Invalidate(FALSE);

}

void CXTPEdit::AdjustBuddyRect(CWnd* pBuddy, CRect& rcUpDown, int nAlignemnt)
{
	if (m_bFlatStyle)
		return;


	if (!m_bShowBorder && ((GetStyle() & WS_BORDER) == 0))
		return;

	if (m_bUseVisualStyle && m_themeEdit->IsAppThemed())
	{
		if (nAlignemnt == UDS_ALIGNRIGHT) rcUpDown.DeflateRect(0, 1, 1, 1) ; else rcUpDown.DeflateRect(1, 1, 0, 1);
	}
	else if (m_nTheme == xtpControlThemeDefault || m_nTheme == xtpControlThemeOffice2000)
	{
		if (nAlignemnt == UDS_ALIGNRIGHT) rcUpDown.DeflateRect(0, 2, 2, 2); else rcUpDown.DeflateRect(2, 2, 0, 2);
	}
	else if (m_nTheme == xtpControlThemeResource && DYNAMIC_DOWNCAST(CSpinButtonCtrl, pBuddy) == 0)
	{
		if (nAlignemnt == UDS_ALIGNRIGHT) rcUpDown.DeflateRect(0, 2, 2, 2); else rcUpDown.DeflateRect(2, 2, 0, 2);
	}
}

void CXTPEdit::DrawBuddyBorders(CWnd* pBuddy, CDC* pDC, CRect rc, int nAlignemnt)
{
	HBRUSH hBrush = GetClientBrush(pDC);

	if (m_bFlatStyle)
		return;

	if (!m_bShowBorder && (GetStyle() & WS_BORDER))
	{
		if (nAlignemnt == UDS_ALIGNRIGHT) rc.left -= 2; else rc.right += 2;

		pDC->Draw3dRect(rc, GetXtremeColor(COLOR_WINDOWFRAME), GetXtremeColor(COLOR_WINDOWFRAME));
		return;

	}

	if (!m_bShowBorder)
		return;

	if (m_bUseVisualStyle && m_themeEdit->IsAppThemed())
	{
		::FillRect(pDC->GetSafeHdc(), rc, hBrush);

		if (nAlignemnt == UDS_ALIGNRIGHT) rc.left -= 2; else rc.right += 2;

		if (SUCCEEDED(m_themeEdit->DrawThemeBackground(*pDC, 0, ETS_NORMAL, &rc, NULL)))
		{
			return;
		}
	}

	if (m_nTheme == xtpControlThemeDefault)
	{
		pDC->DrawEdge(rc, EDGE_SUNKEN,
			(nAlignemnt == UDS_ALIGNRIGHT) ? BF_TOP | BF_BOTTOM | BF_RIGHT : BF_TOP | BF_BOTTOM | BF_LEFT);
		return;
	}

	if (m_nTheme == xtpControlThemeOffice2000)
	{
		::FillRect(pDC->GetSafeHdc(), rc, hBrush);
		if (nAlignemnt == UDS_ALIGNRIGHT) rc.left -= 2; else rc.right += 2;

		pDC->Draw3dRect(rc, m_clrBorderNormal, GetXtremeColor(COLOR_BTNHIGHLIGHT));
		return;
	}

	if (m_nTheme == xtpControlThemeResource && DYNAMIC_DOWNCAST(CSpinButtonCtrl, pBuddy) == 0)
	{
		::FillRect(pDC->GetSafeHdc(), rc, hBrush);
		if (nAlignemnt == UDS_ALIGNRIGHT) rc.left -= 2; else rc.right += 2;

		pDC->Draw3dRect(rc, m_clrBorderNormal, m_clrBorderNormal);
		return;

	}
}

void CXTPEdit::OnSysColorChange()
{
	TBase::OnSysColorChange();

	RefreshMetrics();
	Invalidate(FALSE);

	SendMessage(WM_NCPAINT);
}
