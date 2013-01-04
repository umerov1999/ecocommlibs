// XTPStatusBar.cpp : implementation of the CXTPStatusBar class.
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
#include "Resource.h"

#include "Common/XTPDrawHelpers.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPImageManager.h"
#include "Common/XTPToolTipContext.h"
#include "Common/XTPMarkupRender.h"
#include "Common/XTPPropExchange.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPHookManager.h"
#include "Common/XTPVC80Helpers.h"

#include "XTPCommandBarsDefines.h"
#include "XTPCommandBars.h"
#include "XTPCommandBar.h"
#include "XTPPopupBar.h"
#include "XTPScrollBase.h"
#include "XTPControls.h"
#include "XTPControl.h"
#include "XTPControlButton.h"
#include "XTPControlProgress.h"
#include "XTPStatusBar.h"
#include "XTPPaintManager.h"
#include "XTPMouseManager.h"

#ifdef _XTP_INCLUDE_RIBBON
#include "XTPCommandBarsFrameHook.h"
#endif


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define SBPF_UPDATE 0x0001  // pending update of text
#define CX_PANE_BORDER 2    // 3 pixels on each side of each pane

//////////////////////////////////////////////////////////////////////////
// CXTPStatusBarPane

IMPLEMENT_DYNAMIC(CXTPStatusBarPane, CCmdTarget)

CXTPStatusBarPane::CXTPStatusBarPane()
{
	m_nID = 0;
	m_cxText = 0;
	m_nStyle = 0;
	m_nBestFit = 0;

	m_nIndex = -1;
	m_pStatusBar = NULL;

	m_hWndPane = NULL;
	m_bAutoDeleteWnd = FALSE;
	m_dwHideFlags = 0;

	m_clrText = (COLORREF)-1;
	m_clrTextGray = (COLORREF)-1;
	m_clrBackground = (COLORREF)-1;

	m_nIconIndex = -1;
	m_nAlignment = DT_LEFT;

	m_rcMargins.SetRect(0, 0, 0, 0);
	m_rcPadding.SetRect(3, 1, 3, 1);

	m_bButton = FALSE;
	m_bPressed = FALSE;
	m_pMarkupUIElement = NULL;

	m_rcPane.SetRectEmpty();

	m_bBeginGroup = TRUE;

	m_bCustomizable = TRUE;

}

CXTPStatusBarPane::~CXTPStatusBarPane()
{
	CWnd* pWnd = CWnd::FromHandlePermanent(m_hWndPane);

	if (pWnd && ::IsWindow(pWnd->GetSafeHwnd()))
	{
		pWnd->DestroyWindow();

		if (m_bAutoDeleteWnd)
		{
			delete pWnd;
		}
	}

	XTPMarkupReleaseElement(m_pMarkupUIElement);
}

void CXTPStatusBarPane::SetButton(BOOL bButton)
{
	m_bButton = bButton;
	ShowBorders(FALSE);
}

BOOL CXTPStatusBarPane::IsButton() const
{
	return m_bButton;
}


CRect CXTPStatusBarPane::GetRect() const
{
	return m_rcPane;
}

void CXTPStatusBarPane::Redraw() const
{
	if (this && m_pStatusBar && m_pStatusBar->GetSafeHwnd() && IsVisible())
	{
		m_pStatusBar->InvalidateRect(GetRect());
	}
}

CRect CXTPStatusBarPane::GetMargins() const
{
	return m_rcMargins;
}

CRect CXTPStatusBarPane::GetPadding() const
{
	return m_rcPadding;
}

void CXTPStatusBarPane::SetMargins(int nLeft, int nTop, int nRight, int nBottom)
{
	m_rcMargins.SetRect(nLeft, nTop, nRight, nBottom);
}

void CXTPStatusBarPane::SetPadding(int nLeft, int nTop, int nRight, int nBottom)
{
	m_rcPadding.SetRect(nLeft, nTop, nRight, nBottom);
}

void CXTPStatusBarPane::SetText(LPCTSTR lpszText)
{
	CString strText = lpszText;
	if (m_strText == strText)
		return;

	XTPMarkupReleaseElement(m_pMarkupUIElement);
	m_strText = strText;

	if (m_pStatusBar->GetMarkupContext())
	{
		m_pMarkupUIElement = XTPMarkupParseText(m_pStatusBar->GetMarkupContext(), lpszText);
	}

	if (!IsVisible())
		return;

	if (m_cxText == 0 && m_nBestFit != GetBestFit())
	{
		m_pStatusBar->RecalcLayout();
	}
	else
	{
		Redraw();
	}
}

CString CXTPStatusBarPane::GetText() const
{
	return m_strText;
}

void CXTPStatusBarPane::SetWidth(int cxText)
{
	if (m_cxText != cxText)
	{
		m_cxText = cxText;
		m_pStatusBar->RecalcLayout();
	}
}

int CXTPStatusBarPane::GetWidth()
{
	return m_cxText <= 0 ? GetBestFit() : m_cxText;
}

void CXTPStatusBarPane::SetStyle(UINT nStyle)
{
	if (m_nStyle != nStyle)
	{
		// if the pane is changing SBPS_STRETCH, then...
		if ((m_nStyle ^ nStyle) & SBPS_STRETCH)
		{
			// ... we need to re-layout the panes
			m_nStyle = nStyle;
			m_pStatusBar->RecalcLayout();
		}

		// use SetPaneText, since it updates the style and text
		m_nStyle = nStyle;
		Redraw();
	}
}
UINT CXTPStatusBarPane::GetStyle() const
{
	return m_nStyle;
}

UINT CXTPStatusBarPane::GetID() const
{
	return m_nID;
}

int CXTPStatusBarPane::GetIndex() const
{
	return m_nIndex;
}

CXTPStatusBar* CXTPStatusBarPane::GetStatusBar() const
{
	return m_pStatusBar;
}

void CXTPStatusBarPane::Remove()
{
	m_pStatusBar->RemoveAt(m_nIndex);
}

void CXTPStatusBarPane::SetVisible(BOOL bVisible, BOOL bRecalcLayout)
{
	DWORD dwHideFlags = bVisible ? m_dwHideFlags &~ xtpHideGeneric : m_dwHideFlags | xtpHideGeneric;

	if (m_dwHideFlags != dwHideFlags)
	{
		m_dwHideFlags = dwHideFlags;

		if (bRecalcLayout) m_pStatusBar->RecalcLayout();
	}
}

BOOL CXTPStatusBarPane::IsVisible() const
{
	return m_dwHideFlags == xtpNoHide;
}

void CXTPStatusBarPane::OnDraw(CDC* pDC, CRect rcItem)
{
	CXTPPaintManager* pPaintManager = m_pStatusBar->GetPaintManager();

	if (pPaintManager)
	{
		pPaintManager->DrawStatusBarPaneEntry(pDC, rcItem, this);
	}
}

void CXTPStatusBarPane::BestFit()
{
	SetWidth(GetBestFit());
}

int CXTPStatusBarPane::GetBestFit()
{
	int cxText = 0;

	if (m_pMarkupUIElement)
	{
		XTPMarkupSetDefaultFont(m_pStatusBar->m_pMarkupContext, (HFONT)GetTextFont()->GetSafeHandle(), COLORREF_NULL);
		cxText = XTPMarkupMeasureElement(m_pMarkupUIElement).cx + m_rcPadding.left + m_rcPadding.right;
	}
	else
	{
		CClientDC dc(m_pStatusBar);
		CXTPFontDC font(&dc, GetTextFont());
		cxText = dc.GetTextExtent(m_strText).cx + m_rcPadding.left + m_rcPadding.right;
	}

	CXTPImageManagerIcon* pIcon = GetImage();
	if (pIcon) cxText += pIcon->GetWidth() + (!m_strText.IsEmpty() ? 2 : 0);

	return cxText;
}

void CXTPStatusBarPane::SetTextColor(COLORREF clrText)
{
	m_clrText = clrText;
	Redraw();
}

COLORREF CXTPStatusBarPane::GetTextColor() const
{
	return m_clrText;
}

COLORREF CXTPStatusBarPane::GetTextGrayColor() const
{
	return m_clrTextGray;
}

void CXTPStatusBarPane::SetTextGrayColor(COLORREF clrText)
{
	m_clrTextGray = clrText;
	Redraw();
}

void CXTPStatusBarPane::SetBackgroundColor(COLORREF clrBackground)
{
	m_clrBackground = clrBackground;
	Redraw();
}

COLORREF CXTPStatusBarPane::GetBackgroundColor() const
{
	return m_clrBackground;
}

CFont* CXTPStatusBarPane::GetTextFont()
{
	ASSERT(m_pStatusBar);
	return (m_pStatusBar && m_fntText.m_hObject == NULL) ?
		m_pStatusBar->GetFont(): &m_fntText;
}

void CXTPStatusBarPane::SetTextFont(CFont* pFntText)
{
	ASSERT(pFntText);
	if (!pFntText)
		return;

	LOGFONT lf;
	pFntText->GetLogFont(&lf);

	SetTextFont(&lf);
}

void CXTPStatusBarPane::SetTextFont(PLOGFONT pLogfText)
{
	ASSERT(pLogfText);

	m_fntText.DeleteObject();
	m_fntText.CreateFontIndirect(pLogfText);

	Redraw();
}

CXTPImageManagerIcon* CXTPStatusBarPane::GetImage() const
{
	if (m_nIconIndex != -1)
		return m_pStatusBar->GetImageManager()->GetImage(m_nIconIndex, 0);

	return NULL;
}

int CXTPStatusBarPane::GetIconIndex() const
{
	return m_nIconIndex;
}

void CXTPStatusBarPane::SetIconIndex(int nIconIndex)
{
	if (m_nIconIndex != nIconIndex)
	{
		m_nIconIndex = nIconIndex;
		m_pStatusBar->RecalcLayout();
	}
}

void CXTPStatusBarPane::SetTextAlignment(int nAlign)
{
	if (m_nAlignment != nAlign)
	{
		m_nAlignment = nAlign;
		Redraw();
	}
}

int CXTPStatusBarPane::GetTextAlignment() const
{
	return m_nAlignment;
}

void CXTPStatusBarPane::SetTooltip(LPCTSTR lpszTooltip)
{
	m_strToolTip = lpszTooltip;
}

CString CXTPStatusBarPane::GetTooltip() const
{
	return m_strToolTip;
}

void CXTPStatusBarPane::SetEnabled(BOOL bEnabled)
{
	SetStyle(bEnabled ? GetStyle() & ~SBPS_DISABLED : GetStyle() | SBPS_DISABLED);
}

BOOL CXTPStatusBarPane::IsEnabled() const
{
	return ((m_nStyle & SBPS_DISABLED) == 0);
}

BOOL CXTPStatusBarPane::IsChecked() const
{
	return ((m_nStyle & SBPS_POPOUT) == SBPS_POPOUT);
}

void CXTPStatusBarPane::SetChecked(BOOL bChecked)
{
	SetStyle(!bChecked ? GetStyle() & ~SBPS_POPOUT : GetStyle() | SBPS_POPOUT);
}

void CXTPStatusBarPane::OnMouseMove(CPoint /*point*/)
{

}

INT_PTR CXTPStatusBarPane::OnToolHitTest(CPoint /*point*/, TOOLINFO* /*pTI*/) const
{
	return -1;
}

void CXTPStatusBarPane::OnLButtonDown(CPoint point)
{
	if (!m_bButton)
		return;

	InternalAddRef();

	m_bPressed = TRUE;
	Redraw();

	CRect rcButton = GetRect();
	BOOL bClick = FALSE, bPressed;

	m_pStatusBar->SetCapture();

	while (m_pStatusBar && ::GetCapture() == m_pStatusBar->GetSafeHwnd())
	{
		MSG msg;

		if (!::GetMessage(&msg, NULL, 0, 0))
			break;

		switch (msg.message)
		{
			case WM_MOUSEMOVE:
				point = CPoint((short signed)LOWORD(msg.lParam), (short signed)HIWORD(msg.lParam));

				bPressed = rcButton.PtInRect(point);

				if (bPressed != m_bPressed)
				{
					m_bPressed = bPressed;
					Redraw();
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
	Redraw();

	if (bClick && m_pStatusBar)
	{
		m_pStatusBar->OnPaneClick(this);
	}

	InternalRelease();
}


//////////////////////////////////////////////////////////////////////////
// CXTPStatusBar

IMPLEMENT_DYNAMIC(CXTPStatusBar, CControlBar)

CXTPStatusBar::CXTPStatusBar()
{
	// setup default border/margin depending on type of system
	m_cyTopBorder = 2;
	m_cxLeftBorder = 0;
	m_cxRightBorder = 0;
	m_cyBottomBorder = 0;
	m_cxPaneSpacing = 2;

	m_bCustomizable = FALSE;

	m_nMinHeight = 20;

	m_pCommandBars = NULL;
	m_bDrawDisabledText = TRUE;
	m_pToolTipContext = new CXTPToolTipContext;


	m_pHighlightedPane = NULL;

	m_bShowSizeGripper = FALSE;
	m_nRibbonDividerIndex = 0;

	m_pMarkupContext = NULL;

}

BOOL CXTPStatusBar::IsWin4()
{
#if (_MSC_VER >= 1300)
	return TRUE;
#else
	DWORD dwVersion = ::GetVersion();
	return (BYTE)dwVersion >= 4;
#endif
}


CXTPStatusBar::~CXTPStatusBar()
{
	RemoveAll();

	CMDTARGET_RELEASE(m_pToolTipContext);

	XTPMarkupReleaseContext(m_pMarkupContext);
}

void CXTPStatusBar::EnableMarkup(BOOL bEnableMarkup)
{
	XTPMarkupReleaseContext(m_pMarkupContext);

	if (bEnableMarkup)
	{
		m_pMarkupContext = XTPMarkupCreateContext(m_hWnd);
	}
}

CXTPImageManager* CXTPStatusBar::GetImageManager() const
{
	if (m_pCommandBars)
		return m_pCommandBars->GetImageManager();

	return XTPImageManager();
}

CXTPStatusBarPane* CXTPStatusBar::HitTest(CPoint pt, CRect* lpRect) const
{
	for (int i = 0; i < GetPaneCount(); i++)
	{
		CXTPStatusBarPane* pPane = GetPane(i);
		if (!pPane->IsVisible())
			continue;

		CRect rc = pPane->GetRect();

		if (rc.PtInRect(pt))
		{
			if (lpRect)
				*lpRect = rc;

			return pPane;
		}
	}

	return NULL;
}

void CXTPStatusBar::RemoveAll()
{
	for (int i = 0; i < (int)m_arrPanes.GetSize(); i++)
	{
		m_arrPanes[i]->m_pStatusBar = NULL;
		m_arrPanes[i]->InternalRelease();
	}
	m_arrPanes.RemoveAll();

	m_pHighlightedPane = NULL;

	RecalcLayout();
}

CXTPStatusBarPane* CXTPStatusBar::GetPane(int nIndex) const
{
	ASSERT(nIndex >= 0 && nIndex < GetPaneCount());

	return nIndex >= 0 && nIndex < GetPaneCount() ? m_arrPanes[nIndex]: NULL;
}

CXTPStatusBarPane* CXTPStatusBar::FindPane(UINT nID) const
{
	for (int i = 0; i < GetPaneCount(); i++)
	{
		if (GetPane(i)->m_nID == nID)
			return GetPane(i);
	}
	return NULL;
}

int CXTPStatusBar::GetVisiblePaneCount() const
{
	int nCount = 0;

	for (int i = 0; i < GetPaneCount(); i++)
	{
		if (m_arrPanes[i]->IsVisible()) nCount++;
	}

	return nCount;
}

/////////////////////////////////////////////////////////////////////////////
// CXTPStatusBar creation, etc



BOOL CXTPStatusBar::Create(CWnd* pParentWnd, DWORD dwStyle, UINT nID)
{
	return CreateEx(pParentWnd, 0, dwStyle, nID);
}

CFont* CXTPStatusBar::GetFont()
{
	if (m_fontStatus.GetSafeHandle())
		return &m_fontStatus;

	return &GetPaintManager()->m_fontToolTip;
}

void CXTPStatusBar::SetFont(CFont* pFont, BOOL bRedraw /* = TRUE */)
{
	m_fontStatus.DeleteObject();

	if (pFont)
	{
		LOGFONT lf;
		pFont->GetLogFont(&lf);

		m_fontStatus.CreateFontIndirect(&lf);
	}

	if (bRedraw)
	{
		Invalidate(FALSE);
	}
}

BOOL CXTPStatusBar::CreateEx(CWnd* pParentWnd, DWORD dwCtrlStyle, DWORD dwStyle, UINT nID)
{
	ASSERT_VALID(pParentWnd);   // must have a parent

	// save the style (some of these style bits are MFC specific)
	m_dwStyle = (dwStyle & CBRS_ALL);

	// translate MFC style bits to windows style bits
	dwStyle &= ~CBRS_ALL;
	dwStyle |= CCS_NOPARENTALIGN | CCS_NOMOVEY | CCS_NODIVIDER | CCS_NORESIZE;

	if ((pParentWnd->GetStyle() & WS_THICKFRAME))
		m_bShowSizeGripper = TRUE;

	dwStyle |= dwCtrlStyle;

	CXTPDrawHelpers::RegisterWndClass(0, _T("XTPStatusBar"), CS_DBLCLKS);

	// create the HWND
	CRect rect;
	rect.SetRectEmpty();

	return CWnd::Create(_T("XTPStatusBar"), NULL, dwStyle, rect, pParentWnd, nID);
}

BOOL CXTPStatusBar::PreCreateWindow(CREATESTRUCT& cs)
{
	// in Win4, status bars do not have a border at all, since it is
	//  provided by the client area.
	if (IsWin4() &&
		(m_dwStyle & (CBRS_ALIGN_ANY | CBRS_BORDER_ANY)) == CBRS_BOTTOM)
	{
		m_dwStyle &= ~(CBRS_BORDER_ANY | CBRS_BORDER_3D);
	}
	return CControlBar::PreCreateWindow(cs);
}

BOOL CXTPStatusBar::SetIndicators(const UINT* lpIDArray, int nIDCount)
{
	ASSERT_VALID(this);
	ASSERT(nIDCount >= 1);  // must be at least one of them
	ASSERT(lpIDArray == NULL ||
		AfxIsValidAddress(lpIDArray, sizeof(UINT) * nIDCount, FALSE));
	ASSERT(::IsWindow(m_hWnd));

	RemoveAll();

	// copy initial data from indicator array
	BOOL bResult = TRUE;

	if (lpIDArray != NULL)
	{
		for (int i = 0; i < nIDCount; i++)
		{
			CXTPStatusBarPane* pPane = new CXTPStatusBarPane();
			m_arrPanes.Add(pPane);

			pPane->m_pStatusBar = this;
			pPane->m_nID = *lpIDArray++;

			if (pPane->m_nID != 0)
			{
				if (!pPane->m_strText.LoadString(pPane->m_nID))
				{
					TRACE1("Warning: failed to load indicator string 0x%04X.\n",
						pPane->m_nID);
					bResult = FALSE;
					break;
				}
			}
			else if (i == 0)
			{
				pPane->m_cxText = 1;
				pPane->m_nStyle |= (SBPS_STRETCH | SBPS_NOBORDERS);
			}
		}
	}
	RecalcLayout();

	return bResult;
}

void CXTPStatusBar::CalcInsideRect(CRect& rect, BOOL bHorz) const
{
	ASSERT_VALID(this);
	ASSERT(::IsWindow(m_hWnd));
	ASSERT(bHorz);  // vertical status bar not supported

	// subtract standard CControlBar borders
	CControlBar::CalcInsideRect(rect, bHorz);

	CXTPPaintManager* pPaintManager = GetPaintManager();
	rect.DeflateRect(pPaintManager->m_rcStatusBarBorder);

	// subtract size grip if present
	if (m_bShowSizeGripper && !::IsZoomed(::GetParent(m_hWnd)))
	{
		// size grip uses a border + size of scrollbar + cx border
		rect.right -= m_cxRightBorder + ::GetSystemMetrics(SM_CXVSCROLL) +
			::GetSystemMetrics(SM_CXBORDER) * 2;
	}
}

void CXTPStatusBar::RecalcLayout()
{
	if (!GetSafeHwnd())
		return;

	// get border information and client work area
	CRect rect;
	GetWindowRect(rect);
	rect.OffsetRect(-rect.left, -rect.top);
	CalcInsideRect(rect, TRUE);

	// determine extra space for stretchy pane
	int cxExtra = rect.Width();
	int nStretchyCount = 0, i;
	BOOL bSeparator = FALSE, bFirstVisible = TRUE;

	for (i = 0; i < GetPaneCount(); i++)
	{
		CXTPStatusBarPane* pPane = GetPane(i);
		pPane->m_nIndex = i;
		pPane->m_dwHideFlags &= ~xtpHideWrap;

		if (pPane->GetBeginGroup() && !bFirstVisible) bSeparator = TRUE;

		if (!pPane->IsVisible())
			continue;

		pPane->m_nBestFit = pPane->GetWidth();

		if (pPane->m_nStyle & SBPS_STRETCH)
			++nStretchyCount;

		cxExtra -= (pPane->m_nBestFit + (pPane->HasBorders() ? CX_PANE_BORDER : 0) + (bSeparator ? m_cxPaneSpacing : 0)
			+ pPane->m_rcMargins.left + pPane->m_rcMargins.right);

		bSeparator = FALSE;
		bFirstVisible = FALSE;
	}

	int left = m_cxLeftBorder;
	BOOL bHideAll = FALSE;
	bSeparator = FALSE;
	bFirstVisible = TRUE;

	for (i = 0; i < GetPaneCount(); i++)
	{
		CXTPStatusBarPane* pPane = GetPane(i);

		if (bHideAll)
			pPane->m_dwHideFlags |= xtpHideWrap;

		if (pPane->GetBeginGroup() && !bFirstVisible) bSeparator = TRUE;

		if (!pPane->IsVisible())
		{
			pPane->m_rcPane.SetRectEmpty();
			continue;
		}

		if (bSeparator)
			left += m_cxPaneSpacing;

		int nPaneWidth = pPane->m_nBestFit;
		int right = left + pPane->m_rcMargins.left;

		// determine size of the pane
		ASSERT(nPaneWidth >= 0);
		right += nPaneWidth + (pPane->HasBorders() ? CX_PANE_BORDER : 0);

		if ((pPane->m_nStyle & SBPS_STRETCH) && cxExtra > 0)
		{
			ASSERT(nStretchyCount != 0);
			int cxAddExtra = cxExtra / nStretchyCount;
			right += cxAddExtra;
			--nStretchyCount;
			cxExtra -= cxAddExtra;
		}
		if (right > rect.right)
		{
			if (pPane->m_nStyle & SBPS_STRETCH)
			{
				right = rect.right;
			}
			else
			{
				bHideAll = TRUE;
				pPane->m_dwHideFlags |= xtpHideWrap;
				pPane->m_rcPane.SetRectEmpty();
				continue;
			}
		}
		bSeparator = FALSE;
		bFirstVisible = FALSE;

		pPane->m_rcPane.SetRect(left, rect.top + pPane->m_rcMargins.top, right, rect.bottom - pPane->m_rcMargins.bottom);
		left = right + pPane->m_rcMargins.right;
	}

	m_pHighlightedPane = NULL;

	Invalidate(FALSE);

	PositionControls();

	CPoint pt;
	GetCursorPos(&pt);
	ScreenToClient(&pt);

	HandleMouseMove(pt);
}

void CXTPStatusBar::UpdateAllPanes(BOOL bUpdateRects, BOOL /*bUpdateText*/)
{
	if (bUpdateRects)
	{
		RecalcLayout();
	}
}


/////////////////////////////////////////////////////////////////////////////
// CXTPStatusBar attribute access

int CXTPStatusBar::CommandToIndex(UINT nIDFind) const
{
	ASSERT_VALID(this);

	if (GetPaneCount() <= 0)
		return -1;

	for (int i = 0; i < GetPaneCount(); i++)
	{
		if (GetPane(i)->m_nID == nIDFind)
			return i;
	}

	return -1;
}

UINT CXTPStatusBar::GetItemID(int nIndex) const
{
	ASSERT_VALID(this);

	CXTPStatusBarPane* pPane = GetPane(nIndex);
	ASSERT(pPane);

	if (!pPane)
		return 0;

	return pPane->m_nID;
}

void CXTPStatusBar::GetItemRect(int nIndex, LPRECT lpRect) const
{
	ASSERT_VALID(this);
	ASSERT(::IsWindow(m_hWnd));

	if (!lpRect)
		return;

	CXTPStatusBarPane* pPane = GetPane(nIndex);
	if (!pPane)
	{
		::SetRectEmpty(lpRect);
		return;
	}

	*lpRect = pPane->GetRect();
}

UINT CXTPStatusBar::GetPaneStyle(int nIndex) const
{
	CXTPStatusBarPane* pPane = GetPane(nIndex);
	ASSERT(pPane);

	if (!pPane)
		return 0;

	return pPane->m_nStyle;
}

void CXTPStatusBar::SetPaneStyle(int nIndex, UINT nStyle)
{
	CXTPStatusBarPane* pPane = GetPane(nIndex);
	ASSERT(pPane);

	if (pPane) pPane->SetStyle(nStyle);
}

void CXTPStatusBar::GetPaneInfo(int nIndex, UINT& nID, UINT& nStyle,
	int& cxWidth) const
{
	ASSERT_VALID(this);

	CXTPStatusBarPane* pPane = GetPane(nIndex);
	nID = pPane->m_nID;
	nStyle = pPane->m_nStyle;
	cxWidth = pPane->GetWidth();
}

void CXTPStatusBar::SetPaneInfo(int nIndex, UINT nID, UINT nStyle, int cxWidth)
{
	ASSERT_VALID(this);

	BOOL bChanged = FALSE;
	CXTPStatusBarPane* pPane = GetPane(nIndex);
	pPane->m_nID = nID;
	if (pPane->m_nStyle != nStyle)
	{
		if ((pPane->m_nStyle ^ nStyle) & SBPS_STRETCH)
			bChanged = TRUE;
		else
		{
			pPane->m_nStyle = nStyle;
			pPane->Redraw();
		}
		pPane->m_nStyle = nStyle;
	}
	if (cxWidth != pPane->m_cxText)
	{
		// change width of one pane -> invalidate the entire status bar
		pPane->m_cxText = cxWidth;
		bChanged = TRUE;
	}
	if (bChanged)
		RecalcLayout();
}

void CXTPStatusBar::GetPaneText(int nIndex, CString& s) const
{
	ASSERT_VALID(this);

	CXTPStatusBarPane* pPane = GetPane(nIndex);
	ASSERT(pPane);
	if (!pPane)
		return;

	s = pPane->m_strText;
}

CString CXTPStatusBar::GetPaneText(int nIndex) const
{
	ASSERT_VALID(this);

	CXTPStatusBarPane* pPane = GetPane(nIndex);
	ASSERT(pPane);
	if (!pPane)
		return _T("");

	return pPane->m_strText;
}

void CXTPStatusBar::SetPaneWidth(int nIndex, int cxText)
{
	CXTPStatusBarPane* pPane = GetPane(nIndex);
	ASSERT(pPane);
	if (!pPane)
		return;

	pPane->SetWidth(cxText);
}

BOOL CXTPStatusBar::SetPaneText(int nIndex, LPCTSTR lpszNewText)
{
	ASSERT_VALID(this);
	ASSERT(::IsWindow(m_hWnd));

	CXTPStatusBarPane* pPane = GetPane(nIndex);
	ASSERT(pPane);
	if (!pPane)
		return FALSE;

	pPane->SetText(lpszNewText);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CXTPStatusBar implementation

CSize CXTPStatusBar::CalcFixedLayout(BOOL, BOOL /*bHorz*/)
{
	ASSERT_VALID(this);
	ASSERT(::IsWindow(m_hWnd));

	// determine size of font being used by the status bar
	CSize szText(0, 0);
	{
		CClientDC dc(NULL);
		CXTPFontDC font(&dc, GetFont());
		szText = dc.GetTextExtent(_T("X"), 1);
	}

	CXTPPaintManager* pPaintManager = GetPaintManager();

	// determine size, including borders
	CSize size;
	size.cx = 32767;
	size.cy = szText.cy + m_cyBottomBorder + m_cyTopBorder + ::GetSystemMetrics(SM_CYBORDER) * 2 + pPaintManager->m_rcStatusBarBorder.top
		+ pPaintManager->m_rcStatusBarBorder.bottom;

	if (size.cy < m_nMinHeight)
		size.cy = m_nMinHeight;

	if (size.cy < pPaintManager->m_nStatusBarMinHeight)
		size.cy = pPaintManager->m_nStatusBarMinHeight;

	return size;
}

/////////////////////////////////////////////////////////////////////////////
// CXTPStatusBar message handlers

BEGIN_MESSAGE_MAP(CXTPStatusBar, CControlBar)
	//{{AFX_MSG_MAP(CXTPStatusBar)
	ON_WM_NCHITTEST_EX()
	ON_WM_NCPAINT()
	ON_MESSAGE(WM_PRINTCLIENT, OnPrintClient)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_NCCALCSIZE()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE_VOID(WM_MOUSELEAVE, OnMouseLeave)
	ON_WM_WINDOWPOSCHANGING()
	ON_MESSAGE(WM_SETTEXT, OnSetText)
	ON_MESSAGE(WM_GETTEXT, OnGetText)
	ON_MESSAGE(WM_GETTEXTLENGTH, OnGetTextLength)
	ON_MESSAGE(SB_SETMINHEIGHT, OnSetMinHeight)
	ON_MESSAGE(WM_IDLEUPDATECMDUI, OnIdleUpdateCmdUI)
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_SYSCOMMAND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

int CXTPStatusBar::OnCreate(LPCREATESTRUCT lpcs)
{
	if (CControlBar::OnCreate(lpcs) == -1)
		return -1;

	return 0;
}

void CXTPStatusBar::OnSysCommand(UINT nID, LPARAM lParam)
{
	if (nID == SC_SIZE + HTBOTTOM - HTSIZEFIRST + 1)
	{
		::SendMessage(::GetParent(m_hWnd), WM_SYSCOMMAND, (WPARAM)nID, lParam);
		return;

	}
	CControlBar::OnSysCommand(nID, lParam);
}

LRESULT CXTPStatusBar::OnNcHitTest(CPoint point)
{
	if (m_bShowSizeGripper && !::IsZoomed(::GetParent(m_hWnd)))
	{
		CXTPWindowRect rc(this);

		int dxGripper = (m_cxRightBorder + ::GetSystemMetrics(SM_CXVSCROLL) + ::GetSystemMetrics(SM_CXBORDER) * 2);

		if (GetExStyle() & WS_EX_LAYOUTRTL)
		{
			if (point.x < rc.left + dxGripper)
				return (LRESULT)HTBOTTOMLEFT;
		}
		else
		{
			if (point.x > rc.right - dxGripper)
				return (LRESULT)HTBOTTOMRIGHT;
		}

#ifdef _XTP_INCLUDE_RIBBON
		if (point.y >= rc.bottom - 3 && GetPaintManager()->BaseTheme() == xtpThemeRibbon && CXTPCommandBarsFrameHook::m_nMsgQueryFrameHook != 0)
		{
			CXTPCommandBarsFrameHook* pFrame = (CXTPCommandBarsFrameHook*)::SendMessage(::GetParent(m_hWnd), CXTPCommandBarsFrameHook::m_nMsgQueryFrameHook, 0, 0);
			if (pFrame && pFrame->IsFrameHasStatusBar())
			{
				return (LRESULT)HTBOTTOM;
			}
		}
#endif
	}

	return (LRESULT)HTCLIENT;
}

void CXTPStatusBarPane::SetCustomizationVisible(BOOL bVisible)
{
	DWORD dwHideFlags = bVisible ? m_dwHideFlags &~ xtpHideCustomize : m_dwHideFlags | xtpHideCustomize;

	if (m_dwHideFlags != dwHideFlags)
	{
		m_dwHideFlags = dwHideFlags;

		m_pStatusBar->RecalcLayout();
	}
}

BOOL CXTPStatusBarPane::GetCustomizationVisible() const
{
	return (m_dwHideFlags & xtpHideCustomize) == 0;
}

class CXTPStatusBar::CControlCustomization : public CXTPControlButton
{
public:
	CControlCustomization(CXTPStatusBarPane* pPane)
	{
		m_pPane = pPane;
	}
	void OnExecute()
	{
		m_pPane->SetCustomizationVisible(!m_pPane->GetCustomizationVisible());
		SetChecked(!GetChecked());
	}
protected:
	CXTPStatusBarPane* m_pPane;
};

CXTPPopupBar* CXTPStatusBar::GetStatusBarPopup()
{
	CXTPPopupBar* pPopup = CXTPPopupBar::CreatePopupBar(m_pCommandBars);

	CXTPControl* pControl = pPopup->GetControls()->Add(xtpControlLabel, XTP_IDS_CUSTOMIZE_STATUSBAR);
	pControl->SetItemDefault(TRUE);
	pControl->SetDescription(_T(""));

	for (int i = 0; i < GetPaneCount(); i++)
	{
		CXTPStatusBarPane* pPane = GetPane(i);
		if (pPane->GetID() == 0 || !pPane->m_bCustomizable)
			continue;

		pControl = pPopup->GetControls()->Add(new CControlCustomization(pPane));

		pControl->SetCaption(pPane->m_strCaption.IsEmpty() ? pPane->m_strText : pPane->m_strCaption);
		pControl->SetShortcutText(pPane->m_strValue);
		pControl->SetBeginGroup(pPane->GetBeginGroup() && i > 0);
		pControl->SetChecked(pPane->GetCustomizationVisible());
	}

	return pPopup;
}

void CXTPStatusBar::OnRButtonUp(UINT /*nFlags*/, CPoint point)
{
	if (m_bCustomizable)
	{
		ClientToScreen(&point);

		OnMouseLeave();

		CXTPPopupBar* pPopup = GetStatusBarPopup();

		if (pPopup && pPopup->GetControlCount() > 0)
		{
			CXTPCommandBars::TrackPopupMenu(pPopup, 0, point.x, point.y, this);
		}

		CMDTARGET_RELEASE(pPopup);
	}
}

void CXTPStatusBar::OnMouseLeave()
{
	HandleMouseMove(CPoint(-1, -1));
}

void CXTPStatusBar::HandleMouseMove(CPoint point)
{
	CXTPStatusBarPane* pPane = XTPMouseManager()->IsTrackedLock() ? NULL : HitTest(point);
	if (pPane && (!pPane->IsButton() || !pPane->IsEnabled()))
		pPane = NULL;

	if (pPane != m_pHighlightedPane)
	{
		CXTPStatusBarPane* pHighlightedPane = m_pHighlightedPane;
		m_pHighlightedPane = pPane;

		if (pHighlightedPane) pHighlightedPane->Redraw();
		if (m_pHighlightedPane) m_pHighlightedPane->Redraw();

		if (m_pHighlightedPane)
		{
			TRACKMOUSEEVENT tme =
			{
				sizeof(TRACKMOUSEEVENT), TME_LEAVE, m_hWnd
			};
			_TrackMouseEvent(&tme);
		}

		if (pHighlightedPane)
		{
			pHighlightedPane->OnMouseMove(CPoint(-1, -1));
		}
	}

	if (m_pHighlightedPane)
	{
		m_pHighlightedPane->OnMouseMove(point);
	}
}

void CXTPStatusBar::OnMouseMove(UINT nFlags, CPoint point)
{
	HandleMouseMove(point);


	CControlBar::OnMouseMove(nFlags, point);
}

void CXTPStatusBar::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CControlBar::OnLButtonDblClk(nFlags, point);

	NMMOUSE nm;
	ZeroMemory(&nm, sizeof(nm));

	nm.hdr.code = NM_DBLCLK;
	nm.hdr.idFrom = GetDlgCtrlID();
	nm.hdr.hwndFrom = GetSafeHwnd();
	nm.dwItemSpec = m_pHighlightedPane ? m_pHighlightedPane->GetIndex() : -1;
	nm.pt = point;

	if (GetOwner()->SendMessage(WM_NOTIFY, nm.hdr.idFrom, (LPARAM)&nm) != 0)
		return;

	if (m_pHighlightedPane)
	{
		m_pHighlightedPane->OnLButtonDown(point);
	}
}

void CXTPStatusBar::OnLButtonDown(UINT nFlags, CPoint point)
{
	CControlBar::OnLButtonDown(nFlags, point);

	if (m_pHighlightedPane)
	{
		m_pHighlightedPane->OnLButtonDown(point);
	}
}

void CXTPStatusBar::OnNcCalcSize(BOOL /*bCalcValidRects*/, NCCALCSIZE_PARAMS* lpncsp)
{
	// calculate border space (will add to top/bottom, subtract from right/bottom)
	CRect rect;
	rect.SetRectEmpty();
	CControlBar::CalcInsideRect(rect, TRUE);
	ASSERT(rect.top >= 2);

	// adjust non-client area for border space
	lpncsp->rgrc[0].left += rect.left;
	lpncsp->rgrc[0].top += rect.top - 2;
	lpncsp->rgrc[0].right += rect.right;
	lpncsp->rgrc[0].bottom += rect.bottom;
}

void CXTPStatusBar::OnBarStyleChange(DWORD dwOldStyle, DWORD dwNewStyle)
{
	if (m_hWnd != NULL &&
		((dwOldStyle & CBRS_BORDER_ANY) != (dwNewStyle & CBRS_BORDER_ANY)))
	{
		// recalc non-client area when border styles change
		SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_FRAMECHANGED);
	}
}

void CXTPStatusBar::OnNcPaint()
{
	EraseNonClient();
}

void CXTPStatusBar::OnSize(UINT nType, int cx, int cy)
{
	ASSERT_VALID(this);
	ASSERT(::IsWindow(m_hWnd));

	CControlBar::OnSize(nType, cx, cy);

	// need to adjust pane right edges (because of stretchy pane)
	RecalcLayout();
}

void CXTPStatusBar::OnWindowPosChanging(LPWINDOWPOS lpWndPos)
{
	// not necessary to invalidate the borders
	DWORD dwStyle = m_dwStyle;
	m_dwStyle &= ~(CBRS_BORDER_ANY);
	CControlBar::OnWindowPosChanging(lpWndPos);
	m_dwStyle = dwStyle;
}

LRESULT CXTPStatusBar::OnSetText(WPARAM, LPARAM lParam)
{
	ASSERT_VALID(this);
	ASSERT(::IsWindow(m_hWnd));

	int nIndex = CommandToIndex(0);
	if (nIndex < 0)
		return -1;

	return SetPaneText(nIndex, (LPCTSTR)lParam) ? 0 : -1;
}

LRESULT CXTPStatusBar::OnGetText(WPARAM wParam, LPARAM lParam)
{
	ASSERT_VALID(this);
	ASSERT(::IsWindow(m_hWnd));

	int nMaxLen = (int)wParam;
	if (nMaxLen == 0)
		return 0;       // nothing copied
	LPTSTR lpszDest = (LPTSTR)lParam;

	int nLen = 0;
	int nIndex = CommandToIndex(0); // use pane with ID zero
	if (nIndex >= 0)
	{
		CXTPStatusBarPane* pPane = GetPane(nIndex);
		nLen = pPane->m_strText.GetLength();
		if (nLen > nMaxLen)
			nLen = nMaxLen - 1; // number of characters to copy (less term.)
		MEMCPY_S(lpszDest, (LPCTSTR)pPane->m_strText, nLen*sizeof(TCHAR));
	}
	lpszDest[nLen] = '\0';
	return nLen + 1;      // number of bytes copied
}

LRESULT CXTPStatusBar::OnGetTextLength(WPARAM, LPARAM)
{
	ASSERT_VALID(this);
	ASSERT(::IsWindow(m_hWnd));

	int nLen = 0;
	int nIndex = CommandToIndex(0); // use pane with ID zero
	if (nIndex >= 0)
	{
		CXTPStatusBarPane* pPane = GetPane(nIndex);
		nLen = pPane->m_strText.GetLength();
	}
	return nLen;
}

LRESULT CXTPStatusBar::OnSetMinHeight(WPARAM wParam, LPARAM)
{
	LRESULT lResult = Default();
	m_nMinHeight = (int)wParam;
	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
// CXTPStatusBar idle update through CStatusCmdUI class

class CXTPStatusBar::CStatusCmdUI : public CCmdUI      // class private to this file!
{
public: // re-implementations only
	virtual void Enable(BOOL bOn);
	virtual void SetCheck(int nCheck);
	virtual void SetText(LPCTSTR lpszText);
};

void CXTPStatusBar::CStatusCmdUI::Enable(BOOL bOn)
{
	m_bEnableChanged = TRUE;
	CXTPStatusBar* pStatusBar = (CXTPStatusBar*)m_pOther;
	ASSERT(pStatusBar != NULL);
	if (!pStatusBar)
		return;
	ASSERT_KINDOF(CXTPStatusBar, pStatusBar);
	ASSERT(m_nIndex < m_nIndexMax);

	CXTPStatusBarPane* pPane = pStatusBar->GetPane(m_nIndex);
	if (pPane) pPane->SetEnabled(bOn);
}

void CXTPStatusBar::CStatusCmdUI::SetCheck(int nCheck) // "checking" will pop out the text
{
	CXTPStatusBar* pStatusBar = (CXTPStatusBar*)m_pOther;
	ASSERT(pStatusBar != NULL);
	if (!pStatusBar)
		return;
	ASSERT_KINDOF(CXTPStatusBar, pStatusBar);
	ASSERT(m_nIndex < m_nIndexMax);

	UINT nNewStyle = pStatusBar->GetPaneStyle(m_nIndex) & ~SBPS_POPOUT;
	if (nCheck != 0)
		nNewStyle |= SBPS_POPOUT;
	pStatusBar->SetPaneStyle(m_nIndex, nNewStyle);
}

void CXTPStatusBar::CStatusCmdUI::SetText(LPCTSTR lpszText)
{
	CXTPStatusBar* pStatusBar = (CXTPStatusBar*)m_pOther;
	ASSERT(pStatusBar != NULL);
	if (!pStatusBar)
		return;
	ASSERT_KINDOF(CXTPStatusBar, pStatusBar);
	ASSERT(m_nIndex < m_nIndexMax);

	pStatusBar->SetPaneText(m_nIndex, lpszText);
}

void CXTPStatusBar::OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler)
{
	CStatusCmdUI state;
	state.m_pOther = this;
	state.m_nIndexMax = (UINT)GetPaneCount();
	for (state.m_nIndex = 0; state.m_nIndex < state.m_nIndexMax;
		state.m_nIndex++)
	{
		state.m_nID = GetPane(state.m_nIndex)->m_nID;

		// allow the statusbar itself to have update handlers
		if (CWnd::OnCmdMsg(state.m_nID, CN_UPDATE_COMMAND_UI, &state, NULL))
			continue;

		// allow target (owner) to handle the remaining updates
		state.DoUpdate(pTarget, FALSE);
	}

	// update the dialog controls added to the status bar
	UpdateDialogControls(pTarget, bDisableIfNoHndler);
}

/////////////////////////////////////////////////////////////////////////////
// CXTPStatusBar diagnostics

#ifdef _DEBUG
void CXTPStatusBar::AssertValid() const
{
	CControlBar::AssertValid();
}

void CXTPStatusBar::Dump(CDumpContext& dc) const
{
	CControlBar::Dump(dc);

	if (dc.GetDepth() > 0)
	{
		for (int i = 0; i < GetPaneCount(); i++)
		{
			dc << "\nstatus pane[" << i << "] = {";
			dc << "\n\tnID = " << GetPane(i)->m_nID;
			dc << "\n\tnStyle = " << GetPane(i)->m_nStyle;
			dc << "\n\tcxText = " << GetPane(i)->m_cxText;
			dc << "\n\tstrText = " << GetPane(i)->m_strText;
			dc << "\n\t}";
		}
	}
	dc << "\n";
}
#endif //_DEBUG


CXTPPaintManager* CXTPStatusBar::GetPaintManager() const
{
	if (m_pCommandBars)
		return m_pCommandBars->GetPaintManager();

	return XTPPaintManager();
}

void CXTPStatusBar::DrawPaneEntry(CDC* pDC, int nIndex, CRect rcItem)
{
	CXTPStatusBarPane* pPane = GetPane(nIndex);

	if (pPane)
	{
		pPane->OnDraw(pDC, rcItem);
	}
}

void CXTPStatusBar::OnPaint()
{
	CPaintDC dcPaint (this);
	CXTPBufferDC dc(dcPaint);

	OnDraw(&dc);
}

LRESULT CXTPStatusBar::OnPrintClient(WPARAM wParam, LPARAM /*lParam*/)
{
	if (GetPaintManager() == NULL)
	{
		return Default();
	}

	CDC* pDC = CDC::FromHandle((HDC)wParam);
	if (pDC)
	{
		OnDraw(pDC);
	}

	return TRUE;
}

void CXTPStatusBar::OnDraw(CDC* pDC)
{
	CXTPClientRect rcClient(this);

	CXTPPaintManager* pPaintManager = GetPaintManager();
	pPaintManager->FillStatusBar(pDC, this);

	int nCount = GetPaneCount(), nGripperPane;

	for (nGripperPane = nCount - 1; nGripperPane >= 0; nGripperPane--)
	{
		if (m_arrPanes[nGripperPane]->IsVisible())
			break;
	}
	BOOL bSeparator = FALSE, bFirstVisible = TRUE;

	for (int i = 0; i < nCount; i++)
	{
		CXTPStatusBarPane* pPane = GetPane(i);

		if (pPane->GetBeginGroup() && !bFirstVisible) bSeparator = TRUE;

		if (!pPane->IsVisible())
			continue;

		CRect rcItem = pPane->GetRect();

		BOOL bGripperPane = (i == nGripperPane);

		if (pPane->HasBorders())
		{
			pPaintManager->DrawStatusBarPaneBorder(pDC, rcItem, pPane, bGripperPane);
		}
		else if (bSeparator)
		{
			pPaintManager->DrawStatusBarPaneSeparator(pDC, rcItem, pPane);
		}

		DrawPaneEntry(pDC, i, rcItem);

		bSeparator = FALSE;
		bFirstVisible = FALSE;
	}

	if (m_bShowSizeGripper && !::IsZoomed(::GetParent(m_hWnd)))
		DrawStretch(pDC, rcClient);
}

BOOL CXTPStatusBar::OnEraseBkgnd(CDC* /*pDC*/)
{
	return TRUE;
}

void CXTPStatusBar::DrawStretch(CDC* pDC, CRect& rcClient)
{
	GetPaintManager()->DrawStatusBarGripper(pDC, rcClient);
}



/////////////////////////////////////////////////////////////////////////////
// CXTPStatusBar message handlers

BOOL CXTPStatusBar::RemoveIndicator(UINT nID)
{
	int nIndex = CommandToIndex(nID);
	if (nIndex == -1)
	{
		TRACE1("WARNING: Pane ID %d was not found in the status bar.\n", nID);
		return FALSE;
	}

	return RemoveAt(nIndex);
}

BOOL CXTPStatusBar::RemoveAt(int nIndex)
{

	CXTPStatusBarPane* pPane = GetPane(nIndex);
	if (!pPane)
		return FALSE;

	if (m_pHighlightedPane == pPane)
	{
		m_pHighlightedPane = NULL;
	}

	m_arrPanes.RemoveAt(nIndex);

	pPane->m_pStatusBar = NULL;
	pPane->InternalRelease();

	UpdateAllPanes(TRUE, TRUE);

	return TRUE;
}

CXTPStatusBarPane* CXTPStatusBar::AddIndicator(CXTPStatusBarPane* pPane, UINT nID, int nIndex)
{
	if (nIndex < 0 || nIndex > GetPaneCount())
	{
		nIndex = GetPaneCount();
	}

	if (CommandToIndex(nID) != -1)
	{
		TRACE1("WARNING: Pane ID %d already exists in the status bar.\n", nID);
	}

	m_arrPanes.InsertAt(nIndex, pPane);
	pPane->m_nID = nID;
	pPane->m_pStatusBar = this;

	if (nID != 0)
	{
		CString strText;
		if (strText.LoadString(nID))
		{
			pPane->m_strText = strText;
		}
	}
	else if (nIndex == 0)
	{
		pPane->m_cxText = 1;
		pPane->m_nStyle |= (SBPS_STRETCH | SBPS_NOBORDERS);
	}

	RecalcLayout();

	return pPane;
}

CXTPStatusBarPane* CXTPStatusBar::AddIndicator(UINT nID, int nIndex)
{
	if (nIndex < 0 || nIndex > GetPaneCount())
	{
		nIndex = GetPaneCount();
	}

	if (CommandToIndex(nID) != -1)
	{
		TRACE1("WARNING: Pane ID %d already exists in the status bar.\n", nID);
		return NULL;
	}

	return AddIndicator(new CXTPStatusBarPane(), nID, nIndex);
}

void CXTPStatusBar::PositionControls()
{
	CXTPClientRect rcClient(this);

	int cx = ::GetSystemMetrics(SM_CXEDGE);

	for (int i = 0; i < GetPaneCount(); i++)
	{
		CXTPStatusBarPane* pPane = GetPane(i);
		if (pPane->m_hWndPane == NULL || !IsWindow(pPane->m_hWndPane))
			continue;

		CRect rcPane = pPane->GetRect();
		if (rcPane.IsRectEmpty() || !pPane->IsVisible())
		{
			::SetWindowPos(pPane->m_hWndPane, 0, 0, 0, 0, 0, SWP_HIDEWINDOW);
		}
		else
		{
			if (pPane->HasBorders())
			{
				rcPane.DeflateRect(cx, cx);
			}

			::SetWindowPos(pPane->m_hWndPane, 0, rcPane.left, rcPane.top, rcPane.Width(), rcPane.Height(), SWP_NOZORDER | SWP_SHOWWINDOW);
			::InvalidateRect(pPane->m_hWndPane, 0, FALSE);
		}
	}
}

BOOL CXTPStatusBar::AddControl(CWnd* pWnd, UINT nID, BOOL bAutoDelete)
{
	int nIndex = CommandToIndex (nID);

	if (nIndex == -1)
		return FALSE;

	CXTPStatusBarPane* pPane = GetPane(nIndex);
	if (!pPane)
		return FALSE;

	pPane->m_hWndPane = pWnd->GetSafeHwnd();
	pPane->m_bAutoDeleteWnd = bAutoDelete;

	PositionControls();
	return TRUE;
}

void CXTPStatusBar::OnWindowPosChanged(WINDOWPOS* lpwndpos)
{
	CControlBar::OnWindowPosChanged(lpwndpos);

	CXTPPaintManager* pPaintManager = GetPaintManager();
	if (pPaintManager && pPaintManager->BaseTheme() == xtpThemeRibbon)
	{
		if ((lpwndpos->flags & (SWP_SHOWWINDOW | SWP_HIDEWINDOW)) != 0)
		{
			GetParent()->SetWindowPos(0, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_FRAMECHANGED);
		}
	}
}

LRESULT CXTPStatusBar::OnIdleUpdateCmdUI(WPARAM wParam, LPARAM)
{
	// handle delay hide/show
	BOOL bVis = GetStyle() & WS_VISIBLE;
	UINT swpFlags = 0;
	if ((m_nStateFlags & delayHide) && bVis)
		swpFlags = SWP_HIDEWINDOW;
	else if ((m_nStateFlags & delayShow) && !bVis)
		swpFlags = SWP_SHOWWINDOW;
	m_nStateFlags &= ~(delayShow | delayHide);
	if (swpFlags != 0)
	{
		SetWindowPos(NULL, 0, 0, 0, 0, swpFlags |
			SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
	}

	// the style must be visible and if it is docked
	// the dockbar style must also be visible
	if ((GetStyle() & WS_VISIBLE) &&
		(m_pDockBar == NULL || (m_pDockBar->GetStyle() & WS_VISIBLE)))
	{
		// MFCBUG: removed check for IsFrameWnd so we can update statusbar
		// for dialog based applications as well as doc/view apps.
		CFrameWnd* pTarget = (CFrameWnd*)GetOwner();
		if (pTarget == NULL)
			pTarget = (CFrameWnd*)GetParent();
		if (pTarget != NULL)
			OnUpdateCmdUI(pTarget, (BOOL)wParam);
	}
	return 0L;
}

INT_PTR CXTPStatusBar::OnToolHitTest(CPoint point, TOOLINFO* pTI) const
{
	ASSERT_VALID(this);
	ASSERT(::IsWindow(m_hWnd));

	// check child windows first by calling CControlBar
	INT_PTR nHit = CControlBar::OnToolHitTest(point, pTI);
	if (nHit != -1)
		return nHit;

	CRect rcPane;
	CXTPStatusBarPane* pPane = HitTest(point, &rcPane);
	if (pPane)
	{
		nHit = pPane->OnToolHitTest(point, pTI);
		if (nHit != -1)
			return nHit;

		nHit = pPane->GetID();

		CString strTip = pPane->GetTooltip();
		if (strTip.IsEmpty())
			return -1;

		CXTPToolTipContext::FillInToolInfo(pTI, m_hWnd, rcPane, nHit, strTip, strTip, _T(""), GetImageManager());

		return nHit;
	}
	return -1;
}

void CXTPStatusBar::OnPaneClick(CXTPStatusBarPane* pPane)
{
	GetParent()->SendMessage(WM_COMMAND, pPane->GetID());
}

void CXTPStatusBar::OnSwitchPaneClick(CXTPStatusBarPane* pPane, long Id)
{
	pPane;
	GetParent()->SendMessage(WM_COMMAND, Id);
}



BOOL CXTPStatusBar::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{

	if (m_pMarkupContext)
	{
		CPoint ptMouse(0);
		GetCursorPos(&ptMouse);
		ScreenToClient(&ptMouse);

		CXTPStatusBarPane* pItem = HitTest(ptMouse);
		if (pItem && pItem->GetMarkupUIElement())
		{
			if (XTPMarkupRelayMessage(pItem->GetMarkupUIElement(), message, wParam, lParam, pResult))
				return TRUE;
		}
		else
		{
			if (XTPMarkupRelayMessage(GetMarkupContext(), message, wParam, lParam, pResult))
				return TRUE;
		}
	}


	if (m_pToolTipContext)
	{
		m_pToolTipContext->FilterToolTipMessage(this, message, wParam, lParam);
	}

	return CControlBar::OnWndMsg(message, wParam, lParam, pResult);
}

void CXTPStatusBar::EnableCustomization(BOOL bEnable)
{
	m_bCustomizable = bEnable;
}

void CXTPStatusBar::LoadState(LPCTSTR lpszProfileName)
{
	if (!AfxGetApp()->GetProfileInt(lpszProfileName, _T("StatusBar"), TRUE))
	{
		SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_HIDEWINDOW);
		DelayShow(FALSE);
	}

	if (m_bCustomizable)
	{
		CString strValue = AfxGetApp()->GetProfileString(lpszProfileName,  _T("StatusBarPanes"));

		strValue.Replace('\n', ';'); // For back compatibility

		while (!strValue.IsEmpty())
		{
			CString strPane;

			int nIndex = strValue.Find(_T(';'));
			if (nIndex == -1)
			{
				strPane = strValue;
				strValue.Empty();
			}
			else
			{
				strPane = strValue.Left(nIndex);
				strValue = strValue.Mid(nIndex + 1);
			}

			int nID, bVisible = 0;
			int nResult = SCANF_S(strPane, _T("%i=%i"), &nID, &bVisible);

			CXTPStatusBarPane* pPane = (nResult == 2) && (nID != 0) ? FindPane(nID) : NULL;
			if (pPane)
			{
				pPane->SetCustomizationVisible(bVisible);
			}
		}
	}
}

void CXTPStatusBar::DoPropExchange(CXTPPropExchange* pPX)
{
	if (pPX->IsStoring())
	{
		BOOL bVisible = !GetSafeHwnd() || IsVisible();

		PX_Bool(pPX, _T("Visible"), bVisible, TRUE);

		CString strValue;

		if (m_bCustomizable)
		{
			int nCount = GetPaneCount();

			for (int i = 0; i < nCount; i++)
			{
				CXTPStatusBarPane* pPane = GetPane(i);

				CString strPaneState;
				strPaneState.Format(_T("%i=%i"), pPane->GetID(), pPane->GetCustomizationVisible() ? 1 : 0);
				strValue += (strValue.IsEmpty() ? _T("") : _T(";")) + strPaneState;
			}
		}
		PX_String(pPX, _T("Panes"), strValue, _T(""));
	}
	else
	{
		BOOL bVisible = FALSE;
		PX_Bool(pPX, _T("Visible"), bVisible, TRUE);

		if (!bVisible)
		{
			SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_HIDEWINDOW);
			DelayShow(FALSE);
		}
		else if (GetSafeHwnd()  && !IsVisible())
		{
			DelayShow(TRUE);
		}

		CString strValue;
		PX_String(pPX, _T("Panes"), strValue, _T(""));

		if (m_bCustomizable)
		{
			while (!strValue.IsEmpty())
			{
				CString strPane;

				int nIndex = strValue.Find(_T(';'));
				if (nIndex == -1)
				{
					strPane = strValue;
					strValue.Empty();
				}
				else
				{
					strPane = strValue.Left(nIndex);
					strValue = strValue.Mid(nIndex + 1);
				}

				int nID, nVisible = 0;
				int nResult = SCANF_S(strPane, _T("%i=%i"), &nID, &nVisible);

				CXTPStatusBarPane* pPane = (nResult == 2) && (nID != 0) ? FindPane(nID) : NULL;
				if (pPane)
				{
					pPane->SetCustomizationVisible(nVisible);
				}
			}
		}
	}
}

void CXTPStatusBar::SaveState(LPCTSTR lpszProfileName)
{
	if (GetSafeHwnd() && !IsVisible())
	{
		AfxGetApp()->WriteProfileInt(lpszProfileName, _T("StatusBar"), FALSE);
	}
	else
	{
		AfxGetApp()->WriteProfileString(lpszProfileName, _T("StatusBar"), NULL);
	}

	if (m_bCustomizable)
	{
		int nCount = GetPaneCount();
		CString strValue;
		for (int i = 0; i < nCount; i++)
		{
			CXTPStatusBarPane* pPane = GetPane(i);

			CString strPaneState;
			strPaneState.Format(_T("%i=%i"), pPane->GetID(), pPane->GetCustomizationVisible() ? 1 : 0);
			strValue += (strValue.IsEmpty() ? _T("") : _T(";")) + strPaneState;
		}

		AfxGetApp()->WriteProfileString(lpszProfileName, _T("StatusBarPanes"), strValue.IsEmpty() ? (LPCTSTR)NULL : (LPCTSTR)strValue);
	}
	else
	{
		AfxGetApp()->WriteProfileString(lpszProfileName, _T("StatusBarPanes"), NULL);
	}
}


//////////////////////////////////////////////////////////////////////////
// CXTPStatusBarSwitchPane

IMPLEMENT_DYNAMIC(CXTPStatusBarSwitchPane, CXTPStatusBarPane)

CXTPStatusBarSwitchPane::CXTPStatusBarSwitchPane()
{
	m_nStyle |= SBPS_NOBORDERS;

	m_pHighlighted = NULL;
	m_bButton = TRUE;

	m_rcMargins.SetRect(0, 0, 0, 0);
	m_rcPadding.SetRect(0, 0, 0, 0);
}

CXTPStatusBarSwitchPane::~CXTPStatusBarSwitchPane()
{
	RemoveAll();
}

void CXTPStatusBarSwitchPane::RemoveAll()
{
	for (int i = 0; i < m_arrSwitches.GetSize(); i++)
	{
		delete m_arrSwitches[i];
	}
	m_arrSwitches.RemoveAll();
	m_pHighlighted = NULL;
}

BOOL CXTPStatusBarSwitchPane::SetSwitches(const UINT* lpIDArray, int nIDCount)
{
	for (int i = 0; i < nIDCount; i++)
	{
		if (!AddSwitch(lpIDArray[i]))
			return FALSE;
	}

	m_pStatusBar->RecalcLayout();

	return TRUE;
}

CXTPStatusBarSwitchPane::SWITCH* CXTPStatusBarSwitchPane::AddSwitch(UINT nID)
{
	CString strToolTip;
	strToolTip.LoadString(nID);

	return AddSwitch(nID, strToolTip);
}

CXTPStatusBarSwitchPane::SWITCH* CXTPStatusBarSwitchPane::AddSwitch(UINT nID, LPCTSTR lpszToolTip)
{
	SWITCH* pSwitch = new SWITCH();
	pSwitch->nWidth = 0;
	pSwitch->strToolTip = lpszToolTip;
	pSwitch->nID = nID;
	pSwitch->bHighlighted = FALSE;
	pSwitch->bChecked = FALSE;
	pSwitch->bPressed = FALSE;
	pSwitch->bEnabled = TRUE;

	m_arrSwitches.Add(pSwitch);
	return pSwitch;
}



void CXTPStatusBarSwitchPane::RemoveSwitch(UINT nID)
{
	for (int i = 0; i < m_arrSwitches.GetSize(); i++)
	{
		if (m_arrSwitches[i]->nID == nID)
		{
			delete m_arrSwitches[i];
			m_arrSwitches.RemoveAt(i);
			return;
		}
	}
}

CXTPStatusBarSwitchPane::SWITCH* CXTPStatusBarSwitchPane::FindSwitch(UINT nID) const
{
	for (int i = 0; i < m_arrSwitches.GetSize(); i++)
	{
		if (m_arrSwitches[i]->nID == nID)
		{
			return m_arrSwitches[i];
		}
	}
	return NULL;
}

void CXTPStatusBarSwitchPane::EnableSwitch(UINT nID, BOOL bEnabled)
{
	for (int i = 0; i < m_arrSwitches.GetSize(); i++)
	{
		if (m_arrSwitches[i]->nID == nID)
		{
			m_arrSwitches[i]->bEnabled = bEnabled;
			return;
		}
	}
}

int CXTPStatusBarSwitchPane::GetWidth()
{
	CXTPPaintManager* pPaintManager = m_pStatusBar->GetPaintManager();
	if (!pPaintManager)
		return 0 ;

	return m_cxText = pPaintManager->DrawStatusBarSwitchPane(NULL, CRect(0, 0, 0, 0), this);
}

void CXTPStatusBarSwitchPane::OnDraw(CDC* pDC, CRect rcItem)
{
	CXTPPaintManager* pPaintManager = m_pStatusBar->GetPaintManager();

	if (pPaintManager)
	{
		pPaintManager->DrawStatusBarSwitchPane(pDC, rcItem, this);
	}
}

int CXTPStatusBarSwitchPane::GetSwitchCount() const
{
	return (int)m_arrSwitches.GetSize();
}

CXTPStatusBarSwitchPane::SWITCH* CXTPStatusBarSwitchPane::GetSwitch(int nIndex) const
{
	return m_arrSwitches[nIndex];
}

CXTPStatusBarSwitchPane::SWITCH* CXTPStatusBarSwitchPane::HitTest(CPoint point, CRect& rcItem) const
{
	rcItem = GetRect();

	for (int i = 0; i < m_arrSwitches.GetSize(); i++)
	{
		rcItem.right = rcItem.left + m_arrSwitches[i]->nWidth;

		if (rcItem.PtInRect(point))
			return m_arrSwitches[i];

		rcItem.left = rcItem.right;
	}

	return NULL;
}

INT_PTR CXTPStatusBarSwitchPane::OnToolHitTest(CPoint point, TOOLINFO* pTI) const
{
	CRect rcItem(0, 0, 0, 0);

	SWITCH* pHighlighted = HitTest(point, rcItem);
	if (!pHighlighted)
		return -1;

	INT_PTR nHit = pHighlighted->nID;

	CString strTip = pHighlighted->strToolTip;
	if (strTip.IsEmpty())
		return -1;

	CXTPToolTipContext::FillInToolInfo(pTI, m_pStatusBar->m_hWnd, rcItem, nHit, strTip);

	return nHit;
}

void CXTPStatusBarSwitchPane::OnMouseMove(CPoint point)
{
	CRect rcItem(0, 0, 0, 0);

	SWITCH* pHighlighted = HitTest(point, rcItem);
	if (pHighlighted != m_pHighlighted)
	{
		if (m_pHighlighted)
		{
			m_pHighlighted->bHighlighted = FALSE;
		}

		m_pHighlighted = pHighlighted;

		if (m_pHighlighted)
		{
			m_pHighlighted->bHighlighted = TRUE;
		}

		Redraw();
	}
}

void CXTPStatusBarSwitchPane::OnLButtonDown(CPoint point)
{
	CRect rcItem;

	SWITCH* pHighlighted = HitTest(point, rcItem);
	if (!pHighlighted || !pHighlighted->bEnabled)
		return;

	InternalAddRef();

	pHighlighted->bPressed = TRUE;
	Redraw();

	m_pStatusBar->SetCapture();

	BOOL bClick = FALSE, bPressed;

	while (m_pStatusBar && ::GetCapture() == m_pStatusBar->GetSafeHwnd())
	{
		MSG msg;

		if (!::GetMessage(&msg, NULL, 0, 0))
			break;

		switch (msg.message)
		{
			case WM_MOUSEMOVE:
				point = CPoint((short signed)LOWORD(msg.lParam), (short signed)HIWORD(msg.lParam));

				bPressed = rcItem.PtInRect(point);

				if (bPressed != pHighlighted->bPressed)
				{
					pHighlighted->bPressed = bPressed;
					Redraw();
				}

				break;

			case WM_LBUTTONUP:
				bClick = pHighlighted->bPressed;
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

	pHighlighted->bPressed = FALSE;
	Redraw();

	if (bClick && m_pStatusBar)
	{
		m_pStatusBar->OnSwitchPaneClick(this, pHighlighted->nID);
	}

	InternalRelease();
}

void CXTPStatusBarSwitchPane::SetChecked(UINT nID)
{
	for (int i = 0; i < m_arrSwitches.GetSize(); i++)
	{
		SWITCH* pSwitch = m_arrSwitches[i];
		BOOL bChecked = pSwitch->nID == nID;

		if (pSwitch->bChecked != bChecked)
		{
			pSwitch->bChecked = bChecked;
			Redraw();
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// CXTPStatusBarScrollBarPane

IMPLEMENT_DYNAMIC(CXTPStatusBarScrollBarPane, CXTPStatusBarPane)

CXTPStatusBarScrollBarPane::CXTPStatusBarScrollBarPane()
{
	m_nMax = 100;
	m_nMin = 0;
	m_nValue = 0;
	m_nPage = 0;

	m_nStyle |= SBPS_NOBORDERS;
	m_bButton = TRUE;
	m_rcMargins.SetRect(0, 0, 0, 0);
	m_rcPadding.SetRect(0, 0, 0, 0);

	m_spi.fVert = FALSE;
}

CXTPStatusBarScrollBarPane::~CXTPStatusBarScrollBarPane()
{

}

CRect CXTPStatusBarScrollBarPane::GetScrollBarRect()
{
	CRect rcItem(GetRect());
	rcItem.DeflateRect(m_rcPadding);

	int nHeight = GetScrollBarPaintManager()->m_cyHScroll;
	if (rcItem.Height() > nHeight)
	{
		rcItem.top = (rcItem.top + rcItem.bottom - nHeight) / 2;
		rcItem.bottom = rcItem.top + nHeight;
	}

	return rcItem;
}

void CXTPStatusBarScrollBarPane::GetScrollInfo(SCROLLINFO* psi)
{
	psi->cbSize = sizeof(SCROLLINFO);

	psi->fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
	psi->nMax = m_nMax;
	psi->nMin = m_nMin;
	psi->nPage = m_nPage;
	psi->nPos = m_nValue;
}

BOOL CXTPStatusBarScrollBarPane::IsScrollBarEnabled() const
{
	return IsEnabled();
}

CWnd* CXTPStatusBarScrollBarPane::GetParentWindow() const
{
	return m_pStatusBar;
}

CXTPScrollBarPaintManager* CXTPStatusBarScrollBarPane::GetScrollBarPaintManager() const
{
	return (CXTPScrollBarPaintManager*)m_pStatusBar->GetPaintManager()->GetGalleryPaintManager();
}

void CXTPStatusBarScrollBarPane::OnDraw(CDC* pDC, CRect /*rcItem*/)
{
	if (!m_pSBTrack || !m_pSBTrack->bTrackThumb)
		SetupScrollInfo();

	GetScrollBarPaintManager()->DrawScrollBar(pDC, this);
}

void CXTPStatusBarScrollBarPane::DoScroll(int cmd, int pos)
{
	CWnd* pParent = m_pStatusBar->GetParent();

	NMXTPSCROLL nmScroll;
	nmScroll.hdr.code = XTP_SBN_SCROLL;
	nmScroll.hdr.idFrom = GetID();
	nmScroll.hdr.hwndFrom = m_pStatusBar->GetSafeHwnd();
	nmScroll.pSender = this;
	nmScroll.nPos = pos;
	nmScroll.nSBCode = cmd;

	LRESULT lResult = 0;
	AFX_NOTIFY notify;
	notify.pResult = &lResult;
	notify.pNMHDR = (NMHDR*)&nmScroll.hdr;

	pParent->OnCmdMsg(GetID(), MAKELONG(XTP_SBN_SCROLL, WM_NOTIFY), &notify, NULL);
}

void CXTPStatusBarScrollBarPane::RedrawScrollBar()
{
	Redraw();
}

void CXTPStatusBarScrollBarPane::OnMouseMove(CPoint point)
{
	int ht = HitTestScrollBar(point);

	if (ht != m_spi.ht)
	{
		m_spi.ht = ht;
		Redraw();
	}
}


void CXTPStatusBarScrollBarPane::OnLButtonDown(CPoint point)
{
	PerformTrackInit(m_pStatusBar->m_hWnd, point, &m_spi, (GetKeyState(VK_SHIFT) < 0) ? TRUE : FALSE);
}

void CXTPStatusBarScrollBarPane::SetPos(int nValue)
{
	if (m_nValue != nValue)
	{
		m_nValue = nValue;
		Redraw();
	}
}

int CXTPStatusBarScrollBarPane::GetPos() const
{
	return m_nValue;
}

void CXTPStatusBarScrollBarPane::SetRange(int nMin, int nMax)
{
	m_nMin = nMin;
	m_nMax = nMax;
}

INT_PTR CXTPStatusBarScrollBarPane::OnToolHitTest(CPoint point, TOOLINFO* pTI) const
{
	int ht = HitTestScrollBar(point);
	if (ht < XTP_HTSCROLLUP || ht > XTP_HTSCROLLTHUMB)
		return -1;

	CString strTip = m_strToolTipPart[ht - XTP_HTSCROLLUP];
	if (strTip.IsEmpty())
		return -1;

	CXTPToolTipContext::FillInToolInfo(pTI, m_pStatusBar->m_hWnd, GetScrollBarPartRect(ht), ht, strTip);

	return ht;
}

void CXTPStatusBarScrollBarPane::SetTooltipPart(int nPart, LPCTSTR lpszTooltip)
{
	if (nPart >= XTP_HTSCROLLUP && nPart <= XTP_HTSCROLLTHUMB)
	{
		m_strToolTipPart[nPart - XTP_HTSCROLLUP] = lpszTooltip;
	}
	else
	{
		if (nPart == SB_LINELEFT) m_strToolTipPart[0] = lpszTooltip;
		if (nPart == SB_LINERIGHT) m_strToolTipPart[1] = lpszTooltip;
		if (nPart == SB_PAGELEFT) m_strToolTipPart[2] = lpszTooltip;
		if (nPart == SB_PAGERIGHT) m_strToolTipPart[3] = lpszTooltip;
		if (nPart == SB_THUMBTRACK) m_strToolTipPart[4] = lpszTooltip;
	}
}

//////////////////////////////////////////////////////////////////////////
// CXTPStatusBarSliderPane

IMPLEMENT_DYNAMIC(CXTPStatusBarSliderPane, CXTPStatusBarScrollBarPane)

CXTPStatusBarSliderPane::CXTPStatusBarSliderPane()
{

}

CXTPStatusBarSliderPane::~CXTPStatusBarSliderPane()
{

}

CXTPScrollBarPaintManager* CXTPStatusBarSliderPane::GetScrollBarPaintManager() const
{
	return m_pStatusBar->GetPaintManager()->GetSliderPaintManager();
}

void CXTPStatusBarSliderPane::SetTicks(double* pTicks, int nCount)
{
	SAFE_DELETE(m_pTicks);

	if (nCount)
	{
		m_pTicks = new SLIDERTICKS(pTicks, nCount);
	}
}

//////////////////////////////////////////////////////////////////////////
// CXTPStatusBarProgressPane

IMPLEMENT_DYNAMIC(CXTPStatusBarProgressPane, CXTPStatusBarPane)

CXTPStatusBarProgressPane::CXTPStatusBarProgressPane()
{
	m_nStyle |= SBPS_NOBORDERS;
	m_rcMargins.SetRect(0, 0, 0, 0);
	m_rcPadding.SetRect(0, 0, 0, 0);
	m_cxText = 100;
}

CXTPStatusBarProgressPane::~CXTPStatusBarProgressPane()
{

}

CXTPProgressPaintManager* CXTPStatusBarProgressPane::GetProgressPaintManager() const
{
	return m_pStatusBar->GetPaintManager()->GetProgressPaintManager();
}

void CXTPStatusBarProgressPane::RedrawProgress()
{
	Redraw();
}

CRect CXTPStatusBarProgressPane::GetProgressRect()
{
	CRect rcItem(GetRect());
	rcItem.DeflateRect(m_rcPadding);

	int nHeight = GetProgressPaintManager()->m_cyProgress;
	if (rcItem.Height() > nHeight)
	{
		rcItem.top = (rcItem.top + rcItem.bottom - nHeight) / 2;
		rcItem.bottom = rcItem.top + nHeight;
	}

	return rcItem;
}

void CXTPStatusBarProgressPane::OnDraw(CDC* pDC, CRect /*rcItem*/)
{
	CXTPProgressPaintManager* pPaintManager = GetProgressPaintManager();

	if (pPaintManager)
	{
		pPaintManager->DrawProgress(pDC, this);
	}
}




