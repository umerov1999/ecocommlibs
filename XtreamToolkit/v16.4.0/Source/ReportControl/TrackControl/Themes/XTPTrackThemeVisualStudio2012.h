// XTPTrackThemeVisualStudio2012.h: interface for the CXTPTrackThemeOffice2013 class.
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
#if !defined(__XTPTRACKTHEMEVISUALSTUDIO2013_H__)
#define __XTPTRACKTHEMEVISUALSTUDIO2013_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//===========================================================================
// Summary:
//     CXTPTrackThemeVisualStudio2012Light is a CXTPTrackThemeOffice2013 derived
//     class.  This represents an Office 2013 style theme.
//===========================================================================
class _XTP_EXT_CLASS CXTPTrackThemeVisualStudio2012Light : public CXTPTrackThemeOffice2013
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Default paint manager constructor, handles properties initialization.
	//-----------------------------------------------------------------------
	CXTPTrackThemeVisualStudio2012Light();

	//-----------------------------------------------------------------------
	// Summary:
	//     Loads default images used by Track control.
	//-----------------------------------------------------------------------
	virtual void LoadGlyphs();

	//-------------------------------------------------------------------------
	// Summary:
	//     This method is called to refresh the visual metrics of the
	//     Track control.
	//-------------------------------------------------------------------------
	virtual void RefreshMetrics();
};

//===========================================================================
// Summary:
//     CXTPTrackThemeVisualStudio2012Dark is a CXTPTrackThemeOffice2013 derived
//     class.  This represents an Office 2013 style theme.
//===========================================================================
class _XTP_EXT_CLASS CXTPTrackThemeVisualStudio2012Dark : public CXTPTrackThemeOffice2013
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Default paint manager constructor, handles properties initialization.
	//-----------------------------------------------------------------------
	CXTPTrackThemeVisualStudio2012Dark();

	//-----------------------------------------------------------------------
	// Summary:
	//     Loads default images used by Track control.
	//-----------------------------------------------------------------------
	virtual void LoadGlyphs();

	//-------------------------------------------------------------------------
	// Summary:
	//     This method is called to refresh the visual metrics of the
	//     Track control.
	//-------------------------------------------------------------------------
	virtual void RefreshMetrics();
};

//===========================================================================
// Summary:
//     CXTPTrackThemeVisualStudio2012Blue is a CXTPTrackThemeOffice2013 derived
//     class.  This represents an Office 2013 style theme.
//===========================================================================
class _XTP_EXT_CLASS CXTPTrackThemeVisualStudio2012Blue : public CXTPTrackThemeOffice2013
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Default paint manager constructor, handles properties initialization.
	//-----------------------------------------------------------------------
	CXTPTrackThemeVisualStudio2012Blue();

	//-----------------------------------------------------------------------
	// Summary:
	//     Loads default images used by Track control.
	//-----------------------------------------------------------------------
	virtual void LoadGlyphs();

	//-------------------------------------------------------------------------
	// Summary:
	//     This method is called to refresh the visual metrics of the
	//     Track control.
	//-------------------------------------------------------------------------
	virtual void RefreshMetrics();
};

#endif //#if !defined(__XTPTRACKTHEMEVISUALSTUDIO2013_H__)
