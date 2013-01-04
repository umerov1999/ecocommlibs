// XTPReportRecord.cpp : implementation of the CXTPReportRecord class.
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
		m_pChildren = new CXTPReportRecords(this);

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

