// XTPChartPyramidSeriesLabel.h
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
#if !defined(__XTPCHARTPYRAMIDSERIESLABEL_H__)
#define __XTPCHARTPYRAMIDSERIESLABEL_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


class CXTPChartSeriesView;
class CXTPChartPyramidSeriesPointView;
class CXTPChartPyramidSeriesStyle;

//===========================================================================
// Summary:
//     This enumeration tell the various positions available for the pie label.
// Remarks:
//===========================================================================
enum XTPChartPyramidLabelPosition
{
	xtpChartPyramidLabelLeft,
	xtpChartPyramidLabelRight,
	xtpChartPyramidLabelCenter,
};

//===========================================================================
// Summary:
//     CXTPChartPyramidSeriesLabel is a kind of CXTPChartSeriesLabel, this class
//     abstracts the label of a pie series.
// Remarks:
//
//===========================================================================
class _XTP_EXT_CLASS CXTPChartPyramidSeriesLabel : public CXTPChartSeriesLabel
{
	DECLARE_SERIAL(CXTPChartPyramidSeriesLabel)

	class CView;

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartPyramidSeriesLabel object.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartPyramidSeriesLabel();

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPChartPyramidSeriesLabel object, handles cleanup
	//-------------------------------------------------------------------------
	virtual ~CXTPChartPyramidSeriesLabel();


public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the label position.
	// Returns:
	//     A pie label position object describing the position of pie label.
	//-------------------------------------------------------------------------
	XTPChartPyramidLabelPosition GetPosition() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to set the label position.
	// Parameters:
	//     nPosition - A pie label position object describing the position of
	//                 pie label.
	//-------------------------------------------------------------------------
	void SetPosition(XTPChartPyramidLabelPosition nPosition);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the basic style of the pie series.
	// Returns:
	//     A pointer to CXTPChartPyramidSeriesStyle object.
	//-------------------------------------------------------------------------
	CXTPChartPyramidSeriesStyle* GetStyle() const;

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
	//     Call this function to check whether the label is inside the pie.
	// Returns:
	//     Returns TRUE if the label is inside the pie and FALSE if not.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	BOOL IsInside() const;

protected:

protected:
	XTPChartPyramidLabelPosition m_nPosition;    //The pie label position.



#ifdef _XTP_ACTIVEX
public:
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPChartPyramidSeriesLabel);
	//}}AFX_CODEJOCK_PRIVATE
#endif
};



AFX_INLINE XTPChartPyramidLabelPosition CXTPChartPyramidSeriesLabel::GetPosition() const {
	return m_nPosition;
}
AFX_INLINE void CXTPChartPyramidSeriesLabel::SetPosition(XTPChartPyramidLabelPosition nPosition) {
	m_nPosition = nPosition;
	OnChartChanged();
}


#endif //#if !defined(__XTPCHARTPYRAMIDSERIESLABEL_H__)
