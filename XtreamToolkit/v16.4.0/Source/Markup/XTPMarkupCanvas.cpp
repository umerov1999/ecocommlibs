// XTPMarkupCanvas.cpp: implementation of the CXTPMarkupCanvas class.
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

#include "XTPMarkupObject.h"
#include "XTPMarkupInputElement.h"
#include "XTPMarkupUIElement.h"
#include "XTPMarkupFrameworkElement.h"
#include "XTPMarkupPanel.h"
#include "XTPMarkupCanvas.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// CXTPMarkupCanvas

CXTPMarkupDependencyProperty* CXTPMarkupCanvas::m_pTopProperty = NULL;
CXTPMarkupDependencyProperty* CXTPMarkupCanvas::m_pRightProperty = NULL;
CXTPMarkupDependencyProperty* CXTPMarkupCanvas::m_pLeftProperty = NULL;
CXTPMarkupDependencyProperty* CXTPMarkupCanvas::m_pBottomProperty = NULL;

IMPLEMENT_MARKUPCLASS(L"Canvas", CXTPMarkupCanvas, CXTPMarkupPanel)

void CXTPMarkupCanvas::RegisterMarkupClass()
{
	m_pTopProperty = CXTPMarkupDependencyProperty::RegisterAttached(L"Top", MARKUP_TYPE(CXTPMarkupInt), MARKUP_TYPE(CXTPMarkupCanvas));
	m_pRightProperty =  CXTPMarkupDependencyProperty::RegisterAttached(L"Right", MARKUP_TYPE(CXTPMarkupInt), MARKUP_TYPE(CXTPMarkupCanvas));
	m_pLeftProperty = CXTPMarkupDependencyProperty::RegisterAttached(L"Left", MARKUP_TYPE(CXTPMarkupInt), MARKUP_TYPE(CXTPMarkupCanvas));
	m_pBottomProperty = CXTPMarkupDependencyProperty::RegisterAttached(L"Bottom", MARKUP_TYPE(CXTPMarkupInt), MARKUP_TYPE(CXTPMarkupCanvas));
}

CXTPMarkupCanvas::CXTPMarkupCanvas()
{

}

CXTPMarkupCanvas::~CXTPMarkupCanvas()
{

}

void CXTPMarkupCanvas::SetLeft(CXTPMarkupUIElement* pElement, int nLeft)
{
	pElement->SetValue(m_pLeftProperty, new CXTPMarkupInt(nLeft));
}

void CXTPMarkupCanvas::SetTop(CXTPMarkupUIElement* pElement, int nTop)
{
	pElement->SetValue(m_pTopProperty, new CXTPMarkupInt(nTop));
}

void CXTPMarkupCanvas::SetRight(CXTPMarkupUIElement* pElement, int nRight)
{
	pElement->SetValue(m_pRightProperty, new CXTPMarkupInt(nRight));
}

void CXTPMarkupCanvas::SetBottom(CXTPMarkupUIElement* pElement, int nBottom)
{
	pElement->SetValue(m_pBottomProperty, new CXTPMarkupInt(nBottom));
}

int CXTPMarkupCanvas::GetLeft(CXTPMarkupUIElement* pElement)
{
	CXTPMarkupInt* pValue = MARKUP_STATICCAST(CXTPMarkupInt, pElement->GetValue(m_pLeftProperty));
	return pValue ? (int)*pValue : 0;
}

int CXTPMarkupCanvas::GetTop(CXTPMarkupUIElement* pElement)
{
	CXTPMarkupInt* pValue = MARKUP_STATICCAST(CXTPMarkupInt, pElement->GetValue(m_pTopProperty));
	return pValue ? (int)*pValue : 0;
}

int CXTPMarkupCanvas::GetRight(CXTPMarkupUIElement* pElement)
{
	CXTPMarkupInt* pValue = MARKUP_STATICCAST(CXTPMarkupInt, pElement->GetValue(m_pRightProperty));
	return pValue ? (int)*pValue : 0;
}

int CXTPMarkupCanvas::GetBottom(CXTPMarkupUIElement* pElement)
{
	CXTPMarkupInt* pValue = MARKUP_STATICCAST(CXTPMarkupInt, pElement->GetValue(m_pBottomProperty));
	return pValue ? (int)*pValue : 0;
}

CSize CXTPMarkupCanvas::MeasureOverride(CXTPMarkupDrawingContext* pDC, CSize /*availableSize*/)
{
	int cx = 0, cy = 0;

	int nCount = m_pChildren->GetCount();
	for (int i = 0; i < nCount; i++)
	{
		CXTPMarkupUIElement* pElement = m_pChildren->GetItem(i);
		if (pElement == NULL)
			continue;

		pElement->Measure(pDC, CSize(INT_MAX, INT_MAX));
		CSize sz = pElement->GetDesiredSize();

		cx = max(cx, sz.cx);
		cy = max(cy, sz.cy);
	}

	return CSize(cx, cy);
}

CSize CXTPMarkupCanvas::ArrangeOverride(CSize arrangeSize)
{
	int nCount = m_pChildren->GetCount();
	for (int i = 0; i < nCount; i++)
	{
		CXTPMarkupUIElement* pElement = m_pChildren->GetItem(i);
		if (pElement == NULL)
			continue;

		CPoint pt(0, 0);

		CXTPMarkupInt* pLeft = MARKUP_STATICCAST(CXTPMarkupInt, pElement->GetValue(m_pLeftProperty));
		if (pLeft)
		{
			pt.x = *pLeft;
		}
		else
		{
			CXTPMarkupInt* pRight = MARKUP_STATICCAST(CXTPMarkupInt, pElement->GetValue(m_pRightProperty));
			if (pRight)
			{
				pt.x = arrangeSize.cx - pElement->GetDesiredSize().cx - *pRight;
			}
		}

		CXTPMarkupInt* pTop = MARKUP_STATICCAST(CXTPMarkupInt, pElement->GetValue(m_pTopProperty));
		if (pTop)
		{
			pt.y = *pTop;
		}
		else
		{
			CXTPMarkupInt* pBotton = MARKUP_STATICCAST(CXTPMarkupInt, pElement->GetValue(m_pBottomProperty));
			if (pBotton)
			{
				pt.y = arrangeSize.cy - pElement->GetDesiredSize().cy - *pBotton;
			}
		}

		pElement->Arrange(CRect(pt, pElement->GetDesiredSize()));
	}

	return arrangeSize;
}



#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPMarkupCanvas, CXTPMarkupPanel)
	DISP_FUNCTION_ID(CXTPMarkupCanvas, "SetLeft", 600, OleSetLeft, VT_EMPTY, VTS_DISPATCH VTS_I4)
	DISP_FUNCTION_ID(CXTPMarkupCanvas, "SetTop", 601, OleSetTop, VT_EMPTY, VTS_DISPATCH VTS_I4)
	DISP_FUNCTION_ID(CXTPMarkupCanvas, "SetRight", 602, OleSetRight, VT_EMPTY, VTS_DISPATCH VTS_I4)
	DISP_FUNCTION_ID(CXTPMarkupCanvas, "SetBottom", 603, OleSetBottom, VT_EMPTY, VTS_DISPATCH VTS_I4)
	DISP_FUNCTION_ID(CXTPMarkupCanvas, "GetLeft", 604, OleGetLeft, VT_I4, VTS_DISPATCH)
	DISP_FUNCTION_ID(CXTPMarkupCanvas, "GetTop", 605, OleGetTop, VT_I4, VTS_DISPATCH)
	DISP_FUNCTION_ID(CXTPMarkupCanvas, "GetRight", 606, OleGetRight, VT_I4, VTS_DISPATCH)
	DISP_FUNCTION_ID(CXTPMarkupCanvas, "GetBottom", 607, OleGetBottom, VT_I4, VTS_DISPATCH)
END_DISPATCH_MAP()

// {10039842-AE6D-46a8-BC77-CEE7D5CE9ED7}
static const GUID IID_IMarkupCanvas =
{ 0x10039842, 0xae6d, 0x46a8, { 0xbc, 0x77, 0xce, 0xe7, 0xd5, 0xce, 0x9e, 0xd7 } };


BEGIN_INTERFACE_MAP(CXTPMarkupCanvas, CXTPMarkupPanel)
	INTERFACE_PART(CXTPMarkupCanvas, IID_IMarkupCanvas, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPMarkupCanvas, IID_IMarkupCanvas)

void CXTPMarkupCanvas::OleSetLeft(LPDISPATCH lpElementDisp, int nValue)
{
	SetLeft(MARKUP_STATICCAST(CXTPMarkupUIElement, FromDispatch(lpElementDisp, FALSE)), nValue);
}

void CXTPMarkupCanvas::OleSetTop(LPDISPATCH lpElementDisp, int nValue)
{
	SetTop(MARKUP_STATICCAST(CXTPMarkupUIElement, FromDispatch(lpElementDisp, FALSE)), nValue);
}

void CXTPMarkupCanvas::OleSetRight(LPDISPATCH lpElementDisp, int nValue)
{
	SetRight(MARKUP_STATICCAST(CXTPMarkupUIElement, FromDispatch(lpElementDisp, FALSE)), nValue);
}

void CXTPMarkupCanvas::OleSetBottom(LPDISPATCH lpElementDisp, int nValue)
{
	SetBottom(MARKUP_STATICCAST(CXTPMarkupUIElement, FromDispatch(lpElementDisp, FALSE)), nValue);
}

int CXTPMarkupCanvas::OleGetLeft(LPDISPATCH lpElementDisp)
{
	return GetLeft(MARKUP_STATICCAST(CXTPMarkupUIElement, FromDispatch(lpElementDisp, FALSE)));
}

int CXTPMarkupCanvas::OleGetRight(LPDISPATCH lpElementDisp)
{
	return GetRight(MARKUP_STATICCAST(CXTPMarkupUIElement, FromDispatch(lpElementDisp, FALSE)));
}

int CXTPMarkupCanvas::OleGetTop(LPDISPATCH lpElementDisp)
{
	return GetTop(MARKUP_STATICCAST(CXTPMarkupUIElement, FromDispatch(lpElementDisp, FALSE)));
}

int CXTPMarkupCanvas::OleGetBottom(LPDISPATCH lpElementDisp)
{
	return GetBottom(MARKUP_STATICCAST(CXTPMarkupUIElement, FromDispatch(lpElementDisp, FALSE)));
}



#endif
