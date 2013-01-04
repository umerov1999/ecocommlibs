// XTPChartLineSeriesStyle.cpp
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
#include "../../XTPChartSeriesView.h"
#include "../../XTPChartSeriesPoint.h"
#include "../../XTPChartSeriesStyle.h"
#include "../../XTPChartDiagramView.h"

#include "../../Diagram/Axis/XTPChartAxis.h"
#include "../../Diagram/Axis/XTPChartAxisView.h"
#include "../../Diagram/Axis/XTPChartAxisRange.h"
#include "../../Diagram/Diagram2D/XTPChartDiagram2DSeriesStyle.h"
#include "../../Diagram/Diagram2D/XTPChartDiagram2DSeriesView.h"

#include "../../Drawing/XTPChartDeviceCommand.h"
#include "../../Drawing/XTPChartLineDeviceCommand.h"
#include "../../Appearance/XTPChartLineStyle.h"

#include "../Point/XTPChartPointSeriesView.h"

#include "XTPChartFastLineSeriesStyle.h"


IMPLEMENT_SERIAL(CXTPChartFastLineSeriesStyle, CXTPChartDiagram2DSeriesStyle, VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)

//////////////////////////////////////////////////////////////////////////
// CXTPChartFastLineSeriesStyle

CXTPChartFastLineSeriesStyle::CXTPChartFastLineSeriesStyle()
{
	m_pLineStyle = new CXTPChartLineStyle(this);
	m_pLineStyle->SetThickness(2);

	m_bAntialiasing = TRUE;

	SetLabel(NULL);
}

CXTPChartFastLineSeriesStyle::~CXTPChartFastLineSeriesStyle()
{
	SAFE_RELEASE(m_pLineStyle);

}


CXTPChartSeriesView* CXTPChartFastLineSeriesStyle::CreateView(CXTPChartSeries* pSeries, CXTPChartDiagramView* pDiagramView)
{
	return new CXTPChartFastLineSeriesView(pSeries, pDiagramView);
}

void CXTPChartFastLineSeriesStyle::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPChartDiagram2DSeriesStyle::DoPropExchange(pPX);

	CXTPPropExchangeSection sec(pPX->GetSection(_T("LineStyle")));
	m_pLineStyle->DoPropExchange(&sec);
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartFastLineSeriesView

CXTPChartFastLineSeriesView::CXTPChartFastLineSeriesView(CXTPChartSeries* pSeries, CXTPChartDiagramView* pDiagramView)
	: CXTPChartDiagram2DSeriesView(pSeries, pDiagramView)
{

}

void CXTPChartFastLineSeriesView::CreatePointsView(CXTPChartDeviceContext* /*pDC*/)
{
	m_pPointsView = new CXTPChartElementView(this);
	m_pLabelsView = m_pDiagramView->GetLabelsView();
}

CXTPChartSeriesPointView* CXTPChartFastLineSeriesView::CreateSeriesPointView(CXTPChartDeviceContext* pDC, CXTPChartSeriesPoint* pPoint, CXTPChartElementView* pParentView)
{
	UNREFERENCED_PARAMETER(pDC);
	return new CXTPChartPointSeriesPointView(pPoint, pParentView);
}

CXTPChartDeviceCommand* CXTPChartFastLineSeriesView::CreateDeviceCommand(CXTPChartDeviceContext* /*pDC*/)
{
	CXTPChartDeviceCommand* pSeriesCommand = new CXTPChartHitTestElementCommand(m_pSeries);
	CXTPChartFastLineSeriesStyle* pStyle = (CXTPChartFastLineSeriesStyle*)GetStyle();

	CXTPChartDeviceCommand* pCommand = pSeriesCommand->AddChildCommand(new CXTPChartPolygonAntialiasingDeviceCommand(pStyle->GetAntialiasing()));

	CXTPChartAxisRange* pRange = GetAxisViewX()->GetAxis()->GetRange();

	int nCount = m_pSeries->GetPoints()->GetCount();
	if (nCount > 1)
	{
		CXTPChartPoints arrPoints;

		double dViewMinValue = pRange->GetViewMinValue();
		double dViewMaxValue = pRange->GetViewMaxValue();

		int l = 0, r = nCount - 1;
		while (l < r)
		{
			int m = (l + r) / 2;
			CXTPChartSeriesPoint*  pPoint = m_pSeries->GetPoints()->GetAt(m);

			if (pPoint->GetArgumentValue() < dViewMinValue)
			{
				l = m + 1;
			}
			else
			{
				r = m;
			}
		}

		for (int i = max(l - 1, 0); i < nCount; i++)
		{
			CXTPChartSeriesPoint*  pPoint = m_pSeries->GetPoints()->GetAt(i);

			CXTPChartPointF pointNext = GetScreenPoint(pPoint->GetArgumentValue(), pPoint->GetValue(0));
			arrPoints.Add(pointNext);

			if (pPoint->GetArgumentValue() > dViewMaxValue)
				break;
		}

		pCommand->AddChildCommand(pStyle->GetLineStyle()->CreateDeviceCommand(arrPoints, GetColor()));
	}

	return pSeriesCommand;
}

CXTPChartDeviceCommand* CXTPChartFastLineSeriesView::CreateLegendDeviceCommand(CXTPChartDeviceContext* pDC, CRect rcBounds)
{
	UNREFERENCED_PARAMETER(pDC);
	rcBounds.DeflateRect(1, 1);

	CXTPChartDeviceCommand* pCommand = new CXTPChartDeviceCommand();

	CXTPChartPointF ptCenter(rcBounds.CenterPoint().x, rcBounds.CenterPoint().y) ;

	pCommand->AddChildCommand(new CXTPChartSolidLineDeviceCommand(CXTPPoint3d(rcBounds.left,ptCenter.Y),
		CXTPPoint3d(rcBounds.right,ptCenter.Y), GetColor(), 1));

	return pCommand;
}

#ifdef _XTP_ACTIVEX


BEGIN_DISPATCH_MAP(CXTPChartFastLineSeriesStyle, CXTPChartDiagram2DSeriesStyle)
	DISP_PROPERTY_EX_ID(CXTPChartFastLineSeriesStyle, "LineStyle", 100, OleGetLineStyle, SetNotSupported, VT_DISPATCH)
	DISP_PROPERTY_NOTIFY_ID(CXTPChartFastLineSeriesStyle, "Antialiasing", 200, m_bAntialiasing, OleChartChanged, VT_BOOL)
END_DISPATCH_MAP()


// {DC1BCC77-27BF-4cb1-9ABF-4558D9835223}
static const GUID IID_IChartLineSeriesStyle =
{ 0xdc1bcc77, 0x27bf, 0x4cb1, { 0x9a, 0xbf, 0x45, 0x58, 0xd9, 0x83, 0x52, 0x23 } };

BEGIN_INTERFACE_MAP(CXTPChartFastLineSeriesStyle, CXTPChartDiagram2DSeriesStyle)
INTERFACE_PART(CXTPChartFastLineSeriesStyle, IID_IChartLineSeriesStyle, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartFastLineSeriesStyle, IID_IChartLineSeriesStyle)

// {DC190E6F-92B6-4671-9613-6B2A0FBF80A8}
IMPLEMENT_OLECREATE_EX2(CXTPChartFastLineSeriesStyle, "Codejock.ChartFastLineSeriesStyle." _XTP_AXLIB_VERSION,
0xdc190e6f, 0x92b6, 0x4671, 0x96, 0x13, 0x6b, 0x2a, 0xf, 0xbf, 0x80, 0xa8);

LPDISPATCH CXTPChartFastLineSeriesStyle::OleGetLineStyle()
{
	return XTPGetDispatch(m_pLineStyle);
}


#endif
