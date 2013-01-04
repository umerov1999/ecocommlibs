// XTPMarkupGridLength.cpp: implementation of the CXTPMarkupGridLength class.
//
// This file is a part of the XTREME TOOLKIT PRO MFC class library.
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

#include "../XTPMarkupObject.h"
#include "../XTPMarkupInputElement.h"
#include "../XTPMarkupUIElement.h"
#include "../XTPMarkupFrameworkElement.h"
#include "../XTPMarkupPanel.h"
#include "../XTPMarkupGrid.h"
#include "../XTPMarkupBuilder.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////////
// CXTPMarkupGridLength

IMPLEMENT_MARKUPCLASS(NULL, CXTPMarkupGridLength, CXTPMarkupObject);

void CXTPMarkupGridLength::RegisterMarkupClass()
{

}

CXTPMarkupGridLength::CXTPMarkupGridLength(double nValue, CXTPMarkupGridLength::GridUnitType type)
{
	m_nValue = nValue;
	m_type = type;
}


CXTPMarkupObject* CXTPMarkupGridLength::ConvertFrom(CXTPMarkupBuilder *pBuilder, CXTPMarkupObject *pObject) const
{
	UNREFERENCED_PARAMETER(pBuilder);

	if (IsStringObject(pObject))
	{
		LPCWSTR lpszValue = *((CXTPMarkupString*)pObject);
		int nLength = ((CXTPMarkupString*)pObject)->GetLength();
		if (nLength < 1)
			return NULL;

		if (nLength == 4 && _wcsicmp(lpszValue, L"Auto") == 0)
		{
			return new CXTPMarkupGridLength(1, unitTypeAuto);
		}

		double dValue = 0;

		if (lpszValue[nLength - 1] == '*')
		{
			if (nLength == 1)
				return new CXTPMarkupGridLength(1, unitTypeStar);

			if (!CXTPMarkupBuilder::ConvertDouble(lpszValue, dValue , '*'))
				return NULL;

			return new CXTPMarkupGridLength(dValue, unitTypeStar);
		}

		if (!CXTPMarkupBuilder::ConvertDouble(lpszValue, dValue))
			return NULL;

		return new CXTPMarkupGridLength(dValue);
	}
	return NULL;
}
