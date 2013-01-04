// XTPFlowGraphPaintMetallicTheme.h: interface for the CXTPFlowGraphPaintMetallicTheme class.
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
#if !defined(__XTPFLOWGRAPHMETALLICTHEME_H__)
#define __XTPFLOWGRAPHMETALLICTHEME_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


// --------------------------------------------------------------
// Summary:
//     Represents the metallic theme used to draw the flow graph.
// --------------------------------------------------------------
class _XTP_EXT_CLASS CXTPFlowGraphMetallicTheme : public CXTPFlowGraphPaintManager
{
public:
	// ---------------------------------------------------
	// Summary:
	//     Constructs a CXTPFlowGraphMetallicTheme object.
	// ---------------------------------------------------
	CXTPFlowGraphMetallicTheme();
	// ---------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPFlowGraphMetallicTheme object, handles cleanup and
	//     deallocation.
	// ---------------------------------------------------------------------
	virtual ~CXTPFlowGraphMetallicTheme();

public:
	// -----------------------------------------------------------------
	// Summary:
	//     This method is called to draw a single Node in the flow graph
	//     control.
	// Parameters:
	//     pDC :    Pointer to a valid device context.
	//     pNode :  Points to a CXTPFlowGraphNode object to draw.
	// -----------------------------------------------------------------
	virtual void DrawNode(CXTPFlowGraphDrawContext* pDC, CXTPFlowGraphNode* pNode);

public:
	// -------------------------------------------------------------------
	// Summary:
	//     This method is called to refresh the visual metrics of the flow
	//     graph.
	// -------------------------------------------------------------------
	void RefreshMetrics();
	// ------------------------------------------------------------------
	// Summary:
	//     Cleans up allocated resources used by the theme that have been
	//     allocated during draw routines.
	// ------------------------------------------------------------------
	void Cleanup();
;

};

#endif //#if !defined(__XTPFLOWGRAPHMETALLICTHEME_H__)
