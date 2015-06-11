#include "StdAfx.h"

#include <Common/XTPColorManager.h>
#include <Common/XTPDrawHelpers.h>
#include <Common/ScrollBar/XTPScrollBase.h>
#include <Common/ScrollBar/XTPScrollBarPaintManager.h>
#include <Common/ScrollBar/Themes/XTPScrollBarThemeOffice2013.h>


//////////////////////////////////////////////////////////////////////////
// CXTPScrollBarThemeOffice2013

CXTPScrollBarThemeOffice2013::CXTPScrollBarThemeOffice2013()
	: CXTPScrollBarPaintManager()
{
}

CXTPScrollBarThemeOffice2013::~CXTPScrollBarThemeOffice2013()
{
}

void CXTPScrollBarThemeOffice2013::RefreshMetrics()
{
	CXTPScrollBarPaintManager::RefreshMetrics();

	// Normal
	m_colorNormal.m_clrBackground         = RGB(243, 243, 243);
	m_colorNormal.m_clrButtonBorder       = RGB(171, 171, 171);
	m_colorNormal.m_clrButtonBackground   = RGB(255, 255, 255);
	m_colorNormal.m_clrButtonArrow        = RGB(119, 119, 119);
	m_colorNormal.m_clrThumbBorder        = RGB(171, 171, 171);
	m_colorNormal.m_clrThumbBackground    = RGB(255, 255, 255);

	// Selected
	m_colorSelected.m_clrBackground       = RGB(243, 243, 243);
	m_colorSelected.m_clrButtonBorder     = RGB(119, 119, 119);
	m_colorSelected.m_clrButtonBackground = RGB(255, 255, 255);
	m_colorSelected.m_clrButtonArrow      = RGB(119, 119, 119);
	m_colorSelected.m_clrThumbBorder      = RGB(171, 171, 171);
	m_colorSelected.m_clrThumbBackground  = RGB(240, 240, 240);

	// Pressed
	m_colorPressed.m_clrBackground        = RGB(243, 243, 243);
	m_colorPressed.m_clrButtonBorder      = RGB(119, 119, 119);
	m_colorPressed.m_clrButtonBackground  = RGB(240, 240, 240);
	m_colorPressed.m_clrButtonArrow       = RGB(119, 119, 119);
	m_colorPressed.m_clrThumbBorder       = RGB(119, 119, 119);
	m_colorPressed.m_clrThumbBackground   = RGB(240, 240, 240);
}

void CXTPScrollBarThemeOffice2013::DrawScrollBar(CDC *pDC, CXTPScrollBase *pGallery)
{
	XTP_SCROLLBAR_TRACKINFO *pSBTrack = pGallery->GetScrollBarTrackInfo();
	XTP_SCROLLBAR_POSINFO   *pSBInfo  = pGallery->GetScrollBarPosInfo();

	if(!pSBInfo->fSizebox)
	{
		int nHtSelected = (NULL != pSBTrack) ? -1 : pSBInfo->ht;
		int nHtPressed  = (NULL != pSBTrack) ? pSBInfo->ht : -1;

		CXTPScrollBarColorsOffice2013 *pColorsUp    = NULL;
		CXTPScrollBarColorsOffice2013 *pColorsDown  = NULL;
		CXTPScrollBarColorsOffice2013 *pColorsThumb = NULL;

		if (XTP_HTSCROLLUP == nHtSelected) pColorsUp = &m_colorSelected;
		else if (XTP_HTSCROLLUP == nHtPressed)  pColorsUp = &m_colorPressed;
		else pColorsUp = &m_colorNormal;

		if (XTP_HTSCROLLDOWN == nHtSelected) pColorsDown = &m_colorSelected;
		else if (XTP_HTSCROLLDOWN == nHtPressed)  pColorsDown = &m_colorPressed;
		else pColorsDown = &m_colorNormal;

		if (XTP_HTSCROLLTHUMB == nHtSelected) pColorsThumb = &m_colorSelected;
		else if (XTP_HTSCROLLTHUMB == nHtPressed)  pColorsThumb = &m_colorPressed;
		else pColorsThumb = &m_colorNormal;

		if (pSBInfo->fVert)
		{
			XTPScrollMetricsV scroll;
			CalcVScroll(pGallery, &scroll);

			pDC->FillSolidRect(scroll.rcScrollBar, pColorsThumb->m_clrBackground);

			// Up
			scroll.rcArrowUp.DeflateRect(1, 1, 1, 1);
			XTPDrawHelpers()->SolidRectangle(pDC, scroll.rcArrowUp, pColorsUp->m_clrButtonBorder, pColorsUp->m_clrButtonBackground);
			scroll.rcArrowUp.InflateRect(1, 1, 1, 1);
			DrawArrowGlyph(pDC, scroll.rcArrowUp, xtpArrowGlyphUp, scroll.bEnabled, scroll.bEnabled ? pColorsUp->m_clrButtonArrow : COLORREF_NULL);

			// Thumb
			if (scroll.bEnabled)
			{
				scroll.rcBtnTrack.DeflateRect(1, 0, 1, 0);
				XTPDrawHelpers()->SolidRectangle(pDC, scroll.rcBtnTrack, pColorsThumb->m_clrThumbBorder, pColorsThumb->m_clrThumbBackground);
			}

			// Down
			scroll.rcArrowDown.DeflateRect(1, 1, 1, 1);
			XTPDrawHelpers()->SolidRectangle(pDC, scroll.rcArrowDown, pColorsDown->m_clrButtonBorder, pColorsDown->m_clrButtonBackground);
			scroll.rcArrowDown.InflateRect(1, 1, 1, 1);
			DrawArrowGlyph(pDC, scroll.rcArrowDown, xtpArrowGlyphDown, scroll.bEnabled, scroll.bEnabled ? pColorsUp->m_clrButtonArrow : COLORREF_NULL);
		}
		else
		{
			XTPScrollMetricsH scroll;
			CalcHScroll(pGallery, &scroll);

			pDC->FillSolidRect(scroll.rcScrollBar, pColorsThumb->m_clrBackground);

			// Left
			scroll.rcArrowLeft.DeflateRect(1, 1, 1, 1);
			XTPDrawHelpers()->SolidRectangle(pDC, scroll.rcArrowLeft, pColorsUp->m_clrButtonBorder, pColorsUp->m_clrButtonBackground);
			scroll.rcArrowLeft.InflateRect(1, 1, 1, 1);
			DrawArrowGlyph(pDC, scroll.rcArrowLeft, xtpArrowGlyphLeft, scroll.bEnabled, scroll.bEnabled ? pColorsUp->m_clrButtonArrow : COLORREF_NULL);

			// Thumb
			if (scroll.bEnabled)
			{
				scroll.rcBtnTrack.DeflateRect(0, 1, 0, 1);
				XTPDrawHelpers()->SolidRectangle(pDC, scroll.rcBtnTrack, pColorsThumb->m_clrThumbBorder, pColorsThumb->m_clrThumbBackground);
			}

			// Right
			scroll.rcArrowRight.DeflateRect(1, 1, 1, 1);
			XTPDrawHelpers()->SolidRectangle(pDC, scroll.rcArrowRight, pColorsDown->m_clrButtonBorder, pColorsDown->m_clrButtonBackground);
			scroll.rcArrowRight.InflateRect(1, 1, 1, 1);
			DrawArrowGlyph(pDC, scroll.rcArrowRight, xtpArrowGlyphRight, scroll.bEnabled, scroll.bEnabled ? pColorsUp->m_clrButtonArrow : COLORREF_NULL);
		}
	}
	else
	{
		pDC->FillSolidRect(&pSBInfo->rc, m_colorNormal.m_clrBackground);
	}
}
