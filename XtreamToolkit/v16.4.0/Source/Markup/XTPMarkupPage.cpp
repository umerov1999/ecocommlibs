// XTPMarkupPage.cpp: implementation of the CXTPMarkupPage class.
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
#include "XTPMarkupPage.h"
#include "XTPMarkupBuilder.h"
#include "XTPMarkupContext.h"
#include "XTPMarkupDrawingContext.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_MARKUPCLASS(L"Page", CXTPMarkupPage, CXTPMarkupFrameworkElement);

void CXTPMarkupPage::RegisterMarkupClass()
{

}

CXTPMarkupPage::CXTPMarkupPage()
{
	m_pContent = NULL;

}

CXTPMarkupPage::~CXTPMarkupPage()
{
	SetContent(NULL);
}


CXTPMarkupUIElement* CXTPMarkupPage::GetContent() const
{
	return m_pContent;
}

void CXTPMarkupPage::SetContent(CXTPMarkupUIElement* pContent)
{
	if (m_pContent)
	{
		m_pContent->SetLogicalParent(NULL);
		MARKUP_RELEASE(m_pContent);
	}

	m_pContent = pContent;

	if (m_pContent)
	{
		m_pContent->SetLogicalParent(this);
	}
}


CSize CXTPMarkupPage::MeasureOverride(CXTPMarkupDrawingContext* pDC, CSize szAvailableSize)
{
	if (m_pContent)
	{
		m_pContent->Measure(pDC, szAvailableSize);
		return m_pContent->GetDesiredSize();
	}

	return CSize(0, 0);
}

CSize CXTPMarkupPage::ArrangeOverride(CSize szFinalSize)
{
	if (m_pContent)
	{
		m_pContent->Arrange(CRect(0, 0, szFinalSize.cx, szFinalSize.cy));
	}

	return szFinalSize;
}

void CXTPMarkupPage::SetContentObject(CXTPMarkupBuilder* pBuilder, CXTPMarkupObject* pContent)
{
	if (!pContent->IsKindOf(MARKUP_TYPE(CXTPMarkupUIElement)))
	{
		pBuilder->ThrowBuilderException(CXTPMarkupBuilder::FormatString(_T("'%ls' object cannot be added to '%ls'. Object cannot be converted to type 'CXTPMarkupUIElement'"),
			(LPCTSTR)pContent->GetType()->m_lpszClassName, (LPCTSTR)GetType()->m_lpszClassName));
	}

	if (m_pContent != NULL)
	{
		pBuilder->ThrowBuilderException(CXTPMarkupBuilder::FormatString(_T("'%ls' already has a child and cannot add ")
			_T("'%ls'. '%ls' can accept only one child."),
			(LPCTSTR)GetType()->m_lpszClassName, (LPCTSTR)pContent->GetType()->m_lpszClassName, (LPCTSTR)GetType()->m_lpszClassName));
	}


	SetContent((CXTPMarkupUIElement*)pContent);
}

BOOL CXTPMarkupPage::HasContentObject() const
{
	return m_pContent != NULL;
}

CXTPMarkupInputElement* CXTPMarkupPage::InputHitTestOverride(CPoint /*point*/) const
{
	return NULL;
}


//////////////////////////////////////////////////////////////////////
// CXTPMarkupWindowContainer

IMPLEMENT_MARKUPCLASS(L"WindowContainer", CXTPMarkupWindowContainer, CXTPMarkupFrameworkElement);

void CXTPMarkupWindowContainer::RegisterMarkupClass()
{

}

CXTPMarkupWindowContainer::CXTPMarkupWindowContainer()
{
	m_hWndChild = NULL;
	m_szChild = CSize(0, 0);
}

CXTPMarkupWindowContainer::~CXTPMarkupWindowContainer()
{
}


HWND CXTPMarkupWindowContainer::GetChild() const
{
	return m_hWndChild;
}

void CXTPMarkupWindowContainer::SetChild(HWND hWnd)
{
	m_hWndChild = hWnd;

	CRect rc;

	::GetWindowRect(m_hWndChild, &rc);
	m_szChild = rc.Size();

	InvalidateMeasure();
	InvalidateVisual();
}


CSize CXTPMarkupWindowContainer::MeasureOverride(CXTPMarkupDrawingContext* /*pDC*/, CSize /*szAvailableSize*/)
{
	return m_szChild;
}

CSize CXTPMarkupWindowContainer::ArrangeOverride(CSize szFinalSize)
{
	if (m_hWndChild)
	{

	}

	return szFinalSize;
}

void CXTPMarkupWindowContainer::RenderClipped(CXTPMarkupDrawingContext* /*pDC*/)
{
	if (m_hWndChild)
	{
		::SetWindowPos(m_hWndChild, 0, 0, 0, 0, 0,
			SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_HIDEWINDOW);
	}

}

void CXTPMarkupWindowContainer::OnRender(CXTPMarkupDrawingContext* pDC)
{
	CRect rc = m_pMarkupContext->GetClientBoundingRect(this);

	if (m_hWndChild)
	{
		if (pDC->m_hDWP)
		{
			pDC->m_hDWP = ::DeferWindowPos(pDC->m_hDWP, m_hWndChild, 0, rc.left, rc.top, rc.Width(), rc.Height(), SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOOWNERZORDER |SWP_SHOWWINDOW);
		}
		else
		{
			::SetWindowPos(m_hWndChild, 0, rc.left, rc.top, rc.Width(), rc.Height(), SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOOWNERZORDER |SWP_SHOWWINDOW);
		}
	}
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPMarkupPage, CXTPMarkupFrameworkElement)
	DISP_PROPERTY_EX_ID(CXTPMarkupPage, "Content", 500, OleGetChild, OleSetChild, VT_DISPATCH)
END_DISPATCH_MAP()

// {10069842-AE6D-46a8-BC77-CEE7D5CE9ED7}
static const GUID IID_IMarkupPage =
{ 0x10069842, 0xae6d, 0x46a8, { 0xbc, 0x77, 0xce, 0xe7, 0xd5, 0xce, 0x9e, 0xd7 } };


BEGIN_INTERFACE_MAP(CXTPMarkupPage, CXTPMarkupFrameworkElement)
	INTERFACE_PART(CXTPMarkupPage, IID_IMarkupPage, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPMarkupPage, IID_IMarkupPage)

LPDISPATCH CXTPMarkupPage::OleGetChild()
{
	return XTPGetDispatch(GetContent());
}

void CXTPMarkupPage::OleSetChild(LPDISPATCH lpDisp)
{
	SetContent(MARKUP_STATICCAST(CXTPMarkupUIElement, FromDispatch(lpDisp)));
}

////////////////////////////////////////////////////////////////////
// CXTPMarkupWindowContainer

BEGIN_DISPATCH_MAP(CXTPMarkupWindowContainer, CXTPMarkupFrameworkElement)
	DISP_PROPERTY_EX_ID(CXTPMarkupWindowContainer, "Child", 500, OleGetChild, OleSetChild, VT_HANDLE)
END_DISPATCH_MAP()

// {15669842-AE6D-46a8-BC77-CEE7D5CE9ED7}
static const GUID IID_IMarkupWindowContainer =
{ 0x15669842, 0xae6d, 0x46a8, { 0xbc, 0x77, 0xce, 0xe7, 0xd5, 0xce, 0x9e, 0xd7 } };


BEGIN_INTERFACE_MAP(CXTPMarkupWindowContainer, CXTPMarkupFrameworkElement)
	INTERFACE_PART(CXTPMarkupWindowContainer, IID_IMarkupWindowContainer, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPMarkupWindowContainer, IID_IMarkupWindowContainer)

HWND CXTPMarkupWindowContainer::OleGetChild()
{
	return m_hWndChild;
}

void CXTPMarkupWindowContainer::OleSetChild(HWND hwndChild)
{
	SetChild(hwndChild);
}

#endif
