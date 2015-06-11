// XTPArrowDrawer.h: interface for the CXTPArrowDrawer class.
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
#if !defined(__XTPARROWDRAWER_H__)
#define __XTPARROWDRAWER_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////////
// description
//////////////////////////////////////////////////////////////////////////

class CXTPFlowGraphDrawContext;

class CXTPArrowDrawer
{

protected:

	Gdiplus::PointF *GetBezierCurveNodes(float p1X, float p1Y, float p2X, float p2Y, float p1cX, float p1cY, float p2cX, float p2cY, int numberOfNodes);

	float GetDistance(float x1, float y1, float x2, float y2);

public:

	CXTPArrowDrawer(float width = 10.f, float theta = 0.8f, int nNumberOfBezierNodes = 100);

	// draw a straight arrow when a line has starting point p1 and end point p2
	void Draw(CXTPFlowGraphDrawContext* pDC, Gdiplus::PointF p1, Gdiplus::PointF p2);

	// draw an arrow when a line is a bezier curve (with 2 control points)
	void Draw(CXTPFlowGraphDrawContext* pDC, float p1X, float p1Y, float p2X, float p2Y, float p1cX, float p1cY, float p2cX, float p2cY);

	// draw an arrow when a line is a bezier curve (with 2 control points)
	void Draw(CXTPFlowGraphDrawContext* pDC, Gdiplus::PointF p1, Gdiplus::PointF p2, Gdiplus::PointF cp1, Gdiplus::PointF cp2);

protected:

	float m_Width; // width (in pixels) of the full base of the arrowhead
	float m_Theta; // angle (in radians) at the arrow tip between the two sides of the arrowhead

	int m_nNumberOfBezierNodes; // number of nodes used to calculate the Bezier curve
};

#endif // __XTPARROWDRAWER_H__
