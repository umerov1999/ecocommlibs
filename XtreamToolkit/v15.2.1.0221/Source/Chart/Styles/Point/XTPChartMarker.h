// XTPChartMarker.h
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
#if !defined(__XTPCHARTMARKER_H__)
#define __XTPCHARTMARKER_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CXTPChartSeriesStyle;
class CXTPChartDeviceCommand;
class CXTPChartDeviceContext;
class CXTPChartFillStyle;


//===========================================================================
// Summary:
//     This enumeration is used for naming the various styles for the point series
//     point marking.
//===========================================================================
enum XTPChartMarkerType
{
	xtpChartMarkerCircle,  //The circular style.
	xtpChartMarkerSquare,
	xtpChartMarkerDiamond,
	xtpChartMarkerTriangle,
	xtpChartMarkerPentagon,
	xtpChartMarkerHexagon,
	xtpChartMarkerStar
};

//===========================================================================
// Summary:
//     This class abstracts a marker, used in marking the points in a point series
//     style chart. Objects class is a kind of CXTPChartElement.
// Remarks:
//     XTPChartMarkerType enumeration defines the various marking styles possible
//     with this marker object.
//===========================================================================
class _XTP_EXT_CLASS CXTPChartMarker : public CXTPChartElement
{
	DECLARE_DYNAMIC(CXTPChartMarker)
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartMarker object.
	// Parameters:
	//     pOwner - A pointer to chart series style object, which owns this object.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartMarker(CXTPChartSeriesStyle* pOwner);

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPChartMarker object, handles cleanup.
	//-------------------------------------------------------------------------
	virtual ~CXTPChartMarker();

public:
	int GetSize() const;
	void SetSize(int nSize);

	void SetBorderVisible(BOOL bBorderVisible);
	BOOL GetBorderVisible() const;

	XTPChartMarkerType GetType() const;
	void SetType(XTPChartMarkerType nType);

	CXTPChartFillStyle* GetFillStyle() const;

	BOOL GetVisible() const;
	void SetVisible(BOOL bVisible);
public:
	virtual void DoPropExchange(CXTPPropExchange* pPX);
public:
	//-------------------------------------------------------------------------
	// Summary:
	//     This function create a CXTPChartDeviceCommand object, this object
	//     represents the rendering of a point marker in the point series chart.
	// Parameters:
	//     pDC     - Pointer to a CXTPChartDeviceContext object.
	//     point   - The center point of the marker.
	//     color   - The first color of the gradient.
	//     color2  - The second color of the gradient.
	//     colorBorder - The border color of the marker.
	// Returns:
	//     Returns CXTPChartDeviceCommand object, this object handles
	//     the rendering of an element in the chart.Here it handles
	//     the drawing of the marker point.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	CXTPChartDeviceCommand* CreateDeviceCommand(CXTPChartDeviceContext* pDC, const CXTPChartPointF& point,
		CXTPChartColor color, CXTPChartColor color2, CXTPChartColor colorBorder);

	//-------------------------------------------------------------------------
	// Summary:
	//     This function create a CXTPChartDeviceCommand object, this object
	//     represents the rendering of a point marker in the point series chart.
	// Parameters:
	//     pDC     - Pointer to a CXTPChartDeviceContext object.
	//     point   - The center point of the marker.
	//     nWidth  - The width of the marker.
	//     color   - The first color of the marker gradient.
	//     color2  - The second color of the marker gradient.
	//     colorBorder - The border color of the marker.
	// Returns:
	//     Returns CXTPChartDeviceCommand object, this object handles
	//     the rendering of an element in the chart.Here it handles
	//     the drawing of the marker point.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	CXTPChartDeviceCommand* CreateDeviceCommand(CXTPChartDeviceContext* pDC, const CXTPChartPointF& point, int nWidth,
		CXTPChartColor color, CXTPChartColor color2, CXTPChartColor colorBorder);


protected:

#ifdef _XTP_ACTIVEX
public:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPChartMarker);

	afx_msg LPDISPATCH OleGetFillStyle();
//}}AFX_CODEJOCK_PRIVATE
#endif
protected:
	BOOL m_bBorderVisible;          //TRUE if the border is visible, FALSE if the border is invisible.
	CXTPChartColor m_clrBorderColor; //The color of the marker border.
	CXTPChartColor m_clrColor;      //The first color of the marker gradient.
	XTPChartMarkerType m_nType;     //The Marker Type
	int m_nSize;                    //The size of the marker.
	BOOL m_bVisible;                //TURE if the marker is visible and FALSE if the marker is invisible.
	CXTPChartFillStyle* m_pFillStyle;
};

AFX_INLINE int CXTPChartMarker::GetSize() const {
	return m_nSize;
}
AFX_INLINE void CXTPChartMarker::SetSize(int nSize) {
	m_nSize = nSize;
	OnChartChanged();
}
AFX_INLINE void CXTPChartMarker::SetBorderVisible(BOOL bBorderVisible) {
	m_bBorderVisible = bBorderVisible;
	OnChartChanged();
}
AFX_INLINE BOOL CXTPChartMarker::GetBorderVisible() const {
	return m_bBorderVisible;
}
AFX_INLINE XTPChartMarkerType CXTPChartMarker::GetType() const {
	return m_nType;
}
AFX_INLINE void CXTPChartMarker::SetType(XTPChartMarkerType nType) {
	m_nType = nType;
	OnChartChanged();
}
AFX_INLINE CXTPChartFillStyle* CXTPChartMarker::GetFillStyle() const {
	return m_pFillStyle;
}
AFX_INLINE BOOL CXTPChartMarker::GetVisible() const {
	return m_bVisible;
}
AFX_INLINE void CXTPChartMarker::SetVisible(BOOL bVisible) {
	m_bVisible = bVisible;
	OnChartChanged();
}

#endif //#if !defined(__XTPCHARTMARKER_H__)
