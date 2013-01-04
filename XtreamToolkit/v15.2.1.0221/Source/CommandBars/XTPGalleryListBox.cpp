// XTPGalleryListBox.cpp : implementation file
//
// This file is a part of the XTREME RIBBON MFC class library.
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

#include "Common/XTPIntel80Helpers.h"
#include "Common/XTPImageManager.h"
#include "Common/XTPSystemHelpers.h"
#include "Common/XTPHookManager.h"
#include "Common/XTPColorManager.h"
#include "Common/XTPDrawHelpers.h"
#include "Common/XTPToolTipContext.h"

#include "Ribbon/Resource.h"
#include "XTPCommandBarsDefines.h"
#include "XTPCommandBars.h"
#include "XTPCommandBar.h"
#include "XTPControls.h"
#include "XTPControl.h"
#include "XTPControlButton.h"
#include "XTPControlPopup.h"
#include "XTPScrollBase.h"
#include "XTPControlGallery.h"
#include "XTPGalleryListBox.h"
#include "XTPPaintManager.h"
#include "XTPRecentFileList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define ID_ICON_PINNED    1000
#define ID_ICON_UNPINNED  1001

class CXTPGalleryListBoxItems : public CXTPControlGalleryItems
{
public:
	CXTPGalleryListBoxItems(CXTPGalleryListBox* pParent)
		: CXTPControlGalleryItems(NULL)
	{
		m_pParent = pParent;
	}
	void OnItemsChanged()
	{
		CXTPControlGallery* pGallery = m_pParent->GetGallery();
		pGallery->OnItemsChanged();
	}

	CXTPGalleryListBox* GetListBox() const {
		return m_pParent;
	}

protected:
	CXTPGalleryListBox* m_pParent;
};

class CXTPControlListBoxGallery : public CXTPControlGallery
{
public:
	CXTPControlListBoxGallery(CXTPGalleryListBox* pParent)
	{
		m_pParent = pParent;
	}
	virtual CXTPControlGalleryItems* GetItems() const
	{
		return m_pParent->GetItems();
	}

	void OnExecute()
	{
		CXTPControlGalleryItem* pItem = GetItem(GetSelectedItem());
		if (pItem)
		{
			if (pItem->OnClick())
				return;
		}

		m_pParent->OnExecute();

	}


protected:
	CXTPGalleryListBox* m_pParent;
};



/////////////////////////////////////////////////////////////////////////////
// CXTPRibbonBackstageList

CXTPGalleryListBox::CXTPGalleryListBox()
{
	m_pGallery = (CXTPControlGallery*)GetControls()->Add(new CXTPControlListBoxGallery(this));
	m_pGallery->SetTransparent(TRUE);

	m_pItems = new CXTPGalleryListBoxItems(this);

	m_pImageManager = m_pItems->GetImageManager();
	m_pImageManager->InternalAddRef();


	SetPaintManager(CXTPPaintManager::CreateTheme(xtpThemeRibbon));

	m_pGallery->SetScrollBarAuto(TRUE);

	m_ptLastMousePos = CPoint(-1, -1);
}

CXTPGalleryListBox::~CXTPGalleryListBox()
{
	CMDTARGET_RELEASE(m_pItems);
}




BEGIN_MESSAGE_MAP(CXTPGalleryListBox, CXTPCommandBar)
	//{{AFX_MSG_MAP(CXTPGalleryListBox)
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_KEYDOWN()
	ON_WM_GETDLGCODE()
	ON_WM_MOUSEWHEEL()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CXTPRibbonBackstageList message handlers

void CXTPGalleryListBox::RecalcLayout()
{
	if (!m_hWnd)
		return;

	CXTPClientRect rc(this);

	m_pGallery->SetRect(rc);
}

void CXTPGalleryListBox::FillCommandBarEntry(CDC* pDC)
{
	CXTPClientRect rc(this);
	pDC->FillSolidRect(rc, GetXtremeColor(COLOR_WINDOW));
}


void CXTPGalleryListBox::OnSize(UINT nType, int cx, int cy)
{
	CXTPCommandBar::OnSize(nType, cx, cy);

	RecalcLayout();
}

void CXTPGalleryListBox::OnLButtonDown(UINT nFlags, CPoint point)
{
	SetFocus();

	CXTPCommandBar::OnLButtonDown(nFlags, point);
}

void CXTPGalleryListBox::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (OnHookKeyDown(nChar, MAKELPARAM(nRepCnt, nFlags)))
		return;
}

UINT CXTPGalleryListBox::OnGetDlgCode()
{
	return DLGC_WANTALLKEYS;
}

BOOL CXTPGalleryListBox::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	m_pGallery->OnHookMouseWheel(nFlags, zDelta, pt);

	return TRUE;
}

void CXTPGalleryListBox::OnExecute()
{
	GetOwner()->SendMessage(WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(),
		BN_CLICKED), (LPARAM)m_hWnd);
}


void CXTPGalleryListBox::OnSetFocus(CWnd* pOldWnd)
{
	CWnd::OnSetFocus(pOldWnd);

}

void CXTPGalleryListBox::OnKillFocus(CWnd* pNewWnd)
{
	CWnd::OnKillFocus(pNewWnd);

}



BOOL CXTPGalleryListBox::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{

	return CXTPCommandBar::OnWndMsg(message, wParam, lParam, pResult);
}

void CXTPGalleryListBox::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_ptLastMousePos == point)
		return;

	m_ptLastMousePos = point;

	CXTPCommandBar::OnMouseMove(nFlags, point);
}

/////////////////////////////////////////////////////////////////////////////
//


class CXTPRecentFileListBoxItem : public CXTPControlGalleryItem
{
public:
	CXTPRecentFileListBoxItem(CXTPRecentFileListItem* pRecentItem);

public:
	virtual CSize GetSize();
	virtual void Draw(CDC* pDC, CXTPControlGallery* pGallery, CRect rcItem, BOOL bEnabled, BOOL bSelected, BOOL bPressed, BOOL bChecked);


protected:
	CXTPRecentFileListItem* m_pRecentItem;
};

class CXTPRecentFileListBoxPinItem : public CXTPControlGalleryItem
{
public:
	CXTPRecentFileListBoxPinItem(CXTPRecentFileListItem* pRecentItem);

public:
	virtual CSize GetSize();
	virtual BOOL OnClick();
	virtual void Draw(CDC* pDC, CXTPControlGallery* pGallery, CRect rcItem, BOOL bEnabled, BOOL bSelected, BOOL bPressed, BOOL bChecked);

protected:
	CXTPRecentFileListItem* m_pRecentItem;

};



//////////////////////////////////////////////////////////////////////////
// CXTPRecentFileListBoxPinItem

CXTPRecentFileListBoxPinItem::CXTPRecentFileListBoxPinItem(CXTPRecentFileListItem* pRecentItem)
{
	m_pRecentItem = pRecentItem;
	SetData((DWORD_PTR)pRecentItem);
}


CSize CXTPRecentFileListBoxPinItem::GetSize()
{
	return CSize(20, 36);
}

BOOL CXTPRecentFileListBoxPinItem::OnClick()
{
	m_pRecentItem->SetPinned(!m_pRecentItem->IsPinned());


	CXTPGalleryListBoxItems* pItems = (CXTPGalleryListBoxItems*)m_pItems;

	CXTPRecentFileListBox* pListBox = (CXTPRecentFileListBox*)pItems->GetListBox();

	pListBox->BuildItems(m_pRecentItem->GetList());

	return TRUE;
}


void CXTPRecentFileListBoxPinItem::Draw(CDC* pDC, CXTPControlGallery* pGallery, CRect rcItem, BOOL bEnabled, BOOL bSelected, BOOL bPressed, BOOL bChecked)
{
	CXTPPaintManager* pPaintManager = pGallery->GetPaintManager();

	if (bSelected && m_pRecentItem->IsPinned())
		bPressed = TRUE;

	pPaintManager->DrawRectangle(pDC, rcItem, bSelected, bPressed, bEnabled, bChecked, FALSE, xtpBarTypeNormal, xtpBarPopup);

	CXTPGalleryListBoxItems* pItems = (CXTPGalleryListBoxItems*)m_pItems;

	CXTPRecentFileListBox* pListBox = (CXTPRecentFileListBox*)pItems->GetListBox();

	CXTPImageManagerIcon* pIcon = pListBox->GetImageManager()->GetImage(m_pRecentItem->IsPinned() ? ID_ICON_PINNED : ID_ICON_UNPINNED);

	if (pIcon)
	{
		pIcon->Draw(pDC, CPoint((rcItem.left + rcItem.right - pIcon->GetExtent().cx) / 2, (rcItem.top + rcItem.bottom - pIcon->GetExtent().cy) / 2));
	}
}


//////////////////////////////////////////////////////////////////////////
// CXTPGalleryListBoxSeparatorItem

CXTPGalleryListBoxSeparatorItem::CXTPGalleryListBoxSeparatorItem()
{
	m_bLabel = TRUE;
}

CSize CXTPGalleryListBoxSeparatorItem::GetSize()
{
	if (m_szItem.cy != 0)
		return CSize(0, m_szItem.cy);

	return CSize(0, 14);
}

void CXTPGalleryListBoxSeparatorItem::Draw(CDC* pDC, CXTPControlGallery* /*pGallery*/, CRect rcItem, BOOL /*bEnabled*/, BOOL /*bSelected*/, BOOL /*bPressed*/, BOOL /*bChecked*/)
{
	const unsigned short _cb[] = {0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc};

	CBitmap bmp;
	bmp.CreateBitmap(8, 8, 1, 1, _cb);

	CBrush brush;
	brush.CreatePatternBrush(&bmp);

	CBrush* pOldBrush = (CBrush*)pDC->SelectObject(&brush);

	pDC->SetTextColor(RGB(255, 255, 255));
	pDC->SetBkColor(RGB(199, 200, 201));

	PatBlt(pDC->GetSafeHdc(), rcItem.left, (rcItem.top + rcItem.bottom) / 2, rcItem.Width() - 10, 1, PATCOPY);

	pDC->SelectObject(pOldBrush);
}

//////////////////////////////////////////////////////////////////////////
// CXTPRibbonBackstageRecentFileListItem

CXTPRecentFileListBoxItem::CXTPRecentFileListBoxItem(CXTPRecentFileListItem* pRecentItem)
{
	m_pRecentItem = pRecentItem;
	SetData((DWORD_PTR)pRecentItem);

	m_strCaption = m_pRecentItem->GetPathName();

	SetToolTip(m_strCaption);
}

CSize CXTPRecentFileListBoxItem::GetSize()
{
	CXTPGalleryListBoxItems* pItems = (CXTPGalleryListBoxItems*)m_pItems;

	CXTPGalleryListBox* pListBox = pItems->GetListBox();
	if (!pListBox->GetSafeHwnd())
		return 0;

	CXTPControlGallery* pGallery = pListBox->GetGallery();

	CRect rcItems = pGallery->GetItemsRect();

	return CSize(rcItems.Width() - 30, 36);
}

void CXTPRecentFileListBoxItem::Draw(CDC* pDC, CXTPControlGallery* pGallery, CRect rcItem, BOOL bEnabled, BOOL bSelected, BOOL bPressed, BOOL bChecked)
{
	CXTPPaintManager* pPaintManager = pGallery->GetPaintManager();

	CXTPImageManagerIcon* pImage = GetImage();

	pPaintManager->DrawRectangle(pDC, rcItem, bSelected, bPressed, bEnabled, bChecked, FALSE, xtpBarTypeNormal, xtpBarPopup);

	CSize szImage(32, 32);

	if (pImage)
	{
		CPoint pt(rcItem.left + 7, rcItem.top + 2);

		pImage->Draw(pDC, pt, CSize(32, 32));
	}
	else
	{
		SHFILEINFO sfi;
		if (SUCCEEDED(::SHGetFileInfo(m_strCaption, 0, &sfi, sizeof(SHFILEINFO), SHGFI_LARGEICON | SHGFI_ICON)) && sfi.hIcon)
		{
			DrawIconEx(pDC->GetSafeHdc(), rcItem.left + 7, rcItem.top + 2, sfi.hIcon, 32, 32, 0, 0, DI_NORMAL);

			DestroyIcon(sfi.hIcon);
		}

	}

	CRect rcText(rcItem);
	rcText.DeflateRect(50, 2, 2, 2);

	CRect rcFileName(rcText.left, rcText.top, rcText.right, (rcText.bottom + rcText.top) / 2);
	CRect rcFileFolder(rcText.left, rcFileName.bottom, rcText.right, rcText.bottom);

	int nIndex = m_strCaption.ReverseFind(_T('\\'));

	CString strFileName;
	CString strFileFolder;
	if (nIndex != -1)
	{
		strFileName = m_strCaption.Mid(nIndex + 1);
		strFileFolder = m_strCaption.Left(nIndex);
	}
	else
	{
		strFileName = m_strCaption;
	}

	CXTPFontDC font(pDC, pPaintManager->GetRegularFont());

	pDC->SetTextColor(RGB(38, 38, 38));
	pDC->DrawText(strFileName, rcFileName, DT_END_ELLIPSIS | DT_LEFT | DT_NOPREFIX | DT_VCENTER | DT_SINGLELINE);

	pDC->SetTextColor(RGB(111, 111, 111));
	pDC->DrawText(strFileFolder, rcFileFolder, DT_END_ELLIPSIS | DT_LEFT | DT_NOPREFIX | DT_VCENTER | DT_SINGLELINE);
}


//////////////////////////////////////////////////////////////////////////
// CXTPRecentFileListBox


CXTPRecentFileListBox::CXTPRecentFileListBox()
{
	m_pToolTipContext = new CXTPToolTipContext();


	UINT nID[] = {0, ID_ICON_UNPINNED, ID_ICON_PINNED};

	GetImageManager()->SetIcons(XTP_IDB_RIBBON_PINICON, nID, 3, CSize(16, 16), xtpImageNormal);
}

CRecentFileList* CXTPRecentFileListBox::GetRecentFileList() const
{
	USES_PROTECTED_ACCESS(CXTPRecentFileListBox, CWinApp, CRecentFileList*, m_pRecentFileList)
	return PROTECTED_ACCESS(CWinApp, AfxGetApp(), m_pRecentFileList);
}

void CXTPRecentFileListBox::BuildItems()
{
	CXTPRecentFileList* pRecentFileList = (CXTPRecentFileList*)GetRecentFileList();
	ASSERT (pRecentFileList && pRecentFileList->m_strOriginal == _T("PinableRecentFileList"));

	if (pRecentFileList && pRecentFileList->m_strOriginal == _T("PinableRecentFileList"))
	{
		BuildItems((CXTPRecentFileList*)pRecentFileList);
	}

}
void CXTPRecentFileListBox::BuildItems(CXTPRecentFileList* pRecentFileList)
{
	ASSERT (pRecentFileList);
	if (!pRecentFileList)
		return;

	DWORD_PTR dwSelectedRecentItem = 0;

	CXTPControlGalleryItem* pSelectedItem = GetGallery()->GetItem(GetGallery()->GetSelectedItem());
	if (pSelectedItem)
	{
		dwSelectedRecentItem = pSelectedItem->GetData();
	}
	pSelectedItem = NULL;

	m_pItems->RemoveAll();

	int nIndex;
	BOOL bAllPinned = TRUE;

	for (nIndex = 0; nIndex < pRecentFileList->m_nSize; nIndex++)
	{
		CXTPRecentFileListItem* pItem = pRecentFileList->GetItem(nIndex);
		if (!pItem)
			break;

		if (pItem->IsPinned())
		{
			CXTPControlGalleryItem* p = m_pItems->AddItem(new CXTPRecentFileListBoxItem(pItem));
			p->SetID(ID_FILE_MRU_FILE1 + nIndex);

			m_pItems->AddItem(new CXTPRecentFileListBoxPinItem(pItem));

			if ((DWORD_PTR)pItem == dwSelectedRecentItem)
			{
				pSelectedItem = p;
			}
		}
		else
		{
			bAllPinned = FALSE;
		}
	}

	if (m_pItems->GetItemCount() && !bAllPinned)
	{
		m_pItems->AddItem(new CXTPGalleryListBoxSeparatorItem());
	}

	for (nIndex = 0; nIndex < pRecentFileList->m_nSize; nIndex++)
	{
		CXTPRecentFileListItem* pItem = pRecentFileList->GetItem(nIndex);
		if (!pItem)
			break;

		if (!pItem->IsPinned())
		{
			CXTPControlGalleryItem* p = m_pItems->AddItem(new CXTPRecentFileListBoxItem(pItem));
			p->SetID(ID_FILE_MRU_FILE1 + nIndex);

			m_pItems->AddItem(new CXTPRecentFileListBoxPinItem(pItem));

			if ((DWORD_PTR)pItem == dwSelectedRecentItem)
			{
				pSelectedItem = p;
			}
		}
	}

	if (pSelectedItem)
	{
		GetGallery()->SetSelectedItem(pSelectedItem->GetIndex());
	}

	Redraw(0, FALSE);
}


