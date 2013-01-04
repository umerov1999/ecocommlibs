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

IMPLEMENT_SERIAL(CXTPChartLineSeriesStyle, CXTPChartPointSeriesStyle, VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)

//////////////////////////////////////////////////////////////////////////
// CXTPChartLineSeriesStyle

CXTPChartLineSeriesStyle::CXTPChartLineSeriesStyle()
{
	m_pLineStyle = new CXTPChartLineStyle(this);
	m_pLineStyle->SetThickness(2);
}

CXTPChartLineSeriesStyle::~CXTPChartLineSeriesStyle()
{
	SAFE_RELEASE(m_pLineStyle);

}

CXTPChartSeriesView* CXTPChartLineSeriesStyle::CreateView(CXTPChartSeries* pSeries, CXTPChartDiagramView* pDiagramView)
{
	return new CXTPChartLineSeriesView(pSeries, pDiagramView);
}

void CXTPChartLineSeriesStyle::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPChartPointSeriesStyle::DoPropExchange(pPX);

	CXTPPropExchangeSection sec(pPX->GetSection(_T("LineStyle")));
	m_pLineStyle->DoPropExchange(&sec);
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartLineSeriesView

CXTPChartLineSeriesView::CXTPChartLineSeriesView(CXTPChartSeries* pSeries, CXTPChartDiagramView* pDiagramView, BOOL bSortPoints)
	: CXTPChartDiagram2DSeriesView(pSeries,pDiagramView, bSortPoints)
{

}
CXTPChartSeriesPointView* CXTPChartLineSeriesView::CreateSeriesPointView(CXTPChartDeviceContext* pDC, CXTPChartSeriesPoint* pPoint, CXTPChartElementView* pParentView)
{
	UNREFERENCED_PARAMETER(pDC);
	return new CXTPChartPointSeriesPointView(pPoint, pParentView);
}

CXTPChartDeviceCommand* CXTPChartLineSeriesView::CreateDeviceCommand(CXTPChartDeviceContext* pDC)
{
	CXTPChartDeviceCommand* pSeriesCommand = new CXTPChartHitTestElementCommand(m_pSeries);
	CXTPChartDeviceCommand* pCommand = pSeriesCommand->AddChildCommand(new CXTPChartPolygonAntialiasingDeviceCommand());

	CXTPChartLineSeriesStyle* pStyle = (CXTPChartLineSeriesStyle*)GetStyle();

	int nCount = m_pPointsView->GetCount();
	if (nCount > 1)
	{
		CXTPChartPointF pointPrev;
		BOOL bValidPrev = FALSE;

		for (int i = 0; i < nCount; i++)
		{
			CXTPChartPointSeriesPointView* pPointView =  (CXTPChartPointSeriesPointView*)m_pPointsView->GetAt(i);

			if (CXTPChartMathUtils::IsNan(pPointView->m_dInternalValue))
			{
				bValidPrev = FALSE;
				continue;
			}

			CXTPChartPointF pointNext = pPointView->GetScreenPoint();

			if (bValidPrev)
			{
				pCommand->AddChildCommand(pStyle->GetLineStyle()->CreateDeviceCommand(pointPrev, pointNext, GetColor()));
			}

			pointPrev = pointNext;
			bValidPrev = TRUE;
		}
	}

	pCommand->AddChildCommand(CXTPChartSeriesView::CreateDeviceCommand(pDC));

	return pSeriesCommand;
}

CXTPChartDeviceCommand* CXTPChartLineSeriesView::CreateLegendDeviceCommand(CXTPChartDeviceContext* pDC, CRect rcBounds)
{
	UNREFERENCED_PARAMETER(pDC);
	rcBounds.DeflateRect(1, 1);

	CXTPChartDeviceCommand* pCommand = new CXTPChartDeviceCommand();

	CXTPChartPointSeriesStyle* pStyle = STATIC_DOWNCAST(CXTPChartPointSeriesStyle, m_pSeries->GetStyle());

	CXTPChartPointF ptCenter(rcBounds.CenterPoint().x, rcBounds.CenterPoint().y) ;
	int nSize = 10;

	pCommand->AddChildCommand(new CXTPChartSolidLineDeviceCommand(CXTPPoint3d(rcBounds.left,ptCenter.Y),
		CXTPPoint3d(rcBounds.right,ptCenter.Y), GetColor(), 1));
	pCommand->AddChildCommand(pStyle->GetMarker()->CreateDeviceCommand(pDC, ptCenter, nSize, GetColor(), GetColor2(), GetColor().GetDarkColor()));

	return pCommand;
}

#ifdef _XTP_ACTIVEX


BEGIN_DISPATCH_MAP(CXTPChartLineSeriesStyle, CXTPChartPointSeriesStyle)
	DISP_PROPERTY_EX_ID(CXTPChartLineSeriesStyle, "LineStyle", 100, OleGetLineStyle, SetNotSupported, VT_DISPATCH)
END_DISPATCH_MAP()


// {D33BCC77-27BF-4cb1-9ABF-4558D9835223}
static const GUID IID_IChartLineSeriesStyle =
{ 0xd33bcc77, 0x27bf, 0x4cb1, { 0x9a, 0xbf, 0x45, 0x58, 0xd9, 0x83, 0x52, 0x23 } };

BEGIN_INTERFACE_MAP(CXTPChartLineSeriesStyle, CXTPChartPointSeriesStyle)
INTERFACE_PART(CXTPChartLineSeriesStyle, IID_IChartLineSeriesStyle, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartLineSeriesStyle, IID_IChartLineSeriesStyle)

// {14490E6F-92B6-4671-9613-6B2A0FBF80A8}
IMPLEMENT_OLECREATE_EX2(CXTPChartLineSeriesStyle, "Codejock.ChartLineSeriesStyle." _XTP_AXLIB_VERSION,
	0x14490e6f, 0x92b6, 0x4671, 0x96, 0x13, 0x6b, 0x2a, 0xf, 0xbf, 0x80, 0xa8);

LPDISPATCH CXTPChartLineSeriesStyle::OleGetLineStyle()
{
	return XTPGetDispatch(m_pLineStyle);
}


#endif
