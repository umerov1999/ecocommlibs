// XTPChartRadarPointSeriesLabel.cpp
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

#include "../../XTPChartDefines.h"
#include "../../XTPChartElement.h"
#include "../../XTPChartElementView.h"
#include "../../XTPChartPanel.h"
#include "../../XTPChartDiagramView.h"
#include "../../XTPChartSeriesLabel.h"
#include "../../XTPChartSeriesView.h"

#include "../../Utils/XTPChartMathUtils.h"
#include "../../Utils/XTPChartTextPainter.h"
#include "../../Drawing/XTPChartDeviceCommand.h"
#include "../../Appearance/XTPChartBorder.h"
#include "../../Appearance/XTPChartFillStyle.h"

#include "../../Diagram/RadarDiagram/XTPChartRadarDiagramView.h"
#include "../../Diagram/RadarDiagram/XTPChartRadarDiagramSeriesView.h"

#include "XTPChartRadarPointSeriesLabel.h"
#include "XTPChartRadarPointSeriesView.h"


//////////////////////////////////////////////////////////////////////////
// CXTPChartRadarPointSeriesLabel
IMPLEMENT_SERIAL(CXTPChartRadarPointSeriesLabel, CXTPChartSeriesLabel, VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)

CXTPChartRadarPointSeriesLabel::CXTPChartRadarPointSeriesLabel()
{
	m_nAngle = 45;
}

CXTPChartRadarPointSeriesLabel::~CXTPChartRadarPointSeriesLabel()
{

}

void CXTPChartRadarPointSeriesLabel::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPChartSeriesLabel::DoPropExchange(pPX);

	PX_Int(pPX, _T("Angle"), m_nAngle, 45);
}

CXTPChartElementView* CXTPChartRadarPointSeriesLabel::CreateView(CXTPChartDeviceContext* pDC, CXTPChartSeriesPointView* pPointView, CXTPChartElementView* pParentView)
{
	UNREFERENCED_PARAMETER(pDC);
	return new CXTPChartRadarPointSeriesLabelView(this, pPointView, pParentView);
}


//////////////////////////////////////////////////////////////////////////
// CXTPChartRadarPointSeriesLabelView

CXTPChartRadarPointSeriesLabelView::CXTPChartRadarPointSeriesLabelView(CXTPChartSeriesLabel* pLabel, CXTPChartSeriesPointView* pPointView, CXTPChartElementView* pParentView)
	: CXTPChartSeriesLabelView(pLabel, pPointView, pParentView)
{
}

CRect CXTPChartRadarPointSeriesLabelView::GetPaneRect() const
{
	CXTPChartSeriesView* pSeriseView = m_pPointView->GetSeriesView();

	CXTPChartRadarDiagramView* pDiagramView = (CXTPChartRadarDiagramView*)pSeriseView->GetDiagramView();

	return pDiagramView->GetBounds();
}

CXTPChartPointF CXTPChartRadarPointSeriesLabelView::GetAnchorPoint() const
{
	return ((CXTPChartRadarPointSeriesPointView*)m_pPointView)->GetScreenPoint();
}

double CXTPChartRadarPointSeriesLabelView::GetAnchorAngle() const
{
	double lineAngle = ((CXTPChartRadarPointSeriesLabel*)m_pLabel)->GetAngle();

	return lineAngle;
}

void CXTPChartRadarPointSeriesLabelView::CalculateLayout(CXTPChartDeviceContext* pDC)
{
	CXTPChartString text(m_pLabel->GetPointLabel(m_pPointView->GetPoint()));
	CXTPChartPointF ptAnchorPoint = GetAnchorPoint();

	CalculateLayout(pDC, ptAnchorPoint, GetAnchorAngle(), text);
}

void CXTPChartRadarPointSeriesLabelView::CalculateLayout(CXTPChartDeviceContext* pDC, const CXTPChartPointF& ptAnchorPoint, double dAnchorAngle, const CXTPChartString& text)
{
	double dLineAngle = CXTPChartMathUtils::Degree2Radian(dAnchorAngle);

	int nBorderThickness = m_pLabel->GetBorder()->GetThickness();

	CRect rcPaneRect = GetPaneRect();
	rcPaneRect.InflateRect(1, 1);

	if (!rcPaneRect.PtInRect(CPoint((int)ptAnchorPoint.X, (int)ptAnchorPoint.Y)))
		return;

	CXTPChartTextPainter painter(pDC, text, m_pLabel);
	CXTPChartSizeF szText = painter.GetSize();

	CXTPChartPointF ptStartPoint(CXTPChartMathUtils::Round(ptAnchorPoint.X), CXTPChartMathUtils::Round(ptAnchorPoint.Y));
	CXTPChartPointF ptFinishPoint(CXTPChartMathUtils::Round(ptAnchorPoint.X + (cos(dLineAngle) * m_pLabel->GetLineLength())),
		CXTPChartMathUtils::Round(ptAnchorPoint.Y - (sin(dLineAngle) * m_pLabel->GetLineLength())));

	CXTPChartRectF rcInnerBounds;
	CXTPChartRectF rcBounds = CXTPChartSeriesLabelConnectorPainterBase::CalcBorderBoundsForTangentDrawing(ptFinishPoint, dLineAngle, szText, nBorderThickness, rcInnerBounds);

	CXTPChartSeriesView* pSeriseView = m_pPointView->GetSeriesView();
	CXTPChartRadarDiagramView* pDiagramView = (CXTPChartRadarDiagramView*)pSeriseView->GetDiagramView();
	pDiagramView->CheckLabelBounds(rcBounds);
}


CXTPChartDeviceCommand* CXTPChartRadarPointSeriesLabelView::CreateDeviceCommand(CXTPChartDeviceContext* pDC)
{
	CXTPChartString text(m_pLabel->GetPointLabel(m_pPointView->GetPoint()));
	CXTPChartPointF ptAnchorPoint = GetAnchorPoint();

	return CreateDeviceCommand(pDC, ptAnchorPoint, GetAnchorAngle(), text);
}


CXTPChartDeviceCommand* CXTPChartRadarPointSeriesLabelView::CreateDeviceCommand(CXTPChartDeviceContext* pDC, const CXTPChartPointF& ptAnchorPoint, double dAnchorAngle, const CXTPChartString& text)
{
	if (text.IsEmpty() || text == _T(" "))
		return NULL;

	double dLineAngle = CXTPChartMathUtils::Degree2Radian(dAnchorAngle);

	int nBorderThickness = m_pLabel->GetBorder()->GetThickness();

	CRect rcPaneRect = GetPaneRect();
	rcPaneRect.InflateRect(1, 1);

	if (!rcPaneRect.PtInRect(CPoint((int)ptAnchorPoint.X, (int)ptAnchorPoint.Y)))
		return NULL;

	CXTPChartDeviceCommand* pCommand = new CXTPChartHitTestElementCommand(m_pLabel);


	CXTPChartTextPainter painter(pDC, text, m_pLabel);
	CXTPChartSizeF szText = painter.GetSize();

	CXTPChartColor clrTextColor = GetActualTextColor();


	CXTPChartPointF ptStartPoint(CXTPChartMathUtils::Round(ptAnchorPoint.X), CXTPChartMathUtils::Round(ptAnchorPoint.Y));
	CXTPChartPointF ptFinishPoint(CXTPChartMathUtils::Round(ptAnchorPoint.X + (cos(dLineAngle) * m_pLabel->GetLineLength())),
		CXTPChartMathUtils::Round(ptAnchorPoint.Y - (sin(dLineAngle) * m_pLabel->GetLineLength())));

	CXTPChartRectF rcInnerBounds;
	CXTPChartRectF rcBounds = CXTPChartSeriesLabelConnectorPainterBase::CalcBorderBoundsForTangentDrawing(ptFinishPoint, dLineAngle, szText, nBorderThickness, rcInnerBounds);


	CXTPChartPointF ptLabelPoint = rcInnerBounds.GetLocation();


	CXTPChartColor clrBackColor = m_pLabel->GetActualBackColor();
	pCommand->AddChildCommand(m_pLabel->GetFillStyle()->CreateDeviceCommand(rcBounds, clrBackColor, clrBackColor));

	painter.SetLocation(ptLabelPoint);

	pCommand->AddChildCommand(painter.CreateDeviceCommand(pDC, clrTextColor));


	if (m_pLabel->GetShowLines())
	{
		CXTPChartColor clrConnectorColor = GetActualConnectorColor();

		CXTPChartSeriesLabelLineConnectorPainter linePainter(ptStartPoint, ptFinishPoint, dLineAngle, rcBounds);
		pCommand->AddChildCommand(linePainter.CreateDeviceCommand(pDC, m_pLabel, clrConnectorColor));
	}


	if (nBorderThickness)
	{
		CXTPChartColor clrBorderColor = GetActualBorderColor();

		pCommand->AddChildCommand(m_pLabel->GetBorder()->CreateInnerBorderDeviceCommand(rcBounds, clrBorderColor));
	}

	return pCommand;
}



#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPChartRadarPointSeriesLabel, CXTPChartSeriesLabel)
	DISP_PROPERTY_NOTIFY_ID(CXTPChartRadarPointSeriesLabel, "Angle", 107, m_nAngle, OleChartChanged, VT_I4)
END_DISPATCH_MAP()


// {6ACBCC77-27BF-4cb1-9ABF-4558D9835223}
static const GUID IID_IChartRadarPointSeriesLabel =
{ 0x6acbcc77, 0x27bf, 0x4cb1, { 0x9a, 0xbf, 0x45, 0x58, 0xd9, 0x83, 0x52, 0x23 } };

BEGIN_INTERFACE_MAP(CXTPChartRadarPointSeriesLabel, CXTPChartSeriesLabel)
	INTERFACE_PART(CXTPChartRadarPointSeriesLabel, IID_IChartRadarPointSeriesLabel, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartRadarPointSeriesLabel, IID_IChartRadarPointSeriesLabel)


#endif
