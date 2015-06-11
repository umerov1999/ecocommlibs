#include <StdAfx.h>

#include <Common/XTPDrawHelpers.h>
#include <Common/XTPColorManager.h>
#include <Common/XTPImageManager.h>
#include <Common/XTPResourceImage.h>
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
#include <CommandBars/Themes/XTPCommandBarsResourceTheme.h>

#include <CommandBars/StatusBar/XTPStatusBar.h>
#include <CommandBars/StatusBar/Panes/XTPStatusBarPane.h>
#include <CommandBars/StatusBar/XTPStatusBarPaintManager.h>
#include <CommandBars/StatusBar/XTPStatusBarThemeResource.h>

CXTPStatusBarThemeResource::CXTPStatusBarThemeResource(CXTPResourceTheme *pTheme)
	: CXTPStatusBarPaintManager(pTheme)
	, m_pTheme(pTheme)
{
}

void CXTPStatusBarThemeResource::FillStatusBar(CDC* pDC, CXTPStatusBar* pBar)
{
	if(!XTPResourceImages()->IsValid())
			return;

	const CXTPClientRect rc(pBar);

	// Fill light background.
	CXTPResourceImage* pLightImage = XTPResourceImages()->LoadFile(_T("STATUSBARLIGHT"));
	ASSERT(NULL != pLightImage);
	if (NULL != pLightImage)
	{
		const CRect rcLightImage(0, 0, pLightImage->GetWidth(), pLightImage->GetHeight());
		pLightImage->DrawImage(pDC, rc, rcLightImage);
	}

	// Fill dark panes.
	CXTPResourceImage* pDarkImage = NULL;
	CRect rcDarkImage;
	const int nPaneCount = pBar->GetPaneCount();
	for(int i = 0; i < nPaneCount; ++i)
	{
		if(pBar->GetPane(i)->IsDark())
		{
			if(NULL == pDarkImage)
			{
				pDarkImage = XTPResourceImages()->LoadFile(_T("STATUSBARDARK"));
				rcDarkImage.SetRect(0, 0, pDarkImage->GetWidth(), pDarkImage->GetHeight());
			}

			ASSERT(NULL != pDarkImage);
			if (NULL != pDarkImage)
			{
				CRect rcPane;
				pBar->GetItemRect(i, &rcPane);

				if(i == (nPaneCount - 1))
				{
					rcPane.right = rc.right;
				}

				rcPane.top = rc.top;
				rcPane.bottom = rc.bottom;
				pDarkImage->DrawImage(pDC, rcPane, rcDarkImage);
			}
		}
	}
}

void CXTPStatusBarThemeResource::DrawStatusBarPaneBorder(CDC* pDC, CRect rc, CXTPStatusBarPane* /*pPane*/, BOOL bGripperPane)
{
	if (!bGripperPane)
	{
		pDC->FillSolidRect(rc.right - 1, rc.top, 1, rc.Height() - 2, m_pTheme->m_clrStatusGripperDark);
		pDC->FillSolidRect(rc.right, rc.top, 1, rc.Height() - 2, m_pTheme->m_clrStatusGripperLite);
	}
}

void CXTPStatusBarThemeResource::DrawStatusBarGripper(CDC* pDC, CRect rcClient)
{
	if(!XTPResourceImages()->IsValid())
			return;

	CXTPResourceImage* pImage = XTPResourceImages()->LoadFile(_T("STATUSBARGRIPPER"));
	ASSERT(pImage);
	if (!pImage)
		return;

	CRect rcSrc(0, 0, pImage->GetWidth(), pImage->GetHeight());
	CRect rcGripper(rcClient.right - rcSrc.Width() , rcClient.bottom - rcSrc.Height() - 4, rcClient.right, rcClient.bottom - 4);
	pImage->DrawImage(pDC, rcGripper, rcSrc, CRect(0, 0, 0, 0), RGB(0xFF, 0, 0xFF));
}
