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

#include "XTPFlowGraphElement.h"
#include "XTPFlowGraphTools.h"
#include "XTPFlowGraphPaintManager.h"
#include "XTPFlowGraphMetallicTheme.h"
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

CXTPFlowGraphMetallicTheme::CXTPFlowGraphMetallicTheme()
{
	m_clrTextColor = RGB(0, 0, 0);
	m_clrSelection = RGB(147, 148, 198);
	m_clrConnection = RGB(255, 0, 0);

	m_clrNodeBackground = RGB(172, 172, 172);

	m_nNodeFrameSize = 2;
	m_nPointTextMargin = 12;
	m_nEllipseSize = 14;
	m_bDrawArrow = FALSE;

	m_bShowGrid = TRUE;

	m_lfCaption.lfWeight = FW_BOLD;

}

CXTPFlowGraphMetallicTheme::~CXTPFlowGraphMetallicTheme()
{

}

void CXTPFlowGraphMetallicTheme::Cleanup()
{
	CXTPFlowGraphPaintManager::Cleanup();
}

void CXTPFlowGraphMetallicTheme::RefreshMetrics()
{
	CXTPFlowGraphPaintManager::RefreshMetrics();
}



void CXTPFlowGraphMetallicTheme::DrawNode(CXTPFlowGraphDrawContext* pDC, CXTPFlowGraphNode* pNode)
{
	CRect rcNodeBounds = GetNodeBoundingRectangle(pNode);
	CRect rcBounds = pDC->GetClipRect();
	CXTPFlowGraphControl* pControl = pNode->GetControl();

	if (rcBounds.right < rcNodeBounds.left || rcBounds.left > rcNodeBounds.right || rcBounds.top > rcNodeBounds.bottom || rcBounds.bottom < rcNodeBounds.top)
		return;

	CRect rc(pNode->GetLocation(), pNode->GetSize());

	COLORREF clrNode = pNode->GetColor();
	if (clrNode == (COLORREF)-1)
		clrNode = m_clrNodeBackground;


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

	pDC->SetPen(clrFrame, 1);
	pDC->SetFont(&m_lfText);

	for (int i = 0; i < nCount; i++)
	{
		CXTPFlowGraphConnectionPoint* pPoint = pNode->GetConnectionPoints()->GetAt(i);
		CSize szItem = pPoint->m_rcPoint.Size();

		int nTop = pPoint->m_rcPoint.top + rc.top;

		pDC->GradientFill(CRect(rc.left + m_nNodeFrameSize, nTop, rc.right - m_nNodeFrameSize, nTop + szItem.cy), RGB(226, 226, 226), RGB(208, 208, 208), FALSE);
		if (i != nCount - 1) pDC->FillSolidRect(rc.left, nTop + szItem.cy - 1, rc.Width(), 1, clrFrame);

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
			clrEllipse = RGB(182, 182, 182);

		if (pPoint->GetType() & xtpFlowGraphPointInput)
		{
			pDC->SetBrush(pPoint->IsSelected() ? clrSelection : pPoint->GetConnectionsCount() > 0 ? RGB(250, 252, 58) : clrEllipse);

			CPoint pt(rc.left + 1, nTop + szItem.cy / 2);

			pDC->Ellipse(CRect(pt.x - m_nEllipseSize / 2, pt.y - m_nEllipseSize / 2, pt.x + m_nEllipseSize - m_nEllipseSize / 2, pt.y + m_nEllipseSize - m_nEllipseSize / 2));
		}

		if (pPoint->GetType() & xtpFlowGraphPointOutput)
		{
			pDC->SetBrush(pPoint->IsSelected() ? clrSelection : pPoint->GetConnectionsCount() > 0 ? RGB(250, 252, 58) : clrEllipse);

			CPoint pt(rc.right - 1, nTop + szItem.cy / 2);

			pDC->Ellipse(CRect(pt.x - (m_nEllipseSize - m_nEllipseSize / 2), pt.y - (m_nEllipseSize - m_nEllipseSize / 2), pt.x + m_nEllipseSize / 2, pt.y + m_nEllipseSize / 2));
		}
	}

	if (nCount)
	{
		int nTop = pNode->GetConnectionPoints()->GetAt(nCount - 1)->m_rcPoint.bottom + rc.top;
		int nBottom = rc.bottom - m_nNodeFrameSize;

		if (nTop + 1 < nBottom)
		{
			pDC->GradientFill(CRect(rc.left + m_nNodeFrameSize, nTop, rc.right - m_nNodeFrameSize, nBottom), RGB(226, 226, 226), RGB(208, 208, 208), FALSE);
			pDC->FillSolidRect(rc.left, nTop, rc.Width(), 1, clrFrame);
		}
	}
}
