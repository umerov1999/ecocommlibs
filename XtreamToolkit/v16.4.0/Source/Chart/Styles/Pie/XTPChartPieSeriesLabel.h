// XTPChartPieSeriesLabel.h
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
#if !defined(__XTPCHARTPIESERIESLABEL_H__)
#define __XTPCHARTPIESERIESLABEL_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CXTPChartSeriesView;
class CXTPChartPieSeriesPointView;
class CXTPChartPieSeriesStyleBase;
class CXTPChartPie3DDiagramDomain;
//===========================================================================
// Summary:
//     This enumeration tell the various positions available for the pie label.
// Remarks:
//===========================================================================
enum XTPChartPieLabelPosition
{
	xtpChartPieLabelOutside,    // Outside the pie.
	xtpChartPieLabelInside,     // Inside the pie.
	xtpChartPieLabelRadial,     // Radial label.
	xtpChartPieLabelTwoColumns  // 2 Columns
};

//===========================================================================
// Summary:
//     CXTPChartPieSeriesLabel is a kind of CXTPChartSeriesLabel, this class
//     abstracts the label of a pie series.
// Remarks:
//
//===========================================================================
class _XTP_EXT_CLASS CXTPChartPieSeriesLabel : public CXTPChartSeriesLabel
{
	DECLARE_SERIAL(CXTPChartPieSeriesLabel)

	class CView;

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartPieSeriesLabel object.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartPieSeriesLabel();

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPChartPieSeriesLabel object, handles cleanup
	//-------------------------------------------------------------------------
	virtual ~CXTPChartPieSeriesLabel();


public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the label position.
	// Returns:
	//     A pie label position object describing the position of pie label.
	//-------------------------------------------------------------------------
	XTPChartPieLabelPosition GetPosition() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to set the label position.
	// Parameters:
	//     nPosition - A pie label position object describing the position of
	//                 pie label.
	//-------------------------------------------------------------------------
	void SetPosition(XTPChartPieLabelPosition nPosition);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the basic style of the pie series.
	// Returns:
	//     A pointer to CXTPChartPieSeriesStyleBase object.
	//-------------------------------------------------------------------------
	CXTPChartPieSeriesStyleBase* GetStyle() const;

public:
	void DoPropExchange(CXTPPropExchange* pPX);
protected:
	//-------------------------------------------------------------------------
	// Summary:
	//     This function creates the view of the pie series.
	// Parameters:
	//     pDC - The chart device context object pointer.
	//     pPointView - The series point view.
	// Returns:
	//     Returns an pointer to CXTPChartSeriesLabelView which is a kind of
	//     CXTPChartElementView object newly created.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	virtual CXTPChartElementView* CreateView(CXTPChartDeviceContext* pDC, CXTPChartSeriesPointView* pPointView, CXTPChartElementView* pParentView);

public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to calculate the anchor points and angles of the
	//     pie label stem.
	// Parameters:
	//     pPointView       - The pie series point view.
	//     borderThickness  - The border thickness of the label.
	//     lineAngle        - The line(stem) angle, out param.
	// Returns:
	//     Returns CXTPChartPointF object denoting the anchor point.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	virtual CXTPChartPointF CalculateAnchorPointAndAngles(CXTPChartPieSeriesPointView* pPointView,
		int borderThickness, double& lineAngle);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to check whether the label is inside the pie.
	// Returns:
	//     Returns TRUE if the label is inside the pie and FALSE if not.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	BOOL IsInside() const;

protected:

protected:
	XTPChartPieLabelPosition m_nPosition;    //The pie label position.



#ifdef _XTP_ACTIVEX
public:
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPChartPieSeriesLabel);
	//}}AFX_CODEJOCK_PRIVATE
#endif
};

//===========================================================================
// Summary:
//     CXTPChartPie3DSeriesLabel is a kind of CXTPChartPieSeriesLabel, this class
//     abstracts the label of a 3D pie series.
// Remarks:
//
//===========================================================================
class _XTP_EXT_CLASS CXTPChartPie3DSeriesLabel : public CXTPChartPieSeriesLabel
{
	DECLARE_SERIAL(CXTPChartPie3DSeriesLabel)
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartPie3DSeriesLabel object.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartPie3DSeriesLabel();
	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPChartPie3DSeriesLabel object, handles cleanup
	//-------------------------------------------------------------------------
	virtual ~CXTPChartPie3DSeriesLabel();

protected:
	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to calculate the anchor points and angles of the
	//     3d pie label stem.
	// Parameters:
	//     pPointView       - The pie series point view.
	//     borderThickness  - The border thickness of the label.
	//     lineAngle        - The line(stem) angle, out param.
	// Returns:
	//     Returns CXTPChartPointF object denoting the anchor point.
	// See Also:
	//-------------------------------------------------------------------------
	CXTPChartPointF CalculateAnchorPointAndAngles(CXTPChartPieSeriesPointView* pPointView,
		int borderThickness, double& lineAngle);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to project the 3D pie label on 2D window.
	// Parameters:
	//     pDomain      - A pointer to the 3D diagram domain object.
	//     labelPoint   - The start point of the label.
	//     lineAngle    - The line(stem) angle, out param.
	// Returns:
	//     Returns CXTPChartPointF object denoting the anchor point.
	// See Also:
	//-------------------------------------------------------------------------
	CXTPChartPointF Project(CXTPChartPie3DDiagramDomain* pDomain, CXTPChartPointF labelPoint, double& lineAngle);
};

//===========================================================================
// Summary:
//     CXTPChartTorus3DSeriesLabel is a kind of CXTPChartPie3DSeriesLabel, this class
//     abstracts the label of a 3D pie series.
// Remarks:
//
//===========================================================================
class _XTP_EXT_CLASS CXTPChartTorus3DSeriesLabel : public CXTPChartPie3DSeriesLabel
{
	DECLARE_SERIAL(CXTPChartTorus3DSeriesLabel)
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartTorus3DSeriesLabel object.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartTorus3DSeriesLabel();

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPChartTorus3DSeriesLabel object, handles cleanup
	//-------------------------------------------------------------------------
	virtual ~CXTPChartTorus3DSeriesLabel();

protected:
	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to calculate the anchor points and angles of the
	//     3d torus label stem.
	// Parameters:
	//     pPointView       - The pie series point view.
	//     borderThickness  - The border thickness of the label.
	//     lineAngle        - The line(stem) angle, out param.
	// Returns:
	//     Returns CXTPChartPointF object denoting the anchor point.
	// See Also:
	//-------------------------------------------------------------------------
	CXTPChartPointF CalculateAnchorPointAndAngles(CXTPChartPieSeriesPointView* pPointView,
		int borderThickness, double& lineAngle);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to project the 3D torus label on 2D window.
	// Parameters:
	//     pDomain      - A pointer to the 3D diagram domain object.
	//     labelPoint   - The start point of the label.
	//     lineAngle    - The line(stem) angle, out param.
	// Returns:
	//     Returns CXTPChartPointF object denoting the anchor point.
	// See Also:
	//-------------------------------------------------------------------------
	CXTPChartPointF Project(CXTPChartPie3DDiagramDomain* pDomain, CXTPChartPointF labelPoint, double& lineAngle);

};

class CXTPChartPieSeriesLabelView;

class CXTPChartPieSeriesLabelsView : public CXTPChartElementView
{
public:
	CXTPChartPieSeriesLabelsView(CXTPChartElementView* pParentView);

public:
	CXTPChartDeviceCommand* CreateDeviceCommand(CXTPChartDeviceContext* pDC);

	void ResolveOverlapping();
	void ResolveOverlappingColumn(CArray<CXTPChartPieSeriesLabelView*, CXTPChartPieSeriesLabelView*>& arrColumn);

protected:
};

AFX_INLINE XTPChartPieLabelPosition CXTPChartPieSeriesLabel::GetPosition() const {
	return m_nPosition;
}
AFX_INLINE void CXTPChartPieSeriesLabel::SetPosition(XTPChartPieLabelPosition nPosition) {
	m_nPosition = nPosition;
	OnChartChanged();
}


#endif //#if !defined(__XTPCHARTPIESERIESLABEL_H__)
