// XTPSliderPaintManager.cpp : implementation of the CXTPSliderPaintManager class.
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

#include <Common/XTPColorManager.h>
#include <Common/ScrollBar/XTPScrollBarPaintManager.h>
#include <CommandBars/Slider/XTPSliderPaintManager.h>


//////////////////////////////////////////////////////////////////////////
// CXTPSliderPaintManager

CXTPSliderPaintManager::CXTPSliderPaintManager(CXTPPaintManager* pPaintManager)
	: CXTPScrollBarPaintManager()
	, m_pPaintManager(pPaintManager)
{
}

void CXTPSliderPaintManager::RefreshMetrics()
{
	CXTPScrollBarPaintManager::RefreshMetrics();
	m_cyHScroll = 16;
	m_cxHScroll = 16;
	m_cThumb = 11;
}

void CXTPSliderPaintManager::DrawScrollBar(CDC *pDC, CXTPScrollBase *pScrollBar)
{
	UNUSED_ALWAYS(pDC);
	UNUSED_ALWAYS(pScrollBar);
}
