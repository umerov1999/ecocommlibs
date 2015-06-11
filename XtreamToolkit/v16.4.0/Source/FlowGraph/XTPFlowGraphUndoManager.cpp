// XTPFlowGraphUndoManager.cpp : implementation of the CXTPFlowGraphUndoManager class.
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

#include "StdAfx.h"

#include "Common/Resource.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPPropExchange.h"
#include "Common/XTPImageManager.h"
#include <Common/ScrollBar/XTPScrollBase.h>
#include <Common/ScrollBar/XTPScrollBarContainer.h>

#include "XTPFlowGraphElement.h"
#include "XTPFlowGraphControl.h"
#include "XTPFlowGraphNode.h"
#include "XTPFlowGraphNodes.h"
#include "XTPFlowGraphPage.h"
#include "XTPFlowGraphUndoManager.h"
#include "XTPFlowGraphConnection.h"
#include "XTPFlowGraphConnectionPoint.h"
#include "XTPFlowGraphConnectionPoints.h"
#include "XTPFlowGraphConnections.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int CONTEXT_NONE = 0;
const int CONTEXT_UNDO = 1;
const int CONTEXT_REDO = 2;

//////////////////////////////////////////////////////////////////////////
// CXTPFlowGraphUndoCommand

CXTPFlowGraphUndoCommand::CXTPFlowGraphUndoCommand(int nID)
	: m_nID(nID)
{

}

CXTPFlowGraphUndoCommand::~CXTPFlowGraphUndoCommand()
{
}

void CXTPFlowGraphUndoCommand::Undo()
{
	ASSERT(FALSE);
}


/////////////////////////////////////////////////////////////////////////
// CXTPFlowGraphUndoManager
CXTPFlowGraphUndoManager::CXTPFlowGraphUndoManager()
{
	m_nUndoContext = CONTEXT_NONE;

	m_pUndoGroup = NULL;
	m_bEnabled = TRUE;

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPFlowGraphUndoManager::~CXTPFlowGraphUndoManager()
{
	Clear();

	SAFE_DELETE(m_pUndoGroup);
}

void CXTPFlowGraphUndoManager::Clear()
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

void CXTPFlowGraphUndoManager::StartGroup()
{
	if (m_pUndoGroup != NULL)
	{
		m_arrGroupStack.Add(m_pUndoGroup);
	}

	m_pUndoGroup = new CXTPFlowGraphUndoGroupCommand(this);
}

void CXTPFlowGraphUndoManager::EndGroup()
{
	if (m_pUndoGroup->GetCount() == 0)
	{
		delete m_pUndoGroup;
	}
	else
	{
		if (m_arrGroupStack.GetSize() > 0)
			m_arrGroupStack[m_arrGroupStack.GetSize() - 1]->Add(m_pUndoGroup);
		else if (m_nUndoContext  != CONTEXT_UNDO)
			m_arrUndoCommands.Add(m_pUndoGroup);
		else
			m_arrRedoCommands.Add(m_pUndoGroup);
	}
	m_pUndoGroup = NULL;

	if (m_arrGroupStack.GetSize())
	{
		m_pUndoGroup = m_arrGroupStack[m_arrGroupStack.GetSize() - 1];
		m_arrGroupStack.RemoveAt(m_arrGroupStack.GetSize() - 1);
	}
}


void CXTPFlowGraphUndoManager::AddUndoCommand(CXTPFlowGraphUndoCommand* pCommand)
{
	if (!m_bEnabled)
	{
		delete pCommand;
		return;
	}

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


BOOL CXTPFlowGraphUndoManager::CanUndo() const
{
	return m_arrUndoCommands.GetSize() > 0;
}

BOOL CXTPFlowGraphUndoManager::CanRedo() const
{
	return m_arrRedoCommands.GetSize() > 0;
}

void CXTPFlowGraphUndoManager::Undo()
{
	if (m_arrUndoCommands.GetSize() > 0)
	{
		m_nUndoContext = 1;

		CXTPFlowGraphUndoCommand* pUndoCommand = m_arrUndoCommands[m_arrUndoCommands.GetSize() - 1];

		pUndoCommand->Undo();
		delete pUndoCommand;

		m_arrUndoCommands.RemoveAt(m_arrUndoCommands.GetSize() - 1);

		m_nUndoContext = 0;
	}
}

void CXTPFlowGraphUndoManager::Redo()
{
	if (m_arrRedoCommands.GetSize() > 0)
	{
		m_nUndoContext = 2;

		CXTPFlowGraphUndoCommand* pUndoCommand = m_arrRedoCommands[m_arrRedoCommands.GetSize() - 1];

		pUndoCommand->Undo();
		delete pUndoCommand;

		m_arrRedoCommands.RemoveAt(m_arrRedoCommands.GetSize() - 1);

		m_nUndoContext = 0;
	}
}


//////////////////////////////////////////////////////////////////////////
// CXTPFlowGraphUndoGroupCommand

CXTPFlowGraphUndoGroupCommand::CXTPFlowGraphUndoGroupCommand(CXTPFlowGraphUndoManager* pManager)
: CXTPFlowGraphUndoCommand(XTP_ID_FLOWGRAPHUNDO_GROUP)
{
	m_pManager = pManager;
}

CXTPFlowGraphUndoGroupCommand::~CXTPFlowGraphUndoGroupCommand()
{
	for (int i = (int)m_arrUndoCommands.GetSize() - 1; i >= 0; i--)
	{
		delete m_arrUndoCommands[i];
	}
	m_arrUndoCommands.RemoveAll();
}

void CXTPFlowGraphUndoGroupCommand::Add(CXTPFlowGraphUndoCommand* pCommand)
{
	m_arrUndoCommands.Add(pCommand);
}

void CXTPFlowGraphUndoGroupCommand::Undo()
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

int CXTPFlowGraphUndoGroupCommand::GetCount() const
{
	return (int)m_arrUndoCommands.GetSize();
}


//////////////////////////////////////////////////////////////////////////
// CXTPFlowGraphUndoAddNodeCommand

CXTPFlowGraphUndoAddNodeCommand::CXTPFlowGraphUndoAddNodeCommand(CXTPFlowGraphNode* pNode)
: CXTPFlowGraphUndoCommand(XTP_ID_FLOWGRAPHUNDO_ADDNODE)
{
	m_pNode = pNode;
	m_pNode->InternalAddRef();
}

CXTPFlowGraphUndoAddNodeCommand::~CXTPFlowGraphUndoAddNodeCommand()
{
	m_pNode->InternalRelease();
}

void CXTPFlowGraphUndoAddNodeCommand::Undo()
{
	m_pNode->Remove();
}

//////////////////////////////////////////////////////////////////////////
// CXTPFlowGraphUndoAddConnectionPointCommand

CXTPFlowGraphUndoAddConnectionPointCommand::CXTPFlowGraphUndoAddConnectionPointCommand(CXTPFlowGraphConnectionPoint* pConnectionPoint)
: CXTPFlowGraphUndoCommand(XTP_ID_FLOWGRAPHUNDO_ADDCONNECTIONPOINT)
{
	m_pConnectionPoint = pConnectionPoint;
	m_pConnectionPoint->InternalAddRef();
}

CXTPFlowGraphUndoAddConnectionPointCommand::~CXTPFlowGraphUndoAddConnectionPointCommand()
{
	m_pConnectionPoint->InternalRelease();
}

void CXTPFlowGraphUndoAddConnectionPointCommand::Undo()
{
	m_pConnectionPoint->Remove();
}

//////////////////////////////////////////////////////////////////////////
// CXTPFlowGraphUndoAddConnectionCommand

CXTPFlowGraphUndoAddConnectionCommand::CXTPFlowGraphUndoAddConnectionCommand(CXTPFlowGraphConnection* pConnection)
: CXTPFlowGraphUndoCommand(XTP_ID_FLOWGRAPHUNDO_ADDCONNECTION)
{
	m_pConnection = pConnection;
	m_pConnection->InternalAddRef();
}

CXTPFlowGraphUndoAddConnectionCommand::~CXTPFlowGraphUndoAddConnectionCommand()
{
	m_pConnection->InternalRelease();
}

void CXTPFlowGraphUndoAddConnectionCommand::Undo()
{
	m_pConnection->Remove();
}


//////////////////////////////////////////////////////////////////////////
// CXTPFlowGraphUndoDeleteNodeCommand

CXTPFlowGraphUndoDeleteNodeCommand::CXTPFlowGraphUndoDeleteNodeCommand(CXTPFlowGraphNode* pNode)
	: CXTPFlowGraphUndoCommand(XTP_ID_FLOWGRAPHUNDO_DELETENODE)
{
	m_pNode = pNode;
	m_pNode->InternalAddRef();

	m_pPage = pNode->GetPage();
}

CXTPFlowGraphUndoDeleteNodeCommand::~CXTPFlowGraphUndoDeleteNodeCommand()
{
	m_pNode->InternalRelease();
}

void CXTPFlowGraphUndoDeleteNodeCommand::Undo()
{
	m_pNode->InternalAddRef();

	m_pPage->GetNodes()->AddNode(m_pNode);
}


//////////////////////////////////////////////////////////////////////////
// CXTPFlowGraphUndoDeleteConnectionPointCommand

CXTPFlowGraphUndoDeleteConnectionPointCommand::CXTPFlowGraphUndoDeleteConnectionPointCommand(CXTPFlowGraphConnectionPoint* pConnectionPoint)
: CXTPFlowGraphUndoCommand(XTP_ID_FLOWGRAPHUNDO_DELETECONNECTIONPOINT)
{
	m_pConnectionPoint = pConnectionPoint;
	m_pConnectionPoint->InternalAddRef();

	m_pNode = m_pConnectionPoint->GetNode();
	m_pNode->InternalAddRef();

	m_nIndex = m_pConnectionPoint->m_nConnectionPointIndex;
}

CXTPFlowGraphUndoDeleteConnectionPointCommand::~CXTPFlowGraphUndoDeleteConnectionPointCommand()
{
	m_pNode->InternalRelease();
	m_pConnectionPoint->InternalRelease();
}

void CXTPFlowGraphUndoDeleteConnectionPointCommand::Undo()
{
	if (m_pNode->GetPage() != NULL)
	{
		// old code:
		//m_pNode->GetConnectionPoints()->AddConnectionPoint(m_pConnectionPoint, m_nIndex);
		//m_pConnectionPoint->InternalAddRef();

		XTPFlowGraphConnectionPointType type = m_pConnectionPoint->GetType();

		if (m_pConnectionPoint->m_bInputOutPair)
		{
			type = xtpFlowGraphPointInputAndOutput;
		}

		// new code:
		m_pNode->AddNamedConnectionPoints(m_pConnectionPoint->GetName(), type, m_pConnectionPoint->GetCaption());

		// hack:
		// but before that, in order to correctly complete CXTPFlowGraphUndoDeleteConnectionCommand::Undo
		// we need to set m_pConnectionPoint->m_pNode
		// in the old version the call
		// m_pNode->GetConnectionPoints()->AddConnectionPoint(m_pConnectionPoint, m_nIndex)
		// did that task for us, but now, it will be a new connection point (in terms of a pointer)

		m_pConnectionPoint->m_pNode = m_pNode;

		// this line is required because connection point pointer will change (after AddNamedConnectionPoints)
		// but for later undo/redo operations the index will stay the same
		m_pConnectionPoint->m_nConnectionPointIndex = m_nIndex;
	}
}


//////////////////////////////////////////////////////////////////////////
// CXTPFlowGraphUndoDeleteConnectionCommand

CXTPFlowGraphUndoDeleteConnectionCommand::CXTPFlowGraphUndoDeleteConnectionCommand(CXTPFlowGraphConnection* pConnection)
	: CXTPFlowGraphUndoCommand(XTP_ID_FLOWGRAPHUNDO_DELETECONNECTION)
{
	m_pConnection = pConnection;
	m_pConnection->InternalAddRef();

	m_pPage = pConnection->GetPage();

	m_pInputPoint = m_pConnection->GetInputPoint();
	if (m_pInputPoint) m_pInputPoint->InternalAddRef();

	m_pOutputPoint = m_pConnection->GetOutputPoint();
	if (m_pOutputPoint) m_pOutputPoint->InternalAddRef();
}

CXTPFlowGraphUndoDeleteConnectionCommand::~CXTPFlowGraphUndoDeleteConnectionCommand()
{
	m_pConnection->InternalRelease();

	if (m_pInputPoint) m_pInputPoint->InternalRelease();
	if (m_pOutputPoint) m_pOutputPoint->InternalRelease();
}

void CXTPFlowGraphUndoDeleteConnectionCommand::Undo()
{
	if (!(m_pInputPoint && m_pInputPoint->GetNode() && m_pInputPoint->GetNode()->GetPage() == m_pPage))
		 return;

	if (!(m_pOutputPoint && m_pOutputPoint->GetNode() && m_pOutputPoint->GetNode()->GetPage() == m_pPage))
		return;

	m_pConnection->InternalAddRef();

	// we will not use old pointers now, so we need to find new connection points, created by markup
	CXTPFlowGraphConnectionPoint *pInputNew =
		m_pInputPoint->GetNode()->GetConnectionPoints()->FindConnectionPoint(m_pInputPoint->GetName(), m_pInputPoint->GetType());

	CXTPFlowGraphConnectionPoint *pOutputNew =
		m_pOutputPoint->GetNode()->GetConnectionPoints()->FindConnectionPoint(m_pOutputPoint->GetName(), m_pOutputPoint->GetType());

	ASSERT(pInputNew && pOutputNew);

	if(pInputNew && pOutputNew)
	{
		m_pConnection->SetInputPoint(pInputNew);
		m_pConnection->SetOutputPoint(pOutputNew);

		m_pPage->GetConnections()->AddConnection(m_pConnection);
	}
}

///////////////////////////////////////////////////////////////////////////
// CXTPFlowGraphUndoSetNodePositionCommand

CXTPFlowGraphUndoSetNodePositionCommand::CXTPFlowGraphUndoSetNodePositionCommand(CXTPFlowGraphNode* pNode, CPoint ptOldPosition)
: CXTPFlowGraphUndoCommand(XTP_ID_FLOWGRAPHUNDO_SETNODEPOSITION)
{
	m_ptOldPosition = ptOldPosition;

	m_pNode = pNode;
	m_pNode->InternalAddRef();
}

CXTPFlowGraphUndoSetNodePositionCommand::~CXTPFlowGraphUndoSetNodePositionCommand()
{
	m_pNode->InternalRelease();
}

void CXTPFlowGraphUndoSetNodePositionCommand::Undo()
{
	m_pNode->SetLocation(m_ptOldPosition);
}

///////////////////////////////////////////////////////////////////////////
// CXTPFlowGraphUndoSetNodeSizeCommand

CXTPFlowGraphUndoSetNodeSizeCommand::CXTPFlowGraphUndoSetNodeSizeCommand(CXTPFlowGraphNode* pNode, CSize szOldSize)
: CXTPFlowGraphUndoCommand(XTP_ID_FLOWGRAPHUNDO_SETNODESIZE)
{
	m_szOldSize = szOldSize;

	m_pNode = pNode;
	m_pNode->InternalAddRef();
}

CXTPFlowGraphUndoSetNodeSizeCommand::~CXTPFlowGraphUndoSetNodeSizeCommand()
{
	m_pNode->InternalRelease();
}

void CXTPFlowGraphUndoSetNodeSizeCommand::Undo()
{
	m_pNode->SetSize(m_szOldSize);
}

///////////////////////////////////////////////////////////////////////////
// CXTPFlowGraphUndoSetNodePositionCommand

CXTPFlowGraphUndoSetNodeCaptionCommand::CXTPFlowGraphUndoSetNodeCaptionCommand(CXTPFlowGraphElement* pNode, CString strOldCaption)
	: CXTPFlowGraphUndoCommand(XTP_ID_FLOWGRAPHUNDO_SETNODECAPTION)
{
	m_strOldCaption = strOldCaption;

	m_pNode = pNode;
	m_pNode->InternalAddRef();
}

CXTPFlowGraphUndoSetNodeCaptionCommand::~CXTPFlowGraphUndoSetNodeCaptionCommand()
{
	m_pNode->InternalRelease();
}

void CXTPFlowGraphUndoSetNodeCaptionCommand::Undo()
{
	m_pNode->SetCaption(m_strOldCaption);
}

///////////////////////////////////////////////////////////////////////////
// CXTPFlowGraphUndoSetConnectionPositionCommand

CXTPFlowGraphUndoSetConnectionPositionCommand::CXTPFlowGraphUndoSetConnectionPositionCommand(CXTPFlowGraphConnection* pConnection, CPoint ptOldPosition)
: CXTPFlowGraphUndoCommand(XTP_ID_FLOWGRAPHUNDO_SETCONNECTIONPOSITION)
{
	m_ptOldPosition = ptOldPosition;

	m_pConnection = pConnection;
	m_pConnection->InternalAddRef();
}

CXTPFlowGraphUndoSetConnectionPositionCommand::~CXTPFlowGraphUndoSetConnectionPositionCommand()
{
	m_pConnection->InternalRelease();
}

void CXTPFlowGraphUndoSetConnectionPositionCommand::Undo()
{
	m_pConnection->SetControlPoint(m_ptOldPosition);
}




#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPFlowGraphUndoManager, CCmdTarget)
	DISP_FUNCTION_ID(CXTPFlowGraphUndoManager, "Undo", 1000, Undo, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CXTPFlowGraphUndoManager, "Redo", 1001, Redo, VT_EMPTY, VTS_NONE)
	DISP_PROPERTY_EX_ID(CXTPFlowGraphUndoManager, "CanUndo", 1002, CanUndo, SetNotSupported, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPFlowGraphUndoManager, "CanRedo", 1003, CanRedo, SetNotSupported, VT_BOOL)
	DISP_FUNCTION_ID(CXTPFlowGraphUndoManager, "Clear", 1004, Clear, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CXTPFlowGraphUndoManager, "StartGroup", 1015, StartGroup, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CXTPFlowGraphUndoManager, "EndGroup", 1016, EndGroup, VT_EMPTY, VTS_NONE)
END_DISPATCH_MAP()


// {10353C2D-B341-4329-9D52-97B0BF66B90E}
static const GUID IID_IFlowGraphUndoManager =
{ 0x10353c2d, 0xb341, 0x4329, { 0x9d, 0x52, 0x97, 0xb0, 0xbf, 0x66, 0xb9, 0xe } };


BEGIN_INTERFACE_MAP(CXTPFlowGraphUndoManager, CCmdTarget)
	INTERFACE_PART(CXTPFlowGraphUndoManager, IID_IFlowGraphUndoManager, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPFlowGraphUndoManager, IID_IFlowGraphUndoManager)

#endif
