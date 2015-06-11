// XTPChartPyramidSeriesLabel.cpp
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

#include "../../XTPChartDefines.h"
#include "../../XTPChartElement.h"
#include <Chart/XTPChartLegendItem.h>
#include "../../XTPChartElementView.h"
#include <Chart/XTPChartSeriesPointView.h>
#include "../../XTPChartSeriesView.h"
#include "../../XTPChartSeriesStyle.h"
#include "../../XTPChartSeriesLabel.h"

#include "../../Appearance/XTPChartFillStyle.h"
#include "../../Appearance/XTPChartBorder.h"

#include "../../Drawing/XTPChartDeviceCommand.h"

#include <Common/XTPMathUtils.h>
#include "../../Utils/XTPChartTextPainter.h"

#include "XTPChartPyramidSeriesLabel.h"
#include "XTPChartPyramidSeriesStyle.h"


//////////////////////////////////////////////////////////////////////////
// CXTPChartPyramidSeriesLabel

IMPLEMENT_SERIAL(CXTPChartPyramidSeriesLabel, CXTPChartSeriesLabel, VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)

CXTPChartPyramidSeriesLabel::CXTPChartPyramidSeriesLabel()
{
	m_nPosition = xtpChartPyramidLabelRight;
	m_nLineLength = 20;
}

CXTPChartPyramidSeriesLabel::~CXTPChartPyramidSeriesLabel()
{
}

CXTPChartPyramidSeriesStyle* CXTPChartPyramidSeriesLabel::GetStyle() const
{
	return (CXTPChartPyramidSeriesStyle*)m_pOwner;
}

void CXTPChartPyramidSeriesLabel::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPChartSeriesLabel::DoPropExchange(pPX);

	PX_Enum(pPX, _T("Position"), m_nPosition, xtpChartPyramidLabelRight);
}

//////////////////////////////////////////////////////////////////////////
// CView;

class CXTPChartPyramidSeriesLabel::CView : public CXTPChartSeriesLabelView
{
public:
	CView(CXTPChartSeriesLabel* pLabel, CXTPChartSeriesPointView* pPointView, CXTPChartElementView* pParentView);

protected:
	virtual void CalculateLayout(CXTPChartDeviceContext* pDC);
	virtual CXTPChartDeviceCommand* CreateDeviceCommand(CXTPChartDeviceContext* pDC);

protected:

	XTPChartPyramidLabelPosition GetPosition() const;

};

CXTPChartElementView* CXTPChartPyramidSeriesLabel::CreateView(CXTPChartDeviceContext* pDC, CXTPChartSeriesPointView* pPointView, CXTPChartElementView* pParentView)
{
	UNREFERENCED_PARAMETER(pDC);
	return new CView(this, pPointView, pParentView);
}


CXTPChartPyramidSeriesLabel::CView::CView(CXTPChartSeriesLabel* pLabel, CXTPChartSeriesPointView* pPointView, CXTPChartElementView* pParentView)
	: CXTPChartSeriesLabelView(pLabel, pPointView, pParentView)
{

}

void CXTPChartPyramidSeriesLabel::CView::CalculateLayout(CXTPChartDeviceContext* pDC)
{
	UNREFERENCED_PARAMETER(pDC);
}

BOOL CXTPChartPyramidSeriesLabel::IsInside() const
{
	XTPChartPyramidLabelPosition position = GetPosition();
	return position == xtpChartPyramidLabelCenter;

}

XTPChartPyramidLabelPosition CXTPChartPyramidSeriesLabel::CView::GetPosition() const
{
	return ((CXTPChartPyramidSeriesLabel*)m_pLabel)->GetPosition();
}

CXTPChartDeviceCommand* CXTPChartPyramidSeriesLabel::CView::CreateDeviceCommand(CXTPChartDeviceContext* pDC)
{
	CXTPChartPyramidSeriesLabel* pLabel = STATIC_DOWNCAST(CXTPChartPyramidSeriesLabel, m_pLabel);

	double lineAngle = 0;

	int borderThickness = m_pLabel->GetBorder()->GetThickness();

	CXTPChartPointF anchorPoint;
	bool bCenter = FALSE;

	CXTPChartPyramidSeriesPointView* pPointView = (CXTPChartPyramidSeriesPointView*)m_pPointView;

	if (pLabel->GetPosition() == xtpChartPyramidLabelRight)
	{
		lineAngle = CXTPMathUtils::Degree2Radian(0);
		anchorPoint.X = pPointView->m_rc.GetLeft() + int((pPointView->m_rc.Width + pPointView->m_rc.Width * (pPointView-> m_dFrom + pPointView->m_dTo) / 2) / 2);
		anchorPoint.Y = (float)(int)pPointView->m_rc.GetCenter().Y;
	}
	else if (pLabel->GetPosition() == xtpChartPyramidLabelLeft)
	{
		lineAngle = CXTPMathUtils::Degree2Radian(180);
		anchorPoint.X = pPointView->m_rc.GetLeft() + int((pPointView->m_rc.Width - pPointView->m_rc.Width * (pPointView-> m_dFrom + pPointView->m_dTo) / 2) / 2);
		anchorPoint.Y = (float)(int)pPointView->m_rc.GetCenter().Y;
	}
	else
	{
		anchorPoint = pPointView->m_rc.GetCenter();
		bCenter = TRUE;
	}


	CXTPChartString text(m_pLabel->GetPointLabel(m_pPointView->GetPoint()));
	if (text.IsEmpty() || text == _T(" "))
		return NULL;

	CXTPChartDeviceCommand* pCommand = new CXTPChartHitTestElementCommand(m_pLabel);

	CXTPChartTextPainter painter(pDC, text, m_pLabel);
	CXTPChartSizeF size = painter.GetSize();

	CXTPChartColor clrTextColor = GetActualTextColor();

	CXTPChartPointF startPoint(CXTPMathUtils::Round(anchorPoint.X), CXTPMathUtils::Round(anchorPoint.Y));
	CXTPChartPointF finishPoint(CXTPMathUtils::Round(anchorPoint.X + (cos(lineAngle) * m_pLabel->GetLineLength())),
		CXTPMathUtils::Round(anchorPoint.Y - (sin(lineAngle) * m_pLabel->GetLineLength())));

	CXTPChartRectF innerBounds;
	CXTPChartRectF bounds;

	if (bCenter)
	{
		bounds = CXTPChartRectF(0, 0, size.Width + 2 * borderThickness, size.Height + 2 * borderThickness);
		bounds.Offset(anchorPoint.X - bounds.Width / 2, anchorPoint.Y - bounds.Height / 2);
		bounds.Round();

		innerBounds = bounds;
		innerBounds.Inflate((float)-borderThickness, (float)-borderThickness);
	}
	else
	{
		CXTPChartPointF startPoint(CXTPMathUtils::Round(anchorPoint.X), CXTPMathUtils::Round(anchorPoint.Y));
		CXTPChartPointF finishPoint(CXTPMathUtils::Round(anchorPoint.X + (cos(lineAngle) * m_pLabel->GetLineLength())),
			CXTPMathUtils::Round(anchorPoint.Y - (sin(lineAngle) * m_pLabel->GetLineLength())));

		bounds = CXTPChartSeriesLabelConnectorPainterBase::CalcBorderBoundsForTangentDrawing(finishPoint, lineAngle, size, borderThickness, innerBounds);
	}

	CXTPChartPointF labelPoint = innerBounds.GetLocation();

	CXTPChartColor clrBackColor = m_pLabel->GetActualBackColor();
	pCommand->AddChildCommand(m_pLabel->GetFillStyle()->CreateDeviceCommand(bounds, clrBackColor, clrBackColor));

	painter.SetLocation(labelPoint);

	pCommand->AddChildCommand(painter.CreateDeviceCommand(pDC, clrTextColor));


	if (m_pLabel->GetShowLines() && !bCenter)
	{
		CXTPChartColor clrConnectorColor = GetActualConnectorColor();

		CXTPChartSeriesLabelLineConnectorPainter linePainter(startPoint, finishPoint, lineAngle, bounds);
		pCommand->AddChildCommand(linePainter.CreateDeviceCommand(pDC, m_pLabel, clrConnectorColor));
	}


	if (borderThickness)
	{
		CXTPChartColor clrBorderColor = GetActualBorderColor();

		pCommand->AddChildCommand(m_pLabel->GetBorder()->CreateInnerBorderDeviceCommand(bounds, clrBorderColor));
	}

	return pCommand;
}


#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPChartPyramidSeriesLabel, CXTPChartSeriesLabel)
	DISP_PROPERTY_NOTIFY_ID(CXTPChartPyramidSeriesLabel, "Position", 106, m_nPosition, OleChartChanged, VT_I4)
END_DISPATCH_MAP()


// {909BCC77-27BF-4cb1-9ABF-4558D9835223}
static const GUID IID_IChartPyramidSeriesLabel =
{ 0x909bcc77, 0x27bf, 0x4cb1, { 0x9a, 0xbf, 0x45, 0x58, 0xd9, 0x83, 0x52, 0x23 } };

BEGIN_INTERFACE_MAP(CXTPChartPyramidSeriesLabel, CXTPChartSeriesLabel)
INTERFACE_PART(CXTPChartPyramidSeriesLabel, IID_IChartPyramidSeriesLabel, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartPyramidSeriesLabel, IID_IChartPyramidSeriesLabel)


#endif
