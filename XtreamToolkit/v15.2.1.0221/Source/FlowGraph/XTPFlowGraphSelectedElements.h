// XTPFlowGraphSelectedElements.h: interface for the CXTPFlowGraphSelectedElements class.
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
#if !defined(__XTPFLOWGRAPHSELECTEDELEMENTS_H__)
#define __XTPFLOWGRAPHSELECTEDELEMENTS_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXTPFlowGraphControl;
class CXTPFlowGraphPage;

// ----------------------------------------------------------------------
//
// Summary:
//     Collection of selected elements in the Flow Graph.
//
// Remarks:
//     Selected elements will include Nodes, Connections and Connection
//     Points. This collection can be used to add or remove elements
//     to\\from the selection. Elements can be selected with the mouse or
//     using methods in code.
// ----------------------------------------------------------------------
class _XTP_EXT_CLASS CXTPFlowGraphSelectedElements : public CXTPCmdTarget
{
public:
	// ------------------------------------------------------
	// Summary:
	//     Constructs a CXTPFlowGraphSelectedElements object.
	// ------------------------------------------------------
	CXTPFlowGraphSelectedElements(CXTPFlowGraphPage* pPage);
	// --------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPFlowGraphSelectedElements object, handles cleanup
	//     and deallocation.
	// --------------------------------------------------------------------
	virtual ~CXTPFlowGraphSelectedElements();

public:
	// ---------------------------------------------------------
	//
	// Summary:
	//     Removes all elements from the current selection.
	//
	// ---------------------------------------------------------
	void Clear();

	// ---------------------------------------------------------------------
	//
	// Summary:
	//     Adds an element to the current selection.
	//
	//
	// Parameters:
	//     pElement :  The element to add to the selection, can be any valid
	//                 flow graph element (connection, node).
	// ---------------------------------------------------------------------
	void SetSelection(CXTPFlowGraphElement* pElement);
	// ---------------------------------------------------------------------
	//
	// Summary:
	//     Adds an element to the current selection.
	//
	// Parameters:
	//     pElement :  The element to add to the selection, can be any valid
	//                 flow graph element (connection, node).
	// ---------------------------------------------------------------------
	void AddSelection(CXTPFlowGraphElement* pElement);

	// --------------------------------------------------------------------
	//
	// Summary:
	//     Removes an element from the current selection.
	//
	//
	// Parameters:
	//     pElement :  The element to remove from the selection, can be any
	//                 valid flow graph element (connection, node).
	// --------------------------------------------------------------------
	void Remove(CXTPFlowGraphElement* pElement);
	// ------------------------------------------------------------------------
	//
	// Summary:
	//     Removes an element from the current selection.
	//
	//
	// Parameters:
	//     nIndex :  Index of the element to remove from the current selection.
	// ------------------------------------------------------------------------
	void RemoveAt(int nIndex);

	// -----------------------------------------------------
	//
	// Summary:
	//     Number of elements in the current selection.
	//
	//
	// Returns:
	//     Number of elements in the current selection.
	//
	//
	// -----------------------------------------------------
	int GetCount() const;
	// -----------------------------------------------------------------------
	//
	// Summary:
	//     \Returns an element from the selection at the specified index.
	//
	//
	// Parameters:
	//     nIndex :  Index of a FlowGraphElement within the collection of
	//               elements.
	//
	// Remarks:
	//     Elements are added using the SetSelection method and removed using
	//     the RemoveAt method.
	//
	//
	// -----------------------------------------------------------------------
	CXTPFlowGraphElement* GetAt(int nIndex) const;

protected:

	CArray<CXTPFlowGraphElement*, CXTPFlowGraphElement*> m_arrSelectedElements;  // Collection of selected elements.

	CXTPFlowGraphPage* m_pPage; // Pointer to the page the selected elements are dispalyed on.
};

AFX_INLINE int CXTPFlowGraphSelectedElements::GetCount() const {
	return (int)m_arrSelectedElements.GetSize();
}
AFX_INLINE CXTPFlowGraphElement* CXTPFlowGraphSelectedElements::GetAt(int nIndex) const {
	return nIndex >= 0 && nIndex < m_arrSelectedElements.GetSize() ? m_arrSelectedElements[nIndex] : NULL;
}

#endif //#if !defined(__XTPFLOWGRAPHSELECTEDELEMENTS_H__)
