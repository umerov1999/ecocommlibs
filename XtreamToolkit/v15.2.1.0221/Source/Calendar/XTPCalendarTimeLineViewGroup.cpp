// XTPCalendarTimeLineViewGroup.cpp: implementation of the CXTPCalendarTimeLineViewGroup class.
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

#include "XTPCalendarDefines.h"

#include "XTPCalendarEvent.h"
#include "XTPCalendarEvents.h"


#include "XTPCalendarPtrCollectionT.h"
#include "XTPCalendarOptions.h"

#include "XTPCalendarView.h"
#include "XTPCalendarViewDay.h"
#include "XTPCalendarViewEvent.h"
#include "XTPCalendarViewPart.h"

#include "XTPCalendarControl.h"

#include "XTPCalendarTimeLineViewTimeScale.h"
#include "XTPCalendarTimeLineViewGroup.h"
#include "XTPCalendarTimeLineView.h"

#include "XTPCalendarPaintManager.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////
CXTPCalendarTimeLineViewGroup::CXTPCalendarTimeLineViewGroup(CXTPCalendarTimeLineView* pView,
		CXTPCalendarTimeLineViewGroup* pParentGroup)
	: CXTPCalendarViewGroup(NULL)
{
	ASSERT(pView);
	m_pView = pView;
	m_pParentGroup = pParentGroup;
	m_pChildren = NULL;
}

CXTPCalendarTimeLineViewGroup::~CXTPCalendarTimeLineViewGroup()
{
	CMDTARGET_RELEASE(m_pChildren);
}

void CXTPCalendarTimeLineViewGroup::AdjustLayout(CDC* pDC, const CRect& rcGroup)
{
	m_Layout.m_rcGroup = rcGroup;

	int i;
	int nCount = m_arEvents.GetCount();
	// build initial linear order - if draw - looks like Outlook EventList
	for (i = 0; i < nCount; i++)
	{
		CXTPCalendarTimeLineViewEvent* pViewEv = GetViewEvent(i);
		if (pViewEv)
		{
			pViewEv->CalcRect(pDC);
			pViewEv->ChangeEventPlace(i);
		}
	}

	BOOL bCompact = TRUE;
	CXTPCalendarOptions* pOptions = GetCalendarControl()->GetCalendarOptions();
	if (pOptions)
		bCompact = pOptions->bTimeLineCompact;

	int nMaxY = GetRect().top;
	int R(0), L(0);
	CUIntArray ar;//candidates for moving - each item move maximum once!
	for (i = 0; i < nCount; i++)
		ar.Add(i);
	int nAr = (int)ar.GetSize();
	for (i = 0; i < nCount; i++)
	{
		CXTPCalendarTimeLineViewEvent* pViewEv = GetViewEvent(i);
		if (i > 0 && ar.GetAt(i) == 0) continue; //already moved
		CRect rc1 = pViewEv->GetEventRect();
		R = rc1.right;

		int IX1 = pViewEv->GetEventTimelinePlaceNumber();
		for (int m = 1; m < nAr; m++)
		{
			UINT j = ar.GetAt(m);
			if (j == 0) continue;
			CXTPCalendarTimeLineViewEvent* pViewEv2 = GetViewEvent(j);
			if (pViewEv2)
			{
				CRect rc2 = pViewEv2->GetEventRect();
				L = rc2.left;
				if (R < L)
				{
					if (bCompact)
					{
						ar.SetAt(m, 0);//j-th event already served
						pViewEv2->ChangeEventPlace(IX1);
//----------------------------------------------------------
// https://forum.codejock.com/forum_posts.asp?TID=13465&PID=46020#46020
						for (int mm = m + 1; mm < nAr; mm++)
						{
							UINT jj = ar.GetAt(mm);
							if (jj == 0) continue;
							CXTPCalendarTimeLineViewEvent* pViewEv3 = GetViewEvent(jj);
							int IX3 = pViewEv3->GetEventTimelinePlaceNumber();
							if (pViewEv3)
								pViewEv3->ChangeEventPlace(IX3 - 1);
						}
//----------------------------------------------------------
					}
					R = rc2.right;
				}
			}
		}

		nMaxY = max(nMaxY, rc1.bottom);
	}

	m_Layout.m_rcGroup.bottom = nMaxY + 1;
}

int CXTPCalendarTimeLineViewGroup::GetGroupHeight()
{
	return m_Layout.m_rcGroup.bottom;
}

CXTPCalendarViewEvent* CXTPCalendarTimeLineViewGroup::CreateViewEvent(CXTPCalendarEvent* pEvent)
{
	return new CXTPCalendarTimeLineViewEvent(pEvent, this);
}

void CXTPCalendarTimeLineViewGroup::Populate(CXTPCalendarEvents* pEvents)
{
	m_arEvents.RemoveAll();

	int nCount = pEvents ? pEvents->GetCount() : 0;
	for (int i = 0; i < nCount; i++)
	{
		CXTPCalendarEvent* pEv = pEvents->GetAt(i);

		CXTPCalendarTimeLineViewEvent* pViewEv = (CXTPCalendarTimeLineViewEvent*)CreateViewEvent(pEv);

		m_arEvents.Add(pViewEv);
	}
}

void CXTPCalendarTimeLineViewGroup::Draw(CDC* pDC, int nOffset)
{
	CRect rc = GetRect();
	rc.top += nOffset;
	rc.bottom += nOffset;
	XTP_SAFE_CALL3(GetView(), GetPaintManager(), GetTimeLineViewPart(), DrawGroup(pDC, rc, this));
}

CXTPCalendarControl* CXTPCalendarTimeLineViewGroup::GetCalendarControl() const
{
	if (m_pView)
		return m_pView->GetCalendarControl();
	return NULL;
}

//////////////////////////////////////////////////////////////////////
CXTPCalendarTimeLineViewEvent::CXTPCalendarTimeLineViewEvent(CXTPCalendarEvent* pEvent, CXTPCalendarTimeLineViewGroup* pGroup) :
	CXTPCalendarViewEvent(pEvent, NULL)
{
	ASSERT(pEvent && pGroup);

	m_ptrEvent = pEvent;
	m_pGroup = pGroup;
	m_nEventDurationInPixels = 0;
	m_nEventTimelinePlaceNumber = 0;
}

CXTPCalendarTimeLineView* CXTPCalendarTimeLineViewEvent::GetView() const
{
	return GetViewGroup()->GetView();
}

void CXTPCalendarTimeLineViewEvent::AdjustLayout(CDC* pDC, const CRect& rcEventMax, int nEventPlaceNumber)
{
	CalcRect(pDC);
}

CXTPCalendarTimeLineViewPart* CXTPCalendarTimeLineViewEvent::GetPart() const
{
	return XTP_SAFE_GET2(GetView(), GetPaintManager(), GetTimeLineViewPart(), NULL);
}

void CXTPCalendarTimeLineViewEvent::CalcRect(CDC* pDC)
{
	CRect rcView = GetView()->GetViewRect();
	CRect rcClient = GetViewGroup()->GetRect();
	rcClient.left = rcView.left;
	rcClient.top += 2;

	COleDateTime dtStartView = GetView()->GetStartViewDate();
	COleDateTimeSpan spTimePerPix = GetView()->GetTimeScale()->GetTimeScaleParams()->GetTimePerPixel();

	COleDateTime dtStart = GetEvent()->GetStartTime();
	COleDateTime dtEnd = GetEvent()->GetEndTime();

	COleDateTimeSpan spStartX = dtStart - dtStartView;
	COleDateTimeSpan spEndX = dtEnd - dtStartView;

	double dX1 = (double)spStartX / (double)spTimePerPix;
	double dX2 = (double)spEndX / (double)spTimePerPix;

	CSize szEvent = GetPart()->CalcEventSize(pDC, this);
	(CRect&)m_rcEvent = rcClient;

	m_rcEvent.left = rcClient.left + (int)dX1;
	m_rcEvent.right = rcClient.left + (int)dX2;

	//-------------------------------------------------------------
	m_nEventDurationInPixels = max(0, m_rcEvent.Width());

	m_rcEvent.right = max(m_rcEvent.right + 2, m_rcEvent.left + szEvent.cx);

	m_rcEvent.bottom = m_rcEvent.top + szEvent.cy;

	m_nEventPlaceNumber = 0; //reset
	m_nEventTimelinePlaceNumber = 0;//reset
}

void CXTPCalendarTimeLineViewEvent::ChangeEventPlace(int nNewPlace)
{
	CRect rcClient = GetViewGroup()->GetRect();
	rcClient.top += 2;

	int nEventHeight = 26;

	int nEventsTop = rcClient.top;

	m_rcEvent.top = nEventsTop + nNewPlace * nEventHeight;
	m_rcEvent.bottom = m_rcEvent.top + nEventHeight;

#ifdef _DEBUG
	if (m_nEventPlaceNumber == 0)
		m_nEventPlaceNumber = nNewPlace;
	//set to see the order in Debug session on the top of Event's Icon
#endif

	m_nEventTimelinePlaceNumber = nNewPlace;//set
}

void CXTPCalendarTimeLineViewEvent::Draw(CDC* pDC)
{
}

BOOL CXTPCalendarTimeLineViewEvent::IsSelected() const
{
	CXTPCalendarViewEvents* pSelEvents = XTP_SAFE_GET2(GetViewGroup(), GetView(), GetSelectedViewEvents(), NULL);
	if (!pSelEvents)
	{
		return FALSE;
	}

	int nFIndex = pSelEvents->Find(m_ptrEvent);
	return nFIndex >= 0;
}
