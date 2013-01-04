// XTPMarkupPath.h: interface for the CXTPMarkupPath class.
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
#if !defined(__XTPMARKUPPATH_H__)
#define __XTPMARKUPPATH_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class _XTP_EXT_CLASS CXTPMarkupPath : public CXTPMarkupShape
{
	DECLARE_MARKUPCLASS(CXTPMarkupPath)

protected:
	CXTPMarkupPath();
	~CXTPMarkupPath();

public:
	CXTPMarkupPathData* GetData() const;

protected:
	void SetContentObject(CXTPMarkupBuilder *pBuilder, CXTPMarkupObject *pContent);

protected:
	virtual CSize MeasureOverride(CXTPMarkupDrawingContext* pDC, CSize szAvailableSize);
	virtual void OnRender(CXTPMarkupDrawingContext* pDC);

#ifdef _XTP_ACTIVEX
//{{AFX_CODEJOCK_PRIVATE
public:
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPMarkupPath);
//}}AFX_CODEJOCK_PRIVATE
#endif

public:
	static CXTPMarkupDependencyProperty *m_pPropertyData;
};

#endif // !defined(__XTPMARKUPPATH_H__)
