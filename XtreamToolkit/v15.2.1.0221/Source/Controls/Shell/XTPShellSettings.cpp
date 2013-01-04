// XTPShellSettings.cpp: implementation of the CXTPShellSettings class.
//
// This file is a part of the XTREME CONTROLS MFC class library.
// (c)1998-2012 Codejock Software, All Rights Reserved.
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
#include "Common/XTPSystemHelpers.h"
#include "XTPShellSettings.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const TCHAR COLORKEY[] = _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer");

//////////////////////////////////////////////////////////////////////
// CXTPShellSettings

CXTPShellSettings::CXTPShellSettings()
{
	RefreshSettings();
}

CXTPShellSettings::~CXTPShellSettings()
{
}

//////////////////////////////////////////////////////////////////////
//

void CXTPShellSettings::RefreshSettings()
{
	::ZeroMemory((SHELLFLAGSTATE*)this, sizeof(SHELLFLAGSTATE));

	typedef void (WINAPI* SHGETSETTINGSPROC)(LPSHELLFLAGSTATE lpSFS, DWORD dwMask);
	SHGETSETTINGSPROC SHGetSettings = NULL;

	if (CXTPModuleHandle(_T("shell32.dll")).GetProcAddress((FARPROC*)&SHGetSettings, "SHGetSettings"))
	{
		SHGetSettings((SHELLFLAGSTATE*)this,
			SSF_DESKTOPHTML |
			SSF_DONTPRETTYPATH |
			SSF_DOUBLECLICKINWEBVIEW |
			SSF_HIDEICONS |
			SSF_MAPNETDRVBUTTON |
			SSF_NOCONFIRMRECYCLE |
			SSF_SHOWALLOBJECTS |
			SSF_SHOWATTRIBCOL |
			SSF_SHOWCOMPCOLOR |
			SSF_SHOWEXTENSIONS |
			SSF_SHOWINFOTIP |
			SSF_SHOWSYSFILES |
			SSF_WIN95CLASSIC);
	}

	m_crCompColor = RGB(0x00, 0x00, 0xff); // default blue color.
	m_crEncrColor = RGB(0x13, 0x92, 0x0d); // default green color.

	HKEY hKey = NULL;
	if (::RegOpenKeyEx(HKEY_CURRENT_USER, COLORKEY, 0, KEY_QUERY_VALUE, &hKey) == ERROR_SUCCESS)
	{
		DWORD uBytes = sizeof(COLORREF);
		DWORD dwType = REG_BINARY;
		COLORREF crCompColor;
		COLORREF crEncrColor;

		// try to locate user defined compressed file color.
		if (::RegQueryValueEx(hKey, _T("AltColor"), NULL, &dwType, (LPBYTE)&crCompColor, &uBytes) == ERROR_SUCCESS)
		{
			m_crCompColor = crCompColor;
		}

		uBytes = sizeof(COLORREF);
		// try to locate user defined encrypted file color.
		if (::RegQueryValueEx(hKey, _T("AltEncryptionColor"), NULL, &dwType, (LPBYTE)&crEncrColor, &uBytes) == ERROR_SUCCESS)
		{
			m_crEncrColor = crEncrColor;
		}

		::RegCloseKey(hKey);
	}
}
