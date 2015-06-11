// XTPControlGallery.cpp : implementation file.
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

#include "stdafx.h"
#include <math.h>

#include <Common/XTPImageManager.h>
#include <Common/XTPPropExchange.h>
#include <Common/XTPToolTipContext.h>
#include <Common/XTPSystemHelpers.h>
#include <Common/XTPHookManager.h>
#include <Common/XTPColorManager.h>
#include <Common/XTPDrawHelpers.h>
#include <Common/XTPMarkupRender.h>
#include <Common/ScrollBar/XTPScrollBase.h>
#include <Common/ScrollBar/XTPScrollInfo.h>
#include <Common/ScrollBar/XTPScrollBarPaintManager.h>

#include <CommandBars/XTPCommandBarsDefines.h>
#include <CommandBars/XTPCommandBar.h>
#include <CommandBars/XTPPopupBar.h>
#include <CommandBars/XTPControl.h>
#include <CommandBars/XTPControlButton.h>
#include <CommandBars/XTPControlPopup.h>
#include <CommandBars/XTPControlEdit.h>
#include <CommandBars/XTPControlComboBox.h>

#include <CommandBars/XTPControls.h>
#include <CommandBars/XTPControlGallery.h>
#include <CommandBars/XTPControlComboBoxGalleryPopupBar.h>
#include <CommandBars/XTPPaintManager.h>
#include <CommandBars/XTPCommandBars.h>

#include <CommandBars/Gallery/XTPControlGalleryItem.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//////////////////////////////////////////////////////////////////////////
// CXTPControlGalleryPaintManager

CXTPControlGalleryPaintManager::CXTPControlGalleryPaintManager(CXTPPaintManager* pPaintManager)
	: m_pPaintManager(pPaintManager)
{
	m_cyPopupUp = m_cyPopupDown = 19;
	m_cxPopup = 16;
}

void CXTPControlGalleryPaintManager::DrawLabel(CDC* pDC, CXTPControlGalleryItem* pLabel, CRect rcItem)
{
	CXTPPaintManager* pPaintManager = m_pPaintManager;
	pDC->FillSolidRect(rcItem, pPaintManager->GetXtremeColor(XPCOLOR_LABEL));

	pDC->FillSolidRect(rcItem.left, rcItem.bottom - 1, rcItem.Width(), 1, pPaintManager->GetXtremeColor(XPCOLOR_3DSHADOW));

	CXTPFontDC fnt(pDC, pPaintManager->GetRegularBoldFont());

	CRect rcText(rcItem);
	rcText.DeflateRect(10, 0);
	pDC->SetTextColor(pPaintManager->GetXtremeColor(XPCOLOR_MENUBAR_TEXT));
	pDC->DrawText(pLabel->GetCaption(), rcText, DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS);
}

void CXTPControlGalleryPaintManager::FillControl(CDC* pDC, CXTPControlGallery* pGallery, CRect rcControl)
{
	if (!pGallery->IsTransparent())
	{
		pDC->FillSolidRect(rcControl, m_pPaintManager->GetXtremeColor(XPCOLOR_MENUBAR_FACE));
	}

	if (pGallery->IsShowBorders())
	{
		pDC->Draw3dRect(rcControl, m_clrBorder, m_clrBorder);
	}

	if (pGallery->HasBottomSeparator())
	{
		pDC->FillSolidRect(rcControl.left, rcControl.bottom - 2, rcControl.Width(), 1, m_pPaintManager->GetXtremeColor(XPCOLOR_SEPARATOR));
	}
}

void CXTPControlGalleryPaintManager::RefreshMetrics()
{
	m_cyPopupUp = m_cyPopupDown = 19;
	m_cxPopup = 16;

	m_clrBorder = m_pPaintManager->GetXtremeColor(XPCOLOR_3DSHADOW);
}

void CXTPControlGalleryPaintManager::DrawPopupScrollBar(CDC* pDC, CXTPControlGallery* pGallery)
{
	XTP_SCROLLBAR_TRACKINFO *pSBTrack = pGallery->GetScrollBarTrackInfo();
	XTP_SCROLLBAR_POSINFO   *pSBInfo  = pGallery->GetScrollBarPosInfo();

	CRect rcScroll = pSBInfo->rc;
	pDC->FillSolidRect(rcScroll, m_pPaintManager->GetXtremeColor(XPCOLOR_TOOLBAR_FACE));

	CRect rcScrollUp(rcScroll.left, rcScroll.top, rcScroll.right, rcScroll.top + m_cyPopupUp);
	CRect rcScrollDown(rcScroll.left, rcScrollUp.bottom, rcScroll.right, rcScrollUp.bottom + m_cyPopupDown);
	CRect rcScrollPopup(rcScroll.left, rcScrollDown.bottom, rcScroll.right, rcScroll.bottom);

	BOOL bControlEnabled = pGallery->GetEnabled();
	BOOL nPressedHt = pSBTrack ? pSBInfo->ht : -1;
	BOOL nHotHt = pSBTrack ? -1 : pSBInfo->ht;

	int nState = GETPARTSTATE3(XTP_HTSCROLLUP, (bControlEnabled && pGallery->IsScrollButtonEnabled(XTP_HTSCROLLUP)));
	m_pPaintManager->DrawRectangle(pDC, rcScrollUp, IS_PART_SELECTED(nState) || IS_PART_PRESSED(nState), IS_PART_PRESSED(nState), IS_PART_ENABLED(nState), FALSE, FALSE, xtpBarTypeNormal, xtpBarPopup);
	COLORREF clr = m_pPaintManager->GetRectangleTextColor(IS_PART_SELECTED(nState), IS_PART_PRESSED(nState), IS_PART_ENABLED(nState), FALSE, FALSE, xtpBarTypeNormal, xtpBarPopup);
	CPoint pt = rcScrollUp.CenterPoint();
	m_pPaintManager->Triangle(pDC, CPoint(pt.x - 4 , pt.y + 2), CPoint(pt.x + 4, pt.y + 2), CPoint (pt.x, pt.y - 2), clr);

	nState = GETPARTSTATE3(XTP_HTSCROLLDOWN, (bControlEnabled && pGallery->IsScrollButtonEnabled(XTP_HTSCROLLDOWN)));
	m_pPaintManager->DrawRectangle(pDC, rcScrollDown, IS_PART_SELECTED(nState) || IS_PART_PRESSED(nState), IS_PART_PRESSED(nState), IS_PART_ENABLED(nState), FALSE, FALSE, xtpBarTypeNormal, xtpBarPopup);
	clr = m_pPaintManager->GetRectangleTextColor(IS_PART_SELECTED(nState), IS_PART_PRESSED(nState), IS_PART_ENABLED(nState), FALSE, FALSE, xtpBarTypeNormal, xtpBarPopup);
	pt = rcScrollDown.CenterPoint();
	m_pPaintManager->Triangle(pDC, CPoint(pt.x - 4 , pt.y - 2), CPoint(pt.x + 4, pt.y - 2), CPoint (pt.x, pt.y + 2), clr);

	nState = GETPARTSTATE3(XTP_HTSCROLLPOPUP, bControlEnabled);
	m_pPaintManager->DrawRectangle(pDC, rcScrollPopup, IS_PART_SELECTED(nState) || IS_PART_PRESSED(nState), IS_PART_PRESSED(nState), IS_PART_ENABLED(nState), FALSE, FALSE, xtpBarTypeNormal, xtpBarPopup);
	clr = m_pPaintManager->GetRectangleTextColor(IS_PART_SELECTED(nState), IS_PART_PRESSED(nState), IS_PART_ENABLED(nState), FALSE, FALSE, xtpBarTypeNormal, xtpBarPopup);
	pt = rcScrollPopup.CenterPoint();
	m_pPaintManager->Triangle(pDC, CPoint(pt.x - 4 , pt.y - 1), CPoint(pt.x + 4, pt.y - 1), CPoint (pt.x, pt.y + 3), clr);
	pDC->FillSolidRect(pt.x - 4, pt.y - 4, 9, 2, clr);
}
