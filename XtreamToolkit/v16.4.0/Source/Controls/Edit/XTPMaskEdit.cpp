// XTPMaskEdit.cpp : implementation of the CXTPMaskEdit class.
//
// This file is a part of the XTREME CONTROLS MFC class library.
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
#include "Common/XTPMaskEditT.h"
#include "../Util/XTPControlTheme.h"

#include "XTPEdit.h"
#include "../Util/XTPGlobal.h"
#include "XTPMaskEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTPMaskEdit
/////////////////////////////////////////////////////////////////////////////

CXTPMaskEdit::CXTPMaskEdit()
{
}


IMPLEMENT_DYNAMIC(CXTPMaskEdit, CXTPEdit)


bool CXTPMaskEdit::Initialize(CWnd* /*pParentWnd*/)
{
	SetFont(&XTPAuxData().font);
	return true;
}

/////////////////////////////////////////////////////////////////////////////
// CXTPDateEdit class
/////////////////////////////////////////////////////////////////////////////

CXTPDateEdit::CXTPDateEdit()
{
	m_bUseMask = true;
	m_strMask = _T("00/00/0000");
	m_strLiteral = _T("__/__/____");
	m_strDefault = _T("00/00/0000");
}

COleDateTime CXTPDateEdit::ReadOleDateTime(LPCTSTR lpszData)
{
	COleDateTime dt;
	dt.ParseDateTime(lpszData);
	return dt;
}

IMPLEMENT_DYNAMIC(CXTPDateEdit, CXTPMaskEdit)

void CXTPDateEdit::FormatOleDateTime(CString &strData, COleDateTime dt)
{
	strData = dt.Format(_T("%d/%m/%Y"));
}

void CXTPDateEdit::SetDateTime(COleDateTime& dt)
{
	CString strText;
	FormatOleDateTime(strText, dt);
	m_strWindowText = m_strDefault = strText;
	SetWindowText(strText);
}

void CXTPDateEdit::SetDateTime(LPCTSTR strDate)
{
	m_strWindowText = m_strDefault = strDate;
	SetWindowText(strDate);
}

COleDateTime CXTPDateEdit::GetDateTime()
{
	CString strText;
	GetWindowText(strText);
	return ReadOleDateTime(strText);
}

CString CXTPDateEdit::GetWindowDateTime()
{
	CString strText;
	GetWindowText(strText);
	return strText;
}

BOOL CXTPDateEdit::ProcessMask(TCHAR& nChar, int nEndPos)
{
	// check the key against the mask
	if (m_strMask[nEndPos] == _T('0') && _istdigit((TCHAR)nChar))
	{
		// Allow d/m/y and m/d/y

		if (nEndPos == 0 || nEndPos == 3)
		{
			if (nChar > '3')
				return FALSE;
		}
		if (nEndPos == 1 || nEndPos == 4)
		{
			if (m_strWindowText.GetAt(0) == _T('3'))
			{
				if (nChar > _T('1'))
					return FALSE;
			}
		}

		return TRUE;
	}

	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// CXTPTimeEdit class
/////////////////////////////////////////////////////////////////////////////

CXTPTimeEdit::CXTPTimeEdit()
{
	m_bMilitary = false;
	m_bUseMask = true;
	m_strMask = _T("00:00");
	m_strLiteral = _T("__:__");
	m_strDefault = _T("00:00");
	m_iHours = 0;
	m_iMins = 0;
}

IMPLEMENT_DYNAMIC(CXTPTimeEdit, CXTPDateEdit)

void CXTPTimeEdit::FormatOleDateTime(CString &strData, COleDateTime dt)
{
	if (dt.m_dt == 0)
	{
		strData = _T("00:00");
	}
	else
	{
		strData = dt.Format(_T("%H:%M"));
	}
}

BOOL CXTPTimeEdit::ProcessMask(TCHAR& nChar, int nEndPos)
{
	// check the key against the mask
	if (m_strMask[nEndPos] == _T('0') && _istdigit(nChar))
	{
		switch (nEndPos)
		{
		case 0:
			if (m_bMilitary)
			{
				if (nChar > _T('2'))
					return FALSE;
			}
			else
			{
				if (nChar > _T('1'))
					return FALSE;
			}
			return TRUE;

		case 1:
			if (m_bMilitary)
			{
				if (m_strWindowText.GetAt(0) == _T('2'))
				{
					if (nChar > _T('3'))
						return FALSE;
				}
			}
			else
			{
				if (m_strWindowText.GetAt(0) == _T('1'))
				{
					if (nChar > _T('2'))
						return FALSE;
				}
			}
			return TRUE;

		case 3:
			if (nChar > _T('5'))
				return FALSE;
			return TRUE;

		case 4:
			return TRUE;
		}
	}

	return FALSE;
}

void CXTPTimeEdit::SetHours(int nHours)
{
	m_iHours = nHours;

	CString strText;
	strText.Format(_T("%02d:%02d"), m_iHours, m_iMins);
	SetWindowText(strText);
}

void CXTPTimeEdit::SetMins(int nMins)
{
	m_iMins = nMins;

	CString strText;
	strText.Format(_T("%02d:%02d"), m_iHours, m_iMins);
	SetWindowText(strText);
}

void CXTPTimeEdit::SetTime(int nHours, int nMins)
{
	m_iHours = nHours;
	m_iMins = nMins;

	CString strText;
	strText.Format(_T("%02d:%02d"), m_iHours, m_iMins);
	SetWindowText(strText);
}

/////////////////////////////////////////////////////////////////////////////
// DDX_ routines
/////////////////////////////////////////////////////////////////////////////

_XTP_EXT_CLASS void AFXAPI DDX_XTPOleDateTime(CDataExchange* pDX, int nIDC, CXTPDateEdit& rControl, COleDateTime& rDateTime)
{
	DDX_Control(pDX, nIDC, rControl);

	if (pDX->m_bSaveAndValidate)
	{
		rDateTime = rControl.GetDateTime();
	}
	else
	{
		rControl.SetDateTime(rDateTime);
	}
}
