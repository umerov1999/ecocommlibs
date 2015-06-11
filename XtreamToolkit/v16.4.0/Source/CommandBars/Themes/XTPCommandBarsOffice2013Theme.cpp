// XTPCommandBarsOffice2013Theme.cpp : implementation of the CXTPCommandBarsOffice2013Theme class.
//
// This file is a part of the XTREME COMMANDBARS MFC class library.
// (c)1998-2013 Codejock Software, All Rights Reserved.
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

#include "StdAfx.h"

#include <GraphicLibrary/unzip/unzip.h>
#include <Common/XTPMarkupTheme.h>
#include <Common/XTPResource.h>
#include <Common/XTPResourceManager.h>

#include <Common/XTPImageManager.h>
#include <Common/XTPResourceImage.h>
#include <Common/XTPDrawHelpers.h>
#include <Common/XTPHookManager.h>
#include <Common/XTPColorManager.h>
#include <Common/XTPSystemHelpers.h>
#include <Common/XTPWinThemeWrapper.h>
#include <Common/XTPTheme.h>
#include <Common/XTPMarkupRender.h>
#include <Common/XTPVC80Helpers.h>

// Scrollbar
#include <Common/ScrollBar/XTPScrollBase.h>
#include <Common/ScrollBar/XTPScrollInfo.h>
#include <Common/ScrollBar/XTPScrollBarPaintManager.h>
#include <Common/ScrollBar/Themes/XTPScrollBarThemeOffice2013.h>

#include <CommandBars/XTPCommandBarsDefines.h>
#include <CommandBars/XTPCommandBar.h>
#include <CommandBars/XTPPopupBar.h>
#include <CommandBars/XTPControl.h>
#include <CommandBars/XTPControlButton.h>
#include <CommandBars/XTPControlPopup.h>
#include <CommandBars/XTPControlGallery.h>
#include <CommandBars/XTPControlEdit.h>
#include <CommandBars/XTPControlComboBox.h>
#include <CommandBars/XTPCommandBars.h>
#include <CommandBars/XTPDockBar.h>

#include <CommandBars/XTPPaintManager.h>
#include <CommandBars/Themes/XTPCommandBarsOfficeXPTheme.h>
#include <CommandBars/Themes/ModernUI/XTPCommandBarsModernUITheme.h>
#include <CommandBars/Themes/XTPCommandBarsOffice2013Theme.h>
#include <CommandBars/Themes/Office2013/XTPControlGalleryOffice2013Theme.h>

#include <CommandBars/XTPCommandBarsColorSet.h>
#include <CommandBars/ColorSets/XTPCommandBarsColorSetOffice2013.h>

#ifdef _XTP_INCLUDE_RIBBON
#include <Ribbon/XTPRibbonPaintManager.h>
#include <Ribbon/XTPRibbonMarkupTheme.h>
#include <Ribbon/Themes/XTPRibbonThemeOffice2013.h>
#endif

// Progress
#include <CommandBars/Progress/XTPProgressPaintManager.h>
#include <CommandBars/Progress/XTPProgressThemeWindowsUx.h>

// Slider
#include <CommandBars/Slider/XTPSliderPaintManager.h>
#include <CommandBars/Slider/XTPSliderThemeOffice2013.h>

// Frame
#include <CommandBars/Frame/XTPFramePaintManager.h>
#ifdef _XTP_INCLUDE_RIBBON
#include <Ribbon/XTPRibbonMarkupFrameTheme.h>
#endif
#include <CommandBars/Frame/XTPFrameThemeModernUI.h>
#include <CommandBars/Frame/XTPFrameThemeOffice2013.h>

// Message bar
#include <CommandBars/MessageBar/XTPMessageBar.h>
#include <CommandBars/MessageBar/XTPMessageBarPaintManager.h>
#include <CommandBars/MessageBar/XTPMessageBarThemeOffice2013.h>

// Status bar
#include <CommandBars/StatusBar/XTPStatusBar.h>
#include <CommandBars/StatusBar/XTPStatusBarPaintManager.h>
#include <CommandBars/StatusBar/XTPStatusBarThemeOffice2013.h>

#ifdef _XTP_INCLUDE_RIBBON
#endif

//////////////////////////////////////////////////////////////////////
// CXTPCommandBarsOffice2013Theme

#define Base CXTPCommandBarsModernUITheme

IMPLEMENT_DYNAMIC(CXTPCommandBarsOffice2013Theme, Base)

CXTPCommandBarsOffice2013Theme::CXTPCommandBarsOffice2013Theme()
	: m_bIntersectMenuBorder(FALSE)
{
	// Frame paint manager
	CXTPFrameThemeOffice2013 *pFramePaintManager = new CXTPFrameThemeOffice2013(this);
	SAFE_DELETE(m_pFramePaintManager);
	m_pFramePaintManager = pFramePaintManager;

	// Progress paint manager
	SAFE_DELETE(m_pProgressPaintManager);
	m_pProgressPaintManager = new CXTPProgressThemeWindowsUx(this);

	// Slider paint manager
	SAFE_DELETE(m_pSliderPaintManager);
	m_pSliderPaintManager = new CXTPSliderThemeOffice2013(this);

	// Message bar paint manager
	SAFE_DELETE(m_pMessageBarPaintManager);
	m_pMessageBarPaintManager = new CXTPMessageBarThemeOffice2013(this);

	// Status bar paint manager
	SAFE_DELETE(m_pStatusBarPaintManager);
	m_pStatusBarPaintManager = new CXTPStatusBarThemeOffice2013(this);

	// Ribbon paint manager
	CXTPRibbonOffice2013Theme *pRibbonPaintManager = new CXTPRibbonOffice2013Theme(this);
	SAFE_DELETE(m_pRibbonPaintManager);
	m_pRibbonPaintManager = pRibbonPaintManager;

	// Gallery paint manager
	CXTPControlGalleryOffice2013Theme *pGalleryPaintManager = new CXTPControlGalleryOffice2013Theme(this);
	SAFE_DELETE(m_pGalleryPaintManager);
	m_pGalleryPaintManager = pGalleryPaintManager;

	// Scrollbar paint manager
	CXTPScrollBarThemeOffice2013 *pScrollBarPaintManager = new CXTPScrollBarThemeOffice2013();
	SAFE_DELETE(m_pScrollBarPaintManager);
	m_pScrollBarPaintManager = pScrollBarPaintManager;
}

CXTPCommandBarsOffice2013Theme::~CXTPCommandBarsOffice2013Theme()
{
}

void CXTPCommandBarsOffice2013Theme::SetImageHandle(HMODULE hResource, LPCTSTR lpszIniFileName)
{
	XTPResourceImages()->SetHandle(hResource, lpszIniFileName);
}



void CXTPCommandBarsOffice2013Theme::SetColorSet(const CXTPCommandBarsColorSet *pColorSet)
{
	// Menu pop-up background, gallery background
	m_arrColor[XPCOLOR_MENUBAR_BORDER]           = pColorSet->m_clrMenuBorder;
	m_arrColor[XPCOLOR_MENUBAR_FACE]             = pColorSet->m_clrMenuBackground;

	m_arrColor[XPCOLOR_SEPARATOR]                = pColorSet->m_clrSeparator;

	m_arrColor[XPCOLOR_HIGHLIGHT_BORDER]         = pColorSet->m_clrButtonBorderHighlighted;
	m_arrColor[XPCOLOR_HIGHLIGHT]                = pColorSet->m_clrButtonBackgroundHighlighted;
	m_arrColor[XPCOLOR_HIGHLIGHT_CHECKED_BORDER] = pColorSet->m_clrButtonBorderChecked;
	m_arrColor[XPCOLOR_HIGHLIGHT_CHECKED]        = pColorSet->m_clrButtonBackgroundChecked;
	m_arrColor[XPCOLOR_HIGHLIGHT_PUSHED_BORDER]  = pColorSet->m_clrButtonBorderPushed;
	m_arrColor[XPCOLOR_HIGHLIGHT_PUSHED]         = pColorSet->m_clrButtonBackgroundPushed;

	m_arrColor[XPCOLOR_TOOLBAR_FACE]             = pColorSet->m_clrToolbarFace;
	m_arrColor[XPCOLOR_EDITCTRLBORDER]           = pColorSet->m_clrEditCtrlBorder;

	m_clrFloatingGripper = pColorSet->m_clrMenuBorder;;
}


void CXTPCommandBarsOffice2013Theme::RefreshMetrics()
{
	Base::RefreshMetrics();

	CXTPCommandBarsColorSetOffice2013Word csWord2013;
	csWord2013.RefreshColors(NULL);

	SetColorSet(&csWord2013);

	m_clrMessageBarText = RGB(68,68,68);

	if (XTP_DEFAULT_DISPLAY_DPI < XTPDpiHelper()->GetDPIX()) // this m_nEditHeight adjustment is valid only for screen DPI is higher than 100%
	{
		CNonClientMetrics ncm;
		int nBaseHeight = abs(ncm.lfMenuFont.lfHeight);
		if (XTP_COMMANDBAR_MIN_FONT_HEIGHT < m_nFontHeight)
			nBaseHeight = m_nFontHeight;

		if (nBaseHeight < XTP_COMMANDBAR_MIN_FONT_HEIGHT)
			nBaseHeight = XTP_COMMANDBAR_MIN_FONT_HEIGHT;

		m_nEditHeight = max(XTP_COMMANDBAR_MIN_CONTROL_HEIGHT, nBaseHeight * 195 / XTP_DEFAULT_DISPLAY_DPI);
	}
}

void CXTPCommandBarsOffice2013Theme::FillCommandBarEntry(CDC *pDC, CXTPCommandBar *pBar)
{
	CRect rc;
	pBar->GetClientRect(&rc);

	if (pBar->GetPosition() == xtpBarFloating)
	{
		pDC->Draw3dRect(rc, m_arrColor[XPCOLOR_MENUBAR_BORDER], m_arrColor[XPCOLOR_MENUBAR_BORDER]);
		rc.DeflateRect(1,1,1,1);
		pDC->FillSolidRect(rc, m_arrColor[XPCOLOR_MENUBAR_FACE]);
	}
	else if (xtpBarPopup == pBar->GetPosition())
	{
		Rectangle(pDC, rc, XPCOLOR_MENUBAR_BORDER, XPCOLOR_MENUBAR_FACE);
		if (m_bIntersectMenuBorder)
		{
			FillIntersectRect(pDC, (CXTPPopupBar*)pBar, GetXtremeColor(XPCOLOR_MENUBAR_FACE));
		}
	}
	else if (IsFlatToolBar(pBar))
	{
		pDC->FillSolidRect(rc, GetXtremeColor(XPCOLOR_TOOLBAR_FACE));
	}
	else // Docked command bar
	{
		COLORREF clrToolBarBorder = GetXtremeColor(XPCOLOR_MENUBAR_BORDER);

		pDC->FillSolidRect(rc, GetXtremeColor(XPCOLOR_TOOLBAR_FACE));
		pDC->Draw3dRect(rc, clrToolBarBorder, clrToolBarBorder);
	}
}

void CXTPCommandBarsOffice2013Theme::FillDockBar(CDC *pDC, CXTPDockBar *pBar)
{
	CXTPClientRect rcClient(pBar);
	pDC->FillSolidRect(rcClient, GetXtremeColor(XPCOLOR_TOOLBAR_FACE));
}

void CXTPCommandBarsOffice2013Theme::DrawCommandBarGroupSeparator(CDC* pDC, CXTPCommandBar* pBar, CXTPControl* pLeftControl, CXTPControl* pRightControl, BOOL bDraw)
{
	UNREFERENCED_PARAMETER(pBar);

	if(bDraw)
	{
		ASSERT(NULL != pLeftControl);
		ASSERT(NULL != pRightControl);

		const CRect& rcLeftControl = pLeftControl->GetRect();
		const CRect& rcRightControl = pRightControl->GetRect();
		const int x = (rcRightControl.left + rcLeftControl.right) / 2;
		const int topY = min(rcRightControl.top, rcLeftControl.top);
		const int bottomY = max(rcRightControl.bottom, rcLeftControl.bottom);
		Line(pDC, x, topY, x, bottomY, XPCOLOR_HIGHLIGHT_BORDER);
	}
}

CSize CXTPCommandBarsOffice2013Theme::DrawControlEdit(CDC* pDC, CXTPControlEdit* pControlEdit, BOOL bDraw)
{
	if (!bDraw)
	{
		return CXTPCommandBarsModernUITheme::DrawControlEdit(pDC, pControlEdit, bDraw);
	}

	//BOOL bPopupBar = pControlEdit->GetParent()->GetType() == xtpBarTypePopup;
	BOOL bSelected = pControlEdit->GetSelected();
	BOOL bEnabled = pControlEdit->GetEnabled();

	CXTPPaintManager::DrawControlEdit(pDC, pControlEdit, bDraw);

	CRect rc = pControlEdit->GetRect();
	rc.left += pControlEdit->GetLabelWidth();


	COLORREF clrBackground = GetControlEditBackColor(pControlEdit);

	if (!bEnabled)
	{
		rc.DeflateRect(1, 0, 0, 0);

		pDC->FillSolidRect(rc, clrBackground);

		COLORREF clrBorder = GetXtremeColor(IsKeyboardSelected(bSelected) ? XPCOLOR_HIGHLIGHT_BORDER : XPCOLOR_DISABLED);
		pDC->Draw3dRect(rc, clrBorder, clrBorder);
	}
	else
	{
		pDC->FillSolidRect(rc.left + 1, rc.top + 1, rc.Width() - (pControlEdit->IsSpinButtonsVisible() ? 18 : 2),
			rc.Height() - 2, clrBackground);

		if (bSelected)
		{
			Draw3dRect(pDC, rc, XPCOLOR_HIGHLIGHT_PUSHED_BORDER, XPCOLOR_HIGHLIGHT_PUSHED_BORDER);
		}
		else //if (bPopupBar)
		{
			Draw3dRect(pDC, rc, XPCOLOR_EDITCTRLBORDER, XPCOLOR_EDITCTRLBORDER);
		}
	}

	CRect rcText(pControlEdit->GetRect());
	rcText.DeflateRect(pControlEdit->GetLabelWidth() + 3, 1, 3, 1);
	pControlEdit->DrawEditText(pDC, rcText);

	if (pControlEdit->IsSpinButtonsVisible())
	{
		BOOL bPressed = pControlEdit->GetPressed();
		CRect rcSpin(pControlEdit->GetSpinButtonsRect());
		CRect rcTop(rcSpin.left, rcSpin.top, rcSpin.right, rcSpin.CenterPoint().y);
		CRect rcBottom(rcSpin.left, rcTop.bottom, rcSpin.right, rcSpin.bottom);

		if(bEnabled)
		{
			// Draw top spin button.
			COLORREF clrTop = clrBackground;
			if(XTP_EDITSPIN_UP == bPressed)
			{
				clrTop = GetXtremeColor(XPCOLOR_HIGHLIGHT_PUSHED);
			}
			else if(XTP_EDITSPIN_UP == bSelected)
			{
				clrTop = GetXtremeColor(XPCOLOR_HIGHLIGHT);
			}
			pDC->FillSolidRect(rcTop, clrTop);

			// Draw bottom spin button.
			COLORREF clrBottom = clrBackground;
			if(XTP_EDITSPIN_DOWN == bPressed)
			{
				clrBottom = GetXtremeColor(XPCOLOR_HIGHLIGHT_PUSHED);
			}
			else if(XTP_EDITSPIN_DOWN == bSelected)
			{
				clrBottom = GetXtremeColor(XPCOLOR_HIGHLIGHT);
			}
			pDC->FillSolidRect(rcBottom, clrBottom);

			// Draw spin button borders.
			CPoint ptCursor;
			GetCursorPos(&ptCursor);
			pControlEdit->GetEditCtrl()->GetParent()->ScreenToClient(&ptCursor);
			if(rcSpin.PtInRect(ptCursor))
			{
				pDC->FillSolidRect(rcSpin.left, rcSpin.top, 1, rcSpin.Height(), GetXtremeColor(XPCOLOR_HIGHLIGHT_PUSHED_BORDER));
				pDC->FillSolidRect(rcSpin.left, rcBottom.top, rcSpin.Width(), 1, GetXtremeColor(XPCOLOR_HIGHLIGHT_PUSHED_BORDER));
			}
		}
		else
		{
			// Draw disabled spin buttons.
			pDC->FillSolidRect(rcTop, clrBackground);
			pDC->FillSolidRect(rcBottom, clrBackground);
		}

		// Draw top spin button triangle.
		CPoint pt = rcTop.CenterPoint();
		Triangle(pDC, CPoint(pt.x, pt.y - 1), CPoint(pt.x - 2, pt.y + 1), CPoint(pt.x + 2, pt.y + 1),
			GetXtremeColor(bEnabled && XTP_EDITSPIN_UP == bSelected ? COLOR_BTNTEXT : COLOR_GRAYTEXT));

		// Draw bottom spin button triangle.
		pt = rcBottom.CenterPoint();
		Triangle(pDC, CPoint(pt.x, pt.y + 1), CPoint(pt.x - 2, pt.y - 1), CPoint(pt.x + 2, pt.y - 1),
			GetXtremeColor(bEnabled && XTP_EDITSPIN_DOWN == bSelected ? COLOR_BTNTEXT : COLOR_GRAYTEXT));
	}

	if (pControlEdit->IsBuddyButtonVisible())
	{
		BOOL bPressed = pControlEdit->GetPressed();
		CRect rc(pControlEdit->GetBuddyButtonRect());

		DrawRectangle(pDC, rc, bSelected == XTP_EDITBUDDYBUTTON, bPressed == XTP_EDITBUDDYBUTTON, bEnabled,  FALSE, FALSE, xtpBarTypeNormal, xtpBarTop);

		CXTPImageManager* pImageManager = pControlEdit->GetParent()->GetImageManager();

		CXTPImageManagerIcon* pIcon = pImageManager->GetImage(pControlEdit->GetBuddyButtonId());

		if (pIcon)
		{
			CSize sz(pIcon->GetExtent());
			pIcon->Draw(pDC, CPoint((rc.left + rc.right - sz.cx) / 2, (rc.top + rc.bottom - sz.cy) / 2),
				pIcon->GetIcon(!bEnabled ? xtpImageDisabled : pControlEdit->GetSelected() == XTP_EDITBUDDYBUTTON ?
					xtpImageHot : xtpImageNormal));
		}
	}

	return 0;
}

void CXTPCommandBarsOffice2013Theme::DrawRectangle(CDC* pDC, CRect rc, BOOL bSelected, BOOL bPressed, BOOL bEnabled, BOOL bChecked, BOOL bPopuped, XTPBarType barType, XTPBarPosition barPosition)
{
	if (!bEnabled)
	{
		Base::DrawRectangle(pDC, rc, bSelected, bPressed, bEnabled, bChecked, bPopuped, barType, barPosition);
	}
	else
	{
		if (bPopuped)
		{
			Rectangle(pDC, rc, XPCOLOR_HIGHLIGHT_PUSHED_BORDER, XPCOLOR_HIGHLIGHT_PUSHED);
		}
		else
		{
			Base::DrawRectangle(pDC, rc, bSelected, bPressed, bEnabled, bChecked, bPopuped, barType, barPosition);
		}
	}
}

void CXTPCommandBarsOffice2013Theme::LoadResource()
{
	CXTPResource resource;
	resource.LoadResource(XTPGetInstanceHandle(), _T("XTP_IDR_OFFICE2013_STYLE"), _T("STYLE"));

	ASSERT(resource.m_hResource);

	if (resource.m_hResource) // should be checked
	{
		HZIP hZip = OpenZip(resource.m_pvResourceData, resource.m_dwSize, NULL);

		if (hZip)
		{



			CloseZip(hZip);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// Workspace

void CXTPCommandBarsOffice2013Theme::FillWorkspace(CDC *pDC, CRect rc, CRect rcExclude)
{
	UNUSED_ALWAYS(rcExclude);

	CXTPPaintManagerColorGradient clrWorkspace;
	clrWorkspace.clrLight = RGB(255, 255, 255);
	clrWorkspace.clrDark  = RGB(240, 240, 240);
	XTPDrawHelpers()->GradientFill(pDC, rc, clrWorkspace, FALSE);
}
