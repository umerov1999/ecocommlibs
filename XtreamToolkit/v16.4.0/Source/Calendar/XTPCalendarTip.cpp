// XTPCalendarTip.cpp
//
// This file is a part of the XTREME CALENDAR MFC class library.
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

#include "stdafx.h"

#include "Common/XTPDrawHelpers.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPSmartPtrInternalT.h"
#include "Common/XTPNotifyConnection.h"

#include "XTPCalendarDefines.h"

#include "XTPCalendarPtrCollectionT.h"
#include "XTPCalendarViewPart.h"

#include "XTPCalendarResource.h"

#include "XTPCalendarTheme.h"
#include "XTPCalendarThemePrevNextEventButton.h"
#include "XTPCalendarThemePrevNextEventButtons.h"
#include "XTPCalendarThemeOffice2007.h"

#include "XTPCalendarTip.h"

#include "XTPCalendarControl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTPCalendarTip

CXTPCalendarTip::CXTPCalendarTip()
{
	m_pTheme = NULL;
	m_bAdvancedMode = FALSE;
	m_pParentWnd = NULL;
	m_pFont = NULL;
	m_rcHover.SetRect(0, 0, 0, 0);
}

CXTPCalendarTip::~CXTPCalendarTip()
{
}

BEGIN_MESSAGE_MAP(CXTPCalendarTip, CWnd)
//{{AFX_MSG_MAP(CXTPCalendarTip)
	ON_WM_NCHITTEST_EX()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CXTPCalendarTip message handlers

LRESULT CXTPCalendarTip::OnNcHitTest(CPoint point)
{
	UNREFERENCED_PARAMETER(point);

	return HTTRANSPARENT;
}

BOOL CXTPCalendarTip::Create(CWnd* pParentWnd)
{
	ASSERT_VALID(pParentWnd);

	// Already created ?
	if (m_hWnd)
		return TRUE;

	m_pParentWnd = pParentWnd;
	return CWnd::CreateEx(0, AfxRegisterWndClass(0, AfxGetApp()->LoadStandardCursor(IDC_ARROW)), NULL, WS_POPUP,
		CXTPEmptyRect(), pParentWnd, NULL);
}


void CXTPCalendarTip::Activate(BOOL bActive, BOOL bAdvanced)
{
	if (bActive)
		m_bAdvancedMode = bAdvanced;

	if (bActive)
	{
		CString strText = m_strTooltipText;
		CWindowDC dc(this);
		CXTPFontDC font(&dc, m_pFont);
		CXTPWindowRect rc(this);
		rc.right = rc.left + dc.GetTextExtent(strText).cx + 6;

		CRect rcWork = XTPMultiMonitor()->GetWorkArea(m_pParentWnd);
		if (rc.right > rcWork.right)
			rc.OffsetRect(rcWork.right - rc.right, 0);

		if (!m_pTheme)
		{
			SetWindowRgn(NULL, FALSE);
		}
		else
		{
			m_pTheme->SetTipWindowRgn(this);
		}

		Invalidate(FALSE);
	}

	ShowWindow(bActive ? SW_SHOWNOACTIVATE : SW_HIDE);

}

BOOL CXTPCalendarTip::OnEraseBkgnd(CDC*)
{
	return TRUE;
}

void CXTPCalendarTip::OnPaint()
{
	CPaintDC dc(this);
	CXTPClientRect rc(this);

	CXTPCalendarControl *pControl = static_cast<CXTPCalendarControl *>(m_pParentWnd);

	if (pControl && pControl->GetPaintTheme() == xtpCalendarThemeOffice2013)
	{
		CXTPFontDC font(&dc, m_pFont);

		dc.FillSolidRect(&rc, RGB(0xff, 0xff, 0xff));

		const COLORREF clrBorder = RGB(190, 190, 190);
		dc.Draw3dRect(rc, clrBorder, clrBorder);

		CString str = m_strTooltipText;

		dc.SetBkMode(TRANSPARENT);

		rc.DeflateRect(3, 3, 3, 3);

		DWORD dwFlags = DT_NOPREFIX | DT_EXPANDTABS;
		BOOL bLayoutRTL = GetSafeHwnd() && GetWindowLong(GetSafeHwnd(), GWL_EXSTYLE) & WS_EX_LAYOUTRTL;

		if (bLayoutRTL)
		{
			dwFlags |= DT_RTLREADING | DT_RIGHT;
		}

		COLORREF clrTextOld = dc.SetTextColor(RGB(93, 93, 93));

		dc.DrawText(str, rc, dwFlags | DT_WORDBREAK);
		dc.SetTextColor(clrTextOld);
	}
	else if (!m_pTheme)
	{
		COLORREF clrText = GetSysColor(COLOR_INFOTEXT);
		dc.FillSolidRect(rc, GetSysColor(COLOR_INFOBK));
		dc.Draw3dRect(rc, RGB(241, 239, 226), RGB(113, 111, 100));

		dc.SetTextColor(clrText);
		dc.SetBkMode(TRANSPARENT);

		CXTPFontDC font(&dc, m_pFont);
		CRect rcText(rc);
		rcText.left += 3;
		rcText.top += 3;

		if (m_strTooltipText.GetLength() >= 12
			&& m_strTooltipText.Mid(11).Find(_T(" ")) > -1)
			dc.DrawText(m_strTooltipText, rcText, DT_WORDBREAK | DT_NOPREFIX);
		else
			dc.DrawText(m_strTooltipText, rcText, DT_NOPREFIX);
	}
	else // Office 2007 theme-similar drawings
	{
		// fill background
		COLORREF clrLight = m_pTheme->GetColor(CXTPCalendarThemeOffice2007::xtpCLR_DayViewCellWorkBk);
		COLORREF clrDark = m_pTheme->GetColor(CXTPCalendarThemeOffice2007::xtpCLR_DayViewCellNonWorkBk);
		COLORREF clrText = m_bAdvancedMode ?
			XTPDrawHelpers()->LightenColor(120, m_pTheme->m_clrBaseColor) :
			RGB(0, 0, 0);
		COLORREF clrBorder = m_pTheme->GetColor(CXTPCalendarThemeOffice2007::xtpCLR_DayViewSingleDayEventBorder);

		CXTPPaintManagerColorGradient grclrBk(clrLight, clrDark);

		dc.SetTextColor(clrText);
		dc.SetBkMode(TRANSPARENT);

		m_pTheme->DrawRoundRect(&dc, &rc, clrBorder, FALSE, grclrBk);

		CXTPFontDC font(&dc, m_pTheme->m_fntBaseFont);
		CRect rcText(rc);

		rcText.DeflateRect(3, 3, 3, 3);

		if (m_strTooltipText.GetLength() >= 12
			&& m_strTooltipText.Mid(11).Find(_T(" ")) > -1)
			dc.DrawText(m_strTooltipText, rcText, DT_WORDBREAK | DT_NOPREFIX | DT_VCENTER);
		else
			dc.DrawText(m_strTooltipText, rcText, DT_NOPREFIX | DT_VCENTER |
			(m_bAdvancedMode ? DT_CENTER : 0));
	}
}

CSize CXTPCalendarTip::CalcToolTipRect(BOOL bFull)
{
	CWindowDC dc(NULL);
	CRect rc(0, 0, 146, 20);

	CXTPFontDC font(&dc, m_pFont);
	if (bFull)
	{
		if (m_strTooltipText.GetLength() >= 12
			&& m_strTooltipText.Mid(11).Find(_T(" ")) > -1)
		{
			dc.DrawText(m_strTooltipText, rc, DT_CALCRECT | DT_WORDBREAK);
			return CSize(rc.Width() + 15, rc.Height() + 6);
		}
		else
		{
			dc.DrawText(m_strTooltipText, rc, DT_CALCRECT);
			return CSize(rc.Width() + 15, rc.Height() + 6);
		}
	}
	else
	{
		dc.DrawText(m_strTooltipText, rc, DT_CALCRECT | DT_WORDBREAK);
		return CSize(160, rc.Height() + 6);
	}
}

void CXTPCalendarTip::SetTheme(CXTPCalendarTheme* pTheme)
{
	m_pTheme = pTheme;
}
