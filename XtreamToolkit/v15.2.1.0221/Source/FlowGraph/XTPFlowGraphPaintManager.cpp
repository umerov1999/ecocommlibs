// XTPFlowGraphPaintManager.cpp : implementation of the CXTPFlowGraphPaintManager class.
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

#include "stdafx.h"
#include <math.h>

#include "Common/XTPDrawHelpers.h"
#include "GraphicLibrary/GdiPlus/GdiPlus.h"

#include "XTPFlowGraphTools.h"
#include "XTPFlowGraphElement.h"
#include "XTPFlowGraphPaintManager.h"
#include "XTPFlowGraphDrawContext.h"
#include "XTPFlowGraphControl.h"
#include "XTPFlowGraphNode.h"
#include "XTPFlowGraphNodeGroup.h"
#include "XTPFlowGraphPage.h"
#include "XTPFlowGraphConnection.h"
#include "XTPFlowGraphConnectionPoint.h"
#include "XTPFlowGraphConnectionPoints.h"
#include "XTPFlowGraphImage.h"

using namespace Gdiplus;

CXTPFlowGraphPaintManager::CXTPFlowGraphPaintManager()
{
	m_clrBackground = RGB(255, 255, 255);
	m_clrSelection = RGB(247, 150, 70);
	m_clrTextColor = RGB(255, 255, 255);
	m_clrConnection = RGB(0, 0, 0);
	m_clrGridColor = RGB(240, 240, 240);

	m_clrNodeBackground = RGB(192, 80, 77);

	m_nNodeFrameSize = 3;
	m_nPointTextMargin = 6;
	m_bDrawArrow = TRUE;
	m_nEllipseSize = 9;
	m_bShowGrid = FALSE;

	m_bShowNodeGroupsAlways = FALSE;

	m_nConnectorType = xtpFlowGraphConnectorCurved;

	memset(&m_lfText, 0, sizeof(m_lfText));
	XTPDrawHelpers()->GetIconLogFont(&m_lfText);

	m_lfText.lfHeight = 18;
	m_lfText.lfWeight = FW_NORMAL;
	m_lfCaption = m_lfText;

	m_dMinZoomLevelBackground = 0.2;
	m_dMinZoomLevelConnectionPoints = 0.3;
	m_dMinZoomLevelConnections = 0.05;
	m_dMinZoomLevelGDIQuality = 0.4;

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

void CXTPFlowGraphPaintManager::DrawControlBackground(CXTPFlowGraphDrawContext* pDC, CXTPFlowGraphControl* pControl)
{
	pDC->Clear(m_clrBackground);

	if (m_bShowGrid && !pDC->IsPrinting() && pControl->GetZoomLevel() >= m_dMinZoomLevelBackground)
	{
		CRect rc;
		pControl->GetClientRect(&rc);

		pDC->DrawGrid(rc, pControl->GetScrollOffset(), pControl->GetZoomLevel(), m_clrGridColor);
	}

}

void CXTPFlowGraphPaintManager::RecalcNodeLayout(CXTPFlowGraphDrawContext* pDC, CXTPFlowGraphNode* pNode)
{
	pNode->m_rcWindow.SetRectEmpty();

	CString strCaption = pNode->GetCaption();

	pDC->SetFont(&m_lfCaption);
	CSize szCaption;

	if (pNode->GetMarkupUIElement())
		szCaption = pDC->MeasureMarkupElement(pNode->GetMarkupUIElement());
	else
		szCaption = pDC->MeasureString(strCaption);

	szCaption.cy += 2;

	CXTPFlowGraphImage* pImage = pNode->GetImage();
	if (pImage)
	{
		szCaption.cx += 2 + pImage->GetSize().cx + 3;
	}

	CSize szSize = szCaption;

	pDC->SetFont(&m_lfText);

	int i;
	CXTPFlowGraphConnectionPoints* pPoints = pNode->GetConnectionPoints();

	for (i = 0; i < pPoints->GetCount(); i++)
	{
		CXTPFlowGraphConnectionPoint* pPoint = pPoints->GetAt(i);
		CSize szItem = pPoint->GetMarkupUIElement() ?
			pDC->MeasureMarkupElement(pPoint->GetMarkupUIElement()) :
			pDC->MeasureString(pPoint->GetCaption());
		szItem.cy += 2;

		if (pPoint->GetImage())
			szItem.cx += pPoint->GetImage()->GetSize().cx + 2;

		pPoint->m_rcPoint.SetRect(0, 0, szItem.cx, szItem.cy);


		szSize.cy += szItem.cy;
		szSize.cx = max(szSize.cx, szItem.cx);
	}

	int nTop = 2 * m_nNodeFrameSize + szCaption.cy;
	szSize.cx += 10 + 2 * m_nPointTextMargin;

	CSize sz = pNode->GetWindowSize();
	if (sz != CSize(0, 0))
	{
		szSize.cy += sz.cy + m_nNodeFrameSize;

		szSize.cx = max(szSize.cx, sz.cx + 2 * m_nNodeFrameSize);

		pNode->m_rcWindow.SetRect(m_nNodeFrameSize, nTop, szSize.cx - m_nNodeFrameSize, nTop + sz.cy);

		nTop += sz.cy + m_nNodeFrameSize;
	}

	szSize.cx = max(szSize.cx, pNode->GetUserSize().cx);

	pNode->m_rcCaption.SetRect(m_nNodeFrameSize, m_nNodeFrameSize, szSize.cx - m_nNodeFrameSize, m_nNodeFrameSize + szCaption.cy);

	if (pPoints->GetCount())
		szSize.cy += m_nNodeFrameSize;

	for (i = 0; i < pPoints->GetCount(); i++)
	{
		CXTPFlowGraphConnectionPoint* pPoint = pPoints->GetAt(i);
		pPoint->m_rcPoint.SetRect(m_nNodeFrameSize, nTop, szSize.cx - m_nNodeFrameSize, nTop + pPoint->m_rcPoint.Height());

		nTop += pPoint->m_rcPoint.Height();
	}


	szSize.cy += 2 * m_nNodeFrameSize;

	szSize.cy = max(szSize.cy, pNode->GetUserSize().cy);

	pNode->m_szActualSize = szSize;
}

void CXTPFlowGraphPaintManager::RecalcConnectionLayout(CXTPFlowGraphDrawContext* /*pDC*/, CXTPFlowGraphConnection* pConnection)
{
	CXTPFlowGraphConnectionPoint* pFrom = pConnection->GetOutputPoint();
	CXTPFlowGraphConnectionPoint* pTo = pConnection->GetInputPoint();

	if (pFrom == NULL)
		return;

	CPoint ptFrom, ptTo;

	ptFrom = pFrom->GetNode()->GetLocation();
	ptFrom.x += pFrom->GetNode()->GetSize().cx;
	ptFrom.y += pFrom->m_rcPoint.CenterPoint().y;

	if (pTo != NULL)
	{
		ptTo = pTo->GetNode()->GetLocation();
		ptTo.y += pTo->m_rcPoint.CenterPoint().y;

		if (pTo->GetType() == xtpFlowGraphPointInputAndOutput)
		{
			if (ptFrom.x > ptTo.x)
				ptTo.x += pTo->GetNode()->GetSize().cx;
		}

		pConnection->m_ptInputPoint = ptTo;
	}
	else
	{
		ptTo = pConnection->m_ptInputPoint;
	}

	if (pFrom->GetType() == xtpFlowGraphPointInputAndOutput)
	{
		if (ptTo.x < ptFrom.x - pFrom->GetNode()->GetSize().cx / 2)
			ptFrom.x -= pFrom->GetNode()->GetSize().cx;
	}


	pConnection->m_ptOutputPoint = ptFrom;

	GraphicsPath* path = new GraphicsPath();

	XTPFlowGraphConnectorType type = pConnection->GetStyle() == -1 ? m_nConnectorType : (XTPFlowGraphConnectorType)pConnection->GetStyle();

	CPoint ptControlPoint = pConnection->GetControlPoint();

	if (type == xtpFlowGraphConnectorStraight)
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
	else
	{
		if (ptControlPoint != CPoint(-1, -1))
		{
			/*PointF pt[3] = {PointF((REAL)ptFrom.x, (REAL)ptFrom.y), PointF((REAL)pConnection->GetControlPoint().x, (REAL)pConnection->GetControlPoint().y),
			PointF((REAL)ptTo.x, (REAL)ptTo.y) };
			path->AddCurve(pt, 3);*/

			PointF pt2[4] = {PointF((REAL)ptFrom.x, (REAL)ptFrom.y),
				PointF((REAL)ptControlPoint.x, (REAL)ptFrom.y), PointF((REAL)ptControlPoint.x,  (REAL)ptTo.y),
				PointF((REAL)ptTo.x, (REAL)ptTo.y) };

			path->AddBeziers(pt2, 4);

		}
		else
		{
			PointF pt[4] = {PointF((REAL)ptFrom.x, (REAL)ptFrom.y), PointF((REAL)(ptFrom.x + ptTo.x) / 2, (REAL)ptFrom.y),
				PointF(REAL(ptFrom.x + ptTo.x) / 2, (REAL)ptTo.y), PointF((REAL)ptTo.x, (REAL)ptTo.y) };
			path->AddBeziers(pt, 4);
		}
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

void CXTPFlowGraphPaintManager::DrawNode(CXTPFlowGraphDrawContext* pDC, CXTPFlowGraphNode* pNode)
{
	CRect rcNodeBounds = GetNodeBoundingRectangle(pNode);
	CRect rcBounds = pDC->GetClipRect();
	CXTPFlowGraphControl* pControl = pNode->GetControl();

	if (rcBounds.right < rcNodeBounds.left || rcBounds.left > rcNodeBounds.right || rcBounds.top > rcNodeBounds.bottom || rcBounds.bottom < rcNodeBounds.top)
		return;

	CRect rc(pNode->GetLocation(), pNode->GetSize());

	COLORREF clrNode = pNode->GetColor();
	if (clrNode == -1) clrNode = m_clrNodeBackground;
	COLORREF clrSelection = m_clrSelection;
	if (pNode->IsSelected())
		clrNode = clrSelection;

	COLORREF clrFrame = GetDarkColor(clrNode);

	pDC->FillSolidRect(rc, clrNode);

	pDC->DrawFrame(rc.left, rc.top, rc.Width(), rc.Height(), m_nNodeFrameSize, clrFrame);

	CString strCaption = pNode->GetCaption();

	pDC->SetFont(&m_lfCaption);

	int nCaptionHeight = pNode->m_rcCaption.Height();

	pDC->SetTextColor(m_clrTextColor);

	CRect rcCaption(rc.left, rc.top + m_nNodeFrameSize, rc.right, rc.top + nCaptionHeight + m_nNodeFrameSize);
	UINT uCaptionFormat = DT_CENTER | DT_VCENTER;

	CXTPFlowGraphImage* pImage = pNode->GetImage();
	if (pImage)
	{
		CSize sz = pImage->GetSize();

		pDC->DrawImage(pImage, CRect(CPoint(rc.left + m_nNodeFrameSize + 3, rc.top + m_nNodeFrameSize + (nCaptionHeight - sz.cy) / 2), sz));

		rcCaption = CRect(rc.left + m_nNodeFrameSize + 3 + sz.cx + 2, rc.top + m_nNodeFrameSize, rc.right, rc.top + nCaptionHeight + m_nNodeFrameSize);
		uCaptionFormat = DT_LEFT | DT_VCENTER;
	}

	if (pNode->GetMarkupUIElement())
	{
		pDC->DrawMarkupElement(pNode->GetMarkupUIElement(), rcCaption, uCaptionFormat);
	}
	else
	{
		pDC->DrawText(pNode->GetCaption(), rcCaption, uCaptionFormat);
	}

	int nCount = pNode->GetConnectionPoints()->GetCount();

	if (nCount > 0 || !pNode->m_rcWindow.IsRectEmpty())
	{
		pDC->FillSolidRect(rc.left, rc.top + nCaptionHeight + m_nNodeFrameSize, rc.Width(), m_nNodeFrameSize, clrFrame);
	}

	if (nCount > 0 && !pNode->m_rcWindow.IsRectEmpty())
	{
		pDC->FillSolidRect(rc.left, rc.top + pNode->m_rcWindow.bottom, rc.Width(), m_nNodeFrameSize, clrFrame);
	}

	if (!pNode->m_rcWindow.IsRectEmpty() && pNode->GetWindowHandle() == NULL)
	{
		pNode->DrawWindowContent(pDC);
	}

	pDC->SetFont(&m_lfText);

	for (int i = 0; i < nCount; i++)
	{
		CXTPFlowGraphConnectionPoint* pPoint = pNode->GetConnectionPoints()->GetAt(i);
		CSize szItem = pPoint->m_rcPoint.Size();

		int nTop = pPoint->m_rcPoint.top + rc.top;

		if (pControl->GetZoomLevel() >= m_dMinZoomLevelConnectionPoints)
		{
			pDC->SetTextColor(m_clrTextColor);

			CRect rcText(rc.left + m_nNodeFrameSize + m_nPointTextMargin, nTop, rc.right - (m_nNodeFrameSize + m_nPointTextMargin), nTop + szItem.cy);

			if (pPoint->GetImage())
			{
				CSize sz = pPoint->GetImage()->GetSize();

				pDC->DrawImage(pPoint->GetImage(), CRect(CPoint(rcText.left, (rcText.top + rcText.bottom  - sz.cy) / 2), sz));

				rcText.left += sz.cx + 2;

			}

			UINT uFormat = DT_VCENTER | (pPoint->GetType() == xtpFlowGraphPointOutput ? DT_RIGHT : DT_LEFT);

			if (pPoint->GetMarkupUIElement())
				pDC->DrawMarkupElement(pPoint->GetMarkupUIElement(), rcText, uFormat);
			else
				pDC->DrawText(pPoint->GetCaption(), rcText, uFormat);
		}

		COLORREF clrEllipse = pPoint->GetColor();
		if (clrEllipse == (COLORREF)-1)
			clrEllipse = clrFrame;

		if (pPoint->GetType() & xtpFlowGraphPointInput)
		{
			pDC->SetBrush(pPoint->IsSelected() ? clrSelection : clrEllipse);
			pDC->SetPen(0, 0);

			CPoint pt(rc.left + 1, nTop + szItem.cy / 2);


			pDC->Ellipse(CRect(pt.x - m_nEllipseSize / 2, pt.y - m_nEllipseSize / 2, pt.x + m_nEllipseSize - m_nEllipseSize / 2, pt.y + m_nEllipseSize - m_nEllipseSize / 2));
		}

		if (pPoint->GetType() & xtpFlowGraphPointOutput)
		{
			pDC->SetBrush(pPoint->IsSelected() ? clrSelection : clrEllipse);
			pDC->SetPen(0, 0);

			CPoint pt(rc.right - 1, nTop + szItem.cy / 2);

			pDC->Ellipse(CRect(pt.x - (m_nEllipseSize - m_nEllipseSize / 2), pt.y - (m_nEllipseSize - m_nEllipseSize / 2), pt.x + m_nEllipseSize / 2, pt.y + m_nEllipseSize / 2));
		}
	}
}

REAL XTPGetDistance(PointF& a, PointF& b)
{
	return (REAL)sqrt((a.X - b.X) * (a.X - b.X) + (a.Y - b.Y) * (a.Y - b.Y));
}

PointF XTPGetPosition(GraphicsPath* path, REAL dist)
{
	int pointCount = path->GetPointCount();

	PointF* points = (PointF*)malloc(pointCount * sizeof(PointF));
	path->GetPathPoints(points, pointCount);

	PointF res(points[pointCount - 1]);

	if (dist >= 0)
	{
		for (int i = 0; i < pointCount - 1; i++)
		{
			REAL diff = (REAL)XTPGetDistance(points[i + 1], points[i]);
			if (diff < dist)
			{
				dist -= diff;
				continue;
			}

			res.X =  points[i].X + (points[i + 1].X - points[i].X) * dist / diff;
			res.Y =  points[i].Y + (points[i + 1].Y - points[i].Y) * dist / diff;
			break;
		}
	}
	else
	{
		dist = -dist;
		res = points[0];

		for (int i = pointCount - 1; i >= 1; i--)
		{
			REAL diff = (REAL)XTPGetDistance(points[i - 1], points[i]);
			if (diff < dist)
			{
				dist -= diff;
				continue;
			}

			res.X =  points[i].X + (points[i - 1].X - points[i].X) * dist / diff;
			res.Y =  points[i].Y + (points[i - 1].Y - points[i].Y) * dist / diff;
			break;
		}

	}

	free(points);

	return res;

}

void CXTPFlowGraphPaintManager::DrawConnection(CXTPFlowGraphDrawContext* pDC, CXTPFlowGraphConnection* pConnection)
{
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

	if (m_bDrawArrow)
	{
		Gdiplus::GraphicsPath* path = pConnection->m_pPath->Clone();
		path->Flatten();

		const REAL rArrow = 10.0;
		PointF ptEnd = XTPGetPosition(path, REAL(-m_nEllipseSize / 2));
		PointF ptFrom = XTPGetPosition(path, REAL(- (m_nEllipseSize / 2 + rArrow)));

		Point ptArrow[3];

		PointF ptDiff = ptFrom - ptEnd;

		ptArrow[0] = Point((int)ptEnd.X, (int)ptEnd.Y);
		ptArrow[1] = Point(int(ptFrom.X - ptDiff.Y * (4.0) / rArrow), int(ptFrom.Y + ptDiff.X * (4.0) / rArrow));
		ptArrow[2] = Point(int(ptFrom.X + ptDiff.Y * (4.0) / rArrow), int(ptFrom.Y - ptDiff.X * (4.0) / rArrow));

		pDC->SetBrush(clrConnection);
		pDC->FillPolygon((POINT*)ptArrow, 3);

		delete path;
	}
}

CRect CXTPFlowGraphPaintManager::GetNodeBoundingRectangle(CXTPFlowGraphNode* pNode)
{
	CRect rc(pNode->GetLocation(), pNode->GetSize());
	rc.InflateRect(m_nEllipseSize / 2, 0);

	return rc;
}

BOOL CXTPFlowGraphPaintManager::HitTestConnectionArea(const CXTPFlowGraphConnectionPoint* pPoint, CPoint point)
{
	CXTPFlowGraphNode* pNode = pPoint->GetNode();
	if (!pNode)
		return FALSE;

	CXTPFlowGraphPage* pPage = pNode->GetPage();
	point = pPage->ScreenToPage(point);
	point.Offset(-pNode->GetLocation());


	CRect rc = pPoint->m_rcPoint;

	if (pPoint->GetType() & xtpFlowGraphPointInput)
	{
		CRect rc(rc.left - m_nEllipseSize / 2, rc.top, rc.left + m_nEllipseSize / 2, rc.bottom);

		if (rc.PtInRect(point))
			return TRUE;
	}

	if (pPoint->GetType() & xtpFlowGraphPointOutput)
	{
		CRect rc(rc.right - m_nEllipseSize / 2, rc.top, rc.right + m_nEllipseSize / 2, rc.bottom);
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
		rc = pNode->m_rcCaption;
		rc.OffsetRect(pNode->GetLocation());
	}
	else if(pElement->IsKindOf(RUNTIME_CLASS(CXTPFlowGraphConnectionPoint)))
	{
		CXTPFlowGraphConnectionPoint* pConnectionPoint = (CXTPFlowGraphConnectionPoint*)pElement;

		CXTPFlowGraphNode* pNode = pConnectionPoint->GetNode();

		rc = pConnectionPoint->m_rcPoint;
		rc.DeflateRect(m_nPointTextMargin, 0, m_nPointTextMargin, 1);
		rc.OffsetRect(pNode->GetLocation());

	}
	else
	{
		ASSERT(FALSE);
	}

	return rc;
}

void CXTPFlowGraphPaintManager::DrawNodeGroup(CXTPFlowGraphDrawContext* pDC, CXTPFlowGraphNodeGroup* pNodeGroup)
{
	CRect rc = pNodeGroup->GetWorkRect();
	rc.InflateRect(m_nEllipseSize / 2, 10);

	BOOL bSelected = pNodeGroup->GetAt(0)->IsSelected();

	COLORREF clrGroup = bSelected ? m_clrSelection : RGB(220, 220, 220);


	COLORREF clrSelection = clrGroup | 0x55000000;
	pDC->FillSolidRect(rc, clrSelection);

	COLORREF clrFrame = GetDarkColor(clrGroup);

	pDC->DrawFrame(rc.left, rc.top, rc.Width(), rc.Height(), 1, clrFrame);
}




