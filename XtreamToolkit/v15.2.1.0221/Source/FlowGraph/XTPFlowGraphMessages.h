// XTPFlowGraphMessages.h
//
// This file is a part of the XTREME TOOLKIT PRO MFC class library.
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

//{{AFX_CODEJOCK_PRIVATE
#if !defined(__XTPFLOWGRAPHMESSAGES_H__)
#define __XTPFLOWGRAPHMESSAGES_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXTPFlowGraphNode;

// ---------------------------------
// Summary:
//     Information about the label whose editing has ended.
// ---------------------------------
struct XTP_NM_FLOWGRAPH_ENDLABELEDIT
{
	NMHDR hdr;                  // Standard structure, containing information about a notification message.

	CXTPFlowGraphElement* pItem;   // Item that caption is will be changed
	CString strNewString;       // New Caption of the item
};

// ---------------------------------------------------------------------
// Summary:
//     This notification is sent when editing has ended for a label. The
//     information about the edited item is sent in a
//     XTP_NM_FLOWGRAPH_ENDLABELEDIT structure.
// ---------------------------------------------------------------------
const UINT XTP_FGN_ENDLABELEDIT = (NM_FIRST - 61);


// -----------------------------------------------------------
// Summary:
//     This notification is sent when a page is set to active.
// -----------------------------------------------------------
const UINT XTP_FGN_ACTIVEPAGECHANGED = (NM_FIRST - 62);


// -----------------------------
// Summary:
//     Information about the connection that has changed.
// -----------------------------
struct XTP_NM_FLOWGRAPH_CONNECTIONCHANGED
{
	NMHDR hdr;                  // Standard structure, containing information about a notification message.

	CXTPFlowGraphConnection* pConnection;  // Connection
	int nAction; // 0 - Added, 1 - Removed;
};


// ----------------------------------------------------------------------
// Summary:
//     This notification is sent when a connection is added or removed. A
//     XTP_FGN_CONNECTIONCHANGED structure is sent containing information
//     about the connection that changed.
// ----------------------------------------------------------------------
const UINT XTP_FGN_CONNECTIONCHANGED = (NM_FIRST - 63);

// ------------------------------------------------------------------
// Summary:
//     This notification is sent when the selection has changed. This
//     happens when the selection is cleared, set, added or removed.
// ------------------------------------------------------------------
const UINT XTP_FGN_SELECTIONCHANGED = (NM_FIRST - 64);

// -------------------------------------------------------------------
// Summary:
//     This notification is sent when the page zoom level has changed.
// -------------------------------------------------------------------
const UINT XTP_FGN_PAGEZOOMLEVELCHANGED = (NM_FIRST - 65);


// -----------------------
// Summary:
//     Information about the node that has changed.
// -----------------------
struct XTP_NM_FLOWGRAPH_NODECHANGED
{
	NMHDR hdr;                  // Standard structure, containing information about a notification message.

	CXTPFlowGraphNode* pNode;  // Node
};

// ----------------------------------------------------------------------
// Summary:
//     This notification is sent when the location of a node has changed.
//     The XTP_NM_FLOWGRAPH_NODECHANGED structure is sent with
//     information about the node that changed.
// ----------------------------------------------------------------------
const UINT XTP_FGN_NODELOCATIONCHANGED = (NM_FIRST - 66);

// ----------------------------------------------------------------------
// Summary:
//     This notification is sent when the offset of the page has changed.
//     This can occur when the mouse wheel is scrolled, when the screen
//     is dragged, during page animation transitions, zooming and
//     arranging of pages.
// ----------------------------------------------------------------------
const UINT XTP_FGN_PAGESCROLLOFFSETCHANGED = (NM_FIRST - 67);


// -----------------------------------
// Information about the connection that is currently changing.
// -----------------------------------
struct XTP_NM_FLOWGRAPH_CONNECTIONCHANGING
{
	NMHDR hdr;                  // Standard structure, containing information about a notification message.

	CXTPFlowGraphConnection* pConnection;  // Connection
	CXTPFlowGraphConnectionPoint* pInputConnectionPoint;  // Connection Point
};
// --------------------------------------------------------------------
// Summary:
//     This notification is sent when a connection is changing. A
//     XTP_NM_FLOWGRAPH_CONNECTIONCHANGING structure is sent containing
//     information about the connection changing. This notification is
//     sent during a call to
//     CXTPFlowGraphControl::StartDragConnectionPoint.
// --------------------------------------------------------------------
const UINT XTP_FGN_CONNECTIONCHANGING = (NM_FIRST - 68);


#endif //#if !defined(__XTPFLOWGRAPHMESSAGES_H__)
