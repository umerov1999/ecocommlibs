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
#include <CommandBars/TabClient/TabClientDropTarget.h>


CXTPTabClientWnd::CTabClientDropTarget::CTabClientDropTarget()
{
	m_ptDragLastPoint = CPoint(-1, -1);
	m_dwDragLastTick = 0;
}

void CXTPTabClientWnd::CTabClientDropTarget::OnDragLeave(CWnd* /*pWnd*/)
{
	m_dwDragLastTick = 0;
	m_ptDragLastPoint = CPoint(-1, -1);
}

DROPEFFECT CXTPTabClientWnd::CTabClientDropTarget::OnDragOver(CWnd* /*pWnd*/, COleDataObject* /*pDataObject*/, DWORD /*dwKeyState*/, CPoint point)
{
	ASSERT(m_pTabClientWnd);

	if (!m_pTabClientWnd || !m_pTabClientWnd->GetPaintManager()->m_bSelectOnDragOver)
		return DROPEFFECT_NONE;

	if (m_dwDragLastTick != (DWORD)-1 && m_pTabClientWnd->GetPaintManager()->m_bSelectOnDragOver == 2)
	{
		DWORD dwTick = GetTickCount();

		if (point != m_ptDragLastPoint)
		{
			m_dwDragLastTick = dwTick;
			m_ptDragLastPoint = point;
		}

		if (dwTick - m_dwDragLastTick > CXTPTabPaintManager::m_nSelectOnDragOverDelay)
		{
			m_dwDragLastTick = (DWORD)-1;
		}
	}
	else
	{
		CXTPTabManagerItem* pItem = m_pTabClientWnd->HitTest(point);

		if (pItem)
		{
			if (!pItem->IsSelected())
			m_pTabClientWnd->MDIActivate(CWnd::FromHandle(pItem->GetHandle()));
		}
		else
		{
			m_dwDragLastTick = 0;
		}
	}

	return DROPEFFECT_NONE;
}
