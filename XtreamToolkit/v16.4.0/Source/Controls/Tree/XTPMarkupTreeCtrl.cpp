// XTPMarkupTreeCtrl.cpp : implementation file
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

#if defined(_XTP_INCLUDE_MARKUP) && !defined(_XTP_EXCLUDE_MARKUP)

#include "Markup/XTPMarkupObject.h"
#include "Markup/XTPMarkupBuilder.h"
#include "Markup/XTPMarkupInputElement.h"
#include "Markup/XTPMarkupUIElement.h"
#include "Markup/XTPMarkupDrawingContext.h"
#include "Markup/XTPMarkupContext.h"

#include "Common/XTPToolTipContext.h"

#include "Controls/Tree/XTPTreeBase.h"
#include "Controls/Tree/XTPTreeCtrlView.h"


#include "XTPMarkupTreeCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTPMarkupTreeCtrl

CXTPMarkupTreeCtrl::CXTPMarkupTreeCtrl()
{
}

CXTPMarkupTreeCtrl::~CXTPMarkupTreeCtrl()
{
}


BEGIN_MESSAGE_MAP(CXTPMarkupTreeCtrl, CXTPTreeCtrl)
	//{{AFX_MSG_MAP(CXTPMarkupTreeCtrl)
	ON_NOTIFY_REFLECT(TVN_DELETEITEM,  OnDeleteItem)
	ON_NOTIFY_REFLECT(TVN_GETINFOTIP,  OnGetInfoTip)
	ON_NOTIFY_REFLECT(TVN_GETDISPINFO, OnGetDispInfo)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW,   OnCustomDraw)
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_PAINT()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTPMarkupTreeCtrl message handlers

int CXTPMarkupTreeCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	CXTPTreeCtrl::OnCreate(lpCreateStruct);

	ModifyStyle(TVS_NOTOOLTIPS, TVS_INFOTIP); // Enable TVN_GETINFOTIP


	return 0;
}

void CXTPMarkupTreeCtrl::PreSubclassWindow()
{
	CXTPTreeCtrl::PreSubclassWindow();

	ModifyStyle(TVS_NOTOOLTIPS, TVS_INFOTIP); // Enable TVN_GETINFOTIP
}


HTREEITEM CXTPMarkupTreeCtrl::AddItem(LPCTSTR lpszItem, int nImage, int nSelectedImage, HTREEITEM hParent/* = TVI_ROOT*/, HTREEITEM hInsertAfter/* = TVI_LAST*/)
{
	CXTPMarkupUIElement* pUIElement = Parse(lpszItem);

	HTREEITEM hItem;

	if (pUIElement)
	{
		CPaintDC paintDC(this);
		CXTPMarkupDrawingContext dc(paintDC.m_hDC);

		pUIElement->Measure(&dc, CSize(INT_MAX, GetItemHeight()));
		CSize sz = pUIElement->GetDesiredSize();

		CString sText;
		for (int nChar=0; nChar<sz.cx/3; nChar++)
		{
			sText += _T(" ");
		}

		hItem = InsertItem(sText, nImage, nSelectedImage, hParent, hInsertAfter);
		SetItemData(hItem, (DWORD_PTR)pUIElement);
	}
	else
	{
		hItem = InsertItem(lpszItem, nImage, nSelectedImage, hParent, hInsertAfter);
	}

	return hItem;
}

void CXTPMarkupTreeCtrl::DrawMarkupItem(NMTVCUSTOMDRAW* lpLVCD)
{
	HTREEITEM hItem = (HTREEITEM)lpLVCD->nmcd.dwItemSpec;

	CXTPMarkupUIElement* pUIElement = (CXTPMarkupUIElement*)lpLVCD->nmcd.lItemlParam;
	CRect rcItem(lpLVCD->nmcd.rc);

	SetDefaultTextColor(lpLVCD->clrText);
	SetDefaultFont((HFONT)SendMessage(WM_GETFONT));

	CXTPMarkupDrawingContext dc(lpLVCD->nmcd.hdc);
	pUIElement->Measure(&dc, CSize(INT_MAX, rcItem.Height()));

	CRect rcText;
	GetItemRect(hItem, rcText, TRUE);
	int x = rcText.left, y = rcItem.top;

	int cxMargin = GetSystemMetrics(SM_CXEDGE);
	CRect rcRect(x, y, x + cxMargin * 2 + pUIElement->GetDesiredSize().cx, y + rcItem.Height());
	CRect rcRender(CPoint(x + cxMargin, max(y, (rcItem.top + rcItem.bottom - pUIElement->GetDesiredSize().cy) / 2)), pUIElement->GetDesiredSize());

	::SetBkColor(lpLVCD->nmcd.hdc, lpLVCD->clrTextBk);
	::SetTextColor(lpLVCD->nmcd.hdc, lpLVCD->clrText);
	::ExtTextOut(lpLVCD->nmcd.hdc, 0, 0, ETO_OPAQUE, rcRect, NULL, 0, NULL);

	if (lpLVCD->nmcd.uItemState & CDIS_FOCUS)
	{
		DrawFocusRect(lpLVCD->nmcd.hdc, &rcRect);
	}

	pUIElement->Arrange(rcRender);

	pUIElement->Render(&dc);
}

void CXTPMarkupTreeCtrl::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMTVCUSTOMDRAW* lpLVCD = reinterpret_cast<NMTVCUSTOMDRAW*>(pNMHDR);
	*pResult = CDRF_DODEFAULT;

	switch (lpLVCD->nmcd.dwDrawStage)
	{
	case CDDS_PREPAINT:
		*pResult = CDRF_NOTIFYITEMDRAW;
		break;

	case CDDS_PREPAINT | CDDS_ITEM:
		{
			//HTREEITEM hItem = (HTREEITEM)lpLVCD->nmcd.dwItemSpec;

			if (lpLVCD->nmcd.lItemlParam != NULL)
			{
				*pResult |= CDRF_NOTIFYPOSTPAINT;
			}

			if (lpLVCD->clrTextBk == GetSysColor(COLOR_HIGHLIGHT))
			{
				lpLVCD->clrText = 0;
				lpLVCD->clrTextBk = 0xc9afa6;
			}

			break;
		}

	case CDDS_POSTPAINT | CDDS_ITEM:
	case CDDS_POSTPAINT | CDDS_ITEM  | CDDS_SUBITEM:
		{
			//HTREEITEM hItem = (HTREEITEM)lpLVCD->nmcd.dwItemSpec;

			if (lpLVCD->nmcd.lItemlParam != NULL)
			{
				DrawMarkupItem(lpLVCD);
			}

			break;
		}
	}
}

void CXTPMarkupTreeCtrl::OnDeleteItem(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	if (pNMTreeView->itemOld.lParam)
	{
		((CXTPMarkupUIElement*)(pNMTreeView->itemOld.lParam))->Release();
	}
	*pResult = 0;
}

void CXTPMarkupTreeCtrl::OnGetInfoTip(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMTVGETINFOTIP *pNMGetInfoTip = reinterpret_cast<NMTVGETINFOTIP*>(pNMHDR);

	CXTPMarkupUIElement *pElement =
		reinterpret_cast<CXTPMarkupUIElement*>(GetItemData(pNMGetInfoTip->hItem));

	if (NULL != pElement)
	{
		pNMGetInfoTip->pszText = _T("");
	}

	*pResult = 0;
}

void CXTPMarkupTreeCtrl::OnGetDispInfo(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMTVDISPINFO *pDispInfo = reinterpret_cast<NMTVDISPINFO*>(pNMHDR);
	pDispInfo;

	*pResult = 0;
}


HTREEITEM CXTPMarkupTreeCtrl::HitTestMarkupItem(CPoint point)
{
	UINT nFlags;
	HTREEITEM hItem = HitTest(point, &nFlags);
	if (hItem && (nFlags & TVHT_ONITEMRIGHT))
	{
		CXTPMarkupUIElement* pElement = (CXTPMarkupUIElement*)GetItemData(hItem);
		if (!pElement)
			return NULL;

		if (pElement->GetFinalRect().PtInRect(point))
			return hItem;
	}

	return NULL;
}

void CXTPMarkupTreeCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	HTREEITEM hItem = HitTestMarkupItem(point);
	if (hItem)
	{
		SelectItem(hItem);
	}

	CXTPTreeCtrl::OnLButtonDown(nFlags, point);
}

void CXTPMarkupTreeCtrl::OnRButtonDown(UINT nFlags, CPoint point)
{
	HTREEITEM hItem = HitTestMarkupItem(point);
	if (hItem)
	{
		SelectItem(hItem);
		return;
	}

	CXTPTreeCtrl::OnRButtonDown(nFlags, point);
}


void CXTPMarkupTreeCtrl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	HTREEITEM hItem = HitTestMarkupItem(point);
	if (hItem)
	{
		Expand(hItem, TVE_TOGGLE);
	}

	CXTPTreeCtrl::OnLButtonDblClk(nFlags, point);
}

void CXTPMarkupTreeCtrl::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	DoPaint(dc, FALSE);
}

#define TV_MSGMAX TV_FIRST + 65 // last tree view message

BOOL CXTPMarkupTreeCtrl::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	if ((NULL != m_hWnd) && ((TV_FIRST > message) || (TV_MSGMAX < message)))
	{
		m_hContextWnd = m_hWnd;

		CPoint ptMouse(0);
		GetCursorPos(&ptMouse);
		ScreenToClient(&ptMouse);

		UINT nFlags;
		HTREEITEM hItem = HitTest(ptMouse, &nFlags);

		if (NULL != hItem)
		{
			CXTPMarkupUIElement *pUIElement = reinterpret_cast<CXTPMarkupUIElement*>(GetItemData(hItem));

			if (pUIElement && CXTPMarkupContext::OnWndMsg(pUIElement, message, wParam, lParam, pResult))
				return TRUE;
		}
	}

	return CXTPTreeCtrl::OnWndMsg(message, wParam, lParam, pResult);
}

#endif
