// XTPChartContent.cpp
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


#include "Types/XTPChartTypes.h"

#include "XTPChartDefines.h"
#include "XTPChartElement.h"
#include "XTPChartElementView.h"
#include "XTPChartContent.h"
#include "XTPChartContentView.h"
#include "XTPChartPanel.h"
#include "XTPChartDiagram.h"
#include "XTPChartDiagramView.h"
#include "XTPChartTitle.h"
#include "XTPChartSeries.h"
#include "XTPChartSeriesView.h"
#include "XTPChartSeriesStyle.h"

#include "XTPChartLegend.h"

#include "Drawing/XTPChartDeviceCommand.h"
#include "Drawing/XTPChartRectangleDeviceCommand.h"
#include "Appearance/XTPChartBorder.h"


//////////////////////////////////////////////////////////////////////////
// CXTPChartContentView


CXTPChartContentView::CXTPChartContentView(CXTPChartContainer* pContainer, CXTPChartContent* pContent)
	: CXTPChartElementView(pContainer)
{
	m_pContent = pContent;
	m_rcBounds.SetRectEmpty();

	m_pLegendView = NULL;
	m_pTitlesView = NULL;
	m_pDiagramView = NULL;
}

CXTPChartDeviceCommand* CXTPChartContentView::CreateDeviceCommand(CXTPChartDeviceContext* pDC)
{
	CXTPChartDeviceCommand* pCommand = new CXTPChartHitTestElementCommand(m_pContent, m_rcBounds);

	pCommand->AddChildCommand(new CXTPChartSolidRectangleDeviceCommand(m_rcBounds, m_pContent->GetActualBackgroundColor()));

	CXTPChartDeviceCommand* pDrawingType = pCommand->AddChildCommand(new CXTPChartDeviceCommand());

	if (m_pContent->GetBorder()->IsVisible())
	{
		pDrawingType->AddChildCommand(m_pContent->GetBorder()->CreateInnerBorderDeviceCommand(m_rcBounds, m_pContent->GetActualBorderColor()));
	}

	if (m_pDiagramView)
	{
		pDrawingType->AddChildCommand(m_pDiagramView->CreateDeviceCommand(pDC));
	}

	if (m_pLegendView)
	{
		pDrawingType->AddChildCommand(m_pLegendView->CreateDeviceCommand(pDC));
	}

	if (m_pTitlesView)
	{
		pDrawingType->AddChildCommand(m_pTitlesView->CreateDeviceCommand(pDC));
	}

	return pCommand;
}


void CXTPChartContentView::CreateView(CXTPChartDeviceContext* pDC)
{
	m_pTitlesView = new CXTPChartElementView(this);

	m_pLegendView = m_pContent->GetLegend()->CreateView(pDC, this);

	m_pDiagramView = new CXTPChartElementView(this);

	for (int nPanel = 0; nPanel < m_pContent->GetPanels()->GetCount(); nPanel++)
	{
		CXTPChartPanel* pPanel = m_pContent->GetPanels()->GetAt(nPanel);

		CXTPChartDiagram* pDiagram = DYNAMIC_DOWNCAST(CXTPChartDiagram, pPanel);
		if (pDiagram == NULL)
			continue;

		CXTPChartDiagramView* pDiagramView = pDiagram->CreateView(pDC, m_pDiagramView);

		if (pDiagramView)
		{
			pDiagramView->CreateView(pDC);

			for (int i = 0; i < m_pContent->GetSeries()->GetCount(); i++)
			{
				CXTPChartSeries* pSeries = m_pContent->GetSeries()->GetAt(i);
				if (!pSeries->IsVisible())
					continue;

				if (pSeries->GetDiagram() != pDiagram)
					continue;


				CXTPChartSeriesView* pSeriesView = pSeries->GetStyle()->CreateView(pSeries, pDiagramView);

				if (pSeriesView)
				{
					pSeriesView->CreatePointsView(pDC);

					if (m_pLegendView)
					{
						pSeriesView->AddToLegend(m_pLegendView);
					}
				}
			}

			pDiagramView->UpdateRange(pDC);
		}
	}

	m_pContent->GetTitles()->CreateView(pDC, m_pTitlesView);

}

CXTPChartDiagramView* CXTPChartContentView::HitTestDiagramView(CPoint pt) const
{
	for (int i = 0; i < m_pDiagramView->GetCount(); i++)
	{
		CXTPChartDiagramView* pDiagramView = (CXTPChartDiagramView*)m_pDiagramView->GetAt(i);

		if (pDiagramView->GetBounds().PtInRect(pt))
			return pDiagramView;
	}
	return NULL;
}


void CXTPChartContentView::CalculateView(CXTPChartDeviceContext* pDC, CRect rcBounds)
{
	m_rcBounds = rcBounds;

	if (m_pContent->GetBorder()->IsVisible())
		rcBounds.DeflateRect(m_pContent->GetBorder()->GetThickness(), m_pContent->GetBorder()->GetThickness());

	m_pContent->GetTitles()->CalculateView(pDC, rcBounds, m_pTitlesView);

	rcBounds.DeflateRect(10, 10, 10, 10);


	if (m_pLegendView)
	{
		m_pLegendView->CalculateView(pDC, rcBounds);
	}

	int nCount = m_pDiagramView->GetCount();
	if (nCount == 0)
		return;

	int nGap = m_pContent->GetPanelDistance();

	int nSize = m_pContent->GetPanelDirection() == xtpChartPanelHorizontal ?
		(rcBounds.Width() - nGap * (nCount - 1)) / nCount :
		(rcBounds.Height() - nGap * (nCount - 1)) / nCount;


	for (int i = 0; i < nCount; i++)
	{
		CXTPChartDiagramView* pDiagramView = (CXTPChartDiagramView*)m_pDiagramView->GetAt(i);

		CRect rcDiagram =
			m_pContent->GetPanelDirection() == xtpChartPanelHorizontal ?
				CRect(rcBounds.left + i * (nSize + nGap), rcBounds.top, rcBounds.left + i * (nSize + nGap) + nSize, rcBounds.bottom) :
				CRect(rcBounds.left, rcBounds.top + i * (nSize + nGap), rcBounds.right, rcBounds.top + i * (nSize + nGap) + nSize);

		pDiagramView->CalculateView(pDC, rcDiagram);
	}
}

CXTPChartLegendView* CXTPChartContentView::GetLegendView() const
{
	return m_pLegendView;
}
