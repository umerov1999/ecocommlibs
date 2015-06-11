// XTPCalendarThemePrevNextEventButtons.h : header file
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

//{{AFX_CODEJOCK_PRIVATE
#if !defined(__XTPCALENDARTHEMEOFFICE2007PREVNEXTEVENTBUTTONS_H__)
#define __XTPCALENDARTHEMEOFFICE2007PREVNEXTEVENTBUTTONS_H__
//}}AFX_CODEJOCK_PRIVATE

DECLARE_THEMEPART(CTOPrevNextEventButtons, CXTPCalendarThemePart)

	CTOPrevNextEventButtons();
	virtual ~CTOPrevNextEventButtons();

	DECLARE_THEMEPART_MEMBER_(0, CTOPrevNextEventButton, PrevEventButton, CXTPCalendarThemePart)
	DECLARE_THEMEPART_MEMBER_(1, CTOPrevNextEventButton, NextEventButton, CXTPCalendarThemePart)

	virtual void RefreshMetrics(BOOL bRefreshChildren = TRUE);
	virtual void AdjustLayout(CXTPCalendarView* pObject, CDC* pDC, const CRect& rcRect);

	virtual void Draw(CXTPCalendarView* pObject, CDC* pDC);

	virtual int HitTest(const CPoint* pPoint = NULL) const; // return 0, xtpCalendarHitTestPrevEventButton or xtpCalendarHitTestNextEventButton

	virtual BOOL IsPrevNextButtonsVisible();

	virtual BOOL _IsSomeEventVisible();
	COleDateTime GetVisibleDay(BOOL bFirst);

	virtual BOOL GetPrevEventDay(COleDateTime* pdtDay = NULL);
	virtual BOOL GetNextEventDay(COleDateTime* pdtDay = NULL);

	virtual void OnStatusChanged();

	virtual void OnMouseMove(CCmdTarget* pObject, UINT nFlags, CPoint point);
	virtual BOOL OnLButtonDown(CCmdTarget* pObject, UINT nFlags, CPoint point);

	// Settings
	CXTPCalendarThemeBOOLValue  m_bHide;

	CXTPCalendarThemeIntValue   m_nIdleStepTime_ms;
	CXTPCalendarThemeIntValue   m_nRefreshDaysPerIdleStep;
	CXTPCalendarThemeBOOLValue  m_bUseActiveViewResources;

	virtual void SetCacheSize(int nDays);

protected:
	CXTPCalendarResources* _GetActiveResources();

	void _RequestToFindEvents();
	void _StopRequests();
	BOOL _RefreshDay(long ndtDay, BOOL& rbRefreshed, BOOL bRefreshPermanently);

	//void _ResetData();

	virtual BOOL OnTimer(UINT_PTR uTimerID);
	virtual void OnDetachCalendar();

	UINT_PTR     m_nRefreshTimerID;

	long m_ndtRefreshDatePrev;
	long m_ndtRefreshDateNext;

	BOOL m_bScanPrev;
	BOOL m_bScanNext;

	long m_ndtMinScanDay;
	long m_ndtMaxScanDay;

	CRect m_rcRect;

	class CDaysMap : protected CArray<char, char>
	{
	protected:
		long m_nDaysOffset;
		int m_nCacheSizeMax_days;
	public:
		CDaysMap();

		long GetMinDay();
		long GetMaxDay();

		int GetDayState(long nDay);
		void SetDayState(long nDay, int nState); // states: (0)-unknown, (-1)-has no-events, (1)-has events.
		void SetDaysStateSafe(long nDayStart, long nDayEnd, int nState); // states: (0)-unknown, (-1)-has no-events, (1)-has events.

		void AdjustMiddleDay(long nDay);

		void SetDataSize(int nDays);
		int GetDataSize();
	protected:
		void _GrowArrayIfNeed(long nDay);
	};

	CDaysMap m_mapDaysState;
protected:
	CXTPNotifySink* m_pSink;
	virtual void OnEvent_Calendar(XTP_NOTIFY_CODE Event, WPARAM wParam, LPARAM lParam);
};

//{{AFX_CODEJOCK_PRIVATE
#endif // __XTPCALENDARTHEMEOFFICE2007PREVNEXTEVENTBUTTONS_H__
//}}AFX_CODEJOCK_PRIVATE
