// XTPStatusBar.cpp : implementation of the CXTPStatusBar class.
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

#include "StdAfx.h"

#include <Common/XTPResourceImage.h>
#include <Common/XTPDrawHelpers.h>
#include <Common/XTPColorManager.h>
#include <Common/XTPImageManager.h>
#include <Common/XTPToolTipContext.h>
#include <Common/XTPMarkupRender.h>
#include <Common/XTPPropExchange.h>
#include <Common/XTPSystemHelpers.h>
#include <Common/XTPHookManager.h>
#include <Common/XTPVC80Helpers.h>
#include <Common/Hook/XTPWinEventHook.h>
#include <Common/ScrollBar/XTPScrollBase.h>
#include <Common/ScrollBar/XTPScrollInfo.h>
#include <Common/ScrollBar/XTPScrollBarPaintManager.h>

#include <Common/Controls/Progress/XTPProgressBase.h>

#include <CommandBars/Resource.h>
#include <CommandBars/XTPCommandBarsDefines.h>
#include <CommandBars/XTPCommandBars.h>
#include <CommandBars/XTPCommandBar.h>
#include <CommandBars/XTPPopupBar.h>
#include <CommandBars/Slider/XTPSliderPaintManager.h>
#include <CommandBars/XTPControls.h>
#include <CommandBars/XTPControl.h>
#include <CommandBars/XTPControlButton.h>
#include <CommandBars/XTPPaintManager.h>
#include <CommandBars/XTPMouseManager.h>
#include <CommandBars/Frame/XTPFrameHook.h>

#include <CommandBars/StatusBar/XTPStatusBar.h>
#include <CommandBars/StatusBar/XTPStatusBarPaintManager.h>
#include <CommandBars/StatusBar/Panes/XTPStatusBarPane.h>
#include <CommandBars/StatusBar/Panes/XTPStatusBarPaneScrollBar.h>
#include <CommandBars/StatusBar/Panes/XTPStatusBarPaneSwitch.h>
#include <CommandBars/StatusBar/Panes/XTPStatusBarPaneSlider.h>
#include <CommandBars/StatusBar/Panes/XTPStatusBarPaneProgress.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define SBPF_UPDATE 0x0001  // pending update of text
#define CX_PANE_BORDER 2    // 3 pixels on each side of each pane


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

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
	EnableConnections();
	m_strIdleText = _T("Ready");
#endif

	m_pHighlightedPane = NULL;

	m_bShowSizeGripper = FALSE;
	m_nRibbonDividerIndex = 0;

	m_pMarkupContext = NULL;

	m_bAllCaps = FALSE;
}

BOOL CXTPStatusBar::IsWin4()
{
#if (_MSC_VER >= 1300)
	return TRUE;
#else
	return XTPSystemVersion()->IsWinNT4OrGreater();
#endif
}


CXTPStatusBar::~CXTPStatusBar()
{
	RemoveAll();

	CMDTARGET_RELEASE(m_pToolTipContext);

#ifdef _XTP_ACTIVEX
	ReleaseConnections();
#endif
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

CXTPStatusBarPaintManager* CXTPStatusBar::GetStatusBarPaintManager() const
{
	return GetPaintManager()->GetStatusBarPaintManager();
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
	if (m_pCommandBars)
	{
		if (m_pCommandBars->GetPaintManager()->RequiresResourceImages() && !XTPResourceImages()->IsValid())
			return;
	}

	CXTPClientRect rcClient(this);

	CXTPStatusBarPaintManager* pPaintManager = GetStatusBarPaintManager();
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
	if (m_pCommandBars)
	{
		if (m_pCommandBars->GetPaintManager()->RequiresResourceImages() && !XTPResourceImages()->IsValid())
			return;
	}

	GetStatusBarPaintManager()->DrawStatusBarGripper(pDC, rcClient);
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
#ifndef _XTP_ACTIVEX
	GetParent()->SendMessage(WM_COMMAND, pPane->GetID());
#else
	FirePaneClick(pPane->GetIDispatch(FALSE));
#endif
}

void CXTPStatusBar::OnSwitchPaneClick(CXTPStatusBarPane* pPane, long Id)
{
#ifndef _XTP_ACTIVEX
	pPane;
	GetParent()->SendMessage(WM_COMMAND, Id);
#else
	FireSwitchPaneClick(pPane->GetIDispatch(FALSE), Id);
#endif
}



BOOL CXTPStatusBar::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
#ifdef _XTP_ACTIVEX
	if (message == WM_LBUTTONDOWN || message == WM_RBUTTONDOWN || message == WM_MBUTTONDOWN)
	{
		CPoint point((DWORD)lParam);
		FireMouseDown(AxButtonState(), AxShiftState(), point.x, point.y);
	}

	if (message == WM_LBUTTONUP || message == WM_RBUTTONUP || message == WM_MBUTTONUP)
	{
		CPoint point((DWORD)lParam);
		FireMouseUp(AxButtonState(), AxShiftState(), point.x, point.y);
	}

	if (message == WM_LBUTTONDBLCLK)
	{
		CPoint point((DWORD)lParam);
		CXTPStatusBarPane* pPane = HitTest(point);
		if (pPane)
		{
			FirePaneDblClick(pPane->GetIDispatch(FALSE));
		}
	}

	if (message == WM_LBUTTONDOWN)
	{
		CPoint point((DWORD)lParam);
		CXTPStatusBarPane* pPane = HitTest(point);
		if (pPane && !pPane->IsButton())
		{
			FirePaneClick(pPane->GetIDispatch(FALSE));
		}
	}
#endif

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




#ifdef _XTP_ACTIVEX


void CXTPStatusBar::OnFinalRelease()
{
	if (m_hWnd != NULL)
		DestroyWindow();

	CCmdTarget::OnFinalRelease();
}


BEGIN_DISPATCH_MAP(CXTPStatusBar, CCmdTarget)
	DISP_PROPERTY_EX_ID(CXTPStatusBar, "Visible", 1, OleGetVisible, OleSetVisible, VT_BOOL)
	DISP_FUNCTION_ID(CXTPStatusBar, "AddPane", 2, OleAddPane, VT_DISPATCH, VTS_I4)
	DISP_PROPERTY_NOTIFY_ID(CXTPStatusBar, "IdleText", 3, m_strIdleText, OleOnIdleTextChanged, VT_BSTR)

	DISP_FUNCTION_ID(CXTPStatusBar, "SetPaneText", 4, OleSetPaneText, VT_EMPTY, VTS_I4 VTS_BSTR)
	DISP_FUNCTION_ID(CXTPStatusBar, "SetPaneWidth", 5, OleSetPaneWidth, VT_EMPTY, VTS_I4 VTS_I4)
	DISP_FUNCTION_ID(CXTPStatusBar, "SetPaneStyle", 6, OleSetPaneStyle, VT_EMPTY, VTS_I4 VTS_I4)

	DISP_FUNCTION_ID(CXTPStatusBar, "RemoveAll", 8, RemoveAll, VT_EMPTY, VTS_NONE)
	DISP_PROPERTY_EX_ID(CXTPStatusBar, "PaneCount", 9, OleGetItemCount, SetNotSupported, VT_I4)
	DISP_FUNCTION_ID(CXTPStatusBar, "Pane", DISPID_VALUE, OleGetItem, VT_DISPATCH, VTS_I4)
	DISP_FUNCTION_ID(CXTPStatusBar, "NewEnum", DISPID_NEWENUM, OleNewEnum, VT_DISPATCH, VTS_NONE)

	DISP_FUNCTION_ID(CXTPStatusBar, "InsertPane", 10, OleInsertPane, VT_DISPATCH, VTS_I4 VTS_I4)
	DISP_FUNCTION_ID(CXTPStatusBar, "FindPane", 11, OleFindPane, VT_DISPATCH, VTS_I4)

	DISP_PROPERTY_ID(CXTPStatusBar, "DrawDisabledText", 12, m_bDrawDisabledText, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPStatusBar, "MinHeight", 13, OleGetMinHeight, OleSetMinHeight, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPStatusBar, "Font", DISPID_FONT, OleGetFont, OleSetFont, VT_FONT)
	DISP_FUNCTION_ID(CXTPStatusBar, "HitTest", 14, OleHitTest, VT_DISPATCH, VTS_XPOS_PIXELS VTS_YPOS_PIXELS)
	DISP_PROPERTY_EX_ID(CXTPStatusBar, "ToolTipContext", 15, OleGetToolTipContext, SetNotSupported, VT_DISPATCH)
	DISP_PROPERTY_EX_ID(CXTPStatusBar, "ShowSizeGripper", 16, OleGetShowSizeGripper, OleSetShowSizeGripper, VT_BOOL)

	DISP_FUNCTION_ID(CXTPStatusBar, "AddSwitchPane", 17, OleAddSwitchPane, VT_DISPATCH, VTS_I4)
	DISP_FUNCTION_ID(CXTPStatusBar, "AddSliderPane", 18, OleAddSliderPane, VT_DISPATCH, VTS_I4)
	DISP_FUNCTION_ID(CXTPStatusBar, "AddProgressPane", 21, OleAddProgressPane, VT_DISPATCH, VTS_I4)

	DISP_PROPERTY_ID(CXTPStatusBar, "RibbonDividerIndex", 19, m_nRibbonDividerIndex, VT_I4)

	DISP_FUNCTION_ID(CXTPStatusBar, "EnableCustomization", 20, EnableCustomization, VT_EMPTY, VTS_BOOL)
	DISP_PROPERTY_EX_ID(CXTPStatusBar, "EnableMarkup", 30, OleGetEnableMarkup, OleSetEnableMarkup, VT_BOOL)

	DISP_PROPERTY_EX_ID(CXTPStatusBarPane, "hWnd", DISPID_HWND, OleGetHwnd, SetNotSupported, VT_HANDLE)

	DISP_FUNCTION_ID(CXTPStatusBar, "SetAllCaps", 32, OleSetAllCaps, VT_EMPTY, VTS_BOOL)
	DISP_PROPERTY_EX_ID(CXTPStatusBar, "IsAllCaps", 31, OleIsAllCaps, SetNotSupported, VT_BOOL)

END_DISPATCH_MAP()

// {23745B78-7A0A-447d-9CDC-8F63E7E5AF55}
static const GUID IID_IStatusBar =
{ 0x23745b78, 0x7a0a, 0x447d, { 0x9c, 0xdc, 0x8f, 0x63, 0xe7, 0xe5, 0xaf, 0x55 } };


BEGIN_INTERFACE_MAP(CXTPStatusBar, CCmdTarget)
	INTERFACE_PART(CXTPStatusBar, IID_IStatusBar, Dispatch)
	//INTERFACE_PART(CXTPStatusBar, IID_IEnumVARIANT, EnumVARIANT)
	INTERFACE_PART(CXTPStatusBar, IID_IConnectionPointContainer, ConnPtContainer)

END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPStatusBar, IID_IStatusBar)

// {280A7B16-7BE8-42ec-870C-8849F5540F14}
static const GUID IID_IStatusBarEvents =
{ 0x280a7b16, 0x7be8, 0x42ec, { 0x87, 0xc, 0x88, 0x49, 0xf5, 0x54, 0xf, 0x14 } };


IMPLEMENT_CONNECTION_EX(CXTPStatusBar, IID_IStatusBarEvents)


OLE_HANDLE CXTPStatusBar::OleGetHwnd()
{
	return (OLE_HANDLE)m_hWnd;
}

BOOL CXTPStatusBar::OleGetVisible()
{
	return IsWindowVisible();
}

long CXTPStatusBar::OleGetMinHeight()
{
	return m_nMinHeight;
}

void CXTPStatusBar::OleSetEnableMarkup(BOOL bEnableMarkup)
{
	EnableMarkup(bEnableMarkup);

}

BOOL CXTPStatusBar::OleGetEnableMarkup()
{
	return m_pMarkupContext != NULL;
}

void CXTPStatusBar::OleSetAllCaps(BOOL bAllCaps)
{
	SetAllCaps(bAllCaps);
}

BOOL CXTPStatusBar::OleIsAllCaps()
{
	return IsAllCaps();
}

LPDISPATCH CXTPStatusBar::OleGetToolTipContext()
{
	return GetToolTipContext()->GetIDispatch(TRUE);
}
void CXTPStatusBar::OleSetMinHeight(int nMinHeight)
{
	m_nMinHeight = nMinHeight;
}

void CXTPStatusBar::OleSetShowSizeGripper(BOOL bGripper)
{
	m_bShowSizeGripper = bGripper;
	RecalcLayout();
}

BOOL CXTPStatusBar::OleGetShowSizeGripper()
{
	return m_bShowSizeGripper;
}

void CXTPStatusBar::OleSetVisible(BOOL bNewValue)
{
	ShowWindow(bNewValue ? SW_SHOW : SW_HIDE);

	CFrameWnd* pFrame = GetParentFrame();
	if (pFrame)
	{
		pFrame->RecalcLayout();
	}
}

void CXTPStatusBar::OleOnIdleTextChanged()
{
	GetParent()->SendMessage(WM_POPMESSAGESTRING, 0, (LPARAM)(LPCTSTR)m_strIdleText);
}

#define SBPF_UPDATE 0x0001

LPDISPATCH CXTPStatusBar::OleAddPane(long nID)
{
	ASSERT_VALID(this);

	CXTPStatusBarPane* pPane = AddIndicator(nID, GetPaneCount());
	OleOnIdleTextChanged();

	return XTPGetDispatch(pPane);
}

LPDISPATCH CXTPStatusBar::OleAddSwitchPane(long nID)
{
	CXTPStatusBarPane* pPane = AddIndicator(new CXTPStatusBarSwitchPane(), nID, GetPaneCount());
	return XTPGetDispatch(pPane);
}

LPDISPATCH CXTPStatusBar::OleAddSliderPane(long nID)
{
	CXTPStatusBarPane* pPane = AddIndicator(new CXTPStatusBarSliderPane(), nID, GetPaneCount());
	return XTPGetDispatch(pPane);
}

LPDISPATCH CXTPStatusBar::OleAddProgressPane(long nID)
{
	CXTPStatusBarPane* pPane = AddIndicator(new CXTPStatusBarProgressPane(), nID, GetPaneCount());
	return XTPGetDispatch(pPane);
}


LPDISPATCH CXTPStatusBar::OleFindPane(long nID)
{
	ASSERT_VALID(this);

	CXTPStatusBarPane* pPane = FindPane(nID);

	return XTPGetDispatch(pPane);
}

LPDISPATCH CXTPStatusBar::OleInsertPane(int nIndex, int nID)
{
	CXTPStatusBarPane* pPane = AddIndicator(nID, nIndex);

	return XTPGetDispatch(pPane);
}

void CXTPStatusBar::OleSetPaneText(long nIndex, LPCTSTR strText)
{
	if (nIndex >= GetPaneCount())
		nIndex = GetPaneCount() - 1;

	SetPaneText(nIndex, strText);
}

void CXTPStatusBar::OleSetPaneStyle(long nIndex, long nStyle)
{
	if (nIndex >= GetPaneCount())
		nIndex = GetPaneCount() - 1;

	SetPaneStyle(nIndex, nStyle);
}

void CXTPStatusBar::OleSetPaneWidth(long nIndex, long nWidth)
{
	if (nIndex >= GetPaneCount())
		nIndex = GetPaneCount() - 1;

	SetPaneWidth(nIndex, nWidth);
}

IMPLEMENT_ENUM_VARIANT(CXTPStatusBar);

long CXTPStatusBar::OleGetItemCount()
{
	return GetPaneCount();
}

LPDISPATCH CXTPStatusBar::OleGetItem(int nItem)
{
	CXTPStatusBarPane* pItem = GetPane(nItem);

	return pItem ? pItem->GetIDispatch(TRUE) : FALSE;
}

LPFONTDISP CXTPStatusBar::OleGetFont()
{
	return AxCreateOleFont(GetFont(), this, (LPFNFONTCHANGED)&CXTPStatusBar::OleSetFont);
}

void CXTPStatusBar::OleSetFont(LPFONTDISP pFontDisp)
{
	ASSERT((pFontDisp == NULL) ||
		AfxIsValidAddress(pFontDisp, sizeof(IDispatch), FALSE));

	if (AxCreateFontFromDispatch(&m_fontStatus, pFontDisp))
	{
		SetFont(&m_fontStatus);
	}
	else
	{
		SetFont(NULL);
	}
}

LPDISPATCH CXTPStatusBar::OleHitTest(OLE_XPOS_PIXELS x, OLE_YPOS_PIXELS y)
{
	CXTPStatusBarPane* pItem = HitTest(CPoint(x, y));
	return pItem ? pItem->GetIDispatch(TRUE) : NULL;
}




#endif
