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
#include <CommandBars/StatusBar/XTPStatusBarThemeOffice2013.h>
#include <CommandBars/StatusBar/Panes/XTPStatusBarPane.h>
#include <CommandBars/StatusBar/Panes/XTPStatusBarPaneSwitch.h>


CXTPStatusBarThemeOffice2013::CXTPStatusBarThemeOffice2013(CXTPPaintManager *pPaintManager)
	: CXTPStatusBarPaintManager(pPaintManager)
{
	m_clrButtonNormal      = RGB(43,  87, 154);
	m_clrButtonChecked     = RGB(25,  71, 138);
	m_clrButtonHighlighted = RGB(62, 109, 181);
	m_clrButtonPressed     = RGB(25,  71, 138);
}

void CXTPStatusBarThemeOffice2013::FillStatusBar(CDC *pDC, CXTPStatusBar *pBar)
{
	if ((NULL != pDC) && (NULL != pBar))
	{
		pDC->FillSolidRect(CXTPClientRect(pBar), m_pPaintManager->m_clrStatusBarBackground);
	}
}

void CXTPStatusBarThemeOffice2013::DrawStatusBarGripper(CDC *pDC, CRect rcClient)
{
	// No gripper
	UNUSED_ALWAYS(pDC);
	UNUSED_ALWAYS(rcClient);
}

void CXTPStatusBarThemeOffice2013::DrawStatusBarPaneBorder(CDC *pDC, CRect rc, CXTPStatusBarPane *pPane, BOOL bGripperPane)
{
	UNUSED_ALWAYS(pDC);
	UNUSED_ALWAYS(rc);
	UNUSED_ALWAYS(pPane);
	UNUSED_ALWAYS(bGripperPane);
}

void CXTPStatusBarThemeOffice2013::DrawStatusBarPaneSeparator(CDC *pDC, CRect rc, CXTPStatusBarPane *pPane)
{
	UNUSED_ALWAYS(pDC);
	UNUSED_ALWAYS(rc);
	UNUSED_ALWAYS(pPane);
}

void CXTPStatusBarThemeOffice2013::DrawStatusBarPaneEntry(CDC *pDC, CRect rcItem, CXTPStatusBarPane *pPane)
{
	CXTPStatusBarPaintManager::DrawStatusBarPaneEntry(pDC, rcItem, pPane);
}

void CXTPStatusBarThemeOffice2013::DrawStatusBarButtonFace(CDC *pDC, CRect rcItem, CXTPStatusBarPane *pPane)
{
		  if (pPane->IsPressed())     pDC->FillSolidRect(rcItem, m_clrButtonPressed);
	else if (pPane->IsHighlighted()) pDC->FillSolidRect(rcItem, m_clrButtonHighlighted);
	else if (pPane->IsChecked())     pDC->FillSolidRect(rcItem, m_clrButtonChecked);
	else                             pDC->FillSolidRect(rcItem, m_clrButtonNormal);
}

int CXTPStatusBarThemeOffice2013::DrawStatusBarSwitchPane(CDC *pDC, CRect rcItem, CXTPStatusBarSwitchPane *pPane)
{
	int nWidth = 0;

	if (NULL == pDC)
	{
		for (int i=0; i<pPane->GetSwitchCount(); i++)
		{
			CXTPStatusBarSwitchPane::SWITCH *pSwitch = pPane->GetSwitch(i);
			pSwitch->nWidth = 38;

			nWidth += pSwitch->nWidth;
		}
	}
	else
	{
		for (int i=0; i<pPane->GetSwitchCount(); i++)
		{
			CXTPStatusBarSwitchPane::SWITCH *pSwitch = pPane->GetSwitch(i);

			rcItem.right = rcItem.left + pSwitch->nWidth;
			BOOL bEnabled = pPane->IsEnabled() && pSwitch->bEnabled;

				 if (pSwitch->bPressed)     pDC->FillSolidRect(rcItem, m_clrButtonPressed);
			else if (pSwitch->bHighlighted) pDC->FillSolidRect(rcItem, m_clrButtonHighlighted);
			else if (pSwitch->bChecked)     pDC->FillSolidRect(rcItem, m_clrButtonChecked);
			else                            pDC->FillSolidRect(rcItem, m_clrButtonNormal);

			CXTPImageManagerIcon *pIcon = pPane->GetStatusBar()->GetImageManager()->GetImage(pSwitch->nID, ICON_SMALL);
			if (pIcon)
			{
				CPoint pt((rcItem.left + rcItem.right - pIcon->GetWidth()) / 2, (rcItem.top + rcItem.bottom - pIcon->GetHeight()) / 2);
				pIcon->Draw(pDC, pt, pIcon->GetIcon(!bEnabled ? xtpImageDisabled : pSwitch->bChecked ? xtpImageChecked : xtpImageNormal));
			}

			rcItem.left = rcItem.right;
		}
	}

	return nWidth;
}

COLORREF CXTPStatusBarThemeOffice2013::GetTextColor(CXTPStatusBarPane *pPane)
{
	UNUSED_ALWAYS(pPane);
	return RGB(255,255,255);
}
