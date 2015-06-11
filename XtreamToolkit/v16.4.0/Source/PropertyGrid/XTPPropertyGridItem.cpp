// XTPPropertyGridItem.cpp : implementation of the CXTPPropertyGridItem class.
//
// This file is a part of the XTREME PROPERTYGRID MFC class library.
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
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPImageManager.h"
#include "Common/XTPMarkupRender.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPMaskEditT.h"
#include "Common/XTPColorManager.h"
#include <Common/ScrollBar/XTPScrollBase.h>
#include <Common/ScrollBar/XTPScrollBarCtrl.h>
#include <Common/ScrollBar/XTPScrollBarContainer.h>

#include "XTPPropertyGridDefines.h"
#include "XTPPropertyGridPaintManager.h"
#include "XTPPropertyGridInplaceEdit.h"
#include "XTPPropertyGridInplaceButton.h"
#include "XTPPropertyGridInplaceList.h"
#include "XTPPropertyGridItem.h"
#include "XTPPropertyGridToolBar.h"
#include "XTPPropertyGrid.h"
#include "XTPPropertyGridToolTip.h"
#include "XTPPropertyGridView.h"

#ifdef _XTP_ACTIVEX
#include "PropertyGrid/Includes.h"
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTPPropertyGridItem

IMPLEMENT_DYNAMIC(CXTPPropertyGridItem, CCmdTarget)

CXTPPropertyGridItem::CXTPPropertyGridItem(LPCTSTR lpszCaption, LPCTSTR strValue, CString* pBindString)
{
	Init();
	SetPrompt(lpszCaption);
	m_strDefaultValue = m_strValue = strValue;

	BindToString(pBindString);
}

CXTPPropertyGridItem::CXTPPropertyGridItem(UINT nID, LPCTSTR strValue, CString* pBindString)
{
	Init();
	m_nID = nID;

	CString strPrompt;
	if (strPrompt.LoadString(nID))
	{
		SetPrompt(strPrompt);
	}

	m_strDefaultValue = m_strValue = strValue;
	BindToString(pBindString);
}

CXTPPropertyGridItem::~CXTPPropertyGridItem()
{
	Clear();

	XTPMarkupReleaseElement(m_pMarkupCaption);
	XTPMarkupReleaseElement(m_pMarkupDescription);
	XTPMarkupReleaseElement(m_pMarkupValue);
}

void CXTPPropertyGridItem::Clear()
{
	if (m_pGrid && m_pGrid->m_pSelected == this)
	{
		m_pGrid->m_pSelected = NULL;
	}

	if (m_pGrid && m_pGrid->m_pHotButton && m_pInplaceButtons)
	{
		for (int i = 0; i < m_pInplaceButtons->GetCount(); i++)
		{
			if (m_pInplaceButtons->GetAt(i) == m_pGrid->m_pHotButton)
			{
				m_pGrid->m_pHotButton = NULL;
				break;
			}
		}
	}

	if (m_pGrid && GetInplaceList().m_pItem == this)
	{
		GetInplaceList().DestroyItem();
	}
	if (m_pGrid && GetInplaceEdit().m_pItem == this)
	{
		GetInplaceEdit().DestroyItem();
	}

	if (m_pChilds)
	{
		m_pChilds->Clear();
		CMDTARGET_RELEASE(m_pChilds);
	}

	if (m_pConstraints)
	{
		m_pConstraints->m_pItem = NULL;
		CMDTARGET_RELEASE(m_pConstraints);
	}

	CMDTARGET_RELEASE(m_pValueMetrics);
	CMDTARGET_RELEASE(m_pCaptionMetrics);

	if (m_pInplaceButtons)
	{
		m_pInplaceButtons->RemoveAll();

		m_pInplaceButtons->m_pItem = NULL;
		CMDTARGET_RELEASE(m_pInplaceButtons);
	}

	SAFE_DELETE(m_pInplaceControls);

	m_pGrid = NULL;
}

void CXTPPropertyGridItem::SetPrompt(LPCTSTR lpszPrompt)
{
	if (lpszPrompt == 0)
		return;

	LPCTSTR lpszDescription = _tcschr(lpszPrompt, _T('\n'));

	CString strCaption;

	if (lpszDescription == NULL)
	{
		strCaption = lpszPrompt;
	}
	else
	{
		int nLen = (int)(lpszDescription - lpszPrompt);
		LPTSTR lpszCaption = strCaption.GetBufferSetLength(nLen);
		MEMCPY_S(lpszCaption, lpszPrompt, nLen * sizeof(TCHAR));

		m_strDescription = lpszDescription + 1;
	}

	SetCaption(strCaption);
}

void CXTPPropertyGridItem::Init()
{
	m_bHidden = FALSE;
	m_bExpanded = FALSE;
	m_nIndex = -1;
	m_bVisible = FALSE;
	m_pParent = 0;
	m_pGrid = 0;
	m_nIndent = 0;
	m_strValue = "";
	m_bReadOnly = FALSE;
	m_bCategory = FALSE;
	m_nID = 0;
	m_nFlags = xtpGridItemHasEdit;
	m_pConstraints = new CXTPPropertyGridItemConstraints(this);
	m_pInplaceButtons = new CXTPPropertyGridInplaceButtons(this);
	m_pInplaceControls = new CXTPPropertyGridInplaceControls();
	m_pConstraints->SetCurrent(-1);
	m_pBindString = 0;
	m_bConstraintEdit = FALSE;

	m_bAutoComplete = FALSE;

	m_nLinesCount = 1;
	m_nHeight = -1;

	m_nDropDownItemCount = 10;

	m_bExpandable = TRUE;

	m_bUseMask = FALSE;
	m_bPassword = FALSE;
	m_chPrompt = _T('*');

	m_dwData = 0;
	m_dwEditStyle = ES_AUTOHSCROLL;

	m_pMarkupCaption     = NULL;
	m_pMarkupDescription = NULL;
	m_pMarkupValue       = NULL;

	EnableAutomation();

#ifdef _XTP_ACTIVEX
	m_nType = 0;
	EnableTypeLib();
#endif
	m_pValueMetrics = 0;
	m_pCaptionMetrics = 0;


	m_pChilds = new CXTPPropertyGridItems();
}

void CXTPPropertyGridItem::OnAddChildItem()
{
	if (!m_bCategory) SetFlags(m_nFlags);
}

void CXTPPropertyGridItem::SetFlags(UINT nFlags)
{
	ASSERT(!m_bCategory || ((nFlags & xtpGridItemHasEdit) == 0));
	m_nFlags = nFlags;

	m_pInplaceButtons->Remove(XTP_ID_PROPERTYGRID_EXPANDBUTTON);
	m_pInplaceButtons->Remove(XTP_ID_PROPERTYGRID_COMBOBUTTON);

	if (m_nFlags & xtpGridItemHasExpandButton)
		m_pInplaceButtons->AddExpandButton();

	if (m_nFlags & xtpGridItemHasComboButton)
		m_pInplaceButtons->AddComboButton();
}

UINT CXTPPropertyGridItem::GetFlags() const
{
	return m_nFlags;
}

CXTPPropertyGridItemMetrics* CXTPPropertyGridItem::GetValueMetrics() const
{
	if (m_pValueMetrics == 0)
		m_pValueMetrics = new CXTPPropertyGridItemMetrics(m_pGrid->GetPaintManager());
	return m_pValueMetrics;
}

CXTPPropertyGridItemMetrics* CXTPPropertyGridItem::GetCaptionMetrics() const
{
	if (m_pCaptionMetrics == 0)
		m_pCaptionMetrics = new CXTPPropertyGridItemMetrics(m_pGrid->GetPaintManager());
	return m_pCaptionMetrics;
}

CXTPPropertyGridItemMetrics* CXTPPropertyGridItem::GetMetrics(BOOL bValueMetrics, BOOL bCreate /*= TRUE*/) const
{
	if (bCreate)
		return bValueMetrics ? GetValueMetrics() : GetCaptionMetrics();

	return bValueMetrics ? m_pValueMetrics : m_pCaptionMetrics;
}

CXTPPropertyGridItem* CXTPPropertyGridItem::AddChildItem(CXTPPropertyGridItem* pItem)
{
	return InsertChildItem(m_pChilds->GetCount(), pItem);
}

void CXTPPropertyGridItem::SetIndent(int nIndent)
{
	m_nIndent = nIndent;

	for (int i = 0; i < m_pChilds->GetCount(); i++)
	{
		m_pChilds->GetAt(i)->SetIndent(nIndent + 1);
	}
}

CXTPPropertyGridItem* CXTPPropertyGridItem::InsertChildItem(int nIndex, CXTPPropertyGridItem* pItem)
{
	ASSERT(pItem);
	if (!pItem)
		return NULL;

	ASSERT(!pItem->IsCategory() || IsCategory());
	ASSERT(m_pGrid != NULL);
	if (!m_pGrid)
		return pItem;

	if (nIndex < 0 || nIndex > m_pChilds->GetCount())
		nIndex = m_pChilds->GetCount();

	m_pChilds->InsertAt(nIndex, pItem);
	pItem->m_pGrid = m_pGrid;

	ASSERT(pItem->m_pParent == NULL);
	pItem->m_pParent = this;
	pItem->SetIndent(m_nIndent + 1);

	pItem->OnCaptionChanged();

	pItem->OnAddChildItem();

	if (m_bVisible)
	{
		if (m_bExpanded)
			m_pGrid->SetPropertySort(m_pGrid->m_properetySort, TRUE);
		else if (m_pGrid->GetSafeHwnd() && m_pChilds->GetCount() == 1)
			m_pGrid->Invalidate(FALSE);
	}

	return pItem;
}

void CXTPPropertyGridItem::Move(CXTPPropertyGridItem* pParent, int nIndex /* = -1 */)
{
	if (!pParent)
		return;

	InternalAddRef();
	Remove();
	pParent->InsertChildItem(nIndex, this);
}

void CXTPPropertyGridItem::Collapse()
{
	if (!m_bExpanded) return;

	if (m_pGrid->m_nLockUpdate == 0)
	{
		if (m_pGrid->GetSafeHwnd() && m_bVisible)
		{
			m_pGrid->_DoCollapse(this);
			m_pGrid->OnSelectionChanged();
		}
	}

	m_bExpanded = FALSE;

	m_pGrid->SendNotifyMessage(XTP_PGN_ITEMEXPANDCHANGED, (LPARAM)this);
}

void CXTPPropertyGridItem::Expand()
{
	if (m_bExpanded) return;

	if (m_pGrid->m_nLockUpdate == 0)
	{
		if (m_pGrid->GetSafeHwnd() && m_bVisible)
		{
			m_pGrid->_DoExpand(this, m_nIndex);
			m_pGrid->_RefreshIndexes();
			m_pGrid->OnSelectionChanged();
		}
	}

	m_bExpanded = TRUE;

	m_pGrid->SendNotifyMessage(XTP_PGN_ITEMEXPANDCHANGED, (LPARAM)this);
}

BOOL CXTPPropertyGridItem::HasParent(CXTPPropertyGridItem* pParent)
{
	if (m_pParent == NULL) return FALSE;
	if (m_pParent == pParent) return TRUE;
	return m_pParent->HasParent(pParent);
}

void CXTPPropertyGridItem::Select()
{
	ASSERT(m_pGrid);
	if (!m_pGrid)
		return;

	EnsureVisible();

	if (!m_bVisible)
		return;

	if (m_pGrid->GetStyle() & LBS_MULTIPLESEL)
	{
		m_pGrid->SelItemRange(FALSE, 0, m_pGrid->GetCount());
		m_pGrid->SetSel(m_nIndex);
	}
	else
	{
		m_pGrid->SetCurSel(m_nIndex);
	}
	m_pGrid->OnSelectionChanged();
}

void CXTPPropertyGridItem::OnDeselect()
{
	if (m_pGrid) m_pGrid->FocusInplaceButton(NULL);
	OnValidateEdit();

	for (int i = m_pInplaceControls->GetCount() - 1; i >= 0; i--)
	{
		CXTPPropertyGridInplaceControl* pControl = m_pInplaceControls->GetAt(i);
		pControl->OnDestroyWindow();
	}
}

void CXTPPropertyGridItem::OnValidateEdit()
{
	CXTPPropertyGridInplaceEdit& wndEdit = GetInplaceEdit();
	wndEdit.HideWindow();

	// if Inside Hide window executed handler that refresh it, just exit.
	if (wndEdit.GetSafeHwnd() && wndEdit.GetStyle() & WS_VISIBLE)
	{
		return;
	}

	if (wndEdit.GetSafeHwnd() && wndEdit.m_pItem == this)
	{
		CString strValue;
		wndEdit.GetWindowText(strValue);
		wndEdit.m_pItem = NULL;

		if (wndEdit.m_bCancel)
		{
			OnCancelEdit();
		}
		else if (wndEdit.m_bEditMode && OnAfterEdit(strValue))
		{
			if (m_strValue != strValue && !wndEdit.m_bCancel)
			{
				OnValueChanged(strValue);
			}
		}
	}
}

void CXTPPropertyGridItem::SetEditText(const CString& str)
{
	if (!m_pGrid) return;

	CXTPPropertyGridInplaceEdit& wndEdit = GetInplaceEdit();
	if (wndEdit.GetSafeHwnd() && wndEdit.m_pItem == this)
	{
		wndEdit.SetWindowText(str);
		wndEdit.SetFont(m_pGrid->GetPaintManager()->GetItemFont(this, TRUE));
		wndEdit.SetMargins(3, 3);
	}
}

void CXTPPropertyGridItem::SetMultiLinesCount(int nLinesCount)
{
	if (m_nLinesCount == nLinesCount)
		return;

	m_nLinesCount = nLinesCount;

	RefreshHeight();
}

void CXTPPropertyGridItem::SetHeight(int nHeight)
{
	if (m_nHeight == nHeight)
		return;

	m_nHeight = nHeight;

	RefreshHeight();
}

void CXTPPropertyGridItem::RefreshHeight()
{
	if (IsVisible() && m_pGrid->GetSafeHwnd() && m_pGrid->GetStyle() & LBS_OWNERDRAWVARIABLE && m_nIndex >= 0)
	{
		MEASUREITEMSTRUCT measureItemStruct =
		{
			ODT_LISTBOX, 0, m_nIndex, 0, 0, (ULONG_PTR)this
		};
		m_pGrid->MeasureItem(&measureItemStruct);
		m_pGrid->SetItemHeight(m_nIndex, measureItemStruct.itemHeight);
		m_pGrid->Invalidate(FALSE);
	}
}

void CXTPPropertyGridItem::OnMarkupChanged()
{
	// update markup
	BOOL bMarkupChanged = (NULL != m_pMarkupValue);
	XTPMarkupReleaseElement(m_pMarkupValue);

	if (!m_pGrid || !m_pGrid->m_pGrid)
		return;

	if (m_pGrid->m_pGrid->GetMarkupContext() && GetMarkupTemplate().GetLength())
	{
		CString final_markup = GetMarkupTemplate();
		final_markup.Replace(XTP_PG_ITEMVALUE_TO_REPLACE_IN_MARKUP, GetValue());

		m_pMarkupValue = XTPMarkupParseText(m_pGrid->m_pGrid->GetMarkupContext(), final_markup);
		bMarkupChanged = TRUE;
	}

	if(bMarkupChanged)
	{
		m_pGrid->Refresh(); // to force call MeasureItem, markup should be changed
	}
}

void CXTPPropertyGridItem::SetValue(CString strValue)
{
	if (m_strValue == strValue)
		return;

	m_strValue = strValue;

	if (m_pBindString)
	{
		*m_pBindString = strValue;
	}
#ifdef _XTP_ACTIVEX
	BindDispatch();
#endif

	SetEditText(m_strValue);

	for (int i = m_pInplaceControls->GetCount() - 1; i >= 0; i--)
	{
		CXTPPropertyGridInplaceControl* pControl = m_pInplaceControls->GetAt(i);
		pControl->OnValueChanged();
	}

	OnMarkupChanged();

	SAFE_INVALIDATE(m_pGrid);
}

void CXTPPropertyGridItem::SetMarkupTemplate(LPCTSTR lpstrMarkupTemplate)
{
	if (m_strMarkupTemplate != lpstrMarkupTemplate)
	{
		m_strMarkupTemplate = lpstrMarkupTemplate;

		OnMarkupChanged();
	}
}

void CXTPPropertyGridItem::OnValueChanged(CString strValue)
{
	if (!m_pGrid)
		return;

	SetValue(strValue);
	NotifyValueChanged();
}

void CXTPPropertyGridItem::NotifyValueChanged()
{
	if (m_pGrid && m_pGrid->GetSafeHwnd()) m_pGrid->SendNotifyMessage(XTP_PGN_ITEMVALUE_CHANGED, (LPARAM)this);
}

BOOL CXTPPropertyGridItem::OnRequestEdit()
{
	XTP_PROPERTYGRID_NOTIFYEDIT notify;
	notify.bCancel = FALSE;
	notify.pItem = this;

	if (m_pGrid)
	{
		m_pGrid->SendNotifyMessage(XTP_PGN_REQUESTEDIT, (LPARAM)&notify);
	}
	return !notify.bCancel;
}

BOOL CXTPPropertyGridItem::OnAfterEdit(CString& strValue)
{
	if (!m_pGrid)
		return FALSE;

	XTP_PROPERTYGRID_NOTIFYEDIT notify;
	notify.bCancel = FALSE;
	notify.strValue = strValue;
	notify.pItem = this;

	if (m_pGrid)
	{
		m_pGrid->SendNotifyMessage(XTP_PGN_AFTEREDIT, (LPARAM)&notify);
		strValue = notify.strValue;
	}
	return !notify.bCancel;
}

void CXTPPropertyGridItem::OnCancelEdit()
{
	XTP_PROPERTYGRID_NOTIFYEDIT notify;
	notify.bCancel = FALSE;
	notify.pItem = this;

	if (m_pGrid)
	{
		m_pGrid->SendNotifyMessage(XTP_PGN_CANCELEDIT, (LPARAM)&notify);
	}
}

void CXTPPropertyGridItem::OnSelect()
{
	ASSERT(m_bVisible);

	CString strValue(GetInPlacedEditValue());

	if (m_pInplaceControls->GetCount() > 0)
	{
		CRect rcValue(GetInplaceWindowsRect());

		for (int i = m_pInplaceControls->GetCount() - 1; i >= 0; i--)
		{
			CXTPPropertyGridInplaceControl* pControl = m_pInplaceControls->GetAt(i);
			pControl->OnCreateWindow(rcValue);
		}
	}

	if ((m_nFlags & xtpGridItemHasEdit) && !m_pGrid->m_bTracking)
	{
		CXTPPropertyGridInplaceEdit& wndEdit = GetInplaceEdit();

		wndEdit.SetValue(strValue);
		wndEdit.Create(this, GetValueRect());
		wndEdit.SetReadOnly(!IsAllowEdit() || m_bConstraintEdit);
	}
	else
	{
		GetInplaceEdit().HideWindow();
	}
}

DWORD CXTPPropertyGridItem::GetEditStyle() const
{
	return WS_CHILD | (m_bPassword ? ES_PASSWORD: 0) | (IsMultiLine() ? ES_MULTILINE : 0) | m_dwEditStyle;
}

void CXTPPropertyGridItem::OnInplaceButtonDown(CXTPPropertyGridInplaceButton* pButton)
{
	if (!IsAllowEdit())
		return;

	if (m_pGrid->SendNotifyMessage(XTP_PGN_INPLACEBUTTONDOWN, (LPARAM)pButton) == TRUE)
		return;

	if ((pButton->GetID() == XTP_ID_PROPERTYGRID_COMBOBUTTON) && !m_pConstraints->IsEmpty() && OnRequestEdit())
	{
		GetInplaceList().Create(this, GetItemRect());
		m_pGrid->Invalidate(FALSE);
	}
}

CRect CXTPPropertyGridItem::GetInplaceWindowsRect()
{
	ASSERT(m_bVisible);
	CRect rc = GetItemRect();
	CRect rcValue(m_pGrid->GetDividerPos() + 1, rc.top + 1, rc.right, rc.bottom - 1);

	for (int i = 0; i < m_pInplaceButtons->GetCount(); i++)
	{

		CXTPPropertyGridInplaceButton* pButton = m_pInplaceButtons->GetAt(i);
		if (pButton && IsInplaceButtonVisible(pButton))
			if (pButton->GetAlignment() & DT_RIGHT) rcValue.right -= pButton->GetWidth(); else rcValue.left += pButton->GetWidth();
	}

	m_pGrid->GetPaintManager()->AdjustItemValueRect(this, rcValue);

	return rcValue;
}

CRect CXTPPropertyGridItem::GetValueRect()
{
	CRect rcValue = GetInplaceWindowsRect();

	for (int i = m_pInplaceControls->GetCount() - 1; i >= 0; i--)
	{
		CXTPPropertyGridInplaceControl* pControl = m_pInplaceControls->GetAt(i);
		pControl->OnAdjustValueRect(rcValue);
	}

	return rcValue;
}

CRect CXTPPropertyGridItem::GetItemRect() const
{
	CRect rc;
	m_pGrid->GetItemRect(m_nIndex, rc);
	return rc;
}

BOOL CXTPPropertyGridItem::OnChar(UINT nChar)
{
	ASSERT(nChar != VK_TAB);

	if ((m_nFlags & xtpGridItemHasEdit) && (nChar != VK_ESCAPE))
	{
		OnSelect();

		CXTPPropertyGridInplaceEdit& wndEdit = GetInplaceEdit();
		if (wndEdit.GetSafeHwnd() && wndEdit.IsWindowVisible() && wndEdit.GetItem() == this)
		{
			wndEdit.SetFocus();
			wndEdit.SetSel(0, -1);
			if ((nChar != VK_TAB) && (wndEdit.GetItem() == this)) wndEdit.SendMessage(WM_CHAR, nChar);
		}

		return TRUE;
	}
	else if (nChar == VK_TAB)
	{
		if (m_pInplaceButtons->GetCount() > 0)
		{
			m_pGrid->GetPropertyGrid()->OnNavigate(xtpGridUIInplaceEdit, TRUE, this);
		}
	}
	return FALSE;
}

BOOL CXTPPropertyGridItem::IsValueChanged() const
{
	return m_strDefaultValue != m_strValue;
}

void CXTPPropertyGridItem::ResetValue()
{
	OnValueChanged(m_strDefaultValue);
}

void CXTPPropertyGridItem::SetDefaultValue(LPCTSTR lpszDefaultValue)
{
	m_strDefaultValue = lpszDefaultValue;
}

void CXTPPropertyGridItem::OnLButtonUp(UINT /*nFlags*/, CPoint /*point*/)
{

}

BOOL CXTPPropertyGridItem::PtInValueRect(CPoint point)
{
	CRect rc(GetValueRect());
	return rc.PtInRect(point);
}

void CXTPPropertyGridItem::OnLButtonDblClk(UINT /*nFlags*/, CPoint point)
{
	CXTPPropertyGridInplaceButton* pInplaceButton = m_pInplaceButtons->HitTest(point);
	if (pInplaceButton && IsInplaceButtonVisible(pInplaceButton))
	{
		OnSelect();
		return;
	}

	if (HasChilds() && IsExpandable())
	{
		if (m_bExpanded)
			Collapse();
		else
			Expand();
	}
	else
	{
		OnSelect();

		if (m_nFlags & xtpGridItemHasEdit)
		{
			CXTPPropertyGridInplaceEdit& wndEdit = GetInplaceEdit();
			if (wndEdit.GetSafeHwnd() && wndEdit.IsWindowVisible() && wndEdit.GetItem() == this)
			{
				wndEdit.SetFocus();

				if (wndEdit.GetItem() == this && IsAllowEdit() && wndEdit.SelectConstraint(+1, TRUE))
				{
					OnValidateEdit();

				}
				else
				{
					wndEdit.SetSel(0, -1);
				}
			}
		}
		else if (IsAllowEdit())
		{
			SelectNextConstraint();
		}
	}
}

BOOL CXTPPropertyGridItem::SelectNextConstraint()
{
	CXTPPropertyGridItemConstraints* pList = GetConstraints();
	if (pList->IsEmpty())
		return FALSE;

	if (!OnRequestEdit())
		return FALSE;

	int nIndex = pList->FindConstraint(m_strValue);

	nIndex += +1;
	if (nIndex >= pList->GetCount()) nIndex = 0;
	if (nIndex < 0) nIndex = (ULONG)pList->GetCount() - 1;

	if (!pList->GetConstraintAt(nIndex)->m_bEnabled)
		return FALSE;

	CString strValue = pList->GetAt(nIndex);

	if (!OnAfterEdit(strValue))
		return FALSE;

	pList->SetCurrent(nIndex);
	OnValueChanged(strValue);
	return TRUE;
}

void CXTPPropertyGridItem::RedrawItem()
{
	if (m_pGrid && m_pGrid->GetSafeHwnd() && m_bVisible)
	{
		m_pGrid->InvalidateRect(GetItemRect(), FALSE);
	}
}

BOOL CXTPPropertyGridItem::OnInpaceControlFocus(BOOL /*bForward*/)
{
	if (GetInplaceControls()->GetCount() > 0)
	{
		CWnd* pWnd = GetInplaceControls()->GetAt(0);
		if (pWnd && pWnd->GetSafeHwnd() && pWnd->GetStyle() & WS_TABSTOP)
		{
			pWnd->SetFocus();
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CXTPPropertyGridItem::IsAllowEdit() const
{
	return !GetReadOnly() && GetGrid()->GetPropertyGrid()->m_bAllowEdit;
}

BOOL CXTPPropertyGridItem::OnLButtonDown(UINT nFlags, CPoint point)
{
	BOOL bOldSelected = IsSelected();

	if (GetGrid()->GetStyle() & LBS_MULTIPLESEL)
	{
		GetGrid()->Default();
		GetGrid()->OnSelectionChanged();
		ReleaseCapture();

	}
	else
		Select();

	CXTPPropertyGridInplaceButton* pInplaceButton = m_pInplaceButtons->HitTest(point);
	if (pInplaceButton && IsInplaceButtonVisible(pInplaceButton))
	{
		if (!bOldSelected && !m_pGrid->GetPropertyGrid()->GetShowInplaceButtonsAlways() && !pInplaceButton->GetShowAlways())
			return TRUE;
		pInplaceButton->OnLButtonDown(nFlags, point);
		return TRUE;
	}

	int nOffset = (m_nIndent - (IsCategory() ? 0 : 1)) * XTP_PGI_EXPAND_BORDER;

	if ((point.x >= nOffset && point.x <= XTP_PGI_EXPAND_BORDER + nOffset) && IsExpandable())
	{
		if (m_bExpanded)
			Collapse();
		else
			Expand();
	}

	if ((m_nFlags & xtpGridItemHasEdit) && PtInValueRect(point))
	{
		mouse_event(GetSystemMetrics(SM_SWAPBUTTON) ? MOUSEEVENTF_RIGHTDOWN : MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	}

	if (m_nFlags == xtpGridItemHasComboButton && PtInValueRect(point) && IsAllowEdit()
		&& (m_pInplaceButtons->GetCount() >= 1) && !IsCategory())
	{
		OnInplaceButtonDown(m_pInplaceButtons->GetAt(0));
	}

	return TRUE;
}

void CXTPPropertyGridItem::BindToString(CString* pBindString)
{
	m_pBindString = pBindString;
	if (m_pBindString)
	{
		*m_pBindString = m_strValue;
	}
}

void CXTPPropertyGridItem::OnBeforeInsert()
{
	if (m_pBindString && *m_pBindString != m_strValue)
	{
		m_strValue = *m_pBindString;
	}
}

void CXTPPropertyGridItem::EnsureVisible()
{
	if (!m_bVisible)
	{
		CXTPPropertyGridItem* pItem = GetParentItem();

		while (pItem)
		{
			pItem->Expand();
			pItem = pItem->GetParentItem();
		}
	}

	if (!m_bVisible || (m_nIndex == -1))
		return;

	if (m_nIndex < m_pGrid->GetTopIndex())
	{
		m_pGrid->SetTopIndex(m_nIndex);
		return;
	}

	int top = 0;
	int nTopIndex = m_pGrid->GetTopIndex();
	int bottom = CXTPClientRect(m_pGrid).bottom;

	for (int i = nTopIndex; i < m_pGrid->GetCount(); i++)
	{
		int nRowHeight = m_pGrid->GetItemHeight(i);

		if (top + nRowHeight > bottom)
			break;

		if (i == m_nIndex)
			return;

		top += nRowHeight;
	}

	int nHeight = bottom;
	for (top = m_nIndex; top >= 0; top--)
	{
		int nRowHeight = m_pGrid->GetItemHeight(top);

		if (nHeight - nRowHeight < 0)
		{
			if (top != m_nIndex)
				top++;
			break;
		}

		nHeight -= nRowHeight;
	}
	m_pGrid->SetTopIndex(top);
}

CXTPPropertyGridInplaceEdit& CXTPPropertyGridItem::GetInplaceEdit()
{
	if (m_pGrid) return *m_pGrid->m_pGrid->m_pInplaceEdit;

	static CXTPPropertyGridInplaceEdit  wndEdit;
	return wndEdit;
}

CXTPPropertyGridInplaceList& CXTPPropertyGridItem::GetInplaceList()
{
	if (m_pGrid) return *m_pGrid->m_pGrid->m_pInplaceListBox;

	static CXTPPropertyGridInplaceList  wndListBox;
	return wndListBox;
}

BOOL CXTPPropertyGridItem::IsInplaceButtonVisible(CXTPPropertyGridInplaceButton* pButton) const
{
	if (!m_pGrid || !pButton)
		return FALSE;

	if (!IsAllowEdit())
		return FALSE;

	if (!IsSelected() && !m_pGrid->GetPropertyGrid()->GetShowInplaceButtonsAlways() && !pButton->GetShowAlways())
		return FALSE;

	return TRUE;
}

CXTPPropertyGridInplaceButtons* CXTPPropertyGridItem::GetInplaceButtons() const
{
	return m_pInplaceButtons;
}

CXTPPropertyGridInplaceControls* CXTPPropertyGridItem::GetInplaceControls() const
{
	return m_pInplaceControls;
}

// Deletes the item.
void CXTPPropertyGridItem::Remove()
{
	if (!m_pGrid)
		return;

	CXTPPropertyGridItems* pItems = GetParentItem() == 0 ? m_pGrid->m_pCategories : GetParentItem()->m_pChilds;
	ASSERT(pItems);
	int nIndex = pItems ? pItems->Find(this) : -1;

	if (nIndex == -1)
		return;

	pItems->m_arrItems.RemoveAt(nIndex);

	m_pParent = NULL;

	if (m_pGrid) m_pGrid->Refresh();
	InternalRelease();
}

BOOL CXTPPropertyGridItem::IsHidden() const
{
	return m_bHidden;
}
void CXTPPropertyGridItem::SetHidden(BOOL bHidden)
{
	if (bHidden == m_bHidden)
		return;

	m_bHidden = bHidden;

	if (m_bVisible || !m_bHidden)
		m_pGrid->Refresh();
}

BOOL CXTPPropertyGridItem::HasVisibleChilds() const
{
	for (int i = 0; i < m_pChilds->GetCount(); i++)
	{
		if (!m_pChilds->GetAt(i)->IsHidden())
			return TRUE;
	}
	return FALSE;
}

BOOL CXTPPropertyGridItem::IsSelected() const
{
	if (!m_pGrid->GetSafeHwnd())
		return FALSE;

	if (!m_bVisible)
		return FALSE;

	int nCount = m_pGrid->GetSelCount();

	if (nCount != LB_ERR && nCount > 1)
	{
		CArray<int,int> arr;

		arr.SetSize(nCount);
		m_pGrid->GetSelItems(nCount, arr.GetData());

		for (int i = 0; i < nCount; i++)
		{
			if (arr[i] == m_nIndex)
				return TRUE;
		}
	}

	return m_pGrid && m_pGrid->GetSelectedItem() == this;
}

void CXTPPropertyGridItem::Refresh(BOOL bInvalidate /*= TRUE*/)
{
	if (m_pGrid && m_pGrid->GetSafeHwnd())
	{
		if (IsSelected())
		{
			OnDeselect();
			OnSelect();

		}
		if (bInvalidate)
		{
			m_pGrid->Invalidate(FALSE);
		}
	}
}

void CXTPPropertyGridItem::SetReadOnly(BOOL bReadOnly)
{
	if (m_bReadOnly != bReadOnly)
	{
		m_bReadOnly = bReadOnly;
		Refresh(TRUE);
	}
}

void CXTPPropertyGridItem::SetMask(LPCTSTR strMask, LPCTSTR strLiteral, TCHAR chPrompt /*= _T('_')*/)
{
	m_strMask = strMask;
	m_strLiteral = strLiteral;
	m_chPrompt = chPrompt;
	m_bUseMask = !m_strMask.IsEmpty();

	Refresh(FALSE);
}

CString CXTPPropertyGridItem::GetViewValue()
{
	CString str = GetValue();
	if (m_bPassword)
	{
		return CString(m_chPrompt, str.GetLength());
	}
	return str;
}

CString CXTPPropertyGridItem::GetMaskedText() const
{
	if (m_bUseMask)
	{
		int nLength = m_strValue.GetLength();

		CString strBuffer;
		for (int i = 0; i < nLength; ++i)
		{
			if (m_strLiteral[i] == m_chPrompt)
			{
				strBuffer += m_strValue.GetAt(i);
			}
		}

		return strBuffer;
	}
	else
	{
		return m_strValue;
	}
}

void CXTPPropertyGridItem::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	if (GetMarkupValue() && m_pGrid)
	{
		CXTPPropertyGridPaintManager *pPaintManager = m_pGrid->GetPaintManager();

		CRect rc = GetItemRect();

		CRect rcCaption(rc.left, rc.top, rc.left + m_pGrid->GetDividerPos(), rc.bottom);
		CRect rcValue(rcCaption.right, rc.top, rc.right, rc.bottom - 1);

		XTPMarkupSetDefaultFont(XTPMarkupElementContext(GetMarkupValue()),
			(HFONT)pPaintManager->GetItemFont(this, FALSE)->GetSafeHandle(), pPaintManager->GetItemTextColor(this, TRUE));

		CSize sz = XTPMarkupMeasureElement(GetMarkupValue(), rcValue.Width(), INT_MAX);

		lpMeasureItemStruct->itemHeight = max(lpMeasureItemStruct->itemHeight, (UINT)sz.cy);
	}
	else
	{
		if (m_nHeight != -1)
		{
			lpMeasureItemStruct->itemHeight = m_nHeight;
		}
		else if (m_nLinesCount > 1)
		{
			lpMeasureItemStruct->itemHeight = (lpMeasureItemStruct->itemHeight - 4) * m_nLinesCount + 4;

		}
	}
}


void CXTPPropertyGridItem::SetMaskedText(LPCTSTR lpszMaskedText)
{
	CString strMaskedText = lpszMaskedText;

	if (m_bUseMask)
	{
		int nLen = strMaskedText.GetLength();
		int x = 0;

		m_strValue = m_strLiteral;

		for (int i = 0; (i < m_strValue.GetLength()) && (x < nLen); i++)
		{
			if (strMaskedText[x] == m_strValue[i])
			{
				x++;
			}
			else if (m_strValue[i] == m_chPrompt)
			{
				m_strValue.SetAt(i, strMaskedText[x]);
				x++;
			}
		}
	}
	else
	{
		m_strValue = strMaskedText;
	}

	SetValue(m_strValue);
}

void CXTPPropertyGridItem::SetDescription(LPCTSTR lpszDescription)
{
	CString strDescription = lpszDescription;

	if (m_strDescription == strDescription)
		return;

	XTPMarkupReleaseElement(m_pMarkupDescription);

	m_strDescription = strDescription;

	if (m_pGrid && m_pGrid->m_pGrid && m_pGrid->m_pGrid->GetMarkupContext())
	{
		m_pMarkupDescription = XTPMarkupParseText(m_pGrid->m_pGrid->GetMarkupContext(), m_strDescription);
	}

	if (IsSelected()) GetGrid()->GetParent()->Invalidate(FALSE);
}

int CXTPPropertyGridItem::GetIndex() const
{
	return m_nIndex;
}


void CXTPPropertyGridItem::OnDrawItemConstraint(CDC* pDC, CXTPPropertyGridItemConstraint* pConstraint, CRect rc, BOOL bSelected)
{
	GetGrid()->GetPaintManager()->DrawInplaceListItem(pDC, pConstraint, rc, bSelected);
}

CSize CXTPPropertyGridItem::OnMergeItemConstraint(CDC* pDC, CXTPPropertyGridItemConstraint* pConstraint)
{
	int nThumbLength = GetSystemMetrics(SM_CXHTHUMB);

	CSize sz = pDC->GetTextExtent(pConstraint->m_strConstraint);

	return CSize(sz.cx + nThumbLength * 2, sz.cy + 3);
}

void CXTPPropertyGridItem::SetFocusToInplaceControl()
{
	CXTPPropertyGridInplaceEdit& wndEdit = GetInplaceEdit();

	if (wndEdit.GetSafeHwnd() && wndEdit.GetItem() == this)
	{
		wndEdit.SetFocus();
		wndEdit.SetSel(0, -1);
	}
}

void CXTPPropertyGridItem::AddComboButton()
{
	GetInplaceButtons()->AddComboButton();
}

void CXTPPropertyGridItem::AddExpandButton()
{
	GetInplaceButtons()->AddExpandButton();
}

CXTPPropertyGridInplaceSlider* CXTPPropertyGridItem::AddSliderControl()
{
	CXTPPropertyGridInplaceSlider* pSlider = new CXTPPropertyGridInplaceSlider(this);
	m_pInplaceControls->Add(pSlider);

	return pSlider;
}

CXTPPropertyGridInplaceSpinButton* CXTPPropertyGridItem::AddSpinButton()
{
	CXTPPropertyGridInplaceSpinButton* pSpinButton = new CXTPPropertyGridInplaceSpinButton(this);
	m_pInplaceControls->Add(pSpinButton);

	return pSpinButton;
}

void CXTPPropertyGridItem::OnCaptionChanged()
{
	XTPMarkupReleaseElement(m_pMarkupCaption);

	if (!m_pGrid || !m_pGrid->m_pGrid)
		return;

	if (m_pGrid->m_pGrid->GetMarkupContext())
	{
		m_pMarkupCaption = XTPMarkupParseText(m_pGrid->m_pGrid->GetMarkupContext(), GetCaption());
	}

	RedrawItem();
}


int CXTPPropertyGridItem::NextNumber(LPCTSTR& str)
{
	int nResult = 0;

	if (!str || !*str)
		return nResult;

	while (*str && *str != '-' && (*str < '0' || *str > '9'))
		str++;

	BOOL bHasSign = *str == '-';
	if (bHasSign)
		str++;

	while (*str >= '0' && *str <= '9')
	{
		nResult = nResult * 10 + (*str - '0');
		str++;
	}

	return bHasSign ? -nResult : nResult;
}

//////////////////////////////////////////////////////////////////////////
// Accessible

CCmdTarget* CXTPPropertyGridItem::GetAccessible()
{
	return this;
}

HRESULT CXTPPropertyGridItem::GetAccessibleParent(IDispatch* FAR* ppdispParent)
{
	SAFE_MANAGE_STATE(m_pModuleState);

	*ppdispParent = NULL;

	return m_pGrid->InternalQueryInterface(&IID_IAccessible, (void**)ppdispParent);
}

HRESULT CXTPPropertyGridItem::GetAccessibleName(VARIANT varChild, BSTR* pszName)
{
	if (GetChildIndex(&varChild) != CHILDID_SELF)
		return E_INVALIDARG;

	CString strCaption = GetCaption();

	*pszName = strCaption.AllocSysString();
	return S_OK;
}

HRESULT CXTPPropertyGridItem::GetAccessibleDescription(VARIANT varChild, BSTR* pszDescription)
{
	if (GetChildIndex(&varChild) != CHILDID_SELF)
		return E_INVALIDARG;

	*pszDescription = GetDescription().AllocSysString();
	return S_OK;
}

HRESULT CXTPPropertyGridItem::GetAccessibleRole(VARIANT varChild, VARIANT* pvarRole)
{
	if (!pvarRole)
		return E_INVALIDARG;

	pvarRole->vt = VT_EMPTY;

	if (GetChildIndex(&varChild) == CHILDID_SELF)
	{
		pvarRole->vt = VT_I4;
		pvarRole->lVal = ROLE_SYSTEM_ROW;
		return S_OK;
	}

	return E_INVALIDARG;
}

HRESULT CXTPPropertyGridItem::GetAccessibleState(VARIANT varChild, VARIANT* pvarState)
{
	if (!pvarState)
		return E_INVALIDARG;

	if (GetChildIndex(&varChild) != CHILDID_SELF)
		return E_INVALIDARG;

	pvarState->vt = VT_I4;
	pvarState->lVal = STATE_SYSTEM_FOCUSABLE | STATE_SYSTEM_SELECTABLE |
		(IsSelected() ? STATE_SYSTEM_FOCUSED : 0);

	if (!IsVisible())
		pvarState->lVal |= STATE_SYSTEM_INVISIBLE;

	if (GetReadOnly())
		pvarState->lVal |= STATE_SYSTEM_READONLY;

	return S_OK;
}


HRESULT CXTPPropertyGridItem::AccessibleLocation(long* pxLeft, long* pyTop, long* pcxWidth, long* pcyHeight, VARIANT varChild)
{
	*pxLeft = *pyTop = *pcxWidth = *pcyHeight = 0;

	if (GetChildIndex(&varChild) != CHILDID_SELF)
		return E_INVALIDARG;

	if (!m_pGrid->GetSafeHwnd())
		return S_OK;

	CRect rcItem;
	m_pGrid->GetItemRect(GetIndex(), &rcItem);
	m_pGrid->ClientToScreen(&rcItem);

	*pxLeft = rcItem.left;
	*pyTop = rcItem.top;
	*pcxWidth = rcItem.Width();
	*pcyHeight = rcItem.Height();

	return S_OK;
}

HRESULT CXTPPropertyGridItem::AccessibleHitTest(long xLeft, long yTop, VARIANT* pvarID)
{
	if (pvarID == NULL)
		return E_INVALIDARG;

	pvarID->vt = VT_EMPTY;

	if (!m_pGrid->GetSafeHwnd())
		return S_FALSE;

	if (!CXTPWindowRect(m_pGrid).PtInRect(CPoint(xLeft, yTop)))
		return S_FALSE;

	pvarID->vt = VT_I4;
	pvarID->lVal = 0;

	CRect rcItem;
	m_pGrid->GetItemRect(GetIndex(), &rcItem);
	m_pGrid->ClientToScreen(&rcItem);

	if (!rcItem.PtInRect(CPoint(xLeft, yTop)))
		return S_FALSE;

	return S_OK;
}

HRESULT CXTPPropertyGridItem::AccessibleSelect(long /*flagsSelect*/, VARIANT varChild)
{
	if (GetChildIndex(&varChild) != CHILDID_SELF)
		return E_INVALIDARG;

	Select();

	return S_OK;
}

HRESULT CXTPPropertyGridItem::GetAccessibleValue(VARIANT varChild, BSTR* pszValue)
{
	if (GetChildIndex(&varChild) != CHILDID_SELF)
		return E_INVALIDARG;

	*pszValue = GetValue().AllocSysString();
	return S_OK;
}


/////////////////////////////////////////////////////////////////////////////
// CXTPPropertyGridItems

CXTPPropertyGridItems::CXTPPropertyGridItems()
{

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

void CXTPPropertyGridItems::Clear()
{
	CXTPPropertyGridView* pGrid = GetCount() > 0 ? m_arrItems[0]->m_pGrid : 0;

	if (pGrid)
	{
		if (pGrid->m_nLockUpdate == 0)
			pGrid->ResetContent();
		pGrid->m_nLockUpdate++;
	}

	for (int i = 0; i < GetCount(); i++)
	{
		m_arrItems[i]->Clear();
		m_arrItems[i]->InternalRelease();
	}
	m_arrItems.RemoveAll();

	if (pGrid)
	{
		pGrid->m_nLockUpdate--;
		if (pGrid->m_nLockUpdate == 0)
			pGrid->Refresh();
	}
}

CXTPPropertyGridItem* CXTPPropertyGridItems::GetAt(int nIndex) const
{
	if (nIndex >= 0 && nIndex < GetCount())
	{
		return m_arrItems.GetAt(nIndex);
	}
	return 0;
}

void CXTPPropertyGridItems::RemoveAt (int nIndex)
{
	CXTPPropertyGridItem* pItem = GetAt(nIndex);

	if (pItem)
	{
		m_arrItems.RemoveAt(nIndex);

		if (pItem->GetGrid())
		{
			pItem->GetGrid()->Refresh();
		}

		pItem->InternalRelease();
	}
}

int CXTPPropertyGridItems::Find(CXTPPropertyGridItem* pItem) const
{
	for (int i = 0; i < GetCount(); i++)
	{
		if (m_arrItems[i] == pItem)
			return i;
	}
	return -1;
}


void CXTPPropertyGridItems::AddTail(CXTPPropertyGridItems* pItems)
{
	ASSERT(this != pItems);

	for (int i = 0; i < pItems->GetCount(); i++)
	{
		AddTail(pItems->GetAt(i));
	}
}

int CXTPPropertyGridItems::CompareFunc(const CXTPPropertyGridItem** ppItem1, const CXTPPropertyGridItem** ppItem2)
{
	return (*ppItem1)->GetCaption().Compare((*ppItem2)->GetCaption());
}

void CXTPPropertyGridItems::Sort()
{
	typedef int (_cdecl *GENERICCOMPAREFUNC)(const void *, const void*);

	qsort(m_arrItems.GetData(), m_arrItems.GetSize(), sizeof(CXTPPropertyGridItem*), (GENERICCOMPAREFUNC)CompareFunc);
}

CXTPPropertyGridItem* CXTPPropertyGridItems::FindItem(LPCTSTR strCaption) const
{
	for (int i = 0; i < GetCount(); i++)
	{
		CXTPPropertyGridItem* pItem = GetAt(i);
		if (pItem->GetCaption().Compare(strCaption) == 0)
			return pItem;

		CXTPPropertyGridItem* pChild = pItem->m_pChilds->FindItem(strCaption);
		if (pChild) return pChild;
	}
	return NULL;
}

CXTPPropertyGridItem* CXTPPropertyGridItems::FindItem(UINT nID) const
{
	for (int i = 0; i < GetCount(); i++)
	{
		CXTPPropertyGridItem* pItem = GetAt(i);
		if (pItem->GetID() == nID)
			return pItem;

		CXTPPropertyGridItem* pChild = pItem->m_pChilds->FindItem(nID);
		if (pChild) return pChild;
	}
	return NULL;
}



//////////////////////////////////////////////////////////////////////////
// CXTPPropertyGridItemConstraint
CXTPPropertyGridItemConstraint::CXTPPropertyGridItemConstraint()
{
#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
	m_nIndex = 0;
	m_dwData = 0;
	m_nImage = -1;
	m_pItem = 0;
	m_bEnabled = TRUE;
}

CXTPImageManagerIcon* CXTPPropertyGridItemConstraint::GetImage() const
{
	ASSERT(m_pItem);

	if (!m_pItem)
		return NULL;

	if (m_nImage == -1)
		return NULL;

	return m_pItem->GetGrid()->GetImageManager()->GetImage(m_nImage, 0);
}

/////////////////////////////////////////////////////////////////////////////
// CXTPPropertyGridItemConstraints

// Summary: Constructs a CXTPPropertyGridItemConstraints object
CXTPPropertyGridItemConstraints::CXTPPropertyGridItemConstraints(CXTPPropertyGridItem* pItem)
{
	m_pItem = pItem;
	m_nCurrent = -1;

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPPropertyGridItemConstraints::~CXTPPropertyGridItemConstraints()
{
	RemoveAll();
}

CXTPPropertyGridItemConstraint* CXTPPropertyGridItemConstraints::AddConstraint(LPCTSTR str, DWORD_PTR dwData, int nImage)
{
	CXTPPropertyGridItemConstraint* pConstaint = new CXTPPropertyGridItemConstraint();

	pConstaint->m_strConstraint = str;
	pConstaint->m_dwData = dwData;
	pConstaint->m_nIndex = (int)m_arrConstraints.Add(pConstaint);
	pConstaint->m_pItem = m_pItem;
	pConstaint->m_nImage = nImage;

	SAFE_CALLPTR(m_pItem, OnConstraintsChanged());

	return pConstaint;
}

CString CXTPPropertyGridItemConstraints::GetAt(int nIndex) const
{
	return nIndex >= 0 && nIndex < GetCount() ? m_arrConstraints.GetAt(nIndex)->m_strConstraint : _T("");
}

void CXTPPropertyGridItemConstraints::RemoveAll()
{
	for (int i = 0; i < GetCount(); i++)
		m_arrConstraints[i]->InternalRelease();
	m_arrConstraints.RemoveAll();

	SAFE_CALLPTR(m_pItem, OnConstraintsChanged());
}

void CXTPPropertyGridItemConstraints::RemoveAt(int nIndex)
{
	m_arrConstraints[nIndex]->InternalRelease();
	m_arrConstraints.RemoveAt(nIndex);

	for (int i = nIndex; i < GetCount(); i++)
		m_arrConstraints[i]->m_nIndex = i;

	SAFE_CALLPTR(m_pItem, OnConstraintsChanged());
}

int CXTPPropertyGridItemConstraints::FindConstraint(LPCTSTR str) const
{
	for (int i = 0; i < GetCount(); i++)
	{
		if (GetAt(i).CompareNoCase(str) == 0)
			return i;
	}
	return -1;
}

int CXTPPropertyGridItemConstraints::FindConstraint(DWORD_PTR dwData) const
{
	for (int i = 0; i < GetCount(); i++)
	{
		if (dwData == GetConstraintAt(i)->m_dwData)
			return i;
	}
	return -1;
}

CXTPPropertyGridItemConstraint* CXTPPropertyGridItemConstraints::GetConstraintAt(int nIndex) const
{
	return m_arrConstraints[nIndex];
}

int CXTPPropertyGridItemConstraints::CompareFunc(const CXTPPropertyGridItemConstraint** ppConstraint1, const CXTPPropertyGridItemConstraint** ppConstraint2)
{
	return (*ppConstraint1)->m_strConstraint.Compare((*ppConstraint2)->m_strConstraint);

}

void CXTPPropertyGridItemConstraints::Sort()
{
	typedef int (_cdecl *GENERICCOMPAREFUNC)(const void *, const void*);

	qsort(m_arrConstraints.GetData(), (size_t)m_arrConstraints.GetSize(), sizeof(CXTPPropertyGridItemConstraint*), (GENERICCOMPAREFUNC)CompareFunc);
}


//////////////////////////////////////////////////////////////////////////
// CXTPPropertyGridItemCategory

CXTPPropertyGridItemCategory::CXTPPropertyGridItemCategory(LPCTSTR strCaption)
	:CXTPPropertyGridItem(strCaption)
{
	m_bCategory = TRUE;
	m_nFlags = 0;
}

CXTPPropertyGridItemCategory::CXTPPropertyGridItemCategory(UINT nID)
	:CXTPPropertyGridItem(nID)
{
	m_bCategory = TRUE;
	m_nFlags = 0;
}





///////////////////////////////////////////////////////////////////////////////////
//  Acteve X Methods
//
#ifdef _XTP_ACTIVEX


//////////////////////////////////////////////////////////////////////////
// CXTPPropertyGridItemConstraint


BEGIN_DISPATCH_MAP(CXTPPropertyGridItemConstraint, CCmdTarget)
	DISP_PROPERTY_ID(CXTPPropertyGridItemConstraint, "Caption", DISPID_VALUE, m_strConstraint, VT_BSTR)
	DISP_PROPERTY_ID(CXTPPropertyGridItemConstraint, "Data", 2, m_dwData, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPPropertyGridItemConstraint, "Index", 3, GetIndex, SetNotSupported, VT_I4)
	DISP_PROPERTY_ID(CXTPPropertyGridItemConstraint, "IconIndex", 4, m_nImage, VT_I4)
	DISP_PROPERTY_ID(CXTPPropertyGridItemConstraint, "Enabled", 5, m_bEnabled, VT_I4)
END_DISPATCH_MAP()

// {AA97BA3A-FE51-4e7e-B3C0-A798DDD174E9}
static const GUID IID_IPropertyGridItemConstraint =
{ 0xaa97ba3a, 0xfe51, 0x4e7e, { 0xb3, 0xc0, 0xa7, 0x98, 0xdd, 0xd1, 0x74, 0xe9 } };

BEGIN_INTERFACE_MAP(CXTPPropertyGridItemConstraint, CCmdTarget)
	INTERFACE_PART(CXTPPropertyGridItemConstraint, IID_IPropertyGridItemConstraint, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPPropertyGridItemConstraint, IID_IPropertyGridItemConstraint)


///////////////////////////////////////////////////////////////////////////////
/// CXTPPropertyGridItemConstraints

BEGIN_DISPATCH_MAP(CXTPPropertyGridItemConstraints, CCmdTarget)
	DISP_FUNCTION_ID(CXTPPropertyGridItemConstraints, "Count", dispidCount, OleGetItemCount, VT_I4, VTS_NONE)
	DISP_FUNCTION_ID(CXTPPropertyGridItemConstraints, "Remove", dispidRemove, OleRemove, VT_EMPTY, VTS_I4)
	DISP_FUNCTION_ID(CXTPPropertyGridItemConstraints, "Add", dispidAdd, OleAdd, VT_EMPTY, VTS_BSTR VTS_VARIANT)
	DISP_FUNCTION_ID(CXTPPropertyGridItemConstraints, "Clear", dispidClear, OleClear, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CXTPPropertyGridItemConstraints, "Item", DISPID_VALUE, OleItem, VT_DISPATCH, VTS_I4)
	DISP_FUNCTION_ID(CXTPPropertyGridItemConstraints, "_NewEnum", DISPID_NEWENUM, OleNewEnum, VT_UNKNOWN, VTS_NONE)
END_DISPATCH_MAP()

// {1BA49F02-30A2-44EC-80AB-2ED11DD3D998}
static const IID IID_IPropertyGridItemConstraints =
{ 0x1ba49f02, 0x30a2, 0x44ec, { 0x80, 0xab, 0x2e, 0xd1, 0x1d, 0xd3, 0xd9, 0x98 } };

BEGIN_INTERFACE_MAP(CXTPPropertyGridItemConstraints, CCmdTarget)
	INTERFACE_PART(CXTPPropertyGridItemConstraints, IID_IPropertyGridItemConstraints, Dispatch)
	//INTERFACE_PART(CXTPPropertyGridItemConstraints, IID_IEnumVARIANT, EnumVARIANT)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPPropertyGridItemConstraints, IID_IPropertyGridItemConstraints)


IMPLEMENT_ENUM_VARIANT(CXTPPropertyGridItemConstraints)

long CXTPPropertyGridItemConstraints::OleGetItemCount()
{
	return GetCount();
}

LPDISPATCH CXTPPropertyGridItemConstraints::OleGetItem(long nIndex)
{
	if (nIndex >= 0 && nIndex < GetCount())
	{
		return GetConstraintAt(nIndex)->GetIDispatch(TRUE);
	}
	AfxThrowOleException(E_INVALIDARG);
	return 0;
}

LPDISPATCH CXTPPropertyGridItemConstraints::OleItem(long nIndex)
{
	return OleGetItem(nIndex - 1);
}

void CXTPPropertyGridItemConstraints::OleClear()
{
	RemoveAll();
}

void CXTPPropertyGridItemConstraints::OleAdd(LPCTSTR strItem, const VARIANT* Data)
{
	AddConstraint(strItem, VariantToLong(Data));
}

void CXTPPropertyGridItemConstraints::OleRemove(long nIndex)
{
	if (nIndex > 0 && nIndex <= GetCount())
	{
		RemoveAt(nIndex - 1);
	}
	else
	{
		AfxThrowOleException(E_INVALIDARG);
	}
}



///////////////////////////////////////////////////////////////////////////////
/// CXTPPropertyGridItems

BEGIN_DISPATCH_MAP(CXTPPropertyGridItems, CCmdTarget)
	DISP_FUNCTION_ID(CXTPPropertyGridItems, "Count", dispidCount, OleGetItemCount, VT_I4, VTS_NONE)
	DISP_FUNCTION_ID(CXTPPropertyGridItems, "Remove", dispidRemove, OleRemove, VT_EMPTY, VTS_I4)
	DISP_FUNCTION_ID(CXTPPropertyGridItems, "Clear", dispidClear, OleClear, VT_EMPTY, VTS_NONE)

	DISP_FUNCTION_ID(CXTPPropertyGridItems, "Item", DISPID_VALUE, OleItem, VT_DISPATCH, VTS_I4)
	DISP_FUNCTION_ID(CXTPPropertyGridItems, "_NewEnum", DISPID_NEWENUM, OleNewEnum, VT_UNKNOWN, VTS_NONE)
END_DISPATCH_MAP()

// {ABA49F02-30A2-44EC-80AB-2ED11DD3D998}
static const IID IID_IPropertyGridItems =
{ 0xaba49f02, 0x30a2, 0x44ec, { 0x80, 0xab, 0x2e, 0xd1, 0x1d, 0xd3, 0xd9, 0x98 } };

BEGIN_INTERFACE_MAP(CXTPPropertyGridItems, CCmdTarget)
	INTERFACE_PART(CXTPPropertyGridItems, IID_IPropertyGridItems, Dispatch)
	//INTERFACE_PART(CXTPPropertyGridItems, IID_IEnumVARIANT, EnumVARIANT)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPPropertyGridItems, IID_IPropertyGridItems)

IMPLEMENT_ENUM_VARIANT(CXTPPropertyGridItems)

long CXTPPropertyGridItems::OleGetItemCount()
{
	return GetCount();
}

LPDISPATCH CXTPPropertyGridItems::OleGetItem(long nIndex)
{
	if (nIndex >= 0 && nIndex < GetCount())
	{
		CXTPPropertyGridItem* pItem = GetAt(nIndex);
		return pItem->GetIDispatch(TRUE);
	}
	AfxThrowOleException(E_INVALIDARG);
	return 0;
}

LPDISPATCH CXTPPropertyGridItems::OleItem(long nIndex)
{
	return OleGetItem(nIndex - 1);
}
void CXTPPropertyGridItems::OleClear()
{
	Clear();
}
void CXTPPropertyGridItems::OleRemove(long nIndex)
{
	if (nIndex > 0 && nIndex <= GetCount())
	{
		RemoveAt(nIndex - 1);
	}
	else
	{
		AfxThrowOleException(E_INVALIDARG);
	}
}



///////////////////////////////////////////////////////////////////////////////
/// CXTPPropertyGridItem


BEGIN_DISPATCH_MAP(CXTPPropertyGridItem, CCmdTarget)
	//{{AFX_DISPATCH_MAP(CXTPPropertyGridItem)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_DISPATCH_MAP
	DISP_PROPERTY_EX_ID(CXTPPropertyGridItem, "Expanded", dispidExpanded, OleGetExpanded, OleSetExpanded, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPPropertyGridItem, "ReadOnly", dispidReadOnly, GetReadOnly, SetReadOnly, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPPropertyGridItem, "Selected", dispidSelected, OleGetSelected, OleSetSelected, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPPropertyGridItem, "Caption", dispidCaption, OleGetCaption, OleSetCaption, VT_BSTR)
	DISP_PROPERTY_EX_ID(CXTPPropertyGridItem, "Description", dispidDescription, OleGetDescription, SetDescription, VT_BSTR)
	DISP_PROPERTY_EX_ID(CXTPPropertyGridItem, "MarkupTemplate", dispidMarkupTemplate, OleGetMarkupTemplate, OleSetMarkupTemplate, VT_BSTR)

	DISP_PROPERTY_ID(CXTPPropertyGridItem, "Tag", dispidTag, m_oleTag, VT_VARIANT)
	DISP_PROPERTY_EX_ID(CXTPPropertyGridItem, "Flags", dispidFlags, GetFlags, SetFlags, VT_I4)
	DISP_PROPERTY_ID(CXTPPropertyGridItem, "ConstraintEdit", dispidConstraintEdit, m_bConstraintEdit, VT_BOOL)
	DISP_PROPERTY_ID(CXTPPropertyGridItem, "DefaultValue", 12, m_strDefaultValue, VT_BSTR)

	DISP_PROPERTY_ID(CXTPPropertyGridItem, "DropDownItemCount", 17, m_nDropDownItemCount, VT_I4)

	DISP_PROPERTY_EX_ID(CXTPPropertyGridItem, "Value", DISPID_VALUE, OleGetValue, OleSetValue, VT_VARIANT)

	DISP_FUNCTION_ID(CXTPPropertyGridItem, "Constraints", dispidConstraints, OleGetConstraints, VT_DISPATCH, VTS_NONE)
	DISP_FUNCTION_ID(CXTPPropertyGridItem, "Childs", dispidChilds, OleGetChilds, VT_DISPATCH, VTS_NONE)
	DISP_FUNCTION_ID(CXTPPropertyGridItem, "Parent", dispidParent, OleGetParent, VT_DISPATCH, VTS_NONE)
	DISP_FUNCTION_ID(CXTPPropertyGridItem, "AddChildItem", dispidAddChildItem, OleAddChildItem, VT_DISPATCH, VTS_I4 VTS_BSTR VTS_VARIANT)
	DISP_FUNCTION_ID(CXTPPropertyGridItem, "InsertChildItem", 105, OleInsertChildItem, VT_DISPATCH, VTS_I4 VTS_I4 VTS_BSTR VTS_VARIANT)

	DISP_FUNCTION_ID(CXTPPropertyGridItem, "InsertChildItemBinded", 118, OleInsertChildItemBinded, VT_DISPATCH, VTS_I4 VTS_I4 VTS_DISPATCH VTS_BSTR)
	DISP_FUNCTION_ID(CXTPPropertyGridItem, "AddChildItemBinded", dispidAddChildItemBinded, OleAddChildItemBinded, VT_DISPATCH, VTS_I4 VTS_DISPATCH VTS_BSTR)
	DISP_FUNCTION_ID(CXTPPropertyGridItem, "Type", dispidType, OleGetType, VT_I4, VTS_NONE)
	DISP_PROPERTY_ID(CXTPPropertyGridItem, "Format", dispidFormat, m_strFormat, VT_BSTR)

	DISP_FUNCTION_ID(CXTPPropertyGridItem, "SetMask", 103, OleSetMask, VT_EMPTY, VTS_BSTR VTS_BSTR VTS_VARIANT)
	DISP_PROPERTY_ID(CXTPPropertyGridItem, "PasswordMask", 10, m_bPassword, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPPropertyGridItem, "MaskedText", 104, OleGetMaskedText, SetMaskedText, VT_BSTR)
	DISP_PROPERTY_ID(CXTPPropertyGridItem, "Id", 11, m_nID, VT_I4)
	DISP_FUNCTION_ID(CXTPPropertyGridItem, "Remove", 106, Remove, VT_EMPTY, VTS_NONE)

	DISP_PROPERTY_EX_ID(CXTPPropertyGridItem, "ValueMetrics", 110, OleGetValueMetrics, SetNotSupported, VT_DISPATCH)
	DISP_PROPERTY_EX_ID(CXTPPropertyGridItem, "CaptionMetrics", 111, OleGetCaptionMetrics, SetNotSupported, VT_DISPATCH)
	DISP_PROPERTY_EX_ID(CXTPPropertyGridItem, "Hidden", 13, IsHidden, SetHidden, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPPropertyGridItem, "Height", 14, OleGetHeight, SetHeight, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPPropertyGridItem, "MultiLinesCount", 15, OleGetMultiLinesCount, SetMultiLinesCount, VT_I4)
	DISP_PROPERTY_ID(CXTPPropertyGridItem, "EditStyle", 16, m_dwEditStyle, VT_I4)
	DISP_PROPERTY_ID(CXTPPropertyGridItem, "AutoComplete", 18, m_bAutoComplete, VT_BOOL)
	DISP_PROPERTY_ID(CXTPPropertyGridItem, "Expandable", 19, m_bExpandable, VT_BOOL)
	DISP_PROPERTY_ID(CXTPPropertyGridItem, "Tooltip", 20, m_strTooltip, VT_BSTR)
	DISP_FUNCTION_ID(CXTPPropertyGridItem, "EnsureVisible", 112, EnsureVisible, VT_EMPTY, VTS_NONE)
	DISP_PROPERTY_EX_ID(CXTPPropertyGridItem, "Index", 113, GetIndex, SetNotSupported, VT_I4)
	DISP_FUNCTION_ID(CXTPPropertyGridItem, "AddInplaceButton", 114, OleAddInplaceButton, VT_DISPATCH, VTS_I4)
	DISP_FUNCTION_ID(CXTPPropertyGridItem, "AddSliderControl", 115, OleAddSliderControl, VT_DISPATCH, VTS_NONE)
	DISP_FUNCTION_ID(CXTPPropertyGridItem, "AddSpinButton", 116, OleAddSpinButton, VT_DISPATCH, VTS_NONE)
	DISP_FUNCTION_ID(CXTPPropertyGridItem, "InplaceButtons", 117, OleGetButtons, VT_DISPATCH, VTS_NONE)
	DISP_FUNCTION_ID(CXTPPropertyGridItem, "GetRect", 119, OleGetRect, VT_EMPTY, VTS_PI4 VTS_PI4 VTS_PI4 VTS_PI4)
	DISP_FUNCTION_ID(CXTPPropertyGridItem, "StartEdit", 120, OleStartEdit, VT_EMPTY, VTS_NONE)
END_DISPATCH_MAP()


// Note: we add support for IID_IXTPPropertyGridItem to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the
//  dispinterface in the .ODL file.

// {FBA49F02-30A2-44EC-80AB-2ED11DD3D998}
static const IID IID_IPropertyGridItem =
{ 0xfba49f02, 0x30a2, 0x44ec, { 0x80, 0xab, 0x2e, 0xd1, 0x1d, 0xd3, 0xd9, 0x98 } };


IMPLEMENT_OLETYPELIB_EX(CXTPPropertyGridItem, IID_IPropertyGridItem)

LPDISPATCH CXTPPropertyGridItem::OleAddChildItem(long nType, LPCTSTR strTitle, const VARIANT* varValue)
{
	return OleInsertChildItem(m_pChilds->GetCount(), nType, strTitle, varValue);
}

BSTR CXTPPropertyGridItem::OleGetCaption()
{
	return GetCaption().AllocSysString();
}

void CXTPPropertyGridItem::OleSetCaption(LPCTSTR lpszCaption)
{
	SetCaption(lpszCaption);
}

BSTR CXTPPropertyGridItem::OleGetMarkupTemplate()
{
	return GetMarkupTemplate().AllocSysString();
}

void CXTPPropertyGridItem::OleSetMarkupTemplate(LPCTSTR lpszMarkupTemplate)
{
	SetMarkupTemplate(lpszMarkupTemplate);
}

LPDISPATCH CXTPPropertyGridItem::OleInsertChildItem(int nIndex, long nType, LPCTSTR strTitle, const VARIANT* varValue)
{
	CXTPPropertyGridItem* pItem = 0;

	switch (nType)
	{
	case 1: /*PropertyItemNumber*/
		pItem = InsertChildItem(nIndex, new CXTPPropertyGridItemNumber(strTitle, 0));
		break;


	case 2: /*PropertyItemBool*/
		pItem = InsertChildItem(nIndex, new CXTPPropertyGridItemBool(strTitle, FALSE));
		break;

	case 3: /*PropertyItemColor*/
		pItem = InsertChildItem(nIndex, new CXTPPropertyGridItemColor(strTitle, 0));
		break;

	case 4: /*PropertyItemFont*/
		{
			LOGFONT font;
			ZeroMemory(&font, sizeof(LOGFONT));
			pItem = InsertChildItem(nIndex, new CXTPPropertyGridItemFont(strTitle, font));
		}
		break;
	case 5: /*PropertyItemDouble*/
		pItem = InsertChildItem(nIndex, new CXTPPropertyGridItemDouble(strTitle, 0));
		break;

	case 6: /*PropertyItemDate*/
		{
			COleDateTime dt(2004, 1, 1, 0, 0, 0);
			pItem = InsertChildItem(nIndex, new CXTPPropertyGridItemDate(strTitle, dt));
		}
		break;

	case 7: /*PropertyItemPicture*/
		pItem = InsertChildItem(nIndex, new CXTPPropertyGridItemPicture(strTitle));
		break;

	case 8: /*PropertyItemEnum*/
		pItem = InsertChildItem(nIndex, new CXTPPropertyGridItemEnum(strTitle, 0));
		break;

	case 9: /*PropertyItemFlags*/
		pItem = InsertChildItem(nIndex, new CXTPPropertyGridItemFlags(strTitle, 0));
		break;
	case 10: /*PropertyItemCategory*/
		if (!IsCategory())
			return NULL;
		pItem = InsertChildItem(nIndex, new CXTPPropertyGridItemCategory(strTitle));
		break;

	case 11: /*PropertyItemMultilineString*/
		pItem = InsertChildItem(nIndex, new CXTPPropertyGridItemMultilineString(strTitle, _T("")));
		break;

	case 12: /*PropertyItemOption*/
		pItem = InsertChildItem(nIndex, new CXTPPropertyGridItemOption(strTitle, 0));
		break;

	default: /*PropertyItemString*/
		pItem = InsertChildItem(nIndex, new CXTPPropertyGridItem(strTitle, _T("")));
	}

	ASSERT(pItem);
	if (varValue)
	{
		pItem->OleSetValue(varValue);
	}
	pItem->m_nType = nType;
	pItem->m_nID = AxStringHash(strTitle);
	pItem->m_strDefaultValue = pItem->m_strValue;

	return pItem->GetIDispatch(TRUE);
}

LPDISPATCH CXTPPropertyGridItem::OleAddChildItemBinded(long nType, LPDISPATCH lpHandler, LPCTSTR lpzsProperty)
{
	return OleInsertChildItemBinded(m_pChilds->GetCount(), nType, lpHandler, lpzsProperty);
}

LPDISPATCH CXTPPropertyGridItem::OleInsertChildItemBinded(long nIndex, long nType, LPDISPATCH lpHandler, LPCTSTR lpzsProperty)
{
	if (lpHandler == NULL)
		return 0;

	CString strProperty(lpzsProperty);
	strProperty.Remove(_T(' '));

	OLECHAR szOleProperty[MAX_PATH];
	MBSTOWCS_S(szOleProperty, (LPCTSTR)strProperty, MAX_PATH);

	OLECHAR* szMember = szOleProperty;
	DISPID dispidBinded;

	if (FAILED(lpHandler->GetIDsOfNames(IID_NULL, &szMember, 1, LOCALE_SYSTEM_DEFAULT, &dispidBinded)))
		return 0;

	COleDispatchDriver dispDriverBinded(lpHandler, FALSE);

	CXTPPropertyGridItem* pItem = 0;

	switch (nType)
	{
	case 1: /*PropertyItemNumber*/
		{
			int nValue = 0;
			dispDriverBinded.GetProperty(dispidBinded, VT_I4, &nValue);

			pItem = InsertChildItem(nIndex, new CXTPPropertyGridItemNumber(lpzsProperty, nValue));
		}
		break;
	case 2: /*PropertyItemBool*/
		{
			BOOL bValue;
			dispDriverBinded.GetProperty(dispidBinded, VT_BOOL, &bValue);
			pItem = InsertChildItem(nIndex, new CXTPPropertyGridItemBool(lpzsProperty, bValue));
		}
		break;
	case 3: /*PropertyItemColor*/
		{
			int nValue = 0;
			dispDriverBinded.GetProperty(dispidBinded, VT_I4, &nValue);

			pItem = InsertChildItem(nIndex, new CXTPPropertyGridItemColor(lpzsProperty, nValue));
		}
		break;

	case 5: /*PropertyItemDouble*/
		{
			double fValue = 0;
			dispDriverBinded.GetProperty(dispidBinded, VT_R8, &fValue);

			pItem = InsertChildItem(nIndex, new CXTPPropertyGridItemDouble(lpzsProperty, fValue));
		}
		break;

	case 6: /*PropertyItemDate*/
		{
			COleDateTime dt(2004, 1, 1, 0, 0, 0);
			dispDriverBinded.GetProperty(dispidBinded, VT_DATE, &dt);

			pItem = InsertChildItem(nIndex, new CXTPPropertyGridItemDate(lpzsProperty, dt));
		}
		break;

	case 8: /*PropertyItemEnum*/
		{
			int nValue = 0;
			dispDriverBinded.GetProperty(dispidBinded, VT_I4, &nValue);

			pItem = InsertChildItem(nIndex, new CXTPPropertyGridItemEnum(lpzsProperty, nValue));
		}
		break;
	case 9: /*PropertyItemFlags*/
		{
			int nValue = 0;
			dispDriverBinded.GetProperty(dispidBinded, VT_I4, &nValue);

			pItem = InsertChildItem(nIndex, new CXTPPropertyGridItemFlags(lpzsProperty, nValue));
		}
		break;

	case 11: /*PropertyItemMultilineString*/
		{
			CString strValue;
			dispDriverBinded.GetProperty(dispidBinded, VT_BSTR, &strValue);
			pItem = InsertChildItem(nIndex, new CXTPPropertyGridItemMultilineString(lpzsProperty, strValue));
		}
		break;

	case 12: /*PropertyItemOption*/
		{
			int nValue = 0;
			dispDriverBinded.GetProperty(dispidBinded, VT_I4, &nValue);

			pItem = InsertChildItem(nIndex, new CXTPPropertyGridItemOption(lpzsProperty, nValue));
		}
		break;

	case 0: /*PropertyItemString*/
		{
			CString strValue;
			dispDriverBinded.GetProperty(dispidBinded, VT_BSTR, &strValue);
			pItem = InsertChildItem(nIndex, new CXTPPropertyGridItem(lpzsProperty, strValue));
		}
	}

	if (pItem)
	{
		pItem->m_dispidBinded = dispidBinded;
		pItem->m_dispDriverBinded.AttachDispatch(lpHandler, FALSE);

		pItem->m_nType = nType;
		pItem->m_nID = AxStringHash(strProperty);
		pItem->m_strDefaultValue = pItem->m_strValue;

		return pItem->GetIDispatch(TRUE);
	}
	return 0;
}

LPDISPATCH CXTPPropertyGridItem::OleGetParent()
{
	if (m_pParent) return m_pParent->GetIDispatch(TRUE);

	//AfxThrowOleException(E_FAIL);
	return 0;
}
LPDISPATCH CXTPPropertyGridItem::OleGetChilds()
{
	return m_pChilds->GetIDispatch(TRUE);
}
LPDISPATCH CXTPPropertyGridItem::OleGetConstraints()
{
	return m_pConstraints->GetIDispatch(TRUE);
}

BOOL CXTPPropertyGridItem::OleGetExpanded()
{
	return m_bExpanded;
}

void CXTPPropertyGridItem::OleSetExpanded(BOOL bExpanded)
{
	if (bExpanded)
		Expand();
	else
		Collapse();
}

BSTR CXTPPropertyGridItem::OleGetDescription()
{
	return m_strDescription.AllocSysString();
}

BOOL CXTPPropertyGridItem::OleGetSelected()
{
	if (!m_pGrid) return FALSE;
	return m_pGrid->GetSelectedItem() == this;
}

void CXTPPropertyGridItem::OleSetSelected(BOOL bSelected)
{
	if (bSelected) Select();
}

const VARIANT CXTPPropertyGridItem::OleGetValue()
{
	VARIANT var;
	var.vt = VT_BSTR;
	var.bstrVal = m_strValue.AllocSysString();

	return var;
}

LPDISPATCH CXTPPropertyGridItem::OleAddInplaceButton(UINT Id)
{
	return XTPGetDispatch(m_pInplaceButtons->AddButton(new CXTPPropertyGridInplaceButton(Id)));
}

LPDISPATCH CXTPPropertyGridItem::OleAddSliderControl()
{
	return XTPGetDispatch(AddSliderControl());
}

LPDISPATCH CXTPPropertyGridItem::OleAddSpinButton()
{
	return XTPGetDispatch(AddSpinButton());
}

LPDISPATCH CXTPPropertyGridItem::OleGetButtons()
{
	return XTPGetDispatch(m_pInplaceButtons);
}

int CXTPPropertyGridItem::OleGetHeight()
{
	return m_nHeight;
}

int CXTPPropertyGridItem::OleGetMultiLinesCount()
{
	return m_nLinesCount;
}

BSTR CXTPPropertyGridItem::OleGetMaskedText()
{
	return GetMaskedText().AllocSysString();
}

void CXTPPropertyGridItem::OleSetValue(const VARIANT* varValue)
{
	if (!varValue)
		return;

	if (varValue->vt == (VT_VARIANT | VT_BYREF))
	{
		OleSetValue(varValue->pvarVal);
		return;
	}

	if (varValue->vt == VT_BSTR)
	{
		SetValue(CString(V_BSTR(varValue)));
		return;
	}

	VARIANT var;
	::VariantInit(&var);
	if (SUCCEEDED(VariantChangeType(&var, (VARIANT*)varValue, 0, VT_BSTR)))
	{
		SetValue(CString(V_BSTR(&var)));
	}
	::VariantClear(&var);
}

long CXTPPropertyGridItem::OleGetType()
{
	return m_nType;
}

void CXTPPropertyGridItem::BindDispatch()
{
	if (m_dispDriverBinded.m_lpDispatch)
	{
		m_dispDriverBinded.SetProperty(m_dispidBinded, VT_BSTR, m_strValue);
	}
}

void CXTPPropertyGridItem::OleSetMask(LPCTSTR strMask, LPCTSTR strLiteral, const VARIANT* pPrompt)
{
	TCHAR chPrompt = _T('_');

	if (pPrompt && pPrompt->vt == VT_BSTR)
	{
		CString str(pPrompt->bstrVal);
		if (str.GetLength() == 1)
			chPrompt = str[0];

	}
	SetMask(strMask, strLiteral, chPrompt);

}

LPDISPATCH CXTPPropertyGridItem::OleGetValueMetrics()
{
	return GetValueMetrics()->GetIDispatch(TRUE);
}

LPDISPATCH CXTPPropertyGridItem::OleGetCaptionMetrics()
{
	return GetCaptionMetrics()->GetIDispatch(TRUE);
}

void CXTPPropertyGridItem::OleGetRect(long* pLeft, long* pTop, long* pRight, long* pBottom)
{
	CRect rc = GetItemRect();

	*pLeft = rc.left;
	*pRight = rc.right;
	*pTop = rc.top;
	*pBottom = rc.bottom;
}

void CXTPPropertyGridItem::OleStartEdit()
{
	Select();

	if (m_nFlags & xtpGridItemHasEdit)
	{
		CXTPPropertyGridInplaceEdit& wndEdit = GetInplaceEdit();
		wndEdit.SetFocus();
	}
}


/////////////////////////////////////////////////////////////////////////////
// CXTPPropertyGridItem message handlers

#endif


BEGIN_INTERFACE_MAP(CXTPPropertyGridItem, CCmdTarget)
	INTERFACE_PART(CXTPPropertyGridItem, IID_IAccessible, ExternalAccessible)
#ifdef _XTP_ACTIVEX
	INTERFACE_PART(CXTPPropertyGridItem, IID_IPropertyGridItem, Dispatch)
#endif
END_INTERFACE_MAP()
