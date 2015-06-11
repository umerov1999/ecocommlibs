// XTPChartBarSeriesLabel.cpp
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

#include "../../XTPChartDefines.h"
#include "../../XTPChartElement.h"
#include <Chart/XTPChartLegendItem.h>
#include "../../XTPChartElementView.h"
#include "../../XTPChartSeriesLabel.h"
#include <Chart/XTPChartSeriesPointView.h>
#include "../../XTPChartSeriesView.h"
#include "../../XTPChartPanel.h"
#include "../../XTPChartDiagram.h"

#include "../../Diagram/Diagram2D/XTPChartDiagram2D.h"
#include "../../Diagram/Axis/XTPChartAxis.h"
#include "../../Diagram/Axis/XTPChartAxisView.h"
#include "../../Diagram/Diagram2D/XTPChartDiagram2DSeriesLabel.h"
#include "../../Diagram/Diagram2D/XTPChartDiagram2DSeriesView.h"



#include "XTPChartBarSeriesLabel.h"
#include "XTPChartBarSeriesView.h"

//////////////////////////////////////////////////////////////////////////
// CXTPChartBarSeriesLabel
IMPLEMENT_SERIAL(CXTPChartBarSeriesLabel, CXTPChartDiagram2DSeriesLabel, VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)

CXTPChartBarSeriesLabel::CXTPChartBarSeriesLabel()
{
	m_nPosition = xtpChartBarLabelTop;
}

CXTPChartBarSeriesLabel::~CXTPChartBarSeriesLabel()
{

}

void CXTPChartBarSeriesLabel::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPChartDiagram2DSeriesLabel::DoPropExchange(pPX);

	PX_Enum(pPX, _T("Position"), m_nPosition, xtpChartBarLabelTop);
}

CXTPChartElementView* CXTPChartBarSeriesLabel::CreateView(CXTPChartDeviceContext* pDC, CXTPChartSeriesPointView* pPointView, CXTPChartElementView* pParentView)
{
	UNREFERENCED_PARAMETER(pDC);
	return new CXTPChartBarSeriesLabelView(this, pPointView, pParentView);
}


//////////////////////////////////////////////////////////////////////////
// CXTPChartBarSeriesLabelView

CXTPChartBarSeriesLabelView::CXTPChartBarSeriesLabelView(CXTPChartSeriesLabel* pLabel, CXTPChartSeriesPointView* pPointView, CXTPChartElementView* pParentView)
	: CXTPChartDiagram2DSeriesLabelView(pLabel, pPointView, pParentView)
{

}

double CXTPChartBarSeriesLabelView::GetAnchorAngle() const
{
	CXTPChartBarSeriesLabel* pLabel = (CXTPChartBarSeriesLabel*)m_pLabel;

	CXTPChartDiagram2DSeriesView* pView = (CXTPChartDiagram2DSeriesView*)m_pPointView->GetSeriesView();

	CXTPChartDiagram2D* pDiagram = (CXTPChartDiagram2D*)pView->GetDiagram();

	if (pLabel->GetPosition() == xtpChartBarLabelTop)
	{
		double dValue = m_pPointView->GetInternalValue();

		if (pDiagram->IsRotated())
		{
			BOOL bTop = dValue >= 0;
			if (pView->GetAxisViewY()->GetAxis()->IsReversed())
				bTop = !bTop;

			if (!bTop)
				return 180;
			return 0;
		}
		else
		{
			BOOL bTop = dValue >= 0;
			if (pView->GetAxisViewY()->GetAxis()->IsReversed())
				bTop = !bTop;

			if (!bTop)
				return 270;
			return 90;
		}

	}

	return 0;

}

CXTPChartPointF CXTPChartBarSeriesLabelView::GetAnchorPoint() const
{
	CXTPChartRectF rc(((CXTPChartBarSeriesPointView*)m_pPointView)->GetScreenRect());

	CXTPChartBarSeriesLabel* pLabel = (CXTPChartBarSeriesLabel*)m_pLabel;

	if (pLabel->GetPosition() == xtpChartBarLabelTop)
	{
		double dAngle = GetAnchorAngle();

		if (dAngle == 0)
			return CXTPChartPointF(rc.GetRight(), rc.GetTop() + rc.Height / 2);

		if (dAngle == 180)
			return CXTPChartPointF(rc.GetLeft(), rc.GetTop() + rc.Height / 2);

		if (dAngle == 90)
			return CXTPChartPointF(rc.GetLeft() + rc.Width / 2, rc.GetTop());

		if (dAngle == 270)
			return CXTPChartPointF(rc.GetLeft() + rc.Width / 2, rc.GetBottom());

		return rc.GetCenter();
	}
	else
	{
		return rc.GetCenter();
	}
}

void CXTPChartBarSeriesLabelView::CalculateLayout(CXTPChartDeviceContext* pDC)
{
	CXTPChartBarSeriesLabel* pLabel = (CXTPChartBarSeriesLabel*)m_pLabel;

	if (pLabel->GetPosition() == xtpChartBarLabelTop)
	{
		CXTPChartDiagram2DSeriesLabelView::CalculateLayout(pDC);
	}
	else
	{
		CXTPChartDiagram2DSeriesLabelView::CalculateLayout(pDC, GetAnchorPoint());
	}
}

CXTPChartDeviceCommand* CXTPChartBarSeriesLabelView::CreateDeviceCommand(CXTPChartDeviceContext* pDC)
{
	CXTPChartBarSeriesLabel* pLabel = (CXTPChartBarSeriesLabel*)m_pLabel;

	if (pLabel->GetPosition() == xtpChartBarLabelTop)
	{
		return CXTPChartDiagram2DSeriesLabelView::CreateDeviceCommand(pDC);
	}
	else
	{
		return CXTPChartDiagram2DSeriesLabelView::CreateDeviceCommand(pDC, GetAnchorPoint());
	}

}


#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPChartBarSeriesLabel, CXTPChartDiagram2DSeriesLabel)
	DISP_PROPERTY_NOTIFY_ID(CXTPChartBarSeriesLabel, "Position", 106, m_nPosition, OleChartChanged, VT_I4)
END_DISPATCH_MAP()


// {654BCC77-27BF-4cb1-9ABF-4558D9835223}
static const GUID IID_IChartBarSeriesLabel =
{ 0x654bcc77, 0x27bf, 0x4cb1, { 0x9a, 0xbf, 0x45, 0x58, 0xd9, 0x83, 0x52, 0x23 } };

BEGIN_INTERFACE_MAP(CXTPChartBarSeriesLabel, CXTPChartDiagram2DSeriesLabel)
	INTERFACE_PART(CXTPChartBarSeriesLabel, IID_IChartBarSeriesLabel, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartBarSeriesLabel, IID_IChartBarSeriesLabel)


#endif
