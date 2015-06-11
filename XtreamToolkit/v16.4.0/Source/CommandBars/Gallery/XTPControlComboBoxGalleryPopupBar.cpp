// XTPControlGallery.cpp : implementation file.
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

#include "stdafx.h"
#include <math.h>

#include <Common/XTPImageManager.h>
#include <Common/XTPPropExchange.h>
#include <Common/XTPToolTipContext.h>
#include <Common/XTPSystemHelpers.h>
#include <Common/XTPHookManager.h>
#include <Common/XTPColorManager.h>
#include <Common/XTPDrawHelpers.h>
#include <Common/XTPMarkupRender.h>
#include <Common/ScrollBar/XTPScrollBase.h>
#include <Common/ScrollBar/XTPScrollInfo.h>
#include <Common/ScrollBar/XTPScrollBarPaintManager.h>

#include <CommandBars/XTPCommandBarsDefines.h>
#include <CommandBars/XTPCommandBar.h>
#include <CommandBars/XTPPopupBar.h>
#include <CommandBars/XTPControl.h>
#include <CommandBars/XTPControlButton.h>
#include <CommandBars/XTPControlPopup.h>
#include <CommandBars/XTPControlEdit.h>
#include <CommandBars/XTPControlComboBox.h>

#include <CommandBars/XTPControls.h>
#include <CommandBars/XTPControlGallery.h>
#include <CommandBars/XTPControlComboBoxGalleryPopupBar.h>
#include <CommandBars/XTPPaintManager.h>
#include <CommandBars/XTPCommandBars.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//////////////////////////////////////////////////////////////////////////
// CXTPControlComboBoxGalleryPopupBar

IMPLEMENT_XTP_COMMANDBAR(CXTPControlComboBoxGalleryPopupBar, CXTPControlComboBoxPopupBar)

CXTPControlComboBoxGalleryPopupBar::CXTPControlComboBoxGalleryPopupBar()
{
}

CXTPControlComboBoxGalleryPopupBar* AFX_CDECL CXTPControlComboBoxGalleryPopupBar::CreateComboBoxGalleryPopupBar(CXTPCommandBars* pCommandBars)
{
	CXTPControlComboBoxGalleryPopupBar* pPopupBar = new CXTPControlComboBoxGalleryPopupBar();
	pPopupBar->SetCommandBars(pCommandBars);
	return pPopupBar;
}

BEGIN_MESSAGE_MAP(CXTPControlComboBoxGalleryPopupBar, CXTPControlComboBoxPopupBar)
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

CXTPControlGallery* CXTPControlComboBoxGalleryPopupBar::GetGalleryItem() const
{
	CXTPControl* pItem = GetControl(0);
	ASSERT_KINDOF(CXTPControlGallery, pItem);

	return (CXTPControlGallery*)pItem;
}

void CXTPControlComboBoxGalleryPopupBar::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (GetControls()->HitTest(point) == GetGalleryItem())
	{
		CXTPControlComboBox* pComboBox = (CXTPControlComboBox*)m_pControlPopup;
		pComboBox->OnSelChanged();
		pComboBox->OnExecute();
		return;
	}

	CXTPPopupBar::OnLButtonUp(nFlags, point);
}

BOOL CXTPControlComboBoxGalleryPopupBar::SetTrackingMode(int bMode, BOOL bSelectFirst, BOOL bKeyboard)
{
	CXTPControlComboBox* pComboBox = ((CXTPControlComboBox*)m_pControlPopup);

	if (!CXTPPopupBar::SetTrackingMode(bMode, bSelectFirst, bKeyboard))
		return FALSE;

	if (!bMode)
	{
		pComboBox->NotifySite(CBN_CLOSEUP);

	}
	else
	{
		SetSelected(0, TRUE);

		pComboBox->UpdatePopupSelection();
		pComboBox->NotifySite(CBN_DROPDOWN);

	}

	return TRUE;
}

BOOL CXTPControlComboBoxGalleryPopupBar::OnHookKeyDown(UINT nChar, LPARAM lParam)
{
	ASSERT(m_pControlPopup);
	CXTPControlComboBox* pComboBox = ((CXTPControlComboBox*)m_pControlPopup);
	if (!pComboBox)
		return CXTPCommandBar::OnHookKeyDown(nChar, lParam);

	if (pComboBox->GetEditCtrl() && pComboBox->GetEditCtrl()->IsImeMode())
		return ProcessHookKeyDown(pComboBox, nChar, lParam);

	if (nChar == VK_ESCAPE)
	{
		if (pComboBox->IsFocused())
			return FALSE;

		return CXTPCommandBar::OnHookKeyDown(nChar, lParam);
	}

	if (nChar == VK_TAB) return FALSE;

	if (nChar == VK_RETURN)
	{
		if (GetControl(m_nSelected) == GetGalleryItem())
		{
			m_pControlPopup->OnExecute();
			return TRUE;
		}
	}

	return ProcessHookKeyDown(pComboBox, nChar, lParam);
}
