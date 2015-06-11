// XTPBrowseEdit.cpp : implementation of the CXTPBrowseEdit class.
//
// This file is a part of the XTREME CONTROLS MFC class library.
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
#include "../Resource.h"

#include "Common/XTPResourceManager.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPMaskEditT.h"

#include "../Util/XTPControlTheme.h"

#include "XTPEdit.h"
#include "../Button/XTPButton.h"
#include "../Util/XTPFunctions.h"
#include "../Defines.h"
#include "../Util/XTPGlobal.h"
#include "XTPBrowseEdit.h"
#include "../Dialog/XTPBrowseDialog.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int CX_BUTTON = (::GetSystemMetrics(SM_CXHTHUMB) +
	(::GetSystemMetrics(SM_CXEDGE) * 2));

int CXTPBrowseEdit::m_nDefaultGap = -2;


/////////////////////////////////////////////////////////////////////////////
// CXTPBrowseButton class
/////////////////////////////////////////////////////////////////////////////

CXTPBrowseButton::CXTPBrowseButton()
{
	m_nBorderGap = 0;
	m_nImageGap = 0;
	m_dwBStyle = 0;
	m_pEditWnd = NULL;
	m_nID = 0;
	m_nMenu = 0;
	m_nSubMenuIndx = 0;
}

CXTPBrowseButton::~CXTPBrowseButton()
{
}

IMPLEMENT_DYNAMIC(CXTPBrowseButton, CXTPButton)

BEGIN_MESSAGE_MAP(CXTPBrowseButton, CXTPButton)
	//{{AFX_MSG_MAP(CXTPBrowseButton)
	ON_WM_ENABLE()
	ON_CONTROL_REFLECT(BN_CLICKED, OnBtnClicked)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CXTPBrowseButton::Create(CXTPBrowseEdit* pEditWnd, DWORD dwBStyle, UINT nID, UINT nMenu/*= -1*/, UINT nSubMenuIndx/*= -1*/)
{
	ASSERT_VALID(pEditWnd);              // Must be valid
	ASSERT_VALID(pEditWnd->GetParent());

	// Set the pointers to the edit and edit parent windows.
	m_pEditWnd = pEditWnd;

	// Set the browse style and menu resource
	// ID information (if any).
	m_dwBStyle = dwBStyle;
	m_nMenu = nMenu;
	m_nSubMenuIndx = nSubMenuIndx;

	CString strButtonText;

	DWORD dwStyle = (m_pEditWnd->GetStyle() & WS_VISIBLE) |
		WS_CHILD | WS_TABSTOP | BS_CENTER | BS_VCENTER;

	HICON hPopupIcon = NULL;

	if ((m_dwBStyle & BES_XTP_POPUPMENU) == 0)
	{
		CXTPResourceManager::AssertValid(XTPResourceManager()->LoadString(&strButtonText, XTP_IDS_DOT));
	}
	else
	{
		CImageList imageList;
		imageList.Create(11, 11, ILC_COLOR8 | ILC_MASK, 0, 1);

		CBitmap bmp;
		CXTPResourceManager::AssertValid(XTPResourceManager()->LoadBitmap(&bmp, XTP_IDB_SCROLL_ARW));
		imageList.Add(&bmp, RGB(0xff, 0x00, 0xff));

		if (imageList.GetImageCount() == 8)
		{
			// extract the right arrow icon from the image list.
			hPopupIcon = imageList.ExtractIcon(4);
			ASSERT(hPopupIcon != NULL);

			// add the BS_ICON style to create flags.
			dwStyle |= BS_ICON;
		}
	}

	if (!CXTPButton::Create(strButtonText, dwStyle, CRect(0, 0, 0, 0),
		m_pEditWnd->GetParent(), nID))
	{
		TRACE0("Failed to create browse edit button.\n");
		return FALSE;
	}

	// Set the font for this control.
	if (!strButtonText.IsEmpty())
		SetFont(&XTPAuxData().font);

	// set the arrow icon that is displayed on the button.
	if (NULL != (HICON)hPopupIcon)
	{
		SetIcon(CSize(10, 10), hPopupIcon);
		DestroyIcon(hPopupIcon);
	}



	// Enable the button using the parent's enabled state.
	EnableWindow(m_pEditWnd->IsWindowEnabled());

	return TRUE;
}

void CXTPBrowseButton::SetBrowseStyle(DWORD dwxStyle)
{
	m_dwBStyle = dwxStyle;

	if ((m_dwBStyle & BES_XTP_POPUPMENU) == 0)
	{
		CString strButtonText;
		CXTPResourceManager::AssertValid(XTPResourceManager()->LoadString(&strButtonText, XTP_IDS_DOT));
		SetWindowText(strButtonText);
	}
	else
	{
		CImageList imageList;
		imageList.Create(11, 11, ILC_COLOR8 | ILC_MASK, 0, 1);

		CBitmap bmp;
		CXTPResourceManager::AssertValid(XTPResourceManager()->LoadBitmap(&bmp, XTP_IDB_SCROLL_ARW));
		imageList.Add(&bmp, RGB(0xff, 0x00, 0xff));

		if (imageList.GetImageCount() == 8)
		{
			// extract the right arrow icon from the image list.
			HICON hPopupIcon = imageList.ExtractIcon(4);
			ASSERT(hPopupIcon != NULL);

			// add the BS_ICON style to create flags.
			ModifyStyle(0, BS_ICON);

			// set the arrow icon that is displayed on the button.
			if (NULL != (HICON)hPopupIcon)
			{
				SetIcon(CSize(10, 10), hPopupIcon);

				DestroyIcon(hPopupIcon);
			}
		}
	}
}

BOOL CXTPBrowseButton::PreTranslateMessage(MSG* pMsg)
{
	// activate if space or enter key pressed...
	if (pMsg->message == WM_KEYDOWN &&
		(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_SPACE))
	{
		OnClicked();
		return TRUE; // eat message...
	}

	return CXTPButton::PreTranslateMessage(pMsg);
}

void CXTPBrowseButton::OnBtnClicked()
{
	OnClicked();
}

void CXTPBrowseButton::OnClicked()
{
	if (m_pEditWnd)
	{
		m_pEditWnd->OnBrowse();
	}
}

void CXTPBrowseButton::OnEnable(BOOL bEnable)
{
	CXTPButton::OnEnable(bEnable);
	Invalidate();
}

/////////////////////////////////////////////////////////////////////////////
// CXTPBrowseEdit class
/////////////////////////////////////////////////////////////////////////////

CXTPBrowseEdit::CXTPBrowseEdit()
{
	m_nGap = m_nDefaultGap;
	m_dwBStyle = BES_XTP_CHOOSEDIR;
	m_nMenu = 0;
	m_nSubMenuIndx = 0;
	m_strFileName = _T("");
	m_dwFlags = OFN_HIDEREADONLY | OFN_FILEMUSTEXIST | OFN_EXPLORER;
	m_pFileDialog = NULL;
	m_bOpenFileDialog = TRUE;
	m_bBrowsing = false;

	CXTPResourceManager::AssertValid(XTPResourceManager()->LoadString(&m_strDefExt, XTP_IDS_DEFEXT));
	CXTPResourceManager::AssertValid(XTPResourceManager()->LoadString(&m_strFilter, XTP_IDS_DEFFILT));
	CXTPResourceManager::AssertValid(XTPResourceManager()->LoadString(&m_strTitle, XTP_IDS_DEFTITLE));
}

CXTPBrowseEdit::~CXTPBrowseEdit()
{
}

IMPLEMENT_DYNAMIC(CXTPBrowseEdit, CXTPEdit)

BEGIN_MESSAGE_MAP(CXTPBrowseEdit, CXTPEdit)
	//{{AFX_MSG_MAP(CXTPBrowseEdit)
	ON_WM_ENABLE()
	ON_WM_SHOWWINDOW()
	ON_WM_WINDOWPOSCHANGED()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CXTPBrowseEdit::OnEnable(BOOL bEnable)
{
	CXTPEdit::OnEnable(bEnable);

	if (::IsWindow(m_btnBrowse.GetSafeHwnd()))
		m_btnBrowse.EnableWindow(bEnable);

	Invalidate();
}

void CXTPBrowseEdit::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CXTPEdit::OnShowWindow(bShow, nStatus);

	if (::IsWindow(m_btnBrowse.GetSafeHwnd()))
		m_btnBrowse.ShowWindow(bShow ? SW_SHOW : SW_HIDE);
}

bool CXTPBrowseEdit::Initialize(CWnd* pParentWnd, DWORD dwBStyle, UINT nMenu/*= 0*/, int nSubMenuIndx/*= 0*/)
{
	if (!CXTPEdit::Initialize(pParentWnd))
		return false;

	m_dwBStyle = dwBStyle;

	// only create a browse button if one was specified.
	if (m_dwBStyle & (BES_XTP_CHOOSEDIR | BES_XTP_CHOOSEFILE | BES_XTP_POPUPMENU | BES_XTP_BROWSE))
	{
		// set browse edit style and menu indexes.
		m_nMenu = nMenu;
		m_nSubMenuIndx = nSubMenuIndx;

		// Create the browse button associated with the browse edit control.
		if (!m_btnBrowse.Create(this, m_dwBStyle, XTP_IDC_BTN_BROWSE,
			m_nMenu, m_nSubMenuIndx))
		{
			TRACE1("Error creating browse button for ID %d.\n", GetDlgCtrlID());
			return false;
		}

		CXTPWindowRect rcWindow(this);

		pParentWnd = GetParent();
		ASSERT_VALID(pParentWnd); // must be valid.
		pParentWnd->ScreenToClient(&rcWindow);

		if (m_nGap > 0)
		{
			rcWindow.right -= CX_BUTTON + m_nGap;

			// set the window position to accommodate for the browse button.
			::SetWindowPos(m_hWnd, NULL, rcWindow.left, rcWindow.top,
				rcWindow.Width(), rcWindow.Height(), SWP_FRAMECHANGED | SWP_NOZORDER);
		}
		else
		{
			rcWindow.left = rcWindow.right - CX_BUTTON;

			// set the window position to accommodate for the browse button.
			::SetWindowPos(m_btnBrowse, NULL, rcWindow.left, rcWindow.top,
				rcWindow.Width(), rcWindow.Height(), SWP_FRAMECHANGED | SWP_NOZORDER);

			m_btnBrowse.SetBuddy(this);
		}

		// position browse button.
		PositionBrowseButton();
	}

	return true;
}

void CXTPBrowseEdit::PositionBrowseButton()
{

	if (::IsWindow(m_btnBrowse.GetSafeHwnd()))
	{
		CXTPWindowRect rcWindow(this);

		// construct the size of the browse button.
		CRect rcButton;
		rcButton.left = rcWindow.right + m_nGap;
		rcButton.right = rcButton.left + CX_BUTTON;
		rcButton.top = rcWindow.top;
		rcButton.bottom = rcWindow.top + rcWindow.Height();

		CWnd* pParentWnd = GetParent();
		ASSERT_VALID(pParentWnd); // must be valid.
		pParentWnd->ScreenToClient(&rcButton);

		// set the window position and make sure tab order is correct.
		::SetWindowPos(m_btnBrowse.m_hWnd, m_hWnd,
			rcButton.left, rcButton.top, rcButton.Width(), rcButton.Height(), SWP_FRAMECHANGED);

		// construct the area to be updated on the parent.
		CRect rcRedraw = rcWindow;
		rcRedraw.right = rcButton.right;

		// refresh parent window.
		pParentWnd->InvalidateRect(&rcRedraw);
		m_btnBrowse.Invalidate(FALSE);
	}
}

void CXTPBrowseEdit::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos)
{
	CXTPEdit::OnWindowPosChanged(lpwndpos);

	// position browse button.
	PositionBrowseButton();
}

void CXTPBrowseEdit::ChooseDirectory()
{
	m_bBrowsing = true;

	// Get the current (if any) text that is displayed in
	// the edit control, this will be the initial path for the
	// browse dialog to start from.
	CString strPath;
	GetWindowText(strPath);

	if (!DIRECTORYEXISTS_S(strPath) && !m_strInitialDir.IsEmpty())
		strPath = m_strInitialDir;

	// Instantiate a browse for folder dialog object
	CXTPBrowseDialog browseDlg;
	browseDlg.SetOwner(GetParent()->GetSafeHwnd());
	browseDlg.SetTitle((TCHAR*)(LPCTSTR)m_strTitle);
	browseDlg.SetSelPath((TCHAR*)(LPCTSTR)strPath);

	if (browseDlg.DoModal() == IDOK)
	{
		SetWindowText(browseDlg.GetSelPath());
	}

	m_bBrowsing = false;
}

void CXTPBrowseEdit::ChooseFile()
{
	m_bBrowsing = true;

	if (m_pFileDialog)
	{
		if (m_pFileDialog->DoModal() == IDOK)
			SetWindowText(m_pFileDialog->GetPathName());
	}
	else
	{
		CFileDialog dlg(m_bOpenFileDialog, m_strDefExt, m_strFileName,
			m_dwFlags, m_strFilter, GetParent());

		if (!m_strInitialDir.IsEmpty())
		{
			dlg.m_ofn.lpstrInitialDir = (LPCTSTR)m_strInitialDir;
		}

		// Check to see if this is Windows 2000 or later, if so use the
		// Windows 2000 version of OPENFILENAME.

		if (sizeof(OPENFILENAME) < 88) if (XTPSystemVersion()->IsWin2KOrGreater() && dlg.m_ofn.lStructSize < 88)
		{
			// Windows 2000 version of OPENFILENAME has three extra members,
			// this was copied from newer version of commdlg.h...

			struct OPENFILENAMEEX
			{
				void*  pvReserved; // 4 bytes
				DWORD  dwReserved; // 4 bytes
				DWORD  FlagsEx;    // 4 bytes
			};

			dlg.m_ofn.lStructSize += sizeof(OPENFILENAMEEX); // should equal an additional 12 bytes;
		}

		if (dlg.DoModal() == IDOK)
		{
			SetWindowText(dlg.GetPathName());
		}
	}

	m_bBrowsing = false;
}

void CXTPBrowseEdit::PopupMenu()
{
	if (m_bBrowsing)
		return;

	m_bBrowsing = true;

	ASSERT(m_nMenu != 0);
	if (::IsWindow(m_btnBrowse.GetSafeHwnd()))
		m_btnBrowse.SetState(TRUE);

	CRect rect;
	m_btnBrowse.GetWindowRect(&rect);

	// loading a user resource.
	CMenu menu;
	CXTPResourceManager::AssertValid(XTPResourceManager()->LoadMenu(&menu, m_nMenu));

	CMenu* pPopup = menu.GetSubMenu(m_nSubMenuIndx);
	ASSERT(pPopup != NULL);

	CWnd* pWndPopupOwner = this;
	while (pWndPopupOwner->GetStyle() & WS_CHILD)
	{
		pWndPopupOwner = pWndPopupOwner->GetParent();
	}

	XTPContextMenu(pPopup, TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_VERTICAL,
		rect.right, rect.top, pWndPopupOwner, 0);

	// Return the button state to normal.
	if (::IsWindow(m_btnBrowse.GetSafeHwnd()))
		m_btnBrowse.SetState(FALSE);

	m_bBrowsing = false;
}

void CXTPBrowseEdit::OnBrowse()
{
	// If this is a browse directory control.
	switch (m_dwBStyle & (BES_XTP_CHOOSEDIR | BES_XTP_CHOOSEFILE | BES_XTP_POPUPMENU | BES_XTP_BROWSE))
	{
	case BES_XTP_CHOOSEDIR:
		ChooseDirectory();
		break;
	case BES_XTP_CHOOSEFILE:
		ChooseFile();
		break;
	case BES_XTP_POPUPMENU:
		PopupMenu();
		break;
	case BES_XTP_BROWSE:
		GetParent()->SendMessage( WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(), LBN_XTP_ONBROWSE), (LPARAM)m_hWnd );
		break;
	}

	if (::GetFocus() != m_hWnd)
	{
		SetSel(0, -1);
		SetFocus();
	}
}

/////////////////////////////////////////////////////////////////////////////
// CXTPItemEdit class
/////////////////////////////////////////////////////////////////////////////

CXTPItemEdit::CXTPItemEdit()
{
	// Initialize defaults
	m_bClosePosted = false;
	m_bModified = false;
	m_bEscapeKey = false;
	m_bAutoDelete = false;
	m_dwBStyle = 0;
}

BOOL CXTPItemEdit::Create(CWnd* pParent, const CRect& rect, CString& strWindowText, DWORD dwBStyle/*= BES_XTP_CHOOSEDIR*/, bool bAutoDelete/*= true*/)
{
	m_bAutoDelete = bAutoDelete;
	m_dwBStyle = dwBStyle;
	m_strWindowText = strWindowText;

	// Create the edit control.
	if (!CXTPBrowseEdit::CreateEx(WS_EX_CLIENTEDGE, _T("EDIT"), 0, WS_BORDER | WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL,
		rect, pParent, XTP_IDC_LBOX_EDIT))
		return FALSE;

	// Initialize the edit control.
	if (!Initialize(pParent, dwBStyle))
		return FALSE;

	// Set the window text and select it.
	SetWindowText(m_strWindowText);
	SetSel(0, -1);
	SetFocus();

	return TRUE;
}

CXTPItemEdit::~CXTPItemEdit()
{
}

IMPLEMENT_DYNAMIC(CXTPItemEdit, CXTPBrowseEdit)

BEGIN_MESSAGE_MAP(CXTPItemEdit, CXTPBrowseEdit)
	//{{AFX_MSG_MAP(CXTPItemEdit)
	ON_WM_KILLFOCUS()
	ON_WM_CHAR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CXTPItemEdit::OnKillFocus(CWnd* pNewWnd)
{
	CXTPBrowseEdit::OnKillFocus(pNewWnd);

	if (!::IsWindow(m_btnBrowse.GetSafeHwnd()) || pNewWnd != &m_btnBrowse)
	{
		ASSERT_VALID(this);
		EndLabelEdit();
	}
}

void CXTPItemEdit::PostNcDestroy()
{
	if (m_bAutoDelete)
	{
		delete this;
	}
}

BOOL CXTPItemEdit::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN || pMsg->message == WM_KEYUP)
	{
		if ((pMsg->wParam == VK_RETURN) ||
			(pMsg->wParam == VK_ESCAPE))
		{
			if (pMsg->wParam == VK_ESCAPE)
			{
				m_bEscapeKey = true;
			}

			EndLabelEdit();
			return TRUE;
		}
	}

	return CXTPBrowseEdit::PreTranslateMessage(pMsg);
}

void CXTPItemEdit::EndLabelEdit()
{
	if (m_bClosePosted) return;

	if (::IsWindow(m_btnBrowse.GetSafeHwnd()))
		m_btnBrowse.PostMessage(WM_CLOSE);

	if (::IsWindow(GetSafeHwnd()))
		PostMessage(WM_CLOSE);

	m_bClosePosted = true;

	if (m_bModified == false)
	{
		CWnd* pOwner = GetOwner();
		ASSERT_VALID(pOwner);

		if (m_bEscapeKey == false)
		{
			m_bModified = true;

			// get the window text and pass it on to the
			// notification window...
			GetWindowText(m_strWindowText);

			// Notify of label edit.
			pOwner->SendMessage(WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(),
				BEN_XTP_LABELEDITEND), (LPARAM)GetSafeHwnd());
		}
		else
		{
			// Notify of label edit.
			pOwner->SendMessage(WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(),
				BEN_XTP_LABELEDITCANCEL), (LPARAM)GetSafeHwnd());
		}
	}
}

void CXTPItemEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if ((nChar == VK_RETURN) ||
		(nChar == VK_ESCAPE))
	{
		if (nChar == VK_ESCAPE)
			m_bEscapeKey = TRUE;

		EndLabelEdit();
		return;
	}

	CXTPBrowseEdit::OnChar(nChar, nRepCnt, nFlags);
}
