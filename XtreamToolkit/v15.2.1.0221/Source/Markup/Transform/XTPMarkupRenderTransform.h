// XTPMarkupRenderTransform.h: interface for the CXTPMarkupRenderTransform class.
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

//{{AFX_CODEJOCK_PRIVATE
#if !defined(__XTPMARKUPRENDERTRANSFORM_H__)
#define __XTPMARKUPRENDERTRANSFORM_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXTPMarkupRotateTransform;

class _XTP_EXT_CLASS CXTPMarkupRenderTransform : public CXTPMarkupObject
{
	DECLARE_MARKUPCLASS(CXTPMarkupRenderTransform);

public:

	CXTPMarkupRotateTransform* GetRotateTransform() const;

protected:
	void SetContentObject(CXTPMarkupBuilder *pBuilder, CXTPMarkupObject *pContent);

public:
	static CXTPMarkupDependencyProperty *m_pPropertyRotateTransform;
};

#endif // !defined(__XTPMARKUPRENDERTRANSFORM_H__)
