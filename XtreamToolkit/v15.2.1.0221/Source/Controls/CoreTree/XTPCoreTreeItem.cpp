// XTPCoreTreeItem.cpp: implementation of the CXTPCoreTreeItem class.
//
// This file is a part of the XTREME CONTROLS MFC class library.
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

#include "Common/XTPPropExchange.h"
#include "Common/XTPImageManager.h"
#include "Common/XTPMarkupRender.h"

#include "XTPCoreTreeItem.h"
#include "XTPCoreTreeControl.h"
#include "XTPCoreTreePaintManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(CXTPCoreTreeItem, CObject, VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT);

//////////////////////////////////////////////////////////////////////
// CXTPCoreTreeItem

CXTPCoreTreeItem::CXTPCoreTreeItem()
{
	m_pChildren = NULL;

	m_pNextItem = NULL;
	m_pPrevItem = NULL;
	m_pParentItems = NULL;

	m_pControl = NULL;

	m_rcItem.SetRectEmpty();

	m_bLabel = FALSE;
	m_bHasCheckBox = FALSE;
	m_bExpanded = FALSE;
	m_bEnabled = TRUE;

	m_nChecked = 0;
	m_nIconId = -1;
	m_dwData = 0;
	m_dwRef = 1;

	m_pMarkupUIElement = NULL;
}

CXTPCoreTreeItem::~CXTPCoreTreeItem()
{
	SAFE_DELETE(m_pChildren);
	XTPMarkupReleaseElement(m_pMarkupUIElement);
}


void CXTPCoreTreeItem::DoPropExchange(CXTPPropExchange* pPX)
{
	PX_String(pPX, _T("Caption"), m_strCaption, _T(""));
	PX_Int(pPX, _T("IconIndex"), m_nIconId, -1);
	PX_Bool(pPX, _T("Enabled"), m_bEnabled, TRUE);
	PX_Bool(pPX, _T("IsLabel"), m_bLabel, FALSE);
	PX_Bool(pPX, _T("HasCheckBox"), m_bHasCheckBox, FALSE);
	PX_Int(pPX, _T("Checked"), m_nChecked, 0);

	ULONGLONG dwData = m_dwData;
	PX_UI8(pPX, _T("Data"), dwData, 0);
	if (pPX->IsLoading())
	{
		m_dwData = (DWORD_PTR)dwData;
	}

}

DWORD CXTPCoreTreeItem::AddRef()
{
	return InterlockedIncrement(&m_dwRef);
}

DWORD CXTPCoreTreeItem::Release()
{
	if (m_dwRef == 0)
		return 0;

	LONG lResult = InterlockedDecrement(&m_dwRef);
	if (lResult == 0)
	{
		delete this;
	}
	return lResult;
}

void CXTPCoreTreeItem::SetCaption(LPCTSTR lpszCaption)
{
	CString strCaption(lpszCaption);

	if (m_strCaption != strCaption)
	{
		m_strCaption = strCaption;

		XTPMarkupReleaseElement(m_pMarkupUIElement);

		if (m_pControl && m_pControl->GetMarkupContext())
		{
			m_pMarkupUIElement = XTPMarkupParseText(m_pControl->GetMarkupContext(), lpszCaption);

		}
		Invalidate();
	}
}

CSize CXTPCoreTreeItem::CalcSize(CDC* /*pDC*/, int /*nWidth*/) const
{
	int nHeight = m_pControl->GetPaintManager()->GetItemHeight();

	return CSize(0, nHeight);
}

void CXTPCoreTreeItem::SetRect(CRect rc)
{
	m_rcItem = rc;
}

BOOL CXTPCoreTreeItem::HasChildren() const
{
	return m_pChildren && m_pChildren->GetCount() != 0;
}

CXTPCoreTreeItems* CXTPCoreTreeItem::GetChildren() const
{
	if (!m_pChildren)
	{
		m_pChildren = new CXTPCoreTreeItems();

		ASSERT(m_pControl);
		m_pChildren->m_pControl = m_pControl;
		m_pChildren->m_pOwnerItem = (CXTPCoreTreeItem*)this;
	}

	return m_pChildren;
}

void CXTPCoreTreeItem::Draw(CDC* pDC)
{
	m_pControl->GetPaintManager()->DrawItem(pDC, this);
}

void CXTPCoreTreeItem::SetExpanded(BOOL bExpanded)
{
	if (m_bExpanded != bExpanded)
	{
		m_bExpanded = bExpanded;

		if (m_pControl && HasChildren())
			m_pControl->RecalcLayout();
	}
}

void CXTPCoreTreeItem::SetEnabled(BOOL bEnabled)
{
	m_bEnabled = bEnabled;
	Invalidate();
}

void CXTPCoreTreeItem::Invalidate()
{
	if (m_rcItem.IsRectEmpty())
		return;

	if (m_pControl)
	{
		CRect rcItem(m_rcItem);
		rcItem.OffsetRect(0, -m_pControl->GetScrollOffset());

		m_pControl->RedrawControl(rcItem);
	}
}

BOOL CXTPCoreTreeItem::IsExpanded() const
{
	return m_bExpanded;
}

BOOL CXTPCoreTreeItem::IsSelected() const
{
	return m_pControl->GetFocusedItem() == this;
}

CXTPCoreTreeItem* CXTPCoreTreeItem::GetNextSiblingItem() const
{
	return m_pNextItem;
}

CXTPCoreTreeItem* CXTPCoreTreeItem::GetPrevSiblingItem() const
{
	return m_pPrevItem;
}

CXTPCoreTreeItem* CXTPCoreTreeItem::GetParentItem() const
{
	return m_pParentItems->m_pOwnerItem;
}

CXTPCoreTreeItem* CXTPCoreTreeItem::GetFirstChildItem() const
{
	if (m_pChildren)
	{
		return m_pChildren->GetFirstItem();
	}

	return NULL;
}

CRect CXTPCoreTreeItem::GetRect() const
{
	return m_rcItem;
}

CRect CXTPCoreTreeItem::GetScreenRect() const
{
	CRect rcItem = m_rcItem;

	if (m_pControl) rcItem.OffsetRect(0, -m_pControl->GetScrollOffset());

	return rcItem;
}

CXTPImageManagerIcon* CXTPCoreTreeItem::GetImage() const
{
	if (m_nIconId == -1)
		return NULL;

	return m_pControl->GetImageManager()->GetImage(m_nIconId);
}

CRect CXTPCoreTreeItem::GetButtonRect() const
{
	CXTPCoreTreePaintManager* pPaintManager = m_pControl->GetPaintManager();

	if (!pPaintManager->m_bShowButtons)
	{
		return CRect(0, 0, 0, 0);
	}

	CRect rc(m_rcItem);
	rc.right = rc.left + pPaintManager->GetItemHeight();

	return rc;
}

CRect CXTPCoreTreeItem::GetCheckboxRect() const
{
	if (!IsCheckboxVisible())
		return CRect(0, 0, 0, 0);

	CXTPCoreTreePaintManager* pPaintManager = m_pControl->GetPaintManager();

	int nHeight = pPaintManager->GetItemHeight();

	CRect rc(m_rcItem);

	if (pPaintManager->m_bShowButtons)
	{
		rc.left += nHeight;
	}
	rc.right = rc.left + nHeight;

	return rc;
}

BOOL CXTPCoreTreeItem::IsParent(CXTPCoreTreeItem* pItem) const
{
	while (pItem != NULL)
	{
		if (pItem == (CXTPCoreTreeItem*)this)
			return TRUE;

		pItem = pItem->GetParentItem();
	}

	return FALSE;
}

void CXTPCoreTreeItem::Remove()
{
	m_pParentItems->RemoveItem(this);

}


BOOL CXTPCoreTreeItem::PrepareDrag (COleDataSource& srcItem)
{
	HGLOBAL hGlobal = CacheGlobalData();

	if (!hGlobal)
		return FALSE;

	srcItem.CacheGlobalData (CXTPCoreTreeControl::GetClipboardFormat(), hGlobal);

	return TRUE;
}

HGLOBAL CXTPCoreTreeItem::CacheGlobalData()
{
	try
	{
		CSharedFile sharedFile;
		CArchive ar (&sharedFile, CArchive::store);

		CRuntimeClass* pClass = GetRuntimeClass ();
		ASSERT (pClass != NULL);

		ar.WriteClass (pClass);

		CXTPPropExchangeArchive px(ar);
		DoPropExchange(&px);

		ar.Close();

		return sharedFile.Detach();
	}
	catch (COleException* pEx)
	{
		TRACE(_T("CXTPTaskPanelItem::PrepareDrag. OLE exception: %x\r\n"),
			pEx->m_sc);
		pEx->Delete ();
	}
	catch (CArchiveException* pEx)
	{
		TRACE(_T("CXTPTaskPanelItem::PrepareDrag. Archive exception\r\n"));
		pEx->Delete ();
	}

	return NULL;
}

CXTPCoreTreeItem* CXTPCoreTreeItem::CreateFromOleFile(CFile* pFile)
{
	ASSERT (pFile != NULL);

	CXTPCoreTreeItem* pItem = NULL;

	try
	{
		CArchive ar (pFile, CArchive::load);

		CRuntimeClass* pClass = ar.ReadClass ();

		if (pClass != NULL)
		{
			pItem = (CXTPCoreTreeItem*) pClass->CreateObject();

			if (pItem != NULL)
			{
				CXTPPropExchangeArchive px(ar);
				pItem->DoPropExchange(&px);
			}
		}

		ar.Close ();
		return pItem;
	}
	catch (COleException* pEx)
	{
		TRACE(_T("CXTPTaskPanelItem::CreateFromOleData. OLE exception: %x\r\n"),
			pEx->m_sc);
		pEx->Delete ();
	}
	catch (CArchiveException* pEx)
	{
		TRACE(_T("CXTPTaskPanelItem::CreateFromOleData. Archive exception\r\n"));
		pEx->Delete ();
	}
	catch (CNotSupportedException *pEx)
	{
		TRACE(_T("CXTPTaskPanelItem::CreateFromOleData. \"Not Supported\" exception\r\n"));
		pEx->Delete ();
	}

	if (pItem != NULL)
	{
		delete pItem;
	}

	return NULL;
}



CXTPCoreTreeItem* CXTPCoreTreeItem::CreateFromOleData(COleDataObject* pDataObject)
{
	ASSERT (pDataObject != NULL);
	if (!pDataObject)
		return NULL;

	CFile* pFile = pDataObject->GetFileData(CXTPCoreTreeControl::GetClipboardFormat());
	if (!pFile)
		return NULL;

	CXTPCoreTreeItem* pItem = CreateFromOleFile(pFile);

	delete pFile;

	return pItem;

}

//////////////////////////////////////////////////////////////////////////
// CXTPCoreTreeItems

CXTPCoreTreeItems::CXTPCoreTreeItems()
{
	m_pFirstItem = NULL;
	m_pLastItem = NULL;
	m_nCount = 0;
	m_pControl = NULL;
	m_pOwnerItem = NULL;
}

CXTPCoreTreeItems::~CXTPCoreTreeItems()
{
	m_pControl = NULL;
	RemoveAll();
}

void CXTPCoreTreeItems::RemoveAll()
{
	for (CXTPCoreTreeItem* pItem = m_pFirstItem; pItem != NULL;)
	{
		CXTPCoreTreeItem* pNextItem = pItem->m_pNextItem;

		if (m_pControl)
		{
			m_pControl->OnItemRemoved(pItem);
		}

		pItem->m_pControl = NULL;
		pItem->m_pParentItems = NULL;

		SAFE_RELEASE(pItem);

		pItem = pNextItem;
	}

	m_pFirstItem = m_pLastItem = NULL;
	m_nCount = 0;

	if (m_pControl)
	{
		m_pControl->RecalcLayout();
	}
}



void CXTPCoreTreeItems::RemoveItem(CXTPCoreTreeItem* pItem)
{
	if (!pItem || pItem->m_pParentItems != this)
		return;

	if (pItem->m_pPrevItem)
	{
		pItem->m_pPrevItem->m_pNextItem = pItem->m_pNextItem;

	}

	if (pItem->m_pNextItem)
	{
		pItem->m_pNextItem->m_pPrevItem = pItem->m_pPrevItem;
	}


	if (m_pLastItem == pItem)
	{
		m_pLastItem = pItem->m_pPrevItem;
	}

	if (m_pFirstItem == pItem)
	{
		m_pFirstItem = pItem->m_pNextItem;
	}

	m_nCount--;

	if (m_pControl)
	{
		m_pControl->OnItemRemoved(pItem);
	}

	pItem->m_pControl = NULL;
	pItem->m_pParentItems = NULL;

	SAFE_RELEASE(pItem);

	if (m_pControl)
	{
		m_pControl->RecalcLayout();
	}
}


CXTPCoreTreeItem* CXTPCoreTreeItems::AddItem(LPCTSTR lpszCaption)
{
	CXTPCoreTreeItem* pItem = new CXTPCoreTreeItem();

	AddItemAfter(pItem, m_pLastItem);

	pItem->SetCaption(lpszCaption);

	return pItem;
}

CXTPCoreTreeItem* CXTPCoreTreeItems::AddItemAfter(CXTPCoreTreeItem* pItem, CXTPCoreTreeItem* pPrevItem)
{
	if (!pItem)
		return NULL;

	ASSERT(pItem->m_pParentItems == NULL);

	if (pPrevItem == NULL)
		pPrevItem = m_pLastItem;

	CXTPCoreTreeItem* pNextItem = pPrevItem ? pPrevItem->m_pNextItem : NULL;

	if (pPrevItem)
	{
		pPrevItem->m_pNextItem = pItem;
	}

	if (pNextItem)
	{
		pNextItem->m_pPrevItem = pItem;
	}

	pItem->m_pPrevItem = pPrevItem;
	pItem->m_pNextItem = pNextItem;

	if (m_pLastItem == pPrevItem)
	{
		m_pLastItem = pItem;
	}

	if (m_pFirstItem == NULL)
	{
		m_pFirstItem = pItem;
	}

	ASSERT(m_pControl);
	pItem->m_pControl = m_pControl;
	pItem->m_pParentItems = this;
	m_nCount++;

	m_pControl->RecalcLayout();

	return pItem;
}



CXTPCoreTreeItem* CXTPCoreTreeItems::AddItemBefore(CXTPCoreTreeItem* pItem, CXTPCoreTreeItem* pNextItem)
{
	if (!pItem)
		return NULL;

	ASSERT(pItem->m_pParentItems == NULL);

	if (pNextItem == NULL)
		pNextItem = m_pFirstItem;

	CXTPCoreTreeItem* pPrevItem = pNextItem ? pNextItem->m_pPrevItem : NULL;

	if (pPrevItem)
	{
		pPrevItem->m_pNextItem = pItem;
	}

	if (pNextItem)
	{
		pNextItem->m_pPrevItem = pItem;
	}

	pItem->m_pPrevItem = pPrevItem;
	pItem->m_pNextItem = pNextItem;

	if (m_pLastItem == NULL)
	{
		m_pLastItem = pItem;
	}

	if (m_pFirstItem == pNextItem)
	{
		m_pFirstItem = pItem;
	}

	ASSERT(m_pControl);
	pItem->m_pControl = m_pControl;
	pItem->m_pParentItems = this;
	m_nCount++;

	m_pControl->RecalcLayout();

	return pItem;
}

void CXTPCoreTreeItems::MoveItemAfter(CXTPCoreTreeItem* pItem, CXTPCoreTreeItem* pPrevItem)
{
	ASSERT(pItem->GetParentItems());

	pItem->AddRef();
	pItem->Remove();

	AddItemAfter(pItem, pPrevItem);
}

void CXTPCoreTreeItems::MoveItemBefore(CXTPCoreTreeItem* pItem, CXTPCoreTreeItem* pPrevItem)
{
	ASSERT(pItem->GetParentItems());

	pItem->AddRef();
	pItem->Remove();

	AddItemBefore(pItem, pPrevItem);
}


POSITION CXTPCoreTreeItems::GetHeadPosition() const
{
	return (POSITION)m_pFirstItem;
}

CXTPCoreTreeItem* CXTPCoreTreeItems::GetNext(POSITION& rPosition) const
{
	CXTPCoreTreeItem* pItem = (CXTPCoreTreeItem*)rPosition;

	rPosition = (POSITION)pItem->m_pNextItem;

	return pItem;
}

CXTPCoreTreeItem* CXTPCoreTreeItems::GetNextItem(const CXTPCoreTreeItem* pItem) const
{
	if (!pItem)
		return NULL;

	return pItem->m_pNextItem;
}

CXTPCoreTreeItem* CXTPCoreTreeItems::GetPrevItem(const CXTPCoreTreeItem* pItem) const
{
	if (!pItem)
		return NULL;

	return pItem->m_pPrevItem;
}
