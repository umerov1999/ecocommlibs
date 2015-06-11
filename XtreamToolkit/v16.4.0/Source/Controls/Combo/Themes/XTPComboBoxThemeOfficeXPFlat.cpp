// XTPComboBoxThemeXP.cpp: implementation of the CXTPComboBoxThemeOfficeXPFlat class.
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
#include "math.h"

#include <Common/XTPWinThemeWrapper.h>
#include <Common/XTPColorManager.h>
#include <Common/XTPDrawHelpers.h>

#include <Controls/Util/XTPControlTheme.h>
#include <Controls/Combo/XTPComboBox.h>
#include <Controls/Combo/XTPComboBoxTheme.h>

#include "XTPComboBoxThemeOfficeXPFlat.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXTPComboBoxThemeOfficeXPFlat::CXTPComboBoxThemeOfficeXPFlat(XTPControlTheme nTheme)
	: m_nTheme(nTheme)
{

}

CXTPComboBoxThemeOfficeXPFlat::~CXTPComboBoxThemeOfficeXPFlat()
{

}

void CXTPComboBoxThemeOfficeXPFlat::RefreshMetrics(CXTPComboBox* pComboBox)
{
	CXTPComboBoxTheme::RefreshMetrics(pComboBox);

	if (m_nTheme == xtpControlThemeFlat)
	{
		const COLORREF clrWindowFrame = GetSysColor(COLOR_WINDOWFRAME);
		m_clrBorderNormal.SetStandardValue(clrWindowFrame);
		m_clrBorderHot.SetStandardValue(clrWindowFrame);
	}
	else if (m_nTheme == xtpControlThemeUltraFlat)
	{
		COLORREF clrFace = GetXtremeColor(COLOR_3DFACE);
		COLORREF clrWindow = GetXtremeColor(COLOR_WINDOW);

		m_clrButtonPressed.SetStandardValue(GetXtremeColor(COLOR_3DSHADOW));
		m_clrButtonPressed.SetStandardValue(XTPColorManager()->LightColor(m_clrButtonPressed, clrFace, 0x32));

		m_clrButtonHot.SetStandardValue(clrFace);
		m_clrButtonHot.SetStandardValue(XTPColorManager()->LightColor(m_clrButtonHot, clrWindow, 0x1E));
	}
	else
	{
		ASSERT(m_nTheme == xtpControlThemeOfficeXP);

		m_clrBorderHot.SetStandardValue(GetXtremeColor(XPCOLOR_HIGHLIGHT_BORDER));

		m_clrButtonHot.SetStandardValue(GetXtremeColor(XPCOLOR_HIGHLIGHT));
		m_clrButtonPressed.SetStandardValue(GetXtremeColor(XPCOLOR_HIGHLIGHT_PUSHED));

		m_clrButtonArrowHot.SetStandardValue(GetXtremeColor(XPCOLOR_HIGHLIGHT_TEXT));
		m_clrButtonArrowPressed.SetStandardValue(GetXtremeColor(XPCOLOR_PUSHED_TEXT));
	}
}

void CXTPComboBoxThemeOfficeXPFlat::DrawBackground(CDC *pDC, CXTPComboBox* pComboBox)
{
	CXTPComboBoxTheme::DrawBackground(pDC, pComboBox);

	CRect rc = pComboBox->GetComboBoxRect();

	BOOL bPressed = pComboBox->SendMessage(CB_GETDROPPEDSTATE, 0, 0L) != 0;
	BOOL bSelected = pComboBox->IsFocused() || pComboBox->IsHighlighted();
	BOOL bEnabled = pComboBox->IsWindowEnabled();

	COLORREF clrBorders = bSelected? m_clrBorderHot : m_clrBorderNormal;

	if (!bSelected && !bPressed && bEnabled && pComboBox->GetFlatStyle())
	{
		pDC->Draw3dRect(rc, GetXtremeColor(COLOR_3DFACE), GetXtremeColor(COLOR_3DFACE));
	}
	else
	{
		pDC->Draw3dRect(rc, bEnabled ? clrBorders : GetXtremeColor(COLOR_3DSHADOW), bEnabled ? clrBorders : GetXtremeColor(COLOR_3DSHADOW));
	}
}

void CXTPComboBoxThemeOfficeXPFlat::DrawButton(CDC *pDC, CXTPComboBox* pComboBox, CRect rcBtn)
{
	CRect rc = pComboBox->GetComboBoxRect();
	HBRUSH hBrush = pComboBox->GetClientBrush(pDC);

	BOOL bPressed = pComboBox->SendMessage(CB_GETDROPPEDSTATE, 0, 0L) != 0;
	BOOL bSelected = pComboBox->IsFocused() || pComboBox->IsHighlighted();
	BOOL bEnabled = pComboBox->IsWindowEnabled();

	COLORREF clrBorders = bSelected? m_clrBorderHot : m_clrBorderNormal;

	if (!bSelected && !bPressed && bEnabled && pComboBox->GetFlatStyle())
	{
		COLORREF clrButton = m_clrButtonNormal;
		pDC->FillSolidRect(rcBtn, clrButton);

		DrawFrame(pDC->GetSafeHdc(), rcBtn, 1, hBrush);
	}
	else
	{
		CRect rcFill = rcBtn;

		if ((m_nTheme == xtpControlThemeFlat) && !bPressed)
			rcFill.DeflateRect(1, 1);

		COLORREF clrButton = bPressed ? m_clrButtonPressed : bSelected ? m_clrButtonHot : m_clrButtonNormal;
		pDC->FillSolidRect(rcFill, clrButton);

		CXTPPenDC pen(pDC->GetSafeHdc(), bEnabled ? clrBorders : GetXtremeColor(COLOR_3DSHADOW));

		pDC->MoveTo(rcBtn.left - 1, rcBtn.top);
		pDC->LineTo(rcBtn.left - 1, rcBtn.bottom);
	}
}
