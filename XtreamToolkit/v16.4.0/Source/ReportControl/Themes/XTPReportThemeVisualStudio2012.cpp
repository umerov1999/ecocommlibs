// XTPReportThemeVisualStudio2012.cpp : implementation of the CXTPReportThemeOffice2013 class.
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

#include <ReportControl/Themes/XTPReportThemeOffice2013.h>
#include <ReportControl/Themes/XTPReportThemeVisualStudio2012.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// class: CXTPReportThemeVisualStudio2012Light
/////////////////////////////////////////////////////////////////////////////

CXTPReportThemeVisualStudio2012Light::CXTPReportThemeVisualStudio2012Light()
{
	m_bMakeUpper = FALSE;
	LoadGlyphs();
}

void CXTPReportThemeVisualStudio2012Light::LoadGlyphs()
{
	CMDTARGET_RELEASE(m_pGlyphs);
	m_pGlyphs = new CXTPImageManager();

	CBitmap bmp;
	CXTPResourceManager::AssertValid(XTPResourceManager()->LoadBitmap(&bmp, XTP_IDB_REPORT_GLYPHS_VS2012_LIGHT));

	CXTPTempColorMask mask(m_pGlyphs, RGB(255, 0, 255));
	m_pGlyphs->SetIcons(bmp, 0, xtpReportGlyphCount, CSize(0, 0)); //11
}

void CXTPReportThemeVisualStudio2012Light::RefreshMetrics()
{
	CMDTARGET_RELEASE(m_pHyperlinkStyle);
	m_pHyperlinkStyle = new CXTPReportHyperlinkStyle(RGB(19,100,196), xtpReportTextDecorationNone);

	CMDTARGET_RELEASE(m_pHyperlinkHoverStyle);
	m_pHyperlinkHoverStyle = new CXTPReportHyperlinkStyle(RGB(51,153,255), xtpReportTextDecorationNone);

	m_bInvertColumnOnClick = FALSE;

	m_clrBtnFace.SetStandardValue(RGB(246,246,246));
	m_clrBtnFacePressed.SetStandardValue(RGB(0,122,204));
	m_clrBtnText.SetStandardValue(RGB(30,30,30));
	m_clrControlDark.SetStandardValue(RGB(204,206,219));
	m_clrControlLightLight.SetStandardValue(m_clrBtnFace);
	m_clrHighlight.SetStandardValue(RGB(51,153,255));
	m_clrHighlightText.SetStandardValue(RGB(255,255,255));
	m_clrBoldText.SetStandardValue(m_clrBtnText);
	m_clrBoldTextHighlight.SetStandardValue(m_clrHighlightText);
	m_clrGroupText.SetStandardValue(m_clrBtnText);
	m_clrGroupTextHilite.SetStandardValue(m_clrHighlightText);
	m_clrGroupBack.SetStandardValue(RGB(239,239,242));
	m_clrGroupBackHilite.SetStandardValue(m_clrHighlight);
	m_clrSelectedRow.SetStandardValue(m_clrGroupBack);
	m_clrSelectedRowText.SetStandardValue(m_clrBtnText);
	m_clrItemShade.SetStandardValue(RGB(230,231,232));
	m_clrHotDivider.SetStandardValue(m_clrHighlight);
	m_clrFreezeColsDivider.SetStandardValue(RGB(134,137,153));

	m_clrWindowText.SetStandardValue(m_clrBtnText);
	m_clrControlBack.SetStandardValue(m_clrBtnFace);
	m_clrConnector.SetStandardValue(m_clrControlDark);
	m_clrIndentControl.SetStandardValue(m_clrBtnFace);
	m_clrCaptionText.SetStandardValue(m_clrBtnText);
	m_clrCaptionTextPressed.SetStandardValue(m_clrHighlightText);
	m_clrHeaderControl.SetStandardValue(m_clrGroupBack);
	m_clrGroupShadeBorder.SetStandardValue(m_clrControlDark);
	m_clrGroupShadeBack.SetStandardValue(m_clrBtnFace);
	m_clrGroupShadeText.SetStandardValue(m_clrBtnText);
	m_clrGroupRowText.SetStandardValue(m_clrControlDark);
	m_clrGroupBoxBack.SetStandardValue(m_clrGroupBack);
	m_crlNoGroupByText.SetStandardValue(m_clrControlDark);
	m_clrGradientColumnSeparator.SetStandardValue(m_clrBtnText);
	m_clrGlyph.SetStandardValue(m_clrBtnText);
	m_clrGlyphHighlight.SetStandardValue(m_clrHighlightText);
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

/////////////////////////////////////////////////////////////////////////////
// class: CXTPReportThemeVisualStudio2012Dark
/////////////////////////////////////////////////////////////////////////////

CXTPReportThemeVisualStudio2012Dark::CXTPReportThemeVisualStudio2012Dark()
{
	m_bMakeUpper = FALSE;
	LoadGlyphs();
}

void CXTPReportThemeVisualStudio2012Dark::LoadGlyphs()
{
	CMDTARGET_RELEASE(m_pGlyphs);
	m_pGlyphs = new CXTPImageManager();

	CBitmap bmp;
	CXTPResourceManager::AssertValid(XTPResourceManager()->LoadBitmap(&bmp, XTP_IDB_REPORT_GLYPHS_VS2012_DARK));

	CXTPTempColorMask mask(m_pGlyphs, RGB(255, 0, 255));
	m_pGlyphs->SetIcons(bmp, 0, xtpReportGlyphCount, CSize(0, 0)); //11
}

void CXTPReportThemeVisualStudio2012Dark::RefreshMetrics()
{
	CMDTARGET_RELEASE(m_pHyperlinkStyle);
	m_pHyperlinkStyle = new CXTPReportHyperlinkStyle(RGB(51,153,255), xtpReportTextDecorationNone);

	CMDTARGET_RELEASE(m_pHyperlinkHoverStyle);
	m_pHyperlinkHoverStyle = new CXTPReportHyperlinkStyle(RGB(255,255,255), xtpReportTextDecorationNone);

	m_bInvertColumnOnClick = FALSE;

	m_clrBtnFace.SetStandardValue(RGB(37,37,38));
	m_clrBtnFacePressed.SetStandardValue(RGB(0,122,204));
	m_clrBtnText.SetStandardValue(RGB(153,153,153));
	m_clrControlDark.SetStandardValue(RGB(45,45,48));
	m_clrControlLightLight.SetStandardValue(m_clrBtnFace);
	m_clrHighlight.SetStandardValue(RGB(51,153,255));
	m_clrHighlightText.SetStandardValue(RGB(255,255,255));
	m_clrBoldText.SetStandardValue(m_clrBtnText);
	m_clrBoldTextHighlight.SetStandardValue(m_clrHighlightText);
	m_clrGroupText.SetStandardValue(m_clrBtnText);
	m_clrGroupTextHilite.SetStandardValue(m_clrHighlightText);
	m_clrGroupBack.SetStandardValue(m_clrControlDark);
	m_clrGroupBackHilite.SetStandardValue(m_clrHighlight);
	m_clrSelectedRow.SetStandardValue(m_clrControlDark);
	m_clrSelectedRowText.SetStandardValue(m_clrBtnText);
	m_clrItemShade.SetStandardValue(RGB(51,51,51));
	m_clrHotDivider.SetStandardValue(m_clrHighlight);
	m_clrFreezeColsDivider.SetStandardValue(m_clrBtnText);

	m_clrWindowText.SetStandardValue(m_clrBtnText);
	m_clrControlBack.SetStandardValue(m_clrBtnFace);
	m_clrConnector.SetStandardValue(m_clrControlDark);
	m_clrIndentControl.SetStandardValue(m_clrBtnFace);
	m_clrCaptionText.SetStandardValue(RGB(208,208,208));
	m_clrCaptionTextPressed.SetStandardValue(m_clrHighlightText);
	m_clrHeaderControl.SetStandardValue(RGB(62,62,66));
	m_clrGroupShadeBorder.SetStandardValue(m_clrControlDark);
	m_clrGroupShadeBack.SetStandardValue(m_clrBtnFace);
	m_clrGroupShadeText.SetStandardValue(m_clrBtnText);
	m_clrGroupRowText.SetStandardValue(m_clrControlDark);
	m_clrGroupBoxBack.SetStandardValue(m_clrControlDark);
	m_crlNoGroupByText.SetStandardValue(m_clrControlDark);
	m_clrGradientColumnSeparator.SetStandardValue(RGB(241,241,241));
	m_clrGlyph.SetStandardValue(m_clrGradientColumnSeparator);
	m_clrGlyphHighlight.SetStandardValue(m_clrHighlightText);
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

/////////////////////////////////////////////////////////////////////////////
// class: CXTPReportThemeVisualStudio2012Blue
/////////////////////////////////////////////////////////////////////////////

CXTPReportThemeVisualStudio2012Blue::CXTPReportThemeVisualStudio2012Blue()
{
	m_bMakeUpper = FALSE;
	LoadGlyphs();
}

void CXTPReportThemeVisualStudio2012Blue::LoadGlyphs()
{
	CMDTARGET_RELEASE(m_pGlyphs);
	m_pGlyphs = new CXTPImageManager();

	CBitmap bmp;
	CXTPResourceManager::AssertValid(XTPResourceManager()->LoadBitmap(&bmp, XTP_IDB_REPORT_GLYPHS_VS2012_BLUE));

	CXTPTempColorMask mask(m_pGlyphs, RGB(255, 0, 255));
	m_pGlyphs->SetIcons(bmp, 0, xtpReportGlyphCount, CSize(0, 0)); //11
}

void CXTPReportThemeVisualStudio2012Blue::RefreshMetrics()
{
	CMDTARGET_RELEASE(m_pHyperlinkStyle);
	m_pHyperlinkStyle = new CXTPReportHyperlinkStyle(RGB(19,100,196), xtpReportTextDecorationNone);

	CMDTARGET_RELEASE(m_pHyperlinkHoverStyle);
	m_pHyperlinkHoverStyle = new CXTPReportHyperlinkStyle(RGB(51,153,255), xtpReportTextDecorationNone);

	m_bInvertColumnOnClick = FALSE;

	m_clrBtnFace.SetStandardValue(RGB(255,255,255));
	m_clrBtnFacePressed.SetStandardValue(RGB(255,240,208));
	m_clrBtnText.SetStandardValue(RGB(30,30,30));
	m_clrControlDark.SetStandardValue(RGB(41,57,85));
	m_clrControlLightLight.SetStandardValue(m_clrBtnFace);
	m_clrHighlight.SetStandardValue(RGB(51,153,255));
	m_clrHighlightText.SetStandardValue(m_clrBtnFace);
	m_clrBoldText.SetStandardValue(m_clrBtnText);
	m_clrBoldTextHighlight.SetStandardValue(m_clrHighlightText);
	m_clrGroupText.SetStandardValue(m_clrBtnText);
	m_clrGroupTextHilite.SetStandardValue(m_clrHighlightText);
	m_clrGroupBack.SetStandardValue(RGB(239,239,242));
	m_clrGroupBackHilite.SetStandardValue(m_clrHighlight);
	m_clrSelectedRow.SetStandardValue(m_clrGroupBack);
	m_clrSelectedRowText.SetStandardValue(m_clrBtnText);
	m_clrItemShade.SetStandardValue(RGB(241,243,248));
	m_clrHotDivider.SetStandardValue(m_clrHighlight);
	m_clrFreezeColsDivider.SetStandardValue(RGB(43,59,88));

	m_clrWindowText.SetStandardValue(m_clrBtnText);
	m_clrControlBack.SetStandardValue(m_clrBtnFace);
	m_clrConnector.SetStandardValue(m_clrControlDark);
	m_clrIndentControl.SetStandardValue(m_clrBtnFace);
	m_clrCaptionText.SetStandardValue(m_clrBtnFace);
	m_clrCaptionTextPressed.SetStandardValue(RGB(117,99,61));
	m_clrHeaderControl.SetStandardValue(RGB(77,96,130));
	m_clrGroupShadeBorder.SetStandardValue(m_clrControlDark);
	m_clrGroupShadeBack.SetStandardValue(m_clrBtnFace);
	m_clrGroupShadeText.SetStandardValue(m_clrBtnText);
	m_clrGroupRowText.SetStandardValue(m_clrControlDark);
	m_clrGroupBoxBack.SetStandardValue(m_clrControlDark);
	m_crlNoGroupByText.SetStandardValue(m_clrControlDark);
	m_clrGradientColumnSeparator.SetStandardValue(RGB(206,212,221));
	m_clrGlyph.SetStandardValue(m_clrGradientColumnSeparator);
	m_clrGlyphHighlight.SetStandardValue(m_clrCaptionTextPressed);
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
