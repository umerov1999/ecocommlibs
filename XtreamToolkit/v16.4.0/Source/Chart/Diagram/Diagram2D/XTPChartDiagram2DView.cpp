// XTPChartDiagram2D.cpp
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


#include <Chart/Types/XTPChartTypes.h>
#include <Chart/XTPChartDefines.h>
#include <Chart/XTPChartElement.h>
#include <Chart/XTPChartLegendItem.h>
#include <Chart/XTPChartElementView.h>
#include <Chart/XTPChartPanel.h>
#include <Chart/XTPChartSeriesPointView.h>
#include <Chart/XTPChartSeriesView.h>
#include <Chart/XTPChartSeries.h>
#include <Chart/XTPChartSeriesPoint.h>
#include <Chart/XTPChartDiagram.h>
#include <Chart/XTPChartDiagramView.h>
#include <Chart/XTPChartTitle.h>
#include <Chart/XTPChartSeriesStyle.h>
#include <Chart/XTPChartSeriesLabel.h>

#include <Chart/Diagram/Axis/XTPChartAxis.h>
#include <Chart/Diagram/Axis/XTPChartAxisView.h>

#include <Chart/Diagram/Diagram2D/XTPChartDiagram2D.h>
#include <Chart/Diagram/Diagram2D/XTPChartDiagram2DPane.h>
#include <Chart/Diagram/Diagram2D/XTPChartDiagram2DView.h>
#include <Chart/Diagram/Diagram2D/XTPChartDiagram2DSeriesView.h>
#include <Chart/Diagram/Diagram2D/XTPChartDiagram2DSeriesStyle.h>
#include <Chart/Diagram/Diagram2D/XTPChartDiagram2DSeriesLabel.h>


//////////////////////////////////////////////////////////////////////////
// CXTPChartDiagram2DView

CXTPChartDiagram2DView::CXTPChartDiagram2DView(CXTPChartDiagram* pDiagram, CXTPChartElementView* pParent)
	: CXTPChartDiagramView(pDiagram, pParent)
{
	m_pAxisViews = NULL;
	m_pPaneView = NULL;

	m_rcLabelPadding.SetRectEmpty();
}

void CXTPChartDiagram2DView::AddAxisView(CXTPChartDeviceContext* pDC, CXTPChartElementView* pParentView, CXTPChartAxis* pAxis)
{
	CXTPChartDiagram2DAxisView* pAxisView = (CXTPChartDiagram2DAxisView*)pAxis->CreateView(pDC, pParentView);

	pAxisView->CreateView(pDC);
}

CXTPChartAxisView* CXTPChartDiagram2DView::GetAxisView(CXTPChartAxis* pAxis) const
{
	for (int i = 0; i < m_pAxisViews->GetCount(); i++)
	{
		CXTPChartAxisView* pAxisView = (CXTPChartAxisView*)m_pAxisViews->GetAt(i);

		if (pAxisView->GetAxis() == pAxis)
			return pAxisView;
	}

	return NULL;
}

void CXTPChartDiagram2DView::UpdateRange(CXTPChartDeviceContext* pDC)
{
	int i;

	for (i = 0; i < GetSeriesView()->GetCount(); i++)
	{
		CXTPChartDiagram2DSeriesView* pSeriesView = (CXTPChartDiagram2DSeriesView*)GetSeriesView()->GetAt(i);

		pSeriesView->BeforeUpdateRange(pDC);
	}

	for (i = 0; i < m_pAxisViews->GetCount(); i++)
	{
		CXTPChartDiagram2DAxisView* pAxisView = (CXTPChartDiagram2DAxisView*)m_pAxisViews->GetAt(i);

		for (int j = 0; j < GetSeriesView()->GetCount(); j++)
		{
			CXTPChartSeriesView* pSeriesView = (CXTPChartSeriesView*)GetSeriesView()->GetAt(j);
			if (!pSeriesView->GetSeries()->IsVisible())
				continue;

			CXTPChartDiagram2DSeriesStyle* pStyle = DYNAMIC_DOWNCAST(CXTPChartDiagram2DSeriesStyle, pSeriesView->GetSeries()->GetStyle());

			ASSERT(pStyle);
			if (!pStyle)
				continue;


			BOOL bAxisX = !pAxisView->GetAxis()->IsValuesAxis();

			if (pAxisView->GetAxis()->IsSecondary() != (bAxisX ? pStyle->IsSecondaryAxisX() : pStyle->IsSecondaryAxisY()))
				continue;

			pAxisView->m_arrSeries.Add(pSeriesView);

			if (bAxisX)
				((CXTPChartDiagram2DSeriesView*)pSeriesView)->m_pAxisViewX = pAxisView;
			else
				((CXTPChartDiagram2DSeriesView*)pSeriesView)->m_pAxisViewY = pAxisView;
		}
	}

	for (i = 0; i < m_pAxisViews->GetCount(); i++)
	{
		CXTPChartDiagram2DAxisView* pAxisView = (CXTPChartDiagram2DAxisView*)m_pAxisViews->GetAt(i);

		pAxisView->GetAxis()->UpdateRange(pDC, pAxisView);

		pAxisView->AddLegendItems();
	}

	for (i = 0; i < GetSeriesView()->GetCount(); i++)
	{
		CXTPChartDiagram2DSeriesView* pSeriesView = (CXTPChartDiagram2DSeriesView*)GetSeriesView()->GetAt(i);

		pSeriesView->AfterUpdateRange(pDC);
	}
}

void CXTPChartDiagram2DView::CheckLabelBounds(const CXTPChartRectF& rcBounds)
{
	if (rcBounds.GetTop() < m_rcBounds.top)
	{
		m_rcLabelPadding.top = max(m_rcLabelPadding.top, m_rcBounds.top - (LONG)rcBounds.GetTop());
	}

	if (rcBounds.GetRight() > m_rcBounds.right)
	{
		m_rcLabelPadding.right = max(m_rcLabelPadding.right, (LONG)rcBounds.GetRight() - m_rcBounds.right);
	}

	if (rcBounds.GetLeft() < m_rcBounds.left)
	{
		m_rcLabelPadding.left = max(m_rcLabelPadding.left, m_rcBounds.left - (LONG)rcBounds.GetLeft());
	}

	if (rcBounds.GetBottom() > m_rcBounds.bottom)
	{
		m_rcLabelPadding.bottom = max(m_rcLabelPadding.bottom, (LONG)rcBounds.GetBottom() - m_rcBounds.bottom);
	}
}

void CXTPChartDiagram2DView::CalculateView(CXTPChartDeviceContext* pDC, CRect rcBounds)
{
	m_pDiagram->GetTitles()->CalculateView(pDC, rcBounds, m_pTitlesView);

	m_rcLabelPadding.SetRectEmpty();

	CRect rcDiagramBounds = rcBounds;

	for (int nUpdate = 0; nUpdate < 2; nUpdate++)
	{
		const int AXIS_GAP = 10;

		rcBounds = rcDiagramBounds;
		rcBounds.DeflateRect(m_rcLabelPadding);

		m_rcBounds = rcBounds;

		CRect rcInnerBounds(rcBounds);

		m_rcLabelPadding.SetRectEmpty();

		CRect rcPane(rcBounds);

		int i;

		((CXTPChartDiagram2D*)m_pDiagram)->UpdateLayout(pDC, this, rcBounds);


		CXTPChartDiagram2D* pDiagram = (CXTPChartDiagram2D*)GetDiagram();


		for (i = 0; i < m_pAxisViews->GetCount(); i++)
		{
			CXTPChartDiagram2DAxisView* pAxisView = (CXTPChartDiagram2DAxisView*)m_pAxisViews->GetAt(i);

			pAxisView->CalcSize(pDC, rcPane);

			if (!pAxisView->GetAxis()->IsVisible())
				continue;

			if (pAxisView->GetAxis()->IsVertical())
			{
				if (pAxisView->GetAxis()->GetAlignment() == xtpChartAlignNear)
				{
					if (rcPane.left != rcBounds.left) rcPane.left += AXIS_GAP;
					rcPane.left += pAxisView->GetSize();
				}
				else
				{
					if (rcPane.right != rcBounds.right) rcPane.right -= AXIS_GAP;
					rcPane.right -= pAxisView->GetSize();
				}
			}
			else
			{
				if (pAxisView->GetAxis()->GetAlignment() == xtpChartAlignNear)
				{
					if (rcPane.bottom != rcBounds.bottom) rcPane.bottom -= AXIS_GAP;
					rcPane.bottom -= pAxisView->GetSize();
				}
				else
				{
					if (rcPane.top != rcBounds.top) rcPane.top += AXIS_GAP;
					rcPane.top += pAxisView->GetSize();
				}
			}
		}



		for (i = m_pAxisViews->GetCount() - 1; i >= 0; i--)
		{
			CXTPChartDiagram2DAxisView* pAxisView = (CXTPChartDiagram2DAxisView*)m_pAxisViews->GetAt(i);
			int nSize = pAxisView->GetSize();


			if (pAxisView->GetAxis()->IsVertical())
			{
				if (pAxisView->GetAxis()->GetAlignment() == xtpChartAlignNear)
				{
					if (rcBounds.left != m_rcBounds.left) rcBounds.left += AXIS_GAP;
					pAxisView->SetBounds(pDC, CRect(rcBounds.left, rcPane.top, rcBounds.left + nSize, rcPane.bottom));
					rcBounds.left += nSize;
				}
				else
				{
					if (rcBounds.right != m_rcBounds.right) rcBounds.right -= AXIS_GAP;
					pAxisView->SetBounds(pDC, CRect(rcBounds.right - nSize, rcPane.top, rcBounds.right, rcPane.bottom));
					rcBounds.right -= nSize;
				}
			}
			else
			{
				if (pAxisView->GetAxis()->GetAlignment() == xtpChartAlignNear)
				{
					if (rcBounds.bottom != m_rcBounds.bottom) rcBounds.bottom -= AXIS_GAP;
					pAxisView->SetBounds(pDC, CRect(rcPane.left, rcBounds.bottom - nSize, rcPane.right, rcBounds.bottom));
					rcBounds.bottom -= nSize;
				}
				else
				{
					if (rcBounds.top != m_rcBounds.top) rcBounds.top += AXIS_GAP;
					pAxisView->SetBounds(pDC, CRect(rcPane.left, rcBounds.top, rcPane.right, rcBounds.top + nSize));
					rcBounds.top += nSize;
				}
			}
		}

		m_pPaneView->CalculateView(rcBounds);

		if (m_pPaneView->m_rcBounds.Width() < 1 || m_pPaneView->m_rcBounds.Height() < 1)
			break;

		pDiagram->CalculateSeriesLayout(pDC, this);


		for (i = 0; i < m_pLabelsView->GetCount(); i++)
		{
			CXTPChartDiagram2DSeriesLabelView* pLabelView = (CXTPChartDiagram2DSeriesLabelView*)m_pLabelsView->GetAt(i);

			pLabelView->CalculateLayout(pDC);
		}

		if( m_rcLabelPadding.IsRectNull())
			break;
	}
}

void CXTPChartDiagram2DView::CreateView(CXTPChartDeviceContext* pDC)
{
	CXTPChartDiagramView::CreateView(pDC);

	CXTPChartDiagram2D* pDiagram = (CXTPChartDiagram2D*)GetDiagram();

	CXTPChartElementView* pPaneView = new CXTPChartElementView(this);
	m_pAxisViews = new CXTPChartElementView(this);

	for (int nAxisX=0; nAxisX<pDiagram->GetAxisCountX(); nAxisX++)
	{
		AddAxisView(pDC, m_pAxisViews, pDiagram->GetAxisX(nAxisX));
	}

	for (int nAxisY=0; nAxisY<pDiagram->GetAxisCountY(); nAxisY++)
	{
		AddAxisView(pDC, m_pAxisViews, pDiagram->GetAxisY(nAxisY));
	}

	m_pPaneView = (CXTPChartDiagram2DPaneView*)pDiagram->GetPane()->CreateView(pDC, pPaneView);

	m_pSeriesView = m_pPaneView->CreateSeriesView();


	m_pLabelsView = new CXTPChartElementView(this);
}

CXTPChartDeviceCommand* CXTPChartDiagram2DView::CreateDeviceCommand(CXTPChartDeviceContext* pDC)
{
	if (m_pPaneView->m_rcBounds.Width() < 1 || m_pPaneView->m_rcBounds.Height() < 1)
		return NULL;
	return CXTPChartDiagramView::CreateDeviceCommand(pDC);
}

void CXTPChartDiagram2DView::OnMouseWheel(UINT /*nFlags*/, short zDelta, CPoint pt)
{
	CXTPChartDiagram2D* pDiagram = (CXTPChartDiagram2D*)GetDiagram();

	if (!pDiagram->IsAllowZoom())
		return;

	if (!m_pPaneView->GetBounds().PtInRect(pt))
		return;

	for (int i = 0; i < m_pAxisViews->GetCount(); i++)
	{
		CXTPChartDiagram2DAxisView* pAxisView = (CXTPChartDiagram2DAxisView*)m_pAxisViews->GetAt(i);

		pAxisView->PerformMouseWheel(zDelta, pt);
	}
}

BOOL CXTPChartDiagram2DView::OnSetCursor(CPoint point)
{
	if (!m_pPaneView->GetBounds().PtInRect(point))
	{
		return FALSE;
	}

	for (int i = 0; i < m_pAxisViews->GetCount(); i++)
	{
		CXTPChartDiagram2DAxisView* pAxisView = (CXTPChartDiagram2DAxisView*)m_pAxisViews->GetAt(i);

		if (pAxisView->IsScollBarVisible())
		{
			::SetCursor(m_hcurNormalHand);
			return TRUE;
		}
	}
	return FALSE;
}


void CXTPChartDiagram2DView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CXTPChartDiagram2D *pDiagram = (CXTPChartDiagram2D*)GetDiagram();

	if (!pDiagram->IsAllowScroll())
		return;

	if (m_pPaneView->GetBounds().PtInRect(point))
	{
		m_pPaneView->OnLButtonDown(nFlags, point);
		return;
	}

	for (int i = 0; i < m_pAxisViews->GetCount(); i++)
	{
		CXTPChartDiagram2DAxisView* pAxisView = (CXTPChartDiagram2DAxisView*)m_pAxisViews->GetAt(i);

		if (pAxisView->GetBounds().PtInRect(point))
		{
			pAxisView->OnLButtonDown(nFlags, point);
			return;
		}
	}
}
