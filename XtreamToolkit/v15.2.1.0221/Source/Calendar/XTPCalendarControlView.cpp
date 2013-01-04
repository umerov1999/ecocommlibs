// XTPCalendarControlView.cpp : implementation of the CXTPCalendarControlView class.
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

#include "Resource.h"

#include "Common/XTPResourceManager.h"
#include "Common/XTPImageManager.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPNotifyConnection.h"
#include "Common/XTPSmartPtrInternalT.h"

#include "XTPCalendarUtils.h"
#include "XTPCalendarDefines.h"
#include "XTPCalendarNotifications.h"
#include "XTPCalendarPtrCollectionT.h"
#include "XTPCalendarViewPart.h"

#include "XTPCalendarView.h"
#include "XTPCalendarViewDay.h"

#include "XTPCalendarMonthView.h"

#include "XTPCalendarDayView.h"
#include "XTPCalendarDayViewTimeScale.h"


#include "XTPCalendarControl.h"
#include "XTPCalendarCaptionBarControl.h"

#include "XTPCalendarControlView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//////////////////////////////////////////////////////////////////////////
// CBeforePrintState

CXTPCalendarControlView::CBeforePrintState::CBeforePrintState()
{
	ClearData();
}
void CXTPCalendarControlView::CBeforePrintState::ClearData()
{
	m_bDataValid = FALSE;

	m_spScale_orig = 0;
	m_nTopRow = 0;
}
void CXTPCalendarControlView::CBeforePrintState::Save(CXTPCalendarControlView* pView)
{
	m_bDataValid = FALSE;
	m_spScale_orig = 0;
	m_nTopRow = 0;

	BOOL b = pView->GetCalendarCtrl().m_bMultiColumnWeekMode;
	int nViewType = pView->GetCalendarCtrl().GetActiveView()->GetViewType();
	if (nViewType == xtpCalendarDayView
		|| (b && nViewType == xtpCalendarWeekView)
		|| nViewType == xtpCalendarWorkWeekView
		|| nViewType == xtpCalendarFullWeekView)
	{
		CXTPCalendarDayView* pDayView = DYNAMIC_DOWNCAST(CXTPCalendarDayView, pView->GetCalendarCtrl().GetDayView());
		ASSERT(pDayView);
		if (!pDayView)
			return;

		m_spScale_orig = pDayView->GetScaleInterval();
		m_nTopRow = pDayView->GetTopRow();

		m_bDataValid = TRUE;
	}
}

void CXTPCalendarControlView::CBeforePrintState::Restore(CXTPCalendarControlView* pView)
{
	if (!m_bDataValid)
		return;

	BOOL b = pView->GetCalendarCtrl().m_bMultiColumnWeekMode;
	int nViewType = pView->GetCalendarCtrl().GetActiveView()->GetViewType();
	if (nViewType == xtpCalendarDayView
		|| (b && nViewType == xtpCalendarWeekView)
		|| nViewType == xtpCalendarWorkWeekView
		|| nViewType == xtpCalendarFullWeekView)
	{
		CXTPCalendarDayView* pDayView = DYNAMIC_DOWNCAST(CXTPCalendarDayView, pView->GetCalendarCtrl().GetDayView());
		if (!pDayView)
			return;

		pDayView->SetScaleInterval(m_spScale_orig);
		pView->GetCalendarCtrl().RedrawControl(TRUE);
		pView->CalendarDayView_ScrolV(m_nTopRow);
	}
	pView->GetCalendarCtrl().AdjustLayout();
}



////////////////////////////////////////////////////////////////////////////
AFX_INLINE void MoveRectToXY(CRect& rc, int nX, int nY)
{
	rc.right = nX + rc.Width();
	rc.bottom = nY + rc.Height();

	rc.left = nX;
	rc.top = nY;
}

//////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CXTPCalendarPageSetupDialog, CPageSetupDialog)
	ON_BN_CLICKED(XTP_IDC_PRINT_DATE_HEADER, OnBnClickedPrintDateHeader)
	ON_BN_CLICKED(XTP_IDC_PRINT_DATE_HEADER_FONT_BUTTON, OnBnClickedDateHeaderFont)
	ON_BN_CLICKED(XTP_IDC_CALENDAR_HEADER_FORMAT_BTN, OnBnClickedHeaderFormat)
	ON_BN_CLICKED(XTP_IDC_CALENDAR_FOOTER_FORMAT_BTN, OnBnClickedFooterFormat)
	ON_BN_CLICKED(XTP_IDC_MONTH_VIEW_PRINT_RANGE, OnBnClickedMonthViewPrintRange)
END_MESSAGE_MAP()

CXTPCalendarPageSetupDialog::CXTPCalendarPageSetupDialog(
								CXTPCalendarControlViewPrintOptions* pOptions,
								DWORD dwFlags, CWnd* pParentWnd)
	: CPageSetupDialog(dwFlags, pParentWnd)
{
	ASSERT(pOptions);
	m_pOptions = pOptions;

	if (m_pOptions)
	{
		BOOL bIsInches = m_pOptions->IsMarginsMeasureInches();
		DWORD dwMeasure = bIsInches ? PSD_INTHOUSANDTHSOFINCHES : PSD_INHUNDREDTHSOFMILLIMETERS;

		m_psd.Flags &= ~PSD_INWININIINTLMEASURE;
		m_psd.Flags |= dwMeasure;
	}

	m_psd.Flags |= PSD_ENABLEPAGESETUPTEMPLATEHANDLE;
	m_psd.hPageSetupTemplate = (HGLOBAL)XTPResourceManager()->LoadDialogTemplate(IDD);
	ASSERT(m_psd.hPageSetupTemplate);

	if (m_pOptions)
	{
		m_psd.rtMargin = m_pOptions->m_rcMargins;
	}

	m_nIDHelp = XTP_IDD_CALENDAR_PRINT_PAGE_SETUP;

	VERIFY(CXTPDrawHelpers::GetIconLogFont(&m_lfDateHeader));
}

CXTPCalendarPageSetupDialog::~CXTPCalendarPageSetupDialog()
{
}

#ifndef rad1
	#define rad1        0x0420
	#define rad2        0x0421

	#define grp4        0x0433
#endif

BOOL CXTPCalendarPageSetupDialog::OnInitDialog ()
{
	CPageSetupDialog::OnInitDialog();

	ASSERT(m_pOptions);

	VERIFY( m_ctrlPrintFromCmb.SubclassDlgItem(XTP_IDC_PRINT_FROM_COMBO, this) );
	VERIFY( m_ctrlPrintToCmb.SubclassDlgItem(XTP_IDC_PRINT_TO_COMBO, this) );
	VERIFY( m_ctrlPrintFromTo.SubclassDlgItem(XTP_IDC_PRINT_FROMTO, this) );
	VERIFY( m_ctrlPrintDateHeader.SubclassDlgItem(XTP_IDC_PRINT_DATE_HEADER, this) );
	VERIFY( m_ctrlDateHeaderFontLabel.SubclassDlgItem(XTP_IDC_PRINT_DATE_HEADER_FONT_LABEL, this) );
	VERIFY( m_ctrlDateHeaderFont.SubclassDlgItem(XTP_IDC_PRINT_DATE_HEADER_FONT, this) );
	VERIFY( m_ctrlDateHeaderFontButton.SubclassDlgItem(XTP_IDC_PRINT_DATE_HEADER_FONT_BUTTON, this) );
	VERIFY( m_ctrlBlackWhitePrinting.SubclassDlgItem(XTP_IDC_PRINT_BLACK_WHITE, this) );
	if (GetDlgItem(XTP_IDC_PREV_NEXT))
		VERIFY( m_ctrl3SmallCal.SubclassDlgItem(XTP_IDC_PREV_NEXT, this) );
	VERIFY( m_ctrlHeaderFormat.SubclassDlgItem(XTP_IDC_CALENDAR_HEADER_FORMAT, this) );
	VERIFY( m_ctrlFooterFormat.SubclassDlgItem(XTP_IDC_CALENDAR_FOOTER_FORMAT, this) );
	VERIFY( m_ctrlHeaderFormatBtn.SubclassDlgItem(XTP_IDC_CALENDAR_HEADER_FORMAT_BTN, this) );
	VERIFY( m_ctrlFooterFormatBtn.SubclassDlgItem(XTP_IDC_CALENDAR_FOOTER_FORMAT_BTN, this) );

	VERIFY( m_ctrlMonthViewPrintFrom.SubclassDlgItem(XTP_IDC_MONTHVIEW_BEGIN_DATETIMEPICKER, this) );
	VERIFY( m_ctrlMonthViewPrintTo.SubclassDlgItem(XTP_IDC_MONTHVIEW_END_DATETIMEPICKER, this) );
	VERIFY( m_ctrlMonthViewPrintRange.SubclassDlgItem(XTP_IDC_MONTH_VIEW_PRINT_RANGE, this) );

	CString strTimeFormat_HrMin = CXTPCalendarUtils::GetTimeFormatString_HrMin();
	CString strTime;
	for (int i = 0; i <= 24 * 60; i += 30)
	{
		if (i < 24 * 60)
			strTime = CXTPCalendarUtils::GetTimeFormat((int)(i / 60), (int)(i % 60), strTimeFormat_HrMin);
		else
			strTime = _T("23:59:59");
		m_ctrlPrintFromCmb.AddString(strTime);
		m_ctrlPrintToCmb.AddString(strTime);
	}

	if (m_pOptions)
	{
		strTime = CXTPCalendarUtils::GetTimeFormat(m_pOptions->m_dtPrintFrom, strTimeFormat_HrMin);
		m_ctrlPrintFromCmb.SetWindowText(strTime);
		int iFr = m_ctrlPrintFromCmb.FindStringExact(0, strTime);
		if (iFr > -1)
			m_ctrlPrintFromCmb.SetCurSel(iFr);

		strTime = CXTPCalendarUtils::GetTimeFormat(m_pOptions->m_dtPrintTo, strTimeFormat_HrMin);
		m_ctrlPrintToCmb.SetWindowText(strTime);
		int iTo = m_ctrlPrintToCmb.FindStringExact(0, strTime);
		if (iTo > -1)
			m_ctrlPrintToCmb.SetCurSel(iTo);

		m_ctrlPrintFromTo.SetCheck(m_pOptions->m_bPrintFromToExactly ? 1 : 0);

		m_ctrlPrintDateHeader.SetCheck(m_pOptions->m_bPrintDateHeader ? 1 : 0);
		m_ctrlBlackWhitePrinting.SetCheck(m_pOptions->m_bBlackWhitePrinting? 1 : 0);

		if (GetDlgItem(XTP_IDC_PREV_NEXT))
			m_ctrl3SmallCal.SetCheck(m_pOptions->m_b3SmallCal ? 1 : 0);

		m_lfDateHeader = m_pOptions->m_lfDateHeaderFont;

		CString strFont = FormatFontName(m_lfDateHeader);
		m_ctrlDateHeaderFont.SetWindowText(strFont);

		if (m_pOptions->GetPageHeader())
			m_ctrlHeaderFormat.SetWindowText(m_pOptions->GetPageHeader()->m_strFormatString);

		if (m_pOptions->GetPageFooter())
			m_ctrlFooterFormat.SetWindowText(m_pOptions->GetPageFooter()->m_strFormatString);

		OnBnClickedPrintDateHeader();
	}

	XTP_SAFE_CALL1(GetDlgItem(rad1), EnableWindow(TRUE)); //1056 Portrait
	XTP_SAFE_CALL1(GetDlgItem(rad2), EnableWindow(TRUE)); //1057 Landscape

	BOOL bIsInches = m_pOptions ? m_pOptions->IsMarginsMeasureInches() : FALSE;

	UINT uStrID = bIsInches ? XTP_IDS_CALENDAR_MARGINS_INCH : XTP_IDS_CALENDAR_MARGINS_MM;
	CString strCaption = CXTPCalendarUtils::LoadString(uStrID);

	if (!strCaption.IsEmpty() && GetDlgItem(grp4))
		GetDlgItem(grp4)->SetWindowText(strCaption);

	if(m_pOptions->m_dtMonthViewPrintBeginDate.GetStatus() == COleDateTime::valid 
  && m_pOptions->m_dtMonthViewPrintEndDate.GetStatus() == COleDateTime::valid )
	{
		m_ctrlMonthViewPrintRange.SetCheck(BST_CHECKED);		
	}
	else
	{
		m_ctrlMonthViewPrintRange.SetCheck(BST_UNCHECKED);
	}

	UpdateMonthViewDateControls();

	return FALSE;
}

void CXTPCalendarPageSetupDialog::UpdateMonthViewDateControls()
{
	if(m_pOptions->m_dtMonthViewPrintBeginDate.GetStatus() == COleDateTime::valid)
	{
		m_ctrlMonthViewPrintFrom.EnableWindow(TRUE);
		m_ctrlMonthViewPrintFrom.SetTime(m_pOptions->m_dtMonthViewPrintBeginDate);
	}
	else
	{
		m_ctrlMonthViewPrintFrom.EnableWindow(FALSE);
	}

	if(m_pOptions->m_dtMonthViewPrintEndDate.GetStatus() == COleDateTime::valid)
	{
		m_ctrlMonthViewPrintTo.EnableWindow(TRUE);
		m_ctrlMonthViewPrintTo.SetTime(m_pOptions->m_dtMonthViewPrintEndDate);
	}
	else
	{
		m_ctrlMonthViewPrintTo.EnableWindow(FALSE);
	}
}

void CXTPCalendarPageSetupDialog::OnBnClickedPrintDateHeader()
{
	int nCheck = m_ctrlPrintDateHeader.GetCheck();

	m_ctrlDateHeaderFont.EnableWindow(nCheck != 0);
	m_ctrlDateHeaderFontLabel.EnableWindow(nCheck != 0);
	m_ctrlDateHeaderFontButton.EnableWindow(nCheck != 0);
}

void CXTPCalendarPageSetupDialog::OnOK()
{
	ASSERT(m_pOptions);
	if (!m_pOptions)
	{
		CPageSetupDialog::OnOK();
		return;
	}

	CString strFrom, strTo;
	m_ctrlPrintFromCmb.GetWindowText(strFrom);
	if (strFrom.Find(_T(":")) == -1)
		strFrom += _T(":00");
	int nFrom_min = CXTPCalendarUtils::ParceTimeString_min(strFrom);

	m_ctrlPrintToCmb.GetWindowText(strTo);
	if (strTo.Find(_T(":")) == -1)
		strTo += _T(":00");
	int nTo_min = CXTPCalendarUtils::ParceTimeString_min(strTo);

	m_pOptions->m_dtPrintFrom.SetTime(nFrom_min / 60, nFrom_min % 60, 0);
	m_pOptions->m_dtPrintTo.SetTime(nTo_min / 60, nTo_min % 60, 0);

	m_pOptions->m_bPrintFromToExactly = m_ctrlPrintFromTo.GetCheck() != 0;
	m_pOptions->m_bBlackWhitePrinting = m_ctrlBlackWhitePrinting.GetCheck() != 0;
	if (GetDlgItem(XTP_IDC_PREV_NEXT))
		m_pOptions->m_b3SmallCal = m_ctrl3SmallCal.GetCheck() != 0;

	m_pOptions->m_bPrintDateHeader = m_ctrlPrintDateHeader.GetCheck() != 0;
	if (m_pOptions->m_bPrintDateHeader)
		m_pOptions->m_lfDateHeaderFont = m_lfDateHeader;

	m_pOptions->m_rcMargins = m_psd.rtMargin;

	if (m_pOptions->GetPageHeader())
		m_ctrlHeaderFormat.GetWindowText(m_pOptions->GetPageHeader()->m_strFormatString);

	if (m_pOptions->GetPageFooter())
		m_ctrlFooterFormat.GetWindowText(m_pOptions->GetPageFooter()->m_strFormatString);

	int iCheck = m_ctrlMonthViewPrintRange.GetCheck();
	if(iCheck == BST_CHECKED)
	{
		m_ctrlMonthViewPrintFrom.GetTime(m_pOptions->m_dtMonthViewPrintBeginDate);
		m_ctrlMonthViewPrintTo.GetTime(m_pOptions->m_dtMonthViewPrintEndDate);
	}
	else
	{
		m_pOptions->m_dtMonthViewPrintBeginDate.SetStatus(COleDateTime::invalid);
		m_pOptions->m_dtMonthViewPrintEndDate.SetStatus(COleDateTime::invalid);
	}

	CPageSetupDialog::OnOK();
}

CString CXTPCalendarPageSetupDialog::FormatFontName(const LOGFONT& lfFont)
{
	CClientDC dc(this);
	double dLogPixelY = dc.GetDeviceCaps(LOGPIXELSY);
	double dPointSize = abs(lfFont.lfHeight) * 72.0 / dLogPixelY;
	int nPointSize = (int)(dPointSize + 0.5);

	CString strFont;
	strFont.Format(_T(" %d pt. %s"), nPointSize, lfFont.lfFaceName);

	CString strAttr;
	if (lfFont.lfWeight >= FW_BOLD)
		strAttr += _T(" Bold");

	if (lfFont.lfItalic)
	{
		if (!strAttr.IsEmpty())
			strAttr += _T(",");

		strAttr += _T(" Italic");
	}

	if (!strAttr.IsEmpty())
	{
		strFont += _T("\n");
		strFont += strAttr;
	}

	return strFont;
}

void CXTPCalendarPageSetupDialog::OnBnClickedDateHeaderFont()
{
	CFontDialog dlgFont(&m_lfDateHeader);
	int nDlgRes = (int)dlgFont.DoModal();

	if (nDlgRes == IDOK)
	{
		dlgFont.GetCurrentFont(&m_lfDateHeader);

		CString strFont = FormatFontName(m_lfDateHeader);
		m_ctrlDateHeaderFont.SetWindowText(strFont);
	}
}

void CXTPCalendarPageSetupDialog::OnBnClickedHeaderFormat()
{
	CXTPPrintPageHeaderFooter::DoInsertHFFormatSpecifierViaMenu(
		this, &m_ctrlHeaderFormat, &m_ctrlHeaderFormatBtn);
}

void CXTPCalendarPageSetupDialog::OnBnClickedFooterFormat()
{
	CXTPPrintPageHeaderFooter::DoInsertHFFormatSpecifierViaMenu(
		this, &m_ctrlFooterFormat, &m_ctrlFooterFormatBtn);
}

void CXTPCalendarPageSetupDialog::OnBnClickedMonthViewPrintRange()
{
	int iCheck = m_ctrlMonthViewPrintRange.GetCheck();
	if(iCheck == BST_CHECKED)
	{
		m_pOptions->m_dtMonthViewPrintBeginDate = CXTPCalendarUtils::GetCurrentTime();
		m_pOptions->m_dtMonthViewPrintEndDate = CXTPCalendarUtils::GetCurrentTime();
	}
	else
	{
		m_pOptions->m_dtMonthViewPrintBeginDate.SetStatus(COleDateTime::invalid);
		m_pOptions->m_dtMonthViewPrintEndDate.SetStatus(COleDateTime::invalid);
	}
	
	UpdateMonthViewDateControls();
}

/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CXTPCalendarControlViewPrintOptions, CXTPPrintOptions)

CXTPCalendarControlViewPrintOptions::CXTPCalendarControlViewPrintOptions()
{
	m_dtPrintFrom.SetTime(8, 0, 0);
	m_dtPrintTo.SetTime(19, 0, 0);
	m_bPrintFromToExactly = TRUE;

	LOGFONT lfIcon;
	VERIFY(CXTPDrawHelpers::GetIconLogFont(&lfIcon));

	m_bPrintDateHeader = TRUE;
	m_b3SmallCal = TRUE;
	m_lfDateHeaderFont = lfIcon;
	m_lfDateHeaderFont.lfHeight = -24;
	m_lfDateHeaderFont.lfWeight = FW_BOLD;

	m_lfDateHeaderWeekDayFont = lfIcon;
	m_lfDateHeaderWeekDayFont.lfHeight = -max(abs(lfIcon.lfHeight), abs(m_lfDateHeaderFont.lfHeight*2/3));

	m_lfDateHeaderCalendarFont = lfIcon;

	m_dtMonthViewPrintBeginDate.SetStatus(COleDateTime::invalid);
	m_dtMonthViewPrintEndDate.SetStatus(COleDateTime::invalid);

}

LCID CXTPCalendarControlViewPrintOptions::GetActiveLCID()
{
	return CXTPCalendarUtils::GetActiveLCID();
}

void CXTPCalendarControlViewPrintOptions::Set(const CXTPCalendarControlViewPrintOptions* pSrc)
{
	if (!pSrc)
		return;

	CXTPPrintOptions::Set(pSrc);

	m_dtPrintFrom = pSrc->m_dtPrintFrom;
	m_dtPrintTo = pSrc->m_dtPrintTo;
	m_bPrintFromToExactly = pSrc->m_bPrintFromToExactly;
	m_b3SmallCal = pSrc->m_b3SmallCal;

	m_dtMonthViewPrintBeginDate = pSrc->m_dtMonthViewPrintBeginDate;
	m_dtMonthViewPrintEndDate = pSrc->m_dtMonthViewPrintEndDate;

	m_bPrintDateHeader = pSrc->m_bPrintDateHeader;
	m_lfDateHeaderFont = pSrc->m_lfDateHeaderFont;
	m_lfDateHeaderCalendarFont = pSrc->m_lfDateHeaderCalendarFont;
}

/////////////////////////////////////////////////////////////////////////////
// CXTPCalendarControlView

IMPLEMENT_DYNCREATE(CXTPCalendarControlView, CView)

CXTPCalendarControlView::CXTPCalendarControlView()
{
	m_pSink = new CXTPNotifySink();

	m_pCalendar = new CXTPCalendarControl();
	m_bReleaseCalendarWhenDestroy = TRUE;

	m_pUpdateContect = NULL;

	m_bPrintDirect = FALSE;
	m_bResizeControlWithView = TRUE;

	m_bShowCaptionBar = FALSE;
	m_bDayViewMode = FALSE;
	m_bPrintFineMode = TRUE;
	m_bPrintRepeatHeaderFooterMode = FALSE;
	m_bDF_mode = TRUE;

	m_pScrollBar = NULL;

	m_nInitialTopCell = 0;

	m_bPrintFullScale = TRUE; //print timescale range - if FALSE - print working hours range

	m_pPrintOptions = new CXTPCalendarControlViewPrintOptions();
}

CXTPCalendarControlView::~CXTPCalendarControlView()
{
	if (m_bReleaseCalendarWhenDestroy)
		SAFE_DELETE(m_pCalendar);

	ASSERT(m_pUpdateContect == NULL);
	SAFE_DELETE(m_pUpdateContect);

	CMDTARGET_RELEASE(m_pPrintOptions);

	m_pSink->Delete();
}

CXTPCalendarControl& CXTPCalendarControlView::GetCalendarCtrl()
{
	ASSERT (m_pCalendar);
	return *m_pCalendar;
}

void CXTPCalendarControlView::SetCalendarCtrl(CXTPCalendarControl* pCalendar, BOOL bReleaseWhenDestroy)
{
	if (m_pCalendar && ::IsWindow(m_pCalendar->GetSafeHwnd()))
		m_pCalendar->DestroyWindow();

	if (m_bReleaseCalendarWhenDestroy)
		SAFE_DELETE(m_pCalendar);

	m_pCalendar = pCalendar;
	m_bReleaseCalendarWhenDestroy = bReleaseWhenDestroy;

	m_pSink->UnadviseAll();
	m_pSink->Advise(GetCalendarCtrl().GetConnection(), XTP_NC_CALENDAR_THEME_CHANGED, CreateNotfySinkClassDelegate(this, &CXTPCalendarControlView::OnEvent_CalendarThemeChanged));

	_ResizeControls();
}

void CXTPCalendarControlView::SetScrollBarCtrl(CScrollBar* pScrollBar)
{
	m_pScrollBar = pScrollBar;
}

BEGIN_MESSAGE_MAP(CXTPCalendarControlView, CView)
	//{{AFX_MSG_MAP(CXTPCalendarControlView)
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_PAINT()
	ON_WM_VSCROLL()

	ON_COMMAND(ID_FILE_PAGE_SETUP, OnFilePageSetup)
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)

	ON_BN_CLICKED(ID_EDIT_UNDO, OnUndo)
	ON_BN_CLICKED(ID_EDIT_CUT, OnCut)
	ON_BN_CLICKED(ID_EDIT_COPY, OnCopy)
	ON_BN_CLICKED(ID_EDIT_PASTE, OnPaste)
	ON_BN_CLICKED(ID_EDIT_SELECT_ALL, OnSelectAll)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateCommand)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateCommand)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateCommand)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateCommand)
	ON_UPDATE_COMMAND_UI(ID_EDIT_SELECT_ALL, OnUpdateCommand)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTPCalendarControlView drawing

void CXTPCalendarControlView::OnDraw(CDC* /*pDC*/)
{
}

void CXTPCalendarControlView::OnPaint()
{
	Default();
}

/////////////////////////////////////////////////////////////////////////////
// CXTPCalendarControlView diagnostics

#ifdef _DEBUG
void CXTPCalendarControlView::AssertValid() const
{
	CView::AssertValid();
}

void CXTPCalendarControlView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CXTPCalendarControlView message handlers

BOOL CXTPCalendarControlView::OnEraseBkgnd(CDC* )
{
	return TRUE;
}

CScrollBar* CXTPCalendarControlView::GetScrollBarCtrl(int nBar) const
{
	if (nBar == SB_VERT && m_pScrollBar)
		return m_pScrollBar;

	return CView::GetScrollBarCtrl(nBar);
}

void CXTPCalendarControlView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (m_pScrollBar && pScrollBar == m_pScrollBar)
		GetCalendarCtrl().OnVScroll(nSBCode, nPos, 0);

	CView::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CXTPCalendarControlView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	if (!m_bResizeControlWithView)
		return;

	_ResizeControls();
}

void CXTPCalendarControlView::_ResizeControls()
{
	if (!m_bResizeControlWithView)
		return;

	CXTPClientRect rcClient(this);
	int cx = rcClient.Width();
	int cy = rcClient.Height();

	int nCalendarY = 0;
	//BOOL bThemeOffice2007 = !!DYNAMIC_DOWNCAST(CXTPCalendarThemeOffice2007, GetCalendarCtrl().GetTheme());

	if (m_wndCalendarCaptionBar.GetSafeHwnd())
	{
		if (m_bShowCaptionBar/* && bThemeOffice2007*/)
		{
			CRect rcCaptionBar = m_wndCalendarCaptionBar.CalcMinRect(FALSE);

			nCalendarY = rcCaptionBar.Height();

			if ((m_wndCalendarCaptionBar.GetStyle() & WS_VISIBLE) == 0)
			{
				BOOL b = m_wndCalendarCaptionBar.IsUseOneLine();
				m_wndCalendarCaptionBar.ShowWindow(SW_SHOW);
				m_wndCalendarCaptionBar.GetTheme()->AttachCalendar(&GetCalendarCtrl());
				m_wndCalendarCaptionBar.UseOneLine(b);
			}
		}
		else
		{
			if ((m_wndCalendarCaptionBar.GetStyle() & WS_VISIBLE))
			{
				m_wndCalendarCaptionBar.ShowWindow(SW_HIDE);
				m_wndCalendarCaptionBar.GetTheme()->AttachCalendar(NULL);
			}
		}

		m_wndCalendarCaptionBar.MoveWindow(0, 0, cx, nCalendarY);
	}

	HWND hWnd = GetCalendarCtrl().GetSafeHwnd();
	if (hWnd)
	{
		BOOL bRightToLeft = !!(GetWindowLong(hWnd, GWL_EXSTYLE) & WS_EX_LAYOUTRTL);

		int nScroll = GetSystemMetrics(SM_CXVSCROLL);
		if (m_pScrollBar && m_pScrollBar->GetSafeHwnd())//internal v-scrollbar
		{
			int nDown = 0;
			if (GetCalendarCtrl().m_bTimelineMode)
			{
	//IF WE WANT TO MOVE VSCROLLBAR DOWN <<
				//CXTPCalendarTimeLineView* pTLV = (CXTPCalendarTimeLineView* ) GetCalendarCtrl().GetTimeLineView();
				//if (pTLV)
				//  nDown = pTLV->GetTimeScale()->GetRect().bottom;
			}
	//IF WE WANT TO MOVE VSCROLLBAR DOWN >>

	//RTL support
			if (bRightToLeft)
			{
				m_pScrollBar->MoveWindow(0, nCalendarY + nDown, nScroll, max(0, cy - nCalendarY - nDown));
				GetCalendarCtrl().MoveWindow(nScroll, nCalendarY, cx - nScroll, max(0, cy - nCalendarY));
			}
			else
			{
				GetCalendarCtrl().MoveWindow(0, nCalendarY, cx - nScroll, max(0, cy - nCalendarY));
				m_pScrollBar->MoveWindow(cx - nScroll, nCalendarY + nDown, nScroll, max(0, cy - nCalendarY - nDown));
			}
		}
		else //no scrollbar - ignore nScroll and RTL settings
		{
			GetCalendarCtrl().MoveWindow(0, nCalendarY, cx, max(0, cy - nCalendarY));
		}
	}
}

BOOL CXTPCalendarControlView::IsShowCaptionBar() const
{
	return m_bShowCaptionBar;
}

void CXTPCalendarControlView::ShowCaptionBar(BOOL bShow)
{
	if (m_bShowCaptionBar != bShow)
	{
		m_bShowCaptionBar = bShow;

		_ResizeControls();

		if (m_hWnd)
			Invalidate(NULL);

		CRect rcCaptionBar = m_wndCalendarCaptionBar.CalcMinRect(NULL);
		if (bShow)
			GetCalendarCtrl().m_nCaptionHeight = rcCaptionBar.Height();
		else
			GetCalendarCtrl().m_nCaptionHeight = 0;
	}
}

void CXTPCalendarControlView::OnEvent_CalendarThemeChanged(XTP_NOTIFY_CODE Event, WPARAM wParam , LPARAM lParam)
{
	UNREFERENCED_PARAMETER(Event);
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	ASSERT(Event == XTP_NC_CALENDAR_THEME_CHANGED);

	_ResizeControls();

	if (m_hWnd)
		Invalidate(NULL);
}

int CXTPCalendarControlView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndCalendarCaptionBar.Create(WS_CHILD | WS_TABSTOP, CRect(0, 0, 0, 0), this, XTP_ID_CALENDARCAPTIONBAR_CONTROL) )
	{
		TRACE(_T("Failed to create Calendar Caption Bar control window\n"));
		return -1;
	}

	if (!GetCalendarCtrl().Create(WS_CHILD | WS_TABSTOP | WS_VISIBLE, CRect(0, 0, 0, 0), this, XTP_ID_CALENDAR_CONTROL) )
	{
		TRACE(_T("Failed to create Calendar control window\n"));
		return -1;
	}

	_ResizeControls();

	return 0;
}

void CXTPCalendarControlView::OnSetFocus(CWnd* pOldWnd)
{
	CView::OnSetFocus(pOldWnd);

	GetCalendarCtrl().SetFocus();
}

BOOL CXTPCalendarControlView::OnPreparePrinting(CPrintInfo* pInfo)
{
	pInfo->m_bDirect = m_bPrintDirect;

	pInfo->SetMinPage(1);
	pInfo->SetMaxPage(1);

	return DoPreparePrinting(pInfo);
}

void CXTPCalendarControlView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* pInfo)
{
	pInfo->m_bContinuePrinting = TRUE;
	m_PageStart.RemoveAll();

	m_pritAdjustContext.Reset();
	GetCalendarCtrl().EnableSendNotifications(FALSE);
	ASSERT(m_pUpdateContect == NULL);
	SAFE_DELETE(m_pUpdateContect);
	m_pUpdateContect = new CXTPCalendarControl::CUpdateContext(&GetCalendarCtrl(), xtpCalendarUpdateRedraw);
	m_beforePrintState.Save(this);

	m_dtPrintFrom = m_pPrintOptions->m_dtPrintFrom;
	m_dtPrintTo = m_pPrintOptions->m_dtPrintTo;

	m_dtPrintFrom.SetTime(m_dtPrintFrom.GetHour(), m_dtPrintFrom.GetMinute(), 0);

	if (m_dtPrintTo.GetStatus() != COleDateTime::valid)
	{
		m_dtPrintTo.SetTime(23, 59, 59);
		m_dtPrintTo.SetStatus(COleDateTime::valid);
	}
	else
		m_dtPrintTo.SetTime(m_dtPrintTo.GetHour(), m_dtPrintTo.GetMinute(), 0);

	if (m_dtPrintFrom > m_dtPrintTo)
	{
		COleDateTime dtT = m_dtPrintTo;
		m_dtPrintTo = m_dtPrintFrom;
		m_dtPrintFrom = dtT;
	}
	else if (m_dtPrintFrom == m_dtPrintTo)
	{
		if (m_dtPrintTo.GetHour() < 23)
			m_dtPrintTo += COleDateTimeSpan(0,1,0,0);
		else if (m_dtPrintFrom.GetHour() > 1)
			m_dtPrintFrom -= COleDateTimeSpan(0,1,0,0);
	}

	m_bDayViewMode = FALSE;
	m_bMonthViewMode = FALSE;

	XTPCalendarViewType nViewType = GetCalendarCtrl().GetActiveView()->GetViewType();

	if(nViewType == xtpCalendarMonthView)
		m_bMonthViewMode = TRUE;
	
	BOOL b = GetCalendarCtrl().m_bMultiColumnWeekMode;
	CXTPCalendarDayView* pDayView = DYNAMIC_DOWNCAST(CXTPCalendarDayView, GetCalendarCtrl().GetDayView());
	if (nViewType == xtpCalendarDayView
		|| (b && nViewType == xtpCalendarWeekView)
		|| nViewType == xtpCalendarWorkWeekView
		|| nViewType == xtpCalendarFullWeekView)
	{
		if (pDayView)
		{
			m_nInitialTopCell = pDayView->GetTopRow();
			pDayView->m_nCurPage = 0;
			m_bDayViewMode = TRUE;
			m_bPrintFineMode = !pDayView->m_bDraftMode;
			m_bPrintRepeatHeaderFooterMode = pDayView->m_bPrintRepeatHeaderFooterMode;
			m_bDF_mode = pDayView->m_bDF_mode;
			m_bPrintFullScale = pDayView->m_bPrintFullScale;

			if (m_bDF_mode)
			{
				m_dtInitialScaleInterval = pDayView->GetScaleInterval();

				if (pDayView->m_nDayViewPrintInterval == 60)
					pDayView->SetScaleInterval(COleDateTimeSpan(0, 1, 0, 0));
				else
					pDayView->SetScaleInterval(COleDateTimeSpan(0, 0, pDayView->m_nDayViewPrintInterval, 0));

				GetCalendarCtrl().RedrawControl(TRUE);
			}
			m_dtTimeScaleOldMaxTime = pDayView->GetScaleMaxTime();
			m_dtTimeScaleOldMinTime = pDayView->GetScaleMinTime();

			if (!m_pPrintOptions->m_bPrintFromToExactly)
			{
				if (m_bPrintFullScale)
				{
					m_dtPrintFrom = pDayView->GetScaleMinTime();
					m_dtPrintTo = pDayView->GetScaleMaxTime();
				}
				else
				{
					GetCalendarCtrl().GetWorkDayStartTime(m_dtPrintFrom);
					GetCalendarCtrl().GetWorkDayEndTime(m_dtPrintTo);
				}
			}
			else
			{
				pDayView->SetScaleMinTime(m_dtPrintFrom);
				pDayView->SetScaleMaxTime(m_dtPrintTo);
				pDayView->ScrollToWorkDayBegin();
			}
		}
	}

	if (m_bDayViewMode)
	{
		CRect rcCalendar;
		GetCalendarCtrl().GetClientRect(&rcCalendar);

		m_nStartCell = pDayView->GetCellNumber(m_dtPrintFrom, FALSE);
		m_nEndCell = pDayView->GetCellNumber(m_dtPrintTo, TRUE);
		m_nCellSteps = pDayView->GetVisibleRowCount();
		//int iVisibleEventsMaxCount = pDayView->GetAllDayEventsMaxCount();

		int nCellCount = m_nEndCell - m_nStartCell + 1;
		m_nCurCell = m_nStartCell;
		double dRatio = ((double) (nCellCount) / (double) (m_nCellSteps));
		int nMaxPage = (int) dRatio + 1;
		pInfo->SetMaxPage(nMaxPage); //estimation only - can be adjusted later
		pInfo->m_nNumPreviewPages = 1;
		AfxGetApp()->m_nNumPreviewPages = pInfo->m_nNumPreviewPages;
	}
	else if(m_bMonthViewMode)
	{
		pInfo->m_nNumPreviewPages = 1;

		m_dtMonthViewPrintBeginDate = m_pPrintOptions->m_dtMonthViewPrintBeginDate;
		m_dtMonthViewPrintEndDate = m_pPrintOptions->m_dtMonthViewPrintEndDate;

		if(m_dtMonthViewPrintBeginDate.GetStatus() != COleDateTime::valid || m_dtMonthViewPrintEndDate.GetStatus() != COleDateTime::valid)
		{
			m_dtMonthViewPrintBeginDate = GetCalendarCtrl().GetMonthView()->GetGrid()->GetBeginDate();
			m_dtMonthViewPrintEndDate =  GetCalendarCtrl().GetMonthView()->GetGrid()->GetBeginDate();
		}

		UINT uPagesCount = CalculateMonthViewPrintPagesCount();
	  pInfo->SetMaxPage(uPagesCount); 
		pInfo->m_pPD->m_pd.nToPage = (WORD)uPagesCount;
		AfxGetApp()->m_nNumPreviewPages = pInfo->m_nNumPreviewPages;

		m_dtSavedMonthViewBeginDate = GetCalendarCtrl().GetMonthView()->GetGrid()->GetBeginDate();
	}
	else
	{
		pInfo->m_nNumPreviewPages = 1;
		pInfo->SetMaxPage(1);
		AfxGetApp()->m_nNumPreviewPages = pInfo->m_nNumPreviewPages;
	}

	pInfo->m_nCurPage = 1;
}

UINT CXTPCalendarControlView::CalculateMonthViewPrintPagesCount()
{
	const int nWeeksPerPage = GetCalendarCtrl().GetMonthView()->GetGrid()->GetWeeksCount();
	if(nWeeksPerPage <= 0)
		return 1; // by default we show 1 empty page

	const int nDaysPerWeek = 7;

	GetCalendarCtrl().GetMonthView()->GetGrid()->SetBeginDate(m_dtMonthViewPrintBeginDate);
	COleDateTime dtRealViewBeginDate = GetCalendarCtrl().GetMonthView()->GetGrid()->GetBeginDate();

	COleDateTimeSpan dtPrintRange = m_dtMonthViewPrintEndDate - dtRealViewBeginDate;
	const int nTotalDaysInRange = (int)dtPrintRange.GetTotalDays();
	int nPagesCount = nTotalDaysInRange / (nWeeksPerPage * nDaysPerWeek) + 1;
	if(nPagesCount < 1)
		nPagesCount = 1;

	return (UINT)nPagesCount;
}

void CXTPCalendarControlView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* pInfo)
{
	m_beforePrintState.Restore(this);
	m_beforePrintState.ClearData();

	m_PageStart.RemoveAll();

	if (m_bDayViewMode)
	{
		CXTPCalendarDayView* pDayView = DYNAMIC_DOWNCAST(CXTPCalendarDayView, GetCalendarCtrl().GetDayView());
		if (pDayView)
		{
			pDayView->m_nCurPage = 0;
			pDayView->SetScaleMinTime(m_dtTimeScaleOldMinTime);
			pDayView->SetScaleMaxTime(m_dtTimeScaleOldMaxTime);
			pDayView->ScrollToWorkDayBegin();
			if (m_bDF_mode)
			{
				pDayView->SetScaleInterval(m_dtInitialScaleInterval);
				GetCalendarCtrl().RedrawControl(TRUE);
			}
			pDayView->ScrollV(m_nInitialTopCell, m_nInitialTopCell);
			GetCalendarCtrl().AdjustScrollBarEx(SB_VERT);
		}
	}

	if(m_bMonthViewMode) // restore original month view content
	{
		GetCalendarCtrl().GetMonthView()->GetGrid()->SetBeginDate(m_dtSavedMonthViewBeginDate);
		GetCalendarCtrl().GetMonthView()->Populate();
	}

	GetCalendarCtrl().AdjustLayout();
	GetCalendarCtrl().EnableSendNotifications();

	m_pritAdjustContext.Reset();

	m_bmpGrayDC.DeleteObject();

	ASSERT(m_pUpdateContect);
	SAFE_DELETE(m_pUpdateContect);

	m_bPrintFineMode = TRUE;
	pInfo->m_nCurPage = 1;
}

CRect CXTPCalendarControlView::AdjustCalendarForPrint(CDC* pDC, CRect rcCalendar, CPrintInfo* /*pInfo*/)
{
	GetCalendarCtrl().AdjustLayout(pDC, rcCalendar);

	if (m_bDayViewMode)
	{
		CXTPCalendarDayView* pDayView = DYNAMIC_DOWNCAST(CXTPCalendarDayView, GetCalendarCtrl().GetDayView());
		if (pDayView)
		{
			int nCellCount = m_nEndCell - m_nCurCell + 1;
			int nCellSteps = pDayView->GetVisibleRowCount();
			int nCelHeight = pDayView->GetRowHeight();
			if (nCellCount <= nCellSteps)
				rcCalendar.bottom -= (nCellSteps - nCellCount) * nCelHeight;

			GetCalendarCtrl().AdjustLayout(pDC, rcCalendar);
			pDayView->_ScrollV(m_nCurCell, m_nCurCell);
		}
	}

	return rcCalendar;
}

void CXTPCalendarControlView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)
{
	ASSERT_VALID(this);
	ASSERT_VALID(pDC);
	ASSERT(pInfo != NULL);

	pDC->SetMapMode(MM_ANISOTROPIC);
	pDC->SetViewportExt(pDC->GetDeviceCaps(LOGPIXELSX), pDC->GetDeviceCaps(LOGPIXELSY));
	pDC->SetWindowExt(96, 96);
	pDC->OffsetWindowOrg(0, 0);

	if (pInfo->m_bPreview) //PRINT MODE in RTL does not work!
	{
//------------------------------------------------------------
		if (GetCalendarCtrl().GetExStyle() & WS_EX_RTLREADING)
			pDC->SetTextAlign(TA_RTLREADING);

		if (GetCalendarCtrl().GetExStyle() & WS_EX_LAYOUTRTL)
			XTPDrawHelpers()->SetContextRTL(pDC, LAYOUT_RTL);
//------------------------------------------------------------
	}
}

void CXTPCalendarControlView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
	if (!m_pPrintOptions || !pDC || !pInfo)
	{
		ASSERT(FALSE);
		return;
	}

	SetupCurrentPrintPage(pDC, pInfo);	

	DoPrintPage(pDC, pInfo);

	SelectNextPrintPage(pInfo);
}

void CXTPCalendarControlView::SetupCurrentPrintPage(CDC* pDC, CPrintInfo* pInfo)
{
	if(m_bDayViewMode)
	{
		SetupCurrentDayViewPrintPage(pDC, pInfo);	
		return;
	}

	if(m_bMonthViewMode)
	{
		SetupCurrentMonthViewPrintPage(pInfo);
		return;		
	}
}

void CXTPCalendarControlView::SetupCurrentDayViewPrintPage(CDC* pDC, CPrintInfo* pInfo)
{
	if (pInfo->m_nCurPage == 1)
	{
		m_nCurCell = m_nStartCell;
		if (m_PageStart.GetSize() == 0)
			m_PageStart.Add(m_nCurCell);
	}

	if (pInfo->m_bPreview) //to allow preview go back
	{
		if ((int) pInfo->m_nCurPage - 1 < m_PageStart.GetSize())
			m_nCurCell = m_PageStart.GetAt(pInfo->m_nCurPage - 1);
	}

	CRect rcCalendar;
	GetCalendarCtrl().GetClientRect(&rcCalendar);

	CXTPCalendarDayView* pDayView = DYNAMIC_DOWNCAST(CXTPCalendarDayView, GetCalendarCtrl().GetDayView());
	if (pDayView)
	{
		pDayView->m_LayoutX.m_nTopRow = m_nCurCell;

		rcCalendar.left += pDayView->GetTimeScale()->CalcWidth(pDC) / 2;
		pDayView->m_nCurPage = (int) pInfo->m_nCurPage - 1;

		if (GetCalendarCtrl().GetTheme())
			pDayView->AdjustLayout2(pDC, rcCalendar);
		else
			pDayView->AdjustLayout(pDC, rcCalendar);
	}	
}

void CXTPCalendarControlView::SetupCurrentMonthViewPrintPage(CPrintInfo* pInfo)
{
	int nZeroBasedCurPage = (pInfo->m_nCurPage - 1);
	COleDateTime m_dtPeriodBeg = m_dtMonthViewPrintBeginDate;
	int nWeeksCount = GetCalendarCtrl().GetMonthView()->GetGrid()->GetWeeksCount();
	m_dtPeriodBeg = m_dtPeriodBeg + COleDateTimeSpan(7 * nWeeksCount * nZeroBasedCurPage, 0, 0, 0) ;

	GetCalendarCtrl().GetMonthView()->GetGrid()->SetBeginDate(m_dtPeriodBeg);
	GetCalendarCtrl().GetMonthView()->Populate();
}

void CXTPCalendarControlView::SelectNextPrintPage(CPrintInfo* pInfo)
{
	if (m_bDayViewMode)
	{
		SelectNextDayViewPrintPage(pInfo);
	}
	//else
	//{
	//	pInfo->m_nCurPage++;
	//}
}

void CXTPCalendarControlView::SelectNextDayViewPrintPage(CPrintInfo* pInfo)
{
	CXTPCalendarDayView* pDayView = DYNAMIC_DOWNCAST(CXTPCalendarDayView, GetCalendarCtrl().GetDayView());
	if (pDayView)
		m_nCellSteps = pDayView->GetVisibleRowCount();

	if (!pInfo->m_bPreview) //print is going forward only
	{
		m_nCurCell += m_nCellSteps;

		if (m_nCurCell > m_nEndCell)
			pInfo->m_bContinuePrinting = FALSE;
		else
			pInfo->m_nCurPage++;
	}
	else //preview mode - user can switch form page to page in any direction
	{
		if (m_PageStart.GetSize() == (int) pInfo->m_nCurPage)
		{
			m_nCurCell = m_PageStart.GetAt(pInfo->m_nCurPage - 1) + m_nCellSteps;
			m_PageStart.Add(m_nCurCell);
		}

		if (m_nEndCell - m_nCurCell > 0) //at least one page left to print
			pInfo->m_nCurPage++;
		else
		{
			pInfo->m_bContinuePrinting = FALSE;
			pInfo->SetMaxPage(pInfo->m_nCurPage);

			//              if (pInfo->m_nCurPage >= pInfo->GetMaxPage() - 1)
			//                  pInfo->SetMaxPage(pInfo->m_nCurPage);
		}
	}
}


void CXTPCalendarControlView::DoPrintPage(CDC* pDC, CPrintInfo* pInfo)
{
	CRect rcPrint = ExcludeMarginsFromDrawRect(pDC, pInfo);

	if (!m_pPrintOptions->m_bBlackWhitePrinting && m_bPrintFineMode)
	{
		_OnPrint2(pDC, pInfo, rcPrint);
	}
	else
	{
		CRect rcPrint00(0, 0, rcPrint.Width(), rcPrint.Height());

		CDC memDC;
		VERIFY(memDC.CreateCompatibleDC(pDC));
		memDC.m_bPrinting = TRUE;

		if (!m_bmpGrayDC.m_hObject || m_bmpGrayDC.GetBitmapDimension() != rcPrint00.Size())
		{
			m_bmpGrayDC.DeleteObject();
			m_bmpGrayDC.CreateCompatibleBitmap(pDC, rcPrint00.Width(), rcPrint00.Height());
		}

		CXTPBitmapDC autpBmp(&memDC, &m_bmpGrayDC);
		memDC.FillSolidRect(rcPrint00, RGB(255, 255, 255));

		_OnPrint2(&memDC, pInfo, rcPrint00);

		//      rcPrint00.bottom += rcPrint.top;

		int nCC = max(0, min(m_pPrintOptions->m_nBlackWhiteContrast, 255));
		XTPImageManager()->BlackWhiteBitmap(memDC, rcPrint00, nCC);

		pDC->BitBlt(rcPrint.left, rcPrint.top, rcPrint.Width(), rcPrint.Height(), &memDC, 0, 0, SRCCOPY); //ORIG
	}
}

CRect CXTPCalendarControlView::ExcludeMarginsFromDrawRect(CDC* pDC, CPrintInfo* pInfo)
{
	CRect rcMargins = m_pPrintOptions->GetMarginsLP(pDC);
	CRect rcPrint = pInfo->m_rectDraw;
	rcPrint.DeflateRect(rcMargins);

	return rcPrint;
}

void CXTPCalendarControlView::_OnPrint2(CDC* pDC, CPrintInfo* pInfo, CRect rcPrint)
{
	UNREFERENCED_PARAMETER(pInfo);

	m_pritAdjustContext.m_rcHeader = rcPrint;
	m_pritAdjustContext.m_rcFooter = rcPrint;
	CString strTitle = CXTPPrintPageHeaderFooter::GetParentFrameTitle(this);

	m_pPrintOptions->GetPageHeader()->FormatTexts(pInfo, strTitle);
	m_pPrintOptions->GetPageFooter()->FormatTexts(pInfo, strTitle);

	if (m_bPrintFineMode)
	{
		m_pPrintOptions->GetPageFooter()->Draw(pDC, m_pritAdjustContext.m_rcFooter, TRUE);
		m_pPrintOptions->GetPageHeader()->Draw(pDC, m_pritAdjustContext.m_rcHeader);

		rcPrint.top += m_pritAdjustContext.m_rcHeader.Height();
		rcPrint.bottom -= m_pritAdjustContext.m_rcFooter.Height();
	}
	CRect rcCalendar = rcPrint;

	if (m_bPrintFineMode
		&& m_pPrintOptions->m_bPrintDateHeader
		&& (m_bPrintRepeatHeaderFooterMode || pInfo->m_nCurPage == 1))
	{
		m_pritAdjustContext.m_rcDateHeader = rcPrint;

		DrawDateHeader(FALSE, pDC, m_pritAdjustContext.m_rcDateHeader);

		rcCalendar.top = m_pritAdjustContext.m_rcDateHeader.bottom + 2;
	}

	rcCalendar.DeflateRect(2, 2, 2, 2);

	m_pritAdjustContext.m_rcCalendar = AdjustCalendarForPrint(pDC, rcCalendar, pInfo);
	m_pritAdjustContext.m_rectDraw_prev = rcCalendar;

	pDC->IntersectClipRect(&m_pritAdjustContext.m_rcCalendar);

	GetCalendarCtrl().OnDraw(pDC);

	pDC->SelectClipRgn(NULL);

	CRect rcBorder = m_pritAdjustContext.m_rcCalendar;

	rcBorder.InflateRect(1, 1, 1, 1);
	pDC->Draw3dRect(&rcBorder, 0, 0);

	rcBorder.InflateRect(1, 1, 1, 1);
	pDC->Draw3dRect(&rcBorder, 0, 0);

	if (m_bPrintFineMode)
		m_pPrintOptions->GetPageFooter()->Draw(pDC, m_pritAdjustContext.m_rcFooter);
}

void CXTPCalendarControlView::DrawDateHeader(BOOL bCalculate, CDC* pDC, CRect& rrcHeader)
{
	COleDateTime dtYearMonth1, dtYearMonth2;
	CString strHeader1, strHeader2, strHeaderWeekDay;

	int nMonths = _GetDateHeaderMonths(dtYearMonth1, dtYearMonth2, strHeader1, strHeader2, strHeaderWeekDay);

	if (!strHeader2.IsEmpty())
		strHeader1 += _T(" -");

	CRect rcCalendar1 = rrcHeader;
	CRect rcCalendar2(0, 0, 0, 0);
	CRect rcCalendar3(0, 0, 0, 0);

	BOOL bRTL = FALSE;
	if (GetCalendarCtrl().GetExStyle() & WS_EX_LAYOUTRTL)
		bRTL = TRUE;

	int iYear = dtYearMonth1.GetYear();
	int iMonth = dtYearMonth1.GetMonth();
	if (nMonths == 1 && m_pPrintOptions->m_b3SmallCal)
	{
		if (bRTL)
		{
			if (iMonth > 1)
				iMonth--;
			else
			{
				iMonth = 12;
				iYear--;
			}
			dtYearMonth1.SetDateTime(iYear, iMonth, 1,0,0,0);
		}
		else
		{
			if (iMonth < 12)
				iMonth++;
			else
			{
				iMonth = 1;
				iYear++;
			}
			dtYearMonth1.SetDateTime(iYear, iMonth, 1,0,0,0);
		}
	}

	_DrawDateHeaderCalendar(TRUE, pDC, dtYearMonth1, rcCalendar1);

	int nCalendar1XPos = rrcHeader.right - rcCalendar1.Width() * 11/10;
	int nCalendar2XPos = nCalendar1XPos;

	CFont fntDateHeader;
	VERIFY( fntDateHeader.CreateFontIndirect(&m_pPrintOptions->m_lfDateHeaderFont) );
	CXTPFontDC fntDC(pDC, &fntDateHeader);
	CSize sizeText = pDC->GetTextExtent(strHeader1);

	CFont fntDateHeaderWeekDay;
	VERIFY( fntDateHeaderWeekDay.CreateFontIndirect(&m_pPrintOptions->m_lfDateHeaderWeekDayFont) );
	fntDC.SetFont(&fntDateHeaderWeekDay);
	CSize sizeTextWD = pDC->GetTextExtent(_T("WD"), 2);

	int nDatesCaptionHeight = sizeText.cy * 225/100 + sizeTextWD.cy * 125/100;
	int nMaxHeight = max(rcCalendar1.Height() * 11/10, nDatesCaptionHeight);

	rrcHeader.bottom = rrcHeader.top + nMaxHeight;

	int nCalendarYPos = rrcHeader.top + nMaxHeight/2 - rcCalendar1.Height() / 2;
	MoveRectToXY(rcCalendar1, nCalendar1XPos, nCalendarYPos);

	if (bCalculate)
		return;

	COLORREF clrPrev = pDC->SetTextColor(0);
	int nBkModePrev = pDC->SetBkMode(TRANSPARENT);

	if (nMonths > 1)
	{
		rcCalendar2 = rcCalendar1;
		nCalendar2XPos = nCalendar1XPos - rcCalendar1.Width() * 11 / 10;
		MoveRectToXY(rcCalendar1, nCalendar2XPos, nCalendarYPos);
	}

	if (!m_pPrintOptions->m_bBlackWhitePrinting)
		pDC->FillSolidRect(&rrcHeader, RGB(229, 229, 229));

	pDC->Draw3dRect(&rrcHeader, 0, 0);

	_DrawDateHeaderCalendar(bCalculate, pDC, dtYearMonth1, rcCalendar1);

	if (nMonths > 1)
		_DrawDateHeaderCalendar(FALSE, pDC, dtYearMonth2, rcCalendar2);

	if (nMonths == 1 && m_pPrintOptions->m_b3SmallCal)
	{
		rcCalendar2 = rcCalendar1;
		nCalendar2XPos = nCalendar1XPos - rcCalendar1.Width() * 11 / 10;
		MoveRectToXY(rcCalendar2, nCalendar2XPos, nCalendarYPos);

		if (bRTL)
		{
			if (iMonth < 12)
				iMonth++;
			else
			{
				iMonth = 1;
				iYear++;
			}
			dtYearMonth1.SetDateTime(iYear, iMonth, 1,0,0,0);
		}
		else
		{
			if (iMonth > 1)
				iMonth--;
			else
			{
				iMonth = 12;
				iYear--;
			}
			dtYearMonth1.SetDateTime(iYear, iMonth, 1,0,0,0);
		}

		_DrawDateHeaderCalendar(bCalculate, pDC, dtYearMonth1, rcCalendar2);

		rcCalendar3 = rcCalendar2;
		nCalendar2XPos = nCalendar2XPos - rcCalendar2.Width() * 11 / 10;
		MoveRectToXY(rcCalendar3, nCalendar2XPos, nCalendarYPos);

		if (bRTL)
		{
			if (iMonth < 12)
				iMonth++;
			else
			{
				iMonth = 1;
				iYear++;
			}
			dtYearMonth1.SetDateTime(iYear, iMonth, 1,0,0,0);
		}
		else
		{
			if (iMonth > 1)
				iMonth--;
			else
			{
				iMonth = 12;
				iYear--;
			}
			dtYearMonth1.SetDateTime(iYear, iMonth, 1,0,0,0);
		}

		_DrawDateHeaderCalendar(bCalculate, pDC, dtYearMonth1, rcCalendar3);
	}

	CRect rcHeader1(0, 0, 0, 0);
	CRect rcHeader2(0, 0, 0, 0);
	CRect rcHeader3(0, 0, 0, 0);

	rcHeader1 = rrcHeader;
	rcHeader1.DeflateRect(10, 5, 10, 5);
	rcHeader1.right = nCalendar2XPos - 20;

	rcHeader2 = rcHeader1;

	fntDC.SetFont(&fntDateHeader);

	UINT uDrawFlags = DT_TOP | DT_LEFT | DT_WORDBREAK | DT_NOPREFIX;
	int nHeight1 = pDC->DrawText(strHeader1, &rcHeader1, uDrawFlags);

	if (!strHeader2.IsEmpty())
	{
		rcHeader2.top += nHeight1 + sizeText.cy / 4;
		if (rcHeader2.top < rcHeader2.bottom)
			pDC->DrawText(strHeader2, &rcHeader2, uDrawFlags);
	}

	if (!strHeaderWeekDay.IsEmpty())
	{
		fntDC.SetFont(&fntDateHeaderWeekDay);

		rcHeader3 = rcHeader2;
		rcHeader3.top += nHeight1 + sizeTextWD.cy / 5;

		if (rcHeader3.top < rcHeader3.bottom)
			pDC->DrawText(strHeaderWeekDay, &rcHeader3, uDrawFlags);
	}

	pDC->SetTextColor(clrPrev);
	pDC->SetBkMode(nBkModePrev);
}


void CXTPCalendarControlView::_DrawDateHeaderCalendar(BOOL bCalculate,
									CDC* pDC,
									COleDateTime dtYearMonth,
									CRect& rrcHeaderCalendar)
{
	LOGFONT lfCalWeek = m_pPrintOptions->m_lfDateHeaderCalendarFont;
	lfCalWeek.lfHeight = lfCalWeek.lfHeight * 8 / 10;
	CFont fntCalendarWeek;
	VERIFY( fntCalendarWeek.CreateFontIndirect(&lfCalWeek) );

	CFont fntCalendar;
	VERIFY( fntCalendar.CreateFontIndirect(&m_pPrintOptions->m_lfDateHeaderCalendarFont) );
	CXTPFontDC fntDC(pDC, &fntCalendar);

	CSize sizeCellText = pDC->GetTextExtent(_T("88"), 2);

	CSize sizeCell = sizeCellText;
	sizeCell.cx = sizeCell.cx * 13 / 10;
	sizeCell.cy = sizeCell.cy * 11 / 10;
	int nMonthRowY = sizeCellText.cy;
	int nWSaysRowY = sizeCellText.cy * 13 / 10;

	rrcHeaderCalendar.right = rrcHeaderCalendar.left + sizeCell.cx * 9;
	rrcHeaderCalendar.bottom = rrcHeaderCalendar.top + nMonthRowY + nWSaysRowY
								+ sizeCell.cy * 6;
	if (bCalculate)
		return;

	int nFirstDayOfWeek = GetCalendarCtrl().GetFirstDayOfWeek();

	COleDateTime dtMonthFirst = dtYearMonth;
	VERIFY( CXTPCalendarUtils::UpdateMonthDay(dtMonthFirst, 1) );
	int nMaxMonthDay = CXTPCalendarUtils::GetMaxMonthDay(dtYearMonth);

	CStringArray arWDNames;
	int i;
	for (i = 0; i < 7; i++)
	{
		CString sWD = CXTPCalendarUtils::GetLocaleString(LOCALE_SABBREVDAYNAME1 + ((i - 1 + 7) % 7), 100);

//RTL case fix
		if (GetCalendarCtrl().GetExStyle() & WS_EX_LAYOUTRTL)
		{
			if (sWD.Find(_T(" ")) > -1)
				sWD = sWD.Right(1);
			else
				sWD = sWD.Left(1);

			//if (i == 6) //Hebrew
			//  sWD = sWD.Left(1);
			//else
			//  sWD = sWD.Right(1);
		}

		arWDNames.Add(sWD);
	}
	int nMonth = dtYearMonth.GetMonth();
	int nYear = dtYearMonth.GetYear();
	CString strMonthName = CXTPCalendarUtils::GetLocaleString(LOCALE_SMONTHNAME1 + nMonth - 1, 100);
	CString strMonthYear;
	strMonthYear.Format(_T("%s  %d"), (LPCTSTR)strMonthName, nYear);

	COLORREF clrPrev = pDC->SetTextColor(0);
	int nBkModePrev = pDC->SetBkMode(TRANSPARENT);

	CRect rcText = rrcHeaderCalendar;
	rcText.left += sizeCell.cx;
	pDC->DrawText(strMonthYear, &rcText, DT_TOP | DT_CENTER | DT_SINGLELINE);

	rcText = rrcHeaderCalendar;
	rcText.top += sizeCell.cy;
	for (i = 0; i < 7; i++)
	{
		rcText.left += sizeCell.cx;
		rcText.right = rcText.left + sizeCell.cx;

		int nMidx = (nFirstDayOfWeek - 1 + i) % 7;
		CString strWD = arWDNames[nMidx];

		pDC->DrawText((LPCTSTR)strWD, 1, &rcText, DT_TOP | DT_RIGHT | DT_SINGLELINE);
	}

	CRect rcLine1 = rrcHeaderCalendar;
	rcLine1.left += sizeCell.cx - 0;
	rcLine1.top += nMonthRowY + nWSaysRowY - 1;
	rcLine1.bottom = rcLine1.top + 1;

	CRect rcLine2 = rrcHeaderCalendar;
	rcLine2.left = rcLine1.left;
	rcLine2.right = rcLine2.left + 1;
	rcLine2.top = rcLine1.top;

	pDC->FillSolidRect(&rcLine1, 0);
	pDC->FillSolidRect(&rcLine2, 0);

	int nDay = 1 - (dtMonthFirst.GetDayOfWeek() - nFirstDayOfWeek + 7) % 7;

	int nFirstWeekOfYearDays = GetCalendarCtrl().GetFirstWeekOfYearDays();
	//int nFirstWeekOfYearDays = 1;
	COleDateTimeSpan spFWofY(nFirstWeekOfYearDays - 1);
	COleDateTime dtWeekLast = dtMonthFirst + COleDateTimeSpan(nDay + 6 - 1);
	int nFirstWeekNumber = ((dtWeekLast - spFWofY).GetDayOfYear() - 1) / 7 + 1;

	//--------------------------
	int nTodayDay_toMark = 0;
	COleDateTime dtToday = CXTPCalendarUtils::GetCurrentTime();
	if (dtYearMonth.GetYear() == dtToday.GetYear() &&
		dtYearMonth.GetMonth() == dtToday.GetMonth())
		nTodayDay_toMark = dtToday.GetDay();

	CString strDay, strWeek;
	for (int w = 0; w < 6; w++)
	{
		rcText.top = rrcHeaderCalendar.top + nMonthRowY + nWSaysRowY + sizeCell.cy * w;

		if (nDay <= nMaxMonthDay)
		{
			CRect rcWeek = rcText;
			rcWeek.left = rrcHeaderCalendar.left;
			rcWeek.right = rcWeek.left + sizeCell.cx - 2;
			int iw = nFirstWeekNumber + w;

			if (nMonth == 1 && w == 1 && iw > 52)
				iw = 1;
			if (iw > 53)
				iw = iw % 53;
			strWeek.Format(_T("%d"), iw);

			CXTPFontDC fntDCweek(pDC, &fntCalendarWeek);
			pDC->DrawText(strWeek, &rcWeek, DT_TOP | DT_RIGHT | DT_SINGLELINE);
		}

		for (i = 0; i < 7; i++)
		{
			if (nDay > 0 && nDay <= nMaxMonthDay)
			{
				rcText.left = rrcHeaderCalendar.left + sizeCell.cx * (i + 1);
				rcText.right = rcText.left + sizeCell.cx;

				strDay.Format(_T("%d"), nDay);

				pDC->DrawText(strDay, &rcText, DT_TOP | DT_RIGHT | DT_SINGLELINE);

				if (nDay == nTodayDay_toMark)
				{
					CRect rcToday = rcText;
					rcToday.left += max(1, sizeCell.cx/10);
					rcToday.right += max(1, sizeCell.cx/10);
					rcToday.bottom = rcToday.top + sizeCell.cy;

					CBrush brFrame(RGB(82, 82, 82));
					pDC->FrameRect(&rcToday, &brFrame);
				}
			}
			nDay++;
		}
	}

	pDC->SetTextColor(clrPrev);
	pDC->SetBkMode(nBkModePrev);
}

int CXTPCalendarControlView::_GetDateHeaderMonths(COleDateTime& dtYearMonth1, COleDateTime& dtYearMonth2,
		CString& strHeader1, CString& strHeader2, CString& strHeaderWeekDay)
{
	strHeader1 = strHeader2 = strHeaderWeekDay = _T("");
	dtYearMonth1 = dtYearMonth2 = (DATE)0;

	SYSTEMTIME st;

	BOOL b = GetCalendarCtrl().m_bMultiColumnWeekMode;
	int nViewType = GetCalendarCtrl().GetActiveView()->GetViewType();
	if (nViewType == xtpCalendarDayView
		|| (b && nViewType == xtpCalendarWeekView)
		|| nViewType == xtpCalendarWorkWeekView
		|| (b && nViewType == xtpCalendarFullWeekView))
	{
		CXTPCalendarDayView* pDayView = DYNAMIC_DOWNCAST(CXTPCalendarDayView, GetCalendarCtrl().GetDayView());
		if (!pDayView)
			return 0;

		dtYearMonth1 = pDayView->GetViewDayDate(0);
		CXTPCalendarUtils::GetAsSystemTime(dtYearMonth1, st);
		strHeader1 = CXTPCalendarUtils::GetDateFormat(&st, _T("d MMMM"));

		int nCount = pDayView->GetViewDayCount();
		if (nCount > 1)
		{
			dtYearMonth2 = pDayView->GetViewDayDate(nCount-1);
			CXTPCalendarUtils::GetAsSystemTime(dtYearMonth2, st);
			strHeader2 = CXTPCalendarUtils::GetDateFormat(&st, _T("d MMMM"));

			if (dtYearMonth1.GetMonth() != dtYearMonth2.GetMonth())
				return 2;
		}

		if (nCount == 1)
		{
			COleDateTime dtDay0 = pDayView->GetViewDayDate(0);
			int nWd = dtDay0.GetDayOfWeek();
			int nWdShift = (nWd - 1 + 6) % 7;
			strHeaderWeekDay = CXTPCalendarUtils::GetLocaleString(LOCALE_SDAYNAME1 + nWdShift, 100);
		}

		return 1;
	}
	else if (nViewType == xtpCalendarWeekView
		|| (!b && nViewType == xtpCalendarFullWeekView))
	{
		CXTPCalendarView* pView = GetCalendarCtrl().GetActiveView();
		if (!pView)
			return 0;

		dtYearMonth1 = pView->GetViewDayDate(0);
		dtYearMonth2 = pView->GetViewDayDate(pView->GetViewDayCount()-1);

		CString strHeaderFormat = pView->GetDayHeaderFormat();
		if (strHeaderFormat.IsEmpty())
			strHeaderFormat = _T("d MMMM");

		CXTPCalendarUtils::GetAsSystemTime(dtYearMonth1, st);
		strHeader1 = CXTPCalendarUtils::GetDateFormat(&st, strHeaderFormat);

		CXTPCalendarUtils::GetAsSystemTime(dtYearMonth2, st);
		strHeader2 = CXTPCalendarUtils::GetDateFormat(&st, strHeaderFormat);

		return 1;
	}
	else if (nViewType == xtpCalendarMonthView)
	{
		CXTPCalendarMonthView* pMonthView = DYNAMIC_DOWNCAST(CXTPCalendarMonthView, GetCalendarCtrl().GetMonthView());

		if (!pMonthView || !pMonthView->GetGrid())
			return 0;

		int nWeeksCount = pMonthView->GetGrid()->GetWeeksCount();
		ASSERT(nWeeksCount);
		if (nWeeksCount)
		{
			CXTPCalendarViewDay* pMVday1 = (CXTPCalendarViewDay*)pMonthView->GetGrid()->GetViewDay(0, 6);
			CXTPCalendarViewDay* pMVday2 = (CXTPCalendarViewDay*)pMonthView->GetGrid()->GetViewDay(nWeeksCount-1, 0);
			if (pMVday1 && pMVday2)
			{
				dtYearMonth1 = pMVday1->GetDayDate();

				CXTPCalendarUtils::GetAsSystemTime(dtYearMonth1, st);
				strHeader1 = CXTPCalendarUtils::GetDateFormat(&st, _T("MMMM yyyy"));

				COleDateTime dtLast = pMVday2->GetDayDate();
				if (dtYearMonth1.GetMonth() != dtLast.GetMonth())
				{
					dtYearMonth2 = dtLast;
					CXTPCalendarUtils::GetAsSystemTime(dtYearMonth2, st);
					strHeader2 = CXTPCalendarUtils::GetDateFormat(&st, _T("MMMM yyyy"));
					return 2;
				}
				return 1;
			}
		}
	}
	return 0;
}

CRect CXTPCalendarControlView::_HimetricToLP(CDC* pDC, const CRect rcHimetric)
{
	CSize szLT(rcHimetric.left, rcHimetric.top);
	CSize szRB(rcHimetric.right, rcHimetric.bottom);

	pDC->HIMETRICtoLP(&szLT);
	pDC->HIMETRICtoLP(&szRB);

	CRect rcLP(szLT.cx, szLT.cy, szRB.cx, szRB.cy);

	return rcLP;
}


//////////////////////////////////////////////////////////////////////////
// Clipboard operations

void CXTPCalendarControlView::OnUndo()
{
	GetCalendarCtrl().OnUndo();
}

void CXTPCalendarControlView::OnCut()
{
	CWnd* pFocusWnd = GetFocus();
	if (pFocusWnd)
	{
		LRESULT lRes = pFocusWnd->SendMessage(WM_COMMAND, ID_EDIT_CUT, NULL);
		if (!lRes)
			pFocusWnd->SendMessage(WM_CUT);
	}
}

void CXTPCalendarControlView::OnSelectAll()
{
	CWnd* pFocusWnd = GetFocus();
	if (pFocusWnd)
	{
		LRESULT lRes = pFocusWnd->SendMessage(WM_COMMAND, ID_EDIT_SELECT_ALL, NULL);
		if (!lRes && pFocusWnd->IsKindOf(RUNTIME_CLASS(CEdit)))
			((CEdit*) pFocusWnd)->SetSel(0, -1);
	}
}

void CXTPCalendarControlView::OnCopy()
{
	CWnd* pFocusWnd = GetFocus();
	if (pFocusWnd)
	{
		LRESULT lRes = pFocusWnd->SendMessage(WM_COMMAND, ID_EDIT_COPY, NULL);
		if (!lRes)
			pFocusWnd->SendMessage(WM_COPY);
	}
}

void CXTPCalendarControlView::OnPaste()
{
	CWnd* pFocusWnd = GetFocus();
	if (pFocusWnd)
	{
		LRESULT lRes = pFocusWnd->SendMessage(WM_COMMAND, ID_EDIT_PASTE, NULL);
		if (!lRes)
			pFocusWnd->SendMessage(WM_PASTE);
	}
}

void CXTPCalendarControlView::OnUpdateCommand(CCmdUI* pCmdUI)
{
	GetCalendarCtrl().OnUpdateCmdUI(pCmdUI);
}

void CXTPCalendarControlView::OnFilePageSetup()
{
	DWORD dwFlags = PSD_MARGINS | PSD_INWININIINTLMEASURE;
	CXTPCalendarPageSetupDialog dlgPageSetup(GetPrintOptions(), dwFlags, this);

	XTPGetPrinterDeviceDefaults(dlgPageSetup.m_psd.hDevMode, dlgPageSetup.m_psd.hDevNames);

	int nDlgRes = (int)dlgPageSetup.DoModal();

	if (nDlgRes == IDOK)
		AfxGetApp()->SelectPrinter(dlgPageSetup.m_psd.hDevNames, dlgPageSetup.m_psd.hDevMode, FALSE);
}

void CXTPCalendarControlView::CalendarDayView_ScrolV(int nTopRow)
{
	CXTPCalendarDayView* pDayView = DYNAMIC_DOWNCAST(CXTPCalendarDayView, GetCalendarCtrl().GetDayView());
	ASSERT(pDayView);
	if (pDayView)
		pDayView->_ScrollV(nTopRow, nTopRow);
}

