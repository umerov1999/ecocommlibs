// XTPChartRectangleDeviceCommand.h
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
#if !defined(__XTPCHARTRECTANGLEDEVICECOMMAND_H__)
#define __XTPCHARTRECTANGLEDEVICECOMMAND_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

namespace Gdiplus
{
	class GpPath;
};

class _XTP_EXT_CLASS CXTPChartRectangleDeviceCommand : public CXTPChartDeviceCommand
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartRectangleDeviceCommand object.
	// Parameters:
	//     rect      - The rectangular area to be rendered by the device command.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartRectangleDeviceCommand(const CXTPChartRectF& rect);

protected:

	CXTPChartElement* HitTest(CPoint point, CXTPChartElement* pParent) const;

protected:
	CXTPChartRectF m_rect;       //The bounds of the rectangle.
};

//===========================================================================
// Summary:
//     This class represents a bounded rectangle device command,which is a kind of
//     CXTPChartDeviceCommand.It specifically handles the rendering of bounded rectangular
//     shapes in a chart.
// Remarks:
//===========================================================================
class _XTP_EXT_CLASS CXTPChartBoundedRectangleDeviceCommand : public CXTPChartRectangleDeviceCommand
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartBoundedRectangleDeviceCommand object.
	// Parameters:
	//     rect      - The rectangular area to be rendered by the device command.
	//     color     - The color of the rectangle border.
	//     thickness - The thickness of the rectangular border.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartBoundedRectangleDeviceCommand(const CXTPChartRectF& rect, const CXTPChartColor& color, int thickness);

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
	CXTPChartColor m_color;      //The color of the rectangle border.
	int m_thickness;            //The thickness of the rectangle border.
};


//===========================================================================
// Summary:
//     This class represents a solid rectangle device command,which is a kind of CXTPChartDeviceCommand.
//     It specifically handles the rendering of solid rectangular shapes in a chart like label.
// Remarks:
//===========================================================================
class _XTP_EXT_CLASS CXTPChartSolidRectangleDeviceCommand : public CXTPChartRectangleDeviceCommand
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartSolidRectangleDeviceCommand object.
	// Parameters:
	//     rect      - The rectangular area to be rendered by the device command.
	//     color     - The color of the rectangle.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartSolidRectangleDeviceCommand(const CXTPChartRectF& rect, const CXTPChartColor& color);

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
	CXTPChartColor m_color;      //The color of the rectangle.
};



//===========================================================================
// Summary:
//     This class represents a gradient rectangle device command,which is a kind of CXTPChartDeviceCommand.
//     It specifically handles the rendering of gradient rectangular shapes in a chart.
// Remarks:
//===========================================================================
class _XTP_EXT_CLASS CXTPChartGradientRectangleDeviceCommand : public CXTPChartRectangleDeviceCommand
{
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartGradientRectangleDeviceCommand object.
	// Parameters:
	//     rect      - The rectangular area to be rendered by the device command.
	//     color     - The start color of the gradient.
	//     color2    - The end color of the gradient.
	//     nMode     - The linear gradient mode, it can be horizontal, vertical,
	//                 forward diagonal or backward diagonal.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartGradientRectangleDeviceCommand(const CXTPChartRectF& rect, const CXTPChartColor& color, const CXTPChartColor& color2, XTPChartLinearGradientMode nMode);

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
	CXTPChartColor m_color2;             //The end color of the gradient.
	XTPChartLinearGradientMode m_nMode;  //The gradient mode.
};

//===========================================================================
// Summary:
//     This class represents a hatch rectangle device command,which is a kind of
//     CXTPChartDeviceCommand.It specifically handles the rendering of hatch
//     rectangular shapes in a chart.
// Remarks:
//===========================================================================
class _XTP_EXT_CLASS CXTPChartHatchRectangleDeviceCommand : public CXTPChartRectangleDeviceCommand
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartHatchRectangleDeviceCommand object.
	// Parameters:
	//     rect      - The rectangular area to be rendered by the device command.
	//     color     - The first color of the pattern.
	//     color2    - The second color of the pattern.
	//     nStyle    - chart hatch style.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartHatchRectangleDeviceCommand(const CXTPChartRectF& rect, const CXTPChartColor& color, const CXTPChartColor& color2, XTPChartHatchStyle nStyle);

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
	CXTPChartColor m_color;      // The first color.
	CXTPChartColor m_color2;     // The second color.
	XTPChartHatchStyle m_nStyle; // The hatch style selected.
};


class _XTP_EXT_CLASS CXTPChartInnerBorderDeviceCommand : public CXTPChartBoundedRectangleDeviceCommand
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartInsideBorderDeviceCommand object.
	// Parameters:
	//     rect      - The rectangular area to be rendered by the device command.
	//     color     - The color of the rectangle border.
	//     thickness - The thickness of the rectangular border.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartInnerBorderDeviceCommand(const CXTPChartRectF& rect, const CXTPChartColor& color, int thickness);
};


#ifdef _XTP_DEMOMODE

class _XTP_EXT_CLASS CXTPChartWatermarkBackgroundDeviceCommand : public CXTPChartDeviceCommand
{
public:
	CXTPChartWatermarkBackgroundDeviceCommand(const CXTPChartRectF& rect);

public:
	void ExecuteOverride(CXTPChartDeviceContext* pDC);

protected:
	CXTPChartRectF m_rect;       //The bounds of the rectangle.
};

#endif

#endif //#if !defined(__XTPCHARTRECTANGLEDEVICECOMMAND_H__)
