// XTPReportRecord.cpp : implementation of the CXTPReportRecord class.
//
// This file is a part of the XTREME REPORTCONTROL MFC class library.
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
#include "Common/XTPCustomHeap.h"

#include "XTPReportDefines.h"
#include "XTPReportRecordItem.h"
#include "XTPReportColumn.h"
#include "XTPReportRecord.h"
#include "XTPReportRecords.h"
#include "ItemTypes/XTPReportRecordItemPreview.h"
#include "ItemTypes/XTPReportRecordItemVariant.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_SERIAL(CXTPReportRecord, CCmdTarget, VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)

CXTPReportRecord::CXTPReportRecord()
	: m_bVisible    (TRUE)
	, m_bLocked     (FALSE)
	, m_bExpanded   (FALSE)
	, m_bEditable   (TRUE)
	, m_bSelected   (FALSE)
	, m_pPreviewItem(NULL)
	, m_pControl    (NULL)
	, m_pRecords    (NULL)
	, m_pChildren   (NULL)
{



	m_nIndex = -1;



	m_vtBookmark.vt = VT_EMPTY;
//<<TC>>
	m_nFreeHeight = 0;
//<<TC>>
#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPReportRecord::~CXTPReportRecord()
{
	RemoveAll();

	if (m_pChildren)
		m_pChildren->InternalRelease();
}

void CXTPReportRecord::RemoveAll()
{
	for (int nItem = GetItemCount() - 1; nItem >= 0; nItem--)
	{
		CXTPReportRecordItem* pItem = m_arrItems.GetAt(nItem);
		if (pItem)
			pItem->InternalRelease();
	}
	m_arrItems.RemoveAll();

	CMDTARGET_RELEASE(m_pPreviewItem);
}

void CXTPReportRecord::Delete()
{
	ASSERT(m_pRecords);
	if (m_pRecords) m_pRecords->RemoveAt(m_nIndex);
}

BOOL CXTPReportRecord::HasChildren() const
{
	return m_pChildren && (m_pChildren->GetCount() > 0);
}

CXTPReportRecords* CXTPReportRecord::GetChilds()
{
	if (m_pChildren == NULL)
	{
		m_pChildren = new CXTPReportRecords(this);
	}

	if (GetRecords())
	{
		BOOL bCase = GetRecords()->IsCaseSensitive();
		m_pChildren->SetCaseSensitive(bCase);
	}
	return m_pChildren;
}

CXTPReportRecordItem* CXTPReportRecord::GetItem(CXTPReportColumn* pColumn) const
{
	if (this == NULL)
		return NULL;

	return GetItem(pColumn->GetItemIndex());
}

int CXTPReportRecord::IndexOf(const CXTPReportRecordItem* pItem) const
{
	for (int nItem = 0; nItem < GetItemCount(); nItem++)
	{
		if (GetItem(nItem) == pItem)
			return nItem;
	}
	return -1;
}

CXTPReportRecordItem* CXTPReportRecord::AddItem(CXTPReportRecordItem* pItem)
{
	m_arrItems.Add(pItem);
	pItem->m_pControl = this->m_pControl;
	pItem->m_pRecord  = this;
	return pItem;
}

CXTPReportRecordItemPreview* CXTPReportRecord::GetItemPreview() const
{
	return m_pPreviewItem;
}

void CXTPReportRecord::SetEditable(BOOL bEditable)
{
	m_bEditable = bEditable;
}

void CXTPReportRecord::SetPreviewItem(CXTPReportRecordItemPreview* pItemPreview)
{
	if (m_pPreviewItem)
		m_pPreviewItem->InternalRelease();
	m_pPreviewItem = pItemPreview;

	m_pPreviewItem->m_pRecord = this;
}

BOOL CXTPReportRecord::IsFiltered() const
{
	return FALSE;
}

int CXTPReportRecord::GetIndex() const
{
	return m_nIndex;
}

void CXTPReportRecord::DoPropExchange(CXTPPropExchange* pPX)
{
	PX_Bool(pPX, _T("Locked"), m_bLocked);
	PX_Bool(pPX, _T("Editable"), m_bEditable);

	BOOL bPreview = m_pPreviewItem != NULL;
	PX_Bool(pPX, _T("Preview"), bPreview);


	int nCount = GetItemCount();

	CXTPPropExchangeEnumeratorPtr pEnumItems(pPX->GetEnumerator(_T("Item")));

	if (pPX->IsStoring())
	{
		POSITION posItem = pEnumItems->GetPosition((DWORD)nCount);

		for (int i = 0; i < nCount; i++)
		{
			CXTPReportRecordItem* pItem = GetItem(i);
			ASSERT(pItem);
			if (!pItem)
				AfxThrowArchiveException(CArchiveException::badClass);

			CXTPPropExchangeSection secItem(pEnumItems->GetNext(posItem));
			PX_Object(&secItem, pItem, RUNTIME_CLASS(CXTPReportRecordItem));
		}
	}
	else
	{
		RemoveAll();

		POSITION posItem = pEnumItems->GetPosition();

		while (posItem)
		{
			CXTPReportRecordItem* pItem = NULL;

			CXTPPropExchangeSection sec(pEnumItems->GetNext(posItem));
			PX_Object(&sec, pItem, RUNTIME_CLASS(CXTPReportRecordItem));

			if (!pItem)
				AfxThrowArchiveException(CArchiveException::badClass);

			AddItem(pItem);
		}
	}

	//------------------------------------------------------------
	if (bPreview)
	{
		CXTPPropExchangeSection secPreviewItem(pPX->GetSection(_T("PreviewItem")));

		if (pPX->IsLoading())
		{
			CMDTARGET_RELEASE(m_pPreviewItem);
		}

		PX_Object(&secPreviewItem, m_pPreviewItem, RUNTIME_CLASS(CXTPReportRecordItemPreview));

		if (m_pPreviewItem && pPX->IsLoading())
		{
			m_pPreviewItem->m_pRecord = this;
		}
	}

	//------------------------------------------------------------
	if (pPX->GetSchema() > _XTP_SCHEMA_1041)
	{
		BOOL bHasChildren = HasChildren();
		PX_Bool(pPX, _T("HasChildren"), bHasChildren, FALSE);

		if (bHasChildren)
		{
			CXTPPropExchangeSection secChildren(pPX->GetSection(_T("Children")));
			GetChilds()->_DoPropExchange(&secChildren);
		}
		else if (m_pChildren)
		{
			m_pChildren->RemoveAll();
		}
	}
}

void CXTPReportRecord::TreeAddRef()
{
	InternalAddRef();
	if (HasChildren())
	{
		for (int nChild = 0; nChild < GetChilds()->GetCount(); nChild++)
		{
			GetChilds()->GetAt(nChild)->TreeAddRef();
		}
	}
}

void CXTPReportRecord::TreeRelease()
{
	if (HasChildren())
	{
		for (int nChild = 0; nChild < GetChilds()->GetCount(); nChild++)
		{
			GetChilds()->GetAt(nChild)->TreeRelease();
		}
	}
	InternalRelease();
}

CXTPMarkupContext* CXTPReportRecord::GetMarkupContext() const
{
	CXTPReportRecords* pRecords = m_pRecords;

	while (pRecords != NULL)
	{
		CXTPMarkupContext* pMarkupContext = pRecords->GetMarkupContext();
		if (pMarkupContext)
			return pMarkupContext;

		if (pRecords->m_pOwnerRecord)
			pRecords = pRecords->m_pOwnerRecord->GetRecords();
		else
			return NULL;
	}

	return NULL;
}

CXTPReportRecord* CXTPReportRecord::GetParentRecord() const
{
	if (m_pRecords && m_pRecords->GetOwnerRecord())
		return m_pRecords->GetOwnerRecord();

	return NULL;
}

void CXTPReportRecord::SetExpanded(BOOL bExpanded)
{
	m_bExpanded = bExpanded;
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPReportRecord, CCmdTarget)
	DISP_FUNCTION_ID(CXTPReportRecord, "Item", DISPID_VALUE, OleGetItem, VT_DISPATCH, VTS_I4)
	DISP_FUNCTION_ID(CXTPReportRecord, "_NewEnum", DISPID_NEWENUM, OleNewEnum, VT_UNKNOWN, VTS_NONE)
	DISP_FUNCTION_ID(CXTPReportRecord, "AddItem", dispidAdd, OleAdd, VT_DISPATCH, VTS_VARIANT)
	DISP_FUNCTION_ID(CXTPReportRecord, "DeleteAll", 5, RemoveAll, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CXTPReportRecord, "Childs", 6, OleGetChilds, VT_DISPATCH, VTS_NONE)
	DISP_PROPERTY_EX_ID(CXTPReportRecord, "PreviewText", 7, OleGetPreviewText, OleSetPreviewText, VT_BSTR)
	DISP_PROPERTY_ID(CXTPReportRecord, "Expanded", 8, m_bExpanded, VT_BOOL)
	DISP_PROPERTY_ID(CXTPReportRecord, "Tag", 9, m_oleTag, VT_VARIANT)
	DISP_PROPERTY_EX_ID(CXTPReportRecord, "Index", 10, GetIndex, SetNotSupported, VT_I4)
	DISP_PROPERTY_ID(CXTPReportRecord, "Visible", 11, m_bVisible, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPReportRecord, "PreviewItem", 12, OleGetPreviewItem, SetNotSupported, VT_DISPATCH)
	DISP_FUNCTION_ID(CXTPReportRecord, "ParentRecord", 13, OleGetParentRecord, VT_DISPATCH, VTS_NONE)
	DISP_FUNCTION_ID(CXTPReportRecord, "Records", 14, OleGetRecords, VT_DISPATCH, VTS_NONE)
	DISP_PROPERTY_EX_ID(CXTPReportRecord, "ItemCount", 15, OleGetItemCount, SetNotSupported, VT_I4)
	DISP_PROPERTY_ID(CXTPReportRecord, "Bookmark", 16, m_vtBookmark, VT_VARIANT)

	DISP_FUNCTION_ID(CXTPReportRecord, "AddItemEx", 17, OleAddEx, VT_EMPTY, VTS_DISPATCH)

END_DISPATCH_MAP()

// {8B97BA3A-FE51-4e7e-B3C0-A798DDD174E9}
static const GUID IID_IReportRecord =
{ 0x8b97ba3a, 0xfe51, 0x4e7e, { 0xb3, 0xc0, 0xa7, 0x98, 0xdd, 0xd1, 0x74, 0xe9 } };

BEGIN_INTERFACE_MAP(CXTPReportRecord, CCmdTarget)
	INTERFACE_PART(CXTPReportRecord, IID_IReportRecord, Dispatch)
	//INTERFACE_PART(CXTPReportRecord, IID_IEnumVARIANT, EnumVARIANT)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPReportRecord, IID_IReportRecord)

LPDISPATCH CXTPReportRecord::OleGetItem(long nIndex)
{
	if (nIndex >= 0 && nIndex < GetItemCount())
	{
		return GetItem(nIndex)->GetIDispatch(TRUE);
	}
	AfxThrowOleException(E_INVALIDARG);
	return 0;
}

IMPLEMENT_ENUM_VARIANT(CXTPReportRecord)

int CXTPReportRecord::OleGetItemCount()
{
	return GetItemCount();
}

LPDISPATCH CXTPReportRecord::OleAdd(const VARIANT& lpValue)
{
	if (lpValue.vt == (VT_VARIANT|VT_BYREF) )
	{
		return OleAdd(*lpValue.pvarVal);
	}

	CXTPReportRecordItemVariant* pItem = new CXTPReportRecordItemVariant(lpValue);

	AddItem(pItem);

	return pItem->GetIDispatch(TRUE);
}

void CXTPReportRecord::OleAddEx(LPDISPATCH lpDisp)
{
	CXTPReportRecordItem* pItem = DYNAMIC_DOWNCAST(CXTPReportRecordItem, CCmdTarget::FromIDispatch(lpDisp));

	if (pItem)
	{
		AddItem(pItem);
		pItem->InternalAddRef();
	}
}

LPDISPATCH CXTPReportRecord::OleGetChilds()
{
	return GetChilds()->GetIDispatch(TRUE);
}

BSTR CXTPReportRecord::OleGetPreviewText()
{
	CString strPreviewText;

	if (m_pPreviewItem)
	{
		strPreviewText = m_pPreviewItem->GetPreviewText();
	}

	return strPreviewText.AllocSysString();

}

void CXTPReportRecord::OleSetPreviewText(LPCTSTR lpszText)
{
	if (lpszText)
	{
		if (!m_pPreviewItem)
		{
			SetPreviewItem(new CXTPReportRecordItemPreview());
		}

		m_pPreviewItem->SetPreviewText(lpszText);
	}
	else if (m_pPreviewItem)
	{
		m_pPreviewItem->InternalRelease();
		m_pPreviewItem = NULL;
	}
}

LPDISPATCH CXTPReportRecord::OleGetPreviewItem()
{
	return m_pPreviewItem ? m_pPreviewItem->GetIDispatch(TRUE) : FALSE;
}


LPDISPATCH CXTPReportRecord::OleGetParentRecord()
{
	if (m_pRecords && m_pRecords->GetOwnerRecord())
		return m_pRecords->GetOwnerRecord()->GetIDispatch(TRUE);

	return NULL;
}

LPDISPATCH CXTPReportRecord::OleGetRecords()
{
	if (m_pRecords)
		return m_pRecords->GetIDispatch(TRUE);

	return NULL;
}

#endif
