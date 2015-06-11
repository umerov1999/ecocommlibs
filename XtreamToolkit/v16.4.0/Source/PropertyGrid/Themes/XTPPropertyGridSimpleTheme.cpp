// XTPPropertyGridSimpleTheme.cpp : implementation of the CXTPPropertyGridSimpleTheme class.
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
// CXTPPropertyGridSimpleTheme

CXTPPropertyGridSimpleTheme::CXTPPropertyGridSimpleTheme(CXTPPropertyGrid* pGrid)
	: CXTPPropertyGridPaintManager(pGrid)
{
}

void CXTPPropertyGridSimpleTheme::RefreshMetrics()
{
	CXTPPropertyGridPaintManager::RefreshMetrics();

	m_pMetrics->m_clrLine.SetStandardValue(GetXtremeColor(COLOR_3DFACE));
}



void CXTPPropertyGridSimpleTheme::DrawItem(PDRAWITEMSTRUCT lpDrawItemStruct)
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

	COLORREF clrLine = m_pMetrics->m_clrLine;

	{
		if (!pItem->IsCategory())
		{
			CXTPFontDC fontValue(&dc, GetItemFont(pItem, TRUE));
			CRect rcValue(rcCaption.right, rc.top, rc.right, rc.bottom - 1);
			DrawItemValue(&dc, pItem, rcValue);
		}

		dc.SetTextColor(GetItemTextColor(pItem, FALSE));

		BOOL bSelected = lpDrawItemStruct->itemState & ODS_SELECTED;
		BOOL bFocused = bSelected && CWnd::GetFocus() && ((CWnd::GetFocus() == pView) || (CWnd::GetFocus()->GetParent() == pView) || (CWnd::GetFocus()->GetOwner() == pView));
		if (bFocused)
		{
			dc.SetTextColor(m_clrHighlightText);
			dc.FillSolidRect(CRect(XTP_PGI_EXPAND_BORDER, rcCaption.top, rcCaption.right, rcCaption.bottom), m_clrHighlight);
		}
		else if (bSelected && !m_pGrid->m_bHideSelection)
		{
			dc.FillSolidRect(CRect(XTP_PGI_EXPAND_BORDER, rcCaption.top, rcCaption.right, rcCaption.bottom), clrLine);
		}
		dc.FillSolidRect(XTP_PGI_EXPAND_BORDER - 1, rc.top, 1, rc.Height(), clrLine);

		CXTPPenDC pen(dc, clrLine);
		dc.MoveTo(XTP_PGI_EXPAND_BORDER, rc.bottom - 1);
		dc.LineTo(rc.right, rc.bottom - 1);
		dc.MoveTo(rcCaption.right, rc.top);
		dc.LineTo(rcCaption.right, rc.bottom - 1);


		DrawItemCaption(&dc, pItem, rcCaption);

		CRect rcValue(rcCaption.right, rc.top, rc.right, rc.bottom - 1);

		DrawInplaceButtons(&dc, lpDrawItemStruct, pItem, rcValue);

	}

	if (pView->GetCount() == (int)lpDrawItemStruct->itemID + 1)
		dc.FillSolidRect(0, rc.bottom - 1, rc.Width(), 1, clrLine);

	if (pItem->HasVisibleChilds())
	{
		CRgn rgn;
		rgn.CreateRectRgnIndirect(&rcCaption);
		dc.SelectClipRgn(&rgn);

		DrawExpandButton(dc, pItem, rcCaption);

		dc.SelectClipRgn(NULL);
	}
}
