// XTPDockingPaneWinNativeTheme.cpp : implementation of the CXTPDockingPaneWinNativeTheme class.
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
#include <DockingPane/Themes/XTPDockingPaneThemeGrippered.h>
#include <DockingPane/Themes/XTPDockingPaneThemeVisualStudio2003.h>
#include <DockingPane/Themes/XTPDockingPaneThemeWinNative.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//////////////////////////////////////////////////////////////////////////
/// CXTPDockingPaneWinNativeTheme

CXTPDockingPaneWinNativeTheme::CXTPDockingPaneWinNativeTheme()
{
	m_pPanelPaintManager->SetAppearance(xtpTabAppearancePropertyPage);
	m_pPanelPaintManager->SetColor(xtpTabColorWinNative);
	m_pPanelPaintManager->m_bHotTracking = TRUE;

	m_pTabPaintManager->SetAppearance(xtpTabAppearancePropertyPage);
	m_pTabPaintManager->SetColor(xtpTabColorWinNative);
	m_pTabPaintManager->m_bHotTracking = TRUE;

	m_bGradientCaption = TRUE;
	m_nCaptionFontGap = 7;

	m_nCaptionButtonStyle = xtpPaneCaptionButtonThemedExplorerBar;
}

void CXTPDockingPaneWinNativeTheme::RefreshMetrics()
{
	CXTPDockingPaneVisualStudio2003Theme::RefreshMetrics();

	if (m_themeWindow->IsAppThemed())
	{
		m_clrActiveCaptionText.SetStandardValue(GetXtremeColor(COLOR_CAPTIONTEXT));
		m_clrNormalCaptionText.SetStandardValue(GetXtremeColor(COLOR_INACTIVECAPTIONTEXT));
	}
}

void CXTPDockingPaneWinNativeTheme::DrawFloatingFrame(CDC* pDC, CXTPDockingPaneMiniWnd* pPane, CRect rc)
{
	if (!m_themeWindow->IsAppThemed())
	{
		CXTPDockingPaneVisualStudio2003Theme::DrawFloatingFrame(pDC, pPane, rc);
		return;
	}
	BOOL bActive = m_bHighlightActiveCaption && pPane->IsActive();

	int nState = bActive ? CS_ACTIVE : CS_INACTIVE;

	m_themeWindow->DrawThemeBackground(pDC->GetSafeHdc(), WP_SMALLFRAMELEFT, nState, CRect(rc.left, rc.top, rc.left + 3, rc.bottom), 0);
	m_themeWindow->DrawThemeBackground(pDC->GetSafeHdc(), WP_SMALLFRAMERIGHT, nState, CRect(rc.right - 3, rc.top, rc.right, rc.bottom), 0);
	m_themeWindow->DrawThemeBackground(pDC->GetSafeHdc(), WP_SMALLFRAMEBOTTOM, nState, CRect(rc.left, rc.bottom - 3, rc.right, rc.bottom), 0);

	int nTitleHeight = m_nTitleHeight;
	CRect rcCaption(rc);
	rcCaption.DeflateRect(0, 0, 0, rc.Height() - nTitleHeight - 3 - 2);

	CXTPBufferDC dcCache(*pDC, rcCaption);
	dcCache.SetBkMode(TRANSPARENT);

	HBRUSH hCustomBrush = GetCustomTitleBrush(bActive, pPane->IsCaptionVertical());
	if(NULL == hCustomBrush)
	{
		m_themeWindow->DrawThemeBackground(dcCache, WP_SMALLCAPTION, nState, rcCaption, 0);
	}
	else
	{
		dcCache.FillRect(rcCaption, CBrush::FromHandle(hCustomBrush));
	}

	rcCaption.DeflateRect(3, 0);

	CString strTitle;
	pPane->GetWindowText(strTitle);

	DrawCaptionPart(&dcCache, pPane, rcCaption, strTitle, bActive, FALSE);
}

void CXTPDockingPaneWinNativeTheme::DrawSidePanel(CDC* pDC, CXTPDockingPaneSidePanel* pPane, CRect rc)
{
	CXTPDockingPaneVisualStudio2003Theme::DrawSidePanel(pDC, pPane, rc);
}

void CXTPDockingPaneWinNativeTheme::DrawCaptionPart(CDC* pDC, CXTPDockingPaneBase* pPane, CRect rcCaption, const CString& strTitle, BOOL bActive, BOOL bVertical)
{
	if (!m_themeWindow->IsAppThemed())
	{
		CXTPDockingPaneVisualStudio2003Theme::DrawCaptionPart(pDC, pPane, rcCaption, strTitle, bActive, bVertical);
		return;
	}

	COLORREF clrText = bActive ? m_clrActiveCaptionText : m_clrNormalCaptionText;
	COLORREF clrButton = clrText;

	if (pPane->GetType() == xtpPaneTypeSidePanel)
	{
		clrButton = FillCaptionPart(pDC, pPane, rcCaption, bActive, bVertical);
	}

	XTPDeflateRectDirection(rcCaption, 0, 0, 2, 0, bVertical);
	DrawCaptionButtons(pDC, pPane->GetCaptionButtons(), rcCaption, clrButton, 16, 3, bVertical);

	pDC->SetTextColor(IsCaptionEnabled(pPane) ? clrText : m_clrDisabledCaptionText);
	XTPDeflateRectDirection(rcCaption, 4, 1, 0, 0, bVertical);

	DrawCaptionIcon(pDC, pPane, rcCaption, bVertical);
	DrawCaptionText(pDC, strTitle, rcCaption, bVertical);
}

void CXTPDockingPaneWinNativeTheme::DrawCaption(CDC* pDC, CXTPDockingPaneTabbedContainer* pPane, CRect rc)
{
	if (!m_themeWindow->IsAppThemed())
	{
		CXTPDockingPaneVisualStudio2003Theme::DrawCaption(pDC, pPane, rc);
		return;
	}

	BOOL bVertical = pPane->IsCaptionVertical();
	BOOL bActive = m_bHighlightActiveCaption && pPane->IsActive();

	CRect rcCaption(0, 0, rc.right, m_nTitleHeight + 1);
	if (bVertical)
	{
		rcCaption = CRect(0, 0, m_nTitleHeight + 1, rc.bottom);
	}

	HBRUSH hCustomBrush = GetCustomTitleBrush(bActive, bVertical);
	if(NULL == hCustomBrush)
	{
		m_themeWindow->DrawThemeBackground(pDC->GetSafeHdc(), WP_SMALLCAPTION, bActive ? CS_ACTIVE : CS_INACTIVE, rcCaption, 0);
	}
	else
	{
		pDC->FillRect(rcCaption, CBrush::FromHandle(hCustomBrush));
	}

	CString strTitle = pPane->GetTitle();
	DrawCaptionPart(pDC, pPane, rcCaption, strTitle, bActive, bVertical);
}
