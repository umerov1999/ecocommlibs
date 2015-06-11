// XTPProgressThemeWindowsUx.cpp : implementation of the CXTPProgressThemeWindowsUx class.
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
#include <Common/XTPWinThemeWrapper.h>
#include <Common/XTPSystemHelpers.h>
#include <Common/Controls/Progress/XTPProgressBase.h>

#include <CommandBars/XTPCommandBarsDefines.h>
#include <CommandBars/XTPControl.h>
#include <CommandBars/XTPControlProgress.h>
#include <CommandBars/XTPPaintManager.h>
#include <CommandBars/Progress/XTPProgressPaintManager.h>
#include <CommandBars/Progress/XTPProgressThemeWindowsUx.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CXTPProgressThemeWindowsUx::CXTPProgressThemeWindowsUx(CXTPPaintManager *pPaintManager)
	: CXTPProgressPaintManager(pPaintManager)
{
	m_themeProgress = new CXTPWinThemeWrapper();
}

CXTPProgressThemeWindowsUx::~CXTPProgressThemeWindowsUx()
{
	SAFE_DELETE(m_themeProgress);
}

void CXTPProgressThemeWindowsUx::RefreshMetrics()
{
	CXTPProgressPaintManager::RefreshMetrics();
	m_themeProgress->OpenTheme(0, L"PROGRESS");
}

void CXTPProgressThemeWindowsUx::DrawProgress(CDC* pDC, CXTPProgressBase* pProgressBar)
{
	CRect rc(pProgressBar->GetProgressRect());

	BOOL bThemed   = TRUE;
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

	{
		m_themeProgress->DrawThemeBackground(*pDC, bVertical ? PP_BARVERT : PP_BAR, 0, rc, NULL);

		int dxSpace = 2;

		if (bThemed)
		{
			m_themeProgress->GetThemeInt(0, 0, TMT_PROGRESSCHUNKSIZE, &dxBlock);
			m_themeProgress->GetThemeInt(0, 0, TMT_PROGRESSSPACESIZE, &dxSpace);
		}

		rc = rcClient;

		if (dxBlock == 0)
			dxBlock = 1;    // avoid div by zero

		int nBlocks = (x + (dxBlock + dxSpace) - 1) / (dxBlock + dxSpace); // round up

		for (int i = 0; i < nBlocks; i++)
		{
			if (bVertical)
			{
				rc.top = rc.bottom - dxBlock;

				// are we past the end?
				if (rc.bottom <= rcClient.top)
					break;

				if (rc.top <= rcClient.top)
					rc.top = rcClient.top + 1;

			}
			else
			{
				rc.right = rc.left + dxBlock;

				// are we past the end?
				if (rc.left >= rcClient.right)
					break;

				if (rc.right >= rcClient.right)
					rc.right = rcClient.right - 1;
			}

			m_themeProgress->DrawThemeBackground(*pDC, bVertical ? PP_CHUNKVERT : PP_CHUNK, 0, rc, NULL);

			if (bVertical)
			{
				rc.bottom = rc.top - dxSpace;
			}
			else
			{
				rc.left = rc.right + dxSpace;
			}
		}
	}
}
