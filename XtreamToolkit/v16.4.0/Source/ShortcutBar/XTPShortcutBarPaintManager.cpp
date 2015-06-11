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
#include "Resource.h"

#include <Common/XTPVC80Helpers.h>
#include <Common/XTPDrawHelpers.h>
#include <Common/XTPImageManager.h>
#include <Common/XTPResourceImage.h>
#include <Common/XTPSystemHelpers.h>
#include <Common/XTPResourceManager.h>
#include <Common/XTPColorManager.h>
#include <Common/XTPWinThemeWrapper.h>

#include <ShortcutBar/XTPShortcutBarPaintManager.h>
#include <ShortcutBar/XTPShortcutBar.h>
#include <ShortcutBar/XTPShortcutBarPane.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXTPShortcutBarPaintManager::CXTPShortcutBarPaintManager()
{
	m_bBoldItemCaptionFont = FALSE;
	m_pExpandIcon = NULL;
	m_bClearTypeTextQuality = FALSE;

	CXTPImageManagerIconHandle iconHandle;

	HMODULE hModule = AfxFindResourceHandle(MAKEINTRESOURCE(XTP_IDB_SHORTCUT_EXPANDBUTTON), _T("PNG"));
	if (!hModule) hModule = XTPResourceManager()->GetResourceHandle();

	iconHandle.Attach(CXTPImageManagerIcon::LoadBitmapFromResource(hModule, MAKEINTRESOURCE(XTP_IDB_SHORTCUT_EXPANDBUTTON), NULL), FALSE);

	ASSERT(!iconHandle.IsEmpty());

	if (!iconHandle.IsEmpty())
	{
		m_pExpandIcon = new CXTPImageManagerIcon(0, 16, 16);
		m_pExpandIcon->SetNormalIcon(iconHandle);
	}

	m_nNormalIconWidth = 16;
	m_nExpandedIconWidth = 24;
	m_bUseStandardFont = TRUE;
	m_nPaneItemHeight = 0;

	m_rcCaptionPadding.SetRect(4, 0, 4, 0);
	m_rcItemCaptionPadding.SetRect(7, 0, 7, 0);

	m_bShowBorder = TRUE;

	m_bDrawTextNoPrefix = TRUE;

#ifdef _XTP_ACTIVEX
	EnableAutomation();
	EnableTypeLib();
#endif

}


CXTPShortcutBarPaintManager::~CXTPShortcutBarPaintManager()
{
	SAFE_DELETE(m_pExpandIcon);
}


void CXTPShortcutBarPaintManager::RefreshMetrics()
{
	RefreshXtremeColors(FALSE);

	m_fntPaneCaption.DeleteObject();
	m_fntPaneItemCaption.DeleteObject();
	m_fntVerticalText.DeleteObject();

	LOGFONT lf;
	VERIFY(CXTPDrawHelpers::GetIconLogFont(&lf));

	if (m_bClearTypeTextQuality && XTPSystemVersion()->IsClearTypeTextQualitySupported())
	{
		lf.lfQuality = 5;
	}
	BOOL bOfficeFont = !m_strOfficeFont.IsEmpty() && XTPDrawHelpers()->FontExists(m_strOfficeFont);

	if (bOfficeFont)
	{
		STRCPY_S(lf.lfFaceName, LF_FACESIZE, m_strOfficeFont);
	}

	lf.lfWeight = m_bBoldItemCaptionFont ? FW_BOLD : FW_NORMAL;
	m_fntPaneItemCaption.CreateFontIndirect(&lf);

	if (m_bUseStandardFont || !m_fntText.GetSafeHandle())
	{
		lf.lfWeight = FW_BOLD;
		m_fntText.DeleteObject();
		m_fntText.CreateFontIndirect(&lf);
	}

	if (bOfficeFont)
	{
		STRCPY_S(lf.lfFaceName, LF_FACESIZE, m_strOfficeFont);
	}
	else
	{
		STRCPY_S(lf.lfFaceName, LF_FACESIZE, _T("Arial"));
	}
	lf.lfWeight = FW_BOLD;
	lf.lfHeight = 20;
	m_fntPaneCaption.CreateFontIndirect(&lf);



	lf.lfWeight = FW_BOLD;
	lf.lfOrientation = 900;
	lf.lfEscapement = 900;

	if (bOfficeFont)
	{
		lf.lfHeight = 21;
		STRCPY_S(lf.lfFaceName, LF_FACESIZE, m_strOfficeFont);
	}
	else
	{
		lf.lfHeight = 18;
		STRCPY_S(lf.lfFaceName, LF_FACESIZE, CXTPDrawHelpers::GetVerticalFontName(bOfficeFont));
	}

	m_fntVerticalText.CreateFontIndirect(&lf);



	CWindowDC dc(CWnd::GetDesktopWindow());
	CXTPFontDC font(&dc, &m_fntPaneItemCaption);
	m_nPaneItemHeight = max(19, dc.GetTextExtent(_T(" "), 1).cy + 4);

	m_clrPushedText.SetStandardValue(GetXtremeColor(XPCOLOR_PUSHED_TEXT));
	m_clrSelectedText.SetStandardValue(GetXtremeColor(XPCOLOR_CHECKED_TEXT));
	m_clrHotText.SetStandardValue(GetXtremeColor(XPCOLOR_HIGHLIGHT_TEXT));
	m_clrNormalText.SetStandardValue(GetXtremeColor(COLOR_BTNTEXT));
	m_clrDisabledText.SetStandardValue(GetXtremeColor(COLOR_GRAYTEXT));
	m_clrPopupFrame.SetStandardValue(GetXtremeColor(COLOR_BTNSHADOW));
	m_clrPopupBackground.SetStandardValue(GetXtremeColor(COLOR_BTNFACE));

	m_clrFlatBackground = GetXtremeColor(COLOR_WINDOW);
	m_clrFlatTextColor = GetXtremeColor(COLOR_WINDOWTEXT);
	m_clrAppSplitterFace = GetXtremeColor(COLOR_3DFACE);

}

void CXTPShortcutBarPaintManager::DrawPaneCaptionMinimizeButton(CDC* /*pDC*/, CXTPShortcutBarPane* /*pPane*/)
{

}

void CXTPShortcutBarPaintManager::DrawExpandButton(CDC* pDC, CRect rc)
{
	CPoint ptImage (rc.CenterPoint().x - 8, rc.CenterPoint().y - 8);

	if (m_pExpandIcon)
	{
		m_pExpandIcon->Draw(pDC, ptImage);
	}
}

BOOL CXTPShortcutBarPaintManager::IsShortcutItemTruncated(CDC* pDC, CXTPShortcutBarItem* pItem)
{
	if (!pItem->IsExpanded())
		return FALSE;

	int nTexetWidth = pItem->GetItemRect().Width() - 5;

	CXTPImageManagerIcon* pImage = pItem->GetImage(m_nExpandedIconWidth);
	if (pImage)
	{
		nTexetWidth -= m_nExpandedIconWidth + 5;
	}

	CRect rcText(0, 0, 0, 0);
	CXTPFontDC font(pDC, &m_fntText);
	pDC->DrawText(pItem->GetCaption(), rcText, DT_LEFT | DT_SINGLELINE | DT_CALCRECT | (m_bDrawTextNoPrefix ? DT_NOPREFIX : 0));

	return rcText.Width() > nTexetWidth;
}

void CXTPShortcutBarPaintManager::DrawNavigationItem(CDC* pDC, CXTPShortcutBarItem* pItem)
{
	CRect rc = pItem->GetItemRect();

	pDC->SetBkMode(TRANSPARENT);

	pDC->SetTextColor(m_clrNormalText);
	FillNavigationItemEntry(pDC, pItem);

	CXTPFontDC font(pDC, &m_fntVerticalText);

	CSize sz = pDC->GetTextExtent(pItem->GetCaption());

	CRect rcText(CPoint((rc.left + rc.right - sz.cy) / 2, min(rc.bottom - 3, (rc.top + rc.bottom + sz.cx) / 2)), CSize(sz.cy, sz.cx));


	pDC->IntersectClipRect(rc);

	pDC->DrawText(pItem->GetCaption(), rcText, DT_SINGLELINE | DT_NOCLIP | (m_bDrawTextNoPrefix ? DT_NOPREFIX : 0));

	pDC->SelectClipRgn(NULL);

}


void CXTPShortcutBarPaintManager::DrawShortcutItem(CDC* pDC, CXTPShortcutBarItem* pItem)
{

	CRect rc = pItem->GetItemRect();


	CXTPShortcutBar* pBar = pItem->GetShortcutBar();

	pDC->SetTextColor(pItem->IsEnabled() ? m_clrNormalText : m_clrDisabledText);
	FillShortcutItemEntry(pDC, pItem);

	CSize szIcon(pItem->IsExpanded() ? m_nExpandedIconWidth : m_nNormalIconWidth, 0);
	CXTPImageManagerIcon* pImage = pItem->GetImage(szIcon.cx);

	BOOL bSelected = pBar->GetHotItem() == pItem && (pBar->GetPressedItem() == NULL || pBar->GetPressedItem() == pItem),
		bPressed = pBar->GetPressedItem() == pItem, bChecked = pItem->IsSelected();


	if (pItem->IsExpanded())
	{
		CRect rcText(rc);
		rcText.left = 5;

		if (pImage)
		{
			CPoint ptImage (5, rc.CenterPoint().y - m_nExpandedIconWidth / 2);
			DrawImage(pDC, ptImage, szIcon, pImage, bSelected, bPressed, bChecked, pItem->IsEnabled());

			rcText.left = 5 + m_nExpandedIconWidth + 5;

		}

		pDC->SetBkMode(TRANSPARENT);

		CXTPFontDC font(pDC, &m_fntText);
		pDC->DrawText(pItem->GetCaption(), rcText, DT_VCENTER | DT_LEFT | DT_SINGLELINE | DT_END_ELLIPSIS | (m_bDrawTextNoPrefix ? DT_NOPREFIX : 0));

	}
	else
	{
		if (pItem->IsItemExpandButton())
		{
			DrawExpandButton(pDC, rc);

		}

		else if (pImage)
		{
			CPoint ptImage (rc.CenterPoint().x - m_nNormalIconWidth / 2, rc.CenterPoint().y - m_nNormalIconWidth / 2);
			DrawImage(pDC, ptImage, szIcon, pImage, bSelected, bPressed, bChecked, pItem->IsEnabled());
		}
	}
}

int CXTPShortcutBarPaintManager::DrawPaneCaption(CDC* pDC, CXTPShortcutBarPane* pPane, BOOL bDraw)
{
	if (bDraw)
	{

		CXTPFontDC font(pDC, &m_fntPaneCaption);

		CXTPClientRect rc(pPane);
		rc.bottom = rc.top + 24;
		pDC->FillSolidRect(rc, GetXtremeColor(COLOR_3DSHADOW));

		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(GetXtremeColor(COLOR_WINDOW));

		CRect rcText(rc);
		rcText.DeflateRect(m_rcCaptionPadding);
		pDC->DrawText(pPane->GetCaption(), rcText, DT_LEFT | DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS | (m_bDrawTextNoPrefix ? DT_NOPREFIX : 0));
	}
	return 24;
}

int CXTPShortcutBarPaintManager::DrawPaneItemCaption(CDC* pDC, CXTPShortcutBarPaneItem* pItem, BOOL bDraw)
{
	if (bDraw)
	{
		CXTPFontDC font(pDC, &m_fntPaneItemCaption);

		CRect rc = pItem->GetCaptionRect();
		pDC->FillSolidRect(rc, GetXtremeColor(COLOR_3DFACE));

		pDC->FillSolidRect(0, rc.bottom - 1, rc.Width(), 1, GetXtremeColor(COLOR_3DSHADOW));

		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(GetXtremeColor(COLOR_BTNTEXT));

		CRect rcText(rc);
		rcText.DeflateRect(m_rcItemCaptionPadding);
		pDC->DrawText(pItem->GetCaption(), rcText, DT_LEFT | DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS | (m_bDrawTextNoPrefix ? DT_NOPREFIX : 0));

	}

	return m_nPaneItemHeight;
}

void CXTPShortcutBarPaintManager::Rectangle(CDC* pDC, CRect rc, int nPen, int nBrush)
{
	pDC->FillSolidRect(rc, GetXtremeColor(nBrush));
	pDC->Draw3dRect(rc, GetXtremeColor(nPen), GetXtremeColor(nPen));
}
void CXTPShortcutBarPaintManager::Line(CDC* pDC, int x0, int y0, int x1, int y1, int nPen)
{
	CXTPPenDC pen(*pDC, GetXtremeColor(nPen));
	pDC->MoveTo(x0, y0);
	pDC->LineTo(x1, y1);
}


void CXTPShortcutBarPaintManager::FillNavigationItemEntry(CDC* pDC, CXTPShortcutBarItem* pItem)
{
	FillShortcutItemEntry(pDC, pItem);
}


#ifdef _XTP_ACTIVEX


BEGIN_DISPATCH_MAP(CXTPShortcutBarPaintManager, CCmdTarget)
	DISP_PROPERTY_ID(CXTPShortcutBarPaintManager, "NormalIconWidth", 1, m_nNormalIconWidth,  VT_I4)
	DISP_PROPERTY_ID(CXTPShortcutBarPaintManager, "ExpandedIconWidth", 2, m_nExpandedIconWidth, VT_I4)
	DISP_PROPERTY_NOTIFY_ID(CXTPShortcutBarPaintManager, "ClearTypeTextQuality", 4, m_bClearTypeTextQuality, OleRefreshMetrics, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPShortcutBarPaintManager, "Font", DISPID_FONT, OleGetFont, OleSetFont, VT_DISPATCH)

	DISP_PROPERTY_EX_ID(CXTPShortcutBarPaintManager, "PaneBackgroundColor", 5, OleGetFlatBackgroundColor, SetNotSupported, VT_COLOR)
	DISP_PROPERTY_EX_ID(CXTPShortcutBarPaintManager, "PaneTextColor", 6, OleGetFlatTextColor, SetNotSupported, VT_COLOR)
	DISP_PROPERTY_EX_ID(CXTPShortcutBarPaintManager, "SplitterBackgroundColor", 7, OleGetSplitterColor, SetNotSupported, VT_COLOR)

	DISP_PROPERTY_ID(CXTPShortcutBarPaintManager, "DrawTextNoPrefix", 8, m_bDrawTextNoPrefix, VT_BOOL)
END_DISPATCH_MAP()

// {678113E7-46A4-4f99-83D8-C6D3C2B390FC}
static const IID IID_IShortcutBarPaintManager =
{ 0x678113e7, 0x46a4, 0x4f99, { 0x83, 0xd8, 0xc6, 0xd3, 0xc2, 0xb3, 0x90, 0xfc } };


IMPLEMENT_OLETYPELIB_EX(CXTPShortcutBarPaintManager, IID_IShortcutBarPaintManager)


BEGIN_INTERFACE_MAP(CXTPShortcutBarPaintManager, CCmdTarget)
	INTERFACE_PART(CXTPShortcutBarPaintManager, IID_IShortcutBarPaintManager, Dispatch)
END_INTERFACE_MAP()

void CXTPShortcutBarPaintManager::OleRefreshMetrics()
{
	RefreshMetrics();
}

void CXTPShortcutBarPaintManager::OleSetFont(LPFONTDISP pFontDisp)
{
	ASSERT((pFontDisp == NULL) ||
		AfxIsValidAddress(pFontDisp, sizeof(IDispatch), FALSE));

	LOGFONT lf;
	if (AxGetLogFontFromDispatch(&lf, pFontDisp))
	{
		m_bUseStandardFont = FALSE;
		m_fntText.DeleteObject();
		m_fntText.CreateFontIndirect(&lf);
	}
	else
	{
		m_bUseStandardFont = TRUE;
	}
	RefreshMetrics();
}

LPFONTDISP CXTPShortcutBarPaintManager::OleGetFont()
{
	return AxCreateOleFont(&m_fntText, this, (LPFNFONTCHANGED)&CXTPShortcutBarPaintManager::OleSetFont);
}

OLE_COLOR CXTPShortcutBarPaintManager::OleGetFlatBackgroundColor()
{
	return m_clrFlatBackground;
}

OLE_COLOR CXTPShortcutBarPaintManager::OleGetFlatTextColor()
{
	return m_clrFlatTextColor;
}


OLE_COLOR CXTPShortcutBarPaintManager::OleGetSplitterColor()
{
	return m_clrAppSplitterFace;
}


#endif
