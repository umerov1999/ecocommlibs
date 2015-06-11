// XTPSyntaxEditPaintManager.cpp : implementation of the CXTPSyntaxEditPaintManager class.
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
#include <math.h>

// common includes
#include "Common/XTPVC80Helpers.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPResourceManager.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPSmartPtrInternalT.h"


// syntax editor includes
#include "XTPSyntaxEditDefines.h"
#include "XTPSyntaxEditStruct.h"
#include "XTPSyntaxEditLineMarksManager.h"
#include "XTPSyntaxEditLexPtrs.h"
#include "XTPSyntaxEditTextIterator.h"
#include "XTPSyntaxEditLexParser.h"
#include "XTPSyntaxEditCtrl.h"
#include "XTPSyntaxEditPaintManager.h"
#include "XTPSyntaxEditBufferManager.h"
#include "XTPSyntaxEditDrawTextProcessor.h"
#include "XTPSyntaxEditSelection.h"


using namespace XTPSyntaxEditLexAnalyser;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//===========================================================================
// CXTPSyntaxEditPaintManager class
//===========================================================================
IMPLEMENT_DYNAMIC(CXTPSyntaxEditPaintManager, CCmdTarget)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXTPSyntaxEditPaintManager::CXTPSyntaxEditPaintManager()
{

	m_hCurLine = XTPResourceManager()->LoadCursor(XTP_IDC_EDIT_BACKARROW);
	m_hCurMove = XTPResourceManager()->LoadCursor(XTP_IDC_EDIT_MOVE);
	m_hCurCopy = XTPResourceManager()->LoadCursor(XTP_IDC_EDIT_COPY);
	m_hCurIBeam = AfxGetApp()->LoadStandardCursor(IDC_IBEAM);
	m_hCurArrow = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
	m_hCurNO = AfxGetApp()->LoadStandardCursor(IDC_NO);

	// get non-client metrics info.
	NONCLIENTMETRICS ncm;
	ncm.cbSize = sizeof(NONCLIENTMETRICS);
	::SystemParametersInfo(SPI_GETNONCLIENTMETRICS, 0, &ncm, 0);

	// create the tooltip font.
	VERIFY(SetFontToolTip(&ncm.lfStatusFont));

	// construct default fonts.
	VERIFY(CreateFontIndirect(NULL, FALSE));

	// Initialize metrics
	RefreshMetrics();
	InitBitmaps();

	m_sLineNumberFormat = _T("%d");
	//m_sLineNumberFormat = _T("%02d");
	//m_sLineNumberFormat = _T("%2d"); //Good default for practical cases
#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif

}

CXTPSyntaxEditPaintManager::~CXTPSyntaxEditPaintManager()
{

}

void CXTPSyntaxEditPaintManager::InitBitmaps()
{
	// Check for already initialized bitmaps
	m_ilBookmark.DeleteImageList();

	// Load bookmarks bitmap
	CBitmap bmpBookMarks;       // Regular bookmark image

	if (XTPResourceManager()->LoadBitmap(& bmpBookMarks, XTP_IDB_EDIT_BOOKMARKS))
	{
		BITMAP info;
		bmpBookMarks.GetBitmap(&info);

		if (m_ilBookmark.Create(20, info.bmHeight,
			ILC_COLOR24|ILC_MASK, 0, 1))
		{
			m_ilBookmark.Add(&bmpBookMarks, RGB(0x00, 0xff, 0x00));
		}
	}
}

HCURSOR CXTPSyntaxEditPaintManager::SetMoveCursor(HCURSOR hCurMove)
{
	HCURSOR hCurMoveOld = m_hCurMove;
	m_hCurMove = hCurMove;
	return hCurMoveOld;
}

HCURSOR CXTPSyntaxEditPaintManager::SetCopyCursor(HCURSOR hCurCopy)
{
	HCURSOR hCurCopyOld = m_hCurCopy;
	m_hCurCopy = hCurCopy;
	return hCurCopyOld;
}

HCURSOR CXTPSyntaxEditPaintManager::SetLineSelCursor(HCURSOR hCur)
{
	HCURSOR hCurOld = m_hCurLine;
	m_hCurLine = hCur;
	return hCurOld;
}

AFX_STATIC void GetCustomValue(CWinApp* pWinApp, LPCTSTR lpszRegKey, CXTPPaintManagerColor& pmc)
{
	COLORREF color = pWinApp->GetProfileInt(XTP_EDIT_REG_SETTINGS,
		lpszRegKey, pmc.GetStandardColor());

	if (color != pmc.GetStandardColor())
	{
		pmc = color;
	}
}

void CXTPSyntaxEditPaintManager::RefreshMetrics()
{
	// Initialize default colors.
	m_clrValues.crText.SetStandardValue(::GetSysColor(COLOR_WINDOWTEXT));
	m_clrValues.crBack.SetStandardValue(::GetSysColor(COLOR_WINDOW));
	m_clrValues.crReadOnlyBack.SetStandardValue(::GetSysColor(COLOR_3DFACE));
	m_clrValues.crHiliteText.SetStandardValue(::GetSysColor(COLOR_HIGHLIGHTTEXT));
	m_clrValues.crHiliteBack.SetStandardValue(::GetSysColor(COLOR_HIGHLIGHT));
	m_clrValues.crInactiveHiliteText.SetStandardValue(::GetSysColor(COLOR_INACTIVECAPTIONTEXT));
	m_clrValues.crInactiveHiliteBack.SetStandardValue(::GetSysColor(COLOR_INACTIVECAPTION));
	m_clrValues.crLineNumberText.SetStandardValue(RGB(0x00, 0x82, 0x84));
	m_clrValues.crLineNumberBack.SetStandardValue(::GetSysColor(COLOR_WINDOW));

	// Restore user settings from registry.
	CWinApp* pWinApp = AfxGetApp();
	if (pWinApp != NULL)
	{
		GetCustomValue(pWinApp, XTP_EDIT_REG_TEXTCOLOR, m_clrValues.crText);
		GetCustomValue(pWinApp, XTP_EDIT_REG_BACKCOLOR, m_clrValues.crBack);
		GetCustomValue(pWinApp, XTP_EDIT_REG_SELTEXTCOLOR, m_clrValues.crHiliteText);
		GetCustomValue(pWinApp, XTP_EDIT_REG_SELBACKCOLOR, m_clrValues.crHiliteBack);
		GetCustomValue(pWinApp, XTP_EDIT_REG_INSELTEXTCOLOR, m_clrValues.crInactiveHiliteText);
		GetCustomValue(pWinApp, XTP_EDIT_REG_INSELBACKCOLOR, m_clrValues.crInactiveHiliteBack);
		GetCustomValue(pWinApp, XTP_EDIT_REG_LINENUMTEXTCOLOR, m_clrValues.crLineNumberText);
		GetCustomValue(pWinApp, XTP_EDIT_REG_LINENUMBACKCOLOR, m_clrValues.crLineNumberBack);

		UINT nSize = 0;
		LOGFONT *pLogFont;
		if (pWinApp->GetProfileBinary(XTP_EDIT_REG_SETTINGS, XTP_EDIT_REG_LOGFONT, (LPBYTE*)&pLogFont, &nSize))
		{
			CreateFontIndirect(pLogFont);
			delete [] pLogFont;
		}
	}

	// Initialize gray pen.
	if (m_penGray.m_hObject)
		m_penGray.DeleteObject();
	m_penGray.CreatePen(PS_SOLID, 1, ::GetSysColor(COLOR_3DSHADOW));
}

//////////////////////////////////////////////////////////////////////////

CFont* CXTPSyntaxEditPaintManager::GetFont()
{
	return &m_font;
}

BOOL CXTPSyntaxEditPaintManager::CreateFontIndirect(LPLOGFONT pLogfont, BOOL bUpdateReg/*=FALSE*/)
{
	LOGFONT lfDefault;

// construct default fonts.
	if (!pLogfont)
	{
#ifdef XTP_FIXED
// change default font  "Courier New" => OEM_FIXED_FONT
		HFONT hFont = (HFONT)::GetStockObject(OEM_FIXED_FONT);
		CFont font;
		font.Attach(hFont);
		font.GetLogFont(&lfDefault);
		font.Detach();
#else
		::ZeroMemory(&lfDefault, sizeof(LOGFONT));

		lfDefault.lfCharSet = XTPResourceManager()->GetFontCharset();
		lfDefault.lfWeight = FW_NORMAL;
		lfDefault.lfHeight = -13;
		STRCPY_S(lfDefault.lfFaceName, LF_FACESIZE, _T("Courier New"));
#endif
		pLogfont = &lfDefault;
	}

	if (!SetFont(pLogfont))
		return FALSE;

	if (!SetFontText(pLogfont))
		return FALSE;

	if (!SetFontLineNumber(pLogfont))
		return FALSE;

	if (bUpdateReg)
	{
		CWinApp* pWinApp = AfxGetApp();
		if (pWinApp != NULL)
		{
			if (pWinApp->WriteProfileBinary(XTP_EDIT_REG_SETTINGS, XTP_EDIT_REG_LOGFONT,
				reinterpret_cast<LPBYTE>(pLogfont), sizeof(LOGFONT)))
			{
				return TRUE;
			}
		}
		return FALSE;
	}

	return TRUE;
}

BOOL CXTPSyntaxEditPaintManager::SetFont(LPLOGFONT pLogfont)
{
	if (m_font.m_hObject)
	{
		m_font.DeleteObject();
	}
	return m_font.CreateFontIndirect(pLogfont);
}

CFont* CXTPSyntaxEditPaintManager::GetFontText()
{
	return &m_fontText;
}

BOOL CXTPSyntaxEditPaintManager::SetFontText(LPLOGFONT pLogfont)
{
	if (m_fontText.m_hObject)
	{
		m_fontText.DeleteObject();
	}
	return m_fontText.CreateFontIndirect(pLogfont);
}

CFont* CXTPSyntaxEditPaintManager::GetFontLineNumber()
{
	return &m_fontLineNumber;
}

BOOL CXTPSyntaxEditPaintManager::SetFontLineNumber(LPLOGFONT pLogfont)
{
	if (m_fontLineNumber.m_hObject)
	{
		m_fontLineNumber.DeleteObject();
	}
	return m_fontLineNumber.CreateFontIndirect(pLogfont);
}

CFont* CXTPSyntaxEditPaintManager::GetFontToolTip()
{
	return &m_fontToolTip;
}

BOOL CXTPSyntaxEditPaintManager::SetFontToolTip(LPLOGFONT pLogfont)
{
	if (m_fontToolTip.m_hObject)
	{
		m_fontToolTip.DeleteObject();
	}
	return m_fontToolTip.CreateFontIndirect(pLogfont);
}

//////////////////////////////////////////////////////////////////////////

HCURSOR CXTPSyntaxEditPaintManager::GetCurLine()
{
	return m_hCurLine;
}

HCURSOR CXTPSyntaxEditPaintManager::GetCurMove()
{
	return m_hCurMove;
}

HCURSOR CXTPSyntaxEditPaintManager::GetCurCopy()
{
	return m_hCurCopy;
}

HCURSOR CXTPSyntaxEditPaintManager::GetCurNO()
{
	return m_hCurNO;
}

HCURSOR CXTPSyntaxEditPaintManager::GetCurIBeam()
{
	return m_hCurIBeam;
}

HCURSOR CXTPSyntaxEditPaintManager::GetCurArrow()
{
	return m_hCurArrow;
}

BOOL CXTPSyntaxEditPaintManager::SetTextColor(COLORREF color, BOOL bUpdateReg/*=FALSE*/)
{
	m_clrValues.crText = color;

	if (bUpdateReg)
	{
		CWinApp* pWinApp = AfxGetApp();
		if (pWinApp != NULL)
		{
			if (pWinApp->WriteProfileInt(XTP_EDIT_REG_SETTINGS, XTP_EDIT_REG_TEXTCOLOR, (int)color))
				return TRUE;
		}
		return FALSE;
	}

	return TRUE;
}

BOOL CXTPSyntaxEditPaintManager::SetBackColor(COLORREF color, BOOL bUpdateReg/*=FALSE*/)
{
	m_clrValues.crBack = color;

	if (bUpdateReg)
	{
		CWinApp* pWinApp = AfxGetApp();
		if (pWinApp != NULL)
		{
			if (pWinApp->WriteProfileInt(XTP_EDIT_REG_SETTINGS, XTP_EDIT_REG_BACKCOLOR, (int)color))
				return TRUE;
		}
		return FALSE;
	}

	return TRUE;
}

BOOL CXTPSyntaxEditPaintManager::SetReadOnlyBackColor(COLORREF color, BOOL bUpdateReg/*=FALSE*/)
{
	m_clrValues.crReadOnlyBack = color;

	if (bUpdateReg)
	{
		CWinApp* pWinApp = AfxGetApp();
		if (pWinApp != NULL)
		{
			if (pWinApp->WriteProfileInt(XTP_EDIT_REG_SETTINGS, XTP_EDIT_REG_READONLYBACKCOLOR, (int)color))
				return TRUE;
		}
		return FALSE;
	}

	return TRUE;
}


BOOL CXTPSyntaxEditPaintManager::SetHiliteTextColor(COLORREF color, BOOL bUpdateReg/*=FALSE*/)
{
	m_clrValues.crHiliteText = color;

	if (bUpdateReg)
	{
		CWinApp* pWinApp = AfxGetApp();
		if (pWinApp != NULL)
		{
			if (pWinApp->WriteProfileInt(XTP_EDIT_REG_SETTINGS, XTP_EDIT_REG_SELTEXTCOLOR, (int)color))
				return TRUE;
		}
		return FALSE;
	}

	return TRUE;
}

BOOL CXTPSyntaxEditPaintManager::SetHiliteBackColor(COLORREF color, BOOL bUpdateReg/*=FALSE*/)
{
	m_clrValues.crHiliteBack = color;

	if (bUpdateReg)
	{
		CWinApp* pWinApp = AfxGetApp();
		if (pWinApp != NULL)
		{
			if (pWinApp->WriteProfileInt(XTP_EDIT_REG_SETTINGS, XTP_EDIT_REG_SELBACKCOLOR, (int)color))
				return TRUE;
		}
		return FALSE;
	}

	return TRUE;
}

BOOL CXTPSyntaxEditPaintManager::SetInactiveHiliteTextColor(COLORREF color, BOOL bUpdateReg/*=FALSE*/)
{
	m_clrValues.crInactiveHiliteText = color;

	if (bUpdateReg)
	{
		CWinApp* pWinApp = AfxGetApp();
		if (pWinApp != NULL)
		{
			if (pWinApp->WriteProfileInt(XTP_EDIT_REG_SETTINGS, XTP_EDIT_REG_INSELTEXTCOLOR, (int)color))
				return TRUE;
		}
		return FALSE;
	}

	return TRUE;
}

BOOL CXTPSyntaxEditPaintManager::SetInactiveHiliteBackColor(COLORREF color, BOOL bUpdateReg/*=FALSE*/)
{
	m_clrValues.crInactiveHiliteBack = color;

	if (bUpdateReg)
	{
		CWinApp* pWinApp = AfxGetApp();
		if (pWinApp != NULL)
		{
			if (pWinApp->WriteProfileInt(XTP_EDIT_REG_SETTINGS, XTP_EDIT_REG_INSELBACKCOLOR, (int)color))
				return TRUE;
		}
		return FALSE;
	}

	return TRUE;
}

BOOL CXTPSyntaxEditPaintManager::SetLineNumberTextColor(COLORREF color, BOOL bUpdateReg/*=FALSE*/)
{
	m_clrValues.crLineNumberText = color;

	if (bUpdateReg)
	{
		CWinApp* pWinApp = AfxGetApp();
		if (pWinApp != NULL)
		{
			if (pWinApp->WriteProfileInt(XTP_EDIT_REG_SETTINGS, XTP_EDIT_REG_LINENUMTEXTCOLOR, (int)color))
				return TRUE;
		}
		return FALSE;
	}

	return TRUE;
}

BOOL CXTPSyntaxEditPaintManager::SetLineNumberBackColor(COLORREF color, BOOL bUpdateReg/*=FALSE*/)
{
	m_clrValues.crLineNumberBack = color;

	if (bUpdateReg)
	{
		CWinApp* pWinApp = AfxGetApp();
		if (pWinApp != NULL)
		{
			if (pWinApp->WriteProfileInt(XTP_EDIT_REG_SETTINGS, XTP_EDIT_REG_LINENUMBACKCOLOR, (int)color))
				return TRUE;
		}
		return FALSE;
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
CImageList* CXTPSyntaxEditPaintManager::GetBookmarks()
{
	return &m_ilBookmark;
}

//////////////////////////////////////////////////////////////////////////

BOOL CXTPSyntaxEditPaintManager::UpdateTextFont(CXTPSyntaxEditCtrl* pEditCtrl, const XTP_EDIT_FONTOPTIONS& lf)
{
	LOGFONT lfOpt;
	m_font.GetLogFont(&lfOpt);

	if (pEditCtrl && pEditCtrl->GetSyntaxColor())
	{
		if (lf.lfItalic != XTP_EDIT_FONTOPTIONS_UNSPEC_OPTION)
		{
			lfOpt.lfItalic = lf.lfItalic;
		}
		if (lf.lfWeight != XTP_EDIT_FONTOPTIONS_UNSPEC_OPTION)
		{
			lfOpt.lfWeight = lf.lfWeight;
		}
		if (lf.lfUnderline != XTP_EDIT_FONTOPTIONS_UNSPEC_OPTION)
		{
			lfOpt.lfUnderline = lf.lfUnderline;
		}
	}

	return SetFontText(&lfOpt);
}

void CXTPSyntaxEditPaintManager::DrawLineNumber(CDC *pDC, const CRect& rcRect, int nTextRow, CXTPSyntaxEditCtrl* pEditCtrl)
{
	if (!pEditCtrl)
		return;

	if (nTextRow < 1)
		return;

	if (!pDC->IsPrinting())
	{
		COLORREF clrBk = m_clrValues.crLineNumberBack;
		if (m_clrValues.crLineNumberBack.IsStandardValue()
			&& pEditCtrl->IsReadOnly()
			&& !pEditCtrl->IsViewOnly())
			clrBk = m_clrValues.crReadOnlyBack;

		pDC->FillSolidRect(&rcRect, clrBk);
	}

	CXTPFontDC fontDC(pDC, GetFontLineNumber(), m_clrValues.crLineNumberText);

	CRect rcLineNum(rcRect);
	rcLineNum.right -= 2;

	if (nTextRow <= pEditCtrl->GetRowCount())
	{
		// Print line number
		CString strLineNum;
		CString sLineNumberFormat(m_sLineNumberFormat);
		if (!m_sLineNumberFormat.IsEmpty())
		{
			sLineNumberFormat.Replace(_T("%"), _T(""));
			sLineNumberFormat.Replace(_T("d"), _T(""));
			int N = _ttoi(sLineNumberFormat);
			if (N > 0 && nTextRow < pow(10.0, N))
				strLineNum.Format(m_sLineNumberFormat, nTextRow);
			else
				strLineNum.Format(_T("%d"), nTextRow);
		}
		else
			strLineNum.Format(_T("%d"), nTextRow);

		pDC->DrawText(strLineNum, &rcLineNum, DT_VCENTER|DT_RIGHT);
	}

	if (!pDC->IsPrinting())
		DrawLineNumbersBorder(pDC, rcLineNum, m_clrValues.crLineNumberText);
}

void CXTPSyntaxEditPaintManager::DrawLineNumbersBorder(CDC *pDC, const CRect& rcLineNum, const COLORREF clrBorder)
{
	for (int y = rcLineNum.top; y < rcLineNum.bottom; y++)
	{
		if (y % 2)
			pDC->SetPixelV(rcLineNum.right + 1, y, clrBorder);
	}
}

void CXTPSyntaxEditPaintManager::DrawLineNodeBackground(CDC *pDC, const CRect& rcNodeFull,
			DWORD dwType, int nTextRow, CXTPSyntaxEditCtrl* pEditCtrl)
{
	UNREFERENCED_PARAMETER(dwType); UNREFERENCED_PARAMETER(nTextRow);

	pDC->FillSolidRect(&rcNodeFull, m_clrValues.GetBackColorEx(pEditCtrl));
}

void CXTPSyntaxEditPaintManager::DrawLineNode(CDC *pDC, const CRect& rcNode, const CRect& rcNodeFull,
			DWORD dwType, int nTextRow, CXTPSyntaxEditCtrl* pEditCtrl)
{
	if (!pEditCtrl || !pEditCtrl->GetCollapsibleNodes())
		return;

	// fill node background
	DrawLineNodeBackground(pDC, rcNodeFull, dwType, nTextRow, pEditCtrl);

	// draw node picture
	if (dwType != XTP_EDIT_ROWNODE_NOTHING)
	{
		CXTPPenDC penGray(pDC, &m_penGray);

		if (dwType & XTP_EDIT_ROWNODE_NODEUP)
		{
			// draw up line
			CPoint ptEnd(rcNode.CenterPoint().x, rcNodeFull.top);
			pDC->MoveTo(ptEnd);
			ptEnd.y += rcNodeFull.Height() / 2;
			pDC->LineTo(ptEnd);
		}

		if (dwType & XTP_EDIT_ROWNODE_NODEDOWN)
		{
			// draw down line
			CPoint ptEnd(rcNode.CenterPoint().x, rcNodeFull.bottom);
			pDC->MoveTo(ptEnd);
			ptEnd.y -= rcNodeFull.Height() / 2 + 2;
			pDC->LineTo(ptEnd);
		}

		if (dwType & XTP_EDIT_ROWNODE_ENDMARK)
		{
			// draw down line
			CPoint ptEnd(rcNode.CenterPoint());
			pDC->MoveTo(ptEnd);
			ptEnd.x += rcNodeFull.Width() / 2;
			pDC->LineTo(ptEnd);
		}

		if (dwType & XTP_EDIT_ROWNODE_COLLAPSED ||
			dwType & XTP_EDIT_ROWNODE_EXPANDED)
		{
			// draw collapsed sign
			pDC->Rectangle(&rcNode);
			CPoint ptSign(rcNode.CenterPoint());
			ptSign.x -= 2;
			pDC->MoveTo(ptSign);
			ptSign.x += 5;
			pDC->LineTo(ptSign);
		}

		if (dwType & XTP_EDIT_ROWNODE_COLLAPSED)
		{
			// draw vertical line of the expanded sign
			CPoint ptSign(rcNode.CenterPoint());
			ptSign.y -= 2;
			pDC->MoveTo(ptSign);
			ptSign.y += 5;
			pDC->LineTo(ptSign);

		}
	}
}

void CXTPSyntaxEditPaintManager::DrawCollapsedTextMarks(CXTPSyntaxEditCtrl* pEditCtrl, CDC *pDC)
{
	COLORREF clrFrame = GetSysColor(COLOR_BTNSHADOW);
	COLORREF crText = GetSysColor(COLOR_BTNSHADOW);

	CXTPFontDC fontDC(pDC, GetFont());

	const XTP_EDIT_COLLAPSEDBLOCK* pCoDrawBlk = NULL;
	XTP_EDIT_LMPARAM LMCoParam;
	int nActualRow = 0;

	for (int i = 0; i < pEditCtrl->m_nCollapsedTextRowsCount; i++)
	{
		// get next collapsed row
		int nRow = pEditCtrl->m_arCollapsedTextRows[i];

		if (nRow <= nActualRow)
		{
			continue;
		}

		if (!pEditCtrl->HasRowMark(nRow, xtpEditLMT_Collapsed, &LMCoParam))
		{
			continue;
		}

		// get count of collapsed rows under this row
		int nHiddenRows = 0;
		if (!pEditCtrl->GetCollapsedBlockLen(nRow, nHiddenRows))
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

		CString strText = pCoDrawBlk->collBlock.strCollapsedText; // "[..]"
		CRect rcFrame(pCoDrawBlk->rcCollMark);
		rcFrame.InflateRect(0, 0, 0, 1);

		CRect rcText = pCoDrawBlk->rcCollMark;
		rcText.DeflateRect(1, 0, 1, 0);

		COLORREF clrBack = (pEditCtrl->IsReadOnly() && !pEditCtrl->IsViewOnly())
			? (COLORREF)m_clrValues.crReadOnlyBack
			: (COLORREF)m_clrValues.crBack;
		pDC->FillSolidRect(&rcText, clrBack);

		pDC->SetTextColor(crText);
		pDC->SetBkColor(clrBack);

		pDC->DrawText(strText, &rcText, 0);

		pDC->Draw3dRect(&rcFrame, clrFrame, clrFrame);
	}
}

void CXTPSyntaxEditPaintManager::DrawLineCalcSel(const XTP_EDIT_TEXTBLOCK& txtBlk,
									  int nSelStartX, int nSelEndX,
									  XTP_EDIT_TEXTBLOCK (&arTxtBlk)[4], BOOL (&bUseBlk)[4] )
{
	ASSERT(nSelStartX < nSelEndX);

	if (txtBlk.nPos < nSelStartX && txtBlk.nNextBlockPos-1 >= nSelStartX &&
		txtBlk.nNextBlockPos <= nSelEndX)
	{
		arTxtBlk[1] = arTxtBlk[2] = txtBlk;

		arTxtBlk[1].nNextBlockPos = nSelStartX;
		arTxtBlk[2].nPos = nSelStartX;

		bUseBlk[1] = bUseBlk[2] = TRUE;
	}
	else if (txtBlk.nPos >= nSelStartX && txtBlk.nPos < nSelEndX &&
		txtBlk.nNextBlockPos > nSelEndX)
	{
		arTxtBlk[2] = arTxtBlk[3] = txtBlk;

		arTxtBlk[2].nNextBlockPos = nSelEndX;
		arTxtBlk[3].nPos = nSelEndX;

		bUseBlk[2] = bUseBlk[3] = TRUE;
	}
	else if (txtBlk.nPos >= nSelStartX && txtBlk.nNextBlockPos <= nSelEndX)
	{
		arTxtBlk[2] = txtBlk;
		bUseBlk[2] = TRUE;
	}
	else if (txtBlk.nPos < nSelStartX && txtBlk.nNextBlockPos > nSelEndX)
	{
		arTxtBlk[1] = arTxtBlk[2] = arTxtBlk[3] = txtBlk;

		arTxtBlk[1].nNextBlockPos = nSelStartX;
		arTxtBlk[2].nPos = nSelStartX;
		arTxtBlk[2].nNextBlockPos = nSelEndX;
		arTxtBlk[3].nPos = nSelEndX;

		bUseBlk[1] = bUseBlk[2] = bUseBlk[3] = TRUE;
	}
	else
	{
		ASSERT(txtBlk.nPos >= nSelEndX || txtBlk.nNextBlockPos <= nSelStartX);

		arTxtBlk[1] = txtBlk;
		bUseBlk[1] = TRUE;
	}
}

void CXTPSyntaxEditPaintManager::DrawLineMark(CXTPSyntaxEditCtrl* pEditCtrl, XTP_EDIT_SENMBOOKMARK* pBookmark)
{
	enum { bookMark, bookMarkSel, breakpoint };

	POINT ptStart;
	ptStart.x = pBookmark->rcBookmark.left;
	ptStart.y = pBookmark->rcBookmark.top;

	SIZE szRect;
	szRect.cx = pBookmark->rcBookmark.right - pBookmark->rcBookmark.left;
	szRect.cy = pBookmark->rcBookmark.bottom - pBookmark->rcBookmark.top;

	CDC *pDC = CDC::FromHandle(pBookmark->hDC);

	IMAGEINFO bmpInfo;
	ZeroMemory(&bmpInfo, sizeof(bmpInfo));

	if (pEditCtrl->HasRowMark(pBookmark->nRow, xtpEditLMT_Bookmark))
	{
		int iImage = (pEditCtrl->GetCurrentDocumentRow() == pBookmark->nRow && pEditCtrl->HasFocus())? bookMarkSel: bookMark;
		if (GetBookmarks()->GetImageInfo(iImage, &bmpInfo))
		{
			CRect rcBmp(bmpInfo.rcImage);
			szRect.cx = min(szRect.cx, rcBmp.Width());
			szRect.cy = min(szRect.cy, rcBmp.Height());
		}

		IMAGELISTDRAWINDIRECT_S(GetBookmarks(), pDC, iImage, ptStart, szRect);
	}

	if (pEditCtrl->HasRowMark(pBookmark->nRow, xtpEditLMT_Breakpoint))
	{
		if (GetBookmarks()->GetImageInfo(breakpoint, &bmpInfo))
		{
			CRect rcBmp(bmpInfo.rcImage);
			szRect.cx = min(szRect.cx, rcBmp.Width());
			szRect.cy = min(szRect.cy, rcBmp.Height());
		}

		IMAGELISTDRAWINDIRECT_S(GetBookmarks(), pDC, breakpoint, ptStart, szRect);

		pEditCtrl->SetInternalRowBkColor(pBookmark->nRow, RGB(128, 0, 0));
		pEditCtrl->SetInternalRowColor(pBookmark->nRow, RGB(255, 255, 255));
	}
}

void CXTPSyntaxEditPaintManager::DrawLineMarks(CDC *pDC, const CRect& rcRect, int nTextRow, CXTPSyntaxEditCtrl* pEditCtrl)
{
	// NMHDR codes
	XTP_EDIT_SENMBOOKMARK bookmark;
	bookmark.nmhdr.code = XTP_EDIT_NM_DRAWBOOKMARK;
	bookmark.nmhdr.hwndFrom = pEditCtrl->GetSafeHwnd();
	bookmark.nmhdr.idFrom = 0;

	bookmark.hDC = pDC->GetSafeHdc();

	bookmark.rcBookmark = rcRect;
	bookmark.nRow = nTextRow;

	CRect rcLine = bookmark.rcBookmark;
	rcLine.left = rcLine.right-1;

	pDC->SetBkColor(::GetSysColor(COLOR_WINDOW));
	pDC->SetTextColor(::GetSysColor(COLOR_BTNFACE));
	pDC->FillRect(&bookmark.rcBookmark, pDC->GetHalftoneBrush());
	pDC->FillSolidRect(rcLine, ::GetSysColor(COLOR_3DSHADOW));

	BOOL bHandled = FALSE;
	CWnd *pParent = pEditCtrl->GetParent();
	if (pParent)
	{
		bHandled = (BOOL)pParent->SendMessage(WM_NOTIFY, 0, (LPARAM)&bookmark);
	}

	if (!bHandled)
	{
		DrawLineMark(pEditCtrl, &bookmark);
	}
}



int CXTPSyntaxEditPaintManager::DrawLineTextEx(CDC *pDC, const CRect& rcTextLine, int nTextRow,
											  int nLine, CXTPSyntaxEditCtrl* pEditCtrl)
{
	int nRowHeight = 0;

	ASSERT(pDC && pEditCtrl);
	if (!pDC || !pEditCtrl)
		return nRowHeight;

	CXTPSyntaxEditDrawTextProcessor& drawTxtProc = pEditCtrl->GetDrawTextProcessor();

	drawTxtProc.ResetRowInfo(nLine);

	if (nTextRow <= 0)
		return nRowHeight;

	const CString& strText = pEditCtrl->GetLineText(nTextRow);
	int nTextLenC = (int)_tcsclen(strText);
	CString strDispText;

	drawTxtProc.ExpandChars(strText, strDispText, 0, pEditCtrl->IsEnabledWhiteSpace());
	drawTxtProc.SetRowTabPositions(nLine, strText);

	COLORREF crBreakText = pEditCtrl->GetRowColor(nTextRow);
	BOOL bBreakText = crBreakText != COLORREF_NULL;

	COLORREF crBreakBack = pEditCtrl->GetRowBkColor(nTextRow);
	BOOL bBreakBack  = crBreakBack != COLORREF_NULL;

	if (!bBreakBack)
		crBreakBack = m_clrValues.GetBackColorEx(pEditCtrl);

	if (!pDC->IsPrinting())
		pDC->FillSolidRect(&rcTextLine, crBreakBack);

	if (nTextRow > pEditCtrl->GetRowCount())
		return nRowHeight;

	pDC->SetBkMode(OPAQUE);
	pDC->SetTextColor(m_clrValues.crText);

	CXTPSyntaxEditTextBlockList blocks;

	pEditCtrl->GetRowColors(nTextRow, 0, -1, m_clrValues, &blocks);


	if (blocks.GetCount() == 0)
	{
		XTP_EDIT_TEXTBLOCK defBlk;
		defBlk.nPos = 0;
		defBlk.nNextBlockPos = nTextLenC;
		defBlk.clrBlock = m_clrValues;
		blocks.AddTail(defBlk);
	}

	// draw text

	//=======================================================================
	BOOL bCoBlkDrawn = FALSE;
	XTP_EDIT_COLLAPSEDBLOCK* pCoDrawBlk = NULL;
	XTP_EDIT_LMPARAM LMCoParam;
	if (pEditCtrl->HasRowMark(nTextRow, xtpEditLMT_Collapsed, &LMCoParam))
	{
		pCoDrawBlk = (XTP_EDIT_COLLAPSEDBLOCK*)LMCoParam.GetPtr();
		ASSERT(pCoDrawBlk);
	}

	//---------------------------------------------------------------
	BOOL bInfinitSelEnd = FALSE;
	int nSelStart = pEditCtrl->m_pSelection->GetSelStartForRow_str(nTextRow, nLine);
	int nSelEnd = pEditCtrl->m_pSelection->GetSelEndForRow_str(nTextRow, nLine, &bInfinitSelEnd);

	//---------------------------------------------------------------
	POSITION pos = blocks.GetHeadPosition();
	while (pos)
	{
		const XTP_EDIT_TEXTBLOCK& txtBlk = blocks.GetNext(pos);

		// draw [...] if needed
		if (pCoDrawBlk)
		{
			CRect rcCoBlk = rcTextLine;
			rcCoBlk.left += drawTxtProc.GetRowWidth(nLine) - drawTxtProc.GetScrollXOffset();
			rcCoBlk.left = max(rcCoBlk.left, rcTextLine.left);
			rcCoBlk.left = min(rcCoBlk.left, rcTextLine.right);

			bCoBlkDrawn = pEditCtrl->ProcessCollapsedTextEx(pDC, pCoDrawBlk, txtBlk, rcCoBlk);
			if (bCoBlkDrawn)
			{
				break;
			}
		}

		// define default colors.
		COLORREF crBack = m_clrValues.GetBackColorEx(pEditCtrl);
		COLORREF crText = m_clrValues.crText;
		COLORREF crHiliteBack = m_clrValues.crHiliteBack;
		COLORREF crHiliteText = m_clrValues.crHiliteText;
		//COLORREF crInactiveHiliteText = m_clrValues.crInactiveHiliteText;
		//COLORREF crInactiveHiliteBack = m_clrValues.crInactiveHiliteBack;

// if syntax coloring is enabled use defined colors.
//      if (pEditCtrl->GetSyntaxColor())
//      {
		crBack    = txtBlk.clrBlock.crBack;
		crText    = txtBlk.clrBlock.crText;
		crHiliteBack = txtBlk.clrBlock.crHiliteBack;
		crHiliteText = txtBlk.clrBlock.crHiliteText;
//      }

		if (bBreakBack)
			crBack = crBreakBack;

		if (bBreakText)
			crText = crBreakText;

		//--------------------------------------------------------------
		if (crBack == (COLORREF)m_clrValues.crBack)
			crBack = m_clrValues.GetBackColorEx(pEditCtrl);

		//if (crBack == m_clrValues.crBack.GetStandardColor())
//          crBack = m_clrValues.crBack;

//      if (crText == m_clrValues.crText.GetStandardColor())
//          crText = m_clrValues.crText;

		//--------------------------------------------------------------
		//if (!pEditCtrl->IsActive() || !pEditCtrl->m_bFocused)
		if (!pEditCtrl->IsActive())
		{
			if (crHiliteBack == m_clrValues.crHiliteBack.GetStandardColor())
				crHiliteBack = m_clrValues.crInactiveHiliteBack;

			if (crHiliteText == m_clrValues.crHiliteText.GetStandardColor())
				crHiliteText = m_clrValues.crInactiveHiliteText;
		}

		//--------------------------------------------------------------
		XTP_EDIT_TEXTBLOCK arTxtBlk[4];
		BOOL bUseBlk[4] = {FALSE, FALSE, FALSE, FALSE};

		if (pEditCtrl->m_pSelection->IsIntersectSel_str(nTextRow, txtBlk.nPos/* + 1*/, txtBlk.nNextBlockPos) &&
			!pDC->IsPrinting() && nTextLenC && nSelStart < nSelEnd)
		{
			DrawLineCalcSel(txtBlk, nSelStart, nSelEnd, arTxtBlk, bUseBlk);
		}
		else if (nTextLenC)
		{
			arTxtBlk[1] = txtBlk;
			bUseBlk[1] = TRUE;
		}

		//**************************************************
		for (int i = 1; i <= 3; i++)
		{
			if (!bUseBlk[i])
				continue;

			pDC->SetTextColor((i%2) ? crText : crHiliteText);
			pDC->SetBkColor((i%2) ? crBack : crHiliteBack);

			//pDC->SetTextColor((i%2) ? crText : ((pEditCtrl->GetFocus() == pEditCtrl) ? crHiliteText : crInactiveHiliteText));
			//pDC->SetBkColor((i%2) ? crBack : ((pEditCtrl->GetFocus() == pEditCtrl) ? crHiliteBack : crInactiveHiliteBack));

			int nOutStrPos = drawTxtProc.StrPosToDispPos(nLine, arTxtBlk[i].nPos);
			int nNextOutStrPos = drawTxtProc.StrPosToDispPos(nLine, arTxtBlk[i].nNextBlockPos);
			int nOutStrLen = nNextOutStrPos - nOutStrPos;
			//LPCTSTR pText = (LPCTSTR)strDispText + nOutStrPos;
#ifdef XTP_FIXED
// nOutStrPos : Display Length
			LPCTSTR pText = (LPCTSTR) strDispText + nOutStrPos;
#else
			LPCTSTR pText = _tcsninc((LPCTSTR)strDispText, nOutStrPos);
#endif
			UpdateTextFont(pEditCtrl, arTxtBlk[i].lf);
			CXTPFontDC fontDC(pDC, GetFontText());

			if (!pDC->IsPrinting())
			{
				drawTxtProc.DrawRowPart(pDC, nLine, pText, nOutStrLen);
			}
			else
			{
				int nOffsetY = rcTextLine.top - drawTxtProc.GetTextRect().top;
				nRowHeight = drawTxtProc.PrintRowPart(pDC, nTextRow, nOffsetY, 0,
													  pText, nOutStrLen, 0);//int *pnPrintedTextLen)
			}
		}

		if (!pDC->IsPrinting())
		{
			// draw selection for virtual space, after line text end)
			if (!pos && ((nSelStart < nSelEnd && nSelEnd > nTextLenC) || bInfinitSelEnd))
			{
				CRect rcSel = rcTextLine;
				rcSel.left += drawTxtProc.GetRowWidth(nLine) - drawTxtProc.GetScrollXOffset();

				if (!bInfinitSelEnd || nSelStart > nTextLenC)
				{
					if (nSelStart > nTextLenC)
						rcSel.left += drawTxtProc.GetSpaceWidth() * (nSelStart - nTextLenC);

					if (!bInfinitSelEnd)
						rcSel.right = rcSel.left + drawTxtProc.GetSpaceWidth() * abs(nSelEnd - max(nTextLenC, nSelStart));
				}

				rcSel.left = max(min(rcSel.left, rcTextLine.right), rcTextLine.left);
				rcSel.right = max(min(rcSel.right, rcTextLine.right), rcTextLine.left);

				pDC->FillSolidRect(rcSel, crHiliteBack);
			}
		}
	}

	//===========================================================================
	// draw [...] if needed and not yet
	if (pCoDrawBlk && !bCoBlkDrawn)
	{
		CRect rcCoBlk = rcTextLine;
		rcCoBlk.left += drawTxtProc.GetRowWidth(nLine) - drawTxtProc.GetScrollXOffset();
		rcCoBlk.left = max(rcCoBlk.left, rcTextLine.left);
		rcCoBlk.left = min(rcCoBlk.left, rcTextLine.right);

		pEditCtrl->ProcessCollapsedText(pDC, pCoDrawBlk, rcCoBlk);
	}

	pDC->SetTextColor(m_clrValues.crText);

	return nRowHeight;
}


int CXTPSyntaxEditPaintManager::PrintLineTextEx(CDC *pDC, const CRect& rcTextLine, int nTextRow,
												int nLine, CXTPSyntaxEditCtrl* pEditCtrl, int nFlags)
{
	ASSERT(pDC && pEditCtrl);
	if (!pDC || !pEditCtrl)
		return 0;

	CXTPSyntaxEditDrawTextProcessor& drawTxtProc = pEditCtrl->GetDrawTextProcessor();

	int nRowHeight = drawTxtProc.GetRowHeight();

	const CString& strText = pEditCtrl->GetLineText(nTextRow);
	int nTextLenC = (int)_tcsclen(strText);
	CString strDispText;
	drawTxtProc.ExpandChars(strText, strDispText, 0, pEditCtrl->IsEnabledWhiteSpace());

	drawTxtProc.ResetRowInfo(nLine);
	drawTxtProc.SetRowTabPositions(nLine, strText);

	COLORREF crBreakText = pEditCtrl->GetRowColor(nTextRow);
	BOOL bBreakText = crBreakText != COLORREF_NULL;

	COLORREF crBreakBack = pEditCtrl->GetRowBkColor(nTextRow);
	BOOL bBreakBack  = crBreakBack != COLORREF_NULL;

	CXTPSyntaxEditLexTextSchema* ptrTxtSch = pEditCtrl->m_pBuffer->GetLexParser()->GetTextSchema();

	if (!bBreakBack)
		crBreakBack = m_clrValues.crBack;

	if (nTextRow > pEditCtrl->GetRowCount())
		return 0;

	pDC->SetBkMode(OPAQUE);
	pDC->SetTextColor(m_clrValues.crText);

	CXTPSyntaxEditTextBlockList blocks;

	if (ptrTxtSch)
	{
		CXTPSyntaxEditLexTextBlock* pScreenSchFirstTB = pEditCtrl->GetOnScreenSch(nTextRow);
		if (pScreenSchFirstTB)
		{
			CXTPSyntaxEditTextIterator txtIter(pEditCtrl->GetEditBuffer());
			ptrTxtSch->GetRowColors(&txtIter, nTextRow, 0, -1,
									m_clrValues, &blocks,
									NULL, pScreenSchFirstTB);
		}
	}

	if (blocks.GetCount() == 0)
	{
		XTP_EDIT_TEXTBLOCK defBlk;
		defBlk.nPos = 0;
		defBlk.nNextBlockPos = nTextLenC;
		defBlk.clrBlock = m_clrValues;
		blocks.AddTail(defBlk);
	}

	// draw text

	//=======================================================================
	BOOL bCoBlkDrawn = FALSE;
	XTP_EDIT_COLLAPSEDBLOCK* pCoDrawBlk = NULL;
	XTP_EDIT_LMPARAM LMCoParam;
	if (pEditCtrl->HasRowMark(nTextRow, xtpEditLMT_Collapsed, &LMCoParam))
	{
		pCoDrawBlk = (XTP_EDIT_COLLAPSEDBLOCK*)LMCoParam.GetPtr();
		ASSERT(pCoDrawBlk);
	}

	//---------------------------------------------------------------
	POSITION pos = blocks.GetHeadPosition();
	while (pos)
	{
		const XTP_EDIT_TEXTBLOCK& txtBlk = blocks.GetNext(pos);

		// draw [...] if needed
		if (pCoDrawBlk)
		{
			CRect rcCoBlk = rcTextLine;
			rcCoBlk.left += drawTxtProc.GetRowWidth(nLine) - drawTxtProc.GetScrollXOffset();
			rcCoBlk.left = max(rcCoBlk.left, rcTextLine.left);
			rcCoBlk.left = min(rcCoBlk.left, rcTextLine.right);

			bCoBlkDrawn = pEditCtrl->ProcessCollapsedTextEx(pDC, pCoDrawBlk, txtBlk, rcCoBlk);
			if (bCoBlkDrawn)
			{
				break;
			}
		}

		// define default colors.
		COLORREF crBack    = m_clrValues.crBack.GetStandardColor();
		COLORREF crText    = m_clrValues.crText.GetStandardColor();

		// if syntax coloring is enabled use defined colors.
		if (pEditCtrl->GetSyntaxColor())
		{
			crBack = txtBlk.clrBlock.crBack;
			crText = txtBlk.clrBlock.crText;
		}

		if (bBreakBack)
			crBack = crBreakBack;

		if (bBreakText)
			crText = crBreakText;

		//--------------------------------------------------------------
		if (crBack == m_clrValues.crBack.GetStandardColor())
			crBack = m_clrValues.crBack;

		if (crText == m_clrValues.crText.GetStandardColor())
			crText = m_clrValues.crText;

		//**************************************************
		pDC->SetTextColor(crText);
		pDC->SetBkColor(crBack);

		int nOutStrPos = drawTxtProc.StrPosToDispPos(nLine, txtBlk.nPos);
		int nNextOutStrPos = drawTxtProc.StrPosToDispPos(nLine, txtBlk.nNextBlockPos);
		int nOutStrLen = nNextOutStrPos - nOutStrPos;
		//LPCTSTR pText = (LPCTSTR)strDispText + nOutStrPos;
		LPCTSTR pText = _tcsninc((LPCTSTR)strDispText, nOutStrPos);

		UpdateTextFont(pEditCtrl, txtBlk.lf);
		CXTPFontDC fontDC(pDC, GetFontText());

		int nPrintedTextLen = 0;
		int nOffsetY = rcTextLine.top - drawTxtProc.GetTextRect().top;
		nRowHeight = drawTxtProc.PrintRowPart(pDC, nTextRow, nOffsetY, nFlags,
												  pText, nOutStrLen, &nPrintedTextLen);

		if (nPrintedTextLen < nOutStrLen)
		{
			if ((nFlags & DT_SINGLELINE) == 0)
			{
				nRowHeight = -1;
				pDC->FillSolidRect(&rcTextLine, pDC->GetBkColor());
			}

			break;
		}
	}

	//===========================================================================
	// draw [...] if needed and not yet
	if (pCoDrawBlk && !bCoBlkDrawn)
	{
		CRect rcCoBlk = rcTextLine;
		rcCoBlk.left += drawTxtProc.GetRowWidth(nLine) - drawTxtProc.GetScrollXOffset();
		rcCoBlk.left = max(rcCoBlk.left, rcTextLine.left);
		rcCoBlk.left = min(rcCoBlk.left, rcTextLine.right);

		pEditCtrl->ProcessCollapsedText(pDC, pCoDrawBlk, rcCoBlk);
	}

	pDC->SetTextColor(m_clrValues.crText);

	return nRowHeight;
}

#ifdef _XTP_ACTIVEX

// {296BBEEC-E55D-40e1-A0E5-4AFF478A7042}
static const GUID IID_ISyntaxEditPaintManager =
{ 0x296bbeec, 0xe55d, 0x40e1, { 0xa0, 0xe5, 0x4a, 0xff, 0x47, 0x8a, 0x70, 0x42 } };

BEGIN_INTERFACE_MAP(CXTPSyntaxEditPaintManager, CCmdTarget)
	INTERFACE_PART(CXTPSyntaxEditPaintManager, IID_ISyntaxEditPaintManager, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPSyntaxEditPaintManager, IID_ISyntaxEditPaintManager)

#define XTPDISP_DECLARE_COLOR_PROP(propName, dispID) \
	DISP_PROPERTY_EX_ID(CXTPSyntaxEditPaintManager, #propName, dispID, OleGet##propName, OleSet##propName, VT_COLOR)

BEGIN_DISPATCH_MAP(CXTPSyntaxEditPaintManager, CCmdTarget)
	XTPDISP_DECLARE_COLOR_PROP(TextColor, 1)
	XTPDISP_DECLARE_COLOR_PROP(BackColor, 2)
	XTPDISP_DECLARE_COLOR_PROP(ReadOnlyBackColor, 3)
	XTPDISP_DECLARE_COLOR_PROP(LineNumberTextColor, 4)
	XTPDISP_DECLARE_COLOR_PROP(LineNumberBackColor, 5)
	XTPDISP_DECLARE_COLOR_PROP(SelectedTextColor, 6)
	XTPDISP_DECLARE_COLOR_PROP(SelectedBackColor, 7)
END_DISPATCH_MAP()


#define XTP_IMPLEMENT_COLOR_PROP(propName, colorMember) \
	OLE_COLOR CXTPSyntaxEditPaintManager::OleGet##propName() \
	{                                                   \
	return (OLE_COLOR)(COLORREF)m_clrValues.##colorMember;  \
	}                                                   \
	void CXTPSyntaxEditPaintManager::OleSet##propName(OLE_COLOR oleColor)   \
	{                                                   \
		m_clrValues.##colorMember = AxTranslateColor(oleColor);     \
	}

XTP_IMPLEMENT_COLOR_PROP(TextColor, crText)
XTP_IMPLEMENT_COLOR_PROP(BackColor, crBack)
XTP_IMPLEMENT_COLOR_PROP(ReadOnlyBackColor, crReadOnlyBack)
XTP_IMPLEMENT_COLOR_PROP(LineNumberTextColor, crLineNumberText)
XTP_IMPLEMENT_COLOR_PROP(LineNumberBackColor, crLineNumberBack)
XTP_IMPLEMENT_COLOR_PROP(SelectedTextColor, crHiliteText)
XTP_IMPLEMENT_COLOR_PROP(SelectedBackColor, crHiliteBack)

#endif
