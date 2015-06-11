// XTPCalendarCaptionBarControl.cpp : implementation file
//
// This file is a part of the XTREME CALENDAR MFC class library.
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

#include "Common/XTPResourceManager.h"
#include "Common/XTPResourceImage.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPNotifyConnection.h"
#include "Common/XTPSmartPtrInternalT.h"
#include "Common/XTPWinThemeWrapper.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPVC80Helpers.h"

#include "XTPCalendarUtils.h"
#include "XTPCalendarDefines.h"
#include "XTPCalendarNotifications.h"
#include "XTPCalendarPtrCollectionT.h"
#include "XTPCalendarViewPart.h"
#include "XTPCalendarView.h"
#include "XTPCalendarTimeLineView.h"
#include "XTPCalendarControl.h"
#include "XTPCalendarMonthView.h"

#include "XTPCalendarCaptionBarControl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CXTPCalendarCaptionBarControl, CWnd)
IMPLEMENT_DYNAMIC(CXTPCalendarCaptionBarThemePart, CXTPCalendarWMHandler)
IMPLEMENT_DYNCREATE(CXTPCalendarCaptionBarTheme, CXTPCalendarWMHandler)

CXTPCalendarCaptionBarControl::CXTPCalendarCaptionBarControl()
{
	RegisterWindowClass();

	m_Borders.SetRect(0, 1, 0, 0);


	m_pTheme = NULL;
	SetTheme(new CXTPCalendarCaptionBarTheme());
}

CXTPCalendarCaptionBarControl::~CXTPCalendarCaptionBarControl()
{
	CMDTARGET_RELEASE(m_pTheme);
}

BOOL CXTPCalendarCaptionBarControl::RegisterWindowClass(HINSTANCE hInstance /*= NULL*/)
{
	return XTPDrawHelpers()->RegisterWndClass(hInstance,
			XTPCALENDARCAPTIONBAR_CLASSNAME, /*CS_DBLCLKS |*/ CS_HREDRAW | CS_VREDRAW);
}

BOOL CXTPCalendarCaptionBarControl::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	if (!CWnd::Create(XTPCALENDARCAPTIONBAR_CLASSNAME, NULL, dwStyle, rect, pParentWnd, nID))
		return FALSE;

	AdjustLayout(NULL);

	if (m_pTheme)
	{
		CDC* pDC = GetDC();
		if (pDC)
		{
			m_pTheme->m_dScale = pDC->GetDeviceCaps(LOGPIXELSY) / 96.0;
			ReleaseDC(pDC);
		}
	}
	return TRUE;
}

BEGIN_MESSAGE_MAP(CXTPCalendarCaptionBarControl, CWnd)
//{{AFX_MSG_MAP(CXTPCalendarCaptionBarControl)
	ON_WM_NCCALCSIZE()
	ON_WM_NCPAINT()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_KILLFOCUS()
	ON_WM_DESTROY()
	ON_WM_ENABLE()
	ON_WM_TIMER()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CXTPCalendarCaptionBarControl::SetButtonText(int iButtonNumber, CString sButtonText)
{
	if (!GetTheme())
		return;
	//0 CSwitchViewButtonPart* m_pButtonViewDay;
	//1 CSwitchViewButtonPart* m_pButtonViewWeek;
	//2 CSwitchViewButtonPart* m_pButtonViewMonth;
	//3 CCheckboxButtonPart* m_pShowTimeline;
	//4 CRadioButtonPart* m_pShowWorkWeek;
	//5 CRadioButtonPart* m_pShowFullWeek;
	//6 CRadioButtonPart* m_pShowMcWeek;

	BOOL bShow = !sButtonText.IsEmpty();

	if (iButtonNumber == 0)
	{
		if (GetTheme()->m_pButtonViewDay)
		{
			GetTheme()->m_pButtonViewDay->SetText(sButtonText);
			GetTheme()->m_pButtonViewDay->SetVisible(bShow);
		}
	}
	else if (iButtonNumber == 1)
	{
		if (GetTheme()->m_pButtonViewWeek)
		{
			GetTheme()->m_pButtonViewWeek->SetText(sButtonText);
			GetTheme()->m_pButtonViewWeek->SetVisible(bShow);
		}
	}
	else if (iButtonNumber == 2)
	{
		if (GetTheme()->m_pButtonViewMonth)
		{
			GetTheme()->m_pButtonViewMonth->SetText(sButtonText);
			GetTheme()->m_pButtonViewMonth->SetVisible(bShow);
		}
	}
	else if (iButtonNumber == 3)
	{
		if (GetTheme()->m_pShowTimeline)
		{
			GetTheme()->m_pShowTimeline->SetText(sButtonText);
			GetTheme()->m_pShowTimeline->SetVisible(bShow);
		}
	}
	else if (iButtonNumber == 4)
	{
		if (GetTheme()->m_pShowWorkWeek)
		{
			GetTheme()->m_pShowWorkWeek->SetText(sButtonText);
			GetTheme()->m_pShowWorkWeek->SetVisible(bShow);
		}
	}
	else if (iButtonNumber == 5)
	{
		if (GetTheme()->m_pShowFullWeek)
		{
			GetTheme()->m_pShowFullWeek->SetText(sButtonText);
			GetTheme()->m_pShowFullWeek->SetVisible(bShow);
		}
	}
	else if (iButtonNumber == 6)
	{
		if (GetTheme()->m_pShowMcWeek)
		{
			GetTheme()->m_pShowMcWeek->SetText(sButtonText);
			GetTheme()->m_pShowMcWeek->SetVisible(bShow);
		}
	}
}

void CXTPCalendarCaptionBarControl::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp)
{
	UNREFERENCED_PARAMETER(bCalcValidRects);

	if (!GetTheme())
		return;

	RECT* pClient = &lpncsp->rgrc[0];

	pClient->left += m_Borders.left;
	pClient->top += m_Borders.top;

	pClient->right = max(pClient->left, pClient->right - m_Borders.right);
	pClient->bottom = max(pClient->top, pClient->bottom - m_Borders.bottom);
}

void CXTPCalendarCaptionBarControl::OnNcPaint()
{
	CXTPWindowRect rc(this);
	rc.OffsetRect(-rc.TopLeft());

	CWindowDC dc(this);

	if (GetTheme())
		GetTheme()->DrawBorders(&dc, rc, m_Borders);
}

void CXTPCalendarCaptionBarControl::UpdateBorders()
{
	if (!GetSafeHwnd())
		return;

	::SetWindowPos(m_hWnd, NULL, 0, 0, 0, 0,
		SWP_DRAWFRAME | SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE);

	Invalidate(NULL);
}

void CXTPCalendarCaptionBarControl::SetBorders(int nLeft, int nTop, int nRight, int nBottom)
{
	m_Borders.SetRect(nLeft, nTop, nRight, nBottom);
	UpdateBorders();
}

CRect CXTPCalendarCaptionBarControl::CalcMinRect(BOOL bExactSize)
{
	if (!m_pTheme)
		return CRect(0, 0, 0, 0);

	CRect rcMin = m_pTheme->CalcMinRect(NULL, bExactSize);
	if (rcMin.Height() > 0)
	{
		CRect rcBorders = GetBorders();

		rcMin.left += rcBorders.left + rcBorders.right;
		rcMin.bottom += rcBorders.top + rcBorders.bottom;
	}

	return rcMin;
}


void CXTPCalendarCaptionBarControl::SetTheme(CXTPCalendarCaptionBarTheme* pTheme)
{
	ASSERT(pTheme);
	if (pTheme)
	{
		CMDTARGET_RELEASE(m_pTheme);
		m_pTheme = pTheme;
		m_pTheme->SetOwner(this);
		m_pTheme->RefreshMetrics();
	}
}

void CXTPCalendarCaptionBarControl::AdjustLayout(CDC* pDC, const CRect* prcWnd)
{
	CXTPClientRect rcClient(this);

	if (prcWnd)
		((CRect&)rcClient) = *prcWnd;

	if (m_pTheme)
		m_pTheme->AdjustLayout(pDC, rcClient);
}

void CXTPCalendarCaptionBarControl::OnPaint()
{
	CXTPClientRect rc(this);

	if (rc.IsRectEmpty())
		return;

	CPaintDC dc(this); // device context for painting

	CXTPBufferDC memDC(dc.GetSafeHdc(), rc);

	if (m_pTheme)
		m_pTheme->Draw(&memDC);
}

BOOL CXTPCalendarCaptionBarControl::OnEraseBkgnd(CDC* /*pDC*/)
{
	return TRUE;
}

void CXTPCalendarCaptionBarControl::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	if (!GetSafeHwnd())
		return;

	CRect rcRect(0, 0, cx, cy);
	if (m_pTheme)
		m_pTheme->AdjustLayout(NULL, rcRect);

}

void CXTPCalendarCaptionBarControl::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_pTheme)
		m_pTheme->OnLButtonDown(nFlags, point);

	if (GetCapture() != this)
		SetCapture();
}

void CXTPCalendarCaptionBarControl::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_pTheme)
		m_pTheme->OnLButtonUp(nFlags, point);

	if (GetCapture() == this)
		ReleaseCapture();
}

void CXTPCalendarCaptionBarControl::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_pTheme)
		m_pTheme->OnMouseMove(nFlags, point);
}

void CXTPCalendarCaptionBarControl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (m_pTheme)
		m_pTheme->OnKeyDown(nChar, nRepCnt, nFlags);
}

void CXTPCalendarCaptionBarControl::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (m_pTheme)
		m_pTheme->OnKeyUp(nChar, nRepCnt, nFlags);
}

void CXTPCalendarCaptionBarControl::OnTimer(UINT_PTR uTimerID)
{
	if (m_pTheme)
		m_pTheme->OnTimer(uTimerID);
}

void CXTPCalendarCaptionBarControl::OnDestroy()
{
	if (m_pTheme)
		m_pTheme->OnBeforeDestroy();
}

void CXTPCalendarCaptionBarControl::OnKillFocus (CWnd* pNewWnd)
{
	if (m_pTheme)
		m_pTheme->OnKillFocus(pNewWnd);
}

void CXTPCalendarCaptionBarControl::EnableTimeline(BOOL bEnable)
{
	if (m_pTheme)
		m_pTheme->m_bTimelineEnable = bEnable;
}

BOOL CXTPCalendarCaptionBarControl::IsTimelineEnable()
{
	if (m_pTheme)
		return m_pTheme->m_bTimelineEnable;
	else
		return FALSE;
}

void CXTPCalendarCaptionBarControl::EnableMulticolumns(BOOL bEnable)
{
	if (m_pTheme)
		m_pTheme->m_bMulticolumnsEnable = bEnable;
}

BOOL CXTPCalendarCaptionBarControl::IsMulticolumnsEnable()
{
	if (m_pTheme)
		return m_pTheme->m_bMulticolumnsEnable;
	else
		return FALSE;
}

BOOL CXTPCalendarCaptionBarControl::IsUseOneLine()
{
	if (GetTheme())
		return GetTheme()->m_bIsOneLine;
	else
		return FALSE;
}

#include "XTPCalendarControlView.h"

void CXTPCalendarCaptionBarControl::UseOneLine(BOOL bSet)
{
	if (GetTheme())
	{
		GetTheme()->m_bIsOneLine = bSet;

		GetTheme()->RefreshMetrics();

		CXTPCalendarControlView* pView = DYNAMIC_DOWNCAST(CXTPCalendarControlView, GetParent());
		if (pView)
			pView->_ResizeControls();
	}
}

//////////////////////////////////////////////////////////////////////////

CXTPCalendarCaptionBarTheme::CXTPCalendarCaptionBarTheme()
{
	m_pSink = new CXTPNotifySink();

	m_themeButton = new CXTPWinThemeWrapper();


	m_pCaptionBar   = NULL;
	m_pCalendar     = NULL;
	m_nCheckHotStateTimerID = 0;

	m_bIsSwitchViewButtons = TRUE;
	m_bIsScrollDateButtons = TRUE;
	m_bIsDateLabel = TRUE;
	m_bTimelineEnable = TRUE;
	m_bMulticolumnsEnable = TRUE;
	m_bIsExpandButton = FALSE;
	m_bIsOneLine = FALSE; //Old Layout - default
	m_pConnection = NULL;
	m_dScale = 1.0;

	// Init Theme Parts
	m_pButtonViewDay = new CSwitchViewButtonPart(this, xtpCalendarCaptionBar_ButtonViewDay);
	m_pButtonViewWeek = new CSwitchViewButtonPart(this, xtpCalendarCaptionBar_ButtonViewWeek);
	m_pButtonViewMonth = new CSwitchViewButtonPart(this, xtpCalendarCaptionBar_ButtonViewMonth);

	m_pButtonViewDay->SetText(XTPResourceManager()->LoadString(XTP_IDS_CALENDAR_DAY));
	m_pButtonViewWeek->SetText(XTPResourceManager()->LoadString(XTP_IDS_CALENDAR_WEEK));
	m_pButtonViewMonth->SetText(XTPResourceManager()->LoadString(XTP_IDS_CALENDAR_MONTH));

	m_pShowWorkWeek = new CRadioButtonPart(this, xtpCalendarCaptionBar_ShowWorkWeek);
	m_pShowFullWeek = new CRadioButtonPart(this, xtpCalendarCaptionBar_ShowFullWeek);

	CString s = XTPResourceManager()->LoadString(XTP_IDS_CALENDAR_SHOWWORKWEK);
	s.Replace(_T("Show w"), _T("W"));
	m_pShowWorkWeek->SetText(s);
	m_pShowWorkWeek->SetCheck(0);

	s = XTPResourceManager()->LoadString(XTP_IDS_CALENDAR_SHOWFULLKWEK);
	s.Replace(_T("Show f"), _T("F"));
	m_pShowFullWeek->SetText(s);
	m_pShowFullWeek->SetCheck(0);

	m_pShowTimeline = new CCheckboxButtonPart(this, xtpCalendarCaptionBar_ShowTimeline);
	m_pShowTimeline->SetText(_T("TimeLine mode"));
	m_pShowTimeline->SetCheck(0);

	m_pShowMcWeek = new CCheckboxButtonPart(this, xtpCalendarCaptionBar_MultiColumnsWeek);
	//m_pShowMcWeek = new CRadioButtonPart(this, xtpCalendarCaptionBar_MultiColumnsWeek);
	m_pShowMcWeek->SetText(_T("MultiColumn mode"));
	m_pShowMcWeek->SetCheck(0);

	m_pScrollPrevDate = new CScrollDateButtonPart(this, xtpCalendarCaptionBar_ScrollPrevDate);
	m_pScrollNextDate = new CScrollDateButtonPart(this, xtpCalendarCaptionBar_ScrollNextDate);

	m_pDateLabel = new CDateLabelPart(this, xtpCalendarCaptionBar_DateLabel);

	m_pExpandButton = new CExpandButtonPart(this, xtpCalendarCaptionBar_ButtonExpand);

	m_nPaintTheme = xtpCalendarThemeResource;

	RefreshMetrics();
}

CXTPCalendarCaptionBarTheme::~CXTPCalendarCaptionBarTheme()
{
	m_pSink->Delete();

	CMDTARGET_RELEASE(m_pConnection);

	CMDTARGET_RELEASE(m_pButtonViewDay);
	CMDTARGET_RELEASE(m_pButtonViewWeek);
	CMDTARGET_RELEASE(m_pButtonViewMonth);

	CMDTARGET_RELEASE(m_pShowWorkWeek);
	CMDTARGET_RELEASE(m_pShowFullWeek);

	CMDTARGET_RELEASE(m_pShowTimeline);
	CMDTARGET_RELEASE(m_pShowMcWeek);

	CMDTARGET_RELEASE(m_pScrollPrevDate);
	CMDTARGET_RELEASE(m_pScrollNextDate);

	CMDTARGET_RELEASE(m_pDateLabel);
	CMDTARGET_RELEASE(m_pExpandButton);

	CMDTARGET_RELEASE(m_pCalendar);

	SAFE_DELETE(m_themeButton);
}

void CXTPCalendarCaptionBarTheme::OnBeforeDestroy()
{
	m_pSink->UnadviseAll();
	CMDTARGET_RELEASE(m_pCalendar);
}

void CXTPCalendarCaptionBarTheme::SetOwner(CXTPCalendarCaptionBarControl* pCaptionBar)
{
	ASSERT(pCaptionBar);
	if (pCaptionBar)
	{
		m_pCaptionBar = pCaptionBar;
	}
}

void CXTPCalendarCaptionBarTheme::AttachCalendar(CXTPCalendarControl* pCalendar)
{
	m_pSink->UnadviseAll();
	CMDTARGET_RELEASE(m_pCalendar);

	m_pCalendar = pCalendar;
	CMDTARGET_ADDREF(m_pCalendar);

	if (m_pCalendar && m_pCalendar->GetConnection())
	{
		CXTPNotifyConnection* pConnection = m_pCalendar->GetConnection();

		m_pSink->Advise(pConnection, XTP_NC_CALENDARVIEWWASCHANGED, CreateNotfySinkClassDelegate(this, &CXTPCalendarCaptionBarTheme::OnEvent_Calendar));
		m_pSink->Advise(pConnection, XTP_NC_CALENDAR_THEME_CHANGED, CreateNotfySinkClassDelegate(this, &CXTPCalendarCaptionBarTheme::OnEvent_Calendar));

		UpdateControlsState();

		if (XTPResourceImages() && XTPResourceImages()->GetConnection())
		{
			m_pSink->Advise(XTPResourceImages()->GetConnection(), XTP_NC_COMMON_RESOURCEIMAGES_CHANGED,
				CreateNotfySinkClassDelegate(this, &CXTPCalendarCaptionBarTheme::OnEvent_ResourceImagesChanged));
		}

		SetPaintTheme(GetCalendarCtrl()->GetPaintTheme());
	}
}

CXTPNotifyConnection* CXTPCalendarCaptionBarTheme::GetConnection()
{
	if (!m_pConnection)
		m_pConnection = new CXTPNotifyConnection();

	return m_pConnection;
}

int CXTPCalendarCaptionBarTheme::GetChildHandlersCount() const
{
	return m_arMembers.GetCount();
}

CXTPCalendarWMHandler* CXTPCalendarCaptionBarTheme::GetChildHandlerAt(int nIndex) const
{
	return m_arMembers.GetAt(nIndex);
}

void CXTPCalendarCaptionBarTheme::OnMouseMove(UINT nFlags, CPoint point)
{
	CXTPCalendarWMHandler::OnMouseMove(nFlags, point);

	if (GetOwnerWnd() && m_nCheckHotStateTimerID == 0)
	{
		m_nCheckHotStateTimerID = GetOwnerWnd()->SetTimer(1, 100, NULL);
	}
}

BOOL CXTPCalendarCaptionBarTheme::OnTimer(UINT_PTR uTimerID)
{
	if (uTimerID == m_nCheckHotStateTimerID && GetOwnerWnd() &&
		::GetCapture() != GetOwnerWnd()->GetSafeHwnd())
	{
		BOOL bMouseLeave;
		BOOL bChanged = _UpdateControlsHotState(&bMouseLeave);

		if (bMouseLeave)
		{
			GetOwnerWnd()->KillTimer(1);
			m_nCheckHotStateTimerID = 0;
		}

		if (bChanged)
			Redraw();

		return TRUE;
	}
	return FALSE;
}

CRect CXTPCalendarCaptionBarTheme::CalcMinRect(CDC* /*pDC*/, BOOL bExactSize)
{
	if (!IsSwitchViewButtons()
		&& !IsScrollDateButtons()
		&& !IsDateLabel()
		&& !IsExpandButton())
		return CRect(0, 0, 0, 0);

	int nHeight = 0;
	if (m_bIsOneLine)
	{
		nHeight += 27;
	}
	else
	{
		if (IsSwitchViewButtons())
			nHeight += 27;

		int nH1 = (IsScrollDateButtons() || IsDateLabel()) ? 32 : 0;
		int nH2 = IsExpandButton() ? 30 : 0;

		nHeight += max(nH2, nH1);

		if (!bExactSize)
			nHeight += 11;
	}
	return CRect(0, 0, 525, nHeight);
};

void CXTPCalendarCaptionBarTheme::AdjustLayout(CDC* pDC, const CRect& rcRect)
{
	if (!pDC && GetOwnerWnd() && GetOwnerWnd()->m_hWnd)
	{
		CClientDC dc(GetOwnerWnd());
		AdjustLayout(&dc, rcRect);
		return;
	}

	if (rcRect.IsRectNull() || rcRect.IsRectEmpty())
		return;

	m_rcRect = rcRect;
	CRect rc00(rcRect.left, rcRect.top, rcRect.left, rcRect.top);
	CRect rcClient = m_rcRect;

	CRect rc = rcClient;

	int nNextY = rc.top;
	BOOL bSwitchViewButtons = IsSwitchViewButtons();
	BOOL bScrollDateButtons = IsScrollDateButtons();
	BOOL bShowExpandButton = IsExpandButton();
	BOOL bShowDateLabel = IsDateLabel();

	m_pButtonViewDay->SetVisible(bSwitchViewButtons && !m_pButtonViewDay->GetText().IsEmpty());
	m_pButtonViewWeek->SetVisible(bSwitchViewButtons && !m_pButtonViewWeek->GetText().IsEmpty());
	m_pButtonViewMonth->SetVisible(bSwitchViewButtons && !m_pButtonViewMonth->GetText().IsEmpty());

	int iWid1 = 83;
	int iWid2 = 20;
	int iWid3 = 5;

	if (!m_bIsOneLine) //TWO LINE MODE
	{
		if (bSwitchViewButtons)
		{
			rc.bottom = rc.top + 26;

			if (!m_pButtonViewDay->GetText().IsEmpty())
				rc.right = rc.left + iWid1;
			else
				rc.right = rc.left;
			m_pButtonViewDay->AdjustLayout(pDC, rc);

			rc.left = m_pButtonViewDay->GetRect().right;
			if (!m_pButtonViewWeek->GetText().IsEmpty())
				rc.right = rc.left + iWid1;
			else
				rc.right = rc.left;
			m_pButtonViewWeek->AdjustLayout(pDC, rc);

			rc.left = m_pButtonViewWeek->GetRect().right;
			if (!m_pButtonViewMonth->GetText().IsEmpty())
				rc.right = rc.left + iWid1;
			else
				rc.right = rc.left;
			m_pButtonViewMonth->AdjustLayout(pDC, rc);

			rc.left = m_pButtonViewMonth->GetRect().right + 21;
			rc.right = rc.left + iWid3;

			if (m_bTimelineEnable && !m_pShowTimeline->GetText().IsEmpty())
			{
				m_pShowTimeline->AdjustLayout(pDC, rc);

				rc.left = m_pShowTimeline->GetRect().right + 11;
				rc.right = rc.left + iWid3;
			}

			if (!m_pShowWorkWeek->GetText().IsEmpty())
			{
				m_pShowWorkWeek->AdjustLayout(pDC, rc);

				rc.left = m_pShowWorkWeek->GetRect().right + 11;
				rc.right = rc.left + iWid3;
			}

			if (!m_pShowFullWeek->GetText().IsEmpty())
			{
				m_pShowFullWeek->AdjustLayout(pDC, rc);
				rc.left = m_pShowFullWeek->GetRect().right + 11;
				rc.right = rc.left + iWid3;
			}

			if (!m_pShowMcWeek->GetText().IsEmpty())
				m_pShowMcWeek->AdjustLayout(pDC, rc);

			rc = m_pButtonViewDay->GetRect();

			nNextY = rc.bottom + 1;
		}
		else
		{
			m_pButtonViewDay->AdjustLayout(pDC, rc00);
			m_pButtonViewWeek->AdjustLayout(pDC, rc00);
			m_pButtonViewMonth->AdjustLayout(pDC, rc00);

			m_pShowWorkWeek->AdjustLayout(pDC, rc00);
			m_pShowFullWeek->AdjustLayout(pDC, rc00);
			m_pShowTimeline->AdjustLayout(pDC, rc00);
			m_pShowMcWeek->AdjustLayout(pDC, rc00);

			m_pShowWorkWeek->SetVisible(FALSE);
			m_pShowFullWeek->SetVisible(FALSE);
			m_pShowTimeline->SetVisible(FALSE);
			m_pShowMcWeek->SetVisible(FALSE);

			nNextY = rc.top;
		}

		int nNextX = rc.left;

		m_pScrollPrevDate->SetVisible(bScrollDateButtons);
		m_pScrollNextDate->SetVisible(bScrollDateButtons);

		if (bScrollDateButtons)
		{
			rc.top = nNextY + 12;
			if (rc.top + 20 + 3 > rcClient.bottom)
				rc.top = max(nNextY + 3, rcClient.bottom - 23);

			rc.bottom = rc.top + 20;

			rc.left += m_nScrollDateButtonsMargin;

			rc.right = rc.left + iWid2;

			m_pScrollPrevDate->AdjustLayout(pDC, rc);

			rc.left = m_pScrollPrevDate->GetRect().right + 9;
			rc.right = rc.left + iWid2;

			m_pScrollNextDate->AdjustLayout(pDC, rc);

			nNextX = rc.right;
		}
		else
		{
			m_pScrollPrevDate->AdjustLayout(pDC, rc00);
			m_pScrollNextDate->AdjustLayout(pDC, rc00);
		}

		m_pExpandButton->SetVisible(bShowExpandButton);

		int nRightX = rcClient.right;
		if (bShowExpandButton)
		{
			int nDelta = (bScrollDateButtons || bShowDateLabel) ? 5 : 3;
			rc.top = nNextY + 13;
			if (rc.top + 18 + nDelta > rcClient.bottom)
				rc.top = max(nNextY + 1, rcClient.bottom - 18 - nDelta);

			rc.bottom = rc.top + 18;
			rc.right = rcClient.right - 2;
			rc.left = rc.right - 18;

			m_pExpandButton->AdjustLayout(pDC, rc);

			nRightX = m_pExpandButton->GetRect().left;
		}
		else
		{
			m_pExpandButton->AdjustLayout(pDC, rc00);
		}

		m_pDateLabel->SetVisible(bShowDateLabel);

		if (bShowDateLabel)
		{
			int nDelta = 3;
			rc.top = nNextY + 12;
			if (rc.top + 20 + nDelta > rcClient.bottom)
				rc.top = max(nNextY + 3, rcClient.bottom - 20 - nDelta);

			rc.bottom = rc.top + 22;
			rc.left = nNextX + 11;
			rc.right = nRightX - 2;

			m_pDateLabel->AdjustLayout(pDC, rc);
		}
		else
		{
			m_pDateLabel->AdjustLayout(pDC, rc00);
		}
	}
	else //ONE LINE MODE
	{
		int nNextX(0);
		if (bSwitchViewButtons)
		{
			if (!m_pButtonViewDay->GetText().IsEmpty())
				rc.right = rc.left + iWid1;
			else
				rc.right = rc.left;
			m_pButtonViewDay->AdjustLayout(pDC, rc);

			rc.left = m_pButtonViewDay->GetRect().right;
			if (!m_pButtonViewWeek->GetText().IsEmpty())
				rc.right = rc.left + iWid1;
			else
				rc.right = rc.left;
			m_pButtonViewWeek->AdjustLayout(pDC, rc);

			rc.left = m_pButtonViewWeek->GetRect().right;
			if (!m_pButtonViewMonth->GetText().IsEmpty())
				rc.right = rc.left + iWid1;
			else
				rc.right = rc.left;
			m_pButtonViewMonth->AdjustLayout(pDC, rc);

			rc.left = m_pButtonViewMonth->GetRect().right + 21;
			rc.right = rc.left + iWid3;

			if (m_bTimelineEnable && !m_pShowTimeline->GetText().IsEmpty())
			{
				m_pShowTimeline->AdjustLayout(pDC, rc);

				rc.left = m_pShowTimeline->GetRect().right + 11;
				rc.right = rc.left + iWid3;
			}

			if (!m_pShowWorkWeek->GetText().IsEmpty())
			{
				m_pShowWorkWeek->AdjustLayout(pDC, rc);
				rc.left = m_pShowWorkWeek->GetRect().right + 11;
				rc.right = rc.left + iWid3;
			}

			if (!m_pShowFullWeek->GetText().IsEmpty())
			{
				m_pShowFullWeek->AdjustLayout(pDC, rc);
				rc.left = m_pShowFullWeek->GetRect().right + 11;
				rc.right = rc.left + iWid3;
			}

			if (!m_pShowMcWeek->GetText().IsEmpty())
				m_pShowMcWeek->AdjustLayout(pDC, rc);

			nNextX = m_pShowMcWeek->GetRect().right + 11;
		}
		else
		{
			m_pButtonViewDay->AdjustLayout(pDC, rc00);
			m_pButtonViewWeek->AdjustLayout(pDC, rc00);
			m_pButtonViewMonth->AdjustLayout(pDC, rc00);

			m_pShowWorkWeek->AdjustLayout(pDC, rc00);
			m_pShowFullWeek->AdjustLayout(pDC, rc00);
			m_pShowTimeline->AdjustLayout(pDC, rc00);
			m_pShowMcWeek->AdjustLayout(pDC, rc00);

			m_pShowWorkWeek->SetVisible(FALSE);
			m_pShowFullWeek->SetVisible(FALSE);
			m_pShowTimeline->SetVisible(FALSE);
			m_pShowMcWeek->SetVisible(FALSE);
		}

		m_pScrollPrevDate->SetVisible(bScrollDateButtons);
		m_pScrollNextDate->SetVisible(bScrollDateButtons);

		if (bScrollDateButtons)
		{
			rc.left = nNextX + 20;
			rc.right = rc.left + iWid2;

			m_pScrollPrevDate->AdjustLayout(pDC, rc);

			rc.left = m_pScrollPrevDate->GetRect().right + 9;
			rc.right = rc.left + iWid2;

			m_pScrollNextDate->AdjustLayout(pDC, rc);

			nNextX = rc.right;
		}
		else
		{
			m_pScrollPrevDate->AdjustLayout(pDC, rc00);
			m_pScrollNextDate->AdjustLayout(pDC, rc00);
		}

		m_pExpandButton->SetVisible(bShowExpandButton);

		int nRightX = rcClient.right;
		if (bShowExpandButton)
		{
			rc.right = rcClient.right - 2;
			rc.left = rc.right - 18;

			m_pExpandButton->AdjustLayout(pDC, rc);

			nRightX = m_pExpandButton->GetRect().left;
		}
		else
		{
			m_pExpandButton->AdjustLayout(pDC, rc00);
		}
		m_pDateLabel->SetVisible(bShowDateLabel);

		if (bShowDateLabel)
		{
			rc.left = nNextX + 20;
			rc.right = nRightX - 2;

			m_pDateLabel->AdjustLayout(pDC, rc);
		}
		else
		{
			m_pDateLabel->AdjustLayout(pDC, rc00);
		}
	}
}

COLORREF CXTPCalendarCaptionBarTheme::GetColor2(LPCTSTR pcszColorName, COLORREF clrDefault)
{
	COLORREF clrColor = COLORREF_NULL;

	CXTPResourceImages* pImages = XTPResourceImages();

	if (pImages->IsValid())
	{
		clrColor = pImages->GetImageColor(_T("CalendarControl"), pcszColorName);
	}

	if (clrColor == COLORREF_NULL)
	{
		clrColor = clrDefault;
	}

	return clrColor;
}

void CXTPCalendarCaptionBarTheme::RefreshMetrics()
{
	m_themeButton->OpenTheme(0, L"BUTTON");

	LOGFONT lfBaseFont;
	::ZeroMemory(&lfBaseFont, sizeof(lfBaseFont));
	BOOL bUseFont2007 = FALSE;

	m_nScrollDateButtonsMargin = 20;

	if (GetPaintTheme() >= xtpCalendarThemeResource)
	{
		m_clrSwitchViewBarBk.SetStandardValue(GetColor2(_T("CaptionBarSwitchViewBarBk"), RGB(173, 209, 255)));
		m_clrScrollDateBarBk.SetStandardValue(GetColor2(_T("CaptionBarDateBarBk"), RGB(227, 239, 255)));

		m_clrBorders.SetStandardValue(GetColor2(_T("CaptionBarBorders"), RGB(101, 147, 201)));
		m_clrTextColor.SetStandardValue(GetColor2(_T("CaptionBarTextColor"), RGB(0, 0, 0)));

		// Try to set default Office 2007 font,
		// otherwise icon font used.
		bUseFont2007 = XTPDrawHelpers()->FontExists(XTP_CALENDAR_OFFICE2007_FONT_NAME);

		if (XTPResourceImages()->GetImageInt(_T("CalendarControl"), _T("FlatStyle"), 0))
		{
			m_nScrollDateButtonsMargin = 4;
		}
	}
	else
	{
		//m_clrSwitchViewBarBk.SetStandardValue(RGB(192, 192, 192));
		m_clrSwitchViewBarBk.SetStandardValue(GetSysColor(COLOR_BTNFACE));

		m_clrBorders.SetStandardValue(RGB(130, 130, 130));
		m_clrTextColor.SetStandardValue(RGB(255, 255, 255));

		if (GetPaintTheme() == xtpCalendarThemeOffice2000)
			m_clrScrollDateBarBk.SetStandardValue(RGB(146, 146, 146));
		else if (GetPaintTheme() == xtpCalendarThemeOffice2013)
		{
			m_clrScrollDateBarBk.SetStandardValue(RGB(255, 255, 255));
			m_clrTextColor.SetStandardValue(RGB(38, 38, 38));
			m_clrBorders.SetStandardValue(RGB(213, 213, 213));

			m_clrSwitchViewBarBk.SetStandardValue(RGB(255, 255, 255));
		}
		else
		{
			m_clrScrollDateBarBk.SetStandardValue(RGB(130, 130, 130));
		}
	}

	if (bUseFont2007)
	{
		STRCPY_S(lfBaseFont.lfFaceName, LF_FACESIZE, XTP_CALENDAR_OFFICE2007_FONT_NAME);
		lfBaseFont.lfCharSet = XTPResourceManager()->GetFontCharset();
		lfBaseFont.lfHeight = -11;
		//lfBaseFont.lfHeight = -10;
		lfBaseFont.lfWeight = FW_NORMAL;

		if (XTPSystemVersion()->IsClearTypeTextQualitySupported())
			lfBaseFont.lfQuality = 5;
	}
	else
	{
		VERIFY(CXTPDrawHelpers::GetIconLogFont(&lfBaseFont));
	}

	m_fntTextFont.SetStandardValue(&lfBaseFont);

	int nCount = m_arMembers.GetCount();
	for (int i = 0; i < nCount; i++)
	{
		CXTPCalendarCaptionBarThemePart* pPart = m_arMembers.GetAt(i);
		ASSERT(pPart);
		if (pPart)
			pPart->RefreshMetrics();
	}

	AdjustLayout(NULL, GetRect());
}

void CXTPCalendarCaptionBarTheme::Draw(CDC* pDC)
{

#ifdef _DEBUG
	pDC->FillSolidRect(GetRect(), RGB(255, 0, 0));
#endif

	CRect rcTopPart = m_pButtonViewDay->GetRect();

	if (IsSwitchViewButtons())
	{
		// Fill SwitchViewBar
		rcTopPart.right = m_rcRect.right;
		pDC->FillSolidRect(rcTopPart, m_clrSwitchViewBarBk);

		rcTopPart.top = rcTopPart.bottom;
		rcTopPart.bottom++;

		pDC->FillSolidRect(rcTopPart, m_clrBorders);
	}

	// Fill ScrollDateBar
	CRect rcBottomPart = m_rcRect;
	rcBottomPart.top = rcTopPart.bottom;
	pDC->FillSolidRect(rcBottomPart, m_clrScrollDateBarBk);

	//DrawBorders(pDC, m_rcRect, m_rcRect);

	// Draw members
	int nCount = m_arMembers.GetCount();
	for (int i = 0; i < nCount; i++)
	{
		CXTPCalendarCaptionBarThemePart* pPart = m_arMembers.GetAt(i);
		ASSERT(pPart);
		if (pPart)
			pPart->Draw(pDC);
	}
}

void CXTPCalendarCaptionBarTheme::DrawBorders(CDC* pDC, const CRect& rcRect, const CRect& Borders)
{
	// Draw borders
	CRect rc;
	if (Borders.top)
	{
		rc = rcRect;
		rc.bottom = rc.top + Borders.top;
		pDC->FillSolidRect(rc, m_clrBorders);
	}

	if (Borders.bottom)
	{
		rc = rcRect;
		rc.top = rc.bottom - Borders.bottom;
		pDC->FillSolidRect(rc, m_clrBorders);
	}

	if (Borders.left)
	{
		rc = rcRect;
		rc.right = rc.left + Borders.left;
		pDC->FillSolidRect(rc, m_clrBorders);
	}

	if (Borders.right)
	{
		rc = rcRect;
		rc.left = rc.right - Borders.right;
		pDC->FillSolidRect(rc, m_clrBorders);
	}
}


//////////////////////////////////////////////////////////////////////////
CXTPCalendarCaptionBarThemePart::CXTPCalendarCaptionBarThemePart(CXTPCalendarCaptionBarTheme* pTheme, int nPartID)
{
	ASSERT(pTheme);
	m_pTheme = pTheme;
	m_nPartID = nPartID;
	m_nState = xtpBtnStateNormal;
	m_bVisible = TRUE;

	if (m_pTheme)
		m_pTheme->m_arMembers.Add(this, TRUE);
}

CXTPCalendarCaptionBarThemePart::~CXTPCalendarCaptionBarThemePart()
{

}

void CXTPCalendarCaptionBarThemePart::AdjustLayout(CDC* /*pDC*/, const CRect& rcRect)
{
	m_rcRect = rcRect;
}

COLORREF CXTPCalendarCaptionBarThemePart::GetTextColor()
{
	if (m_clrTextColor.IsDefaultValue() && m_pTheme)
		return (COLORREF)m_pTheme->m_clrTextColor;

	return (COLORREF)m_clrTextColor;
}

void CXTPCalendarCaptionBarThemePart::Draw(CDC* /*pDC*/)
{
};

BOOL CXTPCalendarCaptionBarThemePart::HitTest(const CPoint& pt) const
{
	return IsVisible() && m_rcRect.PtInRect(pt);
};

void CXTPCalendarCaptionBarThemePart::Redraw(BOOL bUpdateNow)
{
	if (m_pTheme)
		m_pTheme->Redraw(bUpdateNow);
}

void CXTPCalendarCaptionBarTheme::Redraw(BOOL bUpdateNow)
{
	if (GetOwnerWnd() && GetOwnerWnd()->GetSafeHwnd())
	{
		GetOwnerWnd()->Invalidate(FALSE);

		if (bUpdateNow)
			GetOwnerWnd()->UpdateWindow();
	}
}

void CXTPCalendarCaptionBarThemePart::OnMouseMove(UINT nFlags, CPoint point)
{
	UNREFERENCED_PARAMETER(nFlags);

	int nStatePrev = m_nState;

	if (HitTest(point))
		m_nState = m_nState | xtpBtnStateMouseOver;
	else
		m_nState = m_nState & (~(xtpBtnStateMouseOver));

	if (nStatePrev != m_nState)
		Redraw();
}

BOOL CXTPCalendarCaptionBarThemePart::OnLButtonDown(UINT nFlags, CPoint point)
{
	UNREFERENCED_PARAMETER(nFlags);

	int nStatePrev = m_nState;

	BOOL bHitTest = HitTest(point);
	if (bHitTest)
	{
		m_nState = m_nState | xtpBtnStatePressed | xtpBtnStateFocused;
	}
	else
		m_nState = m_nState & (~(xtpBtnStatePressed | xtpBtnStateFocused));

	if (nStatePrev != m_nState)
		Redraw();

	return FALSE; // to allow receive this notification for all other parts (to clear state)
}

BOOL CXTPCalendarCaptionBarThemePart::OnLButtonUp(UINT nFlags, CPoint point)
{
	UNREFERENCED_PARAMETER(nFlags);

	int nStatePrev = m_nState;
	BOOL bHitTest = HitTest(point);

	if (bHitTest && (m_nState & (xtpBtnStatePressed | xtpBtnStateFocused)))
	{
		m_nState = m_nState & (~xtpBtnStatePressed);

		if (m_pTheme && IsVisible())
			m_pTheme->OnPartClick(m_nPartID);
	}

	m_nState = m_nState & (~xtpBtnStatePressed);

	if (nStatePrev != m_nState)
		Redraw();

	return FALSE; // to allow receive this notification for all other parts (to clear state)
}

void CXTPCalendarCaptionBarTheme::OnEvent_Calendar(XTP_NOTIFY_CODE Event, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam); UNREFERENCED_PARAMETER(lParam);


	if (Event == XTP_NC_CALENDARVIEWWASCHANGED)
	{
		UpdateControlsState();
	}
	else if (Event == XTP_NC_CALENDAR_THEME_CHANGED)
	{
		SetPaintTheme(GetCalendarCtrl()->GetPaintTheme());
	}
	else {
		ASSERT(FALSE);
		return;
	}

	Redraw();
}

void CXTPCalendarCaptionBarTheme::OnEvent_ResourceImagesChanged(XTP_NOTIFY_CODE Event, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam); UNREFERENCED_PARAMETER(lParam);

	if (Event == XTP_NC_COMMON_RESOURCEIMAGES_CHANGED)
	{
		RefreshMetrics();

		Redraw();
	}
}

void CXTPCalendarCaptionBarTheme::UpdateControlsState()
{
	if (!GetCalendarCtrl() || !GetCalendarCtrl()->GetActiveView())
		return;

	m_pShowWorkWeek->SetVisible(FALSE);
	m_pShowFullWeek->SetVisible(FALSE);
	m_pShowMcWeek->SetVisible(FALSE);

	m_pButtonViewDay->SetCheck(0);
	m_pButtonViewWeek->SetCheck(0);
	m_pButtonViewMonth->SetCheck(0);

	m_pShowTimeline->SetVisible(m_bTimelineEnable);

	int iScale = GetCalendarCtrl()->GetTimeLineScale();
	int nViewType = GetCalendarCtrl()->GetActiveView()->GetViewType();

	//BOOL bTimelineMode = m_pShowTimeline->GetCheck();

	BOOL bTimelineMode = GetCalendarCtrl()->m_bTimelineMode;
	m_pShowTimeline->SetCheck(bTimelineMode);

	if (nViewType == xtpCalendarDayView ||
		(bTimelineMode && iScale == xtpTSPID_Day))
	{
		m_pButtonViewDay->SetCheck(1);
	}
	else if (nViewType == xtpCalendarMonthView ||
		(bTimelineMode && iScale == xtpTSPID_Month))
	{
		m_pButtonViewMonth->SetCheck(1);
	}
	else
	{
		if (bTimelineMode &&
			(iScale == xtpTSPID_Week || iScale == xtpTSPID_WorkWeek))
			m_pButtonViewWeek->SetCheck(1);

		if (IsSwitchViewButtons())
		{
			m_pShowWorkWeek->SetVisible(!m_pShowWorkWeek->GetText().IsEmpty());
			m_pShowFullWeek->SetVisible(!m_pShowFullWeek->GetText().IsEmpty());
			m_pShowMcWeek->SetVisible(m_bMulticolumnsEnable && !m_pShowMcWeek->GetText().IsEmpty());
			m_pShowMcWeek->SetCheck(GetCalendarCtrl()->m_bMultiColumnWeekMode);
		}
		if (nViewType == xtpCalendarWorkWeekView)
		{
			m_pShowWorkWeek->SetCheck(1);
			m_pShowFullWeek->SetCheck(0);
			m_pButtonViewWeek->SetCheck(1);
		}
		else if (nViewType == xtpCalendarFullWeekView)
		{
			m_pShowWorkWeek->SetCheck(0);
			m_pShowFullWeek->SetCheck(1);
			m_pButtonViewWeek->SetCheck(1);
		}
		else if (nViewType == xtpCalendarWeekView)
		{
			m_pShowWorkWeek->SetCheck(0);
			m_pShowFullWeek->SetCheck(1);
			m_pButtonViewWeek->SetCheck(1);
		}
		else if (nViewType == xtpCalendarTimeLineView)
		{
			m_pShowTimeline->SetCheck(1);
		}
	}
}

void CXTPCalendarCaptionBarTheme::OnPartClick(int nPartID)
{
	BOOL bCancel = FALSE;
	if (m_pConnection)
		m_pConnection->SendEvent(XTP_NC_CALENDAR_CAPTIONBARPART_BEFORE_CLICK, (WPARAM)nPartID, (LPARAM)&bCancel);

	if (GetCalendarCtrl() && !bCancel)
		GetCalendarCtrl()->SendNotification(XTP_NC_CALENDAR_CAPTIONBARPART_BEFORE_CLICK, (WPARAM)nPartID, (LPARAM)&bCancel);

	if (bCancel)
		return;

	if (nPartID == xtpCalendarCaptionBar_ButtonExpand)
	{
		m_pExpandButton->SetCheck(m_pExpandButton->GetCheck() ? 0 : 1);

		Redraw();
	}

	if (GetCalendarCtrl() && GetCalendarCtrl()->GetActiveView())
	{
		BOOL bUpdate = TRUE;
		BOOL bTimelineMode = m_pShowTimeline->GetCheck();

		if (nPartID == xtpCalendarCaptionBar_ButtonViewDay)
		{
			GetCalendarCtrl()->SwitchActiveView(xtpCalendarDayView);
			GetCalendarCtrl()->SetFocus();
			if (bTimelineMode)
			{
				GetCalendarCtrl()->m_bTimelineMode = !GetCalendarCtrl()->m_bTimelineMode;
				GetCalendarCtrl()->OnCalendarTimeline();
			}
		}
		else if (nPartID == xtpCalendarCaptionBar_ButtonViewWeek)
		{
			BOOL bMultiColumnWeekMode = GetCalendarCtrl()->m_bMultiColumnWeekMode;
			if (bMultiColumnWeekMode)
				GetCalendarCtrl()->SwitchActiveView(xtpCalendarWorkWeekView);
			else
				GetCalendarCtrl()->SwitchActiveView(xtpCalendarFullWeekView);
			GetCalendarCtrl()->SetFocus();
			if (bTimelineMode)
			{
				GetCalendarCtrl()->m_bTimelineMode = !GetCalendarCtrl()->m_bTimelineMode;
				GetCalendarCtrl()->OnCalendarTimeline();
			}
		}
		else if (nPartID == xtpCalendarCaptionBar_ButtonViewMonth)
		{
			GetCalendarCtrl()->SwitchActiveView(xtpCalendarMonthView);
			GetCalendarCtrl()->SetFocus();
			if (bTimelineMode)
			{
				GetCalendarCtrl()->m_bTimelineMode = !GetCalendarCtrl()->m_bTimelineMode;
				GetCalendarCtrl()->OnCalendarTimeline();
			}
		}
		else if (nPartID == xtpCalendarCaptionBar_ShowWorkWeek)
		{
			GetCalendarCtrl()->SwitchActiveView(xtpCalendarWorkWeekView);
			m_pShowWorkWeek->SetCheck(1);
			m_pShowFullWeek->SetCheck(0);
			GetCalendarCtrl()->SetFocus();
			if (bTimelineMode)
			{
				GetCalendarCtrl()->m_bTimelineMode = !GetCalendarCtrl()->m_bTimelineMode;
				GetCalendarCtrl()->OnCalendarTimeline();
			}
		}
		else if (nPartID == xtpCalendarCaptionBar_ShowFullWeek)
		{
			GetCalendarCtrl()->SwitchActiveView(xtpCalendarFullWeekView);
			m_pShowWorkWeek->SetCheck(0);
			m_pShowFullWeek->SetCheck(1);
			GetCalendarCtrl()->SetFocus();
			if (bTimelineMode)
			{
				GetCalendarCtrl()->m_bTimelineMode = !GetCalendarCtrl()->m_bTimelineMode;
				GetCalendarCtrl()->OnCalendarTimeline();
			}
		}
		else if (nPartID == xtpCalendarCaptionBar_ShowTimeline)
		{
			m_pShowTimeline->SetCheck(!bTimelineMode);
			GetCalendarCtrl()->OnCalendarTimeline();
		}
		else if (nPartID == xtpCalendarCaptionBar_MultiColumnsWeek)
		{
			GetCalendarCtrl()->m_bMultiColumnWeekMode = !GetCalendarCtrl()->m_bMultiColumnWeekMode;
			int nViewType = GetCalendarCtrl()->GetActiveView()->GetViewType();
			if (nViewType == xtpCalendarWeekView)
			{
				GetCalendarCtrl()->SwitchActiveView(xtpCalendarFullWeekView);
				m_pShowWorkWeek->SetCheck(0);
				m_pShowFullWeek->SetCheck(1);
			}
			else if (nViewType == xtpCalendarWorkWeekView)
			{
				GetCalendarCtrl()->SwitchActiveView(xtpCalendarWorkWeekView);
				m_pShowWorkWeek->SetCheck(1);
				m_pShowFullWeek->SetCheck(0);
			}
			else if (nViewType == xtpCalendarFullWeekView)
			{
				GetCalendarCtrl()->SwitchActiveView(xtpCalendarFullWeekView);
				m_pShowWorkWeek->SetCheck(0);
				m_pShowFullWeek->SetCheck(1);
			}
			if (bTimelineMode)
			{
				GetCalendarCtrl()->m_bTimelineMode = !GetCalendarCtrl()->m_bTimelineMode;
				GetCalendarCtrl()->OnCalendarTimeline();
			}
		}
		else if (nPartID == xtpCalendarCaptionBar_ScrollPrevDate ||
				nPartID == xtpCalendarCaptionBar_ScrollNextDate)
		{
			BOOL bNext = nPartID == xtpCalendarCaptionBar_ScrollNextDate;
			COleDateTime dtSelStart, dtSelEnd, dtFirst;
			CXTPCalendarView* pActiveView = GetCalendarCtrl()->GetActiveView();
			int nViewType = pActiveView->GetViewType();

			if (nViewType == xtpCalendarTimeLineView)
			{
				CXTPCalendarTimeLineView* pTimeLineView = (CXTPCalendarTimeLineView*) pActiveView;
				int iTimeScaleID = pTimeLineView->GetTimeScaleID();
				dtSelStart = pTimeLineView->GetStartViewDate();
				if (iTimeScaleID == xtpTSPID_Month)
				{
					int nSelDay = dtSelStart.GetDay();
					CXTPCalendarUtils::ShiftDate_Month(dtSelStart, bNext ? 1 : -1, 1);
					CXTPCalendarUtils::UpdateMonthDay(dtSelStart, nSelDay);
				}
				else if (iTimeScaleID == xtpTSPID_Week)
				{
					dtSelStart += COleDateTimeSpan(bNext ? 7 : -7);
				}
				else if (iTimeScaleID == xtpTSPID_Day)
				{
					dtSelStart += COleDateTimeSpan(bNext ? 1 : -1);
				}
				pTimeLineView->SetStartViewDate(dtSelStart);
				SCROLLINFO SI;
				int nScrollStep;
				if (pTimeLineView->GetScrollBarInfoH(&SI, &nScrollStep))
				{
					int newPos = SI.nPos - SI.nPage;
					if (bNext)
						newPos = SI.nPos + SI.nPage;
					pTimeLineView->ScrollH(newPos, newPos);
				}
			}
			else
			{
				int nGroupIndex = 0;
				BOOL bAllDay = TRUE;
				dtFirst = pActiveView->GetViewDayDate(0);
				if (!pActiveView->GetSelection(&dtSelStart, &dtSelEnd, &bAllDay, &nGroupIndex))
				{
					dtSelStart = dtFirst;
					dtSelEnd = dtFirst + COleDateTimeSpan(1);
				}

				if (nViewType == xtpCalendarDayView)
				{
					int nDays = pActiveView->GetViewDayCount();
					dtSelStart += COleDateTimeSpan(bNext ? nDays : -nDays);
					dtSelEnd += COleDateTimeSpan(bNext ? nDays : -nDays);
				}
				else if (nViewType == xtpCalendarWorkWeekView ||
						nViewType == xtpCalendarFullWeekView ||
						nViewType == xtpCalendarWeekView)
				{
					dtSelStart += COleDateTimeSpan(bNext ? 7 : -7);
					dtSelEnd += COleDateTimeSpan(bNext ? 7 : -7);
				}
				else if (nViewType == xtpCalendarMonthView)
				{
					int nSelDay = dtSelStart.GetDay();
					CXTPCalendarUtils::ShiftDate_Month(dtSelStart, bNext ? 1 : -1, 1);
					CXTPCalendarUtils::UpdateMonthDay(dtSelStart, nSelDay);

					dtSelEnd = dtSelStart;// + COleDateTimeSpan(1);

					CXTPCalendarMonthView* pMView = (CXTPCalendarMonthView*)pActiveView;
					pMView->SetMDayViewLightColorBaseDate(dtSelStart);
				}
				else
				{
					ASSERT(FALSE);
				}

				pActiveView->UnselectAllEvents();
				pActiveView->SetSelection(dtSelStart, dtSelEnd, bAllDay, nGroupIndex);

				GetCalendarCtrl()->SwitchActiveView((XTPCalendarViewType)nViewType);
			}
		}
		else
		{
			bUpdate = FALSE;
		}

		if (bUpdate)
		{
			UpdateControlsState();
			Redraw();
		}
	}

	if (m_pConnection)
		m_pConnection->SendEvent(XTP_NC_CALENDAR_CAPTIONBARPART_CLICK, (WPARAM)nPartID, 0);

	if (GetCalendarCtrl())
		GetCalendarCtrl()->SendNotification(XTP_NC_CALENDAR_CAPTIONBARPART_CLICK, (WPARAM)nPartID, 0);
}

BOOL CXTPCalendarCaptionBarTheme::_UpdateControlsHotState(BOOL* pbMouseLeave)
{
	if (pbMouseLeave)
		*pbMouseLeave = FALSE;

	CPoint ptMouse(0, 0);

	if (GetCursorPos(&ptMouse) && GetOwnerWnd())
		GetOwnerWnd()->ScreenToClient(&ptMouse);
	else
		return FALSE;

	BOOL bMouseLeave = !m_rcRect.PtInRect(ptMouse);

	if (pbMouseLeave)
		*pbMouseLeave = bMouseLeave;

	if (!bMouseLeave)
		return FALSE;

	BOOL bResetFocused = CWnd::GetFocus() != GetOwnerWnd();
	BOOL bChanged = FALSE;

	int nCount = m_arMembers.GetCount();
	for (int i = 0; i < nCount; i++)
	{
		CXTPCalendarCaptionBarThemePart* pPart = m_arMembers.GetAt(i);
		ASSERT(pPart);
		if (pPart)
		{
			int nNewState = pPart->GetState() & (~(CXTPCalendarCaptionBarThemePart::xtpBtnStateMouseOver |
					CXTPCalendarCaptionBarThemePart::xtpBtnStatePressed));
			if (bResetFocused)
				nNewState = nNewState & (~CXTPCalendarCaptionBarThemePart::xtpBtnStateFocused);

			if (nNewState != pPart->GetState())
			{
				pPart->SetState(nNewState);
				bChanged = TRUE;
			}
		}
	}

	return bChanged;
}

void CXTPCalendarCaptionBarTheme::OnKillFocus (CWnd* pNewWnd)
{
	UNREFERENCED_PARAMETER(pNewWnd);

	BOOL bChanged = _UpdateControlsHotState();

	if (bChanged)
		Redraw();
}

void CXTPCalendarCaptionBarTheme::CSwitchViewButtonPart::RefreshMetrics()
{
	if (GetPaintTheme() <= xtpCalendarThemeOffice2003)
		m_clrTextColor.SetStandardValue(RGB(0, 0, 0));
}

void CXTPCalendarCaptionBarTheme::CSwitchViewButtonPart::AdjustLayout(CDC* pDC, const CRect& rcRect)
{
	m_rcRect = rcRect;

	if (m_rcRect.IsRectEmpty())
		return;

	if (pDC)
	{
		CXTPFontDC autoFont(pDC, GetTheme()->m_fntTextFont);

		CSize szText = pDC->GetTextExtent(GetText());

		if (szText.cx + 8 > m_rcRect.Width())
			m_rcRect.right = m_rcRect.left + szText.cx + 8;
	}
}

void CXTPCalendarCaptionBarTheme::CSwitchViewButtonPart::Draw(CDC* pDC)
{
	if (!IsVisible())
		return;

	CXTPResourceImages* pImages = XTPResourceImages();

	CXTPResourceImage* pImage = pImages->LoadFile(_T("CALENDARCAPTIONBARSWITCHVIEWBUTTON"));
	CRect rcRect = GetRect();

	ASSERT(pImage);
	if (!pImage || !GetTheme())
	{
		pDC->FillSolidRect(rcRect , RGB(155, 0, 0));
		return;
	}


	if (GetPaintTheme() >= xtpCalendarThemeResource)
	{
		int nImagePart = (m_nState & xtpBtnStateChecked) ? 3 : 0;

		if (m_nState & xtpBtnStatePressed)
		{
			nImagePart += 2;
		}
		else if (m_nState & xtpBtnStateMouseOver)
		{
			nImagePart += 1;
		}

		pImage->DrawImage(pDC, rcRect, pImage->GetSource(nImagePart, 6), CRect(0, 0, 0, 0));

		rcRect.left = rcRect.right - 1;
		pDC->FillSolidRect(&rcRect, GetTheme()->m_clrBorders);
	}
	else
	{
		if (GetPaintTheme() == xtpCalendarThemeOffice2013)
		{
			if ((m_nState & xtpBtnStateMouseOver) && (m_nState & xtpBtnStateChecked))
			{
				// create and select a solid lighter blue brush
				CBrush brushBlue(RGB(205, 230, 247));
				CBrush* pOldBrush = pDC->SelectObject(&brushBlue);

				// create a blue border pen
				CPen outerPen;
				outerPen.CreatePen(PS_SOLID, 1, RGB(146, 194, 224));
				CPen* pOldPen = pDC->SelectObject(&outerPen);

				pDC->Rectangle(rcRect);

				pDC->SelectObject(pOldBrush);
				pDC->SelectObject(pOldPen);
			}
			else if (m_nState & xtpBtnStateMouseOver)
			{
				pDC->FillSolidRect(rcRect, RGB(205, 230, 247));
			}
			else if ((m_nState & xtpBtnStateChecked))
			{
				pDC->FillSolidRect(rcRect, RGB(177, 214, 240));
			}
		}
		else if (GetTheme()->m_themeButton->IsAppThemed() &&
			GetPaintTheme() == xtpCalendarThemeOffice2003)
		{
			int nState = (m_nState & xtpBtnStatePressed) ? PBS_PRESSED : (m_nState & xtpBtnStateMouseOver) ? PBS_HOT: PBS_NORMAL;

			if (m_nState & xtpBtnStateChecked)
				nState = PBS_PRESSED;

			GetTheme()->m_themeButton->DrawThemeBackground(pDC->GetSafeHdc(), BP_PUSHBUTTON, nState, rcRect, NULL);

		}
		else
		{
			UINT uFlags = DFCS_BUTTONPUSH | ((m_nState & xtpBtnStateChecked) ? DFCS_CHECKED : 0) | ((m_nState & xtpBtnStatePressed) ? DFCS_PUSHED : 0);

			::DrawFrameControl(pDC->GetSafeHdc(), rcRect, DFC_BUTTON, uFlags);
		}
	}


	// drawText
	CFont fntTmp;
	CXTPFontDC autoFont(pDC, GetTheme()->m_fntTextFont, GetTextColor());

	if ((m_nState & xtpBtnStateChecked) && GetPaintTheme() <= xtpCalendarThemeOffice2003)
	{
		LOGFONT lf;
		if (GetTheme()->m_fntTextFont->GetLogFont(&lf))
		{
			lf.lfWeight = FW_BOLD;

			if (fntTmp.CreateFontIndirect(&lf))
				autoFont.SetFont(&fntTmp);
		}
	}

	CRect rcText = GetRect();
	rcText.DeflateRect(1, 1, 1, 1);

	pDC->SetBkMode(TRANSPARENT);
	pDC->DrawText(GetText(), rcText, DT_VCENTER | DT_CENTER | DT_SINGLELINE);
}

LPCTSTR CXTPCalendarCaptionBarTheme::CScrollDateButtonPart::_GetImageName()
{
	if (m_nPartID == xtpCalendarCaptionBar_ScrollNextDate)
		return _T("CALENDARCAPTIONBARNEXTDATEBUTTON");

	return _T("CALENDARCAPTIONBARPREVDATEBUTTON");
}

void CXTPCalendarCaptionBarTheme::CScrollDateButtonPart::AdjustLayout(CDC* /*pDC*/, const CRect& rcRect)
{
	m_rcRect = rcRect;

	if (m_rcRect.IsRectEmpty())
		return;

	CXTPResourceImages* pImages = XTPResourceImages();
	CXTPResourceImage* pImage = pImages->LoadFile(_GetImageName());

	ASSERT(pImage);
	if (!pImage || !GetTheme())
		return;

	CRect rcImage = pImage->GetSource(0, 4);

	m_rcRect.right = m_rcRect.left + rcImage.Width();

	int C = (m_rcRect.top + m_rcRect.bottom) / 2;
	int H = rcImage.Height() / 2;
	m_rcRect.top = C - H;
	m_rcRect.bottom = C + H;

	//m_rcRect.bottom = m_rcRect.top + rcImage.Height();
}

void CXTPCalendarCaptionBarTheme::CScrollDateButtonPart::Draw(CDC* pDC)
{
	if (!IsVisible())
		return;

	CRect rcRect = GetRect();

	CXTPResourceImages* pImages = XTPResourceImages();

	CXTPResourceImage* pImage = pImages->LoadFile(_GetImageName());

	ASSERT(pImage);
	if (!pImage || !GetTheme())
	{
		pDC->FillSolidRect(rcRect, RGB(155, 0, 0));
		return;
	}

	if (GetPaintTheme() == xtpCalendarThemeResource)
	{
		int nImagePart = (m_nState & xtpBtnStatePressed) ? 2 : (m_nState & xtpBtnStateMouseOver) ? 1 : 0;

		if (pImage->IsAlphaImage())
			pImage->DrawImage(pDC, rcRect, pImage->GetSource(nImagePart, 4));
		else
			pImage->DrawImage(pDC, rcRect, pImage->GetSource(nImagePart, 4), CRect(0, 0, 0, 0), RGB(255, 0, 255));

		if (m_nState & xtpBtnStateFocused)
		{
			if (pImage->IsAlphaImage())
				pImage->DrawImage(pDC, GetRect(), pImage->GetSource(3, 4), CRect(1, 1, 1, 1));
			else
				pImage->DrawImage(pDC, GetRect(), pImage->GetSource(3, 4), CRect(1, 1, 1, 1), RGB(255, 0, 255));
		}
	}
	else
	{
		COLORREF clrScrollTriangleColor = RGB(0, 0, 0);

		if (GetPaintTheme() == xtpCalendarThemeOffice2013)
		{
			// TODO: actually Office2013 theme uses smart triangle with vertical gradient,
			// but this will require GDI+ invocation or drawing predefined image

			clrScrollTriangleColor = RGB(110, 112, 113);

			if (m_nState & xtpBtnStateMouseOver)
			{
				pDC->FillSolidRect(rcRect, RGB(230, 242, 250));
			}

		} else if (GetTheme()->m_themeButton->IsAppThemed() &&
			GetPaintTheme() == xtpCalendarThemeOffice2003)
		{
			int nState = (m_nState & xtpBtnStatePressed) ? PBS_PRESSED :
					(m_nState & xtpBtnStateMouseOver) ? PBS_HOT: PBS_NORMAL;

			GetTheme()->m_themeButton->DrawThemeBackground(pDC->GetSafeHdc(), BP_PUSHBUTTON, nState, rcRect, NULL);
		}
		else
		{
			UINT uFlags = DFCS_BUTTONPUSH |
			((m_nState & xtpBtnStateChecked) ? DFCS_CHECKED : 0) |
			((m_nState & xtpBtnStatePressed) ? DFCS_PUSHED : 0); // | (bEnabled ? 0 : DFCS_INACTIVE)

			::DrawFrameControl(pDC->GetSafeHdc(), rcRect, DFC_BUTTON, uFlags);
		}

		BOOL bLeftDirection = GetPartID() == xtpCalendarCaptionBar_ScrollPrevDate;
		DrawScrollTriangle(pDC, rcRect, bLeftDirection, clrScrollTriangleColor);
	}
}

void CXTPCalendarCaptionBarTheme::_DrawTriangle(CDC* pDC, CPoint pt0, CPoint pt1, CPoint pt2, COLORREF clrColor)
{
	CXTPPenDC pen (*pDC, clrColor);
	CXTPBrushDC brush (*pDC, clrColor);

	CPoint pts[3];
	pts[0] = pt0;
	pts[1] = pt1;
	pts[2] = pt2;
	pDC->Polygon(pts, 3);
}

void CXTPCalendarCaptionBarTheme::CScrollDateButtonPart::DrawScrollTriangle(CDC* pDC, CRect rcRect, BOOL bLeftDirection, COLORREF clrColor)
{
	CPoint ptCenter(rcRect.CenterPoint());
	ptCenter.y--;

	if (bLeftDirection)
	{
		ptCenter.x -= 3;
		GetTheme()->_DrawTriangle(pDC, ptCenter, CPoint(ptCenter.x + 4, ptCenter.y - 4),
				CPoint(ptCenter.x + 4, ptCenter.y + 4), clrColor);
	}
	else
	{
		ptCenter.x += 2;
		GetTheme()->_DrawTriangle(pDC, ptCenter, CPoint(ptCenter.x - 4, ptCenter.y - 4),
			CPoint(ptCenter.x - 4, ptCenter.y + 4), clrColor);
	}
}


BOOL CXTPCalendarCaptionBarTheme::CScrollDateButtonPart::OnLButtonDown(UINT nFlags, CPoint point)
{
	CXTPCalendarCaptionBarThemePart::OnLButtonDown(nFlags, point);

	if (GetTheme() && GetTheme()->GetOwnerWnd() && HitTest(point) &&
		CWnd::GetFocus() != GetTheme()->GetOwnerWnd() )
	{
		GetTheme()->GetOwnerWnd()->SetFocus();
	}

	return FALSE;
}

void CXTPCalendarCaptionBarTheme::CScrollDateButtonPart::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	UNREFERENCED_PARAMETER(nRepCnt);  UNREFERENCED_PARAMETER(nFlags);

	if (!IsVisible())
		return;

	if ((m_nState & xtpBtnStateFocused) && (m_nState & xtpBtnStatePressed) == 0 &&
		nChar == VK_SPACE)
	{
		m_nState |= xtpBtnStatePressed;

		Redraw();
	}
}

void CXTPCalendarCaptionBarTheme::CScrollDateButtonPart::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	UNREFERENCED_PARAMETER(nRepCnt);  UNREFERENCED_PARAMETER(nFlags);

	if (!IsVisible())
		return;

	int nMask = xtpBtnStateFocused | xtpBtnStatePressed;

	if ((m_nState & nMask) == nMask && nChar == VK_SPACE && GetTheme())
	{
		GetTheme()->OnPartClick(GetPartID());
	}

	if (nChar == VK_SPACE)
	{
		m_nState = m_nState & (~xtpBtnStatePressed);

		Redraw();
	}
}
//------------------------------------------------------------------
void CXTPCalendarCaptionBarTheme::CRadioButtonPart::RefreshMetrics()
{
	if (GetPaintTheme() <= xtpCalendarThemeOffice2003)
		m_clrTextColor.SetStandardValue(RGB(0, 0, 0));
}

void CXTPCalendarCaptionBarTheme::CRadioButtonPart::AdjustLayout(CDC* pDC, const CRect& rcRect)
{
	m_rcRect = rcRect;

	if (m_rcRect.IsRectEmpty())
		return;

	CSize szRadio(13, 13);
	CRect rcRadio = GetRect();

	if (GetTheme() && GetTheme()->m_themeButton->IsAppThemed())
		GetTheme()->m_themeButton->GetThemePartSize(NULL, BP_RADIOBUTTON, 1, &rcRadio, TS_DRAW, &szRadio);

	if (pDC)
	{
		CXTPFontDC autoFont(pDC, GetTheme()->m_fntTextFont);
		CSize szText = pDC->GetTextExtent(GetText());

		if (szRadio.cx + 9 + szText.cx + 8 > m_rcRect.Width())
			m_rcRect.right = m_rcRect.left + szRadio.cx + 9 + szText.cx + 8;

		szRadio.cy = max(szRadio.cy, szText.cy);
	}
}

void CXTPCalendarCaptionBarTheme::CRadioButtonPart::Draw(CDC* pDC)
{
	if (!IsVisible())
		return;

	CSize szRadio(13, 13);
	CRect rcRadio = GetRect();

	BOOL bThemed = GetTheme() && GetTheme()->m_themeButton->IsAppThemed() &&
					GetPaintTheme() >= xtpCalendarThemeOffice2003;
	if (bThemed)
		GetTheme()->m_themeButton->GetThemePartSize(NULL, BP_RADIOBUTTON, 1, &rcRadio, TS_DRAW, &szRadio);

	rcRadio.right = rcRadio.left + szRadio.cx;

	if (bThemed)
	{
		int md = rcRadio.CenterPoint().y;
		int ht = (int) (rcRadio.Height() / (2 * GetTheme()->m_dScale));
		rcRadio.top = md - ht;
		rcRadio.bottom = md + ht;

		GetTheme()->m_themeButton->GetThemePartSize(NULL, BP_RADIOBUTTON, 1, &rcRadio, TS_DRAW, &szRadio);

		int nState = (m_nState & xtpBtnStatePressed) ? RBS_UNCHECKEDPRESSED :
				(m_nState & xtpBtnStateMouseOver) ? RBS_UNCHECKEDHOT : RBS_UNCHECKEDNORMAL;

		if (m_nState & xtpBtnStateChecked)
			nState += 4;

		GetTheme()->m_themeButton->DrawThemeBackground(pDC->GetSafeHdc(), BP_RADIOBUTTON, nState, rcRadio, NULL);

	}
	else
	{
		UINT uFlags = DFCS_BUTTONRADIO |
			((m_nState & xtpBtnStateChecked) ? DFCS_CHECKED : 0) |
			((m_nState & xtpBtnStatePressed) ? DFCS_PUSHED : 0);

		if (GetPaintTheme() > xtpCalendarThemeOffice2000)
			uFlags |= DFCS_FLAT;

		::DrawFrameControl(pDC->GetSafeHdc(), rcRadio, DFC_BUTTON, uFlags);
	}

	// drawText
	CXTPFontDC autoFont(pDC, GetTheme()->m_fntTextFont, GetTextColor());
	pDC->SetBkMode(TRANSPARENT);

	CRect rcText = GetRect();
	rcText.left = rcRadio.right + 9;

	pDC->DrawText(GetText(), rcText, DT_VCENTER | DT_LEFT | DT_SINGLELINE);

	rcText.left--;
	rcText.right++;

	if (m_nState & xtpBtnStatePressed)
		pDC->DrawFocusRect(rcText);
}
//------------------------------------------------------------------
void CXTPCalendarCaptionBarTheme::CCheckboxButtonPart::RefreshMetrics()
{
	if (GetPaintTheme() <= xtpCalendarThemeOffice2003)
		m_clrTextColor.SetStandardValue(RGB(0, 0, 0));
}

void CXTPCalendarCaptionBarTheme::CCheckboxButtonPart::AdjustLayout(CDC* pDC, const CRect& rcRect)
{
	m_rcRect = rcRect;

	if (m_rcRect.IsRectEmpty())
		return;

	CSize szRadio(13, 13);
	CRect rcRadio = GetRect();

	if (GetTheme() && GetTheme()->m_themeButton->IsAppThemed())
		GetTheme()->m_themeButton->GetThemePartSize(NULL, BP_CHECKBOX, 1, &rcRadio, TS_DRAW, &szRadio);

	if (pDC)
	{
		CXTPFontDC autoFont(pDC, GetTheme()->m_fntTextFont);
		CSize szText = pDC->GetTextExtent(GetText());

		if (szRadio.cx + 9 + szText.cx + 8 > m_rcRect.Width())
			m_rcRect.right = m_rcRect.left + szRadio.cx + 9 + szText.cx + 8;

		szRadio.cy = max(szRadio.cy, szText.cy);
	}
}

void CXTPCalendarCaptionBarTheme::CCheckboxButtonPart::Draw(CDC* pDC)
{
	if (!IsVisible())
		return;

	CSize szRadio(13, 13);
	CRect rcRadio = GetRect();

	BOOL bThemed = GetTheme() && GetTheme()->m_themeButton->IsAppThemed() &&
					GetPaintTheme() >= xtpCalendarThemeOffice2003;
	if (bThemed)
		GetTheme()->m_themeButton->GetThemePartSize(NULL, BP_CHECKBOX, 1, &rcRadio, TS_DRAW, &szRadio);

	rcRadio.right = rcRadio.left + szRadio.cx;

	if (bThemed)
	{
		int md = rcRadio.CenterPoint().y;
		int ht = (int) (rcRadio.Height() / (2 * GetTheme()->m_dScale));
		rcRadio.top = md - ht;
		rcRadio.bottom = md + ht;

		GetTheme()->m_themeButton->GetThemePartSize(NULL, BP_CHECKBOX, 1, &rcRadio, TS_DRAW, &szRadio);

		int nState = (m_nState & xtpBtnStatePressed) ? RBS_UNCHECKEDPRESSED :
				(m_nState & xtpBtnStateMouseOver) ? RBS_UNCHECKEDHOT: RBS_UNCHECKEDNORMAL;

		if (m_nState & xtpBtnStateChecked)
			nState += 4;

		GetTheme()->m_themeButton->DrawThemeBackground(pDC->GetSafeHdc(), BP_CHECKBOX, nState, rcRadio, NULL);

	}
	else
	{
		UINT uFlags = DFCS_BUTTONCHECK |
			((m_nState & xtpBtnStateChecked) ? DFCS_CHECKED : 0) |
			((m_nState & xtpBtnStatePressed) ? DFCS_PUSHED : 0);

		if (GetPaintTheme() > xtpCalendarThemeOffice2000)
			uFlags |= DFCS_FLAT;

		::DrawFrameControl(pDC->GetSafeHdc(), rcRadio, DFC_BUTTON, uFlags);
	}

	// drawText
	CXTPFontDC autoFont(pDC, GetTheme()->m_fntTextFont, GetTextColor());
	pDC->SetBkMode(TRANSPARENT);

	CRect rcText = GetRect();
	rcText.left = rcRadio.right + 9;

	pDC->DrawText(GetText(), rcText, DT_VCENTER | DT_LEFT | DT_SINGLELINE);

	rcText.left--;
	rcText.right++;

	if (m_nState & xtpBtnStatePressed)
		pDC->DrawFocusRect(rcText);
}
//------------------------------------------------------------------
void CXTPCalendarCaptionBarTheme::CDateLabelPart::RefreshMetrics()
{
	if (!GetTheme())
		return;

	m_clrTextColor.SetStandardValue(GetTheme()->m_clrTextColor);

	LOGFONT lfBaseFont;
	::ZeroMemory(&lfBaseFont, sizeof(lfBaseFont));

	GetTheme()->m_fntTextFont->GetLogFont(&lfBaseFont);

	if (GetTheme()->IsOneLine())
	{
		if (XTPSystemVersion()->IsWinVistaOrGreater())
			lfBaseFont.lfHeight = -14;
		else
			lfBaseFont.lfHeight = -18;
	}
	else
		lfBaseFont.lfHeight = -20;

	m_fntTextFont.SetStandardValue(&lfBaseFont);
}

void CXTPCalendarCaptionBarTheme::CDateLabelPart::Draw(CDC* pDC)
{
	if (!IsVisible())
		return;

	CXTPFontDC autoFont(pDC, m_fntTextFont, m_clrTextColor);
	pDC->SetBkMode(TRANSPARENT);

	CXTPCalendarView* pCalView = XTP_SAFE_GET2(GetTheme(), GetCalendarCtrl(), GetActiveView(), NULL);

	if (pCalView)
	{
		BOOL bShowMonthDay = pCalView->GetViewType() != xtpCalendarMonthView;
		COleDateTime dtDay1 = pCalView->GetViewDayDate(0);
		COleDateTime dtDay2 = pCalView->GetViewDayDate(max(0, pCalView->GetViewDayCount() - 1));

		m_strText = _FormatDate(dtDay1, dtDay2, bShowMonthDay);
	}

	if (m_strText.IsEmpty())
		return;

	CRect rcText = GetRect();
	rcText.top -= 6;

	UINT uFlags = DT_VCENTER | DT_SINGLELINE;
	if (GetTheme() && !GetTheme()->IsScrollDateButtons()
		&& GetPaintTheme() < xtpCalendarThemeResource)
	{
		uFlags |= DT_CENTER;
		rcText.right = max(rcText.left, rcText.right - 9);
	}

	pDC->DrawText(GetText(), rcText, uFlags);
}

CString CXTPCalendarCaptionBarTheme::CDateLabelPart::_FormatDate(COleDateTime dtDay1, COleDateTime dtDay2, BOOL bShowMonthDay)
{
	//LOCALE_IDATE  -  short date format ordering
	// 0 Month-Day-Year
	// 1 Day-Month-Year
	// 2 Year-Month-Day
	int nDateOrdering = CXTPCalendarUtils::GetLocaleLong(LOCALE_ILDATE);

	CString strFormat1 = CXTPCalendarUtils::GetLocaleString(LOCALE_SLONGDATE, 256);
	CString strFormat2 = CXTPCalendarUtils::GetLocaleString(LOCALE_SLONGDATE, 256);

	strFormat1 = CXTPCalendarUtils::RemoveFormatPart(strFormat1, _T('g'));
	strFormat1 = CXTPCalendarUtils::RemoveFormatPart(strFormat1, _T("ddd"));
	strFormat1 = CXTPCalendarUtils::RemoveFormatPart(strFormat1, _T("dddd"));

	strFormat2 = CXTPCalendarUtils::RemoveFormatPart(strFormat2, _T('g'));
	strFormat2 = CXTPCalendarUtils::RemoveFormatPart(strFormat2, _T("ddd"));
	strFormat2 = CXTPCalendarUtils::RemoveFormatPart(strFormat2, _T("dddd"));

	if (!bShowMonthDay)
	{
		strFormat1 = CXTPCalendarUtils::RemoveFormatPart(strFormat1, _T('d'));
		strFormat2 = CXTPCalendarUtils::RemoveFormatPart(strFormat2, _T('d'));
	}
	CString strCaption;

	if (!CXTPCalendarUtils::IsEqual(dtDay1, dtDay2))
	{
		//CString strFormat2(strFormat1);

		if (dtDay1.GetMonth() == dtDay2.GetMonth())
		{
			if (nDateOrdering == 1)
				strFormat1 = CXTPCalendarUtils::RemoveFormatPart(strFormat1,  _T('M'));
			else
				strFormat2 = CXTPCalendarUtils::RemoveFormatPart(strFormat2,  _T('M'));
		}

		if (dtDay1.GetYear() == dtDay2.GetYear())
		{
			if (nDateOrdering == 2)
				strFormat2 = CXTPCalendarUtils::RemoveFormatPart(strFormat2,  _T('y'));
			else
				strFormat1 = CXTPCalendarUtils::RemoveFormatPart(strFormat1,  _T('y'));
		}

		if (!strFormat1.IsEmpty())
			strCaption = CXTPCalendarUtils::GetDateFormat(dtDay1, strFormat1);

		if (!strFormat1.IsEmpty() && !strFormat2.IsEmpty())
			strCaption += _T(" - ");

		if (!strFormat2.IsEmpty())
			strCaption += CXTPCalendarUtils::GetDateFormat(dtDay2, strFormat2);
	}
	else
	{
		strCaption = CXTPCalendarUtils::GetDateFormat(dtDay1, strFormat1);
	}

	UserAction_FormatCaptionBarLabel(strCaption, dtDay1);
	return strCaption;
}

void CXTPCalendarCaptionBarTheme::CDateLabelPart::UserAction_FormatCaptionBarLabel(CString& strCaption, COleDateTime dtDay1)
{
	DWORD dwFlags = XTP_SAFE_GET2(GetTheme(), GetCalendarCtrl(), GetAskItemTextFlags(), 0);
	CXTPCalendarView* pActiveView = XTP_SAFE_GET2(GetTheme(), GetCalendarCtrl(), GetActiveView(), NULL);
	if(pActiveView == NULL)
		return;

	XTPCalendarViewType activeViewType = pActiveView->GetViewType();

	XTP_CALENDAR_GETITEMTEXT_PARAMS objRequest;
	::ZeroMemory(&objRequest, sizeof(objRequest));
	objRequest.pstrText = &strCaption;
	objRequest.nWeekNum = CXTPCalendarUtils::GetWeekOfYear(dtDay1);

	if ((dwFlags & xtpCalendarItemText_CaptionBarDayViewDateLabel) && activeViewType == xtpCalendarDayView)
		objRequest.nItem = (int)xtpCalendarItemText_CaptionBarDayViewDateLabel;

	if ((dwFlags & xtpCalendarItemText_CaptionBarWeekViewDateLabel)
		 && (activeViewType == xtpCalendarWeekView || activeViewType == xtpCalendarFullWeekView || activeViewType == xtpCalendarWorkWeekView))
		objRequest.nItem = (int)xtpCalendarItemText_CaptionBarWeekViewDateLabel;

	if ((dwFlags & xtpCalendarItemText_CaptionBarMonthViewDateLabel) && activeViewType == xtpCalendarMonthView)
		objRequest.nItem = (int)xtpCalendarItemText_CaptionBarMonthViewDateLabel;

	if(objRequest.nItem != 0)
		XTP_SAFE_CALL2(GetTheme(), GetCalendarCtrl(), SendNotificationAlways(XTP_NC_CALENDAR_GETITEMTEXT, (WPARAM)&objRequest, 0));
}

void CXTPCalendarCaptionBarTheme::CExpandButtonPart::AdjustLayout(CDC* /*pDC*/, const CRect& rcRect)
{
	m_rcRect = rcRect;

	if (m_rcRect.IsRectEmpty())
		return;

	CXTPResourceImages* pImages = XTPResourceImages();

	CXTPResourceImage* pImage = pImages->LoadFile(_T("CALENDARCAPTIONBAREXPANDBUTTON"));

	ASSERT(pImage);
	if (!pImage || !GetTheme())
		return;

	CRect rcImage = pImage->GetSource(0, 6);

	m_rcRect.left = m_rcRect.right - rcImage.Width();
	m_rcRect.top = m_rcRect.top + m_rcRect.Height() / 2 - rcImage.Height() / 2;
	m_rcRect.bottom = m_rcRect.top + rcImage.Height();
}

void CXTPCalendarCaptionBarTheme::CExpandButtonPart::Draw(CDC* pDC)
{
	if (!IsVisible())
		return;

	CRect rcRect = GetRect();

	CXTPResourceImages* pImages = XTPResourceImages();

	CXTPResourceImage* pImage = pImages->LoadFile(_T("CALENDARCAPTIONBAREXPANDBUTTON"));

	ASSERT(pImage);
	if (!pImage || !GetTheme())
	{
		pDC->FillSolidRect(rcRect, RGB(155, 0, 0));
		return;
	}

	if (GetPaintTheme() == xtpCalendarThemeResource)
	{
		int nImagePart = (m_nState & xtpBtnStateChecked) ? 3 : 0;
		if (m_nState & xtpBtnStatePressed)
			nImagePart += 2;
		else if (m_nState & xtpBtnStateMouseOver)
			nImagePart += 1;

		pImage->DrawImage(pDC, rcRect, pImage->GetSource(nImagePart, 6), CRect(0, 0, 0, 0), RGB(255, 0, 255));
		//pImage->DrawImage(pDC, rcRect, pImage->GetSource(nImagePart, 6));
	}
	else
	{
		if (GetTheme()->m_themeButton->IsAppThemed() &&
			GetPaintTheme() == xtpCalendarThemeOffice2003)
		{
			int nState = (m_nState & xtpBtnStatePressed) ? PBS_PRESSED :
				(m_nState & xtpBtnStateMouseOver) ? PBS_HOT: PBS_NORMAL;

			GetTheme()->m_themeButton->DrawThemeBackground(pDC->GetSafeHdc(), BP_PUSHBUTTON, nState, rcRect, NULL);
		}
		else
		{
			UINT uFlags = DFCS_BUTTONPUSH |
			((m_nState & xtpBtnStateChecked) ? DFCS_CHECKED : 0) |
			((m_nState & xtpBtnStatePressed) ? DFCS_PUSHED : 0);

			::DrawFrameControl(pDC->GetSafeHdc(), rcRect, DFC_BUTTON, uFlags);
		}

		BOOL bDownDirection = (m_nState & xtpBtnStateChecked) == 0;
		DrawExpandTriangle(pDC, rcRect, bDownDirection, RGB(0, 0, 0));
	}
}

void CXTPCalendarCaptionBarTheme::CExpandButtonPart::DrawExpandTriangle(CDC* pDC, CRect rcRect, BOOL bDownDirection, COLORREF clrColor)
{
	CPoint ptCenter(rcRect.CenterPoint());
	if (bDownDirection)
	{
		ptCenter.y += 2;
		GetTheme()->_DrawTriangle(pDC, ptCenter, CPoint(ptCenter.x + 4, ptCenter.y - 4),
			CPoint(ptCenter.x - 4, ptCenter.y - 4), clrColor);
	}
	else
	{
		ptCenter.y -= 2;
		GetTheme()->_DrawTriangle(pDC, ptCenter, CPoint(ptCenter.x + 4, ptCenter.y + 4),
			CPoint(ptCenter.x - 4, ptCenter.y + 4), clrColor);
	}
}
