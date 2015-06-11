// XTPTabManager.cpp: implementation of the CXTPTabManager class.
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

#include "StdAfx.h"

#include "Common/XTPDrawHelpers.h"
#include "Common/XTPToolTipContext.h"
#include "Common/XTPResourceManager.h"
#include "Common/XTPImageManager.h"
#include "Common/XTPMarkupRender.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPWinThemeWrapper.h"

#include <TabManager/Resource.h>
#include <TabManager/XTPTabManagerDefines.h>
#include <TabManager/XTPTabManagerAtom.h>
#include <TabManager/XTPTabManagerNavigateButton.h>
#include <TabManager/XTPTabManagerNavigateButtons.h>
#include <TabManager/XTPTabManagerItem.h>
#include <TabManager/XTPTabManager.h>
#include <TabManager/XTPTabPaintManagerColorSet.h>
#include <TabManager/XTPTabPaintManagerTheme.h>
#include <TabManager/XTPTabPaintManager.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif





//////////////////////////////////////////////////////////////////////////
// CXTPTabManager::CRowIndexer


CXTPTabManager::CRowIndexer::CRowIndexer(CXTPTabManager* pManager)
{
	m_nRowCount = 0;
	m_pRowItems = 0;
	m_pManager = pManager;

	CreateIndexer(1);
}

CXTPTabManager::CRowIndexer::~CRowIndexer()
{
	SAFE_DELETE_AR(m_pRowItems);
}

CXTPTabManager::ROW_ITEMS* CXTPTabManager::CRowIndexer::CreateIndexer(int nRowCount)
{
	if (m_nRowCount != nRowCount)
	{
		SAFE_DELETE_AR(m_pRowItems);
		m_pRowItems = new ROW_ITEMS[nRowCount];
		m_nRowCount = nRowCount;
	}

	if (nRowCount > 0)
	{
		m_pRowItems[0].nFirstItem = 0;
		m_pRowItems[0].nLastItem = m_pManager->GetItemCount() - 1;
	}
	return m_pRowItems;
}

//////////////////////////////////////////////////////////////////////////
// CXTPTabManager::CNavigateButtonArrow

class CXTPTabManager::CNavigateButtonArrow : public CXTPTabManagerNavigateButton
{
protected:
	CNavigateButtonArrow(CXTPTabManager* pManager, XTPTabNavigateButton nID, XTPTabNavigateButtonFlags dwFlags)
		: CXTPTabManagerNavigateButton(pManager, nID, dwFlags)
	{
	}
	virtual void Reposition(CRect& rcNavigateButtons);
	virtual void AdjustWidth(int& nWidth);

};

class CXTPTabManager::CNavigateButtonArrowLeft : public CNavigateButtonArrow
{
public:
	CNavigateButtonArrowLeft(CXTPTabManager* pManager, XTPTabNavigateButtonFlags dwFlags)
		: CNavigateButtonArrow(pManager, xtpTabNavigateButtonLeft, dwFlags)
	{
	}
protected:
	virtual void DrawEntry(CDC* pDC, CRect rc);
	virtual void Reposition(CRect& rcNavigateButtons);
	virtual void OnExecute(BOOL bTick);
};

class CXTPTabManager::CNavigateButtonArrowRight : public CNavigateButtonArrow
{
public:
	CNavigateButtonArrowRight(CXTPTabManager* pManager, XTPTabNavigateButtonFlags dwFlags)
		: CNavigateButtonArrow(pManager, xtpTabNavigateButtonRight, dwFlags)
	{
	}
protected:
	virtual void DrawEntry(CDC* pDC, CRect rc);
	virtual void Reposition(CRect& rcNavigateButtons);
	virtual void OnExecute(BOOL bTick);
};

class CXTPTabManager::CNavigateButtonClose : public CXTPTabManagerNavigateButton
{
public:
	CNavigateButtonClose(CXTPTabManager* pManager, XTPTabNavigateButtonFlags dwFlags)
		: CXTPTabManagerNavigateButton(pManager, xtpTabNavigateButtonClose, dwFlags)
	{
		CXTPResourceManager::AssertValid(XTPResourceManager()->LoadString(&m_strToolTip, XTP_IDS_TABNAVIGATEBUTTON_CLOSE));
	}
protected:
	virtual void DrawEntry(CDC* pDC, CRect rc);
};

class CXTPTabManager::CNavigateButtonTabClose : public CXTPTabManager::CNavigateButtonClose
{
public:
	CNavigateButtonTabClose(CXTPTabManagerItem* pItem, XTPTabNavigateButtonFlags dwFlags)
		: CXTPTabManager::CNavigateButtonClose(pItem->GetTabManager(), dwFlags)
	{
		m_pItem = pItem;
	}
};


void CXTPTabManager::CNavigateButtonClose::DrawEntry(CDC* pDC, CRect rc)
{
	CPoint pt = rc.CenterPoint();

	pDC->MoveTo(pt.x - 4, pt.y - 3);
	pDC->LineTo(pt.x + 3, pt.y + 4);
	pDC->MoveTo(pt.x - 3, pt.y - 3);
	pDC->LineTo(pt.x + 4, pt.y + 4);

	pDC->MoveTo(pt.x - 4, pt.y + 3);
	pDC->LineTo(pt.x + 3, pt.y - 4);
	pDC->MoveTo(pt.x - 3, pt.y + 3);
	pDC->LineTo(pt.x + 4, pt.y - 4);
}

void CXTPTabManager::CNavigateButtonArrowRight::DrawEntry(CDC* pDC, CRect rc)
{
	CPoint pt = rc.CenterPoint();

	if (m_pManager->IsHorizontalPosition())
		CXTPDrawHelpers::Triangle(pDC, CPoint(pt.x - 2, pt.y - 5), CPoint(pt.x + 2, pt.y - 1), CPoint(pt.x - 2, pt.y + 3));
	else
		CXTPDrawHelpers::Triangle(pDC, CPoint(pt.x - 5, pt.y - 2), CPoint(pt.x - 1, pt.y + 2), CPoint(pt.x + 3, pt.y - 2));
}

void CXTPTabManager::CNavigateButtonArrow::AdjustWidth(int& nWidth)
{
	if (m_dwFlags == xtpTabNavigateButtonAlways)
	{
		if ((m_pManager->GetPosition() == xtpTabPositionTop) || (m_pManager->GetPosition() == xtpTabPositionBottom))
		{
			nWidth -= GetSize().cx;
		}
		else
		{
			nWidth -= GetSize().cy;
		}
	}
}
void CXTPTabManager::CNavigateButtonArrow::Reposition(CRect& rcNavigateButtons)
{
	if (m_pManager->GetLayout() == xtpTabLayoutMultiRow)
	{
		m_rcButton.SetRectEmpty();
		return;
	}

	if (m_dwFlags == xtpTabNavigateButtonAutomatic)
	{
		if (m_pManager->GetLayout() == xtpTabLayoutSizeToFit)
		{
			m_rcButton.SetRectEmpty();
			return;
		}

		CRect rc = m_pManager->GetAppearanceSet()->GetHeaderMargin();

		if (!(m_pManager->GetItemsLength() + m_pManager->GetHeaderOffset() - rc.left - 1 > m_pManager->GetRectLength(rcNavigateButtons) - (rc.left + rc.right)
			|| m_pManager->GetHeaderOffset() < 0))
		{
			m_rcButton.SetRectEmpty();
			return;
		}
	}

	CXTPTabManagerNavigateButton::Reposition(rcNavigateButtons);

}
void CXTPTabManager::CNavigateButtonArrowRight::Reposition(CRect& rcNavigateButtons)
{
	XTPResourceManager()->LoadString(&m_strToolTip, m_pManager->IsHorizontalPosition() ? XTP_IDS_TABNAVIGATEBUTTON_RIGHT : XTP_IDS_TABNAVIGATEBUTTON_DOWN);

	CRect rcHeaderMargin = m_pManager->GetPaintManager()->GetAppearanceSet()->GetHeaderMargin();

	m_bEnabled = m_pManager->GetItemsLength() + m_pManager->GetHeaderOffset() >
		m_pManager->GetRectLength(rcNavigateButtons) - (rcHeaderMargin.left + rcHeaderMargin.right) - 28;

	CNavigateButtonArrow::Reposition(rcNavigateButtons);
}

void CXTPTabManager::CNavigateButtonArrowRight::OnExecute(BOOL bTick)
{
	if (bTick)
	{
		m_pManager->OnScrollHeader(TRUE);
	}
}

void CXTPTabManager::OnScrollHeader(BOOL bRight)
{
	if (bRight)
	{
		SetHeaderOffset(GetHeaderOffset() - m_nScrollDelta);
	}
	else
	{
		SetHeaderOffset(GetHeaderOffset() + m_nScrollDelta);
	}
}

void CXTPTabManager::CNavigateButtonArrowLeft::DrawEntry(CDC* pDC, CRect rc)
{
	CPoint pt = rc.CenterPoint();

	if (m_pManager->IsHorizontalPosition())
		CXTPDrawHelpers::Triangle(pDC, CPoint(pt.x + 2, pt.y - 5), CPoint(pt.x - 2, pt.y - 1), CPoint(pt.x + 2, pt.y + 3));
	else
		CXTPDrawHelpers::Triangle(pDC, CPoint(pt.x - 5, pt.y + 2), CPoint(pt.x - 1, pt.y - 2), CPoint(pt.x + 3, pt.y + 2));
}

void CXTPTabManager::CNavigateButtonArrowLeft::Reposition(CRect& rcNavigateButtons)
{
	XTPResourceManager()->LoadString(&m_strToolTip, m_pManager->IsHorizontalPosition() ? XTP_IDS_TABNAVIGATEBUTTON_LEFT : XTP_IDS_TABNAVIGATEBUTTON_UP);

	m_bEnabled = m_pManager->GetHeaderOffset() < 0;

	CNavigateButtonArrow::Reposition(rcNavigateButtons);
}

void CXTPTabManager::CNavigateButtonArrowLeft::OnExecute(BOOL bTick)
{
	if (bTick)
	{
		m_pManager->OnScrollHeader(FALSE);
	}
}

//////////////////////////////////////////////////////////////////////
// CXTPTabManager

CXTPTabManager::CXTPTabManager()
{

	m_pSelected = 0;
	m_pHighlighted = 0;
	m_pPressed = 0;

	m_nHeaderOffset = 0;

	m_bAllowReorder = TRUE;

	m_bActive = TRUE;
	m_nScrollDelta = 20;

	m_rcHeaderRect.SetRectEmpty();
	m_rcControl.SetRectEmpty();
	m_rcClient.SetRectEmpty();

	m_bCloseItemButton = xtpTabNavigateButtonNone;

	m_pHighlightedNavigateButton = NULL;

	m_pRowIndexer = new CRowIndexer(this);

	m_arrNavigateButtons.Add(new CNavigateButtonArrowLeft(this, xtpTabNavigateButtonAutomatic));
	m_arrNavigateButtons.Add(new CNavigateButtonArrowRight(this, xtpTabNavigateButtonAutomatic));
	m_arrNavigateButtons.Add(new CNavigateButtonClose(this, xtpTabNavigateButtonNone));

	m_pMarkupContext = NULL;
}

CXTPTabManager::~CXTPTabManager()
{
	for (int i = 0; i < m_arrItems.GetSize(); i++)
	{
		CXTPTabManagerItem* pItem = m_arrItems[i];
		pItem->OnRemoved();
		pItem->InternalRelease();
	}

	m_arrNavigateButtons.RemoveAll();

	delete m_pRowIndexer;

	XTPMarkupReleaseContext(m_pMarkupContext);
}

void CXTPTabManager::EnableMarkup(BOOL bEnable)
{
	BOOL bMarkupContext = m_pMarkupContext != NULL;
	if (bMarkupContext == bEnable)
		return;

	for (int i = 0; i < m_arrItems.GetSize(); i++)
	{
		XTPMarkupReleaseElement(m_arrItems[i]->m_pMarkupUIElement);
	}

	XTPMarkupReleaseContext(m_pMarkupContext);

	if (bEnable)
	{
		m_pMarkupContext = XTPMarkupCreateContext();
	}
}


void CXTPTabManager::SetActive(BOOL bActive)
{
	if (m_bActive != bActive)
	{
		m_bActive = bActive;
		Reposition();
	}
}

BOOL CXTPTabManager::IsNavigateButtonVisible(CXTPTabManagerNavigateButton* pButton)
{
	if (pButton->GetFlags() == xtpTabNavigateButtonAutomatic)
		return IsNavigateButtonAutomaticVisible(pButton);

	if (pButton->GetFlags() == xtpTabNavigateButtonAlways)
	{
		if (pButton->GetID() == xtpTabNavigateButtonClose && pButton->GetItem())
		{
			return pButton->GetItem()->IsClosable();
		}
		return TRUE;
	}

	return FALSE;
}

BOOL CXTPTabManager::IsNavigateButtonAutomaticVisible(CXTPTabManagerNavigateButton* pButton)
{
	if (pButton->GetID() == xtpTabNavigateButtonClose)
	{
		if (pButton->GetItem())
		{
			return pButton->GetItem()->IsSelected() ?  pButton->GetItem()->IsClosable() : FALSE;
		}
		return m_pSelected ? m_pSelected->IsClosable() : FALSE;
	}

	return TRUE;
}

void CXTPTabManager::EnableTabThemeTexture(HWND hWnd, BOOL bEnable)
{
	CXTPWinThemeWrapper themeTabControl;  // Internal helper for drawing XP interface parts.
	themeTabControl.OpenTheme(hWnd, L"TAB");
	themeTabControl.EnableThemeDialogTexture(hWnd, bEnable ? ETDT_ENABLETAB : ETDT_DISABLE | ETDT_USETABTEXTURE);
}

CXTPTabManagerNavigateButton* CXTPTabManager::FindNavigateButton(UINT nID) const
{
	for (int i = 0; i < m_arrNavigateButtons.GetSize(); i++)
	{
		if (m_arrNavigateButtons[i]->m_nID == nID)
			return m_arrNavigateButtons[i];
	}

	return NULL;
}

void CXTPTabManager::SetHeaderOffset(int nOffset)
{
	if (nOffset > 0) nOffset = 0;

	if (nOffset != m_nHeaderOffset)
	{
		m_nHeaderOffset = nOffset;
		Reposition();
	}
}

int CXTPTabManager::GetItemsLength() const
{
	int nLength = 0;

	if (GetLayout() == xtpTabLayoutRotated)
	{
		nLength = GetPaintManager()->GetAppearanceSet()->GetButtonHeight(this)
			* GetItemCount();
	}
	else
	{
		for (int i = 0; i < GetItemCount(); i++)
			nLength += GetItem(i)->GetButtonLength();
	}

	return nLength;
}


void CXTPTabManager::DeleteAllItems()
{
	for (int i = 0; i < m_arrItems.GetSize(); i++)
	{
		CXTPTabManagerItem* pItem = m_arrItems[i];
		pItem->OnRemoved();
		pItem->InternalRelease();
	}

	m_arrItems.RemoveAll();

	m_pHighlighted = m_pSelected = m_pPressed = NULL;

	OnItemsChanged();
}

BOOL CXTPTabManager::DeleteItem(int nItem)
{
	if (nItem < 0 || nItem >= GetItemCount())
		return FALSE;

	CXTPTabManagerItem* pItem = m_arrItems[nItem];

	BOOL bSelected = (m_pSelected == pItem);

	if (m_pHighlighted == pItem)
		m_pHighlighted = NULL;

	m_arrItems.RemoveAt(nItem);

	pItem->OnRemoved();
	pItem->InternalRelease();

	if (bSelected)
	{
		SetCurSel(nItem);
	}


	OnItemsChanged();

	return TRUE;
}

CXTPTabManagerItem* CXTPTabManager::AddItem(int nItem, CXTPTabManagerItem* pItem /*= NULL*/)
{
	if (!pItem)
		pItem = new CXTPTabManagerItem();

	pItem->m_pTabManager = this;

	if (nItem < 0 || nItem > GetItemCount())
		nItem = GetItemCount();

	m_arrItems.InsertAt(nItem, pItem);
	pItem->m_clrItem = xtpTabColorBlue + (GetPaintManager()->m_nItemColor++ % 8);

	if (m_bCloseItemButton != xtpTabNavigateButtonNone)
	{
		pItem->m_arrNavigateButtons.Add(new CNavigateButtonTabClose(pItem, m_bCloseItemButton));
	}

	OnItemsChanged();

	return pItem;
}

void CXTPTabManager::OnItemsChanged()
{
	for (int i = 0; i < GetItemCount(); i++)
	{
		GetItem(i)->m_nIndex = i;
	}
	Reposition();
}

CString CXTPTabManager::GetItemCaption(const CXTPTabManagerItem* pItem) const
{
	return pItem->m_strCaption;
}

HICON CXTPTabManager::GetItemIcon(const CXTPTabManagerItem* pItem) const
{
	return pItem->m_hIcon;
}

void CXTPTabManager::ShowIcons(BOOL bShowIcons)
{
	GetPaintManager()->m_bShowIcons = bShowIcons;
	Reposition();
}

COLORREF CXTPTabManager::GetItemColor(const CXTPTabManagerItem* pItem) const
{
	COLORREF nColor = pItem->m_clrItem;

	if (nColor >= xtpTabColorBlue && nColor <= xtpTabColorMagenta)
		return CXTPTabPaintManager::GetOneNoteColor((XTPTabOneNoteColor)nColor);

	return nColor;
}

void CXTPTabManager::SetCurSel(int nIndex)
{
	if (GetItemCount() != 0)
	{
		nIndex = nIndex < 0 ? 0: nIndex >= GetItemCount() ? GetItemCount() - 1 : nIndex;

		SetSelectedItem(GetItem(nIndex));
	}
	else
	{
		SetSelectedItem(NULL);
	}
}

int CXTPTabManager::GetCurSel() const
{
	if (m_pSelected)
	{
		return m_pSelected->GetIndex();
	}
	return -1;
}

void CXTPTabManager::SetSelectedItem(CXTPTabManagerItem* pItem)
{
	if (m_pSelected != pItem)
	{
		m_pSelected = pItem;
		Reposition();
		EnsureVisible(pItem);
	}
}

void CXTPTabManager::EnsureVisible(CXTPTabManagerItem* pItem)
{
	if (!pItem)
		return;

	GetPaintManager()->EnsureVisible(this, pItem);
}

void CXTPTabManager::SetPosition(XTPTabPosition tabPosition)
{
	GetPaintManager()->m_tabPosition = tabPosition;
	Reposition();
}

void CXTPTabManager::SetLayoutStyle(XTPTabLayoutStyle tabLayout)
{
	GetPaintManager()->m_tabLayout = tabLayout;
	Reposition();
}


CXTPTabManagerItem* CXTPTabManager::HitTest(CPoint point) const
{
	if (!m_rcControl.PtInRect(point))
		return NULL;

	if (!m_rcHeaderRect.IsRectEmpty() && !m_rcHeaderRect.PtInRect(point))
		return NULL;

	for (int i = 0; i < GetItemCount(); i++)
	{
		CXTPTabManagerItem* pItem = GetItem(i);

		if (pItem->GetRect().PtInRect(point) && pItem->IsEnabled() && pItem->IsVisible())
		{
			return pItem;
		}
	}
	return NULL;
}

XTPTabPosition CXTPTabManager::GetPosition() const
{
	return GetPaintManager()->m_tabPosition;
}

XTPTabLayoutStyle CXTPTabManager::GetLayout() const
{
	return GetPaintManager()->m_tabLayout;
}

void CXTPTabManager::MoveItem(CXTPTabManagerItem* pItem, int nIndex)
{
	ASSERT(pItem && pItem->GetTabManager() == this);

	if (!pItem || pItem->GetTabManager() != this)
		return;

	int nOldIndex = pItem->GetIndex();
	if (nOldIndex == nIndex)
		return;

	ASSERT(nOldIndex >= 0);
	ASSERT(nIndex >= 0 && nIndex < GetItemCount());

	if (nIndex < 0 || nIndex >= GetItemCount())
		nIndex = GetItemCount() - 1;

	m_arrItems.RemoveAt(nOldIndex);
	m_arrItems.InsertAt(nIndex, pItem);

	OnItemsChanged();
}

void CXTPTabManager::TrackClick(HWND hWnd, CPoint pt, CXTPTabManagerItem* pItem)
{
	if (GetPaintManager()->m_bHotTracking)
		m_pHighlighted = pItem;

	m_pPressed = pItem;
	BOOL bHighlighted = TRUE;

	Reposition();
	BOOL bAccept = FALSE;

	::SetCapture(hWnd);

	while (::GetCapture() == hWnd)
	{
		MSG msg;
		VERIFY(::GetMessage(&msg, NULL, 0, 0));

		if (::GetCapture() != hWnd)
		{
			DispatchMessage (&msg);
			break;
		}
		switch (msg.message)
		{
			case WM_MOUSEMOVE:
				{
					pt = CPoint((short signed)LOWORD(msg.lParam), (short signed)HIWORD(msg.lParam));

					bHighlighted = pItem->GetRect().PtInRect(pt);

					if (GetPaintManager()->m_bHotTracking)
					{
						CXTPTabManagerItem* pHighlighted = bHighlighted ? pItem : NULL;
						if (pHighlighted != m_pHighlighted)
						{
							m_pHighlighted = pHighlighted;
							RedrawControl(pItem->GetRect(), TRUE);
						}
					}

				}

				break;

			case WM_KEYDOWN:
				if (msg.wParam != VK_ESCAPE)
					break;
			case WM_CANCELMODE:
			case WM_RBUTTONDOWN:
				goto ExitLoop;

			case WM_LBUTTONUP:
				bAccept = TRUE;
				goto ExitLoop;

			default:
				DispatchMessage (&msg);
				break;
		}
	}

ExitLoop:
	ReleaseCapture();

	m_pPressed = FALSE;

	PerformMouseMove(hWnd, pt);

	RedrawControl(NULL, FALSE);

	if (bAccept && bHighlighted)
	{
		OnItemClick(pItem);
	}
}

void CXTPTabManager::ReOrder(HWND hWnd, CPoint pt, CXTPTabManagerItem* pItem)
{
	CXTPTabManagerItem* pSelected = m_pSelected;
	m_pSelected = pItem;

	if (GetPaintManager()->m_bHotTracking)
		m_pHighlighted = pItem;

	Reposition();

	CArray<CRect, CRect&> arrRects;

	for (int j = 0; j < GetItemCount(); j++)
	{
		CRect rc = GetItem(j)->GetRect();
		arrRects.Add(rc);
	}

	int nIndex = pItem->GetIndex();

	::SetCapture(hWnd);

	while (::GetCapture() == hWnd)
	{
		MSG msg;
		VERIFY(::GetMessage(&msg, NULL, 0, 0));

		if (::GetCapture() != hWnd)
		{
			DispatchMessage (&msg);
			break;
		}
		switch (msg.message)
		{
			case WM_MOUSEMOVE:
				{
					pt = CPoint((short signed)LOWORD(msg.lParam), (short signed)HIWORD(msg.lParam));

					for (int i = 0; i < arrRects.GetSize(); i++)
					{
						if (i != pItem->GetIndex() && arrRects[i].PtInRect(pt))
						{

							while (i != pItem->GetIndex())
							{
								int j = i < pItem->GetIndex() ? pItem->GetIndex() - 1 : pItem->GetIndex() + 1;

								CXTPTabManagerItem* p1 = pItem;
								CXTPTabManagerItem* p2 = m_arrItems[pItem->GetIndex()] = m_arrItems[j];
								m_arrItems[j] = p1;

								OnItemsChanged();

								OnSwitchItem(p1, p2);
							}
							break;
						}
					}
				}

				break;

			case WM_KEYDOWN:
				if (msg.wParam != VK_ESCAPE)
					break;
			case WM_CANCELMODE:
			case WM_LBUTTONUP:
			case WM_RBUTTONDOWN:
				goto ExitLoop;

			default:
				DispatchMessage (&msg);
				break;
		}
	}

ExitLoop:
	ReleaseCapture();
	PerformMouseMove(hWnd, pt);

	m_pSelected = pSelected;
	OnItemClick(pItem);

	if (nIndex != pItem->GetIndex())
	{
		OnItemOrderChanged(pItem, nIndex, pItem->GetIndex());
	}
}

void CXTPTabManager::OnItemClick(CXTPTabManagerItem* pItem)
{
	SetSelectedItem(pItem);
}

void CXTPTabManager::SetFocusedItem(CXTPTabManagerItem* pItem)
{
	if (!OnBeforeItemClick(pItem))
		return;

	if (pItem)
	{
		OnItemClick(pItem);
	}
}

CXTPTabManagerItem* CXTPTabManager::FindNextFocusable(int nIndex, int nDirection) const
{
	CXTPTabManagerItem* pItem = NULL;

	do
	{
		nIndex += nDirection;

		pItem = GetItem(nIndex);
		if (!pItem)
			return NULL;

	}
	while (!(pItem->IsVisible() && pItem->IsEnabled()));

	return pItem;
}

BOOL CXTPTabManager::PerformKeyDown(HWND hWnd, UINT nChar)
{
	const int nCount = GetItemCount();
	if (nCount < 1)
		return FALSE;

	if (nChar == VK_LEFT && (DWORD)GetWindowLong(hWnd, GWL_EXSTYLE) & WS_EX_LAYOUTRTL)
		nChar = VK_RIGHT;
	else if (nChar == VK_RIGHT && (DWORD)GetWindowLong(hWnd, GWL_EXSTYLE) & WS_EX_LAYOUTRTL)
		nChar = VK_LEFT;

	switch (nChar)
	{
	case VK_HOME:
		SetFocusedItem(FindNextFocusable(-1, +1));
		return TRUE;

	case VK_END:
		SetFocusedItem(FindNextFocusable(nCount, -1));
		return TRUE;

	case VK_LEFT:
		if (IsHorizontalPosition() && m_pSelected && m_pSelected->GetIndex() > 0)
			SetFocusedItem(FindNextFocusable(m_pSelected->GetIndex(), -1));
		return TRUE;

	case VK_UP:
		if (!IsHorizontalPosition() && m_pSelected && m_pSelected->GetIndex() > 0)
			SetFocusedItem(FindNextFocusable(m_pSelected->GetIndex(), -1));
		return TRUE;

	case VK_RIGHT:
		if (IsHorizontalPosition() && m_pSelected && m_pSelected->GetIndex() < nCount - 1)
			SetFocusedItem(FindNextFocusable(m_pSelected->GetIndex(), + 1));
		return TRUE;

	case VK_DOWN:
		if (!IsHorizontalPosition() && m_pSelected && m_pSelected->GetIndex() < nCount - 1)
			SetFocusedItem(FindNextFocusable(m_pSelected->GetIndex(), + 1));
		return TRUE;
	}
	return FALSE;
}

BOOL CXTPTabManager::OnBeforeItemClick(CXTPTabManagerItem* /*pItem*/)
{
	return TRUE;
}


CXTPTabManagerNavigateButton* CXTPTabManager::HitTestNavigateButton(CPoint point, BOOL bHeaderOnly, int* pnIndex) const
{
	int i;
	for (i = 0; i < (int)m_arrNavigateButtons.GetSize(); i++)
	{
		CXTPTabManagerNavigateButton* pButton = m_arrNavigateButtons[i];
		if (pButton->m_rcButton.PtInRect(point))
		{
			if (!pButton->IsEnabled())
				return NULL;

			if (pnIndex)
			{
				*pnIndex = i;
			}
			return pButton;
		}
	}

	if (bHeaderOnly)
		return NULL;

	CXTPTabManagerItem* pItem = HitTest(point);
	if (!pItem)
		return NULL;

	for (i = 0; i < (int)pItem->GetNavigateButtons()->GetSize(); i++)
	{
		CXTPTabManagerNavigateButton* pButton = pItem->GetNavigateButtons()->GetAt(i);
		if (pButton->m_rcButton.PtInRect(point))
		{
			if (!pButton->IsEnabled())
				return NULL;

			if (pnIndex)
			{
				*pnIndex = i;
			}
			return pButton;
		}
	}

	return NULL;
}

BOOL CXTPTabManager::PerformClick(HWND hWnd, CPoint pt, BOOL bNoItemClick)
{
	CXTPTabManagerNavigateButton* pNavigateButton = HitTestNavigateButton(pt, FALSE);
	if (pNavigateButton)
	{
		pNavigateButton->PerformClick(hWnd, pt);
		return TRUE;
	}

	if (bNoItemClick)
		return FALSE;

	CXTPTabManagerItem* pItem = HitTest(pt);

	if (pItem)
	{
		if (!OnBeforeItemClick(pItem))
			return FALSE;

		if (IsAllowReorder())
		{
			ReOrder(hWnd, pt, pItem);
		}
		else if (GetPaintManager()->m_bSelectOnButtonDown)
		{
			OnItemClick(pItem);
		}
		else
		{
			TrackClick(hWnd, pt, pItem);
		}
		return TRUE;
	}

	return FALSE;
}


void CXTPTabManager::PerformMouseMove(HWND hWnd, CPoint pt)
{
	CXTPTabPaintManagerTheme* pAppearance = GetPaintManager()->GetAppearanceSet();

	if (!CXTPDrawHelpers::IsTopParentActive(hWnd) || IsMouseLocked())
	{
		if (m_pHighlighted)
		{
			CRect rcRedraw(pAppearance->GetButtonDrawRect(m_pHighlighted));
			m_pHighlighted = NULL;

			RedrawControl(rcRedraw, TRUE);
		}
		return;
	}

	if (GetPaintManager()->m_bHotTracking)
	{
		CXTPTabManagerItem* pItem = HitTest(pt);

		if (pItem != m_pHighlighted)
		{
			if (m_pHighlighted)
			{
				CRect rcRedraw(pAppearance->GetButtonDrawRect(m_pHighlighted));
				m_pHighlighted = NULL;

				RedrawControl(rcRedraw, TRUE);
			}

			m_pHighlighted = pItem;

			if (m_pHighlighted)
			{
				RedrawControl(pAppearance->GetButtonDrawRect(m_pHighlighted), FALSE);
			}

			if (pItem)
			{
				TRACKMOUSEEVENT tme =
				{
					sizeof(TRACKMOUSEEVENT), TME_LEAVE, hWnd
				};
				_TrackMouseEvent(&tme);
			}
		}
	}

	CXTPTabManagerNavigateButton* pNavigateButton = HitTestNavigateButton(pt, FALSE);

	if (pNavigateButton != m_pHighlightedNavigateButton)
	{
		if (m_pHighlightedNavigateButton)
		{
			RedrawControl(m_pHighlightedNavigateButton->GetRect(), TRUE);
		}

		m_pHighlightedNavigateButton = pNavigateButton;

		if (m_pHighlightedNavigateButton)
		{
			RedrawControl(m_pHighlightedNavigateButton->GetRect(), FALSE);
		}

		if (pNavigateButton)
		{
			TRACKMOUSEEVENT tme =
			{
				sizeof(TRACKMOUSEEVENT), TME_LEAVE, hWnd
			};
			_TrackMouseEvent(&tme);
		}
	}
}

CString CXTPTabManager::GetItemTooltip(const CXTPTabManagerItem* pItem) const
{
	return pItem->m_strToolTip;
}

INT_PTR CXTPTabManager::PerformToolHitTest(HWND hWnd, CPoint point, TOOLINFO* pTI) const
{
	if (IsMouseLocked())
		return -1;

	int nIndex = -1;
	CXTPTabManagerNavigateButton* pNavigateButton = HitTestNavigateButton(point, FALSE, &nIndex);
	if (pNavigateButton)
	{
		ASSERT(nIndex != -1);
		CString strTip = pNavigateButton->GetTooltip();
		if (strTip.IsEmpty())
			return -1;

		CXTPToolTipContext::FillInToolInfo(pTI, hWnd, pNavigateButton->GetRect(), nIndex, strTip);

		return nIndex;
	}

	CXTPTabManagerItem* pItem = HitTest(point);

	if (pItem)
	{
		if (GetPaintManager()->m_toolBehaviour == xtpTabToolTipNever)
			return -1;

		if (GetPaintManager()->m_toolBehaviour == xtpTabToolTipShrinkedOnly && !pItem->IsItemShrinked())
			return -1;

		CString strTip = GetItemTooltip(pItem);
		if (strTip.IsEmpty())
			return -1;

		CXTPToolTipContext::FillInToolInfo(pTI, hWnd, pItem->GetRect(), pItem->GetIndex(), strTip, pItem->GetCaption(), strTip);

		return pItem->GetIndex();
	}
	return -1;
}

void CXTPTabManager::GetItemMetrics(CSize* lpszNormal, CSize* lpszMin /*= NULL*/, CSize* lpszMax /*= NULL*/) const
{
	CXTPTabPaintManager* pPaintManager = GetPaintManager();
	if (lpszNormal) *lpszNormal = CSize(pPaintManager->m_nFixedTabWidth, 0);
	if (lpszMin) *lpszMin = CSize(pPaintManager->m_nMinTabWidth, 0);
	if (lpszMax) *lpszMax = CSize(pPaintManager->m_nMaxTabWidth, 0);
}

void CXTPTabManager::SetItemMetrics(CSize szNormal, CSize szMin /*= CSize(0, 0)*/, CSize szMax /*= CSize(0, 0)*/)
{
	CXTPTabPaintManager* pPaintManager = GetPaintManager();
	pPaintManager->m_nFixedTabWidth = szNormal.cx;
	pPaintManager->m_nMinTabWidth = szMin.cx;
	pPaintManager->m_nMaxTabWidth = szMax.cx;

	Reposition();
}

BOOL CXTPTabManager::IsDrawStaticFrame() const
{
	return GetPaintManager()->m_bStaticFrame;
}


CXTPTabPaintManagerColorSet* CXTPTabManager::SetColor(XTPTabColorStyle tabColor)
{
	CXTPTabPaintManagerColorSet* pColorSet = GetPaintManager()->SetColor(tabColor);
	Reposition();
	return pColorSet;
}

CXTPTabPaintManagerColorSet* CXTPTabManager::SetColorSet(CXTPTabPaintManagerColorSet* pColorSet)
{
	GetPaintManager()->SetColorSet(pColorSet);
	Reposition();
	return pColorSet;
}
CXTPTabPaintManagerTheme* CXTPTabManager::SetAppearance(XTPTabAppearanceStyle tabAppearance)
{
	CXTPTabPaintManagerTheme* pAppearanceSet = GetPaintManager()->SetAppearance(tabAppearance);
	Reposition();
	return pAppearanceSet;
}
CXTPTabPaintManagerTheme* CXTPTabManager::SetAppearanceSet(CXTPTabPaintManagerTheme* pAppearanceSet)
{
	GetPaintManager()->SetAppearanceSet(pAppearanceSet);
	Reposition();
	return pAppearanceSet;
}

CXTPTabPaintManagerTheme* CXTPTabManager::GetAppearanceSet() const
{
	return GetPaintManager()->GetAppearanceSet();
}
CXTPTabPaintManagerColorSet* CXTPTabManager::GetColorSet() const

{
	return GetPaintManager()->GetColorSet();
}

XTPTabAppearanceStyle CXTPTabManager::GetAppearance() const
{
	return GetPaintManager()->GetAppearance();
}

XTPTabColorStyle CXTPTabManager::GetColor() const
{
	return GetPaintManager()->GetColor();
}

#ifdef _XTP_ACTIVEX

BEGIN_DISPATCH_MAP(CXTPTabManagerItem, CCmdTarget)
	//{{AFX_DISPATCH_MAP(CXTPTabManagerItem)
	DISP_PROPERTY_EX_ID(CXTPTabManagerItem, "Caption", 1, OleGetCaption, OleSetCaption, VT_BSTR)
	DISP_PROPERTY_EX_ID(CXTPTabManagerItem, "Index", 2, OleGetIndex, SetNotSupported, VT_I4)
	DISP_PROPERTY_ID(CXTPTabManagerItem, "Tag", 3, m_oleTag, VT_VARIANT)
	DISP_PROPERTY_EX_ID(CXTPTabManagerItem, "Selected", 4, OleGetSelected, OleSetSelected, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPTabManagerItem, "Image", 5, OleGetImage, OleSetImage, VT_I4)
	DISP_PROPERTY_EX_ID(CXTPTabManagerItem, "Color", 6, GetColor, SetColor, VT_COLOR)
	DISP_PROPERTY_EX_ID(CXTPTabManagerItem, "Handle", 7, GetHandle, OleSetHandle, VT_HANDLE)
	DISP_PROPERTY_EX_ID(CXTPTabManagerItem, "Visible", 8, IsVisible, OleSetVisible, VT_BOOL)
	DISP_PROPERTY_EX_ID(CXTPTabManagerItem, "Enabled", 9, IsEnabled, SetEnabled, VT_BOOL)
	DISP_PROPERTY_ID(CXTPTabManagerItem, "ToolTip", 10, m_strToolTip, VT_BSTR)
	DISP_FUNCTION_ID(CXTPTabManagerItem, "Move", 11, Move, VT_EMPTY, VTS_I4)
	DISP_PROPERTY_NOTIFY_ID(CXTPTabManagerItem, "Closable", 12, m_bClosable, Reposition, VT_BOOL)
	DISP_FUNCTION_ID(CXTPTabManagerItem, "GetRect", 13, OleGetRect, VT_EMPTY, VTS_PI4 VTS_PI4 VTS_PI4 VTS_PI4)
	DISP_FUNCTION_ID(CXTPTabManagerItem, "EnsureVisible", 14, OleEnsureVisible, VT_EMPTY, VTS_NONE)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_ITabControlItem to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the
//  dispinterface in the .ODL file.

// {CCC528A8-9479-48D5-A14D-077A9FD5D276}
static const IID IID_ITabControlItem =
{ 0xccc528a8, 0x9479, 0x48d5, { 0xa1, 0x4d, 0x7, 0x7a, 0x9f, 0xd5, 0xd2, 0x76 } };

BEGIN_INTERFACE_MAP(CXTPTabManagerItem, CCmdTarget)
	INTERFACE_PART(CXTPTabManagerItem, IID_ITabControlItem, Dispatch)
END_INTERFACE_MAP()

IMPLEMENT_OLETYPELIB_EX(CXTPTabManagerItem, IID_ITabControlItem)

/////////////////////////////////////////////////////////////////////////////
// CXTPTabManagerItem message handlers

BSTR CXTPTabManagerItem::OleGetCaption()
{
	CString strCaption = GetCaption() ;

	return strCaption.AllocSysString();
}

void CXTPTabManagerItem::OleSetCaption(LPCTSTR lpszNewValue)
{
	SetCaption(lpszNewValue);
}

long CXTPTabManagerItem::OleGetIndex()
{
	return GetIndex();
}

long CXTPTabManagerItem::OleGetTag()
{
	return (long)GetData();
}

void CXTPTabManagerItem::OleSetTag(long nNewValue)
{
	SetData((DWORD)nNewValue);

}

BOOL CXTPTabManagerItem::OleGetSelected()
{
	return IsSelected();
}

void CXTPTabManagerItem::OleSetSelected(BOOL bNewValue)
{
	if (bNewValue)
	{
		Select();
	}

}

void CXTPTabManagerItem::OleSetVisible(BOOL bVisible)
{
	SetVisible(bVisible);
}

long CXTPTabManagerItem::OleGetImage()
{
	return GetImageIndex();
}

void CXTPTabManagerItem::OleSetImage(long nNewValue)
{
	SetImageIndex(nNewValue);
}

void CXTPTabManagerItem::OleSetHandle(long hwndChild)
{
	HWND hWndOld = m_hWnd;

	if (m_hWnd)
	{
		ShowWindow(m_hWnd, SW_HIDE);
	}


	m_hWnd = (HWND)hwndChild;

	m_pTabManager->OnItemHandleChanged(this, hWndOld, m_hWnd);
}

void CXTPTabManagerItem::OleGetRect(long* Left, long* Top, long* Right, long* Bottom)
{
	if (Left) *Left = m_rcItem.left;
	if (Top) *Top = m_rcItem.top;
	if (Right) *Right = m_rcItem.right;
	if (Bottom) *Bottom = m_rcItem.bottom;
}

void CXTPTabManagerItem::OleEnsureVisible()
{
	m_pTabManager->EnsureVisible(this);
}

#endif
