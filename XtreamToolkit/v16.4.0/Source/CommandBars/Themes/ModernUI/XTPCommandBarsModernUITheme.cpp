// XTPCommandBarsOffice2013Theme.cpp : implementation of the CXTPCommandBarsOffice2013Theme class.
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

#include <GraphicLibrary/unzip/unzip.h>
#include <Common/XTPMarkupTheme.h>
#include <Common/XTPResource.h>

#include <Common/XTPImageManager.h>
#include <Common/XTPResourceImage.h>
#include <Common/XTPDrawHelpers.h>
#include <Common/XTPHookManager.h>
#include <Common/XTPColorManager.h>
#include <Common/XTPSystemHelpers.h>
#include <Common/XTPWinThemeWrapper.h>
#include <Common/XTPMarkupRender.h>
// ScrollBar
#include <Common/ScrollBar/XTPScrollBase.h>
#include <Common/ScrollBar/XTPScrollInfo.h>
#include <Common/ScrollBar/XTPScrollBarPaintManager.h>

#include <CommandBars/XTPCommandBarsDefines.h>
#include <CommandBars/XTPCommandBar.h>
#include <CommandBars/XTPPopupBar.h>
#include <CommandBars/XTPControl.h>
#include <CommandBars/XTPControlButton.h>
#include <CommandBars/XTPControlPopup.h>
#include <CommandBars/XTPControlGallery.h>
#include <CommandBars/XTPControlEdit.h>
#include <CommandBars/XTPControlComboBox.h>
#include <CommandBars/XTPCommandBars.h>
#include <CommandBars/StatusBar/XTPStatusBar.h>
#include <CommandBars/XTPPaintManager.h>
#include <CommandBars/Frame/XTPFramePaintManager.h>

#include <CommandBars/Themes/XTPCommandBarsOfficeXPTheme.h>
#include <CommandBars/Themes/ModernUI/XTPCommandBarsModernUITheme.h>


//////////////////////////////////////////////////////////////////////
// CXTPCommandBarsModernUITheme

IMPLEMENT_DYNAMIC(CXTPCommandBarsModernUITheme, CXTPCommandBarsOfficeXPTheme)

CXTPCommandBarsModernUITheme::CXTPCommandBarsModernUITheme()
	: m_pImages       (NULL)
	, m_pMarkupContext(NULL)
{
	m_pImages        = XTPResourceImages();
	m_pMarkupContext = XTPMarkupCreateContext(0);
}

CXTPCommandBarsModernUITheme::~CXTPCommandBarsModernUITheme()
{
	XTPMarkupReleaseContext(m_pMarkupContext);
}

void CXTPCommandBarsModernUITheme::RefreshMetrics()
{
	CXTPCommandBarsOfficeXPTheme::RefreshMetrics();

	m_pShadowManager->SetShadowOptions(0);

	m_iconsInfo.bUseFadedIcons   = FALSE;
	m_iconsInfo.bUseDisabledIcons = TRUE;
	m_iconsInfo.bIconsWithShadow = FALSE;
	m_iconsInfo.bOfficeStyleDisabledIcons = FALSE;

	// Status bar
	m_clrStatusBarBackground = m_pImages->GetImageColor(_T("StatusBar"), _T("BackgroundColor"));
	m_clrStatusBarText       = m_pImages->GetImageColor(_T("StatusBar"), _T("TextColor"));

	// Message bar
	m_rcMessageBarMargin = CRect(0, 0, 0, 0);


	// Backwards compatibility
	m_clrStatusTextColor     = m_clrStatusBarText;
	m_clrStatusTextGrayColor = m_clrStatusBarBackground;
}

CXTPFramePaintManager* CXTPCommandBarsModernUITheme::GetFramePaintManager()
{
	return m_pFramePaintManager;
}

CXTPRibbonPaintManager* CXTPCommandBarsModernUITheme::GetRibbonPaintManager()
{
#ifdef _XTP_INCLUDE_RIBBON
	return m_pRibbonPaintManager;
#else
	return NULL;
#endif
}

CXTPMarkupContext* CXTPCommandBarsModernUITheme::GetMarkupContext()
{
	return m_pMarkupContext;
}
