// XTPShortcutListBox.cpp : implementation of the CXTPShortcutListBox class.
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
#include "Common/XTPColorManager.h"
#include "Common/XTPImageManager.h"
#include "Common/XTPDrawHelpers.h"
#include "../Util/XTPControlTheme.h"

#include "../Defines.h"
#include "XTPShortcutListBox.h"
#include "XTPShortcutListBoxTheme.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPShortcutListBox::CONTENT_ITEM

CXTPShortcutListBox::CONTENT_ITEM::CONTENT_ITEM()
{
	m_pIcon = NULL;
}

CXTPShortcutListBox::CONTENT_ITEM::~CONTENT_ITEM()
{
	if (m_pIcon)
		delete m_pIcon;
}

/////////////////////////////////////////////////////////////////////////////
// CXTPShortcutListBox
/////////////////////////////////////////////////////////////////////////////

CXTPShortcutListBox::CXTPShortcutListBox()
: m_bNoBorder(false)
, m_bHilight(true)
, m_bNoUnderline(true)
, m_hCursor(NULL)
, m_bHilightItemID(-1)
, m_bTimerActive(false)
, m_nPrevIndex(-1)
, m_pTheme(NULL)
{
	VERIFY(SetTheme(xtpControlThemeDefault));
}

CXTPShortcutListBox::~CXTPShortcutListBox()
{
	CMDTARGET_RELEASE(m_pTheme);

	while (!m_arContentItems.IsEmpty())
	{
		CONTENT_ITEM* pCI = m_arContentItems.RemoveHead();
		delete pCI;
	}
}


IMPLEMENT_DYNAMIC(CXTPShortcutListBox, CListBox)

BEGIN_MESSAGE_MAP(CXTPShortcutListBox, CListBox)
	//{{AFX_MSG_MAP(CXTPShortcutListBox)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_SETCURSOR()
	ON_MESSAGE_VOID(WM_MOUSELEAVE, OnMouseLeave)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_XTP_SETCONTROLTHEME, OnSetTheme)
END_MESSAGE_MAP()

void CXTPShortcutListBox::RefreshMetrics()
{
	if (m_pTheme)
		m_pTheme->RefreshMetrics(this);

	if (::IsWindow(m_hWnd))
		RedrawWindow();
}

BOOL CXTPShortcutListBox::SetTheme(XTPControlTheme eTheme)
{
	CMDTARGET_RELEASE(m_pTheme);

	switch (eTheme)
	{
		case xtpControlThemeOfficeXP:
			m_pTheme = new CXTPShortcutListBoxThemeOfficeXP;
			break;

		case xtpControlThemeOffice2003:
			m_pTheme = new CXTPShortcutListBoxThemeOffice2003;
			break;

		default:
			m_pTheme = new CXTPShortcutListBoxTheme;
			break;
	}

	RefreshMetrics();

	return (m_pTheme != NULL);
}
LRESULT CXTPShortcutListBox::OnSetTheme(WPARAM wParam, LPARAM /*lParam*/)
{
	XTPControlTheme eTheme = (XTPControlTheme)wParam;

	SetTheme(eTheme);

	return 0;
}

BOOL CXTPShortcutListBox::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	if (message == WM_SETTINGCHANGE || message == WM_SYSCOLORCHANGE)
	{
		RefreshMetrics();
	}

	return CListBox::OnWndMsg(message, wParam, lParam, pResult);
}

// Checks mouse movement, sets point member, sets timer.

BOOL CXTPShortcutListBox::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	if (m_hCursor != NULL)
	{
		CONTENT_ITEM* pCI = GetMenuItem(m_nPrevIndex);
		if (pCI && pCI->m_bEnabled)
		{
			SetCursor(m_hCursor);
			return TRUE;
		}
	}

	return CListBox::OnSetCursor(pWnd, nHitTest, message);
}

void CXTPShortcutListBox::RedrawItem(int iIndex)
{
	CRect rcItem;
	GetItemRect(iIndex, &rcItem);
	InvalidateRect(&rcItem);
}

void CXTPShortcutListBox::RedrawItem(CPoint point)
{
	BOOL bOutSide;
	int nIndex = ItemFromPoint(point, bOutSide);

	if (!bOutSide)
	{
		RedrawItem(nIndex);
	}
}

void CXTPShortcutListBox::OnMouseMove(UINT /*nFlags*/, CPoint point)
{
	BOOL bOutSide;
	int nIndex = ItemFromPoint (point, bOutSide);

	CRect rcItem;
	GetItemRect(nIndex, &rcItem);

	if ((m_nPrevIndex != -1) && (m_nPrevIndex != nIndex))
	{
		RedrawItem(m_nPrevIndex);
	}

	if (bOutSide || !rcItem.PtInRect(point))
	{
		RedrawItem(nIndex);
		nIndex = -1;
	}

	if (nIndex != GetCurSel())
	{
		SetCurSel(nIndex);

		if (nIndex != -1)
		{
			RedrawItem(nIndex);

			TRACKMOUSEEVENT tme =
			{
				sizeof(TRACKMOUSEEVENT), TME_LEAVE, m_hWnd, 0
			};
			_TrackMouseEvent(&tme);
		}
	}

	m_nPrevIndex = nIndex;
}

void CXTPShortcutListBox::OnLButtonDown(UINT nFlags, CPoint point)
{
	CListBox::OnLButtonDown(nFlags, point);
	RedrawItem(point);
}

void CXTPShortcutListBox::OnLButtonUp(UINT nFlags, CPoint point)
{
	CListBox::OnLButtonUp(nFlags, point);
	RedrawItem(point);

	CONTENT_ITEM* pCI = GetMenuItem(GetCurSel());
	if (pCI && !pCI->m_bEnabled)
		return;

	CWnd* pWndOwner = GetOwner();
	ASSERT(pWndOwner);

	if (pWndOwner) pWndOwner->SendMessage(WM_XTP_SHORTCUTLBOX_NOTIFY,
		(LPARAM)GetCurSel(), (WPARAM)GetDlgCtrlID());
}

// Sets the highlight flag based on cursor position.

void CXTPShortcutListBox::OnMouseLeave()
{
	//reset mouse vars
	OnMouseMove(0, CPoint(-1, -1));
}

// Initial draw routine, draws button on mouse over,
// on mouse press, and on mouse out.

void CXTPShortcutListBox::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
	GetTheme()->DrawItem(this, lpDIS);
}

int CXTPShortcutListBox::GetTextHeight(LPCTSTR lspzItem) const
{
	CWindowDC dc(NULL);

	CString strItem(lspzItem);

	// get the height for a single line text item.
	CFont* pOldFont = dc.SelectObject(GetFont());
	CSize size = dc.GetTextExtent(strItem);
	dc.SelectObject(pOldFont);

	// set the height of the item with single line text.
	int iTextHeight = size.cy;

	// for each newline character we need to adjust the height.
	for (int i = 0; i < strItem.GetLength(); ++i)
	{
		if (strItem[i] == _T('\n'))
		{
			iTextHeight += size.cy;
		}
	}

	return iTextHeight;
}

// Sets item height

void CXTPShortcutListBox::MeasureItem(LPMEASUREITEMSTRUCT lpMIS)
{
	// get the text height.
	int iTextHeight = GetTextHeight((LPCTSTR)lpMIS->itemData);

	// calculate the total height of the item.
	lpMIS->itemHeight = GetTheme()->m_cyEdge + GetTheme()->m_cyIcon + GetTheme()->m_cyEdge +
		iTextHeight + GetTheme()->m_cyEdge;
}

void CXTPShortcutListBox::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos)
{
	CListBox::OnWindowPosChanged(lpwndpos);
	InvalidateRect(NULL);
}

BOOL CXTPShortcutListBox::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	// Make sure that the control is owner drawn.
	dwStyle |= LBS_OWNERDRAWVARIABLE | LBS_NOINTEGRALHEIGHT;

	if (!CListBox::Create(dwStyle, rect, pParentWnd, nID))
		return FALSE;

	// Set the font used by the Outlook bar.
	SetFont(CFont::FromHandle((HFONT)
		::GetStockObject(DEFAULT_GUI_FONT)));

	return TRUE;
}

BOOL CXTPShortcutListBox::EnableMenuItem(int iItem, BOOL bEnabled)
{
	CONTENT_ITEM* pCI = GetMenuItem(iItem);
	if (pCI != NULL)
	{
		pCI->m_bEnabled = bEnabled;
		return TRUE;
	}
	return FALSE;
}

int CXTPShortcutListBox::AddMenuItem(UINT nIconID, LPCTSTR lpszText, BOOL bEnabled/*= TRUE*/)
{
	CONTENT_ITEM* pCI = new CONTENT_ITEM;
	ASSERT(pCI != NULL);

	pCI->m_nIndex = AddString(lpszText);
	pCI->m_strText = lpszText;
	pCI->m_bEnabled = bEnabled;

	/*// Load the icon for the menu item.
	HINSTANCE hInst = AfxFindResourceHandle(MAKEINTRESOURCE(nIconID), RT_GROUP_ICON);
	pCI->m_hIcon = (HICON)::LoadImage(hInst, MAKEINTRESOURCE(nIconID),
		IMAGE_ICON, GetTheme()->m_cxIcon, GetTheme()->m_cyIcon, LR_DEFAULTCOLOR);*/

	pCI->m_pIcon = new CXTPImageManagerIcon(nIconID, 0, 0);
	pCI->m_pIcon->SetIcon(nIconID, GetTheme()->m_cxIcon, GetTheme()->m_cyIcon);

	m_arContentItems.AddTail(pCI);

	return pCI->m_nIndex;
}

int CXTPShortcutListBox::InsertMenuItem(int iIndex, UINT nIconID, LPCTSTR lpszText, BOOL bEnabled/*= TRUE*/)
{
	CONTENT_ITEM* pCI = new CONTENT_ITEM;
	ASSERT(pCI != NULL);

	pCI->m_nIndex = InsertString(iIndex, lpszText);
	pCI->m_strText = lpszText;
	pCI->m_bEnabled = bEnabled;

	// Load the icon for the menu item.
//  HINSTANCE hInst = AfxFindResourceHandle(MAKEINTRESOURCE(nIconID), RT_GROUP_ICON);
//  pCI->m_hIcon = (HICON)::LoadImage(hInst, MAKEINTRESOURCE(nIconID),
//      IMAGE_ICON, GetTheme()->m_cxIcon, GetTheme()->m_cyIcon, LR_DEFAULTCOLOR);
	pCI->m_pIcon = new CXTPImageManagerIcon(nIconID, 0, 0);
	pCI->m_pIcon->SetIcon(nIconID, GetTheme()->m_cxIcon, GetTheme()->m_cyIcon);

	POSITION pos = m_arContentItems.FindIndex(pCI->m_nIndex);
	if (pos != NULL)
	{
		m_arContentItems.InsertBefore(pos, pCI);
	}
	else
	{
		m_arContentItems.AddTail(pCI);
	}

	return pCI->m_nIndex;
}

BOOL CXTPShortcutListBox::OnEraseBkgnd(CDC* /*pDC*/)
{
	return FALSE;
}

void CXTPShortcutListBox::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	// Get the client rect.
	CXTPClientRect rcClient(this);

	// Paint to a memory device context to help
	// eliminate screen flicker.
	CXTPBufferDC memDC(dc);
	memDC.FillSolidRect(rcClient, GetTheme()->m_crgBack);

	// Now let the window do its default painting...
	CWnd::DefWindowProc (WM_PAINT, (WPARAM)memDC.m_hDC, 0);
}

CXTPShortcutListBox::CONTENT_ITEM* CXTPShortcutListBox::GetMenuItem(int iItem)
{
	CONTENT_ITEM* pCI = NULL;

	// Get the menu item.
	POSITION pos = m_arContentItems.FindIndex(iItem);
	if (pos != NULL)
	{
		pCI = m_arContentItems.GetAt(pos);
	}

	return pCI;
}

void CXTPShortcutListBox::SetHilightCursor(UINT nIDCursor, bool bNoBorder/*= false*/, bool bNoUnderline/*= false*/)
{
	// load the new cursor.
	m_hCursor = AfxGetApp()->LoadCursor(nIDCursor);

	// display a border or text underline when highlighted ?
	m_bNoBorder = bNoBorder;
	m_bNoUnderline = bNoUnderline;
}

void CXTPShortcutListBox::SetColors(COLORREF clrText, COLORREF clrBack, COLORREF clrBackDark/*= COLORREF_NULL*/)
{
	GetTheme()->m_crItemText.SetCustomValue(clrText);  // Must be custom value used to prevent reset color in RefreshMetrics.
	GetTheme()->m_crgBack.SetCustomValue(clrBack, clrBackDark);
}
