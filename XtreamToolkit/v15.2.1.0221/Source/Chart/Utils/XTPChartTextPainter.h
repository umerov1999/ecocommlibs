// XTPChartTextPainter.h
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
#if !defined(__XTPCHARTROTATEDTEXTPAINTE_H__)
#define __XTPCHARTROTATEDTEXTPAINTE_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CXTPChartDeviceContext;
class CXTPChartDeviceCommand;
class CXTPChartFont;
class CXTPMarkupUIElement;

//===========================================================================
// Summary:
//     This enumeration defines the various text rotations used in the XTP chart.
// Remarks:
//===========================================================================
enum XTPChartTextRotation
{
	xtpChartTextLeftTop,     //Left top.
	xtpChartTextCenterTop,   //Center top.
	xtpChartTextRightTop,    //Right top.
	xtpChartTextLeftCenter,  //Left center.
	xtpChartTextCenterCenter,//Center center.
	xtpChartTextRightCenter, //Right center.
	xtpChartTextLeftBottom,  //Left bottom.
	xtpChartTextCenterBottom,//Center bottom.
	xtpChartTextRightBottom  //Right bottom.
};

class CXTPChartTextElement;

//===========================================================================
// Summary:
//     This abstract class represents a text painter which is used for drawing
//     various text elements in the chart.
// Remarks:
//===========================================================================
class _XTP_EXT_CLASS CXTPChartTextPainterBase
{
protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartTextPainterBase object.
	// Parameters:
	//     text          - The text to be rendered.
	//     size          - The measurements required to render the text.
	//     pTextProvider - The element which owns the text.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartTextPainterBase(CXTPChartDeviceContext* pDC, const CXTPChartString& text, CXTPChartTextElement* pTextProvider);

public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPChartTextPainterBase object, handle cleanup.
	//-------------------------------------------------------------------------
	virtual ~CXTPChartTextPainterBase();

public:

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to get the rounded bounds of the text.
	// Returns:
	//     A CRect object specifying the rounded bounds.
	//-------------------------------------------------------------------------
	CRect GetRoundedBounds();

	CXTPChartSizeF GetSize() const;

protected:
	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to calculate the bounds of the text.
	// Returns:
	//     A CXTPChartRectF object specifying the rounded bounds.
	// Remarks:
	//     This is a pure virtual function, so give meaningful implementation
	//     to this function in derived classes.
	//-------------------------------------------------------------------------
	virtual CXTPChartRectF CalculateBounds() = 0;

protected:
	CXTPChartString m_strText;               //The text to be painted
	CXTPChartSizeF m_szTextSize;             //The measurements of the text.
	CXTPChartTextElement* m_pTextProvider;   //The chart element which owns the text.
	int m_nHeight;                          //The height of the text.
	int m_nWidth;                           //The width of the text.

	CRect m_rcRoundedBounds;                //The rounded bounds of the text.
	CXTPChartRectF m_rcBounds;               //The bounds of the text.
	CXTPMarkupUIElement* m_pMarkupUIElement;
};

//===========================================================================
// Summary:
//     This class represents a text painter which is used for drawing various
//     text elements in the chart.This is a kind of CXTPChartTextPainterBase.
// Remarks:
//===========================================================================
class _XTP_EXT_CLASS CXTPChartTextPainter : public CXTPChartTextPainterBase
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartTextPainterBase object.
	// Parameters:
	//     text          - The text to be rendered.
	//     size          - The measurements required to render the text.
	//     pTextProvider - The element which owns the text.
	//     location      - The location of the text.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartTextPainter(CXTPChartDeviceContext* pDC, const CXTPChartString& text, CXTPChartTextElement* pTextProvider);

	void SetLocation(const CXTPChartPointF& location);

public:

	//-------------------------------------------------------------------------
	// Summary:
	//     This function create a CXTPChartDeviceCommand object, this object
	//     represents the rendering of a text.
	// Parameters:
	//     pDC     - Pointer to a CXTPChartDeviceContext object.
	//     color   - The color of the text.
	// Returns:
	//     Returns CXTPChartDeviceCommand object, this object handles
	//     the rendering of an element in the chart.Here it handles
	//     the drawing of a text.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	CXTPChartDeviceCommand* CreateDeviceCommand(CXTPChartDeviceContext* pDC, const CXTPChartColor& color);

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to calculate the bounds of the text.
	// Returns:
	//     A CXTPChartRectF object specifying the bounds.
	// Remarks:
	//-------------------------------------------------------------------------
	CXTPChartRectF CalculateBounds();

protected:
	CXTPChartPointF m_ptLocation;        //The location of the text.
};

//===========================================================================
// Summary:
//     This abstract base class represents a rotated text painter which is used
//     for drawing various rotated text elements in the chart.This is a kind
//     of CXTPChartTextPainterBase.
// Remarks:
//===========================================================================
class _XTP_EXT_CLASS CXTPChartRotatedTextPainterBase : public CXTPChartTextPainterBase
{
protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartRotatedTextPainterBase object.
	// Parameters:
	//     text          - The text to be rendered.
	//     size          - The measurements required to render the text.
	//     pTextProvider - The element which owns the text.
	//     ptBase        - The base point of the text.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartRotatedTextPainterBase(CXTPChartDeviceContext* pDC, const CXTPChartString& text, CXTPChartTextElement* pTextProvider, CPoint ptBase = CPoint(0, 0));

public:
	//-------------------------------------------------------------------------
	// Summary:
	//     This function create a CXTPChartDeviceCommand object, this object
	//     represents the rendering of a rotated text.
	// Parameters:
	//     pDC     - Pointer to a CXTPChartDeviceContext object.
	// Returns:
	//     Returns CXTPChartDeviceCommand object, this object handles
	//     the rendering of an element in the chart.Here it handles
	//     the drawing of a rotated text.
	// Remarks:
	// See Also:
	//-------------------------------------------------------------------------
	CXTPChartDeviceCommand* CreateDeviceCommand(CXTPChartDeviceContext* pDC, const CXTPChartColor& color);

public:
	void SetBasePoint(CPoint pt);

protected:

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to calculate the bounds of the text.
	// Returns:
	//     A CXTPChartRectF object specifying the bounds.
	// Remarks:
	//-------------------------------------------------------------------------
	CXTPChartRectF CalculateBounds();

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to calculate the bounds of the text.
	// Returns:
	//     A CXTPChartRectF object specifying the bounds.
	// Remarks:
	//-------------------------------------------------------------------------
	CRect GetInitialTextRect();


	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to calculate the left top point of the text.
	// Returns:
	//     A CXTPChartRectF object specifying the bounds.
	// Remarks:
	//     This is a pure virtual function, so give meaningful implementation
	//     to this function in derived classes.
	//-------------------------------------------------------------------------
	virtual CPoint CalculateLeftTopPoint() = 0;

	//-------------------------------------------------------------------------
	// Summary:
	//     Use this function to calculate the rotation of the text.
	// Returns:
	//     An enumerated value, XTPChartTextRotation specifying the text rotation type.
	// Remarks:
	//     This is a pure virtual function, so give meaningful implementation
	//     to this function in derived classes.
	//-------------------------------------------------------------------------
	virtual XTPChartTextRotation CalculateRotation() = 0;

	void CalculatePoints(XTPChartTextRotation rotation, CRect rect, float angle, float points[4][2]);

protected:
	CPoint m_ptBasePoint;   //The base point of the text.
	float m_fAngle;         //The angle of text rotation.
};

//===========================================================================
// Summary:
//     This abstract base class renders a rotated text near a line which is used
//     for drawing various rotated text elements near lines in the chart.This
//     is a kind of CXTPChartTextPainterBase.
// Remarks:
//===========================================================================
class _XTP_EXT_CLASS CXTPChartRotatedTextPainterNearLine : public CXTPChartRotatedTextPainterBase
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPChartRotatedTextPainterNearLine object.
	// Parameters:
	//     text          - The text to be rendered.
	//     size          - The measurements required to render the text.
	//     pTextProvider - The element which owns the text.
	//     ptBase        - The base point of the text.
	//     position      - An enumerated value specifying the near text position.
	//     fAngle        - The angle of rotation.
	// Remarks:
	//-----------------------------------------------------------------------
	CXTPChartRotatedTextPainterNearLine(CXTPChartDeviceContext* pDC, const CXTPChartString& text, CXTPChartTextElement* pTextProvider, CPoint ptBase, XTPChartNearTextPosition position, float fAngle);

public:


protected:
	//-------------------------------------------------------------------------
	// Summary:
	//     Use this function to calculate the left top point of the text.
	// Returns:
	//     A CPoint object specifying the left top.
	// Remarks:
	//-------------------------------------------------------------------------
	CPoint CalculateLeftTopPoint();

	//-------------------------------------------------------------------------
	// Summary:
	//     Use this function to calculate the rotation of the text.
	// Returns:
	//     An enumerated value, XTPChartTextRotation specifying the text rotation type.
	//-------------------------------------------------------------------------
	XTPChartTextRotation CalculateRotation();

protected:
	XTPChartNearTextPosition m_nNearPosition;  //The near text position.
};

AFX_INLINE CXTPChartSizeF CXTPChartTextPainterBase::GetSize() const {
	return m_szTextSize;
}

#endif //#if !defined(__XTPCHARTROTATEDTEXTPAINTE_H__)
