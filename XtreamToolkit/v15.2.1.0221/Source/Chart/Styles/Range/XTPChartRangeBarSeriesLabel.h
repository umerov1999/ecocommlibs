// XTPChartRangeBarSeriesLabel.h
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
#if !defined(__XTPCHARTRANGEBARSERIESLABEL_H__)
#define __XTPCHARTRANGEBARSERIESLABEL_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CXTPChartSeriesView;


enum XTPChartRangeBarLabelPosition
{
	xtpChartRangeBarLabelOutside,
	xtpChartRangeBarLabelInside,
};

enum XTPChartRangeBarLabelType
{
	xtpChartRangeBarOneLabel,
	xtpChartRangeBarTwoLabels,
	xtpChartRangeBarMinValueLabel,
	xtpChartRangeBarMaxValueLabel
};

//===========================================================================
// Summary:
//     This class abstracts the label of a bar series, it is a kind of point
//     series label.
// Remarks:
// See Also:
//===========================================================================
class _XTP_EXT_CLASS CXTPChartRangeBarSeriesLabel : public CXTPChartDiagram2DSeriesLabel
{
	DECLARE_SERIAL(CXTPChartRangeBarSeriesLabel);
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartRangeBarSeriesLabel object.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartRangeBarSeriesLabel();

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPChartRangeBarSeriesLabel object, handles cleanup
	//-------------------------------------------------------------------------
	virtual ~CXTPChartRangeBarSeriesLabel();

public:
	void SetPosition(XTPChartRangeBarLabelPosition nPosition);
	XTPChartRangeBarLabelPosition GetPosition() const;

	void SetType(XTPChartRangeBarLabelType nType);
	XTPChartRangeBarLabelType GetType() const;

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     This function create the view of the bar series label.
	// Parameters:
	//     pDC -        The device context.
	//     pPointView - A pointer to the series point view object.
	// Returns:
	//     A pointer to CXTPChartElementView object, which a polymorphic to CXTPChartRangeBarSeriesLabelView.
	// Remarks:
	//-----------------------------------------------------------------------
	virtual CXTPChartElementView* CreateView(CXTPChartDeviceContext* pDC, CXTPChartSeriesPointView* pPointView, CXTPChartElementView* pParentView);


	void DoPropExchange(CXTPPropExchange* pPX);

protected:
	XTPChartRangeBarLabelPosition m_nPosition;
	XTPChartRangeBarLabelType m_nType;
	int m_nIndent;


#ifdef _XTP_ACTIVEX
public:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPChartRangeBarSeriesLabel);
//}}AFX_CODEJOCK_PRIVATE
#endif
};

//===========================================================================
// Summary:
//     This class abstracts the view of the label of a bar series, it is a kind of point
//     series label view.
// Remarks:
// See Also:
//===========================================================================
class _XTP_EXT_CLASS CXTPChartRangeBarSeriesLabelView : public CXTPChartDiagram2DSeriesLabelView
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartRangeBarSeriesLabel object.
	// Parameters:
	//     pLabel     - Pointer chart series label.
	//     pPointView - Pointer to chart series point view.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartRangeBarSeriesLabelView(CXTPChartSeriesLabel* pLabel, CXTPChartSeriesPointView* pPointView, CXTPChartElementView* pParentView);

	//-----------------------------------------------------------------------
	// Summary:
	//     This function calculates the anchor point of the label stem.
	// Returns:
	//     A CXTPChartPointF object denoting the anchor point.
	// Remarks:
	//-----------------------------------------------------------------------
	virtual CXTPChartPointF GetAnchorPoint(BOOL bMinValue) const;
	virtual double GetAnchorAngle(BOOL bMinValue) const;

protected:
	CXTPChartDeviceCommand* CreateDeviceCommand(CXTPChartDeviceContext* pDC);
	void CalculateLayout(CXTPChartDeviceContext* pDC);
};

AFX_INLINE void CXTPChartRangeBarSeriesLabel::SetPosition(XTPChartRangeBarLabelPosition nPosition) {
	m_nPosition = nPosition;
	OnChartChanged();
}
AFX_INLINE XTPChartRangeBarLabelPosition CXTPChartRangeBarSeriesLabel::GetPosition() const {
	return m_nPosition;
}

AFX_INLINE void CXTPChartRangeBarSeriesLabel::SetType(XTPChartRangeBarLabelType nType) {
	m_nType = nType;
	OnChartChanged();
}
AFX_INLINE XTPChartRangeBarLabelType CXTPChartRangeBarSeriesLabel::GetType() const {
	return m_nType;
}


#endif //#if !defined(__XTPCHARTRANGEBARSERIESLABEL_H__)
