// XTPRibbonOffice2013Theme.cpp : implementation file
//
// This file is a part of the XTREME RIBBON MFC class library.
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
#include <GraphicLibrary/GdiPlus/GdiPlus.h>
#include <Common/XTPMarkupTheme.h>
#include <Common/XTPResource.h>
#include <Common/XTPResourceImage.h>
#include "Common/XTPResourceManager.h"
#include <Common/XTPWinThemeWrapper.h>
#include <Common/XTPImageManager.h>
#include <Common/XTPHookManager.h>
#include <Common/XTPMarkupRender.h>
#include <Common/XTPColorManager.h>
#include <Common/XTPSystemHelpers.h>
#include <Common/XTPDrawHelpers.h>
#include <Common/XTPSystemMetrics.h>
#include <Common/ScrollBar/XTPScrollInfo.h>
#include <Common/Hook/XTPWinEventHook.h>

#include <TabManager/Includes.h>

#include <CommandBars/XTPCommandBarsDefines.h>
#include <CommandBars/XTPCommandBar.h>
#include <CommandBars/XTPCommandBars.h>
#include <CommandBars/XTPToolBar.h>
#include <CommandBars/XTPMenuBar.h>
#include <CommandBars/XTPPopupBar.h>
#include <CommandBars/XTPControls.h>
#include <CommandBars/XTPControl.h>
#include <CommandBars/XTPControlButton.h>
#include <CommandBars/XTPControlPopup.h>
#include <CommandBars/XTPPaintManager.h>
#include <CommandBars/Frame/XTPFrameHook.h>
#include <CommandBars/Frame/XTPFramePaintManager.h>
#include <CommandBars/Themes/XTPCommandBarsOfficeXPTheme.h>
#include <CommandBars/Themes/ModernUI/XTPCommandBarsModernUITheme.h>

#include <Ribbon/XTPRibbonGroup.h>
#include <Ribbon/XTPRibbonPopups.h>
#include <Ribbon/XTPRibbonBar.h>
#include <Ribbon/XTPRibbonTab.h>
#include <Ribbon/XTPRibbonControlTab.h>
#include <Ribbon/XTPRibbonPaintManager.h>
#include <Ribbon/XTPRibbonSystemButton.h>
#include <Ribbon/XTPRibbonMarkupTheme.h>

#include <Markup/XTPMarkupObject.h>
#include <Markup/XTPMarkupInputElement.h>
#include <Markup/XTPMarkupUIElement.h>
#include <Markup/XTPMarkupFrameworkElement.h>
#include <Markup/XTPMarkupTextBlock.h>

#include <Ribbon/Themes/XTPRibbonThemeOffice2013.h>

#ifdef _XTP_ACTIVEX
	#ifndef _WIN64
	#pragma comment(lib, "GdiPlus.lib")
	#else
	#pragma comment(lib, "..\\..\\..\\Source\\GraphicLibrary\\GdiPlus\\AMD64\\GdiPlus.lib")
	#endif
#else
	#pragma comment(lib, "GdiPlus.lib")
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

struct XTPRibbonHeaderColor
{
	COLORREF clrBorder;
	COLORREF clrBackground;
	COLORREF clrText;
};

XTPRibbonHeaderColor headerColors[] =
{
	{RGB(255, 255, 255), RGB(255, 255, 255), RGB(102, 102, 102)}, // xtpRibbonTabContextColorNone
	{RGB( 43,  87, 154), RGB(213, 225, 242), RGB( 62, 109, 181)}, // xtpRibbonTabContextColorBlue
	{RGB(242, 203,  29), RGB(255, 251, 230), RGB(152, 119,  21)}, // xtpRibbonTabContextColorYellow
	{RGB( 73, 163,  73), RGB(230, 243, 230), RGB( 73, 138,  73)}, // xtpRibbonTabContextColorGreen
	{RGB(226, 101, 102), RGB(248, 212, 212), RGB(202,  90,  92)}, // xtpRibbonTabContextColorRed
	{RGB(201,  89, 156), RGB(252, 240, 247), RGB(161,  64, 122)}, // xtpRibbonTabContextColorPurple
	{RGB(153, 200, 233), RGB(205, 230, 247), RGB(  1,  89, 138)}, // xtpRibbonTabContextColorCyan
	{RGB(255, 157,   0), RGB(255, 248, 237), RGB(207,  92,  10)}, // xtpRibbonTabContextColorOrange
};

static Gdiplus::GdiplusStartupOutput gdiplusStartupOutput;

CXTPRibbonOffice2013Theme::CXTPRibbonOffice2013Theme(CXTPPaintManager *pPaintManager)
	: CXTPRibbonMarkupTheme(pPaintManager)
	, m_GdiPlusToken(0)
{
	Gdiplus::GdiplusStartupInput startupInfo;
	startupInfo.SuppressBackgroundThread = TRUE;
	if(Gdiplus::Ok != Gdiplus::GdiplusStartup(&m_GdiPlusToken, &startupInfo, &gdiplusStartupOutput))
	{
		m_GdiPlusToken = (ULONG_PTR)-1;
	}
	else
	{
		gdiplusStartupOutput.NotificationHook(&m_GdiPlusToken);
	}
}

CXTPRibbonOffice2013Theme::~CXTPRibbonOffice2013Theme()
{
	if(IsGdiplusInitialized())
	{
		// Termination of background thread, which is causing the shutdown problem.
		gdiplusStartupOutput.NotificationUnhook(m_GdiPlusToken);
		Gdiplus::GdiplusShutdown(m_GdiPlusToken);
	}
}

BOOL CXTPRibbonOffice2013Theme::IsGdiplusInitialized() const
{
	return (ULONG_PTR)-1 != m_GdiPlusToken;
}

void CXTPRibbonOffice2013Theme::RefreshMetrics()
{
	CXTPRibbonMarkupTheme::RefreshMetrics();

	CXTPResourceImages* pImages = GetImages();

	LPCTSTR lpszKey = _T("Ribbon");

	// Office 2013
	m_clrRibbonFace                  = RGB(255, 255, 255);
	m_clrMinimizedFrameEdgeShadow    = RGB(255, 255, 255);
	m_clrMinimizedFrameEdgeHighLight = RGB(255, 255, 255);

	m_clrControlGroupOption          = RGB(119, 119, 119);
	m_clrGroupCaption                = RGB(119, 119, 119);


	CXTPTabPaintManagerColorSet *pColorSet = m_pTabPaintManager->GetColorSet();
//  m_pTabPaintManager->m_bBoldSelected = TRUE;
	pColorSet->m_clrTextNormal    = pImages->GetImageColor(lpszKey, _T("TabTextNormal"));
	pColorSet->m_clrTextHighlight = pImages->GetImageColor(lpszKey, _T("TabTextHighlighted"));
	pColorSet->m_clrTextSelected  = pImages->GetImageColor(lpszKey, _T("TabTextSelected"));

	HMODULE hModule = XTPResourceImages()->GetHandle();

	if (NULL == hModule)
		hModule = XTPGetInstanceHandle();

	CXTPResource resource;
	resource.LoadResource(hModule, _T("XTP_IDR_OFFICE2013_STYLE"), _T("STYLE"));

	if (NULL != resource.m_hResource)
	{
		HZIP hZip = OpenZip(resource.m_pvResourceData, resource.m_dwSize, NULL);
		LoadParts(hZip, ((CXTPCommandBarsModernUITheme*)m_pPaintManager)->GetMarkupContext());
		CloseZip(hZip);
	}
}

int CXTPRibbonOffice2013Theme::GetRibbonCaptionHeight(const CXTPRibbonBar *pRibbonBar) const
{
	if (pRibbonBar)
	{
		CXTPCommandBarsFrameHook* pFrameHook = pRibbonBar->GetFrameHook();
		if (pFrameHook && pFrameHook->IsDwmEnabled())
		{
			return XTPSystemMetrics()->m_nCaptionY;
		}
	}

	return 28;
}


void CXTPRibbonOffice2013Theme::DrawRibbonTab(CDC *pDC, CXTPRibbonTab *pItem)
{
	XTPRibbonTabContextColor tabColor = pItem->GetContextColor();
	XTPTabPosition tabPosition = pItem->GetTabManager()->GetPosition();

	CRect rcItem(pItem->GetRect());
	CXTPTabPaintManagerTheme::InflateRectEx(rcItem, CRect(0, 1, 0,  -2), tabPosition);

	BOOL bSelected = pItem->IsSelected();

	if (bSelected)
	{
		pDC->FillSolidRect(rcItem, RGB(212,212,212));
		rcItem.DeflateRect(1,1,1,-1);
		pDC->FillSolidRect(rcItem, headerColors[xtpRibbonTabContextColorNone].clrBackground);
	}
	else
	{
		pDC->FillSolidRect(rcItem, headerColors[tabColor].clrBackground);
	}
}

void CXTPRibbonOffice2013Theme::DrawRibbonFrameContextHeaders(CDC *pDC, CXTPRibbonBar *pRibbonBar, CXTPRibbonTabContextHeaders *pContextHeaders)
{
	int nCount = pContextHeaders->GetCount();

	CXTPWinThemeWrapper wrapper(FALSE);
	wrapper.OpenTheme(0, L"WINDOW");

	for (int i = 0; i < nCount; i++)
	{
		CXTPRibbonTabContextHeader* pHeader = pContextHeaders->GetHeader(i);
		XTPRibbonTabContextColor color = pHeader->m_color;

		CString strCaption(pHeader->m_strCaption);
		if (pRibbonBar->GetCommandBars()->IsAllCaps())
		{
			strCaption.MakeUpper();
		}

		CRect rcHeader(pHeader->m_rcRect);
		if (LAYOUT_RTL == pDC->GetLayout())
		{
			// GDI+ has issues with RTL renderring, so this is to emulate RTL.
			CPoint ptHeaderTopLeft = rcHeader.TopLeft();
			pRibbonBar->ClientToScreen(&ptHeaderTopLeft);

			CXTPClientRect rcClient(pRibbonBar);
			CPoint ptWindowTopLeft = rcClient.TopLeft();
			CPoint ptWindowTopRight(rcClient.right, ptWindowTopLeft.y);
			pRibbonBar->ClientToScreen(&ptWindowTopLeft);
			pRibbonBar->ClientToScreen(&ptWindowTopRight);
			ASSERT(ptWindowTopRight.x <= ptWindowTopLeft.x);
			ASSERT(ptHeaderTopLeft.x < ptWindowTopLeft.x && ptWindowTopRight.x < ptHeaderTopLeft.x);

			int nWidth = rcHeader.Width();
			rcHeader.left = rcClient.Width() - (ptWindowTopLeft.x - ptHeaderTopLeft.x) - nWidth - 1;
			rcHeader.right = rcHeader.left + nWidth;
		}

		CRect rcBorder(rcHeader.left, rcHeader.top, rcHeader.right, rcHeader.top+4);
		CRect rcBackground(rcHeader.left, rcBorder.bottom, rcHeader.right, rcHeader.bottom);
		CRect rcText(rcBackground);
		CSize szText = pDC->GetTextExtent(strCaption);

		if (szText.cx > rcText.Width() - 10) szText.cx = rcText.Width() - 10;

		rcText.left += ((rcText.Width() / 2) - (szText.cx / 2));

		if(IsGdiplusInitialized())
		{
			// Draw header using GDI+
			rcText.top += (rcBackground.Height() - szText.cy) / 2;

			Gdiplus::Graphics graphics(pDC->GetSafeHdc());
			graphics.SetCompositingMode(Gdiplus::CompositingModeSourceCopy);
			graphics.SetCompositingQuality(Gdiplus::CompositingQualityDefault);

			// Draw border
			DWORD clrBorder = headerColors[color].clrBorder;
			Gdiplus::SolidBrush brBorder(Gdiplus::Color(
				0xfe, // Not 0xff because GDI+ the applies color blending.
				GetRValue(clrBorder),
				GetGValue(clrBorder),
				GetBValue(clrBorder)));
			Gdiplus::RectF rcfBorder(
				static_cast<Gdiplus::REAL>(rcBorder.left),
				static_cast<Gdiplus::REAL>(rcBorder.top),
				static_cast<Gdiplus::REAL>(rcBorder.Width()),
				static_cast<Gdiplus::REAL>(rcBorder.Height()));
			graphics.FillRectangle(&brBorder, rcfBorder);

			// Draw background
			DWORD clrBackground = headerColors[color].clrBackground;
			Gdiplus::SolidBrush brBackground(Gdiplus::Color(
				0xfe, // Not 0xff because GDI+ the applies color blending.
				GetRValue(clrBackground),
				GetGValue(clrBackground),
				GetBValue(clrBackground)));
			Gdiplus::RectF rcfBackground(
				static_cast<Gdiplus::REAL>(rcBackground.left),
				static_cast<Gdiplus::REAL>(rcBackground.top),
				static_cast<Gdiplus::REAL>(rcBackground.Width()),
				static_cast<Gdiplus::REAL>(rcBackground.Height()));
			graphics.FillRectangle(&brBackground, rcfBackground);

			// Draw text
			Gdiplus::Font font(pDC->GetSafeHdc(), *m_pTabPaintManager->GetFont());
			Gdiplus::RectF rcfText(
				static_cast<Gdiplus::REAL>(rcText.left),
				static_cast<Gdiplus::REAL>(rcText.top),
				static_cast<Gdiplus::REAL>(rcText.Width()),
				static_cast<Gdiplus::REAL>(rcText.Height()));

			DWORD clrText = headerColors[color].clrText;
			Gdiplus::SolidBrush brText(Gdiplus::Color(
				0xfe, // Not 0xff because GDI+ the applies color blending.
				GetRValue(clrText),
				GetGValue(clrText),
				GetBValue(clrText)));

			Gdiplus::StringFormat format(Gdiplus::StringFormatFlagsNoWrap, ::GetUserDefaultLangID());
			format.SetTrimming(Gdiplus::StringTrimmingEllipsisCharacter);
			format.SetLineAlignment(Gdiplus::StringAlignmentCenter);

			graphics.SetCompositingMode(Gdiplus::CompositingModeSourceOver);
#ifdef _UNICODE
			graphics.DrawString(strCaption, strCaption.GetLength(), &font, rcfText, &format, &brText);
#else
			WCHAR* pUnicodeCaption = new WCHAR[strCaption.GetLength() + 1];
			int cchCaption = MultiByteToWideChar(CP_THREAD_ACP, MB_PRECOMPOSED, strCaption,
				strCaption.GetLength(), pUnicodeCaption,(strCaption.GetLength() + 1) * sizeof(WCHAR));
			if(0 < cchCaption)
			{
				graphics.DrawString(pUnicodeCaption, cchCaption, &font, rcfText, &format, &brText);
			}
			delete [] pUnicodeCaption;
#endif
		}
		else
		{
			// Draw header using GDI
			CXTPFontDC font(pDC, m_pTabPaintManager->GetFont());
			pDC->FillSolidRect(rcBorder, headerColors[color].clrBorder);
			pDC->FillSolidRect(rcBackground, headerColors[color].clrBackground);
			pDC->SetTextColor(headerColors[color].clrText);


			pDC->DrawText(strCaption, rcText, DT_SINGLELINE | DT_VCENTER | DT_NOPREFIX | DT_END_ELLIPSIS | DT_LEFT);
		}
	}

	wrapper.CloseTheme();
}
