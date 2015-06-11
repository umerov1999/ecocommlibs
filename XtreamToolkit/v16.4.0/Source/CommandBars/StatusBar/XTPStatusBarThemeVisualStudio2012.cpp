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
#include <CommandBars/StatusBar/XTPStatusBarThemeVisualStudio2012.h>

CXTPStatusBarThemeVisualStudio2012::CXTPStatusBarThemeVisualStudio2012(CXTPPaintManager *pPaintManager)
	: CXTPStatusBarPaintManager(pPaintManager)
{
}

void CXTPStatusBarThemeVisualStudio2012::FillStatusBar(CDC *pDC, CXTPStatusBar *pBar)
{
	if ((NULL != pDC) && (NULL != pBar))
	{
		pDC->FillSolidRect(CXTPClientRect(pBar), m_pPaintManager->m_clrStatusBarBackground);
	}
}

void CXTPStatusBarThemeVisualStudio2012::DrawStatusBarPaneBorder(CDC *pDC, CRect rc, CXTPStatusBarPane *pPane, BOOL bGripperPane)
{
	UNUSED_ALWAYS(pDC);
	UNUSED_ALWAYS(rc);
	UNUSED_ALWAYS(pPane);
	UNUSED_ALWAYS(bGripperPane);
}

void CXTPStatusBarThemeVisualStudio2012::DrawStatusBarGripper(CDC* pDC, CRect rcClient)
{
	CPoint pt(rcClient.BottomRight());

	int a, b, c, x, y;

	// draw light gripper part.
	for (a = 5, c = 12; a < 12; a += 2)
	{
		x = pt.x - a;
		for (b = 5; b < c; b += 2)
		{
			y = pt.y - b;
			pDC->SetPixel(x, y, m_pPaintManager->m_clrStatusGripperLite);
		}
		c -= 2;
	}

	// draw dark gripper part.
	for (a = 6, c = 13; a < 13; a += 2)
	{
		x = pt.x - a;
		for (b = 6; b < c; b += 2)
		{
			y = pt.y - b;
			pDC->SetPixel(x, y, m_pPaintManager->m_clrStatusGripperDark);
		}
		c -= 2;
	}
}
