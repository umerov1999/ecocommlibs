// XTPPropertyGridItemSize.cpp : implementation of the CXTPPropertyGridItemSize class.
//
// This file is a part of the XTREME PROPERTYGRID MFC class library.
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
#include "Common/XTPSystemHelpers.h"

#include "XTPPropertyGridItem.h"
#include "XTPPropertyGridItemSize.h"
#include "XTPPropertyGridItemNumber.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CXTPPropertyGridItemSize::CXTPPropertyGridItemSizeWidth

class CXTPPropertyGridItemSize::CXTPPropertyGridItemSizeWidth : public CXTPPropertyGridItemNumber
{
public:
	CXTPPropertyGridItemSizeWidth(LPCTSTR strCaption)
		: CXTPPropertyGridItemNumber(strCaption)
	{
	}

	virtual void OnValueChanged(CString strValue)
	{
		((CXTPPropertyGridItemSize*)m_pParent)->SetWidth(strValue);
	}
	virtual BOOL GetReadOnly() const
	{
		return m_pParent->GetReadOnly();
	}
};

/////////////////////////////////////////////////////////////////////////////
// CXTPPropertyGridItemSize::CXTPPropertyGridItemSizeHeight

class CXTPPropertyGridItemSize::CXTPPropertyGridItemSizeHeight : public CXTPPropertyGridItemNumber
{
public:
	CXTPPropertyGridItemSizeHeight(LPCTSTR strCaption)
		: CXTPPropertyGridItemNumber(strCaption)
	{
	}

	virtual void OnValueChanged(CString strValue)
	{
		((CXTPPropertyGridItemSize*)m_pParent)->SetHeight(strValue);
	}
	virtual BOOL GetReadOnly() const
	{
		return m_pParent->GetReadOnly();
	}
};

/////////////////////////////////////////////////////////////////////////////
// CXTPPropertyGridItemSize
IMPLEMENT_DYNAMIC(CXTPPropertyGridItemSize, CXTPPropertyGridItem)

CXTPPropertyGridItemSize::CXTPPropertyGridItemSize(LPCTSTR strCaption, CSize size, CSize* pBindSize)
	: CXTPPropertyGridItem(strCaption)
{
	m_pItemWidth = NULL;
	m_pItemHeight = NULL;
	m_szValue = size;
	BindToSize(pBindSize);
	m_strDefaultValue = m_strValue = SizeToString(size);
}

CXTPPropertyGridItemSize::CXTPPropertyGridItemSize(UINT nID, CSize size, CSize* pBindSize)
	: CXTPPropertyGridItem(nID)
{
	m_pItemWidth = NULL;
	m_pItemHeight = NULL;
	m_szValue = size;
	BindToSize(pBindSize);
	m_strDefaultValue = m_strValue = SizeToString(size);
}

CXTPPropertyGridItemSize::~CXTPPropertyGridItemSize()
{

}

/////////////////////////////////////////////////////////////////////////////
//

void CXTPPropertyGridItemSize::OnAddChildItem()
{
	if (m_pItemHeight && m_pItemWidth)
		return;

	m_pItemWidth = (CXTPPropertyGridItemSizeWidth*)AddChildItem(new CXTPPropertyGridItemSizeWidth(_T("Width")));
	m_pItemHeight = (CXTPPropertyGridItemSizeHeight*)AddChildItem(new CXTPPropertyGridItemSizeHeight(_T("Height")));

	UpdateChilds();

	m_pItemWidth->SetDefaultValue(m_pItemWidth->GetValue());
	m_pItemHeight->SetDefaultValue(m_pItemHeight->GetValue());
}

CString CXTPPropertyGridItemSize::SizeToString(CSize size)
{
	CString str;
	str.Format(_T("%i; %i"), size.cx, size.cy);
	return str;
}

CSize CXTPPropertyGridItemSize::StringToSize(LPCTSTR str)
{
	int nWidth = NextNumber(str);
	int nHeight = NextNumber(str);

	return CSize(nWidth, nHeight);
}

void CXTPPropertyGridItemSize::SetValue(CString strValue)
{
	SetSize(StringToSize(strValue));
}

void CXTPPropertyGridItemSize::SetSize(CSize size)
{
	m_szValue = size;

	if (m_pBindSize)
	{
		*m_pBindSize = m_szValue;
	}

	CXTPPropertyGridItem::SetValue(SizeToString(m_szValue));
	UpdateChilds();
}

void CXTPPropertyGridItemSize::BindToSize(CSize* pBindSize)
{
	m_pBindSize = pBindSize;
	if (m_pBindSize)
	{
		*m_pBindSize = m_szValue;
	}
}

void CXTPPropertyGridItemSize::OnBeforeInsert()
{
	if (m_pBindSize && *m_pBindSize != m_szValue)
	{
		SetSize(*m_pBindSize);
	}
}

void CXTPPropertyGridItemSize::UpdateChilds()
{
	m_pItemWidth->SetNumber(m_szValue.cx);
	m_pItemHeight->SetNumber(m_szValue.cy);
}

void CXTPPropertyGridItemSize::SetWidth(LPCTSTR strWidth)
{
	OnValueChanged(SizeToString(CSize(_ttoi(strWidth), m_szValue.cy)));
}

void CXTPPropertyGridItemSize::SetHeight(LPCTSTR strHeight)
{
	OnValueChanged(SizeToString(CSize(m_szValue.cx, _ttoi(strHeight))));
}


/////////////////////////////////////////////////////////////////////////////
// CXTPPropertyGridItemPoint::CItemX

class CXTPPropertyGridItemPoint::CItemX : public CXTPPropertyGridItemNumber
{
public:
	CItemX(LPCTSTR strCaption)
		: CXTPPropertyGridItemNumber(strCaption)
	{
	}

	virtual void OnValueChanged(CString strValue)
	{
		((CXTPPropertyGridItemPoint*)m_pParent)->SetX(strValue);
	}
	virtual BOOL GetReadOnly() const
	{
		return m_pParent->GetReadOnly();
	}
};

/////////////////////////////////////////////////////////////////////////////
// CXTPPropertyGridItemPoint::CItemY

class CXTPPropertyGridItemPoint::CItemY : public CXTPPropertyGridItemNumber
{
public:
	CItemY(LPCTSTR strCaption)
		: CXTPPropertyGridItemNumber(strCaption)
	{
	}

	virtual void OnValueChanged(CString strValue)
	{
		((CXTPPropertyGridItemPoint*)m_pParent)->SetY(strValue);
	}
	virtual BOOL GetReadOnly() const
	{
		return m_pParent->GetReadOnly();
	}
};

/////////////////////////////////////////////////////////////////////////////
// CXTPPropertyGridItemPoint
IMPLEMENT_DYNAMIC(CXTPPropertyGridItemPoint, CXTPPropertyGridItem)

CXTPPropertyGridItemPoint::CXTPPropertyGridItemPoint(LPCTSTR strCaption, CPoint pt, CPoint* pBindPoint)
	: CXTPPropertyGridItem(strCaption)
{
	m_pItemX = NULL;
	m_pItemY = NULL;
	m_ptValue = pt;
	BindToPoint(pBindPoint);
	m_strDefaultValue = m_strValue = PointToString(pt);
}

CXTPPropertyGridItemPoint::CXTPPropertyGridItemPoint(UINT nID, CPoint pt, CPoint* pBindPoint)
	: CXTPPropertyGridItem(nID)
{
	m_pItemX = NULL;
	m_pItemY = NULL;
	m_ptValue = pt;
	BindToPoint(pBindPoint);
	m_strDefaultValue = m_strValue = PointToString(pt);
}

CXTPPropertyGridItemPoint::~CXTPPropertyGridItemPoint()
{

}

/////////////////////////////////////////////////////////////////////////////
//

void CXTPPropertyGridItemPoint::OnAddChildItem()
{
	if (m_pItemX && m_pItemY)
		return;

	m_pItemX = (CItemX*)AddChildItem(new CItemX(_T("X")));
	m_pItemY = (CItemY*)AddChildItem(new CItemY(_T("Y")));

	UpdateChilds();

	m_pItemX->SetDefaultValue(m_pItemX->GetValue());
	m_pItemY->SetDefaultValue(m_pItemY->GetValue());
}

CString CXTPPropertyGridItemPoint::PointToString(CPoint pt)
{
	CString str;
	str.Format(_T("%i; %i"), pt.x, pt.y);

	return str;
}

CPoint CXTPPropertyGridItemPoint::StringToPoint(LPCTSTR str)
{
	int x = NextNumber(str);
	int y = NextNumber(str);

	return CPoint(x, y);
}

void CXTPPropertyGridItemPoint::SetValue(CString strValue)
{
	SetPoint(StringToPoint(strValue));
}

void CXTPPropertyGridItemPoint::SetPoint(CPoint pt)
{
	m_ptValue = pt;

	if (m_pBindPoint)
	{
		*m_pBindPoint = m_ptValue;
	}

	CXTPPropertyGridItem::SetValue(PointToString(m_ptValue));
	UpdateChilds();
}

void CXTPPropertyGridItemPoint::BindToPoint(CPoint* pBindPoint)
{
	m_pBindPoint = pBindPoint;
	if (m_pBindPoint)
	{
		*m_pBindPoint = m_ptValue;
	}
}

void CXTPPropertyGridItemPoint::OnBeforeInsert()
{
	if (m_pBindPoint && *m_pBindPoint != m_ptValue)
	{
		SetPoint(*m_pBindPoint);
	}
}

void CXTPPropertyGridItemPoint::UpdateChilds()
{
	m_pItemX->SetNumber(m_ptValue.x);
	m_pItemY->SetNumber(m_ptValue.y);
}

void CXTPPropertyGridItemPoint::SetX(LPCTSTR sx)
{
	OnValueChanged(PointToString(CPoint(_ttoi(sx), m_ptValue.y)));
}

void CXTPPropertyGridItemPoint::SetY(LPCTSTR sy)
{
	OnValueChanged(PointToString(CPoint(m_ptValue.x, _ttoi(sy))));
}
