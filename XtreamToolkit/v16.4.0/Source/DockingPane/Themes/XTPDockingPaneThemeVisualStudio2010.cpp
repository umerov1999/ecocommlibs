// XTPDockingPaneVisualStudio2010Theme.cpp : implementation of the CXTPDockingPaneVisualStudio2010Theme class.
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
#include <TabManager/Themes/XTPTabThemeVisualStudio2010.h>

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
#include <DockingPane/Themes/XTPDockingPaneThemeVisualStudio2005Beta1.h>
#include <DockingPane/Themes/XTPDockingPaneThemeVisualStudio2005Beta2.h>
#include <DockingPane/Themes/XTPDockingPaneThemeVisualStudio2005.h>
#include <DockingPane/Themes/XTPDockingPaneThemeVisualStudio2008.h>
#include <DockingPane/Themes/XTPDockingPaneThemeVisualStudio2010.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//////////////////////////////////////////////////////////////////////////
// CXTPDockingPaneVisualStudio2010Theme

CXTPDockingPaneVisualStudio2010Theme::CXTPDockingPaneVisualStudio2010Theme()
{
	m_nCaptionFontGap = 6;
	m_nCaptionButtonSize = 15;
	m_nCaptionButtonStyle = xtpPaneCaptionButtonOffice2003;
	m_bAutoHidePanelHighlightSelected = FALSE;


	CXTPTabThemeVisualStudio2010 *pTabTheme = new CXTPTabThemeVisualStudio2010();
	pTabTheme->m_rcHeaderMargin.left = 0;
	pTabTheme->m_rcHeaderMargin.right = 0;

	m_pTabPaintManager->SetAppearanceSet(pTabTheme);
	m_pTabPaintManager->SetColorSet(new CXTPTabColorSetVisualStudio2010(TRUE));
	m_pTabPaintManager->m_clientFrame = xtpTabFrameSingleLine;
	m_pTabPaintManager->m_rcClientMargin.top = 1;
	m_pTabPaintManager->m_bHotTracking = TRUE;


	CXTPTabThemeVisualStudio2010 *pPanelTheme = new CXTPTabThemeVisualStudio2010();
	pPanelTheme->m_bDrawNormalTab = TRUE;
	pPanelTheme->m_rcHeaderMargin.left = 0;

	m_pPanelPaintManager->SetAppearanceSet(pPanelTheme);
	m_pPanelPaintManager->SetColor(xtpTabColorVisualStudio2010);
	m_pPanelPaintManager->m_bHotTracking = TRUE;
}

void CXTPDockingPaneVisualStudio2010Theme::RefreshMetrics()
{
	CXTPDockingPaneVisualStudio2005Theme::RefreshMetrics();

	m_bLunaTheme = XTPSystemVersion()->IsWinVistaOrGreater();

	COLORREF clrBackground = m_pTabPaintManager->GetColorSet()->m_clrAutoHideFace;

	m_clrNormalCaption.SetStandardValue(RGB(77,96,130), RGB(61, 82, 119));
	m_clrActiveCaption.SetStandardValue(RGB(255, 232, 166));
	m_clrCaptionBorder = clrBackground;
	m_clrSplitter.SetStandardValue(clrBackground);
	m_clrSplitterGradient.SetStandardValue(clrBackground);

	m_clrNormalCaptionText.SetStandardValue(RGB(255, 255, 255));
	m_clrActiveCaptionText.SetStandardValue(0);

	if (!m_bLunaTheme)
	{
		m_clrNormalCaption.SetStandardValue(RGB(74, 97, 132));
		m_clrActiveCaption.SetStandardValue(RGB(255, 243, 206));
	}

	m_arrColor[XPCOLOR_3DFACE]                  = clrBackground;
	m_arrColor[XPCOLOR_HIGHLIGHT]               = RGB(255, 255, 247);
	m_arrColor[XPCOLOR_HIGHLIGHT_BORDER]        = RGB(231, 195,  99);
	m_arrColor[XPCOLOR_HIGHLIGHT_PUSHED]        = RGB(255, 235, 165);
	m_arrColor[XPCOLOR_HIGHLIGHT_PUSHED_BORDER] = RGB(231, 195,  99);
	m_arrColor[XPCOLOR_PUSHED_TEXT]             = RGB(  0,   0,   0);
	m_arrColor[XPCOLOR_HIGHLIGHT_TEXT]          = RGB(  0,   0,   0);
}

void CXTPDockingPaneVisualStudio2010Theme::DrawFloatingFrame(CDC* pDC, CXTPDockingPaneMiniWnd* pPane, CRect rc)
{
	pDC->Draw3dRect(rc, m_clrCaptionBorder, m_clrCaptionBorder);
	rc.DeflateRect(1, 1);
	pDC->Draw3dRect(rc, m_clrCaptionBorder, m_clrCaptionBorder);
	rc.DeflateRect(1, 1);
	pDC->Draw3dRect(rc, m_clrCaptionBorder, m_clrCaptionBorder);

	int nTitleHeight = m_nTitleHeight;
	CRect rcCaption(rc);
	rcCaption.DeflateRect(0, 0, 0, rc.Height() - nTitleHeight - 3);

	CXTPBufferDC dcCache(*pDC, rcCaption);
	dcCache.SetBkMode(TRANSPARENT);

	HBRUSH hCustomBrush = GetCustomTitleBrush(pPane->IsActive(), pPane->IsCaptionVertical());
	if(NULL == hCustomBrush)
	{
		dcCache.FillSolidRect(rcCaption, m_clrCaptionBorder);
	}
	else
	{
		dcCache.FillRect(rcCaption, CBrush::FromHandle(hCustomBrush));
	}

	CString strTitle;
	pPane->GetWindowText(strTitle);

	rcCaption.DeflateRect(1, 2, 1, 0);

	BOOL bActive = m_bHighlightActiveCaption && pPane->IsActive();
	DrawCaptionPart(&dcCache, pPane, rcCaption, strTitle, bActive, FALSE);
}

COLORREF CXTPDockingPaneVisualStudio2010Theme::FillCaptionPart(CDC* pDC, CXTPDockingPaneBase* /*pPane*/, CRect rcCaption, BOOL bActive, BOOL bVertical)
{
	HBRUSH hCustomBrush = GetCustomTitleBrush(bActive, bVertical);
	if(NULL == hCustomBrush)
	{
		CXTPPaintManagerColorGradient& clr = bActive ? m_clrActiveCaption : m_clrNormalCaption;
		XTPDrawHelpers()->GradientFill(pDC, rcCaption, clr, bVertical);

		if (bActive)
		{
			CRect rTop(rcCaption);
			if (!bVertical) rTop.bottom = rTop.top + rTop.Height() / 2;
			else rTop.right = rTop.left + rTop.Width() / 2;

			XTPDrawHelpers()->GradientFill(pDC, rTop, RGB(255, 252, 242), RGB(255, 243, 207), bVertical);
		}

		if (!bVertical)
		{
			pDC->SetPixel(rcCaption.left, rcCaption.top, CXTPDrawHelpers::BlendColors(clr, m_clrCaptionBorder, (double)60 / 255));
			pDC->SetPixel(rcCaption.left + 1, rcCaption.top, CXTPDrawHelpers::BlendColors(clr, m_clrCaptionBorder, (double)120 / 255));
			pDC->SetPixel(rcCaption.left, rcCaption.top + 1, CXTPDrawHelpers::BlendColors(clr, m_clrCaptionBorder, (double)120 / 255));

			pDC->SetPixel(rcCaption.right - 1, rcCaption.top, CXTPDrawHelpers::BlendColors(clr, m_clrCaptionBorder, (double)60 / 255));
			pDC->SetPixel(rcCaption.right - 2, rcCaption.top, CXTPDrawHelpers::BlendColors(clr, m_clrCaptionBorder, (double)120 / 255));
			pDC->SetPixel(rcCaption.right - 1, rcCaption.top + 1, CXTPDrawHelpers::BlendColors(clr, m_clrCaptionBorder, (double)120 / 255));
		}
	}
	else
	{
		pDC->FillRect(rcCaption, CBrush::FromHandle(hCustomBrush));
	}

	return bActive ? RGB(115, 97, 57) : RGB(206, 215, 222);
}

void CXTPDockingPaneVisualStudio2010Theme::DrawCaptionButtonEntry(CDC* pDC, CXTPDockingPaneCaptionButton* pButton, CPoint pt, COLORREF clrButton)
{
	if (pButton->GetID() == XTP_IDS_DOCKINGPANE_AUTOHIDE)
	{
		CXTPPenDC pen(*pDC, clrButton);
		pt.y--;

		if (pButton->GetState() & xtpPanePinPushed)
		{
			pDC->MoveTo(pt.x - 5, pt.y); pDC->LineTo(pt.x - 1, pt.y);
			pDC->MoveTo(pt.x - 1, pt.y - 3); pDC->LineTo(pt.x - 1, pt.y + 4);
			pDC->MoveTo(pt.x - 1, pt.y + 2); pDC->LineTo(pt.x + 5, pt.y + 2);
			pDC->LineTo(pt.x + 5, pt.y - 2); pDC->LineTo(pt.x - 1, pt.y - 2);
			pDC->MoveTo(pt.x - 1, pt.y + 1); pDC->LineTo(pt.x + 5, pt.y + 1);
		}
		else
		{
			pt.y += 2;
			pDC->MoveTo(pt.x - 3, pt.y - 1 + 1); pDC->LineTo(pt.x + 4, pt.y - 1 + 1);
			pDC->MoveTo(pt.x, pt.y - 1 + 1); pDC->LineTo(pt.x, pt.y + 3 + 2);
			pDC->MoveTo(pt.x - 2, pt.y - 1 + 1); pDC->LineTo(pt.x - 2, pt.y - 6);
			pDC->LineTo(pt.x + 2, pt.y - 6); pDC->LineTo(pt.x + 2, pt.y - 1 + 1);
			pDC->MoveTo(pt.x + 1, pt.y - 1 + 1); pDC->LineTo(pt.x + 1, pt.y - 6);
		}
	}
	else
	{
		CXTPDockingPaneVisualStudio2008Theme::DrawCaptionButtonEntry(pDC, pButton, pt, clrButton);
	}

}
