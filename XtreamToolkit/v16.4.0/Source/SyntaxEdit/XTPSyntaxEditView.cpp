// XTPSyntaxEditView.cpp : implementation file
//
// This file is a part of the XTREME TOOLKIT PRO MFC class library.
// (c)1998-2013 Codejock Software, All Rights Reserved.
//
// THIS SOURCE FILE IS THE PROPERTY OF CODEJOCK SOFTWARE AND IS NOT TO BE
// RE-DISTRIBUTED BY ANY MEANS WHATSOEVER WITHOUT THE EXPRESSED WRITTEN
// CONSENT OF CODEJOCK SOFTWARE.
//
// THIS SOURCE CODE CAN ONLY BE USED UNDER THE TERMS AND CONDITIONS OUTLINED
// IN THE XTREME SYNTAX EDIT LICENSE AGREEMENT. CODEJOCK SOFTWARE GRANTS TO
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

// common includes
#include "Common/XTPSmartPtrInternalT.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPResourceManager.h"

// syntax editor includes
#include "XTPSyntaxEditDefines.h"
#include "XTPSyntaxEditStruct.h"
#include "XTPSyntaxEditUndoManager.h"
#include "XTPSyntaxEditLexPtrs.h"
#include "XTPSyntaxEditTextIterator.h"
#include "XTPSyntaxEditLexParser.h"
#include "XTPSyntaxEditBufferManager.h"
#include "XTPSyntaxEditFindReplaceDlg.h"
#include "XTPSyntaxEditCtrl.h"
#include "XTPSyntaxEditDoc.h"
#include "XTPSyntaxEditView.h"
#include "XTPSyntaxEditPaintManager.h"
#include "XTPSyntaxEditLexColorFileReader.h"
#include "XTPSyntaxEditDrawTextProcessor.h"
#include "XTPSyntaxEditSelection.h"

using namespace XTPSyntaxEditLexAnalyser;


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

DWORD_PTR           CXTPSyntaxEditView::ms_dwSignature = 0;
BOOL                CXTPSyntaxEditView::ms_bDroppedHere = FALSE;
POINT              CXTPSyntaxEditView::ms_ptDropPos;
CXTPSyntaxEditView* CXTPSyntaxEditView::ms_pTargetView = NULL;

/////////////////////////////////////////////////////////////////////////////
// CXTPSyntaxEditView

CXTPSyntaxEditFindReplaceDlg* CXTPSyntaxEditView::m_pFindReplaceDlg = NULL;

IMPLEMENT_DYNCREATE(CXTPSyntaxEditView, CView)

CXTPSyntaxEditView::CXTPSyntaxEditView() :
m_pParentWnd(NULL)
, m_iTopRow(1)
, m_szPage(CSize(850, 1100))
, m_nPrevTopRow(1)
, m_bOleDragging(FALSE)
, m_bFilesDragging(FALSE)
, m_bDraggingStartedHere(FALSE)
, m_bDraggingOver(FALSE)
, m_bScrollBars(TRUE)
, m_nParserThreadPriority_WhenActive(THREAD_PRIORITY_LOWEST)
, m_nParserThreadPriority_WhenInactive(THREAD_PRIORITY_IDLE)
{
	m_pEditCtrl = new CXTPSyntaxEditCtrl();
	ZeroMemory(&m_lfPrevFont, sizeof(m_lfPrevFont));

	m_pPrintOptions = new CXTPSyntaxEditViewPrintOptions();
	// init defaults
	if (m_pPrintOptions)
	{
		m_pPrintOptions->GetPageHeader()->m_strFormatString = _T("&w &b&b &d &t");
		m_pPrintOptions->GetPageFooter()->m_strFormatString = _T("Page &p of &P");
	}

	GetEditCtrl().SetCreateScrollbarOnParent(TRUE);

	CMDTARGET_ADDREF(m_pFindReplaceDlg);

	m_bPrintDirect = FALSE;
	m_bResizeControlWithView = TRUE;
	m_bOnSizeRunning = FALSE;
}

CXTPSyntaxEditView::~CXTPSyntaxEditView()
{
	CMDTARGET_RELEASE(m_pPrintOptions);

	if (m_pFindReplaceDlg && IsWindow(m_pFindReplaceDlg->GetSafeHwnd()))
		m_pFindReplaceDlg->SendMessage(WM_COMMAND, MAKEWPARAM(IDCANCEL, 0), 0);

	CMDTARGET_RELEASE(m_pFindReplaceDlg);

	SAFE_DELETE(m_pEditCtrl);
}

BEGIN_MESSAGE_MAP(CXTPSyntaxEditView, CView)
	//{{AFX_MSG_MAP(CXTPSyntaxEditView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_KEYDOWN()
	ON_WM_CHAR()
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_WM_SETFOCUS()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
	ON_COMMAND(ID_EDIT_REDO, OnEditRedo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, OnUpdateEditRedo)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEditCut)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	ON_COMMAND(ID_EDIT_CLEAR, OnEditDelete)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CLEAR, OnUpdateEditDelete)
	ON_COMMAND(ID_EDIT_SELECT_ALL, OnEditSelectAll)
	ON_UPDATE_COMMAND_UI(ID_EDIT_SELECT_ALL, OnUpdateEditSelectAll)
	ON_COMMAND(ID_EDIT_FIND, OnEditFind)
	ON_UPDATE_COMMAND_UI(ID_EDIT_FIND, OnUpdateEditFind)
	ON_COMMAND(ID_EDIT_REPLACE, OnEditReplace)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REPLACE, OnUpdateEditReplace)
	ON_COMMAND(ID_EDIT_REPEAT, OnEditRepeat)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REPEAT, OnUpdateEditRepeat)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_OVR, OnUpdateKeyIndicator)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTPSyntaxEditView drawing

void CXTPSyntaxEditView::OnDraw(CDC* pDC)
{
	UNREFERENCED_PARAMETER(pDC);
	// Do nothing because actual drawing is inside the child edit control
}

void CXTPSyntaxEditView::OnPaint()
{
	Default();
}

/////////////////////////////////////////////////////////////////////////////
// CXTPSyntaxEditView diagnostics

#ifdef _DEBUG
void CXTPSyntaxEditView::AssertValid() const
{
	CView::AssertValid();
}

void CXTPSyntaxEditView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CXTPSyntaxEditView command handlers

void CXTPSyntaxEditView::OnEditUndo()
{
	GetEditCtrl().Undo();
	UpdateSiblings();
	Invalidate(FALSE);
}

void CXTPSyntaxEditView::OnUpdateEditUndo(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(GetEditCtrl().CanUndo());
}

void CXTPSyntaxEditView::OnEditRedo()
{
	GetEditCtrl().Redo();
	UpdateSiblings();
	Invalidate(FALSE);
}

void CXTPSyntaxEditView::OnUpdateEditRedo(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(GetEditCtrl().CanRedo());
}

void CXTPSyntaxEditView::OnEditCut()
{
	GetEditCtrl().Cut();
}

void CXTPSyntaxEditView::OnUpdateEditCut(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(GetEditCtrl().IsSelectionExist());
}

void CXTPSyntaxEditView::OnEditCopy()
{
	GetEditCtrl().Copy();
}

void CXTPSyntaxEditView::OnUpdateEditCopy(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(GetEditCtrl().IsSelectionExist());
}

void CXTPSyntaxEditView::OnEditPaste()
{
	GetEditCtrl().Paste();
}

void CXTPSyntaxEditView::OnUpdateEditPaste(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(IsClipboardFormatAvailable(CF_TEXT));
}

void CXTPSyntaxEditView::OnEditDelete()
{
	GetEditCtrl().DeleteSelection();
}

void CXTPSyntaxEditView::OnUpdateEditDelete(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(GetEditCtrl().IsSelectionExist());
}

void CXTPSyntaxEditView::OnEditSelectAll()
{
	GetEditCtrl().SelectAll();
}

void CXTPSyntaxEditView::OnUpdateEditSelectAll(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

CXTPSyntaxEditFindReplaceDlg* CXTPSyntaxEditView::GetFindReplaceDlg()
{
	if (!m_pFindReplaceDlg)
		m_pFindReplaceDlg = new CXTPSyntaxEditFindReplaceDlg();
	return m_pFindReplaceDlg;
}

void CXTPSyntaxEditView::OnEditFind()
{
	_EditFindReplace(FALSE) ;
}

void CXTPSyntaxEditView::OnEditReplace()
{
	_EditFindReplace(TRUE);
}

void CXTPSyntaxEditView::_EditFindReplace(BOOL bReplaceDlg)
{
	if (!GetFindReplaceDlg())
		return;

	CString csSelText;
	GetEditCtrl().GetSelectionText(csSelText);

	if (csSelText.IsEmpty())
	{
		CPoint pt(GetCaretPos());
		pt.y += 2;

		GetEditCtrl().SelectWord(pt);
		GetEditCtrl().GetSelectionText(csSelText);
	}

	csSelText = csSelText.SpanExcluding(_T("\r\n"));
	csSelText.Replace(_T("\t"), _T("    "));

	if (!csSelText.IsEmpty())
	{
		GetFindReplaceDlg()->m_csFindText = csSelText;
	}

	GetFindReplaceDlg()->ShowDialog(&GetEditCtrl(), bReplaceDlg);
}

void CXTPSyntaxEditView::OnUpdateEditFind(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CXTPSyntaxEditView::OnUpdateEditReplace(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CXTPSyntaxEditView::OnEditRepeat()
{
	if (!GetFindReplaceDlg())
		return;

	if (IsWindow(GetFindReplaceDlg()->m_hWnd))
		GetFindReplaceDlg()->SendMessage(WM_COMMAND, MAKEWPARAM(IDCANCEL, 0), 0);

	BOOL bShiftKey  = (::GetKeyState(VK_SHIFT) & KF_UP) != 0;

	if (GetEditCtrl().Find(GetFindReplaceDlg()->m_csFindText,
		GetFindReplaceDlg()->m_bMatchWholeWord,
		GetFindReplaceDlg()->m_bMatchCase, !bShiftKey))
	{
		SetDirty();
		GetParentFrame()->SetMessageText(AFX_IDS_IDLEMESSAGE);
	}
	else
	{
		//AfxMessageBox(XTPResourceManager()->LoadString(XTP_IDS_EDIT_MSG_FSEARCH));

		if (GetEditCtrl().GetRowCount() > 0)
		{
			int nStartRow = 0;
			int nStartCol = 0;
			if (bShiftKey)
			{
				nStartRow = GetEditCtrl().GetRowCount();
				nStartCol = GetEditCtrl().GetEditBuffer()->GetLineTextLengthC(GetEditCtrl().GetRowCount()-1);
			}

			if (GetEditCtrl().Find(GetFindReplaceDlg()->m_csFindText,
				GetFindReplaceDlg()->m_bMatchWholeWord, GetFindReplaceDlg()->m_bMatchCase,
				!bShiftKey, TRUE, nStartRow, nStartCol))
			{
				SetDirty();
			}

			GetParentFrame()->SetMessageText(XTPResourceManager()->LoadString(XTP_IDS_EDIT_MSG_FSEARCH));
		}
	}
}

void CXTPSyntaxEditView::OnUpdateEditRepeat(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(GetFindReplaceDlg() && !GetFindReplaceDlg()->m_csFindText.IsEmpty());
}

/////////////////////////////////////////////////////////////////////////////
// CXTPSyntaxEditView message handlers

int CXTPSyntaxEditView::OnCreate(LPCREATESTRUCT lpCS)
{
	if (CView::OnCreate(lpCS) == -1)
		return -1;

	// initialize parent window pointer.
	m_pParentWnd = CWnd::FromHandlePermanent(lpCS->hwndParent);
	if (!::IsWindow(m_pParentWnd->GetSafeHwnd()))
		return -1;

	// get data manager and context pointers.
	CXTPSyntaxEditBufferManager* pDataManager = GetDataManager();
	CCreateContext* pContext = (CCreateContext*)lpCS->lpCreateParams;

	// create the edit control.
	if (!GetEditCtrl().Create(this, m_bScrollBars, m_bScrollBars, pDataManager, pContext, 100))
	{
		TRACE0("Failed to create edit control.\n");
		return -1;
	}

	GetEditCtrl().RecalcScrollBars();
	GetEditCtrl().SetTopRow(m_iTopRow);
	GetEditCtrl().EnableOleDrag(m_dropTarget.Register(this));

	// get the document.
	CXTPSyntaxEditDoc *pDoc = GetDocument();
	if (!pDoc)
	{
		GetEditCtrl().GetEditBuffer()->InsertText(_T(""), 1, 1, FALSE);
		return 0;
	}

	// Inherit any special back and foreground color lines
	CXTPSyntaxEditView *pView = pDoc->GetFirstView();
	if (pView != this && ::IsWindow(pView->GetSafeHwnd()))
		GetEditCtrl() = pView->GetEditCtrl();

	if (!pDataManager)
		pDoc->SetDataManager(GetEditCtrl().GetEditBuffer());

	return 0;
}

void CXTPSyntaxEditView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	if (m_bOnSizeRunning)
		return;

	m_bOnSizeRunning = TRUE;

	if (m_bResizeControlWithView && ::IsWindow(GetEditCtrl().m_hWnd))
	{
		CXTPClientRect rc(this);
		GetEditCtrl().MoveWindow(&rc, FALSE);
		GetEditCtrl().RecalcScrollBars();

		// for case when RecalcScrollBars() call change a view size (by disabling/enabling scrollbars)
		CXTPClientRect rc2(this);
		if (rc2 != rc)
		{
			GetEditCtrl().MoveWindow(&rc2, FALSE);
			GetEditCtrl().RecalcScrollBars();
		}
	}

	m_bOnSizeRunning = FALSE;
}

void CXTPSyntaxEditView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (GetEditCtrl().SendMessage(WM_KEYDOWN,
		nChar, nFlags) != 0)
	{
		CView::OnKeyDown(nChar, nRepCnt, nFlags);
	}
}

void CXTPSyntaxEditView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (GetEditCtrl().SendMessage(WM_CHAR,
		(WPARAM)nChar, (LPARAM)nFlags) != 0)
	{
		CView::OnChar(nChar, nRepCnt, nFlags);
	}
}

void CXTPSyntaxEditView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (GetEditCtrl().SendMessage(WM_VSCROLL,
		MAKEWPARAM(nSBCode, nPos), (LPARAM)pScrollBar->GetSafeHwnd()) != 0)
	{
		CView::OnVScroll(nSBCode, nPos, pScrollBar);
	}
}

void CXTPSyntaxEditView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (GetEditCtrl().SendMessage(WM_HSCROLL,
		MAKEWPARAM(nSBCode, nPos), (LPARAM)pScrollBar->GetSafeHwnd()) != 0)
	{
		CView::OnHScroll(nSBCode, nPos, pScrollBar);
	}
}

void CXTPSyntaxEditView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

//this call reinit all schemes which already initialized by CXTPSyntaxEditCtrl
//  OnUpdate(NULL, xtpEditHintInitView);
}

BOOL CXTPSyntaxEditView::OnEraseBkgnd(CDC*)
{
	return TRUE;
}

int CXTPSyntaxEditView::GetTopRow() const
{
	return GetEditCtrl().GetTopRow();
}

BOOL CXTPSyntaxEditView::SetTopRow(int iRow)
{
	m_iTopRow = iRow;
	return TRUE;
}

BOOL CXTPSyntaxEditView::OnEditChanging(NMHDR* pNMHDR, LRESULT* pResult)
{
	UNREFERENCED_PARAMETER(pNMHDR);

	*pResult = !CanChangeReadonlyFile();
	return TRUE;
}

BOOL CXTPSyntaxEditView::OnEditChanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	XTP_EDIT_NMHDR_EDITCHANGED* pNMHDR_EC = (XTP_EDIT_NMHDR_EDITCHANGED*)pNMHDR;
	UpdateSiblings(pNMHDR_EC);

	*pResult = 1;
	return TRUE;
}

BOOL CXTPSyntaxEditView::OnSetDocModified(NMHDR* pNMHDR, LRESULT* pResult)
{
	XTP_EDIT_NMHDR_DOCMODIFIED* pNMHDR_DM = (XTP_EDIT_NMHDR_DOCMODIFIED*)pNMHDR;

	CDocument *pDoc = GetDocument();
	if (!pDoc)
		return TRUE;

	pDoc->SetModifiedFlag(pNMHDR_DM->bModified);
	const CString& strTitle = pDoc->GetTitle();

	int nPos = strTitle.ReverseFind(_T('*'));

	if (pNMHDR_DM->bModified)
	{
		if (nPos == -1)
		{
			CString strNewTitle = strTitle;
			strNewTitle += _T(" *");
			pDoc->SetTitle(strNewTitle);
		}
	}
	else
	{
		if (nPos > 0)
		{
			CString strNewTitle = strTitle.Left(nPos - 1);
			pDoc->SetTitle(strNewTitle);
		}
	}

	*pResult = 1;
	return TRUE;
}

BOOL CXTPSyntaxEditView::OnDrawBookmark(NMHDR* pNMHDR, LRESULT* pResult)
{
	UNREFERENCED_PARAMETER(pNMHDR);

	*pResult = 0;
	return TRUE;
}

BOOL CXTPSyntaxEditView::OnRowColChanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	XTP_EDIT_NMHDR_ROWCOLCHANGED* pNMHDR_RCC = (XTP_EDIT_NMHDR_ROWCOLCHANGED*)pNMHDR;
	UNREFERENCED_PARAMETER(pNMHDR_RCC);

	*pResult = 0;
	return TRUE;
}

BOOL CXTPSyntaxEditView::OnUpdateScrollPos(NMHDR* pNMHDR, LRESULT* pResult)
{
	XTP_EDIT_NMHDR_SETSCROLLPOS* pNMHDR_SSP = (XTP_EDIT_NMHDR_SETSCROLLPOS*)pNMHDR;

	CDocument *pDoc = GetDocument();
	ASSERT(pDoc);
	if (pDoc)
	{
		pDoc->UpdateAllViews(this, xtpEditHintUpdateScrollPos, (CObject*)pNMHDR_SSP);
		//TRACE(_T("\n"));
	}

	*pResult = 1;
	return TRUE;
}

BOOL CXTPSyntaxEditView::OnEnableScrollBar(NMHDR* pNMHDR, LRESULT* pResult)
{
	UNREFERENCED_PARAMETER(pNMHDR);

	*pResult = 1;
	return TRUE;
}

BOOL CXTPSyntaxEditView::OnInsertKey(NMHDR* pNMHDR, LRESULT* pResult)
{
	UNREFERENCED_PARAMETER(pNMHDR);

	*pResult = 0;
	return TRUE;
}

BOOL CXTPSyntaxEditView::OnSelInit(NMHDR* pNMHDR, LRESULT* pResult)
{
	UNREFERENCED_PARAMETER(pNMHDR);
	UpdateSiblings(NULL, FALSE);

	*pResult = 0;
	return TRUE;
}

BOOL CXTPSyntaxEditView::OnStartOleDrag(NMHDR* pNMHDR, LRESULT* pResult)
{
	UNREFERENCED_PARAMETER(pNMHDR);
	StartOleDrag();

	*pResult = 0;
	return TRUE;
}

BOOL CXTPSyntaxEditView::OnMarginClicked(NMHDR* pNMHDR, LRESULT* pResult)
{
	XTP_EDIT_NMHDR_MARGINCLICKED* pNMMarginClicked = (XTP_EDIT_NMHDR_MARGINCLICKED*)pNMHDR;
	UNREFERENCED_PARAMETER(pNMMarginClicked);

	*pResult = 0;
	return TRUE;
}

BOOL CXTPSyntaxEditView::OnParseEvent(NMHDR* pNMHDR, LRESULT* pResult)
{
	XTP_EDIT_NMHDR_PARSEEVENT* pNMParseEvent = (XTP_EDIT_NMHDR_PARSEEVENT*)pNMHDR;
	UNREFERENCED_PARAMETER(pNMParseEvent);

	*pResult = 0;
	return TRUE;
}

BOOL CXTPSyntaxEditView::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	NMHDR* pNMHDR = (NMHDR*)lParam;

	if (pNMHDR && pNMHDR->hwndFrom == GetEditCtrl().m_hWnd)
	{
		switch (pNMHDR->code)
		{
		case XTP_EDIT_NM_EDITCHANGING:
			return OnEditChanging(pNMHDR, pResult);

		case XTP_EDIT_NM_EDITCHANGED:
			return OnEditChanged(pNMHDR, pResult);

		case XTP_EDIT_NM_SETDOCMODIFIED:
			return OnSetDocModified(pNMHDR, pResult);

		case XTP_EDIT_NM_DRAWBOOKMARK:
			return OnDrawBookmark(pNMHDR, pResult);

		case XTP_EDIT_NM_ROWCOLCHANGED:
			return OnRowColChanged(pNMHDR, pResult);

		case XTP_EDIT_NM_UPDATESCROLLPOS:
			return OnUpdateScrollPos(pNMHDR, pResult);

		case XTP_EDIT_NM_ENABLESCROLLBAR:
			return OnEnableScrollBar(pNMHDR, pResult);

		case XTP_EDIT_NM_INSERTKEY:
			return OnInsertKey(pNMHDR, pResult);

		case XTP_EDIT_NM_SELINIT:
			return OnSelInit(pNMHDR, pResult);

		case XTP_EDIT_NM_STARTOLEDRAG:
			return OnStartOleDrag(pNMHDR, pResult);

		case XTP_EDIT_NM_MARGINCLICKED:
			return OnMarginClicked(pNMHDR, pResult);

		case XTP_EDIT_NM_PARSEEVENT:
			return OnParseEvent(pNMHDR, pResult);
		}
	}

	return CView::OnNotify(wParam, lParam, pResult);
}

void CXTPSyntaxEditView::Refresh()
{
	CWaitCursor wait;

	GetEditCtrl().RefreshColors();

	m_iTopRow = GetEditCtrl().GetTopRow();

	if (m_iTopRow != -1)
		GetEditCtrl().SetTopRow(m_iTopRow);

	GetEditCtrl().RecalcScrollBars();
	GetEditCtrl().NotifyCurRowCol(GetEditCtrl().GetCurRow(), GetEditCtrl().GetCurCol());
}

void CXTPSyntaxEditView::OnSetFocus(CWnd* pOldWnd)
{
	CView::OnSetFocus(pOldWnd);
	GetEditCtrl().SetFocus();
}

void CXTPSyntaxEditView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)
{
	ASSERT(pInfo != NULL);  // overriding OnPaint -- never get this.
	if (!pInfo)
		return;

	int nVisibleRowsCount = GetEditCtrl().GetVisibleRowsCount();
	if (pInfo->m_nCurPage == 1 && nVisibleRowsCount == 0)
	{
		pInfo->m_bContinuePrinting = TRUE;
	}
	else if (pInfo->m_nCurPage > 1 && pInfo->m_nCurPage == (UINT)m_aPageStart.GetSize() &&
		m_aPageStart[pInfo->m_nCurPage - 1] >= (UINT)nVisibleRowsCount)
	{
		// can't paginate to that page, thus cannot print it.
		pInfo->m_bContinuePrinting = FALSE;
	}
	else if (pInfo->m_nCurPage > (UINT)m_aPageStart.GetSize() &&
		!PaginateTo(pDC, pInfo))
	{
		// can't paginate to that page, thus cannot print it.
		pInfo->m_bContinuePrinting = FALSE;
	}

	//-----------------------------------------------------------------------
	pDC->SetMapMode(MM_ANISOTROPIC);

	pDC->SetViewportExt(pDC->GetDeviceCaps(LOGPIXELSX),
						pDC->GetDeviceCaps(LOGPIXELSY));

	pDC->SetWindowExt(96, 96);

	// ptOrg is in logical coordinates
	pDC->OffsetWindowOrg(0, 0);
}

void CXTPSyntaxEditView::OnFilePageSetup()
{
/*
	DWORD dwFlags = PSD_MARGINS | PSD_INWININIINTLMEASURE;
	CXTPReportPageSetupDialog dlgPageSetup(GetPrintOptions(), dwFlags, this);

	XTPGetPrinterDeviceDefaults(dlgPageSetup.m_psd.hDevMode, dlgPageSetup.m_psd.hDevNames);

	int nDlgRes = (int)dlgPageSetup.DoModal();

	if (nDlgRes == IDOK)
	{
		AfxGetApp()->SelectPrinter(dlgPageSetup.m_psd.hDevNames, dlgPageSetup.m_psd.hDevMode, FALSE);
	}
	*/
}

BOOL CXTPSyntaxEditView::OnPreparePrinting(CPrintInfo* pInfo)
{
	pInfo->m_bDirect = m_bPrintDirect;

	m_nPrevTopRow = GetEditCtrl().GetTopRow();

	CWinApp* pWinApp = AfxGetApp();
	if (pWinApp)
		pWinApp->m_nNumPreviewPages = 1;

	pInfo->SetMinPage(1);

	return DoPreparePrinting(pInfo);
}

void CXTPSyntaxEditView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	if (GetEditCtrl().GetSyntaxColor())
	{
		CWaitCursor _WC;

		XTP_EDIT_LINECOL pos1_0 = {INT_MAX, 0};
		CXTPSyntaxEditLexTextSchema* ptrTextSch = GetEditCtrl().GetEditBuffer()->GetLexParser()->GetTextSchema();
		if (ptrTextSch)
		{
			//GetEditCtrl().GetEditBuffer()->GetLexParser()->StopParseInThread();
			GetEditCtrl().GetEditBuffer()->GetLexParser()->CloseParseThread();

			CXTPSyntaxEditTextIterator txtIter(GetEditCtrl().GetEditBuffer());
			ptrTextSch->RunParseUpdate(TRUE, &txtIter, &pos1_0, NULL);

		}
	}

	//GetEditCtrl().SetRedraw(FALSE);
	GetEditCtrl().m_bDisableRedraw = TRUE;
	m_nPrevTopRow = GetEditCtrl().GetTopRow();


	ASSERT(m_aPageStart.GetSize() == 0);
	m_aPageStart.RemoveAll();
	m_aPageStart.Add(1);

	CString str1, str2;
	if (m_pPrintOptions && m_pPrintOptions->GetPageHeader())
		str1 = m_pPrintOptions->GetPageHeader()->m_strFormatString;
	if (m_pPrintOptions && m_pPrintOptions->GetPageFooter())
		str2 = m_pPrintOptions->GetPageFooter()->m_strFormatString;

	if (str1.Find(_T("&P")) >= 0 || str2.Find(_T("&P")) >= 0)
	{
		int nCurPage = pInfo->m_nCurPage;

		pInfo->m_nCurPage = 65535;

		if (PaginateTo(pDC, pInfo))
		{
			pInfo->SetMaxPage((int)m_aPageStart.GetSize() - 1);

//          CWinApp* pWinApp = AfxGetApp();
//          if (pWinApp)
//              pWinApp->m_nNumPreviewPages = m_aPageStart.GetSize();

		}

		pInfo->m_nCurPage = nCurPage;
	}

//  CView::OnBeginPrinting(pDC, pInfo);
}

void CXTPSyntaxEditView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	CView::OnEndPrinting(pDC, pInfo);

	m_aPageStart.RemoveAll();

	GetEditCtrl().SetTopRow(m_nPrevTopRow);

	//GetEditCtrl().SetRedraw(TRUE);
	GetEditCtrl().m_bDisableRedraw = FALSE;

	GetEditCtrl().RecalcScrollBars();

}

extern BOOL CALLBACK _XTPAbortProc(HDC, int);

BOOL CXTPSyntaxEditView::PaginateTo(CDC* pDC, CPrintInfo* pInfo)
	// attempts pagination to pInfo->m_nCurPage, TRUE == success
{
	ASSERT_VALID(this);
	ASSERT_VALID(pDC);

	BOOL bAborted = FALSE;
	CXTPPrintingDialog dlgPrintStatus(this);
	CString strTemp = GetPageTitle();

	dlgPrintStatus.SetWindowText(_T("Calculating pages..."));

	dlgPrintStatus.SetDlgItemText(AFX_IDC_PRINT_DOCNAME, strTemp);
	dlgPrintStatus.SetDlgItemText(AFX_IDC_PRINT_PRINTERNAME, pInfo->m_pPD->GetDeviceName());
	dlgPrintStatus.SetDlgItemText(AFX_IDC_PRINT_PORTNAME, pInfo->m_pPD->GetPortName());
	dlgPrintStatus.ShowWindow(SW_SHOW);
	dlgPrintStatus.UpdateWindow();

	CRect rectSave = pInfo->m_rectDraw;
	UINT nPageSave = pInfo->m_nCurPage;
	//BOOL bBlackWhiteSaved = m_pPrintOptions->m_bBlackWhitePrinting;
	//m_pPrintOptions->m_bBlackWhitePrinting = FALSE;

	ASSERT(nPageSave > 1);
	ASSERT(nPageSave >= (UINT)m_aPageStart.GetSize());
	VERIFY(pDC->SaveDC() != 0);

	pDC->IntersectClipRect(0, 0, 0, 0);
	pInfo->m_nCurPage = (UINT)m_aPageStart.GetSize();
	while (pInfo->m_nCurPage < nPageSave)
	{
		ASSERT(pInfo->m_nCurPage == (UINT)m_aPageStart.GetSize());
		OnPrepareDC(pDC, pInfo);
		if (!pInfo->m_bContinuePrinting)
			break;

		strTemp.Format(_T("%d"), pInfo->m_nCurPage);
		dlgPrintStatus.SetDlgItemText(AFX_IDC_PRINT_PAGENUM, strTemp);

		pInfo->m_rectDraw.SetRect(0, 0, pDC->GetDeviceCaps(HORZRES), pDC->GetDeviceCaps(VERTRES));
		pDC->DPtoLP(&pInfo->m_rectDraw);
		OnPrint(pDC, pInfo);
		//if (pInfo->m_nCurPage == (UINT)m_aPageStart.GetSize())
		//  break;
		++pInfo->m_nCurPage;

		if (!_XTPAbortProc(0, 0))
		{
			bAborted = TRUE;
			break;
		}
	}
	dlgPrintStatus.DestroyWindow();

	BOOL bResult = !bAborted && (pInfo->m_nCurPage == nPageSave || nPageSave == 65535);

	pInfo->m_bContinuePrinting = bResult;

	pDC->RestoreDC(-1);
	//m_pPrintOptions->m_bBlackWhitePrinting = bBlackWhiteSaved;
	pInfo->m_nCurPage = nPageSave;
	pInfo->m_rectDraw = rectSave;
	ASSERT_VALID(this);

	return bResult;
}

CString CXTPSyntaxEditView::GetPageTitle()
{
	CString strTitle;

	if (!GetDocument())
		return strTitle;

	strTitle = GetDocument()->GetPathName();

	//CString strFileName = strPathName.Right(strPathName.GetLength()-(strPathName.ReverseFind(_T('\\')) + 1));
	if (strTitle.IsEmpty())
	{
		strTitle= GetDocument()->GetTitle();
		strTitle.Remove(_T('*'));
	}

	return strTitle;
}

void CXTPSyntaxEditView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
	CRect rcMargins = m_pPrintOptions->GetMarginsLP(pDC);
	CRect rcPrint = pInfo->m_rectDraw;
	rcPrint.DeflateRect(rcMargins);

	//  if (!m_pPrintOptions->m_bBlackWhitePrinting)

	CRect rcPageHeader = rcPrint;
	CRect rcPageFooter = rcPrint;

	CString strTitle = GetEditCtrl().m_sCustomTitle;
	if (strTitle.IsEmpty())
		strTitle = GetPageTitle();
	//CString strTitle = CXTPPrintPageHeaderFooter::GetParentFrameTitle(this);

	m_pPrintOptions->GetPageHeader()->FormatTexts(pInfo, strTitle);
	m_pPrintOptions->GetPageFooter()->FormatTexts(pInfo, strTitle);

	pDC->SetBkColor(RGB(255, 255, 255));
	if (!m_pPrintOptions->GetPageHeader()->IsEmpty())
	{
		m_pPrintOptions->GetPageHeader()->Draw(pDC, rcPageHeader);

		rcPageHeader.top = rcPageHeader.bottom + 3;
		rcPageHeader.bottom = rcPageHeader.top + 2;
		pDC->FillSolidRect(rcPageHeader, RGB(0, 0, 0));

		rcPrint.top = rcPageHeader.bottom + 7;
	}
	if (!m_pPrintOptions->GetPageFooter()->IsEmpty())
	{
		m_pPrintOptions->GetPageFooter()->Draw(pDC, rcPageFooter, TRUE);
		rcPrint.bottom = rcPageFooter.top - 7;
	}

	//----------------------------------------------------------

	UINT nPage = pInfo->m_nCurPage;
	ASSERT(nPage <= (UINT)m_aPageStart.GetSize());
	UINT nIndex = m_aPageStart[nPage-1];

	GetEditCtrl().SetTopRow(nIndex);

	int nOptions = 0;
	//nOptions = nOptions | DT_WORDBREAK;
	//nOptions = nOptions | DT_SINGLELINE;

	// print as much as possible in the current page.
	nIndex += GetEditCtrl().PrintPage(pDC, rcPrint, nOptions);

	// update pagination information for page just printed
	if (nPage == (UINT)m_aPageStart.GetSize())
	{
		m_aPageStart.Add(nIndex);
	}
	else
	{
		ASSERT(nPage < (UINT)m_aPageStart.GetSize());
		m_aPageStart[nPage] = nIndex;
	}

	if (!m_pPrintOptions->GetPageFooter()->IsEmpty())
	{
		m_pPrintOptions->GetPageFooter()->Draw(pDC, rcPageFooter);

		rcPageFooter.top = rcPageFooter.top - 3;
		rcPageFooter.bottom = rcPageFooter.top + 1;
		pDC->FillSolidRect(rcPageFooter, RGB(0, 0, 0));
	}
}

const CXTPSyntaxEditCtrl& CXTPSyntaxEditView::GetEditCtrl() const
{
	ASSERT(m_pEditCtrl);
	return *m_pEditCtrl;
}

CXTPSyntaxEditCtrl& CXTPSyntaxEditView::GetEditCtrl()
{
	ASSERT(m_pEditCtrl);
	return *m_pEditCtrl;
}

void CXTPSyntaxEditView::SetEditCtrl(CXTPSyntaxEditCtrl* pControl)
{
	if (m_pEditCtrl && ::IsWindow(m_pEditCtrl->GetSafeHwnd()))
		m_pEditCtrl->DestroyWindow();

	SAFE_DELETE(m_pEditCtrl);

	m_pEditCtrl = pControl;
}


void CXTPSyntaxEditView::UpdateScrollPos(CView* pSender, DWORD dwUpdate/* = XTP_EDIT_UPDATE_ALL*/)
{
	CSplitterWnd *pSplitterWnd = GetParentSplitter(this, FALSE);
	CXTPSyntaxEditView* pActiveView = (CXTPSyntaxEditView*)(pSender ? pSender : GetParentFrame()->GetActiveView());

	//TRACE(_T("SyntaxEditView::UpdateScrollPos.this=%x. "), this);

	if (pSplitterWnd && pActiveView && GetEditCtrl().IsCreateScrollbarOnParent())
	{
		int nSplRowsCount = pSplitterWnd->GetRowCount();
		int nSplColsCount = pSplitterWnd->GetColumnCount();

		//  See CSplitterWnd::IdFromRowCol() implementation for details
		int nSenderRow = min(nSplRowsCount-1, (pActiveView->GetDlgCtrlID() - AFX_IDW_PANE_FIRST) / 16);
		int nSenderCol = min(nSplColsCount-1, (pActiveView->GetDlgCtrlID() - AFX_IDW_PANE_FIRST) % 16);

		int nRow = min(nSplRowsCount-1, (GetDlgCtrlID() - AFX_IDW_PANE_FIRST) / 16);
		int nCol = min(nSplColsCount-1, (GetDlgCtrlID() - AFX_IDW_PANE_FIRST) % 16);

		//TRACE(_T(" splitter.senderViev=%x-(Row, Col)(%d, %d).thisView[%d, %d] "), pActiveView, nSenderRow, nSenderCol, nRow, nCol);

		if (nSenderRow != nRow || nSenderCol != nCol)
		{
			if ((dwUpdate & XTP_EDIT_UPDATE_HORZ) && (nRow == nSenderRow) && (nCol != nSenderCol))
			{
				UpdateSiblingScrollPos(pActiveView, XTP_EDIT_UPDATE_HORZ);
				//TRACE(_T(" XTP_EDIT_UPDATE_HORZ"));
			}

			if ((dwUpdate & XTP_EDIT_UPDATE_VERT) && (nRow != nSenderRow) && (nCol == nSenderCol))
			{
				UpdateSiblingScrollPos(pActiveView, XTP_EDIT_UPDATE_VERT);
				//TRACE(_T(" XTP_EDIT_UPDATE_VERT"));
			}

			if ((dwUpdate & XTP_EDIT_UPDATE_DIAG) && (nRow != nSenderRow) && (nCol != nSenderCol))
			{
				UpdateSiblingScrollPos(pActiveView, XTP_EDIT_UPDATE_DIAG);
				//TRACE(_T(" XTP_EDIT_UPDATE_DIAG"));
			}
		}
	}
	else
	{
		GetEditCtrl().RecalcScrollBars();
		GetEditCtrl().Invalidate(FALSE);
	}
	//TRACE(_T(" \n"));
}

void CXTPSyntaxEditView::UpdateSiblingScrollPos(CXTPSyntaxEditView *pSender, DWORD dwUpdate)
{
	ASSERT_VALID(pSender);
	ASSERT_KINDOF(CXTPSyntaxEditView, pSender);

	if (pSender == this)
		return;

	CFrameWnd *pFrameWnd1 = GetParentFrame();
	CFrameWnd *pFrameWnd2 = pSender->GetParentFrame();

	GetEditCtrl().RecalcScrollBars();

	switch (dwUpdate & XTP_EDIT_UPDATE_ALL)
	{
	case XTP_EDIT_UPDATE_HORZ:
		{
			GetEditCtrl().Invalidate(FALSE);
			if (pFrameWnd1 == pFrameWnd2)
			{
				int nTopRow = pSender->GetEditCtrl().GetTopRow();
				if (nTopRow != GetEditCtrl().GetTopRow())
				{
					GetEditCtrl().SetTopRow(nTopRow);

					GetEditCtrl().UpdateWindow();

					GetEditCtrl().RecalcScrollBars();
				}
			}
		}
		break;

	case XTP_EDIT_UPDATE_VERT:
		{
			if (pFrameWnd1 == pFrameWnd2)
			{
				int nXOffset = pSender->GetEditCtrl().GetDrawTextProcessor().GetScrollXOffset();
				GetEditCtrl().GetDrawTextProcessor().SetScrollXOffset(nXOffset);
			}
			GetEditCtrl().Invalidate(FALSE);
		}
		break;

	case XTP_EDIT_UPDATE_DIAG:
		{
			GetEditCtrl().Invalidate(FALSE);
		}
		break;
	}
}

CXTPSyntaxEditView *CXTPSyntaxEditView::GetSplitterView(int nRow, int nCol)
{
	CSplitterWnd *pSplitterWnd = GetParentSplitter(this, FALSE);

	if (pSplitterWnd != NULL)
	{
		CWnd* pView = pSplitterWnd->GetDlgItem(pSplitterWnd->IdFromRowCol(nRow, nCol));
		if (!pView)
			return NULL;

		return DYNAMIC_DOWNCAST(CXTPSyntaxEditView, pView);
	}

	return NULL;
}

BOOL CXTPSyntaxEditView::PreCreateWindow(CREATESTRUCT& cs)
{
	// As CView provides its own window class and own drawing code
	// do not call CView::PreCreateWindow

	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	return TRUE;
}

void CXTPSyntaxEditView::UpdateSiblings(XTP_EDIT_NMHDR_EDITCHANGED* pNMHDR_EC, BOOL bTextChanged)
{
	if (!GetDocument())
		return;

	POSITION pos = GetDocument()->GetFirstViewPosition();
	while (pos)
	{
		CXTPSyntaxEditView* pView = DYNAMIC_DOWNCAST(CXTPSyntaxEditView, GetDocument()->GetNextView(pos));
		if (!pView || pView == this)
			continue;

		CXTPSyntaxEditCtrl& wndEditCtrl = pView->GetEditCtrl();

		if (pNMHDR_EC || bTextChanged)
		{
			wndEditCtrl.CalculateEditbarLength();
		}

		if (pNMHDR_EC)
		{
			wndEditCtrl.InvalidateRows(pNMHDR_EC->nRowFrom);

			wndEditCtrl.Invalidate(FALSE);
			wndEditCtrl.UpdateWindow();
		}
		else
		{
			wndEditCtrl.Unselect();
		}
	}
}

void CXTPSyntaxEditView::SetDirty()
{
	GetEditCtrl().Invalidate(FALSE);
}

AFX_STATIC BOOL AFX_CDECL EnsureViewIsActive(CView* pView)
{
	// Get a pointer to the view's parent frame.
	CFrameWnd* pParentFrame = DYNAMIC_DOWNCAST(
		CMDIChildWnd, pView->GetParentFrame());

	if (pParentFrame)
	{
		// If the top level frame is MDI activate.
		CMDIFrameWnd* pMDIFrameWnd = DYNAMIC_DOWNCAST(
			CMDIFrameWnd, pView->GetTopLevelFrame());

		if (pMDIFrameWnd)
		{
			if (pMDIFrameWnd->MDIGetActive() != pParentFrame)
				pMDIFrameWnd->MDIActivate(pParentFrame);
		}

		// If nested in a splitter set active pane.
		CSplitterWnd* pSplitterWnd = DYNAMIC_DOWNCAST(
			CSplitterWnd, pView->GetParent());

		if (pSplitterWnd)
		{
			if (pSplitterWnd->GetActivePane() != pView)
				pSplitterWnd->SetActivePane(0, 0, pView);
		}
		else
		{
			if (pParentFrame->GetActiveView() != pView)
				pParentFrame->SetActiveView(pView);
		}

		return TRUE;
	}

	return FALSE;
}

DROPEFFECT CXTPSyntaxEditView::OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
#ifdef _UNICODE
	m_bOleDragging = pDataObject->IsDataAvailable(CF_UNICODETEXT);
#else
	m_bOleDragging = pDataObject->IsDataAvailable(CF_TEXT);
#endif

	m_bFilesDragging = pDataObject->IsDataAvailable(CF_HDROP);

	ms_bDroppedHere = FALSE;

	EnsureViewIsActive(this);

	return CView::OnDragEnter(pDataObject, dwKeyState, point);
}

void CXTPSyntaxEditView::OnDragLeave()
{
	m_bOleDragging = FALSE;
	m_bFilesDragging = FALSE;
	ms_bDroppedHere = FALSE;

	CView::OnDragLeave();
}

DROPEFFECT CXTPSyntaxEditView::OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
	UNREFERENCED_PARAMETER(pDataObject);

	if (m_bFilesDragging)
	{
		return DROPEFFECT_COPY;
	}

	if (!m_bOleDragging)
		return DROPEFFECT_NONE;

	DROPEFFECT effect;

	if (dwKeyState & MK_CONTROL)
		effect = DROPEFFECT_COPY;
	else
		effect = DROPEFFECT_MOVE;

	int nRow, nDispCol;

	GetEditCtrl().RowColFromPoint(point, &nRow, NULL, NULL, &nDispCol);

	int iPrevRow = GetEditCtrl().GetCurRow();
	int iPrevCol = GetEditCtrl().GetCurCol();

	int nTopRow = GetEditCtrl().GetTopRow();
	int nRowPerPage = GetEditCtrl().GetRowPerPage();
	int nEndRow = GetEditCtrl().GetRowCount();

	if (nRow == nTopRow && nRow > 1)
		nRow--;

	if (nRow == (nTopRow + nRowPerPage - 1) && nRow < nEndRow)
		nRow++;

	if (nRow != iPrevRow || nDispCol != iPrevCol)
	{
		GetEditCtrl().SetCurPos(nRow, nDispCol, TRUE, TRUE);
	}

	//TRACE(_T("\nOnDragOver top row = %d"), GetEditCtrl().GetTopRow());

	if (GetFocus() != &GetEditCtrl())
		GetEditCtrl().SetFocus();

	m_bDraggingOver = TRUE;
	ms_bDroppedHere = FALSE;

	return effect;
}

BOOL CXTPSyntaxEditView::OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect,
							   CPoint point)
{
	UNREFERENCED_PARAMETER(dropEffect); UNREFERENCED_PARAMETER(point);

	if (m_bFilesDragging)
	{
		HGLOBAL hDropInfo = pDataObject->GetGlobalData(CF_HDROP);
		OnDropFiles((HDROP)hDropInfo);

		m_bFilesDragging = FALSE;
		return TRUE;
	}

	m_bOleDragging = FALSE;

	//TRACE(_T("\nOnDragDrop top row = %d"), GetEditCtrl().GetTopRow());

	if (ms_dwSignature != (DWORD_PTR)(GetEditCtrl().GetEditBuffer()))
	{
#ifdef _UNICODE
		HGLOBAL hMem = pDataObject->GetGlobalData(CF_UNICODETEXT);
#else
		HGLOBAL hMem = pDataObject->GetGlobalData(CF_TEXT);
#endif
		if (!hMem)
				return FALSE;

		LPTSTR szText = (LPTSTR)GlobalLock(hMem);
		if (!szText)
			return FALSE;

		int iRow = GetEditCtrl().GetCurRow();
		int iAbsCol = GetEditCtrl().GetCurAbsCol();

		GetEditCtrl().Unselect();
		GetEditCtrl().InsertString(szText, iRow, iAbsCol, FALSE);
		GetEditCtrl().GetEditBuffer()->GetUndoRedoManager()->SetLastCommandText(XTP_IDS_EDIT_PASTE);

		if (GetDocument())
			GetDocument()->SetModifiedFlag(TRUE);

		GlobalUnlock(hMem);
		GlobalFree(hMem);

		GetEditCtrl().CancelRightButtonDrag();

		ms_bDroppedHere = FALSE;
	}
	else
	{
		int nRow = GetEditCtrl().GetCurRow();
		int nDispCol = GetEditCtrl().GetCurCol();

		if (GetEditCtrl().GetSelection().IsInSel_disp(nRow, nDispCol))
		{
			GetEditCtrl().Unselect();
			return FALSE;
		}

		ms_ptDropPos.x = nDispCol;
		ms_ptDropPos.y = nRow;

		ms_bDroppedHere = TRUE;
		ms_pTargetView = this;
	}

	m_bDraggingStartedHere = FALSE;

	return TRUE;
}

void CXTPSyntaxEditView::StartOleDrag()
{
	HGLOBAL hMemText = GetEditCtrl().GetSelectionBuffer(CF_TEXT);
	if (!hMemText)
		return;

	HGLOBAL hMemUnicodeText = GetEditCtrl().GetSelectionBuffer(CF_UNICODETEXT);
	if (!hMemUnicodeText)
		return;

	COleDataSource dataSource;

	dataSource.CacheGlobalData(CF_TEXT, hMemText);
	dataSource.CacheGlobalData(CF_UNICODETEXT, hMemUnicodeText);

	ms_bDroppedHere = FALSE;

	m_bDraggingStartedHere = TRUE;

	ms_dwSignature = (DWORD_PTR)GetEditCtrl().GetEditBuffer();
	ms_pTargetView = NULL;

	DROPEFFECT effect = dataSource.DoDragDrop(DROPEFFECT_COPY|DROPEFFECT_MOVE);
	//TRACE(_T("\nStartOleDrag top row = %d"), GetEditCtrl().GetTopRow());

	if (effect != DROPEFFECT_NONE)
	{
		if (ms_bDroppedHere)
		{
			GetEditCtrl().SetDropPos(ms_ptDropPos.y, ms_ptDropPos.x);
			GetEditCtrl().HandleDrop(effect == DROPEFFECT_COPY);

			if (ms_pTargetView && ms_pTargetView != this)
				GetEditCtrl().Unselect();

			CXTPSyntaxEditCtrl& wndEditCtrl = ms_pTargetView->GetEditCtrl();
			wndEditCtrl.NotifyCurRowCol(wndEditCtrl.GetCurRow(), wndEditCtrl.GetCurCol());
		}
		else if (effect == DROPEFFECT_MOVE)
		{
			GetEditCtrl().DeleteSelection();
			GetEditCtrl().Invalidate(FALSE);
		}

		GetEditCtrl().CancelRightButtonDrag();

		if (!ms_bDroppedHere)
		{
			CPoint ptStartSel, ptEndSel;

			if (GetEditCtrl().IsSelectionExist())
			{
				XTP_EDIT_LINECOL lcSel = GetEditCtrl().GetSelection().GetNormalStart_disp();
				GetEditCtrl().SetCurPos(lcSel.nLine, lcSel.nCol, TRUE);
			}
		}
	}

	if (effect == DROPEFFECT_NONE)
	{
		if (!m_bDraggingOver)
		{
			if (!GetEditCtrl().IsRightButtonDrag())
				GetEditCtrl().Unselect();

			int nRow, nCol;

			CPoint point;
			GetCursorPos(&point);

			GetEditCtrl().ScreenToClient(&point);

			GetEditCtrl().RowColFromPoint(point, &nRow, &nCol);

			if (!GetEditCtrl().IsRightButtonDrag())
			{
				GetEditCtrl().SetCurPos(nRow, nCol, TRUE, TRUE);
			}
			else
			{
				GetEditCtrl().CancelRightButtonDrag();
				GetEditCtrl().ShowDefaultContextMenu();
			}
		}
		else
		{
			if (GetEditCtrl().IsSelectionExist())
			{
				XTP_EDIT_LINECOL lcSel = GetEditCtrl().GetSelection().GetNormalStart_disp();
				GetEditCtrl().SetCurPos(lcSel.nLine, lcSel.nCol, TRUE);
			}
		}

		if (GetEditCtrl().IsRightButtonDrag())
			GetEditCtrl().CancelRightButtonDrag();
	}

	m_bDraggingStartedHere = FALSE;
	m_bDraggingOver = FALSE;
	ms_bDroppedHere = FALSE;
	ms_dwSignature = 0;
}

void CXTPSyntaxEditView::UpdateAllViews()
{
	if (GetDocument())
	{
		XTP_EDIT_NMHDR_SETSCROLLPOS nmUpdateScroll;
		ZeroMemory(&nmUpdateScroll, sizeof(nmUpdateScroll));
		nmUpdateScroll.dwUpdate = XTP_EDIT_UPDATE_ALL;

		GetDocument()->UpdateAllViews(NULL, xtpEditHintUpdateScrollPos, (CObject*)&nmUpdateScroll);
	}

	Invalidate(FALSE);
	UpdateWindow();
}

void CXTPSyntaxEditView::SetSyntaxColor(BOOL bEnable)
{
	CDocument* pDoc = GetDocument();
	if (pDoc != NULL)
	{
		pDoc->UpdateAllViews(NULL, xtpEditHintSetSyntaxColor, (CObject*)(BOOL*)&bEnable);
	}
}

void CXTPSyntaxEditView::SetAutoIndent(BOOL bEnable)
{
	CDocument* pDoc = GetDocument();
	if (pDoc != NULL)
	{
		pDoc->UpdateAllViews(NULL, xtpEditHintSetAutoIndent, (CObject*)(BOOL*)&bEnable);
	}
}

void CXTPSyntaxEditView::SetSelMargin(BOOL bEnable)
{
	CDocument* pDoc = GetDocument();
	if (pDoc != NULL)
	{
		pDoc->UpdateAllViews(NULL, xtpEditHintSetSelMargin, (CObject*)(BOOL*)&bEnable);
	}
}

BOOL CXTPSyntaxEditView::SetScrollBars(BOOL bHorz, BOOL bVert, BOOL bUpdateReg/*=FALSE*/, BOOL bRecalcLayout/*=TRUE*/)
{
	if (m_bScrollBars)
	{
		if (!GetEditCtrl().SetScrollBars(bHorz, bVert, bUpdateReg))
			return FALSE;
	}

	CSplitterWnd* pSplitterWnd = GetEditCtrl().IsCreateScrollbarOnParent() ? GetParentSplitter(this, TRUE) : NULL;
	if (pSplitterWnd)
	{
		DWORD dwStyle = 0;

		if (bVert)
			dwStyle |= WS_VSCROLL;

		if (bHorz)
			dwStyle |= WS_HSCROLL;

		pSplitterWnd->SetScrollStyle(dwStyle);

		if (bRecalcLayout)
		{
			// Check have all splitter panes already created
			int nRowsCount = pSplitterWnd->GetRowCount();
			int nColsCount = pSplitterWnd->GetColumnCount();

			for (int nRow = 0; nRow < nRowsCount; nRow++)
			{
				for (int nCol = 0; nCol < nColsCount; nCol++)
				{
					if (!GetSplitterView(nRow, nCol))
					{
						return FALSE;
					}
				}
			}

			pSplitterWnd->RecalcLayout();
		}
		return TRUE;
	}

	return TRUE;
}

BOOL CXTPSyntaxEditView::GetVertScrollBar() const
{
	return GetEditCtrl().GetVertScrollBar();
}

BOOL CXTPSyntaxEditView::GetHorzScrollBar() const
{
	return GetEditCtrl().GetHorzScrollBar();
}

BOOL CXTPSyntaxEditView::CanChangeReadonlyFile()
{
	return TRUE;
}

void CXTPSyntaxEditView::SetLineNumbers(BOOL bEnable)
{
	CDocument* pDoc = GetDocument();
	if (pDoc != NULL)
	{
		pDoc->UpdateAllViews(NULL, xtpEditHintSetLineNum, (CObject*)(BOOL*)&bEnable);
	}
}

void CXTPSyntaxEditView::SetFontIndirect(LOGFONT *pLogFont, BOOL bUpdateReg/*=FALSE*/)
{
	GetEditCtrl().SetFontIndirect(pLogFont, bUpdateReg);

	CDocument* pDoc = GetDocument();
	if (pDoc != NULL)
	{
		pDoc->UpdateAllViews(this, xtpEditHintSetFont, (CObject*)pLogFont);
	}
}

void CXTPSyntaxEditView::OnDropFiles(HDROP hDropInfo)
{
	if (AfxGetApp() && AfxGetApp()->m_pMainWnd)
	{
		AfxGetApp()->m_pMainWnd->PostMessage(WM_DROPFILES, (WPARAM)hDropInfo);
	}
	else
	{
		::DragFinish(hDropInfo);
	}
}

void CXTPSyntaxEditView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	if (pSender == this)
	{
		return;
	}

	switch (lHint)
	{
	case xtpEditHintInvalidate:
		{
			if (GetEditCtrl().GetSafeHwnd())
				GetEditCtrl().Invalidate(FALSE);
		}
		break;

	case xtpEditHintUpdateScrollPos:
		{
			XTP_EDIT_NMHDR_SETSCROLLPOS* pNMHDR_SSP = (XTP_EDIT_NMHDR_SETSCROLLPOS*)pHint;
			UpdateScrollPos(pSender, pNMHDR_SSP->dwUpdate);
		}
		break;

	case xtpEditHintSetSyntaxColor:
		{
			BOOL bEnable = *(BOOL*)pHint;
			GetEditCtrl().SetSyntaxColor(bEnable);
		}
		break;

	case xtpEditHintSetAutoIndent:
		{
			BOOL bEnable = *(BOOL*)pHint;
			GetEditCtrl().SetAutoIndent(bEnable);
		}
		break;

	case xtpEditHintSetSelMargin:
		{
			BOOL bEnable = *(BOOL*)pHint;
			GetEditCtrl().SetSelMargin(bEnable);
		}
		break;

	case xtpEditHintSetLineNum:
		{
			BOOL bEnable = *(BOOL*)pHint;
			GetEditCtrl().SetLineNumbers(bEnable);
		}
		break;

	case xtpEditHintSetFont:
		{
			LOGFONT* pLogFont = (LOGFONT*)pHint;
			GetEditCtrl().SetFontIndirect(pLogFont);
		}
		break;

	case xtpEditHintInitView:
		{
			GetEditCtrl().GetRegValues();

			if (GetEditCtrl().m_bUseMonitor)
			{
			}
			else
			{
				//reload scheme
				if (GetEditCtrl().GetLexConfigurationManager())
					GetEditCtrl().GetLexConfigurationManager()->ReloadConfig();
			}

			// Update font.
			CXTPSyntaxEditView* pSrcView = DYNAMIC_DOWNCAST(CXTPSyntaxEditView, pSender);
			if (pSrcView)
			{
				LOGFONT lf;
				pSrcView->GetEditCtrl().GetPaintManager()->GetFont()->GetLogFont(&lf);
				GetEditCtrl().SetFontIndirect(&lf);
			}
			GetEditCtrl().RestoreCursor();

			// Update scrollbars.
			SetScrollBars(GetEditCtrl().GetHorzScrollBar(),
						  GetEditCtrl().GetVertScrollBar(), FALSE, TRUE);

			Refresh();
		}
		break;

	case xtpEditHintRefreshView:
		{
			Refresh();
		}
		break;
	}
}

void CXTPSyntaxEditView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	if (!m_pEditCtrl)
	{
		CView::OnActivateView(bActivate, pActivateView, pDeactiveView);
		return;
	}

	int nPriority = bActivate ? m_nParserThreadPriority_WhenActive :
								m_nParserThreadPriority_WhenInactive;

	CXTPSyntaxEditLexParser* pParser = GetLexParser();
	if (pParser)
	{
		pParser->SetParseThreadPriority(nPriority);
	}

	GetEditCtrl().SetActive(bActivate);

	CView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

void CXTPSyntaxEditView ::OnUpdateKeyIndicator(CCmdUI* pCmdUI)
{
	if (GetEditBuffer()->GetOverwriteFlag())
		pCmdUI->SetText(_T("OVR"));
	else
		pCmdUI->SetText(_T("INS"));
}

CXTPSyntaxEditDoc* CXTPSyntaxEditView::GetDocument() const
{
	return DYNAMIC_DOWNCAST(CXTPSyntaxEditDoc, m_pDocument);
}

CXTPSyntaxEditBufferManager* CXTPSyntaxEditView::GetDataManager() const
{
	CXTPSyntaxEditDoc* pDoc = GetDocument();

	if (pDoc)
		return pDoc->GetDataManager();

	return NULL;
}

XTPSyntaxEditLexAnalyser::CXTPSyntaxEditLexParser* CXTPSyntaxEditView::GetLexParser() const
{
	CXTPSyntaxEditBufferManager* pDataManager = GetDataManager();

	if (!pDataManager)
		pDataManager = GetEditCtrl().GetEditBuffer();

	if (pDataManager)
		return pDataManager->GetLexParser();

	return NULL;
}

/////////////////////////////////////////////////////////////////////////////
//class CXTPSyntaxEditViewPrintOptions

IMPLEMENT_DYNAMIC(CXTPSyntaxEditViewPrintOptions, CXTPPrintOptions)
CXTPSyntaxEditViewPrintOptions::CXTPSyntaxEditViewPrintOptions()
{
#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

#ifdef _XTP_ACTIVEX
//===========================================================================
// {A4A6D163-F565-4076-9530-5B638639964D}
static const GUID IID_ISyntaxEditPrintOptions =
{ 0xa4a6d163, 0xf565, 0x4076, { 0x95, 0x30, 0x5b, 0x63, 0x86, 0x39, 0x96, 0x4d } };

BEGIN_INTERFACE_MAP(CXTPSyntaxEditViewPrintOptions, CXTPPrintOptions)
	INTERFACE_PART(CXTPSyntaxEditViewPrintOptions, IID_ISyntaxEditPrintOptions, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPSyntaxEditViewPrintOptions, IID_ISyntaxEditPrintOptions)

BEGIN_DISPATCH_MAP(CXTPSyntaxEditViewPrintOptions, CXTPPrintOptions)
END_DISPATCH_MAP()

#endif
