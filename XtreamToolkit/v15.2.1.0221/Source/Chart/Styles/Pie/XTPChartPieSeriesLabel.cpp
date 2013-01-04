// XTPChartPieSeriesLabel.cpp
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
#include "../../XTPChartSeriesLabel.h"
#include "../../XTPChartSeriesStyle.h"
#include "../../XTPChartSeriesView.h"
#include "../../XTPChartPanel.h"
#include "../../XTPChartDiagram.h"

#include "../../Appearance/XTPChartFillStyle.h"
#include "../../Appearance/XTPChartBorder.h"

#include "../../Drawing/XTPChartDeviceCommand.h"
#include "../../Drawing/XTPChartTransformationDeviceCommand.h"
#include "../../Drawing/XTPChartLineDeviceCommand.h"

#include "../../Utils/XTPChartMathUtils.h"
#include "../../Utils/XTPChartTextPainter.h"

#include "XTPChartPieDiagram.h"
#include "XTPChartPieSeriesLabel.h"
#include "XTPChartPieSeriesStyle.h"


//////////////////////////////////////////////////////////////////////////
// CXTPChartPieSeriesLabel

IMPLEMENT_SERIAL(CXTPChartPieSeriesLabel, CXTPChartSeriesLabel, VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)

CXTPChartPieSeriesLabel::CXTPChartPieSeriesLabel()
{
	m_nPosition = xtpChartPieLabelOutside;
}

CXTPChartPieSeriesLabel::~CXTPChartPieSeriesLabel()
{
}

CXTPChartPieSeriesStyleBase* CXTPChartPieSeriesLabel::GetStyle() const
{
	return (CXTPChartPieSeriesStyleBase*)m_pOwner;
}

void CXTPChartPieSeriesLabel::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPChartSeriesLabel::DoPropExchange(pPX);

	PX_Enum(pPX, _T("Position"), m_nPosition, xtpChartPieLabelOutside);
}

CXTPChartPieSeriesLabelsView::CXTPChartPieSeriesLabelsView(CXTPChartElementView* pParentView)
	: CXTPChartElementView(pParentView)
{

}



//////////////////////////////////////////////////////////////////////////
// CView;

class CXTPChartPieSeriesLabelView : public CXTPChartSeriesLabelView
{
public:
	CXTPChartPieSeriesLabelView(CXTPChartSeriesLabel* pLabel, CXTPChartSeriesPointView* pPointView, CXTPChartElementView* pParentView);

public:
	XTPChartPieLabelPosition GetPosition() const;

protected:
	virtual void CalculateLayout(CXTPChartDeviceContext* pDC);
	virtual CXTPChartDeviceCommand* CreateDeviceCommand(CXTPChartDeviceContext* pDC);

	CXTPChartDeviceCommand* CreateConnectorDeviceCommand(CXTPChartDeviceContext* pDC);

protected:
	CXTPChartRectF m_rcBounds;
	CXTPChartPointF m_ptStartPoint;
	double m_dLineAngle;


	friend class CXTPChartPieSeriesLabelsView;
};


CXTPChartDeviceCommand* CXTPChartPieSeriesLabelsView::CreateDeviceCommand(CXTPChartDeviceContext* pDC)
{
	CXTPChartDeviceCommand* pDrawingType = new CXTPChartDeviceCommand();

	int i;
	for (i = 0; i < m_arrChildren.GetSize(); i++)
	{
		CXTPChartPieSeriesLabelView* pChildView =(CXTPChartPieSeriesLabelView*)m_arrChildren[i];
		pDrawingType->AddChildCommand(pChildView->CreateConnectorDeviceCommand(pDC));
	}

	for (i = 0; i < m_arrChildren.GetSize(); i++)
	{
		CXTPChartPieSeriesLabelView* pChildView =(CXTPChartPieSeriesLabelView*)m_arrChildren[i];
		pDrawingType->AddChildCommand(pChildView->CreateDeviceCommand(pDC));
	}


	return pDrawingType;
}

void CXTPChartPieSeriesLabelsView::ResolveOverlappingColumn(CArray<CXTPChartPieSeriesLabelView*, CXTPChartPieSeriesLabelView*>& arrColumn)
{
	if (arrColumn.GetSize() < 2)
		return;

	int i;

	for (i = 0; i < arrColumn.GetSize(); i++)
	{
		double& lineAngle = arrColumn[i]->m_dLineAngle;

		if (lineAngle >= CXTPChartMathUtils::m_dPI / 2 && lineAngle <= 3 * CXTPChartMathUtils::m_dPI / 2)
		{

		}
		else
		{
			if (lineAngle > 3 * CXTPChartMathUtils::m_dPI / 2)
				lineAngle -= CXTPChartMathUtils::m_dPI * 2;
			lineAngle = -lineAngle;
		}
	}

	// Yeah, bubble sort, but we're not waiting more than 20 labels here.
	for (i = 0; i < arrColumn.GetSize(); i++)
	{
		for (int j = 0; j < i; j++)
		{
			if (arrColumn[i]->m_dLineAngle < arrColumn[j]->m_dLineAngle)
			{
				CXTPChartPieSeriesLabelView* p = arrColumn[i];
				arrColumn[i] = arrColumn[j];
				arrColumn[j] = p;
			}

		}
	}

	CXTPChartPieSeriesView* pSeriesView = (CXTPChartPieSeriesView*)arrColumn[0]->GetPointView()->GetSeriesView();
	CRect rcBounds = pSeriesView->GetBounds();


	for (i = (int)arrColumn.GetSize() - 2; i >= 0; i--)
	{
		CXTPChartRectF& rcBounds = arrColumn[i]->m_rcBounds;

		if (arrColumn[i]->m_ptStartPoint.Y < rcBounds.GetCenter().Y)
			continue;

		if (rcBounds.GetBottom() + 2 > arrColumn[i + 1]->m_rcBounds.GetTop())
		{
			rcBounds.Y = arrColumn[i + 1]->m_rcBounds.GetTop() - 2 - rcBounds.Height;
		}
	}

	if (arrColumn[0]->m_rcBounds.GetTop() < rcBounds.top)
		arrColumn[0]->m_rcBounds.Y = (float)rcBounds.top;


	for (i = 1; i < arrColumn.GetSize(); i++)
	{
		CXTPChartRectF& rcBounds = arrColumn[i]->m_rcBounds;

		if (rcBounds.GetTop() < arrColumn[i - 1]->m_rcBounds.GetBottom() + 2)
		{
			rcBounds.Y = arrColumn[i - 1]->m_rcBounds.GetBottom() + 2;
		}

	}

}

void CXTPChartPieSeriesLabelsView::ResolveOverlapping()
{
	if (m_arrChildren.GetSize() == 0)
		return;

	if (((CXTPChartPieSeriesLabelView*)GetAt(0))->GetPosition() != xtpChartPieLabelTwoColumns)
		return;


	CXTPChartPieSeriesView* pSeriesView = (CXTPChartPieSeriesView*)((CXTPChartPieSeriesLabelView*)GetAt(0))->GetPointView()->GetSeriesView();
	CXTPChartRectF rcInnerBounds = pSeriesView->GetInnerBounds();
	int nCenter = (int)rcInnerBounds.GetCenter().X;


	CArray<CXTPChartPieSeriesLabelView*, CXTPChartPieSeriesLabelView*>  arrColumn;
	int i;


	for (i = 0; i < m_arrChildren.GetSize(); i++)
	{
		CXTPChartPieSeriesLabelView* pView = (CXTPChartPieSeriesLabelView*)m_arrChildren[i];
		if (pView->m_rcBounds.GetLeft() > nCenter)
		{
			arrColumn.Add(pView);
		}
	}
	ResolveOverlappingColumn(arrColumn);

	arrColumn.RemoveAll();


	for (i = 0; i < m_arrChildren.GetSize(); i++)
	{
		CXTPChartPieSeriesLabelView* pView = (CXTPChartPieSeriesLabelView*)m_arrChildren[i];
		if (!(pView->m_rcBounds.GetLeft() > nCenter))
		{
			arrColumn.Add(pView);
		}
	}

	ResolveOverlappingColumn(arrColumn);
}

CXTPChartPieSeriesLabelView::CXTPChartPieSeriesLabelView(CXTPChartSeriesLabel* pLabel, CXTPChartSeriesPointView* pPointView, CXTPChartElementView* pParentView)
	: CXTPChartSeriesLabelView(pLabel, pPointView, pParentView)
{

}

void CXTPChartPieSeriesLabelView::CalculateLayout(CXTPChartDeviceContext* pDC)
{
	UNREFERENCED_PARAMETER(pDC);

	XTPChartPieLabelPosition position = GetPosition();

	if (position != xtpChartPieLabelTwoColumns) // Smart labels implemented for TwoColumns mode only.
		return;

	double lineAngle;

	int borderThickness = m_pLabel->GetBorder()->GetThickness();

	CXTPChartRectF realBounds(((CXTPChartPieSeriesPointView*)m_pPointView)->GetPie()->GetBounds());

	if (realBounds.Width < 1 || realBounds.Height < 1)
		return;

	CXTPChartPointF anchorPoint = ((CXTPChartPieSeriesLabel*)m_pLabel)->CalculateAnchorPointAndAngles((CXTPChartPieSeriesPointView*)m_pPointView,
		borderThickness, lineAngle);


	CXTPChartString text(m_pLabel->GetPointLabel(m_pPointView->GetPoint()));
	if (text.IsEmpty() || text == _T(" "))
		return;

	if (position == xtpChartPieLabelInside)
	{

	}
	else if (position == xtpChartPieLabelRadial)
	{

	}

	else if (position == xtpChartPieLabelOutside)
	{

	}
	else if (position == xtpChartPieLabelTwoColumns)
	{
		CXTPChartPieSeriesView* pSeriesView = (CXTPChartPieSeriesView*)m_pPointView->GetSeriesView();
		CXTPChartRectF rcInnerBounds = pSeriesView->GetInnerBounds();


		CXTPChartTextPainter painter(pDC, text, m_pLabel);
		CXTPChartSizeF size = painter.GetSize();

		CXTPChartPointF startPoint(anchorPoint);

		CXTPChartPointF finishPoint(anchorPoint.X + (float)(cos(lineAngle) * m_pLabel->GetLineLength()),
			anchorPoint.Y - (float)(sin(lineAngle) * m_pLabel->GetLineLength()));

		CXTPChartPointF finishPoint2;

		CXTPChartRectF bounds(0, 0, (float)floor(size.Width + 2 * borderThickness), (float)floor(size.Height + 2 * borderThickness));

		bounds.Y = (float)(int)(finishPoint.Y - bounds.Height / 2);

		if (lineAngle < 0) lineAngle += 2 * CXTPChartMathUtils::m_dPI;

		if (lineAngle >= CXTPChartMathUtils::m_dPI / 2 && lineAngle <= 3 * CXTPChartMathUtils::m_dPI / 2)
		{
			bounds.X = (float)(int)(rcInnerBounds.GetLeft() - pSeriesView->GetMaxLabelWidth());
			finishPoint2 = CXTPChartPointF(bounds.GetRight(), finishPoint.Y);

			if (finishPoint.X < finishPoint2.X)
				finishPoint = finishPoint2;
		}
		else
		{
			bounds.X = (float)(int)(rcInnerBounds.GetRight() + pSeriesView->GetMaxLabelWidth() - bounds.Width);
			finishPoint2 = CXTPChartPointF(bounds.X, finishPoint.Y);

			if (finishPoint.X > finishPoint2.X)
				finishPoint = finishPoint2;
		}
		m_rcBounds = bounds;
		m_ptStartPoint = startPoint;
		m_dLineAngle = lineAngle;
	}

}

BOOL CXTPChartPieSeriesLabel::IsInside() const
{
	XTPChartPieLabelPosition position = GetPosition();
	return position == xtpChartPieLabelInside || position == xtpChartPieLabelRadial;

}

XTPChartPieLabelPosition CXTPChartPieSeriesLabelView::GetPosition() const
{
	return ((CXTPChartPieSeriesLabel*)m_pLabel)->GetPosition();
}


CXTPChartPointF CXTPChartPieSeriesLabel::CalculateAnchorPointAndAngles(CXTPChartPieSeriesPointView* pPointView,
	int borderThickness, double& lineAngle)
{
	UNREFERENCED_PARAMETER(borderThickness);
	lineAngle = pPointView->GetPie()->GetHalfAngle();

	int nHolePercent = pPointView->GetPie()->GetHolePercent();

	if (nHolePercent > 0 && IsInside())
	{
		double fraction = nHolePercent / 100.0;
		double width = pPointView->GetPie()->GetBounds().Width / 2.0;
		double height = pPointView->GetPie()->GetBounds().Height / 2.0;
		double minWidth = width * fraction;
		double minHeight = height * fraction;
		if (minWidth <= 0.0 || minHeight <= 0.0)
			return CXTPChartPointF(0, 0);

		double startAngle = pPointView->GetPie()->GetStartAngle();
		double halfAngle = pPointView->GetPie()->GetHalfAngle();
		CXTPPoint3d center = pPointView->GetPie()->CalculateCenter(pPointView->GetBasePoint());
		CXTPChartEllipse maxEllipse(center, width, height);
		CXTPChartEllipse minEllipse(center, minWidth, minHeight);
		CXTPChartPie maxPie(startAngle, halfAngle, maxEllipse, 0, 0);
		CXTPChartPie minPie(startAngle, halfAngle, minEllipse, 0, 0);
		return CXTPChartPointF((maxPie.GetFinishPoint().X + minPie.GetFinishPoint().X) / 2.0f, (maxPie.GetFinishPoint().Y + minPie.GetFinishPoint().Y) / 2.0f);

	}

	CXTPChartRectF realBounds(pPointView->GetPie()->GetBounds());

	CXTPChartEllipse actualEllipse(pPointView->GetPie()->CalculateCenter(pPointView->GetBasePoint()), realBounds.Width / 2, realBounds.Height / 2);
	CXTPChartPie actualPie(pPointView->GetPie()->GetStartAngle(), pPointView->GetPie()->GetHalfAngle(), actualEllipse, 0, 0);

	if (!IsInside())
	{
		return actualPie.GetFinishPoint();
	}

	if (pPointView->m_dValue == 1)
	{
		lineAngle = 0;
		return actualPie.GetCenterPoint();
	}

	return CXTPChartPointF((actualPie.GetCenterPoint().X + actualPie.GetFinishPoint().X) / 2.0f, (actualPie.GetCenterPoint().Y + actualPie.GetFinishPoint().Y) / 2.0f);
}

CXTPChartDeviceCommand* CXTPChartPieSeriesLabelView::CreateConnectorDeviceCommand(CXTPChartDeviceContext* pDC)
{
	XTPChartPieLabelPosition position = GetPosition();

	if (position == xtpChartPieLabelInside || position == xtpChartPieLabelRadial)
		return NULL;

	CXTPChartDeviceCommand* pCommand = new CXTPChartDeviceCommand();

	double lineAngle;

	int borderThickness = m_pLabel->GetBorder()->GetThickness();

	CXTPChartRectF realBounds(((CXTPChartPieSeriesPointView*)m_pPointView)->GetPie()->GetBounds());

	if (realBounds.Width < 1 || realBounds.Height < 1)
		return pCommand;

	CXTPChartPointF anchorPoint = ((CXTPChartPieSeriesLabel*)m_pLabel)->CalculateAnchorPointAndAngles((CXTPChartPieSeriesPointView*)m_pPointView,
		borderThickness, lineAngle);


	CXTPChartString text(m_pLabel->GetPointLabel(m_pPointView->GetPoint()));
	if (text.IsEmpty() || text == _T(" "))
		return pCommand;

	CXTPChartColor clrTextColor = GetActualTextColor();

	if (position == xtpChartPieLabelOutside)
	{
		CXTPChartTextPainter painter(pDC, text, m_pLabel);
		CXTPChartSizeF size = painter.GetSize();

		CXTPChartPointF startPoint(anchorPoint);
		CXTPChartPointF finishPoint(anchorPoint.X + (float)(cos(lineAngle) * m_pLabel->GetLineLength()),
			anchorPoint.Y - (float)(sin(lineAngle) * m_pLabel->GetLineLength()));

		CXTPChartRectF innerBounds;
		CXTPChartRectF bounds = CXTPChartSeriesLabelConnectorPainterBase::CalcBorderBoundsForTangentDrawing(finishPoint, lineAngle, size, borderThickness, innerBounds);
		bounds.Round();
		innerBounds.Round();

		if (m_pLabel->GetShowLines())
		{
			CXTPChartColor clrConnectorColor = GetActualConnectorColor();

			CXTPChartSeriesLabelLineConnectorPainter linePainter(startPoint, finishPoint, lineAngle, bounds);
			pCommand->AddChildCommand(linePainter.CreateDeviceCommand(pDC, m_pLabel, clrConnectorColor));
		}
	}
	else if (position == xtpChartPieLabelTwoColumns)
	{
		CXTPChartRectF bounds = m_rcBounds;

		if (m_pLabel->GetShowLines())
		{
			CXTPChartPointF startPoint(anchorPoint);
			CXTPChartPointF finishPoint;
			CXTPChartPointF breakPoint;

			breakPoint.Y = finishPoint.Y = bounds.GetCenter().Y;

			double sn = sin(lineAngle);

			if (m_rcBounds.GetLeft() >= anchorPoint.X)
			{
				breakPoint.X = finishPoint.X = bounds.GetLeft();

				if (CXTPChartMathUtils::Compare(sn, 0, 0.1) != 0)
				{
					double lineLength = (startPoint.Y - finishPoint.Y) / sn;

					breakPoint.X = startPoint.X + (float)(cos(lineAngle) * lineLength);

					if (breakPoint.X > finishPoint.X || breakPoint.X < startPoint.X)
						breakPoint.X = finishPoint.X;
				}

			}
			else
			{
				breakPoint.X = finishPoint.X = bounds.GetRight();

				if (CXTPChartMathUtils::Compare(sn, 0, 0.1) != 0)
				{
					double lineLength = (startPoint.Y - finishPoint.Y) / sn;

					breakPoint.X = startPoint.X + (float)(cos(lineAngle) * lineLength);

					if (breakPoint.X < finishPoint.X || breakPoint.X > startPoint.X)
						breakPoint.X = finishPoint.X;
				}

			}

			CXTPChartColor clrConnectorColor = GetActualConnectorColor();

			CXTPChartDeviceCommand* pContaier = new CXTPChartPolygonAntialiasingDeviceCommand();
			pContaier->AddChildCommand(new CXTPChartSolidLineDeviceCommand(startPoint, breakPoint, clrConnectorColor, 1));
			pContaier->AddChildCommand(new CXTPChartSolidLineDeviceCommand(breakPoint, finishPoint, clrConnectorColor, 1));

			pCommand->AddChildCommand(pContaier);

		}

	}


	return pCommand;
}

CXTPChartDeviceCommand* CXTPChartPieSeriesLabelView::CreateDeviceCommand(CXTPChartDeviceContext* pDC)
{

	CXTPChartDeviceCommand* pCommand = new CXTPChartHitTestElementCommand(m_pLabel);

	double lineAngle;

	int borderThickness = m_pLabel->GetBorder()->GetThickness();

	CXTPChartRectF realBounds(((CXTPChartPieSeriesPointView*)m_pPointView)->GetPie()->GetBounds());

	if (realBounds.Width < 1 || realBounds.Height < 1)
		return pCommand;

	CXTPChartPointF anchorPoint = ((CXTPChartPieSeriesLabel*)m_pLabel)->CalculateAnchorPointAndAngles((CXTPChartPieSeriesPointView*)m_pPointView,
		borderThickness, lineAngle);

	XTPChartPieLabelPosition position = GetPosition();

	CXTPChartString text(m_pLabel->GetPointLabel(m_pPointView->GetPoint()));
	if (text.IsEmpty() || text == _T(" "))
		return pCommand;

	CXTPChartColor clrTextColor = GetActualTextColor();

	if (position == xtpChartPieLabelInside)
	{
		CXTPChartTextPainter painter(pDC, text, m_pLabel);
		CXTPChartSizeF size = painter.GetSize();

		CXTPChartRectF bounds(0, 0, size.Width + 2 * borderThickness, size.Height + 2 * borderThickness);
		bounds.Offset(anchorPoint.X - bounds.Width / 2, anchorPoint.Y - bounds.Height / 2);
		bounds.Round();

		CXTPChartRectF innerBounds = bounds;
		innerBounds.Inflate((float)-borderThickness, (float)-borderThickness);

		CXTPChartPointF labelPoint = innerBounds.GetLocation();

		CXTPChartColor clrBackColor = m_pLabel->GetActualBackColor();
		pCommand->AddChildCommand(m_pLabel->GetFillStyle()->CreateDeviceCommand(bounds, clrBackColor, clrBackColor));

		painter.SetLocation(labelPoint);

		pCommand->AddChildCommand(painter.CreateDeviceCommand(pDC, clrTextColor));

		if (borderThickness)
		{
			CXTPChartColor clrBorderColor = GetActualBorderColor();

			pCommand->AddChildCommand(m_pLabel->GetBorder()->CreateDeviceCommand(bounds, clrBorderColor));
		}
	}
	else if (position == xtpChartPieLabelRadial)
	{
		CXTPChartTextPainter painter(pDC, text, m_pLabel);
		CXTPChartSizeF size = painter.GetSize();

		CXTPChartRectF bounds(0, 0, size.Width + 2 * borderThickness, size.Height + 2 * borderThickness);
		bounds.Offset(- bounds.Width / 2, - bounds.Height / 2);
		bounds.Round();

		CXTPChartRectF innerBounds = bounds;
		innerBounds.Inflate((float)-borderThickness, (float)-borderThickness);

		float fAngle = (float)(-(int)CXTPChartMathUtils::Radian2Degree(lineAngle) + 90);
		fAngle = (float)CXTPChartMathUtils::NormalizeDegree(fAngle);

		fAngle = fAngle <= 180 ? fAngle - 90 : fAngle + 90;

		CXTPChartDeviceCommand* pStateGraphicsCommand = pCommand->AddChildCommand(new CXTPChartSaveStateDeviceCommand());
		pStateGraphicsCommand = pStateGraphicsCommand->AddChildCommand(new CXTPChartTranslateDeviceCommand(anchorPoint.X, anchorPoint.Y, 0));
		pStateGraphicsCommand = pStateGraphicsCommand->AddChildCommand(new CXTPChartRotateDeviceCommand(fAngle));
		pStateGraphicsCommand = pStateGraphicsCommand->AddChildCommand(new CXTPChartPolygonAntialiasingDeviceCommand(TRUE));

		CXTPChartPointF labelPoint = innerBounds.GetLocation();

		CXTPChartColor clrBackColor = m_pLabel->GetActualBackColor();
		pStateGraphicsCommand->AddChildCommand(m_pLabel->GetFillStyle()->CreateDeviceCommand(bounds, clrBackColor, clrBackColor));

		painter.SetLocation(labelPoint);

		pStateGraphicsCommand->AddChildCommand(painter.CreateDeviceCommand(pDC, clrTextColor));

		if (borderThickness)
		{
			CXTPChartColor clrBorderColor = GetActualBorderColor();

			pStateGraphicsCommand->AddChildCommand(m_pLabel->GetBorder()->CreateDeviceCommand(bounds, clrBorderColor));
		}
	}

	else if (position == xtpChartPieLabelOutside)
	{
		CXTPChartTextPainter painter(pDC, text, m_pLabel);
		CXTPChartSizeF size = painter.GetSize();

		CXTPChartPointF startPoint(anchorPoint);
		CXTPChartPointF finishPoint(anchorPoint.X + (float)(cos(lineAngle) * m_pLabel->GetLineLength()),
			anchorPoint.Y - (float)(sin(lineAngle) * m_pLabel->GetLineLength()));

		CXTPChartRectF innerBounds;
		CXTPChartRectF bounds = CXTPChartSeriesLabelConnectorPainterBase::CalcBorderBoundsForTangentDrawing(finishPoint, lineAngle, size, borderThickness, innerBounds);
		bounds.Round();
		innerBounds.Round();


		CXTPChartPointF labelPoint = innerBounds.GetLocation();

		CXTPChartColor clrBackColor = m_pLabel->GetActualBackColor();
		pCommand->AddChildCommand(m_pLabel->GetFillStyle()->CreateDeviceCommand(bounds, clrBackColor, clrBackColor));

		painter.SetLocation(labelPoint);
		pCommand->AddChildCommand(painter.CreateDeviceCommand(pDC, clrTextColor));


		if (borderThickness)
		{
			CXTPChartColor clrBorderColor = GetActualBorderColor();

			pCommand->AddChildCommand(m_pLabel->GetBorder()->CreateInnerBorderDeviceCommand(bounds, clrBorderColor));
		}

	}
	else if (position == xtpChartPieLabelTwoColumns)
	{
		CXTPChartRectF bounds = m_rcBounds;

		CXTPChartTextPainter painter(pDC, text, m_pLabel);
		painter.SetLocation(bounds.GetLocation());

		CXTPChartColor clrBackColor = m_pLabel->GetActualBackColor();
		pCommand->AddChildCommand(m_pLabel->GetFillStyle()->CreateDeviceCommand(bounds, clrBackColor, clrBackColor));

		pCommand->AddChildCommand(painter.CreateDeviceCommand(pDC, clrTextColor));


		if (borderThickness)
		{
			CXTPChartColor clrBorderColor = GetActualBorderColor();

			pCommand->AddChildCommand(m_pLabel->GetBorder()->CreateInnerBorderDeviceCommand(bounds, clrBorderColor));
		}


	}


	return pCommand;
}


CXTPChartElementView* CXTPChartPieSeriesLabel::CreateView(CXTPChartDeviceContext* pDC, CXTPChartSeriesPointView* pPointView, CXTPChartElementView* pParentView)
{
	UNREFERENCED_PARAMETER(pDC);
	return new CXTPChartPieSeriesLabelView(this, pPointView, pParentView);
}


#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPChartPieSeriesLabel, CXTPChartSeriesLabel)
	DISP_PROPERTY_NOTIFY_ID(CXTPChartPieSeriesLabel, "Position", 106, m_nPosition, OleChartChanged, VT_I4)
END_DISPATCH_MAP()


// {CDFBCC77-27BF-4cb1-9ABF-4558D9835223}
static const GUID IID_IChartPieSeriesLabel =
{ 0xcdfbcc77, 0x27bf, 0x4cb1, { 0x9a, 0xbf, 0x45, 0x58, 0xd9, 0x83, 0x52, 0x23 } };

BEGIN_INTERFACE_MAP(CXTPChartPieSeriesLabel, CXTPChartSeriesLabel)
INTERFACE_PART(CXTPChartPieSeriesLabel, IID_IChartPieSeriesLabel, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartPieSeriesLabel, IID_IChartPieSeriesLabel)


#endif
