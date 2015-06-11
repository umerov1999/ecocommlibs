// XTPChartTitle.cpp
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

#include "stdafx.h"

#include "Common/XTPPropExchange.h"

#include "Types/XTPChartTypes.h"

#include "XTPChartDefines.h"
#include "XTPChartElement.h"
#include <Chart/XTPChartLegendItem.h>
#include "XTPChartTitle.h"
#include "XTPChartElementView.h"
#include "XTPChartTitleView.h"

#include "Utils/XTPChartTextPainter.h"
#include "Drawing/XTPChartDeviceCommand.h"
#include "Appearance/XTPChartAppearance.h"

//////////////////////////////////////////////////////////////////////////
// CXTPChartTitle

IMPLEMENT_DYNAMIC(CXTPChartTitle, CXTPChartTextElement)

CXTPChartTitle::CXTPChartTitle()
{
	m_clrTextColor = CXTPChartColor::Empty;
	m_bVisible = TRUE;

	m_bAntialiasing = TRUE;

	m_pFont = CXTPChartFont::GetTahoma18();

	m_nDocking = xtpChartDockTop;
	m_nAlignment = xtpChartAlignCenter;

	m_nIndent = 0;
	m_nInnerIndent = 5;

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPChartTitle::~CXTPChartTitle()
{
	SAFE_RELEASE(m_pFont);
}

void CXTPChartTitle::SetText(LPCTSTR lpszTitle)
{
	m_strText = lpszTitle;
	OnChartChanged();
}

void CXTPChartTitle::SetFont(CXTPChartFont* pFont)
{
	SAFE_RELEASE(m_pFont);
	m_pFont = pFont;
	OnChartChanged();
}

void CXTPChartTitle::SetTextColor(const CXTPChartColor& clrTextColor)
{
	m_clrTextColor = clrTextColor;
	OnChartChanged();
}

void CXTPChartTitle::SetDocking(XTPChartDocking nDocking)
{
	m_nDocking = nDocking;
	OnChartChanged();
}

void CXTPChartTitle::SetIndent(int nIndent)
{
	m_nIndent = nIndent;
	OnChartChanged();
}

void CXTPChartTitle::SetAlignment(XTPChartStringAlignment nAlignment)
{
	m_nAlignment = nAlignment;
	OnChartChanged();
}

int CXTPChartTitle::GetTextAngle() const
{
	int nAngle = 0;

	switch(m_nDocking)
	{
	case xtpChartDockTop:
	case xtpChartDockBottom:
		nAngle = 0;
		break;
	case xtpChartDockRight:
		nAngle = 90;
		break;
	case xtpChartDockLeft:
		nAngle = 270;
		break;
	}

	return nAngle;
}

CXTPChartColor CXTPChartTitle::GetTextColor() const
{
	return m_clrTextColor;
}

CXTPChartColor CXTPChartTitle::GetActualTextColor() const
{
	if (!m_clrTextColor.IsEmpty())
		return m_clrTextColor;

	return GetAppearance()->GetContentAppearance()->GetTitleAppearance()->TextColor;
}


CPoint CXTPChartTitle::CalcOrigin(CRect bounds, CRect startBounds, CRect rect)
{
	if (m_nDocking == xtpChartDockLeft)
	{
		int x = bounds.left + m_nInnerIndent;

		if (m_nAlignment == xtpChartAlignCenter)
			return CPoint(x, startBounds.top + startBounds.Height() / 2);

		if (m_nAlignment == xtpChartAlignNear)
			return CPoint(x, bounds.bottom - rect.Height() / 2 - m_nInnerIndent);

		return CPoint(x, bounds.top + rect.Height() / 2 + m_nInnerIndent);

	}
	else if (m_nDocking == xtpChartDockRight)
	{
		int x = bounds.right - m_nInnerIndent;

		if (m_nAlignment == xtpChartAlignCenter)
			return CPoint(x, startBounds.top + startBounds.Height() / 2);

		if (m_nAlignment == xtpChartAlignNear)
			return CPoint(x, bounds.top + rect.Height() / 2 + m_nInnerIndent);

		return CPoint(x, bounds.bottom - rect.Height() / 2 - m_nInnerIndent);

	}
	else
	{
		int y = m_nDocking == xtpChartDockTop ? bounds.top + m_nInnerIndent : bounds.bottom - m_nInnerIndent;

		if (m_nAlignment == xtpChartAlignCenter)
			return CPoint(startBounds.left + startBounds.Width() / 2, y);

		if (m_nAlignment == xtpChartAlignNear)
			return CPoint(bounds.left + rect.Width() / 2 + m_nInnerIndent, y);

		return CPoint(bounds.right - rect.Width() / 2 - m_nInnerIndent, y);
	}
}

XTPChartNearTextPosition CXTPChartTitle::GetNearTextPosition()
{
	if (m_nDocking == xtpChartDockLeft) return xtpChartTextNearRight;
	else if (m_nDocking == xtpChartDockRight) return xtpChartTextNearLeft;
	else if (m_nDocking == xtpChartDockTop) return xtpChartTextNearBottom;
	return xtpChartTextNearTop;

}



CXTPChartElementView* CXTPChartTitle::CreateView(CXTPChartDeviceContext* /*pDC*/, CXTPChartElementView* pParentView)
{
	if (!m_bVisible)
		return NULL;

	CXTPChartString strText = m_strText;

	CXTPChartTitleView* pView = new CXTPChartTitleView(this, strText, pParentView);

	return pView;
}

void CXTPChartTitle::DoPropExchange(CXTPPropExchange* pPX)
{
	PX_Bool(pPX, _T("Visible"), m_bVisible, FALSE);
	PX_Enum(pPX, _T("Docking"), m_nDocking, xtpChartDockTop);
	PX_Enum(pPX, _T("Alignment"), m_nAlignment, xtpChartAlignCenter);
	PX_Bool(pPX, _T("Antialiasing"), m_bAntialiasing, TRUE);
	PX_Int(pPX, _T("Indent"), m_nIndent, 0);
	PX_Color(pPX, _T("TextColor"), m_clrTextColor);
	PX_String(pPX, _T("Text"), m_strText);

	PX_Font(pPX, _T("Font"), m_pFont);
}


//////////////////////////////////////////////////////////////////////////
// CXTPChartTitleCollection

CXTPChartTitleCollection::CXTPChartTitleCollection(CXTPChartElement* pOwner)
{
	m_pOwner = pOwner;

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPChartTitleCollection::~CXTPChartTitleCollection()
{
}



CXTPChartTitle* CXTPChartTitleCollection::Add(CXTPChartTitle* pTitle)
{
	InsertAt(GetCount(), pTitle);

	return pTitle;
}

void CXTPChartTitleCollection::DoPropExchange(CXTPPropExchange* pPX)
{
	if (pPX->IsLoading())
	{
		RemoveAll();

		CXTPPropExchangeEnumeratorPtr pEnumerator(pPX->GetEnumerator(_T("Title")));
		POSITION pos = pEnumerator->GetPosition(0);

		while (pos)
		{
			CXTPPropExchangeSection pxItem(pEnumerator->GetNext(pos));


			CXTPChartTitle* pTitle = new CXTPChartTitle();
			Add(pTitle);

			pTitle->DoPropExchange(&pxItem);
		}
	}
	else
	{
		CXTPPropExchangeEnumeratorPtr pEnumerator(pPX->GetEnumerator(_T("Title")));
		POSITION pos = pEnumerator->GetPosition((int)m_arrElements.GetSize());

		for (int i = 0; i < GetCount(); i++)
		{
			CXTPPropExchangeSection pxItem(pEnumerator->GetNext(pos));

			m_arrElements[i]->DoPropExchange(&pxItem);
		}

	}
}

void CXTPChartTitleCollection::CreateView(CXTPChartDeviceContext* pDC, CXTPChartElementView* pParentView)
{
	for (int i = 0; i < GetCount(); i++)
	{
		CXTPChartElementView* pTitleView = GetAt(i)->CreateView(pDC, pParentView);
		UNREFERENCED_PARAMETER(pTitleView);
	}
}

void CXTPChartTitleCollection::CalculateView(CXTPChartDeviceContext* pDC, CRect& rcChart, CXTPChartElementView* pParentView)
{
	CRect rc(rcChart);

	for (int i = 0; i < pParentView->GetCount(); i++)
	{
		CXTPChartTitleView* pTitleView = (CXTPChartTitleView*)pParentView->GetAt(i);
		pTitleView->CalculateView(pDC, rcChart, rc);
	}
}


#ifdef _XTP_ACTIVEX



BEGIN_DISPATCH_MAP(CXTPChartTitle, CXTPChartElement)
	DISP_PROPERTY_EX_ID(CXTPChartTitle, "Text", DISPID_VALUE, OleGetText, OleSetText, VT_BSTR)
	DISP_PROPERTY_EX_ID(CXTPChartTitle, "Docking", 1, GetDocking, SetDocking, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPChartTitle, "Indent", 2, GetIndent, SetIndent, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPChartTitle, "Alignment", 3, GetAlignment, SetAlignment, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPChartTitle, "Antialiasing", 4, GetAntialiasing, SetAntialiasing, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPChartTitle, "Font", DISPID_FONT, OleGetFont, SetNotSupported, VT_DISPATCH)

	DISP_PROPERTY_EX_ID(CXTPChartTitle, "Visible", 5, IsVisible, SetVisible, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPChartTitle, "TextColor", 6, OleGetTextColor, OleSetTextColor, VT_COLOR)
END_DISPATCH_MAP()

BSTR CXTPChartTitle::OleGetText()
{
	return m_strText.AllocSysString();
}

void CXTPChartTitle::OleSetText(LPCTSTR lpszText)
{
	SetText(lpszText);
}


LPDISPATCH CXTPChartTitle::OleGetFont()
{
	return XTPGetDispatch(m_pFont);
}

OLE_COLOR CXTPChartTitle::OleGetTextColor()
{
	return m_clrTextColor.ToOleColor();
}

void CXTPChartTitle::OleSetTextColor(OLE_COLOR clr)
{
	SetTextColor(CXTPChartColor::FromOleColor(clr));
}



// {D27BCC77-27BF-4cb1-9ABF-4558D9835223}
static const GUID IID_IChartTitle =
{ 0xd27bcc77, 0x27bf, 0x4cb1, { 0x9a, 0xbf, 0x45, 0x58, 0xd9, 0x83, 0x52, 0x23 } };

BEGIN_INTERFACE_MAP(CXTPChartTitle, CXTPChartElement)
INTERFACE_PART(CXTPChartTitle, IID_IChartTitle, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartTitle, IID_IChartTitle)

//////////////////////////////////////////////////////////////////////////
//

BEGIN_DISPATCH_MAP(CXTPChartTitleCollection, CXTPChartElement)
	DISP_FUNCTION_ID(CXTPChartTitleCollection, "Count", 1, OleGetItemCount, VT_I4, VTS_NONE)
	DISP_FUNCTION_ID(CXTPChartTitleCollection, "Item", DISPID_VALUE, OleGetItem, VT_DISPATCH, VTS_I4)
	DISP_FUNCTION_ID(CXTPChartTitleCollection, "_NewEnum", DISPID_NEWENUM, OleNewEnum, VT_UNKNOWN, VTS_NONE)

	DISP_FUNCTION_ID(CXTPChartTitleCollection, "Add", 2, OleAdd, VT_DISPATCH, VTS_BSTR)
	DISP_FUNCTION_ID(CXTPChartTitleCollection, "DeleteAll", 5, RemoveAll, VT_EMPTY, VTS_NONE)
END_DISPATCH_MAP()

// {D17BCC77-27BF-4cb1-9ABF-4558D9835223}
static const GUID IID_IChartTitleCollection =
{ 0xd17bcc77, 0x27bf, 0x4cb1, { 0x9a, 0xbf, 0x45, 0x58, 0xd9, 0x83, 0x52, 0x23 } };

BEGIN_INTERFACE_MAP(CXTPChartTitleCollection, CXTPChartElement)
INTERFACE_PART(CXTPChartTitleCollection, IID_IChartTitleCollection, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPChartTitleCollection, IID_IChartTitleCollection)


long CXTPChartTitleCollection::OleGetItemCount()
{
	return (long)GetCount();
}

#pragma warning(push)
#pragma warning(disable : 4702)

LPDISPATCH CXTPChartTitleCollection::OleGetItem(long nIndex)
{
	if (nIndex >= 0 && nIndex < GetCount())
	{
		CXTPChartTitle* pItem = GetAt(nIndex);
		return pItem->GetIDispatch(TRUE);
	}
	AfxThrowOleException(E_INVALIDARG);
	return 0;
}

#pragma warning(pop)

LPDISPATCH CXTPChartTitleCollection::OleAdd(LPCTSTR lpszTitle)
{
	CXTPChartTitle* pTitle = Add(new CXTPChartTitle());

	pTitle->SetText(lpszTitle);

	return XTPGetDispatch(pTitle);
}

IMPLEMENT_ENUM_VARIANT(CXTPChartTitleCollection)


#endif
