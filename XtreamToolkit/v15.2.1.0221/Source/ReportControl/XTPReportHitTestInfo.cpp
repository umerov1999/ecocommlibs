// XTPReportHitTestInfo.cpp : implementation of the CXTPReportHitTestInfo class.
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

#include "Common/XTPPropExchange.h"
#include "Common/XTPSmartPtrInternalT.h"
#include "Common/XTPCustomHeap.h"
#include "Common/XTPSystemHelpers.h"

#include "XTPReportDefines.h"
#include "XTPReportColumn.h"
#include "XTPReportRow.h"
#include "XTPReportRecordItem.h"
#include "TrackControl/XTPTrackBlock.h"
#include "XTPReportHitTestInfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CXTPReportHitTestInfo, CXTPCmdTarget)

CXTPReportHitTestInfo::CXTPReportHitTestInfo()
	: m_htCode (xtpReportHitTestUnknown)
	, m_pColumn(NULL)
	, m_pRow   (NULL)
	, m_pItem  (NULL)
	, m_pBlock (NULL)
	, m_iTrackPosition(0)
{
}

CXTPReportHitTestInfo::~CXTPReportHitTestInfo()
{
}

