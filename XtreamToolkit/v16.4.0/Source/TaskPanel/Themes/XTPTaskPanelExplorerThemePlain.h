// XTPTaskPanelExplorerThemePlain.cpp : implementation of the CXTPTaskPanelExplorerThemePlain class.
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
#ifndef XTPTaskPanelExplorerThemePlain_h__
#define XTPTaskPanelExplorerThemePlain_h__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//===========================================================================
// Summary:
//     The CXTPTaskPanelExplorerThemePlain class is used to enable a classic Explorer style theme.
// See Also:
//     CXTPTaskPanel::SetTheme
//===========================================================================
class _XTP_EXT_CLASS CXTPTaskPanelExplorerThemePlain : public CXTPTaskPanelPaintManagerPlain
{
public:

	//-------------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPTaskPanelExplorerThemePlain object.
	//-------------------------------------------------------------------------
	CXTPTaskPanelExplorerThemePlain();

	//-------------------------------------------------------------------------
	// Summary:
	//     This method is called to refresh the visual metrics of task panel.
	//-------------------------------------------------------------------------
	void RefreshMetrics();

	BOOL m_bExplorerTheme;  // TRUE to use Explorer theme under WinXP and to use Office 2000 under older OSes.

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw gripper of the caption.
	// Parameters:
	//     pDC    - Pointer to a valid device context
	//     pGroup - Points to a CXTPTaskPanelGroup object
	//     rc     - Rectangle of group to be drawn.
	// Returns:
	//     Rectangle of caption after draw.
	// Remarks:
	//     The caption gripper is the button that allows the group to be
	//     expanded and collapsed.
	//
	//     If the current operating system is Windows Xp and the current theme
	//     is xtpSystemThemeBlue, xtpSystemThemeOlive, or xtpSystemThemeSilver
	//     then the gripper bitmap is loaded from the operating system.  The WinXp
	//     gripper is created with CXTPTaskPanelPaintManager::CreateGripperBitmaps. For
	//     all other OS and themes, CXTPTaskPanelPaintManager::CreateGripperIcon
	//     is used to create an emulated version of the Windows Xp gripper.
	//
	//     If m_bExplorerTheme is FALSE, then CXTPTaskPanelPaintManager::DrawCaptionGripper
	//     is used to draw the gripper, so no bitmap is used.
	//
	//     If m_bExplorerTheme is TRUE, then CXTPTaskPanelPaintManager::DrawCaptionGripperBitmap
	//     is used to draw the gripper, this load a bitmap as mentioned before.
	//
	// See Also:
	//     CXTPTaskPanelPaintManager::DrawCaptionGripperBitmap,
	//     CXTPTaskPanelPaintManager::DrawCaptionGripperSigns
	//-----------------------------------------------------------------------
	CRect DrawCaptionGripper(CDC* pDC, CXTPTaskPanelGroup* pGroup, CRect rc);
};

#endif // XTPTaskPanelExplorerThemePlain_h__