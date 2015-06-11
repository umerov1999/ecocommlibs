// XTPChartBubbleSeriesStyle.h
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
#if !defined(__XTPCHARTBUBBLESERIESSTYLE_H__)
#define __XTPCHARTBUBBLESERIESSTYLE_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CXTPChartSeriesView;


//===========================================================================
// Summary:
//     This class represents a bubble series style,which is a kind of
//     CXTPChartPointSeriesStyle.
// Remarks:
//     A bubble chart is a type of chart where each plotted entity is defined
//     in terms of three distinct numeric parameters.Bubble charts can facilitate
//     the understanding of the social, economical, medical, and other scientific
//     relationships.
//===========================================================================
class _XTP_EXT_CLASS CXTPChartBubbleSeriesStyle : public CXTPChartPointSeriesStyle
{
	DECLARE_SERIAL(CXTPChartBubbleSeriesStyle)

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartBubbleSeriesStyle object.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartBubbleSeriesStyle();

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPChartBubbleSeriesStyle object, handles cleanup
	//-------------------------------------------------------------------------
	virtual ~CXTPChartBubbleSeriesStyle();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the transparency of the bubbles.
	// Returns:
	//     An integer value denoting the transparency with range from 0-255.
	// Remarks:
	//-----------------------------------------------------------------------
	int GetTransparency() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to set the transparency of the bubbles.
	// Parameters:
	//     nTransparency - An integer value denoting the transparency with
	//     range from 0-255.
	// Remarks:
	//-----------------------------------------------------------------------
	void SetTransparency(int nTransparency);

	double GetMinSize() const;
	void SetMinSize(double dMinSize);

	double GetMaxSize() const;
	void SetMaxSize(double dMinSize);

public:
	virtual int GetSeriesPointValueIndex() const;

public:
	void DoPropExchange(CXTPPropExchange* pPX);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to create view of the bubble series.
	// Parameters:
	//     pSeries      - A pointer to chart series object.
	//     pDiagramView - A pointer to the diagram view object.
	// Returns:
	//     A pointer to CXTPChartBubbleSeriesView object which is a kind of
	//     CXTPChartSeriesView object.
	// Remarks:
	//-----------------------------------------------------------------------
	virtual CXTPChartSeriesView* CreateView(CXTPChartSeries* pSeries, CXTPChartDiagramView* pDiagramView);

protected:
	void CorrectAxisSideMargins(CXTPChartAxis* pAxis, double /*nMinValue*/, double /*nMaxValue*/, double& nCorrection);

#ifdef _XTP_ACTIVEX
public:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPChartBubbleSeriesStyle);
	DECLARE_OLECREATE_EX(CXTPChartBubbleSeriesStyle)

	double OleGetMinSize();
	void OleSetMinSize(double dMinSize);

	double OleGetMaxSize();
	void OleSetMaxSize(double dMinSize);

//}}AFX_CODEJOCK_PRIVATE
#endif

	int m_nTransparency;        //The transparency value.

	double m_dMinSize;
	double m_dMaxSize;
};

//===========================================================================
// Summary:
//     This class represents the view of a bubble series ,which is a kind of
//     CXTPChartSeriesView.
// Remarks:
//     A bubble chart is a type of chart where each plotted entity is defined
//     in terms of three distinct numeric parameters.Bubble charts can facilitate
//     the understanding of the social, economical, medical, and other scientific
//     relationships.
//===========================================================================
class _XTP_EXT_CLASS CXTPChartBubbleSeriesView : public CXTPChartPointSeriesView
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartBubbleSeriesView object.
	// Parameters:
	//     pSeries      - A pointer to the chart series object.
	//     pDiagramView - A pointer to the diagram view object.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartBubbleSeriesView(CXTPChartSeries* pSeries, CXTPChartDiagramView* pDiagramView);

protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to create view of the bubble series point.
	// Parameters:
	//     pDC      - A pointer to chart device context.
	//     pPoint   - A pointer to the chart series point object.
	// Returns:
	//     A pointer to CXTPChartBubbleSeriesPointView object which is a kind of
	//     CXTPChartSeriesPointView object.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartSeriesPointView* CreateSeriesPointView(CXTPChartDeviceContext* pDC, CXTPChartSeriesPoint* pPoint, CXTPChartElementView* pParentView);

	//-------------------------------------------------------------------------
	// Summary:
	//     This function create a CXTPChartDeviceCommand object, this object
	//     represents the rendering of a bubble series.
	// Parameters:
	//     pDC     - Pointer to a CXTPChartDeviceContext object.
	// Returns:
	//     Returns CXTPChartDeviceCommand object, this object handles
	//     the rendering of an element in the chart.Here it handles
	//     the drawing of the bubble series.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	CXTPChartDeviceCommand* CreateDeviceCommand(CXTPChartDeviceContext* pDC);

protected:
	void CreatePointsView(CXTPChartDeviceContext* pDC);

	double m_dMinValue;
	double m_dMaxValue;

	friend class CXTPChartBubbleSeriesPointView;
};

//===========================================================================
// Summary:
//     This class represents the view of a bubble series point,which is a kind of
//     CXTPChartPointSeriesPointView.
// Remarks:
//     A bubble chart is a type of chart where each plotted entity is defined
//     in terms of three distinct numeric parameters.Bubble charts can facilitate
//     the understanding of the social, economical, medical, and other scientific
//     relationships.
//===========================================================================
class _XTP_EXT_CLASS CXTPChartBubbleSeriesPointView : public CXTPChartPointSeriesPointView
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartBubbleSeriesPointView object.
	// Parameters:
	//     pPoint       - A pointer to the chart series point object.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartBubbleSeriesPointView(CXTPChartSeriesPoint* pPoint, CXTPChartElementView* pParentView);

public:
	//-------------------------------------------------------------------------
	// Summary:
	//     This function create a CXTPChartDeviceCommand object, this object
	//     represents the rendering of a bubble series point.
	// Parameters:
	//     pDC     - Pointer to a CXTPChartDeviceContext object.
	// Returns:
	//     Returns CXTPChartPolygonAntialiasingDeviceCommand object, this
	//     object handles the rendering of an element in the chart.
	//     Here it handles the drawing of the bubble series point.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	CXTPChartDeviceCommand* CreateDeviceCommand(CXTPChartDeviceContext* pDC);

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the actual color.
	// Returns:
	//     A CXTPChartColor object representing an ARGB value.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartColor GetActualColor() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the second actual color.
	// Returns:
	//     A CXTPChartColor object representing an ARGB value.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartColor GetActualColor2() const;
};

AFX_INLINE int CXTPChartBubbleSeriesStyle::GetTransparency() const {
	return m_nTransparency;
}
AFX_INLINE void CXTPChartBubbleSeriesStyle::SetTransparency(int nTransparency) {
	m_nTransparency = nTransparency;
	OnChartChanged();
}

AFX_INLINE double CXTPChartBubbleSeriesStyle::GetMinSize() const {
	return m_dMinSize;
}
AFX_INLINE void CXTPChartBubbleSeriesStyle::SetMinSize(double dMinSize) {
	m_dMinSize = dMinSize;
	OnChartChanged();
}
AFX_INLINE double CXTPChartBubbleSeriesStyle::GetMaxSize() const {
	return m_dMaxSize;
}
AFX_INLINE void CXTPChartBubbleSeriesStyle::SetMaxSize(double dMaxSize) {
	m_dMaxSize = dMaxSize;
	OnChartChanged();
}
AFX_INLINE int CXTPChartBubbleSeriesStyle::GetSeriesPointValueIndex() const {
	return 1;
}

#endif //#if !defined(__XTPCHARTBUBBLESERIESSTYLE_H__)
