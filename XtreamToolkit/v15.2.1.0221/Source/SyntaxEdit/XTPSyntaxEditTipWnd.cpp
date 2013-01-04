// XTPSyntaxEditTipWnd.cpp : implementation file
//
// This file is a part of the XTREME TOOLKIT PRO MFC class library.
// (c)1998-2012 Codejock Software, All Rights Reserved.
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

#include "StdAfx.h"
#include "Resource.h"

// common includes
#include "Common/XTPSmartPtrInternalT.h"
#include "Common/XTPVC80Helpers.h"
#include "Common/XTPColorManager.h"

#include "Common/XTPResourceManager.h"
#include "Common/XTPDrawHelpers.h"

// syntax editor includes
#include "XTPSyntaxEditDefines.h"
#include "XTPSyntaxEditStruct.h"
#include "XTPSyntaxEditCtrl.h"
#include "XTPSyntaxEditDoc.h"
#include "XTPSyntaxEditView.h"
#include "XTPSyntaxEditPaintManager.h"
#include "XTPSyntaxEditColorSampleText.h"
#include "XTPSyntaxEditColorComboBox.h"
#include "XTPSyntaxEditPropertiesPage.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif


//===========================================================================
// CXTPSyntaxEditTipWnd
//===========================================================================

CXTPSyntaxEditTipWnd::CXTPSyntaxEditTipWnd()
: m_pListBox(NULL)
, m_iIndex(LB_ERR)
, m_uIDEvent1(1001)
, m_uIDEvent2(1002)
{
	RegisterWindowClass();
}

CXTPSyntaxEditTipWnd::~CXTPSyntaxEditTipWnd()
{
	DestroyWindow();
}

BEGIN_MESSAGE_MAP(CXTPSyntaxEditTipWnd, CWnd)
	//{{AFX_MSG_MAP(CXTPSyntaxEditTipWnd)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_NCHITTEST_EX()
	ON_WM_NCPAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//---------------------------------------------------------------------------
// CXTPSyntaxEditTipWnd diagnostics
//---------------------------------------------------------------------------

#ifdef _DEBUG
void CXTPSyntaxEditTipWnd::AssertValid() const
{
	CWnd::AssertValid();

	ASSERT(::IsWindow(m_hWnd));
	ASSERT_VALID(m_pListBox);
}

void CXTPSyntaxEditTipWnd::Dump(CDumpContext& dc) const
{
	CWnd::Dump(dc);
}
#endif //_DEBUG

//---------------------------------------------------------------------------
// CXTPSyntaxEditTipWnd registration
//---------------------------------------------------------------------------

BOOL CXTPSyntaxEditTipWnd::RegisterWindowClass(HINSTANCE hInstance /*= NULL*/)
{
	WNDCLASS wndcls;
	if (hInstance == NULL) hInstance = AfxGetInstanceHandle();

	if (!(::GetClassInfo(hInstance, XTP_EDIT_CLASSNAME_LBOXTIP, &wndcls)))
	{
		// otherwise we need to register a new class
		wndcls.style = CS_SAVEBITS | CS_HREDRAW | CS_VREDRAW;
		wndcls.lpfnWndProc = ::DefWindowProc;
		wndcls.cbClsExtra = wndcls.cbWndExtra = 0;
		wndcls.hInstance = hInstance;
		wndcls.hIcon = NULL;
		wndcls.hCursor = ::LoadCursor(0, IDC_ARROW);
		wndcls.hbrBackground = (HBRUSH)(COLOR_INFOBK + 1);
		wndcls.lpszMenuName = NULL;
		wndcls.lpszClassName = XTP_EDIT_CLASSNAME_LBOXTIP;

		if (!AfxRegisterClass(&wndcls))
		{
			AfxThrowResourceException();
			return FALSE;
		}
	}

	return TRUE;
}

//---------------------------------------------------------------------------
// CXTPSyntaxEditTipWnd paint routines
//---------------------------------------------------------------------------

BOOL CXTPSyntaxEditTipWnd::OwnerDrawTip(CDC* pDC, CRect rClient)
{
	ASSERT_VALID(this);
	HWND hWndLBox = m_pListBox->GetSafeHwnd();

	if (!::IsWindow(hWndLBox))
		return FALSE;

	DRAWITEMSTRUCT dis;
	::ZeroMemory(&dis, sizeof(DRAWITEMSTRUCT));

	dis.CtlType     = ODT_LISTBOX;
	dis.CtlID       = (UINT)m_pListBox->GetDlgCtrlID();
	dis.itemID      = (UINT)m_iIndex;
	dis.itemAction  = ODA_DRAWENTIRE;
	dis.hwndItem    = hWndLBox;
	dis.hDC         = pDC->GetSafeHdc();
	dis.rcItem      = rClient;
	dis.itemData    = m_pListBox->GetItemData(m_iIndex);

	if (m_pListBox->GetSel(m_iIndex) > 0)
		dis.itemState |= ODS_SELECTED;

	if (m_pListBox->GetCaretIndex() == m_iIndex)
		dis.itemState |= ODS_FOCUS;

	m_pListBox->DrawItem(&dis);

	return TRUE;
}

BOOL CXTPSyntaxEditTipWnd::DrawTip(CDC* pDC, CRect rClient)
{
	ASSERT_VALID(this);

	CString csItemText;
	m_pListBox->GetText(m_iIndex, csItemText);

	CXTPFontDC fontDC(pDC, m_pListBox->GetFont(), GetTextColor());

	pDC->SetBkColor(GetBackColor());
	pDC->FillSolidRect(rClient, pDC->GetBkColor());
	pDC->SetBkMode(TRANSPARENT);

	pDC->DrawText(csItemText, rClient,
		DT_VCENTER | DT_CENTER |DT_SINGLELINE | DT_NOPREFIX | DT_EXPANDTABS);

	return TRUE;
}

COLORREF CXTPSyntaxEditTipWnd::GetTextColor() const
{
	ASSERT_VALID(this);

	return ::GetSysColor((m_pListBox->GetSel(m_iIndex) > 0)?
		COLOR_HIGHLIGHTTEXT: COLOR_INFOTEXT);
}

COLORREF CXTPSyntaxEditTipWnd::GetBackColor() const
{
	ASSERT_VALID(this);

	return ::GetSysColor((m_pListBox->GetSel(m_iIndex) > 0)?
		COLOR_HIGHLIGHT: COLOR_INFOBK);
}

void CXTPSyntaxEditTipWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CXTPClientRect rClient(this);
	if (IsOwnerDrawn())
		OwnerDrawTip(&dc, rClient);
	else
		DrawTip(&dc, rClient);
}

void CXTPSyntaxEditTipWnd::OnNcPaint()
{
	CXTPWindowRect rc(this);
	rc.OffsetRect(-rc.TopLeft());

	CWindowDC dc(this);
	dc.SetBkColor(GetBackColor());
	dc.SetTextColor(GetTextColor());
	dc.FillSolidRect(&rc, dc.GetBkColor());
	dc.DrawFocusRect(&rc);
}

//---------------------------------------------------------------------------
// CXTPSyntaxEditTipWnd message handlers
//---------------------------------------------------------------------------

void CXTPSyntaxEditTipWnd::OnTimer(UINT_PTR nIDEvent)
{
	ASSERT_VALID(this);

	if (nIDEvent == m_uIDEvent1)
	{
		HideTip();
	}

	if (nIDEvent == m_uIDEvent2)
	{
		CPoint point;
		::GetCursorPos(&point);
		ScreenToClient(&point);

		BOOL bOutside = FALSE;
		m_pListBox->ItemFromPoint(point, bOutside);

		if (bOutside)
		{
			HideTip();
		}
	}

	CWnd::OnTimer(nIDEvent);
}

LRESULT CXTPSyntaxEditTipWnd::OnNcHitTest(CPoint point)
{
	UNREFERENCED_PARAMETER(point);
	return (LRESULT)HTTRANSPARENT;
}

BOOL CXTPSyntaxEditTipWnd::Create(CListBox* pListBox)
{
	ASSERT_VALID(pListBox);
	m_pListBox = pListBox;

	if (!::IsWindow(m_pListBox->GetSafeHwnd()))
		return FALSE;

	// Already created?
	if (::IsWindow(m_hWnd))
		return TRUE;

	if (!CWnd::CreateEx(WS_EX_TOOLWINDOW, XTP_EDIT_CLASSNAME_LBOXTIP, NULL,
		WS_POPUP | WS_BORDER | WS_CLIPSIBLINGS, CXTPEmptyRect(), NULL, 0))
	{
		TRACE0("Failed to create tooltip window.\n");
		return FALSE;
	}

	return TRUE;
}

BOOL CXTPSyntaxEditTipWnd::IsOwnerDrawn()
{
	ASSERT_VALID(this);

	HWND hWndLBox = m_pListBox->GetSafeHwnd();

	if (::IsWindow(hWndLBox))
	{
		return ((::GetWindowLong(hWndLBox, GWL_STYLE) &
			(LBS_OWNERDRAWFIXED|LBS_OWNERDRAWVARIABLE)) != 0);
	}
	return FALSE;
}

BOOL CXTPSyntaxEditTipWnd::ShowTip(int iIndex)
{
	ASSERT_VALID(this);

	if ((iIndex < 0) || (iIndex >= m_pListBox->GetCount()))
		return HideTip();

	CPoint ptCursor;
	::GetCursorPos(&ptCursor);

	if (m_iIndex == iIndex)
	{
		if (ptCursor == m_ptCursor)
		{
			RedrawWindow(NULL, NULL, RDW_FRAME | RDW_INVALIDATE | RDW_UPDATENOW);
			return TRUE;
		}
	}

	m_ptCursor = ptCursor;

	// stop existing timer.
	KillTipTimer();

	CRect rItem;
	if (CalcItemRect(iIndex, rItem))
	{
		if ((m_rWindow == rItem) || m_rWindow.IsRectEmpty())
			return HideTip();

		m_rWindow.InflateRect(0, 2);

		if (m_iIndex != iIndex)
		{
			m_iIndex = iIndex;
			RedrawWindow(NULL, NULL, RDW_FRAME | RDW_INVALIDATE | RDW_UPDATENOW);
		}

		ShowWindow(SW_SHOWNA);
		MoveWindow(&m_rWindow);

		// set timer.
		SetTipTimer();

		return TRUE;
	}

	return FALSE;
}

BOOL CXTPSyntaxEditTipWnd::HideTip()
{
	if (::IsWindowVisible(m_hWnd))
	{
		// stop existing timer.
		KillTipTimer();

		ShowWindow(SW_HIDE);
		return TRUE;
	}
	return FALSE;
}

BOOL CXTPSyntaxEditTipWnd::CalcItemRect(int iItem, CRect& rItem)
{
	ASSERT_VALID(this);

	if (m_pListBox->GetItemRect(iItem, &rItem) == LB_ERR)
		return FALSE;

	if (IsOwnerDrawn())
		return FALSE;

	CString csItem;
	m_pListBox->GetText(iItem, csItem);
	m_pListBox->ClientToScreen(rItem);

	if (csItem.IsEmpty())
		return FALSE;

	CWindowDC dc(NULL);
	CXTPFontDC fontDC(&dc, m_pListBox->GetFont());
	int iAdjust = dc.GetTextExtent(csItem).cx + (::GetSystemMetrics(SM_CXEDGE)*2);

	m_rWindow.CopyRect(rItem);
	m_rWindow.right = max(rItem.right, rItem.left + iAdjust);

	return TRUE;
}

void CXTPSyntaxEditTipWnd::SetTipTimer()
{
	SetTimer(m_uIDEvent1, 5000, NULL); // 5 seconds.
	SetTimer(m_uIDEvent2, 100,  NULL); // 10th of a second.
}

void CXTPSyntaxEditTipWnd::KillTipTimer()
{
	// stop existing timers.
	KillTimer(m_uIDEvent1);
	KillTimer(m_uIDEvent2);
}
