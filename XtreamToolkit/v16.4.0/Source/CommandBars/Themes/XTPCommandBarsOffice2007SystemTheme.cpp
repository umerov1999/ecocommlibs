// XTPCommandBarsOffice2007SystemTheme.cpp : implementation of the CXTPCommandBarsOffice2007SystemTheme class.
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

#include <Common/XTPImageManager.h>
#include <Common/XTPDrawHelpers.h>
#include <Common/XTPHookManager.h>
#include <Common/XTPColorManager.h>
#include <Common/XTPSystemHelpers.h>
#include <Common/XTPWinThemeWrapper.h>
#include <Common/XTPTheme.h>
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
#include <CommandBars/XTPCommandBars.h>
#include <CommandBars/XTPPaintManager.h>

// Frame
#include <CommandBars/Frame/XTPFramePaintManager.h>
#ifdef _XTP_INCLUDE_RIBBON
#include <Ribbon/XTPRibbonMarkupFrameTheme.h>
#endif
#include <CommandBars/Frame/XTPFrameThemeOffice2007System.h>

// CommandBars
#include <CommandBars/Themes/XTPCommandBarsOfficeXPTheme.h>
#include <CommandBars/Themes/XTPCommandBarsOffice2007SystemTheme.h>

#ifdef _XTP_INCLUDE_RIBBON
// Ribbon
#include <Ribbon/XTPRibbonPaintManager.h>
#include <Ribbon/XTPRibbonMarkupTheme.h>
#include <Ribbon/Themes/XTPRibbonThemeOffice2007System.h>
#endif


//////////////////////////////////////////////////////////////////////
// CXTPCommandBarsOffice2007SystemTheme

IMPLEMENT_DYNAMIC(CXTPCommandBarsOffice2007SystemTheme, CXTPCommandBarsOfficeXPTheme)

CXTPCommandBarsOffice2007SystemTheme::CXTPCommandBarsOffice2007SystemTheme()
{
	m_pMarkupContext = XTPMarkupCreateContext(0);

	// Frame paint manager
	CXTPFrameThemeOffice2007System *pFramePaintManager = new CXTPFrameThemeOffice2007System(this);
	SAFE_DELETE(m_pFramePaintManager);
	m_pFramePaintManager = pFramePaintManager;

	// Ribbon paint manager
	CXTPRibbonThemeOffice2007System *pRibbonPaintManager =
		new CXTPRibbonThemeOffice2007System(this);
	SAFE_DELETE(m_pRibbonPaintManager);
	m_pRibbonPaintManager = pRibbonPaintManager;
}

CXTPCommandBarsOffice2007SystemTheme::~CXTPCommandBarsOffice2007SystemTheme()
{
}

CXTPFramePaintManager* CXTPCommandBarsOffice2007SystemTheme::GetFramePaintManager()
{
	return m_pFramePaintManager;
}

CXTPRibbonPaintManager* CXTPCommandBarsOffice2007SystemTheme::GetRibbonPaintManager()
{
	return m_pRibbonPaintManager;
}

void CXTPCommandBarsOffice2007SystemTheme::RefreshMetrics()
{
	CXTPCommandBarsOfficeXPTheme::RefreshMetrics();
}
