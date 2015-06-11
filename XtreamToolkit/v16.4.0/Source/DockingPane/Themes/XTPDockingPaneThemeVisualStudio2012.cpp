// XTPDockingPaneVisualStudio2012Theme.cpp : implementation of the CXTPDockingPaneVisualStudio2012Theme class.
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
#include <Common/XTPTheme.h>
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
#include <DockingPane/ColorSets/XTPDockingPaneColorSetVisualStudio2012.h>

#include <DockingPane/Themes/XTPDockingPaneThemeDefault.h>
#include <DockingPane/Themes/XTPDockingPaneThemeDefault.h>
#include <DockingPane/Themes/XTPDockingPaneThemeGrippered.h>
#include <DockingPane/Themes/XTPDockingPaneThemeVisualStudio2003.h>
#include <DockingPane/Themes/XTPDockingPaneThemeVisualStudio2005Beta1.h>
#include <DockingPane/Themes/XTPDockingPaneThemeVisualStudio2005Beta2.h>
#include <DockingPane/Themes/XTPDockingPaneThemeVisualStudio2005.h>
#include <DockingPane/Themes/XTPDockingPaneThemeVisualStudio2008.h>
#include <DockingPane/Themes/XTPDockingPaneThemeVisualStudio2010.h>
#include <DockingPane/Themes/XTPDockingPaneThemeVisualStudio2012.h>

#include <DockingPane/TabThemes/XTPDockingPaneTabThemeVisualStudio2012.h>
#include <DockingPane/TabThemes/XTPDockingPanePanelThemeVisualStudio2012.h>
#include <DockingPane/TabColorSets/XTPDockingPaneTabColorSetVisualStudio2012.h>
#include <DockingPane/TabColorSets/XTPDockingPanePanelColorSetVisualStudio2012.h>
#include <DockingPane/XTPDockingPaneManager.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



//////////////////////////////////////////////////////////////////////////
// CXTPDockingPaneVisualStudio2012Theme

#define Base CXTPDockingPaneDefaultTheme // CXTPDockingPanePaintManager

CXTPDockingPaneVisualStudio2012Theme::CXTPDockingPaneVisualStudio2012Theme()
{
	m_bAutoHidePanelHighlightSelected = FALSE;

	// Tab paint manager
	CXTPDockingPaneTabThemeVisualStudio2012 *pTabTheme = new CXTPDockingPaneTabThemeVisualStudio2012();
	m_pTabPaintManager->SetAppearanceSet(pTabTheme);
	m_pTabPaintManager->SetColorSet(new CXTPDockingPaneTabColorSetVisualStudio2012());
	m_pTabPaintManager->m_clientFrame = xtpTabFrameNone;
	m_pTabPaintManager->m_bShowIcons = FALSE;
	m_pTabPaintManager->m_bHotTracking = TRUE;
	m_pTabPaintManager->m_rcButtonMargin.SetRect(0, 1, 0, 1);


	// Panel paint manager
	CXTPDockingPanePanelThemeVisualStudio2012 *pPanelTheme = new CXTPDockingPanePanelThemeVisualStudio2012();
	m_pPanelPaintManager->SetAppearanceSet(pPanelTheme);
	m_pPanelPaintManager->SetColorSet(new CXTPDockingPanePanelColorSetVisualStudio2012());
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

CXTPDockingPaneVisualStudio2012Theme::~CXTPDockingPaneVisualStudio2012Theme()
{

}

int XTPCalcLogFontHeight(int nPointSize);

void CXTPDockingPaneVisualStudio2012Theme::RefreshMetrics()
{
	Base::RefreshMetrics();

	CXTPResourceImages *pImages = XTPResourceImages();

	m_csInactive.RefreshMetrics(pImages, _T("DockingPane.Inactive"));
	m_csActive  .RefreshMetrics(pImages, _T("DockingPane.Active"));

	LPCTSTR pszKeyNormal   = _T("DockingPane");
	m_strFontFace     = pImages->GetImageString(pszKeyNormal, _T("FontFace"));
	m_nFontSize       = pImages->GetImageInt   (pszKeyNormal, _T("FontSize"));
	m_rcCaptionMargin = pImages->GetImageRect  (pszKeyNormal, _T("CaptionMargin"));
	m_clrSplitter     = pImages->GetImageColor (pszKeyNormal, _T("SplitterColor"));
	m_clrBackground   = pImages->GetImageColor (pszKeyNormal, _T("Background"));

	m_clrSplitterGradient.SetStandardValue(pImages->GetImageColor(pszKeyNormal, _T("SplitterColor")),
		pImages->GetImageColor(pszKeyNormal, _T("SplitterColorDark")));
}

void CXTPDockingPaneVisualStudio2012Theme::DrawSplitter(CDC *pDC, CXTPDockingPaneSplitterWnd *pSplitter)
{
	CXTPClientRect rc(pSplitter);
	pDC->FillSolidRect(rc, m_clrSplitter);

	CXTPDockingPaneManager* pPaneManager = pSplitter->GetDockingPaneManager();
	if (pPaneManager)
	{
		CXTPDockingPaneBase* pPaneBase = pSplitter->GetPaneBase();
		if (pPaneBase)
		{
			XTPDockingPaneDirection direction = pPaneManager->GetPaneDirection(pPaneBase);

			CRect rc1(rc);
			CRect rc2(rc);

			BOOL bIsHoriz = pSplitter->IsHorizontal();
			if (bIsHoriz)
			{
				rc.DeflateRect(2,0,2,0);
				pDC->FillSolidRect(rc, m_clrSplitterGradient.clrDark);
			}
			else
			{
				rc1.bottom = rc1.top + rc1.Height()/2;
				rc2.top = rc1.bottom;

				if (direction == xtpPaneDockLeft)
				{
					rc1.left = rc1.right-2;
					rc2.left = rc2.right-2;
				}
				else
				{
					rc1.right = rc1.left + 2;
					rc2.right = rc2.left + 2;
				}

				XTPDrawHelpers()->GradientFill(pDC, &rc1, m_clrSplitterGradient.clrLight,
					m_clrSplitterGradient.clrDark, bIsHoriz);

				XTPDrawHelpers()->GradientFill(pDC, &rc2, m_clrSplitterGradient.clrDark,
					m_clrSplitterGradient.clrLight, bIsHoriz);
			}
		}
	}
}

CRect CXTPDockingPaneVisualStudio2012Theme::GetPaneCaptionRect(const CXTPDockingPaneSidePanel *pPane)
{
	return Base::GetPaneCaptionRect(pPane);
}

void CXTPDockingPaneVisualStudio2012Theme::AdjustClientRect(CXTPDockingPaneTabbedContainer *pPane, CRect &rc, BOOL bApply)
{
	Base::AdjustClientRect(pPane, rc, bApply);
}

void CXTPDockingPaneVisualStudio2012Theme::AdjustCaptionRect(const CXTPDockingPaneTabbedContainer *pPane, CRect &rc)
{
	Base::AdjustCaptionRect(pPane, rc);
}

void CXTPDockingPaneVisualStudio2012Theme::DrawGripper(CDC *pDC, CRect rcGripper, COLORREF clr, BOOL bVertical)
{
	if (bVertical)
	{
		DrawDashedLineV(pDC, rcGripper.left + 0, rcGripper.top + 0, rcGripper.bottom, clr);
		DrawDashedLineV(pDC, rcGripper.left + 2, rcGripper.top + 2, rcGripper.bottom, clr);
		DrawDashedLineV(pDC, rcGripper.left + 4, rcGripper.top + 0, rcGripper.bottom, clr);
	}
	else
	{
		DrawDashedLineH(pDC, rcGripper.left + 0, rcGripper.top + 0, rcGripper.right, clr);
		DrawDashedLineH(pDC, rcGripper.left + 2, rcGripper.top + 2, rcGripper.right, clr);
		DrawDashedLineH(pDC, rcGripper.left + 0, rcGripper.top + 4, rcGripper.right, clr);
	}
}

void CXTPDockingPaneVisualStudio2012Theme::DrawDashedLineH(CDC *pDC, int left, int top, int right, COLORREF clr)
{
	while(left < right)
	{
		pDC->SetPixel(left, top, clr);
		left += 4;
	}
}

void CXTPDockingPaneVisualStudio2012Theme::DrawDashedLineV(CDC *pDC, int left, int top, int bottom, COLORREF clr)
{
	while(top < bottom)
	{
		pDC->SetPixel(left, top, clr);
		top += 4;
	}
}

void CXTPDockingPaneVisualStudio2012Theme::DrawTitleBar(CDC *pDC, CXTPDockingPaneBase *pPane, CRect rcCaption, BOOL bVertical)
{
	BOOL bActive   = pPane->IsActive();

	CXTPDockingPaneColorSetVisualStudio2012 *pColorSet = bActive ? &m_csActive : &m_csInactive;

	HBRUSH hCustomBrush = GetCustomTitleBrush(bActive, pPane->IsCaptionVertical());
	if(NULL == hCustomBrush)
	{
		if (bVertical || pPane->GetType() == xtpPaneTypeMiniWnd)
		{
			pDC->FillSolidRect(rcCaption, pColorSet->m_clrCaption);
		}
		else
		{
			pDC->FillSolidRect(rcCaption, m_csInactive.m_clrCaption);
			rcCaption.DeflateRect(2,0);
			pDC->FillSolidRect(rcCaption, pColorSet->m_clrCaption);
		}
	}
	else
	{
		pDC->FillRect(rcCaption, CBrush::FromHandle(hCustomBrush));
	}

	rcCaption.left  += m_rcCaptionMargin.left;
	rcCaption.right -= m_rcCaptionMargin.right;

	int nWidth = DrawCaptionText(pDC, pPane->GetTitle(), rcCaption, bVertical, TRUE);

	if(NULL == hCustomBrush)
	{
		CRect rcGripper(rcCaption);
		if (bVertical)
		{
			rcGripper.top  += nWidth + 5;
			rcGripper.left += rcGripper.Width() - 8;
		}
		else
		{
			rcGripper.top   += 8;
			rcGripper.left  += nWidth + 5;
		}
		//rcGripper.right -= 16 * pPane->GetCaptionButtons()->GetSize();
		DrawGripper(pDC, rcGripper, pColorSet->m_clrGripper, bVertical);
	}

	pDC->SetTextColor(pColorSet->m_clrText);
	DrawCaptionText(pDC, pPane->GetTitle(), rcCaption, bVertical, FALSE);
	DrawCaptionIcon(pDC, pPane, rcCaption, bVertical);
	DrawCaptionButtons(pDC, pPane->GetCaptionButtons(), rcCaption, pColorSet->m_clrText, 16, 0, bVertical);
}

// Draw caption when docked
void CXTPDockingPaneVisualStudio2012Theme::DrawCaption(CDC *pDC, CXTPDockingPaneTabbedContainer *pPane, CRect rc)
{
	// Caption
	CRect rcCaption(rc);
	rcCaption.bottom = rcCaption.top + m_nTitleHeight;

	DrawTitleBar(pDC, pPane, rcCaption, FALSE);
}

void CXTPDockingPaneVisualStudio2012Theme::DrawPane(CDC *pDC, CXTPDockingPaneTabbedContainer *pPane, CRect rc)
{
	BOOL bActive   = pPane->IsActive();

	CXTPDockingPaneColorSetVisualStudio2012 *pColorSet = bActive ? &m_csActive : &m_csInactive;

	pDC->FillSolidRect(rc, pColorSet->m_clrBackground);
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

void CXTPDockingPaneVisualStudio2012Theme::DrawSidePanel(CDC *pDC, CXTPDockingPaneSidePanel *pPane, CRect rc)
{
	const XTPDockingPaneDirection direction = pPane->GetDirection();

	const BOOL bVertical = xtpPaneDockLeft  == direction || xtpPaneDockRight == direction;
	const BOOL bActive   = pPane->IsActive();

	CXTPDockingPaneColorSetVisualStudio2012 *pColorSet = bActive ? &m_csActive : &m_csInactive;

	// Border
	CRect rcBorder(rc);
	pDC->Draw3dRect(rcBorder, pColorSet->m_clrBorder, pColorSet->m_clrBorder);

	// Background
	CRect rcBackground(rc);
	rcBackground.DeflateRect(1, 1, 1, 1);
	pDC->FillSolidRect(rcBackground, pColorSet->m_clrBackground);

	CRect rcCaption = GetPaneCaptionRect(pPane);
	DrawTitleBar(pDC, pPane, rcCaption, bVertical);
}

void CXTPDockingPaneVisualStudio2012Theme::DrawFloatingFrame(CDC *pDC, CXTPDockingPaneMiniWnd *pPane, CRect rc)
{
	const BOOL bVertical = pPane->IsCaptionVertical();
	const BOOL bActive   = pPane->IsActive();

	CXTPDockingPaneColorSetVisualStudio2012 *pColorSet = bActive ? &m_csActive : &m_csInactive;

	CString strTitle;
	strTitle = pPane->GetTitle();

	if (bVertical)
	{
		pDC->FillSolidRect(rc, RGB(255,0,0));
	}
	else
	{
		// Border
		CRect rcBorder(rc);
		pDC->Draw3dRect(rcBorder, pColorSet->m_clrBorder, pColorSet->m_clrBorder);

		// Background
		CRect rcBackground(rc);
		rcBackground.DeflateRect(1, 1, 1, 1);
		pDC->FillSolidRect(rcBackground, pColorSet->m_clrBackground);

		// Caption
		CRect rcCaption(rcBackground);
		rcCaption.bottom = rcCaption.top + m_nTitleHeight;

		DrawTitleBar(pDC, pPane, rcCaption, bVertical);
	}
}

void CXTPDockingPaneVisualStudio2012Theme::DrawCaptionButton(CDC *pDC, CXTPDockingPaneCaptionButton *pButton, COLORREF clrButton)
{
	UNUSED_ALWAYS(clrButton);

	BOOL bActive = pButton->GetPane()->IsActive();

	CXTPDockingPaneColorSetVisualStudio2012 *pColorSet = bActive ? &m_csActive : &m_csInactive;
	CXTPColorSetCaptionButton *pColorSetButton;

		 if (pButton->IsPressed())  pColorSetButton = &pColorSet->m_csCaptionButtonPressed;
	else if (pButton->IsSelected()) pColorSetButton = &pColorSet->m_csCaptionButtonHot;
	else                            pColorSetButton = &pColorSet->m_csCaptionButtonNormal;

	CPoint pt = pButton->GetRect().CenterPoint();
	pDC->FillSolidRect(pButton->GetRect(), pColorSetButton->m_clrBackground);
	DrawCaptionButtonEntry(pDC, pButton, pt, pColorSetButton->m_clrFill);
}

void CXTPDockingPaneVisualStudio2012Theme::DrawAutoHidePanel(CDC* pDC, CRect rc, XTPDockingPaneDirection direction)
{
	pDC->FillSolidRect(rc, m_clrBackground);

	switch (direction)
	{
	case xtpPaneDockLeft:
		pDC->FillSolidRect(rc.right-4, rc.top, 4, rc.Height(), m_clrSplitter);
		break;

	case xtpPaneDockTop:
		pDC->FillSolidRect(rc.left, rc.bottom-4, rc.Width(), 4, m_clrSplitter);
		break;

	case xtpPaneDockRight:
		pDC->FillSolidRect(rc.left, rc.top, 4, rc.Height(), m_clrSplitter);
		break;

	case xtpPaneDockBottom:
		pDC->FillSolidRect(rc.left, rc.top, rc.Width(), 4, m_clrSplitter);
		break;
	}
}
