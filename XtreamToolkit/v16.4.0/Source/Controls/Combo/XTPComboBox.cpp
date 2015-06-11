// XTPComboBox.cpp : implementation of the CXTPComboBox class.
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
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPResourceImage.h"
#include "Common/XTPWinThemeWrapper.h"

#include "../Util/XTPControlTheme.h"
#include "XTPComboBox.h"

#include "XTPComboBoxTheme.h"
#include "Themes/XTPComboBoxThemeResource.h"
#include "Themes/XTPComboBoxThemeDefault.h"
#include "Themes/XTPComboBoxThemeOffice2000.h"
#include "Themes/XTPComboBoxThemeOffice2003.h"
#include "Themes/XTPComboBoxThemeOfficeXPFlat.h"
#include "Themes/XTPComboBoxThemeVisualStudio.h"
#include "Themes/XTPComboBoxThemeVisualStudio2010.h"
#include "Themes/XTPComboBoxThemeOffice2013.h"
#include "Themes/XTPComboBoxThemeVisualStudio2012.h"

#include "XTPComboBoxEditCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CXTPComboBox::CXTPComboBox()
{
	m_bHighlighted = FALSE;
	m_bFocused = FALSE;

	m_bFlatStyle = FALSE;
	m_bUseVisualStyle = TRUE;
	m_pTheme = NULL;

	m_bPreSubclassInit = TRUE;

	m_bAutoComp = FALSE;
	m_bDisableAC = FALSE;

	m_pTheme = NULL;

	VERIFY(SetTheme(xtpControlThemeDefault));

	m_pWndEdit = new CXTPComboBoxEditCtrl();
}

CXTPComboBox::~CXTPComboBox()
{
	CMDTARGET_RELEASE(m_pTheme);

	SAFE_DELETE(m_pWndEdit);
}

IMPLEMENT_DYNAMIC(CXTPComboBox, CComboBox)

BEGIN_MESSAGE_MAP(CXTPComboBox, CComboBox)
	ON_WM_CREATE()
	ON_WM_PAINT()

	ON_WM_MOUSEMOVE()
	ON_WM_KILLFOCUS()
	ON_WM_SETFOCUS()
	ON_MESSAGE_VOID(WM_MOUSELEAVE, OnMouseLeave)

	ON_WM_SYSCOLORCHANGE()

	ON_CONTROL_REFLECT_EX(CBN_EDITUPDATE, OnEditUpdate)
	ON_CONTROL_REFLECT_EX(CBN_CLOSEUP, OnEndSel)

	//atsvetkove
	ON_WM_CTLCOLOR()
	ON_WM_CTLCOLOR_REFLECT()
END_MESSAGE_MAP()

void CXTPComboBox::RedrawComboBox(BOOL bUpdateWindow)
{
	if (m_hWnd) Invalidate(FALSE);
	if (m_hWnd && bUpdateWindow) UpdateWindow();
}

void CXTPComboBox::RefreshMetrics()
{
	if (m_pTheme)
		m_pTheme->RefreshMetrics(this);

	if (::IsWindow(m_hWnd))
		RedrawComboBox();
}

void CXTPComboBox::Init()
{
	RefreshMetrics();

	Invalidate(FALSE);

	HWND hWndEdit = 0;
	GetDlgItem(1001, &hWndEdit);

	if (hWndEdit && m_pWndEdit)
	{
		m_pWndEdit->SubclassWindow(hWndEdit);
	}
}

void CXTPComboBox::PreSubclassWindow()
{
	CComboBox::PreSubclassWindow();

	if (m_bPreSubclassInit)
	{
		// Initialize the control.
		Init();
	}
}

int CXTPComboBox::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CComboBox::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Initialize the control.
	Init();

	return 0;
}

BOOL CXTPComboBox::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CComboBox::PreCreateWindow(cs))
		return FALSE;

	// When creating controls dynamically Init() must
	// be called from OnCreate() and not from
	// PreSubclassWindow().

	m_bPreSubclassInit = FALSE;

	return TRUE;
}

BOOL CXTPComboBox::SetTheme(XTPControlTheme nTheme)
{
	//nTheme = xtpControlThemeVisualStudio2010;

	CMDTARGET_RELEASE(m_pTheme);

	switch (nTheme)
	{
		case xtpControlThemeDefault:
			m_pTheme = new CXTPComboBoxThemeDefault();
		break;

		case xtpControlThemeResource:
			m_pTheme = new CXTPComboBoxThemeResource();
		break;

		case xtpControlThemeOffice2000:
			m_pTheme = new CXTPComboBoxThemeOffice2000();
		break;

		case xtpControlThemeOfficeXP:
		case xtpControlThemeFlat:
		case xtpControlThemeUltraFlat:
			m_pTheme = new CXTPComboBoxThemeOfficeXPFlat(nTheme);
		break;

		case xtpControlThemeOffice2013:
			m_pTheme = new CXTPComboBoxThemeOffice2013();
		break;

		case xtpControlThemeOffice2003:
			m_pTheme = new CXTPComboBoxThemeOffice2003();
		break;

		case xtpControlThemeVisualStudio2005:
		case xtpControlThemeVisualStudio2008:
			m_pTheme = new CXTPComboBoxThemeVisualStudio();
		break;

		case xtpControlThemeVisualStudio2010:
			m_pTheme = new CXTPComboBoxThemeVisualStudio2010();
			break;

		case xtpControlThemeVisualStudio2012Light:
		case xtpControlThemeVisualStudio2012Dark:
			m_pTheme = new CXTPComboBoxThemeVisualStudio2012(nTheme == xtpControlThemeVisualStudio2012Light);
			break;
		default:
			m_pTheme = new CXTPComboBoxTheme();
	}

	m_bUseVisualStyle = (nTheme == xtpControlThemeDefault);

	RefreshMetrics();

	if (m_hWnd)
		Invalidate(FALSE);

	return (m_pTheme != NULL);
}

void CXTPComboBox::SetTheme(CXTPComboBoxTheme *pPaintManager)
{
	CMDTARGET_RELEASE(m_pTheme);
	m_pTheme = pPaintManager;

	m_bUseVisualStyle = (pPaintManager->GetThemeStyle() == xtpControlThemeDefault);

	if (m_pTheme)
	{
		m_pTheme->RefreshMetrics(this);
	}

	if (::IsWindow(m_hWnd))
	{
		Invalidate(FALSE);
	}
}

CXTPComboBoxTheme* CXTPComboBox::GetTheme()
{
	return m_pTheme;
}

const CXTPComboBoxTheme* CXTPComboBox::GetTheme() const
{
	return m_pTheme;
}

void CXTPComboBox::SetUseVisualStyle(BOOL bUseVisualStyle/* = TRUE*/)
{
	m_bUseVisualStyle = bUseVisualStyle;

	RefreshMetrics();
	if (m_hWnd) Invalidate(FALSE);
}

void CXTPComboBox::OnPaint()
{
	CPaintDC paintDC(this);
	CXTPClientRect rc(this);
	CXTPBufferDC dcMem(paintDC);

	HBRUSH hBrush = GetClientBrush(&dcMem);
	FillRect(dcMem, rc, hBrush);

	if ((GetStyle() & 3) == CBS_SIMPLE || m_bUseVisualStyle)
	{
		CComboBox::DefWindowProc(WM_PAINT, (WPARAM)dcMem.m_hDC, 0);
	}
	else
	{
		// Resource theme and GDI themes are using DefWindowProc before its own drawing
		DefWindowProc(WM_PAINT, (WPARAM)dcMem.GetSafeHdc(), 0);

		if (!m_pTheme->DrawComboBox(&dcMem, this))
		{
			CComboBox::DefWindowProc(WM_PAINT, (WPARAM)dcMem.m_hDC, 0);
		}
	}
}

HBRUSH CXTPComboBox::GetClientBrush(CDC* pDC)
{
	CWnd* pwndEdit = GetWindow(GW_CHILD);

	BOOL bDisabled = !IsWindowEnabled() || (pwndEdit && pwndEdit->GetStyle() & ES_READONLY);

	HBRUSH hBrush = (HBRUSH)::SendMessage(::GetParent(m_hWnd), bDisabled ? WM_CTLCOLORSTATIC : WM_CTLCOLOREDIT, (WPARAM)pDC->GetSafeHdc(), (LPARAM)m_hWnd);

	if (hBrush)
		return hBrush;

	return GetSysColorBrush(bDisabled ? COLOR_3DFACE : COLOR_WINDOW);
}

CRect CXTPComboBox::GetComboBoxRect() const
{
	CXTPClientRect rc(this);

	return rc;
}

void CXTPComboBox::OnMouseLeave()
{
	Default();

	if (m_pWndEdit && m_pWndEdit->GetSafeHwnd())
	{
		CPoint pt;
		GetCursorPos(&pt);

		CXTPWindowRect rcEdit(m_pWndEdit);

		if (rcEdit.PtInRect(pt))
		{
			TRACKMOUSEEVENT tme = {sizeof(TRACKMOUSEEVENT), TME_LEAVE, m_pWndEdit->GetSafeHwnd(), HOVER_DEFAULT};
			_TrackMouseEvent(&tme);

			SetHighlighted(TRUE);
			return;
		}
	}

	SetHighlighted(FALSE);
}

void CXTPComboBox::SetHighlighted(BOOL bHot)
{
	if (bHot != m_bHighlighted)
	{
		m_bHighlighted = bHot;
		RedrawFocusedFrame();

		if (bHot)
		{
			TRACKMOUSEEVENT tme = {sizeof(TRACKMOUSEEVENT), TME_LEAVE, m_hWnd, HOVER_DEFAULT};
			_TrackMouseEvent(&tme);
		}

		RefreshMetrics();
	}
}

void CXTPComboBox::OnMouseMove(UINT nFlags, CPoint point)
{
	CRect rc;
	GetClientRect(&rc);

	BOOL bHot = rc.PtInRect(point) && !m_bFocused;
	SetHighlighted(bHot);

	CComboBox::OnMouseMove(nFlags, point);
}

void CXTPComboBox::OnKillFocus(CWnd* pNewWnd)
{
	CComboBox::OnKillFocus(pNewWnd);

	if (pNewWnd != m_pWndEdit)
	{
		m_bFocused = FALSE;
		RefreshMetrics();
		RedrawFocusedFrame();
	}
}

void CXTPComboBox::OnSetFocus(CWnd* pOldWnd)
{
	CComboBox::OnSetFocus(pOldWnd);

	m_bFocused = TRUE;
	m_bHighlighted = FALSE;
	RedrawFocusedFrame();
}

BOOL CXTPComboBox::NeedRedraw()
{
	return m_bUseVisualStyle;
}

void CXTPComboBox::RedrawFocusedFrame()
{
	if (((GetStyle() & 3) != CBS_SIMPLE) && NeedRedraw())
	{
		Invalidate(FALSE);
	}
}

void CXTPComboBox::OnSysColorChange()
{
	CComboBox::OnSysColorChange();

	RefreshMetrics();
	Invalidate(FALSE);
}

BOOL CXTPComboBox::PreTranslateMessage(MSG* pMsg)
{
	// Make sure that the keystrokes continue to the edit control.
	if (pMsg->message == WM_KEYDOWN || pMsg->message == WM_KEYUP)
	{
		// if tab, return or escape key, just use default.
		switch (pMsg->wParam)
		{
		case VK_DELETE:
		case VK_BACK:
			{
				if (m_bAutoComp)
				{
					m_bDisableAC = (pMsg->message == WM_KEYDOWN);
				}
				break;
			}
		case VK_TAB:
		case VK_RETURN:
		case VK_ESCAPE:
			{
				return CComboBox::PreTranslateMessage(pMsg);
			}
		}

		// If the combo box has an edit control, don't allow
		// the framework to process accelerators, let the edit
		// control handle it instead.  GetEditSel() will return
		// CB_ERR if there is no edit control present...

		if (GetEditSel() != (DWORD)CB_ERR)
		{
			::TranslateMessage(pMsg);
			::DispatchMessage(pMsg);

			return TRUE;
		}
	}

	return CComboBox::PreTranslateMessage(pMsg);
}

BOOL CXTPComboBox::OnEditUpdate()
{
	// if we are not to auto update the text, get outta here
	if (m_bAutoComp)
	{
		if (m_bDisableAC)
		{
			Default();
		}
		else
		{
			// Get the text in the edit box
			CString strItemTyped;
			GetWindowText(strItemTyped);
			int nLength = strItemTyped.GetLength();

			if (nLength >= 1)
			{
				// Currently selected range
				DWORD dwCurSel = GetEditSel();
				int nStart = LOWORD(dwCurSel);
				int nEnd = HIWORD(dwCurSel);

				// Search for, and select in, and string in the combo box that is prefixed
				// by the text in the edit box
				if (SelectString(-1, strItemTyped) == CB_ERR)
				{
					SetWindowText(strItemTyped);     // No text selected, so restore what was there before
					if (dwCurSel != (DWORD)CB_ERR)
					{
						SetEditSel(nStart, nEnd);   //restore cursor postion
					}
				}

				// Set the text selection as the additional text that we have added
				if (nEnd < nLength && dwCurSel != (DWORD)CB_ERR)
				{
					SetEditSel(nStart, nEnd);
				}
				else
				{
					SetEditSel(nLength, -1);
				}
			}
		}
	}

	return FALSE;
}

BOOL CXTPComboBox::OnEndSel()
{
	if (NeedRedraw())
	{
		Invalidate();
	}

	return FALSE;
}


HBRUSH CXTPComboBox::CtlColor(CDC* pDC, UINT nCtlColor)
{
	if (!m_pTheme)
		return NULL;

	switch (nCtlColor)
	{
	//in mode DropList when combobox without focus, in this mode combobox does not have edit control
	case CTLCOLOR_EDIT:
		{
			pDC->SetTextColor(m_pTheme->GetTextColor());//does not work, if control is disabled
			pDC->SetBkColor(m_pTheme->GetBackColor());

			//brush used for fill self combobox in both modes DropList and DropDown
			return m_pTheme->GetBackgroundBrush();
		}
	//when combobox is disabled in DropList mode, in this mode combobox does not have edit control
	case CTLCOLOR_STATIC:
		{
			pDC->SetTextColor(m_pTheme->GetTextColor());//does not work, if control is disabled
			pDC->SetBkColor(m_pTheme->GetBackColor());

			return m_pTheme->GetBackgroundBrush();
		}
	default:
		{
			ASSERT(FALSE);
			return NULL;
		}
	}
}

HBRUSH CXTPComboBox::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (!m_pTheme)
		return CComboBox::OnCtlColor(pDC, pWnd, nCtlColor);

	switch (nCtlColor)
	{
	//when combobox DropDown mode, inner Edit is enabled
	case CTLCOLOR_EDIT:
		{
			pDC->SetTextColor(m_pTheme->GetTextColor());
			pDC->SetBkColor(m_pTheme->GetBackColor());

			return m_pTheme->GetBackgroundBrush();
		}
	//when combobox is expanded in both modes DropDown and DropList
	case CTLCOLOR_LISTBOX:
		{
			pDC->SetTextColor(m_pTheme->GetTextColor());
			pDC->SetBkColor(m_pTheme->GetBackColor());

			return m_pTheme->GetBackgroundBrush();
		}
	//when combobox is disabled in DropDown mode, inner Edit is disabled
	case CTLCOLOR_STATIC:
		{
			pDC->SetTextColor(m_pTheme->GetTextColor());//does not work, if control is disabled
			pDC->SetBkColor(m_pTheme->GetBackColor());

			return m_pTheme->GetBackgroundBrush();
		}
	default:
		{
			ASSERT(FALSE);
			return CComboBox::OnCtlColor(pDC, pWnd, nCtlColor);
		}
	}
}
