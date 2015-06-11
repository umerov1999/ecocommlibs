// XTPPropertyGridVisualStudio2005Theme.cpp : implementation of the CXTPPropertyGridVisualStudio2005Theme class.
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

#ifdef _XTP_INCLUDE_CONTROLS
#include <Controls/Util/XTPControlTheme.h>
#endif

#include <PropertyGrid/XTPPropertyGridDefines.h>
#include <PropertyGrid/XTPPropertyGridPaintManager.h>
#include <PropertyGrid/XTPPropertyGridInplaceButton.h>
#include <PropertyGrid/XTPPropertyGridItem.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////////
// CXTPPropertyGridVisualStudio2005Theme

CXTPPropertyGridVisualStudio2005Theme::CXTPPropertyGridVisualStudio2005Theme(CXTPPropertyGrid* pGrid)
	: CXTPPropertyGridPaintManager(pGrid)
{
	m_buttonsStyle = xtpGridButtonsThemed;
}

void CXTPPropertyGridVisualStudio2005Theme::DrawCategoryCaptionBackground(CDC* pDC, CXTPPropertyGridItem* /*pItem*/, CRect rc)
{
	pDC->FillSolidRect(rc, m_pMetrics->m_clrLine);

	XTPDrawHelpers()->GradientFill(pDC,
		CRect(rc.left, rc.bottom - 2, rc.right, rc.bottom - 1), m_pMetrics->m_clrLine, GetXtremeColor(COLOR_3DHIGHLIGHT), TRUE);

}

void CXTPPropertyGridVisualStudio2005Theme::RefreshMetrics()
{
	CXTPPropertyGridPaintManager::RefreshMetrics();

	m_pMetrics->m_clrLine.SetStandardValue(GetXtremeColor(COLOR_3DFACE));
	m_pMetrics->m_clrCategoryFore.SetStandardValue(GetXtremeColor(COLOR_BTNTEXT));
	m_clrShadow.SetStandardValue(GetXtremeColor(XPCOLOR_STATICFRAME));
}

void CXTPPropertyGridVisualStudio2005Theme::DrawExpandButton(CDC& dc, CXTPPropertyGridItem* pItem, CRect rcCaption)
{
	if (!pItem->IsExpandable())
		return;

	CRect rcButton(CPoint(XTP_PGI_EXPAND_BORDER / 2 - 5, rcCaption.CenterPoint().y - 4), CSize(9, 9));

	if (pItem->GetIndent() > 0)
		rcButton.OffsetRect((pItem->GetIndent() - (pItem->IsCategory() ? 0 : 1)) * XTP_PGI_EXPAND_BORDER, 0);

	CDC* pDC = &dc;

	COLORREF clrButton = GetXtremeColor(COLOR_3DFACE);
	COLORREF clrFrame = GetXtremeColor(COLOR_3DSHADOW);
	pDC->FillSolidRect(rcButton.left + 1, rcButton.top + 1, 7, 7, clrButton);
	pDC->FillSolidRect(rcButton.left + 1, rcButton.top, 9 - 2, 1, clrFrame);
	pDC->FillSolidRect(rcButton.left + 1, rcButton.bottom - 1, 9 - 2, 1, clrFrame);
	pDC->FillSolidRect(rcButton.left, rcButton.top + 1, 1, 9 - 2, clrFrame);
	pDC->FillSolidRect(rcButton.right - 1, rcButton.top + 1, 1, 9 - 2, clrFrame);
	pDC->FillSolidRect(rcButton.left + 1, rcButton.top + 1, 9 - 2, 3, 0xFFFFFF);
	pDC->FillSolidRect(rcButton.left + 1, rcButton.top + 4, 9 - 4, 2, 0xFFFFFF);

	pDC->FillSolidRect(rcButton.left + 2, rcButton.top + 4, 9 - 4, 1, 0);
	if (!pItem->IsExpanded())
		pDC->FillSolidRect(rcButton.left + 4, rcButton.top + 2, 1, 9 - 4, 0);
}
