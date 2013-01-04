// XTPPropertySheet.cpp : implementation file
//
// This file is a part of the XTREME CONTROLS MFC class library.
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
#include "../Resource.h"

#include "Common/XTPDrawHelpers.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPResourceManager.h"

#include "TabManager/XTPTabManager.h"
#include "TabManager/XTPTabControl.h"

#include "../Util/XTPControlTheme.h"
#include "../Resize/XTPResizeRect.h"
#include "../Resize/XTPResizePoint.h"
#include "../Resize/XTPResize.h"
#include "../ListBox/XTPListBox.h"

#include "XTPPropertyPage.h"
#include "XTPPropertySheet.h"
#include "XTPPropertyPageNavigator.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifndef WM_QUERYUISTATE
#define WM_QUERYUISTATE                 0x0129
#endif

#ifndef WM_CHANGEUISTATE
#define WM_CHANGEUISTATE                0x0127
#endif

#ifndef UISF_HIDEFOCUS
#define UISF_HIDEACCEL                  0x2
#define UISF_HIDEFOCUS                  0x1
#endif

#ifndef UIS_CLEAR
#define UIS_CLEAR                       2
#endif

#define ENTRY_WINDOWPLACEMENT _T("WindowPlacement")

/////////////////////////////////////////////////////////////////////////////
// CXTPPropertySheet dialog

IMPLEMENT_DYNAMIC(CXTPPropertySheet, CDialog)

CXTPPropertySheet::CXTPPropertySheet()
{
	CommonConstruct(NULL, 0);
}

CXTPPropertySheet::CXTPPropertySheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
{
	ASSERT(nIDCaption != 0);

	VERIFY(m_strCaption.LoadString(nIDCaption) != 0);
	CommonConstruct(pParentWnd, iSelectPage);
}

CXTPPropertySheet::CXTPPropertySheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
{
	ASSERT(pszCaption != NULL);

	m_strCaption = pszCaption;
	CommonConstruct(pParentWnd, iSelectPage);
}

void CXTPPropertySheet::SetNavigator(CXTPPropertyPageNavigator* pNavigator)
{
	ASSERT(pNavigator);
	if (!pNavigator)
		return;

	SAFE_DELETE(m_pNavigator);
	m_pNavigator = pNavigator;
	m_pNavigator->SetPropertySheet(this);
}

AFX_INLINE BOOL XTPHelpEnabled()
{
	if (AfxGetApp() == NULL)
		return FALSE;

	// help is enabled if the app has a handler for ID_HELP
	AFX_CMDHANDLERINFO info;

	// check main window first
	CWnd* pWnd = AfxGetMainWnd();
	if (pWnd != NULL && pWnd->OnCmdMsg(ID_HELP, CN_COMMAND, NULL, &info))
		return TRUE;

	// check app last
	return AfxGetApp()->OnCmdMsg(ID_HELP, CN_COMMAND, NULL, &info);
}


void CXTPPropertySheet::CommonConstruct(CWnd* pParentWnd, UINT iSelectPage)
{
	m_pNavigator = new CXTPPropertyPageTabNavigator();
	m_pNavigator->SetPropertySheet(this);

	m_pActivePage = NULL;
	m_rcPage.SetRectEmpty();
	m_bAllowApply = TRUE;
	m_bModeless = FALSE;
	m_bResizable = FALSE;

	m_szWindow = CSize(0, 0);
	m_szButton = CSize(0, 0);
	m_nPageBorderStyle = xtpPageBorderNone;

	m_pParentWnd = pParentWnd;  // m_psh.hwndParent set in DoModal/create

	memset(&m_psh, 0, sizeof(m_psh));
	m_psh.dwSize = sizeof(m_psh);
	m_psh.dwFlags = PSH_PROPSHEETPAGE;
	m_psh.pszCaption = m_strCaption;
	m_psh.nStartPage = iSelectPage;

	if (XTPHelpEnabled())
		m_psh.dwFlags |= PSH_HASHELP;

	const int nDlgTemplateSize = sizeof(DLGTEMPLATE) + sizeof(DWORD) * 8;
	m_lpDlgTemplate =  (DLGTEMPLATE*)malloc(nDlgTemplateSize);
	if (!m_lpDlgTemplate)
		return;
	ZeroMemory(m_lpDlgTemplate, nDlgTemplateSize);

	m_lpDlgTemplate->style =  WS_POPUP | WS_BORDER | WS_CAPTION | WS_SYSMENU |
		DS_NOIDLEMSG | DS_3DLOOK | DS_MODALFRAME | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;

	if (m_bResizable)
	{
		m_lpDlgTemplate->style |= WS_THICKFRAME;
	}

	m_lpDlgTemplate->dwExtendedStyle = WS_EX_DLGMODALFRAME;

	InitModalIndirect(m_lpDlgTemplate, m_pParentWnd);
}

void CXTPPropertySheet::SetResizable(BOOL bResizable)
{
	ASSERT(!m_hWnd);
	m_bResizable = bResizable;

	if (m_bResizable)
	{
		m_lpDlgTemplate->style |= WS_THICKFRAME;
	}
	else
	{
		m_lpDlgTemplate->style &= ~WS_THICKFRAME;
	}
}

CXTPPropertySheet::~CXTPPropertySheet()
{
	DestroyWindow();

	free(m_lpDlgTemplate);

	delete m_pNavigator;
}

BOOL CXTPPropertySheet::IsWizard() const
{
	// Not implemented
	return FALSE; //m_psh.dwFlags & (PSH_WIZARD | PSH_WIZARD97);
}

AFX_INLINE CSize GetBorderSize(XTPPropertyPageBorder style)
{
	return style == xtpPageBorderFrame ? CSize(4, 4) :
		style == xtpPageBorderBottomLine ? CSize(0, 2) : CSize(0, 0);
}

void CXTPPropertySheet::SetPageSize(CSize szPage)
{
	if (m_hWnd)
	{
		szPage += GetBorderSize(m_nPageBorderStyle);
		RecalcLayout(&szPage);
	}
	else
	{
		m_rcPage = CRect(0, 0, szPage.cx, szPage.cy);
	}
}


void CXTPPropertySheet::SetPageBorderStyle(XTPPropertyPageBorder nBorder)
{
	CSize szPage = m_rcPage.Size() - GetBorderSize(m_nPageBorderStyle);
	m_nPageBorderStyle = nBorder;
	szPage += GetBorderSize(m_nPageBorderStyle);

	RecalcLayout(&szPage);
}

BOOL CXTPPropertySheet::Create(CWnd* pParentWnd, DWORD dwStyle, DWORD dwExStyle)
{
	if (dwStyle == (DWORD)-1)
	{
		dwStyle = DS_MODALFRAME | DS_3DLOOK | WS_POPUP | WS_VISIBLE | WS_CAPTION |
			WS_CLIPCHILDREN | WS_CLIPSIBLINGS;

		// Wizards don't have WS_SYSMENU.
		if (!IsWizard())
			dwStyle |= WS_SYSMENU;
	}

	if (m_bResizable)
	{
		dwStyle |= WS_THICKFRAME;
	}

	m_lpDlgTemplate->style =  dwStyle;
	m_lpDlgTemplate->dwExtendedStyle = dwExStyle;


	m_bModeless = TRUE;
	m_psh.dwFlags |= (PSH_MODELESS|PSH_USECALLBACK);

	return CDialog::CreateDlgIndirect(m_lpDlgTemplate, pParentWnd, NULL);
}

void CXTPPropertySheet::AddPage(CXTPPropertyPage* pPage)
{
	InsertPage((int)m_arrPages.GetSize(), pPage);
}

void CXTPPropertySheet::InsertPage(int nIndex, CXTPPropertyPage* pPage)
{
	ASSERT(pPage->m_pSheet == NULL);

	if (nIndex == -1) nIndex = (int)m_arrPages.GetSize();

	m_arrPages.InsertAt(nIndex, pPage);
	pPage->m_pSheet = this;

	if (m_hWnd)
	{
		CreatePage(pPage);

		m_pNavigator->OnPageChanged();
		m_pNavigator->OnPageSelected(GetActivePage());

		CSize szPage = m_rcPage.Size();
		RecalcLayout(&szPage);
	}
}

void CXTPPropertySheet::RemovePage(CXTPPropertyPage* pPage)
{
	if (!pPage)
		return;

	if (pPage->m_pSheet != this)
		return;

	pPage->m_pSheet = NULL;

	int nIndex = GetPageIndex(pPage);
	m_arrPages.RemoveAt(nIndex);

	if (pPage == m_pActivePage)
	{
		SetActivePage(nIndex);
	}

	if (m_hWnd)
	{
		pPage->DestroyWindow();

		m_pNavigator->OnPageChanged();
		m_pNavigator->OnPageSelected(GetActivePage());

		CSize szPage = m_rcPage.Size();
		RecalcLayout(&szPage);
	}
}

void CXTPPropertySheet::RemovePage(int nPage)
{
	RemovePage(GetPage(nPage));

}



void CXTPPropertySheet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CXTPPropertySheet)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}



BEGIN_MESSAGE_MAP(CXTPPropertySheet, CWnd) // Not CDialog!
	//{{AFX_MSG_MAP(CXTPPropertySheet)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_MESSAGE(WM_COMMANDHELP, OnCommandHelp)
	ON_WM_SYSCOMMAND()
	ON_WM_CLOSE()
	//ON_MESSAGE(WM_INITDIALOG, HandleInitDialog)
	ON_WM_PAINT()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTPPropertySheet message handlers

int CXTPPropertySheet::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	SetWindowText(m_strCaption);

	if (m_arrPages.GetSize() == 0)
	{
		return -1;
	}

	if (!CreateClient())
	{
		TRACE(_T("Can't create CXTPPropertySheet client"));
		return -1;
	}

	return 0;
}

BOOL CXTPPropertySheet::AutoLoadPlacement(LPCTSTR pszSection)
{
	m_strSection = pszSection;
	ASSERT(!m_strSection.IsEmpty());

	if (!m_hWnd)
		return TRUE;

	return LoadPlacement(m_strSection);
}

BOOL CXTPPropertySheet::LoadPlacement(LPCTSTR pszSection)
{
	BOOL bResult = FALSE;
	UINT nBytes = 0;
	BYTE* pBytes = 0;
	AfxGetApp()->GetProfileBinary(pszSection, ENTRY_WINDOWPLACEMENT, &pBytes, &nBytes);
	if (nBytes == sizeof(WINDOWPLACEMENT))
	{
		bResult = SetWindowPlacement((WINDOWPLACEMENT*) pBytes);
	}
	if (pBytes && nBytes) delete[] pBytes;

	return bResult;
}

BOOL CXTPPropertySheet::SavePlacement(LPCTSTR pszSection)
{
	WINDOWPLACEMENT wp;
	if (!GetWindowPlacement(&wp))
		return FALSE;

	AfxGetApp()->WriteProfileBinary(pszSection, ENTRY_WINDOWPLACEMENT, (BYTE*) &wp, sizeof(wp));
	return TRUE;
}

void CXTPPropertySheet::OnDestroy()
{
	// Save window placement if AutoLoadPlacement was used.
	if (!m_strSection.IsEmpty())
	{
		SavePlacement(m_strSection);
	}

	CDialog::OnDestroy();

	RemoveButtons();

	m_rcPage.SetRectEmpty();
	m_pActivePage = NULL;
}


BOOL CXTPPropertySheet::CreatePage(CXTPPropertyPage* pPage)
{
	return pPage->CreatePage(this);
}

BOOL CXTPPropertySheet::IsPositionRelativeToWindow()
{
	return m_pParentWnd != 0;
}

void CXTPPropertySheet::ResizeParentToFit(CSize szClient, BOOL bMoveWindow)
{
	CRect rcParent = IsPositionRelativeToWindow()?
		CXTPWindowRect(m_pParentWnd): XTPMultiMonitor()->GetWorkArea(m_pParentWnd);

	CPoint ptPos((rcParent.left + rcParent.right - szClient.cx) / 2 - 3,
		(rcParent.top + rcParent.bottom - szClient.cy) / 2 - 12);

	CRect rcWindow(ptPos, szClient);

	AdjustWindowRectEx(rcWindow, GetStyle(), FALSE, GetExStyle());

	m_szWindow  = rcWindow.Size();

	if (bMoveWindow)
		MoveWindow(rcWindow);
	else
		SetWindowPos(0, 0, 0, rcWindow.Width(), rcWindow.Height(), SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
}

void CXTPPropertySheet::RemoveButtons()
{
	for (int i = 0; i < m_arrButtons.GetSize(); i++)
	{
		delete m_arrButtons[i];
	}
	m_arrButtons.RemoveAll();
}

CButton* CXTPPropertySheet::CreateButton()
{
	return new CButton();
}

BOOL CXTPPropertySheet::AddButton(UINT nIDCaption, UINT nID)
{
	CString strButton;
	CXTPResourceManager::AssertValid(XTPResourceManager()->LoadString(&strButton, nIDCaption));

	CButton* pButton = CreateButton();

	if (!pButton->Create(strButton, WS_CHILD | BS_PUSHBUTTON | WS_TABSTOP | WS_VISIBLE, CRect(0, 0, 0, 0), this, nID))
	{
		delete pButton;
		return FALSE;
	}

	pButton->SendMessage(WM_SETFONT, (WPARAM)(HFONT)m_fntNormal, FALSE);

	m_arrButtons.Add(pButton);
	return TRUE;
}

BOOL CXTPPropertySheet::CreateButtons()
{
	if (m_bModeless && !IsWizard())
		return TRUE;

	if (!AddButton(XTP_IDS_TASK_OK, IDOK))
		return FALSE;

	if (!AddButton(XTP_IDS_TASK_CANCEL, IDCANCEL))
		return FALSE;

	if (!(m_psh.dwFlags & PSH_NOAPPLYNOW))
	{
		if (!AddButton(XTP_IDS_TASK_APPLY, ID_APPLY_NOW))
			return FALSE;
	}

	if (m_psh.dwFlags & PSH_HASHELP)
	{
		if (!AddButton(XTP_IDS_TASK_HELP, IDHELP))
			return FALSE;
	}

	return TRUE;
}

void CXTPPropertySheet::PageInfoChanged(CXTPPropertyPage* pPage, BOOL bModified)
{
	if (pPage)
	{
		pPage->m_bModified = bModified;
	}

	bModified = FALSE;

	for (int i = 0; i < GetPageCount(); i++)
	{
		if (GetPage(i)->m_bModified)
		{
			bModified = TRUE;
			break;
		}
	}

	if (m_bAllowApply && GetDlgItem(ID_APPLY_NOW)) GetDlgItem(ID_APPLY_NOW)->EnableWindow(bModified);
}

void CXTPPropertySheet::MoveButtons(AFX_SIZEPARENTPARAMS* pLayout, CSize szClient)
{
	int nRight = szClient.cx - m_pNavigator->m_rcButtonMargin.right;
	int nTop = szClient.cy + m_pNavigator->m_rcButtonMargin.top;

	CPoint ptButtonPadding  = CXTPDrawHelpers::Dlu2Pix(3, 5);

	CClientDC dc(this);
	CXTPFontDC font(&dc, &m_fntNormal);

	CSize szButton = dc.GetTextExtent(_T("XXXXXXXXXX"));
	szButton.cy += ptButtonPadding.y;

	for (int i = (int)m_arrButtons.GetSize() - 1; i >= 0; i--)
	{
		CButton* pButton = m_arrButtons[i];

		CString strText;
		pButton->GetWindowText(strText);
		CSize szText = dc.GetTextExtent(strText);

		int nButtonWidth = max(szButton.cx, szText.cx) + ptButtonPadding.x;

		//pButton->SetWindowPos(&CWnd::wndTop, nRight - nButtonWidth,
		//  nTop, nButtonWidth, szButton.cy, SWP_SHOWWINDOW);
		AfxRepositionWindow(pLayout, pButton->GetSafeHwnd(),
			CRect(nRight - nButtonWidth, nTop, nRight, nTop + szButton.cy));


		nRight -= nButtonWidth + m_pNavigator->m_rcButtonMargin.left;
	}
	m_szButton = szButton;
}


void CXTPPropertySheet::RecalcLayout(LPSIZE pPageSize, BOOL bMoveWindow /*= FALSE*/)
{
	if (!m_hWnd)
		return;

	AFX_SIZEPARENTPARAMS layout;
	layout.hDWP = ::BeginDeferWindowPos((int)m_arrButtons.GetSize() + 3);

	CRect rcPage;
	CSize szClient;
	m_pNavigator->PageToSize(&layout, pPageSize ? CSize(*pPageSize) : m_rcPage.Size(), rcPage, szClient);

	if (m_arrButtons.GetSize() > 0)
	{
		MoveButtons(&layout, szClient);
		szClient.cy += m_szButton.cy + m_pNavigator->m_rcButtonMargin.top + m_pNavigator->m_rcButtonMargin.bottom;
	}
	else
	{
		m_szButton = CSize(0, 0);
	}
	m_rcPage = rcPage;

	if (m_pActivePage)
	{
		AfxRepositionWindow(&layout, m_pActivePage->GetSafeHwnd(), m_rcPage);
	}

	if (m_scSizeIcon.GetSafeHwnd())
	{
		CRect rcSizeIcon(szClient.cx - GetSystemMetrics(SM_CXVSCROLL), szClient.cy - GetSystemMetrics(SM_CYHSCROLL), szClient.cx, szClient.cy);
		AfxRepositionWindow(&layout, m_scSizeIcon, rcSizeIcon);
	}

	::EndDeferWindowPos(layout.hDWP);

	ResizeParentToFit(szClient, bMoveWindow);
}

void CXTPPropertySheet::CreateFont(CXTPPropertyPage* pActivePage)
{
	HFONT hFont = (HFONT)pActivePage->SendMessage(WM_GETFONT);

	m_fntNormal.DeleteObject();

	LOGFONT lf;
	if (!hFont || !GetObject(hFont, sizeof(LOGFONT), &lf))
	{
		VERIFY(CXTPDrawHelpers::GetIconLogFont(&lf));
	}

	m_fntNormal.CreateFontIndirect(&lf);

	SendMessage(WM_SETFONT, (WPARAM)(HFONT)m_fntNormal, FALSE);
}

BOOL CXTPPropertySheet::CreateClient()
{
	CSize szPage(0, 0);

	CXTPPropertyPage* pActivePage = m_arrPages[0];

	for (UINT i = 0; i < (UINT)m_arrPages.GetSize(); i++)
	{
		CXTPPropertyPage* pPage = m_arrPages[i];

		if (!pPage->LoadTemplate())
			return FALSE;

		if (i == m_psh.nStartPage)
			pActivePage = pPage;


		CSize szPageTotal(pPage->m_totalDev);
		szPage.cx = max(szPage.cx, szPageTotal.cx);
		szPage.cy = max(szPage.cy, szPageTotal.cy);
	}

	if (!CreatePage(pActivePage))
		return FALSE;


	CreateFont(pActivePage);

	if (!m_rcPage.IsRectEmpty())
	{
		pActivePage->MapDialogRect(m_rcPage);
		szPage = m_rcPage.Size();
	}


	// Create the size icon if it already doesn't exist.
	if (m_bResizable)
	{
		m_scSizeIcon.Create(WS_CHILD | WS_VISIBLE | SBS_SIZEGRIP, CRect(0, 0, 0, 0), this, 1005);
	}

	if (!CreateButtons())
		return FALSE;

	if (!m_pNavigator->Create())
		return FALSE;

	SetActivePage(pActivePage);

	PageInfoChanged(NULL, FALSE);

	szPage += GetBorderSize(m_nPageBorderStyle);
	RecalcLayout(&szPage, TRUE);

	return TRUE;
}

LRESULT CXTPPropertySheet::HandleInitDialog(WPARAM, LPARAM)
{
	Default();
	LRESULT lResult = OnInitDialog();
	return lResult;
}


BOOL CXTPPropertySheet::OnInitDialog()
{
	if (m_pActivePage)
	{
		HWND hwndFocus = ::GetNextDlgTabItem(m_pActivePage->GetSafeHwnd(), NULL, FALSE);
		SendMessage(WM_NEXTDLGCTL, (WPARAM)hwndFocus, MAKELPARAM(TRUE, 0));
	}

	if (!m_strSection.IsEmpty())
	{
		LoadPlacement(m_strSection);
	}

	return FALSE;
}


int CXTPPropertySheet::GetPageIndex(CXTPPropertyPage* pPage) const
{
	for (int i = 0; i < GetPageCount(); i++)
	{
		if (GetPage(i) == pPage)
			return i;
	}
	return -1;  // pPage not found
}

BOOL CXTPPropertySheet::SetActivePage(int nPage)
{
	if (m_hWnd == NULL)
	{
		m_psh.nStartPage = nPage;
		return TRUE;
	}
	return SetActivePage(GetPage(nPage));
}

void CXTPPropertySheet::SetTitle(LPCTSTR lpszText)
{
	m_strCaption = lpszText;

	if (m_hWnd)
	{
		SetWindowText(m_strCaption);
	}
}

BOOL CXTPPropertySheet::SetActivePage(CXTPPropertyPage* pPage)
{
	if (!pPage)
		return FALSE;

	if (pPage == m_pActivePage)
		return TRUE;

	if (pPage->m_pSheet != this)
		return FALSE;

	if (!m_hWnd)
	{
		m_psh.nStartPage = GetPageIndex(pPage);
		return TRUE;
	}

	CXTPPropertyPage* pActivePage = m_pActivePage;

	if (pActivePage)
	{
		if (!pActivePage->OnKillActive())
			return FALSE;
	}

	if (!CreatePage(pPage) || !pPage->OnSetActive())
	{
		if (pActivePage) pActivePage->OnSetActive();
		return FALSE;
	}

	m_pActivePage = pPage;

	m_pNavigator->OnPageSelected(pPage);

	pPage->SetWindowPos(&CWnd::wndTop, m_rcPage.left, m_rcPage.top, m_rcPage.Width(), m_rcPage.Height(), SWP_SHOWWINDOW);

	if (pActivePage)
	{
		pActivePage->ShowWindow(SW_HIDE);
	}

	if (::GetFocus() != m_pNavigator->GetSafeHwnd())
	{
		HWND hwndFocus = ::GetNextDlgTabItem(pPage->GetSafeHwnd(), NULL, FALSE);
		::SetFocus(hwndFocus);
	}

	return TRUE;
}

BOOL CXTPPropertySheet::OnButtonClick(UINT nButton)
{
	BOOL bExit = FALSE;
	LPARAM lParam = FALSE;
	int nReturnNew = 0;
	int nNotify;
	int fSuccess = TRUE;
	LRESULT lResult = PSNRET_INVALID_NOCHANGEPAGE;

	switch (nButton)
	{
	case IDOK:
		lParam = TRUE;
		bExit = TRUE;

	case ID_APPLY_NOW:
		// First allow the current dialog to validate itself.
		if (SendPageNotify(GetActivePage(), PSN_KILLACTIVE, 0))
			return FALSE;

		nReturnNew = 1;
		nNotify = PSN_APPLY;
		break;

	case IDCLOSE:
		lParam = TRUE;
		// fall through
	case IDCANCEL:
		bExit = TRUE;
		nNotify = PSN_RESET;
		break;

	default:
		return FALSE;
	}

	int nItem = 0;

	for (nItem = 0; nItem < GetPageCount(); nItem++)
	{
		CXTPPropertyPage* pPage = GetPage(nItem);

		lResult = SendPageNotify(pPage, nNotify, lParam);

		if (lResult)
		{
			fSuccess = FALSE;
			bExit = FALSE;
			break;
		}
		else
		{
		}


		pPage->m_bModified = FALSE;
	}

	if (fSuccess)
	{

	}
	else if (lResult != PSNRET_INVALID_NOCHANGEPAGE)
	{
		SetActivePage(nItem);
	}



	if (!bExit)
	{
		if (fSuccess)
		{
			if (GetDlgItem(ID_APPLY_NOW)) GetDlgItem(ID_APPLY_NOW)->EnableWindow(FALSE);
		}
	}

	if (fSuccess && bExit)
	{
		EndDialog(nButton);
	}


	return fSuccess;
}

LRESULT CXTPPropertySheet::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == PSM_CHANGED)
	{
		PageInfoChanged((CXTPPropertyPage*)CWnd::FromHandle((HWND)wParam), TRUE);
		return 1;
	}

	if (message == PSM_UNCHANGED)
	{
		PageInfoChanged((CXTPPropertyPage*)CWnd::FromHandle((HWND)wParam), FALSE);
		return 1;
	}

	if (message == PSM_APPLY)
	{
		return OnButtonClick(ID_APPLY_NOW);
	}

	if (message == PSM_QUERYSIBLINGS)
	{
		return OnQuerySiblings(wParam, lParam);
	}

	return CDialog::WindowProc(message, wParam, lParam);
}

LRESULT CXTPPropertySheet::SendPageNotify(CXTPPropertyPage* pPage, int code, LPARAM lParam)
{
	PSHNOTIFY pshn;

	pshn.lParam = lParam;

	pshn.hdr.hwndFrom = GetSafeHwnd();
	pshn.hdr.idFrom = GetDlgCtrlID();
	pshn.hdr.code = code;

	if (!pPage || !pPage->GetSafeHwnd())
		return 0;

	return pPage->SendMessage(WM_NOTIFY, pshn.hdr.idFrom, (LPARAM)&pshn);
}


BOOL CXTPPropertySheet::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// crack message parameters
	UINT nID = LOWORD(wParam);
	HWND hWndCtrl = (HWND)lParam;
	int nCode = HIWORD(wParam);

	// set m_nModalResult to ID of button, whenever button is clicked
	if (hWndCtrl != NULL && nCode == BN_CLICKED)
	{
		if (::SendMessage(hWndCtrl, WM_GETDLGCODE, 0, 0) &
			(DLGC_BUTTON|DLGC_DEFPUSHBUTTON))
		{
			LONG lStyle = ::GetWindowLong(hWndCtrl, GWL_STYLE) & 0x0F;
			if (lStyle == BS_PUSHBUTTON || lStyle == BS_DEFPUSHBUTTON ||
				lStyle == BS_USERBUTTON || lStyle == BS_OWNERDRAW)
			{
				m_nModalResult = nID;
			}
		}
	}


	if (lParam && (::GetParent((HWND)lParam) == m_hWnd))
	{

		if ((wParam == ID_APPLY_NOW) || (wParam == IDOK))
		{
			if (OnButtonClick((UINT)wParam))
			{

			}
			return TRUE;
		}

		if ((wParam == IDCLOSE) || (wParam == IDCANCEL))
		{
			if (SendPageNotify(GetActivePage(), PSN_QUERYCANCEL) == 0)
			{
				OnButtonClick((UINT)wParam);
			}
			return TRUE;
		}

		if (wParam == IDHELP)
		{
			SendPageNotify(GetActivePage(), PSN_HELP);
			return TRUE;
		}
	}

	if (GetActivePage()->OnCommand(wParam, lParam))
		return TRUE;

	// allow message map override
	if (CWnd::OnCommand(wParam, lParam))
		return TRUE;

	return FALSE;
}

LRESULT CXTPPropertySheet::OnCommandHelp(WPARAM wParam, LPARAM lParam)
{
	ASSERT_VALID(this);

	CPropertyPage* pPage = GetActivePage();
	ASSERT_VALID(pPage);

	return pPage->SendMessage(WM_COMMANDHELP, wParam, lParam);
}

void CXTPPropertySheet::EndDialog(int nEndID)
{
	CWnd::EndModalLoop(nEndID);
	if (m_bModeless)
		DestroyWindow();
	else
		PostMessage(PSM_PRESSBUTTON, PSBTN_CANCEL);

}

void CXTPPropertySheet::OnClose()
{
	m_nModalResult = IDCANCEL;
	if (m_bModeless)
		DestroyWindow();
	else
		Default();
}

void CXTPPropertySheet::OnSysCommand(UINT nID, LPARAM /*lParam*/)
{
	m_nModalResult = IDCANCEL;
	switch (nID & 0xFFF0)
	{
	case SC_CLOSE:
		if (m_bModeless)
		{
			SendMessage(WM_CLOSE);
			return;
		}
	}
	Default();
}

TCHAR CXTPPropertySheet::ToUpper(TCHAR vkTCHAR)
{
	TCHAR szChar[2] = {vkTCHAR, _T('\0') };

	CharUpper(szChar);
	return szChar[0];
}

BOOL CXTPPropertySheet::PreTranslateMessage(MSG* pMsg)
{
	// allow sheet to translate Ctrl+Tab, Shift+Ctrl+Tab,
	//  Ctrl+PageUp, and Ctrl+PageDown
	if (pMsg->message == WM_KEYDOWN && GetAsyncKeyState(VK_CONTROL) < 0 &&
		(pMsg->wParam == VK_TAB || pMsg->wParam == VK_PRIOR || pMsg->wParam == VK_NEXT))
	{
		BOOL bBack = FALSE;
		if (pMsg->wParam == VK_PRIOR || ((pMsg->wParam == VK_TAB) && (GetKeyState(VK_SHIFT) < 0)))
			bBack = TRUE;

		SendMessage(WM_CHANGEUISTATE, MAKELONG(UIS_CLEAR, UISF_HIDEFOCUS | UISF_HIDEACCEL), 0);

		if (IsWizard())
		{

		}
		else
		{
			int nIndex = GetActiveIndex();
			// tab in reverse if shift is down
			if (bBack)
				nIndex += (GetPageCount() - 1);
			else
				nIndex++;

			nIndex %= GetPageCount();
			SetActivePage(nIndex);
		}

		return TRUE;
	}


	if (pMsg->message == WM_SYSKEYDOWN)
	{
		for (int i = 0; i < GetPageCount(); i++)
		{
			CString strCaption = GetPage(i)->GetCaption();
			const TCHAR* pAmp = _tcsrchr(strCaption, _T('&'));
			if (pAmp)
			{
				if (ToUpper(*(pAmp + 1)) == ToUpper((TCHAR)pMsg->wParam))
				{
					SetActivePage(i);
					return TRUE;
				}
			}
		}
	}


	if (pMsg->message < WM_KEYFIRST || pMsg->message > WM_KEYLAST)
		return CDialog::PreTranslateMessage(pMsg);

	HWND hWndFocusOld = ::GetFocus();

	if (!CDialog::PreTranslateMessage(pMsg))
		return FALSE;

	HWND hWndFocusNew = ::GetFocus();

	if (hWndFocusNew != hWndFocusOld && m_hWnd)
	{
		if (::IsChild(m_pActivePage->GetSafeHwnd(), hWndFocusNew))
		{
			m_pActivePage->OnChildSetFocus(hWndFocusNew);
		}
	}

	return TRUE;
}

void CXTPPropertySheet::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	//m_pNavigator->OnSheetPaint(&dc);
}

LRESULT CXTPPropertySheet::OnQuerySiblings(WPARAM wParam, LPARAM lParam)
{
	for (int i = 0; i < GetPageCount(); i++)
	{
		LRESULT lres = GetPage(i)->SendMessage(PSM_QUERYSIBLINGS, wParam, lParam);
		if (lres)
			return lres;
	}
	return FALSE;
}

void CXTPPropertySheet::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	CSize szWindow = CXTPWindowRect(this).Size();
	if (m_szWindow != szWindow && m_bResizable && m_pActivePage)
	{
		CSize szClient(cx, cy);

		AFX_SIZEPARENTPARAMS layout;
		layout.hDWP = ::BeginDeferWindowPos((int)m_arrButtons.GetSize() + 3);

		if (m_scSizeIcon.GetSafeHwnd())
		{
			CRect rcSizeIcon(szClient.cx - GetSystemMetrics(SM_CXVSCROLL), szClient.cy - GetSystemMetrics(SM_CYHSCROLL), szClient.cx, szClient.cy);
			AfxRepositionWindow(&layout, m_scSizeIcon, rcSizeIcon);
		}

		if (m_szButton != CSize(0, 0))
		{
			szClient.cy -= m_szButton.cy + m_pNavigator->m_rcButtonMargin.top + m_pNavigator->m_rcButtonMargin.bottom;
			MoveButtons(&layout, szClient);
		}

		m_pNavigator->SizeToPage(&layout, szClient, m_rcPage);

		m_szWindow = szWindow;

		AfxRepositionWindow(&layout, m_pActivePage->GetSafeHwnd(), m_rcPage);

		::EndDeferWindowPos(layout.hDWP);
	}
}

HWND CXTPPropertySheet::GetNavigatorHwnd() const
{
	return GetNavigator()->GetSafeHwnd();
}
