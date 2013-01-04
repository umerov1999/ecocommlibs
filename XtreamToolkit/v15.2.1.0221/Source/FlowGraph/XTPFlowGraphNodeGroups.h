// XTPFlowGraphNodeGroups.h: interface for the CXTPFlowGraphNodeGroups class.
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
#if !defined(__XTPFLOWGRAPHNODEGROUPS_H__)
#define __XTPFLOWGRAPHNODEGROUPS_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXTPFlowGraphPage;
class CXTPFlowGraphNode;

// --------------------------------------------------------------------
//
// Summary:
//     Represents a collection of groups in a page.
//
// Remarks:
//     A group is just a "box" that is used to visually group nodes
//     together. By default the group will only be shown when a node in
//     the group is selected, but you can choose to always show the
//     groups with ShowNodeGroupsAlways.
//
//
// --------------------------------------------------------------------
class _XTP_EXT_CLASS CXTPFlowGraphNodeGroups : public CXTPCmdTarget
{
protected:
	// ------------------------------------------------
	// Summary:
	//     Constructs a CXTPFlowGraphNodeGroups object.
	// ------------------------------------------------
	CXTPFlowGraphNodeGroups(CXTPFlowGraphPage* pPage);
	// ------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPFlowGraphNodeGroups object, handles cleanup and
	//     deallocation.
	// ------------------------------------------------------------------
	virtual ~CXTPFlowGraphNodeGroups();

public:
	// -------------------------------------------
	//
	// Summary:
	//     Removes all groups from the collection.
	// -------------------------------------------
	void RemoveAll();
	// ----------------------------------------------------
	//
	// Summary:
	//     Adds a group to the page's collection of groups.
	// Returns:
	//     Reference to the group added.
	// ----------------------------------------------------
	CXTPFlowGraphNodeGroup* AddGroup();

	// ---------------------------------------------
	//
	// Summary:
	//     Total number of groups in the collection.
	// Returns:
	//     Total number of groups in the collection.
	// ---------------------------------------------
	int GetCount() const;
	// -------------------------------------------------------
	// Summary:
	//     Gets the groups at the specified index.
	// Parameters:
	//     nIndex :  Index of a group in the collection.
	// Returns:
	//     Reference to the specified group in the collection.
	// -------------------------------------------------------
	CXTPFlowGraphNodeGroup* GetAt(int nIndex) const;

	// -------------------------------------------------
	//
	// Summary:
	//     Removes a specific group from the collection.
	// Parameters:
	//     pGroup :  Reference to the group to remove.
	// -------------------------------------------------
	void Remove(CXTPFlowGraphNodeGroup* pGroup);

	// ----------------------------------------------------------------------
	//
	// Summary:
	//     Reference to the page that these groups are displayed on.
	// Returns:
	//     Reference to the FlowGraphPage that these groups are displayed on.
	// ----------------------------------------------------------------------
	CXTPFlowGraphPage* GetPage() const;

protected:
	CXTPFlowGraphPage* m_pPage; // Pointer to the page that this collection of groups is displayed on.

	CArray<CXTPFlowGraphNodeGroup*, CXTPFlowGraphNodeGroup*> m_arrGroups; // Collection of groups.

	friend class CXTPFlowGraphPage;

};

AFX_INLINE CXTPFlowGraphPage* CXTPFlowGraphNodeGroups::GetPage() const {
	return m_pPage;
}
AFX_INLINE int CXTPFlowGraphNodeGroups::GetCount() const {
	return (int)m_arrGroups.GetSize();
}
AFX_INLINE CXTPFlowGraphNodeGroup* CXTPFlowGraphNodeGroups::GetAt(int nIndex) const {
	return nIndex >= 0 && nIndex < m_arrGroups.GetSize() ? m_arrGroups[nIndex] : NULL;
}

#endif //#if !defined(__XTPFLOWGRAPHNODEGROUPS_H__)
