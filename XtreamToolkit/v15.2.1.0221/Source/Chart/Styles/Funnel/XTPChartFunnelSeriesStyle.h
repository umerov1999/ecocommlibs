// XTPChartFunnelSeriesStyle.h
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
#if !defined(__XTPCHARTFUNNELSERIESSTYLE_H__)
#define __XTPCHARTFUNNELSERIESSTYLE_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CXTPChartSeriesView;
class CXTPChartBorder;
class CXTPChartFillStyle;


//===========================================================================
// Summary:
//     This class abstracts the basic style of a pie serie.This class is a kind of
//     CXTPChartSeriesStyle.
// Remarks:
//     A pie chart (or a circle graph) is a circular chart divided into sectors,
//     illustrating percents. In a pie chart, the arc length of each sector (and
//     consequently its central angle and area), is proportional to the quantity
//     it represents. Together, the sectors create a full disk.
//===========================================================================
class _XTP_EXT_CLASS CXTPChartFunnelSeriesStyle : public CXTPChartSeriesStyle
{
	DECLARE_SERIAL(CXTPChartFunnelSeriesStyle)
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartFunnelSeriesStyleBase object.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartFunnelSeriesStyle();

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPChartFunnelSeriesStyleBase object, handle cleanup.
	//-------------------------------------------------------------------------
	virtual ~CXTPChartFunnelSeriesStyle();

public:
	int GetPointDistance() const;
	void SetPointDistance(int nPointDistance);

	double GetHeightToWidthRatio() const;
	void SetHeightToWidthRatio(double dRatio);

	void SetEqualHeight(BOOL bEqualHeight);
	BOOL IsEqualHeight() const;

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

protected:
	CXTPChartDiagram* CreateDiagram();
	CXTPChartSeriesView* CreateView(CXTPChartSeries* pSeries, CXTPChartDiagramView* pDiagramView);
	BOOL IsStyleDiagram(CXTPChartDiagram* pDiagram) const;


#ifdef _XTP_ACTIVEX
public:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPChartFunnelSeriesStyle);
	DECLARE_OLECREATE_EX(CXTPChartFunnelSeriesStyle)
//}}AFX_CODEJOCK_PRIVATE
#endif

protected:
	double m_dHeightToWidthRatio;

	int m_nPointDistance;
	CXTPChartBorder* m_pBorder;
	CXTPChartFillStyle* m_pFillStyle;
	BOOL m_bEqualHeight;
};


AFX_INLINE int CXTPChartFunnelSeriesStyle::GetPointDistance() const {
	return m_nPointDistance;
}
AFX_INLINE void CXTPChartFunnelSeriesStyle::SetPointDistance(int nPointDistance) {
	m_nPointDistance = nPointDistance;
	OnChartChanged();
}
AFX_INLINE CXTPChartFillStyle* CXTPChartFunnelSeriesStyle::GetFillStyle() const {
	return m_pFillStyle;
}
AFX_INLINE CXTPChartBorder* CXTPChartFunnelSeriesStyle::GetBorder() const {
	return m_pBorder;
}
AFX_INLINE void CXTPChartFunnelSeriesStyle::SetHeightToWidthRatio(double dRatio) {
	m_dHeightToWidthRatio = dRatio;
	OnChartChanged();
}
AFX_INLINE double CXTPChartFunnelSeriesStyle::GetHeightToWidthRatio() const {
	return m_dHeightToWidthRatio;
}
AFX_INLINE void CXTPChartFunnelSeriesStyle::SetEqualHeight(BOOL bEqualHeight) {
	m_bEqualHeight = bEqualHeight;
	OnChartChanged();
}
AFX_INLINE BOOL CXTPChartFunnelSeriesStyle::IsEqualHeight() const {
	return m_bEqualHeight;
}


#endif //#if !defined(__XTPCHARTFUNNELSERIESSTYLE_H__)
