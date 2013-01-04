// XTPChartLineStyle.h
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
#if !defined(__XTPCHARTLINESTYLE_H__)
#define __XTPCHARTLINESTYLE_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


class CXTPChartElement;
class CXTPChartDeviceCommand;
//===========================================================================
// Summary:
//     This class represents a chart line style, which is a kind of CXTPCmdTarget.
// Remarks:
//===========================================================================
class _XTP_EXT_CLASS CXTPChartLineStyle : public CXTPCmdTarget
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartLineStyle object.
	// Parameters:
	//     pOwner - The owner element.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartLineStyle(CXTPChartElement* pOwner);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to release the object once the object is no more
	//     relevant
	// Remarks:
	//-----------------------------------------------------------------------
	void Release();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to set the thickness of the line.
	// Parameters:
	//     nThickness - The integer value denoting the line thickness.
	// Remarks:
	//-----------------------------------------------------------------------
	void SetThickness(int nThickness);
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the thickness of the line.
	// Returns:
	//     An integer value denoting the line thickness.
	// Remarks:
	//-----------------------------------------------------------------------
	int GetThickness() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to set dash line style.
	// Parameters:
	//     nDashStyle - An enumerated value specifying the line style of choice.
	// Remarks:
	//-----------------------------------------------------------------------
	void SetDashStyle(XTPChartDashStyle nDashStyle);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the dash line style.
	// Returns:
	//     An enumerated value specifying the line style.
	// Remarks:
	//-----------------------------------------------------------------------
	XTPChartDashStyle GetDashStyle() const;

	void SetDashArray(const CXTPChartLineDashArray& dashArray);

public:
	//-------------------------------------------------------------------------
	// Summary:
	//     This function create a CXTPChartDeviceCommand object, this object
	//     represents the rendering of a line.
	// Parameters:
	//     point1     - The start point of the line.
	//     point2     - The end point of the line.
	//     color      - The color of the line.
	// Returns:
	//     Returns CXTPChartDeviceCommand object, this object handles
	//     the rendering of an element in the chart.Here it handles
	//     the drawing of a line.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	CXTPChartDeviceCommand* CreateDeviceCommand(const CXTPChartPointF& point1, const CXTPChartPointF& point2, const CXTPChartColor& color);
	CXTPChartDeviceCommand* CreateDeviceCommand(const CXTPChartPoints& points, const CXTPChartColor& color);

	//-------------------------------------------------------------------------
	// Summary:
	//     This function create a CXTPChartDeviceCommand object, this object
	//     represents the rendering of a spline.
	// Parameters:
	//     points     - The points which form the spline.
	//     color      - The color of the line.
	// Returns:
	//     Returns CXTPChartDeviceCommand object, this object handles
	//     the rendering of an element in the chart.Here it handles
	//     the drawing of a spline.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	CXTPChartDeviceCommand* CreateSplineDeviceCommand(const CXTPChartPoints& points, const CXTPChartColor& color);

public:
	virtual void DoPropExchange(CXTPPropExchange* pPX);


#ifdef _XTP_ACTIVEX
public:
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPChartLineStyle);

	void OleSetDashArray(const VARIANT& var);
//}}AFX_CODEJOCK_PRIVATE
#endif

protected:
	int m_nThickness;               //The line thickness.
	XTPChartDashStyle m_nDashStyle;  //The dash style.
	CXTPChartElement* m_pOwner;      //The owner element.
	CXTPChartLineDashArray m_arrDashArray;
};

AFX_INLINE void CXTPChartLineStyle::Release() {
	InternalRelease();
}
AFX_INLINE int CXTPChartLineStyle::GetThickness() const {
	return m_nThickness;
}
AFX_INLINE XTPChartDashStyle CXTPChartLineStyle::GetDashStyle() const {
	return m_nDashStyle;
}


#endif //#if !defined(__XTPCHARTLINESTYLE_H__)
