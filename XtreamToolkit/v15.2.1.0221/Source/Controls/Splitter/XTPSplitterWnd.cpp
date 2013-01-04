// SplitterWndXT.cpp : implementation file
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

#include "Common/XTPColorManager.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPDrawHelpers.h"

#include "../Defines.h"
#include "../Util/XTPGlobal.h"
#include "../Util/XTPControlTheme.h"
#include "XTPSplitterWnd.h"
#include "XTPSplitterWndTheme.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Visual attributes and other constants

// HitTest return values (values and spacing between values is important)
enum HitTestValue
{
	noHit = 0,
	vSplitterBox = 1,
	hSplitterBox = 2,
	bothSplitterBox = 3, // just for keyboard
	vSplitterBar1 = 101,
	vSplitterBar15 = 115,
	hSplitterBar1 = 201,
	hSplitterBar15 = 215,
	splitterIntersection1 = 301,
	splitterIntersection225 = 525
};

/////////////////////////////////////////////////////////////////////////////
// CXTPSplitterWnd

CXTPSplitterWnd::CXTPSplitterWnd()
: m_pTheme(NULL)
, m_nHiddenCol(-1)
, m_nHiddenRow(-1)
, m_bDotTracker(FALSE)
, m_bNoSize(FALSE)
, m_bFullDrag(FALSE)
, m_bShowBorder(TRUE)
, m_nFullDrag(-1)
, m_point(-1, -1)
, m_bClipStyles(FALSE)
{
	m_cxSplitter = m_cySplitter = 6;
	m_cxSplitterGap = m_cySplitterGap = 6;

	// Get system settings for full drag.
	::SystemParametersInfo(SPI_GETDRAGFULLWINDOWS,
		0, &m_bFullDrag, 0);

	VERIFY(SetTheme(xtpControlThemeDefault));

}

CXTPSplitterWnd::~CXTPSplitterWnd()
{
	CMDTARGET_RELEASE(m_pTheme);
}

IMPLEMENT_DYNAMIC(CXTPSplitterWnd, CSplitterWnd)

BEGIN_MESSAGE_MAP(CXTPSplitterWnd, CSplitterWnd)
	//{{AFX_MSG_MAP(CXTPSplitterWnd)
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
	ON_WM_NCHITTEST_EX()
	ON_MESSAGE(WM_XTP_SETCONTROLTHEME, OnSetTheme)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTPSplitterWnd message handlers

void CXTPSplitterWnd::RefreshMetrics()
{
	if (m_pTheme)
		m_pTheme->RefreshMetrics();

	if (::IsWindow(m_hWnd))
		RedrawWindow();
}

BOOL CXTPSplitterWnd::SetTheme(CXTPSplitterWndTheme* pTheme)
{
	CMDTARGET_RELEASE(m_pTheme);
	m_pTheme = pTheme;

	RefreshMetrics();

	return (m_pTheme != NULL);
}

BOOL CXTPSplitterWnd::SetTheme(XTPControlTheme eTheme)
{
	CXTPSplitterWndTheme* pTheme = NULL;

	switch (eTheme)
	{
	case xtpControlThemeOffice2000:
	case xtpControlThemeOfficeXP:
	case xtpControlThemeVisualStudio2005:
	case xtpControlThemeVisualStudio2008:
		pTheme = new CXTPSplitterWndThemeOfficeXP();
		break;

	case xtpControlThemeVisualStudio2010:
		pTheme = new CXTPSplitterWndThemeVisualStudio2010();
		break;

	case xtpControlThemeOffice2003:
		pTheme = new CXTPSplitterWndThemeOffice2003();
		break;

	case xtpControlThemeResource:
		pTheme = new CXTPSplitterWndThemeResource();
		break;

	case xtpControlThemeDefault:
	default:
		pTheme = new CXTPSplitterWndTheme();
		break;
	}

	return SetTheme(pTheme);
}

LRESULT CXTPSplitterWnd::OnSetTheme(WPARAM wParam, LPARAM /*lParam*/)
{
	XTPControlTheme eTheme = (XTPControlTheme)wParam;

	SetTheme(eTheme);

	return 0;
}

BOOL CXTPSplitterWnd::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	switch (message)
	{
	case WM_SETTINGCHANGE:
		::SystemParametersInfo(SPI_GETDRAGFULLWINDOWS, 0, &m_bFullDrag, 0);
		RefreshMetrics();
		break;

	case WM_SYSCOLORCHANGE:
		RefreshMetrics();
		break;

	case WM_PRINTCLIENT:
		PrintClient(CDC::FromHandle((HDC)wParam));
		return TRUE;

	case WM_LBUTTONDOWN:
		m_htTrack = HitTest(CPoint((int)LOWORD(lParam), (int)HIWORD(lParam)));
		break;

	case WM_LBUTTONUP:
		Invalidate();
		break;
	}

	return CSplitterWnd::OnWndMsg(message, wParam, lParam, pResult);
}

void CXTPSplitterWnd::PrintClient(CDC* pDC)
{
	if (pDC)
	{
		CXTPClientRect rClient(this);
		rClient.InflateRect(-m_cxBorder, -m_cyBorder);

		CRect rectInside;
		GetInsideRect(rectInside);

		// draw the splitter boxes
		if (m_bHasVScroll && m_nRows < m_nMaxRows)
		{
			OnDrawSplitter(pDC, splitBox,
				CRect(rectInside.right, rClient.top,
				rClient.right, rClient.top + m_cySplitter));
		}

		if (m_bHasHScroll && m_nCols < m_nMaxCols)
		{
			OnDrawSplitter(pDC, splitBox,
				CRect(rClient.left, rectInside.bottom,
				rClient.left + m_cxSplitter, rClient.bottom));
		}

		// extend split bars to window border (past margins)
		DrawAllSplitBars(pDC, rectInside.right, rectInside.bottom);
	}
}

void CXTPSplitterWnd::ShowColumn()
{
	ASSERT_VALID(this);
	if (m_nCols == m_nMaxCols && m_nHiddenCol == -1)
	{
		return;
	}
	ASSERT(m_nHiddenCol != -1);

	int nShowCol = m_nHiddenCol;
	m_nHiddenCol = -1;

	int cxNew = m_pColInfo[m_nCols].nCurSize;
	m_nCols++;  // add a column

	ASSERT(m_nCols == m_nMaxCols);

	int nCol;

	// Show the hidden column
	int nRow;
	for (nRow = 0; nRow < m_nRows; ++nRow)
	{
		int nShowRow = m_nHiddenRow != -1 && m_nHiddenRow <= nRow ? nRow + 1 : nRow;
		CWnd* pPaneShow = GetDlgItem(AFX_IDW_PANE_FIRST + nShowRow * 16 + m_nCols);

		ASSERT(pPaneShow != NULL);
		if (!pPaneShow)
			return;
		pPaneShow->ShowWindow(SW_SHOWNA);

		for (nCol = m_nCols - 2; nCol >= nShowCol; --nCol)
		{
			CWnd* pPane = GetPane(nRow, nCol);
			ASSERT(pPane != NULL);
			if (!pPane)
				return;
			pPane->SetDlgCtrlID(IdFromRowCol(nRow, nCol + 1));
		}

		pPaneShow->SetDlgCtrlID(IdFromRowCol(nRow, nShowCol));
	}

	if (m_nHiddenRow != -1)
	{
		CWnd* pPaneHide = GetDlgItem(AFX_IDW_PANE_FIRST + m_nHiddenRow * 16 + m_nCols);
		if (pPaneHide)
		{
			pPaneHide->SetDlgCtrlID(AFX_IDW_PANE_FIRST + m_nMaxRows * 16 + nShowCol);
		}
	}

	// new panes have been created -- recalculate layout
	for (nCol = nShowCol + 1; nCol < m_nCols; nCol++)
		m_pColInfo[nCol].nIdealSize = m_pColInfo[nCol - 1].nCurSize;

	m_pColInfo[nShowCol].nIdealSize = cxNew;
	RecalcLayout();
}

void CXTPSplitterWnd::HideColumn(int nColHide)
{
	ASSERT_VALID(this);
	if (m_nHiddenCol != -1)
	{
		// return if the requested one is hidden
		if (m_nHiddenCol == nColHide)
		{
			return;
		}
		ShowColumn();
	}
	ASSERT(m_nCols > 1);
	ASSERT(nColHide < m_nCols);
	ASSERT(m_nHiddenCol == -1);
	m_nHiddenCol = nColHide;

	// if the column has an active window -- change it
	int nActiveRow, nActiveCol;
	if (GetActivePane(&nActiveRow, &nActiveCol) != NULL)
	{
		if (nActiveCol == nColHide)
		{
			if (++nActiveCol >= m_nCols)
				nActiveCol = 0;
			SetActivePane(nActiveRow, nActiveCol);
		}
	}

	// hide all column panes
	int nRow;
	for (nRow = 0; nRow < m_nRows; nRow++)
	{
		CWnd* pPaneHide = GetPane(nRow, nColHide);
		ASSERT(pPaneHide != NULL);
		if (!pPaneHide)
			return;
		pPaneHide->ShowWindow(SW_HIDE);
		int nRowHide = m_nHiddenRow != -1 && m_nHiddenRow <= nRow ? nRow + 1 : nRow;
		pPaneHide->SetDlgCtrlID(AFX_IDW_PANE_FIRST + nRowHide * 16 + m_nCols);

		int nCol;
		for (nCol = nColHide + 1; nCol < m_nCols; nCol++)
		{
			CWnd* pPane = GetPane(nRow, nCol);
			ASSERT(pPane != NULL);
			if (!pPane)
				return;

			pPane->SetDlgCtrlID(IdFromRowCol(nRow, nCol - 1));
		}
	}

	m_nCols--;
	m_pColInfo[m_nCols].nCurSize = m_pColInfo[nColHide].nCurSize;
	RecalcLayout();
}

void CXTPSplitterWnd::ShowRow()
{
	ASSERT_VALID(this);
	if (m_nRows == m_nMaxRows && m_nHiddenRow == -1)
	{
		return;
	}
	ASSERT(m_nHiddenRow != -1);

	int nShowRow = m_nHiddenRow;
	m_nHiddenRow = -1;

	int cyNew = m_pRowInfo[m_nRows].nCurSize;
	m_nRows++;  // add a nRow

	ASSERT(m_nRows == m_nMaxRows);

	int nRow;

	// Show the hidden nRow
	int nCol;
	for (nCol = 0; nCol < m_nCols; ++nCol)
	{
		int nShowCol = m_nHiddenCol != -1 && m_nHiddenCol <= nCol ? nCol + 1 : nCol;
		CWnd* pPaneShow = GetDlgItem(AFX_IDW_PANE_FIRST + m_nRows * 16 + nShowCol);

		ASSERT(pPaneShow != NULL);
		if (!pPaneShow)
			return;
		pPaneShow->ShowWindow(SW_SHOWNA);

		for (nRow = m_nRows - 2; nRow >= nShowRow; --nRow)
		{
			CWnd* pPane = GetPane(nRow, nCol);
			ASSERT(pPane != NULL);
			if (!pPane)
				return;
			pPane->SetDlgCtrlID(IdFromRowCol(nRow + 1, nCol));
		}

		pPaneShow->SetDlgCtrlID(IdFromRowCol(nShowRow, nCol));
	}

	if (m_nHiddenCol != -1)
	{
		CWnd* pPaneHide = GetDlgItem(AFX_IDW_PANE_FIRST + m_nRows * 16 + m_nHiddenCol);
		if (pPaneHide)
		{
			pPaneHide->SetDlgCtrlID(AFX_IDW_PANE_FIRST + nShowRow * 16 + m_nMaxCols);
		}
	}

	// new panes have been created -- recalculate layout
	for (nRow = nShowRow + 1; nRow < m_nRows; nRow++)
		m_pRowInfo[nRow].nIdealSize = m_pRowInfo[nRow - 1].nCurSize;

	m_pRowInfo[nShowRow].nIdealSize = cyNew;
	RecalcLayout();
}

void CXTPSplitterWnd::HideRow(int nRowHide)
{
	ASSERT_VALID(this);
	if (m_nHiddenRow != -1)
	{
		// return if the requested one is hidden
		if (m_nHiddenRow == nRowHide)
		{
			return;
		}
		ShowRow();
	}
	ASSERT(m_nRows > 1);
	ASSERT(nRowHide < m_nRows);
	ASSERT(m_nHiddenRow == -1);
	m_nHiddenRow = nRowHide;

	int nActiveRow, nActiveCol;

	// if the nRow has an active window -- change it
	if (GetActivePane(&nActiveRow, &nActiveCol) != NULL)
	{
		if (nActiveRow == nRowHide)
		{
			if (++nActiveRow >= m_nRows)
				nActiveRow = 0;
			SetActivePane(nActiveRow, nActiveCol);
		}
	}

	// hide all nRow panes.
	int nCol;
	for (nCol = 0; nCol < m_nCols; ++nCol)
	{
		CWnd* pPaneHide = GetPane(nRowHide, nCol);
		ASSERT(pPaneHide != NULL);
		if (!pPaneHide)
			return;

		pPaneHide->ShowWindow(SW_HIDE);
		int nColHide = m_nHiddenCol != -1 && m_nHiddenCol <= nCol ? nCol + 1 : nCol;
		pPaneHide->SetDlgCtrlID(AFX_IDW_PANE_FIRST + m_nRows * 16 + nColHide);

		int nRow;
		for (nRow = nRowHide + 1; nRow < m_nRows; ++nRow)
		{
			CWnd* pPane = GetPane(nRow, nCol);
			ASSERT(pPane != NULL);
			if (!pPane)
				return;

			pPane->SetDlgCtrlID(IdFromRowCol(nRow-1, nCol));
		}
	}

	m_nRows--;
	m_pRowInfo[m_nRows].nCurSize = m_pRowInfo[nRowHide].nCurSize;
	RecalcLayout();
}

BOOL CXTPSplitterWnd::SwitchView(int nRow, int nCol, CView* pNewView)
{
	CView *pOldView = DYNAMIC_DOWNCAST(CView, GetPane(nRow, nCol));
	ASSERT_KINDOF(CView, pOldView);

	if (pOldView == pNewView)
		return FALSE;

	int nOldID = pOldView->GetDlgCtrlID();
	int nNewID = pNewView->GetDlgCtrlID();

	// hide the views.
	pOldView->ShowWindow(SW_HIDE);
	pNewView->ShowWindow(SW_HIDE);

	// swap ids.
	pOldView->SetDlgCtrlID(nNewID);
	pNewView->SetDlgCtrlID(nOldID);

	// show the views.
	pOldView->ShowWindow(SW_SHOW);
	pNewView->ShowWindow(SW_SHOW);

	RecalcLayout();

	return TRUE;
}

CView* CXTPSplitterWnd::ReplaceView(int nRow, int nCol, CView* pNewView)
{
	CView* pOldView = DYNAMIC_DOWNCAST(CView, GetPane (nRow, nCol));
	ASSERT_KINDOF (CView, pOldView);

	if (pOldView == pNewView)
		return NULL;

	int nCtrlID = pOldView->GetDlgCtrlID();

	// swap ids.
	pOldView->SetDlgCtrlID(0);
	pNewView->SetDlgCtrlID(nCtrlID);

	// show the views.
	pOldView->ShowWindow(SW_HIDE);
	pNewView->ShowWindow(SW_SHOW);

	RecalcLayout();

	return pOldView;
}

CView* CXTPSplitterWnd::ReplaceView(int nRow, int nCol, CRuntimeClass* pViewClass)
{
	CView* pOldView = DYNAMIC_DOWNCAST(CView, GetPane (nRow, nCol));
	ASSERT_KINDOF (CView, pOldView);

	if (pOldView->IsKindOf(pViewClass))
		return NULL;

	// Get pointer to CDocument object so that it can be used in the creation
	// process of the new view
	CDocument* pDocument = pOldView->GetDocument();

	int nCtrlID = pOldView->GetDlgCtrlID();
	int nWidth, nHeight, nMinWidth, nMinHeight;

	GetRowInfo(nRow, nWidth, nMinWidth);
	GetColumnInfo(nCol, nHeight, nMinHeight);

	// Create new view
	CCreateContext contextT;
	contextT.m_pLastView = NULL;
	contextT.m_pCurrentDoc = pDocument;
	contextT.m_pNewViewClass = pViewClass;
	contextT.m_pNewDocTemplate = pDocument ? pDocument->GetDocTemplate() : NULL;
	contextT.m_pCurrentFrame = NULL;

	CWnd* pWnd = NULL;
	TRY
	{
		pWnd = (CWnd*)pViewClass->CreateObject();
		if (pWnd == NULL)
			AfxThrowMemoryException();
	}
	CATCH_ALL(e)
	{
		TRACE0("Out of memory creating a view.\n");
		// Note: DELETE_EXCEPTION(e) not required
		return NULL;
	}
	END_CATCH_ALL

	ASSERT_KINDOF(CWnd, pWnd);
	ASSERT(pWnd->m_hWnd == NULL); // not yet created.

	// Create with the right size (wrong position)
	if (!pWnd->Create(NULL, NULL, WS_CHILD | WS_VISIBLE,
		CRect(0, 0, 0, 0), this, nCtrlID, &contextT))
	{
		TRACE0("Warning: couldn't create new view.\n");
		// pWnd will be cleaned up by PostNcDestroy
		return NULL;
	}

	// Hide the old view.
	pOldView->ShowWindow(SW_HIDE);
	pOldView->SetDlgCtrlID(0);

	SetRowInfo (nRow, nWidth, nMinWidth);
	SetColumnInfo (nCol, nHeight, nMinHeight);
	RecalcLayout ();

	CView* pNewView = DYNAMIC_DOWNCAST (CView, GetPane (nRow, nCol));
	ASSERT_KINDOF (CView, pNewView);
	pNewView->OnInitialUpdate();

	return pOldView;
}

void CXTPSplitterWnd::SetSplitCursor(int ht)
{
	if (ht == vSplitterBox ||
		ht >= vSplitterBar1 &&
		ht <= vSplitterBar15)
	{
		SetCursor(XTPAuxData().hcurVSplitBar);
	}
	else if (
		ht == hSplitterBox ||
		ht >= hSplitterBar1 &&
		ht <= hSplitterBar15)
	{
		SetCursor(XTPAuxData().hcurHSplitBar);
	}
	else
	{
		CSplitterWnd::SetSplitCursor(ht);
	}
}

BOOL CXTPSplitterWnd::FullDragWindows() const
{
	if (m_htTrack == hSplitterBox ||
		m_htTrack == vSplitterBox)
	{
		// do not allow full drag when split box is tracking.
		return FALSE;
	}

	if (m_nFullDrag == -1)
	{
		// use the system setting for full drag.
		return m_bFullDrag;
	}
	else
	{
		// user defined setting for full drag.
		return (BOOL)m_nFullDrag;
	}
}

void CXTPSplitterWnd::DrawTracker(const CRect& rect, CBrush* pBrush)
{
	// do not draw over top of horizontal split box.
	if (m_htTrack == hSplitterBox)
	{
		if (rect.left < m_cxSplitter)
			return;
	}

	// do not draw over top of vertical split box.
	if (m_htTrack == vSplitterBox)
	{
		if (rect.top < m_cySplitter)
			return;
	}

	ASSERT_VALID(this);
	ASSERT(!rect.IsRectEmpty());
	ASSERT((GetStyle() & WS_CLIPCHILDREN) == 0);

	CClientDC dc(this);
	int nSavedDC = dc.SaveDC();

	if (m_bDotTracker)
	{
		CRect rc;
		GetInsideRect(rc);

		if (rect.Width() < rect.Height())
		{
			rc.right = rect.right;
		}
		else
		{
			rc.bottom = rect.top;
		}

		dc.DrawFocusRect(rc);
	}
	else
	{
		CRect rcTracker(&rect);

		BOOL bVert = rcTracker.Height() > rcTracker.Width();

		if (bVert)
		{
			if (rcTracker.Width() != 4)
			{
				rcTracker.left = rcTracker.right - 4;
			}
		}
		else
		{
			if (rcTracker.Height() != 4)
			{
				rcTracker.bottom = rcTracker.top + 4;
			}
		}

		dc.SelectObject(pBrush);
		dc.PatBlt(rcTracker.left, rcTracker.top, rcTracker.Width(), rcTracker.Height(), PATINVERT);
	}

	dc.RestoreDC(nSavedDC);
}

void CXTPSplitterWnd::OnInvertTracker(const CRect& rect)
{
	if (!FullDragWindows())
	{
		if (m_pTheme)
		{
			DrawTracker(rect, m_pTheme->GetTrackBrush());
		}
		else
		{
			CSplitterWnd::OnInvertTracker(rect);
		}
	}
}

void CXTPSplitterWnd::OnMouseMove(UINT nFlags, CPoint pt)
{
	if (!FullDragWindows())
	{
		CSplitterWnd::OnMouseMove(nFlags, pt);
		return;
	}

	if (GetCapture() != this)
		StopTracking(FALSE);

	if (m_bTracking)
	{
		// move tracker to current cursor position

		pt.Offset(m_ptTrackOffset); // pt is the upper right of hit detect
		// limit the point to the valid split range
		if (pt.y < m_rectLimit.top)
			pt.y = m_rectLimit.top;
		else if (pt.y > m_rectLimit.bottom)
			pt.y = m_rectLimit.bottom;
		if (pt.x < m_rectLimit.left)
			pt.x = m_rectLimit.left;
		else if (pt.x > m_rectLimit.right)
			pt.x = m_rectLimit.right;

		if (m_htTrack == vSplitterBox ||
			m_htTrack >= vSplitterBar1 && m_htTrack <= vSplitterBar15)
		{
			if (m_rectTracker.top != pt.y)
			{
				OnInvertTracker(m_rectTracker);
				m_rectTracker.OffsetRect(0, pt.y - m_rectTracker.top);
				OnInvertTracker(m_rectTracker);
			}
		}
		else if (m_htTrack == hSplitterBox ||
			m_htTrack >= hSplitterBar1 && m_htTrack <= hSplitterBar15)
		{
			if (m_rectTracker.left != pt.x)
			{
				OnInvertTracker(m_rectTracker);
				m_rectTracker.OffsetRect(pt.x - m_rectTracker.left, 0);
				OnInvertTracker(m_rectTracker);
			}
		}
		else if (m_htTrack == bothSplitterBox ||
		   (m_htTrack >= splitterIntersection1 &&
			m_htTrack <= splitterIntersection225))
		{
			if (m_rectTracker.top != pt.y)
			{
				OnInvertTracker(m_rectTracker);
				m_rectTracker.OffsetRect(0, pt.y - m_rectTracker.top);
				OnInvertTracker(m_rectTracker);
			}
			if (m_rectTracker2.left != pt.x)
			{
				OnInvertTracker(m_rectTracker2);
				m_rectTracker2.OffsetRect(pt.x - m_rectTracker2.left, 0);
				OnInvertTracker(m_rectTracker2);
			}
		}

		StopTracking(TRUE);
		StartTracking(HitTest(pt));

		if (m_point != pt)
		{
			RedrawWindow(NULL, NULL,
				RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE);
			m_point = pt;
		}
	}
	else
	{
		// simply hit-test and set appropriate cursor

		int ht = HitTest(pt);
		SetSplitCursor(ht);
	}
}

void CXTPSplitterWnd::StartTracking(int ht)
{
	if (!FullDragWindows())
	{
		CSplitterWnd::StartTracking(ht);
		return;
	}

	ASSERT_VALID(this);
	if (ht == noHit)
		return;

	// GetHitRect will restrict 'm_rectLimit' as appropriate
	GetInsideRect(m_rectLimit);

	if (ht >= splitterIntersection1 && ht <= splitterIntersection225)
	{
		// split two directions (two tracking rectangles)
		int row = (ht - splitterIntersection1) / 15;
		int col = (ht - splitterIntersection1) % 15;

		GetHitRect(row + vSplitterBar1, m_rectTracker);
		int yTrackOffset = m_ptTrackOffset.y;
		m_bTracking2 = TRUE;
		GetHitRect(col + hSplitterBar1, m_rectTracker2);
		m_ptTrackOffset.y = yTrackOffset;
	}
	else if (ht == bothSplitterBox)
	{
		// hit on splitter boxes (for keyboard)
		GetHitRect(vSplitterBox, m_rectTracker);
		int yTrackOffset = m_ptTrackOffset.y;
		m_bTracking2 = TRUE;
		GetHitRect(hSplitterBox, m_rectTracker2);
		m_ptTrackOffset.y = yTrackOffset;

		// center it
		m_rectTracker.OffsetRect(0, m_rectLimit.Height()/2);
		m_rectTracker2.OffsetRect(m_rectLimit.Width()/2, 0);
	}
	else
	{
		// only hit one bar
		GetHitRect(ht, m_rectTracker);
	}

	// set capture
	SetCapture();

	// set tracking state and appropriate cursor
	m_bTracking = TRUE;
	m_htTrack = ht;
	SetSplitCursor(ht);
}

void CXTPSplitterWnd::StopTracking(BOOL bAccept)
{
	if (!FullDragWindows())
	{
		CSplitterWnd::StopTracking(bAccept);
		return;
	}

	ASSERT_VALID(this);

	if (!m_bTracking)
		return;

	ReleaseCapture();

	// erase tracker rectangle
	OnInvertTracker(m_rectTracker);
	if (m_bTracking2)
		OnInvertTracker(m_rectTracker2);
	m_bTracking = m_bTracking2 = FALSE;

	// save old active view
	CWnd* pOldActiveView = GetActivePane();

	// m_rectTracker is set to the new splitter position (without border)
	// (so, adjust relative to where the border will be)
	m_rectTracker.OffsetRect(-CX_BORDER , -CY_BORDER);
	m_rectTracker2.OffsetRect(-CX_BORDER, -CY_BORDER);

	if (bAccept)
	{
		if (m_htTrack == vSplitterBox)
		{
			SplitRow(m_rectTracker.top);
		}
		else if (m_htTrack >= vSplitterBar1 && m_htTrack <= vSplitterBar15)
		{
			// set row height
			TrackRowSize(m_rectTracker.top, m_htTrack - vSplitterBar1);
			RecalcLayout();
		}
		else if (m_htTrack == hSplitterBox)
		{
			SplitColumn(m_rectTracker.left);
		}
		else if (m_htTrack >= hSplitterBar1 && m_htTrack <= hSplitterBar15)
		{
			// set column width
			TrackColumnSize(m_rectTracker.left, m_htTrack - hSplitterBar1);
			RecalcLayout();
		}
		else if (m_htTrack >= splitterIntersection1 &&
			m_htTrack <= splitterIntersection225)
		{
			// set row height and column width
			int row = (m_htTrack - splitterIntersection1) / 15;
			int col = (m_htTrack - splitterIntersection1) % 15;

			TrackRowSize(m_rectTracker.top, row);
			TrackColumnSize(m_rectTracker2.left, col);
			RecalcLayout();
		}
		else if (m_htTrack == bothSplitterBox)
		{
			// rectTracker is vSplitter (splits rows)
			// rectTracker2 is hSplitter (splits cols)
			SplitRow(m_rectTracker.top);
			SplitColumn(m_rectTracker2.left);
		}
	}

	if (pOldActiveView == GetActivePane())
	{
		if (pOldActiveView != NULL)
		{
			SetActivePane(-1, -1, pOldActiveView); // re-activate
		}
	}
}

LRESULT CXTPSplitterWnd::OnNcHitTest(CPoint point)
{
	// If m_bNoSize is TRUE, just return a border
	// hit so can at least activate the app if needed.
	if (m_bNoSize)
	{
		return HTBORDER;
	}
	return (LRESULT)CSplitterWnd::OnNcHitTest(point);
}

void CXTPSplitterWnd::OnDrawSplitter(CDC* pDC, ESplitType nType, const CRect& rectArg)
{
	if (m_pTheme)
	{
		// if NULL just invalidate
		if (pDC == NULL)
		{
			RedrawWindow(rectArg, NULL, RDW_INVALIDATE | RDW_NOCHILDREN);
		}
		else
		{
			ASSERT_VALID(pDC);
			m_pTheme->DrawSplitter(pDC, this, nType, rectArg);
		}
	}
	else
	{
		// if no theme defined do default drawing.
		CSplitterWnd::OnDrawSplitter(pDC, nType, rectArg);
	}
}

void CXTPSplitterWnd::SetActivePane(int row, int col, CWnd* pWnd/*= NULL*/)
{
	// set the focus to the pane
	CWnd* pPane = pWnd == NULL ? GetPane(row, col) : pWnd;
	if (pPane->IsKindOf(RUNTIME_CLASS(CView)))
	{
		CFrameWnd* pFrameWnd = GetParentFrame();
		ASSERT_VALID(pFrameWnd);
		pFrameWnd->SetActiveView((CView*)pPane);
	}
	else
	{
		pPane->SetFocus();
	}
}

//-----------------------------------------------------------------------
// Note: The following functions are deprecated and could be removed
// in a future release.
//-----------------------------------------------------------------------

void CXTPSplitterWnd::SetFullDrag(BOOL bFullDrag)
{
	TRACE0("Member function is deprecated use EnableFullDrag() instead.\n");

	EnableFullDrag((int)bFullDrag);
}

void CXTPSplitterWnd::SetSplitterStyle(DWORD dwxStyle)
{
	TRACE0("CXTPSplitterWnd::SetSplitterStyle is deprecated use EnableFullDrag(), EnableDotTracker() or EnableNoSize() instead.\n");

	EnableNoSize(dwxStyle & XTP_SPLIT_NOSIZE ? TRUE : FALSE);
	EnableFullDrag(dwxStyle & XTP_SPLIT_NOFULLDRAG ? 0 : -1);
	EnableDotTracker(dwxStyle & XTP_SPLIT_DOTTRACKER ? TRUE : FALSE);
	EnableBorder(dwxStyle & XTP_SPLIT_NOBORDER ? FALSE : TRUE);
}

DWORD CXTPSplitterWnd::GetSplitterStyle()
{
	TRACE0("CXTPSplitterWnd::GetSplitterStyle is deprecated.\n");

	DWORD dwStyle = 0L;

	if (m_nFullDrag == 0)
		dwStyle |= XTP_SPLIT_NOFULLDRAG;

	if (m_bDotTracker)
		dwStyle |= XTP_SPLIT_DOTTRACKER;

	if (m_bNoSize)
		dwStyle |= XTP_SPLIT_NOSIZE;

	if (!m_bShowBorder)
		dwStyle |= XTP_SPLIT_NOBORDER;

	return dwStyle;
}

void CXTPSplitterWnd::EnableFlatLook(BOOL bFlatSplitter)
{
	TRACE0("CXTPSplitterWnd::EnableFlatLook is deprecated use SetTheme() instead.\n");

	SetTheme(bFlatSplitter ? xtpControlThemeOfficeXP : xtpControlThemeOffice2000);
}

//////////////////////////////////////////////////////////////////////
// CXTPSplitterWndEx
//

IMPLEMENT_DYNAMIC(CXTPSplitterWndEx, CXTPSplitterWnd)

BEGIN_MESSAGE_MAP(CXTPSplitterWndEx, CXTPSplitterWnd)
	//{{AFX_MSG_MAP(CXTPSplitterWndEx)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CXTPSplitterWndEx::CXTPSplitterWndEx()
{
	m_cxBorder = 0;
	m_cyBorder = 0;
	m_cxSplitter = 4;
	m_cySplitter = 4;
	m_cxSplitterGap = 4;
	m_cySplitterGap = 4;
	m_cyTopBorderGap = 6;
	m_bShowTopBorder = true;
}

CXTPSplitterWndEx::~CXTPSplitterWndEx()
{

}

void CXTPSplitterWndEx::OnDrawSplitter(CDC* pDC, ESplitType nType, const CRect& rectArg)
{
	COLORREF clr3DFace = GetTheme()->m_crBack;
	COLORREF clr3DShadow = GetTheme()->m_crInnerBorder;

	if (pDC == NULL)
	{
		RedrawWindow(rectArg, NULL, RDW_INVALIDATE | RDW_NOCHILDREN);
		return;
	}
	ASSERT_VALID(pDC);
	CRect rect = rectArg;
	switch (nType)
	{
	case splitBorder:
		return;
	case splitBox:
		pDC->Draw3dRect(rect, clr3DFace, GetXtremeColor(COLOR_WINDOWFRAME));
		rect.InflateRect(-CX_BORDER, -CY_BORDER);
		pDC->Draw3dRect(rect, GetXtremeColor(COLOR_3DHILIGHT), clr3DShadow);
		rect.InflateRect(-CX_BORDER, -CY_BORDER);
		break;
	}

	pDC->FillSolidRect(rect, clr3DFace);
}

// Generic routine:
//  for X direction: pInfo = m_pColInfo, nMax = m_nMaxCols, nSize = cx
//  for Y direction: pInfo = m_pRowInfo, nMax = m_nMaxRows, nSize = cy
void CXTPSplitterWndEx::LayoutRowCol(CSplitterWnd::CRowColInfo* pInfoArray,
	int nMax, int nSize, int nSizeSplitter)
{
	ASSERT(pInfoArray != NULL);
	ASSERT(nMax > 0);
	ASSERT(nSizeSplitter > 0);

	if (!pInfoArray)
		return;

	CSplitterWnd::CRowColInfo* pInfo = NULL;
	int i;

	if (nSize < 0)
		nSize = 0;  // if really too small, layout as zero size

	// start with ideal sizes
	for (i = 0, pInfo = pInfoArray; i < nMax-1; i++, pInfo++)
	{
		if (pInfo->nIdealSize < pInfo->nMinSize)
			pInfo->nIdealSize = 0;      // too small to see
		pInfo->nCurSize = pInfo->nIdealSize;
	}
	if (!pInfo)
		return;

	pInfo->nCurSize = INT_MAX;  // last row/column takes the rest

	for (i = 0, pInfo = pInfoArray; i < nMax; i++, pInfo++)
	{
		ASSERT(nSize >= 0);
		if (nSize == 0)
		{
			// no more room (set pane to be invisible)
			pInfo->nCurSize = 0;
			continue;       // don't worry about splitters
		}
		else if (nSize < pInfo->nMinSize && i != 0)
		{
			// additional panes below the recommended minimum size
			//   aren't shown and the size goes to the previous pane
			pInfo->nCurSize = 0;

			// previous pane already has room for splitter + border
			//   add remaining size and remove the extra border
			ASSERT(CX_BORDER * 2 == CY_BORDER * 2);
			(pInfo-1)->nCurSize += nSize + CX_BORDER * 2;
			nSize = 0;
		}
		else
		{
			// otherwise we can add the second pane
			ASSERT(nSize > 0);
			if (pInfo->nCurSize == 0)
			{
				// too small to see
				if (i != 0)
					pInfo->nCurSize = 0;
			}
			else if (nSize < pInfo->nCurSize)
			{
				// this row/col won't fit completely - make as small as possible
				pInfo->nCurSize = nSize;
				nSize = 0;
			}
			else
			{
				// can fit everything
				nSize -= pInfo->nCurSize;
			}
		}

		// see if we should add a splitter
		ASSERT(nSize >= 0);
		if (i != nMax - 1)
		{
			// should have a splitter
			if (nSize > nSizeSplitter)
			{
				nSize -= nSizeSplitter; // leave room for splitter + border
				ASSERT(nSize > 0);
			}
			else
			{
				// not enough room - add left over less splitter size
				ASSERT(CX_BORDER * 2 == CY_BORDER * 2);
				pInfo->nCurSize += nSize;
				if (pInfo->nCurSize > (nSizeSplitter - CX_BORDER * 2))
					pInfo->nCurSize -= (nSizeSplitter - CY_BORDER * 2);
				nSize = 0;
			}
		}
	}
	ASSERT(nSize == 0); // all space should be allocated
}

// repositions client area of specified window
// assumes everything has WS_BORDER or is inset like it does
//  (includes scroll bars)
void CXTPSplitterWndEx::DeferClientPos(AFX_SIZEPARENTPARAMS* lpLayout,
	CWnd* pWnd, int x, int y, int cx, int cy, BOOL bScrollBar)
{
	ASSERT(pWnd != NULL);
	if (!pWnd)
		return;
	ASSERT(pWnd->m_hWnd != NULL);

	if (bScrollBar)
	{
		// if there is enough room, draw scroll bar without border
		// if there is not enough room, set the WS_BORDER bit so that
		//   we will at least get a proper border drawn
		BOOL bNeedBorder = (cx <= CX_BORDER || cy <= CY_BORDER);
		pWnd->ModifyStyle(bNeedBorder ? 0 : WS_BORDER,
			bNeedBorder ? WS_BORDER : 0);
	}
	CRect rect(x, y, x + cx, y + cy);

	// adjust for border size (even if zero client size)

	// adjust for 3d border (splitter windows have implied border)
	if ((pWnd->GetExStyle() & WS_EX_CLIENTEDGE) ||
		pWnd->IsKindOf(RUNTIME_CLASS(CSplitterWnd)))
		rect.InflateRect(CX_BORDER * 2, CY_BORDER * 2);

	// first check if the new rectangle is the same as the current
	CRect rectOld;
	pWnd->GetWindowRect(rectOld);
	pWnd->GetParent()->ScreenToClient(&rectOld);
	if (rect != rectOld)
		AfxRepositionWindow(lpLayout, pWnd->m_hWnd, rect);
}

void CXTPSplitterWndEx::RecalcLayout()
{
	ASSERT_VALID(this);
	ASSERT(m_nRows > 0 && m_nCols > 0); // must have at least one pane

	CRect rectClient;
	GetClientRect(rectClient);
	rectClient.InflateRect(-m_cxBorder, -m_cyBorder);
	rectClient.top += m_cyTopBorderGap;

	CRect rectInside;
	GetInsideRect(rectInside);

	// layout columns (restrict to possible sizes)
	LayoutRowCol(m_pColInfo, m_nCols, rectInside.Width(), m_cxSplitterGap);
	LayoutRowCol(m_pRowInfo, m_nRows, rectInside.Height(), m_cySplitterGap);

	// adjust the panes (and optionally scroll bars)

	// give the hint for the maximum number of HWNDs
	AFX_SIZEPARENTPARAMS layout;
	layout.hDWP = ::BeginDeferWindowPos((m_nCols + 1) * (m_nRows + 1) + 1);

	// size of scrollbars
	int cx = (rectClient.right - rectInside.right);
	int cy = (rectClient.bottom - rectInside.bottom);

	// reposition size box
	if (m_bHasHScroll && m_bHasVScroll)
	{
		CWnd* pScrollBar = GetDlgItem(AFX_IDW_SIZE_BOX);
		ASSERT(pScrollBar != NULL);

		// fix style if necessary
		BOOL bSizingParent = (GetSizingParent() != NULL);
		// modifyStyle returns TRUE if style changes
		if (pScrollBar->ModifyStyle(SBS_SIZEGRIP | SBS_SIZEBOX,
				bSizingParent ? SBS_SIZEGRIP : SBS_SIZEBOX))
			pScrollBar->Invalidate();
		pScrollBar->EnableWindow(bSizingParent);

		// reposition the size box
		DeferClientPos(&layout, pScrollBar,
			rectInside.right, rectInside.bottom, cx, cy, TRUE);
	}

	// reposition scroll bars
	if (m_bHasHScroll)
	{
		int cxSplitterBox = m_cxSplitter;// split box bigger
		int x = rectClient.left;
		int y = rectInside.bottom;
		int col;
		for (col = 0; col < m_nCols; col++)
		{
			CWnd* pScrollBar = GetDlgItem(AFX_IDW_HSCROLL_FIRST + col);
			ASSERT(pScrollBar != NULL);
			int cxColumn = m_pColInfo[col].nCurSize;
			if (col == 0 && m_nCols < m_nMaxCols)
				x += cxSplitterBox, cxColumn -= cxSplitterBox;
			DeferClientPos(&layout, pScrollBar, x, y, cxColumn, cy, TRUE);
			x += cxColumn + m_cxSplitterGap;
		}
	}

	if (m_bHasVScroll)
	{
		int cySplitterBox = m_cySplitter;// split box bigger
		int x = rectInside.right;
		int y = rectClient.top;
		int row;
		for (row = 0; row < m_nRows; row++)
		{
			CWnd* pScrollBar = GetDlgItem(AFX_IDW_VSCROLL_FIRST + row);
			ASSERT(pScrollBar != NULL);
			int cyColumn = m_pRowInfo[row].nCurSize;
			if (row == 0 && m_nRows < m_nMaxRows)
				y += cySplitterBox, cyColumn -= cySplitterBox;
			DeferClientPos(&layout, pScrollBar, x, y, cx, cyColumn, TRUE);
			y += cyColumn + m_cySplitterGap;
		}
	}

	//BLOCK: Reposition all the panes
	{
		int x = rectClient.left;
		int col;
		for (col = 0; col < m_nCols; col++)
		{
			int cxColumn = m_pColInfo[col].nCurSize;
			int y = rectClient.top;
			int row;
			for (row = 0; row < m_nRows; row++)
			{
				int cyColumn = m_pRowInfo[row].nCurSize;
				CWnd* pWnd = GetPane(row, col);
				DeferClientPos(&layout, pWnd, x, y, cxColumn, cyColumn, FALSE);
				y += cyColumn + m_cySplitterGap;
			}
			x += cxColumn + m_cxSplitterGap;
		}
	}

	// move and resize all the windows at once!
	if (layout.hDWP == NULL || !::EndDeferWindowPos(layout.hDWP))
		TRACE0("Warning: DeferWindowPos failed - low system resources.\n");

	// invalidate all the splitter bars (with NULL pDC)
	DrawAllSplitBars(NULL, rectInside.right, rectInside.bottom);
}

void CXTPSplitterWndEx::GetInsideRect(CRect& rect) const
{
	CSplitterWnd::GetInsideRect(rect);
	rect.top += m_cyTopBorderGap;
}

void CXTPSplitterWndEx::OnPaint()
{
	CSplitterWnd::OnPaint();

	if (m_bShowTopBorder)
	{
		CClientDC dc(this);

		int iSavedDC = dc.SaveDC();

		CRect rectClient;
		GetClientRect(rectClient);

		if (m_pTheme->GetThemeStyle() == xtpControlThemeDefault)
		{
			CPen penShadow(PS_SOLID, 1, GetXtremeColor(COLOR_3DSHADOW));
			dc.SelectObject(&penShadow);
			dc.MoveTo(rectClient.left, rectClient.top);
			dc.LineTo(rectClient.right, rectClient.top);
		}

		CPen penHilight(PS_SOLID, 1, GetXtremeColor(COLOR_3DHILIGHT));
		dc.SelectObject(&penHilight);
		dc.MoveTo(rectClient.left, rectClient.top + 1);
		dc.LineTo(rectClient.right, rectClient.top + 1);

		dc.RestoreDC(iSavedDC);
	}
}

BOOL CXTPSplitterWndEx::OnEraseBkgnd(CDC* pDC)
{
	CRect rectClient;
	GetClientRect(rectClient);
	rectClient.InflateRect(-m_cxBorder, -m_cyBorder);
	rectClient.bottom = rectClient.top + m_cyTopBorderGap;

	COLORREF clr3DFace = GetTheme()->m_crBack;
	pDC->FillSolidRect(rectClient, clr3DFace);

	return TRUE;
}
