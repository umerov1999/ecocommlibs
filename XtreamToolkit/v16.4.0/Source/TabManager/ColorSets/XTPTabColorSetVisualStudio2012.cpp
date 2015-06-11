#include "StdAfx.h"

#include <Common/XTPColorManager.h>
#include <TabManager/Includes.h>

#define Base CXTPTabPaintManagerColorSet

CXTPTabColorSetVisualStudio2012::CXTPTabColorSetVisualStudio2012()
	: Base()
{
}

void CXTPTabColorSetVisualStudio2012::RefreshMetrics()
{
	Base::RefreshMetrics();

	m_clrButtonNormal      = RGB(239, 239, 242);
	m_clrButtonSelected    = RGB(  0, 122, 204);
	m_clrButtonHighlighted = RGB( 28, 151, 234);
	m_clrButtonPressed     = RGB(  0, 122, 204);

	m_clrTextNormal    = RGB(  30,   30,   30);
	m_clrTextHighlight = RGB(255, 255, 255);
	m_clrTextSelected  = RGB(255, 255, 255);

	m_clrActiveFilesButton            = RGB(113, 113, 113);
	m_clrActiveFilesButtonHighlighted = RGB(  0, 122, 204);
	m_clrActiveFilesButtonPressed     = RGB(255, 255, 255);

	m_clrActiveFilesButtonBackground            = RGB(239, 239, 242);
	m_clrActiveFilesButtonBackgroundHighlighted = RGB(254, 254, 254);
	m_clrActiveFilesButtonBackgroundPressed     = RGB(  0, 122, 204);

	m_clrFrameBorder = m_clrButtonNormal; //RGB(  0, 122, 204);
	m_clrAutoHideFace = RGB(239, 239, 242);

	m_clrHeaderFace = m_clrButtonNormal;
}

COLORREF CXTPTabColorSetVisualStudio2012::GetItemColor(CXTPTabManagerItem *pItem)
{
	COLORREF clrItem;

		 if (pItem->IsSelected())    { clrItem = m_clrButtonSelected;    }
	else if (pItem->IsPressed())     { clrItem = m_clrButtonPressed;     }
	else if (pItem->IsHighlighted()) { clrItem = m_clrButtonHighlighted; }
	else                             { clrItem = m_clrButtonNormal;      }

	return clrItem;
}

void CXTPTabColorSetVisualStudio2012::FillNavigateButton(CDC *pDC, CXTPTabManagerNavigateButton *pButton, CRect &rc)
{
	if (pButton->GetID() == xtpTabNavigateButtonActiveFiles)
	{
		if (pButton->IsHighlighted() && !pButton->IsPressed())
		{
			pDC->FillSolidRect(rc, m_clrActiveFilesButtonBackgroundHighlighted);
			pDC->SetTextColor(m_clrActiveFilesButtonHighlighted);
		}
		else if (pButton->IsHighlighted() && pButton->IsPressed())
		{
			pDC->FillSolidRect(rc, m_clrActiveFilesButtonBackgroundPressed);
			pDC->SetTextColor(m_clrActiveFilesButtonPressed);
		}
		else
		{
			pDC->FillSolidRect(rc, m_clrActiveFilesButtonBackground);
			pDC->SetTextColor(m_clrActiveFilesButton);
		}
	}
}
