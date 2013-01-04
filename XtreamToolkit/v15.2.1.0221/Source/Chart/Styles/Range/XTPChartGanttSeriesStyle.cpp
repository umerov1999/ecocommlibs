// XTPChartGanttSeriesStyle.cpp
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

#include "../../XTPChartElement.h"
#include "../../XTPChartElementView.h"
#include "../../XTPChartSeriesStyle.h"
#include "../../XTPChartSeriesLabel.h"
#include "../../XTPChartSeriesView.h"

#include "../../Diagram/Diagram2D/XTPChartDiagram2DSeriesStyle.h"
#include "../../Diagram/Diagram2D/XTPChartDiagram2DSeriesView.h"
#include "../../Diagram/Diagram2D/XTPChartDiagram2DSeriesLabel.h"



#include "../Bar/XTPChartBarSeriesStyle.h"
#include "../Bar/XTPChartBarSeriesView.h"

#include "XTPChartRangeBarSeriesStyle.h"
#include "XTPChartRangeBarSeriesView.h"
#include "XTPChartGanttSeriesStyle.h"
#include "XTPChartRangeBarSeriesLabel.h"




//////////////////////////////////////////////////////////////////////////
// CXTPChartGanttSeriesStyle

IMPLEMENT_SERIAL(CXTPChartGanttSeriesStyle, CXTPChartRangeBarSeriesStyle, VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT);

CXTPChartGanttSeriesStyle::CXTPChartGanttSeriesStyle()
{
	SetLabel(new CXTPChartRangeBarSeriesLabel());

}

CXTPChartGanttSeriesStyle::~CXTPChartGanttSeriesStyle()
{
}


void CXTPChartGanttSeriesStyle::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPChartRangeBarSeriesStyle::DoPropExchange(pPX);

}

CXTPChartSeriesView* CXTPChartGanttSeriesStyle::CreateView(CXTPChartSeries* pSeries, CXTPChartDiagramView* pDiagramView)
{
	return new CXTPChartGanttSeriesView(pSeries, pDiagramView);
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartGanttSeriesView

CXTPChartGanttSeriesView::CXTPChartGanttSeriesView(CXTPChartSeries* pSeries, CXTPChartDiagramView* pDiagramView)
	: CXTPChartRangeBarSeriesView(pSeries,pDiagramView)
{
}

CXTPChartSeriesPointView* CXTPChartGanttSeriesView::CreateSeriesPointView(CXTPChartDeviceContext* pDC, CXTPChartSeriesPoint* pPoint, CXTPChartElementView* pParentView)
{
	UNREFERENCED_PARAMETER(pDC);
	return new CXTPChartGanttSeriesPointView(pPoint, pParentView);
}


//////////////////////////////////////////////////////////////////////////
// CXTPChartGanttSeriesPointView

CXTPChartGanttSeriesPointView::CXTPChartGanttSeriesPointView(CXTPChartSeriesPoint* pPoint, CXTPChartElementView* pParentView)
	: CXTPChartRangeBarSeriesPointView(pPoint, pParentView)
{

}




#ifdef _XTP_ACTIVEX


BEGIN_DISPATCH_MAP(CXTPChartGanttSeriesStyle, CXTPChartRangeBarSeriesStyle)
END_DISPATCH_MAP()


// {F65BCC77-27BF-4cb1-9ABF-4558D9835223}
static const GUID IID_IChartGanttSeriesStyle =
{ 0xf65bcc77, 0x27bf, 0x4cb1, { 0x9a, 0xbf, 0x45, 0x58, 0xd9, 0x83, 0x52, 0x23 } };

BEGIN_INTERFACE_MAP(CXTPChartGanttSeriesStyle, CXTPChartRangeBarSeriesStyle)
	INTERFACE_PART(CXTPChartGanttSeriesStyle, IID_IChartGanttSeriesStyle , Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartGanttSeriesStyle, IID_IChartGanttSeriesStyle )

// {F6590E6F-92B6-4671-9613-6B2A0FBF80A8}
IMPLEMENT_OLECREATE_EX2(CXTPChartGanttSeriesStyle, "Codejock.ChartGanttSeriesStyle." _XTP_AXLIB_VERSION,
0xf6590e6f, 0x92b6, 0x4671, 0x96, 0x13, 0x6b, 0x2a, 0xf, 0xbf, 0x80, 0xa8);



#endif
