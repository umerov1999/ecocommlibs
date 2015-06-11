#include "StdAfx.h"

#include <Common/XTPColorManager.h>
#include <Common/XTPDrawHelpers.h>

#include <TabManager/XTPTabManagerDefines.h>
#include <TabManager/XTPTabManagerAtom.h>
#include <TabManager/XTPTabManagerNavigateButton.h>
#include <TabManager/XTPTabManagerNavigateButtons.h>
#include <TabManager/XTPTabManagerItem.h>
#include <TabManager/XTPTabManager.h>
#include <TabManager/XTPTabPaintManagerColorSet.h>
#include <TabManager/XTPTabPaintManager.h>
#include <TabManager/XTPTabPaintManager.inl>
#include <TabManager/XTPTabPaintManagerTheme.h>

#include <TabManager/Themes/XTPTabThemePropertyPage2003.h>


//////////////////////////////////////////////////////////////////////////
// CAppearanceSetPropertyPage2003

void CAppearanceSetPropertyPage2003::RefreshMetrics()
{
	CXTPTabPaintManagerTheme::RefreshMetrics();
}

CRect CAppearanceSetPropertyPage2003::GetButtonDrawRect(const CXTPTabManagerItem* pItem)
{
	CRect rc = pItem->GetRect();

	return pItem->GetTabManager()->IsHorizontalPosition() ?
		CRect(rc.left - rc.Height(), rc.top, rc.right + 2, rc.bottom) :
		CRect(rc.left, rc.top - rc.Width(), rc.right, rc.bottom + 2);
}

CRect CAppearanceSetPropertyPage2003::FillTabControl(CXTPTabManager* pTabManager, CDC* pDC, CRect rcControl)
{

	CRect rcHeader = GetHeaderRect(rcControl, pTabManager);

	GetColorSet()->FillHeader(pDC, rcHeader, pTabManager);

	CRect rcClient = GetClientRect(rcControl, pTabManager);
	if (m_pPaintManager->m_clientFrame == xtpTabFrameBorder)
	{
		GetColorSet()->FillClient(pDC, rcClient, pTabManager);
		pDC->Draw3dRect(rcClient, GetColorSet()->m_csPropertyPage2003.clrFrameBorder, GetColorSet()->m_csPropertyPage2003.clrFrameBorder);
	}
	if (m_pPaintManager->m_clientFrame == xtpTabFrameSingleLine)
	{
		DrawSingleLineBorder(pDC, rcClient, pTabManager->GetPosition(), GetColorSet()->m_csPropertyPage2003.clrFrameBorder, GetColorSet()->m_csPropertyPage2003.clrFrameBorder);
	}

	return rcHeader;
}


void CAppearanceSetPropertyPage2003::DrawSingleButton(CDC* pDC, CXTPTabManagerItem* pItem)
{
	CRect rc = pItem->GetRect();

	COLORREF clrBorder = GetColorSet()->m_csPropertyPage2003.clrDarkShadow, clrLeftHighlight = GetColorSet()->m_csPropertyPage2003.clrBorderLeftHighlight, clrTopHighlight = GetColorSet()->m_csPropertyPage2003.clrBorderTopHighlight;
	if (pItem->IsSelected())
	{
		clrBorder = GetColorSet()->m_csPropertyPage2003.clrFrameBorder;
		clrLeftHighlight = RGB(255, 255, 255);
		clrTopHighlight = RGB(255, 255, 255);
	}

	if (pItem->IsHighlighted() && m_bDoubleHighlightedBorder)
	{
		clrLeftHighlight = clrTopHighlight = clrBorder = GetColorSet()->m_csStateButton.clrBorder.clrDark;
	}


	switch (pItem->GetTabManager()->GetPosition())
	{
	case xtpTabPositionTop:
		{
			rc.bottom--;

			POINT ptsFill[] = {{rc.right - 1, rc.bottom}, {0, 2 - rc.Height()}, {5 - rc.Width(), 0}, {3 - rc.Height(), rc.Height() - 3}};
			COLORREF clrDark = FillButton(pDC, pItem, ptsFill, sizeof(ptsFill) / sizeof(ptsFill[0]), pItem->GetTabManager()->GetPosition());

			POINT ptsBorder[] = {{rc.right, rc.bottom - 1}, {+0, 3 - rc.Height()}, {-2, -2}, {9 - rc.Width(), 0}, {-5, 2}, {3 - rc.Height(), rc.Height() - 3}, {-2, 0}};
			DrawPolyLine(pDC, clrBorder, ptsBorder, sizeof(ptsBorder) / sizeof(ptsBorder[0]));

			POINT ptsLeftHighlight[] = {{rc.right - 1, rc.bottom - 1}, {+0, 3 - rc.Height()}, {-2, -2}};
			DrawPolyLine(pDC, clrLeftHighlight, ptsLeftHighlight, sizeof(ptsLeftHighlight) / sizeof(ptsLeftHighlight[0]));

			POINT ptsTopHighlight[] = {{rc.right - 3, rc.top + 1}, {9 - rc.Width(), 0}, {-5, +3}, {4 - rc.Height(), rc.Height() - 4}};
			DrawPolyLine(pDC, clrTopHighlight, ptsTopHighlight, sizeof(ptsTopHighlight) / sizeof(ptsTopHighlight[0]));

			if (pItem->IsSelected())
				HorizontalLine(pDC, rc.left - rc.Height() + 4, rc.bottom, rc.Width() + rc.Height() - 3, clrDark);
		}
		break;

	case xtpTabPositionLeft:
		{
			rc.right--;

			POINT ptsFill[] = {{rc.right, rc.bottom - 1}, {2 - rc.Width(), 0}, {0, 5 - rc.Height()}, {rc.Width() - 2, 2 - rc.Width()}};
			COLORREF clrDark = FillButton(pDC, pItem, ptsFill, sizeof(ptsFill) / sizeof(ptsFill[0]), pItem->GetTabManager()->GetPosition());

			POINT ptsBorder[] = {{rc.right - 1, rc.bottom}, {3 - rc.Width(), 0}, {-2, -2}, {0, 9 - rc.Height()}, {2, -5}, {rc.Width() - 3, 3 - rc.Width()}, {0, -2}};
			DrawPolyLine(pDC, clrBorder, ptsBorder, sizeof(ptsBorder) / sizeof(ptsBorder[0]));

			POINT ptsLeftHighlight[] = {{rc.right - 1, rc.bottom - 1}, {3 - rc.Width(), 0}, {-2, -2}};
			DrawPolyLine(pDC, clrLeftHighlight, ptsLeftHighlight, sizeof(ptsLeftHighlight) / sizeof(ptsLeftHighlight[0]));

			POINT ptsTopHighlight[] = {{rc.left + 1, rc.bottom - 3}, {0, 9 - rc.Height()}, {+3, -5}, {rc.Width() - 4, 4 - rc.Width()}};
			DrawPolyLine(pDC, clrTopHighlight, ptsTopHighlight, sizeof(ptsTopHighlight) / sizeof(ptsTopHighlight[0]));

			if (pItem->IsSelected())
				VerticalLine(pDC, rc.right, rc.top - rc.Width() + 4, rc.Width() + rc.Height() - 3, clrDark);
		}

		break;

	case xtpTabPositionBottom:
		{
			POINT ptsFill[] = {{rc.right - 1, rc.top}, {0, rc.Height() - 1}, {5 - rc.Width(), 0}, {3 - rc.Height(), 2 - rc.Height()}};
			COLORREF clrDark = FillButton(pDC, pItem, ptsFill, sizeof(ptsFill) / sizeof(ptsFill[0]), pItem->GetTabManager()->GetPosition());

			POINT ptsBorder[] = {{rc.right, rc.top + 1}, {+0, rc.Height() - 3}, {-2, 2}, {9 - rc.Width(), 0}, {-5, -2}, {3 - rc.Height(), 3 - rc.Height()}, {-2, 0}};
			DrawPolyLine(pDC, clrBorder, ptsBorder, sizeof(ptsBorder) / sizeof(ptsBorder[0]));

			POINT ptsLeftHighlight[] = {{rc.right - 1, rc.top + 1}, {+0, rc.Height() - 3}, {-2, 2}};
			DrawPolyLine(pDC, clrLeftHighlight, ptsLeftHighlight, sizeof(ptsLeftHighlight) / sizeof(ptsLeftHighlight[0]));

			POINT ptsTopHighlight[] = {{rc.right - 3, rc.bottom - 1}, {9 - rc.Width(), 0}, {-5, -3}, {4 - rc.Height(), 4 - rc.Height()}};
			DrawPolyLine(pDC, clrTopHighlight, ptsTopHighlight, sizeof(ptsTopHighlight) / sizeof(ptsTopHighlight[0]));

			if (pItem->IsSelected())
				HorizontalLine(pDC, rc.left - rc.Height() + 4, rc.top, rc.Width() + rc.Height() - 3, clrDark);

		}
		break;

	case xtpTabPositionRight:
		{
			POINT ptsFill[] = {{rc.left + 1, rc.bottom - 1}, {rc.Width() - 2, 0}, {0, 5 - rc.Height()}, {2 - rc.Width(), 2 - rc.Width()}};
			//POINT ptsFill[] = {{rc.left, rc.bottom - 1}, {rc.Width() - 1, 0}, {0, 5 - rc.Height()}, {2 - rc.Width(), 2 - rc.Width()}};
			COLORREF clrDark = FillButton(pDC, pItem, ptsFill, sizeof(ptsFill) / sizeof(ptsFill[0]), pItem->GetTabManager()->GetPosition());

			POINT ptsBorder[] = {{rc.left + 1, rc.bottom}, {rc.Width() -3, 0}, {2, -2}, {0, 9 - rc.Height()}, {-2, -5}, {3 - rc.Width(), 3 - rc.Width()}, {0, -2}};
			DrawPolyLine(pDC, clrBorder, ptsBorder, sizeof(ptsBorder) / sizeof(ptsBorder[0]));

			POINT ptsLeftHighlight[] = {{rc.left + 1, rc.bottom - 1}, {rc.Width() - 3, 0}, {2, -2}};
			DrawPolyLine(pDC, clrLeftHighlight, ptsLeftHighlight, sizeof(ptsLeftHighlight) / sizeof(ptsLeftHighlight[0]));

			POINT ptsTopHighlight[] = {{rc.right - 1, rc.bottom - 3}, {0, 9 - rc.Height()}, {-3, -5}, {4 - rc.Width(), 4 - rc.Width()}};
			DrawPolyLine(pDC, clrTopHighlight, ptsTopHighlight, sizeof(ptsTopHighlight) / sizeof(ptsTopHighlight[0]));

			if (pItem->IsSelected())
				VerticalLine(pDC, rc.left, rc.top - rc.Width() + 4, rc.Width() + rc.Height() - 3, clrDark);


		}
		break;


	}


	m_pPaintManager->DrawSingleButtonIconAndText(pDC, pItem, pItem->GetRect(), TRUE);
}
