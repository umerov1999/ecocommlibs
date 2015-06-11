// XTPPopupThemeOffice2013.cpp: implementation of the CXTPPopupThemeOffice2013 class.
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

#include "stdafx.h"
#include "Common/XTPImageManager.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPColorManager.h"

#include "../../Defines.h"

#include "../XTPPopupControl.h"
#include "../XTPPopupPaintManager.h"
#include "../XTPPopupItem.h"

#include "XTPPopupThemeOffice2000.h"
#include "XTPPopupThemeOfficeXP.h"
#include "XTPPopupThemeOffice2003.h"
#include "XTPPopupThemeOffice2013.h"


//////////////////////////////////////////////////////////////////////////
// CXTPPopupThemeOffice2013

CXTPPopupThemeOffice2013::CXTPPopupThemeOffice2013()
{
}

CXTPPopupThemeOffice2013::~CXTPPopupThemeOffice2013()
{
}

void CXTPPopupThemeOffice2013::RefreshMetrics()
{
	CXTPPopupThemeOffice2003::RefreshMetrics();

	m_clrGripper = CXTPPaintManagerColorGradient(RGB(170,169,187), RGB(122,117,147));
	m_clrFrame = RGB(167, 171, 176);
	m_clrBackground = CXTPPaintManagerColorGradient(RGB(232,235,241), RGB(179,186,196));
	m_clrText = RGB(68,68,68);

	m_clrButtonSelectedBorder = RGB(146,192,224);
	m_clrButtonSelected = RGB(205,230,247);

	m_clrButtonPressedBorder = RGB(42,141,212);
	m_clrButtonPressed = RGB(146,192,224);

	m_iButtonBorderInflate = 2;
}