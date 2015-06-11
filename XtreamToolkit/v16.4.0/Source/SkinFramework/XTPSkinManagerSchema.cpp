// XTPSkinManagerSchema.cpp: implementation of the CXTPSkinManagerSchema class.
//
// This file is a part of the XTREME SKINFRAMEWORK MFC class library.
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

#include <Common/Tmschema.h>
#include <Common/XTPSystemHelpers.h>
#include <Common/XTPColorManager.h>
#include <Common/XTPVC80Helpers.h>
#include <Common/XTPDrawHelpers.h>
#include <Common/ScrollBar/XTPScrollBase.h>
#include <Common/XTPSynchro.h>

#include "XTPSkinManager.h"
#include "XTPSkinManagerSchema.h"
#include "XTPSkinManagerSchemaCodes.h"
#include "XTPSkinManagerResource.h"
#include "XTPSkinImage.h"
#include "XTPSkinObject.h"
#include "XTPSkinObjectFrame.h"
#include "XTPSkinDrawTools.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#ifndef OIC_WINLOGO
#define OIC_WINLOGO         32517
#endif

#ifndef LAYOUT_BITMAPORIENTATIONPRESERVED
#define LAYOUT_BITMAPORIENTATIONPRESERVED 0x00000008
#endif

void InflateBorders(CRect& rcBorders, int cxFrame, int cyFrame)
{
	rcBorders.left += cxFrame;
	rcBorders.right +=  cxFrame;

	rcBorders.top += cyFrame;
	rcBorders.bottom +=  cyFrame;
}


CXTPSkinManagerSchemaProperty::CXTPSkinManagerSchemaProperty()
{
	propType = XTP_SKINPROPERTY_UNKNOWN;
	nPropertyCode = 0;
	ZeroMemory(&rcVal, sizeof(RECT));
}

CXTPSkinManagerSchemaProperty::~CXTPSkinManagerSchemaProperty()
{
	ClearProperty();
}

void CXTPSkinManagerSchemaProperty::ClearProperty()
{
	if (propType == XTP_SKINPROPERTY_STRING)
		delete[] lpszVal;

	if (propType == XTP_SKINPROPERTY_FONT)
		delete lfVal;

	propType = XTP_SKINPROPERTY_UNKNOWN;
}

void CXTPSkinManagerSchemaProperty::SetPropertyColor(LPCTSTR lpszValue)
{
	ClearProperty();
	int r = 0, g = 0, b = 0;
	SCANF_S(lpszValue, _T("%i %i %i"), &r, &g, &b);
	clrVal = RGB(r, g, b);
}

void CXTPSkinManagerSchemaProperty::SetPropertyString(LPCTSTR lpszValue)
{
	ClearProperty();

	int nLen = (int)_tcslen(lpszValue);
	lpszVal = new TCHAR[nLen + 1];
	STRCPY_S(lpszVal, nLen + 1, lpszValue);

	propType = XTP_SKINPROPERTY_STRING;
}

void CXTPSkinManagerSchemaProperty::SetPropertyBool(LPCTSTR lpszValue)
{
	ClearProperty();
	bVal = _tcsicmp(lpszValue, _T("TRUE")) == 0;
}

void CXTPSkinManagerSchemaProperty::SetPropertyInt(LPCTSTR lpszValue)
{
	ClearProperty();
	iVal = _ttoi(lpszValue);
}

void CXTPSkinManagerSchemaProperty::SetPropertyEnum(int nEnumValue)
{
	ClearProperty();
	iVal = nEnumValue;
}


void CXTPSkinManagerSchemaProperty::SetPropertySize(LPCTSTR lpszValue)
{
	ClearProperty();
	SCANF_S(lpszValue, _T("%i,%i"), &szVal.cx, &szVal.cy);
}

void CXTPSkinManagerSchemaProperty::SetPropertyRect(LPCTSTR lpszValue)
{
	ClearProperty();
	SCANF_S(lpszValue, _T("%ld,%ld,%ld,%ld"), &rcVal.left, &rcVal.right, &rcVal.top, &rcVal.bottom);
}

void CXTPSkinManagerSchemaProperty::SetPropertyFont(LPCTSTR lpszValue)
{
	ClearProperty();
	lfVal = new LOGFONT;
	ZeroMemory(lfVal, sizeof(LOGFONT));

	CWindowDC dc (NULL);
	lfVal->lfCharSet = (BYTE)::GetTextCharsetInfo (dc, NULL, 0);

	TCHAR chBold[50];
	chBold[0] = 0;
	SCANF_S(lpszValue, _T("%[^,], %ld, %s"), SCANF_PARAM_S(lfVal->lfFaceName, LF_FACESIZE), &lfVal->lfHeight, SCANF_PARAM_S(chBold, 50));
	lfVal->lfWeight = _tcsicmp(chBold, _T("BOLD")) != 0?  FW_NORMAL: FW_BOLD;

	int ppi = dc.GetDeviceCaps(LOGPIXELSY);

	lfVal->lfHeight = - MulDiv(lfVal->lfHeight, ppi, 72);
}

BOOL CXTPSkinManagerSchemaProperty::operator==(const CXTPSkinManagerSchemaProperty& prop) const
{
	if (&prop == this)
		return TRUE;

	if (prop.propType != propType)
		return FALSE;

	switch (propType)
	{
		case XTP_SKINPROPERTY_COLOR:    return clrVal == prop.clrVal;
		case XTP_SKINPROPERTY_STRING:   return _tcsicmp(lpszVal, prop.lpszVal) == 0;
		case XTP_SKINPROPERTY_BOOL:     return bVal == prop.bVal;
		case XTP_SKINPROPERTY_RECT:     return ::EqualRect(&prop.rcVal, &rcVal);
		case XTP_SKINPROPERTY_INT:      return iVal == prop.iVal;
		case XTP_SKINPROPERTY_POSITION: return (szVal.cx == prop.szVal.cx) && (szVal.cy == prop.szVal.cy);
		case XTP_SKINPROPERTY_ENUM:     return iVal == prop.iVal;
		case XTP_SKINPROPERTY_FONT:     return memcmp(lfVal, prop.lfVal, sizeof(LOGFONT)) == 0;
	}

	return FALSE;
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXTPSkinManagerSchema::CXTPSkinManagerSchema(CXTPSkinManagerResourceFile* pResourceFile)
{
	::InitializeCriticalSection(&m_csDraw);

	m_pResourceFile = pResourceFile;
	CMDTARGET_ADDREF(m_pResourceFile);

	m_pManager = pResourceFile->GetSkinManager();

	m_bPreMultiplyImages = TRUE;

	m_nGlobalClassId = GetClassCode(_T("GLOBALS"));
	m_nWindowClassId = GetClassCode(_T("WINDOW"));

	m_mapProperties.InitHashTable(199, FALSE);

	m_pMetrics = new CXTPSkinManagerMetrics(this);


#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPSkinManagerSchema::~CXTPSkinManagerSchema()
{
	RemoveAllProperties();

	FreeFrameRegions();

	POSITION pos = m_mapClasses.GetStartPosition();
	CString strClassName;
	CXTPSkinManagerClass* pClass;
	while (pos != NULL)
	{
		m_mapClasses.GetNextAssoc( pos, strClassName, (void*&)pClass);
		delete pClass;
	}
	m_mapClasses.RemoveAll();

	SAFE_DELETE(m_pMetrics);

	CMDTARGET_RELEASE(m_pResourceFile);

	::DeleteCriticalSection(&m_csDraw);
}

void CXTPSkinManagerSchema::RemoveAllProperties()
{
	POSITION pos = m_mapProperties.GetStartPosition();
	UINT nProperty;
	CXTPSkinManagerSchemaProperty* pProperty;
	while (pos != NULL)
	{
		m_mapProperties.GetNextAssoc( pos, nProperty, pProperty);
		delete pProperty;
	}
	m_mapProperties.RemoveAll();

}

#ifdef _OLD_METHOD_GET_TMS_CODES_VERIFY

int CXTPSkinManagerSchema::GetEnumCodeOld(LPCTSTR lpszProperty, LPCTSTR lpszValue)
{
	#include "XTPSkinManagerSchema.inc"
	#undef BEGIN_TM_ENUM
	#undef TM_ENUM
	#undef END_TM_ENUM
	#define BEGIN_TM_ENUM(name)            if (_tcsicmp(_T(#name), lpszProperty) == 0) {
	#define TM_ENUM(val, prefix, name)     if (_tcsicmp(_T(#name), lpszValue) == 0) return val;
	#define END_TM_ENUM()                      }
	#include "Common/Tmschema.h"

	return -1;
}

UINT CXTPSkinManagerSchema::GetClassCodeOld(LPCTSTR lpszClass)
{
	if (_tcsicmp(_T("Globals"), lpszClass) == 0) return 1;
	if (_tcsicmp(_T("Documentation"), lpszClass) == 0) return 2;
	if (_tcsicmp(_T("SysMetrics"), lpszClass) == 0) return 3;
	if (_tcsicmp(_T("ListBox"), lpszClass) == 0) return 4;
	if (_tcsicmp(_T("Dialog"), lpszClass) == 0) return 5;

	UINT nClass = 5;

	#include "XTPSkinManagerSchema.inc"
	#undef BEGIN_TM_CLASS_PARTS
	#define BEGIN_TM_CLASS_PARTS(name)            nClass++; if (_tcsicmp(_T(#name), lpszClass) == 0) return nClass;
	#include "Common/Tmschema.h"

	TRACE(_T("Warning: %s class not supported"), lpszClass);
	return 0; // Not supported
}

UINT CXTPSkinManagerSchema::GetClassPartCodeOld(LPCTSTR lpszClass, LPCTSTR lpszClassPart)
{
	#include "XTPSkinManagerSchema.inc"
	#undef BEGIN_TM_CLASS_PARTS
	#undef TM_PART
	#undef END_TM_CLASS_PARTS
	#define BEGIN_TM_CLASS_PARTS(name)          if (_tcsicmp(_T(#name), lpszClass) == 0) {
	#define TM_PART(val, prefix, name)          if (_tcsicmp(_T(#name), lpszClassPart) == 0) return val;
	#define END_TM_CLASS_PARTS()                }
	#include "Common/Tmschema.h"

	return 0;
}

UINT CXTPSkinManagerSchema::GetStateCodeOld(LPCTSTR lpszClassPart, LPCTSTR lpszState)
{
	#include "XTPSkinManagerSchema.inc"
	#undef BEGIN_TM_PART_STATES
	#undef TM_STATE
	#undef END_TM_PART_STATES
	#define BEGIN_TM_PART_STATES(name)          if (_tcsicmp(_T(#name), lpszClassPart) == 0) {
	#define TM_STATE(val, prefix, name)         if (_tcsicmp(_T(#name), lpszState) == 0) return val;
	#define END_TM_PART_STATES()                }
	#include "Common/Tmschema.h"

	return 0;
}

UINT CXTPSkinManagerSchema::GetPropertyCodeOld(LPCTSTR lpszProperty, XTPSkinManagerProperty& nPropertyType)
{
#include "XTPSkinManagerSchema.inc"
#undef TM_PROP
#define TM_PROP(val, prefix, name, primval)   if (_tcsicmp(_T(#name), lpszProperty) == 0) { nPropertyType = XTP_SKINPROPERTY_##primval; return val; }
#include "Common/Tmschema.h"

	ASSERT(FALSE);

	return 0;
}

#endif // _OLD_METHOD_GET_TMS_CODES_VERIFY

UINT CXTPSkinManagerSchema::GetClassCode(LPCTSTR lpszClass)
{
	UINT nClass = GET_CLASS_CODE(lpszClass);

	if (nClass == -1)
		nClass = 0; // Not supported, old behaviour

#ifdef _OLD_METHOD_GET_TMS_CODES_VERIFY
	UINT nClassOld = GetClassCodeOld(lpszClass);
	ASSERT(nClassOld == nClass);
#endif

	if (!nClass)
	{
		TRACE(_T("Warning: %s class not supported"), lpszClass);
	}

	return nClass;
}

int CXTPSkinManagerSchema::GetEnumCode(LPCTSTR lpszProperty, LPCTSTR lpszValue)
{
	int nEnumCode = GET_ENUM_CODE(lpszProperty, lpszValue);

#ifdef _OLD_METHOD_GET_TMS_CODES_VERIFY

	int nEnumCodeOld = GetEnumCodeOld(lpszProperty, lpszValue);
	ASSERT(nEnumCodeOld == nEnumCode);

#endif

	return nEnumCode;
}

UINT CXTPSkinManagerSchema::GetClassPartCode(LPCTSTR lpszClass, LPCTSTR lpszClassPart)
{
	int nClassPartCode = GET_CLASSPART_CODE(lpszClass, lpszClassPart);

	if (nClassPartCode == -1)
		nClassPartCode = 0; // not found, old behaviour

#ifdef _OLD_METHOD_GET_TMS_CODES_VERIFY

	int nClassPartCodeOld = GetClassPartCodeOld(lpszClass, lpszClassPart);
	ASSERT(nClassPartCodeOld == nClassPartCode);

#endif

	return nClassPartCode;
}

UINT CXTPSkinManagerSchema::GetStateCode(LPCTSTR lpszClassPart, LPCTSTR lpszState)
{
	int nStateCode = GET_STATE_CODE(lpszClassPart, lpszState);

	if (nStateCode == -1)
		nStateCode = 0; // not found, old behaviour

#ifdef _OLD_METHOD_GET_TMS_CODES_VERIFY

	int nStateCodeOld = GetStateCodeOld(lpszClassPart, lpszState);
	ASSERT(nStateCodeOld == nStateCode);

#endif

	return nStateCode;
}


UINT CXTPSkinManagerSchema::GetPropertyCode(LPCTSTR lpszProperty, XTPSkinManagerProperty& nPropertyType)
{
	int nPropertyCode = GET_PROPERTY_CODE(lpszProperty);

	if (nPropertyCode == -1)
	{
		nPropertyCode = 0; // not found, old behaviour

		nPropertyType = XTP_SKINPROPERTY_UNKNOWN;
	}
	else
	{
		nPropertyType = (XTPSkinManagerProperty)(nPropertyCode - (nPropertyCode/10*10) );

		nPropertyCode /= 10;
	}

#ifdef _OLD_METHOD_GET_TMS_CODES_VERIFY

	XTPSkinManagerProperty propTemp = XTP_SKINPROPERTY_UNKNOWN;

	int nPropertyCodeOld = GetPropertyCodeOld(lpszProperty, propTemp);
	ASSERT(nPropertyCodeOld == nPropertyCode);
	ASSERT(propTemp == nPropertyType);

#endif

	return nPropertyCode;
}

UINT CXTPSkinManagerSchema::CalculatePropertyCode(UINT iClassId, int iPartId, int iStateId, int iPropId)
{
	return iPropId + ((iClassId + ((iPartId + (iStateId << 6 )) << 6)) << 14);
}

UINT CXTPSkinManagerSchema::GetSchemaPartCode(CString strSchemaPart)
{
	if (strSchemaPart.Find(_T(':')) != -1)
		return 0; // Not Supported;

	strSchemaPart.Remove(_T(' '));

	CString strClass, strClassPart, strState;

	int nStateIndex = strSchemaPart.Find(_T('('));
	if (nStateIndex != -1)
	{
		strState = strSchemaPart.Mid(nStateIndex + 1, strSchemaPart.GetLength() - nStateIndex - 2);
		strSchemaPart = strSchemaPart.Left(nStateIndex);
	}


	int nClassPartIndex = strSchemaPart.Find(_T('.'));
	if (nClassPartIndex != -1)
	{
		strClass = strSchemaPart.Left(nClassPartIndex);
		strClassPart = strSchemaPart.Mid(nClassPartIndex + 1);
	}
	else
	{
		strClass = strSchemaPart;
	}

	int nClassPartCode = 0;
	if (!strClassPart.IsEmpty())
	{
		nClassPartCode = GetClassPartCode(strClass, strClassPart);
		if (nClassPartCode == 0)
		{
			ASSERT(FALSE);
			return 0;
		}
	}

	int nStateCode = 0;
	if (!strState.IsEmpty())
	{
		nStateCode = GetStateCode(strClassPart.IsEmpty()? strClass: strClassPart, strState);
		if (nStateCode == 0)
		{
			ASSERT(FALSE);
			return 0;
		}
	}

	int nClassCode = GetClassCode(strClass);
	if (nClassCode == 0)
		return 0;

	return CalculatePropertyCode(nClassCode, nClassPartCode, nStateCode, 0);
}

CXTPSkinManagerSchemaProperty* CXTPSkinManagerSchema::CreateProperty(LPCTSTR lpszProperty, XTPSkinManagerProperty nPropertyType, LPCTSTR lpszValue)
{
	CXTPSkinManagerSchemaProperty* pProperty = new CXTPSkinManagerSchemaProperty();

	switch (nPropertyType)
	{
		case XTP_SKINPROPERTY_COLOR:    pProperty->SetPropertyColor(lpszValue); break;
		case XTP_SKINPROPERTY_STRING:   pProperty->SetPropertyString(lpszValue); break;
		case XTP_SKINPROPERTY_BOOL:     pProperty->SetPropertyBool(lpszValue); break;
		case XTP_SKINPROPERTY_RECT:     pProperty->SetPropertyRect(lpszValue); break;
		case XTP_SKINPROPERTY_INT:      pProperty->SetPropertyInt(lpszValue); break;
		case XTP_SKINPROPERTY_POSITION: pProperty->SetPropertySize(lpszValue); break;
		case XTP_SKINPROPERTY_ENUM:     pProperty->SetPropertyEnum(GetEnumCode(lpszProperty, lpszValue)); break;
		case XTP_SKINPROPERTY_FONT:     pProperty->SetPropertyFont(lpszValue); break;

		default:
			delete pProperty;
			return NULL;
	}

	pProperty->propType = nPropertyType;
	return pProperty;
}


HRESULT CXTPSkinManagerSchema::ReadProperties()
{
	CXTPSkinManagerResourceFile* pResourceFile = GetResourceFile();

	RemoveAllProperties();

	CString str, strSchemaPart;

	UINT nSchemaPart = 0;

	while (pResourceFile->ReadString(str))
	{
		str.TrimLeft();

		int nCommentIndex = str.Find(_T(';'));

		if (nCommentIndex == 0)
			continue;

		if (nCommentIndex > 0)
		{
			str = str.Left(nCommentIndex);
		}

		if (str.IsEmpty())
			continue;

		str.MakeUpper();

		if (str[0] == _T('['))
		{
			strSchemaPart = str.Mid(1, str.GetLength() - 2);
			nSchemaPart = GetSchemaPartCode(strSchemaPart);
		}
		else if (nSchemaPart)
		{
			int nPos = str.Find(_T('='));
			if (nPos <= 0)
				continue;

			CString strProperty = str.Left(nPos);
			strProperty.TrimRight();
			strProperty.MakeUpper();

			XTPSkinManagerProperty nPropertyType = XTP_SKINPROPERTY_UNKNOWN;

			UINT nPropertyCode = GetPropertyCode(strProperty, nPropertyType);

			if (nPropertyCode == 0 || nPropertyType == XTP_SKINPROPERTY_UNKNOWN)
			{
				continue;
			}

			CString strValue = str.Mid(nPos + 1);
			strValue.TrimLeft();
			strValue.TrimRight();
			strValue.MakeUpper();

			CXTPSkinManagerSchemaProperty* pProperty = 0;


			if (m_mapProperties.Lookup(nSchemaPart + nPropertyCode, pProperty))
			{
				m_mapProperties.RemoveKey(nSchemaPart + nPropertyCode);
				delete pProperty;
			}

			pProperty = CreateProperty(strProperty, nPropertyType, strValue);

			if (pProperty == 0)
			{
				continue;
			}

			pProperty->nPropertyCode = nPropertyCode;

			m_mapProperties.SetAt(nSchemaPart + nPropertyCode, pProperty);
		}
	}

	int nAlpha = 0;
	if (SUCCEEDED(GetIntProperty(GetClassCode(_T("DOCUMENTATION")), 0, 0, TMT_ALPHALEVEL, nAlpha)) && nAlpha == 255)
	{
		m_bPreMultiplyImages = FALSE;
	}

	return S_OK;
}


CXTPSkinManagerSchemaProperty* CXTPSkinManagerSchema::GetProperty(UINT iClassId, int iPartId, int iStateId, int iPropId)
{
	CXTPSkinManagerSchemaProperty* pProperty;

	if (iStateId && m_mapProperties.Lookup(CalculatePropertyCode(iClassId, iPartId, iStateId, iPropId), pProperty))
		return pProperty;

	UINT nPropertyCode = CalculatePropertyCode(iClassId, iPartId, 0, iPropId);

	if (m_mapProperties.Lookup(nPropertyCode, pProperty))
		return pProperty;

	if (iPartId && iStateId && m_mapProperties.Lookup(CalculatePropertyCode(iClassId, 0, iStateId, iPropId), pProperty))
		return pProperty;

	if (iPartId && m_mapProperties.Lookup(CalculatePropertyCode(iClassId, 0, 0, iPropId), pProperty))
		return pProperty;

	if (m_mapProperties.Lookup(CalculatePropertyCode(m_nGlobalClassId, 0, 0, iPropId), pProperty))
		return pProperty;

	return NULL;
}

HRESULT CXTPSkinManagerSchema::GetStringProperty(UINT iClassId, int iPartId, int iStateId, int iPropId, CString& str)
{
	CXTPSkinManagerSchemaProperty* pProperty = GetProperty(iClassId, iPartId, iStateId, iPropId);

	if (!pProperty)
		return E_FAIL;

	if (pProperty->propType != XTP_SKINPROPERTY_STRING)
		return E_INVALIDARG;

	str = pProperty->lpszVal;

	return S_OK;
}

HRESULT CXTPSkinManagerSchema::GetRectProperty(UINT iClassId, int iPartId, int iStateId, int iPropId, CRect& rc)
{
	CXTPSkinManagerSchemaProperty* pProperty = GetProperty(iClassId, iPartId, iStateId, iPropId);

	if (!pProperty)
		return E_FAIL;

	if (pProperty->propType != XTP_SKINPROPERTY_RECT)
		return E_INVALIDARG;

	rc = pProperty->rcVal;

	return S_OK;
}


HRESULT CXTPSkinManagerSchema::GetIntProperty(UINT iClassId, int iPartId, int iStateId, int iPropId, int& iVal)
{
	CXTPSkinManagerSchemaProperty* pProperty = GetProperty(iClassId, iPartId, iStateId, iPropId);

	if (!pProperty)
		return E_FAIL;

	if (pProperty->propType != XTP_SKINPROPERTY_INT)
		return E_INVALIDARG;

	iVal = pProperty->iVal;

	return S_OK;
}

HRESULT CXTPSkinManagerSchema::GetBoolProperty(UINT iClassId, int iPartId, int iStateId, int iPropId, BOOL& bVal)
{
	CXTPSkinManagerSchemaProperty* pProperty = GetProperty(iClassId, iPartId, iStateId, iPropId);

	if (!pProperty)
		return E_FAIL;

	if (pProperty->propType != XTP_SKINPROPERTY_BOOL)
		return E_INVALIDARG;

	bVal = pProperty->bVal;

	return S_OK;
}

HRESULT CXTPSkinManagerSchema::GetColorProperty(UINT iClassId, int iPartId, int iStateId, int iPropId, COLORREF& clrVal)
{
	CXTPSkinManagerSchemaProperty* pProperty = GetProperty(iClassId, iPartId, iStateId, iPropId);

	if (!pProperty)
		return E_FAIL;

	if (pProperty->propType != XTP_SKINPROPERTY_COLOR)
		return E_INVALIDARG;

	clrVal = pProperty->clrVal;
	GetSkinManager()->ApplyColorFilter(clrVal);

	return S_OK;
}

HRESULT CXTPSkinManagerSchema::GetEnumProperty(UINT iClassId, int iPartId, int iStateId, int iPropId, int& nVal)
{
	CXTPSkinManagerSchemaProperty* pProperty = GetProperty(iClassId, iPartId, iStateId, iPropId);

	if (!pProperty)
		return E_FAIL;

	if (pProperty->propType != XTP_SKINPROPERTY_ENUM)
		return E_INVALIDARG;

	nVal = pProperty->iVal;

	return S_OK;
}

HRESULT CXTPSkinManagerSchema::GetFontProperty(UINT iClassId, int iPartId, int iStateId, int iPropId, LOGFONT& lfVal)
{
	CXTPSkinManagerSchemaProperty* pProperty = GetProperty(iClassId, iPartId, iStateId, iPropId);

	if (!pProperty)
		return E_FAIL;

	if (pProperty->propType != XTP_SKINPROPERTY_FONT)
		return E_INVALIDARG;

	lfVal = *pProperty->lfVal;

	return S_OK;
}

HRESULT CXTPSkinManagerSchema::GetSizeProperty(UINT iClassId, int iPartId, int iStateId, int iPropId, CSize& szVal)
{
	CXTPSkinManagerSchemaProperty* pProperty = GetProperty(iClassId, iPartId, iStateId, iPropId);

	if (!pProperty)
		return E_FAIL;

	if (pProperty->propType != XTP_SKINPROPERTY_POSITION)
		return E_INVALIDARG;

	szVal = pProperty->szVal;

	return S_OK;

}

//////////////////////////////////////////////////////////////////////////
// CXTPSkinManagerSchema

int CXTPSkinManagerSchema::FindBestImageGlyphSize(CXTPSkinManagerClass* pClass, int iPartId, int iStateId, const CRect& rcDest, int nImageCount, BOOL bHorizontalImageLayout)
{
	int nImageFile = TMT_IMAGEFILE5;
	CString strImageFile;

	while (nImageFile >= TMT_IMAGEFILE1)
	{
		strImageFile = pClass->GetThemeString(iPartId, iStateId, nImageFile);
		if (!strImageFile.IsEmpty())
		{
			CSize sz = pClass->GetImages()->GetExtent(GetResourceFile(), strImageFile);
			if (sz == CSize(0))
				return -1;

			if (bHorizontalImageLayout) sz.cx /= nImageCount; else sz.cy /= nImageCount;

			if ((nImageFile == TMT_IMAGEFILE1) || (rcDest.Height() >= sz.cy && rcDest.Width() >= sz.cx))
				return nImageFile;
		}
		nImageFile--;
	}

	return nImageFile;
}

BOOL CXTPSkinManagerSchema::DrawThemeBackgroundGlyph(CDC* pDC, CXTPSkinManagerClass* pClass, int iPartId, int iStateId, const RECT *pRect)
{
	int nImageFile = TMT_GLYPHIMAGEFILE;

	int nImageCount = pClass->GetThemeInt(iPartId, iStateId, TMT_IMAGECOUNT, 1);
	if (nImageCount < 1)
		nImageCount = 1;

	BOOL bHorizontalImageLayout = pClass->GetThemeEnumValue(iPartId, iStateId, TMT_IMAGELAYOUT, IL_HORIZONTAL) == IL_HORIZONTAL;

	CRect rcDest(pRect);
	CRect rcContentMargins = pClass->GetThemeRect(iPartId, iStateId, TMT_CONTENTMARGINS);
	rcDest.DeflateRect(rcContentMargins);


	if (pClass->GetThemeEnumValue(iPartId, iStateId, TMT_IMAGESELECTTYPE) == IST_SIZE)
	{
		nImageFile = FindBestImageGlyphSize(pClass, iPartId, iStateId, rcDest, nImageCount, bHorizontalImageLayout);
		if (nImageFile == -1)
			return FALSE;
	}

	CString strImageFile = pClass->GetThemeString(iPartId, iStateId, nImageFile);
	if (strImageFile.IsEmpty())
		return FALSE;

	CXTPSkinImage* pImage = pClass->GetImages()->LoadFile(GetResourceFile(), strImageFile);
	if (!pImage)
	{
		return FALSE;
	}


	CSize sz(pImage->GetWidth(), pImage->GetHeight());
	if (bHorizontalImageLayout) sz.cx /= nImageCount; else sz.cy /= nImageCount;

	BOOL bTransparent = pImage->IsAlphaImage()? FALSE: pClass->GetThemeBool(iPartId, iStateId, TMT_GLYPHTRANSPARENT);

	CPoint ptTopLeft((rcDest.left + rcDest.right - sz.cx)/2, (rcDest.top + rcDest.bottom - sz.cy)/2);
	if (ptTopLeft.y < rcDest.top)
		ptTopLeft.y  = rcDest.top;

	CRect rcGlyph(ptTopLeft, sz);

	CRect rcSrcImage = bHorizontalImageLayout? CRect((iStateId - 1) * sz.cx,  0, iStateId * sz.cx, sz.cy):
	CRect(0, (iStateId - 1) * sz.cy, sz.cx, iStateId * sz.cy);

	if (nImageCount <= iStateId - 1)
		rcSrcImage = CRect(0, 0, sz.cx, sz.cy);

	COLORREF clrTransparent = COLORREF_NULL;
	if (bTransparent)
	{
		clrTransparent = pClass->GetThemeColor(iPartId, iStateId, TMT_TRANSPARENTCOLOR, RGB(0xFF, 0, 0xFF));
	}
	pImage->DrawImage(pDC, rcGlyph, rcSrcImage, CRect(0, 0, 0, 0), clrTransparent, ST_TRUESIZE, FALSE);

	return TRUE;
}

BOOL CXTPSkinManagerSchema::DrawThemeBackgroundBorder(CDC* pDC, CXTPSkinManagerClass* pClass, int iPartId, int iStateId, const RECT *pRect)
{
	int nBorderSize = pClass->GetThemeInt(iPartId, iStateId, TMT_BORDERSIZE, 1);

	COLORREF clrBorderColor = pClass->GetThemeColor(iPartId, iStateId, TMT_BORDERCOLOR);
	COLORREF clrFillColor = pClass->GetThemeColor(iPartId, iStateId, TMT_FILLCOLOR);
	BOOL bBorderOnly = pClass->GetThemeBool(iPartId, iStateId, TMT_BORDERONLY, FALSE);

	CRect rc(pRect);

	if ((nBorderSize > 0) && (clrBorderColor != COLORREF_NULL))
	{
		pDC->FillSolidRect(rc.left, rc.top, rc.Width(), nBorderSize, clrBorderColor);
		pDC->FillSolidRect(rc.left, rc.bottom - nBorderSize, rc.Width(), nBorderSize, clrBorderColor);

		pDC->FillSolidRect(rc.left, rc.top + nBorderSize, nBorderSize, rc.Height() - nBorderSize * 2, clrBorderColor);
		pDC->FillSolidRect(rc.right - nBorderSize, rc.top + nBorderSize, nBorderSize, rc.Height() - nBorderSize * 2, clrBorderColor);

		rc.DeflateRect(nBorderSize, nBorderSize);
	}

	if ((clrFillColor != COLORREF_NULL) && (!bBorderOnly))
	{
		pDC->FillSolidRect(rc, clrFillColor);
	}

	return TRUE;
}


BOOL CXTPSkinManagerSchema::DrawThemeBackground(CDC* pDC, CXTPSkinManagerClass* pClass, int iPartId, int iStateId, const RECT *pRect)
{
	int nBackgroundType = pClass->GetThemeEnumValue(iPartId, iStateId, TMT_BGTYPE, BT_IMAGEFILE);

	if (nBackgroundType == BT_BORDERFILL)
	{
		return DrawThemeBackgroundBorder(pDC, pClass, iPartId, iStateId, pRect);
	}

	if (nBackgroundType != BT_IMAGEFILE)
		return TRUE;

	int nImageFile = (pClass->GetThemeEnumValue(iPartId, iStateId, TMT_IMAGESELECTTYPE) != IST_NONE) &&
		(pClass->GetThemeEnumValue(iPartId, iStateId, TMT_GLYPHTYPE, GT_NONE) == GT_NONE) ?
	TMT_IMAGEFILE1: TMT_IMAGEFILE;

	CString strImageFile = pClass->GetThemeString(iPartId, iStateId, nImageFile);
	if (strImageFile.IsEmpty())
	{
		if (nImageFile != TMT_IMAGEFILE1)
			return FALSE;

		strImageFile = pClass->GetThemeString(iPartId, iStateId, TMT_IMAGEFILE);
		if (strImageFile.IsEmpty())
			return FALSE;

	}

	CRect rcSizingMargins = pClass->GetThemeRect(iPartId, iStateId, TMT_SIZINGMARGINS);

	CXTPSkinImage* pImage = pClass->GetImages()->LoadFile(GetResourceFile(), strImageFile);
	if (!pImage)
	{
		return FALSE;
	}

	int nImageCount = pClass->GetThemeInt(iPartId, iStateId, TMT_IMAGECOUNT, 1);
	if (nImageCount < 1)
		nImageCount = 1;

	BOOL bHorizontalImageLayout = pClass->GetThemeEnumValue(iPartId, iStateId, TMT_IMAGELAYOUT, IL_HORIZONTAL) == IL_HORIZONTAL;

	CSize sz(pImage->GetWidth(), pImage->GetHeight());
	if (bHorizontalImageLayout) sz.cx /= nImageCount; else sz.cy /= nImageCount;

	BOOL bTransparent = pImage->IsAlphaImage()? FALSE: pClass->GetThemeBool(iPartId, iStateId, TMT_TRANSPARENT);

	CRect rcImage(*pRect);

	int nSizingType = pClass->GetThemeEnumValue(iPartId, iStateId, TMT_SIZINGTYPE, ST_STRETCH);
	if (nSizingType == ST_TRUESIZE)
	{
		CSize szDest(sz);

		if (pClass->GetThemeInt(iPartId, iStateId, TMT_TRUESIZESTRETCHMARK) > 0)
		{
			if (szDest.cx > rcImage.Width())
			{
				szDest.cx = rcImage.Width();
			}

			if (szDest.cy > rcImage.Height())
			{
				szDest.cy = rcImage.Height();
			}
		}

		CPoint ptTopLeft((rcImage.left + rcImage.right - szDest.cx)/2, (rcImage.top + rcImage.bottom - szDest.cy)/2);
		if (ptTopLeft.y < rcImage.top)
			ptTopLeft.y = rcImage.top;

		if (ptTopLeft.x < rcImage.left)
			ptTopLeft.x = rcImage.left;

		if (pClass->GetThemeEnumValue(iPartId, iStateId, TMT_VALIGN) == VA_BOTTOM)
			rcImage = CRect(CPoint(ptTopLeft.x, rcImage.bottom - szDest.cy), szDest);
		else
			rcImage = CRect(ptTopLeft, szDest);

		rcSizingMargins = CRect(0, 0, 0, 0);

	}

	CRect rcSrcImage = bHorizontalImageLayout? CRect((iStateId - 1) * sz.cx,  0, iStateId * sz.cx, sz.cy):
		CRect(0, (iStateId - 1) * sz.cy, sz.cx, iStateId * sz.cy);

	if ((nImageCount <= iStateId - 1) || (iStateId == 0))
		rcSrcImage = CRect(0, 0, sz.cx, sz.cy);

	COLORREF clrTransparent = COLORREF_NULL;
	if (bTransparent)
	{
		clrTransparent = pClass->GetThemeColor(iPartId, iStateId, TMT_TRANSPARENTCOLOR, RGB(0xFF, 0, 0xFF));
	}

	BOOL bBorderOnly = pClass->GetThemeBool(iPartId, iStateId, TMT_BORDERONLY);

	if (pClass->GetClassCode() == m_nWindowClassId && (iPartId >= WP_FRAMELEFT && iPartId <= WP_SMALLFRAMEBOTTOM))
		bBorderOnly = FALSE;

	pImage->m_bMirrorImage =  pClass->GetThemeBool(iPartId, iStateId, TMT_MIRRORIMAGE, TRUE);

	pImage->CreateSolidRectArray(nImageCount, bHorizontalImageLayout, rcSizingMargins);

	pImage->DrawImage(pDC, rcImage, rcSrcImage, rcSizingMargins, clrTransparent, nSizingType, bBorderOnly);

	if (pClass->GetThemeEnumValue(iPartId, iStateId, TMT_GLYPHTYPE) == GT_IMAGEGLYPH)
	{
		DrawThemeBackgroundGlyph(pDC, pClass, iPartId, iStateId, pRect);
	}

	return TRUE;
}

CSize CXTPSkinManagerSchema::GetCaptionButtonSize(CXTPSkinManagerClass* pClass, int yButton)
{
	int iPartId = WP_CLOSEBUTTON, iStateId = 1;

	if (pClass->GetThemeEnumValue(iPartId, iStateId, TMT_BGTYPE) != BT_IMAGEFILE)
		return CSize(yButton, yButton);

	int nImageFile = (pClass->GetThemeEnumValue(iPartId, iStateId, TMT_IMAGESELECTTYPE) != IST_NONE) &&
		(pClass->GetThemeEnumValue(iPartId, iStateId, TMT_GLYPHTYPE, GT_NONE) == GT_NONE) ?
		TMT_IMAGEFILE1: TMT_IMAGEFILE;

	CString strImageFile = pClass->GetThemeString(iPartId, iStateId, nImageFile);
	if (strImageFile.IsEmpty())
	{
		if (nImageFile != TMT_IMAGEFILE1)
			return CSize(yButton, yButton);

		strImageFile = pClass->GetThemeString(iPartId, iStateId, TMT_IMAGEFILE);
		if (strImageFile.IsEmpty())
			return CSize(yButton, yButton);

	}


	CXTPSkinImage* pImage = pClass->GetImages()->LoadFile(GetResourceFile(), strImageFile);
	if (!pImage)
	{
		return CSize(yButton, yButton);
	}

	int nImageCount = pClass->GetThemeInt(iPartId, iStateId, TMT_IMAGECOUNT, 1);
	if (nImageCount < 1)
		nImageCount = 1;

	BOOL bHorizontalImageLayout = pClass->GetThemeEnumValue(iPartId, iStateId, TMT_IMAGELAYOUT, IL_HORIZONTAL) == IL_HORIZONTAL;

	CSize sz(pImage->GetWidth(), pImage->GetHeight());
	if (bHorizontalImageLayout) sz.cx /= nImageCount; else sz.cy /= nImageCount;

	int xButton = MulDiv(sz.cx, yButton, sz.cy) + MulDiv(sz.cx - sz.cy, 4, sz.cy);

	return CSize(xButton, yButton);
}

int CXTPSkinManagerSchema::DrawThemeFrameButtons(CDC* pDC, CXTPSkinObjectFrame* pFrame)
{
	CXTPWindowRect rc(pFrame);
	rc.OffsetRect(-rc.TopLeft());

	CRect rcBorders = pFrame->GetBorders();
	LONG lStyle = pFrame->GetStyle();
	DWORD dwExStyle = pFrame->GetExStyle();

	int cBorders = GetWindowBorders(lStyle, dwExStyle, TRUE, FALSE);

	int cxFrame = cBorders * GetMetrics()->m_cxBorder;

	if ((lStyle & WS_MAXIMIZE) && rcBorders.left > 6) // Special fix for Windows Windows 7/Vista large borders
	{
		int nOffset = rcBorders.left - (cxFrame + (dwExStyle & WS_EX_CLIENTEDGE ? GetMetrics()->m_cxEdge : 0));
		rc.DeflateRect(nOffset, nOffset);
	}

	int nCaptionHeight = rcBorders.top - rc.top;

	int nTop = 0;
	if (dwExStyle & WS_EX_WINDOWEDGE)
		nTop++;
	else if (dwExStyle & WS_EX_STATICEDGE)
		nTop++;

	if (lStyle & WS_SIZEBOX)
		nTop += (1 + GetMetrics()->m_nBorderSize) / 2;

	int yButton = nCaptionHeight - 5 - cxFrame;

	if (dwExStyle & WS_EX_CLIENTEDGE)
	{
		yButton -= 2;
		nTop -= 1;
	}

	CXTPSkinManagerClass* pClass = m_pManager->GetSkinClass(pFrame, _T("WINDOW"));
	CSize szButton = dwExStyle & WS_EX_TOOLWINDOW ? CSize(yButton, yButton) : GetCaptionButtonSize(pClass, yButton);

	CRect rcButton = CRect(CPoint(rc.right - szButton.cx - cBorders - 2, rc.top + nTop + (nCaptionHeight - szButton.cy)/2), szButton);

	CXTPSkinObjectFrame::CCaptionButtons* pButtons = pFrame->GetCaptionButtons();

	for (int i = 0; i < (int)pButtons->GetSize(); i++)
	{
		CXTPSkinObjectFrame::CCaptionButton* pButton = pButtons->GetAt(i);

		pButton->m_rcButton = rcButton;
		pButton->Draw(pDC, pFrame->IsActive());

		rcButton.OffsetRect(-rcButton.Width() - 2, 0);
	}

	return rcButton.right + 2;
}

void CXTPSkinManagerSchema::DrawNonClientRect(CDC* pDC, CRect rcFrame, CXTPSkinObjectFrame* pFrame)
{
	DWORD dwExStyle = pFrame->GetExStyle();
	DWORD dwStyle = pFrame->GetStyle();

	if (dwExStyle & WS_EX_WINDOWEDGE)
	{
		DrawEdge(pDC->GetSafeHdc(), &rcFrame, EDGE_RAISED, BF_RECT | BF_ADJUST);
	}
	else if (dwExStyle & WS_EX_STATICEDGE)
	{
		DrawEdge(pDC->GetSafeHdc(), &rcFrame, BDR_SUNKENOUTER, BF_RECT | BF_ADJUST);
	}

	if ((dwStyle & WS_CAPTION) || (dwExStyle & WS_EX_DLGMODALFRAME))
	{
		COLORREF clr = GetColor((dwExStyle & (WS_EX_CLIENTEDGE | WS_EX_DLGMODALFRAME | WS_EX_WINDOWEDGE | WS_EX_STATICEDGE))
			|| (dwStyle & WS_DLGFRAME) ? COLOR_3DFACE : COLOR_WINDOWFRAME);

		pDC->Draw3dRect(rcFrame, clr, clr);
		rcFrame.DeflateRect(1, 1);
	}

	if (dwStyle & WS_SIZEBOX)
	{
		int nSize = GetMetrics()->m_nBorderSize;
		XTPSkinFrameworkDrawFrame(pDC->GetSafeHdc(), rcFrame, nSize, GetColor(COLOR_3DFACE));

		rcFrame.DeflateRect(nSize, nSize);
	}


	if (dwExStyle & WS_EX_CLIENTEDGE)
	{
		DrawClientEdge(pDC, rcFrame, pFrame);
	}
}

void CXTPSkinManagerSchema::DrawClientEdge(CDC* pDC, const CRect& rcFrame, CXTPSkinObjectFrame* pFrame)
{
	CRect rc = rcFrame;

	CXTPSkinManagerClass* pClass = m_pManager->GetSkinClass(pFrame, pFrame->GetClassName());

	if (pClass->GetThemeEnumValue(0, 0, TMT_BGTYPE) == BT_BORDERFILL)
	{
		COLORREF clrBorderColor = pClass->GetThemeColor(0, 0, TMT_BORDERCOLOR);

		pDC->Draw3dRect(rc, clrBorderColor, clrBorderColor);
		rc.DeflateRect(1, 1);

		HBRUSH hBrush = pFrame->GetClientBrush(pDC);
		XTPSkinFrameworkDrawFrame(pDC->GetSafeHdc(), &rc, 1, hBrush);
	}
	else
	{
		pDC->Draw3dRect(rc, GetMetrics()->m_clrEdgeShadow, GetMetrics()->m_clrEdgeHighLight);
		rc.DeflateRect(1, 1);
		pDC->Draw3dRect(rc, GetMetrics()->m_clrEdgeDkShadow, GetMetrics()->m_clrEdgeLight);
		rc.DeflateRect(1, 1);
	}
}


HICON CXTPSkinManagerSchema::GetFrameSmIcon(HWND hWnd, BOOL bCheckStyle) const
{
	DWORD dwStyle = GetWindowLong(hWnd, GWL_STYLE);
	DWORD dwExStyle = GetWindowLong(hWnd, GWL_EXSTYLE);

	if (bCheckStyle)
	{
		if (dwExStyle & WS_EX_TOOLWINDOW)
			return NULL;

		if ((dwStyle & WS_SYSMENU) == 0)
			return NULL;
	}

	HICON hIcon = (HICON)(DWORD_PTR)::SendMessage(hWnd, WM_GETICON, ICON_SMALL, 0);
	if (hIcon)
		return hIcon;

	hIcon = (HICON)(DWORD_PTR)::SendMessage(hWnd, WM_GETICON, ICON_BIG, 0);
	if (hIcon)
		return hIcon;

	hIcon = (HICON)(DWORD_PTR)::GetClassLongPtr(hWnd, GCLP_HICONSM);
	if (hIcon)
		return hIcon;

	if (((dwStyle & (WS_BORDER | WS_DLGFRAME)) != WS_DLGFRAME) && ((dwExStyle & WS_EX_DLGMODALFRAME) == 0))
	{
		ULONG_PTR dwResult;

		if (SendMessageTimeout(hWnd,
			WM_QUERYDRAGICON,
			0,
			0,
			SMTO_NORMAL,
			100,
			&dwResult))
		{
			hIcon = (HICON)dwResult;
		}

		if (hIcon == NULL)
		{
			hIcon = AfxGetApp()->LoadOEMIcon(OIC_WINLOGO);
		}
	}

	return hIcon;
}

void CXTPSkinManagerSchema::DrawThemeFrame(CDC* pDC, CXTPSkinObjectFrame* pFrame)
{
	XTP_GUARD_SHARED(CXTPSkinObjectFrame, pFrame)
	{
		CXTPWindowRect rc(pFrame);
		rc.OffsetRect(-rc.TopLeft());

		const DWORD dwStyle = pFrame->GetStyle();
		const DWORD dwExStyle = pFrame->GetExStyle();

		const BOOL bToolWindow = (WS_EX_TOOLWINDOW == (WS_EX_TOOLWINDOW & dwExStyle));
		const BOOL bMaximize = (WS_MAXIMIZE == (WS_MAXIMIZE      & dwStyle));
		const BOOL bMinimize = (WS_MINIMIZE == (WS_MINIMIZE      & dwStyle));

		if ((dwStyle & WS_CAPTION) != WS_CAPTION)
		{
			DrawNonClientRect(pDC, rc, pFrame);
			return;
		}


		CRect rcBorders = pFrame->GetBorders();

		if (dwExStyle & WS_EX_CLIENTEDGE)
		{
			DrawClientEdge(pDC, CRect(rcBorders.left - 2, rcBorders.top - 2, rc.Width() - rcBorders.right + 2,
				rc.Height() - rcBorders.bottom + 2), pFrame);

			InflateBorders(rcBorders, -2, -2);
		}


		CRect rcCaption(0, 0, rc.right, rcBorders.top);
		CRect rcFrame(0, rcBorders.top, rc.Width(), rc.Height());

		CXTPSkinManagerClass* pClassWindow = XTPAccessShared(m_pManager)->GetSkinClass(pFrame, _T("WINDOW"));
		XTP_GUARD_SHARED(CXTPSkinManagerClass, pClassWindow)
		{
			int nFrameState = pFrame->IsActive() ? FS_ACTIVE : FS_INACTIVE;

			pClassWindow->DrawThemeBackground(pDC, bToolWindow ? WP_SMALLFRAMELEFT : WP_FRAMELEFT, nFrameState,
				CRect(rcFrame.left, rcFrame.top, rcFrame.left + rcBorders.left, rcFrame.bottom - rcBorders.bottom));

			pClassWindow->DrawThemeBackground(pDC, bToolWindow ? WP_SMALLFRAMERIGHT : WP_FRAMERIGHT, nFrameState,
				CRect(rcFrame.right - rcBorders.right, rcFrame.top, rcFrame.right, rcFrame.bottom - rcBorders.bottom));

			pClassWindow->DrawThemeBackground(pDC, bToolWindow ? WP_SMALLFRAMEBOTTOM : WP_FRAMEBOTTOM, nFrameState,
				CRect(rcFrame.left, rcFrame.bottom - rcBorders.bottom, rcFrame.right, rcFrame.bottom));

			{
				//CXTPLockGuard lock(m_csDraw);
				CXTPBufferDC dc(*pDC, rcCaption);

				int nStateId = WP_CAPTION;

				if (bToolWindow) nStateId = WP_SMALLCAPTION;
				if (bMaximize)   nStateId = WP_MAXCAPTION;
				if (bMinimize)   nStateId = WP_MINCAPTION;

				pClassWindow->DrawThemeBackground(&dc, nStateId, nFrameState, rcCaption);


				CSize szIcon(GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON));

				int nTextLeft = rcBorders.left;

				HICON hIcon = GetFrameSmIcon(pFrame->GetSafeHwnd());

				int cBorders = GetWindowBorders(dwStyle, dwExStyle, TRUE, FALSE) - GetMetrics()->m_cyBorder;

				if (hIcon)
				{

					int cxySlot = rcBorders.top - cBorders;
					int nTop = cBorders + (cxySlot - szIcon.cy) / 2;

					CRect rcButn(rcBorders.left + 2, nTop, rcBorders.left + 2 + szIcon.cx, nTop + szIcon.cy);

					DWORD dwLayout = XTPDrawHelpers()->IsContextRTL(&dc);
					if (dwLayout & LAYOUT_RTL)
						XTPDrawHelpers()->SetContextRTL(&dc, dwLayout | LAYOUT_BITMAPORIENTATIONPRESERVED);

					DrawIconEx(dc.m_hDC, rcButn.left, rcButn.top, hIcon,
						rcButn.Width(), rcButn.Height(), 0, NULL, DI_NORMAL);

					if (dwLayout & LAYOUT_RTL)
						XTPDrawHelpers()->SetContextRTL(&dc, dwLayout);


					nTextLeft = 2 + szIcon.cx + rcBorders.left;
				}


				int nTextRight = DrawThemeFrameButtons(&dc, pFrame);

				dc.SetTextColor(GetColor(pFrame->IsActive() ? COLOR_CAPTIONTEXT : COLOR_INACTIVECAPTIONTEXT));

				CFont* pOldFont = dc.SelectObject(bToolWindow ? &GetMetrics()->m_fntSmCaption : &GetMetrics()->m_fntCaption);

				CString strCaption;

				CXTPDrawHelpers::GetWindowCaption(pFrame->GetSafeHwnd(), strCaption);

				CRect rcText(nTextLeft, 1 + cBorders, nTextRight, rcBorders.top);
				pClassWindow->DrawThemeText(&dc, bToolWindow ? WP_SMALLCAPTION : WP_CAPTION, pFrame->IsActive() ? CS_ACTIVE : CS_INACTIVE,
					strCaption, DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS | DT_NOPREFIX
					| (dwExStyle & WS_EX_RTLREADING ? DT_RTLREADING : 0) | (dwExStyle & WS_EX_RIGHT ? DT_RIGHT : 0), rcText);

				dc.SelectObject(pOldFont);
			}
		}
	}
}

int CXTPSkinManagerSchema::GetWindowBorders(LONG lStyle, DWORD dwExStyle, BOOL fWindow, BOOL fClient)
{
	int cBorders = 0;

	if (fWindow)
	{
		if (dwExStyle & WS_EX_WINDOWEDGE)
			cBorders += 2;
		else if (dwExStyle & WS_EX_STATICEDGE)
			cBorders++;

		if ((lStyle & WS_CAPTION) || (dwExStyle & WS_EX_DLGMODALFRAME))
			cBorders++;

		if (lStyle & WS_SIZEBOX)
			cBorders += GetMetrics()->m_nBorderSize;
	}

	if (fClient)
	{
		if (dwExStyle & WS_EX_CLIENTEDGE)
			cBorders += 2;
	}

	return(cBorders);
}


CRect CXTPSkinManagerSchema::CalcFrameBorders(CXTPSkinObjectFrame* pFrame)
{
	DWORD dwExStyle = pFrame->GetExStyle();
	DWORD dwStyle = pFrame->GetStyle();

	return CalcFrameBorders(dwStyle, dwExStyle);
}

CRect CXTPSkinManagerSchema::CalcFrameBorders(DWORD dwStyle, DWORD dwExStyle)
{
	BOOL bToolWindow = (dwExStyle & WS_EX_TOOLWINDOW) == WS_EX_TOOLWINDOW;
	CXTPSkinManagerMetrics* pMetrics = GetMetrics();

	if (((dwStyle & (WS_CAPTION | WS_MAXIMIZE)) == (WS_CAPTION | WS_MAXIMIZE) && ((dwStyle & WS_CHILD) == 0)))
	{
		int nCaptionHeight = bToolWindow ? pMetrics->m_cySmallCaption: pMetrics->m_cyCaption;

		CRect rc(0, 0, 100, 100);
		AdjustWindowRectEx(rc, dwStyle, FALSE, dwExStyle);
		int nBorder = -rc.left;
		return CRect(nBorder, nBorder + nCaptionHeight, nBorder, nBorder);
	}



	CRect rcBorders(0, 0, 0, 0);
	if ((dwStyle & WS_CAPTION) == WS_CAPTION)
	{
		int nCaptionHeight = bToolWindow ? pMetrics->m_cySmallCaption: pMetrics->m_cyCaption;
		rcBorders.top += nCaptionHeight;
	}

	int cBorders = GetWindowBorders(dwStyle, dwExStyle, TRUE, FALSE);

	InflateBorders(rcBorders, cBorders * pMetrics->m_cxBorder, cBorders * pMetrics->m_cyBorder);

	if ((dwExStyle & WS_EX_CLIENTEDGE) == WS_EX_CLIENTEDGE)
	{
		InflateBorders(rcBorders, pMetrics->m_cxEdge, pMetrics->m_cyEdge);
	}

	return rcBorders;
}

AFX_INLINE void IncludeRgnPart(CRgn* pRgn, int x1, int y1, int x2, int y2)
{
	if (x1 < x2 && y1 < y2)
	{
		if (pRgn->GetSafeHandle() == NULL)
		{
			pRgn->CreateRectRgn(0, 0, 0, 0);
		}
		CRgn rgnExclude;
		rgnExclude.CreateRectRgn(x1, y1, x2, y2);
		pRgn->CombineRgn(pRgn, &rgnExclude, RGN_OR);
	}
}

void CXTPSkinManagerSchema::RegionFromBitmap(CBitmap* pBitmap, FRAMEPART nAlign, FRAMEREGIONPART* pPart, const CRect& rc, COLORREF clrTransparent)
{
	CXTPCompatibleDC dc(NULL, pBitmap);

	CSize sz = rc.Size();
	pPart->nCenter = 0;
	BOOL bCheckCenter = TRUE;

	if (nAlign == frameTop || nAlign == frameBottom)
	{
		for (int y = 0; y < sz.cy; y++)
		{
			int x = 0;

			while ( x < pPart->rcSizingMargins.left && dc.GetPixel(x, y) == clrTransparent)
			{
				x++;
			}

			if (x > 0)
			{
				IncludeRgnPart(&pPart->rgnLeft, 0, y, x, y + 1);
			}

			x = sz.cx - 1;
			int nOffset = sz.cx - pPart->rcSizingMargins.right;

			while (x >= nOffset)
			{
				int x0 = x;

				while (x >= nOffset && dc.GetPixel(x, y) == clrTransparent)
				{
					x--;
				}

				if (x != x0)
				{
					IncludeRgnPart(&pPart->rgnRight, x - nOffset + 1, y, x0 - nOffset + 1, y + 1);
				}

				while (x >= nOffset && dc.GetPixel(x, y) != clrTransparent)
				{
					x--;
				}

				if (x == x0)
					break;
			}


			if (bCheckCenter)
			{
				x = pPart->rcSizingMargins.left;
				int y0 = nAlign == frameTop ? y : sz.cy - 1 - y;

				while (x < sz.cx - pPart->rcSizingMargins.right && dc.GetPixel(x, y0) == clrTransparent)
				{
					x++;
				}
				if (x == sz.cx - pPart->rcSizingMargins.right)
				{
					pPart->nCenter++;
				}
				else
				{
					bCheckCenter = FALSE;
				}
			}
		}
	}
	else
	{
		for (int x = 0; x < sz.cx; x++)
		{
			int y = 0;

			while (y < pPart->rcSizingMargins.top && dc.GetPixel(x, y) == clrTransparent)
			{
				y++;
			}

			if (y > 0)
			{
				IncludeRgnPart(&pPart->rgnLeft, x, 0, x + 1, y);
			}

			y = sz.cy - 1;
			int nOffset = sz.cy - pPart->rcSizingMargins.bottom;

			while (y >= nOffset)
			{
				int y0 = y;

				while (y >= nOffset && dc.GetPixel(x, y) == clrTransparent)
				{
					y--;
				}

				if (y != y0)
				{
					IncludeRgnPart(&pPart->rgnRight, x, y - nOffset + 1, x + 1, y0 - nOffset + 1);
				}

				while (y >= nOffset && dc.GetPixel(x, y) != clrTransparent)
				{
					y--;
				}

				if (y == y0)
					break;
			}


			if (bCheckCenter)
			{
				y = pPart->rcSizingMargins.top;
				int x0 = nAlign == frameLeft ? x : sz.cx - 1 - x;

				while (y < sz.cy - pPart->rcSizingMargins.bottom && dc.GetPixel(x0, y) == clrTransparent)
				{
					y++;
				}
				if (y == sz.cy - pPart->rcSizingMargins.bottom)
				{
					pPart->nCenter++;
				}
				else
				{
					bCheckCenter = FALSE;
				}
			}
		}
	}
}

BOOL CXTPSkinManagerSchema::DrawWindowPart(CXTPSkinObjectFrame* pFrame, CBitmap& bmp, int iPartId, BOOL bVertical, int nBorderHeight, CRect& rcDest, CRect& rcSizingMargins, COLORREF& clrTransparent)
{
	bmp.DeleteObject();

	CXTPSkinManagerClass* pClass = m_pManager->GetSkinClass(pFrame, _T("WINDOW"));

	CString strImageFile = pClass->GetThemeString(iPartId, FS_ACTIVE, TMT_IMAGEFILE);
	if (strImageFile.IsEmpty())
		return  FALSE;

	CSize sz = pClass->GetImages()->GetExtent(GetResourceFile(), strImageFile);
	if (sz == CSize(0))
		return  FALSE;

	CXTPSkinImage* pImage = pClass->GetImages()->LoadFile(GetResourceFile(), strImageFile);
	if (!pImage)
		return FALSE;

	int nImageCount = pClass->GetThemeInt(iPartId, FS_ACTIVE, TMT_IMAGECOUNT, 1);
	ASSERT(nImageCount >= 1);
	if (nImageCount < 1)
		return FALSE;

	CWindowDC dcWindow(pFrame);
	CDC dc;
	dc.CreateCompatibleDC(&dcWindow);

	CRect rcSrc(0, 0, sz.cx, sz.cy / nImageCount);
	rcDest = bVertical ? CRect(0, 0, nBorderHeight, rcSrc.Height()) : CRect(0, 0, sz.cx, nBorderHeight);

	bmp.CreateCompatibleBitmap(&dcWindow, rcDest.Width(), rcDest.Height());

	CBitmap* pOldBitmap = dc.SelectObject(&bmp);

	BOOL bTransparent = pImage->IsAlphaImage()? FALSE : pClass->GetThemeBool(iPartId, FS_ACTIVE, TMT_TRANSPARENT);
	clrTransparent = COLORREF_NULL;
	if (bTransparent)
	{
		clrTransparent = pClass->GetThemeColor(iPartId, FS_ACTIVE, TMT_TRANSPARENTCOLOR, RGB(0xFF, 0, 0xFF));
	}

	dc.FillSolidRect(rcDest, clrTransparent != COLORREF_NULL ? clrTransparent : RGB(0xFF, 0, 0xFF));

	rcSizingMargins = pClass->GetThemeRect(iPartId, FS_ACTIVE, TMT_SIZINGMARGINS);

	if (rcSizingMargins.top + rcSizingMargins.bottom > rcDest.Height())
	{
		rcSizingMargins.bottom = max(0, rcDest.Height() - rcSizingMargins.top);
	}

	pImage->DrawImage(&dc, rcDest, rcSrc, rcSizingMargins, clrTransparent, ST_STRETCH, FALSE);

	dc.SelectObject(pOldBitmap);

	clrTransparent = clrTransparent != COLORREF_NULL ? clrTransparent : RGB(0xFF, 0, 0xFF);
	return TRUE;
};

CXTPSkinManagerSchema::FRAMEREGION* CXTPSkinManagerSchema::CreateFrameRegion(CXTPSkinObjectFrame* pFrame, CSize szDest)
{
	int nCaptionHeight = pFrame->GetBorders().top;
	int nBorderHeight = pFrame->GetBorders().left;
	BOOL bToolWindow = pFrame->GetExStyle() & WS_EX_TOOLWINDOW;

	for (int i = 0; i < m_arrFrameRegions.GetSize(); i++)
	{
		FRAMEREGION* pRegion = m_arrFrameRegions[i];

		if (nCaptionHeight == pRegion->nCaptionHeight && bToolWindow == pRegion->bToolWindow && nBorderHeight == pRegion->nBorderHeight)
		{
			if (szDest.cx < pRegion->Part[frameTop].rcSizingMargins.left + pRegion->Part[frameTop].rcSizingMargins.right)
				break;
			return pRegion;
		}
	}

	CBitmap bmp;
	CRect rcDest, rcSizingMargins;
	COLORREF clrTransparent;

	if (!DrawWindowPart(pFrame, bmp, bToolWindow ? WP_SMALLCAPTION : WP_CAPTION, FALSE, nCaptionHeight, rcDest, rcSizingMargins, clrTransparent))
		return NULL;

	FRAMEREGION* pRegion = new FRAMEREGION();
	pRegion->bAutoDelete = FALSE;
	pRegion->bToolWindow = bToolWindow;
	pRegion->nCaptionHeight = nCaptionHeight;
	pRegion->nBorderHeight = nBorderHeight;

	if (szDest.cx < rcSizingMargins.left + rcSizingMargins.right)
	{
		CRect rcDestSizingMargins = rcSizingMargins;

		rcDest.right = szDest.cx;
		pRegion->bAutoDelete = TRUE;

		rcSizingMargins.left = MulDiv(rcDest.Width(), rcDestSizingMargins.left, (rcDestSizingMargins.left + rcDestSizingMargins.right));
		rcSizingMargins.right = rcDest.Width() - rcDestSizingMargins.left;
	}
	pRegion->Part[frameTop].rcSizingMargins = rcSizingMargins;
	RegionFromBitmap(&bmp, frameTop, &pRegion->Part[frameTop], rcDest, clrTransparent);

	if (DrawWindowPart(pFrame, bmp, bToolWindow ? WP_SMALLFRAMELEFT : WP_FRAMELEFT, TRUE, nBorderHeight, rcDest, rcSizingMargins, clrTransparent))
	{
		pRegion->Part[frameLeft].rcSizingMargins = rcSizingMargins;
		RegionFromBitmap(&bmp, frameLeft, &pRegion->Part[frameLeft], rcDest, clrTransparent);
	}

	if (DrawWindowPart(pFrame, bmp, bToolWindow ? WP_SMALLFRAMERIGHT : WP_FRAMERIGHT, TRUE, nBorderHeight, rcDest, rcSizingMargins, clrTransparent))
	{
		pRegion->Part[frameRight].rcSizingMargins = rcSizingMargins;
		RegionFromBitmap(&bmp, frameRight, &pRegion->Part[frameRight], rcDest, clrTransparent);
	}

	if (DrawWindowPart(pFrame, bmp, bToolWindow ? WP_SMALLFRAMEBOTTOM : WP_FRAMEBOTTOM, FALSE, nBorderHeight, rcDest, rcSizingMargins, clrTransparent))
	{
		pRegion->Part[frameBottom].rcSizingMargins = rcSizingMargins;
		RegionFromBitmap(&bmp, frameBottom, &pRegion->Part[frameBottom], rcDest, clrTransparent);
	}

	if (!pRegion->bAutoDelete)
		m_arrFrameRegions.Add(pRegion);

	return pRegion;
}

void CXTPSkinManagerSchema::FreeFrameRegions()
{
	for (int i = 0; i < m_arrFrameRegions.GetSize(); i++)
	{
		delete m_arrFrameRegions[i];
	}
	m_arrFrameRegions.RemoveAll();
}

void CXTPSkinManagerSchema::RefreshMetrcis()
{
	FreeFrameRegions();

	m_pMetrics->RefreshMetrics();
}

HRGN CXTPSkinManagerSchema::CalcFrameRegion(CXTPSkinObjectFrame* pFrame, CSize sz)
{
	CRgn rgnResult;

	if ((pFrame->GetStyle() & WS_MAXIMIZE) && ((pFrame->GetStyle() & WS_CHILD) == 0))
	{
		int nBorder = pFrame->GetBorders().left;

		if (pFrame->GetExStyle() & WS_EX_CLIENTEDGE)
			nBorder -= 2;

		CRect rc = XTPMultiMonitor()->GetWorkArea(pFrame);
		rgnResult.CreateRectRgn(nBorder, nBorder, sz.cx - nBorder, sz.cy - nBorder);
		return (HRGN)rgnResult.Detach();
	}
	rgnResult.CreateRectRgn(0, 0, sz.cx, sz.cy);

	FRAMEREGION* pRegion = CreateFrameRegion(pFrame, sz);
	if (!pRegion)
	{
		return (HRGN)rgnResult.Detach();
	}

	FRAMEREGIONPART* pPart = &pRegion->Part[frameTop];
	if (pPart->rgnLeft.GetSafeHandle())
	{
		rgnResult.CombineRgn(&rgnResult, &pPart->rgnLeft, RGN_DIFF);
	}

	if (pPart->rgnRight.GetSafeHandle())
	{
		CRgn rgnRight;
		rgnRight.CreateRectRgn(0, 0, 0, 0);
		rgnRight.CopyRgn(&pPart->rgnRight);

		rgnRight.OffsetRgn(sz.cx - pPart->rcSizingMargins.right, 0);

		rgnResult.CombineRgn(&rgnResult, &rgnRight, RGN_DIFF);
	}

	if (pPart->nCenter > 0)
	{
		CRgn rgnCenter;
		rgnCenter.CreateRectRgn(pPart->rcSizingMargins.left, 0, sz.cx - pPart->rcSizingMargins.right, pPart->nCenter);

		rgnResult.CombineRgn(&rgnResult, &rgnCenter, RGN_DIFF);
	}

	pPart = &pRegion->Part[frameBottom];
	if (pPart->rgnLeft.GetSafeHandle())
	{
		CRgn rgnLeft;
		rgnLeft.CreateRectRgn(0, 0, 0, 0);
		rgnLeft.CopyRgn(&pPart->rgnLeft);

		rgnLeft.OffsetRgn(0, sz.cy - pRegion->nBorderHeight);

		rgnResult.CombineRgn(&rgnResult, &rgnLeft, RGN_DIFF);
	}

	if (pPart->rgnRight.GetSafeHandle())
	{
		CRgn rgnRight;
		rgnRight.CreateRectRgn(0, 0, 0, 0);
		rgnRight.CopyRgn(&pPart->rgnRight);

		rgnRight.OffsetRgn(sz.cx - pPart->rcSizingMargins.right, sz.cy - pRegion->nBorderHeight);

		rgnResult.CombineRgn(&rgnResult, &rgnRight, RGN_DIFF);
	}

	if (pPart->nCenter > 0)
	{
		CRgn rgnCenter;
		rgnCenter.CreateRectRgn(pPart->rcSizingMargins.left, sz.cy - pPart->nCenter, sz.cx - pPart->rcSizingMargins.right, sz.cy);

		rgnResult.CombineRgn(&rgnResult, &rgnCenter, RGN_DIFF);
	}

	pPart = &pRegion->Part[frameLeft];

	if (pPart->rgnLeft.GetSafeHandle())
	{
		CRgn rgnLeft;
		rgnLeft.CreateRectRgn(0, 0, 0, 0);
		rgnLeft.CopyRgn(&pPart->rgnLeft);

		rgnLeft.OffsetRgn(0, pRegion->nCaptionHeight);

		rgnResult.CombineRgn(&rgnResult, &rgnLeft, RGN_DIFF);
	}

	if (pPart->rgnRight.GetSafeHandle())
	{
		CRgn rgnRight;
		rgnRight.CreateRectRgn(0, 0, 0, 0);
		rgnRight.CopyRgn(&pPart->rgnRight);

		rgnRight.OffsetRgn(0, sz.cy - pRegion->nBorderHeight - pPart->rcSizingMargins.bottom);

		rgnResult.CombineRgn(&rgnResult, &rgnRight, RGN_DIFF);
	}
	if (pPart->nCenter > 0)
	{
		CRgn rgnCenter;
		rgnCenter.CreateRectRgn(0, pRegion->nCaptionHeight + pPart->rcSizingMargins.top, pPart->nCenter, sz.cy - pRegion->nBorderHeight - pPart->rcSizingMargins.bottom);

		rgnResult.CombineRgn(&rgnResult, &rgnCenter, RGN_DIFF);
	}

	pPart = &pRegion->Part[frameRight];

	if (pPart->rgnLeft.GetSafeHandle())
	{
		CRgn rgnLeft;
		rgnLeft.CreateRectRgn(0, 0, 0, 0);
		rgnLeft.CopyRgn(&pPart->rgnLeft);

		rgnLeft.OffsetRgn(sz.cx - pRegion->nBorderHeight, pRegion->nCaptionHeight);

		rgnResult.CombineRgn(&rgnResult, &rgnLeft, RGN_DIFF);
	}

	if (pPart->rgnRight.GetSafeHandle())
	{
		CRgn rgnRight;
		rgnRight.CreateRectRgn(0, 0, 0, 0);
		rgnRight.CopyRgn(&pPart->rgnRight);

		rgnRight.OffsetRgn(sz.cx - pRegion->nBorderHeight, sz.cy - pRegion->nBorderHeight - pPart->rcSizingMargins.bottom);

		rgnResult.CombineRgn(&rgnResult, &rgnRight, RGN_DIFF);
	}

	if (pPart->nCenter > 0)
	{
		CRgn rgnCenter;
		rgnCenter.CreateRectRgn(sz.cx - pPart->nCenter, pRegion->nCaptionHeight + pPart->rcSizingMargins.top, sz.cx, sz.cy - pRegion->nBorderHeight - pPart->rcSizingMargins.bottom);

		rgnResult.CombineRgn(&rgnResult, &rgnCenter, RGN_DIFF);
	}

	if (pRegion->bAutoDelete)
		delete pRegion;

	return (HRGN)rgnResult.Detach();
}

COLORREF CXTPSkinManagerSchema::GetColor(int nIndex) const
{
	return GetMetrics()->GetColor(nIndex);
}

COLORREF CXTPSkinManagerSchema::GetScrollBarSizeBoxColor(CXTPSkinObjectFrame* /*pFrame*/)
{
	return GetColor(COLOR_3DFACE);
}

void CXTPSkinManagerSchema::DrawThemeScrollBar(CDC* pDC, CXTPSkinObjectFrame* pFrame, XTP_SKINSCROLLBAR_POSINFO *pSBInfo)
{
	#define ABS_UPACTIVE  17
	#define ABS_DOWNACTIVE 18
	#define ABS_LEFTACTIVE 19
	#define ABS_RIGHTACTIVE 20

	#define GETPARTSTATE(ht, pressed, hot, normal, disabled, active) \
		(!bEnabled ? disabled : nPressedHt == ht ? pressed : \
		nHotHt == ht ? hot : bActive ? (active) : normal)

	XTP_SKINSCROLLBAR_TRACKINFO *pSBTrack = pFrame->GetScrollBarTrackInfo();

	BOOL nPressedHt = pSBTrack && pSBTrack->pSBInfo == pSBInfo ?
		(pSBTrack->bTrackThumb || pSBTrack->fHitOld ? pSBInfo->ht : -1) : -1;

	BOOL nHotHt = pSBTrack ? -1 : pSBInfo->ht;
	BOOL bActive = (pSBTrack && pSBTrack->pSBInfo == pSBInfo) || nHotHt  > 0 ;


	int cWidth = (pSBInfo->pxRight - pSBInfo->pxLeft);
	BOOL fVert = pSBInfo->fVert;

	if (cWidth <= 0)
	{
		return;
	}

	HBRUSH hBrush = pFrame->GetClientBrush(pDC);
	FillRect(pDC->GetSafeHdc(), &pSBInfo->rc, hBrush);

	BOOL bEnabled = pSBInfo->posMax - pSBInfo->posMin - pSBInfo->page + 1 > 0;
	if (bEnabled && pSBInfo->nBar == SB_CTL)
		bEnabled = (pFrame->GetStyle() & WS_DISABLED) == 0;

	CXTPSkinManagerClass* pClassScrollBar = m_pManager->GetSkinClass(pFrame, _T("SCROLLBAR"));

	int nBtnTrackSize =   pSBInfo->pxThumbBottom - pSBInfo->pxThumbTop;
	int nBtnTrackPos = pSBInfo->pxThumbTop - pSBInfo->pxUpArrow;

	if (!bEnabled || pSBInfo->pxThumbBottom > pSBInfo->pxDownArrow)
		nBtnTrackPos = nBtnTrackSize = 0;

	int nStateCount = pClassScrollBar->GetThemeInt(SBP_ARROWBTN, 0, TMT_IMAGECOUNT, 16);


	if(!pSBInfo->fSizebox)
	{
		if (!fVert)
		{
			CRect rcHScroll(pSBInfo->rc);

			CRect rcArrowLeft(rcHScroll.left, rcHScroll.top, pSBInfo->pxUpArrow, rcHScroll.bottom);
			CRect rcArrowRight(pSBInfo->pxDownArrow, rcHScroll.top, rcHScroll.right, rcHScroll.bottom);

			pClassScrollBar->DrawThemeBackground(pDC, SBP_ARROWBTN, GETPARTSTATE(XTP_HTSCROLLUP,
				ABS_LEFTPRESSED, ABS_LEFTHOT, ABS_LEFTNORMAL, ABS_LEFTDISABLED, nStateCount == 20 ? ABS_LEFTACTIVE : ABS_LEFTNORMAL),  rcArrowLeft);
			pClassScrollBar->DrawThemeBackground(pDC, SBP_ARROWBTN, GETPARTSTATE(XTP_HTSCROLLDOWN,
				ABS_RIGHTPRESSED, ABS_RIGHTHOT, ABS_RIGHTNORMAL, ABS_RIGHTDISABLED, nStateCount == 20 ? ABS_RIGHTACTIVE : ABS_RIGHTNORMAL),  rcArrowRight);

			CRect rcTrack(rcArrowLeft.right, rcHScroll.top, rcArrowRight.left, rcHScroll.bottom);

			if (!rcTrack.IsRectEmpty())
			{
				CRect rcLowerTrack(rcTrack.left, rcTrack.top, rcTrack.left + nBtnTrackPos, rcTrack.bottom);
				CRect rcBtnTrack(rcLowerTrack.right, rcTrack.top, rcLowerTrack.right + nBtnTrackSize, rcTrack.bottom);
				CRect rcUpperTrack(rcBtnTrack.right, rcTrack.top, rcTrack.right, rcTrack.bottom);

				if (!rcLowerTrack.IsRectEmpty())
					pClassScrollBar->DrawThemeBackground(pDC, SBP_LOWERTRACKHORZ, GETPARTSTATE(XTP_HTSCROLLUPPAGE, SCRBS_PRESSED, SCRBS_HOT, SCRBS_NORMAL, SCRBS_DISABLED, SCRBS_NORMAL),  rcLowerTrack);

				if (!rcBtnTrack.IsRectEmpty())
				{
					pClassScrollBar->DrawThemeBackground(pDC, SBP_THUMBBTNHORZ, GETPARTSTATE(XTP_HTSCROLLTHUMB, SCRBS_PRESSED, SCRBS_HOT, SCRBS_NORMAL, SCRBS_DISABLED, SCRBS_NORMAL),  rcBtnTrack);

					if (rcBtnTrack.Width() > 9)
						pClassScrollBar->DrawThemeBackground(pDC, SBP_GRIPPERHORZ, GETPARTSTATE(XTP_HTSCROLLTHUMB, SCRBS_PRESSED, SCRBS_HOT, SCRBS_NORMAL, SCRBS_DISABLED, SCRBS_NORMAL),  rcBtnTrack);
				}

				if (!rcUpperTrack.IsRectEmpty())
					pClassScrollBar->DrawThemeBackground(pDC, SBP_UPPERTRACKHORZ, GETPARTSTATE(XTP_HTSCROLLDOWNPAGE, SCRBS_PRESSED, SCRBS_HOT, SCRBS_NORMAL, SCRBS_DISABLED, SCRBS_NORMAL),  rcUpperTrack);
			}
		}
		else
		{
			CRect rcVScroll(pSBInfo->rc);

			CRect rcArrowUp(rcVScroll.left, rcVScroll.top, rcVScroll.right, pSBInfo->pxUpArrow);
			CRect rcArrowDown(rcVScroll.left, pSBInfo->pxDownArrow, rcVScroll.right, rcVScroll.bottom);

			pClassScrollBar->DrawThemeBackground(pDC, SBP_ARROWBTN, GETPARTSTATE(XTP_HTSCROLLUP,
				ABS_UPPRESSED, ABS_UPHOT, ABS_UPNORMAL, ABS_UPDISABLED,  nStateCount == 20 ? ABS_UPACTIVE : ABS_UPNORMAL),  rcArrowUp);
			pClassScrollBar->DrawThemeBackground(pDC, SBP_ARROWBTN, GETPARTSTATE(XTP_HTSCROLLDOWN,
				ABS_DOWNPRESSED, ABS_DOWNHOT, ABS_DOWNNORMAL, ABS_DOWNDISABLED, nStateCount == 20 ? ABS_DOWNACTIVE : ABS_DOWNNORMAL),  rcArrowDown);

			CRect rcTrack(rcVScroll.left, rcArrowUp.bottom, rcVScroll.right, rcArrowDown.top);

			if (!rcTrack.IsRectEmpty())
			{
				CRect rcLowerTrack(rcTrack.left, rcTrack.top, rcTrack.right, rcTrack.top + nBtnTrackPos);
				CRect rcBtnTrack(rcTrack.left, rcLowerTrack.bottom, rcTrack.right, rcLowerTrack.bottom + nBtnTrackSize);
				CRect rcUpperTrack(rcTrack.left, rcBtnTrack.bottom, rcTrack.right, rcTrack.bottom);

				if (!rcLowerTrack.IsRectEmpty())
					pClassScrollBar->DrawThemeBackground(pDC, SBP_LOWERTRACKVERT, GETPARTSTATE(XTP_HTSCROLLUPPAGE, SCRBS_PRESSED, SCRBS_HOT, SCRBS_NORMAL, SCRBS_DISABLED, SCRBS_NORMAL),  rcLowerTrack);

				if (!rcBtnTrack.IsRectEmpty())
				{
					pClassScrollBar->DrawThemeBackground(pDC, SBP_THUMBBTNVERT, GETPARTSTATE(XTP_HTSCROLLTHUMB, SCRBS_PRESSED, SCRBS_HOT, SCRBS_NORMAL, SCRBS_DISABLED, SCRBS_NORMAL),  rcBtnTrack);
					if (rcBtnTrack.Height() > 13)
						pClassScrollBar->DrawThemeBackground(pDC, SBP_GRIPPERVERT, GETPARTSTATE(XTP_HTSCROLLTHUMB, SCRBS_PRESSED, SCRBS_HOT, SCRBS_NORMAL, SCRBS_DISABLED, SCRBS_NORMAL),  rcBtnTrack);
				}

				if (!rcUpperTrack.IsRectEmpty())
					pClassScrollBar->DrawThemeBackground(pDC, SBP_UPPERTRACKVERT, GETPARTSTATE(XTP_HTSCROLLDOWNPAGE, SCRBS_PRESSED, SCRBS_HOT, SCRBS_NORMAL, SCRBS_DISABLED, SCRBS_NORMAL),  rcUpperTrack);
			}
		}
	}
	else
	{
		pClassScrollBar->DrawThemeBackground(pDC, SBP_SIZEBOX, 0, &pSBInfo->rc);
	}
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPSkinManagerSchema, CCmdTarget)
END_DISPATCH_MAP()

// {128507E0-C56F-43C0-BCF1-8193B35FE4C4}
static const GUID IID_ISkinManagerSchema =
{ 0x128507e0, 0xc56f, 0x43c0, { 0xbc, 0xf1, 0x81, 0x93, 0xb3, 0x5f, 0xe4, 0xc4 } };

BEGIN_INTERFACE_MAP(CXTPSkinManagerSchema, CCmdTarget)
	INTERFACE_PART(CXTPSkinManagerSchema, IID_ISkinManagerSchema, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPSkinManagerSchema, IID_ISkinManagerSchema)

#endif
