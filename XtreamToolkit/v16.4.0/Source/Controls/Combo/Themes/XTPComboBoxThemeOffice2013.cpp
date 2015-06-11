// XTPComboBoxThemeOffice2013.cpp: implementation of the CXTPComboBoxThemeOffice2013 class.
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

#include <Common/XTPWinThemeWrapper.h>
#include <Common/XTPColorManager.h>
#include <Common/XTPDrawHelpers.h>

#include <Controls/Util/XTPControlTheme.h>
#include <Controls/Combo/XTPComboBox.h>
#include <Controls/Combo/XTPComboBoxTheme.h>

#include "XTPComboBoxThemeOffice2003.h"
#include "XTPComboBoxThemeVisualStudio.h"
#include "XTPComboBoxThemeOffice2013.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXTPComboBoxThemeOffice2013::CXTPComboBoxThemeOffice2013()
{
	m_bHighlight = FALSE;
}

CXTPComboBoxThemeOffice2013::~CXTPComboBoxThemeOffice2013()
{

}

void CXTPComboBoxThemeOffice2013::RefreshMetrics(CXTPComboBox* pComboBox)
{
	CXTPComboBoxTheme::RefreshMetrics(pComboBox);

	//borders
	m_clrBorderNormal.SetStandardValue(RGB(171,171,171));
	m_clrBorderHot.SetStandardValue(RGB(163,189,227));
	m_clrBorderDisabled.SetStandardValue(RGB(255, 0, 0));//does not work

	//button
	m_clrButtonNormal.SetStandardValue(RGB(239, 239, 242));//color of button when combobox is disabled

	//arrow of button
	m_clrButtonArrowNormal.SetStandardValue(RGB(119,119,119));
	m_clrButtonArrowPressed.SetStandardValue(RGB(68, 68, 68));
	m_clrButtonArrowHot.SetStandardValue(RGB(68, 68, 68));

	m_clrText.SetStandardValue(RGB(68, 68, 68));
	m_clrBackground.SetStandardValue(RGB(255, 255, 255));

	m_grcButton = m_clrBackground;//color of button when combobox is enabled
	m_grcButtonHilite.SetStandardValue(RGB(213,225,242));
	m_grcButtonPushed = RGB(163,189,227);

	CXTPComboBoxTheme::UpdateBackgroundBrush();
}

void CXTPComboBoxThemeOffice2013::DrawButton(CDC *pDC, CXTPComboBox* pComboBox, CRect rcBtn)
{
	CXTPComboBoxThemeVisualStudio::DrawButton(pDC, pComboBox, rcBtn);

	if (!pComboBox->IsHighlighted() && !pComboBox->IsFocused())
	{
		// the below 3 lines of code disable drawing of border line in Office 2003 theme
		CXTPPenDC pen(pDC->GetSafeHdc(), m_clrBackground);
		pDC->MoveTo(rcBtn.TopLeft());
		pDC->LineTo(rcBtn.left, rcBtn.bottom);
	}
}
