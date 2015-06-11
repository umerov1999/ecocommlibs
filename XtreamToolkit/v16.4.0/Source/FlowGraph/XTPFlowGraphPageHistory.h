// XTPFlowGraphPageHistory.h: interface for the CXTPFlowGraphPageHistory class.
//
// This file is a part of the XTREME TOOLKIT PRO MFC class library.
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
#if !defined(__XTPFLOWGRAPHPAGEHISTORY_H__)
#define __XTPFLOWGRAPHPAGEHISTORY_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXTPFlowGraphControl;
class CXTPFlowGraphNodes;
class CXTPFlowGraphNodeGroups;
class CXTPFlowGraphConnections;
class CXTPFlowGraphDrawContext;
class CXTPPropExchange;
class CXTPFlowGraphSelectedElements;
class CXTPFlowGraphNode;

// ------------------------------------------------------------------
// Summary:
//     Used internally to store node history for history operations.
// ------------------------------------------------------------------
struct CXTPFlowGraphPageHistoryItem
{
	CXTPFlowGraphNode* pFrom; // Pointer to the previous node.
	CXTPFlowGraphNode* pTo; // Pointer to the destination node.
};

// --------------------------------------------------------------------
//
// Summary:
//     Object that contains page transition history for the flow graph.
// --------------------------------------------------------------------
class _XTP_EXT_CLASS CXTPFlowGraphPageHistory: public CXTPCmdTarget
{
public:
	// -------------------------------------------------
	// Summary:
	//     Constructs a CXTPFlowGraphPageHistory object.
	// -------------------------------------------------
	CXTPFlowGraphPageHistory(CXTPFlowGraphControl* pControl);

protected:
	// -------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPFlowGraphPageHistory object, handles cleanup and
	//     deallocation.
	// -------------------------------------------------------------------
	virtual ~CXTPFlowGraphPageHistory();


public:
	// ---------------------------------------------------------------------
	//
	// Summary:
	//     Moves to the next page in the flow graphs history (if available).
	// ---------------------------------------------------------------------
	void GoForward();
	// -------------------------------------------------------------
	//
	// Summary:
	//     Moves to the previous page in the flow graphs history (if
	//     available).
	// -------------------------------------------------------------
	void GoBackward();

	// -----------------------------------------------------------
	//
	// Summary:
	//     Tests to see if a GoForward operation can be performed.
	// Returns:
	//     True if a GoForward operation can be performed.
	// -----------------------------------------------------------
	BOOL CanGoForward() const;
	// ---------------------------------------------------------------------
	//
	// Summary:
	//     Tests to see if a GoBack operation can be performed.
	// Remarks:
	//     A GoForward or page transition operation must be performed before
	//     a GoBack operation can be performed.
	// Returns:
	//     True if a GoBack operation can be performed.
	// ---------------------------------------------------------------------
	BOOL CanGoBackward() const;

	// -----------------------------------------
	//
	// Summary:
	//     Clears the history of the flow graph.
	// -----------------------------------------
	void Clear();

	// ---------------------------------------------------------------------------
	//
	// Summary:
	//     This method will transition from a node on one page to a node on
	//     another page.
	// Parameters:
	//     pNodeFrom :  Reference to a node on the current page to transition
	//                  from.
	//     pNodeTo :    Reference to a node on a different page to transition
	//                  to.
	//
	// Remarks:
	//     AnimatePageTransition will transition from a node on the current
	//     page to a node on a different page. This will cause the active
	//     page to change to the page that contains the transitioned to node.
	//
	//
	// ---------------------------------------------------------------------------
	void AnimatePageTransition(CXTPFlowGraphNode* pNodeFrom, CXTPFlowGraphNode* pNodeTo);


protected:
	CXTPFlowGraphControl* m_pControl; // Pointer to the parent flow graph control.

	CArray<CXTPFlowGraphPageHistoryItem, CXTPFlowGraphPageHistoryItem&> m_arrPageUndo; // Contains all the undo actions from the history.
	CArray<CXTPFlowGraphPageHistoryItem, CXTPFlowGraphPageHistoryItem&> m_arrPageRedo; // Contains all the redo actions from the history.


#ifdef _XTP_ACTIVEX
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPFlowGraphPageHistory);

	void OleAnimatePageTransition(LPDISPATCH lpDispFrom, LPDISPATCH lpDispTo);

//}}AFX_CODEJOCK_PRIVATE
#endif
};


#endif //#if !defined(__XTPFLOWGRAPHPAGEHISTORY_H__)
