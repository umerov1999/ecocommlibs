// XTPRibbonBackstagePagePrint.cpp : implementation file
//
// This file is a part of the XTREME RIBBON MFC class library.
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

#include <Common/XTPDrawHelpers.h>
#include <Common/XTPResourceImage.h>
#include <Common/XTPVC80Helpers.h>
#include <Common/XTPColorManager.h>
#include <Common/XTPWinThemeWrapper.h>
#include <Common/XTPSystemHelpers.h>
#include <Common/XTPImageManager.h>
#include <Common/XTPHookManager.h>

#include <Controls/Util/XTPControlTheme.h>
#include <Controls/Button/XTPButton.h>
#include <Controls/Resize/XTPResizeRect.h>
#include <Controls/Resize/XTPResizePoint.h>
#include <Controls/Resize/XTPResize.h>
#include <Controls/Resize/XTPResizePropertyPage.h>

#include <CommandBars/Resource.h>
#include <CommandBars/XTPCommandBarsDefines.h>
#include <CommandBars/XTPCommandBars.h>
#include <CommandBars/XTPCommandBar.h>
#include <CommandBars/XTPToolBar.h>
#include <CommandBars/XTPControls.h>
#include <CommandBars/XTPControl.h>
#include <CommandBars/XTPPreviewView.h>

#include <Ribbon/Resource.h>
#include <Ribbon/Backstage/XTPRibbonBackstageButton.h>
#include <Ribbon/Backstage/XTPRibbonBackstageLabel.h>
#include <Ribbon/Backstage/XTPRibbonBackstageSeparator.h>
#include <Ribbon/Backstage/XTPRibbonBackstagePage.h>
#include <Ribbon/Backstage/XTPRibbonBackstageView.h>
#include <Ribbon/Backstage/Pages/XTPRibbonBackstagePagePrint.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTPRibbonBackstagePagePrint property page

IMPLEMENT_DYNCREATE(CXTPRibbonBackstagePagePrint, CXTPRibbonBackstagePage)

CXTPRibbonBackstagePagePrint::CXTPRibbonBackstagePagePrint() : CXTPRibbonBackstagePage(CXTPRibbonBackstagePagePrint::IDD)
{
	nTheme = xtpControlThemeOffice2013;

	m_btnPrint.SetTheme(nTheme);

	//{{AFX_DATA_INIT(CXTPRibbonBackstagePagePrint)
	m_nCopies = 1;
	//}}AFX_DATA_INIT

	m_pPreviewView = NULL;
	m_pFrameWnd = NULL;
	m_pView = NULL;
}

CXTPRibbonBackstagePagePrint::~CXTPRibbonBackstagePagePrint()
{
}

void CXTPRibbonBackstagePagePrint::DoDataExchange(CDataExchange* pDX)
{
	CXTPRibbonBackstagePage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CXTPRibbonBackstagePagePrint)
	DDX_Text(pDX, XTP_IDC_RIBBONBACKSTAGE_EDIT_COPIES, m_nCopies);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, XTP_IDC_RIBBONBACKSTAGE_SEPARATOR_4, m_lblSeparator4);
	DDX_Control(pDX, XTP_IDC_RIBBONBACKSTAGE_SEPARATOR_1, m_lblSeparator1);
	DDX_Control(pDX, XTP_IDC_RIBBONBACKSTAGE_BUTTON_PRINT, m_btnPrint);
	DDX_Control(pDX, XTP_IDC_RIBBONBACKSTAGE_CAPTION, m_lblPrint);

}


BEGIN_MESSAGE_MAP(CXTPRibbonBackstagePagePrint, CXTPRibbonBackstagePage)
	//{{AFX_MSG_MAP(CXTPRibbonBackstagePagePrint)
	ON_BN_CLICKED(XTP_IDC_RIBBONBACKSTAGE_BUTTON_PRINT, OnButtonPrint)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTPRibbonBackstagePagePrint message handlers



void CXTPRibbonBackstagePagePrint::InitButton(UINT nID)
{
	CXTPImageManagerIconHandle hIcon, hIconHot;

	hIcon.Attach(CXTPImageManagerIcon::LoadBitmapFromResource(MAKEINTRESOURCE(nID), NULL), FALSE);

	CXTPRibbonBackstageButton* pButton = (CXTPRibbonBackstageButton*)GetDlgItem(nID);

	pButton->SetIcon(CSize(32, 32), hIcon, hIconHot);
	pButton->SetTextImageRelation(xtpButtonImageAboveText);
	pButton->ShowShadow(TRUE);
}


BOOL CXTPRibbonBackstagePagePrint::OnInitDialog()
{
	CXTPRibbonBackstagePage::OnInitDialog();

	ModifyStyleEx(0, WS_EX_CONTROLPARENT);


	InitButton(XTP_IDC_RIBBONBACKSTAGE_BUTTON_PRINT);
	m_lblSeparator4.SetVerticalStyle(TRUE);

	m_lblPrint.SetFont(&m_fntCaption);
	m_lblPrint.SetTextColor(RGB(94, 94, 94));

	SetResize(XTP_IDC_RIBBONBACKSTAGE_SEPARATOR_4, XTP_ANCHOR_TOPLEFT, XTP_ANCHOR_BOTTOMLEFT);


	m_pFrameWnd = new CFrameWnd();
	if (!m_pFrameWnd->Create(NULL, NULL, WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
		CRect(0, 0, 0, 0), this, NULL, 0, NULL))
	{
		delete m_pFrameWnd;
		return FALSE;
	}

	m_pFrameWnd->ModifyStyleEx(WS_EX_CLIENTEDGE, 0);

	CXTPWindowRect rc(GetDlgItem(XTP_IDC_RIBBONBACKSTAGE_STATIC_PAGE));
	ScreenToClient(rc);

	m_pFrameWnd->SetWindowPos(0, rc.left, rc.top, rc.Width(), rc.Height(), SWP_NOZORDER | SWP_SHOWWINDOW);
	SetResize(0, m_pFrameWnd->GetSafeHwnd(), XTP_ANCHOR_TOPLEFT, XTP_ANCHOR_BOTTOMRIGHT);


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE

}

void CXTPRibbonBackstagePagePrint::UpdateCopies(BOOL bSaveAndValidate)
{
	HGLOBAL hDevMode = NULL, hDevNames = NULL;

	if (!XTPGetPrinterDeviceDefaults(hDevMode, hDevNames) || !hDevMode)
	{
		return;
	}

	LPDEVMODE pDevMode = (LPDEVMODE )::GlobalLock(hDevMode);
	ASSERT(pDevMode);
	if (!pDevMode)
		return;

	if (bSaveAndValidate)
	{
		UpdateData(bSaveAndValidate);

		pDevMode->dmCopies = (short)m_nCopies;
	}
	else
	{
		m_nCopies = pDevMode->dmCopies;
		UpdateData(FALSE);
	}

	::GlobalUnlock(hDevMode);
}

void CXTPRibbonBackstagePagePrint::OnButtonPrint()
{
	UpdateCopies(TRUE);

	CXTPRibbonBackstageView* pView = (CXTPRibbonBackstageView*)GetParent();

	pView->OnCancel();

	AfxGetMainWnd()->SendMessage(WM_COMMAND, ID_FILE_PRINT);

}


BOOL CXTPRibbonBackstagePagePrint::OnSetActive()
{
	if (!CXTPRibbonBackstagePage::OnSetActive())
		return FALSE;

	CXTPRibbonBackstageView* pBackstageView = (CXTPRibbonBackstageView*)GetParent();
	CFrameWnd* pSite = DYNAMIC_DOWNCAST(CFrameWnd, pBackstageView->GetCommandBars()->GetSite());

	m_pView = (CBackstagePrintView*)pSite->GetActiveFrame()->GetActiveView();
	if (!m_pView)
		return FALSE;

	if (!CreatePrintPreview())
		return FALSE;

	UpdateCopies(FALSE);

	return TRUE;
}

BOOL CXTPRibbonBackstagePagePrint::CreatePrintPreview()
{
	CFrameWnd* pParent = m_pFrameWnd;
	CPrintPreviewState* pState = new CPrintPreviewState;

	CCreateContext context;
	context.m_pCurrentFrame = m_pFrameWnd;
	context.m_pCurrentDoc = NULL;
	context.m_pLastView = m_pView;


	CBackstagePreviewView* pView = new CBackstagePreviewView();
	if (pView == NULL)
	{
		TRACE0("Error: Failed to create preview view.\n");
		return FALSE;
	}

	ASSERT_KINDOF(CPreviewView, pView);
	pView->m_pPreviewState = pState;        // save pointer

	pView->m_pToolBar = new CDialogBar;
	if (!pView->m_pToolBar->Create(m_pFrameWnd, MAKEINTRESOURCE(XTP_IDD_PREVIEW_DIALOGBAR),
		CBRS_TOP, AFX_IDW_PREVIEW_BAR))
	{
		TRACE0("Error: Preview could not create toolbar dialog.\n");
		delete pView->m_pToolBar;       // not autodestruct yet
		pView->m_pToolBar = NULL;
		pView->m_pPreviewState = NULL;  // do not delete state structure
		delete pView;
		return NULL;
	}

	pView->m_pToolBar->ModifyStyle(0, WS_CLIPCHILDREN);
	pView->m_pToolBar->m_bAutoDelete = TRUE;    // automatic cleanup


	if (!pView->Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
		CRect(0,0,0,0), pParent, AFX_IDW_PANE_FIRST, &context))
	{
		TRACE0("Error: couldn't create preview view for frame.\n");

		pView->m_pPreviewState = NULL;  // do not delete state structure
		delete pView;

		return NULL;
	}


	pView->SetPrintView(m_pView);


	pParent->SetActiveView(pView);  // set active view - even for MDI

	m_pPreviewView = pView;
	/*
	if (nTheme == xtpControlThemeOffice2013)
	{
		pView->m_pCommandBar->GetCommandBars()->SetTheme(xtpThemeOffice2013);
		pView->m_pCommandBar->GetCommandBars()->GetPaintManager()->SetImageHandle(XTPGetInstanceHandle(), _T("Office2013Word.ini"));
	}
	else*/
//      pView->m_pCommandBar->SetPaintManager(new CXTPRibbonTheme());

	pView->m_pCommandBar->GetControls()->FindControl(XTP_ID_PREVIEW_PRINT)->SetVisible(FALSE);
	pView->m_pCommandBar->GetControls()->FindControl(XTP_ID_PREVIEW_CLOSE)->SetVisible(FALSE);
	pView->m_clrBackColor = RGB(255, 255, 255);
	pView->ModifyStyleEx(WS_EX_CLIENTEDGE, 0);
	pView->RecalLayout();


	pParent->RecalcLayout();            // position and size everything
	pParent->UpdateWindow();

	m_pPreviewView->SetZoomState(0, 0, CPoint(0, 0));

	return TRUE;
}

BOOL CXTPRibbonBackstagePagePrint::OnKillActive()
{
	if (!CXTPRibbonBackstagePage::OnKillActive())
		return FALSE;

	if (m_pView && m_pPreviewView)
	{
		m_pPreviewView->m_pToolBar->DestroyWindow();
		m_pPreviewView->m_pToolBar = NULL;

		m_pView->OnEndPrinting(m_pPreviewView->m_pPreviewDC, m_pPreviewView->m_pPreviewInfo);
		m_pView = NULL;
	}

	if (m_pPreviewView)
	{
		m_pPreviewView->DestroyWindow();

		m_pFrameWnd->SetActiveView(NULL);

		m_pPreviewView = NULL;
	}


	return TRUE;
}
