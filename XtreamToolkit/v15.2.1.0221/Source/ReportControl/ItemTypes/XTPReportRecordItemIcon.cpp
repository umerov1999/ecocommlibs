// XTPReportRecordItemIcon.cpp : implementation of the CXTPReportRecordItemIcon class.
//
// This file is a part of the XTREME REPORTCONTROL MFC class library.
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
#include "Common/XTPPropExchange.h"
#include "Common/XTPMarkupRender.h"
#include "Common/XTPCustomHeap.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPSmartPtrInternalT.h"
#include "Common/XTPColorManager.h"

#include "ReportControl/XTPReportDefines.h"
#include "ReportControl/XTPReportRecordItem.h"
#include "ReportControl/XTPReportControl.h"
#include "ReportControl/XTPReportPaintManager.h"
#include "ReportControl/XTPReportRow.h"
#include "ReportControl/XTPReportRecord.h"
#include "ReportControl/XTPReportHyperlink.h"
#include "XTPReportRecordItemIcon.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_SERIAL(CXTPReportRecordItemIcon, CXTPReportRecordItem, VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)

CXTPReportRecordItemIcon::CXTPReportRecordItemIcon() : CXTPReportRecordItem()
{
	SetAlignment(xtpColumnIconTop | xtpColumnIconCenter | xtpColumnTextWordBreak | xtpColumnTextCenter);
}

int CXTPReportRecordItemIcon::Draw(XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs)
{
	if (pDrawArgs == NULL)
		return CXTPReportRecordItem::Draw(pDrawArgs);

	CXTPReportControl* pControl = DYNAMIC_DOWNCAST(CXTPReportControl, pDrawArgs->pControl);
	if (pControl && pControl->IsIconView())
	{
		pDrawArgs->rcItem = pDrawArgs->pRow->GetRect();
		pDrawArgs->rcItem.DeflateRect(1, 1);
	}

	return CXTPReportRecordItem::Draw(pDrawArgs);
}

void CXTPReportRecordItemIcon::OnDrawCaption(XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs, XTP_REPORTRECORDITEM_METRICS* pMetrics)
{
	if (pDrawArgs == NULL)
		return;

	CXTPReportControl* pControl = DYNAMIC_DOWNCAST(CXTPReportControl, pDrawArgs->pControl);
	if (pControl && pControl->IsIconView())
	{
		// Adjust the rect for the text.
		pDrawArgs->rcItem.DeflateRect(1, 1);
		pDrawArgs->rcItem.top += pControl->m_iIconHeight;

		CXTPReportPaintManager * pPaintManager = pControl->GetPaintManager();

		const UINT c_uiDrawFlags = pDrawArgs->nTextAlign | DT_CENTER | DT_NOPREFIX | DT_WORDBREAK | DT_EDITCONTROL | DT_END_ELLIPSIS | DT_WORD_ELLIPSIS;

		// Get rect for text
		CRect rcItem(pDrawArgs->rcItem);
		rcItem.DeflateRect(2, 1, 2, 0);
		rcItem = pPaintManager->CalculateMaxTextRect(pDrawArgs->pDC, pMetrics->strText, rcItem, TRUE, TRUE, c_uiDrawFlags);

		int nWidth = min(rcItem.Width() + 4, pDrawArgs->rcItem.Width());

		// Center the rect.
		int nLeft = (pDrawArgs->rcItem.left + pDrawArgs->rcItem.right - nWidth) / 2;

		rcItem.left = nLeft;
		rcItem.right = rcItem.left + nWidth;

		if (rcItem.bottom >= pDrawArgs->rcItem.bottom)
		  rcItem.bottom -= 2;

		pDrawArgs->rcItem.left = rcItem.left;
		pDrawArgs->rcItem.right = rcItem.right;

		//Draw the selection
		if (pDrawArgs->pRow->IsSelected() && (pDrawArgs->pDC && !pDrawArgs->pDC->IsPrinting()))
		{
			COLORREF clrBackground = 0;

			if (pControl->HasFocus())
			{
				pMetrics->clrForeground = pPaintManager->m_clrHighlightText;
				pDrawArgs->pDC->SetTextColor(pMetrics->clrForeground);
				clrBackground = pPaintManager->m_clrHighlight;
			}
			else if (!pPaintManager->m_bHideSelection)
			{
				pMetrics->clrForeground = pPaintManager->m_clrSelectedRowText;
				pDrawArgs->pDC->SetTextColor(pMetrics->clrForeground);
				clrBackground = pPaintManager->m_clrSelectedRow;
			}

			// fill select rect
			pDrawArgs->pDC->FillSolidRect(rcItem, clrBackground);
		}

		if (pDrawArgs->pRow->IsFocused() && pControl->HasFocus() && pControl->IsRowFocusVisible())
		{
			rcItem.InflateRect(1, 1);
			pPaintManager->DrawFocusedRow(pDrawArgs->pDC, rcItem);
		}

		pDrawArgs->nTextAlign |= c_uiDrawFlags;
	}

	CXTPReportRecordItem::OnDrawCaption(pDrawArgs, pMetrics);
}

void CXTPReportRecordItemIcon::GetCaptionRect(XTP_REPORTRECORDITEM_ARGS* pDrawArgs, CRect& rcItem)
{
	if (pDrawArgs == NULL)
		return;

	CXTPReportControl* pControl = DYNAMIC_DOWNCAST(CXTPReportControl, pDrawArgs->pControl);
	if (pControl && pControl->IsIconView())
	{
		// Adjust the rect for the text.
		pDrawArgs->rcItem.CopyRect(pDrawArgs->pRow->GetRect());
		pDrawArgs->rcItem.DeflateRect(1, 1);
		pDrawArgs->rcItem.top += pControl->m_iIconHeight;

		// The left coming in here is to the right of the icon, we need to get back to the beginning.
		pDrawArgs->rcItem.left = pDrawArgs->pRow->GetRect().left + 5;
		pDrawArgs->rcItem.DeflateRect(2, 1, 2, 0);

		rcItem = pDrawArgs->rcItem;
		return;
	}
	else
	{
		CXTPReportRecordItem::GetCaptionRect(pDrawArgs, rcItem);
	}
}

void CXTPReportRecordItemIcon::OnEditChanged(XTP_REPORTRECORDITEM_ARGS* pItemArgs, LPCTSTR szText)
{
	if (pItemArgs == NULL)
		return;

	CXTPReportRecordItem::OnEditChanged(pItemArgs, szText);

	int r = pItemArgs->pRow->GetIndex();
	int i = pItemArgs->pItem->GetIndex();
	CXTPReportRecord* pRec = pItemArgs->pRow->GetRecord();
	if (pRec)
	{
		pRec->UpdateRecordField(r, i, szText);
	}
}
