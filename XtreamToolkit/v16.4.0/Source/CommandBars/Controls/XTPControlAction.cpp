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


IMPLEMENT_DYNAMIC(CXTPControlAction, CCmdTarget)

//////////////////////////////////////////////////////////////////////////
// CXTPControlAction

CXTPControlAction::CXTPControlAction(CXTPControlActions* pActions)
{
#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif

	m_nId = 0;
	m_nIconId = 0;
	m_nHelpId = 0;
	m_nTag = 0;
	m_bVisible = TRUE;
	m_bChecked = FALSE;
	m_bEnabled = TRUE;

	m_pTarget = NULL;

	m_pActions = pActions;
}

CXTPControlAction::~CXTPControlAction()
{
	ASSERT(m_arrControls.GetSize() == 0);

	if (m_pTarget)
	{
		CMDTARGET_RELEASE(m_pTarget);
	}
}

void CXTPControlAction::AddControl(CXTPControl* pControl)
{
	for (int i = 0; i < m_arrControls.GetSize(); i++)
	{
		if (m_arrControls[i] == pControl)
			return;
	}

	m_arrControls.Add(pControl);

	pControl->m_bEnabled = -1;
	pControl->m_bChecked = -1;
	pControl->m_nId = m_nId;

	pControl->m_strCaption.Empty();
	pControl->m_strTooltipText.Empty();
	pControl->m_strDescriptionText.Empty();
	pControl->m_strCategory.Empty();
	pControl->m_strShortcutText.Empty();
	pControl->m_strKeyboardTip.Empty();
	pControl->m_strCustomShortcutText.Empty();
}


void CXTPControlAction::RemoveControl(CXTPControl* pControl)
{
	for (int i = 0; i < m_arrControls.GetSize(); i++)
	{
		if (m_arrControls[i] == pControl)
		{
			m_arrControls.RemoveAt(i);

			pControl->m_bEnabled = TRUE;
			pControl->m_bChecked = FALSE;
			return;
		}
	}
}

void CXTPControlAction::OnChanged(int nProperty)
{
	for (int i = 0; i < m_arrControls.GetSize(); i++)
	{
		m_arrControls[i]->OnActionChanged(nProperty);
	}
}

void CXTPControlAction::OnChanging(int nProperty)
{
	for (int i = 0; i < m_arrControls.GetSize(); i++)
	{
		m_arrControls[i]->OnActionChanging(nProperty);
	}
}

void CXTPControlAction::RedrawControls()
{
	for (int i = 0; i < m_arrControls.GetSize(); i++)
	{
		m_arrControls[i]->RedrawParent();
	}
}

void CXTPControlAction::RepositionControls()
{
	for (int i = 0; i < m_arrControls.GetSize(); i++)
	{
		m_arrControls[i]->DelayLayoutParent();
	}
}

void CXTPControlAction::OnRemoved()
{
	while (m_arrControls.GetSize() > 0)
	{
		m_arrControls[0]->SetAction(NULL);
	}
}

void CXTPControlAction::SetCaption(UINT nIDCaption)
{
	CString strCaption;
	if (strCaption.LoadString(nIDCaption))
	{
		SetCaption(strCaption);
	}
}

void CXTPControlAction::SetCaption(LPCTSTR lpszCaption)
{
	CString strCaption(lpszCaption);
	int nShortCutPos = strCaption.Find ('\t');
	if (nShortCutPos != -1)
	{
		strCaption.ReleaseBuffer(nShortCutPos);
	}

	if (m_strCaption != strCaption)
	{
		OnChanging(3);
		m_strCaption = strCaption;
		OnChanged(3);

		RepositionControls();
	}
}

void CXTPControlAction::SetEditHint(LPCTSTR lpszEditHint)
{
	CString strEditHint(lpszEditHint);

	if (m_strEditHint != strEditHint)
	{
		OnChanging(4);
		m_strEditHint = strEditHint;
		OnChanged(4);
	}
}

void CXTPControlAction::SetPrompt(LPCTSTR lpszPrompt)
{
	if (lpszPrompt == NULL || lpszPrompt[0] == _T('\0'))
		return;

	if (_tcschr(lpszPrompt, _T('\n')) != NULL)
	{
		AfxExtractSubString(m_strTooltipText, lpszPrompt, 1);
		AfxExtractSubString(m_strDescriptionText, lpszPrompt, 0);
	}
	else
	{
		m_strDescriptionText = m_strTooltipText = lpszPrompt;
	}


	if (m_strCaption.IsEmpty())
	{
		AfxExtractSubString(m_strCaption, lpszPrompt, 2);
	}

	if (m_strKeyboardTip.IsEmpty())
	{
		AfxExtractSubString(m_strKeyboardTip, lpszPrompt, 3);
	}
}

BOOL CXTPControlAction::GetEnabled() const
{
	if (GetCommandBars()->IsCustomizeMode())
		return TRUE;

	return m_bEnabled;
}

CXTPCommandBars* CXTPControlAction::GetCommandBars() const
{
	return m_pActions->m_pCommandBars;
}

CXTPControl* CXTPControlAction::GetControl(int nIndex) const
{
	return m_arrControls[nIndex];
}

int CXTPControlAction::GetCount() const
{
	return (int)m_arrControls.GetSize();
}
