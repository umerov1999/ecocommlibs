// XTPControlComboBox.cpp : implementation of the CXTPControlComboBox class.
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


// CXTPControlComboBox

IMPLEMENT_XTP_CONTROL(CXTPControlComboBox, CXTPControlPopup)

CXTPControlComboBox::CXTPControlComboBox(CXTPCommandBars* pCommandBars)
{
	EnableAutomation();

	m_controlType = xtpControlComboBox;
	m_bDropDown = FALSE;
	m_comboStyle = xtpComboNormal;
	m_nWidth = 100;
	m_nLabelWidth = 0;
	m_nThumbWidth = 0;
	m_pEdit = NULL;
	m_strEditText = _T("");
	m_nCurSel = m_nLastSel = -1;
	m_bDelayReposition = FALSE;
	m_bDelayDestroy = FALSE;
	m_bIgnoreAutoComplete = m_bAutoComplete = FALSE;
	m_bFocused = FALSE;
	m_nDropDownItemCount = 12;
	m_bIgnoreSelection = FALSE;
	m_nTextLimit = 0;

	m_pCommandBar = new CXTPControlComboBoxList();
	m_pCommandBar->SetCommandBars(pCommandBars);

	((CXTPControlComboBoxList*)m_pCommandBar)->CreateListBox();

	m_dwShellAutoCompleteFlags = 0;

	m_pAutoCompleteWnd = NULL;
	m_nEditIconId = 0;
	m_bSelEndOk = FALSE;
	m_bEditChanged = TRUE;

	m_dwEditStyle = 0;
}

CXTPControlComboBox::~CXTPControlComboBox()
{
	if (m_pEdit)
	{
		delete m_pEdit;
	}
	SAFE_DELETE(m_pAutoCompleteWnd);
}

CString CXTPControlComboBox::GetEditText() const
{
	CString strEditText = _GetEditText();

	return strEditText == GetEditHint() ? _T("") : strEditText;
}

CString CXTPControlComboBox::_GetEditText() const
{
	if (m_pEdit && m_pEdit->GetSafeHwnd() && m_bEditChanged)
	{
		m_pEdit->GetWindowTextEx(m_strEditText);
		m_bEditChanged = FALSE;
	}

	return m_strEditText;
}

void CXTPControlComboBox::SetEditText(const CString& strText)
{
	_SetEditText(strText.IsEmpty() ? GetEditHint() : strText);
}

void CXTPControlComboBox::_SetEditText(const CString& strText)
{
	if (m_pEdit && m_pEdit->GetSafeHwnd() && _GetEditText() != strText)
	{
		m_pEdit->SetWindowTextEx(strText);
	}
	m_strEditText = strText;
	m_bEditChanged = FALSE;
}

void CXTPControlComboBox::SetEditHint(LPCTSTR lpszEditHint)
{
	if (!GetEditHint().IsEmpty() && _GetEditText() == GetEditHint())
	{
		_SetEditText(_T(""));
	}

	m_strEditHint = lpszEditHint;

	if (_GetEditText().IsEmpty() && !GetEditHint().IsEmpty())
	{
		_SetEditText(GetEditHint());
	}
}

void CXTPControlComboBox::SetTextLimit(int nTextLimit)
{
	m_nTextLimit = nTextLimit;
	if (m_pEdit && m_pEdit->GetSafeHwnd())
		m_pEdit->LimitText(m_nTextLimit);
}

int CXTPControlComboBox::GetTextLimit() const
{
	return m_nTextLimit;
}

BOOL CXTPControlComboBox::IsImageVisible() const
{
	if (GetParent()->GetType() == xtpBarTypePopup)
		return GetImage(0) != NULL;

	BOOL bImageVisible = (CXTPControl::GetStyle() == xtpButtonIcon || CXTPControl::GetStyle() == xtpButtonIconAndCaption)
		&& (GetImage(0) != NULL);

	return bImageVisible;
}

BOOL CXTPControlComboBox::IsCaptionVisible() const
{
	if (GetParent()->GetType() == xtpBarTypePopup)
		return TRUE;

	if (GetCaption().IsEmpty())
		return FALSE;

	XTPButtonStyle buttonStyle = CXTPControl::GetStyle();

	if ((buttonStyle == xtpButtonCaption) || (buttonStyle == xtpButtonIconAndCaption) || (buttonStyle == xtpButtonIconAndCaptionBelow))
		return TRUE;

	return m_comboStyle == xtpComboLabel;
}

CString CXTPControlComboBox::GetEditHint() const
{
	return !m_strEditHint.IsEmpty() ? m_strEditHint : m_pAction ? m_pAction->GetEditHint() : _T("");
}

void CXTPControlComboBox::UpdatePopupSelection()
{
	CEdit* pEdit = GetEditCtrl();
	CXTPControlComboBoxPopupBar* pPopupBar = GetComboBoxPopupBar();

	if (!m_bIgnoreSelection && pEdit && pEdit->GetSafeHwnd() && pPopupBar)
	{
		CString strWindowText = _GetEditText();

		int nIndex = pPopupBar->FindStringExact(0, strWindowText);
		if (nIndex != LB_ERR)
			pPopupBar->SetCurSel(nIndex);
		else
		{
			nIndex = pPopupBar->FindString(0, strWindowText);
			pPopupBar->SetTopIndex(nIndex);
			pPopupBar->SetCurSel(nIndex);
			pPopupBar->SetCurSel(-1);
		}
	}
}

void CXTPControlComboBox::OnEditChanged()
{
	CEdit* pEdit = GetEditCtrl();

	CXTPControlComboBoxPopupBar* pPopupBar = GetComboBoxPopupBar();

	if (!m_bIgnoreSelection && m_bAutoComplete && !m_bIgnoreAutoComplete && pEdit && pEdit->GetSafeHwnd() && pPopupBar)
	{
		CString strWindowText = _GetEditText();
		int nLength = strWindowText.GetLength();

		if (nLength > 0)
		{
			// Currently selected range
			int nStart = 0, nEnd = 0;
			pEdit->GetSel(nStart, nEnd);

			if (nStart == nEnd && nLength == nEnd)
			{
				int nIndex = pPopupBar->FindString(0, strWindowText);
				if (nIndex != LB_ERR)
				{
					CString strWindowTextNew;
					pPopupBar->GetText(nIndex, strWindowTextNew);

					_SetEditText(strWindowTextNew);
					pEdit->SetSel(strWindowTextNew.GetLength(), nLength);
				}

			}
		}
		m_bIgnoreAutoComplete = TRUE;
	}

	UpdatePopupSelection();

	NotifySite(CBN_EDITCHANGE);
}



void CXTPControlComboBox::OnSelChanged()
{
	m_bIgnoreSelection = TRUE;
	_SetEditText(GetListBoxText());
	m_bIgnoreSelection = FALSE;

	RedrawParent(FALSE);

	NotifySite(CBN_SELCHANGE);
}

BOOL CXTPControlComboBox::OnHookMouseWheel(UINT /*nFlags*/, short zDelta, CPoint /*pt*/)
{
	if (!IsFocused())
		return FALSE;

	if (GetDroppedState())
		return FALSE;

	if (m_pAutoCompleteWnd)
		return FALSE;

	if (!IsValidList())
		return FALSE;

	((CXTPControlComboBoxPopupBar*)m_pCommandBar)->ProcessHookKeyDown(this, zDelta > 0 ? VK_UP : VK_DOWN, 0);
	return TRUE;
}

BOOL CXTPControlComboBox::OnHookKeyDown(UINT nChar, LPARAM lParam)
{
	ASSERT(IsFocused());

	if (!IsFocused())
		return FALSE;

	m_bIgnoreAutoComplete = (nChar == VK_DELETE) || (nChar == VK_BACK);

	if (m_pEdit && m_pEdit->m_bImeMode)
		return FALSE_EXIT;

	if (nChar == VK_TAB && ((m_pAutoCompleteWnd == NULL) || ((m_dwShellAutoCompleteFlags & SHACF_USETAB) == 0)))
		return TRUE; // Office 2013 Word doesn't move focus when pressing Tab in the combobox

	if (nChar == VK_RETURN)
		return FALSE;

	if (nChar == VK_ESCAPE)
	{
		m_pCommandBar->OnTrackLost();

		SetCurSel(m_nLastSel);
		SetEditText(m_strLastText);

		SetFocused(FALSE);

		return GetParent()->GetType() == xtpBarTypePopup;
	}

	if (m_pEdit && m_pEdit->GetSafeHwnd() && GetKeyState(VK_MENU) < 0 && GetKeyState(VK_CONTROL) >= 0)
	{
		CXTPCommandBars* pCommandBars = m_pParent->GetCommandBars();
		if (pCommandBars && pCommandBars->OnFrameAccel((TCHAR)nChar))
			return TRUE;
	}

	if ((nChar == VK_F4 && GetKeyState(VK_MENU) >= 0) || ((nChar == VK_UP || nChar == VK_DOWN) && GetKeyState(VK_MENU) < 0))
	{
		OnClick(FALSE);
		return TRUE;
	}

	if (m_pAutoCompleteWnd && m_pAutoCompleteWnd->IsDialogCode(nChar, lParam))
		return TRUE;

	if ((!m_pEdit || (nChar == VK_UP || nChar == VK_DOWN || nChar == VK_NEXT || nChar == VK_PRIOR) && (GetKeyState(VK_MENU) >= 0)) && IsValidList())
	{
		((CXTPControlComboBoxPopupBar*)m_pCommandBar)->ProcessHookKeyDown(this, nChar, lParam);
		return TRUE;
	}

	if (m_pEdit && m_pEdit->GetSafeHwnd() && m_pEdit->IsDialogCode(nChar, lParam))
	{
		return TRUE;
	}

	return FALSE_EXIT;
}

BOOL CXTPControlComboBox::GetDropDownListStyle() const
{
	return m_pEdit != NULL;
}

CXTPControlComboBoxEditCtrl* CXTPControlComboBox::CreateEditControl()
{
	return new CXTPControlComboBoxEditCtrl();
}

void CXTPControlComboBox::SetDropDownListStyle(BOOL bSet)
{
	m_bDropDown = bSet;

	if (bSet && m_pEdit == NULL)
	{
		m_pEdit = CreateEditControl();
		m_pEdit->m_pControl = this;
	}
	else if (!bSet && m_pEdit != NULL)
	{
		delete m_pEdit;
		m_pEdit = NULL;
	}
}

void CXTPControlComboBox::EnableShellAutoComplete(DWORD dwFlags /*= SHACF_FILESYSTEM | SHACF_URLALL*/)
{
	m_dwShellAutoCompleteFlags = dwFlags;
	SAFE_DELETE(m_pAutoCompleteWnd);

	if (m_pEdit->GetSafeHwnd() && dwFlags)
	{
		m_pAutoCompleteWnd = new CXTPControlComboBoxAutoCompleteWnd();

		if (FAILED(m_pAutoCompleteWnd->ShellAutoComplete(m_pEdit->GetSafeHwnd(), m_dwShellAutoCompleteFlags)))
		{
			SAFE_DELETE(m_pAutoCompleteWnd);
		}
	}
}

void CXTPControlComboBox::EnableAutoComplete(BOOL bAutoComplete /*= TRUE*/)
{
	m_bAutoComplete = bAutoComplete;
}


void CXTPControlComboBox::SetRect(CRect rcControl)
{
	CString strEditText = GetEditText();

	if (m_bDelayDestroy && m_pEdit && m_pEdit->GetSafeHwnd())
	{
		m_pEdit->DestroyWindow();
		m_bDelayDestroy = FALSE;
	}
	if (m_pEdit && m_pEdit->GetSafeHwnd() && m_pEdit->GetExStyle() & WS_EX_LAYOUTRTL)
	{
		m_pEdit->DestroyWindow();
	}

	if (m_pEdit && m_pEdit->GetSafeHwnd() && m_pEdit->GetParent() != m_pParent)
	{
		m_pEdit->DestroyWindow();
	}

	if (m_rcControl == rcControl && !(m_pEdit && !m_pEdit->GetSafeHwnd()) && (m_bDelayReposition == FALSE))
	{
		if (!(m_pEdit && m_pEdit->GetSafeHwnd() && m_pEdit->GetParent() != m_pParent))
			return;
	}

	m_rcControl = rcControl;

	if (m_pEdit)
	{
		if (!m_pEdit->GetSafeHwnd())
		{
			m_pEdit->CreateEdit(WS_CHILD | ES_AUTOHSCROLL | m_dwEditStyle, m_pParent);

			if (!GetEnabled()) m_pEdit->EnableWindow(FALSE);

			if (m_dwShellAutoCompleteFlags != 0) EnableShellAutoComplete(m_dwShellAutoCompleteFlags);

			if (m_nTextLimit) m_pEdit->LimitText(m_nTextLimit);
		}

		m_pEdit->SetFont(GetPaintManager()->GetIconFont(), FALSE);
		m_pEdit->SetMargins(0, 0);
		m_pEdit->UpdateCharFormat();
		if (m_pEdit->GetSafeHwnd() != ::GetFocus()) SetEditText(strEditText);
		m_pEdit->m_bComposited = HasDwmCompositedRect();

		rcControl.DeflateRect(m_nLabelWidth + 1 + 3, 3, m_nThumbWidth + 3, 3);

		if (GetPaintManager()->GetEditHeight() > 0)
			rcControl.OffsetRect(0, 1);

		DeflateEditRect(rcControl);
		m_pEdit->MoveWindow(rcControl);
		ShowHideEditControl();
	}
	m_bDelayReposition = FALSE;
}

void CXTPControlComboBox::ShowHideEditControl()
{
	if (m_pEdit && m_pEdit->GetSafeHwnd())
	{
		m_pEdit->SetWindowPos(0, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE
			| (m_pControls && m_pParent && IsVisible() && m_pParent->GetSafeHwnd() ? SWP_SHOWWINDOW : SWP_HIDEWINDOW));
	}
}

void CXTPControlComboBox::OnRemoved()
{
	ShowHideEditControl();

	if (!m_pControls && m_pEdit && m_pEdit->GetSafeHwnd())
	{
		m_pEdit->DestroyWindow();
	}

	CXTPControlPopup::OnRemoved();
}

void CXTPControlComboBox::OnEnabledChanged()
{
	if (m_pEdit && m_pEdit->GetSafeHwnd())
	{
		m_pEdit->EnableWindow(GetEnabled());
		m_pEdit->UpdateCharFormat();
	}
}

void CXTPControlComboBox::SetParent(CXTPCommandBar* pParent)
{
	if (pParent != m_pParent && pParent)
	{
		CWnd* pSite = pParent->GetSite();
		if (pSite)
			SetWindowLongPtr(m_pCommandBar->GetSafeHwnd(), GWLP_HWNDPARENT, (LONG_PTR)pSite->GetSafeHwnd());
	}
	if (!pParent && m_pEdit && m_pEdit->GetSafeHwnd())
	{
		m_pEdit->DestroyWindow();
	}
	CXTPControlPopup::SetParent(pParent);
}

BOOL  CXTPControlComboBox::OnSetSelected(int bSelected)
{
	if (!CXTPControl::OnSetSelected(bSelected))
		return FALSE;

	if (!bSelected)
	{
		SetFocused(FALSE);
	}

	if (!bSelected && m_bPopup && GetParent()->GetPosition() != xtpBarPopup)
	{
		m_pParent->SetPopuped(-1);
	}

	if (m_pEdit->GetSafeHwnd())
	{
		m_pEdit->UpdateCharFormat();
	}

	return TRUE;
}

BOOL CXTPControlComboBox::HasFocus() const
{
	return IsFocused() || (m_pCommandBar->IsTrackingMode()) || m_pParent->GetCommandBars()->GetMouseManager()->IsMouseLocked();
}

BOOL CXTPControlComboBox::IsFocusable() const
{
	return TRUE;
}

void CXTPControlComboBox::OnClick(BOOL bKeyboard , CPoint pt)
{
	if (m_pEdit && m_pEdit->m_bImeMode)
		return;

	if (IsCustomizeMode())
	{
		m_pParent->SetPopuped(-1);
		m_pParent->SetSelected(-1);
		CustomizeStartDrag(pt);
	}
	else if (bKeyboard && GetEnabled())
	{
		if (IsFocused())
		{
			OnExecute();
		}
		else
		{
			SetFocused(TRUE);
		}
	}
	else if (m_bPopup && IsFocused())
	{
		m_pParent->SetPopuped(-1);

	}
	else CXTPControlPopup::OnClick(bKeyboard, pt);
}

void CXTPControlComboBox::OnUnderlineActivate()
{
	m_pParent->HideKeyboardTips();
	SetFocused(TRUE);
}

void CXTPControlComboBox::OnMouseHover()
{
}

void CXTPControlComboBox::SetHideFlags(DWORD dwFlags)
{
	if (m_dwHideFlags != dwFlags)
	{
		m_dwHideFlags = dwFlags;

		ShowHideEditControl();
		DelayLayoutParent();
	}
}

void CXTPControlComboBox::OnActionChanged(int nProperty)
{
	CXTPControl::OnActionChanged(nProperty);

	if (nProperty == 2)    // Visible
	{
		ShowHideEditControl();
	}

	if (nProperty == 4) // EditHint
	{
		if (_GetEditText().IsEmpty() && !GetEditHint().IsEmpty())
		{
			_SetEditText(GetEditHint());
		}
	}
}

void CXTPControlComboBox::OnActionChanging(int nProperty)
{
	if (nProperty == 4 && !GetEditHint().IsEmpty() && _GetEditText() == GetEditHint())
	{
		_SetEditText(_T(""));
	}
}

void CXTPControlComboBox::OnCalcDynamicSize(DWORD dwMode)
{
	if (dwMode & LM_VERTDOCK) SetHideFlags(m_dwHideFlags | xtpHideDockingPosition);
	else SetHideFlags(m_dwHideFlags & ~xtpHideDockingPosition);
}

DWORD CXTPControlComboBox::GetEditStyle() const
{
	return m_dwEditStyle;
}

void CXTPControlComboBox::SetEditStyle(DWORD dwStyle)
{
	m_dwEditStyle = dwStyle;

	if (m_pEdit && m_pEdit->GetSafeHwnd())
	{
		m_bDelayDestroy = TRUE;
		DelayLayoutParent();
	}
}


void CXTPControlComboBox::Copy(CXTPControl* pControl, BOOL bRecursive)
{
	CXTPControlComboBox* pControlCombo = DYNAMIC_DOWNCAST(CXTPControlComboBox, pControl);
	ASSERT(pControlCombo);
	if (!pControlCombo)
	{
		CXTPControlPopup::Copy(pControl, bRecursive);
		return;
	}

	m_dwEditStyle = pControlCombo->m_dwEditStyle;
	m_comboStyle = pControlCombo->m_comboStyle;

	SetEditText(pControlCombo->GetEditText());
	SetEditHint(pControlCombo->m_strEditHint);
	SetDropDownListStyle(pControlCombo->m_pEdit != NULL);
	m_bAutoComplete = pControlCombo->m_bAutoComplete;
	EnableShellAutoComplete(pControlCombo->m_dwShellAutoCompleteFlags);
	m_nEditIconId = pControlCombo->m_nEditIconId;
	m_nDropDownItemCount = pControlCombo->m_nDropDownItemCount;
	m_nTextLimit = pControlCombo->m_nTextLimit;


	CXTPControlPopup::Copy(pControl, bRecursive);
}

BOOL CXTPControlComboBox::Compare(CXTPControl* pOther)
{
	if (!CXTPControlPopup::Compare(pOther))
		return FALSE;

	if (!pOther->IsKindOf(RUNTIME_CLASS(CXTPControlComboBox)))
		return FALSE;

	return TRUE;
}

CString CXTPControlComboBox::GetListBoxText() const
{
	CXTPControlComboBoxPopupBar* pPopupBar = GetComboBoxPopupBar();

	if (!pPopupBar)
		return _T("");

	int nSel = pPopupBar->GetCurSel();
	CString str;
	if (nSel >= 0)
		pPopupBar->GetText(nSel, str);
	return str;
}

BOOL CXTPControlComboBox::IsValidList() const
{
	return GetComboBoxPopupBar() != NULL;
}

CXTPControlComboBoxPopupBar* CXTPControlComboBox::GetComboBoxPopupBar() const
{
	return DYNAMIC_DOWNCAST(CXTPControlComboBoxPopupBar, m_pCommandBar);
}

int CXTPControlComboBox::GetCurSel() const
{
	CXTPControlComboBoxPopupBar* pPopupBar = GetComboBoxPopupBar();
	return pPopupBar ? pPopupBar->GetCurSel() : m_nCurSel;
}

void CXTPControlComboBox::SetCurSel(int nIndex)
{
	if ((GetCurSel() != nIndex) || (m_nCurSel != nIndex))
	{
		m_nCurSel = nIndex;

		CXTPControlComboBoxPopupBar* pPopupBar = GetComboBoxPopupBar();
		if (pPopupBar)
		{
			pPopupBar->SetCurSel(nIndex);
		}

		OnSelChanged();
	}
}

BOOL CXTPControlComboBox::IsFocused() const
{
	return m_bFocused;
}

void CXTPControlComboBox::SetFocused(BOOL bFocused)
{
	if (m_bFocused == bFocused)
		return;

	m_bFocused = bFocused;

	if (bFocused)
	{
		m_nLastSel = GetCurSel();
		m_bSelEndOk = FALSE;

		if (m_pEdit->GetSafeHwnd() && ::GetFocus() != m_pEdit->GetSafeHwnd())
			m_pEdit->SetFocus();

		m_pParent->SetTrackingMode(TRUE, FALSE);
		m_pParent->SetSelected(m_nIndex);

		XTPMouseManager()->LockMouseMove();

		NotifySite(CBN_SETFOCUS);
	}
	else
	{
		if (!m_bSelEndOk && !GetDroppedState() && !m_pEdit)
		{
			SetCurSel(m_nLastSel);
			NotifySite(CBN_SELENDCANCEL);
		}


		if (m_pEdit->GetSafeHwnd() && ::GetFocus() == m_pEdit->GetSafeHwnd())
			::SetFocus(m_pParent->GetTrackFocus());

		XTPMouseManager()->UnlockMouseMove();

		NotifySite(CBN_KILLFOCUS);
	}

	RedrawParent(FALSE);
}


void CXTPControlComboBox::OnSetFocus(CWnd* pOldWnd)
{
	if (!GetEditHint().IsEmpty() && m_pEdit->GetSafeHwnd() && GetEditHint() == _GetEditText())
	{
		_SetEditText(_T(""));
	}

	m_strLastText = GetEditText();


	m_bIgnoreAutoComplete = TRUE;
	UpdatePopupSelection();

	if (m_pParent->GetTrackFocus() == 0)
		m_pParent->SetTrackFocus(pOldWnd ? pOldWnd->GetSafeHwnd() : m_pParent->GetSite()->GetSafeHwnd());

	if (!m_bPopup)
		m_pParent->SetPopuped(-1);

	SetFocused(TRUE);
	if (m_pAutoCompleteWnd) m_pAutoCompleteWnd->SetupMessageHook(TRUE);
}

void CXTPControlComboBox::OnKillFocus()
{
	if (!GetEditHint().IsEmpty() && m_pEdit->GetSafeHwnd() && _GetEditText().IsEmpty())
	{
		_SetEditText(GetEditHint());
	}

	SetFocused(FALSE);

	if (GetDroppedState())
		m_pParent->SetPopuped(-1);

	if (m_pAutoCompleteWnd) m_pAutoCompleteWnd->SetupMessageHook(FALSE);
}

BOOL CXTPControlComboBox::GetDroppedState() const
{
	return m_bPopup;
	//return m_pCommandBar->IsTrackingMode() && ((CXTPPopupBar*)m_pCommandBar)->GetControlPopup() == (CXTPControlPopup*)this;
}

BOOL CXTPControlComboBox::OnSetPopup(BOOL bPopup)
{
	if (bPopup)
	{
		m_bSelEndOk = FALSE;
		m_nLastSel = GetCurSel();
	}
	else if (!IsFocused() && m_pEdit == NULL && !m_bSelEndOk)
	{
		SetCurSel(m_nLastSel);
	}

	if (!bPopup && !m_bSelEndOk)
	{
		NotifySite(CBN_SELENDCANCEL);
	}


	if (m_pAutoCompleteWnd && bPopup) m_pAutoCompleteWnd->CloseWindow();

	m_bPopup = bPopup;
	if (bPopup && GetEditCtrl()) GetEditCtrl()->SetFocus();

	return CXTPControlPopup::OnSetPopup(bPopup);
}

void CXTPControlComboBox::OnExecute()
{
	if (GetCurSel() != -1 && (GetDroppedState() || (!m_pEdit && IsFocused())))
		NotifySite(CBN_SELENDOK);

	m_bSelEndOk = TRUE;

	m_nLastSel = GetCurSel();

	CXTPControlPopup::OnExecute();
}

CHARFORMAT2 CXTPControlComboBox::GetDefaultCharFormat()
{
	CHARFORMAT2 cf;
	ZeroMemory(&cf, sizeof(CHARFORMAT2));
	cf.dwMask = CFM_COLOR | CFM_BACKCOLOR;

	CXTPPaintManager *pPaintManager = GetPaintManager();

	COLORREF clrEditTextNormal   = GetXtremeColor(COLOR_WINDOWTEXT);
	COLORREF clrEditTextDisabled = GetXtremeColor(COLOR_GRAYTEXT);

	if (!pPaintManager->m_clrEditTextNormal.IsDefaultValue())
	{
		clrEditTextNormal = pPaintManager->m_clrEditTextNormal;
	}

	if (!pPaintManager->m_clrEditTextDisabled.IsDefaultValue())
	{
		clrEditTextDisabled = pPaintManager->m_clrEditTextDisabled;
	}

	if (GetEnabled())
	{
		cf.crTextColor = clrEditTextNormal;

		if (!GetEditHint().IsEmpty() && GetEditHint() == _GetEditText())
		{
			cf.crTextColor = clrEditTextDisabled;
		}
	}
	else
	{
		cf.crTextColor = clrEditTextDisabled;
	}

	cf.crBackColor = pPaintManager->GetControlEditBackColor(this);

	return cf;
}

void CXTPControlComboBox::ModifyListBoxStyle(DWORD dwRemove, DWORD dwAdd)
{
	if (IsValidList())
	{
		CXTPControlComboBoxList* pComboBoxList = ((CXTPControlComboBoxList*)m_pCommandBar);

		pComboBoxList->m_dwStyle |= dwAdd;
		pComboBoxList->m_dwStyle &= ~dwRemove;

		pComboBoxList->DestroyWindow();
		pComboBoxList->CreateListBox();
	}
}

void CXTPControlComboBox::OnMouseMove(CPoint point)
{
	CXTPControlPopup::OnMouseMove(point);

	if (GetEnabled() && m_bSelected)
	{
		CRect rcButon(GetRect());
		rcButon.left = rcButon.right - m_nThumbWidth;

		if (rcButon.PtInRect(point) && m_bSelected != TRUE_SPLITDROPDOWN)
		{
			m_bSelected = TRUE_SPLITDROPDOWN;
			RedrawParent(FALSE);
		}
		else if (!rcButon.PtInRect(point) && m_bSelected == TRUE_SPLITDROPDOWN)
		{
			m_bSelected = TRUE;
			RedrawParent(FALSE);
		}
	}
}

void CXTPControlComboBox::SetListIconId(int nId)
{
	if (m_pParent) GetCommandBar()->SetCommandBars(m_pParent->GetCommandBars());

	if (IsValidList())
	{
		CXTPControlComboBoxList* pComboBoxList = ((CXTPControlComboBoxList*)m_pCommandBar);
		pComboBoxList->m_nListIconId = nId;

		ModifyListBoxStyle(0, LBS_OWNERDRAWFIXED | LBS_HASSTRINGS);
	}
}

int CXTPControlComboBox::GetListIconId() const
{
	return IsValidList() ?  ((CXTPControlComboBoxList*)m_pCommandBar)->m_nListIconId : -1;
}

int CXTPControlComboBox::GetDropDownWidth() const
{
	return m_pCommandBar->GetWidth();
}

int CXTPControlComboBox::GetCustomizeMinWidth() const
{
	return m_nLabelWidth + m_nThumbWidth + 5;

}

void CXTPControlComboBox::DeflateEditRect(CRect& rcControl)
{
	if (m_nEditIconId > 0)
	{
		CXTPImageManagerIcon* pIcon = m_pParent->GetImageManager()->GetImage(m_nEditIconId, 0);
		if (pIcon)
			rcControl.left += pIcon->GetWidth() + 2;
	}
}

void CXTPControlComboBox::DrawEditText(CDC* pDC, CRect rcText)
{
	CXTPPaintManager *pPaintManager = GetPaintManager();

	COLORREF clrEditTextNormal      = GetXtremeColor(COLOR_WINDOWTEXT);
	COLORREF clrEditTextDisabled    = GetXtremeColor(COLOR_GRAYTEXT);
	COLORREF clrEditTextHighlighted = GetXtremeColor(COLOR_HIGHLIGHTTEXT);

	if (!pPaintManager->m_clrEditTextNormal.IsDefaultValue())
	{
		clrEditTextNormal = pPaintManager->m_clrEditTextNormal;
	}
	if (!pPaintManager->m_clrEditTextDisabled.IsDefaultValue())
	{
		clrEditTextDisabled = pPaintManager->m_clrEditTextDisabled;
	}
	if (!pPaintManager->m_clrEditTextHighlighted.IsDefaultValue())
	{
		clrEditTextHighlighted = pPaintManager->m_clrEditTextHighlighted;
	}


	if (!GetEditCtrl() && GetEnabled() && IsFocused())
	{
		CRect rcFocus(rcText.left - 1, rcText.top + 1, rcText.right - 1, rcText.bottom - 1);
		pDC->FillSolidRect(rcFocus, GetXtremeColor(COLOR_HIGHLIGHT));
	}

	if (m_nEditIconId > 0)
	{
		CXTPImageManagerIcon* pIcon = m_pParent->GetImageManager()->GetImage(m_nEditIconId, 0);
		if (pIcon)
		{
			pIcon->Draw(pDC, CPoint(rcText.left, rcText.CenterPoint().y - pIcon->GetHeight() / 2), GetEnabled() ? pIcon->GetIcon() : pIcon->GetDisabledIcon());
			rcText.left += pIcon->GetWidth() + 2;
		}
	}

	if (GetEditCtrl())
		return;

	CString strListBoxText = GetListBoxText();

	if (!strListBoxText.IsEmpty())
	{
		pDC->SetTextColor(!GetEnabled() ? clrEditTextDisabled : IsFocused() ? clrEditTextHighlighted : clrEditTextNormal);
		pDC->DrawText(strListBoxText, rcText, DT_SINGLELINE | DT_VCENTER | DT_NOPREFIX);
	}
	else if (!GetEditHint().IsEmpty() && GetEnabled() && !IsFocused())
	{
		pDC->SetTextColor(clrEditTextDisabled);
		pDC->DrawText(GetEditHint(), rcText, DT_SINGLELINE | DT_VCENTER | DT_NOPREFIX);
	}
}

void CXTPControlComboBox::MeasureItem(LPMEASUREITEMSTRUCT /*lpMeasureItemStruct*/)
{

}

void CXTPControlComboBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CString strText;
	GetLBText(lpDrawItemStruct->itemID, strText);

	CRect rc(&lpDrawItemStruct->rcItem);

	ASSERT(lpDrawItemStruct->CtlType == ODT_LISTBOX);

	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);

	BOOL bSelected = lpDrawItemStruct->itemState & ODS_SELECTED;
	CXTPPaintManager* pPaintManager = GetPaintManager();

	COLORREF crOldTextColor = pDC->SetTextColor(pPaintManager->GetRectangleTextColor(bSelected, FALSE, TRUE, FALSE, FALSE, xtpBarTypePopup, xtpBarPopup));

	if (bSelected)
		pPaintManager->DrawRectangle(pDC, rc, TRUE, FALSE, TRUE, FALSE, FALSE, xtpBarTypePopup, xtpBarPopup);
	else
		pDC->FillSolidRect(rc, pPaintManager->GetXtremeColor(COLOR_WINDOW));

	int nListIconId = GetListIconId();
	if (nListIconId != -1)
	{
		CXTPImageManager* pImageManager = GetCommandBar()->GetImageManager();
		CXTPImageManagerIcon* pImage = pImageManager->GetImage(nListIconId + lpDrawItemStruct->itemID, 0);

		if (pImage)
		{
			pImage->Draw(pDC, CPoint(rc.left + 2, (rc.top + rc.bottom - pImage->GetHeight()) / 2),
				pImage->GetIcon(bSelected ? xtpImageHot : xtpImageNormal));
			rc.left += pImage->GetWidth() + 2;
		}
	}

	CXTPFontDC font(pDC, pPaintManager->GetIconFont());

	pDC->SetBkMode(TRANSPARENT);
	rc.left += 3;
	pDC->DrawText(strText, rc, DT_SINGLELINE | DT_VCENTER | DT_NOPREFIX);

	pDC->SetTextColor(crOldTextColor);
}


#ifdef _XTP_ACTIVEX


BEGIN_DISPATCH_MAP(CXTPControlComboBox, CXTPControlPopup)
	DISP_FUNCTION_ID(CXTPControlComboBox, "AddItem", dispidAddItem, OleAddItem, VT_EMPTY, VTS_BSTR VTS_VARIANT)
	DISP_PROPERTY_EX_ID(CXTPControlComboBox, "DropDownWidth", dispidDropDownWidth, GetDropDownWidth, OleSetDropDownWidth, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPControlComboBox, "DropDownListStyle", dispidDropDownListStyle, GetDropDownListStyle, OleSetDropDownListStyle, VT_BOOL)
	DISP_FUNCTION_ID(CXTPControlComboBox, "List", dispidList, OleList, VT_BSTR, VTS_I4)
	DISP_FUNCTION_ID(CXTPControlComboBox, "Clear", dispidClear, OleClear, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CXTPControlComboBox, "ListCount", dispidListCount, OleGetCount, VT_I4, VTS_NONE)
	DISP_FUNCTION_ID(CXTPControlComboBox, "RemoveItem", dispidRemoveItem, OleRemoveItem, VT_EMPTY, VTS_I4)

	DISP_PROPERTY_EX_ID(CXTPControlComboBox, "Text", dispidText, OleGetText, OleSetText, VT_BSTR)
	DISP_PROPERTY_EX_ID(CXTPControlComboBox, "ListIndex", dispidListIndex, OleGetListIndex, OleSetListIndex, VT_I4)
	DISP_FUNCTION_ID(CXTPControlComboBox, "FindItem", dispidFindItem, OleFindItem, VT_I4, VTS_BSTR)
	DISP_PROPERTY_PARAM_ID(CXTPControlComboBox, "ItemData", 62, OleGetItemData, OleSetItemData, VT_I4, VTS_I4)
	DISP_PROPERTY_EX_ID(CXTPControlComboBox, "EditHandle", 63, OleGetEditHandle, SetNotSupported, VT_HANDLE)
	DISP_PROPERTY_EX_ID(CXTPControlComboBox, "HasFocus", 66, HasFocus, SetNotSupported, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPControlComboBox, "EditHint", 67, OleGetEditHint, SetEditHint, VT_BSTR)

	DISP_PROPERTY_ID(CXTPControlComboBox, "AutoComplete", 68, m_bAutoComplete, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPControlComboBox, "ShellAutoComplete", 69, OleGetShellAutoComplete, OleSetShellAutoComplete, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPControlComboBox, "EditIconId", 70, OleGetEditIconId, OleSetEditIconId, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPControlComboBox, "EditStyle", 71, GetEditStyle, SetEditStyle, VT_I4)
	DISP_PROPERTY_ID(CXTPControlComboBox, "DropDownItemCount", 72, m_nDropDownItemCount, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPControlComboBox, "ThemedItems", 73, OleGetThemedItems, OleSetThemedItems, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPControlComboBox, "ListIconId", 74, OleGetListIconId, OleSetListIconId, VT_I4)

	DISP_PROPERTY_EX_ID(CXTPControlComboBox, "TextLimit", 80, GetTextLimit, SetTextLimit, VT_I4)

END_DISPATCH_MAP()

// {F09F389C-1A6F-478c-8C8D-38C067A2BC1F}
static const GUID IID_ICommandBarComboBox =
{ 0xf09f389c, 0x1a6f, 0x478c, { 0x8c, 0x8d, 0x38, 0xc0, 0x67, 0xa2, 0xbc, 0x1f } };

BEGIN_INTERFACE_MAP(CXTPControlComboBox, CXTPControlPopup)
	INTERFACE_PART(CXTPControlComboBox, IID_ICommandBarComboBox, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPControlComboBox, IID_ICommandBarComboBox)

int CXTPControlComboBox::OleGetCount()
{
	return GetCount();
}

void CXTPControlComboBox::OleSetEditIconId(int nId)
{
	SetEditIconId(nId);
}

int CXTPControlComboBox::OleGetEditIconId()
{
	return GetEditIconId();
}

void CXTPControlComboBox::OleSetListIconId(int nId)
{
	SetListIconId(nId);
}

int CXTPControlComboBox::OleGetListIconId()
{
	return GetListIconId();
}

HWND CXTPControlComboBox::OleGetEditHandle()
{
	return m_pEdit ? m_pEdit->GetSafeHwnd() : 0;
}

BOOL CXTPControlComboBox::OleGetThemedItems()
{
	CXTPControlComboBoxList* pComboBoxList = ((CXTPControlComboBoxList*)m_pCommandBar);
	return pComboBoxList->m_dwStyle & LBS_OWNERDRAWFIXED;
}

void CXTPControlComboBox::OleSetThemedItems(BOOL bThemedItems)
{
	if (m_pParent) GetCommandBar()->SetCommandBars(m_pParent->GetCommandBars());

	if (bThemedItems)
		ModifyListBoxStyle(0, LBS_OWNERDRAWFIXED | LBS_HASSTRINGS);
	else
		ModifyListBoxStyle(LBS_OWNERDRAWFIXED | LBS_HASSTRINGS, 0);
}

void CXTPControlComboBox::OleAddItem(LPCTSTR strText, const VARIANT* pIndex)
{
	int nIndex = GetListBoxCtrl()->GetCount();
	if (pIndex && (pIndex->vt == VT_I4 || pIndex->vt == VT_I2 || pIndex->vt == (VT_VARIANT | VT_BYREF)))
		nIndex = min(nIndex, VariantToLong(pIndex) - 1);
	GetListBoxCtrl()->InsertString(nIndex, strText);
}

void CXTPControlComboBox::OleSetDropDownWidth(long nWidth)
{
	m_pCommandBar->SetWidth(nWidth);
}
void CXTPControlComboBox::OleSetDropDownListStyle(BOOL bSet)
{
	SetDropDownListStyle(bSet);
}

BSTR CXTPControlComboBox::OleList(long nIndex)
{
	CString str;
	if (nIndex > 0 && nIndex <= GetCount())
	{
		GetListBoxCtrl()->GetText(nIndex - 1, str);
	}
	return str.AllocSysString();
}

void CXTPControlComboBox::OleRemoveItem(long nIndex)
{
	if (nIndex > 0 && nIndex <= GetCount())
	{
		GetListBoxCtrl()->DeleteString(nIndex - 1);
	}

}

void CXTPControlComboBox::OleClear()
{
	GetListBoxCtrl()->ResetContent();
}


BSTR CXTPControlComboBox::OleGetText()
{
	CString str = GetEditText();
	return str.AllocSysString();
}

void CXTPControlComboBox::OleSetText(LPCTSTR strText)
{
	if (!HasFocus())
		SetEditText(strText);
}


long  CXTPControlComboBox::OleGetListIndex()
{
	return GetCurSel() + 1;
}
void  CXTPControlComboBox::OleSetListIndex(long nIndex)
{
	if (!HasFocus())
	{
		SetCurSel(nIndex - 1);
	}
}

long CXTPControlComboBox::OleFindItem(LPCTSTR strText)
{
	return FindStringExact(0, strText) + 1;
}

long CXTPControlComboBox::OleGetItemData(int nIndex)
{
	if (nIndex > 0 && nIndex <= GetCount())
	{
		return (long)GetItemData(nIndex - 1);
	}
	return 0;
}
void CXTPControlComboBox::OleSetItemData(int nIndex, long nNumber)
{
	if (nIndex > 0 && nIndex <= GetCount())
	{
		SetItemData(nIndex - 1, (long)nNumber);
	}
	else
	{
		AfxThrowOleException(E_INVALIDARG);
	}
}

BSTR CXTPControlComboBox::OleGetEditHint()
{
	return GetEditHint().AllocSysString();
}

DWORD CXTPControlComboBox::OleGetShellAutoComplete()
{
	return m_dwShellAutoCompleteFlags;
}

void CXTPControlComboBox::OleSetShellAutoComplete(DWORD dwShellAutoCompleteFlags)
{
	EnableShellAutoComplete(dwShellAutoCompleteFlags);
}



#endif
