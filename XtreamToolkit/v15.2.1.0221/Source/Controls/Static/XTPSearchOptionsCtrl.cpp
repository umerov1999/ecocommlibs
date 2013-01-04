// CXTPSearchOptionsCtrl : implementation file
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
#include "Common/XTPDrawHelpers.h"

#include "../Defines.h"
#include "../Util/XTPGlobal.h"
#include "../Resize/XTPResizeRect.h"
#include "../Resize/XTPResizePoint.h"
#include "XTPSearchOptionsCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTPSearchOptionsCtrl

CXTPSearchOptionsCtrl::CXTPSearchOptionsCtrl()
{
	m_strExpandLabel = _T("");
	m_strContractLabel = _T("");
	m_iMinSize = ::GetSystemMetrics(SM_CYMENU);
	m_iMaxSize = 0;
	m_bExpanded = true;
	m_bPreSubclassInit = true;
}

CXTPSearchOptionsCtrl::~CXTPSearchOptionsCtrl()
{
}

IMPLEMENT_DYNAMIC(CXTPSearchOptionsCtrl, CStatic)

BEGIN_MESSAGE_MAP(CXTPSearchOptionsCtrl, CStatic)
	//{{AFX_MSG_MAP(CXTPSearchOptionsCtrl)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_SETCURSOR()
	ON_WM_CREATE()
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_ENABLE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTPSearchOptionsCtrl message handlers

void CXTPSearchOptionsCtrl::OnEnable(BOOL bEnable)
{
	CStatic::OnEnable(bEnable);

	// enable/disable each of our windows in the hide controls array.
	int iIndex;
	for (iIndex = 0; iIndex < m_arHideCtrls.GetSize(); ++iIndex)
	{
		CWnd* pWndCtrl = (CWnd*)m_arHideCtrls.GetAt(iIndex);
		if (::IsWindow(pWndCtrl->GetSafeHwnd()))
		{
			pWndCtrl->EnableWindow(bEnable);
		}
	}

	// enable/disable each of the controls in our move controls array.
	for (iIndex = 0; iIndex < m_arMoveCtrls.GetSize(); ++iIndex)
	{
		CWnd* pWndCtrl = (CWnd*)m_arMoveCtrls.GetAt(iIndex);
		if (::IsWindow(pWndCtrl->GetSafeHwnd()))
		{
			pWndCtrl->EnableWindow(bEnable);
		}
	}
}

void CXTPSearchOptionsCtrl::AddControl(CWnd* pWndCtrl)
{
	ASSERT_VALID(pWndCtrl); // must be valid.
	m_arHideCtrls.Add(pWndCtrl);
}

void CXTPSearchOptionsCtrl::MoveControl(CWnd* pWndCtrl)
{
	ASSERT_VALID(pWndCtrl); // must be valid.
	m_arMoveCtrls.Add(pWndCtrl);
}

void CXTPSearchOptionsCtrl::SetLabelText(LPCTSTR lpszExpand, LPCTSTR lpszContract)
{
	m_strExpandLabel = lpszExpand;
	m_strContractLabel = lpszContract;
}

void CXTPSearchOptionsCtrl::Expand()
{
	// set our expanded flag to true.
	m_bExpanded = true;

	CWnd* pOwner = GetOwner();
	ASSERT_VALID(pOwner);

	// notify owner of expanding, this is sent prior to our
	// adjusting any controls to allow the parent to prepare
	// itself for resizing.

	if (pOwner != NULL)
	{
		pOwner->SendMessage(WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(),
			SON_XTP_ITEMEXPANDING), (LPARAM)m_hWnd);
	}

	// expand this window.
	CRect rect;
	GetWindowRect(&rect);
	GetParent()->ScreenToClient(&rect);
	rect.bottom = rect.top + m_iMaxSize;
	MoveWindow(&rect);

	// show each of our windows in the hide controls array.
	int iIndex;
	for (iIndex = 0; iIndex < m_arHideCtrls.GetSize(); ++iIndex)
	{
		CWnd* pWndCtrl = (CWnd*)m_arHideCtrls.GetAt(iIndex);
		ASSERT_VALID(pWndCtrl); // must be valid.
		pWndCtrl->ShowWindow(SW_SHOW);
	}

	// move each of the controls in our move controls array.
	for (iIndex = 0; iIndex < m_arMoveCtrls.GetSize(); ++iIndex)
	{
		CWnd* pWndCtrl = (CWnd*)m_arMoveCtrls.GetAt(iIndex);
		ASSERT_VALID(pWndCtrl); // must be valid.

		CWnd* pParentWnd = pWndCtrl->GetParent();
		ASSERT_VALID(pParentWnd);

		CRect rc;
		pWndCtrl->GetWindowRect(&rc);
		pParentWnd->ScreenToClient(&rc);

		int iOffset = m_iMaxSize-m_iMinSize;
		rc.top += iOffset;
		rc.bottom += iOffset;

		// move the control and repaint it.
		pWndCtrl->MoveWindow(&rc);
		pWndCtrl->InvalidateRect(NULL);
	}

	// notify owner that expand has completed.

	if (pOwner != NULL)
	{
		pOwner->SendMessage(WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(),
			SON_XTP_ITEMEXPAND), (LPARAM)m_hWnd);
	}
}

void CXTPSearchOptionsCtrl::Contract()
{
	// set our expanded flag to false.
	m_bExpanded = false;

	CWnd* pOwner = GetOwner();
	ASSERT_VALID(pOwner);

	// notify owner of contracting, this is sent prior to our
	// adjusting any controls to allow the parent to prepare
	// itself for resizing.

	if (pOwner != NULL)
	{
		pOwner->SendMessage(WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(),
			SON_XTP_ITEMCONTRACTING), (LPARAM)m_hWnd);
	}

	// contract this window.
	CRect rect;
	GetWindowRect(&rect);

	if (m_iMaxSize == 0)
		m_iMaxSize = rect.Height();

	GetParent()->ScreenToClient(&rect);
	rect.bottom = rect.top + m_iMinSize;
	MoveWindow(&rect);

	// hide each of our windows in the hide controls array.
	int iIndex;
	for (iIndex = 0; iIndex < m_arHideCtrls.GetSize(); ++iIndex)
	{
		CWnd* pWndCtrl = (CWnd*)m_arHideCtrls.GetAt(iIndex);
		ASSERT_VALID(pWndCtrl); // must be valid.
		pWndCtrl->ShowWindow(SW_HIDE);
	}

	// move each of the controls in our move controls array.
	for (iIndex = 0; iIndex < m_arMoveCtrls.GetSize(); ++iIndex)
	{
		CWnd* pWndCtrl = (CWnd*)m_arMoveCtrls.GetAt(iIndex);
		ASSERT_VALID(pWndCtrl); // must be valid.

		CWnd* pParentWnd = pWndCtrl->GetParent();
		ASSERT_VALID(pParentWnd);

		CRect rc;
		pWndCtrl->GetWindowRect(&rc);
		pParentWnd->ScreenToClient(&rc);

		int iOffset = m_iMaxSize-m_iMinSize;
		rc.top -= iOffset;
		rc.bottom -= iOffset;

		// move the control and repaint it.
		pWndCtrl->MoveWindow(&rc);
		pWndCtrl->InvalidateRect(NULL);
	}

	// notify owner that contracting has completed.

	if (pOwner != NULL)
	{
		pOwner->SendMessage(WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(),
			SON_XTP_ITEMCONTRACT), (LPARAM)m_hWnd);
	}
}

BOOL CXTPSearchOptionsCtrl::OnEraseBkgnd(CDC* /*pDC*/)
{
	return TRUE;
}

void CXTPSearchOptionsCtrl::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect rcClient;
	GetClientRect(&rcClient);

	// exclude the windows in our "hide list" from paint routines.
	int iIndex;
	for (iIndex = 0; iIndex < m_arHideCtrls.GetSize(); ++iIndex)
	{
		CWnd* pWndCtrl = (CWnd*)m_arHideCtrls.GetAt(iIndex);
		ASSERT_VALID(pWndCtrl); // must be valid.

		CRect rcHide;
		pWndCtrl->GetWindowRect(&rcHide);
		ScreenToClient(&rcHide);

		dc.ExcludeClipRect(&rcHide);
	}

	// paint off screen.
	CXTPBufferDC memDC(dc);

	HBRUSH hBrush = (HBRUSH)GetParent()->SendMessage(WM_CTLCOLORSTATIC, (WPARAM)memDC.GetSafeHdc(), (LPARAM)GetSafeHwnd());
	if (hBrush)
	{
		::FillRect(memDC.GetSafeHdc(), rcClient, hBrush);
	}
	else
	{
		memDC.FillSolidRect(rcClient, GetXtremeColor(COLOR_WINDOW));
	}


	BOOL bIsEnabled = IsWindowEnabled();

	memDC.SetBkMode(TRANSPARENT);

	CFont* pOldFont = 0;

	// construct the font's attributes
	if (m_bExpanded)
	{
		pOldFont = memDC.SelectObject(&XTPAuxData().font);
		memDC.SetTextColor(bIsEnabled ? GetXtremeColor(COLOR_HIGHLIGHTTEXT) :
			GetXtremeColor(COLOR_GRAYTEXT));

		// if we are expanded, draw a border and fill caption.
		memDC.Draw3dRect(rcClient, GetXtremeColor(COLOR_HIGHLIGHT),
			GetXtremeColor(COLOR_HIGHLIGHT));

		CRect rcCaption(rcClient);
		rcCaption.bottom = rcCaption.top + m_iMinSize;
		rcCaption.top += 1;
		rcCaption.bottom -= 1;
		memDC.FillSolidRect(rcCaption, GetXtremeColor(COLOR_HIGHLIGHT));
	}
	else
	{
		pOldFont = memDC.SelectObject(&XTPAuxData().fontULine);
		memDC.SetTextColor(bIsEnabled ? GetXtremeColor(COLOR_HIGHLIGHT) :
			GetXtremeColor(COLOR_GRAYTEXT));
	}

	// get the size of the label's text.
	CSize size = memDC.GetTextExtent(m_bExpanded ? m_strExpandLabel : m_strContractLabel);

	// construct the "hot" area used for mouse activation.
	GetWindowRect(&m_rcLabel);
	m_rcLabel.bottom = m_rcLabel.top + m_iMinSize;

	if (m_bExpanded)
		m_rcLabel.left += 3;
	else
		m_rcLabel.right = m_rcLabel.left + size.cx;

	// paint the label.
	ScreenToClient(&m_rcLabel);
	memDC.DrawText(m_bExpanded ? m_strExpandLabel : m_strContractLabel,
		&m_rcLabel, DT_SINGLELINE | DT_LEFT | DT_VCENTER);

	memDC.SelectObject(pOldFont);

}

void CXTPSearchOptionsCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_rcLabel.PtInRect(point))
	{
		if (m_bExpanded)
			Contract();
		else
			Expand();

		Invalidate();
	}

	CStatic::OnLButtonDown(nFlags, point);
}

BOOL CXTPSearchOptionsCtrl::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// get the cursor position in client coordinates.
	CPoint point;
	::GetCursorPos(&point);
	ScreenToClient(&point);

	// if the cursor is over the label set the hand cursor.
	if (m_rcLabel.PtInRect(point))
	{
		::SetCursor(XTPAuxData().hcurHand);
		return TRUE;
	}

	return CStatic::OnSetCursor(pWnd, nHitTest, message);
}

bool CXTPSearchOptionsCtrl::Init()
{
	if (::IsWindow(m_hWnd))
	{
		if (m_iMaxSize == 0)
		{
			CRect rect;
			GetWindowRect(&rect);
			m_iMaxSize = rect.Height();
		}

		// update display.
		RedrawWindow();
		return true;
	}

	return false;
}

void CXTPSearchOptionsCtrl::PreSubclassWindow()
{
	CStatic::PreSubclassWindow();

	if (m_bPreSubclassInit)
	{
		// Initialize the control.
		Init();
	}
}

int CXTPSearchOptionsCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CStatic::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Initialize the control.
	Init();

	return 0;
}

BOOL CXTPSearchOptionsCtrl::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CStatic::PreCreateWindow(cs))
		return FALSE;

	// When creating controls dynamically Init() must
	// be called from OnCreate() and not from
	// PreSubclassWindow().

	m_bPreSubclassInit = false;

	return TRUE;
}

void CXTPSearchOptionsCtrl::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos)
{
	CStatic::OnWindowPosChanged(lpwndpos);

	// Update the control whenever the window size or position has changed.
	Invalidate();
	UpdateWindow();
}


/////////////////////////////////////////////////////////////////////////////

CXTPSearchOptionsView::CResizeWnd::CResizeWnd(CWnd* pWndParent, HWND hWndChild, const XTP_RESIZEPOINT& ptTopLeft, const XTP_RESIZEPOINT& ptTopRight)
: m_hWndChild(hWndChild)
, m_pWndParent(pWndParent)
, m_rcWindow(0, 0, 0, 0)
, m_rcSizing(CXTPResizeRect(ptTopLeft.x, ptTopLeft.y, ptTopRight.x, ptTopRight.y))
{
	ASSERT(m_hWndChild != NULL);
	ASSERT_VALID(m_pWndParent);

	::GetWindowRect(m_hWndChild, &m_rcWindow);
	m_pWndParent->ScreenToClient(&m_rcWindow);

	// if the control is a group box, then make it transparent
	if (IsGroupBox())
	{
		CWnd::ModifyStyleEx(m_hWndChild, 0, WS_EX_TRANSPARENT,
			SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
	}
}

CXTPSearchOptionsView::CResizeWnd::~CResizeWnd()
{
}

bool CXTPSearchOptionsView::CResizeWnd::IsGroupBox()
{
	if (!::IsWindow(m_hWndChild))
		return false;

	TCHAR szClass[8];
	::GetClassName(m_hWndChild, szClass, 8);

	if (_tcsicmp(szClass, _T("Button")) == 0)
	{
		DWORD dwStyle = ::GetWindowLong(m_hWndChild, GWL_STYLE);
		if ((dwStyle & (BS_GROUPBOX | WS_TABSTOP)) == BS_GROUPBOX)
		{
			return true;
		}
	}

	return false;
}

bool CXTPSearchOptionsView::CResizeWnd::Resize(HDWP& hDWP, float dx)
{
	if (!::IsWindow(m_hWndChild))
		return false;

	if (dx > 0)
	{
		CXTPResizeRect rrcWindow = m_rcWindow;
		CXTPResizeRect rrcItem = m_rcWindow;

		rrcItem.left += dx * m_rcSizing.left;
		rrcItem.right += dx * m_rcSizing.right;

		if (rrcItem != rrcWindow)
		{
			// get the size of the dialog item in client coordinates.
			CRect rcOld;
			::GetWindowRect(m_hWndChild, &rcOld);
			m_pWndParent->ScreenToClient(&rcOld);

			int x = (int) rrcItem.left;
			int y = (int) rcOld.top;
			int cx = (int) rrcItem.Width();
			int cy = (int) rcOld.Height();

			// Set positioning flags
			UINT uFlags = SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOOWNERZORDER;

			// if the x-y coordinates have not changed, there is no reason
			// to move the dialog item.
			CRect rcNew = rrcItem;
			if (rcNew.TopLeft() == rcOld.TopLeft())
				uFlags |= SWP_NOMOVE;

			// if the cx-cy size has not changed, there is no reason to
			// size the dialog item.  If size has changed, don't
			// copy bits of the client area (i.e. have them invalidated/redrawn)
			if (rcNew.Size() == rcOld.Size())
				uFlags |= SWP_NOSIZE;
			else
				uFlags |= SWP_NOCOPYBITS;

			::DeferWindowPos(hDWP, m_hWndChild, 0, x, y,
				cx, cy, uFlags);

			if (hDWP == NULL)
			{
				TRACE(_T("DeferWindowPos failed for ID %i\n"), ::GetDlgCtrlID(m_hWndChild));
				return false;
			}
		}
	}

	return true;
}

/////////////////////////////////////////////////////////////////////////////
// CXTPSearchOptionsView

IMPLEMENT_DYNAMIC(CXTPSearchOptionsView, CFormView)

CXTPSearchOptionsView::CXTPSearchOptionsView(LPCTSTR lpszTemplateName)
: CFormView(lpszTemplateName)
, m_rcWindow(0, 0, 0, 0)
{
	m_hBrush = NULL;
	m_clrBrush = (COLORREF)-1;
}

CXTPSearchOptionsView::CXTPSearchOptionsView(UINT nIDTemplate)
: CFormView(nIDTemplate)
, m_rcWindow(0, 0, 0, 0)
{
	m_hBrush = NULL;
	m_clrBrush = (COLORREF)-1;
}

CXTPSearchOptionsView::~CXTPSearchOptionsView()
{
	while (!m_arResizeWnd.IsEmpty())
	{
		CResizeWnd* pResizeWnd = m_arResizeWnd.RemoveHead();
		SAFE_DELETE(pResizeWnd);
	}
	if (m_hBrush != NULL)
		::DeleteObject(m_hBrush);
}

BEGIN_MESSAGE_MAP(CXTPSearchOptionsView, CFormView)
	//{{AFX_MSG_MAP(CXTPSearchOptionsView)
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTPSearchOptionsView message handlers

COLORREF CXTPSearchOptionsView::GetBackColor() const
{
	return GetXtremeColor(COLOR_WINDOW);
}

HBRUSH CXTPSearchOptionsView::OnCtlColor(CDC* pDC, CWnd* /*pWnd*/, UINT /*nCtlColor*/)
{

	COLORREF clr = GetBackColor();

	if (m_hBrush && clr != m_clrBrush)
	{
		DeleteObject(m_hBrush);
		m_hBrush = NULL;
	}

	if (!m_hBrush)
	{
		m_hBrush = ::CreateSolidBrush(clr);
		m_clrBrush = clr;
	}

	pDC->SetBkColor(m_clrBrush);
	pDC->SetTextColor(GetXtremeColor(COLOR_WINDOWTEXT));

	return m_hBrush;
}

LRESULT CXTPSearchOptionsView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRet = CFormView::WindowProc(message, wParam, lParam);

	if (message == WM_INITDIALOG)
	{
		OnInitDialog();
	}

	return lRet;
}

BOOL CXTPSearchOptionsView::OnInitDialog()
{
	GetClientRect(&m_rcWindow);
	return TRUE;
}

void CXTPSearchOptionsView::SetResize(int nID, const XTP_RESIZEPOINT& ptTopLeft, const XTP_RESIZEPOINT& ptTopRight)
{
	CResizeWnd* pResizeWnd = 0;

	// search for an item with the given id
	POSITION pos;
	for (pos = m_arResizeWnd.GetHeadPosition(); pos; m_arResizeWnd.GetNext(pos))
	{
		HWND hWndChild = m_arResizeWnd.GetAt(pos)->m_hWndChild;
		if (hWndChild && ::IsWindow(hWndChild) && (::GetDlgCtrlID(hWndChild) == nID))
		{
			pResizeWnd = m_arResizeWnd.GetAt(pos);
			break;
		}
	}

	if (pResizeWnd == 0)
	{
		HWND hWndChild = ::GetDlgItem(m_hWnd, nID);

		if (hWndChild != NULL)
		{
			pResizeWnd = new CResizeWnd(this, hWndChild, ptTopLeft, ptTopRight);
			m_arResizeWnd.AddHead(pResizeWnd);
		}
	}
	else
	{
		pResizeWnd->m_rcSizing += CXTPResizeRect(ptTopLeft.x, ptTopLeft.y, ptTopRight.x, ptTopRight.y);
	}
}

void CXTPSearchOptionsView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	CRect rcWindow;
	GetClientRect(rcWindow);

	float iOffset = (float)(rcWindow.Width() - m_rcWindow.Width());

	HDWP hDWP = ::BeginDeferWindowPos((int)m_arResizeWnd.GetCount());

	for (POSITION pos = m_arResizeWnd.GetHeadPosition(); pos; m_arResizeWnd.GetNext(pos))
	{
		CResizeWnd* pResizeWnd = m_arResizeWnd.GetAt(pos);
		if (pResizeWnd != NULL)
		{
			pResizeWnd->Resize(hDWP, iOffset);
		}
	}

	::EndDeferWindowPos(hDWP);
}

void CXTPSearchOptionsView::AdjustScrollSizes(CXTPSearchOptionsCtrl* pSOCtrl, bool bAdd)
{
	// get the scroll sizes for the form view.
	int nMapMode;
	CSize sizeTotal, sizePage, sizeLine;
	GetDeviceScrollSizes(nMapMode, sizeTotal, sizePage, sizeLine);

	// adjust the scroll sizes to accomindate for expansion.
	if (bAdd)
	{
		sizeTotal.cy += (pSOCtrl->GetOffsetSize());
	}
	else
	{
		sizeTotal.cy -= (pSOCtrl->GetOffsetSize());
	}

	SetScrollSizes(nMapMode, sizeTotal);

	// make sure the window and all siblings are updated.
	RedrawWindow(NULL, NULL,
		RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE | RDW_ALLCHILDREN);
}
