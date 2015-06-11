// XTPControlPopup.cpp : implementation of the CXTPControlPopup class.
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

#include <Common/XTPHookManager.h>
#include <Common/XTPSystemHelpers.h>
#include <Common/XTPColorManager.h>
#include <Common/ScrollBar/XTPScrollInfo.h>

#include <CommandBars/XTPCommandBarsDefines.h>
#include <CommandBars/XTPControl.h>
#include <CommandBars/XTPControlButton.h>
#include <CommandBars/XTPControlPopup.h>
#include <CommandBars/XTPCommandBar.h>
#include <CommandBars/XTPControls.h>
#include <CommandBars/XTPPopupBar.h>
#include <CommandBars/XTPPaintManager.h>
#include <CommandBars/XTPCommandBars.h>
#include <CommandBars/XTPToolBar.h>
#include <CommandBars/XTPMenuBar.h>

#ifdef _XTP_INCLUDE_RIBBON
#include <Ribbon/XTPRibbonPopups.h>
#include <Ribbon/XTPRibbonBar.h>
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_XTP_CONTROL(CXTPControlPopup, CXTPControlButton)

CXTPControlPopup::CXTPControlPopup()
{
	EnableAutomation();

	m_controlType = xtpControlPopup;
	m_bPopup = FALSE;
	m_pCommandBar = NULL;
	m_dwCommandBarID = 0;
	m_bShowShadow = TRUE;
}

CXTPControlPopup::~CXTPControlPopup()
{
	if (m_pCommandBar)
		m_pCommandBar->InternalRelease();

}

CXTPControlPopup* CXTPControlPopup::CreateControlPopup(XTPControlType controlType)
{
	ASSERT(IsPopupControlType(controlType));

	CXTPControlPopup* pControl = (CXTPControlPopup*)CXTPControlPopup::CreateObject();
	pControl->m_controlType = controlType;
	return pControl;
}

CString CXTPControlPopup::GetCaption() const
{
	CString strCaption =  CXTPControlButton::GetCaption();

	CXTPCommandBar *pCommandBar = GetParent();

#ifdef _XTP_INCLUDE_RIBBON
	if (pCommandBar && pCommandBar->IsKindOf(RUNTIME_CLASS(CXTPRibbonBar)))
	{
	}
	else
#endif
	if (pCommandBar && pCommandBar->IsKindOf(RUNTIME_CLASS(CXTPMenuBar)))
	{
		if (!pCommandBar->GetCommandBars() == NULL)
		{
			if (pCommandBar->GetCommandBars()->IsAllCaps())
			{
				strCaption.MakeUpper();
			}
		}
	}

	return strCaption;
}

void CXTPControlPopup::OnUnderlineActivate()
{
	if (m_pCommandBar)
		m_pParent->SetPopuped(m_nIndex, TRUE);
	else
		CXTPControlButton::OnUnderlineActivate();
}

void CXTPControlPopup::OnClick(BOOL bKeyboard, CPoint pt)
{
	if (!GetEnabled())
		return;

	if (IsCustomizeMode())
	{
		ASSERT(m_pParent);
		if (!m_pParent)
			return;
		CXTPCommandBars* pCommandBars = m_pParent->GetCommandBars();
		ASSERT(pCommandBars);
		if (!pCommandBars)
			return;

		pCommandBars->SetDragControl(this);
	}

	if (m_bPopup && (GetParent()->GetPosition() != xtpBarPopup))
		m_pParent->SetTrackingMode(FALSE, FALSE);
	else
	{
		if (!IsCustomizeMode() && (m_controlType == xtpControlSplitButtonPopup))
		{
			if (bKeyboard)
			{
				if (GetEnabled() != TRUE_SPLITDROPDOWN)
					OnExecute();
				else
					m_pParent->SetPopuped(m_nIndex);
				return;
			}

			CRect rcCommandRect = GetPaintManager()->GetSplitButtonCommandRect(this);

			if ((m_pParent->GetType() == xtpBarTypePopup) && rcCommandRect.PtInRect(pt))
			{
				m_pParent->KillTimer(XTP_TID_HOVER);
				return;
			}

			if ((m_pParent->GetType() != xtpBarTypePopup) && rcCommandRect.PtInRect(pt))
			{
				if (GetEnabled() != TRUE_SPLITDROPDOWN)
					ClickToolBarButton(rcCommandRect);
				return;
			}

			if (GetEnabled() == TRUE_SPLITCOMMAND)
				return;
		}


		if (!(IsCustomizeMode() && m_pCommandBar && !m_pCommandBar->IsCustomizable()))
		{
			if (!m_pParent->SetPopuped(m_nIndex))
				return;
		}
	}

	if (IsCustomizeMode())
	{
		CustomizeStartDrag(pt);
	}
}

void CXTPControlPopup::OnLButtonUp(CPoint point)
{
	if (!m_pCommandBar)
	{
		CXTPControlButton::OnLButtonUp(point);
		return;
	}

	if (GetEnabled() && GetType() == xtpControlSplitButtonPopup && m_pParent->GetType() == xtpBarTypePopup)
	{
		if (m_rcControl.right > point.x + GetPaintManager()->m_nSplitButtonPopupWidth && GetEnabled() != TRUE_SPLITDROPDOWN)
			OnExecute();
	}
}

void CXTPControlPopup::OnMouseMove(CPoint point)
{
	if (!m_pCommandBar)
	{
		CXTPControlButton::OnMouseMove(point);
		return;
	}

	if (GetEnabled() && GetType() == xtpControlSplitButtonPopup && m_bSelected)
	{
		CRect rcCommandRect = GetPaintManager()->GetSplitButtonCommandRect(this);

		if (rcCommandRect.PtInRect(point) && m_bSelected != TRUE_SPLITCOMMAND)
		{
			m_bSelected = TRUE_SPLITCOMMAND;
			RedrawParent(FALSE);
		}
		else if (!rcCommandRect.PtInRect(point) && m_bSelected != TRUE_SPLITDROPDOWN)
		{
			m_bSelected = TRUE_SPLITDROPDOWN;
			RedrawParent(FALSE);
		}
	}
}

BOOL CXTPControlPopup::OnSetSelected(int bSelected)
{
	if (!CXTPControlButton::OnSetSelected(bSelected))
		return FALSE;

	if (!m_pCommandBar)
		return TRUE;

	if (bSelected == 1 && m_pParent->GetSafeHwnd() && GetEnabled() && GetType() == xtpControlSplitButtonPopup)
	{
		CPoint point;
		GetCursorPos(&point);
		m_pParent->ScreenToClient(&point);

		if (GetRect().PtInRect(point))
			OnMouseMove(point);
	}

	if (m_controlType == xtpControlPopup || m_pParent->GetType() == xtpBarTypePopup)
	{
		if (GetEnabled() && bSelected && m_pParent->IsTrackingMode() == TRUE_POPUP && GetParent()->GetPosition() != xtpBarPopup)
		{
			m_pParent->SetPopuped(m_nIndex);
		}
		else if (!bSelected && m_bPopup && GetParent()->GetPosition() != xtpBarPopup)
		{
			m_pParent->SetPopuped(-1);
		}
	}
	else
	{
		if (GetEnabled() && IsKeyboardSelected(bSelected) && m_pParent->IsTrackingMode() == TRUE_POPUP)
		{
			m_pParent->SetPopuped(m_nIndex);
		}

	}

	return TRUE;
}

void CXTPControlPopup::UpdateShadow()
{
	if (m_pCommandBar && m_pParent->GetPosition() != xtpBarPopup && IsPopupControlType(m_controlType))
	{
		if (m_pParent->GetPosition() == xtpBarFloating && m_pParent->GetType() == xtpBarTypePopup)
			return;

		CXTPPaintManager* pPaintManager = GetPaintManager();
		CXTPPopupBar* pPopupBar = DYNAMIC_DOWNCAST(CXTPPopupBar, m_pCommandBar);
		if (pPaintManager && pPopupBar && pPopupBar->m_bShowShadow && m_bShowShadow && pPaintManager->m_bShowShadow && !m_pParent->IsCustomizeMode())
		{
			CRect rc = GetRect();
			GetParent()->ClientToScreen(&rc);

			pPaintManager->GetShadowManager()->SetShadow(rc, m_pCommandBar);
		}
	}
}

BOOL CXTPControlPopup::OnSetPopup(BOOL bPopup)
{
	if (!m_pCommandBar)
		return FALSE;

	if (GetType() == xtpControlSplitButtonPopup && GetEnabled() == TRUE_SPLITCOMMAND)
		return FALSE;

	if (m_pParent->GetSafeHwnd() == NULL)
		return FALSE;

	m_bPopup = bPopup;

	if (bPopup)
	{
		RedrawParent(FALSE);
		m_pParent->GetSite()->UpdateWindow();
		m_pParent->UpdateWindow();

		if (!m_pCommandBar->IsTrackingMode())
		{
			if (!m_pCommandBar->Popup(this, IsKeyboardSelected(GetSelected())))
				return FALSE;

			UpdateShadow();
		}
	}
	else
	{
		m_pCommandBar->SetTrackingMode(FALSE);
		RedrawParent();
	}

	return TRUE;
}

void CXTPControlPopup::ExpandCommandBar()
{
	CXTPPopupBar* pPopupBar = DYNAMIC_DOWNCAST(CXTPPopupBar, m_pCommandBar);
	if (m_bPopup && pPopupBar && m_controlType == xtpControlPopup)
	{
		pPopupBar->ExpandBar();
	}
}

void CXTPControlPopup::OnMouseHover()
{
	if (m_pCommandBar && GetEnabled() && (m_pParent->IsTrackingMode() == TRUE_POPUP || GetParent()->GetPosition() == xtpBarPopup))
		GetParent()->SetPopuped(m_nIndex);
	else CXTPControlButton::OnMouseHover();
}

BOOL CXTPControlPopup::OnLButtonDblClk(CPoint point)
{
	CXTPPopupBar* pPopupBar = DYNAMIC_DOWNCAST(CXTPPopupBar, m_pCommandBar);
	if (pPopupBar && (m_controlType == xtpControlPopup || m_controlType == xtpControlButtonPopup))
	{
		if (!m_bPopup)
			m_pParent->OnLButtonDown(0, point);

		if (m_pCommandBar != pPopupBar)
			return FALSE;

		if (m_bPopup && pPopupBar->GetDefaultItem(0, TRUE) != (UINT)-1)
		{
			CXTPControl* pControl = pPopupBar->GetControl(pPopupBar->GetDefaultItem(0, TRUE));
			pControl->OnExecute();
			return TRUE;
		}

		if (m_bPopup && pPopupBar->m_bCollapsed)
		{
			pPopupBar->ExpandBar();
			return TRUE;
		}
	}
	else
	{
		return CXTPControlButton::OnLButtonDblClk(point);
	}
	return FALSE;
}



void CXTPControlPopup::Copy(CXTPControl* pControl, BOOL bRecursive)
{
	ASSERT(DYNAMIC_DOWNCAST(CXTPControlPopup, pControl));

	CXTPControlButton::Copy(pControl, bRecursive);

	if (bRecursive && ((CXTPControlPopup*)pControl)->m_pCommandBar)
	{
		CXTPCommandBar* pCommandBar = ((CXTPControlPopup*)pControl)->m_pCommandBar->Clone(bRecursive);
		SetCommandBar(pCommandBar);
		pCommandBar->InternalRelease();

	}
	else
	{
		SetCommandBar(((CXTPControlPopup*)pControl)->m_pCommandBar);
	}

}

void CXTPControlPopup::SetCommandBar(CXTPCommandBar* pBar)
{
	if (m_pCommandBar)
		m_pCommandBar->InternalRelease();

	m_pCommandBar = pBar;

	if (m_pCommandBar)
		m_pCommandBar->InternalAddRef();

}

void CXTPControlPopup::SetCommandBar(CMenu* pMenu)
{
	if (!m_pCommandBar)
		m_pCommandBar = CXTPPopupBar::CreatePopupBar(m_pParent ? m_pParent->GetCommandBars() : NULL);

	m_pCommandBar->LoadMenu(pMenu);
}

BOOL CXTPControlPopup::IsContainPopup(CXTPControlPopup* pControlPopup)
{
	if (!m_pCommandBar || !pControlPopup->GetCommandBar())
		return FALSE;

	if (m_pCommandBar == pControlPopup->GetCommandBar()) return TRUE;

	for (int i = 0; i < m_pCommandBar->GetControlCount(); i++)
	{
		CXTPControlPopup* pControlChild = DYNAMIC_DOWNCAST(CXTPControlPopup, m_pCommandBar->GetControl(i));
		if (pControlChild)
		{
			if (pControlPopup->IsContainPopup(pControlChild))
				return TRUE;

			if (pControlChild->IsContainPopup(pControlPopup))
				return TRUE;
		}

	}
	return FALSE;
}


BOOL CXTPControlPopup::Compare(CXTPControl* pOther)
{
	if (!CXTPControlButton::Compare(pOther))
		return FALSE;

	CXTPControlPopup* pPopup = DYNAMIC_DOWNCAST(CXTPControlPopup, pOther);
	if (!pPopup)
		return FALSE;

	if (m_pCommandBar != NULL && pPopup->m_pCommandBar != NULL)
	{
		if (!m_pCommandBar->GetControls()->Compare(pPopup->m_pCommandBar->GetControls()))
			return FALSE;
	}

	return TRUE;

}

BOOL CXTPControlPopup::IsCustomizeDragOverAvail(CXTPCommandBar* pCommandBar, CPoint /*point*/, DROPEFFECT& /*dropEffect*/)
{
	while (pCommandBar != NULL)
	{
		if (pCommandBar == m_pCommandBar)
			return FALSE;

		pCommandBar = pCommandBar->GetParentCommandBar();
	}
	return TRUE;

}

void CXTPControlPopup::OnCustomizeDragOver(CXTPControl* pDataObject, CPoint /*point*/, DROPEFFECT& /*dropEffect*/)
{
	if (m_pCommandBar && m_pCommandBar->IsCustomizable())
	{
		CXTPControlPopup* pControlPopup = DYNAMIC_DOWNCAST(CXTPControlPopup, pDataObject);

		if (!(pControlPopup && pControlPopup->IsContainPopup(this)))
		{
			m_pParent->SetPopuped(m_nIndex);
		}
		else
		{
			m_pParent->SetPopuped(-1);
			m_pParent->SetSelected(-1);
		}
	}
}

void CXTPControlPopup::AdjustExcludeRect(CRect& rc, BOOL bVertical)
{
	GetPaintManager()->AdjustExcludeRect(rc, this, bVertical);
}


#ifdef _XTP_ACTIVEX


BEGIN_DISPATCH_MAP(CXTPControlPopup, CXTPControlButton)
	DISP_PROPERTY_EX_ID(CXTPControlPopup, "CommandBar", dispidCommandBar, OleGetCommandBar, OleSetCommandBar, VT_DISPATCH)
END_DISPATCH_MAP()

// {09028A35-789A-40d9-B564-7577DA549319}
static const GUID IID_ICommandBarPopup =
{ 0x9028a35, 0x789a, 0x40d9, { 0xb5, 0x64, 0x75, 0x77, 0xda, 0x54, 0x93, 0x19 } };

BEGIN_INTERFACE_MAP(CXTPControlPopup, CXTPControlButton)
	INTERFACE_PART(CXTPControlPopup, IID_ICommandBarPopup, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPControlPopup, IID_ICommandBarPopup)

LPDISPATCH CXTPControlPopup::OleGetCommandBar()
{
	return m_pCommandBar ? m_pCommandBar->GetIDispatch(TRUE) : 0;
}

void CXTPControlPopup::OleSetCommandBar(LPDISPATCH lpCommandBar)
{
	SetCommandBar(CXTPPopupBar::FromDispatch(lpCommandBar));
}

#endif
