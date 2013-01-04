// XTPChartPyramidSeriesStyle.cpp
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
#include "../../XTPChartSeriesStyle.h"
#include "../../XTPChartSeriesPoint.h"
#include "../../XTPChartSeriesLabel.h"
#include "../../XTPChartSeriesView.h"
#include "../../XTPChartPanel.h"
#include "../../XTPChartDiagram.h"
#include "../../XTPChartDiagramView.h"

#include "../../Drawing/XTPChartDeviceCommand.h"
#include "../../Utils/XTPChartMathUtils.h"
#include "../../Utils/XTPChartTextPainter.h"

#include "../../Appearance/XTPChartBorder.h"
#include "../../Appearance/XTPChartFillStyle.h"
#include "../../Drawing/XTPChartRectangleDeviceCommand.h"
#include "../../Drawing/XTPChartPolygonDeviceCommand.h"

#include "XTPChartPyramidSeriesStyle.h"
#include "XTPChartPyramidDiagram.h"
#include "XTPChartPyramidSeriesLabel.h"


//////////////////////////////////////////////////////////////////////////
// CXTPChartPyramidSeriesStyleBase

IMPLEMENT_SERIAL(CXTPChartPyramidSeriesStyle, CXTPChartSeriesStyle, VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)


CXTPChartPyramidSeriesStyle::CXTPChartPyramidSeriesStyle()
{
	m_nPointDistance = 4;
	m_dHeightToWidthRatio = 1;

	m_pBorder = new CXTPChartBorder(this);

	m_pFillStyle = new CXTPChartFillStyle(this);
	m_pFillStyle->SetFillMode(xtpChartFillGradient);
	m_pFillStyle->SetGradientAngle(xtpChartGradientAngle315);

	m_bColorEach = TRUE;

	SetLabel(new CXTPChartPyramidSeriesLabel());
}

CXTPChartPyramidSeriesStyle::~CXTPChartPyramidSeriesStyle()
{
	SAFE_RELEASE(m_pFillStyle);
	SAFE_RELEASE(m_pBorder);
}

void CXTPChartPyramidSeriesStyle::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPChartSeriesStyle::DoPropExchange(pPX);

	PX_Int(pPX, _T("PointDistance"), m_nPointDistance, 0);

	PX_Double(pPX, _T("HeightToWidthRatio"), m_dHeightToWidthRatio, 1);

	CXTPPropExchangeSection secBorder(pPX->GetSection(_T("Border")));
	m_pBorder->DoPropExchange(&secBorder);

	CXTPPropExchangeSection secFillStyle(pPX->GetSection(_T("FillStyle")));
	m_pFillStyle->DoPropExchange(&secFillStyle);

}

CXTPChartDiagram* CXTPChartPyramidSeriesStyle::CreateDiagram()
{
	return new CXTPChartPyramidDiagram();
}

CXTPChartSeriesView* CXTPChartPyramidSeriesStyle::CreateView(CXTPChartSeries* pSeries, CXTPChartDiagramView* pDiagramView)
{
	return new CXTPChartPyramidSeriesView(pSeries, pDiagramView);
}

BOOL CXTPChartPyramidSeriesStyle::IsStyleDiagram(CXTPChartDiagram* pDiagram) const
{
	if (DYNAMIC_DOWNCAST(CXTPChartPyramidDiagram, pDiagram) == NULL)
		return FALSE;

	return pDiagram->GetSeries().GetSize() == 0;
}





//////////////////////////////////////////////////////////////////////////
// CXTPChartPyramidDiagramSeriesView

CXTPChartPyramidSeriesView::CXTPChartPyramidSeriesView(CXTPChartSeries* pSeries, CXTPChartDiagramView* pDiagramView)
	: CXTPChartSeriesView(pSeries, pDiagramView)
{

}

CXTPChartPyramidSeriesView::~CXTPChartPyramidSeriesView()
{

}

void CXTPChartPyramidSeriesView::CalculatePointLayout(CXTPChartDeviceContext* pDC, CRect rcBounds)
{
	CXTPChartPyramidSeriesStyle* pStyle = STATIC_DOWNCAST(CXTPChartPyramidSeriesStyle, GetStyle());

	rcBounds.DeflateRect(5, 5, 5, 5);


	CXTPChartPyramidSeriesLabel* pLabel = (CXTPChartPyramidSeriesLabel*)pStyle->GetLabel();

	if (pLabel->IsVisible() && !pLabel->IsInside())
	{
		CSize sz(0, 0);
		for (int i = 0; i < m_pPointsView->GetCount(); i++)
		{
			CXTPChartPyramidSeriesPointView* pPointView = (CXTPChartPyramidSeriesPointView*)m_pPointsView->GetAt(i);

			CXTPChartString text(pLabel->GetPointLabel(pPointView->GetPoint()));
			CXTPChartTextPainter painter(pDC, text, pLabel);

			sz.cx = max(sz.cx, (INT)painter.GetSize().Width);
			sz.cy = max(sz.cy, (INT)painter.GetSize().Height);
		}

		int nLineLength = pStyle->GetLabel()->GetLineLength();
		if (pLabel->GetPosition() == xtpChartPyramidLabelRight)
			rcBounds.DeflateRect(0, 0, nLineLength + sz.cx, 0);
		if (pLabel->GetPosition() == xtpChartPyramidLabelLeft)
			rcBounds.DeflateRect(nLineLength + sz.cx, 0, 0, 0);
	}



	double dRatio = pStyle->GetHeightToWidthRatio();
	if (dRatio > CXTPChartMathUtils::m_dEPS)
	{
		int nHeight = rcBounds.Height();
		int nWidth = int(nHeight / dRatio);

		if (nWidth > rcBounds.Width())
		{
			nWidth = rcBounds.Width();
			nHeight = int(nWidth * dRatio);
		}

		CPoint ptTopLeft((rcBounds.left + rcBounds.right - nWidth) / 2, (rcBounds.top + rcBounds.bottom - nHeight) / 2);
		rcBounds = CRect(ptTopLeft, CSize(nWidth, nHeight));
	}


	m_rcInnterBounds = rcBounds;


	CalculateValues();

	int nCount = m_pPointsView->GetCount();

	double dFrom = 0;

	for (int i = 0; i < nCount; i++)
	{
		CXTPChartPyramidSeriesPointView* pPointView = (CXTPChartPyramidSeriesPointView*)m_pPointsView->GetAt(i);

		pPointView->CalculateLayout(i, nCount, dFrom, dFrom + pPointView->m_dValue);
		dFrom += pPointView->m_dValue;
	}
}


void CXTPChartPyramidSeriesView::CalculateLabelLayout(CXTPChartDeviceContext* pDC)
{
	for (int i = 0; i < m_pLabelsView->GetCount(); i++)
	{
		CXTPChartSeriesLabelView* pLabelView = (CXTPChartSeriesLabelView*)m_pLabelsView->GetAt(i);

		pLabelView->CalculateLayout(pDC);
	}
}

CXTPChartSeriesPointView* CXTPChartPyramidSeriesView::CreateSeriesPointView(CXTPChartDeviceContext* pDC, CXTPChartSeriesPoint* pPoint, CXTPChartElementView* pParentView)
{
	UNREFERENCED_PARAMETER(pDC);
	return new CXTPChartPyramidSeriesPointView(pPoint, pParentView);
}


void CXTPChartPyramidSeriesView::CalculateValues()
{
	double dSum = 0;
	int i;

	for (i = 0; i < m_pPointsView->GetCount(); i++)
	{
		CXTPChartPyramidSeriesPointView* pPointView = (CXTPChartPyramidSeriesPointView*)m_pPointsView->GetAt(i);

		CXTPChartSeriesPoint* pPoint = pPointView->GetPoint();

		double dValue = pPoint->GetValue(0);

		dSum += dValue;
	}

	if (dSum == 0)
		dSum = 1;

	for (i = 0; i < m_pPointsView->GetCount(); i++)
	{
		CXTPChartPyramidSeriesPointView* pPointView = (CXTPChartPyramidSeriesPointView*)m_pPointsView->GetAt(i);

		CXTPChartSeriesPoint* pPoint = pPointView->GetPoint();

		double dValue = pPoint->GetValue(0);

		pPointView->m_dValue = dValue >= 0 ? dValue / dSum : 0;
	}
}

CXTPChartDeviceCommand* CXTPChartPyramidSeriesView::CreateDeviceCommand(CXTPChartDeviceContext* pDC)
{
	CXTPChartDeviceCommand* pCommand = new CXTPChartPolygonAntialiasingDeviceCommand();
	pCommand->AddChildCommand(CXTPChartSeriesView::CreateDeviceCommand(pDC));

	return pCommand;
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartPyramidDiagramSeriesPointView

CXTPChartPyramidSeriesPointView::CXTPChartPyramidSeriesPointView(CXTPChartSeriesPoint* pPoint, CXTPChartElementView* pParentView)
: CXTPChartSeriesPointView(pPoint, pParentView)
{
	m_dValue = 0;
}

CXTPChartPyramidSeriesPointView::~CXTPChartPyramidSeriesPointView()
{
}

void CXTPChartPyramidSeriesPointView::CalculateLayout(int nIndex, int nCount, double dFrom, double dTo)
{
	CXTPChartPyramidSeriesView* pView = (CXTPChartPyramidSeriesView*)GetSeriesView();
	CXTPChartPyramidSeriesStyle* pStyle = STATIC_DOWNCAST(CXTPChartPyramidSeriesStyle, GetSeriesView()->GetStyle());

	CXTPChartRectF rcInnerRect = pView->GetInnerBounds();

	m_rc.X = rcInnerRect.X;
	m_rc.Y = (float)CXTPChartMathUtils::Round(rcInnerRect.Y + rcInnerRect.Height * dFrom);

	int bottom = (int)CXTPChartMathUtils::Round(rcInnerRect.Y + rcInnerRect.Height * dTo);

	if (nIndex != nCount -1)
		bottom -= pStyle->GetPointDistance();


	m_rc.Height = max(1.0f, bottom - m_rc.Y);
	m_rc.Width = rcInnerRect.Width;

	m_dFrom = (m_rc.Y - rcInnerRect.Y) / rcInnerRect.Height;
	m_dTo = ((m_rc.Y + m_rc.Height) - rcInnerRect.Y) / rcInnerRect.Height;
}


CXTPChartColor CXTPChartPyramidSeriesPointView::GetBorderActualColor() const
{
	CXTPChartPyramidSeriesStyle* pStyle = STATIC_DOWNCAST(CXTPChartPyramidSeriesStyle, GetSeriesView()->GetStyle());

	if (!pStyle->GetBorder()->GetColor().IsEmpty())
		return pStyle->GetBorder()->GetColor();

	CXTPChartColor clr = GetColor().GetDarkColor();
	return clr;
}

CXTPChartDeviceCommand* CXTPChartPyramidSeriesPointView::CreateDeviceCommand(CXTPChartDeviceContext* pDC)
{
	UNREFERENCED_PARAMETER(pDC);

	CXTPChartColor color1 = GetColor();
	CXTPChartColor color2 = GetColor2();
	CXTPChartColor clrBorder = GetBorderActualColor();

	CXTPChartDeviceCommand* pCommand = new CXTPChartHitTestElementCommand(m_pPoint);

	CXTPChartPyramidSeriesStyle* pStyle = STATIC_DOWNCAST(CXTPChartPyramidSeriesStyle, GetSeriesView()->GetStyle());

	CXTPChartPoints arrPoints;

	arrPoints.Add(CXTPChartPointF(m_rc.GetLeft() + int((m_rc.Width - m_rc.Width * m_dFrom) / 2), m_rc.GetTop()));
	arrPoints.Add(CXTPChartPointF(m_rc.GetLeft() + int((m_rc.Width + m_rc.Width * m_dFrom) / 2), m_rc.GetTop()));

	arrPoints.Add(CXTPChartPointF(m_rc.GetLeft() + int((m_rc.Width + m_rc.Width * m_dTo) / 2), m_rc.GetBottom()));
	arrPoints.Add(CXTPChartPointF(m_rc.GetLeft() + int((m_rc.Width - m_rc.Width * m_dTo) / 2), m_rc.GetBottom()));

	pCommand->AddChildCommand(pStyle->GetFillStyle()->CreateDeviceCommand(arrPoints, color1, color2));

	if (pStyle->GetBorder()->IsVisible())
		pCommand->AddChildCommand(new CXTPChartBoundedPolygonDeviceCommand(arrPoints, clrBorder, pStyle->GetBorder()->GetThickness()));


	return pCommand;
}

CXTPChartDeviceCommand* CXTPChartPyramidSeriesView::CreateLegendDeviceCommand(CXTPChartDeviceContext* pDC, CRect rcBounds,
	CXTPChartColor color1, CXTPChartColor color2, CXTPChartColor clrBorder)
{
	UNREFERENCED_PARAMETER(pDC);
	rcBounds.DeflateRect(1, 1);

	CXTPChartPyramidSeriesStyle* pStyle = STATIC_DOWNCAST(CXTPChartPyramidSeriesStyle, GetStyle());

	CXTPChartDeviceCommand* pCommand = new CXTPChartPolygonAntialiasingDeviceCommand();

	CXTPChartPoints arrPoints;

	arrPoints.Add(CXTPChartPointF((rcBounds.left + rcBounds.right) / 2, rcBounds.top));

	arrPoints.Add(CXTPChartPointF(rcBounds.right, rcBounds.bottom ));
	arrPoints.Add(CXTPChartPointF(rcBounds.left, rcBounds.bottom));

	pCommand->AddChildCommand(pStyle->GetFillStyle()->CreateDeviceCommand(arrPoints, color1, color2));

	if (pStyle->GetBorder()->IsVisible())
		pCommand->AddChildCommand(new CXTPChartBoundedPolygonDeviceCommand(arrPoints, clrBorder, 1));


	return pCommand;
}

CXTPChartDeviceCommand* CXTPChartPyramidSeriesView::CreateLegendDeviceCommand(CXTPChartDeviceContext* pDC, CRect rcBounds)
{
	CXTPChartColor color1 = GetColor();
	CXTPChartColor color2 = GetColor2();

	CXTPChartColor clrBorder;

	CXTPChartPyramidSeriesStyle* pStyle = STATIC_DOWNCAST(CXTPChartPyramidSeriesStyle, GetStyle());

	if (!pStyle->GetBorder()->GetColor().IsEmpty())
		clrBorder = pStyle->GetBorder()->GetColor();
	else
		clrBorder  = GetColor().GetDarkColor();

	return CreateLegendDeviceCommand(pDC, rcBounds, color1, color2, clrBorder);

}

CXTPChartDeviceCommand* CXTPChartPyramidSeriesPointView::CreateLegendDeviceCommand(CXTPChartDeviceContext* pDC, CRect rcBounds)
{
	CXTPChartColor color1 = GetColor();
	CXTPChartColor color2 = GetColor2();
	CXTPChartColor clrBorder = GetBorderActualColor();

	return ((CXTPChartPyramidSeriesView*)GetSeriesView())->CreateLegendDeviceCommand(pDC, rcBounds,
		color1, color2, clrBorder);
}



#ifdef _XTP_ACTIVEX


BEGIN_DISPATCH_MAP(CXTPChartPyramidSeriesStyle, CXTPChartSeriesStyle)
	DISP_PROPERTY_NOTIFY_ID(CXTPChartPyramidSeriesStyle, "PointDistance", 100, m_nPointDistance, OleChartChanged, VT_I4)
	DISP_PROPERTY_NOTIFY_ID(CXTPChartPyramidSeriesStyle, "HeightToWidthRatio", 101, m_dHeightToWidthRatio, OleChartChanged, VT_R8)
END_DISPATCH_MAP()


// {487BCC77-27BF-4cb1-9ABF-4558D9835223}
static const GUID IID_IChartPyramidSeriesStyle =
{ 0x487bcc77, 0x27bf, 0x4cb1, { 0x9a, 0xbf, 0x45, 0x58, 0xd9, 0x83, 0x52, 0x23 } };

BEGIN_INTERFACE_MAP(CXTPChartPyramidSeriesStyle, CXTPChartSeriesStyle)
	INTERFACE_PART(CXTPChartPyramidSeriesStyle, IID_IChartPyramidSeriesStyle, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartPyramidSeriesStyle, IID_IChartPyramidSeriesStyle)

// {48890E6F-92B6-4671-9613-6B2A0FBF80A8}
IMPLEMENT_OLECREATE_EX2(CXTPChartPyramidSeriesStyle, "Codejock.ChartPyramidSeriesStyle." _XTP_AXLIB_VERSION,
0x48890e6f, 0x92b6, 0x4671, 0x96, 0x13, 0x6b, 0x2a, 0xf, 0xbf, 0x80, 0xa8);


#endif
