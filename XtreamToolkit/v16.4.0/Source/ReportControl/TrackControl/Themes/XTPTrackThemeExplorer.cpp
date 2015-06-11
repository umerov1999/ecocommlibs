// XTPTrackThemeExplorer.cpp : implementation of the CXTPTrackPaintManager class.
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
#include <ReportControl/XTPReportColumn.h>
#include <ReportControl/XTPReportHeader.h>

#include <ReportControl/TrackControl/XTPTrackPaintManager.h>
#include <ReportControl/TrackControl/Themes/XTPTrackThemeExplorer.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// class: CXTPTrackThemeExplorer
/////////////////////////////////////////////////////////////////////////////

CXTPTrackThemeExplorer::CXTPTrackThemeExplorer()
{
	m_columnStyle = xtpReportColumnExplorer;
}

void CXTPTrackThemeExplorer::DrawTriangle(CDC* pDC, CRect rcTriangle, BOOL bToDown, int nShadowWidth)
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

static BOOL XTPReportGlyphIsCheckbox(int iGlyph)
{
	return (xtpReportGlyphEnabledUnchecked  == iGlyph) ||
			 (xtpReportGlyphEnabledChecked    == iGlyph) ||
			 (xtpReportGlyphDisabledUnchecked == iGlyph) ||
			 (xtpReportGlyphDisabledChecked   == iGlyph) ||
			 (xtpReportGlyphEnabledTristate   == iGlyph);
}

CSize CXTPTrackThemeExplorer::DrawGlyph(CDC *pDC, CRect rcColumn, int iGlyph)
{
	CSize size(0,0);

	if (XTPReportGlyphIsCheckbox(iGlyph) && m_themeButton->IsAppThemed())
	{
		m_themeButton->GetThemePartSize(NULL, BP_CHECKBOX, 0, NULL, TS_TRUE, &size);

		if (pDC)
		{
			CRect rectCheckBox(CPoint(rcColumn.left + 1, (rcColumn.top + rcColumn.bottom - size.cy) /2), size);

			int iState = -1;

			switch(iGlyph)
			{
			case xtpReportGlyphEnabledUnchecked:  iState = CBS_UNCHECKEDNORMAL;   break;
			case xtpReportGlyphEnabledChecked:    iState = CBS_CHECKEDNORMAL;     break;
			case xtpReportGlyphDisabledUnchecked: iState = CBS_UNCHECKEDDISABLED; break;
			case xtpReportGlyphDisabledChecked:   iState = CBS_CHECKEDDISABLED;   break;
			case xtpReportGlyphEnabledTristate:   iState = CBS_MIXEDNORMAL;       break;
			} // switch

			m_themeButton->DrawThemeBackground(pDC->m_hDC, BP_CHECKBOX, iState, &rectCheckBox, NULL);
		}
	}
	else
	{
		CXTPImageManagerIcon *pImage = m_pGlyphs->GetImage(iGlyph);

		if (NULL != pImage)
		{
			CSize szImage(pImage->GetWidth(), pImage->GetHeight());
			CSize szColumn(rcColumn.Size());

			if (szImage.cy < szColumn.cy && pDC)
			{
				POINT ptIcon;

				ptIcon.x = rcColumn.left + 1;
				ptIcon.y = rcColumn.top + (szColumn.cy - szImage.cy) /2;
				pImage->Draw(pDC, ptIcon);
			}

			size = CSize(1 + szImage.cx, 1 + szImage.cy);
		}
	}

	return size;
}

BOOL CXTPTrackThemeExplorer::IsColumHotTrackingEnabled() const
{
	return m_bHotTracking && m_themeHeader->IsAppThemed();
}

void CXTPTrackThemeExplorer::DrawColumnBackground(CDC* pDC, CXTPReportColumn* pColumn, CRect rcColumn, BOOL& bColumnPressed, BOOL& /*bDraggingFromHeader*/, CXTPReportHeader* pHeader)
{
	CXTPReportColumn* pPrevColumn = pHeader ? pHeader->GetNextVisibleColumn(pColumn->GetIndex(), -1) : NULL;

	if (m_themeHeader->IsAppThemed())
	{
		int iState = bColumnPressed ? HIS_PRESSED: pColumn->IsHotTracking() ? HIS_HOT : HIS_NORMAL;

		m_themeHeader->DrawThemeBackground(pDC->GetSafeHdc(), HP_HEADERITEM, iState, &rcColumn, NULL);
	}
	else
	{
		if (bColumnPressed)
		{
			pDC->Draw3dRect(rcColumn, m_clrControlDark, m_clrControlDark);
		}
		else
		{
			pDC->Draw3dRect(rcColumn, m_clrControlLightLight, m_clrBtnText);
			rcColumn.DeflateRect(1, 1);
			pDC->Draw3dRect(rcColumn, m_clrHeaderControl, m_clrControlDark);
			if (!pColumn->GetDrawHeaderDivider())
			{
				DrawVerticalLine(pDC, rcColumn.right - 1, rcColumn.top, rcColumn.Height() - 1, m_clrHeaderControl);
				DrawVerticalLine(pDC, rcColumn.right, rcColumn.top, rcColumn.Height() - 1, m_clrHeaderControl);
				pDC->SetPixel(rcColumn.right, rcColumn.top - 1, m_clrControlLightLight);
				pDC->SetPixel(rcColumn.right, rcColumn.bottom - 1, m_clrControlDark);
			}
			if (pPrevColumn && !pPrevColumn->GetDrawHeaderDivider())
			{
				DrawVerticalLine(pDC, rcColumn.left - 1, rcColumn.top, rcColumn.Height() - 1, m_clrHeaderControl);
				pDC->SetPixel(rcColumn.left - 1, rcColumn.bottom - 1, m_clrControlDark);
			}
		}
	}
	bColumnPressed = FALSE;
}

void CXTPTrackThemeExplorer::DrawSectionDivider(
	CDC *pDC, const CRect& rc,
	XTPReportSectionDividerPosition dividerPosition,
	XTPReportSectionDividerStyle    dividerStyle,
	CXTPPaintManagerColor           clrDivider)
{
	if (dividerStyle == xtpReportFixedRowsDividerOutlook)
	{
		pDC->FillSolidRect(rc, m_clrHeaderControl);
		pDC->Draw3dRect(rc, m_clrControlLightLight, m_clrBtnText);

		CRect rcTmp(rc);
		rcTmp.DeflateRect(1, 1);
		pDC->Draw3dRect(rcTmp, m_clrHeaderControl, m_clrControlDark);
	}
	else
	{
		CXTPTrackPaintManager::DrawSectionDivider(pDC, rc, dividerPosition, dividerStyle, clrDivider);
	}
}
