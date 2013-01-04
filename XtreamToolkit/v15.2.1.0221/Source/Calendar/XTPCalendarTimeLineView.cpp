// XTPCalendarTimeLineView.cpp: implementation of the CXTPCalendarTimeLineView class.
//
// This file is a part of the XTREME CALENDAR MFC class library.
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
#include "Common/XTPSmartPtrInternalT.h"

#include "XTPCalendarUtils.h"
#include "XTPCalendarDefines.h"
#include "XTPCalendarPtrCollectionT.h"

#include "XTPCalendarEvent.h"
#include "XTPCalendarEvents.h"

#include "XTPCalendarView.h"
#include "XTPCalendarViewEvent.h"
#include "XTPCalendarViewDay.h"

#include "XTPCalendarTimeLineViewGroup.h"

#include "XTPCalendarTimeLineView.h"
#include "XTPCalendarTimeLineViewTimeScale.h"
#include "XTPCalendarControl.h"
#include "XTPCalendarData.h"

#include "XTPDatePickerControl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CXTPCalendarTimeLineView, CXTPCalendarView)

CXTPCalendarTimeLineView::CXTPCalendarTimeLineView(CXTPCalendarControl* pCalendarControl) :
		CXTPCalendarView(pCalendarControl)
{
	m_pTimeScale = new CXTPCalendarTimeLineViewTimeScale(this);

	m_nHScrollPos = -1;
	m_nVScrollPos = 0;
	//m_uRecalcHScrollTimerID = 0;

	m_rcLastPopulate.SetRect(0, 0, 0, 0);

	m_clrEventBar = ::GetSysColor(COLOR_ACTIVECAPTION);
	m_clrEventBarLine = RGB(255, 0, 0);

	m_clrSelectedText = ::GetSysColor(COLOR_HIGHLIGHTTEXT);
	m_clrSelectedBackground = RGB(192,192,192);
	//m_clrSelectedBackground = ::GetSysColor(COLOR_HIGHLIGHT);

	m_clrBackground = ::GetSysColor(COLOR_WINDOW);
	m_clrText = ::GetSysColor(COLOR_WINDOWTEXT);

	m_clrTimeScaleBackground = RGB(224,224,224);
	//m_clrTimeScaleBackground = ::GetSysColor(COLOR_WINDOW);
	m_clrTimeScaleBorder = RGB(0,0,255);
	m_clrTimeScaleHighlight = RGB(255,0,0);
	m_clrTimeScaleText = ::GetSysColor(COLOR_WINDOWTEXT);

	m_bTimeLineSeparateGroup = FALSE;

	m_nTimelineMaxPixelsForEvent = 50;
	m_nTimelineMaxPixelsForEvent_ScaleDay = 200;
	m_nTimelineMaxPixelsForEvent_ScaleWeek = 100;

	m_pGroups = new CXTPCalendarTimeLineViewGroups();

}

CXTPCalendarTimeLineView::~CXTPCalendarTimeLineView()
{
	OnBeforeDestroy();
	CMDTARGET_RELEASE(m_pTimeScale);

	CMDTARGET_RELEASE(m_pGroups);
}

void CXTPCalendarTimeLineView::OnBeforeDestroy()
{
	TBase::OnBeforeDestroy();
}

void CXTPCalendarTimeLineView::AdjustLayout(CDC* pDC, const CRect& rcView, BOOL bCallPostAdjustLayout)
{
	TBase::AdjustLayout(pDC, rcView, bCallPostAdjustLayout);

	if (!m_pTimeScale)
	{
		ASSERT(FALSE);
		return;
	}

	m_pTimeScale->AdjustLayout(rcView);

	if (m_rcLastPopulate != rcView)
		Populate();

	CRect rcGroup = rcView;
	rcGroup.top = GetTimeScale()->GetRect().bottom + 2;

	int nGroups = m_pGroups->GetCount();
	for (int i = 0; i < nGroups; i++)
	{
		m_pGroups->GetAt(i)->AdjustLayout(pDC, rcGroup);

		rcGroup.top = m_pGroups->GetAt(i)->GetRect().bottom;
	}
//  TRACE(GetStartViewDate().Format(_T("AdjustLayout %d %m %Y\n")));
}

void CXTPCalendarTimeLineView::Populate()
{
	if (IsEditingSubject())
	{
		EndEditSubject(xtpCalendarEditSubjectCommit);
	}

	COleDateTime m_dtPopulatedStartDay;
	COleDateTime m_dtPopulatedEndDay;

	// This 45 days left and right extra range will prevent event rect from jumps during scroll
	COleDateTimeSpan dtOffset(45.0);

	m_dtPopulatedStartDay = CXTPCalendarUtils::ResetTime(GetStartViewDate()) - dtOffset;
	COleDateTimeSpan spTimeperPix = m_pTimeScale->GetTimeScaleParams()->GetTimePerPixel();
	COleDateTimeSpan spVisibleTime = (double)GetViewRect().Width() * (double)spTimeperPix;

	m_dtPopulatedEndDay = GetStartViewDate() + spVisibleTime;
	m_dtPopulatedEndDay = CXTPCalendarUtils::ResetTime(m_dtPopulatedEndDay) + dtOffset;

	CXTPCalendarEventsPtr ptrEvents = _GetEvents(m_dtPopulatedStartDay, m_dtPopulatedEndDay);
	if (ptrEvents)
		ptrEvents->Sort(CXTPCalendarEvents::CompareEvents_ForViewByStart);

	m_pGroups->RemoveAll();

	m_pGroups->Add(new CXTPCalendarTimeLineViewGroup(this));

	m_pGroups->GetAt(0)->Populate(ptrEvents);

	_ReSelectSelectEvents();

	m_rcLastPopulate = GetViewRect();
}

void CXTPCalendarTimeLineView::Draw(CDC* pDC)
{
	CRect rcClient = GetViewRect();

	pDC->FillSolidRect(&rcClient, m_clrBackground);

	if (!m_pTimeScale)
	{
		ASSERT(FALSE);
		return;
	}
	m_pTimeScale->Draw(pDC);

	int nOffset = m_nVScrollPos;
	int nGroups = m_pGroups->GetCount();
	for (int i = 0; i < nGroups; i++)
	{
		CXTPCalendarTimeLineViewGroup* pGroup = m_pGroups->GetAt(i);
		if (pGroup)
		{
			pGroup->Draw(pDC, nOffset);

			CRect rcLine = rcClient;
			rcLine.top = pGroup->GetGroupHeight();
			rcLine.bottom = rcLine.top + 2;
			if (m_bTimeLineSeparateGroup)
				pDC->FillSolidRect(&rcLine, m_clrEventBar);
		}
	}
}

void CXTPCalendarTimeLineView::Draw2(CDC* pDC)
{
	Draw(pDC);
}

BOOL CXTPCalendarTimeLineView::GetScrollBarInfoV(SCROLLINFO* pSI) const
{
	int nScroll = GetSystemMetrics(SM_CYHSCROLL);
	int h(0);
	if (m_pTimeScale)
		h = m_pTimeScale->GetClientRect().Height();
	int H = GetViewRect().Height();
	pSI->nMax = H - nScroll - h;

	int nGroups = m_pGroups->GetCount();
	int iBottom(0);
	for (int i = 0; i < nGroups; i++)
		iBottom = max(iBottom, m_pGroups->GetAt(i)->GetRect().bottom);

	if (iBottom > H)
	{
		pSI->nPage = H;
		pSI->nMax = iBottom;
		pSI->nPos = m_nVScrollPos;
		return TRUE;
	}
	return FALSE;
}

BOOL CXTPCalendarTimeLineView::GetScrollBarInfoH(SCROLLINFO* pSI, int* pnScrollStep) const
{
	int nScroll = GetSystemMetrics(SM_CXVSCROLL);
	pSI->nMin = 0;
	pSI->nMax = GetViewRect().Width() - nScroll;

	int iTimeScaleID = GetTimeScaleID();

	if (iTimeScaleID == xtpTSPID_Month)
		pSI->nPage = int(pSI->nMax * 0.05); //approx 8 days
	else if (iTimeScaleID == xtpTSPID_Week)
		pSI->nPage = int(pSI->nMax * 0.05);
	else if (iTimeScaleID == xtpTSPID_Day)
		pSI->nPage = int(pSI->nMax * 0.05);

	pSI->nPos = m_nHScrollPos;

	if (pnScrollStep)
		*pnScrollStep = 4;

	return TRUE;
}

void CXTPCalendarTimeLineView::ScrollH(int nPos, int nPos_raw)
{
	COleDateTime dtStart = GetStartViewDate();

	COleDateTimeSpan spTimeperPix = m_pTimeScale->GetTimeScaleParams()->GetTimePerPixel();

	int nShift = (nPos - m_nHScrollPos) * 4; // si.nPos;

	COleDateTimeSpan spShift = nShift * (double)spTimeperPix;


#ifdef TRACE_EDITING_ON
	CString s_Start = dtStart.Format(_T("%d %m %Y %H %M"));
	CString s_TimeperPix = spTimeperPix.Format(_T("%H %M %S"));
	s_TimeperPix.Format(_T("D=%f, H=%f M=%f S=%f"),
		spTimeperPix.GetTotalDays(), spTimeperPix.GetTotalHours(),
		spTimeperPix.GetTotalMinutes(), spTimeperPix.GetTotalSeconds());

	CString s_Shift = spShift.Format(_T("%H %M %S"));
	s_Shift.Format(_T("D=%f, H=%f M=%f S=%f"),
		spShift.GetTotalDays(), spShift.GetTotalHours(),
		spShift.GetTotalMinutes(), spShift.GetTotalSeconds());
#endif

	dtStart += spShift;

	SetStartViewDate(dtStart);

	SCROLLINFO si;
	GetScrollBarInfoH(&si);

	m_nHScrollPos = nPos;

	GetCalendarControl()->AdjustScrollBar(SB_VERT);
	GetCalendarControl()->RefreshCaptionBar();
}

void CXTPCalendarTimeLineView::ScrollV(int nPos, int nPos_raw)
{
	m_nVScrollPos = nPos;

	UNREFERENCED_PARAMETER(nPos_raw);
}

void CXTPCalendarTimeLineView::OnScrollEnd(int nBar)
{
	if (nBar == SB_HORZ)
	{
		SCROLLINFO si;
		GetScrollBarInfoH(&si);

		int nBorder = (int) si.nPage;
		m_nHScrollPos = max(m_nHScrollPos, si.nMin + nBorder);
		m_nHScrollPos = min(m_nHScrollPos, si.nMax - nBorder - (int) si.nPage);

		GetCalendarControl()->AdjustScrollBar(SB_HORZ);
	}
}

CXTPCalendarEventsPtr CXTPCalendarTimeLineView::_GetEvents(COleDateTime dtStartDay, COleDateTime dtEndDay)
{
	// TODO: change for multiresources.
	CXTPCalendarData* pData = GetCalendarControl()->GetDataProvider();
	CXTPCalendarEventsPtr ptrEvents = new CXTPCalendarEvents();

	ASSERT(pData);
	if (!pData || !ptrEvents)
		return NULL;

	COleDateTimeSpan spDay(1, 0, 0, 0);

	COleDateTime dtDay = CXTPCalendarUtils::ResetTime(dtStartDay);
	for (; dtDay <= dtEndDay; dtDay += spDay)
	{
		CXTPCalendarEventsPtr ptrDayEvents = pData->RetrieveDayEvents(dtDay);

		int nCount = ptrDayEvents ? ptrDayEvents->GetCount() : 0;
		for (int i = 0; i < nCount; i++)
		{
			CXTPCalendarEvent* pEv = ptrDayEvents->GetAt(i);

			if (ptrEvents->FindEvent(pEv) == NULL)
			{
				ptrEvents->Add(pEv, TRUE);
			}
		}
	}

	return ptrEvents;
}

int CXTPCalendarTimeLineView::GetTimeScaleID() const
{
	return XTP_SAFE_GET2(m_pTimeScale, GetTimeScaleParams(), GetTimeScaleParamsID(), xtpTSPID_Unknown);
}

void CXTPCalendarTimeLineView::SetTimeScaleID(int eTimeScaleParamsID)
{
	if (m_pTimeScale)
	{
		CXTPCalendarControl::CUpdateContext updateContext(m_pControl, xtpCalendarUpdateAll);

		m_pTimeScale->SetTimeScaleParams(eTimeScaleParamsID);
	}
}

BOOL CXTPCalendarTimeLineView::HitTest(CPoint pt, XTP_CALENDAR_HITTESTINFO* pHitTest) const
{
	if (pHitTest)
	{
		/*if (IsVScrollEnabled())
		{
			SCROLLINFO si;
			si.cbSize = sizeof(SCROLLINFO);
			si.nMin = 0;
			si.fMask = SIF_PAGE | SIF_RANGE | SIF_POS;
			GetCalendarControl()->GetActiveView()->GetScrollBarInfoV(&si);
			if (si.nPos > 0)
				pt.y += si.nPos;
		}*/
		CPoint point(pt);
		if (m_pTimeScale->HitTestEx(pt, pHitTest))
			pHitTest->uHitCode = xtpCalendarHitTestTimeLineScale;

		GetCalendarControl()->ScreenToClient(&pt);
		COleDateTime dt = GetStartViewDate();
		if (m_pTimeScale)
		{
			COleDateTimeSpan spTimeperPix = m_pTimeScale->GetTimeScaleParams()->GetTimePerPixel();
			spTimeperPix = spTimeperPix * pt.x;
			if (spTimeperPix.m_status == COleDateTimeSpan::valid)
				dt += spTimeperPix;

		}
		pHitTest->dt = dt;
		pHitTest->nGroup = -1;
		pHitTest->nEvent = 0;
		pHitTest->pViewEvent = NULL;

		int nGroups = m_pGroups->GetCount();
		for (int i = 0; i < nGroups; i++)
		{
			CXTPCalendarTimeLineViewGroup* pGroup = m_pGroups->GetAt(i);
			if (pGroup)
			{
				int nCount = pGroup->GetViewEventsCount();
				for (i = 0; i < nCount; i++)
				{
					CXTPCalendarTimeLineViewEvent* pViewEv = pGroup->GetViewEvent(i);
					if (pViewEv)
					{
						//CRect rc = pViewEv->GetEventRect();
						CRect rc = pViewEv->GetLastVisibleRect();
						if (rc.PtInRect(point))
						{
							pHitTest->nGroup = i;
							pHitTest->nEvent = pViewEv->GetEvent()->GetEventID();
							pHitTest->pViewEvent = pViewEv;
							return TRUE;
						}
					}
				}
			}
		}

		return TRUE;
	}
	return FALSE;
}

CRect CXTPCalendarTimeLineView::GetTooltipRect(const CPoint& ptHit, const XTP_CALENDAR_HITTESTINFO& hitInfo) const
{
	CRect rcTip(0,0,0,0);
	CPoint pt(ptHit);
	if (hitInfo.pViewEvent)
	{
		rcTip = hitInfo.pViewEvent->m_rcEvent;
		int w = rcTip.Width();
		rcTip.left = pt.x;
		rcTip.right = rcTip.left + w;
	}
	return rcTip;
}

#include "XTPDatePickerPaintManager.h"

static BOOL m_bWeHavePopup = FALSE;

void CXTPCalendarTimeLineView::ShowCalendarPopup(COleDateTime dt, CPoint point)
{
	GetCalendarControl()->ClientToScreen(&point);
	CPoint pH = m_pTimeScale->m_rcScale.BottomRight();
	GetCalendarControl()->ClientToScreen(&pH);
//if we follows Outlook spec and only top (title) part of scale call popup

	if (point.y > pH.y - 31)
	{
		m_bWeHavePopup = FALSE;
		return;
	}
//if we follows Outlook spec and looking for extra click if Popup visible

	if (m_bWeHavePopup) //cycle by 2
	{
		m_bWeHavePopup = FALSE;
		return;
	}
	m_bWeHavePopup = TRUE;

	COleDateTime rdtDate(dt);
	CXTPDatePickerControl datePicker;
	CRect rc;
	datePicker.GetMinReqRect(&rc);
	int rWidth = rc.Width();
	int rHeight = rc.Height();

	CRect rcDesk;
//<<MULTI-MONITOR case fix>>
//  GetCalendarControl()->GetDesktopWindow()->GetClientRect(&rcDesk);
	GetCalendarControl()->GetDesktopWindow()->GetWindowRect(&rcDesk);
	GetCalendarControl()->ClientToScreen(&rcDesk);

	if (point.x + rWidth > rcDesk.right)
		point.x -= rWidth;

//if we follows Outlook spec and keep fixed vert pos!
	point.y = pH.y - 31;

	rc.left = point.x;
	rc.top = point.y;
	rc.right = rc.left + rWidth;
	rc.bottom = rc.top + rHeight;

	datePicker.SetButtonsVisible(TRUE, FALSE);
	datePicker.SetShowWeekNumbers(FALSE);
	datePicker.SetBorderStyle(xtpDatePickerBorderOffice);

	XTPCalendarTheme ct = GetCalendarControl()->GetPaintTheme();


	datePicker.SetPaintTheme(ct);
	datePicker.GetPaintManager()->EnableTheme(TRUE);

	datePicker.Select(rdtDate);
	datePicker.SetToday(GetStartViewDate());
	datePicker.EnsureVisibleSelection();

	if (datePicker.GoModal(rc, GetCalendarControl()))
	{
		if (datePicker.GetSelRange(dt, rdtDate))
		{
			if (dt == rdtDate)
			{
			}
			else
			{
			}
		}
		COleDateTime dtNow(rdtDate);
		SetStartViewDate(dtNow);

		OnScrollEnd(SB_HORZ);
	}
	else
	{
	}
	CPoint Point;
	::GetCursorPos(&Point);
	CRect rcClient;
	GetCalendarControl()->GetClientRect(&rcClient);
	GetCalendarControl()->ClientToScreen(&rcClient);
//if we follows Outlook spec and only top (title) part of scale call popup
	if (!rcClient.PtInRect(Point))
		m_bWeHavePopup = FALSE;

	GetCalendarControl()->ClientToScreen(&Point);
	if (Point.y <= pH.y)
		m_bWeHavePopup = FALSE;
}

COleDateTime CXTPCalendarTimeLineView::GetStartViewDate() const
{
	ASSERT(m_pTimeScale);
	return m_pTimeScale ? m_pTimeScale->GetStartViewDate() : COleDateTime::GetCurrentTime();
}

void CXTPCalendarTimeLineView::SetStartViewDate(COleDateTime dt)
{
	ASSERT(m_pTimeScale);
	if (m_pTimeScale)
		m_pTimeScale->SetStartViewDate(dt);

	Populate();
}

COleDateTime CXTPCalendarTimeLineView::GetViewDayDate(int nIndex) const
{
	return CXTPCalendarUtils::ResetTime(GetStartViewDate());
}


