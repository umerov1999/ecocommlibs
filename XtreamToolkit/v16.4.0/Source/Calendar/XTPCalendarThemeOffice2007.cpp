// XTPCalendarThemeOffice2007.cpp : implementation file
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
#include <math.h>

#include "Resource.h"

#include "Common/XTPResourceManager.h"
#include "Common/XTPResourceImage.h"
#include "Common/XTPImageManager.h"
#include "Common/XTPPropExchange.h"
#include "Common/XTPMarkupRender.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPNotifyConnection.h"
#include "Common/XTPSmartPtrInternalT.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPVC80Helpers.h"

#include "XTPCalendarPtrCollectionT.h"

#include "XTPCalendarDefines.h"
#include "XTPCalendarUtils.h"
#include "XTPCalendarOptions.h"
#include "XTPCalendarNotifications.h"

#include "XTPCalendarResource.h"

#include "XTPCalendarEvent.h"
#include "XTPCalendarEvents.h"
#include "XTPCalendarEventLabel.h"

#include "XTPCalendarView.h"
#include "XTPCalendarViewEvent.h"
#include "XTPCalendarViewDay.h"
#include "XTPCalendarViewPart.h"

#include "XTPCalendarDayView.h"
#include "XTPCalendarDayViewEvent.h"
#include "XTPCalendarDayViewDay.h"
#include "XTPCalendarDayViewTimeScale.h"

#include "XTPCalendarWeekView.h"
#include "XTPCalendarWeekViewEvent.h"
#include "XTPCalendarWeekViewDay.h"

#include "XTPCalendarMonthViewEvent.h"
#include "XTPCalendarMonthView.h"
#include "XTPCalendarMonthViewDay.h"

#include "XTPCalendarData.h"

#include "XTPCalendarControl.h"

#include "XTPCalendarTheme.h"
#include "XTPCalendarThemePrevNextEventButton.h"
#include "XTPCalendarThemePrevNextEventButtons.h"

#include "XTPCalendarThemeOffice2007.h"
#include "XTPCalendarPaintManager.h"
#include <Calendar/XTPCalendarShadowDrawer.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// DBG
//#define DBG_TRACE_PREVNEXTBTN  TRACE
//#define DBG_TRACE_PREVNEXTBTN_ARR  TRACE
//#define DBG_ASSERT_PREVNEXTBTN  ASSERT

#ifndef DBG_TRACE_PREVNEXTBTN
	#define DBG_TRACE_PREVNEXTBTN
#endif
#ifndef DBG_TRACE_PREVNEXTBTN_ARR
	#define DBG_TRACE_PREVNEXTBTN_ARR
#endif
#ifndef DBG_ASSERT_PREVNEXTBTN
	#define DBG_ASSERT_PREVNEXTBTN
#endif
// DBG

//===========================================================================
//
#define XTP_CALENDAR_OFFICE2007_BASE_BLUE   RGB(93, 140, 201) // default base color for a "blue" theme
#define XTP_CALENDAR_OFFICE2007_BASE_GREEN  RGB(114, 164, 90) // default base color for a "green" theme

#define XTP_CALENDAR_OFFICE2007_TODAYBORDER RGB(238, 147, 17)

//===========================================================================
// Should be a prime number:
// 37, 53, 79 , 101, 127, 199, 503, 1021, 1511, 2003, 3001, 4001, 5003, 6007, 8009, 12007, 16001, 32003, 48017, 64007
#define XTP_CALENDAR_OFFICE2007_DVG_DEV_AVE_COUNT 37

#define XTP_CALCLR(clrName, nScale) GetColorsSetPart()->SetColor(xtpCLR_##clrName, _T(#clrName), nScale);

static const double dXTPMaxAlpha2007 = 90.;
const int CXTPCalendarThemeOffice2007::cnGripperSize = 5;
const int CXTPCalendarThemeOffice2007::cnShadowWidth = 5;

//////////////////////////////////////////////////////////////////////////
//{{AFX_CODEJOCK_PRIVATE
struct XTP_CALENDAR_EVENT_ICON_INFO
{
	// common data
	RECT                 m_rcDestination;

	// image data
	CXTPResourceImage* m_pImage;
	RECT                 m_rcSrc;

	// icon data
	CXTPImageManagerIcon* m_pIcon;
};
//}}AFX_CODEJOCK_PRIVATE

//{{AFX_CODEJOCK_PRIVATE
class CXTPTextDC
{
public:

	CXTPTextDC(CDC* pDC, COLORREF clrText = COLORREF_NULL, COLORREF clrBk = COLORREF_NULL)
	{
		ASSERT(pDC);

		m_pDC = pDC;
		m_clrText_prev = COLORREF_NULL;
		m_clrBk_prev = COLORREF_NULL;

		SetColors(clrText, clrBk);
	}

	virtual ~CXTPTextDC()
	{
		SetColors(m_clrText_prev, m_clrBk_prev);
	}

	void SetColors(COLORREF clrText, COLORREF clrBk = COLORREF_NULL)
	{
		ASSERT(m_pDC);
		if (!m_pDC)
			return;

		if (clrText != COLORREF_NULL)
		{
			COLORREF clrText_prev = m_pDC->SetTextColor(clrText);
			if (m_clrText_prev == COLORREF_NULL)
				m_clrText_prev = clrText_prev;
		}

		if (clrBk != COLORREF_NULL)
		{
			COLORREF clrBk_prev = m_pDC->SetBkColor(clrBk);
			if (m_clrBk_prev == COLORREF_NULL)
				m_clrBk_prev = clrBk_prev;
		}
	}

protected:
	CDC*     m_pDC;

	COLORREF m_clrText_prev;
	COLORREF m_clrBk_prev;
};
//}}AFX_CODEJOCK_PRIVATE


//////////////////////////////////////////////////////////////////////////
//class CXTPCalendarTheme_DayViewSingleDayEventPlaceBuilder
//{{AFX_CODEJOCK_PRIVATE
class CXTPCalendarTheme_DayViewSingleDayEventPlaceBuilder
{
public:
	CXTPCalendarTheme_DayViewSingleDayEventPlaceBuilder();

	virtual void BuildEventPlaces(CXTPCalendarDayViewGroup* pViewGroup);

protected:
	class CViewEventsMap : public CMap<CXTPCalendarDayViewEvent*, CXTPCalendarDayViewEvent*, int, int>
	{
	public:
		CViewEventsMap();
		int GetMinFreeEventPlace();
		int GetMaxEventPlace();
		int GetMaxEventPlaceCountEx();
	};

	virtual void _Clear();
	virtual void _UpdateDataFor(CXTPCalendarDayViewEvent* pViewEvent, CXTPCalendarDayViewGroup* pViewGroup);
	virtual void _UpdateEventsPaces(CXTPCalendarDayViewGroup* pViewGroup);

	//CXTPCalendarTypedPtrAutoDeleteMap<CXTPCalendarDayViewEvent*, CViewEventsMap*> m_mapEvToOverlapped;
	// to avoid warning C4786 CObject* used.
	CXTPCalendarTypedPtrAutoDeleteMap<CXTPCalendarDayViewEvent*, CObject*> m_mapEvToOverlapped;

	COleDateTimeSpan m_spCellDuration;
	COleDateTime     m_dtMinStartTime;
protected:
	virtual CViewEventsMap* _XGetDataFor(CXTPCalendarDayViewEvent* pViewEvent);

	virtual void _XAddOverlapped(CXTPCalendarDayViewEvent* pVEv_base, CXTPCalendarDayViewEvent* pVEv_overlapped);
};
//}}AFX_CODEJOCK_PRIVATE

/////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CXTPCalendarThemeOffice2007, CXTPCalendarTheme)

////////////////////////////////////////////////////////////////////////
CSize CXTPCalendarThemeOffice2007::DrawText_Auto2SL3ML(CDC* pDC,
		LPCTSTR pcszText1, LPCTSTR pcszText2, LPCTSTR pcszText3,
		CThemeFontColorSet* pFontColor1, CThemeFontColorSet* pFontColor2,
		CThemeFontColorSet* pFontColor3,
		CRect& rcRect, LPCTSTR pcszText1Separator, UINT uWordBreakMode)

{
	CXTPFontDC autoFont(pDC, pFontColor2->Font());
	CSize sz2 = pDC->GetTextExtent(pcszText2);

	autoFont.SetFont(pFontColor1->Font());
	CSize sz1 = pDC->GetTextExtent(pcszText1);

	int height = rcRect.Height();

	if ((sz1.cy + sz2.cy + 1) > height)
	{
		CString strText1 = pcszText1;
		if (_tcslen(pcszText2) > 0)
		{
			strText1 += pcszText1Separator;
		}
		return DrawLine2_CenterLR(pDC, strText1, pcszText2, pFontColor1,
			pFontColor2, rcRect, DT_VCENTER | DT_LEFT, DT_VCENTER | DT_LEFT);
	}

	UINT uFormat = DT_NOPREFIX | DT_LEFT | DT_TOP | DT_EDITCONTROL | uWordBreakMode;
	int nYspace = sz2.cy/4;

	int nHeight1 = 0, nHeight2 = 0, nHeight3 = 0;
	autoFont.SetColor(pFontColor1->Color());
	CRect rcText1 = rcRect;
	nHeight1 = pDC->DrawText(pcszText1, &rcText1, uFormat);

	CRect rcText2 = rcRect;
	rcText2.top = min(rcRect.top + nHeight1 + nYspace, rcRect.bottom);

	if (/*sz2.cy < rcText2.Height() &&*/ _tcslen(pcszText2))
	{
		autoFont.SetFontColor(pFontColor2->Font(), pFontColor2->Color());
		nHeight2 = pDC->DrawText(pcszText2, &rcText2, uFormat);
	}

	int nTLen3 = (int)_tcslen(pcszText3);
	if (pcszText3 && nTLen3)
	{
		autoFont.SetFontColor(pFontColor3->Font(), pFontColor3->Color());

		CRect rcText3 = rcRect;
		rcText3.top = min(rcText2.top + nHeight2 + nYspace, rcRect.bottom);

		if (sz2.cy < rcText3.Height())
		{
			nHeight3 = pDC->DrawText(pcszText3, &rcText3, uFormat);
		}
	}

	return CSize(rcRect.Width(), nHeight1 + nHeight2 + nHeight3 + 2 * nYspace);
}

COLORREF AFX_CDECL CXTPCalendarThemeOffice2007::PowerColor(COLORREF clrBase, double dFactor, double dPower)
{
	BYTE bR = (BYTE)max(0, min(255, (int)GetRValue(clrBase) + (int)(pow((double)GetRValue(clrBase), dPower) * dFactor)) );
	BYTE bG = (BYTE)max(0, min(255, (int)GetGValue(clrBase) + (int)(pow((double)GetGValue(clrBase), dPower) * dFactor)) );
	BYTE bB = (BYTE)max(0, min(255, (int)GetBValue(clrBase) + (int)(pow((double)GetBValue(clrBase), dPower) * dFactor)) );

	return RGB(bR, bG, bB);
}
/////////////////////////////////////////////////////////////////////////////
CXTPCalendarThemeOffice2007::CXTPCalendarThemeOffice2007()
{
	m_pSink = new CXTPNotifySink();

	m_bFlatStyle = FALSE;
	m_nPaintTheme = xtpCalendarThemeResource;

	Init();

	RefreshMetrics();

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif
}

CXTPCalendarThemeOffice2007::~CXTPCalendarThemeOffice2007()
{
	m_pSink->Delete();

	if (m_pCalendar)
		OnDetachCalendar();
}

void CXTPCalendarThemeOffice2007::DrawEventRect(CDC* pDC, const CRect& rcRect, COLORREF clrBorder,
		BOOL bBoldBorder, CXTPPaintManagerColorGradient& grclrBk)
{
	if (m_bFlatStyle)
	{
		DrawRect(pDC, rcRect, clrBorder, bBoldBorder, grclrBk);
	}
	else
	{
		DrawRoundRect(pDC, rcRect, clrBorder, bBoldBorder, grclrBk);
	}
}

int CXTPCalendarThemeOffice2007::DrawClock(CDC* pDC, COleDateTime dtClockTime,
	CRect rcView, COLORREF clrBackground, XTPCalendarClockAlignFlags cafAlign)
{
	ASSERT(pDC);
	if (!pDC)
		return 0;

	static CSize m_szClkRc(15,15);
	// Stores dimensions for a rectangle used to display the time as a clock glyph.

	static const CPoint arrHours[] =
	{
			CPoint(0, -6),
			CPoint(3, -5),
			CPoint(5, -3),
			CPoint(6, 0),
			CPoint(5, 3),
			CPoint(3, 5),
			CPoint(0, 6),
			CPoint(-3, 5),
			CPoint(-5, 3),
			CPoint(-6, 0),
			CPoint(-5, -3),
			CPoint(-3, -5),
			CPoint(0, -6)
	};

	static const CPoint arrHalfHours[] =
	{
			CPoint(1, -5),
			CPoint(3, -3),
			CPoint(4, -1),
			CPoint(5, 1),
			CPoint(3, 3),
			CPoint(1, 5),
			CPoint(-1, 5),
			CPoint(-3, 3),
			CPoint(-5, 1),
			CPoint(-5, -1),
			CPoint(-3, -3),
			CPoint(-1, -5),
			CPoint(1, -5)
	};

	if (rcView.Width() < m_szClkRc.cx || rcView.Height() < m_szClkRc.cy)
		return 0;

	CPoint ptClock(rcView.left, rcView.top);

	if (cafAlign == xtpCalendarClockAlignLeft)
		ptClock.Offset(1, (rcView.Height() - m_szClkRc.cy)/2 + 1);
	else if (cafAlign == xtpCalendarClockAlignRight)
		ptClock.Offset(rcView.Width() - m_szClkRc.cx - 1,
			(rcView.Height() - m_szClkRc.cy)/2 + 1);
	else if (cafAlign == xtpCalendarClockAlignCenter)
		ptClock.Offset((rcView.Width() - m_szClkRc.cx)/2,
			(rcView.Height() - m_szClkRc.cy)/2 + 1);

	CRect rcClock(0, 0, 0, 0);
	rcClock.SetRect(ptClock.x, ptClock.y, ptClock.x + m_szClkRc.cx, ptClock.y + m_szClkRc.cy);

	int nHour = dtClockTime.GetHour();
	int nMin = dtClockTime.GetMinute() /5;

	COLORREF clrFill;
	COLORREF clrItems;
	if (nHour >= 12)
	{
		clrFill = RGB(0, 0, 0);
		clrItems = clrBackground;
	}
	else
	{
		clrFill = clrBackground;
		clrItems = RGB(0, 0, 0);
	}


	nHour = nHour > 12 ? nHour - 12 : nHour;

	CXTPBrushDC brushClock(pDC->m_hDC, clrFill);
	CPoint ptCenter(rcClock.CenterPoint());
	pDC->Ellipse(rcClock);
	CXTPPenDC penItems(pDC->m_hDC, clrItems);
	int x = 0;
	int y = 0;

	for (int i = 0; i <= 12; i++)
	{
		pDC->SetPixel(ptCenter.x + arrHours[i].x, ptCenter.y + arrHours[i].y, clrItems);

		if (nHour == i)
		{
			if (nMin >= 6)
			{
				x = arrHalfHours[i].x;
				y = arrHalfHours[i].y;
			}
			else
			{
				x = arrHours[i].x;
				y = arrHours[i].y;
				x = x > 0 ? x -1 : x == 0 ? 0 : x + 1;
				y = y > 0 ? y -1 : y == 0 ? 0 : y + 1;
			}
			pDC->MoveTo(ptCenter);
			pDC->LineTo(ptCenter.x+ x, ptCenter.y + y);
		}
		if (nMin == i)
		{
			pDC->MoveTo(ptCenter);
			pDC->LineTo(ptCenter.x + arrHours[i].x,
						ptCenter.y + arrHours[i].y);
		}
	}

	return m_szClkRc.cx;

}

CXTPCalendarViewEventSubjectEditor* CXTPCalendarThemeOffice2007::StartEditSubject(CXTPCalendarViewEvent* pViewEvent)
{
	if (!pViewEvent || !pViewEvent->GetCalendarControl())
	{
		ASSERT(FALSE);
		return NULL;
	}

	CXTPCalendarThemeOffice2007::CTOEvent* pEventPart = GetThemePartForEvent(pViewEvent);
	ASSERT(pEventPart);

	COLORREF clrBorderNotSel;
	COLORREF clrBorder;
	CXTPPaintManagerColorGradient grclrBk;
	pEventPart->GetEventColors(NULL, pViewEvent, clrBorder, clrBorderNotSel, grclrBk, FALSE);

	CXTPCalendarViewEventSubjectEditorColored* pSE = new
		CXTPCalendarViewEventSubjectEditorColored(pViewEvent->GetCalendarControl(), pViewEvent, grclrBk);

	if (!pSE)
		return NULL;

	DWORD dwEditStyles = WS_CHILD | WS_VISIBLE | ES_NOHIDESEL | ES_MULTILINE;

	if (pViewEvent->IsMultidayEvent())
		dwEditStyles |= ES_AUTOHSCROLL /*| ES_CENTER*/;
	else
		dwEditStyles |= ES_AUTOVSCROLL | ES_MULTILINE;

	CRect rcEditor = pViewEvent->GetSubjectEditorRect();
	pSE->Create(dwEditStyles, rcEditor, pViewEvent->GetCalendarControl(), 0);
	pSE->SetFocus();


	if (pEventPart)
	{
		CFont* pEditorFont = pEventPart->m_fcsetNormal.fcsetSubject.Font();
		ASSERT(pEditorFont);
		if (pEditorFont)
			pSE->SetFont(pEditorFont);
	}
	pSE->SetWindowText(XTP_SAFE_GET2(pViewEvent, GetEvent(), GetSubject(), _T("")));
	pSE->SetSel(0, 0);

	return pSE;
}

CXTPCalendarThemeOffice2007::CTOEvent* CXTPCalendarThemeOffice2007::GetThemePartForEvent(CXTPCalendarViewEvent* pViewEvent)
{
	if (!pViewEvent)
	{
		ASSERT(FALSE);
		return NULL;
	}

	CXTPCalendarThemeOffice2007::CTOEvent* pEventPart = NULL;

	CXTPCalendarDayViewEvent* pVEDay = DYNAMIC_DOWNCAST(CXTPCalendarDayViewEvent, pViewEvent);
	CXTPCalendarWeekViewEvent* pVEWeek= DYNAMIC_DOWNCAST(CXTPCalendarWeekViewEvent, pViewEvent);
	CXTPCalendarMonthViewEvent* pVEMonth = DYNAMIC_DOWNCAST(CXTPCalendarMonthViewEvent, pViewEvent);
	if (pVEDay)
	{
		if (pViewEvent->IsMultidayEvent())
		{
			pEventPart = GetDayViewPartX()->GetDayPartX()->GetGroupPartX()->GetMultiDayEventPartX();
		}
		else
		{
			pEventPart = GetDayViewPartX()->GetDayPartX()->GetGroupPartX()->GetSingleDayEventPartX();
		}
	}
	else if (pVEWeek)
	{
		if (pViewEvent->IsMultidayEvent())
		{
			pEventPart = GetWeekViewPartX()->GetDayPartX()->GetMultiDayEventPartX();
		}
		else
		{
			pEventPart = GetWeekViewPartX()->GetDayPartX()->GetSingleDayEventPartX();
		}
	}
	else if (pVEMonth)
	{
		if (pViewEvent->IsMultidayEvent())
		{
			pEventPart = GetMonthViewPartX()->GetDayPartX()->GetMultiDayEventPartX();
		}
		else
		{
			pEventPart = GetMonthViewPartX()->GetDayPartX()->GetSingleDayEventPartX();
		}
	}
	else
	{
		ASSERT(FALSE);
	}

	return pEventPart;
}

void CXTPCalendarThemeOffice2007::GetItemTextIfNeed(int nItem, CString* pstrText,
													CXTPCalendarViewDay* pViewDay)
{
	if (!pstrText || !pViewDay)
	{
		ASSERT(FALSE);
		return;
	}

	DWORD dwFlags = GetAskItemTextFlags();

	if (dwFlags & nItem)
	{
		XTP_CALENDAR_GETITEMTEXT_PARAMS objRequest;
		::ZeroMemory(&objRequest, sizeof(objRequest));

		objRequest.nItem = nItem;
		objRequest.pstrText = pstrText;
		objRequest.pViewDay = pViewDay;

		SendNotificationAlways(XTP_NC_CALENDAR_GETITEMTEXT, (WPARAM)&objRequest, 0);
	}
}

void CXTPCalendarThemeOffice2007::SetCalendarControl(CXTPCalendarControl* pCalendar)
{
	m_pSink->UnadviseAll();

	CXTPCalendarTheme::SetCalendarControl(pCalendar);

	if (pCalendar && XTPResourceImages() && XTPResourceImages()->GetConnection())
	{
		m_pSink->Advise(XTPResourceImages()->GetConnection(), XTP_NC_COMMON_RESOURCEIMAGES_CHANGED,
			CreateNotfySinkClassDelegate(this, &CXTPCalendarThemeOffice2007::OnEvent_ResourceImagesChanged));
	}
}

void CXTPCalendarThemeOffice2007::OnEvent_ResourceImagesChanged(XTP_NOTIFY_CODE Event, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam); UNREFERENCED_PARAMETER(lParam);

	ASSERT(Event == XTP_NC_COMMON_RESOURCEIMAGES_CHANGED);
	if (Event == XTP_NC_COMMON_RESOURCEIMAGES_CHANGED)
	{
		RefreshMetrics();

		if (GetCalendarControl())
			GetCalendarControl()->RedrawControl();
	}
}

void CXTPCalendarThemeOffice2007::RefreshMetrics(BOOL bRefreshChildren)
{
	CXTPCalendarTheme::RefreshMetrics(FALSE);

	CXTPResourceImages* pImages = XTPResourceImages();

	pImages->AssertValid();

	m_clrBaseColor.SetStandardValue(GetColor2(_T("BaseColor"), XTP_CALENDAR_OFFICE2007_BASE_BLUE));
	//m_clrBaseColor.SetStandardValue(XTP_CALENDAR_OFFICE2007_BASE_GREEN);

	// Try to set default Office 2007 font,
	// otherwise icon font used.
	BOOL bFontExists = XTPDrawHelpers()->FontExists(XTP_CALENDAR_OFFICE2007_FONT_NAME);
	if (bFontExists)
	{
		LOGFONT lfBaseFont;
		::ZeroMemory(&lfBaseFont, sizeof(lfBaseFont));

		STRCPY_S(lfBaseFont.lfFaceName, LF_FACESIZE, XTP_CALENDAR_OFFICE2007_FONT_NAME);
		lfBaseFont.lfCharSet = XTPResourceManager()->GetFontCharset();
		lfBaseFont.lfHeight = -11;
		lfBaseFont.lfWeight = FW_NORMAL;
		if (XTPSystemVersion()->IsClearTypeTextQualitySupported())
			lfBaseFont.lfQuality = 5;


		m_fntBaseFont.SetStandardValue(&lfBaseFont);

		LOGFONT lfBaseFontBold = lfBaseFont;
		lfBaseFontBold.lfWeight = FW_BOLD;

		m_fntBaseFontBold.SetStandardValue(&lfBaseFontBold);
	}

	m_bFlatStyle = XTPResourceImages()->GetImageInt(_T("CalendarControl"), _T("FlatStyle"), FALSE);

	if (!GetColorsSetPart() || !m_pImagesStd || !m_pImageList)
	{
		return;
	}

/*
			rgb (HSL)
			Blue                                  Silver                                Green
			141,174, 217 (142, 118, 168)  *  30   176, 182, 190 (142, 22, 172)  *   27  156, 191, 139 (66, 68, 155)  *   36
DayBorder   93, 140, 201 (142, 118, 138) -30 *    145, 153, 164 (142, 21, 145) -27  *   114, 164, 90  (66, 68, 119) -36  *
NonWorkBk   230,237, 247 (143, 116, 224)  56 -86  232, 234, 236 (139, 21, 220)  48 -75  233, 241, 230 (68, 64, 222)  67  103
sel cell    41, 76, 122  (142, 120, 77)  -91  61  76, 83, 92    (142, 23, 79)  -93  66  63, 91, 50    (66, 71, 66)  -89 -53
ADEventsBk  165, 191, 225(142, 117, 183)      45  199,203, 209  (143, 22, 192)      47  177, 205, 164 (66, 68, 174)

*/  //-- set dynamic colors -------------------------
 // selected cell for blue, green
	// 41, 76, 122  {93, 140, 201}
	// 63, 91, 50 [114, 164, 90]  114

	GetColorsSetPart()->SetCLR_DarkenOffset(xtpCLR_DarkenOffset);

	//GetColorsSetPart()->SetColor(xtpCLR_
	XTP_CALCLR(SelectedBk, 114); // 51, 77, 111

	XTP_CALCLR(HeaderBorder, 76); // 141, 174, 217

	// Day View colors
	XTP_CALCLR(DayBorder,                      0);   // 93, 140, 201

	XTP_CALCLR(DayViewCellWorkBk,              255); // 255, 255, 255
	XTP_CALCLR(DayViewCellNonWorkBk,           216); // 230, 237, 247

	XTP_CALCLR(DayViewCellWorkBorderBottomInHour, 216); // 230, 237, 247
	XTP_CALCLR(DayViewCellWorkBorderBottomHour,   113); // 165, 191, 225

	XTP_CALCLR(DayViewCellNonWorkBorderBottomInHour, 189); // 213, 225, 241
	XTP_CALCLR(DayViewCellNonWorkBorderBottomHour,   113); // 165, 191, 225

	XTP_CALCLR(DayViewAllDayEventsBk,                113); // 165, 191, 225
	//GetColorsSetPart()->SetColorHSL(xtpCLR_DayViewAllDayEventsBk,                113); // 165, 191, 225
	XTP_CALCLR(DayViewAllDayEventsBorderBottom,      0);   // 93, 140, 201

	XTP_CALCLR(MultiDayEventBorder,         0);   // 93, 140, 201
	XTP_CALCLR(MultiDayEventSelectedBorder, 255); // 0, 0, 0
	XTP_CALCLR(MultiDayEventBkGRfrom,       249); // 251, 252, 254  multi-day event gradient fill Light
	XTP_CALCLR(MultiDayEventBkGRto,         161); // 195, 213, 235 multi-day event gradient fill Dark

	XTP_CALCLR(MultiDayEventFromToDates,     38); // 117, 157, 209

	XTP_CALCLR(DayViewSingleDayEventBorder,          0);   // 93, 140, 201
	XTP_CALCLR(DayViewSingleDayEventSelectedBorder,  255); // 0, 0, 0
	XTP_CALCLR(DayViewSingleDayEventBkGRfrom,        255); // 255, 255, 255 // single-day event gradient fill Light
	XTP_CALCLR(DayViewSingleDayEventBkGRto,          155); // 191, 210, 234 // single-day event gradient fill Dark

	// Month view colors
	XTP_CALCLR(MonthViewDayBkLight,    255); // 255, 255, 255
	XTP_CALCLR(MonthViewDayBkDark,     113); // 165, 191, 225
	XTP_CALCLR(MonthViewDayBkSelected, 216); // 230, 237, 247

	XTP_CALCLR(MonthViewEventTime,  38);

	XTP_CALCLR(MonthViewSingleDayEventBorder,          217); // 230, 238, 247
	XTP_CALCLR(MonthViewSingleDayEventSelectedBorder,  255); // 0, 0, 0
	XTP_CALCLR(MonthViewSingleDayEventBkGRfrom,        250); // 252, 253, 254
	XTP_CALCLR(MonthViewSingleDayEventBkGRto,          181); // 208, 222, 240

	// Week view colors are same as month view by default
	XTP_CALCLR(WeekViewDayBkLight,    255); // 255, 255, 255
	XTP_CALCLR(WeekViewDayBkDark,     113); // 165, 191, 225
	XTP_CALCLR(WeekViewDayBkSelected, 216); // 230, 237, 247

	XTP_CALCLR(WeekViewEventTime,  114); // 51, 77, 111

	XTP_CALCLR(WeekViewSingleDayEventBorder,          217); // 230, 238, 247
	XTP_CALCLR(WeekViewSingleDayEventSelectedBorder,  255); // 0, 0, 0
	XTP_CALCLR(WeekViewSingleDayEventBkGRfrom,        250); // 252, 253, 254
	XTP_CALCLR(WeekViewSingleDayEventBkGRto,          181); // 208, 222, 240

	if (m_pImagesStd)
		VERIFY( m_pImagesStd->InitResourceHandle(_T("Office2007_CalendarHeader_bmp"), _T("PNG")));

	_LoadStdBitmap(xtpIMG_DayViewHeader,        _T("CalendarHeader"));
	_LoadStdBitmap(xtpIMG_DayViewHeaderTooday,  _T("CalendarTodayHeader"));
	_LoadStdBitmap(xtpIMG_DayViewGroupHeader,   _T("CalendarHeaderDVGroup"));

	_LoadStdBitmap(xtpIMG_MonthViewDayHeader,       _T("CalendarHeaderMVDay"));
	_LoadStdBitmap(xtpIMG_MonthViewDayHeaderToday,  _T("CalendarTodayHeaderMVDay"));
	//_LoadStdBitmap(xtpIMG_MonthViewWeekDayHeader,   _T("CalendarHeaderMVWeekDay"));
	_LoadStdBitmap(xtpIMG_MonthViewWeekHeader,      _T("CalendarHeaderMVWeek"));
	_LoadStdBitmap(xtpIMG_WeekViewDayHeader,        _T("CalendarHeaderWVDay"));

	_LoadStdBitmap(xtpIMG_ExpandDayButtons,         _T("CalendarExpandDay"));
	_LoadStdBitmap(xtpIMG_PrevNextEventButtons,   _T("CalendarPrevNextEventButton"));

	if (bRefreshChildren)
		CXTPCalendarThemePart::RefreshMetrics();
}

BOOL CXTPCalendarThemeOffice2007::IsBaseColorOrig()
{
	return !m_clrBaseColor.IsCustomValue() ||
			m_clrBaseColor.GetStandardColor() == (COLORREF)m_clrBaseColor;
}

COLORREF CXTPCalendarThemeOffice2007::GetColor(int eCLR, COLORREF clrBaseColor)
{
	COLORREF clrColor = COLORREF_NULL;

	CXTPResourceImages* pImages = XTPResourceImages();

	if (pImages->IsValid() && IsBaseColorOrig())
	{
		CString strColorName = GetColorsSetPart()->GetName(eCLR);
		clrColor = pImages->GetImageColor(_T("CalendarControl"), strColorName);
	}

	if (clrColor == COLORREF_NULL)
	{
		clrColor = GetColorsSetPart()->GetColor(eCLR, clrBaseColor);
	}

	return clrColor;
}

COLORREF CXTPCalendarThemeOffice2007::GetColor2(LPCTSTR pcszColorName, COLORREF clrDefault)
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

CXTPResourceImage* CXTPCalendarThemeOffice2007::GetBitmap(LPCTSTR pcszStdBmpName)
{
	if (!m_pImagesStd || !m_pImageList || !pcszStdBmpName)
	{
		ASSERT(FALSE);
		return NULL;
	}

	CXTPResourceImage* pBmp = NULL;

	CXTPResourceImages* pImages = XTPResourceImages();

	if (pImages->IsValid() && IsBaseColorOrig())
	{
		pBmp = pImages->LoadFile(pcszStdBmpName);
	}

	if (!pBmp)
	{
		CString strName;
		strName.Format(_T("Office2007_%s_BMP"), pcszStdBmpName);
		pBmp = m_pImagesStd->LoadFile(strName);
	}

	if (!pBmp && !IsBaseColorOrig() && pImages->IsValid())
	{
		pBmp = pImages->LoadFile(pcszStdBmpName);
	}

	return pBmp;
}

int CXTPCalendarThemeOffice2007::CTODay::HitTestExpandDayButton(const CXTPCalendarViewDay* pViewDay, const CPoint* pPoint)
{
	if (!pViewDay || !GetTheme() || !GetTheme()->GetCalendarControl())
	{
		ASSERT(FALSE);
		return 0;
	}

	const XTP_VIEW_DAY_LAYOUT& dayLayout = pViewDay->GetLayout_();
	CRect rc = dayLayout.m_rcDay;
	rc.right -= 5;
	rc.bottom -= 5;

	return _HitTestExpandDayButton(pViewDay, rc, pPoint);
}

void CXTPCalendarThemeOffice2007::CTODayViewHeader::RefreshMetrics(BOOL bRefreshChildren)
{
	CXTPCalendarThemeOffice2007* pThemeX = DYNAMIC_DOWNCAST(CXTPCalendarThemeOffice2007, GetTheme());
	ASSERT(pThemeX);
	if (!pThemeX)
	{
		return;
	}

	TBase::RefreshMetrics(bRefreshChildren);

	RefreshFromParent(pThemeX->GetHeaderPartX());
}

void CXTPCalendarThemeOffice2007::CTODayView::RefreshMetrics(BOOL bRefreshChildren)
{
	CXTPCalendarThemeOffice2007* pThemeX = DYNAMIC_DOWNCAST(CXTPCalendarThemeOffice2007, GetTheme());
	ASSERT(pThemeX);
	if (!pThemeX)
	{
		return;
	}

	TBase::RefreshMetrics(bRefreshChildren);

	if (pThemeX->GetPrevNextEventButtonsPartX())
		pThemeX->GetPrevNextEventButtonsPartX()->RefreshMetrics(bRefreshChildren);
}

void CXTPCalendarThemeOffice2007::CTODayView::AdjustLayout(CDC* pDC, const CRect& rcRect)
{
	CXTPCalendarThemeOffice2007* pThemeX = DYNAMIC_DOWNCAST(CXTPCalendarThemeOffice2007, GetTheme());
	ASSERT(pThemeX);
	if (!pThemeX)
	{
		return;
	}

	TBase::AdjustLayout(pDC, rcRect);

	CRect rcRect2 = rcRect;
	rcRect2.left = XTP_SAFE_GET3(pThemeX->GetCalendarControl(), GetDayView(), GetViewDay_(0),
								GetDayRect().left, rcRect.left);

	if (pThemeX->GetPrevNextEventButtonsPartX())
		pThemeX->GetPrevNextEventButtonsPartX()->AdjustLayout(NULL, pDC, rcRect2);
}

void CXTPCalendarThemeOffice2007::CTODayView::Draw(CDC* pDC)
{
	CXTPCalendarThemeOffice2007* pThemeX = DYNAMIC_DOWNCAST(CXTPCalendarThemeOffice2007, GetTheme());
	ASSERT(pThemeX);
	if (!pThemeX)
	{
		return;
	}

	TBase::Draw(pDC);

	if (pThemeX->GetPrevNextEventButtonsPartX())
		pThemeX->GetPrevNextEventButtonsPartX()->Draw(NULL, pDC);
};

int CXTPCalendarThemeOffice2007::CTODayViewTimeScale::CalcMinRowHeight(CDC* pDC, CXTPCalendarDayViewTimeScale* pTimeScale)
{
	if (!pDC || !pTimeScale)
	{
		ASSERT(FALSE);
		return 0;
	}

	int nHeight = TBase::CalcMinRowHeight(pDC, pTimeScale);

	if (CXTPCalendarUtils::GetTotalMinutes(pTimeScale->GetScaleInterval()) % 60 == 0 && nHeight > 0)
	{
		nHeight -= 1;
	}

	return nHeight;
}

void CXTPCalendarThemeOffice2007::CTODayViewTimeScale::RefreshMetrics(BOOL)
{
	ASSERT(GetTheme());
	if (!GetTheme())
	{
		return;
	}

	TBase::RefreshMetrics();

	XTP_SAFE_CALL1(GetHeightFormulaPart(), SetStandardValue(18, 13, 5));
	m_hflaBigHourFont.SetStandardValue(1, 1, -1);

	m_clrBackground.SetStandardValue(GetTheme()->GetColor2(_T("TimeScaleBk"), RGB(227, 239, 255)));
	m_clrLine.SetStandardValue(GetTheme()->GetColor2(_T("TimeScaleLine"), RGB(101, 147, 207)));

	COLORREF clrText = GetTheme()->GetColor2(_T("TimeScaleTextColor"), m_clrLine.GetStandardColor());
	m_fcsetCaption.clrColor.SetStandardValue(clrText);

	CopySettings(m_fcsetAMPM.clrColor, clrText);
	CopySettings(m_fcsetSmall.clrColor, clrText);
	CopySettings(m_fcsetBigBase.clrColor, clrText);

	CopySettings(m_fcsetBigHour_.clrColor, m_fcsetBigBase.clrColor);

	m_grclrNowLineBk.SetStandardValue(m_clrBackground, RGB(247, 202, 95));
	m_clrNowLine.SetStandardValue(RGB(187, 85, 3));
}

void CXTPCalendarThemeOffice2007::CTODayViewDay::RefreshMetrics(BOOL bRefreshChildren)
{
	CXTPCalendarThemeOffice2007* pThemeX = DYNAMIC_DOWNCAST(CXTPCalendarThemeOffice2007, GetTheme());
	ASSERT(pThemeX);
	if (!pThemeX)
	{
		return;
	}

	m_clrBorder.SetStandardValue(pThemeX->GetColor(xtpCLR_DayBorder));
	//m_clrTodayBorder.SetStandardValue(XTP_CALENDAR_OFFICE2007_TODAYBORDER);

	TBase::RefreshMetrics(bRefreshChildren);

	m_clrTodayBorder.SetStandardValue(GetHeaderPartX()->m_clrTodayBaseColor);
}

void CXTPCalendarThemeOffice2007::CTODayViewDay::DoPropExchange(CXTPPropExchange* pPX)
{
	ASSERT(pPX);
	if (!pPX)
		return;

	CXTPPropExchangeSection secData(pPX->GetSection(_T("DayViewDay")));
	if (pPX->IsStoring())
		secData->EmptySection();

	PX_Color(&secData, _T("BorderColor"), m_clrBorder);
	PX_Color(&secData, _T("TodayBorderColor"), m_clrTodayBorder);

	TBase::DoPropExchange(&secData);
}

void CXTPCalendarThemeOffice2007::CTODayViewDay::Serialize(CArchive& ar)
{
	m_clrBorder.Serialize(ar);
	m_clrTodayBorder.Serialize(ar);

	TBase::Serialize(ar);
}

void CXTPCalendarThemeOffice2007::CTODayViewDayHeader::RefreshMetrics(BOOL bRefreshChildren)
{
	CXTPCalendarThemeOffice2007* pThemeX = DYNAMIC_DOWNCAST(CXTPCalendarThemeOffice2007, GetTheme());
	ASSERT(pThemeX);
	if (!pThemeX)
	{
		return;
	}

	TBase::RefreshMetrics(bRefreshChildren);

	RefreshFromParent(pThemeX->GetDayViewPartX()->GetHeaderPartX());

//  m_clrTodayBaseColor.SetStandardValue(pThemeX->GetHeaderPartX()->m_clrTodayBaseColor);

	m_UseOffice2003HeaderFormat.SetStandardValue(FALSE);
}

void CXTPCalendarThemeOffice2007::CTODayViewDayGroupHeader::RefreshMetrics(BOOL bRefreshChildren)
{
	CXTPCalendarThemeOffice2007* pThemeX = DYNAMIC_DOWNCAST(CXTPCalendarThemeOffice2007, GetTheme());
	ASSERT(pThemeX);
	if (!pThemeX)
	{
		return;
	}

	TBase::RefreshMetrics(bRefreshChildren);

	RefreshFromParent(pThemeX->GetDayViewPartX()->GetDayPartX()->GetHeaderPartX());

	m_bkNormal.nBitmapID.SetStandardValue(xtpIMG_DayViewGroupHeader);
	m_bkSelected.nBitmapID.SetStandardValue(xtpIMG_DayViewGroupHeader);
	m_bkToday.nBitmapID.SetStandardValue(xtpIMG_DayViewGroupHeader);
	m_bkTodaySelected.nBitmapID.SetStandardValue(xtpIMG_DayViewGroupHeader);
}

void CXTPCalendarThemeOffice2007::CTOHeader::RefreshMetrics(BOOL bRefreshChildren)
{
	CXTPCalendarThemeOffice2007* pThemeX = DYNAMIC_DOWNCAST(CXTPCalendarThemeOffice2007, GetTheme());
	ASSERT(pThemeX);
	if (!pThemeX)
	{
		return;
	}

	TBase::RefreshMetrics(bRefreshChildren);

	m_clrBaseColor.SetStandardValue(pThemeX->m_clrBaseColor);
	m_clrTodayBaseColor.SetStandardValue(pThemeX->GetColor2(_T("TodayBorder"), XTP_CALENDAR_OFFICE2007_TODAYBORDER));

	COLORREF clrText = pThemeX->GetColor2(_T("HeaderTextColor"), RGB(0, 0, 0));

	m_TextLeftRight.fcsetNormal.clrColor.SetStandardValue(clrText);
	m_TextLeftRight.fcsetSelected.clrColor.SetStandardValue(clrText);
	m_TextLeftRight.fcsetToday.clrColor.SetStandardValue(clrText);
	m_TextLeftRight.fcsetTodaySelected.clrColor.SetStandardValue(clrText);

	m_TextCenter.fcsetNormal.clrColor.SetStandardValue(clrText);
	m_TextCenter.fcsetSelected.clrColor.SetStandardValue(clrText);
	m_TextCenter.fcsetToday.clrColor.SetStandardValue(clrText);
	m_TextCenter.fcsetTodaySelected.clrColor.SetStandardValue(clrText);

	XTP_SAFE_CALL1(GetHeightFormulaPart(), SetStandardValue(14, 10, 1));
}

void CXTPCalendarThemeOffice2007::CTODayViewDayGroup::RefreshMetrics(BOOL bRefreshChildren)
{
	CXTPCalendarThemeOffice2007* pThemeX = DYNAMIC_DOWNCAST(CXTPCalendarThemeOffice2007, GetTheme());
	ASSERT(pThemeX);
	if (!pThemeX)
	{
		return;
	}

	m_clrBorderLeft.SetStandardValue(pThemeX->GetColor(xtpCLR_DayBorder));

	TBase::RefreshMetrics(bRefreshChildren);
}

void CXTPCalendarThemeOffice2007::CTODayViewDayGroupCell::RefreshMetrics(BOOL bRefreshChildren)
{
	UNREFERENCED_PARAMETER(bRefreshChildren);

	//TBase::RefreshMetrics(); // has no children

	m_clrsetWorkCell.clrBackground.SetStandardValue(GetTheme()->GetColor(xtpCLR_DayViewCellWorkBk));
	m_clrsetWorkCell.clrBorderBottomInHour.SetStandardValue(GetTheme()->GetColor(xtpCLR_DayViewCellWorkBorderBottomInHour));
	m_clrsetWorkCell.clrBorderBottomHour.SetStandardValue(GetTheme()->GetColor(xtpCLR_DayViewCellWorkBorderBottomHour));
	m_clrsetWorkCell.clrSelected.SetStandardValue(GetTheme()->GetColor(xtpCLR_SelectedBk));

	m_clrsetNonWorkCell.clrBackground.SetStandardValue(GetTheme()->GetColor(xtpCLR_DayViewCellNonWorkBk));
	m_clrsetNonWorkCell.clrBorderBottomInHour.SetStandardValue(GetTheme()->GetColor(xtpCLR_DayViewCellNonWorkBorderBottomInHour));
	m_clrsetNonWorkCell.clrBorderBottomHour.SetStandardValue(GetTheme()->GetColor(xtpCLR_DayViewCellNonWorkBorderBottomHour));
	m_clrsetNonWorkCell.clrSelected.SetStandardValue(GetTheme()->GetColor(xtpCLR_SelectedBk));

	m_clrDynHatchBrush = COLORREF_NULL;
}

void CXTPCalendarThemeOffice2007::CTODayViewDayGroupAllDayEvents::RefreshMetrics(BOOL bRefreshChildren)
{
	UNREFERENCED_PARAMETER(bRefreshChildren);

	CXTPCalendarThemeOffice2007* pThemeX = DYNAMIC_DOWNCAST(CXTPCalendarThemeOffice2007, GetTheme());
	ASSERT(pThemeX);
	if (!pThemeX)
	{
		return;
	}

	//TBase::RefreshMetrics(); // has no children

	m_clrBackground.SetStandardValue(pThemeX->GetColor(xtpCLR_DayViewAllDayEventsBk));
	m_clrBackgroundSelected.SetStandardValue(pThemeX->GetColor(xtpCLR_SelectedBk));
	m_clrBottomBorder.SetStandardValue(pThemeX->GetColor(xtpCLR_DayViewAllDayEventsBorderBottom));
}

/////////////////////////////////////////////////////////////////////////////
void CXTPCalendarThemeOffice2007::CTOEvent::CEventFontsColorsSet::CopySettings(
				const CXTPCalendarThemeOffice2007::CTOEvent::CEventFontsColorsSet& rSrc)
{
	CXTPCalendarTheme::CopySettings(clrBorder, rSrc.clrBorder);

	CXTPCalendarTheme::CopySettings(grclrBackground, rSrc.grclrBackground);

	fcsetSubject.CopySettings(rSrc.fcsetSubject);
	fcsetLocation.CopySettings(rSrc.fcsetLocation);
	fcsetBody.CopySettings(rSrc.fcsetBody);

	fcsetStartEnd.CopySettings(rSrc.fcsetStartEnd);
}

void CXTPCalendarThemeOffice2007::CTOEvent::RefreshFromParent(CTOEvent* pParentSrc)
{
	ASSERT(pParentSrc);
	if (!pParentSrc)
	{
		return;
	}

	m_fcsetNormal.CopySettings(pParentSrc->m_fcsetNormal);
	m_fcsetSelected.CopySettings(pParentSrc->m_fcsetSelected);

	CXTPCalendarTheme::CopySettings(m_clrGripperBorder, pParentSrc->m_clrGripperBorder);
	CXTPCalendarTheme::CopySettings(m_clrGripperBackground, pParentSrc->m_clrGripperBackground);

	if (pParentSrc->GetHeightFormulaPart())
		XTP_SAFE_CALL1(GetHeightFormulaPart(), RefreshFromParent(pParentSrc->GetHeightFormulaPart()) );

	if (pParentSrc->GetEventIconsToDrawPart())
		XTP_SAFE_CALL1(GetEventIconsToDrawPart(), RefreshFromParent(pParentSrc->GetEventIconsToDrawPart()) );
}

void CXTPCalendarThemeOffice2007::CTOEvent::RefreshMetrics(BOOL bRefreshChildren)
{
	UNREFERENCED_PARAMETER(bRefreshChildren);

	CXTPCalendarThemeOffice2007* pThemeX = DYNAMIC_DOWNCAST(CXTPCalendarThemeOffice2007, GetTheme());
	ASSERT(pThemeX);
	if (!pThemeX)
	{
		return;
	}

	m_fcsetNormal.clrBorder.SetStandardValue(RGB(0, 0, 0));
	m_fcsetNormal.grclrBackground.SetStandardValue(RGB(255, 255, 255), RGB(255, 255, 255));

	m_fcsetSelected.clrBorder.SetStandardValue(RGB(0, 0, 0));
	m_fcsetSelected.grclrBackground.SetStandardValue(m_fcsetNormal.grclrBackground);

	CFont* pFontDef = pThemeX->m_fntBaseFont;
	CFont* pFontBoldDef = pThemeX->m_fntBaseFontBold;

	m_fcsetNormal.fcsetSubject.clrColor.SetStandardValue(RGB(0, 0, 0));
	m_fcsetNormal.fcsetSubject.fntFont.SetStandardValue(pFontBoldDef);

	m_fcsetNormal.fcsetLocation.clrColor.SetStandardValue(RGB(0, 0, 0));
	m_fcsetNormal.fcsetLocation.fntFont.SetStandardValue(pFontDef);

	m_fcsetNormal.fcsetBody.clrColor.SetStandardValue(RGB(0, 0, 0));
	m_fcsetNormal.fcsetBody.fntFont.SetStandardValue(pFontDef);

	m_fcsetSelected.fcsetSubject.clrColor.SetStandardValue(RGB(0, 0, 0));
	m_fcsetSelected.fcsetSubject.fntFont.SetStandardValue(pFontBoldDef);

	m_fcsetSelected.fcsetLocation.clrColor.SetStandardValue(RGB(0, 0, 0));
	m_fcsetSelected.fcsetLocation.fntFont.SetStandardValue(pFontDef);

	m_fcsetSelected.fcsetBody.clrColor.SetStandardValue(RGB(0, 0, 0));
	m_fcsetSelected.fcsetBody.fntFont.SetStandardValue(pFontDef);

	m_clrGripperBorder.SetStandardValue(m_fcsetSelected.clrBorder);
	m_clrGripperBackground.SetStandardValue(RGB(255, 255, 255));
}

void CXTPCalendarThemeOffice2007::CTOEvent::CEventFontsColorsSet::doPX(
		CXTPPropExchange* pPX, LPCTSTR pcszPropName, CXTPCalendarTheme* pTheme)
{
	if (!pPX || !pcszPropName || !pTheme)
	{
		ASSERT(FALSE);
		return;
	}
	CXTPPropExchangeSection secData(pPX->GetSection(pcszPropName));
	if (pPX->IsStoring())
		secData->EmptySection();

	PX_Color(&secData, _T("BorderColor"), clrBorder);
	PX_GrColor(&secData, _T("Background"), grclrBackground);
	fcsetSubject.doPX(&secData, _T("Subject"), pTheme);
	fcsetLocation.doPX(&secData, _T("Location"), pTheme);
	fcsetBody.doPX(&secData, _T("Body"), pTheme);
	fcsetStartEnd.doPX(&secData, _T("StartEnd"), pTheme);
}

void CXTPCalendarThemeOffice2007::CTOEvent::CEventFontsColorsSet::
		Serialize(CArchive& ar)
{
	clrBorder.Serialize(ar);
	grclrBackground.Serialize(ar);
	fcsetSubject.Serialize(ar);
	fcsetLocation.Serialize(ar);
	fcsetBody.Serialize(ar);
	fcsetStartEnd.Serialize(ar);
}

void CXTPCalendarThemeOffice2007::CTOEvent::DoPropExchange(CXTPPropExchange* pPX)
{
	ASSERT(pPX);
	if (!pPX)
		return;

	CXTPPropExchangeSection secData(pPX->GetSection(_T("Event")));
	if (pPX->IsStoring())
		secData->EmptySection();

	m_fcsetNormal.doPX(&secData, _T("Normal"), GetTheme());
	m_fcsetSelected.doPX(&secData, _T("Selected"), GetTheme());

	PX_Color(&secData, _T("GripperBorderColor"),     m_clrGripperBorder);
	PX_Color(&secData, _T("GripperBackgroundColor"), m_clrGripperBackground);

	TBase::DoPropExchange(&secData);
}

void CXTPCalendarThemeOffice2007::CTOEvent::Serialize(CArchive& ar)
{
	m_fcsetNormal.Serialize(ar);
	m_fcsetSelected.Serialize(ar);

	m_clrGripperBorder.Serialize(ar);
	m_clrGripperBackground.Serialize(ar);

	TBase::Serialize(ar);
}

void CXTPCalendarThemeOffice2007::CTODayViewEvent_MultiDay::DoPropExchange(CXTPPropExchange* pPX)
{
	ASSERT(pPX);
	if (!pPX)
		return;

	CXTPPropExchangeSection secData(pPX->GetSection(_T("DayViewEvent_MultiDay")));
	if (pPX->IsStoring())
		secData->EmptySection();

	m_strDateFormatFrom.DoPropExchange(&secData, _T("DateFormatFrom"));
	m_strDateFormatTo.DoPropExchange(&secData, _T("DateFormatTo"));

	TBase::DoPropExchange(&secData);
}

void CXTPCalendarThemeOffice2007::CTODayViewEvent_MultiDay::Serialize(CArchive& ar)
{
	m_strDateFormatFrom.Serialize(ar);
	m_strDateFormatTo.Serialize(ar);

	TBase::Serialize(ar);
}

int CXTPCalendarThemeOffice2007::CTOEvent::CalcMinEventHeight(CDC* pDC)
{
	CArray<CFont*, CFont*> arFonts;

	arFonts.Add(m_fcsetNormal.fcsetSubject.Font());
	arFonts.Add(m_fcsetNormal.fcsetLocation.Font());
	arFonts.Add(m_fcsetNormal.fcsetStartEnd.Font());

	arFonts.Add(m_fcsetSelected.fcsetSubject.Font());
	arFonts.Add(m_fcsetSelected.fcsetLocation.Font());
	arFonts.Add(m_fcsetSelected.fcsetStartEnd.Font());

	int nFontHeightMax = 0;
	CXTPCalendarUtils::GetMaxHeightFont(arFonts, pDC, &nFontHeightMax);

	ASSERT(GetHeightFormulaPart());
	if (!GetHeightFormulaPart())
	{
		ASSERT(FALSE);
		return TBase::CalcMinEventHeight(pDC);
	}

	int nHeight = GetHeightFormulaPart()->Calculate(nFontHeightMax);
	return nHeight;
}

void CXTPCalendarThemeOffice2007::CTODayViewEvent::RefreshMetrics(BOOL bRefreshChildren)
{
	CXTPCalendarThemeOffice2007* pThemeX = DYNAMIC_DOWNCAST(CXTPCalendarThemeOffice2007, GetTheme());
	ASSERT(pThemeX);
	if (!pThemeX)
	{
		return;
	}

	TBase::RefreshMetrics(bRefreshChildren);

	RefreshFromParent(pThemeX->GetEventPartX());

	XTP_SAFE_CALL1(GetHeightFormulaPart(), SetStandardValue(14, 10, 1));
	XTP_SAFE_CALL1(GetEventIconsToDrawPart(), SetStandardValue(FALSE, TRUE, TRUE, FALSE, TRUE));
}

void CXTPCalendarThemeOffice2007::CTODayViewEvent_MultiDay::RefreshMetrics(BOOL bRefreshChildren)
{
	UNREFERENCED_PARAMETER(bRefreshChildren);

	CXTPCalendarThemeOffice2007* pThemeX = DYNAMIC_DOWNCAST(CXTPCalendarThemeOffice2007, GetTheme());
	ASSERT(pThemeX);
	if (!pThemeX)
	{
		return;
	}

	RefreshFromParent(pThemeX->GetDayViewPartX()->GetEventPartX());

	m_fcsetNormal.clrBorder.SetStandardValue(pThemeX->GetColor(xtpCLR_MultiDayEventBorder));
	m_fcsetNormal.grclrBackground.SetStandardValue(pThemeX->GetColor(xtpCLR_MultiDayEventBkGRfrom),
		pThemeX->GetColor(xtpCLR_MultiDayEventBkGRto)   );

	m_fcsetSelected.clrBorder.SetStandardValue(pThemeX->GetColor(xtpCLR_MultiDayEventSelectedBorder));
	m_fcsetSelected.grclrBackground.SetStandardValue(m_fcsetNormal.grclrBackground);

	//LOCALE_IDATE  -  short date format ordering
	// 0 Month-Day-Year
	// 1 Day-Month-Year
	// 2 Year-Month-Day

	int nDateOrdering = CXTPCalendarUtils::GetLocaleLong(LOCALE_IDATE);
	CString strFormatFrom, strFormatTo;
	XTPResourceManager()->LoadString(&strFormatFrom, XTP_IDS_CALENDAR_FROM);
	XTPResourceManager()->LoadString(&strFormatTo, XTP_IDS_CALENDAR_TO);

	if (nDateOrdering == 1)
	{
		strFormatFrom += _T(": MMM d");
		strFormatTo += _T(": MMM d");
	}
	else
	{
		strFormatFrom += _T(": d MMM");
		strFormatTo += _T(": d MMM");
	}

	m_strDateFormatFrom.SetStandardValue(strFormatFrom);
	m_strDateFormatTo.SetStandardValue(strFormatTo);

	m_bShowFromToArrowTextAlways.SetStandardValue(FALSE);

	m_fcsetNormal.fcsetStartEnd.fntFont.SetStandardValue(pThemeX->m_fntBaseFont);
	m_fcsetNormal.fcsetStartEnd.clrColor.SetStandardValue(pThemeX->GetColor(xtpCLR_MultiDayEventFromToDates));

	m_fcsetSelected.fcsetStartEnd.fntFont.SetStandardValue(pThemeX->m_fntBaseFont);
	m_fcsetSelected.fcsetStartEnd.clrColor.SetStandardValue(pThemeX->GetColor(xtpCLR_MultiDayEventFromToDates));
}

void CXTPCalendarThemeOffice2007::CTODayViewEvent_SingleDay::RefreshMetrics(BOOL bRefreshChildren)
{
	UNREFERENCED_PARAMETER(bRefreshChildren);

	CXTPCalendarThemeOffice2007* pThemeX = DYNAMIC_DOWNCAST(CXTPCalendarThemeOffice2007, GetTheme());
	ASSERT(pThemeX);
	if (!pThemeX)
	{
		return;
	}

	RefreshFromParent(pThemeX->GetDayViewPartX()->GetEventPartX());

	m_fcsetNormal.clrBorder.SetStandardValue(pThemeX->GetColor(xtpCLR_DayViewSingleDayEventBorder));
	m_fcsetNormal.grclrBackground.SetStandardValue(pThemeX->GetColor(xtpCLR_DayViewSingleDayEventBkGRfrom),
													pThemeX->GetColor(xtpCLR_DayViewSingleDayEventBkGRto));

	m_fcsetSelected.clrBorder.SetStandardValue(pThemeX->GetColor(xtpCLR_DayViewSingleDayEventSelectedBorder));
	m_fcsetSelected.grclrBackground.SetStandardValue(m_fcsetNormal.grclrBackground);

	InitBusyStatusDefaultColors();
}

BOOL CXTPCalendarThemeOffice2007::CTOEvent::Draw_ArrowL(CXTPCalendarViewEvent* pViewEvent, CDC* pDC,
														CRect& rrcRect, LONG lArrowOffset)
{
	CXTPCalendarThemeOffice2007* pThemeX = DYNAMIC_DOWNCAST(CXTPCalendarThemeOffice2007, GetTheme());
	ASSERT(pThemeX);

	if (!pThemeX)
	{
		return FALSE;
	}

	CXTPResourceImage* pArrowLR = pThemeX->GetBitmap(_T("CalendarAllDayEventArrowsLR"));

	if (!pArrowLR || !pDC || !pViewEvent || !pViewEvent->GetEvent())
	{
		ASSERT(FALSE);
		return FALSE;
	}

	CSize szArrow =  pArrowLR->GetExtent();
	szArrow.cy /= 2;

	CRect rcEventMax(pViewEvent->GetViewEventRectMax());
	CRect rcEvent(pViewEvent->GetEventRect());
	if (rcEvent.IsRectEmpty())
	{
		return FALSE;
	}

	CRect rcArrow = rcEvent;

	rcArrow.top = rcEvent.top + rcEvent.Height() / 2 - szArrow.cy / 2;
	rcArrow.bottom = rcArrow.top + szArrow.cy;
	rcArrow.left = rcEventMax.left + 5;
	rcArrow.right = rcArrow.left + szArrow.cx;

	if (rcEventMax.Width() < szArrow.cx * 4)
	{
		rcArrow.right = rcArrow.left = rcEventMax.left;
		rrcRect = rcArrow;
		return FALSE;
	}

	rcArrow.left += lArrowOffset;
	rcArrow.right += lArrowOffset;

	rrcRect = rcArrow;

	pArrowLR->DrawImage(pDC, rcArrow, pArrowLR->GetSource(0, 2), CRect(0,0,0,0), RGB(255, 0, 255));

	return TRUE;
}

BOOL CXTPCalendarThemeOffice2007::CTOEvent::Draw_ArrowR(CXTPCalendarViewEvent* pViewEvent, CDC* pDC, CRect& rrcRect)
{
	CXTPCalendarThemeOffice2007* pThemeX = DYNAMIC_DOWNCAST(CXTPCalendarThemeOffice2007, GetTheme());
	ASSERT(pThemeX);

	if (!pThemeX)
	{
		return FALSE;
	}

	CXTPResourceImage* pArrowLR = pThemeX->GetBitmap(_T("CalendarAllDayEventArrowsLR"));

	if (!pArrowLR || !pDC || !pViewEvent || !pViewEvent->GetEvent())
	{
		ASSERT(FALSE);
		return FALSE;
	}

	CSize szArrow =  pArrowLR->GetExtent();
	szArrow.cy /= 2;

	CRect rcEventMax(pViewEvent->GetViewEventRectMax());
	CRect rcEvent(pViewEvent->GetEventRect());
	if (rcEvent.IsRectEmpty())
	{
		return FALSE;
	}

	CRect rcArrow = rcEvent;

	rcArrow.top = rcEvent.top + rcEvent.Height() / 2 - szArrow.cy / 2;
	rcArrow.bottom = rcArrow.top + szArrow.cy;
	rcArrow.left = rcEventMax.right - 5 - szArrow.cx;
	rcArrow.right = rcArrow.left + szArrow.cx;

	if (rcEventMax.Width() < szArrow.cx * 4)
	{
		rcArrow.right = rcArrow.left = rcEventMax.right;
		rrcRect = rcArrow;
		return FALSE;
	}

	rrcRect = rcArrow;

	pArrowLR->DrawImage(pDC, rcArrow, pArrowLR->GetSource(1, 2), CRect(0,0,0,0), RGB(255, 0, 255));

	return TRUE;
}

CSize CXTPCalendarThemeOffice2007::CTOEvent::Draw_Icons(CXTPCalendarViewEvent* pViewEvent,
								CDC* pDC, const CRect& rcIconsMax, BOOL bCalculate)
{
	CXTPCalendarThemeOffice2007* pThemeX = DYNAMIC_DOWNCAST(CXTPCalendarThemeOffice2007, GetTheme());

	if (!pThemeX || !pDC || !pViewEvent || !pViewEvent->GetEvent())
	{
		ASSERT(FALSE);
		return FALSE;
	}
	CXTPCalendarEvent* pEvent = pViewEvent->GetEvent();

	CArray<XTP_CALENDAR_EVENT_ICON_INFO, XTP_CALENDAR_EVENT_ICON_INFO&> arIcons;

	CXTPCalendarCustomIconIDs* pCustIconsIDs = pEvent->GetCustomIcons();
	ASSERT(pCustIconsIDs);
	ASSERT(GetEventIconsToDrawPart());
	int nCustomCount = pCustIconsIDs->GetSize();

	// get standard icons

	CUIntArray arGlyphID, arGlyphIndex;

	if (GetEventIconsToDrawPart())
		GetEventIconsToDrawPart()->FillIconIDs(arGlyphID, arGlyphIndex, pEvent);

	CPoint ptNext_rb = rcIconsMax.BottomRight();
	int nIconYMax = 0;
	int nGlyphCount = (int) arGlyphID.GetSize();

	if (nGlyphCount > 0)
	{
		CXTPResourceImage* pGlyphs = pThemeX->GetBitmap(_T("CalendarEventGlyphs"));
		ASSERT(pGlyphs);

		CSize szGlyph = pGlyphs ? pGlyphs->GetExtent() : CSize(0);
		szGlyph.cy /= 5;

		nIconYMax = max(nIconYMax, szGlyph.cy);

		for (int i = 0; i < nGlyphCount && pGlyphs; i++)
		{
			// skip if customized
			if (nCustomCount > 0)
			{
				int nGl = pCustIconsIDs->Find(arGlyphID[i]);
				if (nGl > - 1)
					continue;
			}
			UINT uIn = arGlyphIndex[i];
			if (ptNext_rb.x - szGlyph.cx >= rcIconsMax.left &&
				ptNext_rb.y - szGlyph.cy >= rcIconsMax.top)
			{
				XTP_CALENDAR_EVENT_ICON_INFO ii;
				::ZeroMemory(&ii, sizeof(ii));

				ii.m_pImage = pGlyphs;
				ii.m_rcSrc = pGlyphs->GetSource(uIn, 5);

				ii.m_rcDestination.bottom = ptNext_rb.y;
				ii.m_rcDestination.right = ptNext_rb.x;
				ii.m_rcDestination.left = ptNext_rb.x - szGlyph.cx;
				ii.m_rcDestination.top = ptNext_rb.y - szGlyph.cy;

				ptNext_rb.x -= szGlyph.cx;

				arIcons.Add(ii);
			}
		}
	}

	// get custom icons
	CXTPImageManager* pImageMan = pThemeX->GetCustomIconsList();
	ASSERT(pImageMan);

	if (pImageMan && pCustIconsIDs && nCustomCount > 0)
	{
		for (int i = 0; i < nCustomCount; i++)
		{
			UINT uIconID = pCustIconsIDs->GetAt(nCustomCount - i - 1);

			CXTPImageManagerIcon* pIcon = pImageMan->GetImage(uIconID, 0);
			if (pIcon)
			{
				CSize szIcon = pIcon->GetExtent();

				if (ptNext_rb.x - szIcon.cx < rcIconsMax.left)
					break;

				//COMMENT this condition on Feb, 24, 2009
				//if (ptNext_rb.y - szIcon.cy < rcIconsMax.top)
				//  continue;

				XTP_CALENDAR_EVENT_ICON_INFO ii;
				::ZeroMemory(&ii, sizeof(ii));

				ii.m_pIcon = pIcon;

				ii.m_rcDestination.bottom = ptNext_rb.y;
				ii.m_rcDestination.right = ptNext_rb.x;
				ii.m_rcDestination.left = ptNext_rb.x - szIcon.cx;
				ii.m_rcDestination.top = ptNext_rb.y - szIcon.cy;

				ptNext_rb.x -= szIcon.cx;
				nIconYMax = max(nIconYMax, szIcon.cy);

				arIcons.Add(ii);
			}
		}
	}

	CSize szIcons(rcIconsMax.right - ptNext_rb.x, nIconYMax);

	if (bCalculate)
		return szIcons;

	CRect rcIcons = rcIconsMax;

	rcIcons.left = max(rcIcons.right - szIcons.cx - 2, rcIconsMax.left);
	rcIcons.top = max(rcIcons.bottom - szIcons.cy - 2, rcIconsMax.top);

	CTOEvent* pEventPart = pThemeX->GetThemePartForEvent(pViewEvent);
	ASSERT(pEventPart);
	if (pEventPart && !rcIcons.IsRectEmpty())
		pEventPart->FillEventBackgroundEx(pDC, pViewEvent, rcIcons);

	// Draw icons
	int nCount = (int)arIcons.GetSize();
	for (int i = 0; i < nCount; i++)
	{
		XTP_CALENDAR_EVENT_ICON_INFO ii = arIcons[i];
		CRect rcIcon = ii.m_rcDestination;

		if (pDC->IsPrinting())
		{
			CRect rcIcon00(0, 0, rcIcon.Width(), rcIcon.Height());

			CBitmap bmp;
			bmp.CreateCompatibleBitmap(pDC, rcIcon.Width(), rcIcon.Height());

			CXTPCompatibleDC dcMem(NULL, &bmp);

			dcMem.m_bPrinting = TRUE;
			pEventPart->FillEventBackgroundEx(&dcMem, pViewEvent, rcIcon00);
			dcMem.m_bPrinting = FALSE;

			if (ii.m_pImage)
				ii.m_pImage->DrawImage(&dcMem, rcIcon00, ii.m_rcSrc, CRect(0,0,0,0), RGB(255, 0, 255));
			else if (ii.m_pIcon)
				ii.m_pIcon->Draw(&dcMem, CPoint(0, 0));
			else
				ASSERT(FALSE);

			pDC->BitBlt(rcIcon.left, rcIcon.top, rcIcon.Width(), rcIcon.Height(), &dcMem, 0, 0, SRCCOPY);
		}
		else
		{
			if (ii.m_pImage)
				ii.m_pImage->DrawImage(pDC, ii.m_rcDestination, ii.m_rcSrc, CRect(0,0,0,0), RGB(255, 0, 255));
			else if (ii.m_pIcon)
				ii.m_pIcon->Draw(pDC, rcIcon.TopLeft());
			else
				ASSERT(FALSE);
		}
	}

	return szIcons;
}

BOOL CXTPCalendarThemeOffice2007::CTODayViewEvent_MultiDay::Draw_ArrowLtext(CXTPCalendarViewEvent* pViewEvent,
				CDC* pDC, CThemeFontColorSetValue* pfcsetText, CRect& rrcRect, int nLeft_x)
{
	if (!pDC || !pViewEvent || !pViewEvent->GetEvent() || !pfcsetText)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	CXTPFontDC autoFont(pDC, pfcsetText->Font(), pfcsetText->Color());

	CString strText = Format_FromToDate(pViewEvent, 1);

	CRect rcEvent(pViewEvent->GetEventRect());
	if (rcEvent.IsRectEmpty())
	{
		return FALSE;
	}

	CSize szText = pDC->GetTextExtent(strText);

	CRect rcText = rcEvent;
	rcText.left = nLeft_x + 5;
	rcText.right = rcText.left + szText.cx;

	if (rcEvent.Width() / 2 < szText.cx * 3 && !(BOOL)m_bShowFromToArrowTextAlways ||
		rcEvent.Width() < szText.cx * 3)
	{
		rcText.left = rcText.right = nLeft_x;
		rrcRect = rcText;
		return FALSE;
	}

	rrcRect = rcText;

	pDC->DrawText(strText, &rcText, DT_LEFT | DT_SINGLELINE | DT_VCENTER);

	return TRUE;
}

BOOL CXTPCalendarThemeOffice2007::CTODayViewEvent_MultiDay::Draw_ArrowRtext(CXTPCalendarViewEvent* pViewEvent,
				CDC* pDC, CThemeFontColorSetValue* pfcsetText, CRect& rrcRect, int nRight_x)
{
	if (!pDC || !pViewEvent || !pViewEvent->GetEvent() || !pfcsetText)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	CXTPFontDC autoFont(pDC, pfcsetText->Font(), pfcsetText->Color());

	CString strText = Format_FromToDate(pViewEvent, 2);

	CRect rcEvent(pViewEvent->GetEventRect());
	if (rcEvent.IsRectEmpty())
	{
		return FALSE;
	}

	CSize szText = pDC->GetTextExtent(strText);

	CRect rcText = rcEvent;
	rcText.right = nRight_x - 5;
	rcText.left = rcText.right - szText.cx;

	if (rcEvent.Width() / 2 < szText.cx * 3 && !(BOOL)m_bShowFromToArrowTextAlways ||
		rcEvent.Width() < szText.cx * 3)
	{
		rcText.left = rcText.right = nRight_x;
		rrcRect = rcText;
		return FALSE;
	}

	rrcRect = rcText;

	pDC->DrawText(strText, &rcText, DT_RIGHT | DT_SINGLELINE | DT_VCENTER);

	return TRUE;
}

CString CXTPCalendarThemeOffice2007::CTODayViewEvent_MultiDay::Format_FromToDate(CXTPCalendarViewEvent* pViewEvent, int nStart1End2)
{
	if (!pViewEvent || !pViewEvent->GetEvent())
	{
		ASSERT(FALSE);
		return _T("");
	}

	ASSERT(nStart1End2 == 1 || nStart1End2 == 2);

	COleDateTime dtDate = nStart1End2 == 1 ? pViewEvent->GetEvent()->GetStartTime() : pViewEvent->GetEvent()->GetEndTime();

	if (nStart1End2 == 2 && CXTPCalendarUtils::IsZeroTime(dtDate))
	{
		dtDate -= COleDateTimeSpan(0, 0, 1, 0);
	}

	const CString& strFormat = nStart1End2 == 1 ? m_strDateFormatFrom : m_strDateFormatTo;

	CString strDate = CXTPCalendarUtils::GetDateFormat(dtDate, strFormat);

	return strDate;
}

///*** ======================================================================
CRect CXTPCalendarThemeOffice2007::CTODayViewDay::ExcludeDayBorder(
				CXTPCalendarDayViewDay* pDayViewDay, const CRect& rcDay)
{
	BOOL bIsOffice2010Theme = (pDayViewDay && pDayViewDay->GetCalendarControl())
		? (pDayViewDay->GetCalendarControl()->GetThemeVersion() == xtpCalendarVersion2010) : FALSE;

	BOOL bIsToday = GetHeaderPart()->IsToday(pDayViewDay);
	int nWidthH = bIsToday ? 2 : 1;

	if (bIsOffice2010Theme)
		nWidthH = 1;

	CRect rcGroups = rcDay;
	rcGroups.DeflateRect(nWidthH, 0, nWidthH, 1);
	return rcGroups;
}

void CXTPCalendarThemeOffice2007::CTODayViewDay::Draw(CCmdTarget* pObject, CDC* pDC)
{
	TBase::Draw(pObject, pDC);

	CXTPCalendarDayViewDay* pDayViewDay = DYNAMIC_DOWNCAST(CXTPCalendarDayViewDay, pObject);
	if (!pDC || !pDayViewDay)
	{
		ASSERT(FALSE);
		return;
	}

	CRect rcBottom = pDayViewDay->GetDayRect();
	rcBottom.top = rcBottom.bottom - 1;

	BOOL bIsToday = GetHeaderPart()->IsToday(pDayViewDay);
	COLORREF clrBorder = bIsToday ? m_clrTodayBorder : m_clrBorder;

	pDC->FillSolidRect(&rcBottom, clrBorder);
}

void CXTPCalendarThemeOffice2007::CTODayViewDay::Draw_DayBorder(CXTPCalendarDayViewDay* pDayViewDay, CDC* pDC)
{
	CRect rcDay = pDayViewDay->GetDayRect();
	CRect rcGroup(rcDay);
	if (pDayViewDay->GetViewGroupsCount() > 0)
	{
		rcGroup = pDayViewDay->GetViewGroup(0)->GetRect();
	}

	CRect rcDayDetails = rcDay;
	rcDayDetails.top = rcGroup.top;

	CRect rcDayDetails2 = ExcludeDayBorder(pDayViewDay, rcDayDetails);

	BOOL bIsToday = GetHeaderPart()->IsToday(pDayViewDay);
	COLORREF clrBorder = bIsToday ? m_clrTodayBorder : m_clrBorder;

	CRect rcLeft = rcDayDetails, rcRight = rcDayDetails, rcBottom = rcDayDetails;
	rcLeft.right = min(rcDayDetails2.left, rcDayDetails.right);
	rcRight.left = max(rcDayDetails2.right, rcDayDetails.left);
	rcBottom.top = max(rcDayDetails2.bottom, rcDayDetails.top);

	pDC->FillSolidRect(&rcLeft, clrBorder);
	pDC->FillSolidRect(&rcRight, clrBorder);
	pDC->FillSolidRect(&rcBottom, clrBorder);
}

void CXTPCalendarThemeOffice2007::CTODayViewDayHeader::AdjustLayout(CCmdTarget* pObject,
													CDC* pDC, const CRect& rcRect)
{
	//TBase::AdjustLayout(pObject, pDC, rcRect); //<< extra call >>

	CXTPCalendarDayViewDay* pDayViewDay = DYNAMIC_DOWNCAST(CXTPCalendarDayViewDay, pObject);
	if (!pDC || !pDayViewDay || !pDayViewDay->GetView())
	{
		ASSERT(FALSE);
		return;
	}
	if (pDayViewDay->GetDayIndex() != 0)
	{
		return;
	}

	m_nWeekDayFormat = 0;

	CXTPCalendarDayView* pDayView = pDayViewDay->GetView();
	if (m_UseOffice2003HeaderFormat)
	{
		int nDayWidth = rcRect.Width() - 4;
		pDayView->CalculateHeaderFormat(pDC, nDayWidth, m_TextCenter.fcsetNormal.Font());
	}
	else
	{
		int nWDLong_max = 0;
		int nWDShort_max = 0;

		CXTPFontDC autoFnt(pDC, m_TextCenter.fcsetNormal.Font());

		for (int i = 0; i < 7; i++)
		{
			CString strWDLong = CXTPCalendarUtils::GetLocaleString(LOCALE_SDAYNAME1 + i, 100);
			CString strWDShort = CXTPCalendarUtils::GetLocaleString(LOCALE_SABBREVDAYNAME1 + i, 100);

			nWDLong_max = max(nWDLong_max, pDC->GetTextExtent(strWDLong).cx);
			nWDShort_max = max(nWDShort_max, pDC->GetTextExtent(strWDShort).cx);
		}

		autoFnt.SetFont(m_TextLeftRight.fcsetNormal.Font());

		int nLR_width = pDC->GetTextExtent(_T("88"), 2).cx * 2;

		int nCenterTextWidth = max(rcRect.Width() - nLR_width, 0);

		if (nCenterTextWidth >= nWDLong_max + 4)
		{
			m_nWeekDayFormat = 2;
		}
		else if (nCenterTextWidth >= nWDShort_max + 4)
		{
			m_nWeekDayFormat = 1;
		}
		else
		{
			m_nWeekDayFormat = 0;
		}
	}
}

void CXTPCalendarThemeOffice2007::CTODayViewDayHeader::Draw(CCmdTarget* pObject, CDC* pDC)
{
	CXTPCalendarThemeOffice2007* pThemeX = DYNAMIC_DOWNCAST(CXTPCalendarThemeOffice2007, GetTheme());
	CXTPCalendarDayViewDay* pDayViewDay = DYNAMIC_DOWNCAST(CXTPCalendarDayViewDay, pObject);
	if (!pDC || !pDayViewDay || !pDayViewDay->GetView() || !GetTheme() || !pThemeX)
	{
		ASSERT(FALSE);
		return;
	}

	XTP_VIEW_DAY_LAYOUT& rLayout = GetTheme()->GetViewDayLayout(pDayViewDay);
	COleDateTime dtDay = pDayViewDay->GetDayDate();

	CPoint pt(0, 0);
	if (pThemeX->GetCalendarControl()->GetSafeHwnd() && GetCursorPos(&pt))
		pThemeX->GetCalendarControl()->ScreenToClient(&pt);

	BOOL bResMax(TRUE);
	SCROLLINFO si;
	int nScrollStep = 1;
	BOOL bScroll = pDayViewDay->GetView()->GetScrollBarInfoH(&si, &nScrollStep);
	int nPage = si.nPage; //Current visible width
	int nMinColumnWidth = pDayViewDay->GetView()->RecalcMinColumnWidth();
	int iResCnt = 0;
	int iMlt = pDayViewDay->GetView()->m_nDayHeaderPeriod;
	if (pDayViewDay->GetView()->GetResources())
		iResCnt = pDayViewDay->GetView()->GetResources()->GetCount();
	if (bScroll && nMinColumnWidth > 0 && iResCnt > 1)
	{
		if (nPage <= iResCnt * nMinColumnWidth
			&& pThemeX->GetCalendarControl()->m_bFullDateMode)
		{
			bResMax = FALSE;

			if (iMlt == 1)
				iMlt = 2;
		}
	}

	BOOL bIsToday    = IsToday(pDayViewDay);
	BOOL bIsSelected = IsSelected(pDayViewDay);

	BOOL bFirstDay = pDayViewDay->GetDayIndex() == 0;
	BOOL bLastDay = pDayViewDay->GetDayIndex() == pDayViewDay->GetView()->GetViewDayCount() - 1;

	int nFlags = bIsToday ? CTOHeader::xtpCalendarItemState_Today : CTOHeader::xtpCalendarItemState_Normal;
	nFlags |= bIsSelected ? CTOHeader::xtpCalendarItemState_Selected : 0;
	nFlags |= bFirstDay ? CTOHeader::xtpCalendarItemFirst : 0;
	nFlags |= bLastDay ? CTOHeader::xtpCalendarItemLast : 0;

	if (rLayout.m_rcDayHeader.PtInRect(pt))
	{
		nFlags |=  CTOHeader::xtpCalendarItemState_Hot;
		rLayout.m_nHotState |= XTP_VIEW_DAY_LAYOUT::xtpHotDayHeader;
	}
	else
	{
		rLayout.m_nHotState &= ~XTP_VIEW_DAY_LAYOUT::xtpHotDayHeader;
	}

	CRect rcRect = rLayout.m_rcDayHeader;

	CString strTextLeft;
	CString strTextCenter;
	CString strTextRight;

	if (m_UseOffice2003HeaderFormat)
	{
		strTextCenter = pDayViewDay->GetCaption();
	}
	else if (!pThemeX->GetCalendarControl()->m_bFullDateMode)
	{
		strTextLeft.Format(_T("%d"), dtDay.GetDay());

		if (m_nWeekDayFormat > 0)
		{
			LCTYPE lctWDName1 = m_nWeekDayFormat > 1 ? LOCALE_SDAYNAME1 : LOCALE_SABBREVDAYNAME1;

			int nWD = dtDay.GetDayOfWeek();
			strTextCenter = CXTPCalendarUtils::GetLocaleString(lctWDName1 + ((nWD - 2 + 7) %7), 100);
		}

		pThemeX->GetItemTextIfNeed(xtpCalendarItemText_DayViewDayHeaderLeft, &strTextLeft, pDayViewDay);
		pThemeX->GetItemTextIfNeed(xtpCalendarItemText_DayViewDayHeaderCenter, &strTextCenter, pDayViewDay);
		pThemeX->GetItemTextIfNeed(xtpCalendarItemText_DayViewDayHeaderRight, &strTextRight, pDayViewDay);
	}
	//Colors for schedules - 0 prevents group modifications (e.g. for Week Day caption)
	if (pThemeX != NULL &&  pThemeX->GetCalendarControl() != NULL &&
		pThemeX->GetCalendarControl()->m_bMultiColorScheduleMode)
	{
		pDC->SetTextColor(RGB(0, 0, 0));
	}

	if (pThemeX->GetCalendarControl()->m_bFullDateMode)
	{
		CString sFmt = pDayViewDay->GetView()->GetDayHeaderFormat();
		if (!sFmt.IsEmpty())
		{
			if (sFmt.Find(_T("MMM")) > -1)
			{
				sFmt = _T("%a, %b %d");
				pDayViewDay->GetView()->SetDayHeaderFormat(sFmt);
			}

			strTextLeft.Empty();
			strTextRight.Empty();

//Week Number DayView header display match to DatePicker display <<
			int iSunMon = pThemeX->GetCalendarControl()->GetFirstDayOfWeek();
			if (sFmt.Find(_T("%W")) > -1 && iSunMon == 1)
				sFmt.Replace(_T("%W"), _T("%U"));
			else if (sFmt.Find(_T("%U")) > -1  && iSunMon > 1)
				sFmt.Replace(_T("%U"), _T("%W"));

			if (sFmt.Find(_T("%W")) > -1) //Mon case
			{
				int iWeekNum = _ttoi(dtDay.Format(_T("%W")));
				if (iWeekNum == 52)
					iWeekNum = 0;
				CString s; s.Format(_T("%d"), iWeekNum + 1);
				sFmt.Replace(_T("%W"), s);
			}
			else if (sFmt.Find(_T("%U")) > -1) //Sun case
			{
				int iWeekNum = _ttoi(dtDay.Format(_T("%U")));
				if (iWeekNum == 52)
					iWeekNum = 0;
				CString s; s.Format(_T("%d"), iWeekNum + 1);
				sFmt.Replace(_T("%U"), s);
			}
//Week Number DayView header display match to DatePicker display >>
			strTextCenter = dtDay.Format(sFmt);
		}
	}

	if (bResMax)
	{
		Draw_Header(pDC, rcRect, nFlags, strTextLeft, strTextCenter, strTextRight);
	}
	else
	{
		CRect rc(rcRect);

		Draw_Header(pDC, rcRect, nFlags, _T(""), _T(""), _T("")); //Background

		nMinColumnWidth *= iMlt;
		rc.right = rc.left + nMinColumnWidth;

		Draw_Header(pDC, rc, nFlags, strTextLeft, strTextCenter, strTextRight, TRUE);

		if (nMinColumnWidth > 0)
		{
			while (rc.right <= rcRect.right - nMinColumnWidth)
			{
				rc.left += nMinColumnWidth;
				rc.right += nMinColumnWidth;

				Draw_Header(pDC, rc, nFlags, strTextLeft, strTextCenter, strTextRight, TRUE);
			}
		}
	}
}

void CXTPCalendarThemeOffice2007::CTODayViewDayHeader::DoPropExchange(CXTPPropExchange* pPX)
{
	ASSERT(pPX);
	if (!pPX)
		return;


	if (pPX->GetSchema() >= _XTP_SCHEMA_112)
	{
		CXTPPropExchangeSection secData(pPX->GetSection(_T("DayViewDayHeader")));
		if (pPX->IsStoring())
		secData->EmptySection();

		m_UseOffice2003HeaderFormat.DoPropExchange(&secData, _T("UseOffice2003HeaderFormat"));

		XTP_SAFE_CALL_BASE(TBase, DoPropExchange(&secData));
	}
	else
	{
		XTP_SAFE_CALL_BASE(TBase, DoPropExchange(pPX));
	}
}

void CXTPCalendarThemeOffice2007::CTODayViewDayHeader::Serialize(CArchive& ar)
{
	m_UseOffice2003HeaderFormat.Serialize(ar);

	XTP_SAFE_CALL_BASE(TBase, Serialize(ar));
}

void CXTPCalendarThemeOffice2007::CTODayViewDayGroupHeader::Draw(CCmdTarget* pObject, CDC* pDC)
{
	CXTPCalendarThemeOffice2007* pThemeX = DYNAMIC_DOWNCAST(CXTPCalendarThemeOffice2007, GetTheme());
	CXTPCalendarDayViewGroup* pDayViewGroup = DYNAMIC_DOWNCAST(CXTPCalendarDayViewGroup, pObject);

	if (!pThemeX || !pDayViewGroup || !pDayViewGroup->GetViewDay())
	{
		ASSERT(FALSE);
		return;
	}

	const XTP_VIEW_GROUP_LAYOUT layoutGoup = CXTPCalendarTheme::GetViewGroupLayout(pDayViewGroup);
	XTP_DAY_VIEW_GROUP_LAYOUT& groupLayoutX = CXTPCalendarTheme::GetDayViewGroupLayout(pDayViewGroup);
	CRect rcRect = layoutGoup.m_rcGroupHeader;

	CPoint pt(0, 0);
	if (pThemeX->GetCalendarControl()->GetSafeHwnd() && GetCursorPos(&pt))
		pThemeX->GetCalendarControl()->ScreenToClient(&pt);

	BOOL bIsToday    = IsToday(pDayViewGroup->GetViewDay());
	BOOL bIsSelected = IsSelected(pDayViewGroup);
	BOOL bHot = rcRect.PtInRect(pt);

	BOOL bFirstDay = pDayViewGroup->GetGroupIndex() == 0;
	BOOL bLastDay = pDayViewGroup->GetGroupIndex() == pDayViewGroup->GetViewDay()->GetViewGroupsCount() - 1;

	int nFlags = bIsToday ? CTOHeader::xtpCalendarItemState_Today : CTOHeader::xtpCalendarItemState_Normal;
	nFlags |= bIsSelected ? CTOHeader::xtpCalendarItemState_Selected : 0;
	nFlags |= bFirstDay ? CTOHeader::xtpCalendarItemFirst : 0;
	nFlags |= bLastDay ? CTOHeader::xtpCalendarItemLast : 0;

	if (bHot)
	{
		nFlags |=  CTOHeader::xtpCalendarItemState_Hot;
		groupLayoutX.m_nHotState |= XTP_DAY_VIEW_GROUP_LAYOUT::xtpHotHeader;
	}
	else
	{
		groupLayoutX.m_nHotState &= ~XTP_DAY_VIEW_GROUP_LAYOUT::xtpHotHeader;
	}

	CString strCaption = pDayViewGroup->GetCaption();

	// Colors for schedules
	if (pThemeX != NULL && pThemeX->GetCalendarControl() != NULL &&
		pThemeX->GetCalendarControl()->m_bMultiColorScheduleMode)
	{
		COLORREF clrNew = pDayViewGroup->m_GroupColor;
		if (clrNew != DWORD(-1))
			pDC->SetTextColor(clrNew);
	}

	Draw_Header(pDC, rcRect, nFlags, _T(" "), strCaption, _T(" "));
}

void CXTPCalendarThemeOffice2007::CTODayViewDay::OnMouseMove(CCmdTarget* pObject, UINT nFlags, CPoint point)
{
	UNREFERENCED_PARAMETER(nFlags);

	CXTPCalendarThemeOffice2007* pThemeX = DYNAMIC_DOWNCAST(CXTPCalendarThemeOffice2007, GetTheme());
	CXTPCalendarViewDay* pViewDay = DYNAMIC_DOWNCAST(CXTPCalendarViewDay, pObject);
	if (!pThemeX || !pViewDay || !pThemeX->GetCalendarControl())
	{
		ASSERT(FALSE);
		return;
	}

	XTP_VIEW_DAY_LAYOUT& dayLayout = CXTPCalendarTheme::GetViewDayLayout(pViewDay);

	BOOL bHotPrev = !!(dayLayout.m_nHotState & XTP_VIEW_DAY_LAYOUT::xtpHotDayHeader);
	if (dayLayout.m_rcDayHeader.PtInRect(point) != bHotPrev)
	{
		CXTPCalendarControl::CUpdateContext updateContext(pThemeX->GetCalendarControl(), xtpCalendarUpdateRedraw);
	}
}

void CXTPCalendarThemeOffice2007::CTODayViewDayGroup::AdjustDayEvents(
								CXTPCalendarDayViewGroup* pDayViewGroup, CDC* pDC)
{
	if (!pDayViewGroup || !pDC)
	{
		ASSERT(FALSE);
		return;
	}

	ASSERT(pDC);

	CXTPCalendarTheme_DayViewSingleDayEventPlaceBuilder placeBuilder;
	placeBuilder.BuildEventPlaces(pDayViewGroup);

	XTP_DAY_VIEW_GROUP_LAYOUT& rLayoutX = GetTheme()->GetDayViewGroupLayout(pDayViewGroup);

	int nCount = pDayViewGroup->GetViewEventsCount();
	for (int i = 0; i < nCount; i++)
	{
		CXTPCalendarDayViewEvent* pViewEvent = pDayViewGroup->GetViewEvent(i);
		ASSERT(pViewEvent);
		if (!pViewEvent || pViewEvent->IsMultidayEvent())
			continue;

		int nPlaceNumber = pViewEvent->GetEventPlaceNumber();

		pViewEvent->AdjustLayout2(pDC, rLayoutX.m_rcDayDetails, nPlaceNumber);
	}
}

CRect CXTPCalendarThemeOffice2007::CTODayViewDayGroup::GetScrollButtonRect(
		const XTP_DAY_VIEW_GROUP_LAYOUT& groupLayout, int nButton)
{
	CRect rc;

	switch (nButton)
	{
	case XTP_DAY_VIEW_GROUP_LAYOUT::xtpHotScrollUp:
		rc = groupLayout.m_rcDayDetails;
		rc.right -= 5;
		rc.top += 5;
		break;
	case XTP_DAY_VIEW_GROUP_LAYOUT::xtpHotScrollDown:
		rc = groupLayout.m_rcDayDetails;
		rc.right -= 5;
		rc.bottom -= 5;
		break;
	default:
		ASSERT(FALSE);
		rc.SetRect(0, 0, 0, 0);
	}

	return rc;
}

int CXTPCalendarThemeOffice2007::CTOCellDynBackground::GetCellDynBackground(CXTPCalendarViewGroup* pViewGroup,
				COleDateTime dtCellTime, int nCellIndex, CXTPCalendarTheme::XTP_BUSY_STATUS_CONTEXT& rContext)
{
	::ZeroMemory(&rContext, sizeof(rContext));

	m_fDrawMonthViewBackgroundHatch = FALSE;
	m_pBkViewEvent = NULL;
	m_pBkTentativeEvent = NULL;

	ASSERT(pViewGroup);
	if (!pViewGroup)
		return xtpCalendarBusyStatusUnknown;

	m_pBkViewEvent = GetCellBackgroundEvent(pViewGroup, dtCellTime, nCellIndex);
	CheckIsBackgroundHatchNeeded();

	if (!m_pBkViewEvent || !m_pBkViewEvent->GetEvent() || !m_pBkViewEvent->GetEvent()->GetCategories())
		return xtpCalendarBusyStatusUnknown;

	CXTPCalendarEvent* pEvent = m_pBkViewEvent->GetEvent();
	CXTPCalendarEventCategory* pCat = NULL;
	if (pEvent->GetCategories()->GetSize())
	{
		UINT nCatID = pEvent->GetCategories()->GetAt(0);
		pCat = XTP_SAFE_GET3(pEvent, GetDataProvider(), GetEventCategories(), Find(nCatID), NULL);
	}

	int nBusyStatus = pEvent->GetBusyStatus();

	if (nBusyStatus == xtpCalendarBusyStatusBusy ||
		nBusyStatus == xtpCalendarBusyStatusTentative)
	{
		if (!pCat)
			return nBusyStatus;

		rContext.clrBkAllDayEvents  = pCat->GetBkColorAllDayEvents();
		rContext.clrBkNonWorkCell   = pCat->GetBkColorNonWorkCell();
		rContext.clrBkWorkCell      = pCat->GetBkColorWorkCell();
		rContext.clrCellBorder      = pCat->GetColorCellBorder();
	}
	else if (nBusyStatus == xtpCalendarBusyStatusOutOfOffice)
	{
		rContext.clrBkAllDayEvents  = RGB(183, 141, 206);
		rContext.clrBkNonWorkCell   = RGB(214, 190, 227);
		rContext.clrBkWorkCell      = RGB(229, 214, 237);
		rContext.clrCellBorder      = rContext.clrBkAllDayEvents;
	}

	return nBusyStatus;
}

CXTPCalendarViewEvent* CXTPCalendarThemeOffice2007::CTOCellDynBackground::GetCellBackgroundEvent(CXTPCalendarViewGroup* pViewGroup,
																			COleDateTime dtCellTime, int nCellIndex)
{
	ASSERT(pViewGroup);
	if (!pViewGroup)
		return NULL;

	// TODO: Not optimal implementation. Optimize later if need.
	//  #pragma XTPNOTE("Not optimal implementation. Optimize later if need.")

	CXTPCalendarViewEvent* pBkVEvent = NULL;
	BOOL bFillAllDayEventsBkgnd = XTP_SAFE_GET3(pViewGroup, GetCalendarControl(), GetCalendarOptions(), bHatchAllDayViewEventsBkgnd, TRUE);

	int nCount = pViewGroup->GetViewEventsCount();
	for (int i = 0; i < nCount; i++)
	{
		CXTPCalendarEvent* pEvent = XTP_SAFE_GET2(pViewGroup, GetViewEvent_(i), GetEvent(), NULL);
		if (XTP_SAFE_GET2(pViewGroup, GetViewEvent_(i), IsMultidayEvent(), FALSE) && pEvent)
		{
			if(pEvent->IsAllDayEvent() && bFillAllDayEventsBkgnd == FALSE)
				continue;

			COleDateTime dtEventStartTime = pEvent->IsAllDayEvent() ? CXTPCalendarUtils::ResetTime(pEvent->GetStartTime()) : pEvent->GetStartTime();
			COleDateTime dtEventEndTime = pEvent->IsAllDayEvent() ? CXTPCalendarUtils::SetTime_235959(pEvent->GetEndTime()) : pEvent->GetEndTime();

			if (nCellIndex < 0 ||
				(dtCellTime >= dtEventStartTime && dtCellTime <= dtEventEndTime))
			{
				if (!pBkVEvent || CmpEventBusyStatus(pBkVEvent->GetEvent(), pEvent) < 0)
				{
					pBkVEvent = pViewGroup->GetViewEvent_(i);
				}

				if(pEvent->GetBusyStatus() == xtpCalendarBusyStatusTentative)
					m_pBkTentativeEvent = pEvent;
			}
		}
	}

	return pBkVEvent;
}

void CXTPCalendarThemeOffice2007::CTOCellDynBackground::CheckIsBackgroundHatchNeeded()
{
	CXTPCalendarThemeOffice2007* pThemeX = DYNAMIC_DOWNCAST(CXTPCalendarThemeOffice2007, GetTheme());
	if(!pThemeX->m_bFlatStyle)
		return;

	if(m_pBkTentativeEvent == NULL)
		return;

	CXTPCalendarEvent* pBkEvent = m_pBkViewEvent->GetEvent();
	if(pBkEvent->GetBusyStatus() != xtpCalendarBusyStatusOutOfOffice)
		return;

	if(CXTPCalendarUtils::ResetTime(pBkEvent->GetStartTime()) <= CXTPCalendarUtils::ResetTime(m_pBkTentativeEvent->GetStartTime()))
		return;

	m_fDrawMonthViewBackgroundHatch = TRUE;
}

BOOL CXTPCalendarThemeOffice2007::CTOCellDynBackground::DrawMonthViewHatchBackground()
{
	return m_fDrawMonthViewBackgroundHatch;
}

CXTPCalendarEvent* CXTPCalendarThemeOffice2007::CTOCellDynBackground::GetBkTentativeEvent()
{
	return m_pBkTentativeEvent;
}

int CXTPCalendarThemeOffice2007::CTOCellDynBackground::_GetEventBusyStatusRank(CXTPCalendarEvent* pEvent)
{
	ASSERT(pEvent);
	if (!pEvent)
		return -1;

	const int cnRankSize = 4;
	ASSERT(xtpCalendarBusyStatusFree < cnRankSize && xtpCalendarBusyStatusTentative < cnRankSize &&
		xtpCalendarBusyStatusBusy < cnRankSize && xtpCalendarBusyStatusOutOfOffice < cnRankSize);

	static int sarBSrank[cnRankSize] = {0, 0, 0, 0};
	if (sarBSrank[0] == 0)
	{
		sarBSrank[xtpCalendarBusyStatusFree]        = 1; // min rank
		sarBSrank[xtpCalendarBusyStatusTentative]   = 2;
		sarBSrank[xtpCalendarBusyStatusOutOfOffice] = 3;
		sarBSrank[xtpCalendarBusyStatusBusy]        = 4; // max rank
	}

	int nBS = pEvent->GetBusyStatus();
	int nBSindex = max(0, min(nBS, cnRankSize-1));

	ASSERT(nBSindex >= 0 && nBSindex < cnRankSize);
	int nBSrank = sarBSrank[nBSindex];

	return nBSrank;
}

int CXTPCalendarThemeOffice2007::CTOCellDynBackground::CmpEventBusyStatus(CXTPCalendarEvent* pEvent1, CXTPCalendarEvent* pEvent2)
{
	ASSERT(pEvent1 && pEvent2);

	if (!pEvent1 || !pEvent2)
		return XTPCompare(pEvent1, pEvent2);

	int nBS1rank = _GetEventBusyStatusRank(pEvent1);
	int nBS2rank = _GetEventBusyStatusRank(pEvent2);

	int nCmp = XTPCompare(nBS1rank, nBS2rank);

	if (nCmp == 0)
		nCmp = XTPCompare(pEvent1->GetDurationMinutes(), pEvent2->GetDurationMinutes());

	if (nCmp == 0 && !CXTPCalendarUtils::IsEqual(pEvent1->GetStartTime(), pEvent2->GetStartTime()))
		nCmp = XTPCompare(pEvent1->GetStartTime(), pEvent2->GetStartTime());

	if (nCmp == 0)
		nCmp = XTPCompare(pEvent1->GetEventID(), pEvent2->GetEventID());

	return nCmp;
}


void CXTPCalendarThemeOffice2007::CTODayViewDayGroupCell::Draw(CDC* pDC, const CRect& rcRect,
		const XTP_CALENDAR_THEME_DAYVIEWCELL_PARAMS& cellParams, CXTPPropsStateContext* pStateCnt)
{
	CXTPCalendarThemeOffice2007* pThemeX = DYNAMIC_DOWNCAST(CXTPCalendarThemeOffice2007, GetTheme());
	CXTPCalendarThemeOffice2007::CTOCellDynBackground* pCellDynBkPart =
		XTP_SAFE_GET1(pThemeX, GetCellDynBackgroundPart(), NULL);

	if (!pDC || !pThemeX || !cellParams.pViewGroup || !pCellDynBkPart || !pThemeX->GetColorsSetPart())
	{
		ASSERT(FALSE);
		return;
	}

	CXTPPropsStateContext autoStateCnt0(this);

	CXTPCalendarTheme::XTP_BUSY_STATUS_CONTEXT bsColors;
	int nCellBusyStatus = pCellDynBkPart->GetCellDynBackground(cellParams.pViewGroup,
							cellParams.dtBeginTime, cellParams.nIndex, bsColors);

	m_clrDynHatchBrush = COLORREF_NULL;

	if (nCellBusyStatus == xtpCalendarBusyStatusBusy ||
		nCellBusyStatus == xtpCalendarBusyStatusOutOfOffice ||
		nCellBusyStatus == xtpCalendarBusyStatusTentative)
	{
		if (bsColors.clrBkAllDayEvents != 0 && bsColors.clrCellBorder != 0) // is Category or Tentative
		{
			if (nCellBusyStatus == xtpCalendarBusyStatusTentative)
				m_clrDynHatchBrush = bsColors.clrBkAllDayEvents;

			if (cellParams.bWorkCell)
			{
				if (nCellBusyStatus != xtpCalendarBusyStatusTentative)
					m_clrsetWorkCell.clrBackground = bsColors.clrBkWorkCell;

				m_clrsetWorkCell.clrBorderBottomHour = bsColors.clrCellBorder;
				int nScale = (nCellBusyStatus == xtpCalendarBusyStatusOutOfOffice) ? 110 : 30;
				m_clrsetWorkCell.clrBorderBottomInHour = XTPDrawHelpers()->LightenColor(nScale, bsColors.clrCellBorder);
			}
			else
			{
				if (nCellBusyStatus != xtpCalendarBusyStatusTentative)
					m_clrsetNonWorkCell.clrBackground = bsColors.clrBkNonWorkCell;

				m_clrsetNonWorkCell.clrBorderBottomHour = bsColors.clrCellBorder;
				int nScale = (nCellBusyStatus == xtpCalendarBusyStatusOutOfOffice) ? 54 : 30;
				m_clrsetNonWorkCell.clrBorderBottomInHour = XTPDrawHelpers()->LightenColor(nScale, bsColors.clrCellBorder);
			}
		}
		else
		{
			if (nCellBusyStatus == xtpCalendarBusyStatusBusy ||
				nCellBusyStatus == xtpCalendarBusyStatusTentative)
			{
				if (nCellBusyStatus == xtpCalendarBusyStatusBusy)
				{
					//GetColorsSetPart()->SetColor(xtpCLR_DayViewSingleDayEventBkGRto,          155); // 191, 210, 234 // single-day event gradient fill Dark
					m_clrsetWorkCell.clrBackground = XTPDrawHelpers()->LightenColor(155, GetTheme()->m_clrBaseColor);
					m_clrsetNonWorkCell.clrBackground = (COLORREF)m_clrsetWorkCell.clrBackground;
				}

				m_clrsetWorkCell.clrBorderBottomHour = GetTheme()->GetColorsSetPart()->GetColor(xtpCLR_HeaderBorder); // 141, 174, 217
				m_clrsetNonWorkCell.clrBorderBottomHour = (COLORREF)m_clrsetWorkCell.clrBorderBottomHour;

				m_clrsetWorkCell.clrBorderBottomInHour = GetTheme()->GetColorsSetPart()->GetColor(xtpCLR_DayViewCellWorkBorderBottomHour);
				m_clrsetNonWorkCell.clrBorderBottomInHour = (COLORREF)m_clrsetWorkCell.clrBorderBottomInHour;
			}

			if (nCellBusyStatus == xtpCalendarBusyStatusTentative)
				m_clrDynHatchBrush = m_clrsetWorkCell.clrBorderBottomHour;
		}
	}

	if (pStateCnt)
		pStateCnt->SendBeforeDrawThemeObject((LPARAM)&cellParams, xtpCalendarBeforeDraw_DayViewCell);

	TBase::Draw(pDC, rcRect, cellParams, NULL);

	//if (nCellBusyStatus == xtpCalendarBusyStatusTentative)
	if (m_clrDynHatchBrush != COLORREF_NULL)
	{
		COLORREF clrBk = 0;
		if (cellParams.bSelected)
			clrBk = cellParams.bWorkCell ? m_clrsetWorkCell.clrSelected : m_clrsetNonWorkCell.clrSelected;
		else
			clrBk = cellParams.bWorkCell ? m_clrsetWorkCell.clrBackground : m_clrsetNonWorkCell.clrBackground;

		CXTPTextDC autoTxt(pDC, COLORREF_NULL, clrBk);
		CBrush br(HS_BDIAGONAL, m_clrDynHatchBrush);

		CRect rc = rcRect;
		if (cellParams.nIndex != cellParams.pViewGroup->GetViewDay()->GetView()->GetTopRow())
			rc.top++;

		pDC->FillRect(&rc, &br);
	}

	autoStateCnt0.RestoreState();
	autoStateCnt0.Clear();

	if (pStateCnt)
		pStateCnt->RestoreState();
}

void CXTPCalendarThemeOffice2007::CTODayViewDayGroupAllDayEvents::SetDynColors(CXTPCalendarDayViewGroup* pViewGroup)
{
	CXTPCalendarThemeOffice2007* pThemeX = DYNAMIC_DOWNCAST(CXTPCalendarThemeOffice2007, GetTheme());
	CXTPCalendarThemeOffice2007::CTOCellDynBackground* pCellDynBkPart =
		XTP_SAFE_GET1(pThemeX, GetCellDynBackgroundPart(), NULL);

	if (!pViewGroup || !pCellDynBkPart || !GetTheme() || !GetTheme()->GetColorsSetPart())
	{
		ASSERT(FALSE);
		return;
	}

	COleDateTime dtDate = pViewGroup->GetViewDay()->GetDayDate();

	CXTPCalendarTheme::XTP_BUSY_STATUS_CONTEXT bsColors;
	m_nDynBusyStatus = pCellDynBkPart->GetCellDynBackground(pViewGroup, dtDate, -1, bsColors);

	m_clrDynHatch = 0;

	if (m_nDynBusyStatus == xtpCalendarBusyStatusBusy ||
		m_nDynBusyStatus == xtpCalendarBusyStatusOutOfOffice ||
		m_nDynBusyStatus == xtpCalendarBusyStatusTentative)
	{
		if (bsColors.clrBkAllDayEvents != 0 && bsColors.clrCellBorder != 0) // is Category or OutOfOffice
		{
			m_clrDynHatch = bsColors.clrBkAllDayEvents;

			if (m_nDynBusyStatus != xtpCalendarBusyStatusTentative)
			{
				m_clrBackground = bsColors.clrBkAllDayEvents;
				m_clrBackgroundSelected = CXTPCalendarThemeOffice2007::PowerColor(bsColors.clrBkAllDayEvents, -7.5, 0.25);
			}
			else
			{
				m_clrBackground.SetDefaultValue();
				m_clrBackgroundSelected.SetDefaultValue();
			}
		}
		else
		{
			if (m_nDynBusyStatus == xtpCalendarBusyStatusBusy)
			{
				m_clrBackground = GetTheme()->GetColorsSetPart()->GetColor(xtpCLR_HeaderBorder); // 141, 174, 217
			}
			else
			{
				m_clrBackground.SetDefaultValue();
				m_clrBackgroundSelected.SetDefaultValue();
			}

			m_clrDynHatch = GetTheme()->GetColorsSetPart()->GetColor(xtpCLR_HeaderBorder); // 141, 174, 217;
		}
	}
	else
	{
		m_clrBackground.SetDefaultValue();
		m_clrBackgroundSelected.SetDefaultValue();
	}
}

void CXTPCalendarThemeOffice2007::CTODayViewDayGroupAllDayEvents::Draw(CCmdTarget* pObject, CDC* pDC)
{
	CXTPCalendarThemeOffice2007* pThemeX = DYNAMIC_DOWNCAST(CXTPCalendarThemeOffice2007, GetTheme());
	CXTPCalendarDayViewGroup* pDayViewGroup = DYNAMIC_DOWNCAST(CXTPCalendarDayViewGroup, pObject);
	if (!pDC || !pDayViewGroup || !pDayViewGroup->GetViewDay() || !pThemeX)
	{
		//ASSERT(FALSE);
		return;
	}
	CRect rcRectBk = pDayViewGroup->GetAllDayEventsRect();
	CRect rcRect2 = rcRectBk;

	if (rcRectBk.IsRectEmpty())
	{
		return;
	}

	BOOL bIsOffice2010Theme = (GetCalendarControl()->GetThemeVersion() == xtpCalendarVersion2010);

	if (bIsOffice2010Theme)
		rcRectBk.bottom -= 1;
	else
		rcRectBk.bottom -= 2;

	rcRect2.top = rcRectBk.bottom;

	BOOL bIsSelected = GetTheme()->GetDayViewPart()->GetDayPart()->GetGroupPart()->IsSelected(pDayViewGroup);
	BOOL bIsToday = GetTheme()->GetDayViewPart()->GetHeaderPart()->IsToday(pDayViewGroup->GetViewDay());

	COLORREF clrBk = bIsSelected ? m_clrBackgroundSelected : m_clrBackground;
	COLORREF clrBorder = bIsToday ? (COLORREF)pThemeX->GetDayViewPartX()->GetDayPartX()->m_clrTodayBorder : m_clrBottomBorder;

	if (bIsOffice2010Theme && bIsToday)
		clrBorder = m_clrBottomBorder;

	pDC->FillSolidRect(&rcRectBk, clrBk);
	pDC->FillSolidRect(&rcRect2, clrBorder);

	if (m_nDynBusyStatus == xtpCalendarBusyStatusTentative)
	{
		CXTPTextDC autoTxt(pDC, COLORREF_NULL, clrBk);
		CBrush br(HS_BDIAGONAL, m_clrDynHatch);

		pDC->FillRect(&rcRectBk, &br);
	}
}

void CXTPCalendarThemeOffice2007::CTODayViewDayGroup::Draw(CCmdTarget* pObject, CDC* pDC)
{
	TBase::Draw(pObject, pDC);

	CXTPCalendarDayViewGroup* pDVGroup = DYNAMIC_DOWNCAST(CXTPCalendarDayViewGroup, pObject);
	CXTPCalendarThemeOffice2007* pThemeX = DYNAMIC_DOWNCAST(CXTPCalendarThemeOffice2007, GetTheme());
	if (!pDC || !pDVGroup || !pThemeX)
	{
		ASSERT(FALSE);
		return;
	}

	XTP_DAY_VIEW_GROUP_LAYOUT& groupLayout = GetTheme()->GetDayViewGroupLayout(pDVGroup);

	if (pDC->IsPrinting())
		return;

	groupLayout.m_nHotState &= ~(XTP_DAY_VIEW_GROUP_LAYOUT::xtpHotScrollUp | XTP_DAY_VIEW_GROUP_LAYOUT::xtpHotScrollDown);

	int nHit = HitTestScrollButton(pDVGroup);

	if (pDVGroup->IsExpandUp())
	{
		int nButton = XTP_DAY_VIEW_GROUP_LAYOUT::xtpHotScrollUp;
		CRect rc = GetScrollButtonRect(groupLayout, nButton);

		BOOL bHot = !!(nHit & xtpCalendarHitTestDayViewScrollUp);

		pThemeX->DrawExpandDayButton(pDC, rc, TRUE, bHot);

		if (bHot)
			groupLayout.m_nHotState |= nButton;
	}

	if (pDVGroup->IsExpandDown())
	{
		int nButton = XTP_DAY_VIEW_GROUP_LAYOUT::xtpHotScrollDown;
		CRect rc = GetScrollButtonRect(groupLayout, nButton);

		BOOL bHot = !!(nHit & xtpCalendarHitTestDayViewScrollDown);

		pThemeX->DrawExpandDayButton(pDC, rc, FALSE, bHot);

		if (bHot)
			groupLayout.m_nHotState |= nButton;
	}
}

void CXTPCalendarThemeOffice2007::CTODayViewDayGroup::OnMouseMove(CCmdTarget* pObject, UINT nFlags, CPoint point)
{
	UNREFERENCED_PARAMETER(nFlags);

	CXTPCalendarThemeOffice2007* pThemeX = DYNAMIC_DOWNCAST(CXTPCalendarThemeOffice2007, GetTheme());
	CXTPCalendarDayViewGroup* pDVGroup = DYNAMIC_DOWNCAST(CXTPCalendarDayViewGroup, pObject);
	if (!pDVGroup || !pThemeX || !pThemeX->GetCalendarControl())
	{
		ASSERT(FALSE);
		return;
	}

	XTP_VIEW_GROUP_LAYOUT groupLayout = GetTheme()->GetViewGroupLayout(pDVGroup);
	XTP_DAY_VIEW_GROUP_LAYOUT& groupLayoutX = GetTheme()->GetDayViewGroupLayout(pDVGroup);
	BOOL bRedraw = FALSE;

	int nHit = HitTestScrollButton(pDVGroup, &point);

	BOOL bHot = !!(nHit & xtpCalendarHitTestDayViewScrollUp);
	BOOL bHotPrev = !!(groupLayoutX.m_nHotState & XTP_DAY_VIEW_GROUP_LAYOUT::xtpHotScrollUp);
	bRedraw |= (bHot != bHotPrev);

	bHot = !!(nHit & xtpCalendarHitTestDayViewScrollDown);
	bHotPrev = !!(groupLayoutX.m_nHotState & XTP_DAY_VIEW_GROUP_LAYOUT::xtpHotScrollDown);
	bRedraw |= (bHot != bHotPrev);

	bHot = groupLayoutX.m_bShowHeader && groupLayout.m_rcGroupHeader.PtInRect(point);
	bHotPrev = !!(groupLayoutX.m_nHotState & XTP_DAY_VIEW_GROUP_LAYOUT::xtpHotHeader);
	bRedraw |= (bHot != bHotPrev);

	if (bRedraw)
		CXTPCalendarControl::CUpdateContext updateContext(pThemeX->GetCalendarControl(), xtpCalendarUpdateRedraw);
}

BOOL CXTPCalendarThemeOffice2007::CTODayViewDayGroup::OnLButtonDown(CCmdTarget* pObject, UINT nFlags, CPoint point)
{
	UNREFERENCED_PARAMETER(nFlags);

	CXTPCalendarThemeOffice2007* pThemeX = DYNAMIC_DOWNCAST(CXTPCalendarThemeOffice2007, GetTheme());
	CXTPCalendarDayViewGroup* pDVGroup = DYNAMIC_DOWNCAST(CXTPCalendarDayViewGroup, pObject);
	if (!pDVGroup || !pDVGroup->GetViewDay() || !pThemeX || !pThemeX->GetCalendarControl())
	{
		ASSERT(FALSE);
		return FALSE;
	}

	int nHitScrollIcons = HitTestAllDayEventsScrollIcons(pDVGroup, point);
	if(nHitScrollIcons & xtpCalendarHitTestAllDayEventsScrollUp)
	{
		pDVGroup->ScrollIcon_ScrollAllDayEventsUp();
		return TRUE;
	}

	if(nHitScrollIcons & xtpCalendarHitTestAllDayEventsScrollDown)
	{
		pDVGroup->ScrollIcon_ScrollAllDayEventsDown();
		return TRUE;
	}

	int nHit = HitTestScrollButton(pDVGroup, &point);

	if (nHit & xtpCalendarHitTestDayViewScrollUp)
	{
		pThemeX->GetCalendarControl()->PostMessage(xtp_wm_UserAction,
			(WPARAM)(xtpCalendarUserAction_OnScrollDay | xtpCalendarScrollDayButton_DayViewUp),
			MAKELPARAM((WORD)pDVGroup->GetGroupIndex(), pDVGroup->GetViewDay()->GetDayIndex()) );
		return TRUE;
	}

	if (nHit & xtpCalendarHitTestDayViewScrollDown)
	{
		pThemeX->GetCalendarControl()->PostMessage(xtp_wm_UserAction,
			(WPARAM)(xtpCalendarUserAction_OnScrollDay | xtpCalendarScrollDayButton_DayViewDown),
			MAKELPARAM((WORD)pDVGroup->GetGroupIndex(), pDVGroup->GetViewDay()->GetDayIndex()) );
		return TRUE;
	}

	return FALSE;
}

int CXTPCalendarThemeOffice2007::CTODayViewDayGroup::HitTestAllDayEventsScrollIcons(const CXTPCalendarDayViewGroup* pDVGroup, const CPoint& point)
{
	if(!pDVGroup->NeedScrollAllDayEvents())
		return 0;

	CXTPCalendarThemeOffice2007* pThemeX = DYNAMIC_DOWNCAST(CXTPCalendarThemeOffice2007, GetTheme());
	if(!pThemeX)
		return 0;

	if(pDVGroup->IsUpAllDayEventsScrollButtonVisible())
	{
		CRect rcUpScrollIcon = pDVGroup->GetAllDayEventsScrollButtonRect(XTP_DAY_VIEW_GROUP_LAYOUT::xtpHotAllDayEventsScrollUp);
		if(pThemeX->ExpandDayButton_HitTest(point, rcUpScrollIcon, TRUE))
			return xtpCalendarHitTestAllDayEventsScrollUp;
	}

	if(pDVGroup->IsDownAllDayEventsScrollButtonVisible())
	{
		CRect rcDownScrollIcon = pDVGroup->GetAllDayEventsScrollButtonRect(XTP_DAY_VIEW_GROUP_LAYOUT::xtpHotAllDayEventsScrollDown);
		if(pThemeX->ExpandDayButton_HitTest(point, rcDownScrollIcon, FALSE))
			return xtpCalendarHitTestAllDayEventsScrollDown;
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////
//class CXTPCalendarTheme_DayViewSingleDayEventPlaceBuilder
CXTPCalendarTheme_DayViewSingleDayEventPlaceBuilder::CXTPCalendarTheme_DayViewSingleDayEventPlaceBuilder()
{
	m_mapEvToOverlapped.InitHashTable(XTP_CALENDAR_OFFICE2007_DVG_DEV_AVE_COUNT, FALSE);
	m_spCellDuration.SetDateTimeSpan(0, 0, 30, 0);
	m_dtMinStartTime = (DATE)0;
}

void CXTPCalendarTheme_DayViewSingleDayEventPlaceBuilder::BuildEventPlaces(CXTPCalendarDayViewGroup* pViewGroup)
{
	ASSERT(pViewGroup);
	if (!pViewGroup)
		return;

	m_spCellDuration = XTP_SAFE_GET3(pViewGroup, GetViewDay(), GetView(), GetCellDuration(), 0);
	if (double(m_spCellDuration) <= XTP_HALF_SECOND)
	{
		m_spCellDuration.SetDateTimeSpan(0, 0, 30, 0);
	}

	DATE dtStart = XTP_SAFE_GET2(pViewGroup, GetViewDay(), GetDayDate(), (DATE)0);

	m_dtMinStartTime = dtStart + COleDateTimeSpan(1, 0, 0, 1) - m_spCellDuration;

	DATE dtScaleMaxTime = XTP_SAFE_GET2(pViewGroup->GetCalendarControl(), GetCalendarOptions(), dtScaleMaxTime, (DATE)0);

	if ((DATE)m_dtMinStartTime < dtStart + dtScaleMaxTime)
		m_dtMinStartTime = dtStart + dtScaleMaxTime;


	_Clear();

	int nCount = pViewGroup->GetViewEventsCount();
	for (int i = 0; i < nCount; i++)
	{
		CXTPCalendarDayViewEvent* pViewEvent = pViewGroup->GetViewEvent(i);
		ASSERT(pViewEvent);
		if (!pViewEvent || pViewEvent->IsMultidayEvent())
			continue;

		// Reset Events Paces
		pViewEvent->SetEventPlacePos(XTP_EVENT_PLACE_POS_UNDEFINED);
		pViewEvent->SetEventPlaceCount(1);

		// Update X Data for each event
		_UpdateDataFor(pViewEvent, pViewGroup);

	}

	// Update Events Paces using X Data
	_UpdateEventsPaces(pViewGroup);

	// Delete intermediate X data
	_Clear();
}

void CXTPCalendarTheme_DayViewSingleDayEventPlaceBuilder::_Clear()
{
	m_mapEvToOverlapped.RemoveAll();
}

void CXTPCalendarTheme_DayViewSingleDayEventPlaceBuilder::_UpdateDataFor(CXTPCalendarDayViewEvent* pViewEvent,
													CXTPCalendarDayViewGroup* pViewGroup)
{
	if (!pViewEvent || !pViewGroup || !pViewGroup->GetViewDay())
	{
		ASSERT(FALSE);
		return;
	}

	COleDateTime dtStart = pViewEvent->GetEvent()->GetStartTime();
	COleDateTime dtEnd = pViewEvent->GetEvent()->GetEndTime();

	COleDateTime dtGroupDay = pViewGroup->GetViewDay()->GetDayDate();
	if (dtStart < dtGroupDay)
	{
		dtStart = dtGroupDay;
		ASSERT(dtStart <= dtEnd);
	}
	COleDateTimeSpan spEventDuration2 = dtEnd - dtStart;


	// adjust end time if need
	//if (pViewEvent->GetEvent()->GetDuration() < m_spCellDuration)
	if (spEventDuration2 < m_spCellDuration)
	{
		dtEnd = dtStart + m_spCellDuration;
	}

	// adjust start time if need
	if (dtStart > m_dtMinStartTime)
	{
		dtStart = m_dtMinStartTime;
	}

	//=====================================================
	int nCount = pViewGroup->GetViewEventsCount();
	for (int i = 0; i < nCount; i++)
	{
		CXTPCalendarDayViewEvent* pVEv_i = pViewGroup->GetViewEvent(i);
		ASSERT(pVEv_i);
		if (!pVEv_i || pVEv_i->IsMultidayEvent() || !pVEv_i->GetEvent())
			continue;

		COleDateTime dtStart_i = pVEv_i->GetEvent()->GetStartTime();
		COleDateTime dtEnd_i = pVEv_i->GetEvent()->GetEndTime();

		if (dtStart_i < dtGroupDay)
		{
			dtStart_i = dtGroupDay;
			ASSERT(dtStart_i <= dtEnd_i);
		}
		COleDateTimeSpan spEventDuration2_i = dtEnd_i - dtStart_i;

		// adjust end time if need
		//if (pVEv_i->GetEvent()->GetDuration() < m_spCellDuration)
		if (spEventDuration2_i < m_spCellDuration)
		{
			dtEnd_i = dtStart_i + m_spCellDuration;
		}

		// adjust start time if need
		if (dtStart_i > m_dtMinStartTime)
		{
			dtStart_i = m_dtMinStartTime;
		}

		// is Overlapped
		if (!(dtEnd_i < dtStart || dtStart_i > dtEnd ||
			CXTPCalendarUtils::IsEqual(dtEnd_i, dtStart) ||
			CXTPCalendarUtils::IsEqual(dtStart_i, dtEnd)))
		{
			_XAddOverlapped(pViewEvent, pVEv_i);
		}
	}
}

void CXTPCalendarTheme_DayViewSingleDayEventPlaceBuilder::_UpdateEventsPaces(CXTPCalendarDayViewGroup* pViewGroup)
{
	UNREFERENCED_PARAMETER(pViewGroup);

	// assign event places
	int i;
	int nCount = pViewGroup->GetViewEventsCount();

	for (i = 0; i < nCount; i++)
	{
		CXTPCalendarDayViewEvent* pViewEvent = pViewGroup->GetViewEvent(i);
		ASSERT(pViewEvent);
		if (!pViewEvent || pViewEvent->IsMultidayEvent() || !pViewEvent->GetEvent())
			continue;

		ASSERT(pViewEvent->GetEventPlacePos() == XTP_EVENT_PLACE_POS_UNDEFINED);

		if (pViewEvent->GetEventPlacePos() == XTP_EVENT_PLACE_POS_UNDEFINED)
		{
			CViewEventsMap* pMapOvr = _XGetDataFor(pViewEvent);
			ASSERT(pMapOvr);
			if (pMapOvr)
			{
				int nPlace = pMapOvr->GetMinFreeEventPlace();
				ASSERT(nPlace >= 0 && nPlace != XTP_EVENT_PLACE_POS_UNDEFINED);
				pViewEvent->SetEventPlacePos(nPlace);
			}
		}
	}

	// update event places count
	for (i = 0; i < nCount; i++)
	{
		CXTPCalendarDayViewEvent* pViewEvent = pViewGroup->GetViewEvent(i);
		ASSERT(pViewEvent);
		if (!pViewEvent || pViewEvent->IsMultidayEvent() || !pViewEvent->GetEvent())
			continue;

		ASSERT(pViewEvent->GetEventPlacePos() != XTP_EVENT_PLACE_POS_UNDEFINED);

		if (pViewEvent->GetEventPlacePos() != XTP_EVENT_PLACE_POS_UNDEFINED)
		{
			CViewEventsMap* pMapOvr = _XGetDataFor(pViewEvent);
			ASSERT(pMapOvr);
			if (pMapOvr)
			{
				int nPlaceMax = pMapOvr->GetMaxEventPlace();
				ASSERT(nPlaceMax >= 0);
				pViewEvent->SetEventPlaceCount(nPlaceMax + 1);
			}
		}
	}

	// update event places count to maximum from overlapped set
	// TODO: update to recurrence
	for (int k = 0; k < min(nCount, 3); k++)
	{
		for (i = 0; i < nCount; i++)
		{
			CXTPCalendarDayViewEvent* pViewEvent = pViewGroup->GetViewEvent(i);
			ASSERT(pViewEvent);
			if (!pViewEvent || pViewEvent->IsMultidayEvent() || !pViewEvent->GetEvent())
				continue;

			ASSERT(pViewEvent->GetEventPlacePos() != XTP_EVENT_PLACE_POS_UNDEFINED);

			if (pViewEvent->GetEventPlacePos() != XTP_EVENT_PLACE_POS_UNDEFINED)
			{
				CViewEventsMap* pMapOvr = _XGetDataFor(pViewEvent);
				ASSERT(pMapOvr);
				if (pMapOvr)
				{
					int nPlaceCountMax = pMapOvr->GetMaxEventPlaceCountEx();
					ASSERT(nPlaceCountMax >= pViewEvent->GetEventPlaceCount());
					pViewEvent->SetEventPlaceCount(nPlaceCountMax);
				}
			}
		}
	}
}

CXTPCalendarTheme_DayViewSingleDayEventPlaceBuilder::CViewEventsMap* CXTPCalendarTheme_DayViewSingleDayEventPlaceBuilder::
			_XGetDataFor(CXTPCalendarDayViewEvent* pViewEvent)
{
	// CViewEventsMap* pVEvMap = NULL;
	// to avoid warning C4786 CObject* used.
	CObject* pVEvMap = NULL;

	if (m_mapEvToOverlapped.Lookup(pViewEvent, pVEvMap) && pVEvMap)
	{
		return (CViewEventsMap*)pVEvMap;
	}

	// Create a new sub-data map
	pVEvMap = new CViewEventsMap();

	if (pVEvMap)
	{
		m_mapEvToOverlapped[pViewEvent] = pVEvMap;
	}

	return (CViewEventsMap*)pVEvMap;
}

void CXTPCalendarTheme_DayViewSingleDayEventPlaceBuilder::_XAddOverlapped(
					CXTPCalendarDayViewEvent* pVEv_base,
					CXTPCalendarDayViewEvent* pVEv_overlapped)
{
	CViewEventsMap* pVEvMap = _XGetDataFor(pVEv_base);
	if (!pVEvMap) // no memory to create
		return;

	pVEvMap->SetAt(pVEv_overlapped, 0);
}

/////////////////////////////////////////////////////////////////////////////
CXTPCalendarTheme_DayViewSingleDayEventPlaceBuilder::CViewEventsMap::CViewEventsMap()
{
	InitHashTable(XTP_CALENDAR_OFFICE2007_DVG_DEV_AVE_COUNT, FALSE);
}

int CXTPCalendarTheme_DayViewSingleDayEventPlaceBuilder::CViewEventsMap::GetMinFreeEventPlace()
{
	int nMinFreeEventPlace = 0;

	BOOL bBysy = FALSE;
	do
	{
		bBysy = FALSE;

		POSITION pos = GetStartPosition();
		while (pos)
		{
			CXTPCalendarDayViewEvent* pVEv = NULL;
			int nTmp = 0;
			GetNextAssoc(pos, pVEv, nTmp);

			if (pVEv && pVEv->GetEventPlacePos() != XTP_EVENT_PLACE_POS_UNDEFINED &&
				pVEv->GetEventPlacePos() == nMinFreeEventPlace)
			{
				nMinFreeEventPlace++;
				bBysy = TRUE;
				break;
			}
		}
	}
	while (bBysy);

	return nMinFreeEventPlace;
}

int CXTPCalendarTheme_DayViewSingleDayEventPlaceBuilder::CViewEventsMap::GetMaxEventPlace()
{
	int nMaxEventPlace = 0;

	POSITION pos = GetStartPosition();
	while (pos)
	{
		CXTPCalendarDayViewEvent* pVEv = NULL;
		int nTmp = 0;
		GetNextAssoc(pos, pVEv, nTmp);

		ASSERT(pVEv && pVEv->GetEventPlacePos() != XTP_EVENT_PLACE_POS_UNDEFINED);

		if (pVEv && pVEv->GetEventPlacePos() != XTP_EVENT_PLACE_POS_UNDEFINED &&
			pVEv->GetEventPlacePos() > nMaxEventPlace)
		{
			nMaxEventPlace = pVEv->GetEventPlacePos();
		}
	}

	return nMaxEventPlace;
}

int CXTPCalendarTheme_DayViewSingleDayEventPlaceBuilder::CViewEventsMap::GetMaxEventPlaceCountEx()
{
	int nMaxEventPlaceCount = 1;

	POSITION pos = GetStartPosition();
	while (pos)
	{
		CXTPCalendarDayViewEvent* pVEv = NULL;
		int nTmp = 0;
		GetNextAssoc(pos, pVEv, nTmp);

		ASSERT(pVEv && pVEv->GetEventPlacePos() != XTP_EVENT_PLACE_POS_UNDEFINED);

		if (pVEv && pVEv->GetEventPlaceCount() > nMaxEventPlaceCount)
			nMaxEventPlaceCount = pVEv->GetEventPlaceCount();
	}

	return nMaxEventPlaceCount;
}

//////////////////////////////////////////////////////////////////////////
BOOL CXTPCalendarThemeOffice2007::CTODayViewDayGroup::IsSelected(CXTPCalendarViewGroup* pViewGroup)
{
	if (!pViewGroup || !pViewGroup->GetViewDay_() || !pViewGroup->GetViewDay_()->GetView_())
	{
		ASSERT(FALSE);
		return FALSE;
	}

	COleDateTime dtBegin;
	COleDateTime dtEnd;

	if (pViewGroup->GetViewDay_()->GetView_()->GetSelection(&dtBegin, &dtEnd))
	{
		COleDateTime dtDay = pViewGroup->GetViewDay_()->GetDayDate();
		ASSERT(CXTPCalendarUtils::IsZeroTime(dtDay));
		dtDay = CXTPCalendarUtils::ResetTime(dtDay);

		COleDateTimeSpan spSel = dtBegin - dtEnd;
		int nSelDays = abs(CXTPCalendarUtils::GetTotalDays(spSel));

		dtBegin = CXTPCalendarUtils::ResetTime(dtBegin);
		int nCorrector = CXTPCalendarUtils::IsZeroTime(dtEnd) ? 0 : 1;
		dtEnd = CXTPCalendarUtils::ResetTime(dtEnd) + COleDateTimeSpan(nCorrector, 0, 0, 0);

//---------------------------fix for one column AllDay cell selection
		CXTPCalendarDayView* pDayView = DYNAMIC_DOWNCAST(CXTPCalendarDayView, XTP_SAFE_GET2(GetTheme(), GetCalendarControl(), GetDayView(), NULL));
		if (pDayView != NULL && pDayView->GetTotalGroupsCount() > 1)
		{
			COleDateTime dtB;
			COleDateTime dtE;
			BOOL bAllDay;
			int iGroup;
			COleDateTimeSpan dtSpan;

			if (pViewGroup->GetViewDay_()->GetView_()->GetSelection(&dtB, &dtE, &bAllDay, &iGroup, &dtSpan))
			{
				if (pViewGroup->GetGroupIndex() == iGroup && bAllDay)
					return nSelDays > 0 && dtBegin <= dtDay && dtDay < dtEnd;
			}
		}
//---------------------------fix for one column AllDay cell selection
		else
			return nSelDays > 0 && dtBegin <= dtDay && dtDay < dtEnd;
	}
	return FALSE;
}

void CXTPCalendarThemeOffice2007::CTODayViewEvent_MultiDay::CalcEventYs(CRect& rrcRect, int nEventPlaceNumber)
{
	CXTPCalendarDayView* pView = DYNAMIC_DOWNCAST(CXTPCalendarDayView, XTP_SAFE_GET2(GetTheme(), GetCalendarControl(), GetDayView(), NULL));
	if (!pView)
	{
		ASSERT(FALSE);
		return;
	}

	XTP_DAY_VIEW_LAYOUT& rLayoutX = GetTheme()->GetDayViewLayout(pView);
	int nAllDayEventHeight = rLayoutX.m_nAllDayEventHeight;

	rrcRect.top = rLayoutX.m_rcAllDayEvents.top + nAllDayEventHeight * nEventPlaceNumber;
	rrcRect.bottom = rrcRect.top + nAllDayEventHeight;
}

void CXTPCalendarThemeOffice2007::CTODayViewEvent_MultiDay::AdjustLayout(CCmdTarget* pObject, CDC* pDC,
		const CRect& rcRect, int nEventPlaceNumber)
{
	UNREFERENCED_PARAMETER(rcRect);

	CXTPCalendarDayViewEvent* pDayViewEvent = DYNAMIC_DOWNCAST(CXTPCalendarDayViewEvent, pObject);
	CXTPCalendarDayView* pView = XTP_SAFE_GET3(pDayViewEvent, GetViewGroup(), GetViewDay(), GetView(), NULL);
	CXTPCalendarEvent* pEvent = XTP_SAFE_GET1(pDayViewEvent, GetEvent(), NULL);
	if (!pDC || !pDayViewEvent || !pView || !pEvent)
	{
		ASSERT(FALSE);
		return;
	}

	if (!pDayViewEvent->IsMultidayEvent())
	{
		ASSERT(FALSE);
		return;
	}

	//**********************
	int nMDEFlags = pDayViewEvent->GetMultiDayEventFlags();

	CalcEventYs(pDayViewEvent->m_rcEventMax, nEventPlaceNumber);

	pDayViewEvent->m_rcEvent.CopyRect(&pDayViewEvent->m_rcEventMax);
	pDayViewEvent->m_rcText.CopyRect(&pDayViewEvent->m_rcEventMax);

	if ((nMDEFlags & xtpCalendarMultiDayFirst) != 0)
		pDayViewEvent->m_rcEvent.left += 8;

	if ((nMDEFlags & xtpCalendarMultiDayLast) != 0)
		pDayViewEvent->m_rcEvent.right -= 8;

	pDayViewEvent->m_rcText.CopyRect(pDayViewEvent->m_rcEvent);
}

void CXTPCalendarThemeOffice2007::CTODayViewEvent_MultiDay::Draw(CCmdTarget* pObject, CDC* pDC)
{
	CXTPCalendarDayViewEvent* pViewEvent = DYNAMIC_DOWNCAST(CXTPCalendarDayViewEvent, pObject);
	CXTPCalendarThemeOffice2007* pThemeX = DYNAMIC_DOWNCAST(CXTPCalendarThemeOffice2007, GetTheme());

	if (!pDC || !pThemeX || !pViewEvent || !pViewEvent->GetEvent())
	{
		ASSERT(FALSE);
		return;
	}

	BOOL bNoVisibleMDEvent = pViewEvent->IsMultidayEvent() && !pViewEvent->IsVisible();

	int nMDEFlags = pViewEvent->GetMultiDayEventFlags();
	if ((nMDEFlags & xtpCalendarMultiDaySlave) || bNoVisibleMDEvent)
		return;// was drawn in the Master (or out of events rect)

	CRect rcEventMax(pViewEvent->GetViewEventRectMax());
	CRect rcEvent(pViewEvent->GetEventRect());
	if (rcEvent.IsRectEmpty())
		return;

	// fill event background
	BOOL bSelected = pViewEvent->IsSelected();

	COLORREF clrBorderNotSel;
	COLORREF clrBorder;
	CXTPPaintManagerColorGradient grclrBk;

	GetEventColors(pDC, pViewEvent, clrBorder, clrBorderNotSel, grclrBk);

	//----------------
	pThemeX->DrawEventRect(pDC, rcEvent, clrBorder, bSelected, grclrBk);

	int nSelOffset = bSelected ? 2 : 1;

	if (nMDEFlags & xtpCalendarMultiDayFirst)
	{
		pThemeX->_DrawEventLTHighlight(pDC, CPoint(rcEvent.left + nSelOffset, rcEvent.top + nSelOffset),
			rcEvent.right - nSelOffset, grclrBk, cnCornerSize / 2, rcEvent.bottom);
	}

	CRect rcText(rcEvent); //(pViewEvent->GetTextRect());
	rcText.DeflateRect(3, 2, 3, 2);

	BOOL bHideArrow = FALSE;
	BOOL bHideArrowText = FALSE;

	if ((nMDEFlags & xtpCalendarMultiDayFirst) == 0)
	{
		rcText.left = rcEventMax.left;

		CRect rcSQ = rcEventMax;
		rcSQ.right = min(rcEvent.left + cnCornerSize, rcEventMax.right);
		CXTPCalendarTheme::DrawRectPartly(pDC, rcSQ, clrBorder, bSelected, grclrBk, CRect(1,1,0,1));

		pThemeX->_DrawEventLTHighlight(pDC, CPoint(rcEvent.left + nSelOffset, rcEvent.top + nSelOffset),
			rcEvent.right - nSelOffset, grclrBk, 0, rcEvent.bottom - nSelOffset);

		CRect rcArrow;
		BOOL bArrow = Draw_ArrowL(pViewEvent, pDC, rcArrow);
		bHideArrow = !bArrow;
		if (bArrow)
			rcText.left = rcArrow.right;

		BOOL bShowArrowText = (nMDEFlags & xtpCalendarMultiDayLast) == 0 ||
			(BOOL)m_bShowFromToArrowTextAlways;

		if (bArrow && bShowArrowText)
		{
			CRect rcArrowText;
			CThemeFontColorSetValue* pFCsetFromTo = bSelected ? &m_fcsetSelected.fcsetStartEnd : &m_fcsetNormal.fcsetStartEnd;

			BOOL bArrowText = Draw_ArrowLtext(pViewEvent, pDC, pFCsetFromTo, rcArrowText, rcArrow.right);
			bHideArrowText = !bArrowText;
			if (bArrowText)
				rcText.left = rcArrowText.right;
		}
	}
	else if (bSelected)
	{
		// draw left Gripper
		CRect rcGripper;
		rcGripper.top = rcEvent.top + rcEvent.Height() / 2 - cnGripperSize / 2;
		rcGripper.bottom = rcGripper.top + cnGripperSize;
		rcGripper.left = rcEvent.left - 1;
		rcGripper.right = rcGripper.left + cnGripperSize;

		pDC->FillSolidRect(&rcGripper, m_clrGripperBackground);
		CBrush brBorder(m_clrGripperBorder);
		pDC->FrameRect(&rcGripper, &brBorder);
	}

	if ((nMDEFlags & xtpCalendarMultiDayLast) == 0)
	{
		rcText.right = rcEventMax.right;

		CRect rcSQ = rcEventMax;
		rcSQ.left = max(rcEvent.right - cnCornerSize, rcEventMax.left);
		CXTPCalendarTheme::DrawRectPartly(pDC, rcSQ, clrBorder, bSelected, grclrBk, CRect(0,1,1,1));

		pThemeX->_DrawEventLTHighlight(pDC, CPoint(rcSQ.left, rcEvent.top + nSelOffset),
			rcEventMax.right - nSelOffset, grclrBk, 0, INT_MAX, FALSE);
		if (!bHideArrow)
		{
			CRect rcArrow;

			BOOL bArrow = Draw_ArrowR(pViewEvent, pDC, rcArrow);
			if (bArrow)
				rcText.right = rcArrow.left;

			BOOL bShowArrowText = (nMDEFlags & xtpCalendarMultiDayFirst) == 0 ||
				(BOOL)m_bShowFromToArrowTextAlways;

			if (bArrow && bShowArrowText && !bHideArrowText)
			{
				CRect rcArrowText;
				CThemeFontColorSetValue* pFCsetFromTo = bSelected ? &m_fcsetSelected.fcsetStartEnd : &m_fcsetNormal.fcsetStartEnd;

				BOOL bArrowText = Draw_ArrowRtext(pViewEvent, pDC, pFCsetFromTo, rcArrowText, rcArrow.left);
				if (bArrowText)
				{
					rcText.right = rcArrowText.left;
				}
			}
		}
	}
	else if (bSelected)
	{
		// draw right gripper
		CRect rcGripper;
		rcGripper.top = rcEvent.top + rcEvent.Height() / 2 - cnGripperSize / 2;
		rcGripper.bottom = rcGripper.top + cnGripperSize;
		rcGripper.right = rcEvent.right + 1;
		rcGripper.left = rcGripper.right - cnGripperSize;;

		pDC->FillSolidRect(&rcGripper, m_clrGripperBackground);
		CBrush brBorder(m_clrGripperBorder);
		pDC->FrameRect(&rcGripper, &brBorder);
	}

	MultidayDrawTime(nMDEFlags, pViewEvent, pDC, rcEvent, FALSE, FALSE, rcText);

	//=======================================================================
	CString strSubj = pViewEvent->GetItemTextSubject();
	CString strLoc = pViewEvent->GetItemTextLocation();

	if (!strLoc.IsEmpty())
		strSubj += _T(";");

	CThemeFontColorSet* pFntClr1 = bSelected ? &m_fcsetSelected.fcsetSubject : &m_fcsetNormal.fcsetSubject;
	CThemeFontColorSet* pFntClr2 = bSelected ? &m_fcsetSelected.fcsetLocation : &m_fcsetNormal.fcsetLocation;
	//rcText.DeflateRect(3, 2, 3, 2);

	pViewEvent->m_rcSubjectEditor.CopyRect(rcText);
	//pViewEvent->AdjustSubjectEditorEx(FALSE);
	pViewEvent->AdjustSubjectEditor();

	CRect rcIconsMax = rcText;

	//
	BOOL bMarkupDrawn = FALSE;
	if (pThemeX->GetCalendarControl()->IsMarkupEnabled())
	{
		CXTPMarkupContext* pMarkupContext = pThemeX->GetCalendarControl()->GetMarkupContext();
		ASSERT(pMarkupContext);

		CXTPMarkupUIElement* pUIElement = pViewEvent->GetMarkupUIElement();

		if (pUIElement && pMarkupContext)
		{
			CSize szIcons = Draw_Icons(pViewEvent, pDC, rcIconsMax, TRUE);

			if (rcIconsMax.Height() > szIcons.cy)
				rcIconsMax.bottom -= (rcIconsMax.Height() - szIcons.cy) / 2 + 1;

			rcText.right = max(rcText.left,  rcText.right - szIcons.cx - 2);

			XTPMarkupSetDefaultFont(pMarkupContext, (HFONT)pFntClr1->Font()->GetSafeHandle(), pFntClr1->Color());

			XTPMarkupRenderElement(pUIElement, pDC->GetSafeHdc(), rcText);

			bMarkupDrawn = TRUE;
		}
	}

	CRect rcCategoryIcons = rcText;

	//--- calculate rects and fix to be centered if enough space ---
	if (!bMarkupDrawn)
	{
		UINT uTextShortAlign = DT_VCENTER | DT_LEFT;

		CSize szText = pThemeX->DrawLine2_CenterLR(pDC, strSubj, strLoc, pFntClr1, pFntClr2,
									rcText, uTextShortAlign | DT_CALCRECT);

		rcIconsMax.left = min(rcIconsMax.left + rcText.Height(), rcIconsMax.right);

		CSize szIcons = Draw_Icons(pViewEvent, pDC, rcIconsMax, TRUE);

		if (rcIconsMax.Height() > szIcons.cy)
		{
			rcIconsMax.bottom -= (rcIconsMax.Height() - szIcons.cy) / 2 + 1;
		}

		int nTIcx = szText.cx + szIcons.cx;
		if (nTIcx + 5 < rcText.Width())
		{
			int ncx_2 = (rcText.Width() - nTIcx) / 2 - 2;
			rcText.left += ncx_2;
			rcText.right = rcText.left + szText.cx + 2;

			rcIconsMax.right -= ncx_2;
		}
		else
		{
			rcText.right -= szIcons.cx + 2;
		}

		//- Draw
		pThemeX->DrawLine2_CenterLR(pDC, strSubj, strLoc, pFntClr1, pFntClr2,
									rcText, uTextShortAlign);
	}

	Draw_Icons(pViewEvent, pDC, rcIconsMax);


	rcCategoryIcons.left = rcText.right + 1;
	DrawCategoryIcons(pDC, pViewEvent, rcCategoryIcons, rcEvent);


	//BOOL bShowExpandButton = XTP_SAFE_GET2(pViewEvent->GetCalendarControl(), GetCalendarOptions(), bShowAllDayExpandButton, TRUE);

 //<< Feb 10 >>
	//CXTPCalendarDayViewGroup* pDayViewGroup = DYNAMIC_DOWNCAST(CXTPCalendarDayViewGroup, pViewEvent->GetViewGroup_());

	//if (pViewEvent->m_bShowArrow && !pDC->IsPrinting() && bShowExpandButton)
	//{
	//  /*
	//  int wM = rcIconsMax.Width();
	//  rcIconsMax.top = rcIconsMax.bottom;
	//  rcIconsMax.bottom = rcIconsMax.top + 10;
	//  rcIconsMax.left = rcText.right;
	//  rcIconsMax.right = rcIconsMax.left + wM;
	//  pThemeX->DrawExpandDayButton(pDC, rcIconsMax, FALSE, TRUE);
	//  */

	//  if (!pDayViewGroup == NULL)
	//  {
	//      CRect rcIconsMax1 = pDayViewGroup->GetAllDayEventsRect();
	//      rcIconsMax1.top = rcIconsMax1.bottom - 13;
	//      rcIconsMax1.bottom = rcIconsMax1.top + 9;
	//      rcIconsMax1.left = pDayViewGroup->GetAllDayEventsRect().right - 17;
	//      rcIconsMax1.right = rcIconsMax1.left + 14;

	//      pThemeX->DrawExpandDayButton(pDC, rcIconsMax1, FALSE, TRUE);
	//  }

	//}
//<< Feb 10 >>

}

const int CXTPCalendarThemeOffice2007::CTODayViewEvent::nCategoryIconWidth = 6;
const int CXTPCalendarThemeOffice2007::CTODayViewEvent::nCategoryIconOffset = 1;

void CXTPCalendarThemeOffice2007::CTODayViewEvent_MultiDay::DrawCategoryIcons(CDC* pDC, CXTPCalendarViewEvent* pViewEvent, CRect rcCategoryIconsBase, const CRect& rcEvent)
{
	CXTPCalendarEvent* pEvent = pViewEvent->GetEvent();

	if(!NeedDrawCategoryIcons(pEvent))
		return;

	const int nCategoriesCount = pEvent->GetCategoriesCount();
	CRect rcCategoryIcons = AdjustCategoryIconsRectWidth(rcCategoryIconsBase, rcEvent, nCategoriesCount);

	CTODayViewEvent::DrawCategoryIcons(pDC, pEvent, rcCategoryIcons);
}

CRect CXTPCalendarThemeOffice2007::CTODayViewEvent_MultiDay::AdjustCategoryIconsRectWidth(const CRect& rcCategoryIconsBase, const CRect& rcEvent, int nCategoriesCount)
{
	CRect rcCategoryIcons = rcCategoryIconsBase;
	int nCategoryAreaWidth = CalcCategoryAreaWidth(nCategoriesCount);
	if(rcCategoryIcons.Width() < nCategoryAreaWidth)
		rcCategoryIcons.left = max(rcEvent.left, rcCategoryIconsBase.left - (nCategoryAreaWidth - rcCategoryIconsBase.Width()));

	return rcCategoryIcons;
}

void CXTPCalendarThemeOffice2007::CTODayViewEvent::DrawCategoryIcons(CDC* pDC, CXTPCalendarEvent* pEvent, const CRect& rcCategoryIcons)
{
	CXTPCalendarEventCategories* pCategories = XTP_SAFE_GET2(pEvent, GetDataProvider(), GetEventCategories(), NULL);
	if(pCategories == NULL)
		return;

	CXTPCalendarEventCategoryIDs* pCategoryIDs = pEvent->GetCategories();
	if(pCategoryIDs == NULL)
		return;

	CRect rcCurrentIcon = rcCategoryIcons;
	rcCurrentIcon.right = rcCurrentIcon.left + nCategoryIconWidth;

	const int nCategoriesCount = pEvent->GetCategoriesCount();
	for(int nCategoryIdx = 1; nCategoryIdx < nCategoriesCount; nCategoryIdx++)
	{
		if(rcCurrentIcon.right > rcCategoryIcons.right)
			break;

		COLORREF clrCatColor, clrIconBorder;
		if(!GetCategoryIconColors(pCategories, pCategoryIDs, nCategoryIdx, clrCatColor, clrIconBorder))
			continue;

		CBrush iconBrush(clrCatColor);
		CXTPCalendarTheme::DrawRect(pDC, rcCurrentIcon, clrIconBorder, FALSE, &iconBrush);

		rcCurrentIcon.OffsetRect(nCategoryIconOffset + nCategoryIconWidth, 0);
	}
}


BOOL CXTPCalendarThemeOffice2007::CTODayViewEvent::NeedDrawCategoryIcons(CXTPCalendarEvent* pEvent)
{
	if(pEvent == NULL)
		return FALSE;

	BOOL bShowCategoryIcons = XTP_SAFE_GET3(GetTheme(), GetCalendarControl(), GetCalendarOptions(), bShowCategoryIcons, FALSE);
	if(!bShowCategoryIcons)
		return FALSE;

	CXTPCalendarEventCategoryIDs* pCategoryIDs = pEvent->GetCategories();
	if(pCategoryIDs == NULL)
		return FALSE;

	if(pCategoryIDs->GetSize() <= 1)
		return FALSE;

	return TRUE;
}

int CXTPCalendarThemeOffice2007::CTODayViewEvent::CalcCategoryAreaWidth(int nCategoriesCount)
{
	nCategoriesCount--; // one is the main category, we use it for background and we dont need to draw it's icon
	if(nCategoriesCount <= 0)
		return 0;

	return (nCategoriesCount*nCategoryIconWidth + (nCategoriesCount-1)*nCategoryIconOffset);
}

bool CXTPCalendarThemeOffice2007::CTODayViewEvent::GetCategoryIconColors(CXTPCalendarEventCategories* pCategories,
																																									CXTPCalendarEventCategoryIDs* pCategoryIDs,
																																									int nCategoryIdx, COLORREF& clrCatColor, COLORREF& clrIconBorder)
{
	UINT nCatID = pCategoryIDs->GetAt(nCategoryIdx);
	CXTPCalendarEventCategory* pCat = pCategories->Find(nCatID);
	if(pCat == NULL)
		return false;

	clrCatColor = (COLORREF)pCat->GetBackgroundColor();
	clrIconBorder = XTPDrawHelpers()->DarkenColor(120, clrCatColor);

	return true;
}

void CXTPCalendarThemeOffice2007::CTOEvent::FillEventBackgroundEx(CDC* pDC,
							CXTPCalendarViewEvent* pViewEvent, const CRect& rcRect)
{
	if (!pDC || !pViewEvent)
	{
		ASSERT(FALSE);
		return;
	}

	CRect rcEventFull = pViewEvent->GetEventRect();

	COLORREF clrBorderNotSel;
	COLORREF clrBorder;
	CXTPPaintManagerColorGradient grclrBk;

	GetEventColors(pDC, pViewEvent, clrBorder, clrBorderNotSel, grclrBk);

	if (pDC->IsPrinting())
	{
		CXTPCalendarTheme::DrawRectPartly(pDC, rcRect, 0, FALSE, grclrBk, CRect(0, 0, 0, 0));
	}
	else
	{
		XTPDrawHelpers()->GradientFill(pDC, &rcEventFull, grclrBk, FALSE, &rcRect);
	}
}

void CXTPCalendarThemeOffice2007::CTOEvent::GetEventColors(CDC* pDC, CXTPCalendarViewEvent* pViewEvent,
		COLORREF& rClrBorder, COLORREF& rClrBorderNotSel, CXTPPaintManagerColorGradient& rGrclrBk,
		BOOL bSelected, int* pnColorsSrc_1Cat2Lbl)
{
	rClrBorderNotSel = m_fcsetNormal.clrBorder;
	rClrBorder = m_fcsetNormal.clrBorder;
	rGrclrBk = m_fcsetNormal.grclrBackground;

	if (!pViewEvent || !pViewEvent->GetEvent())
	{
		ASSERT(FALSE);
		return;
	}

	if (bSelected == -1)
	{
		bSelected = pViewEvent->IsSelected();
	}

	rClrBorder = bSelected ? m_fcsetSelected.clrBorder : m_fcsetNormal.clrBorder;
	rGrclrBk = bSelected ? m_fcsetSelected.grclrBackground : m_fcsetNormal.grclrBackground;

	CXTPCalendarEvent* pEvent = pViewEvent->GetEvent();

	CXTPCalendarEventCategoryIDs* pCategoryIDs = pEvent->GetCategories();
	CXTPCalendarEventLabel* ptrLabel = pEvent->GetLabel();


	if (pnColorsSrc_1Cat2Lbl)
		*pnColorsSrc_1Cat2Lbl = 0;

	if (pCategoryIDs && pCategoryIDs->GetSize())
	{
		UINT nCatID_0 = pCategoryIDs->GetAt(0);

		CXTPCalendarEventCategories* pCategories = pEvent->GetDataProvider()->GetEventCategories();
		if (pCategories)
		{
			CXTPCalendarEventCategory* pCat = pCategories->Find(nCatID_0);
			if (pCat)
			{
				rGrclrBk.SetCustomValue(pCat->GetBackgroundColor());

				if (XTPDrawHelpers()->IsLowResolution(pDC->GetSafeHdc()))
					rGrclrBk.SetCustomValue(pCat->GetBackgroundColor().clrDark);

				rClrBorderNotSel = pCat->GetBorderColor();
				if (!bSelected)
					rClrBorder = rClrBorderNotSel;

				if (pnColorsSrc_1Cat2Lbl)
					*pnColorsSrc_1Cat2Lbl = 1;
			}
		}
	}
	else if (ptrLabel && ptrLabel->m_nLabelID != XTP_CALENDAR_NONE_LABEL_ID)
	{
		COLORREF crBkLight = XTPDrawHelpers()->LightenColor(120, ptrLabel->m_clrColor);
		COLORREF crBkDark = XTPDrawHelpers()->DarkenColor(20, ptrLabel->m_clrColor);

		if (XTPDrawHelpers()->IsLowResolution(pDC->GetSafeHdc()))
			crBkLight = crBkDark = ptrLabel->m_clrColor;

		rGrclrBk.SetCustomValue(crBkLight, crBkDark);

		rClrBorderNotSel = XTPDrawHelpers()->DarkenColor(88, ptrLabel->m_clrColor);
		if (!bSelected)
			rClrBorder = rClrBorderNotSel;

		if (pnColorsSrc_1Cat2Lbl)
			*pnColorsSrc_1Cat2Lbl = 2;
	}
}

void CXTPCalendarThemeOffice2007::CTODayViewEvent_SingleDay::Draw(CCmdTarget* pObject, CDC* pDC)
{
	CXTPCalendarDayViewEvent* pViewEvent = DYNAMIC_DOWNCAST(CXTPCalendarDayViewEvent, pObject);
	CXTPCalendarThemeOffice2007* pThemeX = DYNAMIC_DOWNCAST(CXTPCalendarThemeOffice2007, GetTheme());

	if (!pDC || !pThemeX || !pViewEvent || !pViewEvent->GetEvent() ||
		!pViewEvent->GetViewGroup() || !pThemeX->GetCalendarControl())
	{
		ASSERT(FALSE);
		return;
	}

	BOOL bNoVisibleMDEvent = pViewEvent->IsMultidayEvent() && !pViewEvent->IsVisible();

	int nMDEFlags = pViewEvent->GetMultiDayEventFlags();
	if ((nMDEFlags & xtpCalendarMultiDaySlave) || bNoVisibleMDEvent)
	{
		return;// was drawn in the Master (or out of events rect)
	}

	CRect rcEvent(pViewEvent->GetEventRect());
	CRect rcTimeframe(pViewEvent->m_rcTimeframe);

	if (rcEvent.IsRectEmpty())
	{
		return;
	}

	// ----- Fill event background -----
	int nTopRow = pViewEvent->GetViewGroup()->GetTopRow();
	CRect rcTopRow = pViewEvent->GetViewGroup()->GetCellRect(nTopRow);

	BOOL bShortEvent = rcTimeframe.Height() + 1 < rcTopRow.Height();
	BOOL bSelected = pViewEvent->IsSelected();

	COLORREF clrBorderNotSel;
	COLORREF clrBorder;
	CXTPPaintManagerColorGradient grclrBk;

	GetEventColors(pDC, pViewEvent, clrBorder, clrBorderNotSel, grclrBk);

	// --- Calculate event rectangle ---
	CRect rcEventX(rcEvent);
	if (bShortEvent)
		rcEventX.left += 5;

	// ------ Draw Event Border ------
	pThemeX->DrawEventRect(pDC, rcEventX, clrBorder, bSelected, grclrBk);

	// -------- Draw Event Contents --------
	CRect rcText(rcEventX);
	rcText.left += 2;

	int nSelOffset = bSelected ? 2 : 1;

	int nBusyStatus = pViewEvent->GetEvent()->GetBusyStatus();
	CBrush* pBSBrush = GetBusyStatusBrush(nBusyStatus);

	if (!bShortEvent)
	{
		LONG lShowAsBlockWidth = 0;
		if (DrawShowAsStatus(pDC, pViewEvent, pThemeX, rcEvent, lShowAsBlockWidth))
		{
			rcText.left = rcEvent.left + lShowAsBlockWidth;
			pThemeX->_DrawEventLTHighlight(pDC, CPoint(rcText.left, rcEvent.top + nSelOffset), rcEvent.right - nSelOffset, grclrBk);
		}
		else
		{
			pThemeX->_DrawEventLTHighlight(pDC, CPoint(rcEvent.left + nSelOffset, rcEvent.top + nSelOffset),
				rcEvent.right - nSelOffset, grclrBk, cnCornerSize / 2, rcEvent.bottom);
		}
	}
	else
	{
		CRect rcBS = rcEvent;
		rcBS.right = rcEventX.left + 6;

		CRect rcBSfull = rcBS;
		rcBS.bottom = rcTimeframe.bottom;

		// draw BK's
		if (pDC->IsPrinting())
		{
			CXTPCalendarTheme::DrawRectPartly(pDC, rcBS, 0, FALSE, grclrBk, CRect(0, 0, 0, 0));
		}
		else
		{
			XTPDrawHelpers()->GradientFill(pDC, &rcBSfull, grclrBk, FALSE, &rcBS);
		}

		if (pBSBrush)
		{
			CRect rcBSfill = rcBS;
			rcBSfill.right = rcEventX.left;

			CXTPTextDC autoText(pDC, clrBorderNotSel, RGB(255, 255, 255));
			pDC->FillRect(&rcBSfill, pBSBrush);
		}

		// draw borders
		int nBorderWidth = bSelected ? 2 : 1;
		CXTPCalendarTheme::DrawRectPartly(pDC, rcBS, clrBorder, nBorderWidth, CRect(1, 1, 0, 0));

		rcBS.right = rcEventX.left + 1;
		rcBS.top += nBorderWidth;
		if (pBSBrush)
		{
			CXTPCalendarTheme::DrawRectPartly(pDC, rcBS, clrBorderNotSel, 1, CRect(0, 0, 1, 0));
		}

		rcBS.right += bSelected ? 1 : 0;
		CXTPCalendarTheme::DrawRectPartly(pDC, rcBS, clrBorder, nBorderWidth, CRect(0, 0, 0, 1));

		rcText.left = rcEventX.left;

		if (pBSBrush)
		{
			pThemeX->_DrawEventLTHighlight(pDC, CPoint(rcEventX.left + 1, rcEvent.top + nSelOffset),
				rcEvent.right - nSelOffset, grclrBk);
		}
		else
		{
			pThemeX->_DrawEventLTHighlight(pDC, CPoint(rcEvent.left + nSelOffset, rcEvent.top + nSelOffset),
				rcEvent.right - nSelOffset, grclrBk, 0, rcTimeframe.bottom - nSelOffset);
		}
	}

	// ----- Draw Grippers for selected events -----
	if (bSelected)
	{
		CBrush brBorder(m_clrGripperBorder);
		CRect rcGripper;

		rcGripper.left = rcEvent.left + rcEvent.Width() / 2 - cnGripperSize / 2;
		rcGripper.right = rcGripper.left + cnGripperSize;

		// draw top Gripper
		rcGripper.top = rcEvent.top - 1; //cnGripperSize / 2 + 1;
		rcGripper.bottom = rcGripper.top + cnGripperSize;

		pDC->FillSolidRect(&rcGripper, m_clrGripperBackground);
		pDC->FrameRect(&rcGripper, &brBorder);

		// draw bottom Gripper
		rcGripper.bottom = rcEvent.bottom + 1; //cnGripperSize / 2 - 1;
		rcGripper.top = rcGripper.bottom - cnGripperSize;


		pDC->FillSolidRect(&rcGripper, m_clrGripperBackground);
		pDC->FrameRect(&rcGripper, &brBorder);
	}

	// ===== Process Markup Drawing =====
	rcText.DeflateRect(3, 3, 3, 3);

	CThemeFontColorSet* pFntClr1 = bSelected ? &m_fcsetSelected.fcsetSubject  : &m_fcsetNormal.fcsetSubject;

	BOOL bMarkupDrawn = FALSE;
	if (pThemeX->GetCalendarControl()->IsMarkupEnabled())
	{
		CXTPMarkupContext* pMarkupContext = pThemeX->GetCalendarControl()->GetMarkupContext();
		ASSERT(pMarkupContext);

		CXTPMarkupUIElement* pUIElement = pViewEvent->GetMarkupUIElement();

		if (pUIElement && pMarkupContext)
		{
			XTPMarkupSetDefaultFont(pMarkupContext, (HFONT)pFntClr1->Font()->GetSafeHandle(), pFntClr1->Color());

			XTPMarkupRenderElement(pUIElement, pDC->GetSafeHdc(), rcText);

			bMarkupDrawn = TRUE;
		}
	}

	///////////////////////////////

	// ----- Draw event Icons -----
	CRect rcIconsMax = rcText;

	CSize szIcons = Draw_Icons(pViewEvent, pDC, rcIconsMax, TRUE);

	if (rcIconsMax.Height() >= rcTopRow.Height())
	{
		rcIconsMax.bottom -= 4;
	}
	else
	{
		if (rcTopRow.Height() - 4 > szIcons.cy)
		{
			rcIconsMax.bottom = rcIconsMax.top + rcIconsMax.Height()/2 + szIcons.cy/2;
		}

		rcIconsMax.left = min(rcIconsMax.left + rcTopRow.Height(), rcIconsMax.right);
		rcText.right = max(rcText.right - szIcons.cx - 2, rcText.left + rcTopRow.Height() - 2);
	}

	pViewEvent->m_rcSubjectEditor.CopyRect(rcText);
	//pViewEvent->AdjustSubjectEditorEx(FALSE);
	pViewEvent->AdjustSubjectEditor();

	if (!bMarkupDrawn)
	{
		CString strSubj = pViewEvent->GetItemTextSubject();
		CString strLoc = pViewEvent->GetItemTextLocation();
		CString strBody = pViewEvent->GetItemTextBody();

//test case <<
		//if (strBody == _T("_TEST_RTF_CASE_"))
		//  strBody = _T("{\\rtf1\\ansi\\ansicpg1251\\deff0\\deflang1049{\\fonttbl{\\f0\\fswiss\\fcharset0 Arial;}{\\f1\\fswiss\\fcharset204{\\*\\fname Arial;}Arial CYR;}}{\\colortbl ;\\red255\\green0\\blue0;}{\\*\\generator Msftedit 5.41.15.1512;}\\viewkind4\\uc1\\pard\\cf1\\lang1033\\ul\\f0\\fs20 Test \\cf0\\i RTF\\i0  \\b body\\lang1049\\b0\\f1\\par}");
//test case >>

		if (strBody.Find(_T("{\\rtf1\\")) == 0)
			strBody.Empty();

		CThemeFontColorSet* pFntClr2 = bSelected ? &m_fcsetSelected.fcsetLocation : &m_fcsetNormal.fcsetLocation;
		CThemeFontColorSet* pFntClr3 = bSelected ? &m_fcsetSelected.fcsetBody     : &m_fcsetNormal.fcsetBody;

		UINT uWordBreakMode = DT_WORDBREAK;
		DWORD dwOpt = XTP_SAFE_GET3(GetTheme(), GetCalendarControl(), GetCalendarOptions(), dwAdditionalOptions, 0);
		if (dwOpt & xtpCalendarOptDayViewNoWordBreak)
			uWordBreakMode = DT_WORD_ELLIPSIS;

		rcText.right = rcEventX.right - 5; //<< 2010 >>

		pThemeX->DrawText_Auto2SL3ML(pDC, strSubj, strLoc, strBody,
			pFntClr1, pFntClr2, pFntClr3, rcText, _T(";"), uWordBreakMode);
	}

	Draw_Icons(pViewEvent, pDC, rcIconsMax);

	//CBrush iconBrush(RGB(0,255,0));
	//CRect rcCatIcon = rcText;
	//rcCatIcon.left = rcCatIcon.right - 6;
	//rcCatIcon.top = rcCatIcon.bottom - (rcTopRow.Height() - 6);
	////CXTPCalendarTheme::DrawRect(pDC, rcEventX, RGB(255,0,0), FALSE, &iconBrush);
	////CXTPCalendarTheme::DrawRect(pDC, rcTimeframe, RGB(255,0,0), FALSE, &iconBrush);
	////CXTPCalendarTheme::DrawRect(pDC, rcEvent, RGB(255,0,0), FALSE, &iconBrush);
	//
	//CXTPCalendarTheme::DrawRect(pDC, rcCatIcon, RGB(255,0,0), FALSE, &iconBrush);

	DrawCategoryIcons(pDC, pViewEvent, rcText, rcTopRow);

}

void CXTPCalendarThemeOffice2007::CTODayViewEvent_SingleDay::DrawCategoryIcons(CDC* pDC, CXTPCalendarViewEvent* pViewEvent, const CRect& rcText, const CRect& rcTopRow)
{
	CXTPCalendarEvent* pEvent = pViewEvent->GetEvent();

	if(!NeedDrawCategoryIcons(pEvent))
		return;

  const int nCategoriesCount = pEvent->GetCategoriesCount();
	if(nCategoriesCount == 0)
		return;

	CRect rcCategoryIcons = CalcCategoryIconsRect(nCategoriesCount, rcText, rcTopRow);
	CTODayViewEvent::DrawCategoryIcons(pDC, pEvent, rcCategoryIcons);
}

CRect CXTPCalendarThemeOffice2007::CTODayViewEvent_SingleDay::CalcCategoryIconsRect(int nCategoriesCount, const CRect& rcText, const CRect& rcTopRow)
{
	CRect rcCategoryIcons = rcText;
	rcCategoryIcons.top = rcCategoryIcons.bottom - min(20, (rcTopRow.Height() - 6));
	rcCategoryIcons.left = max(rcCategoryIcons.left, rcCategoryIcons.right - CalcCategoryAreaWidth(nCategoriesCount));

	return rcCategoryIcons;
}

void CXTPCalendarThemeOffice2007::CTODayViewEvent_SingleDay::DrawShadow(CDC* pDC, CXTPCalendarDayViewEvent* pViewEvent)
{
	ASSERT(pDC && pViewEvent);
	if (!pDC || pDC->IsPrinting() || !pViewEvent)
		return;

	CRect rcEvent(pViewEvent->GetEventRect());
	CRect rcTimeframe(pViewEvent->m_rcTimeframe);

	if (rcEvent.IsRectEmpty())
	{
		return;
	}

	CXTPCalendarThemeOffice2007* pThemeX = DYNAMIC_DOWNCAST(CXTPCalendarThemeOffice2007, GetTheme());
	if (pThemeX && pThemeX->m_bFlatStyle)
	{
		return;
	}

	// ----- Fill event background -----
	int nTopRow = pViewEvent->GetViewGroup()->GetTopRow();
	CRect rcTopRow = pViewEvent->GetViewGroup()->GetCellRect(nTopRow);

	BOOL bShortEvent = rcTimeframe.Height() + 1 < rcTopRow.Height();
	// --- Calculate event rectangle ---
	CRect rcRect(rcEvent);
	if (bShortEvent)
		rcRect.left += 5;

	CXTPCalendarShadowDrawer oShadow(pDC, dXTPMaxAlpha2007);

	CRect rcVShadow(rcRect);
	rcVShadow.left = rcVShadow.right;
	rcVShadow.right += 5;
	rcVShadow.top += 5*2 + 1;
	oShadow.DrawShadowRectPart(rcVShadow, oShadow.xtpShadow2007Right);

	CRect rcHShadow(rcRect);
	rcHShadow.left += 5*2;
	rcHShadow.top = rcHShadow.bottom;
	rcHShadow.bottom += 5;
	oShadow.DrawShadowRectPart(rcHShadow, oShadow.xtpShadow2007Bottom);

	CRect rcTRShadow(rcVShadow);
	rcTRShadow.bottom = rcTRShadow.top;
	rcTRShadow.top -= 6;
	oShadow.DrawShadowRectPart(rcTRShadow, oShadow.xtpShadow2007TopRight);

	CRect rcBLShadow(rcHShadow);
	rcBLShadow.right = rcBLShadow.left;
	rcBLShadow.left -= 5;
	oShadow.DrawShadowRectPart(rcBLShadow, oShadow.xtpShadow2007BottomLeft);

	CRect rcBRShadow(rcVShadow.left - 3, rcHShadow.top - 3, rcVShadow.right, rcHShadow.bottom);
	oShadow.DrawShadowRectPart(rcBRShadow, oShadow.xtpShadow2007BottomRight);

}

void CXTPCalendarThemeOffice2007::_DrawEventLTHighlight(CDC* pDC, CPoint ptLT, int nRTx, const CXTPPaintManagerColorGradient& grclrBk,
									int nLTCorner, int nLBy, BOOL bRCorner)
{
	if (m_bFlatStyle)
		return;

	COLORREF clrTop = XTPDrawHelpers()->LightenColor(120, grclrBk.clrLight);

	CRect rcTop(ptLT.x + nLTCorner, ptLT.y, nRTx, ptLT.y + 1);
	if (bRCorner)
		rcTop.right -= cnCornerSize / 2 + 1;

	pDC->FillSolidRect(&rcTop, clrTop);

	if (nLBy != INT_MAX)
	{
		CRect rcLLVert(ptLT.x, ptLT.y + nLTCorner, ptLT.x + 1, nLBy - nLTCorner);
		rcLLVert.top += nLTCorner ? 0 : 1;

		COLORREF clrLight = XTPDrawHelpers()->LightenColor(76, grclrBk.clrLight);
		COLORREF clrDrak = XTPDrawHelpers()->LightenColor(15, grclrBk.clrDark);

		XTPDrawHelpers()->GradientFill(pDC, rcLLVert, clrLight, clrDrak, FALSE);

		CPen pen(PS_SOLID, 1, clrLight);
		CXTPPenDC autoPen(pDC, &pen);

		pDC->MoveTo(rcLLVert.TopLeft());
		pDC->LineTo(rcTop.TopLeft());
	}

	if (bRCorner)
	{
		COLORREF clrLight2 = XTPDrawHelpers()->LightenColor(80, grclrBk.clrLight);
		CPen pen(PS_SOLID, 1, clrLight2);
		CXTPPenDC autoPen(pDC, &pen);

		pDC->MoveTo(rcTop.right, rcTop.top);
		pDC->LineTo(rcTop.right + cnCornerSize / 2 + 1, rcTop.top + cnCornerSize / 2 + 1);
	}
}

void CXTPCalendarThemeOffice2007::CTOEvent::InitBusyStatusDefaultColors()
{
	TBase::InitBusyStatusDefaultColors();

	SetBusyStatusBrush(xtpCalendarBusyStatusBusy, NULL, FALSE);

	// Tentative ---------------------------
	m_bmpTentativePattern.DeleteObject();
	m_brushTentative.DeleteObject();

	CXTPResourceManager::AssertValid(XTPResourceManager()->LoadBitmap(&m_bmpTentativePattern, XTP_IDB_CALENDAR_TENATIVE_PATTERN_2007));
	VERIFY(m_brushTentative.CreatePatternBrush(&m_bmpTentativePattern));

	CXTPCalendarViewPartBrushValue* pBVal_Tentative = _GetBusyStatusBrushValue(xtpCalendarBusyStatusTentative, TRUE);
	if (pBVal_Tentative)
	{
		pBVal_Tentative->SetStandardValue(&m_brushTentative, FALSE);
	}
}

void CXTPCalendarThemeOffice2007::CTODayViewEvent_SingleDay::AdjustLayout(CCmdTarget* pObject,
							CDC* pDC, const CRect& rcRect, int nEventPlaceNumber)
{
	UNREFERENCED_PARAMETER(nEventPlaceNumber);

	CXTPCalendarDayViewEvent* pViewEvent = DYNAMIC_DOWNCAST(CXTPCalendarDayViewEvent, pObject);
	CXTPCalendarThemeOffice2007* pThemeX = DYNAMIC_DOWNCAST(CXTPCalendarThemeOffice2007, GetTheme());

	if (!pDC || !pThemeX || !pViewEvent || !pViewEvent->GetEvent() || !pViewEvent->GetViewGroup() ||
		!pViewEvent->GetViewGroup()->GetViewDay())
	{
		ASSERT(FALSE);
		return;
	}

	CXTPCalendarEvent* pEvent = pViewEvent->GetEvent();
	CXTPCalendarDayViewGroup* pViewGroup = pViewEvent->GetViewGroup();
	CXTPCalendarDayViewDay* pViewDay = pViewGroup->GetViewDay();

	pViewEvent->m_nMultiDayEventFlags |= xtpCalendarMultiDayNoMultiDay;

	// non all-day event
	int nPace = pViewEvent->GetEventPlacePos();
	int nPaceCount = max(1, pViewEvent->GetEventPlaceCount());

	ASSERT(pViewEvent->GetEventPlaceCount() >= 1);
	ASSERT(nPace >= 0 && nPace < nPaceCount);

	pViewEvent->m_rcEvent.CopyRect(rcRect);
	int nEventsAreaX = pViewEvent->m_rcEvent.Width() - 5;
	int nEventWidth = nEventsAreaX / nPaceCount;

	// skip event views if they are not fit even by 5 pixel bar width
	// and correct some first event views heights
	if (nEventWidth <= 5)
	{
		while (nPaceCount > 1 && nEventWidth <= 5)
		{
			nPaceCount--;
			nEventWidth = nEventsAreaX / nPaceCount;
		}
		if (nPace > nPaceCount)
		{
			pViewEvent->m_rcEvent.SetRectEmpty();
			return;
		}
	}

	// calc other rect coordinates
	pViewEvent->m_rcEvent.left += nEventWidth * nPace;
	pViewEvent->m_rcEvent.right = pViewEvent->m_rcEvent.left + nEventWidth;

	COleDateTime dtStartTime(pEvent->GetStartTime());
	COleDateTime dtEndTime(pEvent->GetEndTime());

	int nTopRow = pViewGroup->GetTopRow();

	CRect rcTopRow = pViewGroup->GetCellRect(nTopRow);
	CRect rcRow0 = pViewGroup->GetCellRect(0);
	CRect rcRowLast = pViewGroup->GetCellRect(pViewGroup->GetRowCount()-1);

	COleDateTime dtTopRowTime = pViewDay->GetView()->GetCellTime(nTopRow);
	dtTopRowTime = (double)pViewDay->GetDayDate() + (double)dtTopRowTime;

	double dblCellDur = (double)pViewDay->GetView()->GetCellDuration();
	if (dblCellDur <= XTP_HALF_SECOND)
	{
		dblCellDur = XTP_HALF_SECOND * 2 * 60 * 30;
	}

	double dblEventOffsetTime1 = double(dtStartTime - dtTopRowTime);
	double dblEventOffsetTime2 = double(dtEndTime - dtTopRowTime);
	double dblPixelPerTime = rcTopRow.Height() / dblCellDur;

	int nEventY1 = (int)floor(dblPixelPerTime * dblEventOffsetTime1 + 0.5); // this is a correct rounding
	nEventY1 = max(rcRow0.top,  rcTopRow.top + nEventY1);
	//nEventY1 = min(rcRowLast.top, nEventY1); // commented for issue #28209: causes display of item out of selected print range

	int nEventY2 = (int)floor(dblPixelPerTime * dblEventOffsetTime2 + 0.5); // this is a correct rounding
	nEventY2 = rcTopRow.top + nEventY2;
	//nEventY2 = min(rcRowLast.bottom, nEventY2); // commented for issue #28209: causes display of item out of selected print range

	int nEvent_dy = nEventY2 - nEventY1;

	BOOL bShortEvent = nEvent_dy + 1 < rcTopRow.Height();
	nEvent_dy = max(5, nEvent_dy);

	if (bShortEvent)
	{
		nEventY2 = nEventY1 + rcTopRow.Height();
	}

	///%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	pViewEvent->m_rcEvent.top = nEventY1;
	pViewEvent->m_rcEvent.bottom = nEventY2;

	pViewEvent->m_rcEventMax.top = max(pViewEvent->m_rcEvent.top-5, rcRect.top);
	pViewEvent->m_rcEventMax.bottom = min(pViewEvent->m_rcEvent.bottom + 5, rcRect.bottom);

	// adjust subject area
	pViewEvent->m_rcText.CopyRect(pViewEvent->m_rcEvent);

	pViewEvent->m_rcText.left += 10; // correct to busy area
	pViewEvent->m_rcText.left += 4;

	pViewEvent->m_rcSubjectEditor.CopyRect(pViewEvent->m_rcText);

	// Set time frame rect
	pViewEvent->m_rcTimeframe.top = pViewEvent->m_rcEvent.top;
	pViewEvent->m_rcTimeframe.bottom = pViewEvent->m_rcTimeframe.top + nEvent_dy;

	pViewEvent->m_rcTimeframe.left = pViewEvent->m_rcEvent.left + 1;
	pViewEvent->m_rcTimeframe.right = pViewEvent->m_rcTimeframe.left + 4;

	// set expanded signs for day view
	CRect rcEvents = rcRect;

	if (pViewEvent->m_rcEvent.top >= rcEvents.bottom-10)
		pViewEvent->GetViewGroup()->SetExpandDown();

	if (pViewEvent->m_rcEvent.bottom <= rcEvents.top + 10)
		pViewEvent->GetViewGroup()->SetExpandUp();
}

/////////////////////////////////////////////////////////////////////////////
//
// ***** Month View *****
//

void CXTPCalendarThemeOffice2007::CTOMonthView::RefreshMetrics(BOOL bRefreshChildren)
{
	TBase::RefreshMetrics(bRefreshChildren);
}

void CXTPCalendarThemeOffice2007::CTOMonthViewEvent::RefreshMetrics(BOOL bRefreshChildren)
{
	CXTPCalendarThemeOffice2007* pThemeX = DYNAMIC_DOWNCAST(CXTPCalendarThemeOffice2007, GetTheme());
	ASSERT(pThemeX);
	if (!pThemeX)
	{
		return;
	}

	TBase::RefreshMetrics(bRefreshChildren);

	RefreshFromParent(pThemeX->GetEventPartX());

	XTP_SAFE_CALL1(GetHeightFormulaPart(), SetStandardValue(14, 10, 1));
	XTP_SAFE_CALL1(GetEventIconsToDrawPart(), SetStandardValue(FALSE, FALSE, FALSE, FALSE, FALSE) );
}

void CXTPCalendarThemeOffice2007::CTOMonthViewEvent_MultiDay::RefreshMetrics(BOOL bRefreshChildren)
{
	UNREFERENCED_PARAMETER(bRefreshChildren);

	CXTPCalendarThemeOffice2007* pThemeX = DYNAMIC_DOWNCAST(CXTPCalendarThemeOffice2007, GetTheme());
	ASSERT(pThemeX);
	if (!pThemeX)
	{
		return;
	}

	RefreshFromParent(pThemeX->GetMonthViewPartX()->GetEventPartX());

	m_fcsetNormal.clrBorder.SetStandardValue(pThemeX->GetColor(xtpCLR_MultiDayEventBorder));
	m_fcsetNormal.grclrBackground.SetStandardValue(pThemeX->GetColor(xtpCLR_MultiDayEventBkGRfrom),
		pThemeX->GetColor(xtpCLR_MultiDayEventBkGRto)   );

	m_fcsetSelected.clrBorder.SetStandardValue(pThemeX->GetColor(xtpCLR_MultiDayEventSelectedBorder));
	m_fcsetSelected.grclrBackground.SetStandardValue(m_fcsetNormal.grclrBackground);

	m_fcsetNormal.fcsetStartEnd.fntFont.SetStandardValue(pThemeX->m_fntBaseFont);
	m_fcsetNormal.fcsetStartEnd.clrColor.SetStandardValue(pThemeX->GetColor(xtpCLR_MultiDayEventFromToDates));

	m_fcsetSelected.fcsetStartEnd.fntFont.SetStandardValue(pThemeX->m_fntBaseFont);
	m_fcsetSelected.fcsetStartEnd.clrColor.SetStandardValue(pThemeX->GetColor(xtpCLR_MultiDayEventFromToDates));

	InitBusyStatusDefaultColors();
}

void CXTPCalendarThemeOffice2007::CTOMonthViewEvent_SingleDay::RefreshMetrics(BOOL bRefreshChildren)
{
	UNREFERENCED_PARAMETER(bRefreshChildren);

	CXTPCalendarThemeOffice2007* pThemeX = DYNAMIC_DOWNCAST(CXTPCalendarThemeOffice2007, GetTheme());
	ASSERT(pThemeX);
	if (!pThemeX)
	{
		return;
	}

	RefreshFromParent(pThemeX->GetMonthViewPartX()->GetEventPartX());

	CFont* pFontDef = pThemeX->m_fntBaseFont;

	m_fcsetNormal.clrBorder.SetStandardValue(pThemeX->GetColor(xtpCLR_MonthViewSingleDayEventBorder));
	m_fcsetNormal.grclrBackground.SetStandardValue(pThemeX->GetColor(xtpCLR_MonthViewSingleDayEventBkGRfrom),
		pThemeX->GetColor(xtpCLR_MonthViewSingleDayEventBkGRto) );

	m_fcsetSelected.clrBorder.SetStandardValue(pThemeX->GetColor(xtpCLR_MonthViewSingleDayEventSelectedBorder));
	m_fcsetSelected.grclrBackground.SetStandardValue(pThemeX->GetColor(xtpCLR_MonthViewSingleDayEventBkGRfrom),
		pThemeX->GetColor(xtpCLR_MonthViewSingleDayEventBkGRto) );

	pThemeX->RemoveBoldAttrFromStd(m_fcsetNormal.fcsetSubject.fntFont);
	pThemeX->RemoveBoldAttrFromStd(m_fcsetSelected.fcsetSubject.fntFont);

	m_fcsetNormal.fcsetStartEnd.clrColor.SetStandardValue(pThemeX->GetColor(xtpCLR_MonthViewEventTime));
	m_fcsetNormal.fcsetStartEnd.fntFont.SetStandardValue(pFontDef);

	m_fcsetSelected.fcsetStartEnd.clrColor.SetStandardValue(pThemeX->GetColor(xtpCLR_MonthViewEventTime));
	m_fcsetSelected.fcsetStartEnd.fntFont.SetStandardValue(pFontDef);

	InitBusyStatusDefaultColors();
}

int CXTPCalendarThemeOffice2007::CTOMonthViewWeekHeader::CalcWidth(CDC* pDC)
{
	return CalcHeight(pDC, -1);
}

void CXTPCalendarThemeOffice2007::CTOMonthView::AdjustLayout(CDC* pDC, const CRect& rcRect, BOOL bCallPostAdjustLayout)
{
	CXTPCalendarMonthView* pMonthView = DYNAMIC_DOWNCAST(CXTPCalendarMonthView,
		XTP_SAFE_GET2(GetTheme(), GetCalendarControl(), GetMonthView(), NULL));

	CXTPCalendarThemeOffice2007* pThemeX = DYNAMIC_DOWNCAST(CXTPCalendarThemeOffice2007, GetTheme());

	if (!pDC || !pThemeX || !pMonthView || !pMonthView->GetGrid())
	{
		ASSERT(FALSE);
		return;
	}

	int nWHeaderWidth = GetWeekHeaderPartX()->CalcWidth(pDC);

	CRect rcRectWHeader = rcRect;
	rcRectWHeader.right = min(rcRectWHeader.left + nWHeaderWidth, rcRect.right);

	GetWeekHeaderPartX()->AdjustLayout(pMonthView, pDC, rcRectWHeader);

	CRect rcRectGrid = rcRect;
	rcRectGrid.left = rcRectWHeader.right;

	TBase::AdjustLayout(pDC, rcRectGrid, bCallPostAdjustLayout);

	if (pThemeX->GetPrevNextEventButtonsPartX())
		pThemeX->GetPrevNextEventButtonsPartX()->AdjustLayout(NULL, pDC, rcRectGrid);
}

void CXTPCalendarThemeOffice2007::CTOMonthViewEvent::AdjustLayout(CCmdTarget* pObject,
		CDC* pDC, const CRect& rcRect, int nEventPlaceNumber)
{
	CXTPCalendarMonthViewEvent* pViewEvent = DYNAMIC_DOWNCAST(CXTPCalendarMonthViewEvent, pObject);

	if (!pDC || !GetTheme() || !pViewEvent || !pViewEvent->GetEvent())
	{
		ASSERT(FALSE);
		return;
	}

	int nEventHeight = XTP_SAFE_GET4(pViewEvent, GetViewGroup_(), GetViewDay_(), GetView_(), GetRowHeight(), 0);

	pViewEvent->m_rcEventMax.top = rcRect.top + nEventPlaceNumber * nEventHeight;
	pViewEvent->m_rcEventMax.bottom = pViewEvent->m_rcEventMax.top + nEventHeight;

	pViewEvent->m_rcEvent.top = pViewEvent->m_rcEventMax.top;
	pViewEvent->m_rcEvent.bottom = pViewEvent->m_rcEventMax.bottom;
	pViewEvent->m_rcEvent.left = pViewEvent->m_rcEventMax.left + 4 + 2;
	pViewEvent->m_rcEvent.right = pViewEvent->m_rcEventMax.right - 5 - 2;

	pViewEvent->m_rcToolTip.CopyRect(pViewEvent->m_rcEventMax);

	// adjust subject area
	int nDyx = pViewEvent->IsSelected() ? 5 : 2;

	pViewEvent->m_rcText.CopyRect(pViewEvent->m_rcEvent);
	pViewEvent->m_rcText.DeflateRect(nDyx + 3, 3, nDyx + 2, 3);

	// set times rects
	pViewEvent->m_rcStartTime.SetRectEmpty();
	pViewEvent->m_rcEndTime.SetRectEmpty();

	pViewEvent->m_rcSubjectEditor.CopyRect(pViewEvent->m_rcEvent);
	pViewEvent->m_rcSubjectEditor.top++;
	pViewEvent->m_rcSubjectEditor.bottom--;
}

void CXTPCalendarThemeOffice2007::CTOMonthView::Draw(CDC* pDC)
{
	CXTPCalendarThemeOffice2007* pThemeX = DYNAMIC_DOWNCAST(CXTPCalendarThemeOffice2007, GetTheme());
	ASSERT(pThemeX);

	CXTPCalendarMonthView* pMonthView = DYNAMIC_DOWNCAST(CXTPCalendarMonthView,
		XTP_SAFE_GET2(GetTheme(), GetCalendarControl(), GetMonthView(), NULL));

	if (!pDC || !pMonthView || !pThemeX)
	{
		ASSERT(FALSE);
		return;
	}

	pDC->SetBkMode(TRANSPARENT);

	GetWeekHeaderPartX()->Draw(pMonthView, pDC);

	//TBase::Draw(pDC);

	GetWeekDayHeaderPart()->Draw(pMonthView, pDC);

	//CXTPPropsStateContext autoStateCnt1_day(GetDayPart(), xtpCalendarBeforeDraw_MonthViewDay, TRUE);
	CXTPPropsStateContext autoStateCnt1_day(GetDayPart(), 0, TRUE);

	//-- Draw Days with events ------------------------------
	int nCount = pMonthView->GetViewDayCount();
	int i;

	for (i = nCount - 1; i >= 0; i--)
	{
		CXTPCalendarMonthViewDay* pViewDay = pMonthView->GetViewDay(i);
		ASSERT(pViewDay);
		if (pViewDay)
		{
			GetDayPartX()->SetDynColors(pViewDay);

			autoStateCnt1_day.SendBeforeDrawThemeObject((LPARAM)pViewDay, xtpCalendarBeforeDraw_MonthViewDay);

			GetDayPart()->Draw(pViewDay, pDC);

			autoStateCnt1_day.RestoreState();
		}
	}
	autoStateCnt1_day.Clear();

	if (pThemeX->GetPrevNextEventButtonsPartX())
		pThemeX->GetPrevNextEventButtonsPartX()->Draw(NULL, pDC);

	// All saved data must be freed after draw!
	ASSERT(CXTPPropsState::s_dbg_nActiveStates == 0);
}

BOOL CXTPCalendarThemeOffice2007::CTOMonthView::OnLButtonDown(CCmdTarget* pObject, UINT nFlags, CPoint point)
{
	return GetWeekHeaderPart()->OnLButtonDown(pObject, nFlags, point);
}

void CXTPCalendarThemeOffice2007::CTOMonthViewWeekDayHeader::RefreshMetrics(BOOL bRefreshChildren)
{
	UNREFERENCED_PARAMETER(bRefreshChildren);

	CXTPCalendarThemeOffice2007* pThemeX = DYNAMIC_DOWNCAST(CXTPCalendarThemeOffice2007, GetTheme());
	ASSERT(pThemeX);
	if (!pThemeX)
	{
		return;
	}

	RefreshFromParent(pThemeX->GetHeaderPartX());

	m_clrBaseColor.SetStandardValue(pThemeX->GetColor2(_T("MonthViewWeekDayHeaderBk"), RGB(227, 239, 255)));

	m_bkNormal.nBitmapID.       SetStandardValue(0); //xtpIMG_MonthViewWeekDayHeader);
	m_bkSelected.nBitmapID.     SetStandardValue(0); //xtpIMG_MonthViewWeekDayHeader);
	m_bkToday.nBitmapID.        SetStandardValue(0); //xtpIMG_MonthViewWeekDayHeader);
	m_bkTodaySelected.nBitmapID.SetStandardValue(0); //xtpIMG_MonthViewWeekDayHeader);

	m_TextCenter.fcsetNormal.clrColor.SetStandardValue(pThemeX->GetColor2(_T("MonthViewWeekDayHeaderText"), RGB(101, 147, 201)));

	// 0 - Sunday, 1 - Monday, ...; 7 - Sat/Sun
	m_arWeekDayNamesLong.RemoveAll();
	m_arWeekDayNamesShort.RemoveAll();

	m_arWeekDayNamesLong.SetSize(8);
	m_arWeekDayNamesShort.SetSize(8);

	for (int i = 0; i < 7; i++)
	{
		CString sWDlong = CXTPCalendarUtils::GetLocaleString(LOCALE_SDAYNAME1 + i, 100);
		CString sWDshort = CXTPCalendarUtils::GetLocaleString(LOCALE_SABBREVDAYNAME1 + i, 100);

		m_arWeekDayNamesLong [(i + 1) % 7] = sWDlong;
		m_arWeekDayNamesShort[(i + 1) % 7] = sWDshort;
	}

	m_arWeekDayNamesLong [7] = m_arWeekDayNamesLong[6] + _T("/") + m_arWeekDayNamesLong[0];
	m_arWeekDayNamesShort[7] = m_arWeekDayNamesShort[6] + _T("/") + m_arWeekDayNamesShort[0];

}

void CXTPCalendarThemeOffice2007::CTOMonthViewWeekHeader::RefreshMetrics(BOOL bRefreshChildren)
{
	UNREFERENCED_PARAMETER(bRefreshChildren);

	CXTPCalendarThemeOffice2007* pThemeX = DYNAMIC_DOWNCAST(CXTPCalendarThemeOffice2007, GetTheme());
	ASSERT(pThemeX);
	if (!pThemeX)
	{
		return;
	}

	RefreshFromParent(pThemeX->GetHeaderPartX());

	m_clrFreeSpaceBk.SetStandardValue(pThemeX->GetColor2(_T("MonthViewWeekHeaderFreeSpaceBk"), RGB(227, 239, 255)));

	m_bkNormal.nBitmapID.SetStandardValue(xtpIMG_MonthViewWeekHeader);
	m_bkSelected.nBitmapID.SetStandardValue(xtpIMG_MonthViewWeekHeader);
	m_bkToday.nBitmapID.SetStandardValue(xtpIMG_MonthViewWeekHeader);
	m_bkTodaySelected.nBitmapID.SetStandardValue(xtpIMG_MonthViewWeekHeader);

	LOGFONT lfVert;
	VERIFY( m_TextCenter.fcsetNormal.fntFont->GetLogFont(&lfVert) );
	lfVert.lfOrientation = 900;
	lfVert.lfEscapement = 900;
	if (XTPDrawHelpers()->FontExists(XTP_CALENDAR_OFFICE2007_FONT_NAME))
	{
		STRCPY_S(lfVert.lfFaceName, LF_FACESIZE, XTP_CALENDAR_OFFICE2007_FONT_NAME);
	}
	else
	{
		STRCPY_S(lfVert.lfFaceName, LF_FACESIZE, XTPDrawHelpers()->GetVerticalFontName(TRUE));
	}

	m_TextCenter.fcsetNormal.fntFont.SetStandardValue(&lfVert);
	m_TextCenter.fcsetNormal.clrColor.SetStandardValue(RGB(0, 0, 0)); //pThemeX->GetColor(xtpCLR_HeaderBorder));

	XTP_SAFE_CALL1(GetHeightFormulaPart(), SetStandardValue(13, 10, 1));
}

void CXTPCalendarThemeOffice2007::CTOMonthViewWeekDayHeader::AdjustLayout(
					CCmdTarget* pObject, CDC* pDC, const CRect& rcRect)
{
	CXTPCalendarThemeOffice2007* pThemeX = DYNAMIC_DOWNCAST(CXTPCalendarThemeOffice2007, GetTheme());
	CXTPCalendarMonthView* pMonthView = DYNAMIC_DOWNCAST(CXTPCalendarMonthView, pObject);

	if (!pDC || !pThemeX || !pMonthView || !pMonthView->GetGrid())
	{
		ASSERT(FALSE);
		return;
	}
	CXTPCalendarMonthView::CMonthViewGrid* pGrid = pMonthView->GetGrid();
	int nColsCount = pGrid->GetColsCount();
	int nCellWidth = rcRect.Width() / max(1, nColsCount);

	int nHeaderHeight = CalcHeight(pDC, nCellWidth);
	pGrid->SetColHeaderHeight(nHeaderHeight);

	if (!nColsCount || m_arWeekDayNamesLong.GetSize() < 8 ||
		m_arWeekDayNamesShort.GetSize() < 8)
	{
		ASSERT(FALSE);
		return;
	}

	CString strDWMaxLong;
	for (int i = 0; i < 7; i++)
	{
		if (strDWMaxLong.GetLength() < m_arWeekDayNamesLong[i].GetLength())
		{
			strDWMaxLong = m_arWeekDayNamesLong[i];
		}
	}

	CXTPFontDC autoFont(pDC, m_TextCenter.fcsetNormal.Font());

	CSize szWDlong = pDC->GetTextExtent(strDWMaxLong);
	CSize szSaSuLong = pDC->GetTextExtent(m_arWeekDayNamesLong[7]);

	int nRectSize = nCellWidth;

	m_bWeekDayNamesLong = szWDlong.cx + szWDlong.cx/4 + nRectSize/5 < nRectSize;
	m_bWeekDayNameSaSuLong = szSaSuLong.cx + szSaSuLong.cx/4 + nRectSize/5 < nRectSize;
}

void CXTPCalendarThemeOffice2007::CTOMonthViewWeekDayHeader::Draw(CCmdTarget* pObject, CDC* pDC)
{
	CXTPCalendarThemeOffice2007* pThemeX = DYNAMIC_DOWNCAST(CXTPCalendarThemeOffice2007, GetTheme());
	CXTPCalendarMonthView* pMonthView = DYNAMIC_DOWNCAST(CXTPCalendarMonthView, pObject);
	if (!pDC || !pThemeX || !pMonthView || !pMonthView->GetGrid())
	{
		ASSERT(FALSE);
		return;
	}
	CXTPCalendarMonthView::CMonthViewGrid* pGrid = pMonthView->GetGrid();

	CRect rcWDHeader = pMonthView->GetViewRect();
	rcWDHeader.bottom = rcWDHeader.top + pGrid->GetColHeaderHeight();

	CXTPPropsStateContext autoStateCnt1(this, xtpCalendarBeforeDraw_MonthViewWeekDayHeader, TRUE);
	autoStateCnt1.SendBeforeDrawThemeObject((LPARAM)-1);

	//===========================================
	CStringArray arColHeaderText;
	CUIntArray arWeekDays;
	arColHeaderText.SetSize(7);
	arWeekDays.SetSize(7);

	for (int i = 0; i < 7; i++)
	{
		int nCol = pGrid->GetWeekDayCol(i + 1);

		CString sVal = arColHeaderText[nCol];
		if (!sVal.IsEmpty())
		{
			// compressed col - always Sat/Sun
			arColHeaderText[nCol] = m_bWeekDayNameSaSuLong ? m_arWeekDayNamesLong[7] : m_arWeekDayNamesShort[7];
			arWeekDays[nCol] = 7;
		}
		else
		{
			CString sWD = m_bWeekDayNamesLong ? m_arWeekDayNamesLong[i] : m_arWeekDayNamesShort[i];
			arColHeaderText[nCol] = sWD;
			arWeekDays[nCol] = i + 1;
		}
	}

	CXTPPropsStateContext autoStateCnt2_wd(this, xtpCalendarBeforeDraw_MonthViewWeekDayHeader);
	//===========================================
	int nColsCount = pGrid->GetColsCount();
	for (int nCol = 0; nCol < nColsCount; nCol++)
	{
		CRect rcCell = pGrid->GetCelRect(0, nCol, TRUE);
		rcCell.top = rcWDHeader.top;
		rcCell.bottom = rcWDHeader.bottom;

		CString strWD = arColHeaderText[nCol];
		int nWeekDay = arWeekDays[nCol];

		autoStateCnt2_wd.SendBeforeDrawThemeObject((LPARAM)nWeekDay);

		pMonthView->GetWeekDayTextIfNeed(&strWD, nWeekDay);

		Draw_Header(pDC, rcCell, 0, _T(""), strWD);

		if (nCol + 1 < nColsCount)
		{
			COLORREF clrBorder =  pThemeX->GetMonthViewPartX()->GetDayPartX()->m_clrBorder;

			CRect rcLine = rcCell;
			rcLine.left = rcLine.right - 1;
			pDC->FillSolidRect(rcLine, clrBorder);
		}

		autoStateCnt2_wd.RestoreState();
	}
	autoStateCnt2_wd.Clear();
}

void CXTPCalendarThemeOffice2007::CTOMonthViewWeekHeader::AdjustLayout(CCmdTarget* pObject, CDC* pDC, const CRect& rcRect)
{
	CXTPCalendarThemeOffice2007* pThemeX = DYNAMIC_DOWNCAST(CXTPCalendarThemeOffice2007, GetTheme());
	CXTPCalendarMonthView* pMonthView = DYNAMIC_DOWNCAST(CXTPCalendarMonthView, pObject);
	if (!pDC || !pThemeX || !pMonthView || !pMonthView->GetGrid())
	{
		ASSERT(FALSE);
		return;
	}

	//***
	m_rcHeader = rcRect;

	//***
	int nWeeksCount = pMonthView->GetGrid()->GetWeeksCount();
	if (!nWeeksCount)
	{
		return;
	}

	CXTPCalendarMonthViewDay* pViewDay = pMonthView->GetGrid()->GetViewDay(0, 0);
	ASSERT(pViewDay);
	if (!pViewDay)
	{
		return;
	}
	const XTP_VIEW_DAY_LAYOUT& dayLayout = pThemeX->GetViewDayLayout(pViewDay);

	COleDateTime dtWDay1 = pViewDay->GetDayDate();

	CRect rcWHeaderTextVert = m_rcHeader;
	rcWHeaderTextVert.top = dayLayout.m_rcDayHeader.bottom;
	rcWHeaderTextVert.bottom = dayLayout.m_rcDay.bottom;

	//---
	CString strTmp, strCaptionLong;
	FormatWeekCaption(dtWDay1, strTmp, strCaptionLong, 2);

	CXTPFontDC autoFont(pDC, m_TextCenter.fcsetNormal.fntFont, m_TextCenter.fcsetNormal.clrColor);
	CSize szText1 = pDC->GetTextExtent(strCaptionLong);

	int nRectSize = abs(rcWHeaderTextVert.Height());

	m_bDateFormatShort = TRUE;
	if (szText1.cx + szText1.cx/3 + nRectSize/10 < nRectSize)
	{
		m_bDateFormatShort = FALSE;
	}
}

void CXTPCalendarThemeOffice2007::CTOMonthViewWeekDayHeader::Draw_Borders2(
			CDC* /*pDC*/, const CRect& /*rcRect*/, BOOL /*bDrawRightBorder*/)
{

}

void CXTPCalendarThemeOffice2007::CTOMonthViewWeekHeader::DoPropExchange(CXTPPropExchange* pPX)
{
	ASSERT(pPX);
	if (!pPX)
		return;

	CXTPPropExchangeSection secData(pPX->GetSection(_T("MonthViewWeekHeader")));
	if (pPX->IsStoring())
		secData->EmptySection();

	PX_Color(&secData, _T("FreeSpaceBkColor"), m_clrFreeSpaceBk);

	TBase::DoPropExchange(pPX);
}

void CXTPCalendarThemeOffice2007::CTOMonthViewWeekHeader::Serialize(CArchive& ar)
{
	m_clrFreeSpaceBk.Serialize(ar);

	TBase::Serialize(ar);
}

void CXTPCalendarThemeOffice2007::CTOMonthViewWeekHeader::Draw(CCmdTarget* pObject, CDC* pDC)
{
	CXTPCalendarThemeOffice2007* pThemeX = DYNAMIC_DOWNCAST(CXTPCalendarThemeOffice2007, GetTheme());
	CXTPCalendarMonthView* pMonthView = DYNAMIC_DOWNCAST(CXTPCalendarMonthView, pObject);
	if (!pDC || !pThemeX || !pMonthView || !pMonthView->GetGrid())
	{
		ASSERT(FALSE);
		return;
	}

	CXTPPropsStateContext autoStateCnt1(this, xtpCalendarBeforeDraw_MonthViewWeekHeader, TRUE);
	autoStateCnt1.SendBeforeDrawThemeObject((LPARAM)-1);

	pDC->FillSolidRect(&m_rcHeader, m_clrFreeSpaceBk);

	CXTPFontDC autoFont(pDC, m_TextCenter.fcsetNormal.fntFont, m_TextCenter.fcsetNormal.clrColor);

	CXTPPropsStateContext autoStateCnt2_week(this, xtpCalendarBeforeDraw_MonthViewWeekHeader);

	int nWeeksCount = pMonthView->GetGrid()->GetWeeksCount();
	for (int i = 0; i < nWeeksCount; i++)
	{
		CXTPCalendarMonthViewDay* pViewDay = pMonthView->GetGrid()->GetViewDay(i, 0);
		ASSERT(pViewDay);
		if (!pViewDay)
		{
			continue;
		}

		CRect rcWHeader;
		GetWeekHeaderRect(pThemeX, pViewDay, rcWHeader);

		COleDateTime dtWDay1 = pViewDay->GetDayDate();

		autoStateCnt2_week.SendBeforeDrawThemeObject((LPARAM)i);
		CString strWeekCaption;
		FormatWeekCaption(dtWDay1, strWeekCaption, strWeekCaption, m_bDateFormatShort ? 1 : 2);

		Draw_Background(pDC, rcWHeader, 0);

		Draw_TextCenter(pDC, rcWHeader, 0, strWeekCaption);


		if (i +1 < nWeeksCount)
			autoStateCnt2_week.RestoreState();
	}
	autoStateCnt2_week.Clear();
}

BOOL CXTPCalendarThemeOffice2007::CTOMonthViewWeekHeader::OnLButtonDown(CCmdTarget* pObject, UINT nFlags, CPoint point)
{
	UNREFERENCED_PARAMETER(nFlags);

	CXTPCalendarThemeOffice2007* pThemeX = DYNAMIC_DOWNCAST(CXTPCalendarThemeOffice2007, GetTheme());
	CXTPCalendarMonthView* pMonthView = DYNAMIC_DOWNCAST(CXTPCalendarMonthView, pObject);
	if (!pThemeX || !pMonthView || !pMonthView->GetGrid())
	{
		ASSERT(FALSE);
		return FALSE;
	}

	BOOL bHandled = FALSE;
	int nWeeksCount = pMonthView->GetGrid()->GetWeeksCount();
	for (int i = 0; i < nWeeksCount; i++)
	{
		CXTPCalendarMonthViewDay* pViewDay = pMonthView->GetGrid()->GetViewDay(i, 0);

		ASSERT(pViewDay);
		if (!pViewDay)
			continue;

		CRect rcWHeader;
		GetWeekHeaderRect(pThemeX, pViewDay, rcWHeader);

		if(rcWHeader.PtInRect(point))
		{
			COleDateTime dtWDay1 = pViewDay->GetDayDate();
			pMonthView->SetSelection(dtWDay1, dtWDay1);

			SwitchCalendarActiveViewToWeekView(pThemeX->GetCalendarControl());

			bHandled = TRUE;
			break;
		}
	}

	return bHandled;
}

void CXTPCalendarThemeOffice2007::CTOMonthViewWeekHeader::GetWeekHeaderRect(CXTPCalendarThemeOffice2007* pThemeX, CXTPCalendarMonthViewDay* pViewDay, CRect& rcWHeader)
{
	UNREFERENCED_PARAMETER(pThemeX); //To make VS2005 compiler happy
	const XTP_VIEW_DAY_LAYOUT& dayLayout = pThemeX->GetViewDayLayout(pViewDay);

	rcWHeader = m_rcHeader;
	rcWHeader.left+= 3;
	rcWHeader.top = dayLayout.m_rcDayHeader.bottom - 1;
	rcWHeader.bottom = dayLayout.m_rcDay.bottom;
}

void CXTPCalendarThemeOffice2007::CTOMonthViewWeekHeader::SwitchCalendarActiveViewToWeekView(CXTPCalendarControl* pCalendar)
{
	BOOL bMultiColumnWeekMode = pCalendar->m_bMultiColumnWeekMode;
	if (bMultiColumnWeekMode)
		pCalendar->SwitchActiveView(xtpCalendarWorkWeekView);
	else
		pCalendar->SwitchActiveView(xtpCalendarFullWeekView);
	pCalendar->SetFocus();
}

void CXTPCalendarThemeOffice2007::CTOMonthViewWeekHeader::Draw_TextCenter(CDC* pDC, const CRect& rcRect, int nState,
		LPCTSTR pcszCenterText, int* pnWidth)
{
	UNREFERENCED_PARAMETER(nState);
	UNREFERENCED_PARAMETER(pnWidth);

	// Warning: not supported.
	ASSERT(pnWidth == NULL);

	CRect rcWHeaderTextVert = rcRect;
	rcWHeaderTextVert.DeflateRect(1, 3, 1, 3);

	int nTmp = rcWHeaderTextVert.top;
	rcWHeaderTextVert.top = rcWHeaderTextVert.bottom;
	rcWHeaderTextVert.bottom = nTmp;

	int nRectSize = abs(rcWHeaderTextVert.Height());

	//---
	CSize szText = pDC->GetTextExtent(pcszCenterText);

	// Center text vertically
	if (szText.cx + 7 < nRectSize)
	{
		rcWHeaderTextVert.top = max(rcWHeaderTextVert.top - (nRectSize - szText.cx)/2, rcWHeaderTextVert.bottom);
	}

	pDC->DrawText(CString(pcszCenterText), &rcWHeaderTextVert, DT_NOPREFIX | DT_SINGLELINE); //| DT_NOCLIP
}

void CXTPCalendarThemeOffice2007::CTOMonthViewWeekHeader::FormatWeekCaption(COleDateTime dtWDay1,
							CString& rstrShort, CString& rstrLong, int nShort1Long2 )
{
	//LOCALE_IDATE  -  short date format ordering
	// 0 Month-Day-Year
	// 1 Day-Month-Year
	// 2 Year-Month-Day

	int nDateOrdering = CXTPCalendarUtils::GetLocaleLong(LOCALE_IDATE);
	CString strDSeparator = CXTPCalendarUtils::GetLocaleString(LOCALE_SDATE, 100);

	// SHORT
	COleDateTime dtWDay6 = dtWDay1 + COleDateTimeSpan(6, 0, 0, 0);

	const int cnBuffSize = 128;

	TCHAR szWDay1[cnBuffSize], szMonth1[cnBuffSize];
	TCHAR szWDay6[cnBuffSize], szMonth6[cnBuffSize];

	ITOT_S(dtWDay1.GetDay(), szWDay1, cnBuffSize);
	ITOT_S(dtWDay6.GetDay(), szWDay6, cnBuffSize);

	if (nShort1Long2 == 0 || nShort1Long2 == 1)
	{
		ITOT_S(dtWDay1.GetMonth(), szMonth1, cnBuffSize);
		ITOT_S(dtWDay6.GetMonth(), szMonth6, cnBuffSize);

		rstrShort = _FormatWCaption(szWDay1, szMonth1, szWDay6, szMonth6, strDSeparator, nDateOrdering);
	}

	if (nShort1Long2 == 0 || nShort1Long2 == 2)
	{
		CString strMonth1 = CXTPCalendarUtils::GetLocaleString(LOCALE_SABBREVMONTHNAME1 + dtWDay1.GetMonth() - 1, 100);
		CString strMonth6 = CXTPCalendarUtils::GetLocaleString(LOCALE_SABBREVMONTHNAME1 + dtWDay6.GetMonth() - 1, 100);

		rstrLong = _FormatWCaption(szWDay1, strMonth1, szWDay6, strMonth6, _T(" "), nDateOrdering);
	}
}

CString CXTPCalendarThemeOffice2007::CTOMonthViewWeekHeader::_FormatWCaption(LPCTSTR pcszDay1, LPCTSTR pcszMonth1,
						LPCTSTR pcszDay6, LPCTSTR pcszMonth6,
						LPCTSTR pcszDayMonthSeparator, int nDateOrdering)
{
	//LOCALE_IDATE  -  short date format ordering
	// 0 Month-Day-Year
	// 1 Day-Month-Year
	// 2 Year-Month-Day

	CString strCaption;
	if (_tcsicmp(pcszMonth1, pcszMonth6) == 0)
	{
		if (nDateOrdering == 1)
		{
			strCaption.Format(_T("%s - %s%s%s"), pcszDay1, pcszDay6,
				pcszDayMonthSeparator, pcszMonth6);
		}
		else
		{
			strCaption.Format(_T("%s%s%s - %s"), pcszMonth1, pcszDayMonthSeparator,
				pcszDay1, pcszDay6);
		}
	}
	else
	{
		if (nDateOrdering == 1)
		{
			strCaption.Format(_T("%s%s%s - %s%s%s"), pcszDay1, pcszDayMonthSeparator,
				pcszMonth1, pcszDay6, pcszDayMonthSeparator, pcszMonth6);
		}
		else
		{
			strCaption.Format(_T("%s%s%s - %s%s%s"), pcszMonth1, pcszDayMonthSeparator,
				pcszDay1, pcszMonth6, pcszDayMonthSeparator, pcszDay6);
		}
	}
	return strCaption;
}

void CXTPCalendarThemeOffice2007::CTOMonthViewDay::RefreshMetrics(BOOL bRefreshChildren)
{
	CXTPCalendarThemeOffice2007* pThemeX = DYNAMIC_DOWNCAST(CXTPCalendarThemeOffice2007, GetTheme());
	ASSERT(pThemeX);
	if (!pThemeX)
	{
		return;
	}

	m_clrBorder.SetStandardValue(pThemeX->GetColor(xtpCLR_DayBorder));
	//m_clrTodayBorder.SetStandardValue(XTP_CALENDAR_OFFICE2007_TODAYBORDER);

	m_clrBackgroundLight.SetStandardValue(pThemeX->GetColor(xtpCLR_MonthViewDayBkLight));
	m_clrBackgroundDark.SetStandardValue(pThemeX->GetColor(xtpCLR_MonthViewDayBkDark));
	m_clrBackgroundSelected.SetStandardValue(pThemeX->GetColor(xtpCLR_MonthViewDayBkSelected));

	TBase::RefreshMetrics(bRefreshChildren);

	m_clrTodayBorder.SetStandardValue(GetHeaderPartX()->m_clrTodayBaseColor);
}

void CXTPCalendarThemeOffice2007::CTOMonthViewDay::SetDynColors(CXTPCalendarMonthViewDay* pMViewDay)
{
	CXTPCalendarThemeOffice2007* pThemeX = DYNAMIC_DOWNCAST(CXTPCalendarThemeOffice2007, GetTheme());
	CXTPCalendarThemeOffice2007::CTOCellDynBackground* pCellDynBkPart =
		XTP_SAFE_GET1(pThemeX, GetCellDynBackgroundPart(), NULL);

	if (!pMViewDay || !pCellDynBkPart || !GetTheme() || !GetTheme()->GetColorsSetPart())
	{
		ASSERT(FALSE);
		return;
	}

	CXTPCalendarViewGroup* pViewGroup = pMViewDay->GetViewGroupsCount() ? pMViewDay->GetViewGroup_(0) : NULL;

	CXTPCalendarTheme::XTP_BUSY_STATUS_CONTEXT bsColors;
	m_nDynBusyStatus = pCellDynBkPart->GetCellDynBackground(pViewGroup, pMViewDay->GetDayDate(), -1, bsColors);
	m_fHatchBackground = pCellDynBkPart->DrawMonthViewHatchBackground();

	m_clrDynHatch = 0;

	if (m_nDynBusyStatus == xtpCalendarBusyStatusBusy ||
		m_nDynBusyStatus == xtpCalendarBusyStatusOutOfOffice ||
		m_nDynBusyStatus == xtpCalendarBusyStatusTentative)
	{
		if (bsColors.clrBkAllDayEvents != 0 && bsColors.clrCellBorder != 0) // is Category or OutOfOffice
		{
			m_clrDynHatch = bsColors.clrBkAllDayEvents;

			if (m_nDynBusyStatus != xtpCalendarBusyStatusTentative)
			{
				m_clrBackgroundLight = bsColors.clrBkAllDayEvents;
				m_clrBackgroundDark = bsColors.clrBkAllDayEvents;
				m_clrBackgroundSelected = CXTPCalendarThemeOffice2007::PowerColor(bsColors.clrBkAllDayEvents, -7.5, 0.25);
			}
		}
		else
		{
			if (m_nDynBusyStatus == xtpCalendarBusyStatusBusy)
			{
				m_clrBackgroundLight = GetTheme()->GetColorsSetPart()->GetColor(xtpCLR_HeaderBorder); // 141, 174, 217
				m_clrBackgroundDark = (COLORREF)m_clrBackgroundLight;
			}
			m_clrDynHatch = GetTheme()->GetColorsSetPart()->GetColor(xtpCLR_HeaderBorder); // 141, 174, 217;
		}
	}
	else
	{
		m_clrBackgroundLight.SetDefaultValue();
		m_clrBackgroundDark.SetDefaultValue();
		m_clrBackgroundSelected.SetDefaultValue();
	}

	if(m_nDynBusyStatus != xtpCalendarBusyStatusTentative && m_fHatchBackground)
	{
		CXTPCalendarEvent* pTentativeEvent = pCellDynBkPart->GetBkTentativeEvent();

		CXTPCalendarEventCategory* pCat = NULL;
		if (pTentativeEvent->GetCategories()->GetSize())
		{
			UINT nCatID = pTentativeEvent->GetCategories()->GetAt(0);
			pCat = XTP_SAFE_GET3(pTentativeEvent, GetDataProvider(), GetEventCategories(), Find(nCatID), NULL);
			m_clrDynHatch = pCat->GetBkColorAllDayEvents();
		}
		else
		{
			m_clrDynHatch = GetTheme()->GetColorsSetPart()->GetColor(xtpCLR_HeaderBorder); // 141, 174, 217;
		}
	}
}

void CXTPCalendarThemeOffice2007::CTOMonthViewDay::Draw(CCmdTarget* pObject, CDC* pDC)
{
	CXTPCalendarThemeOffice2007* pThemeX = DYNAMIC_DOWNCAST(CXTPCalendarThemeOffice2007, GetTheme());
	CXTPCalendarMonthViewDay* pMViewDay = DYNAMIC_DOWNCAST(CXTPCalendarMonthViewDay, pObject);
	CXTPCalendarThemeOffice2007::CTOCellDynBackground* pCellDynBkPart =
		XTP_SAFE_GET1(pThemeX, GetCellDynBackgroundPart(), NULL);

	if (!pDC || !pThemeX || !pCellDynBkPart || !pMViewDay || !pMViewDay->GetView() ||
		!pMViewDay->GetView()->GetGrid() || pMViewDay->GetViewGroupsCount() == 0 ||
		!pMViewDay->GetViewGroup(0) || !pThemeX->GetCalendarControl())
	{
		ASSERT(FALSE);
		return;
	}

	CXTPCalendarMonthView::CMonthViewGrid* pGrid = pMViewDay->GetView()->GetGrid();
	CXTPCalendarMonthViewGroup* pMViewGroup = pMViewDay->GetViewGroup(0);
	XTP_VIEW_DAY_LAYOUT& dayLayout = CXTPCalendarTheme::GetViewDayLayout(pMViewDay);

	CRect rcDay = dayLayout.m_rcDay;
	CRect rcBorders = rcDay;
	rcBorders.top = dayLayout.m_rcDayHeader.bottom;

	COleDateTime dtDay = pMViewDay->GetDayDate();
	COleDateTime dtNow = CXTPCalendarUtils::GetCurrentTime();

	BOOL bIsToday = GetHeaderPart()->IsToday(pMViewDay);
	BOOL bIsSelected = GetHeaderPart()->IsSelected(pMViewDay);

	UINT uDay = (UINT) dtDay;
	bIsSelected = bIsSelected || pMViewDay->GetView()->IsInDiscreteSelection(uDay);

	COleDateTime dtMViewBase = XTP_SAFE_GET2(pMViewDay, GetView(), GetMDayViewLightColorBaseDate(), COleDateTime());
	int nCurrentMonth = dtMViewBase.GetStatus() == COleDateTime::valid ?  dtMViewBase.GetMonth() : dtNow.GetMonth();
	int nLiteBkColorIdx = nCurrentMonth % 2;

	int nMonth = dtDay.GetMonth();
	COLORREF clrBackground = nMonth % 2 == nLiteBkColorIdx ? m_clrBackgroundLight : m_clrBackgroundDark;
	if (bIsSelected)
	{
		clrBackground = m_clrBackgroundSelected;
	}

	pDC->FillSolidRect(rcDay, clrBackground);

	if (m_nDynBusyStatus == xtpCalendarBusyStatusTentative || m_fHatchBackground)
	{
		CXTPTextDC autoTxt(pDC, COLORREF_NULL, clrBackground);
		CBrush br(HS_BDIAGONAL, m_clrDynHatch);

		pDC->FillRect(&rcDay, &br);
	}

	if (bIsToday)
	{
		CXTPCalendarTheme::DrawRectPartly(pDC, rcBorders, m_clrTodayBorder, 2, CRect(1, 0, 1, 1));
		CXTPCalendarTheme::DrawRectPartly(pDC, rcBorders, m_clrBorder, 1, CRect(0, 0, 1, 0));
	}
	else
	{
		int nColIdx = pGrid->GetWeekDayCol(dtDay.GetDayOfWeek());
		int nIsLBorder = nColIdx == 0 ? 1 : 0;

		CXTPCalendarTheme::DrawRectPartly(pDC, rcBorders, m_clrBorder, 1, CRect(nIsLBorder, 0, 1, 0));
		CXTPCalendarTheme::DrawRectPartly(pDC, rcBorders, m_clrBorder, 2, CRect(0, 0, 0, 1));
	}
	GetHeaderPart()->Draw(pMViewDay, pDC);

	BOOL bCnt_EvS = FALSE;
	BOOL bCnt_EvMul = FALSE;

	BOOL bDraftMode = pMViewDay->GetView()->m_bDraftMode;
	CString sTotal;

	CXTPPropsStateContext autoStateCnt_EvS;
	CXTPPropsStateContext autoStateCnt_EvMul;

	int nCount = pMViewGroup->GetViewEventsCount();
	for (int i = 0; i < nCount; i++)
	{
		CXTPCalendarViewEvent* pViewEvent = pMViewGroup->GetViewEvent_(i);
		ASSERT(pViewEvent);
		if (pViewEvent && pViewEvent->IsVisible())
		{
			if (pViewEvent->IsMultidayEvent())
			{
				if (!bCnt_EvMul)
					autoStateCnt_EvMul.SetData(GetMultiDayEventPartX(), xtpCalendarBeforeDraw_MonthViewEvent, TRUE);
				bCnt_EvMul = TRUE;

				autoStateCnt_EvMul.SendBeforeDrawThemeObject((LPARAM)pViewEvent);
				if (!bDraftMode || !pDC->IsPrinting())
					GetMultiDayEventPartX()->Draw(pViewEvent, pDC);
				else
				{
					CString strStartTime = pViewEvent->GetItemTextStartTime();
					CString strEndTime = pViewEvent->GetItemTextEndTime();

					DWORD dwOpt = XTP_SAFE_GET3(GetTheme(), GetCalendarControl(), GetCalendarOptions(), dwAdditionalOptions, 0);
					BOOL bShowEndTime = XTP_SAFE_GET2(pViewEvent, GetCalendarControl(), MonthView_IsShowEndDate(), TRUE);
					BOOL bDrawStartTime = dwOpt & xtpCalendarOptMonthViewShowStartTimeAlways;
					BOOL bDrawEndTime = bShowEndTime && (dwOpt & xtpCalendarOptMonthViewShowEndTimeAlways);

					if (bDrawStartTime)
					{
						sTotal += strStartTime;
						sTotal += _T("-");
					}
					if (bDrawEndTime)
					{
						sTotal += strEndTime;
						sTotal += _T("=");
					}
					sTotal += pViewEvent->GetEvent()->GetSubject();
					sTotal += _T("\r\n");
				}

				autoStateCnt_EvMul.RestoreState();
			}
			else
			{
				if (!bCnt_EvS)
					autoStateCnt_EvS.SetData(GetSingleDayEventPartX(), xtpCalendarBeforeDraw_MonthViewEvent, TRUE);
				bCnt_EvS = TRUE;

				autoStateCnt_EvS.SendBeforeDrawThemeObject((LPARAM)pViewEvent);

				if (!bDraftMode || !pDC->IsPrinting())
					GetSingleDayEventPartX()->Draw(pViewEvent, pDC);
				else
				{
					CString strStartTime = pViewEvent->GetItemTextStartTime();
					CString strEndTime = pViewEvent->GetItemTextEndTime();

					DWORD dwOpt = XTP_SAFE_GET3(GetTheme(), GetCalendarControl(), GetCalendarOptions(), dwAdditionalOptions, 0);
					BOOL bShowEndTime = XTP_SAFE_GET2(pViewEvent, GetCalendarControl(), MonthView_IsShowEndDate(), TRUE);
					BOOL bDrawStartTime = dwOpt & xtpCalendarOptMonthViewShowStartTimeAlways;
					BOOL bDrawEndTime = bShowEndTime && (dwOpt & xtpCalendarOptMonthViewShowEndTimeAlways);

					if (bDrawStartTime)
					{
						sTotal += strStartTime;
						sTotal += _T("-");
					}
					if (bDrawEndTime)
					{
						sTotal += strEndTime;
						sTotal += _T("=");
					}
					sTotal += pViewEvent->GetEvent()->GetSubject();
					sTotal += _T("\r\n");
					//sTotal += _T(" ");
				}

				autoStateCnt_EvS.RestoreState();
			}
		}
	}

	if (bDraftMode && pDC->IsPrinting())
	{
		CRect rcPrint = dayLayout.m_rcDay;
		rcPrint.top = dayLayout.m_rcDayHeader.bottom;

		rcPrint.DeflateRect(3, 3);

		CFont m_Font;
		m_Font.CreatePointFont(70, _T("Arial"));
		CXTPFontDC autoFont(pDC, &m_Font, RGB(0,0,0));

		pDC->DrawText(sTotal, &rcPrint, DT_LEFT | DT_WORDBREAK | DT_WORD_ELLIPSIS);
	}

	if (!pDC->IsPrinting() && pMViewDay->NoAllEventsAreVisible())
	{
		CRect rc = dayLayout.m_rcDay;
		rc.right -= 5;
		rc.bottom -= 5;

		BOOL bHot = HitTestExpandDayButton(pMViewDay) != 0;

		dayLayout.m_rcExpandSign = pThemeX->DrawExpandDayButton(pDC, rc, FALSE, bHot);

		if (bHot)
			dayLayout.m_nHotState |= XTP_VIEW_DAY_LAYOUT::xtpHotExpandButton;
		else
			dayLayout.m_nHotState &= ~XTP_VIEW_DAY_LAYOUT::xtpHotExpandButton;
	}

	autoStateCnt_EvS.Clear();
	autoStateCnt_EvMul.Clear();
}

void CXTPCalendarThemeOffice2007::CTOMonthViewDay::OnMouseMove(CCmdTarget* pObject, UINT nFlags, CPoint point)
{
	UNREFERENCED_PARAMETER(nFlags);

	CXTPCalendarThemeOffice2007* pThemeX = DYNAMIC_DOWNCAST(CXTPCalendarThemeOffice2007, GetTheme());
	CXTPCalendarMonthViewDay* pMViewDay = DYNAMIC_DOWNCAST(CXTPCalendarMonthViewDay, pObject);
	if (!pThemeX || !pMViewDay || !pThemeX->GetCalendarControl())
	{
		ASSERT(FALSE);
		return;
	}

	XTP_VIEW_DAY_LAYOUT& dayLayout = CXTPCalendarTheme::GetViewDayLayout(pMViewDay);

	if (pMViewDay->NoAllEventsAreVisible())
	{
		BOOL bHot = pThemeX->ExpandDayButton_HitTest(point, dayLayout.m_rcExpandSign, FALSE);
		BOOL bHotPrev = !!(dayLayout.m_nHotState & XTP_VIEW_DAY_LAYOUT::xtpHotExpandButton);

		if (bHot != bHotPrev)
		{
			CXTPCalendarControl::CUpdateContext updateContext(pThemeX->GetCalendarControl(), xtpCalendarUpdateRedraw);

		}
	}

	BOOL bNotBusy(TRUE);
	CXTPCalendarView* pView = pThemeX->GetCalendarControl()->GetActiveView();
	if (pView != NULL && pView->IsEditingSubject())
		bNotBusy = FALSE;

	BOOL bHotPrev = !!(dayLayout.m_nHotState & XTP_VIEW_DAY_LAYOUT::xtpHotDayHeader);
	if (dayLayout.m_rcDayHeader.PtInRect(point) != bHotPrev && bNotBusy)
	{

		CXTPCalendarControl::CUpdateContext updateContext(pThemeX->GetCalendarControl(), xtpCalendarUpdateRedraw);
	}
}

BOOL CXTPCalendarThemeOffice2007::CTOMonthViewDay::OnLButtonDown(CCmdTarget* pObject, UINT nFlags, CPoint point)
{
	UNREFERENCED_PARAMETER(nFlags);

	CXTPCalendarThemeOffice2007* pThemeX = DYNAMIC_DOWNCAST(CXTPCalendarThemeOffice2007, GetTheme());
	CXTPCalendarMonthViewDay* pMViewDay = DYNAMIC_DOWNCAST(CXTPCalendarMonthViewDay, pObject);
	if (!pThemeX || !pMViewDay || !pThemeX->GetCalendarControl())
	{
		ASSERT(FALSE);
		return FALSE;
	}
	XTP_VIEW_DAY_LAYOUT& dayLayout = CXTPCalendarTheme::GetViewDayLayout(pMViewDay);

	if (pMViewDay->NoAllEventsAreVisible())
	{
		if (pThemeX->ExpandDayButton_HitTest(point, dayLayout.m_rcExpandSign, FALSE))
		{
			pThemeX->GetCalendarControl()->PostMessage(xtp_wm_UserAction,
				(WPARAM)(xtpCalendarUserAction_OnExpandDay | xtpCalendarExpandDayButton_MonthView),
				(LPARAM)(LONG)(DATE)pMViewDay->GetDayDate());

			return TRUE;
		}
	}

	return FALSE;
}

void CXTPCalendarThemeOffice2007::CTOMonthViewDay::DoPropExchange(CXTPPropExchange* pPX)
{
	ASSERT(pPX);
	if (!pPX)
		return;

	CXTPPropExchangeSection secData(pPX->GetSection(_T("MonthViewDay")));
	if (pPX->IsStoring())
		secData->EmptySection();

	PX_Color(&secData, _T("BorderColor"),       m_clrBorder);
	PX_Color(&secData, _T("TodayBorderColor"),  m_clrTodayBorder);

	PX_Color(&secData, _T("BackgroundLightColor"),   m_clrBackgroundLight);
	PX_Color(&secData, _T("BackgroundDarkColor"),    m_clrBackgroundDark);
	PX_Color(&secData, _T("BackgroundSelectedColor"), m_clrBackgroundSelected);

	TBase::DoPropExchange(&secData);
}

void CXTPCalendarThemeOffice2007::CTOMonthViewDay::Serialize(CArchive& ar)
{
	m_clrBorder.Serialize(ar);
	m_clrTodayBorder.Serialize(ar);

	m_clrBackgroundLight.Serialize(ar);
	m_clrBackgroundDark.Serialize(ar);
	m_clrBackgroundSelected.Serialize(ar);

	TBase::Serialize(ar);
}

void CXTPCalendarThemeOffice2007::CTOMonthViewDayHeader::RefreshMetrics(BOOL bRefreshChildren)
{
	UNREFERENCED_PARAMETER(bRefreshChildren);

	CXTPCalendarThemeOffice2007* pThemeX = DYNAMIC_DOWNCAST(CXTPCalendarThemeOffice2007, GetTheme());
	ASSERT(pThemeX);
	if (!pThemeX)
	{
		return;
	}

	RefreshFromParent(pThemeX->GetMonthViewPartX()->GetHeaderPartX());

//  m_clrTodayBaseColor.SetStandardValue(pThemeX->GetHeaderPartX()->m_clrTodayBaseColor);
	m_bkNormal.nBitmapID.SetStandardValue(xtpIMG_MonthViewDayHeader);
	m_bkSelected.nBitmapID.SetStandardValue(xtpIMG_MonthViewDayHeader);
	m_bkToday.nBitmapID.SetStandardValue(xtpIMG_MonthViewDayHeaderToday);
	m_bkTodaySelected.nBitmapID.SetStandardValue(xtpIMG_MonthViewDayHeaderToday);

	m_bkToday.rcBitmapBorder.        SetStandardValue(CRect(2, 2, 2, 2));
	m_bkTodaySelected.rcBitmapBorder.SetStandardValue(CRect(2, 2, 2, 2));
}

void CXTPCalendarThemeOffice2007::CTOMonthViewDayHeader::Draw(CCmdTarget* pObject, CDC* pDC)
{
	CXTPCalendarThemeOffice2007* pThemeX = DYNAMIC_DOWNCAST(CXTPCalendarThemeOffice2007, GetTheme());
	CXTPCalendarMonthViewDay* pMViewDay = DYNAMIC_DOWNCAST(CXTPCalendarMonthViewDay, pObject);
	if (!pDC || !pThemeX || !pMViewDay || !pThemeX->GetCalendarControl() ||
		!pMViewDay->GetView() || !pMViewDay->GetView()->GetGrid())
	{
		ASSERT(FALSE);
		return;
	}
	XTP_VIEW_DAY_LAYOUT& dayLayout = CXTPCalendarTheme::GetViewDayLayout(pMViewDay);
	COleDateTime dtDay = pMViewDay->GetDayDate();
	BOOL bCompressWeD = XTP_SAFE_GET3(pMViewDay, GetView(), GetCalendarControl(), MonthView_IsCompressWeekendDays(), FALSE);

	BOOL bIsToday    = IsToday(pMViewDay);
	BOOL bIsSelected = IsSelected(pMViewDay);

	int nDImod7 = pMViewDay->GetDayIndex() % 7;
	BOOL bFirstDay = nDImod7 == 0 || (nDImod7 == 6 && bCompressWeD);
	BOOL bLastDay = nDImod7 >= (bCompressWeD ? 5 : 6);

	CPoint pt(0, 0);
	if (pThemeX->GetCalendarControl()->GetSafeHwnd() && GetCursorPos(&pt))
		pThemeX->GetCalendarControl()->ScreenToClient(&pt);

	int nFlags = bIsToday ? CTOHeader::xtpCalendarItemState_Today : CTOHeader::xtpCalendarItemState_Normal;
	nFlags |= bIsSelected ? CTOHeader::xtpCalendarItemState_Selected : 0;
	if (dayLayout.m_rcDayHeader.PtInRect(pt))
	{
		nFlags |=  CTOHeader::xtpCalendarItemState_Hot;
		dayLayout.m_nHotState |= XTP_VIEW_DAY_LAYOUT::xtpHotDayHeader;
	}
	else
	{
		dayLayout.m_nHotState &= ~XTP_VIEW_DAY_LAYOUT::xtpHotDayHeader;
	}

	nFlags |= bFirstDay ? CTOHeader::xtpCalendarItemFirst : 0;
	nFlags |= bLastDay ? CTOHeader::xtpCalendarItemLast : 0;

	CRect rcRect = dayLayout.m_rcDay;

	//***
	rcRect.bottom = dayLayout.m_rcDayHeader.bottom;

	CString strTextLeft;
	CString strTextCenter;
	CString strTextRight;

	int nMonth = dtDay.GetMonth();
	int nDay = dtDay.GetDay();

	if (nDay == 1 || pMViewDay->GetDayIndex() == 0)
	{
		CString strMonth = CXTPCalendarUtils::GetLocaleString(LOCALE_SABBREVMONTHNAME1 + nMonth - 1, 100);

		//LOCALE_IDATE  -  short date format ordering
		// 0 Month-Day-Year
		// 1 Day-Month-Year
		// 2 Year-Month-Day
		int nDateOrdering = CXTPCalendarUtils::GetLocaleLong(LOCALE_IDATE);

		if (nDateOrdering == 1)
			strTextLeft.Format(_T("%d %s"), nDay, (LPCTSTR)strMonth);
		else
			strTextLeft.Format(_T("%s %d"), (LPCTSTR)strMonth, nDay);

		if ((CXTPCalendarMonthView*) pMViewDay->GetView_())
			strTextLeft =
			((CXTPCalendarMonthView*) pMViewDay->GetView_())->_FormatDayDate(dtDay, FALSE);
	}
	else
	{
		strTextLeft.Format(_T("%d"), nDay);
	}

	pThemeX->GetItemTextIfNeed(xtpCalendarItemText_MonthViewDayHeaderLeft, &strTextLeft, pMViewDay);
	pThemeX->GetItemTextIfNeed(xtpCalendarItemText_MonthViewDayHeaderCenter, &strTextCenter, pMViewDay);
	pThemeX->GetItemTextIfNeed(xtpCalendarItemText_MonthViewDayHeaderRight, &strTextRight, pMViewDay);

	Draw_Header(pDC, rcRect, nFlags, strTextLeft, strTextCenter, strTextRight);
}

void CXTPCalendarThemeOffice2007::CTOEvent::GetMultidayTimeFontAndColor(CFont*& pFont, COLORREF& clrColor, BOOL bSelected)
{
	pFont = bSelected ? m_fcsetSelected.fcsetStartEnd.Font() : m_fcsetNormal.fcsetStartEnd.Font();
	clrColor = bSelected ? m_fcsetSelected.fcsetStartEnd.Color() : m_fcsetNormal.fcsetStartEnd.Color();
}

void CXTPCalendarThemeOffice2007::CTOEvent::MultidayDrawTimeBlocks(CDC* pDC, const CRect& rcEventRect,
																		CXTPCalendarViewEvent* pViewEvent, BOOL MultiDayFirstPart, BOOL MultiDayLastPart, BOOL fStartTimeAsClock, BOOL fEndTimeAsClock)
{
	if (!pDC || !pViewEvent || !pViewEvent->GetEvent())
	{
		ASSERT(FALSE);
		return;
	}

	DWORD dwOpt = XTP_SAFE_GET3(GetTheme(), GetCalendarControl(), GetCalendarOptions(), dwAdditionalOptions, 0);
	BOOL bShowEndTime = XTP_SAFE_GET2(pViewEvent, GetCalendarControl(), MonthView_IsShowEndDate(), TRUE);
	BOOL bDrawStartTime = (dwOpt & xtpCalendarOptMonthViewShowStartTimeAlways)  && MultiDayFirstPart;
	BOOL bDrawEndTime = bShowEndTime && (dwOpt & xtpCalendarOptMonthViewShowEndTimeAlways) && MultiDayLastPart;

	BOOL bSelected = pViewEvent->IsSelected();

	CFont* pFont = NULL;
	COLORREF clrColor;
	GetMultidayTimeFontAndColor(pFont, clrColor, bSelected);

	CXTPFontDC autoFont(pDC, pFont, clrColor);

	COleDateTime dtStart(pViewEvent->GetEvent()->GetStartTime());
	COleDateTime dtEnd(pViewEvent->GetEvent()->GetEndTime());

	CString strStartTime = pViewEvent->GetItemTextStartTime();
	CString strEndTime = pViewEvent->GetItemTextEndTime();

	CSize szStart = pDC->GetTextExtent(strStartTime);
	CSize szEnd = pDC->GetTextExtent(strEndTime);

	CRect rcStart(rcEventRect);
	CRect rcEnd(rcEventRect);

	BOOL bDrawStartEndTimes = szStart.cx + szEnd.cx + 12 < rcEventRect.Width()/2;

	if (!bDrawStartEndTimes && !bDrawStartTime && !bDrawEndTime)
		return;

	UINT uAlign = DT_VCENTER | DT_SINGLELINE;
	rcStart.left = min(rcStart.left + 6, rcStart.right);
	rcStart.right = min(rcStart.left + szStart.cx + 2, rcStart.right);
	if (bDrawStartTime)
	{
		if (fStartTimeAsClock)
			DrawClock(pDC, dtStart, rcStart, RGB(224,224,224), xtpCalendarClockAlignLeft);
		else
			pDC->DrawText(strStartTime, &rcStart, uAlign | DT_LEFT);
	}
	else
	{
		rcStart.right = rcStart.left;
	}

	rcEnd.right = max(rcEnd.right - 6, rcEventRect.left);
	rcEnd.left = max(rcEnd.right - szEnd.cx - 2, rcEventRect.left);
	if (bDrawEndTime)
	{
		if (fEndTimeAsClock)
			DrawClock(pDC, dtEnd, rcEnd, RGB(224,224,224), xtpCalendarClockAlignLeft);
		else
			pDC->DrawText(strEndTime, &rcEnd, uAlign | DT_RIGHT);
	}
	else
	{
		rcEnd.right = rcEnd.left;
	}

	pViewEvent->m_rcStartTime.CopyRect(&rcStart);
	pViewEvent->m_rcEndTime.CopyRect(&rcEnd);

	pViewEvent->m_rcText.left = rcStart.right;
	pViewEvent->m_rcText.right = rcEnd.left;
}

void CXTPCalendarThemeOffice2007::CTOEvent::MultidayDrawTime(int nMDEFlags, CXTPCalendarViewEvent* pViewEvent, CDC* pDC, const CRect& rcEvent, BOOL fStartTimeAsClock, BOOL fEndTimeAsClock, CRect& rcText)
{
	CXTPCalendarThemeOffice2007* pThemeX = DYNAMIC_DOWNCAST(CXTPCalendarThemeOffice2007, GetTheme());
	BOOL bHideTimes = XTP_SAFE_GET3(GetTheme(), GetCalendarControl(), GetCalendarOptions(), bMonthView_HideTimes, FALSE);
	if(pThemeX->GetCalendarControl()->GetActiveView()->GetViewType() == xtpCalendarMonthView && bHideTimes)
		return;

	if(nMDEFlags & xtpCalendarMultiDayFirst || nMDEFlags & xtpCalendarMultiDayLast)
	{
		BOOL fAllTimesZero = CXTPCalendarUtils::IsZeroTime(pViewEvent->GetEvent()->GetStartTime()) && CXTPCalendarUtils::IsZeroTime(pViewEvent->GetEvent()->GetEndTime());
		if(!pViewEvent->GetEvent()->IsAllDayEvent() && !fAllTimesZero)
		{
			BOOL fMultidayFirstPart = nMDEFlags & xtpCalendarMultiDayFirst ? TRUE : FALSE;
			BOOL fMultidayLastPart =  nMDEFlags & xtpCalendarMultiDayLast ? TRUE : FALSE;

			MultidayDrawTimeBlocks(pDC, rcEvent, pViewEvent, fMultidayFirstPart, fMultidayLastPart, fStartTimeAsClock, fEndTimeAsClock);
			rcText = pViewEvent->GetTextRect();
		}
	}
}

void CXTPCalendarThemeOffice2007::CTOMonthViewEvent_MultiDay::Draw(CCmdTarget* pObject, CDC* pDC)
{
	CXTPCalendarThemeOffice2007* pThemeX = DYNAMIC_DOWNCAST(CXTPCalendarThemeOffice2007, GetTheme());
	CXTPCalendarMonthViewEvent* pViewEvent = DYNAMIC_DOWNCAST(CXTPCalendarMonthViewEvent, pObject);

	CXTPCalendarMonthViewDay* pMVDay = XTP_SAFE_GET2(pViewEvent, GetViewGroup(), GetViewDay(), NULL);
	CXTPCalendarMonthView* pMView = XTP_SAFE_GET1(pMVDay, GetView(), NULL);

	if (!pDC || !pThemeX || !pViewEvent || !pViewEvent->GetEvent() ||
		!pMView || !pMVDay)
	{
		ASSERT(FALSE);
		return;
	}
	XTPCalendarDraggingMode dragMode = pMView->GetDraggingMode();
	// Restriction to prevent extra text flickering for Office2007 Theme draw
	if (dragMode == xtpCalendaDragModeEditSubject)
	{
		CWnd* pEditor = pViewEvent->GetSubjectEditor();
		if (pEditor != NULL && pEditor->IsWindowVisible())
			return;
	}

	BOOL bNoVisibleMDEvent = pViewEvent->IsMultidayEvent() && !pViewEvent->IsVisible();

	int nMDEFlags = pViewEvent->GetMultiDayEventFlags();
	if ((nMDEFlags & xtpCalendarMultiDaySlave) || bNoVisibleMDEvent)
	{
		return;// was drawn in the Master (or out of events rect)
	}

	CRect rcEventMax(pViewEvent->GetViewEventRectMax());
	CRect rcEvent(pViewEvent->GetEventRect());
	if (rcEvent.IsRectEmpty())
	{
		return;
	}

	// fill event background
	BOOL bSelected = pViewEvent->IsSelected();
	COLORREF clrBorderNotSel;
	COLORREF clrBorder;
	CXTPPaintManagerColorGradient grclrBk;

	GetEventColors(pDC, pViewEvent, clrBorder, clrBorderNotSel, grclrBk);

	pThemeX->DrawEventRect(pDC, rcEvent, clrBorder, bSelected, grclrBk);

	int nSelOffset = bSelected ? 2 : 1;

	if (nMDEFlags & xtpCalendarMultiDayFirst)
	{
		pThemeX->_DrawEventLTHighlight(pDC, CPoint(rcEvent.left + nSelOffset, rcEvent.top + nSelOffset),
			rcEvent.right - nSelOffset, grclrBk, cnCornerSize / 2, rcEvent.bottom);
	}

	int nViewDayIndex = pMVDay->GetDayIndex();

	BOOL bCompressWD = XTP_SAFE_GET2(pViewEvent, GetCalendarControl(), MonthView_IsCompressWeekendDays(), FALSE);
	int nLastDayDx = bCompressWD ? 1 : 7;

	BOOL bFirstOnScreen = nViewDayIndex == 0;
	BOOL bLastOnScreen = nViewDayIndex == (pMView->GetViewDayCount() - nLastDayDx);

	CRect rcText(pViewEvent->GetTextRect());

	LONG lShowAsBlockWidth = 0;
	BOOL fShowAsStatusDrawn = FALSE;
	if(pThemeX->m_bFlatStyle)
	{
		fShowAsStatusDrawn = DrawShowAsStatus(pDC, pViewEvent, pThemeX, rcEventMax, lShowAsBlockWidth);
	}

	rcEvent.left += lShowAsBlockWidth;

	if ((nMDEFlags & xtpCalendarMultiDayFirst) == 0)
	{
		CRect rcSQ = rcEventMax;
		rcSQ.right = min(rcEvent.left + cnCornerSize, rcEventMax.right);

		if(!fShowAsStatusDrawn)
			CXTPCalendarTheme::DrawRectPartly(pDC, rcSQ, clrBorder, bSelected, grclrBk, CRect(1,1,0,1));

		pThemeX->_DrawEventLTHighlight(pDC, CPoint(rcEvent.left + nSelOffset, rcEvent.top + nSelOffset),
			rcEvent.right - nSelOffset, grclrBk, 0, rcEvent.bottom - nSelOffset);

		if (bFirstOnScreen)
		{
			CRect rcArrow;
			BOOL bArrow = Draw_ArrowL(pViewEvent, pDC, rcArrow, lShowAsBlockWidth);
			if (bArrow)
				rcText.left = rcArrow.right;
		}
	}
	else if (bSelected)
	{
		// draw left Gripper
		CRect rcGripper;
		rcGripper.top = rcEvent.top + rcEvent.Height() / 2 - cnGripperSize / 2;
		rcGripper.bottom = rcGripper.top + cnGripperSize;
		rcGripper.left = rcEvent.left - 1;
		rcGripper.right = rcGripper.left + cnGripperSize;

		pDC->FillSolidRect(&rcGripper, m_clrGripperBackground);
		CBrush brBorder(m_clrGripperBorder);
		pDC->FrameRect(&rcGripper, &brBorder);
	}

	if ((nMDEFlags & xtpCalendarMultiDayLast) == 0)
	{
		CRect rcSQ = rcEventMax;
		rcSQ.left = max(rcEvent.right - cnCornerSize, rcEventMax.left);
		CXTPCalendarTheme::DrawRectPartly(pDC, rcSQ, clrBorder, bSelected, grclrBk, CRect(0,1,1,1));

		pThemeX->_DrawEventLTHighlight(pDC, CPoint(rcSQ.left, rcEvent.top + nSelOffset),
			rcEventMax.right - nSelOffset, grclrBk, 0, INT_MAX, FALSE);

		if (bLastOnScreen)
		{
			CRect rcArrow;
			BOOL bArrow = Draw_ArrowR(pViewEvent, pDC, rcArrow);
			if (bArrow)
				rcText.right = rcArrow.left;
		}
	}
	else if (bSelected)
	{
		// draw right gripper
		CRect rcGripper;
		rcGripper.top = rcEvent.top + rcEvent.Height() / 2 - cnGripperSize / 2;
		rcGripper.bottom = rcGripper.top + cnGripperSize;
		rcGripper.right = rcEvent.right + 1;
		rcGripper.left = rcGripper.right - cnGripperSize;

		pDC->FillSolidRect(&rcGripper, m_clrGripperBackground);
		CBrush brBorder(m_clrGripperBorder);
		pDC->FrameRect(&rcGripper, &brBorder);
	}

	MultidayDrawTime(nMDEFlags, pViewEvent, pDC, rcEvent, pViewEvent->IsTimeAsClock(), pViewEvent->IsTimeAsClock(), rcText);

	pViewEvent->m_rcText.CopyRect(rcText);

	CThemeFontColorSet* pFntClr1 = bSelected ? &m_fcsetSelected.fcsetSubject : &m_fcsetNormal.fcsetSubject;
	CThemeFontColorSet* pFntClr2 = bSelected ? &m_fcsetSelected.fcsetLocation : &m_fcsetNormal.fcsetLocation;

	rcText.DeflateRect(3, 0, 3, 0);

	pViewEvent->m_rcSubjectEditor.CopyRect(rcText);
	pViewEvent->m_rcSubjectEditor.top = rcEvent.top + 2;
	pViewEvent->m_rcSubjectEditor.bottom = rcEvent.bottom - 2;
	pViewEvent->AdjustSubjectEditor();

	CRect rcIconsMax = rcText;
	rcIconsMax.left = min(rcIconsMax.left + rcText.Height(), rcIconsMax.right);

	//
	BOOL bMarkupDrawn = FALSE;
	if (pThemeX->GetCalendarControl()->IsMarkupEnabled())
	{
		CXTPMarkupContext* pMarkupContext = pThemeX->GetCalendarControl()->GetMarkupContext();
		ASSERT(pMarkupContext);

		CXTPMarkupUIElement* pUIElement = pViewEvent->GetMarkupUIElement();

		if (pUIElement && pMarkupContext)
		{
			CSize szIcons = Draw_Icons(pViewEvent, pDC, rcIconsMax, TRUE);

			if (rcIconsMax.Height() > szIcons.cy)
			{
				rcIconsMax.bottom -= (rcIconsMax.Height() - szIcons.cy) / 2 + 1;
			}

			rcText.right = max(rcText.left,  rcText.right - szIcons.cx - 2);

			XTPMarkupSetDefaultFont(pMarkupContext, (HFONT)pFntClr1->Font()->GetSafeHandle(), pFntClr1->Color());

			XTPMarkupRenderElement(pUIElement, pDC->GetSafeHdc(), rcText);

			bMarkupDrawn = TRUE;
		}
	}

	if (!bMarkupDrawn)
	{
		CString strSubj = pViewEvent->GetItemTextSubject();
		CString strLoc = pViewEvent->GetItemTextLocation();

		if (!strLoc.IsEmpty())
		{
			strSubj += _T(";");
		}

		UINT uTextShortAlign = DT_VCENTER | DT_LEFT;
		CSize szText = pThemeX->DrawLine2_CenterLR(pDC, strSubj, strLoc, pFntClr1, pFntClr2,
									rcText, uTextShortAlign | DT_CALCRECT);

		//--- calculate rects and fix to be centered if enough space ---
		CSize szIcons = Draw_Icons(pViewEvent, pDC, rcIconsMax, TRUE);

		if (rcIconsMax.Height() > szIcons.cy)
		{
			rcIconsMax.bottom -= (rcIconsMax.Height() - szIcons.cy) / 2;
		}

		int nTIcx = szText.cx + szIcons.cx;
		if (nTIcx + 5 < rcText.Width())
		{
			int ncx_2 = (rcText.Width() - nTIcx) / 2 - 2;
			rcText.left += ncx_2;
			rcText.right = rcText.left + szText.cx + 2;

			rcIconsMax.right -= ncx_2;
		}
		else
		{
			rcText.right -= szIcons.cx + 2;
		}

		pThemeX->DrawLine2_CenterLR(pDC, strSubj, strLoc, pFntClr1, pFntClr2,
			rcText, uTextShortAlign);
	}

	Draw_Icons(pViewEvent, pDC, rcIconsMax);
}

BOOL CXTPCalendarThemeOffice2007::CTOEvent::DrawShowAsStatus(CDC* pDC, CXTPCalendarViewEvent* pViewEvent, CXTPCalendarThemeOffice2007* pThemeX, const CRect& rcEvent, LONG& lShowAsBlockWidth)
{
	lShowAsBlockWidth = 0;

	COLORREF clrBorderNotSel;
	COLORREF clrBorder;
	CXTPPaintManagerColorGradient grclrBk;

	BOOL bSelected = pViewEvent->IsSelected();

	GetShowAsStatusColors(pDC, pViewEvent, clrBorder, clrBorderNotSel, grclrBk);

	int nBusyStatus = pViewEvent->GetEvent()->GetBusyStatus();
	CBrush* pBSBrush = GetBusyStatusBrush(nBusyStatus);

	if (!pBSBrush)
		return FALSE;

	CRect rcBS = rcEvent;
	rcBS.right = min(rcBS.left + 12, rcEvent.right);

	CXTPTextDC autoText(pDC, clrBorderNotSel, RGB(255, 255, 255));

	if (pThemeX->m_bFlatStyle)
		pThemeX->DrawRect(pDC, rcBS, clrBorder, bSelected, pBSBrush);
	else
		pThemeX->DrawRoundRect(pDC, rcBS, clrBorder, bSelected, pBSBrush);

	rcBS.right++;
	rcBS.left = min(rcBS.left + 5, rcBS.right);

	CXTPCalendarTheme::DrawRectPartly(pDC, rcBS, clrBorder, bSelected, grclrBk, CRect(0, 1, 0, 1));

	rcBS.right = rcBS.left + 1;
	pDC->FillSolidRect(&rcBS, clrBorder);

	lShowAsBlockWidth = 6;
	return TRUE;
}

void CXTPCalendarThemeOffice2007::CTOEvent::GetShowAsStatusColors(CDC* pDC, CXTPCalendarViewEvent* pViewEvent, COLORREF& clrBorder, COLORREF& clrBorderNotSel, CXTPPaintManagerColorGradient& grclrBk)
{
	GetEventColors(pDC, pViewEvent, clrBorder, clrBorderNotSel, grclrBk);
}

void CXTPCalendarThemeOffice2007::CTOMonthViewEvent_SingleDay::GetShowAsStatusColors(CDC* pDC, CXTPCalendarViewEvent* pViewEvent, COLORREF& clrBorder, COLORREF& clrBorderNotSel, CXTPPaintManagerColorGradient& grclrBk)
{
	BOOL bSelected = pViewEvent->IsSelected();

	int nColorsSrc_1Cat2Lbl = 0;
	GetEventColors(pDC, pViewEvent, clrBorder, clrBorderNotSel, grclrBk, bSelected, &nColorsSrc_1Cat2Lbl);
	if (!bSelected && nColorsSrc_1Cat2Lbl > 0)
		clrBorder = grclrBk.clrLight;
}

void CXTPCalendarThemeOffice2007::CTOMonthViewEvent_SingleDay::Draw(CCmdTarget* pObject, CDC* pDC)
{
	CXTPCalendarThemeOffice2007* pThemeX = DYNAMIC_DOWNCAST(CXTPCalendarThemeOffice2007, GetTheme());
	CXTPCalendarMonthViewEvent* pViewEvent = DYNAMIC_DOWNCAST(CXTPCalendarMonthViewEvent, pObject);
	if (!pDC || !pThemeX || !pViewEvent)
	{
		ASSERT(FALSE);
		return;
	}

	if (!pViewEvent->IsVisible())
	{
		return;
	}

	CRect rcEvent(pViewEvent->GetEventRect());
	if (rcEvent.IsRectEmpty())
	{
		return;
	}

	// fill event background
	Draw_Background(pDC, rcEvent, pViewEvent);

	LONG lShowAsBlockWidth = 0;
	if(pThemeX->m_bFlatStyle)
		DrawShowAsStatus(pDC, pViewEvent, pThemeX, rcEvent, lShowAsBlockWidth);

	//
	BOOL bMarkupDrawn = FALSE;
	if (pThemeX->GetCalendarControl()->IsMarkupEnabled())
	{
		CXTPMarkupContext* pMarkupContext = pThemeX->GetCalendarControl()->GetMarkupContext();
		ASSERT(pMarkupContext);

		CXTPMarkupUIElement* pUIElement = pViewEvent->GetMarkupUIElement();

		if (pUIElement && pMarkupContext)
		{
			BOOL bSelected = pViewEvent->IsSelected();
			CThemeFontColorSet* pFntClr1 = bSelected ? &m_fcsetSelected.fcsetSubject : &m_fcsetNormal.fcsetSubject;

			CRect rcText = rcEvent;
			rcText.DeflateRect(3, 2, 3, 2);
			CRect rcIconsMax = rcText;

			CSize szIcons = Draw_Icons(pViewEvent, pDC, rcIconsMax, TRUE);

			if (rcIconsMax.Height() > szIcons.cy)
			{
				rcIconsMax.bottom -= (rcIconsMax.Height() - szIcons.cy) / 2 + 1;
			}

			rcText.right = max(rcText.left,  rcText.right - szIcons.cx - 2);

			XTPMarkupSetDefaultFont(pMarkupContext, (HFONT)pFntClr1->Font()->GetSafeHandle(), pFntClr1->Color());

			XTPMarkupRenderElement(pUIElement, pDC->GetSafeHdc(), rcText);

			bMarkupDrawn = TRUE;

			Draw_Icons(pViewEvent, pDC, rcIconsMax);
		}
	}
	//

	if (!bMarkupDrawn)
	{
		rcEvent.left += lShowAsBlockWidth;

		BOOL bHideTimes = XTP_SAFE_GET3(GetTheme(), GetCalendarControl(), GetCalendarOptions(), bMonthView_HideTimes, FALSE);
		BOOL bIsAllDayEvent = XTP_SAFE_GET2(pViewEvent, GetEvent(), IsAllDayEvent(), FALSE);

		if(!bIsAllDayEvent && !bHideTimes)
			Draw_Time(pDC, rcEvent, pViewEvent);

		CRect rcText(pViewEvent->GetTextRect());
		rcText.left += lShowAsBlockWidth;
		Draw_Caption(pDC, rcText, pViewEvent);
	}
}

void CXTPCalendarThemeOffice2007::CTOMonthViewEvent_SingleDay::
		Draw_Background(CDC* pDC, const CRect& rcEventRect, CXTPCalendarMonthViewEvent* pViewEvent)
{
	CXTPCalendarThemeOffice2007* pThemeX = DYNAMIC_DOWNCAST(CXTPCalendarThemeOffice2007, GetTheme());

	if (!pDC || !pThemeX || !pViewEvent || !pViewEvent->GetEvent())
	{
		ASSERT(FALSE);
		return;
	}
	BOOL bSelected = pViewEvent->IsSelected();

	COLORREF clrBorderNotSel, clrBorder;
	CXTPPaintManagerColorGradient grclrBk;
	int nColorsSrc_1Cat2Lbl = 0;
	GetEventColors(pDC, pViewEvent, clrBorder, clrBorderNotSel, grclrBk, bSelected, &nColorsSrc_1Cat2Lbl);

	if (!bSelected && nColorsSrc_1Cat2Lbl > 0)
	{
		clrBorder = grclrBk.clrLight;
	}

	pThemeX->DrawEventRect(pDC, rcEventRect, clrBorder, bSelected, grclrBk);
}

CSize CXTPCalendarThemeOffice2007::CTOMonthViewEvent_SingleDay::Draw_Time(
		CDC* pDC, const CRect& rcEventRect, CXTPCalendarMonthViewEvent* pViewEvent)
{
	CSize szResult(0, 0);

	if (!pDC || !pViewEvent || !pViewEvent->GetEvent())
	{
		ASSERT(FALSE);
		return szResult;
	}

	BOOL bSelected = pViewEvent->IsSelected();

	CFont* pFont = bSelected ? m_fcsetSelected.fcsetStartEnd.Font() : m_fcsetNormal.fcsetStartEnd.Font();
	COLORREF clrColor = bSelected ? m_fcsetSelected.fcsetStartEnd.Color() : m_fcsetNormal.fcsetStartEnd.Color();

	CXTPFontDC autoFont(pDC, pFont, clrColor);

	CString strStartTime = pViewEvent->GetItemTextStartTime();
	CString strEndTime = pViewEvent->GetItemTextEndTime();

	CSize szStart = pDC->GetTextExtent(strStartTime);
	CSize szEnd = pDC->GetTextExtent(strEndTime);

	CRect rcStart(rcEventRect);
	CRect rcEnd(rcEventRect);

	COleDateTime dtStart = pViewEvent->GetEvent()->GetStartTime();
	COleDateTime dtEnd = pViewEvent->GetEvent()->GetEndTime();

	DWORD dwOpt = XTP_SAFE_GET3(GetTheme(), GetCalendarControl(), GetCalendarOptions(), dwAdditionalOptions, 0);
	BOOL bShowEndTime = XTP_SAFE_GET2(pViewEvent, GetCalendarControl(), MonthView_IsShowEndDate(), TRUE);
	BOOL bDrawStartTime = szStart.cx + 6 < rcEventRect.Width() / 2 && //||
		(dwOpt & xtpCalendarOptMonthViewShowStartTimeAlways);
	BOOL bDrawEndTime = bShowEndTime && szStart.cx + szEnd.cx + 12 < rcEventRect.Width()/2 && //||
		(dwOpt & xtpCalendarOptMonthViewShowEndTimeAlways);

	if (!bDrawStartTime && !bDrawEndTime)
		return szResult;

	int iOffset(0);

	UINT uAlign = DT_VCENTER | DT_SINGLELINE | DT_LEFT;
	if (bDrawStartTime)
	{
		if (pViewEvent->IsTimeAsClock())
		{
			iOffset = DrawClock(pDC, dtStart, rcStart, RGB(224,224,224), xtpCalendarClockAlignLeft);
			rcStart.left += iOffset + 5;
			rcStart.right = rcStart.left;

			rcEnd.left = rcStart.right;
			rcEnd.right = rcStart.right;
			rcEnd.right += szEnd.cx;
		}
		else
		{
			rcStart.left = min(rcStart.left + 4, rcStart.right);
			rcStart.right = min(rcStart.left + szStart.cx + 6, rcStart.right);

			pDC->DrawText(strStartTime, &rcStart, uAlign);
		}
	}
	else
	{
		rcStart.right = rcStart.left + 5;
	}

	if (bDrawEndTime)
	{
		if (pViewEvent->IsTimeAsClock())
		{
			iOffset = DrawClock(pDC, dtEnd, rcEnd, RGB(224,224,224), xtpCalendarClockAlignLeft);
			rcEnd.left += iOffset;
			rcEnd.right = rcEnd.left + 5;
		}
		else
		{
			rcEnd.left = rcStart.right;
			rcEnd.right = min(rcEnd.left + szEnd.cx + 6, rcEnd.right);

			pDC->DrawText(strEndTime, &rcEnd, uAlign);
		}
	}
	else
	{
		rcEnd.left = rcStart.left;
		rcEnd.right = rcStart.right;
	}

	pViewEvent->m_rcStartTime.CopyRect(&rcStart);
	pViewEvent->m_rcEndTime.CopyRect(&rcEnd);
	pViewEvent->m_rcText.left = rcEnd.right;

	szResult.cx = max(0, rcEnd.right - rcStart.left);
	szResult.cy = szStart.cy;

	return szResult;
}

void CXTPCalendarThemeOffice2007::CTOMonthViewEvent_SingleDay::Draw_Caption(
		CDC* pDC, const CRect& rcTextRect, CXTPCalendarMonthViewEvent* pViewEvent)
{
	CXTPCalendarThemeOffice2007* pThemeX = DYNAMIC_DOWNCAST(CXTPCalendarThemeOffice2007, GetTheme());
	if (!pDC || !pViewEvent || !pThemeX )
	{
		ASSERT(FALSE);
		return;
	}

	CString strSubj = pViewEvent->GetItemTextSubject();
	CString strLoc = pViewEvent->GetItemTextLocation();

	if (!strLoc.IsEmpty())
		strSubj += _T(";");

	BOOL bSelected = pViewEvent->IsSelected();
	CThemeFontColorSet* pFntClr1 = bSelected ? &m_fcsetSelected.fcsetSubject : &m_fcsetNormal.fcsetSubject;
	CThemeFontColorSet* pFntClr2 = bSelected ? &m_fcsetSelected.fcsetLocation : &m_fcsetNormal.fcsetLocation;

	CRect rcText = rcTextRect;
	//rcText.DeflateRect(1, 2, 3, 2);

	pViewEvent->m_rcSubjectEditor.CopyRect(rcText);
	pViewEvent->AdjustSubjectEditor();

	//UINT nAlign = DT_VCENTER | DT_LEFT | DT_SINGLELINE;
	UINT nAlign = DT_VCENTER | DT_LEFT | DT_SINGLELINE | DT_END_ELLIPSIS;
	CSize szText = pThemeX->DrawLine2_CenterLR(pDC, strSubj, strLoc, pFntClr1, pFntClr2,
								rcText, nAlign  | DT_CALCRECT, nAlign);

	CRect rcIconsMax = rcText;
	rcIconsMax.left = min(rcIconsMax.left + rcTextRect.Height(), rcIconsMax.right);

	//--- calculate rects and fix to be centered if enough space ---
	CSize szIcons = Draw_Icons(pViewEvent, pDC, rcIconsMax, TRUE);

	if (rcIconsMax.Height() > szIcons.cy)
	{
		rcIconsMax.bottom -= (rcIconsMax.Height() - szIcons.cy) / 2;
	}

	int nTIcx = szText.cx + szIcons.cx;
	if (nTIcx > rcText.Width())
	{
		rcText.right = max(rcIconsMax.left, rcIconsMax.right - szIcons.cx) - 2;
	}

	pThemeX->DrawLine2_CenterLR(pDC, strSubj, strLoc, pFntClr1, pFntClr2,
								rcText, nAlign, nAlign);

	Draw_Icons(pViewEvent, pDC, rcIconsMax);
}

/////////////////////////////////////////////////////////////////////////////
//
// ***** Week View *****
//

void CXTPCalendarThemeOffice2007::CTOWeekView::RefreshMetrics(BOOL bRefreshChildren)
{
	TBase::RefreshMetrics(bRefreshChildren);
}

void CXTPCalendarThemeOffice2007::CTOWeekView::AdjustLayout(CDC* pDC, const CRect& rcRect, BOOL bCallPostAdjustLayout)
{
	CXTPCalendarThemeOffice2007* pThemeX = DYNAMIC_DOWNCAST(CXTPCalendarThemeOffice2007, GetTheme());
	ASSERT(pThemeX);
	if (!pThemeX)
	{
		return;
	}

	TBase::AdjustLayout(pDC, rcRect, bCallPostAdjustLayout);

	if (pThemeX->GetPrevNextEventButtonsPartX())
		pThemeX->GetPrevNextEventButtonsPartX()->AdjustLayout(NULL, pDC, rcRect);
}

void CXTPCalendarThemeOffice2007::CTOWeekView::Draw(CCmdTarget* /*pObject*/, CDC* pDC)
{
	CXTPCalendarThemeOffice2007* pThemeX = DYNAMIC_DOWNCAST(CXTPCalendarThemeOffice2007, GetTheme());
	ASSERT(pThemeX);
	if (!pThemeX)
	{
		return;
	}

	TBase::Draw(pDC);

	if (pThemeX->GetPrevNextEventButtonsPartX())
		pThemeX->GetPrevNextEventButtonsPartX()->Draw(NULL, pDC);
}

void CXTPCalendarThemeOffice2007::CTOWeekViewEvent::RefreshMetrics(BOOL bRefreshChildren)
{
	CXTPCalendarThemeOffice2007* pThemeX = DYNAMIC_DOWNCAST(CXTPCalendarThemeOffice2007, GetTheme());
	ASSERT(pThemeX);
	if (!pThemeX)
	{
		return;
	}

	TBase::RefreshMetrics(bRefreshChildren);

	RefreshFromParent(pThemeX->GetEventPartX());

	XTP_SAFE_CALL1(GetHeightFormulaPart(), SetStandardValue(14, 10, 1));
	XTP_SAFE_CALL1(GetEventIconsToDrawPart(), SetStandardValue(FALSE, FALSE, FALSE, FALSE, FALSE) );
}

void CXTPCalendarThemeOffice2007::CTOWeekViewEvent_MultiDay::RefreshMetrics(BOOL bRefreshChildren)
{
	UNREFERENCED_PARAMETER(bRefreshChildren);

	CXTPCalendarThemeOffice2007* pThemeX = DYNAMIC_DOWNCAST(CXTPCalendarThemeOffice2007, GetTheme());
	ASSERT(pThemeX);
	if (!pThemeX)
	{
		return;
	}

	RefreshFromParent(pThemeX->GetWeekViewPartX()->GetEventPartX());

	m_fcsetNormal.clrBorder.SetStandardValue(pThemeX->GetColor(xtpCLR_MultiDayEventBorder));
	m_fcsetNormal.grclrBackground.SetStandardValue(pThemeX->GetColor(xtpCLR_MultiDayEventBkGRfrom),
		pThemeX->GetColor(xtpCLR_MultiDayEventBkGRto));

	m_fcsetSelected.clrBorder.SetStandardValue(pThemeX->GetColor(xtpCLR_MultiDayEventSelectedBorder));
	m_fcsetSelected.grclrBackground.SetStandardValue(m_fcsetNormal.grclrBackground);
}

void CXTPCalendarThemeOffice2007::CTOWeekViewEvent_SingleDay::RefreshMetrics(BOOL bRefreshChildren)
{
	UNREFERENCED_PARAMETER(bRefreshChildren);

	CXTPCalendarThemeOffice2007* pThemeX = DYNAMIC_DOWNCAST(CXTPCalendarThemeOffice2007, GetTheme());
	ASSERT(pThemeX);
	if (!pThemeX)
	{
		return;
	}

	RefreshFromParent(pThemeX->GetWeekViewPartX()->GetEventPartX());
	CFont* pFontDef = pThemeX->m_fntBaseFont;

	m_fcsetNormal.clrBorder.SetStandardValue(pThemeX->GetColor(xtpCLR_WeekViewSingleDayEventBorder));
	m_fcsetNormal.grclrBackground.SetStandardValue(pThemeX->GetColor(xtpCLR_WeekViewSingleDayEventBkGRfrom),
		pThemeX->GetColor(xtpCLR_WeekViewSingleDayEventBkGRto) );

	m_fcsetSelected.clrBorder.SetStandardValue(pThemeX->GetColor(xtpCLR_WeekViewSingleDayEventSelectedBorder));
	m_fcsetSelected.grclrBackground.SetStandardValue(pThemeX->GetColor(xtpCLR_WeekViewSingleDayEventBkGRfrom),
		pThemeX->GetColor(xtpCLR_WeekViewSingleDayEventBkGRto) );

	pThemeX->RemoveBoldAttrFromStd(m_fcsetNormal.fcsetSubject.fntFont);
	pThemeX->RemoveBoldAttrFromStd(m_fcsetSelected.fcsetSubject.fntFont);

	m_fcsetNormal.fcsetStartEnd.clrColor.SetStandardValue(pThemeX->GetColor(xtpCLR_WeekViewEventTime));
	m_fcsetNormal.fcsetStartEnd.fntFont.SetStandardValue(pFontDef);

	m_fcsetSelected.fcsetStartEnd.clrColor.SetStandardValue(pThemeX->GetColor(xtpCLR_WeekViewEventTime));
	m_fcsetSelected.fcsetStartEnd.fntFont.SetStandardValue(pFontDef);
}

void CXTPCalendarThemeOffice2007::CTOWeekViewDay::RefreshMetrics(BOOL bRefreshChildren)
{
	CXTPCalendarThemeOffice2007* pThemeX = DYNAMIC_DOWNCAST(CXTPCalendarThemeOffice2007, GetTheme());
	ASSERT(pThemeX);
	if (!pThemeX)
	{
		return;
	}

	m_clrBorder.SetStandardValue(pThemeX->GetColor(xtpCLR_DayBorder));
	//m_clrTodayBorder.SetStandardValue(XTP_CALENDAR_OFFICE2007_TODAYBORDER);

	m_clrBackgroundLight.SetStandardValue(pThemeX->GetColor(xtpCLR_WeekViewDayBkLight));
	m_clrBackgroundDark.SetStandardValue(pThemeX->GetColor(xtpCLR_WeekViewDayBkDark));
	m_clrBackgroundSelected.SetStandardValue(pThemeX->GetColor(xtpCLR_WeekViewDayBkSelected));

	TBase::RefreshMetrics(bRefreshChildren);

	m_clrTodayBorder.SetStandardValue(GetHeaderPartX()->m_clrTodayBaseColor);
}

void CXTPCalendarThemeOffice2007::CTOWeekViewDayHeader::RefreshMetrics(BOOL bRefreshChildren)
{
	UNREFERENCED_PARAMETER(bRefreshChildren);

	CXTPCalendarThemeOffice2007* pThemeX = DYNAMIC_DOWNCAST(CXTPCalendarThemeOffice2007, GetTheme());
	ASSERT(pThemeX);
	if (!pThemeX)
	{
		return;
	}

	//LOCALE_IDATE  -  short date format ordering
	// 0 Month-Day-Year
	// 1 Day-Month-Year
	// 2 Year-Month-Day
	int nDateOrdering = CXTPCalendarUtils::GetLocaleLong(LOCALE_IDATE);

	if (nDateOrdering == 1)
	{
		m_strHeaderFormat.SetStandardValue(_T("dddd, dd MMMM"));
	}
	else
	{
		m_strHeaderFormat.SetStandardValue(_T("dddd, MMMM dd"));
	}

	RefreshFromParent(pThemeX->GetHeaderPartX());

	m_bkNormal.nBitmapID.SetStandardValue(xtpIMG_WeekViewDayHeader);
	m_bkSelected.nBitmapID.SetStandardValue(xtpIMG_WeekViewDayHeader);

	m_UseOffice2003HeaderFormat.SetStandardValue(FALSE);
}

void CXTPCalendarThemeOffice2007::CTOWeekViewDayHeader::AdjustLayout(CCmdTarget* pObject,
													CDC* pDC, const CRect& rcRect)
{
	//TBase::AdjustLayout(pObject, pDC, rcRect);

	CXTPCalendarThemeOffice2007* pThemeX = DYNAMIC_DOWNCAST(CXTPCalendarThemeOffice2007, GetTheme());
	CXTPCalendarWeekViewDay* pWViewDay = DYNAMIC_DOWNCAST(CXTPCalendarWeekViewDay, pObject);
	if (!pDC || !pThemeX || !pWViewDay || !pWViewDay->GetView())
	{
		ASSERT(FALSE);
		return;
	}

	if (pWViewDay->GetDayIndex() != 0)
	{
		return;
	}


	CXTPCalendarWeekView* pWView = pWViewDay->GetView();
	if (m_UseOffice2003HeaderFormat)
	{
		int nDayWidth = rcRect.Width() - 4;
		pWView->CalculateHeaderFormat(pDC, nDayWidth, m_TextLeftRight.fcsetNormal.Font());
	}
}

void CXTPCalendarThemeOffice2007::CTOWeekViewEvent::AdjustLayout(CCmdTarget* pObject,
		CDC* pDC, const CRect& rcRect, int nEventPlaceNumber)
{
	CXTPCalendarWeekViewEvent* pViewEvent = DYNAMIC_DOWNCAST(CXTPCalendarWeekViewEvent, pObject);

	if (!pDC || !GetTheme() || !pViewEvent || !pViewEvent->GetEvent())
	{
		ASSERT(FALSE);
		return;
	}

	int nEventHeight = XTP_SAFE_GET4(pViewEvent, GetViewGroup(), GetViewDay(), GetView(), GetRowHeight(), 0);

	pViewEvent->m_rcEventMax.top = rcRect.top + nEventPlaceNumber * nEventHeight;
	pViewEvent->m_rcEventMax.bottom = pViewEvent->m_rcEventMax.top + nEventHeight;

	pViewEvent->m_rcEvent.top = pViewEvent->m_rcEventMax.top;
	pViewEvent->m_rcEvent.bottom = pViewEvent->m_rcEventMax.bottom;
	pViewEvent->m_rcEvent.left = pViewEvent->m_rcEventMax.left + 4 + 2;
	pViewEvent->m_rcEvent.right = pViewEvent->m_rcEventMax.right - 5 - 2;

	pViewEvent->m_rcToolTip.CopyRect(pViewEvent->m_rcEventMax);

	// adjust subject area
	int nDyx = pViewEvent->IsSelected() ? 5 : 2;

	pViewEvent->m_rcText.CopyRect(pViewEvent->m_rcEvent);
	pViewEvent->m_rcText.DeflateRect(nDyx + 3, 3, nDyx + 2, 3);

	// set times rects
	pViewEvent->m_rcStartTime.SetRectEmpty();
	pViewEvent->m_rcEndTime.SetRectEmpty();

	pViewEvent->m_rcSubjectEditor.CopyRect(pViewEvent->m_rcEvent);
	pViewEvent->m_rcSubjectEditor.top++;
	pViewEvent->m_rcSubjectEditor.bottom--;
}

void CXTPCalendarThemeOffice2007::CTOWeekViewDay::SetDynColors(CXTPCalendarWeekViewDay* pWViewDay)
{
	CXTPCalendarThemeOffice2007* pThemeX = DYNAMIC_DOWNCAST(CXTPCalendarThemeOffice2007, GetTheme());
	CXTPCalendarThemeOffice2007::CTOCellDynBackground* pCellDynBkPart =
		XTP_SAFE_GET1(pThemeX, GetCellDynBackgroundPart(), NULL);

	if (!pWViewDay || !pCellDynBkPart || !GetTheme() || !GetTheme()->GetColorsSetPart())
	{
		ASSERT(FALSE);
		return;
	}

	CXTPCalendarViewGroup* pViewGroup = pWViewDay->GetViewGroupsCount() ? pWViewDay->GetViewGroup_(0) : NULL;

	CXTPCalendarTheme::XTP_BUSY_STATUS_CONTEXT bsColors;
	m_nDynBusyStatus = pCellDynBkPart->GetCellDynBackground(pViewGroup, pWViewDay->GetDayDate(), -1, bsColors);

	m_clrDynHatch = 0;

	if (m_nDynBusyStatus == xtpCalendarBusyStatusBusy ||
		m_nDynBusyStatus == xtpCalendarBusyStatusOutOfOffice ||
		m_nDynBusyStatus == xtpCalendarBusyStatusTentative)
	{
		if (bsColors.clrBkAllDayEvents != 0 && bsColors.clrCellBorder != 0) // is Category or OutOfOffice
		{
			m_clrDynHatch = bsColors.clrBkAllDayEvents;

			if (m_nDynBusyStatus != xtpCalendarBusyStatusTentative)
			{
				m_clrBackgroundLight = bsColors.clrBkAllDayEvents;
				m_clrBackgroundDark = bsColors.clrBkAllDayEvents;
				m_clrBackgroundSelected = CXTPCalendarThemeOffice2007::PowerColor(bsColors.clrBkAllDayEvents, -7.5, 0.25);
			}
		}
		else
		{
			if (m_nDynBusyStatus == xtpCalendarBusyStatusBusy)
			{
				m_clrBackgroundLight = GetTheme()->GetColorsSetPart()->GetColor(xtpCLR_HeaderBorder); // 141, 174, 217
				m_clrBackgroundDark = (COLORREF)m_clrBackgroundLight;
			}
			m_clrDynHatch = GetTheme()->GetColorsSetPart()->GetColor(xtpCLR_HeaderBorder); // 141, 174, 217;
		}
	}
	else
	{
		m_clrBackgroundLight.SetDefaultValue();
		m_clrBackgroundDark.SetDefaultValue();
		m_clrBackgroundSelected.SetDefaultValue();
	}
}

void CXTPCalendarThemeOffice2007::CTOWeekViewDay::Draw(CCmdTarget* pObject, CDC* pDC)
{
	CXTPCalendarThemeOffice2007* pThemeX = DYNAMIC_DOWNCAST(CXTPCalendarThemeOffice2007, GetTheme());
	CXTPCalendarWeekViewDay* pWViewDay = DYNAMIC_DOWNCAST(CXTPCalendarWeekViewDay, pObject);
	if (!pDC || !pThemeX || !pWViewDay || !pWViewDay->GetView() ||
		pWViewDay->GetViewGroupsCount() == 0 ||
		!pWViewDay->GetViewGroup(0))
	{
		ASSERT(FALSE);
		return;
	}

	CXTPCalendarWeekViewGroup* pWViewGroup = pWViewDay->GetViewGroup(0);
	XTP_VIEW_DAY_LAYOUT& dayLayout = CXTPCalendarTheme::GetViewDayLayout(pWViewDay);

	CRect rcDay = dayLayout.m_rcDay;
	CRect rcBorders = rcDay;
	rcBorders.top = dayLayout.m_rcDayHeader.bottom;

	COleDateTime dtDay = pWViewDay->GetDayDate();
	int nDayCol = GetDayCol(pWViewDay);

	BOOL bIsToday = GetHeaderPart()->IsToday(pWViewDay);
	BOOL bIsSelected = GetHeaderPart()->IsSelected(pWViewDay);

	// Draw background
	int nLiteBkColorIdx = 0;
	// for non-work days choose dark background
	if (pWViewDay->GetCalendarControl() &&
		pWViewDay->GetCalendarControl()->GetCalendarOptions())
	{
		int nMask = pWViewDay->GetCalendarControl()->GetCalendarOptions()->nWorkWeekMask;
		int nWeekDay = dtDay.GetDayOfWeek();
		if ((CXTPCalendarUtils::GetDayOfWeekMask(nWeekDay) & nMask) != 0)
		{
			nLiteBkColorIdx = 1;
		}
	}
	// for selected days choose another background
	COLORREF clrBackground = nLiteBkColorIdx ? m_clrBackgroundLight : m_clrBackgroundDark;
	if (bIsSelected)
	{
		clrBackground = m_clrBackgroundSelected;
	}

	// draw background
	pDC->FillSolidRect(rcDay, clrBackground);

	if (m_nDynBusyStatus == xtpCalendarBusyStatusTentative)
	{
		CXTPTextDC autoTxt(pDC, COLORREF_NULL, clrBackground);
		CBrush br(HS_BDIAGONAL, m_clrDynHatch);

		pDC->FillRect(&rcDay, &br);
	}

	if (bIsToday)
	{
		CXTPCalendarTheme::DrawRectPartly(pDC, rcBorders, m_clrTodayBorder, 2, CRect(1, 0, 0, 1));
		CXTPCalendarTheme::DrawRectPartly(pDC, rcBorders, m_clrTodayBorder, 1, CRect(0, 0, 1, 0));
		//CXTPCalendarTheme::DrawRectPartly(pDC, rcBorders, m_clrBorder, 1, CRect(0, 0, 1, 0));
	}
	else
	{
		int nIsRBorder = nDayCol;
		int nIsLBorder = nDayCol;

		CXTPCalendarTheme::DrawRectPartly(pDC, rcBorders, m_clrBorder, 1, CRect(nIsLBorder, 0, nIsRBorder, 0));
		CXTPCalendarTheme::DrawRectPartly(pDC, rcBorders, m_clrBorder, 2, CRect(0, 0, 0, 1));
	}
	GetHeaderPart()->Draw(pWViewDay, pDC);

	BOOL bCnt_EvS = FALSE;
	BOOL bCnt_EvMul = FALSE;

	CXTPPropsStateContext autoStateCnt_EvS;
	CXTPPropsStateContext autoStateCnt_EvMul;

	int nCount = pWViewGroup->GetViewEventsCount();
	for (int i = 0; i < nCount; i++)
	{
		CXTPCalendarViewEvent* pViewEvent = pWViewGroup->GetViewEvent_(i);
		ASSERT(pViewEvent);
		if (pViewEvent && pViewEvent->IsVisible())
		{
			if (pViewEvent->IsMultidayEvent())
			{
				if (!bCnt_EvMul)
					autoStateCnt_EvMul.SetData(GetMultiDayEventPartX(), xtpCalendarBeforeDraw_WeekViewEvent, TRUE);
				bCnt_EvMul = TRUE;

				autoStateCnt_EvMul.SendBeforeDrawThemeObject((LPARAM)pViewEvent);

				GetMultiDayEventPartX()->Draw(pViewEvent, pDC);

				autoStateCnt_EvMul.RestoreState();
			}
			else
			{
				if (!bCnt_EvS)
					autoStateCnt_EvS.SetData(GetSingleDayEventPartX(), xtpCalendarBeforeDraw_WeekViewEvent, TRUE);
				bCnt_EvS = TRUE;

				autoStateCnt_EvS.SendBeforeDrawThemeObject((LPARAM)pViewEvent);

				GetSingleDayEventPartX()->Draw(pViewEvent, pDC);

				autoStateCnt_EvS.RestoreState();
			}
		}
	}

	if (!pDC->IsPrinting() && pWViewDay->NoAllEventsAreVisible())
	{
		CRect rc = dayLayout.m_rcDay;
		rc.right -= 5;
		rc.bottom -= 5;

		BOOL bHot = _HitTestExpandDayButton(pWViewDay, rc) != 0;

		dayLayout.m_rcExpandSign = pThemeX->DrawExpandDayButton(pDC, rc, FALSE, bHot);
		if (bHot)
			dayLayout.m_nHotState |= XTP_VIEW_DAY_LAYOUT::xtpHotExpandButton;
		else
			dayLayout.m_nHotState &= ~XTP_VIEW_DAY_LAYOUT::xtpHotExpandButton;
	}

	autoStateCnt_EvS.Clear();
	autoStateCnt_EvMul.Clear();
}

void CXTPCalendarThemeOffice2007::CTOWeekViewDay::OnMouseMove(CCmdTarget* pObject, UINT nFlags, CPoint point)
{
	UNREFERENCED_PARAMETER(nFlags);

	CXTPCalendarThemeOffice2007* pThemeX = DYNAMIC_DOWNCAST(CXTPCalendarThemeOffice2007, GetTheme());
	CXTPCalendarWeekViewDay* pWViewDay = DYNAMIC_DOWNCAST(CXTPCalendarWeekViewDay, pObject);
	if (!pThemeX || !pWViewDay || !pThemeX->GetCalendarControl())
	{
		ASSERT(FALSE);
		return;
	}

	XTP_VIEW_DAY_LAYOUT& dayLayout = CXTPCalendarTheme::GetViewDayLayout(pWViewDay);

	if (pWViewDay->NoAllEventsAreVisible())
	{
		BOOL bHot = pThemeX->ExpandDayButton_HitTest(point, dayLayout.m_rcExpandSign, FALSE);
		BOOL bHotPrev = !!(dayLayout.m_nHotState & XTP_VIEW_DAY_LAYOUT::xtpHotExpandButton);

		if (bHot != bHotPrev)
		{
			CXTPCalendarControl::CUpdateContext updateContext(pThemeX->GetCalendarControl(), xtpCalendarUpdateRedraw);
		}
	}

	BOOL bHotPrev = !!(dayLayout.m_nHotState & XTP_VIEW_DAY_LAYOUT::xtpHotDayHeader);
	if (dayLayout.m_rcDayHeader.PtInRect(point) != bHotPrev)
	{
		CXTPCalendarControl::CUpdateContext updateContext(pThemeX->GetCalendarControl(), xtpCalendarUpdateRedraw);
	}
}

BOOL CXTPCalendarThemeOffice2007::CTOWeekViewDay::OnLButtonDown(CCmdTarget* pObject, UINT nFlags, CPoint point)
{
	UNREFERENCED_PARAMETER(nFlags);

	CXTPCalendarThemeOffice2007* pThemeX = DYNAMIC_DOWNCAST(CXTPCalendarThemeOffice2007, GetTheme());
	CXTPCalendarWeekViewDay* pViewDay = DYNAMIC_DOWNCAST(CXTPCalendarWeekViewDay, pObject);
	if (!pThemeX || !pViewDay || !pThemeX->GetCalendarControl())
	{
		ASSERT(FALSE);
		return FALSE;
	}
	XTP_VIEW_DAY_LAYOUT& dayLayout = CXTPCalendarTheme::GetViewDayLayout(pViewDay);

	if (pViewDay->NoAllEventsAreVisible())
	{
		if (pThemeX->ExpandDayButton_HitTest(point, dayLayout.m_rcExpandSign, FALSE))
		{
			pThemeX->GetCalendarControl()->PostMessage(xtp_wm_UserAction,
				(WPARAM)(xtpCalendarUserAction_OnExpandDay | xtpCalendarExpandDayButton_WeekView),
				(LPARAM)(LONG)(DATE)pViewDay->GetDayDate());

			return TRUE;
		}
	}

	return FALSE;
}

void CXTPCalendarThemeOffice2007::CTOWeekViewDay::DoPropExchange(CXTPPropExchange* pPX)
{
	ASSERT(pPX);
	if (!pPX)
		return;

	CXTPPropExchangeSection secData(pPX->GetSection(_T("WeekViewDay")));
	if (pPX->IsStoring())
		secData->EmptySection();

	PX_Color(&secData, _T("BorderColor"),       m_clrBorder);
	PX_Color(&secData, _T("TodayBorderColor"),  m_clrTodayBorder);

	PX_Color(&secData, _T("BackgroundLightColor"),   m_clrBackgroundLight);
	PX_Color(&secData, _T("BackgroundDarkColor"),    m_clrBackgroundDark);
	PX_Color(&secData, _T("BackgroundSelectedColor"), m_clrBackgroundSelected);

	TBase::DoPropExchange(&secData);
}

void CXTPCalendarThemeOffice2007::CTOWeekViewDay::Serialize(CArchive& ar)
{
	m_clrBorder.Serialize(ar);
	m_clrTodayBorder.Serialize(ar);

	m_clrBackgroundLight.Serialize(ar);
	m_clrBackgroundDark.Serialize(ar);
	m_clrBackgroundSelected.Serialize(ar);

	TBase::Serialize(ar);
}


void CXTPCalendarThemeOffice2007::CTOWeekViewDayHeader::Draw(CCmdTarget* pObject, CDC* pDC)
{
	CXTPCalendarThemeOffice2007* pThemeX = DYNAMIC_DOWNCAST(CXTPCalendarThemeOffice2007, GetTheme());
	CXTPCalendarWeekViewDay* pWViewDay = DYNAMIC_DOWNCAST(CXTPCalendarWeekViewDay, pObject);
	if (!pDC || !pThemeX || !pWViewDay || !pWViewDay->GetView())
	{
		ASSERT(FALSE);
		return;
	}

	XTP_VIEW_DAY_LAYOUT& dayLayout = CXTPCalendarTheme::GetViewDayLayout(pWViewDay);
	COleDateTime dtDay = pWViewDay->GetDayDate();
	int nDayCol = pThemeX->GetWeekViewPart()->GetDayPart()->GetDayCol(pWViewDay);

	CPoint pt(0, 0);
	if (pThemeX->GetCalendarControl()->GetSafeHwnd() && GetCursorPos(&pt))
		pThemeX->GetCalendarControl()->ScreenToClient(&pt);

	BOOL bIsToday    = IsToday(pWViewDay);
	BOOL bIsSelected = IsSelected(pWViewDay);
	BOOL bFirstDay = nDayCol == 0;
	BOOL bLastDay = nDayCol == 1;

	int nFlags = bIsToday ? CTOHeader::xtpCalendarItemState_Today : CTOHeader::xtpCalendarItemState_Normal;
	nFlags |= bIsSelected ? CTOHeader::xtpCalendarItemState_Selected : 0;
	if (dayLayout.m_rcDayHeader.PtInRect(pt))
	{
		nFlags |=  CTOHeader::xtpCalendarItemState_Hot;
		dayLayout.m_nHotState |= XTP_VIEW_DAY_LAYOUT::xtpHotDayHeader;
	}
	else
	{
		dayLayout.m_nHotState &= ~XTP_VIEW_DAY_LAYOUT::xtpHotDayHeader;
	}

	nFlags |= bFirstDay ? CTOHeader::xtpCalendarItemFirst : 0;
	nFlags |= bLastDay ? CTOHeader::xtpCalendarItemLast : 0;

	CRect rcRect(dayLayout.m_rcDay);
	rcRect.bottom = dayLayout.m_rcDayHeader.bottom;

	CString strTextLeft;
	CString strTextCenter;
	CString strTextRight;

	if (m_UseOffice2003HeaderFormat)
	{
		strTextRight = pWViewDay->GetCaption();
	}
	else
	{
		strTextRight = CXTPCalendarUtils::GetDateFormat(dtDay, m_strHeaderFormat);

		pThemeX->GetItemTextIfNeed(xtpCalendarItemText_WeekViewDayHeaderLeft, &strTextLeft, pWViewDay);
		pThemeX->GetItemTextIfNeed(xtpCalendarItemText_WeekViewDayHeaderCenter, &strTextCenter, pWViewDay);
		pThemeX->GetItemTextIfNeed(xtpCalendarItemText_WeekViewDayHeaderRight, &strTextRight, pWViewDay);
	}

	Draw_Header(pDC, rcRect, nFlags, strTextLeft, strTextCenter, strTextRight);
}

void CXTPCalendarThemeOffice2007::CTOWeekViewDayHeader::DoPropExchange(CXTPPropExchange* pPX)
{
	ASSERT(pPX);
	if (!pPX)
		return;

	CXTPPropExchangeSection secData(pPX->GetSection(_T("WeekViewDayHeader")));
	if (pPX->IsStoring())
		secData->EmptySection();

	m_strHeaderFormat.DoPropExchange(&secData, _T("HeaderFormat"));

	if (pPX->GetSchema() >= _XTP_SCHEMA_112)
		m_UseOffice2003HeaderFormat.DoPropExchange(&secData, _T("UseOffice2003HeaderFormat"));

	TBase::DoPropExchange(&secData);
}

void CXTPCalendarThemeOffice2007::CTOWeekViewDayHeader::Serialize(CArchive& ar)
{
	m_strHeaderFormat.Serialize(ar);
	m_UseOffice2003HeaderFormat.Serialize(ar);

	TBase::Serialize(ar);
}

void CXTPCalendarThemeOffice2007::CTOWeekViewEvent_MultiDay::GetMultidayTimeFontAndColor(CFont*& pFont, COLORREF& clrColor, BOOL bSelected)
{
	pFont = bSelected ? m_fcsetSelected.fcsetSubject.Font() : m_fcsetNormal.fcsetSubject.Font();
	clrColor = bSelected ? m_fcsetSelected.fcsetSubject.Color() : m_fcsetNormal.fcsetSubject.Color();
}

void CXTPCalendarThemeOffice2007::CTOWeekViewEvent_MultiDay::Draw(CCmdTarget* pObject, CDC* pDC)
{
	CXTPCalendarThemeOffice2007* pThemeX = DYNAMIC_DOWNCAST(CXTPCalendarThemeOffice2007, GetTheme());
	CXTPCalendarWeekViewEvent* pViewEvent = DYNAMIC_DOWNCAST(CXTPCalendarWeekViewEvent, pObject);

	CXTPCalendarWeekViewDay* pMVDay = XTP_SAFE_GET2(pViewEvent, GetViewGroup(), GetViewDay(), NULL);
	CXTPCalendarWeekView* pMView = XTP_SAFE_GET1(pMVDay, GetView(), NULL);

	if (!pDC || !pThemeX || !pViewEvent || !pViewEvent->GetEvent() ||
		!pMView || !pMVDay)
	{
		ASSERT(FALSE);
		return;
	}

	BOOL bNoVisibleMDEvent = pViewEvent->IsMultidayEvent() && !pViewEvent->IsVisible();

	int nMDEFlags = pViewEvent->GetMultiDayEventFlags();
	ASSERT(0 == (nMDEFlags & xtpCalendarMultiDaySlave)); // error for week view.

	if (bNoVisibleMDEvent)
	{
		return;
	}

	CRect rcEventMax(pViewEvent->GetViewEventRectMax());
	CRect rcEvent(pViewEvent->GetEventRect());
	if (rcEvent.IsRectEmpty())
	{
		return;
	}

	// fill event background
	BOOL bSelected = pViewEvent->IsSelected();
	COLORREF clrBorderNotSel;
	COLORREF clrBorder;
	CXTPPaintManagerColorGradient grclrBk;

	GetEventColors(pDC, pViewEvent, clrBorder, clrBorderNotSel, grclrBk);

	pThemeX->DrawEventRect(pDC, rcEvent, clrBorder, bSelected, grclrBk);

	int nViewDayIndex = pMVDay->GetDayIndex();

	BOOL bFirstOnScreen = nViewDayIndex == 0;
	BOOL bLastOnScreen = nViewDayIndex == 6;

	CRect rcText(pViewEvent->GetTextRect());

	if ((nMDEFlags & xtpCalendarMultiDayFirst) == 0)
	{
		CRect rcSQ = rcEventMax;
		rcSQ.left += min(1, rcSQ.Width());

		rcSQ.right = min(rcEvent.left + cnCornerSize, rcEventMax.right);
		CXTPCalendarTheme::DrawRectPartly(pDC, rcSQ, clrBorder, bSelected, grclrBk, CRect(0,1,0,1));

		if (bFirstOnScreen)
		{
			CRect rcArrow;
			BOOL bArrow = Draw_ArrowL(pViewEvent, pDC, rcArrow);
			if (bArrow)
				rcText.left = rcArrow.right;
		}
	}
	else if (bSelected)
	{
		// draw left Gripper
		CRect rcGripper;
		rcGripper.top = rcEvent.top + rcEvent.Height() / 2 - cnGripperSize / 2;
		rcGripper.bottom = rcGripper.top + cnGripperSize;
		rcGripper.left = rcEvent.left - 1;
		rcGripper.right = rcGripper.left + cnGripperSize;

		pDC->FillSolidRect(&rcGripper, m_clrGripperBackground);
		CBrush brBorder(m_clrGripperBorder);
		pDC->FrameRect(&rcGripper, &brBorder);
	}


	if ((nMDEFlags & xtpCalendarMultiDayLast) == 0)
	{
		CRect rcSQ = rcEventMax;
		rcSQ.left = max(rcEvent.right - cnCornerSize, rcEventMax.left);
		rcSQ.right -= min(2, rcSQ.Width());
		CXTPCalendarTheme::DrawRectPartly(pDC, rcSQ, clrBorder, bSelected, grclrBk, CRect(0,1,0, 1));

		if (bLastOnScreen)
		{
			CRect rcArrow;
			BOOL bArrow = Draw_ArrowR(pViewEvent, pDC, rcArrow);
			if (bArrow)
				rcText.right = rcArrow.left;
		}
	}
	else if (bSelected)
	{
		// draw right gripper
		CRect rcGripper;
		rcGripper.top = rcEvent.top + rcEvent.Height() / 2 - cnGripperSize / 2;
		rcGripper.bottom = rcGripper.top + cnGripperSize;
		rcGripper.right = rcEvent.right + 1;
		rcGripper.left = rcGripper.right - cnGripperSize;

		pDC->FillSolidRect(&rcGripper, m_clrGripperBackground);
		CBrush brBorder(m_clrGripperBorder);
		pDC->FrameRect(&rcGripper, &brBorder);
	}

	MultidayDrawTime(nMDEFlags, pViewEvent, pDC, rcEvent, pViewEvent->IsTimeAsClock(), pViewEvent->IsTimeAsClock(), rcText);

	//****
	pViewEvent->m_rcText.CopyRect(rcText);

	//=====================================
	CString strSubj = pViewEvent->GetItemTextSubject();
	CString strLoc = pViewEvent->GetItemTextLocation();

	if (!strLoc.IsEmpty())
	{
		strSubj += _T(";");
	}

	CThemeFontColorSet* pFntClr1 = bSelected ? &m_fcsetSelected.fcsetSubject : &m_fcsetNormal.fcsetSubject;
	CThemeFontColorSet* pFntClr2 = bSelected ? &m_fcsetSelected.fcsetLocation : &m_fcsetNormal.fcsetLocation;
	rcText.DeflateRect(3, 0, 3, 0);

	pViewEvent->m_rcSubjectEditor.CopyRect(rcText);
	pViewEvent->m_rcSubjectEditor.top = rcEvent.top + 2;
	pViewEvent->m_rcSubjectEditor.bottom = rcEvent.bottom - 2;
	pViewEvent->AdjustSubjectEditor();

	CRect rcIconsMax = rcText;
	rcIconsMax.left = min(rcIconsMax.left + rcText.Height(), rcIconsMax.right);

	//
	BOOL bMarkupDrawn = FALSE;
	if (pThemeX->GetCalendarControl()->IsMarkupEnabled())
	{
		CXTPMarkupContext* pMarkupContext = pThemeX->GetCalendarControl()->GetMarkupContext();
		ASSERT(pMarkupContext);

		CXTPMarkupUIElement* pUIElement = pViewEvent->GetMarkupUIElement();

		if (pUIElement && pMarkupContext)
		{
			CSize szIcons = Draw_Icons(pViewEvent, pDC, rcIconsMax, TRUE);

			if (rcIconsMax.Height() > szIcons.cy)
			{
				rcIconsMax.bottom -= (rcIconsMax.Height() - szIcons.cy) / 2 + 1;
			}

			rcText.right = max(rcText.left,  rcText.right - szIcons.cx - 2);

			XTPMarkupSetDefaultFont(pMarkupContext, (HFONT)pFntClr1->Font()->GetSafeHandle(), pFntClr1->Color());

			XTPMarkupRenderElement(pUIElement, pDC->GetSafeHdc(), rcText);

			bMarkupDrawn = TRUE;
		}
	}

	if (!bMarkupDrawn)
	{
		UINT uTextShortAlign = DT_VCENTER | DT_LEFT;
		CSize szText = pThemeX->DrawLine2_CenterLR(pDC, strSubj, strLoc, pFntClr1, pFntClr2,
									rcText, uTextShortAlign | DT_CALCRECT);

		//--- calculate rects and fix to be centered if enough space ---
		CSize szIcons = Draw_Icons(pViewEvent, pDC, rcIconsMax, TRUE);

		if (rcIconsMax.Height() > szIcons.cy)
		{
			rcIconsMax.bottom -= (rcIconsMax.Height() - szIcons.cy) / 2;
		}

		int nTIcx = szText.cx + szIcons.cx;
		if (nTIcx + 5 < rcText.Width())
		{
			int ncx_2 = (rcText.Width() - nTIcx) / 2 - 2;
			rcText.left += ncx_2;
			rcText.right = rcText.left + szText.cx + 2;

			rcIconsMax.right -= ncx_2;
		}
		else
		{
			rcText.right -= szIcons.cx + 2;
		}

		pThemeX->DrawLine2_CenterLR(pDC, strSubj, strLoc, pFntClr1, pFntClr2,
			rcText, uTextShortAlign);
	}

	Draw_Icons(pViewEvent, pDC, rcIconsMax);
}

void CXTPCalendarThemeOffice2007::CTOWeekViewEvent_SingleDay::Draw(CCmdTarget* pObject, CDC* pDC)
{
	CXTPCalendarThemeOffice2007* pThemeX = DYNAMIC_DOWNCAST(CXTPCalendarThemeOffice2007, GetTheme());
	CXTPCalendarWeekViewEvent* pViewEvent = DYNAMIC_DOWNCAST(CXTPCalendarWeekViewEvent, pObject);
	if (!pDC || !pThemeX || !pViewEvent)
	{
		ASSERT(FALSE);
		return;
	}

	if (!pViewEvent->IsVisible())
	{
		return;
	}

	CRect rcEvent(pViewEvent->GetEventRect());
	if (rcEvent.IsRectEmpty())
	{
		return;
	}

	DWORD dwAdditionalOpt = 0;
	if (pViewEvent->GetCalendarControl() &&
		pViewEvent->GetCalendarControl()->GetCalendarOptions())
		dwAdditionalOpt = pViewEvent->GetCalendarControl()->GetCalendarOptions()->dwAdditionalOptions;
	BOOL bShowStartTime = dwAdditionalOpt & xtpCalendarOptWeekViewShowStartTimeAlways;
	BOOL bShowEndTime = dwAdditionalOpt & xtpCalendarOptWeekViewShowEndTimeAlways;

	// fill event background
	Draw_Background(pDC, rcEvent, pViewEvent);

	//
	BOOL bMarkupDrawn = FALSE;
	if (pThemeX->GetCalendarControl()->IsMarkupEnabled())
	{
		CXTPMarkupContext* pMarkupContext = pThemeX->GetCalendarControl()->GetMarkupContext();
		ASSERT(pMarkupContext);

		CXTPMarkupUIElement* pUIElement = pViewEvent->GetMarkupUIElement();

		if (pUIElement && pMarkupContext)
		{
			BOOL bSelected = pViewEvent->IsSelected();
			CThemeFontColorSet* pFntClr1 = bSelected ? &m_fcsetSelected.fcsetSubject : &m_fcsetNormal.fcsetSubject;

			CRect rcText = rcEvent;
			rcText.DeflateRect(3, 2, 3, 2);
			CRect rcIconsMax = rcText;

			CSize szIcons = Draw_Icons(pViewEvent, pDC, rcIconsMax, TRUE);

			if (rcIconsMax.Height() > szIcons.cy)
			{
				rcIconsMax.bottom -= (rcIconsMax.Height() - szIcons.cy) / 2 + 1;
			}

			rcText.right = max(rcText.left,  rcText.right - szIcons.cx - 2);

			XTPMarkupSetDefaultFont(pMarkupContext, (HFONT)pFntClr1->Font()->GetSafeHandle(), pFntClr1->Color());

			XTPMarkupRenderElement(pUIElement, pDC->GetSafeHdc(), rcText);

			bMarkupDrawn = TRUE;

			Draw_Icons(pViewEvent, pDC, rcIconsMax);
		}
	}
	//

	if (!bMarkupDrawn)
	{
		if (bShowStartTime || bShowEndTime)
			Draw_Time(pDC, rcEvent, pViewEvent);

		CRect rcText(pViewEvent->GetTextRect());
		Draw_Caption(pDC, rcText, pViewEvent);
	}
}

void CXTPCalendarThemeOffice2007::CTOWeekViewEvent_SingleDay::
		Draw_Background(CDC* pDC, const CRect& rcEventRect, CXTPCalendarWeekViewEvent* pViewEvent)
{
	CXTPCalendarThemeOffice2007* pThemeX = DYNAMIC_DOWNCAST(CXTPCalendarThemeOffice2007, GetTheme());

	if (!pDC || !pThemeX || !pViewEvent || !pViewEvent->GetEvent())
	{
		ASSERT(FALSE);
		return;
	}
	BOOL bSelected = pViewEvent->IsSelected();

	COLORREF clrBorderNotSel, clrBorder;
	CXTPPaintManagerColorGradient grclrBk;
	int nColorsSrc_1Cat2Lbl = 0;
	GetEventColors(pDC, pViewEvent, clrBorder, clrBorderNotSel, grclrBk, bSelected, &nColorsSrc_1Cat2Lbl);

	if (!bSelected && nColorsSrc_1Cat2Lbl > 0)
	{
		clrBorder = grclrBk.clrLight;
	}

	pThemeX->DrawEventRect(pDC, rcEventRect, clrBorder, bSelected, grclrBk);
}

CSize CXTPCalendarThemeOffice2007::CTOWeekViewEvent_SingleDay::Draw_Time(
		CDC* pDC, const CRect& rcEventRect, CXTPCalendarWeekViewEvent* pViewEvent)
{
	CSize szResult(0, 0);

	if (!pDC || !pViewEvent || !pViewEvent->GetEvent())
	{
		ASSERT(FALSE);
		return szResult;
	}

	BOOL bSelected = pViewEvent->IsSelected();

	CFont* pFont = bSelected ? m_fcsetSelected.fcsetStartEnd.Font() : m_fcsetNormal.fcsetStartEnd.Font();
	COLORREF clrColor = bSelected ? m_fcsetSelected.fcsetStartEnd.Color() : m_fcsetNormal.fcsetStartEnd.Color();

	CXTPFontDC autoFont(pDC, pFont, clrColor);

	CString strStartTime = pViewEvent->GetItemTextStartTime();
	CString strEndTime = pViewEvent->GetItemTextEndTime();

	CSize szStart = pDC->GetTextExtent(strStartTime);
	CSize szEnd = pDC->GetTextExtent(strEndTime);

	CRect rcStart(rcEventRect);
	CRect rcEnd(rcEventRect);

	COleDateTime dtStart = pViewEvent->GetEvent()->GetStartTime();
	COleDateTime dtEnd = pViewEvent->GetEvent()->GetEndTime();

	DWORD dwOpt = XTP_SAFE_GET3(GetTheme(), GetCalendarControl(), GetCalendarOptions(), dwAdditionalOptions, 0);
	BOOL bShowEndTime = XTP_SAFE_GET2(pViewEvent, GetCalendarControl(), WeekView_IsShowEndDate(), TRUE);
	BOOL bDrawStartTime = dwOpt & xtpCalendarOptWeekViewShowStartTimeAlways;
	BOOL bDrawEndTime = bShowEndTime && (dwOpt & xtpCalendarOptWeekViewShowEndTimeAlways);

	if (!bDrawStartTime && !bDrawEndTime)
		return szResult;

	int iOffset(0);

	UINT uAlign = DT_VCENTER | DT_SINGLELINE | DT_LEFT;
	if (bDrawStartTime)
	{
		if (pViewEvent->IsTimeAsClock())
		{
			iOffset = DrawClock(pDC, dtStart, rcStart, RGB(224,224,224), xtpCalendarClockAlignLeft);
			rcStart.left += iOffset + 5;
			rcStart.right = rcStart.left;

			rcEnd.left = rcStart.right;
			rcEnd.right = rcStart.right;
			rcEnd.right += szEnd.cx;
		}
		else
		{
			rcStart.left = min(rcStart.left + 4, rcStart.right);
			rcStart.right = min(rcStart.left + szStart.cx + 6, rcStart.right);

			pDC->DrawText(strStartTime, &rcStart, uAlign);
		}
	}
	else
	{
		rcStart.right = rcStart.left + 5;
	}

	if (bDrawEndTime)
	{
		if (pViewEvent->IsTimeAsClock())
		{
			iOffset = DrawClock(pDC, dtEnd, rcEnd, RGB(224,224,224), xtpCalendarClockAlignLeft);
			rcEnd.left += iOffset;
			rcEnd.right = rcEnd.left + 5;
		}
		else
		{
			rcEnd.left = rcStart.right;
			rcEnd.right = min(rcEnd.left + szEnd.cx + 6, rcEnd.right);

			pDC->DrawText(strEndTime, &rcEnd, uAlign);
		}
	}
	else
	{
		rcEnd.left = rcStart.left;
		rcEnd.right = rcStart.right;
	}
	pViewEvent->m_rcStartTime.CopyRect(&rcStart);
	pViewEvent->m_rcEndTime.CopyRect(&rcEnd);

	pViewEvent->m_rcText.left = rcEnd.right;

	szResult.cx = max(0, rcEnd.right - rcStart.left);
	szResult.cy = szStart.cy;

	return szResult;
}

void CXTPCalendarThemeOffice2007::CTOWeekViewEvent_SingleDay::Draw_Caption(
		CDC* pDC, const CRect& rcTextRect, CXTPCalendarWeekViewEvent* pViewEvent)
{
	CXTPCalendarThemeOffice2007* pThemeX = DYNAMIC_DOWNCAST(CXTPCalendarThemeOffice2007, GetTheme());
	if (!pDC || !pViewEvent || !pThemeX )
	{
		ASSERT(FALSE);
		return;
	}

	CString strSubj = pViewEvent->GetItemTextSubject();
	CString strLoc = pViewEvent->GetItemTextLocation();

	if (!strLoc.IsEmpty())
	{
		strSubj += _T(";");
	}

	BOOL bSelected = pViewEvent->IsSelected();
	CThemeFontColorSet* pFntClr1 = bSelected ? &m_fcsetSelected.fcsetSubject : &m_fcsetNormal.fcsetSubject;
	CThemeFontColorSet* pFntClr2 = bSelected ? &m_fcsetSelected.fcsetLocation : &m_fcsetNormal.fcsetLocation;

	CRect rcText = rcTextRect;
	//rcText.DeflateRect(1, 2, 3, 2);

	pViewEvent->m_rcSubjectEditor.CopyRect(rcText);
	pViewEvent->AdjustSubjectEditor();

	UINT nAlign = DT_VCENTER | DT_LEFT | DT_SINGLELINE;
	CSize szText = pThemeX->DrawLine2_CenterLR(pDC, strSubj, strLoc, pFntClr1, pFntClr2,
								rcText, nAlign  | DT_CALCRECT, nAlign);

	CRect rcIconsMax = rcText;
	rcIconsMax.left = min(rcIconsMax.left + rcTextRect.Height(), rcIconsMax.right);

	//--- calculate rects and fix to be centered if enough space ---
	CSize szIcons = Draw_Icons(pViewEvent, pDC, rcIconsMax, TRUE);

	if (rcIconsMax.Height() > szIcons.cy)
	{
		rcIconsMax.bottom -= (rcIconsMax.Height() - szIcons.cy) / 2;
	}

	int nTIcx = szText.cx + szIcons.cx;
	if (nTIcx > rcText.Width())
	{
		rcText.right = max(rcIconsMax.left, rcIconsMax.right - szIcons.cx) - 2;
	}

	pThemeX->DrawLine2_CenterLR(pDC, strSubj, strLoc, pFntClr1, pFntClr2,
								rcText, nAlign, nAlign);

	Draw_Icons(pViewEvent, pDC, rcIconsMax);
}



/////////////////////////////////////////////////////////////////////////////
#ifdef _XTP_ACTIVEX
//===========================================================================
BEGIN_DISPATCH_MAP(CXTPCalendarThemeOffice2007, CXTPCalendarTheme)
	// some properties and methods are inherited from the base class.
	// See also: CXTPCalendarTheme DISPATCH_MAP implementation

	DISP_FUNCTION_ID(CXTPCalendarThemeOffice2007, "Header",     50, OleGet_Header,    VT_DISPATCH, VTS_NONE)
	DISP_FUNCTION_ID(CXTPCalendarThemeOffice2007, "Event",      51, OleGet_Event,     VT_DISPATCH, VTS_NONE)

	DISP_FUNCTION_ID(CXTPCalendarThemeOffice2007, "DayView",    52, OleGet_DayView,   VT_DISPATCH, VTS_NONE)
	DISP_FUNCTION_ID(CXTPCalendarThemeOffice2007, "MonthView",  53, OleGet_MonthView, VT_DISPATCH, VTS_NONE)
	DISP_FUNCTION_ID(CXTPCalendarThemeOffice2007, "WeekView",   54, OleGet_WeekView,  VT_DISPATCH, VTS_NONE)

	DISP_FUNCTION_ID(CXTPCalendarThemeOffice2007, "PrevNextEventButtons", 55, OleGet_PrevNextEventButtons, VT_DISPATCH, VTS_NONE)

END_DISPATCH_MAP()

// {4B255059-5D67-4d85-80AA-B544F1F34305}
//{ 0x4b255059, 0x5d67, 0x4d85, { 0x80, 0xaa, 0xb5, 0x44, 0xf1, 0xf3, 0x43, 0x5 } };
static const GUID IID_ICalendarThemeOffice2007 =
{ 0x4b255059, 0x5d67, 0x4d85, { 0x80, 0xaa, 0xb5, 0x44, 0xf1, 0xf3, 0x43, 0x5 } };

BEGIN_INTERFACE_MAP(CXTPCalendarThemeOffice2007, CXTPCalendarTheme)
	INTERFACE_PART(CXTPCalendarThemeOffice2007, IID_ICalendarThemeOffice2007, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPCalendarThemeOffice2007, IID_ICalendarThemeOffice2007)

//==============
#endif
