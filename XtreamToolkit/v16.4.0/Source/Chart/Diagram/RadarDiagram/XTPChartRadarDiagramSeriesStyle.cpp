// XTPChartRadarDiagramSeriesStyle.cpp
//
// This file is a part of the XTREME TOOLKIT PRO MFC class library.
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

#include "stdafx.h"
#include "Common/XTPPropExchange.h"

#include "../../Types/XTPChartTypes.h"
#include "../../XTPChartElement.h"
#include <Chart/XTPChartLegendItem.h>
#include "../../XTPChartSeriesStyle.h"
#include "../../XTPChartPanel.h"
#include "../../XTPChartDiagram.h"


#include "XTPChartRadarDiagram.h"
#include "XTPChartRadarDiagramSeriesStyle.h"


IMPLEMENT_DYNAMIC(CXTPChartRadarDiagramSeriesStyle, CXTPChartSeriesStyle)

//////////////////////////////////////////////////////////////////////////
// CXTPChartRadarDiagramSeriesStyle

CXTPChartRadarDiagramSeriesStyle::CXTPChartRadarDiagramSeriesStyle()
{
}

CXTPChartRadarDiagramSeriesStyle::~CXTPChartRadarDiagramSeriesStyle()
{
}

void CXTPChartRadarDiagramSeriesStyle::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPChartSeriesStyle::DoPropExchange(pPX);
}

CXTPChartDiagram* CXTPChartRadarDiagramSeriesStyle::CreateDiagram()
{
	return new CXTPChartRadarDiagram();
}

BOOL CXTPChartRadarDiagramSeriesStyle::IsStyleDiagram(CXTPChartDiagram* pDiagram) const
{
	return DYNAMIC_DOWNCAST(CXTPChartRadarDiagram, pDiagram) != NULL;
}
