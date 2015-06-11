// XTPMarkupStatic.cpp : implementation file
//
// This file is a part of the XTREME CONTROLS MFC class library.
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

#ifdef _XTP_INCLUDE_MARKUP

#include "Common/XTPDrawHelpers.h"
#include "Common/XTPColorManager.h"

#include "Markup/XTPMarkupObject.h"
#include "Markup/XTPMarkupBuilder.h"
#include "Markup/XTPMarkupInputElement.h"
#include "Markup/XTPMarkupUIElement.h"
#include "Markup/XTPMarkupDrawingContext.h"
#include "Markup/XTPMarkupContext.h"

#include "XTPMarkupStatic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTPMarkupStatic

CXTPMarkupStatic::CXTPMarkupStatic()
{
	m_pUIElement = NULL;
	m_pMarkupContext = new CXTPMarkupContext();
}

CXTPMarkupStatic::~CXTPMarkupStatic()
{
	MARKUP_RELEASE(m_pUIElement);
	MARKUP_RELEASE(m_pMarkupContext);
}


BEGIN_MESSAGE_MAP(CXTPMarkupStatic, CStatic)
	//{{AFX_MSG_MAP(CXTPMarkupStatic)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTPMarkupStatic message handlers

void CXTPMarkupStatic::SetMarkupTextEx(LPCTSTR lpszMarkup)
{
	MARKUP_RELEASE(m_pUIElement);

	if (lpszMarkup && _tcschr(lpszMarkup, _T('%')) != NULL)
	{
		CString strCaption;
		GetWindowText(strCaption);

		CString strMarkup;
		strMarkup.Format(lpszMarkup, (LPCTSTR)strCaption);

		m_pUIElement = m_pMarkupContext->Parse(strMarkup);
	}
	else
	{
		m_pUIElement = m_pMarkupContext->Parse(lpszMarkup);
	}

	if (m_hWnd) Invalidate(FALSE);
}

void CXTPMarkupStatic::SetMarkupText(LPCTSTR lpszMarkup)
{
	MARKUP_RELEASE(m_pUIElement);

	m_pUIElement = m_pMarkupContext->Parse(lpszMarkup);

	if (m_hWnd) Invalidate(FALSE);
}

void CXTPMarkupStatic::OnPaint()
{
	CPaintDC dcPaint(this);
	CXTPBufferDC dcBuffer(dcPaint);

	CXTPClientRect rc(this);

	HBRUSH hBrush = (HBRUSH)GetParent()->SendMessage(WM_CTLCOLORSTATIC, (WPARAM)dcBuffer.GetSafeHdc(), (LPARAM)m_hWnd);
	if (hBrush)
	{
		::FillRect(dcBuffer.GetSafeHdc(), rc, hBrush);
	}
	else
	{
		dcBuffer.FillSolidRect(rc, GetXtremeColor(COLOR_3DFACE));
	}


	if (m_pUIElement)
	{
		CXTPMarkupDrawingContext dc(dcBuffer);

		m_pUIElement->Measure(&dc, rc.Size());

		m_pUIElement->Arrange(rc);

		m_pUIElement->Render(&dc);
	}
}

BOOL CXTPMarkupStatic::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// To Handle Hyperlinks:
	if (m_pUIElement)
	{
		m_pMarkupContext->m_hContextWnd = m_hWnd;
		if (m_pMarkupContext->OnWndMsg(m_pUIElement, message, wParam, lParam, pResult))
			return TRUE;
	}

	return CStatic::OnWndMsg(message, wParam, lParam, pResult);
}

#endif
