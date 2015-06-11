// XTPChartDiagram2DSeriesView.h
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
#if !defined(__XTPCHARTDIAGRAM2DSERIESVIEW_H__)
#define __XTPCHARTDIAGRAM2DSERIESVIEW_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CXTPChartAxis;
class CXTPChartDiagram2DPane;
class CXTPChartDiagram2DPaneView;
class CXTPChartDiagram2DAppearance;
class CXTPChartAxisView;
class CXTPChartDiagram2DAxisView;


class _XTP_EXT_CLASS CXTPChartDiagram2DSeriesView :public CXTPChartSeriesView
{
protected:
	CXTPChartDiagram2DSeriesView(CXTPChartSeries* pSeries, CXTPChartDiagramView* pDiagramView, BOOL bSortPoints = FALSE);

public:
	CXTPChartDiagram2DAxisView* GetAxisViewX() const;
	CXTPChartDiagram2DAxisView* GetAxisViewY() const;

	CXTPChartPointF GetScreenPoint(double x, double y) const;

	virtual void AfterUpdateRange(CXTPChartDeviceContext* pDC);
	virtual void BeforeUpdateRange(CXTPChartDeviceContext* pDC);

protected:
	static int _cdecl ComparePoints(const CXTPChartSeriesPointView** ppPoint1, const CXTPChartSeriesPointView** ppPoint2);

protected:
	CXTPChartDiagram2DAxisView* m_pAxisViewX;
	CXTPChartDiagram2DAxisView* m_pAxisViewY;

	BOOL m_bSortPoints;

	friend class CXTPChartDiagram2DView;
	friend class CXTPChartDiagram2DPaneView;
};

AFX_INLINE CXTPChartDiagram2DAxisView* CXTPChartDiagram2DSeriesView::GetAxisViewX() const {
	return m_pAxisViewX;
}

AFX_INLINE CXTPChartDiagram2DAxisView* CXTPChartDiagram2DSeriesView::GetAxisViewY() const {
	return m_pAxisViewY;
}


#endif //#if !defined(__XTPCHARTDIAGRAM2DSERIESVIEW_H__)
