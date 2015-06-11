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
#include <Common/Hook/XTPWinEventHook.h>
#include <Common/ScrollBar/XTPScrollBase.h>
#include <Common/ScrollBar/XTPScrollInfo.h>
#include <Common/ScrollBar/XTPScrollBarPaintManager.h>

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
#include <CommandBars/StatusBar/XTPStatusBar.h>
#include <CommandBars/StatusBar/XTPStatusBarPaintManager.h>
#include <CommandBars/StatusBar/XTPStatusBarThemeOfficeXP.h>

CXTPStatusBarThemeOfficeXP::CXTPStatusBarThemeOfficeXP(CXTPPaintManager *pPaintManager)
	: CXTPStatusBarPaintManager(pPaintManager)
{
}

void CXTPStatusBarThemeOfficeXP::DrawStatusBarPaneBorder(CDC* pDC, CRect rc, CXTPStatusBarPane* /*pPane*/, BOOL /*bGripperPane*/)
{
	m_pPaintManager->Draw3dRect(pDC, rc, COLOR_3DSHADOW, COLOR_3DSHADOW);
}

void CXTPStatusBarThemeOfficeXP::DrawStatusBarPaneSeparator(CDC* pDC, CRect rc, CXTPStatusBarPane* /*pPane*/)
{
	m_pPaintManager->VerticalLine(pDC, rc.left - 2, rc.top, rc.bottom, GetXtremeColor(XPCOLOR_SEPARATOR));
}
