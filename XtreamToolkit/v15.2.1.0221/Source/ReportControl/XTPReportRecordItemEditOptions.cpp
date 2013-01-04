// XTPReportRecordItemEditOptions.cpp : implementation of the CXTPReportRecordItemEditOptions class.
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
#include "Common/XTPCustomHeap.h"

#include "XTPReportDefines.h"
#include "XTPReportRecordItem.h"
#include "XTPReportRecordItemConstraint.h"
#include "XTPReportInplaceControls.h"

//////////////////////////////////////////////////////////////////////////
// CXTPReportRecordItemEditOptions

CXTPReportRecordItemEditOptions::CXTPReportRecordItemEditOptions()
{
	m_bAllowEdit = TRUE;
	m_bConstraintEdit = FALSE;
	m_pConstraints = new CXTPReportRecordItemConstraints();
	m_bSelectTextOnEdit = FALSE;
	m_dwEditStyle = ES_AUTOHSCROLL;
	m_nMaxLength = 0;


}

CXTPReportRecordItemEditOptions::~CXTPReportRecordItemEditOptions()
{
	RemoveButtons();

	CMDTARGET_RELEASE(m_pConstraints);
}

void CXTPReportRecordItemEditOptions::RemoveButtons()
{
	for (int j = 0; j < arrInplaceButtons.GetSize(); j++)
		if (arrInplaceButtons[j] != NULL)
			arrInplaceButtons[j]->InternalRelease();

	arrInplaceButtons.RemoveAll();
}

CXTPReportInplaceButton* CXTPReportRecordItemEditOptions::AddComboButton(BOOL bInside)
{
	CXTPReportInplaceButton* pButton = new CXTPReportInplaceButton(XTP_ID_REPORT_COMBOBUTTON);
	if (pButton)
	{
		arrInplaceButtons.Add(pButton);
		pButton->SetInsideCellButton(bInside);
	}
	return pButton;
}

CXTPReportInplaceButton* CXTPReportRecordItemEditOptions::AddExpandButton(BOOL bInside)
{
	CXTPReportInplaceButton* pButton = new CXTPReportInplaceButton(XTP_ID_REPORT_EXPANDBUTTON);
	if (pButton)
	{
		arrInplaceButtons.Add(pButton);
		pButton->SetInsideCellButton(bInside);
	}
	return pButton;
}

CXTPReportInplaceButton* CXTPReportRecordItemEditOptions::AddSpinButton(BOOL bInside)
{
	CXTPReportInplaceButton* pButton = new CXTPReportInplaceButton(XTP_ID_REPORT_SPINBUTTON);
	if (pButton)
	{
		arrInplaceButtons.Add(pButton);
		pButton->SetInsideCellButton(bInside);
	}
	return pButton;
}

CXTPReportInplaceButton* CXTPReportRecordItemEditOptions::GetButton(int nIndex)
{
	if (nIndex >= arrInplaceButtons.GetSize())
		return NULL;
	return arrInplaceButtons.GetAt(nIndex);
}

CXTPReportRecordItemConstraint* CXTPReportRecordItemEditOptions::FindConstraint(DWORD_PTR dwData)
{
	for (int i = 0; i < m_pConstraints->GetCount(); i++)
	{
		CXTPReportRecordItemConstraint *pConstaint = m_pConstraints->GetAt(i);
		if (pConstaint->m_dwData == dwData)
			return pConstaint;
	}
	return NULL;
}

CXTPReportRecordItemConstraint* CXTPReportRecordItemEditOptions::FindConstraint(LPCTSTR lpszConstraint)
{
	for (int i = 0; i < m_pConstraints->GetCount(); i++)
	{
		CXTPReportRecordItemConstraint *pConstaint = m_pConstraints->GetAt(i);
		if (pConstaint->m_strConstraint == lpszConstraint)
			return pConstaint;
	}
	return NULL;
}

CXTPReportRecordItemConstraint* CXTPReportRecordItemEditOptions::AddConstraint(LPCTSTR lpszConstraint, DWORD_PTR dwData /*= 0*/)
{
	CXTPReportRecordItemConstraint* pConstaint = new CXTPReportRecordItemConstraint();

	pConstaint->m_strConstraint = lpszConstraint;
	pConstaint->m_dwData = dwData;
	pConstaint->m_nIndex = (int)m_pConstraints->m_arrConstraints.Add(pConstaint);

	return pConstaint;
}

