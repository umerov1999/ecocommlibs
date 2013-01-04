// XTPReportPaintManager.cpp : implementation of the CXTPReportPaintManager class.
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
#include "Resource.h"

#include "Common/XTPResourceManager.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPImageManager.h"
#include "Common/XTPResourceImage.h"
#include "Common/XTPMarkupRender.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPWinThemeWrapper.h"
#include "Common/XTPCustomHeap.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPSmartPtrInternalT.h"

#include "XTPReportDefines.h"
#include "XTPReportPaintManager.h"
#include "XTPReportRow.h"
#include "XTPReportRows.h"
#include "XTPReportGroupRow.h"
#include "XTPReportColumn.h"
#include "XTPReportHeader.h"
#include "XTPReportControl.h"
#include "XTPReportRecordItem.h"
#include "XTPReportInplaceControls.h"
#include "XTPReportRecordItemControls.h"
#include "XTPReportColumns.h"
#include "XTPReportRecord.h"
#include "XTPReportHyperlink.h"
#include "XTPReportSection.h"
#include "XTPReportPageSetupDialog.h"
#include "XTPReportBorder.h"
#include "XTPReportThickness.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

#define XTP_HLINK_SPACE_X   ((TCHAR)1)

AFX_INLINE int GetTextExtentX(CDC* pDC, const CString& strString)
{
	CString str(strString);
	str.Replace(XTP_HLINK_SPACE_X, _T(' '));

	int nExt1 = pDC->GetTextExtent(str).cx;
	if (pDC->IsPrinting())
	{
		int nExt2 = pDC->GetOutputTextExtent(str).cx;
		return max(nExt1, nExt2);
	}
	return nExt1;
}

AFX_INLINE int GetTextExtentX(CDC* pDC, LPTSTR pszString, int nCount)
{
	CString str(pszString, nCount);
	str.Replace(XTP_HLINK_SPACE_X, _T(' '));

	int nExt1 = pDC->GetTextExtent(str).cx;
	if (pDC->IsPrinting())
	{
		int nExt2 = pDC->GetOutputTextExtent(str).cx;
		return max(nExt1, nExt2);
	}
	return nExt1;
}


//////////////////////////////////////////////////////////////////////
// CXTPReportPaintManager

CXTPReportPaintManager::CXTPReportPaintManager()
	: m_nTreeTextIndentRowParent  (3)
	, m_nTreeTextIndentRowChildren(5)
	, m_nTreeTextIndentGroupRow   (5)

	, m_pControl(NULL)
	, m_pBorder (NULL)
{
	m_pHyperlinkStyle = new CXTPReportHyperlinkStyle(RGB(0, 0, 255), xtpReportTextDecorationNone);
	m_pHyperlinkHoverStyle = new CXTPReportHyperlinkStyle(RGB(0, 0, 255), xtpReportTextDecorationUnderline);

	m_themeHeader = new CXTPWinThemeWrapper();
	m_themeButton = new CXTPWinThemeWrapper();
	m_themeCombo = new CXTPWinThemeWrapper();
	m_themeSpin = new CXTPWinThemeWrapper();

	// Set up default border
	m_pBorder = new CXTPReportBorder();
	m_pBorder->GetBorderThickness()->Set(0,0,0,1);
	m_pBorder->GetEdge(xtpReportBorderEdgeBottom)->SetLineStyle(xtpReportLineStyleSolid);

	m_pMarkupUIElement_NoGroupBy = NULL;

	m_clrPreviewText.SetStandardValue(RGB(0, 0, 255));
	//m_clrColumnOffice2007CustomTheme.SetStandardValue(RGB(196, 221, 255));
	m_clrColumnOffice2007CustomTheme = COLORREF_NULL;
	m_clrFormulaSum = RGB(255,0,0); // Color for Formula Sum fields
	m_clrFormulaSubSum = RGB(0,0,255); // Color for Formula SubSum fields
	m_clrAlternativeBackground = RGB(224,224,224); // Color for Alternative Row Background
	m_bUseAlternativeBackground = FALSE;

	m_nMaxPreviewLines = 3;
	m_szGlyph = CSize(12, 12);

	m_bThemedInplaceButtons = TRUE;

	// defaults
	m_nGroupGridLineHeight = 2;
	m_nResizingWidth = 1;
	m_bIsPreviewMode = FALSE;
	m_bShadeGroupHeadings = FALSE;
	m_bGroupRowTextBold = FALSE;
	m_bUseGroupRowSelectionState = FALSE;
	m_bPrintGroupRowIcon = TRUE;
	m_bShadeSortColumn = TRUE;
	m_columnStyle = xtpReportColumnShaded;

	m_treeStructureStyle = xtpReportTreeStructureSolid;

	m_nGroupGridLineStyle = xtpReportGridSolid;

	m_nRowHeightDefault = 18;
	m_nFixedColumnsIndent = 0;
	m_bMoveScrollbarOnFixedColumnsIndent = FALSE;
	m_bBaseTreeIndent = FALSE;

	m_nTextIndentForGroupRowsWithNoIcon = -1;

	m_bRevertAlignment = FALSE;
	m_bUseEditTextAlignment = TRUE;
	m_bUseColumnTextAlignment = FALSE;
	m_bHotTracking = TRUE;
	m_bInvertColumnOnClick = TRUE;
	m_bColumnPlusMinusSign = FALSE;
	m_RecordNumberWidth = 20;
	m_bShowLockIcon = FALSE;
	m_bUseShellIcon = FALSE;
	m_bShowNonActiveInPlaceButton = FALSE;
	m_bForceShowTooltip = FALSE;
	m_bCleanTooltip = FALSE;
	m_bGrayIfDisable = TRUE;
	m_bRecOrRowNum = TRUE;
	m_iRecOrRowStart = 0;

	m_bSortTriangleOffsetFromRight = FALSE;

	m_bUseDropMarker = TRUE;

	m_nTreeColumnLineLength = 0;
	m_nBaseParentRecordIndent = 0;

	CXTPResourceManager::AssertValid(XTPResourceManager()->LoadString(&m_strNoItems, XTP_IDS_REPORT_NOITEMS));
	CXTPResourceManager::AssertValid(XTPResourceManager()->LoadString(&m_strNoGroupBy, XTP_IDS_REPORT_NOGROUPBY));
	CXTPResourceManager::AssertValid(XTPResourceManager()->LoadString(&m_strNoFieldsAvailable, XTP_IDS_REPORT_NOFIELDSAVAILABLE));

	if (XTPResourceManager()->LoadString(&m_strSortBy, XTP_IDS_REPORT_SORTBY))
		m_strSortBy += _T(" ");

	CXTPResourceManager::AssertValid(XTPResourceManager()->LoadString(&m_strIncreasing, XTP_IDS_REPORT_INCREASING));
	CXTPResourceManager::AssertValid(XTPResourceManager()->LoadString(&m_strDecreasing, XTP_IDS_REPORT_DECREASING));
	CXTPResourceManager::AssertValid(XTPResourceManager()->LoadString(&m_strExpand, XTP_IDS_REPORT_EXPAND));
	CXTPResourceManager::AssertValid(XTPResourceManager()->LoadString(&m_strCollapse, XTP_IDS_REPORT_COLLAPSE));

	m_bHideSelection = FALSE;
	m_rcPreviewIndent.SetRect(10, 0, 10, 2);

	m_nTreeIndent = 20;
	m_bPrintSortTriangle = TRUE ;

	m_DrawSortTriangleStyle = xtpReportDrawSortTriangleDefault;

	// get system parameters
	LOGFONT lfIcon;
	VERIFY(CXTPDrawHelpers::GetIconLogFont(&lfIcon));

	VERIFY(m_fontPreview.CreateFontIndirect(&lfIcon));

	SetCaptionFont(lfIcon);
	SetTextFont(lfIcon);

	CBitmap bmp;
	CXTPResourceManager::AssertValid(XTPResourceManager()->LoadBitmap(&bmp, XTP_IDB_REPORT_GLYPHS));

	m_pGlyphs = new CXTPImageManager();
	CXTPTempColorMask mask(m_pGlyphs, RGB(255, 0, 255));
	m_pGlyphs->SetIcons(bmp, 0, 11, CSize(0, 0)); //11

	m_bFixedRowHeight = TRUE;
	m_bColumnWidthWYSIWYG = FALSE;
	m_bLastColumnWidthWYSIWYG = FALSE;
	m_bShowWYSIWYGMarkers = TRUE;
	m_bPrintVirtualPageNumber = FALSE;
	m_bPrintPageRectangle = TRUE;
	m_bPrintWatermark = FALSE;
	m_PrintPageWidth = 0;
	m_bDrawGridForEmptySpace = FALSE;

	m_bFixedInplaceButtonHeight = FALSE;
	m_nFreezeColsDividerStyle = xtpReportFreezeColsDividerThin | xtpReportFreezeColsDividerShade | xtpReportFreezeColsDividerHeader;

	m_nTreeStructureStyle = 0;
	m_bForceDynamicMarkupForCell = FALSE;
	m_bForceShowDropMarker = FALSE;
	m_bCompactText = FALSE;
	m_bAllowMergeCells = FALSE;

	RefreshMetrics();

}

CXTPReportPaintManager::~CXTPReportPaintManager()
{
	CMDTARGET_RELEASE(m_pGlyphs);

	CMDTARGET_RELEASE(m_pHyperlinkStyle);
	CMDTARGET_RELEASE(m_pHyperlinkHoverStyle);

	XTPMarkupReleaseElement(m_pMarkupUIElement_NoGroupBy);

	SAFE_DELETE(m_themeHeader);
	SAFE_DELETE(m_themeButton);
	SAFE_DELETE(m_themeCombo);
	SAFE_DELETE(m_themeSpin);

	SAFE_DELETE(m_pBorder);
}

COLORREF CXTPReportPaintManager::GetControlBackColor(CXTPReportControl* /*pControl*/)
{
	return m_clrControlBack;
}


void CXTPReportPaintManager::RefreshMetrics()
{
	RefreshXtremeColors();

	m_clrBtnFace.SetStandardValue(GetXtremeColor(COLOR_BTNFACE));
	m_clrBtnText.SetStandardValue(GetXtremeColor(COLOR_BTNTEXT));
	m_clrControlDark.SetStandardValue(GetXtremeColor(COLOR_3DSHADOW));
	m_clrHighlight.SetStandardValue(GetXtremeColor(COLOR_HIGHLIGHT));
	m_clrHighlightText.SetStandardValue(GetXtremeColor(COLOR_HIGHLIGHTTEXT));
	m_clrWindowText.SetStandardValue(GetXtremeColor(COLOR_WINDOWTEXT));
	m_clrControlLightLight.SetStandardValue(GetXtremeColor(COLOR_3DHIGHLIGHT));
	m_clrControlBack.SetStandardValue(GetXtremeColor(COLOR_WINDOW));

	m_clrIndentControl.SetStandardValue(m_clrBtnFace);
	m_clrCaptionText.SetStandardValue(m_clrBtnText);
	SetGridColor(m_clrControlDark);
	m_clrHeaderControl.SetStandardValue(m_clrBtnFace);
	m_clrGroupShadeBorder.SetStandardValue(m_clrControlDark);
	m_clrGroupShadeBack.SetStandardValue(m_clrBtnFace);
	m_clrGroupShadeText.SetStandardValue(m_clrBtnText);
	m_clrGroupRowText.SetStandardValue(m_clrControlDark);
	m_clrGroupBoxBack.SetStandardValue(m_clrControlDark);

	m_clrSelectedRow.SetStandardValue(m_clrBtnFace);
	m_clrSelectedRowText.SetStandardValue(m_clrBtnText);

	m_clrItemShade.SetStandardValue(RGB(245, 245, 245));
	m_clrHotDivider.SetStandardValue(RGB(0xFF, 0, 0));

	m_crlNoGroupByText.SetStandardValue(m_clrControlDark);

	m_clrGradientColumnSeparator.SetStandardValue(m_clrControlDark);

	m_themeHeader->OpenThemeData(0, L"HEADER");
	m_themeButton->OpenThemeData(0, L"BUTTON");
	m_themeCombo->OpenThemeData(0, L"COMBOBOX");
	m_themeSpin->OpenThemeData(0, L"SPIN");

	m_nGroupGridLineStyle = xtpReportGridSolid;

	if (!XTPColorManager()->IsLunaColorsDisabled())
	{

		XTPCurrentSystemTheme systemTheme = XTPColorManager()->GetCurrentSystemTheme();

		switch (systemTheme)
		{
			case xtpSystemThemeBlue:
			case xtpSystemThemeRoyale:
			case xtpSystemThemeAero:
				m_clrGroupShadeBorder.SetStandardValue(RGB(123, 164, 224));
				m_clrIndentControl.SetStandardValue(RGB(253, 238, 201));
				m_clrGroupShadeBack.SetStandardValue(RGB(190, 218, 251));
				m_clrGroupRowText.SetStandardValue(RGB(55, 104, 185));
				SetGridColor(RGB(234, 233, 225));
				break;

			case xtpSystemThemeOlive:
				m_clrGroupShadeBorder.SetStandardValue(RGB(181, 196, 143));
				m_clrIndentControl.SetStandardValue(RGB(253, 238, 201));
				m_clrGroupShadeBack.SetStandardValue(RGB(175, 186, 145));
				m_clrGroupRowText.SetStandardValue(RGB(115, 137, 84));
				SetGridColor(RGB(234, 233, 225));
				break;

			case xtpSystemThemeSilver:
				m_clrGroupShadeBorder.SetStandardValue(RGB(165, 164, 189));
				m_clrIndentControl.SetStandardValue(RGB(253, 238, 201));
				m_clrGroupShadeBack.SetStandardValue(RGB(229, 229, 235));
				m_clrGroupRowText.SetStandardValue(RGB(112, 111, 145));
				SetGridColor(RGB(229, 229, 235));
				break;
		}
	}

	if (m_columnStyle == xtpReportColumnResource)
	{
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
	}
	else if (m_columnStyle == xtpReportColumnOffice2003)
	{
		m_grcGradientColumn.SetStandardValue(XTPColorManager()->LightColor(GetXtremeColor(COLOR_3DFACE), GetXtremeColor(COLOR_WINDOW), 0xcd), GetXtremeColor(COLOR_3DFACE));
		m_grcGradientColumnHot.SetStandardValue(GetXtremeColor(XPCOLOR_HIGHLIGHT));
		m_grcGradientColumnPushed.SetStandardValue(GetXtremeColor(XPCOLOR_HIGHLIGHT_PUSHED));
		m_clrGradientColumnShadow.SetStandardValue(GetXtremeColor(XPCOLOR_TOOLBAR_FACE));
		m_clrGradientColumnSeparator.SetStandardValue(GetXtremeColor(XPCOLOR_SEPARATOR));

		switch (XTPColorManager()->GetCurrentSystemTheme())
		{
		case xtpSystemThemeBlue:
		case xtpSystemThemeRoyale:
		case xtpSystemThemeAero:
			m_grcGradientColumn.SetStandardValue(RGB(221, 236, 254), RGB(129, 169, 226));
			m_clrGradientColumnShadow.SetStandardValue(RGB(59, 97, 156));
			m_clrGradientColumnSeparator.SetStandardValue(RGB(106, 140, 203));
			m_grcGradientColumnPushed.SetStandardValue(RGB(254, 142, 75), RGB(255, 207, 139));
			m_grcGradientColumnHot.SetStandardValue(RGB(255, 242, 200), RGB(255, 212, 151));
			break;

		case xtpSystemThemeOlive:
			m_grcGradientColumn.SetStandardValue(RGB(244, 247, 222), RGB(183, 198, 145));
			m_clrGradientColumnShadow.SetStandardValue(RGB(96, 128, 88));
			m_clrGradientColumnSeparator.SetStandardValue(RGB(96, 128, 88));
			m_grcGradientColumnPushed.SetStandardValue(RGB(254, 142, 75), RGB(255, 207, 139));
			m_grcGradientColumnHot.SetStandardValue(RGB(255, 242, 200), RGB(255, 212, 151));
			break;

		case xtpSystemThemeSilver:
			m_grcGradientColumn.SetStandardValue(RGB(243, 244, 250), RGB(153, 151, 181));
			m_clrGradientColumnShadow.SetStandardValue(RGB(124, 124, 148));
			m_clrGradientColumnSeparator.SetStandardValue(RGB(110, 109, 143));
			m_grcGradientColumnPushed.SetStandardValue(RGB(254, 142, 75), RGB(255, 207, 139));
			m_grcGradientColumnHot.SetStandardValue(RGB(255, 242, 200), RGB(255, 212, 151));
			break;
		}
	}

	m_brushVeriticalGrid.DeleteObject();
	m_brushHorizontalGrid.DeleteObject();
	m_brushTreeStructure.DeleteObject();
	m_brushGroupGridLineStyle.DeleteObject();

	m_clrFreezeColsDivider.SetStandardValue(RGB(0, 0, 222));

	LOGFONT lf;
	m_fontCaption.GetLogFont(&lf);
	SetCaptionFont(lf);
}


void CXTPReportPaintManager::FillRow(CDC* pDC, CXTPReportRow* pRow, CRect rcRow)
{
	if (m_bUseAlternativeBackground && pRow->GetIndex() % 2 == 1)
		pDC->FillSolidRect(rcRow, m_clrAlternativeBackground);
}

void CXTPReportPaintManager::DrawGridPattern(CDC* pDC, CBrush *pBrush, CRect rc, const void *pPattern, COLORREF clr)
{
	if (pBrush->GetSafeHandle() == 0)
	{
		CBitmap bmp;
		bmp.CreateBitmap(8, 8, 1, 1, pPattern);
		pBrush->CreatePatternBrush(&bmp);
	}
	CBrush* pOldBrush = (CBrush*)pDC->SelectObject(pBrush);

	COLORREF clrOldText = pDC->SetTextColor(m_clrControlBack);
	COLORREF clrOldBk = pDC->SetBkColor(clr);

	PatBlt(pDC->GetSafeHdc(), rc.left, rc.top, rc.Width(), rc.Height(), PATCOPY);

	pDC->SetTextColor(clrOldText);
	pDC->SetBkColor(clrOldBk);

	pDC->SelectObject(pOldBrush);
}

void CreateLineStyleBrush(CBrush *pBrush, const void *pPattern)
{
	if (NULL == pBrush->GetSafeHandle())
	{
		CBitmap bmp;
		bmp.CreateBitmap(8, 8, 1, 1, pPattern);
		pBrush->CreatePatternBrush(&bmp);
	}
}

void CXTPReportPaintManager::CreateLineStyleBrushes()
{
	// Horizontal
	static const unsigned short _DotsSmallH[]   = {0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x55};
	static const unsigned short _DotsLargeH[]   = {0xcc, 0x33, 0xcc, 0x33, 0xcc, 0x33, 0xcc, 0x33};
	static const unsigned short _DashH[]        = {0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0};

	CreateLineStyleBrush(&m_brushDotsSmallH,    _DotsSmallH);
	CreateLineStyleBrush(&m_brushDotsLargeH,    _DotsLargeH);
	CreateLineStyleBrush(&m_brushDashH,         _DashH);

	// Vertical
}

XTPReportLineStyle CXTPReportPaintManager::GetGridStyle(BOOL bVertical) const
{
	CXTPReportBorder *pBorder = GetDefaultItemBorder();

	if (bVertical)
	{
		return pBorder->GetEdge(xtpReportBorderEdgeRight)->GetLineStyle();
	}
	else
	{
		return pBorder->GetEdge(xtpReportBorderEdgeBottom)->GetLineStyle();
	}
}

void CXTPReportPaintManager::SetGridStyle(BOOL bVertical, XTPReportLineStyle lineStyle)
{
	CXTPReportBorder *pBorder = GetDefaultItemBorder();

	if (bVertical)
	{
		switch(lineStyle)
		{
		case xtpReportLineStyleNone:
			pBorder->GetBorderThickness()->SetRight(0);
			break;
		default:
			pBorder->GetBorderThickness()->SetRight(1);
			break;
		}

		pBorder->GetEdge(xtpReportBorderEdgeRight)->SetLineStyle(lineStyle);
	}
	else
	{
		switch(lineStyle)
		{
		case xtpReportLineStyleNone:
			pBorder->GetBorderThickness()->SetBottom(0);
			break;
		default:
			pBorder->GetBorderThickness()->SetBottom(1);
			break;
		}

		pBorder->GetEdge(xtpReportBorderEdgeBottom)->SetLineStyle(lineStyle);
	}
}

void CXTPReportPaintManager::SetColumnStyle(XTPReportColumnStyle columnStyle)
{
	m_columnStyle = columnStyle;
	RefreshMetrics();
}

void CXTPReportPaintManager::DrawGrid(CDC *pDC, XTPReportOrientation orientation, CRect rcGrid)
{
	if (xtpReportOrientationHorizontal == orientation)
	{
		switch (GetGridStyle(FALSE))
		{
			case xtpReportGridSolid:
			{
				DrawHorizontalLine(pDC, rcGrid.left, rcGrid.bottom, rcGrid.Width(), GetGridColor());
				break;
			}
			case xtpReportGridSmallDots:
			{
				static const unsigned short _cb[] = {0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x55};
				DrawGridPattern(pDC, &m_brushHorizontalGrid, CRect(rcGrid.left, rcGrid.bottom, rcGrid.right, rcGrid.bottom + 1), _cb, GetGridColor());
				break;
			}
			case xtpReportGridLargeDots:
			{
				static const unsigned short _cb[] = {0xcc, 0x33, 0xcc, 0x33, 0xcc, 0x33, 0xcc, 0x33};
				DrawGridPattern(pDC, &m_brushHorizontalGrid, CRect(rcGrid.left, rcGrid.bottom, rcGrid.right, rcGrid.bottom + 1), _cb, GetGridColor());
				break;
			}
			case xtpReportGridDashes:
			{
				static const unsigned short _cb[] = {0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0};
				DrawGridPattern(pDC, &m_brushHorizontalGrid, CRect(rcGrid.left, rcGrid.bottom, rcGrid.right, rcGrid.bottom + 1), _cb, GetGridColor());
				break;
			}
		}
	}
	else
	{
		int nPos = rcGrid.right - 1;
		switch (GetGridStyle(TRUE))
		{
			case xtpReportGridSolid:
			{
				DrawVerticalLine(pDC, nPos, rcGrid.top, rcGrid.Height(), GetGridColor());
				break;
			}
			case xtpReportGridSmallDots:
			{
				static const unsigned short _cb[] = {0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x55};
				DrawGridPattern(pDC, &m_brushVeriticalGrid, CRect(nPos, rcGrid.top, nPos + 1, rcGrid.bottom), _cb, GetGridColor());
				break;
			}
			case xtpReportGridLargeDots:
			{
				static const unsigned short _cb[] = {0xcc, 0xcc, 0x33, 0x33, 0xcc, 0xcc, 0x33, 0x33};
				DrawGridPattern(pDC, &m_brushVeriticalGrid, CRect(nPos, rcGrid.top, nPos + 1, rcGrid.bottom), _cb, GetGridColor());
				break;
			}
			case xtpReportGridDashes:
			{
				static const unsigned short _cb[] = {0xf0, 0xf0, 0xf0, 0xf0, 0xf, 0xf, 0xf, 0xf};
				DrawGridPattern(pDC, &m_brushVeriticalGrid, CRect(nPos, rcGrid.top, nPos + 1, rcGrid.bottom), _cb, GetGridColor());
				break;
			}
		}
	}
}

void CXTPReportPaintManager::DrawFreezeColsDivider(CDC* pDC, const CRect& rc, CXTPReportControl* pControl, CXTPReportRow* pRow)
{
	BOOL bShowAlways = !!(m_nFreezeColsDividerStyle & xtpReportFreezeColsDividerShowAlways);

	if ((!bShowAlways && pControl->GetReportHeader()->IsAutoColumnSizing()) ||
		(m_nFreezeColsDividerStyle == 0))
	{
		m_nFixedColumnsIndent = 0;
		return;
	}

	CRect rcDvX = rc;

	if ((m_nFreezeColsDividerStyle & xtpReportFreezeColsDividerShade) &&
		pControl->GetScrollPos(SB_HORZ) && !pControl->IsFullColumnScrolling())
	{
		rcDvX.left = rcDvX.right - 1;
		rcDvX.right = rcDvX.left + 4;
		XTPDrawHelpers()->GradientFill(pDC, rcDvX, m_clrFreezeColsDivider,
			RGB(255, 255, 255), TRUE);
	}
	else if (m_nFreezeColsDividerStyle & (xtpReportFreezeColsDividerThin | xtpReportFreezeColsDividerBold))
	{
		int nWidth = (m_nFreezeColsDividerStyle & xtpReportFreezeColsDividerBold) ? 2 : 1;
		rcDvX.left = rcDvX.right - nWidth;
		pDC->FillSolidRect(&rcDvX, m_clrFreezeColsDivider);
	}
	int nMask = (xtpReportFreezeColsDividerThin | xtpReportFreezeColsDividerBold | xtpReportFreezeColsDividerShade);
	if (pRow && pRow->IsSelected() && (m_nFreezeColsDividerStyle & nMask))
	{
		pDC->InvertRect(&rcDvX);
	}
	m_nFixedColumnsIndent = rcDvX.left;
}

void CXTPReportPaintManager::DrawVerticalTimeLineMarker(CDC* pDC, int xPos, CXTPReportControl* pControl, COLORREF color)
{
	CRect rcClipBox = pControl->GetReportRectangle();
	rcClipBox.left = xPos;
	rcClipBox.right = xPos + 1;
	pDC->FillSolidRect(&rcClipBox, color);
}

void CXTPReportPaintManager::FillGroupRowMetrics(CXTPReportGroupRow* pRow,
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



void CXTPReportPaintManager::DrawGroupRow(CDC* pDC, CXTPReportGroupRow* pRow, CRect rcRow,
	XTP_REPORTRECORDITEM_METRICS* pMetrics)
{
	CXTPReportControl *pControl = pRow->GetControl();
	BOOL bControlFocused = pControl->HasFocus();

	if (pMetrics->clrBackground != XTP_REPORT_COLOR_DEFAULT)
	{
		pDC->FillSolidRect(rcRow, pMetrics->clrBackground);
	}

	pDC->SetTextColor(pMetrics->clrForeground);

	ASSERT(pMetrics->pFont);
	CXTPFontDC font(pDC, pMetrics->pFont);

	int nRowLevel = pRow->GetTreeDepth();
	int nIndent = pControl->GetIndent(nRowLevel + 1);

	CRect rcFocus(rcRow.left + nIndent, rcRow.top, rcRow.right, rcRow.bottom);

	if (m_nGroupGridLineStyle != xtpReportGridSolid)
	{
		const unsigned short _cb[] = {0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc};
		DrawGridPattern(pDC, &m_brushGroupGridLineStyle, CRect(rcRow.left, rcRow.bottom - 1, rcRow.right, rcRow.bottom), _cb, m_clrGroupShadeBorder);
	}
	else
	{
		pDC->FillSolidRect(rcRow.left, rcRow.bottom - m_nGroupGridLineHeight, rcRow.Width(), m_nGroupGridLineHeight, m_clrGroupShadeBorder);
		rcFocus.bottom --;
	}

	if (pRow->IsFocused() && bControlFocused && !pDC->IsPrinting() && pControl->IsRowFocusVisible())
	{
		DrawFocusedRow(pDC, rcFocus);
	}

	if (nIndent > 0)
	{
		FillIndent(pDC, CRect(rcRow.left, rcRow.top, rcRow.left + nIndent, rcRow.bottom));
	}

	rcRow.left += nIndent;

	int nBitmapOffset = 0;
	int nTextOffset   = 0;
	CSize szIcon(0, 0);

	int nGroupRowIconAlignment = pMetrics->nGroupRowIconAlignment;

	// check and set defaults if need
	if ((nGroupRowIconAlignment & xtpGroupRowIconHmask) == 0)
	{
		nGroupRowIconAlignment |= xtpGroupRowIconLeft;
	}

	if ((nGroupRowIconAlignment & xtpGroupRowIconVmask) == 0)
	{
		nGroupRowIconAlignment |= xtpGroupRowIconVCenter;
	}

	if (pMetrics->nGroupRowIcon != XTP_REPORT_NOICON)
	{
		CXTPImageManagerIcon *pIcon = pControl->GetImageManager()->GetImage(pMetrics->nGroupRowIcon, 0);

		if (pIcon)
		{
			szIcon = CSize(pIcon->GetWidth(), pIcon->GetHeight());
		}

		if (nGroupRowIconAlignment & xtpGroupRowIconLeft)
		{
			nBitmapOffset = szIcon.cx + 5;
		}

		if (nGroupRowIconAlignment & xtpGroupRowIconBeforeText)
		{
			nTextOffset = szIcon.cx + 10;
		}
	}

	CRect rcBitmap(rcRow);
	rcBitmap.top   = rcBitmap.CenterPoint().y - m_szGlyph.cy / 2;
	rcBitmap.left += nBitmapOffset;

	int nBitmapWidth(0);
	if ((!pDC->IsPrinting() || m_bPrintGroupRowIcon) && !pRow->IsLockExpand())
	{
		nBitmapWidth = DrawCollapsedBitmap(pDC, pRow, rcBitmap).cx;
	}
	else
	{
		rcBitmap.right = rcBitmap.left;
	}

	if (!pDC->IsPrinting())
	{
		pRow->SetCollapseRect(rcBitmap);
	}

	int nNoIconWidth = 0;

	if (pRow->IsExpanded() && m_pGlyphs->GetImage(0) == 0 && m_nTextIndentForGroupRowsWithNoIcon != -1 && pMetrics->nGroupRowIcon == XTP_REPORT_NOICON)
		nNoIconWidth = m_nTextIndentForGroupRowsWithNoIcon;

	else if (pRow->IsExpanded() == FALSE && m_pGlyphs->GetImage(1) == 0 && m_nTextIndentForGroupRowsWithNoIcon != -1 && pMetrics->nGroupRowIcon == XTP_REPORT_NOICON)
		nNoIconWidth = m_nTextIndentForGroupRowsWithNoIcon;

	CRect rcText(rcBitmap.left, rcRow.top, rcRow.right, rcRow.bottom);
	rcText.top = rcText.bottom - rcRow.Height()/2 - 7;
	rcText.left += nBitmapWidth;
	rcText.left += nNoIconWidth;
	rcText.left += nTextOffset;
	rcText.left += m_nTreeTextIndentGroupRow;

	int nRightMask = (xtpGroupRowIconAfterText | xtpGroupRowIconRight);
	if (pMetrics->nGroupRowIcon != XTP_REPORT_NOICON &&
		(nGroupRowIconAlignment & nRightMask))
		rcText.right -= szIcon.cx;

	if (m_bUseGroupRowSelectionState && !pDC->IsPrinting())
		rcText.left += (szIcon.cx + 10);

	BOOL bDrawn = FALSE;
	int nTextWidth = 0;

	if (pMetrics->strText != pRow->GetCaption() && pControl->GetMarkupContext())
	{
		CXTPMarkupUIElement* pMarkupUIElement = XTPMarkupParseText(pControl->GetMarkupContext(), pMetrics->strText);
		if (pMarkupUIElement)
		{

			XTPMarkupSetDefaultFont(XTPMarkupElementContext(pMarkupUIElement), (HFONT)pMetrics->pFont->GetSafeHandle(), pMetrics->clrForeground);

			nTextWidth = XTPMarkupMeasureElement(pMarkupUIElement, rcText.Width(), INT_MAX).cx;
			XTPMarkupRenderElement(pMarkupUIElement, pDC->GetSafeHdc(), &rcText);

			XTPMarkupReleaseElement(pMarkupUIElement);
			bDrawn = TRUE;
		}
	}

	if (!bDrawn && pRow->m_pMarkupUIElement)
	{
		XTPMarkupSetDefaultFont(XTPMarkupElementContext(pRow->m_pMarkupUIElement), (HFONT)pMetrics->pFont->GetSafeHandle(), pMetrics->clrForeground);

		nTextWidth = XTPMarkupMeasureElement(pRow->m_pMarkupUIElement, rcText.Width(), INT_MAX).cx;

		XTPMarkupRenderElement(pRow->m_pMarkupUIElement, pDC->GetSafeHdc(), &rcText);

		bDrawn = TRUE;
	}

	if (!bDrawn)
	{
		UINT nFlags = DT_SINGLELINE | DT_END_ELLIPSIS | DT_VCENTER | DT_NOPREFIX;
		CString sSource = pMetrics->strText;

		// If no group formula is present then do not process "["
		int pos = pRow->GetFormula().IsEmpty() ? -1 : sSource.Find(_T('['));

		int nGroupTextRight = rcText.left;

		if (pos == -1) //non formula case
		{
			pDC->DrawText(sSource, &rcText, nFlags);
		}
		BOOL bDrawnBefore = FALSE;
		while (pos > - 1)
		{
			if (!bDrawnBefore)
			{
				CString sBefore = sSource.Left(pos);
				pDC->DrawText(sBefore, &rcText, nFlags);
				bDrawnBefore = TRUE;

				nGroupTextRight = rcText.left + pDC->GetTextExtent(sBefore).cx;
			}
			CString sAfter = sSource.Mid(pos + 1);
			pos = sAfter.Find(_T(']'));

			if (pos > - 1)
			{
				CString sNum = sAfter.Left(pos);
				int iCol = _ttoi(sNum);
				sSource = sAfter.Mid(pos + 1);

				CXTPReportColumn* pColumn = pControl->GetColumns()->Find(iCol);

				pos = sSource.Find(_T('['));
				if (pos > - 1)
					sAfter = sSource.Left(pos);
				else
					sAfter = sSource;

				if (pColumn && pColumn->IsVisible())
				{
					CRect rcCol = pColumn->GetRect();

					/*
					if (pDC->IsPrinting())
					{
						if (!m_mapColumnPrintPosition.Lookup(pColumn, rcCol))
							rcCol.SetRectEmpty();
					}
					*/

					rcText.left  = rcCol.left  + 1;
					rcText.right = rcCol.right - 1;

					int nTextExtentX = pDC->GetTextExtent(sAfter).cx;

					switch(pColumn->GetAlignment())
					{
					case DT_LEFT:
						break;
					case DT_CENTER:
						break;
					case DT_RIGHT:
						rcText.left = rcText.right - nTextExtentX;
						break;
					}

					if (rcText.left < nGroupTextRight) // Intersect case, move to Right
					{
						rcText.left  = nGroupTextRight + 10;
						//rcText.right = rcText.left + nTextExtentX; ! clip text
					}

					if (rcCol.right > rcCol.left)
					{
						pDC->DrawText(sAfter, &rcText, pColumn->GetAlignment() | nFlags | DT_NOPREFIX | DT_SINGLELINE);
					}
				}
			}
		}
//      pDC->DrawText(pMetrics->strText, &rcText, nFlags);
//For multiple formulas case need to decode given strText with tags as [C#i1]s1 [C#i2]s2 [C#im]sM
//to draw each piece s1, s2, ... sM under column #i1, column #i2, ... column #iM
		nTextWidth = pDC->GetTextExtent(pMetrics->strText).cx;
	}

	if (pMetrics->nGroupRowIcon != XTP_REPORT_NOICON)
	{
		CRect rcIcon(rcRow);

		if (nGroupRowIconAlignment & xtpGroupRowIconBeforeText)
		{
			rcIcon.left = rcBitmap.left + nBitmapWidth + 5;
		}
		else if (nGroupRowIconAlignment & xtpGroupRowIconAfterText)
		{
			rcIcon.left = rcText.left + nTextWidth + 5;
			rcIcon.left = min(rcIcon.left, rcRow.right - szIcon.cx - 1);
		}
		else if (nGroupRowIconAlignment & xtpGroupRowIconRight)
		{
			rcIcon.left = rcRow.right - szIcon.cx - 1;
		}
		else
		{
			// xtpGroupRowIconLeft
		}

		if (nGroupRowIconAlignment & xtpGroupRowIconVTop)
		{
			rcIcon.bottom = rcIcon.top + szIcon.cy + 1;
		}
		else if (nGroupRowIconAlignment & xtpGroupRowIconVCenterToText)
		{
			rcIcon.top = rcText.CenterPoint().y - szIcon.cy / 2;
			rcIcon.bottom = rcIcon.top + szIcon.cy + 1;
		}
		else if (nGroupRowIconAlignment & xtpGroupRowIconVBottom)
		{
			rcIcon.top = rcIcon.bottom - szIcon.cy - 1;
		}
		else
		{
			// xtpGroupRowIconVCenter
		}

		COLORREF clrBk = pMetrics->clrBackground != XTP_REPORT_COLOR_DEFAULT ?
						pMetrics->clrBackground : RGB(255, 255, 255);

		COLORREF clrBkPrev = pDC->SetBkColor(clrBk);

		DrawBitmap(pDC, pControl, rcIcon, pMetrics->nGroupRowIcon);

		pDC->SetBkColor(clrBkPrev);
	}
	else if (m_bUseGroupRowSelectionState && !pDC->IsPrinting())
	{
		CRect rcIcon(rcBitmap);
		rcIcon.left = (rcBitmap.right + 2);

		BOOL bY(FALSE);
		BOOL bN(FALSE);
		BOOL bOr(FALSE);
		BOOL bR(FALSE);
		if (pRow->GetChilds(FALSE))
		{

			for (int I = 0; I < pRow->GetChilds()->GetCount(); I++)
			{
				CXTPReportRow* pChRow = pRow->GetChilds()->GetAt(I);
				if (pChRow)
				{
					bOr = FALSE;
					CXTPReportRecord* pChRec = pChRow->GetRecord();
					if (pChRec)
					{
						bOr = pChRec->IsSelected();
						bR = TRUE;
					}
					if (pChRow->IsSelected() || (bOr && !pRow->IsExpanded()))
						bY = TRUE;
					else
						bN = TRUE;
				}
			}
		}

		if (bY && !bN)
		{
			DrawGlyph(pDC, rcIcon, xtpReportGlyphEnabledChecked);
		}
		else if (bY && bN)
		{
			DrawGlyph(pDC, rcIcon, xtpReportGlyphDisabledChecked);
		}
		else if (bR)
		{
			DrawGlyph(pDC, rcIcon, xtpReportGlyphEnabledUnchecked);
		}
		else if (pRow->m_bHasSelectedChilds)
		{
			DrawGlyph(pDC, rcIcon, xtpReportGlyphDisabledUnchecked);
			pRow->m_bHasSelectedChilds = FALSE; // Clear local flag
		}
		if (bY) //cascade up
		{
			CXTPReportRow* pParRow = pRow->GetParentRow();
			while (pParRow && pParRow->IsGroupRow())
			{
				pParRow->m_bHasSelectedChilds = TRUE;
				pParRow = pParRow->GetParentRow();
			}
		}
	}
}

CSize CXTPReportPaintManager::DrawCollapsedBitmap(CDC* pDC, const CXTPReportRow* pRow, CRect& rcBitmap)
{
	if (m_columnStyle == xtpReportColumnResource)
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
	}


	// Draw Standard image from resource.
	{
		rcBitmap.left += 2;
		CSize sizeGlyph = DrawGlyph(pDC, rcBitmap, pRow->IsExpanded() ? 0 : 1);

		if (sizeGlyph.cx != 0 && pDC != NULL)
			rcBitmap.right = rcBitmap.left + sizeGlyph.cx;

		return CSize(2 + sizeGlyph.cx, 2 + sizeGlyph.cy);
	}
}

void CXTPReportPaintManager::FillIndent(CDC* pDC, CRect rcRow)
{
	pDC->FillSolidRect(&rcRow, m_clrIndentControl);
}

void CXTPReportPaintManager::FillItemShade(CDC* pDC, CRect rcItem)
{
	// fill item background shade (for example when sorted by this column)
	if (!m_bShadeSortColumn)
		return;

	if (pDC)
		pDC->FillSolidRect(rcItem, m_clrItemShade);
}

void CXTPReportPaintManager::FillHeaderControl(CDC* pDC, CRect rcHeader)
{
	pDC->FillSolidRect(rcHeader, m_clrHeaderControl);
}

void CXTPReportPaintManager::DrawNoGroupByText(CDC* pDC, CRect rcItem)
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

	if (m_columnStyle != xtpReportColumnResource)
		FillHeaderControl(pDC, rcItem);

	pDC->DrawText(m_strNoGroupBy, &rcItem, DT_END_ELLIPSIS | DT_CENTER | DT_NOPREFIX);

}

void CXTPReportPaintManager::FillFooter(CDC* pDC, CRect rcFooter)
{
	if (m_columnStyle == xtpReportColumnResource)
	{
		XTPDrawHelpers()->GradientFill(pDC, rcFooter,
			m_grcGradientColumn.clrDark, m_grcGradientColumn.clrLight, FALSE);

		DrawHorizontalLine(pDC, rcFooter.left, rcFooter.top, rcFooter.Width(), m_clrGroupBoxBack);
		DrawHorizontalLine(pDC, rcFooter.left, rcFooter.top + 1, rcFooter.Width(), m_clrGradientColumnShadow);
		DrawHorizontalLine(pDC, rcFooter.left, rcFooter.top + 2, rcFooter.Width(), MixColor(m_grcGradientColumn.clrDark, m_clrGradientColumnShadow, 0.25));
		DrawHorizontalLine(pDC, rcFooter.left, rcFooter.top + 3, rcFooter.Width(), MixColor(m_grcGradientColumn.clrDark, m_clrGradientColumnShadow, 0.1));

		DrawHorizontalLine(pDC, rcFooter.left, rcFooter.bottom - 1, rcFooter.Width(), m_clrGradientColumnShadow);
	}
	else if (m_columnStyle == xtpReportColumnOffice2003)
	{
		XTPDrawHelpers()->GradientFill(pDC, rcFooter,
			m_grcGradientColumn.clrDark, m_grcGradientColumn.clrLight, FALSE);

		DrawHorizontalLine(pDC, rcFooter.left, rcFooter.top, rcFooter.Width(), m_clrControlBack);
		DrawHorizontalLine(pDC, rcFooter.left, rcFooter.top + 1, rcFooter.Width(), m_clrGradientColumnShadow);
		DrawHorizontalLine(pDC, rcFooter.left, rcFooter.top + 2, rcFooter.Width(), MixColor(m_grcGradientColumn.clrDark, m_clrGradientColumnShadow, 0.25));
		DrawHorizontalLine(pDC, rcFooter.left, rcFooter.top + 3, rcFooter.Width(), MixColor(m_grcGradientColumn.clrDark, m_clrGradientColumnShadow, 0.1));

		DrawHorizontalLine(pDC, rcFooter.left, rcFooter.bottom - 1, rcFooter.Width(), m_clrGradientColumnShadow);
	}
	else
	{
		pDC->FillSolidRect(rcFooter, m_clrHeaderControl);

		DrawHorizontalLine(pDC, rcFooter.left, rcFooter.top, rcFooter.Width(), m_clrControlBack);
		DrawHorizontalLine(pDC, rcFooter.left, rcFooter.top + 1, rcFooter.Width(), m_clrControlDark);
		DrawHorizontalLine(pDC, rcFooter.left, rcFooter.top + 2, rcFooter.Width(), MixColor(m_clrHeaderControl, m_clrControlDark, 0.6));
		DrawHorizontalLine(pDC, rcFooter.left, rcFooter.top + 3, rcFooter.Width(), MixColor(m_clrHeaderControl, m_clrControlDark, 0.25));

		DrawHorizontalLine(pDC, rcFooter.left, rcFooter.bottom - 1, rcFooter.Width(), m_clrControlDark);
	}
}

void CXTPReportPaintManager::DrawHorizontalLine(CDC* pDC, int x, int y, int cx, COLORREF clr)
{
	pDC->FillSolidRect(x, y, cx, 1, clr);
}

void CXTPReportPaintManager::DrawVerticalLine(CDC* pDC, int x, int y, int cy, COLORREF clr)
{
	pDC->FillSolidRect(x, y, 1, cy, clr);
}

void CXTPReportPaintManager::DrawTriangle(CDC* pDC, CRect rcTriangle, BOOL bToDown, int nShadowWidth)
{
	CRect rcTRect;
	int iLastImage = 11; // last image in (m_pGlyphs->SetIcons(bmp, 0, 11, CSize(0, 0)) = 11

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

		if (m_columnStyle == xtpReportColumnResource || m_columnStyle == xtpReportColumnExplorer)
		{
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
			return;
		}

		// Set up pens to use for drawing the triangle
		CPen penLight(PS_SOLID, 1, m_clrControlLightLight);
		CPen penShadow(PS_SOLID, 1, m_clrControlDark);

		if (bToDown)
		{
			Line(pDC, pt.x, pt.y + 3, 3, -6, &penLight);
			Line(pDC, pt.x - 1, pt.y + 4, 3, -6, &penLight);
			Line(pDC, pt.x - 4, pt.y - 2, 7, 0, &penShadow);
			Line(pDC, pt.x - 4, pt.y - 1, 3, 6, &penShadow);
			Line(pDC, pt.x - 5, pt.y - 2, 3, 6, &penShadow);
		}
		else
		{
			Line(pDC, pt.x, pt.y - 2, 3, 6, &penLight);
			Line(pDC, pt.x + 1, pt.y - 1, 3, 6, &penLight);
			Line(pDC, pt.x - 3, pt.y + 4, 6, 0, &penLight);
			Line(pDC, pt.x - 4, pt.y + 4, 3, -6, &penShadow);
			Line(pDC, pt.x - 3, pt.y + 3, 3, -6, &penShadow);
		}
	}
}

void CXTPReportPaintManager::DrawItemBitmap(XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs, CRect& rcItem, int nImage)
{
	ASSERT(pDrawArgs->pControl);
	if (!pDrawArgs->pControl)
		return;

	POINT ptIcon;

	CXTPImageManagerIcon* pIcon = pDrawArgs->pControl->GetImageManager()->GetImage(nImage, 0);

	if (!pIcon)
		return;

	CSize szImage(pIcon->GetWidth(), pIcon->GetHeight());
	CSize szColumn(rcItem.Size());

	int nIconAlign = pDrawArgs->nTextAlign & xtpColumnIconMask;

	if (szImage.cx < szColumn.cx && szImage.cy < szColumn.cy && pDrawArgs->pDC)
	{
		// calculate image position
		if (nIconAlign & xtpColumnIconRight)
		{
			ptIcon.x = rcItem.right - szImage.cx - 1;
			rcItem.right -= (1 + szImage.cx); // shift text box to the left
		}
		else if (nIconAlign & xtpColumnIconCenter)
		{
			ptIcon.x = (rcItem.left + rcItem.right - szImage.cx) / 2 ;
		}
		else //xtpColumnIconLeft : by default
		{
			ptIcon.x = rcItem.left + 1;
			rcItem.left += 1 + szImage.cx; // shift text box to the right
		}

		// vertically align
		if (nIconAlign & xtpColumnIconTop)
		{
			ptIcon.y = rcItem.top;
		}
		else if (nIconAlign & xtpColumnIconBottom)
		{

			ptIcon.y = rcItem.bottom - szImage.cy - 1;
		}
		else // xtpColumnIconVCenter - by default
		{
			ptIcon.y = rcItem.top + (szColumn.cy - szImage.cy) /2;
		}

		// draw image
		if (pDrawArgs->pDC->IsPrinting())
		{
			CBitmap bmp;
			bmp.CreateCompatibleBitmap(pDrawArgs->pDC, szImage.cx, szImage.cy);
			CXTPCompatibleDC dcMem(NULL, &bmp);
			dcMem.FillSolidRect(0, 0, szImage.cx, szImage.cy, pDrawArgs->pDC->GetBkColor());

			pIcon->Draw(&dcMem, CPoint(0, 0), pIcon->GetIcon());
			pDrawArgs->pDC->BitBlt(ptIcon.x, ptIcon.y, szImage.cx, szImage.cy, &dcMem, 0, 0, SRCCOPY);
		}
		else
		{
			pIcon->Draw(pDrawArgs->pDC, ptIcon, pIcon->GetIcon(), 0, pDrawArgs->pRow->IsSelected() ? GetXtremeColor(COLOR_HIGHLIGHT) : CLR_NONE, pDrawArgs->pRow->IsSelected() ? GetXtremeColor(COLOR_HIGHLIGHT) : CLR_NONE, pDrawArgs->pRow->IsSelected() ? ILD_SELECTED : 0);
		}
	}
}

int CXTPReportPaintManager::DrawColumnIcon(CDC* pDC, CXTPReportColumn* pColumn,
		CRect rcColumn, CRect rcIcon, int iIcon)
{
	if (!pColumn || !pColumn->GetControl())
	{
		ASSERT(FALSE);
		return 0;
	}

	POINT ptIcon;

	CXTPImageManagerIcon* pIcon = pColumn->GetControl()->GetImageManager()->GetImage(iIcon, 0);

	if (!pIcon)
		return 0;

	CSize szImage(pIcon->GetWidth(), pIcon->GetHeight());
	CSize szColumn(rcIcon.Size());

	if (szImage.cx < szColumn.cx && szImage.cy < szColumn.cy && pDC)
	{
		ptIcon.x = rcIcon.left + 1;
		ptIcon.y = rcIcon.top + (szColumn.cy - szImage.cy) /2;

		if (pDC->IsPrinting())
		{
			CPoint ptIconOffset(ptIcon.x - rcColumn.left, ptIcon.y - rcColumn.top);
			CRect rcColumn2(0, 0, rcColumn.Width(), rcColumn.Height());

			CBitmap bmp;
			bmp.CreateCompatibleBitmap(pDC, rcColumn2.Width(), rcColumn2.Height());

			CXTPCompatibleDC dcMem(NULL, &bmp);

			// Fill background
			dcMem.FillSolidRect(&rcColumn2, pDC->GetBkColor());
			BOOL bColumnPressed = FALSE;
			DrawColumnBackground(&dcMem, pColumn, rcColumn2, bColumnPressed);


			pIcon->Draw(&dcMem, ptIconOffset, pIcon->GetIcon());

			pDC->BitBlt(ptIcon.x, ptIcon.y, szImage.cx, szImage.cy, &dcMem,
						ptIconOffset.x, ptIconOffset.y, SRCCOPY);
		}
		else
		{
			pIcon->Draw(pDC, ptIcon, pIcon->GetIcon());
		}
	}

	return 1 + szImage.cx;
}

int CXTPReportPaintManager::DrawBitmap(CDC* pDC, CXTPReportControl* pControl, CRect rcColumn, int iIcon)
{
	ASSERT(pControl);
	if (!pControl)
		return 0;

	POINT ptIcon;

	CXTPImageManagerIcon* pIcon = pControl->GetImageManager()->GetImage(iIcon, 0);

	if (!pIcon)
		return 0;

	CSize szImage(pIcon->GetWidth(), pIcon->GetHeight());
	CSize szColumn(rcColumn.Size());

	if (szImage.cx < szColumn.cx && szImage.cy < szColumn.cy && pDC)
	{
		ptIcon.x = rcColumn.left + 1;
		ptIcon.y = rcColumn.top + (szColumn.cy - szImage.cy) /2;

		if (pDC->IsPrinting())
		{
			CBitmap bmp;
			bmp.CreateCompatibleBitmap(pDC, szImage.cx, szImage.cy);
			CXTPCompatibleDC dcMem(NULL, &bmp);
			dcMem.FillSolidRect(0, 0, szImage.cx, szImage.cy, pDC->GetBkColor());

			pIcon->Draw(&dcMem, CPoint(0, 0), pIcon->GetIcon());
			pDC->BitBlt(ptIcon.x, ptIcon.y, szImage.cx, szImage.cy, &dcMem, 0, 0, SRCCOPY);

		}
		else
		{
			pIcon->Draw(pDC, ptIcon, pIcon->GetIcon());
		}
	}

	return 1 + szImage.cx;
}

CSize CXTPReportPaintManager::DrawShellIcon(CDC* pDC, CRect rcColumn, int iImage)
{
	HICON hIcon = NULL;
	::ExtractIconEx(_T("SHELL32.DLL"), iImage, NULL, &hIcon, 1);

	if (hIcon)
	{
		::DrawIconEx(pDC->GetSafeHdc(), rcColumn.left + 1, rcColumn.top + 1, hIcon, 16, 16, 0, NULL, DI_NORMAL);

		::DestroyIcon(hIcon);
	}
	else
	{
		DrawGlyph(pDC, rcColumn, 9); //need index of Lock Icon - now it is 9 and 10 (open lock)
	}

	return CSize(17, 17);
}

static BOOL XTPReportGlyphIsCheckbox(int iGlyph)
{
	return (xtpReportGlyphEnabledUnchecked  == iGlyph) ||
			 (xtpReportGlyphEnabledChecked    == iGlyph) ||
			 (xtpReportGlyphDisabledUnchecked == iGlyph) ||
			 (xtpReportGlyphDisabledChecked   == iGlyph) ||
			 (xtpReportGlyphEnabledTristate   == iGlyph);
}

CSize CXTPReportPaintManager::DrawGlyph(CDC *pDC, CRect rcColumn, int iGlyph)
{
	CSize size(0,0);

	if (XTPReportGlyphIsCheckbox(iGlyph)
		&& m_columnStyle == xtpReportColumnExplorer
		&& m_themeButton->IsAppThemed())
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

int CXTPReportPaintManager::GetRowHeight(CDC *pDC, CXTPReportRow *pRow, int nTotalWidth)
{
	CXTPReportControl *pControl = DYNAMIC_DOWNCAST(CXTPReportControl, pRow->GetControl());

	if (NULL == pControl)
	{
		return 0;
	}

	if (!pControl->IsIconView())
	{
		if (pControl->m_bFreeHeightMode)
		{
			if (pRow->m_nFreeHeight == 0)
				pRow->m_nFreeHeight = pControl->m_nDefaultRowFreeHeight;

			if (pRow->GetRecord())
			{
				if (pRow->GetRecord()->m_nFreeHeight > 0)
					pRow->m_nFreeHeight = pRow->GetRecord()->m_nFreeHeight;
				else
					pRow->GetRecord()->m_nFreeHeight = pRow->m_nFreeHeight;
			}

			return pRow->m_nFreeHeight;
		}
		if (m_bFixedRowHeight || pRow->IsGroupRow() || !pRow->IsItemsVisible())
			return GetRowHeight(pDC, pRow);

		CXTPReportColumns* pColumns = pControl->GetColumns();
		int nColumnCount = pColumns->GetCount();

		XTP_REPORTRECORDITEM_DRAWARGS drawArgs;
		drawArgs.pControl = pRow->GetControl();
		drawArgs.pDC = pDC;
		drawArgs.pRow = pRow;

		XTP_REPORTRECORDITEM_METRICS* pItemMetrics = new XTP_REPORTRECORDITEM_METRICS;
		int nHeight = 0;

		for (int nColumn = 0; nColumn < nColumnCount; nColumn++)
		{
			CXTPReportColumn *pColumn = pColumns->GetAt(nColumn);
			if (pColumn && pColumn->IsVisible())
			{
				CXTPReportRecordItem *pItem = pRow->GetRecord()->GetItem(pColumn);
				if (!pItem)
					continue;

				drawArgs.pItem = pItem;
				drawArgs.nTextAlign = pColumn->GetAlignment();
				drawArgs.pColumn = pColumn; //to support MultiLine Mode

				pItemMetrics->Reset();

				pRow->GetItemMetrics(&drawArgs, pItemMetrics);

				CXTPFontDC fnt(pDC, pItemMetrics->pFont);

				int nWidth = (pDC->IsPrinting() && !m_bColumnWidthWYSIWYG) ?
					pColumn->GetPrintWidth(nTotalWidth) : pColumn->GetWidth();

				CRect rcItem(0, 0, nWidth - 4, 0);
				pRow->ShiftTreeIndent(rcItem, pColumn);

				pItem->GetCaptionRect(&drawArgs, rcItem);

				int nIconW(0), nIconH(0),
					nIconIndex(pItem->GetIconIndex()),
					mIconIndex(pItemMetrics->nItemIcon);

				if (nIconIndex != XTP_REPORT_NOICON)
				{
					CXTPImageManagerIcon *pIcon = pControl->GetImageManager()->GetImage(nIconIndex, 0);

					if (NULL != pIcon)
					{
						nIconW = pIcon->GetWidth();
						nIconH = pIcon->GetHeight();
					}
				}
				else if (mIconIndex != XTP_REPORT_NOICON)
				{
					CXTPImageManagerIcon* pIcon =
						pControl->GetImageManager()->GetImage(mIconIndex, 0);

					if (pIcon)
					{
						nIconW = pIcon->GetWidth();
						nIconH = pIcon->GetHeight();
					}
				}
				if (pItem->GetMarkupUIElement())
				{
					XTPMarkupSetDefaultFont(XTPMarkupElementContext(pItem->GetMarkupUIElement()),
						(HFONT)pItemMetrics->pFont->GetSafeHandle(), COLORREF_NULL);

					int nCalculatedHeight = XTPMarkupMeasureElement(pItem->GetMarkupUIElement(), rcItem.Width(), INT_MAX).cy;
					nHeight = max(nHeight, nCalculatedHeight);
				}
				else
				{
					BOOL bWordBreak = !!(pItemMetrics->nColumnAlignment & DT_WORDBREAK);

					CString strText;
					if (pControl->IsVirtualMode())
						strText = pItemMetrics->strText;
					else
						strText = pItem->GetCaption(pColumn);

					ReplaceInHyperLinks(pItem, strText, XTP_HLINK_SPACE_X);

					int iWid = rcItem.Width();
					iWid -= nIconW;
					iWid -= 3; //tolerance level
					int nCalculatedHeight = CalculateRowHeight(pDC, strText, iWid, bWordBreak);
					nHeight = max(nHeight, nCalculatedHeight);
				}
				nHeight = max(nHeight, nIconH);
			}
		}

		CMDTARGET_RELEASE(pItemMetrics);
		int iExtraPix = 5;
		return max(nHeight + iExtraPix, m_nRowHeight) + (IsGridVisible(FALSE) ? 1 : 0);
	}
	else
	{
		if (pControl->GetRowsPerLine() && pRow->GetIndex() % pControl->GetRowsPerLine())
			return 0;
		else
			return pControl->m_iIconHeightSpacing;
	}
}

int CXTPReportPaintManager::GetRowHeight(CDC* /*pDC*/, CXTPReportRow* pRow)
{
	if (!pRow->IsGroupRow())
		return m_nRowHeight + (IsGridVisible(FALSE) ? 1 : 0);

	if (m_bShadeGroupHeadings)
		return m_nRowHeight + 6;

	return m_nRowHeight + 16;
}

BOOL CXTPReportPaintManager::IsColumHotTrackingEnabled() const
{
	return m_bHotTracking && ((m_columnStyle == xtpReportColumnOffice2003) || ((m_columnStyle == xtpReportColumnExplorer) && m_themeHeader->IsAppThemed()));
}

void CXTPReportPaintManager::DrawColumnBackground(CDC* pDC, CXTPReportColumn* pColumn, CRect rcColumn, BOOL& bColumnPressed, CXTPReportHeader* pHeader)
{
	CXTPReportColumn* pPrevColumn = pHeader ? pHeader->GetNextVisibleColumn(pColumn->GetIndex(), -1) : NULL;

	if (m_columnStyle == xtpReportColumnShaded)
	{
		int nShadowWidth = 4;

		if (pColumn->GetDrawHeaderDivider())
			DrawVerticalLine(pDC, rcColumn.right - 1, rcColumn.top + 2, rcColumn.Height() - 2 - nShadowWidth, m_clrControlDark);

		DrawHorizontalLine(pDC, rcColumn.left, rcColumn.bottom - 3, rcColumn.Width(), MixColor(m_clrHeaderControl, m_clrControlDark, 0.25));
		DrawHorizontalLine(pDC, rcColumn.left, rcColumn.bottom - 2, rcColumn.Width(), MixColor(m_clrHeaderControl, m_clrControlDark, 0.4));
		DrawHorizontalLine(pDC, rcColumn.left, rcColumn.bottom - 1, rcColumn.Width(), MixColor(m_clrHeaderControl, m_clrControlDark, 0.6));

		if (!pPrevColumn || pPrevColumn->GetDrawHeaderDivider())
			DrawVerticalLine(pDC, rcColumn.left, rcColumn.top + 2, rcColumn.Height() - 2 - nShadowWidth, m_clrControlLightLight);
	}
	else if (m_columnStyle == xtpReportColumnOffice2003 || m_columnStyle == xtpReportColumnResource)
	{

		XTPDrawHelpers()->GradientFill(pDC, rcColumn,
			bColumnPressed ? m_grcGradientColumnPushed: pColumn->IsHotTracking() ? m_grcGradientColumnHot : m_grcGradientColumn, FALSE);

		DrawHorizontalLine(pDC, rcColumn.left, rcColumn.bottom - 1, rcColumn.Width(), m_clrGradientColumnShadow);

		if (pColumn->GetDrawHeaderDivider())
			DrawVerticalLine(pDC, rcColumn.right - 1, rcColumn.top + 2, rcColumn.Height() - 2 - 4, m_clrGradientColumnSeparator);

		if (m_columnStyle == xtpReportColumnOffice2003 && (!pPrevColumn || pPrevColumn->GetDrawHeaderDivider()))
			DrawVerticalLine(pDC, rcColumn.left, rcColumn.top + 3, rcColumn.Height() - 2 - 4, m_clrControlLightLight);

		if (pColumn->GetControl()->IsGroupByVisible())
			DrawHorizontalLine(pDC, rcColumn.left, rcColumn.top, rcColumn.Width(), m_clrGradientColumnShadow);

		bColumnPressed = FALSE;
	}
	else if (m_columnStyle == xtpReportColumnExplorer)
	{
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
	else
	{ // xtpReportColumnFlat
		pDC->Draw3dRect(rcColumn, m_clrControlLightLight, m_clrControlDark);
		if (!pColumn->GetDrawHeaderDivider())
		{
			DrawVerticalLine(pDC, rcColumn.right - 1, rcColumn.top + 1, rcColumn.Height() - 2, m_clrHeaderControl);
			pDC->SetPixel(rcColumn.right - 1, rcColumn.top, m_clrControlLightLight);
		}
		if (pPrevColumn && !pPrevColumn->GetDrawHeaderDivider())
			DrawVerticalLine(pDC, rcColumn.left, rcColumn.top + 1, rcColumn.Height() - 2, m_clrHeaderControl);
	}
}

void CXTPReportPaintManager::DrawRowNumber(CDC* pDC, CRect rcNumber, CXTPReportRow* pRow)
{
	if (!m_bRecOrRowNum) //Row # case
	{
		int indRow = pRow->GetIndex();
		if (m_iRecOrRowStart > 0)
			indRow += m_iRecOrRowStart;
		CString sRec;
		sRec.Format(_T(" %d "), indRow);

		if (pRow->IsSelected())
		{
			if (pRow->GetControl()->HasFocus())
				pDC->FillSolidRect(rcNumber, GetXtremeColor(COLOR_HIGHLIGHT));
			else
				pDC->FillSolidRect(rcNumber, GetXtremeColor(COLOR_BTNFACE));

			CXTPFontDC font(pDC, GetTextFont(), RGB(255,255,255));
			//pDC->DrawText(sRec, &rcNumber, DT_SINGLELINE | DT_TOP | DT_RIGHT);
			//pDC->DrawText(sRec, &rcNumber, DT_SINGLELINE | DT_VCENTER | DT_RIGHT);
			//pDC->DrawText(sRec, &rcNumber, DT_SINGLELINE | DT_BOTTOM | DT_RIGHT);
			rcNumber.top++;
			pDC->DrawText(sRec, &rcNumber, DT_SINGLELINE | DT_RIGHT);
		}
		else
		{
			//pDC->FillSolidRect(rcNumber, RGB(224,224,224));
			//if we want to indicate read-only mode of record's numbers
			CXTPFontDC font(pDC, GetTextFont(), GetXtremeColor(COLOR_HIGHLIGHT));
			//pDC->DrawText(sRec, &rcNumber, DT_SINGLELINE | DT_TOP | DT_RIGHT);
			//pDC->DrawText(sRec, &rcNumber, DT_SINGLELINE | DT_VCENTER | DT_RIGHT);
			//pDC->DrawText(sRec, &rcNumber, DT_SINGLELINE | DT_BOTTOM | DT_RIGHT);
			rcNumber.top++;
			pDC->DrawText(sRec, &rcNumber, DT_SINGLELINE | DT_RIGHT);
		}
	}
	else //Record # case
	{
		CXTPReportRecord* pRec = pRow->GetRecord();
		if (pRec)
		{
			int indRec = pRec->GetIndex();

			if (m_iRecOrRowStart > 0)
				indRec += m_iRecOrRowStart;

			CString sRec;
			sRec.Format(_T(" %d "), indRec);
			if (pRow->IsSelected())
			{
				if (pRow->GetControl()->HasFocus())
					pDC->FillSolidRect(rcNumber, GetXtremeColor(COLOR_HIGHLIGHT));
				else
					pDC->FillSolidRect(rcNumber, GetXtremeColor(COLOR_BTNFACE));

				CXTPFontDC font(pDC, GetTextFont(), RGB(255,255,255));
				//pDC->DrawText(sRec, &rcNumber, DT_SINGLELINE | DT_TOP | DT_RIGHT);
				//pDC->DrawText(sRec, &rcNumber, DT_SINGLELINE | DT_VCENTER | DT_RIGHT);
				//pDC->DrawText(sRec, &rcNumber, DT_SINGLELINE | DT_BOTTOM | DT_RIGHT);
				rcNumber.top++;
				pDC->DrawText(sRec, &rcNumber, DT_SINGLELINE | DT_RIGHT);
			}
			else
			{
				//pDC->FillSolidRect(rcNumber, RGB(224,224,224));
				//if we want to indicate read-only mode of record's numbers
				CXTPFontDC font(pDC, GetTextFont(), GetXtremeColor(COLOR_HIGHLIGHT));
				//pDC->DrawText(sRec, &rcNumber, DT_SINGLELINE | DT_TOP | DT_RIGHT);
				//pDC->DrawText(sRec, &rcNumber, DT_SINGLELINE | DT_VCENTER | DT_RIGHT);
				//pDC->DrawText(sRec, &rcNumber, DT_SINGLELINE | DT_BOTTOM | DT_RIGHT);
				rcNumber.top++;
				pDC->DrawText(sRec, &rcNumber, DT_SINGLELINE | DT_RIGHT);
			}
		}
	}
}

void CXTPReportPaintManager::DrawColumnFooter(CDC* pDC, CXTPReportColumn* pColumn, CXTPReportHeader* /*pHeader*/, CRect rcColumn)
{
	if (pColumn->GetDrawFooterDivider())
	{
		DrawVerticalLine(pDC, rcColumn.right, rcColumn.top + 6, rcColumn.Height() - 9, m_clrControlLightLight);
		DrawVerticalLine(pDC, rcColumn.right - 1, rcColumn.top + 5, rcColumn.Height() - 9,
			m_columnStyle == xtpReportColumnOffice2003 || m_columnStyle == xtpReportColumnResource ? m_clrGradientColumnSeparator : m_clrControlDark);
	}

	pDC->SetBkMode(TRANSPARENT);
	CXTPFontDC font(pDC, pColumn->GetFooterFont(), m_clrCaptionText);

	CString strCaption = pColumn->GetFooterText();
	CSize sizeText = pDC->GetTextExtent(strCaption);
	CRect rcText(rcColumn);

	rcText.DeflateRect(3, 2, 3, 2);

	if (pColumn->GetFooterAlignment() != DT_LEFT)
	{
		int nLength = 6 + sizeText.cx;

		if (rcText.Width() > nLength)
		{
			if (pColumn->GetFooterAlignment() & DT_RIGHT)
				rcText.left = rcText.right - nLength;
			if (pColumn->GetFooterAlignment() & DT_CENTER)
				rcText.left = (rcText.left + rcText.right - nLength) / 2;
		}
	}

	UINT uFlags = DT_END_ELLIPSIS | DT_NOPREFIX;

	if (!m_bFixedRowHeight && (pColumn->GetFooterAlignment() & DT_WORDBREAK))
	{
		uFlags |= DT_WORDBREAK | DT_WORD_ELLIPSIS;

		// try to center vertically because DT_VCENTER works only for DT_SINGLELINE;
		CRect rcTextReal = rcText;
		pDC->DrawText(strCaption, &rcTextReal, uFlags | DT_CALCRECT);

		int nHeightDiff = rcText.Height() - rcTextReal.Height();
		if (nHeightDiff > 1)
		{
			rcText.top += nHeightDiff/2;
		}
	}
	else
	{
		uFlags |= DT_SINGLELINE | DT_VCENTER;
	}

	if (strCaption.GetLength() > 0 && rcText.right > rcText.left)
	{
		pDC->DrawText(strCaption, &rcText, uFlags);
	}
}


void CXTPReportPaintManager::DrawColumn(CDC *pDC, CXTPReportColumn *pColumn, CXTPReportHeader *pHeader, CRect rcColumn, BOOL bDrawExternal)
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
	int nShadowWidth = (!bPlainColumn && m_columnStyle == xtpReportColumnShaded) ? 4 : 0;

	if (bPlainColumn)
	{
		if (m_columnStyle == xtpReportColumnResource)
		{
			XTPDrawHelpers()->GradientFill(pDC, rcColumn, m_grcGradientColumn, FALSE);
			pDC->Draw3dRect(rcColumn, m_clrGradientColumnShadow, m_clrGradientColumnShadow);
		}
		else
		{
			pDC->Draw3dRect(rcColumn, m_clrControlLightLight, m_clrControlDark);
			pDC->MoveTo(rcColumn.left, rcColumn.bottom);
			pDC->LineTo(rcColumn.right, rcColumn.bottom);
			pDC->LineTo(rcColumn.right, rcColumn.top - 1);
		}
	}
	else
	{
		DrawColumnBackground(pDC, pColumn, rcColumn, bColumnPressed, pHeader);
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

	if (strCaption.GetLength() > 0 && rcText.right > rcText.left)
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
			DrawTriangle(pDC, rcTriangle, pColumn->IsSortedDecreasing(), nShadowWidth);
	}

	if (bColumnPressed && m_bInvertColumnOnClick)
	{
		rcColumn.bottom += nShadowWidth;
		pDC->InvertRect(&rcColumn);
	}
}

int CXTPReportPaintManager::CalcColumnHeight(CDC* pDC, CXTPReportColumn* pColumn,
	int nTotalWidth)
{
	int nIconID = pColumn->GetIconID();
	CString strCaption = pColumn->GetCaption();

	int nWidth = (pDC->IsPrinting() && !m_bColumnWidthWYSIWYG) ?
		pColumn->GetPrintWidth(nTotalWidth) : pColumn->GetWidth();
	CRect rcColumn = pColumn->GetRect();
	rcColumn.right = rcColumn.left + nWidth;

	CRect rcText(rcColumn);

	if (!pColumn->IsAutoSize())
		rcText.left += pColumn->GetIndent();

	BOOL bHasSortTriangle = pColumn->HasSortTriangle() && (!pDC->IsPrinting() || m_bPrintSortTriangle);

	CXTPFontDC font(pDC, &m_fontCaption);
	CSize sizeText = pDC->GetTextExtent(strCaption);

	if (nIconID != XTP_REPORT_NOICON)
	{
		// shift column left by bitmap drawing place
		rcText.left += DrawBitmap(NULL, pColumn->GetControl(), rcText, nIconID);
		// shift column left to the column right to disallow drawing text on Bitmap column headers
		rcText.left += 2;
	}

	rcText.DeflateRect(3, 0, 3, 0);

	if (strCaption.GetLength() > 0 && rcText.right > rcText.left)
	{
		//if (m_DrawSortTriangleStyle == xtpReportDrawSortTriangleAlways && bHasSortTriangle)
		//{
		//  int cx = max(-5, min(sizeText.cx, rcText.Width() - 27));
		//  rcText.right = min(rcText.right, rcText.left + cx + 8);
		//}
		if (m_DrawSortTriangleStyle != xtpReportDrawSortTriangleNever && bHasSortTriangle)
		{
			int cx = max(-5, min(sizeText.cx, rcText.Width() - 27));
			//int cx = max(-5, min(sizeText.cx, rcText.Width() - 11));
			//if (pColumn->IsPlusMinus())
			//  cx = max(-5, min(sizeText.cx, rcText.Width() - 22));
			rcText.right = min(rcText.right, rcText.left + cx + 8);
			//rcText.right -= 11;
			//if (pColumn->IsPlusMinus())
			//  rcText.right -= 11;
		}
		//else if (m_DrawSortTriangleStyle == xtpReportDrawSortTriangleDefault && bHasSortTriangle)
		//{
		//  //if (pColumn->IsPlusMinus())
		//  //{
		//  //  int cx = max(-5, min(sizeText.cx, rcText.Width() - 11));
		//  //  rcText.right = min(rcText.right, rcText.left + cx + 8);
		//  //}
		//  if (pColumn->IsPlusMinus())
		//      rcText.right -= 11;
		//}

		UINT uFlags = DT_END_ELLIPSIS | DT_VCENTER | DT_NOPREFIX;
		uFlags |= (pColumn->GetHeaderAlignment() & DT_WORDBREAK) ?
			DT_WORDBREAK | DT_WORD_ELLIPSIS : DT_SINGLELINE;
		uFlags |= DT_CALCRECT;

		pDC->DrawText(strCaption, &rcText, uFlags);

		return rcText.Height() + 8;
	}

	return 8;
}

void CXTPReportPaintManager::FillGroupByControl(CDC* pDC, CRect& rcGroupBy)
{
	pDC->FillSolidRect(rcGroupBy, m_clrGroupBoxBack);
}

void CXTPReportPaintManager::DrawConnector(CDC* pDC, CPoint ptFrom, CPoint ptTo)
{
	COLORREF crConnector = RGB(0, 0, 0);
	DrawVerticalLine(pDC, ptFrom.x, ptFrom.y, ptTo.y - ptFrom.y, crConnector);
	DrawHorizontalLine(pDC, ptFrom.x, ptTo.y, ptTo.x - ptFrom.x, crConnector);
}

void CXTPReportPaintManager::DrawFocusedRow(CDC* pDC, CRect rcRow)
{
	if (m_pControl && ((CXTPReportControl* ) m_pControl)->IsIconView())
		rcRow.DeflateRect(1, 1);

	COLORREF clrTextColor = pDC->SetTextColor(m_clrControlBack);
	COLORREF clrBkColor = pDC->SetBkColor(m_clrWindowText);
	pDC->DrawFocusRect(rcRow);
	pDC->SetTextColor(clrTextColor);
	pDC->SetBkColor(clrBkColor);
}

int CXTPReportPaintManager::GetPreviewLinesCount(CDC* pDC, CRect& rcText, const CString& strText)
{
	if (rcText.Width() < 1)
		return 0;

	if (pDC->GetTextExtent(strText).cx / rcText.Width() > m_nMaxPreviewLines)
		return m_nMaxPreviewLines;

	CRect rcCalc(rcText.left, 0, rcText.right, 0);
	pDC->DrawText(strText, &rcCalc, DT_WORDBREAK | DT_CALCRECT | DT_NOPREFIX);

	return min(rcCalc.Height() / pDC->GetTextExtent(_T(" "), 1).cy, m_nMaxPreviewLines);
}

void CXTPReportPaintManager::SetTextFont(LOGFONT &lf)
{
	m_fontText.DeleteObject();
	m_fontBoldText.DeleteObject();

	// Normal text
	m_fontText.CreateFontIndirect(&lf);

	// Bold text
	LOGFONT lfBold = lf;
	lfBold.lfWeight = FW_BOLD;
	m_fontBoldText.CreateFontIndirect(&lfBold);

	CWindowDC dc(NULL);
	CXTPFontDC font(&dc, &m_fontText);

	int nFontHeight = dc.GetTextExtent(_T(" "), 1).cy;
	m_nRowHeight = max(m_nRowHeightDefault, nFontHeight + 4);
}

void CXTPReportPaintManager::SetCaptionFont(LOGFONT& lf)
{
	m_fontCaption.DeleteObject();
	VERIFY(m_fontCaption.CreateFontIndirect(&lf));

	CWindowDC dc (NULL);
	CXTPFontDC font(&dc, &m_fontCaption);

	font.SetFont(&m_fontCaption);
	int nFontHeight = dc.GetTextExtent(_T(" "), 1).cy;
	m_nHeaderHeight = max(22, nFontHeight + 8);
	m_nFooterHeight = m_nHeaderHeight;
}

int CXTPReportPaintManager::GetFooterHeight(CXTPReportControl* pControl, CDC* pDC, int nTotalWidth)
{
	ASSERT(pControl && pDC);

	if (!pControl || !pDC)
	{
		return m_nFooterHeight;
	}

	CXTPReportColumns* pColumns = pControl->GetColumns();
	int nColumnCount = pColumns->GetCount();
	int nHeight = 22;

	CXTPFontDC font(pDC);

	for (int nColumn = 0; nColumn < nColumnCount; nColumn++)
	{
		CXTPReportColumn* pColumn = pColumns->GetAt(nColumn);
		if (pColumn && pColumn->IsVisible())
		{
			CString strCaption = pColumn->GetFooterText();

			int nWidth = (pDC->IsPrinting() && !m_bColumnWidthWYSIWYG) ?
			//int nWidth = (pDC->IsPrinting()) ?
				pColumn->GetPrintWidth(nTotalWidth) : pColumn->GetWidth();
			CRect rcText(0, 0, nWidth, 22);
			rcText.DeflateRect(3, 0, 3, 0);

			UINT uFlags = DT_CALCRECT | DT_END_ELLIPSIS | DT_VCENTER | DT_NOPREFIX;
			if (!m_bFixedRowHeight && (pColumn->GetFooterAlignment() & DT_WORDBREAK))
				//uFlags |= DT_WORDBREAK;
				uFlags |= DT_WORDBREAK | DT_WORD_ELLIPSIS;
			else
				uFlags |= DT_SINGLELINE;

			font.SetFont(pColumn->GetFooterFont());
			pDC->DrawText(strCaption, &rcText, uFlags);

			nHeight = max(nHeight, rcText.Height() + 8);
		}
	}
	return  nHeight;
}

int CXTPReportPaintManager::GetHeaderHeight(CXTPReportControl* pControl, CDC *pDC, int nTotalWidth)
{
	ASSERT(pControl && pDC);

	if (m_bFixedRowHeight || !pControl || !pDC)
	{
		return GetHeaderHeight();
	}

	CXTPReportColumns* pColumns = pControl->GetColumns();
	int nColumnCount = pColumns->GetCount();
	int nHeight = m_nHeaderHeight;

	for (int nColumn = 0; nColumn < nColumnCount; nColumn++)
	{
		CXTPReportColumn* pColumn = pColumns->GetAt(nColumn);
		if (pColumn && pColumn->IsVisible())
		{
			if (pColumn->GetHeaderAlignment() & DT_WORDBREAK)
			{
				int nColHeight = CalcColumnHeight(pDC, pColumn, nTotalWidth);

				nHeight = max(nHeight, nColHeight);
			}

			// calculate icon height.
			int nIconID = pColumn->GetIconID();
			if (nIconID != XTP_REPORT_NOICON)
			{
				CXTPImageManagerIcon* pIcon =
					pColumn->GetControl()->GetImageManager()->GetImage(nIconID, 0);

				if (pIcon)
					nHeight = max(nHeight, pIcon->GetHeight() + 2);
			}
		}
	}

	return nHeight;
}

void CXTPReportPaintManager::SetHeaderHeight(int nHeight)
{
	if (nHeight < 0)
		nHeight = 0;

	m_nHeaderHeight = nHeight;
}

void CXTPReportPaintManager::SetPreviewIndent(int nLeft, int nTop, int nRight, int nBottom)
{
	m_rcPreviewIndent.SetRect(nLeft, nTop, nRight, nBottom);
}

BOOL CXTPReportPaintManager::DrawInplaceComboButton(CDC* pDC, CRect* pRc)
{
	if ((!pDC->IsPrinting() || m_bPrintGroupRowIcon)
		&& m_bShowNonActiveInPlaceButton)
	{
		CRect rect(*pRc);

		if (m_bThemedInplaceButtons && m_themeCombo->IsAppThemed())
		{
			m_themeCombo->DrawThemeBackground(*pDC, CP_DROPDOWNBUTTON, CBXS_NORMAL, pRc, NULL);
		}
		else
		{
			CRect rcFrame(rect);
			pDC->FillSolidRect(rect, GetXtremeColor(COLOR_3DFACE));
			pDC->Draw3dRect(rcFrame, GetXtremeColor(COLOR_3DFACE), GetXtremeColor(COLOR_3DDKSHADOW));
			rcFrame.DeflateRect(1, 1);
			pDC->Draw3dRect(rcFrame, GetXtremeColor(COLOR_WINDOW), GetXtremeColor(COLOR_3DSHADOW));

			CXTPPenDC pen (*pDC, GetXtremeColor(COLOR_BTNTEXT));
			CXTPBrushDC brush (*pDC, GetXtremeColor(COLOR_BTNTEXT));

			CPoint pt = rect.CenterPoint();

			CPoint pts[3];
			pts[0] = CPoint(pt.x -3, pt.y -1);
			pts[1] = CPoint(pt.x + 3, pt.y -1);
			pts[2] = CPoint(pt.x, pt.y +2);
			pDC->Polygon(pts, 3);

		}

		return TRUE;
	}
	return FALSE;
}

BOOL CXTPReportPaintManager::DrawInplaceExpandButton(CDC* pDC, CRect* pRc)
{
	if ((!pDC->IsPrinting() || m_bPrintGroupRowIcon)
		&& m_bShowNonActiveInPlaceButton)
	{
		CRect rect(*pRc);
		pDC->FillSolidRect(rect, GetXtremeColor(COLOR_3DFACE));

		if (m_bThemedInplaceButtons && m_themeButton->IsAppThemed())
		{
			m_themeButton->DrawThemeBackground(*pDC, BP_PUSHBUTTON, PBS_NORMAL, pRc, NULL);
		}
		else
		{
			CRect rcFrame(rect);
			pDC->Draw3dRect(rcFrame, GetXtremeColor(COLOR_3DFACE), GetXtremeColor(COLOR_3DDKSHADOW));
			rcFrame.DeflateRect(1, 1);
			pDC->Draw3dRect(rcFrame, GetXtremeColor(COLOR_WINDOW), GetXtremeColor(COLOR_3DSHADOW));

		}
		pDC->Rectangle(rect.left + 3, rect.bottom -5, rect.left + 5, rect.bottom -3);
		pDC->Rectangle(rect.left + 7, rect.bottom -5, rect.left + 9, rect.bottom -3);
		pDC->Rectangle(rect.left + 11, rect.bottom -5, rect.left + 13, rect.bottom -3);

		return TRUE;

	}
	return FALSE;
}

BOOL CXTPReportPaintManager::DrawInplaceSpinButton(CDC* pDC, CRect* pRc)
{
	if ((!pDC->IsPrinting() || m_bPrintGroupRowIcon)
		&& m_bShowNonActiveInPlaceButton
		&& m_bThemedInplaceButtons
		&& m_themeSpin->IsAppThemed())
	{
		int nHeight = pRc->Height() / 2;
		pRc->bottom -= nHeight;
		m_themeSpin->DrawThemeBackground(*pDC, SPNP_UP, UPS_NORMAL, pRc, NULL);
		pRc->OffsetRect(0, nHeight);
		m_themeSpin->DrawThemeBackground(*pDC, SPNP_DOWN, UPS_NORMAL, pRc, NULL);
		return TRUE;
	}
	return FALSE;
}

void CXTPReportPaintManager::DrawInplaceButton(CDC* pDC, CXTPReportInplaceButton* pButton)
{
	CXTPClientRect rect((CWnd*)pButton);
	BOOL bButtonIconDrawn = FALSE;

	if (m_bThemedInplaceButtons)
	{
		if (pButton->GetID() == XTP_ID_REPORT_COMBOBUTTON && m_themeCombo->IsAppThemed())
		{
			m_themeCombo->DrawThemeBackground(*pDC, CP_DROPDOWNBUTTON,
				pButton->IsPressed() ? CBXS_PRESSED:  CBXS_NORMAL, rect, NULL);
			return;
		}

		if (pButton->GetID() == XTP_ID_REPORT_EXPANDBUTTON  && m_themeButton->IsAppThemed())
		{
			pDC->FillSolidRect(rect, GetXtremeColor(COLOR_3DFACE));

			m_themeButton->DrawThemeBackground(*pDC, BP_PUSHBUTTON, pButton->IsPressed() ? PBS_PRESSED :
				CWnd::GetFocus() == pButton ? PBS_DEFAULTED : PBS_NORMAL, rect, 0);

			if (!DrawInplaceButtonIcon(pDC, pButton))
			{
				// draw dots
				pDC->Rectangle(rect.left + 3, rect.bottom -5, rect.left + 5, rect.bottom -3);
				pDC->Rectangle(rect.left + 7, rect.bottom -5, rect.left + 9, rect.bottom -3);
				pDC->Rectangle(rect.left + 11, rect.bottom -5, rect.left + 13, rect.bottom -3);
			}
			return;
		}

		if (pButton->GetID() == XTP_ID_REPORT_SPINBUTTON && m_themeSpin->IsAppThemed())
		{
			int nHeight = rect.Height() / 2;
			rect.bottom -= nHeight;
			m_themeSpin->DrawThemeBackground(*pDC, SPNP_UP, pButton->GetState() == SPNP_UP ? UPS_PRESSED : UPS_NORMAL, rect, 0);
			rect.OffsetRect(0, nHeight);
			m_themeSpin->DrawThemeBackground(*pDC, SPNP_DOWN, pButton->GetState() == SPNP_DOWN ? UPS_PRESSED : UPS_NORMAL, rect, 0);
			return;
		}
	}
	if (pButton->GetID() == XTP_ID_REPORT_COMBOBUTTON)
	{
		DrawInplaceButtonFrame(pDC, pButton);
	}
	else if (pButton->GetID() == XTP_ID_REPORT_EXPANDBUTTON)
	{
		DrawInplaceButtonFrame(pDC, pButton);
		bButtonIconDrawn = DrawInplaceButtonIcon(pDC, pButton);
	}
	else if (pButton->GetID() == XTP_ID_REPORT_SPINBUTTON)
	{
		CXTPClientRect rect((CWnd*)pButton);
		int nHeight = rect.Height() / 2;
		rect.bottom -= nHeight;
		for (int i = 0; i < 2; i++)
		{
			pDC->FillSolidRect(rect, GetXtremeColor(COLOR_3DFACE));

			if (i == 0 && pButton->GetState() == SPNP_UP || i == 1 && pButton->GetState() == SPNP_DOWN)
			{
				pDC->Draw3dRect(rect, GetXtremeColor(COLOR_3DDKSHADOW), GetXtremeColor(COLOR_WINDOW));
			}
			else
			{
				pDC->Draw3dRect(rect, GetXtremeColor(COLOR_3DFACE), GetXtremeColor(COLOR_3DDKSHADOW));
				rect.DeflateRect(1, 1);
				pDC->Draw3dRect(rect, GetXtremeColor(COLOR_WINDOW), GetXtremeColor(COLOR_3DSHADOW));
				rect.InflateRect(1, 1);
			}
			rect.OffsetRect(0, nHeight);
		}
	}

	CXTPPenDC pen (*pDC, GetXtremeColor(COLOR_BTNTEXT));
	CXTPBrushDC brush (*pDC, GetXtremeColor(COLOR_BTNTEXT));

	if (pButton->GetID() == XTP_ID_REPORT_COMBOBUTTON)
	{
		CPoint pt = rect.CenterPoint();

		CPoint pts[3];
		pts[0] = CPoint(pt.x -3, pt.y -1);
		pts[1] = CPoint(pt.x + 3, pt.y -1);
		pts[2] = CPoint(pt.x, pt.y +2);
		pDC->Polygon(pts, 3);
	}

	if (pButton->GetID() == XTP_ID_REPORT_EXPANDBUTTON && !bButtonIconDrawn)
	{
		pDC->Rectangle(rect.left + 3, rect.bottom -5, rect.left + 5, rect.bottom -3);
		pDC->Rectangle(rect.left + 7, rect.bottom -5, rect.left + 9, rect.bottom -3);
		pDC->Rectangle(rect.left + 11, rect.bottom -5, rect.left + 13, rect.bottom -3);
	}

	if (pButton->GetID() == XTP_ID_REPORT_SPINBUTTON)
	{
		CPoint pt = rect.CenterPoint();
		pt.y -= rect.Height() / 4;

		CPoint pts[3];
		pts[0] = CPoint(pt.x - 2, pt.y + 1);
		pts[1] = CPoint(pt.x + 2, pt.y + 1);
		pts[2] = CPoint(pt.x, pt.y - 1);
		pDC->Polygon(pts, 3);

		pt.y += rect.Height() / 2;
		pts[0] = CPoint(pt.x - 2, pt.y - 1);
		pts[1] = CPoint(pt.x + 2, pt.y - 1);
		pts[2] = CPoint(pt.x, pt.y + 1);
		pDC->Polygon(pts, 3);
	}
}

void CXTPReportPaintManager::DrawInplaceButtonFrame(CDC* pDC, CXTPReportInplaceButton* pButton)
{
	CXTPClientRect rect((CWnd*)pButton);

	pDC->FillSolidRect(rect, GetXtremeColor(COLOR_3DFACE));

	if (pButton->IsPressed())
	{
		pDC->Draw3dRect(rect, GetXtremeColor(COLOR_3DDKSHADOW), GetXtremeColor(COLOR_WINDOW));
	}
	else
	{
		pDC->Draw3dRect(rect, GetXtremeColor(COLOR_3DFACE), GetXtremeColor(COLOR_3DDKSHADOW));
		rect.DeflateRect(1, 1);
		pDC->Draw3dRect(rect, GetXtremeColor(COLOR_WINDOW), GetXtremeColor(COLOR_3DSHADOW));
	}
}

BOOL CXTPReportPaintManager::DrawInplaceButtonIcon(CDC* pDC, CXTPReportInplaceButton* pButton)
{
	CXTPClientRect rect((CWnd*)pButton);

	int nIconIndex = pButton->GetIconIndex();
	if (nIconIndex == XTP_REPORT_NOICON)
		return FALSE;

	CXTPImageManagerIcon* pIcon = pButton->pControl->GetImageManager()->GetImage(nIconIndex, 0);
	if (!pIcon)
		return FALSE;

	CSize szImage(pIcon->GetWidth(), pIcon->GetHeight());
	CPoint ptIcon(max(rect.left, rect.left + (rect.Width() - szImage.cx) / 2),
					max(rect.top, rect.top + (rect.Height() - szImage.cy) / 2));
	szImage.cx = min(szImage.cx, rect.Width());
	szImage.cy = min(szImage.cy, rect.Height());
	pIcon->Draw(pDC, ptIcon, pIcon->GetIcon(), szImage);

	return TRUE;
}

void CXTPReportPaintManager::DrawTreeStructureLine(CDC* pDC, int x, int y, int cx, int cy, COLORREF clr)
{
	if (m_treeStructureStyle == xtpReportTreeStructureDots)
	{
		const unsigned short _cb[] = {0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x55};
		DrawGridPattern(pDC, &m_brushTreeStructure, CRect(x, y, x + cx, y + cy), _cb, m_clrControlDark);
	}
	else
	{
		pDC->FillSolidRect(x, y, cx, cy, clr);
	}
}

void CXTPReportPaintManager::DrawTreeStructure(XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs, XTP_REPORTRECORDITEM_METRICS* pMetrics, CRect rcItem, CSize sizeGlyph)
{
	if (m_treeStructureStyle == xtpReportTreeStructureNone)
		return;

	CDC* pDC = pDrawArgs->pDC;
	CXTPReportRow* pRow = pDrawArgs->pRow;
	COLORREF clrTreeStructure = pMetrics->clrForeground;

	if (pDrawArgs->pControl->IsVirtualMode())
	{
		if (pMetrics->nVirtRowLevel == 0)
			return;
	}
	else
	{
		if (pRow->IsGroupRow() || (m_nTreeStructureStyle == 0
			&& (!pRow->GetParentRow() || pRow->GetParentRow()->IsGroupRow())))
			return;
	}

	int nTreeIndent = m_nTreeIndent;
	int nHeight = rcItem.Height();

	LONG lHorLineOffset = nHeight / 2;
	int nIconAlign = pDrawArgs->nTextAlign & xtpColumnIconMask;
	switch (nIconAlign)
	{
	case xtpColumnIconTop:
		lHorLineOffset = sizeGlyph.cy / 2;
		break;
	case xtpColumnIconBottom:
		lHorLineOffset = rcItem.bottom - rcItem.top - 2 - sizeGlyph.cy / 2;
		break;
	}

	if (m_nTreeStructureStyle > 0)
		rcItem.OffsetRect((sizeGlyph.cy + 1) / 2 + 2, 0);

	if (pDrawArgs->pControl->IsVirtualMode())
	{
		BOOL bLastChild = (pMetrics->nVirtRowFlags & xtpVirtRowLastChild) > 0;
		rcItem.OffsetRect((sizeGlyph.cy + 1) / 2 + 2, 0); //extra shift
		int nVirtIndent = rcItem.left;
		int nLevel = pMetrics->nVirtRowLevel;

		if (nLevel == 1)
		{
			DrawTreeStructureLine(pDC, nVirtIndent, rcItem.top - 1, 1, lHorLineOffset + 1, clrTreeStructure);
			DrawTreeStructureLine(pDC, nVirtIndent, rcItem.top + lHorLineOffset, sizeGlyph.cx / 2 + 1, 1, clrTreeStructure);

			if (!bLastChild)
				DrawTreeStructureLine(pDC, nVirtIndent, rcItem.top - 1, 1, nHeight + 1, clrTreeStructure);
		}

		if (nLevel > 1)
		{
			while (nLevel > 1)
			{
				nVirtIndent -= nTreeIndent;
				if (nLevel % 2 == 1)
				{
					if (bLastChild)
					{
						DrawTreeStructureLine(pDC, nVirtIndent, rcItem.top - 1, 1, lHorLineOffset + 1, clrTreeStructure);
						DrawTreeStructureLine(pDC, nVirtIndent, rcItem.top + lHorLineOffset, sizeGlyph.cx / 2 + 1, 1, clrTreeStructure);
						bLastChild = FALSE;
					}
					else
						DrawTreeStructureLine(pDC, nVirtIndent, rcItem.top - 1, 1, nHeight + 1, clrTreeStructure);
				}
				nLevel /= 2;
			}
		}
	}
	else
	{
		CXTPReportRow* pNextRow = pRow->GetNextSiblingRow();
		if (pNextRow)
			DrawTreeStructureLine(pDC, rcItem.left, rcItem.top - 1, 1, nHeight + 1, clrTreeStructure);
		else
			DrawTreeStructureLine(pDC, rcItem.left, rcItem.top - 1, 1, lHorLineOffset + 1, clrTreeStructure);

		DrawTreeStructureLine(pDC, rcItem.left, rcItem.top + lHorLineOffset, ((!pDrawArgs->pRow->HasChildren() && (m_nTreeColumnLineLength > 0)) ? m_nTreeColumnLineLength : sizeGlyph.cx / 2 + 1), 1, clrTreeStructure);

		for (;;)
		{
			pRow = pRow->GetParentRow();

			if (pRow == NULL || pRow->IsGroupRow() ||
				(m_nTreeStructureStyle == 0
				&& (pRow->GetParentRow() == 0 || pRow->GetParentRow()->IsGroupRow())))
				break;

			rcItem.OffsetRect(-nTreeIndent, 0);

			if (pRow->GetNextSiblingRow())
				DrawTreeStructureLine(pDC, rcItem.left, rcItem.top - 1, 1, rcItem.Height() + 1, clrTreeStructure);
		}
	}
}

int CXTPReportPaintManager::DrawString(int* pnCurrDrawPos, CDC* pDC, const CString& strDraw, CRect rcDraw, UINT nFormat)
{
	int nCharCount = 0;


	switch (nFormat)
	{
		case DT_END_ELLIPSIS:
		{
			// in this case char count only for identify if string printed partially
			*pnCurrDrawPos += GetTextExtentX(pDC,strDraw);
			if (*pnCurrDrawPos >= rcDraw.right)
				nCharCount = - 1;
			else
				nCharCount = strDraw.GetLength();

			pDC->DrawText(strDraw, &rcDraw, DT_LEFT | DT_END_ELLIPSIS | DT_NOPREFIX);
			break;
		}
		case DT_WORDBREAK:
		{
			const TCHAR seps[] = _T(" \t\n");
			TCHAR* lpszContext = 0;
			CString strOut = _T("");
			CString strSrc = strDraw;
			TCHAR* szWord = STRTOK_S(strSrc.GetBuffer(strSrc.GetLength()), seps, &lpszContext);
			int nRightStrMargin = GetTextExtentX(pDC, CString(szWord));

			if (nRightStrMargin > rcDraw.Width())
			{
				strOut = szWord;

			}
			else
			{
				while (szWord != NULL && nRightStrMargin < rcDraw.Width())
				{
					strOut = strOut + szWord + _T(" ");
						szWord = STRTOK_S(NULL, seps, &lpszContext);
						nRightStrMargin = GetTextExtentX(pDC,strOut + szWord);
				}
			}

			nCharCount = strOut.GetLength();
			*pnCurrDrawPos += GetTextExtentX(pDC,strOut);
			pDC->DrawText(strOut, &rcDraw, DT_LEFT | DT_NOPREFIX);
			break;
		}
		case DT_LEFT:
		case DT_RIGHT:
		case DT_CENTER:
		{
			*pnCurrDrawPos += GetTextExtentX(pDC,strDraw);
			if (*pnCurrDrawPos >= rcDraw.right)
				nCharCount = - 1;
			else
				nCharCount = strDraw.GetLength();

			pDC->DrawText(strDraw, &rcDraw, nFormat  | DT_NOPREFIX);
			break;
		}

	} // switch

	return nCharCount;
}

void CXTPReportPaintManager::DrawTextLine(
	XTP_REPORTRECORDITEM_DRAWARGS *pDrawArgs,
	XTP_REPORTRECORDITEM_METRICS  *pMetrics,
	const CString& strText, CRect rcItem, int nFlag, int& nCharCounter, int& nHyperlinkCounter)
{
	CXTPReportControl* pControl = DYNAMIC_DOWNCAST(CXTPReportControl, pDrawArgs->pRow->GetControl());

	if (!pControl || !pControl->IsIconView())
	{
		CDC* pDC = pDrawArgs->pDC;
		CXTPReportRecordItem* pItem = pDrawArgs->pItem;
		UINT nAlingFlag = 0;
		int nHyperlink = nHyperlinkCounter; // Hyperlink index
		int nCharCount = nCharCounter; // Number of actually printed chars
		CXTPReportHyperlink* pHl = NULL;
		int nHyperlinks = pItem->GetHyperlinksCount();

		int nCurrStrPos = nCharCount;
		CRect rcText;
		rcText.CopyRect(&rcItem);

		if (nHyperlinks == 0)
		{
			pDC->DrawText(strText, &rcText, pDrawArgs->nTextAlign | nFlag | DT_NOPREFIX | DT_SINGLELINE);
			return;
		}

		// calculate rect for drawing text for text alignment feature
		pDC->DrawText(strText, &rcText, DT_CALCRECT | DT_NOPREFIX | DT_SINGLELINE);

		if (rcText.Width() < rcItem.Width())
		{
			switch (pDrawArgs->nTextAlign & (DT_RIGHT | DT_CENTER))
			{
				case DT_RIGHT :
					rcText.left = rcItem.right - rcText.Width();
					break;
				case DT_CENTER :
					rcText.left = rcItem.left + (rcItem.Width() - rcText.Width()) / 2;
					break;
			}
		}
		rcText.right = rcItem.right;

		int nCurrDrawPos = rcText.left;

		while (strText.GetLength() > nCurrStrPos && nCurrDrawPos < rcText.right)
		{
			CString strOut;
			if (nHyperlinks > nHyperlink)
			{
				pHl = pItem->GetHyperlinkAt(nHyperlink);
				strOut = strText.Mid(nCurrStrPos, pHl->m_nHyperTextBegin - nCurrStrPos);
				if (strOut.GetLength() > 0) // print str
				{
					// replace LF, CR characters (if any)
					strOut.Remove(_T('\n'));
					strOut.Remove(_T('\r'));

					nCharCount = DrawString(&nCurrDrawPos, pDC, strOut, rcText, nFlag);
					rcText.left = nCurrDrawPos > rcText.right ? rcText.right : nCurrDrawPos;

					// if previous string was cut - stop draw
					if (nCharCount < strOut.GetLength())
					{
						nCharCount += nCurrStrPos;
						break;
					}
				}

				// print link
				nCharCount = DrawLink(&nCurrDrawPos, pDrawArgs, pMetrics, pHl, strText, rcText, nFlag);
				rcText.left = nCurrDrawPos > rcText.right ? rcText.right : nCurrDrawPos;

				// update current position in string
				nCurrStrPos = pHl->m_nHyperTextBegin + pHl->m_nHyperTextLen;
				nHyperlink++;

				// if previous string was cut - stop draw
				if (nCharCount < (pHl->m_nHyperTextLen))
				{
					nCharCount += nCurrStrPos;
					break;
				}
			}
			else
			{

				// print whole string without links or tail of string
				strOut = strText.Mid(nCurrStrPos, strText.GetLength() - nCurrStrPos);
				nCharCount = DrawString(&nCurrDrawPos, pDC, strOut, rcText, nFlag);
				nCharCount += nCurrStrPos;
				break; // all was drawn!
			}
		}

		pDC->SetTextAlign(nAlingFlag);
		nCharCounter = nCharCount;

		return;
	}
	else
	{
		CXTPReportPaintManager::DrawMultiLineText(pDrawArgs, pMetrics, strText, rcItem);
	}
}

void CXTPReportPaintManager::DrawItemCaption(
	XTP_REPORTRECORDITEM_DRAWARGS *pDrawArgs,
	XTP_REPORTRECORDITEM_METRICS  *pMetrics)
{
	CString strText = pMetrics->strText;
	CRect& rcItem = pDrawArgs->rcItem;
	rcItem.DeflateRect(2, 1, 2, 0);

	// draw item text
	if (!strText.IsEmpty())
	{
		CXTPFontDC font(pDrawArgs->pDC, pMetrics->pFont);
		if (!m_bFixedRowHeight)// && (pDrawArgs->nTextAlign & DT_WORDBREAK))
		{
			BOOL bWordBreak = !!(pDrawArgs->nTextAlign & DT_WORDBREAK);

			if (pDrawArgs->nTextAlign & DT_VCENTER)
			{
				// try to center vertically because DT_VCENTER works only for DT_SINGLELINE;

				ReplaceInHyperLinks(pDrawArgs->pItem, strText, XTP_HLINK_SPACE_X);

				int nHeightRequired = CalculateRowHeight(pDrawArgs->pDC, strText, rcItem.Width(), bWordBreak);

				int nHeightDiff = rcItem.Height() - nHeightRequired;

				if (nHeightDiff >= 4)
					rcItem.top += nHeightDiff/2 - 2;

				strText = pMetrics->strText;
			}

			DrawMultiLineText(pDrawArgs, pMetrics, strText, rcItem, bWordBreak);
		}
		else
		{
			int nCharCount = 0;
			int nHyperlink = 0;
			DrawTextLine(pDrawArgs, pMetrics, strText, &rcItem, DT_END_ELLIPSIS, nCharCount, nHyperlink);
		}
	}
}

void CXTPReportPaintManager::DrawItemControl(XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs, CXTPReportRecordItemControl* pControl, CRect& rcItem)
{
	if (!pControl)
		return;

	CFont* pFont = pControl->GetFont();
	if (!pFont || !pFont->GetSafeHandle())
		pFont = &m_fontText;

	CXTPFontDC dc(pDrawArgs->pDC, pControl->GetFont(), pControl->GetCaptionColor());
	CRect rcControl = rcItem;
	// calculate control's rectangle
	// horizontal
	int nControlWidth = pControl->GetSize().cx;
	if (nControlWidth > 0)
	{
		if (pControl->GetAlignment() == xtpItemControlRight)
		{
			rcControl.left = rcControl.right - nControlWidth;
			rcItem.right -= nControlWidth;
		}
		else
		{
			rcControl.right = rcControl.left + nControlWidth;
			rcItem.left += nControlWidth;
		}
	}
	else
	{
		//return;
		if (pControl->GetAlignment() == xtpItemControlRight)
		{
			rcControl.left = rcControl.right;
		}
		else
		{
			rcControl.right = rcControl.left;
		}
	}

	int nControlHeight = pControl->GetSize().cy;
	if (nControlHeight > 0 && nControlHeight < rcItem.Height())
	{
		rcControl.top = rcItem.top + (rcItem.Height() - nControlHeight) / 2;
		rcControl.bottom = rcControl.top + nControlHeight;
	}

	// handle alignment
	// set control's rectangle
	pControl->SetRect(rcControl);
	switch (pControl->GetType())
	{
		case xtpItemControlTypeButton :
		{
			// draw button
			CXTPReportRecordItemButton* pButton = DYNAMIC_DOWNCAST(CXTPReportRecordItemButton, pControl);
			ASSERT(pButton);
			int nState = pButton->GetEnable() ? pButton->GetState() : PBS_DISABLED;

			if (m_bThemedInplaceButtons || pButton->GetThemed())
			{
				m_themeButton->DrawThemeBackground(*pDrawArgs->pDC, BP_PUSHBUTTON, nState, rcControl, 0);
			}
			else
			{
				pDrawArgs->pDC->FillSolidRect(rcControl, GetXtremeColor(COLOR_3DFACE));

				if (nState == PBS_PRESSED)
				{
					pDrawArgs->pDC->Draw3dRect(rcControl, GetXtremeColor(COLOR_3DDKSHADOW), GetXtremeColor(COLOR_WINDOW));
				}
				else
				{
					pDrawArgs->pDC->Draw3dRect(rcControl, GetXtremeColor(COLOR_3DFACE), GetXtremeColor(COLOR_3DDKSHADOW));
					rcControl.DeflateRect(1, 1);
					pDrawArgs->pDC->Draw3dRect(rcControl, GetXtremeColor(COLOR_WINDOW), GetXtremeColor(COLOR_3DSHADOW));
				}
			}
			// calculate rectangle for drawing icon or caption
			if (nState == PBS_PRESSED)
				rcControl.DeflateRect(3, 3, 1, 1);
			else
				rcControl.DeflateRect(1, 1);
			// draw button icon
			int nIconIndex = pButton->GetIconIndex(nState);
			if (nIconIndex != XTP_REPORT_NOICON)
			{
				CXTPImageManagerIcon* pIcon = pDrawArgs->pControl->GetImageManager()->GetImage(nIconIndex, 0);
				if (pIcon)
				{
					CSize szImage(pIcon->GetWidth(), pIcon->GetHeight());
					CPoint ptIcon(max(rcControl.left, rcControl.left + (rcControl.Width() - szImage.cx) / 2),
									max(rcControl.top, rcControl.top + (rcControl.Height() - szImage.cy) / 2));
					szImage.cx = min(szImage.cx, rcControl.Width());
					szImage.cy = min(szImage.cy, rcControl.Height());
					if (pDrawArgs->pDC->IsPrinting())
					{
						CBitmap bmp;
						bmp.CreateCompatibleBitmap(pDrawArgs->pDC, szImage.cx, szImage.cy);
						CXTPCompatibleDC dcMem(NULL, &bmp);
						dcMem.FillSolidRect(0, 0, szImage.cx, szImage.cy, GetXtremeColor(COLOR_3DFACE));
						pIcon->Draw(&dcMem, CPoint(0, 0), pIcon->GetIcon());
						pDrawArgs->pDC->BitBlt(ptIcon.x, ptIcon.y, szImage.cx, szImage.cy, &dcMem, 0, 0, SRCCOPY);
					}
					else
						pIcon->Draw(pDrawArgs->pDC, ptIcon, pIcon->GetIcon(), szImage);
				}
			}
			// draw button caption
			if (!pButton->GetCaption().IsEmpty())
			{
				COLORREF clrPrev = pDrawArgs->pDC->SetTextColor(pButton->GetEnable() ? pButton->GetCaptionColor() : GetXtremeColor(XPCOLOR_GRAYTEXT));
				// Issue 20355 - fix
				int iAl = pButton->GetAlignment();
				if (iAl == xtpItemControlLeft)
					pDrawArgs->pDC->DrawText(pButton->GetCaption(), &rcControl, DT_LEFT | DT_VCENTER | DT_NOPREFIX | DT_SINGLELINE);
				else if (iAl == xtpItemControlRight)
					pDrawArgs->pDC->DrawText(pButton->GetCaption(), &rcControl, DT_RIGHT | DT_VCENTER | DT_NOPREFIX | DT_SINGLELINE);
				else
					pDrawArgs->pDC->DrawText(pButton->GetCaption(), &rcControl, DT_CENTER | DT_VCENTER | DT_NOPREFIX | DT_SINGLELINE);

				pDrawArgs->pDC->SetTextColor(clrPrev);
			}
			break;
		}
	}
}

void CXTPReportPaintManager::ReplaceInHyperLinks(CXTPReportRecordItem* pItem, CString& rstrText, TCHAR chReplace )
{
	ASSERT(pItem);
	if (!pItem)
		return;

	int nHyperlinks = pItem->GetHyperlinksCount();
	for (int nHLink = 0; nHLink < nHyperlinks; nHLink++)
	{
		CXTPReportHyperlink* pHLink = pItem->GetHyperlinkAt(nHLink);
		ASSERT(pHLink);
		if (!pHLink)
			continue;

		int nI0 = max(0, min(pHLink->m_nHyperTextBegin, rstrText.GetLength()-1));
		int nI1 = max(0, min(pHLink->m_nHyperTextBegin + pHLink->m_nHyperTextLen, rstrText.GetLength()));
		for (int i = nI0; i < nI1; i++)
		{
			if (rstrText.GetAt(i) == _T(' '))
			{
				rstrText.SetAt(i, chReplace);
			}
		}
	}
}

void CXTPReportPaintManager::DrawMultiLineText(
	XTP_REPORTRECORDITEM_DRAWARGS *pDrawArgs,
	XTP_REPORTRECORDITEM_METRICS  *pMetrics,
	const CString& strText,
	CRect rcItem, BOOL bWordBreak)
{
	CDC* pDC = pDrawArgs->pDC;
	CXTPReportRecordItem* pItem = pDrawArgs->pItem;

	int nCurrStrPos = 0, nStringWidth = 0, nWordWidth = 0;
	int nStartString = -1, nEndString = -1;

	CRect rcText;
	rcText.CopyRect(&rcItem);

	if (0 == pItem->GetHyperlinksCount()) // no hyperlinks
	{
		if (strText.GetLength() > 0)
		{
			if (bWordBreak)
				pDC->DrawText(strText, &rcText, DT_NOPREFIX | pDrawArgs->nTextAlign | DT_WORD_ELLIPSIS | DT_WORDBREAK);
			else
				pDC->DrawText(strText, &rcText, DT_NOPREFIX | pDrawArgs->nTextAlign | DT_WORD_ELLIPSIS);
		}
		return;
	}

	int nLineHeight = pDC->GetTextExtent(_T(" "), 1).cy;
	CString strSrc = strText + _T(" ");
	strSrc.Replace(_T('\r'), _T(' '));

	ReplaceInHyperLinks(pItem, strSrc, XTP_HLINK_SPACE_X);

	CString strWord, strString;
	int nTextLength = strSrc.GetLength();

	TCHAR* lpszBuff = strSrc.GetBuffer(nTextLength + 1);
	TCHAR  cChar = lpszBuff[nCurrStrPos];

	int     nWordLength = 0;
	TCHAR*  lpszWord = strWord.GetBuffer(nTextLength + 1);

	// parsing for the words (tokens)
	while (_T('\0') != (cChar = lpszBuff[nCurrStrPos++]))
	{
		if (cChar == _T(' ') || cChar == _T('\t') || cChar == _T('\n'))
		{
			if (lpszWord[0] == _T('\0') && cChar != _T('\n'))
				continue;

			if (strString.IsEmpty())
			{
				nStartString = nCurrStrPos - nWordLength;
				nEndString = nCurrStrPos;
			}
			nWordWidth = GetTextExtentX(pDC, lpszWord, nWordLength);

			if (nWordWidth > rcItem.Width() && bWordBreak || cChar == _T('\n'))
			{
				if (!strString.IsEmpty())
				{
					DrawSingleLineText(pDrawArgs, pMetrics, strSrc , rcText, nStartString-1, nEndString-1,
						GetTextExtentX(pDC,strString));

					rcText.top += nLineHeight;
					rcText.left = rcItem.left;
				}

				// the word exceeds the cell's width
				DrawSingleLineText(pDrawArgs, pMetrics, strSrc , rcText, nCurrStrPos - nWordLength -1, nCurrStrPos -1, nWordWidth);

				if ((nCurrStrPos - nWordLength) != nCurrStrPos)
				{
					rcText.top += nLineHeight;
					rcText.left = rcItem.left;
				}

				strString.Empty();

				lpszWord[0] = _T('\0');
				nWordLength = 0;
				continue;
			}

			int nPrevStringWidth = GetTextExtentX(pDC,strString);

			if (!strString.IsEmpty())
				strString += _T(" ");

			strString += lpszWord;
			nStringWidth = GetTextExtentX(pDC,strString);

			if (nStringWidth > rcItem.Width() && bWordBreak)
			{
				DrawSingleLineText(pDrawArgs, pMetrics, strSrc , rcText, nStartString-1, nEndString-1,
					nPrevStringWidth);

				rcText.top += nLineHeight;
				rcText.left = rcItem.left;


				nStartString = nCurrStrPos - nWordLength;
				strString = lpszWord;
			}

			nEndString = nCurrStrPos;
			lpszWord[0] = _T('\0');
			nWordLength = 0;
			continue;
		}

		ASSERT(nWordLength <= nTextLength);
		lpszWord[nWordLength] = cChar;
		lpszWord[nWordLength + 1] = _T('\0');

		++nWordLength;
	}

	// the rest of the string
	if (strString.GetLength())
	{
		DrawSingleLineText(pDrawArgs, pMetrics, strSrc , rcText, nStartString-1, strText.GetLength(),
			GetTextExtentX(pDC,strString));
	}
}

void CXTPReportPaintManager::DrawSingleLineText(
	XTP_REPORTRECORDITEM_DRAWARGS *pDrawArgs,
	XTP_REPORTRECORDITEM_METRICS  *pMetrics,
	const CString& strText,
	CRect rcItem, int nStartPos, int nEndPos,int nActualWidth)
{

	CDC* pDC = pDrawArgs->pDC;
	CXTPReportRecordItem* pItem = pDrawArgs->pItem;
	CXTPReportHyperlink* pHl = NULL;

	// alignment
	CRect   rcText(rcItem);
	int nFlag = pDrawArgs->nTextAlign & (DT_LEFT | DT_RIGHT | DT_CENTER);

	if (nFlag & DT_RIGHT)
		rcText.left = rcText.right - nActualWidth;
	else if (nFlag & DT_CENTER)
		rcText.left += (rcText.Width() - nActualWidth) / 2;

	// Left align the string, that exceeds the cell's width.
	rcText.left = max(rcText.left, rcItem.left);

	int nCurrDrawPos = rcText.left;
	int nHyperlink = 0; // Hyperlink index
	int nCurrStrPos = nStartPos;
	int nBeginHyperlink = 0xFFFF;
	int nHyperlinks = pItem->GetHyperlinksCount();

	CString strWord, strPre, strPost, strSrc = strText + _T(" ");
	TCHAR* lpszBuff = strSrc.GetBuffer(strSrc.GetLength());
	TCHAR  cChar = lpszBuff[nCurrStrPos];
	BOOL  bNeedsBlank = FALSE;

	int   nBlankWidth = GetTextExtentX(pDC, _T(" "), 1);

	while (nCurrStrPos <= nEndPos && _T('\0') != (cChar = lpszBuff[nCurrStrPos++]))
	{
		if (cChar == _T(' ') || cChar == _T('\t') || cChar == _T('\n'))
		{
			if (strWord.IsEmpty())
				continue;

			if (bNeedsBlank)
			{
				// between words
				pDC->DrawText(_T(" "), &rcText, DT_LEFT);
				rcText.left += nBlankWidth;
			}

			// as hyperlinks are not sorted in the array, check all
			for (nHyperlink = 0; nHyperlink < nHyperlinks; ++nHyperlink)
			{
				pHl = pItem->GetHyperlinkAt(nHyperlink);
				nBeginHyperlink = pHl->m_nHyperTextBegin;

				// validate the hyperlink
				if (nBeginHyperlink >= 0 &&
					nBeginHyperlink <= strText.GetLength() - 1                  &&
					pHl->m_nHyperTextLen > 1                                    &&
					nBeginHyperlink + pHl->m_nHyperTextLen <= strText.GetLength())
				{
					// particular case: hyperlink begins with blank(s)
					BOOL bInclude = FALSE;
					if (nStartPos > nBeginHyperlink )
					{
						CString strExtracted = strText.Mid(nBeginHyperlink, nStartPos - nBeginHyperlink);
						if (_tcsspnp(strExtracted.GetBuffer(strExtracted.GetLength()),_T(" ")) == NULL)
							bInclude = TRUE;
					}

					// does the hyperlink belong to the string
					if (nBeginHyperlink >= nStartPos &&
						nBeginHyperlink + pHl->m_nHyperTextLen <= nEndPos ||
						bInclude)
					{

						// does the word contain the hyperlink
						CString strExtracted = strText.Mid(nBeginHyperlink,pHl->m_nHyperTextLen);
						strExtracted.TrimLeft();
						if (strWord.Find(strExtracted) != -1)
						{
							if (nBeginHyperlink < nCurrStrPos)
							{
								// before hyperlink
								strPre = strWord.Mid(0,strWord.GetLength() - (nCurrStrPos - nBeginHyperlink - 1));
								if (!strPre.IsEmpty())
								{
									pDC->DrawText(strPre, &rcText, DT_LEFT | DT_END_ELLIPSIS | DT_NOPREFIX);
									rcText.left += GetTextExtentX(pDC,strPre);
								}

								// the hyperlink
								nCurrDrawPos = rcText.left;
								DrawLink(&nCurrDrawPos,pDrawArgs,pMetrics,pHl,strText,rcText, DT_LEFT | DT_END_ELLIPSIS, TRUE);
								rcText.left = nCurrDrawPos;

								// after hyperlink
								int n = strWord.GetLength() - (nCurrStrPos - (nBeginHyperlink+pHl->m_nHyperTextLen +1));
								if (n >=0 && (strWord.GetLength() - n) > 0)
								{
									strPost = strWord.Mid(n,strWord.GetLength() - n);
									if (!strPost.IsEmpty())
									{
										pDC->DrawText(strPost, &rcText, DT_LEFT | DT_END_ELLIPSIS | DT_NOPREFIX);
										rcText.left += GetTextExtentX(pDC,strPost);
									}
								}

								strWord.Empty();
								bNeedsBlank = TRUE;
								continue;
							}
						}
					}
				}
			} // the hyperlink validation

			// non-hyperlink words
			pDC->DrawText(strWord, &rcText, DT_LEFT | DT_END_ELLIPSIS | DT_NOPREFIX);
			rcText.left += GetTextExtentX(pDC,strWord);

			strWord.Empty();
			bNeedsBlank = TRUE;
			continue;
		}

		strWord += cChar;
	}
}

int CXTPReportPaintManager::CalculateRowHeight(CDC* pDC, const CString& strText,
							int nMaxWidth, BOOL bWordBreak)
{
	CString strWord, strBuff(strText);
	strBuff += _T(" ");

	int nLineHeight = pDC->GetTextExtent(_T(" "),1).cy;

	// just calculate new line characters
	if (!bWordBreak)
	{
		int nCount = 0, nPos = -1;

		do
		{
			nCount++;
			nPos = strBuff.Find(_T('\n'), nPos + 1);
		}
		while (nPos != -1);

		return nLineHeight * nCount;
	}

	CRect rc(0, 0, nMaxWidth, nLineHeight);
	//int h = pDC->DrawText(strBuff, &rc, DT_CALCRECT | DT_NOPREFIX | DT_WORDBREAK);
	int h = pDC->DrawText(strBuff, &rc, DT_CALCRECT | DT_NOPREFIX | DT_WORDBREAK | DT_WORD_ELLIPSIS);
	return max(nLineHeight, h);
}

CRect CXTPReportPaintManager::CalculateMaxTextRect(CDC* pDC, const CString& strText,
			const CRect& rcMaxRect, BOOL bWordBreak, BOOL bLimitSize, UINT uiFlags)
{
	CRect rc(rcMaxRect);
	pDC->DrawText(strText, rc, DT_CALCRECT | DT_NOPREFIX | ((bWordBreak) ? DT_WORDBREAK : 0) | uiFlags);

	if (bLimitSize)
	{
		rc.bottom = rc.top + min(rc.Height(), rcMaxRect.Height());
		rc.right = rc.left + min(rc.Width(), rcMaxRect.Width());
	}
	return rc;
}

int CXTPReportPaintManager::DrawLink(
	int *pnCurrDrawPos,
	XTP_REPORTRECORDITEM_DRAWARGS *pDrawArgs,
	XTP_REPORTRECORDITEM_METRICS  *pMetrics,
	CXTPReportHyperlink *pHyperlink,
	CString strText, CRect rcLink, int nFlag, BOOL bTrim)
{
	int nCount = pHyperlink->m_nHyperTextLen;

	int nDiff = pHyperlink->m_nHyperTextBegin + pHyperlink->m_nHyperTextLen - strText.GetLength();
	if (nDiff > 0)
		nCount = max(0, pHyperlink->m_nHyperTextLen - nDiff);

	if (pHyperlink->m_nHyperTextBegin < 0 || pHyperlink->m_nHyperTextBegin >= strText.GetLength() ||
		nCount <= 0)
		return 0;

	CDC* pDC = pDrawArgs->pDC;
	CString strOut = strText.Mid(pHyperlink->m_nHyperTextBegin, nCount);

	if (bTrim)
	{
		strOut.TrimLeft();
		strOut.TrimRight();
	}
	strOut.Replace(XTP_HLINK_SPACE_X, _T(' '));

	BOOL bSelected = pDrawArgs->pRow->IsSelected() && pDrawArgs->pRow->GetControl()->HasFocus() &&
		(!pDrawArgs->pColumn || !pDrawArgs->pRow->GetControl()->GetFocusedColumn() ||
		  pDrawArgs->pColumn != pDrawArgs->pRow->GetControl()->GetFocusedColumn());

	LOGFONT lfLink;
	pMetrics->pFont->GetLogFont(&lfLink);

	COLORREF crLink(RGB(0,0,255));

	if (bSelected)
	{
		crLink = m_clrHighlightText;
	}
	else
	{
		if (pDrawArgs->pControl->m_pHotHyperlink == pHyperlink)
		{
			if (xtpReportTextDecorationUnderline == m_pHyperlinkHoverStyle->GetTextDecoration())
			{
				lfLink.lfUnderline = TRUE;
			}
			crLink = m_pHyperlinkHoverStyle->GetColor();
		}
		else
		{
			if (xtpReportTextDecorationUnderline == m_pHyperlinkStyle->GetTextDecoration())
			{
				lfLink.lfUnderline = TRUE;
			}
			crLink = m_pHyperlinkStyle->GetColor();
		}
	}

	CFont fontLink;
	fontLink.CreateFontIndirect(&lfLink);
	CXTPFontDC fontDC(pDC, &fontLink, crLink);


	TEXTMETRIC textMetrics;
	pDC->GetTextMetrics(&textMetrics);
	int nCharCount = 0;
	//!pHyperlink->m_rcHyperSpot.left = pDC->GetCurrentPosition().x; // update hyperlink left bound before drawing
	pHyperlink->m_rcHyperSpot.left = *pnCurrDrawPos;
	nCharCount = DrawString(pnCurrDrawPos, pDC, strOut, rcLink, nFlag);

	// update hyperlink spot
	pHyperlink->m_rcHyperSpot.top = rcLink.top;
	//!pHyperlink->m_rcHyperSpot.right = pDC->GetCurrentPosition().x;
	pHyperlink->m_rcHyperSpot.right = *pnCurrDrawPos;
	pHyperlink->m_rcHyperSpot.bottom = rcLink.top + textMetrics.tmHeight;

	return nCharCount;
}

void CXTPReportPaintManager::DrawSectionDivider(
	CDC *pDC, const CRect& rc,
	XTPReportSectionDividerPosition dividerPosition,
	XTPReportSectionDividerStyle    dividerStyle,
	CXTPPaintManagerColor           clrDivider)
{
	switch(dividerStyle)
	{
		case xtpReportFixedRowsDividerNone:
		{
			// Nothing
		}
		break;

		case xtpReportFixedRowsDividerOutlook:
		{
			if (m_columnStyle == xtpReportColumnShaded)
			{
				pDC->FillSolidRect(rc, m_clrHeaderControl);
				DrawHorizontalLine(pDC, rc.left, rc.bottom - 2, rc.Width(), MixColor(m_clrHeaderControl, m_clrControlDark, 0.4));
				DrawHorizontalLine(pDC, rc.left, rc.bottom - 1, rc.Width(), MixColor(m_clrHeaderControl, m_clrControlDark, 0.6));
			}
			else if (m_columnStyle == xtpReportColumnOffice2003)
			{
				XTPDrawHelpers()->GradientFill(pDC, rc,m_grcGradientColumn.clrLight, m_grcGradientColumn.clrDark,FALSE);
				DrawHorizontalLine(pDC, rc.left, rc.bottom - 1, rc.Width(), m_clrGradientColumnShadow);
			}
			else if (m_columnStyle == xtpReportColumnResource)
			{
				pDC->FillSolidRect(rc,  MixColor(m_grcGradientColumn.clrLight, m_grcGradientColumn.clrDark, 0.4));
				DrawHorizontalLine(pDC, rc.left, rc.top, rc.Width(), RGB(255, 255, 255));
				DrawHorizontalLine(pDC, rc.left, rc.bottom - 1, rc.Width(), m_clrGradientColumnSeparator);
			}
			else if (m_columnStyle == xtpReportColumnExplorer)
			{
				pDC->FillSolidRect(rc, m_clrHeaderControl);
				pDC->Draw3dRect(rc, m_clrControlLightLight, m_clrBtnText);

				CRect rcTmp(rc);
				rcTmp.DeflateRect(1, 1);
				pDC->Draw3dRect(rcTmp, m_clrHeaderControl, m_clrControlDark);
			}
			else
			{
				pDC->FillSolidRect(rc, m_clrHeaderControl);
				pDC->Draw3dRect(rc, m_clrControlLightLight, m_clrControlDark);
			}
		}
		break;

		case xtpReportFixedRowsDividerThin:
		case xtpReportFixedRowsDividerBold:
		{
			pDC->FillSolidRect(rc, clrDivider);
		}
		break;

		case xtpReportFixedRowsDividerShade:
		{
			int nScrollPos   = m_pControl->GetScrollPos  (SB_VERT);
			int nScrollLimit = m_pControl->GetScrollLimit(SB_VERT);

			if ((xtpReportSectionDividerPositionBottom == dividerPosition) && (nScrollPos > 0))
			{
				CRect rcShade(rc);
				rcShade.top = rcShade.bottom;
				rcShade.bottom += 4;

				XTPDrawHelpers()->GradientFill(pDC, rcShade, clrDivider, RGB(255, 255, 255), FALSE);
			}

			if ((xtpReportSectionDividerPositionTop == dividerPosition) && (nScrollPos != nScrollLimit))
			{
				CRect rcShade(rc);
				rcShade.bottom = rcShade.top;
				rcShade.top -= 4;

				XTPDrawHelpers()->GradientFill(pDC, rcShade, RGB(255, 255, 255), clrDivider, FALSE);
			}
		}
		break;
	}
}

int CXTPReportPaintManager::GetSectionDividerHeight(XTPReportFixedRowsDividerStyle dividerStyle)
{
	int nHeight = 0;

	switch (dividerStyle & ~xtpReportFixedRowsDividerShade)
	{
		case xtpReportFixedRowsDividerNone:     nHeight = 0; break;
		case xtpReportFixedRowsDividerThin:     nHeight = 1; break;
		case xtpReportFixedRowsDividerBold:     nHeight = 2; break;
		case xtpReportFixedRowsDividerOutlook:  nHeight = 8; break;
	}

	return nHeight;
}

BOOL CXTPReportPaintManager::IsGridVisible(BOOL bVertical) const
{
	return IsGridVisible(bVertical ? xtpReportOrientationVertical : xtpReportOrientationHorizontal);
}

BOOL CXTPReportPaintManager::IsGridVisible(XTPReportOrientation orientation) const
{
	switch(orientation)
	{
	case xtpReportOrientationHorizontal:
		return xtpReportGridNoLines != GetGridStyle(FALSE);

	case xtpReportOrientationVertical:
		return xtpReportGridNoLines != GetGridStyle(TRUE);

	default:
		return FALSE;
	}
}

int CXTPReportPaintManager::GetPrintPageWidth()
{
	if (0 == m_PrintPageWidth)
	{
		CPrintInfo printInfo;
		CXTPReportViewPrintOptions printOptions;

		if (AfxGetApp()->GetPrinterDeviceDefaults(&printInfo.m_pPD->m_pd))
		{
			if (NULL == printInfo.m_pPD->m_pd.hDC)
			{
				printInfo.m_pPD->CreatePrinterDC();
				ASSERT(NULL != printInfo.m_pPD->m_pd.hDC);
			}

			CPreviewDC dc;
			dc.SetAttribDC(printInfo.m_pPD->m_pd.hDC);

			int nResHorz = dc.GetDeviceCaps(HORZRES);
			int nResVert = dc.GetDeviceCaps(VERTRES);

			int nLogPixelsX = dc.GetDeviceCaps(LOGPIXELSX);
			int nLogPixelsY = dc.GetDeviceCaps(LOGPIXELSY);

			printInfo.m_rectDraw.SetRect(0, 0, nResHorz, nResVert);

			dc.SetMapMode(MM_ANISOTROPIC);
			dc.SetViewportExt(nLogPixelsX, nLogPixelsY);
			dc.SetWindowExt(96, 96);
			dc.DPtoLP(&printInfo.m_rectDraw);
			dc.ReleaseAttribDC();

			CDC *pDC = m_pControl->GetDC();
			CRect rcMargins = printOptions.GetMarginsLP(pDC);
			m_pControl->ReleaseDC(pDC);

			printInfo.m_rectDraw.DeflateRect(rcMargins);
			m_PrintPageWidth = printInfo.m_rectDraw.Width();
		}
	}

	return m_PrintPageWidth;
}

COLORREF CXTPReportPaintManager::GetGridColor() const
{
	return GetDefaultItemBorder()->GetBorderColor();
}

COLORREF CXTPReportPaintManager::SetGridColor(COLORREF clrGridLine)
{
	COLORREF clrOldColor = GetGridColor();
	GetDefaultItemBorder()->SetBorderColor(clrGridLine);
	return clrOldColor;
}

CXTPReportBorder* CXTPReportPaintManager::GetDefaultItemBorder() const
{
	return m_pBorder;
}

CXTPReportBorder* CXTPReportPaintManager::GetItemBorder(const CXTPReportRecordItem *pItem) const
{
	CXTPReportBorder *pBorder = NULL;

	if (NULL != pItem)
	{
		pBorder = pItem->GetBorder();
	}

	if (NULL == pBorder)
	{
		pBorder = GetDefaultItemBorder();
	}

	return pBorder;
}

