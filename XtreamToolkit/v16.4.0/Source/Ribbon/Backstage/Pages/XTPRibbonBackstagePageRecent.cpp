// XTPRibbonBackstagePageRecent.cpp : implementation file
//
// This file is a part of the XTREME RIBBON MFC class library.
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

#include <StdAfx.h>

#include <Common/XTPDrawHelpers.h>
#include <Common/XTPResourceImage.h>
#include <Common/XTPVC80Helpers.h>
#include <Common/XTPColorManager.h>
#include <Common/XTPWinThemeWrapper.h>
#include <Common/XTPSystemHelpers.h>
#include <Common/XTPImageManager.h>
#include <Common/XTPHookManager.h>
#include <Common/ScrollBar/XTPScrollBase.h>

#include <Controls/Util/XTPControlTheme.h>
#include <Controls/Button/XTPButton.h>
#include <Controls/Resize/XTPResizeRect.h>
#include <Controls/Resize/XTPResizePoint.h>
#include <Controls/Resize/XTPResize.h>
#include <Controls/Resize/XTPResizePropertyPage.h>

#include <CommandBars/Resource.h>
#include <CommandBars/XTPCommandBarsDefines.h>
#include <CommandBars/XTPCommandBars.h>
#include <CommandBars/XTPCommandBar.h>
#include <CommandBars/XTPToolBar.h>
#include <CommandBars/XTPControls.h>
#include <CommandBars/XTPControl.h>
#include <CommandBars/XTPControlButton.h>
#include <CommandBars/XTPControlPopup.h>
#include <CommandBars/XTPControlGallery.h>
#include <CommandBars/XTPPreviewView.h>
#include <CommandBars/XTPRecentFileList.h>

#include <CommandBars/Gallery/XTPControlGalleryItem.h>
#include <CommandBars/Gallery/XTPControlGalleryItems.h>
#include <CommandBars/Gallery/XTPGalleryListBox.h>

#include <Ribbon/Resource.h>
#include <Ribbon/Backstage/XTPRibbonBackstageButton.h>
#include <Ribbon/Backstage/XTPRibbonBackstageLabel.h>
#include <Ribbon/Backstage/XTPRibbonBackstageSeparator.h>
#include <Ribbon/Backstage/XTPRibbonBackstagePage.h>
#include <Ribbon/Backstage/XTPRibbonBackstageView.h>
#include <Ribbon/Backstage/Pages/XTPRibbonBackstagePageRecent.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTPRibbonBackstagePageRecent property page

IMPLEMENT_DYNCREATE(CXTPRibbonBackstagePageRecent, CXTPRibbonBackstagePage)

CXTPRibbonBackstagePageRecent::CXTPRibbonBackstagePageRecent() 
	: CXTPRibbonBackstagePage(CXTPRibbonBackstagePageRecent::IDD)
	, m_pCreatedOnParent(NULL)
{
	//{{AFX_DATA_INIT(CXTPRibbonBackstagePageRecent)
	//}}AFX_DATA_INIT

	//XTPControlTheme nTheme = xtpControlThemeOffice2013;

	//m_wndList.SetTheme(nTheme);
}

CXTPRibbonBackstagePageRecent::~CXTPRibbonBackstagePageRecent()
{
}

void CXTPRibbonBackstagePageRecent::RemoveItem(CXTPRecentFileListBoxItem* pItem)
{
	ASSERT(NULL != pItem);

	CRecentFileList* pFileList = m_wndList.GetRecentFileList();
	int nFileCount = pFileList->GetSize();
	for (int nFile = 0; nFile < nFileCount; ++nFile)
	{
		if ((*pFileList)[nFile] == pItem->GetCaption())
		{
			pItem->GetRecentItem()->SetPinned(FALSE);
			pFileList->Remove(nFile);
			m_wndList.BuildItems();
			break;
		}
	}
}

void CXTPRibbonBackstagePageRecent::PinItem(CXTPRecentFileListBoxItem* pItem)
{
	ASSERT(NULL != pItem);
	if (!pItem->GetRecentItem()->IsPinned())
	{
		pItem->GetRecentItem()->SetPinned(TRUE);
		m_wndList.BuildItems();
	}
}

void CXTPRibbonBackstagePageRecent::UnpinItem(CXTPRecentFileListBoxItem* pItem)
{
	ASSERT(NULL != pItem);
	if (pItem->GetRecentItem()->IsPinned())
	{
		pItem->GetRecentItem()->SetPinned(FALSE);
		m_wndList.BuildItems();
	}
}

void CXTPRibbonBackstagePageRecent::DoDataExchange(CDataExchange* pDX)
{
	CXTPRibbonBackstagePage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CXTPRibbonBackstagePageRecent)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

	DDX_Control(pDX, XTP_IDC_RIBBONBACKSTAGE_SEPARATOR_1, m_lblSeparator1);
	DDX_Control(pDX, XTP_IDC_RIBBONBACKSTAGE_CAPTION, m_lblRecent);

	DDX_Control(pDX, XTP_IDC_RIBBONBACKSTAGE_RECENTFILELIST, m_wndList);

}


BEGIN_MESSAGE_MAP(CXTPRibbonBackstagePageRecent, CXTPRibbonBackstagePage)
	//{{AFX_MSG_MAP(CXTPRibbonBackstagePageRecent)
	//}}AFX_MSG_MAP

	ON_BN_CLICKED(XTP_IDC_RIBBONBACKSTAGE_RECENTFILELIST, OnRecentFileListClick)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTPRibbonBackstagePageRecent message handlers

BOOL CXTPRibbonBackstagePageRecent::OnInitDialog()
{
	CXTPRibbonBackstagePage::OnInitDialog();

	m_pCreatedOnParent = GetParent();

	SetDlgCtrlID(XTP_IDC_RIBBONBACKSTAGEPAGE_RECENT);
	ModifyStyleEx(0, WS_EX_CONTROLPARENT);


	m_lblRecent.SetFont(&m_fntCaption);
	m_lblRecent.SetTextColor(RGB(94, 94, 94));


	SetResize(XTP_IDC_RIBBONBACKSTAGE_SEPARATOR_1, XTP_ANCHOR_TOPLEFT, XTP_ANCHOR_TOPRIGHT);
	SetResize(XTP_IDC_RIBBONBACKSTAGE_RECENTFILELIST, XTP_ANCHOR_TOPLEFT, XTP_ANCHOR_BOTTOMRIGHT);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CXTPRibbonBackstagePageRecent::PreTranslateMessage(MSG* pMsg)
{
	BOOL bTranslated = FALSE;
	if (WM_RBUTTONUP == pMsg->message && NULL != m_pCreatedOnParent)
	{
		CPoint ptCursor(XTP_GET_X_LPARAM(pMsg->lParam), XTP_GET_Y_LPARAM(pMsg->lParam));
		::ClientToScreen(pMsg->hwnd, &ptCursor);
		CXTPControlGalleryItems* pItems = m_wndList.GetItems();
		if (NULL != pItems)
		{
			int nCount = pItems->GetItemCount();
			for (int i = 0; i < nCount; ++i)
			{
				CXTPRecentFileListBoxItem* pItem = DYNAMIC_DOWNCAST(CXTPRecentFileListBoxItem, pItems->GetItem(i));
				if (NULL != pItem)
				{
					CRect rcItem = m_wndList.GetGallery()->GetItemDrawRect(i);
					m_wndList.ClientToScreen(&rcItem);
					if (rcItem.PtInRect(ptCursor))
					{
						NMXTPRRNCONTEXTMENU nm;
						nm.hdr.hwndFrom = m_hWnd;
						nm.hdr.idFrom = GetDlgCtrlID();
						nm.hdr.code = XTP_RRN_CONTEXTMENU;
						nm.pt = ptCursor;
						nm.pItem = pItem;
						m_pCreatedOnParent->SendMessage(WM_NOTIFY, nm.hdr.idFrom, reinterpret_cast<LPARAM>(&nm.hdr));
						bTranslated = TRUE;
						break;
					}
				}
			}
		}
	}

	return bTranslated ? TRUE : CXTPRibbonBackstagePage::PreTranslateMessage(pMsg);
}

BOOL CXTPRibbonBackstagePageRecent::OnSetActive()
{
	m_wndList.BuildItems();


	return TRUE;
}

void CXTPRibbonBackstagePageRecent::OnRecentFileListClick()
{
	CXTPRibbonBackstageView* pView = (CXTPRibbonBackstageView*)GetParent();

	pView->OnCancel();


	CXTPControlGalleryItem* pItem = m_wndList.GetGallery()->GetItem(m_wndList.GetGallery()->GetSelectedItem());

	if (pItem)
	{
		AfxGetMainWnd()->SendMessage(WM_COMMAND, pItem->GetID());
	}
}
