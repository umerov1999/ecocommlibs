// XTPMarkupRenderTransform.cpp: implementation of the CXTPMarkupRenderTransform class.
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
#include "XTPMarkupRenderTransform.h"
#include "XTPMarkupRotateTransform.h"


//////////////////////////////////////////////////////////////////////////
// CXTPMarkupRenderTransform

CXTPMarkupDependencyProperty* CXTPMarkupRenderTransform::m_pPropertyRotateTransform = NULL;

IMPLEMENT_MARKUPCLASS(L"RenderTransform", CXTPMarkupRenderTransform, CXTPMarkupObject);

void CXTPMarkupRenderTransform::RegisterMarkupClass()
{
	m_pPropertyRotateTransform = CXTPMarkupDependencyProperty::Register(L"RotateTransform",
		MARKUP_TYPE(CXTPMarkupRotateTransform), MARKUP_TYPE(CXTPMarkupRenderTransform));
}

void CXTPMarkupRenderTransform::SetContentObject(CXTPMarkupBuilder *pBuilder, CXTPMarkupObject *pContent)
{
	UNREFERENCED_PARAMETER(pBuilder);

	if (pContent->IsKindOf(MARKUP_TYPE(CXTPMarkupRotateTransform)))
	{
		SetValue(m_pPropertyRotateTransform, pContent);
	}
	else
	{
		CXTPMarkupObject::SetContentObject(pBuilder, pContent);
	}
}


CXTPMarkupRotateTransform* CXTPMarkupRenderTransform::GetRotateTransform() const
{
	return MARKUP_STATICCAST(CXTPMarkupRotateTransform, GetValue(m_pPropertyRotateTransform));
}
