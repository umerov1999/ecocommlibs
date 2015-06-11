// XTPChartLineDeviceCommand.h
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
#if !defined(__XTPCHARTLINEDEVICECOMMAND_H__)
#define __XTPCHARTLINEDEVICECOMMAND_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <Common/Base/Types/XTPPoint3.h>

typedef CArray<float, float> CXTPChartLineDashArray;

class _XTP_EXT_CLASS CXTPChartLineDeviceCommand : public CXTPChartDeviceCommand
{
protected:
	CXTPChartLineDeviceCommand(const CXTPPoint3d& p1, const CXTPPoint3d& p2, int thickness);

	virtual CXTPChartElement* HitTest(CPoint point, CXTPChartElement* pParent) const;

protected:
	CXTPPoint3d m_p1;  //The first point.
	CXTPPoint3d m_p2;  //The second point.
	int m_nThickness;           //The thickness of the line.
};


//===========================================================================
// Summary:
//     This class represents a solid line device command, which is a kind of
//     CXTPChartDeviceCommand It specifically handles the rendering of solid
//     lines in a chart.
//
// Remarks:
//===========================================================================
class _XTP_EXT_CLASS CXTPChartSolidLineDeviceCommand : public CXTPChartLineDeviceCommand
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartSolidLineDeviceCommand object.
	// Parameters:
	//     p1        - The first point of the line.
	//     p2        - The second point of the line
	//     color     - The color of the line.
	//     thickness - The thickness of the line.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartSolidLineDeviceCommand(const CXTPPoint3d& p1, const CXTPPoint3d& p2, const CXTPChartColor& color, int thickness);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     This is a virtual function override of base class CXTPChartDeviceContext,
	//     act polymorphically to do the actual drawing of the chart element, to which
	//     this device command is associated with.
	// Parameters:
	//     pDC      - The GDI device context of the chart.
	// Remarks:
	//-----------------------------------------------------------------------
	void ExecuteOverride(CXTPChartDeviceContext* pDC);

protected:
	CXTPChartColor m_color;      //The line color.
};


class _XTP_EXT_CLASS CXTPChartPolylineDeviceCommand : public CXTPChartDeviceCommand
{
protected:
	CXTPChartPolylineDeviceCommand(const CXTPChartPoints& p, int thickness);

protected:
	CXTPChartPoints m_p;     //The points which form the spline.
	int m_nThickness;       //The thickness of the spline.
};

//===========================================================================
// Summary:
//     This class represents a solid polyline device command, which is a kind of
//     CXTPChartDeviceCommand It specifically handles the rendering of solid
//     lines in a chart.
//
// Remarks:
//===========================================================================
class _XTP_EXT_CLASS CXTPChartSolidPolylineDeviceCommand : public CXTPChartPolylineDeviceCommand
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartSolidPolylineDeviceCommand object.
	// Parameters:
	//     p1        - The first point of the line.
	//     p2        - The second point of the line
	//     color     - The color of the line.
	//     thickness - The thickness of the line.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartSolidPolylineDeviceCommand(const CXTPChartPoints& p, const CXTPChartColor& color, int thickness);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     This is a virtual function override of base class CXTPChartDeviceContext,
	//     act polymorphically to do the actual drawing of the chart element, to which
	//     this device command is associated with.
	// Parameters:
	//     pDC      - The GDI device context of the chart.
	// Remarks:
	//-----------------------------------------------------------------------
	void ExecuteOverride(CXTPChartDeviceContext* pDC);

protected:
	CXTPChartColor m_color;      //The line color.
};


class _XTP_EXT_CLASS CXTPChartSplineDeviceCommand : public CXTPChartDeviceCommand
{
protected:
	CXTPChartSplineDeviceCommand(const CXTPChartPoints& p, int thickness);

protected:
	CXTPChartElement* HitTest(CPoint point, CXTPChartElement* pParent) const;

protected:
	CXTPChartPoints m_p;     //The points which form the spline.
	int m_nThickness;       //The thickness of the spline.
};
//===========================================================================
// Summary:
//     This class represents a solid spline device command, which is a kind of
//     CXTPChartDeviceCommand It specifically handles the rendering of solid
//     splines in a chart.
//
// Remarks:
//===========================================================================
class _XTP_EXT_CLASS CXTPChartSolidSplineDeviceCommand : public CXTPChartSplineDeviceCommand
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartSolidLineDeviceCommand object.
	// Parameters:
	//     p         - The points which forms the spline.
	//     color     - The color of the spline.
	//     thickness - The thickness of the spline.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartSolidSplineDeviceCommand(const CXTPChartPoints& p, const CXTPChartColor& color, int thickness);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     This is a virtual function override of base class CXTPChartDeviceContext,
	//     act polymorphically to do the actual drawing of the chart element, to which
	//     this device command is associated with.
	// Parameters:
	//     pDC      - The GDI device context of the chart.
	// Remarks:
	//-----------------------------------------------------------------------
	void ExecuteOverride(CXTPChartDeviceContext* pDC);

protected:
	CXTPChartColor m_color;  //The spline color.

};

//===========================================================================
// Summary:
//     This class represents a dash line device command, which is a kind of
//     CXTPChartDeviceCommand It specifically handles the rendering of solid
//     lines in a chart.
//
// Remarks:
//===========================================================================
class _XTP_EXT_CLASS CXTPChartDashedLineDeviceCommand : public CXTPChartLineDeviceCommand
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartSolidLineDeviceCommand object.
	// Parameters:
	//     p1         - The first point of the line.
	//     p2         - The second point of the line
	//     color      - The color of the line.
	//     thickness  - The thickness of the line.
	//     nDashStyle - The dash style.
	// Remarks:
	//     The dash line style are the following.
	//     Empty style, invisible.
	//     Solid line style.
	//     Dash line style.
	//     Dot line style.
	//     Alternate dash and dot line style.
	//     Dash dot dot line style.
	//-----------------------------------------------------------------------
	CXTPChartDashedLineDeviceCommand(const CXTPPoint3d& p1, const CXTPPoint3d& p2, const CXTPChartColor& color, int thickness, XTPChartDashStyle nDashStyle, const CXTPChartLineDashArray& dashArray);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     This is a virtual function override of base class CXTPChartDeviceContext,
	//     act polymorphically to do the actual drawing of the chart element, to which
	//     this device command is associated with.
	// Parameters:
	//     pDC      - The GDI device context of the chart.
	// Remarks:
	//-----------------------------------------------------------------------
	void ExecuteOverride(CXTPChartDeviceContext* pDC);

protected:
	CXTPChartColor m_color;          //The line color.
	XTPChartDashStyle m_nDashStyle;  //The dash style.
	CXTPChartLineDashArray m_arrDashArray;
};

//===========================================================================
// Summary:
//     This class represents a dashed spline device command, which is a kind of
//     CXTPChartDeviceCommand It specifically handles the rendering of dashed
//     splines in a chart.
//
// Remarks:
//===========================================================================
class _XTP_EXT_CLASS CXTPChartDashedSplineDeviceCommand : public CXTPChartSplineDeviceCommand
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartSolidLineDeviceCommand object.
	// Parameters:
	//     p         - The points which forms the spline.
	//     color     - The color of the spline.
	//     thickness - The thickness of the spline.
	//     nDashStyle - The dash style.
	// Remarks:
	//     The dash line style are the following.
	//     Empty style, invisible.
	//     Solid line style.
	//     Dash line style.
	//     Dot line style.
	//     Alternate dash and dot line style.
	//     Dash dot dot line style.
	//-----------------------------------------------------------------------
	CXTPChartDashedSplineDeviceCommand(const CXTPChartPoints& p, const CXTPChartColor& color, int thickness, XTPChartDashStyle nDashStyle, const CXTPChartLineDashArray& dashArray);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     This is a virtual function override of base class CXTPChartDeviceContext,
	//     act polymorphically to do the actual drawing of the chart element, to which
	//     this device command is associated with.
	// Parameters:
	//     pDC      - The GDI device context of the chart.
	// Remarks:
	//-----------------------------------------------------------------------
	void ExecuteOverride(CXTPChartDeviceContext* pDC);

protected:
	CXTPChartColor m_color;          //The spline color.
	XTPChartDashStyle m_nDashStyle;  //The dash style.
	CXTPChartLineDashArray m_arrDashArray;
};

//===========================================================================
// Summary:
//     This class represents a dashed polyline device command, which is a kind of
//     CXTPChartDeviceCommand It specifically handles the rendering of dashed
//     lines in a chart.
//
// Remarks:
//===========================================================================
class _XTP_EXT_CLASS CXTPChartDashedPolylineDeviceCommand : public CXTPChartPolylineDeviceCommand
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartDashedPolylineDeviceCommand object.
	// Parameters:
	//     p1        - The first point of the line.
	//     p2        - The second point of the line
	//     color     - The color of the line.
	//     thickness - The thickness of the line.
	//     nDashStyle - The dash style.
	// Remarks:
	//     The dash line style are the following.
	//     Empty style, invisible.
	//     Solid line style.
	//     Dash line style.
	//     Dot line style.
	//     Alternate dash and dot line style.
	//     Dash dot dot line style.
	//-----------------------------------------------------------------------
	CXTPChartDashedPolylineDeviceCommand(const CXTPChartPoints& p, const CXTPChartColor& color, int thickness, XTPChartDashStyle nDashStyle, const CXTPChartLineDashArray& dashArray);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     This is a virtual function override of base class CXTPChartDeviceContext,
	//     act polymorphically to do the actual drawing of the chart element, to which
	//     this device command is associated with.
	// Parameters:
	//     pDC      - The GDI device context of the chart.
	// Remarks:
	//-----------------------------------------------------------------------
	void ExecuteOverride(CXTPChartDeviceContext* pDC);

protected:
	CXTPChartColor m_color;          //The spline color.
	XTPChartDashStyle m_nDashStyle;  //The dash style.
	CXTPChartLineDashArray m_arrDashArray;
};




#endif //#if !defined(__XTPCHARTLINEDEVICECOMMAND_H__)
