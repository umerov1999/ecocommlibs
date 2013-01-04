// XTPRibbonCustomizePage.cpp: implementation of the CXTPRibbonCustomizePage class.
//
// This file is a part of the XTREME RIBBON MFC class library.
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
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPHookManager.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPImageManager.h"
#include "Common/XTPPropExchange.h"

#include "TabManager/XTPTabManager.h"

#include "Controls/Resize/XTPResizeRect.h"
#include "Controls/Resize/XTPResizePoint.h"
#include "Controls/Resize/XTPResize.h"
#include "Controls/Dialog/XTPPropertySheet.h"
#include "Controls/Dialog/XTPPropertyPage.h"
#include "Controls/Static/XTPMarkupStatic.h"
#include "Controls/Util/XTPControlTheme.h"
#include "Controls/Button/XTPButton.h"

#include "Controls/CoreTree/XTPCoreTreeControl.h"
#include "Controls/CoreTree/XTPCoreTreeItem.h"
#include "Controls/CoreTree/XTPCoreTreePaintManager.h"

#include "CommandBars/Resource.h"
#include "CommandBars/XTPCommandBarsDefines.h"
#include "CommandBars/XTPCommandBars.h"
#include "CommandBars/XTPCommandBar.h"
#include "CommandBars/XTPToolBar.h"
#include "CommandBars/XTPMenuBar.h"
#include "CommandBars/XTPPopupBar.h"
#include "CommandBars/XTPControls.h"
#include "CommandBars/XTPControl.h"
#include "CommandBars/XTPControlButton.h"
#include "CommandBars/XTPControlPopup.h"

#include "CommandBars/XTPPaintManager.h"
#include "CommandBars/XTPCustomizeCommandsPage.h"
#include "CommandBars/XTPCustomizeSheet.h"

#include "XTPRibbonCustomizePage.h"
#include "XTPRibbonPopups.h"
#include "XTPRibbonBar.h"
#include "XTPRibbonTab.h"
#include "XTPRibbonGroups.h"
#include "XTPRibbonGroup.h"
#include "XTPRibbonSystemButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//////////////////////////////////////////////////////////////////////////
// CXTPRibbonCustomizeTreeCtrl


AFX_INLINE CXTPControl* ControlFromItem(CXTPCoreTreeItem* pItem)
{
	return DYNAMIC_DOWNCAST(CXTPControl, (CCmdTarget*)pItem->GetItemData());
}


CXTPRibbonCustomizeTreeCtrl::CXTPRibbonCustomizeTreeCtrl()
{
	m_bItemsTree = FALSE;
}

int CXTPRibbonCustomizeTreeCtrl::GetItemLevel(CXTPCoreTreeItem* pItem) const
{
	int nLevel = 0;
	while (pItem->GetParentItem() != NULL)
	{
		nLevel++;
		pItem = pItem->GetParentItem();
	}

	return nLevel;
}

void CXTPRibbonCustomizeTreeCtrl::UpdateCommandBars()
{
	CXTPRibbonCustomizePage* pPage = (CXTPRibbonCustomizePage*)GetParent();
	pPage->m_pCommandBars->UpdateCommandBars();
}

void CXTPRibbonCustomizeTreeCtrl::StartDragItem(CXTPCoreTreeItem* pItem)
{
	ASSERT(pItem);


	if (!pItem->IsEnabled() || pItem->IsLabel())
		return;

	if (m_bItemsTree)
	{
		DragItem(pItem, DROPEFFECT_COPY);
	}
	else
	{
		pItem->AddRef();

		int nLevel = GetItemLevel(pItem);
		if (nLevel != 2 && nLevel != 0)
			return;

		if (DragItem(pItem, DROPEFFECT_MOVE | DROPEFFECT_COPY))
		{
			CXTPControl* pControl = ControlFromItem(pItem);
			if (pControl != NULL)
			{
				pControl->GetRibbonGroup()->Remove(pControl);
			}

			UpdateCommandBars();

		}

		pItem->Release();
	}
}

BOOL CXTPRibbonCustomizeTreeCtrl::OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint /*point*/)
{
	if (!m_pDragOverItem)
		return FALSE;

	CXTPCoreTreeItem* pDragOverItem = m_pDragOverItem;
	m_pDragOverItem = NULL;

	RedrawControl();

	if (dropEffect != DROPEFFECT_COPY && dropEffect != DROPEFFECT_MOVE)
		return FALSE;

	if (!pDataObject || !pDataObject->IsDataAvailable(GetClipboardFormat()))
		return FALSE;

	CXTPCoreTreeItem* pItemDrop = CXTPCoreTreeItem::CreateFromOleData(pDataObject);
	if (!pItemDrop)
		return FALSE;

	CXTPControl* pControlToClone = NULL;
	if (m_pDraggingItem)
	{
		pControlToClone = ControlFromItem(m_pDraggingItem);
	}
	else
	{
		pControlToClone = ControlFromItem(pItemDrop);

	}

	if (pControlToClone)
	{
		if (m_nDropPosition == xtpCoreTreeDropAfter)
		{
			pDragOverItem->GetParentItems()->AddItemAfter(pItemDrop, pDragOverItem);

			CXTPControl* pControlDragOver = ControlFromItem(pDragOverItem);
			ASSERT(pControlDragOver);

			if (pControlDragOver)
			{
				CXTPControl* pControlNew = pControlDragOver->GetRibbonGroup()->AddClone(pControlToClone, pControlDragOver->GetRibbonGroup()->IndexOf(pControlDragOver) + 1);

				pItemDrop->SetItemData((DWORD_PTR)pControlNew);
			}
		}
		else if (m_nDropPosition == xtpCoreTreeDropBefore)
		{
			pDragOverItem->GetParentItems()->AddItemBefore(pItemDrop, pDragOverItem);

			CXTPControl* pControlDragOver = ControlFromItem(pDragOverItem);
			ASSERT(pControlDragOver);

			if (pControlDragOver)
			{
				CXTPControl* pControlNew = pControlDragOver->GetRibbonGroup()->AddClone(pControlToClone, pControlDragOver->GetRibbonGroup()->IndexOf(pControlDragOver));
				pItemDrop->SetItemData((DWORD_PTR)pControlNew);
			}
		}
		else if (m_nDropPosition == xtpCoreTreeDropChild)
		{
			pDragOverItem->GetChildren()->AddItemBefore(pItemDrop, NULL);
			pDragOverItem->SetExpanded();

			CXTPRibbonGroup* pGroupDragOver = DYNAMIC_DOWNCAST(CXTPRibbonGroup, (CCmdTarget*)pDragOverItem->GetItemData());
			ASSERT(pGroupDragOver);

			if (pGroupDragOver)
			{
				CXTPControl* pControlNew = pGroupDragOver->AddClone(pControlToClone, 0);
				pItemDrop->SetItemData((DWORD_PTR)pControlNew);
			}
		}

		SetFocusedItem(pItemDrop);
	}
	else
	{
		if (m_pDraggingItem && DYNAMIC_DOWNCAST(CXTPRibbonTab, (CCmdTarget*)m_pDraggingItem->GetItemData()))
		{
			pItemDrop->Release();

			CXTPRibbonTab* pTabDragging = DYNAMIC_DOWNCAST(CXTPRibbonTab, (CCmdTarget*)m_pDraggingItem->GetItemData());

			if (m_nDropPosition == xtpCoreTreeDropAfter)
			{
				pDragOverItem->GetParentItems()->MoveItemAfter(m_pDraggingItem, pDragOverItem);

				CXTPRibbonTab* pTabDragOver = DYNAMIC_DOWNCAST(CXTPRibbonTab, (CCmdTarget*)pDragOverItem->GetItemData());
				ASSERT(pTabDragOver);

				if (pTabDragOver)
				{
					pTabDragging->Move(pTabDragging->GetIndex() <= pTabDragOver->GetIndex() ? pTabDragOver->GetIndex() : pTabDragOver->GetIndex() + 1);
				}

			}
			else if (m_nDropPosition == xtpCoreTreeDropBefore)
			{
				pDragOverItem->GetParentItems()->MoveItemBefore(m_pDraggingItem, pDragOverItem);

				CXTPRibbonTab* pTabDragOver = DYNAMIC_DOWNCAST(CXTPRibbonTab, (CCmdTarget*)pDragOverItem->GetItemData());
				ASSERT(pTabDragOver);

				if (pTabDragOver)
				{
					pTabDragging->Move(pTabDragging->GetIndex() < pTabDragOver->GetIndex() ? pTabDragOver->GetIndex() - 1 : pTabDragOver->GetIndex());
				}
			}
			else
			{
				ASSERT(FALSE);
			}

			m_pDraggingItem = NULL;
		}
		else
		{
			ASSERT(FALSE);
		}

	}

	UpdateCommandBars();

	return TRUE;


}


DROPEFFECT CXTPRibbonCustomizeTreeCtrl::OnDragOver(COleDataObject* /*pDataObject*/, DWORD dwKeyState, CPoint point)
{
	if (m_bItemsTree)
		return DROPEFFECT_NONE;


	CXTPCoreTreeItem* pDragOverItem = HitTest(point);
	if (!pDragOverItem || pDragOverItem == m_pDraggingItem)
	{
		SetDragOverItem(NULL);

		return DROPEFFECT_NONE;
	}

	BOOL bDragControl = m_pDraggingItem == NULL || ControlFromItem(m_pDraggingItem) != NULL;

	if (bDragControl)
	{
		int nLevel = GetItemLevel(pDragOverItem);

		if (nLevel == 2)
		{
			CRect rcItem = pDragOverItem->GetScreenRect();

			XTPCoreTreeDropPosition nDropPosition = rcItem.CenterPoint().y > point.y ? xtpCoreTreeDropBefore : xtpCoreTreeDropAfter;

			SetDragOverItem(pDragOverItem, nDropPosition);
		}
		else if (nLevel == 1)
		{
			SetDragOverItem(pDragOverItem, xtpCoreTreeDropChild);
		}
		else
		{
			SetDragOverItem(NULL);
		}

		DROPEFFECT dropEffect = DROPEFFECT_MOVE;

		if ((dwKeyState & MK_CONTROL) == MK_CONTROL)
		{
			dropEffect = DROPEFFECT_COPY;
		}
		return m_pDragOverItem != NULL ? dropEffect : DROPEFFECT_NONE;

	}
	else if (m_pDraggingItem && GetItemLevel(m_pDraggingItem) == 0)
	{

		int nLevel = GetItemLevel(pDragOverItem);
		if (nLevel == 0 && !pDragOverItem->IsLabel())
		{
			CRect rcItem = pDragOverItem->GetScreenRect();

			XTPCoreTreeDropPosition nDropPosition = rcItem.CenterPoint().y > point.y ? xtpCoreTreeDropBefore : xtpCoreTreeDropAfter;

			SetDragOverItem(pDragOverItem, nDropPosition);
		}
		else
		{
			SetDragOverItem(NULL);
		}
		return m_pDragOverItem != NULL ? DROPEFFECT_MOVE : DROPEFFECT_NONE;

	}
	else
	{
		SetDragOverItem(NULL);
		return DROPEFFECT_NONE;

	}
}



/////////////////////////////////////////////////////////////////////////////
// CXTPRibbonCustomizePage property page

CXTPRibbonCustomizePage::CXTPRibbonCustomizePage(CXTPCustomizeSheet* pSheet)
	:   CXTPPropertyPage(XTP_IDD_RIBBONCUSTOMIZE_RIBBON)
{
	m_pSheet = pSheet;
	m_pCommandBars = pSheet->GetCommandBars();
}

CXTPRibbonCustomizePage::CXTPRibbonCustomizePage(CXTPCommandBars* pCommandBars)
:   CXTPPropertyPage(XTP_IDD_RIBBONCUSTOMIZE_RIBBON)
{
	m_pSheet = NULL;
	m_pCommandBars = pCommandBars;

	m_treeRibbon.RegisterWindowClass(XTPResourceManager()->GetResourceHandle());
}


CXTPRibbonCustomizePage::~CXTPRibbonCustomizePage()
{
	for (int i = 0; i < m_arrCategories.GetSize(); i++)
	{
		delete m_arrCategories[i];
	}
}

void CXTPRibbonCustomizePage::DoDataExchange(CDataExchange* pDX)
{
	CXTPPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CXTPRibbonCustomizePage)
	DDX_Control(pDX, XTP_IDC_RIBBONCOMBO_CATEGORIES, m_lstCategories);
	DDX_Control(pDX, XTP_IDC_RIBBONLIST_QUICKACCESS, m_treeRibbon);
	DDX_Control(pDX, XTP_IDC_RIBBONLIST_COMMANDS, m_lstCommands);
	DDX_Control(pDX, XTP_IDC_RIBBONCOMBO_TABGROUP, m_lstTabGroup);

	DDX_Control(pDX, XTP_IDC_RIBBONBUTTON_IMPORT, m_btnImport);
	DDX_Control(pDX, XTP_IDC_RIBBONBUTTON_UP, m_btnUp);
	DDX_Control(pDX, XTP_IDC_RIBBONBUTTON_DOWN, m_btnDown);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CXTPRibbonCustomizePage, CXTPPropertyPage)
	//{{AFX_MSG_MAP(CXTPRibbonCustomizePage)
	ON_LBN_SELCHANGE(XTP_IDC_RIBBONCOMBO_CATEGORIES, OnCategoriesSelectionChanged)
	ON_BN_CLICKED(XTP_IDC_RIBBONBUTTON_ADD, OnButtonAdd)
	ON_BN_CLICKED(XTP_IDC_RIBBONBUTTON_REMOVE, OnButtonRemove)
	ON_BN_CLICKED(XTP_IDC_RIBBONBUTTON_RESET, OnButtonReset)

	ON_BN_CLICKED(XTP_IDC_RIBBONBUTTON_NEWTAB, OnButtonNewTab)
	ON_BN_CLICKED(XTP_IDC_RIBBONBUTTON_NEWGROUP, OnButtonNewGroup)
	ON_BN_CLICKED(XTP_IDC_RIBBONBUTTON_RENAME, OnButtonRename)

	ON_BN_CLICKED(XTP_IDC_RIBBONBUTTON_UP, OnButtonUp)
	ON_BN_CLICKED(XTP_IDC_RIBBONBUTTON_DOWN, OnButtonDown)
	//}}AFX_MSG_MAP

	ON_NOTIFY(TVN_SELCHANGED, XTP_IDC_RIBBONLIST_COMMANDS, OnCommandsSelChanged)

	ON_NOTIFY(TVN_SELCHANGED, XTP_IDC_RIBBONLIST_QUICKACCESS, OnRibbonSelChanged)

	ON_NOTIFY(XTP_TVN_CHECKCHANGED, XTP_IDC_RIBBONLIST_QUICKACCESS, OnRibbonTreeCheckChanged)

	ON_CBN_DROPDOWN(XTP_IDC_RIBBONBUTTON_IMPORT, OnImportDropDown)


END_MESSAGE_MAP()

CXTPRibbonBar* CXTPRibbonCustomizePage::GetRibbonBar()
{
	CXTPRibbonBar* pRibbonBar = DYNAMIC_DOWNCAST(CXTPRibbonBar, m_pCommandBars->GetMenuBar());
	ASSERT(pRibbonBar);

	return pRibbonBar;
}

/////////////////////////////////////////////////////////////////////////////
// CXTPRibbonCustomizePage message handlers

BOOL CXTPRibbonCustomizePage::OnInitDialog()
{
	CXTPPropertyPage::OnInitDialog();

	ModifyStyle(0, WS_CLIPCHILDREN);

	for (int i = 0; i < m_arrCategories.GetSize(); i++)
	{
		int nIndex = m_lstCategories.AddString(m_arrCategories[i]->strCategory);
		m_lstCategories.SetItemData(nIndex, i);

		if (i == 0) m_lstCategories.SetCurSel(0);
	}

	m_lstCommands.m_pCommandBars = m_pCommandBars;
	m_lstCommands.m_bItemsTree = TRUE;

	m_lstCommands.GetPaintManager()->m_bShowButtons = FALSE;

	m_treeRibbon.m_pCommandBars = m_pCommandBars;

	m_lstCommands.SetImageManager(m_pCommandBars->GetImageManager());
	CMDTARGET_ADDREF(m_pCommandBars->GetImageManager());

	m_treeRibbon.SetImageManager(m_pCommandBars->GetImageManager());
	CMDTARGET_ADDREF(m_pCommandBars->GetImageManager());

	if (GetDlgItem(XTP_IDC_RIBBONPAGE_TITLE) != NULL)
	{
		m_wndTitle.SubclassDlgItem(XTP_IDC_RIBBONPAGE_TITLE, this);

		m_wndTitle.SetMarkupTextEx(_T("<StackPanel Orientation='Horizontal'><Image Source='res://XTP_IDB_RIBBON_ICONQUICKACCESS'/>")
			_T("<TextBlock Padding='3, 0, 0, 0' FontSize='13' VerticalAlignment='Center'>%s</TextBlock></StackPanel>"));
	}

	OnCategoriesSelectionChanged();
	RefreshRibbonList();

	UpdateData(FALSE);

	if (m_pCommandBars->IsLayoutRTL())
	{
		m_lstCommands.ModifyStyleEx(0, WS_EX_LAYOUTRTL);
		m_treeRibbon.ModifyStyleEx(0, WS_EX_LAYOUTRTL);
		m_lstCategories.ModifyStyleEx(0, WS_EX_RIGHT | WS_EX_LEFTSCROLLBAR | WS_EX_RTLREADING);
	}


	SetResize(XTP_IDC_RIBBONCOMBO_CATEGORIES, XTP_ANCHOR_TOPLEFT, XTP_ANCHOR_TOPCENTER);
	SetResize(XTP_IDC_RIBBONLIST_COMMANDS, XTP_ANCHOR_TOPLEFT, XTP_ANCHOR_BOTTOMCENTER);
	SetResize(XTP_IDC_RIBBONLIST_QUICKACCESS, XTP_ANCHOR_TOPCENTER, XTP_ANCHOR_BOTTOMRIGHT);

	SetResize(XTP_IDC_RIBBONBUTTON_RESET, XTP_ANCHOR_BOTTOMCENTER, XTP_ANCHOR_BOTTOMCENTER);
	SetResize(XTP_IDC_RIBBONBUTTON_NEWTAB, XTP_ANCHOR_BOTTOMCENTER, XTP_ANCHOR_BOTTOMCENTER);
	SetResize(XTP_IDC_RIBBONBUTTON_NEWGROUP, XTP_ANCHOR_BOTTOMCENTER, XTP_ANCHOR_BOTTOMCENTER);
	SetResize(XTP_IDC_RIBBONBUTTON_RENAME, XTP_ANCHOR_BOTTOMCENTER, XTP_ANCHOR_BOTTOMCENTER);

	SetResize(XTP_IDC_RIBBONBUTTON_ADD,  CXTPResizePoint(.5, 0.5), CXTPResizePoint(.5, 0.5));
	SetResize(XTP_IDC_RIBBONBUTTON_REMOVE, CXTPResizePoint(.5, 0.5), CXTPResizePoint(.5, 0.5));

	SetResize(XTP_IDC_RIBBONBUTTON_IMPORT, XTP_ANCHOR_BOTTOMCENTER, XTP_ANCHOR_BOTTOMCENTER);
	SetResize(XTP_IDC_RIBBONSTATIC_CUSTOMIZATIONS, XTP_ANCHOR_BOTTOMCENTER, XTP_ANCHOR_BOTTOMCENTER);
	SetResize(XTP_IDC_RIBBONSTATIC_CUSTOMIZE, XTP_ANCHOR_TOPCENTER, XTP_ANCHOR_TOPCENTER);
	SetResize(XTP_IDC_RIBBONCOMBO_TABGROUP, XTP_ANCHOR_TOPCENTER, XTP_ANCHOR_TOPRIGHT);

	SetResize(XTP_IDC_RIBBONBUTTON_UP,  CXTPResizePoint(1, 0.5), CXTPResizePoint(1, 0.5));
	SetResize(XTP_IDC_RIBBONBUTTON_DOWN, CXTPResizePoint(1, 0.5), CXTPResizePoint(1, 0.5));

	//GetDlgItem(XTP_IDC_RIBBONBUTTON_UP)->ShowWindow(SW_HIDE);
	//GetDlgItem(XTP_IDC_RIBBONBUTTON_DOWN)->ShowWindow(SW_HIDE);


	GetDlgItem(XTP_IDC_RIBBONBUTTON_ADD)->EnableWindow(FALSE);
	GetDlgItem(XTP_IDC_RIBBONBUTTON_REMOVE)->EnableWindow(FALSE);

	m_lstTabGroup.AddString(XTPResourceManager()->LoadString(XTP_IDS_RIBBONCUSTOMIZE_MAINTABS));
	m_lstTabGroup.SetCurSel(0);

	m_btnImport.SetPushButtonStyle(xtpButtonDropDown);

	CImageList imageList;
	imageList.Create(11, 11, ILC_COLOR8 | ILC_MASK, 0, 1);

	CBitmap bmp;
	CXTPResourceManager::AssertValid(XTPResourceManager()->LoadBitmap(&bmp, XTP_IDB_RIBBON_UPDOWN));
	imageList.Add(&bmp, RGB(0xff, 0x00, 0xff));

	if (imageList.GetImageCount() == 2)
	{
		HICON hPopupIcon = imageList.ExtractIcon(0);

		m_btnUp.SetIcon(CSize(11, 11), hPopupIcon);
		DestroyIcon(hPopupIcon);

		hPopupIcon = imageList.ExtractIcon(1);

		m_btnDown.SetIcon(CSize(11, 11), hPopupIcon);
		DestroyIcon(hPopupIcon);
	}


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CXTPRibbonCustomizePage::RefreshRibbonList()
{
	m_treeRibbon.SetLockRedraw(TRUE);

	m_treeRibbon.GetItems()->RemoveAll();

	CXTPCoreTreeItem* pLabelItem = m_treeRibbon.GetItems()->AddItem(XTPResourceManager()->LoadString(XTP_IDS_RIBBONCUSTOMIZE_MAINTABS));
	pLabelItem->SetLabel(TRUE);


	CXTPRibbonBar* pRibbonBar = GetRibbonBar();

	for (int i = 0; i < pRibbonBar->GetTabCount(); i++)
	{
		CXTPRibbonTab* pTab = pRibbonBar->GetTab(i);

		CString strCaption(pTab->GetCaption());
		XTPDrawHelpers()->StripMnemonics(strCaption);

		CXTPCoreTreeItem* pItemTab = m_treeRibbon.GetItems()->AddItem(strCaption);
		pItemTab->ShowCheckbox(TRUE);
		pItemTab->SetChecked(pTab->IsVisible());
		pItemTab->SetItemData((DWORD_PTR)pTab);

		for (int g = 0; g < pTab->GetGroups()->GetCount(); g++)
		{
			CXTPRibbonGroup* pGroup = pTab->GetGroups()->GetAt(g);

			CString strCaption(pGroup->GetCaption());
			XTPDrawHelpers()->StripMnemonics(strCaption);

			CXTPCoreTreeItem* pItemGroup = pItemTab->GetChildren()->AddItem(strCaption);
			pItemGroup->SetItemData((DWORD_PTR)pGroup);

			for(int c = 0; c < pGroup->GetCount(); c++)
			{
				CXTPControl* pControl = pGroup->GetAt(c);

				CString strCaption(pControl->GetCaption());
				XTPDrawHelpers()->StripMnemonics(strCaption);

				CXTPCoreTreeItem* pItemControl = pItemGroup->GetChildren()->AddItem(strCaption);
				pItemControl->SetItemData((DWORD_PTR)pControl);
				pItemControl->SetIconId(pControl->GetIconId());
			}
		}
	}


	m_treeRibbon.SetLockRedraw(FALSE);

	OnRibbonSelectionChanged();
	OnCommandsSelectionChanged();
}

void CXTPRibbonCustomizePage::OnCategoriesSelectionChanged()
{

	m_lstCommands.GetItems()->RemoveAll();

	int nIndex = m_lstCategories.GetCurSel();

	if (nIndex == LB_ERR)
		return;

	XTP_COMMANDBARS_CATEGORYINFO* pInfo = GetCategoryInfo((int)m_lstCategories.GetItemData(nIndex));

	if (pInfo == NULL)
		return;

	m_lstCommands.SetLockRedraw(TRUE);

	for (int i = 0; i < pInfo->pControls->GetCount(); i++)
	{
		CXTPControl* pControl = pInfo->pControls->GetAt(i);

		CString strCaption(pControl->GetCaption());
		CXTPPaintManager::StripMnemonics(strCaption);

		CXTPCoreTreeItem* pItem = m_lstCommands.GetItems()->AddItem(strCaption);
		pItem->SetItemData((DWORD_PTR)pControl);
		pItem->SetIconId(pControl->GetIconId());

	}

	m_lstCommands.SetLockRedraw(FALSE);

	OnCommandsSelectionChanged();
}


XTP_COMMANDBARS_CATEGORYINFO* CXTPRibbonCustomizePage::FindCategory(LPCTSTR strCategory) const
{
	for (int i = 0; i < m_arrCategories.GetSize(); i++)
	{
		if (m_arrCategories[i]->strCategory.Compare(strCategory) == 0)
			return m_arrCategories[i];
	}
	return NULL;
}

XTP_COMMANDBARS_CATEGORYINFO* CXTPRibbonCustomizePage::GetCategoryInfo(int nIndex)
{
	if (nIndex >= 0 && nIndex < m_arrCategories.GetSize())
		return m_arrCategories[nIndex];
	return NULL;
}

BOOL CXTPRibbonCustomizePage::AddCategory(LPCTSTR strCategory, CMenu* pMenu, BOOL bListSubItems)
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


BOOL CXTPRibbonCustomizePage::AddCategories(UINT nIDResource, BOOL bListSubItems)
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

BOOL CXTPRibbonCustomizePage::AddCategories(CXTPControls* pControls)
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


CXTPControls* CXTPRibbonCustomizePage::InsertCategory(LPCTSTR strCategory, int nIndex)
{
	XTP_COMMANDBARS_CATEGORYINFO* pInfo = FindCategory(strCategory);
	if (!pInfo)
	{
		pInfo = new XTP_COMMANDBARS_CATEGORYINFO(strCategory, m_pCommandBars);
		m_arrCategories.InsertAt(nIndex == -1 ? m_arrCategories.GetSize() : nIndex, pInfo);
	}
	return pInfo->pControls;
}

CXTPControls* CXTPRibbonCustomizePage::GetControls(LPCTSTR strCategory)
{
	XTP_COMMANDBARS_CATEGORYINFO* pInfo = FindCategory(strCategory);
	return pInfo ? pInfo->pControls : NULL;
}


void CXTPRibbonCustomizePage::OnButtonAdd()
{
	CXTPCoreTreeItem* pItem = m_lstCommands.GetFocusedItem();
	if (!pItem)
		return;

	CXTPControl* pControlToAdd = DYNAMIC_DOWNCAST(CXTPControl, (CCmdTarget*)pItem->GetItemData());
	if (!pControlToAdd)
		return;


	pItem = m_treeRibbon.GetFocusedItem();
	if (!pItem)
		return;

	CXTPControl* pControl = DYNAMIC_DOWNCAST(CXTPControl, (CCmdTarget*)pItem->GetItemData());
	if (pControl)
	{
		CXTPControl* pControlNew = pControl->GetRibbonGroup()->AddClone(pControlToAdd, pControl->GetRibbonGroup()->IndexOf(pControl) + 1);


		CString strCaption(pControlNew->GetCaption());
		XTPDrawHelpers()->StripMnemonics(strCaption);

		CXTPCoreTreeItem* pItemControl = pItem->GetParentItems()->AddItemAfter(new CXTPCoreTreeItem(), pItem);
		pItemControl->SetCaption(strCaption);
		pItemControl->SetItemData((DWORD_PTR)pControlNew);
		pItemControl->SetIconId(pControlNew->GetIconId());

		m_treeRibbon.SetFocusedItem(pItemControl);

		m_pCommandBars->UpdateCommandBars();

		return;
	}

	CXTPRibbonGroup* pGroup = DYNAMIC_DOWNCAST(CXTPRibbonGroup, (CCmdTarget*)pItem->GetItemData());
	if (pGroup)
	{
		CXTPControl* pControlNew = pGroup->AddClone(pControlToAdd, -1);


		CString strCaption(pControlNew->GetCaption());
		XTPDrawHelpers()->StripMnemonics(strCaption);

		CXTPCoreTreeItem* pItemControl = pItem->GetChildren()->AddItemAfter(new CXTPCoreTreeItem());
		pItemControl->SetCaption(strCaption);
		pItemControl->SetItemData((DWORD_PTR)pControlNew);
		pItemControl->SetIconId(pControlNew->GetIconId());

		m_treeRibbon.SetFocusedItem(pItemControl);

		m_pCommandBars->UpdateCommandBars();

		return;
	}
}

void CXTPRibbonCustomizePage::OnButtonRemove()
{
	CXTPCoreTreeItem* pItem = m_treeRibbon.GetFocusedItem();
	if (!pItem)
		return;

	CXTPControl* pControl = DYNAMIC_DOWNCAST(CXTPControl, (CCmdTarget*)pItem->GetItemData());
	if (pControl)
	{
		pControl->GetRibbonGroup()->Remove(pControl);

		pItem->Remove();

		m_pCommandBars->UpdateCommandBars();

		return;
	}

	CXTPRibbonGroup* pGroup = DYNAMIC_DOWNCAST(CXTPRibbonGroup, (CCmdTarget*)pItem->GetItemData());

	if (pGroup)
	{
		pGroup->GetGroups()->Remove(pGroup);

		pItem->Remove();

		m_pCommandBars->UpdateCommandBars();

		return;
	}


	CXTPRibbonTab* pTab = DYNAMIC_DOWNCAST(CXTPRibbonTab, (CCmdTarget*)pItem->GetItemData());

	if (pTab)
	{
		pTab->Remove();

		pItem->Remove();

		m_pCommandBars->UpdateCommandBars();
	}
}

void CXTPRibbonCustomizePage::OnCommandsSelectionChanged()
{
	GetDlgItem(XTP_IDC_RIBBONBUTTON_ADD)->EnableWindow(m_lstCommands.GetFocusedItem() != NULL);

}

void CXTPRibbonCustomizePage::OnRibbonSelectionChanged()
{
	CXTPCoreTreeItem* pItem = m_treeRibbon.GetFocusedItem();
	if (!pItem)
		return;

	CXTPControl* pControl = DYNAMIC_DOWNCAST(CXTPControl, (CCmdTarget*)pItem->GetItemData());
	CXTPRibbonGroup* pGroup = DYNAMIC_DOWNCAST(CXTPRibbonGroup, (CCmdTarget*)pItem->GetItemData());
	CXTPRibbonTab* pTab = DYNAMIC_DOWNCAST(CXTPRibbonTab, (CCmdTarget*)pItem->GetItemData());

	if (pControl || pGroup || pTab)
	{
		GetDlgItem(XTP_IDC_RIBBONBUTTON_REMOVE)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(XTP_IDC_RIBBONBUTTON_REMOVE)->EnableWindow(FALSE);

	}



	if (pTab)
	{
		if (pTab->GetIndex() == 0)
			m_btnUp.EnableWindow(FALSE);
		else
			m_btnUp.EnableWindow(TRUE);

		if (pTab->GetIndex() == pTab->GetTabManager()->GetItemCount() - 1)
			m_btnDown.EnableWindow(FALSE);
		else
			m_btnDown.EnableWindow(TRUE);

	}
	else
	{
		m_btnUp.EnableWindow(FALSE);
		m_btnDown.EnableWindow(FALSE);
	}

}

void CXTPRibbonCustomizePage::OnRibbonTreeCheckChanged(NMHDR* /*pNMHDR*/, LRESULT* /*pResult*/)
{
	CXTPCoreTreeItem* pItem = m_treeRibbon.GetFocusedItem();
	if (!pItem)
		return;

	CXTPRibbonTab* pTab = DYNAMIC_DOWNCAST(CXTPRibbonTab, (CCmdTarget*)pItem->GetItemData());
	if (pTab)
	{
		pTab->SetVisible(pItem->IsChecked());
	}

}

void CXTPRibbonCustomizePage::OnRibbonSelChanged(NMHDR* /*pNMHDR*/, LRESULT* /*pResult*/)
{
	OnRibbonSelectionChanged();
}

void CXTPRibbonCustomizePage::OnCommandsSelChanged(NMHDR* /*pNMHDR*/, LRESULT* /*pResult*/)
{
	OnCommandsSelectionChanged();
}

void CXTPRibbonCustomizePage::OnButtonNewTab()
{

	CXTPCoreTreeItem* pItemBefore = m_treeRibbon.GetFocusedItem();
	if (!pItemBefore)
		return;

	while (pItemBefore->GetParentItem() != NULL)
		pItemBefore = pItemBefore->GetParentItem();

	CXTPRibbonTab* pTabBefore = DYNAMIC_DOWNCAST(CXTPRibbonTab, (CCmdTarget*)pItemBefore->GetItemData());
	if (!pTabBefore)
		return;

	CXTPRibbonBar* pRibbonBar = GetRibbonBar();

	CXTPRibbonTab* pTab = pRibbonBar->InsertTab(pTabBefore->GetIndex() + 1, XTPResourceManager()->LoadString(XTP_IDS_RIBBONCUSTOMIZE_NEWTAB));

	CXTPCoreTreeItem* pItemTab = m_treeRibbon.GetItems()->AddItemAfter(new CXTPCoreTreeItem(), pItemBefore);

	pItemTab->SetCaption(pTab->GetCaption());
	pItemTab->ShowCheckbox(TRUE);
	pItemTab->SetChecked(pTab->IsVisible());
	pItemTab->SetItemData((DWORD_PTR)pTab);


	CXTPRibbonGroup* pGroup = pTab->AddGroup(XTPResourceManager()->LoadString(XTP_IDS_RIBBONCUSTOMIZE_NEWGROUP));

	CXTPCoreTreeItem* pItemGroup = pItemTab->GetChildren()->AddItem(pGroup->GetCaption());
	pItemGroup->SetItemData((DWORD_PTR)pGroup);

	m_treeRibbon.SetFocusedItem(pItemGroup);

	m_treeRibbon.UpdateCommandBars();

}

void CXTPRibbonCustomizePage::OnButtonNewGroup()
{
	CXTPCoreTreeItem* pItemTab = m_treeRibbon.GetFocusedItem();
	if (!pItemTab)
		return;

	while (pItemTab->GetParentItem() != NULL)
		pItemTab = pItemTab->GetParentItem();

	CXTPRibbonTab* pTab = DYNAMIC_DOWNCAST(CXTPRibbonTab, (CCmdTarget*)pItemTab->GetItemData());
	if (!pTab)
		return;

	CXTPRibbonGroup* pGroup = pTab->AddGroup(XTPResourceManager()->LoadString(XTP_IDS_RIBBONCUSTOMIZE_NEWGROUP));

	CXTPCoreTreeItem* pItemGroup = pItemTab->GetChildren()->AddItem(pGroup->GetCaption());
	pItemGroup->SetItemData((DWORD_PTR)pGroup);

	m_treeRibbon.SetFocusedItem(pItemGroup);

	m_treeRibbon.UpdateCommandBars();
}

class CXTPRibbonRenameDialog : public CDialog
{
public:
	CXTPRibbonRenameDialog()
		: CDialog(XTP_IDD_RIBBONCUSTOMIZE_RENAME)
	{

	}

	void DoDataExchange(CDataExchange* pDX)
	{
		CDialog::DoDataExchange(pDX);
		DDX_Text(pDX, XTP_IDC_RIBBONEDIT_RENAME, m_strName);
	}

public:
	CString m_strName;

};

void CXTPRibbonCustomizePage::OnButtonRename()
{
	CXTPCoreTreeItem* pItem = m_treeRibbon.GetFocusedItem();
	if (!pItem)
		return;

	if (pItem)
	{
		CXTPControl* pControl = ControlFromItem(pItem);

		if (pControl)
		{
			CXTPRibbonRenameDialog dlg;
			dlg.m_strName = pControl->GetCaption();

			if (dlg.DoModal() == IDOK)
			{
				pControl->SetCaption(dlg.m_strName);

				XTPDrawHelpers()->StripMnemonics(dlg.m_strName);
				pItem->SetCaption(dlg.m_strName);

				m_treeRibbon.UpdateCommandBars();
			}

			return;
		}
	}

	if (pItem)
	{
		CXTPRibbonTab* pTab = DYNAMIC_DOWNCAST(CXTPRibbonTab, (CCmdTarget*)pItem->GetItemData());

		if (pTab)
		{
			CXTPRibbonRenameDialog dlg;
			dlg.m_strName = pTab->GetCaption();

			if (dlg.DoModal() == IDOK)
			{
				pTab->SetCaption(dlg.m_strName);

				XTPDrawHelpers()->StripMnemonics(dlg.m_strName);
				pItem->SetCaption(dlg.m_strName);

				m_treeRibbon.UpdateCommandBars();
			}

			return;
		}
	}

	if (pItem)
	{
		CXTPRibbonGroup* pGroup = DYNAMIC_DOWNCAST(CXTPRibbonGroup, (CCmdTarget*)pItem->GetItemData());

		if (pGroup)
		{
			CXTPRibbonRenameDialog dlg;
			dlg.m_strName = pGroup->GetCaption();

			if (dlg.DoModal() == IDOK)
			{
				pGroup->SetCaption(dlg.m_strName);

				XTPDrawHelpers()->StripMnemonics(dlg.m_strName);
				pItem->SetCaption(dlg.m_strName);

				m_treeRibbon.UpdateCommandBars();
			}

			return;
		}
	}
}


void CXTPRibbonCustomizePage::OnButtonReset()
{
	CString strPrompt = XTPResourceManager()->LoadString(XTP_IDS_RIBBONCUSTOMIZE_CONFIRMRIBBONRESET);

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


	m_pCommandBars->GetSite()->SendMessage(WM_XTP_CUSTOMIZATION_RIBBONRESET);

	RefreshRibbonList();
}

void CXTPRibbonCustomizePage::OnImportDropDown()
{
	CMenu menu;
	XTPResourceManager()->LoadMenu(&menu, XTP_IDM_RIBBONCUSTOMIZE_IMPORT);

	CRect rc;
	GetDlgItem(XTP_IDC_RIBBONBUTTON_IMPORT)->GetWindowRect(&rc);

	UINT nCommand = TrackPopupMenu(menu.GetSubMenu(0)->GetSafeHmenu(), TPM_RETURNCMD, rc.left, rc.bottom, 0, m_hWnd, rc);

	if (nCommand == XTP_ID_RIBBONCUSTOMIZE_IMPORT)
	{
		CString strFilter = _T("XML Document(*.xml)|*.xml|All files (*.*)|*.*||");
		CFileDialog fd(TRUE, _T("xml"), NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, strFilter);
		if (fd.DoModal() == IDOK)
		{
			CXTPPropExchangeXMLNode px(TRUE, 0, _T("Settings"));
			if (!px.LoadFromFile(fd.GetPathName()))
				return;

			CXTPRibbonBar* pRibbonBar = GetRibbonBar();
			CXTPCommandBar* pSystemPopupBar = pRibbonBar->GetSystemButton() ? pRibbonBar->GetSystemButton()->GetCommandBar() : NULL;
			CMDTARGET_ADDREF(pSystemPopupBar);

			XTP_COMMANDBARS_PROPEXCHANGE_PARAM param;
			param.bSaveOriginalControls = FALSE;
			param.bSerializeControls = TRUE;
			param.bSerializeOptions = FALSE;
			param.bSerializeLayout = FALSE;
			param.bSerializeActions = TRUE;

			m_pCommandBars->DoPropExchange(&px, &param);

			m_pCommandBars->UpdateCommandBars();

			if (pSystemPopupBar)
			{
				GetRibbonBar()->GetSystemButton()->SetCommandBar(pSystemPopupBar);
				CMDTARGET_RELEASE(pSystemPopupBar);
			}

			RefreshRibbonList();

		}

	}
	else if (nCommand == XTP_ID_RIBBONCUSTOMIZE_EXPORT)
	{

		CString strFilter = _T("XML Document(*.xml)|*.xml|All files (*.*)|*.*||");
		CFileDialog fd(FALSE, _T("xml"), NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, strFilter);
		if (fd.DoModal() == IDOK)
		{
			CXTPPropExchangeXMLNode px(FALSE, 0, _T("Settings"));

			XTP_COMMANDBARS_PROPEXCHANGE_PARAM param;
			param.bSaveOriginalControls = FALSE;
			param.bSerializeControls = TRUE;
			param.bSerializeOptions = FALSE;
			param.bSerializeLayout = FALSE;
			param.bSerializeActions = TRUE;

			m_pCommandBars->DoPropExchange(&px, &param);

			px.SaveToFile(fd.GetPathName());

		}
	}

}

CXTPCoreTreeItem* CXTPRibbonCustomizePage::FindItem(CCmdTarget* pItemData, CXTPCoreTreeItems* pItems/* = NULL*/)
{
	if (pItems == NULL)
		pItems = m_treeRibbon.GetItems();

	for (CXTPCoreTreeItem* pItem = pItems->GetFirstItem(); pItem != NULL; pItem = pItem->GetNextSiblingItem())
	{
		if (pItem->GetItemData() == (DWORD_PTR)pItemData)
			return pItem;

		if (pItem->HasChildren())
		{
			CXTPCoreTreeItem* p = FindItem(pItemData, pItem->GetChildren());
			if (p)
				return p;
		}
	}
	return NULL;
}


void CXTPRibbonCustomizePage::OnButtonUp()
{
	CXTPCoreTreeItem* pItem = m_treeRibbon.GetFocusedItem();
	if (!pItem)
		return;

	CXTPRibbonTab* pTab = DYNAMIC_DOWNCAST(CXTPRibbonTab, (CCmdTarget*)pItem->GetItemData());

	if (!pTab)
		return;

	pTab->Move(pTab->GetIndex() - 1);

	m_pCommandBars->UpdateCommandBars();

	RefreshRibbonList();

	pItem = FindItem(pTab, m_treeRibbon.GetItems());
	if (pItem)
		m_treeRibbon.SetFocusedItem(pItem);

}

void CXTPRibbonCustomizePage::OnButtonDown()
{
	CXTPCoreTreeItem* pItem = m_treeRibbon.GetFocusedItem();
	if (!pItem)
		return;

	CXTPRibbonTab* pTab = DYNAMIC_DOWNCAST(CXTPRibbonTab, (CCmdTarget*)pItem->GetItemData());

	if (!pTab)
		return;

	pTab->Move(pTab->GetIndex() + 1);

	m_pCommandBars->UpdateCommandBars();

	RefreshRibbonList();

	pItem = FindItem(pTab, m_treeRibbon.GetItems());
	if (pItem)
		m_treeRibbon.SetFocusedItem(pItem);
}
