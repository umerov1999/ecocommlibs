// XTPPropertyGridItemNumber.cpp : implementation of the CXTPPropertyGridItemNumber class.
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

#include "StdAfx.h"
#include "Common/XTPSystemHelpers.h"

#include "XTPPropertyGridItem.h"
#include "XTPPropertyGridItemNumber.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CXTPPropertyGridItemNumber
IMPLEMENT_DYNAMIC(CXTPPropertyGridItemNumber, CXTPPropertyGridItem)


CXTPPropertyGridItemNumber::CXTPPropertyGridItemNumber(LPCTSTR strCaption, long nValue, long* pBindNumber)
	: CXTPPropertyGridItem(strCaption)
{
	m_pBindNumber = pBindNumber;
	SetNumber(nValue);
	m_strDefaultValue = m_strValue;
}
CXTPPropertyGridItemNumber::CXTPPropertyGridItemNumber(UINT nID, long nValue, long* pBindNumber)
	: CXTPPropertyGridItem(nID)
{
	m_pBindNumber = pBindNumber;
	SetNumber(nValue);
	m_strDefaultValue = m_strValue;
}

CXTPPropertyGridItemNumber::~CXTPPropertyGridItemNumber()
{
}

/////////////////////////////////////////////////////////////////////////////
//

void CXTPPropertyGridItemNumber::SetValue(CString strValue)
{
	SetNumber(_ttol(strValue));
}

void CXTPPropertyGridItemNumber::SetNumber(long nValue)
{
	m_nValue = nValue;

	if (m_pBindNumber)
	{
		*m_pBindNumber = m_nValue;
	}

	CString strValue;
	strValue.Format(_T("%i"), nValue);
	CXTPPropertyGridItem::SetValue(strValue);
}

void CXTPPropertyGridItemNumber::BindToNumber(long* pBindNumber)
{
	m_pBindNumber = pBindNumber;
	if (m_pBindNumber)
	{
		*m_pBindNumber = m_nValue;
	}
}

void CXTPPropertyGridItemNumber::OnBeforeInsert()
{
	if (m_pBindNumber && *m_pBindNumber != m_nValue)
	{
		SetNumber(*m_pBindNumber);
	}
}

#ifdef _XTP_ACTIVEX

void CXTPPropertyGridItemNumber::OleSetValue(const VARIANT* varValue)
{
	SetNumber(VariantToLong(varValue, 0));
}
const VARIANT CXTPPropertyGridItemNumber::OleGetValue()
{
	return CComVariant(m_nValue);
}

void CXTPPropertyGridItemNumber::BindDispatch()
{
	if (m_dispDriverBinded.m_lpDispatch)
	{
		m_dispDriverBinded.SetProperty(m_dispidBinded, VT_I4, m_nValue);
	}
}

#endif




/////////////////////////////////////////////////////////////////////////////
// CXTPPropertyGridItemDouble
IMPLEMENT_DYNAMIC(CXTPPropertyGridItemDouble, CXTPPropertyGridItem)


CXTPPropertyGridItemDouble::CXTPPropertyGridItemDouble(LPCTSTR strCaption, double fValue, LPCTSTR strFormat, double* pBindDouble)
	: CXTPPropertyGridItem(strCaption)
{
	m_pBindDouble = pBindDouble;
	m_strFormat = strFormat;
	m_bUseSystemDecimalSymbol = m_strFormat.IsEmpty();

	SetDouble(fValue);
	m_strDefaultValue = m_strValue;

	EnableAutomation();
}

CXTPPropertyGridItemDouble::CXTPPropertyGridItemDouble(UINT nID, double fValue, LPCTSTR strFormat, double* pBindDouble)
	: CXTPPropertyGridItem(nID)
{
	m_pBindDouble = pBindDouble;
	m_strFormat = strFormat;
	m_bUseSystemDecimalSymbol = m_strFormat.IsEmpty();

	SetDouble(fValue);
	m_strDefaultValue = m_strValue;

	EnableAutomation();
}

CXTPPropertyGridItemDouble::~CXTPPropertyGridItemDouble()
{
}

/////////////////////////////////////////////////////////////////////////////
//

void CXTPPropertyGridItemDouble::SetValue(CString strValue)
{
	SetDouble(StringToDouble(strValue));
}

double CXTPPropertyGridItemDouble::StringToDouble(LPCTSTR strValue)
{
	if (m_bUseSystemDecimalSymbol)
	{
		TRY
		{
			COleVariant oleString(strValue);
			oleString.ChangeType(VT_R8);
			return oleString.dblVal;
		}
		CATCH(COleException, e)
		{
		}
		END_CATCH
	}

#ifdef _UNICODE
	char astring[20];
	WideCharToMultiByte (CP_ACP, 0, strValue, -1, astring, 20, NULL, NULL);
	return (double)atof(astring);
#else
	return (double)atof(strValue);
#endif
}

CString CXTPPropertyGridItemDouble::DoubleToString(double dValue)
{
	if (m_bUseSystemDecimalSymbol)
	{
		TRY
		{
			COleVariant oleString(dValue);
			oleString.ChangeType(VT_BSTR);
			return CString(oleString.bstrVal);
		}
		CATCH(COleException, e)
		{
		}
		END_CATCH
	}

	CString strFormat(m_strFormat);
	if (strFormat.IsEmpty())
		strFormat = _T("%0.2f");

	CString strValue;
	strValue.Format(strFormat, dValue);

	return strValue;
}

void CXTPPropertyGridItemDouble::SetDouble(double fValue)
{
	m_fValue = fValue;

	if (m_pBindDouble)
	{
		*m_pBindDouble = m_fValue;
	}

	CXTPPropertyGridItem::SetValue(DoubleToString(fValue));
}

void CXTPPropertyGridItemDouble::BindToDouble(double* pBindNumber)
{
	m_pBindDouble = pBindNumber;
	if (m_pBindDouble)
	{
		*m_pBindDouble = m_fValue;
	}
}

void CXTPPropertyGridItemDouble::OnBeforeInsert()
{
	if (m_pBindDouble && *m_pBindDouble != m_fValue)
	{
		SetDouble(*m_pBindDouble);
	}
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPPropertyGridItemDouble, CXTPPropertyGridItem)
	DISP_PROPERTY_ID(CXTPPropertyGridItemDouble, "UseSystemDecimalSymbol", 50, m_bUseSystemDecimalSymbol, VT_BOOL)
END_DISPATCH_MAP()

// {C4DD39EA-6750-4257-A80A-A33F44022395}
static const GUID IID_PropertyGridItemDouble =
{ 0xc4dd39ea, 0x6750, 0x4257, { 0xa8, 0xa, 0xa3, 0x3f, 0x44, 0x2, 0x23, 0x95 } };


BEGIN_INTERFACE_MAP(CXTPPropertyGridItemDouble, CXTPPropertyGridItem)
	INTERFACE_PART(CXTPPropertyGridItemDouble, IID_PropertyGridItemDouble, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPPropertyGridItemDouble, IID_PropertyGridItemDouble)


void CXTPPropertyGridItemDouble::OleSetValue(const VARIANT* varValue)
{
	if (varValue->vt == (VT_VARIANT | VT_BYREF))
	{
		OleSetValue(varValue->pvarVal);
		return;
	}
	if (varValue->vt == VT_BSTR)
	{
		SetDouble(StringToDouble(CString(varValue->bstrVal)));
	}
	else if (varValue->vt == VT_CY)
	{
		double dValue;
		VarR8FromCy(V_CY(varValue), &dValue);
		SetDouble(dValue);
	}
	else if (varValue->vt == VT_R8) SetDouble(V_R8(varValue));
	else if (varValue->vt == VT_R4) SetDouble((double)V_R4(varValue));
	else if (varValue->vt == (VT_R8 | VT_BYREF)) SetDouble(*V_R8REF(varValue));
	else if (varValue->vt == (VT_R4 | VT_BYREF)) SetDouble((double)*V_R4REF(varValue));
	else
	{
		VARIANT var;
		::VariantInit(&var);
		if (SUCCEEDED(VariantChangeType(&var, (VARIANT*)varValue, 0, VT_R8)))
		{
			SetDouble(V_R8(&var));
		}
		else
		{
			SetDouble((double)0);
		}
		::VariantClear(&var);
	}
}
const VARIANT CXTPPropertyGridItemDouble::OleGetValue()
{
	return CComVariant(m_fValue);
}

void CXTPPropertyGridItemDouble::BindDispatch()
{
	if (m_dispDriverBinded.m_lpDispatch)
	{
		m_dispDriverBinded.SetProperty(m_dispidBinded, VT_R8, m_fValue);
	}
}

#endif
