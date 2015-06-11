// XTPChartBarSeriesStyle.h
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
#if !defined(__XTPCHARTBARSERIESSTYLE_H__)
#define __XTPCHARTBARSERIESSTYLE_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CXTPChartSeriesView;


//===========================================================================
// Summary:
//     This class represents a bar series style for the chart,which is a kind of
//     CXTPChartSeriesStyle.
// Remarks:
//    A bar chart or bar graph is a chart with rectangular bars with lengths
//    proportional to the values that they represent. Bar charts are used for
//    comparing two or more values that were taken over time or on different
//    conditions, usually on small data sets.
//===========================================================================
class _XTP_EXT_CLASS CXTPChartBarSeriesStyle : public CXTPChartDiagram2DSeriesStyle
{
	DECLARE_SERIAL(CXTPChartBarSeriesStyle)
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartBarSeriesStyle object.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartBarSeriesStyle();

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPChartBarSeriesStyle object, handles cleanup.
	//-------------------------------------------------------------------------
	virtual ~CXTPChartBarSeriesStyle();

public:

	void DoPropExchange(CXTPPropExchange* pPX);
public:

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the width of the bar.
	// Returns:
	//     A double value specifying the width of the bar.
	//-------------------------------------------------------------------------
	double GetBarWidth() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to set the width of the bar.
	// Parameters:
	//     dWidth - A double value specifying the width of the bar.
	//-------------------------------------------------------------------------
	void SetBarWidth(double dWidth);

	CXTPChartFillStyle* GetFillStyle() const;

	CXTPChartBorder* GetBorder() const;

	BOOL IsSideBySide() const;
	void SetSideBySide(BOOL bSideBySide);

	int GetBarDistanceFixed() const;
	void SetBarDistanceFixed(int nBarDistanceFixed);

protected:

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to create the view of the bar series.
	// Parameters:
	//     pSeries      - A pointer to the chart series object, to which this object
	//                    is associated with.
	//     pDiagramView - A pointer to the chart diagram view object, to which this object
	//                    is associated with.
	// Returns:
	//     A pointer to CXTPChartSeriesView, refers a newly created CXTPChartBarSeriesView
	//     object.
	//-------------------------------------------------------------------------
	virtual CXTPChartSeriesView* CreateView(CXTPChartSeries* pSeries, CXTPChartDiagramView* pDiagramView);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to correct the axis side margins.
	// Parameters:
	//     pAxis        - A pointer to the chart axis object, to which this object
	//                    is associated with.
	//     nMinValue    - A double specifying the minimum value.
	//     nMaxValue    - A double specifying the maximum value.
	//     nCorrection  - A reference to a double object, which specifies the correction
	//                    applied.
	// Returns:
	//     A pointer to CXTPChartSeriesView, refers a newly created CXTPChartBarSeriesView
	//     object.
	//-------------------------------------------------------------------------
	virtual void CorrectAxisSideMargins(CXTPChartAxis* pAxis, double nMinValue, double nMaxValue, double& nCorrection);


protected:

	double m_dBarWidth;     //The width of the bar.

	CXTPChartFillStyle* m_pFillStyle;
	CXTPChartBorder* m_pBorder;
	BOOL m_bSideBySide;
	int m_nBarDistanceFixed;

#ifdef _XTP_ACTIVEX
public:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPChartBarSeriesStyle);
	DECLARE_OLECREATE_EX(CXTPChartBarSeriesStyle)

	LPDISPATCH OleGetFillStyle();
	LPDISPATCH OleGetBorder();
//}}AFX_CODEJOCK_PRIVATE
#endif
};

AFX_INLINE double CXTPChartBarSeriesStyle::GetBarWidth() const {
	return m_dBarWidth;
}

AFX_INLINE void CXTPChartBarSeriesStyle::SetBarWidth(double dWidth) {
	m_dBarWidth = dWidth;
	OnChartChanged();
}
AFX_INLINE CXTPChartFillStyle* CXTPChartBarSeriesStyle::GetFillStyle() const {
	return m_pFillStyle;
}
AFX_INLINE CXTPChartBorder* CXTPChartBarSeriesStyle::GetBorder() const {
	return m_pBorder;
}
AFX_INLINE void CXTPChartBarSeriesStyle::SetSideBySide(BOOL bSideBySide) {
	m_bSideBySide = bSideBySide;
	OnChartChanged();
}
AFX_INLINE BOOL CXTPChartBarSeriesStyle::IsSideBySide() const {
	return m_bSideBySide;
}
AFX_INLINE int CXTPChartBarSeriesStyle::GetBarDistanceFixed() const {
	return m_nBarDistanceFixed;
}
AFX_INLINE void CXTPChartBarSeriesStyle::SetBarDistanceFixed(int nBarDistanceFixed) {
	m_nBarDistanceFixed = nBarDistanceFixed;
	OnChartChanged();
}



#endif //#if !defined(__XTPCHARTBARSERIESSTYLE_H__)
