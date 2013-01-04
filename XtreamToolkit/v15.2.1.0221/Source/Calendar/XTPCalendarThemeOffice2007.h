// XTPCalendarThemeOffice2007.h: interface for the CXTPCalendarControlPaintManager class.
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
#if !defined(_XTP_CALENDAR_THEME_OFFICE_2007_H__)
#define _XTP_CALENDAR_THEME_OFFICE_2007_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#if (_MSC_VER > 1100)
#pragma warning(push)
#endif

#pragma warning(disable: 4250)
//#pragma warning(disable: 4097)

#pragma warning(disable : 4100)// TODO: remove when themes will be finished

class CXTPResourceImages;
class CXTPImageManager;
class CXTPCalendarResources;
class CXTPNotifySink;
class CXTPCalendarEventCategories;
class CXTPCalendarEventCategoryIDs;

/////////////////////////////////////////////////////////////////////////////
// Initial version. Will be expanded in the feature.

//{{AFX_CODEJOCK_PRIVATE
class _XTP_EXT_CLASS CXTPCalendarThemeOffice2007 : public CXTPCalendarTheme
{
	DECLARE_DYNCREATE(CXTPCalendarThemeOffice2007)

	typedef CXTPCalendarTheme TBase;
public:

	DECLARE_THEMEPART(CTOHeader, CXTPCalendarTheme::CTOHeader)

		DECLARE_THEMEPART_MEMBER(0, CTOFormula_MulDivC, HeightFormula)
		//===========================================
		virtual void RefreshMetrics(BOOL bRefreshChildren = TRUE);

		virtual void Draw(CCmdTarget* pObject, CDC* pDC) {ASSERT(FALSE);};

	protected:
	};
	friend class CTOHeader;

	//=======================================================================
	DECLARE_THEMEPART(CTOEvent, CXTPCalendarTheme::CTOEvent)
		struct CEventFontsColorsSet
		{
			CXTPPaintManagerColor           clrBorder;
			CXTPPaintManagerColorGradient   grclrBackground;

			CThemeFontColorSetValue         fcsetSubject;
			CThemeFontColorSetValue         fcsetLocation;
			CThemeFontColorSetValue         fcsetBody;

			CThemeFontColorSetValue         fcsetStartEnd; // for a month view single day event times
			                                               // or DayView multiday event From/To
			virtual void CopySettings(const CEventFontsColorsSet& rSrc);

			virtual void doPX(CXTPPropExchange* pPX, LPCTSTR pcszPropName, CXTPCalendarTheme* pTheme);
			virtual void Serialize(CArchive& ar);
		};

		CEventFontsColorsSet    m_fcsetNormal;
		CEventFontsColorsSet    m_fcsetSelected;

		CXTPPaintManagerColor   m_clrGripperBorder;
		CXTPPaintManagerColor   m_clrGripperBackground;

		virtual void RefreshMetrics(BOOL bRefreshChildren = TRUE);
		virtual void RefreshFromParent(CTOEvent* pParentSrc);

		virtual void DoPropExchange(CXTPPropExchange* pPX);
		virtual void Serialize(CArchive& ar);

		virtual int CalcMinEventHeight(CDC* pDC);

		virtual void GetEventColors(CDC* pDC, CXTPCalendarViewEvent* pViewEvent, COLORREF& rClrBorder,
									COLORREF& rClrBorderNotSel, CXTPPaintManagerColorGradient& rGrclrBk,
									BOOL bSelected = -1, int* pnColorsSrc_1Cat2Lbl = NULL);

		virtual void GetShowAsStatusColors(CDC* pDC, CXTPCalendarViewEvent* pViewEvent, COLORREF& clrBorder, COLORREF& clrBorderNotSel, CXTPPaintManagerColorGradient& grclrBk);
		virtual BOOL DrawShowAsStatus(CDC* pDC, CXTPCalendarViewEvent* pViewEvent, CXTPCalendarThemeOffice2007* pThemeX, const CRect& rcEvent, LONG& lShowAsBlockWidth);

		virtual void FillEventBackgroundEx(CDC* pDC, CXTPCalendarViewEvent* pViewEvent, const CRect& rcRect);

		virtual void MultidayDrawTime(int nMDEFlags, CXTPCalendarViewEvent* pViewEvent,
										CDC* pDC, const CRect& rcEvent, BOOL fStartTimeAsClock, BOOL fEndTimeAsClock, CRect& rcText);
		virtual void MultidayDrawTimeBlocks(CDC* pDC, const CRect& rcEventRect, CXTPCalendarViewEvent* pViewEvent,
										BOOL MultiDayFirstPart, BOOL MultiDayLastPart,
																	BOOL fStartTimeAsClock, BOOL fEndTimeAsClock);
		virtual void GetMultidayTimeFontAndColor(CFont*& pFont, COLORREF& clrColor, BOOL bSelected);

  protected:
		virtual BOOL Draw_ArrowL(CXTPCalendarViewEvent* pViewEvent, CDC* pDC, CRect& rrcRect, LONG lArrowOffset = 0);
		virtual BOOL Draw_ArrowR(CXTPCalendarViewEvent* pViewEvent, CDC* pDC, CRect& rrcRect);

		virtual CSize Draw_Icons(CXTPCalendarViewEvent* pViewEvent, CDC* pDC, const CRect& rcIconsMax, BOOL bCalculate = FALSE);

		virtual void InitBusyStatusDefaultColors();
	};
	friend class CTOEvent;

	DECLARE_THEMEPART(CTODay, CXTPCalendarTheme::CTODay)
		virtual int HitTestExpandDayButton(const CXTPCalendarViewDay* pViewDay, const CPoint* pPoint = NULL); // return 0 or xtpCalendarHitTestDayExpandButton
	};

	DECLARE_THEMEPART(CTOPrevNextEventButton, CXTPCalendarThemePart)

		CXTPCalendarThemeIntValue   m_nBitmapID;
		CXTPCalendarThemeRectValue  m_rcBitmapBorder;

		CThemeFontColorSetValue     m_fcsetText;
		CXTPPaintManagerColor       m_clrDisabledText;
		CXTPCalendarThemeStringValue m_strText;

		virtual void RefreshMetrics(BOOL bRefreshChildren = TRUE);

		virtual void DoPropExchange(CXTPPropExchange* pPX){};
		virtual void Serialize(CArchive& ar){};

		virtual void AdjustLayout(CXTPCalendarView* pObject, CDC* pDC, const CRect& rcRect);
		virtual void Draw(CXTPCalendarView* pObject, CDC* pDC);

		virtual int HitTest(const CPoint* pPoint = NULL) const; // return 0, xtpCalendarHitTestPrevEventButton or xtpCalendarHitTestNextEventButton

		virtual void OnMouseMove(CCmdTarget* pObject, UINT nFlags, CPoint point);
		virtual BOOL OnLButtonDown(CCmdTarget* pObject, UINT nFlags, CPoint point);

		virtual CRect GetRect();

		//------------------------------
		CTOPrevNextEventButton()
		{
			m_bPrev = TRUE;
			m_rcRect.SetRect(0,0,0,0);

			m_bHot = FALSE;
			m_bDisabled = FALSE;
			m_bVisible = TRUE;
		}

	protected:
		CSize GetSize(const CRect* prcRect = NULL);
		BOOL m_bPrev;
		CRect m_rcRect;

	public:
		BOOL m_bHot;
		BOOL m_bDisabled;
		BOOL m_bVisible;
	};
	friend class CTOPrevNextEventButton;

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
	friend class CTOPrevNextEventButtons;


	/////////////////////////////////////////////////////////////////////////
	// ******** Day View *********

	//=======================================================================
	DECLARE_THEMEPART2(CTODayViewEvent, CTOEvent, CXTPCalendarTheme::CTODayViewEvent)

		DECLARE_THEMEPART_MEMBER(0, CTOEventIconsToDraw, EventIconsToDraw)
		DECLARE_THEMEPART_MEMBER(1, CTOFormula_MulDivC,  HeightFormula)

		virtual void RefreshMetrics(BOOL bRefreshChildren = TRUE);
	
	protected:
		static const int nCategoryIconWidth;
		static const int nCategoryIconOffset;

		BOOL NeedDrawCategoryIcons(CXTPCalendarEvent* pEvent);
		void DrawCategoryIcons(CDC* pDC, CXTPCalendarEvent* pEvent, const CRect& rcCategoryIcons);
		bool GetCategoryIconColors(CXTPCalendarEventCategories* pCategories,
															CXTPCalendarEventCategoryIDs* pCategoryIDs,
															int nCategoryIdx, COLORREF& clrCatColor, COLORREF& clrIconBorder);
		int CalcCategoryAreaWidth(int nCategoriesCount);
	};

	DECLARE_THEMEPART2(CTODayViewEvent_MultiDay, CTODayViewEvent, CXTPCalendarTheme::CTODayViewEvent_MultiDay)

		CXTPCalendarThemeStringValue m_strDateFormatFrom;
		CXTPCalendarThemeStringValue m_strDateFormatTo;
		CXTPCalendarThemeBOOLValue   m_bShowFromToArrowTextAlways;

		virtual void AdjustLayout(CCmdTarget* pObject, CDC* pDC, const CRect& rcRect, int nEventPlaceNumber);
		virtual void RefreshMetrics(BOOL bRefreshChildren = TRUE);

		virtual void DoPropExchange(CXTPPropExchange* pPX);
		virtual void Serialize(CArchive& ar);

		virtual void Draw(CCmdTarget* pObject, CDC* pDC);

		virtual void CalcEventYs(CRect& rcRect, int nEventPlaceNumber);

	protected:
		virtual BOOL Draw_ArrowLtext(CXTPCalendarViewEvent* pViewEvent, CDC* pDC, CThemeFontColorSetValue* pfcsetText, CRect& rrcRect, int nLeft_x);
		virtual BOOL Draw_ArrowRtext(CXTPCalendarViewEvent* pViewEvent, CDC* pDC, CThemeFontColorSetValue* pfcsetText, CRect& rrcRect, int nRight_x);

		virtual CString Format_FromToDate(CXTPCalendarViewEvent* pViewEvent, int nStart1End2);

		void DrawCategoryIcons(CDC* pDC, CXTPCalendarViewEvent* pViewEvent, CRect rcCategoryIcons, const CRect& rcEventMax);				
		CRect AdjustCategoryIconsRectWidth(const CRect& rcCategoryIconsBase, const CRect& rcEvent, int nCategoriesCount);		
	};
	friend class CTODayViewEvent_MultiDay;

	DECLARE_THEMEPART2(CTODayViewEvent_SingleDay, CTODayViewEvent, CXTPCalendarTheme::CTODayViewEvent_SingleDay)
		virtual void RefreshMetrics(BOOL bRefreshChildren = TRUE);
		virtual void AdjustLayout(CCmdTarget* pObject, CDC* pDC, const CRect& rcRect, int nEventPlaceNumber);
		virtual void Draw(CCmdTarget* pObject, CDC* pDC);
		virtual void DrawShadow(CDC* pDC, CXTPCalendarDayViewEvent* pViewEvent);

		private:
			void DrawCategoryIcons(CDC* pDC, CXTPCalendarViewEvent* pViewEvent, const CRect& rcText, const CRect& rcTopRow);
			CRect CalcCategoryIconsRect(int nCategoriesCount, const CRect& rcText, const CRect& rcTopRow);
	};
	friend class CTODayViewEvent_SingleDay;

	//=======================================================================
	DECLARE_THEMEPART(CTODayViewTimeScale, CXTPCalendarTheme::CTODayViewTimeScale)
		virtual void RefreshMetrics(BOOL bRefreshChildren = TRUE);

		virtual int CalcMinRowHeight(CDC* pDC, CXTPCalendarDayViewTimeScale* pTimeScale);

		DECLARE_THEMEPART_MEMBER(0, CTOFormula_MulDivC, HeightFormula)
	};

	//=======================================================================
	DECLARE_THEMEPART2(CTODayViewDayGroupHeader, CTOHeader, CXTPCalendarTheme::CTODayViewDayGroupHeader)
		virtual void RefreshMetrics(BOOL bRefreshChildren = TRUE);
		virtual void Draw(CCmdTarget* pObject, CDC* pDC);
	};

	DECLARE_THEMEPART(CTODayViewDayGroupAllDayEvents, CXTPCalendarTheme::CTODayViewDayGroupAllDayEvents)

		virtual void RefreshMetrics(BOOL bRefreshChildren = TRUE);

		virtual void Draw(CCmdTarget* pObject, CDC* pDC);

		virtual void SetDynColors(CXTPCalendarDayViewGroup* pViewGroup);
	protected:
		COLORREF m_clrDynHatch;
		int      m_nDynBusyStatus;
	};

	DECLARE_THEMEPART(CTODayViewDayGroupCell, CXTPCalendarTheme::CTODayViewDayGroupCell)

		COLORREF m_clrDynHatchBrush;

		virtual void RefreshMetrics(BOOL bRefreshChildren = TRUE);
		virtual int CalcHeight(CDC* pDC, int nCellWidth) {return 23;}

		virtual void Draw(CDC* pDC, const CRect& rcRect,
						  const XTP_CALENDAR_THEME_DAYVIEWCELL_PARAMS& cellParams, CXTPPropsStateContext* pStateCnt = NULL);
	};

	//=======================================================================
	DECLARE_THEMEPART(CTODayViewDayGroup, CXTPCalendarTheme::CTODayViewDayGroup)

		virtual void RefreshMetrics(BOOL bRefreshChildren = TRUE);
		virtual void Draw(CCmdTarget* pObject, CDC* pDC);

		virtual BOOL IsSelected(CXTPCalendarViewGroup* pViewGroup);

		DECLARE_THEMEPART_MEMBER(0, CTODayViewDayGroupHeader,       Header)
		DECLARE_THEMEPART_MEMBER(1, CTODayViewDayGroupAllDayEvents, AllDayEvents)
		DECLARE_THEMEPART_MEMBER(2, CTODayViewDayGroupCell,         Cell)
		DECLARE_THEMEPART_MEMBER(3, CTODayViewEvent_MultiDay,       MultiDayEvent)
		DECLARE_THEMEPART_MEMBER(4, CTODayViewEvent_SingleDay,      SingleDayEvent)

	public:
		virtual void OnMouseMove(CCmdTarget* pObject, UINT nFlags, CPoint point);
		virtual BOOL OnLButtonDown(CCmdTarget* pObject, UINT nFlags, CPoint point);

		virtual CRect GetScrollButtonRect(const XTP_DAY_VIEW_GROUP_LAYOUT& groupLayout, int nButton);

	protected:
		virtual void AdjustDayEvents(CXTPCalendarDayViewGroup* pDayViewGroup, CDC* pDC);
		int HitTestAllDayEventsScrollIcons(const CXTPCalendarDayViewGroup* pDVGroup, const CPoint& point);
	};

	DECLARE_THEMEPART2(CTODayViewDayHeader, CTOHeader, CXTPCalendarTheme::CTODayViewDayHeader)

		CXTPCalendarThemeBOOLValue m_UseOffice2003HeaderFormat;

		CTODayViewDayHeader()
		{
			m_nWeekDayFormat = 0;
		}

		virtual void RefreshMetrics(BOOL bRefreshChildren = TRUE);
		virtual void AdjustLayout(CCmdTarget* pObject, CDC* pDC, const CRect& rcRect);
		virtual void Draw(CCmdTarget* pObject, CDC* pDC);

		virtual void DoPropExchange(CXTPPropExchange* pPX);
		virtual void Serialize(CArchive& ar);
	private:
		int m_nWeekDayFormat; // 0 - no Week Day, 1 - short, 2 - long
	};

	//=======================================================================
	DECLARE_THEMEPART(CTODayViewDay, CXTPCalendarTheme::CTODayViewDay)

		CXTPPaintManagerColor         m_clrBorder;
		CXTPPaintManagerColor         m_clrTodayBorder;

		DECLARE_THEMEPART_MEMBER(0, CTODayViewDayHeader,            Header)
		DECLARE_THEMEPART_MEMBER(1, CTODayViewDayGroup,             Group)

		virtual void RefreshMetrics(BOOL bRefreshChildren = TRUE);

		virtual void DoPropExchange(CXTPPropExchange* pPX);
		virtual void Serialize(CArchive& ar);

		virtual void Draw(CCmdTarget* pObject, CDC* pDC);

		virtual void Draw_DayBorder(CXTPCalendarDayViewDay* pDayViewDay, CDC* pDC);
		virtual CRect ExcludeDayBorder(CXTPCalendarDayViewDay* pDayViewDay, const CRect& rcDay);
	public:
		virtual void OnMouseMove(CCmdTarget* pObject, UINT nFlags, CPoint point);
	};

	DECLARE_THEMEPART(CTODayViewHeader, CTOHeader)
		virtual void RefreshMetrics(BOOL bRefreshChildren = TRUE);
	};


	//=======================================================================
	DECLARE_THEMEPART(CTODayView, CXTPCalendarTheme::CTODayView)

		virtual void RefreshMetrics(BOOL bRefreshChildren = TRUE);

		virtual void AdjustLayout(CDC* pDC, const CRect& rcRect);
		virtual void Draw(CDC* pDC);

		DECLARE_THEMEPART_MEMBER_(0, CTODayViewHeader,   Header, CXTPCalendarTheme::CTOHeader)
		DECLARE_THEMEPART_MEMBER(1, CTODayViewEvent,     Event)
		DECLARE_THEMEPART_MEMBER(2, CTODayViewTimeScale, TimeScale)
		DECLARE_THEMEPART_MEMBER(3, CTODayViewDay, Day)

		// theme specific control options
		virtual BOOL IsUseCellAlignedDraggingInTimeArea() const {return TRUE;};
	protected:
	};

	/////////////////////////////////////////////////////////////////////////
	// ******** Month View *********

	// ======= MonthViewEvent ======
	DECLARE_THEMEPART2(CTOMonthViewEvent, CTOEvent, CXTPCalendarTheme::CTOMonthViewEvent)
		DECLARE_THEMEPART_MEMBER(0, CTOEventIconsToDraw, EventIconsToDraw)
		DECLARE_THEMEPART_MEMBER(1, CTOFormula_MulDivC,  HeightFormula)

		virtual void RefreshMetrics(BOOL bRefreshChildren = TRUE);
		virtual void AdjustLayout(CCmdTarget* pObject, CDC* pDC, const CRect& rcRect, int nEventPlaceNumber);
	};
	friend class CTOMonthViewEvent;

	// ---- MonthViewEvent_SingleDay ----
	DECLARE_THEMEPART2(CTOMonthViewEvent_SingleDay, CTOMonthViewEvent, CXTPCalendarTheme::CTOMonthViewEvent_SingleDay)

		virtual void RefreshMetrics(BOOL bRefreshChildren = TRUE);
		virtual void Draw(CCmdTarget* pObject, CDC* pDC);

		virtual void GetShowAsStatusColors(CDC* pDC, CXTPCalendarViewEvent* pViewEvent, COLORREF& clrBorder, COLORREF& clrBorderNotSel, CXTPPaintManagerColorGradient& grclrBk);

	protected:
		virtual void Draw_Background(CDC* pDC, const CRect& rcEventRect, CXTPCalendarMonthViewEvent* pViewEvent);
		virtual CSize Draw_Time(CDC* pDC, const CRect& rcEventRect, CXTPCalendarMonthViewEvent* pViewEvent);
		virtual void Draw_Caption(CDC* pDC, const CRect& rcTextRect, CXTPCalendarMonthViewEvent* pViewEvent);

	protected:
	};
	friend class CTOMonthViewEvent_SingleDay;

	// ---- MonthViewEvent_MultiDay ----
	DECLARE_THEMEPART2(CTOMonthViewEvent_MultiDay, CTOMonthViewEvent, CXTPCalendarTheme::CTOMonthViewEvent_MultiDay)

		virtual void RefreshMetrics(BOOL bRefreshChildren = TRUE);
		virtual void Draw(CCmdTarget* pObject, CDC* pDC);
	};
	friend class CTOMonthViewEvent_MultiDay;

	// ===== MonthViewDayHeader ====
	DECLARE_THEMEPART2(CTOMonthViewDayHeader, CTOHeader, CXTPCalendarTheme::CTOMonthViewDayHeader)
		virtual void RefreshMetrics(BOOL bRefreshChildren = TRUE);
		virtual void Draw(CCmdTarget* pObject, CDC* pDC);

	};
	friend class CTOMonthViewDayHeader;

	// ======= MonthViewDay =======
	DECLARE_THEMEPART2(CTOMonthViewDay, CTODay, CXTPCalendarTheme::CTOMonthViewDay)

		CXTPPaintManagerColor m_clrBorder;
		CXTPPaintManagerColor m_clrTodayBorder;

		CXTPPaintManagerColor m_clrBackgroundLight;
		CXTPPaintManagerColor m_clrBackgroundDark;
		CXTPPaintManagerColor m_clrBackgroundSelected;

		virtual void RefreshMetrics(BOOL bRefreshChildren = TRUE);
		virtual void Draw(CCmdTarget* pObject, CDC* pDC);

		virtual void DoPropExchange(CXTPPropExchange* pPX);
		virtual void Serialize(CArchive& ar);

		DECLARE_THEMEPART_MEMBER(0, CTOMonthViewDayHeader,       Header);
		DECLARE_THEMEPART_MEMBER(1, CTOMonthViewEvent_MultiDay,  MultiDayEvent);
		DECLARE_THEMEPART_MEMBER(2, CTOMonthViewEvent_SingleDay, SingleDayEvent);

	public:
		virtual void OnMouseMove(CCmdTarget* pObject, UINT nFlags, CPoint point);
		virtual BOOL OnLButtonDown(CCmdTarget* pObject, UINT nFlags, CPoint point);

		void SetDynColors(CXTPCalendarMonthViewDay* pMViewDay);
	protected:
		COLORREF m_clrDynHatch;
		int      m_nDynBusyStatus;
		BOOL     m_fHatchBackground;
	};
	friend class CTOMonthViewDay;

	// ======= MonthViewWeekDayHeader =======
	DECLARE_THEMEPART2(CTOMonthViewWeekDayHeader, CTOHeader, CXTPCalendarTheme::CTOMonthViewWeekDayHeader)

		virtual void RefreshMetrics(BOOL bRefreshChildren = TRUE);
		virtual void AdjustLayout(CCmdTarget* pObject, CDC* pDC, const CRect& rcRect);
		virtual void Draw(CCmdTarget* pObject, CDC* pDC);

	protected:
		virtual void Draw_Borders2(CDC* pDC, const CRect& rcRect, BOOL bDrawRightBorder);
	protected:
		BOOL m_bWeekDayNamesLong;
		BOOL m_bWeekDayNameSaSuLong;

		CStringArray m_arWeekDayNamesLong;  // 0 - Sunday, 1 - Monday, ...; 7 - Sat/Sun
		CStringArray m_arWeekDayNamesShort; // 0 - Sunday, 1 - Monday, ...; 7 - Sat/Sun

	public:
		CTOMonthViewWeekDayHeader()
		{
			m_bWeekDayNamesLong = FALSE;
			m_bWeekDayNameSaSuLong = FALSE;
		}
	};
	friend class CTOMonthViewWeekDayHeader;

	// ======= MonthViewWeekHeader =======
	DECLARE_THEMEPART(CTOMonthViewWeekHeader, CTOHeader)
		CXTPPaintManagerColor   m_clrFreeSpaceBk;
		// CTOHeader::m_TextCenter.fcsetNormal is used to draw header caption;

		virtual void RefreshMetrics(BOOL bRefreshChildren = TRUE);
		virtual void AdjustLayout(CCmdTarget* pObject, CDC* pDC, const CRect& rcRect);
		virtual void Draw(CCmdTarget* pObject, CDC* pDC);

		virtual void DoPropExchange(CXTPPropExchange* pPX);
		virtual void Serialize(CArchive& ar);

		virtual BOOL OnLButtonDown(CCmdTarget* pObject, UINT nFlags, CPoint point);

		virtual int CalcWidth(CDC* pDC);

		CTOMonthViewWeekHeader()
		{
			m_rcHeader.SetRect(0,0,0,0);
			m_bDateFormatShort = TRUE;
		}
	protected:
		CRect m_rcHeader;
		BOOL  m_bDateFormatShort;


		void GetWeekHeaderRect(CXTPCalendarThemeOffice2007* pThemeX, CXTPCalendarMonthViewDay* pViewDay, CRect& rcWHeader);

		void SwitchCalendarActiveViewToWeekView(CXTPCalendarControl* pCalendar);

		virtual void FormatWeekCaption(COleDateTime dtWDay1, CString& rstrShort, CString& rstrLong, int nShort1Long2 = 0);

		virtual CString _FormatWCaption(LPCTSTR pcszDay1, LPCTSTR pcszMonth1, LPCTSTR pcszDay7,
										LPCTSTR pcszMonth7, LPCTSTR pcszDayMonthSeparator, int nDateOrdering);

		virtual void Draw_TextCenter(CDC* pDC, const CRect& rcRect, int nState,
									 LPCTSTR pcszCenterText, int* pnWidth = NULL);
	};
	friend class CTOMonthViewWeekHeader;

	// ******* MonthView theme part object *******
	DECLARE_THEMEPART(CTOMonthView, CXTPCalendarTheme::CTOMonthView)

		// TODO:
		// BOOL m_bShowWeekNumbers;
		//
		//virtual void DoPropExchange(CXTPPropExchange* pPX);
		//virtual void Serialize(CArchive& ar);

		virtual void RefreshMetrics(BOOL bRefreshChildren = TRUE);
		virtual void AdjustLayout(CDC* pDC, const CRect& rcRect, BOOL bCallPostAdjustLayout);
		virtual void Draw(CDC* pDC);
		virtual BOOL OnLButtonDown(CCmdTarget* pObject, UINT nFlags, CPoint point);

		DECLARE_THEMEPART_MEMBER( 0, CTOHeader,                 Header);
		DECLARE_THEMEPART_MEMBER( 1, CTOMonthViewEvent,         Event);
		DECLARE_THEMEPART_MEMBER( 2, CTOMonthViewWeekDayHeader, WeekDayHeader);
		DECLARE_THEMEPART_MEMBER_(3, CTOMonthViewWeekHeader,    WeekHeader, CTOHeader);
		DECLARE_THEMEPART_MEMBER( 4, CTOMonthViewDay,           Day);
	};
	friend class CTOMonthView;


	/////////////////////////////////////////////////////////////////////////
	// ******** WeekView *********

	// ======= WeekViewEvent =======
	DECLARE_THEMEPART2(CTOWeekViewEvent, CTOEvent, CXTPCalendarTheme::CTOWeekViewEvent)
		DECLARE_THEMEPART_MEMBER(0, CTOEventIconsToDraw, EventIconsToDraw)
		DECLARE_THEMEPART_MEMBER(1, CTOFormula_MulDivC,  HeightFormula)

		virtual void RefreshMetrics(BOOL bRefreshChildren = TRUE);
		virtual void AdjustLayout(CCmdTarget* pObject, CDC* pDC, const CRect& rcRect, int nEventPlaceNumber);
	};
	friend class CTOWeekViewEvent;

	// ==== WeekViewEvent_SingleDay ====
	DECLARE_THEMEPART2(CTOWeekViewEvent_SingleDay, CTOWeekViewEvent, CXTPCalendarTheme::CTOWeekViewEvent_SingleDay)
		virtual void RefreshMetrics(BOOL bRefreshChildren = TRUE);
		virtual void Draw(CCmdTarget* pObject, CDC* pDC);
	protected:
		virtual void Draw_Background(CDC* pDC, const CRect& rcEventRect, CXTPCalendarWeekViewEvent* pViewEvent);
		virtual CSize Draw_Time(CDC* pDC, const CRect& rcEventRect, CXTPCalendarWeekViewEvent* pViewEvent);
		virtual void Draw_Caption(CDC* pDC, const CRect& rcTextRect, CXTPCalendarWeekViewEvent* pViewEvent);
	};
	friend class CTOWeekViewEvent_SingleDay;

	// ==== WeekViewEvent_MultiDay ====
	DECLARE_THEMEPART2(CTOWeekViewEvent_MultiDay, CTOWeekViewEvent, CXTPCalendarTheme::CTOWeekViewEvent_MultiDay)
		virtual void RefreshMetrics(BOOL bRefreshChildren = TRUE);
		virtual void Draw(CCmdTarget* pObject, CDC* pDC);
		virtual void GetMultidayTimeFontAndColor(CFont*& pFont, COLORREF& clrColor, BOOL bSelected);
	};
	friend class CTOWeekViewEvent_MultiDay;

	// ==== WeekViewDayHeader ====
	DECLARE_THEMEPART2(CTOWeekViewDayHeader, CTOHeader, CXTPCalendarTheme::CTOWeekViewDayHeader)
		CXTPCalendarThemeStringValue m_strHeaderFormat;
		CXTPCalendarThemeBOOLValue m_UseOffice2003HeaderFormat;

		virtual void AdjustLayout(CCmdTarget* pObject, CDC* pDC, const CRect& rcRect);
		virtual void RefreshMetrics(BOOL bRefreshChildren = TRUE);
		virtual void Draw(CCmdTarget* pObject, CDC* pDC);

		virtual void DoPropExchange(CXTPPropExchange* pPX);
		virtual void Serialize(CArchive& ar);
	};
	friend class CTOWeekViewDayHeader;

	// ======= WeekViewDay =======
	DECLARE_THEMEPART2(CTOWeekViewDay, CTODay, CXTPCalendarTheme::CTOWeekViewDay)

		CXTPPaintManagerColor m_clrBorder;
		CXTPPaintManagerColor m_clrTodayBorder;

		CXTPPaintManagerColor m_clrBackgroundLight;
		CXTPPaintManagerColor m_clrBackgroundDark;
		CXTPPaintManagerColor m_clrBackgroundSelected;

		virtual void RefreshMetrics(BOOL bRefreshChildren = TRUE);
		virtual void Draw(CCmdTarget* pObject, CDC* pDC);

		virtual void DoPropExchange(CXTPPropExchange* pPX);
		virtual void Serialize(CArchive& ar);

		virtual void OnMouseMove(CCmdTarget* pObject, UINT nFlags, CPoint point);
		virtual BOOL OnLButtonDown(CCmdTarget* pObject, UINT nFlags, CPoint point);

		DECLARE_THEMEPART_MEMBER(0, CTOWeekViewDayHeader,       Header);
		DECLARE_THEMEPART_MEMBER(1, CTOWeekViewEvent_MultiDay,  MultiDayEvent);
		DECLARE_THEMEPART_MEMBER(2, CTOWeekViewEvent_SingleDay, SingleDayEvent);

		virtual void SetDynColors(CXTPCalendarWeekViewDay* pWViewDay);
	protected:
		COLORREF m_clrDynHatch;
		int      m_nDynBusyStatus;
	};
	friend class CTOWeekViewDay;

	// ==== WeekView theme part object ====
	DECLARE_THEMEPART(CTOWeekView, CXTPCalendarTheme::CTOWeekView)
		virtual void RefreshMetrics(BOOL bRefreshChildren = TRUE);
		virtual void AdjustLayout(CDC* pDC, const CRect& rcRect, BOOL bCallPostAdjustLayout);
		virtual void Draw(CCmdTarget* pObject, CDC* pDC);

		DECLARE_THEMEPART_MEMBER(0, CTOWeekViewEvent, Event);
		DECLARE_THEMEPART_MEMBER(1, CTOWeekViewDay,   Day);
	};


	DECLARE_THEMEPART(CTOCellDynBackground, CXTPCalendarThemePart)
		virtual int GetCellDynBackground(CXTPCalendarViewGroup* pVioewGroup, COleDateTime dtCellTime,
										 int nCellIndex, XTP_BUSY_STATUS_CONTEXT& rContext);
		virtual CXTPCalendarViewEvent* GetCellBackgroundEvent(CXTPCalendarViewGroup* pVioewGroup,
										 COleDateTime dtCellTime, int nCellIndex);
		BOOL DrawMonthViewHatchBackground();
		CXTPCalendarEvent* GetBkTentativeEvent();
	protected:
		virtual int CmpEventBusyStatus(CXTPCalendarEvent* pEvent1, CXTPCalendarEvent* pEvent2);
		virtual int _GetEventBusyStatusRank(CXTPCalendarEvent* pEvent);

	protected:
		void CheckIsBackgroundHatchNeeded();

	protected:
		BOOL m_fDrawMonthViewBackgroundHatch;
		CXTPCalendarViewEvent* m_pBkViewEvent;
		CXTPCalendarEvent* m_pBkTentativeEvent;

	};

	//=======================================================================
	DECLARE_THEMEPART_MEMBER(0, CTOColorsSet,   ColorsSet)

	virtual BOOL IsBaseColorOrig();
	virtual COLORREF GetColor(int eCLR, COLORREF clrBase = (COLORREF)-1);
	virtual COLORREF GetColor2(LPCTSTR pcszColorName, COLORREF clrDefault);

	virtual CXTPResourceImage* GetBitmap(LPCTSTR pcszStdBmpName);

	DECLARE_THEMEPART_MEMBER(1, CTOHeader,      Header)
	DECLARE_THEMEPART_MEMBER(2, CTOEvent,       Event)

	DECLARE_THEMEPART_MEMBER(3, CTODayView,     DayView)
	DECLARE_THEMEPART_MEMBER(4, CTOMonthView,   MonthView)
	DECLARE_THEMEPART_MEMBER(5, CTOWeekView,    WeekView)

	// office 2007 theme specific parts
	DECLARE_THEMEPART_MEMBER_(6, CTOCellDynBackground, CellDynBackground, CTOCellDynBackground)

	DECLARE_THEMEPART_MEMBER_(7, CTOPrevNextEventButtons, PrevNextEventButtons, CXTPCalendarThemePart)

	//=======================================================================
	CXTPCalendarThemeOffice2007();
	virtual ~CXTPCalendarThemeOffice2007();

	// If enough space on the rect - draw 3 strings one under other
	// as multi-line text.
	// If rect height allow draw only 1 line - string 1 an 2 are drawn
	// as single line with a separator.
	// Separate font settings are used to draw each text.
	virtual CSize DrawText_Auto2SL3ML(CDC* pDC,
					LPCTSTR pcszText1, LPCTSTR pcszText2, LPCTSTR pcszText3,
					CThemeFontColorSet* pFontColor1, CThemeFontColorSet* pFontColor2,
					CThemeFontColorSet* pFontColor3,
					CRect& rcRect, LPCTSTR pcszText1Separator, UINT uWordBreakMode = DT_WORDBREAK);


	// Common    colors id's: {0   - 99}
	// DayViev   colors id's: {100 - 499}
	// MonthViev colors id's: {500 - 999}
	// WeekViev  colors id's: {1000 - 1499}
	//
	// For darken colors: id + 5000

	enum XTPEnumThemeColorsSet
	{
		xtpCLR_DarkenOffset = 5000,

		xtpCLR_SelectedBk   = 1 + xtpCLR_DarkenOffset,

		xtpCLR_HeaderBorder = 2,
		xtpCLR_DayBorder    = 3,

		xtpCLR_MultiDayEventBorder          = 10,
		xtpCLR_MultiDayEventSelectedBorder  = 11 + xtpCLR_DarkenOffset,
		xtpCLR_MultiDayEventBkGRfrom        = 12,
		xtpCLR_MultiDayEventBkGRto          = 13,

		xtpCLR_MultiDayEventFromToDates     = 20,

		//xtpCLR_DayView_ = 100,
		xtpCLR_DayViewCellWorkBk                = 101,
		xtpCLR_DayViewCellNonWorkBk             = 102,

		xtpCLR_DayViewCellWorkBorderBottomInHour    = 103,
		xtpCLR_DayViewCellWorkBorderBottomHour      = 104,
		xtpCLR_DayViewCellNonWorkBorderBottomInHour = 105,
		xtpCLR_DayViewCellNonWorkBorderBottomHour   = 106,

		xtpCLR_DayViewAllDayEventsBk            = 110,
		xtpCLR_DayViewAllDayEventsBorderBottom  = 111,

		xtpCLR_DayViewSingleDayEventBorder      = 120,
		xtpCLR_DayViewSingleDayEventSelectedBorder= 121 + xtpCLR_DarkenOffset,
		xtpCLR_DayViewSingleDayEventBkGRfrom    = 122,
		xtpCLR_DayViewSingleDayEventBkGRto      = 123,

		//xtpCLR_MonthView_ = 500,
		xtpCLR_MonthViewDayBkLight                  = 501,
		xtpCLR_MonthViewDayBkDark                   = 502,
		xtpCLR_MonthViewDayBkSelected               = 503,

		xtpCLR_MonthViewEventTime                   = 510 + xtpCLR_DarkenOffset,

		xtpCLR_MonthViewSingleDayEventBorder        = 520,
		xtpCLR_MonthViewSingleDayEventSelectedBorder= 521 + xtpCLR_DarkenOffset,
		xtpCLR_MonthViewSingleDayEventBkGRfrom      = 522,
		xtpCLR_MonthViewSingleDayEventBkGRto        = 523,

		//xtpCLR_WeekView_ = 1000,
		xtpCLR_WeekViewDayBkLight                  = 1001,
		xtpCLR_WeekViewDayBkDark                   = 1002,
		xtpCLR_WeekViewDayBkSelected               = 1003,

		xtpCLR_WeekViewEventTime                   = 1010 + xtpCLR_DarkenOffset,

		xtpCLR_WeekViewSingleDayEventBorder         = 1020,
		xtpCLR_WeekViewSingleDayEventSelectedBorder = 1021 + xtpCLR_DarkenOffset,
		xtpCLR_WeekViewSingleDayEventBkGRfrom       = 1022,
		xtpCLR_WeekViewSingleDayEventBkGRto         = 1023,

	};

public:
	virtual CXTPCalendarViewEventSubjectEditor* StartEditSubject(CXTPCalendarViewEvent* pViewEvent);

	static int DrawClock(CDC* pDC, COleDateTime dtClockTime, CRect rcView, COLORREF clrBackground, XTPCalendarClockAlignFlags cafAlign);
	//-----------------------------------------------------------------------
	// Summary:
	//     Thin method determine event view type (day, week, month) and
	//     return corresponding drawing part.
	// Returns:
	//     Theme drawing part for editing event.
	//-----------------------------------------------------------------------
	virtual CTOEvent* GetThemePartForEvent(CXTPCalendarViewEvent* pViewEvent);

	virtual void GetItemTextIfNeed(int nItem, CString* pstrText, CXTPCalendarViewDay* pViewDay);

	static const int cnGripperSize;
	static const int cnShadowWidth;
	BOOL m_bFlatStyle;

	static COLORREF AFX_CDECL PowerColor(COLORREF clrB, double dFactor, double dPower);

	void DrawEventRect(CDC* pDC, const CRect& rcRect, COLORREF clrBorder,
		BOOL bBoldBorder, CXTPPaintManagerColorGradient& grclrBk);


protected:
	virtual void SetCalendarControl(CXTPCalendarControl* pCalendar);
	virtual void RefreshMetrics(BOOL bRefreshChildren = TRUE);

	virtual void _DrawEventLTHighlight(CDC* pDC, CPoint ptLT, int nRTx, const CXTPPaintManagerColorGradient& grclrBk,
										int nLTCorner = 0, int nLBy = INT_MAX, BOOL bRCorner = TRUE);

protected:

	CXTPNotifySink* m_pSink;
	virtual void OnEvent_ResourceImagesChanged(XTP_NOTIFY_CODE Event, WPARAM wParam, LPARAM lParam);



protected:

};
//}}AFX_CODEJOCK_PRIVATE


/////////////////////////////////////////////////////////////////////////////

#if (_MSC_VER > 1100)
#pragma warning(pop)
#endif

#endif // !defined(_XTP_CALENDAR_THEME_OFFICE_2007_H__)
