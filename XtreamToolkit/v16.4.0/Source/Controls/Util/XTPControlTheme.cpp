// XTPControlTheme.cpp : implementation file
//
// This file is a part of the XTREME TOOLKIT PRO MFC class library.
// (c)1998-2013 Codejock Software, All Rights Reserved.
//
// THIS SOURCE FILE IS THE PROPERTY OF CODEJOCK SOFTWARE AND IS NOT TO
// BE RE-DISTRIBUTED BY ANY MEANS WHATSOEVER WITHOUT THE EXPRESSED
// WRITTEN CONSENT OF CODEJOCK SOFTWARE.
//
// THIS SOURCE CODE CAN ONLY BE USED UNDER THE TERMS AND CONDITIONS
// OUTLINED IN THE XTREME TOOLKIT PRO LICENSE AGREEMENT. CODEJOCK
// SOFTWARE GRANTS TO YOU (ONE SOFTWARE DEVELOPER) THE LIMITED RIGHT
// TO USE THIS SOFTWARE ON A SINGLE COMPUTER.
//
// CONTACT INFORMATION:
// support@codejock.com
// http://www.codejock.com
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "../Defines.h"
#include "XTPControlTheme.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTPControlTheme

CXTPControlTheme::CXTPControlTheme()
: m_nTheme(xtpControlThemeDefault)
{

}

XTPControlTheme CXTPControlTheme::GetThemeStyle() const
{
	return m_nTheme;
}

void CXTPControlTheme::SetTheme(HWND hWnd, XTPControlTheme nTheme)
{
	for (HWND hWndChild = ::GetTopWindow(hWnd); hWndChild != NULL;
		hWndChild = ::GetNextWindow(hWndChild, GW_HWNDNEXT))
	{
		::PostMessage(hWndChild, WM_XTP_SETCONTROLTHEME, (WPARAM)nTheme, (LPARAM)(nTheme == xtpControlThemeResource));

		if (::GetTopWindow(hWndChild) != NULL)
		{
			SetTheme(hWndChild, nTheme);
		}
	}
}
