// XTPCommandBarsVisualStudio2012Theme.cpp : implementation of the CXTPCommandBarsVisualStudio2012Theme class.
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
#include "Common/XTPResourceManager.h"

#include <Common/XTPImageManager.h>
#include <Common/XTPResourceImage.h>
#include <Common/XTPDrawHelpers.h>
#include <Common/XTPHookManager.h>
#include <Common/XTPColorManager.h>
#include <Common/XTPSystemHelpers.h>
#include <Common/XTPWinThemeWrapper.h>
#include <Common/XTPTheme.h>
#include <Common/XTPMarkupRender.h>

// Scrollbar
#include <Common/ScrollBar/XTPScrollBase.h>
#include <Common/ScrollBar/XTPScrollInfo.h>
#include <Common/ScrollBar/XTPScrollBarPaintManager.h>
#include <Common/ScrollBar/Themes/XTPScrollBarThemeVisualStudio2012.h>

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
#include <CommandBars/XTPToolBar.h>
#include <CommandBars/XTPMenuBar.h>
#include <CommandBars/Resource.h>

#include <CommandBars/Themes/XTPCommandBarsOfficeXPTheme.h>
#include <CommandBars/Themes/ModernUI/XTPCommandBarsModernUITheme.h>
#include <CommandBars/Themes/XTPCommandBarsVisualStudio2012Theme.h>

#include <CommandBars/XTPCommandBarsColorSet.h>
#include <CommandBars/ColorSets/XTPCommandBarsColorSetVisualStudio2012.h>

// Frame
#include <CommandBars/Frame/XTPFramePaintManager.h>
#ifdef _XTP_INCLUDE_RIBBON
#include <Ribbon/XTPRibbonMarkupFrameTheme.h>
#endif
#include <CommandBars/Frame/XTPFrameThemeModernUI.h>
#include <CommandBars/Frame/XTPFrameThemeVisualStudio2012.h>

// Status bar
#include <CommandBars/StatusBar/XTPStatusBar.h>
#include <CommandBars/StatusBar/XTPStatusBarPaintManager.h>
#include <CommandBars/StatusBar/XTPStatusBarThemeVisualStudio2012.h>

#define Base CXTPCommandBarsModernUITheme

IMPLEMENT_DYNAMIC(CXTPCommandBarsVisualStudio2012Theme, Base)

CXTPCommandBarsVisualStudio2012Theme::CXTPCommandBarsVisualStudio2012Theme(XTPPaintTheme paintTheme)
	: Base()
	, m_pColorSet(NULL)
{
	SAFE_DELETE(m_pStatusBarPaintManager);
	m_pStatusBarPaintManager = new CXTPStatusBarThemeVisualStudio2012(this);

	SAFE_DELETE(m_pFramePaintManager);

	// Frame and color set theme selection.
	ASSERT(xtpThemeVisualStudio2012Light == paintTheme || xtpThemeVisualStudio2012Dark == paintTheme);

	if (paintTheme == xtpThemeVisualStudio2012Dark)
	{
		SAFE_DELETE(m_pFramePaintManager);
		m_pFramePaintManager = new CXTPFrameThemeVisualStudio2012Dark(this);

		// Scrollbar paint manager
		SAFE_DELETE(m_pScrollBarPaintManager);
		m_pScrollBarPaintManager = new CXTPScrollBarThemeVisualStudio2012();

		m_pColorSet = new CXTPCommandBarsColorSetVisualStudio2012Dark();
	}
	else
	{
		SAFE_DELETE(m_pFramePaintManager);
		m_pFramePaintManager = new CXTPFrameThemeVisualStudio2012Light(this);

		// Scrollbar paint manager
		SAFE_DELETE(m_pScrollBarPaintManager);
		m_pScrollBarPaintManager = new CXTPScrollBarThemeVisualStudio2012();

		m_pColorSet = new CXTPCommandBarsColorSetVisualStudio2012Light();
	}

	ASSERT(NULL != m_pFramePaintManager);
	ASSERT(NULL != m_pColorSet);

	CXTPResource resource;

	if (resource.LoadResource(XTPGetInstanceHandle(), _T("XTP_IDR_VS2012_STYLE"), _T("STYLE")))
	{
		HZIP hZip = OpenZip(resource.m_pvResourceData, resource.m_dwSize, NULL);

		if (hZip)
		{
			static_cast<CXTPFrameThemeVisualStudio2012*>(m_pFramePaintManager)->LoadParts(hZip, m_pMarkupContext);
			static_cast<CXTPFrameThemeVisualStudio2012*>(m_pFramePaintManager)->RefreshMetrics(XTPResourceImages());

			CloseZip(hZip);
		}
	}

	m_bThickCheckMark = TRUE;
}

CXTPCommandBarsVisualStudio2012Theme::~CXTPCommandBarsVisualStudio2012Theme()
{
	SAFE_DELETE(m_pColorSet);
}

void CXTPCommandBarsVisualStudio2012Theme::RefreshMetrics()
{
	Base::RefreshMetrics();
	m_pFramePaintManager->RefreshMetrics();

	m_pColorSet->RefreshColors(NULL);

	m_arrColor[COLOR_BTNFACE]           = m_pColorSet->m_clrToolBarBackground;
	m_arrColor[XPCOLOR_TOOLBAR_FACE]    = m_pColorSet->m_clrToolBarBackground;
	m_arrColor[XPCOLOR_TOOLBAR_GRIPPER] = m_pColorSet->m_clrToolBarGripper;
	m_arrColor[XPCOLOR_TOOLBAR_TEXT]    = m_pColorSet->m_clrToolBarText;

	m_arrColor[XPCOLOR_MENUBAR_TEXT]     = m_pColorSet->m_clrMenuText;
	m_arrColor[XPCOLOR_HIGHLIGHT_TEXT]   = m_pColorSet->m_clrMenuTextHighlighted;
	m_arrColor[XPCOLOR_MENUBAR_BORDER]   = m_pColorSet->m_clrMenuBorder;
	m_arrColor[XPCOLOR_MENUBAR_FACE]     = m_pColorSet->m_clrMenuBackground;

	m_arrColor[XPCOLOR_HIGHLIGHT_BORDER] = m_pColorSet->m_clrButtonBorderHighlighted;
	m_arrColor[XPCOLOR_HIGHLIGHT]        = m_pColorSet->m_clrButtonBackgroundHighlighted;

	m_arrColor[XPCOLOR_HIGHLIGHT_PUSHED_BORDER] = m_pColorSet->m_clrButtonBorderPushed;
	m_arrColor[XPCOLOR_HIGHLIGHT_PUSHED]        = m_pColorSet->m_clrButtonBackgroundPushed;

	m_arrColor[XPCOLOR_HIGHLIGHT_CHECKED_BORDER] = m_pColorSet->m_clrButtonBorderChecked;
	m_arrColor[XPCOLOR_HIGHLIGHT_CHECKED]        = m_pColorSet->m_clrButtonBackgroundChecked;

	m_arrColor[XPCOLOR_EDITCTRLBORDER] = m_pColorSet->m_csComboBoxNormal.m_clrComboBoxBorder;
	m_arrColor[COLOR_WINDOW]           = m_pColorSet->m_csComboBoxNormal.m_clrComboBoxBackground;

	m_arrColor[COLOR_APPWORKSPACE]     = m_pColorSet->m_clrAppWorkspace;

	m_clrEditTextNormal      = m_pColorSet->m_clrControlText;
	m_clrEditTextDisabled    = m_pColorSet->m_clrControlTextDisabled;
	m_clrEditTextHighlighted = m_pColorSet->m_clrControlTextHighlighted;

	//for (int i=0; i<XPCOLOR_LAST; i++)
	//{
	//  //m_arrColor[i] = RGB(255,0,0);
	//}

	m_clrStatusBarBackground = RGB(1,120,208);
	m_clrStatusTextGrayColor = m_clrStatusBarBackground;
	m_clrStatusGripperLite = RGB(84,191,255);
	m_clrStatusGripperDark = RGB(0,91,158);
}

void DrawDashedLine(CDC *pDC, int x, int y1, int y2, COLORREF clr)
{
	while(y2 > y1)
	{
		pDC->SetPixel(x, y1, clr);
		y1 += 4;
	}
}

CSize CXTPCommandBarsVisualStudio2012Theme::DrawCommandBarGripper(CDC *pDC, CXTPCommandBar *pBar, BOOL bDraw)
{
	CSize szGripper(0,0);

	CRect rcBar;
	pBar->GetClientRect(&rcBar);

	if (xtpBarTop    == pBar->GetPosition() ||
		 xtpBarBottom == pBar->GetPosition())
	{
		szGripper = CSize(20, 0);

		if (bDraw)
		{
			DrawDashedLine(pDC, rcBar.left + 10, rcBar.top + 5, rcBar.bottom - 3, GetXtremeColor(XPCOLOR_TOOLBAR_GRIPPER));
			DrawDashedLine(pDC, rcBar.left + 12, rcBar.top + 7, rcBar.bottom - 5, GetXtremeColor(XPCOLOR_TOOLBAR_GRIPPER));
			DrawDashedLine(pDC, rcBar.left + 14, rcBar.top + 5, rcBar.bottom - 3, GetXtremeColor(XPCOLOR_TOOLBAR_GRIPPER));
		}
	}
	else
	{
		szGripper = Base::DrawCommandBarGripper(pDC, pBar, bDraw);
	}

	return szGripper;
}

COLORREF CXTPCommandBarsVisualStudio2012Theme::GetControlEditBackColor(CXTPControl* pControl)
{
	COLORREF clrEditColor = CXTPCommandBarsModernUITheme::GetControlEditBackColor(pControl);

	CXTPControlComboBox* pControlCombo = DYNAMIC_DOWNCAST(CXTPControlComboBox, pControl);

	if (pControlCombo)
	{
		BOOL bDropped  = pControlCombo->GetDroppedState() != 0;
		BOOL bSelected = pControlCombo->GetSelected();
		BOOL bEnabled  = pControlCombo->GetEnabled();

		CXTPCommandBarsColorSetComboBox *pColorSet = NULL;

		if (NULL != m_pColorSet)
		{
			if (!bEnabled ) pColorSet = &m_pColorSet->m_csComboBoxDisabled;
			else if ( bDropped ) pColorSet = &m_pColorSet->m_csComboBoxDropped;
			else if ( bSelected) pColorSet = &m_pColorSet->m_csComboBoxHighlighted;
			else                 pColorSet = &m_pColorSet->m_csComboBoxNormal;

			clrEditColor = pColorSet->m_clrComboBoxBackground;
		}
	}

	return clrEditColor;

}

CSize CXTPCommandBarsVisualStudio2012Theme::DrawControlComboBox(CDC* pDC, CXTPControlComboBox* pControlCombo, BOOL bDraw)
{
	if (!bDraw)
	{
		return CXTPPaintManager::DrawControlComboBox(pDC, pControlCombo, bDraw);
	}

	BOOL bDropped  = pControlCombo->GetDroppedState() != 0;
	BOOL bSelected = pControlCombo->GetSelected();
	BOOL bEnabled  = pControlCombo->GetEnabled();

	CXTPCommandBarsColorSetComboBox *pColorSet = NULL;

	if (NULL != m_pColorSet)
	{
			 if (!bEnabled ) pColorSet = &m_pColorSet->m_csComboBoxDisabled;
		else if ( bDropped ) pColorSet = &m_pColorSet->m_csComboBoxDropped;
		else if ( bSelected) pColorSet = &m_pColorSet->m_csComboBoxHighlighted;
		else                 pColorSet = &m_pColorSet->m_csComboBoxNormal;
	}

	int nThumb = pControlCombo->GetThumbWidth();

	CRect rcComboBox = pControlCombo->GetRect();
	rcComboBox.left += pControlCombo->GetLabelWidth();

	CRect rcDropDown(rcComboBox.right - nThumb, rcComboBox.top + 1, rcComboBox.right - 1, rcComboBox.bottom - 1);
	CRect rcText(rcComboBox.left + 3, rcComboBox.top + 1, rcComboBox.right - rcDropDown.Width(), rcComboBox.bottom - 1);

	CXTPPaintManager::DrawControlComboBox(pDC, pControlCombo, bDraw); // Draw label

	if (NULL != pColorSet)
	{
		pDC->FillSolidRect(rcComboBox, pColorSet->m_clrComboBoxBackground);
		pDC->Draw3dRect(rcComboBox, pColorSet->m_clrComboBoxBorder, pColorSet->m_clrComboBoxBorder);

		// Drop-down
		pDC->FillSolidRect(rcDropDown, pColorSet->m_dropDown.m_clrDropDownBackground);

		if (bEnabled)
			CXTPDrawHelpers::DrawLine(pDC, rcDropDown.left, rcDropDown.top, rcDropDown.left, rcDropDown.bottom, pColorSet->m_dropDown.m_clrDropDownBorder);

		DrawComboExpandMark(pDC, rcDropDown, pColorSet->m_dropDown.m_clrDropDownMarker);

		rcDropDown.DeflateRect(1, 1);
	}

	CXTPFontDC font(pDC, GetIconFont());
	pControlCombo->DrawEditText(pDC, rcText);


	return 0;
}

void CXTPCommandBarsVisualStudio2012Theme::FillDockBar(CDC *pDC, CXTPDockBar *pBar)
{
	CXTPClientRect rcClient(pBar);
	pDC->FillSolidRect(rcClient, GetXtremeColor(XPCOLOR_TOOLBAR_FACE));
}

void CXTPCommandBarsVisualStudio2012Theme::FillCommandBarEntry(CDC *pDC, CXTPCommandBar *pBar)
{
	CRect rc;
	pBar->GetClientRect(&rc);

	if (xtpBarPopup == pBar->GetPosition())
	{
		Rectangle(pDC, rc, XPCOLOR_MENUBAR_BORDER, XPCOLOR_MENUBAR_FACE);

		CXTPCommandBar* pParentBar = pBar->GetParentCommandBar();
		if (pParentBar && xtpBarPopup != pParentBar->GetPosition())
		{
			FillIntersectRect(pDC, (CXTPPopupBar*)pBar, GetXtremeColor(XPCOLOR_MENUBAR_FACE));
		}
	}
	else
	{
		Base::FillCommandBarEntry(pDC, pBar);
	}
}

COLORREF CXTPCommandBarsVisualStudio2012Theme::GetGlyphColor(CXTPControl* pControl)
{
	COLORREF clr = COLORREF_NULL;

	if (!pControl->GetEnabled())
	{
		clr = m_pColorSet->m_clrCheckMarkDisabled;
	}
	else if (pControl->GetSelected() && !pControl->GetPressed())
	{
		clr = m_pColorSet->m_clrGlyphHighlighted;
	}
	else if (pControl->GetSelected() && pControl->GetPressed())
	{
		clr = (pControl->GetType() == xtpControlSplitButtonPopup) ? m_pColorSet->m_clrGlyphHighlighted : m_pColorSet->m_clrGlyphPushed;
	}
	else
	{
		clr = m_pColorSet->m_clrGlyph;
	}

	return clr;
}

void CXTPCommandBarsVisualStudio2012Theme::DrawControlPopupGlyph(CDC* pDC, CXTPControl* pButton)
{
	CRect rcButton = pButton->GetRect();
	CPoint pt = CPoint(rcButton.right - 11, rcButton.CenterPoint().y);
	Triangle(pDC, CPoint(pt.x, pt.y - 3), CPoint(pt.x + 3, pt.y), CPoint(pt.x, pt.y + 3), GetGlyphColor(pButton));
}

AFX_INLINE void FillRect(CDC* pDC, CRect rc, BOOL bCheckMark, COLORREF clrFore, COLORREF clrBack)
{
	if (bCheckMark)
	{
		rc.DeflateRect(3,1,0,1);
		pDC->Draw3dRect(rc, clrFore, clrFore);

		rc.DeflateRect(1,1);
		pDC->Draw3dRect(rc, clrFore, clrFore);

		rc.DeflateRect(1,1);
		pDC->FillSolidRect(rc, clrBack);
	}
	else
	{
		pDC->Draw3dRect(rc, clrFore, clrFore);
		rc.DeflateRect(1,1);
		pDC->FillSolidRect(rc, clrBack);
	}
}

BOOL CXTPCommandBarsVisualStudio2012Theme::DrawRectangle(CDC* pDC, CXTPControl* pButton)
{
	if (pButton->GetID() == XTP_ID_CUSTOMIZE_ADDORREMOVE)
	{
		FillRect(pDC, pButton->GetRect(), FALSE, m_pColorSet->m_clrButtonBorderChecked,
			m_pColorSet->m_clrButtonBackgroundHighlighted);

		return TRUE;
	}

	if (pButton->GetPopuped())
	{
		if (pButton->GetID() == XTP_ID_TOOLBAR_EXPAND)
		{
			FillRect(pDC, pButton->GetRect(), FALSE, m_pColorSet->m_clrButtonBorderPopped,
				m_pColorSet->m_clrButtonBackgroundPopped);

			return TRUE;
		}
	}

	return FALSE;
}

void CXTPCommandBarsVisualStudio2012Theme::DrawRectangle(CDC* pDC, CRect rc, BOOL bSelected, BOOL bPressed, BOOL bEnabled, BOOL bChecked, BOOL bPopuped, XTPBarType barType, XTPBarPosition barPosition)
{
	if (bPopuped)
	{
		FillRect(pDC, rc, FALSE, m_pColorSet->m_clrMenuBorder,
			m_pColorSet->m_clrMenuBackground);
	}
	else if (barType == xtpBarTypePopup)
	{
		if (bChecked)
		{
			if (!bEnabled) // disabled
			{
				FillRect(pDC, rc, TRUE, m_pColorSet->m_clrCheckMarkDisabled,
					m_pColorSet->m_clrCheckMarkBackgroundDisabled);
			}
			else if (!bSelected && !bPressed) // normal state
			{
				FillRect(pDC, rc, TRUE, m_pColorSet->m_clrCheckMark,
					m_pColorSet->m_clrCheckMarkBackground);
			}
			else if (bSelected && !bPressed) // hover
			{
				FillRect(pDC, rc, TRUE, m_pColorSet->m_clrCheckMarkHighlighted,
					m_pColorSet->m_clrCheckMarkBackgroundHighlighted);
			}
			else if (bSelected && bPressed) // pressed
			{
				FillRect(pDC, rc, TRUE, m_pColorSet->m_clrCheckMarkPushed,
					m_pColorSet->m_clrCheckMarkBackgroundPushed);
			}
		}
		else
		{
			Base::DrawRectangle(pDC, rc, bSelected, bPressed, bEnabled, bChecked, bPopuped, barType, barPosition);
		}
	}
	else
	{
		if (bChecked)
		{
			if (!bSelected && !bPressed) // normal state
			{
				FillRect(pDC, rc, FALSE, m_pColorSet->m_clrButtonBorderChecked,
					m_pColorSet->m_clrButtonBackgroundChecked);
			}
			else if (bSelected && !bPressed) // hover
			{
				FillRect(pDC, rc, FALSE, m_pColorSet->m_clrButtonBorderChecked,
					m_pColorSet->m_clrButtonBackgroundHighlighted);
			}
			else if (bSelected && bPressed) // pressed
			{
				FillRect(pDC, rc, FALSE, m_pColorSet->m_clrButtonBorderPushed,
					m_pColorSet->m_clrButtonBackgroundPushed);
			}
			else
			{
				Base::DrawRectangle(pDC, rc, bSelected, bPressed, bEnabled, bChecked, bPopuped, barType, barPosition);
			}
		}
		else
		{
			if (barType == xtpControlSplitButtonPopup)
			{
				FillRect(pDC, rc, FALSE, RGB(255,0,0), RGB(255,0,0));
			}
			else if (bSelected && !bPressed) // hover
			{
				FillRect(pDC, rc, FALSE, m_pColorSet->m_clrButtonBorderHighlighted,
					m_pColorSet->m_clrButtonBackgroundHighlighted);
			}
			else if (bSelected && bPressed) // pressed
			{
				FillRect(pDC, rc, FALSE, m_pColorSet->m_clrButtonBorderPushed,
					m_pColorSet->m_clrButtonBackgroundPushed);
			}
			else
			{
				Base::DrawRectangle(pDC, rc, bSelected, bPressed, bEnabled, bChecked, bPopuped, barType, barPosition);
			}
		}
	}
}

void CXTPCommandBarsVisualStudio2012Theme::DrawDropDownGlyph(CDC* pDC, CXTPControl* pControl, CPoint pt, BOOL bSelected, BOOL bPopuped, BOOL bEnabled, BOOL bVert)
{
	UNREFERENCED_PARAMETER(bEnabled);
	UNREFERENCED_PARAMETER(bPopuped);
	UNREFERENCED_PARAMETER(bSelected);

	COLORREF clr = GetGlyphColor(pControl);

	if (pControl->GetPopuped())
	{
		if (pControl->GetType() == xtpControlSplitButtonPopup)
		{
			clr = m_pColorSet->m_clrGlyphPushed;
		}
		else
		{
			clr = m_pColorSet->m_clrGlyph;
		}
	}

	if (bVert)
	{
		Triangle(pDC, CPoint(pt.x + 2 , pt.y - 2), CPoint(pt.x + 2, pt.y + 2), CPoint (pt.x, pt.y), clr);
	}
	else
	{
		Triangle(pDC, CPoint(pt.x - 2 , pt.y - 1), CPoint(pt.x + 2, pt.y - 1), CPoint (pt.x, pt.y + 1), clr);
	}
}

void CXTPCommandBarsVisualStudio2012Theme::DrawPopupBarGripper(CDC *pDC, int xPos, int yPos, int cx, int cy, BOOL bExpanded)
{
	UNUSED_ALWAYS(pDC);
	UNUSED_ALWAYS(xPos);
	UNUSED_ALWAYS(yPos);
	UNUSED_ALWAYS(cx);
	UNUSED_ALWAYS(cy);
	UNUSED_ALWAYS(bExpanded);
}

CSize CXTPCommandBarsVisualStudio2012Theme::DrawCommandBarSeparator(CDC* pDC, CXTPCommandBar* pBar, CXTPControl* pControl, BOOL bDraw)
{
	if (!bDraw)
	{
		if (pBar->GetType() == xtpBarTypePopup)
			return CSize(3, 3);

		return CSize(6, 6);
	}

	CRect rc;
	pBar->GetClientRect(&rc);
	CRect rcControl = pControl->GetRect();
	CRect rcRow = pControl->GetRowRect();

	if (pBar->GetType() == xtpBarTypePopup)
	{
		if (!pControl->GetWrap())
		{
			HorizontalLine(pDC, rcControl.left - 1 + GetPopupBarGripperWidth(pBar) + m_nPopupBarTextPadding,
				rcControl.top - 2, rcControl.right + 1, m_pColorSet->m_clrSeparator);
			HorizontalLine(pDC, rcControl.left - 1 + GetPopupBarGripperWidth(pBar) + m_nPopupBarTextPadding,
				rcControl.top - 1, rcControl.right + 1, m_pColorSet->m_clrSeparatorHighlight);
		}
		else
		{
			VerticalLine(pDC, rcControl.left - 2, rcRow.top - 1, rcRow.bottom + 1, m_pColorSet->m_clrSeparator);
			VerticalLine(pDC, rcControl.left - 1, rcRow.top - 1, rcRow.bottom + 1, m_pColorSet->m_clrSeparatorHighlight);
		}
	}
	else if (pBar->GetPosition() != xtpBarPopup && IsVerticalPosition(pBar->GetPosition()))
	{
		if (!pControl->GetWrap())
		{
			HorizontalLine(pDC, rcRow.left, rcControl.top - 4, rcRow.right, m_pColorSet->m_clrSeparator);
			HorizontalLine(pDC, rcRow.left, rcControl.top - 3, rcRow.right, m_pColorSet->m_clrSeparatorHighlight);
		}
		else
		{
			VerticalLine(pDC, rcRow.right + 2, rcRow.top + 4, rcRow.bottom, m_pColorSet->m_clrSeparator);
			VerticalLine(pDC, rcRow.right + 3, rcRow.top + 4, rcRow.bottom, m_pColorSet->m_clrSeparatorHighlight);
		}
	}
	else
	{
		if (!pControl->GetWrap())
		{
			VerticalLine(pDC, rcControl.left - 4, rcRow.top + 2, rcRow.bottom, m_pColorSet->m_clrSeparator);
			VerticalLine(pDC, rcControl.left - 3, rcRow.top + 2, rcRow.bottom, m_pColorSet->m_clrSeparatorHighlight);
		}
		else
		{
			HorizontalLine(pDC, rcRow.left, rcRow.top - 4, rcRow.right, m_pColorSet->m_clrSeparator);
			HorizontalLine(pDC, rcRow.left, rcRow.top - 3, rcRow.right, m_pColorSet->m_clrSeparatorHighlight);
		}
	}
	return 0;
}

void CXTPCommandBarsVisualStudio2012Theme::DrawExpandGlyph(CDC* pDC, CXTPControl* pButton, CPoint pt0, CPoint pt1, CPoint pt2, COLORREF /*clr*/)
{
	if (pButton->GetPopuped())
	{
		HorizontalLine(pDC, pt2.x-2, pt0.y-3, pt1.x+1, m_pColorSet->m_clrGlyphPushed);
		Triangle(pDC, pt0, pt1, pt2, m_pColorSet->m_clrGlyphPushed);
	}
	else
	{
		if (pButton->GetSelected())
		{
			HorizontalLine(pDC, pt2.x-2, pt0.y-3, pt1.x+1, m_pColorSet->m_clrGlyphHighlighted);
			Triangle(pDC, pt0, pt1, pt2, m_pColorSet->m_clrGlyphHighlighted);
		}
		else
		{
			HorizontalLine(pDC, pt2.x-2, pt0.y-3, pt1.x+1, m_pColorSet->m_clrGlyph);
			Triangle(pDC, pt0, pt1, pt2, m_pColorSet->m_clrGlyph);
		}
	}
}

void CXTPCommandBarsVisualStudio2012Theme::DrawHiddenGlyph(CDC* pDC, CXTPControl* pButton, COLORREF /*clr*/, BOOL bVertical)
{
	if (pButton->GetPopuped())
	{
		CXTPPaintManager::DrawHiddenGlyph(pDC, pButton,
			m_pColorSet->m_clrGlyphPushed, bVertical);
	}
	else
	{
		if (pButton->GetSelected())
		{
			CXTPPaintManager::DrawHiddenGlyph(pDC, pButton,
				m_pColorSet->m_clrGlyphHighlighted, bVertical);
		}
		else
		{
			CXTPPaintManager::DrawHiddenGlyph(pDC, pButton,
				m_pColorSet->m_clrGlyph, bVertical);
		}
	}
}

void CXTPCommandBarsVisualStudio2012Theme::DrawCheckMark(CDC* pDC, CRect rcCheck, BOOL bIsRadio, BOOL bEnabled, BOOL bSelected)
{
	rcCheck.DeflateRect(4,1,0,1);

	COLORREF clr = COLORREF_NULL;

	if (!bEnabled)
	{
		clr = m_pColorSet->m_clrCheckMarkDisabled;
	}
	else if (bSelected)
	{
		clr = m_pColorSet->m_clrCheckMarkHighlighted;
	}
	else
	{
		clr = m_pColorSet->m_clrCheckMark;
	}

	if (bIsRadio)
	{
		DrawPopupBarRadioMark(pDC, rcCheck, bEnabled, clr);
	}
	else
	{
		DrawPopupBarCheckMark(pDC, rcCheck, bEnabled, clr);
	}
}

void CXTPCommandBarsVisualStudio2012Theme::DrawSplitButtonFrame(CDC* pDC, CXTPControl* pButton, CRect rcButton)
{
	if (pButton->GetStyle() == xtpButtonIconAndCaptionBelow)
	{
		Base::DrawSplitButtonFrame(pDC, pButton, rcButton);
		return;
	}

	CPoint pt = CPoint(rcButton.right - 6, rcButton.CenterPoint().y);

	BOOL bEnabled  = pButton->GetEnabled();
	BOOL bSelected = pButton->GetSelected();
	BOOL bPopuped  = pButton->GetPopuped();

	if (bEnabled == TRUE_SPLITCOMMAND)
	{
		bEnabled = FALSE;
	}

	if (bEnabled)
	{
		if (bPopuped)
		{
			pDC->FillSolidRect(rcButton,
				m_pColorSet->m_clrButtonBackgroundPopped);
		}
		else if (pButton->GetSelected() || pButton->GetPressed())
		{
			pDC->FillSolidRect(rcButton,
				m_pColorSet->m_clrButtonBackgroundHighlighted);

			VerticalLine(pDC, rcButton.right - m_nSplitButtonDropDownWidth, rcButton.top, rcButton.bottom,
				m_pColorSet->m_clrSeparator);

			if (pButton->GetPressed())
			{
				rcButton.right -= m_nSplitButtonDropDownWidth;

				pDC->FillSolidRect(rcButton,
					m_pColorSet->m_clrButtonBackgroundPushed);
			}
		}
	}

	DrawDropDownGlyph(pDC, pButton, pt, bSelected, bPopuped, bEnabled, FALSE);
}
