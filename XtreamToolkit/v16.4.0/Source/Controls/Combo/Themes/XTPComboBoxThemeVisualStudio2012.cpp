// XTPComboBoxThemeVisualStudio2012.cpp: implementation of the CXTPComboBoxThemeVisualStudio20122012 class.
//
// This file is a part of the XTREME CONTROLS MFC class library.
// (c)1998-2013 Codejock Software, All Rights Reserved.
//
// THIS SOURCE FILE IS THE PROPERTY OF CODEJOCK SOFTWARE AND IS NOT TO BE
// RE-DISTRIBUTED BY ANY MEANS WHATSOEVER WITHOUT THE E2003RESSED WRITTEN
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

#include "XTPComboBoxThemeOffice2003.h"
#include "XTPComboBoxThemeVisualStudio.h"
#include "XTPComboBoxThemeVisualStudio2012.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXTPComboBoxThemeVisualStudio2012::CXTPComboBoxThemeVisualStudio2012(BOOL bLight)
	: m_bLight(bLight)
{
	m_bHighlight = TRUE;
}

CXTPComboBoxThemeVisualStudio2012::~CXTPComboBoxThemeVisualStudio2012()
{

}

void CXTPComboBoxThemeVisualStudio2012::RefreshMetrics(CXTPComboBox* pComboBox)
{
	CXTPComboBoxTheme::RefreshMetrics(pComboBox);

	if (m_bLight)
	{
		//borders
		m_clrBorderNormal.SetStandardValue(RGB(219, 221, 230));
		m_clrBorderHot.SetStandardValue(m_clrBorderNormal);
		m_clrBorderDisabled.SetStandardValue(RGB(255, 0, 0));//does not work

		//button
		m_clrButtonNormal.SetStandardValue(RGB(239, 239, 242));//color of button when combobox is disabled

		m_grcButton = RGB(252, 252, 252);//color of button when combobox is enabled
		m_grcButtonHilite.SetStandardValue(RGB(222, 223, 232));
		m_grcButtonPushed = RGB(0, 122, 204);

		//arrow of button
		m_clrButtonArrowNormal.SetStandardValue(RGB(113, 113, 113));
		m_clrButtonArrowPressed.SetStandardValue(RGB(255, 255, 255));
		m_clrButtonArrowHot.SetStandardValue(RGB(0, 122, 204));

		m_clrText.SetStandardValue(RGB(30, 30, 30));
		m_clrBackground.SetStandardValue(RGB(252, 252, 252));

		if (pComboBox->IsHighlighted() && pComboBox->IsFocused())
		{
			// background doesn't change here
		}
	}
	else
	{
		//borders
		m_clrBorderNormal.SetStandardValue(RGB(67, 67, 70));
		m_clrBorderHot.SetStandardValue(m_clrBorderNormal);
		m_clrBorderDisabled.SetStandardValue(RGB(255, 0, 0));//does not work

		//button
		m_clrButtonNormal.SetStandardValue(RGB(45, 45, 48));//color of button when combobox is disabled

		m_grcButton = RGB(51, 51, 55);//color of button when combobox is enabled
		m_grcButtonHilite.SetStandardValue(RGB(31, 31, 32));
		m_grcButtonPushed = RGB(0, 122, 204);

		//arrow of button
		m_clrButtonArrowNormal.SetStandardValue(RGB(241, 241, 241));
		m_clrButtonArrowPressed.SetStandardValue(RGB(255, 255, 255));
		m_clrButtonArrowHot.SetStandardValue(RGB(0, 122, 204));

		m_clrText.SetStandardValue(RGB(255, 255, 255));
		m_clrBackground.SetStandardValue(RGB(51, 51, 55));

		if (pComboBox->IsHighlighted() && pComboBox->IsFocused())
		{
			m_clrBackground.SetStandardValue(RGB(63,63,70));
		}
	}

	CXTPComboBoxTheme::UpdateBackgroundBrush();
}

void CXTPComboBoxThemeVisualStudio2012::DrawButton(CDC *pDC, CXTPComboBox* pComboBox, CRect rcBtn)
{
	CXTPComboBoxThemeVisualStudio::DrawButton(pDC, pComboBox, rcBtn);

	// the below 3 lines of code disable drawing of border line in Office 2003 theme
	CXTPPenDC pen(pDC->GetSafeHdc(), m_clrBackground);
	pDC->MoveTo(rcBtn.TopLeft());
	pDC->LineTo(rcBtn.left, rcBtn.bottom);

	BOOL bEnabled = pComboBox->IsWindowEnabled();

	if (pComboBox->IsHighlighted())
	{
		COLORREF clrHoverLine = m_bLight ? RGB(0,122,204) : RGB(10,87,137);

		CXTPPenDC pen(pDC->GetSafeHdc(), bEnabled ? clrHoverLine : GetXtremeColor(COLOR_3DSHADOW));

		pDC->MoveTo(rcBtn.TopLeft());
		pDC->LineTo(rcBtn.left, rcBtn.bottom);
	}

}
