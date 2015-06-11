// XTPFlowGraphPaintManager.cpp : implementation of the CXTPFlowGraphPaintManager class.
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

#include "StdAfx.h"

#include "GraphicLibrary/GdiPlus/GdiPlus.h"
using namespace Gdiplus;

#include "Common/XTPDrawHelpers.h"
#include <Common/ScrollBar/XTPScrollBase.h>
#include <Common/ScrollBar/XTPScrollBarContainer.h>

#include "XTPFlowGraphTools.h"
#include "XTPFlowGraphElement.h"
#include "XTPFlowGraphPaintManager.h"
#include "XTPFlowGraphDrawContext.h"
#include "XTPFlowGraphDrawContextGdiPlus.h"
#include "XTPFlowGraphControl.h"
#include "XTPFlowGraphNode.h"
#include "XTPFlowGraphNodeGroup.h"
#include "XTPFlowGraphPage.h"
#include "XTPFlowGraphConnection.h"
#include "XTPFlowGraphConnectionPoint.h"
#include "XTPFlowGraphConnectionPoints.h"
#include "XTPFlowGraphImage.h"

#include "XTPTextOnPath.h"
#include "XTPArrowDrawer.h"

#include <math.h>

using namespace Gdiplus;

CXTPFlowGraphPaintManager::CXTPFlowGraphPaintManager()
{
	m_clrBackground = RGB(255, 255, 255);
	m_clrGridColor = RGB(240, 240, 240);

	// from the metallic theme

	m_clrTextColor = RGB(0, 0, 0);
	m_clrSelection = RGB(147, 148, 198);
	m_clrSelectionInactive = RGB(220, 220, 220);
	m_clrTextColorSelected  = RGB(255,255,255);
	m_clrConnection = RGB(255, 0, 0);

	m_clrNodeBackground = RGB(172, 172, 172);

	m_nNodeFrameSize = 2;
	m_nPointTextMargin = 12;
	m_nEllipseSize = 12;
	m_bShowGrid = TRUE;

	m_bShowNodeGroupsAlways = FALSE;
	m_bAlwaysDrawConnectionPoints = FALSE; // TRUE for old FG behavior
	m_bRecalcConnectionLayoutWhenNeeded = TRUE;

	m_bChangeNodeColorWhenSelected = TRUE; // old FG behavior

	m_nConnectorType = xtpFlowGraphConnectorCurved;

	memset(&m_lfText, 0, sizeof(m_lfText));
	XTPDrawHelpers()->GetIconLogFont(&m_lfText);

	m_lfText.lfHeight = 18;
	m_lfText.lfWeight = FW_NORMAL;
	m_lfCaption = m_lfText;

	//m_lfCaption.lfWeight = FW_BOLD;

	m_dMinZoomLevelBackground = 0.2;
	m_dMinZoomLevelConnectionPoints = 0.3;
	m_dMinZoomLevelConnections = 0.05;
	m_dMinZoomLevelGDIQuality = 0.4;

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPFlowGraphPaintManager::~CXTPFlowGraphPaintManager()
{
}

void CXTPFlowGraphPaintManager::Cleanup()
{
}

void CXTPFlowGraphPaintManager::RefreshMetrics()
{
}

void CXTPFlowGraphPaintManager::DrawControlBackground(CXTPFlowGraphDrawContext* pDC, CXTPFlowGraphControl* pControl, BOOL bPrintToFile /*= FALSE*/, const CRect &drawRect /*= CRect()*/)
{
	pDC->Clear(m_clrBackground);

	if (m_bShowGrid && !pDC->IsPrinting() && pControl->GetZoomLevel() >= m_dMinZoomLevelBackground)
	{
		CRect rc = drawRect;

		if (drawRect.IsRectEmpty())
			pControl->GetClientRect(&rc);

		pDC->DrawGrid(rc, !bPrintToFile ? pControl->GetScrollOffset() : CPoint(0,0), !bPrintToFile ? pControl->GetZoomLevel() : 1.0, m_clrGridColor);
	}
}

// p1 and p2 - two points of intersection
bool IntersectLineWithCircle(PointF linep1, PointF linep2, PointF circleCenter, double circleR, PointF &p1, PointF &p2)
{
	double R = circleR; // circle radius

	double k = (linep1.Y - linep2.Y);

	if (!k) // otherwise devision by zero
	{
		return false;
	}

	double s = (linep2.X - linep1.X);
	double q = (linep1.X*linep2.Y - linep2.X * linep1.Y);

	// equation line with 2 points defined
	// k*x + s*y + q = 0
	// then
	// x = -(s*y + q)/k

	double px = circleCenter.X;
	double py = circleCenter.Y;

	// equation of the circle
	// (x-px) ^ 2 + (y-py) ^ 2 = R^2
	// or
	// x^2 - 2*px*x + px ^ 2 + y^2 - 2*py*y + py ^ 2 - R^2 = 0
	// or
	// x^2 - 2*px*x + y^2 - 2*py*y + (px^2 + py^2 - R^2) = 0
	// let's take new variable 'o' for convenience

	double o = pow(px,2) + pow(py,2) - pow(R,2);

	// then the equation will be
	// x^2 - 2*px*x + y^2 - 2*py*y + o = 0

	// substitu x = -s/k*y - q/k
	// do some substitution of variables
	double u = -s/k;
	double v = -q/k;
	// then õ = u*y + v, à
	// õ^2 = u^2*y^2 + 2*u*v*y + v^2

	// do substitution with removal of parentheses
	// u^2*y^2 + 2*u*v*y + v^2 - 2*px*u*y - 2*px*v + y^2 - 2*py*y + o = 0
	// do reduction of similar

	// (u^2+1)Y^2 + (2*u*v-2*px*u -2*py)(Y) + v^2 - 2*px*v + o = 0

	// do some substitution of variables

	double a = pow(u,2)+1;
	double b = 2*u*v - 2*px*u - 2*py;
	double c = pow(v,2) - 2*px*v + o;

	// we get
	// a*y^2 + b*y + c = 0
	// solve this quadratic equation

	double D = pow(b,2) - 4*a*c;

	ASSERT(D >= 0); // Discriminant <0

	if (D >= 0)
	{
		double y1 = (-b + sqrt(D))/(2*a);
		double y2 = (-b - sqrt(D))/(2*a);

		double x1 = -(s*y1 + q)/k;
		double x2 = -(s*y2 + q)/k;

		p1.X = (REAL)x1;
		p1.Y = (REAL)y1;

		p2.X = (REAL)x2;
		p2.Y = (REAL)y2;

		return true;
	}

	return false;
}

double CalcDistance(PointF p1, PointF p2)
{
	return sqrt( pow((p2.X - p1.X),2) + pow((p2.Y - p1.Y),2) );
}

bool GetCirclesSmartConnectionPoints(PointF point1, PointF point2, CPoint &ptFrom, CPoint &ptTo, double R)
{
	PointF p1, p2;
	bool bRes1 = IntersectLineWithCircle(point1, point2, point1, R, p1, p2);

	PointF p3, p4;
	bool bRes2 = IntersectLineWithCircle(point1, point2, point2, R, p3, p4);

	if (!bRes1 || !bRes2)
		return false;

	PointF pr1, pr2;

	double d1 = CalcDistance(p1, p3);
	double d2 = CalcDistance(p1, p4);
	double d3 = CalcDistance(p2, p3);
	double d4 = CalcDistance(p2, p4);

	double min = 100000;

	if (d1<min) min = d1;
	if (d2<min) min = d2;
	if (d3<min) min = d3;
	if (d4<min) min = d4;

	if (d1==min) { pr1 = p1; pr2 = p3; }
	if (d2==min) { pr1 = p1; pr2 = p4; }
	if (d3==min) { pr1 = p2; pr2 = p3; }
	if (d4==min) { pr1 = p2; pr2 = p4; }

	ptFrom.x = (LONG)pr1.X;
	ptFrom.y = (LONG)pr1.Y;

	ptTo.x = (LONG)pr2.X;
	ptTo.y = (LONG)pr2.Y;

	return true;
}

PointF GetRectCenterOffset(CRect rc)
{
	PointF pt;
	pt.X = (REAL)rc.TopLeft().x + rc.Width()/2;
	pt.Y = (REAL)rc.TopLeft().y + rc.Height()/2;

	return pt;
}

#define POINTS_PROXIMITY_THRESHOLD  5

BOOL ArePointsApproxOnTheSameLine(CPoint p1, CPoint p2)
{
	return (abs(p1.x - p2.x) < POINTS_PROXIMITY_THRESHOLD || abs(p1.y - p2.y)<POINTS_PROXIMITY_THRESHOLD);
}

BOOL ArePointsApproxOnTheSameLine(PointF p1, PointF p2)
{
	return (fabs(p1.X - p2.X) < POINTS_PROXIMITY_THRESHOLD || fabs(p1.Y - p2.Y)<POINTS_PROXIMITY_THRESHOLD);
}

BOOL CXTPFlowGraphPaintManager::CalculateConnectionPointsPositions(CXTPFlowGraphConnection* pConnection, CPoint &p1, CPoint &p2)
{
	CXTPFlowGraphConnectionPoint* pFrom = pConnection->GetOutputPoint();
	CXTPFlowGraphConnectionPoint* pTo = pConnection->GetInputPoint();

	if (pFrom == NULL)
		return FALSE;

	XTPFlowGraphConnectorType type = pConnection->GetStyle() != -1 ? (XTPFlowGraphConnectorType)pConnection->GetStyle() : m_nConnectorType;

	CRect rc1 = pFrom->GetRect();
	rc1.OffsetRect(pFrom->GetNode()->GetLocation());

	CRect rc2 = pTo ? pTo->GetRect() : CRect(0,0,0,0); // this happens when a connection is dragged

	if (pTo)
		rc2.OffsetRect(pTo->GetNode()->GetLocation());

	// point1,point2 - centers of circles
	PointF point1 = GetRectCenterOffset(rc1);
	PointF point2 = PointF(0,0);

	if (!rc2.IsRectEmpty())
	{
		point2 = GetRectCenterOffset(rc2);
	}
	else
	{
		// this happens when a connection is dragged

		if (pConnection->m_ptInputPoint != CPoint(-1,-1)) // not initialized
		{
			point2 = PointF((REAL)pConnection->m_ptInputPoint.x, (REAL)pConnection->m_ptInputPoint.y);
		}
		else
		{
			return FALSE;
		}
	}

	if (m_bAlwaysDrawConnectionPoints == FALSE && m_bRecalcConnectionLayoutWhenNeeded == FALSE)
	{
		// just take centers

		p1.x = (LONG)point1.X;
		p1.y = (LONG)point1.Y;

		p2.x = (LONG)point2.X;
		p2.y = (LONG)point2.Y;
	}
	else
	{
		if (IsElbowConnection(type) || IsStraightConnection(type))
		{
			// we should use new algorithm of calculating points
			if (!GetCirclesSmartConnectionPoints(point1, point2, p1, p2, m_nEllipseSize/2))
			{
				// take centers

				p1.x = (LONG)point1.X;
				p1.y = (LONG)point1.Y;

				p2.x = (LONG)point2.X;
				p2.y = (LONG)point2.Y;
			}
		}
		else
		{
			p1.x = (LONG)point1.X;
			p1.y = (LONG)point1.Y;

			p2.x = (LONG)point2.X;
			p2.y = (LONG)point2.Y;

			// we should take circle end points

			if (p1.x < p2.x)
			{
				p1.x += m_nEllipseSize/2;
				p2.x -= m_nEllipseSize/2;
			}
			else
			{
				p2.x += m_nEllipseSize/2;
				p1.x -= m_nEllipseSize/2;
			}
		}

		if (m_bAlwaysDrawConnectionPoints == FALSE)
		{
			// we should check if the node is not selected

			if (!pFrom->GetNode()->IsSelected())
			{
				// it will be center

				p1.x = (LONG)point1.X;
				p1.y = (LONG)point1.Y;
			}

			if (pTo && !pTo->GetNode()->IsSelected())
			{
				// it will be center

				p2.x = (LONG)point2.X;
				p2.y = (LONG)point2.Y;
			}
		}
	}

	return TRUE;
}

void CXTPFlowGraphPaintManager::RecalcConnectionLayout(CXTPFlowGraphDrawContext* /*pDC*/, CXTPFlowGraphConnection* pConnection)
{
	CPoint ptFrom, ptTo;
	if (!CalculateConnectionPointsPositions(pConnection, ptFrom, ptTo))
		return;

	pConnection->SetPathReversed(FALSE);

	XTPFlowGraphConnectorType type = pConnection->GetStyle() != -1 ? (XTPFlowGraphConnectorType)pConnection->GetStyle() : m_nConnectorType;

	pConnection->m_ptOutputPoint = ptFrom;
	pConnection->m_ptInputPoint = ptTo;

	GraphicsPath* path = new GraphicsPath();

	CPoint ptControlPoint = pConnection->GetControlPoint();

	bool bForceUseStraight = false;

	if (IsElbowConnection(type) && ArePointsApproxOnTheSameLine(ptFrom, ptTo))
		bForceUseStraight = true;

	if (IsStraightConnection(type) || bForceUseStraight)
	{
		if (ptControlPoint != CPoint(-1, -1))
		{
			Point pts[] = {Point(ptFrom.x, ptFrom.y), Point(ptControlPoint.x, ptControlPoint.y), Point(ptTo.x, ptTo.y) };

			path->AddLines(pts, 3);
		}
		else
		{
			path->AddLine(ptFrom.x, ptFrom.y, ptTo.x, ptTo.y);
		}
	}
	else if (IsCurvedConnection(type))
	{
		if (ptControlPoint != CPoint(-1, -1))
		{
			PointF pt2[4] = {PointF((REAL)ptFrom.x, (REAL)ptFrom.y), PointF((REAL)ptControlPoint.x, (REAL)ptFrom.y),
				PointF((REAL)ptControlPoint.x,  (REAL)ptTo.y), PointF((REAL)ptTo.x, (REAL)ptTo.y) };

			path->AddBeziers(pt2, 4);

		}
		else
		{
			PointF pt[4] = {PointF((REAL)ptFrom.x, (REAL)ptFrom.y), PointF((REAL)(ptFrom.x + ptTo.x) / 2, (REAL)ptFrom.y),
				PointF(REAL(ptFrom.x + ptTo.x) / 2, (REAL)ptTo.y), PointF((REAL)ptTo.x, (REAL)ptTo.y) };

			path->AddBeziers(pt, 4);
		}
	}
	else // elbow
	{
		// the simplest elbow is formed by a rectangle where line p1,p2 is its diagonal
		// there're 2 elbows

		CPoint p1 = ptFrom;
		CPoint p2 = ptTo;

		CPoint controlPoint1, controlPoint2;

		bool bPointsSwap = false;

		if (p1.x > p2.x)
		{
			CPoint p3 = p2;
			p2 = p1;
			p1 = p3;

			bPointsSwap = true;

			pConnection->SetPathReversed(TRUE);
		}

		if (bPointsSwap && p1.y > p2.y)
		{
			controlPoint1.x = p1.x;
			controlPoint1.y = p1.y - (p1.y-p2.y)/2;

			controlPoint2.x = p2.x;
			controlPoint2.y = controlPoint1.y;
		}
		else
		{
			controlPoint1.x = p1.x + (p2.x - p1.x)/2;
			controlPoint1.y = p1.y;

			controlPoint2.x = controlPoint1.x;
			controlPoint2.y = p2.y;
		}

		/*if (0) // old way
		{
			const int min_offset_x = 20;

			int offset_x = (p2.x-p1.x)/2;

			if ( (p1.x > p2.x) || (offset_x < min_offset_x) )
				offset_x = min_offset_x;

			int x = p1.x+offset_x;

			controlPoint1 = CPoint(x, p1.y);
			controlPoint2 = CPoint(x, p2.y);
		}*/

		path->AddLine(p1.x, p1.y, controlPoint1.x, controlPoint1.y);
		path->AddLine(controlPoint1.x, controlPoint1.y, controlPoint2.x, controlPoint2.y);
		path->AddLine(controlPoint2.x, controlPoint2.y, p2.x, p2.y);

	}

	SAFE_DELETE(pConnection->m_pPath);
	pConnection->m_pPath = path;

}

COLORREF CXTPFlowGraphPaintManager::GetDarkColor(COLORREF clrBackground)
{
	DWORD dwHSLBackground = CXTPDrawHelpers::RGBtoHSL(clrBackground);
	DWORD dwL = GetBValue(dwHSLBackground);

	DWORD dwDark = MulDiv(dwL, 2, 3);
	return CXTPDrawHelpers::HSLtoRGB(RGB(GetRValue(dwHSLBackground), GetGValue(dwHSLBackground), dwDark));
}

void CXTPFlowGraphPaintManager::DrawConnection(CXTPFlowGraphDrawContext* pDC, CXTPFlowGraphConnection* pConnection)
{
	// if at least one node is not visible - skip drawing the connection

	CXTPFlowGraphNode *pNodeFrom = pConnection->GetInputNode();
	CXTPFlowGraphNode *pNodeTo = pConnection->GetOutputNode();

	if (pNodeTo && !pNodeTo->IsVisible())
		return;

	if (pNodeFrom && !pNodeFrom->IsVisible())
		return;

	CRect rcConnectionBounds = pConnection->GetBoundingRect();
	CRect rcBounds = pDC->GetClipRect();
	CXTPFlowGraphControl* pControl = pConnection->GetControl();

	if (rcBounds.right < rcConnectionBounds.left || rcBounds.left > rcConnectionBounds.right ||
		rcBounds.top > rcConnectionBounds.bottom || rcBounds.bottom < rcConnectionBounds.top)
		return;

	if (pControl->GetZoomLevel() < m_dMinZoomLevelConnections)
		return;

	COLORREF clrConnection = pConnection->GetColor() != (COLORREF)-1 ? pConnection->GetColor() : m_clrConnection;
	if (pConnection->IsSelected())
		clrConnection = m_clrSelection;

	pDC->SetPen(clrConnection, 2);
	pDC->DrawCurve(pConnection->m_pPath);

	if (pConnection->IsSelected() && pConnection->GetControlPoint() != CPoint(-1, -1))
	{
		pDC->SetPen(0, 0);
		pDC->SetBrush(clrConnection);

		CPoint pt(pConnection->GetControlPoint());
		pDC->Ellipse(CRect(pt.x - 3, pt.y - 3, pt.x + 3, pt.y + 3));
	}

	XTPFlowGraphConnectorType type = pConnection->GetStyle() != -1 ? (XTPFlowGraphConnectorType)pConnection->GetStyle() : m_nConnectorType;

	if (HasEndingArrow(type))
	{
		if ( pConnection->m_pPath)
		{
			Gdiplus::GraphicsPath* path = pConnection->m_pPath->Clone();

			CXTPArrowDrawer arrowDrawer;

			if (IsStraightConnection(type) || IsElbowConnection(type))
				path->Flatten();

			int pointCount = path->GetPointCount();
			PointF* points = (PointF*)malloc(pointCount * sizeof(PointF));
			path->GetPathPoints(points, pointCount);

			bool bForceUseStraight = false;

			if (pointCount == 2 && IsElbowConnection(type) && ArePointsApproxOnTheSameLine(points[0], points[1]))
				bForceUseStraight = true;

			pDC->SetBrush(clrConnection);

			if (IsStraightConnection(type) || bForceUseStraight)
			{
				PointF startingPoint = points[0];
				PointF endingPoint = points[1];

				if (pointCount == 3) // there's a control point
				{
					startingPoint = points[1];
					endingPoint = points[2];
				}

				arrowDrawer.Draw(pDC, startingPoint, endingPoint);

				if (HasStartingArrow(type))
				{
					if (pointCount == 3) // there's a control point
					{
						startingPoint = points[0];
						endingPoint = points[1];
					}

					arrowDrawer.Draw(pDC, endingPoint, startingPoint);
				}
			}
			else if (IsElbowConnection(type) )
			{
				ASSERT(pointCount == 4);

				if (pointCount == 4)
				{
					arrowDrawer.Draw(pDC, points[2], points[3] );

					if (HasStartingArrow(type))
						arrowDrawer.Draw(pDC, points[1], points[0]);
				}
			}
			else if (IsCurvedConnection(type))
			{
				ASSERT(pointCount == 4);

				arrowDrawer.Draw(pDC, points[0], points[3], points[1], points[2]);

				if (HasStartingArrow(type))
					arrowDrawer.Draw(pDC, points[3], points[0], points[2], points[1]);
			}

			free(points);

			delete path;
		}
	}

	if (pConnection->GetCaption().GetLength() && pDC->GetGraphics())
	{
		CXTPTextOnPath textOnPath(pConnection->GetCaption(), pDC->GetGraphics(), pConnection->m_pPath);

		if (pConnection->m_ptOutputPoint.x > pConnection->m_ptInputPoint.x && !pConnection->IsPathReversed())
		{
			textOnPath.ReversePath();
		}

		textOnPath.Draw();
	}
}

CRect CXTPFlowGraphPaintManager::GetNodeBoundingRectangle(CXTPFlowGraphNode* pNode)
{
	return pNode->GetBoundingRectangle();
}

BOOL CXTPFlowGraphPaintManager::HitTestConnectionArea(const CXTPFlowGraphConnectionPoint* pPoint, CPoint point)
{
	CXTPFlowGraphNode* pNode = pPoint->GetNode();
	if (!pNode)
		return FALSE;

	CXTPFlowGraphPage* pPage = pNode->GetPage();
	point = pPage->ScreenToPage(point);
	point.Offset(-pNode->GetLocation());

	CRect rc = pPoint->GetRect();

	if (pPoint->GetType() == xtpFlowGraphPointInput || pPoint->GetType() == xtpFlowGraphPointOutput ||
		pPoint->GetType() == xtpFlowGraphPointBoth)
	{
		if (rc.PtInRect(point))
			return TRUE;
	}

	return FALSE;
}

CRect CXTPFlowGraphPaintManager::GetCaptionRect(CXTPFlowGraphElement* pElement)
{
	CRect rc(0, 0, 0, 0);

	if (pElement->IsKindOf(RUNTIME_CLASS(CXTPFlowGraphNode)))
	{
		CXTPFlowGraphNode* pNode = (CXTPFlowGraphNode*)pElement;

		rc = pNode->GetCaptionRect();
	}
	else if(pElement->IsKindOf(RUNTIME_CLASS(CXTPFlowGraphConnectionPoint)))
	{
		CXTPFlowGraphConnectionPoint* pConnectionPoint = (CXTPFlowGraphConnectionPoint*)pElement;

		CXTPFlowGraphNode* pNode = pConnectionPoint->GetNode();

		rc = pNode->GetTableRowCaptionRect(pConnectionPoint);
	}
	else
	{
		ASSERT(FALSE);
	}

	return rc;
}

void CXTPFlowGraphPaintManager::DrawTransparentRectangle(CXTPFlowGraphDrawContext* pDC, CRect rect, COLORREF clrColor)
{
	COLORREF clrSelection = clrColor | 0x55000000;
	pDC->FillSolidRect(rect, clrSelection);

	COLORREF clrFrame = CXTPFlowGraphPaintManager::GetDarkColor(clrColor);

	pDC->DrawFrame(rect.left, rect.top, rect.Width(), rect.Height(), 1, clrFrame);
}

void CXTPFlowGraphPaintManager::DrawNodeGroup(CXTPFlowGraphDrawContext* pDC, CXTPFlowGraphNodeGroup* pNodeGroup)
{
	CRect rc = pNodeGroup->GetWorkRect();
	rc.InflateRect(m_nEllipseSize/2, m_nEllipseSize/2); // for connection points ellipses

	BOOL bSelected = pNodeGroup->GetAt(0)->IsSelected();

	COLORREF clrGroup = bSelected ? m_clrSelection : m_clrSelectionInactive;

	DrawTransparentRectangle(pDC, rc, clrGroup);
}


#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPFlowGraphPaintManager, CCmdTarget)
	DISP_PROPERTY_ID(CXTPFlowGraphPaintManager, "BackColor", 1, m_clrBackground, VT_COLOR)
	DISP_PROPERTY_ID(CXTPFlowGraphPaintManager, "ForeColor", 2, m_clrTextColor, VT_COLOR)
	DISP_PROPERTY_ID(CXTPFlowGraphPaintManager, "SelectionColor", 3, m_clrSelection, VT_COLOR)
	DISP_PROPERTY_EX_ID(CXTPFlowGraphPaintManager, "Font", DISPID_FONT, OleGetFont, OleSetFont, VT_DISPATCH)

	DISP_PROPERTY_ID(CXTPFlowGraphPaintManager, "ShowGrid", 4, m_bShowGrid, VT_BOOL)

	DISP_PROPERTY_ID(CXTPFlowGraphPaintManager, "ConnectorType", 5, m_nConnectorType, VT_I4)


	DISP_PROPERTY_ID(CXTPFlowGraphPaintManager, "NodeBackgroundColor", 6, m_clrNodeBackground, VT_COLOR)
	DISP_PROPERTY_EX_ID(CXTPFlowGraphPaintManager, "CaptionFont", 7, OleGetCaptionFont, OleSetCaptionFont, VT_DISPATCH)


	DISP_PROPERTY_ID(CXTPFlowGraphPaintManager, "NodeFrameSize", 8, m_nNodeFrameSize, VT_I4)
	DISP_PROPERTY_ID(CXTPFlowGraphPaintManager, "PointTextMargin", 9, m_nPointTextMargin, VT_I4)
	DISP_PROPERTY_ID(CXTPFlowGraphPaintManager, "ConnectionColor", 10, m_clrConnection, VT_COLOR)
	DISP_PROPERTY_ID(CXTPFlowGraphPaintManager, "EllipseSize", 12, m_nEllipseSize, VT_I4)

	DISP_PROPERTY_ID(CXTPFlowGraphPaintManager, "ShowNodeGroupsAlways", 13, m_bShowNodeGroupsAlways, VT_BOOL)

	DISP_PROPERTY_ID(CXTPFlowGraphPaintManager, "GridColor", 14, m_clrGridColor, VT_COLOR)

	DISP_PROPERTY_ID(CXTPFlowGraphPaintManager, "ShowConnectionPointsAlways", 15, m_bAlwaysDrawConnectionPoints, VT_BOOL)
	DISP_PROPERTY_ID(CXTPFlowGraphPaintManager, "RecalculateConnectionsLayoutWhenNeeded", 16, m_bRecalcConnectionLayoutWhenNeeded, VT_BOOL)

	DISP_PROPERTY_ID(CXTPFlowGraphPaintManager, "ChangeNodeColorWhenSelected", 17, m_bChangeNodeColorWhenSelected, VT_BOOL)

END_DISPATCH_MAP()

// {20353C2D-B341-4329-9D52-97B0BF66B90E}
static const GUID IID_IFlowGraphPaintManager =
{ 0x20353c2d, 0xb341, 0x4329, { 0x9d, 0x52, 0x97, 0xb0, 0xbf, 0x66, 0xb9, 0xe } };


BEGIN_INTERFACE_MAP(CXTPFlowGraphPaintManager, CCmdTarget)
	INTERFACE_PART(CXTPFlowGraphPaintManager, IID_IFlowGraphPaintManager, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPFlowGraphPaintManager, IID_IFlowGraphPaintManager)


void CXTPFlowGraphPaintManager::OleSetFont(LPFONTDISP pFontDisp)
{
	ASSERT((pFontDisp == NULL) ||
		AfxIsValidAddress(pFontDisp, sizeof(IDispatch), FALSE));

	LOGFONT lf;
	if (AxGetLogFontFromDispatch(&lf, pFontDisp))
	{
		m_lfText = lf;
	}
	else
	{

	}
	RefreshMetrics();
}

LPFONTDISP CXTPFlowGraphPaintManager::OleGetFont()
{
	return AxCreateOleFont(m_lfText, this, (LPFNFONTCHANGED)&CXTPFlowGraphPaintManager::OleSetFont);
}

void CXTPFlowGraphPaintManager::OleSetCaptionFont(LPFONTDISP pFontDisp)
{
	ASSERT((pFontDisp == NULL) ||
		AfxIsValidAddress(pFontDisp, sizeof(IDispatch), FALSE));

	LOGFONT lf;
	if (AxGetLogFontFromDispatch(&lf, pFontDisp))
	{
		m_lfCaption = lf;
	}
	else
	{

	}
	RefreshMetrics();
}

LPFONTDISP CXTPFlowGraphPaintManager::OleGetCaptionFont()
{
	return AxCreateOleFont(m_lfCaption, this, (LPFNFONTCHANGED)&CXTPFlowGraphPaintManager::OleSetCaptionFont);
}




#endif
