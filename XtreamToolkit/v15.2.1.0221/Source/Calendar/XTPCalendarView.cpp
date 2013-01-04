// XTPCalendarView.cpp : implementation file
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

#include "Common/XTPPropExchange.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPSmartPtrInternalT.h"
#include "Common/XTPNotifyConnection.h"
#include "Common/XTPColorManager.h"

#include "XTPCalendarDefines.h"
#include "XTPCalendarUtils.h"
#include "XTPCalendarPtrCollectionT.h"
#include "XTPCalendarNotifications.h"
#include "XTPCalendarOptions.h"

#include "XTPCalendarTimeZoneHelper.h"
#include "XTPCalendarResource.h"

#include "XTPCalendarEvent.h"
#include "XTPCalendarEvents.h"

#include "XTPCalendarView.h"
#include "XTPCalendarViewEvent.h"
#include "XTPCalendarViewDay.h"
#include "XTPCalendarTimeLineView.h"
#include "XTPCalendarViewPart.h"

#include "XTPCalendarControl.h"
#include "XTPCalendarData.h"

#include "XTPCalendarPaintManager.h"
#include "XTPCalendarTheme.h"
#include "XTPCalendarThemeOffice2007.h"

#include <math.h>

#pragma warning(disable: 4571) // warning C4571: catch(...) blocks compiled with /EHs do not catch or re-throw Structured Exceptions

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define XTP_CLIPBOARD_EVENTS_DATA_VER 2




class CXTPCalendarView::CUndoBuffer
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Default class constructor.
	//-----------------------------------------------------------------------
	CUndoBuffer() {};

	//-----------------------------------------------------------------------
	// Summary:
	//     Default class destructor.
	//-----------------------------------------------------------------------
	virtual ~CUndoBuffer()
	{
		RemoveAll();
	};

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to add an Undo action to the buffer.
	// Parameters:
	//     pPrev - A CXTPCalendarEvent object that contains the instance
	//             before the edit action.
	//     pNew  - A CXTPCalendarEvent that contains the instance after
	//             the edit action.
	//-----------------------------------------------------------------------
	void AddUndoAction(CXTPCalendarEvent* pPrev, CXTPCalendarEvent* pNew)
	{
		CXTPCalendarEventPtr ptrPrevCopy = pPrev ? pPrev->CloneEvent() : NULL;
		CXTPCalendarEventPtr ptrNewCopy = pNew ? pNew->CloneEvent() : NULL;
		m_arUndoActions.Add(SUndoAction(ptrPrevCopy, ptrNewCopy));
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to clear the Undo buffer.
	//-----------------------------------------------------------------------
	void RemoveAll()
	{
		m_arUndoActions.RemoveAll();
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to search for an undo action for a
	//     specified event in the buffer.
	// Parameters:
	//     pEvent - A CXTPCalendarEvent object to search for in the buffer.
	// Returns:
	//     A BOOL. TRUE if undo action for a specified event exists in the
	//     buffer. FALSE otherwise.
	//-----------------------------------------------------------------------
	BOOL IsInUndoBuffer(CXTPCalendarEvent* pEvent)
	{
		int nCount = (int)m_arUndoActions.GetSize();
		for (int i = nCount-1; i >= 0; i--)
		{
			SUndoAction& rAction = m_arUndoActions.ElementAt(i);
			if (rAction.ptrPrev && rAction.ptrPrev->IsEqualIDs(pEvent) ||
				rAction.ptrNew && rAction.ptrNew->IsEqualIDs(pEvent))
			{
				return TRUE;
			}
		}
		return FALSE;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to obtain the number of undo actions.
	// Returns:
	//     An int that contains the number of Undo actions.
	//-----------------------------------------------------------------------
	int GetCount() const
	{
		return (int)m_arUndoActions.GetSize();
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to obtain the first undo event.
	// Returns:
	//     A CXTPCalendarEvent object that contains the first undo event
	//     or NULL if there are no undo events.
	//-----------------------------------------------------------------------
	CXTPCalendarEventPtr GetFirstUndoEvent()
	{
		int nCount = (int)m_arUndoActions.GetSize();
		if (nCount > 0)
		{
			SUndoAction& rAction = m_arUndoActions.ElementAt(0);
			if (rAction.ptrPrev)
			{
				return rAction.ptrPrev;
			}
			else if (rAction.ptrNew)
			{
				return rAction.ptrNew;
			}
		}
		return NULL;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to search for the first action for
	//     a specified event in the buffer.
	// Parameters:
	//     pEvent - A CXTPCalendarEvent object that contains the event to
	//              search for in the buffer.
	// Returns:
	//     A CXTPCalendarEvent that contains the oldest event object from
	//     all of the editing actions or NULL if there are no events.
	//-----------------------------------------------------------------------
	CXTPCalendarEventPtr UndoAllForEvent(CXTPCalendarEvent* pEvent)
	{
		int nCount = (int)m_arUndoActions.GetSize();
		for (int i = 0; i < nCount; i++)
		{
			SUndoAction& rAction = m_arUndoActions.ElementAt(i);
			if (rAction.ptrPrev && rAction.ptrPrev->IsEqualIDs(pEvent) ||
				rAction.ptrNew && rAction.ptrNew->IsEqualIDs(pEvent))
			{
				return rAction.ptrPrev;
			}
		}
		return NULL;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to search for last action for a
	//     specified event in the buffer.
	// Parameters:
	//     pEvent - A CXTPCalendarEvent object that contains the event
	//              to search for in the buffer.
	// Returns:
	//     A CXTPCalendarEvent that contains the latest event object from
	//     all of the editing actions or NULL if there are no events.
	//-----------------------------------------------------------------------
	CXTPCalendarEventPtr RedoAllForEvent(CXTPCalendarEvent* pEvent)
	{
		CXTPCalendarEventPtr ptrResult;
		int nCount = (int)m_arUndoActions.GetSize();
		for (int i = nCount-1; i >= 0; i--)
		{
			SUndoAction& rAction = m_arUndoActions.ElementAt(i);
			if (rAction.ptrPrev && rAction.ptrPrev->IsEqualIDs(pEvent) ||
				rAction.ptrNew && rAction.ptrNew->IsEqualIDs(pEvent))
			{
				return rAction.ptrNew;
			}
		}
		return NULL;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to clear the current buffer of all
	//     undo actions and then add new undo actions to the buffer from
	//     the source object.
	// Parameters:
	//     rSrc - A CUndoBuffer object that contains the new undo actions
	//            to add to the undo buffer object.
	//-----------------------------------------------------------------------
	void Set(CUndoBuffer& rSrc)
	{
		m_arUndoActions.RemoveAll();
		m_arUndoActions.Append(rSrc.m_arUndoActions);
	}
protected:
	//-----------------------------------------------------------------------
	// Remarks:
	//     Helper structure to store editing/undo actions.
	//-----------------------------------------------------------------------
	struct SUndoAction
	{
		CXTPCalendarEventPtr ptrPrev; // Event instance before edit action.
		CXTPCalendarEventPtr ptrNew;  // Event instance after edit action.

		//-----------------------------------------------------------------------
		// Summary:
		//     Default class constructor.
		// Parameters:
		//     pPrev - A CXTPCalendarEvent object that contains the instance
		//             before the edit action.
		//     pNew  - A CXTPCalendarEvent object that contains the instance
		//             after the edit action.
		//-----------------------------------------------------------------------
		SUndoAction(CXTPCalendarEvent* pPrev = NULL, CXTPCalendarEvent* pNew = NULL) :
		ptrPrev(pPrev, TRUE), ptrNew(pNew, TRUE) {};

		//-----------------------------------------------------------------------
		// Parameters:
		//     src - A SUndoAction object that contains the source object.
		// Summary:
		//     Copy class constructor.
		//-----------------------------------------------------------------------
		SUndoAction(const SUndoAction& src)
		{
			*this = src;
		}

		//-----------------------------------------------------------------------
		// Summary:
		//     Default class destructor.
		//-----------------------------------------------------------------------
		virtual ~SUndoAction() {}

		//-----------------------------------------------------------------------
		// Summary:
		//     This is the overloaded assignment operator.
		// Parameters:
		//     src - Source object.
		// Returns:
		//     A reference to the current object.
		// Remarks:
		//     Copy class operator.
		//-----------------------------------------------------------------------
		const SUndoAction& operator=(const SUndoAction& src)
		{
			ptrPrev = src.ptrPrev;
			ptrNew = src.ptrNew;
			return *this;
		}
	};

	CArray<SUndoAction, const SUndoAction&> m_arUndoActions; // Array of editing/undo actions.
	};




//===========================================================================
IMPLEMENT_DYNAMIC(CXTPCalendarWMHandler, CCmdTarget)
IMPLEMENT_DYNAMIC(CXTPCalendarView, CXTPCalendarWMHandler)

/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CXTPCalendarView

CXTPCalendarView::CXTPCalendarView(CXTPCalendarControl* pCalendarControl) :
		m_pControl(pCalendarControl)
{
	//m_strDayHeaderFormatLong = _T("d MMMM");
	//m_strDayHeaderFormatMiddle = _T("d MMM");
	//m_strDayHeaderFormatShort = _T("d");
	//m_strDayHeaderFormatShortest = _T("d");

	m_rcView.SetRectEmpty();
	m_rcTooltip.SetRectEmpty();

	m_Layout.m_nRowHeight = 18;

	m_eDraggingMode = xtpCalendaDragModeUnknown;
	m_ptStartDragging.x = m_ptStartDragging.y = LONG_MAX;
	m_bStartedClickInside = FALSE;

	m_nTimerID_StartEditSubject = 0;
	m_nTimerID_ShowToolTip = 0;

	m_eUndoMode = xtpCalendarUndoModeUnknown;
	m_bResetUndoBuffer = FALSE;

	m_nKeyStateTimerID = 0;

	m_pSelectedEvents = new CXTPCalendarViewEvents();
	m_pResources = NULL;


	m_pUndoBuffer = new CUndoBuffer();
	m_pPrevUndoBuffer = new CUndoBuffer();

	_CalculateEventTimeFormat();

	m_bScrollV_Disabled = FALSE;
	m_bScrollH_Disabled = FALSE;
	ClearDiscreteSelection();

	m_ptLBtnDownMousePos = CPoint(0,0);
	m_ptLBtnUpMousePos = CPoint(0,0);

}

CXTPCalendarView::~CXTPCalendarView()
{
	CMDTARGET_RELEASE(m_pSelectedEvents);
	CMDTARGET_RELEASE(m_pResources);
	ASSERT(m_nTimerID_StartEditSubject == 0);
	ASSERT(m_nTimerID_ShowToolTip == 0);

	ClearDiscreteSelection();


	SAFE_DELETE(m_pUndoBuffer);
	SAFE_DELETE(m_pPrevUndoBuffer);
}

void CXTPCalendarView::OnBeforeDestroy()
{
	// --- Kill Subject editor
	if (IsEditingSubject())
		EndEditSubject(xtpCalendarEditSubjectCommit, FALSE);

	if (m_ptrDraggingEventNew)
		CommitDraggingEvent();

	// --- Kill tooltip (and timer inside)
	ShowToolTip(FALSE);

	// --- Clear selected events collection
	if (m_pSelectedEvents)
		m_pSelectedEvents->RemoveAll();

	// === Call base cleanup code
	TBase::OnBeforeDestroy();
}

void CXTPCalendarView::OnActivateView(BOOL bActivate,
										CXTPCalendarView* pActivateView,
										CXTPCalendarView* pInactiveView)
{
	UNREFERENCED_PARAMETER(pActivateView);
	UNREFERENCED_PARAMETER(pInactiveView);

	if (!bActivate && IsEditingSubject())
		EndEditSubject(xtpCalendarEditSubjectCommit, FALSE);
}

void CXTPCalendarView::RedrawControl()
{
	ASSERT(m_pControl);
	if (m_pControl)
	{
		m_pControl->RedrawControl();
	}
}

void CXTPCalendarView::Populate()
{
	if (IsEditingSubject())
	{
		EndEditSubject(xtpCalendarEditSubjectCommit);
	}

	int nCount = GetViewDayCount();
	for (int i = 0; i < nCount; i++)
	{
		CXTPCalendarViewDay* pViewDay = GetViewDay_(i);
		COleDateTime dtDay = GetViewDayDate(i);
		//ASSERT(pViewDay);
		if (pViewDay)
			pViewDay->Populate(dtDay);
	}

	_ReSelectSelectEvents();
}

BOOL CXTPCalendarView::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_ptLBtnDownMousePos = point;

	CXTPCalendarControl::CUpdateContext updateContext(m_pControl, xtpCalendarUpdateRedrawIfNeed);
	CSelectionChangedContext selChanged(this);

	if (!m_pControl->IsReadonlyMode())
		m_bStartedClickInside = TRUE;

	//---------------------------------------------------------------------------
	TBase::OnLButtonDown(nFlags, point);

	//= kill timeout request to edit subject=====================================
	if (m_nTimerID_StartEditSubject)
	{
		KillTimer(m_nTimerID_StartEditSubject);
		m_nTimerID_StartEditSubject = 0;
		m_ptrEditingViewEvent = NULL;
	}

	//---------------------------------------------------------------------------
	XTP_CALENDAR_HITTESTINFO hitInfo;
	if (HitTest(point, &hitInfo))
	{
		if (hitInfo.uHitCode & xtpCalendarHitTestDay_Mask)
		{
			CXTPCalendarControl::CUpdateContext updateContext1(m_pControl, xtpCalendarUpdateRedraw);
			ProcessDaySelection(&hitInfo, nFlags);
		}
		else if (hitInfo.uHitCode & xtpCalendarHitTestEvent_Mask && !m_pControl->IsReadonlyMode())
		{
			CXTPCalendarControl::CUpdateContext updateContext1(m_pControl, xtpCalendarUpdateRedraw);
			ProcessEventSelection(&hitInfo, nFlags);

			// set request to start dragging when OnMouseMove()
			m_ptStartDragging = point;
		}
	}

	//---------------------------------------------------------------------------
	BOOL bResetUndoLocal = TRUE;
	if (IsEditingSubject() && !m_pControl->IsReadonlyMode())
	{
		if (!GetSubjectEditorRect().PtInRect(point))
		{
			bResetUndoLocal = m_eDraggingMode != xtpCalendaDragModeEditSubject;
			EndEditSubject(xtpCalendarEditSubjectCommit);
		}
	}
	m_bResetUndoBuffer = bResetUndoLocal;

	//---------------------------------------------------------------------------
	CXTPCalendarThemeOffice2007* pThemeX = DYNAMIC_DOWNCAST(CXTPCalendarThemeOffice2007, GetTheme());
	if (pThemeX)
	{
		XTP_SAFE_CALL1(pThemeX->GetPrevNextEventButtonsPartX(), OnLButtonDown(this, nFlags, point));
	}

	return TRUE;
}

BOOL CXTPCalendarView::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_ptLBtnUpMousePos = m_ptLBtnDownMousePos;

	m_ptLBtnDownMousePos = CPoint(0, 0);

	// way to support discrete selection - pick all days while ALT key hold
	XTP_CALENDAR_HITTESTINFO hitAltInfo;
	BOOL bAltHitTest = HitTest(point, &hitAltInfo);
	BOOL bVKAlt = (GetKeyState(VK_MENU)  < 0);

	if (bAltHitTest && bVKAlt && nFlags == 0 && m_bStartedClickInside)
	{
		if (hitAltInfo.dt != NULL)
		{
			UINT uDay = (UINT) hitAltInfo.dt;
			AddToDiscreteSelection(uDay);
			return TRUE;
		}
	}
	else
	{
		ClearDiscreteSelection();
	}

	CXTPCalendarControl::CUpdateContext updateContext(m_pControl, xtpCalendarUpdateRedrawIfNeed);
	CSelectionChangedContext selChanged(this);

	TBase::OnLButtonUp(nFlags, point);

	//---------------------------------------------------------------------------
	m_ptStartDragging.x = m_ptStartDragging.y = LONG_MAX;

	XTP_CALENDAR_HITTESTINFO hitInfo;
	BOOL bHitTest = HitTest(point, &hitInfo);

	if (GetViewType() == xtpCalendarTimeLineView)
	{
		CXTPCalendarTimeLineView* pView = (CXTPCalendarTimeLineView* ) GetCalendarControl()->GetActiveView();
		if (pView)
			pView->ShowCalendarPopup(hitInfo.dt, point);
	}
	int nDragMode = m_eDraggingMode;

	if (!OnEndDragging(point, &hitInfo) && !m_pControl->IsReadonlyMode())
	{
		if (bHitTest)
		{
			UINT uESmask = xtpCalendarHitTestEventTextArea | xtpCalendarHitTestEventResizeArea_Mask;
			if ((hitInfo.uHitCode & uESmask) && !IsEditingSubject() &&
				hitInfo.pViewEvent && hitInfo.pViewEvent->IsSelected())
			{
				BOOL bEnabled = XTP_SAFE_GET1(GetCalendarOptions(), bEnableInPlaceEditEventSubject_ByMouseClick, FALSE);
				bEnabled = bEnabled && IsSingleEventSelected();
				if (bEnabled)
					bEnabled = !OnBeforeEditOperationNotify(xtpCalendarEO_EditSubject_ByMouseClick, hitInfo.pViewEvent);

				if (bEnabled)
				{
					ASSERT(!m_ptrEditingViewEvent);

					m_ptrEditingViewEvent = hitInfo.pViewEvent;
					m_ptrEditingViewEvent->InternalAddRef();

					if (m_nTimerID_StartEditSubject == 0)
					{
						UINT uTimeOut_ms = GetStartEditSubjectTimeOut();
						m_nTimerID_StartEditSubject = SetTimer(uTimeOut_ms);
					}
				}
			}
		}
	}

	//---------------------------------------------------------------------------
	if (_IsDragModeCopyMove(nDragMode) && bHitTest && hitInfo.pViewDay)
	{
		UnselectAllEvents();
		SelectDay(hitInfo.pViewDay, hitInfo.nGroup);
	}

	if (GetCalendarControl())
		GetCalendarControl()->m_mouseMode = xtpCalendarMouseNothing;

	m_bStartedClickInside = FALSE;

	OnMouseMove(nFlags, point);

	return TRUE;
}

void CXTPCalendarView::OnMouseMove(UINT nFlags, CPoint point)
{
	CXTPCalendarControl::CUpdateContext updateContext(m_pControl, xtpCalendarUpdateRedrawIfNeed);
	CSelectionChangedContext selChanged(this);

	CXTPCalendarThemeOffice2007* pThemeX = DYNAMIC_DOWNCAST(CXTPCalendarThemeOffice2007, GetTheme());
	if (pThemeX)
		XTP_SAFE_CALL1(pThemeX->GetPrevNextEventButtonsPartX(), OnMouseMove(this, nFlags, point));

	TBase::OnMouseMove(nFlags, point);

	XTP_CALENDAR_HITTESTINFO hitInfo;
	BOOL bHitTest = HitTest(point, &hitInfo);

	if (bHitTest && hitInfo != m_LastHitInfo)
	{
		m_LastHitInfo = hitInfo;

		BOOL bVKAlt = (GetKeyState(VK_MENU)  < 0);
		if (bVKAlt && (nFlags & MK_LBUTTON && m_bStartedClickInside))
		{
//Preventing continues selection if ALT key hold
			if (m_LastHitInfo.dt != NULL)
			{
				UINT uDay = (UINT) m_LastHitInfo.dt;
				AddToDiscreteSelection(uDay);
			}
		}
		else if (nFlags & MK_LBUTTON && m_bStartedClickInside)
		{
			const int cEpsilon = 2;
			if (m_ptStartDragging.x == LONG_MAX || m_ptStartDragging.y == LONG_MAX ||
					abs(point.x - m_ptStartDragging.x) >= cEpsilon ||
					abs(point.y - m_ptStartDragging.y) >= cEpsilon)
			{
				if (!m_ptrDraggingEventNew && !m_ptrDraggingEventOrig &&
						m_eDraggingMode == xtpCalendaDragModeUnknown)
				{
					XTP_CALENDAR_HITTESTINFO hitInfoSD;
					if (HitTest(m_ptStartDragging, &hitInfoSD))
					{
						if (hitInfoSD.uHitCode & xtpCalendarHitTestEvent_Mask)
						{
							int eDragMode = _GetDraggingMode(&hitInfoSD);
							if (!IsEditOperationDisabledNotify(_DragMod2Operation(eDragMode), hitInfoSD.pViewEvent))
								OnStartDragging(m_ptStartDragging, &hitInfoSD);
						}
					}
				}
				m_ptStartDragging.x = m_ptStartDragging.y = LONG_MAX;

				BOOL bDragging = m_ptrDraggingEventNew && m_eDraggingMode != xtpCalendaDragModeEditSubject;

				if (bDragging)
				{
					CXTPCalendarEventPtr ptrDragEventPrev = m_ptrDraggingEventNew->CloneEvent();
					BOOL bDragged = OnDragging(point, &hitInfo);

					if (bDragged)
					{
						XTP_EVENT_DRAG_OPERATION_PARAMS opParamsDrag;
						::ZeroMemory(&opParamsDrag, sizeof(opParamsDrag));

						opParamsDrag.eOperation = _DragMod2Operation(m_eDraggingMode);
						opParamsDrag.pDraggingEventOriginal = m_ptrDraggingEventOrig;
						opParamsDrag.pDraggingEvent = ptrDragEventPrev;
						opParamsDrag.pDraggingEventNew = m_ptrDraggingEventNew;

						BOOL bHandled = FALSE;
						if (m_pControl)
							m_pControl->SendNotification(XTP_NC_CALENDAR_BEFORE_EVENT_EDIT_OPERATION,
								(WPARAM)&opParamsDrag, (LPARAM)&bHandled);

						if (bHandled) //rollback
							m_ptrDraggingEventNew = ptrDragEventPrev;

						XTP_SAFE_CALL1(GetCalendarControl(), Populate());

						CXTPCalendarControl::CUpdateContext updateContext1(m_pControl, xtpCalendarUpdateLayout | xtpCalendarUpdateRedraw);

						if (m_ptrDraggingEventNew
							&& m_ptrDraggingEventOrig
							&& (!m_ptrDraggingEventNew->IsEqualStartEnd(m_ptrDraggingEventOrig) ||
							m_ptrDraggingEventNew->GetScheduleID() != m_ptrDraggingEventOrig->GetScheduleID()))
								SelectEvent(m_ptrDraggingEventNew);
					}
				}
			}

			if (m_eDraggingMode == xtpCalendaDragModeUnknown &&
				hitInfo.uHitCode & (xtpCalendarHitTestDay_Mask | xtpCalendarHitTestEvent_Mask))

			{
				if ((hitInfo.uHitCode & xtpCalendarHitTestEvent_Mask) &&
					hitInfo.pViewEvent && hitInfo.pViewEvent->IsSelected())
				{}
				else
				{
					CXTPCalendarControl::CUpdateContext updateContext1(m_pControl, xtpCalendarUpdateRedraw);

					ProcessDaySelection(&hitInfo, nFlags | MK_SHIFT);
				}
			}
		}
		// process tooltip events (only when no buttons pressed)
		if (0 == nFlags)
		{
			CRect rcTooltip(GetTooltipRect(point, hitInfo));
			if (m_rcTooltip != rcTooltip)
			{
				// -- kill waiting and hide a tooltip because we switched to another cell
				ShowToolTip(FALSE);

				// -- start waiting for mouse under regular empty cell
				//    (to show later "click to add appointment" button)
				UINT uTimeOut_ms = GetShowToolTipTimeOut();
				m_nTimerID_ShowToolTip = SetTimer(uTimeOut_ms);
				m_rcTooltip = rcTooltip;
				ASSERT(m_nTimerID_ShowToolTip);
			}
		}
	}
	else if (!bHitTest)
	{
		XTP_CALENDAR_HITTESTINFO hitInfoEmpty;
		m_LastHitInfo = hitInfoEmpty;
	}

	//===========================================================================
	if (GetCalendarControl())
	{
		if (_IsDragModeCopyMove(m_eDraggingMode)
			&& GetCalendarControl()->m_mouseMode != xtpCalendarMouseEventDraggingOut)
		{
			GetCalendarControl()->m_mouseMode = m_eDraggingMode == xtpCalendaDragModeCopy ?
					xtpCalendarMouseEventDragCopy : xtpCalendarMouseEventDragMove;

			GetCalendarControl()->UpdateMouseCursor();
		}
	}
}

void CXTPCalendarView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CXTPCalendarControl::CUpdateContext updateContext(m_pControl, xtpCalendarUpdateRedrawIfNeed);
	CSelectionChangedContext selChanged(this);

	TBase::OnChar(nChar, nRepCnt, nFlags);

	BOOL bEnable = XTP_SAFE_GET1(GetCalendarOptions(), bEnableInPlaceCreateEvent, FALSE);
	if (nChar >= _T(' ') && bEnable && !m_pControl->IsReadonlyMode())
	{
		CString strInitialSubj((TCHAR)nChar);
		if (!OnInPlaceCreateEvent(strInitialSubj))
			StartEditNewEventInplace(strInitialSubj);
	}
}

void CXTPCalendarView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CXTPCalendarControl::CUpdateContext updateContext(m_pControl, xtpCalendarUpdateRedrawIfNeed);
	CSelectionChangedContext selChanged(this);

	TBase::OnKeyDown(nChar, nRepCnt, nFlags);

	if (nChar == VK_RETURN || nChar == VK_ESCAPE)
	{
		if (IsEditingSubject())
		{
			CXTPCalendarEventPtr ptrEditingEvent(m_ptrDraggingEventOrig, TRUE);

			EndEditSubject((nChar == VK_RETURN) ? xtpCalendarEditSubjectCommit : xtpCalendarEditSubjectCancel);

			if (nChar == VK_ESCAPE && ptrEditingEvent)
			{
				_Undo(ptrEditingEvent);
				m_pUndoBuffer->Set(*m_pPrevUndoBuffer);
			}
		}

		if (nChar == VK_ESCAPE && m_ptrDraggingEventNew)
		{
			CXTPCalendarEventPtr ptrEditingEvent(m_ptrDraggingEventOrig, TRUE);

			CancelDraggingEvent();

			BOOL bControlUpdated = FALSE;
			if (ptrEditingEvent && !m_bResetUndoBuffer)
			{
				bControlUpdated = _Undo(ptrEditingEvent);
				m_pUndoBuffer->Set(*m_pPrevUndoBuffer);
			}

			if (!bControlUpdated && GetCalendarControl())
			{
				CXTPCalendarControl::CUpdateContext updateContext1(GetCalendarControl(), xtpCalendarUpdateLayout | xtpCalendarUpdateRedraw);
				GetCalendarControl()->Populate();
			}
		}
	}
	else if (nChar == VK_F2)
	{
		if (IsEditingSubject())
		{
			return;
		}

		CXTPCalendarViewEvent* pViewEvent = GetLastSelectedViewEvent();

		if (GetViewType() == xtpCalendarDayView || GetViewType() == xtpCalendarWorkWeekView || GetViewType() == xtpCalendarFullWeekView)
		{
			CPoint point = m_ptLBtnUpMousePos;
			XTP_CALENDAR_HITTESTINFO hitInfo;
			if (HitTest(point, &hitInfo))
			{
				if (pViewEvent)
				{
					CXTPCalendarEvent* pEvent = pViewEvent->GetEvent();
					if (pEvent)
						pViewEvent = FindViewEventByGroup(pEvent, pEvent->GetStartTime(), hitInfo.nGroup);
				}
			}
		}

		BOOL bEnabled = XTP_SAFE_GET1(GetCalendarOptions(), bEnableInPlaceEditEventSubject_ByF2, FALSE);
		bEnabled = bEnabled && m_eDraggingMode == xtpCalendaDragModeUnknown && pViewEvent;

		if (bEnabled)
			bEnabled = !OnBeforeEditOperationNotify(xtpCalendarEO_EditSubject_ByF2, pViewEvent);

		if (bEnabled)
		{
			ASSERT(!m_ptrDraggingEventOrig && !m_ptrDraggingEventNew && !m_ptrEditingViewEvent);

			if (pViewEvent)
			{
				CXTPCalendarControl::CUpdateContext updateContext1(m_pControl, xtpCalendarUpdateRedraw);

				EmptyUndoBuffer();

				StartEditSubject(pViewEvent);
			}
		}
	}
	else if (nChar == VK_DELETE)
	{
		if (m_eDraggingMode == xtpCalendaDragModeUnknown && !m_ptrEditingViewEvent)
		{
			ASSERT(!m_ptrDraggingEventOrig && !m_ptrDraggingEventNew && !m_ptrEditingViewEvent);

			CXTPCalendarViewEvent* pViewEvent = GetLastSelectedViewEvent();

			if (m_selectedBlock.dtEnd.GetStatus() == COleDateTime::valid &&
				GetCalendarControl() && pViewEvent && pViewEvent->IsSelected())
			{
				CXTPCalendarControl::CUpdateContext updateContext1(GetCalendarControl(), xtpCalendarUpdateRedraw | xtpCalendarUpdateLayout);

				VERIFY(GetCalendarControl()->DoDeleteSelectedEvents(pViewEvent));

				GetCalendarControl()->Populate();
			}
		}
	}
	else if (nChar == VK_TAB)
	{
		if (m_eDraggingMode == xtpCalendaDragModeUnknown ||
			m_eDraggingMode == xtpCalendaDragModeEditSubject)
		{
			BOOL bShift = (GetKeyState(VK_SHIFT)  < 0);

			CXTPCalendarControl::CUpdateContext updateContext1(m_pControl, xtpCalendarUpdateRedraw);
			if (IsEditingSubject())
			{
				EndEditSubject(xtpCalendarEditSubjectCommit);
			}

			COleDateTime dtMinStart = GetNextTimeEditByTAB();

			CXTPCalendarViewEvent* pViewEvent = GetLastSelectedViewEvent();
			CXTPCalendarEvent* pLastEvent = pViewEvent ? pViewEvent->GetEvent() : NULL;

			const int cLoopMax = 5000;
			int i;
			for (i = 0; i < cLoopMax; i++) // to prevent infinite looping
			{
				pViewEvent = FindEventToEditByTAB(dtMinStart, bShift, pLastEvent);
				pLastEvent = pViewEvent ? pViewEvent->GetEvent() : NULL;
				if (!pViewEvent || pViewEvent && pViewEvent->IsVisible())
				{
					break;
				}
			}
			ASSERT(i < cLoopMax); // probably infinite looping ???


			UnselectAllEvents();
			if (pViewEvent && pViewEvent->IsVisible() && pViewEvent->GetEvent())
			{
				dtMinStart = pViewEvent->GetEvent()->GetStartTime();
				UpdateNextTimeEditByTAB(dtMinStart, bShift);

				EmptyUndoBuffer();

				BOOL bEnabled = XTP_SAFE_GET1(GetCalendarOptions(), bEnableInPlaceEditEventSubject_ByTab, FALSE);
				if (bEnabled)
				{
					bEnabled = !OnBeforeEditOperationNotify(xtpCalendarEO_EditSubject_ByTab, pViewEvent);
				}

				if (bEnabled)
				{
					StartEditSubject(pViewEvent);
				}
				else
				{
					SelectEvent(pViewEvent->GetEvent());
				}

				SelectDay(XTP_SAFE_GET2(pViewEvent, GetViewGroup_(), GetViewDay_(), NULL),
					XTP_SAFE_GET2(pViewEvent, GetViewGroup_(), GetGroupIndex(), -1));
			}
			else
			{
				dtMinStart = GetViewDayDate(bShift ? GetViewDayCount()-1 : 0);
				UpdateNextTimeEditByTAB(dtMinStart, bShift, TRUE);
			}
		}
	}
}

BOOL CXTPCalendarView::OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CXTPCalendarControl::CUpdateContext updateContext(m_pControl, xtpCalendarUpdateRedrawIfNeed);
	CSelectionChangedContext selChanged(this);

	BOOL bHandled = TBase::OnSysKeyDown(nChar, nRepCnt, nFlags);

	UINT uAltBit = (((UINT)1) << 13);
	if (nChar == VK_BACK && (nFlags & uAltBit))
	{
		bHandled = TRUE;
		if (CanUndo())
			Undo();
		else if (CanRedo())
			Redo();
	}

	return bHandled;
}

BOOL CXTPCalendarView::SetDraggingEvent(CXTPCalendarEvent* pEventOrig)
{
	if (m_ptrDraggingEventOrig || m_ptrDraggingEventNew || !pEventOrig)
		return FALSE;
// Dragging Event already set or CommitDraggingEvent() was not called or invalid parameter

	pEventOrig->InternalAddRef();
	m_ptrDraggingEventOrig = pEventOrig;
	m_ptrDraggingEventNew = pEventOrig->CloneEvent();

	if (m_ptrDraggingEventNew
		&& m_ptrDraggingEventNew->GetRecurrenceState() == xtpCalendarRecurrenceOccurrence)
	{
		VERIFY(m_ptrDraggingEventNew->MakeAsRException());
	}

	return TRUE;
}

CXTPCalendarEvent* CXTPCalendarView::GetDraggingEventOrig() const
{
	return m_ptrDraggingEventOrig;
}

CXTPCalendarEvent* CXTPCalendarView::GetDraggingEventNew() const
{
	return m_ptrDraggingEventNew;
}

BOOL CXTPCalendarView::CommitDraggingEvent()
{
	ASSERT(m_ptrDraggingEventNew);

	if (!GetCalendarControl())
	{
		return FALSE;
	}

	CXTPAutoResetValue<BOOL> autoSet_UpdateCtrlNf(GetCalendarControl()->m_bUpdateWhenEventChangedNotify);
	autoSet_UpdateCtrlNf = FALSE;

	if (m_ptrDraggingEventNew && m_ptrDraggingEventOrig)
	{
		BOOL bDPdiff = m_ptrDraggingEventNew->GetDataProvider() != m_ptrDraggingEventOrig->GetDataProvider();

		BOOL bChanged = !m_ptrDraggingEventNew->IsEqualStartEnd(m_ptrDraggingEventOrig);
		bChanged = bChanged || m_ptrDraggingEventNew->GetSubject() != m_ptrDraggingEventOrig->GetSubject();
		bChanged = bChanged || m_ptrDraggingEventNew->IsAllDayEvent() != m_ptrDraggingEventOrig->IsAllDayEvent();
		bChanged = bChanged || m_ptrDraggingEventNew->GetScheduleID() != m_ptrDraggingEventOrig->GetScheduleID();
		bChanged = bChanged || bDPdiff;

		if (bChanged || m_eDraggingMode == xtpCalendaDragModeCopy)
		{
			if (!m_pUndoBuffer->IsInUndoBuffer(m_ptrDraggingEventOrig) || m_bResetUndoBuffer)
			{
				m_pPrevUndoBuffer->Set(*m_pUndoBuffer);
				EmptyUndoBuffer();
			}

			CXTPCalendarData* pDataNew = m_ptrDraggingEventNew->GetDataProvider(); // _GetDataProviderForDraggingEvent(m_ptrDraggingEventNew);
			CXTPCalendarData* pDataOrig = m_ptrDraggingEventOrig->GetDataProvider();

			if (m_eDraggingMode == xtpCalendaDragModeCopy)
			{
				if (m_ptrDraggingEventNew->GetRecurrenceState() != xtpCalendarRecurrenceNotRecurring)
				{
					VERIFY(m_ptrDraggingEventNew->RemoveRecurrence() );
				}
				if (pDataNew)
				{
					DBG_DATA_VERIFY(pDataNew->AddEvent(m_ptrDraggingEventNew) );
				}
			}
			else
			{
				m_pUndoBuffer->AddUndoAction(m_ptrDraggingEventOrig, m_ptrDraggingEventNew);
				m_eUndoMode = xtpCalendarUndoModeUndo;
				m_bResetUndoBuffer = FALSE;

				if (pDataNew)
				{
					if (!bDPdiff)
					{
						// restore event ID for case when m_ptrDraggingEventNew was recreated
						// during dragging between providers
						m_ptrDraggingEventNew->SetEventID(m_ptrDraggingEventOrig->GetEventID());

						DBG_DATA_VERIFY(pDataNew->ChangeEvent(m_ptrDraggingEventNew) );
					}
					else
					{
						if (m_ptrDraggingEventNew->GetRecurrenceState() != xtpCalendarRecurrenceNotRecurring)
						{
							VERIFY(m_ptrDraggingEventNew->RemoveRecurrence() );
						}
						if (pDataOrig)
						{
							DBG_DATA_VERIFY(pDataOrig->DeleteEvent(m_ptrDraggingEventOrig) );
						}

						DBG_DATA_VERIFY(pDataNew->AddEvent(m_ptrDraggingEventNew));
					}
				}
			}
		}

		m_ptrDraggingEventOrig = NULL;
		m_ptrDraggingEventNew = NULL;

		return bChanged;
	}
	else if (m_ptrDraggingEventNew)
	{
		if (!m_pUndoBuffer->IsInUndoBuffer(m_ptrDraggingEventNew) || m_bResetUndoBuffer)
		{
			m_pPrevUndoBuffer->Set(*m_pUndoBuffer);
			EmptyUndoBuffer();
		}

		CXTPCalendarData* pData = m_ptrDraggingEventNew->GetDataProvider(); //_GetDataProviderForDraggingEvent(m_ptrDraggingEventNew);

		ASSERT(m_ptrDraggingEventNew->GetRecurrenceState() == xtpCalendarRecurrenceNotRecurring);

		if (pData)
		{
			DBG_DATA_VERIFY( pData->AddEvent(m_ptrDraggingEventNew) );
		}

		m_pUndoBuffer->AddUndoAction(NULL, m_ptrDraggingEventNew);
		m_eUndoMode = xtpCalendarUndoModeUndo;
		m_bResetUndoBuffer = FALSE;

		m_ptrDraggingEventOrig = NULL;
		m_ptrDraggingEventNew = NULL;

		return TRUE;
	}

	return FALSE;
}

void CXTPCalendarView::CancelDraggingEvent()
{
	ASSERT(m_ptrDraggingEventNew);
	m_eDraggingMode = xtpCalendaDragModeUnknown;
	if (GetCalendarControl())
	{
		GetCalendarControl()->m_mouseMode = xtpCalendarMouseNothing;
		GetCalendarControl()->UpdateMouseCursor();
	}

	m_ptrDraggingEventOrig = NULL;
	m_ptrDraggingEventNew = NULL;

	m_ptStartDragging.x = m_ptStartDragging.y = LONG_MAX;
	m_bStartedClickInside = FALSE;
}

CXTPCalendarEventsPtr CXTPCalendarView::RetrieveDayEvents(CXTPCalendarResource* pRC,
		COleDateTime dtDay)
{
	ASSERT(pRC);

	dtDay = CXTPCalendarUtils::ResetTime(dtDay);

	CXTPCalendarEventsPtr ptrEvents = pRC ? pRC->RetrieveDayEvents(dtDay) : new CXTPCalendarEvents();

	if (!m_ptrDraggingEventNew || !ptrEvents || !pRC)
	{
		return ptrEvents;
	}

	BOOL bIsDragCopy = m_eDraggingMode == xtpCalendaDragModeCopy;

	COleDateTime dtDay1 = GetDateTimeDay(m_ptrDraggingEventNew->GetStartTime(), m_ptrDraggingEventNew->IsAllDayEvent());
	COleDateTime dtDay2 = GetDateTimeDay(m_ptrDraggingEventNew->GetEndTime(), m_ptrDraggingEventNew->IsAllDayEvent());

	UINT uSchID = m_ptrDraggingEventNew->GetScheduleID();
	BOOL bNewEventScheduleVisible =
			m_ptrDraggingEventNew->GetDataProvider() == pRC->GetDataProvider() &&
			pRC->ExistsScheduleID(uSchID);

	if (!bIsDragCopy)
	{
		int nCount = ptrEvents->GetCount();
		for (int i = nCount-1; i >= 0; i--)
		{
			CXTPCalendarEvent* pEvent = ptrEvents->GetAt(i, FALSE);
			if (!pEvent)
			{
				continue;
			}

			if (m_ptrDraggingEventOrig &&
				m_ptrDraggingEventOrig->GetDataProvider() == pEvent->GetDataProvider() &&
				m_ptrDraggingEventOrig->IsEqualIDs(pEvent) &&
				m_ptrDraggingEventOrig->GetScheduleID() == pEvent->GetScheduleID())
			{
				ptrEvents->RemoveAt(i);
				//break;
			}
		}
	}

	if (dtDay >= dtDay1 && dtDay <= dtDay2 && bNewEventScheduleVisible)
	{
		ptrEvents->Add(m_ptrDraggingEventNew);
	}

	return ptrEvents;
}

XTPCalendarDraggingMode CXTPCalendarView::_GetDraggingMode(XTP_CALENDAR_HITTESTINFO* pHitTest) const
{
	if (!pHitTest)
	{
		ASSERT(FALSE);
		return xtpCalendaDragModeUnknown;
	}

	if (pHitTest->uHitCode & xtpCalendarHitTestEventResizeArea_Mask)
	{
		if (pHitTest->uHitCode & xtpCalendarHitTestEventResizeBegin)
		{
			return xtpCalendaDragModeResizeBegin;
		}
		else if (pHitTest->uHitCode & xtpCalendarHitTestEventResizeEnd)
		{
			return xtpCalendaDragModeResizeEnd;
		}
	}
	else
	{
		BOOL bIsCtrl = (GetKeyState(VK_CONTROL)  < 0);
		return bIsCtrl ? xtpCalendaDragModeCopy : xtpCalendaDragModeMove;
	}

	return xtpCalendaDragModeUnknown;
}

void CXTPCalendarView::OnStartDragging(CPoint /*point*/, XTP_CALENDAR_HITTESTINFO* pHitTest)
{
	if (!pHitTest)
		return;

	if ((pHitTest->uHitCode & xtpCalendarHitTestEvent_Mask) == 0)
		return;

	if (IsEditingSubject())
		EndEditSubject(xtpCalendarEditSubjectCommit, FALSE);

	m_spDraggingStartOffset.SetStatus(COleDateTimeSpan::null);
	m_eDraggingMode = xtpCalendaDragModeUnknown;

	if (!pHitTest->pViewEvent || !pHitTest->pViewEvent->GetEvent())
		return;

	m_eDraggingMode = _GetDraggingMode(pHitTest);
	ASSERT(m_eDraggingMode != xtpCalendaDragModeUnknown);

	if (_IsDragModeCopyMove(m_eDraggingMode))
	{
		if (pHitTest->bTimePartValid)
		{
			m_spDraggingStartOffset = pHitTest->dt - pHitTest->pViewEvent->GetEvent()->GetStartTime();
		}
		else
		{
			m_spDraggingStartOffset = CXTPCalendarUtils::ResetTime(pHitTest->dt) -
				CXTPCalendarUtils::ResetTime(pHitTest->pViewEvent->GetEvent()->GetStartTime());
		}

		if (!m_nKeyStateTimerID)
			m_nKeyStateTimerID = SetTimer(50);
	}

	//---------------------------------------------------------------------------
	VERIFY(SetDraggingEvent(pHitTest->pViewEvent->GetEvent()));
}

BOOL CXTPCalendarView::OnEndDragging(CPoint point, XTP_CALENDAR_HITTESTINFO* /*phitInfo*/)
{
	if (!m_ptrDraggingEventNew || !m_ptrDraggingEventOrig ||
			m_eDraggingMode == xtpCalendaDragModeEditSubject)
	{
		return FALSE;
	}

	CXTPCalendarEventPtr ptrEventToSelect = m_ptrDraggingEventNew;

	BOOL bChanged = CommitDraggingEvent();

	int nDragMode = m_eDraggingMode;
	m_eDraggingMode = xtpCalendaDragModeUnknown;

	if (bChanged && GetCalendarControl())
		GetCalendarControl()->Populate();

	UnselectAllEvents();

	if (_IsDragModeCopyMove(nDragMode))
		EmptyUndoBuffer();

	if (ptrEventToSelect && (nDragMode == xtpCalendaDragModeResizeBegin || nDragMode == xtpCalendaDragModeResizeEnd))
	{
		CXTPCalendarControl::CUpdateContext updateContext(m_pControl, xtpCalendarUpdateRedraw);

		COleDateTime dtFVDate = GetDateTimeDay(ptrEventToSelect->GetStartTime(), ptrEventToSelect->IsAllDayEvent());
		if (nDragMode == xtpCalendaDragModeResizeEnd)
			dtFVDate = CXTPCalendarUtils::RetriveEventEndDate(ptrEventToSelect);

		CXTPCalendarViewEvent* pViewEvent = FindViewEvent(ptrEventToSelect, dtFVDate);
		if (pViewEvent && pViewEvent->GetEvent())
		{
			SelectDay(XTP_SAFE_GET2(pViewEvent, GetViewGroup_(), GetViewDay_(), NULL),
				XTP_SAFE_GET2(pViewEvent, GetViewGroup_(), GetGroupIndex(), -1));

			BOOL bEnabled = XTP_SAFE_GET1(GetCalendarOptions(), bEnableInPlaceEditEventSubject_AfterEventResize, FALSE);
			if (bEnabled)
				bEnabled = !OnBeforeEditOperationNotify(xtpCalendarEO_EditSubject_AfterEventResize, pViewEvent);

			BOOL bNo = pViewEvent->GetEvent()->GetDurationMinutes() < 1440;
			if (bEnabled && bNo)
				StartEditSubject(pViewEvent);
			else
				SelectEvent(pViewEvent->GetEvent());
		}
	}

	SelectEvent(ptrEventToSelect);

	return TRUE;
}

CXTPCalendarViewEvent* CXTPCalendarView::FindViewEvent(CXTPCalendarEvent* pEvent, COleDateTime dtDay) const
{
	if (!pEvent)
	{
		ASSERT(FALSE);
		return NULL;
	}
	dtDay = CXTPCalendarUtils::ResetTime(dtDay);

	CXTPCalendarViewEvent* pFEView = NULL;
	BOOL bFDayProceessed = FALSE;

	int nCount = GetViewDayCount();
	for (int i = 0; i < nCount; i++)
	{
		CXTPCalendarViewDay* pViewDay = GetViewDay_(i);
		if (!pViewDay)
		{
			ASSERT(FALSE);
			continue;
		}

		COleDateTime dtDayDate = pViewDay->GetDayDate();

		int nGroupsCount = pViewDay->GetViewGroupsCount();
		for (int g = 0; g < nGroupsCount; g++)
		{
			CXTPCalendarViewGroup* pViewGroup = pViewDay->GetViewGroup_(g);
			ASSERT(pViewGroup);
			if (!pViewGroup)
			{
				continue;
			}
			int nECount = pViewGroup->GetViewEventsCount();
			for (int j = 0; j < nECount; j++)
			{
				CXTPCalendarViewEvent* pViewEvent = pViewGroup->GetViewEvent_(j);
				CXTPCalendarEvent* pEv = pViewEvent ? pViewEvent->GetEvent() : NULL;
				if (!pEv)
				{
					ASSERT(FALSE);
					continue;
				}
				if (pEvent->IsEqualIDs(pEv) && pEvent->IsEqualStartEnd(pEv))
				{
					if (pViewEvent->IsVisible())
					{
						pFEView = pViewEvent;
					}
					if (CXTPCalendarUtils::IsEqual(dtDay, dtDayDate))
					{
						bFDayProceessed = TRUE;
					}
					if (pFEView && bFDayProceessed)
					{
						return pFEView;
					}
				}
			}
		}
	}
	return NULL;
}

CXTPCalendarViewEvent* CXTPCalendarView::FindViewEventByGroup(CXTPCalendarEvent* pEvent,
												 COleDateTime dtDay, int nScheduleID) const
{
	if (!pEvent)
	{
		ASSERT(FALSE);
		return NULL;
	}
	dtDay = CXTPCalendarUtils::ResetTime(dtDay);

	CXTPCalendarViewEvent* pFEView = NULL;
	BOOL bFDayProceessed = FALSE;

	int nCount = GetViewDayCount();
	for (int i = 0; i < nCount; i++)
	{
		CXTPCalendarViewDay* pViewDay = GetViewDay_(i);
		if (!pViewDay)
		{
			ASSERT(FALSE);
			continue;
		}

		COleDateTime dtDayDate = pViewDay->GetDayDate();

		int nGroupsCount = pViewDay->GetViewGroupsCount();
		if (nScheduleID < nGroupsCount)
		{
			CXTPCalendarViewGroup* pViewGroup = pViewDay->GetViewGroup_(nScheduleID);
			ASSERT(pViewGroup);
			if (!pViewGroup)
			{
				continue;
			}
			int nECount = pViewGroup->GetViewEventsCount();
			for (int j = 0; j < nECount; j++)
			{
				CXTPCalendarViewEvent* pViewEvent = pViewGroup->GetViewEvent_(j);
				CXTPCalendarEvent* pEv = pViewEvent ? pViewEvent->GetEvent() : NULL;
				if (!pEv)
				{
					ASSERT(FALSE);
					continue;
				}
				if (pEvent->IsEqualIDs(pEv) && pEvent->IsEqualStartEnd(pEv))
				{
					if (pViewEvent->IsVisible())
					{
						pFEView = pViewEvent;
					}
					if (CXTPCalendarUtils::IsEqual(dtDay, dtDayDate))
					{
						bFDayProceessed = TRUE;
					}
					if (pFEView && bFDayProceessed)
					{
						return pFEView;
					}
				}
			}
		}
	}
	return NULL;
}

BOOL CXTPCalendarView::HasSelectedViewEvent() const
{
	if (!m_pSelectedEvents)
	{
		return FALSE;
	}
	return m_pSelectedEvents->GetCount() > 0;
}

CXTPCalendarViewEvents* CXTPCalendarView::GetSelectedViewEvents() const
{
	return m_pSelectedEvents;
}

BOOL CXTPCalendarView::IsSingleEventSelected() const
{
	int nCount = m_pSelectedEvents ? m_pSelectedEvents->GetCount() : 0;

	if (nCount == 0)
	return FALSE;

	if (nCount == 1)
		return TRUE;

	CXTPCalendarEvent* pEvent = m_pSelectedEvents->GetAt(0)->GetEvent();

	for (int i = 0; i < nCount; i++)
	{
		CXTPCalendarViewEvent* pViewEvent = m_pSelectedEvents->GetAt(i);
		ASSERT(pViewEvent);
		if (pViewEvent && !pViewEvent->GetEvent()->IsEqualIDs(pEvent))
			return FALSE;
	}

	return TRUE;
}

CXTPCalendarViewEventsPtr CXTPCalendarView::GetSelectedEvents() const
{
	CXTPCalendarViewEventsPtr ptrEventsWithViews = new CXTPCalendarViewEvents();
	CXTPCalendarEvents arEvents;

	ASSERT(m_pSelectedEvents);
	if (!ptrEventsWithViews || !m_pSelectedEvents)
	{
		return NULL;
	}

	int nCount = m_pSelectedEvents->GetCount();
	for (int i = 0; i < nCount; i++)
	{
		CXTPCalendarViewEvent* pViewEvent = m_pSelectedEvents->GetAt(i);
		CXTPCalendarEvent* pEvent = pViewEvent ? pViewEvent->GetEvent() : NULL;

		if (pEvent && arEvents.Find(pEvent) < 0)
		{
			arEvents.Add(pEvent, TRUE);
			ptrEventsWithViews->Add(pViewEvent, TRUE);
		}
	}

	return ptrEventsWithViews;
}

BOOL CXTPCalendarView::GetSelection(COleDateTime* pBegin, COleDateTime* pEnd,
									BOOL* pbAllDayEvent, int* pnGroupIndex,
									COleDateTimeSpan* pspSelectionResolution) const
{
	if (!m_selectedBlock.IsValid())
	{
		return FALSE;
	}

	if (pspSelectionResolution)
	{
		*pspSelectionResolution = COleDateTimeSpan(1, 0, 0, 0);
	}
	if (pbAllDayEvent)
	{
		*pbAllDayEvent = TRUE;
	}
	if (pnGroupIndex)
	{
		*pnGroupIndex = m_selectedBlock.nGroupIndex;
	}

	//---------------------------------------------------------------------------
	if (pBegin)
	{
		*pBegin = min(m_selectedBlock.dtBegin, m_selectedBlock.dtEnd);
	}
	if (pEnd)
	{
		*pEnd = max(m_selectedBlock.dtBegin, m_selectedBlock.dtEnd) + COleDateTimeSpan(1, 0, 0, 0);
	}

	return TRUE;

}

void CXTPCalendarView::SetSelection(const COleDateTime& dtBegin, const COleDateTime& dtEnd,
									BOOL bAllDayEvent, int nGroupIndex)
{
	m_selectedBlock.dtBegin = dtBegin;
	m_selectedBlock.dtEnd = dtEnd;
	m_selectedBlock.bAllDayEvent = bAllDayEvent;
	m_selectedBlock.nGroupIndex = nGroupIndex;

	CSelectionChangedContext selChanged(this, xtpCalendarSelectionDays);
}

void CXTPCalendarView::ResetSelection()
{
	if (IsEditingSubject())
	{
		CXTPCalendarEventPtr ptrEditingEvent(m_ptrDraggingEventOrig, TRUE);
		EndEditSubject(xtpCalendarEditSubjectCancel);
	}
	m_selectedBlock.dtBegin.SetStatus(COleDateTime::null);
	m_selectedBlock.dtEnd.SetStatus(COleDateTime::null);

	m_selectedBlock.bAllDayEvent = FALSE;
	m_selectedBlock.nGroupIndex = -1;

	CSelectionChangedContext selChanged(this, xtpCalendarSelectionDays);
}

BOOL CXTPCalendarView::SelectionContains(COleDateTime date, int nGroupIndex, BOOL* pbAllDaySelection)
{
	COleDateTime dtBegin;
	COleDateTime dtEnd;
	int nSelGroupIndex;

	if (GetSelection(&dtBegin, &dtEnd, pbAllDaySelection, &nSelGroupIndex))
	{
		if (nGroupIndex < 0 || nGroupIndex == nSelGroupIndex)
		{
			return (dtBegin <= date && (double)date <= (double)dtEnd - XTP_HALF_SECOND);
		}
	}

	return FALSE;
}


COleDateTime CXTPCalendarView::GetNextTimeEditByTAB() const
{
	ASSERT(GetViewDayCount() != 0);

	COleDateTime dtDay;

	if (m_selectedBlock.dtEnd.GetStatus() == COleDateTime::valid)
	{
		dtDay = m_selectedBlock.dtEnd;
	}
	else
	{
		dtDay = GetViewDayDate(0);
	}

	dtDay = CXTPCalendarUtils::ResetTime(dtDay);
	return dtDay;
}

void CXTPCalendarView::UpdateNextTimeEditByTAB(COleDateTime dtNext, BOOL /*bReverse*/, BOOL /*bReset*/)
{
	COleDateTime dtNextDay = CXTPCalendarUtils::ResetTime(dtNext);
	COleDateTime dtCurrDay = CXTPCalendarUtils::ResetTime(GetNextTimeEditByTAB());
	if (dtNextDay != dtCurrDay)
	{
		SelectDay(dtNextDay);
	}
}

CXTPCalendarViewEvent* CXTPCalendarView::FindEventToEditByTAB(COleDateTime dtMinStart,
								BOOL bReverse, CXTPCalendarEvent* pAfterEvent)
{
	COleDateTime dtMinStartDay = CXTPCalendarUtils::ResetTime(dtMinStart);
	COleDateTime dtMinStart2 = dtMinStartDay + COleDateTimeSpan(1, 0, 0, 0);

	BOOL bAfterEventWas = pAfterEvent == NULL;
	if (pAfterEvent)
	{
		COleDateTime dtAEStartDay = GetDateTimeDay(pAfterEvent->GetStartTime(), pAfterEvent->IsAllDayEvent());
		if (bReverse ? dtAEStartDay > dtMinStartDay : dtAEStartDay < dtMinStartDay)
		{
			bAfterEventWas = TRUE;
		}
	}

	int nDays = GetViewDayCount();
	for (int nDay = bReverse ? nDays-1 : 0; bReverse ? nDay >= 0 : nDay < nDays;
		bReverse ? nDay-- : nDay++)
	{
		CXTPCalendarViewDay* pVDay = GetViewDay_(nDay);
		if (!pVDay)
		{
			ASSERT(FALSE);
			continue;
		}

		COleDateTime dtDay = pVDay->GetDayDate();

		if (bReverse ? dtDay > dtMinStartDay : dtDay < dtMinStartDay)
		{
			continue;
		}

		int nGroupsCount = pVDay->GetViewGroupsCount();
		for (int nGroup = bReverse ? nGroupsCount - 1 : 0;
			bReverse ? nGroup >= 0 : nGroup < nGroupsCount;
			bReverse ? nGroup-- : nGroup++)
		{
			CXTPCalendarViewGroup* pViewGroup = pVDay->GetViewGroup_(nGroup);
			ASSERT(pViewGroup);
			if (!pViewGroup)
			{
				continue;
			}

			int nEvents = pViewGroup->GetViewEventsCount();
			for (int nEvent = bReverse ? nEvents - 1 : 0;
				bReverse ? nEvent >= 0 : nEvent < nEvents;
				bReverse ? nEvent-- : nEvent++)
			{
				CXTPCalendarViewEvent* pVEvent = pViewGroup->GetViewEvent_(nEvent);
				if (!pVEvent || !pVEvent->GetEvent())
				{
					ASSERT(FALSE);
					continue;
				}

				if (pAfterEvent)
				{
					if (pAfterEvent->IsEqualIDs(pVEvent->GetEvent()))
					{
						bAfterEventWas = TRUE;
						continue;
					}
				}
				COleDateTime dtES = pVEvent->GetEvent()->GetStartTime();
				COleDateTime dtESDay = GetDateTimeDay(dtES, pVEvent->GetEvent()->IsAllDayEvent());

				if (bReverse ? (dtES > dtMinStart2 || dtESDay != dtDay) :
								(dtES < dtMinStart || dtESDay < dtDay))
				{
					continue;
				}

				if (bAfterEventWas)
				{
					return pVEvent;
				}
			}
		}
	}
	return NULL;
}

void CXTPCalendarView::UnselectAllEvents()
{
	if (!m_pSelectedEvents || !GetCalendarControl())
	{
		return;
	}

	int nCount = m_pSelectedEvents->GetCount();
	m_pSelectedEvents->RemoveAll();

	if (nCount)
	{
		CXTPCalendarControl::CUpdateContext updateContext(GetCalendarControl(), xtpCalendarUpdateRedraw);
		CSelectionChangedContext selChanged(this, xtpCalendarSelectionEvents);
	}
}

void CXTPCalendarView::_ReSelectSelectEvents()
{
	if (!m_pSelectedEvents)
		return;

	CXTPCalendarViewEventsPtr ptrSelEvents = GetSelectedEvents();

	m_pSelectedEvents->RemoveAll();

	CXTPCalendarEvent* pEvent;
	if (ptrSelEvents)
	{
		int nCount = ptrSelEvents->GetCount();
		for (int i = 0; i < nCount; i++)
		{
			if (ptrSelEvents->GetAt(i))
			{
				pEvent = ptrSelEvents->GetAt(i)->GetEvent();
				if (pEvent)
					SelectEvent(pEvent);
			}
		}
	}
}

void CXTPCalendarView::SelectDays(COleDateTime dtNewSel)
{
	CXTPCalendarControl::CUpdateContext updateContext(GetCalendarControl(), xtpCalendarUpdateRedraw);
	m_selectedBlock.dtEnd = dtNewSel;

	CSelectionChangedContext selChanged(this, xtpCalendarSelectionDays);
}

void CXTPCalendarView::SelectDay(COleDateTime dtSelDay, int nGroupIndex)
{
	CXTPCalendarControl::CUpdateContext updateContext(GetCalendarControl(), xtpCalendarUpdateRedraw);

	m_selectedBlock.dtBegin = m_selectedBlock.dtEnd = dtSelDay;
	m_selectedBlock.nGroupIndex = nGroupIndex;

	CSelectionChangedContext selChanged(this, xtpCalendarSelectionDays);
}

void CXTPCalendarView::SelectDay(CXTPCalendarViewDay* pDay, int nGroupIndex)
{
	ASSERT(pDay);
	if (pDay)
	{
		SelectDay(pDay->GetDayDate(), nGroupIndex);
	}
}

void CXTPCalendarView::SelectViewEvent(CXTPCalendarViewEvent* pViewEvent, BOOL bSelect)
{
	if (!m_pSelectedEvents || !pViewEvent || !pViewEvent->GetEvent())
	{
		ASSERT(FALSE);
		return;
	}

	m_pSelectedEvents->Remove(pViewEvent->GetEvent());

	if (bSelect)
	{
		pViewEvent->InternalAddRef();
		m_pSelectedEvents->Add(pViewEvent);
	}
	CSelectionChangedContext selChanged(this, xtpCalendarSelectionEvents);
}

CXTPCalendarViewEvent* CXTPCalendarView::GetLastSelectedViewEvent() const
{
	if (m_pSelectedEvents && m_pSelectedEvents->GetCount() > 0)
		return m_pSelectedEvents->GetAt(m_pSelectedEvents->GetCount() - 1);

	return NULL;
}

void CXTPCalendarView::SelectEvent(CXTPCalendarEvent* pEvent, BOOL bSelect)
{
	if (!pEvent)
	{
		ASSERT(FALSE);
		return;
	}
	int nCount = GetViewDayCount();
	for (int i = 0; i < nCount; i++)
	{
		CXTPCalendarViewDay* pViewDay = GetViewDay_(i);
		if (!pViewDay)
		{
			ASSERT(FALSE);
			continue;
		}

		int nGroupsCount = pViewDay->GetViewGroupsCount();
		for (int g = 0; g < nGroupsCount; g++)
		{
			CXTPCalendarViewGroup* pViewGroup = pViewDay->GetViewGroup_(g);
			ASSERT(pViewGroup);
			if (!pViewGroup)
			{
				continue;
			}

			int nECount = pViewGroup->GetViewEventsCount();
			for (int j = 0; j < nECount; j++)
			{
				CXTPCalendarViewEvent* pViewEvent = pViewGroup->GetViewEvent_(j);
				CXTPCalendarEvent* pEv = pViewEvent ? pViewEvent->GetEvent() : NULL;
				ASSERT(pEv);
				if (pEv && pEvent->IsEqualIDs(pEv) && pEvent->IsEqualStartEnd(pEv))
				{
					CXTPCalendarControl::CUpdateContext updateContext(GetCalendarControl(), xtpCalendarUpdateRedraw);
					SelectViewEvent(pViewEvent, bSelect);
				}
			}
		}
	}
}

BOOL CXTPCalendarView::OnDragging(CPoint /*point*/, XTP_CALENDAR_HITTESTINFO* pHitTest)
{
	ASSERT(pHitTest);
	if (!pHitTest || !m_ptrDraggingEventNew || m_eDraggingMode == xtpCalendaDragModeEditSubject)
	{
		return FALSE;
	}
	//---------------------------------------------------------------------------
	if (pHitTest->pViewDay)
	{
		if (m_eDraggingMode == xtpCalendaDragModeResizeBegin || m_eDraggingMode == xtpCalendaDragModeResizeEnd)
		{
			SelectDay(pHitTest->pViewDay, pHitTest->nGroup);
		}
	}
	//---------------------------------------------------------------------------
	COleDateTime dtNewTime;

	if (_IsDragModeCopyMove(m_eDraggingMode))
	{
		if (!pHitTest->bTimePartValid)
		{
			dtNewTime = CXTPCalendarUtils::UpdateDate(m_ptrDraggingEventNew->GetStartTime(), pHitTest->dt);
		}
		else
		{
			dtNewTime = pHitTest->dt;
		}
		dtNewTime -= m_spDraggingStartOffset;

		BOOL bChanged = FALSE;
		//------------------------------------
		if (!CXTPCalendarUtils::IsEqual(dtNewTime, m_ptrDraggingEventNew->GetStartTime()))
		{
			m_ptrDraggingEventNew->MoveEvent(dtNewTime);
			bChanged = TRUE;
		}

		//***
		if (bChanged)
		{
			return TRUE;
		}
	}
	else if (m_eDraggingMode == xtpCalendaDragModeResizeBegin)
	{
		dtNewTime = pHitTest->dt;
		COleDateTime dtEventEnd = m_ptrDraggingEventNew->GetEndTime();

		if (!CXTPCalendarUtils::IsEqual(dtNewTime, m_ptrDraggingEventNew->GetStartTime()))
		{
			if (dtNewTime > dtEventEnd)
			{
				dtNewTime = dtEventEnd;
			}

			m_ptrDraggingEventNew->SetStartTime(dtNewTime);

			BOOL bIsZeroStartEnd = CXTPCalendarUtils::IsZeroTime(m_ptrDraggingEventNew->GetStartTime()) &&
									CXTPCalendarUtils::IsZeroTime(m_ptrDraggingEventNew->GetEndTime());

			BOOL bIsZeroLen = CXTPCalendarUtils::IsEqual(m_ptrDraggingEventNew->GetStartTime(),
									m_ptrDraggingEventNew->GetEndTime());

			m_ptrDraggingEventNew->SetAllDayEvent(bIsZeroStartEnd && !bIsZeroLen);

			return TRUE;
		}
	}
	else if (m_eDraggingMode == xtpCalendaDragModeResizeEnd)
	{
		dtNewTime = pHitTest->dt;

		if (!pHitTest->bTimePartValid)
		{
			dtNewTime += COleDateTimeSpan(1, 0, 0, 0);
		}

		COleDateTime dtEventStart = m_ptrDraggingEventNew->GetStartTime();
		if (!CXTPCalendarUtils::IsEqual(dtNewTime, m_ptrDraggingEventNew->GetEndTime()))
		{
			if ((pHitTest->uHitCode & xtpCalendarHitTestDayViewTimeScale) &&
				(pHitTest->pViewDay == NULL))
			{
				dtNewTime.SetDateTime(
					dtEventStart.GetYear(), dtEventStart.GetMonth(), dtEventStart.GetDay(),
					dtNewTime.GetHour(), dtNewTime.GetMinute(), dtNewTime.GetSecond());
			}

			if (dtNewTime < dtEventStart)
			{
				dtNewTime = dtEventStart;
			}

			m_ptrDraggingEventNew->SetEndTime(dtNewTime);

			BOOL bIsZeroStartEnd = CXTPCalendarUtils::IsZeroTime(m_ptrDraggingEventNew->GetStartTime()) &&
									CXTPCalendarUtils::IsZeroTime(m_ptrDraggingEventNew->GetEndTime());

			BOOL bIsZeroLen = CXTPCalendarUtils::IsEqual(m_ptrDraggingEventNew->GetStartTime(),
									m_ptrDraggingEventNew->GetEndTime());

			m_ptrDraggingEventNew->SetAllDayEvent(bIsZeroStartEnd && !bIsZeroLen);

			return TRUE;
		}
	}

	return FALSE;
}

COleDateTime CXTPCalendarView::GetLastSelectedDate() const
{
	if (m_selectedBlock.dtEnd.GetStatus() != COleDateTime::valid)
	{
		return GetViewDayDate(GetViewDayCount() / 2);
	}
	return CXTPCalendarUtils::ResetTime(m_selectedBlock.dtEnd);
}

int CXTPCalendarView::FindLastSelectedDay() const
{
	COleDateTime dt = GetLastSelectedDate();

	int nCount = GetViewDayCount();
	for (int i = 0; i < nCount; i++)
	{
		CXTPCalendarViewDay* pViewDay = GetViewDay_(i);
		ASSERT(pViewDay);
		if (pViewDay && CXTPCalendarUtils::IsEqual(pViewDay->GetDayDate(), dt))
			return i;
	}
	return -1;
}

void CXTPCalendarView::ProcessDaySelection(XTP_CALENDAR_HITTESTINFO* pInfo, UINT nFlags)
{
	ASSERT(pInfo);
	if (!pInfo)
	{
		UnselectAllEvents();
		return;
	}

	if (nFlags & MK_SHIFT)
	{
		COleDateTime dtNewSelDay = GetViewDayDate(pInfo->nDay);
		SelectDays(dtNewSelDay);
	}
	else
	{
		UnselectAllEvents();

		ASSERT(pInfo->pViewDay);
		if (pInfo->pViewDay)
		{
			pInfo->pViewDay->SetSelected();
		}
	}
}

void CXTPCalendarView::ProcessEventSelection(XTP_CALENDAR_HITTESTINFO* pInfo, UINT nFlags)
{
	ASSERT(pInfo);
	if (!pInfo)
	{
		return;
	}

	BOOL bUnselectEvents = !(nFlags & (MK_SHIFT | MK_CONTROL));
	if (bUnselectEvents)
		UnselectAllEvents();

	ASSERT(pInfo->pViewDay);
	ASSERT(pInfo->pViewEvent);

	if (pInfo->pViewDay)
	{
		pInfo->pViewDay->SetSelected();
	}

	if (pInfo->pViewEvent)
	{

		BOOL bSelect = TRUE;
		if (nFlags & MK_CONTROL)
		{
			bSelect = !pInfo->pViewEvent->IsSelected();
		}
		SelectEvent(pInfo->pViewEvent->GetEvent(), bSelect);
	}
}

UINT CXTPCalendarView::SetTimer(UINT uTimeOut_ms)
{
	if (GetCalendarControl())
	{
		return GetCalendarControl()->SetTimer(uTimeOut_ms);
	}
	return 0;
}

void CXTPCalendarView::KillTimer(UINT uTimerID)
{
	if (GetCalendarControl())
	{
		VERIFY(GetCalendarControl()->KillTimer(uTimerID));
	}
}

BOOL CXTPCalendarView::OnTimer(UINT_PTR uTimerID)
{
	if (TBase::OnTimer(uTimerID))
	{
		return TRUE;
	}

	CXTPCalendarThemeOffice2007* pThemeX = DYNAMIC_DOWNCAST(CXTPCalendarThemeOffice2007, GetTheme());
	if (pThemeX)
	{
		if (pThemeX->OnTimer(uTimerID))
			return TRUE;
	}

	if (uTimerID == m_nTimerID_StartEditSubject)
	{
		KillTimer(m_nTimerID_StartEditSubject);
		m_nTimerID_StartEditSubject = 0;

		ASSERT(m_ptrEditingViewEvent);
		if (m_ptrEditingViewEvent)
		{
			EmptyUndoBuffer();
			StartEditSubject(m_ptrEditingViewEvent);
		}
		return TRUE;
	}
	else if (uTimerID == m_nKeyStateTimerID)
	{
		BOOL bIsCtrl = (GetKeyState(VK_CONTROL)  < 0);
		XTPCalendarDraggingMode eDraggingMode = bIsCtrl ? xtpCalendaDragModeCopy : xtpCalendaDragModeMove;

		BOOL bPopulate = FALSE;

		if (!_IsDragModeCopyMove(m_eDraggingMode))
		{
			KillTimer(m_nKeyStateTimerID);
			m_nKeyStateTimerID = 0;
		}
		else if (m_eDraggingMode != eDraggingMode)
		{
			ASSERT(m_ptrDraggingEventNew);
			if (m_ptrDraggingEventNew)
			{
				CXTPCalendarViewEvent* pEventView = FindViewEvent(m_ptrDraggingEventNew, m_ptrDraggingEventNew->GetStartTime());
				if (!IsEditOperationDisabledNotify(_DragMod2Operation(eDraggingMode), pEventView))
				{
					m_eDraggingMode = eDraggingMode;
					bPopulate = TRUE;
				}
			}
		}

		if (bPopulate && GetCalendarControl())
		{
			GetCalendarControl()->Populate();

			GetCalendarControl()->m_mouseMode = m_eDraggingMode == xtpCalendaDragModeCopy ?
				xtpCalendarMouseEventDragCopy : xtpCalendarMouseEventDragMove;

			GetCalendarControl()->UpdateMouseCursor();
		}
		return TRUE;
	}
	else if (uTimerID == m_nTimerID_ShowToolTip)
	{
		ShowToolTip();
		return TRUE;
	}

	return FALSE;
}

BOOL CXTPCalendarView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	if (m_nTimerID_StartEditSubject)
	{
		KillTimer(m_nTimerID_StartEditSubject);
		m_nTimerID_StartEditSubject = 0;
		m_ptrEditingViewEvent = NULL;
	}

	return TBase::OnLButtonDblClk(nFlags, point);
}

void CXTPCalendarView::OnDraw(CDC* pDC)
{
	if (GetTheme())
		Draw2(pDC);
	else
		Draw(pDC);
}

void CXTPCalendarView::AdjustLayout(CDC* pDC, const CRect& rcView, BOOL bCallPostAdjustLayout)
{
	UNREFERENCED_PARAMETER(pDC);

	m_rcView = rcView;
	_CalculateEventTimeFormat();

	if (bCallPostAdjustLayout)
	{
		OnPostAdjustLayout();
	}
}

void CXTPCalendarView::AdjustLayout2(CDC* pDC, const CRect& rcView, BOOL bCallPostAdjustLayout)
{
	UNREFERENCED_PARAMETER(pDC);
	UNREFERENCED_PARAMETER(bCallPostAdjustLayout);

	ASSERT(GetTheme());

	m_rcView = rcView;
	//_CalculateEventTimeFormat();

//  if (bCallPostAdjustLayout)
//  {
//      OnPostAdjustLayout();
//  }
}

CXTPCalendarViewDay* CXTPCalendarView::_GetViewDay(const COleDateTime& dt) const
{
	COleDateTime dtDay = CXTPCalendarUtils::ResetTime(dt);
	int nDaysCount = GetViewDayCount();

	if (!nDaysCount || !GetViewDay_(0))
	{
		return NULL;
	}

	COleDateTime dtDay0 = GetViewDay_(0)->GetDayDate();

	COleDateTimeSpan spShift = dtDay - dtDay0;

	ASSERT((double)spShift == (double)((int)(double)spShift) );

	int nShift = CXTPCalendarUtils::GetTotalDays(spShift); //(int)(double)spShift

	if (nShift < 0)
	{
		nShift = 0;
	}
	if (nShift >= nDaysCount)
	{
		nShift = nDaysCount - 1;
	}

	if (nShift >= 0 && nShift < nDaysCount)
	{
		return GetViewDay_(nShift);
	}

	return NULL;
}


COleDateTime CXTPCalendarView::GetDateTimeDay(const COleDateTime& dt, BOOL bWholeDayEvent) const
{
	CXTPCalendarOptions* pOptions = GetCalendarControl()->GetCalendarOptions();

	if ((DATE)pOptions->dtScaleMaxTime <= 1 || bWholeDayEvent)
		return CXTPCalendarUtils::ResetTime(dt);

	return CXTPCalendarUtils::ResetTime(dt - pOptions->dtScaleMinTime);
}


void CXTPCalendarView::StartEditNewEventInplace(LPCTSTR pcszInitialSubject)
{
	if (m_ptrDraggingEventOrig || m_ptrDraggingEventNew || IsEditingSubject())
	{
		MessageBeep(0);
		return;
	}

	BOOL bAllDayEvent = FALSE;
	//---------------------------------------------------------------------------
	COleDateTime dtSelBegin, dtSelEnd;
	int nGroup = 0;
	if (!GetSelection(&dtSelBegin, &dtSelEnd, &bAllDayEvent, &nGroup))
	{
		return;
	}

	//---------------------------------------------------------------------------
	CXTPCalendarData* pData = NULL; //XTP_SAFE_GET1(GetCalendarControl(), GetDataProvider(), NULL);

	CXTPCalendarViewDay* pViewDay = _GetViewDay(dtSelBegin);
	if (!pViewDay)
	{
		return;
	}
	CXTPCalendarViewGroup* pViewGroup = NULL;
	if (pViewDay && nGroup < pViewDay->GetViewGroupsCount())
	{
		pViewGroup = pViewDay->GetViewGroup_(nGroup);
		pData = pViewGroup ? pViewGroup->GetDataProvider() : NULL;
	}

	//if (pData != NULL
	//  && pData->GetCustomProperties() != NULL)
	//{
	//  CString sType(_T("Public"));
	//  CString sXML(_T("F:\\SecurityType.xml"));
	//  COleVariant rVarValue(sType);
	//  pData->GetCustomProperties()->LoadFromXML(sXML);

	//  if (pData->GetCustomProperties()->GetProperty(_T("SecurityType"), rVarValue))
	//  {
	//      AfxMessageBox(_T("SecurityType found"));
	//  }
	//  else
	//  {
	//      AfxMessageBox(_T("SecurityType not found"));
	//      pData->GetCustomProperties()->SetProperty(_T("SecurityType"), rVarValue);
	//      pData->GetCustomProperties()->SaveToXML(sXML);
	//  }
	//}

	m_ptrDraggingEventNew = pData ? pData->CreateNewEvent() : NULL;
	if (!m_ptrDraggingEventNew)
	{
		return;
	}

	//---------------------------------------------------------------------------
	UINT uScheduleID = XTP_SAFE_GET1(pViewGroup, GetScheduleID(), 0);
	m_ptrDraggingEventNew->SetScheduleID(uScheduleID);

	m_ptrDraggingEventNew->SetStartTime(min(dtSelBegin, dtSelEnd));
	m_ptrDraggingEventNew->SetEndTime(max(dtSelBegin, dtSelEnd));

	m_ptrDraggingEventNew->SetBusyStatus(bAllDayEvent ? xtpCalendarBusyStatusFree : xtpCalendarBusyStatusBusy);
	m_ptrDraggingEventNew->SetAllDayEvent(bAllDayEvent);

	//===========================================================================
	XTP_SAFE_CALL1(GetCalendarControl(), Populate());
	XTP_SAFE_CALL1(GetCalendarControl(), AdjustLayout());

	//---------------------------------------------------------------------------
	COleDateTime dtEventDay = m_ptrDraggingEventNew->GetStartTime();
	dtEventDay = GetDateTimeDay(dtEventDay, m_ptrDraggingEventNew->IsAllDayEvent());

	CXTPCalendarViewEvent* pViewEvent = FindViewEvent(m_ptrDraggingEventNew, dtEventDay);

	if (!pViewEvent)
	{
		CancelDraggingEvent();
		return;
	}

	StartEditSubject(pViewEvent);

	CEdit* pEditor = DYNAMIC_DOWNCAST(CEdit, pViewEvent->GetSubjectEditor());

	if (pEditor)
	{
		pEditor->SetWindowText(pcszInitialSubject);
		int nInitTxtLen = (int)_tcsclen(pcszInitialSubject);
		pEditor->SetSel(nInitTxtLen, nInitTxtLen);
	}
}

void CXTPCalendarView::StartEditSubject(CXTPCalendarViewEvent* pViewEvent)
{
	if (!pViewEvent)
		return;

	if (m_nTimerID_StartEditSubject)
	{
		KillTimer(m_nTimerID_StartEditSubject);
		m_nTimerID_StartEditSubject = 0;
	}

	m_ptrEditingViewEvent = pViewEvent->StartEditSubject();

	if (!m_ptrEditingViewEvent)
		return;

	CXTPCalendarControl::CUpdateContext updateContext(m_pControl, xtpCalendarUpdateRedraw);

	m_ptrEditingViewEvent->InternalAddRef();
	m_eDraggingMode = xtpCalendaDragModeEditSubject;

	if (!m_ptrDraggingEventOrig && !m_ptrDraggingEventNew)
		SetDraggingEvent(m_ptrEditingViewEvent->GetEvent());

	SelectEvent(m_ptrDraggingEventNew);
}

void CXTPCalendarView::EndEditSubject(XTPCalendarEndEditSubjectAction eAction, BOOL bUpdateControl)
{
	m_eDraggingMode = xtpCalendaDragModeUnknown;

	// kill timeout request to edit subject
	if (m_nTimerID_StartEditSubject)
	{
		ASSERT(m_ptrEditingViewEvent && !m_ptrDraggingEventOrig && !m_ptrDraggingEventNew);

		KillTimer(m_nTimerID_StartEditSubject);
		m_nTimerID_StartEditSubject = 0;
		m_ptrEditingViewEvent = NULL;
		return;
	}

	ASSERT(m_ptrEditingViewEvent && m_ptrDraggingEventNew);

	CXTPCalendarEventPtr ptrEventToSelect(m_ptrDraggingEventNew, TRUE);

	BOOL bPopulate = FALSE;
	CString strNewSubject;

	//- end edit subject operation in Event view (kill edit wnd) ----------------
	if (m_ptrEditingViewEvent)
		strNewSubject = m_ptrEditingViewEvent->EndEditSubject();

	m_ptrEditingViewEvent = NULL;

	//---------------------------------------------------------------------------
	if (m_ptrDraggingEventNew)
	{
		if (eAction == xtpCalendarEditSubjectCommit)
		{
			m_ptrDraggingEventNew->SetSubject(strNewSubject);
			if (!strNewSubject.IsEmpty())
				bPopulate = CommitDraggingEvent();
			else
			{
				ptrEventToSelect.SetPtr(m_ptrDraggingEventOrig, TRUE);

				CancelDraggingEvent();
				bPopulate = TRUE;
			}
		}
		else if (eAction == xtpCalendarEditSubjectCancel)
		{
			ptrEventToSelect.SetPtr(m_ptrDraggingEventOrig, TRUE);

			CancelDraggingEvent();
			bPopulate = TRUE;
		}
		else
		{
			ASSERT(FALSE);
		}
	}

	if (bUpdateControl && bPopulate && GetCalendarControl())
	{
		GetCalendarControl()->Populate();

		if (ptrEventToSelect)
		{
			SelectEvent(ptrEventToSelect, TRUE);
		}
	}
}

void CXTPCalendarView::OnUpdateEditingSubject(CString strNewSubject)
{
	if (m_ptrEditingViewEvent && m_ptrDraggingEventNew)
		m_ptrDraggingEventNew->SetSubject(strNewSubject);
}

CRect CXTPCalendarView::GetSubjectEditorRect() const
{
	ASSERT(m_ptrEditingViewEvent);
	if (m_ptrEditingViewEvent)
	{
		return m_ptrEditingViewEvent->GetSubjectEditorRect();
	}

	return CRect(INT_MAX, INT_MAX, INT_MAX, INT_MAX);
}

CRect CXTPCalendarView::GetTooltipRect(const CPoint& ptHit, const XTP_CALENDAR_HITTESTINFO& hitInfo) const
{
	CRect rcTip;
	rcTip.SetRectEmpty();

	if (hitInfo.pViewGroup)
	{
		rcTip = hitInfo.pViewGroup->GetTooltipRect(ptHit, hitInfo);

		CXTPCalendarThemeOffice2007* pThemeX = DYNAMIC_DOWNCAST(CXTPCalendarThemeOffice2007, GetTheme());
		if (pThemeX)
		{
			CRect rcExclude = XTP_SAFE_GET2(pThemeX->GetPrevNextEventButtonsPartX(),
								GetNextEventButtonPartX(), GetRect(), CRect(0, 0, 0, 0));
			if (!(rcTip.top > rcExclude.bottom || rcTip.bottom < rcExclude.top)
				&& rcTip.right >= rcExclude.left)
			{
				rcTip.right = rcExclude.left - 1;
			}

			rcExclude = XTP_SAFE_GET2(pThemeX->GetPrevNextEventButtonsPartX(),
						GetPrevEventButtonPartX(), GetRect(), CRect(0, 0, 0, 0));

			if (!(rcTip.top > rcExclude.bottom || rcTip.bottom < rcExclude.top)
				&& rcTip.left <= rcExclude.right)
			{
				rcTip.left = rcExclude.right + 1;
			}
		}
	}

	return rcTip;
}

void CXTPCalendarView::ShowToolTip(BOOL bShow)
{
	if (m_nTimerID_ShowToolTip)
	{
		KillTimer(m_nTimerID_ShowToolTip);
		m_nTimerID_ShowToolTip = 0;
	}

	CXTPCalendarControl* pControl = GetCalendarControl();
	if (!pControl)
		return;

	if (!bShow)
	{
		// hide a tooltip
		pControl->HideToolTip();
		return;
	}
	if (pControl->IsToolTipVisible())
	{
		return;
	}

	// Verify whether mouse is still under tooltip rect
	CRect rcTooltip;
	rcTooltip.SetRectEmpty();

	CPoint pt;
	GetCursorPos(&pt);
	pControl->ScreenToClient(&pt);

	XTP_CALENDAR_HITTESTINFO hitInfo;
	BOOL bHitTest = HitTest(pt, &hitInfo);
	if (bHitTest)
	{
		rcTooltip = GetTooltipRect(pt, hitInfo);
	}

	if (m_rcTooltip != rcTooltip)
		m_rcTooltip.SetRectEmpty();

	if (m_rcTooltip.IsRectEmpty())
		return;

	CString strTip;
	if (GetViewType() != xtpCalendarTimeLineView)
	{
		// if we are under event view -- do not show this tooltip
		if (NULL != hitInfo.pViewEvent)
			return;

		BOOL bTipEnabled = XTP_SAFE_GET2(pControl, GetCalendarOptions(), bEnableAddNewTooltip, TRUE);
		BOOL bAllowCreate = XTP_SAFE_GET2(pControl, GetCalendarOptions(), bEnableInPlaceCreateEvent, TRUE);

		if (!bTipEnabled || !bAllowCreate)
			return;

		// create tooltip text
		if (!pControl->m_bReadOnlyMode)
			strTip = XTP_SAFE_GET2(pControl, GetCalendarOptions(), strTooltipAddNewText, _T("Click to add appointment"));
		else
			strTip.Empty();
	}
	else if (hitInfo.pViewEvent) //TimeLine mode
	{
		if (hitInfo.pViewEvent->GetEvent()
			&& hitInfo.pViewEvent->GetEvent()->IsEventVisible())
			strTip = hitInfo.pViewEvent->GetEvent()->GetSubject();
	}
	// show tooltip window
	if (pControl)
	{
		CFont* pFont = &XTP_SAFE_GET2(GetPaintManager(), GetDayViewEventPart(), GetTextFont(), *((CFont*)(NULL)));
		pControl->ShowToolTip(strTip, m_rcTooltip, pFont, TRUE);
	}
}

void CXTPCalendarView::_CalculateEventTimeFormat()
{
	////////////////////////////////////////////////////////////////////////////
	//  LOCALE_STIME    // time separator
	//
	//  LOCALE_ITLZERO  //leading zeros in time field
	//      0 No leading zeros for hours.
	//      1 Leading zeros for hours.
	//
	//  LOCALE_ITIME    // time format specifier
	//      0 AM / PM 12-hour format.
	//      1 24-hour format.
	//
	//  LOCALE_ITIMEMARKPOSN    // time marker position
	//      0 Use as suffix.
	//      1 Use as prefix.
	//
	//  LOCALE_S1159    // AM designator
	//  LOCALE_S2359    // PM designator
	////////////////////////////////////////////////////////////////////////////

	if(m_EventTimeFormat.isCustom == TRUE)
		return;

	CString strTmp;

	CString strTimeSeparator = CXTPCalendarUtils::GetLocaleString(LOCALE_STIME, 16);

	strTmp = CXTPCalendarUtils::GetLocaleString(LOCALE_ITIME, 16);
	BOOL b24_HFormat = _ttoi(strTmp) == 1;

	strTmp = CXTPCalendarUtils::GetLocaleString(LOCALE_ITLZERO, 16);
	BOOL bH_LZero = _ttoi(strTmp) == 1;

	strTmp = CXTPCalendarUtils::GetLocaleString(LOCALE_ITIMEMARKPOSN, 16);
	BOOL bAM_PM_Prefix = _ttoi(strTmp) == 1;

	//---------------------------------------------------------------------------
	CString strTimeFormat;
	strTimeFormat += strTimeSeparator;
	strTimeFormat += _T("mm");

	if (b24_HFormat)
	{
		strTmp = bH_LZero ? _T("HH") : _T("H");
		strTimeFormat = strTmp + strTimeFormat;
	}
	else //12 AM/PM Format
	{
		strTmp = bH_LZero ? _T("hh") : _T("h");
		strTimeFormat = strTmp + strTimeFormat;

		if (bAM_PM_Prefix)
		{
			strTimeFormat = _T("tt ") + strTimeFormat;
		}
		else
		{
			strTimeFormat = strTimeFormat + _T("tt");
		}
	}
	//---------------------------------------------------------------------------
	m_EventTimeFormat.strFormat = strTimeFormat;
	m_EventTimeFormat.b24_HFormat = b24_HFormat;
	m_EventTimeFormat.strAM = CXTPCalendarUtils::GetLocaleString(LOCALE_S1159, 20);
	m_EventTimeFormat.strPM = CXTPCalendarUtils::GetLocaleString(LOCALE_S2359, 20);
}

CString CXTPCalendarView::FormatEventTime(COleDateTime dtTime) const
{
	CString strTime;

	SYSTEMTIME st;
	ZeroMemory(&st, sizeof(st));

	if (CXTPCalendarUtils::GetAsSystemTime(dtTime, st))
	{
		strTime = CXTPCalendarUtils::GetTimeFormat(&st, m_EventTimeFormat.strFormat);
	}

	if (strTime.IsEmpty())
	{
		ASSERT(FALSE);
		strTime.Format(_T("%d:%02d"), dtTime.GetHour(), dtTime.GetMinute());
	}

	//- make AM/PM as lower case ---------------------------------------
	if (!m_EventTimeFormat.b24_HFormat)
	{
		CString strAM_lower = m_EventTimeFormat.strAM;
		CString strPM_lower = m_EventTimeFormat.strPM;

		strAM_lower.MakeLower();
		strPM_lower.MakeLower();

		strTime.Replace(m_EventTimeFormat.strAM, strAM_lower);
		strTime.Replace(m_EventTimeFormat.strPM, strPM_lower);
	}

	return strTime;
}

void CXTPCalendarView::SetCustomTimeFormat(const CString& strFormat)
{
	m_EventTimeFormat.strFormat = strFormat;

	m_EventTimeFormat.isCustom = TRUE;
	m_EventTimeFormat.b24_HFormat = TRUE;
	m_EventTimeFormat.strAM = _T("");
	m_EventTimeFormat.strPM = _T("");
}

void CXTPCalendarView::ResetCustomTimeFormat()
{
	m_EventTimeFormat.isCustom = FALSE;
	_CalculateEventTimeFormat();
}

CString CXTPCalendarView::GetItemTextEventTimeMax() const
{
	COleDateTime dtTestTime;
	dtTestTime.SetTime(23, 59, 59);

	CString strTime = FormatEventTime(dtTestTime);

	DWORD dwFlags = GetCalendarControl()->GetAskItemTextFlags();
	if (dwFlags & (xtpCalendarItemText_EventStartTimeText | xtpCalendarItemText_EventEndTimeText))
	{
		XTP_CALENDAR_GETITEMTEXT_PARAMS objRequest;
		::ZeroMemory(&objRequest, sizeof(objRequest));

		objRequest.nItem = xtpCalendarItemText_EventStartTimeText /*| xtpCalendarItemText_EventEndTimeText*/;
		objRequest.pstrText = &strTime;

		GetCalendarControl()->SendNotificationAlways(XTP_NC_CALENDAR_GETITEMTEXT, (WPARAM)&objRequest, 0);
	}

	return strTime;
}


BOOL CXTPCalendarView::_Undo(CXTPCalendarEvent* pEvent)
{
	if (IsEditingSubject())
		EndEditSubject(xtpCalendarEditSubjectCommit);

	if (!GetCalendarControl())
		return FALSE;

	CXTPAutoResetValue<BOOL> autoSet_UpdateCtrlNf(GetCalendarControl()->m_bUpdateWhenEventChangedNotify);
	autoSet_UpdateCtrlNf = FALSE;

	CXTPCalendarEventPtr ptrEventNew;
	CXTPCalendarEventPtr ptrEventOld;

	if (pEvent)
	{
		ptrEventNew = m_pUndoBuffer->UndoAllForEvent(pEvent);
		ptrEventOld = m_pUndoBuffer->RedoAllForEvent(pEvent);
	}
	else
	{
		CXTPCalendarEventPtr ptrUndoEvent = m_pUndoBuffer->GetFirstUndoEvent();
		if (ptrUndoEvent)
		{
			if (m_eUndoMode == xtpCalendarUndoModeUndo)
			{
				ptrEventNew = m_pUndoBuffer->UndoAllForEvent(ptrUndoEvent);
				ptrEventOld = m_pUndoBuffer->RedoAllForEvent(ptrUndoEvent);
				m_eUndoMode = xtpCalendarUndoModeRedo;
			}
			else
				if (m_eUndoMode == xtpCalendarUndoModeRedo)
				{
					ptrEventNew = m_pUndoBuffer->RedoAllForEvent(ptrUndoEvent);
					ptrEventOld = m_pUndoBuffer->UndoAllForEvent(ptrUndoEvent);
					m_eUndoMode = xtpCalendarUndoModeUndo;
				}
				else
				{
					ASSERT(FALSE);
					return FALSE;
				}
		}
	}

	if (ptrEventNew || ptrEventOld)
	{
		CXTPCalendarControl::CUpdateContext updateContext(m_pControl, xtpCalendarUpdateLayout | xtpCalendarUpdateRedraw);

		if (ptrEventNew && ptrEventOld && ptrEventNew->GetDataProvider())
		{
			VERIFY( ptrEventNew->GetDataProvider()->ChangeEvent(ptrEventNew) );
		}
		else if (ptrEventNew && !ptrEventOld && ptrEventNew->GetDataProvider())
		{
			VERIFY( ptrEventNew->GetDataProvider()->AddEvent(ptrEventNew) );
		}
		else if (!ptrEventNew && ptrEventOld && ptrEventOld->GetDataProvider())
		{
			VERIFY( ptrEventOld->GetDataProvider()->DeleteEvent(ptrEventOld) );
		}

		XTP_SAFE_CALL1(GetCalendarControl(), Populate());

		return TRUE;
	}
	return FALSE;
}

BOOL CXTPCalendarView::CanUndo() const
{
	if (m_eUndoMode == xtpCalendarUndoModeUndo && m_pUndoBuffer->GetCount())
		return TRUE;

	return _CanSubjectEditor(ID_EDIT_UNDO);
}

CWnd* CXTPCalendarView::_GetSubjectEditorWnd() const
{
	if (m_ptrEditingViewEvent)
		return m_ptrEditingViewEvent->GetSubjectEditor();

	return NULL;
}

BOOL CXTPCalendarView::_CanSubjectEditor(UINT idEditCmd) const
{
	CWnd* pSEditorWnd = _GetSubjectEditorWnd();
	if (!pSEditorWnd)
		return FALSE;

	CEdit* pSEditor = DYNAMIC_DOWNCAST(CEdit, pSEditorWnd);
	if (!pSEditor)
		return FALSE;


	BOOL bSEFocused = CWnd::GetFocus() &&
						CWnd::GetFocus()->GetSafeHwnd() == pSEditorWnd->GetSafeHwnd();
	if (!bSEFocused)
		return FALSE;

	int nSel1 = -1, nSel2 = -1;
	pSEditor->GetSel(nSel1, nSel2);

	switch (idEditCmd)
	{
		case ID_EDIT_UNDO:
			return pSEditor->CanUndo();
		case ID_EDIT_CUT:
			return nSel1 < nSel2;
		case ID_EDIT_COPY:
			return nSel1 < nSel2;
		case ID_EDIT_PASTE:
			return ::IsClipboardFormatAvailable(CF_TEXT);
	}

	return FALSE;
}

BOOL CXTPCalendarView::CanRedo() const
{
	return m_eUndoMode == xtpCalendarUndoModeRedo && m_pUndoBuffer->GetCount();
}

BOOL CXTPCalendarView::Undo()
{
	ASSERT(m_eUndoMode == xtpCalendarUndoModeUndo || _CanSubjectEditor(ID_EDIT_UNDO));
	return _Undo();
}

BOOL CXTPCalendarView::Redo()
{
	ASSERT(m_eUndoMode == xtpCalendarUndoModeRedo);
	return _Undo();
}

CString CXTPCalendarView::_GetMaxWidthFormat(CDC* pDC, CString strFormat, BOOL bMonth) const
{
	if (!pDC)
	{
		ASSERT(FALSE);
		return _T("");
	}

	SYSTEMTIME st;
	ZeroMemory(&st, sizeof(st));
	st.wDay = 20;
	st.wMonth = 1;
	st.wYear = (WORD)CXTPCalendarUtils::GetCurrentTime().GetYear();

	int nMax = 0;
	CString str;

	int nCount = bMonth ? 12 : 7;

	for (int i = 0; i < nCount; i++)
	{
		CString strResult = CXTPCalendarUtils::GetDateFormat(&st, strFormat);

		int nLen = pDC->GetTextExtent(strResult).cx;
		if (nMax < nLen)
		{
			str = strResult;
			nMax = nLen;
		}
		if (bMonth)
			st.wMonth++;
		else
			st.wDay++;
	}
	return str;
}

int CXTPCalendarView::_GetMaxWidth(CDC* pDC, CString strFormat) const
{
	if (!pDC)
	{
		ASSERT(FALSE);
		return 1;
	}

	const TCHAR* chFormats[] =
	{
		_T("yyyy"), _T("yyy"), _T("yy"), _T("y"),
		_T("MMMM"), _T("MMM"),
		_T("dddd"), _T("ddd"), _T("dd"), _T("d")
	};

	for (int i = 0; i < _countof(chFormats); i++)
	{
		int nPos = strFormat.Find(chFormats[i]);
		if (nPos >= 0)
		{
			strFormat.Replace(chFormats[i], _GetMaxWidthFormat(pDC, chFormats[i], i < 2));
		}
	}
	return pDC->GetTextExtent(strFormat).cx;
}

void CXTPCalendarView::_RemoveYearPart(CString& strFormat)
{
	strFormat = CXTPCalendarUtils::RemoveFormatPart(strFormat, _T('y'));
}

void CXTPCalendarView::_ReadDefaultHeaderFormats()
{
	m_strDayHeaderFormatDefaultLong = CXTPCalendarUtils::GetLocaleString(LOCALE_SLONGDATE, 81);

	if (!GetCalendarControl()->m_bFullDateMode)
		_RemoveYearPart(m_strDayHeaderFormatDefaultLong);

	m_strDayHeaderFormatDefaultMiddle = m_strDayHeaderFormatDefaultLong;
	m_strDayHeaderFormatDefaultMiddle.Replace(_T("MMMM"), _T("MMM"));

	m_strDayHeaderFormatDefaultShort = m_strDayHeaderFormatDefaultMiddle;
	m_strDayHeaderFormatDefaultShort.Replace(_T("dddd"), _T("ddd"));

	m_strDayHeaderFormatDefaultShortest = CXTPCalendarUtils::GetLocaleString(LOCALE_SSHORTDATE, 81);
	if (!GetCalendarControl()->m_bFullDateMode)
		_RemoveYearPart(m_strDayHeaderFormatDefaultShortest);
}

// nLevel = 0 - Long, 1 - Middle, 2 - Short, 3 - Shortest.
CString CXTPCalendarView::_GetDayHeaderFormat(int nLevel) const
{
	switch (nLevel)
	{
	case 0:
		return m_strDayHeaderFormatLong.IsEmpty() ? m_strDayHeaderFormatDefaultLong : m_strDayHeaderFormatLong;
	case 1:
		return m_strDayHeaderFormatMiddle.IsEmpty() ? m_strDayHeaderFormatDefaultMiddle : m_strDayHeaderFormatMiddle;
	case 2:
		return m_strDayHeaderFormatShort.IsEmpty() ? m_strDayHeaderFormatDefaultShort : m_strDayHeaderFormatShort;
	case 3:
		return m_strDayHeaderFormatShortest.IsEmpty() ? m_strDayHeaderFormatDefaultShortest : m_strDayHeaderFormatShortest;
	}

	ASSERT(FALSE);
	return _T("");
}

void CXTPCalendarView::CalculateHeaderFormat(CDC* pDC, int nWidth, CFont* pTextFont)
{
	if (!GetCalendarControl() || !pTextFont || !pDC)
	{
		ASSERT(FALSE);
		return;
	}

	_ReadDefaultHeaderFormats();

	CXTPFontDC fnt(pDC, pTextFont);
	int arBorder[] = {40, 35, 20, 0};

	for (int i = 0; i <= 3; i++)
	{
		m_strHeaderFormat = _GetDayHeaderFormat(i);
		int nLen = _GetMaxWidth(pDC, m_strHeaderFormat);

		if (nLen < nWidth - arBorder[i])
		{
			return;
		}
	}
}


COleDateTimeSpan CXTPCalendarView::GetEventDurationMin() const
{
	return COleDateTimeSpan(0.);
}

BOOL CXTPCalendarView::CanCut() const
{
	return !IsEditingSubject() && HasSelectedViewEvent() ||
			IsEditingSubject() && _CanSubjectEditor(ID_EDIT_CUT);
}

BOOL CXTPCalendarView::CanCopy() const
{
	return !IsEditingSubject() && HasSelectedViewEvent() ||
			IsEditingSubject() && _CanSubjectEditor(ID_EDIT_COPY);
}

BOOL CXTPCalendarView::CanPaste() const
{
	if (!GetSelection())
	{
		return FALSE;
	}

	//- Register or get existing Clipboard Format ID -------------------------
	UINT uCF_Event = ::RegisterClipboardFormat(XTPCALENDARCTRL_CF_EVENT);

	BOOL bIsEventInClipboard = ::IsClipboardFormatAvailable(uCF_Event);

	return !IsEditingSubject() && bIsEventInClipboard ||
			IsEditingSubject() && _CanSubjectEditor(ID_EDIT_PASTE);
}


BOOL CXTPCalendarView::Cut()
{
	if (IsEditingSubject() && _GetSubjectEditorWnd())
	{
		if (CanCut())
		{
			_GetSubjectEditorWnd()->SendMessage(WM_CUT);
			return TRUE;
		}
		return FALSE;
	}

	//===========================================================================
	CXTPCalendarViewEventsPtr ptrSelViews = GetSelectedEvents();
	if (!ptrSelViews || !ptrSelViews->GetCount())
	{
		return FALSE;
	}

	if (OnBeforeEditOperationNotify(xtpCalendarEO_Cut, ptrSelViews))
	{
		return FALSE;
	}

	//------------------------------------------------------------------------
	DWORD dwCopyFlags = xtpCalendarCopyFlagCutMask;

	COleDataSource* pData = _CopySelectedEvents(dwCopyFlags);
	if (!pData)
	{
		return FALSE;
	}
	pData->SetClipboard();

	int nCount = ptrSelViews->GetCount();
	for (int i = 0; i < nCount; i++)
	{
		CXTPCalendarViewEvent* pViewEvent = ptrSelViews->GetAt(i, FALSE);
		CXTPCalendarEvent* pEvent = pViewEvent ? pViewEvent->GetEvent() : NULL;

		VERIFY( XTP_SAFE_GET2(pEvent, GetDataProvider(), DeleteEvent(pEvent), FALSE) );
	}
	XTP_SAFE_CALL1(GetCalendarControl(), Populate());

	return TRUE;
}

BOOL CXTPCalendarView::Copy()
{
	if (IsEditingSubject() && _GetSubjectEditorWnd())
	{
		if (CanCopy())
		{
			_GetSubjectEditorWnd()->SendMessage(WM_COPY);
			return TRUE;
		}
		return FALSE;
	}

	CXTPCalendarViewEventsPtr ptrSelViews = GetSelectedEvents();
	if (!ptrSelViews || !ptrSelViews->GetCount())
		return FALSE;

	if (OnBeforeEditOperationNotify(xtpCalendarEO_Copy, ptrSelViews))
		return FALSE;

	COleDataSource* pData = _CopySelectedEvents();
	if (!pData)
		return FALSE;

	pData->SetClipboard();

	return TRUE;
}

BOOL CXTPCalendarView::Paste()
{
	if (IsEditingSubject() && _GetSubjectEditorWnd())
	{
		if (CanPaste())
		{
			_GetSubjectEditorWnd()->SendMessage(WM_PASTE);
			return TRUE;
		}
		return FALSE;
	}

	COleDateTime dtSelBegin, dtSelEnd;
	COleDateTimeSpan spSelectionResolution;
	if (!GetSelection(&dtSelBegin, &dtSelEnd, NULL, NULL, &spSelectionResolution))
	{
		return FALSE;
	}
	dtSelBegin = min(dtSelBegin, dtSelEnd);

	// Attach a COleDataObject to the clipboard and paste the data
	COleDataObject objData;
	if (!objData.AttachClipboard())
	{
		return FALSE;
	}

	DWORD dwCopyFlags = 0;
	CXTPCalendarEventsPtr ptrEventsFromCB = _ReadEventsFromClipboard(&objData, &dwCopyFlags);

	if (!ptrEventsFromCB)
		return FALSE;

	if (!GetCalendarControl())
		return FALSE;

	CXTPAutoResetValue<BOOL> autoSet_UpdateCtrlNf(GetCalendarControl()->m_bUpdateWhenEventChangedNotify);
	autoSet_UpdateCtrlNf = FALSE;

	COleDateTimeSpan spStartShift;

	int nCount = ptrEventsFromCB->GetCount();
	for (int i = 0; i < nCount; i++)
	{
		BOOL bUseChangeEvent = FALSE;

		CXTPCalendarEvent* pNewEvent = ptrEventsFromCB->GetAt(i, FALSE);
		if (!pNewEvent)
		{
			ASSERT(FALSE);
			continue;
		}
		int nRState = pNewEvent->GetRecurrenceState();
		if (nRState == xtpCalendarRecurrenceMaster)
		{
			ASSERT(FALSE); // WARNING. Master event in the clipboard - ???
			VERIFY(pNewEvent->RemoveRecurrence());
		}

		if (nRState == xtpCalendarRecurrenceOccurrence &&
			dwCopyFlags & xtpCalendarCopyFlagROccurrence2RException)
		{
			VERIFY(pNewEvent->MakeAsRException());
			bUseChangeEvent = TRUE;
		}
		else
		{
			VERIFY(pNewEvent->RemoveRecurrence());
		}

		// Update recurrence state
		nRState = pNewEvent->GetRecurrenceState();
		if (nRState == xtpCalendarRecurrenceException &&
			dwCopyFlags & xtpCalendarCopyFlagKeepRException)
		{
			CXTPCalendarRecurrencePatternPtr ptrPattern = pNewEvent->GetRecurrencePattern();
			bUseChangeEvent = ptrPattern != NULL;
		}

		if (!bUseChangeEvent && nRState == xtpCalendarRecurrenceException)
		{
			VERIFY( pNewEvent->RemoveRecurrence() );
		}

		COleDateTime dtEventStart = pNewEvent->GetStartTime();

		if (i == 0)
		{
			spStartShift = dtSelBegin - dtEventStart;
			spStartShift += ((double)spStartShift < 0 ? -1 : 1) * XTP_HALF_SECOND;

			double dCells = (double)spStartShift / (double)spSelectionResolution;

			int nCells = (int)dCells; //+ (dCells >= 0 ? 1 : 0);

			double dCellDrobTime = fabs(dCells - (DWORD)dCells)*(double)spSelectionResolution;
			double dCellDrobTimeLimit = XTP_HALF_SECOND*2*10;

			if (dCells >= 0 && dCellDrobTime > dCellDrobTimeLimit)
			{
				nCells++;
			}
			spStartShift = (double)spSelectionResolution * (double)nCells;
		}
		dtEventStart += spStartShift;

		pNewEvent->MoveEvent(dtEventStart);

		if (OnBeforePasteNotify(pNewEvent, bUseChangeEvent))
			continue;

		if (bUseChangeEvent)
		{
			VERIFY( XTP_SAFE_GET2(pNewEvent, GetDataProvider(), ChangeEvent(pNewEvent), FALSE) );
		}
		else
		{
			VERIFY( XTP_SAFE_GET2(pNewEvent, GetDataProvider(), AddEvent(pNewEvent), FALSE) );
		}
	}

	if (dwCopyFlags & xtpCalendarCopyFlagClearCBAfterPaste)
	{
		if (::OpenClipboard(NULL))
		{
			::EmptyClipboard();
			::CloseClipboard();
		}
	}

	XTP_SAFE_CALL1(GetCalendarControl(), Populate());

	return TRUE;
}

CXTPCalendarEventsPtr CXTPCalendarView::_ReadEventsFromClipboard(COleDataObject* pData, DWORD* pdwCopyFlags)
{
	//- Register or get existing Clipboard Format ID -------------------------
	CLIPFORMAT uCF_Event = (CLIPFORMAT)::RegisterClipboardFormat(XTPCALENDARCTRL_CF_EVENT);

	ASSERT(pData);
	if (!pData || !pData->IsDataAvailable(uCF_Event))
	{
		return NULL;
	}

	CXTPCalendarEventsPtr ptrEvents = new CXTPCalendarEvents();
	if (!ptrEvents)
	{
		return NULL;
	}

	const int cErrTextSize = 1024;
	TCHAR szErrText[cErrTextSize + 1];

	try
	{
		HGLOBAL hData = pData->GetGlobalData(uCF_Event);
		if (!hData)
		{
			return NULL;
		}

		CSharedFile fileSahred;
		fileSahred.SetHandle(hData, FALSE);
		CArchive ar(&fileSahred, CArchive::load);

		CXTPPropExchangeArchive px(ar);

		long nSchema = 0;
		DWORD dwCopyFlags = 0;

		PX_Long(&px, _T("Version"), (long&)nSchema);
		px.ExchangeLocale();
		PX_DWord(&px, _T("CopyFlags"), (DWORD&)dwCopyFlags);

		if (nSchema != XTP_CLIPBOARD_EVENTS_DATA_VER)
		{
			ASSERT(FALSE);
			return NULL;
		}

		CXTPPropExchangeSection sec(px.GetSection(_T("Events")));
		CXTPPropExchangeEnumeratorPtr pEnumerator(sec->GetEnumerator(_T("Event")));
		POSITION pos = pEnumerator->GetPosition();

		UINT uScheduleID = 0;
		CXTPCalendarData* pDPsel = _GetDataProviderBySelection(&uScheduleID);

		while (pos)
		{
			CXTPPropExchangeSection secEvent(pEnumerator->GetNext(pos));

			CXTPCalendarData* pDPevent = NULL;

			//-- get data provider --------------------
			CString strDataProvider;
			PX_String(&secEvent, _T("DataProvider_ConnectionString"), strDataProvider, _T(""));

			if (!strDataProvider.IsEmpty())
				pDPevent = _GetDataProviderByConnStr(strDataProvider);

			CXTPCalendarData* pDataProvider = pDPevent ? pDPevent : pDPsel;
			ASSERT(pDataProvider);

			//-- create event and read its data ---------
			CXTPCalendarEventPtr ptrEvent = pDataProvider ? pDataProvider->CreateNewEvent() : NULL;
			if (!ptrEvent)
				return NULL;

			ptrEvent->DoPropExchange(&secEvent);

			if (!pDPevent
				|| !_IsScheduleVisible(pDPevent, ptrEvent->GetScheduleID()))
				ptrEvent->SetScheduleID(uScheduleID);

			ptrEvents->Add(ptrEvent, TRUE);
		}
		ar.Close(); // detach from file

		if (pdwCopyFlags)
			*pdwCopyFlags = dwCopyFlags;

		return ptrEvents;
	}
	catch(CArchiveException* pE)
	{
		if (pE->GetErrorMessage(szErrText, cErrTextSize))
		{
			TRACE(_T("EXCEPTION: CXTPCalendarView::_ReadEventsFromClipboard() - %s\n"), szErrText);
		}
		pE->Delete();
	}
	catch(CFileException* pE)
	{
		if (pE->GetErrorMessage(szErrText, cErrTextSize))
		{
			TRACE(_T("EXCEPTION: CXTPCalendarView::_ReadEventsFromClipboard() - %s\n"), szErrText);
		}
		pE->Delete();
	}
	catch(...)
	{
		TRACE(_T("EXCEPTION: CXTPCalendarView::_ReadEventsFromClipboard() - Unhandled Exception!\n"));
	}

	ASSERT(FALSE);
	return NULL;
}

COleDataSource* CXTPCalendarView::_CopySelectedEvents(DWORD dwCopyFlags)
{
	CXTPCalendarViewEventsPtr ptrSelViews = GetSelectedEvents();
	if (!ptrSelViews || !ptrSelViews->GetCount())
		return NULL;

	const int cErrTextSize = 1024;
	TCHAR szErrText[cErrTextSize + 1];

	try
	{
		CSharedFile fileSahred(GMEM_MOVEABLE | GMEM_DDESHARE | GMEM_ZEROINIT);
		CArchive ar(&fileSahred, CArchive::store);

		CXTPPropExchangeArchive px(ar);

		long nSchema = XTP_CLIPBOARD_EVENTS_DATA_VER;
		PX_Long(&px, _T("Version"), (long&)nSchema);
		px.ExchangeLocale();
		PX_DWord(&px, _T("CopyFlags"), (DWORD&)dwCopyFlags);

		int nCount = ptrSelViews->GetCount();

		CXTPPropExchangeSection sec(px.GetSection(_T("Events")));
		CXTPPropExchangeEnumeratorPtr pEnumerator(sec->GetEnumerator(_T("Event")));
		POSITION posEnum = pEnumerator->GetPosition((DWORD)nCount);

		for (int i = 0; i < nCount; i++)
		{
			CXTPCalendarViewEvent* pViewEvent = ptrSelViews->GetAt(i, FALSE);
			CXTPCalendarEvent* pEvent = pViewEvent->GetEvent();
			if (!pEvent)
			{
				ASSERT(FALSE);
				continue;
			}

			CXTPPropExchangeSection secEvent(pEnumerator->GetNext(posEnum));

			CString strDataProvider = XTP_SAFE_GET2(pEvent, GetDataProvider(), GetConnectionString(), _T(""));
			PX_String(&secEvent, _T("DataProvider_ConnectionString"), strDataProvider);

			pEvent->DoPropExchange(&secEvent);
		}
		ar.Close(); // perform Flush() and detach from file

		//- Prepare Global data ----------------------------------------------
		DWORD dwDataSize = (DWORD)fileSahred.GetLength();
		HGLOBAL hData = fileSahred.Detach();
		if (!hData)
		{
			ASSERT(FALSE);
			return NULL;
		}

		hData = ::GlobalReAlloc(hData, dwDataSize, GMEM_MOVEABLE | GMEM_DDESHARE | GMEM_ZEROINIT);
		if (!hData)
		{
			ASSERT(FALSE);
			return NULL;
		}

		//- Register Clipboard Format ----------------------------------------
		CLIPFORMAT uCF_Event = (CLIPFORMAT)::RegisterClipboardFormat(XTPCALENDARCTRL_CF_EVENT);

		// Cache data
		COleDataSource* pSource = new COleDataSource();
		if (pSource)
		{
			pSource->CacheGlobalData(uCF_Event, hData);
		}

		return pSource;
	}
	catch(CArchiveException* pE)
	{
		if (pE->GetErrorMessage(szErrText, cErrTextSize))
		{
			TRACE(_T("EXCEPTION: CXTPCalendarView::_CopySelectedEvents() - %s\n"), szErrText);
		}
		pE->Delete();
	}
	catch(CFileException* pE)
	{
		if (pE->GetErrorMessage(szErrText, cErrTextSize))
		{
			TRACE(_T("EXCEPTION: CXTPCalendarView::_CopySelectedEvents() - %s\n"), szErrText);
		}
		pE->Delete();
	}
	catch(...)
	{
		TRACE(_T("EXCEPTION: CXTPCalendarView::_CopySelectedEvents() - Unhandled Exception!\n"));
	}

	ASSERT(FALSE);
	return NULL;
}

////////////////////////////////////////////////////////////////////////////
CXTPCalendarPaintManager* CXTPCalendarView::GetPaintManager() const
{
	ASSERT(m_pControl);
	return m_pControl ? m_pControl->GetPaintManager() : NULL;
}

CXTPCalendarTheme* CXTPCalendarView::GetTheme() const
{
	return m_pControl ? m_pControl->GetTheme() : NULL;

}

void CXTPCalendarView::SendNotification(XTP_NOTIFY_CODE EventCode, WPARAM wParam , LPARAM lParam)
{
	if (m_pControl)
		m_pControl->SendNotification(EventCode, wParam, lParam);
}

BOOL CXTPCalendarView::IsEditOperationDisabledNotify(XTPCalendarEditOperation eOperation, CXTPCalendarViewEvent* pEventView) const
{
	return _EditOperationNotify(XTP_NC_CALENDAR_IS_EVENT_EDIT_OPERATION_DISABLED,
								eOperation, pEventView);
}

BOOL CXTPCalendarView::OnBeforeEditOperationNotify(XTPCalendarEditOperation eOperation, CXTPCalendarViewEvent* pEventView)
{
	if (IsEditOperationDisabledNotify(eOperation, pEventView))
		return TRUE;

	return _EditOperationNotify(XTP_NC_CALENDAR_BEFORE_EVENT_EDIT_OPERATION,
								eOperation, pEventView);
}

BOOL CXTPCalendarView::OnBeforeEditOperationNotify(XTPCalendarEditOperation eOperation, CXTPCalendarViewEvents* pEventViews)
{
	if (_EditOperationNotify(XTP_NC_CALENDAR_IS_EVENT_EDIT_OPERATION_DISABLED,
							eOperation, NULL, pEventViews))
		return TRUE;

	return _EditOperationNotify(XTP_NC_CALENDAR_BEFORE_EVENT_EDIT_OPERATION,
			eOperation, NULL, pEventViews);
}

BOOL CXTPCalendarView::OnBeforePasteNotify(CXTPCalendarEvent* pEvent, BOOL bPasteWillChangeExistingEvent)
{
	XTP_EVENT_PASTE_OPERATION_PARAMS opPasteParams;
	::ZeroMemory(&opPasteParams, sizeof(opPasteParams));

	opPasteParams.eOperation = xtpCalendarEO_Paste;

	opPasteParams.pEvent = pEvent;
	opPasteParams.bPasteWillChangeExistingEvent = bPasteWillChangeExistingEvent;

	BOOL bDisabledHandled = FALSE;

	if (m_pControl)
	{
		m_pControl->SendNotification(XTP_NC_CALENDAR_IS_EVENT_EDIT_OPERATION_DISABLED,
			(WPARAM)&opPasteParams, (LPARAM)&bDisabledHandled);
	}

	if (bDisabledHandled)
		return TRUE;

	if (m_pControl)
	{
		m_pControl->SendNotification(XTP_NC_CALENDAR_BEFORE_EVENT_EDIT_OPERATION,
			(WPARAM)&opPasteParams, (LPARAM)&bDisabledHandled);
	}

	return bDisabledHandled;
}

BOOL CXTPCalendarView::OnInPlaceCreateEvent(LPCTSTR pcszInitialSubject)
{
	XTP_EVENT_IN_PLACE_CREATE_EVENT_OPERATION_PARAMS opCrParams;
	::ZeroMemory(&opCrParams, sizeof(opCrParams));

	opCrParams.eOperation = xtpCalendarEO_InPlaceCreateEvent;
	opCrParams.pcszInitialSubject = pcszInitialSubject;

	BOOL bDisabledHandled = FALSE;

	if (m_pControl)
	{
		m_pControl->SendNotification(XTP_NC_CALENDAR_IS_EVENT_EDIT_OPERATION_DISABLED,
									(WPARAM)&opCrParams, (LPARAM)&bDisabledHandled);
	}

	if (bDisabledHandled)
		return TRUE;

	if (m_pControl)
	{
		m_pControl->SendNotification(XTP_NC_CALENDAR_BEFORE_EVENT_EDIT_OPERATION,
									(WPARAM)&opCrParams, (LPARAM)&bDisabledHandled);
	}

	return bDisabledHandled;
}

CXTPCalendarOptions* CXTPCalendarView::GetCalendarOptions() const
{
	return this && m_pControl ? m_pControl->GetCalendarOptions() : NULL;
}

BOOL CXTPCalendarView::_EditOperationNotify(XTP_NOTIFY_CODE ncEvent,
							XTPCalendarEditOperation eOperation,
							CXTPCalendarViewEvent* pEventView,
							CXTPCalendarViewEvents* pEventViews) const
{
	ASSERT(pEventView || pEventViews && !(pEventView && pEventViews));

	XTP_EVENT_EDIT_OPERATION_PARAMS opParams;
	::ZeroMemory(&opParams, sizeof(opParams));

	CXTPCalendarViewEvents arEvents;
	arEvents.Add(pEventView, TRUE);

	opParams.eOperation = eOperation;
	opParams.pEventViews = pEventView ? &arEvents : pEventViews;

	BOOL bDisabledHandled = FALSE;

	if (m_pControl)
	{
		m_pControl->SendNotification(ncEvent, (WPARAM)&opParams, (LPARAM)&bDisabledHandled);
	}

	return bDisabledHandled;
}

XTPEnumCalendarUpdateResult CXTPCalendarView::OnEventChanged_InDataProvider(
								XTP_NOTIFY_CODE nfCode, CXTPCalendarEvent* pEvent)
{
	if (!pEvent)
		return xtpCalendar_Skip;

	if (nfCode != XTP_NC_CALENDAREVENTWASADDED &&
		nfCode != XTP_NC_CALENDAREVENTWASDELETED &&
		nfCode != XTP_NC_CALENDAREVENTWASCHANGED)
		return xtpCalendar_Skip;

	int nDaysCount = GetViewDayCount();
	COleDateTime dtFirstDay = nDaysCount ?  XTP_SAFE_GET1(GetViewDay_(0), GetDayDate(), (DATE)0) : (DATE)0;
	COleDateTime dtLastDay = nDaysCount ?  XTP_SAFE_GET1(GetViewDay_(nDaysCount - 1), GetDayDate(), (DATE)0) : (DATE)0;

	if (nfCode == XTP_NC_CALENDAREVENTWASADDED ||
		nfCode == XTP_NC_CALENDAREVENTWASDELETED)
	{
		if (GetDateTimeDay(pEvent->GetStartTime(), pEvent->IsAllDayEvent()) > dtLastDay ||
			GetDateTimeDay(pEvent->GetEndTime(), pEvent->IsAllDayEvent()) < dtFirstDay)
			return xtpCalendar_Skip;

		return xtpCalendar_Populate;
	}

	return xtpCalendar_Populate;

	//  *****
	//  An Idea how changing event processing can be optimized
	//  from xtpCalendar_Populate to xtpCalendar_Redraw or even xtpCalendar_Skip.
//
//  int nRState_new = pEvent->GetRecurrenceState();
//  if (nRState_new == xtpCalendarRecurrenceMaster) {
//      return xtpCalendar_Populate;
//  }
//
//  //-----------------------------------------------------------------------
//  BOOL bUpdateEvent = FALSE;
//  int nDays = GetViewDayCount();
//  for (int nDay = 0; nDay < nDays; nDay++)
//  {
//      CXTPCalendarViewDay* pDay = GetViewDay_(nDay);
//      ASSERT(pDay);
//      if (!pDay)
//          continue;
//
//
//      int nEvents = pDay->GetViewEventsCount();
//      for (int nEvent = 0; nEvent < nEvents; nEvent++)
//      {
//          CXTPCalendarViewEvent* pEventView = pDay->GetViewEvent_(nEvent);
//          if (!pEventView && !pEventView->GetEvent())
//          {
//              ASSERT(FALSE);
//              continue;
//          }
//          CXTPCalendarEvent* pEvent_I = pEventView->GetEvent();
//          if (pEvent_I->GetEventID() == dwEventID)
//          {
//              if (!bUpdateEvent)
//              {
//                  if (pEvent_I->IsEqualStartEnd(pEvent))
//                      bUpdateEvent = TRUE;
//                  else
//                      return xtpCalendar_Populate;
//              }
//
//              if (bUpdateEvent)
//              {
//                  int nRState_I = pEvent_I->GetRecurrenceState();
//                  //int nRState_new = pEvent->GetRecurrenceState();
//
//                  if (nRState_I == xtpCalendarRecurrenceOccurrence)
//                      pEvent_I->SetRecurrenceState_ExceptionOccurrence(xtpCalendarRecurrenceException);
//
//                  pEvent_I->Update(pEvent);
//
//                  if (nRState_new == xtpCalendarRecurrenceOccurrence ||
//                      nRState_new == xtpCalendarRecurrenceException)
//                  {
//                      ASSERT(nRState_I == xtpCalendarRecurrenceOccurrence ||
//                              nRState_I == xtpCalendarRecurrenceException);
//
//                      pEvent_I->SetRecurrenceState_ExceptionOccurrence((XTPCalendarEventRecurrenceState)nRState_new);
//                  }
//              }
//          }
//      }
//
//  }
//
//  //-----------------------------------------------------------------------
//  if (!bUpdateEvent &&
//      !(CXTPCalendarUtils::ResetTime(pEvent->GetStartTime()) > dtLastDay ||
//        CXTPCalendarUtils::ResetTime(pEvent->GetEndTime()) < dtFirstDay)
//     )
//  {
//      return xtpCalendar_Populate;
//  }
//  return bUpdateEvent ? xtpCalendar_Redraw : xtpCalendar_Skip;
}

CXTPCalendarData* CXTPCalendarView::_GetDataProviderBySelection(UINT* puScheduleID)
{
	COleDateTime dtSelBegin;
	int nGroup = 0;
	if (!GetSelection(&dtSelBegin, NULL, NULL, &nGroup))
		return NULL;

	CXTPCalendarViewDay* pViewDay = _GetViewDay(dtSelBegin);
	if (!pViewDay)
		return NULL;

	CXTPCalendarData* pData = NULL;
	CXTPCalendarViewGroup* pViewGroup = NULL;

	if (pViewDay && nGroup < pViewDay->GetViewGroupsCount())
	{
		pViewGroup = pViewDay->GetViewGroup_(nGroup);
		if (pViewGroup)
		{
			pData = pViewGroup->GetDataProvider();
			if (puScheduleID)
				*puScheduleID = pViewGroup->GetScheduleID();
		}
	}

	return pData;
}

CXTPCalendarData* CXTPCalendarView::_GetDataProviderByConnStr(LPCTSTR pcszConnStr, BOOL bCompareNoCase)
{
	CXTPCalendarViewDay* pViewDay = GetViewDay_(0);

	COleDateTime dtSelBegin;
	if (GetSelection(&dtSelBegin))
		pViewDay = _GetViewDay(dtSelBegin);

	if (!pViewDay)
		return NULL;

	int nGCount = pViewDay->GetViewGroupsCount();
	for (int g = 0; g < nGCount; g++)
	{
		CXTPCalendarViewGroup* pViewGroup = pViewDay->GetViewGroup_(g);
		if (pViewGroup)
		{
			CXTPCalendarData* pData = pViewGroup->GetDataProvider();
			if (pData)
			{
				CString strConn_i = pData->GetConnectionString();
				int nCmp = bCompareNoCase ? strConn_i.CompareNoCase(pcszConnStr) : strConn_i.Compare(pcszConnStr);
				if (nCmp == 0)
					return pData;
			}
		}
	}

	return NULL;
}

BOOL CXTPCalendarView::_IsScheduleVisible(CXTPCalendarData* pDataProvider, UINT uScheduleID)
{
	CXTPCalendarViewDay* pViewDay = GetViewDay_(0);

	COleDateTime dtSelBegin;
	if (GetSelection(&dtSelBegin))
		pViewDay = _GetViewDay(dtSelBegin);

	if (!pViewDay)
		return FALSE;

	int nGCount = pViewDay->GetViewGroupsCount();
	for (int g = 0; g < nGCount; g++)
	{
		CXTPCalendarViewGroup* pViewGroup = pViewDay->GetViewGroup_(g);

		CXTPCalendarData* pData = pViewGroup ? pViewGroup->GetDataProvider() : NULL;
		if (pData && pData == pDataProvider)
		{
			int nRCCount = XTP_SAFE_GET2(pViewGroup, GetResources(), GetCount(), 0);
			for (int i = 0; i < nRCCount; i++)
			{
				if (XTP_SAFE_GET3(pViewGroup, GetResources(), GetAt(i), ExistsScheduleID(uScheduleID), FALSE))
					return TRUE;
			}
		}
	}

	return FALSE;
}


CXTPCalendarWMHandler* CXTPCalendarView::GetChildHandlerAt(int nIndex) const
{
	CXTPCalendarWMHandler* pDay = GetViewDay_(nIndex);
	if (!pDay)
		return NULL;

	ASSERT_KINDOF(CXTPCalendarWMHandler, pDay);
	return pDay;
}

CXTPCalendarResources* CXTPCalendarView::GetResources() const
{
	ASSERT(this);
	if (m_pResources)
		return m_pResources;

	return GetCalendarControl() ? GetCalendarControl()->GetResources() : NULL;
}

void CXTPCalendarView::SetResources(CXTPCalendarResources* pResources)
{
	BOOL bChanged = TRUE;

	if (!pResources)
	{
		bChanged = m_pResources != NULL;
		CMDTARGET_RELEASE(m_pResources);
	}
	else
	{
		if (!m_pResources)
			m_pResources = new CXTPCalendarResources();

		if (!m_pResources)
			return;

		// copy data to allow using self as input parameter
		CXTPCalendarResources arRes;
		arRes.Append(pResources);

		m_pResources->RemoveAll();
		m_pResources->Append(&arRes);
	}

	if (bChanged)
		SendNotification(XTP_NC_CALENDAR_RESOURCES_WHERE_CHANGED, 1, (LPARAM)this);
}

BOOL CXTPCalendarView::IsTodayVisible() const
{
	COleDateTime dtToday = CXTPCalendarUtils::GetCurrentTime();
	dtToday = GetDateTimeDay(dtToday, FALSE);

	int nCount = GetViewDayCount();
	for (int i = 0; i < nCount; i++)
	{
		COleDateTime dtDay = GetViewDayDate(i);
		if (CXTPCalendarUtils::IsEqual(dtDay, dtToday))
			return TRUE;
	}

	return FALSE;
}


void CXTPCalendarView::EmptyUndoBuffer()
{
	m_pUndoBuffer->RemoveAll();
	m_eUndoMode = xtpCalendarUndoModeUnknown;
}

BOOL CXTPCalendarView::HitTest(CPoint pt, XTP_CALENDAR_HITTESTINFO* pHitTest) const
{
	return HitTestEx(pt, pHitTest);
}

BOOL CXTPCalendarView::HitTestEx(CPoint pt, XTP_CALENDAR_HITTESTINFO* pHitTest) const
{
	if (!pHitTest)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	int nCount = GetViewDayCount();
	for (int i = 0; i < nCount; i++)
	{
		CXTPCalendarViewDay* pViewDay = GetViewDay_(i);
		ASSERT(pViewDay);
		if (pViewDay && pViewDay->HitTestEx(pt, pHitTest))
		{
			ASSERT(!pHitTest->pViewDay || pHitTest->pViewDay == pViewDay);

			pHitTest->nDay = i;
			pHitTest->pViewDay = pViewDay;
			return TRUE;
		}
	}
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
CXTPCalendarView::CSelectionChanged_ContextData::CSelectionChanged_ContextData()
{
	m_nLockCount = 0;
	m_nLockCount_Day = 0;
	m_nLockCount_Event = 0;

	m_bRequest_Day = FALSE;
	m_bRequest_Event = FALSE;
}

/////////////////////////////////////////////////////////////////////////////
//  class CSelectionChangedContext
//
CXTPCalendarView::CSelectionChangedContext::CSelectionChangedContext(CXTPCalendarView* pView, int eSelType)
{
	if (!pView)
	{
		ASSERT(FALSE);
		return;
	}
	m_pView = pView;
	m_eSelType = eSelType;

	m_pView->m_cntSelChanged.m_nLockCount += eSelType == xtpCalendarSelectionUnknown ? 1 : 0;
	m_pView->m_cntSelChanged.m_nLockCount_Day += eSelType == xtpCalendarSelectionDays ? 1 : 0;
	m_pView->m_cntSelChanged.m_nLockCount_Event += eSelType == xtpCalendarSelectionEvents ? 1 : 0;
}

CXTPCalendarView::CSelectionChangedContext::~CSelectionChangedContext()
{
	if (!m_pView)
	{
		ASSERT(FALSE);
		return;
	}
	if (m_eSelType == xtpCalendarSelectionUnknown)
		m_pView->m_cntSelChanged.m_nLockCount--;

	if (m_eSelType == xtpCalendarSelectionDays)
	{
		m_pView->m_cntSelChanged.m_nLockCount_Day--;

		if (m_pView->m_cntSelChanged.m_nLockCount_Day == 0)
			m_pView->m_cntSelChanged.m_bRequest_Day = TRUE;
	}
	if (m_eSelType == xtpCalendarSelectionEvents)
	{
		m_pView->m_cntSelChanged.m_nLockCount_Event--;

		if (m_pView->m_cntSelChanged.m_nLockCount_Event == 0)
			m_pView->m_cntSelChanged.m_bRequest_Event = TRUE;
	}

	if (m_pView->m_cntSelChanged.m_nLockCount == 0 && m_pView->m_cntSelChanged.m_bRequest_Day)
	{
		m_pView->m_cntSelChanged.m_bRequest_Day = FALSE;
		m_pView->SendNotification(XTP_NC_CALENDAR_SELECTION_CHANGED, (WPARAM)xtpCalendarSelectionDays);
	}
	if (m_pView->m_cntSelChanged.m_nLockCount == 0 && m_pView->m_cntSelChanged.m_bRequest_Event)
	{
		m_pView->m_cntSelChanged.m_bRequest_Event = FALSE;
		m_pView->SendNotification(XTP_NC_CALENDAR_SELECTION_CHANGED, (WPARAM)xtpCalendarSelectionEvents);
	}
}

/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CXTPCalendarWMHandler
CXTPCalendarWMHandler::CXTPCalendarWMHandler()
{
}

int CXTPCalendarWMHandler::GetChildHandlersCount() const
{
	return 0;
}

CXTPCalendarWMHandler* CXTPCalendarWMHandler::GetChildHandlerAt(int nIndex) const
{
	UNREFERENCED_PARAMETER(nIndex);
	return NULL;
}

BOOL CXTPCalendarWMHandler::OnLButtonDown(UINT nFlags, CPoint point)
{
	BOOL bHandled = FALSE;

	int nCount = GetChildHandlersCount();
	for (int i = 0; i < nCount; i++)
	{
		CXTPCalendarWMHandler* pChild = GetChildHandlerAt(i);
		if (pChild && pChild->OnLButtonDown(nFlags, point))
		{
			bHandled = TRUE;
			break;
		}
	}

	return bHandled;
}

BOOL CXTPCalendarWMHandler::OnLButtonUp(UINT nFlags, CPoint point)
{
	BOOL bHandled = FALSE;

	int nCount = GetChildHandlersCount();
	for (int i = 0; i < nCount; i++)
	{
		CXTPCalendarWMHandler* pChild = GetChildHandlerAt(i);
		if (pChild && pChild->OnLButtonUp(nFlags, point))
		{
			bHandled = TRUE;
			break;
		}
	}

	return bHandled;
}

BOOL CXTPCalendarWMHandler::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	BOOL bHandled = FALSE;

	int nCount = GetChildHandlersCount();
	for (int i = 0; i < nCount; i++)
	{
		CXTPCalendarWMHandler* pChild = GetChildHandlerAt(i);
		if (pChild && pChild->OnLButtonDblClk(nFlags, point))
		{
			bHandled = TRUE;
			break;
		}
	}

	return bHandled;
}

void CXTPCalendarWMHandler::OnMouseMove(UINT nFlags, CPoint point)
{
	int nCount = GetChildHandlersCount();
	for (int i = 0; i < nCount; i++)
	{
		CXTPCalendarWMHandler* pChild = GetChildHandlerAt(i);
		if (pChild)
			pChild->OnMouseMove(nFlags, point);
	}
}

void CXTPCalendarWMHandler::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	int nCount = GetChildHandlersCount();
	for (int i = 0; i < nCount; i++)
	{
		CXTPCalendarWMHandler* pChild = GetChildHandlerAt(i);
		if (pChild)
			pChild->OnChar(nChar, nRepCnt, nFlags);
	}
}

void CXTPCalendarWMHandler::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	int nCount = GetChildHandlersCount();
	for (int i = 0; i < nCount; i++)
	{
		CXTPCalendarWMHandler* pChild = GetChildHandlerAt(i);
		if (pChild)
			pChild->OnKeyDown(nChar, nRepCnt, nFlags);
	}
}

void CXTPCalendarWMHandler::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	int nCount = GetChildHandlersCount();
	for (int i = 0; i < nCount; i++)
	{
		CXTPCalendarWMHandler* pChild = GetChildHandlerAt(i);
		if (pChild)
			pChild->OnKeyUp(nChar, nRepCnt, nFlags);
	}
}

BOOL CXTPCalendarWMHandler::OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	BOOL bHandled = FALSE;

	int nCount = GetChildHandlersCount();
	for (int i = 0; i < nCount; i++)
	{
		CXTPCalendarWMHandler* pChild = GetChildHandlerAt(i);
		if (pChild && pChild->OnSysKeyDown(nChar, nRepCnt, nFlags))
		{
			bHandled = TRUE;
			break;
		}
	}

	return bHandled;
}


BOOL CXTPCalendarWMHandler::OnTimer(UINT_PTR uTimerID)
{
	int nCount = GetChildHandlersCount();
	for (int i = 0; i < nCount; i++)
	{
		CXTPCalendarWMHandler* pChild = GetChildHandlerAt(i);
		if (pChild && pChild->OnTimer(uTimerID))
			return TRUE;
	}

	return FALSE;
}

void CXTPCalendarWMHandler::OnPostAdjustLayout()
{
	int nCount = GetChildHandlersCount();
	for (int i = 0; i < nCount; i++)
	{
		CXTPCalendarWMHandler* pChild = GetChildHandlerAt(i);
		if (pChild)
			pChild->OnPostAdjustLayout();
	}
}

void CXTPCalendarWMHandler::OnFinalRelease()
{
	OnBeforeDestroy();
	CCmdTarget::OnFinalRelease();
}

void CXTPCalendarWMHandler::OnBeforeDestroy()
{
	int nCount = GetChildHandlersCount();
	for (int i = 0; i < nCount; i++)
	{
		CXTPCalendarWMHandler* pChild = GetChildHandlerAt(i);
		if (pChild)
			pChild->OnBeforeDestroy();
	}
}
