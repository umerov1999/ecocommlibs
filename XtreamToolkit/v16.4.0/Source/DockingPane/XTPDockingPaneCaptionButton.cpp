// XTPDockingPanePaintManager.cpp : implementation of the CXTPDockingPanePaintManager class.
//
// This file is a part of the XTREME DOCKINGPANE MFC class library.
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
#include "Resource.h"

#include "Common/XTPVC80Helpers.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPResourceImage.h"
#include "Common/XTPResourceManager.h"
#include "Common/XTPWinThemeWrapper.h"
#include "Common/XTPImageManager.h"
#include "Common/XTPSystemHelpers.h"

#include <TabManager/Includes.h>

#include "XTPDockingPaneDefines.h"
#include "XTPDockingPaneBase.h"
#include "XTPDockingPaneBaseContainer.h"
#include "XTPDockingPanePaintManager.h"
#include "XTPDockingPaneTabbedContainer.h"
#include "XTPDockingPane.h"
#include "XTPDockingPaneMiniWnd.h"
#include "XTPDockingPaneSplitterContainer.h"
#include "XTPDockingPaneSidePanel.h"
#include <DockingPane/XTPDockingPaneCaptionButton.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


BOOL CXTPDockingPaneCaptionButton::IsVisible()
{
	return m_pPane->IsCaptionButtonVisible(this);
}

void AFX_CDECL CXTPDockingPaneCaptionButton::DrawPinnButton(CDC* pDC, CPoint pt, BOOL bPinned)
{
	pt.y--;

	if (bPinned)
	{
		pDC->MoveTo(pt.x - 4, pt.y); pDC->LineTo(pt.x - 1, pt.y);
		pDC->MoveTo(pt.x - 1, pt.y - 3); pDC->LineTo(pt.x - 1, pt.y + 4);
		pDC->MoveTo(pt.x - 1, pt.y + 2); pDC->LineTo(pt.x + 3, pt.y + 2);
		pDC->LineTo(pt.x + 3, pt.y - 2); pDC->LineTo(pt.x - 1, pt.y - 2);
		pDC->MoveTo(pt.x - 1, pt.y + 1); pDC->LineTo(pt.x + 3, pt.y + 1);
	}
	else
	{
		pt.y += 2;
		pDC->MoveTo(pt.x - 3, pt.y - 1); pDC->LineTo(pt.x + 4, pt.y - 1);
		pDC->MoveTo(pt.x, pt.y - 1); pDC->LineTo(pt.x, pt.y + 3);
		pDC->MoveTo(pt.x - 2, pt.y - 1); pDC->LineTo(pt.x - 2, pt.y - 6);
		pDC->LineTo(pt.x + 2, pt.y - 6); pDC->LineTo(pt.x + 2, pt.y - 1);
		pDC->MoveTo(pt.x + 1, pt.y - 1); pDC->LineTo(pt.x + 1, pt.y - 6);
	}
}

void AFX_CDECL CXTPDockingPaneCaptionButton::DrawMaximizeRestoreButton(CDC* pDC, CPoint pt, BOOL bMaximize, COLORREF clr)
{
	if (bMaximize)
	{
		pDC->Draw3dRect(pt.x - 5, pt.y - 5, 9, 9, clr, clr);
		pDC->FillSolidRect(pt.x - 5, pt.y - 5, 9, 2, clr);
	}
	else
	{
		pDC->Draw3dRect(pt.x - 4, pt.y - 2, 6, 6, clr, clr);
		pDC->FillSolidRect(pt.x - 4, pt.y - 2, 6, 2, clr);

		pDC->FillSolidRect(pt.x - 2, pt.y - 5, 6, 2, clr);
		pDC->SetPixel(pt.x - 2, pt.y - 3, clr);
		pDC->FillSolidRect(pt.x + 3, pt.y - 3, 1, 4, clr);
		pDC->SetPixel(pt.x + 2, pt.y, clr);
	}
}

void CXTPDockingPaneCaptionButton::InvalidateRect()
{
	m_pPane->RedrawPane();
}

BOOL CXTPDockingPaneCaptionButton::IsEnabled() const
{
	CXTPDockingPane* pPane = CXTPDockingPanePaintManager::GetSelectedPane(GetPane());
	if (pPane && ((pPane->GetEnabled() & xtpPaneEnableActions) == 0))
		return FALSE;

	return TRUE;
}

DWORD CXTPDockingPaneCaptionButton::Click (CWnd* pWnd, CPoint pt, BOOL bClient)
{
	if ((::GetCapture() != NULL))
		return FALSE;

	pWnd->SetCapture();
	ASSERT(pWnd == CWnd::GetCapture());

	BOOL  bClick = FALSE;
	CRect rcBtn(m_rcButton);

	while (CWnd::GetCapture() == pWnd)
	{

		BOOL bNewDown = ::PtInRect (rcBtn, pt) != 0;
		if (bNewDown != m_bPressed)
		{
			m_bPressed = bNewDown;

			InvalidateRect();
		}

		MSG msg;
		VERIFY(::GetMessage(&msg, NULL, 0, 0));

		switch (msg.message)
		{
		case WM_NCMOUSEMOVE:
			break;
		case WM_MOUSEMOVE:
			pt = CPoint((short signed)LOWORD(msg.lParam), (short signed)HIWORD(msg.lParam));
			if (!bClient)
			{
				pWnd->ClientToScreen (&pt);
				CXTPDrawHelpers::ScreenToWindow(pWnd, &pt);
			}
			break;

		case WM_LBUTTONUP:
			bClick = m_bPressed;
			goto ExitLoop;

		case WM_KEYDOWN:
			if (msg.wParam != VK_ESCAPE)
				break;
		case WM_CANCELMODE:
		case WM_RBUTTONDOWN:
			goto ExitLoop;

		default:
			DispatchMessage (&msg);
			break;
		}
	}
ExitLoop:
	ReleaseCapture();

	m_bPressed = FALSE;
	CheckForMouseOver (pt);

	InvalidateRect();

	return bClick;
}

void CXTPDockingPaneCaptionButton::SetRect (CRect rc)
{
	ASSERT(m_pPane);
	if (!m_pPane)
		return;

	if (rc != m_rcButton)
	{
		m_rcButton = rc;
		m_pPane->OnCaptionButtonRectChanged(this);
	}
}

BOOL CXTPDockingPaneCaptionButton::CheckForMouseOver (CPoint pt)
{
	if (!PtInRect (pt) && m_bSelected)
	{
		m_bSelected = FALSE;
		InvalidateRect();
	}

	if (!m_bSelected && PtInRect (pt) && IsEnabled())
	{
		TRACKMOUSEEVENT tme =
		{
			sizeof(TRACKMOUSEEVENT), TME_LEAVE, m_pPane->GetPaneHwnd()
		};
		_TrackMouseEvent(&tme);

		m_bSelected = TRUE;

		InvalidateRect();
		return TRUE;
	}
	return FALSE;
}
