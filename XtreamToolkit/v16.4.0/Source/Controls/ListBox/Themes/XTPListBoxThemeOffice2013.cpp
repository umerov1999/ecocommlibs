// XTPListBoxThemeOffice2013.cpp : implementation of the CXTPListBoxThemeOffice2013 class.
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
#include <Controls/ListBox/Themes/XTPListBoxThemeOffice2013.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CXTPListBoxThemeOffice2013::CXTPListBoxThemeOffice2013()
{
}


void CXTPListBoxThemeOffice2013::DrawItem(CDC *pDC, CXTPListBox *pListBox, LPDRAWITEMSTRUCT lpDIS)
{
	RefreshMetrics();

	CRect rcItem(lpDIS->rcItem);

	BOOL bSelected = ((lpDIS->itemState & ODS_SELECTED) == ODS_SELECTED);
	BOOL bHot      = (int)lpDIS->itemID == pListBox->m_nHotItem;

	CXTPListBoxThemeOffice2013Colors *pColors = NULL;

	if (bHot)
	{
		pColors = &m_colorHot;
	}
	else if (bSelected)
	{
		pColors = &m_colorSelected;
	}
	else
	{
		pColors = &m_colorNormal;
	}

	pDC->FillSolidRect(rcItem, pColors->m_clrBackground);

	CRect rcText(rcItem);
	rcText.DeflateRect(pListBox->m_nTextPadding, 0);
	DrawItemText(pDC, rcText, pListBox, lpDIS);
}

void CXTPListBoxThemeOffice2013::RefreshMetrics()
{
	LPCTSTR pszKeyNormal   = _T("ListBox.Normal");
	LPCTSTR pszKeyHot      = _T("ListBox.Hot");
	LPCTSTR pszKeySelected = _T("ListBox.Selected");

	CXTPResourceImages *pImages = XTPResourceImages();

	m_colorNormal.m_clrBackground = pImages->GetImageColor(pszKeyNormal, _T("BackgroundColor"));
//  m_colorNormal.m_clrText       = ;

	m_colorHot.m_clrBackground = pImages->GetImageColor(pszKeyHot, _T("BackgroundColor"));

	m_colorSelected.m_clrBackground = pImages->GetImageColor(pszKeySelected, _T("BackgroundColor"));
}
