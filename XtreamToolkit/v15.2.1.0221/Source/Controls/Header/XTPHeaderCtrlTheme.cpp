// XTPHeaderCtrlTheme.cpp: implementation of the CXTPHeaderCtrlTheme class.
//
// This file is a part of the XTREME CONTROLS MFC class library.
// (c)1998-2012 Codejock Software, All Rights Reserved.
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

#include "Common/XTPWinThemeWrapper.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPResourceImage.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPImageManager.h"
#include "../Util/XTPControlTheme.h"

#include "XTPHeaderCtrlTheme.h"
#include "XTPHeaderCtrl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//=============================================================================
// CXTPHeaderCtrlTheme
//=============================================================================

CXTPHeaderCtrlTheme::CXTPHeaderCtrlTheme()
: m_sizePadding(6, 0)
, m_sizeArrow(8, 7)
, m_dwDrawStyle(0)
, m_iArrowPadding(10)
, m_bUseWinThemes(false)
{
	m_dwDrawStyle = HDR_XTP_WINDEF | HDR_XTP_HOTTRACKING | HDR_XTP_SORTARROW;

	m_themeHeader = new CXTPWinThemeWrapper();
}

CXTPHeaderCtrlTheme::~CXTPHeaderCtrlTheme()
{
	SAFE_DELETE(m_themeHeader);

}

void CXTPHeaderCtrlTheme::RefreshMetrics(CXTPHeaderCtrl* /*pHeader*/)
{
	m_crText = GetXtremeColor(COLOR_BTNTEXT);
	m_cr3DFace = GetXtremeColor(COLOR_3DFACE);
	m_cr3DShadow = GetXtremeColor(COLOR_3DSHADOW);
	m_cr3DDkShadow = GetXtremeColor(COLOR_3DDKSHADOW);
	m_cr3DHighLight = GetXtremeColor(COLOR_3DHIGHLIGHT);

	m_themeHeader->OpenThemeData(0, L"HEADER");
}

void CXTPHeaderCtrlTheme::OnDrawBackground(LPDRAWITEMSTRUCT lpDIS)
{
	CDC* pDC = CDC::FromHandle(lpDIS->hDC);
	CRect rcItem = lpDIS->rcItem;

	// fill background
	pDC->FillSolidRect(&rcItem, m_cr3DFace);
	rcItem.right += 2;

	// draw 3D borders.
	pDC->Draw3dRect(&rcItem, m_cr3DHighLight, m_cr3DDkShadow);
	rcItem.DeflateRect(1, 1);
	pDC->Draw3dRect(&rcItem, m_cr3DFace, m_cr3DShadow);
}

void CXTPHeaderCtrlTheme::OnDrawItemBackground(LPDRAWITEMSTRUCT lpDIS)
{
	CDC* pDC = CDC::FromHandle(lpDIS->hDC);

	if (lpDIS->itemState == ODS_SELECTED)
	{
		pDC->Draw3dRect(&lpDIS->rcItem, m_cr3DShadow, m_cr3DShadow);

		CXTPPenDC penDC(pDC->m_hDC, m_cr3DHighLight);
		pDC->MoveTo(lpDIS->rcItem.right, 1);
		pDC->LineTo(lpDIS->rcItem.right, lpDIS->rcItem.bottom-1);

		penDC.Color(m_cr3DFace);
		pDC->MoveTo(lpDIS->rcItem.left+1, lpDIS->rcItem.bottom-2);
		pDC->LineTo(lpDIS->rcItem.right-1, lpDIS->rcItem.bottom-2);
	}
	else
	{
		CXTPPenDC penDC(pDC->m_hDC, m_cr3DShadow);
		pDC->MoveTo(lpDIS->rcItem.right-2, 1);
		pDC->LineTo(lpDIS->rcItem.right-2, lpDIS->rcItem.bottom-1);

		penDC.Color(m_cr3DDkShadow);
		pDC->MoveTo(lpDIS->rcItem.right-1, 0);
		pDC->LineTo(lpDIS->rcItem.right-1, lpDIS->rcItem.bottom-1);

		penDC.Color(m_cr3DHighLight);
		pDC->MoveTo(lpDIS->rcItem.right, 1);
		pDC->LineTo(lpDIS->rcItem.right, lpDIS->rcItem.bottom-1);
	}
}

BOOL CXTPHeaderCtrlTheme::OnDrawBackgroundWinThemed(LPDRAWITEMSTRUCT lpDIS)
{
	if ((GetDrawStyle() & HDR_XTP_HOTTRACKING) == 0)
		return FALSE;

	if (m_themeHeader->DrawThemeBackground(lpDIS->hDC,
		0, HIS_NORMAL, &lpDIS->rcItem, NULL) != S_OK)
	{
		TRACE0("Error drawing background using WinTheme API.\n");
		return FALSE;
	}

	return TRUE;
}

BOOL CXTPHeaderCtrlTheme::OnDrawItemBackgroundWinThemed(LPDRAWITEMSTRUCT lpDIS)
{
	int iState = HIS_NORMAL;

	if (lpDIS->itemState & ODS_HOTLIGHT)
		iState = HIS_HOT;

	if (lpDIS->itemState & ODS_SELECTED)
		iState = HIS_PRESSED;

	if (m_themeHeader->DrawThemeBackground(lpDIS->hDC,
		HP_HEADERITEM, iState, &lpDIS->rcItem, NULL) != S_OK)
	{
		TRACE0("Error drawing background using WinTheme API.\n");
		return FALSE;
	}

	return TRUE;
}

void CXTPHeaderCtrlTheme::DrawSortArrow(CDC* pDC, LPPOINT ptsArrow, BOOL bAscending)
{
	CXTPPenDC dcPen(pDC->m_hDC, bAscending ? m_cr3DHighLight : m_cr3DShadow);
	pDC->MoveTo(ptsArrow[2]);
	pDC->LineTo(ptsArrow[bAscending ? 1 : 0]);
	pDC->LineTo(ptsArrow[bAscending ? 0 : 1]);

	dcPen.Color(bAscending ? m_cr3DShadow : m_cr3DHighLight);
	pDC->MoveTo(ptsArrow[bAscending ? 0 : 2]);
	pDC->LineTo(ptsArrow[bAscending ? 2 : 1]);
}

void CXTPHeaderCtrlTheme::OnDrawItemSortArrow(LPDRAWITEMSTRUCT lpDIS, BOOL bAscending)
{
	CDC* pDC = CDC::FromHandle(lpDIS->hDC);
	CRect rcItem = lpDIS->rcItem;

	CPoint point;
	point.x = rcItem.right + m_iArrowPadding;
	point.y = (rcItem.top - 1) + (rcItem.Height() - m_sizeArrow.cy) / 2;

	if (point.x < rcItem.left)
		return;

	if (lpDIS->itemState == ODS_SELECTED)
		point.Offset(1, 1);

	CRect rcArrow;
	rcArrow.left = point.x;
	rcArrow.top = point.y;
	rcArrow.right = point.x + m_sizeArrow.cx;
	rcArrow.bottom = point.y + m_sizeArrow.cy;

	int x = point.x + (m_sizeArrow.cx/2);
//  int y = point.y + (m_sizeArrow.cy/2);

	POINT ptsArrow[3];
	ptsArrow[0].x = x-m_sizeArrow.cx/2;
	ptsArrow[0].y = bAscending ? rcArrow.bottom : rcArrow.top;
	ptsArrow[1].x = x+m_sizeArrow.cx/2;
	ptsArrow[1].y = bAscending ? rcArrow.bottom : rcArrow.top;
	ptsArrow[2].x = x;
	ptsArrow[2].y = bAscending ? rcArrow.top : rcArrow.bottom;

	DrawSortArrow(pDC, ptsArrow, bAscending);
}

void CXTPHeaderCtrlTheme::OnDrawItemText(LPDRAWITEMSTRUCT lpDIS, CXTPHeaderCtrl* pHeaderCtrl)
{
	ASSERT_VALID(pHeaderCtrl);

	CDC* pDC = CDC::FromHandle(lpDIS->hDC);
	LPHDITEM lpHDI = (LPHDITEM)lpDIS->itemData;

	// prepare the device context to draw text.
	CXTPFontDC fontDC(pDC, pHeaderCtrl->GetFont());

	// construct text size.
	CRect rcItem(lpDIS->rcItem);
	rcItem.DeflateRect(m_sizePadding.cx, 0);

	BOOL bSortAsc;
	UINT nSortItem = pHeaderCtrl->GetSortedCol(&bSortAsc);
	if (nSortItem == lpDIS->itemID)
		rcItem.right -= m_sizeArrow.cx + m_iArrowPadding*2;

	CRect rcText(rcItem);
	CSize sizeText = pDC->GetTextExtent(lpHDI->pszText);

	rcText.top = (m_sizePadding.cy > 0) ?
		m_sizePadding.cy : (rcItem.Height()-sizeText.cy)/2;
	rcText.bottom = rcText.top + sizeText.cy;

	if ((lpHDI->fmt & HDF_BITMAP && lpHDI->hbm != NULL) || (lpHDI->fmt & HDF_IMAGE))
	{
		if (lpHDI->fmt & HDF_BITMAP_ON_RIGHT)
		{
			CRect rcImage(rcText);
			rcImage.left += sizeText.cx + m_sizePadding.cx;
			OnDrawItemBitmap(lpDIS, rcImage, pHeaderCtrl);
		}
		else
		{
			OnDrawItemBitmap(lpDIS, rcText, pHeaderCtrl);
		}
	}

	// determine justification for text.
	switch (lpHDI->fmt & HDF_JUSTIFYMASK)
	{
	case HDF_LEFT:
		rcText.right = rcText.left + sizeText.cx;
		break;
	case HDF_CENTER:
		rcText.left += (rcItem.Width()-sizeText.cx)/2;
		rcText.right = rcText.left + sizeText.cx;
		break;
	case HDF_RIGHT:
		rcText.left = rcText.right - sizeText.cx;
		break;
	}

	// size text to fit available display area.
	rcText.left = __max(rcText.left, rcItem.left);
	rcText.right = __min(rcText.right, rcItem.right);

	// check if we are sorting.
	if (HasSortArrow() && nSortItem == lpDIS->itemID)
	{
		lpDIS->rcItem = rcText;
		OnDrawItemSortArrow(lpDIS, bSortAsc);
	}

	if (rcText.Width() > 0)
	{
		if (lpDIS->itemState == ODS_SELECTED)
			rcText.OffsetRect(1, 1);

		// draw text.
		pDC->DrawText(lpHDI->pszText, &rcText,
			DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS);
	}
}

void CXTPHeaderCtrlTheme::DrawHeader(CDC* pDC, CXTPHeaderCtrl* pHeaderCtrl)
{
	ASSERT_VALID(pHeaderCtrl);

	CXTPClientRect rcClient(pHeaderCtrl);

	DRAWITEMSTRUCT dis;
	dis.CtlType = ODT_BUTTON;
	dis.CtlID = 0;
	dis.itemID = 0;
	dis.itemAction = ODA_DRAWENTIRE;
	dis.itemState = 0;
	dis.hwndItem = pHeaderCtrl->m_hWnd;
	dis.hDC = pDC->m_hDC;
	dis.rcItem = rcClient;
	dis.itemData = 0;

	// draw background.
	if (!m_bUseWinThemes || !OnDrawBackgroundWinThemed(&dis))
		OnDrawBackground(&dis);

	// set text attributes.
	CXTPFontDC fontDC(pDC, pHeaderCtrl->GetFont());
	pDC->SetBkColor(m_cr3DFace);
	pDC->SetTextColor(m_crText);
	pDC->SetBkMode(TRANSPARENT);

	// Set up the header item order array.
	HD_ITEM hdi;
	::ZeroMemory(&hdi, sizeof(HD_ITEM));

	hdi.fmt = HDF_STRING | HDF_LEFT | HDF_BITMAP;
	hdi.mask = HDI_WIDTH | HDI_TEXT | HDI_FORMAT | HDI_ORDER | HDI_BITMAP | HDI_LPARAM;

	int cxy = 0;
	int iItem = 0;
	int iItemCount = pHeaderCtrl->GetItemCount();
	int *plItems = new int[iItemCount];

	for (iItem = 0; iItem < iItemCount; iItem++)
	{
		pHeaderCtrl->GetItem(iItem, &hdi);
		plItems[hdi.iOrder] = iItem;
	}

	hdi.fmt = HDF_STRING | HDF_LEFT | HDF_BITMAP;
	hdi.mask = HDI_WIDTH | HDI_TEXT | HDI_FORMAT | HDI_BITMAP | HDI_LPARAM;

	// get cursor position.
	CPoint point;
	::GetCursorPos(&point);
	pHeaderCtrl->ScreenToClient(&point);

	// Draw each header item
	for (iItem = 0; iItem < iItemCount; ++iItem)
	{
		// Get the header item text and format
		TCHAR szBuffer[256];
		hdi.pszText = szBuffer;
		hdi.cchTextMax = 255;
		pHeaderCtrl->GetItem(plItems[iItem], &hdi);

		// initialize draw item structure.
		dis.itemID = plItems[iItem];
		dis.itemState = 0;
		dis.rcItem = rcClient;
		dis.rcItem.left = cxy;
		dis.rcItem.right = dis.rcItem.left + hdi.cxy;
		dis.itemData = (DWORD_PTR)&hdi;

		// if hot tracking is enabled, set the state.
		if (GetDrawStyle() & HDR_XTP_HOTTRACKING)
		{
			if (pHeaderCtrl->HitTest(point) == plItems[iItem])
				dis.itemState = ODS_HOTLIGHT;
		}

		// check if the item is selected
		if (pHeaderCtrl->ItemPressed(plItems[iItem]))
			dis.itemState = ODS_SELECTED;

		// draw the item background.
		if (!m_bUseWinThemes || !OnDrawItemBackgroundWinThemed(&dis))
			OnDrawItemBackground(&dis);

		// draw the item text.
		OnDrawItemText(&dis, pHeaderCtrl);
		cxy += hdi.cxy;
	}

	delete [] plItems;
}

BOOL CXTPHeaderCtrlTheme::UseWinXPThemes(CXTPHeaderCtrl* pHeaderCtrl)
{
	ASSERT_VALID(pHeaderCtrl);

	// if windows xp themes are not allowed, return FALSE.
	if ((GetDrawStyle() & HDR_XTP_WINDEF) == 0)
		return FALSE;

	// if we got this far then we try to load the theme data for
	// this control if it is not currently open.
	if (!m_themeHeader->ThemeDataOpen())
		m_themeHeader->OpenThemeData(pHeaderCtrl->m_hWnd, L"HEADER");

	// if our application is not "Theme Ready" meaning that we cannot
	// display Windows XP themes, then return FALSE.
#ifndef _XTP_ACTIVEX
	if (!m_themeHeader->IsAppThemeReady())
		return FALSE;
#endif
	// this will return TRUE if we can display visual styles.
	return m_themeHeader->ThemeDataOpen();
}

void CXTPHeaderCtrlTheme::SetDrawStyle(DWORD dwDrawStyle, CXTPHeaderCtrl* pHeaderCtrl)
{
	m_dwDrawStyle = dwDrawStyle;
	m_bUseWinThemes = UseWinXPThemes(pHeaderCtrl) && ((m_dwDrawStyle & HDR_XTP_WINDEF) != 0);
}

DWORD CXTPHeaderCtrlTheme::GetDrawStyle() const
{
	return m_dwDrawStyle;
}

BOOL CXTPHeaderCtrlTheme::Layout(LPHDLAYOUT lpHDL, CXTPHeaderCtrl* pHeaderCtrl)
{
	ASSERT_VALID(pHeaderCtrl);
	return (BOOL)pHeaderCtrl->DefWindowProc(HDM_LAYOUT,
		(WPARAM)0, (LPARAM)lpHDL);
}

BOOL CXTPHeaderCtrlTheme::SetBitmap(int iCol, UINT uBitmapID, BOOL bRemove, COLORREF crMask, CXTPHeaderCtrl* pHeaderCtrl)
{
	ASSERT_VALID(pHeaderCtrl);

	// reset header image list
	HD_ITEM hdi;
	::ZeroMemory(&hdi, sizeof(HD_ITEM));

	hdi.mask = HDI_BITMAP | HDI_FORMAT;
	pHeaderCtrl->GetItem(iCol, &hdi);

	// free previous GDI resources.
	if (hdi.fmt & HDF_BITMAP && hdi.hbm != NULL)
		::ImageList_Destroy((HIMAGELIST)hdi.hbm);

	hdi.hbm = NULL;
	hdi.fmt &= ~HDF_BITMAP;

	pHeaderCtrl->SetItem(iCol, &hdi);
	pHeaderCtrl->RedrawWindow();

	// add new image list
	if (!bRemove)
	{
		HBITMAP hBitmap = (HBITMAP)::LoadImage(AfxGetResourceHandle(),
			MAKEINTRESOURCE(uBitmapID), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);

		if (hBitmap == NULL)
			return FALSE;

		BITMAP bmpInfo;
		ZeroMemory(&bmpInfo, sizeof(BITMAP));

		if (::GetObject(hBitmap, sizeof(BITMAP), &bmpInfo) == 0)
			return FALSE;

		HIMAGELIST hImageList = ::ImageList_Create(bmpInfo.bmWidth,
			bmpInfo.bmHeight, ILC_MASK | ILC_COLOR24, 0, 1);

		if (hImageList == NULL)
			return FALSE;

		if (::ImageList_AddMasked(hImageList, hBitmap, crMask) == -1)
		{
			::ImageList_Destroy(hImageList);
			::DeleteObject(hBitmap);
			return FALSE;
		}

		// we are actually storing a handle to the image list and
		// not the bitmap so we can draw using the mask.
		hdi.hbm = (HBITMAP)hImageList;
		hdi.fmt = HDF_BITMAP;

		pHeaderCtrl->SetItem(iCol, &hdi);
		pHeaderCtrl->RedrawWindow();

		::DeleteObject(hBitmap);
		return TRUE;
	}

	return FALSE;
}

void CXTPHeaderCtrlTheme::OnDrawItemBitmap(LPDRAWITEMSTRUCT lpDIS, CRect& rcText, CXTPHeaderCtrl* pHeaderCtrl)
{
	ASSERT_VALID(pHeaderCtrl);

	HDITEM hdi;
	hdi.mask = HDI_BITMAP | HDI_FORMAT | HDI_IMAGE;
	pHeaderCtrl->GetItem(lpDIS->itemID, &hdi);

	HIMAGELIST hImageList = 0;
	int nIndex = -1;

	if (hdi.fmt & HDF_BITMAP && hdi.hbm != NULL)
	{
		hImageList = (HIMAGELIST)hdi.hbm;
		nIndex = 0;
	}
	else if (hdi.fmt & HDF_IMAGE)
	{
		hImageList = (HIMAGELIST)pHeaderCtrl->SendMessage(HDM_GETIMAGELIST);
		nIndex = hdi.iImage;
	}

	if (nIndex != -1 && (hdi.fmt & HDF_IMAGE) && pHeaderCtrl->GetImageManager())
	{
		CXTPImageManagerIcon* pIcon = pHeaderCtrl->GetImageManager()->GetImage(nIndex, 0);

		if (pIcon)
		{
			CRect rcItem(lpDIS->rcItem);
			CPoint point(rcText.left,  (rcItem.Height() - pIcon->GetHeight()) / 2);

			rcText.left += pIcon->GetWidth() + m_sizePadding.cx;

			XTPImageState imageState = (lpDIS->itemState & ODS_SELECTED) ? xtpImageChecked :
				(lpDIS->itemState & ODS_HOTLIGHT) ? xtpImageHot : xtpImageNormal;

			if (lpDIS->itemState == ODS_SELECTED)
				point.Offset(1, 1);

			if (point.x + pIcon->GetWidth() > lpDIS->rcItem.right)
				return;

			// draw image transparently.
			pIcon->Draw(CDC::FromHandle(lpDIS->hDC), point, pIcon->GetIcon(imageState));

			return;
		}
	}

	if (hImageList)
	{
		int cx, cy;
		if (::ImageList_GetIconSize(hImageList, &cx, &cy) && (nIndex < ImageList_GetImageCount(hImageList)))
		{
			CRect rcItem(lpDIS->rcItem);
			CPoint point(rcText.left,  (rcItem.Height() - cy) / 2);

			rcText.left += cx + m_sizePadding.cx;

			if (lpDIS->itemState == ODS_SELECTED)
				point.Offset(1, 1);

			if (point.x + cx > lpDIS->rcItem.right)
				return;

			// draw image transparently.
			::ImageList_Draw(hImageList, nIndex, lpDIS->hDC, point.x, point.y, ILD_TRANSPARENT);
		}
	}
}

void CXTPHeaderCtrlTheme::CleanUp(CXTPHeaderCtrl* pHeaderCtrl)
{
	ASSERT_VALID(pHeaderCtrl);

	// free GDI resources
	for (int i = 0; i < pHeaderCtrl->GetItemCount(); ++i)
	{
		HDITEM hdi;
		hdi.mask = HDI_FORMAT | HDI_BITMAP;
		pHeaderCtrl->GetItem(i, &hdi);

		if (hdi.fmt & HDF_BITMAP && hdi.hbm != NULL)
		{
			::ImageList_Destroy((HIMAGELIST)hdi.hbm);
		}
	}
}

//=============================================================================
// CXTPHeaderCtrlThemeOfficeXP
//=============================================================================

CXTPHeaderCtrlThemeOfficeXP::CXTPHeaderCtrlThemeOfficeXP()
{
	m_nTheme = xtpControlThemeOfficeXP;
	m_dwDrawStyle = HDR_XTP_SORTARROW;
}

CXTPHeaderCtrlThemeOfficeXP::~CXTPHeaderCtrlThemeOfficeXP()
{

}

void CXTPHeaderCtrlThemeOfficeXP::OnDrawBackground(LPDRAWITEMSTRUCT lpDIS)
{
	CRect rcItem(lpDIS->rcItem);
	CDC* pDC = CDC::FromHandle(lpDIS->hDC);

	// fill background
	pDC->FillSolidRect(&rcItem, m_cr3DFace);
	rcItem.right += 2;

	// draw 3D borders.
	pDC->Draw3dRect(&rcItem, m_cr3DHighLight, m_cr3DShadow);
}

void CXTPHeaderCtrlThemeOfficeXP::OnDrawItemBackground(LPDRAWITEMSTRUCT lpDIS)
{
	CDC* pDC = CDC::FromHandle(lpDIS->hDC);

	if (lpDIS->itemState == ODS_SELECTED)
	{
		CXTPPenDC penDC(lpDIS->hDC, m_cr3DShadow);
		pDC->MoveTo(lpDIS->rcItem.right-2, 2);
		pDC->LineTo(lpDIS->rcItem.right-2, lpDIS->rcItem.bottom-2);

		penDC.Color(m_cr3DHighLight);
		pDC->MoveTo(lpDIS->rcItem.right-1, 2);
		pDC->LineTo(lpDIS->rcItem.right-1, lpDIS->rcItem.bottom-2);
	}
	else
	{
		CXTPPenDC penDC(lpDIS->hDC, m_cr3DShadow);
		pDC->MoveTo(lpDIS->rcItem.right-2, 2);
		pDC->LineTo(lpDIS->rcItem.right-2, lpDIS->rcItem.bottom-2);

		penDC.Color(m_cr3DHighLight);
		pDC->MoveTo(lpDIS->rcItem.right-1, 2);
		pDC->LineTo(lpDIS->rcItem.right-1, lpDIS->rcItem.bottom-2);
	}
}

//=============================================================================
// CXTPHeaderCtrlThemeOffice2003
//=============================================================================

CXTPHeaderCtrlThemeOffice2003::CXTPHeaderCtrlThemeOffice2003()
{
	m_nTheme = xtpControlThemeOffice2003;
	m_dwDrawStyle = HDR_XTP_SORTARROW;
	m_sizePadding = CSize(6, 0);
}

CXTPHeaderCtrlThemeOffice2003::~CXTPHeaderCtrlThemeOffice2003()
{

}

void CXTPHeaderCtrlThemeOffice2003::OnDrawBackground(LPDRAWITEMSTRUCT lpDIS)
{
	CRect rcItem(lpDIS->rcItem);
	CDC* pDC = CDC::FromHandle(lpDIS->hDC);

	// fill background
	pDC->FillSolidRect(&rcItem, m_cr3DFace);
	rcItem.right += 2;

	// draw 3D borders.
	pDC->Draw3dRect(&rcItem, m_cr3DHighLight, MixColor(m_cr3DFace, m_cr3DShadow, 0.6));

	CXTPPenDC penDC(lpDIS->hDC, MixColor(m_cr3DFace, m_cr3DShadow, 0.4));
	pDC->MoveTo(rcItem.left, rcItem.bottom-2);
	pDC->LineTo(rcItem.right, rcItem.bottom-2);

	penDC.Color(MixColor(m_cr3DFace, m_cr3DShadow, 0.25));
	pDC->MoveTo(rcItem.left, rcItem.bottom-3);
	pDC->LineTo(rcItem.right, rcItem.bottom-3);
}

void CXTPHeaderCtrlThemeOffice2003::OnDrawItemBackground(LPDRAWITEMSTRUCT lpDIS)
{
	CDC* pDC = CDC::FromHandle(lpDIS->hDC);

	if (lpDIS->itemState == ODS_SELECTED)
	{
		CXTPPenDC penDC(lpDIS->hDC, m_cr3DShadow);
		pDC->MoveTo(lpDIS->rcItem.right-2, 2);
		pDC->LineTo(lpDIS->rcItem.right-2, lpDIS->rcItem.bottom-4);

		penDC.Color(m_cr3DHighLight);
		pDC->MoveTo(lpDIS->rcItem.right-1, 2);
		pDC->LineTo(lpDIS->rcItem.right-1, lpDIS->rcItem.bottom-4);
	}
	else
	{
		CXTPPenDC penDC(lpDIS->hDC, m_cr3DShadow);
		pDC->MoveTo(lpDIS->rcItem.right-2, 2);
		pDC->LineTo(lpDIS->rcItem.right-2, lpDIS->rcItem.bottom-4);

		penDC.Color(m_cr3DHighLight);
		pDC->MoveTo(lpDIS->rcItem.right-1, 2);
		pDC->LineTo(lpDIS->rcItem.right-1, lpDIS->rcItem.bottom-4);
	}
}

BOOL CXTPHeaderCtrlThemeOffice2003::Layout(LPHDLAYOUT lpHDL, CXTPHeaderCtrl* pHeaderCtrl)
{
	ASSERT_VALID(pHeaderCtrl);

	LPRECT      pRC = lpHDL->prc;
	LPWINDOWPOS pWP = lpHDL->pwpos;

	CWindowDC dc(NULL);
	CXTPFontDC fontDC(&dc, pHeaderCtrl->GetFont());

	TEXTMETRIC tm;
	dc.GetTextMetrics(&tm);
	int iHeight = tm.tmHeight + 10;

	pWP->hwndInsertAfter = 0;
	pWP->x = pRC->left;
	pWP->cx = pRC->right - pRC->left;
	pWP->cy = iHeight;
	pWP->y = pRC->top;
	pRC->top += iHeight;
	pWP->flags = SWP_NOACTIVATE | SWP_NOZORDER;

	return TRUE;
}

//=============================================================================
// CXTPHeaderCtrlThemeExplorer
//=============================================================================

CXTPHeaderCtrlThemeExplorer::CXTPHeaderCtrlThemeExplorer()
{
	m_nTheme = xtpControlThemeNativeWinXP;
	m_dwDrawStyle = HDR_XTP_WINDEF | HDR_XTP_HOTTRACKING | HDR_XTP_SORTARROW;
	m_sizePadding = CSize(6, 0);
	m_sizeArrow = CSize(8, 4);
}

CXTPHeaderCtrlThemeExplorer::~CXTPHeaderCtrlThemeExplorer()
{

}

BOOL CXTPHeaderCtrlThemeExplorer::OnDrawItemBackgroundWinThemed(LPDRAWITEMSTRUCT lpDIS)
{
	return CXTPHeaderCtrlTheme::OnDrawItemBackgroundWinThemed(lpDIS);
}

BOOL CXTPHeaderCtrlThemeExplorer::Layout(LPHDLAYOUT lpHDL, CXTPHeaderCtrl* pHeaderCtrl)
{
	ASSERT_VALID(pHeaderCtrl);

	LPRECT      pRC = lpHDL->prc;
	LPWINDOWPOS pWP = lpHDL->pwpos;

	CWindowDC dc(NULL);
	CXTPFontDC fontDC(&dc, pHeaderCtrl->GetFont());

	TEXTMETRIC tm;
	dc.GetTextMetrics(&tm);
	int iHeight = tm.tmHeight + 6;

	pWP->hwndInsertAfter = 0;
	pWP->x = pRC->left;
	pWP->cx = pRC->right - pRC->left;
	pWP->cy = iHeight;
	pWP->y = pRC->top;
	pRC->top += iHeight;
	pWP->flags = SWP_NOACTIVATE | SWP_NOZORDER;

	return TRUE;
}

void CXTPHeaderCtrlThemeExplorer::DrawSortArrow(CDC* pDC, LPPOINT ptsArrow, BOOL /*bAscending*/)
{
	CXTPPenDC   dcPen(pDC->m_hDC, RGB(172, 168, 153));
	CXTPBrushDC dcBrush(pDC->m_hDC, RGB(172, 168, 153));

	pDC->SetPolyFillMode(WINDING);
	pDC->Polygon(ptsArrow, 3);
}

void CXTPHeaderCtrlThemeExplorer::OnDrawBackground(LPDRAWITEMSTRUCT lpDIS)
{
	CRect rcItem(lpDIS->rcItem);
	CDC* pDC = CDC::FromHandle(lpDIS->hDC);

	// fill background
	pDC->FillSolidRect(&rcItem, m_cr3DFace);
	rcItem.right += 2;

	// draw 3D borders.
	pDC->Draw3dRect(&rcItem, m_cr3DHighLight, m_cr3DShadow);
}

//=============================================================================
// CXTPHeaderCtrlThemeResource
//=============================================================================

CXTPHeaderCtrlThemeResource::CXTPHeaderCtrlThemeResource()
{
	m_nTheme = xtpControlThemeResource;
	m_sizeArrow = CSize(8, 4);
}

void CXTPHeaderCtrlThemeResource::RefreshMetrics(CXTPHeaderCtrl* pHeader)
{
	CXTPHeaderCtrlThemeOfficeXP::RefreshMetrics(pHeader);

	CXTPResourceImages* pImages = XTPResourceImages();
	if (pImages)
	{
		m_crText = pImages->GetImageColor(_T("ReportControl"), _T("HeaderText"), m_crText);
		m_cr3DShadow = pImages->GetImageColor(_T("ReportControl"), _T("ColumnSeparator"), m_crText);
		m_cr3DDkShadow = pImages->GetImageColor(_T("ReportControl"), _T("ColumnShadow"), m_crText);

		m_crgBack.SetStandardValue(
			pImages->GetImageColor(_T("ReportControl"), _T("NormalColumnLight"), m_cr3DFace),
			pImages->GetImageColor(_T("ReportControl"), _T("NormalColumnDark"),  m_cr3DFace));

		m_crgBackPressed.SetStandardValue(
			pImages->GetImageColor(_T("ReportControl"), _T("PressedColumnLight"), m_cr3DFace),
			pImages->GetImageColor(_T("ReportControl"), _T("PressedColumnDark"),  m_cr3DFace));
	}
	else
	{
		m_crgBack.SetStandardValue(m_cr3DFace, m_cr3DFace);
	}
}

void CXTPHeaderCtrlThemeResource::OnDrawBackground(LPDRAWITEMSTRUCT lpDIS)
{
	CXTPResourceImages* pImages = XTPResourceImages();
	if (pImages)
	{
		CDC* pDC = CDC::FromHandle(lpDIS->hDC);
		CRect rcItem = lpDIS->rcItem;

		// fill background
		XTPDrawHelpers()->GradientFill(pDC, rcItem, m_crgBack, FALSE);
		rcItem.right += 2;

		// draw border.
		CXTPPenDC penDC(pDC->m_hDC, m_cr3DDkShadow);
		pDC->MoveTo(lpDIS->rcItem.left, lpDIS->rcItem.bottom-1);
		pDC->LineTo(lpDIS->rcItem.right, lpDIS->rcItem.bottom-1);
	}
	else
	{
		CXTPHeaderCtrlThemeOfficeXP::OnDrawBackground(lpDIS);
	}
}

void CXTPHeaderCtrlThemeResource::OnDrawItemBackground(LPDRAWITEMSTRUCT lpDIS)
{
	CXTPResourceImages* pImages = XTPResourceImages();
	if (pImages)
	{
		CDC* pDC = CDC::FromHandle(lpDIS->hDC);
		CRect rcItem = lpDIS->rcItem;
		rcItem.left++;

		if (lpDIS->itemState == ODS_SELECTED)
		{
			XTPDrawHelpers()->GradientFill(pDC, &rcItem, m_crgBackPressed, FALSE);

			// draw border.
			{
				CXTPPenDC dcPen(pDC->m_hDC, m_cr3DDkShadow);
				pDC->MoveTo(rcItem.left, rcItem.bottom-1);
				pDC->LineTo(rcItem.right, rcItem.bottom-1);
			}

			// draw separator.
			{
				CXTPPenDC dcPen(pDC->m_hDC, m_cr3DShadow);
				pDC->MoveTo(rcItem.right, 2);
				pDC->LineTo(rcItem.right, rcItem.bottom-3);
			}
		}
		else
		{
			// draw separator.
			CXTPPenDC dcPen(pDC->m_hDC, m_cr3DShadow);
			pDC->MoveTo(rcItem.right, 2);
			pDC->LineTo(rcItem.right, rcItem.bottom-3);
		}
	}
	else
	{
		CXTPHeaderCtrlThemeOfficeXP::OnDrawItemBackground(lpDIS);
	}
}

void CXTPHeaderCtrlThemeResource::DrawSortArrow(CDC* pDC, LPPOINT ptsArrow, BOOL bAscending)
{
	CXTPResourceImages* pImages = XTPResourceImages();
	if (pImages)
	{
		CXTPPenDC   dcPen(pDC->m_hDC, m_cr3DShadow);
		CXTPBrushDC dcBrush(pDC->m_hDC, m_cr3DShadow);

		pDC->SetPolyFillMode(WINDING);
		pDC->Polygon(ptsArrow, 3);
	}
	else
	{
		CXTPHeaderCtrlThemeOfficeXP::DrawSortArrow(pDC, ptsArrow, bAscending);
	}
}

void CXTPHeaderCtrlThemeResource::OnDrawItemText(LPDRAWITEMSTRUCT lpDIS, CXTPHeaderCtrl* pHeaderCtrl)
{
	ASSERT_VALID(pHeaderCtrl);

	CDC* pDC = CDC::FromHandle(lpDIS->hDC);
	LPHDITEM lpHDI = (LPHDITEM)lpDIS->itemData;

	// prepare the device context to draw text.
	CXTPFontDC fontDC(pDC, pHeaderCtrl->GetFont());

	// construct text size.
	CRect rcItem(lpDIS->rcItem);
	rcItem.DeflateRect(m_sizePadding.cx, 0);

	BOOL bSortAsc;
	UINT nSortItem = pHeaderCtrl->GetSortedCol(&bSortAsc);
	if (nSortItem == lpDIS->itemID)
		rcItem.right -= m_sizeArrow.cx + m_iArrowPadding*2;

	CRect rcText(rcItem);
	CSize sizeText = pDC->GetTextExtent(lpHDI->pszText);

	rcText.top = (m_sizePadding.cy > 0) ?
		m_sizePadding.cy : (rcItem.Height()-sizeText.cy)/2;
	rcText.bottom = rcText.top + sizeText.cy;

	if ((lpHDI->fmt & HDF_BITMAP && lpHDI->hbm != NULL) || (lpHDI->fmt & HDF_IMAGE))
	{
		if (lpHDI->fmt & HDF_BITMAP_ON_RIGHT)
		{
			CRect rcImage(rcText);
			rcImage.left += sizeText.cx + m_sizePadding.cx;
			OnDrawItemBitmap(lpDIS, rcImage, pHeaderCtrl);
		}
		else
		{
			OnDrawItemBitmap(lpDIS, rcText, pHeaderCtrl);
		}
	}

	// determine justification for text.
	switch (lpHDI->fmt & HDF_JUSTIFYMASK)
	{
	case HDF_LEFT:
		rcText.right = rcText.left + sizeText.cx;
		break;
	case HDF_CENTER:
		rcText.left += (rcItem.Width()-sizeText.cx)/2;
		rcText.right = rcText.left + sizeText.cx;
		break;
	case HDF_RIGHT:
		rcText.left = rcText.right - sizeText.cx;
		break;
	}

	// size text to fit available display area.
	rcText.left = __max(rcText.left, rcItem.left);
	rcText.right = __min(rcText.right, rcItem.right);

	// check if we are sorting.
	if (HasSortArrow() && nSortItem == lpDIS->itemID)
	{
		lpDIS->rcItem = rcText;
		OnDrawItemSortArrow(lpDIS, bSortAsc);
	}

	if (rcText.Width() > 0)
	{
		// draw text.
		pDC->DrawText(lpHDI->pszText, &rcText,
			DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS);
	}
}

void CXTPHeaderCtrlThemeResource::OnDrawItemSortArrow(LPDRAWITEMSTRUCT lpDIS, BOOL bAscending)
{
	CDC* pDC = CDC::FromHandle(lpDIS->hDC);
	CRect rcItem = lpDIS->rcItem;

	CPoint point;
	point.x = rcItem.right + m_iArrowPadding;
	point.y = (rcItem.top - 1) + (rcItem.Height() - m_sizeArrow.cy) / 2;

	if (point.x < rcItem.left)
		return;

	CRect rcArrow;
	rcArrow.left = point.x;
	rcArrow.top = point.y;
	rcArrow.right = point.x + m_sizeArrow.cx;
	rcArrow.bottom = point.y + m_sizeArrow.cy;

	int x = point.x + (m_sizeArrow.cx/2);

	POINT ptsArrow[3];
	ptsArrow[0].x = x-m_sizeArrow.cx/2;
	ptsArrow[0].y = bAscending ? rcArrow.bottom : rcArrow.top;
	ptsArrow[1].x = x+m_sizeArrow.cx/2;
	ptsArrow[1].y = bAscending ? rcArrow.bottom : rcArrow.top;
	ptsArrow[2].x = x;
	ptsArrow[2].y = bAscending ? rcArrow.top : rcArrow.bottom;

	DrawSortArrow(pDC, ptsArrow, bAscending);
}
