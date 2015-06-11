// XTPReportRecordItemConstraint.cpp : implementation of the CXTPReportRecordItemConstraint class.
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

//////////////////////////////////////////////////////////////////////////
// CXTPReportRecordItemConstraint

CXTPReportRecordItemConstraint::CXTPReportRecordItemConstraint()
{
#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif

	m_dwData = 0;
	m_nIndex = 0;
}

int CXTPReportRecordItemConstraint::GetIndex() const
{
	return m_nIndex;
}

//////////////////////////////////////////////////////////////////////////
// CXTPReportRecordItemConstraints

CXTPReportRecordItemConstraints::CXTPReportRecordItemConstraints()
{
#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPReportRecordItemConstraints::~CXTPReportRecordItemConstraints()
{
	RemoveAll();
}


int CXTPReportRecordItemConstraints::GetCount() const
{
	return (int)m_arrConstraints.GetSize();
}

CXTPReportRecordItemConstraint* CXTPReportRecordItemConstraints::GetAt(int nIndex) const
{
	return m_arrConstraints.GetAt(nIndex);
}

void CXTPReportRecordItemConstraints::RemoveAll()
{
	for (int i = 0; i < GetCount(); i++)
	{
		if (m_arrConstraints[i] != NULL)
			m_arrConstraints[i]->InternalRelease();
	}
	m_arrConstraints.RemoveAll();
}

#ifdef _XTP_ACTIVEX

//////////////////////////////////////////////////////////////////////////
// CXTPReportRecordItemConstraint

BEGIN_DISPATCH_MAP(CXTPReportRecordItemConstraint, CCmdTarget)
	DISP_PROPERTY_ID(CXTPReportRecordItemConstraint, "Caption", 1, m_strConstraint, VT_BSTR)
	DISP_PROPERTY_ID(CXTPReportRecordItemConstraint, "Data", 2, m_dwData, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPReportRecordItemConstraint, "Index", 3, GetIndex, SetNotSupported, VT_I4)
END_DISPATCH_MAP()

// {EB97BA3A-FE51-4e7e-B3C0-A798DDD174E9}
static const GUID IID_IReportRecordItemConstraint =
{ 0xeb97ba3a, 0xfe51, 0x4e7e, { 0xb3, 0xc0, 0xa7, 0x98, 0xdd, 0xd1, 0x74, 0xe9 } };

BEGIN_INTERFACE_MAP(CXTPReportRecordItemConstraint, CCmdTarget)
	INTERFACE_PART(CXTPReportRecordItemConstraint, IID_IReportRecordItemConstraint, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPReportRecordItemConstraint, IID_IReportRecordItemConstraint)

//////////////////////////////////////////////////////////////////////////
// CXTPReportRecordItemConstraints

BEGIN_DISPATCH_MAP(CXTPReportRecordItemConstraints, CCmdTarget)
	DISP_FUNCTION_ID(CXTPReportRecordItemConstraints, "Count", dispidCount, OleGetItemCount, VT_I4, VTS_NONE)
	DISP_FUNCTION_ID(CXTPReportRecordItemConstraints, "Constraint", DISPID_VALUE, OleGetItem, VT_DISPATCH, VTS_I4)
	DISP_FUNCTION_ID(CXTPReportRecordItemConstraints, "_NewEnum", DISPID_NEWENUM, OleNewEnum, VT_UNKNOWN, VTS_NONE)
	DISP_FUNCTION_ID(CXTPReportRecordItemConstraints, "Add", dispidAdd, OleAdd, VT_EMPTY, VTS_BSTR VTS_I4)
	DISP_FUNCTION_ID(CXTPReportRecordItemConstraints, "DeleteAll", 5, RemoveAll, VT_EMPTY, VTS_NONE)

END_DISPATCH_MAP()

// {E87BCC77-27BF-4cb1-9ABF-1788D9835223}
static const GUID IID_IReportRecordItemConstraints =
{ 0xE87bcc77, 0x27bf, 0x4cb1, { 0x9a, 0xbf, 0x17, 0x88, 0xd9, 0x83, 0x52, 0x23 } };

BEGIN_INTERFACE_MAP(CXTPReportRecordItemConstraints, CCmdTarget)
	INTERFACE_PART(CXTPReportRecordItemConstraints, IID_IReportRecordItemConstraints, Dispatch)
	//INTERFACE_PART(CXTPReportRecordItemConstraints, IID_IEnumVARIANT, EnumVARIANT)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPReportRecordItemConstraints, IID_IReportRecordItemConstraints)

int CXTPReportRecordItemConstraints::OleGetItemCount()
{
	return GetCount();
}

LPDISPATCH CXTPReportRecordItemConstraints::OleGetItem(long nIndex)
{
	if (nIndex >= 0 && nIndex < GetCount())
	{
		return GetAt(nIndex)->GetIDispatch(TRUE);
	}
	AfxThrowOleException(E_INVALIDARG);
	return 0;
}

IMPLEMENT_ENUM_VARIANT(CXTPReportRecordItemConstraints)

void CXTPReportRecordItemConstraints::OleAdd(LPCTSTR lpszCaption, long dwData)
{

	CXTPReportRecordItemConstraint* pConstaint = new CXTPReportRecordItemConstraint();
	pConstaint->m_strConstraint = lpszCaption;
	pConstaint->m_dwData = dwData;
	pConstaint->m_nIndex = GetCount();

	m_arrConstraints.Add(pConstaint);
}

#endif
