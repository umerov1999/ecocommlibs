#include "StdAfx.h"

#include <GraphicLibrary/unzip/unzip.h>
#include <Common/XTPMarkupTheme.h>

#include <Common/XTPDrawHelpers.h>
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

#include <CommandBars/XTPCommandBarsDefines.h>
#include <CommandBars/XTPCommandBar.h>
#include <CommandBars/XTPControls.h>
#include <CommandBars/XTPControl.h>
#include <CommandBars/XTPPaintManager.h>

// Frame
#include <CommandBars/Frame/XTPFrameHook.h>
#include <CommandBars/Frame/XTPFramePaintManager.h>
#ifdef _XTP_INCLUDE_RIBBON
#include <Ribbon/XTPRibbonMarkupFrameTheme.h>
#endif
#include <CommandBars/Frame/XTPFrameThemeModernUI.h>
#include <CommandBars/Frame/XTPFrameThemeVisualStudio2012.h>

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


/////////////////////////////////////////////////////////////////////////////
// CXTPFrameThemeVisualStudio2012

CXTPFrameThemeVisualStudio2012::CXTPFrameThemeVisualStudio2012(CXTPPaintManager *pPaintManager)
	: CXTPFrameThemeModernUI(pPaintManager)
{
	m_rcIconMargin.SetRect(12,0,12,0);
}

CXTPFrameThemeVisualStudio2012::~CXTPFrameThemeVisualStudio2012()
{
}

void CXTPFrameThemeVisualStudio2012::RefreshMetrics(CXTPResourceImages *pImages)
{
	if (NULL != pImages)
	{
		m_clrFrameCaptionActive   = pImages->GetImageColor(_T("Frame.Active"),   _T("CaptionColor"));
		m_clrFrameCaptionInactive = pImages->GetImageColor(_T("Frame.Inactive"), _T("CaptionColor"));
	}
}

LPCTSTR CXTPFrameThemeVisualStudio2012::GetStyleResourceName() const
{
	return _T("XTP_IDR_VS2012_STYLE");
}

CSize CXTPFrameThemeVisualStudio2012::GetCaptionButtonSize(UINT nId) const
{
	UNUSED_ALWAYS(nId);
	return CSize(34, 26);
}

CRect CXTPFrameThemeVisualStudio2012::CalcFrameBorder(const CXTPCommandBarsFrameHook *pFrameHook) const
{
	ASSERT(NULL != pFrameHook);

	CRect rc = CXTPFrameThemeModernUI::CalcFrameBorder(pFrameHook);
	if (pFrameHook->IsMDIMaximized())
	{
		rc.top -= 4;
	}

	return rc;
}

/////////////////////////////////////////////////////////////////////////////
// CXTPFrameThemeVisualStudio2012Light

CXTPFrameThemeVisualStudio2012Light::CXTPFrameThemeVisualStudio2012Light(CXTPPaintManager *pPaintManager)
	: CXTPFrameThemeVisualStudio2012(pPaintManager)
{
	m_clrFrameCaptionActive   = RGB(239, 239, 242);
	m_clrFrameCaptionInactive = RGB(239, 239, 242);
}

CXTPFrameThemeVisualStudio2012Light::~CXTPFrameThemeVisualStudio2012Light()
{
}


/////////////////////////////////////////////////////////////////////////////
// CXTPFrameThemeVisualStudio2012Dark

CXTPFrameThemeVisualStudio2012Dark::CXTPFrameThemeVisualStudio2012Dark(CXTPPaintManager *pPaintManager)
	: CXTPFrameThemeVisualStudio2012(pPaintManager)
{
	m_clrFrameCaptionActive   = RGB(45, 45, 48);
	m_clrFrameCaptionInactive = RGB(45, 45, 48);
}

CXTPFrameThemeVisualStudio2012Dark::~CXTPFrameThemeVisualStudio2012Dark()
{
}
