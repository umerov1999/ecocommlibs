// XTPComboBoxTheme2000.cpp: implementation of the CXTPComboBoxThemeOffice2000 class.
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

#include <Controls/Util/XTPControlTheme.h>
#include <Controls/Combo/XTPComboBox.h>
#include <Controls/Combo/XTPComboBoxTheme.h>

#include "XTPComboBoxThemeOffice2000.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXTPComboBoxThemeOffice2000::CXTPComboBoxThemeOffice2000()
{

}

CXTPComboBoxThemeOffice2000::~CXTPComboBoxThemeOffice2000()
{

}

void CXTPComboBoxThemeOffice2000::RefreshMetrics(CXTPComboBox* pComboBox)
{
	CXTPComboBoxTheme::RefreshMetrics(pComboBox);
}

void CXTPComboBoxThemeOffice2000::DrawBackground(CDC *pDC, CXTPComboBox* pComboBox)
{
	CXTPComboBoxTheme::DrawBackground(pDC, pComboBox);

	CRect rc = pComboBox->GetComboBoxRect();

	BOOL bPressed = pComboBox->SendMessage(CB_GETDROPPEDSTATE, 0, 0L) != 0;
	BOOL bSelected = pComboBox->IsFocused() || pComboBox->IsHighlighted();
	BOOL bEnabled = pComboBox->IsWindowEnabled();

	if (!bSelected && !bPressed && bEnabled && pComboBox->GetFlatStyle())
	{
		pDC->Draw3dRect(rc, GetXtremeColor(COLOR_3DFACE), GetXtremeColor(COLOR_3DFACE));
	}
	else
	{
		if (pComboBox->GetFlatStyle() && !bEnabled)
		{
			pDC->Draw3dRect(rc, GetXtremeColor(COLOR_3DSHADOW), GetXtremeColor(COLOR_3DSHADOW));
		}
		else
		{
			pDC->Draw3dRect(rc, GetXtremeColor(COLOR_3DSHADOW), GetXtremeColor(COLOR_3DHILIGHT));
			pDC->Draw3dRect(rc.left + 1, rc.top + 1, rc.Width() - 2, rc.Height() - 2,
				GetXtremeColor(COLOR_BTNFACE), GetXtremeColor(COLOR_BTNFACE));
		}
	}
}

void CXTPComboBoxThemeOffice2000::DrawButton(CDC *pDC, CXTPComboBox* pComboBox, CRect rcBtn)
{
	CRect rc = pComboBox->GetComboBoxRect();
	HBRUSH hBrush = pComboBox->GetClientBrush(pDC);

	BOOL bPressed = pComboBox->SendMessage(CB_GETDROPPEDSTATE, 0, 0L) != 0;
	BOOL bSelected = pComboBox->IsFocused() || pComboBox->IsHighlighted();
	BOOL bEnabled = pComboBox->IsWindowEnabled();

	if (!bSelected && !bPressed && bEnabled && pComboBox->GetFlatStyle())
	{
		COLORREF clrButton = m_clrButtonNormal;
		pDC->FillSolidRect(rcBtn, clrButton);

		DrawFrame(pDC->GetSafeHdc(), rcBtn, 1, hBrush);
	}
	else
	{
		if (pComboBox->GetFlatStyle() && !bEnabled)
		{

		}
		else
		{
			COLORREF clrButton = bPressed ? m_clrButtonPressed : bSelected ? m_clrButtonHot : m_clrButtonNormal;
			pDC->FillSolidRect(rcBtn, clrButton);

			pDC->Draw3dRect(rcBtn.left + 1, rcBtn.top + 1, rcBtn.Width() - 2, rcBtn.Height() - 2,
				GetXtremeColor(bPressed ? COLOR_3DSHADOW : COLOR_3DHILIGHT), GetXtremeColor(bPressed ? COLOR_3DHILIGHT : COLOR_3DSHADOW));
		}
	}
}
