// XTPReportRecordItemText.cpp : implementation of the CXTPReportRecordItemText class.
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
#include "XTPReportRecordItemText.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//////////////////////////////////////////////////////////////////////////
// CXTPReportRecordItemText

IMPLEMENT_SERIAL(CXTPReportRecordItemText, CXTPReportRecordItem, VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)

CXTPReportRecordItemText::CXTPReportRecordItemText(LPCTSTR szText)
	: CXTPReportRecordItem()
{
	SetValue(szText);
}

void CXTPReportRecordItemText::SetValue(LPCTSTR szText)
{
	m_strText = szText;
	ParseBBCode(m_strText);
}


CString CXTPReportRecordItemText::GetCaption(CXTPReportColumn *pColumn)
{
	UNREFERENCED_PARAMETER(pColumn);

	if (!m_strCaption.IsEmpty())
		return m_strCaption;

	if (m_strFormatString == _T("%s"))
		return m_strText;

	CString strCaption;
	//if (m_strFormatString.Find(_T("%")) > -1)
	//{
	//  if (m_strFormatString.Find(_T("d")) > -1)
	//      strCaption.Format(m_strFormatString, _ttoi(m_strText));
	//  else if (m_strFormatString.Find(_T("f")) > -1 || m_strFormatString.Find(_T("g")) > -1)
	//      strCaption.Format(m_strFormatString, _tstof(m_strText));
	//  else
	//      strCaption.Format(m_strFormatString, (LPCTSTR)m_strText);
	//}
	strCaption.Format(m_strFormatString, (LPCTSTR)m_strText);
	return strCaption;
}

void CXTPReportRecordItemText::OnEditChanged(XTP_REPORTRECORDITEM_ARGS* pItemArgs, LPCTSTR szText)
{
	SetValue(szText);

	CXTPReportRecordItem::OnEditChanged(pItemArgs, szText);
}

void CXTPReportRecordItemText::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPReportRecordItem::DoPropExchange(pPX);

	PX_String(pPX, _T("Text"), m_strText, _T(""));
}
