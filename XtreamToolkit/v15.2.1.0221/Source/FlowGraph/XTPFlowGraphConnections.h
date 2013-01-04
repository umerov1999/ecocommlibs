// XTPFlowGraphConnections.h: interface for the CXTPFlowGraphConnections class.
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
#if !defined(__XTPFLOWGRAPHCONNECTIONS_H__)
#define __XTPFLOWGRAPHCONNECTIONS_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXTPFlowGraphPage;
class CXTPFlowGraphConnection;

// ------------------------------------------------------
//
// Summary:
//     Represents a collection of Connections for a Node.
// ------------------------------------------------------
class _XTP_EXT_CLASS CXTPFlowGraphConnections : public CXTPCmdTarget
{
protected:
	// -------------------------------------------------
	// Summary:
	//     Constructs a CXTPFlowGraphConnections object.
	// -------------------------------------------------
	CXTPFlowGraphConnections(CXTPFlowGraphPage* pPage);
	// -------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPFlowGraphConnections object, handles cleanup and
	//     deallocation.
	// -------------------------------------------------------------------
	virtual ~CXTPFlowGraphConnections();

public:
	// ------------------------------------------
	// Summary:
	//     Removes all connections from the node.
	// ------------------------------------------
	void RemoveAll();

	// ---------------------------------------------------------------
	// Summary:
	//     Adds a connection to the Node.
	// Parameters:
	//     pConnection :  Reference to the connection to add.
	// Returns:
	//     \Returns a reference to the connection that was just added.
	// ---------------------------------------------------------------
	CXTPFlowGraphConnection* AddConnection(CXTPFlowGraphConnection* pConnection);

	// ---------------------------------------------------------
	// Summary:
	//     Removes the specified connection.
	// Parameters:
	//     pConnection :  Reference to the connection to remove.
	// ---------------------------------------------------------
	void Remove(CXTPFlowGraphConnection* pConnection);
	// ------------------------------------------------------------------
	// Summary:
	//     Removes the specified connection from the node's collection of
	//     connections.
	// Parameters:
	//     nIndex :  Index of the connection to remove.
	// ------------------------------------------------------------------
	void RemoveAt(int nIndex);

	// ----------------------------------------------------------
	// Summary:
	//     Gets the total number of connections for the node.
	// Returns:
	//     \Returns the total number of connections for the node.
	// ----------------------------------------------------------
	int GetCount() const;
	// ---------------------------------------------------------
	//
	// Summary:
	//     Gets the connection at the specified index.
	// Parameters:
	//     nIndex :  Index of the desired connection to return.
	//
	// Returns:
	//     Reference to the connection at the specified index.
	// ---------------------------------------------------------
	CXTPFlowGraphConnection* GetAt(int nIndex) const;

public:
	// ------------------------------------------------------------------------
	// Summary:
	//     Call this member function to Store/Load a collection of
	//     connections using the specified data object.
	// Parameters:
	//     pPX :  Source or destination CXTPPropExchange data object reference.
	// Remarks:
	//     This member function is used to store or load a collection
	//     connections data to or from a storage.
	// ------------------------------------------------------------------------
	void DoPropExchange(CXTPPropExchange* pPX);

protected:
	CXTPFlowGraphPage* m_pPage; // Pointer to the page that displays these connections.
	CArray<CXTPFlowGraphConnection*, CXTPFlowGraphConnection*> m_arrConnections; // Collection of connections.

	friend class CXTPFlowGraphPage;

};

AFX_INLINE int CXTPFlowGraphConnections::GetCount() const {
	return (int)m_arrConnections.GetSize();
}
AFX_INLINE CXTPFlowGraphConnection* CXTPFlowGraphConnections::GetAt(int nIndex) const {
	return nIndex >= 0 && nIndex < m_arrConnections.GetSize() ? m_arrConnections[nIndex] : NULL;
}

#endif //#if !defined(__XTPFLOWGRAPHCONNECTIONS_H__)
