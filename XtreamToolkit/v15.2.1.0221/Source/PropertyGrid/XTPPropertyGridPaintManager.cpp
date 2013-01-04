// XTPPropertyGridPaintManager.cpp : implementation of the CXTPPropertyGridPaintManager class.
//
// This file is a part of the XTREME PROPERTYGRID MFC class library.
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

#include "Common/XTPDrawHelpers.h"
#include "Common/XTPImageManager.h"
#include "Common/XTPResourceImage.h"
#include "Common/XTPMarkupRender.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPWinThemeWrapper.h"
#include "Common/XTPSystemHelpers.h"

#include "XTPPropertyGridDefines.h"
#include "XTPPropertyGridPaintManager.h"
#include "XTPPropertyGridInplaceButton.h"
#include "XTPPropertyGridItem.h"
#include "XTPPropertyGrid.h"
#include "XTPPropertyGridView.h"

#include "XTPPropertyGridItemBool.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif


CXTPPropertyGridItemMetrics::CXTPPropertyGridItemMetrics(CXTPPropertyGridPaintManager* pPaintManager)
	: m_pPaintManager(pPaintManager)
{
	m_nImage = -1;
	m_uDrawTextFormat = DT_VCENTER | DT_SINGLELINE;
	m_nMaxLength = 0;

}
void CXTPPropertyGridItemMetrics::SetDefaultValues()
{
	m_clrHelpBack.SetDefaultValue();
	m_clrHelpFore.SetDefaultValue();
	m_clrLine.SetDefaultValue();
	m_clrBack.SetDefaultValue();
	m_clrFore.SetDefaultValue();
	m_clrReadOnlyFore.SetDefaultValue();
	m_clrCategoryFore.SetDefaultValue();
	m_clrVerbFace.SetDefaultValue();
}

COLORREF CXTPPropertyGridPaintManager::GetItemTextColor(CXTPPropertyGridItem* pItem, BOOL bValuePart)
{
	ASSERT(pItem);
	if (!pItem)
		return m_pMetrics->m_clrFore;

	CXTPPropertyGridItemMetrics* pMetrics = pItem->GetMetrics(bValuePart, FALSE);
	if (pMetrics && !pMetrics->m_clrFore.IsDefaultValue())
		return pMetrics->m_clrFore;

	return pItem->IsCategory() ? m_pMetrics->m_clrCategoryFore :
		pItem->GetReadOnly() ? m_pMetrics->m_clrReadOnlyFore : m_pMetrics->m_clrFore;
}

COLORREF CXTPPropertyGridPaintManager::GetItemBackColor(CXTPPropertyGridItem* pItem, BOOL bValuePart)
{
	ASSERT(pItem);
	if (!pItem)
		return m_pMetrics->m_clrBack;

	CXTPPropertyGridItemMetrics* pMetrics = pItem->GetMetrics(bValuePart, FALSE);
	if (pMetrics && !pMetrics->m_clrBack.IsDefaultValue())
		return pMetrics->m_clrBack;

	return m_pMetrics->m_clrBack;
}

CFont* CXTPPropertyGridPaintManager::GetItemFont(CXTPPropertyGridItem* pItem, BOOL bValuePart)
{
	ASSERT(pItem);
	if (!pItem)
		return &m_pMetrics->m_fontNormal;

	CXTPPropertyGridItemMetrics* pMetrics = pItem->GetMetrics(bValuePart, FALSE);
	if (pMetrics && pMetrics->m_fontNormal.GetSafeHandle())
		return &pMetrics->m_fontNormal;

	if (pItem->IsCategory())
		return &m_pMetrics->m_fontBold;

	if (bValuePart && m_pGrid->IsHighlightChangedItems() && pItem->IsValueChanged())
		return &m_pMetrics->m_fontBold;

	return &m_pMetrics->m_fontNormal;
}



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXTPPropertyGridPaintManager::CXTPPropertyGridPaintManager(CXTPPropertyGrid* pGrid)
	: m_pGrid(pGrid)
{
	m_buttonsStyle = xtpGridButtonsDefault;

	m_themeTree = new CXTPWinThemeWrapper();
	m_themeButton = new CXTPWinThemeWrapper();
	m_themeCombo = new CXTPWinThemeWrapper();

	m_pMetrics = new CXTPPropertyGridItemMetrics(this);

	m_bTransparent = FALSE;

}

CXTPPropertyGridPaintManager::~CXTPPropertyGridPaintManager()
{
	CMDTARGET_RELEASE(m_pMetrics);

	SAFE_DELETE(m_themeTree);
	SAFE_DELETE(m_themeButton);
	SAFE_DELETE(m_themeCombo);
}

void CXTPPropertyGridPaintManager::RefreshMetrics()
{
	RefreshXtremeColors();

	m_clrHighlight.SetStandardValue(GetXtremeColor(COLOR_HIGHLIGHT));
	m_clrHighlightText.SetStandardValue(GetXtremeColor(COLOR_HIGHLIGHTTEXT));

	m_clrFace.SetStandardValue(GetXtremeColor(COLOR_3DFACE));
	m_clrShadow.SetStandardValue(GetXtremeColor(COLOR_3DSHADOW));



	m_pMetrics->m_clrHelpBack.SetStandardValue(m_clrFace);
	m_pMetrics->m_clrHelpFore.SetStandardValue(GetXtremeColor(COLOR_BTNTEXT));
	m_pMetrics->m_clrLine.SetStandardValue(GetSysColor(COLOR_SCROLLBAR));
	m_pMetrics->m_clrCategoryFore.SetStandardValue(GetXtremeColor(COLOR_GRAYTEXT));
	m_pMetrics->m_clrBack.SetStandardValue(GetXtremeColor(COLOR_WINDOW));
	m_pMetrics->m_clrFore.SetStandardValue(GetXtremeColor(COLOR_WINDOWTEXT));
	m_pMetrics->m_clrReadOnlyFore.SetStandardValue(GetXtremeColor(COLOR_GRAYTEXT));

	HWND hWnd = AfxGetMainWnd() ? AfxGetMainWnd()->GetSafeHwnd() : 0;
	m_themeTree->OpenThemeData(hWnd, L"TREEVIEW");
	m_themeButton->OpenThemeData(hWnd, L"BUTTON");
	m_themeCombo->OpenThemeData(hWnd, L"COMBOBOX");

	m_pMetrics->m_clrVerbFace.SetStandardValue(
		XTPSystemVersion()->IsWinVistaOrGreater() && m_themeButton->IsAppThemed()  ? RGB(0, 102, 204) : GetXtremeColor(COLOR_ACTIVECAPTION));

}

void CXTPPropertyGridPaintManager::FillPropertyGrid(CDC* pDC)
{
	CXTPClientRect rc(m_pGrid);

	HBRUSH hBrush = NULL;

	if (m_bTransparent || m_clrFace == GetXtremeColor(COLOR_3DFACE))
	{
		hBrush = (HBRUSH)m_pGrid->GetParent()->SendMessage(WM_CTLCOLORSTATIC, (WPARAM)pDC->GetSafeHdc(), (LPARAM)m_pGrid->m_hWnd);
	}
	if (hBrush)
	{
		::FillRect(pDC->GetSafeHdc(), rc, hBrush);
	}
	else
	{
		pDC->FillSolidRect(rc, m_clrFace);
	}

	pDC->SetBkMode(TRANSPARENT);

	if (m_pGrid->IsHelpVisible())
	{
		CXTPPropertyGridItem* pItem = m_pGrid->GetSelectedItem();

		CRect rcHelp(rc);
		rcHelp.top = rc.bottom - m_pGrid->m_nHelpHeight;

		if (!hBrush || !m_bTransparent) pDC->FillSolidRect(rcHelp, m_pMetrics->m_clrHelpBack);
		DrawPropertyGridBorder(pDC, rcHelp, FALSE);


		CXTPFontDC fontCaption(pDC, &m_pMetrics->m_fontBold);

		if (pItem)
		{
			pDC->SetTextColor(m_pMetrics->m_clrHelpFore);

			pDC->SetBkMode(TRANSPARENT);
			CRect rcCaption(rcHelp);
			rcCaption.DeflateRect(6, 3);

			if (pItem->GetMarkupCaption())
			{
				XTPMarkupSetDefaultFont(XTPMarkupElementContext(pItem->GetMarkupCaption()),
					(HFONT)m_pMetrics->m_fontBold, pDC->GetTextColor());

				CSize sz = XTPMarkupMeasureElement(pItem->GetMarkupCaption(), INT_MAX, INT_MAX);
				XTPMarkupRenderElement(pItem->GetMarkupCaption(), pDC->GetSafeHdc(),
					CRect(rcCaption.TopLeft(), sz));
			}
			else
			{
				pDC->DrawText(pItem->GetCaption(), rcCaption, DT_SINGLELINE | DT_NOPREFIX);
			}

			CString strDesc = pItem->GetDescription();
			if (!strDesc.IsEmpty())
			{
				CXTPFontDC fontDescription(pDC, &m_pMetrics->m_fontNormal);
				CXTPEmptyRect rcCaptionHeight;
				pDC->DrawText(pItem->GetCaption(), rcCaptionHeight, DT_SINGLELINE | DT_NOPREFIX | DT_CALCRECT);
				CRect rcDesc(rcHelp);
				rcDesc.DeflateRect(6, 3 + rcCaptionHeight.Height() + 3, 3, 3);

				if (pItem->GetMarkupDescription())
				{
					XTPMarkupSetDefaultFont(XTPMarkupElementContext(pItem->GetMarkupDescription()),
						(HFONT)m_pMetrics->m_fontNormal, pDC->GetTextColor());

					CSize sz = XTPMarkupMeasureElement(pItem->GetMarkupDescription(), rcDesc.Width(), rcDesc.Height());
					XTPMarkupRenderElement(pItem->GetMarkupDescription(), pDC->GetSafeHdc(), rcDesc);
				}
				else
				{
					pDC->DrawText(strDesc, rcDesc, DT_WORDBREAK | DT_NOPREFIX);
				}
			}
		}

		rc.bottom -= m_pGrid->m_nHelpHeight + 3;
	}

	if (m_pGrid->IsVerbsVisible())
	{
		CRect rcVerbs(rc);
		rcVerbs.top = rc.bottom - m_pGrid->m_nVerbsHeight;

		if (!hBrush || !m_bTransparent) pDC->FillSolidRect(rcVerbs, m_pMetrics->m_clrHelpBack);
		DrawPropertyGridBorder(pDC, rcVerbs, FALSE);

		HitTestVerbs(pDC, rcVerbs, CPoint(0));
	}
}

int CXTPPropertyGridPaintManager::HitTestVerbs(CDC* pDC, CRect rcVerbs, CPoint pt)
{
	#define DRAWTEXTPART(strCaption)\
		CRect rcText(CRect(x, y, min(x + nTextWidth, rcVerbs.right), min(y + nTextHeight, rcVerbs.bottom)));\
		if (bDraw)\
		{\
			pVerb->SetPart(rcText);\
			pDC->SetTextColor(m_pMetrics->m_clrVerbFace);\
			pDC->DrawText(strCaption, rcText, DT_SINGLELINE | DT_NOPREFIX);\
			if (pVerb->IsFocused())\
			{\
				pDC->SetTextColor(m_pMetrics->m_clrHelpBack);\
				pDC->SetBkColor(0);\
				rcText.bottom++;\
				pDC->DrawFocusRect(rcText);\
			}\
		}\
		else if (rcText.PtInRect(pt)) return i;

	BOOL bDraw = (pt == CPoint(0));

	CXTPFontDC font(pDC, &m_pMetrics->m_fontUnderline);

	rcVerbs.DeflateRect(6, 4);

	int nCount = m_pGrid->GetVerbs()->GetCount();

	int x = rcVerbs.left, y = rcVerbs.top, nWidth = rcVerbs.Width();
	int nTextHeight = pDC->GetTextExtent(_T("XXX"), 3).cy;

	for (int i = 0; i < nCount; i++)
	{
		CXTPPropertyGridVerb* pVerb = m_pGrid->GetVerbs()->GetAt(i);
		CString strCaption = pVerb->GetCaption();

		for (;;)
		{
			if (strCaption.IsEmpty())
				break;

			int nTextWidth = pDC->GetTextExtent(strCaption).cx;

			if (x + nTextWidth < nWidth)
			{
				DRAWTEXTPART(strCaption);
				x += nTextWidth;
				break;
			}
			else
			{
				int nStartIndex = (strCaption.GetLength() - 1) * (nWidth - x) / nTextWidth;
				int nIndex = nStartIndex;

				if ((nStartIndex < 1) && (x == rcVerbs.left))
				{
					i = nCount - 1;
					break;
				}

				for (; nIndex >= 0; nIndex--)
				{
					if (strCaption[nIndex] == _T(' ') || strCaption[nIndex] == _T(',') || strCaption[nIndex] == _T('-'))
						break;
				}

				if (nIndex > 0 || (x == rcVerbs.left))
				{
					if (nIndex <= 0) nIndex = nStartIndex;

					CString strLeft = strCaption.Left(nIndex + 1);
					CString strRight = strCaption.Mid(nIndex + 1);

					nTextWidth = pDC->GetTextExtent(strLeft).cx;
					DRAWTEXTPART(strLeft);

					x = rcVerbs.left;
					y += nTextHeight;
					strCaption = strRight;
				}
				else
				{
					y += nTextHeight;
					x = rcVerbs.left;
				}
			}
			if (y >= rcVerbs.bottom)
				break;
		}

		if ((y >= rcVerbs.bottom) || (i == nCount - 1))
			break;

		if (bDraw)
		{
			font.SetFont(&m_pMetrics->m_fontNormal);
			pDC->SetTextColor(m_pMetrics->m_clrHelpFore);
			pDC->DrawText(_T(","), 1, CRect(x, y, x + 8, min(y + nTextHeight, rcVerbs.bottom)), DT_SINGLELINE | DT_NOPREFIX);
			font.SetFont(&m_pMetrics->m_fontUnderline);
		}
		x += 8;
	}
	return -1;
}

void CXTPPropertyGridPaintManager::DrawPropertyGridBorder(CDC* pDC, RECT& rc, BOOL bAdjustRect)
{
	XTPPropertyGridBorderStyle borderStyle = m_pGrid->GetBorderStyle();

	if (pDC)
	{
		switch (borderStyle)
		{
		case xtpGridBorderStaticEdge:
			pDC->Draw3dRect(rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top,
				m_clrShadow, m_clrShadow);
			break;

		case xtpGridBorderFlat:
			pDC->Draw3dRect(rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top,
				GetXtremeColor(COLOR_WINDOWFRAME), GetXtremeColor(COLOR_WINDOWFRAME));
			break;

		case xtpGridBorderClientEdge:
			pDC->Draw3dRect(rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top,
				GetXtremeColor(COLOR_3DSHADOW), GetXtremeColor(COLOR_3DHIGHLIGHT));
			pDC->Draw3dRect(rc.left + 1, rc.top + 1, rc.right - rc.left - 2, rc.bottom - rc.top - 2,
				GetXtremeColor(COLOR_3DDKSHADOW), GetXtremeColor(COLOR_3DFACE));
			break;
		}
	}

	if (bAdjustRect)
	{
		int nSize = borderStyle == xtpGridBorderClientEdge ? 2 :
			borderStyle == xtpGridBorderStaticEdge || borderStyle == xtpGridBorderFlat ? 1 : 0;

		rc.left   += nSize;
		rc.top    += nSize;
		rc.right  -= nSize;
		rc.bottom -= nSize;
	}
}

void CXTPPropertyGridPaintManager::FillPropertyGridView(CDC* pDC)
{
	CXTPPropertyGridView* pView = &m_pGrid->GetGridView();

	CXTPClientRect rc(pView);
	pDC->FillSolidRect(rc, m_pMetrics->m_clrBack);

}

void CXTPPropertyGridPaintManager::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	CXTPPropertyGridView* pView = &m_pGrid->GetGridView();
	if (pView->GetStyle() & LBS_OWNERDRAWVARIABLE)
	{
		CXTPPropertyGridItem* pItem = (CXTPPropertyGridItem*)lpMeasureItemStruct->itemData;
		pItem->MeasureItem(lpMeasureItemStruct);
	}
}

void CXTPPropertyGridPaintManager::DrawCategoryCaptionBackground(CDC* pDC, CXTPPropertyGridItem* /*pItem*/, CRect rc)
{
	pDC->FillSolidRect(rc, m_pMetrics->m_clrLine);
}

void CXTPPropertyGridPaintManager::DrawInplaceListItem(CDC* pDC, CXTPPropertyGridItemConstraint* pConstraint, CRect rc, BOOL bSelected)
{
	pDC->SetBkMode(TRANSPARENT);

	if (bSelected)
	{
		pDC->SetTextColor(m_clrHighlightText);
		pDC->FillSolidRect(rc, m_clrHighlight);
	}
	else
	{
		pDC->SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));
		pDC->FillSolidRect(rc, GetXtremeColor(COLOR_WINDOW));
	}

	if (!pConstraint->m_bEnabled)
		pDC->SetTextColor(GetSysColor(COLOR_GRAYTEXT));

	CRect rcText(rc.left + 2, rc.top, rc.right - 2, rc.bottom);

	CXTPImageManagerIcon* pImage = pConstraint->GetImage();

	if (pImage)
	{
		int nHeight = min(pImage->GetHeight(), rc.Height());
		int nWidth = pImage->GetWidth();

		CPoint pt(rcText.left, rcText.CenterPoint().y - nHeight/2);
		pImage->Draw(pDC, pt, CSize(nWidth, nHeight));

		rcText.left += nWidth + 4;
	}

	// Draw the text.
	pDC->DrawText(pConstraint->m_strConstraint, rcText, DT_LEFT | DT_SINGLELINE | DT_VCENTER | DT_NOPREFIX);
}


void CXTPPropertyGridPaintManager::AdjustItemValueRect(CXTPPropertyGridItem* pItem, CRect& rcValue)
{
	CXTPPropertyGridItemMetrics* pMetrics = pItem->GetMetrics(TRUE, FALSE);
	if (pMetrics && pMetrics->m_nImage != -1)
	{
		CXTPImageManagerIcon* pImage = m_pGrid->GetImageManager()->GetImage(pMetrics->m_nImage, 0);

		if (pImage)
		{
			int nWidth = pImage->GetWidth();
			rcValue.left += nWidth + 4;
		}
	}
}

void CXTPPropertyGridPaintManager::AdjustItemCaptionRect(CXTPPropertyGridItem* pItem, CRect& rcCaption)
{
	CXTPPropertyGridItemMetrics* pMetrics = pItem->GetMetrics(FALSE, FALSE);
	if (pMetrics && pMetrics->m_nImage != -1)
	{
		CXTPImageManagerIcon* pImage = m_pGrid->GetImageManager()->GetImage(pMetrics->m_nImage, 0);

		if (pImage)
		{
			int nWidth = pImage->GetWidth();
			rcCaption.left += nWidth + 4;
		}
	}
}

void CXTPPropertyGridPaintManager::DrawItemValue(CDC* pDC, CXTPPropertyGridItem* pItem, CRect rcValue)
{
	pDC->SetTextColor(GetItemTextColor(pItem, TRUE));
	pDC->FillSolidRect(rcValue, GetItemBackColor(pItem, TRUE));

	rcValue.left += 4;

	for (int i = 0; i < pItem->GetInplaceButtons()->GetCount(); i++)
	{

		CXTPPropertyGridInplaceButton* pButton = pItem->GetInplaceButtons()->GetAt(i);
		if (pButton && pItem->IsInplaceButtonVisible(pButton))
			if ((pButton->GetAlignment() & DT_RIGHT) == 0)  rcValue.left += pButton->GetWidth();
	}

	if (pItem->OnDrawItemValue(*pDC, rcValue))
		return;

	CXTPPropertyGridItemMetrics* pMetrics = pItem->GetMetrics(TRUE, FALSE);
	if (pMetrics && pMetrics->m_nImage != -1)
	{
		CXTPImageManagerIcon* pImage = m_pGrid->GetImageManager()->GetImage(pMetrics->m_nImage, 0);

		if (pImage)
		{
			int nHeight = min(pImage->GetHeight(), rcValue.Height());
			int nWidth = pImage->GetWidth();

			CPoint pt(rcValue.left, rcValue.CenterPoint().y - nHeight/2);
			pImage->Draw(pDC, pt, CSize(nWidth, nHeight));

			rcValue.left += nWidth + 4;

		}

	}
	rcValue.DeflateRect(0, 1);

	UINT nFormat = pMetrics ? pMetrics->m_uDrawTextFormat : DT_VCENTER | DT_SINGLELINE;

	if (pItem->IsMultiLine()) nFormat = (nFormat | DT_WORDBREAK) & ~DT_SINGLELINE;

	pDC->DrawText(pItem->GetViewValue(), rcValue, DT_NOPREFIX | nFormat);
}

void CXTPPropertyGridPaintManager::DrawItemCaption(CDC* pDC, CXTPPropertyGridItem* pItem, CRect rcCaption)
{
	CRect rcText(rcCaption);
	rcText.left = (pItem->GetIndent() + (pItem->IsCategory() ? 1 : 0)) * XTP_PGI_EXPAND_BORDER + 3;

	rcText.right = rcCaption.right - 1;
	rcText.bottom -= 1;

	CXTPPropertyGridItemMetrics* pMetrics = pItem->GetMetrics(FALSE, FALSE);
	if (pMetrics && pMetrics->m_nImage != -1)
	{
		CXTPImageManagerIcon* pImage = m_pGrid->GetImageManager()->GetImage(pMetrics->m_nImage, 0);

		if (pImage)
		{
			int nHeight = min(pImage->GetHeight(), rcText.Height());
			int nWidth = pImage->GetWidth();

			CPoint pt(rcText.left, rcText.CenterPoint().y - nHeight/2);
			pImage->Draw(pDC, pt, CSize(nWidth, nHeight));

			rcText.left += nWidth + 4;
		}

	}

	if (pItem->GetMarkupCaption())
	{
		XTPMarkupSetDefaultFont(XTPMarkupElementContext(pItem->GetMarkupCaption()),
			(HFONT)GetItemFont(pItem, FALSE)->GetSafeHandle(), pDC->GetTextColor());

		XTPMarkupMeasureElement(pItem->GetMarkupCaption(), INT_MAX, INT_MAX);
		XTPMarkupRenderElement(pItem->GetMarkupCaption(), pDC->GetSafeHdc(), rcText);
	}
	else
	{
		UINT nFormat = pMetrics ? pMetrics->m_uDrawTextFormat : DT_VCENTER | DT_SINGLELINE;

		pDC->DrawText(pItem->GetCaption(), rcText, DT_NOPREFIX | nFormat);
	}
}

void CXTPPropertyGridPaintManager::DrawInplaceButtons(CDC* pDC, PDRAWITEMSTRUCT lpDrawItemStruct, CXTPPropertyGridItem* pItem, CRect rcValue)
{
	if (!pItem->IsAllowEdit())
		return;

	BOOL bSelected = lpDrawItemStruct ? (lpDrawItemStruct->itemState & (ODS_SELECTED | ODS_FOCUS)) : TRUE;
	BOOL bShowAlways = m_pGrid->GetShowInplaceButtonsAlways();

	int nRight = rcValue.right;
	int nLeft = rcValue.left + 2;

	for (int i = pItem->GetInplaceButtons()->GetCount() - 1; i >= 0; i--)
	{
		CXTPPropertyGridInplaceButton* pButton = pItem->GetInplaceButtons()->GetAt(i);

		if (!(bSelected || bShowAlways || pButton->GetShowAlways()))
			continue;

		int nWidth = pButton->GetWidth();

		if (pButton->GetAlignment() & DT_RIGHT)
		{
			CRect rcButton(nRight - nWidth, rcValue.top, nRight, rcValue.bottom);
			pButton->OnDraw(pDC, rcButton);
			nRight -= nWidth;
		}
		else
		{
			CRect rcButton(nLeft, rcValue.top, nLeft + nWidth, rcValue.bottom);
			pButton->OnDraw(pDC, rcButton);
			nLeft += nWidth;
		}
	}
}

void CXTPPropertyGridPaintManager::DrawItem(PDRAWITEMSTRUCT lpDrawItemStruct)
{
	CXTPPropertyGridView* pView = &m_pGrid->GetGridView();
	CXTPPropertyGridItem* pItem = (CXTPPropertyGridItem*)lpDrawItemStruct->itemData;

	CXTPWindowRect rcWindow(pView);
	CRect rc = lpDrawItemStruct->rcItem;

	CXTPBufferDC dc(lpDrawItemStruct->hDC, rc);
	CXTPFontDC font(&dc, GetItemFont(pItem, TRUE));

	dc.FillSolidRect(rc, GetItemBackColor(pItem, FALSE));
	dc.SetBkMode(TRANSPARENT);


	CRect rcCaption(rc.left, rc.top, rc.left + pView->GetDividerPos(), rc.bottom);

	if (pItem->IsCategory())
	{
		DrawCategoryCaptionBackground(&dc, pItem, rc);

		CFont* pFont = GetItemFont(pItem, FALSE);
		font.SetFont(pFont);

		dc.SetTextColor(GetItemTextColor(pItem, FALSE));
		CRect rcText(((pItem->GetIndent() + 1) * XTP_PGI_EXPAND_BORDER) + 3, rc.top, rc.right, rc.bottom - 1);

		CXTPPropertyGridItemMetrics* pMetrics = pItem->GetMetrics(FALSE, FALSE);
		if (pMetrics && pMetrics->m_nImage != -1)
		{
			CXTPImageManagerIcon* pImage = m_pGrid->GetImageManager()->GetImage(pMetrics->m_nImage, 0);

			if (pImage)
			{
				int nHeight = min(pImage->GetHeight(), rc.Height());
				int nWidth = pImage->GetWidth();

				CPoint pt(rcText.left, rcText.CenterPoint().y - nHeight/2);
				pImage->Draw(&dc, pt, CSize(nWidth, nHeight));

				rcText.left += nWidth + 4;
			}
		}

		if (pItem->GetMarkupCaption())
		{
			XTPMarkupSetDefaultFont(XTPMarkupElementContext(pItem->GetMarkupCaption()),
				(HFONT)pFont->GetSafeHandle(), dc.GetTextColor());

			XTPMarkupMeasureElement(pItem->GetMarkupCaption(), rcText.Width(), rcText.Height());
			XTPMarkupRenderElement(pItem->GetMarkupCaption(), dc.GetSafeHdc(), rcText);
		}
		else
		{
			dc.DrawText(pItem->GetCaption(), rcText, DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER | DT_LEFT);
		}

		if (lpDrawItemStruct->itemState & ODS_FOCUS)
		{
			CXTPEmptyRect rcFocus;
			if (pItem->GetMarkupCaption())
			{
				CSize sz = XTPMarkupMeasureElement(pItem->GetMarkupCaption(), rcText.Width(), rcText.Height());
				rcFocus.right = rcFocus.left + sz.cx;
			}
			else
			{
				dc.DrawText(pItem->GetCaption(), rcFocus, DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER | DT_CALCRECT);
			}

			rcFocus.SetRect(rcText.left - 3, rcText.top + 1, rcText.left + rcFocus.Width() + 3, rcText.bottom - 1);

			dc.SetTextColor(0);
			dc.DrawFocusRect(rcFocus);
		}

		DrawInplaceButtons(&dc, lpDrawItemStruct, pItem, rcText);
	}
	else
	{

		CRect rcValue(rcCaption.right, rc.top, rc.right, rc.bottom - 1);
		DrawItemValue(&dc, pItem, rcValue);

		dc.SetTextColor(GetItemTextColor(pItem, FALSE));
		font.SetFont(GetItemFont(pItem, FALSE));


		BOOL bSelected = lpDrawItemStruct->itemState & ODS_SELECTED;
		BOOL bFocused = bSelected && CWnd::GetFocus() && ((CWnd::GetFocus() == pView) || (CWnd::GetFocus()->GetParent() == pView) || (CWnd::GetFocus()->GetOwner() == pView));
		if (bFocused)
		{
			dc.SetTextColor(m_clrHighlightText);
			dc.FillSolidRect(rcCaption, m_clrHighlight);
		}
		else if (bSelected && !m_pGrid->m_bHideSelection)
		{
			dc.FillSolidRect(rcCaption, m_pMetrics->m_clrLine);
		}

		CXTPPropertyGridItem* pCategory = pItem->GetParentItem();
		while (pCategory != NULL)
		{
			if (pCategory->IsCategory())
				dc.FillSolidRect(rc.left + pCategory->GetIndent() * XTP_PGI_EXPAND_BORDER, rc.top, XTP_PGI_EXPAND_BORDER, rc.Height(), m_pMetrics->m_clrLine);

			pCategory = pCategory->GetParentItem();
		}

		CXTPPenDC pen(dc, m_pMetrics->m_clrLine);
		dc.MoveTo(0, rc.bottom - 1); dc.LineTo(rc.right, rc.bottom - 1);
		dc.MoveTo(rcCaption.right, rc.top); dc.LineTo(rcCaption.right, rc.bottom - 1);

		DrawItemCaption(&dc, pItem, rcCaption);

		DrawInplaceButtons(&dc, lpDrawItemStruct, pItem, rcValue);
	}

	CXTPPropertyGridItem* pNext = pView->GetItem(lpDrawItemStruct->itemID + 1);
	if (pNext && pNext->IsCategory() && pNext->IsVisible())
	{
		dc.FillSolidRect(pNext->GetIndent() * XTP_PGI_EXPAND_BORDER, rc.bottom - 1, rc.Width(), 1, m_clrCategorySeparator.IsDefaultValue() ? m_clrFace : m_clrCategorySeparator);
	}



	if (pItem->HasVisibleChilds())
	{
		CRgn rgn;
		if (!pItem->IsCategory())
		{
			rgn.CreateRectRgnIndirect(&rcCaption);
			dc.SelectClipRgn(&rgn);
		}

		DrawExpandButton(dc, pItem, rcCaption);

		dc.SelectClipRgn(NULL);
	}
}

void CXTPPropertyGridPaintManager::DrawExpandButton(CDC& dc, CXTPPropertyGridItem* pItem, CRect rcCaption)
{
	if (!pItem->IsExpandable())
		return;

	CRect rcSign(CPoint(XTP_PGI_EXPAND_BORDER / 2 - 5, rcCaption.CenterPoint().y - 4), CSize(9, 9));

	if (pItem->GetIndent() > 0)
		rcSign.OffsetRect((pItem->GetIndent() - (pItem->IsCategory() ? 0 : 1)) * XTP_PGI_EXPAND_BORDER, 0);


	if (m_buttonsStyle == xtpGridButtonsThemed && m_themeTree->IsAppThemed())
	{
		m_themeTree->DrawThemeBackground(dc, TVP_GLYPH, pItem->IsExpanded() ? GLPS_OPENED : GLPS_CLOSED, &rcSign, 0);

	}
	else
	{

		CPoint pt = rcSign.CenterPoint();

		if (!pItem->IsCategory()) dc.FillSolidRect(rcSign, m_pMetrics->m_clrBack);
		dc.Draw3dRect(rcSign, m_pMetrics->m_clrFore, m_pMetrics->m_clrFore);
		CXTPPenDC pen (dc, m_pMetrics->m_clrFore);

		dc.MoveTo(pt.x - 2, pt.y);
		dc.LineTo(pt.x + 3, pt.y);

		if (!pItem->IsExpanded())
		{
			dc.MoveTo(pt.x, pt.y - 2);
			dc.LineTo(pt.x, pt.y + 3);
		}
	}
}

void CXTPPropertyGridPaintManager::FillInplaceButton(CDC* pDC, CXTPPropertyGridInplaceButton* pButton)
{
	CRect rect(pButton->GetRect());
	CPoint ptDots(rect.CenterPoint().x , rect.CenterPoint().y + 3);
	COLORREF clrText = GetXtremeColor(pButton->GetEnabled() ? COLOR_BTNTEXT : COLOR_GRAYTEXT);

	if (pButton->IsHyperlink())
	{
		clrText = !pButton->GetEnabled() ? GetXtremeColor(COLOR_GRAYTEXT) : m_pMetrics->m_clrVerbFace;

	}
	else if (m_buttonsStyle == xtpGridButtonsThemed && m_themeButton->IsAppThemed() && m_themeCombo->IsAppThemed())
	{
		pDC->FillSolidRect(rect, m_pMetrics->m_clrBack);

		if (pButton->GetID() != XTP_ID_PROPERTYGRID_COMBOBUTTON)
		{
			m_themeButton->DrawThemeBackground(*pDC, BP_PUSHBUTTON, !pButton->GetEnabled() ? PBS_DISABLED : pButton->IsPressed() && pButton->IsHot() ? PBS_PRESSED :
				pButton->IsHot() ? PBS_HOT: pButton->IsFocused() ? PBS_DEFAULTED : PBS_NORMAL, rect, 0);
		}
		else
		{
			m_themeCombo->DrawThemeBackground(*pDC, CP_DROPDOWNBUTTON, !pButton->GetEnabled() ? CBXS_DISABLED :
				pButton->IsPressed() && pButton->IsHot() ? CBXS_PRESSED: pButton->IsHot() ? CBXS_HOT : CBXS_NORMAL, rect, 0);
		}
	}
	else
	{
		if (m_buttonsStyle == xtpGridButtonsOfficeXP)
		{
			pDC->FillSolidRect(rect, GetXtremeColor(pButton->IsPressed() && pButton->IsHot() ? XPCOLOR_HIGHLIGHT_PUSHED :
				pButton->IsHot() ? XPCOLOR_HIGHLIGHT : XPCOLOR_TOOLBAR_FACE));
			COLORREF clrBorder = GetXtremeColor(pButton->IsPressed() && pButton->IsHot() ? XPCOLOR_HIGHLIGHT_PUSHED_BORDER :
				pButton->IsHot() ? XPCOLOR_HIGHLIGHT_BORDER : COLOR_3DSHADOW);
			pDC->Draw3dRect(rect, clrBorder, clrBorder);
		}
		else
		{
			pDC->FillSolidRect(rect, GetXtremeColor(COLOR_3DFACE));

			if (pButton->IsPressed() && pButton->IsHot())
				pDC->Draw3dRect(rect, GetXtremeColor(COLOR_3DSHADOW), GetXtremeColor(COLOR_3DSHADOW));
			else
			{
				pDC->Draw3dRect(rect, GetXtremeColor(COLOR_3DFACE), GetXtremeColor(COLOR_3DDKSHADOW));
				rect.DeflateRect(1, 1);
				pDC->Draw3dRect(rect, GetXtremeColor(COLOR_WINDOW), GetXtremeColor(COLOR_3DSHADOW));
			}
		}

		if (pButton->GetID() == XTP_ID_PROPERTYGRID_COMBOBUTTON)
		{
			CPoint pt = rect.CenterPoint();
			CXTPDrawHelpers::Triangle(pDC, CPoint(pt.x -3, pt.y -1), CPoint(pt.x + 3, pt.y -1), CPoint(pt.x, pt.y +2), clrText);
		}
	}
	BOOL bCustom = FALSE;

	if (pButton->GetID() != XTP_ID_PROPERTYGRID_COMBOBUTTON)
	{
		CXTPImageManagerIcon* pIcon = pButton->GetImage();
		if (pIcon)
		{
			CSize sz(pIcon->GetWidth(), pIcon->GetHeight());
			CPoint pt((rect.left + rect.right - sz.cx) / 2, (rect.top + rect.bottom - sz.cy) / 2);
			pIcon->Draw(pDC, pt, sz);
			bCustom = TRUE;
		}
		else if (!pButton->GetCaption().IsEmpty())
		{
			CXTPFontDC font(pDC, pButton->IsHyperlink() ? &m_pMetrics->m_fontUnderline :
				(m_pGrid->IsHighlightChangedButtonItems() ? GetItemFont(pButton->GetItem(), TRUE) : &m_pMetrics->m_fontNormal));

			pDC->SetTextColor(clrText);
			pDC->DrawText(pButton->GetCaption(), rect, DT_SINGLELINE | DT_VCENTER | DT_CENTER | DT_NOPREFIX);
			bCustom = TRUE;
		}
	}

	if (!bCustom && pButton->GetID() == XTP_ID_PROPERTYGRID_EXPANDBUTTON)
	{
		pDC->FillSolidRect(ptDots.x - 4, ptDots.y, 2, 2, clrText);
		pDC->FillSolidRect(ptDots.x, ptDots.y, 2, 2, clrText);
		pDC->FillSolidRect(ptDots.x + 4, ptDots.y, 2, 2, clrText);
	}



	if (pButton->IsFocused())
	{
		pDC->SetTextColor(GetXtremeColor(COLOR_BTNTEXT));
		pDC->SetBkColor(GetXtremeColor(COLOR_3DFACE));
		rect.DeflateRect(2, 2);
		pDC->DrawFocusRect(rect);
	}
}


void CXTPPropertyGridPaintManager::DrawCheckMark(CDC* pDC, CRect rc, BOOL bEnabled, BOOL bChecked)
{
	if (m_buttonsStyle == xtpGridButtonsThemed && m_themeButton->IsAppThemed())
	{
		m_themeButton->DrawThemeBackground(pDC->GetSafeHdc(), BP_CHECKBOX,
			!bEnabled ? (bChecked ? CBS_CHECKEDDISABLED : CBS_UNCHECKEDDISABLED) :
		(bChecked ? CBS_CHECKEDNORMAL : CBS_UNCHECKEDNORMAL),
			rc, 0);
	}
	else
	{
		pDC->DrawFrameControl(rc, DFC_BUTTON, DFCS_BUTTONCHECK |
			(!bEnabled ? DFCS_INACTIVE : 0) | (bChecked ? DFCS_CHECKED : 0));
	}
}

void CXTPPropertyGridPaintManager::DrawRadioMark(CDC* pDC, CRect rc, BOOL bEnabled, BOOL bChecked)
{
	if (m_buttonsStyle == xtpGridButtonsThemed && m_themeButton->IsAppThemed())
	{
		m_themeButton->DrawThemeBackground(pDC->GetSafeHdc(), BP_RADIOBUTTON,
			!bEnabled ? (bChecked ? RBS_CHECKEDDISABLED : RBS_UNCHECKEDDISABLED) :
		(bChecked ? RBS_CHECKEDNORMAL : RBS_UNCHECKEDNORMAL),
			rc, 0);
	}
	else
	{
		pDC->DrawFrameControl(rc, DFC_BUTTON, DFCS_BUTTONRADIO |
			(!bEnabled ? DFCS_INACTIVE : 0) | (bChecked ? DFCS_CHECKED : 0));
	}
}

void CXTPPropertyGridPaintManager::DrawCheckBox(CDC* pDC, CXTPPropertyGridItemBool* pItem)
{
	CRect rcValue = pItem->GetValueRect();
	CRect rcSample(rcValue.left + 2, rcValue.CenterPoint().y - 6, rcValue.left + 2 + 13, rcValue.CenterPoint().y + 7);

	DrawCheckMark(pDC, rcSample, !pItem->GetReadOnly(), pItem->GetBool());
}


//////////////////////////////////////////////////////////////////////
// CXTPPropertyGridNativeXPTheme

CXTPPropertyGridNativeXPTheme::CXTPPropertyGridNativeXPTheme(CXTPPropertyGrid* pGrid)
	: CXTPPropertyGridPaintManager(pGrid)
{
	m_buttonsStyle = xtpGridButtonsThemed;
}

void CXTPPropertyGridNativeXPTheme::RefreshMetrics()
{
	CXTPPropertyGridPaintManager::RefreshMetrics();

	m_clrShadow.SetStandardValue(GetXtremeColor(XPCOLOR_STATICFRAME));
}

//////////////////////////////////////////////////////////////////////
// CXTPPropertyGridOffice2003Theme

CXTPPropertyGridOffice2003Theme::CXTPPropertyGridOffice2003Theme(CXTPPropertyGrid* pGrid)
	: CXTPPropertyGridPaintManager(pGrid)
{
	m_bLunaTheme = FALSE;
	m_buttonsStyle = xtpGridButtonsThemed;
}



void CXTPPropertyGridOffice2003Theme::RefreshMetrics()
{
	CXTPPropertyGridPaintManager::RefreshMetrics();

	m_bLunaTheme = FALSE;

	if (!XTPColorManager()->IsLunaColorsDisabled())
	{
		XTPCurrentSystemTheme systemTheme = XTPColorManager()->GetCurrentSystemTheme();

		switch (systemTheme)
		{
		case xtpSystemThemeRoyale:
		case xtpSystemThemeAero:
		case xtpSystemThemeBlue:
			m_clrFace.SetStandardValue(RGB(221, 236, 254));
			m_clrShadow.SetStandardValue(RGB(123, 164, 224));
			m_pMetrics->m_clrHelpBack.SetStandardValue(m_clrFace);
			m_pMetrics->m_clrLine.SetStandardValue(RGB(169, 199, 240));
			m_pMetrics->m_clrCategoryFore.SetStandardValue(0);

			m_bLunaTheme = TRUE;
			break;

		case xtpSystemThemeOlive:
			m_clrFace.SetStandardValue(RGB(243, 242, 231));
			m_clrShadow.SetStandardValue(RGB(188, 187, 177));
			m_pMetrics->m_clrHelpBack.SetStandardValue(m_clrFace);
			m_pMetrics->m_clrLine.SetStandardValue(RGB(197, 212, 159));
			m_pMetrics->m_clrCategoryFore.SetStandardValue(0);

			m_bLunaTheme = TRUE;
			break;

		case xtpSystemThemeSilver:
			m_clrFace.SetStandardValue(RGB(238, 238, 244));
			m_clrShadow.SetStandardValue(RGB(161, 160, 187));
			m_pMetrics->m_clrHelpBack.SetStandardValue(m_clrFace);
			m_pMetrics->m_clrLine.SetStandardValue(RGB(192, 192, 211));
			m_pMetrics->m_clrCategoryFore.SetStandardValue(0);

			m_bLunaTheme = TRUE;
			break;
		}
	}
}

//////////////////////////////////////////////////////////////////////
// CXTPPropertyGridOffice2007Theme

CXTPPropertyGridOffice2007Theme::CXTPPropertyGridOffice2007Theme(CXTPPropertyGrid* pGrid)
	: CXTPPropertyGridOffice2003Theme(pGrid)
{
}

void CXTPPropertyGridOffice2007Theme::RefreshMetrics()
{
	CXTPPropertyGridPaintManager::RefreshMetrics();

	m_bLunaTheme = FALSE;

	m_pMetrics->m_clrBack.SetCustomValue(RGB(255, 255, 255));
	m_pMetrics->m_clrFore.SetCustomValue(RGB(0, 0, 0));

	CXTPResourceImages* pImages = XTPResourceImages();

	CXTPResourceImage* pImage = pImages->LoadFile(_T("PUSHBUTTON"));
	if (pImage)
	{
		m_clrFace.SetStandardValue(pImages->GetImageColor(_T("PropertyGrid"), _T("Background")));
		m_clrShadow.SetStandardValue(pImages->GetImageColor(_T("PropertyGrid"), _T("Shadow")));
		m_pMetrics->m_clrHelpBack.SetStandardValue(pImages->GetImageColor(_T("PropertyGrid"), _T("HelpBack")));
		m_pMetrics->m_clrHelpFore.SetStandardValue(pImages->GetImageColor(_T("PropertyGrid"), _T("HelpFore")));
		m_pMetrics->m_clrLine.SetStandardValue(pImages->GetImageColor(_T("PropertyGrid"), _T("GridLine")));
		m_pMetrics->m_clrCategoryFore.SetStandardValue(pImages->GetImageColor(_T("PropertyGrid"), _T("CategoryText")));
		m_clrHighlightText.SetStandardValue(pImages->GetImageColor(_T("PropertyGrid"), _T("HighlightText")));
		m_clrHighlight.SetStandardValue(pImages->GetImageColor(_T("PropertyGrid"), _T("Highlight")));
	}
	else
	{
		m_clrFace.SetStandardValue(RGB(173, 209, 255));
		m_clrShadow.SetStandardValue(RGB(101, 147, 207));
		m_pMetrics->m_clrHelpBack.SetStandardValue(RGB(214, 232, 255));
		m_pMetrics->m_clrHelpFore.SetStandardValue(RGB(0, 0, 0));
		m_pMetrics->m_clrLine.SetStandardValue(RGB(214, 232, 255));
		m_pMetrics->m_clrCategoryFore.SetStandardValue(RGB(21, 66, 139));
		m_clrHighlightText.SetStandardValue(0);
		m_clrHighlight.SetStandardValue(RGB(179, 200, 232));
	}
	m_pMetrics->m_clrVerbFace.SetStandardValue(RGB(0, 102, 204));

}

void CXTPPropertyGridOffice2007Theme::DrawRadioMark(CDC* pDC, CRect rc, BOOL bEnabled, BOOL bChecked)
{
	CXTPResourceImages* pImages = XTPResourceImages();

	CXTPResourceImage* pImage = pImages->LoadFile(_T("TOOLBARBUTTONRADIOBUTTON"));
	if (!pImage)
	{
		CXTPPropertyGridOffice2003Theme::DrawCheckMark(pDC, rc, bEnabled, bChecked);
		return;
	}

	int nState = !bEnabled ? 3 : 0;
	if (bChecked) nState += 4;
	CRect rcSrc(pImage->GetSource(nState, 8));

	CRect rcSample(CPoint(rc.left, rc.CenterPoint().y - rcSrc.Height() / 2), rcSrc.Size());

	pImage->DrawImage(pDC, rcSample, rcSrc, CRect(0, 0, 0, 0));
}

void CXTPPropertyGridOffice2007Theme::DrawCheckMark(CDC* pDC, CRect rc, BOOL bEnabled, BOOL bChecked)
{
	CXTPResourceImages* pImages = XTPResourceImages();

	CXTPResourceImage* pImage = pImages->LoadFile(_T("TOOLBARBUTTONCHECKBOX"));
	if (!pImage)
	{
		CXTPPropertyGridOffice2003Theme::DrawCheckMark(pDC, rc, bEnabled, bChecked);
		return;
	}

	int nState = !bEnabled ? 3 : 0;
	if (bChecked) nState += 4;
	CRect rcSrc(pImage->GetSource(nState, 12));

	CRect rcSample(CPoint(rc.left, rc.CenterPoint().y - rcSrc.Height() / 2), rcSrc.Size());

	pImage->DrawImage(pDC, rcSample, rcSrc, CRect(0, 0, 0, 0));
}

void CXTPPropertyGridOffice2007Theme::DrawExpandButton(CDC& dc, CXTPPropertyGridItem* pItem, CRect rcCaption)
{
	if (!pItem->IsExpandable())
		return;

	if (m_buttonsStyle == xtpGridButtonsThemed)
	{
		CXTPResourceImages* pImages = XTPResourceImages();

		CXTPResourceImage* pImage = pImages->LoadFile(_T("REPORTEXPANDBUTTON"));

		if (!pImage)
		{
			CXTPPropertyGridOffice2003Theme::DrawExpandButton(dc, pItem, rcCaption);
		}
		else
		{
			CRect rcSign(CPoint(XTP_PGI_EXPAND_BORDER / 2 - 8, rcCaption.CenterPoint().y - 8), CSize(16, 16));

			if (pItem->GetIndent() > 0)
				rcSign.OffsetRect((pItem->GetIndent() - (pItem->IsCategory() ? 0 : 1)) * XTP_PGI_EXPAND_BORDER, 0);

			CRect rcSrc(pImage->GetSource(pItem->IsExpanded() ? 0 : 2, 4));

			pImage->DrawImage(&dc, rcSign, rcSrc);
		}
	}
	else
	{
		CXTPPropertyGridOffice2003Theme::DrawExpandButton(dc, pItem, rcCaption);
	}
}


void CXTPPropertyGridOffice2007Theme::FillInplaceButton(CDC* pDC, CXTPPropertyGridInplaceButton* pButton)
{
	CRect rect(pButton->GetRect());
	CPoint ptDots(rect.CenterPoint().x , rect.CenterPoint().y + 3);
	COLORREF clrText = GetXtremeColor(pButton->GetEnabled() ? COLOR_BTNTEXT : COLOR_GRAYTEXT);

	if (pButton->IsHyperlink())
	{
		clrText = !pButton->GetEnabled() ? GetXtremeColor(COLOR_GRAYTEXT) : m_pMetrics->m_clrVerbFace;

	}
	else
	{
		CXTPResourceImages* pImages = XTPResourceImages();

		CXTPResourceImage* pImage = pImages->LoadFile(_T("PUSHBUTTON"));
		if (pImage)
		{
			int nState = pButton->IsPressed() && pButton->IsHot() ? 2 : pButton->IsHot() ? 1 : 0;
			pImage->DrawImage(pDC, rect, pImage->GetSource(nState, 5), CRect(3, 3, 3, 3), 0xFF00FF);
		}
		else
		{
			pDC->FillSolidRect(rect, pButton->IsPressed() && pButton->IsHot() ? RGB(255, 171, 63) :
		pButton->IsHot() ? RGB(255, 231, 162) : m_clrFace);

		COLORREF clrBorder = GetXtremeColor(pButton->IsPressed() && pButton->IsHot() ? RGB(251, 140, 60) :
		pButton->IsHot() ? RGB(255, 189, 105) : m_clrShadow);
		pDC->Draw3dRect(rect, clrBorder, clrBorder);
		}
	}



	if (pButton->GetID() == XTP_ID_PROPERTYGRID_COMBOBUTTON)
	{
		CPoint pt = rect.CenterPoint();
		CXTPDrawHelpers::Triangle(pDC, CPoint(pt.x -3, pt.y -1), CPoint(pt.x + 3, pt.y -1), CPoint(pt.x, pt.y +2), clrText);
	}

	BOOL bCustom = FALSE;

	if (pButton->GetID() != XTP_ID_PROPERTYGRID_COMBOBUTTON)
	{
		CXTPImageManagerIcon* pIcon = pButton->GetImage();
		if (pIcon)
		{
			CSize sz(pIcon->GetWidth(), pIcon->GetHeight());
			CPoint pt((rect.left + rect.right - sz.cx) / 2, (rect.top + rect.bottom - sz.cy) / 2);
			pIcon->Draw(pDC, pt, sz);
			bCustom = TRUE;
		}
		else if (!pButton->GetCaption().IsEmpty())
		{
			CXTPFontDC font(pDC, pButton->IsHyperlink() ? &m_pMetrics->m_fontUnderline : GetItemFont(pButton->GetItem(), TRUE));
			pDC->SetTextColor(clrText);
			pDC->DrawText(pButton->GetCaption(), rect, DT_SINGLELINE | DT_VCENTER | DT_CENTER | DT_NOPREFIX);
			bCustom = TRUE;
		}
	}

	if (!bCustom && pButton->GetID() == XTP_ID_PROPERTYGRID_EXPANDBUTTON)
	{
		pDC->FillSolidRect(ptDots.x - 4, ptDots.y, 2, 2, clrText);
		pDC->FillSolidRect(ptDots.x, ptDots.y, 2, 2, clrText);
		pDC->FillSolidRect(ptDots.x + 4, ptDots.y, 2, 2, clrText);
	}



	if (pButton->IsFocused())
	{
		pDC->SetTextColor(GetXtremeColor(COLOR_BTNTEXT));
		pDC->SetBkColor(GetXtremeColor(COLOR_3DFACE));
		rect.DeflateRect(2, 2);
		pDC->DrawFocusRect(rect);
	}
}

//////////////////////////////////////////////////////////////////////
// CXTPPropertyGridCoolTheme

CXTPPropertyGridCoolTheme::CXTPPropertyGridCoolTheme(CXTPPropertyGrid* pGrid)
	: CXTPPropertyGridPaintManager(pGrid)
{
}

void CXTPPropertyGridCoolTheme::RefreshMetrics()
{
	CXTPPropertyGridPaintManager::RefreshMetrics();

	m_pMetrics->m_clrLine.SetStandardValue(GetXtremeColor(COLOR_3DFACE));
}


void CXTPPropertyGridCoolTheme::DrawItem(PDRAWITEMSTRUCT lpDrawItemStruct)
{
	CXTPPropertyGridView* pView = &m_pGrid->GetGridView();
	CXTPPropertyGridItem* pItem = (CXTPPropertyGridItem*)lpDrawItemStruct->itemData;

	ASSERT(pItem != NULL);
	if (!pItem)
		return;

	CXTPWindowRect rcWindow(pView);
	CRect rc = lpDrawItemStruct->rcItem;

	CXTPBufferDC dc(lpDrawItemStruct->hDC, rc);
	CXTPFontDC font(&dc, GetItemFont(pItem, TRUE));

	dc.FillSolidRect(rc, GetItemBackColor(pItem, FALSE));
	dc.SetBkMode(TRANSPARENT);


	CRect rcCaption(rc.left, rc.top, rc.left + pView->GetDividerPos(), rc.bottom);

	COLORREF clrLine = m_pMetrics->m_clrLine;
	COLORREF clrShadow = GetXtremeColor(COLOR_3DSHADOW);

	if (pItem->IsCategory())
	{
		dc.FillSolidRect(rc, clrLine);

		CFont* pFont = GetItemFont(pItem, FALSE);
		font.SetFont(pFont);
		dc.SetTextColor(GetItemTextColor(pItem, FALSE));

		int nIndent = XTP_PGI_EXPAND_BORDER * (pItem->GetIndent() + 1);

		if (lpDrawItemStruct->itemState & ODS_FOCUS)
		{
			dc.SetTextColor(m_clrHighlightText);
			dc.FillSolidRect(CRect(nIndent - 1, rc.top , rc.right, rc.bottom), m_clrHighlight);
		}

		CRect rcText(nIndent + 3, rc.top , rc.right, rc.bottom - 1);

		CXTPPropertyGridItemMetrics* pMetrics = pItem->GetMetrics(FALSE, FALSE);
		if (pMetrics && pMetrics->m_nImage != -1)
		{
			CXTPImageManagerIcon* pImage = m_pGrid->GetImageManager()->GetImage(pMetrics->m_nImage, 0);

			if (pImage)
			{
				int nHeight = min(pImage->GetHeight(), rc.Height());
				int nWidth = pImage->GetWidth();

				CPoint pt(rcText.left, rcText.CenterPoint().y - nHeight/2);
				pImage->Draw(&dc, pt, CSize(nWidth, nHeight));

				rcText.left += nWidth + 4;
			}
		}

		if (pItem->GetMarkupCaption())
		{
			XTPMarkupSetDefaultFont(XTPMarkupElementContext(pItem->GetMarkupCaption()),
				(HFONT)pFont->GetSafeHandle(), dc.GetTextColor());

			XTPMarkupMeasureElement(pItem->GetMarkupCaption(), rcText.Width(), rcText.Height());
			XTPMarkupRenderElement(pItem->GetMarkupCaption(), dc.GetSafeHdc(), rcText);
		}
		else
		{
			dc.DrawText(pItem->GetCaption(), rcText, DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER);
		}

		dc.FillSolidRect((pItem->GetIndent() + (pItem->IsExpanded() ? 1 : 0)) * XTP_PGI_EXPAND_BORDER - 1, rc.bottom - 1, rc.Width(), 1, clrShadow);

		DrawInplaceButtons(&dc, lpDrawItemStruct, pItem, rcText);

		CXTPPropertyGridItem* pCategory = pItem->GetParentItem();
		while (pCategory != NULL)
		{
			if (pCategory->IsCategory())
			{
				dc.FillSolidRect(rc.left + pCategory->GetIndent() * XTP_PGI_EXPAND_BORDER, rc.top, XTP_PGI_EXPAND_BORDER, rc.Height(), clrLine);
				dc.FillSolidRect(rc.left + (pCategory->GetIndent() + 1) * XTP_PGI_EXPAND_BORDER - 1, rc.top, 1, rc.Height() + 2, clrShadow);
			}
			pCategory = pCategory->GetParentItem();
		}
	}
	else
	{
		CRect rcValue(rcCaption.right, rc.top, rc.right, rc.bottom - 1);
		DrawItemValue(&dc, pItem, rcValue);

		dc.SetTextColor(GetItemTextColor(pItem, FALSE));
		font.SetFont(GetItemFont(pItem, FALSE));

		BOOL bSelected = lpDrawItemStruct->itemState & ODS_SELECTED;
		BOOL bFocused = bSelected && CWnd::GetFocus() && ((CWnd::GetFocus() == pView) || (CWnd::GetFocus()->GetParent() == pView) || (CWnd::GetFocus()->GetOwner() == pView));
		if (bFocused)
		{
			dc.SetTextColor(m_clrHighlightText);
			dc.FillSolidRect(rcCaption, m_clrHighlight);
		}
		else if (bSelected && !m_pGrid->m_bHideSelection)
		{
			dc.FillSolidRect(rcCaption, clrLine);
		}

		CXTPPenDC pen(dc, clrLine);
		dc.MoveTo(0, rc.bottom - 1);
		dc.LineTo(rc.right, rc.bottom - 1);
		dc.MoveTo(rcCaption.right, rc.top);
		dc.LineTo(rcCaption.right, rc.bottom - 1);

		CXTPPropertyGridItem* pCategory = pItem->GetParentItem();
		while (pCategory != NULL)
		{
			if (pCategory->IsCategory())
			{
				dc.FillSolidRect(rc.left + pCategory->GetIndent() * XTP_PGI_EXPAND_BORDER, rc.top, XTP_PGI_EXPAND_BORDER, rc.Height(), clrLine);
				dc.FillSolidRect(rc.left + (pCategory->GetIndent() + 1) * XTP_PGI_EXPAND_BORDER - 1, rc.top, 1, rc.Height() + 2, clrShadow);
			}
			pCategory = pCategory->GetParentItem();
		}

		DrawItemCaption(&dc, pItem, rcCaption);

		DrawInplaceButtons(&dc, lpDrawItemStruct, pItem, rcValue);
	}

	CXTPPropertyGridItem* pNext = (CXTPPropertyGridItem*)pView->GetItemDataPtr(lpDrawItemStruct->itemID + 1);
	if (((ULONG_PTR)pNext == (ULONG_PTR)(-1)))
		dc.FillSolidRect(0, rc.bottom - 1, rc.Width(), 1, clrShadow);
	else if (pNext && pNext->IsCategory() && pNext->IsVisible())
	{
		dc.FillSolidRect(pNext->GetIndent() * XTP_PGI_EXPAND_BORDER, rc.bottom - 1, rc.Width(), 1, clrShadow);
	}

	CXTPPropertyGridItem* pPrev = pItem;
	if (!pPrev)
		return;

	CXTPPropertyGridItem* pCategory = pPrev->GetParentItem();
	while (pCategory != NULL)
	{
		if (pItem->HasChilds() && pItem->IsExpanded())
			break;

		if (pCategory->IsCategory() && pCategory->IsVisible() && pCategory->GetChilds()->GetAt(pCategory->GetChilds()->GetCount() - 1) == pPrev)
		{
			dc.FillSolidRect(pCategory->GetIndent() * XTP_PGI_EXPAND_BORDER, rc.bottom - 1, rc.Width(), 1, clrShadow);
		}
		else
			break;
		pPrev = pCategory;
		pCategory = pPrev->GetParentItem();
	}


	if (pItem->HasVisibleChilds())
	{
		CRgn rgn;
		if (!pItem->IsCategory())
		{
			rgn.CreateRectRgnIndirect(&rcCaption);
			dc.SelectClipRgn(&rgn);
		}

		DrawExpandButton(dc, pItem, rcCaption);

		dc.SelectClipRgn(NULL);
	}
}



//////////////////////////////////////////////////////////////////////
// CXTPPropertyGridSimpleTheme

CXTPPropertyGridSimpleTheme::CXTPPropertyGridSimpleTheme(CXTPPropertyGrid* pGrid)
	: CXTPPropertyGridPaintManager(pGrid)
{
}

void CXTPPropertyGridSimpleTheme::RefreshMetrics()
{
	CXTPPropertyGridPaintManager::RefreshMetrics();

	m_pMetrics->m_clrLine.SetStandardValue(GetXtremeColor(COLOR_3DFACE));
}



void CXTPPropertyGridSimpleTheme::DrawItem(PDRAWITEMSTRUCT lpDrawItemStruct)
{
	CXTPPropertyGridView* pView = &m_pGrid->GetGridView();
	CXTPPropertyGridItem* pItem = (CXTPPropertyGridItem*)lpDrawItemStruct->itemData;

	ASSERT(pItem != NULL);
	if (!pItem)
		return;

	CXTPWindowRect rcWindow(pView);
	CRect rc = lpDrawItemStruct->rcItem;

	CXTPBufferDC dc(lpDrawItemStruct->hDC, rc);
	CXTPFontDC font(&dc, GetItemFont(pItem, FALSE));

	dc.FillSolidRect(rc, GetItemBackColor(pItem, FALSE));
	dc.SetBkMode(TRANSPARENT);


	CRect rcCaption(rc.left, rc.top, rc.left + pView->GetDividerPos(), rc.bottom);

	COLORREF clrLine = m_pMetrics->m_clrLine;

	{
		if (!pItem->IsCategory())
		{
			CXTPFontDC fontValue(&dc, GetItemFont(pItem, TRUE));
			CRect rcValue(rcCaption.right, rc.top, rc.right, rc.bottom - 1);
			DrawItemValue(&dc, pItem, rcValue);
		}

		dc.SetTextColor(GetItemTextColor(pItem, FALSE));

		BOOL bSelected = lpDrawItemStruct->itemState & ODS_SELECTED;
		BOOL bFocused = bSelected && CWnd::GetFocus() && ((CWnd::GetFocus() == pView) || (CWnd::GetFocus()->GetParent() == pView) || (CWnd::GetFocus()->GetOwner() == pView));
		if (bFocused)
		{
			dc.SetTextColor(m_clrHighlightText);
			dc.FillSolidRect(CRect(XTP_PGI_EXPAND_BORDER, rcCaption.top, rcCaption.right, rcCaption.bottom), m_clrHighlight);
		}
		else if (bSelected && !m_pGrid->m_bHideSelection)
		{
			dc.FillSolidRect(CRect(XTP_PGI_EXPAND_BORDER, rcCaption.top, rcCaption.right, rcCaption.bottom), clrLine);
		}
		dc.FillSolidRect(XTP_PGI_EXPAND_BORDER - 1, rc.top, 1, rc.Height(), clrLine);

		CXTPPenDC pen(dc, clrLine);
		dc.MoveTo(XTP_PGI_EXPAND_BORDER, rc.bottom - 1);
		dc.LineTo(rc.right, rc.bottom - 1);
		dc.MoveTo(rcCaption.right, rc.top);
		dc.LineTo(rcCaption.right, rc.bottom - 1);


		DrawItemCaption(&dc, pItem, rcCaption);

		CRect rcValue(rcCaption.right, rc.top, rc.right, rc.bottom - 1);

		DrawInplaceButtons(&dc, lpDrawItemStruct, pItem, rcValue);

	}

	if (pView->GetCount() == (int)lpDrawItemStruct->itemID + 1)
		dc.FillSolidRect(0, rc.bottom - 1, rc.Width(), 1, clrLine);

	if (pItem->HasVisibleChilds())
	{
		CRgn rgn;
		rgn.CreateRectRgnIndirect(&rcCaption);
		dc.SelectClipRgn(&rgn);

		DrawExpandButton(dc, pItem, rcCaption);

		dc.SelectClipRgn(NULL);
	}
}



//////////////////////////////////////////////////////////////////////
// CXTPPropertyGridDelphiTheme

CXTPPropertyGridDelphiTheme::CXTPPropertyGridDelphiTheme(CXTPPropertyGrid* pGrid)
	: CXTPPropertyGridPaintManager(pGrid)
{
}


void CXTPPropertyGridDelphiTheme::DrawItem(PDRAWITEMSTRUCT lpDrawItemStruct)
{
	CXTPPropertyGridView* pView = &m_pGrid->GetGridView();
	CXTPPropertyGridItem* pItem = (CXTPPropertyGridItem*)lpDrawItemStruct->itemData;

	ASSERT(pItem != NULL);
	if (!pItem)
		return;

	CXTPWindowRect rcWindow(pView);
	CRect rc = lpDrawItemStruct->rcItem;

	CXTPBufferDC dc(lpDrawItemStruct->hDC, rc);
	CXTPFontDC font(&dc, GetItemFont(pItem, FALSE));

	dc.FillSolidRect(rc, GetItemBackColor(pItem, FALSE));
	dc.SetBkMode(TRANSPARENT);


	CRect rcCaption(rc.left, rc.top, rc.left + pView->GetDividerPos(), rc.bottom);

	COLORREF clrFore = m_pMetrics->m_clrFore;
	COLORREF clrShadow = GetXtremeColor(COLOR_3DSHADOW);
	COLORREF clrLight = GetXtremeColor(COLOR_BTNHIGHLIGHT);

	{
		if (!pItem->IsCategory())
		{
			CXTPFontDC fontValue(&dc, GetItemFont(pItem, TRUE));

			CRect rcValue(rcCaption.right, rc.top, rc.right, rc.bottom - 1);
			DrawItemValue(&dc, pItem, rcValue);
		}

		dc.SetTextColor(GetItemTextColor(pItem, FALSE));

		dc.Draw3dRect(rcCaption.right - 1, rc.top, 1, rc.Height(), clrShadow, clrShadow);
		dc.Draw3dRect(rcCaption.right, rc.top, 1, rc.Height(), clrLight, clrLight);

		BOOL bSelected = lpDrawItemStruct->itemState & ODS_SELECTED;
		BOOL bFocused = bSelected && CWnd::GetFocus() && ((CWnd::GetFocus() == pView) || (CWnd::GetFocus()->GetParent() == pView) || (CWnd::GetFocus()->GetOwner() == pView));
		if (bFocused || (bSelected && !m_pGrid->m_bHideSelection))
		{
			dc.Draw3dRect(rc, clrFore, clrLight);
		}
		else
		{
			for (int i = 0; i < rc.right; i += 2)
				dc.SetPixel(i, rc.bottom - 1, clrShadow);
		}


		DrawItemCaption(&dc, pItem, rcCaption);

		CRect rcValue(rcCaption.right, rc.top, rc.right, rc.bottom - 1);
		DrawInplaceButtons(&dc, lpDrawItemStruct, pItem, rcValue);
	}

	if (pItem->HasVisibleChilds())
	{
		CRgn rgn;
		rgn.CreateRectRgnIndirect(&rcCaption);
		dc.SelectClipRgn(&rgn);

		DrawExpandButton(dc, pItem, rcCaption);

		dc.SelectClipRgn(NULL);
	}
}

void CXTPPropertyGridDelphiTheme::RefreshMetrics()
{
	CXTPPropertyGridPaintManager::RefreshMetrics();

	m_pMetrics->m_clrBack.SetStandardValue(GetXtremeColor(COLOR_3DFACE));
	m_pMetrics->m_clrFore.SetStandardValue(RGB(0, 0, 128));

}

//////////////////////////////////////////////////////////////////////////
// CXTPPropertyGridWhidbeyTheme

CXTPPropertyGridOfficeXPTheme::CXTPPropertyGridOfficeXPTheme(CXTPPropertyGrid* pGrid)
	: CXTPPropertyGridPaintManager(pGrid)
{
	m_buttonsStyle = xtpGridButtonsOfficeXP;
}

void CXTPPropertyGridOfficeXPTheme::RefreshMetrics()
{
	CXTPPropertyGridPaintManager::RefreshMetrics();

	m_pMetrics->m_clrCategoryFore.SetStandardValue(GetXtremeColor(COLOR_BTNTEXT));
	m_pMetrics->m_clrLine.SetStandardValue(GetXtremeColor(XPCOLOR_TOOLBAR_FACE));
}

//////////////////////////////////////////////////////////////////////////
// CXTPPropertyGridWhidbeyTheme

CXTPPropertyGridWhidbeyTheme::CXTPPropertyGridWhidbeyTheme(CXTPPropertyGrid* pGrid)
	: CXTPPropertyGridPaintManager(pGrid)
{
	m_buttonsStyle = xtpGridButtonsThemed;
}

void CXTPPropertyGridWhidbeyTheme::DrawCategoryCaptionBackground(CDC* pDC, CXTPPropertyGridItem* /*pItem*/, CRect rc)
{
	pDC->FillSolidRect(rc, m_pMetrics->m_clrLine);

	XTPDrawHelpers()->GradientFill(pDC,
		CRect(rc.left, rc.bottom - 2, rc.right, rc.bottom - 1), m_pMetrics->m_clrLine, GetXtremeColor(COLOR_3DHIGHLIGHT), TRUE);

}

void CXTPPropertyGridWhidbeyTheme::RefreshMetrics()
{
	CXTPPropertyGridPaintManager::RefreshMetrics();

	m_pMetrics->m_clrLine.SetStandardValue(GetXtremeColor(COLOR_3DFACE));
	m_pMetrics->m_clrCategoryFore.SetStandardValue(GetXtremeColor(COLOR_BTNTEXT));
	m_clrShadow.SetStandardValue(GetXtremeColor(XPCOLOR_STATICFRAME));
}

void CXTPPropertyGridWhidbeyTheme::DrawExpandButton(CDC& dc, CXTPPropertyGridItem* pItem, CRect rcCaption)
{
	if (!pItem->IsExpandable())
		return;

	CRect rcButton(CPoint(XTP_PGI_EXPAND_BORDER / 2 - 5, rcCaption.CenterPoint().y - 4), CSize(9, 9));

	if (pItem->GetIndent() > 0)
		rcButton.OffsetRect((pItem->GetIndent() - (pItem->IsCategory() ? 0 : 1)) * XTP_PGI_EXPAND_BORDER, 0);

	CDC* pDC = &dc;

	COLORREF clrButton = GetXtremeColor(COLOR_3DFACE);
	COLORREF clrFrame = GetXtremeColor(COLOR_3DSHADOW);
	pDC->FillSolidRect(rcButton.left + 1, rcButton.top + 1, 7, 7, clrButton);
	pDC->FillSolidRect(rcButton.left + 1, rcButton.top, 9 - 2, 1, clrFrame);
	pDC->FillSolidRect(rcButton.left + 1, rcButton.bottom - 1, 9 - 2, 1, clrFrame);
	pDC->FillSolidRect(rcButton.left, rcButton.top + 1, 1, 9 - 2, clrFrame);
	pDC->FillSolidRect(rcButton.right - 1, rcButton.top + 1, 1, 9 - 2, clrFrame);
	pDC->FillSolidRect(rcButton.left + 1, rcButton.top + 1, 9 - 2, 3, 0xFFFFFF);
	pDC->FillSolidRect(rcButton.left + 1, rcButton.top + 4, 9 - 4, 2, 0xFFFFFF);

	pDC->FillSolidRect(rcButton.left + 2, rcButton.top + 4, 9 - 4, 1, 0);
	if (!pItem->IsExpanded())
		pDC->FillSolidRect(rcButton.left + 4, rcButton.top + 2, 1, 9 - 4, 0);
}


CXTPPropertyGridVisualStudio2010Theme::CXTPPropertyGridVisualStudio2010Theme(CXTPPropertyGrid* pGrid)
	: CXTPPropertyGridPaintManager(pGrid)
{
	m_buttonsStyle = xtpGridButtonsThemed;

	m_themeTreeExplorer = new CXTPWinThemeWrapper();
}

CXTPPropertyGridVisualStudio2010Theme::~CXTPPropertyGridVisualStudio2010Theme()
{
	SAFE_DELETE(m_themeTreeExplorer);
}


void CXTPPropertyGridVisualStudio2010Theme::RefreshMetrics()
{
	CXTPPropertyGridPaintManager::RefreshMetrics();

	m_pMetrics->m_clrLine.SetStandardValue(RGB(240, 240, 240));
	m_pMetrics->m_clrCategoryFore.SetStandardValue(RGB(168, 179, 194));
	m_clrShadow.SetStandardValue(GetSysColor(COLOR_3DSHADOW));
	m_clrCategorySeparator.SetStandardValue(GetSysColor(COLOR_3DFACE));
	m_clrFace.SetStandardValue(RGB(222, 225, 231));
	m_pMetrics->m_clrHelpBack.SetStandardValue(RGB(222, 225, 231));
	m_pMetrics->m_clrHelpFore.SetStandardValue(0);

	HWND hWnd = m_pGrid->m_hWnd;
	if (hWnd && XTPSystemVersion()->IsWinVistaOrGreater())
	{
		CXTPWinThemeWrapper().SetWindowTheme(hWnd, L"EXPLORER", NULL);

		m_themeTreeExplorer->OpenThemeData(hWnd, L"TREEVIEW");

		CXTPWinThemeWrapper().SetWindowTheme(hWnd, NULL, NULL);
	}
	else
	{
		m_themeTreeExplorer->CloseTheme();
	}
}

void CXTPPropertyGridVisualStudio2010Theme::DrawExpandButton(CDC& dc, CXTPPropertyGridItem* pItem, CRect rcCaption)
{
	if (!pItem->IsExpandable())
		return;


	if (m_buttonsStyle == xtpGridButtonsThemed && m_themeTreeExplorer->IsAppThemed())
	{
		CRect rcSign(CPoint(XTP_PGI_EXPAND_BORDER / 2 - 9, rcCaption.CenterPoint().y - 8), CSize(16, 16));

		if (pItem->GetIndent() > 0)
			rcSign.OffsetRect((pItem->GetIndent() - (pItem->IsCategory() ? 0 : 1)) * XTP_PGI_EXPAND_BORDER, 0);

		m_themeTreeExplorer->DrawThemeBackground(dc, TVP_GLYPH, pItem->IsExpanded() ? GLPS_OPENED : GLPS_CLOSED, &rcSign, 0);

	}
	else
	{
		CRect rcSign(CPoint(XTP_PGI_EXPAND_BORDER / 2 - 5, rcCaption.CenterPoint().y - 4), CSize(9, 9));

		if (pItem->GetIndent() > 0)
			rcSign.OffsetRect((pItem->GetIndent() - (pItem->IsCategory() ? 0 : 1)) * XTP_PGI_EXPAND_BORDER, 0);


		CPoint pt = rcSign.CenterPoint();

		if (!pItem->IsCategory()) dc.FillSolidRect(rcSign, m_pMetrics->m_clrBack);
		dc.Draw3dRect(rcSign, m_pMetrics->m_clrFore, m_pMetrics->m_clrFore);

		CXTPPenDC pen (dc, m_pMetrics->m_clrFore);

		dc.MoveTo(pt.x - 2, pt.y);
		dc.LineTo(pt.x + 3, pt.y);

		if (!pItem->IsExpanded())
		{
			dc.MoveTo(pt.x, pt.y - 2);
			dc.LineTo(pt.x, pt.y + 3);
		}
	}
}


