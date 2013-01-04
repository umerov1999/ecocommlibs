// XTPFlowGraphPages.h: interface for the CXTPFlowGraphPages class.
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
#if !defined(__XTPFLOWGRAPHPAGES_H__)
#define __XTPFLOWGRAPHPAGES_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXTPFlowGraphControl;
class CXTPFlowGraphPage;



// ----------------------------------------------------------------------
//
// Summary:
//     Represents a collection of pages in the flow graph.
//
// Remarks:
//     A page is what contains all the elements in the flow graph. The
//     flow graph can contain many pages, each of which can contain
//     nodes, node groups and connections. A page is the first thing that
//     you must add to the flow graph, then you can add elements to the
//     pages.
// ----------------------------------------------------------------------
class _XTP_EXT_CLASS CXTPFlowGraphPages : public CXTPCmdTarget
{
protected:
	// -------------------------------------------
	// Summary:
	//     Constructs a CXTPFlowGraphPages object.
	// -------------------------------------------
	CXTPFlowGraphPages(CXTPFlowGraphControl* pControl);
	// -------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPFlowGraphPages object, handles cleanup and
	//     deallocation.
	// -------------------------------------------------------------
	virtual ~CXTPFlowGraphPages();

public:
	// ----------------------------------------------------
	//
	// Summary:
	//     Adds a page to the flow graph.
	//
	//
	// Returns:
	//     Reference to the newly added FlowGraphPage.
	//
	//
	// ----------------------------------------------------
	CXTPFlowGraphPage* AddPage(CXTPFlowGraphPage* pPage);

	// -----------------------------------------------
	//
	// Summary:
	//     Removes all pages from the flow graph.
	//
	//
	// -----------------------------------------------
	void RemoveAll();

	// -------------------------------------------------
	//
	// Summary:
	//     Total number of pages in the flow graph.
	//
	// Returns:
	//     Total number of pages in the flow graph.
	// -------------------------------------------------
	int GetCount() const;
	// ----------------------------------------------------------------------
	//
	// Summary:
	//     Gets a page at the specified index from the FlowGraph's collection
	//     \of pages.
	// Parameters:
	//     nIndex :  Index of the specified page.
	//
	// Returns:
	//     \Returns a reference to the specified FlowGraphPage.
	// ----------------------------------------------------------------------
	CXTPFlowGraphPage* GetAt(int nIndex) const;

	// ---------------------------------------------
	//
	// Summary:
	//     Removes a page from the FlowGraph.
	// Parameters:
	//     pPage :  Reference to the page to remove.
	// ---------------------------------------------
	void Remove(CXTPFlowGraphPage* pPage);
	// ---------------------------------------------------
	//
	// Summary:
	//     Removes the specified page from the flow graph.
	// Parameters:
	//     nIndex :  Index of the page to remove.
	// ---------------------------------------------------
	void RemoveAt(int nIndex);

	// --------------------------------------------------------------------
	//
	// Summary:
	//     Finds and returns the specified page.
	//
	// Parameters:
	//     nId :  ID of the page to find and return.
	//
	// Returns:
	//     If found, reference to the page with the specified ID, otherwise
	//     nothing\\null will be returned.
	// --------------------------------------------------------------------
	CXTPFlowGraphPage* FindPage(int nId) const;

public:
	// ------------------------------------------------------------------------
	// Summary:
	//     Call this member function to Store/Load a collection of pages
	//     using the specified data object.
	// Parameters:
	//     pPX :  Source or destination CXTPPropExchange data object reference.
	// Remarks:
	//     This member function is used to store or load a collection pages
	//     data to or from a storage.
	// ------------------------------------------------------------------------
	void DoPropExchange(CXTPPropExchange* pPX);

protected:
	CArray<CXTPFlowGraphPage*, CXTPFlowGraphPage*> m_arrPages; // Collection of pages in the flow graph control.
	CXTPFlowGraphControl* m_pControl; // Pointer to the parent flow graph control.

	friend class CXTPFlowGraphControl;

};

AFX_INLINE int CXTPFlowGraphPages::GetCount() const {
	return (int)m_arrPages.GetSize();
}
AFX_INLINE CXTPFlowGraphPage* CXTPFlowGraphPages::GetAt(int nIndex) const {
	return nIndex >= 0 && nIndex < m_arrPages.GetSize() ? m_arrPages[nIndex] : NULL;
}

#endif //#if !defined(__XTPFLOWGRAPHPAGES_H__)
