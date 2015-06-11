// XTPControlComboBoxList.cpp : implementation of the CXTPControlComboBoxList class.
//
// This file is a part of the XTREME COMMANDBARS MFC class library.
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
#include <Common/XTPPropExchange.h>
#include <Common/XTPImageManager.h>
#include <Common/XTPToolTipContext.h>
#include <Common/XTPSystemHelpers.h>
#include <Common/XTPHookManager.h>
#include <Common/ScrollBar/XTPScrollInfo.h>
#include "Common/XTPResourceManager.h"

#include <CommandBars/XTPCommandBarsDefines.h>
#include <CommandBars/XTPControl.h>
#include <CommandBars/XTPControlButton.h>
#include <CommandBars/XTPControlPopup.h>
#include <CommandBars/XTPCommandBar.h>
#include <CommandBars/XTPPopupBar.h>
#include <CommandBars/XTPControlEdit.h>
#include <CommandBars/XTPControlComboBox.h>
#include <CommandBars/XTPCommandBars.h>
#include <CommandBars/XTPPaintManager.h>
#include <CommandBars/XTPMouseManager.h>
#include <CommandBars/XTPKeyboardManager.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//////////////////////////////////////////////////////////////////////////
// CXTPControlComboBoxList

CXTPControlComboBoxList::CXTPControlComboBoxList()
{
	m_barPosition = xtpBarListBox;
	m_nMRUWidth = 0;
	m_bGrabFocus = FALSE;
	m_dwStyle = 0;
	m_nListIconId = -1;
}

BEGIN_MESSAGE_MAP(CXTPControlComboBoxList, CXTPPopupBar)
	ON_WM_LBUTTONUP()
	ON_WM_NCPAINT()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_DRAWITEM_REFLECT()
	ON_WM_MEASUREITEM_REFLECT()
END_MESSAGE_MAP()


IMPLEMENT_XTP_COMMANDBAR(CXTPControlComboBoxList, CXTPControlComboBoxPopupBar)


void CXTPControlComboBoxList::CreateListBox()
{
	DWORD dwStyle = m_dwStyle & ~CBRS_GRIPPER;

	CWnd* pSite = GetSite();

	if (pSite && ((dwStyle & (LBS_OWNERDRAWFIXED|LBS_OWNERDRAWVARIABLE)) != 0))
	{
		CreateEx(WS_EX_CLIENTEDGE|WS_EX_TOOLWINDOW, _T("LISTBOX"), _T(""),
			WS_CHILD | WS_VSCROLL | WS_CLIPCHILDREN | dwStyle | LBS_NOTIFY,
			CRect(0, 0, 0, 50), pSite, 0);

		SetWindowLongPtr(m_hWnd, GWLP_HWNDPARENT, 0 );
		ModifyStyle(WS_CHILD, WS_POPUP);
		SetWindowLongPtr(m_hWnd, GWLP_HWNDPARENT, (LONG_PTR)(pSite->m_hWnd));
	}
	else
	{
		CreateEx(WS_EX_CLIENTEDGE | WS_EX_TOOLWINDOW, _T("LISTBOX"), _T(""),
			WS_POPUP | WS_VSCROLL | WS_CLIPCHILDREN | dwStyle, CRect(0, 0, 0, 50), 0, 0);
	}

	if (m_pCommandBars && m_nListIconId != -1)
	{
		CXTPImageManagerIcon* pIcon = GetImageManager()->GetImage(m_nListIconId, 0);
		if (pIcon)
		{
			((CListBox*)this)->SetItemHeight(-1, pIcon->GetHeight() + 4);
		}
	}
}

void CXTPControlComboBoxList::DrawItem (LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CXTPControlComboBox* pComboBox = (CXTPControlComboBox*)m_pControlPopup;
	if (pComboBox) pComboBox->DrawItem(lpDrawItemStruct);
}

void CXTPControlComboBoxList::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	CXTPControlComboBox* pComboBox = (CXTPControlComboBox*)m_pControlPopup;
	if (pComboBox) pComboBox->MeasureItem(lpMeasureItemStruct);
}

void CXTPControlComboBoxList::OnLButtonDown(UINT /*nFlags*/, CPoint /*point*/)
{
	// do nothing.
}

int CXTPControlComboBoxList::OnHookMessage(HWND hWnd, UINT nMessage, WPARAM& wParam, LPARAM& lParam, LRESULT& lResult)
{
	if (nMessage == WM_MOUSEWHEEL)
	{
		CWnd::DefWindowProc(WM_MOUSEWHEEL, wParam, lParam);
		return TRUE;
	}
	return CXTPCommandBar::OnHookMessage(hWnd, nMessage, wParam, lParam, lResult);
}


void CXTPControlComboBoxList::OnNcPaint()
{
	CXTPPaintManager* pPaintManager = GetPaintManager();

	Default();
	CWindowDC dc(this);
	CXTPWindowRect rc(this);
	rc.OffsetRect(-rc.TopLeft());
	dc.Draw3dRect(rc, pPaintManager->GetXtremeColor(XPCOLOR_MENUBAR_BORDER), pPaintManager->GetXtremeColor(XPCOLOR_MENUBAR_BORDER));

	rc.DeflateRect(1, 1);
	dc.Draw3dRect(rc, GetXtremeColor(COLOR_WINDOW), GetXtremeColor(COLOR_WINDOW));

}

BOOL CXTPControlComboBoxList::OnEraseBkgnd(CDC* pDC)
{
	return CWnd::OnEraseBkgnd(pDC);
}

void CXTPControlComboBoxList::OnLButtonUp(UINT /*nFlags*/, CPoint /*point*/)
{
	CXTPControlComboBox* pComboBox = (CXTPControlComboBox*)m_pControlPopup;

	pComboBox->OnSelChanged();
	pComboBox->OnExecute();
}

BOOL CXTPControlComboBoxList::SetTrackingMode(int bMode, BOOL bSelectFirst, BOOL bKeyboard)
{
	if (!CXTPCommandBar::SetTrackingMode(bMode, bSelectFirst, bKeyboard))
		return FALSE;

	CXTPControlComboBox* pComboBox = ((CXTPControlComboBox*)m_pControlPopup);

	if (!bMode)
	{
		pComboBox->NotifySite(CBN_CLOSEUP);

		XTPMouseManager()->UnlockMouseMove();
		ShowWindow(SW_HIDE);

		if (GetParentCommandBar())
			GetParentCommandBar()->SetPopuped(-1);

		m_pControlPopup = 0;

	}
	else
	{
		XTPMouseManager()->LockMouseMove();

		pComboBox->UpdatePopupSelection();

		pComboBox->NotifySite(CBN_DROPDOWN);
	}


	return TRUE;
}

CListBox* CXTPControlComboBoxList::GetListBoxCtrl() const
{
	return (CListBox*)this;
}

BOOL CXTPControlComboBoxList::ProcessHookKeyDown(CXTPControlComboBox* pComboBox, UINT nChar, LPARAM lParam)
{
	int nSel = GetCurSel();

	if (pComboBox->GetEditCtrl())
	{
		if ((GetKeyState(VK_MENU) >= 0) && (nChar == VK_UP || nChar == VK_DOWN || nChar == VK_PRIOR || nChar == VK_NEXT))
		{
			CWnd::DefWindowProc(WM_KEYDOWN, (WPARAM)nChar, lParam);
		}
		else
		{
			return FALSE;
		}
	}
	else
	{
		if ((nChar == VK_F4 && GetKeyState(VK_MENU) >= 0) || ((nChar == VK_UP || nChar == VK_DOWN) && GetKeyState(VK_MENU) < 0))
		{
			return FALSE;
		}
		else
		{
			MSG msg;
			msg.message = WM_KEYDOWN;
			msg.hwnd = m_hWnd;
			msg.wParam = (WPARAM)nChar;
			msg.lParam = lParam;

			::IsDialogMessage(m_hWnd, &msg);

			if (PeekMessage(&msg, m_hWnd, WM_CHAR, WM_CHAR, PM_REMOVE))
			{
				CWnd::DefWindowProc(WM_CHAR, msg.wParam, msg.lParam);
			}
		}
	}

	if (nSel != GetCurSel())
	{
		pComboBox->OnSelChanged();
		if (pComboBox->GetEditCtrl()->GetSafeHwnd()) pComboBox->GetEditCtrl()->SetSel(0, -1);
	}
	return TRUE;
}

BOOL CXTPControlComboBoxList::OnHookKeyDown(UINT nChar, LPARAM lParam)
{
	ASSERT(m_pControlPopup);
	CXTPControlComboBox* pComboBox = ((CXTPControlComboBox*)m_pControlPopup);
	if (!pComboBox)
		return CXTPCommandBar::OnHookKeyDown(nChar, lParam);

	if (pComboBox->GetEditCtrl() && pComboBox->GetEditCtrl()->IsImeMode())
		return ProcessHookKeyDown(pComboBox, nChar, lParam);

	if (nChar == VK_ESCAPE)
	{
		if (pComboBox->IsFocused())
			return FALSE;

		return CXTPCommandBar::OnHookKeyDown(nChar, lParam);
	}

	if (nChar == VK_TAB) return FALSE;

	if (nChar == VK_RETURN)
	{
		m_pControlPopup->OnExecute();
		return TRUE;
	}

	return ProcessHookKeyDown(pComboBox, nChar, lParam);
}

void CXTPControlComboBoxList::OnMouseMove(UINT nFlags, CPoint point)
{
	BOOL bOutside;
	UINT nItem = GetListBoxCtrl()->ItemFromPoint(point, bOutside);

	if (!bOutside && GetCurSel() != (int)nItem)
		SetCurSel(nItem);

	CXTPPopupBar::OnMouseMove(nFlags, point);
}



BOOL CXTPControlComboBoxList::Popup(CXTPControlPopup* pControlPopup, BOOL /*bSelectFirst*/)
{
	m_pControlPopup = pControlPopup;

	SetFont(GetPaintManager()->GetIconFont());

	CWnd* pWndOwner = GetOwnerSite();
	if (pWndOwner) pWndOwner->SendMessage(WM_XTP_INITCOMMANDSPOPUP, 0, (LPARAM)this);

	CXTPCommandBars* pCommandBars = GetCommandBars();

	if (pCommandBars && pCommandBars->IsLayoutRTL())
		ModifyStyleEx(WS_EX_LAYOUTRTL, WS_EX_RIGHT | WS_EX_LEFTSCROLLBAR | WS_EX_RTLREADING);
	else ModifyStyleEx(WS_EX_RIGHT | WS_EX_LEFTSCROLLBAR | WS_EX_RTLREADING, 0);

	SetTrackingMode(TRUE, FALSE);

	UpdateFlags();

	CRect rcControl = pControlPopup->GetRect();

	int nItemHeight = GetListBoxCtrl()->GetItemHeight(0);
	int nHeight = min (((CXTPControlComboBox*)m_pControlPopup)->m_nDropDownItemCount,
		max(1, GetListBoxCtrl()->GetCount())) * nItemHeight + 4;
	int nWidth = m_nMRUWidth <= 0 ? rcControl.Width() : m_nMRUWidth;
	UpdateLocation(CSize(nWidth, nHeight));

	EnableWindow();

	return TRUE;
}

void CXTPControlComboBoxList::OnPaint()
{
	CPaintDC paintDC(this);
	CXTPBufferDC memDC(paintDC);

	DrawCommandBar(&memDC, CXTPClientRect(this));
}

void CXTPControlComboBoxList::DrawCommandBar(CDC* pDC, CRect /*rcClipBox*/)
{
	pDC->FillSolidRect(CXTPClientRect(this), GetXtremeColor(COLOR_WINDOW));
	CWnd::DefWindowProc(WM_PAINT, (WPARAM)pDC->m_hDC, 0);
}

void CXTPControlComboBoxList::Copy(CXTPCommandBar* pCommandBar, BOOL bRecursive)
{
	CXTPPopupBar::Copy(pCommandBar, bRecursive);

	m_nListIconId = ((CXTPControlComboBoxList*)pCommandBar)->m_nListIconId;

	if (!m_hWnd)
		CreateListBox();

	CListBox* pListBox = (CListBox*)(CXTPControlComboBoxList*)pCommandBar;
	DWORD dwCount = pListBox->m_hWnd ? pListBox->GetCount() : 0;

	for (UINT i = 0; i < dwCount; i++)
	{
		CString str;
		pListBox->GetText(i, str);
		DWORD_PTR dwData = pListBox->GetItemData(i);

		int nIndex = GetListBoxCtrl()->AddString(str);
		GetListBoxCtrl()->SetItemData(nIndex, dwData);
	}
}

void CXTPControlComboBoxList::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPPopupBar::DoPropExchange(pPX);

	if (pPX->GetSchema() > _XTP_SCHEMA_110)
	{
		PX_Int(pPX, _T("ListIconId"), m_nListIconId, -1);
	}

	CListBox* pThis = (CListBox*)this;
	CString str;

	CXTPPropExchangeSection secItems(pPX->GetSection(_T("Items")));

	if (pPX->IsStoring())
	{
		if (m_hWnd)
		{
			DWORD dwCount = pThis->GetCount();
			secItems->WriteCount(dwCount);

			for (int i = 0; i < (int)dwCount; i++)
			{
				pThis->GetText(i, str);

				CString strSection;
				strSection.Format(_T("Item%i"), i);
				PX_String(&secItems, strSection, str, _T(""));

				if (pPX->GetSchema() >= _XTP_SCHEMA_1340)
				{
					int dwItemData = (int)pThis->GetItemData(i);

					strSection.Format(_T("ItemData%i"), i);
					PX_Int(&secItems, strSection, dwItemData, 0);
				}
			}
		}
		else
		{
			pPX->WriteCount(0);
		}
	}
	else
	{
		if (!m_hWnd)
			CreateListBox();

		int nNewCount = (int)secItems->ReadCount();
		for (int i = 0; i < nNewCount; i++)
		{
			CString strSection;
			strSection.Format(_T("Item%i"), i);

			PX_String(&secItems, strSection, str, _T(""));
			int nIndex = pThis->AddString(str);

			if (pPX->GetSchema() >= _XTP_SCHEMA_1340)
			{
				int dwItemData = 0;

				strSection.Format(_T("ItemData%i"), i);
				PX_Int(&secItems, strSection, dwItemData, 0);

				pThis->SetItemData(nIndex, dwItemData);
			}
		}
	}
}
