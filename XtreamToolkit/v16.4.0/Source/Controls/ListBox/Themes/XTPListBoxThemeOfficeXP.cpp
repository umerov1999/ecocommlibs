// XTPListBoxThemeOfficeXP.cpp : implementation of the CXTPListBoxThemeOfficeXP class.
//
// This file is a part of the XTREME CONTROLS MFC class library.
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

#include <Common/XTPColorManager.h>
#include <Common/XTPDrawHelpers.h>
#include <Common/XTPResourceImage.h>

#include <Controls/Util/XTPControlTheme.h>
#include <Controls//Util/XTPGlobal.h>
#include <Controls/ListBox/XTPListBox.h>
#include <Controls/ListBox/Themes/XTPListBoxThemeOfficeXP.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


void CXTPListBoxThemeOfficeXP::DrawItem(CDC *pDC, CXTPListBox *pListBox, LPDRAWITEMSTRUCT lpDIS)
{
	CRect rcItem = lpDIS->rcItem;

	if ((lpDIS->itemID != (UINT)-1) && (lpDIS->itemAction & (ODA_DRAWENTIRE | ODA_SELECT)))
	{
		COLORREF clrWindow = pListBox->GetBackColor();
		COLORREF clrWindowText = pListBox->IsWindowEnabled() ? GetXtremeColor(COLOR_WINDOWTEXT) : GetXtremeColor(COLOR_GRAYTEXT);
		BOOL bSelected = ((lpDIS->itemState & ODS_SELECTED) != 0);
		CRect rcText(rcItem);
		rcText.DeflateRect(pListBox->m_nTextPadding, 0);

		if (bSelected)
		{
			clrWindow = pListBox->IsWindowEnabled() ? GetXtremeColor(COLOR_HIGHLIGHT) : GetXtremeColor(COLOR_GRAYTEXT);
			clrWindowText = GetXtremeColor(COLOR_HIGHLIGHTTEXT);
		}

		if (bSelected && pListBox->IsWindowEnabled())
		{
			clrWindowText = GetXtremeColor(XPCOLOR_HIGHLIGHT_TEXT);
			clrWindow = GetXtremeColor(XPCOLOR_HIGHLIGHT);
			pDC->Draw3dRect(rcItem, GetXtremeColor(XPCOLOR_HIGHLIGHT_BORDER), GetXtremeColor(XPCOLOR_HIGHLIGHT_BORDER));
			rcItem.DeflateRect(1, 1);
		}

		// set the text and text background colors, then repaint the item.
		pDC->SetBkColor(clrWindow);
		pDC->SetTextColor(clrWindowText);
		if (clrWindow != COLORREF_NULL)
			pDC->FillSolidRect(&rcItem, clrWindow);

		CString strText;
		pListBox->GetText(lpDIS->itemID, strText);

		pDC->DrawText(strText, &rcText, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_EXPANDTABS);
	}

	if ((lpDIS->itemAction & ODA_FOCUS))
		pDC->DrawFocusRect(&lpDIS->rcItem);
}

void CXTPListBoxThemeOfficeXP::RefreshMetrics()
{

}
