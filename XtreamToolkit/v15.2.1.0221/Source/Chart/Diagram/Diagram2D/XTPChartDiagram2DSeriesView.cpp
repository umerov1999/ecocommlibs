// XTPChartDiagram2DSeriesView.cpp
//
// This file is a part of the XTREME TOOLKIT PRO MFC class library.
// (c)1998-2012 Codejock Software, All Rights Reserved.
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

#include "stdafx.h"


#include "../../Types/XTPChartTypes.h"
#include "../../XTPChartDefines.h"
#include "../../XTPChartElement.h"
#include "../../XTPChartElementView.h"
#include "../../XTPChartPanel.h"
#include "../../XTPChartSeriesView.h"
#include "../../XTPChartSeries.h"
#include "../../XTPChartSeriesPoint.h"
#include "../../XTPChartDiagram.h"
#include "../../XTPChartDiagramView.h"

#include "XTPChartDiagram2DView.h"
#include "XTPChartDiagram2DPane.h"

#include "XTPChartDiagram2DSeriesView.h"

//////////////////////////////////////////////////////////////////////////
// CXTPChartDiagram2DSeriesView

CXTPChartDiagram2DSeriesView::CXTPChartDiagram2DSeriesView(CXTPChartSeries* pSeries, CXTPChartDiagramView* pDiagramView, BOOL bSortPoints)
	: CXTPChartSeriesView(pSeries, pDiagramView)
{
	m_bSortPoints = bSortPoints;
	m_pAxisViewX = m_pAxisViewY = NULL;
}

CXTPChartPointF CXTPChartDiagram2DSeriesView::GetScreenPoint(double x, double y) const
{
	CXTPChartDiagram2DView* pDiagramView = (CXTPChartDiagram2DView*)GetDiagramView();

	CXTPChartDiagram2DPaneView* pPaneView = pDiagramView->GetPaneView();

	return pPaneView->GetScreenPoint(this, x, y);
}

int _cdecl CXTPChartDiagram2DSeriesView::ComparePoints(const CXTPChartSeriesPointView** ppPoint1, const CXTPChartSeriesPointView** ppPoint2)
{
	double d = (*ppPoint1)->GetPoint()->GetInternalArgumentValue() - (*ppPoint2)->GetPoint()->GetInternalArgumentValue();

	if (d > 0)
		return 1;
	if (d < 0)
		return -1;
	return 0;
}

void CXTPChartDiagram2DSeriesView::BeforeUpdateRange(CXTPChartDeviceContext* /*pDC*/)
{

}

void CXTPChartDiagram2DSeriesView::AfterUpdateRange(CXTPChartDeviceContext* /*pDC*/)
{
	if (m_bSortPoints)
	{
		typedef int (_cdecl *GENERICCOMPAREFUNC)(const void *, const void*);

		CXTPChartElementView** pChildren = m_pPointsView->GetChildren();

		qsort(pChildren, (size_t) m_pPointsView->GetCount(), sizeof(CXTPChartSeriesPointView*), (GENERICCOMPAREFUNC)ComparePoints);
	}
}
