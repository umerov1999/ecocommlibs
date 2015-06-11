// XTPChartDiagram2DView.h
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
#if !defined(__XTPCHARTDIAGRAM2DVIEW_H__)
#define __XTPCHARTDIAGRAM2DVIEW_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CXTPChartAxis;
class CXTPChartDiagram2DPane;
class CXTPChartDiagram2DPaneView;
class CXTPChartDiagram2DAppearance;
class CXTPChartAxisView;

//===========================================================================
// Summary:
//     This class represents the view of a chart 2D diagram, which is a kind of
//     CXTPChartDiagramView.
// Remarks:
//===========================================================================
class _XTP_EXT_CLASS CXTPChartDiagram2DView :public CXTPChartDiagramView
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartDiagram2DView object.
	// Parameters:
	//     pDiagram     - A pointer to the chart diagram object.
	//     pParent      - A pointer to the parent view object.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartDiagram2DView(CXTPChartDiagram* pDiagram, CXTPChartElementView* pParent);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the diagram pane view object to which this
	//     CXTPChartDiagram2DView object is associated with.
	// Parameters:
	//     pDiagram     - A pointer to the chart diagram object.
	//     pParent      - A pointer to the parent view object.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartDiagram2DPaneView* GetPaneView() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to create the view of the 2D diagram.
	// Parameters:
	//     pDC      - A pointer to the chart device context.
	// Remarks:
	//-----------------------------------------------------------------------
	void CreateView(CXTPChartDeviceContext* pDC);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to calculate the layout based on a rectangular
	//     boundary.
	// Parameters:
	//     pDC      - A pointer to the chart device context.
	//     rcBounds - The diagram boundary.
	// Remarks:
	//-----------------------------------------------------------------------
	void CalculateView(CXTPChartDeviceContext* pDC, CRect rcBounds);

	void UpdateRange(CXTPChartDeviceContext* pDC);

	//-----------------------------------------------------------------------
	// Summary:
	//     Use this function to add an axis view to the diagram view.
	//     boundary.
	// Parameters:
	//     pDC          - A pointer to the chart device context.
	//     pParentView  - The parent view of the axis view.
	//     pAxis        - A pointer to the axis object, whose view is to be
	//                    added.
	// Remarks:
	//-----------------------------------------------------------------------
	void AddAxisView(CXTPChartDeviceContext* pDC, CXTPChartElementView* pParentView, CXTPChartAxis* pAxis);

	CXTPChartDeviceCommand* CreateDeviceCommand(CXTPChartDeviceContext* pDC);

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns Axis View
	// Parameters:
	//     pAxis - Axis
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartAxisView* GetAxisView(CXTPChartAxis* pAxis) const;

	virtual void OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	virtual void OnLButtonDown(UINT nFlags, CPoint point);
	virtual BOOL OnSetCursor(CPoint point);

public:
	void CheckLabelBounds(const CXTPChartRectF& rcBounds);

public:

	CXTPChartElementView* m_pAxisViews;      //The axis view.
	CXTPChartDiagram2DPaneView* m_pPaneView;   //The pane view.

	CRect m_rcLabelPadding;
};


AFX_INLINE CXTPChartDiagram2DPaneView* CXTPChartDiagram2DView::GetPaneView() const {
	return m_pPaneView;
}


#endif //#if !defined(__XTPCHARTDIAGRAM2DVIEW_H__)
