// XTPChartFunnelSeriesStyle.cpp
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
#include "../../XTPChartSeriesStyle.h"
#include "../../XTPChartSeriesPoint.h"
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

#include "XTPChartFunnelSeriesStyle.h"
#include "XTPChartFunnelSeriesView.h"
#include "XTPChartFunnelDiagram.h"
#include "XTPChartFunnelSeriesLabel.h"



//////////////////////////////////////////////////////////////////////////
// CXTPChartFunnelSeriesStyleBase

IMPLEMENT_SERIAL(CXTPChartFunnelSeriesStyle, CXTPChartSeriesStyle, VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)


CXTPChartFunnelSeriesStyle::CXTPChartFunnelSeriesStyle()
{
	m_nPointDistance = 4;
	m_dHeightToWidthRatio = 1;

	m_bEqualHeight = FALSE;

	m_pBorder = new CXTPChartBorder(this);

	m_pFillStyle = new CXTPChartFillStyle(this);
	m_pFillStyle->SetFillMode(xtpChartFillGradient);
	m_pFillStyle->SetGradientAngle(xtpChartGradientAngle90);

	m_bColorEach = TRUE;

	SetLabel(new CXTPChartFunnelSeriesLabel());
}

CXTPChartFunnelSeriesStyle::~CXTPChartFunnelSeriesStyle()
{
	SAFE_RELEASE(m_pFillStyle);
	SAFE_RELEASE(m_pBorder);
}

void CXTPChartFunnelSeriesStyle::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPChartSeriesStyle::DoPropExchange(pPX);

	PX_Int(pPX, _T("PointDistance"), m_nPointDistance, 0);

	PX_Double(pPX, _T("HeightToWidthRatio"), m_dHeightToWidthRatio, 1);

	PX_Bool(pPX, _T("EqualHeight"), m_bEqualHeight, FALSE);

	CXTPPropExchangeSection secBorder(pPX->GetSection(_T("Border")));
	m_pBorder->DoPropExchange(&secBorder);

	CXTPPropExchangeSection secFillStyle(pPX->GetSection(_T("FillStyle")));
	m_pFillStyle->DoPropExchange(&secFillStyle);

}

CXTPChartDiagram* CXTPChartFunnelSeriesStyle::CreateDiagram()
{
	return new CXTPChartFunnelDiagram();
}

CXTPChartSeriesView* CXTPChartFunnelSeriesStyle::CreateView(CXTPChartSeries* pSeries, CXTPChartDiagramView* pDiagramView)
{
	return new CXTPChartFunnelSeriesView(pSeries, pDiagramView);
}

BOOL CXTPChartFunnelSeriesStyle::IsStyleDiagram(CXTPChartDiagram* pDiagram) const
{
	if (DYNAMIC_DOWNCAST(CXTPChartFunnelDiagram, pDiagram) == NULL)
		return FALSE;

	return pDiagram->GetSeries().GetSize() == 0;
}





//////////////////////////////////////////////////////////////////////////
// CXTPChartFunnelDiagramSeriesView

CXTPChartFunnelSeriesView::CXTPChartFunnelSeriesView(CXTPChartSeries* pSeries, CXTPChartDiagramView* pDiagramView)
	: CXTPChartSeriesView(pSeries, pDiagramView)
{

}

CXTPChartFunnelSeriesView::~CXTPChartFunnelSeriesView()
{

}

void CXTPChartFunnelSeriesView::CalculatePointLayout(CXTPChartDeviceContext* pDC, CRect rcBounds)
{
	CXTPChartFunnelSeriesStyle* pStyle = STATIC_DOWNCAST(CXTPChartFunnelSeriesStyle, GetStyle());

	rcBounds.DeflateRect(5, 5, 5, 5);


	CXTPChartFunnelSeriesLabel* pLabel = (CXTPChartFunnelSeriesLabel*)pStyle->GetLabel();

	if (pLabel->IsVisible() && !pLabel->IsInside())
	{
		CSize sz(0, 0);
		for (int i = 0; i < m_pPointsView->GetCount(); i++)
		{
			CXTPChartFunnelSeriesPointView* pPointView = (CXTPChartFunnelSeriesPointView*)m_pPointsView->GetAt(i);

			CXTPChartString text(pLabel->GetPointLabel(pPointView->GetPoint()));
			CXTPChartTextPainter painter(pDC, text, pLabel);

			sz.cx = max(sz.cx, (INT)painter.GetSize().Width);
			sz.cy = max(sz.cy, (INT)painter.GetSize().Height);
		}

		int nLineLength = pStyle->GetLabel()->GetLineLength();
		if (pLabel->GetPosition() == xtpChartFunnelLabelRight)
			rcBounds.DeflateRect(0, 0, nLineLength + sz.cx, 0);
		if (pLabel->GetPosition() == xtpChartFunnelLabelLeft)
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
		CXTPChartFunnelSeriesPointView* pPointView = (CXTPChartFunnelSeriesPointView*)m_pPointsView->GetAt(i);

		if (pStyle->IsEqualHeight())
		{
			pPointView->CalculateLayout(i, nCount, pPointView->m_dValue, i == nCount - 1 ? pPointView->m_dValue :
				((CXTPChartFunnelSeriesPointView*)m_pPointsView->GetAt(i + 1))->m_dValue);
		}
		else
		{
			pPointView->CalculateLayout(i, nCount, dFrom, dFrom + pPointView->m_dValue);
			dFrom += pPointView->m_dValue;

		}
	}
}


void CXTPChartFunnelSeriesView::CalculateLabelLayout(CXTPChartDeviceContext* pDC)
{
	for (int i = 0; i < m_pLabelsView->GetCount(); i++)
	{
		CXTPChartSeriesLabelView* pLabelView = (CXTPChartSeriesLabelView*)m_pLabelsView->GetAt(i);

		pLabelView->CalculateLayout(pDC);
	}
}

CXTPChartSeriesPointView* CXTPChartFunnelSeriesView::CreateSeriesPointView(CXTPChartDeviceContext* pDC, CXTPChartSeriesPoint* pPoint, CXTPChartElementView* pParentView)
{
	UNREFERENCED_PARAMETER(pDC);
	return new CXTPChartFunnelSeriesPointView(pPoint, pParentView);
}


void CXTPChartFunnelSeriesView::CalculateValues()
{
	CXTPChartFunnelSeriesStyle* pStyle = STATIC_DOWNCAST(CXTPChartFunnelSeriesStyle, GetStyle());

	if (pStyle->IsEqualHeight())
	{
		double dMax = 0;
		int i;

		for (i = 0; i < m_pPointsView->GetCount(); i++)
		{
			CXTPChartFunnelSeriesPointView* pPointView = (CXTPChartFunnelSeriesPointView*)m_pPointsView->GetAt(i);

			CXTPChartSeriesPoint* pPoint = pPointView->GetPoint();

			double dValue = pPoint->GetValue(0);

			dMax = max(dMax, dValue);
		}

		if (dMax == 0)
			dMax = 1;

		for (i = 0; i < m_pPointsView->GetCount(); i++)
		{
			CXTPChartFunnelSeriesPointView* pPointView = (CXTPChartFunnelSeriesPointView*)m_pPointsView->GetAt(i);

			CXTPChartSeriesPoint* pPoint = pPointView->GetPoint();

			double dValue = pPoint->GetValue(0);

			pPointView->m_dValue = dValue >= 0 ? dValue / dMax : 0;
		}
	}
	else
	{
		double dSum = 0;
		int i;

		for (i = 0; i < m_pPointsView->GetCount(); i++)
		{
			CXTPChartFunnelSeriesPointView* pPointView = (CXTPChartFunnelSeriesPointView*)m_pPointsView->GetAt(i);

			CXTPChartSeriesPoint* pPoint = pPointView->GetPoint();

			double dValue = pPoint->GetValue(0);

			dSum += dValue;
		}

		if (dSum == 0)
			dSum = 1;

		for (i = 0; i < m_pPointsView->GetCount(); i++)
		{
			CXTPChartFunnelSeriesPointView* pPointView = (CXTPChartFunnelSeriesPointView*)m_pPointsView->GetAt(i);

			CXTPChartSeriesPoint* pPoint = pPointView->GetPoint();

			double dValue = pPoint->GetValue(0);

			pPointView->m_dValue = dValue >= 0 ? dValue / dSum : 0;
		}
	}
}

CXTPChartDeviceCommand* CXTPChartFunnelSeriesView::CreateDeviceCommand(CXTPChartDeviceContext* pDC)
{
	CXTPChartDeviceCommand* pCommand = new CXTPChartPolygonAntialiasingDeviceCommand();
	pCommand->AddChildCommand(CXTPChartSeriesView::CreateDeviceCommand(pDC));

	return pCommand;
}

//////////////////////////////////////////////////////////////////////////
// CXTPChartFunnelDiagramSeriesPointView

CXTPChartFunnelSeriesPointView::CXTPChartFunnelSeriesPointView(CXTPChartSeriesPoint* pPoint, CXTPChartElementView* pParentView)
: CXTPChartSeriesPointView(pPoint, pParentView)
{
	m_dValue = 0;
}

CXTPChartFunnelSeriesPointView::~CXTPChartFunnelSeriesPointView()
{
}

void CXTPChartFunnelSeriesPointView::CalculateLayout(int nIndex, int nCount, double dFrom, double dTo)
{
	CXTPChartFunnelSeriesView* pView = (CXTPChartFunnelSeriesView*)GetSeriesView();
	CXTPChartFunnelSeriesStyle* pStyle = STATIC_DOWNCAST(CXTPChartFunnelSeriesStyle, GetSeriesView()->GetStyle());

	CXTPChartRectF rcInnerRect = pView->GetInnerBounds();

	if (pStyle->IsEqualHeight())
	{
		float Height = (rcInnerRect.Height - (nCount - 1) * pStyle->GetPointDistance() ) / nCount;

		m_rc.X = rcInnerRect.X;
		m_rc.Y = (float)CXTPChartMathUtils::Round(rcInnerRect.Y +(Height + pStyle->GetPointDistance())  * nIndex);

		int bottom = (int)CXTPChartMathUtils::Round(rcInnerRect.Y + (Height + pStyle->GetPointDistance()) * nIndex + Height);
		m_rc.Height = bottom - m_rc.Y;
		m_rc.Width = rcInnerRect.Width;

		m_dFrom = dFrom;
		m_dTo = dTo;
	}
	else
	{
		m_rc.X = rcInnerRect.X;

		double dPosFrom = rcInnerRect.Height * dFrom;
		double dPosTo = rcInnerRect.Height * dTo;

		m_rc.Y = (float)CXTPChartMathUtils::Round(rcInnerRect.Y + dPosFrom);

		int bottom = (int)CXTPChartMathUtils::Round(rcInnerRect.Y + dPosTo);

		if (nIndex != nCount -1)
			bottom -= pStyle->GetPointDistance();

		m_rc.Height = max(1.0f, bottom - m_rc.Y);
		m_rc.Width = rcInnerRect.Width;

		dPosTo = m_rc.GetBottom() - rcInnerRect.Y;


		int nHandHeight = int(rcInnerRect.Height / 3);
		int nHandWidth = int(nHandHeight * 0.618);


		if (dPosFrom < rcInnerRect.Height - nHandHeight)
		{
			m_dFrom = (nHandWidth / rcInnerRect.Width - 1) / (rcInnerRect.Height - nHandHeight) * dPosFrom + 1;
		}
		else
		{
			m_dFrom = nHandWidth / rcInnerRect.Width;
		}

		if (dPosTo < rcInnerRect.Height - nHandHeight)
		{
			m_dTo = (nHandWidth / rcInnerRect.Width - 1) / (rcInnerRect.Height - nHandHeight) * dPosTo + 1;
		}
		else
		{
			m_dTo = nHandWidth / rcInnerRect.Width;
		}

	}
}


CXTPChartColor CXTPChartFunnelSeriesPointView::GetBorderActualColor() const
{
	CXTPChartFunnelSeriesStyle* pStyle = STATIC_DOWNCAST(CXTPChartFunnelSeriesStyle, GetSeriesView()->GetStyle());

	if (!pStyle->GetBorder()->GetColor().IsEmpty())
		return pStyle->GetBorder()->GetColor();

	CXTPChartColor clr = GetColor().GetDarkColor();
	return clr;
}

CXTPChartDeviceCommand* CXTPChartFunnelSeriesPointView::CreateDeviceCommand(CXTPChartDeviceContext* pDC)
{
	UNREFERENCED_PARAMETER(pDC);

	CXTPChartColor color1 = GetColor();
	CXTPChartColor color2 = GetColor2();
	CXTPChartColor clrBorder = GetBorderActualColor();

	CXTPChartDeviceCommand* pCommand = new CXTPChartHitTestElementCommand(m_pPoint);

	CXTPChartFunnelSeriesStyle* pStyle = STATIC_DOWNCAST(CXTPChartFunnelSeriesStyle, GetSeriesView()->GetStyle());

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

CXTPChartDeviceCommand* CXTPChartFunnelSeriesView::CreateLegendDeviceCommand(CXTPChartDeviceContext* pDC, CRect rcBounds,
	CXTPChartColor color1, CXTPChartColor color2, CXTPChartColor clrBorder)
{
	UNREFERENCED_PARAMETER(pDC);
	rcBounds.DeflateRect(1, 1);

	CXTPChartFunnelSeriesStyle* pStyle = STATIC_DOWNCAST(CXTPChartFunnelSeriesStyle, GetStyle());

	CXTPChartDeviceCommand* pCommand = new CXTPChartPolygonAntialiasingDeviceCommand();

	CXTPChartPoints arrPoints;

	int sz = rcBounds.Height() / 3;
	int hsz = (rcBounds.Width() - sz) / 2 - 1;

	arrPoints.Add(CXTPChartPointF(rcBounds.left, rcBounds.top));
	arrPoints.Add(CXTPChartPointF(rcBounds.right, rcBounds.top));

	arrPoints.Add(CXTPChartPointF(rcBounds.right - hsz, rcBounds.bottom - sz));
	arrPoints.Add(CXTPChartPointF(rcBounds.right - hsz, rcBounds.bottom));

	arrPoints.Add(CXTPChartPointF(rcBounds.left + hsz, rcBounds.bottom));
	arrPoints.Add(CXTPChartPointF(rcBounds.left + hsz, rcBounds.bottom - sz));

	pCommand->AddChildCommand(pStyle->GetFillStyle()->CreateDeviceCommand(arrPoints, color1, color2));

	if (pStyle->GetBorder()->IsVisible())
		pCommand->AddChildCommand(new CXTPChartBoundedPolygonDeviceCommand(arrPoints, clrBorder, 1));


	return pCommand;
}

CXTPChartDeviceCommand* CXTPChartFunnelSeriesView::CreateLegendDeviceCommand(CXTPChartDeviceContext* pDC, CRect rcBounds)
{
	CXTPChartColor color1 = GetColor();
	CXTPChartColor color2 = GetColor2();

	CXTPChartColor clrBorder;

	CXTPChartFunnelSeriesStyle* pStyle = STATIC_DOWNCAST(CXTPChartFunnelSeriesStyle, GetStyle());

	if (!pStyle->GetBorder()->GetColor().IsEmpty())
		clrBorder = pStyle->GetBorder()->GetColor();
	else
		clrBorder  = GetColor().GetDarkColor();

	return CreateLegendDeviceCommand(pDC, rcBounds, color1, color2, clrBorder);

}

CXTPChartDeviceCommand* CXTPChartFunnelSeriesPointView::CreateLegendDeviceCommand(CXTPChartDeviceContext* pDC, CRect rcBounds)
{
	CXTPChartColor color1 = GetColor();
	CXTPChartColor color2 = GetColor2();
	CXTPChartColor clrBorder = GetBorderActualColor();

	return ((CXTPChartFunnelSeriesView*)GetSeriesView())->CreateLegendDeviceCommand(pDC, rcBounds,
		color1, color2, clrBorder);
}



#ifdef _XTP_ACTIVEX


BEGIN_DISPATCH_MAP(CXTPChartFunnelSeriesStyle, CXTPChartSeriesStyle)
	DISP_PROPERTY_NOTIFY_ID(CXTPChartFunnelSeriesStyle, "PointDistance", 100, m_nPointDistance, OleChartChanged, VT_I4)
	DISP_PROPERTY_NOTIFY_ID(CXTPChartFunnelSeriesStyle, "HeightToWidthRatio", 101, m_dHeightToWidthRatio, OleChartChanged, VT_R8)
	DISP_PROPERTY_NOTIFY_ID(CXTPChartFunnelSeriesStyle, "EqualHeight", 102, m_bEqualHeight, OleChartChanged, VT_BOOL)
END_DISPATCH_MAP()


// {477BCC77-27BF-4cb1-9ABF-4558D9835223}
static const GUID IID_IChartFunnelSeriesStyle =
{ 0x477bcc77, 0x27bf, 0x4cb1, { 0x9a, 0xbf, 0x45, 0x58, 0xd9, 0x83, 0x52, 0x23 } };

BEGIN_INTERFACE_MAP(CXTPChartFunnelSeriesStyle, CXTPChartSeriesStyle)
	INTERFACE_PART(CXTPChartFunnelSeriesStyle, IID_IChartFunnelSeriesStyle, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartFunnelSeriesStyle, IID_IChartFunnelSeriesStyle)

// {47890E6F-92B6-4671-9613-6B2A0FBF80A8}
IMPLEMENT_OLECREATE_EX2(CXTPChartFunnelSeriesStyle, "Codejock.ChartFunnelSeriesStyle." _XTP_AXLIB_VERSION,
0x47890e6f, 0x92b6, 0x4671, 0x96, 0x13, 0x6b, 0x2a, 0xf, 0xbf, 0x80, 0xa8);


#endif
