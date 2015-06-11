// XTPControlGalleryOffice2013Theme.cpp : implementation of the CXTPControlGalleryOffice2013Theme class.
//
// This file is a part of the XTREME COMMANDBARS MFC class library.
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

#include <Common/XTPImageManager.h>
#include <Common/XTPDrawHelpers.h>
#include <Common/XTPHookManager.h>
#include <Common/XTPColorManager.h>
#include <Common/XTPSystemHelpers.h>
#include <Common/XTPWinThemeWrapper.h>
#include <Common/ScrollBar/XTPScrollBase.h>
#include <Common/ScrollBar/XTPScrollInfo.h>
#include <Common/ScrollBar/XTPScrollBarPaintManager.h>

#include <CommandBars/XTPCommandBarsDefines.h>
#include <CommandBars/XTPCommandBar.h>
#include <CommandBars/XTPPopupBar.h>
#include <CommandBars/XTPControl.h>
#include <CommandBars/XTPControlButton.h>
#include <CommandBars/XTPControlPopup.h>
#include <CommandBars/XTPControlGallery.h>
#include <CommandBars/XTPCommandBars.h>
#include <CommandBars/XTPPaintManager.h>

#include <CommandBars/Themes/XTPCommandBarsOfficeXPTheme.h>
#include <CommandBars/Themes/ModernUI/XTPCommandBarsModernUITheme.h>
#include <CommandBars/Themes/XTPCommandBarsOffice2013Theme.h>
#include <CommandBars/Themes/Office2013/XTPControlGalleryOffice2013Theme.h>


CXTPControlGalleryOffice2013Theme::CXTPControlGalleryOffice2013Theme(CXTPPaintManager *pPaintManager)
	: CXTPControlGalleryPaintManager(pPaintManager)
{
}

void CXTPControlGalleryOffice2013Theme::RefreshMetrics()
{
	m_clrBorder = RGB(171,175,178);

	m_clrScrollTriangle = RGB(119,119,119);
	m_clrScrollTriangleDisabled = RGB(198,198,198);
	m_clrScrollTriangleHot = RGB(68,68,68);
	m_clrScrollTrianglePressed = RGB(0,0,0);

	m_clrScrollBorderDisabled = RGB(225,225,225);
	m_clrScrollBorderNormal = RGB(171,171,171);
	m_clrScrollBorderHot = RGB(163,189,227);

	m_clrScrollFillHot = RGB(213,225,242);
	m_clrScrollFillPressed = RGB(163,189,227);
}

void CXTPControlGalleryOffice2013Theme::DrawScrollButton(CDC* pDC, const CRect &rc, int type, CXTPControlGallery* pGallery)
{
	XTP_SCROLLBAR_TRACKINFO *pSBTrack = pGallery->GetScrollBarTrackInfo();
	XTP_SCROLLBAR_POSINFO   *pSBInfo  = pGallery->GetScrollBarPosInfo();

	BOOL bControlEnabled = pGallery->GetEnabled();
	BOOL nPressedHt = pSBTrack ? pSBInfo->ht : -1;
	BOOL nHotHt = pSBTrack ? -1 : pSBInfo->ht;

	BOOL bEnabled = bControlEnabled;

	if (type != XTP_HTSCROLLPOPUP)
		bEnabled = (bEnabled && pGallery->IsScrollButtonEnabled(type));

	int nState = GETPARTSTATE3(type, bEnabled);

	COLORREF clrBorder = m_clrScrollBorderNormal;

	if (IS_PART_SELECTED(nState))
	{
		clrBorder = m_clrScrollBorderHot;

		CRect fillRect = rc;
		fillRect.DeflateRect(1, 1, 1, 1);
		pDC->FillSolidRect(fillRect, m_clrScrollFillHot);
	}

	if (IS_PART_PRESSED(nState))
	{
		pDC->FillSolidRect(rc, m_clrScrollFillPressed);
	}

	CXTPPenDC penDC(pDC->GetSafeHdc(), clrBorder);
	pDC->SelectObject(::GetStockObject(NULL_BRUSH));
	pDC->Rectangle(rc);

	if (!IS_PART_ENABLED(nState)) // draw disabled line
	{
		CXTPPenDC penDC(pDC->GetSafeHdc(), m_clrScrollBorderDisabled);
		pDC->MoveTo(rc.left, rc.top+1);
		pDC->LineTo(rc.left, rc.bottom-1);
	}

	COLORREF clrTriangleCurrent = m_clrScrollTriangle;

	if (IS_PART_SELECTED(nState))
		clrTriangleCurrent = m_clrScrollTriangleHot;

	if (IS_PART_PRESSED(nState))
		clrTriangleCurrent = m_clrScrollTrianglePressed;

	if (!IS_PART_ENABLED(nState))
		clrTriangleCurrent = m_clrScrollTriangleDisabled;

	CXTPPenDC triangleDC(pDC->GetSafeHdc(), clrTriangleCurrent);
	CPoint pt = rc.CenterPoint();

	switch (type)
	{
		case XTP_HTSCROLLUP:
		{
			pDC->MoveTo(pt.x, pt.y - 1);
			pDC->SetPixel(pt.x, pt.y - 1, clrTriangleCurrent);
			pDC->MoveTo(pt.x-1, pt.y);
			pDC->LineTo(pt.x+2, pt.y);
			pDC->MoveTo(pt.x-2, pt.y+1);
			pDC->LineTo(pt.x+3, pt.y+1);

			break;
		}

		case XTP_HTSCROLLPOPUP:
		{
			pDC->MoveTo(pt.x-3, pt.y - 2);
			pDC->LineTo(pt.x+4, pt.y - 2);

			pt.y += 2;
		}

		case XTP_HTSCROLLDOWN:
		{
			pDC->MoveTo(pt.x-2, pt.y - 1);
			pDC->LineTo(pt.x+3, pt.y - 1);
			pDC->MoveTo(pt.x-1, pt.y);
			pDC->LineTo(pt.x+2, pt.y);
			pDC->MoveTo(pt.x, pt.y+1);
			pDC->SetPixel(pt.x, pt.y + 1, clrTriangleCurrent);

			break;
		}
	}
}

void CXTPControlGalleryOffice2013Theme::DrawPopupScrollBar(CDC* pDC, CXTPControlGallery* pGallery)
{
	XTP_SCROLLBAR_POSINFO   *pSBInfo  = pGallery->GetScrollBarPosInfo();

	CRect rcScroll = pSBInfo->rc;
	pDC->FillSolidRect(rcScroll, m_pPaintManager->GetXtremeColor(XPCOLOR_TOOLBAR_FACE));

	CRect rcScrollUp(rcScroll.left, rcScroll.top-1, rcScroll.right+1, rcScroll.top + m_cyPopupUp);
	CRect rcScrollDown(rcScroll.left, rcScrollUp.bottom-1, rcScroll.right+1, rcScrollUp.bottom + m_cyPopupDown+1);
	CRect rcScrollPopup(rcScroll.left, rcScrollDown.bottom-1, rcScroll.right+1, rcScroll.bottom+1);

	DrawScrollButton(pDC, rcScrollUp, XTP_HTSCROLLUP, pGallery);
	DrawScrollButton(pDC, rcScrollDown, XTP_HTSCROLLDOWN, pGallery);
	DrawScrollButton(pDC, rcScrollPopup, XTP_HTSCROLLPOPUP, pGallery);
}
