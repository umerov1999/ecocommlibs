#include "StdAfx.h"

#include <GraphicLibrary/unzip/unzip.h>
#include <Common/XTPMarkupTheme.h>

#include <Common/XTPDrawHelpers.h>
#include <Common/XTPResource.h>
#include <Common/XTPResourceImage.h>
#include <Common/XTPHookManager.h>
#include <Common/XTPMarkupRender.h>
#include <Common/XTPResourceManager.h>
#include <Common/XTPSystemHelpers.h>
#include <Common/XTPColorManager.h>
#include <Common/XTPWinThemeWrapper.h>
#include <Common/XTPTheme.h>
#include <Common/XTPVC80Helpers.h>
#include <Common/ScrollBar/XTPScrollInfo.h>
#include <Common/Hook/XTPWinEventHook.h>
#include <Common/XTPSystemMetrics.h>

#include <CommandBars/XTPCommandBarsDefines.h>
#include <CommandBars/XTPCommandBar.h>
#include <CommandBars/XTPControls.h>
#include <CommandBars/XTPControl.h>
#include <CommandBars/XTPPaintManager.h>
#include <CommandBars/Themes/XTPCommandBarsOfficeXPTheme.h>
#include <CommandBars/Themes/ModernUI/XTPCommandBarsModernUITheme.h>

#include <CommandBars/Frame/XTPFrameHook.h>
#include <CommandBars/Frame/XTPFramePaintManager.h>
#ifdef _XTP_INCLUDE_RIBBON
#include <Ribbon/XTPRibbonMarkupFrameTheme.h>
#endif
#include <CommandBars/Frame/XTPFrameThemeModernUI.h>

#include <Markup/XTPMarkupObject.h>
#include <Markup/XTPMarkupDrawingContext.h>
#include <Markup/XTPMarkupInputElement.h>           // CXTPMarkupUIElement
#include <Markup/XTPMarkupUIElement.h>              // CXTPMarkupFrameworkElement
#include <Markup/XTPMarkupFrameworkElement.h>   // CXTPMarkupDecorator
#include <Markup/XTPMarkupDecorator.h>              // CXTPMarkupBorder
#include <Markup/XTPMarkupBorder.h>                 // CXTPMarkupBorder
#include <Markup/XTPMarkupPanel.h>                  // CXTPMarkupCanvas
#include <Markup/XTPMarkupCanvas.h>                 // CXTPMarkupCanvas
#include <Markup/XTPMarkupShape.h>                  // CXTPMarkupShape


//---------------------------------------------------------------------------

void CXTPModernUIFrameBorder::SetBorderColor(COLORREF clrBorder)
{
	if (NULL != m_pMarkupUIElement)
	{
		CXTPMarkupBorder *pBorder = MARKUP_DYNAMICCAST(CXTPMarkupBorder, m_pMarkupUIElement->FindName(L"Border"));
		ASSERT(NULL != pBorder); // Element not found in markup

		if (NULL != pBorder)
		{
			pBorder->SetValue(CXTPMarkupBorder::m_pBorderBrushProperty, new CXTPMarkupSolidColorBrush(clrBorder));
		}
	}
}

//---------------------------------------------------------------------------

void CXTPModernUIFrameCaptionButton::SetColors(CXTPColorSetCaptionButton *pColorSet)
{
	SetBackgroundColor(pColorSet->m_clrBackground);
	SetOutlineColor(pColorSet->m_clrOutline);
	SetStrokeColor(pColorSet->m_clrFill);
	SetFillColor(pColorSet->m_clrFill);
}

void CXTPModernUIFrameCaptionButton::SetBackgroundColor(COLORREF clrBackground)
{
	CXTPMarkupBorder *pBorder = MARKUP_DYNAMICCAST(CXTPMarkupBorder, m_pMarkupUIElement->FindName(L"Border"));
	ASSERT(NULL != pBorder); // Element not found in markup

	if (NULL != pBorder)
	{
		pBorder->SetValue(CXTPMarkupBorder::m_pBackgroundProperty, new CXTPMarkupSolidColorBrush(clrBackground));
	}
}

void CXTPModernUIFrameCaptionButton::SetOutlineColor(COLORREF clrOutline)
{
	CXTPMarkupCanvas *pCanvas = MARKUP_DYNAMICCAST(CXTPMarkupCanvas, m_pMarkupUIElement->FindName(L"Canvas"));
	ASSERT(NULL != pCanvas); // Element not found in markup

	if (NULL != pCanvas)
	{
		for (int nChild=0; nChild<pCanvas->GetLogicalChildrenCount(); nChild++)
		{
			CXTPMarkupShape *pShape = MARKUP_DYNAMICCAST(CXTPMarkupShape, pCanvas->GetLogicalChild(nChild));

			if (NULL != pShape)
			{
				CXTPMarkupString *pName = pShape->GetName();

				if ((NULL != pName) && (0 == _wcsicmp(*pName, L"Outline")))
				{
					pShape->SetValue(CXTPMarkupShape::m_pStrokeProperty, new CXTPMarkupSolidColorBrush(clrOutline));
				}
			}
		}
	}
}

void CXTPModernUIFrameCaptionButton::SetStrokeColor(COLORREF clrStroke)
{
	CXTPMarkupCanvas *pCanvas = MARKUP_DYNAMICCAST(CXTPMarkupCanvas, m_pMarkupUIElement->FindName(L"Canvas"));
	ASSERT(NULL != pCanvas); // Element not found in markup

	if (NULL != pCanvas)
	{
		for (int nChild=0; nChild<pCanvas->GetLogicalChildrenCount(); nChild++)
		{
			CXTPMarkupShape *pShape = MARKUP_DYNAMICCAST(CXTPMarkupShape, pCanvas->GetLogicalChild(nChild));

			if (NULL != pShape)
			{
				CXTPMarkupString *pName = pShape->GetName();

				if ((NULL != pName) && (0 == _wcsicmp(*pName, L"Stroke")))
				{
					pShape->SetValue(CXTPMarkupShape::m_pStrokeProperty, new CXTPMarkupSolidColorBrush(clrStroke));
				}
			}
		}
	}
}

void CXTPModernUIFrameCaptionButton::SetFillColor(COLORREF clrFill)
{
	CXTPMarkupCanvas *pCanvas = MARKUP_DYNAMICCAST(CXTPMarkupCanvas, m_pMarkupUIElement->FindName(L"Canvas"));
	ASSERT(NULL != pCanvas); // Element not found in markup

	if (NULL != pCanvas)
	{
		for (int nChild=0; nChild<pCanvas->GetLogicalChildrenCount(); nChild++)
		{
			CXTPMarkupShape *pShape = MARKUP_DYNAMICCAST(CXTPMarkupShape, pCanvas->GetLogicalChild(nChild));

			if (NULL != pShape)
			{
				CXTPMarkupString *pName = pShape->GetName();

				if ((NULL != pName) && (0 == _wcsicmp(*pName, L"Fill")))
				{
					pShape->SetValue(CXTPMarkupShape::m_pStrokeProperty, new CXTPMarkupSolidColorBrush(clrFill));
					pShape->SetValue(CXTPMarkupShape::m_pFillProperty,   new CXTPMarkupSolidColorBrush(clrFill));
				}
			}
		}
	}
}

//---------------------------------------------------------------------------

CXTPFrameThemeModernUI::CXTPFrameThemeModernUI(CXTPPaintManager *pPaintManager)
	: CXTPRibbonMarkupFrameTheme(pPaintManager)
	, m_bLoaded(FALSE)
{
	// Frame border
	m_pBorderLeft   = new CXTPModernUIFrameBorder();
	m_pBorderTop    = new CXTPModernUIFrameBorder();
	m_pBorderRight  = new CXTPModernUIFrameBorder();
	m_pBorderBottom = new CXTPModernUIFrameBorder();

	// Frame caption buttons
	m_pButtonMinimize = new CXTPModernUIFrameCaptionButton();
	m_pButtonMaximize = new CXTPModernUIFrameCaptionButton();
	m_pButtonRestore  = new CXTPModernUIFrameCaptionButton();
	m_pButtonClose    = new CXTPModernUIFrameCaptionButton();
}


CXTPFrameThemeModernUI::~CXTPFrameThemeModernUI()
{
	// Frame border
	SAFE_DELETE(m_pBorderLeft);
	SAFE_DELETE(m_pBorderTop);
	SAFE_DELETE(m_pBorderRight);
	SAFE_DELETE(m_pBorderBottom);

	// Frame caption buttons
	SAFE_DELETE(m_pButtonMinimize);
	SAFE_DELETE(m_pButtonMaximize);
	SAFE_DELETE(m_pButtonRestore);
	SAFE_DELETE(m_pButtonClose);
}

void CXTPFrameThemeModernUI::LoadParts(HZIP hZip, CXTPMarkupContext *pMarkupContext)
{
	if (m_bLoaded)
		return;

	m_bLoaded = TRUE;

	// Frame border
	m_pBorderLeft  ->LoadPart(hZip, _T("Frame/BorderLeft.xaml")  , pMarkupContext);
	m_pBorderTop   ->LoadPart(hZip, _T("Frame/BorderTop.xaml")   , pMarkupContext);
	m_pBorderRight ->LoadPart(hZip, _T("Frame/BorderRight.xaml") , pMarkupContext);
	m_pBorderBottom->LoadPart(hZip, _T("Frame/BorderBottom.xaml"), pMarkupContext);

	// Frame caption buttons
	m_pButtonMinimize->LoadPart(hZip, _T("Frame/ButtonMinimize.xaml"), pMarkupContext);
	m_pButtonMaximize->LoadPart(hZip, _T("Frame/ButtonMaximize.xaml"), pMarkupContext);
	m_pButtonRestore ->LoadPart(hZip, _T("Frame/ButtonRestore.xaml") , pMarkupContext);
	m_pButtonClose   ->LoadPart(hZip, _T("Frame/ButtonClose.xaml")   , pMarkupContext);
}

HRGN CXTPFrameThemeModernUI::CalcFrameRegion(CXTPCommandBarsFrameHook *pFrameHook, CSize sz) const
{
	UNREFERENCED_PARAMETER(pFrameHook);

	HRGN hRgn = ::CreateRectRgn(0, 0, sz.cx, sz.cy);
	ASSERT(NULL != hRgn); // Region creation failed

	return hRgn;
}

void CXTPFrameThemeModernUI::DrawFrame(CDC *pDC, CXTPCommandBarsFrameHook *pFrameHook)
{
	CXTPRibbonMarkupFrameTheme::DrawFrame(pDC, pFrameHook);
}

void CXTPFrameThemeModernUI::DrawFrameCaptionButton(CDC *pDC, CRect rc, int nId, BOOL bSelected, BOOL bPressed, BOOL bActive)
{
	CXTPModernUIFrameCaptionButton *pButton = NULL;

	switch(nId)
	{
	case SC_MINIMIZE: pButton = m_pButtonMinimize; break;
	case SC_MAXIMIZE: pButton = m_pButtonMaximize; break;
	case SC_RESTORE:  pButton = m_pButtonRestore ; break;
	case SC_CLOSE:    pButton = m_pButtonClose   ; break;
	}

	if (bPressed)
	{
		pButton->SetColors(&m_csFrameCaptionButtonPressed);
	}
	else if (bSelected)
	{
		pButton->SetColors(&m_csFrameCaptionButtonHot);
	}
	else if (bActive)
	{
		pButton->SetColors(&m_csFrameCaptionButtonActive);
	}
	else if (!bActive)
	{
		pButton->SetColors(&m_csFrameCaptionButtonInactive);
	}

	pButton->RenderMarkup(pDC, rc);
}


CXTPMarkupThemePart* CXTPFrameThemeModernUI::GetFrameBorder(int nId, BOOL bActive)
{
	CXTPModernUIFrameBorder *pBorder = NULL;

	switch(nId)
	{
	case xtpBorderPartTop:
		pBorder = m_pBorderTop;
		break;
	case xtpBorderPartLeft:
	case xtpBorderPartLeftCaption:
		pBorder = m_pBorderLeft;
		break;
	case xtpBorderPartRight:
	case xtpBorderPartRightCaption:
		pBorder = m_pBorderRight;
		break;
	case xtpBorderPartBottom:
		pBorder = m_pBorderBottom;
		break;
	}

	if(NULL != pBorder)
	{
		if (bActive)
		{
			pBorder->SetBorderColor(m_clrFrameBorderActive);
		}
		else
		{
			pBorder->SetBorderColor(m_clrFrameBorderInactive);
		}
	}

	return pBorder;
}


int CXTPFrameThemeModernUI::GetFrameCaptionHeight() const
{
	return m_nFrameCaptionHeight;
}

void CXTPFrameThemeModernUI::DrawCaptionText(CDC *pDC, CRect rcCaptionText, CWnd *pSite, BOOL bActive)
{
	CXTPFramePaintManager::DrawCaptionText(pDC, rcCaptionText, pSite, bActive);
}

CRect CXTPFrameThemeModernUI::CalcFrameBorder(const CXTPCommandBarsFrameHook *pFrameHook) const
{
	DWORD dwStyle = pFrameHook->GetSiteStyle();

	if ((WS_MAXIMIZE & dwStyle) || pFrameHook->IsDwmEnabled())
	{
		return CXTPFramePaintManager::CalcFrameBorder(pFrameHook);
	}
	else
	{
		return CRect(1,1,1,1);
	}
}


void CXTPFrameThemeModernUI::RefreshMetrics()
{
	HMODULE hModule = XTPResourceImages()->GetHandle();

	if (NULL == hModule)
		hModule = XTPGetInstanceHandle();

	CXTPResource resource;
	resource.LoadResource(hModule, GetStyleResourceName(), _T("STYLE"));

	if (NULL != resource.m_hResource)
	{
		HZIP hZip = OpenZip(resource.m_pvResourceData, resource.m_dwSize, NULL);
		LoadParts(hZip, ((CXTPCommandBarsModernUITheme*)m_pPaintManager)->GetMarkupContext());
		CloseZip(hZip);
	}


	CXTPResourceImages *pImages = XTPResourceImages();

	m_nFrameCaptionHeight = pImages->GetImageInt(_T("Frame"), _T("CaptionHeight"));

	m_clrFrameBorderInactive = pImages->GetImageColor(_T("Frame.Inactive"), _T("BorderColor"));
	m_clrFrameBorderActive   = pImages->GetImageColor(_T("Frame.Active")  , _T("BorderColor"));

	m_clrFrameCaptionTextInactive       = pImages->GetImageColor(_T("Frame.Inactive"), _T("FontColor"));
	m_clrFrameCaptionTextActive     = pImages->GetImageColor(_T("Frame.Active")  , _T("FontColor"));
	m_clrFrameCaptionTextActiveTitle    = m_clrFrameCaptionTextActive;

	m_csFrameCaptionButtonInactive.RefreshMetrics(pImages, _T("Frame.CaptionButton.Inactive"));
	m_csFrameCaptionButtonActive  .RefreshMetrics(pImages, _T("Frame.CaptionButton.Active"));
	m_csFrameCaptionButtonHot     .RefreshMetrics(pImages, _T("Frame.CaptionButton.Hot"));
	m_csFrameCaptionButtonPressed .RefreshMetrics(pImages, _T("Frame.CaptionButton.Pressed"));


	CXTPPaintManager::CNonClientMetrics ncm;
	ncm.lfCaptionFont.lfWeight = FW_NORMAL;
	m_fntFrameCaption.SetStandardFont(&ncm.lfCaptionFont);
}
