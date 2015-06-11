// XTPDockingPaneDefaultTheme.cpp : implementation of the CXTPDockingPaneDefaultTheme class.
//
// This file is a part of the XTREME DOCKINGPANE MFC class library.
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

#include <Common/XTPVC80Helpers.h>
#include <Common/XTPColorManager.h>
#include <Common/XTPDrawHelpers.h>
#include <Common/XTPResourceImage.h>
#include <Common/XTPResourceManager.h>
#include <Common/XTPWinThemeWrapper.h>
#include <Common/XTPImageManager.h>
#include <Common/XTPSystemHelpers.h>

#include <TabManager/Includes.h>

#include <DockingPane/Resource.h>
#include <DockingPane/XTPDockingPaneDefines.h>
#include <DockingPane/XTPDockingPaneBase.h>
#include <DockingPane/XTPDockingPaneBaseContainer.h>
#include <DockingPane/XTPDockingPanePaintManager.h>
#include <DockingPane/XTPDockingPaneTabbedContainer.h>
#include <DockingPane/XTPDockingPane.h>
#include <DockingPane/XTPDockingPaneMiniWnd.h>
#include <DockingPane/XTPDockingPaneSplitterContainer.h>
#include <DockingPane/XTPDockingPaneSidePanel.h>

#include <DockingPane/Themes/XTPDockingPaneThemeDefault.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


///////////////////////////////////////////////////////////////////////////////
// CXTPDockingPaneDefaultTheme

CXTPDockingPaneDefaultTheme::CXTPDockingPaneDefaultTheme()
{
	m_pPanelPaintManager->SetAppearance(xtpTabAppearancePropertyPage);
	m_pTabPaintManager->SetAppearance(xtpTabAppearancePropertyPage);

	m_nCaptionButtonStyle = xtpPaneCaptionButtonDefault;
}


void CXTPDockingPaneDefaultTheme::RefreshMetrics()
{
	CXTPDockingPanePaintManager::RefreshMetrics();

	m_clrActiveCaption.SetStandardValue(GetXtremeColor(COLOR_ACTIVECAPTION));
	m_clrNormalCaption.SetStandardValue(GetXtremeColor(COLOR_INACTIVECAPTION));

	m_clrActiveCaptionText.SetStandardValue(GetXtremeColor(COLOR_CAPTIONTEXT));
	m_clrNormalCaptionText.SetStandardValue(GetXtremeColor(COLOR_INACTIVECAPTIONTEXT));
}

void CXTPDockingPaneDefaultTheme::DrawFloatingFrame(CDC* pDC, CXTPDockingPaneMiniWnd* pPane, CRect rc)
{
	pDC->Draw3dRect(rc, GetXtremeColor(COLOR_3DFACE), 0);
	rc.DeflateRect(1, 1);
	pDC->Draw3dRect(rc, GetXtremeColor(COLOR_3DHIGHLIGHT), GetXtremeColor(COLOR_3DSHADOW));
	rc.DeflateRect(1, 1);
	pDC->Draw3dRect(rc, GetXtremeColor(COLOR_3DFACE), GetXtremeColor(COLOR_3DFACE));

	int nTitleHeight = m_nTitleHeight;
	CRect rcCaption(rc);
	rcCaption.DeflateRect(0, 0, 0, rc.Height() - nTitleHeight - 3);

	CXTPBufferDC dcCache(*pDC, rcCaption);
	dcCache.SetBkMode(TRANSPARENT);

	dcCache.FillSolidRect(rcCaption, GetXtremeColor(COLOR_3DFACE));

	CString strTitle;
	pPane->GetWindowText(strTitle);

	rcCaption.DeflateRect(1, 2, 1, 2);

	BOOL bActive = m_bHighlightActiveCaption && pPane->IsActive();
	DrawCaptionPart(&dcCache, pPane, rcCaption, strTitle, bActive, FALSE);
}

void CXTPDockingPaneDefaultTheme::DrawSidePanel(CDC* pDC, CXTPDockingPaneSidePanel* pPane, CRect rc)
{
	pDC->FillSolidRect(rc, GetXtremeColor(COLOR_3DFACE));
	pDC->SetBkMode(TRANSPARENT);

	pDC->Draw3dRect(rc, GetXtremeColor(COLOR_WINDOWFRAME), GetXtremeColor(COLOR_WINDOWFRAME));

	CRect rcCaption = GetPaneCaptionRect(pPane);

	BOOL bActive = m_bHighlightActiveCaption && pPane->IsActive();

	CString strTitle = pPane->GetTitle();

	DrawCaptionPart(pDC, pPane, rcCaption, strTitle, bActive, pPane->GetDirection() == xtpPaneDockLeft || pPane->GetDirection() == xtpPaneDockRight);
}

void CXTPDockingPaneDefaultTheme::DrawCaptionPart(CDC* pDC, CXTPDockingPaneBase* pPane, CRect rcCaption, const CString& strTitle, BOOL bActive, BOOL bVertical)
{
	HBRUSH hCustomBrush = GetCustomTitleBrush(bActive, bVertical);
	if(NULL == hCustomBrush)
	{
		pDC->FillSolidRect(rcCaption, bActive ? m_clrActiveCaption : m_clrNormalCaption);
	}
	else
	{
		pDC->FillRect(rcCaption, CBrush::FromHandle(hCustomBrush));
	}

	XTPDeflateRectDirection(rcCaption, 0, 0, 2, 0, bVertical);
	DrawCaptionButtons(pDC, pPane->GetCaptionButtons(), rcCaption, GetXtremeColor(COLOR_BTNTEXT), 14, 0, bVertical);

	pDC->SetTextColor(IsCaptionEnabled(pPane) ? (bActive ? m_clrActiveCaptionText : m_clrNormalCaptionText) : m_clrDisabledCaptionText);
	XTPDeflateRectDirection(rcCaption, 4, 1, 0, 0, bVertical);

	DrawCaptionIcon(pDC, pPane, rcCaption, bVertical);
	DrawCaptionText(pDC, strTitle, rcCaption, bVertical);
}

void CXTPDockingPaneDefaultTheme::DrawCaption(CDC* pDC, CXTPDockingPaneTabbedContainer* pPane, CRect rc)
{
	BOOL bVertical = pPane->IsCaptionVertical();

	CRect rcCaption(rc);
	XTPDeflateRectDirection(rcCaption, 0, 2, 0, XTPGetRectDirectionHeight(rc, bVertical)  - m_nTitleHeight - 1, bVertical);

	BOOL bActive = m_bHighlightActiveCaption && pPane->IsActive();

	if (bVertical)
	{
		pDC->FillSolidRect(rc.left, rc.top, rcCaption.right + 2, rcCaption.Height(), GetXtremeColor(COLOR_3DFACE));
	}
	else
	{
		pDC->FillSolidRect(rc.left, rc.top, rc.Width(), rcCaption.bottom + 2, GetXtremeColor(COLOR_3DFACE));
	}
		
	DrawCaptionPart(pDC, pPane, rcCaption, pPane->GetTitle(), bActive, bVertical);
}
