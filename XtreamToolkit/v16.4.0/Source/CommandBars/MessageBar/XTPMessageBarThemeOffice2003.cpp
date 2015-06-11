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

#include <CommandBars/MessageBar/XTPMessageBar.h>
#include <CommandBars/MessageBar/XTPMessageBarPaintManager.h>
#include <CommandBars/MessageBar/XTPMessageBarThemeOffice2003.h>


CXTPMessageBarThemeOffice2003::CXTPMessageBarThemeOffice2003(CXTPCommandBarsOffice2003Theme *pTheme)
	: CXTPMessageBarPaintManager(pTheme)
	, m_pTheme(pTheme)
{
}

void CXTPMessageBarThemeOffice2003::FillMessageBar(CDC* pDC, CXTPMessageBar* pBar)
{
	CXTPClientRect rc((CWnd*)pBar);
	CRect rcFill(rc.left, rc.top, max(rc.Width(), GetSystemMetrics(SM_CXFULLSCREEN) / 2), rc.bottom);

	XTPDrawHelpers()->GradientFill(pDC, rcFill, m_pTheme->m_clrDockBar, TRUE, rc);

	CRect rcMessage = pBar->GetMessageRect();
	pDC->Draw3dRect(rcMessage, m_pTheme->m_clrToolbarShadow, m_pTheme->m_clrToolbarShadow);

	rcMessage.DeflateRect(1, 1);
	pDC->FillSolidRect(rcMessage, GetXtremeColor(COLOR_INFOBK));
}
