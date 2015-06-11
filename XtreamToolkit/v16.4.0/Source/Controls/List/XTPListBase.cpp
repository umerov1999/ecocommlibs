// XTPListCtrl.cpp : implementation of the CXTPListCtrl class.
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

#include "Common/XTPDrawHelpers.h"
#include "../Util/XTPControlTheme.h"

#include "../Util/XTPGlobal.h"
#include "../Header/XTPHeaderCtrl.h"
#include "XTPListBase.h"
#include "../Util/XTPRegistryManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CXTPListBase::CXTPListBase()
: m_bAutoFont(false)
, m_bAutoSave(false)
, m_bRowColor(false)
, m_bListColor(false)
, m_bSortColor(false)
, m_bAscending(true)
, m_bNoColSizing(false)
, m_nMinColWidth(0)
, m_nMaxColWidth(0)
, m_nSortedCol(-1)
, m_crListText((COLORREF)-1)
, m_crListBack((COLORREF)-1)
, m_crSortText((COLORREF)-1)
, m_crSortBack((COLORREF)-1)
{
	RefreshMetrics();
}

CXTPListBase::~CXTPListBase()
{
}

void CXTPListBase::RefreshMetrics()
{
	// update column metrics.
	m_nMaxColWidth = ::GetSystemMetrics(SM_CXFULLSCREEN);

	// set sort background and text colors
	m_crSortText = GetListTextColor();
	m_crSortBack = CXTPDrawHelpers::DarkenColor(8,GetListBackColor());
}

CXTPListBase::ROWCOLOR* CXTPListBase::Lookup(int iRow)
{
	if (m_arRowColor.GetCount() == 0)
		return NULL;

	for (POSITION pos = m_arRowColor.GetHeadPosition(); pos;)
	{
		ROWCOLOR& rowColor = m_arRowColor.GetNext(pos);
		if (rowColor.iRow == iRow)
			return &rowColor;
	}
	return NULL;
}

void CXTPListBase::SetRowColor(int iRow, COLORREF crText, COLORREF crBack, BOOL bRedraw /*= TRUE*/)
{
	ROWCOLOR rcr;
	rcr.iRow   = iRow;
	rcr.crText = crText;
	rcr.crBack = crBack;
	rcr.lParam = 0;
	SetRowColor(&rcr, bRedraw);
}

void CXTPListBase::SetRowColor(ROWCOLOR* lpNewRowColor, BOOL bRedraw)
{
	ROWCOLOR* lpOldRowClr = Lookup(lpNewRowColor->iRow);
	if (lpOldRowClr)
	{
		lpOldRowClr->crText = lpNewRowColor->crText;
		lpOldRowClr->crBack = lpNewRowColor->crBack;
	}
	else
	{
		// initialize row color struct.
		ROWCOLOR rcr;
		rcr.iRow   = lpNewRowColor->iRow;
		rcr.crText = lpNewRowColor->crText;
		rcr.crBack = lpNewRowColor->crBack;
		rcr.lParam = lpNewRowColor->lParam;
		m_arRowColor.AddHead(rcr);
	}

	if (::IsWindow(m_pListCtrl->GetSafeHwnd()) && bRedraw)
	{
		// Redraw window if row is visible
		const int nTopIndex = m_pListCtrl->GetTopIndex();
		const int nBotIndex = nTopIndex + m_pListCtrl->GetCountPerPage();

		if (lpNewRowColor->iRow >= nTopIndex && lpNewRowColor->iRow <= nBotIndex)
		{
			m_pListCtrl->RedrawWindow();
		}
	}
}

void CXTPListBase::RemoveRowColors()
{
	m_arRowColor.RemoveAll();
}

void CXTPListBase::ImplAttach(CListCtrl* pListCtrl)
{
	m_pListCtrl = (CListCtrl_Friendly *)pListCtrl;
}

bool CXTPListBase::MoveRow(int nFrom, int nTo)
{
	// Can't move to the same place, or from or to a negative index
	if ((nFrom == nTo) || (nFrom < 0) || (nTo < 0))
	{
		return false;
	}

	// First Copy the row to the new location
	int iRemove = CopyRow(nFrom, nTo);
	if (iRemove != -1)
	{
		return (m_pListCtrl->DeleteItem(iRemove) == TRUE);
	}

	return false;
}

int CXTPListBase::CopyRow(int nFrom, int nTo)
{
	// Can't move to the same place, or from or to a negative index
	if ((nFrom == nTo) || (nFrom < 0) || (nTo < 0))
	{
		return -1;
	}

	//Copy the row to the new index
	TCHAR szLabel[ 256 ];

	LV_ITEM lvi;
	::ZeroMemory(&lvi, sizeof(LV_ITEM));

	lvi.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
	lvi.pszText = szLabel;
	lvi.iItem = nFrom;
	lvi.cchTextMax = _countof(szLabel);
	m_pListCtrl->GetItem(&lvi);

	lvi.iItem = nTo;
	m_pListCtrl->InsertItem(&lvi);

	// If row has been inserted before original
	// increment the original
	if (nFrom > nTo)
	{
		nFrom++;
	}

	//Loop through subitems
	int i;
	for (i = 1; i < GetColumnCount(); i++)
	{
		CString strLabel = m_pListCtrl->GetItemText(nFrom, i);
		if (!strLabel.IsEmpty())
		{
			m_pListCtrl->SetItemText(
				nTo, i, strLabel);
		}
	}

	return nFrom;
}

int CXTPListBase::GetColumnCount()
{
	// if we are not in report mode return error.
	if ((GetWindowLong(m_pListCtrl->m_hWnd, GWL_STYLE) & LVS_TYPEMASK) != LVS_REPORT)
		return -1;

	// get a pointer to the header control, if NULL return error.
	CHeaderCtrl* pHeaderCtrl = _xtGetHeaderCtrl();
	if (pHeaderCtrl == NULL)
		return -1;

	return pHeaderCtrl->GetItemCount();
}

BOOL CXTPListBase::ModifyExtendedStyle(DWORD dwRemove, DWORD dwAdd)
{
	ASSERT(m_pListCtrl != NULL && m_pListCtrl->m_hWnd != NULL);
	if (!m_pListCtrl)
		return FALSE;

	DWORD dwStyle = (DWORD)::SendMessage(m_pListCtrl->m_hWnd, LVM_GETEXTENDEDLISTVIEWSTYLE, 0, 0);
	DWORD dwNewStyle = (dwStyle & ~dwRemove) | dwAdd;

	if (dwStyle == dwNewStyle)
		return FALSE;

	::SendMessage(m_pListCtrl->m_hWnd, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, dwNewStyle);
	return TRUE;
}

void CXTPListBase::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMLVCUSTOMDRAW* lpLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>(pNMHDR);
	*pResult = CDRF_DODEFAULT;

	switch (lpLVCD->nmcd.dwDrawStage)
	{
	case CDDS_PREPAINT:
		*pResult = CDRF_NOTIFYITEMDRAW;
		break;

	case CDDS_PREPAINT | CDDS_ITEM:
		*pResult = CDRF_NOTIFYSUBITEMDRAW;
		break;

	case CDDS_PREPAINT | CDDS_ITEM | CDDS_SUBITEM:
		{
			ROWCOLOR* lpRowColor = Lookup((int)lpLVCD->nmcd.dwItemSpec);
			BOOL bUseSortColor = (m_bSortColor && (lpLVCD->iSubItem == m_nSortedCol));
			BOOL bUseRowColor = (m_bRowColor && lpRowColor);

			// if the item is selected use system default.
			if (bUseRowColor && m_pListCtrl->GetItemState(
				(int)lpLVCD->nmcd.dwItemSpec, LVIS_SELECTED) == LVIS_SELECTED)
			{
				bUseRowColor = FALSE;
			}

			// if the window is disabled use system default.
			if (!m_pListCtrl->IsWindowEnabled())
			{
				lpLVCD->clrText = ::GetSysColor(COLOR_WINDOWTEXT);
				lpLVCD->clrTextBk = ::GetSysColor(COLOR_3DFACE);
			}

			// user defined sort color.
			else if (bUseSortColor && !bUseRowColor)
			{
				lpLVCD->clrText = GetSortTextColor();
				lpLVCD->clrTextBk = GetSortBackColor();
			}

			// use user defined row color.
			else if (bUseRowColor)
			{
				lpLVCD->clrText = lpRowColor->crText;
				lpLVCD->clrTextBk = lpRowColor->crBack;

				if (bUseSortColor)
					lpLVCD->clrTextBk = GetSortBackColor();
			}

			// user defined list color.
			else if (m_bListColor)
			{
				lpLVCD->clrText = GetListTextColor();
				lpLVCD->clrTextBk = GetListBackColor();
			}

			// default color.
			else
			{
				lpLVCD->clrText = ::GetSysColor(COLOR_WINDOWTEXT);
				lpLVCD->clrTextBk = ::GetSysColor(COLOR_WINDOW);
			}

			break;
		}
	}
}

int CXTPListBase::SetSortImage(int nCol, BOOL bAsc)
{
	int iPrev = m_nSortedCol;

	m_nSortedCol = nCol;
	m_bAscending = (bAsc == TRUE);

	CXTPHeaderCtrl* pHeaderCtrl = DYNAMIC_DOWNCAST(CXTPHeaderCtrl, m_pListCtrl->GetDlgItem(0));
	if (pHeaderCtrl && ::IsWindow(pHeaderCtrl->m_hWnd))
	{
		return pHeaderCtrl->SetSortImage(nCol, bAsc);
	}

	return iPrev;
}

BOOL CXTPListBase::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	HD_NOTIFY *pHDNotify = (HD_NOTIFY*)lParam;

	switch (pHDNotify->hdr.code)
	{
	case HDN_ITEMCLICKA:
	case HDN_ITEMCLICKW:
		{
			CXTPHeaderCtrl* pHeaderCtrl = XTPGetHeaderCtrl();
			if (pHeaderCtrl)
			{
				// left mouse button.
				if (pHDNotify->iButton == 0)
				{
					if (pHDNotify->iItem == m_nSortedCol)
					{
						m_bAscending = !m_bAscending;
					}
					else
					{
						m_bAscending = true;
					}
				}

				// right mouse button.
				else if (pHDNotify->iButton == 1)
				{
					m_bAscending = pHeaderCtrl->GetAscending() ? true : false;
				}
				else
				{
					break;
				}

				// set sort image for header.
				m_nSortedCol = pHDNotify->iItem;
				pHeaderCtrl->SetSortImage (m_nSortedCol, m_bAscending);

				// sort list.
				SortList (m_nSortedCol, m_bAscending);
			}
		}
		break;

	case HDN_ITEMCHANGINGA:
	case HDN_ITEMCHANGINGW:
	case HDN_ENDTRACK:
		{
			if (pHDNotify->pitem->mask & HDI_WIDTH)
			{
				if (pHDNotify->pitem->cxy < m_nMinColWidth)
					pHDNotify->pitem->cxy = m_nMinColWidth;
			}
		}
		break;

	case HDN_DIVIDERDBLCLICKA:
	case HDN_DIVIDERDBLCLICKW:
	case HDN_BEGINTRACKW:
	case HDN_BEGINTRACKA:
		{
			if (!m_bNoColSizing)
				break;

			*pResult = TRUE;
			return TRUE;
		}
	}

	return m_pListCtrl->CListCtrl::OnNotify(wParam, lParam, pResult);
}

bool CXTPListBase::SortList(int nCol, bool bAscending)
{
#ifdef _DEBUG
	TRACE2("Column sort on column index %d, ascending=%d\n", nCol, bAscending);
#else
	UNREFERENCED_PARAMETER(nCol);
	UNREFERENCED_PARAMETER(bAscending);
#endif

	return true;
}

int CXTPListBase::AddColumn(LPCTSTR lpszColHeading, int nWidth/*= -1*/, int nFormat/*= LVCFMT_LEFT*/)
{
	// if we are not in report mode return error.
	if ((GetWindowLong(m_pListCtrl->m_hWnd, GWL_STYLE) & LVS_TYPEMASK) != LVS_REPORT)
		return -1;

	// get a pointer to the header control, if NULL return error.
	CHeaderCtrl* pHeaderCtrl = _xtGetHeaderCtrl();
	if (pHeaderCtrl == NULL)
		return -1;

	int iIndex = pHeaderCtrl->GetItemCount();
	if (nWidth == -1)
	{
		// Get the column width of the previous column from header control
		HD_ITEM hd_item;
		hd_item.mask = HDI_WIDTH;               //indicate that we want the width
		pHeaderCtrl->GetItem(iIndex - 1, &hd_item);
		nWidth = hd_item.cxy;
	}

	return m_pListCtrl->InsertColumn(iIndex, lpszColHeading, nFormat, nWidth, iIndex);
}

bool CXTPListBase::BuildColumns(int nCols, int* nWidth, int* nColString)
{
	int i;
	for (i = 0; i < nCols; ++i)
	{
		CString strColumn;
		if (!strColumn.LoadString(nColString[i]))
		{
		}

		if (m_pListCtrl->InsertColumn(i, strColumn, LVCFMT_LEFT, nWidth[i], i) == -1)
		{
			return false;
		}
	}

	return true;
}

bool CXTPListBase::BuildColumns(int nCols, int* nWidth, CString* strColString)
{
	int i;
	for (i = 0; i < nCols; ++i)
	{
		if (m_pListCtrl->InsertColumn(i, strColString[i], LVCFMT_LEFT, nWidth[i], i) == -1)
		{
			return false;
		}
	}

	return true;
}

int CXTPListBase::HitTestEx(CPoint& point, int* nCol) const
{
	int colnum = 0;
	int row = m_pListCtrl->HitTest(point, NULL);

	if (nCol) *nCol = 0;

	// Make sure that the ListView is in LVS_REPORT
	if ((GetWindowLong(m_pListCtrl->m_hWnd, GWL_STYLE) & LVS_TYPEMASK) != LVS_REPORT)
		return row;

	// Get the top and bottom row visible
	row = m_pListCtrl->GetTopIndex();
	int bottom = row + m_pListCtrl->GetCountPerPage();
	if (bottom > m_pListCtrl->GetItemCount())
		bottom = m_pListCtrl->GetItemCount();

	// get a pointer to the header control, if NULL return error.
	CHeaderCtrl* pHeaderCtrl = _xtGetHeaderCtrl();
	if (pHeaderCtrl == NULL)
		return -1;

	// Get the number of columns
	int nColumnCount = pHeaderCtrl->GetItemCount();

	// Loop through the visible rows
	for (; row <= bottom; row++)
	{
		// Get bounding rect of item and check whether point falls in it.
		CRect rect;
		m_pListCtrl->GetItemRect(row, &rect, LVIR_BOUNDS);
		if (rect.PtInRect(point))
		{
			// Now find the column
			for (colnum = 0; colnum < nColumnCount; colnum++)
			{
				int colwidth = m_pListCtrl->GetColumnWidth(Header_OrderToIndex(pHeaderCtrl->m_hWnd, colnum));
				if (point.x >= rect.left
					&& point.x <= (rect.left + colwidth))
				{
					if (nCol) *nCol = colnum;
					return row;
				}
				rect.left += colwidth;
			}
		}
	}
	return -1;
}

bool CXTPListBase::SubclassHeader(bool bBoldFont/*= false*/)
{
	// if we are not in report mode return false.
	if ((GetWindowLong(m_pListCtrl->m_hWnd, GWL_STYLE) & LVS_TYPEMASK) != LVS_REPORT)
		return false;

	// header was already subclassed!
	if (::IsWindow(m_wndHeader.GetSafeHwnd()))
		return false;

	// Get the windows handle to the header control for the
	// list control then subclass the control.
	HWND hWndHeader = _xtGetHeaderCtrl()->GetSafeHwnd();
	if (!m_wndHeader.SubclassWindow (hWndHeader))
		return false;

	// finish header initialization.
	m_wndHeader.InitializeHeader(bBoldFont);
	return true;
}

void CXTPListBase::AutoSaveColumns(LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszDefault)
{
	m_bAutoSave = true;

	// initialize registry strings.
	if (lpszSection == NULL)
		m_strSection = _T("Settings");
	else
		m_strSection = lpszSection;

	if (lpszEntry == NULL)
		m_strEntry = _T("Column Info");
	else
		m_strEntry = lpszEntry;

	if (lpszDefault != NULL)
		m_strDefault = lpszDefault;

	LoadColumnWidths();
}

void CXTPListBase::SaveColumnWidths()
{
	// if we are not in report mode return.
	if ((GetWindowLong(m_pListCtrl->m_hWnd, GWL_STYLE) & LVS_TYPEMASK) != LVS_REPORT)
		return;

	// get a pointer to the header control.
	CHeaderCtrl* pHeaderCtrl = _xtGetHeaderCtrl();
	if (pHeaderCtrl == NULL)
		return;

	CString strValue;
	int i;
	for (i = 0; i < pHeaderCtrl->GetItemCount(); ++i)
	{
		CString strTemp;
		strTemp.Format(_T("%d,"), m_pListCtrl->GetColumnWidth(Header_OrderToIndex(pHeaderCtrl->m_hWnd, i)));
		strValue += strTemp;
	}

	CXTPRegistryManager regManager;
	regManager.WriteProfileString(m_strSection, m_strEntry, strValue);
}

void CXTPListBase::LoadColumnWidths()
{
	// get the handle for the list control.
	HWND hWndList = m_pListCtrl->GetSafeHwnd();
	if (!::IsWindow(hWndList))
		return;

	// get the style for list control.
	DWORD dwStyle = ::GetWindowLong(hWndList, GWL_STYLE);

	// if we are not in report mode return.
	if ((dwStyle & LVS_TYPEMASK) != LVS_REPORT)
		return;

	// get a pointer to the header control.
	CHeaderCtrl* pHeaderCtrl = _xtGetHeaderCtrl();
	if (!::IsWindow(pHeaderCtrl->GetSafeHwnd()))
		return;

	// load the stored width for each column.
	for (int nCol = 0; nCol < pHeaderCtrl->GetItemCount(); ++nCol)
	{
		int nOrderCol = Header_OrderToIndex(pHeaderCtrl->m_hWnd, nCol);
		SetStoredWidth(nOrderCol);
	}
}

void CXTPListBase::SetStoredWidth(int nCol)
{
	int nWidth = GetStoredWidth(nCol);
	m_pListCtrl->SetColumnWidth(nCol, nWidth);
}

int CXTPListBase::GetStoredWidth(int nCol)
{
	// get the value from the registry.
	CXTPRegistryManager regManager;
	CString strValue = regManager.GetProfileString(m_strSection, m_strEntry, m_strDefault);

	if (!strValue.IsEmpty())
	{
		// extract the sub string to get the column width.
		CString strWidth;
		AfxExtractSubString(strWidth, strValue, nCol, _T(','));

		if (!strWidth.IsEmpty())
		{
			// return the width from the registry.
			return _ttoi(strWidth);
		}
	}

	// value was not found, return default.
	return m_pListCtrl->GetColumnWidth(nCol);
}

void CXTPListBase::OnDestroy()
{
	if (m_bAutoSave) SaveColumnWidths();
}

void CXTPListBase::AutoSizeColumn(int nCol/*= -1*/)
{
	// Call this after your list control is filled
	m_pListCtrl->SetRedraw (FALSE);

	int nMinCol = nCol < 0 ? 0 : nCol;
	int nMaxCol = nCol < 0 ? GetColumnCount()-1 : nCol;

	// get a pointer to the header control.
	CHeaderCtrl* pHeaderCtrl = _xtGetHeaderCtrl();
	if (!::IsWindow(pHeaderCtrl->GetSafeHwnd()))
		return;

	for (nCol = nMinCol; nCol <= nMaxCol; nCol++)
	{
		m_pListCtrl->SetColumnWidth(Header_OrderToIndex(pHeaderCtrl->m_hWnd, nCol), LVSCW_AUTOSIZE);
		int wc1 = m_pListCtrl->GetColumnWidth(Header_OrderToIndex(pHeaderCtrl->m_hWnd, nCol));
		int wc2 = 0;

		CWindowDC dc(NULL);
		CFont* pOF = dc.SelectObject(pHeaderCtrl->GetFont());

		TCHAR szColText[256];
		szColText[0] = _T('\0');

		LVCOLUMN lvc;
		lvc.mask = LVCF_TEXT;
		lvc.pszText = szColText;
		lvc.cchTextMax = _countof(szColText);

		ListView_GetColumn(m_pListCtrl->m_hWnd, nCol, &lvc);

		CSize size = dc.GetTextExtent(szColText);
		wc2 = size.cx;

		dc.SelectObject(pOF);

		wc2 += 12; // add padding.

		CXTPHeaderCtrl* pFlatHeaderCtrl = XTPGetHeaderCtrl();
		if (pFlatHeaderCtrl && pFlatHeaderCtrl->HasSortArrow())
		{
			if (m_nSortedCol == Header_OrderToIndex(pHeaderCtrl->m_hWnd, nCol))
				wc2 += 24; // add padding.
		}

		int wc = __max(m_nMinColWidth, __max(wc1, wc2));

		if (wc > m_nMaxColWidth)
			wc = m_nMaxColWidth;

		// set the column width.
		m_pListCtrl->SetColumnWidth(nCol, wc);
	}

	m_pListCtrl->SetRedraw();
	m_pListCtrl->Invalidate();
}

BOOL CXTPListBase::OnEraseBkgnd(CDC* /*pDC*/)
{
	return TRUE;
}

LRESULT CXTPListBase::OnPrintClient(WPARAM wParam, LPARAM lParam)
{
	m_pListCtrl->CWnd::DefWindowProc(WM_ERASEBKGND, wParam, 0);
	return m_pListCtrl->CWnd::DefWindowProc(WM_PRINTCLIENT, wParam, lParam);
}

void CXTPListBase::OnPaint()
{
	// background is already filled in gray
	CPaintDC dc(m_pListCtrl);

	bool bReportView = ((::GetWindowLong(m_pListCtrl->m_hWnd,
		GWL_STYLE) & LVS_TYPEMASK) == LVS_REPORT);

	// Get the client rect.
	CRect rectClient;
	m_pListCtrl->GetClientRect(&rectClient);

	// Exclude the header control from being re-painted.
	CHeaderCtrl* pHeaderCtrl = NULL;
	if (bReportView)
	{
		// Get a pointer to the header control.
		pHeaderCtrl = _xtGetHeaderCtrl();
		if (pHeaderCtrl && ::IsWindow(pHeaderCtrl->m_hWnd))
		{
			CRect rcHeader;
			pHeaderCtrl->GetWindowRect(&rcHeader);
			m_pListCtrl->ScreenToClient(&rcHeader);
			dc.ExcludeClipRect(&rcHeader);
		}
	}

	// define the background fill color.
	COLORREF crBackColor = GetListBackColor();

	if (!m_pListCtrl->IsWindowEnabled())
		crBackColor = ::GetSysColor(COLOR_3DFACE);

	// Paint to a memory device context to help
	// eliminate screen flicker.
	CXTPBufferDC memDC(dc);
	memDC.FillSolidRect(rectClient, crBackColor);

	if (pHeaderCtrl && ::IsWindow(pHeaderCtrl->m_hWnd))
	{
		CRect rcCol;
		m_pListCtrl->GetClientRect(&rcCol);

		SCROLLINFO si;
		::ZeroMemory(&si, sizeof(SCROLLINFO));
		si.cbSize = sizeof(SCROLLINFO);
		si.fMask = SIF_POS;

		m_pListCtrl->GetScrollInfo(SB_HORZ, &si);
		rcCol.left -= si.nPos;

		for (int iCol = 0; iCol <= pHeaderCtrl->GetItemCount(); iCol++)
		{
			int iColWidth = m_pListCtrl->GetColumnWidth(Header_OrderToIndex(pHeaderCtrl->m_hWnd, iCol));
			rcCol.right = rcCol.left + iColWidth;

			if (m_bSortColor && (Header_OrderToIndex(pHeaderCtrl->m_hWnd, iCol) == m_nSortedCol))
			{
				memDC.FillSolidRect(&rcCol, GetSortBackColor());
				break;
			}

			rcCol.left += iColWidth;
		}
	}

	// Now let the window do its default painting...
	m_pListCtrl->CWnd::DefWindowProc(WM_PAINT, (WPARAM)memDC.m_hDC, 0);

	// Refresh the header control.
	if (pHeaderCtrl && ::IsWindow(pHeaderCtrl->m_hWnd))
	{
		pHeaderCtrl->Invalidate();
		pHeaderCtrl->UpdateWindow();
	}
}

bool CXTPListBase::Init()
{
	if (!::IsWindow(m_pListCtrl->GetSafeHwnd()))
		return false;

	// NOTE: using auto font will also reset the font
	// for the header control.
	if (m_bAutoFont)
	{
		m_pListCtrl->SetFont(&XTPAuxData().font);
	}

	return true;
}

void CXTPListBase::OnSettingChange(UINT /*uFlags*/, LPCTSTR /*lpszSection*/)
{
	RefreshMetrics();
}

CHeaderCtrl* CXTPListBase::_xtGetHeaderCtrl() const
{
	if (m_pListCtrl == NULL)
		return NULL;

	CHeaderCtrl * const pHeaderCtrl =
		(CHeaderCtrl*)m_pListCtrl->GetDlgItem(0);

	// The follow code will ASSERT if you are trying to access the header
	// control prior to adding columns to the list control.  If you are
	// calling SubclassHeader(), you need to make sure that the call to
	// SubClassHeader() is placed after your columns have been inserted.

	ASSERT(pHeaderCtrl != NULL);
	return pHeaderCtrl;
}

CXTPHeaderCtrl* CXTPListBase::XTPGetHeaderCtrl() const
{
	CHeaderCtrl* pHeaderCtrl = _xtGetHeaderCtrl();

	if (!pHeaderCtrl->IsKindOf(RUNTIME_CLASS(CXTPHeaderCtrl)))
		return NULL;

	return (CXTPHeaderCtrl*)pHeaderCtrl;
}
