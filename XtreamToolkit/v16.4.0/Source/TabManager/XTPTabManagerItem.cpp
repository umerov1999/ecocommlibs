#include "StdAfx.h"

#include <Common/XTPImageManager.h>
#include <Common/XTPMarkupRender.h>
#include <Common/XTPColorManager.h>

#include <TabManager/XTPTabManagerDefines.h>
#include <TabManager/XTPTabManagerAtom.h>
#include <TabManager/XTPTabManagerNavigateButton.h>
#include <TabManager/XTPTabManagerNavigateButtons.h>
#include <TabManager/XTPTabManagerItem.h>
#include <TabManager/XTPTabManager.h>
#include <TabManager/XTPTabPaintManagerColorSet.h>
#include <TabManager/XTPTabPaintManagerTheme.h>
#include <TabManager/XTPTabPaintManager.h>


IMPLEMENT_DYNAMIC(CXTPTabManagerItem, CCmdTarget)

//////////////////////////////////////////////////////////////////////
// CXTPTabManagerItem

CXTPTabManagerItem::CXTPTabManagerItem()
{
	EnableAutomation();

#ifdef _XTP_ACTIVEX
	EnableTypeLib();
#endif

	m_bVisible = TRUE;
	m_bEnabled = TRUE;


	m_hIcon = 0;
	m_hWnd = 0;
	m_clrItem = 0;
	m_nIndex = -1;
	m_nImage = -1;
	m_dwData = 0;
	m_rcItem.SetRectEmpty();
	m_nItemRow = 0;
	m_nButtonLength = m_nContentLength = 0;
	m_bClosable = TRUE;
	m_pTabManager = NULL;
	m_bFound = FALSE;

	m_pMarkupUIElement = 0;
}

void CXTPTabManagerItem::OnRemoved()
{
	m_arrNavigateButtons.RemoveAll();
}


CXTPTabManagerItem::~CXTPTabManagerItem()
{
	m_arrNavigateButtons.RemoveAll();

	XTPMarkupReleaseElement(m_pMarkupUIElement);
}



void CXTPTabManagerItem::Reposition()
{
	if (m_pTabManager) m_pTabManager->Reposition();
}

void CXTPTabManagerItem::SetCaption(LPCTSTR lpszCaption)
{
	CString strCaption(lpszCaption);

	if (m_strCaption != strCaption)
	{
		m_strCaption = strCaption;

		XTPMarkupReleaseElement(m_pMarkupUIElement);
		if (m_pTabManager && m_pTabManager->GetMarkupContext())
		{
			m_pMarkupUIElement = XTPMarkupParseText(m_pTabManager->GetMarkupContext(), lpszCaption);
		}

		Reposition();
	}
}

void CXTPTabManagerItem::SetColor(COLORREF clr)
{
	if (m_clrItem != clr)
	{
		m_clrItem = clr;
		m_pTabManager->RedrawControl(GetRect(), FALSE);
	}
}

void CXTPTabManagerItem::SetClosable(BOOL bClosable)
{
	m_bClosable = bClosable;

	Reposition(); // shows/hides the close button
}

CString CXTPTabManagerItem::GetCaption() const
{
	return m_pTabManager->GetItemCaption(this);
}

CString CXTPTabManagerItem::GetTooltip() const
{
	return m_pTabManager->GetItemTooltip(this);
}

COLORREF CXTPTabManagerItem::GetColor()
{
	return m_pTabManager->GetItemColor(this);
}

void CXTPTabManagerItem::SetHandle(HWND hWnd)
{
	m_hWnd = hWnd;
}

void CXTPTabManagerItem::Remove()
{
	if (m_pTabManager) m_pTabManager->DeleteItem(m_nIndex);
}

void CXTPTabManagerItem::Select()
{
	if (m_pTabManager) m_pTabManager->OnItemClick(this);
}

HWND CXTPTabManagerItem::GetHandle() const
{
	return m_hWnd;
}

BOOL CXTPTabManagerItem::IsVisible() const
{
	return m_bVisible;
}

void CXTPTabManagerItem::SetVisible(BOOL bVisible)
{
	if (m_bVisible != bVisible)
	{
		m_bVisible = bVisible;
		Reposition();
	}
}

BOOL CXTPTabManagerItem::IsEnabled() const
{
	return m_bEnabled;
}

void CXTPTabManagerItem::SetEnabled(BOOL bEnabled)
{
	if (m_bEnabled != bEnabled)
	{
		m_bEnabled = bEnabled;
		Reposition();
	}
}

void CXTPTabManagerItem::Move(int nIndex)
{
	if (GetIndex() != nIndex)
	{
		m_pTabManager->MoveItem(this, nIndex);
	}
}

BOOL CXTPTabManagerItem::DrawRotatedImage(CDC* pDC, CRect rcItem, CXTPImageManagerIcon* pImage)
{
	CXTPImageManagerIconHandle& imageHandle = !IsEnabled() ? pImage->GetDisabledIcon(): IsHighlighted() ? pImage->GetHotIcon() :
		IsSelected() ? pImage->GetCheckedIcon() : pImage->GetIcon();

	const int cx = rcItem.Width();
	const int cy = rcItem.Height();
	ASSERT(cx == cy);
	if (cx != cy)
		return FALSE;

	UINT* pSrcBits = NULL, *pDestBits = NULL;
	HBITMAP hbmSrc = CXTPImageManager::Create32BPPDIBSection(NULL, cx, cy, (LPBYTE*)&pSrcBits);
	if (!pSrcBits)
		return FALSE;

	HBITMAP hbmDest = CXTPImageManager::Create32BPPDIBSection(NULL, cx, cy, (LPBYTE*)&pDestBits);
	if (!pDestBits)
		return FALSE;

	CDC dc;
	dc.CreateCompatibleDC(NULL);
	HGDIOBJ hbmpOld = ::SelectObject(dc, hbmSrc);

	CRect rcDraw(0, 0, cx, cy);

	dc.BitBlt(0, 0, cx, cy, pDC, rcItem.left, rcItem.top, SRCCOPY);
	::SelectObject(dc, hbmpOld);

	UINT* pSrcInv = pDestBits;
	UINT* pDestInv = pSrcBits;

	UINT* pDest = &pDestBits[cx];
	int i;

	for (i = 0; i < cy; i++)
	{
		pDest -= 1;
		pDestBits = pDest;
		for (int j = 0; j < cx; j++)
		{
			*pDestBits = *pSrcBits;
			pSrcBits += 1;
			pDestBits += cy;
		}
	}


	hbmpOld = ::SelectObject(dc, hbmDest);
	pImage->Draw(&dc, rcDraw.TopLeft(), imageHandle, rcDraw.Size());
	::SelectObject(dc, hbmpOld);

	pDest = &pDestInv[cx * (cy - 1)];

	for (i = 0; i < cy; i++)
	{
		pDestInv = pDest;
		for (int j = 0; j < cx; j++)
		{
			*pDestInv = *pSrcInv;
			pSrcInv += 1;
			pDestInv -= cy;
		}
		pDest += 1;
	}

	pDC->DrawState(rcItem.TopLeft(), rcItem.Size(), hbmSrc, DST_BITMAP);

	DeleteObject(hbmSrc);
	DeleteObject(hbmDest);

	return TRUE;
}

void CXTPTabManagerItem::DrawImage(CDC* pDC, CRect rcIcon, CXTPImageManagerIcon* pImage)
{
	if (rcIcon.Width() == rcIcon.Height() && GetTabManager()->GetPaintManager()->m_bRotateImageOnVerticalDraw
		&& (GetTabManager()->GetPosition() == xtpTabPositionLeft || GetTabManager()->GetPosition() == xtpTabPositionRight)
		&& DrawRotatedImage(pDC, rcIcon, pImage))
	{

	}
	else
	{
		CXTPImageManagerIconHandle& imageHandle = !IsEnabled() ? pImage->GetDisabledIcon(): IsHighlighted() ? pImage->GetHotIcon() :
			IsSelected() ? pImage->GetCheckedIcon() : pImage->GetIcon();

		pImage->Draw(pDC, rcIcon.TopLeft(), imageHandle, rcIcon.Size());
	}
}

void CXTPTabManagerItem::SetRect(CRect rcItem)
{
	m_rcItem = rcItem;

	for (int i = (int)m_arrNavigateButtons.GetSize() - 1; i >= 0; i--)
	{
		CXTPTabManagerNavigateButton* pButton = m_arrNavigateButtons.GetAt(i);
		pButton->SetRect(CRect(0, 0, 0, 0));
	}
}

BOOL CXTPTabManagerItem::IsHighlighted() const
{
	return m_pTabManager->m_pHighlighted == this;
}

BOOL CXTPTabManagerItem::IsPressed() const
{
	return m_pTabManager->m_pPressed == this;
}

void CXTPTabManagerItem::SetImageIndex(int nImage)
{
	if (m_nImage != nImage)
	{
		m_nImage = nImage; m_pTabManager->Reposition();
	}
}
