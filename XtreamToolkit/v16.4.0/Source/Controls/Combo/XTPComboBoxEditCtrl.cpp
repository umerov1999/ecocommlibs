// XTPComboBoxEditCtrl.cpp : implementation of the CXTPComboBoxEditCtrl class.
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

#include "XTPComboBoxEditCtrl.h"
#include "../Util/XTPControlTheme.h"
#include "XTPComboBox.h"

BEGIN_MESSAGE_MAP(CXTPComboBoxEditCtrl, CEdit)
	//{{AFX_MSG_MAP(CComboBoxCtrl)
	ON_WM_KILLFOCUS()
	ON_WM_SETFOCUS()
	ON_MESSAGE_VOID(WM_MOUSELEAVE, OnMouseLeave)
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CXTPComboBoxEditCtrl::OnMouseLeave()
{
	CXTPComboBox* pControl = (CXTPComboBox*)GetParent();

	if (pControl->m_bHighlighted)
	{
		TRACKMOUSEEVENT tme = {sizeof(TRACKMOUSEEVENT), TME_LEAVE, pControl->m_hWnd, HOVER_DEFAULT};
		_TrackMouseEvent(&tme);
	}
}

void CXTPComboBoxEditCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	CRect rc;
	GetClientRect(&rc);

	CXTPComboBox* pControl = (CXTPComboBox*)GetParent();

	BOOL bHot = rc.PtInRect(point) && !pControl->m_bFocused;

	if (bHot != pControl->m_bHighlighted)
	{
		pControl->m_bHighlighted = bHot;
		pControl->RedrawFocusedFrame();

		if (bHot)
		{
			TRACKMOUSEEVENT tme = {sizeof(TRACKMOUSEEVENT), TME_LEAVE, m_hWnd, HOVER_DEFAULT};
			_TrackMouseEvent(&tme);
		}
	}

	CEdit::OnMouseMove(nFlags, point);
}

void CXTPComboBoxEditCtrl::OnKillFocus(CWnd* pNewWnd)
{
	CEdit::OnKillFocus(pNewWnd);

	CXTPComboBox* pControl = (CXTPComboBox*)GetParent();
	if (pNewWnd != GetParent())
	{
		pControl->m_bFocused = FALSE;
		pControl->RefreshMetrics();
		pControl->RedrawFocusedFrame();
	}
}

void CXTPComboBoxEditCtrl::OnSetFocus(CWnd* pOldWnd)
{
	CEdit::OnSetFocus(pOldWnd);

	CXTPComboBox* pControl = (CXTPComboBox*)GetParent();

	pControl->m_bFocused = TRUE;
	pControl->m_bHighlighted = FALSE;

	pControl->RedrawFocusedFrame();
}