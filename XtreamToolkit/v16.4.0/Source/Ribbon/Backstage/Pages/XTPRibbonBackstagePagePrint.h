// XTPRibbonBackstagePagePrint.h: interface for the CXTPRibbonBackstagePagePrint class.
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
#if !defined(__XTPRIBBONBACKSTAGEPAGEPRINT_H__)
#define __XTPRIBBONBACKSTAGEPAGEPRINT_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CXTPRibbonBackstagePagePrint dialog

class _XTP_EXT_CLASS CXTPRibbonBackstagePagePrint : public CXTPRibbonBackstagePage
{
	DECLARE_DYNCREATE(CXTPRibbonBackstagePagePrint)

// Construction/Destruction
public:
	CXTPRibbonBackstagePagePrint();
	virtual ~CXTPRibbonBackstagePagePrint();

	//{{AFX_DATA(CXTPRibbonBackstagePagePrint)
	enum { IDD = XTP_IDD_RIBBONBACKSTAGEPAGE_PRINT };
	int m_nCopies;
	//}}AFX_DATA

public:
	CXTPRibbonBackstageButton m_btnPrint;
	CXTPRibbonBackstageSeparator m_lblSeparator4;
	CXTPRibbonBackstageSeparator m_lblSeparator1;
	void InitButton(UINT nID);

	XTPControlTheme nTheme;

	CXTPRibbonBackstageLabel m_lblPrint;

	BOOL OnSetActive();
	BOOL OnKillActive();

	class CBackstagePreviewView : CXTPPreviewView
	{

		friend class CXTPRibbonBackstagePagePrint;
	};

	class CBackstagePrintView : CView
	{

		friend class CXTPRibbonBackstagePagePrint;
	};

	CBackstagePreviewView* m_pPreviewView;
	CFrameWnd* m_pFrameWnd;
	CBackstagePrintView* m_pView;

	void UpdateCopies(BOOL bSaveAndValidate);

	BOOL CreatePrintPreview();

	//{{AFX_VIRTUAL(CXTPRibbonBackstagePagePrint)
	protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange *pDX);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CXTPRibbonBackstagePagePrint)
	afx_msg void OnButtonPrint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}

#endif // !defined(__XTPRIBBONBACKSTAGEPAGEPRINT_H__)
