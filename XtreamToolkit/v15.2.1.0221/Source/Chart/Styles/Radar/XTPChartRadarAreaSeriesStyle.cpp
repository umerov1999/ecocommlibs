// XTPChartRadarAreaSeriesStyle.cpp
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
#include "../../XTPChartSeriesPoint.h"
#include "../../XTPChartSeriesView.h"
#include "../../XTPChartSeriesLabel.h"
#include "../../XTPChartSeriesStyle.h"
#include "../../XTPChartPanel.h"
#include "../../XTPChartDiagram.h"
#include "../../XTPChartDiagramView.h"

#include "../../Utils/XTPChartMathUtils.h"
#include "../../Drawing/XTPChartDeviceCommand.h"

#include "../../Appearance/XTPChartBorder.h"
#include "../../Appearance/XTPChartFillStyle.h"

#include "../../Diagram/RadarDiagram/XTPChartRadarDiagramView.h"
#include "../../Diagram/RadarDiagram/XTPChartRadarDiagram.h"
#include "../../Diagram/RadarDiagram/XTPChartRadarDiagramSeriesStyle.h"
#include "../../Diagram/RadarDiagram/XTPChartRadarDiagramSeriesView.h"

#include "XTPChartRadarPointSeriesStyle.h"
#include "XTPChartRadarAreaSeriesStyle.h"
#include "XTPChartRadarAreaSeriesView.h"

IMPLEMENT_SERIAL(CXTPChartRadarAreaSeriesStyle, CXTPChartRadarPointSeriesStyle, VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)

//////////////////////////////////////////////////////////////////////////
// CXTPChartRadarAreaSeriesStyle

CXTPChartRadarAreaSeriesStyle::CXTPChartRadarAreaSeriesStyle()
{
	m_nTransparency = 135;

	m_pBorder = new CXTPChartBorder(this);

	m_pFillStyle = new CXTPChartFillStyle(this);
}

CXTPChartRadarAreaSeriesStyle::~CXTPChartRadarAreaSeriesStyle()
{
	SAFE_RELEASE(m_pBorder);

	SAFE_RELEASE(m_pFillStyle);

}

void CXTPChartRadarAreaSeriesStyle::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPChartRadarPointSeriesStyle::DoPropExchange(pPX);

	PX_Int(pPX, _T("Transparency"), m_nTransparency, 0);

	CXTPPropExchangeSection secBorder(pPX->GetSection(_T("Border")));
	m_pBorder->DoPropExchange(&secBorder);

	CXTPPropExchangeSection secFillStyle(pPX->GetSection(_T("FillStyle")));
	m_pFillStyle->DoPropExchange(&secFillStyle);
}

CXTPChartSeriesView* CXTPChartRadarAreaSeriesStyle::CreateView(CXTPChartSeries* pSeries, CXTPChartDiagramView* pDiagramView)
{
	return new CXTPChartRadarAreaSeriesView(pSeries, pDiagramView);
}


#ifdef _XTP_ACTIVEX


BEGIN_DISPATCH_MAP(CXTPChartRadarAreaSeriesStyle, CXTPChartRadarPointSeriesStyle)
	DISP_PROPERTY_EX_ID(CXTPChartRadarAreaSeriesStyle, "Transparency", 105, GetTransparency, SetTransparency, VT_I4)

	DISP_PROPERTY_EX_ID(CXTPChartRadarAreaSeriesStyle, "FillStyle", 5, OleGetFillStyle, SetNotSupported, VT_DISPATCH)
	DISP_PROPERTY_EX_ID(CXTPChartRadarAreaSeriesStyle, "Border", 6, OleGetBorder, SetNotSupported, VT_DISPATCH)
END_DISPATCH_MAP()


// {77FBCC77-27BF-4cb1-9ABF-4558D9835223}
static const GUID IID_IChartRadarAreaSeriesStyle =
{ 0x77fbcc77, 0x27bf, 0x4cb1, { 0x9a, 0xbf, 0x45, 0x58, 0xd9, 0x83, 0x52, 0x23 } };

BEGIN_INTERFACE_MAP(CXTPChartRadarAreaSeriesStyle, CXTPChartRadarPointSeriesStyle)
	INTERFACE_PART(CXTPChartRadarAreaSeriesStyle, IID_IChartRadarAreaSeriesStyle, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartRadarAreaSeriesStyle, IID_IChartRadarAreaSeriesStyle)

// {77F90E6F-92B6-4671-9613-6B2A0FBF80A8}
IMPLEMENT_OLECREATE_EX2(CXTPChartRadarAreaSeriesStyle, "Codejock.ChartRadarAreaSeriesStyle." _XTP_AXLIB_VERSION,
	0x77f90e6f, 0x92b6, 0x4671, 0x96, 0x13, 0x6b, 0x2a, 0xf, 0xbf, 0x80, 0xa8);


LPDISPATCH CXTPChartRadarAreaSeriesStyle::OleGetFillStyle()
{
	return XTPGetDispatch(GetFillStyle());
}

LPDISPATCH CXTPChartRadarAreaSeriesStyle::OleGetBorder()
{
	return XTPGetDispatch(GetBorder());
}

#endif
