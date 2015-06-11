// XTPFlowGraphUndoManager.h: interface for the CXTPFlowGraphUndoManager class.
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
#if !defined(__XTPFLOWGRAPHUNDOMANAGER_H__)
#define __XTPFLOWGRAPHUNDOMANAGER_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

const int XTP_ID_FLOWGRAPHUNDO_GROUP = -1;                 // The undo group command.
const int XTP_ID_FLOWGRAPHUNDO_ADDNODE = 1003;             // The undo add Node command.
const int XTP_ID_FLOWGRAPHUNDO_DELETENODE = 1004;          // The undo delete Node command.
const int XTP_ID_FLOWGRAPHUNDO_SETNODEPOSITION = 1005;     // The undo set Node position command.
const int XTP_ID_FLOWGRAPHUNDO_DELETECONNECTION= 1006;     // The undo delete Connection command.
const int XTP_ID_FLOWGRAPHUNDO_SETCONNECTIONPOSITION = 1007;     // The undo set Connection position command.
const int XTP_ID_FLOWGRAPHUNDO_ADDCONNECTION = 1008;             // The undo add Connection command.
const int XTP_ID_FLOWGRAPHUNDO_SETNODECAPTION = 1009;       // The set node caption command.
const int XTP_ID_FLOWGRAPHUNDO_SETNODESIZE = 1010;          // The set node size command.
const int XTP_ID_FLOWGRAPHUNDO_DELETECONNECTIONPOINT = 1011;// The undo delete point command.
const int XTP_ID_FLOWGRAPHUNDO_ADDCONNECTIONPOINT = 1012;   // The undo add point command.

//-----------------------------------------------------------------------
// Summary:
//     This class abstracts the undo command.
//-----------------------------------------------------------------------
class _XTP_EXT_CLASS CXTPFlowGraphUndoCommand
{
protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructor, creates a CXTPFlowGraphUndoCommand object.
	// Parameters:
	//     nID - The command id.
	//-----------------------------------------------------------------------
	CXTPFlowGraphUndoCommand(int nID);

	//-----------------------------------------------------------------------
	// Summary:
	//     CXTPFlowGraphUndoCommand destructor, does the cleanups.
	//-----------------------------------------------------------------------
	virtual ~CXTPFlowGraphUndoCommand();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the command id associated with this object.
	// Returns:
	//     An integer value denoting the contained command id.
	//-----------------------------------------------------------------------
	int GetID() const;
protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Use this function to do the undo operation.
	// Remarks:
	//     It a virtual function, so the derived classes can give their
	//     specific implementations
	//-----------------------------------------------------------------------
	virtual void Undo();

protected:
	const int m_nID;                        //The command id.

	friend class CXTPFlowGraphUndoManager;
	friend class CXTPFlowGraphUndoGroupCommand;
};

//-----------------------------------------------------------------------
// Summary:
//     This class abstracts the undo commands in a group.
//-----------------------------------------------------------------------
class _XTP_EXT_CLASS CXTPFlowGraphUndoGroupCommand : public CXTPFlowGraphUndoCommand
{
protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructor, creates a CXTPFlowGraphUndoCommand object.
	// Parameters:
	//     pManager - Pointer to the FlowGraph undo manager.
	//-----------------------------------------------------------------------
	CXTPFlowGraphUndoGroupCommand(CXTPFlowGraphUndoManager* pManager);

	//-----------------------------------------------------------------------
	// Summary:
	//     CXTPFlowGraphUndoGroupCommand destructor, does the cleanups.
	//-----------------------------------------------------------------------
	virtual ~CXTPFlowGraphUndoGroupCommand();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to do the undo operation.
	// Remarks:
	//     It a virtual function, so the derived classes can give their
	//     specific implementations
	//-----------------------------------------------------------------------
	virtual void Undo();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to get the number of command objects in this
	//     group.
	// Returns:
	//     An integer value denoting the number of commands in the group.
	//     specific implementations
	//-----------------------------------------------------------------------
	int GetCount() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to add a command to the command group.
	// Parameters:
	//     pCommand - A pointer to the undo command object.
	//-----------------------------------------------------------------------
	void Add(CXTPFlowGraphUndoCommand* pCommand);


protected:
	CArray<CXTPFlowGraphUndoCommand*, CXTPFlowGraphUndoCommand*> m_arrUndoCommands; //The command collection.
	CXTPFlowGraphUndoManager* m_pManager;                                       //The undo manager.
	friend class CXTPFlowGraphUndoManager;

};

//-----------------------------------------------------------------------
// Summary:
//     This class abstracts the undo delete Node command.This is a kind of
//     CXTPFlowGraphUndoCommand.
//-----------------------------------------------------------------------
class _XTP_EXT_CLASS CXTPFlowGraphUndoDeleteNodeCommand : public CXTPFlowGraphUndoCommand
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructor, creates a CXTPFlowGraphUndoDeleteNodeCommand object.
	// Parameters:
	//     pNode       - Pointer to the FlowGraph node object.
	//-----------------------------------------------------------------------
	CXTPFlowGraphUndoDeleteNodeCommand(CXTPFlowGraphNode* pNode);

	//-----------------------------------------------------------------------
	// Summary:
	//     CXTPFlowGraphUndoDeleteNodeCommand destructor, does the cleanups.
	//-----------------------------------------------------------------------
	virtual ~CXTPFlowGraphUndoDeleteNodeCommand();

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Use this function to do the undo operation.
	// Remarks:
	//     It a virtual function, so the derived classes can give their
	//     specific implementations
	//-----------------------------------------------------------------------
	virtual void Undo();

protected:
	CXTPFlowGraphNode* m_pNode;           //The FlowGraph Node object.
	CXTPFlowGraphPage* m_pPage;           //The FlowGraph Page object.
};


//-----------------------------------------------------------------------
// Summary:
//     This class abstracts the undo delete Commection Point command.This is a kind of
//     CXTPFlowGraphUndoCommand.
//-----------------------------------------------------------------------
class _XTP_EXT_CLASS CXTPFlowGraphUndoDeleteConnectionPointCommand : public CXTPFlowGraphUndoCommand
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructor, creates a CXTPFlowGraphUndoDeleteConnectionPointCommand object.
	// Parameters:
	//     pConnectionPoint       - Pointer to the FlowGraph point object.
	//-----------------------------------------------------------------------
	CXTPFlowGraphUndoDeleteConnectionPointCommand(CXTPFlowGraphConnectionPoint* pConnectionPoint);

	//-----------------------------------------------------------------------
	// Summary:
	//     CXTPFlowGraphUndoDeleteConnectionPointCommand destructor, does the cleanups.
	//-----------------------------------------------------------------------
	virtual ~CXTPFlowGraphUndoDeleteConnectionPointCommand();

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Use this function to do the undo operation.
	// Remarks:
	//     It a virtual function, so the derived classes can give their
	//     specific implementations
	//-----------------------------------------------------------------------
	virtual void Undo();

protected:
	CXTPFlowGraphNode* m_pNode;           //The FlowGraph Node object.
	CXTPFlowGraphConnectionPoint* m_pConnectionPoint;  //The FlowGraph Connection point object.
	int m_nIndex;
};

//-----------------------------------------------------------------------
// Summary:
//     This class abstracts the undo delete Connection command.This is a kind of
//     CXTPFlowGraphUndoCommand.
//-----------------------------------------------------------------------
class _XTP_EXT_CLASS CXTPFlowGraphUndoDeleteConnectionCommand : public CXTPFlowGraphUndoCommand
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructor, creates a CXTPFlowGraphUndoDeleteConnectionCommand object.
	// Parameters:
	//     pNode       - Pointer to the FlowGraph node object.
	//-----------------------------------------------------------------------
	CXTPFlowGraphUndoDeleteConnectionCommand(CXTPFlowGraphConnection* pConnection);

	//-----------------------------------------------------------------------
	// Summary:
	//     CXTPFlowGraphUndoDeleteConnectionCommand destructor, does the cleanups.
	//-----------------------------------------------------------------------
	virtual ~CXTPFlowGraphUndoDeleteConnectionCommand();

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Use this function to do the undo operation.
	// Remarks:
	//     It a virtual function, so the derived classes can give their
	//     specific implementations
	//-----------------------------------------------------------------------
	virtual void Undo();

protected:
	CXTPFlowGraphConnection* m_pConnection;       //The FlowGraph Connection object.
	CXTPFlowGraphConnectionPoint* m_pInputPoint;
	CXTPFlowGraphConnectionPoint* m_pOutputPoint;
	CXTPFlowGraphPage* m_pPage;           //The FlowGraph Page object.
};

//-----------------------------------------------------------------------
// Summary:
//     This class abstracts the undo add Node command.This is a kind of
//     CXTPFlowGraphUndoCommand.
//-----------------------------------------------------------------------
class _XTP_EXT_CLASS CXTPFlowGraphUndoAddNodeCommand : public CXTPFlowGraphUndoCommand
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructor, creates a CXTPFlowGraphUndoAddNodeCommand object.
	// Parameters:
	//     pNode       - Pointer to the FlowGraph Node object.
	//-----------------------------------------------------------------------
	CXTPFlowGraphUndoAddNodeCommand(CXTPFlowGraphNode* pNode);

	//-----------------------------------------------------------------------
	// Summary:
	//     CXTPFlowGraphUndoAddNodeCommand destructor, does the cleanups.
	//-----------------------------------------------------------------------
	virtual ~CXTPFlowGraphUndoAddNodeCommand();

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Use this function to do the undo operation.
	// Remarks:
	//     It a virtual function, so the derived classes can give their
	//     specific implementations
	//-----------------------------------------------------------------------
	virtual void Undo();

protected:
	CXTPFlowGraphNode* m_pNode;   //Pointer to the FlowGraph Node object.
};


//-----------------------------------------------------------------------
// Summary:
//     This class abstracts the undo add Point command.This is a kind of
//     CXTPFlowGraphUndoCommand.
//-----------------------------------------------------------------------
class _XTP_EXT_CLASS CXTPFlowGraphUndoAddConnectionPointCommand : public CXTPFlowGraphUndoCommand
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructor, creates a CXTPFlowGraphUndoAddConnectionPointCommand object.
	// Parameters:
	//     pConnectionPoint       - Pointer to the FlowGraph Point object.
	//-----------------------------------------------------------------------
	CXTPFlowGraphUndoAddConnectionPointCommand(CXTPFlowGraphConnectionPoint* pConnectionPoint);

	//-----------------------------------------------------------------------
	// Summary:
	//     CXTPFlowGraphUndoAddConnectionPointCommand destructor, does the cleanups.
	//-----------------------------------------------------------------------
	virtual ~CXTPFlowGraphUndoAddConnectionPointCommand();

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Use this function to do the undo operation.
	// Remarks:
	//     It a virtual function, so the derived classes can give their
	//     specific implementations
	//-----------------------------------------------------------------------
	virtual void Undo();

protected:
	CXTPFlowGraphConnectionPoint* m_pConnectionPoint;   //Pointer to the FlowGraph Point object.
};



//-----------------------------------------------------------------------
// Summary:
//     This class abstracts the undo add Node command.This is a kind of
//     CXTPFlowGraphUndoCommand.
//-----------------------------------------------------------------------
class _XTP_EXT_CLASS CXTPFlowGraphUndoAddConnectionCommand : public CXTPFlowGraphUndoCommand
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructor, creates a CXTPFlowGraphUndoAddConnectionCommand object.
	// Parameters:
	//     pNode       - Pointer to the FlowGraph Node object.
	//-----------------------------------------------------------------------
	CXTPFlowGraphUndoAddConnectionCommand(CXTPFlowGraphConnection* pConnection);

	//-----------------------------------------------------------------------
	// Summary:
	//     CXTPFlowGraphUndoAddConnectionCommand destructor, does the cleanups.
	//-----------------------------------------------------------------------
	virtual ~CXTPFlowGraphUndoAddConnectionCommand();

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Use this function to do the undo operation.
	// Remarks:
	//     It a virtual function, so the derived classes can give their
	//     specific implementations
	//-----------------------------------------------------------------------
	virtual void Undo();

protected:
	CXTPFlowGraphConnection* m_pConnection;   //Pointer to the FlowGraph Node object.
};


//-----------------------------------------------------------------------
// Summary:
//     This class abstracts the undo set Node position command.This is a
//     kind of CXTPFlowGraphUndoCommand.
//-----------------------------------------------------------------------
class _XTP_EXT_CLASS CXTPFlowGraphUndoSetNodePositionCommand : public CXTPFlowGraphUndoCommand
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructor, creates a CXTPFlowGraphUndoSetNodePositionCommand object.
	// Parameters:
	//     pNode       - Pointer to the FlowGraph Node object.
	//     nOldPosition - The old position of the Node.
	//-----------------------------------------------------------------------
	CXTPFlowGraphUndoSetNodePositionCommand(CXTPFlowGraphNode* pNode, CPoint ptOldPosition);

	//-----------------------------------------------------------------------
	// Summary:
	//     CXTPFlowGraphUndoSetNodePositionCommand destructor, does the cleanups.
	//-----------------------------------------------------------------------
	virtual ~CXTPFlowGraphUndoSetNodePositionCommand();

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Use this function to do the undo operation.
	// Remarks:
	//     It a virtual function, so the derived classes can give their
	//     specific implementations
	//-----------------------------------------------------------------------
	virtual void Undo();

protected:
	CXTPFlowGraphNode* m_pNode;   //Pointer to the FlowGraph Node object.
	CPoint m_ptOldPosition;         //Old position of the Node.
};

//-----------------------------------------------------------------------
// Summary:
//     This class abstracts the undo set Node size command.This is a
//     kind of CXTPFlowGraphUndoCommand.
//-----------------------------------------------------------------------
class _XTP_EXT_CLASS CXTPFlowGraphUndoSetNodeSizeCommand : public CXTPFlowGraphUndoCommand
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructor, creates a CXTPFlowGraphUndoSetNodeSizeCommand object.
	// Parameters:
	//     pNode       - Pointer to the FlowGraph Node object.
	//     nOldPosition - The old position of the Node.
	//-----------------------------------------------------------------------
	CXTPFlowGraphUndoSetNodeSizeCommand(CXTPFlowGraphNode* pNode, CSize szOldSize);

	//-----------------------------------------------------------------------
	// Summary:
	//     CXTPFlowGraphUndoSetNodeSizeCommand destructor, does the cleanups.
	//-----------------------------------------------------------------------
	virtual ~CXTPFlowGraphUndoSetNodeSizeCommand();

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Use this function to do the undo operation.
	// Remarks:
	//     It a virtual function, so the derived classes can give their
	//     specific implementations
	//-----------------------------------------------------------------------
	virtual void Undo();

protected:
	CXTPFlowGraphNode* m_pNode;   //Pointer to the FlowGraph Node object.
	CPoint m_szOldSize;         //Old size of the Node.
};


//-----------------------------------------------------------------------
// Summary:
//     This class abstracts the undo set Node position command.This is a
//     kind of CXTPFlowGraphUndoCommand.
//-----------------------------------------------------------------------
class _XTP_EXT_CLASS CXTPFlowGraphUndoSetNodeCaptionCommand : public CXTPFlowGraphUndoCommand
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructor, creates a CXTPFlowGraphUndoSetNodeCaptionCommand object.
	// Parameters:
	//     pNode       - Pointer to the FlowGraph Node object.
	//     strOldCaption - The old caption of the Node.
	//-----------------------------------------------------------------------
	CXTPFlowGraphUndoSetNodeCaptionCommand(CXTPFlowGraphElement* pNode, CString strOldCaption);

	//-----------------------------------------------------------------------
	// Summary:
	//     CXTPFlowGraphUndoSetNodeCaptionCommand destructor, does the cleanups.
	//-----------------------------------------------------------------------
	virtual ~CXTPFlowGraphUndoSetNodeCaptionCommand();

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Use this function to do the undo operation.
	// Remarks:
	//     It a virtual function, so the derived classes can give their
	//     specific implementations
	//-----------------------------------------------------------------------
	virtual void Undo();

protected:
	CXTPFlowGraphElement* m_pNode;   //Pointer to the FlowGraph Node object.
	CString m_strOldCaption;         //Old position of the Node.
};

//-----------------------------------------------------------------------
// Summary:
//     This class abstracts the undo set Node position command.This is a
//     kind of CXTPFlowGraphUndoCommand.
//-----------------------------------------------------------------------
class _XTP_EXT_CLASS CXTPFlowGraphUndoSetConnectionPositionCommand : public CXTPFlowGraphUndoCommand
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructor, creates a CXTPFlowGraphUndoSetConnectionPositionCommand object.
	// Parameters:
	//     pNode       - Pointer to the FlowGraph Node object.
	//     nOldPosition - The old position of the Node.
	//-----------------------------------------------------------------------
	CXTPFlowGraphUndoSetConnectionPositionCommand(CXTPFlowGraphConnection* pConnection, CPoint ptOldPosition);

	//-----------------------------------------------------------------------
	// Summary:
	//     CXTPFlowGraphUndoSetNodePositionCommand destructor, does the cleanups.
	//-----------------------------------------------------------------------
	virtual ~CXTPFlowGraphUndoSetConnectionPositionCommand();

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Use this function to do the undo operation.
	// Remarks:
	//     It a virtual function, so the derived classes can give their
	//     specific implementations
	//-----------------------------------------------------------------------
	virtual void Undo();

protected:
	CXTPFlowGraphConnection* m_pConnection;   //Pointer to the FlowGraph Connection object.
	CPoint m_ptOldPosition;         //Old position of the Node.
};


// ---------------------------------------------------------------------
// Summary:
//     This class abstracts an undo manager.It manages all the undo
//     \operations of the FlowGraph contorl.This is a kind of
//     CXTPCmdTarget.
// Remarks:
//     The FlowGraphUndoManager allows certain edit operations to be
//     undone and re-done in the Flow Graph. Supported operations are
//     Add/Remove/Move/Resize Nodes/Move Connector Lines/Rename
//     Nodes/Grouping. If any of these operations are performed they can
//     easily be undone\\re-done using the methods of the
//     FlowGraphUndoManager.
//
//     StartGroup and EndGroup are used to "group" a set of "undoable"
//     \operations together. Grouping the operations together is useful
//     as grouped operations can be Undone or Re-done with a single call
//     to Undo or Redo. If the operations are not grouped you would have
//     to call Undo or Redo for each operation performed.
//
//     Any "undoable" operations performed between StartGroup and
//     EndGroup will be undone\\re-done with a single call to Undo or
//     Redo.
// ---------------------------------------------------------------------
class _XTP_EXT_CLASS CXTPFlowGraphUndoManager : public CXTPCmdTarget
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructor, creates a CXTPFlowGraphUndoManager object.
	//-----------------------------------------------------------------------
	CXTPFlowGraphUndoManager();

	//-----------------------------------------------------------------------
	// Summary:
	//     CXTPFlowGraphUndoManager destructor, does the cleanups.
	//-----------------------------------------------------------------------
	virtual ~CXTPFlowGraphUndoManager();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Use this function to do the undo operation.
	//-----------------------------------------------------------------------
	virtual void Undo();

	//-----------------------------------------------------------------------
	// Summary:
	//     Use this function to do the redo operation.
	//-----------------------------------------------------------------------
	virtual void Redo();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to check whether the undo operation is possible
	//     at the current context.
	// Returns:
	//     TRUE if undo is possible, FALSE else.
	//-----------------------------------------------------------------------
	BOOL CanUndo() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to check whether the redo operation is possible
	//     at the current context.
	// Returns:
	//     TRUE if undo is possible, FALSE else.
	//-----------------------------------------------------------------------
	BOOL CanRedo() const;

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to clear the list of undo and redo operations.
	//-----------------------------------------------------------------------
	void Clear();

	//-----------------------------------------------------------------------
	// Summary:
	//     Use this function to add an undo command to the collection of undo
	//     commands.
	// Parameters:
	//     pCommand - A pointer to the FlowGraph undo command object.
	//-----------------------------------------------------------------------
	void AddUndoCommand(CXTPFlowGraphUndoCommand* pCommand);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to start an undo group command.
	//-----------------------------------------------------------------------
	virtual void StartGroup();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to end an undo group command.
	//-----------------------------------------------------------------------
	virtual void EndGroup();

	//-----------------------------------------------------------------------
	// Summary:
	//     Use this function to enable or disable the undo manager.
	// Parameters:
	//     bEnabled - TRUE to enable, FALSE to disable.
	//-----------------------------------------------------------------------
	void EnableUndoManager(BOOL bEnabled);

protected:
	CArray<CXTPFlowGraphUndoCommand*, CXTPFlowGraphUndoCommand*> m_arrUndoCommands; //The undo commands array.
	CArray<CXTPFlowGraphUndoCommand*, CXTPFlowGraphUndoCommand*> m_arrRedoCommands; //The redo command array.

	CArray<CXTPFlowGraphUndoGroupCommand*, CXTPFlowGraphUndoGroupCommand*> m_arrGroupStack; // The groups stack.

	int m_nUndoContext;                                                     //The undo context.
	BOOL m_bEnabled;                                                        // Specifies whether the undo manager is enabled or disabled.
	CXTPFlowGraphUndoGroupCommand* m_pUndoGroup;                                //The undo group command.


#ifdef _XTP_ACTIVEX
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPFlowGraphUndoManager);
//}}AFX_CODEJOCK_PRIVATE
#endif
};

AFX_INLINE int CXTPFlowGraphUndoCommand::GetID() const {
	return m_nID;
}
AFX_INLINE void CXTPFlowGraphUndoManager::EnableUndoManager(BOOL bEnabled) {
	m_bEnabled = bEnabled;
}

#endif //#if !defined(__XTPFLOWGRAPHUNDOMANAGER_H__)
