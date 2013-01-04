// XTPChartRadarAreaSeriesStyle.h
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
#if !defined(__XTPCHARTRADARAREASERIESSTYLE_H__)
#define __XTPCHARTRADARAREASERIESSTYLE_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CXTPChartSeriesView;
class CXTPChartBorder;
class CXTPChartFillStyle;


//===========================================================================
// Summary:
//     This class represents a line series style, which is a kind of
//     CXTPChartSeriesStyle.
// Remarks:
//     A point chart is a type of graph, which displays information as a
//     series of data points.It is a basic type of chart common in many
//     fields.
//===========================================================================
class _XTP_EXT_CLASS CXTPChartRadarAreaSeriesStyle : public CXTPChartRadarPointSeriesStyle
{
	DECLARE_SERIAL(CXTPChartRadarAreaSeriesStyle)

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartRadarAreaSeriesStyle object.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartRadarAreaSeriesStyle();

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPChartRadarAreaSeriesStyle object, handles cleanup
	//-------------------------------------------------------------------------
	virtual ~CXTPChartRadarAreaSeriesStyle();


public:
	void DoPropExchange(CXTPPropExchange* pPX);

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
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to create view of the point series.
	// Parameters:
	//     pSeries - A pointer to chart series object.
	//     pDiagramView - A pointer to the diagram view object.
	// Returns:
	//     A pointer to CXTPChartRadarAreaSeriesView object which is a kind of
	//     CXTPChartSeriesView object.
	// Remarks:
	//-----------------------------------------------------------------------
	virtual CXTPChartSeriesView* CreateView(CXTPChartSeries* pSeries, CXTPChartDiagramView* pDiagramView);


#ifdef _XTP_ACTIVEX
public:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPChartRadarAreaSeriesStyle);
	DECLARE_OLECREATE_EX(CXTPChartRadarAreaSeriesStyle)

	LPDISPATCH OleGetFillStyle();
	LPDISPATCH OleGetBorder();

//}}AFX_CODEJOCK_PRIVATE
#endif

protected:
	int m_nTransparency;                //The transparency values range from 0 to 255.
	CXTPChartBorder* m_pBorder;          //The chart border object pointer.

	CXTPChartFillStyle* m_pFillStyle;    //The chart background fill style.

};


AFX_INLINE int CXTPChartRadarAreaSeriesStyle::GetTransparency() const {
	return m_nTransparency;
}
AFX_INLINE void CXTPChartRadarAreaSeriesStyle::SetTransparency(int nTransparency) {
	m_nTransparency = nTransparency;
	OnChartChanged();
}
AFX_INLINE CXTPChartFillStyle* CXTPChartRadarAreaSeriesStyle::GetFillStyle() const {
	return m_pFillStyle;
}
AFX_INLINE CXTPChartBorder* CXTPChartRadarAreaSeriesStyle::GetBorder() const {
	return m_pBorder;
}

#endif //#if !defined(__XTPCHARTRADARAREASERIESSTYLE_H__)
