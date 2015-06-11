// XTPControlAction.cpp : implementation of the CXTPControlAction class.
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

#include <Common/Resource.h>
#include <Common/XTPResourceManager.h>
#include <Common/XTPImageManager.h>
#include <Common/XTPSystemHelpers.h>
#include <Common/XTPHookManager.h>
#include <Common/XTPColorManager.h>
#include <Common/XTPDrawHelpers.h>
#include <Common/ScrollBar/XTPScrollInfo.h>

#include <CommandBars/Resource.h>
#include <CommandBars/XTPCommandBarsDefines.h>
#include <CommandBars/XTPControl.h>
#include <CommandBars/XTPControls.h>
#include <CommandBars/XTPCommandBar.h>
#include <CommandBars/XTPPaintManager.h>
#include <CommandBars/XTPCommandBars.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//////////////////////////////////////////////////////////////////////////
// CXTPControlActions

CXTPControlActions::CXTPControlActions(CXTPCommandBars* pCommandBars)
{
	m_pCommandBars = pCommandBars;

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPControlActions::~CXTPControlActions()
{
	RemoveAll();
}

CXTPControlAction* CXTPControlActions::GetAt(int nIndex) const
{
	return m_arrActions.GetAt(nIndex);
}

int CXTPControlActions::GetCount() const
{
	return (int)m_arrActions.GetSize();
}

CXTPControlAction* CXTPControlActions::FindAction(int nId) const
{
	if (nId <= 0 || nId >= 0xFFFFFFF)
		return NULL;

	int num = GetCount();
	if (num == 0)
		return NULL;

	CXTPControlAction* const* lo = m_arrActions.GetData();
	CXTPControlAction* const* hi = lo + (num - 1);
	CXTPControlAction* const* mid;

	while (lo <= hi)
	{
		int half = num / 2;
		if (half)
		{
			mid = lo + (num & 1 ? half : (half - 1));

			if ((*mid)->GetID() == nId)
				return (*mid);

			if ((*mid)->GetID() > nId)
			{
				hi = mid - 1;
				num = num & 1 ? half : half - 1;
			}
			else
			{
				lo = mid + 1;
				num = half;
			}
		}
		else if (num)
			return (*lo)->GetID() == nId ? (*lo) : NULL;
		else
			break;
	}

	return NULL;
}

void CXTPControlActions::RemoveAll()
{
	for (int i = 0; i < GetCount(); i++)
	{
		CXTPControlAction* pAction = GetAt(i);

		pAction->OnRemoved();
		pAction->InternalRelease();
	}
	m_arrActions.RemoveAll();
}

void CXTPControlActions::Remove(int nId)
{
	for (int i = 0; i < GetCount(); i++)
	{
		CXTPControlAction* pAction = GetAt(i);

		if (pAction->GetID() == nId)
		{
			m_arrActions.RemoveAt(i);
			pAction->OnRemoved();
			pAction->InternalRelease();
			return;
		}
	}
}

void CXTPControlActions::Insert(CXTPControlAction* pAction)
{
	int nIndex = 0;
	for (; nIndex < GetCount(); nIndex++)
	{
		if (GetAt(nIndex)->GetID() > pAction->GetID())
			break;
	}

	m_arrActions.InsertAt(nIndex, pAction);
}

void CXTPControlActions::SetActionId(CXTPControlAction* pAction, int nId)
{
	ASSERT(FindAction(nId) == NULL);
	pAction->m_nId = nId;
}

CXTPControlAction* CXTPControlActions::Add(int nId, CXTPControlAction* pAction)
{
	SetActionId(pAction, nId);

	CString strPrompt;
#ifdef _XTP_ACTIVEX
	if (nId > 500)
#endif
	if (XTPResourceManager()->LoadString(&strPrompt, nId))
	{
		pAction->SetPrompt(strPrompt);
	}

	Insert(pAction);

	return pAction;
}

CXTPControlAction* CXTPControlActions::Add(int nId)
{
	CXTPControlAction* pAction = FindAction(nId);
	if (pAction)
		return pAction;

	pAction = new CXTPControlAction(this);
	return Add(nId, pAction);
}

void CXTPControlActions::CreateFromMenu(CMenu* pMenu)
{
	int nCount = ::GetMenuItemCount(pMenu->m_hMenu);

	for (int nIndex = 0; nIndex < nCount; nIndex++)
	{
		// Check to see if the item is a separator, we don't want
		// to use GetMenuItemID(i) because it may not equal zero.

		MENUITEMINFO info = { sizeof(MENUITEMINFO), MIIM_TYPE | MIIM_STATE};
		::GetMenuItemInfo(pMenu->m_hMenu, nIndex, TRUE, &info);

		int nID = (int)pMenu->GetMenuItemID(nIndex);
		BOOL bSeparator = ((info.fType & MFT_SEPARATOR) == MFT_SEPARATOR) || (nID == 0);

		if (!bSeparator)
		{
			CMenu* pPopupMenu = pMenu->GetSubMenu(nIndex);

			CString strCaption;
			XTPResourceManager()->GetMenuLocaleString(pMenu, nIndex, strCaption, MF_BYPOSITION);

			if (pPopupMenu)
			{
				nID = 0;
			}

			int iPos = strCaption.Find(_T('\t'));
			if (pPopupMenu &&  iPos > 0)
			{
				nID = _ttoi(strCaption.Mid(iPos + 1));
				strCaption.ReleaseBuffer(iPos);
			}

			if (nID > 0)
			{
				CXTPControlAction* pAction = Add(nID);

				pAction->SetCaption(strCaption);

				CString strPrompt;
				if (XTPResourceManager()->LoadString(&strPrompt, nID))
				{
					pAction->SetPrompt(strPrompt);
				}
			}

			if (pPopupMenu)
			{
				CreateFromMenu(pPopupMenu);
			}
		}
	}
}
