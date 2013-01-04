// XTPRibbonGroup.cpp: implementation of the CXTPRibbonGroup class.
//
// This file is a part of the XTREME RIBBON MFC class library.
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

#include "Common/XTPImageManager.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPHookManager.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPDrawHelpers.h"

#include "CommandBars/XTPCommandBarsDefines.h"
#include "CommandBars/XTPCommandBars.h"
#include "CommandBars/XTPCommandBar.h"
#include "CommandBars/XTPToolBar.h"
#include "CommandBars/XTPMenuBar.h"
#include "CommandBars/XTPPopupBar.h"
#include "CommandBars/XTPControls.h"
#include "CommandBars/XTPControl.h"
#include "CommandBars/XTPControlButton.h"
#include "CommandBars/XTPControlPopup.h"
#include "CommandBars/XTPPaintManager.h"

#include "XTPRibbonGroup.h"
#include "XTPRibbonGroups.h"
#include "XTPRibbonPopups.h"
#include "XTPRibbonBar.h"
#include "XTPRibbonPaintManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(CXTPRibbonGroup, CCmdTarget, VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)


//////////////////////////////////////////////////////////////////////////
// CXTPRibbonGroupOption
IMPLEMENT_XTP_CONTROL(CXTPRibbonGroupOption, CXTPControlButton)

CXTPRibbonGroupOption::CXTPRibbonGroupOption()
{
	SetFlags(xtpFlagNoMovable);
}

void CXTPRibbonGroupOption::Draw(CDC* pDC)
{
	if (m_pRibbonGroup)
	{
		m_pRibbonGroup->GetRibbonBar()->GetRibbonPaintManager()->DrawControlGroupOption(pDC, this, TRUE);
	}
	else
	{
		CXTPControlButton::Draw(pDC);
	}
}

//////////////////////////////////////////////////////////////////////////
// CXTPRibbonGroupControlPopup


CXTPRibbonGroupControlPopup::CXTPRibbonGroupControlPopup(CXTPRibbonGroup* pGroup)
{
	m_pRibbonGroup = pGroup;
	m_pPopupGroup = pGroup;
	SetFlags(xtpFlagManualUpdate | xtpFlagNoMovable);
	m_controlType = xtpControlButtonPopup;
	m_pCommandBar = CXTPPopupBar::CreatePopupBar(0);
}

CSize CXTPRibbonGroupControlPopup::GetSize (CDC* pDC)
{
	return GetParent()->GetPaintManager()->GetRibbonPaintManager()->DrawControlGroupPopup(pDC, this, FALSE);
}

void CXTPRibbonGroupControlPopup::Draw(CDC* pDC)
{
	GetParent()->GetPaintManager()->GetRibbonPaintManager()->DrawControlGroupPopup(pDC, this, TRUE);
}

BOOL CXTPRibbonGroupControlPopup::IsTransparent() const
{
	return TRUE;
}

CXTPRibbonBar* CXTPRibbonGroupControlPopup::GetRibbonBar() const
{
	ASSERT(m_pParent);
	if (!m_pParent)
		return NULL;

	if (DYNAMIC_DOWNCAST(CXTPRibbonBar, m_pParent))
		return (CXTPRibbonBar*)m_pParent;

	if (DYNAMIC_DOWNCAST(CXTPRibbonGroupPopupToolBar, m_pParent))
		return ((CXTPRibbonGroupPopupToolBar*)m_pParent)->m_pRibbonBar;

	if (DYNAMIC_DOWNCAST(CXTPRibbonTabPopupToolBar, m_pParent))
		return ((CXTPRibbonTabPopupToolBar*)m_pParent)->m_pRibbonBar;

	if (DYNAMIC_DOWNCAST(CXTPRibbonBarMorePopupToolBar, m_pParent))
		return ((CXTPRibbonBarMorePopupToolBar*)m_pParent)->m_pRibbonBar;

	ASSERT(FALSE);
	return NULL;
}

void CXTPRibbonGroupControlPopup::Copy(CXTPControl* pControl, BOOL bRecursive)
{
	CXTPControlButton::Copy(pControl, bRecursive);

	if (m_pParent && pControl->GetParent() && GetRibbonBar() != ((CXTPRibbonGroupControlPopup*)pControl)->GetRibbonBar())
	{
		m_pPopupGroup = GetRibbonBar()->FindGroup(GetID());
	}
	else
	{
		m_pPopupGroup = ((CXTPRibbonGroupControlPopup*)pControl)->m_pPopupGroup;
	}
}

void CXTPRibbonGroupControlPopup::GenerateCommandBarList(DWORD& /*nID*/, CXTPCommandBarList* /*pCommandBarList*/, XTP_COMMANDBARS_PROPEXCHANGE_PARAM* /*pParam*/)
{
}

void CXTPRibbonGroupControlPopup::OnLButtonUp(CPoint /*point*/)
{

}

BOOL CXTPRibbonGroupControlPopup::IsVisible(DWORD dwSkipFlags /*= 0*/) const
{
	return CXTPControlPopup::IsVisible(dwSkipFlags) &&
		(!m_pPopupGroup || m_pPopupGroup->m_bVisible);
}

IMPLEMENT_XTP_CONTROL(CXTPRibbonGroupControlPopup, CXTPControlPopup)

BOOL CXTPRibbonGroupControlPopup::OnSetPopup(BOOL bPopup)
{
	m_bPopup = bPopup;
	if (bPopup)
	{
		CMDTARGET_RELEASE(m_pCommandBar);

		CXTPCommandBars* pCommandBars = m_pParent->GetCommandBars();
		ASSERT(pCommandBars);

		if (!m_pPopupGroup)
		{
			m_pCommandBar = new CXTPPopupToolBar();
			m_pCommandBar->SetCommandBars(pCommandBars);
			m_pCommandBar->Popup(this, IsKeyboardSelected(GetSelected()));
		}
		else
		{

			m_pCommandBar = new CXTPRibbonGroupPopupToolBar(m_pPopupGroup);
			m_pCommandBar->SetCommandBars(pCommandBars);

			m_pCommandBar->EnableCustomization(FALSE);
			m_pCommandBar->Popup(this, IsKeyboardSelected(GetSelected()));
		}
	}
	else
	{
		ASSERT(m_pCommandBar);
		if (m_pCommandBar) m_pCommandBar->SetTrackingMode(FALSE);

	}
	RedrawParent();
	return TRUE;
}


CXTPRibbonGroup::CXTPRibbonGroup()
{
	m_nIndex = -1;
	m_pGroups = NULL;
	m_pParent = NULL;
	m_pRibbonBar = NULL;
	m_rcGroup.SetRectEmpty();
	m_nId = -1;
	m_nIconId = -1;
	m_bReduced = FALSE;
	m_nAllowReduce = 4;
	m_bVisible = TRUE;
	m_bControlsCentering = FALSE;

	m_bAutoArrangeEditCaption = TRUE;


	m_pControlGroupOption = new CXTPRibbonGroupOption();
	m_pControlGroupOption->m_pRibbonGroup = this;

	m_pControlGroupPopup = new CXTPRibbonGroupControlPopup(this);

	m_pControlGroupPopup->m_buttonRibbonStyle = xtpButtonIconAndCaptionBelow;

	m_bShowOptionButton = FALSE;

	m_pLayoutInfo = NULL;

	m_nRowCount = 2;
	m_bControlsGrouping = FALSE;
}

CXTPRibbonGroup::~CXTPRibbonGroup()
{
	RemoveAll();

	CMDTARGET_RELEASE(m_pControlGroupOption);
	CMDTARGET_RELEASE(m_pControlGroupPopup);
}

void CXTPRibbonGroup::SetID(int nId)
{
	m_nId = nId;
	m_pControlGroupOption->SetID(m_nId);
	m_pControlGroupPopup->SetID(m_nId);
}

void CXTPRibbonGroup::SetCaption(LPCTSTR lpszCaption)
{
	if (m_strCaption != lpszCaption)
	{
		m_strCaption = lpszCaption;
		if (m_pParent) m_pParent->DelayLayout();
	}
}

void CXTPRibbonGroup::Copy(CXTPRibbonGroup* pGroup)
{
	m_nId = pGroup->m_nId;
	m_nIconId = pGroup->m_nIconId;
	m_bShowOptionButton = pGroup->m_bShowOptionButton;
	m_strCaption = pGroup->m_strCaption;
	m_pGroups = pGroup->m_pGroups;
	m_bControlsGrouping = pGroup->m_bControlsGrouping;
	m_bControlsCentering = pGroup->m_bControlsCentering;
	m_bAutoArrangeEditCaption = pGroup->m_bAutoArrangeEditCaption;
	m_bVisible = pGroup->m_bVisible;
	m_nAllowReduce = pGroup->m_nAllowReduce;

	int nControlPos = m_pControlGroupOption->GetIndex();

	for (int i = 0; i < pGroup->GetCount(); i++)
	{
		CXTPControl* pControl = pGroup->GetAt(i);

		CXTPControl* pClone = m_pParent->GetControls()->AddClone(pControl, nControlPos + i);
		pClone->m_pRibbonGroup = this;
		pClone->SetHideFlag(xtpHideRibbonTab, FALSE);
		m_arrControls.Add(pClone);
		pClone->InternalAddRef();
	}

	m_pControlGroupOption->Copy(pGroup->m_pControlGroupOption, FALSE);
	m_pControlGroupOption->SetHideFlag(xtpHideRibbonTab, FALSE);

	m_pControlGroupPopup->Copy(pGroup->m_pControlGroupPopup, FALSE);
	m_pControlGroupPopup->SetHideFlag(xtpHideRibbonTab, FALSE);
}

void CXTPRibbonGroup::Remove(CXTPControl* pControl)
{
	for (int nIndex = 0; nIndex < m_arrControls.GetSize(); nIndex++)
	{
		if (m_arrControls.GetAt(nIndex) == pControl)
		{
			RemoveAt(nIndex);
			return;
		}
	}
}

CXTPControl* CXTPRibbonGroup::FindControl(int nId) const
{
	return FindControl(xtpControlError, nId, FALSE, FALSE);
}

CXTPControl* CXTPRibbonGroup::FindControl(XTPControlType type, int nId, BOOL bVisible, BOOL bRecursive) const
{
	for (int nIndex = 0; nIndex < GetCount(); nIndex++)
	{
		CXTPControl* pControl = GetAt(nIndex);
		if ((type == xtpControlError || pControl->GetType() == type) &&
			(!bVisible || pControl->IsVisible()) &&
			(nId == -1 || nId == pControl->GetID()))
			return pControl;

		if (bRecursive)
		{
			CXTPCommandBar* pBar = pControl->GetCommandBar();
			if (pBar)
			{
				pControl = pBar->GetControls()->FindControl(type, nId, bVisible, bRecursive);
				if (pControl != NULL) return pControl;
			}
		}
	}
	return NULL;
}

CXTPControl* CXTPRibbonGroup::SetControlType(CXTPControl* pControl, XTPControlType type)
{
	ASSERT(pControl);
	if (!pControl)
		return NULL;

	if (IsPopupControlType(pControl->GetType()) && IsPopupControlType(type))
	{
		pControl->m_controlType = type;
		return pControl;
	}

	CXTPControl* pNew = Add(type, 0, _T(""), IndexOf(pControl) + 1, FALSE);
	pNew->CXTPControl::Copy(pControl, FALSE);
	pNew->m_controlType = type;

	Remove(pControl);

	return pNew;

}
void CXTPRibbonGroup::RemoveAt(int nIndex)
{
	if (nIndex >= 0 && nIndex < GetCount())
	{
		CXTPControl* pControl = m_arrControls.GetAt(nIndex);
		pControl->m_pRibbonGroup = NULL;
		m_arrControls.RemoveAt(nIndex);

		GetRibbonBar()->GetControls()->Remove(pControl);
		pControl->InternalRelease();
	}
}

void CXTPRibbonGroup::RemoveAll()
{
	for (int nIndex = (int)m_arrControls.GetSize() - 1; nIndex >= 0; nIndex--)
	{
		CXTPControl* pControl = m_arrControls.GetAt(nIndex);
		pControl->m_pRibbonGroup = NULL;
		m_arrControls.RemoveAt(nIndex);

		m_pParent->GetControls()->Remove(pControl);
		pControl->InternalRelease();
	}
}

CXTPRibbonTab* CXTPRibbonGroup::GetParentTab() const
{
	return m_pGroups->GetParentTab();
}

BOOL CXTPRibbonGroup::IsHighlighted() const
{
	CXTPRibbonScrollableBar* pScrollableBar = GetScrollableBar(m_pParent);

	return pScrollableBar && pScrollableBar->GetHighlightedGroup() == this;
}

int CXTPRibbonGroup::GetMinimumWidth(CDC* pDC)
{
	CXTPFontDC fnt(pDC, &GetRibbonBar()->GetRibbonPaintManager()->m_fontGroupCaption);
	CSize sz = pDC->GetTextExtent(m_strCaption);

	if (m_bShowOptionButton)
		sz.cx += GetRibbonBar()->GetRibbonPaintManager()->GetGroupCaptionHeight();

	return sz.cx;
}

BOOL CXTPRibbonGroup::IsOptionButtonVisible() const
{
	return !m_bReduced && m_bShowOptionButton;
}

void CXTPRibbonGroup::SetRect(CRect rc)
{
	m_rcGroup = rc;

	if (m_bReduced)
	{
		CRect rcEntry(rc.left, rc.top, rc.right, rc.bottom);
		m_pControlGroupPopup->SetHideFlag(xtpHideWrap, FALSE);
		m_pControlGroupPopup->SetRect(rcEntry);
		m_pControlGroupPopup->SetWrap(TRUE);
	}
	else
	{
		m_pControlGroupPopup->SetHideFlag(xtpHideWrap, TRUE);
	}

	if (!m_bShowOptionButton)
	{
		m_pControlGroupOption->SetHideFlag(xtpHideGeneric, TRUE);
	}
	else if (m_bReduced)
	{
		m_pControlGroupOption->SetHideFlag(xtpHideWrap, TRUE);

	}
	else
	{
		int nHeight = GetRibbonBar()->GetRibbonPaintManager()->GetGroupCaptionHeight();
		m_pControlGroupOption->SetWrap(TRUE);
		m_pControlGroupOption->SetHideFlag(xtpHideGeneric, FALSE);
		m_pControlGroupOption->SetHideFlag(xtpHideWrap, FALSE);
		m_pControlGroupOption->SetRect(CRect(rc.right - nHeight - 1, rc.bottom - nHeight, rc.right - 3, rc.bottom - 3));
	}
}

void CXTPRibbonGroup::RepositionControls(CDC* pDC)
{
	CXTPRibbonPaintManager* pPaintManager = GetRibbonBar()->GetRibbonPaintManager();

	int x = m_rcGroup.left + 4;
	int y = m_rcGroup.top + pPaintManager->GetGroupCaptionHeight();

	for (int i = 0; i < GetCount(); i++)
	{
		CXTPControl* pControl = GetAt(i);
		if (!pControl->IsVisible())
			continue;

		CSize sz = pControl->GetSize(pDC);
		pControl->SetRect(CRect(x, y, x + sz.cx, y + sz.cy));

		x += sz.cx;
	}
}

CRect CXTPRibbonGroup::GetCaptionRect() const
{
	CRect rcCaption(m_rcGroup);
	rcCaption.top = rcCaption.bottom - GetRibbonBar()->GetRibbonPaintManager()->GetGroupCaptionHeight();
	return rcCaption;
}


void CXTPRibbonGroup::Draw(CDC* pDC, CRect rcClipBox)
{
	GetRibbonBar()->GetRibbonPaintManager()->DrawGroup(pDC, this);

	BOOL bFirst = TRUE;

	for (int i = 0; i < GetCount(); i++)
	{
		CXTPControl* pControl = GetAt(i);
		if (!pControl->IsVisible())
			continue;

		if (!m_bControlsGrouping && pControl->GetBeginGroup() && !bFirst)
		{
			GetRibbonBar()->GetPaintManager()->
				DrawCommandBarSeparator(pDC, GetRibbonBar(), pControl);
		}

		if (CRect().IntersectRect(rcClipBox, pControl->GetRect()))
		{
			pControl->Draw(pDC);
		}

		bFirst = FALSE;
	}

	if (m_pControlGroupPopup->IsVisible())
	{
		m_pControlGroupPopup->Draw(pDC);
	}

	if (m_pControlGroupOption->IsVisible())
	{
		m_pControlGroupOption->Draw(pDC);
	}
}

int CXTPRibbonGroup::IndexOf(CXTPControl* pControl) const
{
	if (pControl->GetRibbonGroup() != this)
		return -1;

	return pControl->GetIndex() + GetCount() - m_pControlGroupOption->GetIndex();
}


CXTPControl* CXTPRibbonGroup::AddClone(CXTPControl* pClone, int nBefore, BOOL bRecursive)
{
	if (nBefore < 0 || nBefore >= GetCount())
		nBefore = (int)GetCount();

	CXTPControl* pControl = (CXTPControl*)pClone->GetRuntimeClass()->CreateObject();
	ASSERT(pControl);
	if (!pControl)
		return NULL;

	int nControlPos = m_pControlGroupOption->GetIndex() - GetCount() + nBefore;

	GetRibbonBar()->GetControls()->InsertAt(pControl, nControlPos);

	m_arrControls.InsertAt(nBefore, pControl);
	pControl->InternalAddRef();

	pControl->Copy(pClone, bRecursive);


	pControl->m_pRibbonGroup = this;
	pControl->SetHideFlag(xtpHideRibbonTab, !IsVisible());

	return pControl;
}

CXTPControl* CXTPRibbonGroup::Add(XTPControlType controlType, int nId, LPCTSTR lpszParameter, int nBefore, BOOL bTemporary)
{
	if (nBefore < 0 || nBefore >= GetCount())
		nBefore = (int)GetCount();

	int nControlPos = m_pControlGroupOption->GetIndex() - GetCount() + nBefore;

	CXTPControl* pControl = GetRibbonBar()->GetControls()->Add(controlType, nId, lpszParameter, nControlPos, bTemporary);
	ASSERT(pControl);
	if (!pControl)
		return NULL;

	m_arrControls.InsertAt(nBefore, pControl);
	pControl->InternalAddRef();

	pControl->m_pRibbonGroup = this;
	pControl->SetHideFlag(xtpHideRibbonTab, !IsVisible());

	return pControl;
}

void CXTPRibbonGroup::SetVisible(BOOL bVisible)
{
	if (m_bVisible != bVisible)
	{
		m_bVisible = bVisible;

		GetRibbonBar()->RebuildControls(GetRibbonBar()->GetSelectedTab());
		GetRibbonBar()->OnRecalcLayout();
	}
}

BOOL CXTPRibbonGroup::IsVisible() const
{
	if (m_pRibbonBar != m_pParent)
		return m_bVisible;

	return GetRibbonBar()->GetSelectedTab() == GetParentTab() && m_bVisible;
}

void CXTPRibbonGroup::OnGroupAdded()
{
	CXTPControl* pGroupOption = GetControlGroupOption();

	m_pParent->GetControls()->Add(pGroupOption);
	pGroupOption->InternalAddRef();

	pGroupOption->SetHideFlag(xtpHideRibbonTab, !IsVisible());


	pGroupOption->SetID(GetID());

	CXTPControl* pGroupPopup = (CXTPControl*)GetControlGroupPopup();
	m_pParent->GetControls()->Add(pGroupPopup);
	pGroupPopup->SetCaption(GetCaption());
	pGroupPopup->SetDescription(NULL);

	pGroupPopup->InternalAddRef();

	pGroupPopup->SetHideFlag(xtpHideRibbonTab, !IsVisible());

	CXTPCommandBars* pCommandBars = m_pRibbonBar->GetCommandBars();
	ASSERT(pCommandBars);

	if (pCommandBars && pCommandBars->IsActionsEnabled())
	{
		CXTPControlAction* pAction = pCommandBars->CreateAction(GetID());
		if (pAction->GetCaption().IsEmpty()) pAction->SetCaption(GetCaption());

		pGroupPopup->SetAction(pAction);
		pGroupOption->SetAction(pAction);
	}
}

void CXTPRibbonGroup::OnGroupRemoved()
{
	RemoveAll();

	if (m_pParent == m_pRibbonBar)
	{
		GetControlGroupPopup()->m_pRibbonGroup = NULL;
		m_pParent->GetControls()->Remove(GetControlGroupPopup());

		if (m_pRibbonBar->m_pHighlightedGroup == this)
		{
			m_pRibbonBar->m_pHighlightedGroup = NULL;
		}
	}

	GetControlGroupOption()->m_pRibbonGroup = NULL;
	m_pParent->GetControls()->Remove(GetControlGroupOption());
}


BOOL CXTPRibbonGroup::SetButtons(UINT* pButtons, int nCount)
{
	BOOL bSeparator = FALSE;

	CXTPRibbonBar* pRibbonBar = GetRibbonBar();
	CWnd* pSite = pRibbonBar->GetSite();

	for (int i = 0; i < nCount; i++)
	{
		if (pButtons[i] == 0)
			bSeparator = TRUE;
		else
		{
			XTPControlType controlType = xtpControlButton;
			XTPButtonStyle buttonStyle = xtpButtonAutomatic;
			CXTPControl* pControl = NULL;
			UINT nID = pButtons[i];

			XTP_COMMANDBARS_CREATECONTROL cs;

			if (pSite)
			{
				cs.nID = nID;
				cs.pControl = NULL;
				cs.bToolBar = TRUE;
				cs.pMenu = NULL;
				cs.nIndex = i;
				cs.strCaption = pRibbonBar->GetTitle();
				cs.controlType = controlType;
				cs.pCommandBar = pRibbonBar;
				cs.buttonStyle = buttonStyle;

				if (pSite->SendMessage(WM_XTP_BEFORECREATECONTROL, 0, (LPARAM)&cs) != 0)
				{
					pControl = cs.pControl;
					controlType = cs.controlType;
					buttonStyle = cs.buttonStyle;
					nID = cs.nID;
				}
			}

			int nControlPos = m_pControlGroupOption->GetIndex();

			if (pControl == NULL)
			{
				pControl = pRibbonBar->GetControls()->Add(controlType, nID, NULL, nControlPos);
				if (pControl)
				{
					pControl->SetStyle(buttonStyle);
					if (controlType == xtpControlPopup) pControl->SetIconId(nID);
				}
			}
			else pRibbonBar->GetControls()->Add(pControl, nID, NULL, nControlPos);

			if (!pControl)
				continue;

			if (bSeparator)
			{
				pControl->SetBeginGroup(TRUE);
				bSeparator = FALSE;
			}

			m_arrControls.InsertAt(GetCount(), pControl);
			pControl->InternalAddRef();

			pControl->m_pRibbonGroup = this;
			pControl->SetHideFlag(xtpHideRibbonTab, !IsVisible());

			if (pSite)
			{
				cs.pControl = pControl;
				pSite->SendMessage(WM_XTP_AFTERCREATECONTROL, 0, (LPARAM)&cs);
			}
		}
	}
	return TRUE;
}

CXTPControl* CXTPRibbonGroup::Add(CXTPControl* pControl, int nId, LPCTSTR lpszParameter, int nBefore, BOOL bTemporary)
{
	ASSERT(pControl);
	if (!pControl)
		return NULL;

	if (nBefore < 0 || nBefore >= GetCount())
		nBefore = (int)GetCount();

	int nControlPos = m_pControlGroupOption->GetIndex() - GetCount() + nBefore;

	GetRibbonBar()->GetControls()->Add(pControl, nId, lpszParameter, nControlPos, bTemporary);

	m_arrControls.InsertAt(nBefore, pControl);
	pControl->InternalAddRef();

	pControl->m_pRibbonGroup = this;
	pControl->SetHideFlag(xtpHideRibbonTab, !IsVisible());

	return pControl;
}

CXTPControl* CXTPRibbonGroup::InsertAt(CXTPControl* pControl, int nBefore)
{
	ASSERT(pControl);
	if (!pControl)
		return NULL;

	if (nBefore < 0 || nBefore >= GetCount())
		nBefore = (int)GetCount();

	int nControlPos = m_pControlGroupOption->GetIndex() - GetCount() + nBefore;

	GetRibbonBar()->GetControls()->InsertAt(pControl, nControlPos);

	m_arrControls.InsertAt(nBefore, pControl);
	pControl->InternalAddRef();

	pControl->m_pRibbonGroup = this;
	pControl->SetHideFlag(xtpHideRibbonTab, !IsVisible());

	return pControl;
}

BOOL CXTPRibbonGroup::LoadToolBar(UINT nIDResource, BOOL bLoadIcons)
{
	LPCTSTR lpszResourceName = MAKEINTRESOURCE(nIDResource);

	struct CToolBarData
	{
		WORD wVersion;
		WORD wWidth;
		WORD wHeight;
		WORD wItemCount;
		WORD* items()
		{ return (WORD*)(this + 1); }
	};

	ASSERT_VALID(this);
	ASSERT(lpszResourceName != NULL);
	if (!lpszResourceName)
		return FALSE;

	// determine location of the bitmap in resource fork
	HINSTANCE hInst = AfxFindResourceHandle(lpszResourceName, RT_TOOLBAR);
	if (!hInst)
		return FALSE;

	HRSRC hRsrc = ::FindResource(hInst, lpszResourceName, RT_TOOLBAR);
	if (hRsrc == NULL)
		return FALSE;

	HGLOBAL hGlobal = LoadResource(hInst, hRsrc);
	if (hGlobal == NULL)
		return FALSE;

	CToolBarData* pData = (CToolBarData*)LockResource(hGlobal);
	if (pData == NULL)
		return FALSE;

	ASSERT(pData->wVersion == 1);

	int i;
	UINT* pItems = new UINT[pData->wItemCount];

	for (i = 0; i < pData->wItemCount; i++)
		pItems[i] = pData->items()[i];

	BOOL bResult = SetButtons(pItems, pData->wItemCount);

	if (bLoadIcons)
	{
		CXTPImageManager* pImageManager = GetRibbonBar()->GetImageManager();

		if (!pImageManager->SetIcons(nIDResource, pItems,
			pData->wItemCount, CSize(pData->wWidth, pData->wHeight)))
			bResult = FALSE;
	}

	delete[] pItems;

	UnlockResource(hGlobal);
	FreeResource(hGlobal);


	return bResult;
}

BOOL CXTPRibbonGroup::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	if (nCode == CN_EVENT && nID == XTP_CN_REDRAWPARENT)
	{
		GetRibbonBar()->Redraw(0, FALSE);
		return TRUE;
	}
	return CCmdTarget::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

