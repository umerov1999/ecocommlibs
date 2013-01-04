// XTPControlGallery.cpp : implementation file.
//
// This file is a part of the XTREME COMMANDBARS MFC class library.
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
#include <math.h>

#include "Common/XTPImageManager.h"
#include "Common/XTPPropExchange.h"
#include "Common/XTPToolTipContext.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPHookManager.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPMarkupRender.h"

#include "XTPCommandBarsDefines.h"
#include "XTPCommandBar.h"
#include "XTPPopupBar.h"
#include "XTPControl.h"
#include "XTPControlButton.h"
#include "XTPControlPopup.h"
#include "XTPControlEdit.h"
#include "XTPControlComboBox.h"
#include "XTPScrollBase.h"

#include "XTPControls.h"
#include "XTPControlGallery.h"
#include "XTPControlComboBoxGalleryPopupBar.h"
#include "XTPPaintManager.h"
#include "XTPCommandBars.h"

#define IDSYS_ANIMATE 23346

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CXTPControlGalleryItem

CXTPControlGalleryItem::CXTPControlGalleryItem()
{
	m_pItems = NULL;

	m_nId = 0;
	m_nIndex = -1;
	m_nImage = -1;
	m_dwData = 0;
	m_bLabel = FALSE;
	m_bEnabled = TRUE;

	m_szItem = CSize(0, 0);

	m_nImageWidth = ICON_SMALL;

	m_pMarkupUIElement = NULL;

}

CXTPControlGalleryItem::~CXTPControlGalleryItem()
{
	XTPMarkupReleaseElement(m_pMarkupUIElement);
}

void CXTPControlGalleryItem::SetEnabled(BOOL bEnabled)
{
	if (m_bEnabled == bEnabled)
		return;

	m_bEnabled = bEnabled;

	if (m_pItems && m_pItems->GetAction())
		m_pItems->GetAction()->RedrawControls();
}

void CXTPControlGalleryItem::SetCaption(LPCTSTR lpszCaption)
{
	m_strCaption = lpszCaption;

	XTPMarkupReleaseElement(m_pMarkupUIElement);

	if (m_pItems && m_pItems->GetMarkupContext())
	{
		m_pMarkupUIElement = XTPMarkupParseText(m_pItems->GetMarkupContext(), lpszCaption);
	}
}

BOOL CXTPControlGalleryItem::GetEnabled() const
{
	return m_bEnabled;
}


int CXTPControlGalleryItem::GetIndex() const
{
	return m_nIndex;
}

CSize CXTPControlGalleryItem::GetSize()
{
	if (m_bLabel)
		return CSize(0, m_szItem.cy == 0 ? 19 : m_szItem.cy);

	return CSize(m_szItem.cx == 0 ? m_pItems->m_szItem.cx : m_szItem.cx,
		m_szItem.cy == 0 ? m_pItems->m_szItem.cy : m_szItem.cy);
}

void CXTPControlGalleryItem::SetSize(CSize szItem)
{
	m_szItem = szItem;
}

void CXTPControlGalleryItem::Draw(CDC* pDC, CXTPControlGallery* pGallery, CRect rcItem, BOOL bEnabled, BOOL bSelected, BOOL bPressed, BOOL bChecked)
{
	if (IsLabel())
	{
		pGallery->GetGalleryPaintManager()->
			DrawLabel(pDC, this, rcItem);
		return;
	}

	CXTPPaintManager* pPaintManager = pGallery->GetPaintManager();

	if (m_pMarkupUIElement)
	{
		BOOL bListStyle = this->GetSize().cx == 0;

		COLORREF clrTextColor;

		if (bListStyle)
		{
			pPaintManager->DrawRectangle(pDC, rcItem, bSelected, bPressed, bEnabled, bChecked, FALSE, xtpBarTypePopup, xtpBarPopup);
			clrTextColor = pPaintManager->GetRectangleTextColor(bSelected, FALSE, bEnabled, bChecked, FALSE, xtpBarTypePopup, xtpBarPopup);
		}
		else
		{
			pPaintManager->DrawRectangle(pDC, rcItem, bSelected, bPressed, bEnabled, bChecked, FALSE, xtpBarTypeNormal, xtpBarPopup);
			clrTextColor = pPaintManager->GetRectangleTextColor(bSelected, bPressed, bEnabled, bChecked, FALSE, xtpBarTypeNormal, xtpBarPopup);
		}

		XTPMarkupSetDefaultFont(XTPMarkupElementContext(m_pMarkupUIElement),
			(HFONT)pPaintManager->GetRegularFont()->GetSafeHandle(), clrTextColor);

		CRect rcDest(rcItem);
		rcDest.DeflateRect(3, 3);

		XTPMarkupRenderElement(m_pMarkupUIElement, pDC->GetSafeHdc(), rcDest);

		return;
	}

	CXTPImageManagerIcon* pImage = GetImage();

	BOOL bDrawImgeAndText = (pImage != NULL && !m_strCaption.IsEmpty()) &&
		(pGallery->GetStyle() == xtpButtonIconAndCaption || pGallery->GetStyle() == xtpButtonCaptionAndDescription || pGallery->GetStyle() == xtpButtonIconAndCaptionBelow);

	if (bDrawImgeAndText && pGallery->GetStyle() == xtpButtonIconAndCaptionBelow)
	{
		pPaintManager->DrawRectangle(pDC, rcItem, bSelected, bPressed, bEnabled, bChecked, FALSE, xtpBarTypeNormal, xtpBarPopup);
		CSize szImage(pImage->GetWidth(), pImage->GetHeight());
		CPoint pt((rcItem.left + rcItem.right - szImage.cx) / 2, rcItem.top + 2);

		pImage->Draw(pDC, pt, pImage->GetIcon(bEnabled ? xtpImageNormal : xtpImageDisabled), szImage);

		COLORREF clrTextColor = pPaintManager->GetRectangleTextColor(bSelected, FALSE, bEnabled, FALSE, FALSE, xtpBarTypePopup, xtpBarPopup);

		pDC->SetTextColor(clrTextColor);

		CRect rcText(rcItem);
		rcText.top += 2 + szImage.cy + 2;

		CXTPFontDC font(pDC, pPaintManager->GetRegularFont());
		pDC->DrawText(m_strCaption, rcText, DT_WORDBREAK | DT_CENTER | DT_NOPREFIX);

	}
	else if (bDrawImgeAndText)
	{
		pPaintManager->DrawRectangle(pDC, rcItem, bSelected, FALSE, bEnabled, FALSE, FALSE, xtpBarTypePopup, xtpBarPopup);

		CSize szImage(pImage->GetWidth(), pImage->GetHeight());
		int nImageBoxWidth  = max(rcItem.Height() - 4, szImage.cx);

		if (bChecked)
		{
			CRect rcCheckBox(rcItem.left + 2, rcItem.top + 2, rcItem.left + 2 + nImageBoxWidth, rcItem.bottom - 2);
			pPaintManager->DrawRectangle(pDC, rcCheckBox, FALSE, FALSE, bEnabled, TRUE, FALSE, xtpBarTypePopup, xtpBarPopup);
		}

		CPoint pt(rcItem.left + 2 + (nImageBoxWidth - szImage.cx) / 2, (rcItem.top + rcItem.bottom - szImage.cy) / 2);

		pImage->Draw(pDC, pt, pImage->GetIcon(bEnabled ? xtpImageNormal : xtpImageDisabled), szImage);

		COLORREF clrTextColor = pPaintManager->GetRectangleTextColor(bSelected, FALSE, bEnabled, FALSE, FALSE, xtpBarTypePopup, xtpBarPopup);

		pDC->SetTextColor(clrTextColor);

		CRect rcText(rcItem);
		rcText.left += 2 + nImageBoxWidth + szImage.cx / 3;

		if (pGallery->GetStyle() == xtpButtonCaptionAndDescription)
		{
			CXTPFontDC font(pDC, pPaintManager->GetRegularBoldFont());
			pDC->DrawText(m_strCaption, rcText, DT_END_ELLIPSIS | DT_SINGLELINE | DT_TOP | DT_NOPREFIX);

			rcText.top += pDC->GetTextExtent(_T("XXX"), 3).cy;
			font.SetFont(pPaintManager->GetRegularFont());
			pDC->DrawText(m_strDescriptionText, rcText, DT_WORDBREAK | DT_TOP | DT_NOPREFIX);
		}
		else
		{
			CXTPFontDC font(pDC, pPaintManager->GetRegularFont());
			pDC->DrawText(m_strCaption, rcText, DT_END_ELLIPSIS | DT_SINGLELINE | DT_VCENTER | DT_NOPREFIX);
		}
	}
	else if (pImage)
	{
		pPaintManager->DrawRectangle(pDC, rcItem, bSelected, bPressed, bEnabled, bChecked, FALSE, xtpBarTypeNormal, xtpBarPopup);
		CSize szImage(pImage->GetWidth(), pImage->GetHeight());
		CPoint pt((rcItem.left + rcItem.right - szImage.cx) / 2, (rcItem.top + rcItem.bottom - szImage.cy) / 2);

		pImage->Draw(pDC, pt, pImage->GetIcon(bEnabled ? xtpImageNormal : xtpImageDisabled), szImage);
	}
	else
	{
		pPaintManager->DrawRectangle(pDC, rcItem, bSelected, FALSE, bEnabled, bChecked, FALSE, xtpBarTypePopup, xtpBarPopup);
		COLORREF clrTextColor = pPaintManager->GetRectangleTextColor(bSelected, FALSE, bEnabled, bChecked, FALSE, xtpBarTypePopup, xtpBarPopup);

		pDC->SetTextColor(clrTextColor);
		CXTPFontDC font(pDC, pPaintManager->GetRegularFont());

		CRect rcText(rcItem);
		rcText.left += 3;

		pDC->DrawText(m_strCaption, rcText, DT_END_ELLIPSIS | DT_SINGLELINE | DT_VCENTER | DT_NOPREFIX);
	}
}

CXTPImageManagerIcon* CXTPControlGalleryItem::GetImage()
{
	if (!m_pItems)
		return NULL;

	return m_pItems->GetImageManager()->GetImage(m_nImage >= 0 ? m_nImage : m_nId, m_nImageWidth);
}


//////////////////////////////////////////////////////////////////////////
// CXTPControlGalleryItems

IMPLEMENT_DYNAMIC(CXTPControlGalleryItems, CCmdTarget)

CXTPControlGalleryItems::CXTPControlGalleryItems(CXTPControlAction* pAction)
	: m_pAction(pAction)
{
	if (pAction)
	{
		ASSERT(m_pAction->m_pTarget == NULL);
		m_pAction->m_pTarget = this;
	}

	m_szItem = CSize(0, 20);
	m_pImageManager = new CXTPImageManager();
	m_pImageManager->DrawReverted(2);
	m_bClipItems = TRUE;

	m_pMarkupContext = NULL;

}

CXTPControlGalleryItems::~CXTPControlGalleryItems()
{
	m_pAction = NULL;

	RemoveAll();
	CMDTARGET_RELEASE(m_pImageManager);

	XTPMarkupReleaseContext(m_pMarkupContext);

}


void CXTPControlGalleryItems::EnableMarkup(BOOL bEnableMarkup)
{
	XTPMarkupReleaseContext(m_pMarkupContext);

	if (bEnableMarkup)
	{
		m_pMarkupContext = XTPMarkupCreateContext();
	}
}



void CXTPControlGalleryItems::RemoveAll()
{
	for (int i = 0; i < m_arrItems.GetSize(); i++)
	{
		m_arrItems[i]->InternalRelease();
	}

	m_arrItems.RemoveAll();
	OnItemsChanged();
}

void CXTPControlGalleryItems::Remove(int nIndex)
{
	CXTPControlGalleryItem* pItem = GetItem(nIndex);
	if (!pItem)
		return;

	m_arrItems.RemoveAt(nIndex);
	pItem->InternalRelease();

	UpdateIndexes(nIndex);
	OnItemsChanged();
}

void CXTPControlGalleryItems::UpdateIndexes(int nStart /*= 0*/)
{
	for (int i = nStart; i < GetItemCount(); i++)
		GetItem(i)->m_nIndex = i;
}

CXTPControlGalleryItem* CXTPControlGalleryItems::InsertItem(int nIndex, CXTPControlGalleryItem* pItem, int nId, int nImage)
{
	if (!pItem)
		return NULL;

	if (nIndex < 0 || nIndex > GetItemCount())
		nIndex = GetItemCount();

	m_arrItems.InsertAt(nIndex, pItem);
	UpdateIndexes(nIndex);

	pItem->m_pItems = this;
	pItem->m_nImage = nImage;

	if (nId != -1)
	{
		pItem->m_nId = nId;

		CString strCaption;
		if (nId > 0 && pItem->GetCaption().IsEmpty() && strCaption.LoadString(nId))
		{
			pItem->SetCaption(strCaption);

			if (pItem->GetMarkupUIElement() == NULL)
			{
				pItem->SetToolTip(strCaption);
			}
		}
	}

	OnItemsChanged();

	return pItem;
}

void CXTPControlGalleryItems::OnItemsChanged()
{
	if (!m_pAction)
		return;

	for (int i = 0; i < m_pAction->GetCount(); i++)
	{
		CXTPControlGallery* pGallery = DYNAMIC_DOWNCAST(CXTPControlGallery, m_pAction->GetControl(i));
		if (pGallery)
		{
			pGallery->OnItemsChanged();
		}
	}
}

void CXTPControlGallery::OnItemsChanged()
{
	m_nSelected = -1;
	m_nScrollPos = 0;
	Reposition();

	if (m_bPreview)
	{
		m_bPreview = FALSE;
		NotifySite(XTP_GN_PREVIEWCANCEL);
	}
}

CXTPControlGalleryItem* CXTPControlGalleryItems::AddItem(CXTPControlGalleryItem* pItem, int nId, int nImage)
{
	return InsertItem(GetItemCount(), pItem, nId, nImage);
}

CXTPControlGalleryItem* CXTPControlGalleryItems::AddItem(int nId, int nImage)
{
	return AddItem(new CXTPControlGalleryItem(), nId, nImage);
}

CXTPControlGalleryItem* CXTPControlGalleryItems::AddItem(LPCTSTR lpszCaption, int nImage)
{
	CXTPControlGalleryItem* pItem = AddItem(new CXTPControlGalleryItem(), -1, nImage);
	pItem->SetCaption(lpszCaption);

	return pItem;
}

CXTPControlGalleryItem* CXTPControlGalleryItems::AddItem(LPCTSTR lpszCaption, int nId, int nImage)
{
	CXTPControlGalleryItem* pItem = AddItem(nId, nImage);
	pItem->SetCaption(lpszCaption);

	return pItem;
}

CXTPControlGalleryItem* CXTPControlGalleryItems::AddLabel(int nId)
{
	CXTPControlGalleryItem* pItem = AddItem(new CXTPControlGalleryItem(), nId);
	pItem->m_bLabel = TRUE;

	return pItem;
}

CXTPControlGalleryItem* CXTPControlGalleryItems::AddLabel(LPCTSTR lpszCaption)
{
	CXTPControlGalleryItem* pItem = AddItem(new CXTPControlGalleryItem());
	pItem->SetCaption(lpszCaption);
	pItem->m_bLabel = TRUE;

	return pItem;
}

int CXTPControlGalleryItems::GetItemCount() const
{
	return (int)m_arrItems.GetSize();
}

CXTPControlGalleryItem* CXTPControlGalleryItems::GetItem(int nIndex) const
{
	return nIndex >= 0 && nIndex < GetItemCount() ? m_arrItems.GetAt(nIndex) : NULL;
}

void CXTPControlGalleryItems::SetImageManager(CXTPImageManager* pImageManager)
{
	CMDTARGET_RELEASE(m_pImageManager);

	m_pImageManager = pImageManager;
}

BOOL CXTPControlGalleryItems::PreDrawItem(CDC* pDC, CXTPControlGallery* pGallery, CXTPControlGalleryItem* pItem, CRect rcItem,
	BOOL bEnabled, BOOL bSelected, BOOL bPressed, BOOL bChecked)
{
	UNREFERENCED_PARAMETER(pDC);
	UNREFERENCED_PARAMETER(pGallery);
	UNREFERENCED_PARAMETER(pItem);
	UNREFERENCED_PARAMETER(rcItem);
	UNREFERENCED_PARAMETER(bEnabled);
	UNREFERENCED_PARAMETER(bSelected);
	UNREFERENCED_PARAMETER(bPressed);
	UNREFERENCED_PARAMETER(bChecked);


	return FALSE;
}


//////////////////////////////////////////////////////////////////////////
// CXTPControlGallery

IMPLEMENT_XTP_CONTROL(CXTPControlGallery, CXTPControlPopup)


CXTPControlGallery::CXTPControlGallery()
{
	m_nChecked = -1;
	m_nSelected = -1;
	m_bPressed = FALSE;
	m_bKeyboardSelected = FALSE;
	m_bHideSelection = FALSE;

	m_nScrollPos = 0;
	m_nTotalHeight = 0;
	m_bShowLabels = TRUE;

	m_nWidth = 120;
	m_nHeight = 60;

	m_rcMargin.SetRectEmpty();
	m_ptPressed = CPoint(0, 0);

	m_bShowScrollBar = TRUE;


	m_bShowBorders = FALSE;
	m_bShowShadow = FALSE;

	m_bPreview = FALSE;
	m_nResizable = 0;

	m_controlType = xtpControlGallery;

	m_bAnimation = FALSE;
	m_nScrollPosTarget = 0;
	m_dAnimationStep = 0;

	m_bScrollBarAuto = FALSE;

	m_bTransparent = FALSE;


	SetFlags(xtpFlagShowPopupBarTip);
}

CXTPControlGalleryItems* CXTPControlGalleryItems::CreateItems(CXTPCommandBars* pCommandBars, int nId)
{
	CXTPControlAction* pAction = pCommandBars->CreateAction(nId);

	CXTPControlGalleryItems* pItems = NULL;

	if (pAction->m_pTarget == NULL)
	{
		pItems = new CXTPControlGalleryItems(pAction);
	}
	else
	{
		pItems = DYNAMIC_DOWNCAST(CXTPControlGalleryItems, pAction->m_pTarget);
		ASSERT(pItems);
	}

	return pItems;
}

CXTPControlGalleryItems* CXTPControlGallery::GetItems() const
{
	CXTPControlAction* pAction = GetAction();
	if (!pAction)
		return NULL;

	return (CXTPControlGalleryItems*)pAction->m_pTarget;
}

CXTPControlGallery::~CXTPControlGallery()
{
}

int CXTPControlGallery::GetCustomizeMinWidth() const
{
	if (!m_pParent || !GetItems())
		return 0;

	if (DYNAMIC_DOWNCAST(CXTPPopupBar, m_pParent))
	{
		CXTPControl* pParentControl = ((CXTPPopupBar*)m_pParent)->GetControlPopup();
		if (pParentControl && pParentControl->GetType() == xtpControlGallery)
			return pParentControl->GetRect().Width();
	}

	CRect rcBorders(GetBorders());
	return max(16, GetItems()->m_szItem.cx) + rcBorders.left + rcBorders.right;
}

int CXTPControlGallery::GetCustomizeMinHeight() const
{
	if (!m_pParent || !GetItems())
		return 0;

	CRect rcBorders(GetBorders());
	int nHeight = GetItems()->m_szItem.cy + rcBorders.top + rcBorders.bottom;
	return max(nHeight, 32);
}

void CXTPControlGallery::SetAction(CXTPControlAction* pAction)
{
	CXTPControlPopup::SetAction(pAction);

	if (pAction) Reposition();
}

BOOL CXTPControlGallery::IsShowAsButton() const
{
	return m_buttonRibbonStyle == xtpButtonIconAndCaptionBelow || GetStyle() == xtpButtonIcon;
}

void CXTPControlGallery::SetItems(CXTPControlGalleryItems* pItems)
{
	if (pItems)
	{
		SetAction(pItems->m_pAction);
	}
}

void CXTPControlGallery::RepositionCore(BOOL bCheckScroll)
{
	CRect rcItems = GetItemsRect();

	int x = rcItems.left;
	int y = rcItems.top;
	BOOL bFirstItem = TRUE;

	int nCount = GetItemCount();

	m_arrRects.SetSize(nCount);
	int nRowHeight = 0;

	for (int i = 0 ; i < nCount; i++)
	{
		CXTPControlGalleryItem* pItem = GetItem(i);
		m_arrRects[i].pItem = pItem;
		m_arrRects[i].bBeginRow = FALSE;

		if (pItem->IsLabel())
		{
			if (!m_bShowLabels)
			{
				m_arrRects[i].rcItem = CRect(0, 0, 0, 0);
				continue;
			}

			y += nRowHeight;

			CSize szItem = pItem->GetSize();

			CRect rcItem(rcItems.left, y, rcItems.right, y + szItem.cy);
			m_arrRects[i].rcItem = rcItem;
			m_arrRects[i].bBeginRow = TRUE;

			y += rcItem.Height() + 1;
			nRowHeight = 0;
			x = rcItems.left;
			bFirstItem = TRUE;

		}
		else
		{
			CSize szItem = pItem->GetSize();
			if (szItem.cx == 0)
				szItem.cx = rcItems.Width();

			if (bFirstItem)
			{
				m_arrRects[i].bBeginRow = TRUE;
			}

			if (!bFirstItem && x + szItem.cx > rcItems.right)
			{
				y += nRowHeight;
				x = rcItems.left;
				m_arrRects[i].bBeginRow = TRUE;
				nRowHeight = 0;
			}

			CRect rcItem(x, y, x + szItem.cx, y + szItem.cy);
			m_arrRects[i].rcItem = rcItem;

			nRowHeight = max(nRowHeight, szItem.cy);

			bFirstItem = FALSE;
			x += szItem.cx;
		}

		if (bCheckScroll && y + nRowHeight + rcItems.top > rcItems.Height())
		{
			break;
		}
	}

	y += nRowHeight;

	m_nTotalHeight = y - rcItems.top;
}

void CXTPControlGallery::Reposition()
{
	if (GetParent() == NULL)
		return;

	if (IsShowAsButton())
		return;

	if (m_bScrollBarAuto)
	{
		m_bShowScrollBar = FALSE;
		RepositionCore(TRUE);

		if (m_nTotalHeight > GetItemsRect().Height())
		{
			m_bShowScrollBar = TRUE;
			RepositionCore(FALSE);
		}
	}
	else
	{
		RepositionCore(FALSE);
	}

	CRect rcItems = GetItemsRect();

	if (m_nScrollPos > m_nTotalHeight - rcItems.Height())
		m_nScrollPos = max(0, m_nTotalHeight - rcItems.Height());

	SetupScrollInfo();
}

void CXTPControlGallery::SetCheckedItem(int nId)
{
	if (m_nChecked != nId)
	{
		m_nChecked = nId;
		InvalidateItems();
	}
}

int CXTPControlGallery::GetCheckedItem() const
{
	return m_nChecked;
}

int CXTPControlGallery::GetSelectedItem() const
{
	return m_nSelected;
}

int CXTPControlGallery::GetItemCount() const
{
	CXTPControlGalleryItems* pItems = GetItems();

	return pItems ? pItems->GetItemCount() : 0;
}

CXTPControlGalleryItem* CXTPControlGallery::GetItem(int nIndex) const
{
	CXTPControlGalleryItems* pItems = GetItems();

	return pItems ? pItems->GetItem(nIndex) : NULL;
}

void CXTPControlGallery::DrawItems(CDC* pDC)
{
	CXTPControlGalleryItems* pItems = GetItems();
	if (!pItems)
		return;

	ASSERT(m_arrRects.GetSize() == pItems->GetItemCount());

	CRect rcItems = GetItemsRect();

	CRgn rgn;
	CPoint point = pDC->GetViewportOrg();
	CRect rcClipBox(rcItems);
	rcClipBox.OffsetRect(point);

	rgn.CreateRectRgnIndirect(rcClipBox);
	pDC->SelectClipRgn(&rgn);

	int nSelected = m_bHideSelection ? -1 : m_nSelected;

	for (int i = 0; i < m_arrRects.GetSize(); i++)
	{
		const GALLERYITEM_POSITION& pos = m_arrRects[i];

		CRect rcItem = pos.rcItem;
		rcItem.OffsetRect(0, -m_nScrollPos);

		if (rcItem.bottom < rcItems.top)
			continue;

		BOOL bEnabled = pos.pItem->GetEnabled() && GetEnabled();

		if (!rcItem.IsRectEmpty())
		{
			if (!pItems->PreDrawItem(pDC, this, pos.pItem, rcItem, bEnabled, nSelected == i, nSelected == i && m_bPressed, IsItemChecked(pos.pItem)))
			{
				pos.pItem->Draw(pDC, this, rcItem, bEnabled, nSelected == i, nSelected == i && m_bPressed, IsItemChecked(pos.pItem));
			}
		}

		if (rcItem.top > rcItems.bottom)
			break;
	}

	pDC->SelectClipRgn(NULL);
}

BOOL CXTPControlGallery::IsItemChecked(CXTPControlGalleryItem* pItem) const
{
	return m_nChecked == pItem->GetID();
}

int CXTPControlGallery::HitTestItem(CPoint point, LPRECT lpRect) const
{
	if (!GetEnabled())
		return -1;

	if (IsShowAsButton())
		return -1;

	ASSERT(m_arrRects.GetSize() == GetItemCount());

	CRect rcItems = GetItemsRect();

	for (int i = 0; i < m_arrRects.GetSize(); i++)
	{
		const GALLERYITEM_POSITION& pos = m_arrRects[i];

		CRect rcItem = pos.rcItem;
		rcItem.OffsetRect(0, -m_nScrollPos);

		if (!pos.pItem->GetEnabled())
			continue;

		if (rcItem.top >= rcItems.bottom)
			break;

		if (rcItem.PtInRect(point))
		{
			if (lpRect)
			{
				*lpRect = rcItem;
			}
			return pos.pItem->IsLabel() ? -1 : i;
		}
	}

	return -1;
}

void CXTPControlGallery::InvalidateItems(LPCRECT lpRect, BOOL bAnimate)
{
	GetParent()->Redraw(lpRect ? lpRect : GetRect(), bAnimate);
}

void CXTPControlGallery::OnExecute()
{
	if (m_bPreview)
	{
		NotifySite(XTP_GN_PREVIEWAPPLY);
		m_bPreview = FALSE;
	}

	CXTPControlGalleryItem* pItem = GetItem(GetSelectedItem());
	if (pItem)
	{
		if (pItem->OnClick())
			return;
	}

	CXTPControlPopup::OnExecute();
}

BOOL CXTPControlGallery::OnSetPopup(BOOL bPopup)
{
	if (m_bPreview)
	{
		NotifySite(XTP_GN_PREVIEWCANCEL);
		m_bPreview = FALSE;
	}

	return CXTPControlPopup::OnSetPopup(bPopup);
}


void CXTPControlGallery::OnMouseHover()
{
	if (IsItemSelected() && !m_bPreview && GetSelected())
	{
		m_bPreview = TRUE;
		NotifySite(XTP_GN_PREVIEWSTART);

		NotifySite(XTP_GN_PREVIEWCHANGE);
	}
	if (!m_pParent->m_bTrackOnHover)
		return;

	CXTPControlPopup::OnMouseHover();
}

void CXTPControlGallery::OnCalcDynamicSize(DWORD dwMode)
{
	if (DYNAMIC_DOWNCAST(CXTPControlComboBoxGalleryPopupBar, GetParent()) == 0)
	{
		m_nSelected = -1;
	}
	m_bHideSelection = FALSE;
	m_bPressed = FALSE;

	CXTPControlPopup::OnCalcDynamicSize(dwMode);
}

void CXTPControlGallery::PerformMouseMove(CPoint point)
{
	int nItem = HitTestItem(point);

	if (nItem == -1 && m_bKeyboardSelected)
		return;

	if (nItem == -1 && point == CPoint(-1, -1) && m_nSelected != -1)
	{
		HideSelection();
		return;
	}

	int nSelected = m_nSelected;

	if (nItem != m_nSelected || m_bHideSelection || m_bKeyboardSelected)
	{
		m_nSelected = nItem;
		m_bHideSelection = FALSE;
		m_bKeyboardSelected = FALSE;
		m_bPressed = FALSE;

		CXTPControlGalleryItems* pItems = GetItems();

		if (pItems && pItems->m_bClipItems)
		{
			if (nSelected != -1)
			{
				InvalidateItems(GetItemDrawRect(nSelected), TRUE);
			}

			if (m_nSelected != -1)
			{
				InvalidateItems(GetItemDrawRect(m_nSelected), FALSE);
			}
		}
		else
		{
			InvalidateItems();
		}

		if (!m_bPreview && m_nSelected != -1)
		{
			m_pParent->SetTimer(XTP_TID_HOVER, 200, NULL);
		}

		if (m_bPreview)
		{
			NotifySite(XTP_GN_PREVIEWCHANGE);
		}

		if (m_nSelected == -1 && m_bPreview)
		{
			m_bPreview = FALSE;
			NotifySite(XTP_GN_PREVIEWCANCEL);
		}

		OnSelectedItemChanged();
	}

	if (m_bPressed && (m_ptPressed != CPoint(0, 0)) && (m_nSelected != -1) &&
		(abs(m_ptPressed.x - point.x) > 4 || abs(m_ptPressed.y - point.y) > 4))
	{
		m_ptPressed = CPoint(0, 0);
		NotifySite(XTP_GN_STARTDRAG);
	}
}
void CXTPControlGallery::PerformMouseDown(CPoint point)
{
	if (m_nSelected != -1 && !m_bKeyboardSelected)
	{
		m_bPressed = TRUE;
		m_ptPressed = point;
		InvalidateItems();
	}
}

void CXTPControlGallery::PerformMouseUp(CPoint /*point*/)
{
	if (m_bPressed || (GetParent()->GetPosition() == xtpBarPopup && IsItemSelected()))
	{
		m_bPressed = FALSE;
		InvalidateItems();

		OnExecute();
	}
}

BOOL CXTPControlGallery::HasBottomSeparator() const
{
	if (GetParent()->GetType() != xtpBarTypePopup)
		return FALSE;

	if (m_bShowBorders)
		return FALSE;

	int nIndex = GetIndex();
	int nNextIndex = GetParent()->GetControls()->GetNext(nIndex, +1, TRUE);

	return nNextIndex > nIndex;
}

CRect CXTPControlGallery::GetItemDrawRect(int nIndex)
{
	if (nIndex < 0 || nIndex >= m_arrRects.GetSize())
		return CRect(0, 0, 0, 0);

	CRect rc(m_arrRects[nIndex].rcItem);
	rc.OffsetRect(0, -m_nScrollPos);

	rc.IntersectRect(rc, GetItemsRect());

	return rc;
}

CRect CXTPControlGallery::GetBorders() const
{
	CRect rcBorders(0, 0, 0, 0);
	if (m_bShowBorders)
	{
		rcBorders.SetRect(1, 1, 1, 1);
	}
	if (m_pParent && m_bShowScrollBar)
	{
		rcBorders.right += (GetCommandBar() ? GetGalleryPaintManager()->m_cxPopup : GetGalleryPaintManager()->m_cxVScroll);
	}

	if (m_pParent && HasBottomSeparator())
	{
		rcBorders.bottom = 2;
	}

	rcBorders.DeflateRect(m_rcMargin.left, m_rcMargin.top, -m_rcMargin.right, -m_rcMargin.bottom);

	return rcBorders;
}

CXTPControlGalleryPaintManager* CXTPControlGallery::GetGalleryPaintManager() const
{
	return GetPaintManager()->GetGalleryPaintManager();
}

CXTPScrollBarPaintManager* CXTPControlGallery::GetScrollBarPaintManager() const
{
	return GetPaintManager()->GetGalleryPaintManager();
}

void CXTPControlGallery::SetResizable(BOOL bResizeWidth, BOOL bResizeHeight)
{
	m_nResizable = (bResizeWidth ? 1 : 0) +  (bResizeHeight ? 2 : 0);
}

CRect CXTPControlGallery::GetItemsRect() const
{
	CRect rc = GetRect();
	rc.DeflateRect(GetBorders());

	return rc;
}

void CXTPControlGallery::FillControl(CDC* pDC, CRect rcControl)
{
	GetGalleryPaintManager()->FillControl(pDC, this, rcControl);
}

void CXTPControlGallery::DoAnimate()
{
	if (abs(m_nScrollPos - m_nScrollPosTarget) > fabs(m_dAnimationStep))
	{
		m_nScrollPos = int((double)m_nScrollPos + m_dAnimationStep);
	}
	else
	{
		m_bAnimation = FALSE;
		m_nScrollPos = m_nScrollPosTarget;
		::KillTimer(GetParent()->GetSafeHwnd(), IDSYS_ANIMATE);
	}
	Reposition();

	OnScrollChanged();
	InvalidateItems(NULL, FALSE);
	GetParent()->UpdateWindow();
}

void CXTPControlGallery::StartAnimation(int nScrollPos)
{
	CRect rcItems = GetItemsRect();

	if (nScrollPos > m_nTotalHeight - rcItems.Height())
		nScrollPos = m_nTotalHeight - rcItems.Height();

	if (nScrollPos < 0)
		nScrollPos = 0;

	if (m_bAnimation && m_nScrollPosTarget == nScrollPos)
		return;

	if (!m_bAnimation && m_nScrollPos == nScrollPos)
		return;

	m_bAnimation = TRUE;
	m_nScrollPosTarget = nScrollPos;
	m_dAnimationStep = double(m_nScrollPosTarget - m_nScrollPos) / (m_nTimerElapse > 200 ? 8.0 : 3.0);
	if (m_dAnimationStep > 0 && m_dAnimationStep < 1) m_dAnimationStep = 1;
	if (m_dAnimationStep < 0 && m_dAnimationStep > -1) m_dAnimationStep = -1;

	UINT nms = 40;
	SetTimer(GetParent()->GetSafeHwnd(), IDSYS_ANIMATE, nms, NULL);
	DoAnimate();
}

void CXTPControlGallery::SetScrollPos(int nScrollPos)
{
	CRect rcItems = GetItemsRect();

	if (nScrollPos > m_nTotalHeight - rcItems.Height())
		nScrollPos = m_nTotalHeight - rcItems.Height();

	if (nScrollPos < 0)
		nScrollPos = 0;

	if (m_bAnimation)
	{
		m_bAnimation = FALSE;
		::KillTimer(GetParent()->GetSafeHwnd(), IDSYS_ANIMATE);
		m_nScrollPos = -1;
	}

	if (m_nScrollPos == nScrollPos)
		return;

	m_nScrollPos = nScrollPos;
	Reposition();

	OnScrollChanged();
	InvalidateItems(NULL, FALSE);
}


void CXTPControlGallery::EnsureVisible(int nIndex)
{
	if (!GetParent()->GetSafeHwnd())
		return;

	if (GetItemCount() == 0 || nIndex == -1 || nIndex >= GetItemCount() || nIndex >= (int)m_arrRects.GetSize())
		return;

	const GALLERYITEM_POSITION& pos = m_arrRects[nIndex];

	CRect rcItem = pos.rcItem;
	rcItem.OffsetRect(0, -m_nScrollPos);

	CRect rcItems = GetItemsRect();

	if (rcItem.top < rcItems.top)
	{
		int nScrollPos = pos.rcItem.top - rcItems.top;

		if (m_bShowLabels)
		{
			for (int i = nIndex - 1; i >= 0; i--)
			{
				const GALLERYITEM_POSITION& posLabel = m_arrRects[i];

				if (posLabel.pItem->IsLabel() && pos.rcItem.top - posLabel.rcItem.top <= rcItems.Height() - rcItem.Height())
				{
					nScrollPos = posLabel.rcItem.top - rcItems.top;
					break;
				}

				if (pos.rcItem.top - posLabel.rcItem.top > rcItems.Height()  - rcItem.Height())
					break;
			}
		}

		SetScrollPos(nScrollPos);
	}
	else if (rcItem.bottom > rcItems.bottom)
	{
		SetScrollPos(pos.rcItem.bottom - rcItems.bottom);
	}
}

long CXTPControlGallery::GetNextInRow(long nIndex, int nDirection) const
{
	if (GetItemCount() == 0 || nIndex == -1 || nIndex >= GetItemCount())
		return -1;

	CRect rcItems = GetItemsRect();
	CSize szItem = GetItems()->GetItemSize();

	if (szItem.cx * 2 > rcItems.Width() || szItem.cx == 0)
		return GetNext(nIndex, nDirection);

	int i;

	if (nDirection < 0)
	{
		if (!m_arrRects[nIndex].bBeginRow)
		{
			for (i = nIndex - 1; i >= 0; i--)
			{
				const GALLERYITEM_POSITION& pos = m_arrRects[i];

				if (!IsRectEmpty(&pos.rcItem))
				{
					return i;
				}
			}
		}

		for (i = nIndex + 1; i < m_arrRects.GetSize(); i++)
		{
			const GALLERYITEM_POSITION& pos = m_arrRects[i];

			if (pos.bBeginRow)
			{
				return nIndex;
			}

			if (!IsRectEmpty(&pos.rcItem))
			{
				nIndex = i;
			}
		}
		return nIndex;
	}
	else
	{
		for (i = nIndex + 1; i < GetItemCount(); i++)
		{
			const GALLERYITEM_POSITION& pos = m_arrRects[i];

			if (!IsRectEmpty(&pos.rcItem) && !pos.bBeginRow)
			{
				return i;
			}

			if (pos.bBeginRow)
				break;
		}

		for (i = nIndex; i >= 0; i--)
		{
			const GALLERYITEM_POSITION& pos = m_arrRects[i];

			if (pos.bBeginRow)
			{
				return i;
			}
		}
	}

	return nIndex;
}

long CXTPControlGallery::GetNextInPage(long nIndex, int nDirection) const
{
	if (GetItemCount() == 0)
		return -1;

	if (nIndex == -1 || nIndex >= GetItemCount())
		nIndex = GetNext(-1, nDirection);

	int i;
	RECT rcItem = m_arrRects[nIndex].rcItem;
	CRect rcItems = GetItemsRect();

	if (nDirection < 0)
	{
		for (i = nIndex - 1; i >= 0; i--)
		{
			const GALLERYITEM_POSITION& pos = m_arrRects[i];

			if (!pos.pItem->IsLabel() &&
				rcItem.top - pos.rcItem.top< rcItems.Height())
			{
				nIndex = i;
			}

			if (rcItem.top - pos.rcItem.top> rcItems.Height())
				return nIndex;
		}
	}
	else
	{
		for (i = nIndex + 1; i < GetItemCount(); i++)
		{
			const GALLERYITEM_POSITION& pos = m_arrRects[i];

			if (!pos.pItem->IsLabel() &&
				pos.rcItem.bottom - rcItem.bottom < rcItems.Height())
			{
				nIndex = i;
			}

			if (pos.rcItem.bottom - rcItem.bottom > rcItems.Height())
				return nIndex;
		}
	}

	return nIndex;

}

long CXTPControlGallery::GetNextInColumn(long nIndex, int nDirection) const
{
	if (GetItemCount() == 0)
		return -1;

	if (nIndex == -1 || nIndex >= GetItemCount())
		return GetNext(-1, nDirection);

	int i;
	RECT rcItem = m_arrRects[nIndex].rcItem;

	if (nDirection < 0)
	{
		for (i = nIndex - 1; i >= 0; i--)
		{
			const GALLERYITEM_POSITION& pos = m_arrRects[i];

			if (!pos.pItem->IsLabel() && pos.rcItem.left == rcItem.left)
			{
				return i;
			}
		}
	}
	else
	{
		for (i = nIndex + 1; i < GetItemCount(); i++)
		{
			const GALLERYITEM_POSITION& pos = m_arrRects[i];

			if (!pos.pItem->IsLabel() && pos.rcItem.left == rcItem.left)
			{
				return i;
			}
		}

	}

	return -1;
}

long CXTPControlGallery::GetNext(long nIndex, int nDirection) const
{
	ASSERT(nDirection == +1 || nDirection == -1);
	if (GetItemCount() == 0) return -1;

	long nNext = nIndex + nDirection;

	if (nDirection == -1 && nIndex == -1)
	{
		nNext = GetItemCount() - 1;
	}

	BOOL bCircle = FALSE;

	while (nNext != nIndex)
	{
		if (nNext >= GetItemCount())
		{
			if ((nIndex == -1 && nDirection == +1) || bCircle) return -1;
			nNext = 0;
			bCircle = TRUE;
		}
		if (nNext < 0)
		{
			if ((nIndex == -1 && nDirection == -1) || bCircle) return -1;
			nNext = GetItemCount() - 1;
			bCircle = TRUE;
		}

		if (!GetItem(nNext)->IsLabel())
		{
			return nNext;
		}
		nNext += nDirection;
	}
	return nNext;
}

void CXTPControlGallery::SetSelectedItem(int nItem)
{
	int nSelected = m_nSelected;

	m_bHideSelection = FALSE;
	m_nSelected = nItem;
	m_bPressed = FALSE;
	m_bKeyboardSelected = TRUE;

	CXTPControlGalleryItems* pItems = GetItems();

	if (pItems && pItems->m_bClipItems && nSelected != -1)
	{
		InvalidateItems(GetItemDrawRect(nSelected), TRUE);
	}

	EnsureVisible(m_nSelected);

	if (pItems && pItems->m_bClipItems && m_nSelected != -1)
	{
		InvalidateItems(GetItemDrawRect(m_nSelected), FALSE);
	}

	if (!pItems || !pItems->m_bClipItems)
	{
		InvalidateItems();
	}

	if (m_nSelected != -1)
	{
		AccessibleNotifyWinEvent(EVENT_OBJECT_FOCUS , GetParent()->GetSafeHwnd(), GetID(), m_nSelected + 1);
	}

	if (m_nSelected != -1 && !m_bPreview)
	{
		m_bPreview = TRUE;
		NotifySite(XTP_GN_PREVIEWSTART);
	}

	if (m_bPreview)
	{
		NotifySite(XTP_GN_PREVIEWCHANGE);
	}

	if (m_nSelected == -1 && m_bPreview)
	{
		m_bPreview = FALSE;
		NotifySite(XTP_GN_PREVIEWCANCEL);
	}

	OnSelectedItemChanged();
}

int CXTPControlGallery::FindItem(int nStartAfter, LPCTSTR lpszItem, BOOL bExact) const
{
	if (GetItemCount() == 0)
		return -1;

	UINT len1 = lpszItem ? (UINT)_tcslen(lpszItem) : 0;
	if (len1 < 1)
		return -1;

	int nItem = nStartAfter + 1;
	if (nItem >= GetItemCount())
		nItem = 0;

	if (nItem < 0)
		nItem = 0;

	int nStop = nItem;

	do
	{
		CXTPControlGalleryItem* pItem = GetItem(nItem);

		if (bExact)
		{
			if (pItem->GetCaption().CompareNoCase(lpszItem) == 0)
				return nItem;
		}
		else
		{
			UINT len2 = pItem->GetCaption().GetLength();

			int nResult = CompareString(LOCALE_SYSTEM_DEFAULT, NORM_IGNORECASE, lpszItem, min(len1,len2),
				pItem->GetCaption(), min(len1,len2));

			if (nResult == CSTR_EQUAL)
			{
				if (len1 <= len2)
				{
					return nItem;
				}
			}


		}

		if (++nItem == GetItemCount())
			nItem = 0;

	} while (nItem != nStop);

	return -1;
}

int CXTPControlGallery::GetScrollPos() const
{
	return m_nScrollPos;
}

int CXTPControlGallery::SetTopIndex(int nIndex)
{
	if (nIndex == -1 || nIndex > m_arrRects.GetSize())
		return -1;

	SetScrollPos(m_arrRects[nIndex].rcItem.top);
	return 0;
}

CString CXTPControlGallery::GetItemCaption(int nIndex)
{
	CXTPControlGalleryItem* pItem = GetItem(nIndex);

	if (pItem)
		return pItem->GetCaption();

	return _T("");
}

void CXTPControlGallery::DoScroll(int cmd, int pos)
{
	int y = m_bAnimation ? m_nScrollPosTarget : m_nScrollPos;
	CRect rcItems = GetItemsRect();

	CXTPControlGalleryItems* pItems = GetItems();
	if (!pItems)
		return;

	switch (cmd)
	{
	case SB_TOP:
		y = 0;
		break;
	case SB_BOTTOM:
		y = m_nTotalHeight;
		break;
	case SB_LINEUP:
		y -= pItems->GetItemSize().cy;
		break;
	case SB_LINEDOWN:
		y += pItems->GetItemSize().cy;
		break;
	case SB_PAGEUP:
		y -= rcItems.Height();
		break;
	case SB_PAGEDOWN:
		y += rcItems.Height();
		break;
	case SB_THUMBTRACK:
		y = pos;
		break;
	}

	if (GetCommandBar() && (cmd == SB_LINEUP || cmd == SB_LINEDOWN || cmd == SB_ENDSCROLL))
	{
		StartAnimation(y);
	}
	else
	{
		SetScrollPos(y);
	}
}

void CXTPControlGallery::GetScrollInfo(SCROLLINFO* pSI)
{
	CRect rcItems = GetItemsRect();

	pSI->nMin = 0;
	pSI->nMax = max(0, m_nTotalHeight - 1);
	pSI->nPos = m_nScrollPos;
	pSI->nPage = rcItems.Height();
}


//////////////////////////////////////////////////////////////////////////

CSize CXTPControlGallery::GetSize(CDC* pDC)
{
	if (IsShowAsButton())
	{
		int nWidth = m_nWidth, nHeight = m_nHeight;
		m_nWidth = m_nHeight = 0;

		CSize sz = CXTPControlPopup::GetSize(pDC);
		m_nWidth = nWidth; m_nHeight = nHeight;
		return sz;
	}

	return CSize(max(IsResizable() ? GetCustomizeMinWidth() : 0, m_nWidth), m_nHeight);
}

void CXTPControlGallery::RedrawScrollBar()
{
	InvalidateItems(NULL, FALSE);
}

void CXTPControlGallery::DrawScrollBar(CDC* pDC)
{
	if (GetCommandBar())
		GetGalleryPaintManager()->DrawPopupScrollBar(pDC, this);
	else
		GetGalleryPaintManager()->DrawScrollBar(pDC, this);
}

void CXTPControlGallery::Draw(CDC* pDC)
{
	if (IsShowAsButton())
	{
		CXTPControlPopup::Draw(pDC);
		return;
	}
	FillControl(pDC, m_rcControl);

	DrawItems(pDC);

	if (m_bShowScrollBar)
	{
		DrawScrollBar(pDC);
	}
}

void CXTPControlGallery::OnMouseMove(CPoint point)
{
	if (IsShowAsButton())
	{
		CXTPControlPopup::OnMouseMove(point);
		return;
	}

	PerformMouseMove(point);

	int ht = HitTestScrollBar(point);

	if (ht != m_spi.ht)
	{
		m_spi.ht = ht;
		RedrawParent();
	}
}

void CXTPControlGallery::HideSelection()
{
	m_bHideSelection = TRUE;
	m_bKeyboardSelected = FALSE;

	if (m_nSelected != -1)
	{
		CXTPControlGalleryItems* pItems = GetItems();

		if (pItems && pItems->m_bClipItems)
		{
			InvalidateItems(GetItemDrawRect(m_nSelected), FALSE);
		}
		else
		{
			InvalidateItems();
		}
	}

	if (m_bPreview && m_nSelected != -1)
	{
		NotifySite(XTP_GN_PREVIEWCHANGE);
	}

	if (m_bPreview)
	{
		m_bPreview = FALSE;
		NotifySite(XTP_GN_PREVIEWCANCEL);
	}

	OnSelectedItemChanged();
}

void CXTPControlGallery::OnSelectedItemChanged()
{
	NotifySite(XTP_GN_SELCHANGE);
}

BOOL CXTPControlGallery::OnSetSelected(int bSelected)
{
	if (IsShowAsButton())
	{
		return CXTPControlPopup::OnSetSelected(bSelected);
	}

	if (!bSelected)
	{
		m_bPressed = FALSE;
		m_spi.ht = HTNOWHERE;
		HideSelection();
	}

	if (IsKeyboardSelected(bSelected) && !IsItemSelected())
	{
		if (GetCommandBar() == NULL)
		{
			SetSelectedItem(GetNext(-1, bSelected == TRUE_KEYBOARD_PREV ? -1 : +1));
		}
		else
		{
			SetSelectedItem(-1);
		}
	}

	return CXTPControl::OnSetSelected(bSelected);
}


BOOL CXTPControlGallery::IsFocused() const
{
	return IsShowAsButton() ? FALSE : GetSelected();
}


BOOL CXTPControlGallery::OnHookKeyDown(UINT nChar, LPARAM /*lParam*/)
{
	ASSERT(IsFocused());

	if (!IsFocused())
		return FALSE;

	if (m_nSelected == -1 && GetCommandBar() != NULL)
	{
		if (nChar == VK_RETURN || nChar == VK_SPACE)
		{
			GetParent()->SetPopuped(m_nIndex);
			return TRUE;
		}
		return FALSE;
	}

	int nSelected;
	CXTPDrawHelpers::KeyToLayout(CXTPControl::GetParent(), nChar);

	switch (nChar)
	{
	case VK_HOME:
		SetSelectedItem(GetNext(-1, +1));
		return TRUE;

	case VK_END:
		SetSelectedItem(GetNext(-1, -1));
		return TRUE;

	case VK_LEFT:
		SetSelectedItem(GetNextInRow(m_nSelected, -1));
		return TRUE;

	case VK_RIGHT:
		SetSelectedItem(GetNextInRow(m_nSelected, +1));
		return TRUE;

	case VK_UP:
		nSelected = GetNextInColumn(m_nSelected, -1);

		if (nSelected == -1 && GetControls()->GetNext(m_nIndex, -1) == m_nIndex)
		{
			nSelected = GetNext(-1, -1);
		}

		if (nSelected != -1)
		{
			SetSelectedItem(nSelected);
			return TRUE;
		}
		return FALSE;

	case VK_PRIOR:
		SetSelectedItem(GetNextInPage(m_nSelected, -1));
		return TRUE;

	case VK_NEXT:
		SetSelectedItem(GetNextInPage(m_nSelected, +1));
		return TRUE;

	case VK_DOWN:
		nSelected = GetNextInColumn(m_nSelected, +1);

		if (nSelected == -1 && GetControls()->GetNext(m_nIndex, +1) == m_nIndex)
		{
			nSelected = GetNext(-1, +1);
		}

		if (nSelected != -1)
		{
			SetSelectedItem(nSelected);
			return TRUE;
		}
		return FALSE;

	case VK_TAB:
		if (GetKeyState(VK_SHIFT) >= 0)
		{
			nSelected = GetNext(m_nSelected, +1);
			if (nSelected > m_nSelected)
			{
				SetSelectedItem(nSelected);
				return TRUE;
			}
		}
		else
		{
			nSelected = GetNext(m_nSelected, -1);
			if (nSelected < m_nSelected)
			{
				SetSelectedItem(nSelected);
				return TRUE;
			}
		}
		return FALSE;

	case VK_RETURN:
	case VK_SPACE:
		OnExecute();
		break;
	}

	return FALSE;
}

BOOL CXTPControlGallery::OnHookMouseWheel(UINT /*nFlags*/, short zDelta, CPoint /*pt*/)
{
	CXTPControlGalleryItems* pItems = GetItems();
	if (!pItems)
		return FALSE;

	SetScrollPos(m_nScrollPos + (zDelta < 0 ? 3 * pItems->GetItemSize().cy : -3 * pItems->GetItemSize().cy));
	return TRUE;
}

void CXTPControlGallery::OnScrollChanged()
{
	SetupScrollInfo();
}

void CXTPControlGallery::OnClick(BOOL bKeyboard /*= FALSE*/, CPoint point /*= CPoint(0, 0)*/)
{
	if (IsShowAsButton())
	{
		CXTPControlPopup::OnClick(bKeyboard, point);
		return;
	}

	if (IsCustomizeMode())
	{
		m_pParent->SetPopuped(-1);
		m_pParent->SetSelected(-1);
		CustomizeStartDrag(point);
		return;
	}

	if (!GetEnabled())
		return;

	if (!bKeyboard)
	{
		m_pParent->SetPopuped(-1);
		m_pParent->SetSelected(GetIndex());

		OnMouseMove(point);


		if (m_spi.ht == XTP_HTSCROLLPOPUP)
		{
			m_pParent->SetPopuped(m_nIndex);
		}
		else if (m_spi.ht != HTNOWHERE)
		{
			PerformTrackInit(CXTPControl::GetParent()->GetSafeHwnd(), point, &m_spi, (GetKeyState(VK_SHIFT) < 0) ? TRUE : FALSE);
		}
		else
		{
			PerformMouseDown(point);
		}
	}
	else
	{

	}
}

void CXTPControlGallery::OnLButtonUp(CPoint point)
{
	if (IsShowAsButton())
	{
		CXTPControlPopup::OnLButtonUp(point);
	}
	else
	{
		PerformMouseUp(point);
	}
}


CRect CXTPControlGallery::GetScrollBarRect()
{
	if (!m_bShowScrollBar)
		return CRect(0, 0, 0, 0);

	CRect rc(m_rcControl);

	rc.left = rc.right - (GetCommandBar() ? GetGalleryPaintManager()->m_cxPopup :
		GetGalleryPaintManager()->m_cxVScroll);

	if (m_bShowBorders)
	{
		rc.top++;
		rc.bottom--;
		rc.right--;
		rc.left--;
	}

	if (HasBottomSeparator())
		rc.bottom -= 2;

	return rc;
}

void CXTPControlGallery::CalcScrollBarInfo(LPRECT lprc, SCROLLBARPOSINFO* pSBInfo, SCROLLINFO* pSI)
{
	if (GetCommandBar())
	{
		pSBInfo->rc = *lprc;
		pSBInfo->pxTop = lprc->top;
		pSBInfo->pxBottom = lprc->bottom;
		pSBInfo->pxLeft = lprc->left;
		pSBInfo->pxRight = lprc->right;

		pSBInfo->pos = pSI->nPos;
		pSBInfo->page = pSI->nPage;
		pSBInfo->posMin = pSI->nMin;
		pSBInfo->posMax = pSI->nMax;

		pSBInfo->pxDownArrow = pSBInfo->pxUpArrow  = pSBInfo->pxTop + GetGalleryPaintManager()->m_cyPopupUp;
		pSBInfo->pxPopup = pSBInfo->pxDownArrow + GetGalleryPaintManager()->m_cyPopupDown;
	}
	else
	{
		CXTPScrollBase::CalcScrollBarInfo(lprc, pSBInfo, pSI);
	}
}

CWnd* CXTPControlGallery::GetParentWindow() const
{
	return CXTPControl::GetParent();
}

void CXTPControlGallery::SetRect(CRect rcControl)
{
	if (m_rcControl == rcControl)
		return;

	CXTPControl::SetRect(rcControl);
	Reposition();
}

void CXTPControlGallery::Copy(CXTPControl* pControl, BOOL bRecursive)
{
	ASSERT(DYNAMIC_DOWNCAST(CXTPControlGallery, pControl));

	CXTPControlPopup::Copy(pControl, bRecursive);

	CXTPControlGallery* pGallery = (CXTPControlGallery*)pControl;

	m_bShowLabels = pGallery->m_bShowLabels;
	m_bShowBorders = pGallery->m_bShowBorders;
	m_bShowScrollBar = pGallery->m_bShowScrollBar;

	m_rcMargin = pGallery->m_rcMargin;
	m_nResizable = pGallery->m_nResizable;
}

void CXTPControlGallery::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPControlPopup::DoPropExchange(pPX);

	PX_Bool(pPX, _T("ShowLabels"), m_bShowLabels, TRUE);
	PX_Bool(pPX, _T("ShowBorders"), m_bShowBorders, TRUE);
	PX_Bool(pPX, _T("ShowScrollBar"), m_bShowScrollBar, TRUE);

	PX_Rect(pPX, _T("GalleryMargin"), m_rcMargin, CRect(0, 0, 0, 0));

	if (pPX->GetSchema() > _XTP_SCHEMA_110)
	{
		PX_Int(pPX, _T("Resizable"), m_nResizable, 0);
	}
}

INT_PTR CXTPControlGallery::OnToolHitTest(CPoint point, TOOLINFO* pTI) const
{
	if (IsShowAsButton() || GetItems() == NULL)
	{
		return -1;
	}

	RECT rcItem;
	int nIndex = HitTestItem(point, &rcItem);
	if (nIndex != -1)
	{
		CXTPControlGalleryItem* pItem = GetItem(nIndex);

		INT_PTR nHit = pItem->m_nImage >= 0 ? pItem->m_nImage : pItem->m_nId;

		CXTPToolTipContext::FillInToolInfo(pTI, GetParent()->GetSafeHwnd(), rcItem, nHit,
			pItem->GetToolTip(), pItem->GetToolTip(), pItem->GetDescription(), GetItems()->GetImageManager());

		return nHit;
	}
	return 0;
}

BOOL CXTPControlGallery::IsScrollBarEnabled() const
{
	return GetEnabled();
}

BOOL CXTPControlGallery::IsScrollButtonEnabled(int ht)
{
	if (!GetEnabled())
		return FALSE;

	if (!GetCommandBar())
		return TRUE;

	if (ht == XTP_HTSCROLLUP)
	{
		return m_nScrollPos > 0;
	}
	else if (ht == XTP_HTSCROLLDOWN)
	{
		CRect rcItems = GetItemsRect();
		return m_nScrollPos < m_nTotalHeight - rcItems.Height();
	}
	else
	{
		ASSERT(FALSE);
	}

	return TRUE;
}

void CXTPControlGallery::SetItemsMargin(int nLeft, int nTop, int nRight, int nBottom)
{
	m_rcMargin.SetRect(nLeft, nTop, nRight, nBottom);
}

void CXTPControlGallery::AdjustExcludeRect(CRect& rc, BOOL bVertical)
{
	if (IsShowAsButton())
	{
		CXTPControlPopup::AdjustExcludeRect(rc, bVertical);
	}
	else
	{
		rc.SetRect(m_rcControl.left - 1, m_rcControl.top, m_rcControl.left - 1, m_rcControl.top);
	}
}

HRESULT CXTPControlGallery::GetAccessibleChildCount(long FAR* pChildCount)
{
	if (pChildCount == 0)
		return E_INVALIDARG;

	*pChildCount = GetItemCount();

	return S_OK;
}
HRESULT CXTPControlGallery::GetAccessibleChild(VARIANT /*varChild*/, IDispatch* FAR* ppdispChild)
{
	SAFE_MANAGE_STATE(m_pModuleState);

	*ppdispChild = NULL;
	return S_FALSE;
}

HRESULT CXTPControlGallery::GetAccessibleName(VARIANT varChild, BSTR* pszName)
{
	SAFE_MANAGE_STATE(m_pModuleState);

	int nIndex = GetChildIndex(&varChild);

	if (nIndex == CHILDID_SELF)
		return CXTPControl::GetAccessibleName(varChild, pszName);

	CXTPControlGalleryItem* pItem = GetItem(nIndex - 1);
	if (!pItem)
		return E_INVALIDARG;

	CString strCaption = pItem->GetCaption();
	CXTPDrawHelpers::StripMnemonics(strCaption);

	if (strCaption.IsEmpty())
	{
		strCaption = pItem->GetToolTip();
	}

	*pszName = strCaption.AllocSysString();
	return S_OK;
}

HRESULT CXTPControlGallery::GetAccessibleRole(VARIANT varChild, VARIANT* pvarRole)
{
	pvarRole->vt = VT_I4;
	pvarRole->lVal = GetChildIndex(&varChild) == CHILDID_SELF ? ROLE_SYSTEM_LISTITEM : ROLE_SYSTEM_LIST;
	return S_OK;
}

HRESULT CXTPControlGallery::AccessibleSelect(long flagsSelect, VARIANT varChild)
{
	SAFE_MANAGE_STATE(m_pModuleState);

	int nIndex = GetChildIndex(&varChild);
	if (nIndex == CHILDID_SELF)
		return CXTPControl::AccessibleSelect(flagsSelect, varChild);

	SetSelectedItem(nIndex - 1);

	return S_OK;
}

HRESULT CXTPControlGallery::GetAccessibleState(VARIANT varChild, VARIANT* pvarState)
{
	int nIndex = GetChildIndex(&varChild);

	if (nIndex == CHILDID_SELF)
		return CXTPControl::GetAccessibleState(varChild, pvarState);

	CXTPControlGalleryItem* pItem = GetItem(nIndex - 1);
	if (!pItem)
		return E_INVALIDARG;

	pvarState->vt = VT_I4;
	pvarState->lVal = STATE_SYSTEM_FOCUSABLE | STATE_SYSTEM_SELECTABLE;

	if (m_nSelected == pItem->GetIndex())
		pvarState->lVal |= STATE_SYSTEM_FOCUSED;

	if (IsItemChecked(pItem))
		pvarState->lVal |= STATE_SYSTEM_SELECTED;

	return S_OK;
}

HRESULT CXTPControlGallery::GetAccessibleDefaultAction(VARIANT varChild, BSTR* pszDefaultAction)
{
	if (GetChildIndex(&varChild) == CHILDID_SELF)
			return CXTPControl::GetAccessibleDefaultAction(varChild, pszDefaultAction);

	*pszDefaultAction = SysAllocString(L"Click");

	return S_OK;
}

HRESULT CXTPControlGallery::AccessibleDoDefaultAction(VARIANT varChild)
{
	SAFE_MANAGE_STATE(m_pModuleState);

	int nIndex = GetChildIndex(&varChild);

	if (nIndex == CHILDID_SELF)
		return CXTPControl::AccessibleDoDefaultAction(varChild);

	CXTPControlGalleryItem* pItem = GetItem(nIndex - 1);
	if (!pItem)
		return E_INVALIDARG;

	SetSelectedItem(nIndex - 1);
	return S_OK;
}


HRESULT CXTPControlGallery::AccessibleLocation(long* pxLeft, long* pyTop, long* pcxWidth, long* pcyHeight, VARIANT varChild)
{
	*pxLeft = *pyTop = *pcxWidth = *pcyHeight = 0;

	int nIndex = GetChildIndex(&varChild);

	if (!m_pParent->GetSafeHwnd())
		return S_OK;

	if (!IsVisible())
		return S_OK;

	CRect rcControl = GetRect();

	if (nIndex != CHILDID_SELF)
	{
		rcControl = GetItemDrawRect(nIndex - 1);
	}

	m_pParent->ClientToScreen(&rcControl);

	*pxLeft = rcControl.left;
	*pyTop = rcControl.top;
	*pcxWidth = rcControl.Width();
	*pcyHeight = rcControl.Height();

	return S_OK;
}

HRESULT CXTPControlGallery::AccessibleHitTest(long xLeft, long yTop, VARIANT* pvarID)
{
	if (pvarID == NULL)
		return E_INVALIDARG;

	pvarID->vt = VT_EMPTY;

	if (!m_pParent->GetSafeHwnd())
		return S_FALSE;

	if (!CXTPWindowRect(m_pParent).PtInRect(CPoint(xLeft, yTop)))
		return S_FALSE;

	pvarID->vt = VT_I4;
	pvarID->lVal = CHILDID_SELF;

	CPoint pt(xLeft, yTop);
	m_pParent->ScreenToClient(&pt);

	if (!GetRect().PtInRect(pt))
		return S_FALSE;

	int nIndex = HitTestItem(pt);
	if (nIndex != -1)
	{
		pvarID->lVal = nIndex + 1;
	}

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
// CXTPControlComboBoxGalleryPopupBar

IMPLEMENT_XTP_COMMANDBAR(CXTPControlComboBoxGalleryPopupBar, CXTPControlComboBoxPopupBar)

CXTPControlComboBoxGalleryPopupBar::CXTPControlComboBoxGalleryPopupBar()
{
}

CXTPControlComboBoxGalleryPopupBar* AFX_CDECL CXTPControlComboBoxGalleryPopupBar::CreateComboBoxGalleryPopupBar(CXTPCommandBars* pCommandBars)
{
	CXTPControlComboBoxGalleryPopupBar* pPopupBar = new CXTPControlComboBoxGalleryPopupBar();
	pPopupBar->SetCommandBars(pCommandBars);
	return pPopupBar;
}

BEGIN_MESSAGE_MAP(CXTPControlComboBoxGalleryPopupBar, CXTPControlComboBoxPopupBar)
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

CXTPControlGallery* CXTPControlComboBoxGalleryPopupBar::GetGalleryItem() const
{
	CXTPControl* pItem = GetControl(0);
	ASSERT_KINDOF(CXTPControlGallery, pItem);

	return (CXTPControlGallery*)pItem;
}

void CXTPControlComboBoxGalleryPopupBar::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (GetControls()->HitTest(point) == GetGalleryItem())
	{
		CXTPControlComboBox* pComboBox = (CXTPControlComboBox*)m_pControlPopup;
		pComboBox->OnSelChanged();
		pComboBox->OnExecute();
		return;
	}

	CXTPPopupBar::OnLButtonUp(nFlags, point);
}

BOOL CXTPControlComboBoxGalleryPopupBar::SetTrackingMode(int bMode, BOOL bSelectFirst, BOOL bKeyboard)
{
	CXTPControlComboBox* pComboBox = ((CXTPControlComboBox*)m_pControlPopup);

	if (!CXTPPopupBar::SetTrackingMode(bMode, bSelectFirst, bKeyboard))
		return FALSE;

	if (!bMode)
	{
		pComboBox->NotifySite(CBN_CLOSEUP);

	}
	else
	{
		SetSelected(0, TRUE);

		pComboBox->UpdatePopupSelection();
		pComboBox->NotifySite(CBN_DROPDOWN);

	}

	return TRUE;
}

BOOL CXTPControlComboBoxGalleryPopupBar::OnHookKeyDown(UINT nChar, LPARAM lParam)
{
	ASSERT(m_pControlPopup);
	CXTPControlComboBox* pComboBox = ((CXTPControlComboBox*)m_pControlPopup);
	if (!pComboBox)
		return CXTPCommandBar::OnHookKeyDown(nChar, lParam);

	if (pComboBox->GetEditCtrl() && pComboBox->GetEditCtrl()->IsImeMode())
		return ProcessHookKeyDown(pComboBox, nChar, lParam);

	if (nChar == VK_ESCAPE)
	{
		if (pComboBox->IsFocused())
			return FALSE;

		return CXTPCommandBar::OnHookKeyDown(nChar, lParam);
	}

	if (nChar == VK_TAB) return FALSE;

	if (nChar == VK_RETURN)
	{
		if (GetControl(m_nSelected) == GetGalleryItem())
		{
			m_pControlPopup->OnExecute();
			return TRUE;
		}
	}

	return ProcessHookKeyDown(pComboBox, nChar, lParam);
}

//////////////////////////////////////////////////////////////////////////
// CXTPControlGalleryPaintManager

CXTPControlGalleryPaintManager::CXTPControlGalleryPaintManager(CXTPPaintManager* pPaintManager)
	: CXTPScrollBarPaintManager(pPaintManager)
{
	m_cyPopupUp = m_cyPopupDown = 19;
	m_cxPopup = 16;
}

void CXTPControlGalleryPaintManager::DrawLabel(CDC* pDC, CXTPControlGalleryItem* pLabel, CRect rcItem)
{
	CXTPPaintManager* pPaintManager = m_pPaintManager;
	pDC->FillSolidRect(rcItem, pPaintManager->GetXtremeColor(XPCOLOR_LABEL));

	pDC->FillSolidRect(rcItem.left, rcItem.bottom - 1, rcItem.Width(), 1, pPaintManager->GetXtremeColor(XPCOLOR_3DSHADOW));

	CXTPFontDC fnt(pDC, pPaintManager->GetRegularBoldFont());

	CRect rcText(rcItem);
	rcText.DeflateRect(10, 0);
	pDC->SetTextColor(pPaintManager->GetXtremeColor(XPCOLOR_MENUBAR_TEXT));
	pDC->DrawText(pLabel->GetCaption(), rcText, DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS);
}

void CXTPControlGalleryPaintManager::FillControl(CDC* pDC, CXTPControlGallery* pGallery, CRect rcControl)
{
	if (!pGallery->IsTransparent())
	{
		pDC->FillSolidRect(rcControl, m_pPaintManager->GetXtremeColor(XPCOLOR_MENUBAR_FACE));
	}

	if (pGallery->IsShowBorders())
	{
		pDC->Draw3dRect(rcControl, m_pPaintManager->GetXtremeColor(XPCOLOR_3DSHADOW),
			m_pPaintManager->GetXtremeColor(XPCOLOR_3DSHADOW));
	}

	if (pGallery->HasBottomSeparator())
	{
		pDC->FillSolidRect(rcControl.left, rcControl.bottom - 2, rcControl.Width(), 1, m_pPaintManager->GetXtremeColor(XPCOLOR_SEPARATOR));
	}
}

void CXTPControlGalleryPaintManager::RefreshMetrics()
{
	CXTPScrollBarPaintManager::RefreshMetrics();

	m_cyPopupUp = m_cyPopupDown = 19;
	m_cxPopup = 16;
}

void CXTPControlGalleryPaintManager::DrawScrollBar(CDC* pDC, CXTPScrollBase* pGallery)
{
	CXTPScrollBarPaintManager::DrawScrollBar(pDC, pGallery);
}

void CXTPControlGalleryPaintManager::DrawPopupScrollBar(CDC* pDC, CXTPControlGallery* pGallery)
{
	#define GETPARTSTATE3(ht, bEnabled) \
	(!bEnabled ? 4 : nPressetHt == ht ? 3 : nHotHt == ht ? 2 : 0)

	CXTPControlGallery::SCROLLBARTRACKINFO* pSBTrack = pGallery->GetScrollBarTrackInfo();
	CXTPControlGallery::SCROLLBARPOSINFO* pSBInfo = pGallery->GetScrollBarPosInfo();

	CRect rcScroll = pSBInfo->rc;
	pDC->FillSolidRect(rcScroll, m_pPaintManager->GetXtremeColor(XPCOLOR_TOOLBAR_FACE));

	CRect rcScrollUp(rcScroll.left, rcScroll.top, rcScroll.right, rcScroll.top + m_cyPopupUp);
	CRect rcScrollDown(rcScroll.left, rcScrollUp.bottom, rcScroll.right, rcScrollUp.bottom + m_cyPopupDown);
	CRect rcScrollPopup(rcScroll.left, rcScrollDown.bottom, rcScroll.right, rcScroll.bottom);

	BOOL bControlEnabled = pGallery->GetEnabled();
	BOOL nPressetHt = pSBTrack ? pSBInfo->ht : -1;
	BOOL nHotHt = pSBTrack ? -1 : pSBInfo->ht;

	int nState = GETPARTSTATE3(XTP_HTSCROLLUP, (bControlEnabled && pGallery->IsScrollButtonEnabled(XTP_HTSCROLLUP)));
	m_pPaintManager->DrawRectangle(pDC, rcScrollUp, nState == 3 || nState == 2, nState == 3, nState != 4, FALSE, FALSE, xtpBarTypeNormal, xtpBarPopup);
	COLORREF clr = m_pPaintManager->GetRectangleTextColor(nState == 2, nState == 3, nState != 4, FALSE, FALSE, xtpBarTypeNormal, xtpBarPopup);
	CPoint pt = rcScrollUp.CenterPoint();
	m_pPaintManager->Triangle(pDC, CPoint(pt.x - 4 , pt.y + 2), CPoint(pt.x + 4, pt.y + 2), CPoint (pt.x, pt.y - 2), clr);

	nState = GETPARTSTATE3(XTP_HTSCROLLDOWN, (bControlEnabled && pGallery->IsScrollButtonEnabled(XTP_HTSCROLLDOWN)));
	m_pPaintManager->DrawRectangle(pDC, rcScrollDown, nState == 3 || nState == 2, nState == 3, nState != 4, FALSE, FALSE, xtpBarTypeNormal, xtpBarPopup);
	clr = m_pPaintManager->GetRectangleTextColor(nState == 2, nState == 3, nState != 4, FALSE, FALSE, xtpBarTypeNormal, xtpBarPopup);
	pt = rcScrollDown.CenterPoint();
	m_pPaintManager->Triangle(pDC, CPoint(pt.x - 4 , pt.y - 2), CPoint(pt.x + 4, pt.y - 2), CPoint (pt.x, pt.y + 2), clr);

	nState = GETPARTSTATE3(XTP_HTSCROLLPOPUP, bControlEnabled);
	m_pPaintManager->DrawRectangle(pDC, rcScrollPopup, nState == 3 || nState == 2, nState == 3, nState != 4, FALSE, FALSE, xtpBarTypeNormal, xtpBarPopup);
	clr = m_pPaintManager->GetRectangleTextColor(nState == 2, nState == 3, nState != 4, FALSE, FALSE, xtpBarTypeNormal, xtpBarPopup);
	pt = rcScrollPopup.CenterPoint();
	m_pPaintManager->Triangle(pDC, CPoint(pt.x - 4 , pt.y - 1), CPoint(pt.x + 4, pt.y - 1), CPoint (pt.x, pt.y + 3), clr);
	pDC->FillSolidRect(pt.x - 4, pt.y - 4, 9, 2, clr);
}



