// XTPRibbonQuickAccessControls.cpp: implementation of the CXTPRibbonQuickAccessControls class.
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

#include "Common/XTPSystemHelpers.h"
#include "Common/XTPHookManager.h"

#include "CommandBars/XTPCommandBarsDefines.h"
#include "CommandBars/XTPCommandBar.h"
#include "CommandBars/XTPToolBar.h"
#include "CommandBars/XTPMenuBar.h"
#include "CommandBars/XTPPopupBar.h"
#include "CommandBars/XTPControls.h"
#include "CommandBars/XTPControl.h"
#include "CommandBars/XTPControlButton.h"
#include "CommandBars/XTPControlPopup.h"

#include "XTPRibbonPopups.h"
#include "XTPRibbonBar.h"
#include "XTPRibbonGroup.h"
#include "XTPRibbonQuickAccessControls.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPRibbonControls

IMPLEMENT_DYNCREATE(CXTPRibbonControls, CXTPControls)

CXTPRibbonControls::CXTPRibbonControls()
{

}

BOOL CXTPRibbonControls::OnControlRemoving(CXTPControl* pControl)
{
	if (pControl->GetRibbonGroup())
	{
		if (pControl->GetRibbonGroup()->GetControlGroupOption() == pControl)
			return TRUE;

		if ((CXTPControl*)pControl->GetRibbonGroup()->GetControlGroupPopup() == pControl)
			return TRUE;
	}

	CXTPRibbonBar* pRibbonBar = (CXTPRibbonBar*)m_pParent;

	if (pControl == (CXTPControl*)pRibbonBar->GetControlTab())
		return TRUE;

	if (pControl == pRibbonBar->m_pControlQuickAccess)
		return TRUE;

	if (pControl == pRibbonBar->m_pControlQuickAccessMore)
		return TRUE;

	if (pControl == pRibbonBar->m_pControlScrollGroupsLeft)
		return TRUE;

	if (pControl == pRibbonBar->m_pControlScrollGroupsRight)
		return TRUE;

	return FALSE;
}

void CXTPRibbonControls::OnControlRemoved(CXTPControl* pControl)
{
	pControl->m_pControls = NULL;

	if (pControl->GetRibbonGroup())
	{
		pControl->GetRibbonGroup()->Remove(pControl);
	}
	CXTPRibbonBar* pRibbonBar = (CXTPRibbonBar*)m_pParent;

	if (pControl == (CXTPControl*)pRibbonBar->m_pControlSystemButton)
	{
		pRibbonBar->m_pControlSystemButton = NULL;
	}

	if (pRibbonBar->IsQuickAccessControl(pControl))
	{
		pRibbonBar->GetQuickAccessControls()->Remove(pControl);
	}

	CXTPControls::OnControlRemoved(pControl);
}

BOOL CXTPRibbonControls::ShouldSerializeControl(CXTPControl* pControl)
{
	if (pControl->m_bTemporary)
		return FALSE;

	if (pControl->GetRibbonGroup() != NULL)
		return FALSE;

	CXTPRibbonBar* pRibbonBar = (CXTPRibbonBar*)pControl->GetParent();

	if (pControl == (CXTPControl*)pRibbonBar->m_pControlSystemButton)
		return FALSE;

	if (pRibbonBar->IsQuickAccessControl(pControl))
		return FALSE;

	if (pControl == (CXTPControl*)pRibbonBar->GetControlTab())
		return FALSE;

	if (pControl == pRibbonBar->m_pControlScrollGroupsLeft)
		return FALSE;

	if (pControl == pRibbonBar->m_pControlScrollGroupsRight)
		return FALSE;


	return TRUE;
}

void CXTPRibbonControls::Copy(CXTPControls* pControls, BOOL bRecursive)
{
	RemoveAll();

	for (int i = 0; i < pControls->GetCount(); i++)
	{
		CXTPControl* pControl = pControls->GetAt(i);
		if (ShouldSerializeControl(pControl))
		{
			AddClone(pControl, -1, bRecursive);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// CXTPRibbonQuickAccessControls

CXTPRibbonQuickAccessControls::CXTPRibbonQuickAccessControls()
{

}

CXTPRibbonQuickAccessControls::~CXTPRibbonQuickAccessControls()
{

}

int CXTPRibbonQuickAccessControls::IndexOf(CXTPControl* pControl) const
{
	for (int nIndex = GetCount() - 1; nIndex >= 0; nIndex--)
	{
		if (GetAt(nIndex) == pControl)
		{
			return nIndex;
		}
	}
	return -1;
}

void CXTPRibbonQuickAccessControls::OnControlAdded(CXTPControl* pControl)
{
	int nIndex = IndexOf(pControl) + (((CXTPRibbonBar*)m_pParent)->GetSystemButton() ? 1 : 0);

	m_pParent->GetControls()->InsertAt(pControl, nIndex);
	pControl->InternalAddRef();
}

void CXTPRibbonQuickAccessControls::OnControlRemoved(CXTPControl* pControl)
{
	m_pParent->GetControls()->Remove(pControl);
}


void CXTPRibbonQuickAccessControls::Remove(CXTPControl* pControl)
{
	for (int nIndex = 0; nIndex < GetCount(); nIndex++)
	{
		if (GetAt(nIndex) == pControl)
		{
			m_arrControls.RemoveAt(nIndex);
			OnControlRemoved(pControl);
			pControl->InternalRelease();
			return;
		}
	}
}

CXTPControl* CXTPRibbonQuickAccessControls::FindDuplicate(CXTPControl* pControl)
{
	for (int i = 0; i < GetCount(); i++)
	{
		CXTPControl* pQuickAccessControl = GetAt(i);
		if (pQuickAccessControl->GetID() == pControl->GetID() && pQuickAccessControl->IsVisible(xtpHideWrap) &&
			pQuickAccessControl->GetType() == pControl->GetType() && pQuickAccessControl->GetRuntimeClass() == pControl->GetRuntimeClass())
		{
			return pQuickAccessControl;
		}
	}
	return NULL;
}

void CXTPRibbonQuickAccessControls::RefreshIndexes()
{
	for (int nIndex = 0; nIndex < GetCount(); nIndex++)
	{
		CXTPControl* pControl = GetAt(nIndex);
		pControl->SetParent(m_pParent);
	}
}

void CXTPRibbonQuickAccessControls::Reset()
{
	RemoveAll();

	if (m_pOriginalControls)
	{
		CXTPControls* pControls = m_pOriginalControls;
		for (int i = 0; i < pControls->GetCount(); i++)
		{
			CXTPControl* pControl = pControls->GetAt(i);
			if (!pControl->IsTemporary())
				AddClone(pControl, -1, TRUE);
		}
	}

	if (m_pParent)
	{
		m_pParent->OnRecalcLayout();
	}
}
