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

#include <CommandBars/Resource.h>
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
#include <CommandBars/TabClient/NavigateButtonActiveFiles.h>


CXTPTabClientWnd::CNavigateButtonActiveFiles::CNavigateButtonActiveFiles(CXTPTabManager* pManager, CXTPTabClientWnd* pTabClientWnd)
	: CXTPTabManagerNavigateButton(pManager, xtpTabNavigateButtonActiveFiles, xtpTabNavigateButtonNone)
{
	m_bHiddenTabs = FALSE;
	m_pTabClientWnd = pTabClientWnd;
	XTPResourceManager()->LoadString(&m_strToolTip, XTP_IDS_TABNAVIGATEBUTTON_ACTIVEFILES);
}

void CXTPTabClientWnd::CNavigateButtonActiveFiles::DrawEntry(CDC* pDC, CRect rc)
{
	CPoint pt(rc.CenterPoint().x, rc.CenterPoint().y + 1);
	CPoint pts[] =
	{
		CPoint(pt.x - 4, pt.y - 1), CPoint(pt.x - 1, pt.y + 2) , CPoint(pt.x, pt.y + 2), CPoint(pt.x + 3, pt.y - 1)
	};
	pDC->Polygon(pts, 4);

	if (m_bHiddenTabs)
	{
		pDC->Rectangle(pt.x - 4, pt.y - 5, pt.x + 4, pt.y - 3);
	}
}

void CXTPTabClientWnd::CNavigateButtonActiveFiles::Reposition(CRect& rcNavigateButtons)
{
	CRect rc = m_pManager->GetAppearanceSet()->GetHeaderMargin();

	m_bHiddenTabs = m_pManager->GetItemsLength() > m_pManager->GetRectLength(rcNavigateButtons) - rc.left - rc.right - GetSize().cx;

	CXTPTabManagerNavigateButton::Reposition(rcNavigateButtons);
}
void CXTPTabClientWnd::CNavigateButtonActiveFiles::PerformClick(HWND hWnd, CPoint /*pt*/)
{
	CXTPCommandBars* pCommandBars = m_pTabClientWnd->GetCommandBars();
	if (!pCommandBars || pCommandBars->IsCustomizeMode())
		return;

	m_bPressed = TRUE;
	m_pManager->RedrawControl(NULL, FALSE);

	CPoint pt(m_rcButton.left, m_rcButton.bottom);
	m_pTabClientWnd->WorkspaceToScreen(&pt);
	CRect rcExclude(pt.x, pt.y - m_rcButton.Height(), pt.x + m_rcButton.Width(), pt.y);

	CXTPPopupBar* pPopupBar = CXTPPopupBar::CreatePopupBar(pCommandBars);

	CXTPImageManager* pImageManager = new CXTPImageManager();
	pPopupBar->SetImageManager(pImageManager);

	int nIconId = 0;
	HICON hIconLast = 0;

	//CWnd* pActiveWnd = m_pTabClientWnd->MDIGetActive();

	for (int nIndex = 0; nIndex < m_pManager->GetItemCount(); nIndex++)
	{
		CXTPTabManagerItem* pItem = m_pManager->GetItem(nIndex);
		if (!pItem->GetHandle())
			continue;

		CString strCaption = pItem->GetCaption();
		CXTPControl* pControl = pPopupBar->GetControls()->Add(xtpControlButton, nIndex + 1, _T(""), nIndex, TRUE);

		HICON hIcon = m_pTabClientWnd->GetItemIcon(pItem);
		if (hIconLast != hIcon)
		{
			nIconId++;
			pImageManager->SetIcon(CXTPImageManagerIconHandle(hIcon, FALSE), nIconId);
			hIconLast = hIcon;
		}

		pControl->SetIconId(nIconId);
		pControl->SetCaption(CXTPControlWindowList::ConstructCaption(strCaption, 0));
		pControl->SetDescription(_T(""));

		//if (pActiveWnd && pItem->GetHandle() == pActiveWnd->GetSafeHwnd())
		//  pControl->SetChecked(TRUE);
	}

	int nItem = pCommandBars->TrackPopupMenu(pPopupBar, TPM_RIGHTBUTTON | TPM_RETURNCMD | TPM_NONOTIFY, pt.x, pt.y, m_pTabClientWnd, &rcExclude);
	pPopupBar->InternalRelease();

	m_bPressed = FALSE;
	m_pManager->PerformMouseMove(hWnd, CPoint(-1, -1));
	m_pManager->RedrawControl(NULL, FALSE);

	if (nItem > 0)
	{
		CXTPTabManagerItem* pItem = m_pManager->GetItem(nItem - 1);

		if (pItem)
		{
			m_pTabClientWnd->MDIActivate(CWnd::FromHandle(pItem->GetHandle()));
		}
	}
}
