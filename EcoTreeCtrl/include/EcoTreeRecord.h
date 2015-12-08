// MessageRecord.h: interface for the CMessageRecord class.
//
// This file is a part of the XTREME TOOLKIT PRO MFC class library.
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

#if !defined(AFX_MESSAGERECORD_H__A08F955C_1EA1_40B4_A18F_D2B7857FB244__INCLUDED_)
#define AFX_MESSAGERECORD_H__A08F955C_1EA1_40B4_A18F_D2B7857FB244__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "stdafx.h"
class CReportSampleView;

//////////////////////////////////////////////////////////////////////////
// Customized record item, used for displaying prices.
class CMessageRecordItemPrice : public CXTPReportRecordItemNumber
{
	DECLARE_SERIAL(CMessageRecordItemPrice)
public:
	// Constructs record item with the initial decimal price value.
	CMessageRecordItemPrice(double dValue = .0);

	// Provides custom group captions depending on the price.
	virtual CString GetGroupCaption(CXTPReportColumn* pColumn);

	// Provides custom group values comparison based on price value, 
	// instead of based on captions.
	virtual int CompareGroupCaption(CXTPReportColumn* pColumn, CXTPReportRecordItem* pItem);
};

//////////////////////////////////////////////////////////////////////////
// Customized record item, used for displaying checkboxes.
class CMessageRecordItemCheck : public CXTPReportRecordItemText
{
	DECLARE_SERIAL(CMessageRecordItemCheck)
public:
	// Constructs record item with the initial checkbox value.
	CMessageRecordItemCheck(BOOL bCheck = FALSE);

	// Provides custom group captions depending on checkbox value.
	// Returns caption string ID to be read from application resources.
	virtual int GetGroupCaptionID(CXTPReportColumn* pColumn);

	// Provides custom records comparison by this item based on checkbox value, 
	// instead of based on captions.
	virtual int Compare(CXTPReportColumn* pColumn, CXTPReportRecordItem* pItem);
};

//////////////////////////////////////////////////////////////////////////
// Enumerates possible Message Importance values for using by 
// CMessageRecordItemImportance class
typedef enum MESSAGE_IMPORTANCE
{
	msgImportanceNormal,
	msgImportanceHigh,
	msgImportanceLow
};

//////////////////////////////////////////////////////////////////////////
// Customized record item, used for displaying importance icons.
class CMessageRecordItemImportance : public CXTPReportRecordItem
{
	DECLARE_SERIAL(CMessageRecordItemImportance)
public:
	// Constructs record item with the initial value.
	CMessageRecordItemImportance(MESSAGE_IMPORTANCE eImportance = msgImportanceNormal);
	
	virtual void DoPropExchange(CXTPPropExchange* pPX);
protected:
	MESSAGE_IMPORTANCE m_eImportance;   // Message importance
};

//////////////////////////////////////////////////////////////////////////
// Customized record item, used for displaying attachments icons.
class CMessageRecordItemAttachment : public CXTPReportRecordItem
{
	DECLARE_SERIAL(CMessageRecordItemAttachment)
public:
	// Constructs record item with the initial value.
	CMessageRecordItemAttachment(BOOL bHasAttachment = FALSE);

	virtual void DoPropExchange(CXTPPropExchange* pPX);
protected:
	BOOL m_bHasAttachment;	// TRUE when message has attachments, FALSE otherwise.
};

//////////////////////////////////////////////////////////////////////////
// Customized record item, used for displaying read/unread icons.
class CMessageRecordItemIcon : public CXTPReportRecordItem
{
	DECLARE_SERIAL(CMessageRecordItemIcon)
public:
	// Constructs record item with the initial read/unread value.
	CMessageRecordItemIcon(BOOL bRead = FALSE);

	// Provides custom group captions depending on the read/unread value.
	virtual CString GetGroupCaption(CXTPReportColumn* pColumn);

	// Provides custom group values comparison based on read/unread value, 
	// instead of based on captions.
	virtual int CompareGroupCaption(CXTPReportColumn* pColumn, CXTPReportRecordItem* pItem);

	// Updates record item icon index depending on read/unread value.
	void UpdateReadIcon();

	// Provides custom records comparison by this item based on read/unread value, 
	// instead of based on captions.
	int Compare(CXTPReportColumn* pColumn, CXTPReportRecordItem* pItem);

	virtual void DoPropExchange(CXTPPropExchange* pPX);
public:
	BOOL m_bRead;	// TRUE for read, FALSE for unread.
};

//////////////////////////////////////////////////////////////////////////
// Customized record Date/Time item.
// Main customization purpose is overriding GetGroupCaptionID and providing
// application-specific caption when Report control data is grouped via this item.
class CMessageRecordItemDate : public CXTPReportRecordItemDateTime
{
	DECLARE_SERIAL(CMessageRecordItemDate)
public:
	// Construct record item from COleDateTime value.
	CMessageRecordItemDate(COleDateTime odtValue = COleDateTime::GetCurrentTime());

	// Provides custom group captions depending on the item date value.
	virtual int GetGroupCaptionID(CXTPReportColumn* pColumn);
};

//////////////////////////////////////////////////////////////////////////
// This class is your main custom Record class which you'll manipulate with.
// It contains any kind of specific methods like different types of constructors,
// any additional custom data as class members, any data manipulation methods.
class CAssemblyTreeTreeRecord : public CXTPReportRecord
{
	DECLARE_SERIAL(CAssemblyTreeTreeRecord)
public:
	
	// Construct record object using empty values on each field
	CAssemblyTreeTreeRecord();
	
	// Construct record object from detailed values on each field
	CAssemblyTreeTreeRecord(int nColumnCnt, CString strName, CAssemblyTreeRecordSet* pRecordSet, CAssemblyLevelRecordSet* pAssemLevelRecordSet);

	// Clean up internal objects
	virtual ~CAssemblyTreeTreeRecord();

	// Create record fields using empty values
	virtual void CreateItems();

	// Overridden callback method, where we can customize any drawing item metrics.
	virtual void GetItemMetrics(XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs, XTP_REPORTRECORDITEM_METRICS* pItemMetrics);

	virtual void DoPropExchange(CXTPPropExchange *pPX);

public:
	CXTPReportRecordItemText *m_pItemName;     // Sender
	CAssemblyTreeRecordSet* m_pRecordSet;
	CAssemblyLevelRecordSet* m_pAssemLevelRecordSet;
};


class CAssemblyLevelTreeRecord : public CXTPReportRecord
{
	DECLARE_SERIAL(CAssemblyLevelTreeRecord)
public:
	
	// Construct record object using empty values on each field
	CAssemblyLevelTreeRecord();
	
	// Construct record object from detailed values on each field
	CAssemblyLevelTreeRecord(int nColumnCnt, CString strName, CAssemblyLevelRecordSet* pRecordSet);

	// Clean up internal objects
	virtual ~CAssemblyLevelTreeRecord();

	// Create record fields using empty values
	virtual void CreateItems();

	// Overridden callback method, where we can customize any drawing item metrics.
	virtual void GetItemMetrics(XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs, XTP_REPORTRECORDITEM_METRICS* pItemMetrics);

	virtual void DoPropExchange(CXTPPropExchange *pPX);

public:
	CXTPReportRecordItemText *m_pItemName;     // Sender
	CAssemblyLevelRecordSet* m_pRecordSet;
};


class CShipNoInfoTreeRecord : public CXTPReportRecord
{
	DECLARE_SERIAL(CShipNoInfoTreeRecord)
public:

	// Construct record object using empty values on each field
	CShipNoInfoTreeRecord();

	// Construct record object from detailed values on each field
	CShipNoInfoTreeRecord(int nColumnCnt, CString strName, CShipNoInfoRecordSet* pRecordSet);

	// Clean up internal objects
	virtual ~CShipNoInfoTreeRecord();

	// Create record fields using empty values
	virtual void CreateItems();

	// Overridden callback method, where we can customize any drawing item metrics.
	virtual void GetItemMetrics(XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs, XTP_REPORTRECORDITEM_METRICS* pItemMetrics);

	virtual void DoPropExchange(CXTPPropExchange *pPX);

public:
	CXTPReportRecordItemText *m_pItemName;     // Sender
	CShipNoInfoRecordSet* m_pRecordSet;
};


class CVariableGroupTreeRecord : public CXTPReportRecord
{
	DECLARE_SERIAL(CVariableGroupTreeRecord)
public:

	// Construct record object using empty values on each field
	CVariableGroupTreeRecord();

	// Construct record object from detailed values on each field
	CVariableGroupTreeRecord(int nColumnCnt, CString strName, CVariableGroupRecordSet* pRecordSet);

	// Clean up internal objects
	virtual ~CVariableGroupTreeRecord();

	// Create record fields using empty values
	virtual void CreateItems();

	// Overridden callback method, where we can customize any drawing item metrics.
	virtual void GetItemMetrics(XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs, XTP_REPORTRECORDITEM_METRICS* pItemMetrics);

	virtual void DoPropExchange(CXTPPropExchange *pPX);

public:
	CXTPReportRecordItemText *m_pItemName;     // Sender
	CVariableGroupRecordSet* m_pRecordSet;
};


class CVariableOfGroupTreeRecord : public CXTPReportRecord
{
	DECLARE_SERIAL(CVariableOfGroupTreeRecord)
public:

	// Construct record object using empty values on each field
	CVariableOfGroupTreeRecord();

	// Construct record object from detailed values on each field
	CVariableOfGroupTreeRecord(int nColumnCnt, CString strName, CVariableGroupRecordSet* pRecordSet);
	CVariableOfGroupTreeRecord(int nColumnCnt, CString strName, BOOL bEssential, int nViewOrder, BOOL bPjtID, CVariableOfGroupRecordSet* pRecordSet);

	// Clean up internal objects
	virtual ~CVariableOfGroupTreeRecord();

	// Create record fields using empty values
	virtual void CreateItems();

	// Overridden callback method, where we can customize any drawing item metrics.
	virtual void GetItemMetrics(XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs, XTP_REPORTRECORDITEM_METRICS* pItemMetrics);

	virtual void DoPropExchange(CXTPPropExchange *pPX);

public:
	CXTPReportRecordItemText *m_pItemName;     // Sender
	CEcoTreeBaseRecordSet* m_pRecordSet;
};



class CUserVariableTreeRecord : public CXTPReportRecord
{
	DECLARE_SERIAL(CUserVariableTreeRecord)
public:

	// Construct record object using empty values on each field
	CUserVariableTreeRecord();

	// Construct record object from detailed values on each field
	CUserVariableTreeRecord(int nColumnCnt, CString strName, CString strProperty, CString strType, BOOL bEssential, BOOL bDuplicate, int nViewOrder, BOOL bPjtID, CUserVariableRecordSet* pRecordSet);

	// Clean up internal objects
	virtual ~CUserVariableTreeRecord();

	// Create record fields using empty values
	virtual void CreateItems();

	// Overridden callback method, where we can customize any drawing item metrics.
	virtual void GetItemMetrics(XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs, XTP_REPORTRECORDITEM_METRICS* pItemMetrics);

	virtual void DoPropExchange(CXTPPropExchange *pPX);

public:
	CXTPReportRecordItemText *m_pItemName;     // Sender
	CUserVariableRecordSet* m_pRecordSet;
};


class CMeasureDataTreeTreeRecord : public CXTPReportRecord
{
	DECLARE_SERIAL(CMeasureDataTreeTreeRecord)
public:

	// Construct record object using empty values on each field
	CMeasureDataTreeTreeRecord();

	// Construct record object from detailed values on each field
	CMeasureDataTreeTreeRecord(int nColumnCnt, CString strName, COleDateTime odtDate, UINT nMsrCnt, CMeasurePointRecordSet* pRecordSet);

	// Clean up internal objects
	virtual ~CMeasureDataTreeTreeRecord();

	// Create record fields using empty values
	virtual void CreateItems();

	// Overridden callback method, where we can customize any drawing item metrics.
	virtual void GetItemMetrics(XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs, XTP_REPORTRECORDITEM_METRICS* pItemMetrics);

	virtual void DoPropExchange(CXTPPropExchange *pPX);

public:
	CXTPReportRecordItemText *m_pItemName;     // Sender
	CMeasurePointRecordSet* m_pRecordSet;
};


class CMESMeasureDataTreeTreeRecord : public CXTPReportRecord
{
	DECLARE_SERIAL(CMESMeasureDataTreeTreeRecord)
public:

	// Construct record object using empty values on each field
	CMESMeasureDataTreeTreeRecord();

	// Construct record object from detailed values on each field
	CMESMeasureDataTreeTreeRecord(int nColumnCnt, CString strName, COleDateTime odtDate, UINT nMsrCnt, CMESMeasurePointRecordSet* pRecordSet);

	// Clean up internal objects
	virtual ~CMESMeasureDataTreeTreeRecord();

	// Create record fields using empty values
	virtual void CreateItems();

	// Overridden callback method, where we can customize any drawing item metrics.
	virtual void GetItemMetrics(XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs, XTP_REPORTRECORDITEM_METRICS* pItemMetrics);

	virtual void DoPropExchange(CXTPPropExchange *pPX);

public:
	CXTPReportRecordItemText *m_pItemName;     // Sender
	CMESMeasurePointRecordSet* m_pRecordSet;
};


class CProjectTreeRecord : public CXTPReportRecord
{
	DECLARE_SERIAL(CProjectTreeRecord)
public:

	// Construct record object using empty values on each field
	CProjectTreeRecord();

	// Construct record object from detailed values on each field
	CProjectTreeRecord(int nColumnCnt, CString strName, CString strVarGroupName, CString strUpdateDate, CArray<CString, CString&>& arrColumns, 
						CArray<CProjectPropertyRecordSet, CProjectPropertyRecordSet&>& arrPjtPropRS, CProjectRecordSet* pRecordSet);

	CProjectTreeRecord(int nColumnCnt, CString strName, UINT nPjtType, CString strVarGroup, COleDateTime oleTime, CString strUser, 
						CProjectRecordSet* pRecordSet);

	// Clean up internal objects
	virtual ~CProjectTreeRecord();

	// Create record fields using empty values
	virtual void CreateItems();

	// Overridden callback method, where we can customize any drawing item metrics.
	virtual void GetItemMetrics(XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs, XTP_REPORTRECORDITEM_METRICS* pItemMetrics);

	virtual void DoPropExchange(CXTPPropExchange *pPX);

public:
	CXTPReportRecordItemText *m_pItemName;     // Sender
	CProjectRecordSet* m_pRecordSet;
	CArray<CProjectPropertyRecordSet, CProjectPropertyRecordSet&> m_arrPjtPropRS;
};


class CEcoBlockProjectTreeTreeRecord : public CXTPReportRecord
{
	DECLARE_SERIAL(CEcoBlockProjectTreeTreeRecord)
public:

	// Construct record object using empty values on each field
	CEcoBlockProjectTreeTreeRecord();

	// Construct record object from detailed values on each field
	CEcoBlockProjectTreeTreeRecord(int nColumnCnt, CString strName, CString strFilePjt, CString strComment, CString strUpdateDate, CString strUser, CEcoBlockProjectRecordSet* pRecordSet);

	// Clean up internal objects
	virtual ~CEcoBlockProjectTreeTreeRecord();

	// Create record fields using empty values
	virtual void CreateItems();

	// Overridden callback method, where we can customize any drawing item metrics.
	virtual void GetItemMetrics(XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs, XTP_REPORTRECORDITEM_METRICS* pItemMetrics);

	virtual void DoPropExchange(CXTPPropExchange *pPX);

public:
	CXTPReportRecordItemText *m_pItemName;     // Sender
	CEcoBlockProjectRecordSet* m_pRecordSet;
};

class CEcoOTSProjectTreeTreeRecord : public CXTPReportRecord
{
	DECLARE_SERIAL(CEcoOTSProjectTreeTreeRecord)
public:

	// Construct record object using empty values on each field
	CEcoOTSProjectTreeTreeRecord();

	// Construct record object from detailed values on each field
	CEcoOTSProjectTreeTreeRecord(int nColumnCnt, CString strName, CString strFilePjt, CString strComment, CString strUpdateDate, CString strUser, CEcoOtsProjectRecordSet* pRecordSet);

	// Clean up internal objects
	virtual ~CEcoOTSProjectTreeTreeRecord();

	// Create record fields using empty values
	virtual void CreateItems();

	// Overridden callback method, where we can customize any drawing item metrics.
	virtual void GetItemMetrics(XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs, XTP_REPORTRECORDITEM_METRICS* pItemMetrics);

	virtual void DoPropExchange(CXTPPropExchange *pPX);

public:
	CXTPReportRecordItemText *m_pItemName;     // Sender
	CEcoOtsProjectRecordSet* m_pRecordSet;
};


#endif // !defined(AFX_MESSAGERECORD_H__A08F955C_1EA1_40B4_A18F_D2B7857FB244__INCLUDED_)

