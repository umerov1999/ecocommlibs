// XTPCommandBarsOffice2007SystemTheme.h : interface for the CXTPCommandBarsOffice2007SystemTheme class.
//
// This file is a part of the XTREME COMMANDBARS MFC class library.
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
#if !defined(__XTPCOMMANDBARSOFFICE2007SYSTEMTHEME_H__)
#define __XTPCOMMANDBARSOFFICE2007SYSTEMTHEME_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


class _XTP_EXT_CLASS CXTPCommandBarsOffice2007SystemTheme : public CXTPCommandBarsOfficeXPTheme
{
	DECLARE_DYNAMIC(CXTPCommandBarsOffice2007SystemTheme)
public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPCommandBarsOffice2007SystemTheme object.
	//-----------------------------------------------------------------------
	CXTPCommandBarsOffice2007SystemTheme();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPCommandBarsOffice2007SystemTheme object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTPCommandBarsOffice2007SystemTheme();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns paint manager used to draw frames.
	// Returns:
	//     Returns paint manager used to draw frames.
	//-----------------------------------------------------------------------
	virtual CXTPFramePaintManager* GetFramePaintManager();

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves ribbon paint manager.
	// Returns:
	//     Pointer to CXTPRibbonPaintManager object.
	//-----------------------------------------------------------------------
	virtual CXTPRibbonPaintManager* GetRibbonPaintManager();

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to refresh the visual metrics of the manager.
	//-----------------------------------------------------------------------
	virtual void RefreshMetrics();

protected:

	CXTPMarkupContext *m_pMarkupContext;
};

#endif // !defined(__XTPCOMMANDBARSOFFICE2007SYSTEMTHEME_H__)
