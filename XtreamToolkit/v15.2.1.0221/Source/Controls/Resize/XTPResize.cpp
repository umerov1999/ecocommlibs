// XTPResize.cpp: implementation of the CXTPResize class.
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

#include "Common/XTPDrawHelpers.h"

#include "XTPResizeRect.h"
#include "XTPResizePoint.h"
#include "XTPResize.h"
#include "XTPResizeGroupBox.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

#ifndef OBM_SIZE
#define OBM_SIZE 32766
#endif

#define ENTRY_WINDOWPLACEMENT _T("WindowPlacement")

#define IDC_SIZEICON 0x7FFF

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXTPResize::CXTPResize(CWnd* pWnd, const UINT nFlags) :
	m_pWnd(pWnd),
	m_nFlagsXX(nFlags),
	m_szMin(0, 0),
	m_szMax(0, 0),
	m_szWindow(0, 0),
	m_szInitWindow(0, 0),
	m_strSection(_T(""))
{

}

CXTPResize::~CXTPResize()
{
	RemoveAllControls();
}

//////////////////////////////////////////////////////////////////////

BOOL CXTPResize::RemoveResize(const UINT nID)
{
	// search for an item with the given id

	for (int iItem = (int)m_arrItems.GetSize() - 1; iItem >= 0; iItem--)
	{
		CWnd* pWnd = m_arrItems[iItem]->m_pWnd;
		ASSERT_VALID(pWnd);

		if (pWnd->GetDlgCtrlID() == (int) nID)
		{
			CXTPResizeItem* pItem = m_arrItems[iItem];
			if (pItem != NULL)
			{
				m_arrItems.RemoveAt(iItem);
				SAFE_DELETE(pItem);
				return TRUE;
			}
		}
	}

	return FALSE;
}

void CXTPResize::RemoveAllControls()
{
	// free allocated memory
	for (int iIndex = 0; iIndex < m_arrItems.GetSize(); ++iIndex)
	{
		CXTPResizeItem* pItem = m_arrItems.GetAt(iIndex);
		SAFE_DELETE(pItem);
	}

	// empty array
	m_arrItems.RemoveAll();
}

void CXTPResize::SetResize(const UINT nID, const HWND hWnd, const XTP_RESIZERECT& rrcSizing)
{
	CXTPResizeItem* pItem = 0;

	// search for an item with the given id
	for (int i = (int)m_arrItems.GetSize() - 1; i >= 0; i--)
	{
		CWnd* pWnd = m_arrItems[i]->m_pWnd;
		if (pWnd && ::IsWindow(pWnd->m_hWnd) && (pWnd->GetDlgCtrlID() == (int)nID))
		{
			pItem = m_arrItems[i];
		}
	}

	// if we didn't find it then create one
	if (pItem == 0)
	{
		HWND hWndChild = hWnd;
		if (hWndChild == NULL)
		{
			CWnd* pWnd = m_pWnd->GetDlgItem(nID);
			if (pWnd && ::IsWindow(pWnd->m_hWnd))
			{
				hWndChild = pWnd->m_hWnd;
			}
		}

		bool bDelete = false;

		CWnd* pWnd = m_pWnd->FromHandlePermanent(hWndChild);
		if (pWnd == NULL)
		{
			// there is no permanent window, create one
			pWnd = new CWnd; // CXTPResizeItem handles the delete
			pWnd->Attach(hWndChild);

			bDelete = true;
		}

		CRect rcWindow;
		pWnd->GetWindowRect(rcWindow);
		m_pWnd->ScreenToClient(rcWindow);

		pItem = new CXTPResizeItem(pWnd, rrcSizing, rcWindow, bDelete);
		pItem->MakeTransparent(this);

		m_arrItems.Add(pItem);
	}
	else
	{
		// the item already exists, just add the new sizing option
		pItem->m_rrcSizing += rrcSizing;
	}

	// we should only allow sizing within the rect {0, 0, 1, 1}
	ASSERT((CXTPResizeRect(0, 0, 1, 1) & pItem->m_rrcSizing) == pItem->m_rrcSizing);
}

void CXTPResize::Init()
{
	if (m_pWnd == NULL)
		return;

	// clear the control list when initializing.  The reason for this is we may have a
	// static window that calls Init() multiple times for example CDialog::DoModal().
	RemoveAllControls();

	// get the dialog size
	CRect rcWindow;
	m_pWnd->GetClientRect(rcWindow);
	m_szWindow = m_szInitWindow = rcWindow.Size();

	if (!HasFlag(xtpResizeNoMinsize))
	{
		// set minimum size to current window size
		m_szMin = m_szWindow;
	}

	// set the clip children style to prevent flickering
	if (!HasFlag(xtpResizeNoClipChildren))
	{
		m_pWnd->ModifyStyle(0, WS_CLIPCHILDREN);
	}

	// add the size icon
	if (!HasFlag(xtpResizeNoSizeIcon))
	{
		// Create the size icon if it already doesn't exist.
		if (!::IsWindow(m_scSizeIcon.m_hWnd))
		{
			int cxGrip = GetSystemMetrics(SM_CXVSCROLL);
			int cyGrip = GetSystemMetrics(SM_CYHSCROLL);

			CRect rcIcon(rcWindow);
			rcIcon.OffsetRect(-1, -1);
			rcIcon.left = rcIcon.right - cxGrip;
			rcIcon.top = rcIcon.bottom - cyGrip;

			m_scSizeIcon.Create(WS_CHILD | WS_VISIBLE | SBS_SIZEGRIP,
				rcIcon, m_pWnd, IDC_SIZEICON);
		}

		m_scSizeIcon.SetWindowPos(&CWnd::wndTop, 0, 0, 0, 0,
			SWP_NOSIZE | SWP_NOMOVE | SWP_FRAMECHANGED);

		SetResize(IDC_SIZEICON, XTP_ATTR_REPOS(1));
	}
}

void CXTPResize::Offset(CPoint ptOffset)
{
	if (m_arrItems.GetSize() > 0)
	{

		HDWP hDWP = ::BeginDeferWindowPos((int)m_arrItems.GetSize());

		int i;
		for (i = 0; i < (int)m_arrItems.GetSize(); i++)
		{
			CXTPResizeRect rrcSizing = m_arrItems[i]->m_rrcSizing;
			m_arrItems[i]->m_rrcSizing = CRect(ptOffset.x, ptOffset.y, ptOffset.x, ptOffset.y);
			m_arrItems[i]->m_bInitialSize = TRUE;

			Defer(hDWP, m_arrItems[i], 1, 1);

			m_arrItems[i]->m_rrcSizing = rrcSizing;
		}

		::EndDeferWindowPos(hDWP);

		// refresh group box items.
		for (i = 0; i < m_arrItems.GetSize(); i++)
		{
			if (m_arrItems[i]->m_bIsGroupBox)
				m_arrItems[i]->m_pWnd->Invalidate();
		}
	}
}

void CXTPResize::AdjustResizeRect(CSize&)
{
}

void CXTPResize::Size()
{
	if (m_arrItems.GetSize() > 0)
	{
		CSize szWindow = CXTPClientRect(m_pWnd).Size();
		if (szWindow.cx == 0 || szWindow.cy == 0)
			return;

		AdjustResizeRect(szWindow);

		CSize szDelta = szWindow - m_szWindow;

		HDWP hDWP = ::BeginDeferWindowPos((int)m_arrItems.GetSize());

		int i;
		for (i = 0; i < (int)m_arrItems.GetSize(); i++)
		{
			Defer(hDWP, m_arrItems[i], szDelta.cx, szDelta.cy);
		}

		::EndDeferWindowPos(hDWP);

		// refresh group box items.
		for (i = 0; i < m_arrItems.GetSize(); i++)
		{
			if (m_arrItems[i]->m_bIsGroupBox)
				m_arrItems[i]->m_pWnd->Invalidate();
		}

		m_szWindow = szWindow;
	}
}

void CXTPResize::Reset()
{
	int iSize = (int)m_arrItems.GetSize();
	int i;
	for (i = 0; i < iSize; i++)
	{
		CXTPResizeItem* pItem = m_arrItems[i];
		pItem->m_bInitialSize = FALSE;
		pItem->m_rrcWindow = pItem->m_rrcInitWindow;
	}
}

BOOL CXTPResize::Defer(HDWP& hDWP, CXTPResizeItem* pItem, int dx, int dy)
{
	HWND hWnd = pItem->m_pWnd->m_hWnd;
	if (::IsWindow(hWnd))
	{
		if (!pItem->m_bInitialSize)
		{
			CSize szWindow = CXTPClientRect(m_pWnd).Size();

			AdjustResizeRect(szWindow);

			dx = szWindow.cx - m_szInitWindow.cx;
			dy = szWindow.cy - m_szInitWindow.cy;

			pItem->m_bInitialSize = TRUE;
		}

		CXTPResizeRect rrcItem = pItem->m_rrcWindow;

		rrcItem.left += dx * pItem->m_rrcSizing.left;
		rrcItem.top += dy * pItem->m_rrcSizing.top;
		rrcItem.right += dx * pItem->m_rrcSizing.right;
		rrcItem.bottom += dy * pItem->m_rrcSizing.bottom;

		if (rrcItem != pItem->m_rrcWindow)
		{
			int x = (int) rrcItem.left;
			int y = (int) rrcItem.top;
			int cx = (int) rrcItem.Width();
			int cy = (int) rrcItem.Height();

			// Set positioning flags
			UINT uFlags = SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOOWNERZORDER;

			CRect rOld;
			CRect rNew = rrcItem;

			// get the size of the dialog item in client coordinates.
			pItem->m_pWnd->GetWindowRect(&rOld);
			m_pWnd->ScreenToClient(&rOld);

			// if the x-y coordinates have not changed, there is no reason
			// to move the dialog item.
			if (rNew.TopLeft() == rOld.TopLeft())
				uFlags |= SWP_NOMOVE;

			// if the cx-cy size has not changed, there is no reason to
			// size the dialog item.  If size has changed, don't
			// copy bits of the client area (i.e. have them invalidated/redrawn)
			if (rNew.Size() == rOld.Size())
				uFlags |= SWP_NOSIZE;
			else
				uFlags |= SWP_NOCOPYBITS;

			hDWP = ::DeferWindowPos(hDWP, hWnd, 0, x, y, cx, cy, uFlags);
			if (hDWP == NULL)
			{
				TRACE(_T("DeferWindowPos failed for ID %i\n"), GetDlgCtrlID(hWnd));
				return FALSE;
			}
			pItem->m_rrcWindow = rrcItem;
		}
	}
	return TRUE;
}



void CXTPResize::GetMinMaxInfo(MINMAXINFO* pMMI)
{
	CXTPClientRect rcClient(m_pWnd);
	CXTPWindowRect rcWindow(m_pWnd);
	if (rcClient.IsRectEmpty())
		return;

	CSize szBorder(rcWindow.Width() - rcClient.Width(), rcWindow.Height() - rcClient.Height());

	if (m_szWindow != CSize(0, 0))
	{
		if (HasFlag(xtpResizeNoHorizontal))
		{
			pMMI->ptMaxTrackSize.x = pMMI->ptMaxSize.x = m_szWindow.cx + szBorder.cx;
		}
		if (HasFlag(xtpResizeNoVertical))
		{
			pMMI->ptMaxTrackSize.y = pMMI->ptMaxSize.y = m_szWindow.cy + szBorder.cy;
		}
	}
	if (m_szMin.cx != 0) pMMI->ptMinTrackSize.x = m_szMin.cx + szBorder.cx;
	if (m_szMin.cy != 0) pMMI->ptMinTrackSize.y = m_szMin.cy + szBorder.cy;
	if (m_szMax.cx != 0) pMMI->ptMaxTrackSize.x = m_szMax.cx + szBorder.cx;
	if (m_szMax.cy != 0) pMMI->ptMaxTrackSize.y = m_szMax.cy + szBorder.cy;
}

BOOL CXTPResize::AutoLoadPlacement(LPCTSTR pszSection)
{
	m_strSection = pszSection;
	ASSERT(!m_strSection.IsEmpty());
	return LoadPlacement(m_strSection);
}

BOOL CXTPResize::LoadPlacement(LPCTSTR pszSection)
{
	BOOL bResult = FALSE;
	UINT nBytes = 0;
	BYTE* pBytes = 0;
	AfxGetApp()->GetProfileBinary(pszSection, ENTRY_WINDOWPLACEMENT, &pBytes, &nBytes);
	if (nBytes == sizeof(WINDOWPLACEMENT))
	{
		bResult = m_pWnd->SetWindowPlacement((WINDOWPLACEMENT*) pBytes);
	}
	if (pBytes && nBytes) delete[] pBytes;

	return bResult;
}

BOOL CXTPResize::SavePlacement(LPCTSTR pszSection)
{
	WINDOWPLACEMENT wp;
	if (!m_pWnd->GetWindowPlacement(&wp))
		return FALSE;

	AfxGetApp()->WriteProfileBinary(pszSection, ENTRY_WINDOWPLACEMENT, (BYTE*) &wp, sizeof(wp));
	return TRUE;
}

void CXTPResize::SetFlag(XTPResize eFlag)
{
	m_nFlagsXX &= (eFlag^0xFFFFFFFF);
	m_nFlagsXX |= eFlag;
}

void CXTPResize::SetResize(CWnd* pWnd, const CXTPResizeRect& rrcSizing, CRect rcWindow)
{
	CXTPResizeItem *pItem = new CXTPResizeItem(pWnd, rrcSizing, rcWindow, FALSE);
	m_arrItems.Add(pItem);
}

void CXTPResize::UpdateControlRect(CWnd* pWnd)
{
	if (!pWnd)
		return;

	for (int i = (int)m_arrItems.GetSize(); i--;)
	{
		if (m_arrItems[i]->m_pWnd->m_hWnd == pWnd->m_hWnd)
		{
			CRect rect;
			pWnd->GetWindowRect(&rect);
			m_pWnd->ScreenToClient(&rect);
			m_arrItems[i]->m_rrcWindow = rect;
			break;
		}
	}
}

//////////////////////////////////////////////////////////////////////
// CXTPResizeItem
//////////////////////////////////////////////////////////////////////

CXTPResizeItem::CXTPResizeItem(CWnd* pWnd, const CXTPResizeRect& rrcSizing, CRect& rcWindow, BOOL bAutoDelete)
: m_pWnd(pWnd)
, m_rrcSizing(rrcSizing)
, m_rrcWindow(rcWindow)
, m_rrcInitWindow(rcWindow)
, m_bAutoDelete(bAutoDelete)
, m_bInitialSize(FALSE)
, m_bIsGroupBox(FALSE)
{

}

CXTPResizeItem::~CXTPResizeItem()
{
	if (m_bAutoDelete)
	{
		m_pWnd->Detach();
		SAFE_DELETE(m_pWnd);
	}
}

bool CXTPResizeItem::MakeTransparent(CXTPResize* pXTPResize)
{
	if (pXTPResize->HasFlag(xtpResizeNoTransparentGroup))
	{
		return false;
	}

	if (m_pWnd && ::IsWindow(m_pWnd->m_hWnd))
	{
		TCHAR szClassName[8];
		::GetClassName(m_pWnd->m_hWnd, szClassName, 8);

		// not a CButton derived class.
		if (_tcsicmp(szClassName, _T("Button")) != 0)
		{
			return false;
		}

		// get the window style if not a group box, return.
		DWORD dwStyle = ::GetWindowLong(m_pWnd->m_hWnd, GWL_STYLE);
		if ((dwStyle & (BS_GROUPBOX | WS_TABSTOP)) == BS_GROUPBOX)
		{
			// we don't want CXTPResizeGroupBox transparent.
			if (m_pWnd->IsKindOf(RUNTIME_CLASS(CXTPResizeGroupBox)))
			{
				return false;
			}

			// Get the extended style for the group box.
			DWORD dwStyleEx = ::GetWindowLong(m_pWnd->m_hWnd, GWL_EXSTYLE);

			// add the WS_EX_TRANSPARENT flag to the group box.
			::SetWindowLong(m_pWnd->m_hWnd, GWL_EXSTYLE,
				dwStyleEx | WS_EX_TRANSPARENT);

			// apply the style for the window.
			::SetWindowPos(m_pWnd->m_hWnd, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE |
				SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);

			m_bIsGroupBox = TRUE;

			return true;
		}
	}

	return false;
}

//////////////////////////////////////////////////////////////////////
// CXTPSizeIcon
//////////////////////////////////////////////////////////////////////

CXTPSizeIcon::CXTPSizeIcon()
{
	// Load the size cursor
	m_hCursor = AfxGetApp()->LoadStandardCursor(IDC_SIZENWSE);
}

CXTPSizeIcon::~CXTPSizeIcon()
{
}

BEGIN_MESSAGE_MAP(CXTPSizeIcon, CScrollBar)
	//{{AFX_MSG_MAP(CXTPSizeIcon)
	ON_WM_SETCURSOR()
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTPSizeIcon message handlers

void CXTPSizeIcon::OnSetFocus(CWnd* /*pOldWnd*/)
{

}

BOOL CXTPSizeIcon::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// Set the cursor to the size cursor.
	if (nHitTest == HTCLIENT && m_hCursor != NULL)
	{
		::SetCursor(m_hCursor);
		return TRUE;
	}

	return CScrollBar::OnSetCursor(pWnd, nHitTest, message);
}
