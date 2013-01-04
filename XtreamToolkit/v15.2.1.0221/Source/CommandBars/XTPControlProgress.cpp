// XTPControlProgress.cpp : implementation of the CXTPControlProgress class.
//
// This file is a part of the XTREME COMMANDBARS MFC class library.
// (c)1998-2012 Codejock Software, All Rights Reserved.
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

#include "stdafx.h"

#include "Common/XTPColorManager.h"
#include "Common/XTPPropExchange.h"
#include "Common/XTPWinThemeWrapper.h"
#include "Common/XTPSystemHelpers.h"

#include "XTPCommandBarsDefines.h"
#include "XTPControl.h"
#include "XTPControlProgress.h"
#include "XTPPaintManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CXTPProgressBase::CXTPProgressBase()
{
	m_nMin = 0;
	m_nMax = 100;
	m_nPos = 0;
}

CXTPProgressBase::~CXTPProgressBase()
{

}

void CXTPProgressBase::GetRange(int& nLower, int& nUpper) const
{
	nLower = m_nMin;
	nUpper = m_nMax;
}

void CXTPProgressBase::SetRange(int nLower, int nUpper)
{
	if (m_nMin != nLower || m_nMax != nUpper)
	{
		m_nMin = nLower;
		m_nMax = nUpper;
		RedrawProgress();
	}
}

void CXTPProgressBase::SetPos(int nPos)
{
	if (nPos < m_nMin) nPos = m_nMin;
	if (nPos > m_nMax) nPos = m_nMax;

	if (m_nPos != nPos)
	{
		m_nPos = nPos;
		RedrawProgress();
	}
}

IMPLEMENT_XTP_CONTROL(CXTPControlProgress, CXTPControl)

CXTPControlProgress::CXTPControlProgress()
{
	m_nWidth = 100;
}

CSize CXTPControlProgress::GetSize(CDC* /*pDC*/)
{
	return CSize(m_nWidth, m_nHeight > 0 ? m_nHeight : GetProgressPaintManager()->m_cyProgress);
}

void CXTPControlProgress::Draw(CDC* pDC)
{
	GetProgressPaintManager()->DrawProgress(pDC, this);
}

void CXTPControlProgress::RedrawProgress()
{
	RedrawParent(FALSE);
}

CXTPProgressPaintManager* CXTPControlProgress::GetProgressPaintManager() const
{
	return GetPaintManager()->GetProgressPaintManager();
}

CRect CXTPControlProgress::GetProgressRect()
{
	return GetRect();
}

void CXTPControlProgress::Copy(CXTPControl* pControl, BOOL bRecursive)
{
	CXTPControl::Copy(pControl, bRecursive);

	CXTPControlProgress* pControlProgress = DYNAMIC_DOWNCAST(CXTPControlProgress, pControl);
	if (!pControlProgress)
		return;

	SetRange(pControlProgress->m_nMin, pControlProgress->m_nMax);
	SetPos(pControlProgress->GetPos());
}

void CXTPControlProgress::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPControl::DoPropExchange(pPX);

	PX_Int(pPX, _T("Min"), m_nMin, 0);
	PX_Int(pPX, _T("Max"), m_nMax, 100);
	PX_Int(pPX, _T("Pos"), m_nPos, 0);
}

void CXTPControlProgress::OnClick(BOOL bKeyboard, CPoint pt)
{
	if (!bKeyboard && IsCustomizeMode() && IsCustomizeMovable())
	{
		CustomizeStartDrag(pt);
		return;
	}
}

//////////////////////////////////////////////////////////////////////////
// CXTPProgressPaintManager

CXTPProgressPaintManager::CXTPProgressPaintManager(CXTPPaintManager* pPaintManager)
{
	m_pPaintManager = pPaintManager;
	m_cyProgress = 16;

	m_themeProgress = new CXTPWinThemeWrapper();
}

CXTPProgressPaintManager::~CXTPProgressPaintManager()
{
	SAFE_DELETE(m_themeProgress);
}

void CXTPProgressPaintManager::RefreshMetrics()
{
	m_themeProgress->OpenTheme(0, L"PROGRESS");
}

void CXTPProgressPaintManager::DrawProgress(CDC* pDC, CXTPProgressBase* pProgressBar)
{
	CRect rc(pProgressBar->GetProgressRect());

	XTPPaintTheme theme = GetPaintManager()->BaseTheme();

	BOOL bThemed = (theme != xtpThemeOffice2000) && (theme != xtpThemeOfficeXP) && m_themeProgress->IsAppThemed();
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
	else
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


