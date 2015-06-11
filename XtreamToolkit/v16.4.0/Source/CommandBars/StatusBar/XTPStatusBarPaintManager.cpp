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
#include <CommandBars/StatusBar/Panes/XTPStatusBarPane.h>
#include <CommandBars/StatusBar/Panes/XTPStatusBarPaneSwitch.h>

CXTPStatusBarPaintManager::CXTPStatusBarPaintManager(CXTPPaintManager *pPaintManager)
	: m_pPaintManager(pPaintManager)
{
}

void CXTPStatusBarPaintManager::FillStatusBar(CDC* pDC, CXTPStatusBar* pBar)
{
	if (m_pPaintManager->m_bThemedStatusBar && m_pPaintManager->m_themeStatusBar->IsAppThemed())
	{
		m_pPaintManager->m_themeStatusBar->DrawThemeBackground(pDC->GetSafeHdc(), 0, 0, CXTPClientRect((CWnd*)pBar), 0);
	}
	else
	{
		pDC->FillSolidRect(CXTPClientRect(pBar), GetXtremeColor(COLOR_3DFACE));
	}
}

void CXTPStatusBarPaintManager::DrawStatusBarGripper(CDC* pDC, CRect rcClient)
{
	if (m_pPaintManager->m_bThemedStatusBar && m_pPaintManager->m_themeStatusBar->IsAppThemed())
	{
		CRect rcGripper(rcClient.right - 18, rcClient.top, rcClient.right, rcClient.bottom);
		m_pPaintManager->m_themeStatusBar->DrawThemeBackground(pDC->GetSafeHdc(), SP_GRIPPER, 0, &rcGripper, 0);
	}
	else
	{
		CXTPPenDC penBorder(*pDC, m_pPaintManager->m_clrStatusGripperDark);
		int i;
		for (i = 0; i < 3; i++)
		{

			pDC->MoveTo(rcClient.Width() - 3 -i * 4, rcClient.Height() - 2);
			pDC->LineTo(rcClient.Width() - 1, rcClient.Height() - 4 - i * 4);

			pDC->MoveTo(rcClient.Width() - 4 -i * 4, rcClient.Height() - 2);
			pDC->LineTo(rcClient.Width() - 1, rcClient.Height() - 4 - i * 4 - 1);
		}

		CXTPPenDC penBorderWhite(*pDC, m_pPaintManager->m_clrStatusGripperLite);
		for (i = 0; i < 3; i++)
		{
			pDC->MoveTo(rcClient.Width() - 5 -i * 4, rcClient.Height() - 2);
			pDC->LineTo(rcClient.Width() - 1 , rcClient.Height() - 4 - i * 4 - 2);
		}
	}
}

void CXTPStatusBarPaintManager::DrawStatusBarButtonFace(CDC* pDC, CRect rcItem, CXTPStatusBarPane* pPane)
{
	if (pPane->IsHighlighted() || pPane->IsPressed() || pPane->IsChecked())
	{
		m_pPaintManager->DrawRectangle(pDC, rcItem, pPane->IsHighlighted(), pPane->IsPressed(), TRUE,
			pPane->IsChecked(), FALSE, xtpBarTypeNormal, xtpBarBottom);
	}
}


void CXTPStatusBarPaintManager::DrawStatusBarPaneBorder(CDC* pDC, CRect rc, CXTPStatusBarPane* pPane, BOOL bGripperPane)
{
	if (m_pPaintManager->m_bThemedStatusBar && m_pPaintManager->m_themeStatusBar->IsAppThemed())
	{
		m_pPaintManager->m_themeStatusBar->DrawThemeBackground(pDC->GetSafeHdc(), bGripperPane ? SP_GRIPPERPANE : SP_PANE, 0, rc, 0);
	}
	else
	{
		if (!pPane->IsButton())
			m_pPaintManager->Draw3dRect(pDC, rc, COLOR_3DSHADOW, COLOR_BTNHILIGHT);
	}
}

void CXTPStatusBarPaintManager::DrawStatusBarPaneSeparator(CDC* pDC, CRect rc, CXTPStatusBarPane* /*pPane*/)
{
	CRect rcSeparator(rc.left - 2, rc.top, rc.left, rc.bottom);
	m_pPaintManager->Draw3dRect(pDC, rcSeparator, COLOR_3DSHADOW, COLOR_BTNHILIGHT);
}

COLORREF CXTPStatusBarPaintManager::GetTextColor(CXTPStatusBarPane *pPane)
{
	return pPane->IsButton() && (pPane->IsHighlighted() || pPane->IsPressed()) ?
		m_pPaintManager->GetRectangleTextColor(pPane->IsHighlighted(), pPane->IsPressed(), pPane->IsEnabled(), FALSE, FALSE, xtpBarTypeNormal, xtpBarBottom) : m_pPaintManager->m_clrStatusTextColor;
}

void CXTPStatusBarPaintManager::DrawStatusBarPaneEntry(CDC* pDC, CRect rcItem, CXTPStatusBarPane* pPane)
{
	BOOL bEnabled = pPane->IsEnabled();
	BOOL bDrawText = bEnabled || pPane->GetStatusBar()->GetDrawDisabledText();

	if (pPane->GetBackgroundColor() != (COLORREF)-1)
	{
		CRect rcBackground(rcItem);

		if (pPane->HasBorders())
			rcBackground.DeflateRect(1, 1);

		pDC->FillSolidRect(rcBackground, pPane->GetBackgroundColor());
	}
	if (pPane->IsButton())
	{
		DrawStatusBarButtonFace(pDC, rcItem, pPane);
	}

	CXTPFontDC font(pDC, pPane->GetTextFont());
	rcItem.DeflateRect(pPane->GetPadding());

	CString str = pPane->GetText();

	CXTPMarkupUIElement* pUIElement = pPane->GetMarkupUIElement();
	CXTPImageManagerIcon* pIcon = pPane->GetImage();

	COLORREF clrNormalText = GetTextColor(pPane);

	COLORREF clrText = !bEnabled ? pPane->GetTextGrayColor() != (COLORREF)-1 ? pPane->GetTextGrayColor(): m_pPaintManager->m_clrStatusTextGrayColor: pPane->GetTextColor() != (COLORREF)-1 ? pPane->GetTextColor() : clrNormalText;

	pDC->SetTextColor(clrText);
	XTPMarkupSetDefaultFont(pPane->GetStatusBar()->GetMarkupContext(), (HFONT)pPane->GetTextFont()->GetSafeHandle(), clrText);

	if (pPane->GetTextAlignment() != DT_LEFT)
	{
		int nWidth = 0;

		if (NULL != pUIElement)
		{
			nWidth += XTPMarkupMeasureElement(pUIElement).cx; // Markup
		}
		else
		{
			nWidth += pDC->GetTextExtent(str).cx; // Text
		}

		nWidth += pIcon ? pIcon->GetWidth() + (nWidth > 0 ? 2 : 0) : 0;

		if (rcItem.Width() > nWidth)
		{
			if (pPane->GetTextAlignment() == DT_RIGHT)
			{
				rcItem.left = rcItem.right - nWidth;
			}
			if (pPane->GetTextAlignment() == DT_CENTER)
			{
				rcItem.left = (rcItem.left + rcItem.right - nWidth) / 2;
			}
		}
	}

	if (pIcon)
	{
		if (rcItem.right >= rcItem.left + pIcon->GetWidth())
		{
			pIcon->Draw(pDC, CPoint(rcItem.left, (rcItem.top + rcItem.bottom - pIcon->GetHeight()) / 2), bEnabled ? pIcon->GetIcon() : pIcon->GetDisabledIcon());
		}
		rcItem.left += pIcon->GetWidth() + 2;
	}

	if (bDrawText)
	{
		if (pUIElement)
		{
			XTPMarkupMeasureElement(pUIElement, rcItem.Width(), INT_MAX);
			XTPMarkupRenderElement(pUIElement, pDC->GetSafeHdc(), rcItem);
		}
		else
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->DrawText(str, rcItem, DT_SINGLELINE | DT_VCENTER | DT_EXPANDTABS | DT_NOPREFIX);
		}
	}
}

int CXTPStatusBarPaintManager::DrawStatusBarSwitchPane(CDC* pDC, CRect rcItem, CXTPStatusBarSwitchPane* pPane)
{
	int nWidth = 0;
	int nCount = pPane->GetSwitchCount();

	if (pDC)
	{
		for (int i = 0; i < nCount; i++)
		{
			CXTPStatusBarSwitchPane::SWITCH* pSwitch = pPane->GetSwitch(i);

			rcItem.right = rcItem.left + pSwitch->nWidth;
			BOOL bEnabled = pPane->IsEnabled() && pSwitch->bEnabled;

			m_pPaintManager->DrawRectangle(pDC, rcItem, pSwitch->bHighlighted, pSwitch->bPressed, bEnabled, pSwitch->bChecked, FALSE, xtpBarTypeNormal, xtpBarTop);

			CXTPImageManagerIcon* pIcon = pPane->GetStatusBar()->GetImageManager()->GetImage(pSwitch->nID, ICON_SMALL);
			if (pIcon)
			{
				CPoint pt((rcItem.left + rcItem.right - pIcon->GetWidth()) / 2, (rcItem.top + rcItem.bottom - pIcon->GetHeight()) / 2);
				pIcon->Draw(pDC, pt, pIcon->GetIcon(!bEnabled ? xtpImageDisabled : pSwitch->bChecked ? xtpImageChecked : xtpImageNormal));
			}

			rcItem.left = rcItem.right;
		}

	}
	else
	{
		for (int i = 0; i < nCount; i++)
		{
			CXTPStatusBarSwitchPane::SWITCH *pSwitch = pPane->GetSwitch(i);
			pSwitch->nWidth = 18;

			nWidth += pSwitch->nWidth;
		}
	}

	return nWidth;
}
