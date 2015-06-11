// XTPReportThemeOffice2003Luna.cpp : implementation of the CXTPReportPaintManager class.
//
// This file is a part of the XTREME REPORTCONTROL MFC class library.
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

#include <Common/XTPResourceManager.h>
#include <Common/XTPDrawHelpers.h>
#include <Common/XTPImageManager.h>
#include <Common/XTPResourceImage.h>
#include <Common/XTPMarkupRender.h>
#include <Common/XTPColorManager.h>
#include <Common/XTPWinThemeWrapper.h>
#include <Common/XTPCustomHeap.h>
#include <Common/XTPSystemHelpers.h>
#include <Common/XTPSmartPtrInternalT.h>

#include <ReportControl/Resource.h>
#include <ReportControl/XTPReportDefines.h>
#include <ReportControl/XTPReportPaintManager.h>
#include <ReportControl/XTPReportRow.h>
#include <ReportControl/XTPReportColumn.h>
#include <ReportControl/XTPReportHeader.h>
#include <ReportControl/XTPReportControl.h>

#include <ReportControl/Themes/XTPReportThemeOffice2003Luna.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// class: CXTPReportThemeOffice2003Luna
/////////////////////////////////////////////////////////////////////////////

CXTPReportThemeOffice2003Luna::CXTPReportThemeOffice2003Luna()
{
	m_columnStyle = xtpReportColumnOffice2003;
}

void CXTPReportThemeOffice2003Luna::RefreshMetrics()
{
	CXTPReportPaintManager::RefreshMetrics();

	if (!XTPColorManager()->IsLunaColorsDisabled())
	{
		XTPCurrentSystemTheme systemTheme = XTPColorManager()->GetCurrentSystemTheme();

		switch (systemTheme)
		{
		case xtpSystemThemeBlue:
		case xtpSystemThemeRoyale:
		case xtpSystemThemeAero:
			m_clrGroupShadeBorder.SetStandardValue(RGB(123, 164, 224));
			m_clrIndentControl.SetStandardValue(RGB(253, 238, 201));
			m_clrGroupShadeBack.SetStandardValue(RGB(190, 218, 251));
			m_clrGroupRowText.SetStandardValue(RGB(55, 104, 185));
			SetGridColor(RGB(234, 233, 225));
			break;

		case xtpSystemThemeOlive:
			m_clrGroupShadeBorder.SetStandardValue(RGB(181, 196, 143));
			m_clrIndentControl.SetStandardValue(RGB(253, 238, 201));
			m_clrGroupShadeBack.SetStandardValue(RGB(175, 186, 145));
			m_clrGroupRowText.SetStandardValue(RGB(115, 137, 84));
			SetGridColor(RGB(234, 233, 225));
			break;

		case xtpSystemThemeSilver:
			m_clrGroupShadeBorder.SetStandardValue(RGB(165, 164, 189));
			m_clrIndentControl.SetStandardValue(RGB(253, 238, 201));
			m_clrGroupShadeBack.SetStandardValue(RGB(229, 229, 235));
			m_clrGroupRowText.SetStandardValue(RGB(112, 111, 145));
			SetGridColor(RGB(229, 229, 235));
			break;
		}
	}

	m_grcGradientColumn.SetStandardValue(XTPColorManager()->LightColor(GetXtremeColor(COLOR_3DFACE), GetXtremeColor(COLOR_WINDOW), 0xcd), GetXtremeColor(COLOR_3DFACE));
	m_grcGradientColumnHot.SetStandardValue(GetXtremeColor(XPCOLOR_HIGHLIGHT));
	m_grcGradientColumnPushed.SetStandardValue(GetXtremeColor(XPCOLOR_HIGHLIGHT_PUSHED));
	m_clrGradientColumnShadow.SetStandardValue(GetXtremeColor(XPCOLOR_TOOLBAR_FACE));
	m_clrGradientColumnSeparator.SetStandardValue(GetXtremeColor(XPCOLOR_SEPARATOR));

	switch (XTPColorManager()->GetCurrentSystemTheme())
	{
	case xtpSystemThemeBlue:
	case xtpSystemThemeRoyale:
	case xtpSystemThemeAero:
		m_grcGradientColumn.SetStandardValue(RGB(221, 236, 254), RGB(129, 169, 226));
		m_clrGradientColumnShadow.SetStandardValue(RGB(59, 97, 156));
		m_clrGradientColumnSeparator.SetStandardValue(RGB(106, 140, 203));
		m_grcGradientColumnPushed.SetStandardValue(RGB(254, 142, 75), RGB(255, 207, 139));
		m_grcGradientColumnHot.SetStandardValue(RGB(255, 242, 200), RGB(255, 212, 151));
		break;

	case xtpSystemThemeOlive:
		m_grcGradientColumn.SetStandardValue(RGB(244, 247, 222), RGB(183, 198, 145));
		m_clrGradientColumnShadow.SetStandardValue(RGB(96, 128, 88));
		m_clrGradientColumnSeparator.SetStandardValue(RGB(96, 128, 88));
		m_grcGradientColumnPushed.SetStandardValue(RGB(254, 142, 75), RGB(255, 207, 139));
		m_grcGradientColumnHot.SetStandardValue(RGB(255, 242, 200), RGB(255, 212, 151));
		break;

	case xtpSystemThemeSilver:
		m_grcGradientColumn.SetStandardValue(RGB(243, 244, 250), RGB(153, 151, 181));
		m_clrGradientColumnShadow.SetStandardValue(RGB(124, 124, 148));
		m_clrGradientColumnSeparator.SetStandardValue(RGB(110, 109, 143));
		m_grcGradientColumnPushed.SetStandardValue(RGB(254, 142, 75), RGB(255, 207, 139));
		m_grcGradientColumnHot.SetStandardValue(RGB(255, 242, 200), RGB(255, 212, 151));
		break;
	}
}

void CXTPReportThemeOffice2003Luna::FillFooter(CDC* pDC, CRect rcFooter)
{
	XTPDrawHelpers()->GradientFill(pDC, rcFooter,
		m_grcGradientColumn.clrDark, m_grcGradientColumn.clrLight, FALSE);

	DrawHorizontalLine(pDC, rcFooter.left, rcFooter.top, rcFooter.Width(), m_clrControlBack);
	DrawHorizontalLine(pDC, rcFooter.left, rcFooter.top + 1, rcFooter.Width(), m_clrGradientColumnShadow);
	DrawHorizontalLine(pDC, rcFooter.left, rcFooter.top + 2, rcFooter.Width(), MixColor(m_grcGradientColumn.clrDark, m_clrGradientColumnShadow, 0.25));
	DrawHorizontalLine(pDC, rcFooter.left, rcFooter.top + 3, rcFooter.Width(), MixColor(m_grcGradientColumn.clrDark, m_clrGradientColumnShadow, 0.1));

	DrawHorizontalLine(pDC, rcFooter.left, rcFooter.bottom - 1, rcFooter.Width(), m_clrGradientColumnShadow);
}

BOOL CXTPReportThemeOffice2003Luna::IsColumHotTrackingEnabled() const
{
	return m_bHotTracking;
}

void CXTPReportThemeOffice2003Luna::DrawColumnBackground(CDC* pDC, CXTPReportColumn* pColumn, CRect rcColumn, BOOL& bColumnPressed, BOOL& /*bDraggingFromHeader*/, CXTPReportHeader* pHeader)
{
	CXTPReportColumn* pPrevColumn = pHeader ? pHeader->GetNextVisibleColumn(pColumn->GetIndex(), -1) : NULL;

	XTPDrawHelpers()->GradientFill(pDC, rcColumn,
		bColumnPressed ? m_grcGradientColumnPushed: pColumn->IsHotTracking() ? m_grcGradientColumnHot : m_grcGradientColumn, FALSE);

	DrawHorizontalLine(pDC, rcColumn.left, rcColumn.bottom - 1, rcColumn.Width(), m_clrGradientColumnShadow);

	if (pColumn->GetDrawHeaderDivider())
		DrawVerticalLine(pDC, rcColumn.right - 1, rcColumn.top + 2, rcColumn.Height() - 2 - 4, m_clrGradientColumnSeparator);

	if (!pPrevColumn || pPrevColumn->GetDrawHeaderDivider())
		DrawVerticalLine(pDC, rcColumn.left, rcColumn.top + 3, rcColumn.Height() - 2 - 4, m_clrControlLightLight);

	if (pColumn->GetControl()->IsGroupByVisible())
		DrawHorizontalLine(pDC, rcColumn.left, rcColumn.top, rcColumn.Width(), m_clrGradientColumnShadow);

	bColumnPressed = FALSE;
}

void CXTPReportThemeOffice2003Luna::DrawColumnFooterDivider(CDC* pDC, CXTPReportColumn* pColumn, CRect rcColumn)
{
	if (pColumn->GetDrawFooterDivider())
	{
		DrawVerticalLine(pDC, rcColumn.right, rcColumn.top + 6, rcColumn.Height() - 9, m_clrControlLightLight);
		DrawVerticalLine(pDC, rcColumn.right - 1, rcColumn.top + 5, rcColumn.Height() - 9, m_clrGradientColumnSeparator);
	}
}

void CXTPReportThemeOffice2003Luna::DrawSectionDivider(
	CDC *pDC, const CRect& rc,
	XTPReportSectionDividerPosition dividerPosition,
	XTPReportSectionDividerStyle    dividerStyle,
	CXTPPaintManagerColor           clrDivider)
{
	if (dividerStyle == xtpReportFixedRowsDividerOutlook)
	{
		XTPDrawHelpers()->GradientFill(pDC, rc,m_grcGradientColumn.clrLight, m_grcGradientColumn.clrDark,FALSE);
		DrawHorizontalLine(pDC, rc.left, rc.bottom - 1, rc.Width(), m_clrGradientColumnShadow);
	}
	else
	{
		CXTPReportPaintManager::DrawSectionDivider(pDC, rc, dividerPosition, dividerStyle, clrDivider);
	}
}