// XTPChartHighLowSeriesStyle.cpp
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
#include "Common/Base/Types/XTPPoint3.h"

#include "../../XTPChartDefines.h"
#include "../../XTPChartElement.h"
#include <Chart/XTPChartLegendItem.h>
#include "../../XTPChartElementView.h"
#include "../../XTPChartSeriesStyle.h"
#include <Chart/XTPChartSeriesPointView.h>
#include "../../XTPChartSeriesView.h"
#include "../../XTPChartSeriesPoint.h"

#include "../../Diagram/Axis/XTPChartAxisView.h"
#include "../../Diagram/Diagram2D/XTPChartDiagram2DSeriesStyle.h"
#include "../../Diagram/Diagram2D/XTPChartDiagram2DSeriesView.h"

#include "../../Appearance/XTPChartAppearance.h"

#include "../../Drawing/XTPChartDeviceCommand.h"
#include "../../Drawing/XTPChartLineDeviceCommand.h"

#include "XTPChartHighLowSeriesStyle.h"

enum ValueIndex
{
	chartLow,
	chartHigh,
	chartOpen,
	chartClose
};

//////////////////////////////////////////////////////////////////////////
// CXTPChartHighLowSeriesStyle

IMPLEMENT_SERIAL(CXTPChartHighLowSeriesStyle, CXTPChartDiagram2DSeriesStyle, VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)

CXTPChartHighLowSeriesStyle::CXTPChartHighLowSeriesStyle()
{
	m_nLineThickness = 2;

	m_clrDownColor = CXTPChartColor::Empty;
	m_clrUpColor = CXTPChartColor::Empty;
}

CXTPChartHighLowSeriesStyle::~CXTPChartHighLowSeriesStyle()
{

}

CXTPChartSeriesView* CXTPChartHighLowSeriesStyle::CreateView(CXTPChartSeries* pSeries, CXTPChartDiagramView* pDiagramView)
{
	return new CXTPChartHighLowSeriesView(pSeries, pDiagramView);
}


CXTPChartColor CXTPChartHighLowSeriesStyle::GetUpColor() const
{
	if (!m_clrUpColor.IsEmpty())
		return m_clrUpColor;

	return GetAppearance()->GetFinanceStyleAppearance()->UpColor;

}
CXTPChartColor CXTPChartHighLowSeriesStyle::GetDownColor() const
{
	if (!m_clrDownColor.IsEmpty())
		return m_clrDownColor;

	return GetAppearance()->GetFinanceStyleAppearance()->DownColor;

}


//////////////////////////////////////////////////////////////////////////
// CXTPChartHighLowSeriesView

CXTPChartHighLowSeriesView::CXTPChartHighLowSeriesView(CXTPChartSeries* pSeries, CXTPChartDiagramView* pDiagramView)
	: CXTPChartDiagram2DSeriesView(pSeries,pDiagramView)
{

}
CXTPChartSeriesPointView* CXTPChartHighLowSeriesView::CreateSeriesPointView(CXTPChartDeviceContext* pDC, CXTPChartSeriesPoint* pPoint, CXTPChartElementView* pParentView)
{
	UNREFERENCED_PARAMETER(pDC);
	return new CXTPChartHighLowSeriesPointView(pPoint, pParentView);
}


CXTPChartDeviceCommand* CXTPChartHighLowSeriesView::CreateLegendDeviceCommand(CXTPChartDeviceContext* pDC, CRect rcBounds)
{
	UNREFERENCED_PARAMETER(pDC);
	rcBounds.DeflateRect(1, 1);

	CXTPChartDeviceCommand* pCommand = new CXTPChartDeviceCommand();

	CXTPChartHighLowSeriesStyle* pStyle = STATIC_DOWNCAST(CXTPChartHighLowSeriesStyle, GetStyle());

	CPoint ptCenter(rcBounds.CenterPoint().x, rcBounds.CenterPoint().y) ;

	for (int i = 0; i < 2; i++)
	{
		CXTPChartColor color = i == 0 ? pStyle->GetUpColor() : pStyle->GetDownColor();

		int nHeight = rcBounds.Height() - 3;
		int x = i == 0 ? ptCenter.x - 4 : ptCenter.x + 4;
		int y = i == 0 ? rcBounds.top : rcBounds.top + 3;

		CXTPPoint2i pointHight(x, y);
		CXTPPoint2i pointLow(x, pointHight.Y + nHeight);
		CXTPPoint2i pointOpen(x, pointHight.Y + nHeight / 3);
		CXTPPoint2i pointClose(x, pointHight.Y + nHeight * 2 / 3);
		int nWidth = 6;

		pCommand->AddChildCommand(new CXTPChartSolidLineDeviceCommand(
			CXTPChartPointF(pointHight), CXTPChartPointF(pointLow), color, 2));

		pCommand->AddChildCommand(new CXTPChartSolidLineDeviceCommand(
			CXTPChartPointF(pointOpen), CXTPChartPointF((float)(pointOpen.X - nWidth / 2), (float)pointOpen.Y), color, 2));

		pCommand->AddChildCommand(new CXTPChartSolidLineDeviceCommand(
			CXTPChartPointF(pointClose), CXTPChartPointF((float)(pointClose.X + nWidth / 2), (float)pointClose.Y), color, 2));
	}

	return pCommand;
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartHighLowSeriesPointView

CXTPChartHighLowSeriesPointView::CXTPChartHighLowSeriesPointView(CXTPChartSeriesPoint* pPoint, CXTPChartElementView* pParentView)
	: CXTPChartSeriesPointView(pPoint, pParentView)
{

}
CXTPChartPointF CXTPChartHighLowSeriesPointView::GetScreenPoint(int nIndex)
{
	CXTPChartHighLowSeriesView* pView = (CXTPChartHighLowSeriesView*)GetSeriesView();

	return pView->GetScreenPoint(m_pPoint->GetInternalArgumentValue(), m_pPoint->GetValue(nIndex));
}


CXTPChartDeviceCommand* CXTPChartHighLowSeriesPointView::CreateDeviceCommand(CXTPChartDeviceContext* pDC)
{
	UNREFERENCED_PARAMETER(pDC);

	CXTPPoint2i pointOpen = GetScreenPoint(chartOpen).Round();
	CXTPPoint2i pointClose = GetScreenPoint(chartClose).Round();

	CXTPPoint2i pointLow = GetScreenPoint(chartLow).Round();
	CXTPPoint2i pointHight = GetScreenPoint(chartHigh).Round();

	BOOL bUpColor = TRUE;


	CXTPChartSeriesPoint* pPrevPoint = m_pPoint->GetPreviousPoint();
	if (pPrevPoint)
	{
		if (pPrevPoint->GetValue(chartClose) > m_pPoint->GetValue(chartClose))
		{
			bUpColor = FALSE;
		}
	}

	CXTPChartHighLowSeriesView* pView = (CXTPChartHighLowSeriesView*)GetSeriesView();

	CXTPChartDiagram2DAxisView* pAxisView = pView->GetAxisViewX();
	CXTPChartHighLowSeriesStyle* pStyle = (CXTPChartHighLowSeriesStyle*)GetSeriesView()->GetStyle();

	int nWidth = (int)(pAxisView->DistanceToPoint(1) * 0.5);
	if (nWidth < 5)
		nWidth = 5;

	int nLineThickness = pStyle->GetLineThickness();

	nWidth = (nWidth & ~1) + nLineThickness * 3;

	CXTPChartDeviceCommand* pCommand = new CXTPChartHitTestElementCommand(m_pPoint);

	CXTPChartColor color = bUpColor ? pStyle->GetUpColor() : pStyle->GetDownColor();


	pCommand->AddChildCommand(new CXTPChartSolidLineDeviceCommand(
		CXTPChartPointF(pointHight), CXTPChartPointF(pointLow), color, nLineThickness));

	pCommand->AddChildCommand(new CXTPChartSolidLineDeviceCommand(
		CXTPChartPointF(pointOpen), CXTPChartPointF((float)(pointOpen.X - nWidth / 2), (float)pointOpen.Y), color, nLineThickness));

	pCommand->AddChildCommand(new CXTPChartSolidLineDeviceCommand(
		CXTPChartPointF(pointClose), CXTPChartPointF((float)(pointClose.X + nWidth / 2), (float)pointClose.Y), color, nLineThickness));


	return pCommand;
}




#ifdef _XTP_ACTIVEX



BEGIN_DISPATCH_MAP(CXTPChartHighLowSeriesStyle, CXTPChartDiagram2DSeriesStyle)
	DISP_PROPERTY_EX_ID(CXTPChartHighLowSeriesStyle, "LineThickness", 100, GetLineThickness, SetLineThickness, VT_I4)
END_DISPATCH_MAP()


// {432BCC77-27BF-4cb1-9ABF-4558D9835223}
static const GUID IID_IChartHighLowSeriesStyle =
{ 0x432bcc77, 0x27bf, 0x4cb1, { 0x9a, 0xbf, 0x45, 0x58, 0xd9, 0x83, 0x52, 0x23 } };

BEGIN_INTERFACE_MAP(CXTPChartHighLowSeriesStyle, CXTPChartDiagram2DSeriesStyle)
	INTERFACE_PART(CXTPChartHighLowSeriesStyle, IID_IChartHighLowSeriesStyle, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartHighLowSeriesStyle, IID_IChartHighLowSeriesStyle)

// {43490E6F-92B6-4671-9613-6B2A0FBF80A8}
IMPLEMENT_OLECREATE_EX2(CXTPChartHighLowSeriesStyle, "Codejock.ChartHighLowSeriesStyle." _XTP_AXLIB_VERSION,
	0x43490e6f, 0x92b6, 0x4671, 0x96, 0x13, 0x6b, 0x2a, 0xf, 0xbf, 0x80, 0xa8);


#endif
