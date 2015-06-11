// XTPTabPaintManager.cpp: implementation of the CXTPTabPaintManager class.
//
// This file is a part of the XTREME TOOLKIT PRO MFC class library.
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
#include "Common/XTPVC80Helpers.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPMarkupRender.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPDrawHelpers.h"

#include <TabManager/XTPTabManagerDefines.h>
#include <TabManager/XTPTabManagerAtom.h>
#include <TabManager/XTPTabManagerNavigateButton.h>
#include <TabManager/XTPTabManagerNavigateButtons.h>
#include <TabManager/XTPTabManagerItem.h>
#include <TabManager/XTPTabManager.h>
#include <TabManager/XTPTabPaintManagerColorSet.h>
#include <TabManager/XTPTabPaintManagerTheme.h>
#include <TabManager/XTPTabPaintManager.h>

#include <TabManager/Themes/XTPTabThemeStateButtons.h>
#include <TabManager/Themes/XTPTabThemeFlat.h>
#include <TabManager/Themes/XTPTabThemeOffice2000.h>
#include <TabManager/Themes/XTPTabThemePropertyPageSelected.h>
#include <TabManager/Themes/XTPTabThemePropertyPageFlat.h>
#include <TabManager/Themes/XTPTabThemePropertyPage2003.h>
#include <TabManager/Themes/XTPTabThemePropertyPage2007.h>
#include <TabManager/Themes/XTPTabThemePropertyPageAccess2007.h>
#include <TabManager/Themes/XTPTabThemeExcel2003.h>
#include <TabManager/Themes/XTPTabThemeVisio.h>
#include <TabManager/Themes/XTPTabThemeVisualStudio2003.h>
#include <TabManager/Themes/XTPTabThemeVisualStudio2005.h>
#include <TabManager/Themes/XTPTabThemeVisualStudio2010.h>
#include <TabManager/Themes/XTPTabThemeVisualStudio2012.h>
#include <TabManager/Themes/XTPTabThemeOffice2013.h>

#include <TabManager/ColorSets/XTPTabColorSetDefault.h>
#include <TabManager/ColorSets/XTPTabColorSetOffice2003.h>
#include <TabManager/ColorSets/XTPTabColorSetOffice2007.h>
#include <TabManager/ColorSets/XTPTabColorSetOffice2013.h>
#include <TabManager/ColorSets/XTPTabColorSetOffice2007Access.h>
#include <TabManager/ColorSets/XTPTabColorSetVisualStudio2003.h>
#include <TabManager/ColorSets/XTPTabColorSetVisualStudio2005.h>
#include <TabManager/ColorSets/XTPTabColorSetVisualStudio2008.h>
#include <TabManager/ColorSets/XTPTabColorSetVisualStudio2010.h>
#include <TabManager/ColorSets/XTPTabColorSetVisualStudio2012.h>
#include <TabManager/ColorSets/XTPTabColorSetVisualStudio2012Dark.h>
#include <TabManager/ColorSets/XTPTabColorSetWinNative.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

DWORD CXTPTabPaintManager::m_nSelectOnDragOverDelay = 300;

//////////////////////////////////////////////////////////////////////
// CXTPTabPaintManager


CXTPTabPaintManager::CXTPTabPaintManager()
{
	m_bUseStandardFont = TRUE;

	m_pColorSet = NULL;
	m_pAppearanceSet = NULL;

	SetAppearance(xtpTabAppearancePropertyPage);

	m_bHotTracking = FALSE;
	m_bBoldNormal = m_bBoldSelected = FALSE;
	m_bShowIcons = TRUE;
	m_bSelectOnButtonDown = TRUE;

	m_bDisableLunaColors = FALSE;
	m_bOneNoteColors = FALSE;

	m_tabPosition = xtpTabPositionTop;
	m_tabLayout = xtpTabLayoutAutoSize;

	m_clientFrame = xtpTabFrameBorder;
	m_bStaticFrame = FALSE;

	m_bInvertGradient = FALSE;
	m_bFillBackground = TRUE;
	m_bDrawFocusRect = TRUE;

	m_rcClientMargin.SetRect(0, 0, 0, 0);
	m_rcControlMargin.SetRect(0, 0, 0, 0);
	m_rcButtonMargin.SetRect(0, 0, 0, 0);

	m_nButtonExtraLength = 0;
	m_szIcon = CSize(16, 16);

	m_nFixedTabWidth = 70;
	m_nMinTabWidth = m_nMaxTabWidth = 0;


	m_bDrawTextEndEllipsis = TRUE;
	m_bDrawTextPathEllipsis = FALSE;
	m_bDrawTextNoPrefix = TRUE;
	m_bDrawTextHidePrefix = FALSE;

	m_bMultiRowFixedSelection = FALSE;
	m_bMultiRowJustified = TRUE;

	m_toolBehaviour = xtpTabToolTipShrinkedOnly;
	m_bSelectOnDragOver = 2;
	m_nItemColor = 0;

	m_bClearTypeTextQuality = FALSE;
	m_bRotateImageOnVerticalDraw = FALSE;

	m_rcButtonTextPadding.SetRect(6, 0, 6, 0);

	m_bClipHeader = TRUE;

	m_nDrawTextFormat = DT_LEFT | DT_VCENTER;

	m_szNavigateButton = CSize(14, 15);

	m_bShowTabs = TRUE;

	m_bMultiRowTabsOptimization = TRUE;

	m_bVerticalTextTopToBottom = TRUE;


	EnableAutomation();

#ifdef _XTP_ACTIVEX
	EnableTypeLib();
#endif


	RefreshMetrics();
}

CXTPTabPaintManager::~CXTPTabPaintManager()
{
	SAFE_DELETE(m_pColorSet);
	SAFE_DELETE(m_pAppearanceSet);
}

CXTPTabPaintManagerColorSet* CXTPTabPaintManager::SetColor(XTPTabColorStyle tabColor)
{
	CXTPTabPaintManagerColorSet* pColorSet = NULL;

	m_tabColor = tabColor;

	switch (tabColor)
	{
	case xtpTabColorWinNative:            pColorSet = new CColorSetWinNative();                  break;
	case xtpTabColorResource:             pColorSet = new CXTPTabColorSetOffice2007();           break;
	case xtpTabColorOffice2003:           pColorSet = new CXTPTabColorSetOffice2003();           break;
	case xtpTabColorOffice2007Access:     pColorSet = new CXTPTabColorSetOffice2007Access();     break;
	case xtpTabColorVisualStudio2003:     pColorSet = new CColorSetVisualStudio2003();           break;
	case xtpTabColorVisualStudio2005:     pColorSet = new CColorSetVisualStudio2005();           break;
	case xtpTabColorVisualStudio2008:     pColorSet = new CXTPTabColorSetVisualStudio2008();     break;
	case xtpTabColorVisualStudio2010:     pColorSet = new CXTPTabColorSetVisualStudio2010();     break;
	case xtpTabColorVisualStudio2012:     pColorSet = new CXTPTabColorSetVisualStudio2012();     break;
	case xtpTabColorVisualStudio2012Dark: pColorSet = new CXTPTabColorSetVisualStudio2012Dark(); break;
	case xtpTabColorOffice2013:           pColorSet = new CXTPTabColorSetOffice2013();           break;
	default:                              pColorSet = new CXTPTabColorSetDefault();              break;
	}

	return SetColorSet(pColorSet);
}
CXTPTabPaintManagerColorSet* CXTPTabPaintManager::SetColorSet(CXTPTabPaintManagerColorSet* pColorSet)
{
	if (pColorSet)
	{
		if (m_pColorSet) delete m_pColorSet;
		m_pColorSet = pColorSet;

		pColorSet->m_pPaintManager = this;
		pColorSet->RefreshMetrics();
	}
	OnPropertyChanged();

	return pColorSet;
}

CXTPTabPaintManagerTheme* CXTPTabPaintManager::SetAppearance(XTPTabAppearanceStyle tabAppearance)
{
	CXTPTabPaintManagerTheme* pAppearanceSet = NULL;

	m_tabAppearance = tabAppearance;

	if (tabAppearance == xtpTabThemeExcel2003)
		pAppearanceSet = new CXTPTabThemeExcel2003();
	else if (tabAppearance == xtpTabAppearancePropertyPageFlat)
		pAppearanceSet = new CAppearanceSetPropertyPageFlat();
	else if (tabAppearance == xtpTabAppearanceFlat)
		pAppearanceSet = new CXTPTabThemeFlat();
	else if (tabAppearance == xtpTabAppearanceStateButtons)
		pAppearanceSet = new CAppearanceSetStateButtons();
	else if (tabAppearance == xtpTabAppearancePropertyPageSelected)
		pAppearanceSet = new CAppearanceSetPropertyPageSelected();
	else if (tabAppearance == xtpTabAppearancePropertyPage2003)
		pAppearanceSet = new CAppearanceSetPropertyPage2003();
	else if (tabAppearance == xtpTabAppearanceVisio)
		pAppearanceSet = new CAppearanceSetVisio();
	else if (tabAppearance == xtpTabAppearancePropertyPage2007)
		pAppearanceSet = new CAppearanceSetPropertyPage2007();
	else if (tabAppearance == xtpTabAppearancePropertyPageAccess2007)
		pAppearanceSet = new CAppearanceSetPropertyPageAccess2007();
	else if (tabAppearance == xtpTabThemeVisualStudio2003)
		pAppearanceSet = new CXTPTabThemeVisualStudio2003();
	else if (tabAppearance == xtpTabThemeVisualStudio2005)
		pAppearanceSet = new CAppearanceSetVisualStudio2005();
	else if (tabAppearance == xtpTabAppearanceVisualStudio2010)
		pAppearanceSet = new CXTPTabThemeVisualStudio2010();
	else if (tabAppearance == xtpTabAppearanceVisualStudio2012)
		pAppearanceSet = new CXTPTabThemeVisualStudio2012();
	else if (tabAppearance == xtpTabThemeOffice2013)
		pAppearanceSet = new CXTPTabThemeOffice2013();
	else
		pAppearanceSet = new CXTPTabThemeOffice2000();

	return SetAppearanceSet(pAppearanceSet);

}
CXTPTabPaintManagerTheme* CXTPTabPaintManager::SetAppearanceSet(CXTPTabPaintManagerTheme* pAppearanceSet)
{
	if (pAppearanceSet)
	{
		SAFE_DELETE(m_pAppearanceSet);
		m_pAppearanceSet = pAppearanceSet;

		pAppearanceSet->m_pPaintManager = this;
		pAppearanceSet->RefreshMetrics();

		SetColor(pAppearanceSet->GetDefaultColorSet());
	}
	OnPropertyChanged();

	return pAppearanceSet;

}


void CXTPTabPaintManager::SetFontIndirect(LOGFONT* pLogFont, BOOL bUseStandardFont /*= FALSE*/)
{
	m_bUseStandardFont = bUseStandardFont;

	if (!pLogFont)
		return;

	if (m_bClearTypeTextQuality && XTPSystemVersion()->IsClearTypeTextQualitySupported())
	{
		pLogFont->lfQuality = 5;
	}

	m_fntNormal.DeleteObject();
	m_fntBold.DeleteObject();
	m_fntVerticalNormal.DeleteObject();
	m_fntVerticalBold.DeleteObject();

	pLogFont->lfWeight = m_bBoldNormal ? FW_BOLD : FW_NORMAL;
	VERIFY(m_fntNormal.CreateFontIndirect(pLogFont));

	pLogFont->lfWeight = FW_BOLD;
	VERIFY(m_fntBold.CreateFontIndirect(pLogFont));

	STRCPY_S(pLogFont->lfFaceName, LF_FACESIZE, CXTPDrawHelpers::GetVerticalFontName(FALSE));

	if (m_bVerticalTextTopToBottom)
	{
		pLogFont->lfOrientation = 900;
		pLogFont->lfEscapement = 2700;
	}
	else
	{
		pLogFont->lfOrientation = 900;
		pLogFont->lfEscapement = 900;
	}
	pLogFont->lfHeight = pLogFont->lfHeight < 0 ? __min(-11, pLogFont->lfHeight) : pLogFont->lfHeight;
	pLogFont->lfWeight = m_bBoldNormal ? FW_BOLD : FW_NORMAL;
	VERIFY(m_fntVerticalNormal.CreateFontIndirect(pLogFont));

	pLogFont->lfWeight = FW_BOLD;
	VERIFY(m_fntVerticalBold.CreateFontIndirect(pLogFont));
}

void CXTPTabPaintManager::RefreshMetrics()
{
	if (m_bUseStandardFont)
	{
		LOGFONT lfIcon;
		VERIFY(CXTPDrawHelpers::GetIconLogFont(&lfIcon));

		SetFontIndirect(&lfIcon, TRUE);
	}

	m_pColorSet->RefreshMetrics();
	m_pAppearanceSet->RefreshMetrics();
}

void CXTPTabPaintManager::DisableLunaColors(BOOL bDisableLunaColors)
{
	m_bDisableLunaColors = bDisableLunaColors;
	RefreshMetrics();
	OnPropertyChanged();
}
BOOL CXTPTabPaintManager::IsLunaColorsDisabled() const
{
	return m_bDisableLunaColors || XTPColorManager()->IsLunaColorsDisabled();
}

COLORREF CXTPTabPaintManager::GetOneNoteColor(XTPTabOneNoteColor tabColor)
{
	const COLORREF clrTable[] =
	{
		RGB(138, 168, 228), // xtpTabColorBlue    : Blue tab color used when OneNote colors enabled.
		RGB(255, 219, 117), // xtpTabColorYellow  : Yellow tab color used when OneNote colors enabled.
		RGB(189, 205, 159), // xtpTabColorGreen   : Green tab color used when OneNote colors enabled.
		RGB(240, 158, 159), // xtpTabColorRed     : Red tab color used when OneNote colors enabled.
		RGB(186, 166, 225), // xtpTabColorPurple  : Purple tab color used when OneNote colors enabled.
		RGB(154, 191, 180), // xtpTabColorCyan    : Cyan tab color used when OneNote colors enabled.
		RGB(247, 182, 131), // xtpTabColorOrange  : Orange tab color used when OneNote colors enabled.
		RGB(216, 171, 192)  // xtpTabColorMagenta : Magenta tab color used when OneNote colors enabled.
	};

	return clrTable[tabColor-xtpTabColorBlue];
}

void CXTPTabPaintManager::SetOneNoteColors(BOOL bOneNoteColors /* = TRUE */)
{
	m_bOneNoteColors = bOneNoteColors;
	RefreshMetrics();
}

void CXTPTabPaintManager::DrawNavigateButton(CDC* pDC, CXTPTabManagerNavigateButton* pButton, CRect& rc)
{
	FillNavigateButton(pDC, pButton, rc);

	COLORREF clr = pDC->GetTextColor();
	CXTPPenDC pen(*pDC, clr);

	if (pButton->IsEnabled())
	{
		CXTPBrushDC brush(*pDC, clr);
		pButton->DrawEntry(pDC, rc);
	}
	else
	{
		CBrush* pOldBrush = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);
		pButton->DrawEntry(pDC, rc);
		pDC->SelectObject(pOldBrush);
	}

}

void CXTPTabPaintManager::FillNavigateButton(CDC* pDC, CXTPTabManagerNavigateButton* pButton, CRect& rc)
{
	m_pColorSet->FillNavigateButton(pDC, pButton, rc);
}

void CXTPTabPaintManager::DrawTabControl(CXTPTabManager* pTabManager, CDC* pDC, CRect rcClient)
{
	m_pAppearanceSet->DrawTabControl(pTabManager, pDC, rcClient);
}

void CXTPTabPaintManager::DrawRowItems(CXTPTabManager* pTabManager, CDC* pDC, const CRect& rcClipBox, int nItemRow)
{
	CXTPTabManagerItem* pSelected = NULL;

	CXTPTabManager::ROW_ITEMS* pRowItems = pTabManager->m_pRowIndexer->GetRowItems();

	if (!pRowItems)
		return;

	if (nItemRow >= pTabManager->GetRowCount())
		return;

	int nFirstItem = pRowItems[nItemRow].nFirstItem;
	int nLastItem = pRowItems[nItemRow].nLastItem;
	BOOL bDrawRow = TRUE;

	if (m_pAppearanceSet->m_bButtonsReverseZOrder)
	{
		bDrawRow = FALSE;

		for (int i = nFirstItem; i <= nLastItem; i++)
		{
			CXTPTabManagerItem* pItem = pTabManager->GetItem(i);
			if (!pItem)
				return;

			if (pItem->m_nItemRow != nItemRow)
				break;

			if (pItem->IsSelected())
			{
				nFirstItem = i;
				bDrawRow = TRUE;
				break;
			}

			if (pItem->IsVisible() && CRect().IntersectRect(rcClipBox, m_pAppearanceSet->GetButtonDrawRect(pItem)))
			{
				m_pAppearanceSet->DrawSingleButton(pDC, pItem);
			}
		}
	}

	if (bDrawRow)
	{
		for (int i = nLastItem; i >= nFirstItem; i--)
		{
			CXTPTabManagerItem* pItem = pTabManager->GetItem(i);
			if (!pItem)
				return;

			if (pItem->m_nItemRow != nItemRow)
				break;

			if (pItem->IsVisible() && CRect().IntersectRect(rcClipBox, m_pAppearanceSet->GetButtonDrawRect(pItem)))
			{
				if (pItem->IsSelected())
					pSelected = pItem;
				else
					m_pAppearanceSet->DrawSingleButton(pDC, pItem);
			}
		}
	}


	if (pSelected)
	{
		m_pAppearanceSet->DrawSingleButton(pDC, pSelected);
	}


}

void CXTPTabPaintManager::DrawTabControlEx(CXTPTabManager* pTabManager, CDC* pDC, CRect rcClient)
{
	if (pTabManager->IsDrawStaticFrame())
	{
		pDC->Draw3dRect(rcClient, GetColorSet()->m_clrFrameBorder, GetColorSet()->m_clrFrameBorder);
		CXTPTabPaintManagerTheme::DeflateRectEx(rcClient, CRect(1, 1, 1, 1), pTabManager->GetPosition());
		pDC->Draw3dRect(rcClient, GetColorSet()->m_clrControlFace, GetColorSet()->m_clrControlFace);
		CXTPTabPaintManagerTheme::DeflateRectEx(rcClient, CRect(1, 0, 1, 1), pTabManager->GetPosition());
	}

	if (m_bFillBackground)
	{
		pDC->FillSolidRect(rcClient, GetColorSet()->m_clrControlFace);
	}

	CXTPTabPaintManagerTheme::DeflateRectEx(rcClient, m_rcControlMargin, pTabManager->GetPosition());

	pDC->SetBkMode(TRANSPARENT);


	if (m_bFillBackground)
	{
		m_pAppearanceSet->FillTabControl(pTabManager, pDC, rcClient);
	}

	CRect rcClipBox;
	pDC->GetClipBox(rcClipBox);

	if (m_bClipHeader)
		rcClipBox = m_pAppearanceSet->GetHeaderClipBox(pDC, pTabManager);

	if (!rcClipBox.IsRectEmpty())
	{
		CRgn rgn;
		if (m_bClipHeader)
		{
			CRect rcClipBoxViewport(rcClipBox);
			rcClipBoxViewport.OffsetRect(-pDC->GetWindowOrg());

			rgn.CreateRectRgnIndirect(rcClipBoxViewport);
			pDC->SelectClipRgn(&rgn);

			m_pColorSet->SelectClipRgn(pDC, rcClient, pTabManager);
		}

		int nRowCont = pTabManager->GetRowCount();

		for (int j = nRowCont - 1; j >= 0; j--)
		{
			DrawRowItems(pTabManager, pDC, rcClipBox, j);
		}

		if (m_bClipHeader)
		{
			pDC->SelectClipRgn(NULL);
		}
	}

	for (int i = pTabManager->GetNavigateButtonCount() - 1; i >= 0; i--)
		pTabManager->GetNavigateButton(i)->Draw(pDC);

}
int CXTPTabPaintManager::_GetButtonLength(int nValue, int nMin, int nMax)
{
	nValue += m_rcButtonMargin.left +  m_rcButtonMargin.right;
	if (nMin > 0) nValue = max(nMin, nValue);
	if (nMax > 0) nValue = min(nMax, nValue);
	return nValue;
}

void CXTPTabPaintManager::StripMnemonics(CString& strClear)
{
	CXTPDrawHelpers::StripMnemonics(strClear);
}

int CXTPTabPaintManager::GetPrefixTextExtent(CDC* pDC, const CString& str, UINT uFormat)
{
	if (uFormat & DT_NOPREFIX)
		return pDC->GetTextExtent(str).cx;

	CString strClear = str;
	StripMnemonics(strClear);

	return pDC->GetTextExtent(strClear).cx;
}

void CXTPTabPaintManager::DrawTextPathEllipsis(CDC* pDC, CXTPTabManager* pManager, const CString& strItem, CRect rcItem, UINT uFormat)
{
	if (strItem.Find(_T('\\')) != -1)
	{
		m_pAppearanceSet->DrawText(pDC, pManager, strItem, rcItem, uFormat | DT_PATH_ELLIPSIS);
		return;
	}

	LPCTSTR strEllipsis = _T("...");
	int cxEllipsis = pDC->GetTextExtent(strEllipsis, 3).cx;
	int cxItem = GetPrefixTextExtent(pDC, strItem, uFormat);

	if (cxItem <= rcItem.Width())
	{
		m_pAppearanceSet->DrawText(pDC, pManager, strItem, rcItem, uFormat);
		return;
	}
	if (rcItem.Width() <= cxEllipsis)
	{
		m_pAppearanceSet->DrawText(pDC, pManager, CString(strEllipsis), rcItem, uFormat);
		return;
	}
	int nWidth = rcItem.Width() - cxEllipsis;

	int nResultLength = strItem.GetLength() * nWidth / cxItem;
	CString strResultLeft = strItem.Left(nResultLength - nResultLength / 2);
	CString strResultRight = strItem.Right(nResultLength / 2);

	while (GetPrefixTextExtent(pDC, strResultLeft, uFormat) + GetPrefixTextExtent(pDC, strResultRight, uFormat) < nWidth)
	{
		nResultLength++;
		strResultLeft = strItem.Left(nResultLength - nResultLength / 2);
		strResultRight = strItem.Right(nResultLength / 2);
	}

	while (GetPrefixTextExtent(pDC, strResultLeft, uFormat) + GetPrefixTextExtent(pDC, strResultRight, uFormat) > nWidth)
	{
		nResultLength--;
		strResultLeft = strItem.Left(nResultLength - nResultLength / 2);
		strResultRight = strItem.Right(nResultLength / 2);
	}

	m_pAppearanceSet->DrawText(pDC, pManager, strResultLeft + strEllipsis + strResultRight, rcItem, uFormat);
}

void CXTPTabPaintManager::DrawFocusRect(CDC* pDC, CXTPTabManagerItem* /*pItem*/, CRect rcItem)
{
	if (!m_bDrawFocusRect)
		return;

	rcItem.DeflateRect(3, 3, 3, 3);
	pDC->SetTextColor(0);
	pDC->SetBkColor(0xFFFFFF);
	pDC->DrawFocusRect(rcItem);
}

int CXTPTabPaintManager::DrawSingleButtonIconAndText(CDC* pDC, CXTPTabManagerItem* pItem, CRect rcItem, BOOL bDraw)
{
	CXTPTabManager* pManager = pItem->GetTabManager();

	XTPTabLayoutStyle layout = pManager->GetLayout();

	BOOL bVertical = !pManager->IsHorizontalPosition();
	if (layout == xtpTabLayoutRotated) bVertical = !bVertical;

	CString strItem = pItem->GetCaption();

	CFont* pFont = m_bBoldSelected && pItem->IsSelected() && pManager->IsActive() ? GetBoldFont(bVertical) : GetFont(bVertical);
	CXTPFontDC fnt(pDC, pFont);


	CSize szIcon(m_szIcon);
	BOOL bDrawIcon = pManager->DrawIcon(pDC, 0, pItem, FALSE, szIcon);
	BOOL bDrawText = layout != xtpTabLayoutCompressed || pItem->IsSelected();

	int nNavigateButtonsLength = 0;
	if (bDrawText && pItem->GetNavigateButtons()->GetSize() > 0)
	{
		nNavigateButtonsLength = 0;
		for (int i = 0; i < (int)pItem->GetNavigateButtons()->GetSize(); i++)
		{
			CXTPTabManagerNavigateButton* pButton = pItem->GetNavigateButtons()->GetAt(i);
			if (pManager->IsNavigateButtonVisible(pButton))
			{
				CSize sz = pButton->GetSize();
				nNavigateButtonsLength += bVertical ? sz.cy : sz.cx;
			}
		}

		if (nNavigateButtonsLength > 0) nNavigateButtonsLength += 3;
	}


	if (bDraw)
	{
		if (nNavigateButtonsLength > 0)
		{
			m_pColorSet->SetTextColor(pDC, pItem);

			CRect rcEntry(rcItem);
			rcEntry.DeflateRect(m_rcButtonMargin);

			if (bVertical) rcItem.bottom -= 3; else rcItem.right -= 3;

			for (int i = (int)pItem->GetNavigateButtons()->GetSize() - 1; i >= 0; i--)
			{
				CXTPTabManagerNavigateButton* pButton = pItem->GetNavigateButtons()->GetAt(i);


				if (pManager->IsNavigateButtonVisible(pButton))
				{
					CSize szButton = pButton->GetSize();
					if (!bVertical)
					{
						pButton->SetRect(CRect(rcItem.right - szButton.cx,
							rcEntry.CenterPoint().y + szButton.cy / 2 - szButton.cy, rcItem.right,
							rcEntry.CenterPoint().y + szButton.cy / 2));
						rcItem.right -= szButton.cx;
					}
					else
					{
						pButton->SetRect(CRect(rcEntry.CenterPoint().x - szButton.cx / 2,
							rcItem.bottom - szButton.cy, rcEntry.CenterPoint().x - szButton.cx / 2 + szButton.cx, rcItem.bottom));
						rcItem.bottom -= szButton.cy;
					}
					pItem->GetNavigateButtons()->GetAt(i)->Draw(pDC);
				}
			}
		}

		m_pColorSet->SetTextColor(pDC, pItem);

		CRect rcFocus(rcItem);

		if (bVertical)
		{
			int nAvailLength = pItem->GetButtonLength() - pItem->GetContentLength();
			if (nAvailLength > 0)
			{
				rcItem.DeflateRect(0, nAvailLength / 2);
			}

			rcItem.top += m_rcButtonTextPadding.left + m_rcButtonMargin.left;
			if (pManager->GetPosition() == xtpTabPositionRight)
				rcItem.DeflateRect(m_rcButtonMargin.bottom, 0, m_rcButtonMargin.top, 0);
			else
				rcItem.DeflateRect(m_rcButtonMargin.top, 0, m_rcButtonMargin.bottom, 0);

			if (bDrawIcon)
			{
				if (m_bVerticalTextTopToBottom)
				{
					CPoint pt(rcItem.CenterPoint().x - szIcon.cx/2, rcItem.top - 1);
					if (rcItem.Height() > szIcon.cy) pManager->DrawIcon(pDC, pt, pItem, TRUE, szIcon);
					rcItem.top += szIcon.cy + 2;
				}
				else
				{
					CPoint pt(rcItem.CenterPoint().x - szIcon.cx/2, rcItem.bottom - szIcon.cy - (m_rcButtonTextPadding.left + m_rcButtonMargin.left) + 1);
					if (rcItem.Height() > szIcon.cy) pManager->DrawIcon(pDC, pt, pItem, TRUE, szIcon);
					rcItem.bottom -= szIcon.cy + m_rcButtonTextPadding.left + m_rcButtonMargin.left - 1;
				}
			}

			if (bDrawText && rcItem.bottom > rcItem.top + 8)
			{
				CSize szText = pDC->GetTextExtent(strItem);
				rcItem.left = rcItem.right - (rcItem.Width() - szText.cy - (pManager->GetPosition() == xtpTabPositionRight ? 1 : 0)) / 2;
				rcItem.right = rcItem.left + rcItem.Height();
				rcItem.bottom -= m_rcButtonMargin.right + max(0, m_rcButtonTextPadding.right - 4);

				if (!m_bVerticalTextTopToBottom)
				{
					int nHeight = min(rcItem.Height(), szText.cx);
					rcItem.OffsetRect(-szText.cy, nHeight);
				}

				if (!m_bDrawTextNoPrefix)
					StripMnemonics(strItem);

				if (m_bDrawTextPathEllipsis)
				{
					DrawTextPathEllipsis(pDC, pManager, strItem, rcItem, DT_SINGLELINE | DT_NOCLIP | DT_NOPREFIX);
				}
				else
				{
					m_pAppearanceSet->DrawText(pDC, pManager, strItem, &rcItem, DT_SINGLELINE | DT_NOCLIP |
						DT_NOPREFIX | (m_bDrawTextEndEllipsis ? DT_END_ELLIPSIS : 0));
				}
			}

		}
		else
		{
			int nAvailLength = pItem->GetButtonLength() - pItem->GetContentLength();
			if (nAvailLength > 0)
			{
				rcItem.DeflateRect(nAvailLength / 2, 0);
			}

			rcItem.left += m_rcButtonTextPadding.left + m_rcButtonMargin.left;

			if (pManager->GetPosition() == xtpTabPositionBottom)
				rcItem.DeflateRect(0, m_rcButtonMargin.bottom, 0, m_rcButtonMargin.top);
			else
				rcItem.DeflateRect(0, m_rcButtonMargin.top, 0, m_rcButtonMargin.bottom);

			if (bDrawIcon)
			{
				CPoint pt(rcItem.left - 1, rcItem.CenterPoint().y - szIcon.cy/2);
				if (rcItem.Width() > szIcon.cx) pManager->DrawIcon(pDC, pt, pItem, TRUE, szIcon);
				rcItem.left += szIcon.cx + 2;
			}

			if (bDrawText && rcItem.right > rcItem.left)
			{
				rcItem.right -= m_rcButtonTextPadding.right + m_rcButtonMargin.right - (bDrawIcon ? 2 : 0);

				if (pItem->GetMarkupUIElement())
				{
					XTPMarkupSetDefaultFont(pItem->GetTabManager()->GetMarkupContext(), (HFONT)pFont->GetSafeHandle(), pDC->GetTextColor());

					CRect rcText(rcItem);

					if (m_nDrawTextFormat & DT_VCENTER)
					{
						CSize szText = XTPMarkupMeasureElement(pItem->GetMarkupUIElement());
						rcText.top = (rcText.top + rcText.bottom - szText.cy) / 2;
						rcText.bottom = rcText.top + szText.cy;
					}

					XTPMarkupRenderElement(pItem->GetMarkupUIElement(), pDC->GetSafeHdc(), rcText);
				}
				else if (m_bDrawTextPathEllipsis)
				{
					DrawTextPathEllipsis(pDC, pManager, strItem, rcItem, DT_SINGLELINE | m_nDrawTextFormat | (m_bDrawTextNoPrefix ? DT_NOPREFIX : 0));
				}
				else
				{
					if (m_bDrawTextHidePrefix)
						StripMnemonics(strItem);

					m_pAppearanceSet->DrawText(pDC, pManager, strItem, rcItem, DT_SINGLELINE | m_nDrawTextFormat |
						(m_bDrawTextHidePrefix || m_bDrawTextNoPrefix ? DT_NOPREFIX : 0) | (m_bDrawTextEndEllipsis ? DT_END_ELLIPSIS : 0));
				}
			}
		}

		if (pItem->IsFocused())
		{
			DrawFocusRect(pDC, pItem, rcFocus);
		}
		return 0;
	}
	else
	{
		if (layout == xtpTabLayoutFixed)
		{
			return _GetButtonLength(m_nFixedTabWidth, m_nMinTabWidth, m_nMaxTabWidth);
		}

		if (layout == xtpTabLayoutCompressed)
		{
			if (!bDrawText)
				return _GetButtonLength(8 + (bVertical ? szIcon.cy : szIcon.cx), m_nMinTabWidth, m_nMaxTabWidth) ;

			CXTPFontDC fntItems(pDC, m_bBoldSelected && pManager->IsActive() ? GetBoldFont(bVertical) : GetFont(bVertical));

			int nLength = 0;

			for (int i = 0; i < pManager->GetItemCount(); i++)
			{
				CRect rcItemText(0, 0, 0, 0);
				m_pAppearanceSet->DrawText(pDC, pManager, pManager->GetItem(i)->GetCaption(), rcItemText, DT_SINGLELINE | DT_CALCRECT | DT_NOCLIP | (m_bDrawTextNoPrefix ? DT_NOPREFIX : 0));
				nLength = max(nLength, rcItemText.Width());
			}
			return _GetButtonLength(nNavigateButtonsLength + nLength + m_rcButtonTextPadding.left + m_rcButtonTextPadding.right +
				(!bDrawIcon ? 0: bVertical ? szIcon.cy : szIcon.cx), m_nMinTabWidth, m_nMaxTabWidth);
		}

		CSize szText(0);
		if (pItem->GetMarkupUIElement())
		{
			XTPMarkupSetDefaultFont(pManager->GetMarkupContext(), (HFONT)pFont->GetSafeHandle(), pDC->GetTextColor());
			szText = XTPMarkupMeasureElement(pItem->GetMarkupUIElement());
		}
		else
		{
			m_pAppearanceSet->DrawText(pDC, pManager, strItem, rcItem, DT_SINGLELINE | DT_CALCRECT | DT_NOCLIP | (m_bDrawTextNoPrefix ? DT_NOPREFIX : 0));
			szText = rcItem.Size();
		}

		return _GetButtonLength(nNavigateButtonsLength + szText.cx + m_rcButtonTextPadding.left + m_rcButtonTextPadding.right +
			(!bDrawIcon ? 0: bVertical ? szIcon.cy : szIcon.cx), m_nMinTabWidth, m_nMaxTabWidth);
	}
}


void CXTPTabPaintManager::AdjustClientRect(CXTPTabManager* pTabManager, CRect& rcClient)
{
	if (pTabManager->IsDrawStaticFrame())
		CXTPTabPaintManagerTheme::DeflateRectEx(rcClient, CRect(2, 1, 2, 2), pTabManager->GetPosition());

	if (pTabManager->GetItemCount() > 0)
	{
		m_pAppearanceSet->AdjustClientRect(pTabManager, rcClient);
	}
}

CRect CXTPTabPaintManager::RepositionNavigateButtons(CXTPTabManager* pTabManager, CRect rcClient)
{
	CRect rcHeader = m_pAppearanceSet->GetHeaderRect(rcClient, pTabManager);
	CRect rcNavigateButtons = rcHeader;

	if (pTabManager->IsHorizontalPosition())
		rcNavigateButtons.right -= 1;
	else
		rcNavigateButtons.bottom -= 1;

	CRect rc(rcNavigateButtons);

	for (int i = pTabManager->GetNavigateButtonCount() - 1; i >= 0; i--)
		pTabManager->GetNavigateButton(i)->Reposition(rcNavigateButtons);

	if (rc == rcNavigateButtons) // No Buttons added
		return rcHeader;

	return rcNavigateButtons;
}

void CXTPTabPaintManager::RepositionTabControl(CXTPTabManager* pTabManager, CDC* pDC, CRect rcClient)
{
	pTabManager->m_rcHeaderRect.SetRectEmpty();
	pTabManager->m_rcClient = pTabManager->m_rcControl = rcClient;

	pTabManager->m_pRowIndexer->CreateIndexer(1);

	for (int i = 0; i < pTabManager->GetItemCount(); i++)
	{
		pTabManager->GetItem(i)->m_nItemRow = 0;
	}

	m_pAppearanceSet->RepositionTabControl(pTabManager, pDC, rcClient);
	AdjustClientRect(pTabManager, pTabManager->m_rcClient);
}

BOOL CXTPTabPaintManager::_CreateMultiRowIndexerPlain(CXTPTabManager* pTabManager, int nWidth, int nRow, int nTotalLength)
{
	const int nRowCount = pTabManager->GetRowCount();
	const int nLengthPerRow = !m_bMultiRowJustified ? nWidth : min(nTotalLength / (nRowCount - nRow), nWidth);
	const int nItemCount = pTabManager->GetItemCount();

	int nMinItemsPerRow = 0;

	// calculate minimum number of items per row
	for (int i=1; i<pTabManager->GetRowCount(); i++)
	{
		nMinItemsPerRow = i;

		if (nItemCount - nMinItemsPerRow*(nRowCount-1) <= nMinItemsPerRow)
			break;
	}

	CXTPTabManager::ROW_ITEMS* pRowItems = pTabManager->m_pRowIndexer->GetRowItems();

	int nLastItem = 0;

	for (int r = pTabManager->GetRowCount()-1; r>=0; r--)
	{
		pRowItems[r].nFirstItem = nLastItem;

		int nSumButtonsLength = 0;

		int i = nLastItem;

		for (; i<nItemCount; i++)
		{
			CXTPTabManagerItem* pItem = pTabManager->GetItem(i);

			nSumButtonsLength += pItem->GetButtonLength();

			if (nSumButtonsLength > nLengthPerRow)
			{
				if (r != 0 && (i-nLastItem) < nMinItemsPerRow)
				{
					i++;
				}

				break;
			}
		}

		pRowItems[r].nLastItem = i - 1;

		nLastItem = i;
	}

	pRowItems[0].nLastItem = pTabManager->GetItemCount()-1;

	return TRUE;
}

int CXTPTabPaintManager::_ReduceNumberOfRowsForPlainIndexer(CXTPTabManager* pTabManager, int nRowCount, int nWidth, int nTotalLength)
{
	int nLastItem = 0;

	const int nLengthPerRow = !m_bMultiRowJustified ? nWidth : min(nTotalLength / (nRowCount - 0), nWidth);
	const int nItemCount = pTabManager->GetItemCount();

	int nMinItemsPerRow = 0;

	// calculate minimum number of items per row
	for (int i=1; i<nRowCount; i++)
	{
		nMinItemsPerRow = i;

		if (nItemCount - nMinItemsPerRow*(nRowCount-1) <= nMinItemsPerRow)
			break;
	}


	for (int r = nRowCount; r>=0; r--)
	{
		int nSumButtonsLength = 0;

		int i = nLastItem;

		if (nLastItem == nItemCount && r)
		{
			nRowCount -= r; // HERE'S the place to reduce number of rows

			break;
		}

		for (; i<nItemCount; i++)
		{
			CXTPTabManagerItem* pItem = pTabManager->GetItem(i);

			nSumButtonsLength += pItem->GetButtonLength();

			if (nSumButtonsLength > nLengthPerRow)
			{
				if (r != 0 && (i-nLastItem) < nMinItemsPerRow)
				{
					i++;
				}

				break;
			}
		}

		nLastItem = i;
	}

	return nRowCount;
}

BOOL CXTPTabPaintManager::_CreateMultiRowIndexerBestFit(CXTPTabManager* pTabManager, int nWidth, int nRow, int nTotalLength)
{
	const int nRowCount = pTabManager->GetRowCount();
	const int nLengthPerRow = !m_bMultiRowJustified ? nWidth : min(nTotalLength / (nRowCount - nRow), nWidth);

	CXTPTabManager::ROW_ITEMS* pRowItems = pTabManager->m_pRowIndexer->GetRowItems();
	int x = 0;

	for (int i = pRowItems[nRow].nLastItem; i < pTabManager->GetItemCount(); i++)
	{
		CXTPTabManagerItem* pItem = pTabManager->GetItem(i);
		int nLength = pItem->GetButtonLength();

		if (x + nLength > nLengthPerRow && x != 0 && nLength > 0)
		{
			if (nRow == nRowCount - 1)
				return FALSE;

			pRowItems[nRow + 1].nFirstItem = pRowItems[nRow + 1].nLastItem = i;

			if (_CreateMultiRowIndexerBestFit(pTabManager, nWidth, nRow + 1, nTotalLength - x))
				return TRUE;

			if (x + nLength > nWidth)
				return FALSE;
		}
		pRowItems[nRow].nLastItem = i;
		pItem->m_nItemRow = nRow;
		x += nLength;
	}

	return TRUE;
}

void CXTPTabPaintManager::CreateMultiRowIndexer(CXTPTabManager* pTabManager, CDC* pDC, int nWidth)
{
	int x = 0;
	int nRowCount = 1;
	int i;
	int nTotalLength = 0;
	int nItemCount = pTabManager->GetItemCount();
	CXTPTabManagerItem* pSelectedItem = 0;

	for (i = 0; i < nItemCount; i++)
	{
		CXTPTabManagerItem* pItem = pTabManager->GetItem(i);

		if (pItem->IsSelected())
		{
			pSelectedItem = pItem;
		}

		int nLength = pItem->m_nContentLength = pItem->m_nButtonLength =
			pItem->IsVisible() ? m_pAppearanceSet->GetButtonLength(pDC, pItem) : 0;

		if (x + nLength > nWidth && x != 0)
		{
			x = 0;
			nRowCount++;
		}
		x += nLength;
		nTotalLength += nLength;
	}

	// sometimes, when m_bMultiRowTabsOptimization is OFF, there can be additional row(s)
	if (!m_bMultiRowTabsOptimization && nRowCount > 1)
	{
		nRowCount = _ReduceNumberOfRowsForPlainIndexer(pTabManager,nRowCount, nWidth, nTotalLength);
	}

	CXTPTabManager::ROW_ITEMS* pRowItems = pTabManager->m_pRowIndexer->CreateIndexer(nRowCount);

	if (nRowCount == 1)
		return;

	pRowItems[0].nFirstItem = pRowItems[0].nLastItem = 0;

	if (m_bMultiRowTabsOptimization)
		VERIFY(_CreateMultiRowIndexerBestFit(pTabManager, nWidth, 0, nTotalLength));
	else
		VERIFY(_CreateMultiRowIndexerPlain(pTabManager, nWidth, 0, nTotalLength));

	if (!m_bMultiRowFixedSelection && pSelectedItem)
	{
		int nSelectedRow = pSelectedItem->GetItemRow();

		CXTPTabManager::ROW_ITEMS selectedRow = pRowItems[nSelectedRow];
		pRowItems[nSelectedRow] = pRowItems[0];
		pRowItems[0] = selectedRow;
	}
}

void CXTPTabPaintManager::RepositionTabControlMultiRow(CXTPTabManager* pTabManager, CDC* pDC, CRect rcClient)
{

	pTabManager->m_rcHeaderRect = RepositionNavigateButtons(pTabManager, rcClient);
	pTabManager->m_nHeaderOffset = 0;

	if (pTabManager->GetItemCount() == 0)
		return;

	CRect rcHeaderMargin = m_pAppearanceSet->GetHeaderMargin();
	int nButtonHeight = m_pAppearanceSet->GetButtonHeight(pTabManager);

	if (pTabManager->IsHorizontalPosition())
	{
		int nWidth = pTabManager->m_rcHeaderRect.Width() - (rcHeaderMargin.left + rcHeaderMargin.right);
		CreateMultiRowIndexer(pTabManager, pDC, nWidth);

		int nRowCount = pTabManager->GetRowCount();
		int nTop = 0;

		if (pTabManager->GetPosition() == xtpTabPositionBottom)
		{
			nTop = rcClient.bottom - (nButtonHeight * nRowCount + m_pAppearanceSet->m_nRowMargin * (nRowCount - 1))
				- rcHeaderMargin.top;
		}
		else
		{
			nTop = rcClient.top +  (nButtonHeight * (nRowCount - 1) + m_pAppearanceSet->m_nRowMargin * (nRowCount - 1))
				+ rcHeaderMargin.top;
		}

		int nBottom = nTop + nButtonHeight;

		int nOffset = pTabManager->GetPosition() == xtpTabPositionBottom ?
			nButtonHeight + m_pAppearanceSet->m_nRowMargin: -(nButtonHeight + m_pAppearanceSet->m_nRowMargin);


		CXTPTabManager::ROW_ITEMS* pRowItems = pTabManager->m_pRowIndexer->GetRowItems();
		int i;

		BOOL bSizeToFit = nRowCount > 1 && m_bMultiRowJustified;

		for (int nRow = 0; nRow < nRowCount; nRow++)
		{
			int nTotalLength = 0;

			int nItemInRow = pRowItems[nRow].nLastItem - pRowItems[nRow].nFirstItem + 1;
			int nFirstItem = pRowItems[nRow].nFirstItem;

			if (bSizeToFit)
			{
				for (i = 0; i < nItemInRow; i++)
				{
					nTotalLength += pTabManager->GetItem(nFirstItem + i)->GetButtonLength();
				}
			}

			int nTotalWidth = nWidth;

			int x = pTabManager->m_rcHeaderRect.left + rcHeaderMargin.left;

			for (i = 0; i < nItemInRow; i++)
			{
				CXTPTabManagerItem* pItem = pTabManager->GetItem(nFirstItem + i);
				ASSERT(pItem);
				if (!pItem)
					continue;
				int nButtonLength = pItem->m_nButtonLength;

				if (bSizeToFit)
				{
					int nLength = nButtonLength + (nTotalWidth - nTotalLength) / (nItemInRow - i);
					nTotalWidth -= nLength;
					nTotalLength -= nButtonLength;

					pItem->m_nButtonLength = nButtonLength = nLength;
				}

				pItem->SetRect(CRect(x, nTop, x + nButtonLength, nBottom));
				pItem->m_nItemRow = nRow;


				x += nButtonLength;
			}
			nTop += nOffset;
			nBottom += nOffset;
		}
	}
	else
	{
		int nWidth = pTabManager->m_rcHeaderRect.Height() - (rcHeaderMargin.left + rcHeaderMargin.right);
		CreateMultiRowIndexer(pTabManager, pDC, nWidth);


		int nRowCount = pTabManager->GetRowCount();
		int nLeft = 0;

		if (pTabManager->GetPosition() == xtpTabPositionRight)
		{
			nLeft = rcClient.right - (nButtonHeight * nRowCount + m_pAppearanceSet->m_nRowMargin * (nRowCount - 1))
				- rcHeaderMargin.top;
		}
		else
		{
			nLeft = rcClient.left +  (nButtonHeight * (nRowCount - 1) + m_pAppearanceSet->m_nRowMargin * (nRowCount - 1))
				+ rcHeaderMargin.top;
		}

		int nRight = nLeft + nButtonHeight;

		int nOffset = pTabManager->GetPosition() == xtpTabPositionRight ?
			nButtonHeight + m_pAppearanceSet->m_nRowMargin: -(nButtonHeight + m_pAppearanceSet->m_nRowMargin);


		CXTPTabManager::ROW_ITEMS* pRowItems = pTabManager->m_pRowIndexer->GetRowItems();
		int i;

		BOOL bSizeToFit = nRowCount > 1 && m_bMultiRowJustified;

		for (int nRow = 0; nRow < nRowCount; nRow++)
		{
			int nTotalLength = 0;

			int nItemInRow = pRowItems[nRow].nLastItem - pRowItems[nRow].nFirstItem + 1;
			int nFirstItem = pRowItems[nRow].nFirstItem;

			if (bSizeToFit)
			{
				for (i = 0; i < nItemInRow; i++)
				{
					nTotalLength += pTabManager->GetItem(nFirstItem + i)->GetButtonLength();
				}
			}

			int nTotalWidth = nWidth;

			int y = pTabManager->m_rcHeaderRect.top + rcHeaderMargin.left;

			for (i = 0; i < nItemInRow; i++)
			{
				CXTPTabManagerItem* pItem = pTabManager->GetItem(nFirstItem + i);
				ASSERT(pItem);
				if (!pItem)
					continue;
				int nButtonLength = pItem->m_nButtonLength;

				if (bSizeToFit)
				{
					int nLength = nButtonLength + (nTotalWidth - nTotalLength) / (nItemInRow - i);
					nTotalWidth -= nLength;
					nTotalLength -= nButtonLength;

					pItem->m_nButtonLength = nButtonLength = nLength;
				}

				pItem->SetRect(CRect(nLeft, y, nRight, y + nButtonLength));
				pItem->m_nItemRow = nRow;


				y += nButtonLength;
			}
			nLeft += nOffset;
			nRight += nOffset;
		}
	}
	pTabManager->m_rcHeaderRect = RepositionNavigateButtons(pTabManager, rcClient);
}

void CXTPTabPaintManager::RepositionTabControlEx(CXTPTabManager* pTabManager, CDC* pDC, CRect rcClient)
{
	if (pTabManager->IsDrawStaticFrame())
	{
		CXTPTabPaintManagerTheme::DeflateRectEx(rcClient, CRect(2, 1, 2, 2), pTabManager->GetPosition());
	}

	CXTPTabPaintManagerTheme::DeflateRectEx(rcClient, m_rcControlMargin, pTabManager->GetPosition());

	int i;

	if (!m_bShowTabs)
	{
		pTabManager->m_rcHeaderRect.SetRectEmpty();
		for (i = 0; i < pTabManager->GetItemCount(); i++)
		{
			CXTPTabManagerItem* pItem = pTabManager->GetItem(i);
			pItem->SetRect(CRect(0, 0, 0, 0));
		}

		for (i = pTabManager->GetNavigateButtonCount() - 1; i >= 0; i--)
			pTabManager->GetNavigateButton(i)->SetRect(CRect(0, 0, 0, 0));

		return;
	}
	if (pTabManager->GetLayout() == xtpTabLayoutMultiRow)
	{
		RepositionTabControlMultiRow(pTabManager, pDC, rcClient);
		return;
	}

	for (i = 0; i < pTabManager->GetItemCount(); i++)
	{
		CXTPTabManagerItem* pItem = pTabManager->GetItem(i);

		pItem->m_nButtonLength = pItem->m_nContentLength =
			pItem->IsVisible() ? m_pAppearanceSet->GetButtonLength(pDC, pItem) : 0;

		if (pItem->IsVisible())
			pItem->m_nButtonLength += m_nButtonExtraLength;
	}

	pTabManager->m_rcHeaderRect = m_pAppearanceSet->GetHeaderRect(rcClient, pTabManager);

	if (pTabManager->GetItemCount() == 0)
	{
		RepositionNavigateButtons(pTabManager, rcClient);
		return;
	}

	CRect rcHeaderMargin = m_pAppearanceSet->GetHeaderMargin();
	int nButtonHeight = m_pAppearanceSet->GetButtonHeight(pTabManager);


	if (pTabManager->IsHorizontalPosition())
	{
		int nTop = rcClient.top + rcHeaderMargin.top;

		if (pTabManager->GetPosition() == xtpTabPositionBottom)
		{
			nTop = rcClient.bottom - nButtonHeight - rcHeaderMargin.top;
		}

		int nBottom = nTop + nButtonHeight;


		if (pTabManager->GetLayout() == xtpTabLayoutSizeToFit)
		{
			int nWidth = rcClient.Width() - (rcHeaderMargin.right + rcHeaderMargin.left);

			for (i = pTabManager->GetNavigateButtonCount() - 1; i >= 0; i--)
				pTabManager->GetNavigateButton(i)->AdjustWidth(nWidth);

			SizeToFit(pTabManager, nWidth);
		}

		pTabManager->m_rcHeaderRect = RepositionNavigateButtons(pTabManager, rcClient);

		if (pTabManager->m_nHeaderOffset < 0)
		{
			int nLength = pTabManager->GetItemsLength();
			int nNavigateButtonsWidth = pTabManager->m_rcHeaderRect.Width() - rcHeaderMargin.left - rcHeaderMargin.right;

			if (nLength + pTabManager->m_nHeaderOffset < nNavigateButtonsWidth)
			{
				pTabManager->m_nHeaderOffset = min(0, nNavigateButtonsWidth - nLength);
				pTabManager->m_rcHeaderRect = RepositionNavigateButtons(pTabManager, rcClient);
			}
		}

		int x = pTabManager->m_rcHeaderRect.left + rcHeaderMargin.left + pTabManager->GetHeaderOffset();

		if (pTabManager->GetLayout() == xtpTabLayoutRotated)
		{
			if (pTabManager->GetPosition() == xtpTabPositionTop)
			{
				nBottom = pTabManager->m_rcHeaderRect.bottom - rcHeaderMargin.bottom;
			}
			else
			{
				nTop = pTabManager->m_rcHeaderRect.top + rcHeaderMargin.bottom;

			}

			for (i = 0; i < pTabManager->GetItemCount(); i++)
			{
				CXTPTabManagerItem* pItem = pTabManager->GetItem(i);
				if (!pItem->IsVisible()) continue;

				if (pTabManager->GetPosition() == xtpTabPositionTop)
				{
					pItem->SetRect(CRect(x, nBottom - pItem->m_nButtonLength, x + nButtonHeight, nBottom));
				}
				else
				{
					pItem->SetRect(CRect(x, nTop, x + nButtonHeight, nTop + pItem->m_nButtonLength));

				}
				x += nButtonHeight;
			}
		}
		else
		{

			for (i = 0; i < pTabManager->GetItemCount(); i++)
			{
				CXTPTabManagerItem* pItem = pTabManager->GetItem(i);

				pItem->SetRect(CRect(x, nTop, x + pItem->m_nButtonLength, nBottom));
				x += pItem->m_nButtonLength;
			}
		}

	}
	else
	{
		int nLeft = rcClient.left + rcHeaderMargin.top;

		if (pTabManager->GetPosition() == xtpTabPositionRight)
		{
			nLeft = rcClient.right - nButtonHeight - rcHeaderMargin.top;
		}

		int nRight = nLeft + nButtonHeight;

		if (pTabManager->GetLayout() == xtpTabLayoutSizeToFit)
		{
			int nWidth = rcClient.Height() - (rcHeaderMargin.right + rcHeaderMargin.left);

			for (i = pTabManager->GetNavigateButtonCount() - 1; i >= 0; i--)
				pTabManager->GetNavigateButton(i)->AdjustWidth(nWidth);

			SizeToFit(pTabManager, nWidth);
		}

		pTabManager->m_rcHeaderRect = RepositionNavigateButtons(pTabManager, rcClient);

		if (pTabManager->m_nHeaderOffset < 0)
		{
			int nLength = pTabManager->GetItemsLength();
			int nNavigateButtonsWidth = pTabManager->m_rcHeaderRect.Height() - rcHeaderMargin.left - rcHeaderMargin.right;

			if (nLength + pTabManager->m_nHeaderOffset < nNavigateButtonsWidth)
			{
				pTabManager->m_nHeaderOffset = min(0, nNavigateButtonsWidth - nLength);
				pTabManager->m_rcHeaderRect = RepositionNavigateButtons(pTabManager, rcClient);
			}
		}

		int y = pTabManager->m_rcHeaderRect.top + rcHeaderMargin.left + pTabManager->GetHeaderOffset();

		if (pTabManager->GetLayout() == xtpTabLayoutRotated)
		{
			if (pTabManager->GetPosition() == xtpTabPositionLeft)
			{
				nRight = pTabManager->m_rcHeaderRect.right - rcHeaderMargin.bottom;
			}
			else
			{
				nLeft = pTabManager->m_rcHeaderRect.left + rcHeaderMargin.bottom;

			}

			for (i = 0; i < pTabManager->GetItemCount(); i++)
			{
				CXTPTabManagerItem* pItem = pTabManager->GetItem(i);
				if (!pItem->IsVisible()) continue;

				if (pTabManager->GetPosition() == xtpTabPositionLeft)
				{
					pItem->SetRect(CRect(nRight - pItem->m_nButtonLength, y, nRight, y + nButtonHeight));
				}
				else
				{
					pItem->SetRect(CRect(nLeft, y, nLeft + pItem->m_nButtonLength, y + nButtonHeight));

				}
				y += nButtonHeight;
			}
		}
		else
		{
			for (i = 0; i < pTabManager->GetItemCount(); i++)
			{
				CXTPTabManagerItem* pItem = pTabManager->GetItem(i);

				pItem->SetRect(CRect(nLeft, y, nRight, y + pItem->m_nButtonLength));
				y += pItem->m_nButtonLength;
			}
		}
	}
}

void CXTPTabPaintManager::EnsureVisible(CXTPTabManager* pTabManager, CXTPTabManagerItem* pItem)
{
	CRect rcHeader = pTabManager->m_rcHeaderRect;
	CRect rcItem = pItem->GetRect();

	if (rcHeader.IsRectEmpty() || rcItem.IsRectEmpty())
		return;

	if (GetLayout() == xtpTabLayoutSizeToFit)
		return;

	if (pTabManager->IsHorizontalPosition())
	{
		if (rcItem.left < rcHeader.left)
		{
			pTabManager->SetHeaderOffset(pTabManager->GetHeaderOffset() - rcItem.left + rcHeader.left + m_pAppearanceSet->GetHeaderMargin().left);
		}
		else if (rcItem.right > rcHeader.right)
		{
			pTabManager->SetHeaderOffset(pTabManager->GetHeaderOffset() - rcItem.right + rcHeader.right - 2);
		}

	}
	else
	{
		if (rcItem.top < rcHeader.top)
		{
			pTabManager->SetHeaderOffset(pTabManager->GetHeaderOffset() - rcItem.top + rcHeader.top + m_pAppearanceSet->GetHeaderMargin().left);
		}
		else if (rcItem.bottom > rcHeader.bottom)
		{
			pTabManager->SetHeaderOffset(pTabManager->GetHeaderOffset() - rcItem.bottom + rcHeader.bottom - 2);
		}

	}

}

int _cdecl CXTPTabPaintManager::_SizeToFitCompare(const void *arg1, const void *arg2)
{
	int nLength1 = (*((CXTPTabManagerItem**)arg1))->GetButtonLength();
	int nLength2 = (*((CXTPTabManagerItem**)arg2))->GetButtonLength();

	return nLength1 - nLength2;
};

void CXTPTabPaintManager::SizeToFit(CXTPTabManager* pTabManager, int nWidth)
{
	int nLength = pTabManager->GetItemsLength();
	int nCount = pTabManager->GetItemCount();
	if (nCount == 0)
		return;

	if (nWidth < nLength)
	{
		if (nCount == 1)
			pTabManager->GetItem(0)->m_nButtonLength = nWidth;
		else
		{
			CXTPTabManagerItem** pItems = new CXTPTabManagerItem*[nCount];
			if (!pItems)
				return;

			for (int c = 0; c < nCount; c++)
			{
				pItems[c] = pTabManager->GetItem(c);
			}

			qsort(pItems, nCount, sizeof(CXTPTabManagerItem*), _SizeToFitCompare);

			for (int i = 0; i < nCount; i++)
			{
				int nButtonLength = pItems[i]->m_nButtonLength;

				if (nButtonLength * (nCount - i) >= nWidth)
				{
					for (int j = i; j < nCount; j++)
					{
						nButtonLength = nWidth / (nCount - j);
						pItems[j]->m_nButtonLength = nButtonLength;
						nWidth -= nButtonLength;
					}

					break;
				}

				nWidth -= nButtonLength;
			}

			delete[] pItems;
		}
	}
}

XTPTabAppearanceStyle CXTPTabPaintManager::GetAppearance() const
{
	return m_tabAppearance;
}

XTPTabColorStyle CXTPTabPaintManager::GetColor() const
{
	return m_tabColor;
}

void CXTPTabPaintManager::SetPosition(XTPTabPosition tabPosition)
{
	m_tabPosition = tabPosition;
	OnPropertyChanged();

}

void CXTPTabPaintManager::SetLayout(XTPTabLayoutStyle tabLayout)
{
	m_tabLayout = tabLayout;
	OnPropertyChanged();
}

XTPTabPosition CXTPTabPaintManager::GetPosition() const
{
	return m_tabPosition;
}

XTPTabLayoutStyle CXTPTabPaintManager::GetLayout() const
{
	return m_tabLayout;
}


void CXTPTabPaintManager::OnPropertyChanged()
{
	for (int i = 0; i < m_arrObservers.GetSize(); i++)
	{
		m_arrObservers[i]->OnPropertyChanged();
	}
}


void CXTPTabPaintManager::AddObserver(CXTPTabManagerAtom* pObserver)
{
	for (int i = 0; i < m_arrObservers.GetSize(); i++)
	{
		if (m_arrObservers[i] == pObserver)
			return;
	}
	m_arrObservers.Add(pObserver);
}

//////////////////////////////////////////////////////////////////////////

#ifdef _XTP_ACTIVEX

#define TAB_PAINTMANAGER_COLOR_SCHEME(dispid, externalName, variable) COLORREF Get##externalName(); void Set##externalName(COLORREF clr);

class COleTabPaintManagerColorSet : public CXTPCmdTarget
{
public:
	COleTabPaintManagerColorSet(CXTPTabPaintManager* pPaintManager = NULL);

	CXTPTabPaintManager* m_pPaintManager;

	#include "..\..\Workspace\ActiveX\Common\TabPaintManager.inc"

	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(COleTabPaintManagerColorSet);

};

class CXTPTabPaintManagerMargin : public CXTPCmdTarget
{
public:

	CXTPTabPaintManagerMargin(CRect* pRect = NULL, CXTPTabPaintManager* pPaintManager = NULL);  // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:
	void OnPropertyChanged();

// Implementation
protected:
	CXTPTabPaintManager* m_pPaintManager;
	CRect* m_pRect;
	long m_nTop;
	long m_nLeft;
	long m_nRight;
	long m_nBottom;

	afx_msg void OleSetRect(long nLeft, long nTop, long nRight, long nBottom);

	// Generated message map functions
	//{{AFX_MSG(CTabControlItem)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CTabControlItem)
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPTabPaintManagerMargin);
};



BEGIN_DISPATCH_MAP(CXTPTabPaintManager, CCmdTarget)
	//{{AFX_DISPATCH_MAP(CXTPTabPaintManager)
	DISP_PROPERTY_EX_ID(CXTPTabPaintManager, "Appearance", 1, GetAppearance, SetAppearance, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPTabPaintManager, "Color", 2, GetColor, SetColor, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPTabPaintManager, "Layout", 3, GetLayout, SetLayout, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPTabPaintManager, "Position", 4, GetPosition, SetPosition, VT_I4)

	DISP_PROPERTY_NOTIFY_ID(CXTPTabPaintManager, "BoldSelected", 5, m_bBoldSelected, OnPropertyChanged, VT_BOOL)
	DISP_PROPERTY_NOTIFY_ID(CXTPTabPaintManager, "HotTracking", 6, m_bHotTracking, OnPropertyChanged, VT_BOOL)
	DISP_PROPERTY_NOTIFY_ID(CXTPTabPaintManager, "ShowIcons", 7, m_bShowIcons, OnPropertyChanged, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPTabPaintManager, "DisableLunaColors", 8, IsLunaColorsDisabled, DisableLunaColors, VT_BOOL)
	DISP_PROPERTY_NOTIFY_ID(CXTPTabPaintManager, "OneNoteColors", 9, m_bOneNoteColors, OnPropertyChanged, VT_BOOL)
	DISP_FUNCTION_ID(CXTPTabPaintManager, "IsColorSupported", 10, OleIsColorSupported, VT_BOOL, VTS_I4)
	DISP_PROPERTY_NOTIFY_ID(CXTPTabPaintManager, "ClientFrame", 11, m_clientFrame, OnPropertyChanged, VT_I4)
	DISP_PROPERTY_NOTIFY_ID(CXTPTabPaintManager, "StaticFrame", 12, m_bStaticFrame, OnPropertyChanged, VT_BOOL)

	DISP_FUNCTION_ID(CXTPTabPaintManager, "ClientMargin", 13, OleGetClientMargin, VT_DISPATCH, VTS_NONE)
	DISP_FUNCTION_ID(CXTPTabPaintManager, "ControlMargin", 14, OleGetControlMargin, VT_DISPATCH, VTS_NONE)
	DISP_FUNCTION_ID(CXTPTabPaintManager, "HeaderMargin", 15, OleGetHeaderMargin, VT_DISPATCH, VTS_NONE)
	DISP_FUNCTION_ID(CXTPTabPaintManager, "ButtonMargin", 16, OleGetButtonMargin, VT_DISPATCH, VTS_NONE)
	DISP_PROPERTY_NOTIFY_ID(CXTPTabPaintManager, "DrawTextPathEllipsis", 17, m_bDrawTextPathEllipsis, OnPropertyChanged, VT_BOOL)
	DISP_PROPERTY_NOTIFY_ID(CXTPTabPaintManager, "DrawTextNoPrefix", 18, m_bDrawTextNoPrefix, OnPropertyChanged, VT_BOOL)
	DISP_FUNCTION_ID(CXTPTabPaintManager, "ColorSet", 19, OleGetColorSet, VT_DISPATCH, VTS_NONE)
	DISP_PROPERTY_EX_ID(CXTPTabPaintManager, "Font", DISPID_FONT, OleGetFont, OleSetFont, VT_DISPATCH)
	DISP_FUNCTION_ID(CXTPTabPaintManager, "SetIconSize", 20, OleSetIconSize, VT_EMPTY, VTS_I4 VTS_I4)
	DISP_PROPERTY_NOTIFY_ID(CXTPTabPaintManager, "MultiRowFixedSelection", 21, m_bMultiRowFixedSelection, OnPropertyChanged, VT_BOOL)
	DISP_PROPERTY_NOTIFY_ID(CXTPTabPaintManager, "MultiRowJustified", 22, m_bMultiRowJustified, OnPropertyChanged, VT_BOOL)
	DISP_PROPERTY_ID(CXTPTabPaintManager, "ToolTipBehaviour", 23, m_toolBehaviour, VT_I4)
	DISP_PROPERTY_ID(CXTPTabPaintManager, "SelectTabOnDragOver", 24, m_bSelectOnDragOver, VT_I4)
	DISP_PROPERTY_ID(CXTPTabPaintManager, "FillBackground", 25, m_bFillBackground, VT_BOOL)
	DISP_PROPERTY_ID(CXTPTabPaintManager, "RotateImageOnVerticalDraw", 26, m_bRotateImageOnVerticalDraw, VT_BOOL)
	DISP_PROPERTY_ID(CXTPTabPaintManager, "FixedTabWidth", 27, m_nFixedTabWidth, VT_I4)
	DISP_PROPERTY_ID(CXTPTabPaintManager, "MinTabWidth", 28, m_nMinTabWidth, VT_I4)
	DISP_PROPERTY_ID(CXTPTabPaintManager, "MaxTabWidth", 29, m_nMaxTabWidth, VT_I4)
	DISP_PROPERTY_ID(CXTPTabPaintManager, "DrawTextFormat", 30, m_nDrawTextFormat, VT_I4)
	DISP_PROPERTY_NOTIFY_ID(CXTPTabPaintManager, "ShowTabs", 31, m_bShowTabs, OnPropertyChanged, VT_BOOL)
	DISP_PROPERTY_NOTIFY_ID(CXTPTabPaintManager, "ClearTypeTextQuality", 32, m_bClearTypeTextQuality, OnClearTypeTextQualityChanged, VT_BOOL)

	DISP_PROPERTY_NOTIFY_ID(CXTPTabPaintManager, "VerticalTextTopToBottom", 33, m_bVerticalTextTopToBottom, OnClearTypeTextQualityChanged, VT_BOOL)

	DISP_PROPERTY_NOTIFY_ID(CXTPTabPaintManager, "MultiRowTabsOptimization", 34, m_bMultiRowTabsOptimization, OnPropertyChanged, VT_BOOL)

	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_ITabPaintManager to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the
//  dispinterface in the .ODL file.

// {0EC716DE-9CE6-4D40-B6C5-3B9425FBB55A}
static const IID IID_ITabPaintManager =
{ 0xec716de, 0x9ce6, 0x4d40, { 0xb6, 0xc5, 0x3b, 0x94, 0x25, 0xfb, 0xb5, 0x5a } };

BEGIN_INTERFACE_MAP(CXTPTabPaintManager, CCmdTarget)
	INTERFACE_PART(CXTPTabPaintManager, IID_ITabPaintManager, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPTabPaintManager, IID_ITabPaintManager)

/////////////////////////////////////////////////////////////////////////////
// CTabPaintManager message handlers

void CXTPTabPaintManager::OnClearTypeTextQualityChanged()
{
	if (!m_bUseStandardFont)
	{
		LOGFONT lf;
		m_fntNormal.GetLogFont(&lf);
		SetFontIndirect(&lf, FALSE);
	}

	RefreshMetrics();
}

void CXTPTabPaintManager::OleRefreshMetrics()
{
	RefreshMetrics();
}

void CXTPTabPaintManager::OleSetFont(LPFONTDISP pFontDisp)
{
	ASSERT((pFontDisp == NULL) ||
		AfxIsValidAddress(pFontDisp, sizeof(IDispatch), FALSE));

	LOGFONT lf;
	if (AxGetLogFontFromDispatch(&lf, pFontDisp))
	{
		SetFontIndirect(&lf);
	}
	else
	{
		SetFontIndirect(NULL, TRUE);
	}
	RefreshMetrics();
}

void CXTPTabPaintManager::OleSetIconSize(long cx, long cy)
{
	m_szIcon = CSize(cx, cy);
	OnPropertyChanged();
}

LPFONTDISP CXTPTabPaintManager::OleGetFont()
{
	return AxCreateOleFont(&m_fntNormal, this, (LPFNFONTCHANGED)&CXTPTabPaintManager::OleSetFont);
}

BOOL CXTPTabPaintManager::OleIsColorSupported(long nColor)
{
	return (GetAppearanceSet()->GetSupportedColorSets() & nColor) != 0;
}

LPDISPATCH CXTPTabPaintManager::OleGetClientMargin()
{
	CXTPTabPaintManagerMargin* pMargin = new CXTPTabPaintManagerMargin(&m_rcClientMargin, this);
	return pMargin->GetIDispatch(FALSE);
}

LPDISPATCH CXTPTabPaintManager::OleGetControlMargin()
{
	CXTPTabPaintManagerMargin* pMargin = new CXTPTabPaintManagerMargin(&m_rcControlMargin, this);
	return pMargin->GetIDispatch(FALSE);

}

LPDISPATCH CXTPTabPaintManager::OleGetHeaderMargin()
{
	CXTPTabPaintManagerMargin* pMargin = new CXTPTabPaintManagerMargin(&m_pAppearanceSet->m_rcHeaderMargin, this);
	return pMargin->GetIDispatch(FALSE);
}

LPDISPATCH CXTPTabPaintManager::OleGetButtonMargin()
{
	CXTPTabPaintManagerMargin* pMargin = new CXTPTabPaintManagerMargin(&m_rcButtonMargin, this);
	return pMargin->GetIDispatch(FALSE);
}

LPDISPATCH CXTPTabPaintManager::OleGetColorSet()
{
	COleTabPaintManagerColorSet* pColorSet = new COleTabPaintManagerColorSet(this);
	return pColorSet->GetIDispatch(FALSE);
}



//////////////////////////////////////////////////////////////////////////
// CXTPTabPaintManagerMargin




CXTPTabPaintManagerMargin::CXTPTabPaintManagerMargin(CRect* pRect, CXTPTabPaintManager* pPaintManager)
	: m_pRect(pRect), m_pPaintManager(pPaintManager)
{
	EnableAutomation();
	EnableTypeLib();


	ASSERT(m_pRect);
	m_nTop = m_pRect->top;
	m_nLeft = m_pRect->left;
	m_nBottom = m_pRect->bottom;
	m_nRight = m_pRect->right;
}


BEGIN_DISPATCH_MAP(CXTPTabPaintManagerMargin, CCmdTarget)
	//{{AFX_DISPATCH_MAP(CTabControlItem)
	DISP_PROPERTY_NOTIFY(CXTPTabPaintManagerMargin, "Left", m_nLeft, OnPropertyChanged, VT_I4)
	DISP_PROPERTY_NOTIFY(CXTPTabPaintManagerMargin, "Top", m_nTop, OnPropertyChanged, VT_I4)
	DISP_PROPERTY_NOTIFY(CXTPTabPaintManagerMargin, "Right", m_nRight, OnPropertyChanged, VT_I4)
	DISP_PROPERTY_NOTIFY(CXTPTabPaintManagerMargin, "Bottom", m_nBottom, OnPropertyChanged, VT_I4)
	DISP_FUNCTION(CXTPTabPaintManagerMargin, "SetRect", OleSetRect, VT_EMPTY, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// {1935891D-B4E9-45c7-AAF6-EF340E785590}
static const GUID IID_TabPaintManagerMargin =
{ 0x1935891d, 0xb4e9, 0x45c7, { 0xaa, 0xf6, 0xef, 0x34, 0xe, 0x78, 0x55, 0x90 } };

BEGIN_INTERFACE_MAP(CXTPTabPaintManagerMargin, CCmdTarget)
	INTERFACE_PART(CXTPTabPaintManagerMargin, IID_TabPaintManagerMargin, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPTabPaintManagerMargin, IID_TabPaintManagerMargin)

void CXTPTabPaintManagerMargin::OnPropertyChanged()
{
	m_pRect->SetRect(m_nLeft, m_nTop, m_nRight, m_nBottom);
	m_pPaintManager->OnPropertyChanged();
}

void CXTPTabPaintManagerMargin::OleSetRect(long nLeft, long nTop, long nRight, long nBottom)
{
	m_nLeft = nLeft;
	m_nRight = nRight;
	m_nBottom = nBottom;
	m_nTop = nTop;
	OnPropertyChanged();
}


//////////////////////////////////////////////////////////////////////////
// COleTabPaintManagerColorSet

COleTabPaintManagerColorSet::COleTabPaintManagerColorSet(CXTPTabPaintManager* pPaintManager)
: m_pPaintManager(pPaintManager)
{
	EnableAutomation();
	EnableTypeLib();
}

#undef TAB_PAINTMANAGER_COLOR_SCHEME
#define TAB_PAINTMANAGER_COLOR_SCHEME(dispid, externalName, variable)\
	DISP_PROPERTY_EX_ID(COleTabPaintManagerColorSet, #externalName, dispid, Get##externalName, Set##externalName, VT_COLOR)

BEGIN_DISPATCH_MAP(COleTabPaintManagerColorSet, CCmdTarget)
	//{{AFX_DISPATCH_MAP(CTabControlItem)
	#include "..\..\Workspace\ActiveX\Common\TabPaintManager.inc"
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// {eee5891D-B4E9-45c7-AAF6-EF340E785590}
static const GUID IID_TabPaintManagerColorSet =
{ 0xeee5891d, 0xb4e9, 0x45c7, { 0xaa, 0xf6, 0xef, 0x34, 0xe, 0x78, 0x55, 0x90 } };

BEGIN_INTERFACE_MAP(COleTabPaintManagerColorSet, CCmdTarget)
	INTERFACE_PART(COleTabPaintManagerColorSet, IID_TabPaintManagerColorSet, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(COleTabPaintManagerColorSet, IID_TabPaintManagerColorSet)

#undef TAB_PAINTMANAGER_COLOR_SCHEME
#define TAB_PAINTMANAGER_COLOR_SCHEME(dispid, externalName, variable)\
	COLORREF COleTabPaintManagerColorSet::Get##externalName(){ return m_pPaintManager->GetColorSet()->##variable;} \
	void COleTabPaintManagerColorSet::Set##externalName(COLORREF clr){ m_pPaintManager->GetColorSet()->##variable.SetCustomValue(clr == -1 ? -1 : AxTranslateColor(clr)); }

#include "..\..\Workspace\ActiveX\Common\TabPaintManager.inc"


#endif
