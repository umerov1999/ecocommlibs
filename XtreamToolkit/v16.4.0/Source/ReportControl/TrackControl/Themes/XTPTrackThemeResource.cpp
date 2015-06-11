// XTPTrackThemeResource.cpp : implementation of the CXTPTrackThemeResource class.
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

#include <ReportControl/TrackControl/XTPTrackPaintManager.h>
#include <ReportControl/TrackControl/Themes/XTPTrackThemeResource.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// class: CXTPTrackThemeResource
/////////////////////////////////////////////////////////////////////////////

CXTPTrackThemeResource::CXTPTrackThemeResource()
{
	m_columnStyle = xtpReportColumnResource;
}

void CXTPTrackThemeResource::RefreshMetrics()
{
	CXTPTrackPaintManager::RefreshMetrics();

	if (m_clrColumnOffice2007CustomTheme != COLORREF_NULL)
	{
		COLORREF clrHsl = XTPDrawHelpers()->RGBtoHSL(m_clrColumnOffice2007CustomTheme);
		BYTE Hue = GetRValue(clrHsl);
		BYTE Sat = GetGValue(clrHsl);

		m_grcGradientColumnPushed.SetStandardValue(XTPDrawHelpers()->HSLtoRGB(RGB(Hue, Sat, 212)), XTPDrawHelpers()->HSLtoRGB(RGB(Hue, Sat, 239)));
		m_grcGradientColumn.SetStandardValue(XTPDrawHelpers()->HSLtoRGB(RGB(Hue, Sat, 239)), XTPDrawHelpers()->HSLtoRGB(RGB(Hue, Sat, 212)));
		m_clrGradientColumnShadow.SetStandardValue(XTPDrawHelpers()->HSLtoRGB(RGB(Hue, (BYTE)(Sat * 0.525f), 145)));
		m_clrGradientColumnSeparator.SetStandardValue(XTPDrawHelpers()->HSLtoRGB(RGB(Hue, Sat, 192)));
		m_clrGroupBoxBack.SetStandardValue(XTPDrawHelpers()->HSLtoRGB(RGB(Hue, Sat, 227)));
		m_crlNoGroupByText.SetStandardValue(XTPDrawHelpers()->HSLtoRGB(RGB(Hue, (BYTE)(Sat * 0.75f), 79)));
		m_clrGroupShadeBack.SetStandardValue(XTPDrawHelpers()->HSLtoRGB(RGB(Hue, (BYTE)(Sat * 0.88f), 208)));
	}
	else
	{
		CXTPResourceImages* pImages = XTPResourceImages();

		if (pImages->IsValid())
		{
			m_grcGradientColumn.SetStandardValue(pImages->GetImageColor(_T("ReportControl"), _T("NormalColumnLight")),
				pImages->GetImageColor(_T("ReportControl"), _T("NormalColumnDark")));
			m_grcGradientColumnPushed.SetStandardValue(pImages->GetImageColor(_T("ReportControl"), _T("PressedColumnLight")),
				pImages->GetImageColor(_T("ReportControl"), _T("PressedColumnDark")));

			m_clrGradientColumnShadow.SetStandardValue(pImages->GetImageColor(_T("ReportControl"), _T("ColumnShadow")));
			m_clrGradientColumnSeparator.SetStandardValue(pImages->GetImageColor(_T("ReportControl"), _T("ColumnSeparator")));
			m_clrGroupBoxBack.SetStandardValue(pImages->GetImageColor(_T("ReportControl"), _T("GroupBoxBackground")));
			m_crlNoGroupByText.SetStandardValue(pImages->GetImageColor(_T("ReportControl"), _T("GroupBoxTextColor")));
			m_clrGroupShadeBack.SetStandardValue(pImages->GetImageColor(_T("ReportControl"), _T("GroupShadeBackground")));
			m_clrGroupShadeBorder.SetStandardValue(pImages->GetImageColor(_T("ReportControl"), _T("GroupShadeBorder")));
			m_clrIndentControl.SetStandardValue(pImages->GetImageColor(_T("ReportControl"), _T("GroupShadeIndent")));
			m_clrHighlight.SetStandardValue(pImages->GetImageColor(_T("ReportControl"), _T("SelectionBackground")));
			m_clrHighlightText.SetStandardValue(0);

			m_nGroupGridLineStyle = (XTPReportGridStyle)pImages->GetImageInt(_T("ReportControl"), _T("GroupShadeBorderLineStyle"), xtpReportGridSolid);
		}
		else
		{
			m_grcGradientColumnPushed.SetStandardValue(RGB(196, 221, 255), RGB(252, 253, 255));
			m_grcGradientColumn.SetStandardValue(RGB(252, 253,255), RGB(196, 221, 255));
			m_clrGradientColumnShadow.SetStandardValue(RGB(101, 147, 207));
			m_clrGradientColumnSeparator.SetStandardValue(RGB(154, 198, 255));
			m_clrGroupBoxBack.SetStandardValue(RGB(227, 239, 255));
			m_crlNoGroupByText.SetStandardValue(RGB(50, 75, 44));
			m_clrGroupShadeBack.SetStandardValue(RGB(190, 218, 251));
			m_clrGroupShadeBorder.SetStandardValue(RGB(111, 157, 217));
			m_clrIndentControl.SetStandardValue(RGB(253, 238, 201));
		}

	}

	m_grcGradientColumnHot.SetStandardValue(m_grcGradientColumn);
	m_clrGroupRowText.SetStandardValue(m_crlNoGroupByText);
	m_clrConnector.SetStandardValue(m_clrGroupShadeBorder);
}

CSize CXTPTrackThemeResource::DrawCollapsedBitmap(CDC* pDC, const CXTPReportRow* pRow, CRect& rcBitmap)
{
	//Check for resource image dll
	CXTPResourceImage* pImage = XTPResourceImages()->LoadFile(_T("ReportExpandButton"));
	if (pImage)
	{
		rcBitmap.left += 4;

		//use first image as size for bitmap
		CRect rcSrc(pImage->GetSource(0, 4));

		CSize sizeGlyph(rcSrc.Size());

		if (pDC)
		{
			POINT ptIcon;
			ptIcon.x = rcBitmap.left;

			ptIcon.y = rcBitmap.top + (rcBitmap.Height() - rcSrc.Height()) /2;

			CRect rcDest(ptIcon, rcSrc.Size());

			int nState = pRow->IsExpanded() ? 0 : 2;

			if (pRow == pRow->GetControl()->m_pHotExpandButtonRow)
				nState++;

			pImage->DrawImage(pDC, rcDest, pImage->GetSource(nState, 4));
		}


		if (sizeGlyph.cx != 0 && pDC != NULL)
			rcBitmap.right = rcBitmap.left + sizeGlyph.cx;

		return CSize(2 + sizeGlyph.cx, 2 + sizeGlyph.cy);
	}
	else
	{
		return CXTPTrackPaintManager::DrawCollapsedBitmap(pDC, pRow, rcBitmap);
	}
}

void CXTPTrackThemeResource::DrawNoGroupByText(CDC* pDC, CRect rcItem)
{
	ASSERT(m_pControl);
	if (!m_pControl)
		return;

	if (m_pControl->GetMarkupContext())
	{
		if (!m_pMarkupUIElement_NoGroupBy ||
			m_strNoGroupBy != m_strMarkupUIElement_NoGroupBy)
		{
			XTPMarkupReleaseElement(m_pMarkupUIElement_NoGroupBy);
			m_pMarkupUIElement_NoGroupBy = XTPMarkupParseText(m_pControl->GetMarkupContext(), m_strNoGroupBy);
			m_strMarkupUIElement_NoGroupBy = m_strNoGroupBy;
		}

		if (m_pMarkupUIElement_NoGroupBy)
		{
			CRect rcGroupBy = m_pControl->GetElementRect(xtpReportElementRectGroupByArea);
			rcGroupBy.left += 9;

			XTPMarkupMeasureElement(m_pMarkupUIElement_NoGroupBy, rcGroupBy.Width(), INT_MAX);

			XTPMarkupSetDefaultFont(m_pControl->GetMarkupContext(), (HFONT)m_fontCaption, m_crlNoGroupByText);
			XTPMarkupRenderElement(m_pMarkupUIElement_NoGroupBy, pDC->GetSafeHdc(), &rcGroupBy);

			return;
		}
	}
	else
	{
		XTPMarkupReleaseElement(m_pMarkupUIElement_NoGroupBy);
		m_strMarkupUIElement_NoGroupBy.Empty();
	}

	int nWidth = max(55, pDC->GetTextExtent(m_strNoGroupBy).cx + 8);
	rcItem.right = rcItem.left + nWidth;

	pDC->SetTextColor(m_crlNoGroupByText);
	pDC->DrawText(m_strNoGroupBy, &rcItem, DT_END_ELLIPSIS | DT_CENTER | DT_NOPREFIX);

}

void CXTPTrackThemeResource::FillFooter(CDC* pDC, CRect rcFooter)
{
	XTPDrawHelpers()->GradientFill(pDC, rcFooter,
		m_grcGradientColumn.clrDark, m_grcGradientColumn.clrLight, FALSE);

	DrawHorizontalLine(pDC, rcFooter.left, rcFooter.top, rcFooter.Width(), m_clrGroupBoxBack);
	DrawHorizontalLine(pDC, rcFooter.left, rcFooter.top + 1, rcFooter.Width(), m_clrGradientColumnShadow);
	DrawHorizontalLine(pDC, rcFooter.left, rcFooter.top + 2, rcFooter.Width(), MixColor(m_grcGradientColumn.clrDark, m_clrGradientColumnShadow, 0.25));
	DrawHorizontalLine(pDC, rcFooter.left, rcFooter.top + 3, rcFooter.Width(), MixColor(m_grcGradientColumn.clrDark, m_clrGradientColumnShadow, 0.1));

	DrawHorizontalLine(pDC, rcFooter.left, rcFooter.bottom - 1, rcFooter.Width(), m_clrGradientColumnShadow);
}

void CXTPTrackThemeResource::DrawTriangle(CDC* pDC, CRect rcTriangle, BOOL bToDown, int nShadowWidth, BOOL /*bColumnPressed*/)
{
	CRect rcTRect;
	int iLastImage = xtpReportGlyphCount; // last image in (m_pGlyphs->SetIcons(bmp, 0, 11, CSize(0, 0)) = 11

	if (rcTriangle.Width() > 10)
	{
		//get non-existed image placeholder and fill it with sorting arrows

		CXTPImageManagerIcon* pImage = m_pGlyphs->GetImage(bToDown ? iLastImage : iLastImage + 1);
		if (pImage)
		{
			pImage->Draw(pDC, CPoint((rcTriangle.left + rcTriangle.right - pImage->GetWidth()) /2,
				(rcTriangle.top + rcTriangle.bottom - pImage->GetHeight()) / 2 + (nShadowWidth ? 1 : 0)));
			return;
		}

		CPoint pt(rcTriangle.CenterPoint());

		if (bToDown)
		{
			CXTPDrawHelpers::Triangle(pDC, CPoint(pt.x - 4, pt.y - 2),
				CPoint(pt.x, pt.y + 2), CPoint(pt.x + 4, pt.y  - 2), m_clrGradientColumnSeparator);
		}
		else
		{
			CXTPDrawHelpers::Triangle(pDC, CPoint(pt.x - 4, pt.y + 2),
				CPoint(pt.x, pt.y - 2), CPoint(pt.x + 4, pt.y  + 2), m_clrGradientColumnSeparator);
		}
	}
}

void CXTPTrackThemeResource::DrawColumnBackground(CDC* pDC, CXTPReportColumn* pColumn, CRect rcColumn, BOOL& bColumnPressed, BOOL& /*bDraggingFromHeader*/, CXTPReportHeader* /*pHeader*/)
{
	XTPDrawHelpers()->GradientFill(pDC, rcColumn,
		bColumnPressed ? m_grcGradientColumnPushed: pColumn->IsHotTracking() ? m_grcGradientColumnHot : m_grcGradientColumn, FALSE);

	DrawHorizontalLine(pDC, rcColumn.left, rcColumn.bottom - 1, rcColumn.Width(), m_clrGradientColumnShadow);

	if (pColumn->GetDrawHeaderDivider())
		DrawVerticalLine(pDC, rcColumn.right - 1, rcColumn.top + 2, rcColumn.Height() - 2 - 4, m_clrGradientColumnSeparator);

	if (pColumn->GetControl()->IsGroupByVisible())
		DrawHorizontalLine(pDC, rcColumn.left, rcColumn.top, rcColumn.Width(), m_clrGradientColumnShadow);

	bColumnPressed = FALSE;
}

void CXTPTrackThemeResource::DrawColumnFooterDivider(CDC* pDC, CXTPReportColumn* pColumn, CRect rcColumn)
{
	if (pColumn->GetDrawFooterDivider())
	{
		DrawVerticalLine(pDC, rcColumn.right, rcColumn.top + 6, rcColumn.Height() - 9, m_clrControlLightLight);
		DrawVerticalLine(pDC, rcColumn.right - 1, rcColumn.top + 5, rcColumn.Height() - 9, m_clrGradientColumnSeparator);
	}
}

void CXTPTrackThemeResource::DrawColumn(CDC *pDC, CXTPReportColumn *pColumn, CXTPReportHeader *pHeader, CRect rcColumn, BOOL bDrawExternal, int nShadowWidth/*= 0*/)
{
	// save column parameter for future use in drawing
	BOOL bDraggingFromHeader = bDrawExternal
								&& pColumn->IsDragging()
								&& rcColumn.left == 0
								&& rcColumn.top == 0
								&& pHeader->IsDragHeader();

	BOOL bPlainColumn = bDrawExternal
						&& !bDraggingFromHeader;

	BOOL bColumnPressed = pColumn->IsDragging()
							&& !bDrawExternal
							&& pHeader->IsDragHeader();

	CRect rcColumn0 = rcColumn;
	// draw
	int nIconID = pColumn->GetIconID();
	CString strCaption = pColumn->GetCaption();

	if (bPlainColumn)
	{
		XTPDrawHelpers()->GradientFill(pDC, rcColumn, m_grcGradientColumn, FALSE);
		pDC->Draw3dRect(rcColumn, m_clrGradientColumnShadow, m_clrGradientColumnShadow);
	}
	else
	{
		DrawColumnBackground(pDC, pColumn, rcColumn, bColumnPressed, bDraggingFromHeader, pHeader);
	}

	pDC->SetBkColor(m_clrHeaderControl);

	rcColumn.bottom = rcColumn.bottom - 1 - nShadowWidth;

	CRect rcText(rcColumn);
	if (!pColumn->IsAutoSize() && !bPlainColumn)
		rcText.left += pColumn->GetIndent();

	BOOL bHasSortTriangle = pColumn->HasSortTriangle() && pColumn->IsSortable()
		&& (!pDC->IsPrinting() || m_bPrintSortTriangle);

	if (m_DrawSortTriangleStyle == xtpReportDrawSortTriangleNever)
		bHasSortTriangle = FALSE;

	CXTPFontDC font(pDC, &m_fontCaption);
	CSize sizeText = pDC->GetTextExtent(strCaption);

	CXTPMarkupUIElement* pMarkupUIElement = pColumn->GetMarkupUIElement();

	if (pMarkupUIElement)
	{
		sizeText = XTPMarkupMeasureElement(pMarkupUIElement, INT_MAX, INT_MAX);
	}

	int nHeaderAlignment = pColumn->GetHeaderAlignment();
	BOOL bTextRight = (nHeaderAlignment & DT_RIGHT) > 0;
	BOOL bTextCenter = (nHeaderAlignment & DT_CENTER) > 0;
	BOOL bTextLeft = !bTextRight && !bTextCenter;
	BOOL bIconRight = (nHeaderAlignment & xtpColumnIconRight) > 0;
	BOOL bIconCenter = (nHeaderAlignment & xtpColumnIconCenter) > 0;
	if (strCaption.GetLength() > 0 && bIconCenter)
	{
		bIconCenter = FALSE;
		bIconRight = TRUE;
	}
	int nIconWidth = DrawBitmap(NULL, pColumn->GetControl(), rcText, nIconID) + 2;

	const int nTriangleGap = (bPlainColumn ? 17 : 10);

	const int nTriangleSize = 16;

	if (!bPlainColumn) //PlainColumn don't use PlusMinus Glyph and Column Icon
	{
		if (pColumn->IsPlusMinus()
			&& !pDC->IsPrinting()
			&& pColumn->GetNextVisualBlock() != 0)
		{
			int iIconWidthExtra = DrawGlyph(NULL, rcText, 1).cx;
			if (iIconWidthExtra > 0)
			{
				CRect rcIcon(rcText);
				rcIcon.left = rcIcon.right - iIconWidthExtra - 2;
				if (m_bColumnPlusMinusSign) //[+|-] glyph
					DrawGlyph(pDC, rcIcon, pColumn->IsExpanded() ? 1 : 0);
				else //[<|>] glyph
				{
					if (pColumn->GetNextVisualBlock() > 0)
						DrawGlyph(pDC, rcIcon, pColumn->IsExpanded() ? 7 : 6);
					else if (pColumn->GetNextVisualBlock() < 0)
						DrawGlyph(pDC, rcIcon, pColumn->IsExpanded() ? 7 : 6);
				}
				rcText.right = rcIcon.left;
			}
		}

		//Check - do we have enough space to draw Icon
		if (m_bCompactText
			&& nIconID != XTP_REPORT_NOICON
			&& rcText.Width() <= 2 * nIconWidth
			&& strCaption.GetLength() > 0)
		{
			nIconID = XTP_REPORT_NOICON; //can't draw
		}

		if (!bTextLeft)
		{
			int nLength = 6 + sizeText.cx;

			if (nIconID != XTP_REPORT_NOICON)
				nLength += nIconWidth;

			if (bHasSortTriangle)
				nLength += nTriangleGap + nTriangleSize + 2;

			if (rcText.Width() > nLength)
			{
				if (bTextRight)
					rcText.left = rcText.right - nLength;
				else if (bTextCenter)
					rcText.left = (rcText.left + rcText.right - nLength) / 2;
			}
		}

		if (nIconID != XTP_REPORT_NOICON)
		{
			CRect rcIcon(rcText);
			rcIcon.bottom += nShadowWidth;
			if (bIconCenter) //No Text case only
			{
				int w = rcIcon.Width() - nIconWidth;
				rcIcon.left += w / 2;
				DrawColumnIcon(pDC, pColumn, rcColumn0, rcIcon, nIconID);
				rcText.left = rcIcon.left + nIconWidth + 2;
			}
			else if (bIconRight)
			{
				rcIcon.left = rcIcon.right - nIconWidth;
				DrawColumnIcon(pDC, pColumn, rcColumn0, rcIcon, nIconID);
				rcText.right = rcIcon.left;
			}
			else
			{
				rcText.left += DrawColumnIcon(pDC, pColumn, rcColumn0, rcIcon, nIconID);
				rcText.left += 2;
			}
		}
	}
	else
	{
		nIconID = -1;
		nIconWidth = 0;
	}
	pDC->SetBkMode(TRANSPARENT);


	rcText.DeflateRect(3, 0, 3, 0);

	CRect rcTriangle(rcText);

	if (strCaption.GetLength() > 0 && rcText.right > rcText.left && pColumn->GetDisplayOptions()->Column()->IsShowText())
	{

		if (bHasSortTriangle)
		{
			if ((sizeText.cx + nTriangleGap + nTriangleSize > rcText.Width())
				&& (m_DrawSortTriangleStyle == xtpReportDrawSortTriangleAlways)
				&& (rcText.Width() > nTriangleSize))
			{
				rcTriangle.left = rcTriangle.right - nTriangleSize;
				rcText.right -= nTriangleSize + 2;
			}
			else
			{
				rcTriangle.left += sizeText.cx + nTriangleGap;
			}
		}

		if (pMarkupUIElement)
		{
			XTPMarkupSetDefaultFont(XTPMarkupElementContext(pMarkupUIElement), (HFONT) m_fontCaption.GetSafeHandle(), m_clrCaptionText);
			XTPMarkupRenderElement(pMarkupUIElement, pDC->GetSafeHdc(), &rcText);
		}
		else
		{
			pDC->SetTextColor(m_clrCaptionText);

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
				uFlags |= DT_SINGLELINE | DT_VCENTER;
			}

			pDC->DrawText(strCaption, &rcText, uFlags);
		}
	}

	if (bHasSortTriangle && rcTriangle.Width() >= nTriangleSize)
	{
		if (m_bSortTriangleOffsetFromRight)
			rcTriangle.left = rcTriangle.right - nTriangleSize;
		else
			rcTriangle.right = rcTriangle.left + nTriangleSize;

		if (rcTriangle.right <= rcColumn.right)
			DrawTriangle(pDC, rcTriangle, pColumn->IsSortedDecreasing(), nShadowWidth, bColumnPressed);
	}

	if (bColumnPressed && m_bInvertColumnOnClick)
	{
		rcColumn.bottom += nShadowWidth;
		pDC->InvertRect(&rcColumn);
	}
}

void CXTPTrackThemeResource::DrawSectionDivider(
	CDC *pDC, const CRect& rc,
	XTPReportSectionDividerPosition dividerPosition,
	XTPReportSectionDividerStyle    dividerStyle,
	CXTPPaintManagerColor           clrDivider)
{
	if (dividerStyle == xtpReportFixedRowsDividerOutlook)
	{
		pDC->FillSolidRect(rc,  MixColor(m_grcGradientColumn.clrLight, m_grcGradientColumn.clrDark, 0.4));
		DrawHorizontalLine(pDC, rc.left, rc.top, rc.Width(), RGB(255, 255, 255));
		DrawHorizontalLine(pDC, rc.left, rc.bottom - 1, rc.Width(), m_clrGradientColumnSeparator);
	}
	else
	{
		CXTPTrackPaintManager::DrawSectionDivider(pDC, rc, dividerPosition, dividerStyle, clrDivider);
	}
}
