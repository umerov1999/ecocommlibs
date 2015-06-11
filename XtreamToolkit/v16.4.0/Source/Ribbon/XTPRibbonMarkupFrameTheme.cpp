// XTPRibbonMarkupFrameTheme.cpp : implementation of the CXTPRibbonMarkupFrameTheme class.
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

#include <Common/XTPDrawHelpers.h>
#include <Common/XTPResourceImage.h>
#include <Common/XTPHookManager.h>
#include <Common/XTPMarkupRender.h>
#include <Common/XTPResourceManager.h>
#include <Common/XTPSystemHelpers.h>
#include <Common/XTPColorManager.h>
#include <Common/XTPWinThemeWrapper.h>
#include <Common/XTPVC80Helpers.h>
#include <Common/ScrollBar/XTPScrollInfo.h>
#include <Common/Hook/XTPWinEventHook.h>

#include <CommandBars/XTPCommandBarsDefines.h>
#include <CommandBars/XTPCommandBar.h>
#include <CommandBars/XTPControls.h>
#include <CommandBars/XTPControl.h>
#include <CommandBars/XTPPaintManager.h>
#include <CommandBars/Frame/XTPFrameHook.h>
#include <CommandBars/Frame/XTPFramePaintManager.h>

#include <Ribbon/XTPRibbonPaintManager.h>
#include <Ribbon/XTPRibbonMarkupFrameTheme.h>


CXTPRibbonMarkupFrameTheme::CXTPRibbonMarkupFrameTheme(CXTPPaintManager *pPaintManager)
	: CXTPFramePaintManager(pPaintManager)
{

}

CXTPRibbonMarkupFrameTheme::~CXTPRibbonMarkupFrameTheme()
{

}

void CXTPRibbonMarkupFrameTheme::DrawFrame(CDC *pDC, CXTPCommandBarsFrameHook *pFrameHook)
{
	CString sCompatibility;
	CXTPResourceImages *pImages = XTPResourceImages();

	if (NULL != pImages)
		sCompatibility = pImages->GetImageString(_T("Theme"), _T("Compatibility"));

	XTPPaintTheme paintTheme = m_pPaintManager->GetCurrentTheme();

	switch(paintTheme)
	{
		case xtpThemeResource:
		case xtpThemeRibbon:
			return;
			break;

		case xtpThemeVisualStudio2012:
		case xtpThemeVisualStudio2012Light:
		case xtpThemeVisualStudio2012Dark:
			if (_T("VisualStudio2012") != sCompatibility)
				return;
				break;

		case xtpThemeOffice2013:
			if (_T("Office2013") != sCompatibility)
				return;
				break;
	}

	BOOL bActive = pFrameHook->IsFrameActive();

	DrawFrameBorder(pDC, pFrameHook);

	CRect rcCaption = CalcFrameCaption(pFrameHook);

	if (pFrameHook->IsCaptionVisible())
	{
		CXTPBufferDC dc(*pDC, rcCaption);

		if(!pFrameHook->IsDwmEnabled())
		{
			DrawCaptionBar(&dc, rcCaption, pFrameHook->GetSite(), bActive);
		}

		DrawFrameIcon(&dc, pFrameHook, rcCaption);
		DrawFrameCaptionButtons(&dc, pFrameHook, rcCaption);

		rcCaption.left += pFrameHook->CalcFrameBorder().left;
		DrawCaptionText(&dc, rcCaption, pFrameHook->GetSite(), bActive);
	}

}


void CXTPRibbonMarkupFrameTheme::DrawFrameBorder(CDC *pDC, CXTPCommandBarsFrameHook *pFrameHook)
{
	CWnd *pSite = pFrameHook->GetSite();
	BOOL bActive = pFrameHook->IsFrameActive();

	CXTPWindowRect rcWindow(pSite);
	CXTPClientRect rcClient(pSite);
	pSite->ClientToScreen(rcClient);

	rcClient.OffsetRect(-rcWindow.TopLeft());
	rcWindow.OffsetRect(-rcWindow.TopLeft());

	const int nCaptionHeight = GetSystemMetrics(SM_CYCAPTION);
	CXTPMarkupThemePart *pPart;

	pPart = GetFrameBorder(xtpBorderPartTop, bActive);
	if (NULL != pPart)
	{
		CRect rcBorderTop(rcWindow.left, rcWindow.top, rcWindow.right, rcClient.top);
		pPart->RenderMarkup(pDC, rcBorderTop);
	}

	pPart = GetFrameBorder(xtpBorderPartLeftCaption, bActive);
	if (NULL != pPart)
	{
		CRect rcBorderLeftCaption(rcWindow.left, rcClient.top, rcClient.left, rcClient.top + nCaptionHeight);
		pPart->RenderMarkup(pDC, rcBorderLeftCaption);
	}

	pPart = GetFrameBorder(xtpBorderPartLeft, bActive);
	if (NULL != pPart)
	{
		CRect rcBorderLeft(rcWindow.left, rcClient.top + nCaptionHeight, rcClient.left, rcClient.bottom);
		pPart->RenderMarkup(pDC, rcBorderLeft);
	}

	pPart = GetFrameBorder(xtpBorderPartRightCaption, bActive);
	if (NULL != pPart)
	{
		CRect rcBorderRightCaption(rcClient.right, rcClient.top, rcWindow.right, rcClient.top + nCaptionHeight);
		pPart->RenderMarkup(pDC, rcBorderRightCaption);
	}

	pPart = GetFrameBorder(xtpBorderPartRight, bActive);
	if (NULL != pPart)
	{
		CRect rcBorderRight(rcClient.right, rcClient.top + nCaptionHeight, rcWindow.right, rcClient.bottom);
		pPart->RenderMarkup(pDC, rcBorderRight);
	}

	pPart = GetFrameBorder(xtpBorderPartBottom, bActive);
	if (NULL != pPart)
	{
		CRect rcBorderBottom(rcWindow.left, rcClient.bottom, rcWindow.right, rcWindow.bottom);
		pPart->RenderMarkup(pDC, rcBorderBottom);
	}
}
