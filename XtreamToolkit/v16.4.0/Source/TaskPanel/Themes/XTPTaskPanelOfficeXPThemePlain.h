// XTPTaskPanelOfficeXPThemePlain.h : implementation of the CXTPTaskPanelOfficeXPThemePlain class.
//
// This file is a part of the XTREME TASKPANEL MFC class library.
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
#ifndef XTPTaskPanelOfficeXPThemePlain_h__
#define XTPTaskPanelOfficeXPThemePlain_h__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//===========================================================================
// Summary:
//     The CXTPTaskPanelOfficeXPThemePlain class is used to enable a classic Office XP style theme.
// See Also:
//     CXTPTaskPanel::SetTheme
//===========================================================================
class _XTP_EXT_CLASS CXTPTaskPanelOfficeXPThemePlain : public CXTPTaskPanelPaintManagerPlain
{
public:

	//-------------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPTaskPanelOfficeXPThemePlain object.
	//-------------------------------------------------------------------------
	CXTPTaskPanelOfficeXPThemePlain();

	//-------------------------------------------------------------------------
	// Summary:
	//     This method is called to refresh the visual metrics of task panel.
	//-------------------------------------------------------------------------
	void RefreshMetrics();

};

#endif // XTPTaskPanelOfficeXPThemePlain_h__