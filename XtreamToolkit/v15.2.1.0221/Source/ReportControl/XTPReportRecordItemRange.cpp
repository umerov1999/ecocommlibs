// XTPReportRecordItemRange.cpp
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

#include "XTPReportRecordItemRange.h"



CXTPReportRecordItemId::CXTPReportRecordItemId()
	: m_nColumn(-1)
	, m_nRecord(-1)
{
	// Nothing
}


CXTPReportRecordItemId::CXTPReportRecordItemId(int nColumn, int nRecord)
	: m_nColumn(nColumn)
	, m_nRecord(nRecord)
{
	// Nothing
}


void CXTPReportRecordItemId::Set(int nColumn, int nRecord)
{
	m_nColumn = nColumn;
	m_nRecord = nRecord;
}


int CXTPReportRecordItemId::GetColumn() const
{
	return m_nColumn;
}


int CXTPReportRecordItemId::GetRecord() const
{
	return m_nRecord;
}

bool CXTPReportRecordItemId::operator == (
	const CXTPReportRecordItemId &otherId) const
{
	return (this->m_nColumn == otherId.m_nColumn)
		 && (this->m_nRecord == otherId.m_nRecord);
}


CXTPReportRecordMergeItemId::CXTPReportRecordMergeItemId()
	: CXTPReportRecordItemId()
	, m_nColumnPos(-1)
{
}


CXTPReportRecordMergeItemId::CXTPReportRecordMergeItemId(int nColumnPos, int nColumn, int nRecord)
	: CXTPReportRecordItemId(nColumn, nRecord)
	, m_nColumnPos(nColumnPos)
{
}


/////////////////////////////////////////////////////////////////////////////


CXTPReportRecordItemRange::CXTPReportRecordItemRange()
	: m_nColumnFrom(-1)
	, m_nColumnTo  (-1)
	, m_nRecordFrom(-1)
	, m_nRecordTo  (-1)
{
	// Nothing
}

CXTPReportRecordItemRange::CXTPReportRecordItemRange(
	int nColumnFrom,
	int nColumnTo,
	int nRecordFrom,
	int nRecordTo)
	: m_nColumnFrom(nColumnFrom)
	, m_nColumnTo  (nColumnTo)
	, m_nRecordFrom(nRecordFrom)
	, m_nRecordTo  (nRecordTo)
{
	// Nothing
}

void CXTPReportRecordItemRange::Invalidate()
{
	m_nColumnFrom = -1;
	m_nColumnTo   = -1;
	m_nRecordFrom = -1;
	m_nRecordTo   = -1;
}

bool CXTPReportRecordItemRange::IsValid() const
{
	return (m_nColumnFrom >= 0)
		 && (m_nColumnTo   >= 0)
		 && (m_nRecordFrom >= 0)
		 && (m_nRecordTo   >= 0);
}

bool CXTPReportRecordItemRange::IsInRange(
	const int nColumn,
	const int nRecord) const
{
	return (nColumn >= m_nColumnFrom) && (nColumn <= m_nColumnTo)
		 && (nRecord >= m_nRecordFrom) && (nRecord <= m_nRecordTo);
}

bool CXTPReportRecordItemRange::IsNeighbour(
	const int nColumn,
	const int nRecord) const
{
	bool bNeighbour = false;

	if (IsValid())
	{
		if (abs(m_nColumnFrom - nColumn) <= 1 &&
			 abs(m_nColumnTo   - nColumn) <= 1 &&
			 abs(m_nRecordFrom - nRecord) <= 1 &&
			 abs(m_nRecordTo   - nRecord) <= 1)
		{
			bNeighbour = true;
		}
	}

	return bNeighbour;
}


bool CXTPReportRecordItemRange::IsConnected(
	const int nColumn,
	const int nRecord) const
{
	bool bConnected = false;

/*
	Surrounding items marked as '+' are considered connected,
	items marked as '-' not.

	[-][+][+][-]
	[+][ ][ ][+]
	[+][ ][ ][+]
	[-][+][+][-]
*/

	if (IsValid())
	{
		if (nColumn >= m_nColumnFrom && nColumn <= m_nColumnTo)
		{
			if (nRecord == m_nRecordFrom-1 || nRecord == m_nRecordTo+1)
			{
				bConnected = true;
			}
		}

		if (nRecord >= m_nRecordFrom && nRecord <= m_nRecordTo)
		{
			if (nColumn == m_nColumnFrom-1 || nColumn == m_nColumnTo+1)
			{
				bConnected = true;
			}
		}
	}

	return bConnected;
}

void CXTPReportRecordItemRange::ExpandTo(int nColumn, int nRecord)
{
	if (IsValid())
	{
		m_nColumnFrom = min(m_nColumnFrom, nColumn);
		m_nColumnTo   = max(m_nColumnTo,   nColumn);
		m_nRecordFrom = min(m_nRecordFrom, nRecord);
		m_nRecordTo   = max(m_nRecordTo,   nRecord);
	}
}

void CXTPReportRecordItemRange::ExpandTo(
	const CXTPReportRecordItemId expand)
{
	ExpandTo(expand.m_nColumn, expand.m_nRecord);
}

/////////////////////////////////////////////////////////////////////////////


CXTPReportRecordMergeItem::CXTPReportRecordMergeItem()
{
	// Nothing
}


CXTPReportRecordMergeItem::CXTPReportRecordMergeItem(
	const CXTPReportRecordMergeItem &other)
{
	CopyFrom(other);
}


CXTPReportRecordMergeItem& CXTPReportRecordMergeItem::operator = (
	const CXTPReportRecordMergeItem &other)
{
	if (this != &other)
	{
		m_items.RemoveAll();
		CopyFrom(other);
	}

	return *this;
}


void CXTPReportRecordMergeItem::AddItem(CXTPReportRecordMergeItemId itemId, CRect rcItem)
{
	Item item;
	item.m_itemId = itemId;
	item.m_rcItem = rcItem;

	m_items.AddTail(item);
}


bool CXTPReportRecordMergeItem::FindRange(CXTPReportRecordItemRange &range, CRect &rcItem)
{
	CXTPReportRecordItemRange internal;

	range.Invalidate();

	Item item;

	POSITION pos = m_items.GetHeadPosition();

	while (pos)
	{
		POSITION posOld = pos;
		item = m_items.GetNext(pos);

		if (!internal.IsValid())
		{
			m_items.RemoveAt(posOld);
			rcItem = item.m_rcItem;

			internal.m_nColumnFrom = item.m_itemId.m_nColumnPos;
			internal.m_nColumnTo   = item.m_itemId.m_nColumnPos;
			internal.m_nRecordFrom = item.m_itemId.m_nRecord;
			internal.m_nRecordTo   = item.m_itemId.m_nRecord;

			range.m_nColumnFrom = item.m_itemId.m_nColumn;
			range.m_nColumnTo   = item.m_itemId.m_nColumn;
			range.m_nRecordFrom = item.m_itemId.m_nRecord;
			range.m_nRecordTo   = item.m_itemId.m_nRecord;
		}
		else
		{
			if (internal.IsInRange  (item.m_itemId.m_nColumnPos, item.m_itemId.m_nRecord)
			 || internal.IsConnected(item.m_itemId.m_nColumnPos, item.m_itemId.m_nRecord))
			{
				m_items.RemoveAt(posOld);
				
				internal.ExpandTo(item.m_itemId.m_nColumnPos, item.m_itemId.m_nRecord);
				range.ExpandTo(item.m_itemId);

				rcItem.left   = min(rcItem.left,   item.m_rcItem.left);
				rcItem.right  = max(rcItem.right,  item.m_rcItem.right);
				rcItem.top    = min(rcItem.top,    item.m_rcItem.top);
				rcItem.bottom = max(rcItem.bottom, item.m_rcItem.bottom);
			}
		}
	}

	return range.IsValid();
}


void CXTPReportRecordMergeItem::CopyFrom(
	const CXTPReportRecordMergeItem &other)
{
	POSITION pos = other.m_items.GetHeadPosition();

	while (pos)
	{
		m_items.AddTail(other.m_items.GetAt(pos));
		other.m_items.GetNext(pos);
	}
}
