// XTPListBoxThemeResource.cpp : implementation of the CXTPListBoxThemeResource class.
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
#include <Controls/ListBox/Themes/XTPListBoxThemeResource.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


void CXTPListBoxThemeResource::DrawItem(CDC *pDC, CXTPListBox *pListBox, LPDRAWITEMSTRUCT lpDIS)
{
	CRect rcItem = lpDIS->rcItem;

	BOOL bSelected = ((lpDIS->itemState & ODS_SELECTED) == ODS_SELECTED);

	BOOL bHasFocus = pListBox->HasFocus();
	BOOL bHighlighted = (int)lpDIS->itemID == pListBox->m_nHotItem;

	if ((lpDIS->itemID != (UINT)-1) && (lpDIS->itemAction & (ODA_DRAWENTIRE | ODA_SELECT)))
	{
		COLORREF clrWindow = pListBox->GetBackColor();
		COLORREF clrWindowText = pListBox->IsWindowEnabled() ? GetXtremeColor(COLOR_WINDOWTEXT) : GetXtremeColor(COLOR_GRAYTEXT);
		CRect rcText(rcItem);
		rcText.DeflateRect(pListBox->m_nTextPadding, 0);

		CXTPResourceImages* pImages = XTPResourceImages();


		if ((bSelected || bHighlighted) && pListBox->IsWindowEnabled())
		{
			CXTPResourceImage* pImage = pImages->LoadFile(_T("LISTBOX"));
			if (pImage)
			{
				pImage->DrawImage(pDC, rcItem, pImage->GetSource(bSelected && bHighlighted ? 2 :
					bHasFocus && bSelected ? 1 : !bHasFocus && bSelected ? 3 : 0, 4), CRect(4, 4, 4, 4), COLORREF_NULL);
				clrWindow = COLORREF_NULL;
			}
		}

		pDC->SetBkMode(TRANSPARENT);
		clrWindowText = pImages->GetImageColor(_T("LISTBOX"), _T("NormalText"));

		// set the text and text background colors, then repaint the item.
		pDC->SetBkColor(clrWindow);
		pDC->SetTextColor(clrWindowText);
		if (clrWindow != COLORREF_NULL)
			pDC->FillSolidRect(&rcItem, clrWindow);

		DrawItemText(pDC, rcText, pListBox, lpDIS);
	}
}

void CXTPListBoxThemeResource::RefreshMetrics()
{

}
