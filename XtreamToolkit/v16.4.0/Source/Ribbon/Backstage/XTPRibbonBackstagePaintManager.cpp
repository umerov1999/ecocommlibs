// XTPRibbonBackstagePaintManager.cpp : implementation file
//
// This file is a part of the XTREME RIBBON MFC class library.
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

#include <StdAfx.h>
#include <Ribbon/Backstage/XTPRibbonBackstagePaintManager.h>

const int COMMAND_GAP  = 6;
const int TEXT_GAP     = 15;
const int IMAGE_MARGIN = 7;


CXTPRibbonBackstagePaintManager::CXTPRibbonBackstagePaintManager()
	: m_nCommandGap (COMMAND_GAP)
	, m_nTextGap    (TEXT_GAP)
	, m_nImageMargin(IMAGE_MARGIN)
{
}

CXTPRibbonBackstagePaintManager::~CXTPRibbonBackstagePaintManager()
{
}

void CXTPRibbonBackstagePaintManager::DrawBackground(CDC *pDC, CXTPRibbonBackstageView *pView)
{
	UNUSED_ALWAYS(pDC);
	UNUSED_ALWAYS(pView);
}

void CXTPRibbonBackstagePaintManager::DrawControl(CDC *pDC, CXTPRibbonBackstageView *pView, CXTPControl *pControl)
{
	UNUSED_ALWAYS(pDC);
	UNUSED_ALWAYS(pView);
	UNUSED_ALWAYS(pControl);
}
