// XTPTaskPanelItems.cpp : implementation of the CXTPTaskPanelItems class.
//
// This file is a part of the XTREME TASKPANEL MFC class library.
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

#include "Common/XTPPropExchange.h"
#include "Common/XTPSystemHelpers.h"
#include <Common/ScrollBar/XTPScrollBase.h>
#include <Common/ScrollBar/XTPScrollBarCtrl.h>
#include <Common/ScrollBar/XTPScrollBarContainer.h>

#include "XTPTaskPanelDefines.h"
#include "XTPTaskPanelItems.h"
#include "XTPTaskPanelItem.h"
#include "XTPTaskPanelGroup.h"
#include "XTPTaskPanelGroupItem.h"
#include "XTPTaskPanel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTPTaskPanelItems

IMPLEMENT_DYNCREATE(CXTPTaskPanelItems, CCmdTarget)

CXTPTaskPanelItems::CXTPTaskPanelItems(CXTPTaskPanel* pPanel)
{
	m_pPanel = pPanel;
	m_pOwner = NULL;

	EnableAutomation();

#ifdef _XTP_ACTIVEX
	EnableTypeLib();
#endif

}

CXTPTaskPanelItems::~CXTPTaskPanelItems()
{
	Clear(FALSE);
}

CXTPTaskPanelItem* CXTPTaskPanelItems::Add(CXTPTaskPanelItem* pItem, UINT nID)
{
	pItem->SetID(nID);

	m_arrItems.Add(pItem);

	pItem->m_pItems = this;
	pItem->m_pPanel = m_pPanel;
	pItem->OnInserted();

	m_pPanel->Reposition();

	return pItem;

}

CXTPTaskPanelItem* CXTPTaskPanelItems::InsertAt(int nIndex, CXTPTaskPanelItem* pItem)
{
	if (nIndex < 0 || nIndex > GetCount())
		nIndex = GetCount();

	m_arrItems.InsertAt(nIndex, pItem);

	pItem->m_pItems = this;
	pItem->m_pPanel = m_pPanel;
	pItem->OnInserted();

	m_pPanel->Reposition();

	return pItem;

}

CXTPTaskPanelItem* CXTPTaskPanelItems::Find(UINT nID) const
{
	for (int i = 0; i < m_arrItems.GetSize(); i++)
	{
		if (m_arrItems[i]->GetID() == nID)
			return m_arrItems[i];
	}
	return NULL;
}

int CXTPTaskPanelItems::GetIndex(const CXTPTaskPanelItem* pItem) const
{
	for (int i = 0; i < m_arrItems.GetSize(); i++)
	{
		if (m_arrItems[i] == pItem)
			return i;
	}
	return -1;
}

void CXTPTaskPanelItems::Move(CXTPTaskPanelItem* pItem, int nIndex)
{
	ASSERT(pItem->m_pItems == this);

	int nItemIndex = GetIndex(pItem);

	if ((nItemIndex == -1) || nItemIndex == nIndex)
		return;

	m_arrItems.RemoveAt(nItemIndex);

	m_arrItems.InsertAt(nIndex, pItem);

	m_pPanel->Reposition(FALSE);


}

void CXTPTaskPanelItems::Clear(BOOL bReposition)
{
	for (int i = 0; i < m_arrItems.GetSize(); i++)
	{
		CXTPTaskPanelItem* pItem = m_arrItems[i];

		SAFE_CALLPTR(m_pPanel, OnItemRemoved(pItem));
		pItem->OnRemoved();
		pItem->InternalRelease();
	}

	m_arrItems.RemoveAll();

	if (bReposition && m_pPanel)
	{
		m_pPanel->Reposition();
	}
}

void CXTPTaskPanelItems::Remove(CXTPTaskPanelItem* pItem)
{
	int nIndex = GetIndex(pItem);

	if (nIndex != -1)
	{
		RemoveAt(nIndex);
	}
}
void CXTPTaskPanelItems::RemoveAt(int nIndex)
{
	if (nIndex < 0 || nIndex >= m_arrItems.GetSize())
		return;

	CXTPTaskPanelItem* pItem = m_arrItems[nIndex];
	m_pPanel->OnItemRemoved(pItem);
	pItem->OnRemoved();

	pItem->InternalRelease();
	m_arrItems.RemoveAt(nIndex);

	m_pPanel->Reposition();
}

CXTPTaskPanelItem* CXTPTaskPanelItems::GetFirstVisibleItem() const
{
	return GetAt(GetNextVisibleIndex(-1, +1));
}

CXTPTaskPanelItem* CXTPTaskPanelItems::GetLastVisibleItem() const
{
	return GetAt(GetNextVisibleIndex(GetCount(), -1));
}

int CXTPTaskPanelItems::GetNextVisibleIndex(int nIndex, int nDirection) const
{
	CXTPTaskPanelItem* pItem = NULL;

	do
	{
		nIndex += nDirection;

		pItem = GetAt(nIndex);
		if (!pItem)
			return -1;

		if (pItem->IsVisible())
			return nIndex;
	}
	while (pItem);

	return -1;

}


void CXTPTaskPanelItems::DoPropExchange(CXTPPropExchange* pPX)
{
	m_pPanel->SetLockRedraw(TRUE);

	try
	{
		pPX->m_dwData = (DWORD_PTR)m_pPanel;

		CXTPPropExchangeEnumeratorPtr pEnumerator(pPX->GetEnumerator(_T("Item")));
		POSITION pos = pEnumerator->GetPosition(GetCount());

		if (pPX->IsStoring())
		{
			for (int i = 0; i < GetCount(); i++)
			{
				CXTPTaskPanelItem* pItem = GetAt(i);

				CXTPPropExchangeSection sec(pEnumerator->GetNext(pos));
				PX_Object(&sec, pItem, GetDefaultItemClass());
			}
		}
		else
		{
			Clear(FALSE);

			while (pos)
			{
				CXTPTaskPanelItem* pItem = NULL;

				CXTPPropExchangeSection sec(pEnumerator->GetNext(pos));
				PX_Object(&sec, pItem, GetDefaultItemClass());

				if (!pItem)
					AfxThrowArchiveException(CArchiveException::badClass);

				pItem->m_pItems = this;
				pItem->OnInserted();

				m_arrItems.Add(pItem);
			}
		}
	}
	catch (COleException* pEx)
	{
		TRACE(_T("CXTPTaskPanelItems::Serialize. OLE exception: %x\r\n"),
			pEx->m_sc);
		pEx->Delete ();
	}
	catch (CArchiveException* pEx)
	{
		TRACE(_T("CXTPTaskPanelItems::Serialize. Archive exception\r\n"));
		pEx->Delete ();
	}

	m_pPanel->SetLockRedraw(FALSE);

}

CRuntimeClass* CXTPTaskPanelItems::GetDefaultItemClass() const
{
	return RUNTIME_CLASS(CXTPTaskPanelItem);
}


//////////////////////////////////////////////////////////////////////////
// CXTPTaskPanelGroups

CXTPTaskPanelGroups::CXTPTaskPanelGroups(CXTPTaskPanel* pPanel)
	: CXTPTaskPanelItems(pPanel)
{
	EnableAutomation();
}

CRuntimeClass* CXTPTaskPanelGroups::GetDefaultItemClass() const
{
	return RUNTIME_CLASS(CXTPTaskPanelGroup);
}

//////////////////////////////////////////////////////////////////////////
// CXTPTaskPanelGroupItems

CXTPTaskPanelGroupItems::CXTPTaskPanelGroupItems(CXTPTaskPanel* pPanel)
	: CXTPTaskPanelItems(pPanel)
{
	EnableAutomation();
}


CRuntimeClass* CXTPTaskPanelGroupItems::GetDefaultItemClass() const
{
	return RUNTIME_CLASS(CXTPTaskPanelGroupItem);
}



#ifdef _XTP_ACTIVEX


BEGIN_DISPATCH_MAP(CXTPTaskPanelItems, CCmdTarget)
	DISP_FUNCTION_ID(CXTPTaskPanelItems, "Count", dispidCount, OleGetItemCount, VT_I4, VTS_NONE)
	DISP_FUNCTION_ID(CXTPTaskPanelItems, "Remove", dispidRemove, OleRemove, VT_EMPTY, VTS_I4)
	DISP_FUNCTION_ID(CXTPTaskPanelItems, "Clear", dispidClear, OleClear, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CXTPTaskPanelItems, "Find", dispidFind, OleFind, VT_DISPATCH, VTS_I4)
	DISP_FUNCTION_ID(CXTPTaskPanelItems, "Move", 8, OleMove, VT_EMPTY, VTS_DISPATCH VTS_I4)

	DISP_FUNCTION_ID(CXTPTaskPanelItems, "Item", DISPID_VALUE, OleItem, VT_DISPATCH, VTS_I4)
	DISP_FUNCTION_ID(CXTPTaskPanelItems, "_NewEnum", DISPID_NEWENUM, OleNewEnum, VT_UNKNOWN, VTS_NONE)

END_DISPATCH_MAP()

// {1D726654-84F6-418c-A952-33B09EA4C99C}
static const GUID IID_ITaskPanelItems =
{ 0x1d726654, 0x84f6, 0x418c, { 0xa9, 0x52, 0x33, 0xb0, 0x9e, 0xa4, 0xc9, 0x9c } };


BEGIN_INTERFACE_MAP(CXTPTaskPanelItems, CCmdTarget)
	INTERFACE_PART(CXTPTaskPanelItems, IID_ITaskPanelItems, Dispatch)
	//INTERFACE_PART(CXTPTaskPanelItems, IID_IEnumVARIANT, EnumVARIANT)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPTaskPanelItems, IID_ITaskPanelItems)


BEGIN_DISPATCH_MAP(CXTPTaskPanelGroups, CXTPTaskPanelItems)
	DISP_FUNCTION_ID(CXTPTaskPanelGroups, "Add", dispidAdd, OleAdd, VT_DISPATCH, VTS_I4 VTS_BSTR)
END_DISPATCH_MAP()

BEGIN_DISPATCH_MAP(CXTPTaskPanelGroupItems, CXTPTaskPanelItems)
	DISP_FUNCTION_ID(CXTPTaskPanelGroupItems, "Add", dispidAdd, OleAdd, VT_DISPATCH, VTS_I4 VTS_BSTR VTS_I4 VTS_VARIANT)
	DISP_FUNCTION_ID(CXTPTaskPanelGroupItems, "Insert", 6, OleInsert, VT_DISPATCH, VTS_I4 VTS_I4 VTS_BSTR VTS_I4 VTS_VARIANT)
END_DISPATCH_MAP()


// {7D726654-84F6-418c-A952-33B09EA4C99C}
static const GUID IID_ITaskPanelGroups =
{ 0x7d726654, 0x84f6, 0x418c, { 0xa9, 0x52, 0x33, 0xb0, 0x9e, 0xa4, 0xc9, 0x9c } };


BEGIN_INTERFACE_MAP(CXTPTaskPanelGroups, CXTPTaskPanelItems)
	INTERFACE_PART(CXTPTaskPanelGroups, IID_ITaskPanelGroups, Dispatch)
	//INTERFACE_PART(CXTPTaskPanelGroups, IID_IEnumVARIANT, EnumVARIANT)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPTaskPanelGroups, IID_ITaskPanelGroups)

// {E51E029F-9128-4258-84DD-AFCB00C2C0D4}
static const GUID IID_ITaskPanelGroupItems =
{ 0xe51e029f, 0x9128, 0x4258, { 0x84, 0xdd, 0xaf, 0xcb, 0x0, 0xc2, 0xc0, 0xd4 } };

BEGIN_INTERFACE_MAP(CXTPTaskPanelGroupItems, CXTPTaskPanelItems)
	INTERFACE_PART(CXTPTaskPanelGroupItems, IID_ITaskPanelGroupItems, Dispatch)
	//INTERFACE_PART(CXTPTaskPanelGroupItems, IID_IEnumVARIANT, EnumVARIANT)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPTaskPanelGroupItems, IID_ITaskPanelGroupItems)


LPDISPATCH CXTPTaskPanelGroups::OleAdd(long nID, LPCTSTR lpCaption)
{
	CXTPTaskPanelItem* pGroup = Add(new CXTPTaskPanelGroup(m_pPanel), nID);
	pGroup->SetCaption(lpCaption);

	return pGroup->GetIDispatch(TRUE);
}

LPDISPATCH CXTPTaskPanelGroupItems::OleAdd(long nID, LPCTSTR lpCaption, long nType, const VARIANT& varImageIndex)
{
	CXTPTaskPanel::CRepositionContext context(m_pPanel);

	CXTPTaskPanelItem* pItem = Add(new CXTPTaskPanelGroupItem(), nID);
	pItem->SetCaption(lpCaption);
	pItem->SetType((XTPTaskPanelItemType)nType);
	pItem->SetIconIndex(VariantToLong(&varImageIndex, -1));

	return pItem->GetIDispatch(TRUE);
}

LPDISPATCH CXTPTaskPanelGroupItems::OleInsert(long nIndex, long nID, LPCTSTR lpCaption, long nType, const VARIANT& varImageIndex)
{
	CXTPTaskPanel::CRepositionContext context(m_pPanel);

	CXTPTaskPanelItem* pItem = InsertAt(nIndex - 1, new CXTPTaskPanelGroupItem());
	pItem->SetID(nID);
	pItem->SetCaption(lpCaption);
	pItem->SetType((XTPTaskPanelItemType)nType);
	pItem->SetIconIndex(VariantToLong(&varImageIndex, -1));

	return pItem->GetIDispatch(TRUE);
}

void CXTPTaskPanelItems::OleMove(LPDISPATCH lpItem, long nIndex)
{
	Move(CXTPTaskPanelItem::FromDispatch(lpItem), nIndex - 1);
}

LPDISPATCH CXTPTaskPanelItems::OleFind(long nID)
{
	CXTPTaskPanelItem* pItem = Find(nID);
	return pItem ? pItem->GetIDispatch(TRUE) : NULL;
}

LPDISPATCH CXTPTaskPanelItems::OleGetItem(long nIndex)
{
	if (nIndex >= 0 && nIndex < GetCount())
	{
		CXTPTaskPanelItem* pItem = GetAt(nIndex);
		return pItem->GetIDispatch(TRUE);
	}
	AfxThrowOleException(E_INVALIDARG);
	return 0;
}

LPDISPATCH CXTPTaskPanelItems::OleItem(long nIndex)
{
	return OleGetItem(nIndex - 1);
}

void CXTPTaskPanelItems::OleClear()
{
	Clear();
}
void CXTPTaskPanelItems::OleRemove(long nIndex)
{
	if (nIndex > 0 && nIndex <= GetCount())
	{
		RemoveAt(nIndex - 1);
	}
	else
	{
		AfxThrowOleException(E_INVALIDARG);
	}
}

long CXTPTaskPanelItems::OleGetItemCount()
{
	return GetCount();
}

IMPLEMENT_ENUM_VARIANT(CXTPTaskPanelItems)


#endif
