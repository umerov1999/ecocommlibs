// XTPShortcutBarPaintManager.cpp : implementation of the CXTPShortcutBarPaintManager class.
//
// This file is a part of the XTREME SHORTCUTBAR MFC class library.
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

#include <Common/XTPVC80Helpers.h>
#include <Common/XTPDrawHelpers.h>
#include <Common/XTPImageManager.h>
#include <Common/XTPResourceImage.h>
#include <Common/XTPSystemHelpers.h>
#include <Common/XTPResourceManager.h>
#include <Common/XTPColorManager.h>
#include <Common/XTPWinThemeWrapper.h>

#include <ShortcutBar/Resource.h>
#include <ShortcutBar/XTPShortcutBarPaintManager.h>
#include <ShortcutBar/XTPShortcutBar.h>
#include <ShortcutBar/XTPShortcutBarPane.h>

#include <ShortcutBar/Themes/XTPShortcutBarOffice2000Theme.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////////
// CXTPShortcutBarOffice2000Theme

void CXTPShortcutBarOffice2000Theme::FillShortcutBar(CDC* pDC, CXTPShortcutBar* pShortcutBar)
{
	CXTPClientRect rc(pShortcutBar);

	pDC->FillSolidRect(rc, GetXtremeColor(COLOR_3DSHADOW));

}

void CXTPShortcutBarOffice2000Theme::DrawShortcutBarFrame(CDC* pDC, CXTPShortcutBar* pShortcutBar)
{
	CXTPWindowRect rc(pShortcutBar);
	rc.OffsetRect(-rc.TopLeft());
	pDC->Draw3dRect(rc, GetXtremeColor(COLOR_3DSHADOW), GetXtremeColor(COLOR_3DHIGHLIGHT));
}


void CXTPShortcutBarOffice2000Theme::FillShortcutItemEntry(CDC* pDC, CXTPShortcutBarItem* pItem)
{
	CXTPShortcutBar* pShortcutBar = pItem->GetShortcutBar();
	CRect rc = pItem->GetItemRect();

	if (pItem->IsSelected() &&
		(pShortcutBar->GetHotItem() != pItem || (pShortcutBar->GetPressedItem() != NULL && pShortcutBar->GetPressedItem() != pItem)))
	{
		pDC->Draw3dRect(rc, GetXtremeColor(COLOR_BTNTEXT), GetXtremeColor(COLOR_3DHIGHLIGHT));
	}
	else if (pItem->IsSelected() ||
		(pShortcutBar->GetHotItem() == pItem && pShortcutBar->GetPressedItem() == pItem))
	{
		pDC->Draw3dRect(rc, GetXtremeColor(COLOR_BTNTEXT), GetXtremeColor(COLOR_3DHIGHLIGHT));
	}
	else if ((pShortcutBar->GetHotItem() == pItem && pShortcutBar->GetPressedItem() == NULL) ||
		pShortcutBar->GetPressedItem() == pItem)
	{
		pDC->Draw3dRect(rc, GetXtremeColor(COLOR_3DHIGHLIGHT), GetXtremeColor(COLOR_BTNTEXT));
	}

	pDC->SetTextColor(GetXtremeColor(COLOR_3DHIGHLIGHT));
}


int CXTPShortcutBarOffice2000Theme::DrawGripper(CDC* pDC, CXTPShortcutBar* pShortcutBar, BOOL bDraw)
{
	if (bDraw)
	{
		CRect rc = pShortcutBar->GetGripperRect();
		pDC->FillSolidRect(rc, GetXtremeColor(COLOR_3DFACE));
	}
	return 5;
}



void CXTPShortcutBarOffice2000Theme::DrawImage(CDC* pDC, CPoint pt, CSize sz, CXTPImageManagerIcon* pImage, BOOL bSelected, BOOL bPressed, BOOL bChecked, BOOL bEnabled)
{
	if (!bEnabled) pImage->Draw(pDC, pt, pImage->GetDisabledIcon(), sz);
	else if (bChecked) pImage->Draw(pDC, pt, bChecked && (bSelected || bPressed) ? pImage->GetHotIcon() : pImage->GetCheckedIcon(), sz);
	else if (!bSelected && !bPressed)
	{
		pImage->Draw(pDC, pt, pImage->GetIcon(), sz);
	}
	else if ((bSelected && !bPressed) || (!bSelected && bPressed))
	{
		pImage->Draw(pDC, pt, pImage->GetHotIcon(), sz);
	}
	else if ((bSelected && bPressed))
		pImage->Draw(pDC, CPoint(pt.x + 1, pt.y + 1), pImage->GetHotIcon(), sz);
}
