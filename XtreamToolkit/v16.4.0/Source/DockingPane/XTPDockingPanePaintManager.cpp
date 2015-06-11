// XTPDockingPanePaintManager.cpp : implementation of the CXTPDockingPanePaintManager class.
//
// This file is a part of the XTREME DOCKINGPANE MFC class library.
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

#include "stdafx.h"
#include "Resource.h"

#include "Common/XTPVC80Helpers.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPResourceImage.h"
#include "Common/XTPResourceManager.h"
#include "Common/XTPWinThemeWrapper.h"
#include "Common/XTPImageManager.h"
#include "Common/XTPSystemHelpers.h"

#include <TabManager/Includes.h>

#include "XTPDockingPaneDefines.h"
#include "XTPDockingPaneBase.h"
#include "XTPDockingPaneBaseContainer.h"
#include "XTPDockingPanePaintManager.h"
#include "XTPDockingPaneTabbedContainer.h"
#include "XTPDockingPane.h"
#include "XTPDockingPaneMiniWnd.h"
#include "XTPDockingPaneSplitterContainer.h"
#include "XTPDockingPaneSidePanel.h"
#include <DockingPane/XTPDockingPaneCaptionButton.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


void XTPDeflateRectDirection(CRect& rc, int left, int top, int right, int bottom, BOOL bVertical)
{
	if (bVertical)
	{
		rc.DeflateRect(top, left, bottom, right);
	}
	else
	{
		rc.DeflateRect(left, top, right, bottom);
	}
}

int XTPGetRectDirectionHeight(const CRect& rc, BOOL bVertical)
{
	return bVertical ? rc.Width() : rc.Height();
}

void XTPFillFramePartRect(CDC* pDC, CRect rc, CWnd* pWnd, CWnd* pParentFrame, const CXTPPaintManagerColorGradient& clrHeaderFace, BOOL bHoriz)
{
	COLORREF clrLight = clrHeaderFace.clrDark, clrDark = clrHeaderFace.clrLight;

	CWnd* pFrame = pParentFrame;

	if (XTPColorManager()->IsLowResolution())
	{
		pDC->FillSolidRect(rc, clrDark);
		return;
	}

	if (pFrame)
	{
		CRect rcFrame;
		pFrame->GetWindowRect(&rcFrame);
		pWnd->ScreenToClient(&rcFrame);

		if (bHoriz)
		{
			float fWidth = (float)max(rcFrame.Width(), GetSystemMetrics(SM_CXFULLSCREEN)/ 2);

			COLORREF clr1 = XTPDrawHelpers()->BlendColors(clrLight, clrDark, float(rc.left - rcFrame.left) / fWidth);
			COLORREF clr2 = XTPDrawHelpers()->BlendColors(clrLight, clrDark, float(rc.right - rcFrame.left) / fWidth);

			XTPDrawHelpers()->GradientFill(pDC, rc, clr1, clr2, TRUE);
		}
		else
		{
			float fHeight = (float)rcFrame.Height();

			COLORREF clr1 = XTPDrawHelpers()->BlendColors(clrLight, clrDark, float(rc.top - rcFrame.top) / fHeight);
			COLORREF clr2 = XTPDrawHelpers()->BlendColors(clrLight, clrDark, float(rc.bottom - rcFrame.top) / fHeight);

			XTPDrawHelpers()->GradientFill(pDC, rc, clr1, clr2, FALSE);
		}
	}
}


///////////////////////////////////////////////////////////////////////////////
// CXTPDockingPanePaintManager


CXTPDockingPanePaintManager::CXTPDockingPanePaintManager()
	: m_bUseOfficeFont(FALSE)
	, m_strOfficeFont(_T("Tahoma"))
	, m_bDestroyCustomTitleBrushes(FALSE)
	, m_hbrActiveTitleHorz(NULL)
	, m_hbrInactiveTitleHorz(NULL)
	, m_hbrActiveTitleVert(NULL)
	, m_hbrInactiveTitleVert(NULL)
{
	m_themeToolbar = new CXTPWinThemeWrapper();
	m_themeExplorer = new CXTPWinThemeWrapper();
	m_themeButton = new CXTPWinThemeWrapper();
	m_themeWindow = new CXTPWinThemeWrapper();
	m_themeRebar = new CXTPWinThemeWrapper();

	m_pTabPaintManager = new CXTPTabPaintManager();
	m_pTabPaintManager->SetLayout(xtpTabLayoutSizeToFit);
	m_pTabPaintManager->SetPosition(xtpTabPositionBottom);
	m_pTabPaintManager->m_clientFrame = xtpTabFrameSingleLine;
	m_pTabPaintManager->m_bBoldSelected = FALSE;
	m_pTabPaintManager->EnableToolTips();

	m_pPanelPaintManager = new CXTPTabPaintManager();
	m_pPanelPaintManager->SetLayout(xtpTabLayoutCompressed);
	m_pPanelPaintManager->m_clientFrame = xtpTabFrameNone;
	m_pPanelPaintManager->m_bBoldSelected = FALSE;
	m_pPanelPaintManager->m_bInvertGradient = TRUE;
	m_pPanelPaintManager->m_bFillBackground = FALSE;

	m_bAutoHidePanelHighlightSelected = TRUE;

	m_bDrawSingleTab = FALSE;
	m_nSplitterSize = 4;
	m_nSplitterIndent = 0;
	m_bDrawCaptionIcon = FALSE;

	m_rcCaptionMargin.SetRect(0, 2, 0, 1);
	m_nCaptionFontGap = 5;

	m_bShowCaption = TRUE;
	m_bUseStandardFont = TRUE;

	m_bHighlightActiveCaption = TRUE;
	m_systemTheme = xtpSystemThemeDefault;
	m_bUseBoldCaptionFont = TRUE;
	m_dwSplitterStyle = xtpPaneSplitterFlat;

	m_nCaptionButtonStyle = xtpPaneCaptionButtonDefault;
	m_nTitleHeight = 0;
	m_nTabsHeight = 0;
	m_bLunaTheme = FALSE;

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();

	m_bAxUseCustomTitleColors = FALSE;
	m_clrAxTitleActiveHorz = 0;
	m_clrAxTitleInactiveHorz = 0;
	m_clrAxTitleActiveVert = 0;
	m_clrAxTitleInactiveVert = 0;

#endif
}

CXTPDockingPanePaintManager::~CXTPDockingPanePaintManager()
{
	SetCustomTitleBrushes(NULL);

	m_pTabPaintManager->InternalRelease();
	m_pPanelPaintManager->InternalRelease();

	SAFE_DELETE(m_themeToolbar);
	SAFE_DELETE(m_themeExplorer);
	SAFE_DELETE(m_themeButton);
	SAFE_DELETE(m_themeWindow);
	SAFE_DELETE(m_themeRebar);
}

DWORD CXTPDockingPanePaintManager::GetSplitterStyle() const
{
	return m_dwSplitterStyle;
}

void CXTPDockingPanePaintManager::SetSplitterStyle(DWORD dwStyle)
{
	m_dwSplitterStyle = dwStyle;

	m_nSplitterSize = dwStyle == xtpPaneSplitterFlat ? 4 : 7;

}

XTPCurrentSystemTheme CXTPDockingPanePaintManager::GetCurrentSystemTheme() const
{
	if (m_systemTheme == xtpSystemThemeDefault)
		return XTPColorManager()->IsLunaColorsDisabled() ? xtpSystemThemeUnknown :
			XTPColorManager()->GetCurrentSystemTheme();

	if (m_systemTheme == xtpSystemThemeAuto)
		return XTPColorManager()->GetWinThemeWrapperTheme();

	return m_systemTheme;
}

void CXTPDockingPanePaintManager::SetLunaTheme(XTPCurrentSystemTheme systemTheme)
{
	m_systemTheme = systemTheme;
	RefreshMetrics();

}

COLORREF CXTPDockingPanePaintManager::GetXtremeColor(UINT nIndex)
{
	if (nIndex > XPCOLOR_LAST) return nIndex;
	return m_arrColor[nIndex];
}

void CXTPDockingPanePaintManager::SetColors(int cElements, CONST INT* lpaElements, CONST COLORREF* lpaRgbValues)
{
	for (int i = 0; i < cElements; i++)
		m_arrColor[lpaElements[i]] = lpaRgbValues[i];
}

CXTPTabPaintManager* CXTPDockingPanePaintManager::SetTabPaintManager(CXTPTabPaintManager* pManager)
{
	m_pTabPaintManager->InternalRelease();
	m_pTabPaintManager = pManager;

	m_pTabPaintManager->SetLayout(xtpTabLayoutSizeToFit);
	m_pTabPaintManager->SetPosition(xtpTabPositionBottom);
	m_pTabPaintManager->m_bBoldSelected = FALSE;
	m_pTabPaintManager->EnableToolTips();

	pManager->RefreshMetrics();
	return pManager;
}

CXTPTabPaintManager* CXTPDockingPanePaintManager::SetPanelPaintManager(CXTPTabPaintManager* pManager)
{
	delete m_pPanelPaintManager;
	m_pPanelPaintManager = pManager;

	m_pPanelPaintManager->SetLayout(xtpTabLayoutCompressed);
	m_pPanelPaintManager->m_clientFrame = xtpTabFrameNone;
	m_pPanelPaintManager->m_bBoldSelected = FALSE;
	m_pPanelPaintManager->m_bInvertGradient = TRUE;
	m_pPanelPaintManager->m_bFillBackground = FALSE;


	pManager->RefreshMetrics();
	return pManager;
}


void CXTPDockingPanePaintManager::UseOfficeFont(BOOL bUseOfficeFont)
{
	m_bUseOfficeFont = bUseOfficeFont;
	RefreshMetrics();
}

void CXTPDockingPanePaintManager::SetCaptionFontIndirect(LOGFONT* pLogFont, BOOL bUseStandardFont /*= FALSE*/)
{
	m_bUseStandardFont = bUseStandardFont;

	if (!pLogFont)
		return;

	BOOL bUseSystemFont = (pLogFont->lfCharSet > SYMBOL_CHARSET);
	if (m_bUseOfficeFont && !bUseSystemFont && bUseStandardFont && XTPDrawHelpers()->FontExists(_T("Tahoma")))
	{
		STRCPY_S(pLogFont->lfFaceName, LF_FACESIZE, _T("Tahoma"));
	}

	m_fntTitle.DeleteObject();
	VERIFY(m_fntTitle.CreateFontIndirect(pLogFont));

	pLogFont->lfOrientation = 900;
	pLogFont->lfEscapement = 2700;
	pLogFont->lfHeight = pLogFont->lfHeight < 0 ? __min(-11, pLogFont->lfHeight) : pLogFont->lfHeight;
	STRCPY_S(pLogFont->lfFaceName, LF_FACESIZE, CXTPDrawHelpers::GetVerticalFontName(m_bUseOfficeFont));

	m_fntTitleVertical.DeleteObject();
	m_fntTitleVertical.CreateFontIndirect(pLogFont);


	CWindowDC dc(CWnd::GetDesktopWindow());
	CXTPFontDC font(&dc, &m_fntTitle);
	m_nTitleHeight = max(13, dc.GetTextExtent(_T(" "), 1).cy) + m_nCaptionFontGap;
}

void CXTPDockingPanePaintManager::RefreshMetrics()
{
	RefreshXtremeColors(FALSE);

	CXTPColorManager* pColorManager = XTPColorManager();

	for (int i = 0; i <= XPCOLOR_LAST; i++)
		m_arrColor[i] = pColorManager->GetColor(i);

	m_clrBackground.SetStandardValue(GetSysColor(COLOR_3DFACE));
	m_clrSplitterGripper.SetStandardValue(m_arrColor[COLOR_3DSHADOW]);
	m_clrSplitter.SetStandardValue(m_arrColor[COLOR_3DFACE]);
	m_clrSplitterGradient.SetStandardValue(GetSysColor(COLOR_3DFACE), XTPColorManager()->LightColor(GetSysColor(COLOR_3DFACE), GetSysColor(COLOR_WINDOW), 0xcd));
	m_clrDisabledCaptionText.SetStandardValue(m_arrColor[COLOR_GRAYTEXT]);

	if (m_bUseStandardFont)
	{
		NONCLIENTMETRICS ncm =
		{
			sizeof(NONCLIENTMETRICS)
		};
		VERIFY(::SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICS), &ncm, 0));
		if (!m_bUseBoldCaptionFont)
			ncm.lfSmCaptionFont.lfWeight = FW_NORMAL;
		ncm.lfSmCaptionFont.lfCharSet = XTPResourceManager()->GetFontCharset();

		SetCaptionFontIndirect(&ncm.lfSmCaptionFont, TRUE);
	}

	m_pTabPaintManager->RefreshMetrics();
	m_pPanelPaintManager->RefreshMetrics();

	CWindowDC dc(CWnd::GetDesktopWindow());
	CXTPFontDC font(&dc, m_pTabPaintManager->GetFont(FALSE));
	m_nTabsHeight = max(24, dc.GetTextExtent(_T(" "), 1).cy + 10);

	m_themeExplorer->OpenTheme(0, L"EXPLORERBAR");
	m_themeButton->OpenTheme(0, L"BUTTON");
	m_themeToolbar->OpenThemeData(0, L"TOOLBAR");
	m_themeWindow->OpenThemeData(0, L"WINDOW");
	m_themeRebar->OpenThemeData(0, L"REBAR");

	m_bLunaTheme = FALSE;
}

void CXTPDockingPanePaintManager::Line(CDC* pDC, int x0, int y0, int x1, int y1, int nPen)
{
	CXTPPenDC pen(*pDC, GetXtremeColor(nPen));
	pDC->MoveTo(x0, y0);
	pDC->LineTo(x1, y1);
}

void CXTPDockingPanePaintManager::Line(CDC* pDC, int x0, int y0, int x1, int y1)
{
	pDC->MoveTo(x0, y0);
	pDC->LineTo(x1, y1);
}


void CXTPDockingPanePaintManager::Rectangle(CDC* pDC, CRect rc, int nBrush, int nPen)
{
	pDC->FillSolidRect(rc, GetXtremeColor(nBrush));
	pDC->Draw3dRect(rc, GetXtremeColor(nPen), GetXtremeColor(nPen));
}

void CXTPDockingPanePaintManager::Pixel(CDC* pDC, int x, int y, int nPen)
{
	pDC->SetPixel(x, y, GetXtremeColor(nPen));
}

CRect CXTPDockingPanePaintManager::GetCaptionGripperRect(const CXTPDockingPaneTabbedContainer* /*pPane*/)
{
	return CRect(0, 0, 0, 0);
}


void CXTPDockingPanePaintManager::DrawCaptionButtons(CDC* pDC, CXTPDockingPaneCaptionButtons* pButtons, CRect& rcCaption, COLORREF clr, int nButtonSize, int nButtonGap, BOOL bVertical)
{
	if (!pButtons)
		return;

	CRect rcButton(
		bVertical ? CPoint((rcCaption.left + rcCaption.right - nButtonSize) / 2, rcCaption.bottom - nButtonSize) :
			CPoint(rcCaption.right - nButtonSize , (rcCaption.top + rcCaption.bottom - nButtonSize) / 2),
		CSize(nButtonSize, nButtonSize));

	for (int i = 0; i < pButtons->GetSize(); i++)
	{
		CXTPDockingPaneCaptionButton* pButton = pButtons->GetAt(i);

		if (i == 0)
		{
			CXTPDockingPane* pPane = GetSelectedPane(pButton->GetPane());

			if (pPane && ((pPane->GetEnabled() & xtpPaneEnableActions) == 0))
				clr = m_clrDisabledCaptionText;
		}

		if (pButton->IsVisible())
		{
			pButton->SetRect(rcButton);
			DrawCaptionButton(pDC, pButton, clr);

			if (bVertical)
			{
				rcButton.OffsetRect(0, - nButtonSize - nButtonGap);
			}
			else
			{
				rcButton.OffsetRect(- nButtonSize - nButtonGap, 0);
			}
		}
		else
		{
			pButton->SetRect(CXTPEmptyRect());
		}
	}
	if (bVertical)
	{
		rcCaption.bottom = rcButton.bottom;
	}
	else
	{
		rcCaption.right = rcButton.right;
	}
}

void CXTPDockingPanePaintManager::DrawCaptionButton(CDC* pDC, CXTPDockingPaneCaptionButton* pButton, COLORREF clrButton)
{
	CRect rc = pButton->GetRect();
	CPoint pt = rc.CenterPoint();

	XTPDockingPaneCaptionButtonStyle nCaptionButtonStyle = m_nCaptionButtonStyle;

	if (nCaptionButtonStyle == xtpPaneCaptionButtonThemedToolBar)
	{
		if (!m_themeToolbar->IsAppThemed())
		{
			nCaptionButtonStyle = xtpPaneCaptionButtonOffice;
		}
		else
		{
			if (pButton->IsPressed())
			{
				m_themeToolbar->DrawThemeBackground(pDC->GetSafeHdc(), TP_BUTTON, TS_PRESSED, &rc, 0);
			}
			else if (pButton->IsSelected())
			{
				m_themeToolbar->DrawThemeBackground(pDC->GetSafeHdc(), TP_BUTTON, TS_HOT, &rc, 0);
			}
		}
	}

	if (nCaptionButtonStyle == xtpPaneCaptionButtonThemedToolWindow)
	{
		if (!m_themeWindow->IsAppThemed() || pButton->GetID() != XTP_IDS_DOCKINGPANE_CLOSE )
		{
			nCaptionButtonStyle = xtpPaneCaptionButtonOffice;
		}
		else
		{
			BOOL bActive = TRUE;
			if (pButton->GetPane()->GetType() == xtpPaneTypeTabbedContainer)
				bActive = ((CXTPDockingPaneTabbedContainer*)pButton->GetPane())->IsActive();
			if (pButton->GetPane()->GetType() == xtpPaneTypeMiniWnd)
				bActive = ((CXTPDockingPaneMiniWnd*)pButton->GetPane())->IsActive();

			int nState = pButton->IsPressed() ? CBS_PUSHED: pButton->IsSelected() ? CBS_HOT : !bActive ? 5 : CBS_NORMAL;
			m_themeWindow->DrawThemeBackground(pDC->GetSafeHdc(), WP_SMALLCLOSEBUTTON, nState, &rc, 0);
			return;
		}
	}

	if (nCaptionButtonStyle == xtpPaneCaptionButtonThemedExplorerBar)
	{
		if (!m_themeExplorer->IsAppThemed())
		{
			nCaptionButtonStyle = xtpPaneCaptionButtonOffice;
		}
		else
		{
			if (pButton->GetID() == XTP_IDS_DOCKINGPANE_CLOSE || pButton->GetID() == XTP_IDS_DOCKINGPANE_AUTOHIDE)
			{

				BOOL bActive = TRUE;
				if (pButton->GetPane()->GetType() == xtpPaneTypeTabbedContainer)
					bActive = ((CXTPDockingPaneTabbedContainer*)pButton->GetPane())->IsActive();
				if (pButton->GetPane()->GetType() == xtpPaneTypeMiniWnd)
					bActive = ((CXTPDockingPaneMiniWnd*)pButton->GetPane())->IsActive();

				bActive = m_bHighlightActiveCaption && bActive;

				if (pButton->GetID() == XTP_IDS_DOCKINGPANE_CLOSE)
				{
					int nState = pButton->IsPressed() ? EBHC_PRESSED: bActive || pButton->IsSelected() ? EBHC_HOT : EBHC_NORMAL;
					if (SUCCEEDED(m_themeExplorer->DrawThemeBackground(*pDC, EBP_HEADERCLOSE, nState, pButton->GetRect(), 0)))
						return;
				}
				else
				{
					int nState = pButton->IsPressed() ? EBHP_PRESSED: pButton->IsSelected() ? EBHP_HOT : EBHP_NORMAL;
					if (SUCCEEDED(m_themeExplorer->DrawThemeBackground(*pDC, EBP_HEADERPIN, (pButton->GetState() & xtpPanePinPushed ? 0 : 3) + nState, pButton->GetRect(), 0)))
						return;
				}
			}

			nCaptionButtonStyle = xtpPaneCaptionButtonThemedButton;
		}
	}

	if (nCaptionButtonStyle == xtpPaneCaptionButtonThemedButton)
	{
		if (!m_themeButton->IsAppThemed())
		{
			nCaptionButtonStyle = xtpPaneCaptionButtonOffice;
		}
		else
		{
			int nState = pButton->IsPressed() ? PBS_PRESSED: pButton->IsSelected() ? PBS_HOT: PBS_NORMAL;
			if (nState != PBS_NORMAL)
			{
				m_themeButton->DrawThemeBackground(*pDC, BP_PUSHBUTTON, nState, pButton->GetRect(), 0);
				m_themeButton->GetThemeColor(BP_PUSHBUTTON, nState, TMT_TEXTCOLOR, &clrButton);
			}
		}
	}

	if (nCaptionButtonStyle == xtpPaneCaptionButtonDefault)
	{
		pDC->DrawFrameControl(&rc, DFC_BUTTON, DFCS_ADJUSTRECT | DFCS_BUTTONPUSH | (pButton->IsPressed() ? DFCS_PUSHED : 0));

		if (pButton->IsPressed()) pt.Offset(1, 1);
	}

	if (nCaptionButtonStyle == xtpPaneCaptionButtonOffice)
	{
		if (pButton->IsPressed())
		{
			pDC->Draw3dRect(rc, GetXtremeColor(COLOR_BTNTEXT), GetXtremeColor(COLOR_BTNHILIGHT));
			pt.Offset(1, 1);
		}
		else if (pButton->IsSelected())
			pDC->Draw3dRect(rc, GetXtremeColor(COLOR_BTNHILIGHT), GetXtremeColor(COLOR_BTNTEXT));
	}

	if (nCaptionButtonStyle == xtpPaneCaptionButtonOffice2003)
	{
		if (pButton->IsSelected())
		{
			if (!m_bLunaTheme)
			{
				Rectangle(pDC, rc, XPCOLOR_HIGHLIGHT, XPCOLOR_HIGHLIGHT_BORDER);
				clrButton = GetXtremeColor(XPCOLOR_HIGHLIGHT_TEXT);
			}
			else
			{
				XTPDrawHelpers()->GradientFill(pDC, rc, RGB(255, 242, 200), RGB(255, 212, 151), FALSE);
				pDC->Draw3dRect(rc, GetXtremeColor(XPCOLOR_HIGHLIGHT_BORDER), GetXtremeColor(XPCOLOR_HIGHLIGHT_BORDER));
				clrButton = 0;
			}
		}
		if (pButton->IsPressed())
		{
			if (!m_bLunaTheme)
			{
				Rectangle(pDC, rc, XPCOLOR_HIGHLIGHT_PUSHED, XPCOLOR_HIGHLIGHT_BORDER);
				clrButton = GetXtremeColor(XPCOLOR_PUSHED_TEXT);
			}
			else
			{
				XTPDrawHelpers()->GradientFill(pDC, rc, RGB(254, 142, 75), RGB(255, 207, 139), FALSE);
				pDC->Draw3dRect(rc, GetXtremeColor(XPCOLOR_HIGHLIGHT_BORDER), GetXtremeColor(XPCOLOR_HIGHLIGHT_BORDER));
				clrButton = 0;
			}
		}
	}

	DrawCaptionButtonEntry(pDC, pButton, pt, clrButton);
}



void CXTPDockingPanePaintManager::DrawPane(CDC* pDC, CXTPDockingPaneTabbedContainer* pPane, CRect rc)
{
	pDC->FillSolidRect(rc, GetXtremeColor(COLOR_3DFACE));
	pDC->SetBkMode(TRANSPARENT);

	CRect rcCaption(rc);

	if (pPane->IsTabsVisible())
	{
		AdjustCaptionRect(pPane, rc);
		m_pTabPaintManager->DrawTabControl(pPane, pDC, rc);
	}

	if (pPane->IsTitleVisible())
	{
		DrawCaption(pDC, pPane, rcCaption);
	}

}

void CXTPDockingPanePaintManager::GetCaptionRect(const CXTPDockingPaneTabbedContainer* pPane, CRect& rc)
{
	if (pPane->IsTitleVisible())
	{
		GetClientRect(pPane->GetSafeHwnd(), rc);

		if (pPane->IsCaptionVertical())
		{
			rc.right = rc.left + m_nTitleHeight + m_rcCaptionMargin.top + m_rcCaptionMargin.bottom;
		}
		else
		{
			rc.bottom = rc.top + m_nTitleHeight + m_rcCaptionMargin.top + m_rcCaptionMargin.bottom;
		}
	}
	else
	{
		rc.SetRectEmpty();
	}
}

void CXTPDockingPanePaintManager::AdjustCaptionRect(const CXTPDockingPaneTabbedContainer* pPane, CRect& rc)
{
	if (pPane->IsTitleVisible())
	{
		if (pPane->IsCaptionVertical())
		{
			rc.left += m_nTitleHeight + m_rcCaptionMargin.top + m_rcCaptionMargin.bottom;
		}
		else
		{
			rc.top += m_nTitleHeight + m_rcCaptionMargin.top + m_rcCaptionMargin.bottom;
		}
	}
}

void CXTPDockingPanePaintManager::AdjustClientRect(CXTPDockingPaneTabbedContainer* pPane, CRect& rect, BOOL bApply)
{
	CPoint pt = rect.TopLeft();
	AdjustCaptionRect(pPane, rect);


	if (pPane->IsTabsVisible())
	{
		if (bApply)
		{
			CClientDC dc(pPane);

			CRect rcTabs(rect);
			rcTabs.OffsetRect(-pt);

			m_pTabPaintManager->RepositionTabControl(pPane, &dc, rcTabs);
		}
		m_pTabPaintManager->AdjustClientRect(pPane, rect);
	}
}

void CXTPDockingPanePaintManager::AdjustClientRect(CXTPDockingPaneSidePanel* pPane, CRect& rect)
{
	rect.DeflateRect(3, 3);
	AdjustCaptionRect(pPane, rect);
}

void CXTPDockingPanePaintManager::AdjustCaptionRect(const CXTPDockingPaneSidePanel* pPane, CRect& rect)
{
	int nTitleFont = GetCaptionHeight();
	XTPDockingPaneDirection direction = pPane->GetDirection();

	if (direction == xtpPaneDockLeft) rect.left += nTitleFont;
	else if (direction == xtpPaneDockRight) rect.right -= nTitleFont;
	else if (direction == xtpPaneDockTop) rect.top += nTitleFont;
	else if (direction == xtpPaneDockBottom) rect.bottom -= nTitleFont;
}

CRect CXTPDockingPanePaintManager::GetPaneCaptionRect(const CXTPDockingPaneSidePanel *pPane)
{
	CXTPClientRect rect(pPane);
	rect.DeflateRect(1, 1);

	int nCaptionHeight = GetCaptionHeight();
	XTPDockingPaneDirection direction = pPane->GetDirection();

		 if (xtpPaneDockLeft   == direction) rect.right  = rect.left   + nCaptionHeight;
	else if (xtpPaneDockRight  == direction) rect.left   = rect.right  - nCaptionHeight;
	else if (xtpPaneDockTop    == direction) rect.bottom = rect.top    + nCaptionHeight;
	else if (xtpPaneDockBottom == direction) rect.top    = rect.bottom - nCaptionHeight;

	return rect;
}


void CXTPDockingPanePaintManager::DrawSplitter(CDC* pDC, CXTPDockingPaneSplitterWnd* pSplitter)
{
	CXTPClientRect rc(pSplitter);

	if (m_dwSplitterStyle & xtpPaneSplitter3D)
	{
		pDC->FillSolidRect(rc, m_clrSplitter);

		if (pSplitter->IsHorizontal())
		{
			pDC->FillSolidRect(0, 0, rc.Width(), 1, GetXtremeColor(COLOR_3DHIGHLIGHT));
			pDC->FillSolidRect(0, rc.bottom - 2, rc.Width(), 1, GetXtremeColor(XPCOLOR_3DSHADOW));
			pDC->FillSolidRect(0, rc.bottom - 1, rc.Width(), 1, GetXtremeColor(COLOR_BTNTEXT));
		}
		else
		{
			pDC->FillSolidRect(0, 0, 1, rc.Height(), GetXtremeColor(COLOR_3DHIGHLIGHT));
			pDC->FillSolidRect(rc.right - 2, 0, 1, rc.Height(), GetXtremeColor(XPCOLOR_3DSHADOW));
			pDC->FillSolidRect(rc.right - 1, 0, 1, rc.Height(), GetXtremeColor(COLOR_BTNTEXT));
		}
	}
	else if (m_dwSplitterStyle & xtpPaneSplitterSoft3D)
	{
		int nCenter = m_nSplitterSize / 2;

		if (pSplitter->IsHorizontal())
		{
			pDC->FillSolidRect(0, 0, rc.Width(), 1, GetXtremeColor(COLOR_3DDKSHADOW));
			pDC->FillSolidRect(0, rc.bottom - 1, rc.Width(), 1, GetXtremeColor(COLOR_3DDKSHADOW));

			pDC->FillSolidRect(0, 1, rc.Width(), nCenter - 1, GetXtremeColor(COLOR_WINDOW));
			pDC->FillSolidRect(0, nCenter, rc.Width(), rc.Height() - nCenter - 1, m_clrSplitter);
		}
		else
		{
			pDC->FillSolidRect(0, 0, 1, rc.Height(), GetXtremeColor(COLOR_3DDKSHADOW));
			pDC->FillSolidRect(rc.right - 1, 0, 1, rc.Height(), GetXtremeColor(COLOR_3DDKSHADOW));

			pDC->FillSolidRect(1, 0, nCenter - 1, rc.Height(), GetXtremeColor(COLOR_WINDOW));
			pDC->FillSolidRect(nCenter, 0, rc.Width() - nCenter - 1, rc.Height(), m_clrSplitter);
		}
	}
	else if (m_dwSplitterStyle & xtpPaneSplitterGradient)
	{
		XTPFillFramePartRect(pDC, rc, (CWnd*)pSplitter, pSplitter->GetParent(), m_clrSplitterGradient, TRUE);
	}
	else if (m_dwSplitterStyle & xtpPaneSplitterGradientVert)
	{
		XTPFillFramePartRect(pDC, rc, (CWnd*)pSplitter, pSplitter->GetParent(), m_clrSplitterGradient, FALSE);
	}
	else
	{
		pDC->FillSolidRect(rc, m_clrSplitter);
	}

	if (m_dwSplitterStyle & xtpPaneSplitterGripperOfficeXP)
	{
		if (pSplitter->IsHorizontal())
		{
			pDC->FillSolidRect(rc.CenterPoint().x, 0 + 2, 11, 1, m_clrSplitterGripper);
			pDC->FillSolidRect(rc.CenterPoint().x, 0 + 4, 11, 1, m_clrSplitterGripper);
		}
		else
		{
			pDC->FillSolidRect(2, rc.CenterPoint().y, 1, 11, m_clrSplitterGripper);
			pDC->FillSolidRect(4, rc.CenterPoint().y, 1, 11, m_clrSplitterGripper);
		}
	}
	if (m_dwSplitterStyle & xtpPaneSplitterGripperOffice2003)
	{
		if (pSplitter->IsHorizontal())
		{
			int nLeft = rc.CenterPoint().x - min(32, rc.Width() /2 - 10);
			int nRight = rc.CenterPoint().x + min(32, rc.Width() /2 - 10);
			int y = rc.CenterPoint().y - 1;

			for (int x = nLeft; x < nRight; x += 4)
			{
				pDC->FillSolidRect(CRect(x + 1, y + 1, x + 3, y + 3), GetXtremeColor(COLOR_BTNHIGHLIGHT));
				pDC->FillSolidRect(CRect(x, y, x + 2, y + 2), m_clrSplitterGripper);
			}
		}
		else
		{
			int nTop = rc.CenterPoint().y - min(32, rc.Height() /2 - 10);
			int nBottom = rc.CenterPoint().y + min(32, rc.Height() /2 - 10);
			int x = rc.CenterPoint().x - 1;

			for (int y = nTop; y < nBottom; y += 4)
			{
				pDC->FillSolidRect(CRect(x + 1, y + 1, x + 3, y + 3), GetXtremeColor(COLOR_BTNHIGHLIGHT));
				pDC->FillSolidRect(CRect(x, y, x + 2, y + 2), m_clrSplitterGripper);
			}
		}

	}
}


void CXTPDockingPanePaintManager::DrawCaptionButtonEntry(CDC* pDC, CXTPDockingPaneCaptionButton* pButton, CPoint pt, COLORREF clrButton)
{
	CXTPPenDC pen(*pDC, clrButton);

	if (pButton->GetID() == XTP_IDS_DOCKINGPANE_CLOSE)
	{
		pDC->MoveTo(pt.x - 4, pt.y - 3 - 1); pDC->LineTo(pt.x + 3 , pt.y + 4 - 1);
		pDC->MoveTo(pt.x - 4, pt.y + 3 - 1); pDC->LineTo(pt.x + 3 , pt.y - 4 - 1);

		pDC->MoveTo(pt.x - 3, pt.y - 3 - 1); pDC->LineTo(pt.x + 4 , pt.y + 4 - 1);
		pDC->MoveTo(pt.x - 3, pt.y + 3 - 1); pDC->LineTo(pt.x + 4 , pt.y - 4 - 1);

	}
	else if (pButton->GetID() == XTP_IDS_DOCKINGPANE_AUTOHIDE)
	{
		CXTPDockingPaneCaptionButton::DrawPinnButton(pDC, pt, pButton->GetState() & xtpPanePinPushed);
	}
	else if (pButton->GetID() == XTP_IDS_DOCKINGPANE_MAXIMIZE || pButton->GetID() == XTP_IDS_DOCKINGPANE_RESTORE)
	{
		CXTPDockingPaneCaptionButton::DrawMaximizeRestoreButton(pDC, pt, pButton->GetID() == XTP_IDS_DOCKINGPANE_MAXIMIZE, clrButton);
	}
	else if (pButton->GetID() == XTP_IDS_DOCKINGPANE_MENU)
	{
		CXTPBrushDC brush (*pDC, clrButton);
		CPoint pts[] =
		{
			CPoint(pt.x - 4, pt.y - 1), CPoint(pt.x - 1, pt.y + 2) , CPoint(pt.x, pt.y + 2), CPoint(pt.x + 3, pt.y - 1)
		};
		pDC->Polygon(pts, 4);
	}
}


int CXTPDockingPanePaintManager::DrawCaptionText(CDC* pDC, const CString& strTitle, CRect rcText, BOOL bVertical, BOOL bCalcWidth)
{
	int nWidth = 0;
	if (strTitle.IsEmpty())
		return 0;

	int nOldBkMode = pDC->SetBkMode(TRANSPARENT);
	if (bVertical)
	{
		CXTPFontDC font(pDC, &m_fntTitleVertical);

		CSize sz = pDC->GetTextExtent(strTitle);
		if (rcText.bottom > rcText.top)
		{
			rcText.left = (rcText.right - (rcText.Width() - sz.cy) / 2);
			rcText.right = rcText.left + rcText.Height();

			pDC->DrawText(strTitle, rcText, DT_NOPREFIX | DT_SINGLELINE | DT_NOCLIP | DT_END_ELLIPSIS);
		}
		nWidth = sz.cx;
	}
	else
	{
		CXTPFontDC font(pDC, &m_fntTitle);
		pDC->DrawText(strTitle, rcText, DT_SINGLELINE | DT_END_ELLIPSIS | DT_VCENTER | DT_NOPREFIX);

		if (bCalcWidth)
		{
			CXTPEmptyRect rcCalc;
			pDC->DrawText(strTitle, rcCalc, DT_CALCRECT | DT_SINGLELINE | DT_NOPREFIX);
			nWidth = rcCalc.Width();
		}
	}
	pDC->SetBkMode(nOldBkMode);

	return nWidth;
}

CXTPDockingPane* CXTPDockingPanePaintManager::GetSelectedPane(CXTPDockingPaneBase* pPane)
{
	if (pPane->GetType() == xtpPaneTypeTabbedContainer)
	{
		return ((CXTPDockingPaneTabbedContainer*)pPane)->GetSelected();
	}
	if (pPane->GetType() == xtpPaneTypeSidePanel)
	{
		return ((CXTPDockingPaneSidePanel*)pPane)->GetSelectedPane();
	}
	else if (pPane->GetType() == xtpPaneTypeMiniWnd)
	{
		CXTPDockingPaneBaseList lst;
		pPane->FindPane(xtpPaneTypeTabbedContainer, &lst);

		if (lst.GetCount() == 1)
		{
			return ((CXTPDockingPaneTabbedContainer*)lst.GetHead())->GetSelected();
		}
	}
	return NULL;
}

BOOL CXTPDockingPanePaintManager::IsCaptionEnabled(CXTPDockingPaneBase* pPane)
{
	CXTPDockingPane* pSelectedPane = GetSelectedPane(pPane);
	if (!pSelectedPane)
		return TRUE;
	return pSelectedPane->GetEnabled() & xtpPaneEnableClient;
}

void CXTPDockingPanePaintManager::DrawCaptionIcon(CDC* pDC, CXTPDockingPaneBase* pPane, CRect& rcCaption, BOOL bVertical)
{
	if (!m_bDrawCaptionIcon)
		return;

	CSize szIcon = GetTabPaintManager()->m_szIcon;

	CXTPDockingPane* pSelectedPane = GetSelectedPane(pPane);
	CXTPImageManagerIcon* pImage = pSelectedPane ? pSelectedPane->GetIcon(szIcon.cx) : NULL;

	if (!pImage)
		return;

	BOOL bEnabled = IsCaptionEnabled(pPane);
	CXTPImageManagerIconHandle& imageHandle = !bEnabled ? pImage->GetDisabledIcon(): pImage->GetIcon();

	if (!bVertical && rcCaption.Width() >= szIcon.cx)
	{
		pImage->Draw(pDC, CPoint(rcCaption.left, rcCaption.CenterPoint().y - szIcon.cy / 2),
			imageHandle, szIcon);

		rcCaption.left += szIcon.cx + 3;
	}
	else if (bVertical && rcCaption.Height() >= szIcon.cy)
	{
		pImage->Draw(pDC, CPoint(rcCaption.CenterPoint().x - szIcon.cy / 2, rcCaption.top),
			imageHandle, szIcon);

		rcCaption.top += szIcon.cy + 3;
	}
}

void CXTPDockingPanePaintManager::DrawAutoHidePanel(CDC* pDC, CRect rc, XTPDockingPaneDirection direction)
{
	pDC->FillSolidRect(rc, GetXtremeColor(XPCOLOR_3DFACE));

	switch (direction)
	{
	case xtpPaneDockLeft:
		pDC->Draw3dRect(rc.right - 1, rc.top, 1, rc.Height(), GetXtremeColor(COLOR_BTNTEXT), GetXtremeColor(COLOR_BTNTEXT));
		pDC->Draw3dRect(rc.right - 2, rc.top, 1, rc.Height(), GetXtremeColor(COLOR_3DSHADOW), GetXtremeColor(COLOR_3DSHADOW));
		break;

	case xtpPaneDockTop:
		pDC->Draw3dRect(rc.left, rc.bottom - 1, rc.Width(), 1, GetXtremeColor(COLOR_BTNTEXT), GetXtremeColor(COLOR_BTNTEXT));
		pDC->Draw3dRect(rc.left, rc.bottom - 2, rc.Width(), 1, GetXtremeColor(COLOR_3DSHADOW), GetXtremeColor(COLOR_3DSHADOW));
		break;

	case xtpPaneDockRight:
		pDC->Draw3dRect(rc.left + 1, rc.top, 1, rc.Height(), GetXtremeColor(COLOR_3DHIGHLIGHT), GetXtremeColor(COLOR_3DHIGHLIGHT));
		break;

	case xtpPaneDockBottom:
		pDC->Draw3dRect(rc.left, rc.top + 1, rc.Width(), 1, GetXtremeColor(COLOR_3DHIGHLIGHT), GetXtremeColor(COLOR_3DHIGHLIGHT));
		break;
	}
}

void CXTPDockingPanePaintManager::SetCustomTitleBrushes(
	HBRUSH hbrActiveTitleHorz,
	HBRUSH hbrInactiveTitleHorz /*= NULL*/,
	HBRUSH hbrActiveTitleVert /*= NULL*/,
	HBRUSH hbrInactiveTitleVert /*= NULL*/,
	BOOL bDestroyOnCleanup /*= FALSE*/)
{
	// Cleanup
	if(m_bDestroyCustomTitleBrushes)
	{
		if(NULL != m_hbrActiveTitleHorz)
		{
			DeleteObject(m_hbrActiveTitleHorz);
			if(m_hbrInactiveTitleHorz == m_hbrActiveTitleHorz)
				m_hbrInactiveTitleHorz = NULL;
			if(m_hbrActiveTitleVert == m_hbrActiveTitleHorz)
				m_hbrActiveTitleVert = NULL;
			if(m_hbrInactiveTitleVert == m_hbrActiveTitleHorz)
				m_hbrInactiveTitleVert = NULL;
			m_hbrActiveTitleHorz = NULL;
		}

		if(NULL != m_hbrInactiveTitleHorz)
		{
			DeleteObject(m_hbrInactiveTitleHorz);
			if(m_hbrActiveTitleVert == m_hbrInactiveTitleHorz)
				m_hbrActiveTitleVert = NULL;
			if(m_hbrInactiveTitleVert == m_hbrInactiveTitleHorz)
				m_hbrInactiveTitleVert = NULL;
			m_hbrInactiveTitleHorz = NULL;
		}

		if(NULL != m_hbrActiveTitleVert)
		{
			DeleteObject(m_hbrActiveTitleVert);
			if(m_hbrInactiveTitleVert == m_hbrActiveTitleVert)
				m_hbrInactiveTitleVert = NULL;
			m_hbrActiveTitleVert = NULL;
		}

		if(NULL != m_hbrInactiveTitleVert)
		{
			DeleteObject(m_hbrInactiveTitleVert);
			m_hbrInactiveTitleVert = NULL;
		}
	}

	// Assign new brushes.
	m_hbrActiveTitleHorz = hbrActiveTitleHorz;
	m_hbrInactiveTitleHorz = hbrInactiveTitleHorz;
	m_hbrActiveTitleVert = hbrActiveTitleVert;
	m_hbrInactiveTitleVert = hbrInactiveTitleVert;

	if(NULL == m_hbrInactiveTitleHorz)
		m_hbrInactiveTitleHorz = m_hbrActiveTitleHorz;
	if(NULL == m_hbrActiveTitleVert)
		m_hbrActiveTitleVert = m_hbrActiveTitleHorz;
	if(NULL == m_hbrInactiveTitleVert)
		m_hbrInactiveTitleVert = m_hbrInactiveTitleHorz;

	m_bDestroyCustomTitleBrushes = bDestroyOnCleanup;
}

HBRUSH CXTPDockingPanePaintManager::GetCustomTitleBrush(
	BOOL bActive,
	BOOL bVertical) const
{
	HBRUSH hBrush = (bVertical
		? (bActive ? m_hbrActiveTitleVert : m_hbrInactiveTitleVert)
		: (bActive ? m_hbrActiveTitleHorz : m_hbrInactiveTitleHorz));
	return hBrush;
}


#ifdef _XTP_ACTIVEX

// {FED95A2F-A4A2-4321-9A03-06DBA53BD9F0}
static const GUID IID_IDockingPanePaintManager =
{ 0xfed95a2f, 0xa4a2, 0x4321, { 0x9a, 0x3, 0x6, 0xdb, 0xa5, 0x3b, 0xd9, 0xf0 } };


BEGIN_INTERFACE_MAP(CXTPDockingPanePaintManager, CCmdTarget)
	INTERFACE_PART(CXTPDockingPanePaintManager, IID_IDockingPanePaintManager, Dispatch)
END_INTERFACE_MAP()


BEGIN_DISPATCH_MAP(CXTPDockingPanePaintManager, CCmdTarget)
	DISP_PROPERTY_ID(CXTPDockingPanePaintManager, "ShowCaption", 1, m_bShowCaption, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPDockingPanePaintManager, "CaptionFont", DISPID_FONT, OleGetFont, OleSetFont, VT_DISPATCH)
	DISP_PROPERTY_ID(CXTPDockingPanePaintManager, "HighlightActiveCaption", 2, m_bHighlightActiveCaption, VT_BOOL)
	DISP_PROPERTY_ID(CXTPDockingPanePaintManager, "DrawSingleTab", 3, m_bDrawSingleTab, VT_BOOL)
	DISP_PROPERTY_ID(CXTPDockingPanePaintManager, "SplitterSize", 4, m_nSplitterSize, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPDockingPanePaintManager, "SplitterStyle", 5, GetSplitterStyle, SetSplitterStyle, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPDockingPanePaintManager, "SplitterColor", 6, OleGetSplitterColor, OleSetSplitterColor, VT_COLOR)

	DISP_PROPERTY_ID(CXTPDockingPanePaintManager, "CaptionButtonStyle", 7, m_nCaptionButtonStyle, VT_I4)
	DISP_PROPERTY_NOTIFY_ID(CXTPDockingPanePaintManager, "CaptionMargin", 8, m_nCaptionFontGap, OleRefreshMetrics, VT_I4)
	DISP_PROPERTY_ID(CXTPDockingPanePaintManager, "DrawCaptionIcon", 9, m_bDrawCaptionIcon, VT_BOOL)

	DISP_PROPERTY_EX_ID(CXTPDockingPanePaintManager, "UseCustomTitleColors", 10, OleGetUseCustomTitleColors, OleSetUseCustomTitleColors, VT_BOOL)
	DISP_FUNCTION_ID(CXTPDockingPanePaintManager, "SetCustomTitleColors", 11, OleSetCustomTitleColors, VT_EMPTY, VTS_COLOR VTS_COLOR VTS_COLOR VTS_COLOR)
	DISP_FUNCTION_ID(CXTPDockingPanePaintManager, "GetCustomTitleColor", 12, OleGetCustomTitleColor, VT_COLOR, VTS_BOOL VTS_BOOL)
END_DISPATCH_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPDockingPanePaintManager, IID_IDockingPanePaintManager)

void CXTPDockingPanePaintManager::OleRefreshMetrics()
{
	RefreshMetrics();
}

void CXTPDockingPanePaintManager::OleSetFont(LPFONTDISP pFontDisp)
{
	ASSERT((pFontDisp == NULL) ||
		AfxIsValidAddress(pFontDisp, sizeof(IDispatch), FALSE));

	LOGFONT lf;
	if (AxGetLogFontFromDispatch(&lf, pFontDisp))
	{
		SetCaptionFontIndirect(&lf);
	}
	else
	{
		SetCaptionFontIndirect(NULL, TRUE);
	}
}

LPFONTDISP CXTPDockingPanePaintManager::OleGetFont()
{
	return AxCreateOleFont(&m_fntTitle, this, (LPFNFONTCHANGED)&CXTPDockingPanePaintManager::OleSetFont);
}

BOOL CXTPDockingPanePaintManager::OleGetUseCustomTitleColors()
{
	return m_bAxUseCustomTitleColors;
}

void CXTPDockingPanePaintManager::OleSetUseCustomTitleColors(BOOL fUse)
{
	if(fUse != m_bAxUseCustomTitleColors)
	{
		m_bAxUseCustomTitleColors = fUse;
		if(m_bAxUseCustomTitleColors)
		{
			OleSetCustomTitleColors(
				m_clrAxTitleActiveHorz,
				m_clrAxTitleInactiveHorz,
				m_clrAxTitleActiveVert,
				m_clrAxTitleInactiveVert);
		}
		else
		{
			SetCustomTitleBrushes(NULL);
		}
	}
}

void CXTPDockingPanePaintManager::OleSetCustomTitleColors(
	OLE_COLOR clrActiveHorz,
	OLE_COLOR clrInactiveHorz,
	OLE_COLOR clrActiveVert,
	OLE_COLOR clrInactiveVert)
{
	m_clrAxTitleActiveHorz = clrActiveHorz;
	m_clrAxTitleInactiveHorz = clrInactiveHorz;
	m_clrAxTitleActiveVert = clrActiveVert;
	m_clrAxTitleInactiveVert = clrInactiveVert;

	SetCustomTitleBrushes(
		CreateSolidBrush(m_clrAxTitleActiveHorz),
		CreateSolidBrush(m_clrAxTitleInactiveHorz),
		CreateSolidBrush(m_clrAxTitleActiveVert),
		CreateSolidBrush(m_clrAxTitleInactiveVert),
		TRUE);
}

OLE_COLOR CXTPDockingPanePaintManager::OleGetCustomTitleColor(BOOL fActive, BOOL fVert)
{
	OLE_COLOR clr;
	if(fActive)
	{
		if(fVert)
			clr = m_clrAxTitleActiveVert;
		else
			clr = m_clrAxTitleActiveHorz;
	}
	else
	{
		if(fVert)
			clr = m_clrAxTitleInactiveVert;
		else
			clr = m_clrAxTitleInactiveHorz;
	}

	return clr;
}

COLORREF CXTPDockingPanePaintManager::OleGetSplitterColor()
{
	return m_clrSplitter;
}

void CXTPDockingPanePaintManager::OleSetSplitterColor(OLE_COLOR clr)
{
	m_clrSplitter.SetCustomValue(AxTranslateColor(clr));
}

#endif
