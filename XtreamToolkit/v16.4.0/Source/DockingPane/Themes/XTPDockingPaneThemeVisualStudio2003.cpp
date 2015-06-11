// XTPDockingPaneVisualStudio2003Theme.cpp : implementation of the CXTPDockingPaneVisualStudio2003Theme class.
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

#include <DockingPane/TabColorSets/XTPDockingPaneTabColorSetVisualStudio2003.h>
#include <DockingPane/TabColorSets/XTPDockingPaneTabColorSetOfficePanel.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


///////////////////////////////////////////////////////////////////////////////
// CXTPDockingPaneVisualStudio2003Theme

CXTPDockingPaneVisualStudio2003Theme::CXTPDockingPaneVisualStudio2003Theme()
{
	m_pPanelPaintManager->SetAppearance(xtpTabAppearanceFlat);
	m_pPanelPaintManager->SetColorSet(new CXTPDockingPaneTabColorSetOfficePanel());
	m_pTabPaintManager->SetAppearance(xtpTabAppearanceVisualStudio);
	m_bGradientCaption = FALSE;
	m_bUseBoldCaptionFont = FALSE;
	m_nCaptionButtonSize = 14;
}

void CXTPDockingPaneVisualStudio2003Theme::RefreshMetrics()
{
	CXTPDockingPaneGripperedTheme::RefreshMetrics();

	m_clrActiveCaption.SetStandardValue(GetXtremeColor(COLOR_ACTIVECAPTION), GetXtremeColor(27));
	m_clrNormalCaption.SetStandardValue(GetXtremeColor(COLOR_INACTIVECAPTION), GetSysColor(28));

	m_clrNormalCaptionText.SetStandardValue(GetXtremeColor(COLOR_BTNTEXT));
	m_clrActiveCaptionText.SetStandardValue(GetXtremeColor(COLOR_CAPTIONTEXT));

	if (XTPSystemVersion()->IsWin95())
	{
		// Windows 95
		m_clrNormalCaption.SetStandardValue(GetXtremeColor(COLOR_INACTIVECAPTION));
		m_clrActiveCaption.SetStandardValue(GetXtremeColor(COLOR_ACTIVECAPTION));
	}
}

COLORREF CXTPDockingPaneVisualStudio2003Theme::FillCaptionPart(CDC* pDC, CXTPDockingPaneBase* pPane, CRect rcCaption, BOOL bActive, BOOL bVertical)
{
	COLORREF crText;
	HBRUSH hCustomBrush = GetCustomTitleBrush(bActive, bVertical);
	if(NULL == hCustomBrush)
	{
		if (m_bGradientCaption)
		{
			XTPDrawHelpers()->GradientFill(pDC, rcCaption, bActive ? m_clrActiveCaption : m_clrNormalCaption, !bVertical);
		}
		else
		{
			if (bActive)
			{
				pDC->FillSolidRect(rcCaption, m_clrActiveCaption.clrLight);
			}
			else if (pPane->GetType() != xtpPaneTypeSidePanel)
			{
				CXTPPenDC pen(*pDC, GetXtremeColor(XPCOLOR_3DSHADOW));
				pDC->MoveTo(rcCaption.left + 1, rcCaption.top); pDC->LineTo(rcCaption.right - 1 , rcCaption.top);
				pDC->MoveTo(rcCaption.left, rcCaption.top + 1); pDC->LineTo(rcCaption.left, rcCaption.bottom - 1);
				pDC->MoveTo(rcCaption.left + 1, rcCaption.bottom - 1); pDC->LineTo(rcCaption.right - 1 , rcCaption.bottom - 1);
				pDC->MoveTo(rcCaption.right -1 , rcCaption.top + 1); pDC->LineTo(rcCaption.right - 1, rcCaption.bottom - 1);
			}
		}

		crText = bActive ? m_clrActiveCaptionText : m_clrNormalCaptionText;
	}
	else
	{
		pDC->FillRect(rcCaption, CBrush::FromHandle(hCustomBrush));
		crText = m_clrNormalCaptionText;
	}

	return crText;
}

void CXTPDockingPaneVisualStudio2003Theme::DrawCaptionPart(CDC* pDC, CXTPDockingPaneBase* pPane, CRect rcCaption, const CString& strTitle, BOOL bActive, BOOL bVertical)
{
	COLORREF clrButton = FillCaptionPart(pDC, pPane, rcCaption, bActive, bVertical);

	COLORREF clrText = bActive ? m_clrActiveCaptionText : m_clrNormalCaptionText;

	XTPDeflateRectDirection(rcCaption, 0, 0, 2, 0, bVertical);
	DrawCaptionButtons(pDC, pPane->GetCaptionButtons(), rcCaption, clrButton, m_nCaptionButtonSize, 2, bVertical);

	// Title text
	pDC->SetTextColor(IsCaptionEnabled(pPane) ? clrText : m_clrDisabledCaptionText);
	XTPDeflateRectDirection(rcCaption, 4, 1, 0, 0, bVertical);

	DrawCaptionIcon(pDC, pPane, rcCaption, bVertical);
	DrawCaptionText(pDC, strTitle, rcCaption, bVertical);
}
