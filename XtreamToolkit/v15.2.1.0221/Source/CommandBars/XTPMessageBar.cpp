// XTPMessageBar.cpp : implementation of the CXTPMessageBar class.
//
// This file is a part of the XTREME COMMANDBARS MFC class library.
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

#include "Common/XTPMarkupRender.h"
#include "Common/XTPToolTipContext.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPDrawHelpers.h"

#include "XTPCommandBarsDefines.h"
#include "XTPCommandBars.h"
#include "XTPPaintManager.h"
#include "XTPMessageBar.h"

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
{
	m_pMarkupContext = NULL;
	m_pUIElement = NULL;

	m_rcBorder.SetRect(3, 3, 3, 3);
	m_rcPadding.SetRect(5, 0, 5, 0);
	m_rcContent.SetRectEmpty();

	m_pCommandBars = NULL;

	m_pHotButton = NULL;

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

CSize CXTPMessageBar::CalcFixedLayout(BOOL, BOOL /*bHorz*/)
{
	ASSERT_VALID(this);
	ASSERT(::IsWindow(m_hWnd));

	CSize size(32767, 40);
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
		delete m_arrButtons[i];
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

	GetPaintManager()->DrawMessageBarButton(pDC, pButton);

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
	GetPaintManager()->FillMessageBar(pDC, this);
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

	CXTPClientRect rc(this);
	rc.left += m_rcBorder.left;
	rc.top += m_rcBorder.top;
	rc.right -= m_rcBorder.right + (bCloseButton ? 18 : 0);
	rc.bottom -= m_rcBorder.bottom;

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

	m_rcContent = CRect(CPoint(m_rcBorder.left + m_rcPadding.left, (rcClient.Height() - szContent.cy) /2), szContent);

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

	int nRight = rcClient.right - m_rcBorder.right - m_rcPadding.right - (pCloseButton ? 18 : 0);
	if (nLeft > nRight)
	{
		nLeft = m_rcBorder.left + m_rcPadding.left;

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
		CSize szButton = MeasureButton(pDC, pCloseButton);
		pCloseButton->m_rcButton = CRect(CPoint(rcClient.right - szButton.cx - 1, m_rcBorder.top), szButton);
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
	}
	else
	{
		m_pCommandBars->GetSite()->SendMessage(WM_COMMAND, pButton->m_nID);
	}
}

