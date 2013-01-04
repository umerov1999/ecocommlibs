// XTPDockingPanePaintManager.cpp : implementation of the CXTPDockingPanePaintManager class.
//
// This file is a part of the XTREME DOCKINGPANE MFC class library.
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
#include "Resource.h"

#include "Common/XTPVC80Helpers.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPResourceImage.h"
#include "Common/XTPResourceManager.h"
#include "Common/XTPWinThemeWrapper.h"
#include "Common/XTPImageManager.h"
#include "Common/XTPSystemHelpers.h"

#include "TabManager/XTPTabManager.h"
#include "TabManager/XTPTabPaintManager.h"

#include "XTPDockingPaneDefines.h"
#include "XTPDockingPaneBase.h"
#include "XTPDockingPaneBaseContainer.h"
#include "XTPDockingPanePaintManager.h"
#include "XTPDockingPaneTabbedContainer.h"
#include "XTPDockingPane.h"
#include "XTPDockingPaneMiniWnd.h"
#include "XTPDockingPaneSplitterContainer.h"
#include "XTPDockingPaneSidePanel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


BOOL CXTPDockingPaneCaptionButton::IsVisible()
{
	return m_pPane->IsCaptionButtonVisible(this);
}

AFX_INLINE void XTPDeflateRectDirection(CRect& rc, int left, int top, int right, int bottom, BOOL bVertical)
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

AFX_INLINE int XTPGetRectDirectionHeight(const CRect& rc, BOOL bVertical)
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

void AFX_CDECL CXTPDockingPaneCaptionButton::DrawPinnButton(CDC* pDC, CPoint pt, BOOL bPinned)
{
	pt.y--;

	if (bPinned)
	{
		pDC->MoveTo(pt.x - 4, pt.y); pDC->LineTo(pt.x - 1, pt.y);
		pDC->MoveTo(pt.x - 1, pt.y - 3); pDC->LineTo(pt.x - 1, pt.y + 4);
		pDC->MoveTo(pt.x - 1, pt.y + 2); pDC->LineTo(pt.x + 3, pt.y + 2);
		pDC->LineTo(pt.x + 3, pt.y - 2); pDC->LineTo(pt.x - 1, pt.y - 2);
		pDC->MoveTo(pt.x - 1, pt.y + 1); pDC->LineTo(pt.x + 3, pt.y + 1);
	}
	else
	{
		pt.y += 2;
		pDC->MoveTo(pt.x - 3, pt.y - 1); pDC->LineTo(pt.x + 4, pt.y - 1);
		pDC->MoveTo(pt.x, pt.y - 1); pDC->LineTo(pt.x, pt.y + 3);
		pDC->MoveTo(pt.x - 2, pt.y - 1); pDC->LineTo(pt.x - 2, pt.y - 6);
		pDC->LineTo(pt.x + 2, pt.y - 6); pDC->LineTo(pt.x + 2, pt.y - 1);
		pDC->MoveTo(pt.x + 1, pt.y - 1); pDC->LineTo(pt.x + 1, pt.y - 6);
	}
}

void AFX_CDECL CXTPDockingPaneCaptionButton::DrawMaximizeRestoreButton(CDC* pDC, CPoint pt, BOOL bMaximize, COLORREF clr)
{
	if (bMaximize)
	{
		pDC->Draw3dRect(pt.x - 5, pt.y - 5, 9, 9, clr, clr);
		pDC->FillSolidRect(pt.x - 5, pt.y - 5, 9, 2, clr);
	}
	else
	{
		pDC->Draw3dRect(pt.x - 4, pt.y - 2, 6, 6, clr, clr);
		pDC->FillSolidRect(pt.x - 4, pt.y - 2, 6, 2, clr);

		pDC->FillSolidRect(pt.x - 2, pt.y - 5, 6, 2, clr);
		pDC->SetPixel(pt.x - 2, pt.y - 3, clr);
		pDC->FillSolidRect(pt.x + 3, pt.y - 3, 1, 4, clr);
		pDC->SetPixel(pt.x + 2, pt.y, clr);
	}
}

void CXTPDockingPaneCaptionButton::InvalidateRect()
{
	m_pPane->RedrawPane();
}

BOOL CXTPDockingPaneCaptionButton::IsEnabled() const
{
	CXTPDockingPane* pPane = CXTPDockingPanePaintManager::GetSelectedPane(GetPane());
	if (pPane && ((pPane->GetEnabled() & xtpPaneEnableActions) == 0))
		return FALSE;

	return TRUE;
}

DWORD CXTPDockingPaneCaptionButton::Click (CWnd* pWnd, CPoint pt, BOOL bClient)
{
	if ((::GetCapture() != NULL))
		return FALSE;

	pWnd->SetCapture();
	ASSERT(pWnd == CWnd::GetCapture());

	BOOL  bClick = FALSE;
	CRect rcBtn(m_rcButton);

	while (CWnd::GetCapture() == pWnd)
	{

		BOOL bNewDown = ::PtInRect (rcBtn, pt) != 0;
		if (bNewDown != m_bPressed)
		{
			m_bPressed = bNewDown;

			InvalidateRect();
		}

		MSG msg;
		VERIFY(::GetMessage(&msg, NULL, 0, 0));

		switch (msg.message)
		{
		case WM_NCMOUSEMOVE:
			break;
		case WM_MOUSEMOVE:
			pt = CPoint((short signed)LOWORD(msg.lParam), (short signed)HIWORD(msg.lParam));
			if (!bClient)
			{
				pWnd->ClientToScreen (&pt);
				CXTPDrawHelpers::ScreenToWindow(pWnd, &pt);
			}
			break;

		case WM_LBUTTONUP:
			bClick = m_bPressed;
			goto ExitLoop;

		case WM_KEYDOWN:
			if (msg.wParam != VK_ESCAPE)
				break;
		case WM_CANCELMODE:
		case WM_RBUTTONDOWN:
			goto ExitLoop;

		default:
			DispatchMessage (&msg);
			break;
		}
	}
ExitLoop:
	ReleaseCapture();

	m_bPressed = FALSE;
	CheckForMouseOver (pt);

	InvalidateRect();

	return bClick;
}

void CXTPDockingPaneCaptionButton::SetRect (CRect rc)
{
	ASSERT(m_pPane);
	if (!m_pPane)
		return;

	if (rc != m_rcButton)
	{
		m_rcButton = rc;
		m_pPane->OnCaptionButtonRectChanged(this);
	}
}

BOOL CXTPDockingPaneCaptionButton::CheckForMouseOver (CPoint pt)
{
	if (!PtInRect (pt) && m_bSelected)
	{
		m_bSelected = FALSE;
		InvalidateRect();
	}

	if (!m_bSelected && PtInRect (pt) && IsEnabled())
	{
		TRACKMOUSEEVENT tme =
		{
			sizeof(TRACKMOUSEEVENT), TME_LEAVE, m_pPane->GetPaneHwnd()
		};
		_TrackMouseEvent(&tme);

		m_bSelected = TRUE;

		InvalidateRect();
		return TRUE;
	}
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
// CXTPDockingPanePaintManager


CXTPDockingPanePaintManager::CXTPDockingPanePaintManager()
: m_bUseOfficeFont(FALSE)
, m_strOfficeFont(_T("Tahoma"))
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



CXTPDockingPanePaintManager::~CXTPDockingPanePaintManager()
{
	m_pTabPaintManager->InternalRelease();
	m_pPanelPaintManager->InternalRelease();

	SAFE_DELETE(m_themeToolbar);
	SAFE_DELETE(m_themeExplorer);
	SAFE_DELETE(m_themeButton);
	SAFE_DELETE(m_themeWindow);
	SAFE_DELETE(m_themeRebar);
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
	RefreshXtremeColors();

	CXTPColorManager* pColorManager = XTPColorManager();

	for (int i = 0; i <= XPCOLOR_LAST; i++)
		m_arrColor[i] = pColorManager->GetColor(i);

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

CRect CXTPDockingPanePaintManager::GetPaneCaptionRect(const CXTPDockingPaneSidePanel* pPane)
{
	CXTPClientRect rect(pPane);
	rect.DeflateRect(1, 1);

	int nTitleFont = GetCaptionHeight();
	XTPDockingPaneDirection direction = pPane->GetDirection();

	if (direction == xtpPaneDockLeft) rect.right = rect.left + nTitleFont;
	else if (direction == xtpPaneDockRight) rect.left = rect.right - nTitleFont;
	else if (direction == xtpPaneDockTop) rect.bottom = rect.top + nTitleFont;
	else if (direction == xtpPaneDockBottom) rect.top = rect.bottom - nTitleFont;

	return rect;
}


void CXTPDockingPanePaintManager::DrawSplitter(CDC* pDC, CXTPDockingPaneSplitterWnd* pSplitter)
{
	CXTPClientRect rc((CWnd*)pSplitter);

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

//////////////////////////////////////////////////////////////////////////
// CXTPDockingPaneVisualStudio6Theme

CXTPDockingPaneVisualStudio6Theme::CXTPDockingPaneVisualStudio6Theme()
{
	m_nCaptionButtonStyle = xtpPaneCaptionButtonDefault;
}

void CXTPDockingPaneVisualStudio6Theme::DrawCaptionPart(CDC* pDC, CXTPDockingPaneBase* pPane, CRect rcCaption, const CString& strTitle, BOOL bActive, BOOL bVertical)
{
	UNREFERENCED_PARAMETER(strTitle);
	UNREFERENCED_PARAMETER(bActive);

	XTPDeflateRectDirection(rcCaption, 0, 0, 2, 0, bVertical);
	DrawCaptionButtons(pDC, pPane->GetCaptionButtons(), rcCaption, m_clrNormalCaptionText, 14, 2, bVertical);

	int nOffset = 4;
	XTPDeflateRectDirection(rcCaption, 4, 1, 2, 0, bVertical);

	if (bVertical)
	{
		if (rcCaption.bottom > nOffset)
		{
			int nCenter = rcCaption.CenterPoint().x;
			pDC->Draw3dRect(nCenter - 3, nOffset, 3, rcCaption.bottom - nOffset, GetXtremeColor(COLOR_BTNHILIGHT), GetXtremeColor(COLOR_3DSHADOW));
			pDC->Draw3dRect(nCenter + 1, nOffset, 3, rcCaption.bottom - nOffset, GetXtremeColor(COLOR_BTNHILIGHT), GetXtremeColor(COLOR_3DSHADOW));
		}
	}
	else
	{
		if (rcCaption.right > nOffset)
		{
			int nCenter = rcCaption.CenterPoint().y;
			pDC->Draw3dRect(nOffset, nCenter - 3, rcCaption.right - nOffset, 3, GetXtremeColor(COLOR_BTNHILIGHT), GetXtremeColor(COLOR_3DSHADOW));
			pDC->Draw3dRect(nOffset, nCenter + 1, rcCaption.right - nOffset, 3, GetXtremeColor(COLOR_BTNHILIGHT), GetXtremeColor(COLOR_3DSHADOW));
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
// CXTPDockingPaneVisualStudio2003Theme

class CXTPDockingPaneVisualStudio2003Theme::COfficePanelColorSet :
	public CXTPTabPaintManager::CColorSetVisualStudio2003
{
public:
	virtual void RefreshMetrics();
};

void CXTPDockingPaneVisualStudio2003Theme::COfficePanelColorSet::RefreshMetrics()
{
	CColorSetVisualStudio2003::RefreshMetrics();

	if (m_pPaintManager->GetAppearance() == xtpTabAppearanceFlat)
	{
		m_clrSelectedText.SetStandardValue(GetSysColor(COLOR_3DDKSHADOW));
		m_clrNormalText.SetStandardValue(GetSysColor(COLOR_3DDKSHADOW));
	}
}


CXTPDockingPaneVisualStudio2003Theme::CXTPDockingPaneVisualStudio2003Theme()
{
	m_pPanelPaintManager->SetAppearance(xtpTabAppearanceFlat);
	m_pPanelPaintManager->SetColorSet(new COfficePanelColorSet);
	m_pTabPaintManager->SetAppearance(xtpTabAppearanceVisualStudio);
	m_bGradientCaption = FALSE;
	m_bUseBoldCaptionFont = FALSE;
	m_nCaptionButtonSize = 14;
}

void CXTPDockingPaneVisualStudio2003Theme::RefreshMetrics()
{
	CXTPDockingPaneGripperedTheme::RefreshMetrics();

	m_clrActiveCaption.SetStandardValue(GetXtremeColor(COLOR_ACTIVECAPTION), GetXtremeColor(27));
	m_clrNormalCaption.SetStandardValue(GetXtremeColor(COLOR_INACTIVECAPTION), GetSysColor(28));

	m_clrNormalCaptionText.SetStandardValue(GetXtremeColor(COLOR_BTNTEXT));
	m_clrActiveCaptionText.SetStandardValue(GetXtremeColor(COLOR_CAPTIONTEXT));

	if (XTPSystemVersion()->IsWin95())
	{
		// Windows 95
		m_clrNormalCaption.SetStandardValue(GetXtremeColor(COLOR_INACTIVECAPTION));
		m_clrActiveCaption.SetStandardValue(GetXtremeColor(COLOR_ACTIVECAPTION));
	}
}

COLORREF CXTPDockingPaneVisualStudio2003Theme::FillCaptionPart(CDC* pDC, CXTPDockingPaneBase* pPane, CRect rcCaption, BOOL bActive, BOOL bVertical)
{
	if (m_bGradientCaption)
	{
		XTPDrawHelpers()->GradientFill(pDC, rcCaption, bActive ? m_clrActiveCaption : m_clrNormalCaption, !bVertical);
	}
	else
	{
		if (bActive)
		{
			pDC->FillSolidRect(rcCaption, m_clrActiveCaption.clrLight);
		}
		else if (pPane->GetType() != xtpPaneTypeSidePanel)
		{
			CXTPPenDC pen(*pDC, GetXtremeColor(XPCOLOR_3DSHADOW));
			pDC->MoveTo(rcCaption.left + 1, rcCaption.top); pDC->LineTo(rcCaption.right - 1 , rcCaption.top);
			pDC->MoveTo(rcCaption.left, rcCaption.top + 1); pDC->LineTo(rcCaption.left, rcCaption.bottom - 1);
			pDC->MoveTo(rcCaption.left + 1, rcCaption.bottom - 1); pDC->LineTo(rcCaption.right - 1 , rcCaption.bottom - 1);
			pDC->MoveTo(rcCaption.right -1 , rcCaption.top + 1); pDC->LineTo(rcCaption.right - 1, rcCaption.bottom - 1);
		}
	}

	return  bActive ? m_clrActiveCaptionText : m_clrNormalCaptionText;
}

void CXTPDockingPaneVisualStudio2003Theme::DrawCaptionPart(CDC* pDC, CXTPDockingPaneBase* pPane, CRect rcCaption, const CString& strTitle, BOOL bActive, BOOL bVertical)
{
	COLORREF clrButton = FillCaptionPart(pDC, pPane, rcCaption, bActive, bVertical);

	COLORREF clrText = bActive ? m_clrActiveCaptionText : m_clrNormalCaptionText;

	XTPDeflateRectDirection(rcCaption, 0, 0, 2, 0, bVertical);
	DrawCaptionButtons(pDC, pPane->GetCaptionButtons(), rcCaption, clrButton, m_nCaptionButtonSize, 2, bVertical);

	// Title text
	pDC->SetTextColor(IsCaptionEnabled(pPane) ? clrText : m_clrDisabledCaptionText);
	XTPDeflateRectDirection(rcCaption, 4, 1, 0, 0, bVertical);

	DrawCaptionIcon(pDC, pPane, rcCaption, bVertical);
	DrawCaptionText(pDC, strTitle, rcCaption, bVertical);
}


///////////////////////////////////////////////////////////////////////////////
// CXTPDockingPaneDefaultTheme

CXTPDockingPaneDefaultTheme::CXTPDockingPaneDefaultTheme()
{
	m_pPanelPaintManager->SetAppearance(xtpTabAppearancePropertyPage);
	m_pTabPaintManager->SetAppearance(xtpTabAppearancePropertyPage);

	m_nCaptionButtonStyle = xtpPaneCaptionButtonDefault;
}


void CXTPDockingPaneDefaultTheme::RefreshMetrics()
{
	CXTPDockingPanePaintManager::RefreshMetrics();

	m_clrActiveCaption.SetStandardValue(GetXtremeColor(COLOR_ACTIVECAPTION));
	m_clrNormalCaption.SetStandardValue(GetXtremeColor(COLOR_INACTIVECAPTION));

	m_clrActiveCaptionText.SetStandardValue(GetXtremeColor(COLOR_CAPTIONTEXT));
	m_clrNormalCaptionText.SetStandardValue(GetXtremeColor(COLOR_INACTIVECAPTIONTEXT));
}

void CXTPDockingPaneDefaultTheme::DrawFloatingFrame(CDC* pDC, CXTPDockingPaneMiniWnd* pPane, CRect rc)
{
	pDC->Draw3dRect(rc, GetXtremeColor(COLOR_3DFACE), 0);
	rc.DeflateRect(1, 1);
	pDC->Draw3dRect(rc, GetXtremeColor(COLOR_3DHIGHLIGHT), GetXtremeColor(COLOR_3DSHADOW));
	rc.DeflateRect(1, 1);
	pDC->Draw3dRect(rc, GetXtremeColor(COLOR_3DFACE), GetXtremeColor(COLOR_3DFACE));

	int nTitleHeight = m_nTitleHeight;
	CRect rcCaption(rc);
	rcCaption.DeflateRect(0, 0, 0, rc.Height() - nTitleHeight - 3);

	CXTPBufferDC dcCache(*pDC, rcCaption);
	dcCache.SetBkMode(TRANSPARENT);

	dcCache.FillSolidRect(rcCaption, GetXtremeColor(COLOR_3DFACE));

	CString strTitle;
	pPane->GetWindowText(strTitle);

	rcCaption.DeflateRect(1, 2, 1, 2);

	BOOL bActive = m_bHighlightActiveCaption && pPane->IsActive();
	DrawCaptionPart(&dcCache, pPane, rcCaption, strTitle, bActive, FALSE);
}

void CXTPDockingPaneDefaultTheme::DrawSidePanel(CDC* pDC, CXTPDockingPaneSidePanel* pPane, CRect rc)
{
	pDC->FillSolidRect(rc, GetXtremeColor(COLOR_3DFACE));
	pDC->SetBkMode(TRANSPARENT);

	pDC->Draw3dRect(rc, GetXtremeColor(COLOR_WINDOWFRAME), GetXtremeColor(COLOR_WINDOWFRAME));

	CRect rcCaption = GetPaneCaptionRect(pPane);

	BOOL bActive = m_bHighlightActiveCaption && pPane->IsActive();

	CString strTitle = pPane->GetTitle();

	DrawCaptionPart(pDC, pPane, rcCaption, strTitle, bActive, pPane->GetDirection() == xtpPaneDockLeft || pPane->GetDirection() == xtpPaneDockRight);
}

void CXTPDockingPaneDefaultTheme::DrawCaptionPart(CDC* pDC, CXTPDockingPaneBase* pPane, CRect rcCaption, const CString& strTitle, BOOL bActive, BOOL bVertical)
{
	pDC->FillSolidRect(rcCaption, bActive ? m_clrActiveCaption : m_clrNormalCaption);

	XTPDeflateRectDirection(rcCaption, 0, 0, 2, 0, bVertical);
	DrawCaptionButtons(pDC, pPane->GetCaptionButtons(), rcCaption, GetXtremeColor(COLOR_BTNTEXT), 14, 0, bVertical);

	pDC->SetTextColor(IsCaptionEnabled(pPane) ? (bActive ? m_clrActiveCaptionText : m_clrNormalCaptionText) : m_clrDisabledCaptionText);
	XTPDeflateRectDirection(rcCaption, 4, 1, 0, 0, bVertical);

	DrawCaptionIcon(pDC, pPane, rcCaption, bVertical);
	DrawCaptionText(pDC, strTitle, rcCaption, bVertical);
}

void CXTPDockingPaneDefaultTheme::DrawCaption(CDC* pDC, CXTPDockingPaneTabbedContainer* pPane, CRect rc)
{
	BOOL bVertical = pPane->IsCaptionVertical();

	CRect rcCaption(rc);
	XTPDeflateRectDirection(rcCaption, 0, 2, 0, XTPGetRectDirectionHeight(rc, bVertical)  - m_nTitleHeight - 1, bVertical);

	BOOL bActive = m_bHighlightActiveCaption && pPane->IsActive();

	if (bVertical)
	{
		pDC->FillSolidRect(rc.left, rc.top, rcCaption.right + 2, rcCaption.Height(), GetXtremeColor(COLOR_3DFACE));
	}
	else
	{
		pDC->FillSolidRect(rc.left, rc.top, rc.Width(), rcCaption.bottom + 2, GetXtremeColor(COLOR_3DFACE));
	}

	DrawCaptionPart(pDC, pPane, rcCaption, pPane->GetTitle(), bActive, bVertical);
}

///////////////////////////////////////////////////////////////////////////////
// CXTPDockingPaneGripperedTheme

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

CXTPDockingPaneGripperedTheme::CXTPDockingPaneGripperedTheme()
{
	m_nCaptionButtonStyle = xtpPaneCaptionButtonOffice;
}

void CXTPDockingPaneGripperedTheme::RefreshMetrics()
{
	CXTPDockingPaneDefaultTheme::RefreshMetrics();

	m_clrActiveCaption.SetStandardValue(GetXtremeColor(COLOR_3DFACE));
	m_clrNormalCaption.SetStandardValue(GetXtremeColor(COLOR_3DFACE));

	m_clrActiveCaptionText.SetStandardValue(GetXtremeColor(COLOR_BTNTEXT));
	m_clrNormalCaptionText.SetStandardValue(GetXtremeColor(COLOR_BTNTEXT));
}

void CXTPDockingPaneGripperedTheme::DrawCaptionPart(CDC* pDC, CXTPDockingPaneBase* pPane, CRect rcCaption, const CString& strTitle, BOOL bActive, BOOL bVertical)
{
	COLORREF clrText = bActive ? m_clrActiveCaptionText : m_clrNormalCaptionText;

	XTPDeflateRectDirection(rcCaption, 0, 0, 2, 0, bVertical);
	DrawCaptionButtons(pDC, pPane->GetCaptionButtons(), rcCaption, clrText, 14, 2, bVertical);

	int nOffset = 4;
	XTPDeflateRectDirection(rcCaption, 4, 1, 2, 0, bVertical);

	if (!strTitle.IsEmpty())
	{
		pDC->SetTextColor(IsCaptionEnabled(pPane) ? clrText : m_clrDisabledCaptionText);


		DrawCaptionIcon(pDC, pPane, rcCaption, bVertical);
		int nWidth = DrawCaptionText(pDC, strTitle, rcCaption, bVertical, TRUE);

		nOffset = nWidth + 4 + (bVertical ? rcCaption.top : rcCaption.left);
	}

	if (bVertical)
	{
		if (rcCaption.bottom > nOffset)
		{
			int nCenter = rcCaption.CenterPoint().x;
			pDC->Draw3dRect(nCenter - 3, nOffset, 3, rcCaption.bottom - nOffset, GetXtremeColor(COLOR_BTNHILIGHT), GetXtremeColor(COLOR_3DSHADOW));
			pDC->Draw3dRect(nCenter + 1, nOffset, 3, rcCaption.bottom - nOffset, GetXtremeColor(COLOR_BTNHILIGHT), GetXtremeColor(COLOR_3DSHADOW));
		}
	}
	else
	{
		if (rcCaption.right > nOffset)
		{
			int nCenter = rcCaption.CenterPoint().y;
			pDC->Draw3dRect(nOffset, nCenter - 3, rcCaption.right - nOffset, 3, GetXtremeColor(COLOR_BTNHILIGHT), GetXtremeColor(COLOR_3DSHADOW));
			pDC->Draw3dRect(nOffset, nCenter + 1, rcCaption.right - nOffset, 3, GetXtremeColor(COLOR_BTNHILIGHT), GetXtremeColor(COLOR_3DSHADOW));
		}
	}
}

void CXTPDockingPaneGripperedTheme::DrawFloatingFrame(CDC* pDC, CXTPDockingPaneMiniWnd* pPane, CRect rc)
{
	CXTPDockingPaneDefaultTheme::DrawFloatingFrame(pDC, pPane, rc);
}

void CXTPDockingPaneGripperedTheme::DrawCaption(CDC* pDC, CXTPDockingPaneTabbedContainer* pPane, CRect rc)
{
	BOOL bVertical = pPane->IsCaptionVertical();

	int nTitleHeight = m_nTitleHeight;
	CRect rcCaption(rc);
	XTPDeflateRectDirection(rcCaption, 0, m_rcCaptionMargin.top, 0, XTPGetRectDirectionHeight(rc, bVertical) - nTitleHeight - 1, bVertical);

	BOOL bActive = m_bHighlightActiveCaption && pPane->IsActive();

	if (bVertical)
	{
		pDC->FillSolidRect(rc.left, rc.top, rcCaption.right + 2, rc.Height(), GetXtremeColor(COLOR_3DFACE));
	}
	else
	{
		pDC->FillSolidRect(rc.left, rc.top, rc.Width(), rcCaption.bottom + 2, GetXtremeColor(COLOR_3DFACE));
	}

	DrawCaptionPart(pDC, pPane, rcCaption, pPane->GetTitle(), bActive, bVertical);
}

//////////////////////////////////////////////////////////////////////////
// CXTPDockingPaneWinExplorerTheme

CXTPDockingPaneWinExplorerTheme::CXTPDockingPaneWinExplorerTheme()
{
	m_bUseBoldCaptionFont = FALSE;
	m_rcCaptionMargin.SetRect(0, 0, 0, 0);
	m_nCaptionFontGap = 9;

	m_nCaptionButtonStyle = xtpPaneCaptionButtonThemedToolBar;
}

void CXTPDockingPaneWinExplorerTheme::DrawCaptionPart(CDC* pDC, CXTPDockingPaneBase* pPane, CRect rcCaption, const CString& strTitle, BOOL bActive, BOOL bVertical)
{
	if (m_themeRebar->IsAppThemed())
	{
		CWnd* pWnd = CWnd::FromHandle(pPane->GetPaneHwnd());

		ASSERT(pWnd);
		if (!pWnd)
			return;

		if (pPane->GetType() == xtpPaneTypeMiniWnd)
		{
			CRect rcClient;
			pWnd->GetWindowRect(rcClient);
			rcClient.OffsetRect(-rcClient.TopLeft());

			m_themeRebar->DrawThemeBackground(pDC->GetSafeHdc(), 0, 0, &rcClient, &rcCaption);
		}
		else
		{

			CWnd* pParent = pPane->GetDockingSite();

			CRect rc, rcParent, rcClient, rcClipRect;
			pWnd->GetClientRect(&rcClient);
			rcClipRect = rcClient;

			if (pParent)
			{
				pParent->GetWindowRect(rcParent);
				pWnd->GetWindowRect(rc);

				rcClient.TopLeft() += rcParent.TopLeft() - rc.TopLeft();
				rcClient.BottomRight() += rcParent.BottomRight() - rc.BottomRight();
			}

			m_themeRebar->DrawThemeBackground(pDC->GetSafeHdc(), 0, 0, &rcClient, &rcCaption);
		}
	}

	COLORREF clrText = bActive ? m_clrActiveCaptionText : m_clrNormalCaptionText;

	XTPDeflateRectDirection(rcCaption, 0, 0, 2, 0, bVertical);
	DrawCaptionButtons(pDC, pPane->GetCaptionButtons(), rcCaption, clrText, 16, 2, bVertical);

	pDC->SetTextColor(IsCaptionEnabled(pPane) ? clrText : m_clrDisabledCaptionText);
	XTPDeflateRectDirection(rcCaption, 4, 1, 0, 0, bVertical);

	DrawCaptionIcon(pDC, pPane, rcCaption, bVertical);
	DrawCaptionText(pDC, strTitle, rcCaption, bVertical);
}

void CXTPDockingPaneWinExplorerTheme::RefreshMetrics()
{
	CXTPDockingPaneGripperedTheme::RefreshMetrics();


	if (m_themeRebar->IsAppThemed())
	{
		m_pPanelPaintManager->SetAppearance(xtpTabAppearancePropertyPage);
		m_pPanelPaintManager->SetColor(xtpTabColorWinNative);
		m_pPanelPaintManager->m_bHotTracking = TRUE;

		m_pTabPaintManager->SetAppearance(xtpTabAppearancePropertyPage);
		m_pTabPaintManager->SetColor(xtpTabColorWinNative);
		m_pTabPaintManager->m_bHotTracking = TRUE;
	}
	else
	{
		m_pPanelPaintManager->SetAppearance(xtpTabAppearanceFlat);
		m_pPanelPaintManager->m_bHotTracking = FALSE;

		m_pTabPaintManager->SetAppearance(xtpTabAppearanceVisualStudio);
		m_pTabPaintManager->m_bHotTracking = FALSE;

	}
}

///////////////////////////////////////////////////////////////////////////////
// CXTPDockingPaneOffice2002VisioTheme

CXTPDockingPaneOffice2002VisioTheme::CXTPDockingPaneOffice2002VisioTheme()
{
	m_pPanelPaintManager->SetAppearance(xtpTabAppearanceFlat);

	m_pTabPaintManager->SetAppearance(xtpTabAppearanceVisio);
	m_pTabPaintManager->SetPosition(xtpTabPositionTop);

	m_nCaptionFontGap = 2;
	m_nCaptionButtonStyle = xtpPaneCaptionButtonOffice2003;
}

void CXTPDockingPaneOffice2002VisioTheme::RefreshMetrics()
{
	CXTPDockingPanePaintManager::RefreshMetrics();
}


void CXTPDockingPaneOffice2002VisioTheme::AdjustCaptionRect(const CXTPDockingPaneTabbedContainer* pPane, CRect& rc)
{
	CXTPDockingPaneVisualStudio2003Theme::AdjustCaptionRect(pPane, rc);

	BOOL bVertical = pPane->IsCaptionVertical();

	XTPDeflateRectDirection(rc, 3, 0, 3, 3, bVertical);

	if (!pPane->IsTitleVisible())
	{
		rc.DeflateRect(0, 3, 0, 0);
	}
	if (!pPane->IsTabsVisible() && m_pTabPaintManager->m_clientFrame != xtpTabFrameNone)
	{
		rc.DeflateRect(1, 1);
	}
}

void CXTPDockingPaneOffice2002VisioTheme::DrawPane(CDC* pDC, CXTPDockingPaneTabbedContainer* pPane, CRect rc)
{
	BOOL bActive = m_bHighlightActiveCaption && pPane->IsActive();
	pDC->FillSolidRect(rc, GetXtremeColor(bActive ? XPCOLOR_HIGHLIGHT : XPCOLOR_TOOLBAR_FACE));
	pDC->SetBkMode(TRANSPARENT);

	CRect rcClient(rc);

	if (pPane->IsTitleVisible())
	{
		pDC->FillSolidRect(rc.left, rc.top, rc.Width(), 1, m_clrSplitter);
		DrawCaption(pDC, pPane, rc);
		rcClient.top++;
	}

	AdjustCaptionRect(pPane, rc);

	if (pPane->IsTabsVisible())
	{
		m_pTabPaintManager->DrawTabControl(pPane, pDC, rc);
	}
	else if (m_pTabPaintManager->m_clientFrame != xtpTabFrameNone)
	{
		pDC->Draw3dRect(rc.left - 1, rc.top - 1, rc.Width() + 2, rc.Height() + 2, m_pTabPaintManager->GetColorSet()->m_clrFrameBorder, m_pTabPaintManager->GetColorSet()->m_clrFrameBorder);
	}


	Pixel(pDC, rcClient.left, rcClient.top, XPCOLOR_3DFACE);
	Pixel(pDC, rcClient.right - 1, rcClient.top, XPCOLOR_3DFACE);
	Pixel(pDC, rcClient.left, rcClient.bottom -1, XPCOLOR_3DFACE);
	Pixel(pDC, rcClient.right - 1, rcClient.bottom -1, XPCOLOR_3DFACE);
}

void CXTPDockingPaneOffice2002VisioTheme::DrawFloatingFrame(CDC* pDC, CXTPDockingPaneMiniWnd* pPane, CRect rc)
{
	BOOL bActive = m_bHighlightActiveCaption && pPane->IsActive();
	COLORREF clrFrame = GetXtremeColor(bActive ? XPCOLOR_HIGHLIGHT : XPCOLOR_TOOLBAR_FACE);

	pDC->Draw3dRect(rc, clrFrame, 0);
	rc.DeflateRect(1, 1);
	pDC->Draw3dRect(rc, GetXtremeColor(COLOR_3DHIGHLIGHT), GetXtremeColor(COLOR_3DSHADOW));
	rc.DeflateRect(1, 1);
	pDC->Draw3dRect(rc, clrFrame, clrFrame);

	int nTitleHeight = m_nTitleHeight;
	CRect rcCaption(rc);
	rcCaption.DeflateRect(0, 0, 0, rc.Height() - nTitleHeight - 3);

	pDC->FillSolidRect(rcCaption, clrFrame);

	rcCaption.right -= 2;
	DrawCaptionButtons(pDC, pPane->GetCaptionButtons(), rcCaption, GetXtremeColor(COLOR_BTNTEXT), 16, 0, FALSE);

	CString strTitle;
	pPane->GetWindowText(strTitle);


	int nRight = rcCaption.right;


	COLORREF clrText = GetXtremeColor(COLOR_BTNTEXT);
	int nLeft = 4;
	if (!strTitle.IsEmpty())
	{
		pDC->SetBkMode(TRANSPARENT);
		CRect rcText(rcCaption.left + 4, rcCaption.top + 1, nRight - 2, rcCaption.bottom);
		pDC->SetTextColor(clrText);
		CXTPFontDC font(pDC, &m_fntTitle);
		pDC->DrawText(strTitle, rcText, DT_SINGLELINE | DT_END_ELLIPSIS | DT_VCENTER | DT_NOPREFIX);

		CRect rcCalc(0, 0, 0, 0);
		pDC->DrawText(strTitle, rcCalc, DT_CALCRECT | DT_SINGLELINE | DT_NOPREFIX);

		nLeft = rcCalc.Width() + rcCaption.left + 14;
	}
	nRight -= 4;

	if (nRight - nLeft > 0)
	{
		int nCenter = rcCaption.CenterPoint().y;
		Line(pDC, nLeft, nCenter - 3, nRight, nCenter - 3, !bActive ? XPCOLOR_TOOLBAR_GRIPPER : COLOR_BTNTEXT);
		Line(pDC, nLeft, nCenter - 1, nRight, nCenter - 1, !bActive ? XPCOLOR_TOOLBAR_GRIPPER : COLOR_BTNTEXT);
		Line(pDC, nLeft, nCenter + 1, nRight, nCenter + 1, !bActive ? XPCOLOR_TOOLBAR_GRIPPER : COLOR_BTNTEXT);
		Line(pDC, nLeft, nCenter + 3, nRight, nCenter + 3, !bActive ? XPCOLOR_TOOLBAR_GRIPPER : COLOR_BTNTEXT);
	}
}

void CXTPDockingPaneOffice2002VisioTheme::DrawCaption(CDC* pDC, CXTPDockingPaneTabbedContainer* pPane, CRect rc)
{
	BOOL bVertical = pPane->IsCaptionVertical();
	BOOL bActive = m_bHighlightActiveCaption && pPane->IsActive();
	int nTitleHeight = m_nTitleHeight;
	CString strTitle = pPane->GetTitle();

	CRect rcCaption(rc);
	XTPDeflateRectDirection(rcCaption, 0, 2, 1, XTPGetRectDirectionHeight(rc, bVertical) - nTitleHeight - 3, bVertical);


	DrawCaptionButtons(pDC, pPane->GetCaptionButtons(), rcCaption, 0, 16, 0, bVertical);

	COLORREF clrText = GetXtremeColor(COLOR_BTNTEXT);

	int nOffset = 4;
	XTPDeflateRectDirection(rcCaption, 4, 1, 2, 0, bVertical);

	if (!strTitle.IsEmpty())
	{
		pDC->SetTextColor(IsCaptionEnabled(pPane) ? clrText : m_clrDisabledCaptionText);
		int nWidth = DrawCaptionText(pDC, strTitle, rcCaption, bVertical, TRUE);

		nOffset = nWidth + 10 + (bVertical ? rcCaption.top : rcCaption.left);
	}

	if (bVertical)
	{
		if (rcCaption.bottom - nOffset > 0)
		{
			int nCenter = rcCaption.CenterPoint().x;
			Line(pDC, nCenter - 3, nOffset, nCenter - 3, rcCaption.bottom, !bActive ? XPCOLOR_TOOLBAR_GRIPPER : COLOR_BTNTEXT);
			Line(pDC, nCenter - 1, nOffset, nCenter - 1, rcCaption.bottom, !bActive ? XPCOLOR_TOOLBAR_GRIPPER : COLOR_BTNTEXT);
			Line(pDC, nCenter + 1, nOffset, nCenter + 1, rcCaption.bottom, !bActive ? XPCOLOR_TOOLBAR_GRIPPER : COLOR_BTNTEXT);
			Line(pDC, nCenter + 3, nOffset, nCenter + 3, rcCaption.bottom, !bActive ? XPCOLOR_TOOLBAR_GRIPPER : COLOR_BTNTEXT);
		}
	}
	else
	{
		if (rcCaption.right - nOffset > 0)
		{
			int nCenter = rcCaption.CenterPoint().y;
			Line(pDC, nOffset, nCenter - 3, rcCaption.right, nCenter - 3, !bActive ? XPCOLOR_TOOLBAR_GRIPPER : COLOR_BTNTEXT);
			Line(pDC, nOffset, nCenter - 1, rcCaption.right, nCenter - 1, !bActive ? XPCOLOR_TOOLBAR_GRIPPER : COLOR_BTNTEXT);
			Line(pDC, nOffset, nCenter + 1, rcCaption.right, nCenter + 1, !bActive ? XPCOLOR_TOOLBAR_GRIPPER : COLOR_BTNTEXT);
			Line(pDC, nOffset, nCenter + 3, rcCaption.right, nCenter + 3, !bActive ? XPCOLOR_TOOLBAR_GRIPPER : COLOR_BTNTEXT);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
// CXTPDockingPaneOffice2003VisioTheme

CXTPDockingPaneOffice2003VisioTheme::CXTPDockingPaneOffice2003VisioTheme()
{

}

///////////////////////////////////////////////////////////////////////////////
// CXTPDockingPaneOffice2007VisioTheme

CXTPDockingPaneOffice2007VisioTheme::CXTPDockingPaneOffice2007VisioTheme()
{

}

///////////////////////////////////////////////////////////////////////////////
// CXTPDockingPaneOffice2003Theme

CXTPDockingPaneOffice2003Theme::CXTPDockingPaneOffice2003Theme()
{
	m_bDrawGripper = TRUE;
	m_bRoundedCaption = TRUE;
	m_nCaptionFontGap = 7;
	m_bUseBoldCaptionFont = TRUE;

	m_pPanelPaintManager->SetAppearance(xtpTabAppearancePropertyPageFlat);
	m_pPanelPaintManager->SetColor(xtpTabColorOffice2003);
	m_pPanelPaintManager->m_bHotTracking = TRUE;

	m_pTabPaintManager->SetAppearance(xtpTabAppearancePropertyPageFlat);
	m_pTabPaintManager->SetColor(xtpTabColorOffice2003);
	m_pTabPaintManager->m_bHotTracking = TRUE;

	m_nCaptionButtonStyle = xtpPaneCaptionButtonOffice2003;
}

CXTPDockingPaneOffice2003Theme::~CXTPDockingPaneOffice2003Theme()
{

}


void CXTPDockingPaneOffice2003Theme::RefreshMetrics()
{
	CXTPDockingPaneVisualStudio2003Theme::RefreshMetrics();

	m_clrNormalCaption.SetStandardValue(XTPColorManager()->LightColor(GetXtremeColor(COLOR_3DFACE), GetXtremeColor(COLOR_WINDOW), 0xcd), GetXtremeColor(COLOR_3DFACE));
	m_clrNormalCaptionText.SetStandardValue(GetXtremeColor(COLOR_BTNTEXT));

	m_clrActiveCaption.SetStandardValue(GetXtremeColor(XPCOLOR_HIGHLIGHT_CHECKED));
	m_clrActiveCaptionText.SetStandardValue(GetXtremeColor(COLOR_BTNTEXT));

	m_bLunaTheme = FALSE;

	if (XTPColorManager()->IsLowResolution())
	{
		m_clrNormalCaption.SetStandardValue(GetXtremeColor(COLOR_3DFACE));
		m_clrActiveCaption.SetStandardValue(GetXtremeColor(COLOR_HIGHLIGHT));
		m_clrActiveCaptionText.SetStandardValue(GetXtremeColor(COLOR_HIGHLIGHTTEXT));
	}

	XTPCurrentSystemTheme systemTheme = GetCurrentSystemTheme();

	switch (systemTheme)
	{
	case xtpSystemThemeBlue:
	case xtpSystemThemeRoyale:
	case xtpSystemThemeAero:
		{
			m_clrNormalCaption.SetStandardValue(RGB(221, 236, 254), RGB(129, 169, 226), 0.75f);
			m_clrSplitter.SetStandardValue(RGB(216, 231, 252));
			m_clrSplitterGripper.SetStandardValue(GetXtremeColor(XPCOLOR_TOOLBAR_GRIPPER));

			INT nElements[] =
			{
				XPCOLOR_TOOLBAR_GRIPPER, XPCOLOR_SEPARATOR, XPCOLOR_DISABLED, XPCOLOR_MENUBAR_FACE, XPCOLOR_MENUBAR_BORDER, XPCOLOR_HIGHLIGHT, XPCOLOR_HIGHLIGHT_BORDER, XPCOLOR_HIGHLIGHT_PUSHED_BORDER, XPCOLOR_HIGHLIGHT_CHECKED_BORDER, XPCOLOR_HIGHLIGHT_PUSHED, XPCOLOR_HIGHLIGHT_CHECKED, XPCOLOR_TOOLBAR_FACE, XPCOLOR_PUSHED_TEXT
			};
			COLORREF clrValues[] =
			{
				RGB(39, 65, 118), RGB(106, 140, 203), RGB(109, 150, 208), RGB(246, 246, 246), RGB(0, 45, 150), RGB(255, 238, 194), RGB(0, 0, 128), RGB(0, 0, 128), RGB(0, 0, 128), RGB(254, 128, 62), RGB(255, 192, 111), RGB(158, 190, 245), 0
			};
			SetColors(sizeof(nElements)/sizeof(INT), nElements, clrValues);

			m_bLunaTheme = TRUE;

		}
		break;

	case xtpSystemThemeOlive:
		{
			m_clrNormalCaption.SetStandardValue(RGB(244, 247, 222), RGB(183, 198, 145), 0.3f);
			m_clrSplitter.SetStandardValue(RGB(226, 231, 191));
			m_clrSplitterGripper.SetStandardValue(GetXtremeColor(XPCOLOR_TOOLBAR_GRIPPER));

			INT nElements[] =
			{
				XPCOLOR_TOOLBAR_GRIPPER, XPCOLOR_SEPARATOR, XPCOLOR_DISABLED, XPCOLOR_MENUBAR_FACE, XPCOLOR_MENUBAR_BORDER, XPCOLOR_HIGHLIGHT, XPCOLOR_HIGHLIGHT_BORDER, XPCOLOR_HIGHLIGHT_CHECKED_BORDER, XPCOLOR_HIGHLIGHT_PUSHED_BORDER, XPCOLOR_HIGHLIGHT_PUSHED, XPCOLOR_HIGHLIGHT_CHECKED, XPCOLOR_TOOLBAR_FACE, XPCOLOR_PUSHED_TEXT
			};
			COLORREF clrValues[] =
			{
				RGB(81, 94, 51), RGB(96, 128, 88), RGB(159, 174, 122), RGB(244, 244, 238), RGB(117, 141, 94), RGB(255, 238, 194), RGB(63, 93, 56), RGB(63, 93, 56), RGB(63, 93, 56), RGB(254, 128, 62), RGB(255, 192, 111), RGB(217, 217, 167), 0
			};
			SetColors(sizeof(nElements)/sizeof(INT), nElements, clrValues);

			m_bLunaTheme = TRUE;
		}
		break;

	case xtpSystemThemeSilver:
		{
			m_clrNormalCaption.SetStandardValue(RGB(249, 249, 255), RGB(156, 155, 183), 0.75f);
			m_clrSplitter.SetStandardValue(RGB(223, 223, 234));
			m_clrSplitterGripper.SetStandardValue(GetXtremeColor(XPCOLOR_TOOLBAR_GRIPPER));

			INT nElements[] =
			{
				XPCOLOR_TOOLBAR_GRIPPER, XPCOLOR_SEPARATOR, XPCOLOR_DISABLED, XPCOLOR_MENUBAR_FACE, XPCOLOR_MENUBAR_BORDER, XPCOLOR_HIGHLIGHT, XPCOLOR_HIGHLIGHT_BORDER, XPCOLOR_HIGHLIGHT_CHECKED_BORDER, XPCOLOR_HIGHLIGHT_PUSHED_BORDER, XPCOLOR_HIGHLIGHT_PUSHED, XPCOLOR_HIGHLIGHT_CHECKED, XPCOLOR_TOOLBAR_FACE, XPCOLOR_PUSHED_TEXT
			};
			COLORREF clrValues[] =
			{
				RGB(84, 84, 117), RGB(110, 109, 143), RGB(168, 167, 190), RGB(253, 250, 255), RGB(124, 124, 148), RGB(255, 238, 194), RGB(75, 75, 111), RGB(75, 75, 111), RGB(75, 75, 111), RGB(254, 128, 62), RGB(255, 192, 111), RGB(215, 215, 229), 0
			};
			SetColors(sizeof(nElements)/sizeof(INT), nElements, clrValues);

			m_bLunaTheme = TRUE;
		}
		break;
	}

	if (m_bLunaTheme)
	{
		m_clrActiveCaption.SetStandardValue(RGB(255, 213, 140), RGB(255, 166, 76));
		m_clrSplitterGradient.SetStandardValue(XTPColorManager()->grcDockBar);
	}


}

CRect CXTPDockingPaneOffice2003Theme::GetCaptionGripperRect(const CXTPDockingPaneTabbedContainer* /*pPane*/)
{
	return CRect(0, 0, 10, m_nTitleHeight);
}

CRect CXTPDockingPaneOffice2003Theme::GetPaneCaptionRect(const CXTPDockingPaneSidePanel* pPane)
{
	CXTPClientRect rect(pPane);
	rect.DeflateRect(1, 1);

	int nTitleFont = GetCaptionHeight() + 2;
	XTPDockingPaneDirection direction = pPane->GetDirection();

	if (direction == xtpPaneDockLeft) rect.right = rect.left + nTitleFont;
	else if (direction == xtpPaneDockRight) rect.left = rect.right - nTitleFont;
	else if (direction == xtpPaneDockTop) rect.bottom = rect.top + nTitleFont;
	else if (direction == xtpPaneDockBottom) rect.top = rect.bottom - nTitleFont;

	return rect;
}

int CXTPDockingPanePaintManager::DrawCaptionText(CDC* pDC, const CString& strTitle, CRect rcText, BOOL bVertical, BOOL bCalcWidth)
{
	int nWidth = 0;
	if (strTitle.IsEmpty())
		return 0;

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

void CXTPDockingPaneOffice2003Theme::DrawCaptionPart(CDC* pDC, CXTPDockingPaneBase* pPane, CRect rcCaption, const CString& strTitle, BOOL bActive, BOOL bVertical)
{
	COLORREF clrText = bActive ? m_clrActiveCaptionText : m_clrNormalCaptionText ;

	XTPDeflateRectDirection(rcCaption, 0, 0, 4, 0, bVertical);
	DrawCaptionButtons(pDC, pPane->GetCaptionButtons(), rcCaption, clrText, 16, 0, bVertical);

	if (!bVertical && m_bDrawGripper && (rcCaption.right > rcCaption.left + 7))
	{
		for (int y = rcCaption.top + 5; y < rcCaption.bottom - 3; y += 4)
		{
			pDC->FillSolidRect(rcCaption.left + 4 + 2, y + 1, 2, 2, GetXtremeColor(COLOR_WINDOW));
			pDC->FillSolidRect(rcCaption.left + 3 + 2, y, 2, 2, GetXtremeColor(XPCOLOR_TOOLBAR_GRIPPER));
		}
		rcCaption.left += 8;
	}

	if (bVertical && m_bDrawGripper && (rcCaption.bottom > rcCaption.top + 7))
	{
		for (int x = rcCaption.left + 3; x < rcCaption.right - 5; x += 4)
		{
			pDC->FillSolidRect(x + 2, rcCaption.top + 4 + 2, 2, 2, GetXtremeColor(COLOR_WINDOW));
			pDC->FillSolidRect(x + 1, rcCaption.top + 3 + 2, 2, 2, GetXtremeColor(XPCOLOR_TOOLBAR_GRIPPER));
		}
		rcCaption.top += 8;
	}

	pDC->SetTextColor(IsCaptionEnabled(pPane) ? clrText : m_clrDisabledCaptionText);

	XTPDeflateRectDirection(rcCaption, 6, 1, 2, 0, bVertical);

	DrawCaptionIcon(pDC, pPane, rcCaption, bVertical);
	DrawCaptionText(pDC, strTitle, rcCaption, bVertical);
}

void CXTPDockingPaneOffice2003Theme::DrawFloatingFrame(CDC* pDC, CXTPDockingPaneMiniWnd* pPane, CRect rc)
{
	COLORREF clrFrame = GetXtremeColor(XPCOLOR_MENUBAR_BORDER);

	pDC->Draw3dRect(rc, clrFrame, clrFrame);
	rc.DeflateRect(1, 1);
	pDC->Draw3dRect(rc, clrFrame, clrFrame);
	rc.DeflateRect(1, 1);

	pDC->Draw3dRect(rc, clrFrame, clrFrame);
	XTPDrawHelpers()->ExcludeCorners(pDC, rc);
	pDC->Draw3dRect(rc, m_clrNormalCaption.clrDark, m_clrNormalCaption.clrDark);

	int nTitleHeight = m_nTitleHeight;
	CRect rcCaption(rc);
	rcCaption.DeflateRect(0, 0, 0, rc.Height() - nTitleHeight - 3);

	CXTPBufferDC dcCache(*pDC, rcCaption);
	dcCache.SetBkMode(TRANSPARENT);

	BOOL bActive = m_bHighlightActiveCaption && pPane->IsActive();

	XTPDrawHelpers()->GradientFill(&dcCache, rcCaption,
		bActive ? m_clrActiveCaption : m_clrNormalCaption, FALSE);

	dcCache.SetPixel(rcCaption.left, rcCaption.bottom, m_clrNormalCaption.clrDark);
	dcCache.SetPixel(rcCaption.right - 1, rcCaption.bottom, m_clrNormalCaption.clrDark);

	rcCaption.DeflateRect(1, 0);

	CString strTitle;
	pPane->GetWindowText(strTitle);

	DrawCaptionPart(&dcCache, pPane, rcCaption, strTitle, bActive, FALSE);
}

void CXTPDockingPaneOffice2003Theme::DrawSidePanel(CDC* pDC, CXTPDockingPaneSidePanel* pPane, CRect rc)
{
	BOOL bActive = m_bHighlightActiveCaption && pPane->IsActive();

	pDC->FillSolidRect(rc, bActive ? m_clrActiveCaption : m_clrNormalCaption);
	pDC->SetBkMode(TRANSPARENT);

	pDC->Draw3dRect(rc, GetXtremeColor(COLOR_WINDOWFRAME), GetXtremeColor(COLOR_WINDOWFRAME));
	rc.DeflateRect(1, 1);

	CRect rcCaption = rc;
	int nTitleFont = GetCaptionHeight() + 2;
	XTPDockingPaneDirection direction = pPane->GetDirection();

	if (direction == xtpPaneDockLeft) rcCaption.right = rcCaption.left + nTitleFont;
	else if (direction == xtpPaneDockRight) rcCaption.left = rcCaption.right - nTitleFont;
	else if (direction == xtpPaneDockTop) rcCaption.bottom = rcCaption.top + nTitleFont;
	else if (direction == xtpPaneDockBottom) rcCaption.top = rcCaption.bottom - nTitleFont;


	BOOL bVertical = direction == xtpPaneDockLeft || direction == xtpPaneDockRight;

	XTPDrawHelpers()->GradientFill(pDC, rcCaption, bActive ? m_clrActiveCaption : m_clrNormalCaption, bVertical);

	DrawCaptionPart(pDC, pPane, rcCaption, pPane->GetTitle(), bActive, bVertical);
}

void CXTPDockingPaneOffice2003Theme::DrawCaption(CDC* pDC, CXTPDockingPaneTabbedContainer* pPane, CRect rc)
{
	BOOL bVertical = pPane->IsCaptionVertical();

	int nTitleHeight = m_nTitleHeight;
	CRect rcCaption(rc);

	if (bVertical)
	{
		rcCaption.DeflateRect(0, 0, rc.Width() - nTitleHeight - 3, 0);
	}
	else
	{
		rcCaption.DeflateRect(0, 0, 0, rc.Height() - nTitleHeight - 3);
	}

	CRect rcFill(rcCaption);

	if (m_bRoundedCaption)
	{
		XTPDeflateRectDirection(rcFill, 1, 0, 0, 0, bVertical);
		pDC->FillSolidRect(rcCaption, m_clrSplitter);

		XTPDrawHelpers()->ExcludeCorners(pDC, rcFill);
	}

	BOOL bActive = m_bHighlightActiveCaption && pPane->IsActive();

	XTPDrawHelpers()->GradientFill(pDC, rcFill,
		bActive ? m_clrActiveCaption : m_clrNormalCaption, bVertical);

	if (!m_bRoundedCaption)
	{
		pDC->FillSolidRect(rcCaption.left, rcCaption.top, 1, rcCaption.Height(), GetXtremeColor(XPCOLOR_FRAME));
		pDC->FillSolidRect(rcCaption.left, rcCaption.top, rcCaption.Width(), 1, GetXtremeColor(XPCOLOR_FRAME));
		pDC->FillSolidRect(rcCaption.right - 1, rcCaption.top, 1, rcCaption.Height(), GetXtremeColor(XPCOLOR_FRAME));
	}

	CString strTitle = pPane->GetTitle();
	DrawCaptionPart(pDC, pPane, rcCaption, strTitle, bActive, bVertical);
}


//////////////////////////////////////////////////////////////////////////
// CXTPDockingPaneOffice2007Theme

CXTPDockingPaneOffice2007Theme::CXTPDockingPaneOffice2007Theme()
{
	m_bRoundedCaption = FALSE;
	m_dwSplitterStyle = xtpPaneSplitterGradientVert;
	m_bDrawGripper = FALSE;

	m_pPanelPaintManager->SetAppearance(xtpTabAppearancePropertyPage2007);
	m_pPanelPaintManager->m_bHotTracking = TRUE;

	m_pTabPaintManager->SetAppearance(xtpTabAppearancePropertyPage2007);
	m_pTabPaintManager->m_bHotTracking = TRUE;

	m_pPanelPaintManager->m_rcButtonMargin.SetRect(2, 1, 2, 1);
	m_pTabPaintManager->m_rcButtonMargin.SetRect(2, 1, 2, 1);

	m_nCaptionFontGap = 5;
}

void CXTPDockingPaneOffice2007Theme::DrawCaption(CDC* pDC, CXTPDockingPaneTabbedContainer* pPane, CRect rc)
{
	BOOL bVertical = pPane->IsCaptionVertical();

	int nTitleHeight = m_nTitleHeight;
	CRect rcCaption(rc);

	if (bVertical)
	{
		rcCaption.DeflateRect(0, 0, rc.Width() - nTitleHeight - 3, 0);
	}
	else
	{
		rcCaption.DeflateRect(0, 0, 0, rc.Height() - nTitleHeight - 3);
	}

	BOOL bActive = m_bHighlightActiveCaption && pPane->IsActive();

	XTPDrawHelpers()->GradientFill(pDC, rcCaption,  bActive ? m_clrActiveCaption : m_clrNormalCaption, bVertical);

	DrawCaptionPart(pDC, pPane, rcCaption, pPane->GetTitle(), bActive, bVertical);
}

void CXTPDockingPaneOffice2007Theme::RefreshMetrics()
{
	CXTPDockingPaneOffice2003Theme::RefreshMetrics();

	CXTPResourceImages* pImages = XTPResourceImages();

	if (pImages->IsValid())
	{
		m_arrColor[XPCOLOR_FRAME] = pImages->GetImageColor(_T("Window"), _T("WindowFrame"));
		m_arrColor[XPCOLOR_3DFACE] = pImages->GetImageColor(_T("Window"), _T("ButtonFace"));
		m_arrColor[COLOR_3DHIGHLIGHT] = pImages->GetImageColor(_T("Window"), _T("EdgeHighLightColor"));

		m_clrSplitter.SetStandardValue(pImages->GetImageColor(_T("DockingPane"), _T("Splitter")));
		m_clrSplitterGripper.SetStandardValue(m_arrColor[XPCOLOR_FRAME]);

		m_arrColor[XPCOLOR_TOOLBAR_GRIPPER] = pImages->GetImageColor(_T("Toolbar"), _T("ToolbarGripper"));
		m_arrColor[XPCOLOR_MENUBAR_BORDER] = pImages->GetImageColor(_T("Toolbar"), _T("MenuPopupBorder"));

		m_bLunaTheme = TRUE;

		m_clrSplitterGradient.SetStandardValue(pImages->GetImageColor(_T("DockingPane"), _T("SplitterLight")),
			pImages->GetImageColor(_T("DockingPane"), _T("SplitterDark")));

		m_clrNormalCaption.SetStandardValue(pImages->GetImageColor(_T("DockingPane"), _T("NormalCaptionLight")),
			pImages->GetImageColor(_T("DockingPane"), _T("NormalCaptionDark")), 0.25f);
		m_clrNormalCaptionText.SetStandardValue(pImages->GetImageColor(_T("DockingPane"), _T("NormalCaptionText")));

		m_clrActiveCaption.SetStandardValue(pImages->GetImageColor(_T("DockingPane"), _T("ActiveCaptionLight")),
			pImages->GetImageColor(_T("DockingPane"), _T("ActiveCaptionDark")), 0.25f);
		m_clrActiveCaptionText.SetStandardValue(pImages->GetImageColor(_T("DockingPane"), _T("ActiveCaptionText")));

	}
	else
	{
		m_arrColor[XPCOLOR_FRAME] = RGB(101, 147, 207);
		m_arrColor[XPCOLOR_3DFACE] = RGB(191, 219, 255);
		m_arrColor[COLOR_3DHIGHLIGHT] = RGB(255, 255, 255);

		m_clrNormalCaption.SetStandardValue(RGB(227, 239, 255), RGB(175, 210, 255), 0.25f);
		m_clrSplitter.SetStandardValue(RGB(216, 231, 252));
		m_clrSplitterGripper.SetStandardValue(GetXtremeColor(XPCOLOR_TOOLBAR_GRIPPER));
		m_clrSplitterGradient.SetStandardValue(RGB(190, 218, 254), RGB(100, 143, 203));

		INT nElements[] =
		{
			XPCOLOR_TOOLBAR_GRIPPER, XPCOLOR_SEPARATOR, XPCOLOR_DISABLED, XPCOLOR_MENUBAR_FACE, XPCOLOR_MENUBAR_BORDER, XPCOLOR_HIGHLIGHT, XPCOLOR_HIGHLIGHT_BORDER, XPCOLOR_HIGHLIGHT_PUSHED_BORDER, XPCOLOR_HIGHLIGHT_CHECKED_BORDER, XPCOLOR_HIGHLIGHT_PUSHED, XPCOLOR_HIGHLIGHT_CHECKED, XPCOLOR_TOOLBAR_FACE, XPCOLOR_PUSHED_TEXT
		};
		COLORREF clrValues[] =
		{
			RGB(39, 65, 118), RGB(106, 140, 203), RGB(109, 150, 208), RGB(246, 246, 246), RGB(0, 45, 150), RGB(255, 238, 194), RGB(0, 0, 128), RGB(0, 0, 128), RGB(0, 0, 128), RGB(254, 128, 62), RGB(255, 192, 111), RGB(158, 190, 245), 0
		};
		SetColors(sizeof(nElements)/sizeof(INT), nElements, clrValues);

		m_bLunaTheme = TRUE;

		m_clrActiveCaption.SetStandardValue(RGB(255, 231, 162), RGB(255, 166, 76));
		m_clrNormalCaptionText.SetStandardValue(RGB(8, 55, 114));
		m_clrActiveCaptionText.SetStandardValue(RGB(8, 55, 114));
	}
}


//////////////////////////////////////////////////////////////////////////
// CXTPDockingPaneOffice2007OutlookTheme

CXTPDockingPaneOffice2007OutlookTheme::CXTPDockingPaneOffice2007OutlookTheme()
{
	m_nCaptionFontGap = 3;
}

void CXTPDockingPaneOffice2007OutlookTheme::DrawCaption(CDC* pDC, CXTPDockingPaneTabbedContainer* pPane, CRect rc)
{
	BOOL bVertical = pPane->IsCaptionVertical();

	int nTitleHeight = m_nTitleHeight;
	CRect rcCaption(rc);

	if (bVertical)
	{
		rcCaption.DeflateRect(0, 0, rc.Width() - nTitleHeight - 3, 0);
	}
	else
	{
		rcCaption.DeflateRect(0, 0, 0, rc.Height() - nTitleHeight - 3);
	}

	BOOL bActive = m_bHighlightActiveCaption && pPane->IsActive();

	XTPDrawHelpers()->GradientFill(pDC, rcCaption,  bActive ? m_clrActiveCaption : m_clrNormalCaption, bVertical);

	if (bVertical)
	{
		pDC->FillSolidRect(rcCaption.left, rcCaption.top, rcCaption.Width(), 1, GetXtremeColor(XPCOLOR_FRAME));
		pDC->FillSolidRect(rcCaption.left, rcCaption.top, 1, rcCaption.Height(), GetXtremeColor(XPCOLOR_FRAME));
		pDC->FillSolidRect(rcCaption.left, rcCaption.bottom - 1, rcCaption.Width(), 1, GetXtremeColor(XPCOLOR_FRAME));

		pDC->FillSolidRect(rcCaption.left + 1, rcCaption.top + 1, rcCaption.Width() - 1, 1, GetXtremeColor(COLOR_3DHIGHLIGHT));
		pDC->FillSolidRect(rcCaption.left + 1, rcCaption.top + 1, 1, rcCaption.Height() - 2, GetXtremeColor(COLOR_3DHIGHLIGHT));

	}
	else
	{
		pDC->FillSolidRect(rcCaption.left, rcCaption.top, rcCaption.Width(), 1, GetXtremeColor(XPCOLOR_FRAME));
		pDC->FillSolidRect(rcCaption.left, rcCaption.top, 1, rcCaption.Height(), GetXtremeColor(XPCOLOR_FRAME));
		pDC->FillSolidRect(rcCaption.right - 1, rcCaption.top, 1, rcCaption.Height(), GetXtremeColor(XPCOLOR_FRAME));

		pDC->FillSolidRect(rcCaption.left + 1, rcCaption.top + 1, rcCaption.Width() - 2, 1, GetXtremeColor(COLOR_3DHIGHLIGHT));
		pDC->FillSolidRect(rcCaption.left + 1, rcCaption.top + 1, 1, rcCaption.Height() - 1, GetXtremeColor(COLOR_3DHIGHLIGHT));
	}

	DrawCaptionPart(pDC, pPane, rcCaption, pPane->GetTitle(), bActive, bVertical);
}

void CXTPDockingPaneOffice2007OutlookTheme::RefreshMetrics()
{
	CXTPDockingPaneOffice2007Theme::RefreshMetrics();

	if (m_bUseStandardFont)
	{
		LOGFONT lf;
		VERIFY(CXTPDrawHelpers::GetIconLogFont(&lf));

		CString strOfficeFont = _T("Segoe UI");

		BOOL bOfficeFont = XTPDrawHelpers()->FontExists(m_strOfficeFont);

		if (bOfficeFont)
		{
			STRCPY_S(lf.lfFaceName, LF_FACESIZE, strOfficeFont);
			if (XTPSystemVersion()->IsClearTypeTextQualitySupported()) lf.lfQuality = 5;
		}
		else
		{
			STRCPY_S(lf.lfFaceName, LF_FACESIZE, _T("Arial"));
		}

		lf.lfWeight = FW_BOLD;
		lf.lfHeight = 20;

		SetCaptionFontIndirect(&lf);
	}
}

//////////////////////////////////////////////////////////////////////////
// CXTPDockingPaneOffice2007WordTheme


CXTPDockingPaneOffice2007WordTheme::CXTPDockingPaneOffice2007WordTheme()
{
	m_bRoundedCaption = FALSE;
	m_dwSplitterStyle = xtpPaneSplitterFlat;
	m_bDrawGripper = FALSE;

	m_pPanelPaintManager->SetAppearance(xtpTabAppearancePropertyPageAccess2007);
	m_pPanelPaintManager->m_bHotTracking = TRUE;

	m_pTabPaintManager->SetAppearance(xtpTabAppearancePropertyPageAccess2007);
	m_pTabPaintManager->m_bHotTracking = TRUE;

	m_nCaptionFontGap = 2;
	m_nSplitterSize = 0;
	m_nSplitterIndent = 2;
}

void CXTPDockingPaneOffice2007WordTheme::DrawCaption(CDC* pDC, CXTPDockingPaneTabbedContainer* pPane, CRect rc)
{
	BOOL bVertical = pPane->IsCaptionVertical();

	int nTitleHeight = m_nTitleHeight;
	CRect rcCaption(rc);

	if (bVertical)
	{
		rcCaption.DeflateRect(0, 0, rc.Width() - nTitleHeight - 3, 0);
	}
	else
	{
		rcCaption.DeflateRect(0, 0, 0, rc.Height() - nTitleHeight - 3);
	}

	CRect rcFill(rcCaption);

	BOOL bActive = m_bHighlightActiveCaption && pPane->IsActive();

	XTPDrawHelpers()->GradientFill(pDC, rcFill, bActive ? m_clrActiveCaption : m_clrNormalCaption, bVertical);

	CString strTitle = pPane->GetTitle();
	DrawCaptionPart(pDC, pPane, rcCaption, strTitle, bActive, bVertical);
}

void CXTPDockingPaneOffice2007WordTheme::DrawCaptionPart(CDC* pDC, CXTPDockingPaneBase* pPane, CRect rcCaption, const CString& strTitle, BOOL bActive, BOOL bVertical)
{
	COLORREF clrText = bActive ? m_clrActiveCaptionText : m_clrNormalCaptionText;

	DrawCaptionButtons(pDC, pPane->GetCaptionButtons(), rcCaption, clrText, 16, 0, bVertical);

	pDC->SetTextColor(IsCaptionEnabled(pPane) ? clrText : m_clrDisabledCaptionText);

	XTPDeflateRectDirection(rcCaption, 2, 1, 2, 0, bVertical);

	DrawCaptionIcon(pDC, pPane, rcCaption, bVertical);
	DrawCaptionText(pDC, strTitle, rcCaption, bVertical);

}

BOOL CXTPDockingPaneOffice2007WordTheme::PaneHasBorders(CXTPDockingPaneTabbedContainer* pPane) const
{
	return pPane->IsTitleVisible();
}

void CXTPDockingPaneOffice2007WordTheme::AdjustClientRect(CXTPDockingPaneTabbedContainer* pPane, CRect& rect, BOOL bApply)
{
	if (PaneHasBorders(pPane))
	{
		rect.DeflateRect(3, 3);
	}
	CXTPDockingPaneOffice2003Theme::AdjustClientRect(pPane, rect, bApply);
}

void CXTPDockingPaneOffice2007WordTheme::DrawPane(CDC* pDC, CXTPDockingPaneTabbedContainer* pPane, CRect rc)
{
	if (m_bOffice2010Style)
	{
		pDC->FillSolidRect(rc, m_arrColor[XPCOLOR_3DFACE]);
		pDC->Draw3dRect(rc.left, rc.top, rc.Width(), rc.Height(), m_arrColor[XPCOLOR_TOOLBAR_GRIPPER], m_arrColor[XPCOLOR_TOOLBAR_GRIPPER]);
		rc.DeflateRect(3, 3);
	}
	else
	{
		if (PaneHasBorders(pPane))
		{
			pDC->FillSolidRect(rc, m_arrColor[XPCOLOR_FRAME]);

			pDC->FillSolidRect(rc.left + 3, rc.top + 2, rc.Width() - 6, 1, m_arrColor[XPCOLOR_TOOLBAR_GRIPPER]);
			pDC->FillSolidRect(rc.left + 2, rc.top + 3, 1, rc.Height() - 5, m_arrColor[XPCOLOR_TOOLBAR_GRIPPER]);
			pDC->FillSolidRect(rc.right - 3, rc.top + 3, 1, rc.Height() - 5, m_arrColor[XPCOLOR_TOOLBAR_GRIPPER]);
			pDC->FillSolidRect(rc.left + 2, rc.bottom - 3, rc.Width() - 4, 1, m_arrColor[XPCOLOR_TOOLBAR_GRIPPER]);

			rc.DeflateRect(3, 3);
		}
	}

	CXTPDockingPaneOffice2003Theme::DrawPane(pDC, pPane, rc);
}

void CXTPDockingPaneOffice2007WordTheme::RefreshMetrics()
{
	CXTPDockingPaneOffice2003Theme::RefreshMetrics();

	CXTPResourceImages* pImages = XTPResourceImages();
	pImages->AssertValid();


	m_bOffice2010Style = pImages->GetImageInt(_T("DockingPane"), _T("Office2010Style"), FALSE);

	m_arrColor[XPCOLOR_FRAME] = pImages->GetImageColor(_T("DockingPane"), _T("WordSplitter"));
	m_arrColor[XPCOLOR_3DFACE] = pImages->GetImageColor(_T("DockingPane"), _T("WordPaneBackground"));
	m_arrColor[COLOR_3DHIGHLIGHT] = pImages->GetImageColor(_T("Window"), _T("EdgeHighLightColor"));


	m_arrColor[XPCOLOR_TOOLBAR_GRIPPER] = pImages->GetImageColor(_T("DockingPane"), _T("WordPaneBorder"));
	m_arrColor[XPCOLOR_MENUBAR_BORDER] = pImages->GetImageColor(_T("DockingPane"), _T("WordSplitter"));

	m_arrColor[XPCOLOR_HIGHLIGHT_BORDER] = pImages->GetImageColor(_T("Window"), _T("HighlightSelectedBorder"));
	m_arrColor[XPCOLOR_HIGHLIGHT] = pImages->GetImageColor(_T("Window"), _T("HighlightSelected"));
	m_arrColor[XPCOLOR_HIGHLIGHT_PUSHED_BORDER] = pImages->GetImageColor(_T("Window"), _T("HighlightPressedBorder"));
	m_arrColor[XPCOLOR_HIGHLIGHT_PUSHED] = pImages->GetImageColor(_T("Window"), _T("HighlightPressed"));
	m_arrColor[XPCOLOR_HIGHLIGHT_TEXT] = 0;
	m_arrColor[XPCOLOR_PUSHED_TEXT] = 0;

	m_bLunaTheme = FALSE;

	m_clrSplitterGradient.SetStandardValue(pImages->GetImageColor(_T("DockingPane"), _T("WordSplitter")));
	m_clrSplitter.SetStandardValue(m_clrSplitterGradient);

	m_clrNormalCaption.SetStandardValue(pImages->GetImageColor(_T("DockingPane"), _T("WordNormalCaption")));
	m_clrNormalCaptionText.SetStandardValue(pImages->GetImageColor(_T("DockingPane"), _T("WordCaptionText")));

	m_clrActiveCaption.SetStandardValue(pImages->GetImageColor(_T("DockingPane"), _T("WordActiveCaption")));
	m_clrActiveCaptionText.SetStandardValue(pImages->GetImageColor(_T("DockingPane"), _T("WordCaptionText")));
	m_clrSplitterGripper.SetStandardValue(m_clrNormalCaptionText);

	if (m_bOffice2010Style)
	{
		m_nSplitterSize = 4;
		m_nSplitterIndent = 0;
	}
	else
	{
		m_nSplitterSize = 0;
		m_nSplitterIndent = 2;
	}

	if (m_bOffice2010Style)
	{
		m_arrColor[XPCOLOR_MENUBAR_BORDER] = m_arrColor[XPCOLOR_TOOLBAR_GRIPPER];
	}
}


///////////////////////////////////////////////////////////////////////////////
// CXTPDockingPaneOffice2003OutlookTheme


CXTPDockingPaneOffice2003OutlookTheme::CXTPDockingPaneOffice2003OutlookTheme()
{
	m_bDrawGripper = FALSE;
	m_bRoundedCaption = FALSE;
	m_nCaptionFontGap = 3;
}

CXTPDockingPaneOffice2003OutlookTheme::~CXTPDockingPaneOffice2003OutlookTheme()
{
}


void CXTPDockingPaneOffice2003OutlookTheme::RefreshMetrics()
{
	CXTPDockingPaneOffice2003Theme::RefreshMetrics();

	if (m_bUseStandardFont)
	{
		LOGFONT lf;
		VERIFY(CXTPDrawHelpers::GetIconLogFont(&lf));

		STRCPY_S(lf.lfFaceName, LF_FACESIZE, _T("Arial"));
		lf.lfWeight = FW_BOLD;
		lf.lfHeight = 20;

		SetCaptionFontIndirect(&lf);
	}

	if (m_bLunaTheme)
	{
		m_clrNormalCaption.SetStandardValue(XTPColorManager()->grcCaption);
		m_clrNormalCaptionText.SetStandardValue(RGB(255, 255, 255));
	}
	else
	{
		m_clrNormalCaption.SetStandardValue(XTPColorManager()->LightColor(
			GetXtremeColor(COLOR_3DSHADOW), GetXtremeColor(COLOR_WINDOW), 0xcd),
			GetXtremeColor(COLOR_3DSHADOW));
	}
}

CRect CXTPDockingPaneOffice2003OutlookTheme::GetCaptionGripperRect(const CXTPDockingPaneTabbedContainer* /*pPane*/)
{
	return CRect(0, 0, 0, 0);
}


//////////////////////////////////////////////////////////////////////////
///

CXTPDockingPaneWinNativeTheme::CXTPDockingPaneWinNativeTheme()
{
	m_pPanelPaintManager->SetAppearance(xtpTabAppearancePropertyPage);
	m_pPanelPaintManager->SetColor(xtpTabColorWinNative);
	m_pPanelPaintManager->m_bHotTracking = TRUE;

	m_pTabPaintManager->SetAppearance(xtpTabAppearancePropertyPage);
	m_pTabPaintManager->SetColor(xtpTabColorWinNative);
	m_pTabPaintManager->m_bHotTracking = TRUE;

	m_bGradientCaption = TRUE;
	m_nCaptionFontGap = 7;

	m_nCaptionButtonStyle = xtpPaneCaptionButtonThemedExplorerBar;
}

void CXTPDockingPaneWinNativeTheme::RefreshMetrics()
{
	CXTPDockingPaneVisualStudio2003Theme::RefreshMetrics();

	if (m_themeWindow->IsAppThemed())
	{
		m_clrActiveCaptionText.SetStandardValue(GetXtremeColor(COLOR_CAPTIONTEXT));
		m_clrNormalCaptionText.SetStandardValue(GetXtremeColor(COLOR_INACTIVECAPTIONTEXT));
	}
}

void CXTPDockingPaneWinNativeTheme::DrawFloatingFrame(CDC* pDC, CXTPDockingPaneMiniWnd* pPane, CRect rc)
{
	if (!m_themeWindow->IsAppThemed())
	{
		CXTPDockingPaneVisualStudio2003Theme::DrawFloatingFrame(pDC, pPane, rc);
		return;
	}
	BOOL bActive = m_bHighlightActiveCaption && pPane->IsActive();

	int nState = bActive ? CS_ACTIVE : CS_INACTIVE;

	m_themeWindow->DrawThemeBackground(pDC->GetSafeHdc(), WP_SMALLFRAMELEFT, nState, CRect(rc.left, rc.top, rc.left + 3, rc.bottom), 0);
	m_themeWindow->DrawThemeBackground(pDC->GetSafeHdc(), WP_SMALLFRAMERIGHT, nState, CRect(rc.right - 3, rc.top, rc.right, rc.bottom), 0);
	m_themeWindow->DrawThemeBackground(pDC->GetSafeHdc(), WP_SMALLFRAMEBOTTOM, nState, CRect(rc.left, rc.bottom - 3, rc.right, rc.bottom), 0);

	int nTitleHeight = m_nTitleHeight;
	CRect rcCaption(rc);
	rcCaption.DeflateRect(0, 0, 0, rc.Height() - nTitleHeight - 3 - 2);

	CXTPBufferDC dcCache(*pDC, rcCaption);
	dcCache.SetBkMode(TRANSPARENT);

	m_themeWindow->DrawThemeBackground(dcCache, WP_SMALLCAPTION, nState, rcCaption, 0);

	rcCaption.DeflateRect(3, 0);

	CString strTitle;
	pPane->GetWindowText(strTitle);

	DrawCaptionPart(&dcCache, pPane, rcCaption, strTitle, bActive, FALSE);
}

void CXTPDockingPaneWinNativeTheme::DrawSidePanel(CDC* pDC, CXTPDockingPaneSidePanel* pPane, CRect rc)
{
	CXTPDockingPaneVisualStudio2003Theme::DrawSidePanel(pDC, pPane, rc);
}

void CXTPDockingPaneWinNativeTheme::DrawCaptionPart(CDC* pDC, CXTPDockingPaneBase* pPane, CRect rcCaption, const CString& strTitle, BOOL bActive, BOOL bVertical)
{
	if (!m_themeWindow->IsAppThemed())
	{
		CXTPDockingPaneVisualStudio2003Theme::DrawCaptionPart(pDC, pPane, rcCaption, strTitle, bActive, bVertical);
		return;
	}

	COLORREF clrText = bActive ? m_clrActiveCaptionText : m_clrNormalCaptionText;
	COLORREF clrButton = clrText;

	if (pPane->GetType() == xtpPaneTypeSidePanel)
	{
		clrButton = FillCaptionPart(pDC, pPane, rcCaption, bActive, bVertical);
	}

	XTPDeflateRectDirection(rcCaption, 0, 0, 2, 0, bVertical);
	DrawCaptionButtons(pDC, pPane->GetCaptionButtons(), rcCaption, clrButton, 16, 3, bVertical);

	pDC->SetTextColor(IsCaptionEnabled(pPane) ? clrText : m_clrDisabledCaptionText);
	XTPDeflateRectDirection(rcCaption, 4, 1, 0, 0, bVertical);

	DrawCaptionIcon(pDC, pPane, rcCaption, bVertical);
	DrawCaptionText(pDC, strTitle, rcCaption, bVertical);
}

void CXTPDockingPaneWinNativeTheme::DrawCaption(CDC* pDC, CXTPDockingPaneTabbedContainer* pPane, CRect rc)
{
	if (!m_themeWindow->IsAppThemed())
	{
		CXTPDockingPaneVisualStudio2003Theme::DrawCaption(pDC, pPane, rc);
		return;
	}

	BOOL bVertical = pPane->IsCaptionVertical();
	BOOL bActive = m_bHighlightActiveCaption && pPane->IsActive();

	CRect rcCaption(0, 0, rc.right, m_nTitleHeight + 1);
	if (bVertical)
	{
		rcCaption = CRect(0, 0, m_nTitleHeight + 1, rc.bottom);
	}

	m_themeWindow->DrawThemeBackground(pDC->GetSafeHdc(), WP_SMALLCAPTION, bActive ? CS_ACTIVE : CS_INACTIVE, rcCaption, 0);

	CString strTitle = pPane->GetTitle();
	DrawCaptionPart(pDC, pPane, rcCaption, strTitle, bActive, bVertical);
}


//////////////////////////////////////////////////////////////////////////
// CXTPDockingPaneVisualStudio2005Beta1Theme

class CXTPDockingPaneVisualStudio2005Beta1Theme::CColorSetVisualStudio2005 : public CXTPTabPaintManager::CColorSetOffice2003
{
public:
	CColorSetVisualStudio2005(BOOL bHighlightSelected)
	{
		m_bHighlightSelected = bHighlightSelected;
		m_bTransparentButton = FALSE;
		m_bGradientButton = FALSE;

	}
	void RefreshMetrics();

	void XTPFillFramePartRect(CDC* pDC, CRect rc, CWnd* pWnd, CWnd* pSite)
	{
		::XTPFillFramePartRect(pDC, rc, pWnd, pSite, m_clrHeaderFace, TRUE);
	}

	void FillHeader(CDC* pDC, CRect rc, CXTPTabManager* pTabManager)
	{
		XTPFillFramePartRect(pDC, rc, (CXTPDockingPaneTabbedContainer*)pTabManager, ((CXTPDockingPaneTabbedContainer*)pTabManager)->GetDockingSite());
	}

	COLORREF FillPropertyButton(CDC* pDC, CRect rcItem, CXTPTabManagerItem* pItem)
	{
		if (m_bTransparentButton && (!m_bHighlightSelected || !pItem->IsSelected()))
			return COLORREF_NULL;

		if (m_bGradientButton)
			return CColorSetOffice2003::FillPropertyButton(pDC, rcItem, pItem);

		return CXTPTabPaintManagerColorSet::FillPropertyButton(pDC, rcItem, pItem);

	}
protected:
	BOOL m_bGradientButton;
	BOOL m_bTransparentButton;
	BOOL m_bHighlightSelected;

};

void CXTPDockingPaneVisualStudio2005Beta1Theme::CColorSetVisualStudio2005::RefreshMetrics()
{
	CXTPTabPaintManagerColorSet ::RefreshMetrics();

	if (m_bHighlightSelected)
	{
		m_clrButtonSelected.SetStandardValue(XTPColorManager()->LightColor(
			GetSysColor(COLOR_HIGHLIGHT), GetSysColor(COLOR_WINDOW), 0x02));

		m_clrSelectedText.SetStandardValue(GetSysColor(COLOR_WINDOWTEXT));
	}
	else
	{
		m_clrSelectedText.SetStandardValue(GetSysColor(COLOR_3DDKSHADOW));
	}
	m_clrHeaderFace.SetStandardValue(GetSysColor(COLOR_3DFACE), XTPColorManager()->LightColor(GetSysColor(COLOR_3DFACE), GetSysColor(COLOR_WINDOW), 0xcd));
	m_clrNormalText.SetStandardValue(GetSysColor(COLOR_3DDKSHADOW));

	m_bGradientButton = FALSE;
	m_bLunaTheme = FALSE;

	switch (XTPColorManager()->GetCurrentSystemTheme())
	{
	case xtpSystemThemeSilver:
		m_bGradientButton = TRUE;
		m_clrButtonNormal.SetStandardValue(RGB(242, 242, 247));

		m_csPropertyPage2003.clrBorderTopHighlight.SetStandardValue(RGB(255, 255, 255));
		m_csPropertyPage2003.clrDarkShadow.SetStandardValue(RGB(145, 155, 156));
		m_csPropertyPage2003.clrFrameBorder.SetStandardValue(RGB(145, 155, 156));
		m_csPropertyPage2003.clrBorderLeftHighlight.SetStandardValue(RGB(190, 190, 216));

		m_clrHeaderFace.SetStandardValue(RGB(215, 215, 229), RGB(243, 243, 247));
		break;

	case xtpSystemThemeBlue:
		m_bGradientButton = TRUE;
		m_csPropertyPage2003.clrBorderTopHighlight.SetStandardValue(RGB(255, 255, 255));

		m_clrHeaderFace.SetStandardValue(RGB(229, 229, 215), RGB(244, 241, 231));
		break;

	case xtpSystemThemeOlive:
		m_bGradientButton = TRUE;
		m_csPropertyPage2003.clrBorderTopHighlight.SetStandardValue(RGB(255, 255, 255));

		m_clrHeaderFace.SetStandardValue(RGB(229, 229, 215), RGB(244, 241, 231));
		break;
	}

	m_clrAutoHideFace.SetStandardValue(m_clrHeaderFace);
}

class CXTPDockingPaneVisualStudio2005Beta1Theme::CColorSetVisualStudio2003 : public CColorSetVisualStudio2005
{
public:
	CColorSetVisualStudio2003(BOOL bHighlightSelected)
		: CColorSetVisualStudio2005(bHighlightSelected)
	{
		m_bTransparentButton = TRUE;
	}

	void RefreshMetrics();
};

void CXTPDockingPaneVisualStudio2005Beta1Theme::CColorSetVisualStudio2003::RefreshMetrics()
{
	CColorSetVisualStudio2005::RefreshMetrics();

	m_bGradientButton = FALSE;
	m_bLunaTheme = FALSE;

	switch (XTPColorManager()->GetCurrentSystemTheme())
	{
	case xtpSystemThemeSilver:
	case xtpSystemThemeBlue:
	case xtpSystemThemeOlive:
		m_clrNormalText.SetStandardValue(RGB(113, 111, 100));
		if (!m_bHighlightSelected) m_clrSelectedText.SetStandardValue(RGB(113, 111, 100));
		break;
	}
}


CXTPDockingPaneVisualStudio2005Beta1Theme::CXTPDockingPaneVisualStudio2005Beta1Theme()
{
	CXTPTabPaintManager::CAppearanceSetPropertyPageFlat* pAppearanceFlat = (CXTPTabPaintManager::CAppearanceSetPropertyPageFlat*)
		m_pTabPaintManager->SetAppearance(xtpTabAppearancePropertyPageFlat);

	m_pTabPaintManager->m_bDisableLunaColors = TRUE;
	pAppearanceFlat->m_bBlurPoints = FALSE;
	m_pTabPaintManager->SetColorSet(new CColorSetVisualStudio2005(TRUE));
	m_pTabPaintManager->m_bHotTracking = FALSE;
	m_pTabPaintManager->m_clientFrame = xtpTabFrameNone;

	m_pPanelPaintManager->m_bDisableLunaColors = TRUE;
	pAppearanceFlat = (CXTPTabPaintManager::CAppearanceSetPropertyPageFlat*)m_pPanelPaintManager->SetAppearance(xtpTabAppearancePropertyPageFlat);
	pAppearanceFlat->m_bBlurPoints = FALSE;
	m_pPanelPaintManager->SetColorSet(new CColorSetVisualStudio2005(FALSE));
	m_pPanelPaintManager->m_bHotTracking = FALSE;
	m_pPanelPaintManager->SetLayout(xtpTabLayoutAutoSize);

	m_rcCaptionMargin.SetRect(0, 0, 0, 0);
	m_bGradientCaption = TRUE;
	m_bUseBoldCaptionFont = FALSE;
	m_dwSplitterStyle = xtpPaneSplitterGradient;

	m_systemTheme = xtpSystemThemeUnknown;
}

void CXTPDockingPaneVisualStudio2005Beta1Theme::RefreshMetrics()
{
	CXTPDockingPaneVisualStudio2003Theme::RefreshMetrics();

	m_clrNormalCaptionText.SetStandardValue(GetXtremeColor(COLOR_INACTIVECAPTIONTEXT));

	switch (XTPColorManager()->GetCurrentSystemTheme())
	{
	case xtpSystemThemeBlue:
		m_clrSplitterGradient.SetStandardValue(RGB(229, 229, 215), RGB(244, 241, 231));
		m_clrNormalCaption.SetStandardValue(RGB(216, 215, 198), RGB(238, 239, 229));
		m_clrNormalCaptionText.SetStandardValue(0);
		break;

	case xtpSystemThemeOlive:
		m_clrSplitterGradient.SetStandardValue(RGB(229, 229, 215), RGB(244, 241, 231));
		m_clrNormalCaption.SetStandardValue(RGB(216, 215, 198), RGB(238, 239, 229));
		m_clrNormalCaptionText.SetStandardValue(0);
		break;

	case xtpSystemThemeSilver:
		m_clrSplitterGradient.SetStandardValue(RGB(215, 215, 229), RGB(243, 243, 247));
		m_clrNormalCaption.SetStandardValue(RGB(224, 224, 235), RGB(242, 242, 246));
		m_clrActiveCaption.SetStandardValue(RGB(160, 158, 186), RGB(224, 225, 235));
		m_clrNormalCaptionText.SetStandardValue(0);
		break;
	}

}
CXTPDockingPaneVisualStudio2005Beta2Theme::CXTPDockingPaneVisualStudio2005Beta2Theme()
{
	m_pTabPaintManager->SetAppearance(xtpTabAppearanceVisualStudio2005);
	m_pTabPaintManager->SetColorSet(new CColorSetVisualStudio2005(TRUE));

	m_pPanelPaintManager->SetAppearance(xtpTabAppearanceVisualStudio2005);
	m_pPanelPaintManager->SetColorSet(new CColorSetVisualStudio2005(TRUE));
}

CXTPDockingPaneVisualStudio2005Theme::CXTPDockingPaneVisualStudio2005Theme()
{
	CXTPTabPaintManager::CAppearanceSetPropertyPageFlat* pAppearance =
		new CXTPTabPaintManager::CAppearanceSetPropertyPageFlat;
	pAppearance->m_bBlurPoints = FALSE;

	m_pPanelPaintManager->SetAppearanceSet(pAppearance);
	m_pPanelPaintManager->SetColorSet(new CColorSetVisualStudio2003(FALSE));
	m_pPanelPaintManager->m_bRotateImageOnVerticalDraw = TRUE;


	pAppearance = new CXTPTabPaintManager::CAppearanceSetPropertyPageFlat;
	pAppearance->m_bBlurPoints = FALSE;
	pAppearance->m_bVisualStudio2005Style = TRUE;

	m_pTabPaintManager->SetAppearanceSet(pAppearance);
	m_pTabPaintManager->SetColorSet(new CColorSetVisualStudio2003(TRUE));

	m_pTabPaintManager->m_clientFrame = xtpTabFrameSingleLine;
	m_pTabPaintManager->m_rcClientMargin.top = 1;

}



void CXTPDockingPaneVisualStudio2005Beta2Theme::RefreshMetrics()
{
	CXTPDockingPaneVisualStudio2005Beta1Theme::RefreshMetrics();

	m_clrNormalCaption.SetStandardValue(GetXtremeColor(COLOR_INACTIVECAPTION));
	m_clrActiveCaption.SetStandardValue(GetXtremeColor(COLOR_ACTIVECAPTION));
	m_clrCaptionBorder = GetXtremeColor(COLOR_3DSHADOW);

	switch (XTPColorManager()->GetCurrentSystemTheme())
	{
	case xtpSystemThemeBlue:
		m_clrNormalCaption.SetStandardValue(RGB(204, 199, 186));
		m_clrActiveCaption.SetStandardValue(RGB(59, 128, 237), RGB(49, 106, 197));
		m_clrNormalCaptionText.SetStandardValue(0);
		break;
	case xtpSystemThemeOlive:
		m_clrNormalCaption.SetStandardValue(RGB(204, 199, 186));
		m_clrActiveCaption.SetStandardValue(RGB(182, 195, 146), RGB(145, 160, 117));
		m_clrNormalCaptionText.SetStandardValue(0);
		break;
	case xtpSystemThemeSilver:
		m_clrNormalCaption.SetStandardValue(RGB(240, 240, 245));
		m_clrActiveCaption.SetStandardValue(RGB(211, 212, 221), RGB(166, 165, 191));
		m_clrNormalCaptionText.SetStandardValue(0);
		m_clrCaptionBorder = RGB(145, 155, 156);
		break;
	}
}

COLORREF CXTPDockingPaneVisualStudio2005Beta2Theme::FillCaptionPart(CDC* pDC, CXTPDockingPaneBase* pPane, CRect rcCaption, BOOL bActive, BOOL bVertical)
{
	XTPDrawHelpers()->GradientFill(pDC, rcCaption, bActive ? m_clrActiveCaption : m_clrNormalCaption, bVertical);

	if (pPane->GetType() == xtpPaneTypeSidePanel)
		return bActive ? m_clrActiveCaptionText : m_clrNormalCaptionText;

	CXTPPenDC pen(*pDC, m_clrCaptionBorder);
	if (bVertical)
	{
		pDC->MoveTo(rcCaption.right, rcCaption.bottom - 1);
		pDC->LineTo(rcCaption.left , rcCaption.bottom - 1);
		pDC->LineTo(rcCaption.left, rcCaption.top);
		pDC->LineTo(rcCaption.right, rcCaption.top);

	}
	else
	{
		pDC->MoveTo(rcCaption.left, rcCaption.bottom);
		pDC->LineTo(rcCaption.left , rcCaption.top);
		pDC->LineTo(rcCaption.right - 1, rcCaption.top);
		pDC->LineTo(rcCaption.right - 1, rcCaption.bottom - 1);
	}

	return bActive ? m_clrActiveCaptionText : m_clrNormalCaptionText;
}

//////////////////////////////////////////////////////////////////////////
// CXTPDockingPaneVisualStudio2008Theme

CXTPDockingPaneVisualStudio2008Theme::CXTPDockingPaneVisualStudio2008Theme()
{

}

void CXTPDockingPaneVisualStudio2008Theme::RefreshMetrics()
{
	CXTPDockingPaneVisualStudio2005Theme::RefreshMetrics();

	if (m_themeRebar->IsAppThemed())
	{
		m_pPanelPaintManager->SetAppearance(xtpTabAppearancePropertyPage);
		m_pPanelPaintManager->SetColor(xtpTabColorWinNative);
		m_pPanelPaintManager->m_bHotTracking = TRUE;

		m_pTabPaintManager->SetAppearance(xtpTabAppearancePropertyPage);
		m_pTabPaintManager->SetColor(xtpTabColorWinNative);
		m_pTabPaintManager->m_bHotTracking = TRUE;
	}
}

//////////////////////////////////////////////////////////////////////////
// CXTPDockingPaneVisualStudio2010Theme

CXTPDockingPaneVisualStudio2010Theme::CXTPDockingPaneVisualStudio2010Theme()
{
	m_nCaptionFontGap = 6;
	m_nCaptionButtonSize = 15;
	m_bAutoHidePanelHighlightSelected = FALSE;


	CXTPTabPaintManager::CAppearanceSetVisualStudio2010* pAppearance =
		new CXTPTabPaintManager::CAppearanceSetVisualStudio2010();
	pAppearance->m_rcHeaderMargin.left = 0;
	pAppearance->m_rcHeaderMargin.right = 0;

	m_pTabPaintManager->SetAppearanceSet(pAppearance);
	m_pTabPaintManager->SetColorSet(new CXTPTabPaintManager::CColorSetVisualStudio2010(TRUE));

	m_pTabPaintManager->m_clientFrame = xtpTabFrameSingleLine;
	m_pTabPaintManager->m_rcClientMargin.top = 1;


	pAppearance =
		new CXTPTabPaintManager::CAppearanceSetVisualStudio2010();
	pAppearance->m_bDrawNormalTab = TRUE;
	m_pPanelPaintManager->SetAppearanceSet(pAppearance);

	pAppearance->m_rcHeaderMargin.left = 0;
	m_pPanelPaintManager->SetColor(xtpTabColorVisualStudio2010);

	m_pTabPaintManager->m_bHotTracking = TRUE;
	m_pPanelPaintManager->m_bHotTracking = TRUE;
	m_nCaptionButtonStyle = xtpPaneCaptionButtonOffice2003;

}

void CXTPDockingPaneVisualStudio2010Theme::RefreshMetrics()
{
	CXTPDockingPaneVisualStudio2005Theme::RefreshMetrics();

	m_bLunaTheme = XTPSystemVersion()->IsWinVistaOrGreater();

	COLORREF clrBackground = m_pTabPaintManager->GetColorSet()->m_clrAutoHideFace;

	m_clrNormalCaption.SetStandardValue(RGB(77,96,130), RGB(61, 82, 119));
	m_clrActiveCaption.SetStandardValue(RGB(255, 232, 166));
	m_clrCaptionBorder = clrBackground;
	m_clrSplitter.SetStandardValue(clrBackground);
	m_clrSplitterGradient.SetStandardValue(clrBackground);

	m_clrNormalCaptionText.SetStandardValue(RGB(255, 255, 255));
	m_clrActiveCaptionText.SetStandardValue(0);

	if (!m_bLunaTheme)
	{
		m_clrNormalCaption.SetStandardValue(RGB(74, 97, 132));
		m_clrActiveCaption.SetStandardValue(RGB(255, 243, 206));
	}

	m_arrColor[XPCOLOR_3DFACE] = clrBackground;

	m_arrColor[XPCOLOR_HIGHLIGHT] = RGB(255, 255, 247);
	m_arrColor[XPCOLOR_HIGHLIGHT_BORDER] = RGB(231, 195, 99);
	m_arrColor[XPCOLOR_HIGHLIGHT_PUSHED] = RGB(255, 235, 165);
	m_arrColor[XPCOLOR_HIGHLIGHT_PUSHED_BORDER] = RGB(231, 195, 99);
	m_arrColor[XPCOLOR_PUSHED_TEXT] = RGB(0, 0, 0);
	m_arrColor[XPCOLOR_HIGHLIGHT_TEXT] = RGB(0, 0, 0);
}

void CXTPDockingPaneVisualStudio2010Theme::DrawFloatingFrame(CDC* pDC, CXTPDockingPaneMiniWnd* pPane, CRect rc)
{
	pDC->Draw3dRect(rc, m_clrCaptionBorder, m_clrCaptionBorder);
	rc.DeflateRect(1, 1);
	pDC->Draw3dRect(rc, m_clrCaptionBorder, m_clrCaptionBorder);
	rc.DeflateRect(1, 1);
	pDC->Draw3dRect(rc, m_clrCaptionBorder, m_clrCaptionBorder);

	int nTitleHeight = m_nTitleHeight;
	CRect rcCaption(rc);
	rcCaption.DeflateRect(0, 0, 0, rc.Height() - nTitleHeight - 3);

	CXTPBufferDC dcCache(*pDC, rcCaption);
	dcCache.SetBkMode(TRANSPARENT);

	dcCache.FillSolidRect(rcCaption, m_clrCaptionBorder);

	CString strTitle;
	pPane->GetWindowText(strTitle);

	rcCaption.DeflateRect(1, 2, 1, 0);

	BOOL bActive = m_bHighlightActiveCaption && pPane->IsActive();
	DrawCaptionPart(&dcCache, pPane, rcCaption, strTitle, bActive, FALSE);
}

COLORREF CXTPDockingPaneVisualStudio2010Theme::FillCaptionPart(CDC* pDC, CXTPDockingPaneBase* /*pPane*/, CRect rcCaption, BOOL bActive, BOOL bVertical)
{
	CXTPPaintManagerColorGradient& clr = bActive ? m_clrActiveCaption : m_clrNormalCaption;
	XTPDrawHelpers()->GradientFill(pDC, rcCaption, clr, bVertical);

	if (bActive)
	{
		CRect rTop(rcCaption);
		if (!bVertical) rTop.bottom = rTop.top + rTop.Height() / 2;
		else rTop.right = rTop.left + rTop.Width() / 2;

		XTPDrawHelpers()->GradientFill(pDC, rTop, RGB(255, 252, 242), RGB(255, 243, 207), bVertical);

	}

	if (!bVertical)
	{
		pDC->SetPixel(rcCaption.left, rcCaption.top, CXTPDrawHelpers::BlendColors(clr, m_clrCaptionBorder, (double)60 / 255));
		pDC->SetPixel(rcCaption.left + 1, rcCaption.top, CXTPDrawHelpers::BlendColors(clr, m_clrCaptionBorder, (double)120 / 255));
		pDC->SetPixel(rcCaption.left, rcCaption.top + 1, CXTPDrawHelpers::BlendColors(clr, m_clrCaptionBorder, (double)120 / 255));

		pDC->SetPixel(rcCaption.right - 1, rcCaption.top, CXTPDrawHelpers::BlendColors(clr, m_clrCaptionBorder, (double)60 / 255));
		pDC->SetPixel(rcCaption.right - 2, rcCaption.top, CXTPDrawHelpers::BlendColors(clr, m_clrCaptionBorder, (double)120 / 255));
		pDC->SetPixel(rcCaption.right - 1, rcCaption.top + 1, CXTPDrawHelpers::BlendColors(clr, m_clrCaptionBorder, (double)120 / 255));
	}

	return bActive ? RGB(115, 97, 57) : RGB(206, 215, 222);
}

void CXTPDockingPaneVisualStudio2010Theme::DrawCaptionButtonEntry(CDC* pDC, CXTPDockingPaneCaptionButton* pButton, CPoint pt, COLORREF clrButton)
{
	if (pButton->GetID() == XTP_IDS_DOCKINGPANE_AUTOHIDE)
	{
		CXTPPenDC pen(*pDC, clrButton);
		pt.y--;

		if (pButton->GetState() & xtpPanePinPushed)
		{
			pDC->MoveTo(pt.x - 5, pt.y); pDC->LineTo(pt.x - 1, pt.y);
			pDC->MoveTo(pt.x - 1, pt.y - 3); pDC->LineTo(pt.x - 1, pt.y + 4);
			pDC->MoveTo(pt.x - 1, pt.y + 2); pDC->LineTo(pt.x + 5, pt.y + 2);
			pDC->LineTo(pt.x + 5, pt.y - 2); pDC->LineTo(pt.x - 1, pt.y - 2);
			pDC->MoveTo(pt.x - 1, pt.y + 1); pDC->LineTo(pt.x + 5, pt.y + 1);
		}
		else
		{
			pt.y += 2;
			pDC->MoveTo(pt.x - 3, pt.y - 1 + 1); pDC->LineTo(pt.x + 4, pt.y - 1 + 1);
			pDC->MoveTo(pt.x, pt.y - 1 + 1); pDC->LineTo(pt.x, pt.y + 3 + 2);
			pDC->MoveTo(pt.x - 2, pt.y - 1 + 1); pDC->LineTo(pt.x - 2, pt.y - 6);
			pDC->LineTo(pt.x + 2, pt.y - 6); pDC->LineTo(pt.x + 2, pt.y - 1 + 1);
			pDC->MoveTo(pt.x + 1, pt.y - 1 + 1); pDC->LineTo(pt.x + 1, pt.y - 6);
		}
	}
	else
	{
		CXTPDockingPaneVisualStudio2008Theme::DrawCaptionButtonEntry(pDC, pButton, pt, clrButton);
	}

}

