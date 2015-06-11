// XTPMarkupThickness.cpp: implementation of the CXTPMarkupThickness class.
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

#include "Common/XTPVc80Helpers.h"
#include "Common/XTPResourceManager.h"

#include "XTPMarkupObject.h"
#include "XTPMarkupBuilder.h"
#include "XTPMarkupThickness.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPMarkupThickness
IMPLEMENT_MARKUPCLASS(NULL, CXTPMarkupThickness, CXTPMarkupObject);

void CXTPMarkupThickness::RegisterMarkupClass()
{

}

CXTPMarkupThickness* CXTPMarkupThickness::CreateValue()
{
	static CXTPMarkupAutoPtr defaultValues(new CXTPMarkupThickness());
	return (CXTPMarkupThickness*)defaultValues.AddRef();
}

CXTPMarkupThickness::CXTPMarkupThickness()
	: left  (0)
	, top   (0)
	, right (0)
	, bottom(0)
{
	// Nothing
}

CXTPMarkupThickness::CXTPMarkupThickness(const CXTPMarkupThickness &other)
	: left  (other.left)
	, top   (other.top)
	, right (other.right)
	, bottom(other.bottom)
{
	// Nothing
}

CXTPMarkupThickness::CXTPMarkupThickness(long nThickness)
	: left  (nThickness)
	, top   (nThickness)
	, right (nThickness)
	, bottom(nThickness)
{
	// Nothing
}

CXTPMarkupThickness::CXTPMarkupThickness(long nLeft, long nTop, long nRight, long nBottom)
	: left  (nLeft)
	, top   (nTop)
	, right (nRight)
	, bottom(nBottom)
{
	// Nothing
}

CRect CXTPMarkupThickness::HelperDeflateRect(CRect rt, CXTPMarkupThickness* thick)
{
	if (!thick)
		return rt;

	return CRect(CPoint(rt.left + thick->left, rt.top + thick->top),
		CSize(max(0, (rt.Width() - thick->left) - thick->right), max(0, (rt.Height() - thick->top) - thick->bottom)));
}


CXTPMarkupObject* CXTPMarkupThickness::ConvertFrom(CXTPMarkupBuilder *pBuilder, CXTPMarkupObject* pObject) const
{
	UNREFERENCED_PARAMETER(pBuilder);

	if (IsStringObject(pObject))
	{
		LPCWSTR lpszValue = *((CXTPMarkupString*)pObject);
		int nLength = ((CXTPMarkupString*)pObject)->GetLength();

		if (wcschr(lpszValue, L',') == NULL)
		{
			int nThickness = _wtoi(lpszValue);

			if ((nLength > 2) && _wcsicmp(lpszValue + nLength - 2, L"pt") == 0)
			{
				return new CXTPMarkupThickness(MulDiv(nThickness, 96, 72));
			}

			if ((nLength > 2) && _wcsicmp(lpszValue + nLength - 2, L"in") == 0)
			{
				return new CXTPMarkupThickness(nThickness * 96);
			}

			if ((nLength > 2) && _wcsicmp(lpszValue + nLength - 2, L"cm") == 0)
			{
				return new CXTPMarkupThickness(int((double)nThickness * 37.79528));
			}

			if ((nLength > 2) && _wcsicmp(lpszValue + nLength - 2, L"mm") == 0)
			{
				return new CXTPMarkupThickness(int((double)nThickness * 3.779528));
			}

			return new CXTPMarkupThickness(nThickness);
		}
		else
		{
			if ((nLength > 2) && _wcsicmp(lpszValue + nLength - 2, L"pt") == 0)
			{
				int left = 0, top = 0, right = 0, bottom = 0;
				if ( WSCANF_S(lpszValue, L"%ipt, %ipt, %ipt, %ipt", &left, &top, &right, &bottom) != 4)
					return NULL;

				return new CXTPMarkupThickness(MulDiv(left, 96, 72), MulDiv(top, 96, 72), MulDiv(right, 96, 72), MulDiv(bottom, 96, 72));
			}

			if ((nLength > 2) && _wcsicmp(lpszValue + nLength - 2, L"in") == 0)
			{
				int left = 0, top = 0, right = 0, bottom = 0;
				if ( WSCANF_S(lpszValue, L"%iin, %iin, %iin, %iin", &left, &top, &right, &bottom) != 4)
					return NULL;

				return new CXTPMarkupThickness(left * 96, top * 96, right * 96, bottom * 96);
			}

			if ((nLength > 2) && _wcsicmp(lpszValue + nLength - 2, L"cm") == 0)
			{
				int left = 0, top = 0, right = 0, bottom = 0;
				if ( WSCANF_S(lpszValue, L"%icm, %icm, %icm, %icm", &left, &top, &right, &bottom) != 4)
					return NULL;

				return new CXTPMarkupThickness(int((double)left * 37.79528), int((double)top * 37.79528), int((double)right * 37.79528), int((double)bottom * 37.79528));
			}

			if ((nLength > 2) && _wcsicmp(lpszValue + nLength - 2, L"mm") == 0)
			{
				int left = 0, top = 0, right = 0, bottom = 0;
				if ( WSCANF_S(lpszValue, L"%imm, %imm, %imm, %imm", &left, &top, &right, &bottom) != 4)
					return NULL;

				return new CXTPMarkupThickness(int((double)left * 3.779528), int((double)top * 3.779528), int((double)right * 3.779528), int((double)bottom * 3.779528));
			}

			int left = 0, top = 0, right = 0, bottom = 0;
			if ( WSCANF_S(lpszValue, L"%i, %i, %i, %i", &left, &top, &right, &bottom) != 4)
				return NULL;

			return new CXTPMarkupThickness(left, top, right, bottom);
		}

	}

	return NULL;
}

BOOL CXTPMarkupThickness::IsEqual(const CXTPMarkupObject *pObject) const
{
	if (!pObject)
		return FALSE;

	if (pObject->GetType() != MARKUP_TYPE(CXTPMarkupThickness))
		return FALSE;

	const CXTPMarkupThickness *pOther = reinterpret_cast<const CXTPMarkupThickness*>(pObject);

	return left   == pOther->left  &&
			 top    == pOther->top   &&
			 right  == pOther->right &&
			 bottom == pOther->bottom;
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPMarkupThickness, CXTPMarkupObject)
	DISP_PROPERTY_ID(CXTPMarkupThickness, "Left", 100, left, VT_I4)
	DISP_PROPERTY_ID(CXTPMarkupThickness, "Top", 101, top, VT_I4)
	DISP_PROPERTY_ID(CXTPMarkupThickness, "Right", 102, right, VT_I4)
	DISP_PROPERTY_ID(CXTPMarkupThickness, "Bottom", 103, bottom, VT_I4)
END_DISPATCH_MAP()

// {10139842-AE6D-46a8-BC77-CEE7D5CE9ED7}
static const GUID IID_IMarkupThickness =
{ 0x10139842, 0xae6d, 0x46a8, { 0xbc, 0x77, 0xce, 0xe7, 0xd5, 0xce, 0x9e, 0xd7 } };


BEGIN_INTERFACE_MAP(CXTPMarkupThickness, CXTPMarkupObject)
INTERFACE_PART(CXTPMarkupThickness, IID_IMarkupThickness, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPMarkupThickness, IID_IMarkupThickness)


#endif
