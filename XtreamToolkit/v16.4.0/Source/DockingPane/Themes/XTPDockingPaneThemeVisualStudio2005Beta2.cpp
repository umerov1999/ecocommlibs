// XTPDockingPaneVisualStudio2005Beta2Theme.cpp : implementation of the CXTPDockingPaneVisualStudio2005Beta2Theme class.
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
#include <DockingPane/Themes/XTPDockingPaneThemeVisualStudio2005Beta1.h>
#include <DockingPane/Themes/XTPDockingPaneThemeVisualStudio2005Beta2.h>

#include <DockingPane/TabColorSets/XTPDockingPaneTabColorSetVisualStudio2003.h>
#include <DockingPane/TabColorSets/XTPDockingPaneTabColorSetVisualStudio2005.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//////////////////////////////////////////////////////////////////////////
// CXTPDockingPaneVisualStudio2005Beta2Theme

CXTPDockingPaneVisualStudio2005Beta2Theme::CXTPDockingPaneVisualStudio2005Beta2Theme()
{
	m_pTabPaintManager->SetAppearance(xtpTabThemeVisualStudio2005);
	m_pTabPaintManager->SetColorSet(new CXTPDockingPaneTabColorSetVisualStudio2005(TRUE));

	m_pPanelPaintManager->SetAppearance(xtpTabThemeVisualStudio2005);
	m_pPanelPaintManager->SetColorSet(new CXTPDockingPaneTabColorSetVisualStudio2005(TRUE));
}

void CXTPDockingPaneVisualStudio2005Beta2Theme::RefreshMetrics()
{
	CXTPDockingPaneVisualStudio2005Beta1Theme::RefreshMetrics();

	m_clrNormalCaption.SetStandardValue(GetXtremeColor(COLOR_INACTIVECAPTION));
	m_clrActiveCaption.SetStandardValue(GetXtremeColor(COLOR_ACTIVECAPTION));
	m_clrCaptionBorder = GetXtremeColor(COLOR_3DSHADOW);

	switch (XTPColorManager()->GetCurrentSystemTheme())
	{
	case xtpSystemThemeBlue:
		m_clrNormalCaption.SetStandardValue(RGB(204, 199, 186));
		m_clrActiveCaption.SetStandardValue(RGB(59, 128, 237), RGB(49, 106, 197));
		m_clrNormalCaptionText.SetStandardValue(0);
		break;
	case xtpSystemThemeOlive:
		m_clrNormalCaption.SetStandardValue(RGB(204, 199, 186));
		m_clrActiveCaption.SetStandardValue(RGB(182, 195, 146), RGB(145, 160, 117));
		m_clrNormalCaptionText.SetStandardValue(0);
		break;
	case xtpSystemThemeSilver:
		m_clrNormalCaption.SetStandardValue(RGB(240, 240, 245));
		m_clrActiveCaption.SetStandardValue(RGB(211, 212, 221), RGB(166, 165, 191));
		m_clrNormalCaptionText.SetStandardValue(0);
		m_clrCaptionBorder = RGB(145, 155, 156);
		break;
	}
}

COLORREF CXTPDockingPaneVisualStudio2005Beta2Theme::FillCaptionPart(CDC* pDC, CXTPDockingPaneBase* pPane, CRect rcCaption, BOOL bActive, BOOL bVertical)
{
	HBRUSH hCustomBrush = GetCustomTitleBrush(bActive, bVertical);
	if(NULL == hCustomBrush)
	{
		XTPDrawHelpers()->GradientFill(pDC, rcCaption, bActive ? m_clrActiveCaption : m_clrNormalCaption, bVertical);
	}
	else
	{
		pDC->FillRect(rcCaption, CBrush::FromHandle(hCustomBrush));
	}

	if (pPane->GetType() == xtpPaneTypeSidePanel)
		return bActive ? m_clrActiveCaptionText : m_clrNormalCaptionText;

	CXTPPenDC pen(*pDC, m_clrCaptionBorder);
	if (bVertical)
	{
		pDC->MoveTo(rcCaption.right, rcCaption.bottom - 1);
		pDC->LineTo(rcCaption.left , rcCaption.bottom - 1);
		pDC->LineTo(rcCaption.left, rcCaption.top);
		pDC->LineTo(rcCaption.right, rcCaption.top);

	}
	else
	{
		pDC->MoveTo(rcCaption.left, rcCaption.bottom);
		pDC->LineTo(rcCaption.left , rcCaption.top);
		pDC->LineTo(rcCaption.right - 1, rcCaption.top);
		pDC->LineTo(rcCaption.right - 1, rcCaption.bottom - 1);
	}

	return bActive ? m_clrActiveCaptionText : m_clrNormalCaptionText;
}
