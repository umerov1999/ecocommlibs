// XTPTabPaintManager.cpp: implementation of the CXTPTabPaintManager class.
//
// This file is a part of the XTREME TOOLKIT PRO MFC class library.
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

#include "stdafx.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPResourceImage.h"
#include "Common/XTPImageManager.h"

#include <TabManager/XTPTabManagerDefines.h>
#include <TabManager/XTPTabManagerAtom.h>
#include <TabManager/XTPTabManagerNavigateButton.h>
#include <TabManager/XTPTabManagerNavigateButtons.h>
#include <TabManager/XTPTabManagerItem.h>
#include <TabManager/XTPTabManager.h>
#include <TabManager/XTPTabPaintManagerColorSet.h>
#include <TabManager/XTPTabPaintManagerTheme.h>
#include <TabManager/XTPTabPaintManager.h>
#include <TabManager/XTPTabPaintManager.inl>

#ifndef _countof
#define _countof(array) (sizeof(array)/sizeof(array[0]))
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif



//////////////////////////////////////////////////////////////////////////
// CXTPTabPaintManagerTheme

CXTPTabPaintManagerTheme::CXTPTabPaintManagerTheme()
{
	m_pPaintManager = NULL;
	m_rcHeaderMargin.SetRect(0, 0, 0, 0);
	m_nRowMargin = -2;
	m_bButtonsReverseZOrder = FALSE;

}
CXTPTabPaintManagerTheme::~CXTPTabPaintManagerTheme()
{
}

void CXTPTabPaintManagerTheme::DrawText(CDC* pDC, CXTPTabManager* /*pManager*/, const CString& str, LPRECT lpRect, UINT nFormat)
{
	pDC->DrawText(str, lpRect, nFormat);
}

void CXTPTabPaintManagerTheme::RefreshMetrics()
{
	CWindowDC dc (NULL);
	CXTPFontDC fnt(&dc, m_pPaintManager->GetFont());
	CSize szRect = dc.GetTextExtent(_T(" "), 1);

	m_nButtonHeight = max(18, szRect.cy + 3);
}

CRect CXTPTabPaintManagerTheme::GetHeaderClipBox(CDC* pDC, CXTPTabManager* pTabManager)
{
	CRect rcClipBox;
	pDC->GetClipBox(rcClipBox);

	rcClipBox.IntersectRect(rcClipBox, pTabManager->GetHeaderRect());

	return rcClipBox;
}

CRect CXTPTabPaintManagerTheme::GetButtonDrawRect(const CXTPTabManagerItem* pItem)
{
	CRect rc = pItem->GetRect();
	rc.InflateRect(2, 2);

	return rc;
}

int CXTPTabPaintManagerTheme::GetButtonHeight(const CXTPTabManager* pTabManager)
{
	CSize szButtonMin(0);

	int nMargin = m_pPaintManager->m_rcButtonMargin.top + m_pPaintManager->m_rcButtonMargin.bottom;

	if (pTabManager) pTabManager->GetItemMetrics(NULL, &szButtonMin, NULL);
	int nButtonHeight = max(m_nButtonHeight + nMargin, szButtonMin.cy);

	if (!m_pPaintManager->m_bShowIcons)
		return nButtonHeight;

	CSize szIcon(m_pPaintManager->m_szIcon);
	if (pTabManager) pTabManager->DrawIcon(NULL, 0, NULL, FALSE, szIcon);

	return max(max(szIcon.cx, szIcon.cy) + 4 + nMargin, nButtonHeight);

}


void CXTPTabPaintManagerTheme::DrawPolyLine(CDC* pDC, COLORREF clr, LPPOINT pts, int nCount)
{
	CXTPPenDC pen(*pDC, clr);
	pDC->MoveTo(pts[0].x, pts[0].y);
	CPoint pt(pts[0]);

	for (int i = 1; i < nCount; i++)
	{
		pt.Offset(pts[i]);
		pDC->LineTo(pt);
	}
}

void CXTPTabPaintManagerTheme::InflateRectEx(CRect& rc, CRect rcInflate, XTPTabPosition position)
{
	switch (position)
	{
		case xtpTabPositionTop: rc.top -= rcInflate.top; rc.left -= rcInflate.left; rc.bottom += rcInflate.bottom; rc.right += rcInflate.right; break;
		case xtpTabPositionLeft: rc.left -= rcInflate.top; rc.bottom += rcInflate.right; rc.right += rcInflate.bottom;  rc.top -= rcInflate.left; break;
		case xtpTabPositionBottom: rc.bottom += rcInflate.top; rc.right += rcInflate.right; rc.top -= rcInflate.bottom; rc.left -= rcInflate.left; break;
		case xtpTabPositionRight: rc.right += rcInflate.top; rc.top -= rcInflate.left; rc.left -= rcInflate.bottom; rc.bottom += rcInflate.right; break;

	}
}

void CXTPTabPaintManagerTheme::DeflateRectEx(CRect& rc, CRect rcDeflate, XTPTabPosition position)
{
	InflateRectEx(rc, CRect(-rcDeflate.left, -rcDeflate.top, -rcDeflate.right, -rcDeflate.bottom), position);
}

void CXTPTabPaintManagerTheme::DrawSingleLineBorder(CDC* pDC, CRect rc, XTPTabPosition position, COLORREF clrTopLeft, COLORREF clrBottomRight)
{
	switch (position)
	{
		case xtpTabPositionTop:
			HorizontalLine(pDC, rc.left, rc.top, rc.Width(), clrTopLeft);
			break;
		case xtpTabPositionBottom:
			HorizontalLine(pDC, rc.left, rc.bottom - 1, rc.Width(), clrBottomRight);
			break;
		case xtpTabPositionLeft:
			VerticalLine(pDC, rc.left, rc.top, rc.Height(), clrTopLeft);
			break;
		case xtpTabPositionRight:
			VerticalLine(pDC, rc.right - 1, rc.top, rc.Height(), clrBottomRight);
			break;
	}
}


COLORREF CXTPTabPaintManagerTheme::FillButton(CDC* pDC, CXTPTabManagerItem* pItem, LPPOINT pts, int nCount, XTPTabPosition /*tabPosition*/, BOOL bAbsoulute)
{
	LPPOINT lpPointRgn = new POINT[nCount];
	CPoint pt(0);
	for (int i = 0; i < nCount; i++)
	{
		if (!bAbsoulute)
		{
			pt.Offset(pts[i]);
			lpPointRgn[i] = pt;
		}
		else
		{
			lpPointRgn[i] = pts[i];
		}
	}

	CRect rcClip;
	pDC->GetClipBox(rcClip);
	CRgn rgnClip;
	rgnClip.CreateRectRgnIndirect(rcClip);


	CRgn rgn;
	rgn.CreatePolygonRgn(lpPointRgn, nCount, ALTERNATE);

	pDC->SelectClipRgn(&rgn, RGN_AND);

	CRect rcFill;
	rgn.GetRgnBox(rcFill);

	CBitmap bmp;
	bmp.CreateCompatibleBitmap(pDC, rcFill.Width(), rcFill.Height());
	CXTPCompatibleDC dcButton(pDC, &bmp);

	COLORREF clrFill = GetColorSet()->FillPropertyButton(&dcButton, CRect(0, 0, rcFill.Width(), rcFill.Height()), pItem);

	pDC->BitBlt(rcFill.left, rcFill.top, rcFill.Width(), rcFill.Height(), &dcButton, 0, 0, SRCCOPY);

	pDC->SelectClipRgn(&rgnClip);
	delete[] lpPointRgn;

	return clrFill;
}

int CXTPTabPaintManagerTheme::GetButtonLength(CDC* pDC, CXTPTabManagerItem* pItem)
{
	return m_pPaintManager->DrawSingleButtonIconAndText(pDC, pItem, CXTPEmptyRect(), FALSE);
}

int CXTPTabPaintManagerTheme::GetHeaderHeight(CXTPTabManager* pTabManager)
{
	if (!m_pPaintManager->m_bShowTabs)
		return 0;

	int nMargin = m_rcHeaderMargin.top + m_rcHeaderMargin.bottom;

	int nButtonHeight = GetButtonHeight(pTabManager) * pTabManager->GetRowCount() +
		m_nRowMargin * (pTabManager->GetRowCount() - 1);

	if (pTabManager->GetLayout() == xtpTabLayoutRotated)
	{
		for (int i = 0; i < pTabManager->GetItemCount(); i++)
		{
			CXTPTabManagerItem* pItem = pTabManager->GetItem(i);

			if (pItem->GetButtonLength() > nButtonHeight)
				nButtonHeight = pItem->GetButtonLength();
		}

	}

	return nButtonHeight + nMargin;
}

CRect CXTPTabPaintManagerTheme::GetHeaderRect(CRect rcControl, CXTPTabManager* pTabManager)
{
	CRect rcHeader(rcControl);
	int nHeight = GetHeaderHeight(pTabManager);

	switch (pTabManager->GetPosition())
	{
		case xtpTabPositionTop: rcHeader.bottom = rcControl.top + nHeight; break;
		case xtpTabPositionLeft: rcHeader.right = rcControl.left + nHeight; break;
		case xtpTabPositionBottom: rcHeader.top = rcControl.bottom - nHeight; break;
		case xtpTabPositionRight: rcHeader.left = rcControl.right - nHeight; break;
	}
	return rcHeader;
}
CRect CXTPTabPaintManagerTheme::GetClientRect(CRect rcControl, CXTPTabManager* pTabManager)
{
	if (!m_pPaintManager->m_bShowTabs)
		return rcControl;

	CRect rcClient(rcControl);
	CRect rcHeader = GetHeaderRect(rcControl, pTabManager);

	switch (pTabManager->GetPosition())
	{
		case xtpTabPositionTop: rcClient.top = rcHeader.bottom - 1; break;
		case xtpTabPositionLeft: rcClient.left = rcHeader.right - 1; break;
		case xtpTabPositionBottom: rcClient.bottom = rcHeader.top + 1; break;
		case xtpTabPositionRight: rcClient.right = rcHeader.left + 1; break;
	}
	return rcClient;
}

CRect CXTPTabPaintManagerTheme::GetClientMargin()
{
	return m_pPaintManager->m_rcClientMargin;
}


void CXTPTabPaintManagerTheme::AdjustClientRect(CXTPTabManager* pTabManager, CRect& rcClient)
{
	DeflateRectEx(rcClient, m_pPaintManager->m_rcControlMargin, pTabManager->GetPosition());
	rcClient = GetClientRect(rcClient, pTabManager);

	DeflateRectEx(rcClient, GetClientMargin(), pTabManager->GetPosition());

	if (m_pPaintManager->m_clientFrame == xtpTabFrameBorder)
	{
		DeflateRectEx(rcClient, CRect(2, 2, 2, 2), pTabManager->GetPosition());
	}
	if (m_pPaintManager->m_clientFrame == xtpTabFrameSingleLine)
	{
		DeflateRectEx(rcClient, CRect(0, 2, 0, 0), pTabManager->GetPosition());
	}
}

void CXTPTabPaintManagerTheme::DrawWorkspacePart(CDC* pDC, LPRECT lpRect, XTPTabWorkspacePart workspacePart)
{
	if (!m_pPaintManager->m_bStaticFrame)
	{

		switch (workspacePart)
		{
		case xtpTabWorkspacePartBorder:
			break;

		case xtpTabWorkspacePartWidth:
			lpRect->top -= 2;
			lpRect->left -= 2;
			lpRect->right += 2;
			lpRect->bottom += 2;
			break;

		case xtpTabWorkspacePartVSplitter:
			pDC->FillSolidRect(lpRect, GetColorSet()->m_clrHeaderFace.clrLight);
			break;

		case xtpTabWorkspacePartHSplitter:
			pDC->FillSolidRect(lpRect, GetColorSet()->m_clrHeaderFace.clrLight);
			break;
		}
	}
	else
	{

		switch (workspacePart)
		{
		case xtpTabWorkspacePartBorder:
			pDC->Draw3dRect(1, 0, lpRect->right - lpRect->left - 2, lpRect->bottom - lpRect->top - 1, GetXtremeColor(COLOR_3DFACE), GetXtremeColor(COLOR_3DFACE));
			pDC->Draw3dRect(0, 0, lpRect->right - lpRect->left, lpRect->bottom - lpRect->top, GetColorSet()->m_clrFrameBorder, GetColorSet()->m_clrFrameBorder);
			break;

		case xtpTabWorkspacePartWidth:
			lpRect->top -= 1;
			break;

		case xtpTabWorkspacePartVSplitter:
			pDC->FillSolidRect(lpRect, GetColorSet()->m_clrControlFace);
			pDC->FillSolidRect(lpRect->right - 1, lpRect->top, 1, lpRect->bottom - lpRect->top, GetColorSet()->m_clrFrameBorder);
			break;

		case xtpTabWorkspacePartHSplitter:
			pDC->FillSolidRect(lpRect, GetColorSet()->m_clrControlFace);
			pDC->FillSolidRect(lpRect->left, lpRect->bottom - 1, lpRect->right - lpRect->left, 1, GetColorSet()->m_clrFrameBorder);
			break;
		}
	}
}


//////////////////////////////////////////////////////////////////////////

CRect CXTPTabPaintManagerTheme::GetHeaderMargin()
{
	return m_rcHeaderMargin;
}

CRect CXTPTabPaintManagerTheme::FillTabControl(CXTPTabManager* /*pTabManager*/, CDC* /*pDC*/, CRect /*rcControl*/)
{
	return CRect(0, 0, 0, 0);
}

void CXTPTabPaintManagerTheme::DrawSingleButton(CDC* /*pDC*/, CXTPTabManagerItem* /*pItem*/)
{
}

void CXTPTabPaintManagerTheme::DrawTabControl(CXTPTabManager* pTabManager, CDC* pDC, CRect rcControl)
{
	m_pPaintManager->DrawTabControlEx(pTabManager, pDC, rcControl);
}

void CXTPTabPaintManagerTheme::RepositionTabControl(CXTPTabManager* pTabManager, CDC* pDC, CRect rcClient)
{
	m_pPaintManager->RepositionTabControlEx(pTabManager, pDC, rcClient);
}

CXTPTabPaintManagerColorSet* CXTPTabPaintManagerTheme::GetColorSet() const
{
	return m_pPaintManager->m_pColorSet;
}
