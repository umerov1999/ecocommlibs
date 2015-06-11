// XTPPropertyGridToolBar.cpp : implementation of the CXTPPropertyGridToolBar class.
//
// This file is a part of the XTREME PROPERTYGRID MFC class library.
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
#include "Common/Resource.h"

#include "Common/XTPResourceManager.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPImageManager.h"
#include "Common/XTPToolTipContext.h"
#include "Common/XTPMarkupRender.h"
#include "Common/XTPMaskEditT.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPColorManager.h"
#include <Common/ScrollBar/XTPScrollBase.h>
#include <Common/ScrollBar/XTPScrollBarCtrl.h>
#include <Common/ScrollBar/XTPScrollBarContainer.h>

#include "XTPPropertyGridDefines.h"
#include "XTPPropertyGridInplaceEdit.h"
#include "XTPPropertyGridInplaceButton.h"
#include "XTPPropertyGridInplaceList.h"
#include "XTPPropertyGridItem.h"
#include "XTPPropertyGridPaintManager.h"
#include "XTPPropertyGridToolTip.h"
#include "XTPPropertyGridToolBar.h"
#include "XTPPropertyGrid.h"
#include "XTPPropertyGridView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CXTPPropertyGridToolBar

CXTPPropertyGridToolBar::CXTPPropertyGridToolBar()
{
	m_cxLeftBorder = m_cxRightBorder = m_cyBottomBorder = m_cyTopBorder = 0;
}

CXTPPropertyGridToolBar::~CXTPPropertyGridToolBar()
{
}

BEGIN_MESSAGE_MAP(CXTPPropertyGridToolBar, CToolBar)
	//{{AFX_MSG_MAP(CToolBar)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_NCCALCSIZE()
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomDraw)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CXTPPropertyGridToolBar::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMLVCUSTOMDRAW* lpLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>(pNMHDR);
	*pResult = CDRF_DODEFAULT;

	switch (lpLVCD->nmcd.dwDrawStage)
	{
	case CDDS_PREPAINT:
		CWnd* pGrid = GetParent();
		HBRUSH hBrush = (HBRUSH)pGrid->GetParent()->SendMessage(WM_CTLCOLORSTATIC,
			(WPARAM)lpLVCD->nmcd.hdc, (LPARAM)pGrid->m_hWnd);

		::FillRect(lpLVCD->nmcd.hdc, &lpLVCD->nmcd.rc, hBrush ? hBrush : GetSysColorBrush(COLOR_3DFACE));
		break;
	}
}


BOOL CXTPPropertyGridToolBar::OnEraseBkgnd(CDC*)
{
	return TRUE;
}

void CXTPPropertyGridToolBar::OnNcCalcSize(BOOL /*bCalcValidRects*/, NCCALCSIZE_PARAMS* /*lpncsp*/)
{
}

void CXTPPropertyGridToolBar::OnPaint()
{
	CPaintDC dc(this);

	CXTPClientRect rc(this);
	CXTPBufferDC memDC(dc, rc);
	memDC.FillSolidRect(rc, GetXtremeColor(COLOR_3DFACE));

	CToolBar::DefWindowProc(WM_PAINT, (WPARAM)memDC.m_hDC, 0);
}

void CXTPPropertyGridToolBar::OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL /*bDisableIfNoHndler*/)
{
}

BOOL CXTPPropertyGridToolBar::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	CXTPPropertyGrid* pGrid = (CXTPPropertyGrid*)GetParent();
	if (pGrid->GetToolTipContext())
	{
		pGrid->GetToolTipContext()->FilterToolTipMessage(this, message, wParam, lParam);
	}

	return CWnd::OnWndMsg(message, wParam, lParam, pResult);
}
