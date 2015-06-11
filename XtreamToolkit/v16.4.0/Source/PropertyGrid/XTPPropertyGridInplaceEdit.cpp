// PropertyGripInplaceEdit.cpp : implementation of the CPropertyGripInplaceEdit class.
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

#include "stdafx.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPMaskEditT.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPColorManager.h"
#include <Common/ScrollBar/XTPScrollBase.h>
#include <Common/ScrollBar/XTPScrollBarCtrl.h>
#include <Common/ScrollBar/XTPScrollBarContainer.h>

#include "XTPPropertyGridDefines.h"
#include "XTPPropertyGridPaintManager.h"
#include "XTPPropertyGridInplaceEdit.h"
#include "XTPPropertyGridInplaceButton.h"
#include "XTPPropertyGridItem.h"
#include "XTPPropertyGridToolTip.h"
#include "XTPPropertyGridView.h"
#include "XTPPropertyGridToolBar.h"
#include "XTPPropertyGrid.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTPPropertyGridInplaceEdit

CXTPPropertyGridInplaceEdit::CXTPPropertyGridInplaceEdit()
: m_pItem(0)
, m_pGrid(0)
{
	m_clrBack = 0;
	m_bDelayCreate = FALSE;
	m_bCreated = FALSE;
	m_bCancel = FALSE;
	m_bEditMode = FALSE;
	m_bCheckAutoComplete = FALSE;
	m_bIgnoreEditChanged = FALSE;
}

CXTPPropertyGridInplaceEdit::~CXTPPropertyGridInplaceEdit()
{
}

IMPLEMENT_DYNAMIC(CXTPPropertyGridInplaceEdit, CEdit)

BEGIN_MESSAGE_MAP(CXTPPropertyGridInplaceEdit, CXTPMaskEditT<CEdit>)
	ON_MASKEDIT_REFLECT()
	//{{AFX_MSG_MAP(CXTPPropertyGridInplaceEdit)
	ON_WM_CTLCOLOR_REFLECT()
	ON_CONTROL_REFLECT(EN_KILLFOCUS, OnEnKillfocus)
	ON_CONTROL_REFLECT(EN_SETFOCUS, OnEnSetfocus)
	ON_CONTROL_REFLECT(EN_CHANGE, OnEnChange)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_KEYDOWN()
	ON_WM_GETDLGCODE()
	ON_WM_CHAR()
	ON_WM_SYSKEYDOWN()
	ON_WM_STYLECHANGED()
	ON_WM_MOUSEWHEEL()
	ON_WM_KEYUP()
	ON_WM_SYSKEYUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTPPropertyGridInplaceEdit message handlers

void CXTPPropertyGridInplaceEdit::SetValue(LPCTSTR strValue)
{
	m_strValue = strValue;
}

void CXTPPropertyGridInplaceEdit::HideWindow()
{
	if (m_hWnd)
	{
		ShowWindow(SW_HIDE);
	}
}

void CXTPPropertyGridInplaceEdit::OnStyleChanged(int nStyleType, LPSTYLESTRUCT lpStyleStruct)
{
	if (nStyleType == GWL_EXSTYLE && lpStyleStruct->styleNew & WS_EX_LAYOUTRTL)
	{
		HideWindow();
		m_bDelayCreate = TRUE;
	}

	CEdit::OnStyleChanged(nStyleType, lpStyleStruct);
}

void CXTPPropertyGridInplaceEdit::Create(CXTPPropertyGridItem* pItem, CRect rect)
{
	ASSERT(pItem && pItem->GetGrid());
	if (!pItem || !pItem->GetGrid())
		return;

	m_pGrid = pItem->GetGrid();
	m_pItem = pItem;
	m_bCreated = FALSE;
	m_bEditMode = FALSE;
	m_bCancel = FALSE;
	m_bCheckAutoComplete = FALSE;

	DWORD dwEditStyle = m_pItem->GetEditStyle();

	if (m_hWnd)
	{
		DWORD dwStyle = (WS_CHILD | WS_VISIBLE | ES_READONLY) | (m_pGrid->GetExStyle() & WS_EX_LAYOUTRTL ? ES_RIGHT : 0);

		if ((GetStyle() | dwStyle) != (dwEditStyle | dwStyle))
			m_bDelayCreate = TRUE;

		if (GetExStyle() & WS_EX_LAYOUTRTL)
			m_bDelayCreate = TRUE;

		if (m_bDelayCreate)
			DestroyWindow();
	}

	if (!m_hWnd)
	{
		CEdit::Create(dwEditStyle, rect, m_pGrid, 0);
	}

	m_bDelayCreate = FALSE;

	if (m_pItem->m_bPassword)
		CEdit::SetPasswordChar(pItem->m_chPrompt);

	if (m_pItem->GetMetrics(TRUE, FALSE))
	{
		CEdit::SetLimitText(m_pItem->GetMetrics(TRUE, FALSE)->m_nMaxLength);
	}
	else
	{
		CEdit::SetLimitText(0);
	}

	SetFont(m_pGrid->GetPaintManager()->GetItemFont(pItem, TRUE));
	SetWindowText(m_strValue);
	SetWindowPos(0, rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOZORDER | SWP_SHOWWINDOW);

	SetMargins(3, 3);

	SetUseMask(m_pItem->m_bUseMask);
	if (m_pItem->m_bUseMask)
	{
		SetPromptChar(m_pItem->m_chPrompt, FALSE);
		SetEditMask(m_pItem->m_strMask, m_pItem->m_strLiteral, m_pItem->GetInPlacedEditValue());
	}

	m_bCreated = TRUE;
}

HBRUSH CXTPPropertyGridInplaceEdit::CtlColor(CDC* pDC, UINT /*nCtlColor*/)
{
	CXTPPropertyGridPaintManager* pPaintManager = m_pGrid ? m_pGrid->GetPaintManager() : 0;

	if (!m_pItem || !pPaintManager)
		return 0;

	pDC->SetTextColor(pPaintManager->GetItemTextColor(m_pItem, TRUE));

	COLORREF clr = pPaintManager->GetItemBackColor(m_pItem, TRUE);

	if (clr != m_clrBack || !m_brBack.GetSafeHandle())
	{
		m_brBack.DeleteObject();
		m_brBack.CreateSolidBrush(clr);
		m_clrBack = clr;

	}

	pDC->SetBkColor(m_clrBack);


	return m_brBack;
}

void CXTPPropertyGridInplaceEdit::OnEnSetfocus()
{
	if (!m_pGrid)
		return;

	m_pGrid->Invalidate(FALSE);

#ifdef _XTP_ACTIVEX
	XTP_SITENOTIFY_ONFOCUS(this, m_pGrid, TRUE)
#endif

	if (m_pItem && !m_pItem->OnRequestEdit())
		m_pGrid->SetFocus();

	m_bEditMode = TRUE;
}

void CXTPPropertyGridInplaceEdit::OnEnKillfocus()
{
	if (!m_pGrid)
		return;

	if (m_pItem)
	{
		m_pItem->OnValidateEdit();
		if (m_pGrid) m_pGrid->Invalidate(FALSE);
	}

#ifdef _XTP_ACTIVEX
	XTP_SITENOTIFY_ONFOCUS(this, m_pGrid, FALSE)
#endif
}

void CXTPPropertyGridInplaceEdit::OnEnChange()
{
	if (!m_bCreated || m_bIgnoreEditChanged)
		return;

	if (m_pItem && m_pGrid && m_pItem->IsAllowEdit() && m_pItem->m_bAutoComplete
		&& !m_pItem->GetConstraintEdit() && m_bCheckAutoComplete)
	{
		m_bCheckAutoComplete = FALSE;

		CXTPPropertyGridItemConstraints* pList = m_pItem->GetConstraints();
		if (!pList->IsEmpty())
		{
			CString strWindowText;
			GetWindowText(strWindowText);
			int nLength = strWindowText.GetLength();


			// Currently selected range
			int nStart = 0, nEnd = 0;
			GetSel(nStart, nEnd);
			if (nStart == nEnd && nLength == nEnd && nLength > 0)
			{
				for (int nIndex = 0; nIndex < pList->GetCount(); nIndex += 1)
				{
					CString strWindowTextNew = pList->GetAt(nIndex);

					if (strWindowTextNew.GetLength() > nLength &&
						strWindowText.CompareNoCase(strWindowTextNew.Left(nLength)) == 0)
					{
						m_bIgnoreEditChanged = TRUE;
						SetWindowText(strWindowTextNew);
						SetSel(strWindowTextNew.GetLength(), nLength);
						m_bIgnoreEditChanged = FALSE;
						break;
					}
				}
			}
		}
	}

	if (m_pItem && m_pGrid)
	{
		ASSERT(m_pItem);
		m_pGrid->SendNotifyMessage(XTP_PGN_EDIT_CHANGED, (LPARAM)this);
	}
}

UINT CXTPPropertyGridInplaceEdit::OnGetDlgCode()
{
	return DLGC_WANTALLKEYS;
}

void CXTPPropertyGridInplaceEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_TAB) return;

	if ((nChar == VK_RETURN) && (GetStyle() & ES_WANTRETURN))
	{
		CXTPMaskEditT<CEdit>::OnChar(nChar, nRepCnt, nFlags);
		return;
	}

	if (nChar == VK_ESCAPE || nChar == VK_RETURN)
	{
		m_pGrid->SetFocus();
		return;
	}

	if (m_pItem && m_pItem->IsAllowEdit() && m_pItem->GetConstraintEdit())
	{
		CXTPPropertyGridItemConstraints* pList = m_pItem->GetConstraints();
		if (!pList->IsEmpty())
		{

			CString strWindowText;
			GetWindowText(strWindowText);
			int nIndex = pList->FindConstraint(strWindowText);
			int nIndexStart = nIndex == -1 ? pList->GetCount() - 1 : nIndex;

			CString strSeach ((TCHAR)nChar);

			do
			{

				nIndex = nIndex < pList->GetCount() - 1 ? nIndex + 1 : 0;
				CString str = pList->GetAt(nIndex);

				if (strSeach.CompareNoCase(str.Left(1)) == 0 && pList->GetConstraintAt(nIndex)->m_bEnabled)
				{
					SetWindowText(str);
					pList->SetCurrent(nIndex);
					SetSel(0, -1);
					return;
				}

			}
			while (nIndex != nIndexStart);

			return;
		}
	}

	m_bCheckAutoComplete = (nChar != VK_DELETE) && (nChar != VK_BACK);

	CXTPMaskEditT<CEdit>::OnChar(nChar, nRepCnt, nFlags);
}

void CXTPPropertyGridInplaceEdit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
#ifdef _XTP_ACTIVEX
	XTP_SITENOTIFY_KEY(this, this, FALSE, nChar)
#endif
	CXTPMaskEditT<CEdit>::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CXTPPropertyGridInplaceEdit::OnSysKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
#ifdef _XTP_ACTIVEX
	XTP_SITENOTIFY_KEY(this, this, FALSE, nChar)
#endif
	CXTPMaskEditT<CEdit>::OnSysKeyUp(nChar, nRepCnt, nFlags);
}

void CXTPPropertyGridInplaceEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
#ifdef _XTP_ACTIVEX
	XTP_SITENOTIFY_KEY(this, this, TRUE, nChar)
#endif

	if (!m_pItem)
		return;

	if (nChar == VK_TAB && m_pGrid)
	{
		((CXTPPropertyGrid*)m_pGrid->GetParent())->OnNavigate(
			xtpGridUIInplaceEdit, GetKeyState(VK_SHIFT) >= 0, m_pItem);
		return;
	}
	else if (nChar == VK_ESCAPE)
	{
		m_bCancel = TRUE;
		m_bCheckAutoComplete = FALSE;
		SetWindowText(m_strValue);
		return ;
	}
	else if (nChar == VK_RETURN)
	{
		return ;
	}
	else if (nChar == VK_F4)
	{
		CXTPPropertyGridInplaceButton* pButton = m_pItem->GetInplaceButtons()->Find(XTP_ID_PROPERTYGRID_COMBOBUTTON);
		if (pButton)
			m_pItem->OnInplaceButtonDown(pButton);
	}
	else if (nChar == VK_DOWN || nChar == VK_UP)
	{
		if (m_pItem != NULL && m_pItem->IsAllowEdit())
		{
			if (SelectConstraint(nChar == VK_DOWN ? +1 : -1, FALSE))
			{
				SetSel(0, -1);
				return ;
			}
		}
	}
	m_bCheckAutoComplete = (nChar != VK_DELETE) && (nChar != VK_BACK);

	CXTPMaskEditT<CEdit>::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CXTPPropertyGridInplaceEdit::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if (m_pGrid && m_pItem != NULL && m_pItem->IsAllowEdit())
	{
		if (SelectConstraint(zDelta > 0 ? -1 : +1, TRUE))
			return TRUE;

		int nCount = m_pItem->GetInplaceControls()->GetCount();
		CXTPPropertyGridInplaceControl* pControl =  nCount > 0 ? m_pItem->GetInplaceControls()->GetAt(nCount - 1) : NULL;

		if (pControl && pControl->OnEditMouseWheel(this, zDelta))
		{
			return TRUE;
		}
	}


	return CXTPMaskEditT<CEdit>::OnMouseWheel(nFlags, zDelta, pt);
}

UINT CXTPPropertyGridInplaceSlider::OnGetDlgCode()
{
	return CWnd::OnGetDlgCode() | DLGC_WANTTAB | DLGC_WANTARROWS;
}

void CXTPPropertyGridInplaceEdit::OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
#ifdef _XTP_ACTIVEX
	XTP_SITENOTIFY_KEY(this, this, TRUE, nChar)
#endif

	if ((nChar == VK_DOWN || nChar == VK_UP) && m_pItem)
	{
		CXTPPropertyGridInplaceButton* pButton = m_pItem->GetInplaceButtons()->Find(XTP_ID_PROPERTYGRID_COMBOBUTTON);
		if (pButton)
			m_pItem->OnInplaceButtonDown(pButton);
	}

	CXTPMaskEditT<CEdit>::OnSysKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CXTPPropertyGridInplaceEdit::SelectConstraint(int nDirection, BOOL bCircle)
{
	CXTPPropertyGridItemConstraints* pList = m_pItem->GetConstraints();
	if (pList->IsEmpty())
		return FALSE;

	CString str;
	GetWindowText(str);
	int nIndex = pList->FindConstraint(str);

	if (nIndex == -1)
		return FALSE;

	nIndex += nDirection;
	if (nIndex >= pList->GetCount()) nIndex = bCircle ? 0 : (ULONG)pList->GetCount() - 1;
	if (nIndex < 0) nIndex = bCircle ? (ULONG)pList->GetCount() - 1 : 0;

	if (!pList->GetConstraintAt(nIndex)->m_bEnabled)
		return FALSE;

	m_bCheckAutoComplete = FALSE;
	SetWindowText(pList->GetAt(nIndex));
	pList->SetCurrent(nIndex);

	return TRUE;
}

void CXTPPropertyGridInplaceEdit::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	if (m_pGrid && m_pItem != NULL && m_pItem->IsAllowEdit() && SelectConstraint(+1, TRUE))
	{
		m_pGrid->SetFocus();
		return;
	}
	CXTPMaskEditT<CEdit>::OnLButtonDblClk(nFlags, point);
}

void CXTPPropertyGridInplaceEdit::DestroyItem()
{
	// reset variables to defaults.
	m_pItem = NULL;
	m_pGrid = NULL;
	m_strValue.Empty();
	m_brBack.DeleteObject();

	// destroy the window.
	//DestroyWindow();
	if (::IsWindow(m_hWnd)) ShowWindow(SW_HIDE);
}


BOOL CXTPPropertyGridInplaceEdit::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && IsDialogMessage(pMsg))
		return TRUE;

	return CXTPMaskEditT<CEdit>::PreTranslateMessage(pMsg);
}


//////////////////////////////////////////////////////////////////////////
// CXTPPropertyGridInplaceControl

IMPLEMENT_DYNAMIC(CXTPPropertyGridInplaceControl, CWnd)

CXTPPropertyGridInplaceControl::CXTPPropertyGridInplaceControl(CXTPPropertyGridItem* pItem)
{
	m_pItem = pItem;
	m_nWidth = 0;
}

void CXTPPropertyGridInplaceControl::OnFinalRelease()
{
	if (m_hWnd != NULL)
		DestroyWindow();

	CCmdTarget::OnFinalRelease();
}

void CXTPPropertyGridInplaceControl::OnDestroyWindow()
{
	DestroyWindow();
}

void CXTPPropertyGridInplaceControl::OnValueChanged()
{

}

BEGIN_MESSAGE_MAP(CXTPPropertyGridInplaceControl, CWnd)
	ON_WM_CHAR()
	ON_WM_KEYDOWN()
	ON_WM_GETDLGCODE()
END_MESSAGE_MAP()


UINT CXTPPropertyGridInplaceControl::OnGetDlgCode()
{
	return CWnd::OnGetDlgCode() | DLGC_WANTTAB;
}

void CXTPPropertyGridInplaceControl::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_TAB) return;

	CWnd::OnChar(nChar, nRepCnt, nFlags);
}

void CXTPPropertyGridInplaceControl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_TAB && m_pItem)
	{
		((CXTPPropertyGrid*)m_pItem->GetGrid()->GetParent())->OnNavigate(
			xtpGridUIInplaceControl, GetKeyState(VK_SHIFT) >= 0, m_pItem);
		return;
	}

	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

//////////////////////////////////////////////////////////////////////////
// CXTPPropertyGridInplaceSlider

IMPLEMENT_DYNAMIC(CXTPPropertyGridInplaceSlider, CXTPPropertyGridInplaceControl)

CXTPPropertyGridInplaceSlider::CXTPPropertyGridInplaceSlider(CXTPPropertyGridItem* pItem)
	: CXTPPropertyGridInplaceControl(pItem)
{
	m_nMin = 0;
	m_nMax = 100;

	m_nValue = 0;

	m_clrBack = COLORREF_NULL;

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif

}

BEGIN_MESSAGE_MAP(CXTPPropertyGridInplaceSlider, CXTPPropertyGridInplaceControl)
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_HSCROLL_REFLECT()
	ON_WM_MOUSEWHEEL()
	ON_WM_GETDLGCODE()
END_MESSAGE_MAP()

int CXTPPropertyGridInplaceSlider::CalcualteEditWidth()
{
	if (m_nWidth != 0)
		return m_nWidth;

	CWindowDC dc(m_pItem->GetGrid());
	CXTPFontDC font (&dc, m_pItem->GetGrid()->GetFont());

	CString strMax;
	strMax.Format(_T("%i"), m_nMax);

	CString strMin;
	strMin.Format(_T("%i"), m_nMin);

	int nWidth = max(dc.GetTextExtent(strMax).cx, dc.GetTextExtent(strMin).cx) + 6;

	return nWidth;
}

void CXTPPropertyGridInplaceSlider::HScroll(UINT nSBCode, UINT nPos)
{
	OnScroll(nSBCode, nPos);
}

BOOL CXTPPropertyGridInplaceSlider::OnMouseWheel(UINT /*nFlags*/, short zDelta, CPoint /*pt*/)
{
	int nValue = GetSliderCtrl()->GetPos();

	nValue = nValue + (zDelta > 0 ? +1 : -1);

	nValue = max(m_nMin, min(m_nMax, nValue));

	GetSliderCtrl()->SetPos(nValue);
	OnScroll(zDelta > 0 ? SB_LEFT : SB_RIGHT, nValue);

	return TRUE;
}

BOOL CXTPPropertyGridInplaceSlider::OnEditMouseWheel(CEdit* pEdit, short zDelta)
{
	CString strValue;
	pEdit->GetWindowText(strValue);

	long nValue = _ttol(strValue) + (zDelta > 0 ? +1 : -1);

	nValue = max(m_nMin, min(m_nMax, nValue));

	CString str;
	str.Format(_T("%i"), nValue);
	m_pItem->OnValueChanged(str);

	return TRUE;
}

void CXTPPropertyGridInplaceSlider::OnScroll(UINT /*nSBCode*/, UINT /*nPos*/)
{
	int nValue = GetSliderCtrl()->GetPos();
	if (nValue != m_nValue)
	{
		m_nValue = nValue;
		CString strValue;
		strValue.Format(_T("%i"), nValue);

		m_pItem->SetValue(strValue);
		m_pItem->OnValueChanged(strValue);

		m_pItem->GetGrid()->Invalidate(FALSE);
	}
}

void CXTPPropertyGridInplaceSlider::OnCreateWindow(CRect& rcValue)
{
	CRect rcSlider(rcValue);

	if (m_nWidth > 0)
	{
		rcSlider.left = rcSlider.right - m_nWidth;

		if (rcSlider.left < rcValue.left)
			return;

		rcValue.right -= m_nWidth;
	}
	else
	{
		int nWidth = CalcualteEditWidth();
		rcSlider.left += nWidth;

		if (rcSlider.right < rcSlider.left)
			return;

		rcValue.right = rcValue.left + nWidth;
	}

	if (!m_hWnd)
	{
		VERIFY(AfxDeferRegisterClass(AFX_WNDCOMMCTL_BAR_REG));

		Create(TRACKBAR_CLASS, NULL, WS_CHILD | TBS_HORZ | WS_TABSTOP, rcSlider, (CWnd*)m_pItem->GetGrid(), 0);
	}

	m_nValue = _ttol(m_pItem->GetValue());
	GetSliderCtrl()->SetRange(m_nMin, m_nMax, TRUE);
	GetSliderCtrl()->SetPos(m_nValue);
	GetSliderCtrl()->EnableWindow(m_pItem->IsAllowEdit());

	MoveWindow(rcSlider);
	ShowWindow(SW_SHOW);
}

HBRUSH CXTPPropertyGridInplaceSlider::CtlColor(CDC* pDC, UINT /*nCtlColor*/)
{
	CXTPPropertyGridView* pGrid = m_pItem ? m_pItem->GetGrid() : 0;
	CXTPPropertyGridPaintManager* pPaintManager = pGrid ? pGrid->GetPaintManager() : 0;

	if (!m_pItem || !pPaintManager)
		return 0;

	pDC->SetTextColor(pPaintManager->GetItemTextColor(m_pItem, TRUE));

	COLORREF clr = pPaintManager->GetItemBackColor(m_pItem, TRUE);

	if (clr != m_clrBack || !m_brBack.GetSafeHandle())
	{
		m_brBack.DeleteObject();
		m_brBack.CreateSolidBrush(clr);
		m_clrBack = clr;
	}

	pDC->SetBkColor(m_clrBack);


	return m_brBack;
}

void CXTPPropertyGridInplaceSlider::OnAdjustValueRect(CRect& rcValue)
{
	if (!m_hWnd)
		return;

	if (m_nWidth > 0)
	{
		rcValue.right -= m_nWidth;
		return;
	}

	int nWidth = CalcualteEditWidth();

	if (rcValue.left + nWidth > rcValue.right)
		return;

	rcValue.right = rcValue.left + nWidth;
}

void CXTPPropertyGridInplaceSlider::OnValueChanged()
{
	int nValue = _ttol(m_pItem->GetValue());
	int nValueRange = max(m_nMin, min(m_nMax, nValue));

	if (nValueRange != m_nValue || nValueRange != nValue)
	{
		m_nValue = nValueRange;

		CString str;
		str.Format(_T("%i"), nValueRange);
		m_pItem->SetValue(str);

		if (m_hWnd) GetSliderCtrl()->SetPos(m_nValue);
	}
}

//////////////////////////////////////////////////////////////////////////
// CXTPPropertyGridInplaceSpinButton

IMPLEMENT_DYNAMIC(CXTPPropertyGridInplaceSpinButton, CXTPPropertyGridInplaceControl)

CXTPPropertyGridInplaceSpinButton::CXTPPropertyGridInplaceSpinButton(CXTPPropertyGridItem* pItem)
	: CXTPPropertyGridInplaceControl(pItem)
{
	m_nMin = 0;
	m_nMax = 100;
	m_nWidth = GetSystemMetrics(SM_CXVSCROLL);

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif

}

BEGIN_MESSAGE_MAP(CXTPPropertyGridInplaceSpinButton, CXTPPropertyGridInplaceControl)
	ON_NOTIFY_REFLECT(UDN_DELTAPOS, OnDeltapos)
	ON_WM_GETDLGCODE()
END_MESSAGE_MAP()

#define AFX_WNDCOMMCTL_UPDOWN_REG 0x00040

void CXTPPropertyGridInplaceSpinButton::OnCreateWindow(CRect& rcValue)
{
	CRect rcButton(rcValue);

	rcButton.left = rcButton.right - m_nWidth;
	rcValue.right -= m_nWidth;

	if (!m_hWnd)
	{
		VERIFY(AfxDeferRegisterClass(AFX_WNDCOMMCTL_UPDOWN_REG));

		Create(UPDOWN_CLASS, NULL, WS_CHILD | UDS_ARROWKEYS, rcButton, (CWnd*)m_pItem->GetGrid(), 0);
	}

	int nValue = _ttol(m_pItem->GetValue());
	GetSpinButtonCtrl()->SetPos(nValue);
	GetSpinButtonCtrl()->SendMessage(UDM_SETRANGE32, (WPARAM)m_nMin, (LPARAM)m_nMax);
	GetSpinButtonCtrl()->EnableWindow(m_pItem->IsAllowEdit());

	MoveWindow(rcButton);
	ShowWindow(SW_SHOW);
}

BOOL CXTPPropertyGridInplaceSpinButton::OnEditMouseWheel(CEdit* pEdit, short zDelta)
{
	CString strValue;
	pEdit->GetWindowText(strValue);

	long nValue = _ttol(strValue) + (zDelta > 0 ? +1 : -1);

	nValue = max(m_nMin, min(m_nMax, nValue));

	CString str;
	str.Format(_T("%i"), nValue);
	m_pItem->OnValueChanged(str);

	return TRUE;
}

void CXTPPropertyGridInplaceSpinButton::OnDeltapos(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	m_pItem->OnValidateEdit();
	long nValue = _ttol(m_pItem->GetValue()) + pNMUpDown->iDelta;

	nValue = max(m_nMin, min(m_nMax, nValue));

	CString str;
	str.Format(_T("%i"), nValue);
	m_pItem->OnValueChanged(str);

	*pResult = 1;
}

UINT CXTPPropertyGridInplaceSpinButton::OnGetDlgCode()
{
	return CWnd::OnGetDlgCode() | DLGC_WANTTAB | DLGC_WANTARROWS;
}

void CXTPPropertyGridInplaceSpinButton::OnAdjustValueRect(CRect& rcValue)
{
	if (m_hWnd)
	{
		rcValue.right -= m_nWidth;
	}
}

void CXTPPropertyGridInplaceSpinButton::OnValueChanged()
{
	int nValue = _ttol(m_pItem->GetValue());
	int nValueRange = max(m_nMin, min(m_nMax, nValue));

	if (nValue != nValueRange)
	{
		CString str;
		str.Format(_T("%i"), nValueRange);

		m_pItem->SetValue(str);
	}
}
//////////////////////////////////////////////////////////////////////////
// CXTPPropertyGridInplaceControls


CXTPPropertyGridInplaceControls::CXTPPropertyGridInplaceControls()
{

}

CXTPPropertyGridInplaceControls::~CXTPPropertyGridInplaceControls()
{
	RemoveAll();
}

void CXTPPropertyGridInplaceControls::RemoveAll()
{
	for (int i = 0; i < (int)m_arrControls.GetSize(); i++)
	{
		m_arrControls[i]->InternalRelease();
	}
	m_arrControls.RemoveAll();
}

int CXTPPropertyGridInplaceControls::GetCount() const
{
	return (int)m_arrControls.GetSize();
}

void CXTPPropertyGridInplaceControls::Add(CXTPPropertyGridInplaceControl* pWindow)
{
	m_arrControls.Add(pWindow);
}

CXTPPropertyGridInplaceControl* CXTPPropertyGridInplaceControls::GetAt(int nIndex)
{
	return m_arrControls[nIndex];
}


#ifdef _XTP_ACTIVEX
///////////////////////////////////////////////////////////////////////////////////
//  Acteve X Methods
//


//////////////////////////////////////////////////////////////////////////
// CXTPPropertyGridInplaceButton


BEGIN_DISPATCH_MAP(CXTPPropertyGridInplaceSlider, CCmdTarget)
	DISP_PROPERTY_ID(CXTPPropertyGridInplaceSlider, "Min", 1, m_nMin, VT_I4)
	DISP_PROPERTY_ID(CXTPPropertyGridInplaceSlider, "Max", 2, m_nMax, VT_I4)
	DISP_PROPERTY_ID(CXTPPropertyGridInplaceSlider, "Width", 4, m_nWidth, VT_I4)
END_DISPATCH_MAP()

// {1347BA3A-FE51-4e7e-B3C0-A798DDD174E9}
static const GUID IID_IPropertyGridInplaceSlider =
{ 0x1347ba3a, 0xfe51, 0x4e7e, { 0xb3, 0xc0, 0xa7, 0x98, 0xdd, 0xd1, 0x74, 0xe9 } };

BEGIN_INTERFACE_MAP(CXTPPropertyGridInplaceSlider, CCmdTarget)
	INTERFACE_PART(CXTPPropertyGridInplaceSlider, IID_IPropertyGridInplaceSlider, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPPropertyGridInplaceSlider, IID_IPropertyGridInplaceSlider)



BEGIN_DISPATCH_MAP(CXTPPropertyGridInplaceSpinButton, CCmdTarget)
	DISP_PROPERTY_ID(CXTPPropertyGridInplaceSpinButton, "Min", 1, m_nMin, VT_I4)
	DISP_PROPERTY_ID(CXTPPropertyGridInplaceSpinButton, "Max", 2, m_nMax, VT_I4)
	DISP_PROPERTY_ID(CXTPPropertyGridInplaceSpinButton, "Width", 4, m_nWidth, VT_I4)
END_DISPATCH_MAP()

// {1357BA3A-FE51-4e7e-B3C0-A798DDD174E9}
static const GUID IID_IPropertyGridInplaceSpinButton =
{ 0x1357ba3a, 0xfe51, 0x4e7e, { 0xb3, 0xc0, 0xa7, 0x98, 0xdd, 0xd1, 0x74, 0xe9 } };

BEGIN_INTERFACE_MAP(CXTPPropertyGridInplaceSpinButton, CCmdTarget)
	INTERFACE_PART(CXTPPropertyGridInplaceSpinButton, IID_IPropertyGridInplaceSpinButton, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPPropertyGridInplaceSpinButton, IID_IPropertyGridInplaceSpinButton)


#endif
