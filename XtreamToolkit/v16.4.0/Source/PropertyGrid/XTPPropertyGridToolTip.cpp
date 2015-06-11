// XTPPropertyGridToolTip.cpp implementation for the CXTPPropertyGridToolTip class.
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

#include "StdAfx.h"

#include "Common/XTPDrawHelpers.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPToolTipContext.h"
#include <Common/ScrollBar/XTPScrollBase.h>
#include <Common/ScrollBar/XTPScrollBarCtrl.h>
#include <Common/ScrollBar/XTPScrollBarContainer.h>

#include "XTPPropertyGridDefines.h"
#include "XTPPropertyGridPaintManager.h"
#include "XTPPropertyGridItem.h"
#include "XTPPropertyGridItemOption.h"
#include "XTPPropertyGridToolTip.h"
#include "XTPPropertyGridView.h"
#include "XTPPropertyGridToolBar.h"
#include "XTPPropertyGrid.h"

/////////////////////////////////////////////////////////////////////////////
// CXTPPropertyGridToolTip

CXTPPropertyGridToolTip::CXTPPropertyGridToolTip()
{
	m_pGrid = NULL;
}

BEGIN_MESSAGE_MAP(CXTPPropertyGridToolTip, CWnd)
	//{{AFX_MSG_MAP(CXTPPropertyGridView)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_NCHITTEST_EX()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

LRESULT CXTPPropertyGridToolTip::OnNcHitTest(CPoint /*point*/)
{
	return (LRESULT)HTTRANSPARENT;
}
void CXTPPropertyGridToolTip::Create(CXTPPropertyGridView* pParentWnd)
{
	CWnd::CreateEx(0, AfxRegisterWndClass(0, AfxGetApp()->LoadStandardCursor(IDC_ARROW)), _T(""), WS_POPUP, CXTPEmptyRect(), pParentWnd, 0);
	m_pGrid = pParentWnd;

}

void CXTPPropertyGridToolTip::Activate(BOOL bActive, CXTPPropertyGridItem* pItem, BOOL bValuePart)
{
	if (bActive)
	{
		ASSERT(pItem);
		m_fnt.DeleteObject();
		LOGFONT lf;
		m_pGrid->GetPaintManager()->GetItemFont(pItem, bValuePart)->GetLogFont(&lf);

		m_fnt.CreateFontIndirect(&lf);

		CString strText;
		GetWindowText(strText);
		CWindowDC dc(this);
		CXTPFontDC font(&dc, &m_fnt);
		CXTPWindowRect rc (this);
		rc.right = rc.left + dc.GetTextExtent(strText).cx + 8;

		CRect rcWork = XTPMultiMonitor()->GetWorkArea();

		if (rc.right > rcWork.right)
			rc.OffsetRect(rcWork.right - rc.right, 0);

		if (rc.left < rcWork.left)
			rc.OffsetRect(rcWork.left - rc.left, 0);

		MoveWindow(rc);
		ShowWindow(SW_SHOWNOACTIVATE);
	}
	else
	{
		DestroyWindow();
	}
}

BOOL CXTPPropertyGridToolTip::OnEraseBkgnd(CDC*)
{
	return TRUE;
}

void CXTPPropertyGridToolTip::OnPaint()
{
	CPaintDC dc(this);
	CXTPClientRect rc(this);

	dc.SetBkMode(TRANSPARENT);

	if (m_pGrid->GetPropertyGrid()->GetToolTipContext())
	{
		m_pGrid->GetPropertyGrid()->GetToolTipContext()->DrawBackground(&dc, rc);
	}
	else
	{
		COLORREF clrText = GetXtremeColor(COLOR_INFOTEXT);
		dc.FillSolidRect(rc, GetXtremeColor(COLOR_INFOBK));

		dc.Draw3dRect(rc, clrText, clrText);
		dc.SetTextColor(clrText);
	}

	CString strText;
	GetWindowText(strText);

	CXTPFontDC font(&dc, &m_fnt);
	CRect rcText(rc);
	rcText.left += 4;

	dc.DrawText(strText, rcText, DT_SINGLELINE | DT_VCENTER | DT_NOPREFIX);

}