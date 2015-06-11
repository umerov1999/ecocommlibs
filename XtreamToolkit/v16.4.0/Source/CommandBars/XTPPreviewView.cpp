// XTPPreviewView.cpp : implementation file
//
// This file is a part of the XTREME TOOLKIT PRO MFC class library.
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
#include "Resource.h"

#include "Common/XTPDrawHelpers.h"
#include "Common/XTPResourceManager.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPHookManager.h"
#include <Common/ScrollBar/XTPScrollInfo.h>

#include "XTPCommandBarsDefines.h"
#include "XTPCommandBar.h"
#include "XTPToolBar.h"
#include "XTPPopupBar.h"
#include "XTPControls.h"
#include "XTPControl.h"
#include "XTPControlButton.h"
#include "XTPControlPopup.h"
#include "XTPControlEdit.h"
#include "XTPControlComboBox.h"
#include "XTPControlExt.h"
#include "XTPPreviewView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPControlPageSelector

class CXTPControlPageSelector : public CXTPControlSelector
{
public:
	CXTPControlPageSelector();
	void OnDrawItem(CDC* pDC, CRect rcItem, COLORREF clrText);
};

CXTPControlPageSelector::CXTPControlPageSelector()
{

	m_szItemsCount = CSize(3, 3);
	m_szItemsMax = CSize(3, 3);
	m_szItem = CSize(32, 32);
}


void CXTPControlPageSelector::OnDrawItem(CDC* pDC, CRect rcItem, COLORREF clrText)
{
	rcItem.DeflateRect(3, 4, 3, 4);

	for (int y = rcItem.top, i = 0; y < rcItem.bottom; y += 3, i++)
	{
		pDC->FillSolidRect(rcItem.left, y, (i + 1) % 5 == 0? rcItem.Width() / 2: rcItem.Width(), 1, clrText);
	}

}


/////////////////////////////////////////////////////////////////////////////
// CXTPPreviewView

IMPLEMENT_DYNCREATE(CXTPPreviewView, CPreviewView)

CXTPPreviewView::CXTPPreviewView(int nPageCount /*=9*/)
{
	// replace the PAGE_INFO array with specified page count.
	m_pPageInfo2 = new PAGE_INFO[nPageCount];
	m_pPageInfo = m_pPageInfo2;

	m_Across = 1; // default number of pages across the screen
	m_Down = 1; // default number of pages down the screen
	m_nPages = 1;
	m_nZoomOutPages = 1;
	AfxGetApp()->m_nNumPreviewPages = 1;

	CWindowDC dc(NULL);
	m_cxPixelsPerInch = ::GetDeviceCaps(dc, LOGPIXELSX);
	m_cyPixelsPerInch = ::GetDeviceCaps(dc, LOGPIXELSY);

	m_clrBackColor = GetSysColor(COLOR_APPWORKSPACE);

	m_pCommandBar = new CXTPToolBar();
}

CXTPPreviewView::~CXTPPreviewView()
{
	SAFE_DELETE_AR(m_pPageInfo2);
	CMDTARGET_RELEASE(m_pCommandBar);
}

BEGIN_MESSAGE_MAP(CXTPPreviewView, CPreviewView)
	//{{AFX_MSG_MAP(CXTPPreviewView)
	ON_WM_CREATE()
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_SETCURSOR()
	ON_WM_VSCROLL()
	ON_WM_SIZE()

	ON_COMMAND(AFX_ID_PREVIEW_NUMPAGE, OnNumPageChange)
	ON_COMMAND(XTP_ID_PREVIEW_ZOOMIN, OnZoomIn)
	ON_COMMAND(XTP_ID_PREVIEW_ZOOMOUT, OnZoomOut)
	ON_COMMAND(XTP_ID_PREVIEW_CLOSE, CPreviewView::OnPreviewClose)
	ON_COMMAND(XTP_ID_PREVIEW_NEXT, CPreviewView::OnNextPage)
	ON_COMMAND(XTP_ID_PREVIEW_PREV, CPreviewView::OnPrevPage)
	ON_COMMAND(XTP_ID_PREVIEW_PRINT, CPreviewView::OnPreviewPrint)

	ON_COMMAND(XTP_ID_PREVIEW_ONEPAGE, OnOnePage)

	ON_UPDATE_COMMAND_UI(AFX_ID_PREVIEW_NUMPAGE, OnUpdateNumPageChange)
	ON_UPDATE_COMMAND_UI(XTP_ID_PREVIEW_ZOOMIN, OnUpdateZoomIn)
	ON_UPDATE_COMMAND_UI(XTP_ID_PREVIEW_ZOOMOUT, OnUpdateZoomOut)
	ON_UPDATE_COMMAND_UI(XTP_ID_PREVIEW_NEXT, CPreviewView::OnUpdateNextPage)
	ON_UPDATE_COMMAND_UI(XTP_ID_PREVIEW_PREV, CPreviewView::OnUpdatePrevPage)

	ON_NOTIFY(EN_SETFOCUS, XTP_ID_PREVIEW_GOTOPAGE, OnGotoPageSetFocus)
	ON_NOTIFY(EN_KILLFOCUS, XTP_ID_PREVIEW_GOTOPAGE, OnGotoPageKillFocus)
	ON_UPDATE_COMMAND_UI(XTP_ID_PREVIEW_GOTOPAGE, OnUpdateGotoPage)
	ON_XTP_EXECUTE(XTP_ID_PREVIEW_ZOOMCOMBO, OnZoomCombo)
	ON_XTP_EXECUTE(XTP_ID_PREVIEW_TWOPAGE, OnTwoPage)

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

UINT nDefaultZooms[] = {10, 25, 50, 75, 100, 150, 200, 500};

int CXTPPreviewView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CPreviewView::OnCreate(lpCreateStruct) == -1)
		return -1;

	ASSERT_VALID (m_pToolBar); // dialog bar must be valid.

	// Create the toolbar that is used in the dialog bar.
	if (!m_pCommandBar->CreateToolBar( WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS, m_pToolBar) ||
		!m_pCommandBar->LoadToolBar( XTP_IDR_PREVIEW_TOOLBAR ) )
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	m_pCommandBar->SetOwner(this);

	// display text for print and close buttons.
	FindToolbarButton(XTP_ID_PREVIEW_PRINT)->SetStyle(xtpButtonIconAndCaption);
	FindToolbarButton(XTP_ID_PREVIEW_CLOSE)->SetStyle(xtpButtonIconAndCaption);


	CXTPControlPopup* pTwoPages = (CXTPControlPopup*)m_pCommandBar->GetControls()->SetControlType(FindToolbarButton(XTP_ID_PREVIEW_TWOPAGE), xtpControlButtonPopup);
	pTwoPages->SetFlags(xtpFlagManualUpdate);

	pTwoPages->GetCommandBar()->GetControls()->Add(new CXTPControlPageSelector(), XTP_ID_PREVIEW_TWOPAGE);
	pTwoPages->GetCommandBar()->SetShowGripper(FALSE);


	CXTPControlEdit* pControlGotoPage = (CXTPControlEdit*)m_pCommandBar->GetControls()->SetControlType(FindToolbarButton(XTP_ID_PREVIEW_GOTOPAGE), xtpControlEdit);

	pControlGotoPage->SetWidth(100);

	CXTPControlComboBox* pControlZoom = (CXTPControlComboBox*)m_pCommandBar->GetControls()->SetControlType(FindToolbarButton(XTP_ID_PREVIEW_ZOOMCOMBO), xtpControlComboBox);
	pControlZoom->SetFlags(xtpFlagManualUpdate);

	pControlZoom->SetWidth(100);
	pControlZoom->SetDropDownListStyle();

	pControlZoom->AddString(XTPResourceManager()->LoadString(XTP_IDS_PREVIEW_WHOLEPAGE));
	pControlZoom->AddString(XTPResourceManager()->LoadString(XTP_IDS_PREVIEW_PAGEWIDTH));

	for (int i = 0; i < _countof(nDefaultZooms); i++)
	{
		CString strZoom;
		strZoom.Format(_T("%i%%"), nDefaultZooms[i]);

		int nIndex = pControlZoom->AddString(strZoom);
		pControlZoom->SetItemData(nIndex, nDefaultZooms[i]);
	}

	pControlZoom->SetCurSel(0);

	return 0;
}

CXTPControl* CXTPPreviewView::FindToolbarButton(UINT nCommandID)
{
	return m_pCommandBar->GetControls()->FindControl(nCommandID);
}

void CXTPPreviewView::RecalLayout()
{
	// Get a pointer to the parent frame.
	CFrameWnd* pFrameWnd = m_pToolBar->GetDockingFrame( );
	ASSERT_VALID( pFrameWnd );

	// Get the size of the client area for the docking frame.
	CRect r;
	pFrameWnd->GetClientRect( &r );

	// Calculate the actual size of the toolbar.
	CSize size(0, 0);

	if (m_pCommandBar->IsVisible())
		size = m_pCommandBar->CalcDockingLayout(r.Width(), /*LM_HIDEWRAP| */ LM_HORZDOCK | LM_HORZ | LM_COMMIT);

	size.cx = r.Width( );

	// Set the height of the dialog bar equal to the
	// toolbar's height.
	m_pToolBar->m_sizeDefault.cy = size.cy;

	// Reposition the toolbar.
	m_pCommandBar->MoveWindow( 0, 0, size.cx, size.cy );
	m_pCommandBar->Invalidate(FALSE);
}

void CXTPPreviewView::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos)
{
	CPreviewView::OnWindowPosChanged(lpwndpos);

	// If the toolbar is a window...
	if ( ::IsWindow( m_pCommandBar->m_hWnd ) )
	{
		RecalLayout();
	}

	Invalidate(FALSE);
}

void CXTPPreviewView::OnPaint()
{
	// standard paint routine
	CPaintDC dc(this);

	CXTPClientRect rc(this);
	CXTPBufferDC memDC(dc, rc);

	// Fill background with APPWORKSPACE
	memDC.FillSolidRect(rc, m_clrBackColor);

	int nMapMode = memDC.GetMapMode();
	OnPrepareDC(&memDC);

	OnDraw(&memDC);

	memDC.SetViewportOrg(CPoint(0, 0));
	memDC.SetMapMode(nMapMode);
}

BOOL CXTPPreviewView::OnEraseBkgnd(CDC*)
{
	return TRUE;
}

BOOL CXTPPreviewView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CPreviewView::PreCreateWindow(cs);
}

#ifdef _DEBUG
void CXTPPreviewView::AssertValid() const
{
	// Bug nearby Article ID: Q192853
	//  CPreviewView::AssertValid();
}

void CXTPPreviewView::Dump(CDumpContext& dc) const
{
	CPreviewView::Dump(dc);
}
#endif //_DEBUG

BOOL CXTPPreviewView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	if (nHitTest != HTCLIENT)
		return CScrollView::OnSetCursor(pWnd, nHitTest, message);

	CPoint point;
	::GetCursorPos(&point);
	ScreenToClient(&point);     // client coordinates of mouse position

	UINT nPage;
	if (m_nZoomState < 500 &&
		FindPageRect(point, nPage))
	{                       // On a page and not zoomed all the way in
		if (m_hMagnifyCursor == NULL)
		{
			HINSTANCE hInst = AfxFindResourceHandle(
				MAKEINTRESOURCE(AFX_IDC_MAGNIFY), RT_GROUP_CURSOR);
			m_hMagnifyCursor = ::LoadCursor(hInst,
				MAKEINTRESOURCE(AFX_IDC_MAGNIFY));
		}
		::SetCursor(m_hMagnifyCursor);
	}
	else
	{
		::SetCursor(::LoadCursor(NULL, IDC_ARROW));
	}
	return 0;
}

void CXTPPreviewView::OnLButtonDown(UINT, CPoint point)
{
	UINT nPage;
	if (!FindPageRect(point, nPage))
	{
		return;                         // Didn't click on a page
	}

	// Set new zoom state
	ZoomIn(nPage, point);

	if (m_nZoomState == XTP_ZOOM_WHOLEPAGE)
	{
		// make sure the current page does not cause blank pages to be shown
		if (m_nCurrentPage >= m_pPreviewInfo->GetMaxPage() - m_nPages)
			m_nCurrentPage = m_pPreviewInfo->GetMaxPage() - m_nPages + 1;
		if (m_nCurrentPage < 0)
			m_nCurrentPage = 0 ;
	}
}

void CXTPPreviewView::SetZoomState(UINT nNewState, UINT nPage, CPoint point)
{
	if (m_nZoomState != nNewState)
	{
		CXTPControlComboBox* pControlCombo = (CXTPControlComboBox*)FindToolbarButton(XTP_ID_PREVIEW_ZOOMCOMBO);
		if (nNewState == XTP_ZOOM_WHOLEPAGE)
		{
			pControlCombo->SetCurSel(0);
		}
		else if (nNewState == XTP_ZOOM_PAGEWIDTH)
		{
			pControlCombo->SetCurSel(1);
		}
		else
		{
			CString str;
			str.Format(_T("%i%%"), nNewState);

			pControlCombo->SetCurSel(-1);
			pControlCombo->SetEditText(str);
		}


		m_nZoomState = nNewState;
		DoZoom(nPage, point);
	}
}

// Actual zoom code.
void CXTPPreviewView::DoZoom(UINT nPage, CPoint point)
{
	if (m_nZoomState == XTP_ZOOM_WHOLEPAGE)
	{
		// taking over scroll bars
		m_nPages = m_nZoomOutPages;
		ShowScrollBar(SB_HORZ, FALSE);      // hide the horizontal bar

		SCROLLINFO info;
		info.fMask = SIF_PAGE | SIF_RANGE | SIF_POS;
		info.nMin = 0;
		info.nMax = 0;
		info.nPage = 1;
		info.nPos = 0;
		SetScrollInfo(SB_HORZ, &info, FALSE);

		BOOL bShowBar = m_pPreviewInfo->GetMaxPage() < 0x8000 &&
			m_pPreviewInfo->GetMaxPage() -
			m_pPreviewInfo->GetMinPage() <= 32767U;

		ShowScrollBar(SB_VERT, bShowBar);       // Show the vertical bar

		if (bShowBar)
		{
			info.fMask = SIF_PAGE | SIF_RANGE;
			info.nMin = m_pPreviewInfo->GetMinPage();
			info.nMax = m_pPreviewInfo->GetMaxPage();
			info.nPage = 1;
			if (!SetScrollInfo(SB_VERT, &info, FALSE))
				SetScrollRange(SB_VERT, info.nMin, info.nMax, FALSE);
		}

		SetCurrentPage(m_nCurrentPage, TRUE);
		SetupScrollbar() ;
	}
	else
	{
		ShowScrollBar(SB_HORZ, FALSE);

		m_nPages = 1;       // only one page in zoomed states
		m_pPageInfo[0].sizeZoomOutRatio = m_pPageInfo[nPage].sizeZoomOutRatio;
		m_pPageInfo[0].sizeUnscaled = m_pPageInfo[nPage].sizeUnscaled;

		// Sets the printer page
		SetCurrentPage(m_nCurrentPage + nPage, FALSE);

		SetScaledSize(0);

		CSize* pRatio = &m_pPageInfo[nPage].sizeScaleRatio;

		// convert Hit Point from screen 1:1
		point.x = MulDiv(point.x, pRatio->cx, pRatio->cy);
		point.y = MulDiv(point.y, pRatio->cx, pRatio->cy);

		// Adjust point for page position
		point += (CSize)m_pPageInfo[0].rectScreen.TopLeft();

		// Scroll to center
		CenterOnPoint(point);
	}
}

void CXTPPreviewView::PositionPage(UINT nPage)
{

	CSize windowSize = CalcPageDisplaySize();

	VERIFY(m_dcPrint.Escape(GETPHYSPAGESIZE, 0, NULL, (LPVOID)&m_pPageInfo[nPage].sizeUnscaled));

	CSize* pSize = &m_pPageInfo[nPage].sizeUnscaled;

	// Convert page size to screen coordinates
	pSize->cx = MulDiv(pSize->cx, m_cxPixelsPerInch, m_sizePrinterPPI.cx);
	pSize->cy = MulDiv(pSize->cy, m_cyPixelsPerInch, m_sizePrinterPPI.cy);

	m_pPageInfo[nPage].sizeZoomOutRatio = CalcScaleRatio(windowSize, *pSize);

	SetScaledSize(nPage);
}

#define PREVIEW_MARGIN  8
#define PREVIEW_PAGEGAP 8

void CXTPPreviewView::SetScaledSize(UINT nPage)
{
	CSize* pSize = &m_pPageInfo[nPage].sizeUnscaled;
	CSize* pRatio = &m_pPageInfo[nPage].sizeScaleRatio;
	CSize* pZoomOutRatio = &m_pPageInfo[nPage].sizeZoomOutRatio;
	CSize windowSize = CalcPageDisplaySize();
	BOOL bPaperLarger = pZoomOutRatio->cx < pZoomOutRatio->cy;
	// whether the paper is larger than the screen, or vice versa

	switch (m_nZoomState)
	{
		case XTP_ZOOM_WHOLEPAGE:
			*pRatio = *pZoomOutRatio;
			break;

		case XTP_ZOOM_PAGEWIDTH:
			m_Across = 1;
			m_Down = 1;
			m_nPages = 1;
			m_nZoomOutPages = m_nPages;
			m_nMaxPages = 1;

			pRatio->cx = windowSize.cx - PREVIEW_MARGIN * 2 - 6;
			pRatio->cy = pSize->cx;
			break;

		case 100:
			if (bPaperLarger)
			{
				pRatio->cx = pRatio->cy = 1;
			}
			else
			{
				// if the paper is smaller than the screen space we're displaying
				// it in, then using a ratio of 1/1 will result in a smaller image
				// on the screen, not a larger one. To get a larger image in this
				// case we double the zoom out ratio.
				pRatio->cy = pZoomOutRatio->cy;
				pRatio->cx = 2*pZoomOutRatio->cx - pZoomOutRatio->cy;
			}

			break;

		default:
			pRatio->cx = m_nZoomState;
			pRatio->cy = 100;
	}

	// Convert to scaled size
	CSize scaledSize;
	scaledSize.cx = MulDiv(pSize->cx, pRatio->cx, pRatio->cy);
	scaledSize.cy = MulDiv(pSize->cy, pRatio->cx, pRatio->cy);

	CRect* pRect = &m_pPageInfo[nPage].rectScreen;
	pRect->SetRect(PREVIEW_MARGIN, PREVIEW_MARGIN,
		scaledSize.cx + PREVIEW_MARGIN + 3,
		scaledSize.cy + PREVIEW_MARGIN + 3);

	if (m_nZoomState == XTP_ZOOM_WHOLEPAGE)
	{
		pRect->OffsetRect((windowSize.cx - pRect->Size().cx) / 2 - 1, (windowSize.cy - pRect->Size().cy) / 2 - 1);

		// we need to offset the page multiple times
		int local = nPage % m_Across ;
		while (local-- >= 1)
		{
			pRect->OffsetRect(m_PageOffset.x, 0);
		}
		local = nPage / m_Across ;
		while (local-- > 0)
		{
			pRect->OffsetRect(0, m_PageOffset.y);
		}
	}
	else
	{
		// set up scroll size
		SetScrollSizes(MM_TEXT, pRect->Size() +
			CSize(PREVIEW_MARGIN * 2, PREVIEW_MARGIN * 2), windowSize);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CXTPPreviewView

void CXTPPreviewView::OnUpdateNumPageChange(CCmdUI* /*pCmdUI*/)
{
	// button has been removed from toolbar
	//UINT nPages = m_nZoomState == XTP_ZOOM_WHOLEPAGE ? m_nPages : m_nZoomOutPages;
	//pCmdUI->Enable(m_nZoomState == XTP_ZOOM_WHOLEPAGE && m_nMaxPages != 1 && (m_pPreviewInfo->GetMaxPage() > 1 || m_nPages > 1));
	//  CPreviewView::OnUpdateNumPageChange(pCmdUI);
}

void CXTPPreviewView::OnUpdateZoomIn(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_nZoomState < 500);
}

void CXTPPreviewView::OnUpdateZoomOut(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_nZoomState > 10);
}

void CXTPPreviewView::OnNumPageChange()
{
	// doesn't do anything any more
	// button has been removed from toolbar
}

void CXTPPreviewView::OnZoomIn()
{
	ZoomIn(0, CPoint(0, 0));
}

void CXTPPreviewView::ZoomIn(UINT nPage, CPoint point)
{
	if (m_nZoomState == XTP_ZOOM_WHOLEPAGE || m_nZoomState == XTP_ZOOM_PAGEWIDTH)
	{
		SetZoomState(100, nPage, point);
		return;
	}

	for (int i = 0; i < _countof(nDefaultZooms); i++)
	{
		if (m_nZoomState < nDefaultZooms[i])
		{
			SetZoomState(nDefaultZooms[i], nPage, point);
			return;
		}
	}
}

void CXTPPreviewView::OnZoomOut()
{
	for (int i = _countof(nDefaultZooms) - 1; i >= 0; i--)
	{
		if (m_nZoomState > nDefaultZooms[i])
		{
			SetZoomState(nDefaultZooms[i], 0, CPoint(0, 0));
			return;
		}
	}
}

CSize CXTPPreviewView::CalcPageDisplaySize()
	// calculate the current page size
	// MFC used to set 'm_nSecondPageOffset' to start of second page
	// as we have multiple pages we use m_PageOffset which holds the
	// differences across and down the pages
	// return size of current page less margins
{
	// just checking...
	ASSERT(m_Down >= 1) ;
	ASSERT(m_Across >= 1) ;
	ASSERT(m_nPages >= 1) ;

	CSize windowSize, scrollSize;
	GetTrueClientSize(windowSize, scrollSize);

	// subtract out vertical scrollbar if zoomed out and page range is known
	// and there is more than one page.
	if (m_nZoomState == XTP_ZOOM_WHOLEPAGE && (m_pPreviewInfo->GetMaxPage() != 0xffff) && (m_pPreviewInfo->GetMaxPage() - m_pPreviewInfo->GetMinPage() != 0))
		windowSize.cx -= scrollSize.cx;
	m_PageOffset.y = 0 ;
	if (m_Down > 1)
		{
		// we need to make room for more pages under the first
		windowSize.cy = (windowSize.cy - (PREVIEW_MARGIN * (m_Down - 1))) / m_Down ;
		m_PageOffset.y = windowSize.cy + PREVIEW_MARGIN ;
		}
	else
		{
		// its a single page down, it uses all the area previously calculated
		}

	if (m_Across <= 2)
		m_PageOffset.x = (windowSize.cx - PREVIEW_MARGIN) / 2;
	else
		m_PageOffset.x = (windowSize.cx - PREVIEW_MARGIN) / m_Across ;

	// make sure all pages across fit in the screen area
	windowSize.cx = (windowSize.cx - ((m_Across + 1) * PREVIEW_MARGIN)) / m_Across ;
	//windowSize.cx = (m_nPages == 2) ? (windowSize.cx - 3*PREVIEW_MARGIN) / 2 :
	//                              windowSize.cx - 2*PREVIEW_MARGIN;

	windowSize.cy -= 2*PREVIEW_MARGIN;
	return windowSize;
}

void CXTPPreviewView::SetupScrollbar()
{
	// this procedure makes sure that the scroll bar does not allow us to scroll the window
	// such that we end up displaying blank pages
	// correctly range the scroll bars
	if (m_pPreviewInfo->GetMaxPage() < 0x8000 && m_pPreviewInfo->GetMaxPage() - m_pPreviewInfo->GetMinPage() <= 32767U)
	{
		SCROLLINFO info;
		info.fMask = SIF_PAGE|SIF_RANGE;
		info.nMin = m_pPreviewInfo->GetMinPage();
		info.nMax = m_pPreviewInfo->GetMaxPage() - (m_nPages - 1) ;
		info.nPage = 1; // correct? MSDN: Specifies the page size, in device units

		if (info.nMin == info.nMax == 1)
			return; // prevent scrollbar from hiding. Mantis issue: 214

		if (!SetScrollInfo(SB_VERT, &info, FALSE))
			SetScrollRange(SB_VERT, info.nMin, info.nMax, FALSE);
	}
	else
		ShowScrollBar(SB_VERT, FALSE);      // if no range specified, or too
}

void CXTPPreviewView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (m_nZoomState != XTP_ZOOM_WHOLEPAGE)
	{
		CScrollView::OnVScroll(nSBCode, nPos, pScrollBar);
		return;
	}
	SetupScrollbar() ;
	switch (nSBCode)
	{
	case SB_BOTTOM:
		SetCurrentPage(m_pPreviewInfo->GetMaxPage(), TRUE);
		break;

	case SB_TOP:
		SetCurrentPage(m_pPreviewInfo->GetMinPage(), TRUE);
		break;

	case SB_PAGEDOWN:
		SetCurrentPage(m_nCurrentPage +
			(m_pPreviewInfo->GetMaxPage() - m_pPreviewInfo->GetMinPage() + 9) / 10, TRUE);
		break;

	case SB_PAGEUP:
		SetCurrentPage(m_nCurrentPage -
			(m_pPreviewInfo->GetMaxPage() - m_pPreviewInfo->GetMinPage() + 9) / 10, TRUE);
		break;

	case SB_LINEDOWN:
		if (m_nCurrentPage <= m_pPreviewInfo->GetMaxPage() - m_nPages)
			SetCurrentPage(m_nCurrentPage + 1, TRUE);
		break;

	case SB_LINEUP:
		if (m_nCurrentPage > 0)
			SetCurrentPage(m_nCurrentPage - 1, TRUE);
		break;

	case SB_THUMBPOSITION:
		SetCurrentPage(nPos, TRUE);
		break;
	}
}

void CXTPPreviewView::OnGotoPageSetFocus(NMHDR* pNMHDR, LRESULT* /*pRes*/)
{
	NMXTPCONTROL* tagNMCONTROL = (NMXTPCONTROL*)pNMHDR;
	if (!tagNMCONTROL->pControl->GetEnabled())
		return;

	((CXTPControlEdit*)tagNMCONTROL->pControl)->SetEditText(_T(""));

}

void CXTPPreviewView::OnGotoPageKillFocus(NMHDR* pNMHDR, LRESULT* /*pRes*/)
{
	NMXTPCONTROL* tagNMCONTROL = (NMXTPCONTROL*)pNMHDR;

	if (!tagNMCONTROL->pControl->GetEnabled())
		return;

	CString strText = ((CXTPControlEdit*)tagNMCONTROL->pControl)->GetEditText();
	if (strText.IsEmpty())
		return;

	UINT nCurrentPage = _ttoi(strText);
	if (nCurrentPage > m_pPreviewInfo->GetMaxPage())
		nCurrentPage = m_pPreviewInfo->GetMaxPage();
	if (nCurrentPage < m_pPreviewInfo->GetMinPage())
		nCurrentPage = m_pPreviewInfo->GetMinPage();

	if (nCurrentPage != m_nCurrentPage)
		SetCurrentPage(nCurrentPage, TRUE);
}

void CXTPPreviewView::OnUpdateGotoPage(CCmdUI* pCmdUI)
{
	CXTPControlEdit* pControl = DYNAMIC_DOWNCAST(CXTPControlEdit, CXTPControl::FromUI(pCmdUI));
	if (!pControl)
		return;

	if (!pControl->HasFocus())
	{
		CString strText, strFormat;
		if (m_pPreviewInfo->GetMaxPage() == 0xffff)
		{
			XTPResourceManager()->LoadString(&strFormat, XTP_IDS_PREVIEW_PAGEI);
			strText.Format(strFormat, m_nCurrentPage);
		}
		else
		{
			XTPResourceManager()->LoadString(&strFormat, XTP_IDS_PREVIEW_PAGEIOFI);
			strText.Format(strFormat, m_nCurrentPage, m_pPreviewInfo->GetMaxPage());
		}

		pControl->SetEditText(strText);
	}

	pCmdUI->Enable((m_nCurrentPage + m_nPages - 1 < m_pPreviewInfo->GetMaxPage()) || (m_nCurrentPage > m_pPreviewInfo->GetMinPage()));
}

void CXTPPreviewView::OnZoomCombo(NMHDR* pNMHDR, LRESULT* pRes)
{
	CXTPControlComboBox* pControl = DYNAMIC_DOWNCAST(CXTPControlComboBox, ((NMXTPCONTROL*)pNMHDR)->pControl);
	if (!pControl)
		return;

	int nCurSel = pControl->GetCurSel();

	if (nCurSel != -1)
	{
		if (nCurSel == XTP_ZOOM_WHOLEPAGE || nCurSel == XTP_ZOOM_PAGEWIDTH)
		{
			SetZoomState(nCurSel, 0, CPoint(0, 0));
		}
		else if (nCurSel > 1)
		{
			SetZoomState((UINT)pControl->GetItemData(nCurSel), 0, CPoint(0, 0));
		}
	}
	else
	{
		int nZoom = _ttoi(pControl->GetEditText());
		if (nZoom > 0 && nZoom < 3000)
		{
			SetZoomState(nZoom, 0, CPoint(0, 0));

			CString strZoom;
			strZoom.Format(_T("%i%%"), m_nZoomState);
			pControl->SetEditText(strZoom);
		}
	}


	*pRes = TRUE;
}

void CXTPPreviewView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	UNUSED_ALWAYS(bActivate);
	UNUSED_ALWAYS(pActivateView);
	UNUSED_ALWAYS(pDeactiveView);

	ASSERT(m_pPreviewInfo);

	if (m_pPreviewInfo)
	{
		if (m_pPreviewInfo->m_nNumPreviewPages == 2)
			SetNumPreviewPages(2, 1);
	}
}

void CXTPPreviewView::SetNumPreviewPages(int nAcross, int nDown)
{
	ASSERT(m_pPreviewInfo);

	if (!m_pPreviewInfo)
		return;

	m_Across = nAcross;
	m_Down = nDown;
	m_nPages = nAcross * nDown;

	AfxGetApp()->m_nNumPreviewPages = m_nPages;

	m_nZoomOutPages = m_nPages;
	m_nMaxPages = m_nPages;

	SetZoomState(XTP_ZOOM_WHOLEPAGE, 0, CPoint(0, 0));

	if (m_nZoomState == XTP_ZOOM_WHOLEPAGE)
	{
		bool bSinglePage = (m_nPages == 1);

		// make sure the current page does not cause blank pages to be shown
		if (bSinglePage)
		{
			if (m_nCurrentPage > m_pPreviewInfo->GetMaxPage() - m_nPages + 1)
				m_nCurrentPage = m_pPreviewInfo->GetMaxPage() - m_nPages + 1;
		}
		else
		{
			if (m_nCurrentPage >= m_pPreviewInfo->GetMaxPage() - m_nPages)
				m_nCurrentPage = m_pPreviewInfo->GetMaxPage() - m_nPages + 1;
		}

		if (m_nCurrentPage < 0)
			m_nCurrentPage = 0 ;
	}
	// Just do this to set the status correctly and invalidate
	SetCurrentPage(m_nCurrentPage, TRUE);
	SetupScrollbar() ;
}

void CXTPPreviewView::OnOnePage()
{
	SetNumPreviewPages(1, 1);
}

void CXTPPreviewView::OnTwoPage(NMHDR* pNMHDR, LRESULT* pRes)
{
	CXTPControlSelector* pControl = DYNAMIC_DOWNCAST(CXTPControlSelector, ((NMXTPCONTROL*)pNMHDR)->pControl);
	if (!pControl)
		return;

	SetNumPreviewPages(pControl->m_szResult.cx, pControl->m_szResult.cy);

	*pRes = TRUE;
}

void CXTPPreviewView::OnSize(UINT nType, int cx, int cy)
{
	// CScrollView handles everything if zoomed in.
	if (m_nZoomState == XTP_ZOOM_WHOLEPAGE)
	{
		// Force recalc of scale ratios on next draw
		for (UINT i = 0; i < m_nMaxPages; i++)
			m_pPageInfo[i].sizeScaleRatio.cx = 0;           // zero scale ratios

		CView::OnSize(nType, cx, cy);       // No scroll functionality
	}
	else
	{
		// adjust scroll size to size of page
		m_pageDev.cx = cx;
		m_pageDev.cy = cy;
		m_lineDev.cx = cx / 10;
		m_lineDev.cy = cy / 10;

		if (m_nZoomState == XTP_ZOOM_PAGEWIDTH)
		{
			// Force recalc of scale ratios on next draw
			for (UINT i = 0; i < m_nMaxPages; i++)
				m_pPageInfo[i].sizeScaleRatio.cx = 0;           // zero scale ratios
			CView::OnSize(nType, cx, cy);       // No scroll functionality
		}
		else
		{
			CScrollView::OnSize(nType, cx, cy);
		}
	}
}
