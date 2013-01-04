// XTPChartDiagram.cpp
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

#include "Common/XTPResourceManager.h"
#include "Resource.h"

#include "Types/XTPChartTypes.h"
#include "XTPChartDefines.h"
#include "XTPChartElement.h"
#include "XTPChartPanel.h"
#include "XTPChartElementView.h"
#include "XTPChartDiagram.h"
#include "XTPChartDiagramView.h"
#include "XTPChartSeriesView.h"
#include "XTPChartSeries.h"
#include "XTPChartTitle.h"

#include "Drawing/XTPChartDeviceCommand.h"


IMPLEMENT_DYNAMIC(CXTPChartDiagram, CXTPChartPanel)

CXTPChartDiagram::CXTPChartDiagram()
{

}


void CXTPChartDiagram::Release()
{
	m_pOwner = NULL;

	for (int i = (int)m_arrSeries.GetSize() - 1; i >= 0; i--)
	{
		m_arrSeries[i]->SetDiagram(NULL);
	}

	CXTPChartPanel::Release();
}

CXTPChartDiagramView* CXTPChartDiagram::CreateView(CXTPChartDeviceContext* pDC, CXTPChartElementView* pParent)
{
	UNREFERENCED_PARAMETER(pDC);
	UNREFERENCED_PARAMETER(pParent);
	return NULL;
}

void CXTPChartDiagram::CalculateSeriesLayout(CXTPChartDeviceContext* pDC, CXTPChartDiagramView* pView)
{
	CXTPChartElementView* pSeriesView = pView->GetSeriesView();

	for (int i = 0; i < pSeriesView->GetCount(); i++)
	{
		CXTPChartSeriesView* pView = (CXTPChartSeriesView*)pSeriesView->GetAt(i);

		pView->CalculateLayout(pDC);
	}

}


BOOL CXTPChartDiagram::OnSeriesAdded(CXTPChartSeries* pSeries)
{
	m_arrSeries.Add(pSeries);
	return TRUE;
}

void CXTPChartDiagram::OnSeriesRemoved(CXTPChartSeries* pSeries)
{
	for (int i = 0; i < m_arrSeries.GetSize(); i++)
	{
		if (m_arrSeries.GetAt(i) == pSeries)
		{
			m_arrSeries.RemoveAt(i);
			break;
		}
	}

	if (m_arrSeries.GetSize() == 0)
	{
		Remove();
	}
}

void CXTPChartDiagram::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPChartPanel::DoPropExchange(pPX);
}



//////////////////////////////////////////////////////////////////////////
// CXTPChartDiagramView

CXTPChartDiagramView::CXTPChartDiagramView(CXTPChartDiagram* pDiagram, CXTPChartElementView* pParentView)
	: CXTPChartElementView(pParentView)
{
	m_pDiagram = pDiagram;
	m_rcBounds.SetRectEmpty();
	m_pLabelsView = NULL;
	m_pSeriesView = NULL;

	m_hcurNormalHand = XTPResourceManager()->LoadCursor(XTP_IDC_CHART_NORMALHAND);
	m_hcurDragHand = XTPResourceManager()->LoadCursor(XTP_IDC_CHART_DRAGHAND);


	m_pTitlesView = new CXTPChartElementView(this);
}

void CXTPChartDiagramView::CreateView(CXTPChartDeviceContext* pDC)
{
	m_pDiagram->GetTitles()->CreateView(pDC, m_pTitlesView);
}

void CXTPChartDiagramView::CalculateView(CXTPChartDeviceContext* pDC, CRect rcBounds)
{
	m_pDiagram->GetTitles()->CalculateView(pDC, rcBounds, m_pTitlesView);

	m_rcBounds = rcBounds;
	if (!m_pSeriesView)
		return;

	m_pDiagram->CalculateSeriesLayout(pDC, this);
}

void CXTPChartDiagramView::UpdateRange(CXTPChartDeviceContext* /*pDC*/)
{

}

CXTPChartDeviceCommand* CXTPChartDiagramView::CreateDeviceCommand(CXTPChartDeviceContext* pDC)
{
	CXTPChartDeviceCommand* pCommand = new CXTPChartHitTestElementCommand(m_pDiagram, m_rcBounds);

	pCommand->AddChildCommand(CXTPChartElementView::CreateDeviceCommand(pDC));

	return pCommand;
}

void CXTPChartDiagramView::OnMouseWheel(UINT /*nFlags*/, short /*zDelta*/, CPoint /*pt*/)
{

}

