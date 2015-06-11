// XTPDockingPaneOffice2013Theme.cpp : implementation of the CXTPDockingPaneOffice2013Theme class.
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
#include <DockingPane/XTPDockingPaneCaptionButton.h>

#include <DockingPane/Themes/XTPDockingPaneThemeDefault.h>
#include <DockingPane/Themes/XTPDockingPaneThemeGrippered.h>
#include <DockingPane/Themes/XTPDockingPaneThemeVisualStudio2003.h>
#include <DockingPane/Themes/XTPDockingPaneThemeOffice2003.h>
#include <DockingPane/Themes/XTPDockingPaneThemeOffice2007.h>
#include <DockingPane/Themes/XTPDockingPaneThemeOffice2013.h>

#include <DockingPane/TabThemes/XTPDockingPaneTabThemeVisualStudio2012.h>
#include <DockingPane/TabThemes/XTPDockingPanePanelThemeVisualStudio2012.h>
#include <DockingPane/TabColorSets/XTPDockingPaneTabColorSetVisualStudio2012.h>
#include <DockingPane/TabColorSets/XTPDockingPanePanelColorSetVisualStudio2012.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//////////////////////////////////////////////////////////////////////////
// CXTPDockingPaneOffice2013Theme

CXTPDockingPaneOffice2013Theme::CXTPDockingPaneOffice2013Theme()
{
	m_bAutoHidePanelHighlightSelected = FALSE;

	// Tab paint manager
	CXTPDockingPaneTabThemeVisualStudio2012 *pTabTheme = new CXTPDockingPaneTabThemeVisualStudio2012();
	m_pTabPaintManager->SetAppearanceSet(pTabTheme);
	m_pTabPaintManager->SetColorSet(new CXTPTabColorSetOffice2013());
	m_pTabPaintManager->m_clientFrame = xtpTabFrameNone;
	m_pTabPaintManager->m_bShowIcons = FALSE;
	m_pTabPaintManager->m_bHotTracking = TRUE;
	m_pTabPaintManager->m_rcButtonMargin.SetRect(0, 2, 0, 2);


	// Panel paint manager
	CXTPDockingPanePanelThemeVisualStudio2012 *pPanelTheme = new CXTPDockingPanePanelThemeVisualStudio2012();
	m_pPanelPaintManager->SetAppearanceSet(pPanelTheme);
	m_pPanelPaintManager->SetColorSet(new CXTPTabColorSetOffice2013());
	m_pPanelPaintManager->SetLayout(xtpTabLayoutAutoSize);
	m_pPanelPaintManager->m_bRotateImageOnVerticalDraw = TRUE;
	m_pPanelPaintManager->m_rcClientMargin.SetRect(0, 0, 0, 0);
	m_pPanelPaintManager->m_rcControlMargin.SetRect(0, 0, 0, 0);
	m_pPanelPaintManager->m_rcButtonMargin.SetRect(0, 0, 0, 0);
	m_pPanelPaintManager->m_rcButtonTextPadding.SetRect(0, 0, 0, 0);
	m_pPanelPaintManager->m_nButtonExtraLength = 10;
	m_pPanelPaintManager->m_bShowIcons = FALSE;
	m_pPanelPaintManager->m_bHotTracking = TRUE;
}

CXTPDockingPaneOffice2013Theme::~CXTPDockingPaneOffice2013Theme()
{

}

int XTPCalcLogFontHeight(int nPointSize)
{
	int nLogPixelsY    = 96; // Default DPI for display

	HDC hDC = ::GetDC(NULL); // Get screen DC

	if (NULL != hDC)
	{
		nLogPixelsY = ::GetDeviceCaps(hDC, LOGPIXELSY);
		::ReleaseDC(NULL, hDC);
	}

	return -MulDiv(nPointSize, nLogPixelsY, 72);
}

void CXTPDockingPaneOffice2013Theme::RefreshMetrics()
{
	CXTPDockingPanePaintManager::RefreshMetrics();

	LPCTSTR pszKeyNormal   = _T("DockingPane");

	CXTPResourceImages *pImages = XTPResourceImages();

	m_clrBorder       = pImages->GetImageColor (pszKeyNormal, _T("BorderColor"));
	m_clrBackground   = pImages->GetImageColor (pszKeyNormal, _T("BackgroundColor"));
	m_clrSplitter     = pImages->GetImageColor (pszKeyNormal, _T("SplitterColor"));
	m_clrText         = pImages->GetImageColor (pszKeyNormal, _T("TextColor"));
	m_strFontFace     = pImages->GetImageString(pszKeyNormal, _T("FontFace"));
	m_nFontSize       = pImages->GetImageInt   (pszKeyNormal, _T("FontSize"));
	m_rcCaptionMargin = pImages->GetImageRect  (pszKeyNormal, _T("CaptionMargin"));

	m_clrCaptionButtonNormal  = pImages->GetImageColor (pszKeyNormal, _T("CaptionButtonNormal"));
	m_clrCaptionButtonHot     = pImages->GetImageColor (pszKeyNormal, _T("CaptionButtonHot"));
	m_clrCaptionButtonPressed = pImages->GetImageColor (pszKeyNormal, _T("CaptionButtonPressed"));

	LOGFONT lf;
	VERIFY(CXTPDrawHelpers::GetIconLogFont(&lf));
	lf.lfHeight = XTPCalcLogFontHeight(m_nFontSize);
	lf.lfWidth  = 0;
	lf.lfWeight = FW_NORMAL;

	BOOL bOfficeFont = XTPDrawHelpers()->FontExists(m_strFontFace);
	TRACE(_T("%s\n"), m_strFontFace);

	if (bOfficeFont)
	{
		STRCPY_S(lf.lfFaceName, LF_FACESIZE, m_strFontFace);
		if (XTPSystemVersion()->IsClearTypeTextQualitySupported()) lf.lfQuality = 5;
	}
	else
	{
		STRCPY_S(lf.lfFaceName, LF_FACESIZE, _T("Arial")); // Fall-back font
	}

	SetCaptionFontIndirect(&lf);

}

void CXTPDockingPaneOffice2013Theme::DrawSplitter(CDC *pDC, CXTPDockingPaneSplitterWnd *pSplitter)
{
	CXTPClientRect rc(pSplitter);

	pDC->FillSolidRect(rc.left , rc.top, rc.right, rc.bottom, m_clrBackground);

	if(!pSplitter->IsHorizontal())
	{
		pDC->FillSolidRect(rc.right-1, rc.top, rc.right, rc.bottom, m_clrSplitter);
	}
}

CRect CXTPDockingPaneOffice2013Theme::GetPaneCaptionRect(const CXTPDockingPaneSidePanel *pPane)
{
	return CXTPDockingPanePaintManager::GetPaneCaptionRect(pPane);
}

void CXTPDockingPaneOffice2013Theme::AdjustClientRect(CXTPDockingPaneTabbedContainer *pPane, CRect &rc, BOOL bApply)
{
	CXTPDockingPanePaintManager::AdjustClientRect(pPane, rc, bApply);
}

void CXTPDockingPaneOffice2013Theme::AdjustCaptionRect(const CXTPDockingPaneTabbedContainer *pPane, CRect &rc)
{
	CXTPDockingPanePaintManager::AdjustCaptionRect(pPane, rc);
}


// Draw caption when docked
void CXTPDockingPaneOffice2013Theme::DrawCaption(CDC *pDC, CXTPDockingPaneTabbedContainer *pPane, CRect rc)
{
	BOOL bVertical = pPane->IsCaptionVertical();

	CRect rcCaption(rc);
	rcCaption.top   += m_rcCaptionMargin.top;
	rcCaption.left  += m_rcCaptionMargin.left;
	rcCaption.right -= m_rcCaptionMargin.right;
	rcCaption.bottom  = rcCaption.top + m_nTitleHeight;

	HBRUSH hCustomBrush = GetCustomTitleBrush(pPane->IsActive(), bVertical);
	if(NULL != hCustomBrush)
	{
		pDC->FillRect(rcCaption, CBrush::FromHandle(hCustomBrush));
	}

	pDC->SetTextColor(m_clrText);
	DrawCaptionButtons(pDC, pPane->GetCaptionButtons(), rcCaption, m_clrText, 16, 0, bVertical);
	DrawCaptionIcon(pDC, pPane, rcCaption, bVertical);
	DrawCaptionText(pDC, pPane->GetTitle(), rcCaption, bVertical);
}

void CXTPDockingPaneOffice2013Theme::DrawPane(CDC *pDC, CXTPDockingPaneTabbedContainer *pPane, CRect rc)
{
	pDC->FillSolidRect(rc, m_clrBackground);
	pDC->SetBkMode(TRANSPARENT);

	CRect rcCaption(rc);

	if (pPane->IsTabsVisible())
	{
		AdjustCaptionRect(pPane, rc);
		m_pTabPaintManager->DrawTabControl(pPane, pDC, rc);
	}

	if (pPane->IsTitleVisible())
	{
		DrawCaption(pDC, pPane, rcCaption);
	}
}

void CXTPDockingPaneOffice2013Theme::DrawFloatingFrame(CDC *pDC, CXTPDockingPaneMiniWnd *pPane, CRect rc)
{
	BOOL bVertical = FALSE;

	CString strTitle;
	pPane->GetWindowText(strTitle);

	// Border
	CRect rcBorder(rc);
	pDC->Draw3dRect(rcBorder, m_clrBorder, m_clrBorder);

	// Background
	CRect rcBackground(rc);
	rcBackground.DeflateRect(1, 1, 1, 1);
	pDC->FillSolidRect(rcBackground, m_clrBackground);

	CRect rcCaption(rcBackground);
	rcCaption.left  += m_rcCaptionMargin.left;
	rcCaption.right -= m_rcCaptionMargin.right;
	rcCaption.bottom = rcCaption.top + m_nTitleHeight;

	HBRUSH hCustomBrush = GetCustomTitleBrush(pPane->IsActive(), pPane->IsCaptionVertical());
	if(NULL != hCustomBrush)
	{
		pDC->FillRect(rcCaption, CBrush::FromHandle(hCustomBrush));
	}

	pDC->SetTextColor(m_clrText);
	DrawCaptionButtons(pDC, pPane->GetCaptionButtons(), rcCaption, m_clrText, 16, 0, bVertical);
	DrawCaptionIcon(pDC, pPane, rcCaption, bVertical);
	DrawCaptionText(pDC, strTitle, rcCaption, bVertical);
}

void CXTPDockingPaneOffice2013Theme::DrawSidePanel(CDC *pDC, CXTPDockingPaneSidePanel *pPane, CRect rc)
{
	const XTPDockingPaneDirection direction = pPane->GetDirection();

	const BOOL bVertical = xtpPaneDockLeft  == direction || xtpPaneDockRight == direction;

	// Border
	CRect rcBorder(rc);
	pDC->Draw3dRect(rcBorder, m_clrBorder, m_clrBorder);

	// Background
	CRect rcBackground(rc);
	rcBackground.DeflateRect(1, 1, 1, 1);
	pDC->FillSolidRect(rcBackground, m_clrBackground);

	CRect rcCaption = GetPaneCaptionRect(pPane);

	HBRUSH hCustomBrush = GetCustomTitleBrush(pPane->IsActive(), pPane->IsCaptionVertical());
	if(NULL != hCustomBrush)
	{
		pDC->FillRect(rcCaption, CBrush::FromHandle(hCustomBrush));
	}

	pDC->SetTextColor(m_clrText);
	DrawCaptionButtons(pDC, pPane->GetCaptionButtons(), rcCaption, m_clrText, 16, 0, bVertical);
	DrawCaptionIcon(pDC, pPane, rcCaption, bVertical);
	DrawCaptionText(pDC, pPane->GetTitle(), rcCaption, bVertical);
}

void CXTPDockingPaneOffice2013Theme::DrawCaptionButton(CDC *pDC, CXTPDockingPaneCaptionButton *pButton, COLORREF clrButton)
{
		 if (pButton->IsPressed())  clrButton = m_clrCaptionButtonPressed;
	else if (pButton->IsSelected()) clrButton = m_clrCaptionButtonHot;
	else                            clrButton = m_clrCaptionButtonNormal;

	CPoint pt = pButton->GetRect().CenterPoint();
	DrawCaptionButtonEntry(pDC, pButton, pt, clrButton);
}
