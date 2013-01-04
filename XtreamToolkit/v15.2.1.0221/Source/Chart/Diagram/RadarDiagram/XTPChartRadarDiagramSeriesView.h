// XTPChartRadarDiagramSeriesView.h
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

//{{AFX_CODEJOCK_PRIVATE
#if !defined(__XTPCHARTRADARDIAGRAMSERIESVIEW_H__)
#define __XTPCHARTRADARDIAGRAMSERIESVIEW_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CXTPChartAxis;
class CXTPChartDiagramView;
class CXTPChartAxisView;


class _XTP_EXT_CLASS CXTPChartRadarDiagramSeriesView :public CXTPChartSeriesView
{
protected:
	CXTPChartRadarDiagramSeriesView(CXTPChartSeries* pSeries, CXTPChartDiagramView* pDiagramView);

public:
	CXTPChartRadarAxisView* GetAxisViewX() const;
	CXTPChartRadarAxisView* GetAxisViewY() const;

	CXTPChartPointF GetScreenPoint(double x, double y) const;

	virtual void AfterUpdateRange(CXTPChartDeviceContext* pDC);
	virtual void BeforeUpdateRange(CXTPChartDeviceContext* pDC);

protected:
	CXTPChartRadarAxisView* m_pAxisViewX;
	CXTPChartRadarAxisView* m_pAxisViewY;

	friend class CXTPChartRadarDiagramView;
};

AFX_INLINE CXTPChartRadarAxisView* CXTPChartRadarDiagramSeriesView::GetAxisViewX() const {
	return m_pAxisViewX;
}

AFX_INLINE CXTPChartRadarAxisView* CXTPChartRadarDiagramSeriesView::GetAxisViewY() const {
	return m_pAxisViewY;
}


#endif //#if !defined(__XTPCHARTRADARDIAGRAMSERIESVIEW_H__)
