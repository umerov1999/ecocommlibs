// XTPMarkupRotateTransform.cpp: implementation of the CXTPMarkupRotateTransform class.
//
// This file is a part of the XTREME TOOLKIT PRO MFC class library.
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

#include "../XTPMarkupObject.h"
#include "XTPMarkupRotateTransform.h"

//////////////////////////////////////////////////////////////////////////
// CXTPMarkupRotateTransform

CXTPMarkupDependencyProperty* CXTPMarkupRotateTransform::m_pPropertyAngle = NULL;

IMPLEMENT_MARKUPCLASS(L"RotateTransform", CXTPMarkupRotateTransform, CXTPMarkupObject);

void CXTPMarkupRotateTransform::RegisterMarkupClass()
{
	m_pPropertyAngle = CXTPMarkupDependencyProperty::Register(L"Angle",
		MARKUP_TYPE(CXTPMarkupInt), MARKUP_TYPE(CXTPMarkupRotateTransform));
}


int CXTPMarkupRotateTransform::GetAngle() const
{
	int nAngle = 0;

	CXTPMarkupInt *pAngle = MARKUP_STATICCAST(CXTPMarkupInt, GetValue(m_pPropertyAngle));

	if (NULL != pAngle)
	{
		nAngle = pAngle->GetValue();
	}

	return nAngle;
}
