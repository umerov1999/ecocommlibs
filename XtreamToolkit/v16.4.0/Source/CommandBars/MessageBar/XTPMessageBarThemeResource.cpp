#include <StdAfx.h>

#include <Common/XTPDrawHelpers.h>
#include <Common/XTPColorManager.h>
#include <Common/XTPImageManager.h>
#include <Common/XTPToolTipContext.h>
#include <Common/XTPMarkupRender.h>
#include <Common/XTPPropExchange.h>
#include <Common/XTPSystemHelpers.h>
#include <Common/XTPHookManager.h>
#include <Common/XTPVC80Helpers.h>
#include <Common/XTPWinThemeWrapper.h>
#include <Common/XTPResourceImage.h>
#include <Common/Hook/XTPWinEventHook.h>
#include <Common/ScrollBar/XTPScrollBase.h>
#include <Common/ScrollBar/XTPScrollInfo.h>
#include <Common/ScrollBar/XTPScrollBarPaintManager.h>

#include <CommandBars/XTPCommandBarsDefines.h>
#include <CommandBars/XTPPaintManager.h>
#include <CommandBars/Resource.h>
#include <CommandBars/XTPCommandBarsDefines.h>
#include <CommandBars/XTPCommandBars.h>
#include <CommandBars/XTPCommandBar.h>
#include <CommandBars/XTPPopupBar.h>
#include <CommandBars/Slider/XTPSliderPaintManager.h>
#include <CommandBars/XTPControls.h>
#include <CommandBars/XTPControl.h>
#include <CommandBars/XTPControlButton.h>

#include <CommandBars/XTPPaintManager.h>
#include <CommandBars/Themes/XTPCommandBarsOfficeXPTheme.h>
#include <CommandBars/Themes/XTPCommandBarsOffice2003Theme.h>
#include <CommandBars/Themes/XTPCommandBarsResourceTheme.h>

#include <CommandBars/MessageBar/XTPMessageBar.h>
#include <CommandBars/MessageBar/XTPMessageBarPaintManager.h>
#include <CommandBars/MessageBar/XTPMessageBarThemeResource.h>


CXTPMessageBarThemeResource::CXTPMessageBarThemeResource(CXTPResourceTheme *pTheme)
	: CXTPMessageBarPaintManager(pTheme)
	, m_pTheme(pTheme)
{
}

void CXTPMessageBarThemeResource::FillMessageBar(CDC* pDC, CXTPMessageBar* pBar)
{
	CXTPClientRect rcClient(pBar);
	pDC->FillSolidRect(rcClient, m_pTheme->m_clrMessageBarFace);

	CRect rcMessage = pBar->GetMessageRect();
	pDC->Draw3dRect(rcMessage, m_pTheme->m_clrMessageBarFrame, m_pTheme->m_clrMessageBarFrame);

	rcMessage.DeflateRect(1, 1);
	XTPDrawHelpers()->GradientFill(pDC, rcMessage, m_pTheme->m_clrMessageBar, FALSE);
}

void CXTPMessageBarThemeResource::DrawMessageBarButton(CDC* pDC, CXTPMessageBarButton* pButton)
{
	BOOL bCloseButton = (pButton->m_nID == SC_CLOSE);
	CRect rc(pButton->m_rcButton);

	if (pButton->m_bPressed && bCloseButton && XTPResourceImages()->IsValid())
	{
		pDC->FillSolidRect(rc, GetXtremeColor(XPCOLOR_HIGHLIGHT_PUSHED));
		pDC->Draw3dRect(rc, GetXtremeColor(XPCOLOR_HIGHLIGHT_PUSHED_BORDER), GetXtremeColor(XPCOLOR_HIGHLIGHT_PUSHED_BORDER));
	}
	else if (pButton->m_bHot && XTPResourceImages()->IsValid())
	{
		pDC->FillSolidRect(rc, GetXtremeColor(XPCOLOR_HIGHLIGHT));
		pDC->Draw3dRect(rc, GetXtremeColor(XPCOLOR_HIGHLIGHT_BORDER), GetXtremeColor(XPCOLOR_HIGHLIGHT_BORDER));
	}
	else if (!bCloseButton)
	{
		pDC->FillSolidRect(rc, 0xFFFFFF);
		pDC->Draw3dRect(rc, m_pTheme->m_clrMessageBarFrame, m_pTheme->m_clrMessageBarFrame);
	}

	if (bCloseButton)
	{
		if(!XTPResourceImages()->IsValid())
			return;

		CXTPResourceImage* pImage = m_pTheme->LoadImage(_T("FRAMECAPTIONCLOSE17"));

		CRect rcSrc(pImage->GetSource(0, 5));

		CRect rcGlyph(CPoint((rc.right + rc.left - rcSrc.Width()) / 2, (rc.top + rc.bottom - rcSrc.Height()) / 2), rcSrc.Size());

		pImage->DrawImage(pDC, rcGlyph, rcSrc, CRect(0, 0, 0, 0), 0xFF00FF);

	}
}
