// XTPMarkupListBox.cpp : implementation file
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

#if defined(_XTP_INCLUDE_MARKUP) && !defined(_XTP_EXCLUDE_MARKUP)

#include "Markup/XTPMarkupObject.h"
#include "Markup/XTPMarkupBuilder.h"
#include "Markup/XTPMarkupInputElement.h"
#include "Markup/XTPMarkupUIElement.h"
#include "Markup/XTPMarkupDrawingContext.h"
#include "Markup/XTPMarkupContext.h"

#include "Controls/Util/XTPControlTheme.h"
#include "Controls/ListBox/XTPListBox.h"

#include "XTPMarkupListBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTPMarkupListBox

XTP_MARKUP_LISTITEM::XTP_MARKUP_LISTITEM(CXTPMarkupContext *pContext)
{
	this->pItem = NULL;
	this->pContext = pContext;
}

XTP_MARKUP_LISTITEM::~XTP_MARKUP_LISTITEM()
{
	MARKUP_RELEASE(pItem);
}


void XTP_MARKUP_LISTITEM::Resolve(LPCTSTR lpszMarkup)
{
	strMarkup = lpszMarkup;

	MARKUP_RELEASE(pItem);
	pItem = pContext->Parse(strMarkup);
}

CXTPMarkupListBox::CXTPMarkupListBox()
{
}

CXTPMarkupListBox::~CXTPMarkupListBox()
{
}

void CXTPMarkupListBox::Init()
{
	// Don't call base class.
}

BEGIN_MESSAGE_MAP(CXTPMarkupListBox, CXTPListBox)
	//{{AFX_MSG_MAP(CXTPMarkupListBox)
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTPMarkupListBox message handlers

void CXTPMarkupListBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	XTP_MARKUP_LISTITEM *pListItem =
		reinterpret_cast<XTP_MARKUP_LISTITEM*>(lpDrawItemStruct->itemData);

	if (NULL == pListItem)
		return;

	if (NULL == pListItem->pItem)
		return;

	CXTPMarkupDrawingContext dc(lpDrawItemStruct->hDC);

	if (((LONG)(lpDrawItemStruct->itemID) >= 0) &&
		(lpDrawItemStruct->itemAction & (ODA_DRAWENTIRE | ODA_SELECT)))
	{

		if (((lpDrawItemStruct->itemState & ODS_SELECTED) != 0))
		{
			dc.FillSolidRect(&lpDrawItemStruct->rcItem, 0xc9afa6);
		}
		else
		{
			dc.FillSolidRect(&lpDrawItemStruct->rcItem, GetSysColor(COLOR_WINDOW));
		}

		pListItem->pItem->Arrange(lpDrawItemStruct->rcItem);

		pListItem->pItem->Render(&dc);
	}

	if ((lpDrawItemStruct->itemAction & ODA_FOCUS) != 0)
	{
		::SetTextColor(lpDrawItemStruct->hDC, 0);
		::SetBkColor(lpDrawItemStruct->hDC,0xFFFFFF);
		::DrawFocusRect(lpDrawItemStruct->hDC, &lpDrawItemStruct->rcItem);
	}

}

void CXTPMarkupListBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	XTP_MARKUP_LISTITEM *pListItem = (XTP_MARKUP_LISTITEM*)lpMeasureItemStruct->itemData;
	if (!pListItem)
		return;

	if (!pListItem->pItem)
		return;

	CRect rc;
	GetClientRect(rc);

	CXTPMarkupDrawingContext dc;
	pListItem->pItem->Measure(&dc, CSize(rc.Width(), rc.Height()));

	lpMeasureItemStruct->itemHeight = pListItem->pItem->GetDesiredSize().cy;
}

void CXTPMarkupListBox::DeleteItem(LPDELETEITEMSTRUCT lpDeleteItemStruct)
{
	delete (XTP_MARKUP_LISTITEM*)lpDeleteItemStruct->itemData;

	CXTPListBox::DeleteItem(lpDeleteItemStruct);
}

void CXTPMarkupListBox::OnSize(UINT nType, int cx, int cy)
{
	CXTPListBox::OnSize(nType, cx, cy);

	CRect rc;
	GetClientRect(rc);
	CXTPMarkupDrawingContext dc;

	for (int i = 0; i < GetCount(); i++)
	{
		XTP_MARKUP_LISTITEM *pListItem = (XTP_MARKUP_LISTITEM*)GetItemDataPtr(i);
		if (!pListItem)
			continue;

		if (!pListItem->pItem)
			continue;

		pListItem->pItem->Measure(&dc, CSize(rc.Width(), rc.Height()));
		SetItemHeight(i, pListItem->pItem->GetDesiredSize().cy);
	}

}

void CXTPMarkupListBox::ItemChanged(XTP_MARKUP_LISTITEM* pItem)
{
	CRect rc;
	GetClientRect(rc);

	for (int i = 0; i < GetCount(); i++)
	{
		XTP_MARKUP_LISTITEM *pListItem = (XTP_MARKUP_LISTITEM*)GetItemDataPtr(i);
		if (pListItem == pItem)
		{
			CXTPMarkupDrawingContext dc;
			pListItem->pItem->Measure(&dc, CSize(rc.Width(), rc.Height()));
			SetItemHeight(i, pListItem->pItem->GetDesiredSize().cy);
			break;
		}
	}
	Invalidate(FALSE);
}

void CXTPMarkupListBox::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CXTPListBox::OnLButtonDown(nFlags, point);
}

void CXTPMarkupListBox::AddMarkupString(LPCTSTR lpszMarkup)
{
	XTP_MARKUP_LISTITEM *pItem = new XTP_MARKUP_LISTITEM(this);
	if (pItem)
	{
		pItem->Resolve(lpszMarkup);
		AddString((LPCTSTR)pItem);
	}
}

#define LB_MSGMIN LB_ADDSTRING // First listbox message

BOOL CXTPMarkupListBox::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	if ((NULL != m_hWnd) && ((LB_MSGMIN > message) || (LB_MSGMAX < message)))
	{
		if (-1 != m_nHotItem)
		{
			XTP_MARKUP_LISTITEM *pItem = reinterpret_cast<XTP_MARKUP_LISTITEM*>(GetItemDataPtr(m_nHotItem));

			// To Handle Hyperlinks:
			if (pItem->pItem)
			{
				m_hContextWnd = m_hWnd;

				if (CXTPMarkupContext::OnWndMsg(pItem->pItem, message, wParam, lParam, pResult))
					return TRUE;
			}
		}
	}

	return CXTPListBox::OnWndMsg(message, wParam, lParam, pResult);
}

#endif
