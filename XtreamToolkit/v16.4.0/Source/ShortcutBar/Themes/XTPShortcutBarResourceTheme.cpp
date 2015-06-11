// XTPShortcutBarPaintManager.cpp : implementation of the CXTPShortcutBarPaintManager class.
//
// This file is a part of the XTREME SHORTCUTBAR MFC class library.
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

#include <Common/XTPVC80Helpers.h>
#include <Common/XTPDrawHelpers.h>
#include <Common/XTPImageManager.h>
#include <Common/XTPResourceImage.h>
#include <Common/XTPSystemHelpers.h>
#include <Common/XTPResourceManager.h>
#include <Common/XTPColorManager.h>
#include <Common/XTPWinThemeWrapper.h>

#include <ShortcutBar/Resource.h>
#include <ShortcutBar/XTPShortcutBarPaintManager.h>
#include <ShortcutBar/XTPShortcutBar.h>
#include <ShortcutBar/XTPShortcutBarPane.h>
#include <ShortcutBar/Themes/XTPShortcutBarOffice2003Theme.h>
#include <ShortcutBar/Themes/XTPShortcutBarResourceTheme.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////////
// CXTPShortcutBarResourceTheme

CXTPShortcutBarResourceTheme::CXTPShortcutBarResourceTheme()
{
	m_bFlatStyle = FALSE;
	m_bBoldItemCaptionFont = TRUE;
	m_bShowTopLeftCaptionShadow = TRUE;
	m_bClearTypeTextQuality = TRUE;
	m_strOfficeFont = _T("Segoe UI");

	m_rcCaptionPadding.SetRect(8, 0, 8, 0);
	m_rcItemCaptionPadding.SetRect(8, 0, 8, 0);
}

void CXTPShortcutBarResourceTheme::RefreshMetrics()
{
	CXTPShortcutBarOffice2003Theme::RefreshMetrics();

	CXTPResourceImages* pImages = XTPResourceImages();

	CXTPResourceImage* pImage = pImages->LoadFile(_T("SHORTCUTBARITEMS"));
	pImages->AssertValid();

	m_bFlatStyle = FALSE;

	if (!pImage)
		return;

	COLORREF clrText = pImages->GetImageColor(_T("ShortcutBar"), _T("NormalText"));
	m_clrPushedText.SetStandardValue(clrText);
	m_clrNormalText.SetStandardValue(clrText);
	m_clrHotText.SetStandardValue(clrText);
	m_clrSelectedText.SetStandardValue(pImages->GetImageColor(_T("ShortcutBar"), _T("SelectedText")));

	m_clrShortcutItemShadow.SetStandardValue(pImages->GetImageColor(_T("ShortcutBar"), _T("FrameBorder")));

	m_grcCaption.SetStandardValue(pImages->GetImageColor(_T("ShortcutBar"), _T("CaptionLight")),
		pImages->GetImageColor(_T("ShortcutBar"), _T("CaptionDark")));
	m_clrCaptionText.SetStandardValue(pImages->GetImageColor(_T("ShortcutBar"), _T("CaptionText")));

	m_grcShortcutBarGripper.SetStandardValue(pImages->GetImageColor(_T("ShortcutBar"), _T("GripperLight")),
		pImages->GetImageColor(_T("ShortcutBar"), _T("GripperDark")));

	m_grcItemCaption.SetStandardValue(pImages->GetImageColor(_T("ShortcutBar"), _T("ItemNormalCaptionLight")),
		pImages->GetImageColor(_T("ShortcutBar"), _T("ItemNormalCaptionDark")));

	m_grcItemHotCaption.SetStandardValue(pImages->GetImageColor(_T("ShortcutBar"), _T("ItemHotCaptionLight")),
		pImages->GetImageColor(_T("ShortcutBar"), _T("ItemHotCaptionDark")));

	m_clrItemCaptionText.SetStandardValue(pImages->GetImageColor(_T("ShortcutBar"), _T("ItemCaptionText")));

	m_clrItemCaptionTop.SetStandardValue(pImages->GetImageColor(_T("ShortcutBar"), _T("ItemCaptionTop")));
	m_clrItemCaptionShadow.SetStandardValue(pImages->GetImageColor(_T("ShortcutBar"), _T("ItemCaptionShadow")));

	m_grcShortcutItem.SetStandardValue(RGB(227, 239, 255), RGB(192, 219, 255), 0.25f);

	m_clrPopupFrame.SetStandardValue(m_clrShortcutItemShadow);
	m_clrPopupBackground.SetStandardValue(pImages->GetImageColor(_T("ShortcutBar"), _T("CaptionLight")));


	m_bFlatStyle = pImages->GetImageInt(_T("ShortcutBar"), _T("FlatStyle"), FALSE);

	if (m_bFlatStyle)
	{
		m_clrFlatBackground = pImages->GetImageColor(_T("ShortcutBar"), _T("FlatBackground"));
		m_clrFlatTextColor = m_clrItemCaptionText;
	}

	m_clrAppSplitterFace = pImages->GetImageColor(_T("Window"), _T("ButtonFace"));
	m_clrAppSplitterFace = pImages->GetImageColor(_T("Window"), _T("SplitterBackground"), m_clrAppSplitterFace);

	if (m_bFlatStyle)
	{
		m_bShowBorder = FALSE;
		m_bShowTopLeftCaptionShadow = FALSE;
		m_bBoldItemCaptionFont = FALSE;
	}
	else
	{
		m_bShowBorder = TRUE;
		m_bShowTopLeftCaptionShadow = TRUE;
		m_bBoldItemCaptionFont = TRUE;
	}

	LOGFONT lf;
	m_fntPaneItemCaption.GetLogFont(&lf);
	lf.lfWeight = m_bBoldItemCaptionFont ? FW_BOLD : FW_NORMAL;

	m_fntPaneItemCaption.DeleteObject();
	m_fntPaneItemCaption.CreateFontIndirect(&lf);

}

void CXTPShortcutBarResourceTheme::DrawPaneCaptionMinimizeButton(CDC* pDC, CXTPShortcutBarPane* pPane)
{
	CRect rcMinimizeButton = pPane->GetMinimizeButtonRect();
	if (!rcMinimizeButton.IsRectEmpty())
	{
		CXTPResourceImages* pImages = XTPResourceImages();

		if (pPane->IsMinimizeButtonHighlighted() || pPane->IsMinimizeButtonPressed())
		{
			//if (!m_bFlatStyle)
			{
				CXTPResourceImage* pImage = pImages->LoadFile(_T("SHORTCUTBARITEMS"));
				int nState = pPane->IsMinimizeButtonPressed() && pPane->IsMinimizeButtonHighlighted() ? 3 : 1;
				pImage->DrawImage(pDC, rcMinimizeButton, pImage->GetSource(nState, 4));
			}
		}


		CXTPResourceImage* pImage = pImages->LoadFile(_T("SHORTCUTBARGLYPHS"));
		ASSERT(pImage);
		if (pImage)
		{


			CRect rcSrc(pImage->GetSource(pPane->GetShortcutBar()->IsShortcutBarMinimized() ? 0 : 1, 5));

			CRect rcDest(CPoint((rcMinimizeButton.left + rcMinimizeButton.right - rcSrc.Width()) / 2, (rcMinimizeButton.top + rcMinimizeButton.bottom - rcSrc.Height()) / 2), rcSrc.Size());
			pImage->DrawImage(pDC, rcDest, rcSrc);
		}
	}

}


int CXTPShortcutBarResourceTheme::DrawPaneCaption(CDC* pDC, CXTPShortcutBarPane* pPane, BOOL bDraw)
{
	if (pPane->IsFlatStyle())
	{
		if (bDraw)
		{

			CRect rcMinimizeButton = pPane->GetMinimizeButtonRect();
			if (!rcMinimizeButton.IsRectEmpty())
			{
				DrawPaneCaptionMinimizeButton(pDC, pPane);
			}
		}
		return 18;
	}
	else
	{
		if (bDraw)
		{

			CXTPFontDC font(pDC, &m_fntPaneCaption);

			CXTPClientRect rc(pPane);
			rc.bottom = rc.top + 24;
			XTPDrawHelpers()->GradientFill(pDC, rc, m_grcCaption, FALSE);

			if (m_bShowTopLeftCaptionShadow)
			{
				pDC->FillSolidRect(0, rc.top, rc.Width(), 1, RGB(0xFF, 0xFF, 0xFF));
				pDC->FillSolidRect(0, rc.top, 1, rc.Height(), RGB(0xFF, 0xFF, 0xFF));
			}

			CRect rcText(rc);

			CRect rcMinimizeButton = pPane->GetMinimizeButtonRect();
			if (!rcMinimizeButton.IsRectEmpty())
			{
				DrawPaneCaptionMinimizeButton(pDC, pPane);

				rcText.right = rcMinimizeButton.left - 2;
			}

			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(m_clrCaptionText);

			rcText.DeflateRect(m_rcCaptionPadding);
			pDC->DrawText(pPane->GetCaption(), rcText, DT_LEFT | DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS | (m_bDrawTextNoPrefix ? DT_NOPREFIX : 0));
		}
		return 24;
	}
}

int CXTPShortcutBarResourceTheme::DrawPaneItemCaption(CDC* pDC, CXTPShortcutBarPaneItem* pItem, BOOL bDraw)
{
	if (bDraw)
	{
		CXTPResourceImages* pImages = XTPResourceImages();

		CXTPFontDC font(pDC, &m_fntPaneItemCaption);

		CRect rc = pItem->GetCaptionRect();

		COLORREF clrLight = pItem->IsHighlighted() ? m_grcItemHotCaption.clrLight : m_grcItemCaption.clrLight;
		COLORREF clrDark = pItem->IsHighlighted() ? m_grcItemHotCaption.clrDark : m_grcItemCaption.clrDark;

		XTPDrawHelpers()->GradientFill(pDC, CRect(rc.left, rc.top, rc.CenterPoint().x, rc.bottom), clrDark, clrLight, TRUE);
		XTPDrawHelpers()->GradientFill(pDC, CRect(rc.CenterPoint().x, rc.top, rc.right, rc.bottom), clrLight, clrDark, TRUE);

		if (!m_bFlatStyle)
		{
			pDC->FillSolidRect(0, rc.bottom - 1, rc.Width(), 1, m_clrItemCaptionShadow);
			pDC->FillSolidRect(0, rc.top, rc.Width(), 1, m_clrItemCaptionTop);

			if (m_bShowTopLeftCaptionShadow)
			{
				pDC->FillSolidRect(0, rc.top + 1, rc.Width(), 1, RGB(0xFF, 0xFF, 0xFF));
				pDC->FillSolidRect(0, rc.top + 1, 1, rc.Height() - 2, RGB(0xFF, 0xFF, 0xFF));
			}
		}

		CRect rcText(rc);
		rcText.DeflateRect(m_rcItemCaptionPadding);

		if (pItem->IsExpandable())
		{
			CXTPResourceImage* pImage = pImages->LoadFile(_T("SHORTCUTBARGLYPHS"));
			ASSERT(pImage);
			if (!pImage)
				return m_nPaneItemHeight;

			CRect rcSrc(pImage->GetSource(pItem->IsExpanded() ? 2 : 3, 5));

			if (!m_bFlatStyle)
			{
				CRect rcDest(CPoint(rc.right - 2 - rcSrc.Width(), (rc.top + rc.bottom + 2 - rcSrc.Height()) / 2), rcSrc.Size());
				pImage->DrawImage(pDC, rcDest, rcSrc);

				rcText.right = rcDest.left - 2;
			}
			else
			{
				CRect rcDest(CPoint(rc.left + 3, (rc.top + rc.bottom + 2 - rcSrc.Height()) / 2), rcSrc.Size());
				pImage->DrawImage(pDC, rcDest, rcSrc);

				rcText.left = rcDest.right + 2;

			}
		}

		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(m_clrItemCaptionText);

		pDC->DrawText(pItem->GetCaption(), rcText, DT_LEFT | DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS | (m_bDrawTextNoPrefix ? DT_NOPREFIX : 0));

	}

	return m_nPaneItemHeight;
}

void CXTPShortcutBarResourceTheme::FillShortcutBar(CDC* pDC, CXTPShortcutBar* pShortcutBar)
{

	CXTPClientRect rc(pShortcutBar);

	if (m_bFlatStyle)
	{
		pDC->FillSolidRect(rc, m_clrFlatBackground);
	}
	else
	{
		CXTPResourceImages* pImages = XTPResourceImages();

		CXTPResourceImage* pImage = pImages->LoadFile(_T("SHORTCUTBARITEMS"));
		if (!pImage)
		{
			CXTPShortcutBarOffice2003Theme::FillShortcutBar(pDC, pShortcutBar);
			return;
		}

		int nItemHeight = pShortcutBar->GetItemSize().cy;

		if (pShortcutBar->IsClientPaneVisible())
		{
			pDC->FillSolidRect(rc, GetXtremeColor(COLOR_WINDOW));

			int nTopExpandedLines = pShortcutBar->GetGripperRect().bottom +
				(pShortcutBar->GetExpandedLinesCount() - 1) * nItemHeight;

			pImage->DrawImage(pDC, CRect(0, nTopExpandedLines, rc.Width(), rc.bottom), pImage->GetSource(0, 4), CRect(2, 2, 2, 2));
		}
		else
		{
			pImage->DrawImage(pDC, CRect(0, (pShortcutBar->GetExpandedLinesCount() - 1) * nItemHeight - 1, rc.Width(),
				rc.Height()), pImage->GetSource(0, 4), CRect(2, 2, 2, 2));
		}
	}
}

void CXTPShortcutBarResourceTheme::FillShortcutItemEntry(CDC* pDC, CXTPShortcutBarItem* pItem)
{
	CXTPResourceImages* pImages = XTPResourceImages();


	CXTPResourceImage* pImage = pImages->LoadFile(_T("SHORTCUTBARITEMS"));

	if (!pImage)
	{
		CXTPShortcutBarOffice2003Theme::FillShortcutItemEntry(pDC, pItem);
		return;
	}

	CXTPShortcutBar* pShortcutBar = pItem->GetShortcutBar();
	CRect rc = pItem->GetItemRect();
	CXTPClientRect rcShortcutBar(pShortcutBar);

	if (m_bFlatStyle)
	{
		if (!pItem->IsExpanded())
		{
			CXTPResourceImage* pImageSmall = pImages->LoadFile(_T("SHORTCUTBARSMALLITEMS"));
			if (pImageSmall)
			{
				pImage = pImageSmall;
				rc.DeflateRect(0, 2, 0, 2);
			}
		}

		if (pItem->IsSelected() && pShortcutBar->GetHotItem() == pItem)
		{
			pImage->DrawImage(pDC, rc, pImage->GetSource(1, 4), CRect(4, 4, 4, 4));
		}
		else if (pShortcutBar->GetPressedItem() == pItem)
		{
			pImage->DrawImage(pDC, rc, pImage->GetSource(3, 4), CRect(4, 4, 4, 4));
			pDC->SetTextColor(m_clrPushedText);
		}
		else if (pShortcutBar->GetHotItem() == pItem)
		{
			pImage->DrawImage(pDC, rc, pImage->GetSource(2, 4), CRect(4, 4, 4, 4));
			pDC->SetTextColor(m_clrPushedText);
		}
		else if (pItem->IsSelected())
		{
			pImage->DrawImage(pDC, rc, pImage->GetSource(0, 4), CRect(4, 4, 4, 4));
		}

	}
	else
	{
		if (pItem->IsSelected() &&
			(pShortcutBar->GetHotItem() != pItem || (pShortcutBar->GetPressedItem() != NULL && pShortcutBar->GetPressedItem() != pItem)))
		{
			pImage->DrawImage(pDC, rc, pImage->GetSource(2, 4), CRect(2, 2, 2, 2));
		}
		else if (pItem->IsSelected() ||
			(pShortcutBar->GetHotItem() == pItem && pShortcutBar->GetPressedItem() == pItem))
		{
			pImage->DrawImage(pDC, rc, pImage->GetSource(3, 4), CRect(2, 2, 2, 2));
			pDC->SetTextColor(m_clrPushedText);
		}
		else if ((pShortcutBar->GetHotItem() == pItem && pShortcutBar->GetPressedItem() == NULL) ||
			pShortcutBar->GetPressedItem() == pItem)
		{
			pImage->DrawImage(pDC, rc, pImage->GetSource(1, 4), CRect(2, 2, 2, 2));
			pDC->SetTextColor(m_clrHotText);
		}
		else if (pItem->IsExpanded())
		{
			pImage->DrawImage(pDC, rc, pImage->GetSource(0, 4), CRect(2, 2, 2, 2));
		}

		if (pItem->IsExpanded() && rc.bottom != rcShortcutBar.bottom)
		{
			pDC->FillSolidRect(0, rc.bottom - 1, rc.Width(), 1, m_clrShortcutItemShadow);
		}
	}

	if (pItem->IsSelected())
	{
		pDC->SetTextColor(m_clrSelectedText);
	}

}

void CXTPShortcutBarResourceTheme::FillNavigationItemEntry(CDC* pDC, CXTPShortcutBarItem* pItem)
{
	CXTPResourceImages* pImages = XTPResourceImages();

	CXTPShortcutBar* pShortcutBar = pItem->GetShortcutBar();
	CRect rc = pItem->GetItemRect();

	if (m_bFlatStyle)
	{
		CXTPResourceImage* pImage = pImages->LoadFile(_T("SHORTCUTBARNAVIGATEITEM"));

		int nState = pItem->IsExpanded() ? 3 : pShortcutBar->GetHotItem() == pItem && pShortcutBar->GetPressedItem() == pItem ? 3 :
		pShortcutBar->GetHotItem() == pItem ? 2 : -1;

		if (nState >= 0)
			pImage->DrawImage(pDC, rc, pImage->GetSource(nState, 4), CRect(6, 6, 6, 6));

	}
	else
	{
		CXTPResourceImage* pImage = pImages->LoadFile(_T("SHORTCUTBARNAVIGATEITEM"));

		if (!pImage)
		{
			CXTPShortcutBarOffice2003Theme::FillNavigationItemEntry(pDC, pItem);
			return;
		}

		CXTPClientRect rcShortcutBar(pShortcutBar);

		int nState = pItem->IsExpanded() ? 3 : pShortcutBar->GetHotItem() == pItem && pShortcutBar->GetPressedItem() == pItem ? 2 :
		pShortcutBar->GetHotItem() == pItem ? 1 : 0;


		pImage->DrawImage(pDC, rc, pImage->GetSource(nState, 4), CRect(2, 2, 2, 2));
	}
}

void CXTPShortcutBarResourceTheme::DrawExpandButton(CDC* pDC, CRect rc)
{
	CXTPResourceImages* pImages = XTPResourceImages();

	CXTPResourceImage* pImage = pImages->LoadFile(_T("SHORTCUTBARGLYPHS"));
	ASSERT(pImage);
	if (!pImage)
	{
		CXTPShortcutBarOffice2003Theme::DrawExpandButton(pDC, rc);
		return;
	}

	CRect rcSrc(pImage->GetSource(4, 5));
	CRect rcDest(CPoint((rc.left + rc.right - rcSrc.Width()) / 2, (rc.top + rc.bottom - rcSrc.Height()) / 2), rcSrc.Size());

	pImage->DrawImage(pDC, rcDest, rcSrc);
}

int CXTPShortcutBarResourceTheme::DrawGripper(CDC* pDC, CXTPShortcutBar* pShortcutBar, BOOL bDraw)
{
	if (bDraw)
	{
		CXTPResourceImages* pImages = XTPResourceImages();

		CXTPResourceImage* pImage = pImages->LoadFile(_T("SHORTCUTBARGRIPPER"));
		ASSERT(pImage);
		if (!pImage)
		{
			return CXTPShortcutBarOffice2003Theme::DrawGripper(pDC, pShortcutBar, bDraw);
		}

		CRect rc = pShortcutBar->GetGripperRect();

		if (m_bFlatStyle)
		{
			rc.DeflateRect(0, 0, 0, 2);
		}

		if (m_bFlatStyle)
		{
			XTPDrawHelpers()->GradientFill(pDC, CRect(rc.left + 1, rc.top + 1, rc.right - 1, rc.bottom - 1), m_grcShortcutBarGripper, FALSE);
			pDC->Draw3dRect(rc.left + 1, rc.top, rc.Width() - 2, rc.Height(), m_clrShortcutItemShadow, m_clrShortcutItemShadow);

		}
		else
		{
			XTPDrawHelpers()->GradientFill(pDC, rc, m_grcShortcutBarGripper, FALSE);

			pDC->FillSolidRect(rc.left, rc.top, rc.Width(), 1, m_clrShortcutItemShadow);
			pDC->FillSolidRect(rc.left, rc.top + 1, rc.Width(), 1, RGB(0xFF, 0xFF, 0xFF));
			pDC->FillSolidRect(rc.left, rc.bottom - 1, rc.Width(), 1, m_clrShortcutItemShadow);

			CRect rcSrc(pImage->GetSource());
			CRect rcDest(CPoint((rc.left + rc.right - rcSrc.Width()) / 2, (rc.top + rc.bottom - rcSrc.Height()) / 2), rcSrc.Size());

			pImage->DrawImage(pDC, rcDest, rcSrc);
		}
	}

	return m_bFlatStyle ? 7 : 9;
}
