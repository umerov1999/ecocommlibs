// XTPReportThemeVisualStudio2012.h: interface for the CXTPReportThemeOffice2013 class.
//
// This file is a part of the XTREME REPORTCONTROL MFC class library.
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
#if !defined(__XTPREPORTTHEMEVISUALSTUDIO2013_H__)
#define __XTPREPORTTHEMEVISUALSTUDIO2013_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//===========================================================================
// Summary:
//     CXTPReportThemeVisualStudio2012Light is a CXTPReportThemeOffice2013 derived
//     class.  This represents an Office 2013 style theme.
//===========================================================================
class _XTP_EXT_CLASS CXTPReportThemeVisualStudio2012Light : public CXTPReportThemeOffice2013
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Default paint manager constructor, handles properties initialization.
	//-----------------------------------------------------------------------
	CXTPReportThemeVisualStudio2012Light();

	//-----------------------------------------------------------------------
	// Summary:
	//     Loads default images used by report control.
	//-----------------------------------------------------------------------
	virtual void LoadGlyphs();

	//-------------------------------------------------------------------------
	// Summary:
	//     This method is called to refresh the visual metrics of the
	//     report control.
	//-------------------------------------------------------------------------
	virtual void RefreshMetrics();
};

//===========================================================================
// Summary:
//     CXTPReportThemeVisualStudio2012Dark is a CXTPReportThemeOffice2013 derived
//     class.  This represents an Office 2013 style theme.
//===========================================================================
class _XTP_EXT_CLASS CXTPReportThemeVisualStudio2012Dark : public CXTPReportThemeOffice2013
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Default paint manager constructor, handles properties initialization.
	//-----------------------------------------------------------------------
	CXTPReportThemeVisualStudio2012Dark();

	//-----------------------------------------------------------------------
	// Summary:
	//     Loads default images used by report control.
	//-----------------------------------------------------------------------
	virtual void LoadGlyphs();

	//-------------------------------------------------------------------------
	// Summary:
	//     This method is called to refresh the visual metrics of the
	//     report control.
	//-------------------------------------------------------------------------
	virtual void RefreshMetrics();
};

//===========================================================================
// Summary:
//     CXTPReportThemeVisualStudio2012Blue is a CXTPReportThemeOffice2013 derived
//     class.  This represents an Office 2013 style theme.
//===========================================================================
class _XTP_EXT_CLASS CXTPReportThemeVisualStudio2012Blue : public CXTPReportThemeOffice2013
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Default paint manager constructor, handles properties initialization.
	//-----------------------------------------------------------------------
	CXTPReportThemeVisualStudio2012Blue();

	//-----------------------------------------------------------------------
	// Summary:
	//     Loads default images used by report control.
	//-----------------------------------------------------------------------
	virtual void LoadGlyphs();

	//-------------------------------------------------------------------------
	// Summary:
	//     This method is called to refresh the visual metrics of the
	//     report control.
	//-------------------------------------------------------------------------
	virtual void RefreshMetrics();
};

#endif //#if !defined(__XTPREPORTTHEMEVISUALSTUDIO2013_H__)
