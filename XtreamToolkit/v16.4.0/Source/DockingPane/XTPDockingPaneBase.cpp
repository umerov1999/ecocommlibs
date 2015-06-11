// XTPDockingPaneBase.cpp : implementation of the CXTPDockingPaneBase class.
//
// This file is a part of the XTREME DOCKINGPANE MFC class library.
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

#include "stdafx.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPSystemHelpers.h"

#include <TabManager/Includes.h>

#include "XTPDockingPaneDefines.h"
#include "XTPDockingPaneBase.h"
#include "XTPDockingPaneManager.h"
#include "XTPDockingPaneLayout.h"
#include "XTPDockingPanePaintManager.h"
#include <DockingPane/XTPDockingPaneCaptionButton.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//////////////////////////////////////////////////////////////////////////
// CXTPDockingPaneCaptionButtons

BOOL CXTPDockingPaneCaptionButtons::CheckForMouseOver(CPoint pt)
{
	BOOL bResult = FALSE;
	for (int i = 0; i < GetSize(); i++)
	{
		if (GetAt(i)->CheckForMouseOver(pt))
			bResult = TRUE;
	}
	return bResult;
}


//////////////////////////////////////////////////////////////////////////
// CXTPDockingPaneBase


CXTPDockingPaneBase::CXTPDockingPaneBase(XTPDockingPaneType type, CXTPDockingPaneLayout* pLayout)
	: m_type(type), m_pLayout(pLayout)
{
	m_pParentContainer = 0;
	m_pDockingSite = GetDockingPaneManager()->GetSite();
	m_szDocking = 0;
	m_rcWindow.SetRectEmpty();

	m_nIndex = 0;
	m_nLength = 0;

	pLayout->Push(this);

	m_pCaptionButtons = NULL;

}

CXTPDockingPaneBase::~CXTPDockingPaneBase()
{
	if (m_pCaptionButtons)
	{
		for (int i = 0; i < m_pCaptionButtons->GetSize(); i++)
			delete m_pCaptionButtons->GetAt(i);

		delete m_pCaptionButtons;
		m_pCaptionButtons = NULL;
	}
}

CXTPDockingPaneCaptionButton* CXTPDockingPaneBase::FindCaptionButton(int nID) const
{
	if (!m_pCaptionButtons)
		return NULL;

	for (int i = 0; i < m_pCaptionButtons->GetSize(); i++)
	{
		if (m_pCaptionButtons->GetAt(i)->GetID() == nID)
			return m_pCaptionButtons->GetAt(i);
	}

	ASSERT(FALSE);
	return NULL;
}

void CXTPDockingPaneBase::OnSizeParent(CWnd* /*pParent*/, CRect rect, LPVOID lParam)
{
	AFX_SIZEPARENTPARAMS* lpLayout = (AFX_SIZEPARENTPARAMS*)lParam;
	lpLayout->rect = m_rcWindow = rect;
}

void CXTPDockingPaneBase::RedrawPane()
{
	if (GetPaneHwnd())
		::InvalidateRect(GetPaneHwnd(), 0, FALSE);
}

void CXTPDockingPaneBase::OnFocusChanged()
{
}

BOOL CXTPDockingPaneBase::IsHidden() const
{
	return FALSE;
}

void CXTPDockingPaneBase::GetMinMaxInfo(LPMINMAXINFO pMinMaxInfo) const
{
	ZeroMemory(pMinMaxInfo, sizeof(MINMAXINFO));

	if (m_type == xtpPaneTypeClient && !GetDockingPaneManager()->IsClientHidden())
	{
		pMinMaxInfo->ptMinTrackSize = GetDockingPaneManager()->m_ptMinClientSize;
	}

	pMinMaxInfo->ptMaxTrackSize = CPoint(32000, 32000);
}

POSITION CXTPDockingPaneBase::ContainPane(CXTPDockingPaneBase* pPane) const
{
	return pPane == this ? (POSITION)TRUE : NULL;
}

CRect CXTPDockingPaneBase::GetPaneWindowRect() const
{
	ASSERT(m_pDockingSite);

	if (!m_pDockingSite || !m_pDockingSite->GetSafeHwnd())
		return m_rcWindow;

	CRect rc(m_rcWindow);
	m_pDockingSite->ClientToScreen(&rc);
	return rc;
}

void CXTPDockingPaneBase::OnChildContainerChanged(CXTPDockingPaneBase* /*pContainer*/)
{
}

void CXTPDockingPaneBase::OnParentContainerChanged(CXTPDockingPaneBase* /*pContainer*/)
{
}

void CXTPDockingPaneBase::InvalidatePane(BOOL bSelectionChanged)
{
	if (m_pParentContainer)
	{
		m_pParentContainer->InvalidatePane(bSelectionChanged);
	}
}


void CXTPDockingPaneBase::RemovePane(CXTPDockingPaneBase* /*pPane*/)
{
}

void CXTPDockingPaneBase::FindPane(XTPDockingPaneType type, CXTPDockingPaneBaseList* pList) const
{
	if (GetType() == type)
	{
		pList->AddTail((CXTPDockingPaneBase*)this);
	}
}

BOOL CXTPDockingPaneBase::IsEmpty() const
{
	return GetType() == xtpPaneTypeClient && GetDockingPaneManager()->m_bHideClient;
}

void CXTPDockingPaneBase::Copy(CXTPDockingPaneBase* pClone, CXTPPaneToPaneMap* /*pMap*/, DWORD /*dwIgnoredOptions*/)
{
	m_szDocking = pClone->m_szDocking;
}

CXTPDockingPaneBase* CXTPDockingPaneBase::Clone(CXTPDockingPaneLayout* pLayout, CXTPPaneToPaneMap* pMap, DWORD dwIgnoredOptions)
{
	CXTPDockingPaneBase* pPane = GetDockingPaneManager()->OnCreatePane(GetType(), pLayout);

	pPane->Copy(this, pMap, dwIgnoredOptions);

	SAFE_CALLPTR(pMap, SetAt(this, pPane));

	return pPane;
}

CXTPDockingPaneManager* CXTPDockingPaneBase::GetDockingPaneManager() const
{
	ASSERT(m_pLayout);
	if (!m_pLayout)
		return NULL;

	ASSERT(m_pLayout->m_pManager);
	return m_pLayout->m_pManager;
}

CXTPDockingPanePaintManager* CXTPDockingPaneBase::GetPaintManager() const
{
	CXTPDockingPaneManager* pManager = GetDockingPaneManager();
	ASSERT(pManager);
	if (!pManager)
		return NULL;

	ASSERT(pManager->m_pPaintManager);
	return pManager->m_pPaintManager;
}

void CXTPDockingPaneBase::CreateContainer()
{
	SAFE_CALLPTR(m_pParentContainer, CreateContainer());
}

//////////////////////////////////////////////////////////////////////////

CXTPDockingPaneClientContainer::CXTPDockingPaneClientContainer(CXTPDockingPaneLayout* pLayout)
	: CXTPDockingPaneBase(xtpPaneTypeClient, pLayout)
{
#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

void CXTPDockingPaneClientContainer::DeletePane()
{
	InternalRelease();
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPDockingPaneClientContainer, CCmdTarget)
	DISP_FUNCTION_ID(CXTPDockingPaneClientContainer, "Count", 1, OleGetItemCount, VT_I4, VTS_NONE)
	DISP_FUNCTION_ID(CXTPDockingPaneClientContainer, "Pane", DISPID_VALUE, OleGetPane, VT_DISPATCH, VTS_I4)
	DISP_PROPERTY_EX_ID(CXTPDockingPaneClientContainer, "ParentContainer", 20, OleGetContainer, SetNotSupported, VT_DISPATCH)
	DISP_PROPERTY_EX_ID(CXTPDockingPaneClientContainer, "Type", 21, OleGetType, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPDockingPaneClientContainer, "hWnd", DISPID_HWND, OleGetHwnd, SetNotSupported, VT_HANDLE)
	DISP_PROPERTY_EX_ID(CXTPDockingPaneClientContainer, "IsEmpty", 22, OleIsEmpty, SetNotSupported, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPDockingPaneClientContainer, "Position", 23, OleGetPosition, SetNotSupported, VT_I4)
END_DISPATCH_MAP()

// {ABC0C93B-61BC-4172-893C-CD3463CF2E01}
static const IID IID_IDockingPaneContainer =
{ 0xabc0c93b, 0x61bc, 0x4172, { 0x89, 0x3c, 0xcd, 0x34, 0x63, 0xcf, 0x2e, 0x1 } };

IMPLEMENT_OLETYPELIB_EX(CXTPDockingPaneClientContainer, IID_IDockingPaneContainer)


BEGIN_INTERFACE_MAP(CXTPDockingPaneClientContainer, CCmdTarget)
	INTERFACE_PART(CXTPDockingPaneClientContainer, IID_IDockingPaneContainer, Dispatch)
END_INTERFACE_MAP()


LPDISPATCH CXTPDockingPaneClientContainer::OleGetContainer()
{
	return m_pParentContainer ? m_pParentContainer->OleGetDispatch(TRUE) : NULL;
}

int CXTPDockingPaneClientContainer::OleGetType()
{
	return m_type;
}

int CXTPDockingPaneClientContainer::OleGetPosition()
{
	return (XTPDockingPaneDirection)-1;
}

BOOL CXTPDockingPaneClientContainer::OleIsEmpty()
{
	return IsEmpty();
}

int CXTPDockingPaneClientContainer::OleGetItemCount()
{
	return 0;
}

LPDISPATCH CXTPDockingPaneClientContainer::OleGetPane(int nIndex)
{
	return NULL;
}

HWND CXTPDockingPaneClientContainer::OleGetHwnd()
{
	return 0;
}

LPDISPATCH CXTPDockingPaneClientContainer::OleGetItem(int nIndex)
{
	return NULL;
}

LPDISPATCH CXTPDockingPaneClientContainer::OleGetDispatch(BOOL bAddRef)
{
	return GetIDispatch(bAddRef);
}

#endif
