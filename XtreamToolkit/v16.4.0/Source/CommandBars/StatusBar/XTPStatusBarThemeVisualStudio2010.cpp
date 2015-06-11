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
#include <CommandBars/StatusBar/XTPStatusBarThemeVisualStudio2010.h>

CXTPStatusBarThemeVisualStudio2010::CXTPStatusBarThemeVisualStudio2010(CXTPPaintManager *pPaintManager)
	: CXTPStatusBarPaintManager(pPaintManager)
{
}

void CXTPStatusBarThemeVisualStudio2010::FillStatusBar(CDC* pDC, CXTPStatusBar* pBar)
{
	pDC->FillSolidRect(CXTPClientRect(pBar), RGB(41, 57, 85));
}

void CXTPStatusBarThemeVisualStudio2010::DrawStatusBarGripper(CDC* pDC, CRect rcClient)
{
	if (m_pPaintManager->m_bThemedStatusBar && m_pPaintManager->m_themeStatusBar->IsAppThemed())
	{
		for (int y = 0; y <= 3; y++)
		{
			for (int x = 0; x <= 3 - y; x++)
			{
				int left = rcClient.right - x * 3 - 4;
				int top = rcClient.bottom - y * 3 - 4;

				pDC->SetPixel(left, top, RGB(248, 249, 250));
				pDC->SetPixel(left + 1, top, RGB(198, 206, 221));
				pDC->SetPixel(left, top + 1, RGB(227, 231, 238));
				pDC->SetPixel(left + 1, top + 1, RGB(170, 183, 205));
			}
		}
	}
	else
	{
		CXTPStatusBarPaintManager::DrawStatusBarGripper(pDC, rcClient);
	}
}
