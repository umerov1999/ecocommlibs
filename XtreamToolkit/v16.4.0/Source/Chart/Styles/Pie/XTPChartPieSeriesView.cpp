// XTPChartPieSeriesView.cpp
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

#include "StdAfx.h"
#include <math.h>

#include <Common/XTPPropExchange.h>
#include <Common/XTPMathUtils.h>

#include <Chart/Types/XTPChartTypes.h>
#include <Chart/Types/XTPChartPie.h>

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
#include <Chart/Utils/XTPChartTextPainter.h>
#include <Chart/Drawing/XTPChartDeviceCommand.h>

#include "XTPChartPieDiagram.h"
#include "XTPChartPieDiagramView.h"
#include "XTPChartPieSeriesLabel.h"
#include "XTPChartPieSeriesStyle.h"
#include "XTPChartPieSeriesPointView.h"
#include "XTPChartPieSeriesView.h"


class CXTPChartPieDiagramDomain : public CXTPChartDiagramDomain
{
public:
	CXTPChartPieDiagramDomain(CXTPChartRectF rcInnerBounds);

protected:
	virtual CXTPChartRectF GetInnerBounds() const;

protected:
	CXTPChartRectF m_rcInnerBounds;
};

CXTPChartPieDiagramDomain::CXTPChartPieDiagramDomain(CXTPChartRectF rcInnerBounds)
{
	m_rcInnerBounds = rcInnerBounds;
}

CXTPChartRectF CXTPChartPieDiagramDomain::GetInnerBounds() const
{
	return m_rcInnerBounds;
}


//////////////////////////////////////////////////////////////////////////
// CXTPChartPieSeriesView

CXTPChartPieSeriesView::CXTPChartPieSeriesView(CXTPChartSeries* pSeries, CXTPChartDiagramView* pDiagramView)
	: CXTPChartSeriesView(pSeries, pDiagramView)
{
	m_pDomain = NULL;

	m_nMaxLabelWidth = 0;

	m_rcBounds.SetRectEmpty();
}

CXTPChartPieSeriesView::~CXTPChartPieSeriesView()
{
	SAFE_DELETE(m_pDomain);
}

void CXTPChartPieSeriesView::CalculatePointLayout(CXTPChartDeviceContext* pDC, CRect rcBounds)
{
	SAFE_DELETE(m_pDomain);

	UNREFERENCED_PARAMETER(pDC);

	CalculateValues();

	m_pDomain = CreateDiagramDomain(pDC, rcBounds);

	double circleStartAngle = CXTPMathUtils::Degree2Radian(((CXTPChartPieSeriesStyleBase*)GetStyle())->GetRotation());

	CXTPChartEllipse ellipse(CXTPPoint3d(), m_pDomain->GetInnerBounds().Width / 2, m_pDomain->GetInnerBounds().Height / 2);

	double dAngle = ellipse.CalcEllipseAngleFromCircleAngle(circleStartAngle);


	for (int i = 0; i < m_pPointsView->GetCount(); i++)
	{
		CXTPChartPieSeriesPointView* pPointView = (CXTPChartPieSeriesPointView*)m_pPointsView->GetAt(i);

		pPointView->CalculateLayout(dAngle);
	}
}


CXTPChartDiagramDomain* CXTPChartPieSeriesView::CreateDiagramDomain(CXTPChartDeviceContext* pDC, CRect rcBounds)
{
	m_rcBounds = rcBounds;

	CRect rcInnerBounds = rcBounds;

	CXTPChartPieSeriesStyleBase* pStyle = (CXTPChartPieSeriesStyleBase*)m_pSeries->GetStyle();
	CXTPChartPieSeriesLabel* pLabel = (CXTPChartPieSeriesLabel*)pStyle->GetLabel();

	if (pLabel->IsVisible() && !pLabel->IsInside())
	{
		CSize sz(0, 0);
		BOOL bHasExanded = FALSE;
		for (int i = 0; i < m_pPointsView->GetCount(); i++)
		{
			CXTPChartPieSeriesPointView* pPointView = (CXTPChartPieSeriesPointView*)m_pPointsView->GetAt(i);

			CXTPChartString text(pLabel->GetPointLabel(pPointView->GetPoint()));
			CXTPChartTextPainter painter(pDC, text, pLabel);

			sz.cx = max(sz.cx, (INT)painter.GetSize().Width);
			sz.cy = max(sz.cy, (INT)painter.GetSize().Height);

			if (pPointView->GetPoint()->m_bSpecial)
				bHasExanded = TRUE;
		}


		int nLineLength = pStyle->GetLabel()->GetLineLength();

		int nGap = 6;

		if (bHasExanded)
		{
			nGap += min(rcInnerBounds.Width(), rcInnerBounds.Height()) * pStyle->GetExplodedDistancePercent() / 2 / 100;
		}

		rcInnerBounds.DeflateRect(nGap + nLineLength + sz.cx, nGap + nLineLength + sz.cy);

		m_nMaxLabelWidth = nGap + sz.cx;
	}
	else
	{
		rcInnerBounds.DeflateRect(10, 10);
	}

	int nWidth = min(rcInnerBounds.Width(), rcInnerBounds.Height());

	if (nWidth < 0)
	{
		nWidth = 1;
		rcInnerBounds = CRect(rcBounds.CenterPoint(), CSize(nWidth, nWidth));
	}
	int nHeight = nWidth;



	CXTPChartRectF rcInnerBoundsF(float(rcInnerBounds.left + rcInnerBounds.right - nWidth) / 2,
		float(rcInnerBounds.top + rcInnerBounds.bottom - nHeight) / 2, (float)nWidth, (float)nHeight);

	return new CXTPChartPieDiagramDomain(rcInnerBoundsF);

}

void CXTPChartPieSeriesView::CalculateLabelLayout(CXTPChartDeviceContext* pDC)
{
	for (int i = 0; i < m_pLabelsView->GetCount(); i++)
	{
		CXTPChartSeriesLabelView* pLabelView = (CXTPChartSeriesLabelView*)m_pLabelsView->GetAt(i);

		pLabelView->CalculateLayout(pDC);
	}

	((CXTPChartPieSeriesLabelsView*)m_pLabelsView)->ResolveOverlapping();
}

CXTPChartRectF CXTPChartPieSeriesView::GetInnerBounds() const
{
	return m_pDomain->GetInnerBounds();
}

CXTPChartSeriesPointView* CXTPChartPieSeriesView::CreateSeriesPointView(CXTPChartDeviceContext* pDC, CXTPChartSeriesPoint* pPoint, CXTPChartElementView* pParentView)
{
	UNREFERENCED_PARAMETER(pDC);
	return new CXTPChartPieSeriesPointView(pPoint, pParentView);
}


void CXTPChartPieSeriesView::CalculateValues()
{
	if (m_pPointsView->GetCount() == 1)
	{
		CXTPChartPieSeriesPointView* pPointView = (CXTPChartPieSeriesPointView*)m_pPointsView->GetAt(0);
		pPointView->m_dValue = 1;
		return;
	}

	double dSum = 0;
	int i;

	for (i = 0; i < m_pPointsView->GetCount(); i++)
	{
		CXTPChartPieSeriesPointView* pPointView = (CXTPChartPieSeriesPointView*)m_pPointsView->GetAt(i);

		CXTPChartSeriesPoint* pPoint = pPointView->GetPoint();

		double dValue = pPoint->GetValue(0);

		if (dValue >= 0)
			dSum += dValue;
	}

	if (dSum == 0)
		dSum = 1;

	for (i = 0; i < m_pPointsView->GetCount(); i++)
	{
		CXTPChartPieSeriesPointView* pPointView = (CXTPChartPieSeriesPointView*)m_pPointsView->GetAt(i);

		CXTPChartSeriesPoint* pPoint = pPointView->GetPoint();

		double dValue = pPoint->GetValue(0);

		pPointView->m_dValue = dValue >= 0 ? dValue / dSum : 0;
	}

}

CXTPChartDeviceCommand* CXTPChartPieSeriesView::CreateDeviceCommand(CXTPChartDeviceContext* pDC)
{
	CXTPChartDeviceCommand* pCommand = new CXTPChartPolygonAntialiasingDeviceCommand();
	pCommand->AddChildCommand(CXTPChartSeriesView::CreateDeviceCommand(pDC));

	return pCommand;
}

CXTPChartDeviceCommand* CXTPChartPieSeriesView::CreateLegendDeviceCommand(CXTPChartDeviceContext* pDC, CRect rcBounds)
{
	UNREFERENCED_PARAMETER(pDC);
	rcBounds.DeflateRect(1, 1);

	CXTPChartDeviceCommand* pCommand = new CXTPChartPolygonAntialiasingDeviceCommand();

	CXTPChartColor color1 = m_pSeries->GetColor();
	CXTPChartColor color2 = m_pSeries->GetColor2();

	CXTPChartEllipse explodeEllipse(CXTPChartPointF(rcBounds.left, rcBounds.bottom), rcBounds.Width(), rcBounds.Height());

	CXTPChartPie pie(0, CXTPMathUtils::m_dPI / 2, explodeEllipse, 0, 0);
	pCommand->AddChildCommand(pie.CreatePieDeviceCommand(color1, color2, CXTPChartPointF(0, 0)));

	CXTPChartPieSeriesStyle* pStyle = DYNAMIC_DOWNCAST(CXTPChartPieSeriesStyle, GetStyle());
	if (pStyle)
	{
		if (pStyle->GetBorder()->IsVisible())
		{
			pCommand->AddChildCommand(pie.CreateBoundedPieDeviceCommand(pStyle->GetBorderActualColor(), 1, CXTPChartPointF(0, 0)));

		}
	}
	else
	{
		CXTPChartColor clrBorder = GetStyle()->GetAppearance()->GetPieStyleAppearance()->BorderColor;

		pCommand->AddChildCommand(pie.CreateBoundedPieDeviceCommand(clrBorder, 1, CXTPChartPointF(0, 0)));
	}

	return pCommand;
}
