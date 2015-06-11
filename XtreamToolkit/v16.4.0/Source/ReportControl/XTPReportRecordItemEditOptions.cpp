// XTPReportRecordItemEditOptions.cpp : implementation of the CXTPReportRecordItemEditOptions class.
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
	m_bExpandOnSelect = FALSE;
	m_dwEditStyle = ES_AUTOHSCROLL;
	m_nMaxLength = 0;

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif

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

#ifdef _XTP_ACTIVEX

//////////////////////////////////////////////////////////////////////////
// CXTPReportRecordItemEditOptions

BEGIN_DISPATCH_MAP(CXTPReportRecordItemEditOptions, CCmdTarget)
	DISP_FUNCTION_ID(CXTPReportRecordItemEditOptions, "Constraints", 1, OleGetConstraints, VT_DISPATCH, VTS_NONE)
	DISP_PROPERTY_ID(CXTPReportRecordItemEditOptions, "ConstraintEdit", 2, m_bConstraintEdit, VT_BOOL)
	DISP_FUNCTION_ID(CXTPReportRecordItemEditOptions, "AddComboButton", 3, OleAddComboButton, VT_DISPATCH, VTS_VARIANT)
	DISP_FUNCTION_ID(CXTPReportRecordItemEditOptions, "AddExpandButton", 4, OleAddExpandButton, VT_DISPATCH, VTS_VARIANT)
	DISP_PROPERTY_ID(CXTPReportRecordItemEditOptions, "AllowEdit", 5, m_bAllowEdit, VT_BOOL)
	DISP_PROPERTY_ID(CXTPReportRecordItemEditOptions, "SelectTextOnEdit", 6, m_bSelectTextOnEdit, VT_BOOL)
	DISP_PROPERTY_ID(CXTPReportRecordItemEditOptions, "EditControlStyle", 7, m_dwEditStyle, VT_I4)
	DISP_PROPERTY_ID(CXTPReportRecordItemEditOptions, "MaxLength", 8, m_nMaxLength, VT_I4)
	DISP_FUNCTION_ID(CXTPReportRecordItemEditOptions, "RemoveButtons", 9, RemoveButtons, VT_EMPTY, VTS_NONE)

	DISP_FUNCTION_ID(CXTPReportRecordItemEditOptions, "GetInplaceButton", 10, OleGetInplaceButton, VT_DISPATCH, VTS_I4)
	DISP_FUNCTION_ID(CXTPReportRecordItemEditOptions, "InplaceButtonsCount", 11, OleInplaceButtonsCount, VT_I4, VTS_NONE)

	DISP_FUNCTION_ID(CXTPReportRecordItemEditOptions, "AddSpinButton", 12, OleAddSpinButton, VT_DISPATCH, VTS_VARIANT)

	DISP_PROPERTY_ID(CXTPReportRecordItemEditOptions, "ExpandOnSelect", 13, m_bExpandOnSelect, VT_BOOL)
END_DISPATCH_MAP()

// {E17BCC77-27BF-4cb1-9ABF-1788D9835223}
static const GUID IID_IReportRecordItemEditOptions =
{ 0xe17bcc77, 0x27bf, 0x4cb1, { 0x9a, 0xbf, 0x17, 0x88, 0xd9, 0x83, 0x52, 0x23 } };

BEGIN_INTERFACE_MAP(CXTPReportRecordItemEditOptions, CCmdTarget)
	INTERFACE_PART(CXTPReportRecordItemEditOptions, IID_IReportRecordItemEditOptions, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPReportRecordItemEditOptions, IID_IReportRecordItemEditOptions)

LPDISPATCH CXTPReportRecordItemEditOptions::OleGetConstraints()
{
	return m_pConstraints->GetIDispatch(TRUE);

}

LPDISPATCH CXTPReportRecordItemEditOptions::OleGetInplaceButton(long nIndex)
{
	int nCount = arrInplaceButtons.GetSize();
	if (nIndex < nCount && nCount >= 0)
	{
		return arrInplaceButtons[nIndex] ? arrInplaceButtons[nIndex]->GetIDispatch(TRUE) : NULL;
	}
	return NULL;
}

long CXTPReportRecordItemEditOptions::OleInplaceButtonsCount()
{
	return arrInplaceButtons.GetSize();
}

LPDISPATCH CXTPReportRecordItemEditOptions::OleAddComboButton(const VARIANT& bInside)
{
	BOOL bIn = VariantToBool(&bInside);
	return XTPGetDispatch(AddComboButton(bIn));
}

LPDISPATCH CXTPReportRecordItemEditOptions::OleAddExpandButton(const VARIANT& bInside)
{
	BOOL bIn = VariantToBool(&bInside);
	return XTPGetDispatch(AddExpandButton(bIn));
}

LPDISPATCH CXTPReportRecordItemEditOptions::OleAddSpinButton(const VARIANT& bInside)
{
	BOOL bIn = VariantToBool(&bInside);
	return XTPGetDispatch(AddSpinButton(bIn));
}

#endif // _XTP_ACTIVEX
