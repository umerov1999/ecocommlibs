// XTPSyntaxEditCtrl.cpp
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
#include <imm.h>
#include <math.h>

// common includes
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPImageManager.h"
#include "Common/XTPNotifyConnection.h"
#include "Common/XTPSmartPtrInternalT.h"
#include "Common/XTPVC80Helpers.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPResourceManager.h"

// syntax editor includes
#include "XTPSyntaxEditDefines.h"
#include "XTPSyntaxEditStruct.h"
#include "XTPSyntaxEditDrawTextProcessor.h"
#include "XTPSyntaxEditSelection.h"
#include "XTPSyntaxEditUndoManager.h"
#include "XTPSyntaxEditLineMarksManager.h"
#include "XTPSyntaxEditLexPtrs.h"
#include "XTPSyntaxEditTextIterator.h"
#include "XTPSyntaxEditSectionManager.h"
#include "XTPSyntaxEditLexParser.h"
#include "XTPSyntaxEditLexColorFileReader.h"
#include "XTPSyntaxEditBufferManager.h"
#include "XTPSyntaxEditToolTipCtrl.h"
#include "XTPSyntaxEditAutoCompleteWnd.h"
#include "XTPSyntaxEditCtrl.h"
#include "XTPSyntaxEditPaintManager.h"

using namespace XTPSyntaxEditLexAnalyser;


#define XTP_IDC_EDIT_DRAG_MOVE 0xE12D
#define XTP_IDC_EDIT_DRAG_COPY 0xE12E

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



//////////////////////////////////////////////////////////////////////////
// Imm


class CXTPSyntaxEditCtrl::CImmWrapper
{
public:
	CImmWrapper();
	virtual ~CImmWrapper();

public:
	BOOL ImmIsIME(HKL hKL = NULL);

	XTP_HIMC ImmGetContext(HWND hWnd);
	BOOL ImmReleaseContext(HWND hWnd, XTP_HIMC hIMC);

	BOOL ImmSetCompositionWindow(XTP_HIMC hIMC, COMPOSITIONFORM* pCompForm);
	BOOL ImmSetCompositionFont(XTP_HIMC hIMC, LOGFONT* plfFont);
public:
	typedef BOOL     (WINAPI *PFN_ImmIsIME)(HKL);

	typedef XTP_HIMC (WINAPI *PFN_ImmGetContext)(HWND);
	typedef BOOL     (WINAPI *PFN_ImmReleaseContext)(HWND, XTP_HIMC);

	typedef BOOL     (WINAPI *PFN_ImmSetCompositionFont)(XTP_HIMC, LPLOGFONT); // A/W
	typedef BOOL     (WINAPI *PFN_ImmSetCompositionWindow)(XTP_HIMC, COMPOSITIONFORM*);

protected:
	HMODULE m_hImmDll;         // Handle to the imm32 dll.

	PFN_ImmIsIME                m_pfnImmIsIME;
	PFN_ImmGetContext           m_pfnImmGetContext;
	PFN_ImmReleaseContext       m_pfnImmReleaseContext;
	PFN_ImmSetCompositionFont   m_pfnImmSetCompositionFont;
	PFN_ImmSetCompositionWindow m_pfnImmSetCompositionWindow;
};


/////////////////////////////////////////////////////////////////////////////
//class _XTP_EXT_CLASS CXTPImmWrapper
#ifdef _UNICODE
#define XTP_PROC_NAME_AW(procName) procName "W"
#else
#define XTP_PROC_NAME_AW(procName) procName "A"
#endif

#define XTP_IMM_FNCALL(fnName, errRetVal) \
	if (!m_pfn##fnName) \
	return errRetVal; \
return m_pfn##fnName

CXTPSyntaxEditCtrl::CImmWrapper::CImmWrapper()
{
	m_hImmDll = ::LoadLibrary(_T("imm32.dll"));
	if (m_hImmDll)
	{
		m_pfnImmIsIME = (PFN_ImmIsIME)GetProcAddress(m_hImmDll, "ImmIsIME");
		m_pfnImmGetContext = (PFN_ImmGetContext)GetProcAddress(m_hImmDll, "ImmGetContext");
		m_pfnImmReleaseContext = (PFN_ImmReleaseContext)GetProcAddress(m_hImmDll, "ImmReleaseContext");
		m_pfnImmSetCompositionFont = (PFN_ImmSetCompositionFont)GetProcAddress(m_hImmDll, XTP_PROC_NAME_AW("ImmSetCompositionFont"));
		m_pfnImmSetCompositionWindow = (PFN_ImmSetCompositionWindow)GetProcAddress(m_hImmDll, "ImmSetCompositionWindow");
	}
	else
	{
		m_pfnImmIsIME = NULL;
		m_pfnImmGetContext = NULL;
		m_pfnImmReleaseContext = NULL;
		m_pfnImmSetCompositionFont = NULL;
		m_pfnImmSetCompositionWindow = NULL;
	}
}

CXTPSyntaxEditCtrl::CImmWrapper::~CImmWrapper()
{
	if (m_hImmDll)
	{
		::FreeLibrary(m_hImmDll);
	}
}

BOOL CXTPSyntaxEditCtrl::CImmWrapper::ImmIsIME(HKL hKL)
{
	ASSERT(m_pfnImmIsIME);

	if (!m_pfnImmIsIME)
		return FALSE;

	if (hKL == NULL)
		hKL = ::GetKeyboardLayout(0);

	return m_pfnImmIsIME(hKL);
}

XTP_HIMC CXTPSyntaxEditCtrl::CImmWrapper::ImmGetContext(HWND hWnd)
{
	ASSERT(m_pfnImmGetContext);
	XTP_IMM_FNCALL(ImmGetContext, NULL)(hWnd);
}

BOOL CXTPSyntaxEditCtrl::CImmWrapper::ImmReleaseContext(HWND hWnd, XTP_HIMC hIMC)
{
	ASSERT(m_pfnImmReleaseContext);
	XTP_IMM_FNCALL(ImmReleaseContext, FALSE)(hWnd, hIMC);
}

BOOL CXTPSyntaxEditCtrl::CImmWrapper::ImmSetCompositionWindow(XTP_HIMC hIMC, COMPOSITIONFORM* pCompForm)
{
	ASSERT(m_pfnImmReleaseContext);
	XTP_IMM_FNCALL(ImmSetCompositionWindow, FALSE)(hIMC, pCompForm);
}

BOOL CXTPSyntaxEditCtrl::CImmWrapper::ImmSetCompositionFont(XTP_HIMC hIMC, LOGFONT* plfFont)
{
	ASSERT(m_pfnImmReleaseContext);
	XTP_IMM_FNCALL(ImmSetCompositionFont, FALSE)(hIMC, plfFont);
}


/////////////////////////////////////////////////////////////////////////////

namespace XTPSyntaxEditLexAnalyser
{
	CString DBG_TraceTB_StartEndCls(CXTPSyntaxEditLexTextBlock* pTB);
}


class CXTPSyntaxEditCtrl::CTextSearchCache
{
public:
	CTextSearchCache();
	void Update(int nCurrTopRow);

	int nForTopRow;
	CXTPSyntaxEditLexTextBlockPtr ptrTBStart;
};

//===========================================================================
// CXTPSyntaxEditCtrl::CScreenSearchBlock
//===========================================================================
class CXTPSyntaxEditCtrl::CScreenSearchBlock
{
public:
	CScreenSearchBlock();
	virtual ~CScreenSearchBlock();
	CScreenSearchBlock(const CScreenSearchBlock& rSrc);

	int nRowStart;
	int nRowEnd;
	DWORD dwLastAccessTime;
	CXTPSyntaxEditLexTextBlockPtr ptrTBFirst;
};


//===========================================================================
// CXTPSyntaxEditCtrl::CScreenSearchCache
//===========================================================================
class CXTPSyntaxEditCtrl::CScreenSearchCache : public CArray<CXTPSyntaxEditCtrl::CScreenSearchBlock, CXTPSyntaxEditCtrl::CScreenSearchBlock&>
{
	typedef CArray<CXTPSyntaxEditCtrl::CScreenSearchBlock, CXTPSyntaxEditCtrl::CScreenSearchBlock&> Base;
public:
	DWORD m_dwLastRemoveOldTime;

	CScreenSearchCache();
	virtual ~CScreenSearchCache();

	void RemoveAll();
	void RemoveAt(int nIndex);
	void RemoveOld(int nTimeOut_sec);
};


const int MARGIN_LENGTH             = 20;
const int NODES_WIDTH               = 10;
const int TEXT_LEFT_OFFSET          = 4;
const UINT TIMER_SELECTION_ID       = 100;
const UINT TIMER_SELECTION_TIME     = 50;
const UINT TIMER_REDRAW_WHEN_PARSE  = 200;
const UINT TIMER_AUTOSCROLL_ID      = 110;
const UINT TIMER_AUTOSCROLL_TIME    = 120;


enum XTPSyntaxEditFlags
{
	xtpEditRedraw       = 0x0001,
	xtpEditForceRedraw  = 0x0002,
	xtpEditTextAsBlock  = 0x0004,
	xtpEditDispCol      = 0x0008,
};

//////////////////////////////////////////////////////////////////////////
// CXTPSyntaxEditOptions

CXTPSyntaxEditOptions::CXTPSyntaxEditOptions()
: m_bSyntaxColor(TRUE)
, m_bAutoIndent(TRUE)
, m_bSelMargin(TRUE)
, m_bLineNumbers(TRUE)
, m_bWideCaret(TRUE)
, m_bTabWithSpace(FALSE)
, m_bDrawNodes(TRUE)
, m_bEnableWhiteSpace(FALSE)

{
	m_bReadOnly = FALSE;
	m_bVirtualSpace = FALSE;
	m_bHideCaret = FALSE;
	m_bViewOnly = FALSE;
	m_bEnableEditAccelerators = FALSE;

}

/////////////////////////////////////////////////////////////////////////////
// CXTPSyntaxEditCtrl

CXTPSyntaxEditCtrl::CXTPSyntaxEditCtrl()
: m_bVertScrollBar(TRUE)
, m_bHorzScrollBar(TRUE)
, m_bDragging(FALSE)
, m_bDroppable(FALSE)
, m_bTokensLoaded(FALSE)
, m_bIsScrollingEndRow(FALSE)
, m_bRightButtonDrag(FALSE)
, m_bIsSmartIndent(TRUE)
, m_bEnableOleDrag(FALSE)
, m_bCaseSensitive(TRUE)
, m_bScrolling(FALSE)
, m_bFocused(FALSE)
, m_bIsActive(FALSE)
, m_nTopCalculatedRow(-1)
, m_nBottomCalculatedRow(-1)
, m_nTopRow(1)
, m_nCurrentCol(1)
, m_nCurrentDocumentRow(1)
, m_nDispCol(1)
, m_nAutoIndentCol(0)
, m_nMarginLength(MARGIN_LENGTH)
, m_nLineNumLength(0)
, m_nNodesWidth(NODES_WIDTH)
, m_nEditbarLength(MARGIN_LENGTH + NODES_WIDTH)
, m_nCollapsedTextRowsCount(0)
, m_nWheelScroll(4)
, m_nAverageLineLen(XTP_EDIT_AVELINELEN)
, m_dwInsertPos(0)
, m_dwLastRedrawTime(0)
, m_ptPrevMouse(CPoint(0, 0))
, m_pBuffer(NULL)
, m_pParentWnd(NULL)
{

	m_pOptions = new CXTPSyntaxEditOptions();
	m_pSelection = new CXTPSyntaxEditSelection();
	m_pDrawTextProcessor = new CXTPSyntaxEditDrawTextProcessor();

	m_pSink = new CXTPNotifySink();

	m_pSelection->m_pOwnerCtrl = this;

	m_bCreateScrollbarOnParent = TRUE;
	m_bAllowExpandCollapse = TRUE;
	m_bActivateOnFocus = FALSE;

	m_pImmWrapper = new CImmWrapper();

#ifndef _UNICODE
	m_chPrevLeadByte = 0;
#endif
	m_bIMEsupported = FALSE;

	m_pPaintManeger = new CXTPSyntaxEditPaintManager();

	GetRegValues();
	RegisterWindowClass();

	m_strDefaultCfgFilePath = GetModulePath() + XTP_EDIT_LEXPARSER_CFG_FILENAME;

	m_nHScrollMaxWidth = 0;
	m_bWndCreateInProgress = FALSE;

	m_dwAutoScrollDirection = 0;
	m_bDisableRedraw = FALSE;

	m_pToolTip = new CXTPSyntaxEditToolTipCtrl();
	m_pAutoComplete = new CXTPSyntaxEditAutoCompleteWnd();

	m_fcCollapsable = new CTextSearchCache();
	m_fcRowColors = new CTextSearchCache();
	m_arOnScreenSchCache = new CScreenSearchCache();

	m_pBuffer = new CXTPSyntaxEditBufferManager;

	m_bUseMonitor = TRUE;
	m_bConfigFileMode = TRUE;

	m_sCustomTitle.Empty();
}

CXTPSyntaxEditCtrl::~CXTPSyntaxEditCtrl()
{
	if (m_pBuffer && m_pBuffer->GetLexParser())
		m_pBuffer->GetLexParser()->CloseParseThread();

	m_pSink->UnadviseAll();

	m_arOnScreenSchCache->RemoveAll();
	CMDTARGET_RELEASE(m_pBuffer);

	DestroyWindow();

	CMDTARGET_RELEASE(m_pPaintManeger);

	SAFE_DELETE(m_pToolTip);
	SAFE_DELETE(m_pAutoComplete);

	SAFE_DELETE(m_fcCollapsable);
	SAFE_DELETE(m_fcRowColors);
	SAFE_DELETE(m_arOnScreenSchCache);
	SAFE_DELETE(m_pImmWrapper);
	SAFE_DELETE(m_pSelection);
	SAFE_DELETE(m_pDrawTextProcessor);

	CMDTARGET_RELEASE(m_pOptions);

	m_pSink->Delete();
}

void CXTPSyntaxEditCtrl::OnFinalRelease()
{
	CWnd::OnFinalRelease();
}


IMPLEMENT_DYNAMIC(CXTPSyntaxEditCtrl, CWnd)

BEGIN_MESSAGE_MAP(CXTPSyntaxEditCtrl, CWnd)
	//{{AFX_MSG_MAP(CXTPSyntaxEditCtrl)
	ON_WM_PAINT()
	ON_WM_SETCURSOR()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_KEYDOWN()
	ON_WM_SYSKEYDOWN()
	ON_WM_CHAR()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	ON_WM_NCACTIVATE()
	ON_WM_MOUSEACTIVATE()
	ON_WM_KEYUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_ERASEBKGND()
	ON_WM_CONTEXTMENU()
	ON_WM_TIMER()
	ON_WM_KILLFOCUS()
	ON_WM_SHOWWINDOW()
	ON_WM_ACTIVATE()
	ON_WM_MOUSEWHEEL()
	ON_WM_GETDLGCODE()
	ON_WM_DESTROY()
	ON_COMMAND(XTP_IDC_EDIT_DRAG_COPY, OnDragCopy)
	ON_COMMAND(XTP_IDC_EDIT_DRAG_MOVE, OnDragMove)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_SETTEXT, OnSetText)
	ON_MESSAGE(WM_GETTEXT, OnGetText)
	ON_MESSAGE(WM_GETTEXTLENGTH, OnGetTextLen)

	ON_MESSAGE(WM_GETFONT, OnGetFont)
	ON_MESSAGE(WM_SETFONT, OnSetFont)

	ON_MESSAGE(WM_INPUTLANGCHANGE, OnInputLanguage)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTPSyntaxEditCtrl message handlers

BOOL CXTPSyntaxEditCtrl::GetRegValues()
{
	CWinApp* pWinApp = AfxGetApp();
	if (pWinApp != NULL)
	{
		m_bVertScrollBar = pWinApp->GetProfileInt(XTP_EDIT_REG_SETTINGS, XTP_EDIT_REG_VSCROLLBAR, m_bVertScrollBar);
		m_bHorzScrollBar = pWinApp->GetProfileInt(XTP_EDIT_REG_SETTINGS, XTP_EDIT_REG_HSCROLLBAR, m_bHorzScrollBar);

		m_pOptions->m_bSyntaxColor   = pWinApp->GetProfileInt(XTP_EDIT_REG_SETTINGS, XTP_EDIT_REG_SYNTAXCOLOR,  m_pOptions->m_bSyntaxColor);
		m_pOptions->m_bAutoIndent    = pWinApp->GetProfileInt(XTP_EDIT_REG_SETTINGS, XTP_EDIT_REG_AUTOINDENT,   m_pOptions->m_bAutoIndent);
		m_pOptions->m_bSelMargin     = pWinApp->GetProfileInt(XTP_EDIT_REG_SETTINGS, XTP_EDIT_REG_SELMARGIN,    m_pOptions->m_bSelMargin);
		m_pOptions->m_bLineNumbers   = pWinApp->GetProfileInt(XTP_EDIT_REG_SETTINGS, XTP_EDIT_REG_LINENUMBERS,  m_pOptions->m_bLineNumbers);
		m_pOptions->m_bWideCaret     = pWinApp->GetProfileInt(XTP_EDIT_REG_SETTINGS, XTP_EDIT_REG_WIDECARET,    m_pOptions->m_bWideCaret);
		m_pOptions->m_bTabWithSpace  = pWinApp->GetProfileInt(XTP_EDIT_REG_SETTINGS, XTP_EDIT_REG_TABWITHSPACE, m_pOptions->m_bTabWithSpace);
		m_pOptions->m_bVirtualSpace  = pWinApp->GetProfileInt(XTP_EDIT_REG_SETTINGS, XTP_EDIT_REG_VIRTUALSPACE, m_pOptions->m_bVirtualSpace);
		m_pOptions->m_bDrawNodes     = pWinApp->GetProfileInt(XTP_EDIT_REG_SETTINGS, XTP_EDIT_REG_COLLAPSIBLENODES, m_pOptions->m_bDrawNodes);

		return TRUE;
	}

	return FALSE;
}

BOOL CXTPSyntaxEditCtrl::SetValueInt(LPCTSTR lpszValue, int nNewValue, int& nRefValue, BOOL bUpdateReg)
{
	nRefValue = nNewValue;

	if (bUpdateReg)
	{
		CWinApp* pWinApp = AfxGetApp();
		if (pWinApp != NULL)
		{
			if (pWinApp->WriteProfileInt(XTP_EDIT_REG_SETTINGS, lpszValue, nNewValue))
				return TRUE;
		}
		return FALSE;
	}

	return TRUE;
}

BOOL CXTPSyntaxEditCtrl::SetValueBool(LPCTSTR lpszValue, BOOL bNewValue, BOOL& bRefValue, BOOL bUpdateReg)
{
	bRefValue = bNewValue;

	if (bUpdateReg)
	{
		CWinApp* pWinApp = AfxGetApp();
		if (pWinApp != NULL)
		{
			if (pWinApp->WriteProfileInt(XTP_EDIT_REG_SETTINGS, lpszValue, (int)bNewValue))
				return TRUE;
		}
		return FALSE;
	}

	return TRUE;
}

BOOL CXTPSyntaxEditCtrl::SetAutoIndent(BOOL bAutoIndent, BOOL bUpdateReg/*=FALSE*/)
{
	if (!SetValueBool(XTP_EDIT_REG_AUTOINDENT, bAutoIndent, m_pOptions->m_bAutoIndent, bUpdateReg))
		return FALSE;

	return TRUE;
}

void CXTPSyntaxEditCtrl::OnPaint()
{
	DWORD dwStartTime = ::GetTickCount();

	CPaintDC dc(this); // device context for painting
	CXTPClientRect rcClient(this);

	//if ((!m_bChanged || m_bDisableRedraw) && m_bmpCache.GetSafeHandle() != 0)
	if (m_bDisableRedraw && m_bmpCache.GetSafeHandle() != 0)
	{
		CXTPCompatibleDC memDC(&dc, &m_bmpCache);
		dc.BitBlt(0, 0, rcClient.right, rcClient.bottom, &memDC, 0, 0, SRCCOPY);
	}
	else
	{
		CDC memDC;
		memDC.CreateCompatibleDC(&dc);

		if (!m_bmpCache.m_hObject)
			m_bmpCache.CreateCompatibleBitmap(&dc, rcClient.Width(), rcClient.Height());

		CXTPBitmapDC autoBitmap(&memDC, &m_bmpCache);

	#ifdef _DEBUG
		memDC.FillSolidRect(rcClient, 0xFF);
	#endif

		Draw(&memDC, rcClient);

		if (!IsWindowEnabled())
		{
			XTPImageManager()->DisableBitmap(memDC, rcClient, XTP_EDIT_DISABLED_COLOR_LIGHT, XTP_EDIT_DISABLED_COLOR_DARK);
		}

		dc.BitBlt(0, 0, rcClient.right, rcClient.bottom, &memDC, 0, 0, SRCCOPY);

		//m_bChanged = FALSE;

		// Draw caret
		CSize szCaret;
		szCaret.cx = GetWideCaret() ? m_pDrawTextProcessor->GetTextMetrics().tmAveCharWidth : 2;
		szCaret.cy = m_pDrawTextProcessor->GetRowHeight();

		BOOL bVirtSpace = _IsVirtualSpaceActive() || m_nAutoIndentCol;
		int nCol = m_nDispCol - 1;
		BOOL bHideCaret = m_pOptions->m_bHideCaret || !m_bFocused;
		CPoint ptCaret = m_pDrawTextProcessor->SetCaretPos(this, szCaret, max(0, GetCurrentVisibleRow() - 1), nCol,
							bHideCaret, bVirtSpace);

		// IME Support
		if (m_bIMEsupported)
		{
			XTP_HIMC hIMC = m_pImmWrapper->ImmGetContext(m_hWnd);
			if (hIMC)
			{
				COMPOSITIONFORM compForm;
				::ZeroMemory(&compForm, sizeof(compForm));

				compForm.dwStyle = CFS_POINT; //CFS_FORCE_POSITION
				compForm.ptCurrentPos = ptCaret;

				VERIFY(m_pImmWrapper->ImmSetCompositionWindow(hIMC, &compForm));

				VERIFY(m_pImmWrapper->ImmReleaseContext(m_hWnd, hIMC));
			}
		}
	}

	DWORD dwEndTime = ::GetTickCount();

	m_aveRedrawScreenTime.AddValue(::labs(dwEndTime - dwStartTime));

	//TRACE(_T("aveRedrawScreenTime = %d ms \n"), (int)m_aveRedrawScreenTime.GetAverageValue(0));
}

AFX_STATIC void AFX_CDECL XTPSECollapsedBlockDeleteFn(void* pPtr)
{
	XTP_EDIT_COLLAPSEDBLOCK* pBlock = (XTP_EDIT_COLLAPSEDBLOCK*)pPtr;
	SAFE_DELETE(pBlock);
}

void CXTPSyntaxEditCtrl::Draw(CDC *pDC, const CRect& rcRect)
{
	ASSERT(pDC);
	if (!pDC)
		return;

	pDC->SetBkMode(TRANSPARENT);

	// calculate rects
	CRect rcBookMarks, rcLineNum, rcNodes, rcText;
	CalcEditRects(&rcBookMarks, &rcLineNum, &rcNodes, &rcText, &rcRect);

	// set text rect
	m_pDrawTextProcessor->SetTextRect(rcText);
	m_pDrawTextProcessor->SetTabSize(GetTabSize());

	// Set Row Height
	m_pDrawTextProcessor->RecalcRowHeight(pDC, GetPaintManager()->GetFont());

	//--------------------------------------------------
	m_fcCollapsable->Update(m_nTopRow);
	m_fcRowColors->Update(m_nTopRow);

	//-------------------
	int nRowHeight = max(1, m_pDrawTextProcessor->GetRowHeight());
	int nLinesCount = rcRect.Height() / nRowHeight + ((rcRect.Height() % nRowHeight) ? 1 : 0);
	int nSkipLines = 0;
	m_nCollapsedTextRowsCount = 0;

	m_mapInternalRowBkColor.RemoveAll();
	m_mapInternalRowForeColor.RemoveAll();

	for (int nLine = 0; nLine < nLinesCount;)
	{
		int nTextRow = m_nTopRow + nLine + nSkipLines;

		int nCollapsedRowsCount = 0;
		DWORD dwCollapcedType = ProcessCollapsedRowsBeroreDraw(nTextRow, nCollapsedRowsCount);
		nSkipLines += nCollapsedRowsCount;

		//if (nCollapsedRowsCount)
		//  continue;

		// Draw Line attributes (bookmark, number, node)
		int nLineY0 = rcRect.top + m_pDrawTextProcessor->GetRowHeight() * nLine;
		int nLineY1 = nLineY0 + m_pDrawTextProcessor->GetRowHeight();

		if (GetSelMargin())
		{
			CRect rcLMark(rcBookMarks);
			rcLMark.top = nLineY0;
			rcLMark.bottom = nLineY1;
			GetPaintManager()->DrawLineMarks(pDC, rcLMark, nTextRow, this);
		}

		if (GetLineNumbers())
		{
			CRect rcLNum(rcLineNum);
			rcLNum.top = nLineY0;
			rcLNum.bottom = nLineY1;
			GetPaintManager()->DrawLineNumber(pDC, rcLNum, nTextRow, this);
		}

		if (GetCollapsibleNodes())
		{
			CRect rcNode, rcNodeFull;
			GetLineNodeRect(nLine, rcNode, &rcNodeFull);
			GetPaintManager()->DrawLineNode(pDC, rcNode, rcNodeFull, dwCollapcedType, nTextRow, this);
		}
		else
		{
			CRect rcGap(rcLineNum);

			rcGap.top = nLineY0;
			rcGap.bottom = nLineY1;
			rcGap.left = rcLineNum.right;
			rcGap.right = rcText.left;

			pDC->FillSolidRect(&rcGap, GetPaintManager()->GetBackColorEx(this));
		}


		// D R A W    T E X T /////////////////////////////////////////////

		CRect rcTextLine(rcText);
		rcTextLine.top = nLineY0;
		rcTextLine.bottom = nLineY1;

		GetPaintManager()->DrawLineTextEx(pDC, rcTextLine, nTextRow, nLine, this);

		nLine++;
	}

	//---------------------------------------------------------------------------
	GetPaintManager()->DrawCollapsedTextMarks(this, pDC);
	//---------------------------------------------------------------------------
}

int CXTPSyntaxEditCtrl::PrintPage(CDC *pDC, const CRect& rcRect, int nFlags) // returns printed rows count
{
	int nPrintedRowsCount = 0;

	ASSERT(pDC);
	if (!pDC)
		return 0;

	BOOL bSelMargin_orig = m_pOptions->m_bSelMargin;
	m_pOptions->m_bSelMargin = FALSE;
	CalculateEditbarLength(pDC);

	pDC->SetBkMode(TRANSPARENT);

	// calculate rects
	CRect rcBookMarks, rcLineNum, rcNodes, rcText;
	CalcEditRects(&rcBookMarks, &rcLineNum, &rcNodes, &rcText, &rcRect);

	// set text rect
	m_pDrawTextProcessor->SetTextRect(rcText);
	m_pDrawTextProcessor->SetTabSize(GetTabSize());

	// Set Row Height
	m_pDrawTextProcessor->RecalcRowHeight(pDC, GetPaintManager()->GetFont());

	//--------------------------------------------------
	m_fcCollapsable->Update(m_nTopRow);
	m_fcRowColors->Update(m_nTopRow);

	//-------------------
	int nRowHeight = max(1, m_pDrawTextProcessor->GetRowHeight());
	int nLinesCount = rcRect.Height() / nRowHeight + ((rcRect.Height() % nRowHeight) ? 1 : 0);
	int nSkipLines = 0;
	m_nCollapsedTextRowsCount = 0;

	int nNextLineY = 0;

	m_mapInternalRowBkColor.RemoveAll();
	m_mapInternalRowForeColor.RemoveAll();


	for (int nLine = 0;
		nLine < nLinesCount && nNextLineY + nRowHeight < rcText.Height();
		nPrintedRowsCount++)
	{
		int nTextRow = m_nTopRow + nLine + nSkipLines;

		int nCollapsedRowsCount = 0;
		ProcessCollapsedRowsBeroreDraw(nTextRow, nCollapsedRowsCount);
		nSkipLines += nCollapsedRowsCount;

		// Draw Line attributes (bookmark, number, node)
		int nLineY0 = rcRect.top + nNextLineY; //m_pDrawTextProcessor->GetRowHeight() * nLine;
		int nLineY1 = nLineY0 + m_pDrawTextProcessor->GetRowHeight();

		CRect rcTextLine(rcText);
		rcTextLine.top = nLineY0;
		rcTextLine.bottom = nLineY1;

		// calculate is enough vertical space to print all line text
		int nLineHeight = GetPaintManager()->PrintLineTextEx(pDC, rcTextLine, nTextRow, nLine,
							this, nFlags);
		if (nLineHeight < 0)
			break;

//      if (GetSelMargin())
//      {
//          CRect rcLMark(rcBookMarks);
//          rcLMark.top = nLineY0;
//          rcLMark.bottom = nLineY1;
//          GetPaintManager()->DrawLineMarks(pDC, rcLMark, nTextRow, this);
//      }

		if (GetLineNumbers() && (nFlags & DT_CALCRECT) == 0)
		{
			CRect rcLNum(rcLineNum);
			rcLNum.top = nLineY0;
			rcLNum.bottom = nLineY1;
			GetPaintManager()->DrawLineNumber(pDC, rcLNum, nTextRow, this);
		}

//      if (m_bDrawNodes)
//      {
//          CRect rcNode, rcNodeFull;
//          GetLineNodeRect(nLine, rcNode, &rcNodeFull);
//          GetPaintManager()->DrawLineNode(pDC, rcNode, rcNodeFull, dwCollapcedType, this);
//      }

		// D R A W    T E X T /////////////////////////////////////////////

//      if ((nFlags & DT_CALCRECT) == 0)
//          nLineHeight = GetPaintManager()->PrintLineTextEx(pDC, rcTextLine, nTextRow,
//                          nLine, this, nFlags);

		nNextLineY += nLineHeight;

		nLine++;
	}

	//---------------------------------------------------------------------------
	GetPaintManager()->DrawCollapsedTextMarks(this, pDC);
	//---------------------------------------------------------------------------

	m_pOptions->m_bSelMargin = bSelMargin_orig;

	return nPrintedRowsCount;
}


void CXTPSyntaxEditCtrl::UpdateRowInfoInternally(int nTextRow)
{
	CClientDC wndDC(this);
	CDC memDC;
	memDC.CreateCompatibleDC(&wndDC);

	int nDispRow = GetVisibleRow(nTextRow) - 1;
	if (nDispRow > m_pDrawTextProcessor->GetRowsCount(TRUE) + 10)
		nDispRow = m_pDrawTextProcessor->GetRowsCount(TRUE) + 10;

	CRect rcTextLine;
	CalcEditRects(NULL, NULL, NULL, &rcTextLine);

	int nCollapsedRowsCount = 0;
	ProcessCollapsedRowsBeroreDraw(nTextRow, nCollapsedRowsCount); // to update collapsed block info.

	GetPaintManager()->DrawLineTextEx(&memDC, rcTextLine, nTextRow, nDispRow, this);
}

DWORD CXTPSyntaxEditCtrl::ProcessCollapsedRowsBeroreDraw(int nTextRow, int& rnSkipRowsCount)
{
	rnSkipRowsCount = 0;

	if (nTextRow > GetRowCount())
		return XTP_EDIT_ROWNODE_NOTHING;

	XTP_EDIT_LMPARAM LMCoParam;
	BOOL bIsRowCollapsed = HasRowMark(nTextRow, xtpEditLMT_Collapsed, &LMCoParam);

	// retrieve row nodes
	CXTPSyntaxEditRowsBlockArray arCoBlocks;
	DWORD dwType = XTP_EDIT_ROWNODE_NOTHING;

	GetCollapsableBlocksInfo(nTextRow, arCoBlocks);

	BOOL bCollapsedProcessed = FALSE;
	int nCount = (int)arCoBlocks.GetSize();

	//-----------------------------------------
	if (bIsRowCollapsed && nCount == 0)
	{
		GetLineMarksManager()->DeleteLineMark(nTextRow, xtpEditLMT_Collapsed);
	}

	//-----------------------------------------
	for (int i = 0; i < nCount; i++)
	{
		XTP_EDIT_ROWSBLOCK coBlk = arCoBlocks[i];
		if (coBlk.lcStart.nLine == nTextRow)
		{
			if (bIsRowCollapsed && !bCollapsedProcessed)
			{
				XTP_EDIT_COLLAPSEDBLOCK* pCoDrawBlk;
				pCoDrawBlk = (XTP_EDIT_COLLAPSEDBLOCK*)LMCoParam.GetPtr();
				if (!pCoDrawBlk)
				{
					pCoDrawBlk = new XTP_EDIT_COLLAPSEDBLOCK;
					LMCoParam.SetPtr(pCoDrawBlk, XTPSECollapsedBlockDeleteFn);
				}
				pCoDrawBlk->collBlock = coBlk;
				GetLineMarksManager()->SetLineMark(nTextRow,
												xtpEditLMT_Collapsed, &LMCoParam);

				m_arCollapsedTextRows.SetAtGrow(m_nCollapsedTextRowsCount, nTextRow);
				m_nCollapsedTextRowsCount++;

				bCollapsedProcessed = TRUE;
				rnSkipRowsCount = max(1, coBlk.lcEnd.nLine - coBlk.lcStart.nLine);

				dwType |= XTP_EDIT_ROWNODE_COLLAPSED;
			}
			else
			{
				dwType |= XTP_EDIT_ROWNODE_EXPANDED;
			}
		}

		BOOL bLastLineEnd = coBlk.lcEnd.nLine > nTextRow && nTextRow == GetRowCount();
		if (coBlk.lcEnd.nLine == nTextRow || bLastLineEnd)
		{
			dwType |= XTP_EDIT_ROWNODE_ENDMARK;
		}
		if (coBlk.lcStart.nLine < nTextRow)
		{
			dwType |= XTP_EDIT_ROWNODE_NODEUP;
		}
		if (coBlk.lcEnd.nLine > nTextRow && !bCollapsedProcessed && !bLastLineEnd)
		{
			dwType |= XTP_EDIT_ROWNODE_NODEDOWN;
		}
		// check whether to skip the row
		if ((coBlk.lcStart.nLine < nTextRow) &&
			(coBlk.lcEnd.nLine >= nTextRow))
		{
			if (HasRowMark(coBlk.lcStart.nLine, xtpEditLMT_Collapsed))
			{
				rnSkipRowsCount = max(1, coBlk.lcEnd.nLine - nTextRow);
			}
		}
	}

	return dwType;
}

CRect CXTPSyntaxEditCtrl::CalcEditRects(CRect* prcBookMarks, CRect* prcLineNum, CRect* prcNodes, CRect* prcText,
										const CRect* prcClient) const
{
	CRect rcClient(0, 0, 100, 100);
	if (prcClient)
		rcClient = *prcClient;
	else if (m_hWnd)
		GetClientRect(&rcClient);

	// calculate rects
	CRect rcBookMarks = rcClient;
	rcBookMarks.right = rcBookMarks.left + (GetSelMargin() ? m_nMarginLength : 0);

	CRect rcLineNum = rcClient;
	rcLineNum.left = rcBookMarks.right;
	rcLineNum.right = rcLineNum.left + (GetLineNumbers() ? m_nLineNumLength : 0);

	CRect rcNodes = rcClient;
	rcNodes.left = rcLineNum.right;
	rcNodes.right = rcNodes.left + (GetCollapsibleNodes() ? m_nNodesWidth : 0);

	CRect rcText = rcClient;
	rcText.left += m_nEditbarLength;

	//--------------------------------------------
	if (prcBookMarks)
		*prcBookMarks = rcBookMarks;

	if (prcLineNum)
		*prcLineNum = rcLineNum;

	if (prcNodes)
		*prcNodes = rcNodes;

	if (prcText)
		*prcText = rcText;

	return rcClient;
}

BOOL CXTPSyntaxEditCtrl::RegisterWindowClass(HINSTANCE hInstance /*= NULL*/)
{
	return XTPDrawHelpers()->RegisterWndClass(hInstance,
			XTP_EDIT_CLASSNAME_EDITCTRL, CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS);
}

BOOL CXTPSyntaxEditCtrl::PreCreateWindow(CREATESTRUCT& )
{
	m_bWndCreateInProgress = TRUE;

	return TRUE;
}

void CXTPSyntaxEditCtrl::PreSubclassWindow()
{
	if (!m_bWndCreateInProgress)
	{
		// process only for SubclassWindow call
		DWORD dwStyle = ::GetWindowLong(m_hWnd, GWL_STYLE);

		m_bVertScrollBar = 0 != (dwStyle & WS_VSCROLL);
		m_bHorzScrollBar = 0 != (dwStyle & WS_HSCROLL);

		m_pParentWnd = GetParent();

		_InitEditControl();
	}
}

int CXTPSyntaxEditCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	m_bWndCreateInProgress = FALSE;

	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!_InitEditControl())
		return -1;

	return 0;
}

void CXTPSyntaxEditCtrl::OnDestroy()
{
	if (m_pToolTip->GetSafeHwnd())
		m_pToolTip->DestroyWindow();

	if (m_pAutoComplete->GetSafeHwnd())
		m_pAutoComplete->DestroyWindow();

	m_pSink->UnadviseAll();

	CWnd::OnDestroy();
}

BOOL CXTPSyntaxEditCtrl::Create(CWnd* pParentWnd, BOOL bHorzScroll, BOOL bVertScroll,
								CXTPSyntaxEditBufferManager *pBuffer,
								CCreateContext *lpCS, UINT nID)
{
	if (pBuffer)
	{
		CMDTARGET_ADDREF(pBuffer);

		CMDTARGET_RELEASE(m_pBuffer);

		m_pBuffer = pBuffer;
	}

	//-------------------------------------------
	m_bVertScrollBar = bVertScroll;
	m_bHorzScrollBar = bHorzScroll;

	DWORD dwStyle = (WS_CHILD | WS_VISIBLE);

	if (m_bVertScrollBar && !IsCreateScrollbarOnParent())
		dwStyle |= WS_VSCROLL;

	if (m_bHorzScrollBar && !IsCreateScrollbarOnParent())
		dwStyle |= WS_HSCROLL;

	//-------------------------------------------
	ASSERT_VALID(pParentWnd); // must be valid.
	m_pParentWnd = pParentWnd;

	BOOL bCreate = CWnd::CreateEx(WS_EX_ACCEPTFILES, XTP_EDIT_CLASSNAME_EDITCTRL, NULL,
					dwStyle, CRect(0, 0, 100, 100), m_pParentWnd, nID, (LPVOID)lpCS);

	if (!bCreate)
	{
		TRACE0("Failed to create edit window.\n");
	}

	return bCreate;
}

BOOL CXTPSyntaxEditCtrl::_InitEditControl()
{
	// create tip window
	VERIFY(m_pToolTip->Create(this));

	// Create AutoComplete window.
	VERIFY(m_pAutoComplete->Create(this));

	CRect rcText(0, 0, 3000, 2000);
	m_pDrawTextProcessor->SetTextRect(rcText);
	m_pDrawTextProcessor->SetTabSize(GetTabSize());

	// create data manager if need
	//if (!m_pBuffer)
	//  m_pBuffer = new CXTPSyntaxEditBufferManager;

	CXTPSyntaxEditConfigurationManager* pMan = GetLexConfigurationManager();
	if (pMan)
	{
		pMan->m_bUseMonitor = m_bUseMonitor;
		pMan->m_bConfigFileMode = m_bConfigFileMode;
		pMan->m_sIniSet = m_sIniSet;

		if (!m_bConfigFileMode
			&& !m_strSyntaxScheme.IsEmpty()
			&& !m_strColorScheme.IsEmpty())
		{
			pMan->SetSyntaxAndColorScheme(&m_strSyntaxScheme, &m_strColorScheme);
		}
	}

	if (!m_pBuffer)
		return FALSE;

	if (GetConfigFile().IsEmpty())
	{
		CString csCfgFilePath = GetDefaultCfgFilePath();
		if (!SetConfigFile(csCfgFilePath))
		{
//          TRACE1("\n*** Could not locate config file '%s'.\n\n", (LPCTSTR)csCfgFilePath);
		}
	}
	_UpdateIMEStatus();

	SetCurCaretPos(1, 1, FALSE, FALSE);

	// Advise to events
	m_pSink->UnadviseAll();

	CXTPNotifyConnection* ptrConnParser = m_pBuffer->GetLexParser()->GetConnection();
	ASSERT(ptrConnParser);
	if (ptrConnParser)
	{
		m_pSink->Advise(ptrConnParser, xtpEditOnParserStarted, CreateNotfySinkClassDelegate(this, &CXTPSyntaxEditCtrl::OnParseEvent));
		m_pSink->Advise(ptrConnParser, xtpEditOnTextBlockParsed, CreateNotfySinkClassDelegate(this, &CXTPSyntaxEditCtrl::OnParseEvent));
		m_pSink->Advise(ptrConnParser, xtpEditOnParserEnded, CreateNotfySinkClassDelegate(this, &CXTPSyntaxEditCtrl::OnParseEvent));
	}

	CXTPNotifyConnection* ptrConnBufMan = m_pBuffer->GetConnection();
	ASSERT(ptrConnBufMan);
	if (ptrConnBufMan)
	{
		m_pSink->Advise(ptrConnBufMan, xtpEditClassSchWasChanged, CreateNotfySinkClassDelegate(this, &CXTPSyntaxEditCtrl::OnLexCfgWasChanged));
		m_pSink->Advise(ptrConnBufMan, xtpEditThemeWasChanged, CreateNotfySinkClassDelegate(this, &CXTPSyntaxEditCtrl::OnLexCfgWasChanged));
		m_pSink->Advise(ptrConnBufMan, xtpEditAllConfigWasChanged, CreateNotfySinkClassDelegate(this, &CXTPSyntaxEditCtrl::OnLexCfgWasChanged));
	}

	return TRUE;
}

UINT CXTPSyntaxEditCtrl::OnGetDlgCode()
{
	return DLGC_WANTTAB | DLGC_WANTARROWS | DLGC_WANTCHARS | DLGC_WANTALLKEYS;
}

BOOL CXTPSyntaxEditCtrl::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	UNREFERENCED_PARAMETER(pWnd);
	UNREFERENCED_PARAMETER(nHitTest);
	UNREFERENCED_PARAMETER(message);

	CPoint pt;
	::GetCursorPos(&pt);
	ScreenToClient(&pt);

	CXTPClientRect rcClient(this);
	CRect rcText(rcClient);
	rcText.left += m_nEditbarLength;

	CRect rcBookmark(0, 0, GetSelMargin() ? m_nMarginLength : 0, rcClient.Height());
	CRect rcLineNumAndNodes(rcBookmark.right, 0, m_nEditbarLength, rcClient.Height());
	CXTPEmptyRect rcNode;

	int nRow = 0, nCol = 0, nDispRow = 0;
	RowColFromPoint(pt, &nRow, &nCol, &nDispRow);

	if (GetCollapsibleNodes())
	{
		// calculate node icon rect

		DWORD dwType = XTP_EDIT_ROWNODE_NOTHING;
		if (GetRowNodes(nRow, dwType) && (dwType & (XTP_EDIT_ROWNODE_COLLAPSED | XTP_EDIT_ROWNODE_EXPANDED)) )
		{
			GetLineNodeRect(nDispRow-1, rcNode);
		}
	}

	if (m_bDragging)
	{
		if (rcClient.PtInRect(pt))
		{
			if ((::GetKeyState(VK_CONTROL) & KF_UP) == 0)
			{
				SetCursor(GetPaintManager()->GetCurMove());
				return TRUE;
			}
			else
			{
				SetCursor(GetPaintManager()->GetCurCopy());
				return TRUE;
			}
		}
		else
		{
			SetCursor(GetPaintManager()->GetCurNO());
			return TRUE;
		}
	}

	else if (rcText.PtInRect(pt))
	{
		if (m_pSelection->IsInSel_str(nRow, nCol-1) && !m_pSelection->bSelectingRunning
			|| m_bRightButtonDrag)
		{
			SetCursor(GetPaintManager()->GetCurArrow());
			return TRUE;
		}
		else
		{
			if (!GetBlockFromPt(pt))
			{
				SetCursor(GetPaintManager()->GetCurIBeam());
				return TRUE;
			}
		}
	}

	else if (rcBookmark.PtInRect(pt))
	{
		SetCursor(GetPaintManager()->GetCurArrow());
		return TRUE;
	}

	else if (rcNode.PtInRect(pt))
	{
		SetCursor(GetPaintManager()->GetCurArrow());
		return TRUE;
	}

	else if (rcLineNumAndNodes.PtInRect(pt))
	{
		SetCursor(GetPaintManager()->GetCurLine());
		return TRUE;
	}

	SetCursor(GetPaintManager()->GetCurArrow());
	return TRUE;
}

LRESULT CXTPSyntaxEditCtrl::OnSetText(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);

	LPCTSTR szText = (LPCTSTR)lParam;
	SetText(szText);

	return (LRESULT)TRUE;
}

LRESULT CXTPSyntaxEditCtrl::OnGetTextLen(WPARAM, LPARAM)
{
	if (!GetEditBuffer())
		return 0;

	int nTextSize = 0;
	int nRowsCount = GetRowCount();
	for (int i = 1; i <= nRowsCount; i++)
	{
		nTextSize += GetEditBuffer()->GetLineTextLength(i, i < nRowsCount);
	}

	return (LRESULT)nTextSize;
}

LRESULT CXTPSyntaxEditCtrl::OnGetText(WPARAM wBufferSize, LPARAM lpBuffer)
{
	if (wBufferSize == 0)
		return OnGetTextLen(0, 0);

	CMemFile memFile;
	if (!GetText(memFile, (int)wBufferSize))
		return 0;

	void* pTextData = NULL;
	void* pTextEnd = NULL;

	UINT uBufferB = memFile.GetBufferPtr(CFile::bufferRead, (UINT)wBufferSize, &pTextData, &pTextEnd);

	LPTSTR pDest = (LPTSTR)lpBuffer;
	STRNCPY_S(pDest, wBufferSize, (LPCTSTR)pTextData, wBufferSize - 1);
	pDest[wBufferSize-1] = _T('\0');

	int nTextSize = (int)uBufferB/sizeof(TCHAR) - sizeof(TCHAR);
	nTextSize = max(0, nTextSize);
	nTextSize = min(nTextSize, (int)wBufferSize);

	//ASSERT(nTextSize == (int)_tcslen(pDest));

	return (LRESULT)nTextSize;
}

CString CXTPSyntaxEditCtrl::GetText(int nMaxLen) const
{
	CMemFile memFile;

	if (!GetText(memFile, nMaxLen))
		return _T("");

	void* pTextData = NULL;
	void* pTextEnd = NULL;

	memFile.GetBufferPtr(CFile::bufferRead, (UINT)nMaxLen, &pTextData, &pTextEnd);

	if (pTextEnd)
	{
		TCHAR* pEnd = (TCHAR*)pTextEnd;
		*(pEnd - 1) = _T('\0');
	}

	return CString((LPCTSTR)pTextData);
}

BOOL CXTPSyntaxEditCtrl::GetText(CMemFile& memFile, int nMaxLen) const
{
	if (!GetEditBuffer())
		return FALSE;

	CArchive ar(&memFile, CArchive::store);

	BOOL bUnicode = (sizeof(TCHAR) == 2);
	if (nMaxLen > 0)
		nMaxLen = nMaxLen * sizeof(TCHAR);


	GetEditBuffer()->SerializeEx(ar, bUnicode, FALSE, (UINT)-1, NULL, nMaxLen);

	ar << (TCHAR)0;

	ar.Close();

	memFile.SeekToBegin();

	return TRUE;
}

void CXTPSyntaxEditCtrl::SetText(LPCTSTR pcszText)
{
	if (!GetEditBuffer() || !pcszText)
	{
		ASSERT(FALSE);
		return;
	}
	int nStrLenB = (int)_tcslen(pcszText) * sizeof(TCHAR);
	CMemFile memFile((BYTE*)pcszText, nStrLenB);
	memFile.SeekToBegin();

	CArchive ar(&memFile, CArchive::load);

	BOOL bUnicode = (sizeof(TCHAR) == 2);
	GetEditBuffer()->SerializeEx(ar, bUnicode);

	//---------------------------------------
	RefreshColors();
	RecalcScrollBars();

	Invalidate(FALSE);
}


void CXTPSyntaxEditCtrl::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	UNREFERENCED_PARAMETER(pScrollBar); UNREFERENCED_PARAMETER(nPos);

	SCROLLINFO info;
	ZeroMemory(&info, sizeof(SCROLLINFO));
	info.cbSize = sizeof(SCROLLINFO);
	info.fMask = SIF_ALL;

	GetScrollInfo(SB_HORZ, &info);

	int nStep = m_pDrawTextProcessor->GetTextMetrics().tmAveCharWidth;

	int nCurrPos = m_pDrawTextProcessor->GetScrollXOffset(); //GetScrollPos(SB_HORZ);

	switch (nSBCode)
	{
	case SB_LINELEFT:
		nCurrPos -= nStep;
		break;
	case SB_LINERIGHT:
		nCurrPos += nStep;
		break;
	case SB_PAGELEFT:
		nCurrPos -= info.nPage;
		break;
	case SB_PAGERIGHT:
		nCurrPos += info.nPage;
		break;
	case SB_LEFT:
		nCurrPos = info.nMin;
		break;
	case SB_RIGHT:
		nCurrPos = info.nMax;
	case SB_ENDSCROLL:
		return;
	case SB_THUMBPOSITION:
	case SB_THUMBTRACK:
		{
			nCurrPos = nPos;
			int nDelta = ((nCurrPos % nStep) >= nStep/2 ? 1 : 0);
			nCurrPos = (nCurrPos / nStep + nDelta) * nStep;
			ASSERT(nCurrPos <= info.nMax);
		}
		break;
	default:
		break;
	}

	nCurrPos = max(0, nCurrPos);

	SetScrollPos(SB_HORZ, nCurrPos);
	m_pDrawTextProcessor->SetScrollXOffset(nCurrPos);

	if (m_bFocused)
	{
		SetCurCaretPos(GetCurrentDocumentRow(), m_nDispCol, FALSE, FALSE);
	}

	Invalidate(FALSE);
	UpdateWindow();
}

void CXTPSyntaxEditCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	UNREFERENCED_PARAMETER(nPos);
	UNREFERENCED_PARAMETER(pScrollBar);

	SCROLLINFO info;
	ZeroMemory(&info, sizeof(SCROLLINFO));
	info.cbSize = sizeof(SCROLLINFO);
	info.fMask = SIF_ALL;

	GetScrollInfo(SB_VERT, &info);

	int iMin = 0, iMax = 0;
	GetScrollRange(SB_VERT, &iMin, &iMax);

	int iPos = GetScrollPos(SB_VERT);
	BOOL bChanged = FALSE;

	switch (nSBCode) {
	case SB_LINEDOWN:
		bChanged = ShiftCurrentVisibleRowDown(1);
		if (bChanged)
			SetScrollPos(SB_VERT, iPos + 1);
		break;
	case SB_LINEUP:
		bChanged = ShiftCurrentVisibleRowUp(1);
		if (bChanged)
		SetScrollPos(SB_VERT, iPos - 1);
		break;
	case SB_PAGEUP:
		bChanged = ShiftCurrentVisibleRowUp(info.nPage);
		if (bChanged)
			SetScrollPos(SB_VERT, max(1, iPos - info.nPage));
		break;
	case SB_PAGEDOWN:
		bChanged = ShiftCurrentVisibleRowDown(info.nPage);
		if (bChanged)
			SetScrollPos(SB_VERT, (iPos + info.nPage));
		break;
	case SB_TOP:
		SetScrollPos(SB_VERT, iMin);
		SetCurrentDocumentRow(iMin);
		m_nCurrentCol = m_nDispCol = 1;
		bChanged = TRUE;
		break;
	case SB_BOTTOM:
		SetScrollPos(SB_VERT, iMax);
		SetCurrentDocumentRow(iMax);
		m_nCurrentCol = m_nDispCol = 1;
		bChanged = TRUE;
		break;
	case SB_THUMBTRACK:
		{
			SCROLLINFO si;
			ZeroMemory(&si, sizeof(SCROLLINFO));
			si.cbSize = sizeof(SCROLLINFO);
			si.fMask = SIF_TRACKPOS;

			if (GetScrollInfo(SB_VERT, &si))
			{
				int nShift = si.nTrackPos - GetVisibleRowsCount(m_nTopRow);
				if (nShift > 0)
				{
					bChanged = ShiftCurrentVisibleRowDown(nShift);
				}
				else if (nShift < 0)
				{
					bChanged = ShiftCurrentVisibleRowUp(-nShift);
				}
				SetScrollPos(SB_VERT, si.nTrackPos);
		}
		}
		break;
	case SB_ENDSCROLL:
		return;
	default:
		break;
	}

	int nTopDocRow = GetDocumentRow(1);
	int nBottomDocRow = GetDocumentRow(GetRowPerPage());
	int nCurDocRow = GetCurrentDocumentRow();
	if (nCurDocRow >= nTopDocRow && nCurDocRow <= nBottomDocRow)
	{
		SetCurCaretPos(GetCurrentDocumentRow(), m_nDispCol, FALSE, FALSE);
	}

	if (bChanged)
	{
		Invalidate(FALSE);
		UpdateWindow();
	}

	RecalcScrollBars();
}

void CXTPSyntaxEditCtrl::OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CXTPSmartPtrInternalT<CCmdTarget> ptrThisLock(this, TRUE);

#ifdef XTP_SYNTAXEDIT_SITENOTIFY_KEY
	if (!XTP_SYNTAXEDIT_SITENOTIFY_KEY(this, TRUE, nChar))
		return;
#endif

	if (nChar == 0)
	{
		return;
	}

	CWnd::OnSysKeyDown(nChar, nRepCnt, nFlags);
}

void CXTPSyntaxEditCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CXTPSmartPtrInternalT<CCmdTarget> ptrThisLock(this, TRUE);

#ifdef XTP_SYNTAXEDIT_SITENOTIFY_KEY
	if (!XTP_SYNTAXEDIT_SITENOTIFY_KEY(this, TRUE, nChar))
		return;
#endif

	if (nChar == 0)
	{
		return;
	}

	m_pToolTip->Hide();

	if (m_bDragging || m_bRightButtonDrag || m_pSelection->bSelectingRunning /* || m_bSelectionStarted*/)
	{
		OnSetCursor(0, 0, 0);
		return;
	}

	BOOL bCtrlKey  = (::GetKeyState(VK_CONTROL) & KF_UP) != 0;
	BOOL bShiftKey  = (::GetKeyState(VK_SHIFT) & KF_UP) != 0;

	BOOL bUpdateAll = FALSE;
	int nTopRow_prev = m_nTopRow;
	int nDispCol_prev = m_nDispCol;
	int nTextRow_prev = GetCurrentDocumentRow();
	int nRowCount_prev = GetRowCount();
	CXTPSyntaxEditSelection selData_prev = *m_pSelection;

	switch (nChar)
	{
	case VK_UP:
		m_nAutoIndentCol = 0;

		if (GetCurrentDocumentRow() < 1)
		{
			// Do nothing
		}
		else if (bCtrlKey)
		{
			m_pSelection->Reset_disp(GetCurrentDocumentRow(), m_nDispCol) ;

			ShiftCurrentVisibleRowUp(1, TRUE);
			SetCurCaretPos(GetCurrentDocumentRow(), m_nDispCol, FALSE);
		}
		else if (GetCurrentDocumentRow() >= 1)
		{
			if (!m_pSelection->IsSelExist())
				m_pSelection->Reset_disp(GetCurrentDocumentRow(), m_nDispCol);

			if (!bShiftKey)
			{
				m_pSelection->Reset_disp(m_pSelection->GetNormalStart_disp().nLine,
					m_nDispCol); //m_pSelection->GetNormalEnd_disp().nCol);

				SetCurCaretPos(m_pSelection->GetNormalStart_disp().nLine,
					m_pSelection->GetNormalEnd_disp().nCol, FALSE);
			}

			MoveCurrentVisibleRowUp(1);

			SetCurCaretPos(GetCurrentDocumentRow(), m_nDispCol, FALSE);

			if (bShiftKey)
				m_pSelection->SetEnd_disp(GetCurrentDocumentRow(), m_nDispCol);
			else
				m_pSelection->Reset_disp(GetCurrentDocumentRow(), m_nDispCol);

		}
		break;

	case VK_DOWN:
		m_nAutoIndentCol = 0;

		if (bCtrlKey)
		{
			m_pSelection->Reset_disp(GetCurrentDocumentRow(), m_nDispCol) ;

			ShiftCurrentVisibleRowDown(1, TRUE);
			SetCurCaretPos(GetCurrentDocumentRow(), m_nDispCol, FALSE);

		}
		else if (GetCurrentDocumentRow() <= GetRowCount())
		{
			if (!m_pSelection->IsSelExist())
				m_pSelection->Reset_disp(GetCurrentDocumentRow(), m_nDispCol);

			if (!bShiftKey)
			{
				m_pSelection->Reset_disp(m_pSelection->GetNormalEnd_disp().nLine,
					m_nDispCol); //m_pSelection->GetNormalEnd_disp().nCol);

				SetCurCaretPos(m_pSelection->GetNormalStart_str().nLine,
					m_pSelection->GetNormalEnd_disp().nCol, FALSE, TRUE);
			}

			MoveCurrentVisibleRowDown(1);

			SetCurCaretPos(GetCurrentDocumentRow(), m_nDispCol, FALSE/*, FALSE*/);

			if (bShiftKey)
				m_pSelection->SetEnd_disp(GetCurrentDocumentRow(), m_nDispCol);
			else
				m_pSelection->Reset_disp(GetCurrentDocumentRow(), m_nDispCol);

		}

		break;

	case VK_LEFT:
		m_nAutoIndentCol = 0;

		if (!m_pSelection->IsSelExist())
			m_pSelection->Reset_disp(GetCurrentDocumentRow(), m_nDispCol);

		if (!bShiftKey && m_pSelection->IsSelExist())
		{
			// move cursor to the selection begin
			m_pSelection->Reset_disp(m_pSelection->GetNormalStart_disp().nLine,
				m_pSelection->GetNormalStart_disp().nCol);
			SetCurCaretPos(m_pSelection->GetNormalStart_disp().nLine,
				m_pSelection->GetNormalStart_disp().nCol, FALSE/*, FALSE*/);
			break;
		}

		if (!bCtrlKey)
		{
			int nCurDocRow = GetCurrentDocumentRow();
			if ((nCurDocRow > 1 && m_nCurrentCol >= 1) ||
				(nCurDocRow == 1 && m_nCurrentCol > 1))
			{
				m_nCurrentCol--;
			}

			if (m_nCurrentCol < 1 && nCurDocRow > 1)
			{
				if (_IsVirtualSpaceActive())
					m_nCurrentCol = 1;
				else
					MoveCurrentVisibleRowUp(1);
			}

			LPCTSTR szText = GetLineText(GetCurrentDocumentRow());
			if (m_nCurrentCol < 1)
				m_nCurrentCol = (int)_tcsclen(szText) + 1;

			m_nDispCol = CalcDispCol(szText, m_nCurrentCol);
		}
		else
		{
			// This will modify Current Row, m_nCurrentCol, m_nDispCol
			FindWord(XTP_EDIT_FINDWORD_PREV);
		}

		if (bShiftKey)
			m_pSelection->SetEnd_disp(GetCurrentDocumentRow(), m_nDispCol);
		else
			m_pSelection->Reset_disp(GetCurrentDocumentRow(), m_nDispCol);

		break;

	case VK_RIGHT:
		m_nAutoIndentCol = 0;

		if (!m_pSelection->IsSelExist())
			m_pSelection->Reset_disp(GetCurrentDocumentRow(), m_nDispCol);

		if (!bShiftKey && m_pSelection->IsSelExist())
		{
			// move cursor to the end of the selection
			m_pSelection->Reset_disp(m_pSelection->GetNormalEnd_disp().nLine,
				m_pSelection->GetNormalEnd_disp().nCol);

			SetCurCaretPos(m_pSelection->GetNormalStart_str().nLine,
				m_pSelection->GetNormalEnd_disp().nCol, FALSE/*, FALSE*/);
			break;
		}

		if (!bCtrlKey)
		{
			int nCurDocRow = GetCurrentDocumentRow();
			LPCTSTR szText = GetLineText(nCurDocRow);
			const int nTextLen = (int)_tcsclen(szText);
			if (m_nCurrentCol > nTextLen && (nCurDocRow < GetRowCount()) && !_IsVirtualSpaceActive())
			{
				MoveCurrentVisibleRowDown(1);
				m_nCurrentCol = 1;
			}
			else if (m_nCurrentCol <= nTextLen || _IsVirtualSpaceActive())
			{
				m_nCurrentCol++;
			}

			m_nDispCol = CalcDispCol(szText, m_nCurrentCol);
		}
		else
		{
			FindWord(XTP_EDIT_FINDWORD_NEXT);
		}

		if (bShiftKey)
			m_pSelection->SetEnd_disp(GetCurrentDocumentRow(), m_nDispCol);
		else
			m_pSelection->Reset_disp(GetCurrentDocumentRow(), m_nDispCol);

		break;

	case VK_HOME:
		m_nAutoIndentCol = 0;

		if (!m_pSelection->IsSelExist())
			m_pSelection->Reset_disp(GetCurrentDocumentRow(), m_nDispCol);

		if (!bShiftKey && m_pSelection->IsSelExist())
		{
			// move cursor to the selection begin
			m_pSelection->Reset_disp(m_pSelection->GetNormalStart_disp().nLine,
				m_pSelection->GetNormalEnd_disp().nCol);
			SetCurCaretPos(m_pSelection->GetNormalStart_disp().nLine,
				m_pSelection->GetNormalEnd_disp().nCol, FALSE/*, FALSE*/);
		}

		if (bCtrlKey)
		{
			SetCurrentDocumentRow(1);
			EnsureVisibleRow(1);
			SetCurCaretPos(1, 1, FALSE/*, FALSE*/);
		}
		else
		{
			LPCTSTR szText = GetLineText(GetCurrentDocumentRow());

			// find number of tabs and spaces from the left
			int iCol = 1;
			LPCTSTR szPtr = szText;

			while (szPtr)
			{
				if (*szPtr == _T('\t') || *szPtr == _T(' '))
					iCol++;
				else
					break;

				szPtr = _tcsinc(szPtr);
			}

			if (m_nCurrentCol == iCol)
			{
				//m_nCurrentCol = 1;
				//m_nDispCol = 1;
				SetCurCaretPos(GetCurrentDocumentRow(), 1, FALSE/*, FALSE*/);
			}
			else
			{
				m_nDispCol = CalcDispCol(szText, iCol);
				SetCurCaretPos(GetCurrentDocumentRow(), m_nDispCol, FALSE/*, FALSE*/);
			}
		}

		if (bShiftKey)
			m_pSelection->SetEnd_disp(GetCurrentDocumentRow(), m_nDispCol);
		else
			m_pSelection->Reset_disp(GetCurrentDocumentRow(), m_nDispCol);

		break;
	case VK_END:
		{
			m_nAutoIndentCol = 0;

			if (!m_pSelection->IsSelExist())
				m_pSelection->Reset_disp(GetCurrentDocumentRow(), m_nDispCol);

			if (!bShiftKey && m_pSelection->IsSelExist())
			{
				// move cursor to the end of the selection
				m_pSelection->Reset_disp(m_pSelection->GetNormalEnd_disp().nLine,
					m_pSelection->GetNormalEnd_disp().nCol);

				SetCurCaretPos(m_pSelection->GetNormalStart_str().nLine,
					m_pSelection->GetNormalEnd_disp().nCol, FALSE/*, FALSE*/);
			}

			if (bCtrlKey)
			{
				SetCurrentDocumentRow(GetRowCount());
				EnsureVisibleRow(GetRowCount());
			}

			LPCTSTR szText = GetLineText(GetCurrentDocumentRow());
			int nLenC = (int)_tcsclen(szText);

			SetCurCaretPos(GetCurrentDocumentRow(), CalcDispCol(szText, nLenC + 1),
							FALSE/*, FALSE*/);
		}

		if (bShiftKey)
			m_pSelection->SetEnd_disp(GetCurrentDocumentRow(), m_nDispCol);
		else
			m_pSelection->Reset_disp(GetCurrentDocumentRow(), m_nDispCol);

		break;
	case VK_PRIOR:
		m_nAutoIndentCol = 0;
		{
			if (!m_pSelection->IsSelExist())
				m_pSelection->Reset_disp(GetCurrentDocumentRow(), m_nDispCol);

			if (!bShiftKey && m_pSelection->IsSelExist())
			{
				// move cursor to the selection begin
				m_pSelection->Reset_disp(m_pSelection->GetNormalStart_disp().nLine,
					m_pSelection->GetNormalEnd_disp().nCol);
				SetCurCaretPos(m_pSelection->GetNormalStart_disp().nLine,
					m_pSelection->GetNormalEnd_disp().nCol, FALSE/*, FALSE*/);
			}

			int iRowPerPage = GetRowPerPage();

			if ((m_nTopRow - iRowPerPage) < 1)
			{
				SetCurrentDocumentRow(1);
				EnsureVisibleRow(1);
			}
			else
			{
				MoveCurrentVisibleRowUp(iRowPerPage);
			}

			SetCurCaretPos(GetCurrentDocumentRow(), m_nDispCol, FALSE/*, FALSE*/);

			if (bShiftKey)
				m_pSelection->SetEnd_disp(GetCurrentDocumentRow(), m_nDispCol);
			else
				m_pSelection->Reset_disp(GetCurrentDocumentRow(), m_nDispCol);
		}

		break;

	case VK_NEXT:
		m_nAutoIndentCol = 0;

		{
			if (!m_pSelection->IsSelExist())
				m_pSelection->Reset_disp(GetCurrentDocumentRow(), m_nDispCol);

			if (!bShiftKey && m_pSelection->IsSelExist())
			{
				// move cursor to the end of the selection
				m_pSelection->Reset_disp(m_pSelection->GetNormalEnd_disp().nLine,
					m_pSelection->GetNormalEnd_disp().nCol);

				SetCurCaretPos(m_pSelection->GetNormalStart_str().nLine,
					m_pSelection->GetNormalEnd_disp().nCol, FALSE/*, FALSE*/);
			}

			int iRowPerPage = GetRowPerPage();

			if ((m_nTopRow + iRowPerPage) > GetRowCount())
			{
				SetCurrentDocumentRow(GetRowCount());
				EnsureVisibleRow(GetRowCount());
			}
			else
			{
				MoveCurrentVisibleRowDown(iRowPerPage);
			}

			SetCurCaretPos(GetCurrentDocumentRow(), m_nDispCol, FALSE/*, FALSE*/);

			if (bShiftKey)
				m_pSelection->SetEnd_disp(GetCurrentDocumentRow(), m_nDispCol);
			else
				m_pSelection->Reset_disp(GetCurrentDocumentRow(), m_nDispCol);
		}

		break;

	case VK_DELETE:
		{
			BOOL bDeleted = TRUE;

			if (m_pSelection->IsSelExist() && !bCtrlKey)
			{
				bDeleted = DeleteSelection();
			}
			else if (!bCtrlKey)
			{
				if (GetAutoIndent() && m_nAutoIndentCol > 0)
				{
					CString strInsertText(
						CString(_T('\t'), m_nInsertTabCount) +
						CString(_T(' '), m_nInsertSpaceCount));

					int iNewDispCol = (m_nInsertTabCount * GetTabSize()) + m_nInsertSpaceCount + 1;
					int iNewCol = m_nInsertTabCount + m_nInsertSpaceCount + 1;
					//**----------------------
					OnBeforeEditChanged(GetCurrentDocumentRow(), 1);

					m_pBuffer->InsertText(strInsertText, GetCurrentDocumentRow(), 1);

					OnEditChanged(GetCurrentDocumentRow(), 1, GetCurrentDocumentRow(), iNewCol, xtpEditActInsert);
					//**----------------------

					m_nCurrentCol = iNewCol;
					m_nDispCol = iNewDispCol;

					m_nAutoIndentCol = 0;
				}

				bDeleted = DeleteChar(GetCurrentDocumentRow(), m_nCurrentCol, xtpEditDelPosAfter);

				m_pSelection->Reset_disp(GetCurrentDocumentRow(), m_nDispCol);
			}
			else
			{
				BOOL bSelectionExist = m_pSelection->IsSelExist();
				int nDelFlags = 0;

				if (bSelectionExist)
				{
					nDelFlags |= xtpEditForceRedraw;
					DeleteSelection();
				}

				int iStartRow = GetCurrentDocumentRow();
				int iStartCol = m_nCurrentCol;

				FindWord(XTP_EDIT_FINDWORD_NEXT);

				int iEndRow = GetCurrentDocumentRow();
				int iEndCol = m_nCurrentCol;

				DeleteBuffer(iStartRow, iStartCol, iEndRow, iEndCol, nDelFlags);

				m_pSelection->Reset_disp(GetCurrentDocumentRow(), m_nDispCol);

				if (bSelectionExist)
					m_pBuffer->GetUndoRedoManager()->ChainLastCommand();
			}

			bUpdateAll = TRUE;

			m_nAutoIndentCol = 0;
		}

		break;
	case VK_BACK:
		if (m_nAutoIndentCol > 0)
		{
			if (m_nInsertSpaceCount == 0)
				m_nInsertTabCount--;
			else
				m_nInsertSpaceCount--;

			if (m_nInsertTabCount || m_nInsertSpaceCount)
			{
				m_nAutoIndentCol = (m_nInsertTabCount * GetTabSize()) + m_nInsertSpaceCount + 1;
				m_nDispCol = m_nAutoIndentCol;
			}
			else
			{
				m_nAutoIndentCol = 0;
				m_nDispCol = 1;
			}
		}
		else if (!bCtrlKey)
		{
			if (m_pSelection->IsSelExist())
			{
				DeleteSelection();
			}
			else
			{
				DeleteChar(GetCurrentDocumentRow(), m_nCurrentCol, xtpEditDelPosBefore);
			}
			m_pSelection->Reset_disp(GetCurrentDocumentRow(), m_nDispCol);
		}
		else
		{
			BOOL bSelectionExist = m_pSelection->IsSelExist();

			if (bSelectionExist)
			{
				DeleteSelection();
			}

			int iStartRow = GetCurrentDocumentRow();
			int iStartCol = m_nCurrentCol;

			FindWord(XTP_EDIT_FINDWORD_PREV);

			int iEndRow = GetCurrentDocumentRow();
			int iEndCol = m_nCurrentCol;

			DeleteBuffer(iStartRow, iStartCol, iEndRow, iEndCol);

			m_pSelection->Reset_disp(GetCurrentDocumentRow(), m_nDispCol);

			if (bSelectionExist)
				m_pBuffer->GetUndoRedoManager()->ChainLastCommand();

		}

		bUpdateAll = TRUE;

		break;
	case VK_INSERT:
		{
			NotifyParent(XTP_EDIT_NM_INSERTKEY);
			SetCurCaretPos(GetCurrentDocumentRow(), m_nDispCol, FALSE/*, FALSE*/);

			SetOverwriteMode(!m_pBuffer->GetOverwriteFlag());
		}
		break;
	case VK_SPACE:
		if (bCtrlKey)
		{
			CPoint pt(CWnd::GetCaretPos());
			pt.y += m_pDrawTextProcessor->GetRowHeight();
			ClientToScreen(&pt);

			CString strText(GetLineText(GetCurrentDocumentRow()));
			int nTextPos = m_nCurrentCol - 2;

			CString strSearch;
//NON-COVERED CASE <<
			if (m_pAutoComplete->m_strDelims.IsEmpty())
				m_pAutoComplete->m_strDelims = _T(" \t");
//NON-COVERED CASE <<
			while (strText.GetLength() > 0 && nTextPos >= 0 && m_pAutoComplete->m_strDelims.Find(strText.GetAt(nTextPos)) < 0)
			{
				strSearch = strText.GetAt(nTextPos--) + strSearch;
			}
			SetAutoCompleteList();
			m_pAutoComplete->Show(pt, strSearch);
		}
		break;

	}

	BOOL bChanged = nTopRow_prev != m_nTopRow || nDispCol_prev != m_nDispCol ||
					nTextRow_prev != GetCurrentDocumentRow() || nRowCount_prev != GetRowCount() ||
					selData_prev != *m_pSelection;
	if (bChanged)
	{
		int nCurDocRow = GetCurrentDocumentRow();

		SetCurCaretPos(nCurDocRow, m_nDispCol);

		m_pBuffer->GetUndoRedoManager()->SetGroupInsertMode(FALSE);

		Invalidate(FALSE);
		UpdateWindow();

		if (nTopRow_prev != m_nTopRow)
			RecalcScrollBars();

		DWORD dwUpdate = (XTP_EDIT_UPDATE_HORZ|XTP_EDIT_UPDATE_VERT);

		if (bUpdateAll)
			dwUpdate |= XTP_EDIT_UPDATE_DIAG;

		UpdateScrollPos(dwUpdate);
	}

	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CXTPSyntaxEditCtrl::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (m_bDragging || m_bRightButtonDrag)
	{
		OnSetCursor(0, 0, 0);
		return;
	}

	NotifySelInit();

	BOOL bAltKey  = (::GetKeyState(VK_MENU) & KF_UP) != 0;
	BOOL bCtrlKey  = (::GetKeyState(VK_CONTROL) & KF_UP) != 0;
	BOOL bShiftKey  = (::GetKeyState(VK_SHIFT) & KF_UP) != 0;
	CString strCurCRLF = m_pBuffer->GetCurCRLF();
	LPCTSTR szCurCRLF = strCurCRLF;

	BOOL bProcessed = FALSE;

	if (nChar == VK_TAB && !bCtrlKey)
	{
		if (bShiftKey)
			bProcessed = DecreaseIndent();
		else
			bProcessed = IncreaseIndent();
	}

	if (!bProcessed && (bCtrlKey && !bAltKey) && nChar == 0x0C) // 0x0C is 'l' or 'L'
	{
		// Delete the entire line or the selection
		DeleteSelectedLines(GetCurrentDocumentRow());
	}
	else if (nChar != VK_BACK && nChar != VK_ESCAPE && !(bCtrlKey && !bAltKey) && !bProcessed)
	{
		if (!CanEditDoc())
		{
			return;
		}
		BOOL bModified = FALSE;
		//**----------------------
		OnBeforeEditChanged(GetCurrentDocumentRow(), m_nCurrentCol);
		//**----------------------

		int nChainActionCount = 1;
		BOOL bRedraw = FALSE;
		BOOL bPrevOverwriteMode = m_pBuffer->GetOverwriteFlag();

		if (m_pSelection->IsSelExist())
		{
			if (!m_pBuffer->GetOverwriteFlag() || (m_pBuffer->GetOverwriteFlag() && nChar != VK_RETURN))
			{
				BOOL bRes = DeleteSelection();
				bModified |= bRes;

				nChainActionCount = 2;
				bRedraw = TRUE;
				m_pBuffer->SetOverwriteFlag(FALSE);
			}
		}

		TCHAR szText[3];
		szText[0] = (TCHAR)nChar;
		szText[1] = NULL;

		// DBCS Support (specially for IME)
#ifndef _UNICODE
		//if (m_bIsImeEnabled)
		if (isleadbyte((int)nChar) && m_chPrevLeadByte == 0)
		{
			// do not process lead byte. just remember it and exit.
			m_chPrevLeadByte = (BYTE)nChar;
			return;
		}

		if (m_chPrevLeadByte)
		{
			szText[0] = (TCHAR)m_chPrevLeadByte;
			szText[1] = (TCHAR)nChar;
			szText[2] = NULL;
		}
		m_chPrevLeadByte = 0;
#endif

		if (nChar == _T('\r'))
		{
			lstrcpy(szText, szCurCRLF);
			szText[2] = NULL;

			// here we should expand collapsed block if we are on it
			if (GetLineMarksManager()->HasRowMark(GetCurrentDocumentRow(), xtpEditLMT_Collapsed))
			{
				GetLineMarksManager()->DeleteLineMark(GetCurrentDocumentRow(), xtpEditLMT_Collapsed);
			}
		}

		int iNewDispCol  = m_nDispCol;
		int iNewCol      = m_nCurrentCol;
		int iNewRow      = GetCurrentDocumentRow();
		int iEditRowFrom = iNewRow;
		int iEditRowTo   = iNewRow;

		CString strTextToIns;

		// Create text to insert
		BOOL bCanProcess =
			CreateInsertText(szText,
							strTextToIns,
							iNewRow,
							iNewCol,
							iNewDispCol,
							iEditRowFrom,
							iEditRowTo,
							nChainActionCount);

		BOOL bGroupInsMode = FALSE;

		if (!bPrevOverwriteMode)
		{
			static LPCTSTR szSeps = _T(" [{()}];.,\t\r\n\"");

			if (_tcschr(szSeps, (TCHAR)nChar))
				bGroupInsMode = FALSE;
			else
				bGroupInsMode = TRUE;

			m_pBuffer->GetUndoRedoManager()->SetGroupInsertMode(bGroupInsMode);
		}
		else
			m_pBuffer->GetUndoRedoManager()->SetGroupInsertMode(FALSE);

		if (bCanProcess)
		{
			int nInsCol = m_nCurrentCol;
			if (nChar == VK_RETURN && m_nAutoIndentCol)
				nInsCol = min(m_nCurrentCol, m_pBuffer->GetLineTextLengthC(GetCurrentDocumentRow()) + 1);

			BOOL bInsRes = m_pBuffer->InsertText(strTextToIns, GetCurrentDocumentRow(), nInsCol, TRUE);
			bModified |= bInsRes;

			if (nChainActionCount > 1)
			{
				m_pBuffer->GetUndoRedoManager()->ChainLastCommand();
				m_pBuffer->GetUndoRedoManager()->SetLastCommandText(XTP_IDS_EDIT_TYPING);
			}
		}

		//m_pBuffer->GetUndoRedoManager()->SetGroupInsertMode(FALSE);
		m_pBuffer->SetOverwriteFlag(bPrevOverwriteMode);

		//**----------------------
		OnEditChanged(GetCurrentDocumentRow(), m_nCurrentCol, iNewRow, iNewCol, xtpEditActInsert);
		//**----------------------
		BOOL bInsAt0 = (m_nCurrentCol == 1) || GetLineText(GetCurrentDocumentRow()).GetLength() == 0;

		m_nCurrentCol = iNewCol;
		m_nDispCol    = iNewDispCol;

		SetCurrentDocumentRow(iNewRow);

		BOOL bNewRow = FALSE;

		if (nChar == VK_RETURN)
			bNewRow = TRUE;

		UINT nAction = XTP_EDIT_EDITACTION_MODIFYROW;

		if (bNewRow)
			nAction |= XTP_EDIT_EDITACTION_INSERTROW;

		if (bNewRow && bInsAt0)
			nAction |= XTP_EDIT_EDITACTION_INSERTROW_NEW;


		NotifyEditChanged(iEditRowFrom, iEditRowTo, nAction);

//      if (nChar == VK_RETURN)
//      {
//          DoAutoIndentIfNeed(nDispCol_prev);
//      }

		SetCurCaretPos(GetCurrentDocumentRow(), m_nDispCol);
		RecalcScrollBars();

		Invalidate(FALSE);
		UpdateWindow();

		UpdateScrollPos();

		if (bModified)
			SetDocModified();

	}
	else if (!bProcessed && nChar == VK_ESCAPE)
	{
		Unselect();
		UpdateScrollPos();
	}

	// AutoComplete Processing
	if (m_pAutoComplete->IsOpenTag((TCHAR)nChar))
	{
		CPoint pt(CWnd::GetCaretPos());
		pt.y += m_pDrawTextProcessor->GetRowHeight();
		ClientToScreen(&pt);
		SetAutoCompleteList();
		m_pAutoComplete->Show(pt);
	}

	CWnd::OnChar(nChar, nRepCnt, nFlags);
}

void CXTPSyntaxEditCtrl::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CXTPSmartPtrInternalT<CCmdTarget> ptrThisLock(this, TRUE);

	#ifdef XTP_SYNTAXEDIT_SITENOTIFY_KEY
		if (!XTP_SYNTAXEDIT_SITENOTIFY_KEY(this, FALSE, nChar))
			return;
	#endif

	if (nChar == 0)
	{
		return;
	}

	if (m_bDragging)
	{
		OnSetCursor(0, 0, 0);
		return;
	}

	CWnd::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CXTPSyntaxEditCtrl::SetCurPos(int nTextRow, int nDispCol, BOOL bRemainSelected, BOOL bForceVisible)
{
	if (nTextRow > GetRowCount())
		nTextRow = GetRowCount();

	//reset autoindent
	m_nAutoIndentCol = 0;

	CString strText = GetLineText(nTextRow);

	// validate col
	m_nCurrentCol = CalcAbsCol(strText, nDispCol);
	m_nDispCol = CalcDispCol(strText, m_nCurrentCol);

	if (!bRemainSelected && m_pSelection->IsSelExist())
	{
		m_pSelection->Reset_disp(nTextRow, m_nDispCol);
	}

	SetCurCaretPos(nTextRow, m_nDispCol, TRUE, bForceVisible);

	if (::IsWindow(m_hWnd) || m_hWnd)
	{
		Invalidate(FALSE);
		UpdateWindow();
	}
	else
		InvalidateAll();

	UpdateScrollPos(XTP_EDIT_UPDATE_HORZ|XTP_EDIT_UPDATE_VERT);
}

void CXTPSyntaxEditCtrl::SetCurCaretPos(int nTextRow, int nDispCol, BOOL bRowColNotify, BOOL bEnsureVisible)
{
	if (!::IsWindow(m_hWnd) || !m_hWnd)
		return;

	if (bEnsureVisible)
		_EnsureVisible(nTextRow, nDispCol);

	int nDispRow = GetVisibleRow(nTextRow);
	if (nDispRow > m_pDrawTextProcessor->GetRowsCount(TRUE) + 10)
		nDispRow = m_pDrawTextProcessor->GetRowsCount(TRUE) + 10;

	if (m_nAutoIndentCol > 0)
		nDispCol = m_nAutoIndentCol;
	int nCol = max(0, nDispCol - 1);

	m_nDispCol = nCol + 1;
	m_nCurrentCol = CalcAbsCol(nTextRow, m_nDispCol);
	SetCurrentDocumentRow(nTextRow);

	if (bRowColNotify)
		NotifyCurRowCol(GetCurrentDocumentRow(), m_nDispCol);

	Invalidate(FALSE);

	//TRACE(_T("CXTPSyntaxEditCtrl::SetCurCaretPos(%d, %d) \n"), nDispRow, nDispCol);

}

void CXTPSyntaxEditCtrl::_EnsureVisible(int nTextRow, int nDispCol/*, BOOL bRedraw*/)
{
	BOOL bVScrolled = EnsureVisibleRow(nTextRow);

	nDispCol = nDispCol;
	if (m_nAutoIndentCol > 0)
		nDispCol = m_nAutoIndentCol;

	int nDispRow = GetVisibleRow(nTextRow);

	BOOL bHScrolled = EnsureVisibleCol(nDispRow, nDispCol);

//  BOOL bVirtSpace = _IsVirtualSpaceActive() || m_nAutoIndentCol;
//  m_nCurrentCol = m_pDrawTextProcessor->DispPosToStrPos(nDispRow - 1, m_nDispCol-1, bVirtSpace) + 1;

	if (bVScrolled || bHScrolled /*|| bRedraw*/)
	{
		Invalidate(FALSE);
	}
}

BOOL CXTPSyntaxEditCtrl::EnsureVisibleRow(int nTextRow)
{
	int nPrevTopRow = m_nTopRow;

	int nDispRowsCount = m_pDrawTextProcessor->GetRowsCount(FALSE);
	int nCurPageMaxTextRow = GetDocumentRow(nDispRowsCount);

	if (nDispRowsCount == 0)
	{
		return FALSE;
	}

	if (nTextRow > nCurPageMaxTextRow)
	{
		m_nTopRow += nTextRow - nCurPageMaxTextRow;
	}
	else if (nTextRow < m_nTopRow)
	{
		m_nTopRow = max (1, nTextRow);
	}

	if (GetVertScrollBar() /*&& nPrevTopRow != m_nTopRow*/)
	{
		BOOL bEnableVertScrl = (CalculateVisibleRow(1, GetRowCount()) > nDispRowsCount);

		EnableScrollBarCtrl(SB_VERT, bEnableVertScrl);
		if (bEnableVertScrl)
			SetScrollPos(SB_VERT,  CalculateVisibleRow(1, m_nTopRow));
	}

	return nPrevTopRow != m_nTopRow;
}

BOOL CXTPSyntaxEditCtrl::EnsureVisibleCol(int nDispRow, int nDispCol)
{
	int nTextRow = GetDocumentRow(nDispRow);
	UpdateRowInfoInternally(nTextRow);

	int nColWidth = 0;
	int nColPos = m_pDrawTextProcessor->GetColPosX(nDispRow - 1, nDispCol, &nColWidth, IsEnabledVirtualSpace());
	int nOffsetX = m_pDrawTextProcessor->GetScrollXOffset();

	CRect rcText = m_pDrawTextProcessor->GetTextRect();
	int nNewScrollOffset = -1;

	if (nColPos - max(14, nColWidth * 2) < rcText.left)
	{
		nNewScrollOffset = max(0, nOffsetX - (rcText.left - nColPos )- nColWidth * 15);
	}
	else if (nColPos + nColWidth >= rcText.right)
	{
		nNewScrollOffset = nOffsetX + (nColPos + nColWidth) - rcText.right + min(rcText.Width()/3, nColWidth * 15);
	}

	if (nNewScrollOffset >= 0)
	{
		SetScrollPos(SB_HORZ, nNewScrollOffset);
		m_pDrawTextProcessor->SetScrollXOffset(nNewScrollOffset);
	}

	return nNewScrollOffset >= 0;
}

int CXTPSyntaxEditCtrl::CalcDispCol(int nTextRow, int nActualCol) const
{
	CString strLine = GetLineText(nTextRow);
	return CalcDispCol(strLine, nActualCol);
}

int CXTPSyntaxEditCtrl::CalcAbsCol(int nTextRow, int nDispCol) const
{
	CString strLine = GetLineText(nTextRow);
	return CalcAbsCol(strLine, nDispCol);
}

int CXTPSyntaxEditCtrl::CalcDispCol(LPCTSTR szText, int nActualCol) const
{
	int iDispCol = 0;
	int nAbsCol = 1;

	for (LPCTSTR pcszChar = szText; pcszChar && *pcszChar; pcszChar = _tcsinc(pcszChar))
	{
		if (nAbsCol >= nActualCol)
			break;

		if (*pcszChar == _T('\t'))
		{
			// Now calculate tab size
			iDispCol += (GetTabSize() - (iDispCol % GetTabSize()));
		}
#ifdef XTP_FIXED
		else if (isleadbyte( *pcszChar ))
// multi byte character : DisplayLength 2
			iDispCol += 2;
#endif
		else
			iDispCol++;

		nAbsCol++;
	}

	iDispCol++;

	if (nAbsCol < nActualCol)
	{
		if (_IsVirtualSpaceActive())
			iDispCol += nActualCol - nAbsCol;
		else
			iDispCol++;
	}

	return iDispCol;
}

int CXTPSyntaxEditCtrl::CalcAbsCol(LPCTSTR szText, int iDispCol) const
{
	int nAbsCol = 0;
	int nDispColCalc = 0;

	for (LPCTSTR pcszChar = szText; pcszChar && *pcszChar; pcszChar = _tcsinc(pcszChar))
	{
		if (nDispColCalc >= iDispCol)
			break;

		//if (szText[i] == _T('\t'))
		if (*pcszChar == _T('\t'))
			nDispColCalc += (GetTabSize() - (nDispColCalc % GetTabSize()));
#ifdef XTP_FIXED
		else if (isleadbyte( *pcszChar ))
// multi byte character : DisplayLength 2
			nDispColCalc += 2;
#endif
		else
			nDispColCalc++;

		nAbsCol++;
	}

	if (nDispColCalc < iDispCol)
	{
		if (_IsVirtualSpaceActive())
			nAbsCol += iDispCol - nDispColCalc;
		else
			nAbsCol++;
	}

	if (nAbsCol == 0)
		nAbsCol = 1;

	return nAbsCol;
}

int CXTPSyntaxEditCtrl::CalcValidDispCol(LPCTSTR szText, int iCol) const
{
	int iDispCol = 0;

	for (LPCTSTR pcszChar = szText; pcszChar && *pcszChar; pcszChar = _tcsinc(pcszChar))
	{
		if (iDispCol >= iCol-1)
			break;

		if (*pcszChar == _T('\t'))
		{
			// Now calculate tab size
			iDispCol += (GetTabSize() - (iDispCol % GetTabSize()));
		}
#ifdef XTP_FIXED
		else if (isleadbyte(*pcszChar))
// multi byte character : DisplayLength 2
			iDispCol += 2;
#endif
		else
			iDispCol++;
	}

	return ++iDispCol;
}

int CXTPSyntaxEditCtrl::CalcMaximumWidth(LPCTSTR szText)
{
	int iMaxWidth = 1;

	for (LPCTSTR pcszChar = szText; pcszChar && *pcszChar; pcszChar = _tcsinc(pcszChar))
	{
		if (*pcszChar == _T('\t'))
		{
			// Now calculate tab size
			iMaxWidth += ((GetTabSize() + 1) - (iMaxWidth - ((iMaxWidth / GetTabSize()) * GetTabSize())));
		}
#ifdef XTP_FIXED
		else if (isleadbyte(*pcszChar))
// multi byte character : DisplayLength 2
			iMaxWidth += 2;
#endif
		else
			iMaxWidth++;
	}

	return iMaxWidth;
}

void CXTPSyntaxEditCtrl::GetLineNodeRect(int nRow, CRect& rcNode, CRect* prcNodeFull) const
{
	// calculate node rect
	CRect rcNodes, rcText;
	CalcEditRects(NULL, NULL, &rcNodes, &rcText);

	int nRowHeight = m_pDrawTextProcessor->GetRowHeight();
	int nYPos = rcNodes.top + nRow * nRowHeight;

	rcNode = rcNodes;
	rcNode.top = nYPos + (nRowHeight - m_nNodesWidth) / 2;
	rcNode.bottom = rcNode.top + m_nNodesWidth - 1;

	if (prcNodeFull)
	{
		prcNodeFull->left = rcNodes.left;
		prcNodeFull->right = rcText.left;
		prcNodeFull->top = nYPos;
		prcNodeFull->bottom = nYPos + nRowHeight;
	}
}

BOOL CXTPSyntaxEditCtrl::ProcessCollapsedTextEx(CDC* pDC, XTP_EDIT_COLLAPSEDBLOCK* pCoDrawBlk,
								const XTP_EDIT_TEXTBLOCK& txtBlk,
								CRect& rcCoBlk)
{
	if (!pCoDrawBlk)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	if (txtBlk.nPos < pCoDrawBlk->collBlock.lcStart.nCol)
	{
		return FALSE;
	}

	ProcessCollapsedText(pDC, pCoDrawBlk, rcCoBlk);

	return TRUE;
}

void CXTPSyntaxEditCtrl::ProcessCollapsedText(CDC* pDC, XTP_EDIT_COLLAPSEDBLOCK* pCoDrawBlk,
										CRect& rcCoBlk)
{
	CString strText = pCoDrawBlk->collBlock.strCollapsedText; // "[..]"

	CXTPFontDC fontDC(pDC, GetPaintManager()->GetFont());

	rcCoBlk.right = rcCoBlk.left + pDC->GetTextExtent(strText).cx + 3;
	pCoDrawBlk->rcCollMark = rcCoBlk;
}

CString CXTPSyntaxEditCtrl::GetCollapsedText(XTP_EDIT_COLLAPSEDBLOCK* pCoDrawBlk, int nMaxLinesCount) const
{
	if (!pCoDrawBlk || !m_pBuffer)
	{
		ASSERT(FALSE);
		return _T("");
	}
	CString strCoText;

	int nLine1 = pCoDrawBlk->collBlock.lcStart.nLine;
	int nLine2 = pCoDrawBlk->collBlock.lcEnd.nLine;
	int nLine2max = min(nLine2, nLine1 + nMaxLinesCount-1);

	int nCol1 = pCoDrawBlk->collBlock.lcStart.nCol;
	int nCol2 = pCoDrawBlk->collBlock.lcEnd.nCol;

	for (int nLine = nLine1; nLine <= nLine2max; nLine++)
	{
		CString strTmp = m_pBuffer->GetLineText(nLine);
		int nTextLen = strTmp.GetLength();

		if (nTextLen)
		{
			if (nLine == nLine2)
			{
				ASSERT(nCol2 < nTextLen+2);
				nCol2 = max(0, min(nCol2, nTextLen-1));
				strTmp = strTmp.Left(nCol2+1);
			}

			if (nLine == nLine1)
			{
				nTextLen = strTmp.GetLength();
				ASSERT(nCol1 < nTextLen+2);

				if (nCol1 < nTextLen)
				{
					strTmp = strTmp.Mid(nCol1);
				}
				else
				{
					strTmp.Empty();
				}
			}
		}
		if (!strCoText.IsEmpty())
		{
			strCoText += _T("\r\n");
		}
		strCoText += strTmp;
	}

	return strCoText;
}

int CXTPSyntaxEditCtrl::ExpandChars(CDC* pDC, LPCTSTR pszChars, int nCurPos, CString& strBuffer)
{
	int nTabSize = GetTabSize();

	const int nLength = (int)_tcslen(pszChars);
	if (nLength == 0)
		return 0;

	int nActualOffset = nCurPos;
	int I;
	for (I = 0; I < nLength; I++)
	{
		if (pszChars[I] == _T('\t'))
			nActualOffset += (nTabSize - nActualOffset % nTabSize);
		else
			nActualOffset ++;
	}

	int nActualLength = nActualOffset - nCurPos;

	for (I = 0; I < nLength; I++)
	{
		if (pszChars[I] == _T('\t'))
		{
			int nSpaces = nTabSize - (nCurPos % nTabSize);

			BOOL bFirstChar = TRUE;
			while (nSpaces > 0)
			{
				if (bFirstChar && IsEnabledWhiteSpace() && !pDC->IsPrinting())
				{
					strBuffer += (TCHAR)(unsigned char)0xBB;
					bFirstChar = FALSE;
				}
				else
					strBuffer += _T(' ');

				nSpaces--;
				nCurPos++;
			}
		}
		else
		{
			if (IsEnabledWhiteSpace() && pszChars[I] == _T(' ') && !pDC->IsPrinting())
				strBuffer += (TCHAR)(unsigned char)0xB7;
			else
				strBuffer += pszChars[I];

			nCurPos++;
		}
	}

	return nActualLength;
}

void CXTPSyntaxEditCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (IsViewOnly() && !m_bAllowExpandCollapse)
		return;

	if (!IsViewOnly())
	{
		SetFocus();

		// Manage AutoCompleteView
		if (m_pAutoComplete->IsActive())
		{
			CRect rcACcomplete;
			m_pAutoComplete->GetWindowRect(&rcACcomplete);
			ScreenToClient(&rcACcomplete);
			if (rcACcomplete.PtInRect(point))
				return;
			else
				m_pAutoComplete->Hide();
		}

		m_bDragging = FALSE;
		m_bRightButtonDrag = FALSE;
		m_nAutoIndentCol = 0;

		m_pSelection->bSelectingRunning = FALSE;

		if (m_pSelection->IsSelExist())
			Invalidate(FALSE);

		NotifySelInit();

		SetCapture();
	}

	BOOL bCtrlKey  = (::GetKeyState(VK_CONTROL) & KF_UP) != 0;
	BOOL bShiftKey  = (::GetKeyState(VK_SHIFT) & KF_UP) != 0;
	BOOL bAltKey  = (::GetKeyState(VK_MENU) & KF_UP) != 0;

	CXTPClientRect rcClient(this);

	CRect rcBookmark, rcLineNum, rcNodes, rcText;
	rcClient.bottom = rcClient.top + m_pDrawTextProcessor->GetRowHeight() * m_pDrawTextProcessor->GetRowsCount(FALSE);
	CalcEditRects(&rcBookmark, &rcLineNum, &rcNodes, &rcText, &rcClient);

	CRect rcLineNumAndNodes(rcLineNum);
	rcLineNumAndNodes.right = rcNodes.right;

	int nNewRow = 0, nNewCol = 0, nNewDispRow = 0, nNewDispCol = 0;
	RowColFromPoint(point, &nNewRow, &nNewCol, &nNewDispRow, &nNewDispCol);
	BOOL bSelFromLeftBar = rcLineNumAndNodes.PtInRect(point);

//  TRACE(_T("*** OnLButtonDown. RowColFromPoint: nNewRow=%d, nNewCol=%d, nNewDispRow=%d, nNewDispCol=%d \n"),
//          nNewRow, nNewCol, nNewDispRow, nNewDispCol);
	if (!IsViewOnly())
	{
		if (GetCurrentDocumentRow() != nNewRow || m_nDispCol != nNewDispCol)
		{
			// reset undo buffer group mode processing
			m_pBuffer->GetUndoRedoManager()->SetGroupInsertMode(FALSE);
		}

		if (rcBookmark.PtInRect(point))
		{
			m_pSelection->Reset_disp(nNewRow, nNewDispCol);

			if (!NotifyMarginLBtnClick(nNewRow, nNewDispRow))
			{
				AddRemoveBreakPoint(nNewRow);
				SetCurPos(nNewRow, 1);
			}

			CWnd::OnLButtonDown(nFlags, point);
			NotifyMouseEvent(XTP_EDIT_NM_LBUTTONDOWN, nFlags, point);
			return;
		}
	}
	if (!IsViewOnly() || (IsViewOnly() && m_bAllowExpandCollapse))
	{
		if (GetCollapsibleNodes() && rcNodes.PtInRect(point))
		{
			CXTPEmptyRect rcNode;

			DWORD dwType = XTP_EDIT_ROWNODE_NOTHING;
			if (GetRowNodes(nNewRow, dwType) && (dwType & (XTP_EDIT_ROWNODE_COLLAPSED | XTP_EDIT_ROWNODE_EXPANDED)))
			{
				GetLineNodeRect(nNewDispRow-1, rcNode);
			}

			if (rcNode.PtInRect(point) && m_pBuffer)
			{
				m_pSelection->Reset_disp(m_pSelection->GetEnd_disp().nLine, m_pSelection->GetEnd_disp().nCol);

				// expand/collapse a node
				CollapseExpandBlock(nNewRow);
				SetCurPos(nNewRow, 1);

				CWnd::OnLButtonDown(nFlags, point);
				NotifyMouseEvent(XTP_EDIT_NM_LBUTTONDOWN, nFlags, point);
				return;
			}
		}
	}
	if (!IsViewOnly())
	{
		if (rcText.PtInRect(point) && m_pSelection->IsInSel_disp(nNewRow, nNewDispCol))
		{
			m_pSelection->bSelectingRunning = FALSE;

			if (!m_bEnableOleDrag)
				m_bDragging = TRUE;
			else
				NotifyParent(XTP_EDIT_NM_STARTOLEDRAG);

			CWnd::OnLButtonDown(nFlags, point);
			NotifyMouseEvent(XTP_EDIT_NM_LBUTTONDOWN, nFlags, point);
			return;
		}

		if (bAltKey)
		{
			m_pSelection->Reset_disp(nNewRow, nNewDispCol);
			m_pSelection->bSelectingRunning = TRUE;
			m_pSelection->bBlockSelectionMode = TRUE;
		}
		else if (bShiftKey)
		{
			m_pSelection->SetEnd_disp(nNewRow, nNewDispCol);

			m_pSelection->bSelectingRunning = TRUE;
			m_pSelection->bBlockSelectionMode = FALSE;
			m_pSelection->bWordSelectionMode = bCtrlKey;
		}
		else
		{
			m_pSelection->Reset_disp(nNewRow, nNewDispCol);
			m_pSelection->bSelectingRunning = TRUE;
			m_pSelection->bWordSelectionMode = bCtrlKey;
			if (bSelFromLeftBar)
				m_pSelection->nSelStartTextRowFromLeftBar = m_pSelection->GetStart_str().nLine;
		}

	//  TRACE(_T("Selection: (row=%d, strPos=%d, DispCol=%d)-(row=%d, strPos=%d, DispCol=%d) \n"),
	//          m_pSelection->GetStart_str().nLine, m_pSelection->GetStart_str().nCol, m_pSelection->GetStart_disp().nCol,
	//          m_pSelection->GetEnd_str().nLine, m_pSelection->GetEnd_str().nCol, m_pSelection->GetEnd_disp().nCol);

		if (m_pSelection->bWordSelectionMode)
		{
			XTP_EDIT_LINECOL lcWord1, lcWord2;
			BOOL bOverSpace = FALSE;

			UINT nFindDir = m_pSelection->IsSelNormal() ? XTP_EDIT_FINDWORD_NEXT : XTP_EDIT_FINDWORD_PREV;
			BOOL bFind = FindWordEx_str(nFindDir, m_pSelection->GetEnd_str(), lcWord1, lcWord2, bOverSpace);
	//      TRACE(_T("1-FindWordEx_str = %d (%d, %d - %d, %d)"), bFind,
	//              lcWord1.nLine, lcWord1.nCol, lcWord2.nLine, lcWord2.nCol);

			if (bFind && lcWord1.IsValidData() && lcWord1 != m_pSelection->GetNormalStart_str())
			{
				if (m_pSelection->IsSelNormal())
					m_pSelection->SetStart_str(lcWord1.nLine, lcWord1.nCol-1);
				else
					m_pSelection->SetEnd_str(lcWord1.nLine, lcWord1.nCol-1);
			}

			if (lcWord2.IsValidData() && lcWord2 != m_pSelection->GetNormalEnd_str())
			{
				if (m_pSelection->IsSelNormal())
					m_pSelection->SetEnd_str(lcWord2.nLine, lcWord2.nCol-1);
				else
					m_pSelection->SetStart_str(lcWord2.nLine, lcWord2.nCol-1);
			}
		}

		SetCurrentDocumentRow(nNewRow);
		m_nDispCol =  m_pSelection->GetEnd_disp().nCol;
		m_nCurrentCol = m_pSelection->GetEnd_str().nCol + 1;

		SetCurCaretPos(m_pSelection->GetEnd_disp().nLine, m_nDispCol, TRUE, FALSE);

		if (m_pSelection->bSelectingRunning)
			SetTimer(TIMER_SELECTION_ID, TIMER_SELECTION_TIME, NULL);

		Invalidate(FALSE);

		CWnd::OnLButtonDown(nFlags, point);
		NotifyMouseEvent(XTP_EDIT_NM_LBUTTONDOWN, nFlags, point);
	}
}

void CXTPSyntaxEditCtrl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	if (IsViewOnly())
		return;

	KillTimer(TIMER_SELECTION_ID);

	int nRow = ProcessCollapsedBlockDblClick(point);
	if (nRow > 0)
	{
		// close tooltip.
		ShowCollapsedToolTip(CPoint(0, 0));
	}
	else
	{
		// Select the word
		SelectWord(point);
	}

	CWnd::OnLButtonDblClk(nFlags, point);
	NotifyMouseEvent(XTP_EDIT_NM_LBUTTONDBLCLICK, nFlags, point);
}

void CXTPSyntaxEditCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (IsViewOnly())
		return;

	if (!GetCapture() || GetCapture() != this || GetFocus() != this)
		return;

	if (GetCapture() == this)
		ReleaseCapture();

	int nNewRow = 0, nNewCol = 0, nNewDispRow = 0, nNewDispCol = 0;
	BOOL bRowCol = RowColFromPoint(point, &nNewRow, &nNewCol, &nNewDispRow, &nNewDispCol);

	if (bRowCol && m_pSelection->IsInSel_disp(nNewRow, nNewDispCol) &&
		!m_pSelection->bSelectingRunning && !m_bDragging) // dragging
	{
		m_pSelection->Reset_disp(m_pSelection->GetStart_disp().nLine, m_pSelection->GetStart_disp().nCol);
	}
	else
	{
		SetCurCaretPos(GetCurrentDocumentRow(), m_nDispCol, FALSE, FALSE);
	}

	m_pSelection->bSelectingRunning = FALSE;
	KillTimer(TIMER_SELECTION_ID);

	m_dwAutoScrollDirection = 0;
	KillTimer(TIMER_AUTOSCROLL_ID);


	//-----------------------------------------------------------------------
	if (m_bDragging)
	{
		if (m_bDroppable)
		{
			BOOL bCtrlKey  = (::GetKeyState(VK_CONTROL) & KF_UP) != 0;
			CopyOrMoveText(bCtrlKey);
		}
		else
		{
			m_pSelection->Reset_disp(GetCurrentDocumentRow(), m_nDispCol);
		}
	}

	m_bDragging = FALSE;
	m_bDroppable = FALSE;

	SetCurCaretPos(GetCurrentDocumentRow(), m_nDispCol);

	Invalidate(FALSE);
	UpdateWindow();

	CWnd::OnLButtonUp(nFlags, point);
	NotifyMouseEvent(XTP_EDIT_NM_LBUTTONUP, nFlags, point);
}

void CXTPSyntaxEditCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	if (IsViewOnly())
		return;

	if (m_bDragging || m_bRightButtonDrag)
	{
		m_bDragging = TRUE;

		CXTPClientRect rcWnd(this);

		if (rcWnd.PtInRect(point))
		{
			int nNewRow = 0, nNewCol = 0, nNewDispCol = 0;
			RowColFromPoint(point, &nNewRow, &nNewCol, NULL, &nNewDispCol);
			//TRACE(_T("DropPOS: nNewRow=%d, nNewCol=%d, nNewDispCol=%d \n"), nNewRow, nNewCol, nNewDispCol);

			if (nNewRow != m_ptDropPos.y || nNewDispCol != m_ptDropPos.x)
			{
				m_nDispCol = nNewDispCol;
				m_ptDropPos.x = m_nDispCol;
				m_ptDropPos.y = nNewRow;

				//SetCurrentDocumentRow(nNewRow);
				SetCurCaretPos(m_ptDropPos.y, m_ptDropPos.x, FALSE, FALSE);
			}

			m_bDroppable = !m_pSelection->IsInSel_disp(m_ptDropPos.y, m_ptDropPos.x);

			OnSetCursor(0, 0, 0);
		}
		else
		{
			m_bDroppable = FALSE;
			SetCursor(AfxGetApp()->LoadStandardCursor(IDC_NO));
		}
	}
//  else if (m_pSelection->bSelectingRunning)
//  {
//      OnTimer(TIMER_SELECTION_ID);
//  }

	ShowCollapsedToolTip(point);

	CWnd::OnMouseMove(nFlags, point);
	NotifyMouseEvent(XTP_EDIT_NM_MOUSEMOVE, nFlags, point);
}

XTP_EDIT_COLLAPSEDBLOCK* CXTPSyntaxEditCtrl::GetBlockFromPt(const CPoint& ptMouse) const
{
	// enumerate all collapsed blocks coordinates
	// to find the one where mouse into
	XTP_EDIT_COLLAPSEDBLOCK* pCoDrawBlk = NULL;
	XTP_EDIT_LMPARAM LMCoParam;
	int nActualRow = 0;

	for (int i = 0; i < m_nCollapsedTextRowsCount; i++)
	{
		int nRow = m_arCollapsedTextRows[i];

		if (nRow <= nActualRow)
		{
			continue;
		}

		if (!HasRowMark(nRow, xtpEditLMT_Collapsed, &LMCoParam))
		{
			continue;
		}

		// get count of collapsed rows under this row
		int nHiddenRows = 0;
		if (!GetCollapsedBlockLen(nRow, nHiddenRows))
		{
			continue;
		}

		nActualRow = nRow + nHiddenRows;

		// get collapsed block pointer
		pCoDrawBlk = (XTP_EDIT_COLLAPSEDBLOCK*)LMCoParam.GetPtr();

		if (!pCoDrawBlk)
		{
			ASSERT(FALSE);
			continue;
		}

		CRect crCollapsed(pCoDrawBlk->rcCollMark);

		if (crCollapsed.PtInRect(ptMouse))
		{
			return pCoDrawBlk;
		}
	}
	return NULL;
}

int CXTPSyntaxEditCtrl::ProcessCollapsedBlockDblClick(const CPoint& ptMouse)
{
	// Find required collapsed block by coordinates
	XTP_EDIT_COLLAPSEDBLOCK* pCoDrawBlk = GetBlockFromPt(ptMouse);
	if (pCoDrawBlk)
	{
		int nRow = pCoDrawBlk->collBlock.lcStart.nLine;
		CollapseExpandBlock(nRow);
		return nRow;
	}
	return 0;
}

AFX_STATIC CString AFX_CDECL TrimLeftIndent(const CString& csText)
{
	CString csTrimmedText(csText);
	int nLastLine = csTrimmedText.ReverseFind('\n');
	CString csTrimText(csTrimmedText.Right(csTrimmedText.GetLength() - nLastLine).SpanIncluding(_T("\n \t")));
	csTrimmedText.Replace(csTrimText, _T("\n"));
	return csTrimmedText;
}

void CXTPSyntaxEditCtrl::ShowCollapsedToolTip(const CPoint& ptMouse)
{
	if (ptMouse == m_ptPrevMouse)
	{
		return;
	}

	// Find required collapsed block by coordinates
	XTP_EDIT_COLLAPSEDBLOCK* pCoDrawBlk = GetBlockFromPt(ptMouse);
	if (pCoDrawBlk)
	{
			m_ptPrevMouse = ptMouse;

			// set tooltip rect
			CRect rcTip(ptMouse, ptMouse);
			rcTip.right += 200;
			rcTip.bottom += 200;
			rcTip +=  CSize(18, 18);
			ClientToScreen(&rcTip);
			m_pToolTip->SetHoverRect(rcTip);

			// set tooltip text
			CString strToolText(GetCollapsedText(pCoDrawBlk));
			m_pToolTip->Activate(TrimLeftIndent(strToolText));
	}

	// Then Hide Tip if it is displayed for some other block
	if (ptMouse != m_ptPrevMouse)
	{
		m_pToolTip->Hide();
	}
}

void CXTPSyntaxEditCtrl::OnRButtonDown(UINT nFlags, CPoint point)
{
	m_bDragging = FALSE;
	m_nAutoIndentCol = 0;

	KillTimer(TIMER_SELECTION_ID);

	int iRow, iCol;
	RowColFromPoint(point, &iRow, &iCol);

//  if (!m_pSelection->IsSelExist() || !m_pSelection->IsInSel_str(iRow, iCol))
	if (!m_pSelection->IsSelExist()
		|| !m_pSelection->IsInSel_str(iRow, iCol-1))
// iCol-1: otherwise selection was cleared with click on the last character
// but retained when clicking one character in front of the selection!
	{
		LPCTSTR szLineText = GetLineText(iRow);

		//SetCurrentDocumentRow(iRow);

		m_nDispCol = CalcDispCol(szLineText, iCol);
		m_nCurrentCol = CalcAbsCol(szLineText, iCol);

		SetCurCaretPos(iRow, m_nDispCol);
		//Unselect();
		m_pSelection->Reset_disp(iRow, m_nDispCol);

		NotifySelInit();
	}


	SetCapture();

	if (m_pSelection->IsSelExist() && m_pSelection->IsInSel_str(iRow, iCol))
	{
		m_bRightButtonDrag = TRUE;

		if (m_bEnableOleDrag)
		{
			NotifyParent(XTP_EDIT_NM_STARTOLEDRAG);
		}
	}

//  if (!m_pSelection->IsSelExist() && !m_bEnableOleDrag)
//  {
//      RowColFromPoint(point, &iRow, &iCol);

//      // SetCurPos will determine valid column
//      SetCurPos(iRow, iCol);
//  }

	CWnd::OnRButtonDown(nFlags, point);
	NotifyMouseEvent(XTP_EDIT_NM_RBUTTONDOWN, nFlags, point);
}

void CXTPSyntaxEditCtrl::OnRButtonUp(UINT nFlags, CPoint point)
{
	CWnd::OnRButtonUp(nFlags, point);
	NotifyMouseEvent(XTP_EDIT_NM_RBUTTONUP, nFlags, point);
}

void CXTPSyntaxEditCtrl::Scroll(int x, int y)
{
	m_bScrolling = TRUE;

	if (y > 0)
		ShiftCurrentVisibleRowDown(y);
	else if (y < 0)
		ShiftCurrentVisibleRowUp(-y);

	if (x)
	{
		SCROLLINFO info;
		ZeroMemory(&info, sizeof(SCROLLINFO));
		info.cbSize = sizeof(SCROLLINFO);
		info.fMask = SIF_ALL;
		GetScrollInfo(SB_HORZ, &info);

		int nStep = m_pDrawTextProcessor->GetTextMetrics().tmAveCharWidth;

		int nCurrPos = m_pDrawTextProcessor->GetScrollXOffset();
		nCurrPos += x * nStep;
		nCurrPos = max(0, nCurrPos);

//      TRACE(_T("SCROLL.X. from %d -> %d \n"), (int)m_pDrawTextProcessor->GetScrollXOffset(), nCurrPos);

		SetScrollPos(SB_HORZ, nCurrPos);
		m_pDrawTextProcessor->SetScrollXOffset(nCurrPos);
	}

	//RecalcVertScrollPos();
	//RecalcHorzScrollPos();
	Invalidate(FALSE);
	UpdateWindow();

	RecalcScrollBars();

	int nTopDocRow = GetDocumentRow(1);
	int nBottomDocRow = GetDocumentRow(GetRowPerPage());
	int nCurDocRow = GetCurrentDocumentRow();

	if (nCurDocRow >= nTopDocRow && nCurDocRow <= nBottomDocRow)
	{
		SetCurCaretPos(nCurDocRow, m_nDispCol, FALSE, FALSE);
	}


	DWORD dwUpdate = 0;

	if (y != 0)
		dwUpdate |= XTP_EDIT_UPDATE_HORZ;

	if (x != 0)
		dwUpdate |= XTP_EDIT_UPDATE_VERT;


	if (dwUpdate != 0)
		UpdateScrollPos(dwUpdate);


	m_bScrolling = FALSE;
}

BOOL CXTPSyntaxEditCtrl::RowColFromPoint(CPoint pt, int *pRow, int *pCol,
									int *pDispRow, int *pDispCol, BOOL bVirtualSpace)
{
	if (bVirtualSpace < 0)
		bVirtualSpace = _IsVirtualSpaceActive();

	int nDispRow0 = 0, nDispCol0 = 0;

	if (!m_pDrawTextProcessor->HitTestRow(pt.y, nDispRow0))
	{
		nDispRow0 = pt.y < m_pDrawTextProcessor->GetTextRect().top ? 0 : m_pDrawTextProcessor->GetRowsCount(FALSE) - 1;
	}

	int nRow = GetDocumentRow(nDispRow0 + 1);

	if (nRow < 1 || nRow > GetRowCount())
		nRow = nRow < 1 ? 1 : GetRowCount();

	nDispRow0 = GetVisibleRow(nRow) - 1;

	UpdateRowInfoInternally(nRow); // to ensure than x positions are valid

	BOOL bRet = m_pDrawTextProcessor->ColFromXPos(nDispRow0, pt.x, nDispCol0, bVirtualSpace);
	if (!bRet)
	{
		if (pt.x < m_pDrawTextProcessor->GetTextRect().left)
		{
			nDispCol0 = 0;
		}
		else if (pt.x > m_pDrawTextProcessor->GetTextRect().right)
		{
			int nX1 = max(0, m_pDrawTextProcessor->GetTextRect().right - 1);
			m_pDrawTextProcessor->ColFromXPos(nDispRow0, nX1, nDispCol0, bVirtualSpace);
		}
	}
	int nCol0 = m_pDrawTextProcessor->DispPosToStrPos(nDispRow0, nDispCol0, bVirtualSpace);
	// to align to TABs positions
	nDispCol0 = m_pDrawTextProcessor->StrPosToDispPos(nDispRow0, nCol0, bVirtualSpace);

	if (pRow)
		*pRow = nRow;

	if (pCol)
		*pCol = nCol0 + 1;

	if (pDispRow)
		*pDispRow = nDispRow0 + 1;

	if (pDispCol)
		*pDispCol = nDispCol0 + 1;

	return bRet;
}

void CXTPSyntaxEditCtrl::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	if (m_bmpCache.m_hObject)
		m_bmpCache.DeleteObject();

	if (!GetPaintManager()->GetFont()->GetSafeHandle())
		return;

	CRect rcText;
	CalcEditRects(NULL, NULL, NULL, &rcText);

	m_pDrawTextProcessor->SetTextRect(rcText);

	RecalcVertScrollPos();

	Invalidate(FALSE);
	UpdateWindow();

	RecalcHorzScrollPos();
}

int CXTPSyntaxEditCtrl::GetScrollPos(int nBar) const
{
	CWnd* pParentWnd = DYNAMIC_DOWNCAST(CView, m_pParentWnd);

	if (pParentWnd != NULL && IsCreateScrollbarOnParent())
	{
		return pParentWnd->GetScrollPos(nBar);
	}

#ifdef _XTP_ACTIVEX

	if (m_pParentWnd)
		return m_pParentWnd->GetScrollPos(nBar);

#endif

	return CWnd::GetScrollPos(nBar);
}

void CXTPSyntaxEditCtrl::GetScrollRange(int nBar, LPINT lpMinPos, LPINT lpMaxPos) const
{
	CWnd* pParentWnd = DYNAMIC_DOWNCAST(CView, m_pParentWnd);

	if (pParentWnd != NULL && IsCreateScrollbarOnParent())
	{
		pParentWnd->GetScrollRange(nBar, lpMinPos, lpMaxPos);
	}
	else
	{
#ifdef _XTP_ACTIVEX

		if (m_pParentWnd && IsCreateScrollbarOnParent())
		{
			m_pParentWnd->GetScrollRange(nBar, lpMinPos, lpMaxPos);

			return;
		}

#endif

		CWnd::GetScrollRange(nBar, lpMinPos, lpMaxPos);
	}
}

void CXTPSyntaxEditCtrl::ScrollWindow(int xAmount, int yAmount, LPCRECT lpRect, LPCRECT lpClipRect)
{
	CWnd* pParentWnd = DYNAMIC_DOWNCAST(CView, m_pParentWnd);

	if (pParentWnd != NULL && IsCreateScrollbarOnParent())
	{
		pParentWnd->ScrollWindow(xAmount, yAmount, lpRect, lpClipRect);
	}
	else
	{
#ifdef _XTP_ACTIVEX

		if (m_pParentWnd && IsCreateScrollbarOnParent())
		{
			m_pParentWnd->ScrollWindow(xAmount, yAmount, lpRect, lpClipRect);

			return;
		}

#endif

		CWnd::ScrollWindow(xAmount, yAmount, lpRect, lpClipRect);
	}
}

int CXTPSyntaxEditCtrl::ScrollWindowEx(int dx, int dy, LPCRECT lpRectScroll, LPCRECT lpRectClip, CRgn *prgnUpdate, LPRECT lpRectUpdate, UINT flags)
{
	CWnd* pParentWnd = DYNAMIC_DOWNCAST(CView, m_pParentWnd);

	if (pParentWnd != NULL && IsCreateScrollbarOnParent())
	{
		return pParentWnd->ScrollWindowEx(dx, dy, lpRectScroll, lpRectClip, prgnUpdate, lpRectUpdate, flags);
	}

#ifdef _XTP_ACTIVEX

	if (m_pParentWnd && IsCreateScrollbarOnParent())
		return m_pParentWnd->ScrollWindowEx(dx, dy, lpRectScroll, lpRectClip, prgnUpdate, lpRectUpdate, flags);

#endif

	return CWnd::ScrollWindowEx(dx, dy, lpRectScroll, lpRectClip, prgnUpdate, lpRectUpdate, flags);
}

BOOL CXTPSyntaxEditCtrl::GetScrollInfo(int nBar, LPSCROLLINFO lpScrollInfo, UINT nMask)
{
	CWnd* pParentWnd = DYNAMIC_DOWNCAST(CView, m_pParentWnd);

	if (pParentWnd != NULL && IsCreateScrollbarOnParent())
	{
		return pParentWnd->GetScrollInfo(nBar, lpScrollInfo, nMask);
	}

#ifdef _XTP_ACTIVEX

	if (m_pParentWnd && IsCreateScrollbarOnParent())
		return m_pParentWnd->GetScrollInfo(nBar, lpScrollInfo, nMask);

#endif

	return CWnd::GetScrollInfo(nBar, lpScrollInfo, nMask);
}

int CXTPSyntaxEditCtrl::GetScrollLimit(int nBar)
{
	CWnd* pParentWnd = DYNAMIC_DOWNCAST(CView, m_pParentWnd);

	if (pParentWnd != NULL && IsCreateScrollbarOnParent())
	{
		return pParentWnd->GetScrollLimit(nBar);
	}

#ifdef _XTP_ACTIVEX

	if (m_pParentWnd && IsCreateScrollbarOnParent())
		return m_pParentWnd->GetScrollLimit(nBar);

#endif

	return CWnd::GetScrollLimit(nBar);
}

BOOL CXTPSyntaxEditCtrl::SetScrollInfo(int nBar, LPSCROLLINFO lpScrollInfo, BOOL bRedraw)
{
	CWnd* pParentWnd = DYNAMIC_DOWNCAST(CView, m_pParentWnd);

	if (pParentWnd != NULL && IsCreateScrollbarOnParent())
	{
		return pParentWnd->SetScrollInfo(nBar, lpScrollInfo, bRedraw);
	}

#ifdef _XTP_ACTIVEX

	if (m_pParentWnd && IsCreateScrollbarOnParent())
		return m_pParentWnd->SetScrollInfo(nBar, lpScrollInfo, bRedraw);

#endif

	return CWnd::SetScrollInfo(nBar, lpScrollInfo, bRedraw);
}

int CXTPSyntaxEditCtrl::SetScrollPos(int nBar, int nPos, BOOL bRedraw)
{
	CWnd* pParentWnd = DYNAMIC_DOWNCAST(CView, m_pParentWnd);

	if (pParentWnd != NULL && IsCreateScrollbarOnParent())
	{
		return pParentWnd->SetScrollPos(nBar, nPos, bRedraw);
	}

#ifdef _XTP_ACTIVEX

	if (m_pParentWnd && IsCreateScrollbarOnParent())
		return m_pParentWnd->SetScrollPos(nBar, nPos, bRedraw);

#endif

	return CWnd::SetScrollPos(nBar, nPos, bRedraw);
}

void CXTPSyntaxEditCtrl::SetScrollRange(int nBar, int nMinPos, int nMaxPos, BOOL bRedraw)
{
	CWnd* pParentWnd = DYNAMIC_DOWNCAST(CView, m_pParentWnd);

	if (pParentWnd != NULL && IsCreateScrollbarOnParent())
	{
		pParentWnd->SetScrollRange(nBar, nMinPos, nMaxPos, bRedraw);
	}
	else
	{
#ifdef _XTP_ACTIVEX

		if (m_pParentWnd && IsCreateScrollbarOnParent())
		{
			m_pParentWnd->SetScrollRange(nBar, nMinPos, nMaxPos, bRedraw);

			return;
		}

#endif

		CWnd::SetScrollRange(nBar, nMinPos, nMaxPos, bRedraw);
	}
}

void CXTPSyntaxEditCtrl::ShowScrollBar(UINT nBar, BOOL bShow)
{
	CWnd* pParentWnd = DYNAMIC_DOWNCAST(CView, m_pParentWnd);

	if (pParentWnd != NULL && IsCreateScrollbarOnParent())
	{
		pParentWnd->ShowScrollBar(nBar, bShow);
	}
	else
	{
#ifdef _XTP_ACTIVEX

		if (m_pParentWnd && IsCreateScrollbarOnParent())
		{
			m_pParentWnd->ShowScrollBar(nBar, bShow);

			return;
		}

#endif

		CWnd::ShowScrollBar(nBar, bShow);
	}
}

void CXTPSyntaxEditCtrl::EnableScrollBarCtrl(int nBar, BOOL bEnable)
{
	CWnd* pParentWnd = DYNAMIC_DOWNCAST(CView, m_pParentWnd);

	if (pParentWnd != NULL && IsCreateScrollbarOnParent())
	{
		pParentWnd->EnableScrollBarCtrl(nBar, bEnable);

		DWORD dwScrollBar = (nBar == SB_HORZ) ? WS_HSCROLL : 0;
		dwScrollBar |= (nBar == SB_VERT) ? WS_VSCROLL : 0;
		dwScrollBar |= (nBar == SB_BOTH) ? (WS_HSCROLL | WS_VSCROLL) : 0;

		_EnableScrollBarNotify((DWORD)dwScrollBar, bEnable ? (DWORD)dwScrollBar : 0);
	}
	else
	{
#ifdef _XTP_ACTIVEX

		if (m_pParentWnd && IsCreateScrollbarOnParent())
		{
			m_pParentWnd->EnableScrollBarCtrl(nBar, bEnable);

			return;
		}

#endif

		CWnd::EnableScrollBarCtrl(nBar, bEnable);
	}
}

CScrollBar* CXTPSyntaxEditCtrl::GetScrollBarCtrl(int nBar) const
{
	CWnd* pParentWnd = DYNAMIC_DOWNCAST(CView, m_pParentWnd);

	if (pParentWnd != NULL && IsCreateScrollbarOnParent())
	{
		return pParentWnd->GetScrollBarCtrl(nBar);
	}

#ifdef _XTP_ACTIVEX

	if (m_pParentWnd && IsCreateScrollbarOnParent())
		return m_pParentWnd->GetScrollBarCtrl(nBar);

#endif

	return CWnd::GetScrollBarCtrl(nBar);
}

void CXTPSyntaxEditCtrl::RepositionBars(UINT nIDFirst, UINT nIDLast, UINT nIDLeftOver, UINT nFlag, LPRECT lpRectParam, LPCRECT lpRectClient, BOOL bStretch)
{
	CWnd* pParentWnd = DYNAMIC_DOWNCAST(CView, m_pParentWnd);

	if (pParentWnd != NULL && IsCreateScrollbarOnParent())
	{
		pParentWnd->RepositionBars(nIDFirst, nIDLast, nIDLeftOver, nFlag, lpRectParam, lpRectClient, bStretch);
	}
	else
	{
#ifdef _XTP_ACTIVEX

		if (m_pParentWnd && IsCreateScrollbarOnParent())
		{
			m_pParentWnd->RepositionBars(nIDFirst, nIDLast, nIDLeftOver, nFlag, lpRectParam, lpRectClient, bStretch);

			return;
		}

#endif

		CWnd::RepositionBars(nIDFirst, nIDLast, nIDLeftOver, nFlag, lpRectParam, lpRectClient, bStretch);
	}
}

BOOL CXTPSyntaxEditCtrl::DeleteSelection()
{
	if (!m_pSelection->IsSelExist())
		return TRUE;

	int nFlags = (m_pSelection->bBlockSelectionMode ? xtpEditTextAsBlock : 0) | xtpEditDispCol;

	if (!DeleteBuffer(m_pSelection->GetNormalStart_disp().nLine, m_pSelection->GetNormalStart_disp().nCol,
			m_pSelection->GetNormalEnd_disp().nLine, m_pSelection->GetNormalEnd_disp().nCol,
			nFlags))
	{
		return FALSE;
	}

	m_pSelection->Reset_disp(m_pSelection->GetNormalStart_disp().nLine, m_pSelection->GetNormalStart_disp().nCol);

	SetCurrentDocumentRow(m_pSelection->GetNormalStart_disp().nLine);
	EnsureVisibleRow(GetCurrentDocumentRow());

	SetCurCaretPos(GetCurrentDocumentRow(), m_pSelection->GetStart_disp().nCol);

	Invalidate(FALSE);

	return TRUE;
}

BOOL CXTPSyntaxEditCtrl::DeleteBuffer(int iRowFrom, int iColFrom, int iRowTo, int iColTo,
		int nFlags )
{
	int iTempRow1 = iRowFrom, iTempRow2 = iRowTo;
	int iTempCol1 = iColFrom, iTempCol2 = iColTo;

	if (iTempRow2 < iTempRow1)
	{
		iRowFrom = iTempRow2;
		iColFrom = iTempCol2;
		iRowTo = iTempRow1;
		iColTo = iTempCol1;
	}
	else if (iTempRow1 == iTempRow2)
	{
		iColFrom = min(iTempCol1, iTempCol2);
		iColTo = max(iTempCol1, iTempCol2);
	}

	if ((nFlags & xtpEditTextAsBlock) &&
		(iColTo < iColFrom))
	{
		// swap them
		int nTemp = iColTo;
		iColTo = iColFrom;
		iColFrom = nTemp;
	}

	if (!CanEditDoc())
	{
		return FALSE;
	}
	BOOL bDispCol = (nFlags & xtpEditDispCol) != 0;
	int nColFrom_str = bDispCol ? CalcAbsCol(iRowFrom, iColFrom) : iColFrom;
	int nColTo_str = bDispCol ? CalcAbsCol(iRowTo, iColTo) : iColTo;

	//**----------------------
	OnBeforeEditChanged(iRowFrom, nColFrom_str);
	//**----------------------

	ASSERT((nFlags & xtpEditTextAsBlock) && bDispCol || !(nFlags & xtpEditTextAsBlock));

	if ((nFlags & xtpEditTextAsBlock) && iRowFrom != iRowTo)
	{
		m_pBuffer->GetUndoRedoManager()->SetGroupInsertMode(TRUE);

		for (int i = iRowFrom; i <= iRowTo; i++)
		{
			m_pBuffer->DeleteText(i, iColFrom, i, iColTo, TRUE, bDispCol);
		}
		m_pBuffer->GetUndoRedoManager()->SetGroupInsertMode(FALSE);

		m_pBuffer->GetUndoRedoManager()->SetLastCommandText(XTP_IDS_EDIT_DELETE_TEXT_BLOCK);
	}
	else
	{
		m_pBuffer->GetUndoRedoManager()->SetGroupInsertMode(FALSE);
		m_pBuffer->DeleteText(iRowFrom, iColFrom, iRowTo, iColTo, TRUE, bDispCol);
	}

	LPCTSTR szLineText = GetLineText(iRowFrom);
	SetCurrentDocumentRow(iRowFrom);
	m_nCurrentCol = nColFrom_str;
	m_nDispCol = CalcDispCol(szLineText, m_nCurrentCol);

	//**----------------------
	OnEditChanged(iRowFrom, nColFrom_str, iRowTo, nColTo_str, xtpEditActDelete);
	//**----------------------

	UINT uFlags = XTP_EDIT_EDITACTION_DELETEROW | XTP_EDIT_EDITACTION_MODIFYROW;

	NotifyEditChanged(iRowFrom, iRowTo, uFlags);

	SetDocModified();

	RecalcScrollBars();
	SetCurCaretPos(GetCurrentDocumentRow(), m_nDispCol);

	if (nFlags & xtpEditRedraw)
	{
		Invalidate(FALSE);
		UpdateWindow();
	}

	return TRUE;
}

BOOL CXTPSyntaxEditCtrl::DeleteChar(int iRow, int iCol, XTPSyntaxEditDeletePos pos)
{
	CString strText;

	int iRowTo = iRow, iColTo = iCol;

	if (pos == xtpEditDelPosAfter) // If deleting using DEL key
	{
		GetLineText(iRow, strText);

		int nLineLen = (int)_tcsclen(strText);

		if (iCol > nLineLen)
		{
			iRowTo = iRow + 1;
			iColTo = 1;

			if (iRowTo > GetRowCount())
				return FALSE;

			int nColFrom = nLineLen + 1;
			int nAdditionalTabs = 0;
			int nDispCol2 = CalcDispCol(strText, iCol);
			int nDispCol0  = CalcDispCol(strText, nColFrom);

			int nDC = nDispCol0 + GetTabSize() - (nDispCol0 -1) % GetTabSize();
			if (nDC <= nDispCol2)
				nAdditionalTabs++;
			else
				nDC = nDispCol0;

			for (; nDC + GetTabSize() <= nDispCol2; nDC += GetTabSize())
				nAdditionalTabs++;

			int nAdditionalSpaces = max(0, nDispCol2 - nDC);

			CString strTabs = CString(_T('\t'), nAdditionalTabs) +  CString(_T(' '), nAdditionalSpaces);

			//**----------------------
			OnBeforeEditChanged(iRow, nColFrom);

			XTP_EDIT_LINECOL lcFinal;
			m_pBuffer->InsertText(strTabs, iRow, nColFrom, TRUE, &lcFinal);

			m_pBuffer->GetUndoRedoManager()->SetLastCommandText(XTP_IDS_EDIT_FORMAT);

			OnEditChanged(iRow, nColFrom, iRow, lcFinal.nCol, xtpEditActInsert);
			//**----------------------

			iCol = lcFinal.nCol;
		}
		else
			iColTo++;
	}
	else // If deleting using back space
	{
		if (iCol == 1)
		{
			if (iRow > 1)
			{
				iRow--;
				// if we are in the end of collapsed block, expand it...
				CXTPSyntaxEditRowsBlockArray arCoBlocks;
				GetCollapsableBlocksInfo(iRow, arCoBlocks);
				int nCount = (int)arCoBlocks.GetSize();
				for (int i = 0; i < nCount; i++)
				{
					XTP_EDIT_ROWSBLOCK coBlk = arCoBlocks[i];
					if (coBlk.lcEnd.nLine == iRow) // Expand!
					{
						if (HasRowMark(coBlk.lcStart.nLine, xtpEditLMT_Collapsed))
							GetLineMarksManager()->DeleteLineMark(coBlk.lcStart.nLine, xtpEditLMT_Collapsed);
					}
				}

				//GetLineText(iRow, strText);
				//iCol = (int)_tcsclen(strText) + 1;
				iCol = m_pBuffer->GetLineTextLengthC(iRow) + 1;
			}
			else
				return FALSE;
		}
		else
			iCol--;
	}

	if (!DeleteBuffer(iRow, iCol, iRowTo, iColTo))
		return FALSE;

	return TRUE;
}

BOOL CXTPSyntaxEditCtrl::DoUndoRedo(int nActionsCount, BOOL bUndoRedo)
{
	CWaitCursor wait;

	if (!CanEditDoc())
		return FALSE;

	CXTPSyntaxEditUndoRedoManager* pUndoMgr = m_pBuffer->GetUndoRedoManager();
	if (!pUndoMgr)
		return FALSE;

	m_pSelection->Reset_disp(m_pSelection->GetEnd_disp().nLine, m_pSelection->GetEnd_disp().nCol);

	m_nAutoIndentCol = 0;
	int nCurDocRow = GetCurrentDocumentRow();
	XTP_EDIT_LINECOL lcTotalFrom = XTP_EDIT_LINECOL::MAXPOS;
	XTP_EDIT_LINECOL lcTotalTo = XTP_EDIT_LINECOL::MINPOS;

	for (int nAction = 0; nAction < nActionsCount; nAction++)
	{
		XTP_EDIT_LINECOL lcFrom, lcTo;

		// perform undo
		int nEditAction = bUndoRedo ?
			pUndoMgr->DoUndo(lcFrom, lcTo, this) :
			pUndoMgr->DoRedo(lcFrom, lcTo, this);

		// perform some total calculations
		lcTotalFrom = min(lcTotalFrom, lcFrom);
		lcTotalTo = max(lcTotalTo, lcTo);

		// TODO: ???????????????????????????????????
		// send update notifications

		// Commented out to eliminate double call of OnEditChanged that
		// leads to improper calculation of collapsible blocks borders
//      if (nEditAction & XTP_EDIT_EDITACTION_INSERTTEXT)
//      {
//          OnEditChanged(lcFrom, lcTo, xtpEditActInsert);
//      }
//      else if (nEditAction & XTP_EDIT_EDITACTION_DELETETEXT)
//      {
//          OnEditChanged(lcFrom, lcTo, xtpEditActDelete);
//      }

		NotifyEditChanged(lcFrom.nLine, lcTo.nLine, nEditAction);
	}

	int nRowsAffected = lcTotalTo.nLine - lcTotalFrom.nLine;

	m_nDispCol = CalcDispCol(GetCurrentDocumentRow(), m_nCurrentCol);

	RecalcScrollBars();

	if (GetCurrentDocumentRow() > GetRowCount())
	{
		SetCurrentDocumentRow(GetRowCount());
		m_nCurrentCol = m_nDispCol = 1;
	}

	nCurDocRow = GetCurrentDocumentRow();

	if (nActionsCount > 1)
	{
		ValidateCol(nCurDocRow, m_nDispCol, m_nCurrentCol);
		SetCurCaretPos(nCurDocRow, m_nDispCol, FALSE/*, FALSE*/);
	}

	nCurDocRow = GetCurrentDocumentRow();
	if ((nCurDocRow > 0 && m_nCurrentCol > 0) ||
		!(nRowsAffected == 0 && nActionsCount == 1))
	{
		SetCurCaretPos(nCurDocRow, m_nDispCol);
	}

	Invalidate(FALSE);
	UpdateWindow();

	UpdateScrollPos();

	SetDocModified(m_pBuffer->IsModified());

	return TRUE;
}

BOOL CXTPSyntaxEditCtrl::Undo(int nActionsCount)
{
	if (CanUndo())
	{
		return DoUndoRedo(nActionsCount, TRUE);
	}
	return FALSE;
}

BOOL CXTPSyntaxEditCtrl::Redo(int nActionsCount)
{
	if (CanRedo())
	{
		return DoUndoRedo(nActionsCount, FALSE);
	}
	return FALSE;
}

BOOL CXTPSyntaxEditCtrl::CanUndo() const
{
	if (m_pBuffer && m_pBuffer->GetUndoRedoManager())
		return CanEditDoc() && m_pBuffer->GetUndoRedoManager()->CanUndo();
	return FALSE;
}

BOOL CXTPSyntaxEditCtrl::CanRedo() const
{
	if (m_pBuffer && m_pBuffer->GetUndoRedoManager())
		return CanEditDoc() && m_pBuffer->GetUndoRedoManager()->CanRedo();
	return FALSE;
}

void CXTPSyntaxEditCtrl::Cut()
{
	if (IsViewOnly())
		return;

	if (m_pSelection->IsSelExist())
	{
		Copy();

		DeleteSelection();

		UpdateWindow();
	}
}

void CXTPSyntaxEditCtrl::Copy()
{
	if (IsViewOnly())
		return;

	if (!m_pSelection->IsSelExist())
		return;

	CWaitCursor wait;

	if (!OpenClipboard())
	{
#ifdef _DEBUG
		LPVOID lpMsgBuf;
		::FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			::GetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
			(LPTSTR) &lpMsgBuf,
			0,
			NULL
		);
		// Process any inserts in lpMsgBuf.
		// ...
		// Display the string.
		AfxMessageBox((LPCTSTR)lpMsgBuf, MB_OK | MB_ICONINFORMATION );

		// Free the buffer.
		::LocalFree( lpMsgBuf );
#endif
		return;
	}

	// prepare clipboard
	::EmptyClipboard();

	// retrieve the text string from buffer
	CMemFile file(CalcAveDataSize(m_pSelection->GetNormalStart_str().nLine,
			m_pSelection->GetNormalEnd_str().nLine));

	BOOL bRes = m_pBuffer->GetBuffer(m_pSelection->GetNormalStart_disp(),
		m_pSelection->GetNormalEnd_disp(), file,
		m_pSelection->bBlockSelectionMode, TRUE);
	if (!bRes)
		return;

	// Copy the buffer to clipboard
	DWORD dwBytes = (DWORD)file.GetLength();
	BYTE *pBytes = file.Detach();

	HGLOBAL hMem = ::GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, dwBytes + sizeof(TCHAR));
	if (!hMem)
		return;
	LPVOID lpSource = (LPVOID)::GlobalLock(hMem);
	if (!lpSource)
		return;

	MEMCPY_S(lpSource, pBytes, dwBytes);
	::ZeroMemory(((BYTE *)lpSource + dwBytes), sizeof(TCHAR));
	free(pBytes);

	UINT uCodePage = m_pBuffer->GetCodePage();

#ifdef _UNICODE
	// Determine the byte requirement
	int nLen  = ::WideCharToMultiByte(uCodePage, 0, (LPWSTR)lpSource, -1, NULL, 0, NULL, NULL);

	HGLOBAL hMBCSMem = ::GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, (nLen + 2));    // Maximum number of bytes is dwBytes
	if (!hMBCSMem)
	{
		CloseClipboard();
		return;
	}
	LPSTR lpMBCSSource = (LPSTR)::GlobalLock(hMBCSMem);
	if (!lpMBCSSource)
		return;

	int nBytes = ::WideCharToMultiByte(uCodePage, 0, (LPWSTR)lpSource, -1, lpMBCSSource, nLen, NULL, NULL);

	ASSERT(nBytes <= (int)dwBytes);

	lpMBCSSource[nBytes] = _T('\0');

	::GlobalUnlock(hMem);
	::GlobalUnlock(hMBCSMem);

	// lpSource is Unicode text
	::SetClipboardData(CF_UNICODETEXT, hMem);
	::SetClipboardData(CF_TEXT, hMBCSMem);
	::SetClipboardData(CF_OEMTEXT, hMBCSMem);
#else
	int nUBytes = (dwBytes + 2) * sizeof(WCHAR);
	HGLOBAL hUnicodeMem = ::GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, nUBytes);
	if (!hUnicodeMem)
	{
		CloseClipboard();
		return;
	}
	LPWSTR lpWSource = (LPWSTR)::GlobalLock(hUnicodeMem);
	if (!lpWSource)
	{
		CloseClipboard();
		return;
	}

	int nLen = ::MultiByteToWideChar(uCodePage, 0, (LPCSTR)lpSource, -1, lpWSource, nUBytes);

	ASSERT(nLen <= (int)(nUBytes / sizeof(WCHAR)));

	lpWSource[nLen] = _T('\0');

	::GlobalUnlock(hMem);
	::GlobalUnlock(hUnicodeMem);

	// lpSource is MBCS text
	::SetClipboardData(CF_TEXT, hMem);
	::SetClipboardData(CF_OEMTEXT, hMem);
	::SetClipboardData(CF_UNICODETEXT, hUnicodeMem);
#endif

	if (m_pSelection->bBlockSelectionMode)
	{
		HGLOBAL hMem_BLK = ::GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, 1);
		if (!hMem_BLK)
		{
			CloseClipboard();
			return;
		}
		BYTE* pMem_BLK = (BYTE*)::GlobalLock(hMem_BLK);
		if (pMem_BLK)
		{
			*pMem_BLK = 1;
		}
		::GlobalUnlock(hMem_BLK);

		CLIPFORMAT uCF_MSDEVColumnSelect = (CLIPFORMAT)::RegisterClipboardFormat(XTP_EDIT_CFMSDEVCOLSEL);
		::SetClipboardData(uCF_MSDEVColumnSelect, hMem_BLK);
	}

	CloseClipboard();
}

void CXTPSyntaxEditCtrl::Paste()
{
	CWaitCursor wait;

	if (!OpenClipboard())
		return;

	BOOL bCBTextIsBlock = FALSE;
	CLIPFORMAT uCF_MSDEVColumnSelect = (CLIPFORMAT)::RegisterClipboardFormat(XTP_EDIT_CFMSDEVCOLSEL);
	HANDLE hMem_TextBlok = ::GetClipboardData(uCF_MSDEVColumnSelect);
	if (hMem_TextBlok)
	{
		int nSize = (int)::GlobalSize(hMem_TextBlok);
		BYTE* arColBlk = (BYTE*)::GlobalLock(hMem_TextBlok);
		if (arColBlk && nSize)
		{
			bCBTextIsBlock = TRUE;
			//TRACE(_T("MSDEVColumnSelect data [size=%d] = %d \n"), nSize, (int)arColBlk[0]);
		}
	}

#ifdef _UNICODE
	HANDLE hMem = ::GetClipboardData(CF_UNICODETEXT);
#else
	HANDLE hMem = ::GetClipboardData(CF_TEXT);
#endif

	if (!hMem)
	{
		return;
	}

	LPTSTR szData = (LPTSTR)::GlobalLock(hMem);
	BOOL bReplace = GetTabWithSpace();
	BOOL bOverwrite = m_pBuffer->GetOverwriteFlag();

	SetTabWithSpace(FALSE);
	m_pBuffer->SetOverwriteFlag(FALSE);

	if (bCBTextIsBlock)
	{
		InsertTextBlock(szData, GetCurrentDocumentRow(), m_nCurrentCol, m_pSelection->IsSelExist());
	}
	else
	{
		InsertString(szData, GetCurrentDocumentRow(), m_nCurrentCol, m_pSelection->IsSelExist());
	}

	SetTabWithSpace(bReplace);
	m_pBuffer->SetOverwriteFlag(bOverwrite);

	m_pBuffer->GetUndoRedoManager()->SetLastCommandText(XTP_IDS_EDIT_PASTE);

	::GlobalUnlock(hMem);

	CloseClipboard();

	SetCurCaretPos(GetCurrentDocumentRow(), m_nDispCol);

	SetDocModified();

	UpdateScrollPos(XTP_EDIT_UPDATE_HORZ|XTP_EDIT_UPDATE_VERT);

	UpdateWindow();
}

void CXTPSyntaxEditCtrl::SetRowText(int nRow, LPCTSTR pcszText, BOOL bCanUndo)
{
	if (!m_pBuffer)
		return;

	int nRowLen = m_pBuffer->GetLineTextLengthC(nRow, FALSE);
	if (nRowLen)
	{
		//**----------------------
		OnBeforeEditChanged(nRow, 1);
		//**----------------------

		if (bCanUndo)
			m_pBuffer->GetUndoRedoManager()->SetGroupInsertMode(FALSE);

		m_pBuffer->DeleteText(nRow, 1, nRow, nRowLen + 1, bCanUndo, FALSE);

		//**----------------------
		OnEditChanged(nRow, 1, nRow + 1, 1, xtpEditActDelete);
		//**----------------------
	}

	//**----------------------
	OnBeforeEditChanged(nRow, 1);
	//**----------------------

	if (bCanUndo)
		m_pBuffer->GetUndoRedoManager()->SetGroupInsertMode(FALSE);

	//XTP_EDIT_LINECOL finalLC;
	m_pBuffer->InsertText(pcszText, nRow, 1, bCanUndo);//finalLC);

	//**----------------------
	OnEditChanged(nRow, 1, nRow + 1, 1, xtpEditActInsert);
	//**----------------------

	NotifyEditChanged(nRow, nRow, XTP_EDIT_EDITACTION_MODIFYROW);
}

void CXTPSyntaxEditCtrl::InsertRow(int nRow, LPCTSTR pcszText, BOOL bCanUndo)
{
	if (!m_pBuffer)
		return;

	CString strLine = pcszText;
	strLine += m_pBuffer->GetCurCRLF();

	//**----------------------
	OnBeforeEditChanged(nRow, 1);
	//**----------------------

	if (bCanUndo)
		m_pBuffer->GetUndoRedoManager()->SetGroupInsertMode(FALSE);

	//XTP_EDIT_LINECOL finalLC;
	m_pBuffer->InsertText(pcszText, nRow, 1, bCanUndo);//finalLC);

	//**----------------------
	OnEditChanged(nRow, 1, nRow + 1, 1, xtpEditActInsert);
	//**----------------------

	NotifyEditChanged(nRow, nRow, XTP_EDIT_EDITACTION_INSERTROW);

}

void CXTPSyntaxEditCtrl::RemoveRow(int nRow, BOOL bCanUndo)
{
	if (!m_pBuffer)
		return;

	//**----------------------
	OnBeforeEditChanged(nRow, 1);
	//**----------------------

	if (bCanUndo)
		m_pBuffer->GetUndoRedoManager()->SetGroupInsertMode(FALSE);

	m_pBuffer->RemoveLine(nRow, bCanUndo);

	//**----------------------
	OnEditChanged(nRow, 1, nRow + 1, 1, xtpEditActDelete);
	//**----------------------

	NotifyEditChanged(nRow, nRow, XTP_EDIT_EDITACTION_DELETEROW);
}

void CXTPSyntaxEditCtrl::OnSetFocus(CWnd* pOldWnd)
{
	CXTPSmartPtrInternalT<CCmdTarget> ptrThisLock(this, TRUE);

//  TRACE (_T("CXTPSyntaxEditCtrl::OnSetFocus. this=%x, pOldWnd=%x \n"), this, pOldWnd);

	::DestroyCaret();

	m_bFocused = TRUE;

	Invalidate(FALSE);

	RestoreCursor();
	NotifyCurRowCol(GetCurrentDocumentRow(), m_nDispCol);

	if (m_bActivateOnFocus)
		SetActive(TRUE);

	UpdateWindow();

	CWnd::OnSetFocus(pOldWnd);

#ifdef XTP_SYNTAXEDIT_SITENOTIFY_ONFOCUS
	XTP_SYNTAXEDIT_SITENOTIFY_ONFOCUS(this, this, TRUE)
#endif
}

void CXTPSyntaxEditCtrl::OnKillFocus(CWnd* pNewWnd)
{
	CXTPSmartPtrInternalT<CCmdTarget> ptrThisLock(this, TRUE);

	CWnd::OnKillFocus(pNewWnd);

	::DestroyCaret();

	m_pAutoComplete->Hide();
	m_bFocused = FALSE;

	if (m_bActivateOnFocus)
		SetActive(FALSE);

	Invalidate(FALSE);

#ifdef XTP_SYNTAXEDIT_SITENOTIFY_ONFOCUS
	XTP_SYNTAXEDIT_SITENOTIFY_ONFOCUS(this, this, FALSE)
#endif
}

BOOL CXTPSyntaxEditCtrl::OnNcActivate(BOOL bActive)
{
	return CWnd::OnNcActivate(bActive);
}

int CXTPSyntaxEditCtrl::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
//  SetFocus();

	return CWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);
}

BOOL CXTPSyntaxEditCtrl::IsSelectionExist() const
{
	return m_pSelection->IsSelExist();
}


void CXTPSyntaxEditCtrl::SelectAll()
{
	int nLastRow = GetRowCount();
	LPCTSTR szLineText = GetLineText(nLastRow);
	const int nEndCol = (int)_tcsclen(szLineText) + 1;

	SetCurrentDocumentRow(nLastRow);
	m_nCurrentCol = nEndCol;
	m_nDispCol = CalcDispCol(szLineText, m_nCurrentCol);

	m_pSelection->Reset_disp(1, 1);
	m_pSelection->SetEnd_disp(nLastRow, m_nDispCol);

	m_nAutoIndentCol = 0;

	SetCurCaretPos(GetCurrentDocumentRow(), m_nDispCol);
}

void CXTPSyntaxEditCtrl::SetFontIndirect(LPLOGFONT pLogfont, BOOL bUpdateReg/*=FALSE*/)
{
	GetPaintManager()->CreateFontIndirect(pLogfont, bUpdateReg);

	CWindowDC dc(NULL);
	CXTPFontDC fontDC(&dc, GetPaintManager()->GetFont());

	m_pDrawTextProcessor->RecalcRowHeight(&dc, GetPaintManager()->GetFont());

	if (m_hWnd && ::IsWindow(m_hWnd))
	{
		//m_pToolTip->SetFont(GetPaintManager()->GetFontToolTip());

		CalculateEditbarLength();

		Invalidate(FALSE);

		_UpdateIMEStatus();
	}
}

void CXTPSyntaxEditCtrl::_UpdateIMEStatus()
{
	m_bIMEsupported = m_pImmWrapper->ImmIsIME();

	if (!m_bIMEsupported || !m_hWnd || !::IsWindow(m_hWnd))
		return;

	// IME Support

	XTP_HIMC hIMC = m_pImmWrapper->ImmGetContext(m_hWnd);
	if (hIMC)
	{
		LOGFONT lfFont;
		::ZeroMemory(&lfFont, sizeof(lfFont));

		BOOL bFont = GetPaintManager()->GetFont()->GetLogFont(&lfFont);
		ASSERT(bFont);
		if (bFont)
		{
			VERIFY(m_pImmWrapper->ImmSetCompositionFont(hIMC, &lfFont));
		}

		VERIFY(m_pImmWrapper->ImmReleaseContext(m_hWnd, hIMC));
	}
}

LRESULT CXTPSyntaxEditCtrl::OnGetFont(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	return (LRESULT)GetPaintManager()->GetFont()->GetSafeHandle();
}

LRESULT CXTPSyntaxEditCtrl::OnSetFont(WPARAM wParam, LPARAM lParam)
{
	HFONT hFont = (HFONT)wParam;

	LOGFONT lfFont;
	LOGFONT *pLF = NULL; // set the default font if hFont == NULL

	if (hFont)
	{
		::ZeroMemory(&lfFont, sizeof(LOGFONT));

		if (!::GetObject(hFont, sizeof(LOGFONT), &lfFont))
		{
			ASSERT(FALSE);
			return 0;
		}

		pLF = &lfFont;
	}

	SetFontIndirect(pLF, FALSE);

	if (LOWORD(lParam))
		UpdateWindow();

	//Default();
	return 0;
}

LRESULT CXTPSyntaxEditCtrl::OnInputLanguage(WPARAM, LPARAM)
{
	_UpdateIMEStatus();

	if (m_bIMEsupported)
		SetCurCaretPos(GetCurrentDocumentRow(), m_nDispCol, FALSE, FALSE);

	Default();

	return 1;
}

void CXTPSyntaxEditCtrl::SetDocModified(BOOL bModified)
{
	XTP_EDIT_NMHDR_DOCMODIFIED dm;

	// NMHDR codes
	dm.nmhdr.code = XTP_EDIT_NM_SETDOCMODIFIED;
	dm.nmhdr.hwndFrom = m_hWnd;
	dm.nmhdr.idFrom = GetDlgCtrlID();

	// modified flag
	dm.bModified = bModified;

	// Notify the parent window
	if (::IsWindow(m_pParentWnd->GetSafeHwnd()))
	{
		m_pParentWnd->SendMessage(
			WM_NOTIFY, (WPARAM)dm.nmhdr.idFrom, (LPARAM)&dm);
	}
}

void CXTPSyntaxEditCtrl::InsertString(LPCTSTR szText, int iRow, int iCol, BOOL bDeleteSelection)
{
	if (!CanEditDoc())
		return;

	if (bDeleteSelection)
		DeleteSelection();

	int nPrevRow = GetCurrentDocumentRow();

	if (nPrevRow != iRow)
		SetCurrentDocumentRow(iRow);
	m_nCurrentCol = iCol;

	// At first determine what type of CRLF it has
	const int nTextLen = (int)_tcsclen(szText);

	if (nTextLen == 0)
		return;

	int nRowFrom = GetCurrentDocumentRow();
	int nColFrom = m_nCurrentCol;

	//**----------------------
	OnBeforeEditChanged(nRowFrom, nColFrom);
	//**----------------------

	if (GetAutoIndent() && m_nAutoIndentCol > 0)
	{
		CString strInsertText(
			CString(_T('\t'), m_nInsertTabCount) +
			CString(_T(' '), m_nInsertSpaceCount));

		m_pBuffer->InsertText(strInsertText, GetCurrentDocumentRow(), m_nCurrentCol);

		m_nCurrentCol = m_nInsertTabCount + m_nInsertSpaceCount + 1;

		m_nAutoIndentCol = 0;
	}


	XTP_EDIT_LINECOL finalLC;
	BOOL bInsRes = m_pBuffer->InsertText(szText, GetCurrentDocumentRow(),
										m_nCurrentCol, TRUE, &finalLC);

	if (bDeleteSelection)
		m_pBuffer->GetUndoRedoManager()->ChainLastCommand();

	if (bInsRes)
	{
		SetCurrentDocumentRow(finalLC.nLine);
		m_nCurrentCol = finalLC.nCol;

		m_nDispCol = CalcDispCol(GetLineText(finalLC.nLine), m_nCurrentCol);
	}

	RecalcScrollBars();

	UINT nAction = XTP_EDIT_EDITACTION_MODIFYROW;

	int nCurDocRow = GetCurrentDocumentRow();
	if (nCurDocRow > nPrevRow)
		nAction |= XTP_EDIT_EDITACTION_INSERTROW;

	//**----------------------
	OnEditChanged(nRowFrom, nColFrom, nCurDocRow, m_nCurrentCol, xtpEditActInsert);
	//**----------------------

	NotifyEditChanged(nPrevRow, nCurDocRow, nAction);

	SetCurCaretPos(nCurDocRow, m_nDispCol, FALSE/*, FALSE*/);
}

void CXTPSyntaxEditCtrl::InsertTextBlock(LPCTSTR szText, int iRow, int iCol, BOOL bDeleteSelection)
{
	if (!CanEditDoc())
		return;

	int nPrevRow = GetCurrentDocumentRow();

	if (nPrevRow != iRow)
	{
		SetCurrentDocumentRow(iRow);
	}
	m_nCurrentCol = iCol;

	if (bDeleteSelection)
	{
		DeleteSelection();
	}

	// At first determine what type of CRLF it has
	const int nTextLen = (int)_tcsclen(szText);

	if (nTextLen == 0)
		return;

	int nRowFrom = GetCurrentDocumentRow();
	int nColFrom = m_nCurrentCol;

	//**----------------------
	OnBeforeEditChanged(nRowFrom, nColFrom);
	//**----------------------

	XTP_EDIT_LINECOL finalLC;
	BOOL bInsRes = m_pBuffer->InsertTextBlock(szText, GetCurrentDocumentRow(),
												iCol, TRUE, &finalLC);
	UNREFERENCED_PARAMETER(bInsRes);

	if (bDeleteSelection)
		m_pBuffer->GetUndoRedoManager()->ChainLastCommand();

	RecalcScrollBars();

	UINT nAction = XTP_EDIT_EDITACTION_MODIFYROW;

	int nCurDocRow = GetCurrentDocumentRow();
	if (nCurDocRow > nPrevRow)
		nAction |= XTP_EDIT_EDITACTION_INSERTROW;

	//**----------------------
	OnEditChanged(nRowFrom, nColFrom, nCurDocRow, m_nCurrentCol, xtpEditActInsert);
	//**----------------------

	NotifyEditChanged(nPrevRow, nCurDocRow, nAction);

	SetCurCaretPos(nCurDocRow, m_nDispCol, FALSE/*, FALSE*/);
}

void CXTPSyntaxEditCtrl::UpdateScrollPos(DWORD dwUpdate/* = XTP_EDIT_UPDATE_ALL*/)
{
	if (!::IsWindow(m_hWnd))
		return;

	XTP_EDIT_NMHDR_SETSCROLLPOS ssp;

	// NMHDR codes
	ssp.nmhdr.code = XTP_EDIT_NM_UPDATESCROLLPOS;
	ssp.nmhdr.hwndFrom = m_hWnd;
	ssp.nmhdr.idFrom = GetDlgCtrlID();

	// Update flag
	ssp.dwUpdate = dwUpdate;

	// Notify the parent window
	if (::IsWindow(m_pParentWnd->GetSafeHwnd()))
	{
		m_pParentWnd->SendMessage(
			WM_NOTIFY, (WPARAM)ssp.nmhdr.idFrom, (LPARAM)&ssp);
	}
}

void CXTPSyntaxEditCtrl::_EnableScrollBarNotify(DWORD dwScrollBar, DWORD dwState)
{
	ASSERT(dwScrollBar != 0 && (dwScrollBar & (WS_VSCROLL|WS_HSCROLL)) != 0);

	XTP_EDIT_NMHDR_ENABLESCROLLBAR nmEScroll;

	// NMHDR codes
	nmEScroll.nmhdr.code = XTP_EDIT_NM_ENABLESCROLLBAR;
	nmEScroll.nmhdr.hwndFrom = m_hWnd;
	nmEScroll.nmhdr.idFrom = GetDlgCtrlID();

	// Update flag
	nmEScroll.dwScrollBar = dwScrollBar;
	nmEScroll.dwState = dwState;

	// Notify the parent window
	if (::IsWindow(m_pParentWnd->GetSafeHwnd()))
	{
		m_pParentWnd->SendMessage(
			WM_NOTIFY, (WPARAM)nmEScroll.nmhdr.idFrom, (LPARAM)&nmEScroll);
	}
}

void CXTPSyntaxEditCtrl::SetOverwriteMode(BOOL bOverwriteMode)
{
	if (bOverwriteMode != m_pBuffer->GetOverwriteFlag() )
	{
		m_pBuffer->SetOverwriteFlag(bOverwriteMode);

		SetCurCaretPos(GetCurrentDocumentRow(), m_nDispCol, FALSE/*, FALSE*/);
	}
}

BOOL CXTPSyntaxEditCtrl::OnEraseBkgnd(CDC* pDC)
{
	UNREFERENCED_PARAMETER(pDC);
	return TRUE;
}

XTP_EDIT_LINECOL CXTPSyntaxEditCtrl::_SubtractSelSizeFromPos(XTP_EDIT_LINECOL lcDispPos)
{
	XTP_EDIT_LINECOL lcStart = m_pSelection->GetNormalStart_disp();
	XTP_EDIT_LINECOL lcEnd = m_pSelection->GetNormalEnd_disp();

	if (lcDispPos <= lcStart)
		return lcDispPos;

	if (lcDispPos < lcEnd && !m_pSelection->bBlockSelectionMode)
	{
		ASSERT(FALSE);
		lcDispPos = lcEnd;
	}

	if (m_pSelection->bBlockSelectionMode)
	{
		if (lcDispPos.nLine >= lcStart.nLine && lcDispPos.nLine <= lcEnd.nLine)
		{
			if (lcDispPos.nCol >= lcEnd.nCol)
				lcDispPos.nCol -= lcEnd.nCol - lcStart.nCol;
		}
	}
	else
	{
		if (lcEnd.nLine != lcStart.nLine)
			lcDispPos.nLine -= lcEnd.nLine - lcStart.nLine;

		if (lcEnd.nLine == lcDispPos.nLine)
		{
			if (lcEnd.nLine == lcStart.nLine)
			{
				ASSERT(lcDispPos.nCol >= lcEnd.nCol);
				lcDispPos.nCol -= lcEnd.nCol - lcStart.nCol;
			}
			else
			{
				lcDispPos.nCol -= lcEnd.nCol;
			}
		}
	}


	return lcDispPos;
}

void CXTPSyntaxEditCtrl::CopyOrMoveText(BOOL bCopy)
{
	if (!m_pSelection->IsSelExist() || m_pSelection->IsInSel_disp(m_ptDropPos.y, m_ptDropPos.x))
		return;

	CMemFile memfile(CalcAveDataSize(m_pSelection->GetNormalStart_str().nLine,
			m_pSelection->GetNormalEnd_str().nLine));

	BOOL bRes = m_pBuffer->GetBuffer(m_pSelection->GetNormalStart_disp(),
			m_pSelection->GetNormalEnd_disp(), memfile,
			m_pSelection->bBlockSelectionMode);
	if (!bRes)
		return;

	memfile.Write((const void *)_T("\0"), sizeof(TCHAR));
	BYTE *pBytes = memfile.Detach();

	CString strDropText = (LPTSTR)pBytes;
	free(pBytes);

	// First insert the text
	if (m_ptDropPos.y > GetRowCount())
		m_ptDropPos.y = GetRowCount();

	XTP_EDIT_LINECOL lcDropPos_disp = XTP_EDIT_LINECOL::MakeLineCol(m_ptDropPos.y, m_ptDropPos.x);
	BOOL bBlockSelectionMode = m_pSelection->bBlockSelectionMode;

	if (!bCopy)
	{
		if (!(lcDropPos_disp < m_pSelection->GetNormalStart_disp()))
		{
			lcDropPos_disp = _SubtractSelSizeFromPos(lcDropPos_disp);
		}
		DeleteSelection(); // reset current selection
	}

	CString strText;
	m_pBuffer->GetLineText(lcDropPos_disp.nLine, strText);
	int nAbsDropCol = CalcAbsCol(strText, lcDropPos_disp.nCol);

	//**----------------------
	OnBeforeEditChanged(lcDropPos_disp.nLine, nAbsDropCol);
	//**----------------------

	BOOL bReplace = GetTabWithSpace();
	SetTabWithSpace(FALSE);

	XTP_EDIT_LINECOL finalLC;
	BOOL bInsRes = FALSE;
	if (bBlockSelectionMode)
	{
		bInsRes = m_pBuffer->InsertTextBlock(strDropText, lcDropPos_disp.nLine, nAbsDropCol,
												TRUE, &finalLC);
	}
	else
	{
		bInsRes = m_pBuffer->InsertText(strDropText, lcDropPos_disp.nLine, nAbsDropCol,
										TRUE, &finalLC);
	}

	SetTabWithSpace(bReplace);

	if (!bCopy)
	{
		m_pBuffer->GetUndoRedoManager()->ChainLastCommand();
		m_pBuffer->GetUndoRedoManager()->SetLastCommandText(XTP_IDS_EDIT_MOVE);
	}
	else
	{
		// It's a drag copy
		m_pBuffer->GetUndoRedoManager()->SetLastCommandText(XTP_IDS_EDIT_COPY);
	}

	m_pSelection->Reset_str(lcDropPos_disp.nLine, nAbsDropCol - 1);
	m_pSelection->SetEnd_str(finalLC.nLine, finalLC.nCol - 1);
	m_pSelection->bBlockSelectionMode = bBlockSelectionMode;

	int nRowFrom = m_pSelection->GetNormalStart_str().nLine;
	int nRowTo = m_pSelection->GetNormalEnd_str().nLine;
	//**----------------------
	OnEditChanged(nRowFrom, m_pSelection->GetNormalStart_str().nCol + 1,
		nRowTo, m_pSelection->GetNormalEnd_str().nCol + 1, xtpEditActInsert);
	//**----------------------

	CalculateEditbarLength();

	SetCurCaretPos(m_pSelection->GetEnd_disp().nLine, m_pSelection->GetEnd_disp().nCol);

	SetDocModified();

	//**----------------------
//  OnEditChanged(nRowFrom, m_pSelection->GetNormalStart_str().nCol,
//                nRowTo, m_pSelection->GetNormalEnd_str().nCol, xtpEditActInsert);
	//**----------------------

	UINT nAction = (XTP_EDIT_EDITACTION_MODIFYROW | XTP_EDIT_EDITACTION_INSERTROW);
	NotifyEditChanged(nRowFrom, nRowTo, nAction);
}

void CXTPSyntaxEditCtrl::OnContextMenu(CWnd* pWnd, CPoint point)
{
	if (GetCapture() == this)
	{
		ReleaseCapture();
	}

	if (m_bRightButtonDrag && m_bDragging && m_bDroppable)
	{
		HandleDrop(FALSE);
	}
	else
	{
		CXTPClientRect rcWnd(this);
		ClientToScreen(&rcWnd);

		m_bDragging = FALSE;

		if (point.x < 0 || point.y < 0) //keyboard ContextMenu button pressed
		{
			::GetCaretPos(&point);
			ClientToScreen(&point);
			if (point.x < 0 || point.y < 0)
				point = rcWnd.CenterPoint();
		}
		if (m_pParentWnd && rcWnd.PtInRect(point))
		{
			CWnd::HideCaret();
			m_pParentWnd->SendMessage(WM_CONTEXTMENU, (WPARAM)pWnd->GetSafeHwnd(), (LPARAM)MAKELONG(point.x, point.y));
			CWnd::ShowCaret();

			Invalidate(FALSE);
			UpdateWindow();
		}
	}

	m_bRightButtonDrag = FALSE;
}

void CXTPSyntaxEditCtrl::OnDragCopy()
{
	CopyOrMoveText(TRUE);
	m_bRightButtonDrag = FALSE;
}

void CXTPSyntaxEditCtrl::OnDragMove()
{
	CopyOrMoveText(FALSE);
	m_bRightButtonDrag = FALSE;
}

void CXTPSyntaxEditCtrl::RestoreCursor()
{
	SetCurCaretPos(GetCurrentDocumentRow(), m_nDispCol, FALSE, FALSE);
}

void CXTPSyntaxEditCtrl::SetInternalRowBkColor(int nRow, COLORREF crBack)
{
	m_mapInternalRowBkColor[nRow] = crBack;
}

void CXTPSyntaxEditCtrl::SetInternalRowColor(int nRow, COLORREF clrFront)
{
	m_mapInternalRowForeColor[nRow] = clrFront;
}

void CXTPSyntaxEditCtrl::SetRowBkColor(int nRow, COLORREF crBack)
{
	CXTPSyntaxEditRowColorMap& mapRowBkColor = m_pOptions->m_mapRowBkColor;

	if (nRow == -1)
	{
		if (mapRowBkColor.GetCount() == 0)
			return;

		mapRowBkColor.RemoveAll();
	}
	else
	{
		if (crBack == RGB(255, 255, 255) || crBack == COLORREF_NULL)
			mapRowBkColor.RemoveKey(nRow);
		else
			mapRowBkColor[nRow] = crBack;
	}

	InvalidateAll();
}

void CXTPSyntaxEditCtrl::SetRowColor(int nRow, COLORREF clrFront)
{
	CXTPSyntaxEditRowColorMap& mapRowColor = m_pOptions->m_mapRowForeColor;

	if (nRow == -1)
	{
		if (mapRowColor.GetCount() == 0)
			return;

		mapRowColor.RemoveAll();
	}
	else
	{
		if (clrFront == RGB(0, 0, 0) || clrFront == COLORREF_NULL)
			mapRowColor.RemoveKey(nRow);
		else
			mapRowColor[nRow] = clrFront;
	}

	InvalidateAll();
}

COLORREF CXTPSyntaxEditCtrl::GetRowColor(int nRow) const
{
	COLORREF clrFront;

	if (m_mapInternalRowForeColor.Lookup(nRow, clrFront))
	{
		return clrFront;
	}

	if (m_pOptions->m_mapRowForeColor.Lookup(nRow, clrFront))
	{
		return clrFront;
	}

	return COLORREF_NULL;
}

COLORREF CXTPSyntaxEditCtrl::GetRowBkColor(int nRow) const
{
	COLORREF crBack;

	if (m_mapInternalRowBkColor.Lookup(nRow, crBack))
	{
		return crBack;
	}

	if (m_pOptions->m_mapRowBkColor.Lookup(nRow, crBack))
	{
		return crBack;
	}

	return COLORREF_NULL;
}


const CXTPSyntaxEditCtrl& CXTPSyntaxEditCtrl::operator=(const CXTPSyntaxEditCtrl& src)
{
	if (::IsWindow(src.m_hWnd))
	{
		SetCurrentDocumentRow(src.m_nCurrentDocumentRow);
		m_nCurrentCol = src.m_nCurrentCol;
		m_nDispCol    = src.m_nDispCol;
	}


	CalculateEditbarLength();


	CMDTARGET_RELEASE(m_pOptions);
	m_pOptions = src.m_pOptions;
	CMDTARGET_ADDREF(m_pOptions);


	RecalcScrollBars();

	SetScrollPos(SB_HORZ, 0);
	m_pDrawTextProcessor->SetScrollXOffset(0);

	SetPaintManager(src.GetPaintManager());
	CMDTARGET_ADDREF(m_pPaintManeger);


	return *this;
}

CWnd * CXTPSyntaxEditCtrl::GetEffectiveParent() const
{
	CWnd *pScrollWnd = GetParent();

	if (GetParent())
	{
		pScrollWnd = GetParent();

		if (!IsKindOf(RUNTIME_CLASS(CSplitterWnd)))
		{
			pScrollWnd = GetParent();
		}
	}
	else
	{
		pScrollWnd = const_cast<CXTPSyntaxEditCtrl *>(this);
	}

	return pScrollWnd;
}

DWORD CXTPSyntaxEditCtrl::GetAutoscrollDirection(int* pnCols, int* pnRows)
{
	CPoint ptCursor;
	GetCursorPos(&ptCursor);
	ScreenToClient(&ptCursor);

	CRect rcText = m_pDrawTextProcessor->GetTextRect();
	int nAutoZoneY = min(m_pDrawTextProcessor->GetRowHeight(), rcText.Height()/4);
	int nAutoZoneX2 = min(m_pDrawTextProcessor->GetTextMetrics().tmAveCharWidth, rcText.Width()/4);

	DWORD dwDirection = 0;

	int nScrollX = 0, nScrollY = 0;

	if (ptCursor.y < rcText.top + nAutoZoneY)
	{
		dwDirection += xtpTop;
		nScrollY = -1 - (rcText.top + nAutoZoneY - ptCursor.y) / (nAutoZoneY / 2);
	}
	if (ptCursor.y > rcText.bottom - nAutoZoneY)
	{
		dwDirection += xtpBottom;
		nScrollY = 1 + (ptCursor.y - rcText.bottom + nAutoZoneY) / (nAutoZoneY / 2);
	}
	if (ptCursor.x < rcText.left)
	{
		dwDirection += xtpLeft;
		nScrollX = -1 - (rcText.left - ptCursor.x) / (nAutoZoneX2 / 2);
	}
	if (ptCursor.x > rcText.right - nAutoZoneX2)
	{
		dwDirection += xtpRight;
		nScrollX = 1 + (ptCursor.x - rcText.right + nAutoZoneX2) / (nAutoZoneX2 / 2);
	}

	if (pnCols)
		*pnCols = nScrollX;

	if (pnRows)
		*pnRows = nScrollY;

	return dwDirection;
}

void CXTPSyntaxEditCtrl::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == TIMER_AUTOSCROLL_ID)
	{
		int nScrollX = 0, nScrollY = 0;
		m_dwAutoScrollDirection = GetAutoscrollDirection(&nScrollX, &nScrollY);

		if (nScrollX || nScrollY)
		{
			Scroll(nScrollX, nScrollY);

			OnTimer(TIMER_SELECTION_ID);
			//UpdateWindow();
		}

		BOOL bLButton = (::GetKeyState(VK_LBUTTON) & KF_UP) == KF_UP;

		if (!m_dwAutoScrollDirection || !bLButton)
		{
			m_dwAutoScrollDirection = 0;
			KillTimer(TIMER_AUTOSCROLL_ID);
		}
	}
	else if (nIDEvent == TIMER_SELECTION_ID)
	{
		if ((::GetKeyState(VK_LBUTTON) & KF_UP) == 0)
		{
			m_pSelection->bSelectingRunning = FALSE;

			KillTimer(TIMER_SELECTION_ID);
			return;
		}

		CPoint ptCursor;
		GetCursorPos(&ptCursor);
		ScreenToClient(&ptCursor);

		BOOL bIsAutoScrollTimer = !!m_dwAutoScrollDirection;

		m_dwAutoScrollDirection = GetAutoscrollDirection();
		if (m_dwAutoScrollDirection)
		{
			if (!bIsAutoScrollTimer)
				SetTimer(TIMER_AUTOSCROLL_ID, TIMER_AUTOSCROLL_TIME, NULL);
		}
		else
		{
			m_dwAutoScrollDirection = 0;
			KillTimer(TIMER_AUTOSCROLL_ID);
		}

		//-------------------------------------------------------------------
		CRect rcBookmark, rcLineNum, rcNodes, rcText;
		CalcEditRects(&rcBookmark, &rcLineNum, &rcNodes, &rcText);
		CRect rcLineNumAndNodes(rcLineNum);
		rcLineNumAndNodes.right = rcNodes.right;

		int nNewRow = 0, nNewCol = 0, nNewDispRow = 0, nNewDispCol = 0;
		BOOL bVirtualSpace = m_pSelection->IsbBlockSelectionMode() ? TRUE : -1;
		RowColFromPoint(ptCursor, &nNewRow, &nNewCol, &nNewDispRow, &nNewDispCol, bVirtualSpace);

		BOOL bSelFromLeftBar = rcLineNumAndNodes.PtInRect(ptCursor);

		if (bSelFromLeftBar && m_pSelection->nSelStartTextRowFromLeftBar)
		{
			if (nNewRow >= m_pSelection->nSelStartTextRowFromLeftBar)
			{
				nNewRow++;
				m_pSelection->SetStart_str(m_pSelection->nSelStartTextRowFromLeftBar, 0);
			}
			else
			{
				m_pSelection->SetStart_str(m_pSelection->nSelStartTextRowFromLeftBar+1, 0);
			}
		}

		int nEndRow_prev = m_pSelection->GetEnd_disp().nLine;
		int nEndDispCol_prev = m_pSelection->GetEnd_disp().nCol;

		if (nEndRow_prev != nNewRow || nEndDispCol_prev != nNewDispCol)
		{
//          TRACE(_T("OnTimer.sel. RowColFromPoint: nNewRow=%d, nNewCol=%d, nNewDispRow=%d, nNewDispCol=%d \n"),
//                  nNewRow, nNewCol, nNewDispRow, nNewDispCol);

			m_pSelection->SetEnd_disp(nNewRow, nNewDispCol);

			if (m_pSelection->bWordSelectionMode)
			{
				XTP_EDIT_LINECOL lcWord1, lcWord2;
				BOOL bOverSpace = FALSE;

				UINT nFindDir = m_pSelection->IsSelNormal() ? XTP_EDIT_FINDWORD_NEXT : XTP_EDIT_FINDWORD_PREV;
				BOOL bFind = FindWordEx_str(nFindDir, m_pSelection->GetEnd_str(), lcWord1, lcWord2, bOverSpace);
				//TRACE(_T("2-FindWordEx_str = %d (%d, %d - %d, %d)"), bFind,
				//      lcWord1.nLine, lcWord1.nCol, lcWord2.nLine, lcWord2.nCol);

				XTP_EDIT_LINECOL& lcWordEnd = m_pSelection->IsSelNormal() ? lcWord2 : lcWord1;
				lcWordEnd.nCol--;

				if (bFind && lcWordEnd.IsValidData() && lcWordEnd != m_pSelection->GetEnd_str())
				{
					m_pSelection->SetEnd_str(lcWordEnd.nLine, lcWordEnd.nCol);
				}
			}

			SetCurrentDocumentRow(nNewRow);
			m_nDispCol =  m_pSelection->GetEnd_disp().nCol;
			m_nCurrentCol = m_pSelection->GetEnd_str().nCol + 1;

			int nCaretLine = m_pSelection->GetEnd_disp().nLine;
			if (bSelFromLeftBar && m_pSelection->GetEnd_disp() >= m_pSelection->GetStart_disp())
				nCaretLine = max(1, nCaretLine - 1);

			SetCurCaretPos(nCaretLine, m_nDispCol, FALSE, FALSE);

			//TRACE(_T("Selection: (row=%d, strPos=%d, DispCol=%d)-(row=%d, strPos=%d, DispCol=%d) \n"),
			//      m_pSelection->GetStart_str().nLine, m_pSelection->GetStart_str().nCol, m_pSelection->GetStart_disp().nCol,
			//      m_pSelection->GetEnd_str().nLine, m_pSelection->GetEnd_str().nCol, m_pSelection->GetEnd_disp().nCol);

			Invalidate(FALSE);
			UpdateWindow();
		}
	}
	else if (nIDEvent == TIMER_REDRAW_WHEN_PARSE)
	{
		KillTimer(TIMER_REDRAW_WHEN_PARSE);

		Invalidate(FALSE);
		UpdateWindow();
	}
}

BOOL CXTPSyntaxEditCtrl::Select(int nRow1, int nDispCol1, int nRow2, int nDispCol2, BOOL bRedraw)
{
	if (nRow1 < 1 || nRow1 > GetRowCount())
		return FALSE;

	if (nRow2 < 1 || nRow2 > GetRowCount())
		return FALSE;

	m_pSelection->Reset_disp(nRow1, nDispCol1);
	m_pSelection->SetEnd_disp(nRow2, nDispCol2);

	SetCurrentDocumentRow(m_pSelection->GetNormalEnd_disp().nLine);
	m_nDispCol = m_pSelection->GetNormalEnd_disp().nCol;
	m_nCurrentCol = m_pSelection->GetNormalEnd_str().nCol + 1;

	if (bRedraw)
	{
		Invalidate(FALSE);
	}

	return TRUE;
}

void CXTPSyntaxEditCtrl::Unselect()
{
	if (m_pSelection->IsSelExist())
	{
		m_pSelection->Reset_disp(m_pSelection->GetEnd_disp().nLine,
							   m_pSelection->GetEnd_disp().nCol);
		Invalidate(FALSE);
	}
}

BOOL CXTPSyntaxEditCtrl::ReplaceSel(LPCTSTR szNewText)
{
	if (m_pSelection->IsSelExist())
	{
		SetCurrentDocumentRow(m_pSelection->GetNormalStart_disp().nLine);
		m_nDispCol = m_pSelection->GetNormalStart_disp().nCol;
		m_nCurrentCol = m_pSelection->GetNormalStart_str().nCol + 1; //CalcAbsCol(GetLineText(m_pSelection->GetNormalStart_disp().nLine), m_nDispCol);
	}

	BOOL bOverwriteMode = m_pBuffer->GetOverwriteFlag();
	m_pBuffer->SetOverwriteFlag(FALSE);

	InsertString(szNewText, GetCurrentDocumentRow(), m_nCurrentCol, TRUE);

	m_pBuffer->SetOverwriteFlag(bOverwriteMode);

	return TRUE;
}

int CXTPSyntaxEditCtrl::ReplaceAll(LPCTSTR szFindText, LPCTSTR szReplaceText,
							  BOOL bMatchWholeWord, BOOL bMatchCase)
{
	int nMatchFound = 0;

	if (_tcsclen(szFindText) == 0)
	{
		return 0;
	}

	int iTopRow = GetTopRow();
	int iCurRow = GetCurRow();
	int iCurCol = GetCurCol();

	SetCurPos(1, 1, FALSE, FALSE);

	CWaitCursor wait;

	BOOL bFirst = TRUE;

	while (GetCurRow() <= GetRowCount())
	{
		BOOL bFound = Find(szFindText,
			bMatchWholeWord, bMatchCase,
			TRUE, FALSE);

		if (bFound)
		{
			ReplaceSel(szReplaceText);
			if (!bFirst)
				GetEditBuffer()->GetUndoRedoManager()->ChainLastCommand();
		}
		else
			break;

		bFirst = FALSE;

		nMatchFound++;
	}

	if (!bFirst)
		GetEditBuffer()->GetUndoRedoManager()->SetLastCommandText(XTP_IDS_EDIT_REPLACE_ALL);

	SetTopRow(iTopRow);
	SetCurPos(iCurRow, iCurCol);

	return nMatchFound;
}


BOOL CXTPSyntaxEditCtrl::Find(LPCTSTR szText, BOOL bMatchWholeWord, BOOL bMatchCase, BOOL bSearchDown,
		BOOL bRedraw, int nStartRow, int nStartCol)
{
	// First check the current row
	CString strLineText;

	int iStartCol = m_nCurrentCol;
	int iStartRow = GetCurrentDocumentRow();

	int nTopRow = m_nTopRow;
	int nBottomRow = GetDocumentRow(GetRowPerPage());

	if (m_pSelection->IsSelExist())
	{
		if (bSearchDown)
		{
			iStartCol = m_pSelection->GetNormalEnd_str().nCol + 1;  // m_ptEndSel.x;
			iStartRow = m_pSelection->GetNormalEnd_str().nLine; // m_ptEndSel.y;
		}
		else
		{
			iStartCol = m_pSelection->GetNormalStart_str().nCol + 1;  // m_ptStartSel.x;
			iStartRow = m_pSelection->GetNormalStart_str().nLine; // m_ptStartSel.y;
		}
	}

	iStartCol = nStartCol < 0 ? iStartCol : nStartCol;
	iStartRow = nStartRow < 0 ? iStartRow : nStartRow;

	m_pBuffer->GetLineText(iStartRow, strLineText);

	BOOL bResult = MatchText(iStartRow, strLineText, szText, iStartCol, bMatchWholeWord, bMatchCase, bSearchDown, bRedraw);

	if (bResult)
		return TRUE;

	if (bSearchDown)
	{
		for (int i = (iStartRow + 1); i <= GetRowCount(); i++)
		{
			m_pBuffer->GetLineText(i, strLineText);

			bResult = MatchText(i, strLineText, szText, 0, bMatchWholeWord, bMatchCase, TRUE, bRedraw);

			if (bResult)
				break;
		}
	}
	else
	{
		for (int i = (iStartRow - 1); i >= 1; i--)
		{
			m_pBuffer->GetLineText(i, strLineText);

			bResult = MatchText(i, strLineText, szText, (int)_tcsclen(strLineText) + 1, bMatchWholeWord, bMatchCase, FALSE, bRedraw);

			if (bResult)
				break;
		}
	}

	if (bResult)
	{
		if (m_pSelection->GetStart_str().nLine > nBottomRow)
		{
			ShiftCurrentVisibleRowDown(2 * GetRowPerPage() / 3);
		}
		else if (m_pSelection->GetStart_str().nLine < nTopRow)
		{
			ShiftCurrentVisibleRowUp(GetRowPerPage() / 3);
		}

		SetCurCaretPos(m_pSelection->GetNormalEnd_disp().nLine, m_pSelection->GetNormalEnd_disp().nCol);
	}

	return bResult;
}

BOOL CXTPSyntaxEditCtrl::MatchText(int nRow, LPCTSTR szLineText, LPCTSTR szMatchText, int nStartPos, BOOL bMatchWholeWord, BOOL bMatchCase, BOOL bSearchForward, BOOL bRedraw)
{
	BOOL bMatched = FALSE;

#ifdef XTP_FIXED
	const int nLen = (int) _tcslen(szLineText);
	const int nSearchLen = (int) _tcslen(szMatchText);
#else
	const int nLen = (int) _tcsclen(szLineText);
	const int nSearchLen = (int) _tcsclen(szMatchText);
#endif

	static LPCTSTR szSeps = _T("() \t<>{}:;,.=%\"'!@#$^&*-\\|[]/?");
#ifdef XTP_FIXED
	int count = 0;
	int temp = 0;
	for (count = 0; count < nStartPos - 1; count ++)
	{
		if (isleadbyte( szLineText[temp] ))
			temp += 2;
		else
			temp++;
	}
	nStartPos = temp;
#endif

	if (bSearchForward)
	{
		for (int i = nStartPos; i <= (nLen - nSearchLen + 1); i++)
		{
			int nFound = -1;
			if (bMatchCase)
				nFound = _tcsncmp((szLineText + i - 1), szMatchText, nSearchLen);
			else
				nFound = _tcsnicmp((szLineText + i - 1), szMatchText, nSearchLen);

			bMatched = (nFound == 0);

			if (bMatchWholeWord && nFound == 0)
			{
				BOOL bSepAfter = FALSE;
				BOOL bSepBefore = FALSE;

				if ((i - 1 + nSearchLen) >= nLen)
				{
					bSepAfter = TRUE;
				}
				else
				{
					LPCTSTR pcszTextAfter = (szLineText + i - 1) + nSearchLen;
					bSepAfter = (_tcschr(szSeps, *pcszTextAfter) != 0);
				}

				if (i == 1)
					bSepBefore = TRUE;
				else
					bSepBefore = (_tcschr(szSeps, *(szLineText + i - 2)) != 0);

				if (!bSepAfter || !bSepBefore)
					bMatched = FALSE;
			}

			if (bMatched)
			{
#ifdef XTP_FIXED
				LPTSTR buffer = (LPTSTR)malloc( sizeof( TCHAR ) * ( nLen + 1 ) );
				memcpy( buffer, szLineText, sizeof( TCHAR ) * ( nLen + 1 ) );
				buffer[nLen] = '\0';
				buffer[i + nSearchLen - 1] = '\0';
				int nDispCol2 = _tcsclen( buffer );
				buffer[i - 1] = '\0';
				int nDispCol1 = _tcsclen( buffer );
				nDispCol1 = CalcDispCol(szLineText, nDispCol1 + 1 );
				nDispCol2 = CalcDispCol(szLineText, nDispCol2 + 1 );
				Select(nRow, nDispCol1, nRow, nDispCol2, bRedraw);
				free( buffer );
#else
				int nDispCol1 = CalcDispCol(szLineText, i);
				int nDispCol2 = CalcDispCol(szLineText, (i + nSearchLen));
				Select(nRow, nDispCol1, nRow, nDispCol2, bRedraw);
#endif
				m_nDispCol = nDispCol2;
				m_nCurrentCol = (i + nSearchLen);
				SetCurCaretPos(m_pSelection->GetNormalEnd_disp().nLine,
					m_pSelection->GetNormalEnd_disp().nCol, FALSE/*, FALSE*/);

				break;
			}
		}
	}
	else
	{
		for (int i = (nStartPos-nSearchLen); i >= 1 ; i--)
		{
			int nFound = -1;
			if (bMatchCase)
				nFound = _tcsncmp((szLineText + i - 1), szMatchText, nSearchLen);
			else
				nFound = _tcsnicmp((szLineText + i - 1), szMatchText, nSearchLen);

			bMatched = (nFound == 0);

			if (bMatchWholeWord && nFound == 0)
			{
				BOOL bSepAfter = FALSE;
				BOOL bSepBefore = FALSE;

				if ((i - 1 + nSearchLen) >= nLen)
				{
					bSepAfter = TRUE;
				}
				else
				{
					LPCTSTR pcszTextAfter = (szLineText + i - 1) + nSearchLen;
					bSepAfter = (_tcschr(szSeps, *pcszTextAfter) != 0);
				}

				if (i == 1)
					bSepBefore = TRUE;
				else
					bSepBefore = (_tcschr(szSeps, *(szLineText + i - 2)) != 0);

				if (!bSepAfter || !bSepBefore)
					bMatched = FALSE;
			}

			if (bMatched)
			{
				LPTSTR buffer = (LPTSTR)malloc( sizeof(TCHAR) * (nLen + 1) );
				memcpy( buffer, szLineText, sizeof(TCHAR) * (nLen + 1) );
				buffer[nLen] = '\0';
				buffer[i + nSearchLen - 1] = '\0';
				int nDispCol2 = (int) _tcsclen( buffer );
				buffer[i - 1] = '\0';
				int nDispCol1 = (int) _tcsclen( buffer );
				nDispCol1 = CalcDispCol(szLineText, nDispCol1 + 1 );
				nDispCol2 = CalcDispCol(szLineText, nDispCol2 + 1 );
				Select(nRow, nDispCol1, nRow, nDispCol2, bRedraw);
				free( buffer );

				m_nDispCol = nDispCol1;
				m_nCurrentCol = i;
				SetCurCaretPos(m_pSelection->GetNormalEnd_disp().nLine,
							   m_pSelection->GetNormalEnd_disp().nCol, FALSE/*, FALSE*/);

				break;
			}
		}
	}

	return bMatched;
}

void CXTPSyntaxEditCtrl::ShowHScrollBar(BOOL bShow)
{
	ShowScrollBar(SB_HORZ, bShow);
}

void CXTPSyntaxEditCtrl::SelectWord(CPoint point)
{
	int nRow = 0, nCol = 0;

	if (!RowColFromPoint(point, &nRow, &nCol))
		return;

	XTP_EDIT_LINECOL lcWord1, lcWord2;
	BOOL bOverSpace = FALSE;

	UINT nFindDir = XTP_EDIT_FINDWORD_NEXT;
	BOOL bFind = FindWordEx(nFindDir, nRow, nCol, lcWord1, lcWord2, bOverSpace);

	if (bFind && lcWord1.IsValidData() && lcWord2.IsValidData() && !bOverSpace)
	{
		m_pSelection->SetStart_str(lcWord1.nLine, lcWord1.nCol-1);
		m_pSelection->SetEnd_str(lcWord2.nLine, lcWord2.nCol-1);

		SetCurCaretPos(m_pSelection->GetEnd_disp().nLine,
					   m_pSelection->GetEnd_disp().nCol);

		Invalidate(FALSE);
		UpdateWindow();
	}
}

void CXTPSyntaxEditCtrl::FindWord(UINT nFindWhat)
{
	XTP_EDIT_LINECOL lcWord1, lcWord2;
	BOOL bOverSpace;

	BOOL bFind = FindWordEx(nFindWhat, GetCurrentDocumentRow(), m_nCurrentCol,
							lcWord1, lcWord2, bOverSpace);

	XTP_EDIT_LINECOL& lcWordEnd = (nFindWhat == XTP_EDIT_FINDWORD_NEXT) ? lcWord2 : lcWord1;


	if (!bFind || !lcWordEnd.IsValidData())
		return;

	int nDispCol = CalcDispCol(lcWordEnd.nLine, lcWordEnd.nCol);
	SetCurCaretPos(lcWordEnd.nLine, nDispCol);
}

BOOL CXTPSyntaxEditCtrl::FindWordEx_str(UINT nFindWhat, XTP_EDIT_LINECOL lcPos_str,
		XTP_EDIT_LINECOL& rlcWordStart, XTP_EDIT_LINECOL& rlcWordEnd, BOOL& rbOverSpace)
{
	return FindWordEx(nFindWhat, lcPos_str.nLine, lcPos_str.nCol + 1, rlcWordStart,
		rlcWordEnd, rbOverSpace);
}

BOOL CXTPSyntaxEditCtrl::FindWordEx(UINT nFindWhat, int nTextRow, int nCol,
									XTP_EDIT_LINECOL& rlcWordStart, XTP_EDIT_LINECOL& rlcWordEnd, BOOL& rbOverSpace)
{
	// set word invalid borders
	rlcWordStart = rlcWordEnd = XTP_EDIT_LINECOL::MINPOS;
	rbOverSpace = FALSE;

	CString strLineText;
	const TCHAR* pCh = NULL;

	// Volker Elies:    added '+' character
	const TCHAR szSeps[]    = _T("[]{}()<>.,;:!?=*&%@^#-+\"\'/");
//  const TCHAR szSepsAll[] = _T("[]{}()<>.,;:!?=*&%@^#-\"\'/\n\r\t ");
	const TCHAR szSpaceSeps[] = _T(" \t");
	const TCHAR szNLSeps[] = _T("\r\n");

	int nDir = nFindWhat == XTP_EDIT_FINDWORD_PREV ? -1 : 1;
	nCol = min(nCol-1, GetEditBuffer()->GetLineTextLengthC(nTextRow));
	XTP_EDIT_LINECOL lcPosStart = XTP_EDIT_LINECOL::MakeLineCol(nTextRow, max(0, nCol));

	CXTPSyntaxEditTextIterator txtIter(GetEditBuffer());
	BOOL bSeek = txtIter.SeekPos(lcPosStart);
	if (!bSeek)
		return FALSE;

	pCh = (nDir < 0) ? txtIter.SeekPrev() : txtIter.GetText(2);
	if (!pCh)
		return FALSE;

	// Is over space/tab
	BOOL bOverSpace = !!_tcschr(szSpaceSeps, *pCh);
	BOOL bOverNL = !!_tcschr(szNLSeps, *pCh);
	rbOverSpace = (bOverSpace || bOverNL);

	if (!pCh)
		return FALSE;

	BOOL bStartCharSep = !!_tcschr(szSeps, *pCh);

	if (rbOverSpace && nDir < 0)
	{
		while (pCh && (_tcschr(szSpaceSeps, *pCh) ||
					   !bOverSpace && _tcschr(szNLSeps, *pCh)) )
		{
			rlcWordEnd = txtIter.GetPosLC();
			pCh = txtIter.SeekPrev();
		}

		if (!pCh || rlcWordEnd.nCol == 0 || bOverNL)
		{
			rlcWordEnd.nCol++;
			rlcWordStart = rlcWordEnd;
			return TRUE;
		}

		bStartCharSep = !!_tcschr(szSeps, *pCh);
	}

	if (rbOverSpace && nDir > 0) // one word border find - find the other one
	{
		XTP_EDIT_LINECOL& rlc1 = (nDir < 0) ? rlcWordEnd   : rlcWordStart;
		XTP_EDIT_LINECOL& rlc2 = (nDir < 0) ? rlcWordStart : rlcWordEnd;

		rlc1 = txtIter.GetPosLC();
		rlc2 = txtIter.GetPosLC();

		while (pCh && (_tcschr(szSpaceSeps, *pCh) ||
			_tcschr(szNLSeps, *pCh)) )
		{
			pCh = (nDir < 0) ? txtIter.SeekPrev() : txtIter.SeekNext(1, 2);
			rlc2 = txtIter.GetPosLC();
		}
	}
	else //find 2 word borders
	{
		rlcWordStart = txtIter.GetPosLC();
		while (pCh && !((bStartCharSep == (!_tcschr(szSeps, *pCh))) ||
							_tcschr(szSpaceSeps, *pCh) || _tcschr(szNLSeps, *pCh)))
		{
			rlcWordStart = txtIter.GetPosLC();
			pCh = txtIter.SeekPrev();
		}

		pCh = txtIter.SeekNext(1, 2);
		rlcWordEnd = txtIter.GetPosLC();
		while (pCh && !(bStartCharSep == !_tcschr(szSeps, *pCh) ||
						_tcschr(szSpaceSeps, *pCh) || _tcschr(szNLSeps, *pCh))
			)

		{
			pCh = txtIter.SeekNext(1, 2);
			rlcWordEnd = txtIter.GetPosLC();
		}

		if (rlcWordEnd > txtIter.GetPosLC_last(FALSE))
			rlcWordEnd = txtIter.GetPosLC_last(FALSE);
	}

	rlcWordStart.nCol++;
	rlcWordEnd.nCol++;

	return TRUE;
}

void CXTPSyntaxEditCtrl::DeleteSelectedLines(int iForceDeleteRow)
{
	int iLineFrom, iLineTo;
	int iColTo = 1;

	if (m_pSelection->IsSelExist())
	{
		iLineFrom = m_pSelection->GetNormalStart_str().nLine;
		iLineTo = m_pSelection->GetNormalEnd_str().nLine;;
	}
	else
	{
		iLineFrom = iForceDeleteRow;
		iLineTo = iLineFrom + 1;
	}

	if (iLineTo > GetRowCount())
	{
		iLineTo = GetRowCount();

		const int iLen = (int)_tcsclen(GetLineText(iLineTo));

		if (iLen == 0 && (iLineTo < 1 || iLineTo < iLineFrom))
			iColTo = 1;
		else
			iColTo = iLen + 1;

		if (iLineFrom == iLineTo && iColTo == 1 && iLen == 0)   // Nothing to delete
			return;
	}

	m_pSelection->Reset_disp(iLineFrom, 1);

	int nFlags = xtpEditLMRefresh_Delete | xtpEditLMRefresh_Delete_only1;
	if (iColTo > 1)
		nFlags |= xtpEditLMRefresh_Delete_only2;

	GetLineMarksManager()->RefreshLineMarks(iLineFrom, iLineTo, nFlags);

	DeleteBuffer(iLineFrom, 1, iLineTo, iColTo);
}

BOOL CXTPSyntaxEditCtrl::IncreaseIndent()
{
	if (!m_pSelection->IsSelExist() ||
		(m_pSelection->GetStart_disp().nLine == m_pSelection->GetEnd_disp().nLine &&
		m_pSelection->GetStart_disp().nLine < GetRowCount()))
	{
		if (!m_pBuffer->GetOverwriteFlag())
			return FALSE;

		CString strLineText = GetLineText(GetCurrentDocumentRow());
		const int iLineLen = (int)_tcsclen(strLineText);

		int iMaxDispCol = CalcDispCol(strLineText, (iLineLen + 1));

		if (m_nDispCol >= iMaxDispCol)
			return FALSE;

		if (m_nCurrentCol > (iLineLen + 1))
			m_nCurrentCol = (iLineLen + 1);

		m_nDispCol += (GetTabSize() - (m_nDispCol % GetTabSize()));     // Align by tab size

		if (m_nDispCol < 1)
			m_nDispCol = 1;

		m_nDispCol = CalcValidDispCol(strLineText, m_nDispCol);
		m_nCurrentCol = CalcAbsCol(strLineText, m_nDispCol);

		SetCurCaretPos(GetCurrentDocumentRow(), m_nDispCol);
		return TRUE;
	}

	int iRowFrom = m_pSelection->GetNormalStart_str().nLine;
	int iRowTo = m_pSelection->GetNormalEnd_str().nLine;
	int iColTo = m_pSelection->GetNormalEnd_str().nCol;
	int iColTo2 = m_pSelection->GetStart_disp().nCol;
	if (iColTo == 0 && iColTo2 == 1 && iRowTo > iRowFrom)
		iRowTo--;

	if (!CanEditDoc())
		return FALSE;

	BOOL bModified = FALSE;

	// First insert tabs in front
	// If overwrite flag is not set
	if (!m_pBuffer->GetOverwriteFlag())
	{
		const CString strText(GetTabWithSpace() ? CString((TCHAR)(' '), GetTabSize()) : _T("\x09"));

		for (int i = iRowFrom; i <= iRowTo; i++)
		{
			//**----------------------
			OnBeforeEditChanged(i, 1);
			//**----------------------

			m_pBuffer->InsertText(strText, i, 1);

			bModified = TRUE;

			if (i > iRowFrom)
			{
				m_pBuffer->GetUndoRedoManager()->ChainLastCommand();
				m_pBuffer->GetUndoRedoManager()->SetLastCommandText(XTP_IDS_EDIT_INDENTATION);
			}

			//**----------------------
			OnEditChanged(i, 1, i, 2, xtpEditActInsert);
			//**----------------------
		}

		if (bModified)
		{
			NotifyEditChanged(iRowFrom, iRowTo, XTP_EDIT_EDITACTION_MODIFYROW);
		}
	}

	// Select the lines
	m_pSelection->Reset_disp(iRowFrom, 1);
	m_pSelection->SetEnd_disp(iRowTo + 1, 1);

	if (bModified)
		SetDocModified();

	SetCurCaretPos(GetCurrentDocumentRow(), m_nDispCol);

	UpdateWindow();
	UpdateScrollPos();

	return TRUE;
}

BOOL CXTPSyntaxEditCtrl::DecreaseIndent()
{
	if (!m_pSelection->IsSelExist())
	{
		if (m_nCurrentCol < 1)
			m_nCurrentCol = 1;

		LPCTSTR szLineText = GetLineText(GetCurrentDocumentRow());

		int iAddFactor = 1;

		if ((m_nDispCol - 1) % GetTabSize())
			iAddFactor = 0;

		m_nDispCol = 1 + ((((m_nDispCol - 1) / GetTabSize()) - iAddFactor) * GetTabSize()); // Align by tab size

		if (m_nDispCol < 1)
			m_nDispCol = 1;

		m_nDispCol = CalcValidDispCol(szLineText, m_nDispCol);
		m_nCurrentCol = CalcAbsCol(szLineText, m_nDispCol);

		SetCurCaretPos(GetCurrentDocumentRow(), m_nDispCol);

		return TRUE;
	}

	int iRowFrom = m_pSelection->GetNormalStart_str().nLine;
	int iRowTo = m_pSelection->GetNormalEnd_str().nLine;
	int iColTo = m_pSelection->GetNormalEnd_str().nCol;
	int iColTo2 = m_pSelection->GetStart_disp().nCol;
	if (iColTo == 0 && iColTo2 == 1 && iRowTo > iRowFrom)
		iRowTo--;

	if (!CanEditDoc())
		return FALSE;

	CString strLineText;

	BOOL bFrontDeleted = FALSE;
	BOOL bModified = FALSE;

	// First tabs or blank spaces from front
	for (int i = iRowFrom; i <= iRowTo; i++)
	{
		GetLineText(i, strLineText);
		const int iLen = (int)_tcsclen(strLineText);

		if (iLen == 0)
			continue;

		//**----------------------
		OnBeforeEditChanged(i, 1);
		//**----------------------

		if (strLineText.GetAt(0) == _T('\t'))
		{
			m_pBuffer->DeleteText(i, 1, i, 2);
			bFrontDeleted = TRUE;
			bModified = TRUE;
		}
		else
		{
			// No tab found in the front
			// try to find blank spaces
			int iSpaceCount = 0;
			for (int iCol = 0; iCol < GetTabSize() && iCol < iLen; iCol++)
			{
				if (strLineText.GetAt(iCol) == 0x20)
					iSpaceCount++;
				else
					break;
			}
			if (iSpaceCount)
			{
				m_pBuffer->DeleteText(i, 1, i, (iSpaceCount + 1));
				bFrontDeleted = TRUE;
				bModified = TRUE;
			}
		}

		if (bFrontDeleted)
		{
			m_pBuffer->GetUndoRedoManager()->ChainLastCommand();
			m_pBuffer->GetUndoRedoManager()->SetLastCommandText(XTP_IDS_EDIT_INDENT_DECREASE);
		}

		//**----------------------
		OnEditChanged(i, 1, i, 1, xtpEditActDelete);
		//**----------------------
	}

	if (bModified)
	{
		NotifyEditChanged(iRowFrom, iRowTo, XTP_EDIT_EDITACTION_MODIFYROW);
	}

	// Select the lines
	m_pSelection->Reset_disp(iRowFrom, 1);
	m_pSelection->SetEnd_disp(iRowTo + 1, 1);

	if (bModified)
		SetDocModified();

	SetCurCaretPos(GetCurrentDocumentRow(), m_nDispCol);

	UpdateWindow();
	UpdateScrollPos();

	return TRUE;
}

void CXTPSyntaxEditCtrl::RecalcScrollBars()
{
	_RecalcScrollBars();

	CalculateEditbarLength();
}

void CXTPSyntaxEditCtrl::RedrawScrollBar(int nBar)
{
	CScrollBar* pScrollBar = GetScrollBarCtrl(nBar);

	if (pScrollBar)
	{
		SAFE_INVALIDATE(pScrollBar);
	}
	else
	{
		if (m_hWnd) SendMessage(WM_NCPAINT);
	}
}

void CXTPSyntaxEditCtrl::_RecalcHScrollMaxWidth()
{
	CRect rcText = m_pDrawTextProcessor->GetTextRect();
	int nPageX = rcText.Width();

	int nMaxX = m_pDrawTextProcessor->GetRowsMaxWidth();
	nMaxX += max(nPageX/4, m_pDrawTextProcessor->GetTextMetrics().tmAveCharWidth * 20);
	nMaxX = max(nMaxX, rcText.Width() * 2);

	m_nHScrollMaxWidth = max(nMaxX, m_nHScrollMaxWidth);
}

void CXTPSyntaxEditCtrl::_RecalcScrollBars()
{
	if (!GetPaintManager()->GetFont()->GetSafeHandle())
		return;

	if (!m_hWnd || !::IsWindow(m_hWnd))
		return;

	// create scroll bar info
	SCROLLINFO info;
	ZeroMemory(&info, sizeof(SCROLLINFO));
	info.cbSize = sizeof(SCROLLINFO);
	info.fMask = SIF_ALL;

	int nVisRows = GetVisibleRowsCount();

	CRect rcText = m_pDrawTextProcessor->GetTextRect();

	int nPageX = rcText.Width();
	int nPageY = m_pDrawTextProcessor->GetRowsCount(FALSE);

	_RecalcHScrollMaxWidth();

	if (GetRowCount() <= 0)
	{
		if (GetVertScrollBar())
		{
			GetScrollInfo(SB_VERT, &info);

			info.cbSize = sizeof(info);
			info.fMask =  SIF_PAGE | SIF_RANGE;
			info.nPage = nPageY;
			info.nMax= nPageY;

			SetScrollInfo(SB_VERT, &info);
			EnableScrollBarCtrl(SB_VERT, FALSE);
		}

		if (GetHorzScrollBar())
		{
			info.fMask = SIF_ALL;
			GetScrollInfo(SB_HORZ, &info);

			info.nMax = m_nHScrollMaxWidth;
			info.nPage = nPageX;
			info.fMask = SIF_DISABLENOSCROLL | SIF_PAGE | SIF_RANGE;

			SetScrollInfo(SB_HORZ, &info);
		}

		if (GetVertScrollBar() && m_bVertScrollBarEnabled)
		{
			m_bVertScrollBarEnabled = FALSE;
			RedrawScrollBar(SB_VERT);
		}
	}
	else
	{
		if (GetHorzScrollBar())
		{
			info.fMask = SIF_ALL;
			GetScrollInfo(SB_HORZ, &info);

			info.nMin = 0;
			info.nMax = max(m_nHScrollMaxWidth, info.nMax);
			info.nPage = nPageX;
			info.fMask = SIF_PAGE | SIF_POS | SIF_RANGE;

			SetScrollInfo(SB_HORZ, &info);
			EnableScrollBarCtrl(SB_HORZ);
		}

		//---------------------------------------
		if (GetVertScrollBar())
		{
			info.fMask = SIF_ALL;
			GetScrollInfo(SB_VERT, &info);

			int nMaxRowInPage = GetRowPerPage();
			BOOL bVertScrl = (nVisRows >= nMaxRowInPage);

			if (bVertScrl)
			{
				int nVisRow = GetVisibleRowsCount(m_nTopRow);

				info.nMin = 1;
				info.nMax = nVisRows;
				info.nPage = nPageY;
				info.nPos = nVisRow;

				info.fMask = SIF_PAGE | SIF_POS | SIF_RANGE;

				SetScrollInfo(SB_VERT, &info);
			}
			else
			{
				m_nTopRow = 1;
				Invalidate(FALSE);
			}

			EnableScrollBarCtrl(SB_VERT, bVertScrl);

			if (m_bVertScrollBarEnabled != bVertScrl)
			{
				m_bVertScrollBarEnabled = bVertScrl;
				RedrawScrollBar(SB_VERT);
			}
		}
	}

	if (GetCurrentDocumentRow() > GetRowCount())
		SetCurCaretPos(GetRowCount(), m_nDispCol);
}

void CXTPSyntaxEditCtrl::RecalcVertScrollPos()
{
	if (!::IsWindow(m_hWnd))
		return;

	if (!GetPaintManager()->GetFont()->GetSafeHandle())
		return;

	//if (!GetVertScrollBar())
	//  return;

	int nPageRows = GetRowPerPage();
	int nTotalVisRows = GetVisibleRowsCount();

	BOOL bVertScrl = (nTotalVisRows >= nPageRows);

	if (bVertScrl)
	{
		int nVisRow = GetVisibleRowsCount(m_nTopRow);

		SCROLLINFO si;
		ZeroMemory(&si, sizeof(SCROLLINFO));

		si.cbSize = sizeof(si);
		si.fMask = SIF_PAGE | SIF_POS | SIF_RANGE;
		si.nMin = 1;
		si.nMax = nTotalVisRows;
		si.nPage = nPageRows;
		si.nPos = nVisRow;

		VERIFY(SetScrollInfo(SB_VERT, &si));
	}
	else
	{
		m_nTopRow = 1;
		Invalidate(FALSE);
	}

	EnableScrollBarCtrl(SB_VERT, bVertScrl);
}

void CXTPSyntaxEditCtrl::RecalcHorzScrollPos()
{
	if (!::IsWindow(m_hWnd))
		return;

	if (!GetPaintManager()->GetFont()->GetSafeHandle())
		return;

	if (!GetHorzScrollBar())
		return;

	_RecalcHScrollMaxWidth();

	int nPos = GetScrollPos(SB_HORZ);

	SCROLLINFO si;
	ZeroMemory(&si, sizeof(SCROLLINFO));

	si.cbSize = sizeof(si);
	si.fMask = SIF_PAGE | SIF_POS | SIF_RANGE;
	si.nMin = 0;
	si.nMax = si.nMax = max(m_nHScrollMaxWidth, si.nMax);
	si.nPage = m_pDrawTextProcessor->GetTextRect().Width();
	si.nPos = nPos;

	VERIFY(SetScrollInfo(SB_HORZ, &si, FALSE));
}

void CXTPSyntaxEditCtrl::NotifyCurRowCol(int iRow, int iCol) const
{
	// Notify the parent class that position has been changed
	XTP_EDIT_NMHDR_ROWCOLCHANGED src;

	// NMHDR codes
	src.nmhdr.code = XTP_EDIT_NM_ROWCOLCHANGED;
	src.nmhdr.hwndFrom = m_hWnd;
	src.nmhdr.idFrom = GetDlgCtrlID();

	// Row col specific codes
	src.nRow = iRow;
	src.nCol = iCol;

	// Notify the parent window
	if (::IsWindow(m_pParentWnd->GetSafeHwnd()))
	{
		m_pParentWnd->SendMessage(
			WM_NOTIFY, (WPARAM)src.nmhdr.idFrom, (LPARAM)&src);
	}
}

void CXTPSyntaxEditCtrl::ValidateCol(const int nRow, int &nCol, int& nAbsCol)
{
	CString strText;

	GetLineText(nRow, strText);

	nCol = CalcValidDispCol(strText, nCol);

	nAbsCol = CalcAbsCol(strText, nCol);
}

void CXTPSyntaxEditCtrl::EnableOleDrag(BOOL bEnableDrag)
{
	m_bEnableOleDrag = bEnableDrag;
}

HGLOBAL CXTPSyntaxEditCtrl::GetSelectionBuffer(UINT nFormat)
{
	if (!m_pSelection->IsSelExist())
		return NULL;

	if (nFormat != CF_TEXT && nFormat != CF_UNICODETEXT)
		return NULL;

	CMemFile file(CalcAveDataSize(m_pSelection->GetNormalStart_str().nLine,
		m_pSelection->GetNormalEnd_str().nLine));

	BOOL bRes = m_pBuffer->GetBuffer(m_pSelection->GetNormalStart_disp(),
		m_pSelection->GetNormalEnd_disp(), file,
		m_pSelection->bBlockSelectionMode, TRUE);
	if (!bRes)
		return NULL;

	file.Seek(0, CFile::end);
	file.Write(_T("\0"), sizeof(TCHAR));
	int nLen = (int)file.GetLength();
	BYTE *pBytes = file.Detach();

	UINT uCodePage = m_pBuffer->GetCodePage();
#ifdef _UNICODE
	// If Unicode defined then for CF_TEXT conversion is needed
	if (nFormat == CF_TEXT)
	{
		nLen = ::WideCharToMultiByte(uCodePage, 0, (LPWSTR)pBytes, -1, NULL, 0, NULL, NULL);

	}
#else
	if (nFormat == CF_UNICODETEXT)
	{
		nLen = ::MultiByteToWideChar(uCodePage, 0, (LPCSTR)pBytes, -1, NULL, 0);
		nLen *= sizeof(wchar_t);
	}
#endif

	HGLOBAL hMem = ::GlobalAlloc(GMEM_MOVEABLE, nLen);

	if (!hMem)
		return 0;

	void *pText = ::GlobalLock(hMem);

#ifdef _UNICODE
	if (nFormat == CF_UNICODETEXT)
		MEMCPY_S(pText, pBytes, nLen);
	else
	{
		::WideCharToMultiByte(uCodePage, 0, (LPWSTR)pBytes, -1, (LPSTR)pText, nLen, NULL, NULL);
	}
#else
	if (nFormat == CF_TEXT)
		MEMCPY_S(pText, pBytes, nLen);
	else
	{
		::MultiByteToWideChar(uCodePage, 0, (LPSTR)pBytes, -1, (LPWSTR)pText, (nLen / sizeof(wchar_t)));
	}
#endif

	::GlobalUnlock(hMem);

	::free(pBytes);

	return hMem;
}

int CXTPSyntaxEditCtrl::GetSelectionText(CString &strText)
{
	if (!m_pSelection->IsSelExist())
		return 0;

	CMemFile file(CalcAveDataSize(m_pSelection->GetNormalStart_str().nLine,
		m_pSelection->GetNormalEnd_str().nLine));
	BOOL bRes = m_pBuffer->GetBuffer(m_pSelection->GetNormalStart_disp(),
		m_pSelection->GetNormalEnd_disp(), file,
		m_pSelection->bBlockSelectionMode, TRUE);
	if (!bRes)
		return 0;

	file.Write((LPVOID)_T("\0"), sizeof(TCHAR));

	int iLen = (int)file.GetLength();

	BYTE *pBytes = file.Detach();

	strText = (LPTSTR)pBytes;

	free(pBytes);

	return iLen;
}

void CXTPSyntaxEditCtrl::SetDropPos(int iRow, int iCol)
{
	m_ptDropPos.y = iRow;
	m_ptDropPos.x = iCol;
}

void CXTPSyntaxEditCtrl::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CWnd::OnShowWindow(bShow, nStatus);
}

void CXTPSyntaxEditCtrl::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CWnd::OnActivate(nState, pWndOther, bMinimized);

	SetActive(nState != WA_INACTIVE);
}

void CXTPSyntaxEditCtrl::SetActive(BOOL bIsActive)
{
	m_bIsActive = bIsActive;

	if (::IsWindow(m_hWnd))
	{
		RedrawWindow();
	}
}

void CXTPSyntaxEditCtrl::HandleDrop(BOOL bCopy)
{
	if (!CanEditDoc())
		return;

	if (!m_bRightButtonDrag)
		CopyOrMoveText(bCopy);
	else
	{
		CMenu menu;
		menu.CreatePopupMenu();

		CString csMenuText;

		XTPResourceManager()->LoadString(&csMenuText, XTP_IDS_EDIT_MOVE_HERE);
		menu.AppendMenu(MF_STRING, XTP_IDC_EDIT_DRAG_MOVE, csMenuText);

		XTPResourceManager()->LoadString(&csMenuText, XTP_IDS_EDIT_COPY_HERE);
		menu.AppendMenu(MF_STRING, XTP_IDC_EDIT_DRAG_COPY, csMenuText);

		menu.AppendMenu(MF_SEPARATOR);

		XTPResourceManager()->LoadString(&csMenuText, XTP_IDS_EDIT_CANCEL);
		menu.AppendMenu(MF_STRING, IDCANCEL, csMenuText);

		CPoint pt;
		GetCursorPos(&pt);

		int nID = menu.TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON|TPM_NONOTIFY|TPM_RETURNCMD, pt.x, pt.y, this);

		if (nID == XTP_IDC_EDIT_DRAG_MOVE || nID == XTP_IDC_EDIT_DRAG_COPY)
			CopyOrMoveText(nID == XTP_IDC_EDIT_DRAG_COPY);
	}

	m_bRightButtonDrag = FALSE;
	m_bDragging = FALSE;
}

void CXTPSyntaxEditCtrl::ShowDefaultContextMenu()
{
	CPoint ptCur;
	GetCursorPos(&ptCur);
	OnContextMenu(this, ptCur);
}

void CXTPSyntaxEditCtrl::EnableWhiteSpace(BOOL bShow)
{
	m_pOptions->m_bEnableWhiteSpace = bShow;

	InvalidateAll();
}

void CXTPSyntaxEditCtrl::EnableVirtualSpace(BOOL bEnable, BOOL bUpdateReg)
{
	SetValueBool(XTP_EDIT_REG_VIRTUALSPACE, bEnable, m_pOptions->m_bVirtualSpace, bUpdateReg);
}

void CXTPSyntaxEditCtrl::SetCollapsibleNodes(BOOL bDrawNodes, BOOL bUpdateReg)
{
	SetValueBool(XTP_EDIT_REG_COLLAPSIBLENODES, bDrawNodes, m_pOptions->m_bDrawNodes, bUpdateReg);

	CalculateEditbarLength();
	SetCurCaretPos(GetCurrentDocumentRow(), m_nDispCol, FALSE, TRUE);

	if (!GetCollapsibleNodes())
		ExpandAll();
	else
		InvalidateAll();
}


BOOL CXTPSyntaxEditCtrl::SetTabWithSpace(BOOL bTabWithSpace, BOOL bUpdateReg/*=FALSE*/)
{
	if (!SetValueBool(XTP_EDIT_REG_TABWITHSPACE, bTabWithSpace, m_pOptions->m_bTabWithSpace, bUpdateReg))
		return FALSE;

	return TRUE;
}

void CXTPSyntaxEditCtrl::NotifySelInit() const
{
	NotifyParent(XTP_EDIT_NM_SELINIT);
}

BOOL CXTPSyntaxEditCtrl::SetTabSize(int nTabSize, BOOL bUpdateReg)
{
	if (!m_pBuffer->SetTabSize(nTabSize, bUpdateReg))
		return FALSE;

	m_pDrawTextProcessor->SetTabSize(nTabSize);

	CString strText;
	GetLineText(GetCurrentDocumentRow(), strText);

	m_nDispCol = CalcDispCol(strText, m_nCurrentCol);

	SetCurCaretPos(GetCurrentDocumentRow(), m_nDispCol, FALSE, FALSE);

	return TRUE;
}

int CXTPSyntaxEditCtrl::GetTabSize() const
{
	return m_pBuffer ? m_pBuffer->GetTabSize() : 4;
}

BOOL CXTPSyntaxEditCtrl::SetSyntaxColor(BOOL bSyntaxColor, BOOL bUpdateReg)
{
	if (!SetValueBool(XTP_EDIT_REG_SYNTAXCOLOR, bSyntaxColor, m_pOptions->m_bSyntaxColor, bUpdateReg))
		return FALSE;

	m_arOnScreenSchCache->RemoveAll();
	m_pBuffer->EnableParser(bSyntaxColor);

	return TRUE;
}

LRESULT CXTPSyntaxEditCtrl::NotifyParent(UINT uCode) const
{
	if (!m_pParentWnd)
		return -1;

	ASSERT_VALID(m_pParentWnd);

	if (::IsWindow(m_pParentWnd->GetSafeHwnd()))
	{
		NMHDR hdr;
		hdr.hwndFrom = m_hWnd;
		hdr.code = uCode;
		hdr.idFrom = GetDlgCtrlID();

		return m_pParentWnd->SendMessage(
			WM_NOTIFY, (WPARAM)hdr.idFrom, (LPARAM)&hdr);
	}

	return -1;
}

LRESULT CXTPSyntaxEditCtrl::NotifyMouseEvent(UINT uCode, UINT nFlags, const CPoint& point) const
{
	UNREFERENCED_PARAMETER(nFlags);

	ASSERT_VALID(GetParent());

	if (::IsWindow(GetParent()->GetSafeHwnd()))
	{
		NMMOUSE hdrMouse;
		::ZeroMemory(&hdrMouse, sizeof(hdrMouse));

		hdrMouse.hdr.hwndFrom = m_hWnd;
		hdrMouse.hdr.code = uCode;
		hdrMouse.hdr.idFrom = GetDlgCtrlID();
		hdrMouse.pt = point;

		return GetParent()->SendMessage(WM_NOTIFY, (WPARAM)hdrMouse.hdr.idFrom,
			(LPARAM)&hdrMouse);
	}

	return -1;
}

BOOL CXTPSyntaxEditCtrl::SetSelMargin(BOOL bSelMargin, BOOL bUpdateReg)
{
	if (!SetValueBool(XTP_EDIT_REG_SELMARGIN, bSelMargin, m_pOptions->m_bSelMargin, bUpdateReg))
		return FALSE;

	CalculateEditbarLength();

	SetCurCaretPos(GetCurrentDocumentRow(), m_nDispCol, FALSE, FALSE);

	return TRUE;
}

BOOL CXTPSyntaxEditCtrl::SetScrollBars(BOOL bHorzSBar, BOOL bVertSBar, BOOL bUpdateReg/*=FALSE*/)
{
	if (!SetValueBool(XTP_EDIT_REG_HSCROLLBAR, bHorzSBar, m_bHorzScrollBar, bUpdateReg))
		return FALSE;

	if (!SetValueBool(XTP_EDIT_REG_VSCROLLBAR, bVertSBar, m_bVertScrollBar, bUpdateReg))
		return FALSE;

	DWORD dwStyle = ::GetWindowLong(m_hWnd, GWL_STYLE) & ~(WS_HSCROLL|WS_VSCROLL);

	if (bHorzSBar && !IsCreateScrollbarOnParent())
		dwStyle |= WS_HSCROLL;

	if (bVertSBar && !IsCreateScrollbarOnParent())
		dwStyle |= WS_VSCROLL;

	EnableScrollBarCtrl(SB_HORZ, m_bHorzScrollBar);
	EnableScrollBarCtrl(SB_VERT, m_bVertScrollBar);

	::SetWindowLong(m_hWnd, GWL_STYLE, dwStyle);

	return TRUE;
}

BOOL CXTPSyntaxEditCtrl::CanEditDoc() const
{
	if(!m_hWnd)
		return FALSE;

	if (IsReadOnly())
		return FALSE;

	BOOL bCancel = (BOOL)NotifyParent(XTP_EDIT_NM_EDITCHANGING);
	return !bCancel;
}

void CXTPSyntaxEditCtrl::NotifyEditChanged(int iRowFrom, int iRowTo, UINT nActions)
{
	XTP_EDIT_NMHDR_EDITCHANGED sec;

	sec.nmhdr.code = XTP_EDIT_NM_EDITCHANGED;
	sec.nmhdr.hwndFrom = m_hWnd;
	sec.nmhdr.idFrom = GetDlgCtrlID();
	sec.nRowFrom = iRowFrom;
	sec.nRowTo = iRowTo;
	sec.nAction = nActions;

	RefreshLineMarks(&sec);

	// Notify the parent window
	if (::IsWindow(m_pParentWnd->GetSafeHwnd()))
	{
		m_pParentWnd->SendMessage(
			WM_NOTIFY, (WPARAM)sec.nmhdr.idFrom, (LPARAM)&sec);
	}
}

BOOL CXTPSyntaxEditCtrl::CreateInsertText(LPTSTR szText,
		CString& strTextToIns,
		int& iNewRow,
		int& iNewCol,
		int& iNewDispCol,
		int& iEditRowFrom,
		int& iEditRowTo,
		int& iChainActionCount)
{
	// As this function will always be called by OnChar
	// We can assume szText will contain only one character
	// or the CRLF

	int nCurDocRow = GetCurrentDocumentRow();
	CString strLineText;
	GetLineText(nCurDocRow, strLineText);

	int nLineLenC = (int)_tcsclen(strLineText);

	BOOL bTextCRLF = m_pBuffer->IsTextCRLF(szText);

	BOOL bProcess = TRUE;

	if (!bTextCRLF)
	{
		if (szText[0] == _T('\t') && !GetTabWithSpace() &&
			m_nCurrentCol > 1 && nLineLenC > 0)
		{
			int nAlign = (m_nDispCol-1) % GetTabSize();
			iNewDispCol = m_nDispCol - nAlign + GetTabSize();

			int nDispCol = CalcDispCol(strLineText, nLineLenC + 1);
			int nCurCol = m_nCurrentCol;

			if (nDispCol < m_nDispCol)
			{
				nCurCol = nLineLenC+1;
			}
			else
			{
				nDispCol = m_nDispCol;
			}

			int nCharsToCheck = nCurCol-1;

			if (nCharsToCheck)
			{
				LPCTSTR pCheck = _tcsninc((LPCTSTR)strLineText, max(0, nCurCol - 1 - 1));
				int nBlanks;
				for (nBlanks = 0; pCheck && nBlanks < nCharsToCheck; nBlanks++)
				{
					if (!pCheck || *pCheck != _T(' '))
						break;

					pCheck = _tcsdec((LPCTSTR)strLineText, pCheck);
				}

				if (nBlanks > 0)
				{
					int nAdditionalTabs = 0;
					int nDC = m_nDispCol - nBlanks;
					nDC += GetTabSize() - nDC % GetTabSize();
					for (; nDC < m_nDispCol; nDC += GetTabSize())
					{
						nAdditionalTabs++;
					}

					m_pBuffer->SetOverwriteFlag(FALSE);

					nCurDocRow = GetCurrentDocumentRow();

					//**----------------------
					OnBeforeEditChanged(nCurDocRow, nCurCol - nBlanks);

					m_pBuffer->DeleteText(nCurDocRow, nCurCol - nBlanks, nCurDocRow, nCurCol);

					OnEditChanged(nCurDocRow, nCurCol - nBlanks, nCurDocRow, nCurCol, xtpEditActDelete);
					//**----------------------

					m_pBuffer->GetUndoRedoManager()->SetLastCommandText(XTP_IDS_EDIT_FORMAT);

					m_nDispCol -= nBlanks;
					m_nCurrentCol -= nBlanks;

					iChainActionCount = 2;

					GetLineText(nCurDocRow, strLineText);
					nLineLenC = (int)_tcsclen(strLineText);

					if (nAdditionalTabs)
					{
						strTextToIns += CString(_T('\t'), nAdditionalTabs);
					}
				}
			}

		}
		else
		{
			//iNewDispCol = m_nDispCol + (szText[0] == 0x09 ? GetTabSize() : 1);
			//iNewCol = m_nCurrentCol + (szText[0] == 0x09 ? (GetTabWithSpace() ? GetTabSize() : 1) : 1);

			//if (GetAutoIndent() && m_nAutoIndentCol > 0)
			//{
			//  strTextToIns += CString(_T('\t'), m_nInsertTabCount) +
			//                  CString(_T(' '), m_nInsertSpaceCount);
			//  m_nDispCol = 1;
			//  m_nCurrentCol = 1;
			//}
			if (szText[0] == _T('\t'))
			{
				// Align to the next tab position
				int nAlign = (m_nDispCol-1) % GetTabSize();
				iNewDispCol = m_nDispCol - nAlign + GetTabSize();

				if (GetTabWithSpace())
				{
					int spaceCnt = GetTabSize() - nAlign;
					strTextToIns += CString(_T(' '), spaceCnt);
					szText++;       // Skip tab, spaces are displayed

					iNewCol = m_nCurrentCol + spaceCnt;
				}
				else
					iNewCol = m_nCurrentCol + 1;
			}
			else
			{
#ifdef XTP_FIXED
				iNewDispCol = m_nDispCol + ( isleadbyte( szText[0] ) ? 2 : 1 );
#else
				iNewDispCol = m_nDispCol + 1;
#endif
				iNewCol = m_nCurrentCol  + 1;
			}

			if (GetAutoIndent() && m_nAutoIndentCol > 0)
			{
				if (GetTabWithSpace())
					strTextToIns += CString(_T(' '), m_nInsertTabCount * GetTabSize());
				else
					strTextToIns += CString(_T('\t'), m_nInsertTabCount);

				strTextToIns += CString(_T(' '), m_nInsertSpaceCount);
				m_nDispCol = 1;
				m_nCurrentCol = 1;
			}
		}

		iNewRow = GetCurrentDocumentRow();

		strTextToIns += szText;

		m_nAutoIndentCol = 0;
		m_nInsertTabCount = 0;
		m_nInsertSpaceCount = 0;
	}
	else if (bTextCRLF)
	{
		// Enter pressed
		nCurDocRow = GetCurrentDocumentRow();

		iNewRow = nCurDocRow + 1;
		iNewCol = iNewDispCol = 1;

		iEditRowFrom = nCurDocRow;

		iEditRowTo = iEditRowFrom + 1;

		strTextToIns = szText;

		if (!GetAutoIndent())
			return TRUE;

		BOOL bInsertMid = (m_nCurrentCol <= nLineLenC);

		DoAutoIndentIfNeed(GetCurrentDocumentRow(), m_nDispCol);

		if (m_nAutoIndentCol > 0 && bInsertMid)
		{
			CString strTabs;
			FillTabs(strTabs, m_nAutoIndentCol);
			strTextToIns += strTabs;

			iNewDispCol = m_nAutoIndentCol;
			iNewCol = (int)_tcsclen(strTabs);

			m_nAutoIndentCol = 0;
		}
	}

	return bProcess;
}

void CXTPSyntaxEditCtrl::DoAutoIndentIfNeed(int nBaseDocRow, int nDispCol_prev)
{
	if (!GetAutoIndent())
		return;

	m_nAutoIndentCol = 0;

	if (nDispCol_prev == 1)
		return;

	CString strTempLineText;
	BOOL bFound = FALSE;

	GetLineText(nBaseDocRow, strTempLineText);

	LPCTSTR pChar = (LPCTSTR)strTempLineText;
	for (; pChar && *pChar; pChar = _tcsinc(pChar))
	{
		if (*pChar == _T('\t'))
		{
			m_nAutoIndentCol += (GetTabSize() - (m_nAutoIndentCol % GetTabSize()));
		}
		else if (*pChar == 0x20)
		{
			m_nAutoIndentCol++;
		}
		else
		{
			bFound = TRUE;
			break;
		}
	}

	if (!bFound && nDispCol_prev > 1)
	{
		bFound = TRUE;
		m_nAutoIndentCol = nDispCol_prev - 1;
	}

	if (nDispCol_prev > 1 && m_nAutoIndentCol > nDispCol_prev - 1)
		m_nAutoIndentCol = nDispCol_prev - 1;

	if (bFound)
	{
		m_nInsertTabCount = m_nAutoIndentCol / GetTabSize();
		m_nInsertSpaceCount = m_nAutoIndentCol % GetTabSize();

		m_nAutoIndentCol++;
	}
	else
	{
		m_nAutoIndentCol = 0;
		m_nInsertTabCount = 0;
		m_nInsertSpaceCount = 0;
	}
}

void CXTPSyntaxEditCtrl::FillTabs(CString &rstrTextToIns, int nDispColl)
{
	int nTabsCount = GetTabWithSpace() ? 0 : (nDispColl-1) / GetTabSize();
	int nSpaceCount = nDispColl - nTabsCount * GetTabSize() - 1;

	if (nTabsCount)
	{
		CString strTabs(_T('\t'), nTabsCount);
		rstrTextToIns += strTabs;
	}

	if (nSpaceCount)
	{
		CString strSpaces(_T(' '), nSpaceCount);
		rstrTextToIns += strSpaces;
	}
}

BOOL CXTPSyntaxEditCtrl::IsRowVisible(int iRow) const
{
	if (iRow > GetRowCount() || iRow < 1)
		return FALSE;

	int iMaxRowInPage = GetRowPerPage();

	return (iRow >= m_nTopRow && iRow <= (m_nTopRow + iMaxRowInPage));
}

BOOL CXTPSyntaxEditCtrl::LowercaseSelection()
{
	return DoChangeSelectionCase(FALSE);
}

BOOL CXTPSyntaxEditCtrl::UppercaseSelection()
{
	return DoChangeSelectionCase(TRUE);
}

BOOL CXTPSyntaxEditCtrl::DoChangeSelectionCase(BOOL bUpper)
{
	if (!m_pSelection->IsSelExist())
		return FALSE;

	m_pBuffer->GetUndoRedoManager()->SetGroupInsertMode(FALSE);

	m_pBuffer->GetUndoRedoManager()->SetGroupInsertMode(TRUE);

	int nRowStrat = m_pSelection->GetNormalStart_disp().nLine;
	int nRowEnd = m_pSelection->GetNormalEnd_disp().nLine;
	for (int nRow = nRowStrat; nRow <= nRowEnd; nRow++)
	{
		int nVisFrom = 0;
		int nVisTo = INT_MAX;

		if (m_pSelection->bBlockSelectionMode || nRow == nRowStrat)
			nVisFrom = m_pSelection->GetNormalStart_disp().nCol;

		if (m_pSelection->bBlockSelectionMode || nRow == nRowEnd)
			nVisTo = m_pSelection->GetNormalEnd_disp().nCol;

		m_pBuffer->ChangeCase(nRow, nVisFrom, nVisTo, bUpper);
	}

	m_pBuffer->GetUndoRedoManager()->SetGroupInsertMode(FALSE);

	m_pBuffer->GetUndoRedoManager()->SetLastCommandText(XTP_IDS_EDIT_CASE);

	InvalidateRows(nRowStrat, nRowEnd);
	Invalidate(FALSE);

	return TRUE;
}

BOOL CXTPSyntaxEditCtrl::DoChangeSelectionTabify(BOOL bTabify)
{
	if (!IsSelectionExist())
		return FALSE;

	m_pBuffer->GetUndoRedoManager()->SetGroupInsertMode(FALSE);
	m_pBuffer->GetUndoRedoManager()->SetGroupInsertMode(TRUE);

	int nRowStrat = m_pSelection->GetNormalStart_disp().nLine;
	int nRowEnd = m_pSelection->GetNormalEnd_disp().nLine;
	for (int nRow = nRowStrat; nRow <= nRowEnd; nRow++)
	{
		int nVisFrom = 0;
		int nVisTo = INT_MAX;

		if (m_pSelection->bBlockSelectionMode || nRow == nRowStrat)
			nVisFrom = m_pSelection->GetNormalStart_disp().nCol;

		if (m_pSelection->bBlockSelectionMode || nRow == nRowEnd)
			nVisTo = m_pSelection->GetNormalEnd_disp().nCol;

		m_pBuffer->ChangeTabification(nRow, nVisFrom, nVisTo, bTabify);
	}

	m_pBuffer->GetUndoRedoManager()->SetGroupInsertMode(FALSE);
	m_pBuffer->GetUndoRedoManager()->SetLastCommandText(bTabify ? XTP_IDS_EDIT_TABIFY : XTP_IDS_EDIT_UNTABIFY);

	InvalidateRows(nRowStrat, nRowEnd);
	Invalidate(FALSE);

	return TRUE;
}

BOOL CXTPSyntaxEditCtrl::TabifySelection()
{
	return DoChangeSelectionTabify(TRUE);
}

BOOL CXTPSyntaxEditCtrl::UnTabifySelection()
{
	return DoChangeSelectionTabify(FALSE);
}

BOOL CXTPSyntaxEditCtrl::SetLineNumbers(BOOL bLineNumbers, BOOL bUpdateReg/*=FALSE*/)
{
	if (m_pOptions->m_bLineNumbers == bLineNumbers)
		return TRUE;

	if (!SetValueBool(XTP_EDIT_REG_LINENUMBERS, bLineNumbers, m_pOptions->m_bLineNumbers, bUpdateReg))
		return FALSE;

	CalculateEditbarLength();
	SetCurCaretPos(GetCurrentDocumentRow(), m_nDispCol, FALSE, TRUE);

	InvalidateAll();

	return TRUE;
}

void CXTPSyntaxEditCtrl::CalculateEditbarLength(CDC* pDC)
{
	if (!m_hWnd)
		return;

	int nPrvEditbarLength = m_nEditbarLength;

	// gutter
	m_nEditbarLength = GetSelMargin() ? m_nMarginLength : 0;

	// line numbers
	m_nLineNumLength = 0;

	if (GetLineNumbers())
	{
		int nMaxPageRow = GetRowCount();

		if (pDC && pDC->IsPrinting())
			nMaxPageRow = GetTopRow() + GetRowPerPage();

		int nRowNumLen = (nMaxPageRow > 9 ? (int)log10l(nMaxPageRow) : 0) + 1;
		CString sLineNumberFormat(GetPaintManager()->m_sLineNumberFormat);
		if (!sLineNumberFormat.IsEmpty())
		{
			sLineNumberFormat.Replace(_T("%"), _T(""));
			sLineNumberFormat.Replace(_T("d"), _T(""));
			nRowNumLen = max(_ttoi(sLineNumberFormat), nRowNumLen);
		}

		CWindowDC dc(NULL);

		CXTPFontDC fontDC(&dc, GetPaintManager()->GetFontLineNumber());

		TEXTMETRIC tm;
		VERIFY(dc.GetTextMetrics(&tm));

		m_nLineNumLength = (nRowNumLen + 1) * (tm.tmAveCharWidth) + 2;
		m_nEditbarLength += m_nLineNumLength;
	}

	// collapsible nodes area
	if (GetCollapsibleNodes())
	{
		m_nEditbarLength += m_nNodesWidth;
	}

	if (GetSelMargin() || GetLineNumbers() || GetCollapsibleNodes())
		m_nEditbarLength += TEXT_LEFT_OFFSET;

	// update window if necessary
	if (nPrvEditbarLength != m_nEditbarLength && (!pDC || !pDC->IsPrinting()))
	{
		CRect rcText;
		CalcEditRects(NULL, NULL, NULL, &rcText);
		m_pDrawTextProcessor->SetTextRect(rcText);

		_RecalcScrollBars();

		SetCurCaretPos(GetCurrentDocumentRow(), m_nDispCol, FALSE, FALSE);

		Invalidate(FALSE);
	}
}


void CXTPSyntaxEditCtrl::AddRemoveBookmark(int nRow)
{
	if (GetLineMarksManager())
	{
		GetLineMarksManager()->AddRemoveLineMark(nRow, xtpEditLMT_Bookmark);
	}

	RedrawLineMarks();
}

void CXTPSyntaxEditCtrl::AddRemoveBreakPoint(int nRow)
{
	if (GetLineMarksManager())
	{
		GetLineMarksManager()->AddRemoveLineMark(nRow, xtpEditLMT_Breakpoint);
	}

	RedrawLineMarks();
}

void CXTPSyntaxEditCtrl::CollapseExpandBlock(int nRow)
{
	CXTPSyntaxEditLineMarksManager* pMgr = GetLineMarksManager();
	if (!pMgr)
		return;

	// nLineEnd: adjust also column, for example when collapsing to the string of lower length
	int nLineEnd = m_nCurrentCol; //m_nDispCol;

	if (pMgr->HasRowMark(nRow, xtpEditLMT_Collapsed))
	{
		// try expand
		pMgr->DeleteLineMark(nRow, xtpEditLMT_Collapsed);
	}
	else
	{
		// try collapse
		XTP_EDIT_LMPARAM LMCoParam;
		CXTPSyntaxEditRowsBlockArray arCoBlocks;
		GetCollapsableBlocksInfo(nRow, arCoBlocks);

		int nCount = (int)arCoBlocks.GetSize();
		for (int i = 0; i < nCount; i++)
		{
			XTP_EDIT_ROWSBLOCK coBlk = arCoBlocks[i];
			if (coBlk.lcStart.nLine == nRow)
			{
				XTP_EDIT_COLLAPSEDBLOCK* pCoDrawBlk = (XTP_EDIT_COLLAPSEDBLOCK*)LMCoParam.GetPtr();
				if (!pCoDrawBlk)
				{
					pCoDrawBlk = new XTP_EDIT_COLLAPSEDBLOCK;
					if (!pCoDrawBlk)
					{
						return;
					}
					LMCoParam.SetPtr(pCoDrawBlk, XTPSECollapsedBlockDeleteFn);
				}
				pCoDrawBlk->collBlock = coBlk;
				pMgr->SetLineMark(nRow, xtpEditLMT_Collapsed, &LMCoParam);
				m_arCollapsedTextRows.SetAtGrow(m_nCollapsedTextRowsCount, nRow);
				nLineEnd = min(nLineEnd, pCoDrawBlk->collBlock.lcEnd.nCol);
				m_nCollapsedTextRowsCount++;
			}
		}
	}

	SetCurrentDocumentRow(nRow);
	m_nDispCol = nLineEnd + 1;
	SetCurCaretPos(nRow, m_nDispCol);

	Invalidate(FALSE);

	RecalcScrollBars();
}

void CXTPSyntaxEditCtrl::DeleteBookmark(int nRow)
{
	if (GetLineMarksManager())
	{
		GetLineMarksManager()->DeleteLineMark(nRow, xtpEditLMT_Bookmark);
	}
}

void CXTPSyntaxEditCtrl::PrevBookmark()
{
	CXTPSyntaxEditLineMarksManager* pMgr = GetLineMarksManager();
	if (!pMgr)
	{
		ASSERT(FALSE);
		return;
	}
	int nRow = GetCurRow();
	int nVisRow = GetCurrentVisibleRow();

	int nPrevRow = nRow;
	do {
		pMgr->FindPrevLineMark(--nPrevRow, xtpEditLMT_Bookmark);
	} while (nPrevRow > 0 && GetVisibleRow(nPrevRow) == nVisRow);

	if (nPrevRow < 0)
	{
		POSITION posLast = pMgr->GetLastLineMark(xtpEditLMT_Bookmark);
		XTP_EDIT_LMDATA* pData = pMgr->GetLineMarkAt(posLast, xtpEditLMT_Bookmark);
		nPrevRow = pData ? pData->m_nRow : -1;
	}
	if (GetVisibleRow(nPrevRow) == nVisRow)
	{
		nPrevRow = -1;
	}

	if (nPrevRow >= 0)
	{
		SetCurPos(nPrevRow, 1);

		Invalidate(FALSE);
	}
}

void CXTPSyntaxEditCtrl::NextBookmark()
{
	CXTPSyntaxEditLineMarksManager* pMgr = GetLineMarksManager();
	if (!pMgr)
	{
		ASSERT(FALSE);
		return;
	}

	int nRow = GetCurRow();
	int nVisRow = GetCurrentVisibleRow();

	// move down to the next bookmark after the current collapsed block area
	int nNextRow = nRow;
	do {
		pMgr->FindNextLineMark(++nNextRow, xtpEditLMT_Bookmark);
	} while (nNextRow > 0 && GetVisibleRow(nNextRow) == nVisRow);

	if (nNextRow < 0)
	{
		// find first line mark
		POSITION posFirst = pMgr->GetFirstLineMark(xtpEditLMT_Bookmark);
		XTP_EDIT_LMDATA* pData = pMgr->GetNextLineMark(posFirst, xtpEditLMT_Bookmark);
		nNextRow = pData ? pData->m_nRow : -1;
	}

	if (GetVisibleRow(nNextRow) == nVisRow)
	{
		nNextRow = -1;
	}

	if (nNextRow >= 0)
	{
		SetCurPos(nNextRow, 1);

		Invalidate(FALSE);
	}
}

BOOL CXTPSyntaxEditCtrl::HasRowMark(int nRow, const XTP_EDIT_LINEMARKTYPE& lmType,
											XTP_EDIT_LMPARAM* pParam) const
{
	if (GetLineMarksManager())
	{
		return GetLineMarksManager()->HasRowMark(nRow, lmType, pParam);
	}
	return FALSE;
}


void CXTPSyntaxEditCtrl::DeleteBreakpoint(int nRow)
{
	if (GetLineMarksManager())
	{
		GetLineMarksManager()->DeleteLineMark(nRow, xtpEditLMT_Breakpoint);
	}
}

void CXTPSyntaxEditCtrl::RefreshLineMarks(XTP_EDIT_NMHDR_EDITCHANGED* pEditChanged)
{
	if (!GetLineMarksManager())
	{
		ASSERT(FALSE);
		return ;
	}

	int eRefreshType = xtpEditLMRefresh_Unknown;

	if (pEditChanged->nAction & XTP_EDIT_EDITACTION_DELETEROW)
	{
		eRefreshType = xtpEditLMRefresh_Delete;
	}
	else if (pEditChanged->nAction & XTP_EDIT_EDITACTION_INSERTROW)
	{
		eRefreshType = xtpEditLMRefresh_Insert;

		if (pEditChanged->nAction & XTP_EDIT_EDITACTION_INSERTROW_NEW)
			eRefreshType |= xtpEditLMRefresh_InsertAt0;
	}
	else
	{
		return;
	}

	GetLineMarksManager()->RefreshLineMarks(pEditChanged->nRowFrom,
										pEditChanged->nRowTo,  eRefreshType);

	RedrawLineMarks();
}


void CXTPSyntaxEditCtrl::InvalidateAll()
{
	CView* pParentWnd = DYNAMIC_DOWNCAST(CView, m_pParentWnd);

	if (pParentWnd != NULL && pParentWnd->GetDocument() != NULL)
	{
		pParentWnd->GetDocument()->UpdateAllViews(NULL, xtpEditHintInvalidate);
	}
	else
	{
		if (m_hWnd) Invalidate(FALSE);
	}
}

void CXTPSyntaxEditCtrl::RedrawLineMarks()
{
	InvalidateAll();
}

BOOL CXTPSyntaxEditCtrl::HasBookmarks() const
{
	CXTPSyntaxEditLineMarksManager* pMgr = GetLineMarksManager();
	if (pMgr)
	{
		int nCount = pMgr->GetCount(xtpEditLMT_Bookmark);
		return nCount > 0;
	}
	return FALSE;
}

BOOL CXTPSyntaxEditCtrl::HasBreakpoints() const
{
	CXTPSyntaxEditLineMarksManager* pMgr = GetLineMarksManager();
	if (pMgr)
	{
		int nCount = pMgr->GetCount(xtpEditLMT_Breakpoint);
		return nCount > 0;
	}
	return FALSE;
}

CXTPSyntaxEditLineMarksManager* CXTPSyntaxEditCtrl::GetLineMarksManager() const
{
	ASSERT(m_pBuffer);
	return m_pBuffer ? m_pBuffer->GetLineMarksManager() : NULL;
}

CString CXTPSyntaxEditCtrl::GetModulePath()
{
	TCHAR zsFileName[_MAX_PATH];

	//DWORD dwRes = ::GetModuleFileName(NULL, zsFileName, _MAX_PATH);
	DWORD dwRes = ::GetModuleFileName(XTPGetInstanceHandle(), zsFileName, _MAX_PATH);

	ASSERT(dwRes);

	CString csFilePath = zsFileName;
	int nFLs = csFilePath.ReverseFind(_T('\\'));
	if (nFLs > 0)
	{
		csFilePath.ReleaseBuffer(nFLs + 1);
	}

	return csFilePath;
}

CString CXTPSyntaxEditCtrl::GetDefaultCfgFilePath()
{
	return m_strDefaultCfgFilePath;
}

void CXTPSyntaxEditCtrl::RefreshColors()
{
	if (!::IsWindow(m_hWnd) || !m_hWnd)
		return;

	m_arOnScreenSchCache->RemoveAll();

	if (!m_pBuffer)
	{
		ASSERT(FALSE);
		return;
	}

	if (GetSyntaxColor())
	{
		XTP_EDIT_LINECOL pos1_0 = {1, 0};
		BOOL bParseInThread = m_pBuffer->GetLexParser()->GetSchemaOptions(
						m_pBuffer->GetFileExt() )->m_bFirstParseInSeparateThread;
		if (bParseInThread)
		{
			m_pBuffer->GetLexParser()->StartParseInThread(m_pBuffer, &pos1_0, NULL, 0, TRUE);
		}
		else
		{
			CXTPSyntaxEditLexTextSchema* ptrTextSch = m_pBuffer->GetLexParser()->GetTextSchema();
			if (ptrTextSch)
			{
				CXTPSyntaxEditTextIterator txtIter(m_pBuffer);
				ptrTextSch->RunParseUpdate(TRUE, &txtIter, &pos1_0, NULL);
			}
		}
	}
	else
	{
		GetLineMarksManager()->RemoveAll(xtpEditLMT_Collapsed);
	}

	// Load data into AC
	SetAutoCompleteList();

	Invalidate(FALSE);
}

CString CXTPSyntaxEditCtrl::GetConfigFile() const
{
	if (!m_pBuffer)
		return _T("");

	return m_pBuffer->GetConfigFile();
}

BOOL CXTPSyntaxEditCtrl::SetConfigFile(LPCTSTR szPath, BOOL bConfigFileMode)
{
	m_strDefaultCfgFilePath = szPath;
	//m_bConfigFileMode = bConfigFileMode;
	if (!bConfigFileMode)
	{
		m_bUseMonitor = FALSE;
		m_bConfigFileMode = FALSE;
	}
	if (!m_pBuffer)
		return FALSE;

	if (m_bConfigFileMode && !FILEEXISTS_S(szPath))
		return FALSE;

	m_pBuffer->SetConfigFile(szPath);
	return TRUE;
}

CString CXTPSyntaxEditCtrl::GetCurrentTheme() const
{
	if (!m_pBuffer || !m_pBuffer->GetLexConfigurationManager() || !GetSyntaxColor())
	{
		static CString strEmpty;
		return strEmpty;
	}
	return m_pBuffer->GetLexConfigurationManager()->GetCurrentTheme();
}

CStringArray& CXTPSyntaxEditCtrl::GetThemes()
{
	if (!m_pBuffer || !m_pBuffer->GetLexConfigurationManager())
	{
		static CStringArray s_ar;
		return s_ar;
	}
	return m_pBuffer->GetLexConfigurationManager()->GetThemeManager().GetThemes();
}

void CXTPSyntaxEditCtrl::ApplyTheme(CString strTheme)
{
	if (!m_pBuffer || !m_pBuffer->GetLexConfigurationManager() || !GetSyntaxColor())
	{
		return;
	}
	CXTPSyntaxEditLexTextSchema* ptrTextSch = m_pBuffer->GetLexParser()->GetTextSchema();
	m_pBuffer->GetLexConfigurationManager()->SetTheme(strTheme, ptrTextSch);

	m_arOnScreenSchCache->RemoveAll();

	Invalidate(FALSE);
	UpdateWindow();
}


void CXTPSyntaxEditCtrl::OnBeforeEditChanged(int nRow, int nCol)
{
	UNREFERENCED_PARAMETER(nRow); UNREFERENCED_PARAMETER(nCol);

	if (GetSyntaxColor())
	{
		m_pBuffer->GetLexParser()->OnBeforeEditChanged();
	}
}

void CXTPSyntaxEditCtrl::OnEditChanged(const XTP_EDIT_LINECOL& LCFrom,
		const XTP_EDIT_LINECOL& LCTo, int eEditAction)
{
	OnEditChanged(LCFrom.nLine, LCFrom.nCol, LCTo.nLine, LCTo.nCol, eEditAction);
}

void CXTPSyntaxEditCtrl::OnEditChanged(int nRowFrom, int nColFrom,
		int nRowTo, int nColTo, int eEditAction)
{
	InvalidateRows(nRowFrom);

	if (GetSyntaxColor())
	{
		XTP_EDIT_LINECOL posFrom = {nRowFrom, max(0, nColFrom-1) };
		XTP_EDIT_LINECOL posTo = {nRowTo, max(0, nColTo-1) };

		m_pBuffer->GetLexParser()->OnEditChanged(posFrom, posTo, eEditAction, m_pBuffer);
	}
}

BOOL CXTPSyntaxEditCtrl::NotifyParseEvent(XTP_NOTIFY_CODE code, WPARAM wParam, LPARAM lParam)
{
	// Notify the parent class that position has been changed
	XTP_EDIT_NMHDR_PARSEEVENT pe;

	// NMHDR codes
	pe.nmhdr.code = XTP_EDIT_NM_PARSEEVENT;
	pe.nmhdr.hwndFrom = m_hWnd;
	pe.nmhdr.idFrom = GetDlgCtrlID();

	// notification event code.
	pe.code   = code;
	pe.wParam = wParam;
	pe.lParam = lParam;

	// Notify the parent window
	if (::IsWindow(m_pParentWnd->GetSafeHwnd()))
	{
		return (BOOL)m_pParentWnd->SendMessage(
			WM_NOTIFY, (WPARAM)pe.nmhdr.idFrom, (LPARAM)&pe);
	}

	return FALSE;
}

void CXTPSyntaxEditCtrl::OnParseEvent(XTP_NOTIFY_CODE code, WPARAM wParam, LPARAM lParam)
{
	NotifyParseEvent(code, wParam, lParam);

	//-----------------------------------------------------------------------
	int nRSFactor = 3;
	int nAveRedrawTime = m_aveRedrawScreenTime.GetAverageValue(333);
	int nUpdateTimeOut = max(1000, nAveRedrawTime * nRSFactor);

	BOOL bRedraw = FALSE;
	BOOL bRedrawTimeOut = TRUE;

	if (code == xtpEditOnParserStarted)
	{
		DWORD dwTime = ::GetTickCount();
		m_dwLastRedrawTime = dwTime;
	}
	else if (code == xtpEditOnTextBlockParsed)
	{
		CXTPSyntaxEditLexTextBlock* pTBended = (CXTPSyntaxEditLexTextBlock*)wParam;

		DWORD dwTime = ::GetTickCount();
		bRedrawTimeOut = labs(dwTime - m_dwLastRedrawTime) >= nUpdateTimeOut;
		if (bRedrawTimeOut)
		{
			m_dwLastRedrawTime = dwTime;
		}

		if (pTBended)
		{
			int nDispMax = GetRowPerPage() - 1;
			int nBottomRow = GetDocumentRow(nDispMax+5);

			if (!(pTBended->m_PosStartLC.nLine > nBottomRow ||
				pTBended->m_PosEndLC.nLine < m_nTopRow) )
			{
				bRedraw = TRUE;
			}

			ClearOnScreenSchCache(pTBended->m_PosStartLC.nLine);
		}
	}
	else if (code == xtpEditOnParserEnded)
	{
		bRedraw = TRUE;
	}

	//-----------------------------
	if (bRedraw)
	{
		KillTimer(TIMER_REDRAW_WHEN_PARSE);

		if (bRedrawTimeOut)
		{
			Invalidate(FALSE);
			UpdateWindow();
		}
		else
		{
			SetTimer(TIMER_REDRAW_WHEN_PARSE, nUpdateTimeOut*5, NULL);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// Row / col functions

void CXTPSyntaxEditCtrl::SetTopRow(int nRow)
{
	if (nRow < 1)
		m_nTopRow = 1;
	else if (nRow > GetRowCount())
		m_nTopRow = max (1, GetRowCount());
	else
		m_nTopRow = max (1, nRow);
}

int CXTPSyntaxEditCtrl::GetCurRow() const
{
	return GetCurrentDocumentRow();
}


void CXTPSyntaxEditCtrl::GoToRow(int iRow, BOOL bSelectRow/* = FALSE*/)
{
	CString csLineText = GetLineText(iRow);

	m_nCurrentCol = 1;
	m_nDispCol = 1;

	if (bSelectRow)
	{
		m_pSelection->Reset_disp(iRow, 1);
		m_pSelection->SetEnd_disp(iRow+1, 1);

		SetCurrentDocumentRow(iRow);
		Invalidate(FALSE);

		SetCurCaretPos(GetCurrentDocumentRow(), m_nDispCol, TRUE, TRUE);
		//SetTimer(TIMER_SELECTION_ID, TIMER_SELECTION_TIME, NULL);
	}
	else
	{
		SetCurrentDocumentRow(iRow);
		SetCurPos(iRow, 1);
	}
}

int CXTPSyntaxEditCtrl::GetCurrentVisibleRow()
{
	return GetVisibleRow(GetCurrentDocumentRow());
}

int CXTPSyntaxEditCtrl::GetCurrentDocumentRow() const
{
	return m_nCurrentDocumentRow;
}

void CXTPSyntaxEditCtrl::SetCurrentDocumentRow(int nRow)
{
	if (nRow > GetRowCount())
		nRow = GetRowCount();

	if (nRow < 1)
		nRow = 1;

	m_nCurrentDocumentRow = nRow;
}

int CXTPSyntaxEditCtrl::GetDocumentRow(int nVisibleRow)
{
	return CalculateDocumentRow(m_nTopRow, nVisibleRow);
}

int CXTPSyntaxEditCtrl::CalculateDocumentRow(int nStartDocumentRow, int nRowDelta)
{
	int nDocRow = nStartDocumentRow + nRowDelta - 1;
	int nNextCollapsedRow = nStartDocumentRow - 1;

	POSITION pos = GetLineMarksManager()->FindNextLineMark(nNextCollapsedRow, xtpEditLMT_Collapsed);
	while (pos != NULL)
	{
		XTP_EDIT_LMDATA* pData = GetLineMarksManager()->GetNextLineMark(pos, xtpEditLMT_Collapsed);
		if (pData && pData->m_nRow >= nNextCollapsedRow) // mark should be not within previous collapsed block
		{
			if (pData->m_nRow >= nDocRow) // finish if mark is greater then row to calculate for
				break;

			XTP_EDIT_COLLAPSEDBLOCK* pCoDBlk = (XTP_EDIT_COLLAPSEDBLOCK*)pData->m_Param.GetPtr();
			if (!pCoDBlk)
				continue;

			int nHiddenRows = pCoDBlk->collBlock.lcEnd.nLine - pCoDBlk->collBlock.lcStart.nLine;

			nDocRow += nHiddenRows;
			nNextCollapsedRow = pData->m_nRow + nHiddenRows;
		}
	}
	return nDocRow;
}

int CXTPSyntaxEditCtrl::GetVisibleRow(int nDocumentRow)
{
	return CalculateVisibleRow(m_nTopRow, nDocumentRow);
}

int CXTPSyntaxEditCtrl::CalculateVisibleRow(int nStartDocumentRow, int nDocumentRow)
{
	int nVisRow = nDocumentRow - nStartDocumentRow + 1;
	int nNextCollapsedRow = nStartDocumentRow - 1;
	const int nRowCount = GetRowCount();

	POSITION pos = GetLineMarksManager()->FindNextLineMark(nNextCollapsedRow, xtpEditLMT_Collapsed);
	while (pos != NULL)
	{
		XTP_EDIT_LMDATA* pData = GetLineMarksManager()->GetNextLineMark(pos, xtpEditLMT_Collapsed);
		if (pData && pData->m_nRow >= nNextCollapsedRow) // mark should be not within previous collapsed block
		{
			if (pData->m_nRow >= nDocumentRow) // finish if mark is greater then row to calculate for
				break;

			XTP_EDIT_COLLAPSEDBLOCK* pCoDBlk = (XTP_EDIT_COLLAPSEDBLOCK*)pData->m_Param.GetPtr();
			if (!pCoDBlk)
				continue;

			int nHiddenRows = min(nRowCount, pCoDBlk->collBlock.lcEnd.nLine) -
				pCoDBlk->collBlock.lcStart.nLine;

			nVisRow -= nHiddenRows;
			nNextCollapsedRow = pData->m_nRow + nHiddenRows;
		}
	}
	return max(nVisRow, 1);
}

int CXTPSyntaxEditCtrl::MoveCurrentVisibleRowUp(int nCount)
{
	int nOldVisRow = GetCurrentVisibleRow();
	int nOldDocRow = GetCurrentDocumentRow();

	int nNewVisRow = nOldVisRow - nCount;

	// do not shift a window up
	if (nNewVisRow > 0)
	{
		SetCurrentDocumentRow(GetDocumentRow(nNewVisRow));
		return nNewVisRow;
	}

	// find global visible row index
	int nVisRow = CalculateVisibleRow(1, nOldDocRow);
	int nTargetDocRow = CalculateDocumentRow(1, max(nVisRow - nCount, 1));
	int nDocDiff = nOldDocRow - nTargetDocRow;
	nDocDiff = min(nDocDiff, m_nTopRow - 1);
	nOldDocRow -= nDocDiff;
	m_nCurrentDocumentRow -= nDocDiff;
	m_nTopRow -= nDocDiff;

	nNewVisRow = nOldVisRow - nDocDiff + GetRowPerPage();

	return GetCurrentVisibleRow();
}

int CXTPSyntaxEditCtrl::MoveCurrentVisibleRowDown(int nCount)
{
	int nOldRow = GetCurrentVisibleRow();
	int nNewDocRow = min(GetDocumentRow(nOldRow + nCount), GetRowCount());
	SetCurrentDocumentRow(nNewDocRow);
	int nMaxVisRow = GetRowPerPage();
	int nMaxDocRow = GetDocumentRow(nMaxVisRow);
	if (nNewDocRow > nMaxDocRow)
	{
		m_nTopRow = max (1, GetDocumentRow(nCount + 1));
	}
	return GetCurrentVisibleRow();
}

int CXTPSyntaxEditCtrl::GetRowPerPage() const
{
	return m_pDrawTextProcessor->GetRowsCount(FALSE);
}

int CXTPSyntaxEditCtrl::GetVisibleRowsCount(int nMaxDocRow)
{
	if (nMaxDocRow < 0)
		nMaxDocRow = GetRowCount();

	int nVisRow = nMaxDocRow;
	CXTPSyntaxEditLineMarksManager* pMgr = GetLineMarksManager();
	if (pMgr)
	{
		int nNextCollapsedRow = -1;
		POSITION pos = pMgr->GetFirstLineMark(xtpEditLMT_Collapsed);
		while (pos != NULL)
		{
			XTP_EDIT_LMDATA* pData = pMgr->GetNextLineMark(pos, xtpEditLMT_Collapsed);
			if (pData && pData->m_nRow >= nNextCollapsedRow) // mark should be not within previous collapsed block
			{
				if (pData->m_nRow >= nMaxDocRow) // finish if mark is greater then row to calculate for
					break;

				XTP_EDIT_COLLAPSEDBLOCK* pCoDBlk = (XTP_EDIT_COLLAPSEDBLOCK*)pData->m_Param.GetPtr();
				if (!pCoDBlk)
					continue;

				int nHiddenRows = pCoDBlk->collBlock.lcEnd.nLine - pCoDBlk->collBlock.lcStart.nLine;

				nVisRow -= nHiddenRows;
				nNextCollapsedRow = pData->m_nRow + nHiddenRows;
			}
		}
	}
	return nVisRow;
}

BOOL CXTPSyntaxEditCtrl::ShiftCurrentVisibleRowUp(int nCount, BOOL bChangeCaret)
{
	BOOL bChanged = FALSE;
	if (m_nTopRow > 1)
	{
		int nOldTopRow = m_nTopRow;
		int nDocRow = GetCurrentDocumentRow();      // store old document row
		SetCurrentDocumentRow(GetDocumentRow(1));   // move to first visible row
		MoveCurrentVisibleRowUp(nCount);            // move nCount row up
		// move to old document row
		int nBottomDocRow = GetDocumentRow(GetRowPerPage());
		if (bChangeCaret)
			nDocRow = min(nDocRow, nBottomDocRow);
		SetCurrentDocumentRow(nDocRow);
		if (nDocRow > nBottomDocRow)
			CWnd::HideCaret();
		if (nOldTopRow != m_nTopRow)
			bChanged = TRUE;
	}
	return bChanged;
}

BOOL CXTPSyntaxEditCtrl::ShiftCurrentVisibleRowDown(int nCount, BOOL bChangeCaret)
{
	BOOL bChanged = FALSE;
	if (m_nTopRow < GetRowCount())
	{
		int nOldTopRow = m_nTopRow;
		int nDocRow = GetCurrentDocumentRow();
		int nLastVisRow = GetRowPerPage();
		SetCurrentDocumentRow(GetDocumentRow(nLastVisRow));
		MoveCurrentVisibleRowDown(nCount);
		int nTopDocRow = GetDocumentRow(1);
		if (bChangeCaret)
			nDocRow = max(nDocRow, nTopDocRow);
		SetCurrentDocumentRow(nDocRow);
		if (nDocRow < nTopDocRow)
			CWnd::HideCaret();
		if (nOldTopRow != m_nTopRow)
			bChanged = TRUE;
	}
	return bChanged;
}

BOOL CXTPSyntaxEditCtrl::GetCollapsedBlockLen(int nStartRow, int& rnLen) const
{
	rnLen = 0;

	XTP_EDIT_LMPARAM LMCoParam;
	BOOL bIsCollapsed = HasRowMark(nStartRow, xtpEditLMT_Collapsed, &LMCoParam);
	if (!bIsCollapsed)
	{
		return FALSE;
	}

	XTP_EDIT_COLLAPSEDBLOCK* pCoDBlk = (XTP_EDIT_COLLAPSEDBLOCK*)LMCoParam.GetPtr();
	if (!pCoDBlk)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	rnLen = pCoDBlk->collBlock.lcEnd.nLine - pCoDBlk->collBlock.lcStart.nLine;
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// Line text internal functions

CString CXTPSyntaxEditCtrl::GetLineText(int nRow, BOOL bAddCRLF, int iCRLFStyle) const
{
	CString strResult;
	GetLineText(nRow, strResult, bAddCRLF, iCRLFStyle);

	return strResult;
}

void CXTPSyntaxEditCtrl::GetLineText(int nRow, CString& strBuffer, BOOL bAddCRLF, int iCRLFStyle) const
{
	m_pBuffer->GetLineText(nRow, strBuffer, bAddCRLF, iCRLFStyle);

	XTP_EDIT_LMPARAM LMCoParam;
	BOOL bIsCollapsed = HasRowMark(nRow, xtpEditLMT_Collapsed, &LMCoParam);
	if (bIsCollapsed)
	{
		XTP_EDIT_COLLAPSEDBLOCK* pCoDBlk = (XTP_EDIT_COLLAPSEDBLOCK*)LMCoParam.GetPtr();
		if (!pCoDBlk)
		{
			ASSERT(FALSE);
			return;
		}
		if (strBuffer.GetLength() > pCoDBlk->collBlock.lcStart.nCol)
		{
			strBuffer.SetAt(pCoDBlk->collBlock.lcStart.nCol, 0);
		}
	}
}

void CXTPSyntaxEditCtrl::SetAutoCompleteList()
{
	CXTPSyntaxEditLexTokensDefArray arTokens;
	m_pBuffer->GetLexParser()->GetTokensForAutoCompleate(arTokens, FALSE);
	CXTPSyntaxEditACDataArray arrData;

	CXTPSyntaxEditLexTokensDef TokenDef;
	int nTokensDef = (int)arTokens.GetSize();
	if (!nTokensDef)
		return;

	for (int nTokenDef = 0; nTokenDef < nTokensDef; nTokenDef++)
	{
		TokenDef = (arTokens.GetAt(nTokenDef));
		int nTags = (int)TokenDef.m_arTokens.GetSize();
		for (int nTag = 0; nTag < nTags; nTag++)
		{
			CString sDef = TokenDef.m_arTokens.GetAt(nTag);
			arrData.Add(new XTP_EDIT_ACDATA(0, sDef));
			//arrData.Add(new XTP_EDIT_ACDATA(0, TokenDef.m_arTokens.GetAt(nTag)));
		}

		int nDelims = (int)TokenDef.m_arStartSeps.GetSize();
		int nDelim;
		for (nDelim = 0; nDelim < nDelims; nDelim++)
		{
			if (m_pAutoComplete->m_strDelims.Find(TokenDef.m_arStartSeps.GetAt(nDelim)) < 0)
				m_pAutoComplete->m_strDelims += TokenDef.m_arStartSeps.GetAt(nDelim);
		}

		nDelims = (int)TokenDef.m_arEndSeps.GetSize();
		for (nDelim = 0; nDelim < nDelims; nDelim++)
		{
			if (m_pAutoComplete->m_strDelims.Find(TokenDef.m_arEndSeps.GetAt(nDelim)) < 0)
				m_pAutoComplete->m_strDelims += TokenDef.m_arEndSeps.GetAt(nDelim);
		}

	}

	m_pAutoComplete->SetList(arrData);
}

BOOL CXTPSyntaxEditCtrl::PreTranslateMessage(MSG* pMsg)
{
	if (m_pAutoComplete->IsActive())
	{
		switch (pMsg->message)
		{
			case WM_KEYDOWN:
			case WM_KEYUP:
			case WM_CHAR:
			case WM_DEADCHAR:
			{
				m_pAutoComplete->SendMessage(
					pMsg->message, pMsg->wParam, pMsg->lParam);
			}
			break;
		}

		if (pMsg->message == WM_KEYDOWN)
		{
			switch (pMsg->wParam)
			{
				case VK_SPACE:
				case VK_PRIOR:
				case VK_NEXT:
				case VK_END:
				case VK_HOME:
				case VK_LEFT:
				case VK_UP:
				case VK_RIGHT:
				case VK_DOWN:
				case VK_SELECT:
				case VK_PRINT:
				case VK_EXECUTE:
				case VK_SNAPSHOT:
				case VK_INSERT:
				case VK_DELETE:
				case VK_HELP:
				case VK_RETURN:
				{
					return TRUE;
				}

			}
		}
	}
	else if (IsEnabledEditAccelerators() &&
		(pMsg->message == WM_KEYDOWN || pMsg->message == WM_SYSKEYDOWN))
	{
		BOOL bAltKey  = (::GetKeyState(VK_MENU) & KF_UP) != 0;
		BOOL bCtrlKey  = (::GetKeyState(VK_CONTROL) & KF_UP) != 0;
		BOOL bShiftKey  = (::GetKeyState(VK_SHIFT) & KF_UP) != 0;

		BOOL bAlt   = bAltKey && !bCtrlKey && !bShiftKey;
		BOOL bCtrl  = bCtrlKey && !bAltKey && !bShiftKey;
		BOOL bShift = bShiftKey && !bAltKey && !bCtrlKey;
		BOOL bCtrlShift = bCtrlKey && bShiftKey && !bAltKey;

		BOOL bProcessed = TRUE;

		if (bCtrlKey && pMsg->wParam == (BYTE)'X')
		{
			if (IsSelectionExist() && CanEditDoc())
				Cut();
		}
		else if (bCtrl && pMsg->wParam == (BYTE)'C' ||
			bCtrl && pMsg->wParam == VK_INSERT)
		{
			if (IsSelectionExist())
				Copy();
		}
		else if (bCtrl && pMsg->wParam == (BYTE)'V' ||
			bShift && pMsg->wParam == VK_INSERT)
		{
			if (CanEditDoc())
				Paste();
		}
		else if (bAlt && pMsg->wParam == VK_BACK ||
			bCtrl && pMsg->wParam == (BYTE)'Z')
		{
			if (CanUndo())
				Undo();
		}
		else if (bCtrl && pMsg->wParam == (BYTE)'Y' ||
			bCtrlShift && pMsg->wParam == (BYTE)'Z')
		{
			if (CanRedo())
				Redo();
		}
		else if (bCtrl && pMsg->wParam == (BYTE)'A')
		{
			SelectAll();
		}
		else
		{
			bProcessed = FALSE;
		}

		if (bProcessed)
			return TRUE;
	}

	return CWnd::PreTranslateMessage(pMsg);
}

BOOL CXTPSyntaxEditCtrl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if (m_pAutoComplete->IsActive())
	{
		return (BOOL)m_pAutoComplete->SendMessage(WM_MOUSEWHEEL,
						MAKEWPARAM(nFlags, zDelta), MAKELPARAM(pt.x, pt.y));

		//CXTPWindowRect rcWndAC(&m_wndAutoComplete);
		//if (rcWndAC.PtInRect(pt))
		//  return (BOOL)m_pAutoComplete->SendMessage(WM_MOUSEWHEEL, MAKEWPARAM(nFlags, zDelta), MAKELPARAM(pt.x, pt.y));
		//else
		//  m_pAutoComplete->Hide();
	}

	m_pToolTip->Hide();

	if (zDelta < 0)
		Scroll(0, m_nWheelScroll);
	else
		Scroll(0, -m_nWheelScroll);

	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}

void CXTPSyntaxEditCtrl::CollapseAll()
{
	CXTPSyntaxEditLineMarksManager* pMgr = GetLineMarksManager();
	if (!pMgr)
		return;

	CWaitCursor _WC;

	CXTPSyntaxEditRowsBlockArray arCoBlocks;
	GetCollapsableBlocksInfo(-1, arCoBlocks);

	for (int nRow = 0; nRow < GetRowCount(); nRow++)
	{
		XTP_EDIT_LMPARAM LMCoParam;

		int toRemove = -1;

		int nCount = (int)arCoBlocks.GetSize();
		for (int i = 0; i < nCount; i++)
		{
			XTP_EDIT_ROWSBLOCK coBlk = arCoBlocks[i];
			if (coBlk.lcStart.nLine == nRow)
			{
				XTP_EDIT_COLLAPSEDBLOCK* pCoDrawBlk = (XTP_EDIT_COLLAPSEDBLOCK*)LMCoParam.GetPtr();
				if (!pCoDrawBlk)
				{
					pCoDrawBlk = new XTP_EDIT_COLLAPSEDBLOCK;
					if (!pCoDrawBlk)
					{
						return;
					}
					LMCoParam.SetPtr(pCoDrawBlk, XTPSECollapsedBlockDeleteFn);
				}
				pCoDrawBlk->collBlock = coBlk;
				pMgr->SetLineMark(nRow, xtpEditLMT_Collapsed, &LMCoParam);
				m_arCollapsedTextRows.SetAtGrow(m_nCollapsedTextRowsCount, nRow);
				m_nCollapsedTextRowsCount++;
				if (toRemove < 0)
					toRemove = i;
			}
		}
		if (toRemove >= 0)
			arCoBlocks.RemoveAt(0, toRemove+1);
	}

	// refresh picture
	SetCurCaretPos(GetCurrentDocumentRow(), 1);

	UpdateWindow();

	RecalcScrollBars();

	UpdateScrollPos();
}

void CXTPSyntaxEditCtrl::ExpandAll()
{
	CXTPSyntaxEditLineMarksManager* pMgr = GetLineMarksManager();
	if (!pMgr)
		return;

	CWaitCursor _WC;

	pMgr->RemoveAll(xtpEditLMT_Collapsed);

	// refresh picture
	SetCurCaretPos(GetCurrentDocumentRow(), m_nDispCol);

	if (::IsWindow(m_hWnd))
		UpdateWindow();

	RecalcScrollBars();

	UpdateScrollPos();
}

void CXTPSyntaxEditCtrl::GetRowColors(int nTextRow, int nColFrom, int nColTo,
		const XTP_EDIT_COLORVALUES& clrDefault,
		CXTPSyntaxEditTextBlockList* rBlocks)
{
	if (!GetSyntaxColor())
		return;

	CXTPSyntaxEditLexTextSchema* ptrTxtSch = m_pBuffer->GetLexParser()->GetTextSchema();
	if (ptrTxtSch)
	{
		CXTPSyntaxEditLexTextBlock* pScreenSchFirstTB = GetOnScreenSch(nTextRow);
		//CXTPSyntaxEditLexTextBlock* pScreenSchFirstTB = ptrTxtSch->GetBlocks();
		if (pScreenSchFirstTB)
		{
			CXTPSyntaxEditTextIterator txtIter(GetEditBuffer());

			ptrTxtSch->GetRowColors(&txtIter, nTextRow, nColFrom, nColTo,
							clrDefault, rBlocks, NULL, pScreenSchFirstTB);
		}
	}
}

void CXTPSyntaxEditCtrl::GetCollapsableBlocksInfo(int nTextRow,
		CXTPSyntaxEditRowsBlockArray& rarCoBlocks) const
{
	rarCoBlocks.RemoveAll();

	if (GetSyntaxColor())
	{
		CXTPSyntaxEditLexTextSchema* ptrTxtSch = m_pBuffer->GetLexParser()->GetTextSchema();
		if (ptrTxtSch)
			ptrTxtSch->GetCollapsableBlocksInfo(nTextRow, rarCoBlocks, &m_fcCollapsable->ptrTBStart);
	}
}

CXTPSyntaxEditLexTextBlock* CXTPSyntaxEditCtrl::GetOnScreenSch(int nForRow)
{
	int nLifeTime = m_pBuffer->GetLexParser()->GetSchemaOptions(
						m_pBuffer->GetFileExt())->m_dwOnScreenSchCacheLifeTime_sec;
	m_arOnScreenSchCache->RemoveOld(nLifeTime);

	//========================================================================
	int nCount = (int)m_arOnScreenSchCache->GetSize();
	int i;

	for (i = 0; i < nCount; i++)
	{
		CScreenSearchBlock& rSchBlk = m_arOnScreenSchCache->ElementAt(i);

		if (nForRow >= rSchBlk.nRowStart && nForRow <= rSchBlk.nRowEnd)
		{
			rSchBlk.dwLastAccessTime = ::GetTickCount();
			return rSchBlk.ptrTBFirst;
		}
	}

	//---------------------------------------------------------------------------
	CXTPSyntaxEditLexTextSchema* ptrTxtSch = m_pBuffer->GetLexParser()->GetTextSchema();
	if (ptrTxtSch)
	{
		int nRowPerPage = GetRowPerPage()+1;

		CScreenSearchBlock schBlkNew;
		schBlkNew.nRowStart = nForRow;
		schBlkNew.nRowEnd = nForRow + nRowPerPage;

		//--------------------------------------------------------------------
		int nRowStartMin = 1;
		// 1. search next on screen region
		nCount = (int)m_arOnScreenSchCache->GetSize();

		for (i = 0; i < nCount; i++)
		{
			const CScreenSearchBlock& rSchBlk = m_arOnScreenSchCache->ElementAt(i);

			if (rSchBlk.nRowStart > schBlkNew.nRowStart && rSchBlk.nRowStart <= schBlkNew.nRowEnd)
			{
				schBlkNew.nRowEnd = rSchBlk.nRowStart-1;
			}

			if (rSchBlk.nRowEnd+1 > nRowStartMin && rSchBlk.nRowEnd+1 < nForRow)
			{
				nRowStartMin = rSchBlk.nRowEnd+1;
			}
		}
		//--------------------------------------------------------------------
		// 2. search next collapsed block start
		for (i = 0; i < m_nCollapsedTextRowsCount; i++)
		{
			int nCoStartRow = m_arCollapsedTextRows[i];

			if (nCoStartRow >= schBlkNew.nRowStart && nCoStartRow < schBlkNew.nRowEnd)
			{
				schBlkNew.nRowEnd = nCoStartRow;
			}
		}

		//===========================
		if (schBlkNew.nRowEnd - schBlkNew.nRowStart < nRowPerPage/2)
		{
			schBlkNew.nRowStart = max(schBlkNew.nRowStart-nRowPerPage, nRowStartMin);
		}

		//--------------------------------------------------------------------
		CXTPSyntaxEditTextIterator txtIter(m_pBuffer);

		BOOL bParseRes = ptrTxtSch->RunParseOnScreen(&txtIter, schBlkNew.nRowStart,
									schBlkNew.nRowEnd, schBlkNew.ptrTBFirst);
		if (bParseRes)
		{
			schBlkNew.dwLastAccessTime = ::GetTickCount();

			m_arOnScreenSchCache->Add(schBlkNew);
			return schBlkNew.ptrTBFirst;
		}
	}

	return NULL;
}

void CXTPSyntaxEditCtrl::InvalidateRows(int nRowFrom, int nRowTo)
{
	int nCountVDR = (int)m_arValidDispRows.GetSize();

	//-* Invalidate ALL ------------------------------------------------------
	if (nRowFrom < 0 && nRowTo < 0)
	{
		m_arOnScreenSchCache->RemoveAll();

		for (int i = 0; i < nCountVDR; i++) {
			m_arValidDispRows.SetAtGrow(i, 0);
		}
		return ;
	}

	//-* Invalidate Part -----------------------------------------------------
	int nRow1 = nRowFrom;
	int nRow2 = nRowTo;

	if (nRow1 < 0)
	{
		nRow1 = m_nTopRow;
	}
	if (nRow2 < nRow1)
	{
		nRow2 = GetDocumentRow(GetRowPerPage());
	}

	int nDispRow1= CalculateVisibleRow(m_nTopRow, nRow1);
	int nDispRow2 = CalculateVisibleRow(m_nTopRow, nRow2);

	int nI1 = max(min(nDispRow1, 1000), 0);
	int nI2 = max(min(nDispRow2, 1000), nCountVDR-1);
	for (int i = nI1; i <= nI2; i++)
	{
		m_arValidDispRows.SetAtGrow(i, 0);
	}

	//===========================================================================
	ClearOnScreenSchCache(nRowFrom);
}

void CXTPSyntaxEditCtrl::ClearOnScreenSchCache(int nRowFrom)
{
	//-* clear ALL ------------------------------------------------------
	if (nRowFrom < 0)
	{
		m_arOnScreenSchCache->RemoveAll();
		return ;
	}

	//===========================================================================
	int nCountSSC = (int)m_arOnScreenSchCache->GetSize();
	for (int i = nCountSSC-1; i >= 0; i--)
	{
		CScreenSearchBlock& rSchBlk = m_arOnScreenSchCache->ElementAt(i);

		BOOL bRemove = FALSE;

		if (nRowFrom >= rSchBlk.nRowStart && nRowFrom <= rSchBlk.nRowEnd)
		{
			rSchBlk.nRowEnd = nRowFrom-1;
			bRemove = !(rSchBlk.nRowStart <= rSchBlk.nRowEnd);
		}
		else if (nRowFrom < rSchBlk.nRowStart)
		{
			bRemove = TRUE;
		}

		//-------------------------------------
		if (bRemove)
		{
			m_arOnScreenSchCache->RemoveAt(i);
		}
		else
		{
			ASSERT(m_arOnScreenSchCache->ElementAt(i).nRowStart == rSchBlk.nRowStart);
			ASSERT(m_arOnScreenSchCache->ElementAt(i).nRowEnd == rSchBlk.nRowEnd);
		}
	}
}

void CXTPSyntaxEditCtrl::SetRowValid(int nDispRow)
{
	ASSERT(nDispRow < 1000);
	m_arValidDispRows.SetAtGrow(nDispRow, 1);
}

BOOL CXTPSyntaxEditCtrl::IsRowValid(int nDispRow) const
{
	int nCount = (int)m_arValidDispRows.GetSize();

	if (nDispRow > 0 && nDispRow < nCount)
	{
		int nValid = m_arValidDispRows[nDispRow];
		return (nValid != 0);
	}

	return FALSE;

}

const CStringList& CXTPSyntaxEditCtrl::GetUndoTextList()
{
	return m_pBuffer->GetUndoRedoManager()->GetUndoTextList();
}

const CStringList& CXTPSyntaxEditCtrl::GetRedoTextList()
{
	return m_pBuffer->GetUndoRedoManager()->GetRedoTextList();
}

void CXTPSyntaxEditCtrl::OnLexCfgWasChanged(XTP_NOTIFY_CODE code, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam); UNREFERENCED_PARAMETER(lParam);

	if (code == xtpEditAllConfigWasChanged ||
		code == xtpEditClassSchWasChanged ||
		code == xtpEditThemeWasChanged)
	{
		m_arOnScreenSchCache->RemoveAll();

		Invalidate(FALSE);
	}
	else
	{
		ASSERT(FALSE);
	}
}

BOOL CXTPSyntaxEditCtrl::GetRowNodes(int nRow, DWORD& dwType) const
{
	dwType = XTP_EDIT_ROWNODE_NOTHING;

	// retrieve row nodes
	CXTPSyntaxEditRowsBlockArray arCoBlocks;
	GetCollapsableBlocksInfo(nRow, arCoBlocks);

	int nCount = (int)arCoBlocks.GetSize();
	for (int i = 0; i < nCount; i++)
	{
		XTP_EDIT_ROWSBLOCK coBlk = arCoBlocks[i];
		if (coBlk.lcStart.nLine == nRow)
		{
			if (HasRowMark(coBlk.lcStart.nLine, xtpEditLMT_Collapsed))
			{
				dwType |= XTP_EDIT_ROWNODE_COLLAPSED;
			}
			else
			{
				dwType |= XTP_EDIT_ROWNODE_EXPANDED;
			}
		}
		if (coBlk.lcEnd.nLine == nRow)
		{
			dwType |= XTP_EDIT_ROWNODE_ENDMARK;
		}
		if (coBlk.lcStart.nLine < nRow)
		{
			dwType |= XTP_EDIT_ROWNODE_NODEUP;
		}
		if (coBlk.lcEnd.nLine > nRow && (dwType & XTP_EDIT_ROWNODE_COLLAPSED))
		{
			dwType |= XTP_EDIT_ROWNODE_NODEDOWN;
		}
	}

	return (dwType != XTP_EDIT_ROWNODE_NOTHING);
}

BOOL CXTPSyntaxEditCtrl::NotifyMarginLBtnClick(int nRow, int nDispRow) const
{
	// Notify the parent class that position has been changed
	XTP_EDIT_NMHDR_MARGINCLICKED somc;

	// NMHDR codes
	somc.nmhdr.code = XTP_EDIT_NM_MARGINCLICKED;
	somc.nmhdr.hwndFrom = m_hWnd;
	somc.nmhdr.idFrom = GetDlgCtrlID();

	// Row col specific codes
	somc.nRow = nRow;
	somc.nDispRow = nDispRow;

	// Notify the parent window
	if (::IsWindow(m_pParentWnd->GetSafeHwnd()))
	{
		return (BOOL)m_pParentWnd->SendMessage(
			WM_NOTIFY, (WPARAM)somc.nmhdr.idFrom, (LPARAM)&somc);
	}

	return FALSE;
}

UINT CXTPSyntaxEditCtrl::CalcAveDataSize(int nRowStart, int nRowEnd)
{
	UINT uSize = m_nAverageLineLen * abs(nRowStart - nRowEnd);
	uSize = (uSize / 1024 + 1) * 1024;
	return uSize;
}

BOOL CXTPSyntaxEditCtrl::GetWideCaret() const
{
	return m_pOptions->m_bWideCaret && m_pBuffer && m_pBuffer->GetOverwriteFlag();
}

CXTPSyntaxEditSelection& CXTPSyntaxEditCtrl::GetSelection()
{
	return *m_pSelection;
}

CXTPSyntaxEditDrawTextProcessor& CXTPSyntaxEditCtrl::GetDrawTextProcessor()
{
	return *m_pDrawTextProcessor;
}


BOOL CXTPSyntaxEditCtrl::SetWideCaret(BOOL bWideCaret, BOOL bUpdateReg/*=FALSE*/)
{
	if (m_pOptions->m_bWideCaret != bWideCaret)
	{
		if (!SetValueBool(XTP_EDIT_REG_WIDECARET, bWideCaret, m_pOptions->m_bWideCaret, bUpdateReg))
			return FALSE;

		SetCurCaretPos(GetCurrentDocumentRow(), m_nDispCol, FALSE, FALSE);
	}

	return TRUE;
}

void CXTPSyntaxEditCtrl::SetPaintManager(CXTPSyntaxEditPaintManager* pPaintManager)
{
	ASSERT(pPaintManager);
	if (pPaintManager)
	{
		CMDTARGET_RELEASE(m_pPaintManeger);
		m_pPaintManeger = pPaintManager;
	}
}

//===========================================================================
// CXTPSyntaxEditCtrl::CTextSearchCache
//===========================================================================

CXTPSyntaxEditCtrl::CTextSearchCache::CTextSearchCache()
{
	nForTopRow = 0;
}

void CXTPSyntaxEditCtrl::CTextSearchCache::Update(int nCurrTopRow)
{
	if (nCurrTopRow != nForTopRow)
	{
		ptrTBStart = NULL;
		nForTopRow = nCurrTopRow;
	}
}

//===========================================================================
// CXTPSyntaxEditCtrl::CScreenSearchBlock
//===========================================================================

CXTPSyntaxEditCtrl::CScreenSearchBlock::CScreenSearchBlock()
{
	nRowStart = nRowEnd = 0;
	dwLastAccessTime = 0;
}

CXTPSyntaxEditCtrl::CScreenSearchBlock::CScreenSearchBlock(const CScreenSearchBlock& rSrc)
{
	nRowStart = rSrc.nRowStart;
	nRowEnd = rSrc.nRowEnd;
	ptrTBFirst = rSrc.ptrTBFirst;
	dwLastAccessTime = rSrc.dwLastAccessTime;
}

CXTPSyntaxEditCtrl::CScreenSearchBlock::~CScreenSearchBlock()
{

}

//===========================================================================
// CXTPSyntaxEditCtrl::CScreenSearchCache
//===========================================================================

CXTPSyntaxEditCtrl::CScreenSearchCache::CScreenSearchCache()
{
	m_dwLastRemoveOldTime = 0;
}

CXTPSyntaxEditCtrl::CScreenSearchCache::~CScreenSearchCache()
{
	RemoveAll();
}

void CXTPSyntaxEditCtrl::CScreenSearchCache::RemoveAll()
{
	for (int i = 0; i < (int)GetSize(); i++)
	{
		CScreenSearchBlock& rSchBlk = ElementAt(i);
		CXTPSyntaxEditLexTextSchema::Close(rSchBlk.ptrTBFirst);
	}
	Base::RemoveAll();
}

void CXTPSyntaxEditCtrl::CScreenSearchCache::RemoveAt(int nIndex)
{
	CScreenSearchBlock& rSchBlk = ElementAt(nIndex);

	CXTPSyntaxEditLexTextSchema::Close(rSchBlk.ptrTBFirst);
	Base::RemoveAt(nIndex);
}

void CXTPSyntaxEditCtrl::CScreenSearchCache::RemoveOld(int nTimeOut_sec)
{
	int nTimeOut_ms = nTimeOut_sec * 1000;

	DWORD dwTime = ::GetTickCount();

	int cnCheckTimeOut_ms = min(nTimeOut_sec, 10*1000);

	if (nTimeOut_sec == 0 || nTimeOut_sec == -1 ||
		labs(dwTime - m_dwLastRemoveOldTime) < cnCheckTimeOut_ms)
	{
		return;
	}

	//================================================================
	m_dwLastRemoveOldTime = dwTime;
	for (int i = (int)GetSize() - 1; i >= 0; i--)
	{
		CScreenSearchBlock& rSchBlk = ElementAt(i);

		if (labs(dwTime - rSchBlk.dwLastAccessTime) >= nTimeOut_ms)
		{
			CXTPSyntaxEditLexTextSchema::Close(rSchBlk.ptrTBFirst);
			Base::RemoveAt(i);
		}
	}
}

//===========================================================================
// CXTPSyntaxEditCtrl::CAverageVal
//===========================================================================

CXTPSyntaxEditCtrl::CAverageVal::CAverageVal(int nDataSize/* = 100*/)
{
	m_nDataSize = max(1, nDataSize);
	m_nNextIndex = 0;
}

void CXTPSyntaxEditCtrl::CAverageVal::AddValue(UINT uVal)
{
	m_arData.SetAtGrow(m_nNextIndex, uVal);
	m_nNextIndex = (m_nNextIndex+1) % m_nDataSize;
}

UINT CXTPSyntaxEditCtrl::CAverageVal::GetAverageValue(UINT uDefaultIfNoData/* = 0*/)
{
	int nCount = (int)m_arData.GetSize();
	if (nCount <= 0)
		return uDefaultIfNoData;

	UINT uSumm = 0;
	for (int i = 0; i < nCount; i++)
		uSumm += m_arData[i];

	UINT uAveVal = uSumm / nCount;

	return uAveVal;
}

int CXTPSyntaxEditCtrl::GetRowCount() const
{
	return m_pBuffer ? m_pBuffer->GetRowCount() : 0;
}

CXTPSyntaxEditConfigurationManager* CXTPSyntaxEditCtrl::GetLexConfigurationManager() const
{
	return m_pBuffer? m_pBuffer->GetLexConfigurationManager(): NULL;
}

void CXTPSyntaxEditCtrl::ReloadSyntaxConfigFileMode()
{
	CXTPSyntaxEditConfigurationManager* pMan = GetLexConfigurationManager();
	if (pMan)
	{
		pMan->m_bConfigFileMode = TRUE;
		pMan->m_bUseMonitor = TRUE;
		pMan->m_sIniSet = _T("");
	}
	m_bUseMonitor = TRUE;
	m_bConfigFileMode = TRUE;
}

void CXTPSyntaxEditCtrl::ReloadSyntaxStringMode()
{
	if (GetEditBuffer())
	{
		if (m_sPassedIniSet.IsEmpty())
			m_sIniSet = _T("[Schemes]\r\nCPP\r\n[Themes]\r\nDefault\r\nAlternative\r\n");
		else
			m_sIniSet = m_sPassedIniSet;
		CXTPSyntaxEditConfigurationManager* pMan = GetLexConfigurationManager();
		if (pMan)
		{
			pMan->m_bConfigFileMode = FALSE;
			pMan->m_bUseMonitor = FALSE;
			pMan->m_sIniSet = m_sIniSet;
		}
		m_bUseMonitor = FALSE;
		m_bConfigFileMode = FALSE;

		SetConfigFile(_T(""), FALSE);
	}
}

void CXTPSyntaxEditCtrl::SetSyntaxAndColorScheme(CString sSyntaxScheme, CString sColorScheme,
												 BOOL bScheme /*FALSE*/, BOOL bColors /*FALSE*/)
{
	if (!sSyntaxScheme.IsEmpty() && !sColorScheme.IsEmpty())
	{
		if (m_sPassedIniSet.IsEmpty())
			m_sIniSet = _T("[Schemes]\r\nCPP\r\n[Themes]\r\nDefault\r\nAlternative\r\n");
		else
			m_sIniSet = m_sPassedIniSet;
		m_strSyntaxScheme = sSyntaxScheme;
		m_strColorScheme = sColorScheme;

		if (bScheme)
		{
			CXTPSyntaxEditStringDlg dlg;
			dlg.m_Syntax = m_strSyntaxScheme;
			if (dlg.DoModal() == IDOK)
				m_strSyntaxScheme = dlg.m_Syntax;
		}

		if (bColors)
		{
			CXTPSyntaxEditStringDlg dlg;
			dlg.m_Syntax = m_strColorScheme;
			if (dlg.DoModal() == IDOK)
				m_strColorScheme = dlg.m_Syntax;
		}

		m_bUseMonitor = FALSE;
		m_bConfigFileMode = FALSE;
		CXTPSyntaxEditConfigurationManager* pMan = GetLexConfigurationManager();
		if (pMan)
		{
			pMan->p_sSyntaxScheme = &m_strSyntaxScheme;
			pMan->p_sColorScheme = &m_strColorScheme;

			pMan->m_bConfigFileMode = FALSE;
			pMan->m_bUseMonitor = FALSE;
			pMan->m_sIniSet = m_sIniSet;

			SetConfigFile(_T(""), FALSE);
		}
	}
}

#ifndef IDC_SYNTAX_EDIT
#define IDC_SYNTAX_EDIT                 1009
#endif
// CXTPSyntaxEditStringDlg dialog

IMPLEMENT_DYNAMIC(CXTPSyntaxEditStringDlg, CDialog)

CXTPSyntaxEditStringDlg::CXTPSyntaxEditStringDlg(CWnd* pParent) : CDialog()
{
	UNREFERENCED_PARAMETER(pParent);
}

CXTPSyntaxEditStringDlg::~CXTPSyntaxEditStringDlg()
{
}

void CXTPSyntaxEditStringDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SYNTAX_EDIT, m_edit);
}


BEGIN_MESSAGE_MAP(CXTPSyntaxEditStringDlg, CDialog)
	ON_WM_SIZE()
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
END_MESSAGE_MAP()

void CXTPSyntaxEditStringDlg::OnOK()
{
	 m_edit.GetWindowText(m_Syntax);

	CDialog::OnOK();
}

BOOL CXTPSyntaxEditStringDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_edit.SetWindowText(m_Syntax);

	return TRUE;
}

void CXTPSyntaxEditStringDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if (GetDlgItem(IDC_SYNTAX_EDIT))
	{
		int H = 35;
		CRect rc;
		CWnd* pWnd = GetDlgItem(IDC_SYNTAX_EDIT);
		pWnd->MoveWindow(0, 0, cx, cy - H - 5);

		pWnd = GetDlgItem(ID_EDIT_CUT);
		pWnd->GetWindowRect(&rc);
		ScreenToClient(&rc);
		pWnd->MoveWindow(rc.left, cy - H, rc.Width(), rc.Height());

		pWnd = GetDlgItem(ID_EDIT_COPY);
		pWnd->GetWindowRect(&rc);
		ScreenToClient(&rc);
		pWnd->MoveWindow(rc.left, cy - H, rc.Width(), rc.Height());

		pWnd = GetDlgItem(ID_EDIT_PASTE);
		pWnd->GetWindowRect(&rc);
		ScreenToClient(&rc);
		pWnd->MoveWindow(rc.left, cy - H, rc.Width(), rc.Height());

		pWnd = GetDlgItem(ID_EDIT_UNDO);
		pWnd->GetWindowRect(&rc);
		ScreenToClient(&rc);
		pWnd->MoveWindow(rc.left, cy - H, rc.Width(), rc.Height());

		pWnd = GetDlgItem(IDOK);
		pWnd->GetWindowRect(&rc);
		ScreenToClient(&rc);
		pWnd->MoveWindow(rc.left, cy - H, rc.Width(), rc.Height());

		pWnd = GetDlgItem(IDCANCEL);
		pWnd->GetWindowRect(&rc);
		ScreenToClient(&rc);
		pWnd->MoveWindow(rc.left, cy - H, rc.Width(), rc.Height());
	}
}

INT_PTR CXTPSyntaxEditStringDlg::DoModal()
{
	BYTE DlgTempl[]=
	{
		0X01, 0X00, 0XFF, 0XFF, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X48, 0X00, 0XCC, 0X80,
		0X07, 0X00, 0X00, 0X00, 0X00, 0X00, 0X78, 0X01, 0X2B, 0X01, 0X00, 0X00, 0X00, 0X00, 0X43, 0X00,
		0X6F, 0X00, 0X6E, 0X00, 0X74, 0X00, 0X65, 0X00, 0X6E, 0X00, 0X74, 0X00, 0X20, 0X00, 0X65, 0X00,
		0X64, 0X00, 0X69, 0X00, 0X74, 0X00, 0X6F, 0X00, 0X72, 0X00, 0X00, 0X00, 0X08, 0X00, 0X90, 0X01,
		0X00, 0X01, 0X4D, 0X00, 0X53, 0X00, 0X20, 0X00, 0X53, 0X00, 0X68, 0X00, 0X65, 0X00, 0X6C, 0X00,
		0X6C, 0X00, 0X20, 0X00, 0X44, 0X00, 0X6C, 0X00, 0X67, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00,
		0X00, 0X00, 0X00, 0X00, 0X01, 0X00, 0X01, 0X50, 0X01, 0X01, 0X18, 0X01, 0X32, 0X00, 0X0E, 0X00,
		0X01, 0X00, 0X00, 0X00, 0XFF, 0XFF, 0X80, 0X00, 0X4F, 0X00, 0X4B, 0X00, 0X00, 0X00, 0X00, 0X00,
		0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X01, 0X50, 0X40, 0X01, 0X18, 0X01,
		0X32, 0X00, 0X0E, 0X00, 0X02, 0X00, 0X00, 0X00, 0XFF, 0XFF, 0X80, 0X00, 0X43, 0X00, 0X61, 0X00,
		0X6E, 0X00, 0X63, 0X00, 0X65, 0X00, 0X6C, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00,
		0X00, 0X00, 0X00, 0X00, 0X84, 0X10, 0XB1, 0X50, 0X00, 0X00, 0X01, 0X00, 0X77, 0X01, 0X11, 0X01,
		0XF1, 0X03, 0X00, 0X00, 0XFF, 0XFF, 0X81, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00,
		0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X01, 0X50, 0X05, 0X00, 0X18, 0X01, 0X32, 0X00, 0X0E, 0X00,
		0X23, 0XE1, 0X00, 0X00, 0XFF, 0XFF, 0X80, 0X00, 0X43, 0X00, 0X75, 0X00, 0X74, 0X00, 0X00, 0X00,
		0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X01, 0X50,
		0X44, 0X00, 0X18, 0X01, 0X32, 0X00, 0X0E, 0X00, 0X22, 0XE1, 0X00, 0X00, 0XFF, 0XFF, 0X80, 0X00,
		0X43, 0X00, 0X6F, 0X00, 0X70, 0X00, 0X79, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00,
		0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X01, 0X50, 0X83, 0X00, 0X18, 0X01, 0X32, 0X00, 0X0E, 0X00,
		0X25, 0XE1, 0X00, 0X00, 0XFF, 0XFF, 0X80, 0X00, 0X50, 0X00, 0X61, 0X00, 0X73, 0X00, 0X74, 0X00,
		0X65, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00,
		0X00, 0X00, 0X01, 0X50, 0XC2, 0X00, 0X18, 0X01, 0X32, 0X00, 0X0E, 0X00, 0X2B, 0XE1, 0X00, 0X00,
		0XFF, 0XFF, 0X80, 0X00, 0X55, 0X00, 0X6E, 0X00, 0X64, 0X00, 0X6F, 0X00, 0X00, 0X00, 0X00, 0X00};
	InitModalIndirect((LPCDLGTEMPLATE)&DlgTempl, NULL);

	return CDialog::DoModal();
}

void CXTPSyntaxEditStringDlg::OnEditCopy()
{
	m_edit.Copy();
}

void CXTPSyntaxEditStringDlg::OnEditCut()
{
	m_edit.Cut();
}

void CXTPSyntaxEditStringDlg::OnEditClear()
{
	m_edit.Clear();
}

void CXTPSyntaxEditStringDlg::OnEditPaste()
{
	m_edit.Paste();
}

void CXTPSyntaxEditStringDlg::OnEditSelectAll()
{
	m_edit.SetSel(0, -1);
}

void CXTPSyntaxEditStringDlg::OnEditUndo()
{
	m_edit.Undo();
}
