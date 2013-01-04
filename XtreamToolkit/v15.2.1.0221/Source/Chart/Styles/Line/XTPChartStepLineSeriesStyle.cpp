// XTPChartStepLineSeriesStyle.cpp
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

#include "../../Diagram/Diagram2D/XTPChartDiagram2DSeriesStyle.h"
#include "../../Diagram/Diagram2D/XTPChartDiagram2DSeriesView.h"

#include "../../Drawing/XTPChartDeviceCommand.h"
#include "../../Drawing/XTPChartLineDeviceCommand.h"
#include "../../Appearance/XTPChartLineStyle.h"
#include "../../Utils/XTPChartMathUtils.h"

#include "../Point/XTPChartPointSeriesStyle.h"
#include "../Point/XTPChartPointSeriesView.h"
#include "../Point/XTPChartMarker.h"

#include "XTPChartLineSeriesStyle.h"
#include "XTPChartStepLineSeriesStyle.h"


IMPLEMENT_SERIAL(CXTPChartStepLineSeriesStyle, CXTPChartLineSeriesStyle, VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)

//////////////////////////////////////////////////////////////////////////
// CXTPChartStepLineSeriesStyle

CXTPChartStepLineSeriesStyle::CXTPChartStepLineSeriesStyle()
{
	m_bInvertedStep = FALSE;
}

CXTPChartStepLineSeriesStyle::~CXTPChartStepLineSeriesStyle()
{

}

CXTPChartSeriesView* CXTPChartStepLineSeriesStyle::CreateView(CXTPChartSeries* pSeries, CXTPChartDiagramView* pDiagramView)
{
	return new CXTPChartStepLineSeriesView(pSeries, pDiagramView);
}

void CXTPChartStepLineSeriesStyle::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPChartLineSeriesStyle::DoPropExchange(pPX);

	PX_Bool(pPX, _T("InvertedStep"), m_bInvertedStep, FALSE);
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartStepLineSeriesView

CXTPChartStepLineSeriesView::CXTPChartStepLineSeriesView(CXTPChartSeries* pSeries, CXTPChartDiagramView* pDiagramView)
	: CXTPChartDiagram2DSeriesView(pSeries,pDiagramView, TRUE)
{

}
CXTPChartSeriesPointView* CXTPChartStepLineSeriesView::CreateSeriesPointView(CXTPChartDeviceContext* pDC, CXTPChartSeriesPoint* pPoint, CXTPChartElementView* pParentView)
{
	UNREFERENCED_PARAMETER(pDC);
	return new CXTPChartStepLineSeriesPointView(pPoint, pParentView);
}

CXTPChartDeviceCommand* CXTPChartStepLineSeriesView::CreateDeviceCommand(CXTPChartDeviceContext* pDC)
{
	CXTPChartDeviceCommand* pSeriesCommand = new CXTPChartHitTestElementCommand(m_pSeries);
	CXTPChartDeviceCommand* pCommand = pSeriesCommand;

	CXTPChartStepLineSeriesStyle* pStyle = (CXTPChartStepLineSeriesStyle*)GetStyle();

	int nCount = m_pPointsView->GetCount();
	if (nCount > 1)
	{
		CXTPChartPointF pointPrev;
		CXTPChartPointSeriesPointView* pPointPrev = NULL;

		for (int i = 0; i < nCount; i++)
		{
			CXTPChartPointSeriesPointView* pPointView =  (CXTPChartPointSeriesPointView*)m_pPointsView->GetAt(i);

			if (CXTPChartMathUtils::IsNan(pPointView->m_dInternalValue))
			{
				pPointPrev = NULL;
				continue;
			}

			CXTPChartPointF pointNext = pPointView->GetScreenPoint();

			if (pPointPrev)
			{
				CXTPChartPointF pointMiddle;
				if (!pStyle->GetInvertedStep())
				{
					pointMiddle= GetScreenPoint(pPointView->GetPoint()->GetInternalArgumentValue(),
						pPointPrev->m_dInternalValue);
				}
				else
				{
					pointMiddle = GetScreenPoint(pPointPrev->GetPoint()->GetInternalArgumentValue(),
						pPointView->m_dInternalValue);

				}

				pCommand->AddChildCommand(pStyle->GetLineStyle()->CreateDeviceCommand(pointPrev, pointMiddle, GetColor()));

				pCommand->AddChildCommand(pStyle->GetLineStyle()->CreateDeviceCommand(pointMiddle, pointNext, GetColor()));
			}

			pointPrev = pointNext;
			pPointPrev = pPointView;
		}
	}

	pCommand->AddChildCommand(CXTPChartSeriesView::CreateDeviceCommand(pDC));

	return pSeriesCommand;
}

CXTPChartDeviceCommand* CXTPChartStepLineSeriesView::CreateLegendDeviceCommand(CXTPChartDeviceContext* pDC, CRect rcBounds)
{
	UNREFERENCED_PARAMETER(pDC);
	rcBounds.DeflateRect(1, 1);

	CXTPChartDeviceCommand* pCommand = new CXTPChartDeviceCommand();

	CXTPChartPointSeriesStyle* pStyle = STATIC_DOWNCAST(CXTPChartPointSeriesStyle, GetSeries()->GetStyle());

	CXTPChartPointF ptCenter(rcBounds.CenterPoint().x, rcBounds.CenterPoint().y) ;
	int nSize = 8;

	pCommand->AddChildCommand(new CXTPChartSolidLineDeviceCommand(CXTPPoint3d(rcBounds.left, rcBounds.bottom),
		CXTPPoint3d(rcBounds.left, ptCenter.Y), GetColor(), 1));
	pCommand->AddChildCommand(new CXTPChartSolidLineDeviceCommand(CXTPPoint3d(rcBounds.left, ptCenter.Y),
		CXTPPoint3d(rcBounds.right, ptCenter.Y), GetColor(), 1));
	pCommand->AddChildCommand(new CXTPChartSolidLineDeviceCommand(CXTPPoint3d(rcBounds.right, ptCenter.Y),
		CXTPPoint3d(rcBounds.right, rcBounds.top), GetColor(), 1));

	pCommand->AddChildCommand(pStyle->GetMarker()->CreateDeviceCommand(pDC, ptCenter, nSize, GetColor(), GetColor2(), GetColor().GetDarkColor()));

	return pCommand;
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartStepLineSeriesPointView

CXTPChartStepLineSeriesPointView::CXTPChartStepLineSeriesPointView(CXTPChartSeriesPoint* pPoint, CXTPChartElementView* pParentView)
	: CXTPChartPointSeriesPointView(pPoint, pParentView)
{

}

CXTPChartDeviceCommand* CXTPChartStepLineSeriesPointView::CreateLegendDeviceCommand(CXTPChartDeviceContext* pDC, CRect rcBounds)
{
	UNREFERENCED_PARAMETER(pDC);
	rcBounds.DeflateRect(1, 1);

	CXTPChartDeviceCommand* pCommand = new CXTPChartDeviceCommand();

	CXTPChartPointSeriesStyle* pStyle = STATIC_DOWNCAST(CXTPChartPointSeriesStyle, GetSeriesView()->GetSeries()->GetStyle());

	CXTPChartPointF ptCenter(rcBounds.CenterPoint().x, rcBounds.CenterPoint().y) ;
	int nSize = 8;

	pCommand->AddChildCommand(new CXTPChartSolidLineDeviceCommand(CXTPPoint3d(rcBounds.left, rcBounds.bottom),
		CXTPPoint3d(rcBounds.left, ptCenter.Y), GetSeriesView()->GetColor(), 1));
	pCommand->AddChildCommand(new CXTPChartSolidLineDeviceCommand(CXTPPoint3d(rcBounds.left, ptCenter.Y),
		CXTPPoint3d(rcBounds.right, ptCenter.Y), GetSeriesView()->GetColor(), 1));
	pCommand->AddChildCommand(new CXTPChartSolidLineDeviceCommand(CXTPPoint3d(rcBounds.right, ptCenter.Y),
		CXTPPoint3d(rcBounds.right, rcBounds.top), GetSeriesView()->GetColor(), 1));

	pCommand->AddChildCommand(pStyle->GetMarker()->CreateDeviceCommand(pDC, ptCenter, nSize, GetColor(), GetColor2(), GetColor().GetDarkColor()));

	return pCommand;
}

#ifdef _XTP_ACTIVEX


BEGIN_DISPATCH_MAP(CXTPChartStepLineSeriesStyle, CXTPChartLineSeriesStyle)
	DISP_PROPERTY_EX_ID(CXTPChartStepLineSeriesStyle, "InvertedStep", 200, GetInvertedStep, SetInvertedStep, VT_BOOL)
END_DISPATCH_MAP()


// {771BCC77-27BF-4cb1-9ABF-4558D9835223}
static const GUID IID_IChartStepLineSeriesStyle =
{ 0x771bcc77, 0x27bf, 0x4cb1, { 0x9a, 0xbf, 0x45, 0x58, 0xd9, 0x83, 0x52, 0x23 } };

BEGIN_INTERFACE_MAP(CXTPChartStepLineSeriesStyle, CXTPChartLineSeriesStyle)
	INTERFACE_PART(CXTPChartStepLineSeriesStyle, IID_IChartStepLineSeriesStyle, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartStepLineSeriesStyle, IID_IChartStepLineSeriesStyle)

// {77190E6F-92B6-4671-9613-6B2A0FBF80A8}
IMPLEMENT_OLECREATE_EX2(CXTPChartStepLineSeriesStyle, "Codejock.ChartStepLineSeriesStyle." _XTP_AXLIB_VERSION,
	0x77190e6f, 0x92b6, 0x4671, 0x96, 0x13, 0x6b, 0x2a, 0xf, 0xbf, 0x80, 0xa8);


#endif
