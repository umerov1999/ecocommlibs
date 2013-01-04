// XTPProgressCtrl.cpp : implementation of the CXTPProgressCtrl class.
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

#include "Common/XTPDrawHelpers.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPResourceImage.h"
#include "Common/XTPWinThemeWrapper.h"
#include "Common/XTPSystemHelpers.h"

#include "../Util/XTPControlTheme.h"
#include "XTPProgressCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define TID_TIMER 101

IMPLEMENT_DYNAMIC(CXTPProgressCtrl, CProgressCtrl)

/////////////////////////////////////////////////////////////////////////////
// CXTPScrollBar
/////////////////////////////////////////////////////////////////////////////

CXTPProgressCtrl::CXTPProgressCtrl()
{
	m_bPreSubclassInit = TRUE;
	m_nMarqueePos = 0;
	m_nMarqueeDelay = 0;
	m_nTheme = xtpControlThemeDefault;
	m_bFlatStyle = FALSE;
	m_bUseVisualStyle = TRUE;

	m_themeProgress = new CXTPWinThemeWrapper();
}

CXTPProgressCtrl::~CXTPProgressCtrl()
{
	SAFE_DELETE(m_themeProgress);
}

BEGIN_MESSAGE_MAP(CXTPProgressCtrl, CProgressCtrl)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_SYSCOLORCHANGE()

	ON_MESSAGE(PBM_SETMARQUEE, OnStartMarquee)
	ON_MESSAGE(PBM_STEPIT, OnStepIt)
	ON_MESSAGE(PBM_SETPOS, OnSetPos)
	ON_WM_TIMER()
END_MESSAGE_MAP()

void CXTPProgressCtrl::Init()
{
	RefreshMetrics();

	ModifyStyleEx(WS_EX_STATICEDGE | WS_EX_CLIENTEDGE, 0);
	SetWindowPos(NULL, 0,0,0,0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);
	SetWindowText(_T(""));

	m_nMarqueePos = 0;

	SendMessage(WM_NCPAINT);
	Invalidate(FALSE);
}

void CXTPProgressCtrl::PreSubclassWindow()
{
	CProgressCtrl::PreSubclassWindow();

	if (m_bPreSubclassInit)
	{
		// Initialize the control.
		Init();
	}
}

int CXTPProgressCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CProgressCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Initialize the control.
	Init();

	return 0;
}

BOOL CXTPProgressCtrl::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CProgressCtrl::PreCreateWindow(cs))
		return FALSE;

	// When creating controls dynamically Init() must
	// be called from OnCreate() and not from
	// PreSubclassWindow().

	m_bPreSubclassInit = FALSE;

	return TRUE;
}

void CXTPProgressCtrl::RefreshMetrics()
{
	RefreshXtremeColors();
	m_clrBorderNormal.SetStandardValue(GetSysColor(COLOR_3DSHADOW));

	if (m_nTheme == xtpControlThemeFlat)
	{
		m_clrBorderNormal.SetStandardValue(GetSysColor(COLOR_WINDOWFRAME));

	}

	if (m_nTheme == xtpControlThemeOffice2003)
	{
		XTPCurrentSystemTheme systemTheme = XTPColorManager()->GetCurrentSystemTheme();
		switch (systemTheme)
		{
		case xtpSystemThemeBlue:
		case xtpSystemThemeRoyale:
		case xtpSystemThemeAero:
			m_clrBorderNormal.SetStandardValue(RGB(127, 157, 185));
			break;

		case xtpSystemThemeOlive:
			m_clrBorderNormal.SetStandardValue(RGB(164, 185, 127));
			break;

		case xtpSystemThemeSilver:
			m_clrBorderNormal.SetStandardValue(RGB(165, 172, 178));
			break;
		}
	}

	m_themeProgress->OpenTheme(0, L"PROGRESS");

	m_clrBarColor.SetStandardValue(GetXtremeColor(COLOR_HIGHLIGHT));
	m_clrTextColor.SetStandardValue(GetXtremeColor(COLOR_HIGHLIGHTTEXT));
	m_clrBackColor.SetStandardValue(GetXtremeColor(COLOR_3DFACE));
}

COLORREF CXTPProgressCtrl::GetBackColor()
{
	return m_clrBackColor;
}

void CXTPProgressCtrl::DrawNcBorders(CDC* pDC, CRect rc)
{
	COLORREF clrBack = GetBackColor();

	if (m_bUseVisualStyle && m_themeProgress->IsAppThemed())
		return;

	if (m_bFlatStyle)
		return;

	if (m_nTheme == xtpControlThemeDefault)
	{
		pDC->DrawEdge(rc, EDGE_SUNKEN, BF_RECT);
		return;
	}
	if (m_nTheme == xtpControlThemeOffice2000)
	{
		pDC->Draw3dRect(rc, m_clrBorderNormal, GetXtremeColor(COLOR_BTNHIGHLIGHT));
	}
	else
	{
		COLORREF clrBorder = m_clrBorderNormal;
		pDC->Draw3dRect(rc, clrBorder, clrBorder);
	}

	rc.DeflateRect(1, 1);
	pDC->Draw3dRect(rc, clrBack, clrBack);
}


void CXTPProgressCtrl::OnSysColorChange()
{
	CProgressCtrl::OnSysColorChange();
	RefreshMetrics();
	Invalidate(FALSE);
}


void CXTPProgressCtrl::SetBarColor(COLORREF clrBarColor)
{
	m_clrBarColor.SetCustomValue(clrBarColor);
	Invalidate(FALSE);
}

COLORREF CXTPProgressCtrl::SetBkColor(COLORREF clrNew)
{
	m_clrBackColor.SetCustomValue(clrNew);
	Invalidate(FALSE);
	return m_clrBackColor;
}


COLORREF CXTPProgressCtrl::SetTextColor(COLORREF clrNew)
{
	m_clrTextColor.SetCustomValue(clrNew);
	Invalidate(FALSE);
	return m_clrTextColor;
}


void CXTPProgressCtrl::DoPaint(CDC* pDC)
{
	CRect rcClient;
	GetClientRect(rcClient);

	BOOL bThemed = m_bUseVisualStyle && m_themeProgress->IsAppThemed();
	BOOL bVertical = GetStyle() & PBS_VERTICAL;

	if (!bThemed && m_nTheme == xtpControlThemeResource && !bVertical)
	{
		PaintOffice2007(pDC);
		return;
	}

	if (!bThemed && !m_bFlatStyle)
	{
		DrawNcBorders(pDC, rcClient);
		rcClient.DeflateRect(2, 2);
	}


	pDC->FillSolidRect(rcClient, GetBackColor());


	if (bThemed)
	{
		m_themeProgress->DrawThemeBackground(*pDC, bVertical ? PP_BARVERT : PP_BAR, 0, rcClient, NULL);
	}

	BOOL bVista = XTPSystemVersion()->IsWinVistaOrGreater();

	int x, dxSpace, dxBlock, nBlocks;
	int iStart, iEnd;

	BOOL bSmooth = GetStyle() & PBS_SMOOTH;
	BOOL bMarquee = GetStyle() & PBS_MARQUEE;

	if (bThemed)
	{
		if (bVertical)
			rcClient.DeflateRect(3, 4);
		else
			rcClient.DeflateRect(4, 3);
	}
	else if (!m_bFlatStyle && m_nTheme == xtpControlThemeDefault)
	{
		//  give 1 pixel around the bar
		rcClient.DeflateRect(1, 1);
	}
	else if (!m_bFlatStyle && (m_nTheme == xtpControlThemeFlat) && bSmooth)
	{
		//  give 1 pixel around the bar
		rcClient.InflateRect(1, 1);
	}

	CRect rc = rcClient;

	if (bMarquee)
	{
		dxBlock = rc.Height() * 2 / 3;
		dxSpace = bVista ? 0 : 2;

		if (bThemed)
		{
			m_themeProgress->GetThemeInt(0, 0, TMT_PROGRESSCHUNKSIZE, &dxBlock);
			m_themeProgress->GetThemeInt(0, 0, TMT_PROGRESSSPACESIZE, &dxSpace);
		}

		rc.left = m_nMarqueePos + rcClient.left;

		for (int i = 0; i < 5; i++)
		{
			if (rc.left >= rcClient.right)
				rc.left = rcClient.left;

			rc.right = rc.left + dxBlock;

			if (rc.right > rcClient.right)
				rc.right = rcClient.right;

			if (bThemed)
			{
				m_themeProgress->DrawThemeBackground(*pDC, PP_CHUNK, 0, rc, NULL);
			}
			else
			{
				pDC->FillSolidRect(rc, m_clrBarColor);
			}
			rc.left = rc.right + dxSpace;
		}
	}
	else
	{
		PBRANGE range;
		SendMessage(PBM_GETRANGE, (WPARAM) FALSE, (LPARAM) &range);

		int iLow = range.iLow;
		int iHigh = range.iHigh;
		int iPos =  (int)SendMessage(PBM_GETPOS);


		if (bVertical)
		{
			iStart = rc.top;
			iEnd = rc.bottom;
			dxBlock = (rc.right - rc.left) * 2 / 3;
		}
		else
		{
			iStart = rc.left;
			iEnd = rc.right;
			dxBlock = (rc.bottom - rc.top) * 2 / 3;
		}

		x = MulDiv(iEnd - iStart, iPos - iLow, iHigh - iLow);

		dxSpace = 2;

		if (bThemed)
		{
			m_themeProgress->GetThemeInt(0, 0, TMT_PROGRESSCHUNKSIZE, &dxBlock);
			m_themeProgress->GetThemeInt(0, 0, TMT_PROGRESSSPACESIZE, &dxSpace);
		}

		if (dxBlock == 0)
			dxBlock = 1;    // avoid div by zero

		if (!bThemed && bSmooth)
		{
			dxBlock = 1;
			dxSpace = 0;
		}

		nBlocks = (x + (dxBlock + dxSpace) - 1) / (dxBlock + dxSpace); // round up

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

			if (bThemed)
			{
				m_themeProgress->DrawThemeBackground(*pDC, bVertical ? PP_CHUNKVERT : PP_CHUNK, 0, rc, NULL);
			}
			else
			{
				pDC->FillSolidRect(&rc, m_clrBarColor);
			}

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

	if (bThemed)
		return;

	CString strText;
	GetWindowText(strText);

	if (!strText.IsEmpty())
	{
		CRect rcText(rcClient);
		rcText.DeflateRect(2, 0);

		pDC->SetTextColor(m_clrTextColor);
		pDC->SetBkMode(TRANSPARENT);
		CXTPFontDC font(pDC, CWnd::GetFont());
		pDC->DrawText(strText, rcText, DT_SINGLELINE | DT_END_ELLIPSIS | DT_NOPREFIX | DT_VCENTER);
	}
}


BOOL CXTPProgressCtrl::OnEraseBkgnd(CDC* /*pDC*/)
{
	if (m_bUseVisualStyle && m_themeProgress->IsAppThemed() && m_themeProgress->IsAppThemeReady())
	{
		return (BOOL)Default();
	}
	return TRUE;
}

void CXTPProgressCtrl::OnPaint()
{
	if (m_bUseVisualStyle && m_themeProgress->IsAppThemed() && m_themeProgress->IsAppThemeReady())
	{
		Default();
	}
	else
	{
		CPaintDC dcPaint(this);
		CXTPBufferDC dc(dcPaint);

		DoPaint(&dc);
	}
}

void CXTPProgressCtrl::PaintOffice2007(CDC* pDC)
{
	CXTPResourceImages* pImages = XTPResourceImages();

	CXTPResourceImage* pImage =  pImages->LoadFile(_T("PROGRESSTRACK"));
	ASSERT(pImage);
	if (!pImage)
		return;

	CRect rc;
	GetClientRect(rc);

	pDC->FillSolidRect(rc, GetBackColor());

	pImage->DrawImage(pDC, rc, pImage->GetSource(), CRect(2, 2, 2, 2), 0xFF00FF);

	PBRANGE range;
	SendMessage(PBM_GETRANGE, (WPARAM) FALSE, (LPARAM) &range);

	int nLower = range.iLow;
	int nUpper = range.iHigh;
	int nPos =  (int)SendMessage(PBM_GETPOS);


	rc.DeflateRect(2, 2);

	int nWidth = rc.Width();

	int x = MulDiv(nWidth, nPos - nLower, nUpper - nLower);

	pImage =  pImages->LoadFile(_T("PROGRESSCHUNK"));
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


LRESULT CXTPProgressCtrl::OnStartMarquee(WPARAM wParam, LPARAM lParam)
{
	m_nMarqueePos = 0;
	m_nMarqueeDelay = 0;

	if (((GetStyle() & PBS_MARQUEE) == 0))
		return Default();

	if (m_bUseVisualStyle && m_themeProgress->IsAppThemed() && m_themeProgress->IsAppThemeReady())
		return Default();


	BOOL bMarquee = wParam && (lParam > 0);
	m_nMarqueePos = 0;

	KillTimer(TID_TIMER);

	if (bMarquee)
	{
		m_nMarqueeDelay = (UINT)lParam;
		SetTimer(TID_TIMER, m_nMarqueeDelay, NULL);
	}

	return 1;
}


void CXTPProgressCtrl::DoStep(int nStep)
{
	CXTPClientRect rcClient(this);

	BOOL bVista = XTPSystemVersion()->IsWinVistaOrGreater();
	int dxBlock = (rcClient.Height() - 2) * 2 / 3;
	int nSpace = bVista ? 0 : 2;

	m_nMarqueePos += (dxBlock + nSpace) * nStep;

	if (m_nMarqueePos > rcClient.right)
		m_nMarqueePos = 0;

	Invalidate(FALSE);
}

void CXTPProgressCtrl::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == TID_TIMER && (GetStyle() & PBS_MARQUEE))
	{
		DoStep(m_nMarqueeDelay > 50 ? 1 : 2);
		return;
	}

	CWnd::OnTimer(nIDEvent);
}

LRESULT CXTPProgressCtrl::OnStepIt(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	if (((GetStyle() & PBS_MARQUEE) == 0))
		return Default();

	if (m_bUseVisualStyle && m_themeProgress->IsAppThemed() && m_themeProgress->IsAppThemeReady())
		return Default();

	DoStep(1);
	return 1;
}

LRESULT CXTPProgressCtrl::OnSetPos(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	LRESULT lResult = Default();

	Invalidate(FALSE);
	return lResult;
}


void CXTPProgressCtrl::SetUseVisualStyle(BOOL bUseVisualStyle/* = TRUE*/)
{
	m_bUseVisualStyle = bUseVisualStyle;

	RefreshMetrics();
	if (m_hWnd) Invalidate(FALSE);
}

void CXTPProgressCtrl::SetTheme(XTPControlTheme nTheme)
{
	m_nTheme = nTheme;

	m_bUseVisualStyle = (m_nTheme == xtpControlThemeDefault);

	RefreshMetrics();

	if (m_hWnd) Invalidate(FALSE);
}
