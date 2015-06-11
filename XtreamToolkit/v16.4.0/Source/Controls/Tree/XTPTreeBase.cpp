// XTPTreeBase.cpp : implementation file
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
#include "Common/XTPSystemHelpers.h"
#include <Common/XTPWinThemeWrapper.h>

#include "../Util/XTPControlTheme.h"
#include "XTPTreeTheme.h"

#include "XTPTreeBase.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifndef SCROLL_TIMER_PERIOD
#define SCROLL_TIMER_PERIOD 75
#endif

#ifndef HOVER_TIMER_PERIOD
#define HOVER_TIMER_PERIOD  85
#endif

#ifndef COLOR_HOTLIGHT
#define COLOR_HOTLIGHT      26
#endif // COLOR_HOTLIGHT

#define XTP_TREE_ICON_IDENT 3

/////////////////////////////////////////////////////////////////////////////
// CXTPTreeBase

CXTPTreeBase::CXTPTreeBase()
: m_hSelect(NULL)
, m_bMultiSelect(false)
, m_bBandLabel(true)
, m_bActionDone(false)
, m_bOkToEdit(true)
, m_htiEdit(NULL)
, m_htiLast(NULL)
, m_pTheme (NULL)
, m_bExplorerTheme(FALSE)
{
	m_pTreeCtrl = NULL;

	m_hBitmapPlus = NULL;
	m_hBitmapMinus = NULL;

	m_pWindowsTreeTheme = new CXTPWinThemeWrapper();

	SetTheme(xtpControlThemeDefault);
}

CXTPTreeBase::~CXTPTreeBase()
{
	CMDTARGET_RELEASE(m_pTheme);

	SAFE_DELETE(m_pWindowsTreeTheme);

	if (m_hBitmapPlus)
		::DeleteObject(m_hBitmapPlus);

	if (m_hBitmapMinus)
		::DeleteObject(m_hBitmapMinus);
}

/////////////////////////////////////////////////////////////////////////////
// CXTPTreeBase message handlers

void CXTPTreeBase::OnLButtonDown(UINT nFlags, CPoint point)
{
	// If multiselect control, process possible left
	// click drag selection.
	if (m_bMultiSelect)
	{
		UINT nHitFlags = 0;

		HTREEITEM hItemHit = m_pTreeCtrl->HitTest(point, &nHitFlags);
		HTREEITEM hItemSel = m_pTreeCtrl->GetSelectedItem();

		// if expanding/contracting call base class.
		if ((nHitFlags & (TVHT_ONITEMBUTTON | TVHT_ONITEMSTATEICON)) != 0)
		{
			m_pTreeCtrl->Default();
			return;
		}

		if (HasEditLabels())
		{
			if (!(nFlags & (MK_CONTROL | MK_SHIFT)) && (nHitFlags & TVHT_ONITEMLABEL))
			{
				//For begin edit of label need have only one selected item.
				UINT nSelected = GetSelectedCount();
				if (m_bOkToEdit && (hItemHit == hItemSel) && (nSelected == 1))
				{
					SelectAll(FALSE);
					SelectItem(hItemHit);

					m_pTreeCtrl->Default();
					return;
				}
			}

			if ((nHitFlags & TVHT_ONITEM) == 0)
			{
				m_bOkToEdit = false;
			}
			else if (GetFocusedItem() == hItemSel)
			{
				m_bOkToEdit = true;
			}
		}

		OnButtonDown(TRUE, nFlags, point);
	}

	else
	{
		m_pTreeCtrl->Default();
	}
}

void CXTPTreeBase::OnRButtonDown(UINT /*nFlags*/, CPoint point)
{
	if (m_pTreeCtrl->GetStyle() & TVS_SINGLEEXPAND)
	{
		m_pTreeCtrl->Default();
		return;
	}


	// hittest to get the tree item under the cursor
	// and select it.
	UINT uFlags = 0;
	HTREEITEM hItem = m_pTreeCtrl->HitTest(point, &uFlags);
	if (hItem != NULL && (uFlags & TVHT_ONITEM) != 0)
	{
		// if the item is not selected, clear previous
		// selections and select the item under cursor.
		if (!IsSelected(hItem))
		{
			SelectAll(FALSE);
			SelectItem(hItem);
		}
	}
	else
	{
		// clear previous selections.
		SelectAll(FALSE);
	}

	// call Default() so correct notification messages are
	// sent such as TVN_BEGINRDRAG.
	m_pTreeCtrl->Default();

	// get the owner of the tree control.
	//HWND hWnd = m_pTreeCtrl->GetOwner()->m_hWnd;

	//if (::IsWindow(hWnd))
	//{
		// construct a NMHDR struct...
		//NMHDR mHDR;
		//mHDR.hwndFrom = m_pTreeCtrl->m_hWnd;
		//mHDR.code = NM_RCLICK;
		//mHDR.idFrom = m_pTreeCtrl->GetDlgCtrlID();

		// and send a WM_NOTIFY message to our owner.
		//SendNotify(&mHDR);
	//}
}

void CXTPTreeBase::OnSetFocus(CWnd* /*pOldWnd*/)
{
	if (m_bMultiSelect)
	{
		//'emulated' selected items will remain greyed
		// if application gets covered
		HTREEITEM hItem = GetFirstSelectedItem();
		while (hItem)
		{
			RECT rect;
			m_pTreeCtrl->GetItemRect(hItem, &rect, TRUE);
			m_pTreeCtrl->InvalidateRect(&rect);
			hItem = GetNextSelectedItem(hItem);
		}
	}

	m_pTreeCtrl->Default();
}

void CXTPTreeBase::OnKillFocus(CWnd* /*pNewWnd*/)
{
	m_pTreeCtrl->Default();

	if (m_bMultiSelect)
	{
		//'emulated' selected items may not get
		// refreshed to grey
		HTREEITEM hItem = GetFirstSelectedItem();
		while (hItem)
		{
			RECT rect;
			m_pTreeCtrl->GetItemRect(hItem, &rect, TRUE);
			m_pTreeCtrl->InvalidateRect(&rect);
			hItem = GetNextSelectedItem(hItem);
		}
	}
}

AFX_INLINE BOOL IsArrowKey(UINT nChar)
{
	return (nChar == VK_UP) || (nChar == VK_DOWN) || (nChar == VK_PRIOR) || (nChar == VK_NEXT) || (nChar == VK_HOME) || (nChar == VK_END) ||
		(nChar == VK_LEFT) || (nChar == VK_RIGHT) || (nChar == VK_BACK) || (nChar == VK_ADD) || (nChar == VK_MULTIPLY) || (nChar == VK_SUBTRACT);
}

void CXTPTreeBase::OnKeyDown(UINT nChar, UINT /*nRepCnt*/, UINT /*nFlags*/)
{
	if (!m_bMultiSelect)
	{
		m_pTreeCtrl->Default();
		return;
	}

	BOOL bCtrl = (::GetKeyState(VK_CONTROL) < 0);
	BOOL bShift = (::GetKeyState(VK_SHIFT) < 0);


	if (!bCtrl && !bShift && IsArrowKey(nChar))
	{
		UINT uCount = GetSelectedCount();

		if ((uCount > 1) || (uCount == 1 && !IsSelected(GetFocusedItem())))
			SelectAll(FALSE);
	}

	HTREEITEM hSel = NULL;
	switch (nChar)
	{
	case VK_UP:
	case VK_DOWN:
	case VK_PRIOR:
	case VK_NEXT:
		hSel = m_pTreeCtrl->GetSelectedItem();
		if (!m_hSelect)
		{
			m_hSelect = hSel;
		}
		if (!bCtrl && !bShift)
		{
			m_hSelect = NULL;   //reset
		}
		break;
	}

	BOOL bDir = (nChar == VK_UP) || (nChar == VK_PRIOR);

	m_pTreeCtrl->Default();
	if (!hSel || (!bCtrl && !bShift))
	{
		return;
	}

	HTREEITEM hNext = (nChar == VK_NEXT) || (nChar == VK_PRIOR) ? GetFocusedItem() :
		bDir ? m_pTreeCtrl->GetPrevVisibleItem(hSel) : m_pTreeCtrl->GetNextVisibleItem(hSel);

	if (!hNext)
	{
		hNext = hSel;
	}
	if (bShift)
	{
		SelectItems(m_hSelect, hNext, TRUE);
	}
	else if (bCtrl)
	{
		SetItemState(hNext, TVIS_FOCUSED, TVIS_FOCUSED);
	}
}

BOOL CXTPTreeBase::OnBeginLabelEdit(NMHDR* pNMHDR, LRESULT* pResult)
{
	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;
	*pResult = 0;

	// set m_htiEdit equal to pTVDispInfo->item.hItem.
	m_htiEdit = pTVDispInfo->item.hItem;

	return FALSE;   //pass to parent
}

BOOL CXTPTreeBase::OnEndLabelEdit(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
	*pResult = 0;

	// set m_htiEdit equal to NULL.
	m_htiEdit = NULL;

	return FALSE;   //pass to parent
}

BOOL CXTPTreeBase::OnItemExpanding(NMHDR* pNMHDR, LRESULT* pResult)
{
	if (!m_bMultiSelect)
		return FALSE;

	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	*pResult = 0;

	if ((pNMTreeView->action == TVE_COLLAPSE) || (pNMTreeView->action == TVE_COLLAPSERESET))
	{
		// clear selection of children, it would be confusing otherwise
		// - the notifications can be over-ridden to stop the de-selection
		// if required.
		// Unfortunately, the parent window can't over-ride this functionality
		// because MFC gives this class first crack.  So if changes are required
		// a derived class will have to be used..
		ASSERT(pNMTreeView->itemNew.hItem);

		// if a descendent item has focus the parent will get selected as a
		// consequence of collapsing the tree, so preserve
		// (if the parent was already selected it will gain focus, but
		// that's acceptable)
		BOOL bWasSel = IsSelected(pNMTreeView->itemNew.hItem);
		BOOL bWasFocus = SelectChildren(pNMTreeView->itemNew.hItem, FALSE, TRUE);

		if (bWasFocus && !bWasSel)
		{
			FocusItem(pNMTreeView->itemNew.hItem); // give parent focus.
		}
	}

	return FALSE;   //pass to parent
}

HTREEITEM CXTPTreeBase::GetNextItem(HTREEITEM hItem) const
{
	HTREEITEM hti = NULL;

	if (m_pTreeCtrl->ItemHasChildren(hItem))
	{
		hti = m_pTreeCtrl->GetChildItem(hItem);
	}

	if (hti == NULL)
	{
		while ((hti = m_pTreeCtrl->GetNextSiblingItem(hItem)) == NULL)
		{
			if ((hItem = m_pTreeCtrl->GetParentItem(hItem)) == NULL)
				return NULL;
		}
	}

	return hti;
}

HTREEITEM CXTPTreeBase::GetPrevItem(HTREEITEM hItem) const
{
	HTREEITEM hti = NULL;

	hti = m_pTreeCtrl->GetPrevSiblingItem(hItem);
	if (hti == NULL)
	{
		hti = m_pTreeCtrl->GetParentItem(hItem);
	}
	else
	{
		hti = GetLastItem(hti);
	}

	return hti;
}

HTREEITEM CXTPTreeBase::GetLastItem(HTREEITEM hItem) const
{
	// Temporary used variable
	HTREEITEM htiNext;

	// Get the last item at the top level
	if (hItem == NULL)
	{
		hItem = m_pTreeCtrl->GetRootItem();

		htiNext = m_pTreeCtrl->GetNextSiblingItem(hItem);
		while (htiNext != NULL)
		{
			hItem = htiNext;
			htiNext = m_pTreeCtrl->GetNextSiblingItem(htiNext);
		}
	}

	while (m_pTreeCtrl->ItemHasChildren(hItem) != NULL)
	{
		// Find the last child of hItem
		htiNext = m_pTreeCtrl->GetChildItem(hItem);
		while (htiNext != NULL)
		{
			hItem = htiNext;
			htiNext = m_pTreeCtrl->GetNextSiblingItem(htiNext);
		}
	}

	return hItem;

}

HTREEITEM CXTPTreeBase::FindItemInBranch(LPCTSTR lpszSearch, BOOL bCaseSensitive /*= FALSE*/,
	BOOL bWholeWord /*= FALSE*/, HTREEITEM htiItem /*= NULL*/)
{
	HTREEITEM htiFound = NULL;

	if (!m_pTreeCtrl->ItemHasChildren(htiItem))
		return NULL;

	CString strSearch = lpszSearch;
	int iLen = strSearch.GetLength();
	if (iLen == 0)
	{
		return NULL;
	}

	if (!bCaseSensitive)
	{
		strSearch.MakeLower();
	}

	HTREEITEM htiChild = m_pTreeCtrl->GetChildItem(htiItem);
	while (htiChild != NULL)
	{
		if (m_pTreeCtrl->ItemHasChildren(htiChild))
		{
			htiFound = FindItemInBranch(lpszSearch, bCaseSensitive, bWholeWord, htiChild);
			if (htiFound != NULL)
				return htiFound;
		}

		CString strItemText = m_pTreeCtrl->GetItemText(htiChild);
		if (!bCaseSensitive)
		{
			strItemText.MakeLower();
		}

		int iIndex;
		while ((iIndex = strItemText.Find(strSearch)) != -1)
		{
			// Search string found
			if (bWholeWord)
			{
				// Check preceding char
				if (iIndex != 0)
				{
					if (_istalpha(strItemText[iIndex-1]) || strItemText[iIndex-1] == '_')
					{
						// Not whole word
						strItemText = strItemText.Right(strItemText.GetLength() -
							iIndex - iLen);
						continue;
					}
				}

				// Check succeeding char
				if (strItemText.GetLength() > iIndex + iLen &&
					(_istalpha(strItemText[iIndex + iLen]) ||
					(strItemText[iIndex + iLen] == '_')))
				{
					// Not whole word
					strItemText = strItemText.Right(strItemText.GetLength() -
						iIndex - strSearch.GetLength());
					continue;
				}
			}

			if (IsFindValid(htiChild))
			{
				return htiChild;
			}

			else
			{
				break;
			}
		}

		htiChild = m_pTreeCtrl->GetNextSiblingItem(htiChild);
	}

	return NULL;
}

HTREEITEM CXTPTreeBase::FindItem(LPCTSTR lpszSearch, BOOL bCaseSensitive /*= FALSE*/,
	BOOL bDownDir /*= TRUE*/, BOOL bWholeWord /*= FALSE*/, HTREEITEM hItem /*= NULL*/)
{
	return FindItemImpl(lpszSearch, bCaseSensitive, bDownDir, bWholeWord, hItem, FALSE);
}

HTREEITEM CXTPTreeBase::FindItemExact(LPCTSTR lpszSearch, BOOL bCaseSensitive /*=FALSE*/, 
	BOOL bDownDir /*=TRUE*/, HTREEITEM hItem /*=NULL*/)
{
	return FindItemImpl(lpszSearch, bCaseSensitive, bDownDir, FALSE, hItem, TRUE);
}

HTREEITEM CXTPTreeBase::FindItemImpl(LPCTSTR lpszSearch, BOOL bCaseSensitive /*= FALSE*/,
	BOOL bDownDir /*= TRUE*/, BOOL bWholeWord /*= FALSE*/, HTREEITEM hItem /*= NULL*/, 
	BOOL bExactlySameText /*= FALSE*/)
{
	CString str = lpszSearch;

	int lenSearchStr = str.GetLength();
	if (lenSearchStr == 0)
	{
		return NULL;
	}

	// For the first pass, set the current item equal to the selection
	HTREEITEM htiSel = hItem ? hItem : m_pTreeCtrl->GetSelectedItem();

	// If NULL, use root item.
	if (htiSel == NULL)
		htiSel = m_pTreeCtrl->GetRootItem();

	HTREEITEM htiCur = htiSel;
	CString strSearch = str;

	// make sure it ends if we started with no selection
	if ((htiCur == NULL) && (htiSel != NULL))
	{
		if (bDownDir)
		{
			htiCur = m_pTreeCtrl->GetRootItem();
		}
		else
		{
			htiCur = GetLastItem(NULL);
		}
	}

	if (!bCaseSensitive)
	{
		strSearch.MakeLower();
	}

	// For the first pass only, we check to see if it
	// is the item we're looking for.
	BOOL bFirstPass = TRUE;
	BOOL bFoundRoot = FALSE;

	while (htiCur && (htiCur != htiSel || bFirstPass))
	{
		bFirstPass = FALSE;

		CString strItemText = m_pTreeCtrl->GetItemText(htiCur);
		if (!bCaseSensitive)
		{
			strItemText.MakeLower();
		}

		//Search for items with exactly the same text as the given search string
		if (bExactlySameText)
		{
			if(strItemText == strSearch)
			{
				if (IsFindValid(htiCur))
				{
					return htiCur;
				}
			}
		}
		else
		{
			int iIndex;
			while ((iIndex = strItemText.Find(strSearch)) != -1)
			{
				// Search string found
				if (bWholeWord)
				{
					// Check preceding char
					if (iIndex != 0)
					{
						if (_istalpha(strItemText[iIndex-1]) || strItemText[iIndex-1] == '_')
						{
							// Not whole word
							strItemText = strItemText.Right(strItemText.GetLength() -
								iIndex - lenSearchStr);
							continue;
						}
					}

					// Check succeeding char
					if (strItemText.GetLength() > iIndex + lenSearchStr &&
						(_istalpha(strItemText[iIndex + lenSearchStr]) ||
						(strItemText[iIndex + lenSearchStr] == '_')))
					{
						// Not whole word
						strItemText = strItemText.Right(strItemText.GetLength() -
							iIndex - strSearch.GetLength());
						continue;
					}
				}

				if (IsFindValid(htiCur))
				{
					return htiCur;
				}
				else
				{
					break;
				}
			}
		}

		htiCur = bDownDir ? GetNextItem(htiCur) : GetPrevItem(htiCur);
		if (htiCur == NULL)
		{
			if (bFoundRoot)
				return NULL;

			if (bDownDir)
			{
				htiCur = m_pTreeCtrl->GetRootItem();
			}
			else
			{
				htiCur = GetLastItem(NULL);
			}

			bFoundRoot = TRUE;
		}
	}

	return NULL;
}

BOOL CXTPTreeBase::IsFindValid(HTREEITEM)
{
	return TRUE;
}

void CXTPTreeBase::SetItemFont(HTREEITEM hItem, LOGFONT& logfont)
{
	CLRFONT cf;
	if (!m_mapColorFont.Lookup(hItem, cf))
	{
		cf.color = COLORREF_NULL;
	}

	cf.logfont = logfont;
	m_mapColorFont[hItem] = cf;
}

BOOL CXTPTreeBase::GetItemFont(HTREEITEM hItem, LOGFONT* plogfont)
{
	CLRFONT cf;
	if (!m_mapColorFont.Lookup(hItem, cf))
	{
		return FALSE;
	}

	if (cf.logfont.lfFaceName[0] == _T('\0'))
	{
		return FALSE;
	}

	*plogfont = cf.logfont;

	return TRUE;

}

void CXTPTreeBase::SetItemBold(HTREEITEM hItem, BOOL bBold)
{
	SetItemState(hItem, bBold ? TVIS_BOLD : 0, TVIS_BOLD);
	m_pTreeCtrl->InvalidateRect(NULL);
}

BOOL CXTPTreeBase::GetItemBold(HTREEITEM hItem)
{
	return GetItemState(hItem, TVIS_BOLD) & TVIS_BOLD;
}

void CXTPTreeBase::SetItemColor(HTREEITEM hItem, COLORREF color)
{
	CLRFONT cf;
	m_mapColorFont.Lookup(hItem, cf);

	cf.color = color;
	m_mapColorFont[hItem] = cf;
	m_pTreeCtrl->InvalidateRect(NULL);
}

COLORREF CXTPTreeBase::GetItemColor(HTREEITEM hItem)
{
	CLRFONT cf;
	if (m_mapColorFont.Lookup(hItem, cf))
		return cf.color;

	return COLORREF_NULL;
}

void CXTPTreeBase::SetItemBackColor(HTREEITEM hItem, COLORREF color)
{
	CLRFONT cf;
	m_mapColorFont.Lookup(hItem, cf);

	cf.colorBack = color;
	m_mapColorFont[hItem] = cf;
	m_pTreeCtrl->InvalidateRect(NULL);
}

COLORREF CXTPTreeBase::GetItemBackColor(HTREEITEM hItem)
{
	CLRFONT cf;
	if (m_mapColorFont.Lookup(hItem, cf))
		return cf.colorBack;

	return COLORREF_NULL;
}

COLORREF CXTPTreeBase::GetTreeBackColor() const
{
#if _MSC_VER >= 1200 // MFC 6.0

	if (XTPSystemVersion()->GetComCtlVersion() >= MAKELONG(71, 4))
	{
		// is there a user defined color available ?
		COLORREF crBack = m_pTreeCtrl->GetBkColor();
		if (crBack != COLORREF_NULL)
			return crBack;
	}
#endif

	ASSERT(m_pTheme);

	if (m_pTheme)
		return m_pTheme->GetTreeBackColor();

	return COLORREF_NULL;
}

COLORREF CXTPTreeBase::GetTreeTextColor() const
{
#if _MSC_VER >= 1200 // MFC 6.0
	if (XTPSystemVersion()->GetComCtlVersion() >= MAKELONG(71, 4))
	{
		// is ther a user defined color available ?
		COLORREF crText = m_pTreeCtrl->GetTextColor();
		if (crText != COLORREF_NULL)
			return crText;
	}

#endif

	ASSERT(m_pTheme);

	if (m_pTheme)
		return m_pTheme->GetTreeTextColor();

	return COLORREF_NULL;
}

COLORREF CXTPTreeBase::GetItemBackColor(UINT uState, bool bTreeHasFocus, DWORD dwStyle, COLORREF crBack) const
{
	ASSERT(m_pTheme);

	if (m_pTheme)
		return m_pTheme->GetItemBackColor(uState, bTreeHasFocus, dwStyle, crBack, m_pTreeCtrl);

	return COLORREF_NULL;
}

COLORREF CXTPTreeBase::GetItemTextColor(UINT uState, bool bTreeHasFocus, DWORD dwStyle, COLORREF crText) const
{
	ASSERT(m_pTheme);

	if (m_pTheme)
		return m_pTheme->GetItemTextColor(uState, bTreeHasFocus, dwStyle, crText, m_pTreeCtrl);

	return COLORREF_NULL;
}

void CXTPTreeBase::SetTheme(int nTheme)
{
	if (m_pTreeCtrl)
		m_pTreeCtrl->SendMessage( TVM_SETBKCOLOR, 0, GetSysColor(COLOR_WINDOW));

	if (m_pTreeCtrl)
		m_pTreeCtrl->SendMessage( TVM_SETTEXTCOLOR, 0, GetSysColor(COLOR_WINDOWTEXT));

	CMDTARGET_RELEASE(m_pTheme);

	m_nTheme = nTheme;

	switch ((XTPControlTheme)m_nTheme)
	{
		case xtpControlThemeDefault:
		{
			m_pTheme = new CXTPTreeTheme();

			break;
		}

		case xtpControlThemeVisualStudio2012:
		case xtpControlThemeVisualStudio2012Light:
		case xtpControlThemeVisualStudio2012Dark:
		{
			m_pTheme = new CXTPTreeVisualStudio2012Theme((XTPControlTheme)m_nTheme == xtpControlThemeVisualStudio2012Light);

			break;
		}
		default:
		{
			m_pTheme = new CXTPTreeTheme();
		}
	}

	RefreshMetrics();
}

void CXTPTreeBase::RefreshMetrics()
{
	if (m_pTheme)
		m_pTheme->RefreshMetrics(this);

	if (m_pTreeCtrl)
		m_pTreeCtrl->SendMessage( TVM_SETBKCOLOR, 0, m_pTheme->GetTreeBackColor());

	if (m_pTreeCtrl)
		m_pTreeCtrl->SendMessage( TVM_SETTEXTCOLOR, 0, m_pTheme->GetTreeTextColor());

	if (m_pWindowsTreeTheme && m_pWindowsTreeTheme->IsThemeActive())
	{
		if (m_bExplorerTheme)
		{
			m_pWindowsTreeTheme->SetWindowTheme(m_pTreeCtrl->GetSafeHwnd(), L"EXPLORER", NULL);
		}
		else
		{
			m_pWindowsTreeTheme->SetWindowTheme(m_pTreeCtrl->GetSafeHwnd(), L"", NULL);
		}

		m_pWindowsTreeTheme->OpenTheme(m_pTreeCtrl->GetSafeHwnd(), L"TREEVIEW" );
	}

	// Update line pen
	if (NULL != m_penLines.m_hObject)
	{
		m_penLines.DeleteObject();
	}

	LOGBRUSH logBrush = { 0 };
	logBrush.lbStyle = PS_SOLID;
	logBrush.lbColor = ::GetSysColor(COLOR_BTNSHADOW);
	if (m_pWindowsTreeTheme != NULL)
	{
		m_pWindowsTreeTheme->GetThemeColor(TVP_BRANCH, 0, TMT_EDGESHADOWCOLOR, &logBrush.lbColor);
	}

	m_penLines.Attach(ExtCreatePen(PS_DOT | PS_GEOMETRIC, 1, &logBrush, 0, NULL));
}

void CXTPTreeBase::CreateCustomPlusMinusIcons(HDC hDC)
{
	// the method currently used to create icons when no themes are available
	// in future it will support custom icons

	static BYTE bufferMinus[] = { 0x42, 0x4d, 0xbe, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x76, 0x0, 0x0, 0x0, 0x28, 0x0, 0x0, 0x0, 0x9, 0x0, 0x0, 0x0, 0x9, 0x0, 0x0, 0x0, 0x1, 0x0, 0x4, 0x0, 0x0, 0x0, 0x0, 0x0, 0x48, 0x0, 0x0, 0x0, 0xc2, 0xe, 0x0, 0x0, 0xc2, 0xe, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x80, 0x0, 0x0, 0x80, 0x0, 0x0, 0x0, 0x80, 0x80, 0x0, 0x80, 0x0, 0x0, 0x0, 0x80, 0x0, 0x80, 0x0, 0x80, 0x80, 0x0, 0x0, 0x80, 0x80, 0x80, 0x0, 0xc0, 0xc0, 0xc0, 0x0, 0x0, 0x0, 0xff, 0x0, 0x0, 0xff, 0x0, 0x0, 0x0, 0xff, 0xff, 0x0, 0xff, 0x0, 0x0, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0xff, 0x0, 0x0, 0xff, 0xff, 0xff, 0x0, 0x77, 0x77, 0x77, 0x77, 0x70, 0x0, 0x0, 0x0, 0x7f, 0xff, 0xff, 0xff, 0x70, 0x0, 0x0, 0x0, 0x7f, 0xff, 0xff, 0xff, 0x70, 0x0, 0x0, 0x0, 0x7f, 0xff, 0xff, 0xff, 0x70, 0x0, 0x0, 0x0, 0x7f, 0x0, 0x0, 0xf, 0x70, 0x0, 0x0, 0x0, 0x7f, 0xff, 0xff, 0xff, 0x70, 0x0, 0x0, 0x0, 0x7f, 0xff, 0xff, 0xff, 0x70, 0x0, 0x0, 0x0, 0x7f, 0xff, 0xff, 0xff, 0x70, 0x0, 0x0, 0x0, 0x77, 0x77, 0x77, 0x77, 0x70, 0x0, 0x0, 0x0 };
	static BYTE bufferPlus [] = { 0x42, 0x4d, 0xbe, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x76, 0x0, 0x0, 0x0, 0x28, 0x0, 0x0, 0x0, 0x9, 0x0, 0x0, 0x0, 0x9, 0x0, 0x0, 0x0, 0x1, 0x0, 0x4, 0x0, 0x0, 0x0, 0x0, 0x0, 0x48, 0x0, 0x0, 0x0, 0xc2, 0xe, 0x0, 0x0, 0xc2, 0xe, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x80, 0x0, 0x0, 0x80, 0x0, 0x0, 0x0, 0x80, 0x80, 0x0, 0x80, 0x0, 0x0, 0x0, 0x80, 0x0, 0x80, 0x0, 0x80, 0x80, 0x0, 0x0, 0x80, 0x80, 0x80, 0x0, 0xc0, 0xc0, 0xc0, 0x0, 0x0, 0x0, 0xff, 0x0, 0x0, 0xff, 0x0, 0x0, 0x0, 0xff, 0xff, 0x0, 0xff, 0x0, 0x0, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0xff, 0x0, 0x0, 0xff, 0xff, 0xff, 0x0, 0x77, 0x77, 0x77, 0x77, 0x70, 0x0, 0x0, 0x0, 0x7f, 0xff, 0xff, 0xff, 0x70, 0x0, 0x0, 0x0, 0x7f, 0xff, 0xf, 0xff, 0x70, 0x0, 0x0, 0x0, 0x7f, 0xff, 0xf, 0xff, 0x70, 0x0, 0x0, 0x0, 0x7f, 0x0, 0x0, 0xf, 0x70, 0x0, 0x0, 0x0, 0x7f, 0xff, 0xf, 0xff, 0x70, 0x0, 0x0, 0x0, 0x7f, 0xff, 0xf, 0xff, 0x70, 0x0, 0x0, 0x0, 0x7f, 0xff, 0xff, 0xff, 0x70, 0x0, 0x0, 0x0, 0x77, 0x77, 0x77, 0x77, 0x70, 0x0, 0x0, 0x0 };

	if (!m_hBitmapMinus)
	{
		BITMAPFILEHEADER& bfh = (BITMAPFILEHEADER&)bufferMinus[0];
		BITMAPINFO& bi = (BITMAPINFO&)bufferMinus[sizeof(BITMAPFILEHEADER)];
		BITMAPINFOHEADER& bih = bi.bmiHeader;

		BYTE* bitmap = &bufferMinus[bfh.bfOffBits];

		m_hBitmapMinus = CreateDIBitmap(hDC, &bih, CBM_INIT, bitmap, &bi, DIB_RGB_COLORS );

		bfh = (BITMAPFILEHEADER&)bufferPlus[0];
		bi = (BITMAPINFO&)bufferPlus[sizeof(BITMAPFILEHEADER)];
		bih = bi.bmiHeader;

		bitmap = &bufferPlus[bfh.bfOffBits];

		m_hBitmapPlus = CreateDIBitmap(hDC, &bih, CBM_INIT, bitmap, &bi, DIB_RGB_COLORS );
	}
}

void CXTPTreeBase::DrawDottedLine(CDC* pDC, const CPoint& ptStart, const CPoint& ptEnd)
{
	ASSERT(NULL != m_penLines.m_hObject);

	CPen* oldPen = pDC->SelectObject(&m_penLines);
	pDC->MoveTo(ptStart);
	pDC->LineTo(ptEnd);
	pDC->SelectObject(oldPen);
}

CRect CXTPTreeBase::GetItemIconRect(HTREEITEM hItem) const
{
	CRect rcIcon(0, 0, 0, 0);
	CImageList *pImageList = m_pTreeCtrl->GetImageList(TVSIL_NORMAL);
	if (pImageList)
	{
		int nImage = 0;
		int nSelectedImage = 0;
		m_pTreeCtrl->GetItemImage(hItem, nImage, nSelectedImage);

		IMAGEINFO ImageInfo;
		if (pImageList->GetImageInfo(nImage, &ImageInfo))
		{
			CSize szImage(
				ImageInfo.rcImage.right - ImageInfo.rcImage.left,
				ImageInfo.rcImage.bottom - ImageInfo.rcImage.top);
			m_pTreeCtrl->GetItemRect(hItem, &rcIcon, TRUE);
			rcIcon.OffsetRect(-(XTP_TREE_ICON_IDENT + szImage.cx), 0);
			rcIcon.right = rcIcon.left + szImage.cx;
			rcIcon.top = (rcIcon.top + rcIcon.bottom) / 2 - szImage.cy / 2;
			rcIcon.bottom = rcIcon.top + szImage.cy;
		}
	}

	return rcIcon;
}

CRect CXTPTreeBase::GetItemStateIconRect(
	HTREEITEM hItem,
	const CRect& rcIcon) const
{
	CRect rcStateIcon(0, 0, 0, 0);
	CImageList *pImageListState = m_pTreeCtrl->GetImageList(TVSIL_STATE);
	if (NULL != pImageListState)
	{
		TVITEM item;
		item.hItem = hItem;
		item.stateMask = TVIS_STATEIMAGEMASK;
		item.mask = TVIF_STATE;
		BOOL bRes = (BOOL)::SendMessage(m_pTreeCtrl->GetSafeHwnd(), TVM_GETITEM, 0, (LPARAM)&item);
		if (bRes)
		{
			int nStateImage = item.state >> 12;
			if (0 != nStateImage)
			{
				IMAGEINFO ImageInfo;
				if (pImageListState->GetImageInfo(nStateImage, &ImageInfo))
				{
					int nStateImageSize = (ImageInfo.rcImage.right - ImageInfo.rcImage.left);
					m_pTreeCtrl->GetItemRect(hItem, &rcStateIcon, TRUE);
					rcStateIcon.OffsetRect(-(XTP_TREE_ICON_IDENT + nStateImageSize), 0);
					rcStateIcon.right = rcStateIcon.left + nStateImageSize;
					rcStateIcon.top += (rcStateIcon.Height() / 2 - nStateImageSize / 2); // vertical alignment center
					if (!rcIcon.IsRectEmpty())
					{
						rcStateIcon.OffsetRect(-(XTP_TREE_ICON_IDENT + nStateImageSize), 0);
					}
				}
			}
		}
	}

	return rcStateIcon;
}

CRect CXTPTreeBase::GetItemExpanderRect(
	HTREEITEM hItem,
	CDC* pDC,
	const CRect& rcIcon,
	const CRect& rcStateIcon) const
{
	ASSERT(NULL != pDC);

	CRect rcExpander(0, 0, 0, 0);

	// Determine expander part size.
	CSize partSize(0, 0);
	if (m_pWindowsTreeTheme->IsAppThemed())
	{
		m_pWindowsTreeTheme->GetThemePartSize(pDC->GetSafeHdc(), TVP_GLYPH, GLPS_CLOSED, NULL, TS_DRAW, &partSize);
	}
	else
	{
		partSize = CSize(9, 9);
	}

	// All checks below are obtained by blind copying of Windows default behavior
	m_pTreeCtrl->GetItemRect(hItem, &rcExpander, TRUE);

	bool bSmallPartSize = (partSize.cx < rcIcon.Width());
	rcExpander.OffsetRect(-(XTP_TREE_ICON_IDENT + partSize.cx), 0);
	rcExpander.right = rcExpander.left + partSize.cx;
	rcExpander.top = (rcExpander.top + rcExpander.bottom) / 2 - partSize.cy / 2;
	rcExpander.bottom = rcExpander.top + partSize.cy;

	if (!rcIcon.IsRectEmpty())
	{
		rcExpander.OffsetRect(-(XTP_TREE_ICON_IDENT + rcIcon.Width()), 0);
	}

	if (!rcStateIcon.IsRectEmpty())
	{
		rcExpander.OffsetRect(-(XTP_TREE_ICON_IDENT + rcStateIcon.Width()), 0);
	}
	else if (bSmallPartSize)
	{
		rcExpander.OffsetRect(-XTP_TREE_ICON_IDENT, 0);
	}

	return rcExpander;
}

void CXTPTreeBase::DrawItemIcons(CDC *pDC, HTREEITEM hItem)
{
	CRect rcItem;
	m_pTreeCtrl->GetItemRect(hItem, &rcItem, FALSE);

	CRect rcIcon = GetItemIconRect(hItem);
	CRect rcStateIcon = GetItemStateIconRect(hItem, rcIcon);
	CRect rcExpander = GetItemExpanderRect(hItem, pDC, rcIcon, rcStateIcon);
	UINT nItemState = m_pTreeCtrl->GetItemState(hItem, TVIS_SELECTED | TVIS_EXPANDED | TVIS_OVERLAYMASK);

	// draw item icon (if available)
	CImageList *pImageList = m_pTreeCtrl->GetImageList(TVSIL_NORMAL);
	if (NULL != pImageList && !rcIcon.IsRectEmpty())
	{
		UINT nOverlayImage = (nItemState & TVIS_OVERLAYMASK) >> 8;
		int nImage = 0;
		int nSelectedImage = 0;
		m_pTreeCtrl->GetItemImage(hItem, nImage, nSelectedImage);
		pImageList->Draw(pDC, (nItemState & TVIS_SELECTED) ? nSelectedImage : nImage, 
			rcIcon.TopLeft(), ILD_NORMAL | INDEXTOOVERLAYMASK(nOverlayImage));
	}

	// draw item state image
	CImageList *pImageListState = m_pTreeCtrl->GetImageList(TVSIL_STATE);
	if (NULL != pImageListState && !rcStateIcon.IsRectEmpty())
	{
		TVITEM item;
		item.hItem = hItem;
		item.mask = TVIF_STATE;
		BOOL bRes = (BOOL)::SendMessage(m_pTreeCtrl->GetSafeHwnd(), TVM_GETITEM, 0, (LPARAM)&item);
		if (bRes)
		{
			int nStateImage = item.state >> 12;
			pImageListState->Draw(pDC, nStateImage, rcStateIcon.TopLeft(), ILD_NORMAL);
		}
	}

	// draw lines if present
	const DWORD dwTreeStyle = m_pTreeCtrl->GetStyle();
	if (dwTreeStyle & TVS_HASLINES)
	{
		if ( !(dwTreeStyle & TVS_LINESATROOT)) // not lines at root
		{
			if (!m_pTreeCtrl->GetParentItem(hItem))
				return;
		}

		CRect rcLeftIcon = rcIcon;
		if (!rcStateIcon.IsRectEmpty())
			rcLeftIcon = rcStateIcon;

		int nVLineTop = rcItem.top;
		int nVLineBottom = rcItem.bottom;
		int nVLineHalfSize = (nVLineBottom - nVLineTop) / 2;
		int nLineWidth = 12;
		int nLineX = rcExpander.left + rcExpander.Width() / 2;

		// Draw vertical line

		// This node is not the first child of the root, so there should be a vertical top half-line here
		if (m_pTreeCtrl->GetPrevSiblingItem(hItem) || m_pTreeCtrl->GetParentItem(hItem))
		{
			DrawDottedLine(pDC, CPoint(nLineX, nVLineTop), CPoint(nLineX , nVLineTop + nVLineHalfSize));
		}

		// This node is not the last child, so there should be a vertical bottom half-line here
		if (m_pTreeCtrl->GetNextSiblingItem(hItem))
		{
			DrawDottedLine(pDC, CPoint(nLineX, nVLineTop + nVLineHalfSize + 1), CPoint(nLineX , nVLineBottom));
		}

		// draw horizontal line
		DrawDottedLine(pDC, CPoint(nLineX, nVLineTop + nVLineHalfSize), CPoint(nLineX + nLineWidth, nVLineTop + nVLineHalfSize));

		// draw full vertical lines

		HTREEITEM hParent = m_pTreeCtrl->GetParentItem(hItem);
		while (hParent)
		{
			if (m_pTreeCtrl->GetNextSiblingItem(hParent)) // This node has a sibling node, so there should be a full vertical line here
			{
				CRect rcParentIcon = GetItemIconRect(hParent);
				CRect rcParentStateIcon = GetItemStateIconRect(hParent, rcParentIcon);
				CRect rcParentExpandCollapse = GetItemExpanderRect(hParent, pDC, rcParentIcon, rcParentStateIcon);

				int nVParentLineTop = rcItem.top;
				int nVParentLineBottom = rcItem.bottom;
				int nParentLineX = rcParentExpandCollapse.left + rcParentExpandCollapse.Width() / 2;

				DrawDottedLine(pDC, CPoint(nParentLineX, nVParentLineTop), CPoint(nParentLineX , nVParentLineBottom));
			}

			hParent = m_pTreeCtrl->GetParentItem(hParent);
		}
	}

	// Draw expand/collapse icon after (over) lines
	if ((dwTreeStyle & TVS_HASBUTTONS) && m_pTreeCtrl->ItemHasChildren(hItem)) // item has children
	{
		if (m_pWindowsTreeTheme->IsAppThemed())
		{
			int nPart = TVP_GLYPH;
			int nState = (nItemState & TVIS_EXPANDED) ? GLPS_OPENED : GLPS_CLOSED;

			if (IsExplorerTheme() && XTPSystemVersion()->IsWinVistaOrGreater()) // no glow effect on Windows XP
			{
				// check mouse cursor

				POINT cursor;
				::GetCursorPos(&cursor);
				m_pTreeCtrl->ScreenToClient(&cursor);

				if (rcExpander.PtInRect(cursor))
				{
					const int TVP_HOTGLYPH = 4;
					enum HOTGLYPHSTATES {
						HGLPS_CLOSED = 1,
						HGLPS_OPENED = 2,
					};

					nPart = TVP_HOTGLYPH;
					nState = (nItemState & TVIS_EXPANDED) ? HGLPS_OPENED : HGLPS_CLOSED;
				}
			}

			m_pWindowsTreeTheme->DrawThemeBackground(pDC->GetSafeHdc(), nPart, nState, rcExpander, NULL);
		}
		else
		{
			if (!m_hBitmapMinus)
				CreateCustomPlusMinusIcons(pDC->GetSafeHdc());

			rcExpander.OffsetRect(0, 2);

			HBITMAP hBitmap = (nItemState & TVIS_EXPANDED) ? m_hBitmapMinus : m_hBitmapPlus;

			CDC dcMem;
			dcMem.CreateCompatibleDC(pDC);
			dcMem.SelectObject(hBitmap);
			pDC->BitBlt(rcExpander.left, rcExpander.top, rcExpander.Width(), rcExpander.Height(), &dcMem, 0, 0, SRCCOPY);
			dcMem.DeleteDC();
		}
	}
}

static BOOL g_bDefaultIconsDrawing = FALSE;

void CXTPTreeBase::DoPaint(CDC& dc, BOOL bInternal)
{
	// Get the client rect.
	CRect rcClient;
	m_pTreeCtrl->GetClientRect(&rcClient);

	const COLORREF crTreeBack = GetTreeBackColor();
	const COLORREF crTreeText = GetTreeTextColor();

	// Paint to a memory device context to help
	// eliminate screen flicker.
	CXTPBufferDC memDC(dc, rcClient);
	memDC.FillSolidRect(rcClient, crTreeBack);

	// Now let the window do its default painting...
	if (!bInternal)
	{
		m_pTreeCtrl->DefWindowProc(WM_PAINT, (WPARAM)memDC.m_hDC, 0);
		return;
	}

	int nSavedDC = memDC.SaveDC();

	// check to see if a tree label is getting edited.
	BOOL bEditing = (HasEditLabels() && m_pTreeCtrl->GetEditControl());

	// if not editing, make sure the edit item is set to NULL.
	if (!bEditing)
		m_htiEdit = NULL;

	// check to see if the tree is enabled.
	BOOL bIsEnabled = m_pTreeCtrl->IsWindowEnabled();

	// check to see if the tree has focus.
	bool bTreeHasFocus = (CWnd::GetFocus() == m_pTreeCtrl);

	// get the style for the tree.
	DWORD dwStyle = m_pTreeCtrl->GetStyle();

	// get the visible count.
	HTREEITEM hItem = m_pTreeCtrl->GetFirstVisibleItem();
	int iVisibleCount = m_pTreeCtrl->GetVisibleCount()+1;

	LOGFONT treefont;
	if (!m_pTreeCtrl->GetFont()->GetLogFont(&treefont))
	{
		::GetObject(::GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &treefont);
	}


	while (hItem && iVisibleCount--)
	{
		if (!g_bDefaultIconsDrawing)
			DrawItemIcons(&memDC, hItem);

		// get the tree item rect.
		CRect rcItem;
		m_pTreeCtrl->GetItemRect(hItem, &rcItem, TRUE);

		// refresh the background.
		CRect rcFill = rcItem;
		rcFill.right = rcClient.right;
		memDC.FillSolidRect(&rcFill, crTreeBack);

		// get the state of the tree item.
		UINT uState = GetItemState(hItem, TVIS_SELECTED | TVIS_FOCUSED | TVIS_DROPHILITED);

		if (m_pTreeCtrl->GetDropHilightItem() && uState & TVIS_SELECTED && hItem != m_pTreeCtrl->GetDropHilightItem())
			uState ^= TVIS_SELECTED;

		BOOL bSelected = ((uState & (TVIS_SELECTED | TVIS_DROPHILITED)) != 0);

		bool bUnderline = m_htiLast == hItem;

		// define the background and text colors.
		COLORREF crItemBack = GetItemBackColor(uState, bTreeHasFocus, dwStyle, crTreeBack);
		COLORREF crItemText = GetItemTextColor(uState, bTreeHasFocus, dwStyle, crTreeText);

		// if the item is getting edited paint white.
		if (m_htiEdit == hItem)
		{
			crItemBack = crTreeBack;
			crItemText = crTreeBack;
		}

		// create the LOGFONT used by the tree item.
		LOGFONT logfont;
		logfont = treefont;


		CLRFONT cf;
		if (m_mapColorFont.Lookup(hItem, cf))
		{
			if (!bSelected && bIsEnabled)
			{
				if (cf.color != COLORREF_NULL)
					crItemText = cf.color;

				if (cf.colorBack != COLORREF_NULL)
					crItemBack = cf.colorBack;
			}

			if (cf.logfont.lfFaceName[0] != _T('\0'))
			{
				logfont = cf.logfont;
			}
		}

		if (GetItemBold(hItem))
		{
			logfont.lfWeight = FW_BOLD;
		}

		if (dwStyle & TVS_TRACKSELECT)
		{
			logfont.lfUnderline = bUnderline ? (BYTE)1 : (BYTE)0;

			if (logfont.lfUnderline && !bSelected)
			{
				if (XTPSystemVersion()->IsWin95() || XTPSystemVersion()->IsWinNT4())
					crItemText = GetXtremeColor(COLOR_HIGHLIGHT);
				else
					crItemText = GetXtremeColor(COLOR_HOTLIGHT);
			}
		}

		// create a CFont object from the LOGFONT structure and
		// select it into the current device context.
		CFont fontDC;
		fontDC.CreateFontIndirect(&logfont);
		CFont* pOldFont = memDC.SelectObject(&fontDC);

		// get the text for the tree item and determine its size.
		CString strItem = m_pTreeCtrl->GetItemText(hItem);
		CSize sizeText = memDC.GetTextExtent(strItem);

		// if the text is wider than the tree item label, adjust accordingly.
		if (rcItem.Width() < sizeText.cx)
		{
			rcItem.right = rcItem.left + sizeText.cx + 2;
		}

		// set the font foreground and background colors.
		memDC.SetBkColor(crItemBack);
		memDC.SetTextColor(crItemText);

		// draw the label background if selected.
		if (bSelected)
		{
			memDC.FillSolidRect(&rcItem, crItemBack);
		}

		CRect rcText(rcItem);
		rcText.OffsetRect(3, 0);

		// draw the text and restore the device context.
		memDC.DrawText(strItem, &rcText, DT_SINGLELINE | DT_LEFT | DT_VCENTER | DT_NOPREFIX);

		// draw the focus rect.
		if ((uState & TVIS_FOCUSED) && (m_pTheme && m_pTheme->IsDrawFocusRect()))
		{
			if (!bEditing && bIsEnabled && CWnd::GetFocus() == m_pTreeCtrl)
			{
				memDC.SetTextColor(crTreeText);
				memDC.DrawFocusRect(&rcItem);
			}
		}

		memDC.SelectObject(pOldFont);
		fontDC.DeleteObject();

		// move to the next visible item.
		hItem = m_pTreeCtrl->GetNextVisibleItem(hItem);
	}

	memDC.RestoreDC(nSavedDC);
}


HTREEITEM CXTPTreeBase::GetPrevSelectedItem(HTREEITEM hItem) const
{
	for (hItem = m_pTreeCtrl->GetPrevVisibleItem(hItem);
		hItem != NULL;
		hItem = m_pTreeCtrl->GetPrevVisibleItem(hItem))
	{
		if (GetItemState(hItem, TVIS_SELECTED) & TVIS_SELECTED)
		{
			return hItem;
		}
	}

	return NULL;
}

UINT CXTPTreeBase::GetSelectedCount() const
{
	UINT nCount = 0;
	HTREEITEM hItem = GetFirstSelectedItem();
	while (hItem)
	{
		nCount++;
		hItem = GetNextSelectedItem(hItem);
	}
	return nCount;
}

BOOL CXTPTreeBase::EnableMultiSelect(BOOL bMultiSelect)
{
	BOOL bReturn = m_bMultiSelect;
	m_bMultiSelect = bMultiSelect;

	if (!m_bMultiSelect)
	{
		HTREEITEM hItem = m_pTreeCtrl->GetSelectedItem();
		if (hItem && !IsSelected(hItem))
		{
			hItem = NULL;
		}
		SelectAllIgnore(FALSE, hItem);
		if (hItem)
		{
			SelectItem(hItem);
		}
	}

	return bReturn;
}

BOOL CXTPTreeBase::SetItemState(HTREEITEM hItem, UINT nState, UINT nStateMask)
{
	ASSERT(hItem);
	if (!hItem)
		return FALSE;

	if (!m_bMultiSelect)
	{
		return m_pTreeCtrl->SetItemState(hItem, nState, nStateMask);
	}

	HTREEITEM hFocus = m_pTreeCtrl->GetSelectedItem();           // current focus
	BOOL bWasFocus = (hFocus == hItem);
	BOOL bFocusWasSel = hFocus && IsSelected(hFocus); // selection state of current focus
	BOOL bWasSel = IsSelected(hItem);            // select state of acting item

	UINT nS = (nState & ~TVIS_FOCUSED);
	UINT nSM = (nStateMask & ~TVIS_FOCUSED);

	BOOL bVista = XTPSystemVersion()->IsWinVistaOrGreater();

	if (nStateMask & TVIS_FOCUSED)
	{
		//wanted to affect focus
		if (nState & TVIS_FOCUSED)
		{
			if (bVista)
			{
				//wanted to set focus
				if (!bWasFocus && hFocus)
				{
					//because SelectItem would de-select the current 'real' selection
					// (the one with focus), need to make the tree ctrl think there is
					// no 'real' selection but still keep the the old item selected
					//it has to be done before the SelectItem call because
					// otherwise the TVN_SELCHANGING/ED notification handlers
					// wouldn't be able to get the proper list of selected items
					m_pTreeCtrl->SelectItem(NULL); //will cause notify, but can be taken as focus change
					m_pTreeCtrl->SetItemState(hFocus, TVIS_SELECTED, TVIS_SELECTED);
				}

				if (!m_pTreeCtrl->SelectItem(hItem))    //set focus (will consequently select, if not already focused)
				{
					return FALSE;
				}

				if (!bWasFocus && hFocus)
				{
					m_pTreeCtrl->SetItemState(hFocus, bFocusWasSel ? TVIS_SELECTED : 0, TVIS_SELECTED);
				}
			}
			else
			{
				//wanted to set focus
				if (!bWasFocus && bFocusWasSel)
				{
					//because SelectItem would de-select the current 'real' selection
					// (the one with focus), need to make the tree ctrl think there is
					// no 'real' selection but still keep the the old item selected
					//it has to be done before the SelectItem call because
					// otherwise the TVN_SELCHANGING/ED notification handlers
					// wouldn't be able to get the proper list of selected items
					m_pTreeCtrl->SelectItem(NULL); //will cause notify, but can be taken as focus change
					m_pTreeCtrl->SetItemState(hFocus, TVIS_SELECTED, TVIS_SELECTED);
					m_pTreeCtrl->UpdateWindow();
				}

				if (!m_pTreeCtrl->SelectItem(hItem))    //set focus (will consequently select, if not already focused)
				{
					return FALSE;
				}
			}

			if (nStateMask & TVIS_SELECTED)
			{
				//wanted to affect select state
				if (nState & TVIS_SELECTED)
				{
					//wanted to select, already done if wasn't focused
					if (!bVista && (!bWasFocus || bFocusWasSel))
					{
						nS &= ~TVIS_SELECTED;
						nSM &= ~TVIS_SELECTED;
					}
				}
				//else wanted to clear, base call will do that
			}
			else
			{
				//didn't want to affect select state
				if (!bWasSel)
				{
					//it wasn't previously selected, let base clear (correct)
					nS &= ~TVIS_SELECTED;
					nSM |= TVIS_SELECTED;
				}
				//else was already selected, no harm done
			}
		}
		else
		{
			//wanted to clear focus
			if (bWasFocus)
			{
				//it had the focus
				m_pTreeCtrl->SelectItem(NULL);  //clear focus
				if (!(nStateMask & TVIS_SELECTED))
				{
					//didn't want to affect select state
					if (bWasSel)
					{
						//it was selected, so restore
						ASSERT(!(nS & TVIS_SELECTED));
						ASSERT(!(nSM & TVIS_SELECTED));
						//set state here, to avoid double-notify
						m_pTreeCtrl->SetItemState(hItem, TVIS_SELECTED, TVIS_SELECTED);
						//let base do other states
					}
				}
				else if (nState & TVIS_SELECTED)
				{
					//wanted to select (but clear focus)
					if (bWasSel)
					{
						//if was selected, restore
						m_pTreeCtrl->SetItemState(hItem, TVIS_SELECTED, TVIS_SELECTED);
					}
					//don't want to notify, default did it
					nS &= ~TVIS_SELECTED;
					nSM &= ~TVIS_SELECTED;
				}
			}
		}
	}

	if (!nSM)
	{
		return TRUE;    //no other states to alter
	}

	if (nSM & TVIS_SELECTED)
	{
		//still need to alter selection state
		NMTREEVIEW nmtv;
		ZeroMemory(&nmtv, sizeof(NMTREEVIEW));

		nmtv.hdr.hwndFrom = m_pTreeCtrl->m_hWnd;
		nmtv.hdr.idFrom = ::GetDlgCtrlID(m_pTreeCtrl->m_hWnd);
		nmtv.hdr.code = TVN_SELCHANGING;
		nmtv.itemOld.mask = nmtv.itemNew.mask = 0;
		nmtv.itemOld.hItem = nmtv.itemNew.hItem = NULL;

		TVITEM& item = (nS & TVIS_SELECTED) ? nmtv.itemNew : nmtv.itemOld;
		item.mask = TVIF_HANDLE | TVIF_PARAM;
		item.hItem = hItem;
		item.lParam = m_pTreeCtrl->GetItemData(hItem);
		item.state = nS;
		item.stateMask = nSM;

		if (SendNotify(&nmtv.hdr))
		{
			return FALSE;   //sel-changing stopped
		}

		VERIFY(m_pTreeCtrl->SetItemState(hItem, nS, nSM));
		nmtv.hdr.code = TVN_SELCHANGED;
		SendNotify(&nmtv.hdr);
		nS &= ~TVIS_SELECTED;
		nSM &= ~TVIS_SELECTED;
	}

	if (!nSM)
	{
		return TRUE;
	}

	return m_pTreeCtrl->SetItemState(hItem, nS, nSM);
}

UINT CXTPTreeBase::GetItemState(HTREEITEM hItem, UINT nStateMask) const
{
	UINT n = m_pTreeCtrl->GetItemState(hItem, nStateMask & ~TVIS_FOCUSED);
	if (nStateMask & TVIS_FOCUSED)
	{
		if (m_pTreeCtrl->GetSelectedItem() == hItem)
		{
			n |= TVIS_FOCUSED;
		}
	}
	return n;
}

BOOL CXTPTreeBase::SelectItem(HTREEITEM hItem)
{
	if (m_bMultiSelect)
	{
		return SetItemState(hItem,
			TVIS_SELECTED | TVIS_FOCUSED, TVIS_SELECTED | TVIS_FOCUSED);
	}
	else
	{
		return m_pTreeCtrl->SelectItem(hItem);
	}
}

BOOL CXTPTreeBase::FocusItem(HTREEITEM hItem)
{
	ASSERT(m_bMultiSelect);

	BOOL bRet = FALSE;
	if (hItem)
	{
		bRet = SetItemState(hItem, TVIS_FOCUSED, TVIS_FOCUSED);
	}
	else
	{
		hItem = m_pTreeCtrl->GetSelectedItem();
		if (hItem)
		{
			bRet = SetItemState(hItem, 0, TVIS_FOCUSED);
		}
	}
	return bRet;
}

LRESULT CXTPTreeBase::SendNotify(LPNMHDR pNMHDR)
{
	CWnd* pWndOwner = m_pTreeCtrl->GetOwner();
	if (pWndOwner->GetSafeHwnd())
	{
		return ::SendMessage(pWndOwner->m_hWnd, WM_NOTIFY,
			(WPARAM)pNMHDR->idFrom, (LPARAM)pNMHDR);
	}

	return NULL;
}

HTREEITEM CXTPTreeBase::GetFirstSelectedItem() const
{
	HTREEITEM hItem = m_pTreeCtrl->GetRootItem();
	while (hItem)
	{
		if (IsSelected(hItem))
		{
			break;
		}
		hItem = m_pTreeCtrl->GetNextVisibleItem(hItem);
	}
	return hItem;
}

HTREEITEM CXTPTreeBase::GetNextSelectedItem(HTREEITEM hItem) const
{
	hItem = m_pTreeCtrl->GetNextVisibleItem(hItem);
	while (hItem)
	{
		if (IsSelected(hItem))
		{
			break;
		}
		hItem = m_pTreeCtrl->GetNextVisibleItem(hItem);
	}
	return hItem;
}

void CXTPTreeBase::SelectAll(BOOL bSelect /*= TRUE*/, HTREEITEM htItem/*= NULL*/)
{
	bSelect = !!bSelect;    //ensure 0 or 1
	UINT nState = bSelect ? TVIS_SELECTED : 0;

	if (htItem == NULL)
		htItem = m_pTreeCtrl->GetRootItem();

	while (htItem)
	{
		if (IsSelected(htItem) != bSelect)
		{
			SetItemState(htItem, nState, TVIS_SELECTED);
		}
		htItem = m_pTreeCtrl->GetNextVisibleItem(htItem);
	}
}

void CXTPTreeBase::SelectAllIgnore(BOOL bSelect, HTREEITEM hIgnore)
{
	//special case to avoid multiple notifications for
	// the same item
	bSelect = !!bSelect;    //ensure 0 or 1
	UINT nState = bSelect ? TVIS_SELECTED : 0;
	HTREEITEM hItem = m_pTreeCtrl->GetRootItem();
	while (hItem)
	{
		if (hItem != hIgnore)
		{
			if (IsSelected(hItem) != bSelect)
			{
				SetItemState(hItem, nState, TVIS_SELECTED);
			}
		}
		hItem = m_pTreeCtrl->GetNextVisibleItem(hItem);
	}
}

void CXTPTreeBase::SelectItems(HTREEITEM hFirst, HTREEITEM hLast, BOOL bOnly /*= TRUE*/)
{
	//locate (and select) either first or last
	// (so order is arbitrary)
	HTREEITEM hItem = m_pTreeCtrl->GetRootItem();
	while (hItem)
	{
		if ((hItem == hFirst) || (hItem == hLast))
		{
			if (hFirst != hLast)
			{
				if (!IsSelected(hItem))
				{
					SetItemState(hItem, TVIS_SELECTED, TVIS_SELECTED);
				}
				hItem = m_pTreeCtrl->GetNextVisibleItem(hItem);
			}
			break;
		}

		if (bOnly && IsSelected(hItem))
		{
			SetItemState(hItem, 0, TVIS_SELECTED);
		}

		hItem = m_pTreeCtrl->GetNextVisibleItem(hItem);
	}

	//select rest of range
	while (hItem)
	{
		if (!IsSelected(hItem))
		{
			SetItemState(hItem, TVIS_SELECTED, TVIS_SELECTED);
		}
		if ((hItem == hFirst) || (hItem == hLast))
		{
			hItem = m_pTreeCtrl->GetNextVisibleItem(hItem);
			break;
		}
		hItem = m_pTreeCtrl->GetNextVisibleItem(hItem);
	}
	if (!bOnly)
	{
		return;
	}
	while (hItem)
	{
		if (IsSelected(hItem))
		{
			SetItemState(hItem, 0, TVIS_SELECTED);
		}
		hItem = m_pTreeCtrl->GetNextVisibleItem(hItem);
	}
}

BOOL CXTPTreeBase::OnButtonDown(BOOL bLeft, UINT nFlags, CPoint point)
{
	UINT      nHF = 0;
	HTREEITEM hItem = NULL;

	if (!m_bMultiSelect)
		return TRUE;

	BOOL bBase = FALSE;

	hItem = m_pTreeCtrl->HitTest(point, &nHF);
	if (hItem)
	{
		//base always handles expanding items
		//(doesn't really mean much to right button, but pass anyway)
		bBase = (nHF & (TVHT_ONITEMBUTTON));
		if (!bBase && bLeft && (m_pTreeCtrl->GetStyle() & TVS_CHECKBOXES))
		{
			//when the tree has check-boxes, the default handler makes
			// a quick selection of the clicked item, then re-selects
			// the previously selected item - to cause a sel-changed
			// notification.  Fortunately it doesn't affect the multi-
			// selection, so can pass on.
			bBase = (nHF & TVHT_ONITEMSTATEICON);

#ifdef _MST_MULTI_CHECK
			//Use the above define if you want all selected items to
			// be checked the same when any one of them is checked
			// - Interestingly this doesn't happen in the listview control
			//  (LVS_EX_CHECKBOXES)
			if (bBase)
			{
				//the default selection notification would mess
				// the multi-selection up, so generate the notification
				// manually
				// (anyway, this is smoother than the selection flicker
				//  the default gives)
				NMTREEVIEW nmtv;
				ZeroMemory(&nmtv, sizeof(NMTREEVIEW));

#ifdef TVN_CHKCHANGE
				nmtv.hdr.code = TVN_CHKCHANGE;
#else
				nmtv.hdr.code = TVN_SELCHANGED;
#endif
				nmtv.hdr.hwndFrom = m_hWnd;
				nmtv.hdr.idFrom = ::GetDlgCtrlID(m_hWnd);
				nmtv.itemOld.hItem = NULL;
				nmtv.itemNew.mask = TVIF_HANDLE | TVIF_PARAM;

				BOOL bChk = !GetCheck(hItem);
				if (IsSelected(hItem))
				{
					HTREEITEM h = GetFirstSelectedItem();
					while (h)
					{
						if (!GetCheck(h) == bChk) //! to ensure 0 or 1
						{
							SetCheck(h, bChk);
#ifdef TVN_CHKCHANGE
							//only send multiple check-change
							// notifications (not sel-changed)
							if (h != hItem) //clicked item will be done last
							{
								nmtv.itemNew.hItem = h;
								nmtv.itemNew.lParam = m_pTreeCtrl->GetItemData(h);
								SendNotify(&nmtv.hdr);
							}
#endif
						}
						h = GetNextSelectedItem(h);
					}
				}
				else
				{
					SetCheck(hItem, bChk);
				}
				//notify clicked item
				nmtv.itemNew.hItem = hItem;
				nmtv.itemNew.lParam = m_pTreeCtrl->GetItemData(hItem);
				SendNotify(&nmtv.hdr);
				return TRUE;
			}
#endif
		}
	}

	if (bBase)
		return TRUE;

	if (!hItem || (nHF & (TVHT_ONITEMRIGHT | TVHT_NOWHERE | TVHT_ONITEMINDENT)))
	{
		//clicked in space, do rubber-banding
		DoBanding(nFlags, point);
		return TRUE;
	}

	ASSERT(nHF & (TVHT_ONITEM | TVHT_ONITEMSTATEICON));   //nothing else left

	DoPreSelection(hItem, bLeft, nFlags);
	DoAction(hItem, bLeft, nFlags, point);

	return m_bActionDone;  // as set in doaction
}

void CXTPTreeBase::DoPreSelection(HTREEITEM hItem, BOOL bLeft, UINT nFlags)
{
	if (bLeft)
	{
		//if shift key down, select immediately
		if ((nFlags & MK_SHIFT))
		{
			if (!m_hSelect)
			{
				m_hSelect = m_pTreeCtrl->GetSelectedItem(); //focus
			}

			SetItemState(hItem, TVIS_FOCUSED, TVIS_FOCUSED);    //focus changes to last clicked
			SelectItems(m_hSelect, hItem, !(nFlags & MK_CONTROL));
		}
		else
		{
			if (!(nFlags & MK_CONTROL))
			{
				//if ctrl was down, then the selection is delayed until
				// mouse up, otherwise select the one item
				if (!IsSelected(hItem))
				{
					SelectAllIgnore(FALSE, hItem);
				}
				SetItemState(hItem, TVIS_SELECTED | TVIS_FOCUSED, TVIS_SELECTED | TVIS_FOCUSED);
			}
			m_hSelect = NULL;   //reset when a non-shift operation occurs
		}
		return;
	}

	//right mouse
	if (nFlags & (MK_CONTROL | MK_SHIFT))
	{
		if (!(nFlags & MK_SHIFT))
		{
			m_hSelect = hItem;
		}
		return;     //do nothing if shift or ctrl
	}
	if (!IsSelected(hItem))
	{
		SelectAllIgnore(FALSE, hItem);
	}
	SetItemState(hItem, TVIS_SELECTED | TVIS_FOCUSED, TVIS_SELECTED | TVIS_FOCUSED);
}

void CXTPTreeBase::DoAction(HTREEITEM hItem, BOOL bLeft, UINT nFlags, CPoint point)
{
	m_bActionDone = false;
	::SetCapture(m_pTreeCtrl->m_hWnd);
	ASSERT(::GetCapture() == m_pTreeCtrl->m_hWnd);

	MSG msg;
	UINT nDone = 0;
	CPoint pt;
	CSize sizeDrag(::GetSystemMetrics(SM_CXDRAG), ::GetSystemMetrics(SM_CYDRAG));

	while (!nDone && ::GetMessage(&msg, NULL, 0, 0))
	{
		if (::GetCapture() != m_pTreeCtrl->m_hWnd)
		{
			break;
		}
		switch (msg.message)
		{
			case WM_MOUSEMOVE:
				pt.x = LOWORD(msg.lParam);
				pt.y = HIWORD(msg.lParam);
				if ((abs(pt.x - point.x) > sizeDrag.cx) ||
					((abs(pt.y - point.y) > sizeDrag.cy)))
				{
					nDone = 2;
				}
				//because we exit loop, button up will still be dispatched
				// which means WM_CONTEXTMENU will be sent after TVN_BEGINRDRAG
				// - this is the same behavior as original tree
				break;

			case WM_LBUTTONUP:
			case WM_RBUTTONUP:
				nDone = 1;
				break;

			default:
				::DispatchMessage(&msg);
				break;
		}

	}

	::ReleaseCapture();
	ASSERT(::GetCapture() != m_pTreeCtrl->m_hWnd);

	//construct tree notification info
	NMTREEVIEW nmtv;
	ZeroMemory(&nmtv, sizeof(NMTREEVIEW));

	nmtv.hdr.hwndFrom = m_pTreeCtrl->m_hWnd;
	nmtv.hdr.idFrom = ::GetDlgCtrlID(m_pTreeCtrl->m_hWnd);
	nmtv.itemNew.mask = TVIF_HANDLE | TVIF_PARAM;
	nmtv.itemNew.hItem = hItem;
	nmtv.itemNew.lParam = m_pTreeCtrl->GetItemData(hItem);
	DWORD dwStyle = m_pTreeCtrl->GetStyle();

	if (nDone == 1)
	{
		//click
		if (!(nFlags & MK_SHIFT) && bLeft)
		{
			if ((nFlags & MK_CONTROL))
			{
				UINT nState = (GetItemState(hItem, TVIS_SELECTED) & TVIS_SELECTED) ? 0 : TVIS_SELECTED;
				SetItemState(hItem, TVIS_FOCUSED | nState, TVIS_FOCUSED | TVIS_SELECTED);
			}
			else
			{
				SelectAllIgnore(FALSE, hItem);
				SetItemState(hItem, TVIS_FOCUSED | TVIS_SELECTED, TVIS_FOCUSED | TVIS_SELECTED);
			}
		}
		if (::GetFocus() != m_pTreeCtrl->m_hWnd)
		{
			::SetFocus(m_pTreeCtrl->m_hWnd);
		}

		nmtv.hdr.code = bLeft ? NM_CLICK : NM_RCLICK;
		SendNotify(&nmtv.hdr);
		m_bActionDone = true;
	}
	else if (nDone == 2)
	{
		//drag
		SetItemState(hItem, TVIS_FOCUSED | TVIS_SELECTED, TVIS_FOCUSED | TVIS_SELECTED);
		if (!(dwStyle & TVS_DISABLEDRAGDROP))
		{
			nmtv.hdr.code = bLeft ? TVN_BEGINDRAG : TVN_BEGINRDRAG;
			nmtv.ptDrag = point;
			SendNotify(&nmtv.hdr);
		}
		m_bActionDone = true;
	}
}

void CXTPTreeBase::DoBanding(UINT nFlags, CPoint point)
{
	if (::GetFocus() != m_pTreeCtrl->m_hWnd)
	{
		::SetFocus(m_pTreeCtrl->m_hWnd);
	}

	::SetCapture(m_pTreeCtrl->m_hWnd);

	CTypedPtrList<CPtrList, HTREEITEM> list;
	if (nFlags & (MK_SHIFT | MK_CONTROL))
	{
		GetSelectedList(list);
	}

	CClientDC dc(m_pTreeCtrl);
	CRect rectCli;
	m_pTreeCtrl->GetClientRect(&rectCli);

	MSG msg;
	BOOL bDone = FALSE;
	CPoint pt;
	CSize sizeDrag(::GetSystemMetrics(SM_CXDRAG), ::GetSystemMetrics(SM_CYDRAG));
	BOOL bDrag = FALSE;
	CSize sizeEdge(1, 1);

	UINT_PTR nTimer = m_pTreeCtrl->SetTimer(1, SCROLL_TIMER_PERIOD, NULL);  //for scroll
	CPoint ptScr(m_pTreeCtrl->GetScrollPos(SB_HORZ), m_pTreeCtrl->GetScrollPos(SB_VERT));
	CRect rect(0, 0, 0, 0);
	UINT h = 0;
	HTREEITEM hItem = m_pTreeCtrl->GetRootItem();
	if (hItem)
	{
		m_pTreeCtrl->GetItemRect(hItem, &rect, FALSE);
		ptScr.y *= (h = rect.Height());     //this assumes equal height items
	}
	point += ptScr;

	while (!bDone && ::GetMessage(&msg, NULL, 0, 0))
	{
		if (::GetCapture() != m_pTreeCtrl->m_hWnd)
		{
			break;
		}

		switch (msg.message)
		{
			case WM_TIMER:
				{
					if (msg.wParam == SCROLL_TIMER_PERIOD)
					{
						pt = msg.pt;
						m_pTreeCtrl->ScreenToClient(&pt);
						if (rectCli.PtInRect(pt))
						{
							::DispatchMessage(&msg);
							break;
						}
						msg.lParam = MAKELPARAM(pt.x, pt.y);
					}
					else
					{
						break;
					}
				}
				//fall through to mouse move

			case WM_MOUSEMOVE:
				pt = CPoint(msg.lParam);
				if (!bDrag)
				{
					if ((abs(pt.x - point.x) <= sizeDrag.cx) &&
						((abs(pt.y - point.y) <= sizeDrag.cy)))
					{
						break;
					}
					bDrag = TRUE;
					if (!(nFlags & (MK_CONTROL | MK_SHIFT)))
					{
						SelectAll(FALSE);
					}
					m_pTreeCtrl->UpdateWindow();
					rect.SetRect(point, point);
					dc.DrawDragRect(rect, sizeEdge, NULL, sizeEdge);
				}

				dc.DrawDragRect(rect, sizeEdge, NULL, sizeEdge);    //delete

				if (pt.y < rectCli.top)
				{
					::SendMessage(m_pTreeCtrl->m_hWnd, WM_VSCROLL, SB_LINEUP, 0);
				}
				else if (pt.y >= rectCli.bottom)
				{
					::SendMessage(m_pTreeCtrl->m_hWnd, WM_VSCROLL, SB_LINEDOWN, 0);
				}
				if (pt.x < rectCli.left)
				{
					::SendMessage(m_pTreeCtrl->m_hWnd, WM_HSCROLL, SB_LINELEFT, 0);
				}
				else if (pt.x >= rectCli.right)
				{
					::SendMessage(m_pTreeCtrl->m_hWnd, WM_HSCROLL, SB_LINERIGHT, 0);
				}

				ptScr = point;
				ptScr.x -= m_pTreeCtrl->GetScrollPos(SB_HORZ);
				ptScr.y -= m_pTreeCtrl->GetScrollPos(SB_VERT) * h;
				rect.SetRect(ptScr, pt);
				rect.NormalizeRect();
				UpdateSelectionForRect(rect, nFlags, list);
				dc.DrawDragRect(rect, sizeEdge, NULL, sizeEdge);    //draw
				break;

			case WM_LBUTTONUP:
			case WM_RBUTTONUP:
				bDone = TRUE;
				break;

			case WM_KEYDOWN:
				if (LOWORD(msg.wParam) == VK_ESCAPE)
				{
					SelectAll(FALSE);
					bDone = TRUE;
					break;
				}
				//dispatch

			default:
				::DispatchMessage(&msg);
				break;
		}
	}
	m_pTreeCtrl->KillTimer(nTimer);
	::ReleaseCapture();

	if (bDrag)
	{
		dc.DrawDragRect(rect, sizeEdge, NULL, sizeEdge);
	}
	else
	{
		if (!(nFlags & (MK_CONTROL | MK_SHIFT)))
		{
			SelectAll(FALSE);
		}
	}
}

void CXTPTreeBase::UpdateSelectionForRect(LPCRECT pRect, UINT nFlags, CTypedPtrList<CPtrList, HTREEITEM>& list)
{
	CRect rect;
	BOOL bSel;
	POSITION pos;

	HTREEITEM hItem = m_pTreeCtrl->GetRootItem();
	while (hItem)
	{
		bSel = IsSelected(hItem);
		m_pTreeCtrl->GetItemRect(hItem, &rect, m_bBandLabel);
		if (rect.IntersectRect(rect, pRect))
		{
			//item in rect
			pos = list.Find(hItem);
			if (!bSel && !pos)
			{
				SetItemState(hItem, TVIS_SELECTED, TVIS_SELECTED);
			}
			else if ((nFlags & MK_CONTROL) && pos)
			{
				SetItemState(hItem, 0, TVIS_SELECTED);
			}
			else if ((nFlags & MK_SHIFT) && pos)
			{
				list.RemoveAt(pos);     //if shift and in rect, don't lock anymore
			}
		}
		else
		{
			//item not in rect
			pos = list.Find(hItem);
			if (bSel && !pos)
			{
				SetItemState(hItem, 0, TVIS_SELECTED);
			}
			else if (pos)
			{
				SetItemState(hItem, TVIS_SELECTED, TVIS_SELECTED);
			}
		}
		hItem = m_pTreeCtrl->GetNextVisibleItem(hItem);
	}
	m_pTreeCtrl->UpdateWindow();
}

BOOL CXTPTreeBase::SelectChildren(HTREEITEM hParent, BOOL bSelect /*= TRUE*/, BOOL bRecurse /*= TRUE*/)
{
	UINT nS = bSelect ? TVIS_SELECTED : 0;

	BOOL bFocusWasInHere = FALSE;

	HTREEITEM hItem = GetNextItem(hParent, TVGN_CHILD);
	while (hItem)
	{
		UINT nState = GetItemState(hItem, TVIS_SELECTED | TVIS_EXPANDED | TVIS_FOCUSED);
		if ((nState & TVIS_SELECTED) != nS)
		{
			SetItemState(hItem, nS, TVIS_SELECTED);
		}
		bFocusWasInHere |= (nState & TVIS_FOCUSED);
		if (bRecurse && (nState & TVIS_EXPANDED))
		{
			bFocusWasInHere |= SelectChildren(hItem, bSelect, bRecurse);
		}
		hItem = m_pTreeCtrl->GetNextSiblingItem(hItem);
	}
	return bFocusWasInHere;
}

void CXTPTreeBase::GetSelectedList(CTypedPtrList<CPtrList, HTREEITEM>& list) const
{
	list.RemoveAll();

	HTREEITEM hItem = GetFirstSelectedItem();
	while (hItem)
	{
		list.AddTail(hItem);
		hItem = GetNextSelectedItem(hItem);
	}
}

BOOL CXTPTreeBase::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	NMHDR* pNMHDR = (NMHDR*)lParam;

	switch (pNMHDR->code)
	{
	case TTN_SHOW:
		{
			// get a pointer to the tooltip control.
			HWND hWnd = TreeView_GetToolTips(m_pTreeCtrl->m_hWnd);
			if (hWnd != NULL)
			{
				// make sure the tooltip is at the top of the "Z" order, otherwise
				// it will appear behind popup windows....
				::SetWindowPos(hWnd, HWND_TOP, 0, 0, 0, 0,
					SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
			}
		}
		break;
	}

	return m_pTreeCtrl->CTreeCtrl::OnNotify(wParam, lParam, pResult);
}

BOOL CXTPTreeBase::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		// ensure that keystrokes are handled by the edit control.
		if (HasEditLabels() && m_pTreeCtrl->GetEditControl())
		{
			::TranslateMessage(pMsg);
			::DispatchMessage(pMsg);

			return TRUE;
		}

		// toggle expand / contract when return key is hit.
		if (pMsg->wParam == VK_RETURN)
		{
			HTREEITEM htItem = m_pTreeCtrl->GetSelectedItem();
			if (htItem != NULL)
			{
				m_pTreeCtrl->Expand(htItem, TVE_TOGGLE);
				return TRUE;
			}
		}
	}

	return m_pTreeCtrl->CTreeCtrl::PreTranslateMessage(pMsg);
}

BOOL CXTPTreeBase::OnDeleteItem(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	// Remove the tree item from the map.
	m_mapColorFont.RemoveKey(pNMTreeView->itemOld.hItem);
	*pResult = 0;

	return FALSE;
}

bool CXTPTreeBase::HasEditLabels() const
{
	return ((m_pTreeCtrl->GetStyle() & TVS_EDITLABELS) == TVS_EDITLABELS);
}

void CXTPTreeBase::OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/)
{
	m_pTreeCtrl->Default();

	// MFCBUG: During label editing if the tree is resized while
	// there is an edit box open the edit box is not moved to the
	// new location of the tree item, but is left in it previous
	// location.

	if (HasEditLabels() && m_htiEdit)
	{
		CEdit* pEdit = m_pTreeCtrl->GetEditControl();
		if (pEdit && ::IsWindow(pEdit->m_hWnd))
		{
			CRect rcEdit;
			pEdit->GetWindowRect(&rcEdit);

			CRect rcItem;
			m_pTreeCtrl->GetItemRect(m_htiEdit, &rcItem, TRUE);

			rcItem.top -= 1;
			rcItem.left -= 3;
			rcItem.right = rcItem.left + rcEdit.Width();
			rcItem.bottom = rcItem.top + rcEdit.Height();

			pEdit->MoveWindow(&rcItem);
		}
	}
}

void CXTPTreeBase::OnMouseMove(UINT /*nFlags*/, CPoint point)
{
	if (m_pTreeCtrl->GetStyle() & TVS_TRACKSELECT)
	{
		UINT nHitFlags = 0;
		HTREEITEM hItemHit = m_pTreeCtrl->HitTest(point, &nHitFlags);

		HTREEITEM hItem = hItemHit && nHitFlags & TVHT_ONITEM ? hItemHit : NULL;

		if (m_htiLast != hItem)
		{
			m_htiLast = hItem;

			if (m_htiLast)
			{
				UINT_PTR uID = m_pTreeCtrl->SetTimer(HOVER_TIMER_PERIOD, 55, NULL);
				ASSERT(uID == HOVER_TIMER_PERIOD);
			}

			m_pTreeCtrl->Invalidate(FALSE);
		}
	}

	if (IsExplorerTheme())
	{
		// [IP]
		// we need to track hot items here
		// the code below is rather untested, and it's better
		// to be chanded to something more stable in the future

		UINT nHitFlags = 0;
		m_pTreeCtrl->HitTest(point, &nHitFlags);

		bool bInvalidate = false;

		// if expanding/contracting call base class.
		if (nHitFlags & TVHT_ONITEMBUTTON)
			bInvalidate = true;

		if (nHitFlags & TVHT_ONITEMINDENT)
			bInvalidate = true;

		if (bInvalidate)
			m_pTreeCtrl->Invalidate(FALSE);
	}

	m_pTreeCtrl->Default();
}

void CXTPTreeBase::OnTimer(UINT_PTR nIDEvent)
{
	if (m_pTreeCtrl->GetStyle() & TVS_TRACKSELECT)
	{
		if (nIDEvent == HOVER_TIMER_PERIOD)
		{
			if (m_htiLast != NULL)
			{
				CPoint pt(::GetMessagePos());
				CRect rc;
				m_pTreeCtrl->GetWindowRect(&rc);

				if (rc.PtInRect(pt) == FALSE)
				{
					m_pTreeCtrl->KillTimer(HOVER_TIMER_PERIOD);

					m_htiLast = NULL;
					m_pTreeCtrl->Invalidate(FALSE);
				}
			}
			else
			{
				m_pTreeCtrl->KillTimer(HOVER_TIMER_PERIOD);
			}
			return;
		}
	}

	m_pTreeCtrl->Default();
}

void CXTPTreeBase::OnNcMouseMove(UINT /*nHitTest*/, CPoint /*point*/)
{
	if (m_pTreeCtrl->GetStyle() & TVS_TRACKSELECT)
	{
		if (m_htiLast)
		{
			m_htiLast = NULL;
			m_pTreeCtrl->Invalidate(FALSE);
		}
	}

	m_pTreeCtrl->Default();
}

bool CXTPTreeBase::Init()
{
	if (!::IsWindow(m_pTreeCtrl->GetSafeHwnd()))
		return false;

	RefreshMetrics();

	if (m_pTreeCtrl)
		m_pTreeCtrl->Invalidate(FALSE);

	return true;
}

void CXTPTreeBase::SetExplorerTheme(BOOL bSet)
{
	if (XTPSystemVersion()->IsWinVistaOrGreater() && m_pWindowsTreeTheme->IsThemeActive())
	{
		m_bExplorerTheme = bSet;

		RefreshMetrics();
	}
}
