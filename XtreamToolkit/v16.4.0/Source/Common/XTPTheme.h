// XTPTheme.h:
//
// This file is a part of the XTREME CALENDAR MFC class library.
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

//{{AFX_CODEJOCK_PRIVATE
#if !defined(_XTPTHEME_H_)
#define _XTPTHEME_H_
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXTPResourceImages;

struct _XTP_EXT_CLASS CXTPColorSetCaptionButton
{
	void RefreshMetrics(CXTPResourceImages *pImages, LPCTSTR lpszSectionName);

	CXTPPaintManagerColor m_clrBackground;   // Background color
	CXTPPaintManagerColor m_clrOutline;      // Outline color
	CXTPPaintManagerColor m_clrFill;         // Fill color
};


#if 0
enum XTPTheme
{
	// Visual Studio
	xtpThemeVisualStudio97   = 1997,
	xtpThemeVisualStudio6    = 1998,
	xtpThemeVisualStudio2002 = 2002,
	xtpThemeVisualStudio2003 = 2003,
	xtpThemeVisualStudio2005 = 2005,
	xtpThemeVisualStudio2008 = 2008,
	xtpThemeVisualStudio2010 = 2010,
	xtpThemeVisualStudio2012 = 2012,

	// Office
	xtpThemeOffice2000       = 2000, // Office 2000 theme
	xtpThemeOfficeXP         = 2001, // Office XP   theme
	xtpThemeOffice2003       = 2003, // Office 2003 theme
	xtpThemeOffice2007       = 2007, // Office 2007 theme
	xtpThemeOffice2010       = 2010, // Office 2010 theme
	xtpThemeOffice2013       = 2013, // Office 2010 theme
};
#endif

enum XTPThemeOffice2013
{
	xtpThemeOffice2013Word,
	xtpThemeOffice2013Excel,
};

#endif // !defined(_XTPTHEME_H_)
