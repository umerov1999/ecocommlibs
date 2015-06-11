#include "StdAfx.h"

#include <GraphicLibrary/unzip/unzip.h>
#include <Common/XTPMarkupTheme.h>


#include <Common/XTPDrawHelpers.h>
#include <Common/XTPImageManager.h>
#include <Common/XTPHookManager.h>
#include <Common/XTPMarkupRender.h>
#include <Common/XTPColorManager.h>
#include <Common/XTPSystemHelpers.h>
#include <Common/XTPSystemMetrics.h>
#include <Common/ScrollBar/XTPScrollInfo.h>
#include <Common/Hook/XTPWinEventHook.h>

#include <TabManager/Includes.h>

#include <CommandBars/XTPCommandBarsDefines.h>
#include <CommandBars/XTPCommandBar.h>
#include <CommandBars/XTPToolBar.h>
#include <CommandBars/XTPMenuBar.h>
#include <CommandBars/XTPPopupBar.h>
#include <CommandBars/XTPControls.h>
#include <CommandBars/XTPControl.h>
#include <CommandBars/XTPControlButton.h>
#include <CommandBars/XTPControlPopup.h>
#include <CommandBars/XTPPaintManager.h>
#include <CommandBars/Frame/XTPFrameHook.h>
#include <CommandBars/Frame/XTPFramePaintManager.h>


#include <Ribbon/XTPRibbonGroup.h>
#include <Ribbon/XTPRibbonPopups.h>
#include <Ribbon/XTPRibbonBar.h>
#include <Ribbon/XTPRibbonTab.h>
#include <Ribbon/XTPRibbonControlTab.h>
#include <Ribbon/XTPRibbonPaintManager.h>
#include <Ribbon/XTPRibbonSystemButton.h>
#include <Ribbon/XTPRibbonMarkupTheme.h>

#include <Markup/XTPMarkupObject.h>
#include <Markup/XTPMarkupInputElement.h>
#include <Markup/XTPMarkupUIElement.h>
#include <Markup/XTPMarkupFrameworkElement.h>
#include <Markup/XTPMarkupTextBlock.h>
#include <Markup/XTPMarkupDrawingContext.h>


CXTPMarkupThemePart::CXTPMarkupThemePart()
	: m_pMarkupUIElement(NULL)
{
}

CXTPMarkupThemePart::~CXTPMarkupThemePart()
{
	if (NULL != m_pMarkupUIElement)
	{
		XTPMarkupReleaseElement(m_pMarkupUIElement);
	}
}

BOOL CXTPMarkupThemePart::LoadPart(HZIP hZip, LPCTSTR pszFileName, CXTPMarkupContext *pMarkupContext)
{
	ZRESULT zResult;
   ZIPENTRY zipEntry;
   int iIndex;

	ASSERT(NULL == m_pMarkupUIElement);

	zResult = ::FindZipItem(hZip, pszFileName, true, &iIndex, &zipEntry);
	ASSERT(ZR_OK == zResult); // File not found in archive

	if (ZR_OK == zResult)
	{
		char *pszBuffer = new char[zipEntry.unc_size+1];
		ZeroMemory(pszBuffer, sizeof(char)*(zipEntry.unc_size+1));

		zResult = ::UnzipItem(hZip, iIndex, pszBuffer, zipEntry.unc_size);
		ASSERT(ZR_OK == zResult); // Extracting failed

		CString sXaml = pszBuffer;
		m_pMarkupUIElement = XTPMarkupParseText(pMarkupContext, sXaml);

		SAFE_DELETE_AR(pszBuffer);
	}

	return (ZR_OK == zResult);
}

void CXTPMarkupThemePart::RenderMarkup(CDC *pDC, CRect rc)
{
	ASSERT(NULL != m_pMarkupUIElement);
	if (NULL != m_pMarkupUIElement)
	{
		XTPMarkupRenderElement(m_pMarkupUIElement, pDC->GetSafeHdc(), rc);
	}
}

CSize CXTPMarkupThemePart::Measure(int cx, int cy)
{
	if (NULL != m_pMarkupUIElement)
	{
		return XTPMarkupMeasureElement(m_pMarkupUIElement, cx, cy);
	}
	else
	{
		return CSize(0,0);
	}
}



void CXTPRibbonMarkupThemePartGroup::SetCaption(LPCTSTR pszCaption)
{
	CXTPMarkupTextBlock *pCaptionText = MARKUP_DYNAMICCAST(CXTPMarkupTextBlock, m_pMarkupUIElement->FindName(L"CaptionText"));

	if (NULL != pCaptionText)
	{
		pCaptionText->SetValue(CXTPMarkupTextBlock::m_pTextProperty, CXTPMarkupString::CreateValue(pszCaption));
	}
}

void CXTPRibbonMarkupThemePartGroup::SetTextColor(COLORREF clrText)
{
	CXTPMarkupTextBlock *pCaptionText = MARKUP_DYNAMICCAST(CXTPMarkupTextBlock, m_pMarkupUIElement->FindName(L"CaptionText"));

	if (NULL != pCaptionText)
	{
		pCaptionText->SetValue(CXTPMarkupTextBlock::m_pForegroundProperty, new CXTPMarkupSolidColorBrush(clrText));
	}
}

void CXTPRibbonMarkupThemePartGroup::SetFontSize(int nFontSize)
{
	CXTPMarkupTextBlock *pCaptionText = MARKUP_DYNAMICCAST(CXTPMarkupTextBlock, m_pMarkupUIElement->FindName(L"CaptionText"));

	if (NULL != pCaptionText)
	{
		pCaptionText->SetValue(CXTPMarkupTextBlock::m_pFontSizeProperty, new CXTPMarkupInt(nFontSize));
	}
}


/////////////////////////////////////////////////////////////////////////////


void CXTPRibbonMarkupThemePartSystemMenu::SetCaption(LPCTSTR pszCaption)
{
	CXTPMarkupTextBlock *pCaptionText = MARKUP_DYNAMICCAST(CXTPMarkupTextBlock, m_pMarkupUIElement->FindName(L"Caption"));

	if (NULL != pCaptionText)
	{
		pCaptionText->SetValue(CXTPMarkupTextBlock::m_pTextProperty, CXTPMarkupString::CreateValue(pszCaption));
	}
}

void CXTPRibbonMarkupThemePartSystemMenu::SetFontSize(int nFontSize)
{
	CXTPMarkupTextBlock *pCaptionText = MARKUP_DYNAMICCAST(CXTPMarkupTextBlock, m_pMarkupUIElement->FindName(L"Caption"));

	if (NULL != pCaptionText)
	{
		pCaptionText->SetValue(CXTPMarkupTextBlock::m_pFontSizeProperty, new CXTPMarkupInt(nFontSize));
	}
}
