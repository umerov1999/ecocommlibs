// XTPChartAreaSeriesStyle.cpp
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
#include "../../XTPChartSeries.h"
#include "../../XTPChartSeriesPoint.h"
#include "../../XTPChartSeriesStyle.h"
#include <Chart/XTPChartSeriesPointView.h>
#include "../../XTPChartSeriesView.h"

#include "../../Appearance/XTPChartBorder.h"
#include "../../Appearance/XTPChartFillStyle.h"

#include "../../Diagram/Axis/XTPChartAxis.h"
#include "../../Diagram/Axis/XTPChartAxisRange.h"
#include "../../Diagram/Axis/XTPChartAxisView.h"

#include "../../Diagram/Diagram2D/XTPChartDiagram2DSeriesStyle.h"
#include "../../Diagram/Diagram2D/XTPChartDiagram2DSeriesView.h"

#include "../Point/XTPChartPointSeriesStyle.h"
#include "../Point/XTPChartPointSeriesView.h"
#include "../Point/XTPChartMarker.h"

#include "../../Drawing/XTPChartDeviceCommand.h"
#include "../../Drawing/XTPChartLineDeviceCommand.h"

#include "XTPChartAreaSeriesStyle.h"
#include "XTPChartAreaSeriesView.h"

//////////////////////////////////////////////////////////////////////////
// CXTPChartAreaSeriesStyle

IMPLEMENT_SERIAL(CXTPChartAreaSeriesStyle, CXTPChartPointSeriesStyle, VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)

CXTPChartAreaSeriesStyle::CXTPChartAreaSeriesStyle()
{
	m_nTransparency = 135;

	m_pBorder = new CXTPChartBorder(this);

	m_pFillStyle = new CXTPChartFillStyle(this);
}

CXTPChartAreaSeriesStyle::~CXTPChartAreaSeriesStyle()
{
	SAFE_RELEASE(m_pBorder);

	SAFE_RELEASE(m_pFillStyle);

}

CXTPChartSeriesView* CXTPChartAreaSeriesStyle::CreateView(CXTPChartSeries* pSeries, CXTPChartDiagramView* pDiagramView)
{
	return new CXTPChartAreaSeriesView(pSeries, pDiagramView);
}

void CXTPChartAreaSeriesStyle::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPChartPointSeriesStyle::DoPropExchange(pPX);

	PX_Int(pPX, _T("Transparency"), m_nTransparency, 0);

	CXTPPropExchangeSection secBorder(pPX->GetSection(_T("Border")));
	m_pBorder->DoPropExchange(&secBorder);

	CXTPPropExchangeSection secFillStyle(pPX->GetSection(_T("FillStyle")));
	m_pFillStyle->DoPropExchange(&secFillStyle);
}


//////////////////////////////////////////////////////////////////////////
// CXTPChartAreaSeriesView

CXTPChartAreaSeriesView::CXTPChartAreaSeriesView(CXTPChartSeries* pSeries, CXTPChartDiagramView* pDiagramView)
	: CXTPChartDiagram2DSeriesView(pSeries,pDiagramView, TRUE)
{

}

CXTPChartSeriesPointView* CXTPChartAreaSeriesView::CreateSeriesPointView(CXTPChartDeviceContext* pDC, CXTPChartSeriesPoint* pPoint, CXTPChartElementView* pParentView)
{
	UNREFERENCED_PARAMETER(pDC);
	return new CXTPChartPointSeriesPointView(pPoint, pParentView);
}

CXTPChartColor CXTPChartAreaSeriesView::GetActualColor() const
{
	CXTPChartColor clr = m_pSeries->GetColor();
	return CXTPChartColor((BYTE)((CXTPChartAreaSeriesStyle*)GetStyle())->GetTransparency(), clr.GetR(), clr.GetG(), clr.GetB());
}

CXTPChartColor CXTPChartAreaSeriesView::GetActualColor2() const
{
	CXTPChartColor clr = m_pSeries->GetColor2();
	return CXTPChartColor((BYTE)((CXTPChartAreaSeriesStyle*)GetStyle())->GetTransparency(), clr.GetR(), clr.GetG(), clr.GetB());
}


CXTPChartColor CXTPChartAreaSeriesView::GetBorderActualColor() const
{
	CXTPChartAreaSeriesStyle* pStyle = (CXTPChartAreaSeriesStyle*)GetStyle();

	if (!pStyle->GetBorder()->GetColor().IsEmpty())
		return pStyle->GetBorder()->GetColor();

	CXTPChartColor clr = m_pSeries->GetColor().GetDarkColor();
	return clr;
}

CXTPChartDeviceCommand* CXTPChartAreaSeriesView::CreateLegendDeviceCommand(CXTPChartDeviceContext* pDC, CRect rcBounds)
{
	UNREFERENCED_PARAMETER(pDC);
	rcBounds.DeflateRect(1, 1);

	CXTPChartDeviceCommand* pCommand = new CXTPChartPolygonAntialiasingDeviceCommand();

	CXTPChartAreaSeriesStyle* pStyle = STATIC_DOWNCAST(CXTPChartAreaSeriesStyle, m_pSeries->GetStyle());

	CXTPChartPointF ptCenter(rcBounds.CenterPoint().x, rcBounds.top + 4) ;
	int nSize = 8;


	CXTPChartPoints arrPoints;
	arrPoints.Add(CXTPChartPointF(rcBounds.left, rcBounds.bottom + 1));
	arrPoints.Add(CXTPChartPointF(ptCenter.X, float(rcBounds.top + 4)));
	arrPoints.Add(CXTPChartPointF(rcBounds.right, rcBounds.bottom + 1));

	pCommand->AddChildCommand(pStyle->GetFillStyle()->CreateDeviceCommand(arrPoints, GetActualColor(), GetActualColor2()));


	CXTPChartColor clrBorder = GetBorderActualColor();
	pCommand->AddChildCommand(new CXTPChartSolidLineDeviceCommand(arrPoints[0], arrPoints[1], clrBorder, 1));
	pCommand->AddChildCommand(new CXTPChartSolidLineDeviceCommand(arrPoints[1], arrPoints[2], clrBorder, 1));

	pCommand->AddChildCommand(pStyle->GetMarker()->CreateDeviceCommand(pDC, ptCenter, nSize, m_pSeries->GetColor(), m_pSeries->GetColor2(), m_pSeries->GetColor().GetDarkColor()));

	return pCommand;
}

CXTPChartDeviceCommand* CXTPChartAreaSeriesView::CreateDeviceCommand(CXTPChartDeviceContext* pDC)
{
	CXTPChartDeviceCommand* pSeriesCommand = new CXTPChartHitTestElementCommand(m_pSeries);
	CXTPChartDeviceCommand* pCommand = pSeriesCommand->AddChildCommand(new CXTPChartPolygonAntialiasingDeviceCommand());

	CXTPChartAreaSeriesStyle* pStyle = (CXTPChartAreaSeriesStyle*)GetStyle();

	CXTPChartAxis* pAxisY = GetAxisViewY()->GetAxis();

	CXTPChartColor clrBorder = GetBorderActualColor();

	double dZero = max(0, pAxisY->GetRange()->GetMinValue());

	int nCount = m_pPointsView->GetCount();
	if (nCount > 1)
	{
		CXTPChartPointSeriesPointView* pPointView =  (CXTPChartPointSeriesPointView*)m_pPointsView->GetAt(0);

		CXTPChartPointF pointPrev = GetScreenPoint(pPointView->GetPoint()->GetInternalArgumentValue(), pPointView->m_dInternalValue);

		CXTPChartPoints arrPoints;
		arrPoints.Add(GetScreenPoint(pPointView->GetPoint()->GetInternalArgumentValue(), dZero));

		for (int i = 1; i < nCount; i++)
		{
			pPointView =  (CXTPChartPointSeriesPointView*)m_pPointsView->GetAt(i);
			CXTPChartPointF pointNext = pPointView->GetScreenPoint();

			arrPoints.Add(pointPrev);

			pointPrev = pointNext;
		}

		arrPoints.Add(pointPrev);

		pPointView =  (CXTPChartPointSeriesPointView*)m_pPointsView->GetAt(nCount - 1);

		arrPoints.Add(GetScreenPoint(pPointView->GetPoint()->GetInternalArgumentValue(), dZero));

		pCommand->AddChildCommand(pStyle->GetFillStyle()->CreateDeviceCommand(arrPoints, GetActualColor(), GetActualColor2()));


		arrPoints.RemoveAt(0);
		arrPoints.RemoveAt(arrPoints.GetSize() - 1);

		if (pStyle->GetBorder()->IsVisible())
			pCommand->AddChildCommand(new CXTPChartSolidPolylineDeviceCommand(arrPoints, clrBorder, pStyle->GetBorder()->GetThickness()));
	}

	pCommand->AddChildCommand(CXTPChartSeriesView::CreateDeviceCommand(pDC));

	return pSeriesCommand;
}


#ifdef _XTP_ACTIVEX


BEGIN_DISPATCH_MAP(CXTPChartAreaSeriesStyle, CXTPChartPointSeriesStyle)
	DISP_PROPERTY_EX_ID(CXTPChartAreaSeriesStyle, "Transparency", 105, GetTransparency, SetTransparency, VT_I4)

	DISP_PROPERTY_EX_ID(CXTPChartAreaSeriesStyle, "FillStyle", 5, OleGetFillStyle, SetNotSupported, VT_DISPATCH)
	DISP_PROPERTY_EX_ID(CXTPChartAreaSeriesStyle, "Border", 6, OleGetBorder, SetNotSupported, VT_DISPATCH)

END_DISPATCH_MAP()


// {383BCC77-27BF-4cb1-9ABF-4558D9835223}
static const GUID IID_IChartAreaSeriesStyle =
{ 0x383bcc77, 0x27bf, 0x4cb1, { 0x9a, 0xbf, 0x45, 0x58, 0xd9, 0x83, 0x52, 0x23 } };

BEGIN_INTERFACE_MAP(CXTPChartAreaSeriesStyle, CXTPChartPointSeriesStyle)
INTERFACE_PART(CXTPChartAreaSeriesStyle, IID_IChartAreaSeriesStyle, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartAreaSeriesStyle, IID_IChartAreaSeriesStyle)

// {38490E6F-92B6-4671-9613-6B2A0FBF80A8}
IMPLEMENT_OLECREATE_EX2(CXTPChartAreaSeriesStyle, "Codejock.ChartAreaSeriesStyle." _XTP_AXLIB_VERSION,
						0x38490e6f, 0x92b6, 0x4671, 0x96, 0x13, 0x6b, 0x2a, 0xf, 0xbf, 0x80, 0xa8);

LPDISPATCH CXTPChartAreaSeriesStyle::OleGetFillStyle()
{
	return XTPGetDispatch(GetFillStyle());
}

LPDISPATCH CXTPChartAreaSeriesStyle::OleGetBorder()
{
	return XTPGetDispatch(GetBorder());
}

#endif
