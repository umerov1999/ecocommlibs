// XTPResourceTheme.cpp : implementation of the CXTPResourceTheme class.
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

#include <Common/XTPDrawHelpers.h>
#include <Common/XTPResourceImage.h>
#include <Common/XTPHookManager.h>
#include <Common/XTPMarkupRender.h>
#include <Common/XTPResourceManager.h>
#include <Common/XTPSystemHelpers.h>
#include <Common/XTPColorManager.h>
#include <Common/XTPWinThemeWrapper.h>
#include <Common/XTPVC80Helpers.h>
#include <Common/ScrollBar/XTPScrollBase.h>
#include <Common/ScrollBar/XTPScrollInfo.h>
#include <Common/ScrollBar/XTPScrollBarPaintManager.h>
#include <Common/ScrollBar/Themes/XTPScrollBarThemeResource.h>

#include <CommandBars/XTPCommandBarsDefines.h>
#include <CommandBars/XTPCommandBar.h>
#include <CommandBars/XTPPopupBar.h>
#include <CommandBars/XTPControls.h>
#include <CommandBars/XTPControl.h>
#include <CommandBars/XTPControlButton.h>
#include <CommandBars/XTPControlPopup.h>
#include <CommandBars/XTPControlGallery.h>
#include <CommandBars/XTPPaintManager.h>


// Progress
#include <CommandBars/Progress/XTPProgressPaintManager.h>
#include <CommandBars/Progress/XTPProgressThemeResource.h>

// Slider
#include <CommandBars/Slider/XTPSliderPaintManager.h>
#include <CommandBars/Slider/XTPSliderThemeResource.h>

// Message bar
#include <CommandBars/MessageBar/XTPMessageBar.h>
#include <CommandBars/MessageBar/XTPMessageBarPaintManager.h>
#include <CommandBars/MessageBar/XTPMessageBarThemeResource.h>

// Status bar
#include <CommandBars/StatusBar/XTPStatusBar.h>
#include <CommandBars/StatusBar/XTPStatusBarPaintManager.h>
#include <CommandBars/StatusBar/XTPStatusBarThemeResource.h>

// Frame
#include <CommandBars/Frame/XTPFramePaintManager.h>
#include <CommandBars/Frame/XTPFrameThemeOffice2007.h>

#include <CommandBars/Themes/XTPCommandBarsOfficeXPTheme.h>
#include <CommandBars/Themes/XTPCommandBarsOffice2003Theme.h>
#include <CommandBars/Themes/XTPCommandBarsResourceTheme.h>

#include <CommandBars/Themes/Resource/XTPControlGalleryResourceTheme.h>


#ifdef _XTP_INCLUDE_RIBBON
#include "Ribbon/XTPRibbonPaintManager.h"
#include <Ribbon/Themes/XTPRibbonThemeResource.h>
#endif


#ifndef OIC_WINLOGO
#define OIC_WINLOGO 32517
#endif

#ifndef LAYOUT_BITMAPORIENTATIONPRESERVED
#define LAYOUT_BITMAPORIENTATIONPRESERVED 0x00000008
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CXTPResourceTheme, CXTPOffice2003Theme)

CXTPResourceTheme::CXTPResourceTheme()
{
	m_pShadowManager->SetShadowOptions(xtpShadowOfficeAlpha | xtpShadowShowPopupControl);

	m_systemTheme = xtpSystemThemeUnknown;
	m_bThemedStatusBar = TRUE;
	m_bOffice2007Padding = TRUE;

	SAFE_DELETE(m_pGalleryPaintManager);
	m_pGalleryPaintManager = new CXTPControlGalleryResourceTheme(this);

	SAFE_DELETE(m_pSliderPaintManager);
	m_pSliderPaintManager = new CXTPSliderResourceTheme(this);

	SAFE_DELETE(m_pProgressPaintManager);
	m_pProgressPaintManager = new CXTPProgressResourceTheme(this);

	SAFE_DELETE(m_pScrollBarPaintManager);
	m_pScrollBarPaintManager = new CXTPScrollBarThemeResource();

	SAFE_DELETE(m_pStatusBarPaintManager);
	m_pStatusBarPaintManager = new CXTPStatusBarThemeResource(this);

	// Message bar paint manager
	SAFE_DELETE(m_pMessageBarPaintManager);
	m_pMessageBarPaintManager = new CXTPMessageBarThemeResource(this);


	m_pImages = XTPResourceImages();
}

CXTPFramePaintManager* CXTPResourceTheme::GetFramePaintManager()
{
	if (!m_pFramePaintManager)
	{
		m_pFramePaintManager = new CXTPRibbonOffice2007FrameTheme(this);
		m_pFramePaintManager->RefreshMetrics();
	}
	return m_pFramePaintManager;
}

CXTPRibbonPaintManager* CXTPResourceTheme::GetRibbonPaintManager()
{
#ifdef _XTP_INCLUDE_RIBBON
	if (!m_pRibbonPaintManager)
	{
		m_pRibbonPaintManager = new CXTPRibbonThemeResource(this);
		m_pRibbonPaintManager->RefreshMetrics();
	}
#endif
	return m_pRibbonPaintManager;
}



CXTPResourceTheme::~CXTPResourceTheme()
{
	m_pImages->RemoveAll();
}

CXTPResourceImages* CXTPResourceTheme::GetImages() const
{
	return m_pImages;
}

void CXTPResourceTheme::SetImages(CXTPResourceImages* pImages)
{
	m_pImages = pImages;
}

void CXTPResourceTheme::SetImageHandle(HMODULE hResource, LPCTSTR lpszIniFileName)
{
	GetImages()->SetHandle(hResource, lpszIniFileName);
	RefreshMetrics();
}

CXTPResourceImage* CXTPResourceTheme::LoadImage(LPCTSTR lpszFileName)
{
	return GetImages()->LoadFile(lpszFileName);
}

BOOL CXTPResourceTheme::IsImagesAvailable()
{
	return TRUE;
}

COLORREF CXTPResourceTheme::GetRectangleTextColor(BOOL bSelected, BOOL bPressed, BOOL bEnabled, BOOL bChecked, BOOL bPopuped, XTPBarType barType, XTPBarPosition barPosition)
{
	if (barType == xtpBarTypeMenuBar && !bSelected && bEnabled && !bPressed && !bChecked && !bPopuped)
	{
		return m_clrMenuBarText;
	}
	return CXTPOffice2003Theme::GetRectangleTextColor(bSelected, bPressed, bEnabled, bChecked, bPopuped, barType, barPosition);
}

void CXTPResourceTheme::RefreshMetrics()
{
	CXTPOffice2003Theme::RefreshMetrics();

	if (!GetImages()->IsValid())
		return;

	m_clrStatusTextColor = GetImages()->GetImageColor(_T("StatusBar"), _T("StatusBarText"));
	m_clrStatusTextGrayColor = GetImages()->GetImageColor(_T("StatusBar"), _T("StatusBarTextGray"));
	m_clrStatusBarShadow = GetImages()->GetImageColor(_T("StatusBar"), _T("StatusBarShadow"));

	m_clrStatusBarTop.SetStandardValue(GetImages()->GetImageColor(_T("StatusBar"), _T("StatusBarFaceTopLight")),
		GetImages()->GetImageColor(_T("StatusBar"), _T("StatusBarFaceTopDark")));
	m_clrStatusBarBottom.SetStandardValue(GetImages()->GetImageColor(_T("StatusBar"), _T("StatusBarFaceBottomLight")),
		GetImages()->GetImageColor(_T("StatusBar"), _T("StatusBarFaceBottomDark")));



	m_clrDockBar.SetStandardValue(GetImages()->GetImageColor(_T("Toolbar"), _T("DockBarFace")));
	m_clrCommandBar.SetStandardValue(GetImages()->GetImageColor(_T("Toolbar"), _T("ToolbarFaceLight")),
		GetImages()->GetImageColor(_T("Toolbar"), _T("ToolbarFaceDark")), 0.75f);

	m_clrToolbarShadow.SetStandardValue(GetImages()->GetImageColor(_T("Toolbar"), _T("ToolbarFaceShadow")));

	m_clrToolbarExpand.SetStandardValue(GetImages()->GetImageColor(_T("Toolbar"), _T("ControlToolbarExpandLight")),
		GetImages()->GetImageColor(_T("Toolbar"), _T("ControlToolbarExpandDark")), 0.75f);


	m_clrPopupControl.SetStandardValue(GetImages()->GetImageColor(_T("Toolbar"), _T("ControlHighlightPopupedLight")),
		GetImages()->GetImageColor(_T("Toolbar"), _T("ControlHighlightPopupedDark")));
	m_clrMenuGripper.SetStandardValue(GetImages()->GetImageColor(_T("Toolbar"), _T("MenuPopupGripperLight")),
		GetImages()->GetImageColor(_T("Toolbar"), _T("MenuPopupGripperDark")));
	m_clrMenuExpandedGripper.SetStandardValue(GetImages()->GetImageColor(_T("Toolbar"), _T("MenuPopupExpandedGripperLight")),
		GetImages()->GetImageColor(_T("Toolbar"), _T("MenuPopupExpandedGripperDark")));
	m_clrMenuExpand = m_clrPopupControl;

	m_pShadowManager->SetShadowColor(0);
	m_clrTearOffGripper.SetStandardValue(GetImages()->GetImageColor(_T("Toolbar"), _T("MenuPopupTearOffGripper")));

	m_clrMenuBarText = GetImages()->GetImageColor(_T("Toolbar"), _T("MenuBarText"));

	m_arrColor[XPCOLOR_MENUBAR_FACE].SetStandardValue(RGB(246, 246, 246));
	m_arrColor[XPCOLOR_MENUBAR_BORDER].SetStandardValue(GetImages()->GetImageColor(_T("Toolbar"), _T("MenuPopupBorder")));
	m_arrColor[XPCOLOR_TOOLBAR_GRIPPER].SetStandardValue(GetImages()->GetImageColor(_T("Toolbar"), _T("ToolbarGripper")));
	m_arrColor[XPCOLOR_MENUBAR_TEXT].SetStandardValue(0);
	m_arrColor[XPCOLOR_HIGHLIGHT_TEXT].SetStandardValue(0);
	m_arrColor[XPCOLOR_TOOLBAR_TEXT].SetStandardValue(0);

	m_arrColor[XPCOLOR_TOOLBAR_GRAYTEXT].SetStandardValue(RGB(141, 141, 141));
	m_arrColor[XPCOLOR_HIGHLIGHT_DISABLED_BORDER].SetStandardValue(RGB(141, 141, 141));
	m_arrColor[XPCOLOR_MENUBAR_GRAYTEXT].SetStandardValue(RGB(141, 141, 141));

	m_arrColor[XPCOLOR_FRAME].SetStandardValue(GetImages()->GetImageColor(_T("Window"), _T("WindowFrame")));

	m_clrFloatingGripper.SetStandardValue(GetImages()->GetImageColor(_T("Toolbar"), _T("FloatingBarGripper")));
	m_clrFloatingGripperText.SetStandardValue(GetImages()->GetImageColor(_T("Toolbar"), _T("FloatingBarGripperText")));
	m_arrColor[XPCOLOR_FLOATBAR_BORDER].SetStandardValue(GetImages()->GetImageColor(_T("Toolbar"), _T("FloatingBarBorder")));

	m_arrColor[COLOR_APPWORKSPACE].SetStandardValue(GetImages()->GetImageColor(_T("Workspace"), _T("AppWorkspace")));
	m_arrColor[XPCOLOR_3DFACE].SetStandardValue(GetImages()->GetImageColor(_T("Window"), _T("ButtonFace")));
	m_arrColor[XPCOLOR_3DSHADOW].SetStandardValue(m_clrToolbarShadow);

	m_arrColor[XPCOLOR_TOOLBAR_FACE].SetStandardValue(GetImages()->GetImageColor(_T("Toolbar"), _T("ToolbarFace")));
	m_arrColor[XPCOLOR_SEPARATOR].SetStandardValue(GetImages()->GetImageColor(_T("Toolbar"), _T("ToolbarSeparator")));
	m_arrColor[XPCOLOR_DISABLED].SetStandardValue(GetImages()->GetImageColor(_T("Toolbar"), _T("ControlDisabledIcon")));

	m_clrWorkspaceClientTop = GetImages()->GetImageColor(_T("Workspace"), _T("WorkspaceClientTop"));
	m_clrWorkspaceClientMiddle = GetImages()->GetImageColor(_T("Workspace"), _T("WorkspaceClientMiddle"));
	m_clrWorkspaceClientBottom =  GetImages()->GetImageColor(_T("Workspace"), _T("WorkspaceClientBottom"));


	m_arrColor[XPCOLOR_HIGHLIGHT].SetStandardValue(GetImages()->GetImageColor(_T("Window"), _T("HighlightSelected")));
	m_arrColor[XPCOLOR_HIGHLIGHT_BORDER].SetStandardValue(GetImages()->GetImageColor(_T("Window"), _T("HighlightSelectedBorder")));

	m_arrColor[XPCOLOR_HIGHLIGHT_PUSHED].SetStandardValue(GetImages()->GetImageColor(_T("Window"), _T("HighlightPressed")));
	m_arrColor[XPCOLOR_HIGHLIGHT_PUSHED_BORDER].SetStandardValue(GetImages()->GetImageColor(_T("Window"), _T("HighlightPressedBorder")));

	m_arrColor[XPCOLOR_HIGHLIGHT_CHECKED].SetStandardValue(GetImages()->GetImageColor(_T("Window"), _T("HighlightChecked")));
	m_arrColor[XPCOLOR_HIGHLIGHT_CHECKED_BORDER].SetStandardValue(GetImages()->GetImageColor(_T("Window"), _T("HighlightCheckedBorder")));

	m_arrColor[XPCOLOR_PUSHED_TEXT].SetStandardValue(0);

	m_bLunaTheme = TRUE;

	m_grcLunaSelected.SetStandardValue(GetImages()->GetImageColor(_T("Window"), _T("HighlightSelectedLight")),
		GetImages()->GetImageColor(_T("Window"), _T("HighlightSelectedDark")));
	m_grcLunaChecked.SetStandardValue(GetImages()->GetImageColor(_T("Window"), _T("HighlightCheckedLight")),
		GetImages()->GetImageColor(_T("Window"), _T("HighlightCheckedDark")));
	m_grcLunaPushed.SetStandardValue(GetImages()->GetImageColor(_T("Window"), _T("HighlightPressedLight")),
		GetImages()->GetImageColor(_T("Window"), _T("HighlightPressedDark")));



	m_arrColor[XPCOLOR_LABEL].SetStandardValue(GetImages()->GetImageColor(_T("Toolbar"), _T("ControlLabel")));
	m_arrColor[XPCOLOR_EDITCTRLBORDER].SetStandardValue(GetImages()->GetImageColor(_T("Toolbar"), _T("ControlEditBorder")));

	m_clrDisabledIcon.SetStandardValue(GetImages()->GetImageColor(_T("Toolbar"), _T("ControlDisabledIconLight")),
		GetImages()->GetImageColor(_T("Toolbar"), _T("ControlDisabledIconDark")));

	m_clrMessageBar.SetStandardValue(GetImages()->GetImageColor(_T("MessageBar"), _T("MessageBarLight")),
		GetImages()->GetImageColor(_T("MessageBar"), _T("MessageBarDark")));
	m_clrMessageBarFrame = GetImages()->GetImageColor(_T("MessageBar"), _T("MessageBarFrame"));
	m_clrMessageBarText = GetImages()->GetImageColor(_T("MessageBar"), _T("MessageBarText"));
	m_clrMessageBarFace = GetImages()->GetImageColor(_T("MessageBar"), _T("MessageBar"));

	CreateGradientCircle();

	m_clrStatusGripperLite = RGB(255, 255, 255);
	m_clrStatusGripperDark = GetXtremeColor(XPCOLOR_SEPARATOR);
}





void CXTPResourceTheme::FillWorkspace(CDC* pDC, CRect rc, CRect rcExclude)
{
	if(!XTPResourceImages()->IsValid())
			return;

	CRgn rgn;
	rgn.CreateRectRgnIndirect(rc);
	pDC->SelectClipRgn(&rgn);

	pDC->ExcludeClipRect(rcExclude);

	CXTPResourceImage* pImage = LoadImage(_T("WORKSPACETOPLEFT"));
	ASSERT(pImage);
	if (!pImage)
		return;

	CRect rcSrc(pImage->GetSource());
	CRect rcTopLeft(rc);
	rcTopLeft.bottom = rcTopLeft.top + rcSrc.Height();
	rcTopLeft.right = rcTopLeft.left + max(rcTopLeft.Width(), rcSrc.Width());

	pImage->DrawImage(pDC, rcTopLeft, rcSrc, CRect(rcSrc.Width() - 1, 0, 0, 0));

	CRect rcFill(rc.left, rc.top + rcSrc.Height(), rc.right, rc.bottom);
	CRect rcFillTop(rcFill.left, rcFill.top, rcFill.right, rcFill.top + rcFill.Height() * 2 / 3);
	CRect rcFillBottom(rcFill.left, rcFillTop.bottom, rcFill.right, rcFill.bottom);

	XTPDrawHelpers()->GradientFill(pDC, rcFillTop, m_clrWorkspaceClientTop, m_clrWorkspaceClientMiddle, FALSE);
	XTPDrawHelpers()->GradientFill(pDC, rcFillBottom, m_clrWorkspaceClientMiddle, m_clrWorkspaceClientBottom, FALSE);

	pDC->SelectClipRgn(NULL);
}
