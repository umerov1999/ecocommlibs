// XTPMarkupPathGeometry.h: interface for the CXTPMarkupPathGeometry class.
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

//{{AFX_CODEJOCK_PRIVATE
#if !defined(__XTPMARKUPPATHGEOMETRY_H__)
#define __XTPMARKUPPATHGEOMETRY_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class _XTP_EXT_CLASS CXTPMarkupPathGeometry : public CXTPMarkupObject
{
	DECLARE_MARKUPCLASS(CXTPMarkupPathGeometry);

protected:
	CXTPMarkupPathGeometry();
	~CXTPMarkupPathGeometry();

protected:
	void SetContentObject(CXTPMarkupBuilder *pBuilder, CXTPMarkupObject *pContent);
	CXTPMarkupObject* ConvertFrom(CXTPMarkupBuilder *pBuilder, CXTPMarkupObject *pObject) const;

public:
	static CXTPMarkupDependencyProperty *m_pPropertyGeometryFigures;
};



#endif // !defined(__XTPMARKUPPATHGEOMETRY_H__)
