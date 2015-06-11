// XTPMarkupPathData.h: interface for the CXTPMarkupPathData class.
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
#if !defined(__XTPMARKUPPATHDATA_H__)
#define __XTPMARKUPPATHDATA_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class _XTP_EXT_CLASS CXTPMarkupPathData : public CXTPMarkupObject
{
public:

	DECLARE_MARKUPCLASS(CXTPMarkupPathData)
public:
	CXTPMarkupPathData();
	CXTPMarkupPathData(const MARKUP_POINTF* pPoints, const BYTE* pTypes, int nCount);

	~CXTPMarkupPathData();

public:
	MARKUP_POINTF* GetPoints() const;
	int GetCount() const;
	BYTE* GetTypes() const;

	CRect GetBounds() const;

	CXTPMarkupPathData* Stretch(CSize sz) const;

protected:
	void SetContentObject(CXTPMarkupBuilder *pBuilder, CXTPMarkupObject *pContent);
	CXTPMarkupObject* ConvertFrom(CXTPMarkupBuilder* pBuilder, CXTPMarkupObject* pObject) const;
	void UpdateBounds();

protected:
	MARKUP_POINTF* m_pPoints;
	int m_nCount;
	BYTE* m_pTypes;
	CRect m_rcBounds;
public:
	int m_nFillRule;
	int m_nPixelOffsetMode;

public:
	static CXTPMarkupDependencyProperty *m_pPropertyGeometry;
};

AFX_INLINE MARKUP_POINTF* CXTPMarkupPathData::GetPoints() const
{
	return m_pPoints;
}

AFX_INLINE int CXTPMarkupPathData::GetCount() const
{
	return m_nCount;
}

AFX_INLINE BYTE* CXTPMarkupPathData::GetTypes() const
{
	return m_pTypes;
}

AFX_INLINE CRect CXTPMarkupPathData::GetBounds() const
{
	return m_rcBounds;
}

#endif // !defined(__XTPMARKUPPATHDATA_H__)
