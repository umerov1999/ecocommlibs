// XTPChartAreaSeriesStyle.h
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
#if !defined(__XTPCHARTAREASERIESSTYLE_H__)
#define __XTPCHARTAREASERIESSTYLE_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CXTPChartSeriesView;
class CXTPChartBorder;
class CXTPChartFillStyle;

//===========================================================================
// Summary:
//     CXTPChartAreaSeriesStyle specialized class from CXTPChartPointSeriesStyle
//     class represents the area series style.
// Remarks:
//     Area series displays graphically the quantitative data in a chart.
//     it displays a series as a set of points connected by a line or spline
//     with all the area filled in below the line.
//
//===========================================================================
class _XTP_EXT_CLASS CXTPChartAreaSeriesStyle : public CXTPChartPointSeriesStyle
{
	DECLARE_SERIAL(CXTPChartAreaSeriesStyle)

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartAreaSeriesStyle object.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartAreaSeriesStyle();

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPChartAreaSeriesStyle object, handles cleanup
	//-------------------------------------------------------------------------
	virtual ~CXTPChartAreaSeriesStyle();

public:

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the background fill style.
	// Returns:
	//     Returns the pointer to CXTPChartFillStyle object, which abstracts
	//     various fill styles.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	CXTPChartFillStyle* GetFillStyle() const;

	CXTPChartBorder* GetBorder() const;

public:
	void DoPropExchange(CXTPPropExchange* pPX);

public:

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the transparency of the filled area with
	//     respect to the background.
	// Returns:
	//     Returns an integer whose values are from 0 to 255.
	// Remarks:
	//     A value of 0 means fully transparent and 255 fully opaque
	// See Also:
	//-------------------------------------------------------------------------
	int GetTransparency() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to set the transparency of the filled area with
	//     respect to the background.
	// Parameters:
	//     nTransparency - an integer whose values are from 0 to 255.
	// Remarks:
	//     A value of 0 means fully transparent and 255 fully opaque
	// See Also:
	//-------------------------------------------------------------------------
	void SetTransparency(int nTransparency);

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
	DECLARE_OLETYPELIB_EX(CXTPChartAreaSeriesStyle);
	DECLARE_OLECREATE_EX(CXTPChartAreaSeriesStyle)

	LPDISPATCH OleGetFillStyle();
	LPDISPATCH OleGetBorder();
//}}AFX_CODEJOCK_PRIVATE
#endif

protected:
	int m_nTransparency;                //The transparency values range from 0 to 255.
	CXTPChartBorder* m_pBorder;          //The chart border object pointer.

	CXTPChartFillStyle* m_pFillStyle;    //The chart background fill style.

};

AFX_INLINE int CXTPChartAreaSeriesStyle::GetTransparency() const {
	return m_nTransparency;
}
AFX_INLINE void CXTPChartAreaSeriesStyle::SetTransparency(int nTransparency) {
	m_nTransparency = nTransparency;
	OnChartChanged();
}
AFX_INLINE CXTPChartFillStyle* CXTPChartAreaSeriesStyle::GetFillStyle() const {
	return m_pFillStyle;
}
AFX_INLINE CXTPChartBorder* CXTPChartAreaSeriesStyle::GetBorder() const {
	return m_pBorder;
}

#endif //#if !defined(__XTPCHARTAREASERIESSTYLE_H__)
