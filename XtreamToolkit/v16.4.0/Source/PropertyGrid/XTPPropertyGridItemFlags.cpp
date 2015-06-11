// XTPPropertyGridItemFlags.cpp interface for the CXTPPropertyGridItemFlags class.
//
// This file is a part of the XTREME PROPERTYGRID MFC class library.
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
#include "Resource.h"

#include "Common/XTPResourceManager.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPColorManager.h"
#include <Common/ScrollBar/XTPScrollBase.h>
#include <Common/ScrollBar/XTPScrollBarCtrl.h>
#include <Common/ScrollBar/XTPWnd.h>

#include "XTPPropertyGridDefines.h"
#include "XTPPropertyGridItem.h"
#include "XTPPropertyGridItemFlags.h"
#include "XTPPropertyGridItemBool.h"
#include "XTPPropertyGridPaintManager.h"
#include "XTPPropertyGridToolTip.h"
#include "XTPPropertyGridView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

class CXTPPropertyGridItemFlags::CXTPPropertyGridItemFlag : public CXTPPropertyGridItemBool
{
public:

	CXTPPropertyGridItemFlag(CString strCaption, DWORD dwFlag)
		:  CXTPPropertyGridItemBool(strCaption), m_dwFlag(dwFlag)
	{
	}

	void OnValueChanged(CString strValue);

	virtual BOOL GetReadOnly() const
	{
		return m_pParent->GetReadOnly();
	}

	DWORD m_dwFlag;
};

void CXTPPropertyGridItemFlags::CXTPPropertyGridItemFlag::OnValueChanged(CString strValue)
{
	SetValue(strValue);

	CXTPPropertyGridItemFlags* pParent = DYNAMIC_DOWNCAST(CXTPPropertyGridItemFlags, m_pParent);
	ASSERT(pParent);
	if (!pParent)
		return;

	if (GetBool())
		pParent->m_nValue |= m_dwFlag;
	else
		pParent->m_nValue &= ~m_dwFlag;

	pParent->OnValueChanged(pParent->GetFlagsString());

}

IMPLEMENT_DYNAMIC(CXTPPropertyGridItemFlags, CXTPPropertyGridItem)

CXTPPropertyGridItemFlags::CXTPPropertyGridItemFlags(LPCTSTR strCaption, int nValue, int* pBindFlags)
: CXTPPropertyGridItem(strCaption)
{
	m_pBindFlags = pBindFlags;
	_Init(nValue);

}

CXTPPropertyGridItemFlags::CXTPPropertyGridItemFlags(UINT nID, int nValue, int* pBindFlags)
: CXTPPropertyGridItem(nID)
{
	m_pBindFlags = pBindFlags;
	_Init(nValue);
}

CXTPPropertyGridItemFlags::~CXTPPropertyGridItemFlags()
{
}

void CXTPPropertyGridItemFlags::_Init(int nValue)
{
	SetFlags(nValue);
	m_nFlags = xtpGridItemHasEdit;
	m_strDefaultValue = m_strValue;
}

BOOL CXTPPropertyGridItemFlags::HasFlag(const CString& strValue, CString strFlag) const
{
	strFlag.MakeLower();
	int nIndex = -1;

	do
	{
		nIndex = strValue.Find(strFlag, nIndex + 1);
		if (nIndex == -1)
			return FALSE;

		TCHAR chLast = nIndex + strFlag.GetLength() == strValue.GetLength() ? _T(']') : strValue[strFlag.GetLength() + nIndex];
		TCHAR chFirst = nIndex == 0 ? _T('[') : strValue[nIndex - 1];

		if ((chLast == ',' || chLast == ';' || chLast == ']')
			&& (chFirst == ',' || chFirst == ';' || chFirst == '['))
		{
			return TRUE;
		}
	}
	while (nIndex != -1);

	return FALSE;
}

void CXTPPropertyGridItemFlags::SetValue(CString strValue)
{
	int nValue = 0;
	strValue.MakeLower();

	CXTPPropertyGridItemConstraints* pConstraints = GetConstraints();
	for (int i = 0; i < pConstraints->GetCount(); i++)
	{
		if (HasFlag(strValue, pConstraints->GetAt(i)))
			nValue |= pConstraints->GetConstraintAt(i)->m_dwData;
	}

	SetFlags(nValue);
}

void CXTPPropertyGridItemFlags::SetFlags(int nValue)
{
	m_nValue = nValue;

	if (m_pBindFlags)
	{
		*m_pBindFlags = nValue;
	}

	UpdateChilds();

	CXTPPropertyGridItem::SetValue(GetFlagsString());
}

void CXTPPropertyGridItemFlags::BindToFlags(int* pBindFlags)
{
	m_pBindFlags = pBindFlags;
	if (m_pBindFlags)
	{
		*m_pBindFlags = m_nValue;
	}
}

CString CXTPPropertyGridItemFlags::GetFlagsString()
{
	CString str;

	CXTPPropertyGridItemConstraints* pConstraints = GetConstraints();
	int nValue = 0;

	for (int i = 0; i < pConstraints->GetCount(); i++)
	{
		CXTPPropertyGridItemConstraint* pConstraint = pConstraints->GetConstraintAt(i);

		if ((nValue & pConstraint->m_dwData) == pConstraint->m_dwData)
			continue;

		if ((m_nValue & pConstraint->m_dwData) == pConstraint->m_dwData)
		{
			str += (str.IsEmpty() ? _T("") : _T(";")) + pConstraint->m_strConstraint;
			nValue |= pConstraint->m_dwData;
		}
	}

	return  _T("[") + str + _T("]");
}

void CXTPPropertyGridItemFlags::UpdateChilds()
{
	CXTPPropertyGridItems* pItems = GetChilds();
	for (int i = 0; i < pItems->GetCount(); i++)
	{
		CXTPPropertyGridItemFlag* pItem = (CXTPPropertyGridItemFlag*)pItems->GetAt(i);
		pItem->SetBool((m_nValue & pItem->m_dwFlag) == pItem->m_dwFlag);
		pItem->SetReadOnly(GetReadOnly());
	}
}


void CXTPPropertyGridItemFlags::OnBeforeInsert()
{
	if (m_pBindFlags && *m_pBindFlags != m_nValue)
	{
		SetFlags(*m_pBindFlags);
	}
}

void CXTPPropertyGridItemFlags::OnConstraintsChanged()
{
	GetChilds()->Clear();

	CXTPPropertyGridItemConstraints* pConstraints = GetConstraints();

	int i;

	for (i = 0; i < pConstraints->GetCount(); i++)
	{
		AddChildItem(new CXTPPropertyGridItemFlag(pConstraints->GetAt(i), (int)pConstraints->GetConstraintAt(i)->m_dwData));
	}
	UpdateChilds();
	m_strDefaultValue = m_strValue = GetFlagsString();

	CXTPPropertyGridItems* pItems = GetChilds();
	for (i = 0; i < pItems->GetCount(); i++)
	{
		CXTPPropertyGridItemFlag* pItem = (CXTPPropertyGridItemFlag*)pItems->GetAt(i);
		pItem->SetDefaultValue(pItem->GetValue());
	}

}

void CXTPPropertyGridItemFlags::SetReadOnly(BOOL bReadOnly)
{
	CXTPPropertyGridItem::SetReadOnly(bReadOnly);

	UpdateChilds();
}


#ifdef _XTP_ACTIVEX

void CXTPPropertyGridItemFlags::OleSetValue(const VARIANT* varValue)
{
	SetFlags(VariantToLong(varValue));
}

const VARIANT CXTPPropertyGridItemFlags::OleGetValue()
{
	return CComVariant(m_nValue);
}

void CXTPPropertyGridItemFlags::BindDispatch()
{
	if (m_dispDriverBinded.m_lpDispatch)
	{
		m_dispDriverBinded.SetProperty(m_dispidBinded, VT_I4, m_nValue);
	}
}

#endif