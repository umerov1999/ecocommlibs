// XTPReportView.cpp : implementation of the CXTPReportView class.
//
// This file is a part of the XTREME REPORTCONTROL MFC class library.
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

#include "StdAfx.h"


#include "Common/XTPDrawHelpers.h"
#include "Common/XTPImageManager.h"
#include "Common/XTPCustomHeap.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPSmartPtrInternalT.h"
#include "Common/XTPColorManager.h"

#include "Resource.h"


#include "XTPReportDefines.h"
#include "XTPReportControl.h"
#include "XTPReportView.h"
#include "XTPReportColumn.h"
#include "XTPReportColumns.h"
#include "XTPReportRows.h"
#include "XTPReportSection.h"
#include "XTPReportSelectedRows.h"
#include "XTPReportPaintManager.h"
#include "XTPReportPageSetupDialog.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CXTPReportView

IMPLEMENT_DYNCREATE(CXTPReportView, CView)

CXTPReportView::CXTPReportView()
	: m_nColumnStart(0)
	, m_nColumnEnd  (0)

	, m_pScrollBarVer(NULL)
	, m_pScrollBarHor(NULL)
	, m_pSlider      (NULL)
{
	m_pReport = NULL;
	m_pPrintOptions = new CXTPReportViewPrintOptions();

	m_bPrintSelection = FALSE;
	m_bPaginated = FALSE;

	m_bPrintDirect = FALSE;
	m_bResizeControlWithView = TRUE;

	m_bAllowCut = TRUE;
	m_bAllowPaste = TRUE;


	m_nStartIndex = 0;
	m_bSwitchMode = FALSE;
}

CXTPReportView::~CXTPReportView()
{
	CMDTARGET_RELEASE(m_pPrintOptions);
	SAFE_DELETE(m_pReport);
}


CXTPReportControl& CXTPReportView::GetReportCtrl() const
{
	return m_pReport == NULL ? (CXTPReportControl&)m_wndReport : *m_pReport;
}

void CXTPReportView::SetReportCtrl(CXTPReportControl* pReport)
{
	if (::IsWindow(m_wndReport.GetSafeHwnd()))
		m_wndReport.DestroyWindow();

	m_pReport = pReport;
}

void CXTPReportView::SetScrollBarCtrl(CScrollBar* pScrollBar, BOOL bHor)
{
	if (bHor)
	{
		m_pScrollBarHor = pScrollBar;
	}
	else
	{
		m_pScrollBarVer = pScrollBar;
	}
}

void CXTPReportView::SetSliderCtrl(CSliderCtrl* pSlider)
{
	m_pSlider = pSlider;
}

BEGIN_MESSAGE_MAP(CXTPReportView, CView)
	//{{AFX_MSG_MAP(CXTPReportView)
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_PAINT()
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEditCut)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTPReportView drawing

void CXTPReportView::OnDraw(CDC* pDC)
{
	UNREFERENCED_PARAMETER(pDC);
}

void CXTPReportView::OnPaint()
{
	Default();
}

BOOL CXTPReportView::OnEraseBkgnd(CDC* pDC)
{
	UNREFERENCED_PARAMETER(pDC);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CXTPReportView diagnostics

#ifdef _DEBUG
void CXTPReportView::AssertValid() const
{
	CView::AssertValid();
}

void CXTPReportView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CXTPReportView message handlers

CScrollBar* CXTPReportView::GetScrollBarCtrl(int nBar) const
{
	if (nBar == SB_VERT && m_pScrollBarVer)
		return m_pScrollBarVer;

	if (nBar == SB_HORZ && m_pScrollBarHor)
		return m_pScrollBarHor;

	return CView::GetScrollBarCtrl(nBar);
}

void CXTPReportView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (m_pScrollBarVer && pScrollBar == m_pScrollBarVer)
	{
		GetReportCtrl().OnVScroll(nSBCode, nPos, 0);
	}

	CView::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CXTPReportView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (m_pScrollBarHor && pScrollBar == m_pScrollBarHor)
	{
		GetReportCtrl().OnHScroll(nSBCode, nPos, 0);
	}

	CView::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CXTPReportView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	int nScroll = GetSystemMetrics(SM_CXVSCROLL);
	//SM_CXVSCROLL SM_CYHSCROLL SM_CXBORDER SM_CYBORDER
	int dx = 0;
	if (GetPaintManager()->m_bMoveScrollbarOnFixedColumnsIndent)
		dx = GetPaintManager()->m_nFixedColumnsIndent;

	BOOL bV = (m_pScrollBarVer != NULL) && (m_pScrollBarVer->GetSafeHwnd() != NULL);
	BOOL bH = (m_pScrollBarHor != NULL) && (m_pScrollBarHor->GetSafeHwnd() != NULL);

	CRect rc; GetClientRect(&rc);
	cx = rc.Width();
	cy = rc.Height();

	int CX = cx;
	int CY = cy;

	if (bV || bH)
	{
		if (bV && bH)
		{
			cx -= nScroll;
			cy -= nScroll;
		}
		else if (bV)
		{
			cx -= nScroll;
		}
		else if (bH)
		{
			cy -= nScroll;
		}

		// Right-to-left
		if (GetReportCtrl().IsLayoutRTL())
		{
			if (bV)
			{
				m_pScrollBarVer->MoveWindow(0, 0, nScroll, cy);
			}
			if (bH)
			{
				m_pScrollBarHor->MoveWindow(0, cy, CX - dx, nScroll);

				if (WS_EX_LAYOUTRTL != (m_pScrollBarHor->GetExStyle() & WS_EX_LAYOUTRTL))
				{
					// Turn RTL on
					m_pScrollBarHor->ModifyStyleEx(0, WS_EX_LAYOUTRTL);
				}
			}

			if (m_pSlider)
			{
				if (dx)
				{
					m_pSlider->MoveWindow(CX - dx, cy, dx, nScroll);
					m_pSlider->ShowWindow(SW_SHOW);
				}
				else
				{
					m_pSlider->MoveWindow(CX - dx, cy, dx, nScroll);
					m_pSlider->ShowWindow(SW_HIDE);
				}
			}
		}
		// Left-to-right
		else
		{
			if (bV)
			{
				m_pScrollBarVer->MoveWindow(cx, 0, nScroll, cy);
			}
			if (bH)
			{
				m_pScrollBarHor->MoveWindow(dx, cy, CX - dx, nScroll);

				if (WS_EX_LAYOUTRTL == (m_pScrollBarHor->GetExStyle() & WS_EX_LAYOUTRTL))
				{
					// Turn RTL off
					m_pScrollBarHor->ModifyStyleEx(WS_EX_LAYOUTRTL, 0);
				}
			}

			if (m_pSlider)
			{
				if (dx)
				{
					m_pSlider->MoveWindow(0, cy, dx, nScroll);
					m_pSlider->ShowWindow(SW_SHOW);
				}
				else
				{
					m_pSlider->MoveWindow(0, cy, dx, nScroll);
					m_pSlider->ShowWindow(SW_HIDE);
				}
			}
		}
	}
	else
		nScroll = 0;

	if (m_bResizeControlWithView && GetReportCtrl().GetSafeHwnd())
	{
		if (nScroll > 0)
		{
			if (GetReportCtrl().IsLayoutRTL())
			{
				if (bV)
					GetReportCtrl().MoveWindow(nScroll, 0, cx, cy);
				else
					GetReportCtrl().MoveWindow(0, 0, cx, cy);
			}
			else
			{
				if (bV)
					GetReportCtrl().MoveWindow(0, 0, cx, cy);
				else
					GetReportCtrl().MoveWindow(0, 0, cx, cy);
			}
		}
		else
		{
			GetReportCtrl().MoveWindow(0, 0, CX, CY);
		}
	}
}

BOOL CXTPReportView::PreCreateWindow(CREATESTRUCT &cs)
{
	cs.style |= WS_CLIPCHILDREN|WS_CLIPSIBLINGS;

	return CView::PreCreateWindow(cs);
}

int CXTPReportView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!GetReportCtrl().Create(WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_TABSTOP | WS_VISIBLE,
		CRect(0, 0, 0, 0), this, XTP_ID_REPORT_CONTROL))
	{
		TRACE(_T("Failed to create report control window\n"));
		return -1;
	}

	return 0;
}

void CXTPReportView::OnSetFocus(CWnd* pOldWnd)
{
	CView::OnSetFocus(pOldWnd);

	if (IsWindow(GetReportCtrl().GetSafeHwnd()))
		GetReportCtrl().SetFocus();
}


/////////////////////////////////////////////////////////////////////////////

BOOL CXTPReportView::OnPreparePrinting(CPrintInfo* pInfo)
{
	if (GetReportCtrl().IsIconView())
	{
		GetReportCtrl().SetIconView(FALSE);
		m_bSwitchMode = TRUE;
	}

	m_bShowRowNumber = GetReportCtrl().IsShowRowNumber();
	GetReportCtrl().ShowRowNumber(FALSE);

	if (GetReportCtrl().GetSelectedRows()->GetCount() > 0)
		pInfo->m_pPD->m_pd.Flags &= ~PD_NOSELECTION;

	pInfo->m_bDirect = m_bPrintDirect;

	// default preparation
	if (!DoPreparePrinting(pInfo))
		return FALSE;

	m_bPrintSelection = pInfo->m_pPD->PrintSelection();

	return TRUE;
}

void CXTPReportView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	m_aPageStart.RemoveAll();

	if (m_pPrintOptions->m_bBlackWhitePrinting)
		m_bmpGrayDC.DeleteObject();

	if (m_bSwitchMode)
	{
		GetReportCtrl().SetIconView(TRUE);
		m_bSwitchMode = FALSE;
	}
	GetReportCtrl().ShowRowNumber(m_bShowRowNumber);
}

CXTPReportPaintManager* CXTPReportView::GetPaintManager() const
{
	return GetReportCtrl().GetPaintManager();
}

void CXTPReportView::PrintHeader(CDC *pDC, CRect rcHeader)
{
	CXTPReportColumns *pColumns = GetReportCtrl().GetColumns();

	GetPaintManager()->FillHeaderControl(pDC, rcHeader);

	int x = rcHeader.left;
	int nWidth;

	for (int nColumn=m_nColumnStart; nColumn<m_nColumnEnd; nColumn++)
	{
		CXTPReportColumn *pColumn = pColumns->GetAt(nColumn);
		if (pColumn->IsVisible())
		{
			if (GetPaintManager()->IsColumnWidthWYSIWYG())
			{
				nWidth = pColumn->GetWidth();
			}
			else
			{
				nWidth = pColumn->GetPrintWidth(rcHeader.Width());
			}
			CRect rcItem(x, rcHeader.top, x + nWidth, rcHeader.bottom);
			GetPaintManager()->DrawColumn(pDC, pColumn, GetReportCtrl().GetReportHeader(), rcItem);
			x += nWidth;
		}
	}
}

void CXTPReportView::PrintFooter(CDC* pDC, CRect rcFooter)
{
	CXTPReportColumns* pColumns = GetReportCtrl().GetColumns();

	GetPaintManager()->FillFooter(pDC, rcFooter);

	int x = rcFooter.left;
	int nWidth(0);
	for (int nColumn=m_nColumnStart; nColumn<m_nColumnEnd; nColumn++)
	{
		CXTPReportColumn* pColumn = pColumns->GetAt(nColumn);
		if (!pColumn->IsVisible())
			continue;

		if (GetPaintManager()->IsColumnWidthWYSIWYG())
			nWidth = pColumn->GetWidth();
		else
			nWidth = pColumn->GetPrintWidth(rcFooter.Width());
		CRect rcItem(x, rcFooter.top, x + nWidth, rcFooter.bottom);
		GetPaintManager()->DrawColumnFooter(pDC, pColumn, GetReportCtrl().GetReportHeader(), rcItem);
		x += nWidth;
	}
}


int CXTPReportView::PrintReport(CDC *pDC, CPrintInfo *pInfo, CRect rcPage,
	int nRowFrom, int nColumnFrom, int nColumnTo)
{
	CXTPReportControl &wndReport = GetReportCtrl();

	int nMaxPages = -1;
	if (pInfo->GetMaxPage() != 65535)
		nMaxPages = pInfo->GetMaxPage();

	if (GetPaintManager()->m_bPrintWatermark)
	{
		CRect rcWater(rcPage);
		CRect rcClient(0, 0, rcWater.Width(), rcWater.Height());

		wndReport.DrawWatermark(pDC, rcWater, rcClient);
	}

	int nHeaderHeight = PrintPageHeader(pDC, pInfo, rcPage, FALSE,
		pInfo->m_nCurPage, nMaxPages,
		m_nStartIndex, (int) GetPaintManager()->m_arStartCol.GetSize() - 1);

	int nFooterHeight = PrintPageFooter(pDC, pInfo, rcPage, FALSE,
		pInfo->m_nCurPage, nMaxPages,
		m_nStartIndex, (int) GetPaintManager()->m_arStartCol.GetSize() - 1);

	CRect rcRows(rcPage);

	if (GetPaintManager()->m_bPrintPageRectangle)
	{
		rcRows.DeflateRect(1,1);
	}

	rcRows.top += nHeaderHeight;
	rcRows.bottom -= nFooterHeight;
	int nFooterTop = rcRows.bottom;


	// print header rows (if exist and visible)
	if (wndReport.GetHeaderRows()->GetCount() > 0 && GetReportCtrl().IsHeaderRowsVisible())
	{
		// print on the first page, at least
		if (0 == nRowFrom || m_pPrintOptions->m_bRepeatHeaderRows)
		{
			int nHeight = 0;
			wndReport.DrawRows(pDC, rcRows, rcRows.top, wndReport.GetHeaderRows(), 0, nColumnFrom, nColumnTo, &nHeight);
			rcRows.top += nHeight;

			// print divider
			rcRows.top += PrintFixedRowsDivider(pDC, rcRows, TRUE);
		}
	}

	// calculate space for footer rows
	int nNeedForFooterRows = 0;

	if (wndReport.GetFooterRows()->GetCount() > 0 && GetReportCtrl().IsFooterRowsVisible())
	{
		nNeedForFooterRows = wndReport.GetRowsHeight(GetReportCtrl().GetFooterRows(), rcRows.Width());
		nNeedForFooterRows += GetPaintManager()->GetSectionDividerHeight(wndReport.m_pSectionFooter->GetDividerStyle());

		if (m_pPrintOptions->m_bRepeatFooterRows)
		{
			// decrease space for body rows, if footer rows to be repeated on every page.
			rcRows.bottom = nFooterTop - nNeedForFooterRows;
		}
	}

	// print body rows
	int nPrintedBodyRowsHeight = 0;
	nRowFrom = wndReport.DrawRows(pDC, rcRows, rcRows.top, wndReport.GetRows(), nRowFrom, nColumnFrom, nColumnTo, &nPrintedBodyRowsHeight);

	// print footer rows (if exist and visible)
	if (nNeedForFooterRows > 0)
	{
		CRect rcFooterRows(rcRows);

		if (wndReport.IsFooterRowsPinnedPrinted())
		{
			rcFooterRows.top    = rcRows.top + nPrintedBodyRowsHeight; //immediately after body rows
			rcFooterRows.bottom = nFooterTop;
		}
		else
		{
			rcFooterRows.top    = nFooterTop - nNeedForFooterRows;
			rcFooterRows.bottom = nFooterTop;
		}

		// one more check, if there is enough space for footer divider + footer rows
		if (rcFooterRows.Height() >= nNeedForFooterRows)
		{
			// print divider
			rcFooterRows.top += PrintFixedRowsDivider(pDC, rcFooterRows, FALSE);

			// print footer rows
			int nHeight = 0;
			wndReport.DrawRows(pDC, rcFooterRows, rcFooterRows.top, wndReport.GetFooterRows(), 0, nColumnFrom, nColumnTo, &nHeight);
		}
	}

	if (GetPaintManager()->m_bPrintPageRectangle)
	{
		pDC->Draw3dRect(rcPage, 0, 0);
	}

	return nRowFrom;
}

int CXTPReportView::PrintFixedRowsDivider(CDC* pDC, const CRect& rc, BOOL bHeaderRows)
{
	CRect rcDivider(rc);

	XTPReportSectionDividerPosition dividerPosition = bHeaderRows
		? GetReportCtrl().m_pSectionHeader->GetDividerPosition()
		: GetReportCtrl().m_pSectionFooter->GetDividerPosition();

	XTPReportFixedRowsDividerStyle dividerStyle = bHeaderRows
		? GetReportCtrl().m_pSectionHeader->GetDividerStyle()
		: GetReportCtrl().m_pSectionFooter->GetDividerStyle();

	CXTPPaintManagerColor dividerColor = bHeaderRows
		? GetReportCtrl().m_pSectionHeader->GetDividerColor()
		: GetReportCtrl().m_pSectionHeader->GetDividerColor();

	int nHeight = GetPaintManager()->GetSectionDividerHeight(dividerStyle);

	rcDivider.bottom = rcDivider.top + nHeight;

	GetPaintManager()->DrawSectionDivider(pDC, rcDivider, dividerPosition, dividerStyle, dividerColor);

	return nHeight;
}

int CXTPReportView::PrintPage(CDC *pDC, CPrintInfo *pInfo, CRect rcPage,
	int nRowFrom, int nColumnFrom, int nColumnTo)
{
	if (!m_pPrintOptions || !pDC || !pInfo)
	{
		ASSERT(FALSE);
		return INT_MAX;
	}

	CRect rcPageHeader = rcPage;
	CRect rcPageFooter = rcPage;

	CString strTitle = CXTPPrintPageHeaderFooter::GetParentFrameTitle(this);

	int N = (int) GetPaintManager()->m_arStartCol.GetSize() - 1;
	int nVirPage(0);
	if (N > 0)
		nVirPage = 1 + m_nStartIndex;

	if (GetPaintManager()->m_bPrintVirtualPageNumber)
	{
		m_pPrintOptions->GetPageHeader()->FormatTexts(pInfo, strTitle, nVirPage);
		m_pPrintOptions->GetPageFooter()->FormatTexts(pInfo, strTitle, nVirPage);
	}
	else
	{
		m_pPrintOptions->GetPageHeader()->FormatTexts(pInfo, strTitle);
		m_pPrintOptions->GetPageFooter()->FormatTexts(pInfo, strTitle);
	}

	m_pPrintOptions->GetPageHeader()->Draw(pDC, rcPageHeader);
	m_pPrintOptions->GetPageFooter()->Draw(pDC, rcPageFooter);

	CRect rcReport = rcPage;
	rcReport.top    += rcPageHeader.Height() + 2;
	rcReport.bottom -= rcPageFooter.Height() + 2;

	nRowFrom = PrintReport(pDC, pInfo, rcReport, nRowFrom, nColumnFrom, nColumnTo);

	return nRowFrom;
}

int CXTPReportView::PrintPageHeader(CDC* pDC, CPrintInfo* pInfo, CRect rcPage,
					BOOL bOnlyCalculate, int nPageNumber, int nNumberOfPages,
					int nHorizontalPageNumber, int nNumberOfHorizontalPages)
{
	UNREFERENCED_PARAMETER(pInfo);
	UNREFERENCED_PARAMETER(nPageNumber);
	UNREFERENCED_PARAMETER(nNumberOfPages);
	UNREFERENCED_PARAMETER(nHorizontalPageNumber);
	UNREFERENCED_PARAMETER(nNumberOfHorizontalPages);
	UNREFERENCED_PARAMETER(bOnlyCalculate);
	UNREFERENCED_PARAMETER(nPageNumber);
	UNREFERENCED_PARAMETER(nHorizontalPageNumber);
	UNREFERENCED_PARAMETER(nNumberOfHorizontalPages);

	int nHeaderHeight = 0;

	if (GetPaintManager()->m_bPrintPageRectangle)
		rcPage.DeflateRect(1,1);

	if (GetReportCtrl().IsHeaderVisible())
		nHeaderHeight = GetPaintManager()->GetHeaderHeight(&GetReportCtrl(), pDC, rcPage.Width());

	rcPage.bottom = rcPage.top + nHeaderHeight;

	if (nHeaderHeight)
		PrintHeader(pDC, rcPage);

	return nHeaderHeight;
}

int CXTPReportView::PrintPageFooter(CDC* pDC, CPrintInfo* pInfo, CRect rcPage,
					BOOL bOnlyCalculate, int nPageNumber, int nNumberOfPages,
					int nHorizontalPageNumber, int nNumberOfHorizontalPages)
{
	UNREFERENCED_PARAMETER(pInfo);
	UNREFERENCED_PARAMETER(nPageNumber);
	UNREFERENCED_PARAMETER(nNumberOfPages);
	UNREFERENCED_PARAMETER(nHorizontalPageNumber);
	UNREFERENCED_PARAMETER(nNumberOfHorizontalPages);
	UNREFERENCED_PARAMETER(bOnlyCalculate);
	UNREFERENCED_PARAMETER(nPageNumber);
	UNREFERENCED_PARAMETER(nHorizontalPageNumber);
	UNREFERENCED_PARAMETER(nNumberOfHorizontalPages);

	int nFooterHeight = 0;

	if (GetPaintManager()->m_bPrintPageRectangle)
		rcPage.DeflateRect(1,1);

	if (GetReportCtrl().IsFooterVisible())
		nFooterHeight = GetPaintManager()->GetFooterHeight(&GetReportCtrl(), pDC, rcPage.Width());

	rcPage.top = rcPage.bottom - nFooterHeight;

	if (nFooterHeight)
		PrintFooter(pDC, rcPage);

	return nFooterHeight;
}

int CXTPReportView::SetupStartCol(CDC* pDC, CPrintInfo* pInfo)
{
	pDC->SetMapMode(MM_ANISOTROPIC);
	pDC->SetViewportExt(pDC->GetDeviceCaps(LOGPIXELSX), pDC->GetDeviceCaps(LOGPIXELSY));
	pDC->SetWindowExt(96, 96);
	pDC->OffsetWindowOrg(0, 0);

	pInfo->m_rectDraw.SetRect(0, 0, pDC->GetDeviceCaps(HORZRES), pDC->GetDeviceCaps(VERTRES));
	pDC->DPtoLP(&pInfo->m_rectDraw);

	CRect rcMargins = m_pPrintOptions->GetMarginsLP(pDC);
	CRect rc = pInfo->m_rectDraw;
	rc.DeflateRect(rcMargins);
	GetPaintManager()->m_PrintPageWidth = rc.Width();
	GetPaintManager()->m_arStartCol.RemoveAll();
	GetPaintManager()->m_arStartCol.Add(0);
	CXTPReportColumns* pColumns = GetReportCtrl().GetColumns();
	int nColumnCount = pColumns->GetCount();

	if (GetPaintManager()->IsColumnWidthWYSIWYG())
	{
		int x = 0;
		for (int nColumn = 0; nColumn < nColumnCount; nColumn++)
		{
			CXTPReportColumn* pColumn = pColumns->GetAt(nColumn);
			if (pColumn && pColumn->IsVisible())
			{
				int nWidth = pColumn->GetWidth();

				if (x + nWidth <= GetPaintManager()->m_PrintPageWidth)
				{
					x += nWidth;
				}
				else
				{
					GetPaintManager()->m_arStartCol.Add(nColumn);
					x = nWidth;
				}
			}
		}
	}
	GetPaintManager()->m_arStartCol.Add(nColumnCount);

	return (int) GetPaintManager()->m_arStartCol.GetSize() - 1;
}


//struct CPrintInfo // Printing information structure
//  CPrintDialog* m_pPD;     // pointer to print dialog
//  BOOL m_bPreview;         // TRUE if in preview mode
//  BOOL m_bDirect;          // TRUE if bypassing Print Dialog
//  BOOL m_bContinuePrinting;// set to FALSE to prematurely end printing
//  UINT m_nCurPage;         // Current page
//  UINT m_nNumPreviewPages; // Desired number of preview pages
//  CString m_strPageDesc;   // Format string for page number display
//  void SetMinPage(UINT nMinPage);
//  void SetMaxPage(UINT nMaxPage);
//  UINT GetMinPage() const;
//  UINT GetMaxPage() const;
//  UINT GetFromPage() const;
//  UINT GetToPage() const;
void CXTPReportView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	m_aPageStart.RemoveAll();
	m_aPageStart.Add(0);

	int nVirtualPages = SetupStartCol(pDC, pInfo);
	if (pInfo->m_bPreview)
	{
		pInfo->m_nNumPreviewPages = 1;
		AfxGetApp()->m_nNumPreviewPages = pInfo->m_nNumPreviewPages;
	}

	CString str1, str2;
	if (m_pPrintOptions && m_pPrintOptions->GetPageHeader())
		str1 = m_pPrintOptions->GetPageHeader()->m_strFormatString;
	if (m_pPrintOptions && m_pPrintOptions->GetPageFooter())
		str2 = m_pPrintOptions->GetPageFooter()->m_strFormatString;

	if (nVirtualPages > 1 && m_pPrintOptions->GetPageHeader()->m_strFormatString.IsEmpty())
		m_pPrintOptions->GetPageHeader()->m_strFormatString = _T(" &p/&P ");

	UINT pFr = pInfo->GetFromPage();
	UINT pTo = pInfo->GetToPage();
	if (pFr > 0)
	{
		pInfo->SetMinPage(pFr);
		pInfo->m_nCurPage = pFr;
	}

	if (pTo < 65535)
	{
		pInfo->SetMaxPage(pTo);
	}

	if (GetReportCtrl().m_bForcePagination)
	{
		m_bPaginated = FALSE;
	}

	if ((!pInfo->m_bPreview || GetReportCtrl().m_bForcePagination)
		&& (str1.Find(_T("&P")) >= 0
		|| nVirtualPages > 1
		|| pFr > 1
		|| pTo < 65535
		|| str2.Find(_T("&P")) >= 0))
	{
		m_nStartIndex = 0;
		m_nColumnStart = GetPaintManager()->m_arStartCol.GetAt(0);
		m_nColumnEnd   = GetPaintManager()->m_arStartCol.GetAt(1);

		int nCurPage = pInfo->m_nCurPage;
		pInfo->m_nCurPage = 65535;

		if (PaginateTo(pDC, pInfo))
			pInfo->SetMaxPage((int) m_aPageStart.GetSize() - 1 - m_nStartIndex);

		pInfo->m_nCurPage = nCurPage;
		if (GetReportCtrl().m_bForcePagination)
		{
			m_bPaginated = TRUE;
		}
	}
	m_nStartIndex = 0;
	m_nColumnStart = GetPaintManager()->m_arStartCol.GetAt(0);
	m_nColumnEnd   = GetPaintManager()->m_arStartCol.GetAt(1);
}

void CXTPReportView::OnPrint(CDC *pDC, CPrintInfo *pInfo)
{
	if (!m_pPrintOptions || !pDC || !pInfo)
	{
		ASSERT(FALSE);
		return;
	}
	CRect rcMargins = m_pPrintOptions->GetMarginsLP(pDC);
	CRect rc = pInfo->m_rectDraw;
	rc.DeflateRect(rcMargins);

	UINT nVirtualPages = (UINT) GetPaintManager()->m_arStartCol.GetSize() - 1;
	UINT nPage = pInfo->m_nCurPage;
	int nSize = (int) m_aPageStart.GetSize();

	ASSERT(nPage <= (UINT) nSize);

	UINT nIndex = m_aPageStart[nPage - 1];
	UINT mIndex(0);

	//if (m_bPaginated)
	{
		m_nStartIndex = (pInfo->m_nCurPage - 1) % nVirtualPages;
		m_nColumnStart = GetPaintManager()->m_arStartCol.GetAt(m_nStartIndex);
		m_nColumnEnd   = GetPaintManager()->m_arStartCol.GetAt(m_nStartIndex + 1);
	}

	//TRACE(_T("CXTPReportView::OnPrint %d %d %d %d %d\n"), pInfo->m_nCurPage, nVirtualPages, m_nStartIndex, m_nColumnStart, m_nColumnEnd);

	if (!m_pPrintOptions->m_bBlackWhitePrinting)
	{
		mIndex = PrintPage(pDC, pInfo, rc, nIndex, m_nColumnStart, m_nColumnEnd);
	}
	else
	{
		CRect rc00(0, 0, rc.Width(), rc.Height());

		CDC memDC;
		VERIFY(memDC.CreateCompatibleDC(pDC));
		memDC.m_bPrinting = TRUE;

		if (!m_bmpGrayDC.m_hObject
			|| m_bmpGrayDC.GetBitmapDimension() != rc00.Size())
		{
			m_bmpGrayDC.DeleteObject();
			m_bmpGrayDC.CreateCompatibleBitmap(pDC, rc00.Width(), rc00.Height());
		}

		CXTPBitmapDC autpBmp(&memDC, &m_bmpGrayDC);

		memDC.FillSolidRect(rc00, RGB(255, 255, 255));

		mIndex = PrintPage(&memDC, pInfo, rc00, nIndex, m_nColumnStart, m_nColumnEnd);

		int nCC = max(0, min(m_pPrintOptions->m_nBlackWhiteContrast, 255));
		XTPImageManager()->BlackWhiteBitmap(memDC, rc00, nCC);

		pDC->BitBlt(rc.left, rc.top, rc.Width(), rc.Height(), &memDC, 0, 0, SRCCOPY);
	}

	if (!m_bPaginated)
	{
		m_nStartIndex++;
		if (m_nStartIndex >= nVirtualPages) //done with the page!
			m_nStartIndex = 0;

		m_nColumnStart = GetPaintManager()->m_arStartCol.GetAt(m_nStartIndex);
		m_nColumnEnd   = GetPaintManager()->m_arStartCol.GetAt(m_nStartIndex + 1);

		if (m_nStartIndex > 0)
		{
			if (nPage == (UINT) nSize)
				m_aPageStart.Add(nIndex);
			else if (nPage < (UINT) nSize)
				m_aPageStart[nPage] = nIndex;
		}
		else
		{
			if (nPage == (UINT) nSize)
				m_aPageStart.Add(mIndex);
			else if (nPage < (UINT) nSize)
				m_aPageStart[nPage] = mIndex;

			if ((int) mIndex == GetReportCtrl().GetRows()->GetCount())
				pInfo->SetMaxPage(pInfo->m_nCurPage);
		}
	}

	if (pInfo->m_bPreview)
	{
		pInfo->m_nCurPage++;
	}
}

extern BOOL CALLBACK _XTPAbortProc(HDC, int);

BOOL CXTPReportView::PaginateTo(CDC* pDC, CPrintInfo* pInfo)
{
	ASSERT_VALID(this);
	ASSERT_VALID(pDC);

	UINT nVirtualPages = (UINT) GetPaintManager()->m_arStartCol.GetSize() - 1;
	BOOL bAborted = FALSE;
	CXTPPrintingDialog PrintStatus(this);
	CString strTemp;
	if (GetParentFrame())
		GetParentFrame()->GetWindowText(strTemp);

	PrintStatus.SetWindowText(_T("Calculating pages..."));
	PrintStatus.SetDlgItemText(AFX_IDC_PRINT_DOCNAME, strTemp);
	PrintStatus.SetDlgItemText(AFX_IDC_PRINT_PRINTERNAME, pInfo->m_pPD->GetDeviceName());
	PrintStatus.SetDlgItemText(AFX_IDC_PRINT_PORTNAME, pInfo->m_pPD->GetPortName());
	PrintStatus.ShowWindow(SW_SHOW);
	PrintStatus.UpdateWindow();

	CRect rectSave = pInfo->m_rectDraw;
	UINT nPageSave = pInfo->m_nCurPage;
	BOOL bBlackWhiteSaved = m_pPrintOptions->m_bBlackWhitePrinting;
	m_pPrintOptions->m_bBlackWhitePrinting = FALSE;

	ASSERT(nPageSave > 1);
	ASSERT(nPageSave >= (UINT) m_aPageStart.GetSize());
	VERIFY(pDC->SaveDC() != 0);

	pDC->IntersectClipRect(0, 0, 0, 0);
	UINT nCurPage = (UINT) m_aPageStart.GetSize();
	pInfo->m_nCurPage = nCurPage;

	while (pInfo->m_nCurPage < nPageSave && pInfo->m_nCurPage <= pInfo->GetMaxPage())
	{
		if (pInfo->m_bPreview)
			ASSERT(pInfo->m_nCurPage == (UINT) m_aPageStart.GetSize());

		OnPrepareDC(pDC, pInfo);

		if (!pInfo->m_bContinuePrinting)
			break;
		if (nVirtualPages > 1)
			strTemp.Format(_T("%d [%d - %d]"),
				pInfo->m_nCurPage,
				1 + (pInfo->m_nCurPage / nVirtualPages),
				1 + (pInfo->m_nCurPage % nVirtualPages));
		else
			strTemp.Format(_T("%d"), pInfo->m_nCurPage);

		PrintStatus.SetDlgItemText(AFX_IDC_PRINT_PAGENUM, strTemp);

		pInfo->m_rectDraw.SetRect(0, 0, pDC->GetDeviceCaps(HORZRES), pDC->GetDeviceCaps(VERTRES));
		pDC->DPtoLP(&pInfo->m_rectDraw);

		OnPrint(pDC, pInfo);

		if (!pInfo->m_bPreview)
			pInfo->m_nCurPage++;

		if (pInfo->GetMaxPage() == 65535)
			pInfo->SetMaxPage(max(pInfo->GetMaxPage(), pInfo->m_nCurPage));

		if (!_XTPAbortProc(0, 0))
		{
			bAborted = TRUE;
			break;
		}
	}
	PrintStatus.DestroyWindow();

	BOOL bResult = !bAborted
		&& (pInfo->m_nCurPage == nPageSave || nPageSave == 65535);

	pInfo->m_bContinuePrinting = bResult;
	pDC->RestoreDC(-1);
	m_pPrintOptions->m_bBlackWhitePrinting = bBlackWhiteSaved;
	pInfo->m_nCurPage = nPageSave;

	pInfo->m_rectDraw = rectSave;
	ASSERT_VALID(this);

	return bResult;
}

void CXTPReportView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)
{
	ASSERT_VALID(this);
	ASSERT_VALID(pDC);
	if (!pInfo)
		return;

	if (!m_bPaginated)
	{
		int nRowCount = GetReportCtrl().GetRows()->GetCount();
		int nSize = (int) m_aPageStart.GetSize();
		int nPage = pInfo->m_nCurPage;
		UINT nVirtualPages = (UINT) GetPaintManager()->m_arStartCol.GetSize() - 1;
		UINT nVirPage(0);
		if (nVirtualPages > 0)
			nVirPage = 1 + m_nStartIndex;

		if (nPage == 1 && nRowCount == 0)                       //First page?
			pInfo->m_bContinuePrinting = TRUE;
		else if (nVirPage > 0 && nVirPage < nVirtualPages)      //not finished page
			pInfo->m_bContinuePrinting = TRUE;
		else if (nPage == nSize && m_aPageStart[nPage - 1] >= (UINT) nRowCount
			&& m_nStartIndex >= nVirtualPages - 1)              //Last page?
			pInfo->m_bContinuePrinting = FALSE;                 // can't paginate to that page
		else if (nPage > nSize
			&& m_nStartIndex > nVirtualPages - 1
			&& !PaginateTo(pDC, pInfo))                         //Can be last page?
			pInfo->m_bContinuePrinting = FALSE;                 // can't paginate to that page

		if (pInfo->m_nCurPage > pInfo->GetMaxPage())
			pInfo->m_bContinuePrinting = FALSE;
//TRACE(_T("OnPrepareDC pInfo->m_nCurPage=%d\n"), pInfo->m_nCurPage);
	}
	pDC->SetMapMode(MM_ANISOTROPIC);
	pDC->SetViewportExt(pDC->GetDeviceCaps(LOGPIXELSX), pDC->GetDeviceCaps(LOGPIXELSY));
	pDC->SetWindowExt(96, 96);
	pDC->OffsetWindowOrg(0, 0);

	if (pInfo->m_bPreview) //PRINT MODE in RTL does not work!
	{
//------------------------------------------------------------
	if (GetReportCtrl().GetExStyle() & WS_EX_RTLREADING)
		pDC->SetTextAlign(TA_RTLREADING);

		if (GetReportCtrl().GetExStyle() & WS_EX_LAYOUTRTL)
		//if (GetReportCtrl().IsLayoutRTL())
		XTPDrawHelpers()->SetContextRTL(pDC, LAYOUT_RTL);
//------------------------------------------------------------
	}
}

//////////////////////////////////////////////////////////////////////////
// Clipboard operations

void CXTPReportView::OnUpdateEditCopy(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(GetReportCtrl().CanCopy());
}

void CXTPReportView::OnUpdateEditCut(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_bAllowCut && GetReportCtrl().CanCut());
}

void CXTPReportView::OnUpdateEditPaste(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_bAllowPaste && GetReportCtrl().CanPaste());
}

void CXTPReportView::OnEditCut()
{
	if (m_bAllowCut) GetReportCtrl().Cut();
}

void CXTPReportView::OnEditCopy()
{
	GetReportCtrl().Copy();
}

void CXTPReportView::OnEditPaste()
{
	if (m_bAllowPaste) GetReportCtrl().Paste();
}

void CXTPReportView::OnFilePageSetup()
{
	DWORD dwFlags = PSD_MARGINS | PSD_INWININIINTLMEASURE;
	CXTPReportPageSetupDialog dlgPageSetup(GetPrintOptions(), dwFlags, this);

	XTPGetPrinterDeviceDefaults(dlgPageSetup.m_psd.hDevMode, dlgPageSetup.m_psd.hDevNames);

	int nDlgRes = (int) dlgPageSetup.DoModal();

	if (nDlgRes == IDOK)
		AfxGetApp()->SelectPrinter(dlgPageSetup.m_psd.hDevNames, dlgPageSetup.m_psd.hDevMode, FALSE);
}
