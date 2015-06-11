// XTPReportRecordItemPreview.cpp : implementation of the CXTPReportRecordItemPreview class.
//
// This file is a part of the XTREME REPORTCONTROL MFC class library.
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
#include "XTPReportRecordItemPreview.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_SERIAL(CXTPReportRecordItemPreview, CXTPReportRecordItem, VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)

CXTPReportRecordItemPreview::CXTPReportRecordItemPreview(LPCTSTR szPreviewText)
	: CXTPReportRecordItem()
{
	SetPreviewText(szPreviewText);
}

void CXTPReportRecordItemPreview::SetPreviewText(LPCTSTR strPreviewText)
{
	m_strPreviewText = strPreviewText;

	XTPMarkupReleaseElement(m_pMarkupUIElement);

	CXTPMarkupContext* pMarkupContext = m_pRecord ? m_pRecord->GetMarkupContext() : NULL;

	if (pMarkupContext)
	{
		m_pMarkupUIElement = XTPMarkupParseText(pMarkupContext, m_strPreviewText);
	}
}

void CXTPReportRecordItemPreview::SetCaption(LPCTSTR strCaption)
{
	SetPreviewText(strCaption);
}

CString CXTPReportRecordItemPreview::GetCaption(CXTPReportColumn* /*pColumn*/)
{
	return GetPreviewText();
}

void CXTPReportRecordItemPreview::GetItemMetrics(
	XTP_REPORTRECORDITEM_DRAWARGS *pDrawArgs,
	XTP_REPORTRECORDITEM_METRICS  *pItemMetrics)
{
	pItemMetrics->clrForeground = pDrawArgs->pControl->GetPaintManager()->m_clrPreviewText;

	if (m_clrText != XTP_REPORT_COLOR_DEFAULT)
		pItemMetrics->clrForeground = m_clrText;

	pItemMetrics->pFont = &pDrawArgs->pControl->GetPaintManager()->m_fontPreview;
}

void CXTPReportRecordItemPreview::OnEditChanged(XTP_REPORTRECORDITEM_ARGS* /*pItemArgs*/, LPCTSTR szText)
{
	m_strPreviewText = szText;
}

void CXTPReportRecordItemPreview::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPReportRecordItem::DoPropExchange(pPX);

	PX_String(pPX, _T("PreviewText"), m_strPreviewText);

	if (pPX->IsLoading())
	{
		XTPMarkupReleaseElement(m_pMarkupUIElement);

		CXTPMarkupContext* pMarkupContext = m_pRecord ? m_pRecord->GetMarkupContext() : NULL;
		if (pMarkupContext)
		{
			m_pMarkupUIElement = XTPMarkupParseText(pMarkupContext, m_strPreviewText);
		}
	}
}

int CXTPReportRecordItemPreview::GetPreviewHeight(CDC* pDC, CXTPReportRow* pRow, int nWidth)
{
	int nHeight = 0;
	XTP_REPORTRECORDITEM_METRICS* pMetrics = new XTP_REPORTRECORDITEM_METRICS;
	pMetrics->strText = GetPreviewText();
	pRow->FillMetrics(NULL, this, pMetrics);

	CString strPreviewText = pMetrics->strText;

	if (strPreviewText.IsEmpty())
	{
		pMetrics->InternalRelease();
		return 0;
	}

	CXTPReportControl* pControl = pRow->GetControl();

	int nIndentWidth = pControl->GetHeaderIndent();


	CRect& rcIndent = pControl->GetPaintManager()->m_rcPreviewIndent;

	CRect rcPreviewItem(nIndentWidth + rcIndent.left, 0, nWidth - rcIndent.right, 0);

	if (m_pMarkupUIElement)
	{
		// Calculate Markup item height
		XTPMarkupSetDefaultFont(XTPMarkupElementContext(m_pMarkupUIElement), (HFONT)pMetrics->pFont->GetSafeHandle(), pMetrics->clrForeground);
		CSize szMarkup = XTPMarkupMeasureElement(m_pMarkupUIElement, rcPreviewItem.Width(), INT_MAX);
		nHeight = szMarkup.cy + rcIndent.top + rcIndent.bottom;
	}
	else
	{
		// Calculate height of a usual text
		CXTPFontDC font(pDC, pMetrics->pFont);

		int nMaxPreviewLines = pControl->GetPaintManager()->GetPreviewLinesCount(pDC, rcPreviewItem, strPreviewText);
		int nFontHeight = pDC->GetTextExtent(_T(" "), 1).cy;


		nHeight = nFontHeight * nMaxPreviewLines + rcIndent.top + rcIndent.bottom;
	}

	// Cleanup
	pMetrics->InternalRelease();
	return nHeight;
}

void CXTPReportRecordItemPreview::GetCaptionRect(XTP_REPORTRECORDITEM_ARGS* pDrawArgs, CRect& rcItem)
{
	ASSERT(pDrawArgs->pControl);
	if (!pDrawArgs->pControl)
		return;

	CRect& rcIndent = pDrawArgs->pControl->GetPaintManager()->m_rcPreviewIndent;
	rcItem.DeflateRect(rcIndent.left - 2, -1, rcIndent.right, -rcIndent.bottom);
}


void CXTPReportRecordItemPreview::OnDrawCaption(
	XTP_REPORTRECORDITEM_DRAWARGS *pDrawArgs,
	XTP_REPORTRECORDITEM_METRICS  *pMetrics)
{
	ASSERT(pDrawArgs->pControl);
	if (!pDrawArgs->pControl)
		return;

	if (!pDrawArgs->pControl->GetPaintManager()->OnDrawAction(pDrawArgs))
		return;

	//pDrawArgs->pRow->GetItemMetrics(pDrawArgs, pMetrics);

	CString strText = pMetrics->strText;

	// draw item text
	if (!strText.IsEmpty())
	{
		CRect rcItem(pDrawArgs->rcItem);

		CRect& rcIndent = pDrawArgs->pControl->GetPaintManager()->m_rcPreviewIndent;

		rcItem.DeflateRect(rcIndent.left, rcIndent.top, rcIndent.right, rcIndent.bottom);

		if (m_pMarkupUIElement)
		{
			XTPMarkupSetDefaultFont(XTPMarkupElementContext(m_pMarkupUIElement), (HFONT)pMetrics->pFont->GetSafeHandle(), pMetrics->clrForeground);

			XTPMarkupMeasureElement(m_pMarkupUIElement, rcItem.Width(), INT_MAX);

			XTPMarkupRenderElement(m_pMarkupUIElement, pDrawArgs->pDC->GetSafeHdc(), &rcItem);
		}
		else
		{
			CXTPFontDC font(pDrawArgs->pDC, pMetrics->pFont);
			pDrawArgs->pDC->DrawText(strText, rcItem, DT_WORDBREAK | DT_LEFT | DT_NOPREFIX);
		}
	}
}


void CXTPReportRecordItemPreview::OnBeginEdit(XTP_REPORTRECORDITEM_ARGS *pItemArgs)
{
	if (m_pControl && m_pControl->IsPreviewAllowEdit())
	{
		CXTPReportRecordItem::OnBeginEdit(pItemArgs);
	}
}
