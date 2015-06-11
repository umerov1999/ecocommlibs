// XTPControlProgress.cpp : implementation of the CXTPControlProgress class.
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

#include <StdAfx.h>

#include <Common/XTPColorManager.h>
#include <Common/XTPSystemHelpers.h>
#include <Common/Controls/Progress/XTPProgressBase.h>

#include <CommandBars/XTPCommandBarsDefines.h>
#include <CommandBars/XTPControl.h>
#include <CommandBars/XTPControlProgress.h>
#include <CommandBars/XTPPaintManager.h>
#include <CommandBars/Progress/XTPProgressPaintManager.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//////////////////////////////////////////////////////////////////////////
// CXTPProgressPaintManager

CXTPProgressPaintManager::CXTPProgressPaintManager(CXTPPaintManager* pPaintManager)
{
	m_pPaintManager = pPaintManager;
	m_cyProgress = 16;
}

CXTPProgressPaintManager::~CXTPProgressPaintManager()
{
}

void CXTPProgressPaintManager::RefreshMetrics()
{
}

void CXTPProgressPaintManager::DrawProgress(CDC* pDC, CXTPProgressBase* pProgressBar)
{
	CRect rc(pProgressBar->GetProgressRect());

	BOOL bThemed   = FALSE;
	BOOL bVertical = FALSE;

	int nLower, nUpper, nPos;
	pProgressBar->GetRange(nLower, nUpper);
	nPos = pProgressBar->GetPos();

	int nWidth, dxBlock;

	CRect rcClient(rc);

	if (!bThemed)
		rcClient.DeflateRect(2, 2);
	else if (bVertical)
		rcClient.DeflateRect(3, 4);
	else
		rcClient.DeflateRect(4, 3);

	if (bVertical)
	{
		nWidth = rcClient.bottom - rcClient.top;
		dxBlock = (rcClient.right - rcClient.left) * 2 / 3;
	}
	else
	{
		nWidth = rcClient.right - rcClient.left;
		dxBlock = (rcClient.bottom - rcClient.top) * 2 / 3;
	}

	int x = MulDiv(nWidth, nPos - nLower, nUpper - nLower);

	if (!bThemed)
	{
		pDC->FillSolidRect(rc, GetXtremeColor(COLOR_BTNFACE));
		pDC->Draw3dRect(rc, GetXtremeColor(COLOR_3DSHADOW), GetXtremeColor(COLOR_3DSHADOW));

		if (rcClient.Width() < 2)
			return;

		pDC->FillSolidRect(rcClient.left, rcClient.top, x, rcClient.Height(), GetXtremeColor(COLOR_HIGHLIGHT));
	}
}
