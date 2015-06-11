// XTPRibbonCustomizeTreeCtrl.cpp: implementation of the CXTPRibbonCustomizeTreeCtrl class.
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


#include "stdafx.h"

#include <Common/XTPResourceManager.h>
#include <Common/XTPSystemHelpers.h>
#include <Common/XTPHookManager.h>
#include <Common/XTPColorManager.h>
#include <Common/XTPDrawHelpers.h>
#include <Common/XTPImageManager.h>
#include <Common/XTPPropExchange.h>
#include <Common/ScrollBar/XTPScrollInfo.h>

#include <TabManager/Includes.h>

#include <Controls/Resize/XTPResizeRect.h>
#include <Controls/Resize/XTPResizePoint.h>
#include <Controls/Resize/XTPResize.h>
#include <Controls/Dialog/XTPPropertySheet.h>
#include <Controls/Dialog/XTPPropertyPage.h>
#include <Controls/Static/XTPMarkupStatic.h>
#include <Controls/Util/XTPControlTheme.h>
#include <Controls/Button/XTPButton.h>
#include <Controls/Combo/XTPComboBox.h>

#include <Controls/CoreTree/XTPCoreTreeControl.h>
#include <Controls/CoreTree/XTPCoreTreeItem.h>
#include <Controls/CoreTree/XTPCoreTreePaintManager.h>

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
#include <Ribbon/Customize/XTPRibbonCustomizeTreeCtrl.h>
#include <Ribbon/Customize/XTPRibbonCustomizePage.h>
#include <Ribbon/XTPRibbonPopups.h>
#include <Ribbon/XTPRibbonBar.h>
#include <Ribbon/XTPRibbonTab.h>
#include <Ribbon/XTPRibbonGroups.h>
#include <Ribbon/XTPRibbonGroup.h>
#include <Ribbon/XTPRibbonSystemButton.h>

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
