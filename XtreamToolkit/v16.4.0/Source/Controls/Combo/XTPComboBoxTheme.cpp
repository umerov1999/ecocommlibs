// XTPComboBoxTheme.cpp: implementation of the CXTPComboBoxTheme class.
//
// This file is a part of the XTREME CONTROLS MFC class library.
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

#include <Common/XTPWinThemeWrapper.h>
#include <Common/XTPColorManager.h>
#include <Common/XTPDrawHelpers.h>

#include <Controls/Util/XTPControlTheme.h>
#include <Controls/Combo/XTPComboBox.h>
#include <Controls/Combo/XTPComboBoxTheme.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXTPComboBoxTheme::CXTPComboBoxTheme()
	: m_nThumbWidth(0)
{
	m_themeComboBox = new CXTPWinThemeWrapper();
}

CXTPComboBoxTheme::~CXTPComboBoxTheme()
{
	SAFE_DELETE(m_themeComboBox);
}

void CXTPComboBoxTheme::RefreshMetrics(CXTPComboBox* pComboBox)
{
	RefreshXtremeColors(FALSE);

	// Set default theme colors.
	const COLORREF clrShadow = GetSysColor(COLOR_3DSHADOW);
	m_clrBorderDisabled.SetStandardValue(clrShadow);
	m_clrBorderNormal.SetStandardValue(clrShadow);
	m_clrBorderHot.SetStandardValue(clrShadow);

	const COLORREF clrBtnFace = GetSysColor(COLOR_BTNFACE);
	m_clrButtonNormal.SetStandardValue(clrBtnFace);
	m_clrButtonHot.SetStandardValue(clrBtnFace);
	m_clrButtonPressed.SetStandardValue(clrBtnFace);

	const COLORREF clrBtnText = GetXtremeColor(COLOR_BTNTEXT);
	m_clrButtonArrowNormal.SetStandardValue(clrBtnText);
	m_clrButtonArrowHot.SetStandardValue(clrBtnText);
	m_clrButtonArrowPressed.SetStandardValue(clrBtnText);

	m_clrBackground.SetStandardValue(GetXtremeColor(COLOR_WINDOW));
	m_clrText.SetStandardValue(GetXtremeColor(COLOR_WINDOWTEXT));

	m_themeComboBox->OpenTheme(0, L"COMBOBOX");

	// Init thumb width.
	if (pComboBox->GetSafeHwnd())
	{
		CClientDC dc(pComboBox);
		CXTPFontDC font(&dc, pComboBox->GetFont());

		TEXTMETRIC tm;
		dc.GetTextMetrics(&tm);
		m_nThumbWidth = max(15, tm.tmAveCharWidth * 2 + 4);

		m_nThumbWidth = min(m_nThumbWidth, GetSystemMetrics(SM_CXVSCROLL) + 1);
	}

	UpdateBackgroundBrush();
}

void CXTPComboBoxTheme::UpdateBackgroundBrush()
{
	m_brBackground.DeleteObject();
	m_brBackground.CreateSolidBrush(m_clrBackground);
}

BOOL CXTPComboBoxTheme::DrawComboBox(CDC* pDC, CXTPComboBox* pComboBox)
{
	CRect rc = pComboBox->GetComboBoxRect();
	BOOL bRightAlign = pComboBox->GetExStyle() & WS_EX_RIGHT;

	CRect rcBtn(rc.right - m_nThumbWidth, rc.top, rc.right, rc.bottom);
	if (bRightAlign)
		rcBtn = CRect(rc.left, rc.top, rc.left + m_nThumbWidth, rc.bottom);

	rcBtn.DeflateRect(1, 1);

	DrawBackground(pDC, pComboBox);

	DrawButton(pDC, pComboBox, rcBtn);

	DrawButtonTriangle(pDC, pComboBox, rcBtn);

	return TRUE;
}

void CXTPComboBoxTheme::DrawFrame(HDC hdc, LPRECT lprc, int nSize, HBRUSH hBrush)
{
	FillSolidRect(hdc, lprc->left, lprc->top, lprc->right - lprc->left - nSize, nSize, hBrush);
	FillSolidRect(hdc, lprc->left, lprc->top, nSize, lprc->bottom - lprc->top - nSize, hBrush);
	FillSolidRect(hdc, lprc->right - nSize, lprc->top, nSize, lprc->bottom - lprc->top, hBrush);
	FillSolidRect(hdc, lprc->left, lprc->bottom - nSize, lprc->right - lprc->left, nSize, hBrush);
}

void CXTPComboBoxTheme::FillSolidRect(HDC hdc, int x, int y, int cx, int cy, HBRUSH hBrush)
{
	::FillRect(hdc, CRect(x, y, x + cx, y + cy), hBrush);
}

void CXTPComboBoxTheme::DrawBackground(CDC *pDC, CXTPComboBox* pComboBox)
{
	CRect rc = pComboBox->GetComboBoxRect();
	HBRUSH hBrush = pComboBox->GetClientBrush(pDC);
	BOOL bRightAlign = pComboBox->GetExStyle() & WS_EX_RIGHT;

	pDC->SelectClipRgn(NULL);

	DrawFrame(pDC->GetSafeHdc(), rc, 3, hBrush);

	if (bRightAlign)
		FillRect(pDC->GetSafeHdc(), CRect(rc.left, rc.top, rc.left + GetSystemMetrics(SM_CXVSCROLL) + 3, rc.bottom), hBrush);
	else
		FillRect(pDC->GetSafeHdc(), CRect(rc.right - GetSystemMetrics(SM_CXVSCROLL) - 3, rc.top, rc.right, rc.bottom), hBrush);
}

void CXTPComboBoxTheme::DrawButton(CDC *pDC, CXTPComboBox* pComboBox, CRect rcBtn)
{
	UNREFERENCED_PARAMETER(pDC);
	UNREFERENCED_PARAMETER(pComboBox);
	UNREFERENCED_PARAMETER(rcBtn);
}

void CXTPComboBoxTheme::DrawButtonTriangle(CDC *pDC, CXTPComboBox* pComboBox, CRect rcBtn)
{
	BOOL bPressed = pComboBox->SendMessage(CB_GETDROPPEDSTATE, 0, 0L) != 0;
	BOOL bSelected = pComboBox->IsFocused() || pComboBox->IsHighlighted();
	BOOL bEnabled = pComboBox->IsWindowEnabled();

	CPoint pt(rcBtn.CenterPoint());
	int nWidth = max(2, rcBtn.Width() / 2 - 4);

	XTPDrawHelpers()->Triangle(pDC, CPoint(pt.x - nWidth, pt.y - nWidth/2), CPoint(pt.x + nWidth, pt.y - nWidth/2),
		CPoint (pt.x , pt.y - nWidth/2 + nWidth),
		!bEnabled ? GetXtremeColor(XPCOLOR_DISABLED) : bPressed ? m_clrButtonArrowPressed:
		bSelected ? m_clrButtonArrowHot : m_clrButtonArrowNormal);

	if (!bEnabled && pComboBox->GetFlatStyle()) // draw line vertical line when disabled and flat
	{
		CXTPPenDC pen(pDC->GetSafeHdc(), m_clrButtonNormal);
		pDC->MoveTo(rcBtn.left-1,rcBtn.top);
		pDC->LineTo(rcBtn.left-1,rcBtn.bottom);
	}
}
