// XTPReportThemeOffice2003.cpp : implementation of the CXTPReportPaintManager class.
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

#include <ReportControl/Themes/XTPReportThemeOffice2003.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// class: CXTPReportThemeOffice2003
/////////////////////////////////////////////////////////////////////////////

CXTPReportThemeOffice2003::CXTPReportThemeOffice2003()
{
	m_columnStyle = xtpReportColumnShaded;
}

void CXTPReportThemeOffice2003::DrawColumnBackground(CDC* pDC, CXTPReportColumn* pColumn, CRect rcColumn, BOOL& /*bColumnPressed*/, BOOL& /*bDraggingFromHeader*/, CXTPReportHeader* pHeader)
{
	CXTPReportColumn* pPrevColumn = pHeader ? pHeader->GetNextVisibleColumn(pColumn->GetIndex(), -1) : NULL;

	int nShadowWidth = 4;

	if (pColumn->GetDrawHeaderDivider())
		DrawVerticalLine(pDC, rcColumn.right - 1, rcColumn.top + 2, rcColumn.Height() - 2 - nShadowWidth, m_clrControlDark);

	DrawHorizontalLine(pDC, rcColumn.left, rcColumn.bottom - 3, rcColumn.Width(), MixColor(m_clrHeaderControl, m_clrControlDark, 0.25));
	DrawHorizontalLine(pDC, rcColumn.left, rcColumn.bottom - 2, rcColumn.Width(), MixColor(m_clrHeaderControl, m_clrControlDark, 0.4));
	DrawHorizontalLine(pDC, rcColumn.left, rcColumn.bottom - 1, rcColumn.Width(), MixColor(m_clrHeaderControl, m_clrControlDark, 0.6));

	if (!pPrevColumn || pPrevColumn->GetDrawHeaderDivider())
		DrawVerticalLine(pDC, rcColumn.left, rcColumn.top + 2, rcColumn.Height() - 2 - nShadowWidth, m_clrControlLightLight);
}

void CXTPReportThemeOffice2003::DrawColumn(CDC *pDC, CXTPReportColumn *pColumn, CXTPReportHeader *pHeader, CRect rcColumn, BOOL bDrawExternal, int /*nShadowWidth*//*= 0*/)
{
	// save column parameter for future use in drawing
	BOOL bDraggingFromHeader = bDrawExternal
		&& pColumn->IsDragging()
		&& rcColumn.left == 0
		&& rcColumn.top == 0
		&& pHeader->IsDragHeader();

	BOOL bPlainColumn = bDrawExternal
		&& !bDraggingFromHeader;

	CXTPReportPaintManager::DrawColumn(pDC, pColumn, pHeader, rcColumn, bDrawExternal, !bPlainColumn ? 4 : 0);
}

void CXTPReportThemeOffice2003::DrawSectionDivider(
	CDC *pDC, const CRect& rc,
	XTPReportSectionDividerPosition dividerPosition,
	XTPReportSectionDividerStyle    dividerStyle,
	CXTPPaintManagerColor           clrDivider)
{
	if (dividerStyle == xtpReportFixedRowsDividerOutlook)
	{
		pDC->FillSolidRect(rc, m_clrHeaderControl);
		DrawHorizontalLine(pDC, rc.left, rc.bottom - 2, rc.Width(), MixColor(m_clrHeaderControl, m_clrControlDark, 0.4));
		DrawHorizontalLine(pDC, rc.left, rc.bottom - 1, rc.Width(), MixColor(m_clrHeaderControl, m_clrControlDark, 0.6));
	}
	else
	{
		CXTPReportPaintManager::DrawSectionDivider(pDC, rc, dividerPosition, dividerStyle, clrDivider);
	}
}
