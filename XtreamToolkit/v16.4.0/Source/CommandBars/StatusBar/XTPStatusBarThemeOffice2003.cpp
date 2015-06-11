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
#include <CommandBars/Themes/XTPCommandBarsOfficeXPTheme.h>
#include <CommandBars/Themes/XTPCommandBarsOffice2003Theme.h>
#include <CommandBars/StatusBar/XTPStatusBar.h>
#include <CommandBars/StatusBar/XTPStatusBarPaintManager.h>
#include <CommandBars/StatusBar/XTPStatusBarThemeOffice2003.h>


CXTPStatusBarThemeOffice2003::CXTPStatusBarThemeOffice2003(CXTPCommandBarsOffice2003Theme *pTheme)
	: CXTPStatusBarPaintManager(pTheme)
	, m_pTheme(pTheme)
{
}

void CXTPStatusBarThemeOffice2003::DrawStatusBarGripper(CDC* pDC, CRect rcClient)
{
	if (m_pTheme->m_bThemedStatusBar && m_pTheme->m_themeStatusBar->IsAppThemed())
	{
		CRect rcGripper(rcClient.right - 18, rcClient.top, rcClient.right, rcClient.bottom);
		m_pTheme->m_themeStatusBar->DrawThemeBackground(pDC->GetSafeHdc(), SP_GRIPPER, 0, &rcGripper, 0);

	}
	else
	{
		CXTPPenDC penBorder(*pDC, m_pTheme->m_clrStatusGripperDark);
		int i;
		for (i = 0; i < 3; i++)
		{
			pDC->MoveTo(rcClient.Width() - i * 5, rcClient.Height() - 1);
			pDC->LineTo(rcClient.Width(), rcClient.Height() - 1 - i * 5);

			pDC->MoveTo(rcClient.Width() - 1 - i * 5, rcClient.Height() - 1);
			pDC->LineTo(rcClient.Width(), rcClient.Height() - 2 - i * 5);

			pDC->MoveTo(rcClient.Width() - 2 - i * 5, rcClient.Height() - 1);
			pDC->LineTo(rcClient.Width(), rcClient.Height() - 3 - i * 5);
		}

		CXTPPenDC penBorderWhite(*pDC, m_pTheme->m_clrStatusGripperLite);
		for (i = 0; i < 3; i++)
		{
			pDC->MoveTo(rcClient.Width() - 3 - i * 5, rcClient.Height() - 1);
			pDC->LineTo(rcClient.Width(), rcClient.Height() - 4 - i * 5);
		}

	}
}

void CXTPStatusBarThemeOffice2003::DrawStatusBarPaneBorder(CDC* pDC, CRect rc, CXTPStatusBarPane* pPane, BOOL bGripperPane)
{
	if (m_pTheme->m_bThemedStatusBar && m_pTheme->m_themeStatusBar->IsAppThemed())
	{
		CXTPStatusBarPaintManager::DrawStatusBarPaneBorder(pDC, rc, pPane, bGripperPane);
	}
	else
	{
		pDC->Draw3dRect(rc, m_pTheme->m_clrStatusPane, m_pTheme->m_clrStatusPane);
	}
}
