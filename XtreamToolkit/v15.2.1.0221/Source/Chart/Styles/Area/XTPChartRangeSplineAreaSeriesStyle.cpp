// XTPChartRangeSplineAreaSeriesStyle.cpp
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
#include "../../Utils/XTPChartMathUtils.h"

#include "../../XTPChartDefines.h"
#include "../../XTPChartElement.h"
#include "../../XTPChartElementView.h"
#include "../../XTPChartSeries.h"
#include "../../XTPChartSeriesPoint.h"
#include "../../XTPChartSeriesStyle.h"
#include "../../XTPChartSeriesView.h"
#include "../../XTPChartSeriesLabel.h"

#include "../../Appearance/XTPChartBorder.h"
#include "../../Appearance/XTPChartFillStyle.h"


#include "../../Diagram/Diagram2D/XTPChartDiagram2DSeriesStyle.h"
#include "../../Diagram/Diagram2D/XTPChartDiagram2DSeriesView.h"
#include "../../Diagram/Diagram2D/XTPChartDiagram2DSeriesLabel.h"

#include "../Point/XTPChartPointSeriesStyle.h"
#include "../Point/XTPChartPointSeriesView.h"
#include "../Point/XTPChartMarker.h"

#include "../../Drawing/XTPChartDeviceCommand.h"
#include "../../Drawing/XTPChartLineDeviceCommand.h"

#include "XTPChartAreaSeriesStyle.h"
#include "XTPChartAreaSeriesView.h"
#include "XTPChartRangeSplineAreaSeriesStyle.h"

#include "../Point/XTPChartPointSeriesLabel.h"
#include "../Point/XTPChartPointSeriesLabelView.h"
#include "XTPChartRangeAreaSeriesLabel.h"

//////////////////////////////////////////////////////////////////////////
// CXTPChartRangeSplineAreaSeriesStyle

IMPLEMENT_SERIAL(CXTPChartRangeSplineAreaSeriesStyle, CXTPChartAreaSeriesStyle, VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT);

CXTPChartRangeSplineAreaSeriesStyle::CXTPChartRangeSplineAreaSeriesStyle()
{
	SetLabel(new CXTPChartRangeAreaSeriesLabel());

}

CXTPChartRangeSplineAreaSeriesStyle::~CXTPChartRangeSplineAreaSeriesStyle()
{
}

CXTPChartSeriesView* CXTPChartRangeSplineAreaSeriesStyle::CreateView(CXTPChartSeries* pSeries, CXTPChartDiagramView* pDiagramView)
{
	return new CXTPChartRangeSplineAreaSeriesView(pSeries, pDiagramView);
}


//////////////////////////////////////////////////////////////////////////
// CXTPChartRangeSplineAreaSeriesPointView

CXTPChartRangeSplineAreaSeriesPointView::CXTPChartRangeSplineAreaSeriesPointView(CXTPChartSeriesPoint* pPoint, CXTPChartElementView* pParentView)
	: CXTPChartPointSeriesPointView(pPoint, pParentView)
{

}

void CXTPChartRangeSplineAreaSeriesPointView::UpdateMinMaxRange(double& nMinValue, double& nMaxValue) const
{
	if (m_pPoint->GetValueLength() > 1)
	{
		double nValue = m_pPoint->GetValue(1);

		nMaxValue = max(nMaxValue, nValue);
		nMinValue = min(nMinValue, nValue);
	}
}

CXTPChartDeviceCommand* CXTPChartRangeSplineAreaSeriesPointView::CreateDeviceCommand(CXTPChartDeviceContext* pDC)
{
	CXTPChartColor color = GetColor();
	CXTPChartColor color2 = GetColor2();
	CXTPChartColor colorBorder = GetColor().GetDarkColor();

	if (CXTPChartMathUtils::IsNan(m_dInternalValue))
		return NULL;

	CXTPChartDeviceCommand* pCommand = new CXTPChartHitTestElementCommand(m_pPoint);

	CXTPChartPointSeriesView* pView = (CXTPChartPointSeriesView*)GetSeriesView();

	CXTPChartPointF point =  pView->GetScreenPoint(m_pPoint->GetInternalArgumentValue(), m_pPoint->GetValue(0));
	pCommand->AddChildCommand(((CXTPChartPointSeriesStyle*)GetSeriesView()->GetStyle())->GetMarker()->CreateDeviceCommand(pDC, point, color, color2, colorBorder));

	point =  pView->GetScreenPoint(m_pPoint->GetInternalArgumentValue(), m_pPoint->GetValue(1));
	pCommand->AddChildCommand(((CXTPChartPointSeriesStyle*)GetSeriesView()->GetStyle())->GetMarker()->CreateDeviceCommand(pDC, point, color, color2, colorBorder));

	return pCommand;
}


//////////////////////////////////////////////////////////////////////////
// CXTPChartRangeSplineAreaSeriesView

CXTPChartRangeSplineAreaSeriesView::CXTPChartRangeSplineAreaSeriesView(CXTPChartSeries* pSeries, CXTPChartDiagramView* pDiagramView)
	: CXTPChartAreaSeriesView(pSeries,pDiagramView)
{

}

CXTPChartSeriesPointView* CXTPChartRangeSplineAreaSeriesView::CreateSeriesPointView(CXTPChartDeviceContext* pDC, CXTPChartSeriesPoint* pPoint, CXTPChartElementView* pParentView)
{
	UNREFERENCED_PARAMETER(pDC);
	return new CXTPChartRangeSplineAreaSeriesPointView(pPoint, pParentView);
}




CXTPChartDeviceCommand* CXTPChartRangeSplineAreaSeriesView::CreateDeviceCommand(CXTPChartDeviceContext* pDC)
{
	CXTPChartDeviceCommand* pSeriesCommand = new CXTPChartHitTestElementCommand(m_pSeries);
	CXTPChartDeviceCommand* pCommand = pSeriesCommand->AddChildCommand(new CXTPChartPolygonAntialiasingDeviceCommand());

	CXTPChartRangeSplineAreaSeriesStyle* pStyle = (CXTPChartRangeSplineAreaSeriesStyle*)GetStyle();

	CXTPChartColor clrBorder = GetBorderActualColor();


	int nCount = m_pPointsView->GetCount();
	if (nCount > 1)
	{
		CXTPChartSeriesPointView* pPointView;

		CXTPChartPoints arrPoints;

		int i;

		for (i = 0; i < nCount; i++)
		{
			pPointView =  (CXTPChartSeriesPointView*)m_pPointsView->GetAt(i);

			CXTPChartPointF point = GetScreenPoint(pPointView->GetPoint()->GetInternalArgumentValue(),
				pPointView->GetPoint()->GetValue(1));

			arrPoints.Add(point);
		}

		if (pStyle->GetBorder()->IsVisible())
			pCommand->AddChildCommand(new CXTPChartSolidSplineDeviceCommand(arrPoints, clrBorder, pStyle->GetBorder()->GetThickness()));

		CXTPChartPoints arrBottomPoints;

		for (i = nCount - 1; i >= 0; i--)
		{
			pPointView =  (CXTPChartSeriesPointView*)m_pPointsView->GetAt(i);

			CXTPChartPointF point = GetScreenPoint(pPointView->GetPoint()->GetInternalArgumentValue(),
				pPointView->GetPoint()->GetValue(0));

			arrPoints.Add(point);
			arrBottomPoints.Add(point);
		}
		if (pStyle->GetBorder()->IsVisible())
			pCommand->AddChildCommand(new CXTPChartSolidSplineDeviceCommand(arrBottomPoints, clrBorder, pStyle->GetBorder()->GetThickness()));

		pCommand->AddChildCommand(pStyle->GetFillStyle()->CreateSplineDeviceCommand(arrPoints, GetActualColor(), GetActualColor2(), TRUE));


	}

	pCommand->AddChildCommand(CXTPChartSeriesView::CreateDeviceCommand(pDC));

	return pSeriesCommand;
}

#ifdef _XTP_ACTIVEX


BEGIN_DISPATCH_MAP(CXTPChartRangeSplineAreaSeriesStyle, CXTPChartAreaSeriesStyle)
END_DISPATCH_MAP()


// {5A4BCC77-27BF-4cb1-9ABF-4558D9835223}
static const GUID IID_IChartRangeSplineAreaSeriesStyle =
{ 0x5a4bcc77, 0x27bf, 0x4cb1, { 0x9a, 0xbf, 0x45, 0x58, 0xd9, 0x83, 0x52, 0x23 } };

BEGIN_INTERFACE_MAP(CXTPChartRangeSplineAreaSeriesStyle, CXTPChartAreaSeriesStyle)
	INTERFACE_PART(CXTPChartRangeSplineAreaSeriesStyle, IID_IChartRangeSplineAreaSeriesStyle, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartRangeSplineAreaSeriesStyle, IID_IChartRangeSplineAreaSeriesStyle)

// {5A490E6F-92B6-4671-9613-6B2A0FBF80A8}
IMPLEMENT_OLECREATE_EX2(CXTPChartRangeSplineAreaSeriesStyle, "Codejock.ChartRangeSplineAreaSeriesStyle." _XTP_AXLIB_VERSION,
0x5a490e6f, 0x92b6, 0x4671, 0x96, 0x13, 0x6b, 0x2a, 0xf, 0xbf, 0x80, 0xa8);

#endif
