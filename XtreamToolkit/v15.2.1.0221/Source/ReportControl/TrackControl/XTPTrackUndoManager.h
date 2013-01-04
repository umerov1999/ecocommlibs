// XTPTrackUndoManager.h: interface for the CXTPTrackUndoManager class.
//
// This file is a part of the XTREME REPORTCONTROL MFC class library.
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
#if !defined(__XTPTRACKUNDOMANAGER_H__)
#define __XTPTRACKUNDOMANAGER_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


const int XTP_ID_TRACKUNDO_GROUP = -1;                  //The undo group command.
const int XTP_ID_TRACKUNDO_SETMARKERPOSITION = 1000;    //The undo set marker position command.
const int XTP_ID_TRACKUNDO_ADDMARKER = 1001;            //The undo add marker command.
const int XTP_ID_TRACKUNDO_DELETEMARKER = 1002;         //The undo delete marker command.

const int XTP_ID_TRACKUNDO_ADDBLOCK = 1003;             //The undo add block command.
const int XTP_ID_TRACKUNDO_DELETEBLOCK = 1004;          //The undo delete block command.
const int XTP_ID_TRACKUNDO_SETBLOCKPOSITION = 1005;     //The undo set block position command.

//-----------------------------------------------------------------------
// Summary:
//     This class abstracts the undo command.
//-----------------------------------------------------------------------
class _XTP_EXT_CLASS CXTPTrackUndoCommand
{
protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructor, creates a CXTPTrackUndoCommand object.
	// Parameters:
	//     nID - The command id.
	//-----------------------------------------------------------------------
	CXTPTrackUndoCommand(int nID);

	//-----------------------------------------------------------------------
	// Summary:
	//     CXTPTrackUndoCommand destructor, does the cleanups.
	//-----------------------------------------------------------------------
	virtual ~CXTPTrackUndoCommand();

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

	friend class CXTPTrackUndoManager;
	friend class CXTPTrackUndoGroupCommand;
};

//-----------------------------------------------------------------------
// Summary:
//     This class abstracts the undo commands in a group.
//-----------------------------------------------------------------------
class _XTP_EXT_CLASS CXTPTrackUndoGroupCommand : public CXTPTrackUndoCommand
{
protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructor, creates a CXTPTrackUndoCommand object.
	// Parameters:
	//     pManager - Pointer to the track undo manager.
	//-----------------------------------------------------------------------
	CXTPTrackUndoGroupCommand(CXTPTrackUndoManager* pManager);

	//-----------------------------------------------------------------------
	// Summary:
	//     CXTPTrackUndoGroupCommand destructor, does the cleanups.
	//-----------------------------------------------------------------------
	virtual ~CXTPTrackUndoGroupCommand();

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
	void Add(CXTPTrackUndoCommand* pCommand);


protected:
	CArray<CXTPTrackUndoCommand*, CXTPTrackUndoCommand*> m_arrUndoCommands; //The command collection.
	CXTPTrackUndoManager* m_pManager;                                       //The undo manager.
	friend class CXTPTrackUndoManager;

};

//-----------------------------------------------------------------------
// Summary:
//     This class abstarcts the undo delete marker command.This is a kind of
//     CXTPTrackUndoCommand.
//-----------------------------------------------------------------------
class _XTP_EXT_CLASS CXTPTrackUndoDeleteMarkerCommand : public CXTPTrackUndoCommand
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructor, creates a CXTPTrackUndoDeleteMarkerCommand object.
	// Parameters:
	//     pMarker - Pointer to the track marker object.
	//-----------------------------------------------------------------------
	CXTPTrackUndoDeleteMarkerCommand(CXTPTrackMarker* pMarker);

	//-----------------------------------------------------------------------
	// Summary:
	//     CXTPTrackUndoDeleteMarkerCommand destructor, does the cleanups.
	//-----------------------------------------------------------------------
	virtual ~CXTPTrackUndoDeleteMarkerCommand();

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
	CXTPTrackMarker* m_pMarker; //The track marker object.
};

//-----------------------------------------------------------------------
// Summary:
//     This class abstracts the undo add marker command.This is a kind of
//     CXTPTrackUndoCommand.
//-----------------------------------------------------------------------
class _XTP_EXT_CLASS CXTPTrackUndoAddMarkerCommand : public CXTPTrackUndoCommand
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructor, creates a CXTPTrackUndoDeleteMarkerCommand object.
	// Parameters:
	//     pMarker - Pointer to the track marker object.
	//-----------------------------------------------------------------------
	CXTPTrackUndoAddMarkerCommand(CXTPTrackMarker* pMarker);

	//-----------------------------------------------------------------------
	// Summary:
	//     CXTPTrackUndoAddMarkerCommand destructor, does the cleanups.
	//-----------------------------------------------------------------------
	virtual ~CXTPTrackUndoAddMarkerCommand();

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
	CXTPTrackMarker* m_pMarker; //The track marker object.
};


//-----------------------------------------------------------------------
// Summary:
//     This class abstracts the undo set marker position command.This is a kind of
//     CXTPTrackUndoCommand.
//-----------------------------------------------------------------------
class _XTP_EXT_CLASS CXTPTrackUndoSetMarkerPositionCommand : public CXTPTrackUndoCommand
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructor, creates a CXTPTrackUndoSetMarkerPositionCommand object.
	// Parameters:
	//     pMarker      - Pointer to the track marker object.
	//     nOldPosition - The old position of the marker.
	//-----------------------------------------------------------------------
	CXTPTrackUndoSetMarkerPositionCommand(CXTPTrackMarker* pMarker, int nOldPosition);

	//-----------------------------------------------------------------------
	// Summary:
	//     CXTPTrackUndoSetMarkerPositionCommand destructor, does the cleanups.
	//-----------------------------------------------------------------------
	virtual ~CXTPTrackUndoSetMarkerPositionCommand();

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
	CXTPTrackMarker* m_pMarker; //The track marker object.
	int m_nOldPosition;         //The old position of the marker.
};

//-----------------------------------------------------------------------
// Summary:
//     This class abstracts the undo delete block command.This is a kind of
//     CXTPTrackUndoCommand.
//-----------------------------------------------------------------------
class _XTP_EXT_CLASS CXTPTrackUndoDeleteBlockCommand : public CXTPTrackUndoCommand
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructor, creates a CXTPTrackUndoDeleteBlockCommand object.
	// Parameters:
	//     pBlock       - Pointer to the track block object.
	//-----------------------------------------------------------------------
	CXTPTrackUndoDeleteBlockCommand(CXTPTrackBlock* pBlock);

	//-----------------------------------------------------------------------
	// Summary:
	//     CXTPTrackUndoDeleteBlockCommand destructor, does the cleanups.
	//-----------------------------------------------------------------------
	virtual ~CXTPTrackUndoDeleteBlockCommand();

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
	CXTPTrackBlock* m_pBlock;           //The track block object.
	CXTPTrackControlItem* m_pOldItem;   //The old track control item.
};

//-----------------------------------------------------------------------
// Summary:
//     This class abstracts the undo add block command.This is a kind of
//     CXTPTrackUndoCommand.
//-----------------------------------------------------------------------
class _XTP_EXT_CLASS CXTPTrackUndoAddBlockCommand : public CXTPTrackUndoCommand
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructor, creates a CXTPTrackUndoAddBlockCommand object.
	// Parameters:
	//     pBlock       - Pointer to the track block object.
	//-----------------------------------------------------------------------
	CXTPTrackUndoAddBlockCommand(CXTPTrackBlock* pBlock);

	//-----------------------------------------------------------------------
	// Summary:
	//     CXTPTrackUndoAddBlockCommand destructor, does the cleanups.
	//-----------------------------------------------------------------------
	virtual ~CXTPTrackUndoAddBlockCommand();

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
	CXTPTrackBlock* m_pBlock;   //Pointer to the track block object.
};

//-----------------------------------------------------------------------
// Summary:
//     This class abstracts the undo set block position command.This is a
//     kind of CXTPTrackUndoCommand.
//-----------------------------------------------------------------------
class _XTP_EXT_CLASS CXTPTrackUndoSetBlockPositionCommand : public CXTPTrackUndoCommand
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructor, creates a CXTPTrackUndoSetBlockPositionCommand object.
	// Parameters:
	//     pBlock       - Pointer to the track block object.
	//     nOldPosition - The old position of the block.
	//     nOldLength   - The old length of the block.
	//-----------------------------------------------------------------------
	CXTPTrackUndoSetBlockPositionCommand(CXTPTrackBlock* pBlock, int nOldPosition, int nOldLength);

	//-----------------------------------------------------------------------
	// Summary:
	//     CXTPTrackUndoSetBlockPositionCommand destructor, does the cleanups.
	//-----------------------------------------------------------------------
	virtual ~CXTPTrackUndoSetBlockPositionCommand();

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
	CXTPTrackBlock* m_pBlock;   //Pointer to the track block object.
	int m_nOldPosition;         //Old position of the block.
	int m_nOldLength;           //Old length of the block.
};

//-----------------------------------------------------------------------
// Summary:
//     This class abstracts an undo manager.It manages all the undo
//     operations of the track contorl.This is a kind of CXTPCmdTarget.
//-----------------------------------------------------------------------
class _XTP_EXT_CLASS CXTPTrackUndoManager : public CXTPCmdTarget
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructor, creates a CXTPTrackUndoManager object.
	//-----------------------------------------------------------------------
	CXTPTrackUndoManager();

	//-----------------------------------------------------------------------
	// Summary:
	//     CXTPTrackUndoManager destructor, does the cleanups.
	//-----------------------------------------------------------------------
	~CXTPTrackUndoManager();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Use this function to do the undo operation.
	//-----------------------------------------------------------------------
	void Undo();

	//-----------------------------------------------------------------------
	// Summary:
	//     Use this function to do the redo operation.
	//-----------------------------------------------------------------------
	void Redo();

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
	//     pCommand - A pointer to the track undo command object.
	//-----------------------------------------------------------------------
	void AddUndoCommand(CXTPTrackUndoCommand* pCommand);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to start an undo group command.
	//-----------------------------------------------------------------------
	void StartGroup();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this function to end an undo group command.
	//-----------------------------------------------------------------------
	void EndGroup();

protected:
	CArray<CXTPTrackUndoCommand*, CXTPTrackUndoCommand*> m_arrUndoCommands; //The undo commands array.
	CArray<CXTPTrackUndoCommand*, CXTPTrackUndoCommand*> m_arrRedoCommands; //The redo command array.

	int m_nUndoContext;                                                     //The undo context.
	CXTPTrackUndoGroupCommand* m_pUndoGroup;                                //The undo group command.


#ifdef _XTP_ACTIVEX
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPTrackUndoManager);
//}}AFX_CODEJOCK_PRIVATE
#endif
};

AFX_INLINE int CXTPTrackUndoCommand::GetID() const {
	return m_nID;
}

#endif //#if !defined(__XTPTRACKUNDOMANAGER_H__)
