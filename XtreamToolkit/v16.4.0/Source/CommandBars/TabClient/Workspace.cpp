#include "StdAfx.h"

#include <Common/Resource.h>
#include <Common/XTPResourceManager.h>
#include <Common/XTPImageManager.h>
#include <Common/XTPDrawHelpers.h>
#include <Common/XTPToolTipContext.h>
#include <Common/XTPMarkupRender.h>
#include <Common/XTPColorManager.h>
#include <Common/XTPSystemHelpers.h>
#include <Common/XTPHookManager.h>
#include <Common/ScrollBar/XTPScrollInfo.h>
#include <Common/Hook/XTPWinEventHook.h>

#include <TabManager/Includes.h>

#include <CommandBars/XTPCommandBarsDefines.h>
#include <CommandBars/XTPCommandBars.h>
#include <CommandBars/XTPCommandBar.h>
#include <CommandBars/XTPPopupBar.h>
#include <CommandBars/XTPToolBar.h>
#include <CommandBars/XTPMenuBar.h>
#include <CommandBars/XTPControls.h>
#include <CommandBars/XTPControl.h>
#include <CommandBars/XTPControlButton.h>
#include <CommandBars/XTPControlEdit.h>
#include <CommandBars/XTPControlExt.h>
#include <CommandBars/XTPMouseManager.h>
#include <CommandBars/XTPPaintManager.h>
#include <CommandBars/Themes/XTPCommandBarsOfficeXPTheme.h>
#include <CommandBars/Themes/XTPCommandBarsOffice2003Theme.h>

#include <CommandBars/XTPTabClientWnd.h>
#include <CommandBars/TabClient/Workspace.h>



//////////////////////////////////////////////////////////////////////////
// CWorkspace

CXTPTabClientWnd::CWorkspace::CWorkspace()
{
	m_pTabClientWnd = NULL;
	m_dHeight = 100;

	m_pNewWindowItem = NULL;
}

CXTPTabClientWnd::CWorkspace::~CWorkspace()
{

}

void CXTPTabClientWnd::CWorkspace::SetSelectedItem(CXTPTabManagerItem* pItem)
{
	if (pItem == m_pNewWindowItem)
		pItem = GetItem(GetItemCount() - 1);

#ifdef _XTP_ACTIVEX
	if (m_pSelected != pItem)
	{
		if (pItem)
			m_pTabClientWnd->FireSelectedChanged(pItem->GetIDispatch(FALSE));
	}
#endif

	CXTPTabManager::SetSelectedItem(pItem);
}


BOOL CXTPTabClientWnd::CWorkspace::IsMouseLocked() const
{
	return CXTPTabManager::IsMouseLocked() || XTPMouseManager()->IsTrackedLock();
}

void CXTPTabClientWnd::CWorkspace::RedrawControl(LPCRECT lpRect, BOOL /*bAnimate*/)
{
	if (!m_pTabClientWnd->m_bLockReposition)
		m_pTabClientWnd->InvalidateRect(lpRect, FALSE);
}

void CXTPTabClientWnd::CWorkspace::Reposition()
{
	if (!m_pTabClientWnd)
		return;

	if (m_pTabClientWnd->m_bLockReposition)
	{
		m_pTabClientWnd->m_bForceToRecalc = TRUE;
	}
	else
	{
		m_pTabClientWnd->Reposition();
	}
}

BOOL CXTPTabClientWnd::CWorkspace::IsAllowReorder() const
{
	return m_pTabClientWnd->m_bAllowReorder;
}

void CXTPTabClientWnd::CWorkspace::SetAllowReorder(BOOL bAllowReorder)
{
	m_pTabClientWnd->m_bAllowReorder = bAllowReorder;
}

BOOL CXTPTabClientWnd::CWorkspace::IsDrawStaticFrame() const
{
	return GetPaintManager()->m_bStaticFrame && !m_pTabClientWnd->m_bEnableGroups;
}


BOOL CXTPTabClientWnd::CWorkspace::DrawIcon(CDC* pDC, CPoint pt, CXTPTabManagerItem* pItem, BOOL bDraw, CSize& szIcon) const
{
	if (pItem == m_pNewWindowItem)
	{
		if (!bDraw)
			return TRUE;

		CXTPCommandBars* pCommandBars = m_pTabClientWnd->GetCommandBars();
		CXTPImageManager* pImageManager = pCommandBars ? pCommandBars->GetImageManager() : XTPImageManager();

		CXTPImageManagerIcon* pIcon = pImageManager->GetImage(m_pTabClientWnd->m_nNewWindowTabCommand);
		if (pIcon) pIcon->Draw(pDC, pt, szIcon);

		return TRUE;
	}

	if (GetPaintManager()->m_bShowIcons == FALSE)
		return FALSE;

	if (!bDraw)
	{
		return TRUE;
	}


	if (pItem->GetImageIndex() != -1)
	{
		CXTPCommandBars* pCommandBars = m_pTabClientWnd->GetCommandBars();
		CXTPImageManager* pImageManager = pCommandBars ? pCommandBars->GetImageManager() : XTPImageManager();

		CXTPImageManagerIcon* pIcon = pImageManager->GetImage(pItem->GetImageIndex());
		if (pIcon) pIcon->Draw(pDC, pt, szIcon);

		return TRUE;
	}

	HICON hIcon = GetItemIcon(pItem);
	if (hIcon)
	{
		DrawIconEx(pDC->GetSafeHdc(), pt.x, pt.y, hIcon, szIcon.cx, szIcon.cy, 0, NULL, DI_NORMAL);
	}

	return TRUE;
}

CXTPTabPaintManager* CXTPTabClientWnd::CWorkspace::GetPaintManager() const
{
	return m_pTabClientWnd ? m_pTabClientWnd->m_pPaintManager : NULL;
}

CString CXTPTabClientWnd::CWorkspace::GetItemTooltip(const CXTPTabManagerItem* pItem) const
{
	return m_pTabClientWnd->GetItemTooltip(pItem);
}

// finds the tab item for specified window. Returns NULL if not found
CXTPTabManagerItem* CXTPTabClientWnd::CWorkspace::FindItem(const HWND hWnd) const
{
	// loop through all tab items
	for (int nIndex = 0; nIndex < GetItemCount(); nIndex++)
	{
		// check for window handle
		if (GetItem(nIndex)->GetHandle() == hWnd)
		{
			return GetItem(nIndex);
		}
	}

	return NULL;
}


// add new item into the tab control for specified MDIChild. If bRedraw is
// set to TRUE then framework will be redrawn in order to show new item.
CXTPTabManagerItem* CXTPTabClientWnd::CWorkspace::AddItem(const CWnd* pChildWnd)
{
	ASSERT(pChildWnd != NULL);
	if (!pChildWnd)
		return NULL;
	ASSERT(::IsWindow(pChildWnd->GetSafeHwnd()));

	// make sure we add MDIChild window
	if ((pChildWnd->GetExStyle() & WS_EX_MDICHILD) == 0)
		return NULL;

	if (GetItemCount() == 0)
		AddNewWindowItem();

	int nIndex = GetItemCount();

	if (m_pNewWindowItem) nIndex--;


	switch (m_pTabClientWnd->GetNewTabPositon())
	{
	case xtpWorkspaceNewTabLeftMost:
		nIndex = 0;
		break;

	case xtpWorkspaceNewTabNextToActive:
		nIndex = GetCurSel() + 1;
		break;
	}

	// save information about new entry
	CXTPTabManagerItem* pItem = CXTPTabManager::AddItem(nIndex);
	pItem->SetCaption(m_pTabClientWnd->GetItemText(pChildWnd));

	return pItem;
}

BOOL CXTPTabClientWnd::CWorkspace::OnBeforeItemClick(CXTPTabManagerItem* pItem)
{
	return m_pTabClientWnd->OnBeforeItemClick(pItem);
}

void CXTPTabClientWnd::CWorkspace::OnItemClick(CXTPTabManagerItem* pItem)
{
	ASSERT(pItem);
	if (!pItem)
		return;

	if (pItem == m_pNewWindowItem)
	{
#ifdef _XTP_ACTIVEX

		CXTPControlButton* pControl = new CXTPControlButton();
		pControl->SetID(m_pTabClientWnd->m_nNewWindowTabCommand);

		NMXTPCONTROL tagNMCONTROL;
		pControl->NotifySite(m_pTabClientWnd->GetParentFrame(), CBN_XTP_EXECUTE, &tagNMCONTROL);

		pControl->InternalRelease();
#else
		m_pTabClientWnd->GetParent()->SendMessage(WM_COMMAND, m_pTabClientWnd->m_nNewWindowTabCommand);
#endif

		return;
	}

	CWnd* pWnd = CWnd::FromHandle(pItem->GetHandle());
	if (pWnd)
	{
		m_pTabClientWnd->MDIActivate(pWnd);

		CWnd* pFocus = GetFocus();

		BOOL bHasFocus = pFocus->GetSafeHwnd() &&
			(pFocus == pWnd || pWnd->IsChild(pFocus) || (pFocus->GetOwner()->GetSafeHwnd() && pWnd->IsChild(pFocus->GetOwner())));

		if (!bHasFocus)
			pWnd->SetFocus();
	}
}

CWnd* CXTPTabClientWnd::CWorkspace::GetWindow() const
{
	return m_pTabClientWnd;
}

void CXTPTabClientWnd::CWorkspace::OnNavigateButtonClick(CXTPTabManagerNavigateButton* pButton)
{
	if (pButton->GetID() != xtpTabNavigateButtonClose)
		return;

	CXTPTabManagerItem* pItem = pButton->GetItem() ? pButton->GetItem() : m_pSelected;

	if (!pItem)
		return;

	m_pTabClientWnd->PostMessage(WM_IDLEUPDATECMDUI);

	HWND hWnd = pItem->GetHandle();
	::SendMessage(hWnd, WM_CLOSE, 0, 0);
}

void CXTPTabClientWnd::CWorkspace::OnRecalcLayout()
{

}

CXTPMarkupContext* CXTPTabClientWnd::CWorkspace::GetMarkupContext() const
{
	return m_pTabClientWnd->m_pMarkupContext;
}

HICON CXTPTabClientWnd::CWorkspace::GetItemIcon(const CXTPTabManagerItem* pItem) const
{
	return m_pTabClientWnd->GetItemIcon(pItem);
}

COLORREF CXTPTabClientWnd::CWorkspace::GetItemColor(const CXTPTabManagerItem* pItem) const
{
	COLORREF clr = m_pTabClientWnd->GetItemColor(pItem);

	if (clr != COLORREF_NULL)
	{
		if (clr >= xtpTabColorBlue && clr <= xtpTabColorMagenta)
			return CXTPTabPaintManager::GetOneNoteColor((XTPTabOneNoteColor)clr);

		return clr;
	}

	return CXTPTabManager::GetItemColor(pItem);
}

void CXTPTabClientWnd::CWorkspace::ReOrder(HWND hWnd, CPoint pt, CXTPTabManagerItem* pItem)
{
	if (pItem == m_pNewWindowItem)
	{
		m_pTabClientWnd->SetActiveWorkspace(this);

		m_pHighlighted = NULL;

		OnItemClick(pItem);

		return;
	}

	if (m_pTabClientWnd->GetWorkspaceCount() == 1 && pItem->GetTabManager()->GetItemCount() == 1)
	{
		OnItemClick(pItem);
		return;
	}

	if (m_pSelected != pItem)
	{
		m_pSelected = pItem;
#ifdef _XTP_ACTIVEX
		m_pTabClientWnd->FireSelectedChanged(pItem->GetIDispatch(FALSE));
#endif
	}


	m_pTabClientWnd->m_bLockReposition = TRUE;
	m_pTabClientWnd->SetActiveWorkspace(this);
	m_pTabClientWnd->m_bLockReposition = FALSE;

	if (GetPaintManager()->m_bHotTracking)
		m_pHighlighted = pItem;

	::SetCapture(hWnd);

	Reposition();

	CArray<CRect, CRect&> arrRects;

	for (int j = 0; j < GetItemCount(); j++)
	{
		CRect rc = GetItem(j)->GetRect();
		arrRects.Add(rc);
	}

	BOOL bRefreshCursor = FALSE;
	BOOL bAccept = FALSE;

	for (;;)
	{
		MSG msg;
		VERIFY(::GetMessage(&msg, NULL, 0, 0));

		if (::GetCapture() != hWnd)
		{
			DispatchMessage (&msg);
			goto ExitLoop;
		}
		switch (msg.message)
		{
			case WM_MOUSEMOVE:
				{
					pt = CPoint((short signed)LOWORD(msg.lParam), (short signed)HIWORD(msg.lParam));

					if (m_rcHeaderRect.IsRectEmpty() || m_rcHeaderRect.PtInRect(pt))
					{
						if (bRefreshCursor)
						{
							m_pTabClientWnd->CancelLoop();
							::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
							bRefreshCursor = FALSE;

						}
						for (int i = 0; i < arrRects.GetSize(); i++)
						{
							if (i != pItem->GetIndex() && arrRects[i].PtInRect(pt) && m_arrItems[i] != m_pNewWindowItem)
							{

								CXTPTabManagerItem* p = pItem;
								m_arrItems[pItem->GetIndex()] = m_arrItems[i];
								m_arrItems[i] = p;

								OnItemsChanged();
								break;
							}
						}

					}
					else if (m_pTabClientWnd->m_bEnableGroups)
					{
						if (!bRefreshCursor)
						{
							m_pTabClientWnd->InitLoop();
							bRefreshCursor = TRUE;
						}
						m_pTabClientWnd->ReorderWorkspace(pt, pItem);
					}
				}

				break;

			case WM_KEYDOWN:
				if (msg.wParam != VK_ESCAPE)
					break;
			case WM_CANCELMODE:
			case WM_RBUTTONDOWN:
				goto ExitLoop;

			case WM_LBUTTONUP:
				bAccept = TRUE;
				goto ExitLoop;


			default:
				DispatchMessage (&msg);
				break;
		}
	}

ExitLoop:

	ReleaseCapture();
	PerformMouseMove(hWnd, pt);
	OnItemClick(pItem);

	if (bRefreshCursor)
	{
		m_pTabClientWnd->CancelLoop();
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));

		if (bAccept && m_pTabClientWnd->m_nFocusedAction != XTP_TABCLIENT_ACTION_CANCEL)
		{
			if (m_pTabClientWnd->m_nFocusedAction == XTP_TABCLIENT_ACTION_POPUP)
				m_pTabClientWnd->ContextMenu(pt);
			else  m_pTabClientWnd->DoWorkspaceCommand(pItem, m_pTabClientWnd->m_pFocusWorkspace, m_pTabClientWnd->m_nFocusedAction);
		}
	}

	pItem->GetTabManager()->EnsureVisible(pItem);
}

BOOL CXTPTabClientWnd::CWorkspace::DrawParentBackground(CDC* pDC, CRect rc)
{
	if (m_pTabClientWnd->m_themeCommandBars == xtpThemeVisualStudio2008)
	{
		CXTPCommandBars* pCommandBars = m_pTabClientWnd->GetCommandBars();
		if (!pCommandBars)
			return FALSE;

		CXTPOffice2003Theme* pPaintManager = (CXTPOffice2003Theme*)pCommandBars->GetPaintManager();
		if (((CXTPPaintManager*)pPaintManager)->BaseTheme() != xtpThemeVisualStudio2008)
			return FALSE;

		CWnd* pWnd = m_pTabClientWnd->GetParentFrame();
		CXTPWindowRect rcFill((CWnd*)pWnd);

		m_pTabClientWnd->ScreenToClient(rcFill);
		rcFill.right = rcFill.left + max(rcFill.Width(), GetSystemMetrics(SM_CXFULLSCREEN) / 2);
		rcFill.top = rc.top;
		rcFill.bottom = rc.bottom;

		XTPDrawHelpers()->GradientFill(pDC, rcFill, pPaintManager->m_clrDockBar, TRUE, rc);
		return TRUE;
	}
	return FALSE;
}
