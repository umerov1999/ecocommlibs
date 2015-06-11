// XTPArrowDrawer.cpp : implementation of the CXTPArrowDrawer class.
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

#include "stdafx.h"

#define _USE_MATH_DEFINES // for M_PI
#include <math.h>

#ifndef M_PI
#define M_PI       3.14159265358979323846
#endif

#include "GraphicLibrary/GdiPlus/GdiPlus.h"

#include "Common/XTPDrawHelpers.h"
#include "Common/XTPResourceManager.h"

using namespace Gdiplus;

#include "XTPArrowDrawer.h"

// TODO: remove this dependencies
#include "XTPFlowGraphTools.h"
#include "XTPFlowGraphDrawContext.h"

CXTPArrowDrawer::CXTPArrowDrawer(float width /*= 10.f*/, float theta /*= 0.8f*/, int nNumberOfBezierNodes /*= 100*/)
	: m_Width(width), m_Theta(theta), m_nNumberOfBezierNodes(nNumberOfBezierNodes)
{

}

PointF *CXTPArrowDrawer::GetBezierCurveNodes(float p1X, float p1Y, float p2X, float p2Y, float p1cX, float p1cY, float p2cX, float p2cY, int numberOfNodes)
{
	PointF *apt = new PointF[numberOfNodes];

	float dt = 1.0f / (numberOfNodes - 1);
	float t = -dt;

	for (int i = 0; i < numberOfNodes; i++)
	{
		t += dt;

		float tt = t * t;
		float ttt = tt * t;
		float tt1 = (1 - t) * (1 - t);
		float ttt1 = tt1 * (1 - t);

		float x = ttt1 * p1X + 3 * t * tt1 * p1cX + 3 * tt * (1 - t) * p2cX + ttt * p2X;
		float y = ttt1 * p1Y + 3 * t * tt1 * p1cY + 3 * tt * (1 - t) * p2cY + ttt * p2Y;

		apt[i] = PointF(x, y);
	}

	return apt;
}

float CXTPArrowDrawer::GetDistance(float x1, float y1, float x2, float y2)
{
	return (float)sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

void CXTPArrowDrawer::Draw(CXTPFlowGraphDrawContext* pDC, PointF p1, PointF p2)
{
#define VectorF PointF

	PointF aptArrowHead[3];

	// set first node to terminal point
	aptArrowHead[0] = p2;

	VectorF vecLine (p2.X - p1.X, p2.Y - p1.Y); // build the line vector
	VectorF vecLeft (-vecLine.Y, vecLine.X); // build the arrow base vector - normal to the line

	// setup remaining arrow head points
	float lineLength = (float)sqrt(vecLine.X * vecLine.X + vecLine.Y * vecLine.Y);
	float th = m_Width / (2.0f * lineLength);
	float ta = m_Width / (2.0f * ((float)tan(m_Theta / 2.0f)) * lineLength);

	// find the base of the arrow
	PointF pBase(aptArrowHead[0].X + -ta * vecLine.X, aptArrowHead[0].Y + -ta * vecLine.Y); //base of the arrow

	// build the points on the sides of the arrow
	aptArrowHead[1] = PointF(pBase.X + th * vecLeft.X, pBase.Y + th * vecLeft.Y);
	aptArrowHead[2] = PointF(pBase.X + -th * vecLeft.X, pBase.Y + -th * vecLeft.Y);

	POINT ptArrow[3];

	for (int i=0; i<3; i++) // copy
	{
		ptArrow[i].x = (LONG)aptArrowHead[i].X;
		ptArrow[i].y = (LONG)aptArrowHead[i].Y;
	}

	pDC->FillPolygon(ptArrow, 3); // outline
}

void CXTPArrowDrawer::Draw(CXTPFlowGraphDrawContext* pDC, float p1X, float p1Y, float p2X, float p2Y, float p1cX, float p1cY, float p2cX, float p2cY)
{
	PointF* bezierLine = GetBezierCurveNodes(p1X, p1Y, p2X, p2Y, p1cX, p1cY, p2cX, p2cY, m_nNumberOfBezierNodes);

	if (bezierLine)
	{
		float arrowHeadHeight = m_Width / (2.0f * ((float)tan(m_Theta / 2.0f)));
		float distDelta = arrowHeadHeight;
		int lineTerminalNode = m_nNumberOfBezierNodes - 2;

		int i=0;

		for (i = m_nNumberOfBezierNodes - 2; i >= 1; i--)
		{
			float currDist = GetDistance(bezierLine[i].X, bezierLine[i].Y, p2X, p2Y);
			float tempDelta = (float)fabs(arrowHeadHeight - currDist);

			if (tempDelta > distDelta)
				break;

			distDelta = tempDelta;
			lineTerminalNode = i;
		}

		PointF pBase = bezierLine[lineTerminalNode]; // set arrow base node
		arrowHeadHeight = GetDistance(pBase.X, pBase.Y, p2X, p2Y);

		PointF aptArrowHead[3];
		aptArrowHead[0] = PointF(p2X, p2Y); // set first node to terminal point

		float th = m_Width / (2.0f * arrowHeadHeight); // coefficient used for remaining arrow head points setup
		// build the points on the remaining sides of the arrow
		aptArrowHead[1] = PointF(pBase.X + th * (pBase.Y - p2Y), pBase.Y + th * (p2X - pBase.X));
		aptArrowHead[2] = PointF(pBase.X + th * (p2Y - pBase.Y), pBase.Y + th * (pBase.X - p2X));

		POINT ptArrow[3];

		for (i=0; i<3; i++) // copy
		{
			ptArrow[i].x = (LONG)aptArrowHead[i].X;
			ptArrow[i].y = (LONG)aptArrowHead[i].Y;
		}

		pDC->FillPolygon(ptArrow, 3); // draw outline

		delete bezierLine;
	}
}

void CXTPArrowDrawer::Draw(CXTPFlowGraphDrawContext* pDC, PointF p1, PointF p2, PointF cp1, PointF cp2)
{
	Draw(pDC, p1.X, p1.Y, p2.X, p2.Y, cp1.X, cp1.Y, cp2.X, cp2.Y);
}
