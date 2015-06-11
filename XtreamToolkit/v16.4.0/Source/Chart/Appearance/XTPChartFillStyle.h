// XTPChartFillStyle.h
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
#if !defined(__XTPCHARTFILLSTYLE_H__)
#define __XTPCHARTFILLSTYLE_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


class CXTPChartElement;
class CXTPChartDeviceCommand;
class CXTPPropExchange;

//===========================================================================
// Summary:
//     This class abstracts a fill style used to fill the background of various
//     chart elements.
// Remarks:
//===========================================================================
class _XTP_EXT_CLASS CXTPChartFillStyle : public CXTPCmdTarget
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartFillStyle object.
	// Parameters:
	//     pOwner - The pointer to a chart element which owns this fill style
	//              object.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartFillStyle(CXTPChartElement* pOwner);

public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to delete the object.
	//-------------------------------------------------------------------------
	void Release();

public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to set a filling mode.
	// Parameters:
	//     nFillMode - The enumerated value specifying the fill mode of choice.
	// Remarks:
	//     The fill modes used in XTP chart are Empty,Solid,Gradient and Hatch.
	//-------------------------------------------------------------------------
	void SetFillMode(XTPChartFillMode nFillMode);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get a filling mode.
	// Returns:
	//    The enumerated value specifying the current fill mode.
	// Remarks:
	//     The fill modes used in XTP chart are Empty,Solid,Gradient and Hatch.
	//-------------------------------------------------------------------------
	XTPChartFillMode GetFillMode() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to set the angle for gradient filling.
	// Parameters:
	//     nDirection - The enumerated value specifying the angle of gradient.
	// Remarks:
	//     The common gradient angle are 0, 45, 90, 135, 180, 225, 270, 315.
	//-------------------------------------------------------------------------
	void SetGradientAngle(XTPChartGradientAngle nAngle);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the angle for gradient filling.
	// Returns:
	//     An enumerated value specifying the current angle of gradient.
	// Remarks:
	//     The common gradient angle are 0, 45, 90, 135, 180, 225, 270, 315.
	//-------------------------------------------------------------------------
	XTPChartGradientAngle GetGradientAngle() const;

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to set the hatch style used for filling the background
	//     of a particular chart entity.
	// Parameters:
	//     nStyle - The enumerated value specifying the hatch style of choice.
	// Remarks:
	//-------------------------------------------------------------------------
	void SetHatchStyle(XTPChartHatchStyle nStyle);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the hatch style used for filling the background
	//     of a particular chart entity.
	// Returns:
	//     An enumerated value specifying the hatch style currently using.
	// Remarks:
	//-------------------------------------------------------------------------
	XTPChartHatchStyle GetHatchStyle() const;


public:
	//-------------------------------------------------------------------------
	// Summary:
	//     This function create a CXTPChartDeviceCommand object, this object
	//     represents the rendering of an element in the chart.
	// Parameters:
	//     bounds  - The rectangular boundary, within the limit the drawing has
	//               to be done.
	//     color   - The first color used in drawing.
	//     color2  - The second color used in drawing.
	// Returns:
	//     Returns CXTPChartDeviceCommand object, this object handles
	//     the rendering of an element in the chart.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	CXTPChartDeviceCommand* CreateDeviceCommand(const CXTPChartRectF& bounds, const CXTPChartColor& color, const CXTPChartColor& color2);

	//-------------------------------------------------------------------------
	// Summary:
	//     This function create a CXTPChartDeviceCommand object, this object
	//     represents the rendering of an element in the chart.
	// Parameters:
	//     arrPoints - The array of points which form a polygonal element.
	//     color     - The first color used in drawing.
	//     color2    - The second color used in drawing.
	// Returns:
	//     Returns CXTPChartDeviceCommand object, this object handles
	//     the rendering of an element in the chart.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	CXTPChartDeviceCommand* CreateDeviceCommand(const CXTPChartPoints& arrPoints, const CXTPChartColor& color, const CXTPChartColor& color2);

	CXTPChartDeviceCommand* CreateCircleDeviceCommand(const CXTPChartPointF& center, double radius, const CXTPChartColor& color, const CXTPChartColor& color2);

	//-------------------------------------------------------------------------
	// Summary:
	//     This function create a CXTPChartDeviceCommand object for rendering,
	//     of a spline.
	// Parameters:
	//     arrPoints - The array of points which form a spline element.
	//     color     - The first color used in drawing.
	//     color2    - The second color used in drawing.
	// Returns:
	//     Returns CXTPChartDeviceCommand object, this object handles
	//     the rendering of an element in the chart.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	CXTPChartDeviceCommand* CreateSplineDeviceCommand(const CXTPChartPoints& arrPoints, const CXTPChartColor& color, const CXTPChartColor& color2, BOOL bTwoSides = FALSE);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to Store/Load the properties of fill style
	//     object using the specified data object.
	// Parameters:
	//     pPX - Source or destination CXTPPropExchange data object reference.
	// Remarks:
	//     This member function is used to store or load property data to or
	//     from a storage.
	//-----------------------------------------------------------------------
	void DoPropExchange(CXTPPropExchange* pPX);

public:
	CXTPChartFillStyle* CreateRotatedStyle();

protected:

#ifdef _XTP_ACTIVEX
public:
	//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPChartFillStyle);

	int OleGetFillMode();
	int OleGetHatchStyle();
	int OleGetGradientAngle();
	//}}AFX_CODEJOCK_PRIVATE
#endif

protected:
	CXTPChartElement* m_pOwner;              // The owner element.
	XTPChartFillMode m_nFillMode;            // The background fill mode.
	XTPChartGradientAngle m_nGradientAngle;  // The gradient angle.
	XTPChartHatchStyle m_nHatchStyle;        // The hatch style used.
};


AFX_INLINE void CXTPChartFillStyle::Release()
{
	InternalRelease();
}

AFX_INLINE XTPChartFillMode CXTPChartFillStyle::GetFillMode() const
{
	return m_nFillMode;
}

AFX_INLINE XTPChartGradientAngle CXTPChartFillStyle::GetGradientAngle() const
{
	return m_nGradientAngle;
}

AFX_INLINE XTPChartHatchStyle CXTPChartFillStyle::GetHatchStyle() const
{
	return m_nHatchStyle;
}


#endif //#if !defined(__XTPCHARTFILLSTYLE_H__)
