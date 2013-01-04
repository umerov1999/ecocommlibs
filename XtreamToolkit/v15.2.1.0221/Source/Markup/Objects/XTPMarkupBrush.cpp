// XTPMarkupBrush.cpp: implementation of the CXTPMarkupBrush class.
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
#include "../XTPMarkupDrawingContext.h"


//////////////////////////////////////////////////////////////////////////
// CXTPMarkupBrush

IMPLEMENT_MARKUPCLASS(NULL, CXTPMarkupBrush, CXTPMarkupObject);

void CXTPMarkupBrush::RegisterMarkupClass()
{

}

COLORREF CXTPMarkupBrush::GetHintColor() const
{
	return 0;
}

CXTPMarkupObject* CXTPMarkupBrush::ConvertFrom(CXTPMarkupBuilder *pBuilder, CXTPMarkupObject *pObject) const
{
	UNREFERENCED_PARAMETER(pBuilder);

	if (IsStringObject(pObject))
	{
		COLORREF clr;
		if (CXTPMarkupColor::ConvertFromString(*((CXTPMarkupString*)pObject), clr))
		{
			return new CXTPMarkupSolidColorBrush((BYTE)(clr >> 24), clr & 0xFFFFFF);
		}
	}

	return NULL;
}


#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPMarkupBrush, CXTPMarkupObject)
END_DISPATCH_MAP()

// {10159842-AE6D-46a8-BC77-CEE7D5CE9ED7}
static const GUID IID_IMarkupBrush =
{ 0x10159842, 0xae6d, 0x46a8, { 0xbc, 0x77, 0xce, 0xe7, 0xd5, 0xce, 0x9e, 0xd7 } };


BEGIN_INTERFACE_MAP(CXTPMarkupBrush, CXTPMarkupObject)
	INTERFACE_PART(CXTPMarkupBrush, IID_IMarkupBrush, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPMarkupBrush, IID_IMarkupBrush)

#endif
