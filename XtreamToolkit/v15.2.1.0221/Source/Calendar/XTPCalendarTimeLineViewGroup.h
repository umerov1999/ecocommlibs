// XTPCalendarTimeLineViewGroup.h: interface for the CXTPCalendarTimeLineViewGroup class.
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

//{{AFX_CODEJOCK_PRIVATE
#if !defined(__XTPCALENDARTIMELINEVIEWGROUP_H__)
#define __XTPCALENDARTIMELINEVIEWGROUP_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning (disable: 4100)


class CXTPCalendarEvent;
class CXTPCalendarTimeLineView;
class CXTPCalendarTimeLineViewGroup;
class CXTPCalendarTimeLineViewGroups;
class CXTPCalendarTimeLineViewEvent;
class CXTPCalendarTimeLineViewPart;

//===========================================================================
// Summary:
//  This class represents an event timeline view portion of the CXTPCalendarViewEvent.
// Remarks:
//  It represents a specific view of the event associated data according
//  to various view types and display settings and provides basic
//  functionality on this data using user input through keyboard and mouse.
//  CXTPCalendarTimeLineViewEvent is a descendant of the CXTPCalendarViewEvent class.
//===========================================================================
class _XTP_EXT_CLASS CXTPCalendarTimeLineViewEvent : public CXTPCalendarViewEvent
{
public:

	// -----------------------------
	// Summary:
	//     Default object constructor.
	// Parameters:
	//     pEvent      - A pointer to a CXTPCalendarEvent object.
	//     pGroup    - A pointer to a CXTPCalendarTimeLineViewGroup object.
	// -----------------------------
	CXTPCalendarTimeLineViewEvent(CXTPCalendarEvent* pEvent, CXTPCalendarTimeLineViewGroup* pGroup);

	// ------------------------
	// Summary:
	//     This member function is used to calculate rectangle for event
	// Parameters:
	//     pDC - A pointer to a valid device context.
	// ------------------------
	void CalcRect(CDC* pDC);

	// -----------------------------
	// Summary:
	//     This member function is used to obtain the owner view object.
	// Returns:
	//     A CXTPCalendarTimeLineView pointer to the owner view object.
	// -----------------------------
	CXTPCalendarTimeLineView* GetView() const;

	// -----------------------------
	// Summary:
	//     access function to get view group
	// Returns:
	//     current view group as pointer to CXTPCalendarTimeLineViewGroup
	// -----------------------------
	CXTPCalendarTimeLineViewGroup* GetViewGroup() const
	{
		return m_pGroup;
	}

	// -----------------------------
	// Summary:
	//     access function to get view group
	// Returns:
	//     current view group as pointer to CXTPCalendarTimeLineViewGroup
	// -----------------------------
	virtual CXTPCalendarViewGroup* GetViewGroup_() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to determine which view item,
	//     if any, is at a specified position index, and returns additional
	//     info in a XTP_CALENDAR_HITTESTINFO struct.
	// Parameters:
	//     pt       - A CPoint that contains the coordinates of the point to
	//                test.
	//     pHitTest - A pointer to a XTP_CALENDAR_HITTESTINFO structure that
	//                contains information about the point to test.
	// Returns:
	//     A BOOL. TRUE if the item is found. FALSE otherwise.
	// See Also: XTP_CALENDAR_HITTESTINFO
	//-----------------------------------------------------------------------
	virtual BOOL HitTest(CPoint pt, XTP_CALENDAR_HITTESTINFO* pHitTest) const { return FALSE; };
	virtual BOOL HitTestEx(CPoint pt, XTP_CALENDAR_HITTESTINFO* pHitTest) const {return FALSE; }

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to get is group visible.
	// Returns:
	//     TRUE if object is visible, FALSE otherwise.
	//-----------------------------------------------------------------------
	virtual BOOL IsVisible() const { return FALSE; };

	// -----------------
	// Summary:
	//     This member function is used to get is group selected.
	// Returns:
	//     TRUE if object is selected, FALSE otherwise.
	// -----------------
	virtual BOOL IsSelected() const;

	// --------------------------------------
	// Summary:
	//     This member function is used to adjust the view's layout
	// Parameters:
	//  pDC - A pointer to a valid device context.
	//  rcEventMax - A CRect object that contains the rectangle coordinates
	//               that are used to draw the view.
	//  nEventPlaceNumber - ordinal number of event
	// --------------------------------------
	virtual void AdjustLayout(CDC* pDC, const CRect& rcEventMax, int nEventPlaceNumber);

	// --------------------------------------
	// Summary:
	//     This member function is used to adjust the view's layout with theme used
	// Parameters:
	//  pDC - A pointer to a valid device context.
	//  rcEventMax - A CRect object that contains the rectangle coordinates
	//               that are used to draw the view.
	//  nEventPlaceNumber - ordinal number of event
	// --------------------------------------
	virtual void AdjustLayout2(CDC* pDC, const CRect& rcEventMax, int nEventPlaceNumber)
	{
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to draw the view content using
	//     the specified device context.
	// Parameters:
	//     pDC - A pointer to a valid device context.
	virtual void Draw(CDC* pDC);

	// ------------------------------
	// Summary:
	//     specific for Timeline view function to organized view in logical order
	// Parameters:
	//     nNewPlace :  ordinal number to use
	// ------------------------------
	virtual void ChangeEventPlace(int nNewPlace);

	// -----------------
	// Summary:
	//  access function to get visual part of object
	// Returns:
	//  pointer to CXTPCalendarTimeLineViewPart object
	// -----------------
	virtual CXTPCalendarTimeLineViewPart* GetPart() const;

	// -----------------
	// Summary:
	//  access function to get event duration in pixels
	// Returns:
	//     event duration in pixels as int
	// -----------------
	virtual int GetEventDurationInPixels() const;

	// -----------------
	// Summary:
	//  Returns the rect in client coordinates into which the event was drawn last time
	// Note
	//  GetLastVisibleRect and SetLastVisibleRect are workaround functions for HitTest in Timeline view
	//  The SetLastVisibleRects called by draw operation each time the event is drawn in timeline view
	//  The GetLastVisibleRect is used in HitTest function to determine the hittest info
	// -----------------
	const CRect& GetLastVisibleRect() const;
	
	// -----------------
	// Summary:
	//  Sets the rect in client coordinates into which the event was drawn last time
	// Note
	//  GetLastVisibleRect and SetLastVisibleRect are workaround functions for HitTest in timeline view
	//  The SetLastVisibleRects called by draw operation each time the event is drawn in timeline view
	//  The GetLastVisibleRect is used in HitTest function to determine the hittest info
	// -----------------
	void SetLastVisibleRect(const CRect& rc);
	

protected:
	CXTPCalendarTimeLineViewGroup * m_pGroup;   // internal nmember to keep pointer to CXTPCalendarTimeLineViewGroup
	int m_nEventDurationInPixels;               // internal nmember to keep event duration in pixels value
	CRect m_rcLastVisible;
};

//===========================================================================
// Summary:
//  This class represents an event timeline view portion of the CalendarViewGroup.
// Remarks:
//  It represents a specific view of the event associated data according
//  to various view types and display settings and provides basic
//  functionality on this data using user input through keyboard and mouse.
//  CXTPCalendarTimeLineViewGroup is a descendant of the CXTPCalendarViewGroup class.
//===========================================================================
class _XTP_EXT_CLASS CXTPCalendarTimeLineViewGroup : public CXTPCalendarViewGroup
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Base class type definition.
	//-----------------------------------------------------------------------
	typedef CXTPCalendarViewGroup TBase;

	// -----------------------------
	// Summary:
	//     Default object constructor.
	// Parameters:
	//     pView - A pointer to a CXTPCalendarTimeLineView object.
	//     pParentGroup - A pointer to a CXTPCalendarTimeLineViewGroup object.
	// -----------------------------
	CXTPCalendarTimeLineViewGroup(CXTPCalendarTimeLineView* pView,
								  CXTPCalendarTimeLineViewGroup* pParentGroup = NULL);

	//-----------------------------------------------------------------------
	// Summary:
	//     Default class destructor.
	//-----------------------------------------------------------------------
	virtual ~CXTPCalendarTimeLineViewGroup();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to populate the view with data for
	//     all event items contained in the view.
	// Parameters:
	//     dtDayDate - A COleDateTime object that contains the day's date.
	// Remarks:
	//     This function provides common functionality for all CXTPCalendarViewGroup -
	//     derived classes.
	//-----------------------------------------------------------------------
	virtual void Populate(COleDateTime dtDayDate) {};

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to populate the view with data for
	//     all event items contained in the view.
	// Parameters:
	//     pEvents - pointer to CXTPCalendarEvents.
	//-----------------------------------------------------------------------
	virtual void Populate(CXTPCalendarEvents* pEvents);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to adjust the view's layout
	//     depending on the provided rectangle and calls AdjustLayout()
	//     for all sub-items.
	// Parameters:
	//     pDC - A pointer to a valid device context.
	//     rcGroup - rectangle used to draw the group.
	// ----------------------------
	virtual void AdjustLayout(CDC* pDC, const CRect& rcGroup);

	// ----------------------------------------------------------------------
	// Summary:
	//     This member function is used to draw the group using the specified
	//     device context.
	// Parameters:
	//     pDC :      A pointer to a valid device context.
	//     nOffset :  Vertical offset to draw.
	// ----------------------------------------------------------------------
	virtual void Draw(CDC* pDC, int nOffset = 0);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used to determine which view item,
	//     if any, is at a specified position index, and returns additional
	//     info in a XTP_CALENDAR_HITTESTINFO struct.
	// Parameters:
	//     pt       - A CPoint that contains the coordinates of the point to
	//                test.
	//     pHitTest - A pointer to a XTP_CALENDAR_HITTESTINFO structure that
	//                contains information about the point to test.
	// Returns:
	//     A BOOL. TRUE if the item is found. FALSE otherwise.
	// See Also: XTP_CALENDAR_HITTESTINFO
	//-----------------------------------------------------------------------
	virtual BOOL HitTest(CPoint pt, XTP_CALENDAR_HITTESTINFO* pHitTest) const {return FALSE; };
	virtual void FillHitTestEx(XTP_CALENDAR_HITTESTINFO* pHitTest) const {}

	// Summary:
	//     This member function is used to obtain the owner view object.
	// Returns:
	//     A CXTPCalendarTimeLineView pointer to the owner view object.
	//-----------------------------------------------------------------------
	CXTPCalendarTimeLineView* GetView() const
	{
		return (CXTPCalendarTimeLineView*)m_pView;
	}

	// ---------------------------
	// Summary:
	//     Call this member function to obtain a pointer to the base CXTPCalendarViewEvent*
	//     object that corresponds to the event's index.
	// Parameters:
	//     nIndex :  index of event in m_arEvents
	// Returns:
	//     pointer to CXTPCalendarViewEvent object
	// ---------------------------
	virtual CXTPCalendarTimeLineViewEvent* GetViewEvent(int nIndex) const
	{
		return (CXTPCalendarTimeLineViewEvent*)m_arEvents.GetAt(nIndex);
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to obtain a pointer to the containing
	//     CXTPCalendarViewDay object.
	// Returns:
	//     A pointer to the CXTPCalendarViewDay object.
	//-----------------------------------------------------------------------
	virtual CXTPCalendarViewDay* GetViewDay_() const
	{
		ASSERT(FALSE);
		return 0;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to obtain a pointer to the calendar control
	//     object.
	// Returns:
	//     A pointer to a CXTPCalendarControl object.
	//-----------------------------------------------------------------------
	virtual CXTPCalendarControl* GetCalendarControl() const;

	// ---------------------------
	// Summary:
	//     Call this member function to obtain current height of group
	// Returns:
	//     int height value
	// ---------------------------
	int GetGroupHeight();

	void AddViewEvent(CXTPCalendarEvent* pEvent);

	virtual CXTPCalendarViewEvent* CreateViewEvent(CXTPCalendarEvent* pEvent);


protected:
	//------------------------------------------------------------------------
	// Remarks:
	//     Event views collection type definition.
	//------------------------------------------------------------------------
	typedef CXTPCalendarPtrCollectionT<CXTPCalendarTimeLineViewEvent> TViewEventsCollection;

	CXTPCalendarTimeLineViewGroup* m_pParentGroup; // pointer to the owner view group object

	CXTPCalendarTimeLineViewGroups* m_pChildren; // not used in current version - always NULL

	CXTPCalendarTimeLineView* m_pView;
};

//===========================================================================
// Summary:
//  This class represents an collection of CXTPCalendarTimeLineViewGroup objects
//===========================================================================
class _XTP_EXT_CLASS CXTPCalendarTimeLineViewGroups : public CXTPCalendarPtrCollectionT<CXTPCalendarTimeLineViewGroup>
{
};

/////////////////////////////////////////////////////////////////////////////
AFX_INLINE CXTPCalendarViewGroup* CXTPCalendarTimeLineViewEvent::GetViewGroup_() const {
	return m_pGroup;
}

AFX_INLINE int CXTPCalendarTimeLineViewEvent::GetEventDurationInPixels() const {
	return m_nEventDurationInPixels;
}
AFX_INLINE const CRect& CXTPCalendarTimeLineViewEvent::GetLastVisibleRect() const {
	return m_rcLastVisible;
}

AFX_INLINE void CXTPCalendarTimeLineViewEvent::SetLastVisibleRect(const CRect& rc) {
	m_rcLastVisible = rc;
}

//===========================================================================

AFX_INLINE void CXTPCalendarTimeLineViewGroup::AddViewEvent(CXTPCalendarEvent* /*pEvent*/) {
	ASSERT(FALSE);
}

//===========================================================================

#endif // !defined(__XTPCALENDARTIMELINEVIEWGROUP_H__)
