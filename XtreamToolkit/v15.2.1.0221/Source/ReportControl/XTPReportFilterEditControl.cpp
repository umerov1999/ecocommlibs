// XTPReportFilterEditControl.cpp : implementation of the CXTPReportFilterEditControl class.
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
#include "Resource.h"

#include "Common/XTPResourceManager.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPCustomHeap.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPSmartPtrInternalT.h"
#include "Common/XTPColorManager.h"

#include "XTPReportDefines.h"
#include "XTPReportControl.h"
#include "XTPReportFilterEditControl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CXTPReportFilterEditControl

CXTPReportFilterEditControl::CXTPReportFilterEditControl()
{
	m_pReportCtrl = NULL;
	m_strHint = XTPResourceManager()->LoadString(XTP_IDS_REPORT_FILERT_TEXT_HINT) ;
}

CXTPReportFilterEditControl::~CXTPReportFilterEditControl()
{
}


BEGIN_MESSAGE_MAP(CXTPReportFilterEditControl, CEdit)
	//{{AFX_MSG_MAP(CXTPReportFilterEditControl)
	ON_CONTROL_REFLECT_EX(EN_CHANGE, OnChange)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTPReportFilterEditControl attributes

BOOL CXTPReportFilterEditControl::SetReportCtrl(CXTPReportControl* pReportCtrl)
{
	if (pReportCtrl == NULL)
		return FALSE;

	ASSERT_KINDOF(CXTPReportControl, pReportCtrl);
	m_pReportCtrl = pReportCtrl;

	return TRUE;
}

CXTPReportControl* CXTPReportFilterEditControl::GetReportCtrl()
{
	return m_pReportCtrl;
}

/////////////////////////////////////////////////////////////////////////////
// CXTPReportFilterEditControl operations


/////////////////////////////////////////////////////////////////////////////
// CXTPReportFilterEditControl message handlers

void CXTPReportFilterEditControl::SetHint(LPCTSTR lpszHint)
{
	m_strHint = lpszHint;
}

CString CXTPReportFilterEditControl::GetHint() const
{
	return m_strHint;
}

// Handles the EN_CHANGE message
BOOL CXTPReportFilterEditControl::OnChange()
{
	if (!m_pReportCtrl)
		return FALSE;

	// Get text
	CString strNewFilter;
	GetWindowText(strNewFilter);

	// Apply new filter
	if (m_pReportCtrl->GetFilterText() != strNewFilter)
	{
		m_pReportCtrl->SetFilterText(strNewFilter);

		// refresh control
		m_pReportCtrl->Populate();
	}

	// Returning FALSE allows the parent window to also handle the EN_CHANGE message
	return FALSE;
}

void CXTPReportFilterEditControl::OnPaint()
{
	if (GetWindowTextLength() == 0 && ::GetFocus() != m_hWnd)
	{
		CPaintDC dc(this); // device context for painting

		CXTPFontDC autoFont(&dc, GetFont(), GetXtremeColor(COLOR_GRAYTEXT));

		// show hint text
		CString  strText = GetHint();

		CRect rc;
		GetClientRect(&rc);
		dc.FillSolidRect(rc, GetXtremeColor(COLOR_WINDOW));

		CRect rcText;
		GetRect(&rcText);
		dc.DrawText(strText, rcText, DT_SINGLELINE | DT_VCENTER | DT_NOPREFIX | DT_EDITCONTROL);
	}
	else
	{
		Default();
	}
}
