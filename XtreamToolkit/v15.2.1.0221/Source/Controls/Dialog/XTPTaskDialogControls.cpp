// XTPTaskDialogControls.cpp
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

#include "Common/Resource.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPResourceManager.h"
#include "Common/XTPWinThemeWrapper.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPImageManager.h"
#include "Common/XTPMarkupRender.h"

#include "../Util/XTPControlTheme.h"

#include "../Button/XTPButton.h"
#include "../Button/XTPButtonTheme.h"
#include "XTPTaskDialogAPI.h"
#include "XTPTaskDialogControls.h"

#ifndef WM_UPDATEUISTATE
#define WM_UPDATEUISTATE    0x0128
#endif

#ifndef WM_QUERYUISTATE
#define WM_QUERYUISTATE 0x0129
#endif

#ifndef UISF_HIDEACCEL
#define UISF_HIDEACCEL 0x2
#endif

#ifndef UISF_HIDEFOCUS
#define UISF_HIDEFOCUS 0x1
#endif

#ifndef ODS_NOACCEL
#define ODS_NOACCEL 0x0100
#endif


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTPTaskDialogLinkCtrl

CXTPTaskDialogLinkCtrl::CXTPTaskDialogLinkCtrl()
	: m_nFocused(-1)
	, m_crBack(COLORREF_NULL)
	, m_crText(GetXtremeColor(COLOR_WINDOWTEXT))
	, m_crTextLink(RGB(0x00, 0x66, 0xcc))
	, m_hcurHand(NULL)
	, m_bPreSubclassWindow(TRUE)
{
	// try loading system hand cursor first.
	m_hcurHand = ::LoadCursor(NULL, MAKEINTRESOURCE(32649));

	// if this fails load default.
	if (m_hcurHand == NULL)
	{
		m_hcurHand = XTPResourceManager()->LoadCursor(XTP_IDC_HAND);
		ASSERT(m_hcurHand != NULL);
	}
}

CXTPTaskDialogLinkCtrl::~CXTPTaskDialogLinkCtrl()
{
	RemoveAllLinks();
}

void CXTPTaskDialogLinkCtrl::RemoveAllLinks()
{
	for (int i = 0; i < m_arrLinks.GetSize(); i++)
	{
		delete m_arrLinks[i];
	}
	m_arrLinks.RemoveAll();
}

IMPLEMENT_DYNCREATE(CXTPTaskDialogLinkCtrl, CWnd)

BEGIN_MESSAGE_MAP(CXTPTaskDialogLinkCtrl, CWnd)
	//{{AFX_MSG_MAP(CXTPTaskDialogLinkCtrl)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_NCHITTEST_EX()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_GETDLGCODE()
	ON_WM_KEYDOWN()
	ON_MESSAGE(WM_SETTEXT, OnSetText)

	ON_MESSAGE(WM_UPDATEUISTATE, OnUpdateUIState)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTPTaskDialogLinkCtrl message handlers


CXTPTaskDialogLinkCtrl::LINKITEM* CXTPTaskDialogLinkCtrl::HitTest(CPoint pt) const
{
	for (int i = 0; i < m_arrLinks.GetSize(); i++)
	{
		LINKITEM* pItem = m_arrLinks[i];

		for (int j = 0; j < pItem->arrParts.GetSize(); j++)
		{
			if (::PtInRect(&pItem->arrParts[j], pt))
				return pItem;
		}
	}

	return NULL;
}

void CXTPTaskDialogLinkCtrl::OnPaint()
{
	CPaintDC dc(this);

	CXTPClientRect rClient(this);
	CXTPBufferDC memDC(dc);

	if (m_crBack != COLORREF_NULL)
		memDC.FillSolidRect(&rClient, m_crBack);
	else
	{
		HBRUSH hBrush = (HBRUSH)GetParent()->SendMessage(WM_CTLCOLORSTATIC, (WPARAM)memDC.GetSafeHdc(), (LPARAM)m_hWnd);
		if (hBrush)
		{
			::FillRect(memDC.GetSafeHdc(), rClient, hBrush);
		}
		else
		{
			memDC.FillSolidRect(rClient, GetXtremeColor(COLOR_3DFACE));
		}
	}
	memDC.SetBkMode(TRANSPARENT);

	DrawText(&memDC, rClient);
}

int CXTPTaskDialogLinkCtrl::DrawTextPartText(CDC* pDC, CString strBuffer, int x, int y, LINKITEM* pItem)
{
	if (strBuffer.IsEmpty())
		return 0;

	CSize sz = pDC->GetTextExtent(strBuffer);
	pDC->DrawText(strBuffer, CRect(x, y, x + sz.cx + 10, y + sz.cy), DT_TOP | DT_LEFT | DT_NOPREFIX | DT_SINGLELINE);

	CRect rcItem(x, y, x + sz.cx, y + sz.cy);

	if (pItem)
	{
		if (m_nFocused == pItem->nIndex)
		{
			COLORREF clr = pDC->SetTextColor(m_crText);
			pDC->DrawFocusRect(rcItem);

			pDC->SetTextColor(clr);
		}

		pItem->arrParts.Add(rcItem);
	}

	return sz.cx;
}

void CXTPTaskDialogLinkCtrl::DrawTextPart(CDC* pDC, int&x, int&y, int nWidth, CString strBuffer, LINKITEM* pItem)
{
	CXTPFontDC fontDC(pDC, pItem && m_fontUL.GetSafeHandle() ? &m_fontUL : GetFont(), pItem ? m_crTextLink :m_crText);

	int nTextHeight = pDC->GetTextExtent(_T("XXX"), 3).cy;

	int nStartPos = 0;
	int nGoodPos = 0;

	while (nStartPos < strBuffer.GetLength())
	{
		TCHAR t = strBuffer[nStartPos];

		if (t == _T(' ') || t == _T('\t') || t == _T('\n'))
		{
			CString strPart = strBuffer.Left(nStartPos);
			if (x + pDC->GetTextExtent(strPart).cx > nWidth)
			{
				if (x == 0 && nGoodPos == 0)
					nGoodPos = nStartPos - 1;

				if (nGoodPos > 0)
				{
					strPart = strBuffer.Left(nGoodPos);
					DrawTextPartText(pDC, strPart, x, y, pItem);

					strBuffer.Delete(0, nGoodPos + 1);
					nStartPos -= nGoodPos + 1;
				}
				else
				{
					if (strBuffer[0] == _T(' '))
					{
						strBuffer.Delete(0);
						nStartPos--;
					}
				}

				x = 0;
				y += nTextHeight;

				nGoodPos = 0;
				if (t != _T('\n')) continue;
			}
			else
				nGoodPos = nStartPos;
		}

		if (t == _T('\n'))
		{
			CString strPart = strBuffer.Left(nStartPos);
			DrawTextPartText(pDC, strPart, x, y, pItem);

			strBuffer.Delete(0, nStartPos + 1);

			x = 0;
			y += nTextHeight;

			nStartPos = nGoodPos = 0;
			continue;
		}

		nStartPos++;
	}

	CString strPart = strBuffer;
	if (x + pDC->GetTextExtent(strPart).cx > nWidth)
	{
		if (nGoodPos != 0)
		{
			strPart = strBuffer.Left(nGoodPos);
			DrawTextPartText(pDC, strPart, x, y, pItem);

			strBuffer.Delete(0, nGoodPos + 1);
		}

		x = 0;
		y += nTextHeight;
	}

	int nLength = DrawTextPartText(pDC, strBuffer, x, y, pItem);
	x += nLength;
}

void CXTPTaskDialogLinkCtrl::DrawText(CDC* pDC, CRect rClient)
{
	if (m_arrLinks.GetSize() == 0)
	{
		CXTPFontDC fontDC(pDC, GetFont(), m_crText);
		pDC->DrawText(m_strBuffer, rClient, DT_TOP | DT_WORDBREAK | DT_NOPREFIX);
		return;
	}
	int x = 0, y = 0, nWidth = rClient.Width();

	CString strBuffer = m_strBuffer;

	int nPos = 0;

	for (int i = 0; i < m_arrLinks.GetSize(); i++)
	{
		LINKITEM* pItem = m_arrLinks.GetAt(i);

		int iStart = pItem->nStart;
		int iCount = pItem->strLabel.GetLength();

		pItem->arrParts.RemoveAll();

		DrawTextPart(pDC, x, y, nWidth, m_strBuffer.Mid(nPos, iStart - nPos), NULL);
		DrawTextPart(pDC, x, y, nWidth, m_strBuffer.Mid(iStart, iCount), pItem);

		nPos = iStart + iCount;
	}

	DrawTextPart(pDC, x, y, nWidth, m_strBuffer.Mid(nPos, m_strBuffer.GetLength() - nPos), NULL);

}


BOOL CXTPTaskDialogLinkCtrl::OnEraseBkgnd(CDC* pDC)
{
	UNREFERENCED_PARAMETER(pDC);
	return TRUE;
}


BOOL CXTPTaskDialogLinkCtrl::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	//
	// PreCreateWindow is called when a control is dynamically
	// created. We want to set m_bPreSubclassWindow to FALSE
	// here so the control is initialized from CWnd::Create and
	// not CWnd::PreSubclassWindow.
	//

	m_bPreSubclassWindow = FALSE;

	return TRUE;
}

void CXTPTaskDialogLinkCtrl::PreSubclassWindow()
{
	CWnd::PreSubclassWindow();

	if (m_bPreSubclassWindow)
	{
		RemoveAllLinks();
		GetWindowText(m_strBuffer);

		ExtractLinks(m_strBuffer);
		DefWindowProc(WM_SETTEXT, 0, (LPARAM)(LPCTSTR)m_strBuffer);

		if (m_arrLinks.GetSize() > 0 && GetFont())
		{
			LOGFONT lf;
			GetFont()->GetLogFont(&lf);

			lf.lfUnderline = 1;
			m_fontUL.DeleteObject();
			m_fontUL.CreateFontIndirect(&lf);
		}
		Invalidate(FALSE);
	}
}

LRESULT CXTPTaskDialogLinkCtrl::OnSetText(WPARAM, LPARAM lParam)
{
	RemoveAllLinks();

	m_strBuffer = (LPCTSTR)lParam;
	ExtractLinks(m_strBuffer);

	DefWindowProc(WM_SETTEXT, 0, (LPARAM)(LPCTSTR)m_strBuffer);
	Invalidate(FALSE);

	return m_strBuffer.GetLength();
}

BOOL CXTPTaskDialogLinkCtrl::Create(const CRect& rect, CString& strBuffer, CFont* pFont, CWnd* pParentWnd)
{
	RemoveAllLinks();

	if (strBuffer.IsEmpty() || !pFont->GetSafeHandle() || !::IsWindow(pParentWnd->GetSafeHwnd()))
		return FALSE;

	m_strBuffer = strBuffer;
	if (!ExtractLinks(m_strBuffer))
		return FALSE;

	if (!CStatic::Create(0, WS_CHILD | WS_VISIBLE | WS_TABSTOP, rect, pParentWnd, 0))
		return FALSE;

	LOGFONT lf;
	pFont->GetLogFont(&lf);

	lf.lfUnderline = 1;
	m_fontUL.DeleteObject();
	m_fontUL.CreateFontIndirect(&lf);

	SetFont(pFont);
	DefWindowProc(WM_SETTEXT, 0, (LPARAM)(LPCTSTR)m_strBuffer);

	strBuffer = m_strBuffer;

	return TRUE;
}

DWORD CXTPTaskDialogLinkCtrl::GetLinkPos(CString strBuffer, int iPos /*= 0*/)
{
	strBuffer.MakeLower();

	int iStart = strBuffer.Find(_T("<a href=\""), iPos);
	if (iStart == -1)
		return NULL;

	int iValid = strBuffer.Find(_T("\""), iStart);
	if (iValid == -1)
		return NULL;

	int iEnd = strBuffer.Find(_T("</a>"), iValid);
	if (iEnd == -1)
		return NULL;

	return MAKELONG(iStart, (iEnd + 4) - iStart);
}

BOOL CXTPTaskDialogLinkCtrl::ExtractLinks(CString& strBuffer)
{
	m_arrLinks.RemoveAll();

	for (DWORD dwPos = GetLinkPos(strBuffer); dwPos;)
	{
		int iStart = LOWORD(dwPos);
		int iCount = HIWORD(dwPos);

		LINKITEM* pItem = new LINKITEM();

		CString strLink = strBuffer.Mid(iStart,iCount);

		// extract the link and execute strings from the link.
		AfxExtractSubString(pItem->strUrl, strLink, 1, '"');
		AfxExtractSubString(pItem->strUrl, pItem->strUrl,  0, '"');
		AfxExtractSubString(pItem->strLabel, strLink, 1, '>');
		AfxExtractSubString(pItem->strLabel, pItem->strLabel, 0, '<');

		// reformat text to remove link tags.
		CString strTemp = strBuffer.Left(iStart);
		strTemp  += pItem->strLabel;
		strTemp  += strBuffer.Right(strBuffer.GetLength()-(iStart + iCount));
		strBuffer = strTemp;

		pItem->nStart = iStart;
		pItem->nIndex = (int)m_arrLinks.Add(pItem);

		dwPos = GetLinkPos(strBuffer);
	}

	return (m_arrLinks.GetSize() > 0);
}

LRESULT CXTPTaskDialogLinkCtrl::OnNcHitTest(CPoint point)
{
	ScreenToClient(&point);

	if (HitTest(point))
	{
		return HTCLIENT;
	}

	return (LRESULT)HTTRANSPARENT;
}

void CXTPTaskDialogLinkCtrl::FocusItem(int iFocusItem)
{
	if (m_nFocused != iFocusItem)
	{
		m_nFocused = iFocusItem;
		Invalidate(FALSE);
	}
}

UINT CXTPTaskDialogLinkCtrl::OnGetDlgCode()
{
	const MSG& msg = AfxGetThreadState()->m_lastSentMsg;

	if (!msg.lParam)
		return DLGC_UNDEFPUSHBUTTON;

	LPMSG lpMsg = (LPMSG)msg.lParam;

	if (lpMsg->message == WM_CHAR)
	{
		if (lpMsg->wParam == VK_TAB)
			return DLGC_WANTTAB;

		if (lpMsg->wParam == VK_RETURN)
			return DLGC_WANTCHARS;
	}

	if (lpMsg->message == WM_KEYDOWN)
	{
		UINT nChar = (UINT)lpMsg->wParam;

		if ((nChar == VK_RETURN || nChar == VK_SPACE) && (m_nFocused != -1))
			return DLGC_WANTALLKEYS;

		if (nChar == VK_TAB)
		{
			if (GetKeyState(VK_SHIFT) >= 0)
			{
				if (m_nFocused < m_arrLinks.GetSize() - 1)
				{
					FocusItem(m_nFocused + 1);
					return DLGC_WANTTAB;
				}
			}
			else
			{
				if (m_nFocused > 0)
				{
					FocusItem(m_nFocused - 1);
					return DLGC_WANTTAB;
				}
			}
		}
	}

	return DLGC_UNDEFPUSHBUTTON;
}

void CXTPTaskDialogLinkCtrl::OnKeyDown(UINT nChar, UINT /*nRepCnt*/, UINT /*nFlags*/)
{
	if ((nChar == VK_RETURN || nChar == VK_SPACE) && (m_nFocused != -1))
	{
		GetOwner()->SendMessage(WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(), BN_CLICKED), (LPARAM)m_hWnd);
	}
}

void CXTPTaskDialogLinkCtrl::OnLButtonDown(UINT /*nFlags*/, CPoint point)
{
	LINKITEM* pItem = HitTest(point);

	if (pItem)
	{
		FocusItem(pItem->nIndex);
		SetFocus();
	}
	else
	{
		FocusItem(-1);
	}
}

void CXTPTaskDialogLinkCtrl::OnSetFocus(CWnd* /*pOldWnd*/)
{
	if (m_nFocused == -1)
	{
		FocusItem(0);
	}
}

void CXTPTaskDialogLinkCtrl::OnKillFocus(CWnd* /*pNewWnd*/)
{
	FocusItem(-1);
}

CXTPTaskDialogLinkCtrl::LINKITEM* CXTPTaskDialogLinkCtrl::GetFocusedLink() const
{
	return m_nFocused >= 0 && m_nFocused < m_arrLinks.GetSize() ? m_arrLinks[m_nFocused] : NULL;
}


void CXTPTaskDialogLinkCtrl::OnLButtonUp(UINT /*nFlags*/, CPoint point)
{
	LINKITEM* pItem = HitTest(point);

	if (pItem && pItem->nIndex == m_nFocused)
	{
		GetOwner()->SendMessage(WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(), BN_CLICKED), (LPARAM)m_hWnd );
	}
}

BOOL CXTPTaskDialogLinkCtrl::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	if (nHitTest == HTCLIENT && (m_hcurHand != NULL))
	{
		::SetCursor(m_hcurHand);
		return TRUE;
	}

	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

LRESULT CXTPTaskDialogLinkCtrl::OnUpdateUIState(WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult = ::DefWindowProc(m_hWnd, WM_UPDATEUISTATE, wParam, lParam);
	Invalidate(FALSE);

	return lResult;
}
//===========================================================================
// CXTPTaskButtonTheme
//===========================================================================
const UINT  xtpDluMargin           = 5;   // non-client margin.


CXTPTaskButtonTheme::CXTPTaskButtonTheme(BOOL bThemeReady)
{
	m_bThemeReady = bThemeReady;
	m_ptMargin = XTPDlu2Pix(xtpDluMargin, xtpDluMargin);
	m_pFontTitle = NULL;
	m_bOffsetHiliteText = FALSE;
	m_nBorderWidth = 3;
}

CXTPTaskButtonTheme::CXTPTaskButtonTheme()
{
	m_themeButton->OpenThemeData(0, L"BUTTON");

	m_bThemeReady = m_themeButton->IsAppThemed();
	m_ptMargin = XTPDlu2Pix(xtpDluMargin, xtpDluMargin);
	m_pFontTitle = NULL;
	m_bOffsetHiliteText = FALSE;
	m_nBorderWidth = 3;
}

void CXTPTaskButtonTheme::DrawButton(CDC* pDC, CXTPButton* pButton)
{
	if (!DrawWinThemeBackground(pDC, pButton))
	{
		DrawButtonThemeBackground(pDC, pButton);
	}

	DrawButtonText(pDC, pButton);
	DrawButtonIcon(pDC, pButton);

	// Draw the focus rect if style is set and we have focus.
	if (::GetFocus() == pButton->GetSafeHwnd() && ((pButton->SendMessage(WM_QUERYUISTATE) & UISF_HIDEFOCUS) == 0))
	{
		DrawFocusRect(pDC, pButton);
	}
}

BOOL CXTPTaskButtonTheme::DrawButtonThemeBackground(CDC* pDC, CXTPButton* pButton)
{
	CXTPClientRect rcItem(pButton);

	if (pButton->GetChecked())
		pDC->FillSolidRect(rcItem, m_crBorderHilite);
	else if (pButton->IsPushed() || pButton->IsHighlighted())
	{
		pDC->FillSolidRect(rcItem, m_crBack);
	}
	else
	{

		HBRUSH hBrush = (HBRUSH)pButton->GetParent()->SendMessage(WM_CTLCOLORBTN, (WPARAM)pDC->GetSafeHdc(), (LPARAM)pButton->GetSafeHwnd());
		if (hBrush)
		{
			::FillRect(pDC->GetSafeHdc(), rcItem, hBrush);
		}
		else
		{
			pDC->FillSolidRect(rcItem, m_crBack);
		}
	}


	if (pButton->IsPushed())
	{
		pDC->Draw3dRect(rcItem, m_crBorder3DShadow, m_crBorder3DHilite);
		rcItem.DeflateRect(1,1);
		pDC->Draw3dRect(rcItem, m_crBorderShadow, m_crBorderHilite);
	}
	else if (pButton->IsHighlighted())
	{
		pDC->Draw3dRect(rcItem, m_crBorder3DHilite, m_crBorder3DShadow);
		rcItem.DeflateRect(1,1);
		pDC->Draw3dRect(rcItem, m_crBorderHilite, m_crBorderShadow);
	}

	if (pButton->GetButtonStyle() == BS_DEFPUSHBUTTON)
	{
		pDC->Draw3dRect(CXTPClientRect(pButton), m_crBorderActive, m_crBorderActive);
	}

	return TRUE;
}

BOOL CXTPTaskButtonTheme::DrawWinThemeBackground(CDC* pDC, CXTPButton* pButton)
{
	if (m_bThemeReady)
	{
		CXTPClientRect rcItem(pButton);

		HBRUSH hBrush = (HBRUSH)pButton->GetParent()->SendMessage(WM_CTLCOLORBTN, (WPARAM)pDC->GetSafeHdc(), (LPARAM)pButton->GetSafeHwnd());
		if (hBrush)
		{
			::FillRect(pDC->GetSafeHdc(), rcItem, hBrush);
		}
		else
		{
			pDC->FillSolidRect(rcItem, m_crBack);
		}


		if (!m_themeButton->ThemeDataOpen())
			m_themeButton->OpenThemeData(0, L"BUTTON");

		if (m_themeButton->IsAppThemed() && XTPSystemVersion()->IsWinVistaOrGreater())
		{
			CRect rcButton(rcItem);

			int iStateId = (pButton->IsPushed()) ? PBS_PRESSED :
				pButton->IsHighlighted() ? PBS_HOT :
				pButton->GetButtonStyle() == BS_DEFPUSHBUTTON ? PBS_DEFAULTED : PBS_NORMAL;

			if (!pButton->IsWindowEnabled())
				iStateId = PBS_DISABLED;

			if (SUCCEEDED(m_themeButton->DrawThemeBackground(pDC->GetSafeHdc(),
				6, iStateId, &rcButton, NULL)))
			{
				return TRUE;
			}
		}

		XTPDrawHelpers()->ExcludeCorners(pDC, rcItem);

		BOOL bSelected = pButton->IsPushed();

		if (bSelected)
		{
			pDC->FillSolidRect(rcItem, m_crBackSelected);
			pDC->Draw3dRect(rcItem, m_crBorderHilite, m_crBorderHilite);

			rcItem.DeflateRect(1,1);
			XTPDrawHelpers()->DrawLine(pDC, rcItem.left, rcItem.top, rcItem.Width(), 0, m_crBorder3DShadow);

			rcItem.top += 1;
			pDC->Draw3dRect(&rcItem, m_crBorder3DHilite, m_crBorder3DHilite);

			rcItem.DeflateRect(1,1);
			pDC->Draw3dRect(&rcItem, m_crBorderShadow, m_crBorderShadow);
		}

		else if (pButton->IsHighlighted())
		{
			XTPDrawHelpers()->GradientFill(pDC, &rcItem, m_crBack, m_crBorderShadow, FALSE);
			pDC->Draw3dRect(rcItem, m_crBorderHilite, m_crBorderHilite);

			rcItem.DeflateRect(1,1);
			pDC->Draw3dRect(&rcItem, m_crBack, m_crBack);
		}

		if (pButton->GetButtonStyle() == BS_DEFPUSHBUTTON)
		{
			if (!bSelected && !pButton->IsHighlighted())
				pDC->Draw3dRect(rcItem, m_crBorderActive, m_crBorderActive);
		}
		return TRUE;
	}

	return FALSE;
}

void CXTPTaskButtonTheme::DrawButtonIcon(CDC* pDC, CXTPButton* pButton)
{
	CXTPImageManagerIcon* pIcon = pButton->GetIcon();
	if (pIcon != NULL)
	{
		CXTPFontDC fontDC(pDC, m_pFontTitle ? m_pFontTitle : pButton->GetFont());
		CSize size = pDC->GetTextExtent(_T(" "), 1);
		CPoint point(7, m_ptMargin.y - pIcon->GetHeight() / 2 + size.cy / 2);

		if (!pButton->IsWindowEnabled())
			pIcon->Draw(pDC, point, pIcon->GetDisabledIcon(), pButton->GetImageSize());

		else if (pButton->IsHighlighted())
			pIcon->Draw(pDC, point, pIcon->GetHotIcon(), pButton->GetImageSize());

		else
			pIcon->Draw(pDC, point, pIcon->GetIcon(), pButton->GetImageSize());
	}
}

CPoint CXTPTaskButtonTheme::GetTextPosition(CDC* /*pDC*/, CSize sizeText, CXTPButton* pButton)
{
	CXTPClientRect rcItem(pButton);

	CPoint point;
	point.x = m_ptMargin.x;
	point.y = rcItem.top + (rcItem.Height()-sizeText.cy)/2;

	if (pButton->GetIcon())
		point.x += pButton->GetImageSize().cx;

	return point;
}

void CXTPTaskButtonTheme::DrawButtonText(CDC* pDC, CXTPButton* pButton)
{
	if (m_strButton.IsEmpty())
		GetButtonText(pButton);


	CXTPClientRect rcItem(pButton);

	CRect rcText = rcItem;
	rcText.DeflateRect(m_ptMargin.x, m_ptMargin.y);

	if (pButton->GetIcon())
		rcText.left += pButton->GetImageSize().cx;

	CFont* pFontNote = pButton->GetFont();

	CSize sizeNoteText = rcText.Size();
	XTPCalcTextSize(m_strNoteText, sizeNoteText, *pFontNote);

	CSize sizeCaptText = rcText.Size();
	XTPCalcTextSize(m_strCaptText, sizeCaptText, m_pFontTitle ? *m_pFontTitle : *pFontNote);

	if (pButton->GetMarkupUIElement())
	{
		XTPMarkupSetDefaultFont(pButton->GetMarkupContext(), m_pFontTitle ? (HFONT)m_pFontTitle->GetSafeHandle(): (HFONT)pFontNote->GetSafeHandle(), COLORREF_NULL);

		sizeCaptText = XTPMarkupMeasureElement(pButton->GetMarkupUIElement(), rcText.Width(), INT_MAX);
	}

	// get the text size.
	CXTPEmptySize sizeText;
	sizeText.cy = sizeCaptText.cy + sizeNoteText.cy;
	sizeText.cx = __max(sizeCaptText.cx, sizeNoteText.cx);

	CPoint point = GetTextPosition(pDC, sizeText, pButton);

	BOOL  bSelected = pButton->GetChecked() || pButton->IsPushed();
	if (bSelected && m_bOffsetHiliteText)
		point.Offset(1,1);

	// Set the draw state flags.
	pDC->SetTextColor(GetTextColor(pButton));
	pDC->SetTextAlign(TA_LEFT);
	pDC->SetBkMode(TRANSPARENT);

	if (!m_strCaptText.IsEmpty())
	{
		CRect rcButton(point, sizeCaptText);

		if (pButton->GetMarkupUIElement())
		{
			XTPMarkupSetDefaultFont(pButton->GetMarkupContext(), m_pFontTitle ? (HFONT)m_pFontTitle->GetSafeHandle(): (HFONT)pFontNote->GetSafeHandle(), pDC->GetTextColor());

			XTPMarkupRenderElement(pButton->GetMarkupUIElement(), pDC->GetSafeHdc(), rcButton);
		}
		else
		{
			CString strCaptText = m_strCaptText;

			if (pButton->SendMessage(WM_QUERYUISTATE) & UISF_HIDEACCEL)
			{
				XTPDrawHelpers()->StripMnemonics(strCaptText);
			}

			CXTPFontDC fontDC(pDC, m_pFontTitle ? m_pFontTitle : pFontNote);

			pDC->DrawText(strCaptText, &rcButton, DT_LEFT | DT_TOP | DT_WORDBREAK);
		}

		point.y += sizeCaptText.cy + 1;
	}

	if (!m_strNoteText.IsEmpty())
	{
		CString strNoteText = m_strNoteText;

		CXTPFontDC fontDC(pDC, pFontNote);
		CRect rcButton(point, sizeNoteText);
		pDC->DrawText(strNoteText, &rcButton, DT_LEFT | DT_TOP | DT_WORDBREAK | DT_NOPREFIX);
	}
}

void CXTPTaskButtonTheme::GetButtonText(CXTPButton* pButton)
{
	if (::IsWindow(pButton->GetSafeHwnd()))
	{
		pButton->GetWindowText(m_strButton);
		m_strCaptText = XTPExtractSubString(m_strButton, 0);
		m_strNoteText = XTPExtractSubString(m_strButton, 1);
	}
}

void CXTPTaskButtonTheme::SetTitleFont(CFont* pFontTitle)
{
	m_pFontTitle = pFontTitle;
}

CFont* CXTPTaskButtonTheme::GetTitleFont()
{
	return m_pFontTitle;
}

void CXTPTaskButtonTheme::RefreshMetrics(CXTPButton* pButton)
{
	CXTPButtonTheme::RefreshMetrics(pButton);

	if (m_bThemeReady)
	{
		COLORREF crWindow = GetXtremeColor(COLOR_WINDOW);
		COLORREF cr3dFace = GetXtremeColor(COLOR_3DFACE);
		COLORREF cr3dShad = GetXtremeColor(COLOR_3DSHADOW);

		m_crBack           = crWindow;
		m_crText           = RGB(21,28,85);
		m_crTextHilite     = RGB(0x00, 0x4a, 0xe5);
		m_crBorderActive   = RGB(0xa3, 0xed, 0xff);

		m_crBackSelected   = XTPColorManager()->LightColor(crWindow, cr3dFace, 200);
		m_crBorder3DShadow = XTPColorManager()->LightColor(cr3dFace, cr3dShad, 775);
		m_crBorder3DHilite = XTPColorManager()->LightColor(cr3dFace, cr3dShad, 875);
		m_crBorderShadow   = XTPColorManager()->LightColor(crWindow, cr3dFace, 0);
		m_crBorderHilite   = XTPColorManager()->LightColor(cr3dFace, cr3dShad, 175);
	}
	else
	{
		m_crBorderActive = m_crBorder3DShadow;
	}
}

COLORREF CXTPTaskButtonTheme::GetTextColor(CXTPButton* pButton)
{
	if (!pButton->IsWindowEnabled())
		return GetXtremeColor(COLOR_GRAYTEXT);

	if (pButton->IsHighlighted())
		return m_crTextHilite;

	return m_crText;
}

/*
CPoint CXTPTaskButtonTheme::CalculateImagePosition(CDC* pDC, UINT nState, CRect& rcItem, bool bHasPushedImage, CXTPButton* pButton)
{
	//CPoint point = CXTPButtonTheme::CalculateImagePosition(pDC, nState, rcItem, bHasPushedImage, pButton);
	CPoint point = 0;

	if (!m_strNoteText.IsEmpty())
		point.y = m_ptMargin.y;

	point.x += 2;
	return point;
}*/

//===========================================================================
// CXTPTaskButtonThemeExpando
//===========================================================================


CXTPTaskButtonThemeExpando::CXTPTaskButtonThemeExpando()
{
	m_bOffsetHiliteText = FALSE;
}

void CXTPTaskButtonThemeExpando::DrawButton(CDC* pDC, CXTPButton* pButton)
{
	// define some temporary variables.
	CXTPClientRect rcItem(pButton);

	pDC->FillSolidRect(rcItem, GetXtremeColor(COLOR_3DFACE));

	// Set the background mode to transparent.
	pDC->SetBkColor(GetXtremeColor(COLOR_3DFACE));


	// select font into device context.
	CXTPFontDC fontDC(pDC, pButton->GetFont());

	CString strText;
	pButton->GetWindowText(strText);

	// Remove ampersand
	if (pButton->SendMessage(WM_QUERYUISTATE) & UISF_HIDEACCEL)
	{
		XTPDrawHelpers()->StripMnemonics(strText);
	}

	CRect rcText(rcItem);
	rcText.left += 26;

	CSize szExtent = pDC->GetTextExtent(strText);
	if (szExtent.cx < rcItem.Width())
	{
		rcText.top = (rcItem.top + rcItem.bottom - szExtent.cy) / 2;
		rcText.bottom = rcText.top + szExtent.cy;
	}

	pDC->SetTextColor(GetXtremeColor(COLOR_BTNTEXT));
	pDC->DrawText(strText, rcText, DT_TOP | DT_WORDBREAK);

	// Draw the focus rect if style is set and we have focus.
	if (::GetFocus() == pButton->GetSafeHwnd() && ((pButton->SendMessage(WM_QUERYUISTATE) & UISF_HIDEFOCUS) == 0))
	{
		pDC->DrawText(strText, rcText, DT_TOP | DT_WORDBREAK | DT_CALCRECT);
		rcText.InflateRect(1, 0);
		pDC->DrawFocusRect(rcText);
	}

	rcItem.right = rcItem.left + 18;
	rcItem.bottom = rcItem.top + min(20, rcItem.Height());

	CPoint pt(rcItem.right / 2 - 1, rcItem.bottom / 2 - 3);

	BOOL bDrawStandard = TRUE;


	if (m_themeButton->IsAppThemed())
	{
		CXTPWinThemeWrapper themeTaskDialog;
		themeTaskDialog.OpenTheme(0, L"TASKDIALOG");

		if (themeTaskDialog.IsAppThemed())
		{
			CRect rcButton(0, 0, 19, 21);

			int iStateId = pButton->IsPushed() ? 3 : pButton->IsHighlighted() ? 2 : 1;
			if (pButton->GetChecked()) iStateId += 3;

			if (SUCCEEDED(themeTaskDialog.DrawThemeBackground(pDC->GetSafeHdc(),
				13, iStateId, &rcButton, NULL)))
			{
				return;
			}
		}

		CRect rc(rcItem);
		rc.right += 1;

		int iStateId = pButton->IsPushed() ? PBS_PRESSED : pButton->IsHighlighted() ? PBS_HOT : PBS_NORMAL;

		if (SUCCEEDED(m_themeButton->DrawThemeBackground(pDC->GetSafeHdc(),
			BP_PUSHBUTTON, iStateId, &rc, NULL)))
		{
			bDrawStandard = FALSE;
			pt.x += 1;
		}
	}

	if (bDrawStandard)
	{
		pDC->DrawFrameControl(&rcItem, DFC_BUTTON, (pButton->IsPushed() ? DFCS_PUSHED : 0) | DFCS_BUTTONPUSH);
		if (pButton->IsPushed()) pt.Offset(1, 1);
	}

	if (!pButton->GetChecked())
		XTPDrawHelpers()->Triangle(pDC, pt - CPoint(4, 0),
			pt + CPoint(4, 0), pt + CPoint(0, 4), GetXtremeColor(COLOR_BTNTEXT));
	else
		XTPDrawHelpers()->Triangle(pDC, pt - CPoint(4, -4),
			pt + CPoint(4, 4), pt + CPoint(0, 0), GetXtremeColor(COLOR_BTNTEXT));
}

//////////////////////////////////////////////////////////////////////////
// CXTPTaskDialogProgressCtrl

#ifndef PBS_MARQUEE
#define PBS_MARQUEE             0x08
#endif

#ifndef PBM_SETMARQUEE
#define PBM_SETMARQUEE          (WM_USER+10)
#endif

#ifndef PBM_SETSTATE
#define PBM_SETSTATE            (WM_USER+16)
#endif

CXTPTaskDialogProgressCtrl::CXTPTaskDialogProgressCtrl()
{
	m_nState = -1;
	m_nMarqueePos = 0;
	m_bMarquee = FALSE;
	m_nMarqueeDelay = 0;
}

BEGIN_MESSAGE_MAP(CXTPTaskDialogProgressCtrl, CProgressCtrl)
	//{{AFX_MSG_MAP(CXTPTaskDialogProgressCtrl)
	ON_WM_PAINT()
	ON_MESSAGE(PBM_SETMARQUEE, OnStartMarquee)
	ON_MESSAGE(PBM_SETSTATE, OnSetState)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CXTPTaskDialogProgressCtrl::OnPaint()
{
	if (HIWORD(XTPSystemVersion()->GetComCtlVersion()) > 5)
	{
		Default();
		return;
	}

	if (((GetStyle() & PBS_MARQUEE) == 0) || !m_bMarquee)
	{
		Default();
		return;
	}

	BOOL bVista = XTPSystemVersion()->IsWinVistaOrGreater();

	CPaintDC dcPaint(this);
	CXTPBufferDC dc(dcPaint);

	CXTPClientRect rcClient(this);
	dc.FillSolidRect(rcClient, GetXtremeColor(COLOR_3DFACE));

	if (m_nState == PBST_ERROR || m_nState == PBST_PAUSED)
		return;

	rcClient.DeflateRect(1, 1);
	CRect rc(rcClient);

	int dxBlock = rc.Height() * 2 / 3;
	int nSpace = bVista ? 0 : 2;
	rc.left = m_nMarqueePos + 1;

	for (int i = 0; i < 5; i++)
	{
		if (rc.left >= rcClient.right)
			rc.left = 1;

		rc.right = rc.left + dxBlock;

		if (rc.right > rcClient.right)
			rc.right = rcClient.right;

		dc.FillSolidRect(rc, GetXtremeColor(COLOR_HIGHLIGHT));
		rc.left = rc.right + nSpace;
	}
}

LRESULT CXTPTaskDialogProgressCtrl::OnSetState(WPARAM wParam, LPARAM /*lParam*/)
{
	m_nState = (int)wParam;
	Default();

	return 1;
}

LRESULT CXTPTaskDialogProgressCtrl::OnStartMarquee(WPARAM wParam, LPARAM lParam)
{
	m_bMarquee = FALSE;
	m_nMarqueePos = 0;
	m_nMarqueeDelay = 0;

	if (HIWORD(XTPSystemVersion()->GetComCtlVersion()) > 5)
		return Default();

	if (((GetStyle() & PBS_MARQUEE) == 0))
		return Default();

	m_bMarquee = wParam && (lParam > 0);
	m_nMarqueePos = 0;

	KillTimer(101);

	if (m_bMarquee)
	{
		m_nMarqueeDelay = (UINT)lParam;
		SetTimer(101, m_nMarqueeDelay, NULL);
	}

	return 1;
}

void CXTPTaskDialogProgressCtrl::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 101 && m_bMarquee)
	{
		CXTPClientRect rcClient(this);

		BOOL bVista = XTPSystemVersion()->IsWinVistaOrGreater();
		int dxBlock = (rcClient.Height() - 2) * 2 / 3;
		int nSpace = bVista ? 0 : 2;

		m_nMarqueePos += (dxBlock + nSpace) * (m_nMarqueeDelay > 50 ? 1 : 2);

		if (m_nMarqueePos > rcClient.right)
			m_nMarqueePos = 0;

		Invalidate(FALSE);

		return;
	}

	CWnd::OnTimer(nIDEvent);
}
