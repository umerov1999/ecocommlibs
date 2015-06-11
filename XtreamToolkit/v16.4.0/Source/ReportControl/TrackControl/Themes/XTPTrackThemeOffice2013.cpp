// XTPTrackThemeOffice2013.cpp : implementation of the CXTPTrackPaintManager class.
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

#include <Common/XTPResourceManager.h>
#include <Common/XTPDrawHelpers.h>
#include <Common/XTPImageManager.h>
#include <Common/XTPResourceImage.h>
#include <Common/XTPMarkupRender.h>
#include <Common/XTPColorManager.h>
#include <Common/XTPWinThemeWrapper.h>
#include <Common/XTPCustomHeap.h>
#include <Common/XTPSystemHelpers.h>
#include <Common/XTPSmartPtrInternalT.h>

#include <ReportControl/Resource.h>
#include <ReportControl/XTPReportDefines.h>
#include <ReportControl/XTPReportPaintManager.h>
#include <ReportControl/XTPReportRow.h>
#include <ReportControl/XTPReportGroupRow.h>
#include <ReportControl/XTPReportColumn.h>
#include <ReportControl/XTPReportHeader.h>
#include <ReportControl/XTPReportControl.h>
#include <ReportControl/XTPReportRecordItem.h>
#include <ReportControl/XTPReportInplaceControls.h>
#include <ReportControl/XTPReportRecordItemControls.h>
#include <ReportControl/XTPReportColumns.h>
#include <ReportControl/XTPReportRecord.h>
#include <ReportControl/XTPReportHyperlink.h>

#include <ReportControl/TrackControl/XTPTrackPaintManager.h>
#include <ReportControl/TrackControl/Themes/XTPTrackThemeOffice2013.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// class: CXTPTrackThemeOffice2013
/////////////////////////////////////////////////////////////////////////////

CXTPTrackThemeOffice2013::CXTPTrackThemeOffice2013()
{
	m_bMakeUpper = TRUE;
	m_columnStyle = xtpReportColumnShaded;
	LoadGlyphs();
}

void CXTPTrackThemeOffice2013::LoadGlyphs()
{
	CMDTARGET_RELEASE(m_pGlyphs);
	m_pGlyphs = new CXTPImageManager();

	CBitmap bmp;
	CXTPResourceManager::AssertValid(XTPResourceManager()->LoadBitmap(&bmp, XTP_IDB_REPORT_GLYPHS_2013));

	CXTPTempColorMask mask(m_pGlyphs, RGB(255, 0, 255));
	m_pGlyphs->SetIcons(bmp, 0, xtpReportGlyphCount, CSize(0, 0)); //11
}

void CXTPTrackThemeOffice2013::RefreshMetrics()
{
	CMDTARGET_RELEASE(m_pHyperlinkStyle);
	m_pHyperlinkStyle = new CXTPReportHyperlinkStyle(RGB(0x33,0x66,0x99), xtpReportTextDecorationNone);

	CMDTARGET_RELEASE(m_pHyperlinkHoverStyle);
	m_pHyperlinkHoverStyle = new CXTPReportHyperlinkStyle(RGB(0x33,0x66,0x99), xtpReportTextDecorationUnderline);

	m_bInvertColumnOnClick = FALSE;

	m_clrBtnFace.SetStandardValue(RGB(0xff,0xff,0xff));
	m_clrBtnFacePressed.SetStandardValue(RGB(0xf3,0xf5,0xf8));
	m_clrBtnText.SetStandardValue(RGB(0x26,0x26,0x26));
	m_clrControlDark.SetStandardValue(RGB(0xa5,0xac,0xb5));
	m_clrControlLightLight.SetStandardValue(RGB(0xff,0xff,0xff));
	m_clrHighlight.SetStandardValue(RGB(0xcd,0xe6,0xf7));
	m_clrHighlightText.SetStandardValue(m_clrBtnText);
	m_clrBoldText.SetStandardValue(RGB(0x00,0x67,0xb0));
	m_clrBoldTextHighlight.SetStandardValue(m_clrHighlightText);
	m_clrGroupText.SetStandardValue(RGB(0x44,0x44,0x44));
	m_clrGroupTextHilite.SetStandardValue(m_clrGroupText);
	m_clrGroupBack.SetStandardValue(RGB(0xe1,0xe1,0xe1));
	m_clrGroupBackHilite.SetStandardValue(RGB(0xcd,0xe6,0xf7));
	m_clrSelectedRow.SetStandardValue(RGB(0xe1,0xe1,0xe1));
	m_clrSelectedRowText.SetStandardValue(m_clrBtnText);
	m_clrItemShade.SetStandardValue(RGB(0xe6,0xf2,0xfa));
	m_clrHotDivider.SetStandardValue(RGB(0xFF, 0, 0));
	m_clrFreezeColsDivider.SetStandardValue(RGB(0, 0, 222));

	m_clrWindowText.SetStandardValue(m_clrBtnText);
	m_clrControlBack.SetStandardValue(m_clrBtnFace);
	m_clrConnector.SetStandardValue(m_clrControlDark);

	m_clrIndentControl.SetStandardValue(m_clrBtnFace);
	m_clrCaptionText.SetStandardValue(m_clrBtnText);
	m_clrCaptionTextPressed.SetStandardValue(m_clrCaptionText);
	m_clrHeaderControl.SetStandardValue(m_clrBtnFace);
	m_clrGroupShadeBorder.SetStandardValue(m_clrControlDark);
	m_clrGroupShadeBack.SetStandardValue(m_clrBtnFace);
	m_clrGroupShadeText.SetStandardValue(m_clrBtnText);

	m_clrGroupRowText.SetStandardValue(m_clrControlDark);
	m_clrGroupBoxBack.SetStandardValue(m_clrBtnFace);
	m_crlNoGroupByText.SetStandardValue(m_clrControlDark);

	m_clrGradientColumnSeparator.SetStandardValue(m_clrControlDark);
	m_clrGlyph.SetStandardValue(m_clrControlDark);
	m_clrGlyphHighlight.SetStandardValue(m_clrControlDark);
	SetGridColor(m_clrBtnFace);

	m_nGroupGridLineStyle = xtpReportGridSolid;

	m_brushVeriticalGrid.DeleteObject();
	m_brushHorizontalGrid.DeleteObject();
	m_brushTreeStructure.DeleteObject();
	m_brushGroupGridLineStyle.DeleteObject();

	LOGFONT lf;
	m_fontCaption.GetLogFont(&lf);
	SetCaptionFont(lf);
}

void CXTPTrackThemeOffice2013::DrawColumnText(CDC* pDC, CRect rcText, CString strCaption, int nHeaderAlignment, BOOL bIsHeader, BOOL bColumnPressed)
{
	if (bIsHeader == FALSE && m_bMakeUpper == TRUE)
	{
		strCaption.MakeUpper();
	}

	pDC->SetTextColor(bColumnPressed ? m_clrCaptionTextPressed : m_clrCaptionText);

	UINT uFlags = DT_END_ELLIPSIS | DT_NOPREFIX;
	if ((nHeaderAlignment & DT_WORDBREAK) > 0)
	{
		uFlags |= DT_WORDBREAK | DT_WORD_ELLIPSIS;
		if (nHeaderAlignment & DT_RIGHT)
			uFlags |= DT_RIGHT;

		// try to center vertically because DT_VCENTER works only for DT_SINGLELINE;
		CRect rcTextReal = rcText;
		pDC->DrawText(strCaption, &rcTextReal, uFlags | DT_CALCRECT);

		int nHeightDiff = rcText.Height() - rcTextReal.Height();
		if (nHeightDiff > 1)
			rcText.top += nHeightDiff / 2;
	}

	else
	{
		rcText.OffsetRect(2,0);
		uFlags |= DT_SINGLELINE | DT_VCENTER;
	}

	pDC->DrawText(strCaption, &rcText, uFlags);
}

void CXTPTrackThemeOffice2013::DrawPlainColumnBackground(CDC* pDC, CRect rcColumn)
{
	pDC->Draw3dRect(rcColumn, m_clrControlDark, m_clrControlDark);
}

void CXTPTrackThemeOffice2013::DrawColumnBackground(CDC* pDC, CXTPReportColumn* pColumn, CRect rcColumn, BOOL& bColumnPressed, BOOL& bDraggingFromHeader, CXTPReportHeader* pHeader)
{
	CXTPReportColumn* pPrevColumn = pHeader ? pHeader->GetNextVisibleColumn(pColumn->GetIndex(), -1) : NULL;

	if (!bDraggingFromHeader)
	{
		if (bColumnPressed)
		{
			pDC->FillSolidRect(rcColumn, m_clrBtnFacePressed);
		}

		DrawHorizontalLine(pDC, rcColumn.left, rcColumn.bottom - 1, rcColumn.Width(), m_clrControlDark);

		if (pPrevColumn && pColumn->GetDrawHeaderDivider())
			DrawVerticalLine(pDC, rcColumn.left, rcColumn.top + 3, rcColumn.Height() - 6, m_clrControlDark);

		if (pColumn->GetControl()->IsGroupByVisible())
			DrawHorizontalLine(pDC, rcColumn.left, rcColumn.top, rcColumn.Width(), m_clrControlDark);
	}
}

void CXTPTrackThemeOffice2013::DrawColumnInverted(CDC *pDC, CRect rcColumn, BOOL /*bColumnPressed*/, BOOL bDraggingFromHeader, int nShadowWidth)
{
	if (bDraggingFromHeader)
	{
		rcColumn.bottom += nShadowWidth;
		pDC->InvertRect(&rcColumn);
	}
}

void CXTPTrackThemeOffice2013::DrawTriangle(CDC* pDC, CRect rcTriangle, BOOL bToDown, BOOL bDraggingFromHeader, int /*nShadowWidth*/, BOOL bColumnPressed)
{
	if (rcTriangle.Width() > 10 && !bDraggingFromHeader)
	{
		CPoint pt(rcTriangle.CenterPoint());
		pt.Offset(10,0);

		if (bToDown)
		{
			CXTPDrawHelpers::Triangle(pDC, CPoint(pt.x - 4, pt.y - 2),
				CPoint(pt.x, pt.y + 2), CPoint(pt.x + 4, pt.y  - 2), bColumnPressed ? m_clrGlyphHighlight : m_clrGlyph);
		}
		else
		{
			CXTPDrawHelpers::Triangle(pDC, CPoint(pt.x - 4, pt.y + 2),
				CPoint(pt.x, pt.y - 2), CPoint(pt.x + 4, pt.y  + 2), bColumnPressed ? m_clrGlyphHighlight : m_clrGlyph);
		}
	}
}

void CXTPTrackThemeOffice2013::DrawWindowFrame(CDC* pDC, CRect rcWindow)
{
	if (::IsWindow(m_pControl->GetSafeHwnd()))
	{
		if (m_pControl->GetExStyle() & WS_EX_STATICEDGE)
		{
			pDC->Draw3dRect(rcWindow, m_clrBtnFace, m_clrBtnFace);
		}
		else
		{
			CXTPTrackPaintManager::DrawWindowFrame(pDC, rcWindow);
		}
	}
}

void CXTPTrackThemeOffice2013::FillGroupRowMetrics(CXTPReportGroupRow* pRow,
												 XTP_REPORTRECORDITEM_METRICS* pMetrics, BOOL bPrinting)
{
	ASSERT(pRow && pMetrics);
	if (!pRow || !pRow->GetControl() || !pMetrics)
		return;

	BOOL bControlFocused = pRow->GetControl()->HasFocus();

	pMetrics->clrForeground = m_clrGroupRowText;
	pMetrics->clrBackground = XTP_REPORT_COLOR_DEFAULT;

	if (pRow->IsSelected() && bControlFocused && !bPrinting)
	{
		pMetrics->clrForeground = m_clrHighlightText;
		pMetrics->clrBackground = m_clrHighlight;
	}
	else if (m_bShadeGroupHeadings)
	{
		pMetrics->clrForeground = m_clrGroupShadeText;
		pMetrics->clrBackground = m_clrGroupShadeBack;
	}

	pMetrics->pFont = m_bGroupRowTextBold ? &m_fontBoldText : &m_fontText;
}

void CXTPTrackThemeOffice2013::DrawGroupRowBack(CDC* pDC, CRect rcRow, XTP_REPORTRECORDITEM_METRICS* pMetrics)
{
	if (pMetrics->clrBackground != XTP_REPORT_COLOR_DEFAULT)
	{
		pDC->FillSolidRect(rcRow, pMetrics->clrBackground);
		DrawHorizontalLine(pDC, rcRow.left, rcRow.top-1, rcRow.Width(), m_clrBtnFace);
		DrawHorizontalLine(pDC, rcRow.left, rcRow.top, rcRow.Width(), m_clrBtnFace);
	}
}

void CXTPTrackThemeOffice2013::DrawGroupRow(CDC* pDC, CXTPReportGroupRow* pRow, CRect rcRow,
										  XTP_REPORTRECORDITEM_METRICS* pMetrics)
{
	CXTPReportControl *pControl = pRow->GetControl();
	BOOL bControlFocused = pControl->HasFocus();

	if (pRow->IsFocused() && bControlFocused && !pDC->IsPrinting() && pControl->IsRowFocusVisible())
	{
		m_clrGroupShadeBorder = m_clrGroupBackHilite;
		pMetrics->clrForeground = m_clrGroupTextHilite;
	}
	else
	{
		m_clrGroupShadeBorder = m_clrGroupBack;
		pMetrics->clrForeground = m_clrGroupText;
	}

	pMetrics->clrBackground = m_clrGroupShadeBorder;

	CXTPTrackPaintManager::DrawGroupRow(pDC, pRow, rcRow, pMetrics);
}

void CXTPTrackThemeOffice2013::DrawFocusedRow(CDC* pDC, CRect rcRow)
{
	if (m_pControl && ((CXTPReportControl* ) m_pControl)->IsIconView())
		rcRow.DeflateRect(1, 1);

	pDC->Draw3dRect(rcRow, m_clrBtnFace, m_clrBtnFace);
	DrawHorizontalLine(pDC, rcRow.left, rcRow.bottom, rcRow.Width(), m_clrBtnFace);

	COLORREF clrTextColor = pDC->SetTextColor(RGB(0x00,0x00,0x00));
	COLORREF clrBkColor = pDC->SetBkColor(m_clrBtnFace);

	pDC->DrawFocusRect(rcRow);
	pDC->SetTextColor(clrTextColor);
	pDC->SetBkColor(clrBkColor);
}

int CXTPTrackThemeOffice2013::GetRowHeight(CDC* pDC, CXTPReportRow* pRow)
{
	if (m_pControl->m_bThemeMetrics)
	{
		if (!pRow->IsGroupRow())
			return m_nRowHeight + (IsGridVisible(FALSE) ? 1 : 0);

		return m_nRowHeight + 6;
	}
	else
	{
		return CXTPTrackPaintManager::GetRowHeight(pDC, pRow);
	}
}

CRect CXTPTrackThemeOffice2013::GetGroupRowTextSize(CRect rcBitmap, CRect rcRow, int nBitmapWidth, int nNoIconWidth, int nTextOffset)
{
	if (m_pControl->m_bThemeMetrics)
	{
		CRect rcText(rcBitmap.left, rcRow.top, rcRow.right, rcRow.bottom);
		rcText.left += nBitmapWidth;
		rcText.left += nNoIconWidth;
		rcText.left += nTextOffset;
		rcText.left += m_nTreeTextIndentGroupRow;
		return rcText;
	}
	else
	{
		return CXTPTrackPaintManager::GetGroupRowTextSize(
			rcBitmap, rcRow, nBitmapWidth, nNoIconWidth, nTextOffset);
	}
}

CRect CXTPTrackThemeOffice2013::GetGroupRowBmpSize(CRect rcRow, int nBitmapOffset)
{
	if (m_pControl->m_bThemeMetrics)
	{
		CRect rcBitmap(rcRow);
		rcBitmap.OffsetRect(nBitmapOffset,0);
		return rcBitmap;
	}
	else
	{
		return CXTPTrackPaintManager::GetGroupRowBmpSize(rcRow, nBitmapOffset);
	}
}
