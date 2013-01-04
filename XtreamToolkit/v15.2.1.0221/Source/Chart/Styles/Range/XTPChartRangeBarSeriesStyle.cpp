// XTPChartRangeBarSeriesStyle.cpp
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


#include "../../XTPChartSeriesPoint.h"

#include "../../Diagram/Axis/XTPChartAxisView.h"
#include "../../Diagram/Diagram2D/XTPChartDiagram2DSeriesStyle.h"
#include "../../Diagram/Diagram2D/XTPChartDiagram2DSeriesView.h"
#include "../../Diagram/Diagram2D/XTPChartDiagram2DSeriesLabel.h"

#include "../../Drawing/XTPChartDeviceCommand.h"


#include "../Point/XTPChartMarker.h"

#include "../Bar/XTPChartBarSeriesStyle.h"
#include "../Bar/XTPChartBarSeriesView.h"

#include "XTPChartRangeBarSeriesStyle.h"
#include "XTPChartRangeBarSeriesLabel.h"
#include "XTPChartRangeBarSeriesView.h"


//////////////////////////////////////////////////////////////////////////
// CXTPChartRangeBarSeriesStyle

IMPLEMENT_SERIAL(CXTPChartRangeBarSeriesStyle, CXTPChartBarSeriesStyle, VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT);

CXTPChartRangeBarSeriesStyle::CXTPChartRangeBarSeriesStyle()
{
	m_pMinValueMarker = new CXTPChartMarker(this);
	m_pMinValueMarker->SetVisible(FALSE);

	m_pMaxValueMarker = new CXTPChartMarker(this);
	m_pMaxValueMarker->SetVisible(FALSE);

	SetLabel(new CXTPChartRangeBarSeriesLabel());

}

CXTPChartRangeBarSeriesStyle::~CXTPChartRangeBarSeriesStyle()
{
	SAFE_RELEASE(m_pMinValueMarker);
	SAFE_RELEASE(m_pMaxValueMarker);
}


void CXTPChartRangeBarSeriesStyle::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPChartBarSeriesStyle::DoPropExchange(pPX);

}


CXTPChartSeriesView* CXTPChartRangeBarSeriesStyle::CreateView(CXTPChartSeries* pSeries, CXTPChartDiagramView* pDiagramView)
{
	return new CXTPChartRangeBarSeriesView(pSeries, pDiagramView);
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartRangeBarSeriesView

CXTPChartRangeBarSeriesView::CXTPChartRangeBarSeriesView(CXTPChartSeries* pSeries, CXTPChartDiagramView* pDiagramView)
	: CXTPChartBarSeriesView(pSeries,pDiagramView)
{
}

CXTPChartSeriesPointView* CXTPChartRangeBarSeriesView::CreateSeriesPointView(CXTPChartDeviceContext* pDC, CXTPChartSeriesPoint* pPoint, CXTPChartElementView* pParentView)
{
	UNREFERENCED_PARAMETER(pDC);
	return new CXTPChartRangeBarSeriesPointView(pPoint, pParentView);
}


//////////////////////////////////////////////////////////////////////////
// CXTPChartRangeBarSeriesPointView

CXTPChartRangeBarSeriesPointView::CXTPChartRangeBarSeriesPointView(CXTPChartSeriesPoint* pPoint, CXTPChartElementView* pParentView)
	: CXTPChartBarSeriesPointView(pPoint, pParentView)
{

}

CXTPChartRectF CXTPChartRangeBarSeriesPointView::GetScreenRect() const
{
	CXTPChartRangeBarSeriesView* pSeriesView = (CXTPChartRangeBarSeriesView*)GetSeriesView();
	CXTPChartRangeBarSeriesStyle* pSeriesStyle = (CXTPChartRangeBarSeriesStyle*)GetSeriesView()->GetStyle();

	CXTPChartDiagram2DAxisView* pAxisXView = pSeriesView->GetAxisViewX();

	if (pSeriesView->m_nBarIndex != -1)
	{
		int nCount = pSeriesView->m_nBarCount;
		int nIndex = pSeriesView->m_nBarIndex;

		double dOffset = (1 + pSeriesStyle->GetBarDistanceFixed()) / pAxisXView->GetScale();

		double dRangeBarWidth = (pSeriesStyle->GetBarWidth() - dOffset * (nCount - 1)) / nCount;

		CXTPChartPointF ptTopLeft(pSeriesView->GetScreenPoint(m_pPoint->GetInternalArgumentValue() -
			pSeriesStyle->GetBarWidth() / 2 + dRangeBarWidth * nIndex + dOffset * nIndex, m_pPoint->GetValue(0)));

		CXTPChartPointF ptBottomRight(pSeriesView->GetScreenPoint(m_pPoint->GetInternalArgumentValue() -
			pSeriesStyle->GetBarWidth() / 2 + dRangeBarWidth * nIndex + dOffset * nIndex + dRangeBarWidth, m_pPoint->GetValue(1)));

		CXTPChartRectF rc(ptTopLeft, ptBottomRight);
		rc.Width++;
		rc.Height++;

		return rc;
	}

	return CXTPChartRectF(0, 0, 0, 0);
}

CXTPChartPointF CXTPChartRangeBarSeriesPointView::GetScreenPoint(BOOL bMinValue) const
{
	CXTPChartRangeBarSeriesView* pSeriesView = (CXTPChartRangeBarSeriesView*)GetSeriesView();
	CXTPChartRangeBarSeriesStyle* pSeriesStyle = (CXTPChartRangeBarSeriesStyle*)GetSeriesView()->GetStyle();

	CXTPChartDiagram2DAxisView* pAxisXView = pSeriesView->GetAxisViewX();

	if (pSeriesView->m_nBarIndex != -1)
	{
		int nCount = pSeriesView->m_nBarCount;
		int nIndex = pSeriesView->m_nBarIndex;

		double dOffset = (1 + pSeriesStyle->GetBarDistanceFixed()) / pAxisXView->GetScale();

		double dRangeBarWidth = (pSeriesStyle->GetBarWidth() - dOffset * (nCount - 1)) / nCount;

		CXTPChartPointF pt(pSeriesView->GetScreenPoint(m_pPoint->GetInternalArgumentValue() -
			pSeriesStyle->GetBarWidth() / 2 + dRangeBarWidth / 2 + dRangeBarWidth * nIndex + dOffset * nIndex, m_pPoint->GetValue(bMinValue ? 0 : 1)));

		return pt;
	}

	return CXTPChartPointF(0, 0);
}

CXTPChartDeviceCommand* CXTPChartRangeBarSeriesPointView::CreateDeviceCommand(CXTPChartDeviceContext* pDC)
{
	CXTPChartDeviceCommand* pCommand = CXTPChartBarSeriesPointView::CreateDeviceCommand(pDC);

	CXTPChartColor color = GetColor();
	CXTPChartColor color2 = GetColor2();
	CXTPChartColor colorBorder = GetColor().GetDarkColor();


	CXTPChartPointF point = GetScreenPoint(TRUE);
	pCommand->AddChildCommand(((CXTPChartRangeBarSeriesStyle*)GetSeriesView()->GetStyle())->GetMinValueMarker()->CreateDeviceCommand(pDC, point, color, color2, colorBorder));


	point = GetScreenPoint(FALSE);
	pCommand->AddChildCommand(((CXTPChartRangeBarSeriesStyle*)GetSeriesView()->GetStyle())->GetMaxValueMarker()->CreateDeviceCommand(pDC, point, color, color2, colorBorder));


	return pCommand;
}

void CXTPChartRangeBarSeriesPointView::UpdateMinMaxRange(double& nMinValue, double& nMaxValue) const
{
	if (m_pPoint->GetValueLength() > 1)
	{
		double nValue = m_pPoint->GetValue(1);

		nMaxValue = max(nMaxValue, nValue);
		nMinValue = min(nMinValue, nValue);
	}
}




#ifdef _XTP_ACTIVEX


BEGIN_DISPATCH_MAP(CXTPChartRangeBarSeriesStyle, CXTPChartBarSeriesStyle)
	DISP_PROPERTY_EX_ID(CXTPChartRangeBarSeriesStyle, "MinValueMarker", 60, OleGetMinValueMarker, SetNotSupported, VT_DISPATCH)
	DISP_PROPERTY_EX_ID(CXTPChartRangeBarSeriesStyle, "MaxValueMarker", 61, OleGetMaxValueMarker, SetNotSupported, VT_DISPATCH)
END_DISPATCH_MAP()


// {765BCC77-27BF-4cb1-9ABF-4558D9835223}
static const GUID IID_IChartRangeBarSeriesStyle =
{ 0x765bcc77, 0x27bf, 0x4cb1, { 0x9a, 0xbf, 0x45, 0x58, 0xd9, 0x83, 0x52, 0x23 } };

BEGIN_INTERFACE_MAP(CXTPChartRangeBarSeriesStyle, CXTPChartBarSeriesStyle)
	INTERFACE_PART(CXTPChartRangeBarSeriesStyle, IID_IChartRangeBarSeriesStyle , Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartRangeBarSeriesStyle, IID_IChartRangeBarSeriesStyle )

// {76590E6F-92B6-4671-9613-6B2A0FBF80A8}
IMPLEMENT_OLECREATE_EX2(CXTPChartRangeBarSeriesStyle, "Codejock.ChartRangeBarSeriesStyle." _XTP_AXLIB_VERSION,
0x76590e6f, 0x92b6, 0x4671, 0x96, 0x13, 0x6b, 0x2a, 0xf, 0xbf, 0x80, 0xa8);



LPDISPATCH CXTPChartRangeBarSeriesStyle::OleGetMinValueMarker()
{
	return XTPGetDispatch(m_pMinValueMarker);
}


LPDISPATCH CXTPChartRangeBarSeriesStyle::OleGetMaxValueMarker()
{
	return XTPGetDispatch(m_pMaxValueMarker);
}

#endif
