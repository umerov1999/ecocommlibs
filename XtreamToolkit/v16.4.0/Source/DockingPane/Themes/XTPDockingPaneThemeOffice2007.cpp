// XTPDockingPaneOffice2007Theme.cpp : implementation of the CXTPDockingPaneOffice2007Theme class.
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
#include <DockingPane/Themes/XTPDockingPaneThemeOffice2003.h>
#include <DockingPane/Themes/XTPDockingPaneThemeOffice2007.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//////////////////////////////////////////////////////////////////////////
// CXTPDockingPaneOffice2007Theme

CXTPDockingPaneOffice2007Theme::CXTPDockingPaneOffice2007Theme()
{
	m_bRoundedCaption = FALSE;
	m_dwSplitterStyle = xtpPaneSplitterGradientVert;
	m_bDrawGripper = FALSE;

	m_pPanelPaintManager->SetAppearance(xtpTabAppearancePropertyPage2007);
	m_pPanelPaintManager->m_bHotTracking = TRUE;

	m_pTabPaintManager->SetAppearance(xtpTabAppearancePropertyPage2007);
	m_pTabPaintManager->m_bHotTracking = TRUE;

	m_pPanelPaintManager->m_rcButtonMargin.SetRect(2, 1, 2, 1);
	m_pTabPaintManager->m_rcButtonMargin.SetRect(2, 1, 2, 1);

	m_nCaptionFontGap = 5;
}

void CXTPDockingPaneOffice2007Theme::DrawCaption(CDC* pDC, CXTPDockingPaneTabbedContainer* pPane, CRect rc)
{
	BOOL bVertical = pPane->IsCaptionVertical();

	int nTitleHeight = m_nTitleHeight;
	CRect rcCaption(rc);

	if (bVertical)
	{
		rcCaption.DeflateRect(0, 0, rc.Width() - nTitleHeight - 3, 0);
	}
	else
	{
		rcCaption.DeflateRect(0, 0, 0, rc.Height() - nTitleHeight - 3);
	}

	BOOL bActive = m_bHighlightActiveCaption && pPane->IsActive();

	HBRUSH hCustomBrush = GetCustomTitleBrush(bActive, bVertical);
	if(NULL == hCustomBrush)
	{
		XTPDrawHelpers()->GradientFill(pDC, rcCaption,  bActive ? m_clrActiveCaption : m_clrNormalCaption, bVertical);
	}
	else
	{
		pDC->FillRect(rcCaption, CBrush::FromHandle(hCustomBrush));
	}

	DrawCaptionPart(pDC, pPane, rcCaption, pPane->GetTitle(), bActive, bVertical);
}

void CXTPDockingPaneOffice2007Theme::RefreshMetrics()
{
	CXTPDockingPaneOffice2003Theme::RefreshMetrics();

	CXTPResourceImages* pImages = XTPResourceImages();

	if (pImages->IsValid())
	{
		m_arrColor[XPCOLOR_FRAME] = pImages->GetImageColor(_T("Window"), _T("WindowFrame"));
		m_arrColor[XPCOLOR_3DFACE] = pImages->GetImageColor(_T("Window"), _T("ButtonFace"));
		m_arrColor[COLOR_3DHIGHLIGHT] = pImages->GetImageColor(_T("Window"), _T("EdgeHighLightColor"));

		m_clrSplitter.SetStandardValue(pImages->GetImageColor(_T("DockingPane"), _T("Splitter")));
		m_clrSplitterGripper.SetStandardValue(m_arrColor[XPCOLOR_FRAME]);

		m_arrColor[XPCOLOR_TOOLBAR_GRIPPER] = pImages->GetImageColor(_T("Toolbar"), _T("ToolbarGripper"));
		m_arrColor[XPCOLOR_MENUBAR_BORDER] = pImages->GetImageColor(_T("Toolbar"), _T("MenuPopupBorder"));

		m_bLunaTheme = TRUE;

		m_clrSplitterGradient.SetStandardValue(pImages->GetImageColor(_T("DockingPane"), _T("SplitterLight")),
			pImages->GetImageColor(_T("DockingPane"), _T("SplitterDark")));

		m_clrNormalCaption.SetStandardValue(pImages->GetImageColor(_T("DockingPane"), _T("NormalCaptionLight")),
			pImages->GetImageColor(_T("DockingPane"), _T("NormalCaptionDark")), 0.25f);
		m_clrNormalCaptionText.SetStandardValue(pImages->GetImageColor(_T("DockingPane"), _T("NormalCaptionText")));

		m_clrActiveCaption.SetStandardValue(pImages->GetImageColor(_T("DockingPane"), _T("ActiveCaptionLight")),
			pImages->GetImageColor(_T("DockingPane"), _T("ActiveCaptionDark")), 0.25f);
		m_clrActiveCaptionText.SetStandardValue(pImages->GetImageColor(_T("DockingPane"), _T("ActiveCaptionText")));

	}
	else
	{
		m_arrColor[XPCOLOR_FRAME] = RGB(101, 147, 207);
		m_arrColor[XPCOLOR_3DFACE] = RGB(191, 219, 255);
		m_arrColor[COLOR_3DHIGHLIGHT] = RGB(255, 255, 255);

		m_clrNormalCaption.SetStandardValue(RGB(227, 239, 255), RGB(175, 210, 255), 0.25f);
		m_clrSplitter.SetStandardValue(RGB(216, 231, 252));
		m_clrSplitterGripper.SetStandardValue(GetXtremeColor(XPCOLOR_TOOLBAR_GRIPPER));
		m_clrSplitterGradient.SetStandardValue(RGB(190, 218, 254), RGB(100, 143, 203));

		INT nElements[] =
		{
			XPCOLOR_TOOLBAR_GRIPPER, XPCOLOR_SEPARATOR, XPCOLOR_DISABLED, XPCOLOR_MENUBAR_FACE, XPCOLOR_MENUBAR_BORDER, XPCOLOR_HIGHLIGHT, XPCOLOR_HIGHLIGHT_BORDER, XPCOLOR_HIGHLIGHT_PUSHED_BORDER, XPCOLOR_HIGHLIGHT_CHECKED_BORDER, XPCOLOR_HIGHLIGHT_PUSHED, XPCOLOR_HIGHLIGHT_CHECKED, XPCOLOR_TOOLBAR_FACE, XPCOLOR_PUSHED_TEXT
		};
		COLORREF clrValues[] =
		{
			RGB(39, 65, 118), RGB(106, 140, 203), RGB(109, 150, 208), RGB(246, 246, 246), RGB(0, 45, 150), RGB(255, 238, 194), RGB(0, 0, 128), RGB(0, 0, 128), RGB(0, 0, 128), RGB(254, 128, 62), RGB(255, 192, 111), RGB(158, 190, 245), 0
		};
		SetColors(sizeof(nElements)/sizeof(INT), nElements, clrValues);

		m_bLunaTheme = TRUE;

		m_clrActiveCaption.SetStandardValue(RGB(255, 231, 162), RGB(255, 166, 76));
		m_clrNormalCaptionText.SetStandardValue(RGB(8, 55, 114));
		m_clrActiveCaptionText.SetStandardValue(RGB(8, 55, 114));
	}
}
