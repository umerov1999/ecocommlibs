// XTPMarkupRotateTransform.h: interface for the CXTPMarkupRotateTransform class.
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
#if !defined(__XTPMARKUPROTATETRANSFORM_H__)
#define __XTPMARKUPROTATETRANSFORM_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class _XTP_EXT_CLASS CXTPMarkupRotateTransform : public CXTPMarkupObject
{
	DECLARE_MARKUPCLASS(CXTPMarkupRotateTransform);

public:

	int GetAngle() const;

public:

	static CXTPMarkupDependencyProperty *m_pPropertyAngle;
};

#endif // !defined(__XTPMARKUPROTATETRANSFORM_H__)
