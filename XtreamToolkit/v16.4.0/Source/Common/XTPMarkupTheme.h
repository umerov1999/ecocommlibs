// XTPMarkupTheme.h: Interface for the CXTPMarkupThemePart class.
//
// This file is a part of the XTREME RIBBON MFC class library.
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
#if !defined(__XTPMARKUPTHEME_H__)
#define __XTPMARKUPTHEME_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CXTPMarkupContext;
class CXTPMarkupUIElement;

//===========================================================================
// Summary:
//===========================================================================
class _XTP_EXT_CLASS CXTPMarkupThemePart
{
public:

	CXTPMarkupThemePart();
	virtual ~CXTPMarkupThemePart();

	BOOL LoadPart(HZIP hZip, LPCTSTR pszFileName, CXTPMarkupContext *pMarkupContext);

	void RenderMarkup(CDC *pDC, CRect rc);

	CSize Measure(int cx, int cy);

protected:
	CXTPMarkupUIElement *m_pMarkupUIElement;
};

#endif // !defined(__XTPMARKUPTHEME_H__)
