#include "StdAfx.h"

#include <Common/XTPColorManager.h>
#include <TabManager/Includes.h>

#define Base CXTPTabPaintManagerColorSet

CXTPTabColorSetOffice2013::CXTPTabColorSetOffice2013()
	: Base()
{
}

void CXTPTabColorSetOffice2013::RefreshMetrics()
{
	Base::RefreshMetrics();

	m_clrButtonNormal      = RGB(255, 255, 255);
	m_clrButtonSelected    = RGB(43, 87, 154);
	m_clrButtonHighlighted = RGB(213, 225, 242);
	m_clrButtonPressed     = m_clrButtonHighlighted;

	m_clrTextNormal    = RGB(0,0,0);
	m_clrTextHighlight = RGB(0, 0, 0);
	m_clrTextSelected  = RGB(255,255,255);

	m_clrFrameBorder = RGB(  255, 255, 255);
	m_clrAutoHideFace = RGB(255,255,255);

	m_clrHeaderFace = RGB(255,255,255);
}

COLORREF CXTPTabColorSetOffice2013::GetItemColor(CXTPTabManagerItem *pItem)
{
	COLORREF clrItem;

		 if (pItem->IsSelected())    { clrItem = m_clrButtonSelected;    }
	else if (pItem->IsPressed())     { clrItem = m_clrButtonPressed;     }
	else if (pItem->IsHighlighted()) { clrItem = m_clrButtonHighlighted; }
	else                             { clrItem = m_clrButtonNormal;      }

	return clrItem;
}

void CXTPTabColorSetOffice2013::FillNavigateButton(CDC *pDC, CXTPTabManagerNavigateButton *pButton, CRect &rc)
{
	UNREFERENCED_PARAMETER(pDC);
	UNREFERENCED_PARAMETER(pButton);
	UNREFERENCED_PARAMETER(rc);
}
