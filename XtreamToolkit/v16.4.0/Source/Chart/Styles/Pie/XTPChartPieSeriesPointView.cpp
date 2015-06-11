// XTPChartPieSeriesPointView.cpp
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
#include <math.h>

#include "Common/XTPPropExchange.h"

#include "../../Types/XTPChartTypes.h"
#include "Common/Base/Types/XTPPoint3.h"
#include "../../Types/XTPChartPie.h"


#include <Chart/XTPChartDefines.h>
#include <Chart/XTPChartElement.h>
#include <Chart/XTPChartLegendItem.h>
#include <Chart/XTPChartElementView.h>
#include <Chart/XTPChartSeries.h>
#include <Chart/XTPChartSeriesLabel.h>
#include <Chart/XTPChartSeriesView.h>
#include <Chart/XTPChartSeriesStyle.h>
#include <Chart/XTPChartSeriesPoint.h>
#include <Chart/XTPChartPanel.h>
#include <Chart/XTPChartDiagram.h>
#include <Chart/XTPChartDiagramView.h>
#include <Chart/XTPChartSeriesPointView.h>

#include <Chart/Appearance/XTPChartAppearance.h>
#include <Chart/Appearance/XTPChartBorder.h>
#include <Common/XTPMathUtils.h>
#include <Chart/Utils/XTPChartTextPainter.h>
#include <Chart/Drawing/XTPChartDeviceCommand.h>

#include "XTPChartPieDiagram.h"
#include "XTPChartPieDiagramView.h"
#include "XTPChartPieSeriesLabel.h"
#include "XTPChartPieSeriesStyle.h"
#include "XTPChartPieSeriesPointView.h"
#include "XTPChartPieSeriesView.h"


//////////////////////////////////////////////////////////////////////////
// CXTPChartPieDiagramSeriesPointView

CXTPChartPieSeriesPointView::CXTPChartPieSeriesPointView(CXTPChartSeriesPoint* pPoint, CXTPChartElementView* pParentView)
	: CXTPChartSeriesPointView(pPoint, pParentView)
{
	m_dValue = 0;
	m_pPie = NULL;
}

CXTPChartPieSeriesPointView::~CXTPChartPieSeriesPointView()
{
	SAFE_DELETE(m_pPie);
}

void CXTPChartPieSeriesPointView::CalculateLayout(double& dStartAngle)
{
	SAFE_DELETE(m_pPie);

	CXTPChartPieSeriesView* pView = (CXTPChartPieSeriesView*)GetSeriesView();

	CXTPChartPointF basePoint = pView->GetInnerBounds().GetCenter();
	CXTPChartSizeF size = pView->GetInnerBounds().GetSize();

	CXTPChartEllipse ellipse(CXTPPoint3d(), size.Width / 2, size.Height / 2);

	double pieArea = ellipse.GetArea() * m_dValue;

	double dFinishAngle = ellipse.CalcEllipseSectorFinishAngle(pieArea, dStartAngle);

	int nDepth = ((CXTPChartPieSeriesStyleBase*)pView->GetStyle())->GetDepth();
	int nHolePercent = ((CXTPChartPieSeriesStyleBase*)pView->GetStyle())->GetHolePercent();


	if (m_pPoint->m_bSpecial)
	{
		CXTPChartPieSeriesStyleBase* pStyle = ((CXTPChartPieSeriesStyleBase*)pView->GetStyle());

		CXTPChartPie pie(dStartAngle, dFinishAngle, ellipse, nDepth, nHolePercent);

		double angle = pie.GetHalfAngle();
		double radius = ellipse.CalcEllipseRadius(angle);
		double explodeX = pie.GetCenterPoint().X + pStyle->GetExplodedDistancePercent() / 100.0 * radius * cos(angle);
		double explodeY = pie.GetCenterPoint().Y - pStyle->GetExplodedDistancePercent() / 100.0 * radius * sin(angle);

		CXTPPoint3d explodeLocation(explodeX, explodeY);
		CXTPChartEllipse explodeEllipse(explodeLocation, ellipse.m_dMajorSemiaxis, ellipse.m_dMinorSemiaxis);

		m_pPie = new CXTPChartPie(dStartAngle, dFinishAngle, explodeEllipse, nDepth, nHolePercent);

	}
	else
	{
		m_pPie = new CXTPChartPie(dStartAngle, dFinishAngle, ellipse, nDepth, nHolePercent);
	}

	m_basePoint = basePoint;

	dStartAngle = dFinishAngle;
}



CXTPChartDeviceCommand* CXTPChartPieSeriesPointView::CreateDeviceCommand(CXTPChartDeviceContext* pDC)
{
	UNREFERENCED_PARAMETER(pDC);

	CXTPChartColor color1 = GetColor();
	CXTPChartColor color2 = GetColor2();

	if (!m_pPie)
		return 0;

	CXTPChartDeviceCommand* pCommand = new CXTPChartHitTestElementCommand(m_pPoint);

	pCommand->AddChildCommand(m_pPie->CreatePieDeviceCommand(color1, color2, m_basePoint));

	CXTPChartPieSeriesStyle* pStyle = DYNAMIC_DOWNCAST(CXTPChartPieSeriesStyle, GetSeriesView()->GetStyle());
	ASSERT(pStyle);

	if (pStyle->GetBorder()->IsVisible())
	{
		pCommand->AddChildCommand(m_pPie->CreateBoundedPieDeviceCommand(pStyle->GetBorderActualColor(), pStyle->GetBorder()->GetThickness(), m_basePoint));
	}

	return pCommand;
}

CXTPChartDeviceCommand* CXTPChartPieSeriesPointView::CreateLegendDeviceCommand(CXTPChartDeviceContext* pDC, CRect rcBounds)
{
	UNREFERENCED_PARAMETER(pDC);
	rcBounds.DeflateRect(1, 1);

	CXTPChartDeviceCommand* pCommand = new CXTPChartPolygonAntialiasingDeviceCommand();

	CXTPChartColor color1 = GetColor();
	CXTPChartColor color2 = GetColor2();

	CXTPChartEllipse explodeEllipse(CXTPChartPointF(rcBounds.left, rcBounds.bottom), rcBounds.Width(), rcBounds.Height());

	CXTPChartPie pie(0, CXTPMathUtils::m_dPI / 2, explodeEllipse, 0, 0);
	pCommand->AddChildCommand(pie.CreatePieDeviceCommand(color1, color2, CXTPChartPointF(0, 0)));

	CXTPChartPieSeriesStyle* pStyle = DYNAMIC_DOWNCAST(CXTPChartPieSeriesStyle, GetSeriesView()->GetStyle());
	if (pStyle)
	{
		if (pStyle->GetBorder()->IsVisible())
		{
			pCommand->AddChildCommand(pie.CreateBoundedPieDeviceCommand(pStyle->GetBorderActualColor(), 1, CXTPChartPointF(0, 0)));

		}
	}
	else
	{
		CXTPChartColor clrBorder = GetSeriesView()->GetStyle()->GetAppearance()->GetPieStyleAppearance()->BorderColor;

		pCommand->AddChildCommand(pie.CreateBoundedPieDeviceCommand(clrBorder, 1, CXTPChartPointF(0, 0)));
	}

	return pCommand;
}
