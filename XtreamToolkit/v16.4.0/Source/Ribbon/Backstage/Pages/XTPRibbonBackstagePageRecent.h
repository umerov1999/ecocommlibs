// XTPRibbonBackstagePageRecent.h: interface for the CXTPRibbonBackstagePageRecent class.
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

//{{AFX_CODEJOCK_PRIVATE
#if !defined(__XTPRIBBONBACKSTAGEPAGERECENT_H__)
#define __XTPRIBBONBACKSTAGEPAGERECENT_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXTPRecentFileListBoxItem;

/////////////////////////////////////////////////////////////////////////////
// CXTPRibbonBackstagePageRecent dialog

#define XTP_IDC_RIBBONBACKSTAGEPAGE_RECENT XTP_IDD_RIBBONBACKSTAGEPAGE_RECENT

const UINT XTP_RRN_CONTEXTMENU = 0x1000;

typedef struct tagNMXTPRRNCONTEXTMENU {
	NMHDR hdr;   // NMHDR structure that contains additional information about this notification.
	POINT pt;    // Point in screen coordinate where an event has been triggered
	CXTPRecentFileListBoxItem* pItem; // Clicked item pointer
} NMXTPRRNCONTEXTMENU, *LPNMXTPRRNCONTEXTMENU;

class _XTP_EXT_CLASS CXTPRibbonBackstagePageRecent : public CXTPRibbonBackstagePage
{
	DECLARE_DYNCREATE(CXTPRibbonBackstagePageRecent)

// Construction
public:
	CXTPRibbonBackstagePageRecent();
	virtual ~CXTPRibbonBackstagePageRecent();

	void RemoveItem(CXTPRecentFileListBoxItem* pItem);
	void PinItem(CXTPRecentFileListBoxItem* pItem);
	void UnpinItem(CXTPRecentFileListBoxItem* pItem);

	//{{AFX_DATA(CXTPRibbonBackstagePageRecent)
	enum { IDD = XTP_IDD_RIBBONBACKSTAGEPAGE_RECENT };
	//}}AFX_DATA


	//{{AFX_VIRTUAL(CXTPRibbonBackstagePageRecent)
protected:
	virtual void DoDataExchange(CDataExchange *pDX);
	//}}AFX_VIRTUAL

	BOOL OnSetActive();


	CXTPRibbonBackstageSeparator m_lblSeparator1;
	CXTPRibbonBackstageLabel m_lblRecent;

	CXTPRecentFileListBox m_wndList;

protected:
	//{{AFX_MSG(CXTPRibbonBackstagePageRecent)
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_MSG
	afx_msg void OnRecentFileListClick();
	DECLARE_MESSAGE_MAP()

private:
	CWnd* m_pCreatedOnParent;

};

//{{AFX_INSERT_LOCATION}}

#endif // !defined(__XTPRIBBONBACKSTAGEPAGERECENT_H__)
