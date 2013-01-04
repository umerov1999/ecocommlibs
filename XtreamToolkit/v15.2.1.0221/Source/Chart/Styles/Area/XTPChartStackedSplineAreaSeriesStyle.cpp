// XTPChartStackedSplineAreaSeriesStyle.cpp
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

#include "Common/XTPPropExchange.h"

#include "../../Types/XTPChartTypes.h"
#include "Common/Base/Types/XTPPoint3.h"

#include "../../XTPChartDefines.h"
#include "../../XTPChartElement.h"
#include "../../XTPChartElementView.h"
#include "../../XTPChartSeries.h"
#include "../../XTPChartSeriesPoint.h"
#include "../../XTPChartSeriesStyle.h"
#include "../../XTPChartSeriesView.h"

#include "../../Appearance/XTPChartBorder.h"
#include "../../Appearance/XTPChartFillStyle.h"


#include "../../Diagram/Diagram2D/XTPChartDiagram2DSeriesStyle.h"
#include "../../Diagram/Diagram2D/XTPChartDiagram2DSeriesView.h"

#include "../Point/XTPChartPointSeriesStyle.h"
#include "../Point/XTPChartPointSeriesView.h"

#include "../../Drawing/XTPChartDeviceCommand.h"
#include "../../Drawing/XTPChartLineDeviceCommand.h"

#include "XTPChartAreaSeriesStyle.h"
#include "XTPChartAreaSeriesView.h"
#include "XTPChartStackedAreaSeriesStyle.h"
#include "XTPChartStackedSplineAreaSeriesStyle.h"


//////////////////////////////////////////////////////////////////////////
// CXTPChartStackedSplineAreaSeriesStyle

IMPLEMENT_SERIAL(CXTPChartStackedSplineAreaSeriesStyle, CXTPChartStackedAreaSeriesStyle, VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT);

CXTPChartStackedSplineAreaSeriesStyle::CXTPChartStackedSplineAreaSeriesStyle()
{
	m_nTransparency = 135;
	m_dStackHeight = 0;
}

CXTPChartStackedSplineAreaSeriesStyle::~CXTPChartStackedSplineAreaSeriesStyle()
{
}

CXTPChartSeriesView* CXTPChartStackedSplineAreaSeriesStyle::CreateView(CXTPChartSeries* pSeries, CXTPChartDiagramView* pDiagramView)
{
	return new CXTPChartStackedSplineAreaSeriesView(pSeries, pDiagramView);
}


//////////////////////////////////////////////////////////////////////////
// CXTPChartStackedSplineAreaSeriesView

CXTPChartStackedSplineAreaSeriesView::CXTPChartStackedSplineAreaSeriesView(CXTPChartSeries* pSeries, CXTPChartDiagramView* pDiagramView)
	: CXTPChartStackedAreaSeriesView(pSeries,pDiagramView)
{

}

CXTPChartDeviceCommand* CXTPChartStackedSplineAreaSeriesView::CreateDeviceCommand(CXTPChartDeviceContext* pDC)
{
	CXTPChartDeviceCommand* pSeriesCommand = new CXTPChartHitTestElementCommand(m_pSeries);
	CXTPChartDeviceCommand* pCommand = pSeriesCommand->AddChildCommand(new CXTPChartPolygonAntialiasingDeviceCommand());

	CXTPChartStackedSplineAreaSeriesStyle* pStyle = (CXTPChartStackedSplineAreaSeriesStyle*)GetStyle();

	CXTPChartColor clrBorder = GetBorderActualColor();


	int nCount = m_pPointsView->GetCount();
	if (nCount > 1)
	{
		CXTPChartStackedAreaSeriesPointView* pPointView;

		CXTPChartPoints arrPoints;

		int i;

		for (i = 0; i < nCount; i++)
		{
			pPointView =  (CXTPChartStackedAreaSeriesPointView*)m_pPointsView->GetAt(i);

			CXTPChartPointF point = GetScreenPoint(pPointView->GetPoint()->GetInternalArgumentValue(),
				pPointView->m_dValueTo);

			arrPoints.Add(point);
		}

		if (pStyle->GetBorder()->IsVisible())
			pCommand->AddChildCommand(new CXTPChartSolidSplineDeviceCommand(arrPoints, clrBorder, pStyle->GetBorder()->GetThickness()));

		for (i = nCount - 1; i >= 0; i--)
		{
			pPointView =  (CXTPChartStackedAreaSeriesPointView*)m_pPointsView->GetAt(i);

			CXTPChartPointF point = GetScreenPoint(pPointView->GetPoint()->GetInternalArgumentValue(),
				pPointView->m_dValueFrom);

			arrPoints.Add(point);
		}

		pCommand->AddChildCommand(pStyle->GetFillStyle()->CreateSplineDeviceCommand(arrPoints, GetActualColor(), GetActualColor2(), TRUE));


	}

	pCommand->AddChildCommand(CXTPChartSeriesView::CreateDeviceCommand(pDC));

	return pSeriesCommand;
}

#ifdef _XTP_ACTIVEX


BEGIN_DISPATCH_MAP(CXTPChartStackedSplineAreaSeriesStyle, CXTPChartStackedAreaSeriesStyle)
END_DISPATCH_MAP()


// {7E7BCC77-27BF-4cb1-9ABF-4558D9835223}
static const GUID IID_IChartStackedSplineAreaSeriesStyle =
{ 0x7e7bcc77, 0x27bf, 0x4cb1, { 0x9a, 0xbf, 0x45, 0x58, 0xd9, 0x83, 0x98, 0x76 } };

BEGIN_INTERFACE_MAP(CXTPChartStackedSplineAreaSeriesStyle, CXTPChartStackedAreaSeriesStyle)
	INTERFACE_PART(CXTPChartStackedSplineAreaSeriesStyle, IID_IChartStackedSplineAreaSeriesStyle, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartStackedSplineAreaSeriesStyle, IID_IChartStackedSplineAreaSeriesStyle)

// {7E790E6F-92B6-4671-9613-6B2A0FBF80A8}
IMPLEMENT_OLECREATE_EX2(CXTPChartStackedSplineAreaSeriesStyle, "Codejock.ChartStackedSplineAreaSeriesStyle." _XTP_AXLIB_VERSION,
0x7e790e6f, 0x92b6, 0x4671, 0x96, 0x13, 0x6b, 0x2a, 0xf, 0xbf, 0x54, 0x32);

#endif
