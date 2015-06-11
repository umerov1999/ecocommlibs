// XTPControlComboBoxPopupBar.cpp : implementation of the CXTPControlComboBoxPopupBar class.
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


//////////////////////////////////////////////////////////////////////////
// CXTPControlComboBoxPopupBar


IMPLEMENT_XTP_COMMANDBAR(CXTPControlComboBoxPopupBar, CXTPPopupBar)

CXTPControlComboBoxPopupBar::CXTPControlComboBoxPopupBar()
{
	m_bComboBar = TRUE;
	m_bGrabFocus = FALSE;
}

BOOL CXTPControlComboBoxPopupBar::ProcessHookKeyDown(CXTPControlComboBox* pComboBox, UINT nChar, LPARAM lParam)
{
	int nSel = GetCurSel();

	if (pComboBox->GetEditCtrl())
	{
		if ((GetKeyState(VK_MENU) >= 0) && (nChar == VK_UP || nChar == VK_DOWN || nChar == VK_PRIOR || nChar == VK_NEXT))
		{
			CXTPCommandBar::OnHookKeyDown(nChar, lParam);

			if (nChar == VK_DOWN && nSel < (GetCount() - 1) )
				SetCurSel(nSel + 1);
			else if (nChar == VK_UP && nSel > 1 )
				SetCurSel(nSel - 1);
		}
		else
		{
			return FALSE;
		}
	}
	else
	{
		if ((nChar == VK_F4 && GetKeyState(VK_MENU) >= 0) || ((nChar == VK_UP || nChar == VK_DOWN) && GetKeyState(VK_MENU) < 0))
		{
			return FALSE;
		}
		else
		{
			CXTPCommandBar::OnHookKeyDown(nChar, lParam);
		}
	}

	if (nSel != GetCurSel())
	{
		pComboBox->OnSelChanged();
		if (pComboBox->GetEditCtrl()->GetSafeHwnd()) pComboBox->GetEditCtrl()->SetSel(0, -1);
	}
	return TRUE;
}

BOOL CXTPControlComboBoxPopupBar::OnHookKeyDown(UINT nChar, LPARAM lParam)
{
	ASSERT(m_pControlPopup);
	CXTPControlComboBox* pComboBox = ((CXTPControlComboBox*)m_pControlPopup);
	if (!pComboBox)
		return CXTPCommandBar::OnHookKeyDown(nChar, lParam);

	if (nChar == VK_ESCAPE)
	{
		if (pComboBox->IsFocused())
			return FALSE;

		return CXTPCommandBar::OnHookKeyDown(nChar, lParam);
	}

	if (nChar == VK_TAB) return FALSE;

	return ProcessHookKeyDown(pComboBox, nChar, lParam);
}
