// XTPChartCircleDeviceCommand.h
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
#if !defined(__XTPCHARTCIRCLEDEVICECOMMAND_H__)
#define __XTPCHARTCIRCLEDEVICECOMMAND_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//===========================================================================
// Summary:
//     This class is the base class for a all circle device commands.
//===========================================================================
class _XTP_EXT_CLASS CXTPChartCircleDeviceCommand : public CXTPChartDeviceCommand
{
protected:
	CXTPChartCircleDeviceCommand(const CXTPChartPointF& center, double radius);

public:
	CXTPChartElement* HitTest(CPoint point, CXTPChartElement* pParent) const;

protected:
	CXTPChartPointF m_center;            //The center of the circle.
	double m_radius;                    //The radius of the circle.
};

//===========================================================================
// Summary:
//     This class represents a gradient circle device command,which is a kind of CXTPChartDeviceCommand.
//     It specifically handles the rendering of gradient circular shapes in a chart.
// Remarks:
//===========================================================================
class _XTP_EXT_CLASS CXTPChartSolidCircleDeviceCommand : public CXTPChartCircleDeviceCommand
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartSolidCircleDeviceCommand object.
	// Parameters:
	//     center    - The center point of the circle.
	//     radius    - The radius of the circle.
	//     color     - The color of the circle.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartSolidCircleDeviceCommand(const CXTPChartPointF& center, double radius, const CXTPChartColor& color);

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
	CXTPChartColor m_color;              //The start color.
};

//===========================================================================
// Summary:
//     This class represents a gradient circle device command,which is a kind of CXTPChartDeviceCommand.
//     It specifically handles the rendering of gradient circular shapes in a chart.
// Remarks:
//===========================================================================
class _XTP_EXT_CLASS CXTPChartGradientCircleDeviceCommand : public CXTPChartCircleDeviceCommand
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartGradientCircleDeviceCommand object.
	// Parameters:
	//     center    - The center point of the circle.
	//     radius    - The radius of the circle.
	//     color     - The start color of the gradient.
	//     color2    - The end color of the gradient.
	//     nMode     - The linear gradient mode, it can be horizontal, vertical,
	//                 forward diagonal or backward diagonal.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartGradientCircleDeviceCommand(const CXTPChartPointF& center, double radius, const CXTPChartColor& color, const CXTPChartColor& color2, XTPChartLinearGradientMode nMode);

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
	CXTPChartColor m_color;              //The start color.
	CXTPChartColor m_color2;             //The end color.
	XTPChartLinearGradientMode m_nMode;  //The gradient mode used.
};


//===========================================================================
// Summary:
//     This class represents a gradient hatch device command,which is a kind of CXTPChartCircleDeviceCommand.
//     It specifically handles the rendering of hatch circular shapes in a chart.
// Remarks:
//===========================================================================
class _XTP_EXT_CLASS CXTPChartHatchCircleDeviceCommand : public CXTPChartCircleDeviceCommand
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartHatchCircleDeviceCommand object.
	// Parameters:
	//     center    - The center point of the circle.
	//     radius    - The radius of the circle.
	//     color     - The start color of the gradient.
	//     color2    - The end color of the gradient.
	//     nStyle    - Chart hatch style.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartHatchCircleDeviceCommand(const CXTPChartPointF& center, double radius, const CXTPChartColor& color, const CXTPChartColor& color2, XTPChartHatchStyle nStyle);

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
	CXTPChartColor m_color;              //The start color.
	CXTPChartColor m_color2;             //The end color.
	XTPChartHatchStyle m_nStyle;         //The hatch mode used.
};


//===========================================================================
// Summary:
//     This class represents a bounded circle device command,which is a kind of CXTPChartDeviceCommand.
//     It specifically handles the rendering of circular shapes in a chart.
// Remarks:
//===========================================================================
class _XTP_EXT_CLASS CXTPChartBoundedCircleDeviceCommand : public CXTPChartDeviceCommand
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartBoundedCircleDeviceCommand object.
	// Parameters:
	//     center    - The center point of the circle.
	//     radius    - The radius of the circle.
	//     color     - The color of boundary.
	//     thickness - The thickness of the boundary.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartBoundedCircleDeviceCommand(const CXTPChartPointF& center, double radius, const CXTPChartColor& color, int thickness);

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
	CXTPChartColor m_color;          //The color of the cirular boundary.
	int m_thickness;                //The thickness of the circular boundary.
	CXTPChartPointF m_center;        //The center point of the circle.
	double m_radius;                //The radius of the circle.
};

#endif //#if !defined(__XTPCHARTCIRCLEDEVICECOMMAND_H__)
