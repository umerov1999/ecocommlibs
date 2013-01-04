// XTPChartPolygonDeviceCommand.h
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
#if !defined(__XTPCHARTPOLYGONDEVICECOMMAND_H__)
#define __XTPCHARTPOLYGONDEVICECOMMAND_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class _XTP_EXT_CLASS CXTPChartPolygonDeviceCommand : public CXTPChartDeviceCommand
{
protected:
	CXTPChartPolygonDeviceCommand(const CXTPChartPoints& points);

protected:
	CXTPChartElement* HitTest(CPoint point, CXTPChartElement* pParent) const;

protected:
	CXTPChartPoints m_points;    //The points which form the polygon.
	CXTPChartRectF m_bounds;             //The rectanglur broundary of the polygon.

};

//===========================================================================
// Summary:
//     This class represents a polygon device command,which is a kind of CXTPChartDeviceCommand.
//     It specifically handles the rendering of polygonal shapes in a chart.
// Remarks:
//===========================================================================
class _XTP_EXT_CLASS CXTPChartSolidPolygonDeviceCommand : public CXTPChartPolygonDeviceCommand
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartSolidPolygonDeviceCommand object.
	// Parameters:
	//     points    - The collection of points which make the polygon.
	//     color     - The color of boundary.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartSolidPolygonDeviceCommand(const CXTPChartPoints& points, const CXTPChartColor& color);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     This is a virtual function override of base class CXTPChartDeviceContext,
	//     act polymorphically to do the actual drawing of the chart element, to which
	//     this device command is associated with.
	// Parameters:
	//     pDC      - The device context of the chart.
	// Remarks:
	//-----------------------------------------------------------------------
	void ExecuteOverride(CXTPChartDeviceContext* pDC);

protected:
	CXTPChartColor m_color;      //The color of the boundary.
};

//===========================================================================
// Summary:
//     This class represents a solid spline polygon device command,which is a kind
//     of CXTPChartDeviceCommand.It specifically handles the rendering of solid spline
//     polygonal shapes in a chart.
//
// Remarks:
//===========================================================================
class _XTP_EXT_CLASS CXTPChartSolidSplinePolygonDeviceCommand : public CXTPChartDeviceCommand
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartSolidSplinePolygonDeviceCommand object.
	// Parameters:
	//     points    - The collection of points which make the polygon spline.
	//     color     - The fill color of the solid spline.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartSolidSplinePolygonDeviceCommand(const CXTPChartPoints& points, const CXTPChartColor& color, BOOL bTwoSides = FALSE);
	~CXTPChartSolidSplinePolygonDeviceCommand();

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     This is a virtual function override of base class CXTPChartDeviceContext,
	//     act polymorphically to do the actual drawing of the chart element, to which
	//     this device command is associated with.
	// Parameters:
	//     pDC      - The device context of the chart.
	// Remarks:
	//-----------------------------------------------------------------------
	void ExecuteOverride(CXTPChartDeviceContext* pDC);

	CXTPChartElement* HitTest(CPoint point, CXTPChartElement* pParent) const;

protected:
	CXTPChartColor m_color;      //The fill color of the spline polygon.

	Gdiplus::GpPath* m_pGpPath;
};

//===========================================================================
// Summary:
//     This class represents a gradient polygon device command,which is a kind
//     of CXTPChartDeviceCommand.It specifically handles the rendering of gradient
//     polygonal shapes in a chart.
//
// Remarks:
//===========================================================================
class _XTP_EXT_CLASS CXTPChartGradientPolygonDeviceCommand : public CXTPChartPolygonDeviceCommand
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartGradientPolygonDeviceCommand object.
	// Parameters:
	//     points    - The collection of points which make the polygon spline.
	//     color     - The start color of the gradient.
	//     color2    - The end color of the gradient.
	//     nMode     - The linear gradient mode, it can be horizontal, vertical,
	//                 forward diagonal or backward diagonal.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartGradientPolygonDeviceCommand(const CXTPChartPoints& points, const CXTPChartColor& color, const CXTPChartColor& color2, XTPChartLinearGradientMode nMode);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     This is a virtual function override of base class CXTPChartDeviceContext,
	//     act polymorphically to do the actual drawing of the chart element, to which
	//     this device command is associated with.
	// Parameters:
	//     pDC      - The device context of the chart.
	// Remarks:
	//-----------------------------------------------------------------------
	void ExecuteOverride(CXTPChartDeviceContext* pDC);

protected:
	CXTPChartColor m_color;              //The start color of the gradient.
	CXTPChartColor m_color2;             //The end color of the gradiednt.
	XTPChartLinearGradientMode m_nMode;  //The gradient mode.
};
//===========================================================================
// Summary:
//     This class represents a hatch polygon device command,which is a kind
//     of CXTPChartDeviceCommand.It specifically handles the rendering of hatch
//     polygonal shapes in a chart.
//
// Remarks:
//===========================================================================
class _XTP_EXT_CLASS CXTPChartHatchPolygonDeviceCommand : public CXTPChartPolygonDeviceCommand
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartHatchPolygonDeviceCommand object.
	// Parameters:
	//     points    - The collection of points which make the polygon spline.
	//     color     - The first color of the pattern.
	//     color2    - The second color of the pattern.
	//     nStyle    - The hatch style.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartHatchPolygonDeviceCommand(const CXTPChartPoints& points, const CXTPChartColor& color, const CXTPChartColor& color2, XTPChartHatchStyle nStyle);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     This is a virtual function override of base class CXTPChartDeviceContext,
	//     act polymorphically to do the actual drawing of the chart element, to which
	//     this device command is associated with.
	// Parameters:
	//     pDC      - The device context of the chart.
	// Remarks:
	//-----------------------------------------------------------------------
	void ExecuteOverride(CXTPChartDeviceContext* pDC);

protected:
	CXTPChartColor m_color;          //The first color of the pattern.
	CXTPChartColor m_color2;         //The second color of the pattern.
	XTPChartHatchStyle m_nStyle;     //The hatch style.
};

//===========================================================================
// Summary:
//     This class represents a bounded rectangle device command,which is a kind of
//     CXTPChartDeviceCommand.It specifically handles the rendering of bounded rectangular
//     shapes in a chart.
// Remarks:
//===========================================================================
class _XTP_EXT_CLASS CXTPChartBoundedPolygonDeviceCommand : public CXTPChartDeviceCommand
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartBoundedPolygonDeviceCommand object.
	// Parameters:
	//     rect      - The rectangular area to be rendered by the device command.
	//     color     - The color of the rectangle border.
	//     thickness - The thickness of the rectangular border.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartBoundedPolygonDeviceCommand(const CXTPChartPoints& points, const CXTPChartColor& color, int thickness);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     This is a virtual function override of base class CXTPChartDeviceContext,
	//     act polymorphically to do the actual drawing of the chart element, to which
	//     this device command is associated with.
	// Parameters:
	//     pDC      - The device context of the chart.
	// Remarks:
	//-----------------------------------------------------------------------
	void ExecuteOverride(CXTPChartDeviceContext* pDC);

protected:
	CXTPChartPoints m_points;    //The points which form the spline polygon.
	CXTPChartColor m_color;      //The color of the rectangle border.
	int m_thickness;            //The thickness of the rectangle border.
};

#endif //#if !defined(__XTPCHARTPOLYGONDEVICECOMMAND_H__)
