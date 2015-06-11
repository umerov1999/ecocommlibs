// XTPReportRecordItemDateTime.cpp : implementation of the CXTPReportRecordItemDateTime class.
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

#include "StdAfx.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPPropExchange.h"
#include "Common/XTPMarkupRender.h"
#include "Common/XTPCustomHeap.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPSmartPtrInternalT.h"
#include "Common/XTPColorManager.h"

#include "ReportControl/XTPReportDefines.h"
#include "ReportControl/XTPReportRecordItem.h"
#include "ReportControl/XTPReportControl.h"
#include "ReportControl/XTPReportPaintManager.h"
#include "ReportControl/XTPReportRow.h"
#include "ReportControl/XTPReportRecord.h"
#include "ReportControl/XTPReportHyperlink.h"
#include "XTPReportRecordItemDateTime.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//////////////////////////////////////////////////////////////////////////
// CXTPReportRecordItemDateTime

IMPLEMENT_SERIAL(CXTPReportRecordItemDateTime, CXTPReportRecordItem, VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)

CXTPReportRecordItemDateTime::CXTPReportRecordItemDateTime(COleDateTime odtValue)
	: CXTPReportRecordItem(), m_odtValue(odtValue)
{
	static const CString cstrDateFormatDefault(_T("%a %b/%d/%Y %I:%M %p")); // to avoid new string data allocation for each record
	m_strFormatString = cstrDateFormatDefault;
}

CString CXTPReportRecordItemDateTime::GetCaption(CXTPReportColumn* /*pColumn*/)
{
	if (!m_strCaption.IsEmpty())
		return m_strCaption;

	//return m_odtValue.Format(m_strFormatString);
	return CXTPReportControlLocale::FormatDateTime(m_odtValue, m_strFormatString);
}

int CXTPReportRecordItemDateTime::Compare(CXTPReportColumn*, CXTPReportRecordItem* pItem)
{
	CXTPReportRecordItemDateTime* pItemDateTime = DYNAMIC_DOWNCAST(CXTPReportRecordItemDateTime, pItem);
	if (!pItemDateTime)
		return 0;

	if (m_odtValue == pItemDateTime->m_odtValue)
		return 0;

	if (m_odtValue.GetStatus() != COleDateTime::valid ||
		pItemDateTime->m_odtValue.GetStatus() != COleDateTime::valid)
		return int(m_odtValue.m_dt - pItemDateTime->m_odtValue.m_dt);

	if (m_odtValue > pItemDateTime->m_odtValue)
		return 1;

	return -1;
}

void CXTPReportRecordItemDateTime::OnEditChanged(XTP_REPORTRECORDITEM_ARGS* /*pItemArgs*/, LPCTSTR szText)
{
	m_odtValue.ParseDateTime(szText);
}

void CXTPReportRecordItemDateTime::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPReportRecordItem::DoPropExchange(pPX);

	PX_DateTime(pPX, _T("Value"), m_odtValue);
}
