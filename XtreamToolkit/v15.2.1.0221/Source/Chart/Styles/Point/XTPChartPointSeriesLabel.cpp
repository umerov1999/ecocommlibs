// XTPChartPointSeriesLabel.cpp
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
#include "../../XTPChartDefines.h"
#include "../../XTPChartElement.h"
#include "../../XTPChartElementView.h"
#include "../../XTPChartSeriesLabel.h"
#include "../../XTPChartSeriesView.h"
#include "../../XTPChartPanel.h"
#include "../../XTPChartDiagram.h"


#include "../../Diagram/Diagram2D/XTPChartDiagram2D.h"
#include "../../Diagram/Axis/XTPChartAxis.h"
#include "../../Diagram/Axis/XTPChartAxisView.h"
#include "../../Diagram/Diagram2D/XTPChartDiagram2DSeriesView.h"
#include "../../Diagram/Diagram2D/XTPChartDiagram2DSeriesLabel.h"


#include "XTPChartPointSeriesLabel.h"
#include "XTPChartPointSeriesLabelView.h"
#include "XTPChartPointSeriesView.h"


//////////////////////////////////////////////////////////////////////////
// CXTPChartPointSeriesLabel
IMPLEMENT_SERIAL(CXTPChartPointSeriesLabel, CXTPChartDiagram2DSeriesLabel, VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)

CXTPChartPointSeriesLabel::CXTPChartPointSeriesLabel()
{
	m_nAngle = 45;
}

CXTPChartPointSeriesLabel::~CXTPChartPointSeriesLabel()
{

}

void CXTPChartPointSeriesLabel::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPChartDiagram2DSeriesLabel::DoPropExchange(pPX);

	PX_Int(pPX, _T("Angle"), m_nAngle, 45);
}


CXTPChartElementView* CXTPChartPointSeriesLabel::CreateView(CXTPChartDeviceContext* pDC, CXTPChartSeriesPointView* pPointView, CXTPChartElementView* pParentView)
{
	UNREFERENCED_PARAMETER(pDC);
	return new CXTPChartPointSeriesLabelView(this, pPointView, pParentView);
}


//////////////////////////////////////////////////////////////////////////
// CXTPChartPointSeriesLabelView

CXTPChartPointSeriesLabelView::CXTPChartPointSeriesLabelView(CXTPChartSeriesLabel* pLabel, CXTPChartSeriesPointView* pPointView, CXTPChartElementView* pParentView)
	: CXTPChartDiagram2DSeriesLabelView(pLabel, pPointView, pParentView)
{

}


CXTPChartPointF CXTPChartPointSeriesLabelView::GetAnchorPoint() const
{
	return ((CXTPChartPointSeriesPointView*)m_pPointView)->GetScreenPoint();
}

double CXTPChartPointSeriesLabelView::GetAnchorAngle() const
{
	CXTPChartDiagram2DSeriesView* pView = (CXTPChartDiagram2DSeriesView*)m_pPointView->GetSeriesView();

	CXTPChartDiagram2D* pDiagram = (CXTPChartDiagram2D*)pView->GetDiagram();

	double lineAngle = ((CXTPChartPointSeriesLabel*)m_pLabel)->GetAngle();

	if (pView->GetAxisViewX()->GetAxis()->IsReversed())
		lineAngle = 180 - lineAngle;

	if (pView->GetAxisViewY()->GetAxis()->IsReversed())
		lineAngle = - lineAngle;

	if (pDiagram->IsRotated())
	{
		lineAngle = 90 - lineAngle;
	}
	return lineAngle;
}


#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPChartPointSeriesLabel, CXTPChartSeriesLabel)
	DISP_PROPERTY_NOTIFY_ID(CXTPChartPointSeriesLabel, "Angle", 107, m_nAngle, OleChartChanged, VT_I4)
END_DISPATCH_MAP()


// {789BCC77-27BF-4cb1-9ABF-4558D9835223}
static const GUID IID_IChartPointSeriesLabel =
{ 0x789bcc77, 0x27bf, 0x4cb1, { 0x9a, 0xbf, 0x45, 0x58, 0xd9, 0x83, 0x52, 0x23 } };

BEGIN_INTERFACE_MAP(CXTPChartPointSeriesLabel, CXTPChartSeriesLabel)
	INTERFACE_PART(CXTPChartPointSeriesLabel, IID_IChartPointSeriesLabel, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartPointSeriesLabel, IID_IChartPointSeriesLabel)


#endif
