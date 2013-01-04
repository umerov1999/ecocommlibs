// XTPChartDiagram2DPane.cpp
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
#include "Common/Base/Types/XTPPoint3.h"

#include "../../XTPChartDefines.h"
#include "../../XTPChartElement.h"
#include "../../XTPChartElementView.h"
#include "../../XTPChartPanel.h"
#include "../../XTPChartDiagram.h"
#include "../../XTPChartDiagramView.h"
#include "../../XTPChartSeriesView.h"

#include "../../Drawing/XTPChartDeviceCommand.h"
#include "../../Drawing/XTPChartRectangleDeviceCommand.h"
#include "../../Drawing/XTPChartTransformationDeviceCommand.h"
#include "../../Appearance/XTPChartAppearance.h"
#include "../../Appearance/XTPChartFillStyle.h"

#include "XTPChartDiagram2D.h"
#include "XTPChartDiagram2DView.h"
#include "XTPChartDiagram2DPane.h"
#include "../Axis/XTPChartAxisView.h"
#include "XTPChartDiagram2DSeriesView.h"


using namespace Gdiplus;

IMPLEMENT_DYNAMIC(CXTPChartDiagram2DPane, CXTPChartElement);

CXTPChartDiagram2DPane::CXTPChartDiagram2DPane(CXTPChartDiagram* pDiagram)
{
	m_pOwner = pDiagram;

	m_pBackgroundFillStyle = new CXTPChartFillStyle(this);
	m_pBackgroundFillStyle->SetFillMode(xtpChartFillEmpty);
}

CXTPChartDiagram2DPane::~CXTPChartDiagram2DPane()
{
	SAFE_RELEASE(m_pBackgroundFillStyle);
}

CXTPChartElementView* CXTPChartDiagram2DPane::CreateView(CXTPChartDeviceContext* pDC, CXTPChartElementView* pParent)
{
	UNREFERENCED_PARAMETER(pDC);

	CXTPChartDiagram2DPaneView* pView = new CXTPChartDiagram2DPaneView(this, pParent);

	return pView;
}

CXTPChartDiagram2DAppearance* CXTPChartDiagram2DPane::GetAppearance() const
{
	return CXTPChartAppearance::GetAppearance(this)->GetDiagram2DAppearance();
}

CXTPChartColor CXTPChartDiagram2DPane::GetActualBackgroundColor() const
{
	if (!m_clrBackgroundColor.IsEmpty())
		return m_clrBackgroundColor;

	return GetAppearance()->BackgroundColor;
}

CXTPChartColor CXTPChartDiagram2DPane::GetActualBackgroundColor2() const
{
	if (!m_clrBackgroundColor2.IsEmpty())
		return m_clrBackgroundColor2;

	return GetAppearance()->BackgroundColor2;
}

CXTPChartColor CXTPChartDiagram2DPane::GetActualBorderColor() const
{
	if (!m_clrBorderColor.IsEmpty())
		return m_clrBorderColor;

	return GetAppearance()->BorderColor;
}

CXTPChartColor CXTPChartDiagram2DPane::GetBackgroundColor() const
{
	return m_clrBackgroundColor;
}

CXTPChartColor CXTPChartDiagram2DPane::GetBackgroundColor2() const
{
	return m_clrBackgroundColor2;
}

CXTPChartColor CXTPChartDiagram2DPane::GetBorderColor() const
{
	return m_clrBorderColor;
}



CXTPChartFillStyle* CXTPChartDiagram2DPane::GetActualFillStyle() const
{
	if (m_pBackgroundFillStyle->GetFillMode() != xtpChartFillEmpty)
		return m_pBackgroundFillStyle;

	return GetAppearance()->BackgroundFillStyle;
}

void CXTPChartDiagram2DPane::DoPropExchange(CXTPPropExchange* pPX)
{
	PX_Color(pPX, _T("BackgroundColor"), m_clrBackgroundColor);
	PX_Color(pPX, _T("BackgroundColor2"), m_clrBackgroundColor2);
	PX_Color(pPX, _T("BorderColor"), m_clrBorderColor);

	CXTPPropExchangeSection secFillStyle(pPX->GetSection(_T("BackgroundFillStyle")));
	m_pBackgroundFillStyle->DoPropExchange(&secFillStyle);

}


//////////////////////////////////////////////////////////////////////////
// CXTPChartDiagram2DPaneView

CXTPChartDiagram2DPaneView::CXTPChartDiagram2DPaneView(CXTPChartDiagram2DPane* pPane, CXTPChartElementView* pParentView)
	: CXTPChartElementView(pParentView)
{
	m_pPane = pPane;

	m_ptOldPosition = CPoint(-1, -1);
}

CXTPChartDiagram2DPaneView::~CXTPChartDiagram2DPaneView()
{

}

void CXTPChartDiagram2DPaneView::CalculateView(CRect rcBounds)
{
	m_rcBounds = rcBounds;
}

CXTPChartAxisView* CXTPChartDiagram2DPaneView::GetAxisView(CXTPChartAxis* pAxis) const
{
	CXTPChartDiagram2DView* pDiagramView = (CXTPChartDiagram2DView*)m_pParentView->GetParentView();

	CXTPChartElementView* pAxisViews =  pDiagramView->m_pAxisViews;

	for (int i = 0; i < pAxisViews->GetCount(); i++)
	{
		CXTPChartAxisView* pAxisView = (CXTPChartAxisView*)pAxisViews->GetAt(i);
		if (pAxisView->GetAxis() == pAxis)
			return pAxisView;
	}
	return NULL;
}

CXTPChartDeviceCommand* CXTPChartDiagram2DPaneView::CreateGridLinesDeviceCommand(CXTPChartDeviceContext* pDC, CXTPChartDiagram2DAxisView* pAxisView)
{
	return pAxisView->CreateGridLinesDeviceCommand(pDC, m_rcBounds);
}

CXTPChartDeviceCommand* CXTPChartDiagram2DPaneView::CreateConstantLinesDeviceCommand(CXTPChartDeviceContext* pDC, CXTPChartDiagram2DAxisView* pAxisView, BOOL bBehind)
{
	return pAxisView->CreateConstantLinesDeviceCommand(pDC, m_rcBounds, bBehind);
}

CXTPChartDeviceCommand* CXTPChartDiagram2DPaneView::CreateStripsDeviceCommand(CXTPChartDeviceContext* pDC, CXTPChartDiagram2DAxisView* pAxisView)
{
	return pAxisView->CreateStripsDeviceCommand(pDC, m_rcBounds);
}


CXTPChartDeviceCommand* CXTPChartDiagram2DPaneView::CreateInterlacedDeviceCommand(CXTPChartDeviceContext* pDC, CXTPChartDiagram2DAxisView* pAxisView)
{
	return pAxisView->CreateInterlacedDeviceCommand(pDC, m_rcBounds);
}

CXTPChartDeviceCommand* CXTPChartDiagram2DPaneView::CreateDeviceCommand(CXTPChartDeviceContext* pDC)
{
	CXTPChartDeviceCommand* pCommand = new CXTPChartHitTestElementCommand(m_pPane, m_rcBounds);

	pCommand->AddChildCommand(m_pPane->GetActualFillStyle()->CreateDeviceCommand(m_rcBounds,
		m_pPane->GetActualBackgroundColor(), m_pPane->GetActualBackgroundColor2()));

	CXTPChartDiagram2DView* pDiagramView = (CXTPChartDiagram2DView*)m_pParentView->GetParentView();

	CXTPChartElementView* pAxisViews =  pDiagramView->m_pAxisViews;

	pCommand->AddChildCommand(new CXTPChartBoundedRectangleDeviceCommand(m_rcBounds, m_pPane->GetActualBorderColor(), 1));

	CRect rcBounds(m_rcBounds);
	rcBounds.DeflateRect(1, 1, 0, 0);

	CXTPChartDeviceCommand* pClipDeviceCommand = new CXTPChartClipDeviceCommand(rcBounds);
	pCommand->AddChildCommand(pClipDeviceCommand);


	int i;

	for (i = 0; i < pAxisViews->GetCount(); i++)
	{
		CXTPChartDiagram2DAxisView* pAxisView = (CXTPChartDiagram2DAxisView*)pAxisViews->GetAt(i);
		pClipDeviceCommand->AddChildCommand(CreateInterlacedDeviceCommand(pDC,pAxisView));
	}

	for (i = 0; i < pAxisViews->GetCount(); i++)
	{
		CXTPChartDiagram2DAxisView* pAxisView = (CXTPChartDiagram2DAxisView*)pAxisViews->GetAt(i);
		pClipDeviceCommand->AddChildCommand(CreateStripsDeviceCommand(pDC,pAxisView));
	}

	for (i = 0; i < pAxisViews->GetCount(); i++)
	{
		CXTPChartDiagram2DAxisView* pAxisView = (CXTPChartDiagram2DAxisView*)pAxisViews->GetAt(i);
		pClipDeviceCommand->AddChildCommand(CreateGridLinesDeviceCommand(pDC,pAxisView));
	}

	for (i = 0; i < pAxisViews->GetCount(); i++)
	{
		CXTPChartDiagram2DAxisView* pAxisView = (CXTPChartDiagram2DAxisView*)pAxisViews->GetAt(i);
		pClipDeviceCommand->AddChildCommand(CreateConstantLinesDeviceCommand(pDC,pAxisView, TRUE));
	}

	pClipDeviceCommand->AddChildCommand(m_pSeriesView->CreateDeviceCommand(pDC));

	for (i = 0; i < pAxisViews->GetCount(); i++)
	{
		CXTPChartDiagram2DAxisView* pAxisView = (CXTPChartDiagram2DAxisView*)pAxisViews->GetAt(i);
		pClipDeviceCommand->AddChildCommand(CreateConstantLinesDeviceCommand(pDC,pAxisView, FALSE));
	}


	return pCommand;

}

CXTPChartPointF CXTPChartDiagram2DPaneView::GetScreenPoint(const CXTPChartSeriesView* pView, double x, double y) const
{
	CRect rcBounds = GetBounds();

	CXTPChartPointF res;

	CXTPChartDiagram2DAxisView* pAxisViewX = ((CXTPChartDiagram2DSeriesView*)pView)->m_pAxisViewX;
	CXTPChartDiagram2DAxisView* pAxisViewY = ((CXTPChartDiagram2DSeriesView*)pView)->m_pAxisViewY;

	res.X = (float)pAxisViewX->ValueToPoint(x);
	res.Y = (float)pAxisViewY->ValueToPoint(y);

	CXTPChartDiagram2DView* pDiagramView = (CXTPChartDiagram2DView*)m_pParentView->GetParentView();
	CXTPChartDiagram2D* pDiagram = (CXTPChartDiagram2D*)pDiagramView->GetDiagram();

	if (pDiagram->IsRotated())
	{
		res = CXTPChartPointF(res.Y, res.X);
	}

	return res;
}


CXTPChartElementView* CXTPChartDiagram2DPaneView::CreateSeriesView()
{
	m_pSeriesView = new CXTPChartElementView(this);

	return m_pSeriesView;
}

void CXTPChartDiagram2DPaneView::OnLButtonDown(UINT /*nFlags*/, CPoint point)
{
	CXTPChartDiagram2DView* pDiagramView = (CXTPChartDiagram2DView*)m_pParentView->GetParentView();

	BOOL bScrollBarFound = FALSE;
	for (int i = 0; i < pDiagramView->m_pAxisViews->GetCount(); i++)
	{
		CXTPChartDiagram2DAxisView* pAxisView = (CXTPChartDiagram2DAxisView*)pDiagramView->m_pAxisViews->GetAt(i);

		if (pAxisView->IsScollBarVisible())
		{
			bScrollBarFound = TRUE;
		}
	}
	if (!bScrollBarFound)
		return;


	CXTPChartContainer* pContainer = m_pContainer;
	ASSERT (pContainer);

	m_pContainer->SetCapture(this);
	m_ptOldPosition = point;

	::SetCursor(pDiagramView->m_hcurDragHand);
}

void CXTPChartDiagram2DPaneView::OnMouseMove(UINT /*nFlags*/, CPoint point)
{
	int dx = point.x - m_ptOldPosition.x;
	int dy = point.y - m_ptOldPosition.y;
	m_ptOldPosition = point;

	CXTPChartDiagram2DView* pDiagramView = (CXTPChartDiagram2DView*)m_pParentView->GetParentView();

	for (int i = 0; i < pDiagramView->m_pAxisViews->GetCount(); i++)
	{
		CXTPChartDiagram2DAxisView* pAxisView = (CXTPChartDiagram2DAxisView*)pDiagramView->m_pAxisViews->GetAt(i);

		pAxisView->PerformPaneDragging(dx, dy);
	}

}

BOOL CXTPChartDiagram2DPaneView::OnSetCursor(CPoint /*point*/)
{
	CXTPChartDiagram2DView* pDiagramView = (CXTPChartDiagram2DView*)m_pParentView->GetParentView();

	::SetCursor(pDiagramView->m_hcurDragHand);
	return TRUE;
}
