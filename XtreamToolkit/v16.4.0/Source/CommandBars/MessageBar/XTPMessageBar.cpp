// XTPMessageBar.cpp : implementation of the CXTPMessageBar class.
//
// This file is a part of the XTREME COMMANDBARS MFC class library.
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

#include <StdAfx.h>

#include <Common/XTPMarkupRender.h>
#include <Common/XTPToolTipContext.h>
#include <Common/XTPColorManager.h>
#include <Common/XTPDrawHelpers.h>

#include <CommandBars/XTPCommandBarsDefines.h>
#include <CommandBars/XTPCommandBars.h>
#include <CommandBars/XTPPaintManager.h>
#include <CommandBars/MessageBar/XTPMessageBar.h>
#include <CommandBars/MessageBar/XTPMessageBarPaintManager.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CXTPMessageBarButton::CXTPMessageBarButton()
{
	m_rcButton.SetRectEmpty();
	m_nID = 0;
	m_bPressed = FALSE;
	m_bHot = FALSE;
	m_pUIElement = NULL;
}

CXTPMessageBarButton::~CXTPMessageBarButton()
{
	XTPMarkupReleaseElement(m_pUIElement);
}

void CXTPMessageBarButton::PerformClick(CXTPMessageBar* pBar, CPoint pt)
{
	if ((::GetCapture() != NULL))
		return;

	m_bPressed = TRUE;
	pBar->InvalidateRect(m_rcButton, FALSE);

	::SetCapture(pBar->GetSafeHwnd());

	BOOL bClick = FALSE, bPressed;

	while (::GetCapture() == pBar->GetSafeHwnd())
	{
		MSG msg;

		if (!::GetMessage(&msg, NULL, 0, 0))
			break;

		switch (msg.message)
		{
			case WM_MOUSEMOVE:
				pt = CPoint((short signed)LOWORD(msg.lParam), (short signed)HIWORD(msg.lParam));

				bPressed = m_rcButton.PtInRect(pt);

				if (bPressed != m_bPressed)
				{
					m_bPressed = bPressed;
					pBar->InvalidateRect(m_rcButton, FALSE);
				}

				break;

			case WM_LBUTTONUP:
				bClick = m_bPressed;
				goto ExitLoop;

			case WM_KEYDOWN:
				if (msg.wParam != VK_ESCAPE)
					break;

			case WM_CANCELMODE:
			case WM_RBUTTONDOWN:
				goto ExitLoop;

			default:
				DispatchMessage (&msg);
				break;
		}
	}

ExitLoop:
	ReleaseCapture();

	m_bPressed = FALSE;
	pBar->InvalidateRect(m_rcButton, FALSE);

	if (bClick)
	{
		pBar->Click(this);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CXTPMessageBar

CXTPMessageBar::CXTPMessageBar()
	: m_nHeight(40)
	, m_bAutoSize(FALSE)
{
	m_pMarkupContext = NULL;
	m_pUIElement = NULL;

	m_rcPadding.SetRect(5, 0, 5, 0);
	m_rcContent.SetRectEmpty();

	m_pCommandBars = NULL;

	m_pHotButton = NULL;

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPMessageBar::~CXTPMessageBar()
{
	RemoveButtons();

	XTPMarkupReleaseElement(m_pUIElement);
	XTPMarkupReleaseContext(m_pMarkupContext);
}

BOOL CXTPMessageBar::Create(CXTPCommandBars* pCommandBars, DWORD dwStyle, UINT nID)
{
	m_dwStyle = (dwStyle & CBRS_ALL);
	m_pCommandBars = pCommandBars;

	if (!CWnd::Create(AfxRegisterWndClass(0, AfxGetApp()->LoadStandardCursor(IDC_ARROW)), NULL, dwStyle, CRect(0, 0, 0, 0), pCommandBars->GetSite(), nID))
		return FALSE;

	if (m_pCommandBars->IsLayoutRTL())
	{
		ModifyStyleEx(0, WS_EX_LAYOUTRTL);
	}

	return TRUE;
}


void CXTPMessageBar::OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler)
{
	// update the dialog controls added to the status bar
	UpdateDialogControls(pTarget, bDisableIfNoHndler);
}


void CXTPMessageBar::SetHeight(int nHeight)
{
	m_nHeight = nHeight;
}

void CXTPMessageBar::SetAutoSize(BOOL bAutoSize)
{
	m_bAutoSize = bAutoSize;
}

CSize CXTPMessageBar::CalcFixedLayout(BOOL bStretch, BOOL bHorz)
{
	UNREFERENCED_PARAMETER(bStretch);
	UNREFERENCED_PARAMETER(bHorz);

	ASSERT_VALID(this);
	ASSERT(::IsWindow(m_hWnd));

	CSize size(32767, m_nHeight);

	if (m_bAutoSize) // Auto expand height
	{
		CRect rcMargin = GetPaintManager()->m_rcMessageBarMargin;

		CClientDC dc(this);
		CSize szMeasure = MeasureContent(&dc);
		size.cy  = szMeasure.cy;
		size.cy += rcMargin.top  + rcMargin.bottom;
		size.cy += m_rcPadding.top + m_rcPadding.bottom;
		size.cy += 4;
	}

	size.cy = max(size.cy, m_nHeight);

	return size;
}


void CXTPMessageBar::EnableMarkup(BOOL bEnableMarkup)
{
	XTPMarkupReleaseContext(m_pMarkupContext);

	if (bEnableMarkup)
	{
		m_pMarkupContext = XTPMarkupCreateContext(m_hWnd);
	}
}

void CXTPMessageBar::AddButton(UINT nID, LPCTSTR lpszCaption, LPCTSTR lpszToolTop)
{
	CXTPMessageBarButton* pButton = new CXTPMessageBarButton();
	pButton->m_nID = nID;
	pButton->m_strCaption = lpszCaption;
	pButton->m_strToolTip = lpszToolTop;
	pButton->m_pUIElement = XTPMarkupParseText(m_pMarkupContext, lpszCaption);

	m_arrButtons.Add(pButton);
	Invalidate(FALSE);
}

void CXTPMessageBar::RemoveButtons()
{
	for (int i = 0; i < m_arrButtons.GetSize(); i++)
	{
		SAFE_DELETE(m_arrButtons[i]);
	}
	m_arrButtons.RemoveAll();
	m_pHotButton = NULL;

	if (m_hWnd) Invalidate(FALSE);
}

void CXTPMessageBar::SetMessage(LPCTSTR lpszMessage)
{
	m_strContent = lpszMessage;

	XTPMarkupReleaseElement(m_pUIElement);

	if (m_pMarkupContext)
	{
		m_pUIElement = XTPMarkupParseText(m_pMarkupContext, lpszMessage);
	}
	Invalidate(FALSE);
}

CXTPPaintManager* CXTPMessageBar::GetPaintManager() const
{
	return m_pCommandBars->GetPaintManager();
}

CXTPMessageBarPaintManager* CXTPMessageBar::GetMessageBarPaintManager() const
{
	return GetPaintManager()->GetMessageBarPaintManager();
}

BEGIN_MESSAGE_MAP(CXTPMessageBar, CControlBar)
	//{{AFX_MSG_MAP(CXTPMessageBar)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_MESSAGE_VOID(WM_MOUSELEAVE, OnMouseLeave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CXTPMessageBar message handlers


void CXTPMessageBar::OnPaint()
{
	CPaintDC dcPaint(this); // device context for painting

	CXTPBufferDC dc(dcPaint);

	CXTPFontDC font(&dc, GetPaintManager()->GetRegularFont());

	RecalcLayout(&dc);

	FillMessageBar(&dc);

	DrawContent(&dc);

	DrawButtons(&dc);
}

void CXTPMessageBar::DrawButtons(CDC* pDC)
{
	for (int i = 0; i < m_arrButtons.GetSize(); i++)
	{
		CXTPMessageBarButton* pButton = m_arrButtons.GetAt(i);
		DrawButton(pDC, pButton);
	}
};

void CXTPMessageBar::DrawButton(CDC* pDC, CXTPMessageBarButton* pButton)
{
	BOOL bCloseButton = (pButton->m_nID == SC_CLOSE);

	GetMessageBarPaintManager()->DrawMessageBarButton(pDC, pButton);

	if (!bCloseButton)
	{
		COLORREF clrText = GetPaintManager()->m_clrMessageBarText;

		if (pButton->m_pUIElement)
		{
			XTPMarkupSetDefaultFont(m_pMarkupContext, (HFONT)GetPaintManager()->GetRegularFont()->GetSafeHandle(), clrText);

			CSize sz = XTPMarkupMeasureElement(pButton->m_pUIElement);

			CPoint pt((pButton->m_rcButton.left + pButton->m_rcButton.right - sz.cx) / 2,
				(pButton->m_rcButton.top + pButton->m_rcButton.bottom - sz.cy) / 2);

			XTPMarkupRenderElement(pButton->m_pUIElement, pDC->GetSafeHdc(), CRect(pt, sz));
		}
		else
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(clrText);
			pDC->DrawText(pButton->m_strCaption, pButton->m_rcButton, DT_CENTER | DT_VCENTER | DT_NOPREFIX | DT_SINGLELINE);
		}
	}
}

void CXTPMessageBar::FillMessageBar(CDC* pDC)
{
	GetMessageBarPaintManager()->FillMessageBar(pDC, this);
}

void CXTPMessageBar::DrawContent(CDC* pDC)
{
	COLORREF clrText = GetPaintManager()->m_clrMessageBarText;

	if (m_pUIElement)
	{
		XTPMarkupSetDefaultFont(m_pMarkupContext, (HFONT)GetPaintManager()->GetRegularFont()->GetSafeHandle(), clrText);
		XTPMarkupRenderElement(m_pUIElement, pDC->GetSafeHdc(), m_rcContent);
	}
	else
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(clrText);
		pDC->DrawText(m_strContent, m_rcContent, DT_SINGLELINE | DT_END_ELLIPSIS | DT_NOPREFIX);
	}
}

CRect CXTPMessageBar::GetMessageRect()
{
	BOOL bCloseButton = FindButton(SC_CLOSE) != NULL;

	CRect rcMargin = GetPaintManager()->m_rcMessageBarMargin;

	CXTPClientRect rc(this);
	rc.left   += rcMargin.left;
	rc.top    += rcMargin.top;
	rc.right  -= rcMargin.right + (bCloseButton ? 18 : 0);
	rc.bottom -= rcMargin.bottom;

	return rc;
}

CSize CXTPMessageBar::MeasureContent(CDC* pDC)
{
	if (m_pUIElement)
	{
		XTPMarkupSetDefaultFont(m_pMarkupContext, (HFONT)GetPaintManager()->GetRegularFont()->GetSafeHandle(), COLORREF_NULL);
		CSize sz = XTPMarkupMeasureElement(m_pUIElement);
		return sz;
	}

	CSize sz = pDC->GetTextExtent(m_strContent);
	return sz;
}

CSize CXTPMessageBar::MeasureButton(CDC* pDC, CXTPMessageBarButton* pButton)
{
	if (pButton->m_nID == SC_CLOSE)
		return CSize(18, 18);

	CSize sz;

	if (pButton->m_pUIElement)
	{
		XTPMarkupSetDefaultFont(m_pMarkupContext, (HFONT)GetPaintManager()->GetRegularFont()->GetSafeHandle(), COLORREF_NULL);
		sz = XTPMarkupMeasureElement(pButton->m_pUIElement);
	}
	else
	{
		sz = pDC->GetTextExtent(pButton->m_strCaption);
	}

	sz.cy = max(sz.cy + 2, 23);
	sz.cx = max(89, sz.cx + 10);

	return sz;
}

void CXTPMessageBar::RecalcLayout(CDC* pDC)
{
	if (!m_hWnd)
		return;

	CXTPClientRect rcClient(this);

	CSize szContent = MeasureContent(pDC);

	CRect rcMargin = GetPaintManager()->m_rcMessageBarMargin;

	m_rcContent = CRect(CPoint(rcMargin.left + m_rcPadding.left, (rcClient.Height() - szContent.cy) /2), szContent);

	int nLeft = m_rcContent.right, i;

	CXTPMessageBarButton* pCloseButton = NULL;

	for (i = 0; i < m_arrButtons.GetSize(); i++)
	{
		CXTPMessageBarButton* pButton = m_arrButtons.GetAt(i);
		if (pButton->m_nID == SC_CLOSE)
		{
			pCloseButton = pButton;
			continue;
		}

		CSize szButton = MeasureButton(pDC, pButton);

		pButton->m_rcButton = CRect(CPoint(nLeft + m_rcPadding.right, (rcClient.Height() - szButton.cy) /2), szButton);
		nLeft = pButton->m_rcButton.right;
	}

	int nRight = rcClient.right - rcMargin.right - m_rcPadding.right - (pCloseButton ? 18 : 0);
	if (nLeft > nRight)
	{
		nLeft = rcMargin.left + m_rcPadding.left;

		for (i = (int)m_arrButtons.GetSize() - 1; i >= 0; i--)
		{
			CXTPMessageBarButton* pButton = m_arrButtons.GetAt(i);
			if (pButton->m_nID == SC_CLOSE)
				continue;

			int nWidth = pButton->m_rcButton.Width();
			pButton->m_rcButton.right = nRight;
			pButton->m_rcButton.left = nRight - nWidth;
			if (pButton->m_rcButton.left < nLeft)
			{
				pButton->m_rcButton.SetRectEmpty();
			}
			else
			{
				nRight = nRight - nWidth - m_rcPadding.right;
			}
		}

		m_rcContent.right = nRight;
	}

	if (pCloseButton)
	{
		pCloseButton->m_rcButton = GetMessageBarPaintManager()->GetCloseButtonRect(
			rcClient.right, rcMargin.top, rcClient.Height());
	}
};

void CXTPMessageBar::OnSize(UINT nType, int cx, int cy)
{
	CControlBar::OnSize(nType, cx, cy);

	Invalidate(FALSE);
}

BOOL CXTPMessageBar::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	if (m_pUIElement)
	{
		if (XTPMarkupRelayMessage(m_pUIElement, message, wParam, lParam, pResult))
			return TRUE;
	}

	m_pCommandBars->GetToolTipContext()->FilterToolTipMessage(this, message, wParam, lParam);

	return CControlBar::OnWndMsg(message, wParam, lParam, pResult);
}

CXTPMessageBarButton* CXTPMessageBar::FindButton(UINT nID) const
{
	for (int i = 0; i < m_arrButtons.GetSize(); i++)
	{
		CXTPMessageBarButton* pButton = m_arrButtons.GetAt(i);
		if (pButton->m_nID == nID)
			return pButton;
	}

	return NULL;

}
CXTPMessageBarButton* CXTPMessageBar::HitTestButton(CPoint point) const
{
	for (int i = 0; i < m_arrButtons.GetSize(); i++)
	{
		CXTPMessageBarButton* pButton = m_arrButtons.GetAt(i);
		if (pButton->m_rcButton.PtInRect(point))
			return pButton;
	}

	return NULL;
}

void CXTPMessageBar::OnMouseLeave()
{
	OnMouseMove(0, CPoint(-1, -1));
}

void CXTPMessageBar::OnMouseMove(UINT nFlags, CPoint point)
{
	CXTPMessageBarButton* pButton = HitTestButton(point);

	if (pButton != m_pHotButton)
	{
		if (m_pHotButton) m_pHotButton->m_bHot = FALSE;

		m_pHotButton = pButton;

		if (m_pHotButton) m_pHotButton->m_bHot = TRUE;

		if (m_pHotButton)
		{
			TRACKMOUSEEVENT tme =
			{
				sizeof(TRACKMOUSEEVENT), TME_LEAVE, m_hWnd
			};
			_TrackMouseEvent(&tme);
		}

		Invalidate(FALSE);
	}

	CControlBar::OnMouseMove(nFlags, point);
}

void CXTPMessageBar::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_pHotButton)
	{
		m_pHotButton->PerformClick(this, point);
		return;
	}

	CControlBar::OnLButtonDown(nFlags, point);
}

INT_PTR CXTPMessageBar::OnToolHitTest(CPoint point, TOOLINFO* pTI) const
{
	ASSERT_VALID(this);
	ASSERT(::IsWindow(m_hWnd));

	// check child windows first by calling CControlBar
	INT_PTR nHit = CControlBar::OnToolHitTest(point, pTI);
	if (nHit != -1)
		return nHit;

	if (m_pHotButton)
	{
		nHit = m_pHotButton->m_nID;

		CString strTip = m_pHotButton->m_strToolTip;
		if (strTip.IsEmpty())
			return -1;

		CXTPToolTipContext::FillInToolInfo(pTI, m_hWnd, m_pHotButton->m_rcButton, nHit, strTip, strTip, _T(""), m_pCommandBars->GetImageManager());

		return nHit;
	}
	return -1;
}

void CXTPMessageBar::Click(CXTPMessageBarButton* pButton)
{
	if (pButton->m_nID == SC_CLOSE)
	{
		SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_HIDEWINDOW);
		m_pCommandBars->RecalcFrameLayout();

		m_pCommandBars->GetSite()->SendMessage(WM_XTP_MESSAGEBARCLOSED, 0, reinterpret_cast<LPARAM>(this));
	}
	else
	{
#ifndef _XTP_ACTIVEX
		m_pCommandBars->GetSite()->SendMessage(WM_COMMAND, pButton->m_nID);
#else
		m_pCommandBars->GetSite()->SendMessage(WM_XTP_DELAYEXECUTE, pButton->m_nID);
#endif
	}
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPMessageBar, CCmdTarget)
	DISP_PROPERTY_EX_ID(CXTPMessageBar, "Visible", 1, OleGetVisible, OleSetVisible, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPMessageBar, "Message", 2, OleGetMessage, OleSetMessage, VT_BSTR)
	DISP_FUNCTION_ID(CXTPMessageBar, "AddButton", 3, OleAddButton, VT_EMPTY, VTS_I4 VTS_BSTR VTS_BSTR)
	DISP_FUNCTION_ID(CXTPMessageBar, "RemoveButtons", 4, RemoveButtons, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CXTPMessageBar, "AddCloseButton", 5, OleAddCloseButton, VT_EMPTY, VTS_BSTR)
	DISP_PROPERTY_EX_ID(CXTPMessageBar, "Height", 6, OleGetHeight, OleSetHeight, VT_I4)
END_DISPATCH_MAP()

// {46445B78-7A0A-447d-9CDC-8F63E7E5AF55}
static const GUID IID_IMessageBar =
{ 0x46445b78, 0x7a0a, 0x447d, { 0x9c, 0xdc, 0x8f, 0x63, 0xe7, 0xe5, 0xaf, 0x55 } };


BEGIN_INTERFACE_MAP(CXTPMessageBar, CCmdTarget)
	INTERFACE_PART(CXTPMessageBar, IID_IMessageBar, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPMessageBar, IID_IMessageBar)

void CXTPMessageBar::OnFinalRelease()
{
	if (m_hWnd != NULL)
		DestroyWindow();

	CCmdTarget::OnFinalRelease();
}

int CXTPMessageBar::OleGetHeight()
{
	return m_nHeight;
}

void CXTPMessageBar::OleSetHeight(int nHeight)
{
	SetHeight(nHeight);

	CFrameWnd* pFrame = GetParentFrame();
	if (pFrame)
	{
		pFrame->RecalcLayout();
	}
}

void CXTPMessageBar::OleSetVisible(BOOL bNewValue)
{
	ShowWindow(bNewValue ? SW_SHOW : SW_HIDE);

	CFrameWnd* pFrame = GetParentFrame();
	if (pFrame)
	{
		pFrame->RecalcLayout();
	}
}

BSTR CXTPMessageBar::OleGetMessage()
{
	return m_strContent.AllocSysString();
}

void CXTPMessageBar::OleSetMessage(LPCTSTR lpszMessage)
{
	SetMessage(lpszMessage);
}

BOOL CXTPMessageBar::OleGetVisible()
{
	return IsWindowVisible();
}

void CXTPMessageBar::OleAddButton(long Id, LPCTSTR lpszCaption, LPCTSTR lpszToolTip)
{
	AddButton(Id, lpszCaption, lpszToolTip);
}

void CXTPMessageBar::OleAddCloseButton(LPCTSTR lpszToolTip)
{
	AddButton(SC_CLOSE, NULL, lpszToolTip);
}


#endif
