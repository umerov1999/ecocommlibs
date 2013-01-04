// XTPChartPieDiagram.cpp
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
#include <math.h>

#include "Common/XTPPropExchange.h"

#include "../../Types/XTPChartTypes.h"
#include "Common/Base/Types/XTPPoint3.h"
#include "../../Types/XTPChartPie.h"


#include "../../XTPChartDefines.h"
#include "../../XTPChartElement.h"
#include "../../XTPChartElementView.h"
#include "../../XTPChartSeries.h"
#include "../../XTPChartSeriesLabel.h"
#include "../../XTPChartSeriesView.h"
#include "../../XTPChartSeriesStyle.h"
#include "../../XTPChartSeriesPoint.h"
#include "../../XTPChartPanel.h"
#include "../../XTPChartDiagram.h"
#include "../../XTPChartDiagramView.h"

#include "../../Appearance/XTPChartAppearance.h"
#include "../../Appearance/XTPChartBorder.h"
#include "../../Utils/XTPChartMathUtils.h"
#include "../../Utils/XTPChartTextPainter.h"
#include "../../Drawing/XTPChartDeviceCommand.h"

#include "XTPChartPieDiagram.h"
#include "XTPChartPieDiagramView.h"
#include "XTPChartPieSeriesLabel.h"
#include "XTPChartPieSeriesStyle.h"


//////////////////////////////////////////////////////////////////////////
// CXTPChartPieDiagram

IMPLEMENT_SERIAL(CXTPChartPieDiagram, CXTPChartDiagram, VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)

CXTPChartPieDiagram::CXTPChartPieDiagram()
{
#ifdef _XTP_ACTIVEX
	EnableAutomation();
#endif
}

CXTPChartDiagramView* CXTPChartPieDiagram::CreateView(CXTPChartDeviceContext* pDC, CXTPChartElementView* pParent)
{
	UNREFERENCED_PARAMETER(pDC);
	return new CXTPChartPieDiagramView(this, pParent);
}

void CXTPChartPieDiagram::CalculateSeriesLayout(CXTPChartDeviceContext* pDC, CXTPChartDiagramView* pView)
{
	if (pView->GetCount() == 0)
		return;

	for (int i = 0; i < pView->GetSeriesView()->GetCount(); i++)
	{
		CXTPChartPieSeriesView* pSeriesView = (CXTPChartPieSeriesView*)pView->GetSeriesView()->GetAt(i);

		pSeriesView->CalculatePointLayout(pDC, pView->GetBounds());
		pSeriesView->CalculateLabelLayout(pDC);
	}
}


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
// CXTPChartPieDiagramSeriesView

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

	double circleStartAngle = CXTPChartMathUtils::Degree2Radian(((CXTPChartPieSeriesStyleBase*)GetStyle())->GetRotation());

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

	CXTPChartColor color1 = GetColor();
	CXTPChartColor color2 = GetColor2();

	CXTPChartEllipse explodeEllipse(CXTPChartPointF(rcBounds.left, rcBounds.bottom), rcBounds.Width(), rcBounds.Height());

	CXTPChartPie pie(0, CXTPChartMathUtils::m_dPI / 2, explodeEllipse, 0, 0);
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

	CXTPChartPie pie(0, CXTPChartMathUtils::m_dPI / 2, explodeEllipse, 0, 0);
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



//////////////////////////////////////////////////////////////////////////
// CXTPChartPieDiagramView

CXTPChartPieDiagramView::CXTPChartPieDiagramView(CXTPChartDiagram* pDiagram, CXTPChartElementView* pParent)
	: CXTPChartDiagramView(pDiagram, pParent)
{
	m_pSeriesView = new CXTPChartElementView(this);

	m_pLabelsView = new CXTPChartPieSeriesLabelsView(this);

}


#ifdef _XTP_ACTIVEX


BEGIN_DISPATCH_MAP(CXTPChartPieDiagram, CXTPChartDiagram)
END_DISPATCH_MAP()

// {786BCC77-27BF-4cb1-9ABF-4558D9835223}
static const GUID IID_IChartPieDiagram =
{ 0x786bcc77, 0x27bf, 0x4cb1, { 0x9a, 0xbf, 0x45, 0x58, 0xd9, 0x83, 0x52, 0x23 } };

BEGIN_INTERFACE_MAP(CXTPChartPieDiagram, CXTPChartDiagram)
	INTERFACE_PART(CXTPChartPieDiagram, IID_IChartPieDiagram, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartPieDiagram, IID_IChartPieDiagram)

// {78690E6F-92B6-4671-9613-6B2A0FBF80A8}
IMPLEMENT_OLECREATE_EX2(CXTPChartPieDiagram, "Codejock.ChartPieDiagram." _XTP_AXLIB_VERSION,
						0x78690e6f, 0x92b6, 0x4671, 0x96, 0x13, 0x6b, 0x2a, 0xf, 0xbf, 0x80, 0xa8);

#endif
