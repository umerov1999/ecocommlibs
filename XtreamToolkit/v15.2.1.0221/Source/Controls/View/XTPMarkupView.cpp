// XTPMarkupView.cpp : implementation file
//
// This file is a part of the XTREME CONTROLS MFC class library.
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

#include "stdafx.h"

#ifdef _XTP_INCLUDE_MARKUP

#include "Common/XTPMarkupRender.h"
#include "Common/XTPDrawHelpers.h"

#include "Markup/XTPMarkupObject.h"
#include "Markup/XTPMarkupBuilder.h"
#include "Markup/XTPMarkupInputElement.h"
#include "Markup/XTPMarkupUIElement.h"
#include "Markup/XTPMarkupDrawingContext.h"
#include "Markup/XTPMarkupContext.h"


#include "XTPMarkupView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTPMarkupView

IMPLEMENT_DYNCREATE(CXTPMarkupView, CView)

BEGIN_MESSAGE_MAP(CXTPMarkupView, CView)
	//{{AFX_MSG_MAP(CXTPMarkupView)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTPMarkupView construction/destruction



CXTPMarkupView::CXTPMarkupView()
{
	m_pUIElement = NULL;
	m_bInRender = FALSE;
}

CXTPMarkupView::~CXTPMarkupView()
{
	XTPMarkupReleaseElement(m_pUIElement);
}

BOOL CXTPMarkupView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	cs.style |= WS_CLIPCHILDREN | WS_CLIPSIBLINGS;

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CXTPMarkupView drawing

void CXTPMarkupView::SetMarkupText(LPCTSTR lpszMarkup)
{
	MARKUP_RELEASE(m_pUIElement);

	m_pUIElement = Parse(lpszMarkup);

	if (m_hWnd) Invalidate(FALSE);
}


BOOL CXTPMarkupView::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	if (m_pUIElement)
	{
		if (XTPMarkupRelayMessage(m_pUIElement, message, wParam, lParam, pResult))
			return TRUE;
	}

	return CView::OnWndMsg(message, wParam, lParam, pResult);
}

void CXTPMarkupView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	XTPMarkupReleaseElement(m_pUIElement);

	m_hContextWnd = m_hWnd;
}

void CXTPMarkupView::OnDraw(CDC* /*pDC*/)
{
	ASSERT(FALSE);
}

void CXTPMarkupView::DrawMarkup(CXTPMarkupDrawingContext* pDC, CRect rc)
{
	CXTPMarkupUIElement* pElement = m_pUIElement;

	if (!pElement)
		return;

	if (m_bInRender)
		return;

	m_bInRender = TRUE;

	pElement->Measure(pDC, rc.Size());
	pElement->Arrange(rc);

	pElement->Render(pDC);


	m_bInRender = FALSE;
}


void CXTPMarkupView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
	CXTPMarkupPrintingContext dc(this, pDC->m_hDC, pDC->m_hAttribDC);
	DrawMarkup(&dc, pInfo->m_rectDraw);
}


/////////////////////////////////////////////////////////////////////////////
// CXTPMarkupView printing

BOOL CXTPMarkupView::OnPreparePrinting(CPrintInfo* pInfo)
{
	pInfo->m_nNumPreviewPages = 1;
	pInfo->SetMinPage(1);
	pInfo->SetMaxPage(1);


	// default preparation
	return DoPreparePrinting(pInfo);
}

void CXTPMarkupView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CXTPMarkupView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CXTPMarkupView diagnostics

#ifdef _DEBUG
void CXTPMarkupView::AssertValid() const
{
	CView::AssertValid();
}

void CXTPMarkupView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CXTPMarkupView message handlers

BOOL CXTPMarkupView::OnEraseBkgnd(CDC* /*pDC*/)
{
	return TRUE;
}

void CXTPMarkupView::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CXTPClientRect rc(this);

	CXTPBufferDC dcMem(dc);
	dcMem.FillSolidRect(rc, GetSysColor(COLOR_WINDOW));

	CXTPMarkupDrawingContext dcMarkup(this, dcMem);
	DrawMarkup(&dcMarkup, rc);
}

void CXTPMarkupView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)
{
	ASSERT_VALID(this);
	ASSERT_VALID(pDC);
	ASSERT(pInfo != NULL);  // overriding OnPaint -- never get this.
	if (!pInfo)
		return;

	pDC->SetMapMode(MM_ANISOTROPIC);

	pDC->SetViewportExt(pDC->GetDeviceCaps(LOGPIXELSX), pDC->GetDeviceCaps(LOGPIXELSY));

	pDC->SetWindowExt(96, 96);

	// ptOrg is in logical coordinates
	pDC->OffsetWindowOrg(0, 0);

}

#endif
