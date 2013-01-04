// XTPChartSplineSeriesStyle.cpp
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

#include "../../XTPChartDefines.h"
#include "../../XTPChartElement.h"
#include "../../XTPChartElementView.h"
#include "../../XTPChartSeries.h"
#include "../../XTPChartSeriesView.h"
#include "../../XTPChartSeriesStyle.h"

#include "../../Diagram/Diagram2D/XTPChartDiagram2DSeriesStyle.h"
#include "../../Diagram/Diagram2D/XTPChartDiagram2DSeriesView.h"

#include "../../Drawing/XTPChartDeviceCommand.h"
#include "../../Appearance/XTPChartLineStyle.h"
#include "../../Utils/XTPChartMathUtils.h"

#include "../Point/XTPChartPointSeriesStyle.h"
#include "../Point/XTPChartPointSeriesView.h"

#include "XTPChartLineSeriesStyle.h"
#include "XTPChartSplineSeriesStyle.h"


//////////////////////////////////////////////////////////////////////////
// CXTPChartSplineSeriesStyle

IMPLEMENT_SERIAL(CXTPChartSplineSeriesStyle, CXTPChartLineSeriesStyle, VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT);

CXTPChartSplineSeriesStyle::CXTPChartSplineSeriesStyle()
{
}

CXTPChartSplineSeriesStyle::~CXTPChartSplineSeriesStyle()
{
}

CXTPChartSeriesView* CXTPChartSplineSeriesStyle::CreateView(CXTPChartSeries* pSeries, CXTPChartDiagramView* pDiagramView)
{
	return new CXTPChartSplineSeriesView(pSeries, pDiagramView);
}


//////////////////////////////////////////////////////////////////////////
// CXTPChartSplineSeriesView

CXTPChartSplineSeriesView::CXTPChartSplineSeriesView(CXTPChartSeries* pSeries, CXTPChartDiagramView* pDiagramView)
	: CXTPChartLineSeriesView(pSeries,pDiagramView)
{

}
CXTPChartSeriesPointView* CXTPChartSplineSeriesView::CreateSeriesPointView(CXTPChartDeviceContext* pDC, CXTPChartSeriesPoint* pPoint, CXTPChartElementView* pParentView)
{
	UNREFERENCED_PARAMETER(pDC);
	return new CXTPChartPointSeriesPointView(pPoint, pParentView);
}

CXTPChartDeviceCommand* CXTPChartSplineSeriesView::CreateDeviceCommand(CXTPChartDeviceContext* pDC)
{
	CXTPChartDeviceCommand* pSeriesCommand = new CXTPChartHitTestElementCommand(m_pSeries);
	CXTPChartDeviceCommand* pCommand = pSeriesCommand->AddChildCommand(new CXTPChartPolygonAntialiasingDeviceCommand());

	CXTPChartSplineSeriesStyle* pStyle = (CXTPChartSplineSeriesStyle*)GetStyle();

	int nCount = m_pPointsView->GetCount();

	CXTPChartPoints points;

	for (int i = 0; i < nCount; i++)
	{
		CXTPChartPointSeriesPointView* pPointView =  (CXTPChartPointSeriesPointView*)m_pPointsView->GetAt(i);
		if (CXTPChartMathUtils::IsNan(pPointView->m_dInternalValue))
		{
			if (points.GetSize() > 1)
			{
				pCommand->AddChildCommand(pStyle->GetLineStyle()->CreateSplineDeviceCommand(points, GetColor()));
			}
			points.RemoveAll();
			continue;
		}

		CXTPChartPointF pointNext = pPointView->GetScreenPoint();
		points.Add(pointNext);
	}

	if (points.GetSize() > 1)
	{
		pCommand->AddChildCommand(pStyle->GetLineStyle()->CreateSplineDeviceCommand(points, GetColor()));
	}

	pCommand->AddChildCommand(CXTPChartSeriesView::CreateDeviceCommand(pDC));

	return pSeriesCommand;
}


#ifdef _XTP_ACTIVEX


BEGIN_DISPATCH_MAP(CXTPChartSplineSeriesStyle, CXTPChartPointSeriesStyle)
	DISP_PROPERTY_EX_ID(CXTPChartSplineSeriesStyle, "LineStyle", 100, OleGetLineStyle, SetNotSupported, VT_DISPATCH)
END_DISPATCH_MAP()


// {373BCC77-27BF-4cb1-9ABF-4558D9835223}
static const GUID IID_IChartSplineSeriesStyle =
{ 0x373bcc77, 0x27bf, 0x4cb1, { 0x9a, 0xbf, 0x45, 0x58, 0xd9, 0x83, 0x52, 0x23 } };

BEGIN_INTERFACE_MAP(CXTPChartSplineSeriesStyle, CXTPChartPointSeriesStyle)
	INTERFACE_PART(CXTPChartSplineSeriesStyle, IID_IChartSplineSeriesStyle, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartSplineSeriesStyle, IID_IChartSplineSeriesStyle)

// {37490E6F-92B6-4671-9613-6B2A0FBF80A8}
IMPLEMENT_OLECREATE_EX2(CXTPChartSplineSeriesStyle, "Codejock.ChartSplineSeriesStyle." _XTP_AXLIB_VERSION,
	0x37490e6f, 0x92b6, 0x4671, 0x96, 0x13, 0x6b, 0x2a, 0xf, 0xbf, 0x80, 0xa8);

LPDISPATCH CXTPChartSplineSeriesStyle::OleGetLineStyle()
{
	return XTPGetDispatch(m_pLineStyle);
}


#endif
