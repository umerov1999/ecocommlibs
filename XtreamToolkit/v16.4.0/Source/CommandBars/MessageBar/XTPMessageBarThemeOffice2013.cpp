#include <StdAfx.h>

#include <Common/XTPDrawHelpers.h>
#include <Common/XTPColorManager.h>

#include <CommandBars/XTPCommandBarsDefines.h>
#include <CommandBars/XTPPaintManager.h>

#include <CommandBars/MessageBar/XTPMessageBar.h>
#include <CommandBars/MessageBar/XTPMessageBarPaintManager.h>
#include <CommandBars/MessageBar/XTPMessageBarThemeOffice2013.h>


CXTPMessageBarThemeOffice2013::CXTPMessageBarThemeOffice2013(CXTPPaintManager *pPaintManager)
	: CXTPMessageBarPaintManager(pPaintManager)
{
}

void CXTPMessageBarThemeOffice2013::FillMessageBar(CDC *pDC, CXTPMessageBar *pBar)
{
	CXTPClientRect rcClient(pBar);
	pDC->FillSolidRect(rcClient, RGB(252,247,182));
	pDC->Draw3dRect(rcClient, RGB(215,216,137), RGB(215,216,137));
}

void CXTPMessageBarThemeOffice2013::DrawMessageBarButton(CDC* pDC, CXTPMessageBarButton* pButton)
{
	CRect rc(pButton->m_rcButton);

	if (pButton->m_nID == SC_CLOSE)
	{
		CPoint pt = rc.CenterPoint();

		COLORREF clr;
		if (pButton->m_bPressed)
		{
			clr = RGB(68,68,68);
		}
		else if (pButton->m_bHot)
		{
			clr = RGB(68,68,68);
		}
		else
		{
			clr = RGB(119,119,119);
		}

		CXTPPenDC pen(*pDC, clr);

		pDC->MoveTo(pt.x - 4, pt.y - 3);
		pDC->LineTo(pt.x + 3, pt.y + 4);
		pDC->MoveTo(pt.x - 4, pt.y - 4);
		pDC->LineTo(pt.x + 4, pt.y + 4);
		pDC->MoveTo(pt.x - 3, pt.y - 4);
		pDC->LineTo(pt.x + 4, pt.y + 3);

		pDC->MoveTo(pt.x - 4, pt.y + 2);
		pDC->LineTo(pt.x + 3, pt.y - 5);
		pDC->MoveTo(pt.x - 4, pt.y + 3);
		pDC->LineTo(pt.x + 4, pt.y - 5);
		pDC->MoveTo(pt.x - 3, pt.y + 3);
		pDC->LineTo(pt.x + 4, pt.y - 4);
	}
	else
	{
		if (pButton->m_bPressed)
		{
			pDC->FillSolidRect(rc, RGB(198,198,198));
			pDC->Draw3dRect(rc, RGB(171,171,171), RGB(171,171,171));
		}
		else if (pButton->m_bHot)
		{
			pDC->FillSolidRect(rc, RGB(240,240,240));
			pDC->Draw3dRect(rc, RGB(198,198,198), RGB(198,198,198));

		}
		else
		{
			pDC->FillSolidRect(rc, RGB(253,253,253));
			pDC->Draw3dRect(rc, RGB(171,171,171), RGB(171,171,171));
		}
	}
}

CRect CXTPMessageBarThemeOffice2013::GetCloseButtonRect(int x, int y, int cy)
{
	CXTPEmptyRect rc;

	CSize sz(8,8);

	rc.top    = y + ((cy - sz.cy) / 2);
	rc.bottom = rc.top + sz.cy;
	rc.right  = x - 11;
	rc.left   = rc.right - sz.cx;

	return rc;
}
