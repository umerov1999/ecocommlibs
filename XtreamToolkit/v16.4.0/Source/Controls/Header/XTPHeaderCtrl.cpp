// XTPHeaderCtrl.cpp : implementation of the CXTPHeaderCtrl class.
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
#include "Common/XTPResourceManager.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPImageManager.h"
#include "../Util/XTPControlTheme.h"
#include "Common/XTPDrawHelpers.h"

#include "../Defines.h"
#include "../Resource.h"
#include "../Util/XTPGlobal.h"
#include "XTPHeaderCtrlTheme.h"
#include "XTPHeaderCtrl.h"
#include "../Util/XTPFunctions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTPHeaderCtrl

CXTPHeaderCtrl::CXTPHeaderCtrl()
: m_iMinSize(0)
, m_nPos(0)
, m_iOverIndex(-1)
, m_nSortedCol(-1)
, m_bRTL(DetermineRTL())
, m_bAutoSize(false)
, m_bEnableMenus(TRUE)
, m_bAscending(TRUE)
, m_bLBtnDown(FALSE)
, m_bPainted(FALSE)
, m_popupMenuID(0)
, m_pt(CPoint(0, 0))
, m_pTheme(NULL)
{
	m_pImageManager = NULL;
	VERIFY(SetTheme(xtpControlThemeDefault));
}

CXTPHeaderCtrl::~CXTPHeaderCtrl()
{
	CMDTARGET_RELEASE(m_pTheme);
	CMDTARGET_RELEASE(m_pImageManager);
}

void CXTPHeaderCtrl::SetImageManager(CXTPImageManager* pImageManager)
{
	CMDTARGET_RELEASE(m_pImageManager);
	m_pImageManager = pImageManager;
}

BEGIN_MESSAGE_MAP(CXTPHeaderCtrl, CHeaderCtrl)
	//{{AFX_MSG_MAP(CXTPHeaderCtrl)
	ON_WM_WINDOWPOSCHANGING()
	ON_WM_SETCURSOR()
	ON_WM_PAINT()
	ON_MESSAGE(WM_PRINTCLIENT, OnPrintClient)
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_RBUTTONDOWN()
	ON_NOTIFY_REFLECT_EX(HDN_ITEMCHANGING, OnItemchanging)
	ON_COMMAND(XTP_IDC_SORTASC, OnSortAsc)
	ON_COMMAND(XTP_IDC_SORTDSC, OnSortDsc)
	ON_COMMAND(XTP_IDC_ALIGNLEFT, OnAlignLeft)
	ON_COMMAND(XTP_IDC_ALIGNCENTER, OnAlignCenter)
	ON_COMMAND(XTP_IDC_ALIGNRIGHT, OnAlignRight)
	ON_MESSAGE(HDM_LAYOUT, OnLayout)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_XTP_SETCONTROLTHEME, OnSetTheme)
END_MESSAGE_MAP()

IMPLEMENT_DYNAMIC(CXTPHeaderCtrl, CHeaderCtrl)

/////////////////////////////////////////////////////////////////////////////
// CXTPHeaderCtrl message handlers

void CXTPHeaderCtrl::OnThemeChanged()
{
	RecalcLayout();
}

BOOL CXTPHeaderCtrl::IsThemeValid() const
{
	return (m_pTheme != NULL);
}

void CXTPHeaderCtrl::ApplyFieldWidths(int iNewWidth)
{
	CListCtrl* pListCtrl = (CListCtrl*)GetParent();
	ASSERT_VALID(pListCtrl);

	int iItemCount = GetItemCount();
	int iFrozenCount = (int)m_arFrozenCols.GetCount();
	int iThawedCount = iItemCount - iFrozenCount;

	if (iThawedCount <= 0)
		return;

	int iWidth = iNewWidth/iThawedCount;

	int iItem;
	for (iItem = 0; iItem < iItemCount; iItem++)
	{
		if (IsColFrozen(iItem))
			continue;

		if (iWidth > m_iMinSize)
			pListCtrl->SetColumnWidth(iItem, iWidth);

		iNewWidth -= iWidth;
	}
}

void CXTPHeaderCtrl::ResizeColumnsToFit()
{
	FitFieldWidths(0);
}

void CXTPHeaderCtrl::FitFieldWidths(int iNewWidth)
{
	if (iNewWidth <= 0)
	{
		CXTPWindowRect rc(this);
		iNewWidth = rc.Width();
	}

	// adjust for vertical scroll bar.
	DWORD dwStyle = ::GetWindowLong(::GetParent(m_hWnd), GWL_STYLE);
	if ((dwStyle & WS_VSCROLL) == WS_VSCROLL)
		iNewWidth -= ::GetSystemMetrics(SM_CXVSCROLL);

	// adjust for frozen columns.
	iNewWidth -= GetFrozenColWidth();

	ApplyFieldWidths(iNewWidth);
}

void CXTPHeaderCtrl::OnWindowPosChanging(WINDOWPOS FAR* lpwndpos)
{
	if (m_bAutoSize)
	{
		int iCount = GetItemCount();
		if (iCount > 0)
		{
			if (GetCapture() != GetParent()) // indicates scrolling
			{
				// is the window size changing?
				CXTPWindowRect rc(this);
				if (rc.Width() != lpwndpos->cx)
				{
					FitFieldWidths(lpwndpos->cx);
				}
			}
		}
	}
	else
	{
		CHeaderCtrl::OnWindowPosChanging(lpwndpos);
	}
}

void CXTPHeaderCtrl::EnableAutoSize(bool bEnable/*=true*/)
{
	m_bAutoSize = bEnable;
}

void CXTPHeaderCtrl::FreezeColumn(int iCol)
{
	m_arFrozenCols.AddTail(iCol);
}

void CXTPHeaderCtrl::ThawColumn(int iCol)
{
	for (POSITION pos = m_arFrozenCols.GetHeadPosition(); pos; m_arFrozenCols.GetNext(pos))
	{
		int iNext = m_arFrozenCols.GetAt(pos);
		if (iNext == iCol)
		{
			m_arFrozenCols.RemoveAt(pos);
			break;
		}
	}
}

void CXTPHeaderCtrl::ThawAllColumns()
{
	m_arFrozenCols.RemoveAll();
}

BOOL CXTPHeaderCtrl::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	if (m_arFrozenCols.GetCount())
	{
		CPoint pt;
		::GetCursorPos(&pt);

		CPoint ptClient = pt;
		ScreenToClient(&ptClient);

		HDHITTESTINFO hti;
		::ZeroMemory(&hti, sizeof(hti));
		hti.pt.x = ptClient.x;
		hti.pt.y = ptClient.y;

		int nIndex = (int)::SendMessage(GetSafeHwnd(),
			HDM_HITTEST, 0L, (LPARAM)&hti);

		if (nIndex > -1)
		{
			// if we are over one of the frozen columns, we can stop
			if (IsColFrozen(nIndex))
			{
				::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
				return TRUE;
			}

			else
			{
				// determine if the current index is adjacent to a frozen column index.
				// if columns are resized by dragging to the right, test for the frozen column on the left.
				// if columns are resized by dragging to the left, test for the frozen column on the right.

				int iAdjIndex = nIndex + (m_bRTL ? 1 : -1);
				if ((iAdjIndex > -1) && IsColFrozen(iAdjIndex))
				{
					CRect r;
					Header_GetItemRect(m_hWnd, nIndex, &r);
					int nMidPoint = (r.left + (r.Width()/2));

					// if columns resize to the right and the point is the right half of the header item...
					if (!m_bRTL && (ptClient.x <= nMidPoint))
					{
						::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
						return TRUE;
					}

					// if columns resize to the left and the point is the left half of the header item...
					else if (m_bRTL && (ptClient.x >= nMidPoint))
					{
						::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
						return TRUE;
					}
				}
			}
		}
	}

	return CHeaderCtrl::OnSetCursor(pWnd, nHitTest, message);
}

BOOL CXTPHeaderCtrl::OnItemchanging(NMHDR* pNMHDR, LRESULT* pResult)
{
	HD_NOTIFY* pHDN = (HD_NOTIFY*)pNMHDR;
	*pResult = FALSE;

	if (pHDN && (pHDN->pitem->mask & HDI_WIDTH) != 0)
	{
		// if sizing is disabled for this column, keep the user from resizing
		if (m_arFrozenCols.GetCount() && IsColFrozen(pHDN->iItem))
			*pResult = TRUE;

		// if the column is smaller than the minimum size, keep the user from resizing
		if (pHDN->pitem->mask & HDI_WIDTH && pHDN->pitem->cxy < m_iMinSize)
			*pResult = TRUE;
	}

	return BOOL(*pResult);
}

bool CXTPHeaderCtrl::IsColFrozen(int iCol)
{
	for (POSITION pos = m_arFrozenCols.GetHeadPosition(); pos; m_arFrozenCols.GetNext(pos))
	{
		int iNext = m_arFrozenCols.GetAt(pos);
		if (iNext == iCol)
		{
			return true;
		}
	}

	return false;
}

bool CXTPHeaderCtrl::DetermineRTL()
{
	CWindowDC dc(NULL);

	// determine if columns are resized by dragging them right (most languages) or
	// left (RTL languages like Arabic & Hebrew).

	UINT nAlign = dc.GetTextAlign();
	ASSERT(nAlign != GDI_ERROR);

	// will only be true for RTL languages, text is laid out right to left and
	// columns resize to the left
	if ((nAlign != GDI_ERROR) && (nAlign & TA_RTLREADING))
	{
		return true;
	}

	return false;
}

int CXTPHeaderCtrl::GetFrozenColWidth()
{
	int iFrozenWidth = 0;

	for (POSITION pos = m_arFrozenCols.GetHeadPosition(); pos; m_arFrozenCols.GetNext(pos))
	{
		int iCol = m_arFrozenCols.GetAt(pos);

		CRect r;
		Header_GetItemRect(m_hWnd, iCol, &r);
		iFrozenWidth += r.Width();
	}

	return iFrozenWidth;
}

void CXTPHeaderCtrl::SetMinSize(int iMinSize)
{
	m_iMinSize = iMinSize;
}

void CXTPHeaderCtrl::OnPaint()
{
	CPaintDC dc(this);

	if (IsThemeValid())
	{
		CXTPBufferDC memDC(dc);
		m_pTheme->DrawHeader(&memDC, this);
	}
	else
	{
		CHeaderCtrl::DefWindowProc(WM_PAINT, (WPARAM)dc.m_hDC, 0);
	}
}

LRESULT CXTPHeaderCtrl::OnPrintClient(WPARAM wParam, LPARAM /*lParam*/)
{
	CDC* pDC = CDC::FromHandle((HDC)wParam);
	if (pDC && IsThemeValid())
	{
		m_pTheme->DrawHeader(pDC, this);
		return TRUE;
	}

	return Default();
}

BOOL CXTPHeaderCtrl::OnEraseBkgnd(CDC* /*pDC*/)
{
	return TRUE;
}

void CXTPHeaderCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_bLBtnDown = TRUE;
	CHeaderCtrl::OnLButtonDown(nFlags, point);
}

void CXTPHeaderCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_bLBtnDown = FALSE;
	CHeaderCtrl::OnLButtonUp(nFlags, point);
}

void CXTPHeaderCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	if (IsThemeValid() && m_pTheme->UseWinXPThemes(this))
	{
		SetTimer(1, 10, NULL);
	}

	CHeaderCtrl::OnMouseMove(nFlags, point);
}

int CXTPHeaderCtrl::HitTest(CPoint pt, UINT* pFlags/*=NULL*/) const
{
	HDHITTESTINFO hti;
	hti.pt.x = pt.x;
	hti.pt.y = pt.y;

	int iItem = (int)::SendMessage(m_hWnd, HDM_HITTEST, 0, (LPARAM)(&hti));

	if (pFlags != NULL)
		*pFlags = hti.flags;

	return iItem;
}

BOOL CXTPHeaderCtrl::ItemPressed(int iItem)
{
	if (m_bLBtnDown)
	{
		CPoint point;
		::GetCursorPos(&point);
		ScreenToClient(&point);

		UINT uFlags;
		if (HitTest(point, &uFlags) == iItem)
			return ((uFlags & HHT_ONHEADER) == HHT_ONHEADER);
	}

	return FALSE;
}

void CXTPHeaderCtrl::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 1)
	{
		CPoint pt;
		::GetCursorPos(&pt);
		ScreenToClient(&pt);

		int iOverIndex = HitTest(pt);
		if (iOverIndex == -1)
		{
			KillTimer(1);

			if (m_bPainted == TRUE)
			{
				RedrawWindow();
			}

			m_bPainted = false;
			return;
		}

		if (iOverIndex != m_iOverIndex)
		{
			m_iOverIndex = iOverIndex;
			m_bPainted = false;
		}

		if (!m_bPainted)
		{
			RedrawWindow();
			m_bPainted = true;
		}
	}

	CHeaderCtrl::OnTimer(nIDEvent);
}

LRESULT CXTPHeaderCtrl::OnLayout(WPARAM wParam, LPARAM lParam)
{
	if (IsThemeValid())
	{
		return (LRESULT)m_pTheme->Layout(
			(LPHDLAYOUT)lParam, this);
	}

	return CHeaderCtrl::DefWindowProc(HDM_LAYOUT, wParam, lParam);
}

int CXTPHeaderCtrl::SetSortImage(int iSortCol, BOOL bSortAsc)
{
	ASSERT(iSortCol < GetItemCount());

	int nPrevCol = m_nSortedCol;
	m_nSortedCol = iSortCol;
	m_bAscending = bSortAsc;

	RedrawWindow();
	return nPrevCol;
}

int CXTPHeaderCtrl::GetSortedCol(BOOL* pbSortAsc/*=NULL*/)
{
	if (pbSortAsc)
		*pbSortAsc = m_bAscending;

	return m_nSortedCol;
}

BOOL CXTPHeaderCtrl::RecalcLayout()
{
	if (!::IsWindow(m_hWnd))
		return FALSE;

	HD_LAYOUT hdl;

	CXTPClientRect rcClient(this);
	hdl.prc = &rcClient;

	WINDOWPOS wp;
	ZeroMemory(&wp, sizeof(WINDOWPOS));
	hdl.pwpos = &wp;

	if (!Header_Layout(m_hWnd, &hdl))
		return FALSE;

	// Set the size, position, and visibility of the header window.
	::SetWindowPos(m_hWnd, wp.hwndInsertAfter, wp.x, wp.y,
		wp.cx, wp.cy, wp.flags | SWP_FRAMECHANGED);

	CWnd* pWndParent = GetParent();
	if (!::IsWindow(pWndParent->GetSafeHwnd()))
		return FALSE;

	// Force list control to recalculate it's layout.
	CXTPWindowRect rcWindow(pWndParent);

	const int cx = rcWindow.Width();
	const int cy = rcWindow.Height();

	pWndParent->SetWindowPos(NULL, 0, 0, cx, cy+1,
		SWP_NOMOVE | SWP_FRAMECHANGED);

	pWndParent->SetWindowPos(NULL, 0, 0, cx, cy,
		SWP_NOMOVE | SWP_FRAMECHANGED);

	return TRUE;
}

void CXTPHeaderCtrl::OnDestroy()
{
	if (IsThemeValid())
		m_pTheme->CleanUp(this);

	CHeaderCtrl::OnDestroy();
}

void CXTPHeaderCtrl::SetBitmap(int iCol, UINT uBitmapID, DWORD dwRemove/*=NULL*/)
{
	if (dwRemove)
	{
		HD_ITEM hdi;
		hdi.mask = HDI_FORMAT;
		GetItem(iCol, &hdi);
		hdi.fmt &= ~dwRemove;
		SetItem(iCol, &hdi);
	}

	SetBitmap(iCol, uBitmapID, FALSE, RGB(192, 192, 192));
}

void CXTPHeaderCtrl::SetBitmap(int iCol, UINT uBitmapID, BOOL bRemove, COLORREF crMask)
{
	if (IsThemeValid())
	{
		m_pTheme->SetBitmap(iCol, uBitmapID, bRemove, crMask, this);
	}
}

void CXTPHeaderCtrl::InitializeHeader(BOOL bBoldFont)
{
	SetFont(bBoldFont ? &XTPAuxData().fontBold : &XTPAuxData().font);
	RedrawWindow();
}

BOOL CXTPHeaderCtrl::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	if (message == WM_SETTINGCHANGE || message == WM_SYSCOLORCHANGE)
	{
		RefreshMetrics();
	}

	return CHeaderCtrl::OnWndMsg(message, wParam, lParam, pResult);
}

void CXTPHeaderCtrl::OnRButtonDown(UINT nFlags, CPoint point)
{
	CPoint pt = m_pt = point;
	ClientToScreen(&pt);

	// If no sort headers are defined for the parent control or popup menus
	// has been disabled, call the base class and return.
	CWnd* pParentWnd = GetParent();
	if (pParentWnd->GetStyle() & LVS_NOSORTHEADER || m_bEnableMenus == FALSE)
	{
		CHeaderCtrl::OnRButtonDown(nFlags, point);
		return;
	}

	// No menu was defined use default
	if (!m_popupMenuID)
	{
		// Get the index to the header item under the cursor.
		int iIndex = HitTest(m_pt);

		if (iIndex != -1)
		{
			CMenu menu;
			CXTPResourceManager::AssertValid(XTPResourceManager()->LoadMenu(&menu, XTP_IDM_POPUP));

			CMenu* pPopup = menu.GetSubMenu(2);
			ASSERT(pPopup != NULL);
			if (!pPopup)
				return;

			if (m_nSortedCol == iIndex && m_bAscending == TRUE)
				pPopup->CheckMenuItem(XTP_IDC_SORTASC, MF_CHECKED | MF_BYCOMMAND);

			else if (m_nSortedCol == iIndex && m_bAscending == FALSE)
				pPopup->CheckMenuItem(XTP_IDC_SORTDSC, MF_CHECKED | MF_BYCOMMAND);

			if (pParentWnd && (
				pParentWnd->IsKindOf(RUNTIME_CLASS(CListCtrl)) ||
				pParentWnd->IsKindOf(RUNTIME_CLASS(CListView))))
			{
				LVCOLUMN lvc;
				lvc.mask = LVCF_FMT;

				ListView_GetColumn(pParentWnd->m_hWnd, iIndex, &lvc);

				switch (lvc.fmt & LVCFMT_JUSTIFYMASK)
				{
				case LVCFMT_LEFT:
					pPopup->CheckMenuItem(XTP_IDC_ALIGNLEFT, MF_CHECKED | MF_BYCOMMAND);
					break;
				case LVCFMT_CENTER:
					pPopup->CheckMenuItem(XTP_IDC_ALIGNCENTER, MF_CHECKED | MF_BYCOMMAND);
					break;
				case LVCFMT_RIGHT:
					pPopup->CheckMenuItem(XTP_IDC_ALIGNRIGHT, MF_CHECKED | MF_BYCOMMAND);
					break;
				}
			}

			XTPContextMenu(pPopup, TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, this, XTP_IDR_TBAR_HDR);
		}
	}
	else
	{
		CMenu menu;
		VERIFY(menu.LoadMenu(m_popupMenuID));

		CMenu* pPopup = menu.GetSubMenu(m_nPos);
		ASSERT(pPopup != NULL);
		if (!pPopup)
			return;

		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,
			pt.x, pt.y, GetOwner());
	}
}

void CXTPHeaderCtrl::SetMenuID(UINT popupMenuID, int nPos)
{
	m_popupMenuID = popupMenuID;
	m_nPos = nPos;
}

void CXTPHeaderCtrl::SendNotify(int iIndex)
{
	CWnd* pParentWnd = GetParent();
	if (!pParentWnd || !::IsWindow(pParentWnd->m_hWnd))
		return;

	if (pParentWnd->IsKindOf(RUNTIME_CLASS(CListCtrl)) ||
		pParentWnd->IsKindOf(RUNTIME_CLASS(CListView)))
	{
		TCHAR  lpBuffer[256];

		HDITEM hdi;
		hdi.mask = HDI_TEXT | HDI_BITMAP | HDI_FORMAT | HDI_IMAGE | HDI_LPARAM | HDI_ORDER | HDI_WIDTH;
		hdi.pszText = lpBuffer;
		hdi.cchTextMax = 256;

		GetItem(iIndex, &hdi);

		NMHEADER nmh;
		nmh.hdr.hwndFrom = m_hWnd;
		nmh.hdr.idFrom = GetDlgCtrlID();
		nmh.hdr.code = HDN_ITEMCLICK;
		nmh.iItem = iIndex;
		nmh.iButton = 1;
		nmh.pitem = &hdi;

		// send message to the parent's owner window.
		pParentWnd->SendMessage(WM_NOTIFY,
			(WPARAM)(int)nmh.hdr.idFrom, (LPARAM)(NMHEADER*)&nmh);

		// then forward to the descendants.
		pParentWnd->SendMessageToDescendants(WM_NOTIFY,
			(WPARAM)(int)nmh.hdr.idFrom, (LPARAM)(NMHEADER*)&nmh);
	}
}

void CXTPHeaderCtrl::OnSortAsc()
{
	int iIndex = HitTest(m_pt);
	if (iIndex != -1)
	{
		if (m_nSortedCol != iIndex || m_bAscending == FALSE)
		{
			m_bAscending = TRUE;
			m_nSortedCol = iIndex;
			SendNotify(iIndex);
		}
	}
}

void CXTPHeaderCtrl::OnSortDsc()
{
	int iIndex = HitTest(m_pt);
	if (iIndex != -1)
	{
		if (m_nSortedCol != iIndex || m_bAscending == TRUE)
		{
			m_bAscending = FALSE;
			m_nSortedCol = iIndex;
			SendNotify(iIndex);
		}
	}
}

BOOL CXTPHeaderCtrl::SetAlingment(int nFlag)
{
	int iIndex = HitTest(m_pt);

	if (iIndex != -1)
	{
		CWnd* pParentWnd = GetParent();
		if (pParentWnd && (
			pParentWnd->IsKindOf(RUNTIME_CLASS(CListCtrl)) ||
			pParentWnd->IsKindOf(RUNTIME_CLASS(CListView))))
		{
			LVCOLUMN lvc;
			lvc.mask = LVCF_FMT;

			ListView_GetColumn(pParentWnd->m_hWnd, iIndex, &lvc);
			lvc.fmt &= ~LVCFMT_JUSTIFYMASK;
			lvc.fmt |= nFlag;
			ListView_SetColumn(pParentWnd->m_hWnd, iIndex, &lvc);
			ListView_SetWorkAreas(pParentWnd->m_hWnd, 0, NULL);
		}
	}

	return FALSE;
}

void CXTPHeaderCtrl::OnAlignLeft()
{
	SetAlingment(LVCFMT_LEFT);
}

void CXTPHeaderCtrl::OnAlignCenter()
{
	SetAlingment(LVCFMT_CENTER);
}

void CXTPHeaderCtrl::OnAlignRight()
{
	SetAlingment(LVCFMT_RIGHT);
}

BOOL CXTPHeaderCtrl::HasSortArrow()
{
	if (IsThemeValid())
		return m_pTheme->HasSortArrow();
	return FALSE;
}

void CXTPHeaderCtrl::ShowSortArrow(BOOL bSortArrow)
{
	if (IsThemeValid())
	{
		DWORD dwStyle = m_pTheme->GetDrawStyle() & ~HDR_XTP_SORTARROW;

		if (bSortArrow)
			dwStyle |= HDR_XTP_SORTARROW;

		m_pTheme->SetDrawStyle(dwStyle, this);
	}
}

void CXTPHeaderCtrl::SetFontBold(BOOL bBoldFont)
{
	ASSERT(::IsWindow(m_hWnd));
	SetFont(bBoldFont ? &XTPAuxData().fontBold : &XTPAuxData().font);
}

void CXTPHeaderCtrl::RefreshMetrics()
{
	if (m_pTheme)
		m_pTheme->RefreshMetrics(this);

	if (::IsWindow(m_hWnd))
		RedrawWindow();
}

BOOL CXTPHeaderCtrl::SetTheme(CXTPHeaderCtrlTheme* pTheme)
{
	CMDTARGET_RELEASE(m_pTheme);

	m_pTheme = pTheme;

	RefreshMetrics();

	return (m_pTheme != NULL);
}

BOOL CXTPHeaderCtrl::SetTheme(XTPControlTheme eTheme)
{
	CMDTARGET_RELEASE(m_pTheme);

	switch (eTheme)
	{
	case xtpControlThemeFlat:
	case xtpControlThemeUltraFlat:
	case xtpControlThemeOffice2000:
	case xtpControlThemeOfficeXP:
		m_pTheme = new CXTPHeaderCtrlThemeOfficeXP;
		break;

	case xtpControlThemeOffice2003:
		m_pTheme = new CXTPHeaderCtrlThemeOffice2003;
		break;

	case xtpControlThemeVisualStudio2005:
	case xtpControlThemeVisualStudio2008:
	case xtpControlThemeVisualStudio2010:
	case xtpControlThemeNativeWinXP:
		m_pTheme = new CXTPHeaderCtrlThemeExplorer;
		m_pTheme->SetDrawStyle(HDR_XTP_WINDEF|HDR_XTP_HOTTRACKING|HDR_XTP_SORTARROW, this);
		break;

	case xtpControlThemeResource:
		m_pTheme = new CXTPHeaderCtrlThemeResource;
		break;

	case xtpControlThemeDefault:
	default:
		m_pTheme = new CXTPHeaderCtrlTheme;
		break;
	}

	RefreshMetrics();

	return (m_pTheme != NULL);
}

LRESULT CXTPHeaderCtrl::OnSetTheme(WPARAM wParam, LPARAM /*lParam*/)
{
	XTPControlTheme eTheme = (XTPControlTheme)wParam;

	SetTheme(eTheme);

	return 0;
}
