// XTPCalendarWeekViewEvent.cpp: implementation of the CXTPCalendarWeekViewEvent class.
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

#include "Common/XTPSmartPtrInternalT.h"
#include "Common/XTPColorManager.h"

#include "XTPCalendarDefines.h"
#include "XTPCalendarOptions.h"
#include "XTPCalendarPtrCollectionT.h"

#include "XTPCalendarEvent.h"

#include "XTPCalendarView.h"
#include "XTPCalendarViewEvent.h"
#include "XTPCalendarViewDay.h"
#include "XTPCalendarViewPart.h"

#include "XTPCalendarWeekViewEvent.h"
#include "XTPCalendarWeekViewDay.h"

#include "XTPCalendarPaintManager.h"
#include "XTPCalendarControl.h"
#include "XTPCalendarTheme.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CXTPCalendarWeekViewEvent, CXTPCalendarViewEvent)

CXTPCalendarWeekViewEvent::CXTPCalendarWeekViewEvent(CXTPCalendarEvent* pEvent, CXTPCalendarWeekViewGroup* pViewGroup)
	: TBase(pEvent, pViewGroup)
{

}

CXTPCalendarWeekViewEvent::~CXTPCalendarWeekViewEvent()
{
}

int CXTPCalendarWeekViewEvent::CalcIconsRect(CRect rc)
{
	rc.top -= 2;
	rc.bottom -= 2;
	return TBase::CalcIconsRect(rc);
}

void CXTPCalendarWeekViewEvent::AdjustLayout(CDC* pDC, const CRect& rcEventMax, int nEventPlaceNumber)
{
	CXTPCalendarEvent* pEvent = GetEvent();

	if (!pEvent || !pDC)
	{
		ASSERT(FALSE);
		return;
	}

	TBase::AdjustLayout(pDC, rcEventMax, nEventPlaceNumber);

	//---------------------------------------------------
	CString strLoc = GetItemTextLocation();
	CString strEventText = GetItemTextSubject();
	if (strLoc.GetLength() > 0)
	{
		strEventText += _T(" (") + strLoc + _T(")");
	}

	m_szText = XTP_SAFE_GET3(GetView(), GetPaintManager(),
					GetDayViewEventPart(), GetTextExtent(pDC, strEventText), CSize(0));
	//---------------------------------------------------

	if (!IsMultidayEvent())
	{
		m_nMultiDayEventFlags = xtpCalendarMultiDayNoMultiDay;

		int nEventHeight = XTP_SAFE_GET1(GetView(), GetRowHeight(), 0);

		m_rcEventMax.top = rcEventMax.top + nEventHeight * nEventPlaceNumber + 1;
		m_rcEventMax.bottom = m_rcEventMax.top + nEventHeight - 2;

		m_rcEventMax.left += 1;
		m_rcEvent.top = m_rcEventMax.top;
		m_rcEvent.bottom = m_rcEventMax.bottom;
		m_rcEvent.DeflateRect(4, 0, 4, 0);
		m_rcText.CopyRect(m_rcEvent);

		int nTimeCellWidth = 0;

		if (IsTimeAsClock())
		{
			nTimeCellWidth = 2 + XTP_SAFE_GET2(GetView(), GetPaintManager(), GetClockSize().cx, 0);
		}
		else
		{
			CString strTime = XTP_SAFE_GET1(GetView(), GetItemTextEventTimeMax(), _T(""));
			nTimeCellWidth = 2 + XTP_SAFE_GET3(GetView(), GetPaintManager(), GetWeekViewEventPart(), GetTextExtent(pDC, strTime).cx, 0);
		}

		DWORD dwOpt = XTP_SAFE_GET2(GetCalendarControl(), GetCalendarOptions(), dwAdditionalOptions, 0);
		int nText = nTimeCellWidth + CalcIconsRect(m_rcText) + 5;
		if (m_rcText.Width() - nText > 1 ||
			(dwOpt & (xtpCalendarOptWeekViewShowStartTimeAlways | xtpCalendarOptWeekViewShowEndTimeAlways)))
		{
			m_rcStartTime.CopyRect(m_rcEvent);
			m_rcStartTime.right = min(m_rcStartTime.left + nTimeCellWidth, m_rcEvent.right);
			m_rcText.left = m_rcStartTime.right;

			if (IsShowEndTime() &&
				(m_rcText.Width() - nText > 1 || (dwOpt & xtpCalendarOptWeekViewShowEndTimeAlways)))
			{
				m_rcEndTime.CopyRect(m_rcStartTime);
				m_rcEndTime.OffsetRect(m_rcStartTime.Width() + 1, 0);
				m_rcEndTime.right = min(m_rcEndTime.right, m_rcEvent.right);
				m_rcText.left = m_rcEndTime.right;
			}

			m_rcText.left = min(m_rcText.left + 5, m_rcText.right);
		}

		CRect rcIcon(m_rcText);
		rcIcon.OffsetRect(0, (m_rcText.Height() - 11)/2 - 1);
		m_rcText.left = min(m_rcText.left + CalcIconsRect(rcIcon) + 3 , m_rcEvent.right);

		m_rcSubjectEditor.CopyRect(m_rcText);
		m_rcSubjectEditor.left = max(m_rcStartTime.right, m_rcEndTime.right);
		m_rcSubjectEditor.right = m_rcEventMax.right;
		m_rcSubjectEditor.top++;
		m_rcSubjectEditor.bottom--;

	}
	else
	{
		//calc icons rect
		AdjustMultidayEvent(rcEventMax, nEventPlaceNumber);

		if (!pEvent->IsAllDayEvent() &&
			m_nMultiDayEventFlags & xtpCalendarMultiDayLast)
		{
			COleDateTimeSpan spDay(1, 0, 0, 0);
			COleDateTime dtDay = XTP_SAFE_GET2(GetViewGroup_(), GetViewDay_(), GetDayDate(), (DATE)0);
			if (dtDay != m_ptrEvent->GetEndTime() - spDay)
			{
				CSize szClock = XTP_SAFE_GET2(GetView(), GetPaintManager(), GetClockSize(), CSize(0));
				m_rcLastClock.CopyRect(m_rcEvent);
				m_rcLastClock.top -= 1;
				m_rcLastClock.bottom -= 1;
				m_rcLastClock.right += 1;
				m_rcLastClock.left = m_rcLastClock.right - szClock.cx;
				m_rcText.right = m_rcLastClock.left;
			}
		}
	}
}

void CXTPCalendarWeekViewEvent::AdjustLayout2(CDC* pDC, const CRect& rcEventMax, int nEventPlaceNumber)
{
	TBase::AdjustLayout(pDC, rcEventMax, nEventPlaceNumber);

	m_nMultiDayEventFlags = 0;

	CXTPCalendarView* pView = GetView();
	CXTPCalendarEvent* pEvent = GetEvent();

	if (!pEvent  || !pDC ||
		!pView || !XTP_SAFE_GET1(pView, GetTheme(), NULL) )
	{
		ASSERT(FALSE);
		return;
	}
	CXTPCalendarTheme* pTheme = pView->GetTheme();

	// all-day event
	if (IsMultidayEvent())
	{
		// to calculate multiday-flags
		AdjustMultidayEvent(rcEventMax, nEventPlaceNumber);

		pTheme->GetWeekViewPart()->GetDayPart()->GetMultiDayEventPart()->AdjustLayout(this, pDC, rcEventMax, nEventPlaceNumber);
		return;

	}

	m_nMultiDayEventFlags = xtpCalendarMultiDayNoMultiDay;

	pTheme->GetWeekViewPart()->GetDayPart()->GetSingleDayEventPart()->AdjustLayout(this, pDC, rcEventMax, nEventPlaceNumber);
}

void CXTPCalendarWeekViewEvent::Draw(CDC* pDC)
{
	XTP_SAFE_CALL3(GetView(), GetPaintManager(), GetWeekViewEventPart(),
					OnDraw(pDC, this) );
}

BOOL CXTPCalendarWeekViewEvent::HitTestEx(CPoint point, XTP_CALENDAR_HITTESTINFO* pInfo) const
{
	if (!m_rcEventMax.PtInRect(point) || !IsVisible())
	{
		return FALSE;
	}

	if (!pInfo || !GetViewGroup() || !GetViewGroup()->GetViewDay() || !GetEvent())
	{
		ASSERT(FALSE);
		return FALSE;
	}

	pInfo->dt = GetViewGroup()->GetViewDay()->GetDayDate();
	pInfo->pViewDay = GetViewGroup()->GetViewDay();
	pInfo->pViewGroup = GetViewGroup();
	pInfo->pViewEvent = (CXTPCalendarViewEvent*)this;

	BOOL bAllDay = GetEvent()->IsAllDayEvent();
	BOOL bNoMDay = (m_nMultiDayEventFlags & xtpCalendarMultiDayFMLmask) == 0;

	if (IsVisible() && bNoMDay && !bAllDay)
		pInfo->uHitCode = xtpCalendarHitTestEventDragArea;
	else
		pInfo->uHitCode = xtpCalendarHitTestUnknown;

	BOOL bIsResizable = m_nMultiDayEventFlags & xtpCalendarMultiDayFirst ||
						m_nMultiDayEventFlags & xtpCalendarMultiDayLast;

	CRect rcBegin = m_rcEventMax;
	rcBegin.right = min(m_rcEvent.left + 5, m_rcEvent.right);
	if ((!bIsResizable || (m_nMultiDayEventFlags & xtpCalendarMultiDayLast)) && !bAllDay)
	{
		rcBegin = m_rcEvent;
		rcBegin.right = rcBegin.left;
	}

	CRect rcEnd = m_rcEventMax;
	rcEnd.left = max(m_rcEvent.right - 5, m_rcEvent.left);
	if ((!bIsResizable || (m_nMultiDayEventFlags & xtpCalendarMultiDayFirst)) && !bAllDay)
	{
		rcEnd = m_rcEvent;
		rcEnd.left = rcEnd.right;
	}

	if (bIsResizable && rcBegin.PtInRect(point) && IsVisible())
	{
		pInfo->uHitCode = xtpCalendarHitTestEventResizeHArea | xtpCalendarHitTestEventResizeBegin;
		return TRUE;
	}

	if (bIsResizable && rcEnd.PtInRect(point) && IsVisible())
	{
		pInfo->uHitCode = xtpCalendarHitTestEventResizeHArea | xtpCalendarHitTestEventResizeEnd;
		return TRUE;
	}

	CRect rcText(m_rcEvent);
	if (bNoMDay && !bAllDay)
	{
		rcText.left = rcText.left + GetStartTimeRect().Width() + GetEndTimeRect().Width();
		rcText.left += 5;
		rcText.right = rcEnd.left;
	}

	if (rcText.PtInRect(point))
	{
		pInfo->uHitCode = xtpCalendarHitTestEventTextArea;
		//*pInfo = tmpInfo;
		return TRUE;
	}

	return TRUE;
}

BOOL CXTPCalendarWeekViewEvent::IsVisible() const
{
//<<EventVisible>>
	if (GetEvent() && !GetEvent()->IsEventVisible())
		return FALSE;
//<<EventVisible>>

	ASSERT(XTP_SAFE_GET1(GetViewGroup(), GetViewDay(), NULL));
	if (!XTP_SAFE_GET1(GetViewGroup(), GetViewDay(), NULL))
	{
		return FALSE;
	}
	CRect rcDayEvents = GetViewGroup()->GetViewDay()->GetDayEventsRect();
	BOOL bVisible = m_rcEvent.bottom <= rcDayEvents.bottom;
	return bVisible;
}

BOOL CXTPCalendarWeekViewEvent::IsShowEndTime() const
{
	return XTP_SAFE_GET1(GetCalendarControl(), WeekView_IsShowEndDate(), FALSE);
}

BOOL CXTPCalendarWeekViewEvent::IsTimeAsClock() const
{
	return XTP_SAFE_GET1(GetCalendarControl(), WeekView_IsShowTimeAsClocks(), FALSE);
}

CFont* CXTPCalendarWeekViewEvent::GetSubjectEditorFont() const
{
	CFont* pFont = &XTP_SAFE_GET3(GetView(), GetPaintManager(), GetWeekViewEventPart(), GetTextFont(), *((CFont*)(NULL)));
	ASSERT(pFont);
	return pFont;
}

void CXTPCalendarWeekViewEvent::OnPostAdjustLayout()
{
	TBase::OnPostAdjustLayout();

	BOOL bIsAllDayEvent = XTP_SAFE_GET1(GetEvent(), IsAllDayEvent(), FALSE);

	BOOL bAMDay = m_nMultiDayEventFlags & xtpCalendarMultiDayFirst ||
		m_nMultiDayEventFlags & xtpCalendarMultiDayMiddle ||
		m_nMultiDayEventFlags & xtpCalendarMultiDayLast ||
		bIsAllDayEvent;

	if (bAMDay)
	{
		m_rcSubjectEditor.left = m_rcEvent.left;
		m_rcSubjectEditor.right = m_rcEvent.right;
		if (m_nMultiDayEventFlags & xtpCalendarMultiDayFirst ||
			bIsAllDayEvent)
		{
			m_rcSubjectEditor.left += 1;
		}
		m_rcReminderGlyph.OffsetRect(0, (m_rcText.Height() - 11)/2 - 1);
		m_rcRecurrenceGlyph.OffsetRect(0, (m_rcText.Height() - 11)/2 - 1);
		m_rcMeetingGlyph.OffsetRect(0, (m_rcText.Height() - 11)/2 - 1);
		m_rcPrivateGlyph.OffsetRect(0, (m_rcText.Height() - 11)/2 - 1);
	}

	BOOL bOutOfBorders = m_rcText.Width() < m_szText.cx;
	SetTextOutOfBorders(bOutOfBorders);
}
