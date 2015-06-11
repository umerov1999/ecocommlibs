// XTPPropertyGridDelphiTheme.cpp : implementation of the CXTPPropertyGridDelphiTheme class.
//
// This file is a part of the XTREME PROPERTYGRID MFC class library.
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
#include <Common/XTPImageManager.h>
#include <Common/XTPResourceImage.h>
#include <Common/XTPMarkupRender.h>
#include <Common/XTPColorManager.h>
#include <Common/XTPWinThemeWrapper.h>
#include <Common/XTPSystemHelpers.h>
#include <Common/ScrollBar/XTPScrollBase.h>
#include <Common/ScrollBar/XTPScrollBarCtrl.h>
#include <Common/ScrollBar/XTPScrollBarContainer.h>

#ifdef _XTP_INCLUDE_CONTROLS
#include <Controls/Util/XTPControlTheme.h>
#endif

#include <PropertyGrid/XTPPropertyGridDefines.h>
#include <PropertyGrid/XTPPropertyGridPaintManager.h>
#include <PropertyGrid/XTPPropertyGridInplaceButton.h>
#include <PropertyGrid/XTPPropertyGridItem.h>
#include <PropertyGrid/XTPPropertyGridToolBar.h>
#include <PropertyGrid/XTPPropertyGrid.h>
#include <PropertyGrid/XTPPropertyGridToolTip.h>
#include <PropertyGrid/XTPPropertyGridView.h>
#include <PropertyGrid/XTPPropertyGridItemBool.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// CXTPPropertyGridDelphiTheme

CXTPPropertyGridDelphiTheme::CXTPPropertyGridDelphiTheme(CXTPPropertyGrid* pGrid)
	: CXTPPropertyGridPaintManager(pGrid)
{
}


void CXTPPropertyGridDelphiTheme::DrawItem(PDRAWITEMSTRUCT lpDrawItemStruct)
{
	CXTPPropertyGridView* pView = &m_pGrid->GetGridView();
	CXTPPropertyGridItem* pItem = (CXTPPropertyGridItem*)lpDrawItemStruct->itemData;

	ASSERT(pItem != NULL);
	if (!pItem)
		return;

	CXTPWindowRect rcWindow(pView);
	CRect rc = lpDrawItemStruct->rcItem;

	CXTPBufferDC dc(lpDrawItemStruct->hDC, rc);
	CXTPFontDC font(&dc, GetItemFont(pItem, FALSE));

	dc.FillSolidRect(rc, GetItemBackColor(pItem, FALSE));
	dc.SetBkMode(TRANSPARENT);


	CRect rcCaption(rc.left, rc.top, rc.left + pView->GetDividerPos(), rc.bottom);

	COLORREF clrFore = m_pMetrics->m_clrFore;
	COLORREF clrShadow = GetXtremeColor(COLOR_3DSHADOW);
	COLORREF clrLight = GetXtremeColor(COLOR_BTNHIGHLIGHT);

	{
		if (!pItem->IsCategory())
		{
			CXTPFontDC fontValue(&dc, GetItemFont(pItem, TRUE));

			CRect rcValue(rcCaption.right, rc.top, rc.right, rc.bottom - 1);
			DrawItemValue(&dc, pItem, rcValue);
		}

		dc.SetTextColor(GetItemTextColor(pItem, FALSE));

		dc.Draw3dRect(rcCaption.right - 1, rc.top, 1, rc.Height(), clrShadow, clrShadow);
		dc.Draw3dRect(rcCaption.right, rc.top, 1, rc.Height(), clrLight, clrLight);

		BOOL bSelected = lpDrawItemStruct->itemState & ODS_SELECTED;
		BOOL bFocused = bSelected && CWnd::GetFocus() && ((CWnd::GetFocus() == pView) || (CWnd::GetFocus()->GetParent() == pView) || (CWnd::GetFocus()->GetOwner() == pView));
		if (bFocused || (bSelected && !m_pGrid->m_bHideSelection))
		{
			dc.Draw3dRect(rc, clrFore, clrLight);
		}
		else
		{
			for (int i = 0; i < rc.right; i += 2)
				dc.SetPixel(i, rc.bottom - 1, clrShadow);
		}


		DrawItemCaption(&dc, pItem, rcCaption);

		CRect rcValue(rcCaption.right, rc.top, rc.right, rc.bottom - 1);
		DrawInplaceButtons(&dc, lpDrawItemStruct, pItem, rcValue);
	}

	if (pItem->HasVisibleChilds())
	{
		CRgn rgn;
		rgn.CreateRectRgnIndirect(&rcCaption);
		dc.SelectClipRgn(&rgn);

		DrawExpandButton(dc, pItem, rcCaption);

		dc.SelectClipRgn(NULL);
	}
}

void CXTPPropertyGridDelphiTheme::RefreshMetrics()
{
	CXTPPropertyGridPaintManager::RefreshMetrics();

	m_pMetrics->m_clrBack.SetStandardValue(GetXtremeColor(COLOR_3DFACE));
	m_pMetrics->m_clrFore.SetStandardValue(RGB(0, 0, 128));

}
