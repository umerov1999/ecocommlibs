// XTPCalendarThemePrevNextEventButton.cpp : implementation file
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

#include <StdAfx.h>

#include <Common/XTPResourceManager.h>
#include <Common/XTPResourceImage.h>
#include <Common/XTPResourceImageList.h>
#include <Common/XTPDrawHelpers.h>
#include <Common/XTPColorManager.h>
#include <Common/XTPNotifyConnection.h>
#include <Common/XTPSmartPtrInternalT.h>
#include <Common/XTPVC80Helpers.h>

#include <Calendar/Resource.h>
#include <Calendar/XTPCalendarPtrCollectionT.h>
#include <Calendar/XTPCalendarDefines.h>
#include <Calendar/XTPCalendarView.h>
#include <Calendar/XTPCalendarViewEvent.h>
#include <Calendar/XTPCalendarViewDay.h>
#include <Calendar/XTPCalendarViewPart.h>
#include <Calendar/XTPCalendarDayViewEvent.h>
#include <Calendar/XTPCalendarWeekViewEvent.h>
#include <Calendar/XTPCalendarMonthViewEvent.h>
#include <Calendar/XTPCalendarControl.h>
#include <Calendar/XTPCalendarTheme.h>
#include <Calendar/XTPCalendarThemePrevNextEventButton.h>
#include <Calendar/XTPCalendarThemePrevNextEventButtons.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CTOPrevNextEventButton::CTOPrevNextEventButton()
{
	m_bPrev = TRUE;
	m_rcRect.SetRect(0,0,0,0);

	m_bHot = FALSE;
	m_bDisabled = FALSE;
	m_bVisible = TRUE;
}

CTOPrevNextEventButton::~CTOPrevNextEventButton()
{

}

void CTOPrevNextEventButton::RefreshMetrics(BOOL /*bRefreshChildren*/)
{
	m_bPrev = 0 == _tcsnicmp(GetInstanceName(), _T("Prev"), 4);

	m_nBitmapID.SetStandardValue(CXTPCalendarTheme::xtpIMG_PrevNextEventButtons) ;
	m_rcBitmapBorder.SetStandardValue(CRect(1, 8, 1, 8));

	if (!GetTheme())
		return;

	LOGFONT lfVert;
	VERIFY( GetTheme()->m_fntBaseFont->GetLogFont(&lfVert) );
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

	m_fcsetText.fntFont.SetStandardValue(&lfVert);
	m_fcsetText.clrColor.SetStandardValue(RGB(0, 0, 0));
	m_clrDisabledText.SetStandardValue(RGB(136, 128, 128));

	m_strText.SetStandardValue(XTPResourceManager()->LoadString(m_bPrev ? XTP_IDS_CALENDAR_PREVAPPOINTMENT : XTP_IDS_CALENDAR_NEXTAPPOINTMENT));
}

CSize CTOPrevNextEventButton::GetSize(const CRect* prcRect)
{
	CSize szSize(23, 172);

	if (prcRect)
	{
		if (prcRect->Height() < 35 ||
			prcRect->Width() < szSize.cx * 2 + 1)
		{
			szSize.cx = szSize.cy = 0;
		}
		else if (szSize.cy + 70 > prcRect->Height())
		{
			szSize.cy = 34;
		}
	}

	return szSize;
}

CRect CTOPrevNextEventButton::GetRect()
{
	if (m_bVisible)
		return m_rcRect;
	else
		return CRect(0, 0, 0, 0);
}

void CTOPrevNextEventButton::AdjustLayout(CXTPCalendarView* /*pObject*/, CDC* /*pDC*/, const CRect& rcRect)
{
	CSize szSize = GetSize(&rcRect);
	m_rcRect = rcRect;

	m_rcRect.top += (rcRect.Height() - szSize.cy) / 2;
	m_rcRect.bottom = m_rcRect.top + szSize.cy;

	if (m_bPrev)
	{
		m_rcRect.left++;
		m_rcRect.right = m_rcRect.left + szSize.cx;
	}
	else
	{
		m_rcRect.right--;
		m_rcRect.left = m_rcRect.right - szSize.cx;
	}
}

void CTOPrevNextEventButton::Draw(CXTPCalendarView* /*pObject*/, CDC* pDC)
{
	if (m_rcRect.IsRectEmpty() || !m_bVisible || !GetTheme() || !GetTheme()->GetImageList())
		return;

	UINT nBitmapID = (UINT)(int)m_nBitmapID;
	// Lookup for bitmap
	CXTPResourceImage* pImage = GetTheme()->GetImageList()->GetBitmap(nBitmapID);
	ASSERT(pImage);

	CXTPResourceImage* pArrow = GetTheme()->GetBitmap(_T("CalendarPrevNextEventButtonArr"));
	ASSERT(pArrow);

	if (!pImage || !pArrow)
	{
	#ifdef _DEBUG
		pDC->FillSolidRect(m_rcRect, RGB(200, 0, 0));
	#else
		m_bVisible = FALSE;
	#endif
		return;
	}

	m_bHot = HitTest();

	int nBkIdx = (m_bPrev ? 0 : 2) + (m_bHot && !m_bDisabled ? 1 : 0);
	int nArrIdx = (m_bPrev ? 0 : 2) + (m_bDisabled ? 1 : 0);

	CRect rcBkSrc = pImage->GetSource(nBkIdx, 4);
	CRect rcArrSrc = pArrow->GetSource(nArrIdx, 4);

	CRect rcArrDest = m_rcRect;
	rcArrDest.OffsetRect(9, 12);
	rcArrDest.right = rcArrDest.left + rcArrSrc.Width();
	rcArrDest.bottom = rcArrDest.top + rcArrSrc.Height();

	pImage->DrawImage(pDC, m_rcRect, rcBkSrc, m_rcBitmapBorder, RGB(255, 0, 255));
	pArrow->DrawImage(pDC, rcArrDest, rcArrSrc, CRect(0,0,0,0), RGB(255, 0, 255));

	CRect rcText = m_rcRect;
	rcText.top = rcArrDest.bottom;
	if (rcText.Height() > 100)
	{
		rcText.DeflateRect(1, 3, 1, 3);

		int nTmp = rcText.top;
		rcText.top = rcText.bottom;
		rcText.bottom = nTmp;

		int nRectSize = abs(rcText.Height());

		//---
		COLORREF clrText = m_bDisabled ? (COLORREF)m_clrDisabledText : (COLORREF)m_fcsetText.clrColor;
		CXTPFontDC autoFont(pDC, m_fcsetText.fntFont, clrText);
		CSize szText = pDC->GetTextExtent(m_strText);

		// Center text vertically
		if (szText.cx + 7 < nRectSize)
		{
			rcText.top = max(rcText.top - (nRectSize - szText.cx)/2, rcText.bottom);
		}

		if (szText.cy < rcText.Width())
		{
			rcText.left += (rcText.Width() - szText.cy) / 2;
		}

		pDC->DrawText((const CString&)m_strText, &rcText, DT_NOPREFIX | DT_SINGLELINE); //| DT_NOCLIP
	}
}

int CTOPrevNextEventButton::HitTest(const CPoint* pPoint/* = NULL*/) const
{
	if (!m_bVisible || !GetTheme())
		return 0;

	CPoint pt(0, 0);
	if (pPoint)
	{
		pt = *pPoint;
	}
	else if (GetTheme()->GetCalendarControl()->GetSafeHwnd() && GetCursorPos(&pt))
	{
		GetTheme()->GetCalendarControl()->ScreenToClient(&pt);
	}

	if (m_rcRect.PtInRect(pt))
	{
		return m_bPrev ? xtpCalendarHitTestPrevEventButton : xtpCalendarHitTestNextEventButton;
	}

	return 0;
}

void CTOPrevNextEventButton::OnMouseMove(CCmdTarget* /*pObject*/, UINT /*nFlags*/, CPoint point)
{
	CXTPCalendarTheme* pThemeX = GetTheme();

	if (!pThemeX )
	{
		ASSERT(FALSE);
		return;
	}

	BOOL bHotNew = !!HitTest(&point);
	if (m_bHot != bHotNew)
	{
		CXTPCalendarControl::CUpdateContext updateContext(pThemeX->GetCalendarControl(), xtpCalendarUpdateRedraw);
	}
}

BOOL CTOPrevNextEventButton::OnLButtonDown(CCmdTarget* /*pObject*/, UINT /*nFlags*/, CPoint point, CTOPrevNextEventButtons *pButtons)
{
	BOOL bHitTest = HitTest(&point);
	if (bHitTest)
	{
		COleDateTime dtDay;
		BOOL bDayValid = FALSE;

		if (m_bPrev)
			bDayValid = pButtons->GetPrevEventDay(&dtDay);
		else
			bDayValid = pButtons->GetNextEventDay(&dtDay);

		CXTPCalendarView* pView = XTP_SAFE_GET2(GetTheme(), GetCalendarControl(), GetActiveView(), NULL);
		ASSERT(pView);
		if (pView && bDayValid)
		{
			XTPCalendarViewType nViewType = pView->GetViewType();
			pView->ShowDay(dtDay);

			CXTPCalendarView* pNewView = GetTheme()->GetCalendarControl()->GetActiveView();
			//pView = GetTheme()->GetCalendarControl()->GetActiveView();
			if (pView != pNewView)
			{
				pView = pNewView;
				if (pView && pView->GetViewType() != nViewType)
					GetTheme()->GetCalendarControl()->SwitchActiveView(nViewType);
			}
		}
	}

	return bHitTest;
}