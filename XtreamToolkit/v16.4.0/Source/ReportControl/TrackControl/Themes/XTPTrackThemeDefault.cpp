// XTPTrackThemeDefault.cpp : implementation of the CXTPTrackPaintManager class.
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

#include <ReportControl/TrackControl/XTPTrackPaintManager.h>
#include <ReportControl/TrackControl/Themes/XTPTrackThemeDefault.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// class: CXTPTrackThemeDefault
/////////////////////////////////////////////////////////////////////////////

CXTPTrackThemeDefault::CXTPTrackThemeDefault()
{
	m_columnStyle = xtpReportColumnShaded;
}

void CXTPTrackThemeDefault::DrawColumnBackground(CDC* pDC, CXTPReportColumn* pColumn, CRect rcColumn, BOOL& /*bColumnPressed*/, BOOL& /*bDraggingFromHeader*/, CXTPReportHeader* pHeader)
{
	CXTPReportColumn* pPrevColumn = pHeader ? pHeader->GetNextVisibleColumn(pColumn->GetIndex(), -1) : NULL;

	pDC->DrawEdge(rcColumn, EDGE_RAISED, BF_RECT);

	if (!pColumn->GetDrawHeaderDivider())
	{
		DrawVerticalLine(pDC, rcColumn.right - 1, rcColumn.top + 1, rcColumn.Height() - 2, m_clrHeaderControl);
		pDC->SetPixel(rcColumn.right - 1, rcColumn.top, m_clrControlLightLight);
	}
	if (pPrevColumn && !pPrevColumn->GetDrawHeaderDivider())
		DrawVerticalLine(pDC, rcColumn.left, rcColumn.top + 1, rcColumn.Height() - 2, m_clrHeaderControl);
}
