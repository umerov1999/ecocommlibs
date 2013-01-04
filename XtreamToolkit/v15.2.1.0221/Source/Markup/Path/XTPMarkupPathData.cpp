// XTPMarkupPathData.cpp: implementation of the CXTPMarkupPathData class.
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
#include <math.h>

#include <Common/XTPVC80Helpers.h>

#include <Markup/XTPMarkupObject.h>
#include <Markup/XTPMarkupInputElement.h>
#include <Markup/XTPMarkupUIElement.h>
#include <Markup/XTPMarkupFrameworkElement.h>
#include <Markup/XTPMarkupShape.h>
#include <Markup/Path/XTPMarkupPath.h>
#include <Markup/Path/XTPMarkupPathData.h>
#include <Markup/Path/XTPMarkupPathGeometry.h>


//////////////////////////////////////////////////////////////////////////
// CXTPMarkupPathData

IMPLEMENT_MARKUPCLASS(L"Data", CXTPMarkupPathData, CXTPMarkupObject)

CXTPMarkupDependencyProperty* CXTPMarkupPathData::m_pPropertyGeometry = NULL;

void CXTPMarkupPathData::RegisterMarkupClass()
{
	m_pPropertyGeometry = CXTPMarkupDependencyProperty::Register(L"PathGeometry",
		MARKUP_TYPE(CXTPMarkupPathGeometry), MARKUP_TYPE(CXTPMarkupPathData));
}

CXTPMarkupPathData::CXTPMarkupPathData()
{
	m_nCount = 0;
	m_pPoints = NULL;
	m_pTypes = NULL;
	m_rcBounds.SetRectEmpty();
	m_nFillRule = 0;
	m_nPixelOffsetMode = 0;
}

CXTPMarkupPathData::CXTPMarkupPathData(const MARKUP_POINTF* pPoints, const BYTE* pTypes, int nCount)
{
	m_nCount = nCount;
	m_pPoints = NULL;
	m_pTypes = NULL;
	m_nFillRule = 0;
	m_nPixelOffsetMode = 0;

	m_rcBounds.SetRectEmpty();

	if (nCount > 0)
	{
		m_pPoints = new MARKUP_POINTF[nCount];
		MEMCPY_S(m_pPoints, pPoints, sizeof(MARKUP_POINTF) * nCount);

		m_pTypes = new BYTE[nCount];
		MEMCPY_S(m_pTypes, pTypes, sizeof(BYTE) * nCount);
	}

	UpdateBounds();
}

void CXTPMarkupPathData::UpdateBounds()
{
	m_rcBounds.SetRectEmpty();
	if (m_nCount > 0)
	{
		m_rcBounds.SetRect(INT_MAX, INT_MAX, -INT_MAX, -INT_MAX);

		for (int i = 0; i < m_nCount; i++)
		{
			const MARKUP_POINTF& pt = m_pPoints[i];
			if (pt.x > m_rcBounds.right)  m_rcBounds.right  = (LONG)ceil(pt.x);
			if (pt.y > m_rcBounds.bottom) m_rcBounds.bottom = (LONG)ceil(pt.y);
			if (pt.x < m_rcBounds.left)   m_rcBounds.left   = (LONG)floor(pt.x);
			if (pt.y < m_rcBounds.top)    m_rcBounds.top    = (LONG)floor(pt.y);
		}
	}
};

CXTPMarkupPathData::~CXTPMarkupPathData()
{
	if (m_pPoints)
	{
		delete[] m_pPoints;
	}
	if (m_pTypes)
	{
		delete[] m_pTypes;
	}
}

CXTPMarkupPathData* CXTPMarkupPathData::Stretch(CSize sz) const
{
	CXTPMarkupPathData *pData = new CXTPMarkupPathData();

	pData->m_pPoints   = new MARKUP_POINTF[m_nCount];
	pData->m_pTypes    = new BYTE[m_nCount];
	pData->m_nCount    = m_nCount;
	pData->m_nFillRule = m_nFillRule;

	CRect rc = GetBounds();
	if (rc.Width() == 0 || rc.Height() == 0)
		return pData;

	for (int i = 0; i < m_nCount; i++)
	{
		MARKUP_POINTF pt = m_pPoints[i];
		pt.x = (pt.x - rc.left) * sz.cx / rc.Width();
		pt.y = (pt.y - rc.top) * sz.cy / rc.Height();
		pData->m_pPoints[i] = pt;
	}

	MEMCPY_S(pData->m_pTypes, m_pTypes, sizeof(BYTE) * m_nCount);
	pData->UpdateBounds();

	return pData;
}

void CXTPMarkupPathData::SetContentObject(CXTPMarkupBuilder *pBuilder, CXTPMarkupObject *pContent)
{
	UNREFERENCED_PARAMETER(pBuilder);

	if (pContent->IsKindOf(MARKUP_TYPE(CXTPMarkupPathGeometry)))
	{
		SetValue(m_pPropertyGeometry, pContent);
	}
	else
	{
		CXTPMarkupObject::SetContentObject(pBuilder, pContent);
	}
}

CXTPMarkupObject* CXTPMarkupPathData::ConvertFrom(CXTPMarkupBuilder* pBuilder, CXTPMarkupObject* pObject) const
{
	if (IsStringObject(pObject))
	{
		CXTPMarkupString* pString = (CXTPMarkupString*)pObject;

		CXTPMarkupPathGeometryBuilder builder;

		builder.Parse(pBuilder, (LPCWSTR)*pString, pString->GetLength());

		return builder.CreateData();
	}

	return NULL;
}

