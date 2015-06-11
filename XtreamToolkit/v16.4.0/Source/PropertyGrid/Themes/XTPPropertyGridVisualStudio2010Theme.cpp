// XTPPropertyGridVisualStudio2010Theme.cpp : implementation of the CXTPPropertyGridVisualStudio2010Theme class.
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


//////////////////////////////////////////////////////////////////////////
// CXTPPropertyGridVisualStudio2010Theme

CXTPPropertyGridVisualStudio2010Theme::CXTPPropertyGridVisualStudio2010Theme(CXTPPropertyGrid* pGrid)
	: CXTPPropertyGridPaintManager(pGrid)
{
	m_buttonsStyle = xtpGridButtonsThemed;

	m_themeTreeExplorer = new CXTPWinThemeWrapper();
}

CXTPPropertyGridVisualStudio2010Theme::~CXTPPropertyGridVisualStudio2010Theme()
{
	SAFE_DELETE(m_themeTreeExplorer);
}


void CXTPPropertyGridVisualStudio2010Theme::RefreshMetrics()
{
	CXTPPropertyGridPaintManager::RefreshMetrics();

	m_pMetrics->m_clrLine.SetStandardValue(RGB(240, 240, 240));
	m_pMetrics->m_clrCategoryFore.SetStandardValue(RGB(168, 179, 194));
	m_clrShadow.SetStandardValue(GetSysColor(COLOR_3DSHADOW));
	m_clrCategorySeparator.SetStandardValue(GetSysColor(COLOR_3DFACE));
	m_clrFace.SetStandardValue(RGB(222, 225, 231));
	m_pMetrics->m_clrHelpBack.SetStandardValue(RGB(222, 225, 231));
	m_pMetrics->m_clrHelpFore.SetStandardValue(0);

	HWND hWnd = m_pGrid->m_hWnd;
	if (hWnd && XTPSystemVersion()->IsWinVistaOrGreater())
	{
		CXTPWinThemeWrapper().SetWindowTheme(hWnd, L"EXPLORER", NULL);

		m_themeTreeExplorer->OpenThemeData(hWnd, L"TREEVIEW");

		CXTPWinThemeWrapper().SetWindowTheme(hWnd, NULL, NULL);
	}
	else
	{
		m_themeTreeExplorer->CloseTheme();
	}
}

void CXTPPropertyGridVisualStudio2010Theme::DrawExpandButton(CDC& dc, CXTPPropertyGridItem* pItem, CRect rcCaption)
{
	if (!pItem->IsExpandable())
		return;

	if (m_buttonsStyle == xtpGridButtonsThemed && m_themeTreeExplorer->IsAppThemed())
	{
		CRect rcSign(CPoint(XTP_PGI_EXPAND_BORDER / 2 - 9, rcCaption.CenterPoint().y - 8), CSize(16, 16));

		if (pItem->GetIndent() > 0)
			rcSign.OffsetRect((pItem->GetIndent() - (pItem->IsCategory() ? 0 : 1)) * XTP_PGI_EXPAND_BORDER, 0);

		m_themeTreeExplorer->DrawThemeBackground(dc, TVP_GLYPH, pItem->IsExpanded() ? GLPS_OPENED : GLPS_CLOSED, &rcSign, 0);

	}
	else
	{
		DrawNotThemedExpandButton(dc, pItem, rcCaption);
	}
}

void CXTPPropertyGridVisualStudio2010Theme::DrawNotThemedExpandButton(CDC& dc, CXTPPropertyGridItem* pItem, CRect rcCaption)
{
	if (!pItem->IsExpandable())
		return;

	CRect rcSign(CPoint(XTP_PGI_EXPAND_BORDER / 2 - 5, rcCaption.CenterPoint().y - 4), CSize(9, 9));

	if (pItem->GetIndent() > 0)
		rcSign.OffsetRect((pItem->GetIndent() - (pItem->IsCategory() ? 0 : 1)) * XTP_PGI_EXPAND_BORDER, 0);


	CPoint pt = rcSign.CenterPoint();

	if (!pItem->IsCategory()) dc.FillSolidRect(rcSign, m_pMetrics->m_clrBack);
	dc.Draw3dRect(rcSign, m_pMetrics->m_clrFore, m_pMetrics->m_clrFore);

	CXTPPenDC pen (dc, m_pMetrics->m_clrFore);

	dc.MoveTo(pt.x - 2, pt.y);
	dc.LineTo(pt.x + 3, pt.y);

	if (!pItem->IsExpanded())
	{
		dc.MoveTo(pt.x, pt.y - 2);
		dc.LineTo(pt.x, pt.y + 3);
	}
}
