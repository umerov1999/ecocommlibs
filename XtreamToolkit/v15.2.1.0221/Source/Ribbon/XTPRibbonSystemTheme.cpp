// XTPRibbonTheme.cpp: implementation of the CXTPRibbonPaintManager class.
//
// This file is a part of the XTREME RIBBON MFC class library.
// (c)1998-2012 Codejock Software, All Rights Reserved.
//
// THIS SOURCE FILE IS THE PROPERTY OF CODEJOCK SOFTWARE AND IS NOT TO BE
// RE-DISTRIBUTED BY ANY MEANS WHATSOEVER WITHOUT THE EXPRESSED WRITTEN
// CONSENT OF CODEJOCK SOFTWARE.
//
// THIS SOURCE CODE CAN ONLY BE USED UNDER THE TERMS AND CONDITIONS OUTLINED
// IN THE XTREME TOOLKIT PRO LICENSE AGREEMENT. CODEJOCK SOFTWARE GRANTS TO
// YOU (ONE SOFTWARE DEVELOPER) THE LIMITED RIGHT TO USE THIS SOFTWARE ON A
// SINGLE COMPUTER.
//
// CONTACT INFORMATION:
// support@codejock.com
// http://www.codejock.com
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "Common/XTPImageManager.h"
#include "Common/XTPHookManager.h"
#include "Common/XTPMarkupRender.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPDrawHelpers.h"

#include "TabManager/XTPTabManager.h"
#include "TabManager/XTPTabPaintManager.h"

#include "CommandBars/XTPCommandBarsDefines.h"
#include "CommandBars/XTPCommandBar.h"
#include "CommandBars/XTPToolBar.h"
#include "CommandBars/XTPMenuBar.h"
#include "CommandBars/XTPPopupBar.h"
#include "CommandBars/XTPControls.h"
#include "CommandBars/XTPControl.h"
#include "CommandBars/XTPControlButton.h"
#include "CommandBars/XTPControlPopup.h"

#include "CommandBars/XTPPaintManager.h"
#include "CommandBars/XTPCommandBarsFrameHook.h"


#include "XTPRibbonGroup.h"
#include "XTPRibbonPopups.h"
#include "XTPRibbonBar.h"
#include "XTPRibbonTab.h"
#include "XTPRibbonControlTab.h"
#include "XTPRibbonPaintManager.h"
#include "XTPRibbonSystemTheme.h"
#include "XTPRibbonSystemButton.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CXTPRibbonSystemTheme::CXTPRibbonSystemTheme(CXTPPaintManager* pPaintManager)
	: CXTPRibbonPaintManager(pPaintManager)
{
	m_pMarkupContext = XTPMarkupCreateContext(0);
}


CXTPRibbonSystemTheme::~CXTPRibbonSystemTheme()
{
	XTPMarkupReleaseContext(m_pMarkupContext);
}

void CXTPRibbonSystemTheme::RenderMarkup(CDC* pDC, CRect rc, LPCTSTR lpszMarkupText)
{
	CXTPMarkupUIElement* pMarkupUIElement = XTPMarkupParseText(m_pMarkupContext, lpszMarkupText);

	XTPMarkupRenderElement(pMarkupUIElement, pDC->GetSafeHdc(), rc);

	XTPMarkupReleaseElement(pMarkupUIElement);

}

void CXTPRibbonSystemTheme::RefreshMetrics()
{
	CXTPRibbonPaintManager::RefreshMetrics();

	m_clrRibbonFace = GetSysColor(COLOR_3DFACE);
	m_clrMinimizedFrameEdgeShadow = GetSysColor(COLOR_3DSHADOW);
	m_clrMinimizedFrameEdgeHighLight = GetSysColor(COLOR_3DHIGHLIGHT);
	m_clrRibbonGroupCaptionText = GetSysColor(COLOR_BTNTEXT);

	m_pTabPaintManager->GetColorSet()->m_clrHighlightText.SetStandardValue(m_clrRibbonGroupCaptionText);
	m_pTabPaintManager->GetColorSet()->m_clrNormalText.SetStandardValue(m_clrRibbonGroupCaptionText);
	m_pTabPaintManager->GetColorSet()->m_clrSelectedText.SetStandardValue(m_clrRibbonGroupCaptionText);

	m_clrRecentFileListEdgeShadow = GetSysColor(COLOR_3DSHADOW);
	m_clrRecentFileListEdgeHighLight = GetSysColor(COLOR_3DHIGHLIGHT);

	m_nGroupSpacing = 2;
	m_bFlatFrame = FALSE;
}


void CXTPRibbonSystemTheme::DrawRibbonTab(CDC* pDC, CXTPRibbonTab* pItem)
{
	XTPTabPosition tabPosition = pItem->GetTabManager()->GetPosition();

	CRect rcEntry(pItem->GetRect());
	CXTPTabPaintManagerAppearanceSet::InflateRectEx(rcEntry, CRect(0, 1, -1,  -1), tabPosition);

	XTPRibbonTabContextColor tabColor = pItem->GetContextColor();

	if (tabColor != xtpRibbonTabContextColorNone)
	{
		CXTPRibbonPaintManager::DrawRibbonTab(pDC, pItem);
		return;
	}


	CXTPRibbonBar* pRibbonBar = pItem->GetRibbonBar();

	BOOL bSelected = pItem->IsSelected();
	BOOL bHighlighted = pItem->IsHighlighted();
	BOOL bFocused = pItem->IsFocused();

	if (bSelected && pRibbonBar->IsRibbonMinimized())
	{
		CXTPRibbonControlTab* pControlTab =  (CXTPRibbonControlTab*)pItem->GetTabManager();
		if (pControlTab->GetCommandBar() && pControlTab->GetCommandBar()->IsTrackingMode())
		{
			bFocused = bHighlighted = FALSE;
		}
		else if (pControlTab->GetSelected() && bFocused)
		{
			bFocused = bSelected = FALSE;
			bHighlighted = TRUE;
		}
		else
		{
			bFocused = bSelected = FALSE;
		}
	}



	if (bSelected || bHighlighted)
	{

		LPCTSTR lpszMarkupText = NULL;
		if (bSelected && bFocused)
		{
			lpszMarkupText =
				_T("<Grid Margin = '0, 0, 0, 0' ClipToBounds='True'>")
				_T("<Border Margin = '2, 2, 2, -2' BorderThickness='1' CornerRadius = '3' BorderBrush='{x:Static SystemColors.HighlightBrush}'><Border.Background>")
				_T("<LinearGradientBrush StartPoint='0,0' EndPoint='0,1'>")
				_T("    <GradientStop Offset='0.2' Color='{x:Static SystemColors.ControlLightLightColor}'/>")
				_T("    <GradientStop Offset='1.3' Color='{x:Static SystemColors.HighlightColor}'/>")
				_T("</LinearGradientBrush></Border.Background></Border>")

				_T("<Border Margin='3, 3, 3, -2' CornerRadius = '2' BorderThickness='1' BorderBrush='{x:Static SystemColors.ControlLightLightBrush}'/>")

				_T("</Grid>");

		}
		else if (bSelected && bHighlighted)
		{
			lpszMarkupText =
				_T("<Grid Margin = '0, 0, 0, 0' ClipToBounds='True'>")
				_T("<Border Margin = '2, 2, 2, -2' BorderThickness='1' CornerRadius = '3' BorderBrush='{x:Static SystemColors.ControlDarkBrush}'><Border.Background>")
				_T("<LinearGradientBrush StartPoint='0,0' EndPoint='0,1'>")
				_T("    <GradientStop Offset='0.2' Color='{x:Static SystemColors.ControlLightLightColor}'/>")
				_T("    <GradientStop Offset='1.3' Color='{x:Static SystemColors.ControlColor}'/>")
				_T("</LinearGradientBrush></Border.Background></Border>")

				_T("<Border Margin='3, 3, 3, -2' CornerRadius = '2' BorderThickness='1' BorderBrush='{x:Static SystemColors.ControlLightLightBrush}'/>")

				_T("<Border Margin = '1, 1, 1, -3' BorderThickness='3' CornerRadius = '3'><Border.BorderBrush>")
				_T("<LinearGradientBrush StartPoint='0,0' EndPoint='0,1'>")
				_T("    <GradientStop Offset='-0.2' Color='Transparent'/>")
				_T("    <GradientStop Offset='2' Color='{x:Static SystemColors.HighlightColor}'/>")
				_T("</LinearGradientBrush></Border.BorderBrush></Border>")

				_T("</Grid>");

		}
		else if (bSelected)
		{
			lpszMarkupText =
			_T("<Grid Margin = '0, 0, 0, 0' ClipToBounds='True'>")
			_T("<Border Margin = '2, 2, 2, -2' BorderThickness='1' CornerRadius = '3' BorderBrush='{x:Static SystemColors.ControlDarkBrush}'><Border.Background>")
			_T("<LinearGradientBrush StartPoint='0,0' EndPoint='0,1'>")
			_T("    <GradientStop Offset='0.2' Color='{x:Static SystemColors.ControlLightLightColor}'/>")
			_T("    <GradientStop Offset='1.3' Color='{x:Static SystemColors.ControlColor}'/>")
			_T("</LinearGradientBrush></Border.Background></Border>")

			_T("<Border Margin='3, 3, 3, -2' CornerRadius = '2' BorderThickness='1' BorderBrush='{x:Static SystemColors.ControlLightLightBrush}'/>")
			_T("</Grid>");
		}
		else if (bHighlighted)
		{
			lpszMarkupText =
				_T("<Grid Margin = '0, 0, 0, 0' ClipToBounds='True'>")
				_T("<Border Margin = '2, 2, 2, -2' BorderThickness='1' CornerRadius = '3' BorderBrush='{x:Static SystemColors.ControlDarkBrush}'><Border.Background>")
				_T("<LinearGradientBrush StartPoint='0,0' EndPoint='0,1'>")
				_T("    <GradientStop Offset='0.2' Color='Transparent'/>")
				_T("    <GradientStop Offset='1.3' Color='{x:Static SystemColors.ControlDarkColor}'/>")
				_T("</LinearGradientBrush></Border.Background></Border>")

			_T("<Border Margin='3, 3, 3, -2' CornerRadius = '2' BorderThickness='1' BorderBrush='#A0FFFFFF'/>")
			_T("</Grid>");

		}


		RenderMarkup(pDC, rcEntry, lpszMarkupText);
	}
}

void CXTPRibbonSystemTheme::FillRibbonBar(CDC* pDC, CXTPRibbonBar* pRibbonBar)
{

	CXTPClientRect rcRibbonClient(pRibbonBar);
	if (pRibbonBar->IsFrameThemeEnabled())
		rcRibbonClient.top += m_nRibbonCaptionHeight;

	CRect rcRibbonTabs(rcRibbonClient.left, rcRibbonClient.top, rcRibbonClient.right, rcRibbonClient.top + m_nTabsHeight);
	CRect rcRibbonGroups(rcRibbonClient.left, rcRibbonTabs.bottom, rcRibbonClient.right, rcRibbonClient.bottom);

	pDC->FillSolidRect(rcRibbonTabs, m_clrRibbonFace);
	pDC->FillSolidRect(rcRibbonGroups, m_clrRibbonFace);

	if (pRibbonBar->IsQuickAccessBelowRibbon())
	{
		CRect rcQuickAccess = pRibbonBar->GetQuickAccessRect();
		pDC->Draw3dRect(rcQuickAccess, GetSysColor(COLOR_3DSHADOW), GetSysColor(COLOR_3DSHADOW));
	}

	if (!pRibbonBar->IsGroupsVisible() && !pRibbonBar->IsQuickAccessBelowRibbon())
	{
		m_pPaintManger->HorizontalLine(pDC, rcRibbonClient.left, rcRibbonClient.bottom - 2, rcRibbonClient.right, m_clrMinimizedFrameEdgeShadow);
		m_pPaintManger->HorizontalLine(pDC, rcRibbonClient.left, rcRibbonClient.bottom - 1, rcRibbonClient.right, m_clrMinimizedFrameEdgeHighLight);
	}
}

void CXTPRibbonSystemTheme::FillGroupRect(CDC* pDC, CXTPRibbonTab* /*pActiveTab*/, CRect rcGroups)
{
	LPCTSTR lpszMarkupText =

	_T("<Grid>")
		_T("<Border><Border.Background>")
		_T("<LinearGradientBrush StartPoint='0,0' EndPoint='0,1'>")
		_T("    <GradientStop Offset='0' Color='#00FFFFFF'/>")
		_T("    <GradientStop Offset='1.5' Color='{x:Static SystemColors.ControlDarkColor}'/>")
		_T("</LinearGradientBrush></Border.Background></Border>")


		_T("<Border Margin='1, 0, 1, 2' CornerRadius = '3' BorderThickness='1'><Border.Background>")
		_T("<LinearGradientBrush StartPoint='0,0' EndPoint='0,1'>")
		_T("    <GradientStop Offset='0' Color='{x:Static SystemColors.ControlLightLightColor}'/>")
		_T("    <GradientStop Offset='0.2' Color='{x:Static SystemColors.ControlColor}'/>")
		_T("</LinearGradientBrush></Border.Background><Border.BorderBrush>")
		_T("<LinearGradientBrush StartPoint='0,0' EndPoint='0,1'>")
		_T("    <GradientStop Offset='0' Color='{x:Static SystemColors.ControlDarkColor}'/>")
		_T("    <GradientStop Offset='0.6' Color='{x:Static SystemColors.ControlDarkColor}'/>")
		_T("    <GradientStop Offset='1.2' Color='{x:Static SystemColors.ControlDarkDarkColor}'/>")
		_T("</LinearGradientBrush></Border.BorderBrush></Border>")
		_T("<Border Margin='2, 1, 2, 3' CornerRadius = '3' BorderThickness='1, 1, 1, 1'> <Border.BorderBrush>")
		_T("<LinearGradientBrush StartPoint='0,0' EndPoint='0,1'>")
		_T("    <GradientStop Offset='0' Color='#00FFFFFF'/>")
		_T("    <GradientStop Offset='0.2' Color='#00FFFFFF'/>")
		_T("    <GradientStop Offset='1' Color='#6FFFFFFF'/>")
		_T("</LinearGradientBrush></Border.BorderBrush></Border>")
		_T("</Grid>");

	RenderMarkup(pDC, rcGroups, lpszMarkupText);
}

void CXTPRibbonSystemTheme::FillGroupPopupToolBar(CDC* pDC, CXTPRibbonGroup* /*pGroup*/, CXTPCommandBar* pCommandBar)
{
	pDC->SetBkMode(TRANSPARENT);

	CXTPClientRect rcRibbon(pCommandBar);

	pDC->FillSolidRect(rcRibbon, m_clrRibbonFace);
}


void CXTPRibbonSystemTheme::DrawGroup(CDC* pDC, CXTPRibbonGroup* pGroup)
{
	CRect rcGroup(pGroup->GetRect());

	CRect rcGroupClient(rcGroup.left, rcGroup.top, rcGroup.right, rcGroup.bottom - m_nGroupCaptionHeight);
	CRect rcGroupCaption(rcGroup.left, rcGroupClient.bottom, rcGroup.right, rcGroup.bottom);


	if (!pGroup->IsReduced())
	{

		if (pGroup->IsHighlighted())
		{
			LPCTSTR lpszMarkupText =

				_T("<Grid>")
				_T("<Border Margin='1, 1, 0, 0' CornerRadius = '2' BorderThickness='1' BorderBrush='{x:Static SystemColors.ControlLightLightBrush}'/>")
				_T("<Border Margin='1, 1, 2, 2' CornerRadius = '2' BorderThickness='1' BorderBrush='{x:Static SystemColors.ControlLightLightBrush}'/>")
				_T("<Border Margin='0, 0, 1, 1' Background='#5FFFFFFF' CornerRadius = '2' BorderThickness='1' BorderBrush='{x:Static SystemColors.ControlDarkBrush}'/>")
				_T("</Grid>");

			RenderMarkup(pDC, rcGroup, lpszMarkupText);

			lpszMarkupText =

				_T("<Grid>")
				_T("<Border Margin='1, 0, 2, 2'><Border.Background>")
				_T("<LinearGradientBrush StartPoint='0,-0.4' EndPoint='0,1'>")
				_T("    <GradientStop Offset='0' Color='{x:Static SystemColors.ControlColor}'/>")
				_T("    <GradientStop Offset='1' Color='{x:Static SystemColors.ControlDarkColor}'/>")
				_T("</LinearGradientBrush></Border.Background></Border>")
				_T("</Grid>");

			RenderMarkup(pDC, rcGroupCaption, lpszMarkupText);

			lpszMarkupText =

				_T("<Grid>")
				_T("<Border Margin='1, 1, 2, 2' CornerRadius = '2' BorderThickness='1' BorderBrush='{x:Static SystemColors.ControlLightLightBrush}'/>")
				_T("</Grid>");

			RenderMarkup(pDC, rcGroup, lpszMarkupText);

		}
		else
		{
			LPCTSTR lpszMarkupText =

				_T("<Grid>")
				_T("<Border Margin='1, 1, 0, 0' CornerRadius = '2' BorderThickness='1' BorderBrush='{x:Static SystemColors.ControlLightLightBrush}'/>")
				_T("<Border Margin='0, 0, 1, 1' CornerRadius = '2' BorderThickness='1' BorderBrush='{x:Static SystemColors.ControlDarkBrush}'/>")
				_T("</Grid>");

				RenderMarkup(pDC, rcGroup, lpszMarkupText);

			lpszMarkupText =

				_T("<Grid>")
				_T("<Border Margin='1, 0, 2, 2'><Border.Background>")
				_T("<LinearGradientBrush StartPoint='0,-0.2' EndPoint='0,1.2'>")
				_T("    <GradientStop Offset='0' Color='{x:Static SystemColors.ControlColor}'/>")
				_T("    <GradientStop Offset='1' Color='{x:Static SystemColors.ControlDarkColor}'/>")
				_T("</LinearGradientBrush></Border.Background></Border>")
				_T("</Grid>");

			RenderMarkup(pDC, rcGroupCaption, lpszMarkupText);
		}


		CString strCaption = pGroup->GetCaption();
		XTPDrawHelpers()->StripMnemonics(strCaption);

		CXTPFontDC font(pDC, &m_fontGroupCaption);

		CRect rcGroupCaptionText(rcGroupCaption);
		rcGroupCaptionText.OffsetRect(0, -1);
		if (pGroup->IsOptionButtonVisible())
			rcGroupCaptionText.right -= m_nGroupCaptionHeight + 1;
		rcGroupCaptionText.DeflateRect(2, 2);

		pDC->SetTextColor(m_clrRibbonGroupCaptionText);
		pDC->DrawText(strCaption, rcGroupCaptionText, DT_SINGLELINE | DT_VCENTER | DT_NOPREFIX | DT_END_ELLIPSIS | DT_CENTER);
	}
}

void CXTPRibbonSystemTheme::DrawRibbonFrameCaption(CDC* pDC, CXTPRibbonBar* pRibbonBar, BOOL bActive)
{
	if (pRibbonBar->IsDwmEnabled())
	{
		CXTPRibbonPaintManager::DrawRibbonFrameCaption(pDC, pRibbonBar, bActive);
		return;
	}

	CFrameWnd* pSite = (CFrameWnd*)pRibbonBar->GetSite();
	CRect rcCaption = pRibbonBar->GetCaptionRect();

	if (!pRibbonBar->IsDwmEnabled())
	{

		LPCTSTR lpszMarkupText = bActive ?
			_T("<Border BorderThickness='0,0,0,1' BorderBrush='{x:Static SystemColors.ActiveCaptionBrush}'><Border.Background>")
			_T("<LinearGradientBrush StartPoint='0,0' EndPoint='0,1'>")
			_T("    <GradientStop Offset='0' Color='{x:Static SystemColors.GradientActiveCaptionColor}'/>")
			_T("    <GradientStop Offset='0.2' Color='{x:Static SystemColors.ActiveCaptionColor}'/>")
			_T("    <GradientStop Offset='1.5' Color='{x:Static SystemColors.GradientActiveCaptionColor}'/>")
			_T("</LinearGradientBrush></Border.Background></Border>") :

		_T("<Border BorderThickness='0,0,0,1' BorderBrush='{x:Static SystemColors.InactiveCaptionBrush}'><Border.Background>")
			_T("<LinearGradientBrush StartPoint='0,0' EndPoint='0,1'>")
			_T("    <GradientStop Offset='0' Color='{x:Static SystemColors.GradientInactiveCaptionColor}'/>")
			_T("    <GradientStop Offset='0.2' Color='{x:Static SystemColors.InactiveCaptionColor}'/>")
			_T("    <GradientStop Offset='1.5' Color='{x:Static SystemColors.GradientInactiveCaptionColor}'/>")
			_T("</LinearGradientBrush></Border.Background></Border>");

		RenderMarkup(pDC, rcCaption, lpszMarkupText);
	}
	else
	{
		pDC->FillSolidRect(rcCaption, 0);
	}

	//////////////////////////////////////////////////////////////////////////

	if (!pRibbonBar->IsQuickAccessBelowRibbon() && pRibbonBar->IsQuickAccessVisible() &&
		((CXTPControls*)pRibbonBar->GetQuickAccessControls())->GetVisibleCount() > 0)
	{
		CRect rcQuickAccess = pRibbonBar->GetQuickAccessRect();
		CRect rcQuickAccessArea(rcQuickAccess.left - 15, rcQuickAccess.top + 2, rcQuickAccess.right, rcQuickAccess.top + 2 + pRibbonBar->GetQuickAccessHeight());

		CString strMarkupText;

		rcQuickAccessArea.DeflateRect(1, 2, 3, 2);

		CSize sz = rcQuickAccessArea.Size();

		strMarkupText.Format(
			_T("<Path Data='M0 0 S %i %i %i %i L %i %i S %i %i %i %i l 0 0 S  %i %i %i %i Z' Stroke='#22FFFFFF' StrokeThickness='2' Fill='#44FFFFFF' ></Path>")
			, sz.cy /2, 0, sz.cy / 2, sz.cy,
			sz.cx - sz.cy / 2, sz.cy, sz.cx, sz.cy, sz.cx, sz.cy / 2, sz.cx, 0, sz.cx - sz.cy /2, 0);

		RenderMarkup(pDC, rcQuickAccessArea, strMarkupText);
	}


	//////////////////////////////////////////////////////////////////////////


	if (pRibbonBar->GetSystemButton() == NULL)
	{
		HICON hIcon = m_pPaintManger->GetFramePaintManager()->GetFrameSmallIcon(pSite);
		if (hIcon)
		{
			CSize szIcon(GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON));

			int nTop = (rcCaption.Height()  - szIcon.cy)/2;
			int nLeft = rcCaption.left + 10;

			DrawIconEx(pDC->GetSafeHdc(), nLeft, nTop, hIcon,
				szIcon.cx, szIcon.cy, 0, NULL, DI_NORMAL);
		}
	}


	CRect rcCaptionText = pRibbonBar->GetCaptionTextRect();

	CXTPRibbonTabContextHeaders* pContextHeaders = pRibbonBar->GetContextHeaders();
	if (pContextHeaders->GetCount() != 0)
	{
		DrawRibbonFrameContextHeaders(pDC, pRibbonBar, pContextHeaders);
	}

	CXTPFontDC font(pDC, &m_pPaintManger->GetFramePaintManager()->m_fontFrameCaption);

	CString strWindowText;
	pSite->GetWindowText(strWindowText);
	rcCaptionText.left += max(0, (rcCaptionText.Width() - pDC->GetTextExtent(strWindowText).cx) / 2);

	if (pSite->GetStyle() & WS_MAXIMIZE)
	{
		rcCaptionText.top += pRibbonBar->GetFrameHook()->GetFrameBorder() / 2;
	}

	if (!pRibbonBar->IsDwmEnabled())
	{
		m_pPaintManger->GetFramePaintManager()->DrawCaptionText(pDC, rcCaptionText, pSite, bActive);
	}
	else
	{
		DrawDwmCaptionText(pDC, rcCaptionText, strWindowText, pSite, bActive);
	}
}

void CXTPRibbonSystemTheme::DrawRibbonFrameCaptionBar(CDC* pDC, CXTPRibbonBar* pRibbonBar)
{
	CRect rcCaption = pRibbonBar->GetCaptionRect();
	rcCaption.top--;


	LPCTSTR lpszMarkupText =
		_T("<Border BorderThickness='0,0,0,1' BorderBrush='{x:Static SystemColors.ActiveCaptionBrush}'><Border.Background>")
		_T("<LinearGradientBrush StartPoint='0,0' EndPoint='0,1'>")
		_T("    <GradientStop Offset='0' Color='{x:Static SystemColors.GradientActiveCaptionColor}'/>")
		_T("    <GradientStop Offset='0.2' Color='{x:Static SystemColors.ActiveCaptionColor}'/>")
		_T("    <GradientStop Offset='1.5' Color='{x:Static SystemColors.GradientActiveCaptionColor}'/>")
		_T("</LinearGradientBrush></Border.Background></Border>");

	RenderMarkup(pDC, rcCaption, lpszMarkupText);


	if (!pRibbonBar->IsQuickAccessBelowRibbon() && pRibbonBar->IsQuickAccessVisible() &&
		((CXTPControls*)pRibbonBar->GetQuickAccessControls())->GetVisibleCount() > 0)
	{
		CRect rcQuickAccess = pRibbonBar->GetQuickAccessRect();
		int nFrameBorder = 0;
		CRect rcQuickAccessArea(rcQuickAccess.left - 15, nFrameBorder, rcQuickAccess.right, pRibbonBar->GetQuickAccessHeight() + nFrameBorder);

		CString strMarkupText;

		rcQuickAccessArea.DeflateRect(1, 2, 3, 2);

		CSize sz = rcQuickAccessArea.Size();

		strMarkupText.Format(
			_T("<Path Data='M0 0 S %i %i %i %i L %i %i S %i %i %i %i l 0 0 S  %i %i %i %i Z' Stroke='#22FFFFFF' StrokeThickness='2' Fill='#44FFFFFF' ></Path>")
			, sz.cy /2, 0, sz.cy / 2, sz.cy,
			sz.cx - sz.cy / 2, sz.cy, sz.cx, sz.cy, sz.cx, sz.cy / 2, sz.cx, 0, sz.cx - sz.cy /2, 0);

		RenderMarkup(pDC, rcQuickAccessArea, strMarkupText);

	}

	CXTPRibbonTabContextHeaders* pContextHeaders = pRibbonBar->GetContextHeaders();
	if (pContextHeaders->GetCount() != 0)
	{
		DrawRibbonFrameContextHeaders(pDC, pRibbonBar, pContextHeaders);
	}
}


CSize CXTPRibbonSystemTheme::DrawControlGroupOption(CDC* pDC, CXTPControl* pControlGroupOption, BOOL bDraw)
{
	if (bDraw)
	{
		CRect rc = pControlGroupOption->GetRect();

		m_pPaintManger->DrawRectangle(pDC, rc, pControlGroupOption->GetSelected(), pControlGroupOption->GetPressed(), TRUE, FALSE, FALSE, xtpBarTypeNormal, xtpBarTop);

		rc = CRect(CPoint((rc.left + rc.right - 6) / 2, (rc.bottom + rc.top - 8) / 2), CSize(6, 8));

		CXTPPenDC pen(*pDC, 0);
		CXTPBrushDC brush(*pDC, 0);
		pDC->MoveTo(rc.left, rc.top + 5);
		pDC->LineTo(rc.left, rc.top);
		pDC->LineTo(rc.left + 6, rc.top);
		pDC->MoveTo(rc.left + 3, rc.top + 3);
		pDC->LineTo(rc.left + 7, rc.top + 7);

		XTPDrawHelpers()->Triangle(pDC, CPoint(rc.left + 3, rc.top + 6), CPoint(rc.left + 6, rc.top + 3), CPoint(rc.left + 6, rc.top + 6));

		pen.Color(0xFFFFFF);
		pDC->MoveTo(rc.left + 1, rc.top + 5);
		pDC->LineTo(rc.left + 1, rc.top + 1);
		pDC->LineTo(rc.left + 6, rc.top + 1);

		pDC->MoveTo(rc.left + 4, rc.top + 7);
		pDC->LineTo(rc.left + 7, rc.top + 7);
		pDC->LineTo(rc.left + 7, rc.top + 2);
		pDC->SetPixel(rc.left + 4, rc.top + 3, 0xFFFFFF);
	}

	return CSize(15, 14);
}

CSize CXTPRibbonSystemTheme::DrawControlGroupPopup(CDC* pDC, CXTPControlPopup* pControlGroupPopup, BOOL bDraw)
{
	CXTPRibbonGroup* pRibbonGroup = pControlGroupPopup->GetRibbonGroup();


	if (!pRibbonGroup)
	{
		if (bDraw)
		{
			m_pPaintManger->DrawControlEntry(pDC, pControlGroupPopup);
		}
		return m_pPaintManger->DrawControlToolBarParent(pDC, pControlGroupPopup, bDraw);
	}

	if (bDraw)
	{
		CRect rc = pControlGroupPopup->GetRect();

		if (pControlGroupPopup->GetPopuped())
		{
			LPCTSTR lpszMarkupText =

				_T("<Grid>")
				_T("<Border Margin='1, 1, 2, 2' CornerRadius = '2' BorderThickness='1' BorderBrush='{x:Static SystemColors.ControlLightLightBrush}'/>")
				_T("<Border Margin='0, 0, 1, 1' Background='#5FFFFFFF' CornerRadius = '2' BorderThickness='1' BorderBrush='{x:Static SystemColors.ControlDarkBrush}'><Border.Background>")
				_T("<LinearGradientBrush StartPoint='0,0' EndPoint='0,1'>")
				_T("    <GradientStop Offset='-0.2' Color='{x:Static SystemColors.ControlDarkColor}'/>")
				_T("    <GradientStop Offset='2' Color='Transparent'/>")
				_T("</LinearGradientBrush></Border.Background></Border>")
				_T("</Grid>");

			RenderMarkup(pDC, rc, lpszMarkupText);
		}
		else if (pControlGroupPopup->GetSelected())
		{
			LPCTSTR lpszMarkupText =

				_T("<Grid>")
				_T("<Border Margin='1, 1, 2, 2' CornerRadius = '2' BorderThickness='1' BorderBrush='{x:Static SystemColors.ControlLightLightBrush}'/>")
				_T("<Border Margin='0, 0, 1, 1' Background='#5FFFFFFF' CornerRadius = '2' BorderThickness='1' BorderBrush='{x:Static SystemColors.ControlDarkBrush}'/>")
				_T("</Grid>");

			RenderMarkup(pDC, rc, lpszMarkupText);

		}
		else
		{
			LPCTSTR lpszMarkupText =
				_T("<Grid>")
				_T("<Border Margin='1, 1, 2, 2' CornerRadius = '2' BorderThickness='1' BorderBrush='{x:Static SystemColors.ControlLightLightBrush}'/>")
				_T("<Border Margin='0, 0, 1, 1' CornerRadius = '2' BorderThickness='1' BorderBrush='{x:Static SystemColors.ControlDarkBrush}'/>")
				_T("</Grid>");
			RenderMarkup(pDC, rc, lpszMarkupText);
		}
	}

	CSize szButton(max(42, m_pPaintManger->m_nTextHeight * 2 - 3), GetEditHeight() * 3);

	CXTPFontDC font(pDC, m_pPaintManger->GetControlFont(pControlGroupPopup));

	int nSplitDropDownHeight = m_pPaintManger->GetSplitDropDownHeight();

	if (bDraw)
	{
		CXTPImageManagerIcon* pImageIcon = pControlGroupPopup->GetImage(0);
		BOOL bDrawImage = (pImageIcon != NULL);

		CRect rcButton = pControlGroupPopup->GetRect();
		if (bDrawImage)
		{
			CSize sz(pImageIcon->GetWidth(), pImageIcon->GetHeight());

			CPoint pt = CPoint(rcButton.CenterPoint().x - sz.cx / 2, rcButton.top + 4);

			if (sz.cx < 20)
			{

				pt.y = rcButton.top + 9;
			}

			m_pPaintManger->DrawImage(pDC, pt, sz, pImageIcon,
				pControlGroupPopup->GetSelected(), pControlGroupPopup->GetPressed(),
				pControlGroupPopup->GetEnabled(), FALSE, pControlGroupPopup->GetPopuped());
		}

		CRect rcText = rcButton;
		rcText.bottom -= m_nGroupCaptionHeight + 2;
		rcText.top = rcText.bottom - nSplitDropDownHeight;

		pDC->SetTextColor(m_pPaintManger->GetControlTextColor(pControlGroupPopup));
		m_pPaintManger->DrawControlText2(pDC, pControlGroupPopup, rcText, TRUE, FALSE, TRUE);
	}
	else
	{
		CSize szText = m_pPaintManger->DrawControlText2(pDC, pControlGroupPopup, CXTPEmptyRect(), FALSE, FALSE, TRUE);

		szButton.cx = max(szButton.cx, szText.cx + 5);
	}
	return szButton;
}

void CXTPRibbonSystemTheme::DrawRibbonQuickAccessButton(CDC* pDC, CXTPControlPopup* pControl)
{

	m_pPaintManger->DrawRectangle(pDC, pControl->GetRect(), pControl->GetSelected(), pControl->GetPressed(), pControl->GetEnabled(), FALSE,
		pControl->GetPopuped(), xtpBarTypeNormal, xtpBarTop);


	{
		COLORREF clr = GetSysColor(COLOR_3DHIGHLIGHT);

		CXTPPenDC pen(*pDC, clr);
		CXTPBrushDC brush(*pDC, clr);

		CPoint pt = pControl->GetRect().CenterPoint();
		pt.y++;

		pDC->MoveTo(pt.x - 2, pt.y - 3);
		pDC->LineTo(pt.x + 3, pt.y - 3);

		XTPDrawHelpers()->Triangle(pDC, CPoint(pt.x - 2, pt.y), CPoint(pt.x + 2, pt.y), CPoint(pt.x, pt.y + 2));
	}

	{
		COLORREF clr = GetSysColor(COLOR_BTNTEXT);

		CXTPPenDC pen(*pDC, clr);
		CXTPBrushDC brush(*pDC, clr);

		CPoint pt = pControl->GetRect().CenterPoint();

		pDC->MoveTo(pt.x - 2, pt.y - 3);
		pDC->LineTo(pt.x + 3, pt.y - 3);

		XTPDrawHelpers()->Triangle(pDC, CPoint(pt.x - 2, pt.y), CPoint(pt.x + 2, pt.y), CPoint(pt.x, pt.y + 2));
	}

}

void CXTPRibbonSystemTheme::DrawRibbonQuickAccessMoreButton(CDC* pDC, CXTPControlPopup* pControl)
{
	m_pPaintManger->DrawRectangle(pDC, pControl->GetRect(), pControl->GetSelected(), pControl->GetPressed(), pControl->GetEnabled(), FALSE,
		pControl->GetPopuped(), xtpBarTypeNormal, xtpBarTop);

	{
		COLORREF clr = GetSysColor(COLOR_3DHIGHLIGHT);

		CXTPPenDC pen(*pDC, clr);
		CXTPBrushDC brush(*pDC, clr);

		CPoint pt = pControl->GetRect().CenterPoint();

		XTPDrawHelpers()->Triangle(pDC, CPoint(pt.x - 3, pt.y - 2), CPoint(pt.x - 1, pt.y), CPoint(pt.x - 3, pt.y + 2));
		XTPDrawHelpers()->Triangle(pDC, CPoint(pt.x + 1, pt.y - 2), CPoint(pt.x + 3, pt.y), CPoint(pt.x + 1, pt.y + 2));
	}

	{
		COLORREF clr = GetSysColor(COLOR_BTNTEXT);

		CXTPPenDC pen(*pDC, clr);
		CXTPBrushDC brush(*pDC, clr);

		CPoint pt = pControl->GetRect().CenterPoint();


		XTPDrawHelpers()->Triangle(pDC, CPoint(pt.x - 3, pt.y - 1), CPoint(pt.x - 2, pt.y), CPoint(pt.x - 3, pt.y + 1));
		XTPDrawHelpers()->Triangle(pDC, CPoint(pt.x + 1, pt.y - 1), CPoint(pt.x + 2, pt.y), CPoint(pt.x + 1, pt.y + 1));
	}
}

void CXTPRibbonSystemTheme::FillMorePopupToolBarEntry(CDC* pDC, CXTPCommandBar* pBar)
{
	CRect rc;
	pBar->GetClientRect(&rc);

	pDC->FillSolidRect(rc, m_clrRibbonFace);
	pDC->Draw3dRect(rc, GetXtremeColor(COLOR_3DSHADOW), GetXtremeColor(COLOR_3DSHADOW));
}


void CXTPRibbonSystemTheme::FillSystemPopupBarEntry(CDC* pDC, CXTPPopupBar* pBar)
{
	CXTPClientRect rc(pBar);


	LPCTSTR lpszMarkupText =

		_T("<Grid>")
		_T("<Border BorderThickness='1' BorderBrush='{x:Static SystemColors.ControlDarkBrush}' Background='{x:Static SystemColors.WindowBrush}'/>")
		_T("<Border Margin='1' BorderThickness='5, 17, 5, 28' BorderBrush='{x:Static SystemColors.ControlBrush}'/>")

		_T("<Border Margin='1, 1, 1, 0' Height = '10' VerticalAlignment='Top'><Border.Background>")
		_T("<LinearGradientBrush StartPoint='0,0' EndPoint='0,1'>")
		_T("    <GradientStop Offset='0' Color='{x:Static SystemColors.ControlLightLightColor}'/>")
		_T("    <GradientStop Offset='1' Color='{x:Static SystemColors.ControlColor}'/>")
		_T("</LinearGradientBrush></Border.Background></Border>")

		_T("<Border Margin='5, 17, 5, 28' BorderThickness='1' BorderBrush='{x:Static SystemColors.ControlDarkBrush}' />")
		_T("<Border Margin='4, 16, 4, 27' BorderThickness='1' BorderBrush='{x:Static SystemColors.ControlLightLightBrush}' />")

		_T("<Border Margin='1, 0, 1, 1' Height = '26' VerticalAlignment='Bottom'><Border.Background>")
		_T("<LinearGradientBrush StartPoint='0,0' EndPoint='0,1'>")
		_T("    <GradientStop Offset='0' Color='{x:Static SystemColors.ControlColor}'/>")
		_T("    <GradientStop Offset='1' Color='{x:Static SystemColors.ControlDarkColor}'/>")
		_T("</LinearGradientBrush></Border.Background></Border>")

		_T("</Grid>");

	RenderMarkup(pDC, rc, lpszMarkupText);

	CXTPRibbonControlSystemButton* pControlPopup = DYNAMIC_DOWNCAST(CXTPRibbonControlSystemButton, pBar->GetControlPopup());

	if (!pControlPopup)
		return;


	CXTPRibbonBar* pRibbonBar = DYNAMIC_DOWNCAST(CXTPRibbonBar, pControlPopup->GetParent());
	if (!pRibbonBar)
		return;


	CRect rcPopup = pControlPopup->GetRect();
	pControlPopup->GetParent()->ClientToScreen(rcPopup);
	pBar->ScreenToClient(rcPopup);

	if (pRibbonBar->IsCaptionVisible() && CRect().IntersectRect(rcPopup, rc))
	{
		DrawRibbonFrameSystemButton(pDC, pControlPopup, rcPopup);
	}

	for (int i = 0; i < pBar->GetControlCount(); i++)
	{
		CXTPControl* pControl = pBar->GetControl(i);
		if (pControl && pControl->GetWrap())
		{
			CRect rcRow = pControl->GetRowRect();
			pDC->FillSolidRect(rcRow, GetXtremeColor(XPCOLOR_TOOLBAR_FACE));
			break;
		}
	}
}


void CXTPRibbonSystemTheme::DrawSystemPopupBarButton(CDC* pDC, CXTPControl* pButton)
{
	LPCTSTR lpszMarkupText =

		pButton->GetSelected() ?
		_T("<Grid>")
		_T("<Border BorderThickness='1' CornerRadius='2' BorderBrush='{x:Static SystemColors.ControlDarkBrush}'><Border.Background>")
		_T("<LinearGradientBrush StartPoint='0,0' EndPoint='0,1'>")
		_T("    <GradientStop Offset='0' Color='{x:Static SystemColors.ControlColor}'/>")
		_T("    <GradientStop Offset='1' Color='{x:Static SystemColors.ControlColor}'/>")
		_T("</LinearGradientBrush></Border.Background></Border>")
		_T("</Grid>")   :

		_T("<Grid>")
		_T("<Border BorderThickness='1' CornerRadius='2' BorderBrush='{x:Static SystemColors.ControlDarkBrush}'><Border.Background>")
		_T("<LinearGradientBrush StartPoint='0,0' EndPoint='0,1'>")
		_T("    <GradientStop Offset='0' Color='{x:Static SystemColors.ControlLightLightColor}'/>")
		_T("    <GradientStop Offset='1' Color='{x:Static SystemColors.ControlColor}'/>")
		_T("</LinearGradientBrush></Border.Background></Border>")
		_T("</Grid>");

	RenderMarkup(pDC, pButton->GetRect(), lpszMarkupText);


	m_pPaintManger->DrawControlToolBarParent(pDC, pButton, TRUE);
}

void CXTPRibbonSystemTheme::DrawRibbonFrameSystemButton(CDC* pDC, CXTPRibbonControlSystemButton* pControl, CRect rc)
{
	CXTPRibbonBar* pRibbonBar = (CXTPRibbonBar*)pControl->GetParent();

	if (!pRibbonBar->IsCaptionVisible() || !pRibbonBar->IsTabsVisible())
	{
		m_pPaintManger->DrawControlToolBarParent(pDC, pControl, TRUE);
		return;
	}

	XTPImageState nState = xtpImageNormal;
	if (pControl->GetPopuped())
		nState = xtpImagePressed;
	else if (pControl->GetSelected())
		nState = xtpImageHot;

	CXTPImageManagerIcon* pIcon = pControl->GetImage(32);

	if (pIcon && pIcon->GetWidth() > 32)
	{
		CPoint ptIcon((rc.left + rc.right - pIcon->GetWidth()) / 2, (rc.top + rc.bottom - pIcon->GetHeight()) / 2);

		pIcon->m_bDrawComposited = pControl->HasDwmCompositedRect();
		pIcon->Draw(pDC, ptIcon, pIcon->GetIcon(nState));
		pIcon->m_bDrawComposited = FALSE;
		return;
	}


	int nOffset = 0;
	if (rc.top < 0)
	{
		nOffset = rc.top;
		rc.OffsetRect(0, -nOffset);
	}

	{
		CSize szDest(46, 45);
		CPoint ptDest((rc.left + rc.right - szDest.cx) / 2, nOffset + (rc.top + rc.bottom - szDest.cy) / 2);
		LPCTSTR lpszMarkupText = NULL;

		if (pControl->GetPopuped())
		{

			lpszMarkupText =
				_T("<Grid Margin='0, 2, 0, 0'>")

				_T("<Ellipse Width='38' Height='38' Margin='0, 0, -1, -1'>")
				_T("<Ellipse.Fill>")
				_T("<LinearGradientBrush StartPoint='1, 1' EndPoint='0,0'>")
				_T("<GradientStop Offset='0' Color='#32000000' />")
				_T("<GradientStop Offset='0.95' Color='#01000000' />")
				_T("<GradientStop Offset='1' Color='Transparent' />")
				_T("</LinearGradientBrush>")
				_T("</Ellipse.Fill>         ")
				_T("</Ellipse>          ")
				_T("<Ellipse Width='37' Height='37'>")
				_T("<Ellipse.Fill>")
				_T("<SolidColorBrush Color='{x:Static SystemColors.HighlightColor}' />")
				_T("</Ellipse.Fill>         ")
				_T("</Ellipse>")
				_T("<Ellipse Width='37' Height='37' StrokeThickness='1'>")
				_T("<Ellipse.Fill>")
				_T("<LinearGradientBrush StartPoint='0, 0' EndPoint='0, 1'>")
				_T("<GradientStop Offset='0' Color='#FEFFFFFF' />")
				_T("<GradientStop Offset='0.25' Color='#AAFFFFFF' />")
				_T("<GradientStop Offset='0.5' Color='#55FFFFFF' />")
				_T("<GradientStop Offset='0.5' Color='#00FFFFFF' />")
				_T("<GradientStop Offset='1' Color='#66FFFFFF' />")
				_T("</LinearGradientBrush>")
				_T("</Ellipse.Fill>")
				_T("<Ellipse.Stroke>")
				_T("<LinearGradientBrush StartPoint='0,0' EndPoint='0,1'>")
				_T("<GradientStop Offset='0' Color='{x:Static SystemColors.ControlDarkColor}' />")
				_T("<GradientStop Offset='0.5' Color='{x:Static SystemColors.ControlDarkColor}' />")
				_T("<GradientStop Offset='1' Color='{x:Static SystemColors.ControlDarkColor}' />")
				_T("</LinearGradientBrush>")
				_T("</Ellipse.Stroke>")
				_T("</Ellipse>      ")
				_T("<Ellipse Width='35' Height='35' StrokeThickness='1'>")
				_T("<Ellipse.Stroke>")
				_T("<LinearGradientBrush StartPoint='0,0' EndPoint='1,1'>")
				_T("<GradientStop Offset='0' Color='#55E1E7EF' />")
				_T("<GradientStop Offset='0.5' Color='#55BCC8DB' />")
				_T("<GradientStop Offset='1' Color='#55FFFFFF' />")
				_T("</LinearGradientBrush>")
				_T("</Ellipse.Stroke>")
				_T("</Ellipse>")
				_T("</Grid>");
		}
		else if (pControl->GetSelected())
		{
			lpszMarkupText =
				_T("<Grid Margin='0, 2, 0, 0'>")

				_T("<Ellipse Width='38' Height='38' Margin='0, 0, -1, -1'>")
				_T("<Ellipse.Fill>")
				_T("<LinearGradientBrush StartPoint='1, 1' EndPoint='0,0'>")
				_T("<GradientStop Offset='0' Color='#32000000' />")
				_T("<GradientStop Offset='0.95' Color='#01000000' />")
				_T("<GradientStop Offset='1' Color='Transparent' />")
				_T("</LinearGradientBrush>")
				_T("</Ellipse.Fill>         ")
				_T("</Ellipse>          ")
				_T("<Ellipse Width='37' Height='37'>")
				_T("<Ellipse.Fill>")
				_T("<LinearGradientBrush StartPoint='0, 0' EndPoint='0,1'>")
				_T("<GradientStop Offset='0' Color='{x:Static SystemColors.ControlDarkColor}' />")
				_T("<GradientStop Offset='1' Color='{x:Static SystemColors.HighlightColor}' />")
				_T("</LinearGradientBrush>")
				_T("</Ellipse.Fill>         ")
				_T("</Ellipse>")
				_T("<Ellipse Width='37' Height='37' StrokeThickness='1'>")
				_T("<Ellipse.Fill>")
				_T("<LinearGradientBrush StartPoint='0, 0' EndPoint='0, 1'>")
				_T("<GradientStop Offset='0' Color='#FEFFFFFF' />")
				_T("<GradientStop Offset='0.25' Color='#AAFFFFFF' />")
				_T("<GradientStop Offset='0.5' Color='#55FFFFFF' />")
				_T("<GradientStop Offset='0.5' Color='#00FFFFFF' />")
				_T("<GradientStop Offset='1' Color='#66FFFFFF' />")
				_T("</LinearGradientBrush>")
				_T("</Ellipse.Fill>")
				_T("<Ellipse.Stroke>")
				_T("<LinearGradientBrush StartPoint='0,0' EndPoint='0,1'>")
				_T("<GradientStop Offset='0' Color='{x:Static SystemColors.ControlDarkColor}' />")
				_T("<GradientStop Offset='0.5' Color='{x:Static SystemColors.ControlDarkColor}' />")
				_T("<GradientStop Offset='1' Color='{x:Static SystemColors.ControlDarkColor}' />")
				_T("</LinearGradientBrush>")
				_T("</Ellipse.Stroke>")
				_T("</Ellipse>      ")
				_T("<Ellipse Width='35' Height='35' StrokeThickness='1'>")
				_T("<Ellipse.Stroke>")
				_T("<LinearGradientBrush StartPoint='0,0' EndPoint='1,1'>")
				_T("<GradientStop Offset='0' Color='#55E1E7EF' />")
				_T("<GradientStop Offset='0.5' Color='#55BCC8DB' />")
				_T("<GradientStop Offset='1' Color='#55FFFFFF' />")
				_T("</LinearGradientBrush>")
				_T("</Ellipse.Stroke>")
				_T("</Ellipse>")
				_T("</Grid>");
		}
		else
		{

			lpszMarkupText =
				_T("<Grid Margin='0, 2, 0, 0'>")

				_T("<Ellipse Width='38' Height='38' Margin='0, 0, -1, -1'>")
				_T("<Ellipse.Fill>")
				_T("<LinearGradientBrush StartPoint='1, 1' EndPoint='0,0'>")
				_T("<GradientStop Offset='0' Color='#32000000' />")
				_T("<GradientStop Offset='0.95' Color='#01000000' />")
				_T("<GradientStop Offset='1' Color='Transparent' />")
				_T("</LinearGradientBrush>")
				_T("</Ellipse.Fill>         ")
				_T("</Ellipse>          ")
				_T("<Ellipse Width='37' Height='37'>")
				_T("<Ellipse.Fill>")
				_T("<SolidColorBrush Color='{x:Static SystemColors.ControlDarkColor}' />")
				_T("</Ellipse.Fill>         ")
				_T("</Ellipse>")
				_T("<Ellipse Width='37' Height='37' StrokeThickness='1'>")
				_T("<Ellipse.Fill>")
				_T("<LinearGradientBrush StartPoint='0, 0' EndPoint='0, 1'>")
				_T("<GradientStop Offset='0' Color='#FEFFFFFF' />")
				_T("<GradientStop Offset='0.25' Color='#AAFFFFFF' />")
				_T("<GradientStop Offset='0.5' Color='#55FFFFFF' />")
				_T("<GradientStop Offset='0.5' Color='#00FFFFFF' />")
				_T("<GradientStop Offset='1' Color='#66FFFFFF' />")
				_T("</LinearGradientBrush>")
				_T("</Ellipse.Fill>")
				_T("<Ellipse.Stroke>")
				_T("<LinearGradientBrush StartPoint='0,0' EndPoint='0,1'>")
				_T("<GradientStop Offset='0' Color='{x:Static SystemColors.ControlDarkColor}' />")
				_T("<GradientStop Offset='0.5' Color='{x:Static SystemColors.ControlDarkColor}' />")
				_T("<GradientStop Offset='1' Color='{x:Static SystemColors.ControlDarkColor}' />")
				_T("</LinearGradientBrush>")
				_T("</Ellipse.Stroke>")
				_T("</Ellipse>      ")
				_T("<Ellipse Width='35' Height='35' StrokeThickness='1'>")
				_T("<Ellipse.Stroke>")
				_T("<LinearGradientBrush StartPoint='0,0' EndPoint='1,1'>")
				_T("<GradientStop Offset='0' Color='#E1E7EF' />")
				_T("<GradientStop Offset='0.5' Color='#BCC8DB' />")
				_T("<GradientStop Offset='1' Color='#FFFFFF' />")
				_T("</LinearGradientBrush>")
				_T("</Ellipse.Stroke>")
				_T("</Ellipse>")
				_T("</Grid>");
		}

		RenderMarkup(pDC, CRect(ptDest, szDest), lpszMarkupText);

	}

	if (pIcon)
	{
		CPoint ptIcon((rc.left + rc.right - pIcon->GetWidth()) / 2, nOffset + (rc.top + rc.bottom + 1 - pIcon->GetHeight()) / 2);
		pIcon->m_bDrawComposited = pControl->HasDwmCompositedRect();
		pIcon->Draw(pDC, ptIcon, pIcon->GetIcon(nState));
		pIcon->m_bDrawComposited = FALSE;
	}
	else
	{
		CWnd* pSite = pControl->GetParent()->GetSite();
		HICON hIcon = m_pPaintManger->GetFramePaintManager()->GetFrameLargeIcon(pSite);
		if (hIcon)
		{
			CPoint ptIcon((rc.left + rc.right - 33) / 2, nOffset + (rc.top + rc.bottom - 31) / 2);

				DrawIconEx(pDC->GetSafeHdc(), ptIcon.x, ptIcon.y, hIcon, 32, 32, NULL, NULL, DI_NORMAL);
		}
	}
}

void CXTPRibbonSystemTheme::DrawRibbonScrollButton(CDC* pDC, CXTPControl* pControl, BOOL bScrollLeft)
{
	CRect rc(pControl->GetRect());

	if (bScrollLeft)
	{
		LPCTSTR lpszMarkupText =
		_T("<Grid ClipToBounds='True' Background='{x:Static SystemColors.ControlBrush}' Margin='0, 0, 0, 2'>")
			_T("<Border Margin='1, 0, -3, 0' Background='{x:Static SystemColors.ControlBrush}' BorderThickness='1' CornerRadius='3' BorderBrush='{x:Static SystemColors.ControlDarkBrush}'/>")
			_T("<Border Margin='1, 0, 0, 0' BorderThickness='0, 0, 1, 0' BorderBrush='{x:Static SystemColors.ControlDarkBrush}'/>")
			_T("</Grid>");

		RenderMarkup(pDC, rc, lpszMarkupText);
	}
	else
	{
		LPCTSTR lpszMarkupText =
			_T("<Grid ClipToBounds='True' Background='{x:Static SystemColors.ControlBrush}' Margin='0, 0, 0, 2'>")
			_T("<Border Margin='-3, 0, 1, 0' Background='{x:Static SystemColors.ControlBrush}' BorderThickness='1' CornerRadius='3' BorderBrush='{x:Static SystemColors.ControlDarkBrush}'/>")
			_T("<Border Margin='0, 0, 1, 0' BorderThickness='1, 0, 0, 0' BorderBrush='{x:Static SystemColors.ControlDarkBrush}'/>")
			_T("</Grid>");

		RenderMarkup(pDC, rc, lpszMarkupText);
	}

	CPoint pt = rc.CenterPoint();

	if (bScrollLeft)
	{
		XTPDrawHelpers()->Triangle(pDC, CPoint(pt.x - 1, pt.y), CPoint(pt.x + 1, pt.y - 2), CPoint(pt.x + 1, pt.y + 2), GetSysColor(COLOR_BTNTEXT));
	}
	else
	{
		XTPDrawHelpers()->Triangle(pDC, CPoint(pt.x - 1, pt.y - 2), CPoint(pt.x + 1, pt.y), CPoint(pt.x - 1, pt.y + 2), GetSysColor(COLOR_BTNTEXT));
	}
}



BOOL CXTPRibbonSystemTheme::DrawRibbonGroupControlEntry(CDC* pDC, CXTPControl* pButton)
{
	if (pButton->GetRibbonGroup() && pButton->GetRibbonGroup()->IsControlsGrouping())
	{
		if (!pButton->GetSelected())
		{
			BOOL bLeft = pButton->GetBeginGroup() || pButton->GetWrap();

			int nIndexNext = pButton->GetControls()->GetNext(pButton->GetIndex());

			BOOL bRight = nIndexNext <= pButton->GetIndex() ||
				pButton->GetControls()->GetAt(nIndexNext)->GetBeginGroup() ||
				pButton->GetControls()->GetAt(nIndexNext)->GetWrap();


			LPCTSTR lpszMarkupText = NULL;

			if (bLeft && bRight)
				lpszMarkupText = _T("<Grid><Border BorderThickness='1' CornerRadius='2' BorderBrush='{x:Static SystemColors.ControlDarkBrush}'><Border.Background>")
				_T("<LinearGradientBrush StartPoint='0,0' EndPoint='0,1'>")
				_T("    <GradientStop Offset='0' Color='{x:Static SystemColors.ControlLightLightColor}'/>")
				_T("    <GradientStop Offset='1' Color='{x:Static SystemColors.ControlColor}'/>")
				_T("</LinearGradientBrush></Border.Background></Border></Grid>");
			else if (bLeft)
				lpszMarkupText = _T("<Grid ClipToBounds='True'><Border Margin='0, 0, -2, 0' BorderThickness='1' CornerRadius='2' BorderBrush='{x:Static SystemColors.ControlDarkBrush}'><Border.Background>")
				_T("<LinearGradientBrush StartPoint='0,0' EndPoint='0,1'>")
				_T("    <GradientStop Offset='0' Color='{x:Static SystemColors.ControlLightLightColor}'/>")
				_T("    <GradientStop Offset='1' Color='{x:Static SystemColors.ControlColor}'/>")
				_T("</LinearGradientBrush></Border.Background></Border><Rectangle Width='1' Stretch='Fill' Fill = '{x:Static SystemColors.ControlDarkBrush}' HorizontalAlignment='Right' /> </Grid>");
			else if (bRight)
				lpszMarkupText = _T("<Grid ClipToBounds='True'><Border Margin='-2, 0, 0, 0' BorderThickness='1' CornerRadius='2' BorderBrush='{x:Static SystemColors.ControlDarkBrush}'><Border.Background>")
				_T("<LinearGradientBrush StartPoint='0,0' EndPoint='0,1'>")
				_T("    <GradientStop Offset='0' Color='{x:Static SystemColors.ControlLightLightColor}'/>")
				_T("    <GradientStop Offset='1' Color='{x:Static SystemColors.ControlColor}'/>")
				_T("</LinearGradientBrush></Border.Background></Border></Grid>");
			else
				lpszMarkupText = _T("<Grid ClipToBounds='True'><Border Margin='-2, 0, -2, 0' BorderThickness='1' CornerRadius='2' BorderBrush='{x:Static SystemColors.ControlDarkBrush}'><Border.Background>")
				_T("<LinearGradientBrush StartPoint='0,0' EndPoint='0,1'>")
				_T("    <GradientStop Offset='0' Color='{x:Static SystemColors.ControlLightLightColor}'/>")
				_T("    <GradientStop Offset='1' Color='{x:Static SystemColors.ControlColor}'/>")
				_T("</LinearGradientBrush></Border.Background></Border><Rectangle Width='1' Stretch='Fill' Fill = '{x:Static SystemColors.ControlDarkBrush}' HorizontalAlignment='Right' /></Grid>");

			RenderMarkup(pDC, pButton->GetRect(), lpszMarkupText);
			return FALSE;
		}

		return FALSE;
	}

	return FALSE;
}
