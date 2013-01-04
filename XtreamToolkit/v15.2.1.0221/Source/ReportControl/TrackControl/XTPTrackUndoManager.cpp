// XTPTrackUndoManager.cpp : implementation of the CXTPTrackUndoManager class.
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

#include "stdafx.h"

#include "Common/XTPCustomHeap.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPSmartPtrInternalT.h"


#include "../XTPReportDefines.h"
#include "../XTPReportRecordItem.h"
#include "../XTPReportControl.h"


#include "XTPTrackControl.h"
#include "XTPTrackControlItem.h"
#include "XTPTrackUndoManager.h"
#include "XTPTrackBlock.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int CONTEXT_NONE = 0;
const int CONTEXT_UNDO = 1;
const int CONTEXT_REDO = 2;

//////////////////////////////////////////////////////////////////////////
// CXTPTrackUndoCommand

CXTPTrackUndoCommand::CXTPTrackUndoCommand(int nID)
	: m_nID(nID)
{

}

CXTPTrackUndoCommand::~CXTPTrackUndoCommand()
{
}

void CXTPTrackUndoCommand::Undo()
{
	ASSERT(FALSE);
}


/////////////////////////////////////////////////////////////////////////
// CXTPTrackUndoManager
CXTPTrackUndoManager::CXTPTrackUndoManager()
{
	m_nUndoContext = CONTEXT_NONE;

	m_pUndoGroup = NULL;

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPTrackUndoManager::~CXTPTrackUndoManager()
{
	Clear();

	SAFE_DELETE(m_pUndoGroup);
}

void CXTPTrackUndoManager::Clear()
{
	int i;
	for (i = 0; i < m_arrUndoCommands.GetSize(); i++)
	{
		delete m_arrUndoCommands[i];
	}
	m_arrUndoCommands.RemoveAll();

	for (i = 0; i < m_arrRedoCommands.GetSize(); i++)
	{
		delete m_arrRedoCommands[i];
	}
	m_arrRedoCommands.RemoveAll();

	SAFE_DELETE(m_pUndoGroup);
}

void CXTPTrackUndoManager::StartGroup()
{
	ASSERT(m_pUndoGroup == NULL);
	m_pUndoGroup = new CXTPTrackUndoGroupCommand(this);
}

void CXTPTrackUndoManager::EndGroup()
{
	if (m_pUndoGroup->GetCount() == 0)
	{
		delete m_pUndoGroup;
	}
	else
	{
		if (m_nUndoContext  != CONTEXT_UNDO)
			m_arrUndoCommands.Add(m_pUndoGroup);
		else
			m_arrRedoCommands.Add(m_pUndoGroup);
	}
	m_pUndoGroup = NULL;
}


void CXTPTrackUndoManager::AddUndoCommand(CXTPTrackUndoCommand* pCommand)
{
	if (m_pUndoGroup)
	{
		m_pUndoGroup->Add(pCommand);
	}
	else if (m_nUndoContext != CONTEXT_UNDO)
	{
		m_arrUndoCommands.Add(pCommand);
	}
	else
	{
		m_arrRedoCommands.Add(pCommand);
	}

	if (m_nUndoContext == CONTEXT_NONE)
	{
		for (int i = 0; i < m_arrRedoCommands.GetSize(); i++)
		{
			delete m_arrRedoCommands[i];
		}
		m_arrRedoCommands.RemoveAll();
	}
}


BOOL CXTPTrackUndoManager::CanUndo() const
{
	return m_arrUndoCommands.GetSize() > 0;
}

BOOL CXTPTrackUndoManager::CanRedo() const
{
	return m_arrRedoCommands.GetSize() > 0;
}

void CXTPTrackUndoManager::Undo()
{
	if (m_arrUndoCommands.GetSize() > 0)
	{
		m_nUndoContext = 1;

		CXTPTrackUndoCommand* pUndoCommand = m_arrUndoCommands[m_arrUndoCommands.GetSize() - 1];

		pUndoCommand->Undo();
		delete pUndoCommand;

		m_arrUndoCommands.RemoveAt(m_arrUndoCommands.GetSize() - 1);

		m_nUndoContext = 0;
	}
}

void CXTPTrackUndoManager::Redo()
{
	if (m_arrRedoCommands.GetSize() > 0)
	{
		m_nUndoContext = 2;

		CXTPTrackUndoCommand* pUndoCommand = m_arrRedoCommands[m_arrRedoCommands.GetSize() - 1];

		pUndoCommand->Undo();
		delete pUndoCommand;

		m_arrRedoCommands.RemoveAt(m_arrRedoCommands.GetSize() - 1);

		m_nUndoContext = 0;
	}
}


//////////////////////////////////////////////////////////////////////////
// CXTPTrackUndoGroupCommand

CXTPTrackUndoGroupCommand::CXTPTrackUndoGroupCommand(CXTPTrackUndoManager* pManager)
: CXTPTrackUndoCommand(XTP_ID_TRACKUNDO_GROUP)
{
	m_pManager = pManager;
}

CXTPTrackUndoGroupCommand::~CXTPTrackUndoGroupCommand()
{
	for (int i = (int)m_arrUndoCommands.GetSize() - 1; i >= 0; i--)
	{
		delete m_arrUndoCommands[i];
	}
	m_arrUndoCommands.RemoveAll();
}

void CXTPTrackUndoGroupCommand::Add(CXTPTrackUndoCommand* pCommand)
{
	m_arrUndoCommands.Add(pCommand);
}

void CXTPTrackUndoGroupCommand::Undo()
{
	m_pManager->StartGroup();

	for (int i = (int)m_arrUndoCommands.GetSize() - 1; i >= 0; i--)
	{
		m_arrUndoCommands[i]->Undo();
		delete m_arrUndoCommands[i];
	}
	m_arrUndoCommands.RemoveAll();

	m_pManager->EndGroup();
}

int CXTPTrackUndoGroupCommand::GetCount() const
{
	return (int)m_arrUndoCommands.GetSize();
}



//////////////////////////////////////////////////////////////////////////
// CXTPTrackUndoAddMarkerCommand

CXTPTrackUndoAddMarkerCommand::CXTPTrackUndoAddMarkerCommand(CXTPTrackMarker* pMarker)
	: CXTPTrackUndoCommand(XTP_ID_TRACKUNDO_ADDMARKER)
{
	m_pMarker = pMarker;
	m_pMarker->InternalAddRef();
}

CXTPTrackUndoAddMarkerCommand::~CXTPTrackUndoAddMarkerCommand()
{
	m_pMarker->InternalRelease();
}

void CXTPTrackUndoAddMarkerCommand::Undo()
{
	m_pMarker->Remove();
}

//////////////////////////////////////////////////////////////////////////
// CXTPTrackUndoDeleteMarkerCommand

CXTPTrackUndoDeleteMarkerCommand::CXTPTrackUndoDeleteMarkerCommand(CXTPTrackMarker* pMarker)
	: CXTPTrackUndoCommand(XTP_ID_TRACKUNDO_DELETEMARKER)
{
	m_pMarker = pMarker;
	m_pMarker->InternalAddRef();
}

CXTPTrackUndoDeleteMarkerCommand::~CXTPTrackUndoDeleteMarkerCommand()
{
	m_pMarker->InternalRelease();
}

void CXTPTrackUndoDeleteMarkerCommand::Undo()
{
	m_pMarker->InternalAddRef();
	m_pMarker->GetControl()->GetMarkers()->Add(m_pMarker);

}


///////////////////////////////////////////////////////////////////////////
// CXTPTrackUndoSetMarkerPositionCommand

CXTPTrackUndoSetMarkerPositionCommand::CXTPTrackUndoSetMarkerPositionCommand(CXTPTrackMarker* pMarker, int nOldPosition)
	: CXTPTrackUndoCommand(XTP_ID_TRACKUNDO_SETMARKERPOSITION)
{
	m_nOldPosition = nOldPosition;

	m_pMarker = pMarker;
	m_pMarker->InternalAddRef();
}

CXTPTrackUndoSetMarkerPositionCommand::~CXTPTrackUndoSetMarkerPositionCommand()
{
	m_pMarker->InternalRelease();
}

void CXTPTrackUndoSetMarkerPositionCommand::Undo()
{
	m_pMarker->SetPosition(m_nOldPosition);
}




//////////////////////////////////////////////////////////////////////////
// CXTPTrackUndoAddBlockCommand

CXTPTrackUndoAddBlockCommand::CXTPTrackUndoAddBlockCommand(CXTPTrackBlock* pBlock)
: CXTPTrackUndoCommand(XTP_ID_TRACKUNDO_ADDBLOCK)
{
	m_pBlock = pBlock;
	m_pBlock->InternalAddRef();
}

CXTPTrackUndoAddBlockCommand::~CXTPTrackUndoAddBlockCommand()
{
	m_pBlock->InternalRelease();
}

void CXTPTrackUndoAddBlockCommand::Undo()
{
	m_pBlock->Remove();
}

//////////////////////////////////////////////////////////////////////////
// CXTPTrackUndoDeleteBlockCommand

CXTPTrackUndoDeleteBlockCommand::CXTPTrackUndoDeleteBlockCommand(CXTPTrackBlock* pBlock)
	: CXTPTrackUndoCommand(XTP_ID_TRACKUNDO_DELETEBLOCK)
{
	m_pBlock = pBlock;
	m_pBlock->InternalAddRef();

	m_pOldItem = pBlock->GetItem();
	m_pOldItem->InternalAddRef();
}

CXTPTrackUndoDeleteBlockCommand::~CXTPTrackUndoDeleteBlockCommand()
{
	m_pBlock->InternalRelease();
	m_pOldItem->InternalRelease();
}

void CXTPTrackUndoDeleteBlockCommand::Undo()
{
	m_pBlock->InternalAddRef();

	m_pOldItem->Add(m_pBlock);
}

///////////////////////////////////////////////////////////////////////////
// CXTPTrackUndoSetMarkerPositionCommand

CXTPTrackUndoSetBlockPositionCommand::CXTPTrackUndoSetBlockPositionCommand(CXTPTrackBlock* pBlock, int nOldPosition, int nOldLength)
: CXTPTrackUndoCommand(XTP_ID_TRACKUNDO_SETBLOCKPOSITION)
{
	m_nOldPosition = nOldPosition;
	m_nOldLength = nOldLength;

	m_pBlock = pBlock;
	m_pBlock->InternalAddRef();
}

CXTPTrackUndoSetBlockPositionCommand::~CXTPTrackUndoSetBlockPositionCommand()
{
	m_pBlock->InternalRelease();
}

void CXTPTrackUndoSetBlockPositionCommand::Undo()
{
	m_pBlock->SetPosition(m_nOldPosition);
	m_pBlock->SetLength(m_nOldLength);
	m_pBlock->GetItem()->RecalcLayout();
}



#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPTrackUndoManager, CCmdTarget)
	DISP_FUNCTION_ID(CXTPTrackUndoManager, "Undo", 1000, Undo, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CXTPTrackUndoManager, "Redo", 1001, Redo, VT_EMPTY, VTS_NONE)
	DISP_PROPERTY_EX_ID(CXTPTrackUndoManager, "CanUndo", 1002, CanUndo, SetNotSupported, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPTrackUndoManager, "CanRedo", 1003, CanRedo, SetNotSupported, VT_BOOL)
	DISP_FUNCTION_ID(CXTPTrackUndoManager, "Clear", 1004, Clear, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CXTPTrackUndoManager, "StartGroup", 1015, StartGroup, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CXTPTrackUndoManager, "EndGroup", 1016, EndGroup, VT_EMPTY, VTS_NONE)
END_DISPATCH_MAP()

// {ABC09CE5-E015-4f87-885D-DE3326A63BEA}
static const GUID IID_ITrackUndoManager =
{ 0xabc09ce5, 0xe015, 0x4f87, { 0x88, 0x5d, 0xde, 0x33, 0x26, 0xa6, 0x3b, 0xea } };

BEGIN_INTERFACE_MAP(CXTPTrackUndoManager, CCmdTarget)
INTERFACE_PART(CXTPTrackUndoManager, IID_ITrackUndoManager, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPTrackUndoManager, IID_ITrackUndoManager)

#endif
