// XTPChartSeriesView.h
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

//{{AFX_CODEJOCK_PRIVATE
#if !defined(__XTPCHARTSERIESVIEW_H__)
#define __XTPCHARTSERIESVIEW_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CXTPChartSeriesView;
class CXTPChartSeriesPoint;
class CXTPChartSeriesStyle;
class CXTPChartSeries;
class CXTPChartDiagramView;
class CXTPChartDiagram;
class CXTPChartLegendView;

class _XTP_EXT_CLASS CXTPChartSeriesView : public CXTPChartElementView, public CXTPChartLegendItemView
{
public:
	CXTPChartSeriesView(CXTPChartSeries* pSeries, CXTPChartDiagramView* pDiagramView);

public:
	virtual void CreatePointsView(CXTPChartDeviceContext* pDC);

	CXTPChartSeries* GetSeries() const;
	CXTPChartSeriesStyle* GetStyle() const;
	CXTPChartDiagram* GetDiagram() const;

	CXTPChartDiagramView* GetDiagramView() const;

	CXTPChartElementView* GetPointsView() const;

	CXTPChartElementView* GetErrorBarsView() const;

public:
	virtual void CalculateLayout(CXTPChartDeviceContext* pDC);
	virtual void UpdateRange(CXTPChartDeviceContext* pDC);

	virtual void AddToLegend(CXTPChartLegendView* pView);

	virtual CXTPChartDeviceCommand* CreateLegendDeviceCommand(CXTPChartDeviceContext* pDC, CRect rcBounds);
protected:
	virtual CXTPChartSeriesPointView* CreateSeriesPointView(CXTPChartDeviceContext* pDC, CXTPChartSeriesPoint* pPoint, CXTPChartElementView* pParentView);

	virtual CXTPChartString GetLegendName() const;



public:
	CXTPChartSeries      *m_pSeries;
	CXTPChartDiagramView *m_pDiagramView;
	CXTPChartElementView *m_pPointsView;
	CXTPChartElementView *m_pLabelsView;
	CXTPChartElementView *m_pErrorBarsView;
};

AFX_INLINE CXTPChartDiagramView* CXTPChartSeriesView::GetDiagramView() const
{
	return m_pDiagramView;
}

AFX_INLINE CXTPChartSeries* CXTPChartSeriesView::GetSeries() const
{
	return m_pSeries;
}

AFX_INLINE CXTPChartElementView* CXTPChartSeriesView::GetPointsView() const
{
	return m_pPointsView;
}

AFX_INLINE CXTPChartElementView* CXTPChartSeriesView::GetErrorBarsView() const
{
	return m_pErrorBarsView;
}

#endif //#if !defined(__XTPCHARTSERIESVIEW_H__)
