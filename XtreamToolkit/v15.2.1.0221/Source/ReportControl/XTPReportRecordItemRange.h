// XTPReportRecordItemRange.h: interface for the CXTPReportRecordItemRange class.
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
#if !defined(__XTPREPORTRECORDITEMRANGE_H__)
#define __XTPREPORTRECORDITEMRANGE_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXTPReportRecordItem;

class _XTP_EXT_CLASS CXTPReportRecordItemId
{
public:
	CXTPReportRecordItemId();
	CXTPReportRecordItemId(int nColumn, int nRecord);

	void Set(int nColumn, int nRecord);

	int GetColumn() const;

	int GetRecord() const;

	bool operator == (const CXTPReportRecordItemId &otherId) const;

public:
	int m_nColumn;   // Column index
	int m_nRecord;   // Record index
};


class _XTP_EXT_CLASS CXTPReportRecordMergeItemId : public CXTPReportRecordItemId
{
public:
	CXTPReportRecordMergeItemId();
	CXTPReportRecordMergeItemId(int nColumnPos, int nColumn, int nRecord);

public:
	int m_nColumnPos;   // Column position
};


class _XTP_EXT_CLASS CXTPReportRecordItemRange
{
public:

	CXTPReportRecordItemRange();

	CXTPReportRecordItemRange(
		int nColumnFrom,
		int nColumnTo,
		int nRecordFrom,
		int nRecordTo);

	void SetTopLeft(const CXTPReportRecordItemId topLeft);
	void SetBottomRight(const CXTPReportRecordItemId bottomRight);

	void ExpandTo(int nColumn, int nRecord);

	void ExpandTo(const CXTPReportRecordItemId expand);

	void Invalidate();

	bool IsValid() const;

	bool IsInRange(const int nColumn, const int nRecord) const;

	bool IsNeighbour(const int nColumn, const int nRecord) const;

	bool IsConnected(const int nColumn, const int nRecord) const;

public:
	int m_nColumnFrom;
	int m_nColumnTo;
	int m_nRecordFrom;
	int m_nRecordTo;
};


class _XTP_EXT_CLASS CXTPReportRecordMergeItem
{
public:

	CXTPReportRecordMergeItem();
	CXTPReportRecordMergeItem(const CXTPReportRecordMergeItem &other);

	CXTPReportRecordMergeItem& operator = (const CXTPReportRecordMergeItem &other);

	void AddItem(CXTPReportRecordMergeItemId itemId, CRect rcItem);

	bool FindRange(CXTPReportRecordItemRange &range, CRect &rcItem);

	struct Item
	{
		CXTPReportRecordMergeItemId  m_itemId;
		CRect m_rcItem;
	};

public:

	void CopyFrom(const CXTPReportRecordMergeItem &other);

	CList<Item, Item> m_items;
};


class CXTPReportRecordMergeItems : public CMap< CXTPReportRecordItem*,     CXTPReportRecordItem*, CXTPReportRecordMergeItem, CXTPReportRecordMergeItem&>
{

};


#endif //#if !defined(__XTPREPORTRECORDITEMRANGE_H__)
