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
#include <Common/Controls/Progress/XTPProgressBase.h>
#include <Common/ScrollBar/XTPScrollInfo.h>

#include <CommandBars/XTPCommandBarsDefines.h>
#include <CommandBars/XTPCommandBar.h>
#include <CommandBars/XTPPopupBar.h>
#include <CommandBars/MessageBar/XTPMessageBar.h>
#include <CommandBars/XTPControls.h>
#include <CommandBars/XTPControl.h>
#include <CommandBars/XTPControlProgress.h>
#include <CommandBars/XTPPaintManager.h>
#include <CommandBars/Themes/XTPCommandBarsOfficeXPTheme.h>
#include <CommandBars/Themes/XTPCommandBarsOffice2003Theme.h>
#include <CommandBars/Themes/XTPCommandBarsResourceTheme.h>

#include <CommandBars/Progress/XTPProgressPaintManager.h>
#include <CommandBars/Progress/XTPProgressThemeResource.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPProgressResourceTheme

CXTPProgressResourceTheme::CXTPProgressResourceTheme(CXTPPaintManager* pPaintManager)
	: CXTPProgressPaintManager(pPaintManager)
{
	m_cyProgress = 12;
}

void CXTPProgressResourceTheme::DrawProgress(CDC* pDC, CXTPProgressBase* pProgressBar)
{
	if(!XTPResourceImages()->IsValid())
			return;

	CXTPResourceTheme* pPaintManager = (CXTPResourceTheme*)m_pPaintManager;

	CXTPResourceImage* pImage =  pPaintManager->LoadImage(_T("PROGRESSTRACK"));
	ASSERT(pImage);
	if (!pImage)
		return;

	CRect rc = pProgressBar->GetProgressRect();

	pImage->DrawImage(pDC, rc, pImage->GetSource(), CRect(2, 2, 2, 2), 0xFF00FF);

	int nLower, nUpper, nPos;
	pProgressBar->GetRange(nLower, nUpper);
	nPos = pProgressBar->GetPos();

	rc.DeflateRect(2, 2);

	int nWidth = rc.Width();

	int x = MulDiv(nWidth, nPos - nLower, nUpper - nLower);

	pImage =  pPaintManager->LoadImage(_T("PROGRESSCHUNK"));
	ASSERT(pImage);
	if (!pImage)
		return;

	CRect rcSrc(pImage->GetSource());
	rcSrc.right -= 4;

	CRect rcDest(rc.left, rc.top, rc.left + x, rc.bottom);

	if (rcDest.Width() < rcSrc.Width())
		rcSrc.left = rcSrc.right - rcDest.Width();

	pImage->DrawImage(pDC, rcDest, rcSrc, CRect(2, 2, 2, 2), 0xFF00FF);

	if (rc.left + x < rc.right - 1)
	{
		int nShadow = min(4, rc.right - rc.left - x);
		rcSrc = CRect(rcSrc.right, rcSrc.top, rcSrc.right + nShadow, rcSrc.bottom);

		rcDest = CRect(rcDest.right, rcDest.top, rcDest.right + nShadow, rcDest.bottom);

		pImage->DrawImage(pDC, rcDest, rcSrc, CRect(0, 2, 0, 2), 0xFF00FF);
		return;
	}
}
