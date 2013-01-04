// XTPSplitterWndTheme.cpp: implementation of the XTPSplitterWndTheme class.
//
// This file is a part of the XTREME CONTROLS MFC class library.
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
#include "Common/XTPResourceImage.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPDrawHelpers.h"

#include "../Util/XTPControlTheme.h"
#include "XTPSplitterWnd.h"
#include "XTPSplitterWndTheme.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

enum ESplitType { splitBox, splitBar, splitIntersection, splitBorder };

/////////////////////////////////////////////////////////////////////////////
// CXTPSplitterWndTheme

CXTPSplitterWndTheme::CXTPSplitterWndTheme()
: m_bTabColors(FALSE)
{
}

void CXTPSplitterWndTheme::RefreshMetrics()
{
	// set splitter colors.
	m_crBack.SetStandardValue(::GetSysColor(COLOR_3DFACE));
	m_crOuterBorder.SetStandardValue(::GetSysColor(COLOR_3DHILIGHT), ::GetSysColor(COLOR_3DSHADOW));
	m_crInnerBorder.SetStandardValue(::GetSysColor(COLOR_3DFACE), ::GetSysColor(COLOR_WINDOWFRAME));

	// set split box colors.
	m_crBoxBack = m_crBack;
	m_crOuterBoxBorder = m_crOuterBorder;
	m_crInnerBoxBorder = m_crInnerBorder;
}

void CXTPSplitterWndTheme::DrawSplitter(CDC* pDC, CXTPSplitterWnd* pSplitter, int nType, const CRect& rectArg)
{
	CRect rect = rectArg;

	// draw the borders.
	switch (nType)
	{
	case splitBorder:
		pDC->Draw3dRect(rect, pSplitter->IsBorderVisible() ? m_crOuterBorder.clrDark : m_crBack, pSplitter->IsBorderVisible() ? m_crOuterBorder.clrLight : m_crBack);
		rect.InflateRect(-CX_BORDER, -CY_BORDER);
		pDC->Draw3dRect(rect, pSplitter->IsBorderVisible() ? m_crInnerBorder.clrDark : m_crBack, pSplitter->IsBorderVisible() ? m_crInnerBorder.clrLight : m_crBack);
		return;

	case splitIntersection:
		ASSERT(!XTPSystemVersion()->IsWin95OrGreater());
		break;

	case splitBox:
		pDC->Draw3dRect(rect, m_crOuterBoxBorder.clrLight, m_crOuterBoxBorder.clrDark);
		rect.InflateRect(-CX_BORDER, -CY_BORDER);
		pDC->Draw3dRect(rect, m_crInnerBoxBorder.clrLight, m_crInnerBoxBorder.clrDark);
		rect.InflateRect(-CX_BORDER, -CY_BORDER);
		pDC->FillSolidRect(&rect, m_crBoxBack.clrDark);
		return;

	case splitBar:
		break;

	default:
		ASSERT(FALSE);  // unknown splitter type
	}

	// fill the background.
	pDC->FillSolidRect(&rect, m_crBack.clrDark);
}

void CXTPSplitterWndTheme::EnableTabColors(BOOL bEnable /*=TRUE*/)
{
	m_bTabColors = bEnable;
	RefreshMetrics();
}

CBrush* CXTPSplitterWndTheme::GetTrackBrush()
{
	return CDC::GetHalftoneBrush();
}

/////////////////////////////////////////////////////////////////////////////
// CXTPSplitterWndThemeOfficeXP

CXTPSplitterWndThemeOfficeXP::CXTPSplitterWndThemeOfficeXP()
{
	m_nTheme = xtpControlThemeOfficeXP;
}

void CXTPSplitterWndThemeOfficeXP::RefreshMetrics()
{
	// set splitter colors.
	m_crBack.SetStandardValue(::GetSysColor(COLOR_3DFACE));
	m_crOuterBorder.SetStandardValue(::GetSysColor(COLOR_3DFACE));
	m_crInnerBorder.SetStandardValue(::GetSysColor(COLOR_3DHILIGHT), ::GetSysColor(COLOR_3DSHADOW));

	// set split box colors.
	m_crBoxBack = m_crBack;
	m_crOuterBoxBorder.SetStandardValue(::GetSysColor(COLOR_3DHILIGHT), ::GetSysColor(COLOR_3DSHADOW));
	m_crInnerBoxBorder.SetStandardValue(::GetSysColor(COLOR_3DFACE));
}

CBrush* CXTPSplitterWndThemeOfficeXP::GetTrackBrush()
{
	return CBrush::FromHandle((HBRUSH)::GetStockObject(WHITE_BRUSH));
}

/////////////////////////////////////////////////////////////////////////////
// CXTPSplitterWndThemeOffice2003

CXTPSplitterWndThemeOffice2003::CXTPSplitterWndThemeOffice2003()
{
	m_nTheme = xtpControlThemeOffice2003;
}

void CXTPSplitterWndThemeOffice2003::RefreshMetrics()
{
	CXTPSplitterWndThemeOfficeXP::RefreshMetrics();

	// set splitter colors.
	if (!XTPColorManager()->IsLunaColorsDisabled())
	{
		XTPCurrentSystemTheme systemTheme = XTPColorManager()->GetCurrentSystemTheme();

		switch (systemTheme)
		{
		case xtpSystemThemeBlue:
		case xtpSystemThemeRoyale:
		case xtpSystemThemeAero:
			m_crBack.SetStandardValue(RGB(165, 195, 255));
			m_crOuterBorder.SetStandardValue(RGB(165, 195, 255));
			m_crInnerBorder.SetStandardValue(RGB(0, 44, 148));
			break;

		case xtpSystemThemeOlive:
			m_crBack.SetStandardValue(RGB(226, 231, 191));
			m_crOuterBorder.SetStandardValue(RGB(226, 231, 191));
			m_crInnerBorder.SetStandardValue(RGB(99, 130, 90));
			break;

		case xtpSystemThemeSilver:
			m_crBack.SetStandardValue(RGB(223, 223, 234));
			m_crOuterBorder.SetStandardValue(RGB(223, 223, 234));
			m_crInnerBorder.SetStandardValue(RGB(123, 125, 148));
			break;
		}
	}

	// set split box colors.
	m_crBoxBack = m_crBack;
	m_crOuterBoxBorder = m_crOuterBorder;
	m_crInnerBoxBorder = m_crInnerBorder;
}

void CXTPSplitterWndThemeOffice2003::DrawSplitter(CDC* pDC, CXTPSplitterWnd* pSplitter, int nType, const CRect& rectArg)
{
	if (nType == splitBox)
	{
		// fill the background.
		CRect rect = rectArg;
		pDC->FillSolidRect(&rect, m_crBoxBack.clrDark);

		CXTPPenDC pen(*pDC, m_crInnerBorder.clrDark);

		if (rect.Width() > rect.Height())
		{
			// vSplitterBox
			pDC->MoveTo(rect.left,rect.top);
			pDC->LineTo(rect.left,rect.bottom-1);
			pDC->LineTo(rect.right,rect.bottom-1);
		}
		else
		{
			// hSplitterBox
			pDC->MoveTo(rect.left,rect.top);
			pDC->LineTo(rect.right-1,rect.top);
			pDC->LineTo(rect.right-1,rect.bottom);
		}
	}
	else
	{
		CXTPSplitterWndThemeOfficeXP::DrawSplitter(pDC, pSplitter, nType, rectArg);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CXTPSplitterWndThemeResource

CXTPSplitterWndThemeResource::CXTPSplitterWndThemeResource()
{
	m_nTheme = xtpControlThemeResource;
}

void CXTPSplitterWndThemeResource::RefreshMetrics()
{
	CXTPResourceImages* pImages = XTPResourceImages();
	if (pImages)
	{
		// get colors from resource.
		COLORREF crBack = pImages->GetImageColor(_T("Window"), m_bTabColors ? _T("SplitterFaceTab") : _T("SplitterFace"), GetXtremeColor(COLOR_3DFACE));
		COLORREF crBorder = pImages->GetImageColor(_T("Window"), m_bTabColors ? _T("SplitterFrameTab") : _T("SplitterFrame"), GetXtremeColor(COLOR_3DSHADOW));

		// set splitter colors.
		m_crBack.SetStandardValue(crBack);
		m_crOuterBorder.SetStandardValue(crBack);
		m_crInnerBorder.SetStandardValue(crBorder);

		// set split box colors.
		m_crBoxBack = m_crBack;
		m_crOuterBoxBorder = m_crOuterBorder;
		m_crInnerBoxBorder = m_crInnerBorder;
	}
	else
	{
		CXTPSplitterWndThemeOfficeXP::RefreshMetrics();
	}
}

/////////////////////////////////////////////////////////////////////////////
// CXTPSplitterWndThemeVisualStudio2010

CXTPSplitterWndThemeVisualStudio2010::CXTPSplitterWndThemeVisualStudio2010()
{
	m_nTheme = xtpControlThemeVisualStudio2010;
}

void CXTPSplitterWndThemeVisualStudio2010::RefreshMetrics()
{
	// set splitter colors.
	m_crBack.SetStandardValue(m_bTabColors ? RGB(255, 243, 206) : RGB(173, 186, 206));
	m_crOuterBorder.SetStandardValue(m_bTabColors ? RGB(255, 243, 206) : RGB(173, 186, 206));
	m_crInnerBorder.SetStandardValue(m_bTabColors ? RGB(255, 243, 206) : RGB(132, 146, 165));

	// set split box colors.
	m_crBoxBack.SetStandardValue(RGB(208, 215, 226));
	m_crOuterBoxBorder.SetStandardValue(m_bTabColors ? RGB(155, 167, 183) : RGB(208, 215, 226));
	m_crInnerBoxBorder.SetStandardValue(RGB(208, 215, 226));
}

void CXTPSplitterWndThemeVisualStudio2010::DrawSplitter(CDC* pDC, CXTPSplitterWnd* pSplitter, int nType, const CRect& rectArg)
{
	if (m_bTabColors)
	{
		CXTPSplitterWndThemeOfficeXP::DrawSplitter(pDC, pSplitter, nType, rectArg);
	}
	else
	{
		CXTPSplitterWndThemeOffice2003::DrawSplitter(pDC, pSplitter, nType, rectArg);
	}
}
