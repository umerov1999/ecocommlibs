// XTPCalendarDayViewDay.cpp: implementation of the CXTPCalendarDayViewDay class.
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

#include "Common/XTPSmartPtrInternalT.h"
#include "Common/XTPNotifyConnection.h"
#include "Common/XTPColorManager.h"

#include "XTPCalendarUtils.h"
#include "XTPCalendarDefines.h"
#include "XTPCalendarPtrCollectionT.h"

#include "XTPCalendarResource.h"
#include "XTPCalendarEvent.h"
#include "XTPCalendarNotifications.h"


#include "XTPCalendarView.h"
#include "XTPCalendarViewDay.h"
#include "XTPCalendarViewEvent.h"
#include "XTPCalendarViewPart.h"

#include "XTPCalendarDayViewEvent.h"
#include "XTPCalendarDayViewDay.h"
#include "XTPCalendarDayView.h"

#include "XTPCalendarControl.h"

#include "XTPCalendarPaintManager.h"
#include "XTPCalendarTheme.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CXTPCalendarDayViewDay, CXTPCalendarViewDay)
IMPLEMENT_DYNAMIC(CXTPCalendarDayViewGroup, CXTPCalendarViewGroup)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXTPCalendarDayViewDay::CXTPCalendarDayViewDay(CXTPCalendarDayView* pDayView, COleDateTime dtDate)
		: TBase(pDayView)
{
	m_dtDate = dtDate;

}

CXTPCalendarDayViewDay::~CXTPCalendarDayViewDay()
{
}

void CXTPCalendarDayViewDay::Populate(COleDateTime dtDayDate)
{
	m_dtDate = CXTPCalendarUtils::ResetTime(dtDayDate);
	m_arViewGroups.RemoveAll();

	if (!GetView_())
	{
		return;
	}

	OnPrePopulateDay();

	CXTPCalendarResources* pResources = GetResources();
	if (!pResources)
		return;

	CUIntArray arScheduleIDs;
	CUIntArray arGroupIDs;
	BOOL bLinks(FALSE);
	BOOL bFind(FALSE);
	BOOL bHide(FALSE);
	BOOL bMultipleSchedulesMode(FALSE);
	int nCount = pResources->GetCount();
	for (int i = 0; i < nCount; i++)
	{
		CXTPCalendarResource* pRC = pResources->GetAt(i);
		if (!pRC)
		{
			ASSERT(FALSE);
			continue;
		}
		CString sCap = pRC->GetName();
		int iSch = (int) pRC->GetSchedules()->GetSize();
		for (int I = 0; I < iSch; I++)
		{
			int iSchID = pRC->GetSchedules()->GetAt(I);
			arScheduleIDs.Add(iSchID);
			arGroupIDs.Add(i);
		}

		CXTPCalendarDayViewGroup* pGroup = new CXTPCalendarDayViewGroup(this);
		if (!pGroup)
			return;

		m_arViewGroups.Add(pGroup);

		pGroup->AddResource(pRC, TRUE);
		pGroup->m_GroupColor = pRC->GetColor();

		pGroup->Populate(m_dtDate);

		if (!bFind && pGroup->GetCalendarControl())
		{
			bFind = TRUE;
			bLinks = pGroup->GetCalendarControl()->m_bShowLinks;
			bHide = pGroup->GetCalendarControl()->m_bHideLinkContainer;
			bMultipleSchedulesMode = pGroup->GetCalendarControl()->m_bMultipleSchedulesMode;
		}

		if (bMultipleSchedulesMode && bHide && iSch > 1)
			pGroup->m_bHide = TRUE;
	}

	if (!bMultipleSchedulesMode && !bLinks)
		return;

	int nViewGroupsCount = GetViewGroupsCount();
	if (nViewGroupsCount > 1)
	{
		for (int i = 0; i < nViewGroupsCount; i++)
		{
			CXTPCalendarDayViewGroup* pGroup = GetViewGroup(i);
			if (pGroup)
			{
				int nGroupViewEventsCount = pGroup->GetViewEventsCount();
				for (int j = 0; j < nGroupViewEventsCount; j++)
				{
					CXTPCalendarDayViewEvent* pVe = pGroup->GetViewEvent(j);
					if (pVe)
					{
						CXTPCalendarEvent* pE = pVe->GetEvent();
						if (pE)
						{
							int iOwnId = pE->GetScheduleID();
							CString sSet = pE->GetScheduleSet();
							if (sSet != _T("|"))
							{
								CString sLast;
								int jPos = sSet.Find(_T("|"), 1);
								while (jPos > -1)
								{
									sLast = sSet.Mid(jPos);
									sSet = sSet.Left(jPos);
									sSet.Replace(_T("|"), _T(""));
									int jSch = _ttoi(sSet);
									if (iOwnId != jSch)
									{
										for (int I = 0; I < (int) arScheduleIDs.GetSize(); I++)
										{
											if (jSch == (int) arScheduleIDs.GetAt(I))
											{
												int jGr = (int) arGroupIDs.GetAt(I);
												if (jGr != i)
												{
													CXTPCalendarDayViewGroup* pGroup1 = GetViewGroup(jGr);
													if (pGroup1)
													{
														if (!pGroup1->GetViewEventByEvent_(pE))
															pGroup1->AddViewEvent(pE);
													}
												}
											}
										}
									}
									sSet = sLast;
									jPos = sSet.Find(_T("|"), 1);
								}
							}
						}
					}
				}
			}
		}

		for (int iD = 0; iD < GetViewGroupsCount(); iD++)
		{
			CXTPCalendarDayViewGroup* pGroup = GetViewGroup(iD);
			if (pGroup)
			{
				if (pGroup->m_bHide)
				{
					m_arViewGroups.RemoveAt(iD);
					iD--;
				}
			}
		}
	}
}

void CXTPCalendarDayViewDay::AdjustLayout(CDC* pDC, const CRect& rcDay)
{
	if (!GetView() || !pDC)
	{
		ASSERT(FALSE);
		return;
	}

	CXTPCalendarViewDay::AdjustLayout(pDC, rcDay);
	//TBase::AdjustLayout(pDC, rcDay);

	CRect rcDayHeader = GetView()->GetDayHeaderRectangle();
	m_Layout.m_rcDayHeader.SetRect(rcDay.left, rcDayHeader.top, rcDay.right, rcDayHeader.bottom);
	CRect rcGroups = rcDay;
	rcGroups.top = m_Layout.m_rcDayHeader.bottom;

	int nGroupsCount = GetViewGroupsCount();
	int nGroupWidth = rcGroups.Width() / max(1, nGroupsCount);

	for (int i = 0; i < nGroupsCount; i++)
	{
		CXTPCalendarDayViewGroup* pViewGroup = GetViewGroup(i);
		ASSERT(pViewGroup);

		if (pViewGroup)
		{
			CRect rcGroupI = rcGroups;
			rcGroupI.left += nGroupWidth * i;
			if (i < nGroupsCount - 1)
			{
				rcGroupI.right = rcGroupI.left + nGroupWidth;
			}

			pViewGroup->AdjustLayout(pDC, rcGroupI);
		}
	}
}

void CXTPCalendarDayViewDay::AdjustLayout2(CDC* pDC, const CRect& rcDay)
{
	if (!pDC || !GetView() || !GetView()->GetTheme())
	{
		ASSERT(FALSE);
		return;
	}

	CXTPCalendarViewDay::AdjustLayout(pDC, rcDay);

	GetView()->GetTheme()->GetDayViewPart()->GetDayPart()->AdjustLayout(this, pDC, rcDay);
}

CString CXTPCalendarDayViewDay::GetCaption() const
{
	COleDateTime dt(m_dtDate);
	SYSTEMTIME st;
	CXTPCalendarUtils::GetAsSystemTime(dt, st);

	CString strHeaderFormat = GetView() ? GetView()->GetDayHeaderFormat() : _T("");
	CString strResult = CXTPCalendarUtils::GetDateFormat(&st, strHeaderFormat);

	if (strHeaderFormat.Find(_T("%")) > -1) //C-format used
		strResult = dt.Format(strHeaderFormat);

	//-------------------------------------------------------------
	DWORD dwFlags = XTP_SAFE_GET2(GetView(), GetCalendarControl(), GetAskItemTextFlags(), 0);

	if (dwFlags & xtpCalendarItemText_DayViewDayHeader)
	{
		XTP_CALENDAR_GETITEMTEXT_PARAMS objRequest;
		::ZeroMemory(&objRequest, sizeof(objRequest));

		objRequest.nItem = (int)xtpCalendarItemText_DayViewDayHeader;
		objRequest.pstrText = &strResult;
		objRequest.pViewDay = (CXTPCalendarViewDay*)this;

		XTP_SAFE_CALL2(GetView(), GetCalendarControl(), SendNotificationAlways(
						XTP_NC_CALENDAR_GETITEMTEXT, (WPARAM)&objRequest, 0));
	}
	//-------------------------------------------------------------

	return strResult;
}

void CXTPCalendarDayViewDay::Draw(CDC* pDC)
{
	// draw groups
	int nGroupsCount = GetViewGroupsCount();

	for (int i = 0; i < nGroupsCount; i++)
	{
		CXTPCalendarDayViewGroup* pViewGroup = GetViewGroup(i);
		ASSERT(pViewGroup);

		if (pViewGroup)
		{
			pViewGroup->Draw(pDC);
		}
	}
	// draw header part
	XTP_SAFE_CALL3(GetView(), GetPaintManager(), GetDayViewHeaderPart(),
		OnDraw(pDC, this, m_Layout.m_rcDayHeader, GetCaption()) );

}

void CXTPCalendarDayViewDay::DrawTopmostLayer(CDC* pDC)
{
	const int nGroupsCount = GetViewGroupsCount();
	for (int i = 0; i < nGroupsCount; i++)
	{
		CXTPCalendarDayViewGroup* pViewGroup = GetViewGroup(i);
		ASSERT(pViewGroup);

		if (pViewGroup)
		{
			pViewGroup->DrawAllDayAreaScrollIcons(pDC);
		}
	}
}

void CXTPCalendarDayViewDay::FillHitTestEx(XTP_CALENDAR_HITTESTINFO* pInfo) const
{
	if (!pInfo)
	{
		ASSERT(FALSE);
		return;
	}

	pInfo->dt = m_dtDate;
	pInfo->pViewDay = (CXTPCalendarViewDay*)this;
	pInfo->pViewGroup = NULL;
	pInfo->pViewEvent = NULL;
	pInfo->uHitCode = xtpCalendarHitTestUnknown;
}

void CXTPCalendarDayViewDay::OnMouseMove(UINT nFlags, CPoint point)
{
	if (GetView() && GetView()->GetTheme() && GetView()->GetTheme()->GetDayViewPart() &&
		GetView()->GetTheme()->GetDayViewPart()->GetDayPart())
		GetView()->GetTheme()->GetDayViewPart()->GetDayPart()->OnMouseMove(this, nFlags, point);

	TBase::OnMouseMove(nFlags, point);
}

//////////////////////////////////////////////////////////////////////////

CXTPCalendarDayViewGroup::CXTPCalendarDayViewGroup(CXTPCalendarDayViewDay* pViewDay)
: TBase(pViewDay)
{
	m_LayoutX.m_rcAllDayEvents.SetRectEmpty();
	m_LayoutX.m_rcDayDetails.SetRectEmpty();

	m_LayoutX.m_bShowHeader = TRUE;
	m_LayoutX.m_nHotState = 0;

	m_bExpandUP = FALSE;
	m_bExpandDOWN = FALSE;

	m_bIsShowAllDayEventsUpScrollButton = TRUE;
	m_bIsShowAllDayEventsDownScrollButton = TRUE;

	m_bHide = FALSE;
	m_GroupColor = (COLORREF)-1;
}

CXTPCalendarDayViewGroup::~CXTPCalendarDayViewGroup()
{
}

void CXTPCalendarDayViewGroup::AdjustLayout(CDC* pDC, const CRect& rcGroup)
{
	if (!pDC || !GetViewDay() || !GetViewDay()->GetView())
	{
		ASSERT(FALSE);
		return;
	}

	m_bExpandUP = FALSE;
	m_bExpandDOWN = FALSE;

	m_Layout.m_rcGroup = rcGroup;

	m_Layout.m_rcGroupHeader = rcGroup;
	m_Layout.m_rcGroupHeader.bottom = m_Layout.m_rcGroupHeader.top; // set empty rect

	m_LayoutX.m_bShowHeader = XTP_SAFE_GET2(GetViewDay(), GetView(), IsGroupHeaderVisible(), TRUE);

	if (m_LayoutX.m_bShowHeader)
	{
		CRect rcHeader = GetViewDay()->GetView()->GetDayHeaderRectangle();
		m_Layout.m_rcGroupHeader.bottom += rcHeader.Height();
	}

	CRect rcAllDayEvents = GetViewDay()->GetView()->GetAllDayEventsRectangle();
	m_LayoutX.m_rcAllDayEvents = rcGroup;
	m_LayoutX.m_rcAllDayEvents.top = m_Layout.m_rcGroupHeader.bottom;
	m_LayoutX.m_rcAllDayEvents.bottom = m_LayoutX.m_rcAllDayEvents.top + rcAllDayEvents.Height();

	m_LayoutX.m_rcDayDetails = rcGroup;
	m_LayoutX.m_rcDayDetails.top = m_LayoutX.m_rcAllDayEvents.bottom;

	int nAllDayEventPlace = -GetViewDay()->GetView()->m_LayoutX.m_nTopAllDayRow;

	// adjust layout of events of this day
	//m_bExpandUP = FALSE;
	//m_bExpandDOWN = FALSE;

	int nEventsCount = GetViewEventsCount();
	for (int i = 0; i < nEventsCount; i++)
	{
		CXTPCalendarDayViewEvent* pViewEvent = GetViewEvent(i);
		ASSERT(pViewEvent);

		if (pViewEvent && pViewEvent->IsMultidayEvent())
		{
			rcAllDayEvents = m_LayoutX.m_rcAllDayEvents;
			rcAllDayEvents.top += 1;
			pViewEvent->AdjustLayout(pDC, rcAllDayEvents, nAllDayEventPlace);
			nAllDayEventPlace++;
		}
	}

	AdjustDayEvents(pDC);
}

void CXTPCalendarDayViewGroup::AdjustLayout2(CDC* pDC, const CRect& rcGroup)
{
	if (!pDC || !GetViewDay() || !GetViewDay()->GetView() || !GetViewDay()->GetView()->GetTheme())
	{
		ASSERT(FALSE);
		return;
	}

	m_bExpandUP = FALSE;
	m_bExpandDOWN = FALSE;

	m_Layout.m_rcGroup = rcGroup;

	m_Layout.m_rcGroupHeader = rcGroup;
	m_Layout.m_rcGroupHeader.bottom = m_Layout.m_rcGroupHeader.top; // set empty rect

	GetViewDay()->GetView()->GetTheme()->GetDayViewPart()->GetDayPart()->GetGroupPart()->AdjustLayout(this, pDC, rcGroup);
}

CRect CXTPCalendarDayViewGroup::GetAllDayEventsScrollButtonRect(XTP_DAY_VIEW_GROUP_LAYOUT::XTPEnumHotItem nButton) const
{
	CRect rc = GetAllDayEventsRect();

	rc.left = GetAllDayEventsRect().right - 23;
	rc.right = rc.left + 20;

	switch (nButton)
	{
	case XTP_DAY_VIEW_GROUP_LAYOUT::xtpHotAllDayEventsScrollUp:
		//rc.top += 9;
		rc.bottom = rc.top + 13;
		break;
	case XTP_DAY_VIEW_GROUP_LAYOUT::xtpHotAllDayEventsScrollDown:
		rc.top = rc.bottom - 13;
		rc.bottom = rc.top + 9;
		break;
	default:
		ASSERT(FALSE);
		rc.SetRect(0, 0, 0, 0);
	}

	return rc;
}

BOOL CXTPCalendarDayViewGroup::NeedScrollAllDayEvents() const
{
	CXTPCalendarDayView* pDayView = GetViewDay()->GetView();
	if(!pDayView)
		return FALSE;

	const int eventsInAllDayAreaCount = GetEventsCountInAllDayArea();
	if(eventsInAllDayAreaCount <= 0)
		return FALSE;

	const XTP_DAY_VIEW_LAYOUT& rDayLayout = pDayView->GetLayout();
	if( GetEventsCountInAllDayArea() > rDayLayout.m_nVisibleAllDayCount )
		return TRUE;

	if(GetFirstVisibleAllDayEvent() > 0)
		return TRUE;

	if(GetLastVisibleAllDayEvent() > GetEventsCountInAllDayArea())
		return TRUE;

	return FALSE;
}

int CXTPCalendarDayViewGroup::GetEventsCountInAllDayArea() const
{
	int allDayEventsCount = 0;
	const int viewEventsCount = GetViewEventsCount();
	for(int iEvent = 0; iEvent < viewEventsCount; iEvent++)
	{
		BOOL bIsAllDay = XTP_SAFE_GET2(GetViewEvent_(iEvent), GetEvent(), IsAllDayEvent(), FALSE);
		BOOL bIsMultiDay = XTP_SAFE_GET1(GetViewEvent_(iEvent), IsMultidayEvent(), FALSE);
		if(bIsAllDay || bIsMultiDay)
			allDayEventsCount++;
	}

	return allDayEventsCount;
}

int CXTPCalendarDayViewGroup::GetFirstVisibleAllDayEvent() const
{
	CXTPCalendarDayView* pDayView = GetViewDay()->GetView();
	if(!pDayView)
		return -1;

	XTP_DAY_VIEW_LAYOUT& rDayLayout = pDayView->GetLayout();

	int iFisrtVisible = rDayLayout.m_nTopAllDayRow;
	if (iFisrtVisible + rDayLayout.m_nVisibleAllDayCount > rDayLayout.m_nAllDayEventsCountMax)
	{
		iFisrtVisible = rDayLayout.m_nAllDayEventsCountMax - rDayLayout.m_nVisibleAllDayCount;
	}

	return max(0, iFisrtVisible);
}

int CXTPCalendarDayViewGroup::GetLastVisibleAllDayEvent() const
{
	CXTPCalendarDayView* pDayView = GetViewDay()->GetView();
	if(!pDayView)
		return -1;

	XTP_DAY_VIEW_LAYOUT& rDayLayout = pDayView->GetLayout();
	int iFirstVisible = GetFirstVisibleAllDayEvent();
	if(iFirstVisible < 0)
		return -1;

	return min(GetEventsCountInAllDayArea(), iFirstVisible + rDayLayout.m_nVisibleAllDayCount);
}

BOOL CXTPCalendarDayViewGroup::IsUpAllDayEventsScrollButtonVisible() const
{
	if(!IsShowAllDayEventsUpScrollButton())
		return FALSE;

	return GetFirstVisibleAllDayEvent() > 0;
}

BOOL CXTPCalendarDayViewGroup::IsDownAllDayEventsScrollButtonVisible() const
{
	if(!IsShowAllDayEventsDownScrollButton())
		return FALSE;

	return GetLastVisibleAllDayEvent() < GetEventsCountInAllDayArea();
}

BOOL CXTPCalendarDayViewGroup::IsShowAllDayEventsUpScrollButton()  const
{
	return m_bIsShowAllDayEventsUpScrollButton;
}

BOOL CXTPCalendarDayViewGroup::IsShowAllDayEventsDownScrollButton() const
{
	return m_bIsShowAllDayEventsDownScrollButton;
}

void CXTPCalendarDayViewGroup::SetShowAllDayEventsUpScrollButton(BOOL bShow)
{
	m_bIsShowAllDayEventsUpScrollButton = bShow;
}

void CXTPCalendarDayViewGroup::SetShowAllDayEventsDownScrollButton(BOOL bShow)
{
	m_bIsShowAllDayEventsDownScrollButton = bShow;
}

void CXTPCalendarDayViewGroup::Draw(CDC* pDC)
{
	int nRowHeight = GetRowHeight();

	if (nRowHeight <= 0 ||
		!XTP_SAFE_GET2(GetViewDay(), GetView(), GetPaintManager(), NULL))
	{
		return;
	}

	int nRowCount = GetVisibleRowCount();
	int nTopRow = GetTopRow();

	BOOL bAllDayEventSelected = FALSE;
	GetViewDay()->GetView()->GetSelection(NULL, NULL, &bAllDayEventSelected);

	BOOL bHasSelectedEvents = GetViewDay()->GetView()->HasSelectedViewEvent();
	int nGroupIndex = GetGroupIndex();
	// draw rows grid
	for (int i = 0; i < nRowCount; i++)
	{
		CRect rcCell(m_Layout.m_rcGroup.left, m_LayoutX.m_rcDayDetails.top + nRowHeight * i, m_Layout.m_rcGroup.right, m_LayoutX.m_rcDayDetails.top + nRowHeight * (i + 1));

		COleDateTime dtDateTime = GetCellDateTime(nTopRow + i) + COleDateTimeSpan(0, 0, 0, 1);

		BOOL bSelected = !bAllDayEventSelected && !bHasSelectedEvents &&
			GetViewDay()->GetView()->SelectionContains(dtDateTime, nGroupIndex);

		CXTPCalendarPaintManager::CDayViewCellPart* pViewPart = (CXTPCalendarPaintManager::CDayViewCellPart*)GetCellViewPart(dtDateTime);
		ASSERT(pViewPart);
		if (pViewPart)
		{
			XTP_CALENDAR_DAYVIEWCELL_PARAMS cellParams;
			::ZeroMemory(&cellParams, sizeof(cellParams));

			cellParams.bSelected = bSelected;
			cellParams.nIndex = nTopRow + i;
			cellParams.dtBeginTime = dtDateTime;

			pViewPart->GetParams(this, &cellParams);

			//---------------------------------
			WPARAM wParam = (WPARAM)this;
			LPARAM lParam = (LPARAM)&cellParams;
			XTP_SAFE_CALL2(GetViewDay_(), GetView_(),
				SendNotification(XTP_NC_CALENDAR_BEFORE_DRAW_DAYVIEWCELL, wParam, lParam));
			//---------------------------------

			// these members are read only. changed ???
			ASSERT(cellParams.bSelected == bSelected);
			ASSERT(cellParams.nIndex == nTopRow + i);
			ASSERT(cellParams.dtBeginTime == dtDateTime);

			// ensure to have default values
			cellParams.bSelected = bSelected;
			cellParams.nIndex = nTopRow + i;
			cellParams.dtBeginTime = dtDateTime;
			//---------------------------------
			pViewPart->OnDraw(pDC, this, rcCell, cellParams);
		}
	}

	//1. draw no multiday, no selected events
	int nEvent = 0;
	int nEventsCount = GetViewEventsCount();

	for (nEvent = 0; nEvent < nEventsCount; nEvent++)
	{
		CXTPCalendarDayViewEvent* pViewEvent = GetViewEvent(nEvent);
		if (pViewEvent && !pViewEvent->IsMultidayEvent() && !pViewEvent->IsSelected())
		{
			pViewEvent->Draw(pDC);
		}
	}

	//2. draw no multiday, selected events
	for (nEvent = 0; nEvent < nEventsCount; nEvent++)
	{
		CXTPCalendarDayViewEvent* pViewEvent = GetViewEvent(nEvent);
		if (pViewEvent && !pViewEvent->IsMultidayEvent() && pViewEvent->IsSelected())
		{
			pViewEvent->Draw(pDC);
		}
	}

	// draw header part

	if (m_LayoutX.m_bShowHeader)
	{
		XTP_SAFE_CALL4(GetViewDay(), GetView(), GetPaintManager(), GetDayViewGroupHeaderPart(),
			OnDraw(pDC, this, m_Layout.m_rcGroupHeader, GetCaption()) );
	}

	// draw all day events area
	if (!m_LayoutX.m_rcAllDayEvents.IsRectEmpty())
	{
		BOOL bSelected = !bHasSelectedEvents && GetViewDay()->GetView()->SelectionContains(GetViewDay()->GetDayDate()) &&
			GetViewDay()->GetView()->SelectionContains(GetViewDay()->GetDayDate() + COleDateTimeSpan(0, 23, 59, 59),
			nGroupIndex);

		XTP_SAFE_CALL4(GetViewDay(), GetView(), GetPaintManager(), GetDayViewAllDayEventsPart(),
			OnDraw(pDC, this, m_LayoutX.m_rcAllDayEvents, bSelected) );
	}

	// draw all day events
	for (nEvent = 0; nEvent < nEventsCount; nEvent++)
	{
		CXTPCalendarDayViewEvent* pViewEvent = GetViewEvent(nEvent);
		if (pViewEvent && pViewEvent->IsMultidayEvent())
		{
			pViewEvent->Draw(pDC);
		}
	}
}

void CXTPCalendarDayViewGroup::DrawAllDayAreaScrollIcons(CDC* pDC)
{
	BOOL bNeedScroll = NeedScrollAllDayEvents();
	if (!bNeedScroll)
		return;

	CRect rcIconsUp = GetAllDayEventsScrollButtonRect(XTP_DAY_VIEW_GROUP_LAYOUT::xtpHotAllDayEventsScrollUp);
	CRect rcIconsDown = GetAllDayEventsScrollButtonRect(XTP_DAY_VIEW_GROUP_LAYOUT::xtpHotAllDayEventsScrollDown);

	BOOL bHighlightUp = IsUpAllDayEventsScrollButtonVisible();
	BOOL bHighlightDown = IsDownAllDayEventsScrollButtonVisible();

	XTP_SAFE_CALL4(GetViewDay(), GetView(), GetPaintManager(), GetDayViewAllDayEventScrollIconsPart(),
		OnDraw(pDC, this, IsShowAllDayEventsUpScrollButton(), IsShowAllDayEventsDownScrollButton(), rcIconsUp, rcIconsDown,
		bHighlightUp, bHighlightDown) );
}

BOOL CXTPCalendarDayViewGroup::HitTestAllDayEventsScrollIcons(CPoint pt, XTP_CALENDAR_HITTESTINFO* pHitTest) const
{
	if(IsShowAllDayEventsUpScrollButton())
	{
		CRect rcIconsUp = GetAllDayEventsScrollButtonRect(XTP_DAY_VIEW_GROUP_LAYOUT::xtpHotAllDayEventsScrollUp);
		if(rcIconsUp.PtInRect(pt))
		{
			FillHitTestEx(pHitTest);
			pHitTest->uHitCode = xtpCalendarHitTestDayViewScrollUp;
			return TRUE;
		}
	}


	if(IsShowAllDayEventsDownScrollButton())
	{
		CRect rcIconsDown = GetAllDayEventsScrollButtonRect(XTP_DAY_VIEW_GROUP_LAYOUT::xtpHotAllDayEventsScrollDown);
		if(rcIconsDown.PtInRect(pt))
		{
			FillHitTestEx(pHitTest);
			pHitTest->uHitCode = xtpCalendarHitTestDayViewScrollDown;
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CXTPCalendarDayViewGroup::HitTestEx(CPoint pt, XTP_CALENDAR_HITTESTINFO* pHitTest) const
{
	if (!pHitTest || !GetViewDay())
	{
		ASSERT(FALSE);
		return FALSE;
	}

	int nHit = XTP_SAFE_GET5(GetViewDay()->GetView(), GetTheme(), GetDayViewPart(), GetDayPart(),
					GetGroupPart(), HitTestScrollButton(this, &pt), 0);
	if (nHit)
	{
		FillHitTestEx(pHitTest);
		pHitTest->uHitCode = nHit;
		return TRUE;
	}

	BOOL bHit = FALSE;

	if(NeedScrollAllDayEvents())
	{
		bHit = HitTestAllDayEventsScrollIcons(pt, pHitTest);
		if(bHit)
			return TRUE;
	}

	//- process selected day events at the first -------------------------
	int nEventsCount = GetViewEventsCount();
	for (int nEvent = 0; nEvent < nEventsCount; nEvent++)
	{
		CXTPCalendarDayViewEvent* pViewEvent = GetViewEvent(nEvent);
		ASSERT(pViewEvent);

		if (pViewEvent && pViewEvent->IsSelected() && !pViewEvent->IsMultidayEvent())
		{
			bHit = pViewEvent->HitTestEx(pt, pHitTest);
			if (bHit)
			{
				pHitTest->nEvent = nEvent;
				pHitTest->pViewEvent = pViewEvent;
				break;
			}
		}
	}

	//---------------------------------------------------------------------------
	if (!bHit)
	{
		bHit = TBase::HitTestEx(pt, pHitTest);
	}

	//---------------------------------------------------------------------------
	if (!bHit)
	{
		XTP_CALENDAR_HITTESTINFO hitInfo;
		hitInfo.pt = pt;
		FillHitTestEx(&hitInfo);

		bHit = HitTestDateTime(&hitInfo);
		if (bHit)
		{
			*pHitTest = hitInfo;
		}
	}
	else
	{
		pHitTest->pt = pt;
		HitTestDateTime(pHitTest);
	}


	return bHit;
}

void CXTPCalendarDayViewGroup::FillHitTestEx(XTP_CALENDAR_HITTESTINFO* pHitTest) const
{
	ASSERT(pHitTest && GetViewDay());
	if (pHitTest && GetViewDay())
	{
		GetViewDay()->FillHitTestEx(pHitTest);
		pHitTest->pViewGroup = (CXTPCalendarViewGroup*)this;
	}
}

BOOL CXTPCalendarDayViewGroup::HitTestDateTime(XTP_CALENDAR_HITTESTINFO* pHitTest) const
{
	if (!pHitTest || !GetViewDay())
	{
		ASSERT(FALSE);
		return FALSE;
	}
	int nVisibleRowCount = GetVisibleRowCount();

	if (m_LayoutX.m_rcDayDetails.PtInRect(pHitTest->pt) && nVisibleRowCount > 0)
	{
		int nCell = (pHitTest->pt.y - m_LayoutX.m_rcDayDetails.top) / max(GetRowHeight(), 1);

		if (nCell >= 0 && nCell < nVisibleRowCount)
		{
			pHitTest->dt = GetCellDateTime(nCell + GetTopRow());
			pHitTest->bTimePartValid = TRUE;
			pHitTest->uHitCode |= xtpCalendarHitTestDayViewCell;

			return TRUE;
		}
	}

	if (m_LayoutX.m_rcAllDayEvents.PtInRect(pHitTest->pt) ||
		m_Layout.m_rcGroupHeader.PtInRect(pHitTest->pt) ||
		GetViewDay()->m_Layout.m_rcDayHeader.PtInRect(pHitTest->pt))
	{
		pHitTest->dt = GetViewDay()->GetDayDate();
		pHitTest->bTimePartValid = FALSE;
		pHitTest->uHitCode |= xtpCalendarHitTestDayViewAllDayEvent;
		return TRUE;
	}

	return FALSE;
}

BOOL CXTPCalendarDayViewGroup::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (!GetViewDay())
	{
		ASSERT(FALSE);
		return FALSE;
	}

	if (XTP_SAFE_GET5(GetViewDay()->GetView(), GetTheme(), GetDayViewPart(), GetDayPart(),
		GetGroupPart(), OnLButtonDown(this, nFlags, point), FALSE))
	{
		return TRUE;
	}

	if(OnAllDaysAreaClick(nFlags, point))
		return TRUE;

	return TBase::OnLButtonDown(nFlags, point);
}

BOOL CXTPCalendarDayViewGroup::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	if(OnAllDaysAreaClick(nFlags, point))
		return TRUE;

	return TBase::OnLButtonDblClk(nFlags, point);
}

BOOL CXTPCalendarDayViewGroup::OnAllDaysAreaClick(UINT nFlags, CPoint point)
{
	UNREFERENCED_PARAMETER(nFlags);

	if(!NeedScrollAllDayEvents())
		return FALSE;

	XTP_CALENDAR_HITTESTINFO hitTest;
	HitTestAllDayEventsScrollIcons(point, &hitTest);

	if(hitTest.uHitCode == xtpCalendarHitTestDayViewScrollUp)
	{
		ScrollIcon_ScrollAllDayEventsUp();
		return TRUE;
	}

	if(hitTest.uHitCode == xtpCalendarHitTestDayViewScrollDown)
	{
		ScrollIcon_ScrollAllDayEventsDown();
		return TRUE;
	}

	return FALSE;
}

BOOL CXTPCalendarDayViewGroup::ScrollIcon_ScrollAllDayEventsUp()
{
	BOOL bHandledByDefault = FALSE;
	if(UserAction_OnScrollAllDayEvents(XTP_DAY_VIEW_GROUP_LAYOUT::xtpHotAllDayEventsScrollUp) == FALSE)
	{
		XTP_DAY_VIEW_LAYOUT& rDayLayout = GetViewDay()->GetView()->GetLayout();

		CXTPCalendarControl::CUpdateContext updateContext(GetCalendarControl(), xtpCalendarUpdateAll);
		if(rDayLayout.m_nTopAllDayRow > 0)
			rDayLayout.m_nTopAllDayRow--;

		bHandledByDefault = TRUE;
	}
	return bHandledByDefault;
}

BOOL CXTPCalendarDayViewGroup::ScrollIcon_ScrollAllDayEventsDown()
{
	BOOL bHandledByDefault = FALSE;
	if(UserAction_OnScrollAllDayEvents(XTP_DAY_VIEW_GROUP_LAYOUT::xtpHotAllDayEventsScrollDown) == FALSE)
	{
		XTP_DAY_VIEW_LAYOUT& rDayLayout = GetViewDay()->GetView()->GetLayout();

		CXTPCalendarControl::CUpdateContext updateContext(GetCalendarControl(), xtpCalendarUpdateAll);
		if(GetLastVisibleAllDayEvent() < GetEventsCountInAllDayArea())
			rDayLayout.m_nTopAllDayRow++;

		bHandledByDefault = TRUE;
	}
	return bHandledByDefault;
}

void CXTPCalendarDayViewGroup::OnMouseMove(UINT nFlags, CPoint point)
{
	if (!GetViewDay())
	{
		ASSERT(FALSE);
		return;
	}

	XTP_SAFE_CALL5(GetViewDay()->GetView(), GetTheme(), GetDayViewPart(), GetDayPart(),
		GetGroupPart(), OnMouseMove(this, nFlags, point));

	TBase::OnMouseMove(nFlags, point);
}

void CXTPCalendarDayViewGroup::AdjustDayEvents(CDC* pDC)
{
	ASSERT(pDC);

	int nViewEventsCount = GetViewEventsCount();
	if (!nViewEventsCount || !pDC)
	{
		return;
	}

	TViewEventsCollection arCellEViews;

	BOOL bRegion = FALSE;
	int nRegionMaxSize = 0;

	COleDateTime dtMinRegionTime;
	COleDateTime dtMaxRegionTime;

	int nCount = GetRowCount();
	for (int i = 0; i < nCount; i++)
	{
		COleDateTime dtMinEventTime, dtMaxEventTime;

		GetCellEvents(i, &arCellEViews, dtMinEventTime, dtMaxEventTime);
		int nEventsCount = arCellEViews.GetCount();

		BOOL bRegionEnded = FALSE;
		if (i == 0 || dtMinEventTime > dtMaxRegionTime ||
			CXTPCalendarUtils::IsEqual(dtMinEventTime, dtMaxRegionTime) )
		{
			bRegionEnded = bRegion;
			// save times for new region
			dtMinRegionTime = dtMinEventTime;
			dtMaxRegionTime = dtMaxEventTime;
		}
		else if (nEventsCount)
		{
			// continue region
			dtMinRegionTime = min(dtMinEventTime, dtMinRegionTime);
			dtMaxRegionTime = max(dtMaxEventTime, dtMaxRegionTime);
		}

		if (nEventsCount && !bRegion)
		{ //Region startted
			bRegion = TRUE;
			nRegionMaxSize = 0;
		}
		else if (bRegion && (nEventsCount == 0 || bRegionEnded))
		{ //Region ended
			_OnRegionEnded(pDC, nRegionMaxSize);

			bRegion = nEventsCount > 0;
			nRegionMaxSize = 0;
		}
		ASSERT(nEventsCount && bRegion || !bRegion && (!nEventsCount || bRegionEnded) );

		nRegionMaxSize = max(nRegionMaxSize, nEventsCount);

		CMap<int, int, BOOL, BOOL> mapBusuRegionPlaces;
		int nPlace = 0;

		//--make busy event places set ----------------------------------
		int nE;
		for (nE = 0; nE < nEventsCount; nE++)
		{
			CXTPCalendarDayViewEvent* pViewEvent = arCellEViews.GetAt(nE, FALSE);
			if (!pViewEvent)
			{
				ASSERT(FALSE);
				continue;
			}

			_AddInRegion_IfNeed(pViewEvent);

			int nEventPlace = pViewEvent->GetEventPlacePos();
			if (nEventPlace != XTP_EVENT_PLACE_POS_UNDEFINED)
			{
				mapBusuRegionPlaces.SetAt(nEventPlace, TRUE);
				nRegionMaxSize = max(nRegionMaxSize, nEventPlace + 1);
			}
		}

		//- assine places to new events for region ------------------------
		for (nE = 0; nE < nEventsCount; nE++)
		{
			CXTPCalendarDayViewEvent* pViewEvent = arCellEViews.GetAt(nE, FALSE);
			if (!pViewEvent)
			{
				ASSERT(FALSE);
				continue;
			}

			int nEventPlace = pViewEvent->GetEventPlacePos();
			if (nEventPlace == XTP_EVENT_PLACE_POS_UNDEFINED)
			{
				BOOL b;
				while (mapBusuRegionPlaces.Lookup(nPlace, b))
				{
					nPlace++;
				}
				pViewEvent->SetEventPlacePos(nPlace);

				if (nPlace >= pViewEvent->GetEventPlaceCount())
				{
					pViewEvent->SetEventPlaceCount(nPlace + 1);
				}

				mapBusuRegionPlaces.SetAt(nPlace, TRUE);
				nPlace++;

				nRegionMaxSize = max(nRegionMaxSize, nPlace);
			}
		}
	}

	if (bRegion)
	{
		_OnRegionEnded(pDC, nRegionMaxSize);
	}
}

void CXTPCalendarDayViewGroup::GetCellEvents(int nCell,
		TViewEventsCollection* pViewEvents,
		COleDateTime& rdtMinEventTime, COleDateTime& rdtMaxEventTime)
{
	if (!pViewEvents || !GetViewDay() || !GetViewDay()->GetView())
	{
		ASSERT(FALSE);
		return;
	}

	pViewEvents->RemoveAll();

	COleDateTime dtCellBegin = GetViewDay()->GetView()->GetCellTime(nCell);

	if ((DATE)GetViewDay()->GetView()->GetScaleMaxTime() > 1)
		dtCellBegin = GetViewDay()->GetDayDate() + dtCellBegin;
	else
		dtCellBegin = CXTPCalendarUtils::UpdateTime(GetViewDay()->GetDayDate(), dtCellBegin);

	COleDateTime dtCellEnd = GetViewDay()->GetView()->GetCellTime(nCell);
	dtCellEnd += GetViewDay()->GetView()->GetCellDuration();
	dtCellEnd -= XTP_HALF_SECOND * 2;
	dtCellEnd = CXTPCalendarUtils::UpdateTime(GetViewDay()->GetDayDate(), dtCellEnd);

	rdtMinEventTime = dtCellBegin;
	rdtMaxEventTime = dtCellEnd;

	int nEventsCount = GetViewEventsCount();
	for (int i = 0; i < nEventsCount; i++)
	{
		CXTPCalendarDayViewEvent* pViewEvent = GetViewEvent(i);

		if (!pViewEvent || !pViewEvent->GetEvent())
		{
			ASSERT(FALSE);
			continue;
		}

		if (pViewEvent->IsMultidayEvent())
		{
			continue;
		}

		COleDateTime dtS = pViewEvent->GetEvent()->GetStartTime();
		COleDateTime dtE = pViewEvent->GetEvent()->GetEndTime();

		if (!(dtE < dtCellBegin || dtS > dtCellEnd ||
			CXTPCalendarUtils::IsEqual(dtE, dtCellBegin) ||
			CXTPCalendarUtils::IsEqual(dtS, dtCellEnd)
			) ||
			CXTPCalendarUtils::IsEqual(dtS, dtE) &&
			( CXTPCalendarUtils::IsEqual(dtS, dtCellBegin) ||
			CXTPCalendarUtils::IsEqual(dtE, dtCellEnd)
			)
			)
		{
			pViewEvents->Add(pViewEvent, TRUE);

			if (dtS < rdtMinEventTime)
			{
				rdtMinEventTime = dtS;
			}
			if (dtE > rdtMaxEventTime)
			{
				rdtMaxEventTime = dtE;
			}
		}
	}
}

void CXTPCalendarDayViewGroup::_AddInRegion_IfNeed(CXTPCalendarDayViewEvent* pViewEvent)
{
	if (!pViewEvent)
	{
		ASSERT(FALSE);
		return;
	}

	int nRCount = m_arRegionViewEvents.GetCount();

	for (int j = 0; j < nRCount; j++)
	{
		CXTPCalendarDayViewEvent* pEV = m_arRegionViewEvents.GetAt(j);
		if (pViewEvent == pEV)
		{
			return;
		}
	}

	m_arRegionViewEvents.Add(pViewEvent, TRUE);

	pViewEvent->SetEventPlacePos(XTP_EVENT_PLACE_POS_UNDEFINED);
	pViewEvent->SetEventPlaceCount(0);
}

void CXTPCalendarDayViewGroup::_OnRegionEnded(CDC* pDC, int nRegionMaxSize)
{
	ASSERT(pDC);

	int nRCount = m_arRegionViewEvents.GetCount();

	for (int j = 0; j < nRCount; j++)
	{
		CXTPCalendarDayViewEvent* pViewEvent = m_arRegionViewEvents.GetAt(j, FALSE);
		if (!pViewEvent)
		{
			ASSERT(FALSE);
			continue;
		}

		ASSERT(pViewEvent->GetEventPlaceCount() <= nRegionMaxSize);

		if (nRegionMaxSize > pViewEvent->GetEventPlaceCount())
		{
			pViewEvent->SetEventPlaceCount(nRegionMaxSize);
		}

		int nPlaceNumber = pViewEvent->GetEventPlaceNumber();
		pViewEvent->AdjustLayout(pDC, m_LayoutX.m_rcDayDetails, nPlaceNumber);
	}
	m_arRegionViewEvents.RemoveAll();
}

COleDateTime CXTPCalendarDayViewGroup::GetCellDateTime(int nCell) const
{
	if (!GetViewDay() || !GetViewDay()->GetView())
	{
		return COleDateTime((DATE)0);
	}

	if ((DATE)GetViewDay()->GetView()->GetScaleMaxTime() > 1)
		return GetViewDay()->GetDayDate() + GetViewDay()->GetView()->GetCellTime(nCell);

	return CXTPCalendarUtils::UpdateTime(GetViewDay()->GetDayDate(), GetViewDay()->GetView()->GetCellTime(nCell));
}

int CXTPCalendarDayViewGroup::GetDateTimeCell(const COleDateTime& dtTime, BOOL bForEndTime) const
{
	if (!GetViewDay() || !GetViewDay()->GetView())
	{
		return 0;
	}

	COleDateTime dtTimeOnly = GetViewDay()->GetView()->GetDateTimeTime(dtTime);
	return GetViewDay()->GetView()->GetCellNumber(dtTimeOnly, bForEndTime);
}

BOOL CXTPCalendarDayViewGroup::IsWorkDateTime(const COleDateTime& dtDateTime) const
{
	if (!XTP_SAFE_GET1(GetViewDay(), GetCalendarControl(), NULL))
	{
		return FALSE;
	}

	COleDateTime dtStartW;
	COleDateTime dtEndW;

	GetViewDay()->GetCalendarControl()->GetWorkDayStartTime(dtStartW);
	dtStartW = (double)dtStartW - XTP_HALF_SECOND;

	GetViewDay()->GetCalendarControl()->GetWorkDayEndTime(dtEndW);
//<<>>
	if (dtEndW.GetStatus() != COleDateTime::valid)
	{
		dtEndW.SetTime(23, 59, 59);
		dtEndW.SetStatus(COleDateTime::valid);
		GetViewDay()->GetCalendarControl()->SetWorkDayEndTime(dtEndW);
	}
	else
		dtEndW -= COleDateTimeSpan(0, 0, 0, 1);

	int nWWMask = GetViewDay()->GetCalendarControl()->GetWorkWeekMask();

	int nDayOfWeek = dtDateTime.GetDayOfWeek();
	int nDayMask = CXTPCalendarUtils::GetDayOfWeekMask(nDayOfWeek);

	COleDateTime dtTime = CXTPCalendarUtils::ResetDate(dtDateTime);

	return (nWWMask & nDayMask) && dtTime >= dtStartW && dtTime <= dtEndW;
}

CRect CXTPCalendarDayViewGroup::GetCellRect(int nCell) const
{
	nCell -= GetTopRow();
	int nRowHeight = GetRowHeight();
	return CRect(m_Layout.m_rcGroup.left, m_LayoutX.m_rcDayDetails.top + nRowHeight * nCell, m_Layout.m_rcGroup.right, m_LayoutX.m_rcDayDetails.top + nRowHeight * (nCell + 1));
}

CXTPCalendarViewPart* CXTPCalendarDayViewGroup::GetCellViewPart(const COleDateTime& dtDateTime) const
{
	if (IsWorkDateTime(dtDateTime))
	{
		return (CXTPCalendarViewPart*)XTP_SAFE_GET3(GetViewDay(), GetView(),
			GetPaintManager(), GetDayViewWorkCellPart(), NULL);
	}
	return (CXTPCalendarViewPart*)XTP_SAFE_GET3(GetViewDay(), GetView(),
		GetPaintManager(), GetDayViewNonworkCellPart(), NULL);
}

int CXTPCalendarDayViewGroup::GetRowHeight() const
{
	return XTP_SAFE_GET2(GetViewDay(), GetView(), GetRowHeight(), 1);
}

int CXTPCalendarDayViewGroup::GetRowCount() const
{
	return XTP_SAFE_GET2(GetViewDay(), GetView(), m_LayoutX.m_nRowCount, 1);
}

int CXTPCalendarDayViewGroup::GetVisibleRowCount() const
{
	return XTP_SAFE_GET2(GetViewDay(), GetView(), m_LayoutX.m_nVisibleRowCount, 1);
}

int CXTPCalendarDayViewGroup::GetTopRow() const
{
	return XTP_SAFE_GET2(GetViewDay(), GetView(), m_LayoutX.m_nTopRow, 0);
}

int CXTPCalendarDayViewGroup::GetBusyStatus(COleDateTime dtTime) const
{
	int nRes = xtpCalendarBusyStatusUnknown;

	int nCount = GetViewEventsCount();
	for (int i = 0; i < nCount; i++)
	{
		CXTPCalendarEvent* pEvent = XTP_SAFE_GET1(GetViewEvent(i), GetEvent(), NULL);
		if (XTP_SAFE_GET1(GetViewEvent(i), IsMultidayEvent(), FALSE) && pEvent)
		{
			if (pEvent->IsAllDayEvent() ||
				(dtTime >= pEvent->GetStartTime() && dtTime <= pEvent->GetEndTime()))
			{
				int nEBs = pEvent->GetBusyStatus();
				nRes = max(nRes, nEBs);
			}
		}
	}

	return nRes;
}

void CXTPCalendarDayViewGroup::FindMinMaxGroupDayEvents(CXTPCalendarDayViewEvent*& rpMin, CXTPCalendarDayViewEvent*& rpMax) const
{
	rpMin = rpMax = NULL;

	COleDateTime dtMin = xtpCalendarDateTime_max;
	COleDateTime dtMax = xtpCalendarDateTime_min;

	int nCount = GetViewEventsCount();
	for (int i = 0; i < nCount; i++)
	{
		CXTPCalendarDayViewEvent* pEv = GetViewEvent(i);
		if (!pEv || !pEv->GetEvent() || pEv->IsMultidayEvent())
			continue;

		if (pEv->GetEvent()->GetStartTime() < dtMin)
		{
			dtMin = pEv->GetEvent()->GetStartTime();
			rpMin = pEv;
		}
		if (pEv->GetEvent()->GetStartTime() > dtMax)
		{
			dtMax = pEv->GetEvent()->GetStartTime();
			rpMax = pEv;
		}
	}
}

BOOL CXTPCalendarDayViewGroup::UserAction_OnScrollDay(XTPCalendarScrollDayButton eButton)
{
	XTP_CALENDAR_USERACTION_PARAMS uaParams;
	::ZeroMemory(&uaParams, sizeof(uaParams));

	uaParams.m_eAction = xtpCalendarUserAction_OnScrollDay;
	uaParams.m_ScrollDayButton.nButton = eButton;
	uaParams.m_ScrollDayButton.pViewGroup = this;

	BOOL bCancel = FALSE;

	XTP_SAFE_CALL2(GetViewDay(), GetCalendarControl(), SendNotification(
		XTP_NC_CALENDAR_USERACTION, (WPARAM)&uaParams, (LPARAM)&bCancel));

	return bCancel;
}

BOOL CXTPCalendarDayViewGroup::UserAction_OnScrollAllDayEvents(XTP_DAY_VIEW_GROUP_LAYOUT::XTPEnumHotItem eHotBtn)
{
	XTP_CALENDAR_USERACTION_PARAMS uaParams;
	::ZeroMemory(&uaParams, sizeof(uaParams));

	if(eHotBtn == XTP_DAY_VIEW_GROUP_LAYOUT::xtpHotAllDayEventsScrollUp)
		uaParams.m_eAction = xtpCalendarUserAction_OnScrollAllDayEvensUp;
	else
		uaParams.m_eAction = xtpCalendarUserAction_OnScrollAllDayEvensDown;


	BOOL bCancel = FALSE;
	GetCalendarControl()->SendNotification(XTP_NC_CALENDAR_USERACTION, (WPARAM)&uaParams, (LPARAM)&bCancel);

	return bCancel;
}

CRect CXTPCalendarDayViewGroup::GetTooltipRect(const CPoint& ptHit, const XTP_CALENDAR_HITTESTINFO& hitInfo) const
{
	UNREFERENCED_PARAMETER(ptHit);

	int nCell = GetDateTimeCell(hitInfo.dt, FALSE);
	CRect rcTip = GetCellRect(nCell);

	return rcTip;
}
