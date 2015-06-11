// XTPReportBehaviorSchemeOutlook.cpp
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

#include "StdAfx.h"

#include <Common/XTPCustomHeap.h>

#include <ReportControl/XTPReportDefines.h>
#include <ReportControl/Behavior/XTPReportBehavior.h>

void CXTPReportBehavior::SetSchemeOutlook()
{
	//////////////////////////////////////////////////////////////////////////
	// Group row
	//////////////////////////////////////////////////////////////////////////

	// Left mouse button
	GroupRow->MouseDown->Left->None->bFocusRow             = TRUE;
	GroupRow->MouseDown->Left->None->bFocusRowTemporarily  = FALSE;
	GroupRow->MouseDown->Left->None->bSelectRow            = TRUE;
	GroupRow->MouseDown->Left->None->bSelectRowTemporarily = FALSE;
	GroupRow->MouseDown->Left->bFocusColumn          = TRUE;
	GroupRow->MouseDown->Left->bEnsureVisible        = FALSE;
	GroupRow->MouseDown->Left->bClick                = FALSE; // Should be TRUE
	GroupRow->MouseDown->Left->bContextMenu          = FALSE;
	GroupRow->MouseDown->Left->bCheckSelectedRows    = FALSE;
	GroupRow->MouseDown->Left->bDragBegin            = TRUE;
	GroupRow->MouseDown->Left->bDragEnd              = FALSE;
	GroupRow->MouseDown->Left->bDragSelectGroup      = TRUE;

	GroupRow->MouseUp->Left->None->bFocusRow               = FALSE; // Set on mouse down
	GroupRow->MouseUp->Left->None->bFocusRowTemporarily    = FALSE;
	GroupRow->MouseUp->Left->None->bSelectRow              = FALSE; // Set on mouse down
	GroupRow->MouseUp->Left->None->bSelectRowTemporarily   = FALSE;
	GroupRow->MouseUp->Left->bFocusColumn            = FALSE; // Set on mouse down
	GroupRow->MouseUp->Left->bEnsureVisible          = FALSE;
	GroupRow->MouseUp->Left->bClick                  = TRUE;  // Should be FALSE, see Left->bClick
	GroupRow->MouseUp->Left->bContextMenu            = FALSE;
	GroupRow->MouseUp->Left->bCheckSelectedRows      = FALSE;
	GroupRow->MouseUp->Left->bDragBegin              = FALSE; // Begin on mouse down
	GroupRow->MouseUp->Left->bDragEnd                = TRUE;
	GroupRow->MouseUp->Left->bDragSelectGroup        = FALSE;

	// Right mouse button
	GroupRow->MouseDown->Right->None->bFocusRow             = TRUE;
	GroupRow->MouseDown->Right->None->bFocusRowTemporarily  = FALSE;
	GroupRow->MouseDown->Right->None->bSelectRow            = TRUE;
	GroupRow->MouseDown->Right->None->bSelectRowTemporarily = FALSE;
	GroupRow->MouseDown->Right->bFocusColumn          = TRUE;
	GroupRow->MouseDown->Right->bEnsureVisible        = FALSE;
	GroupRow->MouseDown->Right->bClick                = FALSE;
	GroupRow->MouseDown->Right->bContextMenu          = FALSE;
	GroupRow->MouseDown->Right->bCheckSelectedRows    = FALSE;
	GroupRow->MouseDown->Right->bDragBegin            = TRUE;
	GroupRow->MouseDown->Right->bDragEnd              = FALSE;
	GroupRow->MouseDown->Right->bDragSelectGroup      = TRUE;

	GroupRow->MouseUp->Right->None->bFocusRow               = FALSE;
	GroupRow->MouseUp->Right->None->bFocusRowTemporarily    = FALSE;
	GroupRow->MouseUp->Right->None->bSelectRow              = FALSE;
	GroupRow->MouseUp->Right->None->bSelectRowTemporarily   = FALSE;
	GroupRow->MouseUp->Right->bFocusColumn            = FALSE;
	GroupRow->MouseUp->Right->bEnsureVisible          = FALSE;
	GroupRow->MouseUp->Right->bClick                  = FALSE;
	GroupRow->MouseUp->Right->bContextMenu            = TRUE;
	GroupRow->MouseUp->Right->bCheckSelectedRows      = FALSE;
	GroupRow->MouseUp->Right->bDragBegin              = FALSE;
	GroupRow->MouseUp->Right->bDragEnd                = TRUE;
	GroupRow->MouseUp->Right->bDragSelectGroup        = FALSE;

	//////////////////////////////////////////////////////////////////////////
	// Row
	//////////////////////////////////////////////////////////////////////////

	// Left mouse button
	Row->MouseDown->Left->None->bFocusRow               = TRUE;
	Row->MouseDown->Left->None->bFocusRowTemporarily    = FALSE;
	Row->MouseDown->Left->None->bSelectRow              = TRUE;
	Row->MouseDown->Left->None->bSelectRowTemporarily   = FALSE;
	Row->MouseDown->Left->None->bKeepFocus              = TRUE;  // Drag multi selection
	Row->MouseDown->Left->None->bKeepSelection          = TRUE;  // Drag multi selection
	Row->MouseDown->Left->Control->bKeepSelection = TRUE;  // Copy multi selection
	Row->MouseDown->Left->Control->bKeepFocus     = TRUE;  // Copy multi selection
	Row->MouseDown->Left->bFocusColumn            = TRUE;
	Row->MouseDown->Left->bEnsureVisible          = FALSE;
	Row->MouseDown->Left->bClick                  = FALSE; // Should be TRUE
	Row->MouseDown->Left->bContextMenu            = FALSE;
	Row->MouseDown->Left->bCheckSelectedRows      = FALSE;
	Row->MouseDown->Left->bDragBegin              = TRUE;
	Row->MouseDown->Left->bDragEnd                = FALSE;

	Row->MouseUp->Left->None->bFocusRow               = FALSE; // Set on mouse down
	Row->MouseUp->Left->None->bFocusRowTemporarily    = FALSE;
	Row->MouseUp->Left->None->bSelectRow              = FALSE;
	Row->MouseUp->Left->None->bSelectRowTemporarily   = FALSE;
	Row->MouseUp->Left->Control->bKeepSelection = TRUE;  // Ctrl/Shift
	Row->MouseUp->Left->Control->bKeepFocus     = TRUE;  // Ctrl/Shift
	Row->MouseUp->Left->bFocusColumn            = FALSE; // Set on mouse down
	Row->MouseUp->Left->bEnsureVisible          = FALSE;
	Row->MouseUp->Left->bClick                  = TRUE;  // Should be FALSE, see Left->bClick
	Row->MouseUp->Left->bContextMenu            = FALSE;
	Row->MouseUp->Left->bCheckSelectedRows      = FALSE;
	Row->MouseUp->Left->bDragBegin              = FALSE; // Begin on mouse down
	Row->MouseUp->Left->bDragEnd                = TRUE;

	// Right mouse button
	Row->MouseDown->Right->None->bFocusRowTemporarily  = FALSE;
	Row->MouseDown->Right->None->bSelectRow            = FALSE;
	Row->MouseDown->Right->None->bSelectRowTemporarily = FALSE;
	Row->MouseDown->Right->bFocusColumn          = FALSE;
	Row->MouseDown->Right->bEnsureVisible        = FALSE;
	Row->MouseDown->Right->bClick                = FALSE;
	Row->MouseDown->Right->bContextMenu          = FALSE;
	Row->MouseDown->Right->bCheckSelectedRows    = FALSE;
	Row->MouseDown->Right->bDragBegin            = TRUE;
	Row->MouseDown->Right->bDragEnd              = FALSE;

	Row->MouseUp->Right->None->bFocusRow               = FALSE;
	Row->MouseUp->Right->None->bFocusRowTemporarily    = TRUE;
	Row->MouseUp->Right->None->bSelectRow              = FALSE;
	Row->MouseUp->Right->None->bSelectRowTemporarily   = FALSE;
	Row->MouseUp->Right->bFocusColumn            = FALSE;
	Row->MouseUp->Right->bEnsureVisible          = FALSE;
	Row->MouseUp->Right->bClick                  = FALSE;
	Row->MouseUp->Right->bContextMenu            = TRUE;
	Row->MouseUp->Right->bCheckSelectedRows      = FALSE;
	Row->MouseUp->Right->bDragBegin              = FALSE;
	Row->MouseUp->Right->bDragEnd                = TRUE;
}
