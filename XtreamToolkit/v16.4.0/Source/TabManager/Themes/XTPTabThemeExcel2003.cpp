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

#include <TabManager/Themes/XTPTabThemeFlat.h>
#include <TabManager/Themes/XTPTabThemeExcel2003.h>

//////////////////////////////////////////////////////////////////////////
// CXTPTabThemeExcel2003

void CXTPTabThemeExcel2003::DrawSingleButton(CDC* pDC, CXTPTabManagerItem* pItem)
{
	CRect rc(pItem->GetRect());

	CXTPTabPaintManagerColorSet::COLORSET_TAB_EXCEL& cs = pItem->IsSelected() ? GetColorSet()->m_csExcelSelected : GetColorSet()->m_csExcelNormal;

	switch (pItem->GetTabManager()->GetPosition())
	{
	case xtpTabPositionTop:
		{
			rc.bottom -= 1;
			int nMargin = rc.Height() / 2;
			rc.OffsetRect(- nMargin / 2 , 0);

			POINT ptsFill[] = {{rc.left, rc.bottom}, {nMargin, - nMargin * 2}, {rc.Width() - nMargin, 0}, {nMargin, nMargin * 2}};
			COLORREF clrDark = FillButton(pDC, pItem, ptsFill, _countof(ptsFill), pItem->GetTabManager()->GetPosition(), FALSE);

			LineEx(pDC, cs.clrLeftOuterBorder, rc.left + 1, rc.bottom - 1, rc.left + nMargin + 1, rc.top);
			LineEx(pDC, cs.clrLeftOuterBorder, rc.left + nMargin, rc.top + 1, rc.right, rc.top + 1);
			LineEx(pDC, cs.clrRightOuterBorder, rc.right - 1, rc.top, rc.right + nMargin - 1, rc.bottom);

			LineEx(pDC, cs.clrLeftInnerBorder, rc.left, rc.bottom - 1, rc.left + nMargin, rc.top);
			LineEx(pDC, cs.clrLeftInnerBorder, rc.left + nMargin, rc.top, rc.right, rc.top);
			LineEx(pDC, cs.clrRightInnerBorder, rc.right, rc.top, rc.right + nMargin, rc.bottom);

			if (pItem->IsSelected())
				HorizontalLine(pDC, rc.left, rc.bottom, rc.Width() + nMargin, clrDark);

		}
		break;
	case xtpTabPositionLeft:
		{
			rc.right -= 1;
			int nMargin = rc.Width() / 2;
			rc.OffsetRect(0, - nMargin / 2);

			POINT ptsFill[] = {{rc.right, rc.top}, {-nMargin * 2, nMargin}, {0, rc.Height() - nMargin}, {nMargin * 2, nMargin}};
			COLORREF clrDark = FillButton(pDC, pItem, ptsFill, _countof(ptsFill), pItem->GetTabManager()->GetPosition(), FALSE);

			LineEx(pDC, cs.clrLeftOuterBorder, rc.right - 1, rc.top + 1, rc.left, rc.top + nMargin + 1);
			LineEx(pDC, cs.clrLeftOuterBorder, rc.left + 1, rc.top + nMargin, rc.left + 1, rc.bottom);
			LineEx(pDC, cs.clrRightOuterBorder, rc.left , rc.bottom - 1, rc.right, rc.bottom + nMargin - 1);

			LineEx(pDC, cs.clrLeftInnerBorder, rc.right - 1, rc.top, rc.left, rc.top + nMargin);
			LineEx(pDC, cs.clrLeftInnerBorder, rc.left, rc.top + nMargin, rc.left, rc.bottom);
			LineEx(pDC, cs.clrRightInnerBorder, rc.left, rc.bottom, rc.right, rc.bottom + nMargin);

			if (pItem->IsSelected())
				VerticalLine(pDC, rc.right, rc.top, rc.Height() + nMargin, clrDark);

		}
		break;

	case xtpTabPositionBottom:
		{
			rc.bottom -= 1;
			int nMargin = rc.Height() / 2;
			rc.OffsetRect(- nMargin /2 , 0);

			POINT ptsFill[] = {{rc.left, rc.top + 1}, {nMargin, nMargin * 2}, {rc.Width() - nMargin, 0}, {nMargin, - nMargin * 2}};
			COLORREF clrDark = FillButton(pDC, pItem, ptsFill, _countof(ptsFill), pItem->GetTabManager()->GetPosition(), FALSE);

			LineEx(pDC, cs.clrLeftOuterBorder, rc.left + 1, rc.top + 1, rc.left + nMargin + 1, rc.bottom);
			LineEx(pDC, cs.clrTopOuterBorder, rc.left + nMargin, rc.bottom - 1, rc.right, rc.bottom - 1);
			LineEx(pDC, cs.clrRightOuterBorder, rc.right - 1, rc.bottom, rc.right + nMargin - 1, rc.top);

			LineEx(pDC, cs.clrLeftInnerBorder, rc.left, rc.top + 1, rc.left + nMargin, rc.bottom);
			LineEx(pDC, cs.clrTopInnerBorder, rc.left + nMargin, rc.bottom, rc.right, rc.bottom);
			LineEx(pDC, cs.clrRightInnerBorder, rc.right, rc.bottom, rc.right + nMargin, rc.top);

			if (pItem->IsSelected())
				HorizontalLine(pDC, rc.left, rc.top, rc.Width() + nMargin, clrDark);
		}

		break;

	case xtpTabPositionRight:
		{
			rc.right -= 1;
			int nMargin = rc.Width() / 2;
			rc.OffsetRect(0, - nMargin / 2);

			POINT ptsFill[] = {{rc.left + 1, rc.top}, {nMargin * 2, nMargin}, {0, rc.Height() - nMargin}, {- nMargin * 2, nMargin}};
			COLORREF clrDark = FillButton(pDC, pItem, ptsFill, _countof(ptsFill), pItem->GetTabManager()->GetPosition(), FALSE);

			LineEx(pDC, cs.clrLeftOuterBorder, rc.left + 1, rc.top + 1, rc.right, rc.top + nMargin + 1);
			LineEx(pDC, cs.clrLeftOuterBorder, rc.right - 1, rc.top + nMargin, rc.right - 1, rc.bottom);
			LineEx(pDC, cs.clrRightOuterBorder, rc.right , rc.bottom - 1, rc.left, rc.bottom + nMargin - 1);

			LineEx(pDC, cs.clrLeftInnerBorder, rc.left + 1, rc.top, rc.right, rc.top + nMargin);
			LineEx(pDC, cs.clrLeftInnerBorder, rc.right, rc.top + nMargin, rc.right, rc.bottom);
			LineEx(pDC, cs.clrRightInnerBorder, rc.right, rc.bottom, rc.left, rc.bottom + nMargin);

			if (pItem->IsSelected())
				VerticalLine(pDC, rc.left, rc.top, rc.Height() + nMargin, clrDark);
		}
		break;
	}

	m_pPaintManager->DrawSingleButtonIconAndText(pDC, pItem, pItem->GetRect(), TRUE);
}

CRect CXTPTabThemeExcel2003::GetButtonDrawRect(const CXTPTabManagerItem* pItem)
{
	CRect rc = pItem->GetRect();

	return pItem->GetTabManager()->IsHorizontalPosition() ?
		CRect(rc.left - rc.Height() / 2, rc.top, rc.right + rc.Height() / 2 , rc.bottom) :
		CRect(rc.left, rc.top  - rc.Width() / 2, rc.right, rc.bottom  + rc.Width() / 2);
}
