// XTPRibbonCustomizeQuickAccessPage.cpp: implementation of the CXTPRibbonCustomizeQuickAccessPage class.
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


#include "StdAfx.h"

#include <Common/XTPResourceManager.h>
#include <Common/XTPSystemHelpers.h>
#include <Common/XTPHookManager.h>
#include <Common/XTPColorManager.h>
#include <Common/ScrollBar/XTPScrollInfo.h>

#include <Controls/Resize/XTPResizeRect.h>
#include <Controls/Resize/XTPResizePoint.h>
#include <Controls/Resize/XTPResize.h>
#include <Controls/Dialog/XTPPropertySheet.h>
#include <Controls/Dialog/XTPPropertyPage.h>

#include <Controls/Static/XTPMarkupStatic.h>

#include <CommandBars/Resource.h>
#include <CommandBars/XTPCommandBarsDefines.h>
#include <CommandBars/XTPCommandBars.h>
#include <CommandBars/XTPCommandBar.h>
#include <CommandBars/XTPToolBar.h>
#include <CommandBars/XTPMenuBar.h>
#include <CommandBars/XTPPopupBar.h>
#include <CommandBars/XTPControls.h>
#include <CommandBars/XTPControl.h>
#include <CommandBars/XTPControlButton.h>
#include <CommandBars/XTPControlPopup.h>
#include <CommandBars/XTPPaintManager.h>
#include <CommandBars/Customize/XTPCustomizePageCommands.h>
#include <CommandBars/Customize/XTPCustomizeSheet.h>

#include <Ribbon/Resource.h>
#include <Ribbon/XTPRibbonQuickAccessControls.h>
#include <Ribbon/Customize/XTPRibbonCustomizeQuickAccessPage.h>
#include <Ribbon/XTPRibbonPopups.h>
#include <Ribbon/XTPRibbonBar.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTPRibbonCustomizeQuickAccessPage property page

CXTPRibbonCustomizeQuickAccessPage::CXTPRibbonCustomizeQuickAccessPage(CXTPCustomizeSheet* pSheet, UINT nIDTemplate)
	: CXTPPropertyPage(nIDTemplate == (UINT)-1 ? XTP_IDD_RIBBONCUSTOMIZE_QUICKACCESS : nIDTemplate)
	// Conditional compilation padding issue workaround.
#ifdef _XTP_INCLUDE_MARKUP
	, m_wndTitle(*(new CXTPMarkupStatic()))
#else
	, m_wndTitle(*(new CStatic()))
#endif
{
	m_pSheet = pSheet;
	m_pCommandBars = pSheet->GetCommandBars();

	m_bShowQuickAccessBelow = FALSE;
}

CXTPRibbonCustomizeQuickAccessPage::CXTPRibbonCustomizeQuickAccessPage(CXTPCommandBars* pCommandBars)
	: CXTPPropertyPage(XTP_IDD_RIBBONCUSTOMIZE_QUICKACCESS_LARGE)
	// Conditional compilation padding issue workaround.
#ifdef _XTP_INCLUDE_MARKUP
	, m_wndTitle(*(new CXTPMarkupStatic()))
#else
	, m_wndTitle(*(new CStatic()))
#endif
{
	m_pSheet = NULL;
	m_pCommandBars = pCommandBars;

	m_bShowQuickAccessBelow = FALSE;
}

CXTPRibbonCustomizeQuickAccessPage::~CXTPRibbonCustomizeQuickAccessPage()
{
	for (int i = 0; i < m_arrCategories.GetSize(); i++)
	{
		delete m_arrCategories[i];
	}

	// Conditional compilation padding issue workaround.
	delete &m_wndTitle;
}

void CXTPRibbonCustomizeQuickAccessPage::DoDataExchange(CDataExchange* pDX)
{
	CXTPPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CXTPRibbonCustomizeQuickAccessPage)
	DDX_Control(pDX, XTP_IDC_RIBBONCOMBO_CATEGORIES, m_lstCategories);
	DDX_Control(pDX, XTP_IDC_RIBBONLIST_QUICKACCESS, m_lstQuickAccess);
	DDX_Control(pDX, XTP_IDC_RIBBONLIST_COMMANDS, m_lstCommands);
	DDX_Check(pDX, XTP_IDC_RIBBONCHECK_SHOWQUICKACCESSBELOW, m_bShowQuickAccessBelow);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CXTPRibbonCustomizeQuickAccessPage, CXTPPropertyPage)
	//{{AFX_MSG_MAP(CXTPRibbonCustomizeQuickAccessPage)
	ON_BN_CLICKED(XTP_IDC_RIBBONCHECK_SHOWQUICKACCESSBELOW, OnCheckShowQuickAccessBelow)
	ON_LBN_SELCHANGE(XTP_IDC_RIBBONCOMBO_CATEGORIES, OnCategoriesSelectionChanged)
	ON_LBN_DBLCLK(XTP_IDC_RIBBONLIST_QUICKACCESS, OnDblclkListQuickAccess)
	ON_LBN_DBLCLK(XTP_IDC_RIBBONLIST_COMMANDS, OnDblclkListCommands)
	ON_BN_CLICKED(XTP_IDC_RIBBONBUTTON_ADD, OnButtonAdd)
	ON_BN_CLICKED(XTP_IDC_RIBBONBUTTON_REMOVE, OnButtonRemove)
	ON_BN_CLICKED(XTP_IDC_RIBBONBUTTON_RESET, OnButtonReset)
	ON_LBN_SELCHANGE(XTP_IDC_RIBBONLIST_COMMANDS, OnCommandsSelectionChanged)
	ON_LBN_SELCHANGE(XTP_IDC_RIBBONLIST_QUICKACCESS, OnQuickAccessSelectionChanged)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CXTPRibbonBar* CXTPRibbonCustomizeQuickAccessPage::GetRibbonBar()
{
	CXTPRibbonBar* pRibbonBar = DYNAMIC_DOWNCAST(CXTPRibbonBar, m_pCommandBars->GetMenuBar());
	ASSERT(pRibbonBar);

	return pRibbonBar;
}

/////////////////////////////////////////////////////////////////////////////
// CXTPRibbonCustomizeQuickAccessPage message handlers

BOOL CXTPRibbonCustomizeQuickAccessPage::OnInitDialog()
{
	CXTPPropertyPage::OnInitDialog();

	ModifyStyle(0, WS_CLIPCHILDREN);

	for (int i = 0; i < m_arrCategories.GetSize(); i++)
	{
		int nIndex = m_lstCategories.AddString(m_arrCategories[i]->strCategory);
		m_lstCategories.SetItemData(nIndex, i);

		if (i == 0) m_lstCategories.SetCurSel(0);
	}

	CSize sz = m_pCommandBars->GetPaintManager()->DrawListBoxControl(NULL, NULL, CRect(0, 0, 0, 0), FALSE, FALSE, m_pCommandBars);
	m_lstCommands.SetItemHeight(-1, sz.cy);
	m_lstQuickAccess.SetItemHeight(-1, sz.cy);

	m_lstCommands.m_pCommandBars = m_pCommandBars;
	m_lstQuickAccess.m_pCommandBars = m_pCommandBars;

	OnCategoriesSelectionChanged();
	RefreshQuickAccessList();

	m_bShowQuickAccessBelow = GetRibbonBar()->IsQuickAccessBelowRibbon();
	UpdateData(FALSE);

	if (m_pCommandBars->IsLayoutRTL())
	{
		m_lstCommands.ModifyStyleEx(0, WS_EX_LAYOUTRTL);
		m_lstQuickAccess.ModifyStyleEx(0, WS_EX_LAYOUTRTL);
		m_lstCategories.ModifyStyleEx(0, WS_EX_RIGHT | WS_EX_LEFTSCROLLBAR | WS_EX_RTLREADING);
	}

	if (GetDlgItem(XTP_IDC_RIBBONPAGE_TITLE) != NULL)
	{
		m_wndTitle.SubclassDlgItem(XTP_IDC_RIBBONPAGE_TITLE, this);

		m_wndTitle.SetMarkupTextEx(_T("<StackPanel Orientation='Horizontal'><Image Source='res://XTP_IDB_RIBBON_ICONQUICKACCESS'/>")
			_T("<TextBlock Padding='3, 0, 0, 0' FontSize='13' VerticalAlignment='Center'>%s</TextBlock></StackPanel>"));
	}

	SetResize(XTP_IDC_RIBBONCOMBO_CATEGORIES, XTP_ANCHOR_TOPLEFT, XTP_ANCHOR_TOPCENTER);
	SetResize(XTP_IDC_RIBBONLIST_COMMANDS, XTP_ANCHOR_TOPLEFT, XTP_ANCHOR_BOTTOMCENTER);
	SetResize(XTP_IDC_RIBBONLIST_QUICKACCESS, XTP_ANCHOR_TOPCENTER, XTP_ANCHOR_BOTTOMRIGHT);

	SetResize(XTP_IDC_RIBBONCHECK_SHOWQUICKACCESSBELOW, XTP_ANCHOR_BOTTOMLEFT, XTP_ANCHOR_BOTTOMLEFT);
	SetResize(XTP_IDC_RIBBONBUTTON_RESET, XTP_ANCHOR_BOTTOMCENTER, XTP_ANCHOR_BOTTOMCENTER);

	SetResize(XTP_IDC_RIBBONBUTTON_ADD,  CXTPResizePoint(.5, 0.5), CXTPResizePoint(.5, 0.5));
	SetResize(XTP_IDC_RIBBONBUTTON_REMOVE, CXTPResizePoint(.5, 0.5), CXTPResizePoint(.5, 0.5));

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CXTPRibbonCustomizeQuickAccessPage::OnCheckShowQuickAccessBelow()
{
	UpdateData();

	GetRibbonBar()->ShowQuickAccessBelowRibbon(m_bShowQuickAccessBelow);
}


void CXTPRibbonCustomizeQuickAccessPage::RefreshQuickAccessList()
{
	m_lstQuickAccess.SetRedraw(FALSE);
	int nTopIndex = m_lstQuickAccess.GetTopIndex();
	m_lstQuickAccess.ResetContent();

	CXTPControls* pControls = GetRibbonBar()->GetQuickAccessControls();

	for (int i = 0; i < pControls->GetCount(); i++)
	{
		CXTPControl* pControl = pControls->GetAt(i);
		if (!pControl->IsVisible())
			continue;
		m_lstQuickAccess.SendMessage(LB_INSERTSTRING, m_lstQuickAccess.GetCount(), (LPARAM)pControl);
	}
	m_lstQuickAccess.SetTopIndex(nTopIndex);
	m_lstQuickAccess.SetRedraw(TRUE);

	OnQuickAccessSelectionChanged();
	OnCommandsSelectionChanged();
}

void CXTPRibbonCustomizeQuickAccessPage::OnCategoriesSelectionChanged()
{

	m_lstCommands.ResetContent();

	int nIndex = m_lstCategories.GetCurSel();

	if (nIndex == LB_ERR)
		return;

	XTP_COMMANDBARS_CATEGORYINFO* pInfo = GetCategoryInfo((int)m_lstCategories.GetItemData(nIndex));

	if (pInfo == NULL)
		return;

	for (int i = 0; i < pInfo->pControls->GetCount(); i++)
	{
		CXTPControl* pControl = pInfo->pControls->GetAt(i);
		m_lstCommands.SendMessage(LB_INSERTSTRING, m_lstCommands.GetCount(), (LPARAM)pControl);
	}
	OnCommandsSelectionChanged();
}


XTP_COMMANDBARS_CATEGORYINFO* CXTPRibbonCustomizeQuickAccessPage::FindCategory(LPCTSTR strCategory) const
{
	for (int i = 0; i < m_arrCategories.GetSize(); i++)
	{
		if (m_arrCategories[i]->strCategory.Compare(strCategory) == 0)
			return m_arrCategories[i];
	}
	return NULL;
}

XTP_COMMANDBARS_CATEGORYINFO* CXTPRibbonCustomizeQuickAccessPage::GetCategoryInfo(int nIndex)
{
	if (nIndex >= 0 && nIndex < m_arrCategories.GetSize())
		return m_arrCategories[nIndex];
	return NULL;
}

BOOL CXTPRibbonCustomizeQuickAccessPage::AddCategory(LPCTSTR strCategory, CMenu* pMenu, BOOL bListSubItems)
{
	CXTPControls* pCategoryControls = InsertCategory(strCategory);

	int nCount = pMenu->GetMenuItemCount();

	for (int i = 0; i < nCount; i++)
	{
		if (pMenu->GetMenuItemID(i) > 0)
		{
			CXTPControlPopup* pControl = DYNAMIC_DOWNCAST(CXTPControlPopup,
				pCategoryControls->AddMenuItem(pMenu, i));

			if (pControl && bListSubItems)
			{
				CXTPControls* pControls = pControl->GetCommandBar()->GetControls();
				for (int j = 0; j < pControls->GetCount(); j++)
				{
					pCategoryControls->AddClone(pControls->GetAt(j));
				}
			}
		}
	}

	return TRUE;
}


BOOL CXTPRibbonCustomizeQuickAccessPage::AddCategories(UINT nIDResource, BOOL bListSubItems)
{
	CMenu menu;
	if (!XTPResourceManager()->LoadMenu(&menu, nIDResource))
		return FALSE;

	int nCount = menu.GetMenuItemCount();

	for (int i = 0; i < nCount; i++)
	{
		CString strCategory;
		if (menu.GetMenuString(i, strCategory, MF_BYPOSITION) > 0)
		{
			CMenu* pMenu = menu.GetSubMenu(i);
			if (pMenu)
			{
				CXTPPaintManager::StripMnemonics(strCategory);

				if (!AddCategory(strCategory, pMenu, bListSubItems))
					return FALSE;
			}
		}

	}

	return TRUE;
}

BOOL CXTPRibbonCustomizeQuickAccessPage::AddCategories(CXTPControls* pControls)
{
	for (int i = 0; i < pControls->GetCount(); i++)
	{
		CXTPControl* pControl = pControls->GetAt(i);
		CString strCategory = pControl->GetCategory();

		if (!strCategory.IsEmpty())
		{
			CXTPControls* pCategoryControls = InsertCategory(strCategory);
			pCategoryControls->AddClone(pControl);
		}
	}
	return TRUE;
}


CXTPControls* CXTPRibbonCustomizeQuickAccessPage::InsertCategory(LPCTSTR strCategory, int nIndex)
{
	XTP_COMMANDBARS_CATEGORYINFO* pInfo = FindCategory(strCategory);
	if (!pInfo)
	{
		pInfo = new XTP_COMMANDBARS_CATEGORYINFO(strCategory, m_pCommandBars);
		m_arrCategories.InsertAt(nIndex == -1 ? m_arrCategories.GetSize() : nIndex, pInfo);
	}
	return pInfo->pControls;
}

CXTPControls* CXTPRibbonCustomizeQuickAccessPage::GetControls(LPCTSTR strCategory)
{
	XTP_COMMANDBARS_CATEGORYINFO* pInfo = FindCategory(strCategory);
	return pInfo ? pInfo->pControls : NULL;
}

void CXTPRibbonCustomizeQuickAccessPage::OnDblclkListQuickAccess()
{
	OnButtonRemove();

}

void CXTPRibbonCustomizeQuickAccessPage::OnDblclkListCommands()
{
	OnButtonAdd();

}

void CXTPRibbonCustomizeQuickAccessPage::OnButtonAdd()
{
	if (!GetDlgItem(XTP_IDC_RIBBONBUTTON_ADD)->IsWindowEnabled())
		return;

	if (m_lstCommands.GetCurSel() == LB_ERR)
		return;

	CXTPControl* pControl = (CXTPControl*)m_lstCommands.GetItemDataPtr(m_lstCommands.GetCurSel());
	if (!pControl)
		return;

	GetRibbonBar()->GetQuickAccessControls()->AddClone(pControl);
	GetRibbonBar()->OnRecalcLayout();

	RefreshQuickAccessList();
	m_lstQuickAccess.SetCurSel(m_lstQuickAccess.GetCount() - 1);

	OnQuickAccessSelectionChanged();
}

void CXTPRibbonCustomizeQuickAccessPage::OnButtonRemove()
{
	if (m_lstQuickAccess.GetCurSel() == LB_ERR)
		return;

	CXTPControl* pControl = (CXTPControl*)m_lstQuickAccess.GetItemDataPtr(m_lstQuickAccess.GetCurSel());
	if (!pControl)
		return;

	GetRibbonBar()->GetQuickAccessControls()->Remove(pControl);
	GetRibbonBar()->OnRecalcLayout();

	RefreshQuickAccessList();
}

void CXTPRibbonCustomizeQuickAccessPage::OnButtonReset()
{
	CString strPrompt;
	CXTPResourceManager::AssertValid(XTPResourceManager()->LoadString(&strPrompt, XTP_IDS_RIBBONCUSTOMIZE_CONFIRMRESET));

	if (m_pSheet)
	{
		if (m_pSheet->MessageBox(strPrompt, MB_ICONWARNING | MB_YESNO) != IDYES)
			return;
	}
	else
	{
		if (AfxMessageBox(strPrompt, MB_ICONWARNING | MB_YESNO) != IDYES)
			return;
	}

	GetRibbonBar()->GetQuickAccessControls()->Reset();
	RefreshQuickAccessList();
}

void CXTPRibbonCustomizeQuickAccessPage::OnCommandsSelectionChanged()
{
	BOOL bEnabled = m_lstCommands.GetCurSel() != LB_ERR;

	if (bEnabled)
	{
		CXTPControl* pControl = (CXTPControl*)m_lstCommands.GetItemDataPtr(m_lstCommands.GetCurSel());
		bEnabled = GetRibbonBar()->IsAllowQuickAccessControl(pControl);
	}

	GetDlgItem(XTP_IDC_RIBBONBUTTON_ADD)->EnableWindow(bEnabled);
}

void CXTPRibbonCustomizeQuickAccessPage::OnQuickAccessSelectionChanged()
{
	GetDlgItem(XTP_IDC_RIBBONBUTTON_REMOVE)->EnableWindow(m_lstQuickAccess.GetCurSel() != LB_ERR);

}

void CXTPRibbonCustomizeQuickAccessPage::UpdatePage(void)
{
	if (NULL != GetSafeHwnd())
	{
		RefreshQuickAccessList();
	}
}