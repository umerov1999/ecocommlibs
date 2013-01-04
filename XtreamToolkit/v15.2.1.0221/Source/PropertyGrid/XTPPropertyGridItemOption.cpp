// XTPPropertyGridItemOption.cpp : implementation of the CXTPPropertyGridItemOption class.
//
// This file is a part of the XTREME PROPERTYGRID MFC class library.
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

#include "StdAfx.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPColorManager.h"

#include "XTPPropertyGridDefines.h"
#include "XTPPropertyGridPaintManager.h"
#include "XTPPropertyGridItem.h"
#include "XTPPropertyGridItemOption.h"
#include "XTPPropertyGridView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CXTPPropertyGridItemColor

IMPLEMENT_DYNAMIC(CXTPPropertyGridItemOption, CXTPPropertyGridItem)

CXTPPropertyGridItemOption::CXTPPropertyGridItemOption(LPCTSTR strCaption, int nValue, int* pBindOption)
	: CXTPPropertyGridItem(strCaption)
{
	m_pBindOption = pBindOption;
	_Init(nValue);
}

CXTPPropertyGridItemOption::CXTPPropertyGridItemOption(UINT nID, int nValue, int* pBindOption)
	: CXTPPropertyGridItem(nID)
{
	m_pBindOption = pBindOption;
	_Init(nValue);
}

CXTPPropertyGridItemOption::~CXTPPropertyGridItemOption()
{
}

void CXTPPropertyGridItemOption::_Init(int nValue)
{
	SetOption(nValue);
	m_nFlags = 0;
	m_bCheckBoxStyle = FALSE;

	m_strDefaultValue = m_strValue;
}

void CXTPPropertyGridItemOption::SetValue(CString strValue)
{
	int nValue = _ttoi(strValue);
	SetOption(nValue);
}

void CXTPPropertyGridItemOption::SetOption(int nValue)
{
	m_nValue = nValue;

	if (m_pBindOption)
	{
		*m_pBindOption = nValue;
	}

	CString strValue;
	strValue.Format(_T("%i"), m_nValue);

	CXTPPropertyGridItem::SetValue(strValue);
}

void CXTPPropertyGridItemOption::BindToOption(int* pBindOption)
{
	m_pBindOption = pBindOption;
	if (m_pBindOption)
	{
		*m_pBindOption = m_nValue;
	}
}

void CXTPPropertyGridItemOption::OnBeforeInsert()
{
	if (m_pBindOption && *m_pBindOption != m_nValue)
	{
		SetOption(*m_pBindOption);
	}
}

void CXTPPropertyGridItemOption::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	if (m_nHeight != -1)
	{
		lpMeasureItemStruct->itemHeight = m_nHeight;
	}

	int nLinesCount = max(1, m_pConstraints->GetCount());
	lpMeasureItemStruct->itemHeight = (lpMeasureItemStruct->itemHeight - 3) * nLinesCount + 4;
}

BOOL CXTPPropertyGridItemOption::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (!CXTPPropertyGridItem::OnLButtonDown(nFlags, point))
		return FALSE;

	if (!IsAllowEdit())
		return TRUE;

	CRect rcValue(GetValueRect());
	rcValue.DeflateRect(0, 2, 0, 1);
	if (!rcValue.PtInRect(point))
		return TRUE;

	int nCount = m_pConstraints->GetCount();
	if (nCount < 1)
		return TRUE;

	int nHeight = (rcValue.Height() + 1) / nCount;


	int nIndex = (point.y - rcValue.top) / nHeight;

	ToggleConstraint(nIndex);

	return TRUE;
}

void CXTPPropertyGridItemOption::ToggleConstraint(int nIndex)
{
	int nCount = m_pConstraints->GetCount();
	if (nCount < 1)
		return;

	if (nIndex >= nCount) nIndex = nCount - 1;
	if (nIndex < 0) nIndex = 0;

	if (!m_pConstraints->GetConstraintAt(nIndex)->m_bEnabled)
		return;

	if (!OnRequestEdit())
		return;

	int nValue = m_nValue;

	int dwData = (int)m_pConstraints->GetConstraintAt(nIndex)->m_dwData;

	if (m_bCheckBoxStyle)
	{
		if (nValue & dwData) nValue &= ~dwData; else nValue |= dwData;
	}
	else
	{
		nValue = dwData;
	}
	CString strValue;
	strValue.Format(_T("%i"), nValue);

	if (!OnAfterEdit(strValue))
		return;

	m_pConstraints->SetCurrent(nIndex);
	OnValueChanged(strValue);
}

BOOL CXTPPropertyGridItemOption::OnDrawItemValue(CDC& dc, CRect rcValue)
{
	int nCount = m_pConstraints->GetCount();
	if (nCount < 1)
		return TRUE;

	rcValue.DeflateRect(0, 2, 0, 1);

	int nHeight = rcValue.Height() / nCount;
	BOOL bEnabled = !GetReadOnly();
	CXTPPropertyGridPaintManager* pPaintManager = m_pGrid->GetPaintManager();

	CXTPFontDC font(&dc, pPaintManager->GetItemFont(this, TRUE));

	for (int i = 0; i < nCount; i++)
	{
		CRect rcItem(rcValue.left, rcValue.top + i * nHeight, rcValue.right, rcValue.top + (i + 1) * nHeight);
		CXTPPropertyGridItemConstraint* pConstraint = m_pConstraints->GetConstraintAt(i);

		CRect rcSample(rcItem.left - 1, rcItem.CenterPoint().y - 7, rcItem.left + 12, rcItem.CenterPoint().y + 6);

		if (m_bCheckBoxStyle)
		{
			pPaintManager->DrawCheckMark(&dc, rcSample, bEnabled && pConstraint->m_bEnabled, m_nValue & (int)pConstraint->m_dwData);
		}
		else
		{
			pPaintManager->DrawRadioMark(&dc, rcSample, bEnabled && pConstraint->m_bEnabled, m_nValue == (int)pConstraint->m_dwData);
		}

		CRect rcText(rcItem.left + 14, rcItem.top, rcItem.right, rcItem.bottom);
		COLORREF clr = pPaintManager->GetItemTextColor(this, TRUE);
		if (!pConstraint->m_bEnabled)
			clr = pPaintManager->GetItemMetrics()->m_clrReadOnlyFore;
		dc.SetTextColor(clr);

		dc.DrawText(pConstraint->m_strConstraint, rcText, DT_SINGLELINE | DT_NOPREFIX);

		if (m_pConstraints->GetCurrent() == i && IsSelected() && ::GetFocus() == m_pGrid->GetSafeHwnd())
		{
			rcText.left--;
			rcText.right = rcText.left + dc.GetTextExtent(pConstraint->m_strConstraint).cx + 2;
			dc.DrawFocusRect(rcText);
		}
	}

	return TRUE;
}

void CXTPPropertyGridItemOption::OnDeselect()
{
	CXTPPropertyGridItem::OnDeselect();

	if (m_pConstraints->GetCurrent() != -1)
	{
		m_pConstraints->SetCurrent(-1);
		RedrawItem();
	}
}

BOOL CXTPPropertyGridItemOption::OnInpaceControlFocus(BOOL bForward)
{
	int nCount = m_pConstraints->GetCount();
	if (nCount < 1 || !IsAllowEdit())
		return FALSE;

	int nCurrent = m_pConstraints->GetCurrent();

	if (nCurrent == -1)
	{
		nCurrent = bForward ? 0 : nCount - 1;
		m_pConstraints->SetCurrent(nCurrent);
		RedrawItem();
		return TRUE;
	}
	else if ((bForward && nCurrent < nCount - 1) || (!bForward && nCurrent > 0))
	{
		nCurrent += bForward ? +1 : -1;
		m_pConstraints->SetCurrent(nCurrent);
		RedrawItem();
		return TRUE;
	}

	return FALSE;
}

BOOL CXTPPropertyGridItemOption::OnKeyDown(UINT nChar)
{
	if (IsAllowEdit() && m_pConstraints->GetCurrent() != -1 && ((nChar == VK_DOWN) || (nChar == VK_UP)))
	{
		int nCurrent = m_pConstraints->GetCurrent();
		int nCount = m_pConstraints->GetCount();
		BOOL bForward = (nChar == VK_DOWN);

		if ((bForward && nCurrent < nCount - 1) || (!bForward && nCurrent > 0))
		{
			nCurrent += bForward ? +1 : -1;
			m_pConstraints->SetCurrent(nCurrent);
			RedrawItem();
			return TRUE;
		}
	}

	if (IsAllowEdit() && m_pConstraints->GetCurrent() != -1 && ((nChar == VK_SPACE) || (nChar == VK_RETURN)))
	{
		ToggleConstraint(m_pConstraints->GetCurrent());
		return TRUE;
	}

	return FALSE;
}

void CXTPPropertyGridItemOption::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	OnLButtonDown(nFlags, point);
}

