// XTPChartRadarPointSeriesLabel.h
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
#if !defined(__XTPCHARTRADARPOINTSERIESLABEL_H__)
#define __XTPCHARTRADARPOINTSERIESLABEL_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CXTPChartSeriesView;


//===========================================================================
// Summary:
//     This class abstracts the label of a point series, it is a kind of chart
//     series label.
// Remarks:
// See Also:
//===========================================================================
class _XTP_EXT_CLASS CXTPChartRadarPointSeriesLabel : public CXTPChartSeriesLabel
{
	DECLARE_SERIAL(CXTPChartRadarPointSeriesLabel);
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartRadarPointSeriesLabel object.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartRadarPointSeriesLabel();

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPChartRadarPointSeriesLabel object, handles cleanup
	//-------------------------------------------------------------------------
	virtual ~CXTPChartRadarPointSeriesLabel();

public:
	void DoPropExchange(CXTPPropExchange* pPX);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to set the angle of the label stem.
	// Parameters:
	//     nAngle - The angle of the label stem.
	// Remarks:
	//-----------------------------------------------------------------------
	void SetAngle(int nAngle);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the angle of the label stem.
	// Returns:
	//     An intger value denoting angle of the label stem.
	// Remarks:
	//-----------------------------------------------------------------------
	int GetAngle() const;

protected:
	CXTPChartElementView* CreateView(CXTPChartDeviceContext* pDC, CXTPChartSeriesPointView* pPointView, CXTPChartElementView* pParentView);

protected:
	int m_nAngle;       //The stem angle.

#ifdef _XTP_ACTIVEX
public:
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPChartRadarPointSeriesLabel);
	//}}AFX_CODEJOCK_PRIVATE
#endif
};

//===========================================================================
// Summary:
//     This class abstracts the view of  the label of a point series, it is a kind of chart
//     series label view.
// Remarks:
// See Also:
//===========================================================================
class _XTP_EXT_CLASS CXTPChartRadarPointSeriesLabelView : public CXTPChartSeriesLabelView
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartRadarPointSeriesLabelView object.
	// Parameters:
	//     pLabel - The pointer to chart series label object.
	//     pPointView - A pointer to chart series view.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartRadarPointSeriesLabelView(CXTPChartSeriesLabel* pLabel, CXTPChartSeriesPointView* pPointView, CXTPChartElementView* pParentView);

	//-------------------------------------------------------------------------
	// Summary:
	//     This function create a CXTPChartDeviceCommand object, this object
	//     represents the rendering of the chart series label.
	// Parameters:
	//     pDC     - Pointer to a CXTPChartDeviceContext object.
	// Returns:
	//     Returns a pointer to CXTPChartDeviceCommand object, this polymorphic object
	//     handles the rendering of an element in the chart.Here it handles the drawing
	//     of the chart series label.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	CXTPChartDeviceCommand* CreateDeviceCommand(CXTPChartDeviceContext* pDC);
	CXTPChartDeviceCommand* CreateDeviceCommand(CXTPChartDeviceContext* pDC, const CXTPChartPointF& ptAnchorPoint, double dAnchorAngle, const CXTPChartString& text);

	//-----------------------------------------------------------------------
	// Summary:
	//     This function calculates the anchor point of the label stem.
	// Returns:
	//     A CXTPChartPointF object denoting the anchor point.
	// Remarks:
	//-----------------------------------------------------------------------
	virtual CXTPChartPointF GetAnchorPoint() const;
	virtual double GetAnchorAngle() const;

	virtual void CalculateLayout(CXTPChartDeviceContext* pDC);
	void CalculateLayout(CXTPChartDeviceContext* pDC, const CXTPChartPointF& ptAnchorPoint, double dAnchorAngle, const CXTPChartString& text);


protected:
	CRect GetPaneRect() const;
};

AFX_INLINE void CXTPChartRadarPointSeriesLabel::SetAngle(int nAngle) {
	m_nAngle = nAngle;
	OnChartChanged();
}
AFX_INLINE int CXTPChartRadarPointSeriesLabel::GetAngle() const {
	return m_nAngle;
}


#endif //#if !defined(__XTPCHARTRADARPOINTSERIESLABEL_H__)
