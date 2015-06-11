#include "StdAfx.h"

#include <Common/XTPColorManager.h>
#include <Common/XTPDrawHelpers.h>

#include <TabManager/Includes.h>
#include <TabManager/Themes/XTPTabThemeVisualStudio2010.h>


//////////////////////////////////////////////////////////////////////////
// CXTPTabThemeVisualStudio2010

void CXTPTabThemeVisualStudio2010::DrawSingleButton(CDC* pDC, CXTPTabManagerItem* pItem)
{
	CRect rcItem(pItem->GetRect());
	XTPTabPosition tabPosition = pItem->GetTabManager()->GetPosition();

	CRect rcTab(rcItem);

	InflateRectEx(rcTab, CRect(0, 0, 0, 1), tabPosition);

	XTPDrawHelpers()->ExcludeCorners(pDC, rcTab, TRUE);

	if (!m_bDrawNormalTab  && !pItem->IsSelected() && !pItem->IsHighlighted())
	{
		m_pPaintManager->DrawSingleButtonIconAndText(pDC, pItem, pItem->GetRect(), TRUE);
		return;
	}

	CXTPTabPaintManagerColorSet *pColorSet = GetColorSet();
	COLORREF clrFrameBorder = pItem->IsHighlighted() && !pItem->IsSelected() ? pColorSet->m_clrFrameHighlightBorder : COLORREF_NULL;
	if (clrFrameBorder == COLORREF_NULL) clrFrameBorder = pColorSet->m_clrFrameBorder;

	pColorSet->FillPropertyButton(pDC, rcItem, pItem);

	if (pItem->IsSelected())
	{
		if (!pColorSet->m_clrFrameHighlightBorder.IsDefaultValue())
			InflateRectEx(rcTab, CRect(1, 1, 1, 0), tabPosition);
		else
			pDC->Draw3dRect(rcTab, clrFrameBorder, clrFrameBorder);
	}
	else
	{
		pDC->Draw3dRect(rcTab, clrFrameBorder, clrFrameBorder);
	}

	if (m_bBlurPoints)
	{
		BlurTab(pDC, rcTab, pColorSet->m_clrHeaderFace, clrFrameBorder, tabPosition);
	}

	CFont *pFont = pDC->GetCurrentFont();
	LOGFONT lf;
	pFont->GetLogFont(&lf);
	TRACE(_T("%s %d\n"), lf.lfFaceName, lf.lfWeight);

	m_pPaintManager->DrawSingleButtonIconAndText(pDC, pItem, pItem->GetRect(), TRUE);
}
