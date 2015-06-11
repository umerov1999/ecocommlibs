// XTPResourceTheme.cpp : implementation of the CXTPResourceTheme class.
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

#include <Common/XTPDrawHelpers.h>
#include <Common/XTPResourceImage.h>
#include <Common/XTPHookManager.h>
#include <Common/XTPMarkupRender.h>
#include <Common/XTPResourceManager.h>
#include <Common/XTPSystemHelpers.h>
#include <Common/XTPColorManager.h>
#include <Common/XTPWinThemeWrapper.h>
#include <Common/XTPVC80Helpers.h>
#include <Common/ScrollBar/XTPScrollBase.h>
#include <Common/ScrollBar/XTPScrollInfo.h>
#include <Common/ScrollBar/XTPScrollBarPaintManager.h>

#include <CommandBars/Slider/XTPSliderPaintManager.h>
#include <CommandBars/XTPCommandBarsDefines.h>
#include <CommandBars/XTPCommandBar.h>
#include <CommandBars/XTPPopupBar.h>
#include <CommandBars/MessageBar/XTPMessageBar.h>
#include <CommandBars/XTPControls.h>
#include <CommandBars/XTPControl.h>
#include <CommandBars/XTPControlButton.h>
#include <CommandBars/XTPControlPopup.h>
#include <CommandBars/XTPControlGallery.h>
#include <CommandBars/XTPPaintManager.h>

#include <CommandBars/Gallery/XTPControlGalleryItem.h>

#include <CommandBars/Themes/XTPCommandBarsOfficeXPTheme.h>
#include <CommandBars/Themes/XTPCommandBarsOffice2003Theme.h>
#include <CommandBars/Themes/XTPCommandBarsResourceTheme.h>
#include <CommandBars/Themes/Resource/XTPControlGalleryResourceTheme.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//////////////////////////////////////////////////////////////////////////
// CXTPControlGalleryResourceTheme

CXTPControlGalleryResourceTheme::CXTPControlGalleryResourceTheme(CXTPPaintManager* pPaintManager)
	: CXTPControlGalleryPaintManager(pPaintManager)
{
}

void CXTPControlGalleryResourceTheme::FillControl(CDC* pDC, CXTPControlGallery* pGallery, CRect rcControl)
{
	if (!pGallery->IsTransparent())
	{
		pDC->FillSolidRect(rcControl,
			pGallery->GetParent()->GetPosition() == xtpBarPopup  && pGallery->GetParent()->GetType() == xtpBarTypePopup?
			m_pPaintManager->GetXtremeColor(XPCOLOR_MENUBAR_FACE) :
			pGallery->GetSelected() && pGallery->GetEnabled() ? m_clrControlGallerySelected : m_clrControlGalleryNormal);
	}

	if (pGallery->IsShowBorders())
	{
		pDC->Draw3dRect(rcControl, m_clrControlGalleryBorder, m_clrControlGalleryBorder);
	}

	if (pGallery->HasBottomSeparator())
	{
		pDC->FillSolidRect(rcControl.left, rcControl.bottom - 2, rcControl.Width(), 1, RGB(197, 197, 197));
	}
}

void CXTPControlGalleryResourceTheme::RefreshMetrics()
{
	CXTPControlGalleryPaintManager::RefreshMetrics();

	if(!XTPResourceImages()->IsValid())
			return;

	m_cyPopupUp = 21;
	m_cyPopupDown = 19;

	CXTPResourceTheme* pPaintManager = (CXTPResourceTheme*)m_pPaintManager;

	m_clrControlGallerySelected = pPaintManager->GetImages()->GetImageColor(_T("Toolbar"), _T("ControlGallerySelected"));
	m_clrControlGalleryNormal = pPaintManager->GetImages()->GetImageColor(_T("Toolbar"), _T("ControlGalleryNormal"));
	m_clrControlGalleryBorder = pPaintManager->GetImages()->GetImageColor(_T("Toolbar"), _T("ControlGalleryBorder"));
}

void CXTPControlGalleryResourceTheme::DrawLabel(CDC* pDC, CXTPControlGalleryItem* pLabel, CRect rcItem)
{
	CXTPPaintManager* pPaintManager = m_pPaintManager;
	pDC->FillSolidRect(rcItem, pPaintManager->GetXtremeColor(XPCOLOR_LABEL));

	pDC->FillSolidRect(rcItem.left, rcItem.bottom - 1, rcItem.Width(), 1, RGB(197, 197, 197));

	CXTPFontDC fnt(pDC, pPaintManager->GetRegularBoldFont());

	CRect rcText(rcItem);
	rcText.DeflateRect(10, 0);
	pDC->SetTextColor(pPaintManager->GetXtremeColor(XPCOLOR_MENUBAR_TEXT));
	pDC->DrawText(pLabel->GetCaption(), rcText, DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS);
}

void CXTPControlGalleryResourceTheme::DrawPopupScrollBar(CDC* pDC, CXTPControlGallery* pGallery)
{
	#define GETPARTSTATE3_R(ht, bEnabled) (!bEnabled ? 4 : nPressedHt == ht ? 3 : nHotHt == ht ? 2 : nHotHt > 0 || nPressedHt > 0 ? 1 : 0)

	XTP_SCROLLBAR_TRACKINFO *pSBTrack = pGallery->GetScrollBarTrackInfo();
	XTP_SCROLLBAR_POSINFO   *pSBInfo  = pGallery->GetScrollBarPosInfo();

	CRect rcControl = pGallery->GetRect();
	CRect rcScroll(rcControl.right - 15, rcControl.top, rcControl.right, rcControl.bottom);

	CRect rcScrollUp(rcScroll.left, rcScroll.top, rcScroll.right, rcScroll.top + m_cyPopupUp);
	CRect rcScrollDown(rcScroll.left, rcScrollUp.bottom, rcScroll.right, rcScrollUp.bottom + m_cyPopupDown);
	CRect rcScrollPopup(rcScroll.left, rcScrollDown.bottom, rcScroll.right, rcScroll.bottom);

	if(!XTPResourceImages()->IsValid())
			return;

	CXTPResourceTheme* pPaintManager = (CXTPResourceTheme*)m_pPaintManager;

	CXTPResourceImage* pImage = pPaintManager->LoadImage(_T("CONTROLGALLERYUP"));
	ASSERT(pImage);
	if (!pImage)
		return;

	BOOL bControlEnabled = pGallery->GetEnabled();
	BOOL nPressedHt = pSBTrack ? pSBInfo->ht : -1;
	BOOL nHotHt = pSBTrack ? -1 : pSBInfo->ht;

	if (nHotHt == HTNOWHERE && nPressedHt == -1 && bControlEnabled && IsKeyboardSelected(pGallery->GetSelected()) && pGallery->GetSelectedItem() == -1)
		nHotHt = XTP_HTSCROLLPOPUP;

	int nState = GETPARTSTATE3_R(XTP_HTSCROLLUP, (bControlEnabled && pGallery->IsScrollButtonEnabled(XTP_HTSCROLLUP)));
	pImage->DrawImage(pDC, rcScrollUp, pImage->GetSource(nState, 5), CRect(3, 3, 3, 3), 0xFF00FF);


	nState = GETPARTSTATE3_R(XTP_HTSCROLLDOWN, (bControlEnabled && pGallery->IsScrollButtonEnabled(XTP_HTSCROLLDOWN)));
	pImage = pPaintManager->LoadImage(_T("CONTROLGALLERYDOWN"));
	pImage->DrawImage(pDC, rcScrollDown, pImage->GetSource(nState, 5), CRect(3, 3, 3, 3), 0xFF00FF);

	nState = GETPARTSTATE3_R(XTP_HTSCROLLPOPUP, bControlEnabled);
	pImage = pPaintManager->LoadImage(_T("CONTROLGALLERYPOPUP"));
	pImage->DrawImage(pDC, rcScrollPopup, pImage->GetSource(nState, 5), CRect(3, 3, 3, 3), 0xFF00FF);
}
