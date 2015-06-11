// XTPTaskPanelShortcutBarOffice2003Theme.h : implementation of the CXTPTaskPanelShortcutBarOffice2003Theme class.
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
#ifndef XTPTaskPanelShortcutBarOffice2003Theme_h__
#define XTPTaskPanelShortcutBarOffice2003Theme_h__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//===========================================================================
// Summary:
//     CXTPTaskPanelShortcutBarOffice2003Theme is a CXTPTaskPanelListViewOffice2003Theme
//     derived class used to enable an Office 2003 ShortcutBar style theme.
// See Also:
//     CXTPTaskPanel::SetTheme
//===========================================================================
class _XTP_EXT_CLASS CXTPTaskPanelShortcutBarOffice2003Theme : public CXTPTaskPanelListViewOffice2003Theme
{
public:

	//-------------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPTaskPanelShortcutBarOffice2003Theme object.
	//-------------------------------------------------------------------------
	CXTPTaskPanelShortcutBarOffice2003Theme();

	//-------------------------------------------------------------------------
	// Summary:
	//     This method is called to refresh the visual metrics of task panel.
	//-------------------------------------------------------------------------
	void RefreshMetrics();

protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     This member is called to draw the background of the group caption.
	// Parameters:
	//     pDC    - Pointer to a valid device context.
	//     pGroup - Fill group caption of this group.
	//     rc     - Bounding rectangle of the group caption.
	// Returns:
	//     Bounding rectangle of text within group caption.
	// Remarks:
	//     This member will draw the normal, hot, and pressed background for
	//     the group caption.  It will then fill the group caption with the
	//     Office 2003 color gradient.
	//
	//     This task panel theme looks similar to the ShortcutBar Office 2003
	//     theme.
	//-----------------------------------------------------------------------
	CRect DrawGroupCaptionBackground(CDC* pDC, CXTPTaskPanelGroup* pGroup, CRect rc);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the face of the group.
	// Parameters:
	//     pDC - Pointer to a valid device context
	//     pGroup - Points to a CXTPTaskPanelGroup object
	//     rc - Rectangle of group to be draw.
	//-----------------------------------------------------------------------
	void DrawGroupClientFace(CDC* pDC, CXTPTaskPanelGroup* pGroup, CRect rc);

	//-----------------------------------------------------------------------
	// Summary:
	//     Fills group rectangle
	// Parameters:
	//     pDC - Pointer to a valid device context
	//     pGroup - Points to a CXTPTaskPanelGroup object
	//     rc - Rectangle of group to be draw.
	// See Also: DrawGroupClientFace
	//-----------------------------------------------------------------------
	void FillGroupClientFace(CDC* pDC, CXTPTaskPanelGroup* pGroup, CRect rc);

	//-----------------------------------------------------------------------
	//     This method is called to fill background of task panel control.
	// Parameters:
	//     pDC - Pointer to a valid device context
	//     pTaskPanel - Points to a CXTPTaskPanel object
	//-----------------------------------------------------------------------
	void FillTaskPanel(CDC* pDC, CXTPTaskPanel* pTaskPanel);

};

#endif // XTPTaskPanelShortcutBarOffice2003Theme_h__