// XTPChartBarSeriesLabel.h
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
#if !defined(__XTPCHARTBARSERIESLABEL_H__)
#define __XTPCHARTBARSERIESLABEL_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CXTPChartSeriesView;


enum XTPChartBarLabelPosition
{
	xtpChartBarLabelTop,
	xtpChartBarLabelCenter
};

//===========================================================================
// Summary:
//     This class abstracts the label of a bar series, it is a kind of point
//     series label.
// Remarks:
// See Also:
//===========================================================================
class _XTP_EXT_CLASS CXTPChartBarSeriesLabel : public CXTPChartDiagram2DSeriesLabel
{
	DECLARE_SERIAL(CXTPChartBarSeriesLabel);
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartBarSeriesLabel object.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartBarSeriesLabel();

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPChartBarSeriesLabel object, handles cleanup
	//-------------------------------------------------------------------------
	virtual ~CXTPChartBarSeriesLabel();

public:
	void SetPosition(XTPChartBarLabelPosition nPosition);
	XTPChartBarLabelPosition GetPosition() const;

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     This function create the view of the bar series label.
	// Parameters:
	//     pDC -        The device context.
	//     pPointView - A pointer to the series point view object.
	// Returns:
	//     A pointer to CXTPChartElementView object, which a polymorphic to CXTPChartBarSeriesLabelView.
	// Remarks:
	//-----------------------------------------------------------------------
	virtual CXTPChartElementView* CreateView(CXTPChartDeviceContext* pDC, CXTPChartSeriesPointView* pPointView, CXTPChartElementView* pParentView);


	void DoPropExchange(CXTPPropExchange* pPX);

protected:
	XTPChartBarLabelPosition m_nPosition;


#ifdef _XTP_ACTIVEX
public:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPChartBarSeriesLabel);
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
class _XTP_EXT_CLASS CXTPChartBarSeriesLabelView : public CXTPChartDiagram2DSeriesLabelView
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartBarSeriesLabel object.
	// Parameters:
	//     pLabel     - Pointer chart series label.
	//     pPointView - Pointer to chart series point view.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartBarSeriesLabelView(CXTPChartSeriesLabel* pLabel, CXTPChartSeriesPointView* pPointView, CXTPChartElementView* pParentView);

	//-----------------------------------------------------------------------
	// Summary:
	//     This function calculates the anchor point of the label stem.
	// Returns:
	//     A CXTPChartPointF object denoting the anchor point.
	// Remarks:
	//-----------------------------------------------------------------------
	virtual CXTPChartPointF GetAnchorPoint() const;
	virtual double GetAnchorAngle() const;

	CXTPChartDeviceCommand* CreateDeviceCommand(CXTPChartDeviceContext* pDC);
	void CalculateLayout(CXTPChartDeviceContext* pDC);
};

AFX_INLINE void CXTPChartBarSeriesLabel::SetPosition(XTPChartBarLabelPosition nPosition) {
	m_nPosition = nPosition;
	OnChartChanged();
}
AFX_INLINE XTPChartBarLabelPosition CXTPChartBarSeriesLabel::GetPosition() const {
	return m_nPosition;
}

#endif //#if !defined(__XTPCHARTBARSERIESLABEL_H__)
