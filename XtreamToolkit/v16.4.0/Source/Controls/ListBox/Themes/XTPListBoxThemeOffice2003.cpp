// XTPListBoxThemeOffice2003.cpp : implementation of the CXTPListBoxThemeOffice2003 class.
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

#include <Common/XTPColorManager.h>
#include <Common/XTPDrawHelpers.h>
#include <Common/XTPResourceImage.h>

#include <Controls/Util/XTPControlTheme.h>
#include <Controls//Util/XTPGlobal.h>
#include <Controls/ListBox/XTPListBox.h>
#include <Controls/ListBox/Themes/XTPListBoxThemeOfficeXP.h>
#include <Controls/ListBox/Themes/XTPListBoxThemeOffice2003.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


void CXTPListBoxThemeOffice2003::DrawItem(CDC *pDC, CXTPListBox *pListBox, LPDRAWITEMSTRUCT lpDIS)
{
	CXTPListBoxThemeOfficeXP::DrawItem(pDC, pListBox, lpDIS);
}

void CXTPListBoxThemeOffice2003::RefreshMetrics()
{
	CXTPListBoxThemeOfficeXP::RefreshMetrics();
}
