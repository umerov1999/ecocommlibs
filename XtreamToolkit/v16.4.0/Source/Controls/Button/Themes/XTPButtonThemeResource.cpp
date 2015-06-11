// XTPButtonThemeResource.cpp: implementation of the CXTPButtonThemeResource class.
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
#include <Common/XTPMarkupRender.h>
#include <Common/XTPDrawHelpers.h>
#include <Common/XTPResourceImage.h>
#include <Common/XTPImageManager.h>

#include <Controls/Util/XTPControlTheme.h>
#include <Controls/Button/XTPButton.h>
#include <Controls/Button/XTPButtonTheme.h>
#include <Controls/Button/Themes/XTPButtonThemeUltraFlat.h>
#include <Controls/Button/Themes/XTPButtonThemeResource.h>

#ifndef _XTP_ACTIVEX_BUTTON
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPButtonThemeResource

CXTPButtonThemeResource::CXTPButtonThemeResource()
{
	m_nTheme = xtpControlThemeResource;
	m_nBorderWidth = 2;
	m_bHiglightButtons = TRUE;
}


void CXTPButtonThemeResource::RefreshMetrics(CXTPButton* pButton)
{
	CXTPButtonThemeUltraFlat::RefreshMetrics(pButton);

	if (XTPResourceImages()->IsValid())
	{
		m_crBorderShadow.SetStandardValue(XTPResourceImages()->GetImageColor(_T("Controls"), _T("GroupBoxFrame")));
	}
}

void CXTPButtonThemeResource::DrawCheckBoxMark(CDC* pDC, CXTPButton* pButton)
{
	if (IsVisualThemeUsed(pButton))
	{
		CXTPButtonTheme::DrawCheckBoxMark(pDC, pButton);
		return;
	}

	CXTPResourceImage* pImage = XTPResourceImages()->LoadFile(_T("TOOLBARBUTTONCHECKBOX"));
	if (!pImage)
	{
		CXTPButtonTheme::DrawCheckBoxMark(pDC, pButton);
		return;
	}

	BOOL bPressed = pButton->IsPushed();
	BOOL bSelected = pButton->IsHighlighted();
	BOOL bEnabled = pButton->IsWindowEnabled();
	BOOL bChecked = pButton->GetChecked();


	CRect rc;
	CalcRect(pDC, pButton, &rc, CBR_CHECKBOX);


	int nState = 0;
	if (!bEnabled)
		nState = 3;
	else if (bSelected && bPressed)
		nState = 2;
	else if (bSelected)
		nState = 1;

	if (bChecked == 1)
		nState += 4;

	if (bChecked == 2)
		nState += 8;

	pImage->DrawImage(pDC, rc, pImage->GetSource(nState, 12), CRect(0, 0, 0, 0));
}

void CXTPButtonThemeResource::DrawRadioButtonMark(CDC* pDC, CXTPButton* pButton)
{
	if (IsVisualThemeUsed(pButton))
	{
		CXTPButtonTheme::DrawRadioButtonMark(pDC, pButton);
		return;
	}

	CXTPResourceImage* pImage = XTPResourceImages()->LoadFile(_T("TOOLBARBUTTONRADIOBUTTON"));
	if (!pImage)
	{
		CXTPButtonTheme::DrawRadioButtonMark(pDC, pButton);
		return;
	}

	CRect rc;
	CalcRect(pDC, pButton, &rc, CBR_RADIOBOX);


	BOOL bPressed = pButton->IsPushed();
	BOOL bSelected = pButton->IsHighlighted();
	BOOL bEnabled = pButton->IsWindowEnabled();
	BOOL bChecked = pButton->GetChecked();

	int nState = 0;
	if (!bEnabled)
		nState = 3;
	else if (bSelected && bPressed)
		nState = 2;
	else if (bSelected)
		nState = 1;

	if (bChecked)
		nState += 4;

	pImage->DrawImage(pDC, rc, pImage->GetSource(nState, 8), CRect(0, 0, 0, 0));
}

void CXTPButtonThemeResource::DrawButtonBackground(CDC* pDC, CXTPButton* pButton)
{
	BOOL bPressed = pButton->IsPushed();
	BOOL bSelected = pButton->IsHighlighted();
	BYTE bStyle = pButton->GetButtonStyle();
	BOOL bDefault = bStyle == BS_DEFPUSHBUTTON;
	BOOL bEnabled = pButton->IsWindowEnabled();
	BOOL bChecked = pButton->GetChecked();
	CRect rc(pButton->GetButtonRect());

	if (pButton->GetFlatStyle() && !bSelected && !bPressed && !bChecked)
		return;

	if (pButton->GetUseVisualStyle() && m_themeButton->IsAppThemed())
	{
		DrawButtonVisualStyleBackground(pDC, pButton);
		return;
	}

	CXTPResourceImage* pImage = XTPResourceImages()->LoadFile(_T("PUSHBUTTON"));
	if (!pImage)
	{
		CXTPButtonTheme::DrawButtonBackground(pDC, pButton);
		return;
	}

	int nState = bDefault ? 4 : 0;

	if (!bEnabled)
	{
		nState = 3;
	}
	else if (bChecked && !bSelected && !bPressed) nState = 2;
	else if (bChecked && bSelected && !bPressed) nState = 1;
	else if ((bSelected && bPressed) || pButton->IsPushed()) nState = 2;
	else if (bSelected || bPressed) nState = 1;

	if (nState != -1)
	{
		pImage->DrawImage(pDC, rc, pImage->GetSource(nState, 5), CRect(4, 4, 4, 4), 0xFF00FF);
	}
}
