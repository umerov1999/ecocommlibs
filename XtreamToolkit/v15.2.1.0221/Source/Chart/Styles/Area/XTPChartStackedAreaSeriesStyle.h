// XTPChartStackedAreaSeriesStyle.h
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
#if !defined(__XTPCHARTSTACKEDAREASERIESSTYLE_H__)
#define __XTPCHARTSTACKEDAREASERIESSTYLE_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CXTPChartSeriesView;
class CXTPChartBorder;
class CXTPChartFillStyle;

//===========================================================================
// Summary:
//     CXTPChartStackedAreaSeriesStyle specialized class from CXTPChartPointSeriesStyle
//     class represents the area series style.
// Remarks:
//     Area series displays graphically the quantitative data in a chart.
//     it displays a series as a set of points connected by a line or spline
//     with all the area filled in below the line.
//
//===========================================================================
class _XTP_EXT_CLASS CXTPChartStackedAreaSeriesStyle : public CXTPChartAreaSeriesStyle
{
	DECLARE_SERIAL(CXTPChartStackedAreaSeriesStyle)

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartStackedAreaSeriesStyle object.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartStackedAreaSeriesStyle();

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPChartStackedAreaSeriesStyle object, handles cleanup
	//-------------------------------------------------------------------------
	virtual ~CXTPChartStackedAreaSeriesStyle();

public:
	void SetStackHeight(double dLevel = 100);
	double GetStackHeight() const;


public:


protected:

	//-------------------------------------------------------------------------
	// Summary:
	//     Use this function to create a chart series view object.
	// Parameters:
	//     pSeries      - Pointer to a chart series object.
	//     pDiagramView - Pointer to a chart diagram view object.
	// Returns:
	//     A pointer to CXTPChartSeriesView object newly created.
	// Remarks:
	//     CXTPChartSeriesView abstracts the view of a series.
	// See Also:
	//-------------------------------------------------------------------------
	virtual CXTPChartSeriesView* CreateView(CXTPChartSeries* pSeries, CXTPChartDiagramView* pDiagramView);

#ifdef _XTP_ACTIVEX
public:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPChartStackedAreaSeriesStyle);
	DECLARE_OLECREATE_EX(CXTPChartStackedAreaSeriesStyle)

	double OleGetStackHeight();
	void OleSetStackHeight(double dStackHeight);
//}}AFX_CODEJOCK_PRIVATE
#endif

protected:
	double m_dStackHeight;

};

//===========================================================================
// Summary:
//     CXTPChartStackedAreaSeriesView is a kind of CXTPChartSeriesView, this class
//     represents the view of the area series of a chart.
// Remarks:
//     Area series displays graphically the quantitative data in a chart.
//     it displays a series as a set of points connected by a line or spline
//     with all the area filled in below the line.
//
//===========================================================================
class _XTP_EXT_CLASS CXTPChartStackedAreaSeriesView : public CXTPChartAreaSeriesView
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartStackedAreaSeriesView object.
	// Parameters:
	//     pSeries      - Pointer to a chart series object.
	//     pDiagramView - Pointer to a chart diagram view object.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartStackedAreaSeriesView(CXTPChartSeries* pSeries, CXTPChartDiagramView* pDiagramView);

protected:

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to create a chart area series point view object.
	// Parameters:
	//     pDC      - Pointer to a chart device context object.
	//     pPoint   - Pointer to a chart series object.
	// Returns:
	//     A pointer to CXTPChartSeriesPointView which refers a  newly created.
	//     CXTPChartPointSeriesPointView object.
	// Remarks:
	//     CXTPChartSeriesPointView object abstracts the view of a point in a series.
	// See Also:
	//-------------------------------------------------------------------------
	CXTPChartSeriesPointView* CreateSeriesPointView(CXTPChartDeviceContext* pDC, CXTPChartSeriesPoint* pPoint, CXTPChartElementView* pParentView);

	//-------------------------------------------------------------------------
	// Summary:
	//     This function create a CXTPChartDeviceCommand object, this object
	//     represents the rendering of an area series in the chart.
	// Parameters:
	//     pDC     - Pointer to a CXTPChartDeviceContext object.
	// Returns:
	//     Returns CXTPChartDeviceCommand object, this polymorphic object handles
	//     the rendering of an element in the chart.Here it handles the drawing
	//     of the area series of the chart.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	CXTPChartDeviceCommand* CreateDeviceCommand(CXTPChartDeviceContext* pDC);

	void UpdateRange(CXTPChartDeviceContext* pDC);

};

class _XTP_EXT_CLASS CXTPChartStackedAreaSeriesPointView : public CXTPChartPointSeriesPointView
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartPointSeriesPointView object.
	// Parameters:
	//     pPoint       - A pointer to the chart series point.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartStackedAreaSeriesPointView(CXTPChartSeriesPoint* pPoint, CXTPChartElementView* pParentView);

public:
	double m_dValueFrom;
	double m_dValueTo;

	friend class CXTPChartStackedAreaSeriesView;
};

AFX_INLINE void CXTPChartStackedAreaSeriesStyle::SetStackHeight(double dStackHeight) {
	m_dStackHeight = dStackHeight;
	OnChartChanged();
}
AFX_INLINE double CXTPChartStackedAreaSeriesStyle::GetStackHeight() const {
	return m_dStackHeight;
}
#endif //#if !defined(__XTPCHARTSTACKEDAREASERIESSTYLE_H__)
