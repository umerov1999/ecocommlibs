// XTPChartBarSeriesStyle.cpp
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
#include "../../XTPChartSeries.h"
#include "../../XTPChartSeriesView.h"
#include "../../XTPChartSeriesStyle.h"
#include "../../XTPChartDiagramView.h"
#include "../../XTPChartPanel.h"
#include "../../XTPChartDiagram.h"

#include "../../XTPChartSeriesPoint.h"

#include "../../Diagram/Diagram2D/XTPChartDiagram2D.h"
#include "../../Diagram/Axis/XTPChartAxisView.h"
#include "../../Diagram/Axis/XTPChartAxis.h"
#include "../../Diagram/Axis/XTPChartAxisRange.h"
#include "../../Diagram/Diagram2D/XTPChartDiagram2DSeriesStyle.h"
#include "../../Diagram/Diagram2D/XTPChartDiagram2DSeriesView.h"
#include "../../Diagram/Diagram2D/XTPChartDiagram2DSeriesLabel.h"

#include "../../Drawing/XTPChartDeviceCommand.h"
#include "../../Drawing/XTPChartRectangleDeviceCommand.h"

#include "../../Appearance/XTPChartBorder.h"
#include "../../Appearance/XTPChartFillStyle.h"

#include "XTPChartBarSeriesView.h"
#include "XTPChartBarSeriesStyle.h"
#include "XTPChartStackedBarSeriesStyle.h"
#include "XTPChartBarSeriesLabel.h"



//////////////////////////////////////////////////////////////////////////
// CXTPChartBarSeriesStyle

IMPLEMENT_SERIAL(CXTPChartBarSeriesStyle, CXTPChartDiagram2DSeriesStyle, VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT);

CXTPChartBarSeriesStyle::CXTPChartBarSeriesStyle()
{
	m_dBarWidth = 0.6;
	m_bSideBySide = TRUE;

	m_nBarDistanceFixed = 1;

	m_pBorder = new CXTPChartBorder(this);
	m_pFillStyle = new CXTPChartFillStyle(this);

	m_pFillStyle->SetFillMode(xtpChartFillGradient);
	m_pFillStyle->SetGradientAngle(xtpChartGradientAngle270);

	SetLabel(new CXTPChartBarSeriesLabel());
}

CXTPChartBarSeriesStyle::~CXTPChartBarSeriesStyle()
{
	SAFE_RELEASE(m_pBorder);
	SAFE_RELEASE(m_pFillStyle);
}


void CXTPChartBarSeriesStyle::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPChartDiagram2DSeriesStyle::DoPropExchange(pPX);

	PX_Double(pPX, _T("BarWidth"), m_dBarWidth, 0);
	PX_Int(pPX, _T("BarDistanceFixed"), m_nBarDistanceFixed, 1);

	PX_Bool(pPX, _T("SideBySide"), m_bSideBySide, TRUE);

	CXTPPropExchangeSection secFillStyle(pPX->GetSection(_T("FillStyle")));
	m_pFillStyle->DoPropExchange(&secFillStyle);

	CXTPPropExchangeSection secBorder(pPX->GetSection(_T("Border")));
	m_pBorder->DoPropExchange(&secBorder);
}


CXTPChartSeriesView* CXTPChartBarSeriesStyle::CreateView(CXTPChartSeries* pSeries, CXTPChartDiagramView* pDiagramView)
{
	return new CXTPChartBarSeriesView(pSeries, pDiagramView);
}

void CXTPChartBarSeriesStyle::CorrectAxisSideMargins(CXTPChartAxis* pAxis, double /*nMinValue*/, double /*nMaxValue*/, double& nCorrection)
{
	if (!pAxis->IsValuesAxis())
		nCorrection = max(nCorrection, 0.5);
}


//////////////////////////////////////////////////////////////////////////
// CXTPChartBarSeriesView

CXTPChartBarSeriesView::CXTPChartBarSeriesView(CXTPChartSeries* pSeries, CXTPChartDiagramView* pDiagramView)
	: CXTPChartDiagram2DSeriesView(pSeries, pDiagramView)
{
	m_nBarCount = 0;
	m_nBarIndex = -1;
	m_bFirstStack = TRUE;
}

CXTPChartSeriesPointView* CXTPChartBarSeriesView::CreateSeriesPointView(CXTPChartDeviceContext* pDC, CXTPChartSeriesPoint* pPoint, CXTPChartElementView* pParentView)
{
	UNREFERENCED_PARAMETER(pDC);
	return new CXTPChartBarSeriesPointView(pPoint, pParentView);
}


void CXTPChartBarSeriesView::BeforeUpdateRange(CXTPChartDeviceContext* pDC)
{
	UNREFERENCED_PARAMETER(pDC);

	CXTPChartDiagramView* pDiagramView = GetDiagramView();
	int i;

	for (i = 0; i < pDiagramView->GetSeriesView()->GetCount(); i++)
	{
		CXTPChartBarSeriesView* pView = (CXTPChartBarSeriesView*)pDiagramView->GetSeriesView()->GetAt(i);

		CXTPChartBarSeriesStyle* pStyle = DYNAMIC_DOWNCAST(CXTPChartBarSeriesStyle, pView->GetSeries()->GetStyle());
		if (!pStyle)
			continue;

		if (pView != this)
			return;

		break;
	}

	int nBarCount = 0;

	CMap<int, int, int, int> mapGroupToIndex;
	for (i = 0; i < pDiagramView->GetSeriesView()->GetCount(); i++)
	{
		CXTPChartBarSeriesView* pView = (CXTPChartBarSeriesView*)pDiagramView->GetSeriesView()->GetAt(i);

		CXTPChartBarSeriesStyle* pStyle = DYNAMIC_DOWNCAST(CXTPChartBarSeriesStyle, pView->GetSeries()->GetStyle());
		if (!pStyle)
			continue;

		CXTPChartStackedBarSeriesStyle* pStackedStyle = DYNAMIC_DOWNCAST(CXTPChartStackedBarSeriesStyle, pStyle);

		if (pStackedStyle)
		{
			int nIndex = -1;

			if (mapGroupToIndex.Lookup(pStackedStyle->GetStackGroup(), nIndex))
			{
				pView->m_nBarIndex = nIndex;
				pView->m_bFirstStack = FALSE;
			}
			else
			{
				mapGroupToIndex[pStackedStyle->GetStackGroup()] = nBarCount;

				pView->m_nBarIndex = nBarCount;
				pView->m_bFirstStack = TRUE;

				nBarCount++;
			}
		}
		else
		{
			pView->m_nBarIndex = nBarCount;
			pView->m_bFirstStack = TRUE;

			if (pStyle->IsSideBySide())
				nBarCount++;
		}
	}

	for (i = 0; i < pDiagramView->GetSeriesView()->GetCount(); i++)
	{
		CXTPChartBarSeriesView* pView = (CXTPChartBarSeriesView*)pDiagramView->GetSeriesView()->GetAt(i);

		CXTPChartBarSeriesStyle* pStyle = DYNAMIC_DOWNCAST(CXTPChartBarSeriesStyle, pView->GetSeries()->GetStyle());
		if (!pStyle)
			continue;

		if (pStyle->IsSideBySide())
		{
			pView->m_nBarCount = nBarCount;
		}
		else
		{
			pView->m_nBarIndex = 0;
			pView->m_nBarCount = 1;
		}
	}
}

CXTPChartDeviceCommand* CXTPChartBarSeriesView::CreateLegendDeviceCommand(CXTPChartDeviceContext* pDC, CRect rcBounds)
{
	UNREFERENCED_PARAMETER(pDC);
	rcBounds.DeflateRect(1, 1);

	CXTPChartBarSeriesStyle* pStyle = (CXTPChartBarSeriesStyle*)GetSeries()->GetStyle();

	CXTPChartDeviceCommand* pCommand = new CXTPChartDeviceCommand();

	pCommand->AddChildCommand(pStyle->GetFillStyle()->CreateDeviceCommand(rcBounds, GetColor(), GetColor2()));
	pCommand->AddChildCommand(new CXTPChartInnerBorderDeviceCommand(rcBounds, GetColor().GetDarkColor(), 1));

	return pCommand;
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartBarSeriesPointView

CXTPChartBarSeriesPointView::CXTPChartBarSeriesPointView(CXTPChartSeriesPoint* pPoint, CXTPChartElementView* pParentView)
	: CXTPChartSeriesPointView(pPoint, pParentView)
{

}

CXTPChartRectF CXTPChartBarSeriesPointView::GetScreenRect() const
{
	CXTPChartBarSeriesView* pSeriesView = (CXTPChartBarSeriesView*)GetSeriesView();
	CXTPChartBarSeriesStyle* pSeriesStyle = (CXTPChartBarSeriesStyle*)GetSeriesView()->GetStyle();

	CXTPChartDiagram2DAxisView* pAxisXView = pSeriesView->GetAxisViewX();
	CXTPChartDiagram2DAxisView* pAxisYView = pSeriesView->GetAxisViewY();

	if (pSeriesView->m_nBarIndex != -1)
	{
		int nCount = pSeriesView->m_nBarCount;
		int nIndex = pSeriesView->m_nBarIndex;

		double dOffset = (1 + pSeriesStyle->GetBarDistanceFixed()) / pAxisXView->GetScale();

		double dBarWidth = (pSeriesStyle->GetBarWidth() - dOffset * (nCount - 1)) / nCount;

		CXTPChartPointF ptTopLeft(pSeriesView->GetScreenPoint(m_pPoint->GetInternalArgumentValue() -
			pSeriesStyle->GetBarWidth() / 2 + dBarWidth * nIndex + dOffset * nIndex, GetInternalValue()));

		double dZero = max(0, pAxisYView->GetAxis()->GetRange()->GetMinValue());

		CXTPChartPointF ptBottomRight(pSeriesView->GetScreenPoint(m_pPoint->GetInternalArgumentValue() -
			pSeriesStyle->GetBarWidth() / 2 + dBarWidth * nIndex + dOffset * nIndex + dBarWidth, dZero));

		CXTPChartRectF rc(ptTopLeft, ptBottomRight);
		rc.Width++;
		rc.Height++;

		return rc;
	}

	return CXTPChartRectF(0, 0, 0, 0);
}


CXTPChartDeviceCommand* CXTPChartBarSeriesPointView::CreateDeviceCommand(CXTPChartDeviceContext* pDC)
{
	CXTPChartDiagramView* pDiagramView = GetSeriesView()->GetDiagramView();
	CXTPChartDiagram2D* pDiagram = (CXTPChartDiagram2D*)pDiagramView->GetDiagram();
	CXTPChartBarSeriesStyle* pStyle = (CXTPChartBarSeriesStyle*)GetSeriesView()->GetSeries()->GetStyle();

	UNREFERENCED_PARAMETER(pDC);

	CXTPChartRectF rc(GetScreenRect());

	CXTPChartDeviceCommand* pContainer = new CXTPChartPolygonAntialiasingDeviceCommand(FALSE);
	CXTPChartDeviceCommand* pCommand = pContainer->AddChildCommand(new CXTPChartHitTestElementCommand(m_pPoint, rc));

	if (pDiagram->IsRotated())
	{
		CXTPChartFillStyle* pFillStyle = pStyle->GetFillStyle()->CreateRotatedStyle();

		pCommand->AddChildCommand(pFillStyle->CreateDeviceCommand(rc, GetColor(), GetColor2()));

		pFillStyle->InternalRelease();
	}
	else
	{
		pCommand->AddChildCommand(pStyle->GetFillStyle()->CreateDeviceCommand(rc, GetColor(), GetColor2()));
	}

	CXTPChartColor clrBorder = pStyle->GetBorder()->GetColor();
	if (clrBorder.IsEmpty()) clrBorder = GetColor().GetDarkColor();

	pCommand->AddChildCommand(pStyle->GetBorder()->CreateInnerBorderDeviceCommand(rc, clrBorder));

	return pContainer;
}

CXTPChartDeviceCommand* CXTPChartBarSeriesPointView::CreateLegendDeviceCommand(CXTPChartDeviceContext* pDC, CRect rcBounds)
{
	UNREFERENCED_PARAMETER(pDC);
	rcBounds.DeflateRect(1, 1);

	CXTPChartBarSeriesStyle* pStyle = (CXTPChartBarSeriesStyle*)GetSeriesView()->GetSeries()->GetStyle();

	CXTPChartDeviceCommand* pCommand = new CXTPChartDeviceCommand();

	pCommand->AddChildCommand(pStyle->GetFillStyle()->CreateDeviceCommand(rcBounds, GetColor(), GetColor2()));
	pCommand->AddChildCommand(new CXTPChartInnerBorderDeviceCommand(rcBounds, GetColor().GetDarkColor(), 1));

	return pCommand;
}


#ifdef _XTP_ACTIVEX


BEGIN_DISPATCH_MAP(CXTPChartBarSeriesStyle, CXTPChartDiagram2DSeriesStyle)
	DISP_PROPERTY_EX_ID(CXTPChartBarSeriesStyle, "BarWidth", 4, GetBarWidth, SetBarWidth, VT_R8)

	DISP_PROPERTY_EX_ID(CXTPChartBarSeriesStyle, "FillStyle", 5, OleGetFillStyle, SetNotSupported, VT_DISPATCH)
	DISP_PROPERTY_EX_ID(CXTPChartBarSeriesStyle, "Border", 6, OleGetBorder, SetNotSupported, VT_DISPATCH)

	DISP_PROPERTY_EX_ID(CXTPChartBarSeriesStyle, "SideBySide", 7, IsSideBySide, SetSideBySide, VT_BOOL)

END_DISPATCH_MAP()


// {323BCC77-27BF-4cb1-9ABF-4558D9835223}
static const GUID IID_IChartBarSeriesStyle =
{ 0x323bcc77, 0x27bf, 0x4cb1, { 0x9a, 0xbf, 0x45, 0x58, 0xd9, 0x83, 0x52, 0x23 } };

BEGIN_INTERFACE_MAP(CXTPChartBarSeriesStyle, CXTPChartDiagram2DSeriesStyle)
	INTERFACE_PART(CXTPChartBarSeriesStyle, IID_IChartBarSeriesStyle , Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartBarSeriesStyle, IID_IChartBarSeriesStyle )

// {32490E6F-92B6-4671-9613-6B2A0FBF80A8}
IMPLEMENT_OLECREATE_EX2(CXTPChartBarSeriesStyle, "Codejock.ChartBarSeriesStyle." _XTP_AXLIB_VERSION,
						0x32490e6f, 0x92b6, 0x4671, 0x96, 0x13, 0x6b, 0x2a, 0xf, 0xbf, 0x80, 0xa8);


LPDISPATCH CXTPChartBarSeriesStyle::OleGetFillStyle()
{
	return XTPGetDispatch(GetFillStyle());
}

LPDISPATCH CXTPChartBarSeriesStyle::OleGetBorder()
{
	return XTPGetDispatch(GetBorder());
}


#endif
