// XTPChartPointSeriesStyle.cpp
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

#include <Common/XTPPropExchange.h>
#include <Common/XTPMathUtils.h>

#include <Chart/Types/XTPChartTypes.h>
#include <Chart/XTPChartDefines.h>
#include <Chart/XTPChartElement.h>
#include <Chart/XTPChartLegendItem.h>
#include <Chart/XTPChartElementView.h>
#include <Chart/XTPChartSeries.h>
#include <Chart/XTPChartSeriesPoint.h>
#include <Chart/XTPChartSeriesPointView.h>
#include <Chart/XTPChartSeriesView.h>
#include <Chart/XTPChartSeriesLabel.h>
#include <Chart/XTPChartSeriesStyle.h>
#include <Chart/XTPChartErrorBar.h>
#include <Chart/Drawing/XTPChartDeviceCommand.h>
#include <Chart/Diagram/Diagram2D/XTPChartDiagram2DSeriesStyle.h>
#include <Chart/Diagram/Diagram2D/XTPChartDiagram2DSeriesLabel.h>
#include <Chart/Diagram/Diagram2D/XTPChartDiagram2DSeriesView.h>

#include "XTPChartPointSeriesStyle.h"
#include "XTPChartPointSeriesLabel.h"
#include "XTPChartPointSeriesView.h"
#include "XTPChartPointSeriesErrorBar.h"
#include "XTPChartMarker.h"

IMPLEMENT_SERIAL(CXTPChartPointSeriesStyle, CXTPChartDiagram2DSeriesStyle, VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)

//////////////////////////////////////////////////////////////////////////
// CXTPChartPointSeriesStyle

CXTPChartPointSeriesStyle::CXTPChartPointSeriesStyle()
{
	m_pMarker = new CXTPChartMarker(this);

	SetLabel(new CXTPChartPointSeriesLabel());
	SetErrorBar(new CXTPChartPointSeriesErrorBar());
}

CXTPChartPointSeriesStyle::~CXTPChartPointSeriesStyle()
{
	SAFE_RELEASE(m_pMarker);
}

void CXTPChartPointSeriesStyle::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPChartDiagram2DSeriesStyle::DoPropExchange(pPX);

	CXTPPropExchangeSection secMarker(pPX->GetSection(_T("Marker")));
	m_pMarker->DoPropExchange(&secMarker);
}

CXTPChartSeriesView* CXTPChartPointSeriesStyle::CreateView(CXTPChartSeries* pSeries, CXTPChartDiagramView* pDiagramView)
{
	return new CXTPChartPointSeriesView(pSeries, pDiagramView);
}


//////////////////////////////////////////////////////////////////////////
// CXTPChartPointSeriesView

CXTPChartPointSeriesView::CXTPChartPointSeriesView(CXTPChartSeries* pSeries, CXTPChartDiagramView* pDiagramView)
	: CXTPChartDiagram2DSeriesView(pSeries,pDiagramView)
{

}
CXTPChartSeriesPointView* CXTPChartPointSeriesView::CreateSeriesPointView(CXTPChartDeviceContext* pDC, CXTPChartSeriesPoint* pPoint, CXTPChartElementView* pParentView)
{
	UNREFERENCED_PARAMETER(pDC);
	return new CXTPChartPointSeriesPointView(pPoint, pParentView);
}

CXTPChartDeviceCommand* CXTPChartPointSeriesView::CreateLegendDeviceCommand(CXTPChartDeviceContext* pDC, CRect rcBounds)
{
	UNREFERENCED_PARAMETER(pDC);
	rcBounds.DeflateRect(1, 1, 2, 2);

	CXTPChartDeviceCommand* pCommand = new CXTPChartDeviceCommand();

	CXTPChartPointSeriesStyle* pStyle = STATIC_DOWNCAST(CXTPChartPointSeriesStyle, m_pSeries->GetStyle());

	CXTPChartPointF ptCenter(rcBounds.CenterPoint().x, rcBounds.CenterPoint().y) ;
	int nSize = min(rcBounds.Width(), rcBounds.Height());

	pCommand->AddChildCommand(pStyle->GetMarker()->CreateDeviceCommand(pDC, ptCenter, nSize, m_pSeries->GetColor(), m_pSeries->GetColor2(), m_pSeries->GetColor().GetDarkColor()));

	return pCommand;
}


//////////////////////////////////////////////////////////////////////////
// CXTPChartPointSeriesPointView

CXTPChartPointSeriesPointView::CXTPChartPointSeriesPointView(CXTPChartSeriesPoint* pPoint, CXTPChartElementView* pParentView)
	: CXTPChartSeriesPointView(pPoint, pParentView)
{

}

CXTPChartPointF CXTPChartPointSeriesPointView::GetScreenPoint() const
{
	CXTPChartPointSeriesView* pView = (CXTPChartPointSeriesView*)GetSeriesView();

	return pView->GetScreenPoint(m_pPoint->GetInternalArgumentValue(), m_dInternalValue);
}


CXTPChartDeviceCommand* CXTPChartPointSeriesPointView::CreateDeviceCommand(CXTPChartDeviceContext* pDC)
{
	CXTPChartColor color = GetColor();
	CXTPChartColor color2 = GetColor2();
	CXTPChartColor colorBorder = GetColor().GetDarkColor();

	if (CXTPMathUtils::IsNan(m_dInternalValue))
		return NULL;

	CXTPChartDeviceCommand* pCommand = new CXTPChartHitTestElementCommand(m_pPoint);

	CXTPChartPointF point = GetScreenPoint();

	pCommand->AddChildCommand(((CXTPChartPointSeriesStyle*)GetSeriesView()->GetStyle())->GetMarker()->CreateDeviceCommand(pDC, point, color, color2, colorBorder));

	return pCommand;
}

CXTPChartDeviceCommand* CXTPChartPointSeriesPointView::CreateLegendDeviceCommand(CXTPChartDeviceContext* pDC, CRect rcBounds)
{
	UNREFERENCED_PARAMETER(pDC);
	rcBounds.DeflateRect(1, 1, 2, 2);

	CXTPChartDeviceCommand* pCommand = new CXTPChartDeviceCommand();

	CXTPChartPointSeriesStyle* pStyle = STATIC_DOWNCAST(CXTPChartPointSeriesStyle, GetSeriesView()->GetSeries()->GetStyle());

	CXTPChartPointF ptCenter(rcBounds.CenterPoint().x, rcBounds.CenterPoint().y) ;
	int nSize = min(rcBounds.Width(), rcBounds.Height());

	pCommand->AddChildCommand(pStyle->GetMarker()->CreateDeviceCommand(pDC, ptCenter, nSize, GetColor(), GetColor2(), GetColor().GetDarkColor()));

	return pCommand;

}



#ifdef _XTP_ACTIVEX



BEGIN_DISPATCH_MAP(CXTPChartPointSeriesStyle, CXTPChartDiagram2DSeriesStyle)
	DISP_PROPERTY_EX_ID(CXTPChartPointSeriesStyle, "Marker", 4, OleGetMarker, SetNotSupported, VT_DISPATCH)
END_DISPATCH_MAP()


// {D32BCC77-27BF-4cb1-9ABF-4558D9835223}
static const GUID IID_IChartPointSeriesStyle =
{ 0xd32bcc77, 0x27bf, 0x4cb1, { 0x9a, 0xbf, 0x45, 0x58, 0xd9, 0x83, 0x52, 0x23 } };

BEGIN_INTERFACE_MAP(CXTPChartPointSeriesStyle, CXTPChartDiagram2DSeriesStyle)
	INTERFACE_PART(CXTPChartPointSeriesStyle, IID_IChartPointSeriesStyle, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartPointSeriesStyle, IID_IChartPointSeriesStyle)

// {24490E6F-92B6-4671-9613-6B2A0FBF80A8}
IMPLEMENT_OLECREATE_EX2(CXTPChartPointSeriesStyle, "Codejock.ChartPointSeriesStyle." _XTP_AXLIB_VERSION,
	0x24490e6f, 0x92b6, 0x4671, 0x96, 0x13, 0x6b, 0x2a, 0xf, 0xbf, 0x80, 0xa8);


LPDISPATCH CXTPChartPointSeriesStyle::OleGetMarker()
{
	return XTPGetDispatch(m_pMarker);
}

#endif
