// XTPReportInplaceControls.cpp
//
// This file is a part of the XTREME REPORTCONTROL MFC class library.
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

#include "StdAfx.h"
#include "Common/Tmschema.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPCustomHeap.h"
#include "Common/XTPSmartPtrInternalT.h"
#include "Common/XTPColorManager.h"

#include "XTPReportDefines.h"
#include "XTPReportRecordItem.h"
#include "XTPReportControl.h"
#include "XTPReportInplaceControls.h"
#include "XTPReportColumn.h"
//#include "ItemTypes/XTPReportRecordItemText.h"
#include "XTPReportRecordItemConstraint.h"
#include "XTPReportPaintManager.h"
#include "XTPReportRow.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CXTPReportInplaceControl::CXTPReportInplaceControl()
{
}

CXTPReportInplaceControl::~CXTPReportInplaceControl()
{
	SetItemArgs(0);
}

void CXTPReportInplaceControl::SetItemArgs(XTP_REPORTRECORDITEM_ARGS* pItemArgs)
{
	if (pItemArgs)
	{
		pItemArgs->AddRef();
		Release();

		pItem = pItemArgs->pItem;
		pControl = pItemArgs->pControl;
		pRow = pItemArgs->pRow;
		pColumn = pItemArgs->pColumn;
		rcItem = pItemArgs->rcItem;
	}
	else
	{
		Release();

		pItem = NULL;
		pControl = NULL;
		pRow = NULL;
		pColumn = NULL;
	}
}


//////////////////////////////////////////////////////////////////////////
// CXTPReportInplaceButton

CXTPReportInplaceButton::CXTPReportInplaceButton(UINT nID)
{
	m_nID = nID;
	m_nWidth = 17;
	m_nFixedHeight = 19;
	m_bInsideCell = FALSE;
	m_nIconIndex = XTP_REPORT_NOICON;

	m_bPressed = m_bOver = FALSE;
	m_nState = 0;
	m_nSpinIncrement = 0;
	m_unSpinTimerCnt = 0;
	m_unSpinTimerId = 0;

	m_nSpinMin = INT_MIN;
	m_nSpinMax = INT_MAX;
	m_nSpinStep = 1;

	m_Items2Show = 10;

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

void CXTPReportInplaceButton::Create(XTP_REPORTRECORDITEM_ARGS* pItemArgs, CRect& rcButtons)
{
	m_bPressed = m_bOver = FALSE;
	m_nState = 0;
	m_nSpinIncrement = 0;
	m_unSpinTimerCnt = 0;
	m_unSpinTimerId = 0;
	SetItemArgs(pItemArgs);

	CRect rect(rcButtons);
	if (pControl->GetPaintManager()->IsFixedInplaceButtonHeight())
	{
		rect.bottom = min(rect.bottom, rect.top + m_nFixedHeight);
	}

	if (m_bInsideCell)
	{
		rect.right = rcButtons.left;
		rect.left = rect.right - m_nWidth;

		rcButtons.left = rect.left;
	}
	else
	{
		rect.left = rcButtons.right;
		rect.right = rect.left + m_nWidth;

		rcButtons.right = rect.right;
	}

	//to keep focused frame no touched
	rect.top++;
	rect.bottom -= 2;
	rect.left--;
	if (m_nID == XTP_ID_REPORT_COMBOBUTTON)
		rect.right--;

	if (!m_hWnd)
		CStatic::Create(NULL, SS_NOTIFY | WS_CHILD, rect, pItemArgs->pControl);

	SetWindowPos(0, rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOZORDER | SWP_SHOWWINDOW);
}

BEGIN_MESSAGE_MAP(CXTPReportInplaceButton, CStatic)
	//{{AFX_MSG_MAP(CXTPReportInplaceButton)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEMOVE()
	ON_WM_CAPTURECHANGED()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CXTPReportInplaceButton::OnPaint()
{
	CPaintDC dc(this);

	if (pControl)
	{
		pControl->GetPaintManager()->DrawInplaceButton(&dc, this);
	}
}

void CXTPReportInplaceButton::OnFinalRelease()
{
	if (m_hWnd != NULL)
		DestroyWindow();

	CCmdTarget::OnFinalRelease();
}

void CXTPReportInplaceButton::OnLButtonDown(UINT, CPoint point)
{
	switch (m_nID)
	{
		case XTP_ID_REPORT_COMBOBUTTON :
			m_bOver = m_bPressed = TRUE;
			break;
		case XTP_ID_REPORT_EXPANDBUTTON :
			m_bOver = m_bPressed = TRUE;
			break;
		case XTP_ID_REPORT_SPINBUTTON :
		{
			CXTPClientRect rect(this);
			m_bOver = TRUE;
			rect.bottom -= rect.Height() / 2;
			m_nState = rect.PtInRect(point) ? SPNP_UP : SPNP_DOWN;

			m_nSpinIncrement = m_nState == SPNP_UP ? m_nSpinStep : -m_nSpinStep;

			pItem->OnInplaceButtonDown(this);

			// start timer
			m_unSpinTimerCnt = 0;
			m_unSpinTimerId = SetTimer(1, 500, NULL);
			break;
		}
	}
	Invalidate(FALSE);
	SetCapture();
}

void CXTPReportInplaceButton::OnLButtonUp(UINT nFlags, CPoint point)
{
	if ((m_bPressed || m_nState) && pItem)
	{
		if (m_unSpinTimerId)
		{
			KillTimer(1);
			m_unSpinTimerId = 0;
		}
		m_nSpinIncrement = 0;
		m_unSpinTimerCnt = 0;
		m_bPressed = FALSE;
		m_nState = 0;
		Invalidate(FALSE);
		ReleaseCapture();
		if (m_bOver && m_nID != XTP_ID_REPORT_SPINBUTTON)
		{
			pItem->OnInplaceButtonDown(this);
		}
	}
	CStatic::OnLButtonUp(nFlags, point);
}

void CXTPReportInplaceButton::Activate()
{
	m_bOver = m_bPressed = TRUE;
	Invalidate(FALSE);
	SetCapture();
}

void CXTPReportInplaceButton::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	UNREFERENCED_PARAMETER(nFlags);

	if (m_nID == XTP_ID_REPORT_SPINBUTTON)
	{
		CXTPClientRect rect(this);
		if (rect.PtInRect(point))
		{
			rect.bottom -= rect.Height() / 2;
			m_nState = rect.PtInRect(point) ? SPNP_UP : SPNP_DOWN;
			m_nSpinIncrement = m_nState == SPNP_UP ? m_nSpinStep : -m_nSpinStep;
			pItem->OnInplaceButtonDown(this);
			// start timer
			m_unSpinTimerCnt = 0;
			m_unSpinTimerId = SetTimer(1, 500, NULL);
		}
		Invalidate(FALSE);
		SetCapture();
	}
}

void CXTPReportInplaceButton::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_bPressed || m_nState)
	{
		CXTPClientRect rect(this);
		BOOL bOver;
		if (m_nID != XTP_ID_REPORT_SPINBUTTON)
			bOver = rect.PtInRect(point);
		else
		{
			bOver = rect.PtInRect(point) &&
				(point.y < (rect.bottom - rect.Height() / 2) && m_nState == SPNP_UP ||
				point.y >= (rect.bottom - rect.Height() / 2) && m_nState == SPNP_DOWN);
		}
		if ((bOver && !m_bOver) || (!bOver && m_bOver))
		{
			m_bOver = bOver;
			if (m_nID == XTP_ID_REPORT_SPINBUTTON)
			{
				if (m_bOver)
				{
					m_nSpinIncrement = m_nState == SPNP_UP ? 1 : -1;
					m_unSpinTimerCnt = 0;
					m_unSpinTimerId = SetTimer(1, 500, NULL); // start timer
				}
				else if (m_unSpinTimerId)
				{
					// stop timer
					KillTimer(1);
					m_nSpinIncrement = 0;
					m_unSpinTimerCnt = 0;
					m_unSpinTimerId = 0;
				}
			}
			Invalidate(FALSE);
		}
	}

	CStatic::OnMouseMove(nFlags, point);
}

void CXTPReportInplaceButton::OnCaptureChanged(CWnd* pWnd)
{
	m_bPressed = FALSE;
	m_nState = 0;
	Invalidate(FALSE);

	CStatic::OnCaptureChanged(pWnd);
}

void CXTPReportInplaceButton::OnTimer(UINT_PTR nIDEvent)
{
	if (m_unSpinTimerCnt == 0 && abs(m_nSpinIncrement) < 10)
	{
		// first timer event, reset timer
		KillTimer(1);
		m_unSpinTimerId = SetTimer(1, 100, NULL);
	}
	m_unSpinTimerCnt++;
	if (m_unSpinTimerCnt >= 20 && abs(m_nSpinIncrement < 100000))
	{
		m_nSpinIncrement *= 10;
		m_unSpinTimerCnt = 0;
	}
	pItem->OnInplaceButtonDown(this);

	CStatic::OnTimer(nIDEvent);
}

//////////////////////////////////////////////////////////////////////////
// CXTPReportInplaceList

CXTPReportInplaceList::CXTPReportInplaceList()
{
	m_bApply = FALSE;
	m_dwLastKeyDownTime = 0;
	m_Items2Show = 10;
}


void CXTPReportInplaceList::Create(XTP_REPORTRECORDITEM_ARGS* pItemArgs, CXTPReportRecordItemConstraints* pConstaints)
{
	SetItemArgs(pItemArgs);

	CRect rect(pItemArgs->rcItem);

	if (!m_hWnd)
	{
		CListBox::CreateEx(WS_EX_TOOLWINDOW | (pControl->GetExStyle() & WS_EX_LAYOUTRTL), _T("LISTBOX"), _T(""), LBS_NOTIFY | WS_CHILD | WS_BORDER | WS_VSCROLL, CRect(0, 0, 0, 0), pControl, 0);
		SetOwner(pControl);
	}

	SetFont(pControl->GetPaintManager()->GetTextFont());
	ResetContent();

	int dx = rect.right - rect.left + 1;

	CWindowDC dc(pControl);
	CXTPFontDC font(&dc, GetFont());
	int nThumbLength = GetSystemMetrics(SM_CXHTHUMB);

	CString strCaption = pItem->GetCaption(pColumn);
	DWORD dwData = pItem->GetSelectedConstraintData(pItemArgs);

	for (int i = 0; i < pConstaints->GetCount(); i++)
	{
		CXTPReportRecordItemConstraint* pConstaint = pConstaints->GetAt(i);
		CString str = pConstaint->m_strConstraint;
		int nIndex = AddString(str);
		SetItemDataPtr(nIndex, pConstaint);

		dx = max(dx, dc.GetTextExtent(str).cx + nThumbLength);

		if ((dwData == (DWORD)-1 && strCaption == str) || (dwData == pConstaint->m_dwData))
			SetCurSel(nIndex);
	}

	int nHeight = GetItemHeight(0);
	rect.top = rect.bottom;
	//rect.bottom += nHeight * min(10, GetCount()) + 2;
	rect.bottom += nHeight * min(m_Items2Show, GetCount()) + 2;
	rect.left = rect.right - dx;

	pControl->ClientToScreen(&rect);

	CRect rcWork = XTPMultiMonitor()->GetWorkArea(rect);
	if (rect.bottom > rcWork.bottom && rect.top > rcWork.CenterPoint().y)
		rect.OffsetRect(0, - rect.Height() - pItemArgs->rcItem.Height());

	if (rect.left < rcWork.left) rect.OffsetRect(rcWork.left - rect.left, 0);
	if (rect.right > rcWork.right) rect.OffsetRect(rcWork.right - rect.right, 0);

	SetFocus();

	if (!m_hWnd) // Can be destroyed after focus set
		return;

	SetWindowLongPtr(m_hWnd, GWLP_HWNDPARENT, 0);
	ModifyStyle(WS_CHILD, WS_POPUP);
	SetWindowLongPtr(m_hWnd, GWLP_HWNDPARENT, (LONG_PTR)pControl->m_hWnd);

	SetWindowPos(&CWnd::wndTopMost, rect.left, rect.top, rect.Width(), rect.Height(), SWP_SHOWWINDOW | SWP_NOACTIVATE | SWP_NOOWNERZORDER);

	CXTPMouseMonitor::SetupHook(this);
}

BEGIN_MESSAGE_MAP(CXTPReportInplaceList, CListBox)
	//{{AFX_MSG_MAP(CXTPReportInplaceList)
	ON_WM_MOUSEACTIVATE()
	ON_WM_KILLFOCUS()
	ON_WM_LBUTTONUP()
	ON_WM_CHAR()
	ON_WM_KEYDOWN()
	ON_WM_SYSKEYDOWN()
	ON_WM_GETDLGCODE()
	ON_WM_NCPAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CXTPReportInplaceList::OnNcPaint()
{
	Default();

	CWindowDC dc(this);

	CXTPWindowRect rc(this);
	rc.OffsetRect(-rc.TopLeft());

	dc.Draw3dRect(rc, GetSysColor(COLOR_WINDOWFRAME), GetSysColor(COLOR_WINDOWFRAME));
}


int CXTPReportInplaceList::OnMouseActivate(CWnd* /*pDesktopWnd*/, UINT /*nHitTest*/, UINT /*message*/)
{
	return MA_NOACTIVATE;
}

UINT CXTPReportInplaceList::OnGetDlgCode()
{
	return DLGC_WANTALLKEYS;
}

void CXTPReportInplaceList::SetItemArgs(XTP_REPORTRECORDITEM_ARGS* pItemArgs)
{
	m_bApply = FALSE;
	CXTPReportInplaceControl::SetItemArgs(pItemArgs);

	m_dwLastKeyDownTime = 0;
	m_strHotSearchContext.Empty();
}

void CXTPReportInplaceList::PostNcDestroy()
{
	CXTPMouseMonitor::SetupHook(NULL);
	SetItemArgs(NULL);

	CListBox::PostNcDestroy();
}


void CXTPReportInplaceList::OnKillFocus(CWnd* pNewWnd)
{
	//ASSERT(pItem || m_bApply);

	if (pItem && !m_bApply)
		pItem->OnEditCanceled(this);

	CListBox::OnKillFocus(pNewWnd);
	DestroyWindow();
}

void CXTPReportInplaceList::OnLButtonUp(UINT, CPoint point)
{
	CXTPClientRect rc(this);

	if (rc.PtInRect(point))
		Apply();
	else
		Cancel();
}


void CXTPReportInplaceList::Cancel()
{
	m_bApply = FALSE;

	GetOwner()->SetFocus();
}

void CXTPReportInplaceList::Apply()
{
	if (!pControl)
		return;

	CXTPReportControl* pReportControl = pControl;

	int nIndex = GetCurSel();
	if (nIndex != LB_ERR)
	{
		m_bApply = TRUE;

		CXTPReportRecordItemConstraint* pConstraint = (CXTPReportRecordItemConstraint*)GetItemDataPtr(nIndex);

		XTP_REPORTRECORDITEM_ARGS itemArgs = *((XTP_REPORTRECORDITEM_ARGS*)this);
		itemArgs.AddRef();

		pItem->OnConstraintChanged(&itemArgs, pConstraint);
		pReportControl->RedrawControl();

		pReportControl->SendMessageToParent(itemArgs.pRow, itemArgs.pItem, itemArgs.pColumn, XTP_NM_REPORT_VALUECHANGED, 0);

		itemArgs.Release();
	}

	pReportControl->SetFocus();
}

void CXTPReportInplaceList::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	UNREFERENCED_PARAMETER(nRepCnt); UNREFERENCED_PARAMETER(nFlags);

	const DWORD cwdHotSearchTomeOut_ms = 1300;

	DWORD dwTime = GetTickCount();

	if (dwTime - m_dwLastKeyDownTime > cwdHotSearchTomeOut_ms)
		m_strHotSearchContext.Empty();

	m_dwLastKeyDownTime = dwTime;

	//----------------------------------------------
	m_strHotSearchContext += (TCHAR)nChar;

	int nIndex = GetCurSel();
	if (nIndex == LB_ERR)
		nIndex = 0;

	int nFindIdx = FindString(nIndex, m_strHotSearchContext);

	if (nFindIdx == LB_ERR && nIndex > 0)
		nFindIdx = FindString(0, m_strHotSearchContext);

	if (nFindIdx != LB_ERR)
	{
		SetCurSel(nFindIdx);

		if (nIndex != nFindIdx)
			OnSelectionChanged(nFindIdx);
	}
}

void CXTPReportInplaceList::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar)
	{
	case VK_UP:
	case VK_DOWN:
	case VK_PRIOR:
	case VK_NEXT:
	case VK_HOME:
	case VK_END:
		m_strHotSearchContext.Empty();
	}

	//----------------------------------------------
	if (nChar == VK_ESCAPE)
	{
		Cancel();
	}
	else if (nChar == VK_RETURN || nChar == VK_F4)
	{
		Apply();
	}
	else
	{
		int nPrevSel = CListBox::GetCurSel();
		CListBox::OnKeyDown(nChar, nRepCnt, nFlags);
		int nActualSel = CListBox::GetCurSel();

		if (nPrevSel != nActualSel)
			OnSelectionChanged(nActualSel);
	}
}

BOOL CXTPReportInplaceList::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pControl)
	{
		if (!pControl->OnPreviewKeyDown((UINT&)pMsg->wParam, LOWORD(pMsg->lParam), HIWORD(pMsg->lParam)) )
			return TRUE;
	}

	if (pMsg->message == WM_KEYDOWN && IsDialogMessage(pMsg))
		return TRUE;

	return CListBox::PreTranslateMessage(pMsg);
}

void CXTPReportInplaceList::OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_DOWN || nChar == VK_UP)
	{
		Apply();
		return;
	}

	CListBox::OnSysKeyDown(nChar, nRepCnt, nFlags);
}

void CXTPReportInplaceList::OnSelectionChanged(int nLBIndex)
{
	CString strActual, str;
	CListBox::GetText(nLBIndex,strActual);

	CXTPReportRecordItemEditOptions* pEditOptions = pItem->GetEditOptions(pColumn);
	CXTPReportRecordItemConstraints* pConstraints = pEditOptions->GetConstraints();

	int nCount = pConstraints->GetCount();
	for (int nIndex = 0; nIndex < nCount; nIndex++)
	{
		CXTPReportRecordItemConstraint* pConstraint = pConstraints->GetAt(nIndex);
		str = pConstraint->m_strConstraint;

		if (strActual.CompareNoCase(str) == 0)
		{
			((CXTPReportControl*) pControl)->OnConstraintSelecting(pRow, pItem, pColumn, pConstraint);
			break;
		}
	}
}


#ifdef _XTP_ACTIVEX


BEGIN_DISPATCH_MAP(CXTPReportInplaceButton, CCmdTarget)
	DISP_PROPERTY_ID(CXTPReportInplaceButton, "Id", DISPID_VALUE, m_nID, VT_I4)

	DISP_PROPERTY_EX_ID(CXTPReportInplaceButton, "Row", 1, OleGetRow, SetNotSupported, VT_DISPATCH)
	DISP_PROPERTY_EX_ID(CXTPReportInplaceButton, "Column", 2, OleGetColumn, SetNotSupported, VT_DISPATCH)
	DISP_PROPERTY_EX_ID(CXTPReportInplaceButton, "Item", 3, OleGetItem, SetNotSupported, VT_DISPATCH)

	DISP_FUNCTION_ID(CXTPReportInplaceButton, "GetRect", 4, OleGetRect, VT_EMPTY, VTS_PI4 VTS_PI4 VTS_PI4 VTS_PI4)

	DISP_PROPERTY_ID(CXTPReportInplaceButton, "InsideCellButton", 5, m_bInsideCell, VT_BOOL)
	DISP_PROPERTY_ID(CXTPReportInplaceButton, "IconIndex", 6, m_nIconIndex, VT_I4)

	DISP_PROPERTY_ID(CXTPReportInplaceButton, "SpinButtonMin", 7, m_nSpinMin, VT_I4)
	DISP_PROPERTY_ID(CXTPReportInplaceButton, "SpinButtonMax", 8, m_nSpinMax, VT_I4)
	DISP_PROPERTY_ID(CXTPReportInplaceButton, "SpinButtonStep", 9, m_nSpinStep, VT_I4)
	DISP_PROPERTY_ID(CXTPReportInplaceButton, "ItemsShowInDroplist", 10, m_Items2Show, VT_I4)

END_DISPATCH_MAP()

// {C17BCC77-27BF-4cb1-9ABF-1788D9835223}
static const GUID IID_IReportInplaceButton =
{ 0xc17bcc77, 0x27bf, 0x4cb1, { 0x9a, 0xbf, 0x17, 0x88, 0xd9, 0x83, 0x52, 0x23 } };

BEGIN_INTERFACE_MAP(CXTPReportInplaceButton, CCmdTarget)
	INTERFACE_PART(CXTPReportInplaceButton, IID_IReportInplaceButton, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPReportInplaceButton, IID_IReportInplaceButton)

LPDISPATCH CXTPReportInplaceButton::OleGetRow()
{
	return pRow ? pRow->GetIDispatch(TRUE) : 0;
}

LPDISPATCH CXTPReportInplaceButton::OleGetColumn()
{
	return pColumn ? pColumn->GetIDispatch(TRUE) : 0;
}

LPDISPATCH CXTPReportInplaceButton::OleGetItem()
{
	return pItem ? pItem->GetIDispatch(TRUE) : 0;
}

void CXTPReportInplaceButton::OleGetRect(long* pnLeft, long* pnTop, long* pnRight, long* pnBottom)
{
	CRect rcButton;
	if (m_hWnd)
	{
		GetWindowRect(&rcButton);
		if (GetParent())
			GetParent()->ScreenToClient(&rcButton);
	}
	else if (pRow && pItem)
	{
		rcButton = pRow->GetItemRect(pItem);

		rcButton.right = rcButton.left;
		rcButton.bottom = rcButton.top;
	}
	if (pnLeft)
		*pnLeft   = rcButton.left;
	if (pnTop)
		*pnTop    = rcButton.top;
	if (pnRight)
		*pnRight  = rcButton.right;
	if (pnBottom)
		*pnBottom = rcButton.bottom;
}

#endif
